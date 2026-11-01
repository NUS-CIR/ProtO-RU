// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/e1ap/e1ap_pdu_contents.h"

using namespace asn1;
using namespace asn1::e1ap;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// BCBearerContextModificationConfirmIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_mod_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 163, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bc_bearer_context_mod_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 163, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_mod_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 163:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_mod_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 163:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_mod_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 163, crit_e{crit_e::reject}, [this, &bref]() { return bc_bearer_context_to_modify_confirm.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_mod_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_mod_confirm_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 163: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return bc_bearer_context_to_modify_confirm.unpack(bref); }));
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
void bc_bearer_context_mod_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 163);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  bc_bearer_context_to_modify_confirm.to_json(j);
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

// BCBearerContextModificationFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 0, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bc_bearer_context_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_mod_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
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
presence_e bc_bearer_context_mod_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
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

OCUDUASN_CODE bc_bearer_context_mod_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_mod_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_mod_fail_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
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
void bc_bearer_context_mod_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
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

// BCBearerContextModificationRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 160};
  return map_enum_number(names, 3, idx, "id");
}
bool bc_bearer_context_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 160};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 160:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 160:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 160, crit_e{crit_e::reject}, [this, &bref]() { return bc_bearer_context_to_modify.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_mod_request_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 160: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return bc_bearer_context_to_modify.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void bc_bearer_context_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 160);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  bc_bearer_context_to_modify.to_json(j);
  j.end_obj();
  j.end_array();
}

// BCBearerContextModificationRequiredIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_mod_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 162};
  return map_enum_number(names, 3, idx, "id");
}
bool bc_bearer_context_mod_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 162};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_mod_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 162:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_mod_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 162:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_mod_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 162, crit_e{crit_e::reject}, [this, &bref]() { return bc_bearer_context_to_modify_required.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_mod_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_mod_required_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 162: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return bc_bearer_context_to_modify_required.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void bc_bearer_context_mod_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 162);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  bc_bearer_context_to_modify_required.to_json(j);
  j.end_obj();
  j.end_array();
}

// BCBearerContextModificationResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 161, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bc_bearer_context_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 161, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 161:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 161:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 161, crit_e{crit_e::reject}, [this, &bref]() { return bc_bearer_context_to_modify_resp.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_mod_resp_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 161: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return bc_bearer_context_to_modify_resp.unpack(bref); }));
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
void bc_bearer_context_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 161);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  bc_bearer_context_to_modify_resp.to_json(j);
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

// BCBearerContextReleaseCommandIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool bc_bearer_context_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_release_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_release_cmd_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
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
void bc_bearer_context_release_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// BCBearerContextReleaseCompleteIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_release_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 1};
  return map_enum_number(names, 3, idx, "id");
}
bool bc_bearer_context_release_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_release_complete_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_release_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_release_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_release_complete_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_release_complete_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
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
void bc_bearer_context_release_complete_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
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

// BCBearerContextReleaseRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool bc_bearer_context_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_release_request_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
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
void bc_bearer_context_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// BCBearerContextSetupFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 0, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bc_bearer_context_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += gnb_cu_up_mbs_e1ap_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  if (gnb_cu_up_mbs_e1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_setup_fail_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        gnb_cu_up_mbs_e1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
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
void bc_bearer_context_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  if (gnb_cu_up_mbs_e1ap_id_present) {
    j.start_obj();
    j.write_int("id", 156);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_up_mbs_e1ap_id);
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
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BCBearerContextSetupRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 157, 158, 199, 200};
  return map_enum_number(names, 5, idx, "id");
}
bool bc_bearer_context_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 157, 158, 199, 200};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 157:
      return crit_e::reject;
    case 158:
      return crit_e::reject;
    case 199:
      return crit_e::ignore;
    case 200:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 157:
      return presence_e::mandatory;
    case 158:
      return presence_e::mandatory;
    case 199:
      return presence_e::optional;
    case 200:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += associated_session_id_present ? 1 : 0;
  nof_ies += mbs_service_area_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 157, crit_e{crit_e::reject}, [this, &bref]() { return global_mbs_session_id.pack(bref); }));
  HANDLE_CODE(
      pack_ie(bref, 158, crit_e{crit_e::reject}, [this, &bref]() { return bc_bearer_context_to_setup.pack(bref); }));
  if (associated_session_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 199, crit_e{crit_e::ignore}, [this, &bref]() { return associated_session_id.pack(bref); }));
  }
  if (mbs_service_area_present) {
    HANDLE_CODE(pack_ie(bref, 200, crit_e{crit_e::ignore}, [this, &bref]() { return mbs_service_area.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_setup_request_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 157: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_mbs_session_id.unpack(bref); }));
        break;
      }
      case 158: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return bc_bearer_context_to_setup.unpack(bref); }));
        break;
      }
      case 199: {
        associated_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return associated_session_id.unpack(bref); }));
        break;
      }
      case 200: {
        mbs_service_area_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_service_area.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void bc_bearer_context_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 157);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 158);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  bc_bearer_context_to_setup.to_json(j);
  j.end_obj();
  if (associated_session_id_present) {
    j.start_obj();
    j.write_int("id", 199);
    j.write_str("criticality", "ignore");
    j.write_str("value", associated_session_id.to_string());
    j.end_obj();
  }
  if (mbs_service_area_present) {
    j.start_obj();
    j.write_int("id", 200);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mbs_service_area.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BCBearerContextSetupResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bc_bearer_context_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 159, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bc_bearer_context_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 159, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bc_bearer_context_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 159:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bc_bearer_context_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 159:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bc_bearer_context_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 159, crit_e{crit_e::reject}, [this, &bref]() { return bc_bearer_context_to_setup_resp.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bc_bearer_context_setup_resp_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 159: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return bc_bearer_context_to_setup_resp.unpack(bref); }));
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
void bc_bearer_context_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 159);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  bc_bearer_context_to_setup_resp.to_json(j);
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

// BearerContextInactivityNotificationIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_inactivity_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 24};
  return map_enum_number(names, 3, idx, "id");
}
bool bearer_context_inactivity_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 24};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_inactivity_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 24:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_inactivity_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 24:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_inactivity_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 24, crit_e{crit_e::reject}, [this, &bref]() { return activity_info.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_inactivity_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_inactivity_notif_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 24: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return activity_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void bearer_context_inactivity_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 24);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  activity_info.to_json(j);
  j.end_obj();
  j.end_array();
}

