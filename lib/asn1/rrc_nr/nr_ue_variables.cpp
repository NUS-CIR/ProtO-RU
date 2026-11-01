// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/rrc_nr/nr_ue_variables.h"

using namespace asn1;
using namespace asn1::rrc_nr;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// CLI-TriggeredList-r16 ::= CHOICE
void cli_triggered_list_r16_c::destroy_()
{
  switch (type_) {
    case types::srs_rsrp_triggered_list_r16:
      c.destroy<srs_rsrp_triggered_list_r16_l>();
      break;
    case types::cli_rssi_triggered_list_r16:
      c.destroy<cli_rssi_triggered_list_r16_l>();
      break;
    default:
      break;
  }
}
void cli_triggered_list_r16_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::srs_rsrp_triggered_list_r16:
      c.init<srs_rsrp_triggered_list_r16_l>();
      break;
    case types::cli_rssi_triggered_list_r16:
      c.init<cli_rssi_triggered_list_r16_l>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cli_triggered_list_r16_c");
  }
}
cli_triggered_list_r16_c::cli_triggered_list_r16_c(const cli_triggered_list_r16_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::srs_rsrp_triggered_list_r16:
      c.init(other.c.get<srs_rsrp_triggered_list_r16_l>());
      break;
    case types::cli_rssi_triggered_list_r16:
      c.init(other.c.get<cli_rssi_triggered_list_r16_l>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cli_triggered_list_r16_c");
  }
}
cli_triggered_list_r16_c& cli_triggered_list_r16_c::operator=(const cli_triggered_list_r16_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::srs_rsrp_triggered_list_r16:
      c.set(other.c.get<srs_rsrp_triggered_list_r16_l>());
      break;
    case types::cli_rssi_triggered_list_r16:
      c.set(other.c.get<cli_rssi_triggered_list_r16_l>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cli_triggered_list_r16_c");
  }

  return *this;
}
srs_rsrp_triggered_list_r16_l& cli_triggered_list_r16_c::set_srs_rsrp_triggered_list_r16()
{
  set(types::srs_rsrp_triggered_list_r16);
  return c.get<srs_rsrp_triggered_list_r16_l>();
}
cli_rssi_triggered_list_r16_l& cli_triggered_list_r16_c::set_cli_rssi_triggered_list_r16()
{
  set(types::cli_rssi_triggered_list_r16);
  return c.get<cli_rssi_triggered_list_r16_l>();
}
void cli_triggered_list_r16_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srs_rsrp_triggered_list_r16:
      j.start_array("srs-RSRP-TriggeredList-r16");
      for (const auto& e1 : c.get<srs_rsrp_triggered_list_r16_l>()) {
        j.write_int(e1);
      }
      j.end_array();
      break;
    case types::cli_rssi_triggered_list_r16:
      j.start_array("cli-RSSI-TriggeredList-r16");
      for (const auto& e1 : c.get<cli_rssi_triggered_list_r16_l>()) {
        j.write_int(e1);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "cli_triggered_list_r16_c");
  }
  j.end_obj();
}
OCUDUASN_CODE cli_triggered_list_r16_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::srs_rsrp_triggered_list_r16:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<srs_rsrp_triggered_list_r16_l>(), 1, 32, integer_packer<uint8_t>(0, 63)));
      break;
    case types::cli_rssi_triggered_list_r16:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<cli_rssi_triggered_list_r16_l>(), 1, 64, integer_packer<uint8_t>(0, 63)));
      break;
    default:
      log_invalid_choice_id(type_, "cli_triggered_list_r16_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cli_triggered_list_r16_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::srs_rsrp_triggered_list_r16:
      HANDLE_CODE(
          unpack_dyn_seq_of(c.get<srs_rsrp_triggered_list_r16_l>(), bref, 1, 32, integer_packer<uint8_t>(0, 63)));
      break;
    case types::cli_rssi_triggered_list_r16:
      HANDLE_CODE(
          unpack_dyn_seq_of(c.get<cli_rssi_triggered_list_r16_l>(), bref, 1, 64, integer_packer<uint8_t>(0, 63)));
      break;
    default:
      log_invalid_choice_id(type_, "cli_triggered_list_r16_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cli_triggered_list_r16_c::types_opts::to_string() const
{
  static const char* names[] = {"srs-RSRP-TriggeredList-r16", "cli-RSSI-TriggeredList-r16"};
  return convert_enum_idx(names, 2, value, "cli_triggered_list_r16_c::types");
}

void cells_triggered_list_item_c_::destroy_() {}
void cells_triggered_list_item_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
cells_triggered_list_item_c_::cells_triggered_list_item_c_(const cells_triggered_list_item_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::pci:
      c.init(other.c.get<uint16_t>());
      break;
    case types::pci_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::pci_utra_fdd_r16:
      c.init(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cells_triggered_list_item_c_");
  }
}
cells_triggered_list_item_c_& cells_triggered_list_item_c_::operator=(const cells_triggered_list_item_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::pci:
      c.set(other.c.get<uint16_t>());
      break;
    case types::pci_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::pci_utra_fdd_r16:
      c.set(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cells_triggered_list_item_c_");
  }

  return *this;
}
uint16_t& cells_triggered_list_item_c_::set_pci()
{
  set(types::pci);
  return c.get<uint16_t>();
}
uint16_t& cells_triggered_list_item_c_::set_pci_eutra()
{
  set(types::pci_eutra);
  return c.get<uint16_t>();
}
uint16_t& cells_triggered_list_item_c_::set_pci_utra_fdd_r16()
{
  set(types::pci_utra_fdd_r16);
  return c.get<uint16_t>();
}
void cells_triggered_list_item_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::pci:
      j.write_int("physCellId", c.get<uint16_t>());
      break;
    case types::pci_eutra:
      j.write_int("physCellIdEUTRA", c.get<uint16_t>());
      break;
    case types::pci_utra_fdd_r16:
      j.write_int("physCellIdUTRA-FDD-r16", c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "cells_triggered_list_item_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE cells_triggered_list_item_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::pci:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1007u));
      break;
    case types::pci_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)503u));
      break;
    case types::pci_utra_fdd_r16:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)511u));
      break;
    default:
      log_invalid_choice_id(type_, "cells_triggered_list_item_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cells_triggered_list_item_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::pci:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1007u));
      break;
    case types::pci_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)503u));
      break;
    case types::pci_utra_fdd_r16:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)511u));
      break;
    default:
      log_invalid_choice_id(type_, "cells_triggered_list_item_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cells_triggered_list_item_c_::types_opts::to_string() const
{
  static const char* names[] = {"physCellId", "physCellIdEUTRA", "physCellIdUTRA-FDD-r16"};
  return convert_enum_idx(names, 3, value, "cells_triggered_list_item_c_::types");
}

// SNPN-Identity-r18 ::= SEQUENCE
OCUDUASN_CODE sn_pn_id_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(plmn_id_r18.pack(bref));
  HANDLE_CODE(nid_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_pn_id_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(plmn_id_r18.unpack(bref));
  HANDLE_CODE(nid_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void sn_pn_id_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("plmn-Identity-r18");
  plmn_id_r18.to_json(j);
  j.write_fieldname("nid-r18");
  nid_r18.to_json(j);
  j.end_obj();
}

// VarAppLayerIdle-r18 ::= SEQUENCE
OCUDUASN_CODE var_app_layer_idle_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(app_layer_meas_prio_r18_present, 1));

  HANDLE_CODE(pack_integer(bref, meas_cfg_app_layer_id_r18, (uint8_t)0u, (uint8_t)15u));
  HANDLE_CODE(service_type_r18.pack(bref));
  HANDLE_CODE(app_layer_idle_inactive_cfg_r18.pack(bref));
  if (app_layer_meas_prio_r18_present) {
    HANDLE_CODE(pack_integer(bref, app_layer_meas_prio_r18, (uint8_t)1u, (uint8_t)16u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_app_layer_idle_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(app_layer_meas_prio_r18_present, 1));

  HANDLE_CODE(unpack_integer(meas_cfg_app_layer_id_r18, bref, (uint8_t)0u, (uint8_t)15u));
  HANDLE_CODE(service_type_r18.unpack(bref));
  HANDLE_CODE(app_layer_idle_inactive_cfg_r18.unpack(bref));
  if (app_layer_meas_prio_r18_present) {
    HANDLE_CODE(unpack_integer(app_layer_meas_prio_r18, bref, (uint8_t)1u, (uint8_t)16u));
  }

  return OCUDUASN_SUCCESS;
}
void var_app_layer_idle_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("measConfigAppLayerId-r18", meas_cfg_app_layer_id_r18);
  j.write_str("serviceType-r18", service_type_r18.to_string());
  j.write_fieldname("appLayerIdleInactiveConfig-r18");
  app_layer_idle_inactive_cfg_r18.to_json(j);
  if (app_layer_meas_prio_r18_present) {
    j.write_int("appLayerMeasPriority-r18", app_layer_meas_prio_r18);
  }
  j.end_obj();
}

const char* var_app_layer_idle_r18_s::service_type_r18_opts::to_string() const
{
  static const char* names[] = {"streaming", "mtsi", "vr", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "var_app_layer_idle_r18_s::service_type_r18_e_");
}

// VarAppLayerIdleConfig-r18 ::= SEQUENCE
OCUDUASN_CODE var_app_layer_idle_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_dyn_seq_of(bref, app_layer_idle_cfg_list_r18, 1, 16));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_app_layer_idle_cfg_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_dyn_seq_of(app_layer_idle_cfg_list_r18, bref, 1, 16));

  return OCUDUASN_SUCCESS;
}
void var_app_layer_idle_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("appLayerIdleConfigList-r18");
  for (const auto& e1 : app_layer_idle_cfg_list_r18) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// VarAppLayerPLMN-List-r18 ::= SEQUENCE