// EUTRAN-BearerContextModificationConfirm ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t eutran_bearer_context_mod_confirm_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {41};
  return map_enum_number(names, 1, idx, "id");
}
bool eutran_bearer_context_mod_confirm_o::is_id_valid(const uint32_t& id)
{
  return 41 == id;
}
crit_e eutran_bearer_context_mod_confirm_o::get_crit(const uint32_t& id)
{
  if (id == 41) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e eutran_bearer_context_mod_confirm_o::get_presence(const uint32_t& id)
{
  if (id == 41) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE eutran_bearer_context_mod_confirm_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += drb_confirm_modified_list_eutran_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (drb_confirm_modified_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_confirm_modified_list_eutran, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_bearer_context_mod_confirm_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &eutran_bearer_context_mod_confirm_o::get_crit));

    if (id == 41) {
      {
        drb_confirm_modified_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_confirm_modified_list_eutran, bref, 1, 32, true); }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void eutran_bearer_context_mod_confirm_container::to_json(json_writer& j) const
{
  j.start_array();
  if (drb_confirm_modified_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 41);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drb_confirm_modified_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// NG-RAN-BearerContextModificationConfirm ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ng_ran_bearer_context_mod_confirm_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {50};
  return map_enum_number(names, 1, idx, "id");
}
bool ng_ran_bearer_context_mod_confirm_o::is_id_valid(const uint32_t& id)
{
  return 50 == id;
}
crit_e ng_ran_bearer_context_mod_confirm_o::get_crit(const uint32_t& id)
{
  if (id == 50) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e ng_ran_bearer_context_mod_confirm_o::get_presence(const uint32_t& id)
{
  if (id == 50) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE ng_ran_bearer_context_mod_confirm_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pdu_session_res_confirm_modified_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (pdu_session_res_confirm_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_confirm_modified_list, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_bearer_context_mod_confirm_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_ran_bearer_context_mod_confirm_o::get_crit));

    if (id == 50) {
      {
        pdu_session_res_confirm_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_confirm_modified_list, bref, 1, 256, true);
        }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void ng_ran_bearer_context_mod_confirm_container::to_json(json_writer& j) const
{
  j.start_array();
  if (pdu_session_res_confirm_modified_list_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_confirm_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// System-BearerContextModificationConfirm ::= CHOICE
void sys_bearer_context_mod_confirm_c::destroy_()
{
  switch (type_) {
    case types::e_utran_bearer_context_mod_confirm:
      c.destroy<eutran_bearer_context_mod_confirm_container>();
      break;
    case types::ng_ran_bearer_context_mod_confirm:
      c.destroy<ng_ran_bearer_context_mod_confirm_container>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void sys_bearer_context_mod_confirm_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::e_utran_bearer_context_mod_confirm:
      c.init<eutran_bearer_context_mod_confirm_container>();
      break;
    case types::ng_ran_bearer_context_mod_confirm:
      c.init<ng_ran_bearer_context_mod_confirm_container>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_confirm_c");
  }
}
sys_bearer_context_mod_confirm_c::sys_bearer_context_mod_confirm_c(const sys_bearer_context_mod_confirm_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::e_utran_bearer_context_mod_confirm:
      c.init(other.c.get<eutran_bearer_context_mod_confirm_container>());
      break;
    case types::ng_ran_bearer_context_mod_confirm:
      c.init(other.c.get<ng_ran_bearer_context_mod_confirm_container>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_confirm_c");
  }
}
sys_bearer_context_mod_confirm_c&
sys_bearer_context_mod_confirm_c::operator=(const sys_bearer_context_mod_confirm_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::e_utran_bearer_context_mod_confirm:
      c.set(other.c.get<eutran_bearer_context_mod_confirm_container>());
      break;
    case types::ng_ran_bearer_context_mod_confirm:
      c.set(other.c.get<ng_ran_bearer_context_mod_confirm_container>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_confirm_c");
  }

  return *this;
}
eutran_bearer_context_mod_confirm_container& sys_bearer_context_mod_confirm_c::set_e_utran_bearer_context_mod_confirm()
{
  set(types::e_utran_bearer_context_mod_confirm);
  return c.get<eutran_bearer_context_mod_confirm_container>();
}
ng_ran_bearer_context_mod_confirm_container& sys_bearer_context_mod_confirm_c::set_ng_ran_bearer_context_mod_confirm()
{
  set(types::ng_ran_bearer_context_mod_confirm);
  return c.get<ng_ran_bearer_context_mod_confirm_container>();
}
protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>&
sys_bearer_context_mod_confirm_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>>();
}
void sys_bearer_context_mod_confirm_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::e_utran_bearer_context_mod_confirm:
      j.write_fieldname("e-UTRAN-BearerContextModificationConfirm");
      c.get<eutran_bearer_context_mod_confirm_container>().to_json(j);
      break;
    case types::ng_ran_bearer_context_mod_confirm:
      j.write_fieldname("nG-RAN-BearerContextModificationConfirm");
      c.get<ng_ran_bearer_context_mod_confirm_container>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_confirm_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sys_bearer_context_mod_confirm_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::e_utran_bearer_context_mod_confirm:
      HANDLE_CODE(c.get<eutran_bearer_context_mod_confirm_container>().pack(bref));
      break;
    case types::ng_ran_bearer_context_mod_confirm:
      HANDLE_CODE(c.get<ng_ran_bearer_context_mod_confirm_container>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_confirm_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_bearer_context_mod_confirm_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::e_utran_bearer_context_mod_confirm:
      HANDLE_CODE(c.get<eutran_bearer_context_mod_confirm_container>().unpack(bref));
      break;
    case types::ng_ran_bearer_context_mod_confirm:
      HANDLE_CODE(c.get<ng_ran_bearer_context_mod_confirm_container>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_mod_confirm_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_confirm_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sys_bearer_context_mod_confirm_c::types_opts::to_string() const
{
  static const char* names[] = {
      "e-UTRAN-BearerContextModificationConfirm", "nG-RAN-BearerContextModificationConfirm", "choice-extension"};
  return convert_enum_idx(names, 3, value, "sys_bearer_context_mod_confirm_c::types");
}

// BearerContextModificationConfirmIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_mod_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 20, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bearer_context_mod_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 20, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_mod_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 20:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_mod_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 20:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_mod_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += sys_bearer_context_mod_confirm_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (sys_bearer_context_mod_confirm_present) {
    HANDLE_CODE(pack_ie(
        bref, 20, crit_e{crit_e::ignore}, [this, &bref]() { return sys_bearer_context_mod_confirm.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_mod_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_mod_confirm_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 20: {
        sys_bearer_context_mod_confirm_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sys_bearer_context_mod_confirm.unpack(bref); }));
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
void bearer_context_mod_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  if (sys_bearer_context_mod_confirm_present) {
    j.start_obj();
    j.write_int("id", 20);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sys_bearer_context_mod_confirm.to_json(j);
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

// BearerContextModificationFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 0, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bearer_context_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_mod_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
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
presence_e bearer_context_mod_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
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

OCUDUASN_CODE bearer_context_mod_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_mod_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_mod_fail_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
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
void bearer_context_mod_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
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

// EUTRAN-BearerContextModificationRequest ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t eutran_bearer_context_mod_request_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {51, 33, 34, 83, 84};
  return map_enum_number(names, 5, idx, "id");
}
bool eutran_bearer_context_mod_request_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {51, 33, 34, 83, 84};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e eutran_bearer_context_mod_request_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 51:
      return crit_e::reject;
    case 33:
      return crit_e::reject;
    case 34:
      return crit_e::reject;
    case 83:
      return crit_e::ignore;
    case 84:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e eutran_bearer_context_mod_request_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 51:
      return presence_e::optional;
    case 33:
      return presence_e::optional;
    case 34:
      return presence_e::optional;
    case 83:
      return presence_e::optional;
    case 84:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE eutran_bearer_context_mod_request_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += drb_to_setup_mod_list_eutran_present ? 1 : 0;
  nof_ies += drb_to_modify_list_eutran_present ? 1 : 0;
  nof_ies += drb_to_rem_list_eutran_present ? 1 : 0;
  nof_ies += sub_profile_id_for_rfp_present ? 1 : 0;
  nof_ies += add_rrm_prio_idx_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (drb_to_setup_mod_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 51, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_to_setup_mod_list_eutran, 1, 32, true);
    }));
  }
  if (drb_to_modify_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 33, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_to_modify_list_eutran, 1, 32, true);
    }));
  }
  if (drb_to_rem_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 34, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_to_rem_list_eutran, 1, 32, true);
    }));
  }
  if (sub_profile_id_for_rfp_present) {
    HANDLE_CODE(pack_ie(bref, 83, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sub_profile_id_for_rfp, (uint16_t)1u, (uint16_t)256u, true, true);
    }));
  }
  if (add_rrm_prio_idx_present) {
    HANDLE_CODE(pack_ie(bref, 84, crit_e{crit_e::ignore}, [this, &bref]() { return add_rrm_prio_idx.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_bearer_context_mod_request_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &eutran_bearer_context_mod_request_o::get_crit));

    switch (id) {
      case 51: {
        drb_to_setup_mod_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_to_setup_mod_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 33: {
        drb_to_modify_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_to_modify_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 34: {
        drb_to_rem_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_to_rem_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 83: {
        sub_profile_id_for_rfp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sub_profile_id_for_rfp, bref, (uint16_t)1u, (uint16_t)256u, true, true);
        }));
        break;
      }
      case 84: {
        add_rrm_prio_idx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return add_rrm_prio_idx.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void eutran_bearer_context_mod_request_container::to_json(json_writer& j) const
{
  j.start_array();
  if (drb_to_setup_mod_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 51);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drb_to_setup_mod_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drb_to_modify_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 33);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drb_to_modify_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drb_to_rem_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 34);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drb_to_rem_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sub_profile_id_for_rfp_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_int("value", sub_profile_id_for_rfp);
    j.end_obj();
  }
  if (add_rrm_prio_idx_present) {
    j.start_obj();
    j.write_int("id", 84);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    add_rrm_prio_idx.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// NG-RAN-BearerContextModificationRequest ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ng_ran_bearer_context_mod_request_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {56, 43, 44};
  return map_enum_number(names, 3, idx, "id");
}
bool ng_ran_bearer_context_mod_request_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {56, 43, 44};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_ran_bearer_context_mod_request_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 56:
      return crit_e::reject;
    case 43:
      return crit_e::reject;
    case 44:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_ran_bearer_context_mod_request_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 56:
      return presence_e::optional;
    case 43:
      return presence_e::optional;
    case 44:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_ran_bearer_context_mod_request_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pdu_session_res_to_setup_mod_list_present ? 1 : 0;
  nof_ies += pdu_session_res_to_modify_list_present ? 1 : 0;
  nof_ies += pdu_session_res_to_rem_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (pdu_session_res_to_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 56, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_to_setup_mod_list, 1, 256, true);
    }));
  }
  if (pdu_session_res_to_modify_list_present) {
    HANDLE_CODE(pack_ie(bref, 43, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_to_modify_list, 1, 256, true);
    }));
  }
  if (pdu_session_res_to_rem_list_present) {
    HANDLE_CODE(pack_ie(bref, 44, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_to_rem_list, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_bearer_context_mod_request_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_ran_bearer_context_mod_request_o::get_crit));

    switch (id) {
      case 56: {
        pdu_session_res_to_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_to_setup_mod_list, bref, 1, 256, true);
        }));
        break;
      }
      case 43: {
        pdu_session_res_to_modify_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_to_modify_list, bref, 1, 256, true); }));
        break;
      }
      case 44: {
        pdu_session_res_to_rem_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_to_rem_list, bref, 1, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void ng_ran_bearer_context_mod_request_container::to_json(json_writer& j) const
{
  j.start_array();
  if (pdu_session_res_to_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 56);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_to_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_to_modify_list_present) {
    j.start_obj();
    j.write_int("id", 43);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_to_modify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 44);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// System-BearerContextModificationRequest ::= CHOICE
void sys_bearer_context_mod_request_c::destroy_()
{
  switch (type_) {
    case types::e_utran_bearer_context_mod_request:
      c.destroy<eutran_bearer_context_mod_request_container>();
      break;
    case types::ng_ran_bearer_context_mod_request:
      c.destroy<ng_ran_bearer_context_mod_request_container>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void sys_bearer_context_mod_request_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::e_utran_bearer_context_mod_request:
      c.init<eutran_bearer_context_mod_request_container>();
      break;
    case types::ng_ran_bearer_context_mod_request:
      c.init<ng_ran_bearer_context_mod_request_container>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_request_c");
  }
}
sys_bearer_context_mod_request_c::sys_bearer_context_mod_request_c(const sys_bearer_context_mod_request_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::e_utran_bearer_context_mod_request:
      c.init(other.c.get<eutran_bearer_context_mod_request_container>());
      break;
    case types::ng_ran_bearer_context_mod_request:
      c.init(other.c.get<ng_ran_bearer_context_mod_request_container>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_request_c");
  }
}
sys_bearer_context_mod_request_c&
sys_bearer_context_mod_request_c::operator=(const sys_bearer_context_mod_request_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::e_utran_bearer_context_mod_request:
      c.set(other.c.get<eutran_bearer_context_mod_request_container>());
      break;
    case types::ng_ran_bearer_context_mod_request:
      c.set(other.c.get<ng_ran_bearer_context_mod_request_container>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_request_c");
  }

  return *this;
}
eutran_bearer_context_mod_request_container& sys_bearer_context_mod_request_c::set_e_utran_bearer_context_mod_request()
{
  set(types::e_utran_bearer_context_mod_request);
  return c.get<eutran_bearer_context_mod_request_container>();
}
ng_ran_bearer_context_mod_request_container& sys_bearer_context_mod_request_c::set_ng_ran_bearer_context_mod_request()
{
  set(types::ng_ran_bearer_context_mod_request);
  return c.get<ng_ran_bearer_context_mod_request_container>();
}
protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>&
sys_bearer_context_mod_request_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>>();
}
void sys_bearer_context_mod_request_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::e_utran_bearer_context_mod_request:
      j.write_fieldname("e-UTRAN-BearerContextModificationRequest");
      c.get<eutran_bearer_context_mod_request_container>().to_json(j);
      break;
    case types::ng_ran_bearer_context_mod_request:
      j.write_fieldname("nG-RAN-BearerContextModificationRequest");
      c.get<ng_ran_bearer_context_mod_request_container>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_request_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sys_bearer_context_mod_request_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::e_utran_bearer_context_mod_request:
      HANDLE_CODE(c.get<eutran_bearer_context_mod_request_container>().pack(bref));
      break;
    case types::ng_ran_bearer_context_mod_request:
      HANDLE_CODE(c.get<ng_ran_bearer_context_mod_request_container>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_request_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_bearer_context_mod_request_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::e_utran_bearer_context_mod_request:
      HANDLE_CODE(c.get<eutran_bearer_context_mod_request_container>().unpack(bref));
      break;
    case types::ng_ran_bearer_context_mod_request:
      HANDLE_CODE(c.get<ng_ran_bearer_context_mod_request_container>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_mod_request_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_request_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sys_bearer_context_mod_request_c::types_opts::to_string() const
{
  static const char* names[] = {
      "e-UTRAN-BearerContextModificationRequest", "nG-RAN-BearerContextModificationRequest", "choice-extension"};
  return convert_enum_idx(names, 3, value, "sys_bearer_context_mod_request_c::types");
}

// BearerContextModificationRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2,  3,  13,  14,  66,  17,  26,  59,  70,  18, 76,
                                   77, 23, 146, 150, 154, 174, 178, 187, 195, 196};
  return map_enum_number(names, 21, idx, "id");
}
bool bearer_context_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2,  3,  13,  14,  66,  17,  26,  59,  70,  18, 76,
                                   77, 23, 146, 150, 154, 174, 178, 187, 195, 196};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 13:
      return crit_e::reject;
    case 14:
      return crit_e::reject;
    case 66:
      return crit_e::reject;
    case 17:
      return crit_e::reject;
    case 26:
      return crit_e::reject;
    case 59:
      return crit_e::reject;
    case 70:
      return crit_e::ignore;
    case 18:
      return crit_e::reject;
    case 76:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 23:
      return crit_e::ignore;
    case 146:
      return crit_e::ignore;
    case 150:
      return crit_e::ignore;
    case 154:
      return crit_e::ignore;
    case 174:
      return crit_e::reject;
    case 178:
      return crit_e::ignore;
    case 187:
      return crit_e::ignore;
    case 195:
      return crit_e::ignore;
    case 196:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 13:
      return presence_e::optional;
    case 14:
      return presence_e::optional;
    case 66:
      return presence_e::optional;
    case 17:
      return presence_e::optional;
    case 26:
      return presence_e::optional;
    case 59:
      return presence_e::optional;
    case 70:
      return presence_e::optional;
    case 18:
      return presence_e::optional;
    case 76:
      return presence_e::optional;
    case 77:
      return presence_e::optional;
    case 23:
      return presence_e::optional;
    case 146:
      return presence_e::optional;
    case 150:
      return presence_e::optional;
    case 154:
      return presence_e::optional;
    case 174:
      return presence_e::optional;
    case 178:
      return presence_e::optional;
    case 187:
      return presence_e::optional;
    case 195:
      return presence_e::optional;
    case 196:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += security_info_present ? 1 : 0;
  nof_ies += ue_dl_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += ue_dl_max_integrity_protected_data_rate_present ? 1 : 0;
  nof_ies += bearer_context_status_change_present ? 1 : 0;
  nof_ies += new_ul_tnl_info_required_present ? 1 : 0;
  nof_ies += ue_inactivity_timer_present ? 1 : 0;
  nof_ies += data_discard_required_present ? 1 : 0;
  nof_ies += sys_bearer_context_mod_request_present ? 1 : 0;
  nof_ies += ran_ue_id_present ? 1 : 0;
  nof_ies += gnb_du_id_present ? 1 : 0;
  nof_ies += activity_notif_level_present ? 1 : 0;
  nof_ies += mdt_polluted_meas_ind_present ? 1 : 0;
  nof_ies += ue_slice_max_bit_rate_list_present ? 1 : 0;
  nof_ies += scg_activation_status_present ? 1 : 0;
  nof_ies += sdt_continue_rohc_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_mod_list_present ? 1 : 0;
  nof_ies += inactivity_info_request_present ? 1 : 0;
  nof_ies += mt_sdt_info_request_present ? 1 : 0;
  nof_ies += sdt_data_size_thres_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (security_info_present) {
    HANDLE_CODE(pack_ie(bref, 13, crit_e{crit_e::reject}, [this, &bref]() { return security_info.pack(bref); }));
  }
  if (ue_dl_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(bref, 14, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, ue_dl_aggr_max_bit_rate, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (ue_dl_max_integrity_protected_data_rate_present) {
    HANDLE_CODE(pack_ie(bref, 66, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(
          bref, ue_dl_max_integrity_protected_data_rate, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (bearer_context_status_change_present) {
    HANDLE_CODE(
        pack_ie(bref, 17, crit_e{crit_e::reject}, [this, &bref]() { return bearer_context_status_change.pack(bref); }));
  }
  if (new_ul_tnl_info_required_present) {
    HANDLE_CODE(
        pack_ie(bref, 26, crit_e{crit_e::reject}, [this, &bref]() { return new_ul_tnl_info_required.pack(bref); }));
  }
  if (ue_inactivity_timer_present) {
    HANDLE_CODE(pack_ie(bref, 59, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, ue_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true);
    }));
  }
  if (data_discard_required_present) {
    HANDLE_CODE(
        pack_ie(bref, 70, crit_e{crit_e::ignore}, [this, &bref]() { return data_discard_required.pack(bref); }));
  }
  if (sys_bearer_context_mod_request_present) {
    HANDLE_CODE(pack_ie(
        bref, 18, crit_e{crit_e::reject}, [this, &bref]() { return sys_bearer_context_mod_request.pack(bref); }));
  }
  if (ran_ue_id_present) {
    HANDLE_CODE(pack_ie(bref, 76, crit_e{crit_e::ignore}, [this, &bref]() { return ran_ue_id.pack(bref); }));
  }
  if (gnb_du_id_present) {
    HANDLE_CODE(pack_ie(bref, 77, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true);
    }));
  }
  if (activity_notif_level_present) {
    HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() { return activity_notif_level.pack(bref); }));
  }
  if (mdt_polluted_meas_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 146, crit_e{crit_e::ignore}, [this, &bref]() { return mdt_polluted_meas_ind.pack(bref); }));
  }
  if (ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 150, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }
  if (scg_activation_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 154, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_status.pack(bref); }));
  }
  if (sdt_continue_rohc_present) {
    HANDLE_CODE(pack_ie(bref, 174, crit_e{crit_e::reject}, [this, &bref]() { return sdt_continue_rohc.pack(bref); }));
  }
  if (management_based_mdt_plmn_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 178, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_mod_list, 0, 16, true);
    }));
  }
  if (inactivity_info_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 187, crit_e{crit_e::ignore}, [this, &bref]() { return inactivity_info_request.pack(bref); }));
  }
  if (mt_sdt_info_request_present) {
    HANDLE_CODE(pack_ie(bref, 195, crit_e{crit_e::ignore}, [this, &bref]() { return mt_sdt_info_request.pack(bref); }));
  }
  if (sdt_data_size_thres_present) {
    HANDLE_CODE(pack_ie(bref, 196, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sdt_data_size_thres, (uint32_t)1u, (uint32_t)192000u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_mod_request_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 13: {
        security_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return security_info.unpack(bref); }));
        break;
      }
      case 14: {
        ue_dl_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ue_dl_aggr_max_bit_rate, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 66: {
        ue_dl_max_integrity_protected_data_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              ue_dl_max_integrity_protected_data_rate, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 17: {
        bearer_context_status_change_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return bearer_context_status_change.unpack(bref); }));
        break;
      }
      case 26: {
        new_ul_tnl_info_required_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return new_ul_tnl_info_required.unpack(bref); }));
        break;
      }
      case 59: {
        ue_inactivity_timer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ue_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true);
        }));
        break;
      }
      case 70: {
        data_discard_required_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return data_discard_required.unpack(bref); }));
        break;
      }
      case 18: {
        sys_bearer_context_mod_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sys_bearer_context_mod_request.unpack(bref); }));
        break;
      }
      case 76: {
        ran_ue_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_ue_id.unpack(bref); }));
        break;
      }
      case 77: {
        gnb_du_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true);
        }));
        break;
      }
      case 23: {
        activity_notif_level_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return activity_notif_level.unpack(bref); }));
        break;
      }
      case 146: {
        mdt_polluted_meas_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mdt_polluted_meas_ind.unpack(bref); }));
        break;
      }
      case 150: {
        ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      case 154: {
        scg_activation_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_status.unpack(bref); }));
        break;
      }
      case 174: {
        sdt_continue_rohc_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_continue_rohc.unpack(bref); }));
        break;
      }
      case 178: {
        management_based_mdt_plmn_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(management_based_mdt_plmn_mod_list, bref, 0, 16, true);
        }));
        break;
      }
      case 187: {
        inactivity_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return inactivity_info_request.unpack(bref); }));
        break;
      }
      case 195: {
        mt_sdt_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mt_sdt_info_request.unpack(bref); }));
        break;
      }
      case 196: {
        sdt_data_size_thres_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sdt_data_size_thres, bref, (uint32_t)1u, (uint32_t)192000u, true, true);
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
void bearer_context_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  if (security_info_present) {
    j.start_obj();
    j.write_int("id", 13);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    security_info.to_json(j);
    j.end_obj();
  }
  if (ue_dl_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 14);
    j.write_str("criticality", "reject");
    j.write_uint("value", ue_dl_aggr_max_bit_rate);
    j.end_obj();
  }
  if (ue_dl_max_integrity_protected_data_rate_present) {
    j.start_obj();
    j.write_int("id", 66);
    j.write_str("criticality", "reject");
    j.write_uint("value", ue_dl_max_integrity_protected_data_rate);
    j.end_obj();
  }
  if (bearer_context_status_change_present) {
    j.start_obj();
    j.write_int("id", 17);
    j.write_str("criticality", "reject");
    j.write_str("value", bearer_context_status_change.to_string());
    j.end_obj();
  }
  if (new_ul_tnl_info_required_present) {
    j.start_obj();
    j.write_int("id", 26);
    j.write_str("criticality", "reject");
    j.write_str("value", "required");
    j.end_obj();
  }
  if (ue_inactivity_timer_present) {
    j.start_obj();
    j.write_int("id", 59);
    j.write_str("criticality", "reject");
    j.write_int("value", ue_inactivity_timer);
    j.end_obj();
  }
  if (data_discard_required_present) {
    j.start_obj();
    j.write_int("id", 70);
    j.write_str("criticality", "ignore");
    j.write_str("value", "required");
    j.end_obj();
  }
  if (sys_bearer_context_mod_request_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    sys_bearer_context_mod_request.to_json(j);
    j.end_obj();
  }
  if (ran_ue_id_present) {
    j.start_obj();
    j.write_int("id", 76);
    j.write_str("criticality", "ignore");
    j.write_str("value", ran_ue_id.to_string());
    j.end_obj();
  }
  if (gnb_du_id_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_id);
    j.end_obj();
  }
  if (activity_notif_level_present) {
    j.start_obj();
    j.write_int("id", 23);
    j.write_str("criticality", "ignore");
    j.write_str("value", activity_notif_level.to_string());
    j.end_obj();
  }
  if (mdt_polluted_meas_ind_present) {
    j.start_obj();
    j.write_int("id", 146);
    j.write_str("criticality", "ignore");
    j.write_str("value", mdt_polluted_meas_ind.to_string());
    j.end_obj();
  }
  if (ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 150);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (scg_activation_status_present) {
    j.start_obj();
    j.write_int("id", 154);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_status.to_string());
    j.end_obj();
  }
  if (sdt_continue_rohc_present) {
    j.start_obj();
    j.write_int("id", 174);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (management_based_mdt_plmn_mod_list_present) {
    j.start_obj();
    j.write_int("id", 178);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_mod_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (inactivity_info_request_present) {
    j.start_obj();
    j.write_int("id", 187);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (mt_sdt_info_request_present) {
    j.start_obj();
    j.write_int("id", 195);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (sdt_data_size_thres_present) {
    j.start_obj();
    j.write_int("id", 196);
    j.write_str("criticality", "ignore");
    j.write_int("value", sdt_data_size_thres);
    j.end_obj();
  }
  j.end_array();
}

// EUTRAN-BearerContextModificationRequired ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t eutran_bearer_context_mod_required_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {35, 36};
  return map_enum_number(names, 2, idx, "id");
}
bool eutran_bearer_context_mod_required_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {35, 36};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e eutran_bearer_context_mod_required_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 35:
      return crit_e::reject;
    case 36:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e eutran_bearer_context_mod_required_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 35:
      return presence_e::optional;
    case 36:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE eutran_bearer_context_mod_required_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += drb_required_to_modify_list_eutran_present ? 1 : 0;
  nof_ies += drb_required_to_rem_list_eutran_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (drb_required_to_modify_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 35, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_required_to_modify_list_eutran, 1, 32, true);
    }));
  }
  if (drb_required_to_rem_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 36, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_required_to_rem_list_eutran, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_bearer_context_mod_required_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &eutran_bearer_context_mod_required_o::get_crit));

    switch (id) {
      case 35: {
        drb_required_to_modify_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(drb_required_to_modify_list_eutran, bref, 1, 32, true);
        }));
        break;
      }
      case 36: {
        drb_required_to_rem_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_required_to_rem_list_eutran, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void eutran_bearer_context_mod_required_container::to_json(json_writer& j) const
{
  j.start_array();
  if (drb_required_to_modify_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 35);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drb_required_to_modify_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drb_required_to_rem_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 36);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drb_required_to_rem_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// NG-RAN-BearerContextModificationRequired ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ng_ran_bearer_context_mod_required_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {45, 44};
  return map_enum_number(names, 2, idx, "id");
}
bool ng_ran_bearer_context_mod_required_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {45, 44};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_ran_bearer_context_mod_required_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 45:
      return crit_e::reject;
    case 44:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_ran_bearer_context_mod_required_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 45:
      return presence_e::optional;
    case 44:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_ran_bearer_context_mod_required_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pdu_session_res_required_to_modify_list_present ? 1 : 0;
  nof_ies += pdu_session_res_to_rem_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (pdu_session_res_required_to_modify_list_present) {
    HANDLE_CODE(pack_ie(bref, 45, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_required_to_modify_list, 1, 256, true);
    }));
  }
  if (pdu_session_res_to_rem_list_present) {
    HANDLE_CODE(pack_ie(bref, 44, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_to_rem_list, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_bearer_context_mod_required_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_ran_bearer_context_mod_required_o::get_crit));

    switch (id) {
      case 45: {
        pdu_session_res_required_to_modify_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_required_to_modify_list, bref, 1, 256, true);
        }));
        break;
      }
      case 44: {
        pdu_session_res_to_rem_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_to_rem_list, bref, 1, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void ng_ran_bearer_context_mod_required_container::to_json(json_writer& j) const
{
  j.start_array();
  if (pdu_session_res_required_to_modify_list_present) {
    j.start_obj();
    j.write_int("id", 45);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_required_to_modify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 44);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// System-BearerContextModificationRequired ::= CHOICE
void sys_bearer_context_mod_required_c::destroy_()
{
  switch (type_) {
    case types::e_utran_bearer_context_mod_required:
      c.destroy<eutran_bearer_context_mod_required_container>();
      break;
    case types::ng_ran_bearer_context_mod_required:
      c.destroy<ng_ran_bearer_context_mod_required_container>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void sys_bearer_context_mod_required_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::e_utran_bearer_context_mod_required:
      c.init<eutran_bearer_context_mod_required_container>();
      break;
    case types::ng_ran_bearer_context_mod_required:
      c.init<ng_ran_bearer_context_mod_required_container>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_required_c");
  }
}
sys_bearer_context_mod_required_c::sys_bearer_context_mod_required_c(const sys_bearer_context_mod_required_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::e_utran_bearer_context_mod_required:
      c.init(other.c.get<eutran_bearer_context_mod_required_container>());
      break;
    case types::ng_ran_bearer_context_mod_required:
      c.init(other.c.get<ng_ran_bearer_context_mod_required_container>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_required_c");
  }
}
sys_bearer_context_mod_required_c&
sys_bearer_context_mod_required_c::operator=(const sys_bearer_context_mod_required_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::e_utran_bearer_context_mod_required:
      c.set(other.c.get<eutran_bearer_context_mod_required_container>());
      break;
    case types::ng_ran_bearer_context_mod_required:
      c.set(other.c.get<ng_ran_bearer_context_mod_required_container>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_required_c");
  }

  return *this;
}
eutran_bearer_context_mod_required_container&
sys_bearer_context_mod_required_c::set_e_utran_bearer_context_mod_required()
{
  set(types::e_utran_bearer_context_mod_required);
  return c.get<eutran_bearer_context_mod_required_container>();
}
ng_ran_bearer_context_mod_required_container&
sys_bearer_context_mod_required_c::set_ng_ran_bearer_context_mod_required()
{
  set(types::ng_ran_bearer_context_mod_required);
  return c.get<ng_ran_bearer_context_mod_required_container>();
}
protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>&
sys_bearer_context_mod_required_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>>();
}
void sys_bearer_context_mod_required_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::e_utran_bearer_context_mod_required:
      j.write_fieldname("e-UTRAN-BearerContextModificationRequired");
      c.get<eutran_bearer_context_mod_required_container>().to_json(j);
      break;
    case types::ng_ran_bearer_context_mod_required:
      j.write_fieldname("nG-RAN-BearerContextModificationRequired");
      c.get<ng_ran_bearer_context_mod_required_container>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_required_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sys_bearer_context_mod_required_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::e_utran_bearer_context_mod_required:
      HANDLE_CODE(c.get<eutran_bearer_context_mod_required_container>().pack(bref));
      break;
    case types::ng_ran_bearer_context_mod_required:
      HANDLE_CODE(c.get<ng_ran_bearer_context_mod_required_container>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_required_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_bearer_context_mod_required_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::e_utran_bearer_context_mod_required:
      HANDLE_CODE(c.get<eutran_bearer_context_mod_required_container>().unpack(bref));
      break;
    case types::ng_ran_bearer_context_mod_required:
      HANDLE_CODE(c.get<ng_ran_bearer_context_mod_required_container>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_mod_required_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_required_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sys_bearer_context_mod_required_c::types_opts::to_string() const
{
  static const char* names[] = {
      "e-UTRAN-BearerContextModificationRequired", "nG-RAN-BearerContextModificationRequired", "choice-extension"};
  return convert_enum_idx(names, 3, value, "sys_bearer_context_mod_required_c::types");
}

// BearerContextModificationRequiredIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_mod_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 21};
  return map_enum_number(names, 3, idx, "id");
}
bool bearer_context_mod_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 21};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_mod_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 21:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_mod_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 21:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_mod_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 21, crit_e{crit_e::reject}, [this, &bref]() { return sys_bearer_context_mod_required.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_mod_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_mod_required_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 21: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sys_bearer_context_mod_required.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void bearer_context_mod_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 21);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  sys_bearer_context_mod_required.to_json(j);
  j.end_obj();
  j.end_array();
}

// EUTRAN-BearerContextModificationResponse ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t eutran_bearer_context_mod_resp_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {52, 53, 39, 40, 85};
  return map_enum_number(names, 5, idx, "id");
}
bool eutran_bearer_context_mod_resp_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {52, 53, 39, 40, 85};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e eutran_bearer_context_mod_resp_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 52:
      return crit_e::ignore;
    case 53:
      return crit_e::ignore;
    case 39:
      return crit_e::ignore;
    case 40:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e eutran_bearer_context_mod_resp_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 52:
      return presence_e::optional;
    case 53:
      return presence_e::optional;
    case 39:
      return presence_e::optional;
    case 40:
      return presence_e::optional;
    case 85:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE eutran_bearer_context_mod_resp_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += drb_setup_mod_list_eutran_present ? 1 : 0;
  nof_ies += drb_failed_mod_list_eutran_present ? 1 : 0;
  nof_ies += drb_modified_list_eutran_present ? 1 : 0;
  nof_ies += drb_failed_to_modify_list_eutran_present ? 1 : 0;
  nof_ies += retainability_meass_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (drb_setup_mod_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 52, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_setup_mod_list_eutran, 1, 32, true);
    }));
  }
  if (drb_failed_mod_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 53, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_failed_mod_list_eutran, 1, 32, true);
    }));
  }
  if (drb_modified_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_modified_list_eutran, 1, 32, true);
    }));
  }
  if (drb_failed_to_modify_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_failed_to_modify_list_eutran, 1, 32, true);
    }));
  }
  if (retainability_meass_info_present) {
    HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, retainability_meass_info, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_bearer_context_mod_resp_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &eutran_bearer_context_mod_resp_o::get_crit));

    switch (id) {
      case 52: {
        drb_setup_mod_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_setup_mod_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 53: {
        drb_failed_mod_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_failed_mod_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 39: {
        drb_modified_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_modified_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 40: {
        drb_failed_to_modify_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_failed_to_modify_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 85: {
        retainability_meass_info_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(retainability_meass_info, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void eutran_bearer_context_mod_resp_container::to_json(json_writer& j) const
{
  j.start_array();
  if (drb_setup_mod_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 52);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drb_setup_mod_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drb_failed_mod_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 53);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drb_failed_mod_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drb_modified_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 39);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drb_modified_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drb_failed_to_modify_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 40);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drb_failed_to_modify_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (retainability_meass_info_present) {
    j.start_obj();
    j.write_int("id", 85);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : retainability_meass_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// NG-RAN-BearerContextModificationResponse ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ng_ran_bearer_context_mod_resp_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {54, 55, 48, 49, 85};
  return map_enum_number(names, 5, idx, "id");
}
bool ng_ran_bearer_context_mod_resp_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {54, 55, 48, 49, 85};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_ran_bearer_context_mod_resp_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 54:
      return crit_e::reject;
    case 55:
      return crit_e::reject;
    case 48:
      return crit_e::reject;
    case 49:
      return crit_e::reject;
    case 85:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_ran_bearer_context_mod_resp_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 54:
      return presence_e::optional;
    case 55:
      return presence_e::optional;
    case 48:
      return presence_e::optional;
    case 49:
      return presence_e::optional;
    case 85:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_ran_bearer_context_mod_resp_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pdu_session_res_setup_mod_list_present ? 1 : 0;
  nof_ies += pdu_session_res_failed_mod_list_present ? 1 : 0;
  nof_ies += pdu_session_res_modified_list_present ? 1 : 0;
  nof_ies += pdu_session_res_failed_to_modify_list_present ? 1 : 0;
  nof_ies += retainability_meass_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (pdu_session_res_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 54, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_setup_mod_list, 1, 256, true);
    }));
  }
  if (pdu_session_res_failed_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 55, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_mod_list, 1, 256, true);
    }));
  }
  if (pdu_session_res_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 48, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_modified_list, 1, 256, true);
    }));
  }
  if (pdu_session_res_failed_to_modify_list_present) {
    HANDLE_CODE(pack_ie(bref, 49, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_modify_list, 1, 256, true);
    }));
  }
  if (retainability_meass_info_present) {
    HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, retainability_meass_info, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_bearer_context_mod_resp_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_ran_bearer_context_mod_resp_o::get_crit));

    switch (id) {
      case 54: {
        pdu_session_res_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_setup_mod_list, bref, 1, 256, true); }));
        break;
      }
      case 55: {
        pdu_session_res_failed_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_failed_mod_list, bref, 1, 256, true); }));
        break;
      }
      case 48: {
        pdu_session_res_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_modified_list, bref, 1, 256, true); }));
        break;
      }
      case 49: {
        pdu_session_res_failed_to_modify_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_modify_list, bref, 1, 256, true);
        }));
        break;
      }
      case 85: {
        retainability_meass_info_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(retainability_meass_info, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void ng_ran_bearer_context_mod_resp_container::to_json(json_writer& j) const
{
  j.start_array();
  if (pdu_session_res_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_failed_mod_list_present) {
    j.start_obj();
    j.write_int("id", 55);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_modified_list_present) {
    j.start_obj();
    j.write_int("id", 48);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_failed_to_modify_list_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_modify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (retainability_meass_info_present) {
    j.start_obj();
    j.write_int("id", 85);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : retainability_meass_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// System-BearerContextModificationResponse ::= CHOICE
void sys_bearer_context_mod_resp_c::destroy_()
{
  switch (type_) {
    case types::e_utran_bearer_context_mod_resp:
      c.destroy<eutran_bearer_context_mod_resp_container>();
      break;
    case types::ng_ran_bearer_context_mod_resp:
      c.destroy<ng_ran_bearer_context_mod_resp_container>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void sys_bearer_context_mod_resp_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::e_utran_bearer_context_mod_resp:
      c.init<eutran_bearer_context_mod_resp_container>();
      break;
    case types::ng_ran_bearer_context_mod_resp:
      c.init<ng_ran_bearer_context_mod_resp_container>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_resp_c");
  }
}
sys_bearer_context_mod_resp_c::sys_bearer_context_mod_resp_c(const sys_bearer_context_mod_resp_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::e_utran_bearer_context_mod_resp:
      c.init(other.c.get<eutran_bearer_context_mod_resp_container>());
      break;
    case types::ng_ran_bearer_context_mod_resp:
      c.init(other.c.get<ng_ran_bearer_context_mod_resp_container>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_resp_c");
  }
}
sys_bearer_context_mod_resp_c& sys_bearer_context_mod_resp_c::operator=(const sys_bearer_context_mod_resp_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::e_utran_bearer_context_mod_resp:
      c.set(other.c.get<eutran_bearer_context_mod_resp_container>());
      break;
    case types::ng_ran_bearer_context_mod_resp:
      c.set(other.c.get<ng_ran_bearer_context_mod_resp_container>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_resp_c");
  }

  return *this;
}
eutran_bearer_context_mod_resp_container& sys_bearer_context_mod_resp_c::set_e_utran_bearer_context_mod_resp()
{
  set(types::e_utran_bearer_context_mod_resp);
  return c.get<eutran_bearer_context_mod_resp_container>();
}
ng_ran_bearer_context_mod_resp_container& sys_bearer_context_mod_resp_c::set_ng_ran_bearer_context_mod_resp()
{
  set(types::ng_ran_bearer_context_mod_resp);
  return c.get<ng_ran_bearer_context_mod_resp_container>();
}
protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>& sys_bearer_context_mod_resp_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>>();
}
void sys_bearer_context_mod_resp_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::e_utran_bearer_context_mod_resp:
      j.write_fieldname("e-UTRAN-BearerContextModificationResponse");
      c.get<eutran_bearer_context_mod_resp_container>().to_json(j);
      break;
    case types::ng_ran_bearer_context_mod_resp:
      j.write_fieldname("nG-RAN-BearerContextModificationResponse");
      c.get<ng_ran_bearer_context_mod_resp_container>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_resp_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sys_bearer_context_mod_resp_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::e_utran_bearer_context_mod_resp:
      HANDLE_CODE(c.get<eutran_bearer_context_mod_resp_container>().pack(bref));
      break;
    case types::ng_ran_bearer_context_mod_resp:
      HANDLE_CODE(c.get<ng_ran_bearer_context_mod_resp_container>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_resp_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_bearer_context_mod_resp_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::e_utran_bearer_context_mod_resp:
      HANDLE_CODE(c.get<eutran_bearer_context_mod_resp_container>().unpack(bref));
      break;
    case types::ng_ran_bearer_context_mod_resp:
      HANDLE_CODE(c.get<ng_ran_bearer_context_mod_resp_container>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_mod_resp_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_mod_resp_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sys_bearer_context_mod_resp_c::types_opts::to_string() const
{
  static const char* names[] = {
      "e-UTRAN-BearerContextModificationResponse", "nG-RAN-BearerContextModificationResponse", "choice-extension"};
  return convert_enum_idx(names, 3, value, "sys_bearer_context_mod_resp_c::types");
}

// BearerContextModificationResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 19, 1, 188};
  return map_enum_number(names, 5, idx, "id");
}
bool bearer_context_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 19, 1, 188};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    case 188:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    case 188:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += sys_bearer_context_mod_resp_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += ue_inactivity_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (sys_bearer_context_mod_resp_present) {
    HANDLE_CODE(
        pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return sys_bearer_context_mod_resp.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (ue_inactivity_info_present) {
    HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ue_inactivity_info, (uint16_t)1u, (uint16_t)7200u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_mod_resp_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 19: {
        sys_bearer_context_mod_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sys_bearer_context_mod_resp.unpack(bref); }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 188: {
        ue_inactivity_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ue_inactivity_info, bref, (uint16_t)1u, (uint16_t)7200u, true, true);
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
void bearer_context_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  if (sys_bearer_context_mod_resp_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sys_bearer_context_mod_resp.to_json(j);
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
  if (ue_inactivity_info_present) {
    j.start_obj();
    j.write_int("id", 188);
    j.write_str("criticality", "ignore");
    j.write_int("value", ue_inactivity_info);
    j.end_obj();
  }
  j.end_array();
}

// BearerContextReleaseCommandIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool bearer_context_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_release_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_release_cmd_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
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
void bearer_context_release_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// BearerContextReleaseCompleteIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_release_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 1, 85};
  return map_enum_number(names, 4, idx, "id");
}
bool bearer_context_release_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 1, 85};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_release_complete_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_release_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    case 85:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_release_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += retainability_meass_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (retainability_meass_info_present) {
    HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, retainability_meass_info, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_release_complete_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_release_complete_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 85: {
        retainability_meass_info_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(retainability_meass_info, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void bearer_context_release_complete_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (retainability_meass_info_present) {
    j.start_obj();
    j.write_int("id", 85);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : retainability_meass_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// BearerContextReleaseRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 22, 0};
  return map_enum_number(names, 4, idx, "id");
}
bool bearer_context_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 22, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 22:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 22:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += drb_status_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (drb_status_list_present) {
    HANDLE_CODE(pack_ie(bref, 22, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_status_list, 1, 32, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_release_request_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 22: {
        drb_status_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(drb_status_list, bref, 1, 32, true); }));
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
void bearer_context_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  if (drb_status_list_present) {
    j.start_obj();
    j.write_int("id", 22);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drb_status_list) {
      e1.to_json(j);
    }
    j.end_array();
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

// BearerContextSetupFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 0, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bearer_context_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += gnb_cu_up_ue_e1ap_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (gnb_cu_up_ue_e1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_setup_fail_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        gnb_cu_up_ue_e1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
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
void bearer_context_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  if (gnb_cu_up_ue_e1ap_id_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_cu_up_ue_e1ap_id);
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
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// EUTRAN-BearerContextSetupRequest ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t eutran_bearer_context_setup_request_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {32, 83, 84};
  return map_enum_number(names, 3, idx, "id");
}
bool eutran_bearer_context_setup_request_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {32, 83, 84};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e eutran_bearer_context_setup_request_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 32:
      return crit_e::reject;
    case 83:
      return crit_e::ignore;
    case 84:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e eutran_bearer_context_setup_request_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 32:
      return presence_e::mandatory;
    case 83:
      return presence_e::optional;
    case 84:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE eutran_bearer_context_setup_request_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += sub_profile_id_for_rfp_present ? 1 : 0;
  nof_ies += add_rrm_prio_idx_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 32, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, drb_to_setup_list_eutran, 1, 32, true);
  }));
  if (sub_profile_id_for_rfp_present) {
    HANDLE_CODE(pack_ie(bref, 83, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sub_profile_id_for_rfp, (uint16_t)1u, (uint16_t)256u, true, true);
    }));
  }
  if (add_rrm_prio_idx_present) {
    HANDLE_CODE(pack_ie(bref, 84, crit_e{crit_e::ignore}, [this, &bref]() { return add_rrm_prio_idx.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_bearer_context_setup_request_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &eutran_bearer_context_setup_request_o::get_crit));

    switch (id) {
      case 32: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_to_setup_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 83: {
        sub_profile_id_for_rfp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sub_profile_id_for_rfp, bref, (uint16_t)1u, (uint16_t)256u, true, true);
        }));
        break;
      }
      case 84: {
        add_rrm_prio_idx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return add_rrm_prio_idx.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void eutran_bearer_context_setup_request_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 32);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : drb_to_setup_list_eutran) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (sub_profile_id_for_rfp_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_int("value", sub_profile_id_for_rfp);
    j.end_obj();
  }
  if (add_rrm_prio_idx_present) {
    j.start_obj();
    j.write_int("id", 84);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    add_rrm_prio_idx.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// NG-RAN-BearerContextSetupRequest ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ng_ran_bearer_context_setup_request_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {42};
  return map_enum_number(names, 1, idx, "id");
}
bool ng_ran_bearer_context_setup_request_o::is_id_valid(const uint32_t& id)
{
  return 42 == id;
}
crit_e ng_ran_bearer_context_setup_request_o::get_crit(const uint32_t& id)
{
  if (id == 42) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e ng_ran_bearer_context_setup_request_o::get_presence(const uint32_t& id)
{
  if (id == 42) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE ng_ran_bearer_context_setup_request_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 42, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_to_setup_list, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_bearer_context_setup_request_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_ran_bearer_context_setup_request_o::get_crit));

    if (id == 42) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_to_setup_list, bref, 1, 256, true); }));
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
void ng_ran_bearer_context_setup_request_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 42);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_to_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// System-BearerContextSetupRequest ::= CHOICE
void sys_bearer_context_setup_request_c::destroy_()
{
  switch (type_) {
    case types::e_utran_bearer_context_setup_request:
      c.destroy<eutran_bearer_context_setup_request_container>();
      break;
    case types::ng_ran_bearer_context_setup_request:
      c.destroy<ng_ran_bearer_context_setup_request_container>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void sys_bearer_context_setup_request_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::e_utran_bearer_context_setup_request:
      c.init<eutran_bearer_context_setup_request_container>();
      break;
    case types::ng_ran_bearer_context_setup_request:
      c.init<ng_ran_bearer_context_setup_request_container>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_request_c");
  }
}
sys_bearer_context_setup_request_c::sys_bearer_context_setup_request_c(const sys_bearer_context_setup_request_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::e_utran_bearer_context_setup_request:
      c.init(other.c.get<eutran_bearer_context_setup_request_container>());
      break;
    case types::ng_ran_bearer_context_setup_request:
      c.init(other.c.get<ng_ran_bearer_context_setup_request_container>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_request_c");
  }
}
sys_bearer_context_setup_request_c&
sys_bearer_context_setup_request_c::operator=(const sys_bearer_context_setup_request_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::e_utran_bearer_context_setup_request:
      c.set(other.c.get<eutran_bearer_context_setup_request_container>());
      break;
    case types::ng_ran_bearer_context_setup_request:
      c.set(other.c.get<ng_ran_bearer_context_setup_request_container>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_request_c");
  }

  return *this;
}
eutran_bearer_context_setup_request_container&
sys_bearer_context_setup_request_c::set_e_utran_bearer_context_setup_request()
{
  set(types::e_utran_bearer_context_setup_request);
  return c.get<eutran_bearer_context_setup_request_container>();
}
ng_ran_bearer_context_setup_request_container&
sys_bearer_context_setup_request_c::set_ng_ran_bearer_context_setup_request()
{
  set(types::ng_ran_bearer_context_setup_request);
  return c.get<ng_ran_bearer_context_setup_request_container>();
}
protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>&
sys_bearer_context_setup_request_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>>();
}
void sys_bearer_context_setup_request_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::e_utran_bearer_context_setup_request:
      j.write_fieldname("e-UTRAN-BearerContextSetupRequest");
      c.get<eutran_bearer_context_setup_request_container>().to_json(j);
      break;
    case types::ng_ran_bearer_context_setup_request:
      j.write_fieldname("nG-RAN-BearerContextSetupRequest");
      c.get<ng_ran_bearer_context_setup_request_container>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_request_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sys_bearer_context_setup_request_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::e_utran_bearer_context_setup_request:
      HANDLE_CODE(c.get<eutran_bearer_context_setup_request_container>().pack(bref));
      break;
    case types::ng_ran_bearer_context_setup_request:
      HANDLE_CODE(c.get<ng_ran_bearer_context_setup_request_container>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_request_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_bearer_context_setup_request_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::e_utran_bearer_context_setup_request:
      HANDLE_CODE(c.get<eutran_bearer_context_setup_request_container>().unpack(bref));
      break;
    case types::ng_ran_bearer_context_setup_request:
      HANDLE_CODE(c.get<ng_ran_bearer_context_setup_request_container>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_setup_request_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_request_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sys_bearer_context_setup_request_c::types_opts::to_string() const
{
  static const char* names[] = {
      "e-UTRAN-BearerContextSetupRequest", "nG-RAN-BearerContextSetupRequest", "choice-extension"};
  return convert_enum_idx(names, 3, value, "sys_bearer_context_setup_request_c::types");
}

// BearerContextSetupRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2,   13,  14,  66,  58,  23, 59,  17,  15,  76,  77, 81,
                                   111, 113, 121, 134, 139, 3,  146, 150, 154, 195, 196};
  return map_enum_number(names, 23, idx, "id");
}
bool bearer_context_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2,   13,  14,  66,  58,  23, 59,  17,  15,  76,  77, 81,
                                   111, 113, 121, 134, 139, 3,  146, 150, 154, 195, 196};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 13:
      return crit_e::reject;
    case 14:
      return crit_e::reject;
    case 66:
      return crit_e::reject;
    case 58:
      return crit_e::ignore;
    case 23:
      return crit_e::reject;
    case 59:
      return crit_e::reject;
    case 17:
      return crit_e::reject;
    case 15:
      return crit_e::reject;
    case 76:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 81:
      return crit_e::ignore;
    case 111:
      return crit_e::reject;
    case 113:
      return crit_e::ignore;
    case 121:
      return crit_e::reject;
    case 134:
      return crit_e::ignore;
    case 139:
      return crit_e::ignore;
    case 3:
      return crit_e::ignore;
    case 146:
      return crit_e::ignore;
    case 150:
      return crit_e::ignore;
    case 154:
      return crit_e::ignore;
    case 195:
      return crit_e::ignore;
    case 196:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 13:
      return presence_e::mandatory;
    case 14:
      return presence_e::mandatory;
    case 66:
      return presence_e::optional;
    case 58:
      return presence_e::mandatory;
    case 23:
      return presence_e::mandatory;
    case 59:
      return presence_e::optional;
    case 17:
      return presence_e::optional;
    case 15:
      return presence_e::mandatory;
    case 76:
      return presence_e::optional;
    case 77:
      return presence_e::optional;
    case 81:
      return presence_e::optional;
    case 111:
      return presence_e::optional;
    case 113:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    case 134:
      return presence_e::optional;
    case 139:
      return presence_e::optional;
    case 3:
      return presence_e::optional;
    case 146:
      return presence_e::optional;
    case 150:
      return presence_e::optional;
    case 154:
      return presence_e::optional;
    case 195:
      return presence_e::optional;
    case 196:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 6;
  nof_ies += ue_dl_max_integrity_protected_data_rate_present ? 1 : 0;
  nof_ies += ue_inactivity_timer_present ? 1 : 0;
  nof_ies += bearer_context_status_change_present ? 1 : 0;
  nof_ies += ran_ue_id_present ? 1 : 0;
  nof_ies += gnb_du_id_present ? 1 : 0;
  nof_ies += trace_activation_present ? 1 : 0;
  nof_ies += npn_context_info_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_list_present ? 1 : 0;
  nof_ies += cho_initiation_present ? 1 : 0;
  nof_ies += add_ho_info_present ? 1 : 0;
  nof_ies += direct_forwarding_path_availability_present ? 1 : 0;
  nof_ies += gnb_cu_up_ue_e1ap_id_present ? 1 : 0;
  nof_ies += mdt_polluted_meas_ind_present ? 1 : 0;
  nof_ies += ue_slice_max_bit_rate_list_present ? 1 : 0;
  nof_ies += scg_activation_status_present ? 1 : 0;
  nof_ies += mt_sdt_info_request_present ? 1 : 0;
  nof_ies += sdt_data_size_thres_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 13, crit_e{crit_e::reject}, [this, &bref]() { return security_info.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 14, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ue_dl_aggr_max_bit_rate, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
  }));
  if (ue_dl_max_integrity_protected_data_rate_present) {
    HANDLE_CODE(pack_ie(bref, 66, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(
          bref, ue_dl_max_integrity_protected_data_rate, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 58, crit_e{crit_e::ignore}, [this, &bref]() { return serving_plmn.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() { return activity_notif_level.pack(bref); }));
  if (ue_inactivity_timer_present) {
    HANDLE_CODE(pack_ie(bref, 59, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, ue_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true);
    }));
  }
  if (bearer_context_status_change_present) {
    HANDLE_CODE(
        pack_ie(bref, 17, crit_e{crit_e::reject}, [this, &bref]() { return bearer_context_status_change.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(
      bref, 15, crit_e{crit_e::reject}, [this, &bref]() { return sys_bearer_context_setup_request.pack(bref); }));
  if (ran_ue_id_present) {
    HANDLE_CODE(pack_ie(bref, 76, crit_e{crit_e::ignore}, [this, &bref]() { return ran_ue_id.pack(bref); }));
  }
  if (gnb_du_id_present) {
    HANDLE_CODE(pack_ie(bref, 77, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true);
    }));
  }
  if (trace_activation_present) {
    HANDLE_CODE(pack_ie(bref, 81, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));
  }
  if (npn_context_info_present) {
    HANDLE_CODE(pack_ie(bref, 111, crit_e{crit_e::reject}, [this, &bref]() { return npn_context_info.pack(bref); }));
  }
  if (management_based_mdt_plmn_list_present) {
    HANDLE_CODE(pack_ie(bref, 113, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_list, 1, 16, true);
    }));
  }
  if (cho_initiation_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::reject}, [this, &bref]() { return cho_initiation.pack(bref); }));
  }
  if (add_ho_info_present) {
    HANDLE_CODE(pack_ie(bref, 134, crit_e{crit_e::ignore}, [this, &bref]() { return add_ho_info.pack(bref); }));
  }
  if (direct_forwarding_path_availability_present) {
    HANDLE_CODE(pack_ie(
        bref, 139, crit_e{crit_e::ignore}, [this, &bref]() { return direct_forwarding_path_availability.pack(bref); }));
  }
  if (gnb_cu_up_ue_e1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (mdt_polluted_meas_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 146, crit_e{crit_e::ignore}, [this, &bref]() { return mdt_polluted_meas_ind.pack(bref); }));
  }
  if (ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 150, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }
  if (scg_activation_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 154, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_status.pack(bref); }));
  }
  if (mt_sdt_info_request_present) {
    HANDLE_CODE(pack_ie(bref, 195, crit_e{crit_e::ignore}, [this, &bref]() { return mt_sdt_info_request.pack(bref); }));
  }
  if (sdt_data_size_thres_present) {
    HANDLE_CODE(pack_ie(bref, 196, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sdt_data_size_thres, (uint32_t)1u, (uint32_t)192000u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 6;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_setup_request_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 13: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return security_info.unpack(bref); }));
        break;
      }
      case 14: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ue_dl_aggr_max_bit_rate, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 66: {
        ue_dl_max_integrity_protected_data_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              ue_dl_max_integrity_protected_data_rate, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 58: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return serving_plmn.unpack(bref); }));
        break;
      }
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return activity_notif_level.unpack(bref); }));
        break;
      }
      case 59: {
        ue_inactivity_timer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ue_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true);
        }));
        break;
      }
      case 17: {
        bearer_context_status_change_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return bearer_context_status_change.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sys_bearer_context_setup_request.unpack(bref); }));
        break;
      }
      case 76: {
        ran_ue_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_ue_id.unpack(bref); }));
        break;
      }
      case 77: {
        gnb_du_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true);
        }));
        break;
      }
      case 81: {
        trace_activation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_activation.unpack(bref); }));
        break;
      }
      case 111: {
        npn_context_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return npn_context_info.unpack(bref); }));
        break;
      }
      case 113: {
        management_based_mdt_plmn_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(management_based_mdt_plmn_list, bref, 1, 16, true); }));
        break;
      }
      case 121: {
        cho_initiation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cho_initiation.unpack(bref); }));
        break;
      }
      case 134: {
        add_ho_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return add_ho_info.unpack(bref); }));
        break;
      }
      case 139: {
        direct_forwarding_path_availability_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return direct_forwarding_path_availability.unpack(bref); }));
        break;
      }
      case 3: {
        gnb_cu_up_ue_e1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 146: {
        mdt_polluted_meas_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mdt_polluted_meas_ind.unpack(bref); }));
        break;
      }
      case 150: {
        ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      case 154: {
        scg_activation_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_status.unpack(bref); }));
        break;
      }
      case 195: {
        mt_sdt_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mt_sdt_info_request.unpack(bref); }));
        break;
      }
      case 196: {
        sdt_data_size_thres_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sdt_data_size_thres, bref, (uint32_t)1u, (uint32_t)192000u, true, true);
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
void bearer_context_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 13);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  security_info.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 14);
  j.write_str("criticality", "reject");
  j.write_uint("value", ue_dl_aggr_max_bit_rate);
  j.end_obj();
  if (ue_dl_max_integrity_protected_data_rate_present) {
    j.start_obj();
    j.write_int("id", 66);
    j.write_str("criticality", "reject");
    j.write_uint("value", ue_dl_max_integrity_protected_data_rate);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 58);
  j.write_str("criticality", "ignore");
  j.write_str("value", serving_plmn.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_str("value", activity_notif_level.to_string());
  j.end_obj();
  if (ue_inactivity_timer_present) {
    j.start_obj();
    j.write_int("id", 59);
    j.write_str("criticality", "reject");
    j.write_int("value", ue_inactivity_timer);
    j.end_obj();
  }
  if (bearer_context_status_change_present) {
    j.start_obj();
    j.write_int("id", 17);
    j.write_str("criticality", "reject");
    j.write_str("value", bearer_context_status_change.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  sys_bearer_context_setup_request.to_json(j);
  j.end_obj();
  if (ran_ue_id_present) {
    j.start_obj();
    j.write_int("id", 76);
    j.write_str("criticality", "ignore");
    j.write_str("value", ran_ue_id.to_string());
    j.end_obj();
  }
  if (gnb_du_id_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_id);
    j.end_obj();
  }
  if (trace_activation_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    trace_activation.to_json(j);
    j.end_obj();
  }
  if (npn_context_info_present) {
    j.start_obj();
    j.write_int("id", 111);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    npn_context_info.to_json(j);
    j.end_obj();
  }
  if (management_based_mdt_plmn_list_present) {
    j.start_obj();
    j.write_int("id", 113);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (cho_initiation_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (add_ho_info_present) {
    j.start_obj();
    j.write_int("id", 134);
    j.write_str("criticality", "ignore");
    j.write_str("value", "discard-pdpc-SN");
    j.end_obj();
  }
  if (direct_forwarding_path_availability_present) {
    j.start_obj();
    j.write_int("id", 139);
    j.write_str("criticality", "ignore");
    j.write_str("value", direct_forwarding_path_availability.to_string());
    j.end_obj();
  }
  if (gnb_cu_up_ue_e1ap_id_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_cu_up_ue_e1ap_id);
    j.end_obj();
  }
  if (mdt_polluted_meas_ind_present) {
    j.start_obj();
    j.write_int("id", 146);
    j.write_str("criticality", "ignore");
    j.write_str("value", mdt_polluted_meas_ind.to_string());
    j.end_obj();
  }
  if (ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 150);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (scg_activation_status_present) {
    j.start_obj();
    j.write_int("id", 154);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_status.to_string());
    j.end_obj();
  }
  if (mt_sdt_info_request_present) {
    j.start_obj();
    j.write_int("id", 195);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (sdt_data_size_thres_present) {
    j.start_obj();
    j.write_int("id", 196);
    j.write_str("criticality", "ignore");
    j.write_int("value", sdt_data_size_thres);
    j.end_obj();
  }
  j.end_array();
}

// EUTRAN-BearerContextSetupResponse ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t eutran_bearer_context_setup_resp_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {37, 38};
  return map_enum_number(names, 2, idx, "id");
}
bool eutran_bearer_context_setup_resp_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {37, 38};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e eutran_bearer_context_setup_resp_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 37:
      return crit_e::ignore;
    case 38:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e eutran_bearer_context_setup_resp_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 37:
      return presence_e::mandatory;
    case 38:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE eutran_bearer_context_setup_resp_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += drb_failed_list_eutran_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 37, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, drb_setup_list_eutran, 1, 32, true);
  }));
  if (drb_failed_list_eutran_present) {
    HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drb_failed_list_eutran, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_bearer_context_setup_resp_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &eutran_bearer_context_setup_resp_o::get_crit));

    switch (id) {
      case 37: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_setup_list_eutran, bref, 1, 32, true); }));
        break;
      }
      case 38: {
        drb_failed_list_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_failed_list_eutran, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void eutran_bearer_context_setup_resp_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 37);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : drb_setup_list_eutran) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (drb_failed_list_eutran_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drb_failed_list_eutran) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// NG-RAN-BearerContextSetupResponse ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ng_ran_bearer_context_setup_resp_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {46, 47};
  return map_enum_number(names, 2, idx, "id");
}
bool ng_ran_bearer_context_setup_resp_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {46, 47};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_ran_bearer_context_setup_resp_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 46:
      return crit_e::ignore;
    case 47:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_ran_bearer_context_setup_resp_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 46:
      return presence_e::mandatory;
    case 47:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_ran_bearer_context_setup_resp_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += pdu_session_res_failed_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 46, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_setup_list, 1, 256, true);
  }));
  if (pdu_session_res_failed_list_present) {
    HANDLE_CODE(pack_ie(bref, 47, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_list, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_bearer_context_setup_resp_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_ran_bearer_context_setup_resp_o::get_crit));

    switch (id) {
      case 46: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_setup_list, bref, 1, 256, true); }));
        break;
      }
      case 47: {
        pdu_session_res_failed_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_failed_list, bref, 1, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ng_ran_bearer_context_setup_resp_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 46);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (pdu_session_res_failed_list_present) {
    j.start_obj();
    j.write_int("id", 47);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// System-BearerContextSetupResponse ::= CHOICE
void sys_bearer_context_setup_resp_c::destroy_()
{
  switch (type_) {
    case types::e_utran_bearer_context_setup_resp:
      c.destroy<eutran_bearer_context_setup_resp_container>();
      break;
    case types::ng_ran_bearer_context_setup_resp:
      c.destroy<ng_ran_bearer_context_setup_resp_container>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void sys_bearer_context_setup_resp_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::e_utran_bearer_context_setup_resp:
      c.init<eutran_bearer_context_setup_resp_container>();
      break;
    case types::ng_ran_bearer_context_setup_resp:
      c.init<ng_ran_bearer_context_setup_resp_container>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_resp_c");
  }
}
sys_bearer_context_setup_resp_c::sys_bearer_context_setup_resp_c(const sys_bearer_context_setup_resp_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::e_utran_bearer_context_setup_resp:
      c.init(other.c.get<eutran_bearer_context_setup_resp_container>());
      break;
    case types::ng_ran_bearer_context_setup_resp:
      c.init(other.c.get<ng_ran_bearer_context_setup_resp_container>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_resp_c");
  }
}
sys_bearer_context_setup_resp_c&
sys_bearer_context_setup_resp_c::operator=(const sys_bearer_context_setup_resp_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::e_utran_bearer_context_setup_resp:
      c.set(other.c.get<eutran_bearer_context_setup_resp_container>());
      break;
    case types::ng_ran_bearer_context_setup_resp:
      c.set(other.c.get<ng_ran_bearer_context_setup_resp_container>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_resp_c");
  }

  return *this;
}
eutran_bearer_context_setup_resp_container& sys_bearer_context_setup_resp_c::set_e_utran_bearer_context_setup_resp()
{
  set(types::e_utran_bearer_context_setup_resp);
  return c.get<eutran_bearer_context_setup_resp_container>();
}
ng_ran_bearer_context_setup_resp_container& sys_bearer_context_setup_resp_c::set_ng_ran_bearer_context_setup_resp()
{
  set(types::ng_ran_bearer_context_setup_resp);
  return c.get<ng_ran_bearer_context_setup_resp_container>();
}
protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>&
sys_bearer_context_setup_resp_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>>();
}
void sys_bearer_context_setup_resp_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::e_utran_bearer_context_setup_resp:
      j.write_fieldname("e-UTRAN-BearerContextSetupResponse");
      c.get<eutran_bearer_context_setup_resp_container>().to_json(j);
      break;
    case types::ng_ran_bearer_context_setup_resp:
      j.write_fieldname("nG-RAN-BearerContextSetupResponse");
      c.get<ng_ran_bearer_context_setup_resp_container>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_resp_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sys_bearer_context_setup_resp_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::e_utran_bearer_context_setup_resp:
      HANDLE_CODE(c.get<eutran_bearer_context_setup_resp_container>().pack(bref));
      break;
    case types::ng_ran_bearer_context_setup_resp:
      HANDLE_CODE(c.get<ng_ran_bearer_context_setup_resp_container>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_resp_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_bearer_context_setup_resp_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::e_utran_bearer_context_setup_resp:
      HANDLE_CODE(c.get<eutran_bearer_context_setup_resp_container>().unpack(bref));
      break;
    case types::ng_ran_bearer_context_setup_resp:
      HANDLE_CODE(c.get<ng_ran_bearer_context_setup_resp_container>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_bearer_context_setup_resp_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_bearer_context_setup_resp_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sys_bearer_context_setup_resp_c::types_opts::to_string() const
{
  static const char* names[] = {
      "e-UTRAN-BearerContextSetupResponse", "nG-RAN-BearerContextSetupResponse", "choice-extension"};
  return convert_enum_idx(names, 3, value, "sys_bearer_context_setup_resp_c::types");
}

// BearerContextSetupResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t bearer_context_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 16, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool bearer_context_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 16, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bearer_context_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 16:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bearer_context_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 16:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bearer_context_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 16, crit_e{crit_e::ignore}, [this, &bref]() { return sys_bearer_context_setup_resp.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearer_context_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &bearer_context_setup_resp_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 16: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sys_bearer_context_setup_resp.unpack(bref); }));
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
void bearer_context_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 16);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  sys_bearer_context_setup_resp.to_json(j);
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

// CellTrafficTraceIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t cell_traffic_trace_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 82, 114, 115, 117};
  return map_enum_number(names, 6, idx, "id");
}
bool cell_traffic_trace_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 82, 114, 115, 117};
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
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 82:
      return crit_e::ignore;
    case 114:
      return crit_e::ignore;
    case 115:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cell_traffic_trace_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 82:
      return presence_e::mandatory;
    case 114:
      return presence_e::mandatory;
    case 115:
      return presence_e::optional;
    case 117:
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
  nof_ies += ur_iaddress_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 82, crit_e{crit_e::ignore}, [this, &bref]() { return trace_id.pack(bref); }));
  HANDLE_CODE(pack_ie(
      bref, 114, crit_e{crit_e::ignore}, [this, &bref]() { return trace_collection_entity_ip_address.pack(bref); }));
  if (privacy_ind_present) {
    HANDLE_CODE(pack_ie(bref, 115, crit_e{crit_e::ignore}, [this, &bref]() { return privacy_ind.pack(bref); }));
  }
  if (ur_iaddress_present) {
    HANDLE_CODE(pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return ur_iaddress.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cell_traffic_trace_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 82: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_id.unpack(bref); }));
        break;
      }
      case 114: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_collection_entity_ip_address.unpack(bref); }));
        break;
      }
      case 115: {
        privacy_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return privacy_ind.unpack(bref); }));
        break;
      }
      case 117: {
        ur_iaddress_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ur_iaddress.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 82);
  j.write_str("criticality", "ignore");
  j.write_str("value", trace_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 114);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  trace_collection_entity_ip_address.to_json(j);
  j.end_obj();
  if (privacy_ind_present) {
    j.start_obj();
    j.write_int("id", 115);
    j.write_str("criticality", "ignore");
    j.write_str("value", privacy_ind.to_string());
    j.end_obj();
  }
  if (ur_iaddress_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_str("value", ur_iaddress.to_string());
    j.end_obj();
  }
  j.end_array();
}

// DLDataNotificationIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t dl_data_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 63, 67, 194, 197};
  return map_enum_number(names, 6, idx, "id");
}
bool dl_data_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 63, 67, 194, 197};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dl_data_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 63:
      return crit_e::ignore;
    case 67:
      return crit_e::ignore;
    case 194:
      return crit_e::ignore;
    case 197:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dl_data_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 63:
      return presence_e::optional;
    case 67:
      return presence_e::optional;
    case 194:
      return presence_e::optional;
    case 197:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dl_data_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ppi_present ? 1 : 0;
  nof_ies += pdu_session_to_notify_list_present ? 1 : 0;
  nof_ies += mt_sdt_info_present ? 1 : 0;
  nof_ies += sdt_data_size_thres_crossed_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ppi_present) {
    HANDLE_CODE(pack_ie(bref, 63, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ppi, (uint8_t)0u, (uint8_t)7u, true, true);
    }));
  }
  if (pdu_session_to_notify_list_present) {
    HANDLE_CODE(pack_ie(bref, 67, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_to_notify_list, 1, 256, true);
    }));
  }
  if (mt_sdt_info_present) {
    HANDLE_CODE(pack_ie(bref, 194, crit_e{crit_e::ignore}, [this, &bref]() { return mt_sdt_info.pack(bref); }));
  }
  if (sdt_data_size_thres_crossed_present) {
    HANDLE_CODE(
        pack_ie(bref, 197, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_data_size_thres_crossed.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_data_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_data_notif_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 63: {
        ppi_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_integer(ppi, bref, (uint8_t)0u, (uint8_t)7u, true, true); }));
        break;
      }
      case 67: {
        pdu_session_to_notify_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_to_notify_list, bref, 1, 256, true); }));
        break;
      }
      case 194: {
        mt_sdt_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mt_sdt_info.unpack(bref); }));
        break;
      }
      case 197: {
        sdt_data_size_thres_crossed_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_data_size_thres_crossed.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void dl_data_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  if (ppi_present) {
    j.start_obj();
    j.write_int("id", 63);
    j.write_str("criticality", "ignore");
    j.write_int("value", ppi);
    j.end_obj();
  }
  if (pdu_session_to_notify_list_present) {
    j.start_obj();
    j.write_int("id", 67);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_to_notify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (mt_sdt_info_present) {
    j.start_obj();
    j.write_int("id", 194);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mt_sdt_info.to_json(j);
    j.end_obj();
  }
  if (sdt_data_size_thres_crossed_present) {
    j.start_obj();
    j.write_int("id", 197);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// DataUsageReportIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t data_usage_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 25};
  return map_enum_number(names, 3, idx, "id");
}
bool data_usage_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 25};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e data_usage_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 25:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e data_usage_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 25:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE data_usage_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 25, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, data_usage_report_list, 1, 32, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_usage_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &data_usage_report_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 25: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(data_usage_report_list, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void data_usage_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 25);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : data_usage_report_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// DeactivateTraceIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t deactiv_trace_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 82};
  return map_enum_number(names, 3, idx, "id");
}
bool deactiv_trace_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 82};
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
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 82:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e deactiv_trace_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 82:
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

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 82, crit_e{crit_e::ignore}, [this, &bref]() { return trace_id.pack(bref); }));

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
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 82: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 82);
  j.write_str("criticality", "ignore");
  j.write_str("value", trace_id.to_string());
  j.end_obj();
  j.end_array();
}