OCUDUASN_CODE var_app_layer_plmn_list_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, meas_cfg_app_layer_id_r18, (uint8_t)0u, (uint8_t)15u));
  HANDLE_CODE(pack_dyn_seq_of(bref, plmn_id_list_r18, 1, 16));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_app_layer_plmn_list_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(meas_cfg_app_layer_id_r18, bref, (uint8_t)0u, (uint8_t)15u));
  HANDLE_CODE(unpack_dyn_seq_of(plmn_id_list_r18, bref, 1, 16));

  return OCUDUASN_SUCCESS;
}
void var_app_layer_plmn_list_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("measConfigAppLayerId-r18", meas_cfg_app_layer_id_r18);
  j.start_array("plmn-IdentityList-r18");
  for (const auto& e1 : plmn_id_list_r18) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// VarAppLayerPLMN-ListConfig-r18 ::= SEQUENCE
OCUDUASN_CODE var_app_layer_plmn_list_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_dyn_seq_of(bref, plmn_cfg_list_r18, 1, 16));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_app_layer_plmn_list_cfg_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_dyn_seq_of(plmn_cfg_list_r18, bref, 1, 16));

  return OCUDUASN_SUCCESS;
}
void var_app_layer_plmn_list_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("plmnConfigList-r18");
  for (const auto& e1 : plmn_cfg_list_r18) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// VarConditionalReconfig ::= SEQUENCE
OCUDUASN_CODE var_conditional_recfg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(
      pack_bits(bref, {cond_recfg_list.size() > 0, scpac_ref_cfg_r18.size() > 0, sk_counter_cfg_r18.size() > 0}));

  if (cond_recfg_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cond_recfg_list, 1, 8));
  }
  if (scpac_ref_cfg_r18.size() > 0) {
    HANDLE_CODE(scpac_ref_cfg_r18.pack(bref));
  }
  if (sk_counter_cfg_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sk_counter_cfg_r18, 1, 9));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_conditional_recfg_s::unpack(cbit_ref& bref)
{
  bool cond_recfg_list_present;
  bool scpac_ref_cfg_r18_present;
  bool sk_counter_cfg_r18_present;
  HANDLE_CODE(unpack_bits(bref, {&cond_recfg_list_present, &scpac_ref_cfg_r18_present, &sk_counter_cfg_r18_present}));

  if (cond_recfg_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cond_recfg_list, bref, 1, 8));
  }
  if (scpac_ref_cfg_r18_present) {
    HANDLE_CODE(scpac_ref_cfg_r18.unpack(bref));
  }
  if (sk_counter_cfg_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sk_counter_cfg_r18, bref, 1, 9));
  }

  return OCUDUASN_SUCCESS;
}
void var_conditional_recfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (cond_recfg_list.size() > 0) {
    j.start_array("condReconfigList");
    for (const auto& e1 : cond_recfg_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (scpac_ref_cfg_r18.size() > 0) {
    j.write_str("scpac-ReferenceConfiguration-r18", scpac_ref_cfg_r18.to_string());
  }
  if (sk_counter_cfg_r18.size() > 0) {
    j.start_array("sk-CounterConfiguration-r18");
    for (const auto& e1 : sk_counter_cfg_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// VarConnEstFailReport-r16 ::= SEQUENCE
OCUDUASN_CODE var_conn_est_fail_report_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(conn_est_fail_report_r16.pack(bref));
  HANDLE_CODE(network_id_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_conn_est_fail_report_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(conn_est_fail_report_r16.unpack(bref));
  HANDLE_CODE(network_id_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void var_conn_est_fail_report_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("connEstFailReport-r16");
  conn_est_fail_report_r16.to_json(j);
  j.write_fieldname("networkIdentity-r18");
  network_id_r18.to_json(j);
  j.end_obj();
}

void var_conn_est_fail_report_r16_s::network_id_r18_c_::destroy_()
{
  switch (type_) {
    case types::plmn_id_r18:
      c.destroy<plmn_id_s>();
      break;
    case types::snpn_id_r18:
      c.destroy<sn_pn_id_r18_s>();
      break;
    default:
      break;
  }
}
void var_conn_est_fail_report_r16_s::network_id_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::plmn_id_r18:
      c.init<plmn_id_s>();
      break;
    case types::snpn_id_r18:
      c.init<sn_pn_id_r18_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_conn_est_fail_report_r16_s::network_id_r18_c_");
  }
}
var_conn_est_fail_report_r16_s::network_id_r18_c_::network_id_r18_c_(
    const var_conn_est_fail_report_r16_s::network_id_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::plmn_id_r18:
      c.init(other.c.get<plmn_id_s>());
      break;
    case types::snpn_id_r18:
      c.init(other.c.get<sn_pn_id_r18_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_conn_est_fail_report_r16_s::network_id_r18_c_");
  }
}
var_conn_est_fail_report_r16_s::network_id_r18_c_& var_conn_est_fail_report_r16_s::network_id_r18_c_::operator=(
    const var_conn_est_fail_report_r16_s::network_id_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::plmn_id_r18:
      c.set(other.c.get<plmn_id_s>());
      break;
    case types::snpn_id_r18:
      c.set(other.c.get<sn_pn_id_r18_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_conn_est_fail_report_r16_s::network_id_r18_c_");
  }

  return *this;
}
plmn_id_s& var_conn_est_fail_report_r16_s::network_id_r18_c_::set_plmn_id_r18()
{
  set(types::plmn_id_r18);
  return c.get<plmn_id_s>();
}
sn_pn_id_r18_s& var_conn_est_fail_report_r16_s::network_id_r18_c_::set_snpn_id_r18()
{
  set(types::snpn_id_r18);
  return c.get<sn_pn_id_r18_s>();
}
void var_conn_est_fail_report_r16_s::network_id_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::plmn_id_r18:
      j.write_fieldname("plmn-Identity-r18");
      c.get<plmn_id_s>().to_json(j);
      break;
    case types::snpn_id_r18:
      j.write_fieldname("snpn-Identity-r18");
      c.get<sn_pn_id_r18_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "var_conn_est_fail_report_r16_s::network_id_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE var_conn_est_fail_report_r16_s::network_id_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::plmn_id_r18:
      HANDLE_CODE(c.get<plmn_id_s>().pack(bref));
      break;
    case types::snpn_id_r18:
      HANDLE_CODE(c.get<sn_pn_id_r18_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "var_conn_est_fail_report_r16_s::network_id_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_conn_est_fail_report_r16_s::network_id_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::plmn_id_r18:
      HANDLE_CODE(c.get<plmn_id_s>().unpack(bref));
      break;
    case types::snpn_id_r18:
      HANDLE_CODE(c.get<sn_pn_id_r18_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "var_conn_est_fail_report_r16_s::network_id_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* var_conn_est_fail_report_r16_s::network_id_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"plmn-Identity-r18", "snpn-Identity-r18"};
  return convert_enum_idx(names, 2, value, "var_conn_est_fail_report_r16_s::network_id_r18_c_::types");
}

// VarConnEstFailReportList-r17 ::= SEQUENCE
OCUDUASN_CODE var_conn_est_fail_report_list_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_dyn_seq_of(bref, conn_est_fail_report_list_r17, 1, 4));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_conn_est_fail_report_list_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_dyn_seq_of(conn_est_fail_report_list_r17, bref, 1, 4));

  return OCUDUASN_SUCCESS;
}
void var_conn_est_fail_report_list_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("connEstFailReportList-r17");
  for (const auto& e1 : conn_est_fail_report_list_r17) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// VarEnhMeasIdleConfig-r18 ::= SEQUENCE
OCUDUASN_CODE var_enh_meas_idle_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(meas_idle_validity_dur_r18_present, 1));

  if (meas_idle_validity_dur_r18_present) {
    HANDLE_CODE(meas_idle_validity_dur_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_enh_meas_idle_cfg_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(meas_idle_validity_dur_r18_present, 1));

  if (meas_idle_validity_dur_r18_present) {
    HANDLE_CODE(meas_idle_validity_dur_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void var_enh_meas_idle_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (meas_idle_validity_dur_r18_present) {
    j.write_str("measIdleValidityDuration-r18", meas_idle_validity_dur_r18.to_string());
  }
  j.end_obj();
}

// VarLTM-ServingCellNoResetID-r18 ::= SEQUENCE
OCUDUASN_CODE var_ltm_serving_cell_no_reset_id_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ltm_serving_cell_no_reset_id_r18_present, 1));

  if (ltm_serving_cell_no_reset_id_r18_present) {
    HANDLE_CODE(pack_integer(bref, ltm_serving_cell_no_reset_id_r18, (uint8_t)1u, (uint8_t)9u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_ltm_serving_cell_no_reset_id_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ltm_serving_cell_no_reset_id_r18_present, 1));

  if (ltm_serving_cell_no_reset_id_r18_present) {
    HANDLE_CODE(unpack_integer(ltm_serving_cell_no_reset_id_r18, bref, (uint8_t)1u, (uint8_t)9u));
  }

  return OCUDUASN_SUCCESS;
}
void var_ltm_serving_cell_no_reset_id_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ltm_serving_cell_no_reset_id_r18_present) {
    j.write_int("ltm-ServingCellNoResetID-r18", ltm_serving_cell_no_reset_id_r18);
  }
  j.end_obj();
}