// E1ReleaseRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t e1_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 0};
  return map_enum_number(names, 2, idx, "id");
}
bool e1_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e1_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e1_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e1_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e1_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e1_release_request_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
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
void e1_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// E1ReleaseResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t e1_release_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool e1_release_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e1_release_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e1_release_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e1_release_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e1_release_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e1_release_resp_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
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
void e1_release_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
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

// EUTRAN-GNB-CU-UP-CounterCheckRequest ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t eutran_gnb_cu_up_counter_check_request_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {61};
  return map_enum_number(names, 1, idx, "id");
}
bool eutran_gnb_cu_up_counter_check_request_o::is_id_valid(const uint32_t& id)
{
  return 61 == id;
}
crit_e eutran_gnb_cu_up_counter_check_request_o::get_crit(const uint32_t& id)
{
  if (id == 61) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e eutran_gnb_cu_up_counter_check_request_o::get_presence(const uint32_t& id)
{
  if (id == 61) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// EarlyForwardingSNTransferIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t early_forwarding_sn_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 119};
  return map_enum_number(names, 3, idx, "id");
}
bool early_forwarding_sn_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 119};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e early_forwarding_sn_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 119:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e early_forwarding_sn_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 119:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE early_forwarding_sn_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 119, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, drbs_subject_to_early_forwarding_list, 1, 32, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE early_forwarding_sn_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &early_forwarding_sn_transfer_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 119: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(drbs_subject_to_early_forwarding_list, bref, 1, 32, true);
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
void early_forwarding_sn_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 119);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : drbs_subject_to_early_forwarding_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// ErrorIndication-IEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t error_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 2, 3, 0, 1, 155, 156};
  return map_enum_number(names, 7, idx, "id");
}
bool error_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 2, 3, 0, 1, 155, 156};
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
    case 57:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    case 3:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    case 155:
      return crit_e::ignore;
    case 156:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e error_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    case 3:
      return presence_e::optional;
    case 0:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    case 155:
      return presence_e::optional;
    case 156:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE error_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += gnb_cu_cp_ue_e1ap_id_present ? 1 : 0;
  nof_ies += gnb_cu_up_ue_e1ap_id_present ? 1 : 0;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += gnb_cu_cp_mbs_e1ap_id_present ? 1 : 0;
  nof_ies += gnb_cu_up_mbs_e1ap_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (gnb_cu_cp_ue_e1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (gnb_cu_up_ue_e1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (cause_present) {
    HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (gnb_cu_cp_mbs_e1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
    }));
  }
  if (gnb_cu_up_mbs_e1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
    }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &error_ind_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 2: {
        gnb_cu_cp_ue_e1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        gnb_cu_up_ue_e1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
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
      case 155: {
        gnb_cu_cp_mbs_e1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        gnb_cu_up_mbs_e1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
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
void error_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (gnb_cu_cp_ue_e1ap_id_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
    j.end_obj();
  }
  if (gnb_cu_up_ue_e1ap_id_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_cu_up_ue_e1ap_id);
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
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (gnb_cu_cp_mbs_e1ap_id_present) {
    j.start_obj();
    j.write_int("id", 155);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
    j.end_obj();
  }
  if (gnb_cu_up_mbs_e1ap_id_present) {
    j.start_obj();
    j.write_int("id", 156);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_up_mbs_e1ap_id);
    j.end_obj();
  }
  j.end_array();
}