// VarLTM-ServingCellUE-MeasuredTA-ID-r18 ::= SEQUENCE
OCUDUASN_CODE var_ltm_serving_cell_ue_measured_ta_id_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ltm_serving_cell_ue_measured_ta_id_r18_present, 1));

  if (ltm_serving_cell_ue_measured_ta_id_r18_present) {
    HANDLE_CODE(pack_integer(bref, ltm_serving_cell_ue_measured_ta_id_r18, (uint8_t)1u, (uint8_t)9u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_ltm_serving_cell_ue_measured_ta_id_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ltm_serving_cell_ue_measured_ta_id_r18_present, 1));

  if (ltm_serving_cell_ue_measured_ta_id_r18_present) {
    HANDLE_CODE(unpack_integer(ltm_serving_cell_ue_measured_ta_id_r18, bref, (uint8_t)1u, (uint8_t)9u));
  }

  return OCUDUASN_SUCCESS;
}
void var_ltm_serving_cell_ue_measured_ta_id_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ltm_serving_cell_ue_measured_ta_id_r18_present) {
    j.write_int("ltm-ServingCellUE-MeasuredTA-ID-r18", ltm_serving_cell_ue_measured_ta_id_r18);
  }
  j.end_obj();
}

// VarLogMeasConfig-r16 ::= SEQUENCE
OCUDUASN_CODE var_log_meas_cfg_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {area_cfg_r16_present,
                         bt_name_list_r16.size() > 0,
                         wlan_name_list_r16.size() > 0,
                         sensor_name_list_r16_present,
                         early_meas_ind_r17_present,
                         area_cfg_r17_present,
                         area_cfg_v1800_present}));

  if (area_cfg_r16_present) {
    HANDLE_CODE(area_cfg_r16.pack(bref));
  }
  if (bt_name_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, bt_name_list_r16, 1, 4));
  }
  if (wlan_name_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, wlan_name_list_r16, 1, 4));
  }
  if (sensor_name_list_r16_present) {
    HANDLE_CODE(sensor_name_list_r16.pack(bref));
  }
  HANDLE_CODE(logging_dur_r16.pack(bref));
  HANDLE_CODE(report_type.pack(bref));
  if (area_cfg_r17_present) {
    HANDLE_CODE(area_cfg_r17.pack(bref));
  }
  if (area_cfg_v1800_present) {
    HANDLE_CODE(area_cfg_v1800.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_log_meas_cfg_r16_s::unpack(cbit_ref& bref)
{
  bool bt_name_list_r16_present;
  bool wlan_name_list_r16_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&area_cfg_r16_present,
                           &bt_name_list_r16_present,
                           &wlan_name_list_r16_present,
                           &sensor_name_list_r16_present,
                           &early_meas_ind_r17_present,
                           &area_cfg_r17_present,
                           &area_cfg_v1800_present}));

  if (area_cfg_r16_present) {
    HANDLE_CODE(area_cfg_r16.unpack(bref));
  }
  if (bt_name_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(bt_name_list_r16, bref, 1, 4));
  }
  if (wlan_name_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(wlan_name_list_r16, bref, 1, 4));
  }
  if (sensor_name_list_r16_present) {
    HANDLE_CODE(sensor_name_list_r16.unpack(bref));
  }
  HANDLE_CODE(logging_dur_r16.unpack(bref));
  HANDLE_CODE(report_type.unpack(bref));
  if (area_cfg_r17_present) {
    HANDLE_CODE(area_cfg_r17.unpack(bref));
  }
  if (area_cfg_v1800_present) {
    HANDLE_CODE(area_cfg_v1800.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void var_log_meas_cfg_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (area_cfg_r16_present) {
    j.write_fieldname("areaConfiguration-r16");
    area_cfg_r16.to_json(j);
  }
  if (bt_name_list_r16.size() > 0) {
    j.start_array("bt-NameList-r16");
    for (const auto& e1 : bt_name_list_r16) {
      j.write_str(e1.to_string());
    }
    j.end_array();
  }
  if (wlan_name_list_r16.size() > 0) {
    j.start_array("wlan-NameList-r16");
    for (const auto& e1 : wlan_name_list_r16) {
      j.write_str(e1.to_string());
    }
    j.end_array();
  }
  if (sensor_name_list_r16_present) {
    j.write_fieldname("sensor-NameList-r16");
    sensor_name_list_r16.to_json(j);
  }
  j.write_str("loggingDuration-r16", logging_dur_r16.to_string());
  j.write_fieldname("reportType");
  report_type.to_json(j);
  if (early_meas_ind_r17_present) {
    j.write_str("earlyMeasIndication-r17", "true");
  }
  if (area_cfg_r17_present) {
    j.write_fieldname("areaConfiguration-r17");
    area_cfg_r17.to_json(j);
  }
  if (area_cfg_v1800_present) {
    j.write_fieldname("areaConfiguration-v1800");
    area_cfg_v1800.to_json(j);
  }
  j.end_obj();
}

void var_log_meas_cfg_r16_s::report_type_c_::destroy_()
{
  switch (type_) {
    case types::periodical:
      c.destroy<logged_periodical_report_cfg_r16_s>();
      break;
    case types::event_triggered:
      c.destroy<logged_event_trigger_cfg_r16_s>();
      break;
    default:
      break;
  }
}
void var_log_meas_cfg_r16_s::report_type_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::periodical:
      c.init<logged_periodical_report_cfg_r16_s>();
      break;
    case types::event_triggered:
      c.init<logged_event_trigger_cfg_r16_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_cfg_r16_s::report_type_c_");
  }
}
var_log_meas_cfg_r16_s::report_type_c_::report_type_c_(const var_log_meas_cfg_r16_s::report_type_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::periodical:
      c.init(other.c.get<logged_periodical_report_cfg_r16_s>());
      break;
    case types::event_triggered:
      c.init(other.c.get<logged_event_trigger_cfg_r16_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_cfg_r16_s::report_type_c_");
  }
}
var_log_meas_cfg_r16_s::report_type_c_&
var_log_meas_cfg_r16_s::report_type_c_::operator=(const var_log_meas_cfg_r16_s::report_type_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::periodical:
      c.set(other.c.get<logged_periodical_report_cfg_r16_s>());
      break;
    case types::event_triggered:
      c.set(other.c.get<logged_event_trigger_cfg_r16_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_cfg_r16_s::report_type_c_");
  }

  return *this;
}
logged_periodical_report_cfg_r16_s& var_log_meas_cfg_r16_s::report_type_c_::set_periodical()
{
  set(types::periodical);
  return c.get<logged_periodical_report_cfg_r16_s>();
}
logged_event_trigger_cfg_r16_s& var_log_meas_cfg_r16_s::report_type_c_::set_event_triggered()
{
  set(types::event_triggered);
  return c.get<logged_event_trigger_cfg_r16_s>();
}
void var_log_meas_cfg_r16_s::report_type_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::periodical:
      j.write_fieldname("periodical");
      c.get<logged_periodical_report_cfg_r16_s>().to_json(j);
      break;
    case types::event_triggered:
      j.write_fieldname("eventTriggered");
      c.get<logged_event_trigger_cfg_r16_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_cfg_r16_s::report_type_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE var_log_meas_cfg_r16_s::report_type_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::periodical:
      HANDLE_CODE(c.get<logged_periodical_report_cfg_r16_s>().pack(bref));
      break;
    case types::event_triggered:
      HANDLE_CODE(c.get<logged_event_trigger_cfg_r16_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_cfg_r16_s::report_type_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_log_meas_cfg_r16_s::report_type_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::periodical:
      HANDLE_CODE(c.get<logged_periodical_report_cfg_r16_s>().unpack(bref));
      break;
    case types::event_triggered:
      HANDLE_CODE(c.get<logged_event_trigger_cfg_r16_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_cfg_r16_s::report_type_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* var_log_meas_cfg_r16_s::report_type_c_::types_opts::to_string() const
{
  static const char* names[] = {"periodical", "eventTriggered"};
  return convert_enum_idx(names, 2, value, "var_log_meas_cfg_r16_s::report_type_c_::types");
}

// VarLogMeasReport-r16 ::= SEQUENCE
OCUDUASN_CODE var_log_meas_report_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(absolute_time_info_r16.pack(bref));
  HANDLE_CODE(trace_ref_r16.pack(bref));
  HANDLE_CODE(trace_recording_session_ref_r16.pack(bref));
  HANDLE_CODE(tce_id_r16.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, log_meas_info_list_r16, 1, 520));
  HANDLE_CODE(id_list_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_log_meas_report_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(absolute_time_info_r16.unpack(bref));
  HANDLE_CODE(trace_ref_r16.unpack(bref));
  HANDLE_CODE(trace_recording_session_ref_r16.unpack(bref));
  HANDLE_CODE(tce_id_r16.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(log_meas_info_list_r16, bref, 1, 520));
  HANDLE_CODE(id_list_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void var_log_meas_report_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("absoluteTimeInfo-r16");
  absolute_time_info_r16.to_json(j);
  j.write_fieldname("traceReference-r16");
  trace_ref_r16.to_json(j);
  j.write_str("traceRecordingSessionRef-r16", trace_recording_session_ref_r16.to_string());
  j.write_str("tce-Id-r16", tce_id_r16.to_string());
  j.start_array("logMeasInfoList-r16");
  for (const auto& e1 : log_meas_info_list_r16) {
    e1.to_json(j);
  }
  j.end_array();
  j.write_str("sigLoggedMeasType-r17", "true");
  j.write_fieldname("identityList-r18");
  id_list_r18.to_json(j);
  j.end_obj();
}

void var_log_meas_report_r16_s::id_list_r18_c_::destroy_()
{
  switch (type_) {
    case types::plmn_id_list_r18:
      c.destroy<plmn_id_list2_r16_l>();
      break;
    case types::snpn_cfg_id_list_r18:
      c.destroy<sn_pn_cfg_id_list_r18_l>();
      break;
    default:
      break;
  }
}
void var_log_meas_report_r16_s::id_list_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init<plmn_id_list2_r16_l>();
      break;
    case types::snpn_cfg_id_list_r18:
      c.init<sn_pn_cfg_id_list_r18_l>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_report_r16_s::id_list_r18_c_");
  }
}
var_log_meas_report_r16_s::id_list_r18_c_::id_list_r18_c_(const var_log_meas_report_r16_s::id_list_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_cfg_id_list_r18:
      c.init(other.c.get<sn_pn_cfg_id_list_r18_l>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_report_r16_s::id_list_r18_c_");
  }
}
var_log_meas_report_r16_s::id_list_r18_c_&
var_log_meas_report_r16_s::id_list_r18_c_::operator=(const var_log_meas_report_r16_s::id_list_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::plmn_id_list_r18:
      c.set(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_cfg_id_list_r18:
      c.set(other.c.get<sn_pn_cfg_id_list_r18_l>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_report_r16_s::id_list_r18_c_");
  }

  return *this;
}
plmn_id_list2_r16_l& var_log_meas_report_r16_s::id_list_r18_c_::set_plmn_id_list_r18()
{
  set(types::plmn_id_list_r18);
  return c.get<plmn_id_list2_r16_l>();
}
sn_pn_cfg_id_list_r18_l& var_log_meas_report_r16_s::id_list_r18_c_::set_snpn_cfg_id_list_r18()
{
  set(types::snpn_cfg_id_list_r18);
  return c.get<sn_pn_cfg_id_list_r18_l>();
}
void var_log_meas_report_r16_s::id_list_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::plmn_id_list_r18:
      j.start_array("plmn-IdentityList-r18");
      for (const auto& e1 : c.get<plmn_id_list2_r16_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::snpn_cfg_id_list_r18:
      j.start_array("snpn-ConfigID-List-r18");
      for (const auto& e1 : c.get<sn_pn_cfg_id_list_r18_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_report_r16_s::id_list_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE var_log_meas_report_r16_s::id_list_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<plmn_id_list2_r16_l>(), 1, 16));
      break;
    case types::snpn_cfg_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<sn_pn_cfg_id_list_r18_l>(), 1, 16));
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_report_r16_s::id_list_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_log_meas_report_r16_s::id_list_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<plmn_id_list2_r16_l>(), bref, 1, 16));
      break;
    case types::snpn_cfg_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<sn_pn_cfg_id_list_r18_l>(), bref, 1, 16));
      break;
    default:
      log_invalid_choice_id(type_, "var_log_meas_report_r16_s::id_list_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* var_log_meas_report_r16_s::id_list_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"plmn-IdentityList-r18", "snpn-ConfigID-List-r18"};
  return convert_enum_idx(names, 2, value, "var_log_meas_report_r16_s::id_list_r18_c_::types");
}