// GNB-CU-CP-ConfigurationUpdateIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_cp_cfg_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 9, 27, 28, 29, 86, 129};
  return map_enum_number(names, 7, idx, "id");
}
bool gnb_cu_cp_cfg_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 9, 27, 28, 29, 86, 129};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cp_cfg_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 9:
      return crit_e::ignore;
    case 27:
      return crit_e::ignore;
    case 28:
      return crit_e::ignore;
    case 29:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    case 129:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cp_cfg_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 9:
      return presence_e::optional;
    case 27:
      return presence_e::optional;
    case 28:
      return presence_e::optional;
    case 29:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    case 129:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cp_cfg_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += gnb_cu_cp_name_present ? 1 : 0;
  nof_ies += gnb_cu_cp_tnl_a_to_add_list_present ? 1 : 0;
  nof_ies += gnb_cu_cp_tnl_a_to_rem_list_present ? 1 : 0;
  nof_ies += gnb_cu_cp_tnl_a_to_upd_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += extended_gnb_cu_cp_name_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (gnb_cu_cp_name_present) {
    HANDLE_CODE(pack_ie(bref, 9, crit_e{crit_e::ignore}, [this, &bref]() { return gnb_cu_cp_name.pack(bref); }));
  }
  if (gnb_cu_cp_tnl_a_to_add_list_present) {
    HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, gnb_cu_cp_tnl_a_to_add_list, 1, 32, true);
    }));
  }
  if (gnb_cu_cp_tnl_a_to_rem_list_present) {
    HANDLE_CODE(pack_ie(bref, 28, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, gnb_cu_cp_tnl_a_to_rem_list, 1, 32, true);
    }));
  }
  if (gnb_cu_cp_tnl_a_to_upd_list_present) {
    HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, gnb_cu_cp_tnl_a_to_upd_list, 1, 32, true);
    }));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return transport_layer_address_info.pack(bref); }));
  }
  if (extended_gnb_cu_cp_name_present) {
    HANDLE_CODE(
        pack_ie(bref, 129, crit_e{crit_e::ignore}, [this, &bref]() { return extended_gnb_cu_cp_name.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_cfg_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_cp_cfg_upd_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 9: {
        gnb_cu_cp_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_cp_name.unpack(bref); }));
        break;
      }
      case 27: {
        gnb_cu_cp_tnl_a_to_add_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(gnb_cu_cp_tnl_a_to_add_list, bref, 1, 32, true); }));
        break;
      }
      case 28: {
        gnb_cu_cp_tnl_a_to_rem_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(gnb_cu_cp_tnl_a_to_rem_list, bref, 1, 32, true); }));
        break;
      }
      case 29: {
        gnb_cu_cp_tnl_a_to_upd_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(gnb_cu_cp_tnl_a_to_upd_list, bref, 1, 32, true); }));
        break;
      }
      case 86: {
        transport_layer_address_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return transport_layer_address_info.unpack(bref); }));
        break;
      }
      case 129: {
        extended_gnb_cu_cp_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_gnb_cu_cp_name.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_cfg_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (gnb_cu_cp_name_present) {
    j.start_obj();
    j.write_int("id", 9);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_cu_cp_name.to_string());
    j.end_obj();
  }
  if (gnb_cu_cp_tnl_a_to_add_list_present) {
    j.start_obj();
    j.write_int("id", 27);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_cp_tnl_a_to_add_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_cp_tnl_a_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 28);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_cp_tnl_a_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_cp_tnl_a_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 29);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_cp_tnl_a_to_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (extended_gnb_cu_cp_name_present) {
    j.start_obj();
    j.write_int("id", 129);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_cu_cp_name.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNB-CU-CP-ConfigurationUpdateAcknowledgeIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_cp_cfg_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 1, 30, 31, 86};
  return map_enum_number(names, 5, idx, "id");
}
bool gnb_cu_cp_cfg_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 1, 30, 31, 86};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cp_cfg_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 30:
      return crit_e::ignore;
    case 31:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cp_cfg_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    case 30:
      return presence_e::optional;
    case 31:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cp_cfg_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += gnb_cu_cp_tnl_a_setup_list_present ? 1 : 0;
  nof_ies += gnb_cu_cp_tnl_a_failed_to_setup_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (gnb_cu_cp_tnl_a_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 30, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, gnb_cu_cp_tnl_a_setup_list, 1, 32, true);
    }));
  }
  if (gnb_cu_cp_tnl_a_failed_to_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 31, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, gnb_cu_cp_tnl_a_failed_to_setup_list, 1, 32, true);
    }));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return transport_layer_address_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_cfg_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_cp_cfg_upd_ack_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 30: {
        gnb_cu_cp_tnl_a_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(gnb_cu_cp_tnl_a_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 31: {
        gnb_cu_cp_tnl_a_failed_to_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(gnb_cu_cp_tnl_a_failed_to_setup_list, bref, 1, 32, true);
        }));
        break;
      }
      case 86: {
        transport_layer_address_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return transport_layer_address_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_cfg_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (gnb_cu_cp_tnl_a_setup_list_present) {
    j.start_obj();
    j.write_int("id", 30);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_cp_tnl_a_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_cp_tnl_a_failed_to_setup_list_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_cp_tnl_a_failed_to_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNB-CU-CP-ConfigurationUpdateFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_cp_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_cu_cp_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cp_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 12:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cp_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 12:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cp_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 12, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_cp_cfg_upd_fail_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 12: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
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
void gnb_cu_cp_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
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
    j.write_int("id", 12);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
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

// GNB-CU-CP-E1SetupFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_cp_e1_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_cu_cp_e1_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cp_e1_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 12:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cp_e1_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 12:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cp_e1_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 12, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_e1_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_cp_e1_setup_fail_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 12: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
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
void gnb_cu_cp_e1_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
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
    j.write_int("id", 12);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
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

// GNB-CU-CP-E1SetupRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_cp_e1_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 9, 86, 129};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_cu_cp_e1_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 9, 86, 129};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cp_e1_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 9:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    case 129:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cp_e1_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 9:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    case 129:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cp_e1_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += gnb_cu_cp_name_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += extended_gnb_cu_cp_name_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (gnb_cu_cp_name_present) {
    HANDLE_CODE(pack_ie(bref, 9, crit_e{crit_e::ignore}, [this, &bref]() { return gnb_cu_cp_name.pack(bref); }));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return transport_layer_address_info.pack(bref); }));
  }
  if (extended_gnb_cu_cp_name_present) {
    HANDLE_CODE(
        pack_ie(bref, 129, crit_e{crit_e::ignore}, [this, &bref]() { return extended_gnb_cu_cp_name.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_e1_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_cp_e1_setup_request_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 9: {
        gnb_cu_cp_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_cp_name.unpack(bref); }));
        break;
      }
      case 86: {
        transport_layer_address_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return transport_layer_address_info.unpack(bref); }));
        break;
      }
      case 129: {
        extended_gnb_cu_cp_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_gnb_cu_cp_name.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_e1_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (gnb_cu_cp_name_present) {
    j.start_obj();
    j.write_int("id", 9);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_cu_cp_name.to_string());
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (extended_gnb_cu_cp_name_present) {
    j.start_obj();
    j.write_int("id", 129);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_cu_cp_name.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SupportedPLMNs-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t supported_plmns_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {110, 125, 135, 145};
  return map_enum_number(names, 4, idx, "id");
}
bool supported_plmns_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {110, 125, 135, 145};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e supported_plmns_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 110:
      return crit_e::reject;
    case 125:
      return crit_e::reject;
    case 135:
      return crit_e::ignore;
    case 145:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e supported_plmns_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 110:
      return presence_e::optional;
    case 125:
      return presence_e::optional;
    case 135:
      return presence_e::optional;
    case 145:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE supported_plmns_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += npn_support_info_present ? 1 : 0;
  nof_ies += extended_slice_support_list_present ? 1 : 0;
  nof_ies += extended_nr_cgi_support_list_present ? 1 : 0;
  nof_ies += ecgi_support_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (npn_support_info_present) {
    HANDLE_CODE(pack_ie(bref, 110, crit_e{crit_e::reject}, [this, &bref]() { return npn_support_info.pack(bref); }));
  }
  if (extended_slice_support_list_present) {
    HANDLE_CODE(pack_ie(bref, 125, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, extended_slice_support_list, 1, 65535, true);
    }));
  }
  if (extended_nr_cgi_support_list_present) {
    HANDLE_CODE(pack_ie(bref, 135, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, extended_nr_cgi_support_list, 1, 16384, true);
    }));
  }
  if (ecgi_support_list_present) {
    HANDLE_CODE(pack_ie(bref, 145, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ecgi_support_list, 1, 512, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE supported_plmns_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &supported_plmns_ext_ies_o::get_crit));

    switch (id) {
      case 110: {
        npn_support_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return npn_support_info.unpack(bref); }));
        break;
      }
      case 125: {
        extended_slice_support_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(extended_slice_support_list, bref, 1, 65535, true); }));
        break;
      }
      case 135: {
        extended_nr_cgi_support_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(extended_nr_cgi_support_list, bref, 1, 16384, true); }));
        break;
      }
      case 145: {
        ecgi_support_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ecgi_support_list, bref, 1, 512, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void supported_plmns_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (npn_support_info_present) {
    j.start_obj();
    j.write_int("id", 110);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    npn_support_info.to_json(j);
    j.end_obj();
  }
  if (extended_slice_support_list_present) {
    j.start_obj();
    j.write_int("id", 125);
    j.write_str("criticality", "reject");
    j.start_array("extensionValue");
    for (const auto& e1 : extended_slice_support_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (extended_nr_cgi_support_list_present) {
    j.start_obj();
    j.write_int("id", 135);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : extended_nr_cgi_support_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ecgi_support_list_present) {
    j.start_obj();
    j.write_int("id", 145);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : ecgi_support_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// SupportedPLMNs-Item ::= SEQUENCE
OCUDUASN_CODE supported_plmns_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(slice_support_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(nr_cgi_support_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(qos_params_support_list_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.pack(bref));
  if (slice_support_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, slice_support_list, 1, 1024, true));
  }
  if (nr_cgi_support_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, nr_cgi_support_list, 1, 512, true));
  }
  if (qos_params_support_list_present) {
    HANDLE_CODE(qos_params_support_list.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE supported_plmns_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool slice_support_list_present;
  HANDLE_CODE(bref.unpack(slice_support_list_present, 1));
  bool nr_cgi_support_list_present;
  HANDLE_CODE(bref.unpack(nr_cgi_support_list_present, 1));
  HANDLE_CODE(bref.unpack(qos_params_support_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.unpack(bref));
  if (slice_support_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(slice_support_list, bref, 1, 1024, true));
  }
  if (nr_cgi_support_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(nr_cgi_support_list, bref, 1, 512, true));
  }
  if (qos_params_support_list_present) {
    HANDLE_CODE(qos_params_support_list.unpack(bref));
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
void supported_plmns_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  if (slice_support_list.size() > 0) {
    j.start_array("slice-Support-List");
    for (const auto& e1 : slice_support_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (nr_cgi_support_list.size() > 0) {
    j.start_array("nR-CGI-Support-List");
    for (const auto& e1 : nr_cgi_support_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (qos_params_support_list_present) {
    j.write_fieldname("qoS-Parameters-Support-List");
    qos_params_support_list.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GNB-CU-CP-E1SetupResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_cp_e1_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 7, 8, 10, 11, 64, 86, 130, 1};
  return map_enum_number(names, 9, idx, "id");
}
bool gnb_cu_cp_e1_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 7, 8, 10, 11, 64, 86, 130, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cp_e1_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 7:
      return crit_e::reject;
    case 8:
      return crit_e::ignore;
    case 10:
      return crit_e::reject;
    case 11:
      return crit_e::reject;
    case 64:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    case 130:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cp_e1_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 8:
      return presence_e::optional;
    case 10:
      return presence_e::mandatory;
    case 11:
      return presence_e::mandatory;
    case 64:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cp_e1_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += gnb_cu_up_name_present ? 1 : 0;
  nof_ies += gnb_cu_up_capacity_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += extended_gnb_cu_up_name_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_id, (uint64_t)0u, (uint64_t)68719476735u, false, true);
  }));
  if (gnb_cu_up_name_present) {
    HANDLE_CODE(pack_ie(bref, 8, crit_e{crit_e::ignore}, [this, &bref]() { return gnb_cu_up_name.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() { return cn_support.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 11, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, supported_plmns, 1, 12, true);
  }));
  if (gnb_cu_up_capacity_present) {
    HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_capacity, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return transport_layer_address_info.pack(bref); }));
  }
  if (extended_gnb_cu_up_name_present) {
    HANDLE_CODE(
        pack_ie(bref, 130, crit_e{crit_e::ignore}, [this, &bref]() { return extended_gnb_cu_up_name.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_e1_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_cp_e1_setup_resp_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true);
        }));
        break;
      }
      case 8: {
        gnb_cu_up_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_up_name.unpack(bref); }));
        break;
      }
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cn_support.unpack(bref); }));
        break;
      }
      case 11: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(supported_plmns, bref, 1, 12, true); }));
        break;
      }
      case 64: {
        gnb_cu_up_capacity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_capacity, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 86: {
        transport_layer_address_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return transport_layer_address_info.unpack(bref); }));
        break;
      }
      case 130: {
        extended_gnb_cu_up_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_gnb_cu_up_name.unpack(bref); }));
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
void gnb_cu_cp_e1_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_id);
  j.end_obj();
  if (gnb_cu_up_name_present) {
    j.start_obj();
    j.write_int("id", 8);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_cu_up_name.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_str("value", cn_support.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 11);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : supported_plmns) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (gnb_cu_up_capacity_present) {
    j.start_obj();
    j.write_int("id", 64);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_up_capacity);
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (extended_gnb_cu_up_name_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_cu_up_name.to_json(j);
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

// GNB-CU-CPMeasurementResultsInformationIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_cp_meas_results_info_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 128};
  return map_enum_number(names, 3, idx, "id");
}
bool gnb_cu_cp_meas_results_info_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 128};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cp_meas_results_info_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 128:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cp_meas_results_info_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 128:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cp_meas_results_info_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 128, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, drb_meas_results_info_list, 1, 32, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_meas_results_info_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_cp_meas_results_info_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 128: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drb_meas_results_info_list, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_meas_results_info_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 128);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : drb_meas_results_info_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// GNB-CU-UP-ConfigurationUpdateIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_up_cfg_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 7, 8, 11, 64, 73, 86, 130, 171};
  return map_enum_number(names, 9, idx, "id");
}
bool gnb_cu_up_cfg_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 7, 8, 11, 64, 73, 86, 130, 171};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_up_cfg_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 7:
      return crit_e::reject;
    case 8:
      return crit_e::ignore;
    case 11:
      return crit_e::reject;
    case 64:
      return crit_e::ignore;
    case 73:
      return crit_e::reject;
    case 86:
      return crit_e::ignore;
    case 130:
      return crit_e::ignore;
    case 171:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_up_cfg_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 8:
      return presence_e::optional;
    case 11:
      return presence_e::optional;
    case 64:
      return presence_e::optional;
    case 73:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    case 171:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_up_cfg_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += gnb_cu_up_name_present ? 1 : 0;
  nof_ies += supported_plmns_present ? 1 : 0;
  nof_ies += gnb_cu_up_capacity_present ? 1 : 0;
  nof_ies += gnb_cu_up_tnl_a_to_rem_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += extended_gnb_cu_up_name_present ? 1 : 0;
  nof_ies += gnb_cu_up_mbs_support_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_id, (uint64_t)0u, (uint64_t)68719476735u, false, true);
  }));
  if (gnb_cu_up_name_present) {
    HANDLE_CODE(pack_ie(bref, 8, crit_e{crit_e::ignore}, [this, &bref]() { return gnb_cu_up_name.pack(bref); }));
  }
  if (supported_plmns_present) {
    HANDLE_CODE(pack_ie(bref, 11, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, supported_plmns, 1, 12, true);
    }));
  }
  if (gnb_cu_up_capacity_present) {
    HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_capacity, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (gnb_cu_up_tnl_a_to_rem_list_present) {
    HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, gnb_cu_up_tnl_a_to_rem_list, 1, 32, true);
    }));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return transport_layer_address_info.pack(bref); }));
  }
  if (extended_gnb_cu_up_name_present) {
    HANDLE_CODE(
        pack_ie(bref, 130, crit_e{crit_e::ignore}, [this, &bref]() { return extended_gnb_cu_up_name.pack(bref); }));
  }
  if (gnb_cu_up_mbs_support_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 171, crit_e{crit_e::reject}, [this, &bref]() { return gnb_cu_up_mbs_support_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_cfg_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_up_cfg_upd_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true);
        }));
        break;
      }
      case 8: {
        gnb_cu_up_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_up_name.unpack(bref); }));
        break;
      }
      case 11: {
        supported_plmns_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(supported_plmns, bref, 1, 12, true); }));
        break;
      }
      case 64: {
        gnb_cu_up_capacity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_capacity, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 73: {
        gnb_cu_up_tnl_a_to_rem_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(gnb_cu_up_tnl_a_to_rem_list, bref, 1, 32, true); }));
        break;
      }
      case 86: {
        transport_layer_address_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return transport_layer_address_info.unpack(bref); }));
        break;
      }
      case 130: {
        extended_gnb_cu_up_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_gnb_cu_up_name.unpack(bref); }));
        break;
      }
      case 171: {
        gnb_cu_up_mbs_support_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_up_mbs_support_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_up_cfg_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_id);
  j.end_obj();
  if (gnb_cu_up_name_present) {
    j.start_obj();
    j.write_int("id", 8);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_cu_up_name.to_string());
    j.end_obj();
  }
  if (supported_plmns_present) {
    j.start_obj();
    j.write_int("id", 11);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : supported_plmns) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_up_capacity_present) {
    j.start_obj();
    j.write_int("id", 64);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_up_capacity);
    j.end_obj();
  }
  if (gnb_cu_up_tnl_a_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 73);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_up_tnl_a_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (extended_gnb_cu_up_name_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_cu_up_name.to_json(j);
    j.end_obj();
  }
  if (gnb_cu_up_mbs_support_info_present) {
    j.start_obj();
    j.write_int("id", 171);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    gnb_cu_up_mbs_support_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNB-CU-UP-ConfigurationUpdateAcknowledgeIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_up_cfg_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 1, 86};
  return map_enum_number(names, 3, idx, "id");
}
bool gnb_cu_up_cfg_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 1, 86};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_up_cfg_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_up_cfg_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_up_cfg_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return transport_layer_address_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_cfg_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_up_cfg_upd_ack_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 86: {
        transport_layer_address_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return transport_layer_address_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_up_cfg_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNB-CU-UP-ConfigurationUpdateFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_up_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_cu_up_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_up_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 12:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_up_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 12:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_up_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 12, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_up_cfg_upd_fail_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 12: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
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
void gnb_cu_up_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
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
    j.write_int("id", 12);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
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

OCUDUASN_CODE eutran_gnb_cu_up_counter_check_request_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 61, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, drbs_subject_to_counter_check_list_eutran, 1, 32, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_gnb_cu_up_counter_check_request_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &eutran_gnb_cu_up_counter_check_request_o::get_crit));

    if (id == 61) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(drbs_subject_to_counter_check_list_eutran, bref, 1, 32, true);
        }));
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
void eutran_gnb_cu_up_counter_check_request_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 61);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : drbs_subject_to_counter_check_list_eutran) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// NG-RAN-GNB-CU-UP-CounterCheckRequest ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ng_ran_gnb_cu_up_counter_check_request_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {62};
  return map_enum_number(names, 1, idx, "id");
}
bool ng_ran_gnb_cu_up_counter_check_request_o::is_id_valid(const uint32_t& id)
{
  return 62 == id;
}
crit_e ng_ran_gnb_cu_up_counter_check_request_o::get_crit(const uint32_t& id)
{
  if (id == 62) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e ng_ran_gnb_cu_up_counter_check_request_o::get_presence(const uint32_t& id)
{
  if (id == 62) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE ng_ran_gnb_cu_up_counter_check_request_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 62, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, drbs_subject_to_counter_check_list_ng_ran, 1, 32, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_gnb_cu_up_counter_check_request_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_ran_gnb_cu_up_counter_check_request_o::get_crit));

    if (id == 62) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(drbs_subject_to_counter_check_list_ng_ran, bref, 1, 32, true);
        }));
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
void ng_ran_gnb_cu_up_counter_check_request_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 62);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : drbs_subject_to_counter_check_list_ng_ran) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// System-GNB-CU-UP-CounterCheckRequest ::= CHOICE
void sys_gnb_cu_up_counter_check_request_c::destroy_()
{
  switch (type_) {
    case types::e_utran_gnb_cu_up_counter_check_request:
      c.destroy<eutran_gnb_cu_up_counter_check_request_container>();
      break;
    case types::ng_ran_gnb_cu_up_counter_check_request:
      c.destroy<ng_ran_gnb_cu_up_counter_check_request_container>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void sys_gnb_cu_up_counter_check_request_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::e_utran_gnb_cu_up_counter_check_request:
      c.init<eutran_gnb_cu_up_counter_check_request_container>();
      break;
    case types::ng_ran_gnb_cu_up_counter_check_request:
      c.init<ng_ran_gnb_cu_up_counter_check_request_container>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_gnb_cu_up_counter_check_request_c");
  }
}
sys_gnb_cu_up_counter_check_request_c::sys_gnb_cu_up_counter_check_request_c(
    const sys_gnb_cu_up_counter_check_request_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::e_utran_gnb_cu_up_counter_check_request:
      c.init(other.c.get<eutran_gnb_cu_up_counter_check_request_container>());
      break;
    case types::ng_ran_gnb_cu_up_counter_check_request:
      c.init(other.c.get<ng_ran_gnb_cu_up_counter_check_request_container>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_gnb_cu_up_counter_check_request_c");
  }
}
sys_gnb_cu_up_counter_check_request_c&
sys_gnb_cu_up_counter_check_request_c::operator=(const sys_gnb_cu_up_counter_check_request_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::e_utran_gnb_cu_up_counter_check_request:
      c.set(other.c.get<eutran_gnb_cu_up_counter_check_request_container>());
      break;
    case types::ng_ran_gnb_cu_up_counter_check_request:
      c.set(other.c.get<ng_ran_gnb_cu_up_counter_check_request_container>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sys_gnb_cu_up_counter_check_request_c");
  }

  return *this;
}
eutran_gnb_cu_up_counter_check_request_container&
sys_gnb_cu_up_counter_check_request_c::set_e_utran_gnb_cu_up_counter_check_request()
{
  set(types::e_utran_gnb_cu_up_counter_check_request);
  return c.get<eutran_gnb_cu_up_counter_check_request_container>();
}
ng_ran_gnb_cu_up_counter_check_request_container&
sys_gnb_cu_up_counter_check_request_c::set_ng_ran_gnb_cu_up_counter_check_request()
{
  set(types::ng_ran_gnb_cu_up_counter_check_request);
  return c.get<ng_ran_gnb_cu_up_counter_check_request_container>();
}
protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>&
sys_gnb_cu_up_counter_check_request_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>>();
}
void sys_gnb_cu_up_counter_check_request_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::e_utran_gnb_cu_up_counter_check_request:
      j.write_fieldname("e-UTRAN-GNB-CU-UP-CounterCheckRequest");
      c.get<eutran_gnb_cu_up_counter_check_request_container>().to_json(j);
      break;
    case types::ng_ran_gnb_cu_up_counter_check_request:
      j.write_fieldname("nG-RAN-GNB-CU-UP-CounterCheckRequest");
      c.get<ng_ran_gnb_cu_up_counter_check_request_container>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "sys_gnb_cu_up_counter_check_request_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sys_gnb_cu_up_counter_check_request_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::e_utran_gnb_cu_up_counter_check_request:
      HANDLE_CODE(c.get<eutran_gnb_cu_up_counter_check_request_container>().pack(bref));
      break;
    case types::ng_ran_gnb_cu_up_counter_check_request:
      HANDLE_CODE(c.get<ng_ran_gnb_cu_up_counter_check_request_container>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_gnb_cu_up_counter_check_request_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_gnb_cu_up_counter_check_request_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::e_utran_gnb_cu_up_counter_check_request:
      HANDLE_CODE(c.get<eutran_gnb_cu_up_counter_check_request_container>().unpack(bref));
      break;
    case types::ng_ran_gnb_cu_up_counter_check_request:
      HANDLE_CODE(c.get<ng_ran_gnb_cu_up_counter_check_request_container>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<sys_gnb_cu_up_counter_check_request_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sys_gnb_cu_up_counter_check_request_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sys_gnb_cu_up_counter_check_request_c::types_opts::to_string() const
{
  static const char* names[] = {
      "e-UTRAN-GNB-CU-UP-CounterCheckRequest", "nG-RAN-GNB-CU-UP-CounterCheckRequest", "choice-extension"};
  return convert_enum_idx(names, 3, value, "sys_gnb_cu_up_counter_check_request_c::types");
}

// GNB-CU-UP-CounterCheckRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_up_counter_check_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 60};
  return map_enum_number(names, 3, idx, "id");
}
bool gnb_cu_up_counter_check_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 60};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_up_counter_check_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 60:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_up_counter_check_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 60:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_up_counter_check_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 60, crit_e{crit_e::reject}, [this, &bref]() { return sys_gnb_cu_up_counter_check_request.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_counter_check_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_up_counter_check_request_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 60: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sys_gnb_cu_up_counter_check_request.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_up_counter_check_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 60);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  sys_gnb_cu_up_counter_check_request.to_json(j);
  j.end_obj();
  j.end_array();
}