// VarMeasConfig ::= SEQUENCE
OCUDUASN_CODE var_meas_cfg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {meas_id_list.size() > 0,
                         meas_obj_list.size() > 0,
                         report_cfg_list.size() > 0,
                         quant_cfg_present,
                         s_measure_cfg_present}));

  if (meas_id_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, meas_id_list, 1, 64));
  }
  if (meas_obj_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, meas_obj_list, 1, 64));
  }
  if (report_cfg_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, report_cfg_list, 1, 64));
  }
  if (quant_cfg_present) {
    HANDLE_CODE(quant_cfg.pack(bref));
  }
  if (s_measure_cfg_present) {
    HANDLE_CODE(s_measure_cfg.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_meas_cfg_s::unpack(cbit_ref& bref)
{
  bool meas_id_list_present;
  bool meas_obj_list_present;
  bool report_cfg_list_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&meas_id_list_present,
                           &meas_obj_list_present,
                           &report_cfg_list_present,
                           &quant_cfg_present,
                           &s_measure_cfg_present}));

  if (meas_id_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(meas_id_list, bref, 1, 64));
  }
  if (meas_obj_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(meas_obj_list, bref, 1, 64));
  }
  if (report_cfg_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(report_cfg_list, bref, 1, 64));
  }
  if (quant_cfg_present) {
    HANDLE_CODE(quant_cfg.unpack(bref));
  }
  if (s_measure_cfg_present) {
    HANDLE_CODE(s_measure_cfg.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void var_meas_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (meas_id_list.size() > 0) {
    j.start_array("measIdList");
    for (const auto& e1 : meas_id_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (meas_obj_list.size() > 0) {
    j.start_array("measObjectList");
    for (const auto& e1 : meas_obj_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (report_cfg_list.size() > 0) {
    j.start_array("reportConfigList");
    for (const auto& e1 : report_cfg_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (quant_cfg_present) {
    j.write_fieldname("quantityConfig");
    quant_cfg.to_json(j);
  }
  if (s_measure_cfg_present) {
    j.write_fieldname("s-MeasureConfig");
    s_measure_cfg.to_json(j);
  }
  j.end_obj();
}

void var_meas_cfg_s::s_measure_cfg_c_::destroy_() {}
void var_meas_cfg_s::s_measure_cfg_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
var_meas_cfg_s::s_measure_cfg_c_::s_measure_cfg_c_(const var_meas_cfg_s::s_measure_cfg_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ssb_rsrp:
      c.init(other.c.get<uint8_t>());
      break;
    case types::csi_rsrp:
      c.init(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_meas_cfg_s::s_measure_cfg_c_");
  }
}
var_meas_cfg_s::s_measure_cfg_c_&
var_meas_cfg_s::s_measure_cfg_c_::operator=(const var_meas_cfg_s::s_measure_cfg_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ssb_rsrp:
      c.set(other.c.get<uint8_t>());
      break;
    case types::csi_rsrp:
      c.set(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_meas_cfg_s::s_measure_cfg_c_");
  }

  return *this;
}
uint8_t& var_meas_cfg_s::s_measure_cfg_c_::set_ssb_rsrp()
{
  set(types::ssb_rsrp);
  return c.get<uint8_t>();
}
uint8_t& var_meas_cfg_s::s_measure_cfg_c_::set_csi_rsrp()
{
  set(types::csi_rsrp);
  return c.get<uint8_t>();
}
void var_meas_cfg_s::s_measure_cfg_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ssb_rsrp:
      j.write_int("ssb-RSRP", c.get<uint8_t>());
      break;
    case types::csi_rsrp:
      j.write_int("csi-RSRP", c.get<uint8_t>());
      break;
    default:
      log_invalid_choice_id(type_, "var_meas_cfg_s::s_measure_cfg_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE var_meas_cfg_s::s_measure_cfg_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ssb_rsrp:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)127u));
      break;
    case types::csi_rsrp:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)127u));
      break;
    default:
      log_invalid_choice_id(type_, "var_meas_cfg_s::s_measure_cfg_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_meas_cfg_s::s_measure_cfg_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ssb_rsrp:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)127u));
      break;
    case types::csi_rsrp:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)127u));
      break;
    default:
      log_invalid_choice_id(type_, "var_meas_cfg_s::s_measure_cfg_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* var_meas_cfg_s::s_measure_cfg_c_::types_opts::to_string() const
{
  static const char* names[] = {"ssb-RSRP", "csi-RSRP"};
  return convert_enum_idx(names, 2, value, "var_meas_cfg_s::s_measure_cfg_c_::types");
}

// VarMeasConfigSL-r16 ::= SEQUENCE
OCUDUASN_CODE var_meas_cfg_sl_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {sl_meas_id_list_r16.size() > 0,
                         sl_meas_obj_list_r16.size() > 0,
                         sl_report_cfg_list_r16.size() > 0,
                         sl_quant_cfg_r16_present}));

  if (sl_meas_id_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_meas_id_list_r16, 1, 64));
  }
  if (sl_meas_obj_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_meas_obj_list_r16, 1, 64));
  }
  if (sl_report_cfg_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_report_cfg_list_r16, 1, 64));
  }
  if (sl_quant_cfg_r16_present) {
    HANDLE_CODE(sl_quant_cfg_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_meas_cfg_sl_r16_s::unpack(cbit_ref& bref)
{
  bool sl_meas_id_list_r16_present;
  bool sl_meas_obj_list_r16_present;
  bool sl_report_cfg_list_r16_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&sl_meas_id_list_r16_present,
                           &sl_meas_obj_list_r16_present,
                           &sl_report_cfg_list_r16_present,
                           &sl_quant_cfg_r16_present}));

  if (sl_meas_id_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_meas_id_list_r16, bref, 1, 64));
  }
  if (sl_meas_obj_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_meas_obj_list_r16, bref, 1, 64));
  }
  if (sl_report_cfg_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_report_cfg_list_r16, bref, 1, 64));
  }
  if (sl_quant_cfg_r16_present) {
    HANDLE_CODE(sl_quant_cfg_r16.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void var_meas_cfg_sl_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_meas_id_list_r16.size() > 0) {
    j.start_array("sl-MeasIdList-r16");
    for (const auto& e1 : sl_meas_id_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_meas_obj_list_r16.size() > 0) {
    j.start_array("sl-MeasObjectList-r16");
    for (const auto& e1 : sl_meas_obj_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_report_cfg_list_r16.size() > 0) {
    j.start_array("sl-ReportConfigList-r16");
    for (const auto& e1 : sl_report_cfg_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_quant_cfg_r16_present) {
    j.write_fieldname("sl-QuantityConfig-r16");
    sl_quant_cfg_r16.to_json(j);
  }
  j.end_obj();
}

// VarMeasIdleConfig-r16 ::= SEQUENCE
OCUDUASN_CODE var_meas_idle_cfg_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {meas_idle_carrier_list_nr_r16.size() > 0,
                         meas_idle_carrier_list_eutra_r16.size() > 0,
                         validity_area_list_r16.size() > 0}));

  if (meas_idle_carrier_list_nr_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, meas_idle_carrier_list_nr_r16, 1, 8));
  }
  if (meas_idle_carrier_list_eutra_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, meas_idle_carrier_list_eutra_r16, 1, 8));
  }
  HANDLE_CODE(meas_idle_dur_r16.pack(bref));
  if (validity_area_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, validity_area_list_r16, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_meas_idle_cfg_r16_s::unpack(cbit_ref& bref)
{
  bool meas_idle_carrier_list_nr_r16_present;
  bool meas_idle_carrier_list_eutra_r16_present;
  bool validity_area_list_r16_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&meas_idle_carrier_list_nr_r16_present,
                           &meas_idle_carrier_list_eutra_r16_present,
                           &validity_area_list_r16_present}));

  if (meas_idle_carrier_list_nr_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(meas_idle_carrier_list_nr_r16, bref, 1, 8));
  }
  if (meas_idle_carrier_list_eutra_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(meas_idle_carrier_list_eutra_r16, bref, 1, 8));
  }
  HANDLE_CODE(meas_idle_dur_r16.unpack(bref));
  if (validity_area_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(validity_area_list_r16, bref, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
void var_meas_idle_cfg_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (meas_idle_carrier_list_nr_r16.size() > 0) {
    j.start_array("measIdleCarrierListNR-r16");
    for (const auto& e1 : meas_idle_carrier_list_nr_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (meas_idle_carrier_list_eutra_r16.size() > 0) {
    j.start_array("measIdleCarrierListEUTRA-r16");
    for (const auto& e1 : meas_idle_carrier_list_eutra_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.write_str("measIdleDuration-r16", meas_idle_dur_r16.to_string());
  if (validity_area_list_r16.size() > 0) {
    j.start_array("validityAreaList-r16");
    for (const auto& e1 : validity_area_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

const char* var_meas_idle_cfg_r16_s::meas_idle_dur_r16_opts::to_string() const
{
  static const char* names[] = {"sec10", "sec30", "sec60", "sec120", "sec180", "sec240", "sec300", "spare"};
  return convert_enum_idx(names, 8, value, "var_meas_idle_cfg_r16_s::meas_idle_dur_r16_e_");
}
uint16_t var_meas_idle_cfg_r16_s::meas_idle_dur_r16_opts::to_number() const
{
  static const uint16_t numbers[] = {10, 30, 60, 120, 180, 240, 300};
  return map_enum_number(numbers, 7, value, "var_meas_idle_cfg_r16_s::meas_idle_dur_r16_e_");
}

// VarMeasIdleReport-r16 ::= SEQUENCE
OCUDUASN_CODE var_meas_idle_report_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {meas_report_idle_nr_r16_present, meas_report_idle_eutra_r16_present}));

  if (meas_report_idle_nr_r16_present) {
    HANDLE_CODE(meas_report_idle_nr_r16.pack(bref));
  }
  if (meas_report_idle_eutra_r16_present) {
    HANDLE_CODE(meas_report_idle_eutra_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_meas_idle_report_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&meas_report_idle_nr_r16_present, &meas_report_idle_eutra_r16_present}));

  if (meas_report_idle_nr_r16_present) {
    HANDLE_CODE(meas_report_idle_nr_r16.unpack(bref));
  }
  if (meas_report_idle_eutra_r16_present) {
    HANDLE_CODE(meas_report_idle_eutra_r16.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void var_meas_idle_report_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (meas_report_idle_nr_r16_present) {
    j.write_fieldname("measReportIdleNR-r16");
    meas_report_idle_nr_r16.to_json(j);
  }
  if (meas_report_idle_eutra_r16_present) {
    j.write_fieldname("measReportIdleEUTRA-r16");
    meas_report_idle_eutra_r16.to_json(j);
  }
  j.end_obj();
}

// VarMeasReport ::= SEQUENCE
OCUDUASN_CODE var_meas_report_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {cells_triggered_list.size() > 0,
                         cli_triggered_list_r16_present,
                         tx_pool_meas_to_add_mod_list_nr_r16.size() > 0,
                         relays_triggered_list_r17.size() > 0,
                         cells_met_leaving_cond_r18.size() > 0,
                         reported_best_neighbour_cell_r18.size() > 0}));

  HANDLE_CODE(pack_integer(bref, meas_id, (uint8_t)1u, (uint8_t)64u));
  if (cells_triggered_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_triggered_list, 1, 32));
  }
  HANDLE_CODE(pack_unconstrained_integer(bref, nof_reports_sent));
  if (cli_triggered_list_r16_present) {
    HANDLE_CODE(cli_triggered_list_r16.pack(bref));
  }
  if (tx_pool_meas_to_add_mod_list_nr_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, tx_pool_meas_to_add_mod_list_nr_r16, 1, 8, integer_packer<uint8_t>(1, 16)));
  }
  if (relays_triggered_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, relays_triggered_list_r17, 1, 32));
  }
  if (cells_met_leaving_cond_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_met_leaving_cond_r18, 1, 8, integer_packer<uint16_t>(0, 1007)));
  }
  if (reported_best_neighbour_cell_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, reported_best_neighbour_cell_r18, 1, 2, integer_packer<uint16_t>(0, 1007)));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_meas_report_s::unpack(cbit_ref& bref)
{
  bool cells_triggered_list_present;
  bool tx_pool_meas_to_add_mod_list_nr_r16_present;
  bool relays_triggered_list_r17_present;
  bool cells_met_leaving_cond_r18_present;
  bool reported_best_neighbour_cell_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&cells_triggered_list_present,
                           &cli_triggered_list_r16_present,
                           &tx_pool_meas_to_add_mod_list_nr_r16_present,
                           &relays_triggered_list_r17_present,
                           &cells_met_leaving_cond_r18_present,
                           &reported_best_neighbour_cell_r18_present}));

  HANDLE_CODE(unpack_integer(meas_id, bref, (uint8_t)1u, (uint8_t)64u));
  if (cells_triggered_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cells_triggered_list, bref, 1, 32));
  }
  HANDLE_CODE(unpack_unconstrained_integer(nof_reports_sent, bref));
  if (cli_triggered_list_r16_present) {
    HANDLE_CODE(cli_triggered_list_r16.unpack(bref));
  }
  if (tx_pool_meas_to_add_mod_list_nr_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(tx_pool_meas_to_add_mod_list_nr_r16, bref, 1, 8, integer_packer<uint8_t>(1, 16)));
  }
  if (relays_triggered_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(relays_triggered_list_r17, bref, 1, 32));
  }
  if (cells_met_leaving_cond_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cells_met_leaving_cond_r18, bref, 1, 8, integer_packer<uint16_t>(0, 1007)));
  }
  if (reported_best_neighbour_cell_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(reported_best_neighbour_cell_r18, bref, 1, 2, integer_packer<uint16_t>(0, 1007)));
  }

  return OCUDUASN_SUCCESS;
}
void var_meas_report_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("measId", meas_id);
  if (cells_triggered_list.size() > 0) {
    j.start_array("cellsTriggeredList");
    for (const auto& e1 : cells_triggered_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.write_int("numberOfReportsSent", nof_reports_sent);
  if (cli_triggered_list_r16_present) {
    j.write_fieldname("cli-TriggeredList-r16");
    cli_triggered_list_r16.to_json(j);
  }
  if (tx_pool_meas_to_add_mod_list_nr_r16.size() > 0) {
    j.start_array("tx-PoolMeasToAddModListNR-r16");
    for (const auto& e1 : tx_pool_meas_to_add_mod_list_nr_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (relays_triggered_list_r17.size() > 0) {
    j.start_array("relaysTriggeredList-r17");
    for (const auto& e1 : relays_triggered_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cells_met_leaving_cond_r18.size() > 0) {
    j.start_array("cellsMetLeavingCond-r18");
    for (const auto& e1 : cells_met_leaving_cond_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (reported_best_neighbour_cell_r18.size() > 0) {
    j.start_array("reportedBestNeighbourCell-r18");
    for (const auto& e1 : reported_best_neighbour_cell_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  j.end_obj();
}

// VarMeasReportSL-r16 ::= SEQUENCE
OCUDUASN_CODE var_meas_report_sl_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_freq_triggered_list_r16.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, sl_meas_id_r16, (uint8_t)1u, (uint8_t)64u));
  if (sl_freq_triggered_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_freq_triggered_list_r16, 1, 8, integer_packer<uint32_t>(0, 3279165)));
  }
  HANDLE_CODE(pack_unconstrained_integer(bref, sl_nof_reports_sent_r16));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_meas_report_sl_r16_s::unpack(cbit_ref& bref)
{
  bool sl_freq_triggered_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_freq_triggered_list_r16_present, 1));

  HANDLE_CODE(unpack_integer(sl_meas_id_r16, bref, (uint8_t)1u, (uint8_t)64u));
  if (sl_freq_triggered_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_freq_triggered_list_r16, bref, 1, 8, integer_packer<uint32_t>(0, 3279165)));
  }
  HANDLE_CODE(unpack_unconstrained_integer(sl_nof_reports_sent_r16, bref));

  return OCUDUASN_SUCCESS;
}
void var_meas_report_sl_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sl-MeasId-r16", sl_meas_id_r16);
  if (sl_freq_triggered_list_r16.size() > 0) {
    j.start_array("sl-FrequencyTriggeredList-r16");
    for (const auto& e1 : sl_freq_triggered_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  j.write_int("sl-NumberOfReportsSent-r16", sl_nof_reports_sent_r16);
  j.end_obj();
}

// VarMeasReselectionConfig-r18 ::= SEQUENCE
OCUDUASN_CODE var_meas_resel_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {meas_resel_carrier_list_nr_r18.size() > 0, meas_resel_validity_dur_r18_present}));

  if (meas_resel_carrier_list_nr_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, meas_resel_carrier_list_nr_r18, 1, 8));
  }
  if (meas_resel_validity_dur_r18_present) {
    HANDLE_CODE(meas_resel_validity_dur_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_meas_resel_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool meas_resel_carrier_list_nr_r18_present;
  HANDLE_CODE(unpack_bits(bref, {&meas_resel_carrier_list_nr_r18_present, &meas_resel_validity_dur_r18_present}));

  if (meas_resel_carrier_list_nr_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(meas_resel_carrier_list_nr_r18, bref, 1, 8));
  }
  if (meas_resel_validity_dur_r18_present) {
    HANDLE_CODE(meas_resel_validity_dur_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void var_meas_resel_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (meas_resel_carrier_list_nr_r18.size() > 0) {
    j.start_array("measReselectionCarrierListNR-r18");
    for (const auto& e1 : meas_resel_carrier_list_nr_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (meas_resel_validity_dur_r18_present) {
    j.write_str("measReselectionValidityDuration-r18", meas_resel_validity_dur_r18.to_string());
  }
  j.end_obj();
}

// VarMobilityHistoryReport-r17 ::= SEQUENCE
OCUDUASN_CODE var_mob_history_report_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(visited_pscell_info_list_r17.size() > 0, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, visited_cell_info_list_r16, 1, 16));
  if (visited_pscell_info_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, visited_pscell_info_list_r17, 1, 16));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_mob_history_report_r17_s::unpack(cbit_ref& bref)
{
  bool visited_pscell_info_list_r17_present;
  HANDLE_CODE(bref.unpack(visited_pscell_info_list_r17_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(visited_cell_info_list_r16, bref, 1, 16));
  if (visited_pscell_info_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(visited_pscell_info_list_r17, bref, 1, 16));
  }

  return OCUDUASN_SUCCESS;
}
void var_mob_history_report_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("visitedCellInfoList-r16");
  for (const auto& e1 : visited_cell_info_list_r16) {
    e1.to_json(j);
  }
  j.end_array();
  if (visited_pscell_info_list_r17.size() > 0) {
    j.start_array("visitedPSCellInfoList-r17");
    for (const auto& e1 : visited_pscell_info_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// VarPendingRNA-Update ::= SEQUENCE
OCUDUASN_CODE var_pending_rna_upd_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(pending_rna_upd_present, 1));

  if (pending_rna_upd_present) {
    HANDLE_CODE(bref.pack(pending_rna_upd, 1));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_pending_rna_upd_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(pending_rna_upd_present, 1));

  if (pending_rna_upd_present) {
    HANDLE_CODE(bref.unpack(pending_rna_upd, 1));
  }

  return OCUDUASN_SUCCESS;
}
void var_pending_rna_upd_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pending_rna_upd_present) {
    j.write_bool("pendingRNA-Update", pending_rna_upd);
  }
  j.end_obj();
}