// GNB-CU-UP-E1SetupFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_up_e1_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_cu_up_e1_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_up_e1_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 12:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_up_e1_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 12:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_up_e1_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 12, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_e1_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_up_e1_setup_fail_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 12: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
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
void gnb_cu_up_e1_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
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
    j.write_int("id", 12);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
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

// GNB-CU-UP-E1SetupRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_up_e1_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 7, 8, 10, 11, 64, 86, 130, 171};
  return map_enum_number(names, 9, idx, "id");
}
bool gnb_cu_up_e1_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 7, 8, 10, 11, 64, 86, 130, 171};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_up_e1_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 7:
      return crit_e::reject;
    case 8:
      return crit_e::ignore;
    case 10:
      return crit_e::reject;
    case 11:
      return crit_e::reject;
    case 64:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    case 130:
      return crit_e::ignore;
    case 171:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_up_e1_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 8:
      return presence_e::optional;
    case 10:
      return presence_e::mandatory;
    case 11:
      return presence_e::mandatory;
    case 64:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    case 171:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_up_e1_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += gnb_cu_up_name_present ? 1 : 0;
  nof_ies += gnb_cu_up_capacity_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += extended_gnb_cu_up_name_present ? 1 : 0;
  nof_ies += gnb_cu_up_mbs_support_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_id, (uint64_t)0u, (uint64_t)68719476735u, false, true);
  }));
  if (gnb_cu_up_name_present) {
    HANDLE_CODE(pack_ie(bref, 8, crit_e{crit_e::ignore}, [this, &bref]() { return gnb_cu_up_name.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() { return cn_support.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 11, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, supported_plmns, 1, 12, true);
  }));
  if (gnb_cu_up_capacity_present) {
    HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_capacity, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return transport_layer_address_info.pack(bref); }));
  }
  if (extended_gnb_cu_up_name_present) {
    HANDLE_CODE(
        pack_ie(bref, 130, crit_e{crit_e::ignore}, [this, &bref]() { return extended_gnb_cu_up_name.pack(bref); }));
  }
  if (gnb_cu_up_mbs_support_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 171, crit_e{crit_e::reject}, [this, &bref]() { return gnb_cu_up_mbs_support_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_e1_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_up_e1_setup_request_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true);
        }));
        break;
      }
      case 8: {
        gnb_cu_up_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_up_name.unpack(bref); }));
        break;
      }
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cn_support.unpack(bref); }));
        break;
      }
      case 11: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(supported_plmns, bref, 1, 12, true); }));
        break;
      }
      case 64: {
        gnb_cu_up_capacity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_capacity, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 86: {
        transport_layer_address_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return transport_layer_address_info.unpack(bref); }));
        break;
      }
      case 130: {
        extended_gnb_cu_up_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_gnb_cu_up_name.unpack(bref); }));
        break;
      }
      case 171: {
        gnb_cu_up_mbs_support_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_up_mbs_support_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_up_e1_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_id);
  j.end_obj();
  if (gnb_cu_up_name_present) {
    j.start_obj();
    j.write_int("id", 8);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_cu_up_name.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_str("value", cn_support.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 11);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : supported_plmns) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (gnb_cu_up_capacity_present) {
    j.start_obj();
    j.write_int("id", 64);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_up_capacity);
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (extended_gnb_cu_up_name_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_cu_up_name.to_json(j);
    j.end_obj();
  }
  if (gnb_cu_up_mbs_support_info_present) {
    j.start_obj();
    j.write_int("id", 171);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    gnb_cu_up_mbs_support_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNB-CU-UP-E1SetupResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_up_e1_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 9, 86, 129, 1};
  return map_enum_number(names, 5, idx, "id");
}
bool gnb_cu_up_e1_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 9, 86, 129, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_up_e1_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 9:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    case 129:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_up_e1_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 9:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    case 129:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_up_e1_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += gnb_cu_cp_name_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += extended_gnb_cu_cp_name_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (gnb_cu_cp_name_present) {
    HANDLE_CODE(pack_ie(bref, 9, crit_e{crit_e::ignore}, [this, &bref]() { return gnb_cu_cp_name.pack(bref); }));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return transport_layer_address_info.pack(bref); }));
  }
  if (extended_gnb_cu_cp_name_present) {
    HANDLE_CODE(
        pack_ie(bref, 129, crit_e{crit_e::ignore}, [this, &bref]() { return extended_gnb_cu_cp_name.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_e1_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_up_e1_setup_resp_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 9: {
        gnb_cu_cp_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_cp_name.unpack(bref); }));
        break;
      }
      case 86: {
        transport_layer_address_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return transport_layer_address_info.unpack(bref); }));
        break;
      }
      case 129: {
        extended_gnb_cu_cp_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_gnb_cu_cp_name.unpack(bref); }));
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
void gnb_cu_up_e1_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (gnb_cu_cp_name_present) {
    j.start_obj();
    j.write_int("id", 9);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_cu_cp_name.to_string());
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (extended_gnb_cu_cp_name_present) {
    j.start_obj();
    j.write_int("id", 129);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_cu_cp_name.to_json(j);
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

// GNB-CU-UP-StatusIndicationIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t gnb_cu_up_status_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 65};
  return map_enum_number(names, 2, idx, "id");
}
bool gnb_cu_up_status_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 65};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_up_status_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 65:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_up_status_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 65:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_up_status_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 65, crit_e{crit_e::reject}, [this, &bref]() { return gnb_cu_up_overload_info.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_status_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &gnb_cu_up_status_ind_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 65: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_cu_up_overload_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_up_status_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 65);
  j.write_str("criticality", "reject");
  j.write_str("value", gnb_cu_up_overload_info.to_string());
  j.end_obj();
  j.end_array();
}

// IAB-UPTNLAddressUpdateIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t iab_up_tnl_address_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 108};
  return map_enum_number(names, 2, idx, "id");
}
bool iab_up_tnl_address_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 108};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_up_tnl_address_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 108:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_up_tnl_address_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 108:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_up_tnl_address_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += dl_up_tnl_address_to_upd_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (dl_up_tnl_address_to_upd_list_present) {
    HANDLE_CODE(pack_ie(bref, 108, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, dl_up_tnl_address_to_upd_list, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_up_tnl_address_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_up_tnl_address_upd_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 108: {
        dl_up_tnl_address_to_upd_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(dl_up_tnl_address_to_upd_list, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_up_tnl_address_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (dl_up_tnl_address_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 108);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : dl_up_tnl_address_to_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// IAB-UPTNLAddressUpdateAcknowledgeIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t iab_up_tnl_address_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 1, 109};
  return map_enum_number(names, 3, idx, "id");
}
bool iab_up_tnl_address_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 1, 109};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_up_tnl_address_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 109:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_up_tnl_address_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    case 109:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_up_tnl_address_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += ul_up_tnl_address_to_upd_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (ul_up_tnl_address_to_upd_list_present) {
    HANDLE_CODE(pack_ie(bref, 109, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ul_up_tnl_address_to_upd_list, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_up_tnl_address_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_up_tnl_address_upd_ack_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 109: {
        ul_up_tnl_address_to_upd_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ul_up_tnl_address_to_upd_list, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_up_tnl_address_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (ul_up_tnl_address_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 109);
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

// IAB-UPTNLAddressUpdateFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t iab_up_tnl_address_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool iab_up_tnl_address_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 0, 12, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_up_tnl_address_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 12:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_up_tnl_address_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 12:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_up_tnl_address_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 12, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_up_tnl_address_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_up_tnl_address_upd_fail_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 12: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
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
void iab_up_tnl_address_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
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
    j.write_int("id", 12);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
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

// IABPSKNotificationIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t iabpsk_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 144};
  return map_enum_number(names, 2, idx, "id");
}
bool iabpsk_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 144};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iabpsk_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 144:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iabpsk_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 144:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iabpsk_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 144, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, iab_donor_cu_up_psk_info, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iabpsk_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iabpsk_notif_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 144: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(iab_donor_cu_up_psk_info, bref, 1, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iabpsk_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 144);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : iab_donor_cu_up_psk_info) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// MCBearerContextModificationConfirmIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_mod_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 169, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool mc_bearer_context_mod_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 169, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_mod_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 169:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_mod_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 169:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_mod_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 169, crit_e{crit_e::reject}, [this, &bref]() { return mc_bearer_context_to_modify_confirm.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_mod_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_mod_confirm_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 169: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return mc_bearer_context_to_modify_confirm.unpack(bref); }));
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
void mc_bearer_context_mod_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 169);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mc_bearer_context_to_modify_confirm.to_json(j);
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

// MCBearerContextModificationFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 170, 0, 1};
  return map_enum_number(names, 5, idx, "id");
}
bool mc_bearer_context_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 170, 0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_mod_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 170:
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
presence_e mc_bearer_context_mod_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 170:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_mod_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += mbs_multicast_f1_u_context_descriptor_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(pack_ie(bref, 170, crit_e{crit_e::reject}, [this, &bref]() {
      return mbs_multicast_f1_u_context_descriptor.pack(bref);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_mod_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_mod_fail_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 170: {
        mbs_multicast_f1_u_context_descriptor_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return mbs_multicast_f1_u_context_descriptor.unpack(bref); }));
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
void mc_bearer_context_mod_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  if (mbs_multicast_f1_u_context_descriptor_present) {
    j.start_obj();
    j.write_int("id", 170);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    mbs_multicast_f1_u_context_descriptor.to_json(j);
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
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MCBearerContextModificationRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 166};
  return map_enum_number(names, 3, idx, "id");
}
bool mc_bearer_context_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 166};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 166:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 166:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 166, crit_e{crit_e::reject}, [this, &bref]() { return mc_bearer_context_to_modify.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_mod_request_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 166: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mc_bearer_context_to_modify.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 166);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mc_bearer_context_to_modify.to_json(j);
  j.end_obj();
  j.end_array();
}

// MCBearerContextModificationRequiredIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_mod_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 168};
  return map_enum_number(names, 3, idx, "id");
}
bool mc_bearer_context_mod_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 168};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_mod_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 168:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_mod_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 168:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_mod_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 168, crit_e{crit_e::reject}, [this, &bref]() { return mc_bearer_context_to_modify_required.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_mod_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_mod_required_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 168: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return mc_bearer_context_to_modify_required.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_mod_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 168);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mc_bearer_context_to_modify_required.to_json(j);
  j.end_obj();
  j.end_array();
}

// MCBearerContextModificationResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 167, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool mc_bearer_context_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 167, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 167:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 167:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 167, crit_e{crit_e::reject}, [this, &bref]() { return mc_bearer_context_to_modify_resp.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_mod_resp_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 167: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mc_bearer_context_to_modify_resp.unpack(bref); }));
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
void mc_bearer_context_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 167);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mc_bearer_context_to_modify_resp.to_json(j);
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

// MCBearerContextReleaseCommandIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool mc_bearer_context_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_release_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_release_cmd_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
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
void mc_bearer_context_release_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// MCBearerContextReleaseCompleteIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_release_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 1};
  return map_enum_number(names, 3, idx, "id");
}
bool mc_bearer_context_release_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_release_complete_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_release_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_release_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_release_complete_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_release_complete_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
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
void mc_bearer_context_release_complete_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
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

// MCBearerContextReleaseRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool mc_bearer_context_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_release_request_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
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
void mc_bearer_context_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// MCBearerContextSetupFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 0, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool mc_bearer_context_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += gnb_cu_up_mbs_e1ap_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  if (gnb_cu_up_mbs_e1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_setup_fail_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        gnb_cu_up_mbs_e1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
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
void mc_bearer_context_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  if (gnb_cu_up_mbs_e1ap_id_present) {
    j.start_obj();
    j.write_int("id", 156);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_up_mbs_e1ap_id);
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
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MCBearerContextSetupRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 157, 164};
  return map_enum_number(names, 3, idx, "id");
}
bool mc_bearer_context_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 157, 164};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 157:
      return crit_e::reject;
    case 164:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 157:
      return presence_e::mandatory;
    case 164:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 157, crit_e{crit_e::reject}, [this, &bref]() { return global_mbs_session_id.pack(bref); }));
  HANDLE_CODE(
      pack_ie(bref, 164, crit_e{crit_e::reject}, [this, &bref]() { return mc_bearer_context_to_setup.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_setup_request_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 157: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_mbs_session_id.unpack(bref); }));
        break;
      }
      case 164: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mc_bearer_context_to_setup.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 157);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 164);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mc_bearer_context_to_setup.to_json(j);
  j.end_obj();
  j.end_array();
}

// MCBearerContextSetupResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_context_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 165, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool mc_bearer_context_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 165, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 165:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 165:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 165, crit_e{crit_e::reject}, [this, &bref]() { return mc_bearer_context_to_setup_resp.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_context_setup_resp_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 165: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mc_bearer_context_to_setup_resp.unpack(bref); }));
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
void mc_bearer_context_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 165);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mc_bearer_context_to_setup_resp.to_json(j);
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

// MCBearerNotificationIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mc_bearer_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 156, 191};
  return map_enum_number(names, 3, idx, "id");
}
bool mc_bearer_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 156, 191};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    case 191:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 191:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 155, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_mbs_e1ap_id, (uint32_t)0u, (uint32_t)16777215u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 156, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_mbs_e1ap_id, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 191, crit_e{crit_e::ignore}, [this, &bref]() { return mbs_session_res_notif.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mc_bearer_notif_ies_o::get_crit));

    switch (id) {
      case 155: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)16777215u, false, true);
        }));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_mbs_e1ap_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 191: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_res_notif.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mc_bearer_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 155);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_up_mbs_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 191);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  mbs_session_res_notif.to_json(j);
  j.end_obj();
  j.end_array();
}

// MRDC-DataUsageReportIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t mrdc_data_usage_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 68};
  return map_enum_number(names, 3, idx, "id");
}
bool mrdc_data_usage_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 68};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mrdc_data_usage_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 68:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mrdc_data_usage_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 68:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mrdc_data_usage_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 68, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_data_usage_list, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrdc_data_usage_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mrdc_data_usage_report_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 68: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_data_usage_list, bref, 1, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mrdc_data_usage_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 68);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_data_usage_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// Value ::= OPEN TYPE
void e1ap_private_ies_empty_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.end_obj();
}
OCUDUASN_CODE e1ap_private_ies_empty_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e1ap_private_ies_empty_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}

const char* e1ap_private_ies_empty_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {};
  return convert_enum_idx(names, 0, value, "e1ap_private_ies_empty_o::value_c::types");
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

// ResetAll ::= ENUMERATED
const char* reset_all_opts::to_string() const
{
  static const char* names[] = {"reset-all"};
  return convert_enum_idx(names, 1, value, "reset_all_e");
}

// UE-associatedLogicalE1-ConnectionItemRes ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ue_associated_lc_e1_conn_item_res_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {5};
  return map_enum_number(names, 1, idx, "id");
}
bool ue_associated_lc_e1_conn_item_res_o::is_id_valid(const uint32_t& id)
{
  return 5 == id;
}
crit_e ue_associated_lc_e1_conn_item_res_o::get_crit(const uint32_t& id)
{
  if (id == 5) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ue_associated_lc_e1_conn_item_res_o::value_c ue_associated_lc_e1_conn_item_res_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 5) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ue_associated_lc_e1_conn_item_res_o::get_presence(const uint32_t& id)
{
  if (id == 5) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ue_associated_lc_e1_conn_item_res_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ue_associated_lc_e1_conn_item_res_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_associated_lc_e1_conn_item_res_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ue_associated_lc_e1_conn_item_res_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"UE-associatedLogicalE1-ConnectionItem"};
  return convert_enum_idx(names, 1, value, "ue_associated_lc_e1_conn_item_res_o::value_c::types");
}
uint8_t ue_associated_lc_e1_conn_item_res_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ue_associated_lc_e1_conn_item_res_o::value_c::types");
}

// ResetType ::= CHOICE
void reset_type_c::destroy_()
{
  switch (type_) {
    case types::part_of_e1_interface:
      c.destroy<ue_associated_lc_e1_conn_list_res_l>();
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
    case types::e1_interface:
      break;
    case types::part_of_e1_interface:
      c.init<ue_associated_lc_e1_conn_list_res_l>();
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
    case types::e1_interface:
      c.init(other.c.get<reset_all_e>());
      break;
    case types::part_of_e1_interface:
      c.init(other.c.get<ue_associated_lc_e1_conn_list_res_l>());
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
    case types::e1_interface:
      c.set(other.c.get<reset_all_e>());
      break;
    case types::part_of_e1_interface:
      c.set(other.c.get<ue_associated_lc_e1_conn_list_res_l>());
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
reset_all_e& reset_type_c::set_e1_interface()
{
  set(types::e1_interface);
  return c.get<reset_all_e>();
}
ue_associated_lc_e1_conn_list_res_l& reset_type_c::set_part_of_e1_interface()
{
  set(types::part_of_e1_interface);
  return c.get<ue_associated_lc_e1_conn_list_res_l>();
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
    case types::e1_interface:
      j.write_str("e1-Interface", "reset-all");
      break;
    case types::part_of_e1_interface:
      j.start_array("partOfE1-Interface");
      for (const auto& e1 : c.get<ue_associated_lc_e1_conn_list_res_l>()) {
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
    case types::e1_interface:
      HANDLE_CODE(c.get<reset_all_e>().pack(bref));
      break;
    case types::part_of_e1_interface:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ue_associated_lc_e1_conn_list_res_l>(), 1, 65536, true));
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
    case types::e1_interface:
      HANDLE_CODE(c.get<reset_all_e>().unpack(bref));
      break;
    case types::part_of_e1_interface:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ue_associated_lc_e1_conn_list_res_l>(), bref, 1, 65536, true));
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
  static const char* names[] = {"e1-Interface", "partOfE1-Interface", "choice-extension"};
  return convert_enum_idx(names, 3, value, "reset_type_c::types");
}

// ResetIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t reset_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 0, 4};
  return map_enum_number(names, 3, idx, "id");
}
bool reset_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 0, 4};
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
    case 57:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 4:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e reset_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 4:
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

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 4, crit_e{crit_e::reject}, [this, &bref]() { return reset_type.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reset_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &reset_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 4: {
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
void reset_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
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
  j.write_int("id", 4);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  reset_type.to_json(j);
  j.end_obj();
  j.end_array();
}

// UE-associatedLogicalE1-ConnectionItemResAck ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ue_associated_lc_e1_conn_item_res_ack_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {5};
  return map_enum_number(names, 1, idx, "id");
}
bool ue_associated_lc_e1_conn_item_res_ack_o::is_id_valid(const uint32_t& id)
{
  return 5 == id;
}
crit_e ue_associated_lc_e1_conn_item_res_ack_o::get_crit(const uint32_t& id)
{
  if (id == 5) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ue_associated_lc_e1_conn_item_res_ack_o::value_c ue_associated_lc_e1_conn_item_res_ack_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 5) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ue_associated_lc_e1_conn_item_res_ack_o::get_presence(const uint32_t& id)
{
  if (id == 5) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ue_associated_lc_e1_conn_item_res_ack_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ue_associated_lc_e1_conn_item_res_ack_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_associated_lc_e1_conn_item_res_ack_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ue_associated_lc_e1_conn_item_res_ack_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"UE-associatedLogicalE1-ConnectionItem"};
  return convert_enum_idx(names, 1, value, "ue_associated_lc_e1_conn_item_res_ack_o::value_c::types");
}
uint8_t ue_associated_lc_e1_conn_item_res_ack_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ue_associated_lc_e1_conn_item_res_ack_o::value_c::types");
}

// ResetAcknowledgeIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t reset_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 6, 1};
  return map_enum_number(names, 3, idx, "id");
}
bool reset_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 6, 1};
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
    case 57:
      return crit_e::reject;
    case 6:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e reset_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 6:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE reset_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ue_associated_lc_e1_conn_list_res_ack_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (ue_associated_lc_e1_conn_list_res_ack_present) {
    HANDLE_CODE(pack_ie(bref, 6, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_associated_lc_e1_conn_list_res_ack, 1, 65536, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &reset_ack_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 6: {
        ue_associated_lc_e1_conn_list_res_ack_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ue_associated_lc_e1_conn_list_res_ack, bref, 1, 65536, true);
        }));
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
void reset_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ue_associated_lc_e1_conn_list_res_ack_present) {
    j.start_obj();
    j.write_int("id", 6);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_associated_lc_e1_conn_list_res_ack) {
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

// ResourceStatusFailureIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t res_status_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 89, 90, 0, 1};
  return map_enum_number(names, 5, idx, "id");
}
bool res_status_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 89, 90, 0, 1};
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
    case 57:
      return crit_e::reject;
    case 89:
      return crit_e::reject;
    case 90:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 89:
      return presence_e::mandatory;
    case 90:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += gnb_cu_up_meas_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (gnb_cu_up_meas_id_present) {
    HANDLE_CODE(pack_ie(bref, 90, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &res_status_fail_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 90: {
        gnb_cu_up_meas_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
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
void res_status_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_meas_id);
  j.end_obj();
  if (gnb_cu_up_meas_id_present) {
    j.start_obj();
    j.write_int("id", 90);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_up_meas_id);
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
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusRequestIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t res_status_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 89, 90, 91, 92, 93};
  return map_enum_number(names, 6, idx, "id");
}
bool res_status_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 89, 90, 91, 92, 93};
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
    case 57:
      return crit_e::reject;
    case 89:
      return crit_e::reject;
    case 90:
      return crit_e::ignore;
    case 91:
      return crit_e::reject;
    case 92:
      return crit_e::reject;
    case 93:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 89:
      return presence_e::mandatory;
    case 90:
      return presence_e::conditional;
    case 91:
      return presence_e::mandatory;
    case 92:
      return presence_e::conditional;
    case 93:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += gnb_cu_up_meas_id_present ? 1 : 0;
  nof_ies += report_characteristics_present ? 1 : 0;
  nof_ies += report_periodicity_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (gnb_cu_up_meas_id_present) {
    HANDLE_CODE(pack_ie(bref, 90, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_cu_up_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 91, crit_e{crit_e::reject}, [this, &bref]() { return regist_request.pack(bref); }));
  if (report_characteristics_present) {
    HANDLE_CODE(
        pack_ie(bref, 92, crit_e{crit_e::reject}, [this, &bref]() { return report_characteristics.pack(bref); }));
  }
  if (report_periodicity_present) {
    HANDLE_CODE(pack_ie(bref, 93, crit_e{crit_e::ignore}, [this, &bref]() { return report_periodicity.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &res_status_request_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 90: {
        gnb_cu_up_meas_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 91: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return regist_request.unpack(bref); }));
        break;
      }
      case 92: {
        report_characteristics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return report_characteristics.unpack(bref); }));
        break;
      }
      case 93: {
        report_periodicity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return report_periodicity.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_meas_id);
  j.end_obj();
  if (gnb_cu_up_meas_id_present) {
    j.start_obj();
    j.write_int("id", 90);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_cu_up_meas_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 91);
  j.write_str("criticality", "reject");
  j.write_str("value", regist_request.to_string());
  j.end_obj();
  if (report_characteristics_present) {
    j.start_obj();
    j.write_int("id", 92);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    report_characteristics.to_json(j);
    j.end_obj();
  }
  if (report_periodicity_present) {
    j.start_obj();
    j.write_int("id", 93);
    j.write_str("criticality", "ignore");
    j.write_str("value", report_periodicity.to_string());
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusResponseIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t res_status_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 89, 90, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool res_status_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 89, 90, 1};
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
    case 57:
      return crit_e::reject;
    case 89:
      return crit_e::reject;
    case 90:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 89:
      return presence_e::mandatory;
    case 90:
      return presence_e::mandatory;
    case 1:
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

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 90, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &res_status_resp_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 90: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
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
void res_status_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 90);
  j.write_str("criticality", "ignore");
  j.write_int("value", gnb_cu_up_meas_id);
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

// ResourceStatusUpdateIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t res_status_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 89, 90, 94, 95};
  return map_enum_number(names, 5, idx, "id");
}
bool res_status_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 89, 90, 94, 95};
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
    case 57:
      return crit_e::reject;
    case 89:
      return crit_e::reject;
    case 90:
      return crit_e::ignore;
    case 94:
      return crit_e::ignore;
    case 95:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 89:
      return presence_e::mandatory;
    case 90:
      return presence_e::mandatory;
    case 94:
      return presence_e::optional;
    case 95:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += tnl_available_capacity_ind_present ? 1 : 0;
  nof_ies += hw_capacity_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 90, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (tnl_available_capacity_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 94, crit_e{crit_e::ignore}, [this, &bref]() { return tnl_available_capacity_ind.pack(bref); }));
  }
  if (hw_capacity_ind_present) {
    HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::ignore}, [this, &bref]() { return hw_capacity_ind.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &res_status_upd_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 90: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 94: {
        tnl_available_capacity_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return tnl_available_capacity_ind.unpack(bref); }));
        break;
      }
      case 95: {
        hw_capacity_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return hw_capacity_ind.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_cp_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 90);
  j.write_str("criticality", "ignore");
  j.write_int("value", gnb_cu_up_meas_id);
  j.end_obj();
  if (tnl_available_capacity_ind_present) {
    j.start_obj();
    j.write_int("id", 94);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    tnl_available_capacity_ind.to_json(j);
    j.end_obj();
  }
  if (hw_capacity_ind_present) {
    j.start_obj();
    j.write_int("id", 95);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    hw_capacity_ind.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TraceStartIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t trace_start_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 81};
  return map_enum_number(names, 3, idx, "id");
}
bool trace_start_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 81};
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
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 81:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trace_start_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 81:
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

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 81, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));

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
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 81: {
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
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 81);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  trace_activation.to_json(j);
  j.end_obj();
  j.end_array();
}

// ULDataNotificationIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t ul_data_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 67};
  return map_enum_number(names, 3, idx, "id");
}
bool ul_data_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 67};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_data_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 67:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ul_data_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 67:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ul_data_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 67, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_to_notify_list, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_data_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_data_notif_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 67: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_to_notify_list, bref, 1, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ul_data_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_cp_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_up_ue_e1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 67);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_to_notify_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}