// VarRA-Report-r16 ::= SEQUENCE
OCUDUASN_CODE var_ra_report_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_dyn_seq_of(bref, ra_report_list_r16, 1, 8));
  HANDLE_CODE(id_list_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_ra_report_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_dyn_seq_of(ra_report_list_r16, bref, 1, 8));
  HANDLE_CODE(id_list_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void var_ra_report_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("ra-ReportList-r16");
  for (const auto& e1 : ra_report_list_r16) {
    e1.to_json(j);
  }
  j.end_array();
  j.write_fieldname("identityList-r18");
  id_list_r18.to_json(j);
  j.end_obj();
}

void var_ra_report_r16_s::id_list_r18_c_::destroy_()
{
  switch (type_) {
    case types::plmn_id_list_r18:
      c.destroy<plmn_id_list2_r16_l>();
      break;
    case types::snpn_id_list_r18:
      c.destroy<snpn_id_list_r18_l_>();
      break;
    default:
      break;
  }
}
void var_ra_report_r16_s::id_list_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init<plmn_id_list2_r16_l>();
      break;
    case types::snpn_id_list_r18:
      c.init<snpn_id_list_r18_l_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_ra_report_r16_s::id_list_r18_c_");
  }
}
var_ra_report_r16_s::id_list_r18_c_::id_list_r18_c_(const var_ra_report_r16_s::id_list_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_id_list_r18:
      c.init(other.c.get<snpn_id_list_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_ra_report_r16_s::id_list_r18_c_");
  }
}
var_ra_report_r16_s::id_list_r18_c_&
var_ra_report_r16_s::id_list_r18_c_::operator=(const var_ra_report_r16_s::id_list_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::plmn_id_list_r18:
      c.set(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_id_list_r18:
      c.set(other.c.get<snpn_id_list_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_ra_report_r16_s::id_list_r18_c_");
  }

  return *this;
}
plmn_id_list2_r16_l& var_ra_report_r16_s::id_list_r18_c_::set_plmn_id_list_r18()
{
  set(types::plmn_id_list_r18);
  return c.get<plmn_id_list2_r16_l>();
}
var_ra_report_r16_s::id_list_r18_c_::snpn_id_list_r18_l_& var_ra_report_r16_s::id_list_r18_c_::set_snpn_id_list_r18()
{
  set(types::snpn_id_list_r18);
  return c.get<snpn_id_list_r18_l_>();
}
void var_ra_report_r16_s::id_list_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::plmn_id_list_r18:
      j.start_array("plmn-IdentityList-r18");
      for (const auto& e1 : c.get<plmn_id_list2_r16_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::snpn_id_list_r18:
      j.start_array("snpn-IdentityList-r18");
      for (const auto& e1 : c.get<snpn_id_list_r18_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "var_ra_report_r16_s::id_list_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE var_ra_report_r16_s::id_list_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<plmn_id_list2_r16_l>(), 1, 16));
      break;
    case types::snpn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<snpn_id_list_r18_l_>(), 1, 12));
      break;
    default:
      log_invalid_choice_id(type_, "var_ra_report_r16_s::id_list_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_ra_report_r16_s::id_list_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<plmn_id_list2_r16_l>(), bref, 1, 16));
      break;
    case types::snpn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<snpn_id_list_r18_l_>(), bref, 1, 12));
      break;
    default:
      log_invalid_choice_id(type_, "var_ra_report_r16_s::id_list_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* var_ra_report_r16_s::id_list_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"plmn-IdentityList-r18", "snpn-IdentityList-r18"};
  return convert_enum_idx(names, 2, value, "var_ra_report_r16_s::id_list_r18_c_::types");
}

// VarRLF-Report-r16 ::= SEQUENCE
OCUDUASN_CODE var_rlf_report_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(rlf_report_r16.pack(bref));
  HANDLE_CODE(id_list_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_rlf_report_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(rlf_report_r16.unpack(bref));
  HANDLE_CODE(id_list_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void var_rlf_report_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("rlf-Report-r16");
  rlf_report_r16.to_json(j);
  j.write_fieldname("identityList-r18");
  id_list_r18.to_json(j);
  j.end_obj();
}

void var_rlf_report_r16_s::id_list_r18_c_::destroy_()
{
  switch (type_) {
    case types::plmn_id_list_r18:
      c.destroy<plmn_id_list2_r16_l>();
      break;
    case types::snpn_id_list_r18:
      c.destroy<snpn_id_list_r18_l_>();
      break;
    default:
      break;
  }
}
void var_rlf_report_r16_s::id_list_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init<plmn_id_list2_r16_l>();
      break;
    case types::snpn_id_list_r18:
      c.init<snpn_id_list_r18_l_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_rlf_report_r16_s::id_list_r18_c_");
  }
}
var_rlf_report_r16_s::id_list_r18_c_::id_list_r18_c_(const var_rlf_report_r16_s::id_list_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_id_list_r18:
      c.init(other.c.get<snpn_id_list_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_rlf_report_r16_s::id_list_r18_c_");
  }
}
var_rlf_report_r16_s::id_list_r18_c_&
var_rlf_report_r16_s::id_list_r18_c_::operator=(const var_rlf_report_r16_s::id_list_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::plmn_id_list_r18:
      c.set(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_id_list_r18:
      c.set(other.c.get<snpn_id_list_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_rlf_report_r16_s::id_list_r18_c_");
  }

  return *this;
}
plmn_id_list2_r16_l& var_rlf_report_r16_s::id_list_r18_c_::set_plmn_id_list_r18()
{
  set(types::plmn_id_list_r18);
  return c.get<plmn_id_list2_r16_l>();
}
var_rlf_report_r16_s::id_list_r18_c_::snpn_id_list_r18_l_& var_rlf_report_r16_s::id_list_r18_c_::set_snpn_id_list_r18()
{
  set(types::snpn_id_list_r18);
  return c.get<snpn_id_list_r18_l_>();
}
void var_rlf_report_r16_s::id_list_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::plmn_id_list_r18:
      j.start_array("plmn-IdentityList-r18");
      for (const auto& e1 : c.get<plmn_id_list2_r16_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::snpn_id_list_r18:
      j.start_array("snpn-IdentityList-r18");
      for (const auto& e1 : c.get<snpn_id_list_r18_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "var_rlf_report_r16_s::id_list_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE var_rlf_report_r16_s::id_list_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<plmn_id_list2_r16_l>(), 1, 16));
      break;
    case types::snpn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<snpn_id_list_r18_l_>(), 1, 12));
      break;
    default:
      log_invalid_choice_id(type_, "var_rlf_report_r16_s::id_list_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_rlf_report_r16_s::id_list_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<plmn_id_list2_r16_l>(), bref, 1, 16));
      break;
    case types::snpn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<snpn_id_list_r18_l_>(), bref, 1, 12));
      break;
    default:
      log_invalid_choice_id(type_, "var_rlf_report_r16_s::id_list_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* var_rlf_report_r16_s::id_list_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"plmn-IdentityList-r18", "snpn-IdentityList-r18"};
  return convert_enum_idx(names, 2, value, "var_rlf_report_r16_s::id_list_r18_c_::types");
}

// VarResumeMAC-Input ::= SEQUENCE
OCUDUASN_CODE var_resume_mac_input_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, source_pci, (uint16_t)0u, (uint16_t)1007u));
  HANDLE_CODE(target_cell_id.pack(bref));
  HANDLE_CODE(pack_integer(bref, source_c_rnti, (uint32_t)0u, (uint32_t)65535u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_resume_mac_input_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(source_pci, bref, (uint16_t)0u, (uint16_t)1007u));
  HANDLE_CODE(target_cell_id.unpack(bref));
  HANDLE_CODE(unpack_integer(source_c_rnti, bref, (uint32_t)0u, (uint32_t)65535u));

  return OCUDUASN_SUCCESS;
}
void var_resume_mac_input_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sourcePhysCellId", source_pci);
  j.write_fieldname("targetCellIdentity");
  target_cell_id.to_json(j);
  j.write_int("source-c-RNTI", source_c_rnti);
  j.end_obj();
}

// VarServingSecurityCellSetID ::= SEQUENCE
OCUDUASN_CODE var_serving_security_cell_set_id_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, serving_security_cell_set_id_r18, (uint8_t)1u, (uint8_t)9u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_serving_security_cell_set_id_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(serving_security_cell_set_id_r18, bref, (uint8_t)1u, (uint8_t)9u));

  return OCUDUASN_SUCCESS;
}
void var_serving_security_cell_set_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("servingSecurityCellSetId-r18", serving_security_cell_set_id_r18);
  j.end_obj();
}

// VarShortMAC-Input ::= SEQUENCE
OCUDUASN_CODE var_short_mac_input_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, source_pci, (uint16_t)0u, (uint16_t)1007u));
  HANDLE_CODE(target_cell_id.pack(bref));
  HANDLE_CODE(pack_integer(bref, source_c_rnti, (uint32_t)0u, (uint32_t)65535u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_short_mac_input_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(source_pci, bref, (uint16_t)0u, (uint16_t)1007u));
  HANDLE_CODE(target_cell_id.unpack(bref));
  HANDLE_CODE(unpack_integer(source_c_rnti, bref, (uint32_t)0u, (uint32_t)65535u));

  return OCUDUASN_SUCCESS;
}
void var_short_mac_input_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sourcePhysCellId", source_pci);
  j.write_fieldname("targetCellIdentity");
  target_cell_id.to_json(j);
  j.write_int("source-c-RNTI", source_c_rnti);
  j.end_obj();
}

// VarSuccessHO-Report-r17 ::= SEQUENCE
OCUDUASN_CODE var_success_ho_report_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(success_ho_report_r17.pack(bref));
  HANDLE_CODE(id_list_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_success_ho_report_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(success_ho_report_r17.unpack(bref));
  HANDLE_CODE(id_list_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void var_success_ho_report_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("successHO-Report-r17");
  success_ho_report_r17.to_json(j);
  j.write_fieldname("identityList-r18");
  id_list_r18.to_json(j);
  j.end_obj();
}

void var_success_ho_report_r17_s::id_list_r18_c_::destroy_()
{
  switch (type_) {
    case types::plmn_id_list_r18:
      c.destroy<plmn_id_list2_r16_l>();
      break;
    case types::snpn_id_list_r18:
      c.destroy<snpn_id_list_r18_l_>();
      break;
    default:
      break;
  }
}
void var_success_ho_report_r17_s::id_list_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init<plmn_id_list2_r16_l>();
      break;
    case types::snpn_id_list_r18:
      c.init<snpn_id_list_r18_l_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_success_ho_report_r17_s::id_list_r18_c_");
  }
}
var_success_ho_report_r17_s::id_list_r18_c_::id_list_r18_c_(const var_success_ho_report_r17_s::id_list_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_id_list_r18:
      c.init(other.c.get<snpn_id_list_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_success_ho_report_r17_s::id_list_r18_c_");
  }
}
var_success_ho_report_r17_s::id_list_r18_c_&
var_success_ho_report_r17_s::id_list_r18_c_::operator=(const var_success_ho_report_r17_s::id_list_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::plmn_id_list_r18:
      c.set(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_id_list_r18:
      c.set(other.c.get<snpn_id_list_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_success_ho_report_r17_s::id_list_r18_c_");
  }

  return *this;
}
plmn_id_list2_r16_l& var_success_ho_report_r17_s::id_list_r18_c_::set_plmn_id_list_r18()
{
  set(types::plmn_id_list_r18);
  return c.get<plmn_id_list2_r16_l>();
}
var_success_ho_report_r17_s::id_list_r18_c_::snpn_id_list_r18_l_&
var_success_ho_report_r17_s::id_list_r18_c_::set_snpn_id_list_r18()
{
  set(types::snpn_id_list_r18);
  return c.get<snpn_id_list_r18_l_>();
}
void var_success_ho_report_r17_s::id_list_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::plmn_id_list_r18:
      j.start_array("plmn-IdentityList-r18");
      for (const auto& e1 : c.get<plmn_id_list2_r16_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::snpn_id_list_r18:
      j.start_array("snpn-IdentityList-r18");
      for (const auto& e1 : c.get<snpn_id_list_r18_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "var_success_ho_report_r17_s::id_list_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE var_success_ho_report_r17_s::id_list_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<plmn_id_list2_r16_l>(), 1, 16));
      break;
    case types::snpn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<snpn_id_list_r18_l_>(), 1, 12));
      break;
    default:
      log_invalid_choice_id(type_, "var_success_ho_report_r17_s::id_list_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_success_ho_report_r17_s::id_list_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<plmn_id_list2_r16_l>(), bref, 1, 16));
      break;
    case types::snpn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<snpn_id_list_r18_l_>(), bref, 1, 12));
      break;
    default:
      log_invalid_choice_id(type_, "var_success_ho_report_r17_s::id_list_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* var_success_ho_report_r17_s::id_list_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"plmn-IdentityList-r18", "snpn-IdentityList-r18"};
  return convert_enum_idx(names, 2, value, "var_success_ho_report_r17_s::id_list_r18_c_::types");
}

// VarSuccessPSCell-Report-r18 ::= SEQUENCE
OCUDUASN_CODE var_success_pscell_report_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(success_pscell_report_r18.pack(bref));
  HANDLE_CODE(id_list_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_success_pscell_report_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(success_pscell_report_r18.unpack(bref));
  HANDLE_CODE(id_list_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void var_success_pscell_report_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("successPSCell-Report-r18");
  success_pscell_report_r18.to_json(j);
  j.write_fieldname("identityList-r18");
  id_list_r18.to_json(j);
  j.end_obj();
}

void var_success_pscell_report_r18_s::id_list_r18_c_::destroy_()
{
  switch (type_) {
    case types::plmn_id_list_r18:
      c.destroy<plmn_id_list2_r16_l>();
      break;
    case types::snpn_id_list_r18:
      c.destroy<snpn_id_list_r18_l_>();
      break;
    default:
      break;
  }
}
void var_success_pscell_report_r18_s::id_list_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init<plmn_id_list2_r16_l>();
      break;
    case types::snpn_id_list_r18:
      c.init<snpn_id_list_r18_l_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_success_pscell_report_r18_s::id_list_r18_c_");
  }
}
var_success_pscell_report_r18_s::id_list_r18_c_::id_list_r18_c_(
    const var_success_pscell_report_r18_s::id_list_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::plmn_id_list_r18:
      c.init(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_id_list_r18:
      c.init(other.c.get<snpn_id_list_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_success_pscell_report_r18_s::id_list_r18_c_");
  }
}
var_success_pscell_report_r18_s::id_list_r18_c_&
var_success_pscell_report_r18_s::id_list_r18_c_::operator=(const var_success_pscell_report_r18_s::id_list_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::plmn_id_list_r18:
      c.set(other.c.get<plmn_id_list2_r16_l>());
      break;
    case types::snpn_id_list_r18:
      c.set(other.c.get<snpn_id_list_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "var_success_pscell_report_r18_s::id_list_r18_c_");
  }

  return *this;
}
plmn_id_list2_r16_l& var_success_pscell_report_r18_s::id_list_r18_c_::set_plmn_id_list_r18()
{
  set(types::plmn_id_list_r18);
  return c.get<plmn_id_list2_r16_l>();
}
var_success_pscell_report_r18_s::id_list_r18_c_::snpn_id_list_r18_l_&
var_success_pscell_report_r18_s::id_list_r18_c_::set_snpn_id_list_r18()
{
  set(types::snpn_id_list_r18);
  return c.get<snpn_id_list_r18_l_>();
}
void var_success_pscell_report_r18_s::id_list_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::plmn_id_list_r18:
      j.start_array("plmn-IdentityList-r18");
      for (const auto& e1 : c.get<plmn_id_list2_r16_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::snpn_id_list_r18:
      j.start_array("snpn-IdentityList-r18");
      for (const auto& e1 : c.get<snpn_id_list_r18_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "var_success_pscell_report_r18_s::id_list_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE var_success_pscell_report_r18_s::id_list_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<plmn_id_list2_r16_l>(), 1, 16));
      break;
    case types::snpn_id_list_r18:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<snpn_id_list_r18_l_>(), 1, 12));
      break;
    default:
      log_invalid_choice_id(type_, "var_success_pscell_report_r18_s::id_list_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_success_pscell_report_r18_s::id_list_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::plmn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<plmn_id_list2_r16_l>(), bref, 1, 16));
      break;
    case types::snpn_id_list_r18:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<snpn_id_list_r18_l_>(), bref, 1, 12));
      break;
    default:
      log_invalid_choice_id(type_, "var_success_pscell_report_r18_s::id_list_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* var_success_pscell_report_r18_s::id_list_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"plmn-IdentityList-r18", "snpn-IdentityList-r18"};
  return convert_enum_idx(names, 2, value, "var_success_pscell_report_r18_s::id_list_r18_c_::types");
}

// VarTSS-Info-r18 ::= SEQUENCE
OCUDUASN_CODE var_tss_info_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, stored_event_id_r18, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(stored_global_gnb_id_r18.plmn_id_r18.pack(bref));
  HANDLE_CODE(stored_global_gnb_id_r18.gnb_id_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE var_tss_info_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(stored_event_id_r18, bref, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(stored_global_gnb_id_r18.plmn_id_r18.unpack(bref));
  HANDLE_CODE(stored_global_gnb_id_r18.gnb_id_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void var_tss_info_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("storedEventID-r18", stored_event_id_r18);
  j.write_fieldname("storedGlobalGnbID-r18");
  j.start_obj();
  j.write_fieldname("plmn-Identity-r18");
  stored_global_gnb_id_r18.plmn_id_r18.to_json(j);
  j.write_fieldname("gnb-ID-r18");
  stored_global_gnb_id_r18.gnb_id_r18.to_json(j);
  j.end_obj();
  j.end_obj();
}
