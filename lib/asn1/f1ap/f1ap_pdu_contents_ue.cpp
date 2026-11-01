// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/f1ap/f1ap_pdu_contents_ue.h"

using namespace asn1;
using namespace asn1::f1ap;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// DLRRCMessageTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t dl_rrc_msg_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 47, 64, 109, 50, 108, 184, 222, 218, 221, 217, 248, 597, 861, 919};
  return map_enum_number(names, 16, idx, "id");
}
bool dl_rrc_msg_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 47, 64, 109, 50, 108, 184, 222, 218, 221, 217, 248, 597, 861, 919};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dl_rrc_msg_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 47:
      return crit_e::reject;
    case 64:
      return crit_e::reject;
    case 109:
      return crit_e::ignore;
    case 50:
      return crit_e::reject;
    case 108:
      return crit_e::reject;
    case 184:
      return crit_e::ignore;
    case 222:
      return crit_e::reject;
    case 218:
      return crit_e::reject;
    case 221:
      return crit_e::ignore;
    case 217:
      return crit_e::reject;
    case 248:
      return crit_e::ignore;
    case 597:
      return crit_e::ignore;
    case 861:
      return crit_e::ignore;
    case 919:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dl_rrc_msg_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 47:
      return presence_e::optional;
    case 64:
      return presence_e::mandatory;
    case 109:
      return presence_e::optional;
    case 50:
      return presence_e::mandatory;
    case 108:
      return presence_e::optional;
    case 184:
      return presence_e::optional;
    case 222:
      return presence_e::optional;
    case 218:
      return presence_e::optional;
    case 221:
      return presence_e::optional;
    case 217:
      return presence_e::optional;
    case 248:
      return presence_e::optional;
    case 597:
      return presence_e::optional;
    case 861:
      return presence_e::optional;
    case 919:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dl_rrc_msg_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += old_gnb_du_ue_f1ap_id_present ? 1 : 0;
  nof_ies += execute_dupl_present ? 1 : 0;
  nof_ies += rat_freq_prio_info_present ? 1 : 0;
  nof_ies += rrc_delivery_status_request_present ? 1 : 0;
  nof_ies += ue_context_not_retrievable_present ? 1 : 0;
  nof_ies += redirected_rrc_msg_present ? 1 : 0;
  nof_ies += plmn_assist_info_for_net_shar_present ? 1 : 0;
  nof_ies += new_gnb_cu_ue_f1ap_id_present ? 1 : 0;
  nof_ies += add_rrm_prio_idx_present ? 1 : 0;
  nof_ies += srb_map_info_present ? 1 : 0;
  nof_ies += plmn_idx_nr_assist_info_for_net_shar_present ? 1 : 0;
  nof_ies += area_specific_semi_persistent_srs_pos_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (old_gnb_du_ue_f1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 47, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, old_gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, srb_id, (uint8_t)0u, (uint8_t)3u, true, true);
  }));
  if (execute_dupl_present) {
    HANDLE_CODE(pack_ie(bref, 109, crit_e{crit_e::ignore}, [this, &bref]() { return execute_dupl.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::reject}, [this, &bref]() { return rrc_container.pack(bref); }));
  if (rat_freq_prio_info_present) {
    HANDLE_CODE(pack_ie(bref, 108, crit_e{crit_e::reject}, [this, &bref]() { return rat_freq_prio_info.pack(bref); }));
  }
  if (rrc_delivery_status_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 184, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_delivery_status_request.pack(bref); }));
  }
  if (ue_context_not_retrievable_present) {
    HANDLE_CODE(
        pack_ie(bref, 222, crit_e{crit_e::reject}, [this, &bref]() { return ue_context_not_retrievable.pack(bref); }));
  }
  if (redirected_rrc_msg_present) {
    HANDLE_CODE(pack_ie(bref, 218, crit_e{crit_e::reject}, [this, &bref]() { return redirected_rrc_msg.pack(bref); }));
  }
  if (plmn_assist_info_for_net_shar_present) {
    HANDLE_CODE(pack_ie(
        bref, 221, crit_e{crit_e::ignore}, [this, &bref]() { return plmn_assist_info_for_net_shar.pack(bref); }));
  }
  if (new_gnb_cu_ue_f1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 217, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, new_gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (add_rrm_prio_idx_present) {
    HANDLE_CODE(pack_ie(bref, 248, crit_e{crit_e::ignore}, [this, &bref]() { return add_rrm_prio_idx.pack(bref); }));
  }
  if (srb_map_info_present) {
    HANDLE_CODE(pack_ie(bref, 597, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, srb_map_info, (uint8_t)1u, (uint8_t)32u, false, true);
    }));
  }
  if (plmn_idx_nr_assist_info_for_net_shar_present) {
    HANDLE_CODE(pack_ie(bref, 861, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, plmn_idx_nr_assist_info_for_net_shar, (uint8_t)1u, (uint8_t)12u, false, true);
    }));
  }
  if (area_specific_semi_persistent_srs_pos_info_present) {
    HANDLE_CODE(pack_ie(bref, 919, crit_e{crit_e::ignore}, [this, &bref]() {
      return area_specific_semi_persistent_srs_pos_info.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_rrc_msg_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_rrc_msg_transfer_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 47: {
        old_gnb_du_ue_f1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 64: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_integer(srb_id, bref, (uint8_t)0u, (uint8_t)3u, true, true); }));
        break;
      }
      case 109: {
        execute_dupl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return execute_dupl.unpack(bref); }));
        break;
      }
      case 50: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_container.unpack(bref); }));
        break;
      }
      case 108: {
        rat_freq_prio_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rat_freq_prio_info.unpack(bref); }));
        break;
      }
      case 184: {
        rrc_delivery_status_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_delivery_status_request.unpack(bref); }));
        break;
      }
      case 222: {
        ue_context_not_retrievable_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_not_retrievable.unpack(bref); }));
        break;
      }
      case 218: {
        redirected_rrc_msg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return redirected_rrc_msg.unpack(bref); }));
        break;
      }
      case 221: {
        plmn_assist_info_for_net_shar_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return plmn_assist_info_for_net_shar.unpack(bref); }));
        break;
      }
      case 217: {
        new_gnb_cu_ue_f1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 248: {
        add_rrm_prio_idx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return add_rrm_prio_idx.unpack(bref); }));
        break;
      }
      case 597: {
        srb_map_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(srb_map_info, bref, (uint8_t)1u, (uint8_t)32u, false, true);
        }));
        break;
      }
      case 861: {
        plmn_idx_nr_assist_info_for_net_shar_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(plmn_idx_nr_assist_info_for_net_shar, bref, (uint8_t)1u, (uint8_t)12u, false, true);
        }));
        break;
      }
      case 919: {
        area_specific_semi_persistent_srs_pos_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return area_specific_semi_persistent_srs_pos_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void dl_rrc_msg_transfer_ies_container::to_json(json_writer& j) const
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
  if (old_gnb_du_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 47);
    j.write_str("criticality", "reject");
    j.write_uint("value", old_gnb_du_ue_f1ap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 64);
  j.write_str("criticality", "reject");
  j.write_int("value", srb_id);
  j.end_obj();
  if (execute_dupl_present) {
    j.start_obj();
    j.write_int("id", 109);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 50);
  j.write_str("criticality", "reject");
  j.write_str("value", rrc_container.to_string());
  j.end_obj();
  if (rat_freq_prio_info_present) {
    j.start_obj();
    j.write_int("id", 108);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    rat_freq_prio_info.to_json(j);
    j.end_obj();
  }
  if (rrc_delivery_status_request_present) {
    j.start_obj();
    j.write_int("id", 184);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (ue_context_not_retrievable_present) {
    j.start_obj();
    j.write_int("id", 222);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (redirected_rrc_msg_present) {
    j.start_obj();
    j.write_int("id", 218);
    j.write_str("criticality", "reject");
    j.write_str("value", redirected_rrc_msg.to_string());
    j.end_obj();
  }
  if (plmn_assist_info_for_net_shar_present) {
    j.start_obj();
    j.write_int("id", 221);
    j.write_str("criticality", "ignore");
    j.write_str("value", plmn_assist_info_for_net_shar.to_string());
    j.end_obj();
  }
  if (new_gnb_cu_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 217);
    j.write_str("criticality", "reject");
    j.write_uint("value", new_gnb_cu_ue_f1ap_id);
    j.end_obj();
  }
  if (add_rrm_prio_idx_present) {
    j.start_obj();
    j.write_int("id", 248);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    add_rrm_prio_idx.to_json(j);
    j.end_obj();
  }
  if (srb_map_info_present) {
    j.start_obj();
    j.write_int("id", 597);
    j.write_str("criticality", "ignore");
    j.write_int("value", srb_map_info);
    j.end_obj();
  }
  if (plmn_idx_nr_assist_info_for_net_shar_present) {
    j.start_obj();
    j.write_int("id", 861);
    j.write_str("criticality", "ignore");
    j.write_int("value", plmn_idx_nr_assist_info_for_net_shar);
    j.end_obj();
  }
  if (area_specific_semi_persistent_srs_pos_info_present) {
    j.start_obj();
    j.write_int("id", 919);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    area_specific_semi_persistent_srs_pos_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// InitialULRRCMessageTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t init_ul_rrc_msg_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {41, 111, 95, 50, 128, 178, 78, 226, 241, 578, 592, 617, 783};
  return map_enum_number(names, 13, idx, "id");
}
bool init_ul_rrc_msg_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {41, 111, 95, 50, 128, 178, 78, 226, 241, 578, 592, 617, 783};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e init_ul_rrc_msg_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 41:
      return crit_e::reject;
    case 111:
      return crit_e::reject;
    case 95:
      return crit_e::reject;
    case 50:
      return crit_e::reject;
    case 128:
      return crit_e::reject;
    case 178:
      return crit_e::ignore;
    case 78:
      return crit_e::ignore;
    case 226:
      return crit_e::ignore;
    case 241:
      return crit_e::ignore;
    case 578:
      return crit_e::ignore;
    case 592:
      return crit_e::ignore;
    case 617:
      return crit_e::ignore;
    case 783:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e init_ul_rrc_msg_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 41:
      return presence_e::mandatory;
    case 111:
      return presence_e::mandatory;
    case 95:
      return presence_e::mandatory;
    case 50:
      return presence_e::mandatory;
    case 128:
      return presence_e::optional;
    case 178:
      return presence_e::optional;
    case 78:
      return presence_e::mandatory;
    case 226:
      return presence_e::optional;
    case 241:
      return presence_e::optional;
    case 578:
      return presence_e::optional;
    case 592:
      return presence_e::optional;
    case 617:
      return presence_e::optional;
    case 783:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE init_ul_rrc_msg_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += du_to_cu_rrc_container_present ? 1 : 0;
  nof_ies += sul_access_ind_present ? 1 : 0;
  nof_ies += ran_ue_id_present ? 1 : 0;
  nof_ies += rrc_container_rrc_setup_complete_present ? 1 : 0;
  nof_ies += nr_red_cap_ue_ind_present ? 1 : 0;
  nof_ies += sdt_info_present ? 1 : 0;
  nof_ies += sidelink_relay_cfg_present ? 1 : 0;
  nof_ies += nr_e_red_cap_ue_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 111, crit_e{crit_e::reject}, [this, &bref]() { return nr_cgi.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, c_rnti, (uint32_t)0u, (uint32_t)65535u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::reject}, [this, &bref]() { return rrc_container.pack(bref); }));
  if (du_to_cu_rrc_container_present) {
    HANDLE_CODE(
        pack_ie(bref, 128, crit_e{crit_e::reject}, [this, &bref]() { return du_to_cu_rrc_container.pack(bref); }));
  }
  if (sul_access_ind_present) {
    HANDLE_CODE(pack_ie(bref, 178, crit_e{crit_e::ignore}, [this, &bref]() { return sul_access_ind.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 78, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true);
  }));
  if (ran_ue_id_present) {
    HANDLE_CODE(pack_ie(bref, 226, crit_e{crit_e::ignore}, [this, &bref]() { return ran_ue_id.pack(bref); }));
  }
  if (rrc_container_rrc_setup_complete_present) {
    HANDLE_CODE(pack_ie(
        bref, 241, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_container_rrc_setup_complete.pack(bref); }));
  }
  if (nr_red_cap_ue_ind_present) {
    HANDLE_CODE(pack_ie(bref, 578, crit_e{crit_e::ignore}, [this, &bref]() { return nr_red_cap_ue_ind.pack(bref); }));
  }
  if (sdt_info_present) {
    HANDLE_CODE(pack_ie(bref, 592, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_info.pack(bref); }));
  }
  if (sidelink_relay_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 617, crit_e{crit_e::ignore}, [this, &bref]() { return sidelink_relay_cfg.pack(bref); }));
  }
  if (nr_e_red_cap_ue_ind_present) {
    HANDLE_CODE(pack_ie(bref, 783, crit_e{crit_e::ignore}, [this, &bref]() { return nr_e_red_cap_ue_ind.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE init_ul_rrc_msg_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &init_ul_rrc_msg_transfer_ies_o::get_crit));

    switch (id) {
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 111: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_cgi.unpack(bref); }));
        break;
      }
      case 95: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(c_rnti, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 50: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_container.unpack(bref); }));
        break;
      }
      case 128: {
        du_to_cu_rrc_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return du_to_cu_rrc_container.unpack(bref); }));
        break;
      }
      case 178: {
        sul_access_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sul_access_ind.unpack(bref); }));
        break;
      }
      case 78: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 226: {
        ran_ue_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_ue_id.unpack(bref); }));
        break;
      }
      case 241: {
        rrc_container_rrc_setup_complete_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_container_rrc_setup_complete.unpack(bref); }));
        break;
      }
      case 578: {
        nr_red_cap_ue_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_red_cap_ue_ind.unpack(bref); }));
        break;
      }
      case 592: {
        sdt_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_info.unpack(bref); }));
        break;
      }
      case 617: {
        sidelink_relay_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sidelink_relay_cfg.unpack(bref); }));
        break;
      }
      case 783: {
        nr_e_red_cap_ue_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_e_red_cap_ue_ind.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void init_ul_rrc_msg_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
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
  j.start_obj();
  j.write_int("id", 95);
  j.write_str("criticality", "reject");
  j.write_int("value", c_rnti);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 50);
  j.write_str("criticality", "reject");
  j.write_str("value", rrc_container.to_string());
  j.end_obj();
  if (du_to_cu_rrc_container_present) {
    j.start_obj();
    j.write_int("id", 128);
    j.write_str("criticality", "reject");
    j.write_str("value", du_to_cu_rrc_container.to_string());
    j.end_obj();
  }
  if (sul_access_ind_present) {
    j.start_obj();
    j.write_int("id", 178);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "ignore");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ran_ue_id_present) {
    j.start_obj();
    j.write_int("id", 226);
    j.write_str("criticality", "ignore");
    j.write_str("value", ran_ue_id.to_string());
    j.end_obj();
  }
  if (rrc_container_rrc_setup_complete_present) {
    j.start_obj();
    j.write_int("id", 241);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_container_rrc_setup_complete.to_string());
    j.end_obj();
  }
  if (nr_red_cap_ue_ind_present) {
    j.start_obj();
    j.write_int("id", 578);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (sdt_info_present) {
    j.start_obj();
    j.write_int("id", 592);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sdt_info.to_json(j);
    j.end_obj();
  }
  if (sidelink_relay_cfg_present) {
    j.start_obj();
    j.write_int("id", 617);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sidelink_relay_cfg.to_json(j);
    j.end_obj();
  }
  if (nr_e_red_cap_ue_ind_present) {
    j.start_obj();
    j.write_int("id", 783);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// UEContextModificationConfirmIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_mod_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 49, 19, 50, 7, 109, 195, 337, 604, 616, 653};
  return map_enum_number(names, 12, idx, "id");
}
bool ue_context_mod_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 49, 19, 50, 7, 109, 195, 337, 604, 616, 653};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_mod_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 49:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    case 50:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 109:
      return crit_e::ignore;
    case 195:
      return crit_e::ignore;
    case 337:
      return crit_e::ignore;
    case 604:
      return crit_e::reject;
    case 616:
      return crit_e::reject;
    case 653:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_mod_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 49:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    case 50:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 109:
      return presence_e::optional;
    case 195:
      return presence_e::optional;
    case 337:
      return presence_e::optional;
    case 604:
      return presence_e::optional;
    case 616:
      return presence_e::optional;
    case 653:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_mod_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += res_coordination_transfer_container_present ? 1 : 0;
  nof_ies += drbs_modified_conf_list_present ? 1 : 0;
  nof_ies += rrc_container_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += execute_dupl_present ? 1 : 0;
  nof_ies += res_coordination_transfer_info_present ? 1 : 0;
  nof_ies += sl_drbs_modified_conf_list_present ? 1 : 0;
  nof_ies += uu_rlc_ch_modified_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_modified_list_present ? 1 : 0;
  nof_ies += ue_multicast_m_rbs_confirmed_to_be_modified_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (res_coordination_transfer_container_present) {
    HANDLE_CODE(pack_ie(
        bref, 49, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_container.pack(bref); }));
  }
  if (drbs_modified_conf_list_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_modified_conf_list, 1, 64, true);
    }));
  }
  if (rrc_container_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_container.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (execute_dupl_present) {
    HANDLE_CODE(pack_ie(bref, 109, crit_e{crit_e::ignore}, [this, &bref]() { return execute_dupl.pack(bref); }));
  }
  if (res_coordination_transfer_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 195, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_info.pack(bref); }));
  }
  if (sl_drbs_modified_conf_list_present) {
    HANDLE_CODE(pack_ie(bref, 337, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_modified_conf_list, 1, 512, true);
    }));
  }
  if (uu_rlc_ch_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 604, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_modified_list, 1, 32, true);
    }));
  }
  if (pc5_rlc_ch_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 616, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_modified_list, 1, 512, true);
    }));
  }
  if (ue_multicast_m_rbs_confirmed_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 653, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_multicast_m_rbs_confirmed_to_be_modified_list, 1, 64, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_mod_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_mod_confirm_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 49: {
        res_coordination_transfer_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_container.unpack(bref); }));
        break;
      }
      case 19: {
        drbs_modified_conf_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_modified_conf_list, bref, 1, 64, true); }));
        break;
      }
      case 50: {
        rrc_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_container.unpack(bref); }));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 109: {
        execute_dupl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return execute_dupl.unpack(bref); }));
        break;
      }
      case 195: {
        res_coordination_transfer_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_info.unpack(bref); }));
        break;
      }
      case 337: {
        sl_drbs_modified_conf_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_modified_conf_list, bref, 1, 512, true); }));
        break;
      }
      case 604: {
        uu_rlc_ch_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_modified_list, bref, 1, 32, true); }));
        break;
      }
      case 616: {
        pc5_rlc_ch_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pc5_rlc_ch_modified_list, bref, 1, 512, true); }));
        break;
      }
      case 653: {
        ue_multicast_m_rbs_confirmed_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ue_multicast_m_rbs_confirmed_to_be_modified_list, bref, 1, 64, true);
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
void ue_context_mod_confirm_ies_container::to_json(json_writer& j) const
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
  if (res_coordination_transfer_container_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_str("value", res_coordination_transfer_container.to_string());
    j.end_obj();
  }
  if (drbs_modified_conf_list_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_modified_conf_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (rrc_container_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_container.to_string());
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
  if (execute_dupl_present) {
    j.start_obj();
    j.write_int("id", 109);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (res_coordination_transfer_info_present) {
    j.start_obj();
    j.write_int("id", 195);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    res_coordination_transfer_info.to_json(j);
    j.end_obj();
  }
  if (sl_drbs_modified_conf_list_present) {
    j.start_obj();
    j.write_int("id", 337);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_modified_conf_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_modified_list_present) {
    j.start_obj();
    j.write_int("id", 604);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_modified_list_present) {
    j.start_obj();
    j.write_int("id", 616);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_multicast_m_rbs_confirmed_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 653);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ue_multicast_m_rbs_confirmed_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// UEContextModificationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 0, 7, 376};
  return map_enum_number(names, 5, idx, "id");
}
bool ue_context_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 0, 7, 376};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 376:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_mod_fail_ies_o::get_presence(const uint32_t& id)
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
    case 376:
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
  nof_ies += requested_target_cell_global_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (requested_target_cell_global_id_present) {
    HANDLE_CODE(pack_ie(
        bref, 376, crit_e{crit_e::reject}, [this, &bref]() { return requested_target_cell_global_id.pack(bref); }));
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
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 376: {
        requested_target_cell_global_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_target_cell_global_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  if (requested_target_cell_global_id_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    requested_target_cell_global_id.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextModificationRefuseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_mod_refuse_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 0, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_context_mod_refuse_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_mod_refuse_ies_o::get_crit(const uint32_t& id)
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
presence_e ue_context_mod_refuse_ies_o::get_presence(const uint32_t& id)
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

OCUDUASN_CODE ue_context_mod_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_mod_refuse_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_mod_refuse_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 7: {
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
void ue_context_mod_refuse_ies_container::to_json(json_writer& j) const
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

// UEContextModificationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40,  41,  63,  107, 96,  38,  9,   79,  49,  87,  50,  56,  52,  76,  37,  31,
                                   72,  33,  97,  108, 159, 174, 175, 162, 158, 109, 184, 195, 182, 215, 94,  248,
                                   253, 267, 263, 265, 306, 307, 308, 309, 340, 332, 326, 328, 374, 428, 432, 684,
                                   510, 511, 536, 547, 586, 594, 595, 596, 618, 599, 600, 601, 608, 609, 610, 619,
                                   626, 632, 633, 685, 628, 643, 647, 675, 683, 695, 690, 691, 712, 716, 722, 845,
                                   721, 726, 728, 843, 723, 741, 779, 780, 781, 782, 794, 801, 838, 855, 863};
  return map_enum_number(names, 95, idx, "id");
}
bool ue_context_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40,  41,  63,  107, 96,  38,  9,   79,  49,  87,  50,  56,  52,  76,  37,  31,
                                   72,  33,  97,  108, 159, 174, 175, 162, 158, 109, 184, 195, 182, 215, 94,  248,
                                   253, 267, 263, 265, 306, 307, 308, 309, 340, 332, 326, 328, 374, 428, 432, 684,
                                   510, 511, 536, 547, 586, 594, 595, 596, 618, 599, 600, 601, 608, 609, 610, 619,
                                   626, 632, 633, 685, 628, 643, 647, 675, 683, 695, 690, 691, 712, 716, 722, 845,
                                   721, 726, 728, 843, 723, 741, 779, 780, 781, 782, 794, 801, 838, 855, 863};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 63:
      return crit_e::ignore;
    case 107:
      return crit_e::reject;
    case 96:
      return crit_e::ignore;
    case 38:
      return crit_e::ignore;
    case 9:
      return crit_e::reject;
    case 79:
      return crit_e::ignore;
    case 49:
      return crit_e::ignore;
    case 87:
      return crit_e::ignore;
    case 50:
      return crit_e::reject;
    case 56:
      return crit_e::ignore;
    case 52:
      return crit_e::ignore;
    case 76:
      return crit_e::reject;
    case 37:
      return crit_e::reject;
    case 31:
      return crit_e::reject;
    case 72:
      return crit_e::reject;
    case 33:
      return crit_e::reject;
    case 97:
      return crit_e::reject;
    case 108:
      return crit_e::reject;
    case 159:
      return crit_e::ignore;
    case 174:
      return crit_e::ignore;
    case 175:
      return crit_e::ignore;
    case 162:
      return crit_e::reject;
    case 158:
      return crit_e::ignore;
    case 109:
      return crit_e::ignore;
    case 184:
      return crit_e::ignore;
    case 195:
      return crit_e::ignore;
    case 182:
      return crit_e::ignore;
    case 215:
      return crit_e::ignore;
    case 94:
      return crit_e::reject;
    case 248:
      return crit_e::ignore;
    case 253:
      return crit_e::ignore;
    case 267:
      return crit_e::reject;
    case 263:
      return crit_e::reject;
    case 265:
      return crit_e::reject;
    case 306:
      return crit_e::ignore;
    case 307:
      return crit_e::ignore;
    case 308:
      return crit_e::ignore;
    case 309:
      return crit_e::ignore;
    case 340:
      return crit_e::ignore;
    case 332:
      return crit_e::reject;
    case 326:
      return crit_e::reject;
    case 328:
      return crit_e::reject;
    case 374:
      return crit_e::reject;
    case 428:
      return crit_e::reject;
    case 432:
      return crit_e::ignore;
    case 684:
      return crit_e::ignore;
    case 510:
      return crit_e::reject;
    case 511:
      return crit_e::reject;
    case 536:
      return crit_e::ignore;
    case 547:
      return crit_e::ignore;
    case 586:
      return crit_e::ignore;
    case 594:
      return crit_e::ignore;
    case 595:
      return crit_e::ignore;
    case 596:
      return crit_e::ignore;
    case 618:
      return crit_e::ignore;
    case 599:
      return crit_e::reject;
    case 600:
      return crit_e::reject;
    case 601:
      return crit_e::reject;
    case 608:
      return crit_e::reject;
    case 609:
      return crit_e::reject;
    case 610:
      return crit_e::reject;
    case 619:
      return crit_e::ignore;
    case 626:
      return crit_e::ignore;
    case 632:
      return crit_e::reject;
    case 633:
      return crit_e::reject;
    case 685:
      return crit_e::reject;
    case 628:
      return crit_e::reject;
    case 643:
      return crit_e::ignore;
    case 647:
      return crit_e::ignore;
    case 675:
      return crit_e::ignore;
    case 683:
      return crit_e::ignore;
    case 695:
      return crit_e::ignore;
    case 690:
      return crit_e::ignore;
    case 691:
      return crit_e::ignore;
    case 712:
      return crit_e::ignore;
    case 716:
      return crit_e::ignore;
    case 722:
      return crit_e::reject;
    case 845:
      return crit_e::ignore;
    case 721:
      return crit_e::reject;
    case 726:
      return crit_e::ignore;
    case 728:
      return crit_e::ignore;
    case 843:
      return crit_e::ignore;
    case 723:
      return crit_e::reject;
    case 741:
      return crit_e::reject;
    case 779:
      return crit_e::ignore;
    case 780:
      return crit_e::ignore;
    case 781:
      return crit_e::ignore;
    case 782:
      return crit_e::ignore;
    case 794:
      return crit_e::ignore;
    case 801:
      return crit_e::ignore;
    case 838:
      return crit_e::ignore;
    case 855:
      return crit_e::ignore;
    case 863:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 63:
      return presence_e::optional;
    case 107:
      return presence_e::optional;
    case 96:
      return presence_e::optional;
    case 38:
      return presence_e::optional;
    case 9:
      return presence_e::optional;
    case 79:
      return presence_e::optional;
    case 49:
      return presence_e::optional;
    case 87:
      return presence_e::optional;
    case 50:
      return presence_e::optional;
    case 56:
      return presence_e::optional;
    case 52:
      return presence_e::optional;
    case 76:
      return presence_e::optional;
    case 37:
      return presence_e::optional;
    case 31:
      return presence_e::optional;
    case 72:
      return presence_e::optional;
    case 33:
      return presence_e::optional;
    case 97:
      return presence_e::optional;
    case 108:
      return presence_e::optional;
    case 159:
      return presence_e::optional;
    case 174:
      return presence_e::optional;
    case 175:
      return presence_e::optional;
    case 162:
      return presence_e::optional;
    case 158:
      return presence_e::optional;
    case 109:
      return presence_e::optional;
    case 184:
      return presence_e::optional;
    case 195:
      return presence_e::optional;
    case 182:
      return presence_e::optional;
    case 215:
      return presence_e::optional;
    case 94:
      return presence_e::optional;
    case 248:
      return presence_e::optional;
    case 253:
      return presence_e::optional;
    case 267:
      return presence_e::optional;
    case 263:
      return presence_e::optional;
    case 265:
      return presence_e::optional;
    case 306:
      return presence_e::optional;
    case 307:
      return presence_e::optional;
    case 308:
      return presence_e::optional;
    case 309:
      return presence_e::optional;
    case 340:
      return presence_e::optional;
    case 332:
      return presence_e::optional;
    case 326:
      return presence_e::optional;
    case 328:
      return presence_e::optional;
    case 374:
      return presence_e::optional;
    case 428:
      return presence_e::optional;
    case 432:
      return presence_e::optional;
    case 684:
      return presence_e::optional;
    case 510:
      return presence_e::optional;
    case 511:
      return presence_e::optional;
    case 536:
      return presence_e::optional;
    case 547:
      return presence_e::optional;
    case 586:
      return presence_e::optional;
    case 594:
      return presence_e::optional;
    case 595:
      return presence_e::optional;
    case 596:
      return presence_e::optional;
    case 618:
      return presence_e::optional;
    case 599:
      return presence_e::optional;
    case 600:
      return presence_e::optional;
    case 601:
      return presence_e::optional;
    case 608:
      return presence_e::optional;
    case 609:
      return presence_e::optional;
    case 610:
      return presence_e::optional;
    case 619:
      return presence_e::optional;
    case 626:
      return presence_e::optional;
    case 632:
      return presence_e::optional;
    case 633:
      return presence_e::optional;
    case 685:
      return presence_e::optional;
    case 628:
      return presence_e::optional;
    case 643:
      return presence_e::optional;
    case 647:
      return presence_e::optional;
    case 675:
      return presence_e::optional;
    case 683:
      return presence_e::optional;
    case 695:
      return presence_e::optional;
    case 690:
      return presence_e::optional;
    case 691:
      return presence_e::optional;
    case 712:
      return presence_e::optional;
    case 716:
      return presence_e::optional;
    case 722:
      return presence_e::optional;
    case 845:
      return presence_e::optional;
    case 721:
      return presence_e::optional;
    case 726:
      return presence_e::optional;
    case 728:
      return presence_e::optional;
    case 843:
      return presence_e::optional;
    case 723:
      return presence_e::optional;
    case 741:
      return presence_e::optional;
    case 779:
      return presence_e::optional;
    case 780:
      return presence_e::optional;
    case 781:
      return presence_e::optional;
    case 782:
      return presence_e::optional;
    case 794:
      return presence_e::optional;
    case 801:
      return presence_e::optional;
    case 838:
      return presence_e::optional;
    case 855:
      return presence_e::optional;
    case 863:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += sp_cell_id_present ? 1 : 0;
  nof_ies += serv_cell_idx_present ? 1 : 0;
  nof_ies += sp_cell_ul_cfg_present ? 1 : 0;
  nof_ies += drx_cycle_present ? 1 : 0;
  nof_ies += cu_to_du_rrc_info_present ? 1 : 0;
  nof_ies += tx_action_ind_present ? 1 : 0;
  nof_ies += res_coordination_transfer_container_present ? 1 : 0;
  nof_ies += rrc_recfg_complete_ind_present ? 1 : 0;
  nof_ies += rrc_container_present ? 1 : 0;
  nof_ies += scell_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += scell_to_be_remd_list_present ? 1 : 0;
  nof_ies += srbs_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += drbs_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += drbs_to_be_modified_list_present ? 1 : 0;
  nof_ies += srbs_to_be_released_list_present ? 1 : 0;
  nof_ies += drbs_to_be_released_list_present ? 1 : 0;
  nof_ies += inactivity_monitoring_request_present ? 1 : 0;
  nof_ies += rat_freq_prio_info_present ? 1 : 0;
  nof_ies += drx_cfg_ind_present ? 1 : 0;
  nof_ies += rlc_fail_ind_present ? 1 : 0;
  nof_ies += ul_tx_direct_current_list_info_present ? 1 : 0;
  nof_ies += gnb_du_cfg_query_present ? 1 : 0;
  nof_ies += gnb_du_ue_ambr_ul_present ? 1 : 0;
  nof_ies += execute_dupl_present ? 1 : 0;
  nof_ies += rrc_delivery_status_request_present ? 1 : 0;
  nof_ies += res_coordination_transfer_info_present ? 1 : 0;
  nof_ies += serving_cell_mo_present ? 1 : 0;
  nof_ies += needfor_gap_present ? 1 : 0;
  nof_ies += full_cfg_present ? 1 : 0;
  nof_ies += add_rrm_prio_idx_present ? 1 : 0;
  nof_ies += lower_layer_presence_status_change_present ? 1 : 0;
  nof_ies += bh_chs_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += bh_chs_to_be_modified_list_present ? 1 : 0;
  nof_ies += bh_chs_to_be_released_list_present ? 1 : 0;
  nof_ies += nr_v2x_services_authorized_present ? 1 : 0;
  nof_ies += ltev2x_services_authorized_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += pc5_link_ambr_present ? 1 : 0;
  nof_ies += sl_drbs_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += sl_drbs_to_be_modified_list_present ? 1 : 0;
  nof_ies += sl_drbs_to_be_released_list_present ? 1 : 0;
  nof_ies += conditional_intra_du_mob_info_present ? 1 : 0;
  nof_ies += f1_c_transfer_path_present ? 1 : 0;
  nof_ies += scg_ind_present ? 1 : 0;
  nof_ies += ul_tx_direct_current_two_carrier_list_info_present ? 1 : 0;
  nof_ies += iab_conditional_rrc_msg_delivery_ind_present ? 1 : 0;
  nof_ies += f1_c_transfer_path_nr_dc_present ? 1 : 0;
  nof_ies += mdt_polluted_meas_ind_present ? 1 : 0;
  nof_ies += scg_activation_request_present ? 1 : 0;
  nof_ies += cg_sdt_query_ind_present ? 1 : 0;
  nof_ies += five_g_pro_se_authorized_present ? 1 : 0;
  nof_ies += five_g_pro_se_ue_pc5_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += five_g_pro_se_pc5_link_ambr_present ? 1 : 0;
  nof_ies += upd_remote_ue_local_id_present ? 1 : 0;
  nof_ies += uu_rlc_ch_to_be_setup_list_present ? 1 : 0;
  nof_ies += uu_rlc_ch_to_be_modified_list_present ? 1 : 0;
  nof_ies += uu_rlc_ch_to_be_released_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_to_be_setup_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_to_be_modified_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_to_be_released_list_present ? 1 : 0;
  nof_ies += path_switch_cfg_present ? 1 : 0;
  nof_ies += gnb_du_ue_slice_max_bit_rate_list_present ? 1 : 0;
  nof_ies += multicast_mbs_session_setup_list_present ? 1 : 0;
  nof_ies += multicast_mbs_session_rem_list_present ? 1 : 0;
  nof_ies += ue_multicast_m_rbs_to_be_setup_at_modify_list_present ? 1 : 0;
  nof_ies += ue_multicast_m_rbs_to_be_released_list_present ? 1 : 0;
  nof_ies += sldrx_cycle_list_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_mod_list_present ? 1 : 0;
  nof_ies += sdt_bearer_cfg_query_ind_present ? 1 : 0;
  nof_ies += daps_ho_status_present ? 1 : 0;
  nof_ies += serving_cell_mo_list_present ? 1 : 0;
  nof_ies += ul_tx_direct_current_more_carrier_info_present ? 1 : 0;
  nof_ies += cp_acmcg_info_present ? 1 : 0;
  nof_ies += network_ctrl_repeater_authorized_present ? 1 : 0;
  nof_ies += sdt_volume_thres_present ? 1 : 0;
  nof_ies += ltm_info_modify_present ? 1 : 0;
  nof_ies += ltmcfra_res_cfg_list_present ? 1 : 0;
  nof_ies += ltm_cfg_id_map_list_present ? 1 : 0;
  nof_ies += early_sync_info_request_present ? 1 : 0;
  nof_ies += early_sync_candidate_cell_info_list_present ? 1 : 0;
  nof_ies += early_sync_serving_cell_info_present ? 1 : 0;
  nof_ies += ltm_cells_to_be_released_list_present ? 1 : 0;
  nof_ies += path_addition_info_present ? 1 : 0;
  nof_ies += nr_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += ltea2_x_services_authorized_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present ? 1 : 0;
  nof_ies += dl_lbt_fail_info_request_present ? 1 : 0;
  nof_ies += sl_positioning_ranging_service_info_present ? 1 : 0;
  nof_ies += non_integer_drx_cycle_present ? 1 : 0;
  nof_ies += ltm_reset_info_present ? 1 : 0;
  nof_ies += ltmtci_states_cfgs_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (sp_cell_id_present) {
    HANDLE_CODE(pack_ie(bref, 63, crit_e{crit_e::ignore}, [this, &bref]() { return sp_cell_id.pack(bref); }));
  }
  if (serv_cell_idx_present) {
    HANDLE_CODE(pack_ie(bref, 107, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, serv_cell_idx, (uint8_t)0u, (uint8_t)31u, true, true);
    }));
  }
  if (sp_cell_ul_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 96, crit_e{crit_e::ignore}, [this, &bref]() { return sp_cell_ul_cfg.pack(bref); }));
  }
  if (drx_cycle_present) {
    HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::ignore}, [this, &bref]() { return drx_cycle.pack(bref); }));
  }
  if (cu_to_du_rrc_info_present) {
    HANDLE_CODE(pack_ie(bref, 9, crit_e{crit_e::reject}, [this, &bref]() { return cu_to_du_rrc_info.pack(bref); }));
  }
  if (tx_action_ind_present) {
    HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::ignore}, [this, &bref]() { return tx_action_ind.pack(bref); }));
  }
  if (res_coordination_transfer_container_present) {
    HANDLE_CODE(pack_ie(
        bref, 49, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_container.pack(bref); }));
  }
  if (rrc_recfg_complete_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 87, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_recfg_complete_ind.pack(bref); }));
  }
  if (rrc_container_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::reject}, [this, &bref]() { return rrc_container.pack(bref); }));
  }
  if (scell_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 56, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, scell_to_be_setup_mod_list, 1, 32, true);
    }));
  }
  if (scell_to_be_remd_list_present) {
    HANDLE_CODE(pack_ie(bref, 52, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, scell_to_be_remd_list, 1, 32, true);
    }));
  }
  if (srbs_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 76, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_to_be_setup_mod_list, 1, 8, true);
    }));
  }
  if (drbs_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 37, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_to_be_setup_mod_list, 1, 64, true);
    }));
  }
  if (drbs_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 31, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_to_be_modified_list, 1, 64, true);
    }));
  }
  if (srbs_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 72, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_to_be_released_list, 1, 8, true);
    }));
  }
  if (drbs_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 33, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_to_be_released_list, 1, 64, true);
    }));
  }
  if (inactivity_monitoring_request_present) {
    HANDLE_CODE(pack_ie(
        bref, 97, crit_e{crit_e::reject}, [this, &bref]() { return inactivity_monitoring_request.pack(bref); }));
  }
  if (rat_freq_prio_info_present) {
    HANDLE_CODE(pack_ie(bref, 108, crit_e{crit_e::reject}, [this, &bref]() { return rat_freq_prio_info.pack(bref); }));
  }
  if (drx_cfg_ind_present) {
    HANDLE_CODE(pack_ie(bref, 159, crit_e{crit_e::ignore}, [this, &bref]() { return drx_cfg_ind.pack(bref); }));
  }
  if (rlc_fail_ind_present) {
    HANDLE_CODE(pack_ie(bref, 174, crit_e{crit_e::ignore}, [this, &bref]() { return rlc_fail_ind.pack(bref); }));
  }
  if (ul_tx_direct_current_list_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 175, crit_e{crit_e::ignore}, [this, &bref]() { return ul_tx_direct_current_list_info.pack(bref); }));
  }
  if (gnb_du_cfg_query_present) {
    HANDLE_CODE(pack_ie(bref, 162, crit_e{crit_e::reject}, [this, &bref]() { return gnb_du_cfg_query.pack(bref); }));
  }
  if (gnb_du_ue_ambr_ul_present) {
    HANDLE_CODE(pack_ie(bref, 158, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_du_ue_ambr_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (execute_dupl_present) {
    HANDLE_CODE(pack_ie(bref, 109, crit_e{crit_e::ignore}, [this, &bref]() { return execute_dupl.pack(bref); }));
  }
  if (rrc_delivery_status_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 184, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_delivery_status_request.pack(bref); }));
  }
  if (res_coordination_transfer_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 195, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_info.pack(bref); }));
  }
  if (serving_cell_mo_present) {
    HANDLE_CODE(pack_ie(bref, 182, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, serving_cell_mo, (uint16_t)1u, (uint16_t)64u, true, true);
    }));
  }
  if (needfor_gap_present) {
    HANDLE_CODE(pack_ie(bref, 215, crit_e{crit_e::ignore}, [this, &bref]() { return needfor_gap.pack(bref); }));
  }
  if (full_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 94, crit_e{crit_e::reject}, [this, &bref]() { return full_cfg.pack(bref); }));
  }
  if (add_rrm_prio_idx_present) {
    HANDLE_CODE(pack_ie(bref, 248, crit_e{crit_e::ignore}, [this, &bref]() { return add_rrm_prio_idx.pack(bref); }));
  }
  if (lower_layer_presence_status_change_present) {
    HANDLE_CODE(pack_ie(
        bref, 253, crit_e{crit_e::ignore}, [this, &bref]() { return lower_layer_presence_status_change.pack(bref); }));
  }
  if (bh_chs_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 267, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_to_be_setup_mod_list, 1, 65536, true);
    }));
  }
  if (bh_chs_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 263, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_to_be_modified_list, 1, 65536, true);
    }));
  }
  if (bh_chs_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 265, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_to_be_released_list, 1, 65536, true);
    }));
  }
  if (nr_v2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 306, crit_e{crit_e::ignore}, [this, &bref]() { return nr_v2x_services_authorized.pack(bref); }));
  }
  if (ltev2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 307, crit_e{crit_e::ignore}, [this, &bref]() { return ltev2x_services_authorized.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 308, crit_e{crit_e::ignore}, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 309, crit_e{crit_e::ignore}, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (pc5_link_ambr_present) {
    HANDLE_CODE(pack_ie(bref, 340, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, pc5_link_ambr, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (sl_drbs_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 332, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_to_be_setup_mod_list, 1, 512, true);
    }));
  }
  if (sl_drbs_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 326, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_to_be_modified_list, 1, 512, true);
    }));
  }
  if (sl_drbs_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 328, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_to_be_released_list, 1, 512, true);
    }));
  }
  if (conditional_intra_du_mob_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 374, crit_e{crit_e::reject}, [this, &bref]() { return conditional_intra_du_mob_info.pack(bref); }));
  }
  if (f1_c_transfer_path_present) {
    HANDLE_CODE(pack_ie(bref, 428, crit_e{crit_e::reject}, [this, &bref]() { return f1_c_transfer_path.pack(bref); }));
  }
  if (scg_ind_present) {
    HANDLE_CODE(pack_ie(bref, 432, crit_e{crit_e::ignore}, [this, &bref]() { return scg_ind.pack(bref); }));
  }
  if (ul_tx_direct_current_two_carrier_list_info_present) {
    HANDLE_CODE(pack_ie(bref, 684, crit_e{crit_e::ignore}, [this, &bref]() {
      return ul_tx_direct_current_two_carrier_list_info.pack(bref);
    }));
  }
  if (iab_conditional_rrc_msg_delivery_ind_present) {
    HANDLE_CODE(pack_ie(bref, 510, crit_e{crit_e::reject}, [this, &bref]() {
      return iab_conditional_rrc_msg_delivery_ind.pack(bref);
    }));
  }
  if (f1_c_transfer_path_nr_dc_present) {
    HANDLE_CODE(
        pack_ie(bref, 511, crit_e{crit_e::reject}, [this, &bref]() { return f1_c_transfer_path_nr_dc.pack(bref); }));
  }
  if (mdt_polluted_meas_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 536, crit_e{crit_e::ignore}, [this, &bref]() { return mdt_polluted_meas_ind.pack(bref); }));
  }
  if (scg_activation_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 547, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_request.pack(bref); }));
  }
  if (cg_sdt_query_ind_present) {
    HANDLE_CODE(pack_ie(bref, 586, crit_e{crit_e::ignore}, [this, &bref]() { return cg_sdt_query_ind.pack(bref); }));
  }
  if (five_g_pro_se_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 594, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_authorized.pack(bref); }));
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(bref, 595, crit_e{crit_e::ignore}, [this, &bref]() {
      return five_g_pro_se_ue_pc5_aggr_max_bitrate.pack(bref);
    }));
  }
  if (five_g_pro_se_pc5_link_ambr_present) {
    HANDLE_CODE(pack_ie(bref, 596, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, five_g_pro_se_pc5_link_ambr, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (upd_remote_ue_local_id_present) {
    HANDLE_CODE(pack_ie(bref, 618, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, upd_remote_ue_local_id, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }
  if (uu_rlc_ch_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 599, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_to_be_setup_list, 1, 32, true);
    }));
  }
  if (uu_rlc_ch_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 600, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_to_be_modified_list, 1, 32, true);
    }));
  }
  if (uu_rlc_ch_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 601, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_to_be_released_list, 1, 32, true);
    }));
  }
  if (pc5_rlc_ch_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 608, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_to_be_setup_list, 1, 512, true);
    }));
  }
  if (pc5_rlc_ch_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 609, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_to_be_modified_list, 1, 512, true);
    }));
  }
  if (pc5_rlc_ch_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 610, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_to_be_released_list, 1, 512, true);
    }));
  }
  if (path_switch_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 619, crit_e{crit_e::ignore}, [this, &bref]() { return path_switch_cfg.pack(bref); }));
  }
  if (gnb_du_ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 626, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, gnb_du_ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }
  if (multicast_mbs_session_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 632, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, multicast_mbs_session_setup_list, 1, 256, true);
    }));
  }
  if (multicast_mbs_session_rem_list_present) {
    HANDLE_CODE(pack_ie(bref, 633, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, multicast_mbs_session_rem_list, 1, 256, true);
    }));
  }
  if (ue_multicast_m_rbs_to_be_setup_at_modify_list_present) {
    HANDLE_CODE(pack_ie(bref, 685, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_multicast_m_rbs_to_be_setup_at_modify_list, 1, 64, true);
    }));
  }
  if (ue_multicast_m_rbs_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 628, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_multicast_m_rbs_to_be_released_list, 1, 64, true);
    }));
  }
  if (sldrx_cycle_list_present) {
    HANDLE_CODE(pack_ie(bref, 643, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sldrx_cycle_list, 1, 32, true);
    }));
  }
  if (management_based_mdt_plmn_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 647, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_mod_list, 0, 16, true);
    }));
  }
  if (sdt_bearer_cfg_query_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 675, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_bearer_cfg_query_ind.pack(bref); }));
  }
  if (daps_ho_status_present) {
    HANDLE_CODE(pack_ie(bref, 683, crit_e{crit_e::ignore}, [this, &bref]() { return daps_ho_status.pack(bref); }));
  }
  if (serving_cell_mo_list_present) {
    HANDLE_CODE(pack_ie(bref, 695, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, serving_cell_mo_list, 1, 16, true);
    }));
  }
  if (ul_tx_direct_current_more_carrier_info_present) {
    HANDLE_CODE(pack_ie(bref, 690, crit_e{crit_e::ignore}, [this, &bref]() {
      return ul_tx_direct_current_more_carrier_info.pack(bref);
    }));
  }
  if (cp_acmcg_info_present) {
    HANDLE_CODE(pack_ie(bref, 691, crit_e{crit_e::ignore}, [this, &bref]() { return cp_acmcg_info.pack(bref); }));
  }
  if (network_ctrl_repeater_authorized_present) {
    HANDLE_CODE(pack_ie(
        bref, 712, crit_e{crit_e::ignore}, [this, &bref]() { return network_ctrl_repeater_authorized.pack(bref); }));
  }
  if (sdt_volume_thres_present) {
    HANDLE_CODE(pack_ie(bref, 716, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sdt_volume_thres, (uint32_t)1u, (uint32_t)192000u, true, true);
    }));
  }
  if (ltm_info_modify_present) {
    HANDLE_CODE(pack_ie(bref, 722, crit_e{crit_e::reject}, [this, &bref]() { return ltm_info_modify.pack(bref); }));
  }
  if (ltmcfra_res_cfg_list_present) {
    HANDLE_CODE(pack_ie(bref, 845, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ltmcfra_res_cfg_list, 1, 8, true);
    }));
  }
  if (ltm_cfg_id_map_list_present) {
    HANDLE_CODE(pack_ie(bref, 721, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ltm_cfg_id_map_list, 1, 8, true);
    }));
  }
  if (early_sync_info_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 726, crit_e{crit_e::ignore}, [this, &bref]() { return early_sync_info_request.pack(bref); }));
  }
  if (early_sync_candidate_cell_info_list_present) {
    HANDLE_CODE(pack_ie(bref, 728, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, early_sync_candidate_cell_info_list, 1, 8, true);
    }));
  }
  if (early_sync_serving_cell_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 843, crit_e{crit_e::ignore}, [this, &bref]() { return early_sync_serving_cell_info.pack(bref); }));
  }
  if (ltm_cells_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 723, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ltm_cells_to_be_released_list, 1, 8, true);
    }));
  }
  if (path_addition_info_present) {
    HANDLE_CODE(pack_ie(bref, 741, crit_e{crit_e::reject}, [this, &bref]() { return path_addition_info.pack(bref); }));
  }
  if (nr_a2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 779, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_services_authorized.pack(bref); }));
  }
  if (ltea2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 780, crit_e{crit_e::ignore}, [this, &bref]() { return ltea2_x_services_authorized.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present) {
    HANDLE_CODE(pack_ie(bref, 781, crit_e{crit_e::ignore}, [this, &bref]() {
      return nr_ue_sidelink_aggr_max_bitrate_for_a2_x.pack(bref);
    }));
  }
  if (lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present) {
    HANDLE_CODE(pack_ie(bref, 782, crit_e{crit_e::ignore}, [this, &bref]() {
      return lte_ue_sidelink_aggr_max_bitrate_for_a2_x.pack(bref);
    }));
  }
  if (dl_lbt_fail_info_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 794, crit_e{crit_e::ignore}, [this, &bref]() { return dl_lbt_fail_info_request.pack(bref); }));
  }
  if (sl_positioning_ranging_service_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 801, crit_e{crit_e::ignore}, [this, &bref]() { return sl_positioning_ranging_service_info.pack(bref); }));
  }
  if (non_integer_drx_cycle_present) {
    HANDLE_CODE(
        pack_ie(bref, 838, crit_e{crit_e::ignore}, [this, &bref]() { return non_integer_drx_cycle.pack(bref); }));
  }
  if (ltm_reset_info_present) {
    HANDLE_CODE(pack_ie(bref, 855, crit_e{crit_e::ignore}, [this, &bref]() { return ltm_reset_info.pack(bref); }));
  }
  if (ltmtci_states_cfgs_list_present) {
    HANDLE_CODE(pack_ie(bref, 863, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ltmtci_states_cfgs_list, 1, 8, true);
    }));
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
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 63: {
        sp_cell_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sp_cell_id.unpack(bref); }));
        break;
      }
      case 107: {
        serv_cell_idx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(serv_cell_idx, bref, (uint8_t)0u, (uint8_t)31u, true, true);
        }));
        break;
      }
      case 96: {
        sp_cell_ul_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sp_cell_ul_cfg.unpack(bref); }));
        break;
      }
      case 38: {
        drx_cycle_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return drx_cycle.unpack(bref); }));
        break;
      }
      case 9: {
        cu_to_du_rrc_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cu_to_du_rrc_info.unpack(bref); }));
        break;
      }
      case 79: {
        tx_action_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return tx_action_ind.unpack(bref); }));
        break;
      }
      case 49: {
        res_coordination_transfer_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_container.unpack(bref); }));
        break;
      }
      case 87: {
        rrc_recfg_complete_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_recfg_complete_ind.unpack(bref); }));
        break;
      }
      case 50: {
        rrc_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_container.unpack(bref); }));
        break;
      }
      case 56: {
        scell_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(scell_to_be_setup_mod_list, bref, 1, 32, true); }));
        break;
      }
      case 52: {
        scell_to_be_remd_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(scell_to_be_remd_list, bref, 1, 32, true); }));
        break;
      }
      case 76: {
        srbs_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_to_be_setup_mod_list, bref, 1, 8, true); }));
        break;
      }
      case 37: {
        drbs_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_to_be_setup_mod_list, bref, 1, 64, true); }));
        break;
      }
      case 31: {
        drbs_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_to_be_modified_list, bref, 1, 64, true); }));
        break;
      }
      case 72: {
        srbs_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_to_be_released_list, bref, 1, 8, true); }));
        break;
      }
      case 33: {
        drbs_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_to_be_released_list, bref, 1, 64, true); }));
        break;
      }
      case 97: {
        inactivity_monitoring_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return inactivity_monitoring_request.unpack(bref); }));
        break;
      }
      case 108: {
        rat_freq_prio_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rat_freq_prio_info.unpack(bref); }));
        break;
      }
      case 159: {
        drx_cfg_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return drx_cfg_ind.unpack(bref); }));
        break;
      }
      case 174: {
        rlc_fail_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rlc_fail_ind.unpack(bref); }));
        break;
      }
      case 175: {
        ul_tx_direct_current_list_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ul_tx_direct_current_list_info.unpack(bref); }));
        break;
      }
      case 162: {
        gnb_du_cfg_query_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return gnb_du_cfg_query.unpack(bref); }));
        break;
      }
      case 158: {
        gnb_du_ue_ambr_ul_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_ambr_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 109: {
        execute_dupl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return execute_dupl.unpack(bref); }));
        break;
      }
      case 184: {
        rrc_delivery_status_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_delivery_status_request.unpack(bref); }));
        break;
      }
      case 195: {
        res_coordination_transfer_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_info.unpack(bref); }));
        break;
      }
      case 182: {
        serving_cell_mo_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(serving_cell_mo, bref, (uint16_t)1u, (uint16_t)64u, true, true);
        }));
        break;
      }
      case 215: {
        needfor_gap_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return needfor_gap.unpack(bref); }));
        break;
      }
      case 94: {
        full_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return full_cfg.unpack(bref); }));
        break;
      }
      case 248: {
        add_rrm_prio_idx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return add_rrm_prio_idx.unpack(bref); }));
        break;
      }
      case 253: {
        lower_layer_presence_status_change_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lower_layer_presence_status_change.unpack(bref); }));
        break;
      }
      case 267: {
        bh_chs_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bh_chs_to_be_setup_mod_list, bref, 1, 65536, true); }));
        break;
      }
      case 263: {
        bh_chs_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bh_chs_to_be_modified_list, bref, 1, 65536, true); }));
        break;
      }
      case 265: {
        bh_chs_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bh_chs_to_be_released_list, bref, 1, 65536, true); }));
        break;
      }
      case 306: {
        nr_v2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_v2x_services_authorized.unpack(bref); }));
        break;
      }
      case 307: {
        ltev2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltev2x_services_authorized.unpack(bref); }));
        break;
      }
      case 308: {
        nr_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 309: {
        lte_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 340: {
        pc5_link_ambr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(pc5_link_ambr, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 332: {
        sl_drbs_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_to_be_setup_mod_list, bref, 1, 512, true); }));
        break;
      }
      case 326: {
        sl_drbs_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_to_be_modified_list, bref, 1, 512, true); }));
        break;
      }
      case 328: {
        sl_drbs_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_to_be_released_list, bref, 1, 512, true); }));
        break;
      }
      case 374: {
        conditional_intra_du_mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return conditional_intra_du_mob_info.unpack(bref); }));
        break;
      }
      case 428: {
        f1_c_transfer_path_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return f1_c_transfer_path.unpack(bref); }));
        break;
      }
      case 432: {
        scg_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_ind.unpack(bref); }));
        break;
      }
      case 684: {
        ul_tx_direct_current_two_carrier_list_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return ul_tx_direct_current_two_carrier_list_info.unpack(bref); }));
        break;
      }
      case 510: {
        iab_conditional_rrc_msg_delivery_ind_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return iab_conditional_rrc_msg_delivery_ind.unpack(bref); }));
        break;
      }
      case 511: {
        f1_c_transfer_path_nr_dc_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return f1_c_transfer_path_nr_dc.unpack(bref); }));
        break;
      }
      case 536: {
        mdt_polluted_meas_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mdt_polluted_meas_ind.unpack(bref); }));
        break;
      }
      case 547: {
        scg_activation_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_request.unpack(bref); }));
        break;
      }
      case 586: {
        cg_sdt_query_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cg_sdt_query_ind.unpack(bref); }));
        break;
      }
      case 594: {
        five_g_pro_se_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_authorized.unpack(bref); }));
        break;
      }
      case 595: {
        five_g_pro_se_ue_pc5_aggr_max_bitrate_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_ue_pc5_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 596: {
        five_g_pro_se_pc5_link_ambr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(five_g_pro_se_pc5_link_ambr, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 618: {
        upd_remote_ue_local_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(upd_remote_ue_local_id, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 599: {
        uu_rlc_ch_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_to_be_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 600: {
        uu_rlc_ch_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_to_be_modified_list, bref, 1, 32, true); }));
        break;
      }
      case 601: {
        uu_rlc_ch_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_to_be_released_list, bref, 1, 32, true); }));
        break;
      }
      case 608: {
        pc5_rlc_ch_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pc5_rlc_ch_to_be_setup_list, bref, 1, 512, true); }));
        break;
      }
      case 609: {
        pc5_rlc_ch_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pc5_rlc_ch_to_be_modified_list, bref, 1, 512, true); }));
        break;
      }
      case 610: {
        pc5_rlc_ch_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pc5_rlc_ch_to_be_released_list, bref, 1, 512, true); }));
        break;
      }
      case 619: {
        path_switch_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return path_switch_cfg.unpack(bref); }));
        break;
      }
      case 626: {
        gnb_du_ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(gnb_du_ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      case 632: {
        multicast_mbs_session_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(multicast_mbs_session_setup_list, bref, 1, 256, true); }));
        break;
      }
      case 633: {
        multicast_mbs_session_rem_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(multicast_mbs_session_rem_list, bref, 1, 256, true); }));
        break;
      }
      case 685: {
        ue_multicast_m_rbs_to_be_setup_at_modify_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ue_multicast_m_rbs_to_be_setup_at_modify_list, bref, 1, 64, true);
        }));
        break;
      }
      case 628: {
        ue_multicast_m_rbs_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ue_multicast_m_rbs_to_be_released_list, bref, 1, 64, true);
        }));
        break;
      }
      case 643: {
        sldrx_cycle_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(sldrx_cycle_list, bref, 1, 32, true); }));
        break;
      }
      case 647: {
        management_based_mdt_plmn_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(management_based_mdt_plmn_mod_list, bref, 0, 16, true);
        }));
        break;
      }
      case 675: {
        sdt_bearer_cfg_query_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_bearer_cfg_query_ind.unpack(bref); }));
        break;
      }
      case 683: {
        daps_ho_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return daps_ho_status.unpack(bref); }));
        break;
      }
      case 695: {
        serving_cell_mo_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(serving_cell_mo_list, bref, 1, 16, true); }));
        break;
      }
      case 690: {
        ul_tx_direct_current_more_carrier_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return ul_tx_direct_current_more_carrier_info.unpack(bref); }));
        break;
      }
      case 691: {
        cp_acmcg_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cp_acmcg_info.unpack(bref); }));
        break;
      }
      case 712: {
        network_ctrl_repeater_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return network_ctrl_repeater_authorized.unpack(bref); }));
        break;
      }
      case 716: {
        sdt_volume_thres_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sdt_volume_thres, bref, (uint32_t)1u, (uint32_t)192000u, true, true);
        }));
        break;
      }
      case 722: {
        ltm_info_modify_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltm_info_modify.unpack(bref); }));
        break;
      }
      case 845: {
        ltmcfra_res_cfg_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ltmcfra_res_cfg_list, bref, 1, 8, true); }));
        break;
      }
      case 721: {
        ltm_cfg_id_map_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ltm_cfg_id_map_list, bref, 1, 8, true); }));
        break;
      }
      case 726: {
        early_sync_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return early_sync_info_request.unpack(bref); }));
        break;
      }
      case 728: {
        early_sync_candidate_cell_info_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(early_sync_candidate_cell_info_list, bref, 1, 8, true);
        }));
        break;
      }
      case 843: {
        early_sync_serving_cell_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return early_sync_serving_cell_info.unpack(bref); }));
        break;
      }
      case 723: {
        ltm_cells_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ltm_cells_to_be_released_list, bref, 1, 8, true); }));
        break;
      }
      case 741: {
        path_addition_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return path_addition_info.unpack(bref); }));
        break;
      }
      case 779: {
        nr_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 780: {
        ltea2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltea2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 781: {
        nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate_for_a2_x.unpack(bref); }));
        break;
      }
      case 782: {
        lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate_for_a2_x.unpack(bref); }));
        break;
      }
      case 794: {
        dl_lbt_fail_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return dl_lbt_fail_info_request.unpack(bref); }));
        break;
      }
      case 801: {
        sl_positioning_ranging_service_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sl_positioning_ranging_service_info.unpack(bref); }));
        break;
      }
      case 838: {
        non_integer_drx_cycle_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return non_integer_drx_cycle.unpack(bref); }));
        break;
      }
      case 855: {
        ltm_reset_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltm_reset_info.unpack(bref); }));
        break;
      }
      case 863: {
        ltmtci_states_cfgs_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ltmtci_states_cfgs_list, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  if (sp_cell_id_present) {
    j.start_obj();
    j.write_int("id", 63);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sp_cell_id.to_json(j);
    j.end_obj();
  }
  if (serv_cell_idx_present) {
    j.start_obj();
    j.write_int("id", 107);
    j.write_str("criticality", "reject");
    j.write_int("value", serv_cell_idx);
    j.end_obj();
  }
  if (sp_cell_ul_cfg_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "ignore");
    j.write_str("value", sp_cell_ul_cfg.to_string());
    j.end_obj();
  }
  if (drx_cycle_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    drx_cycle.to_json(j);
    j.end_obj();
  }
  if (cu_to_du_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 9);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    cu_to_du_rrc_info.to_json(j);
    j.end_obj();
  }
  if (tx_action_ind_present) {
    j.start_obj();
    j.write_int("id", 79);
    j.write_str("criticality", "ignore");
    j.write_str("value", tx_action_ind.to_string());
    j.end_obj();
  }
  if (res_coordination_transfer_container_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_str("value", res_coordination_transfer_container.to_string());
    j.end_obj();
  }
  if (rrc_recfg_complete_ind_present) {
    j.start_obj();
    j.write_int("id", 87);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_recfg_complete_ind.to_string());
    j.end_obj();
  }
  if (rrc_container_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "reject");
    j.write_str("value", rrc_container.to_string());
    j.end_obj();
  }
  if (scell_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 56);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : scell_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (scell_to_be_remd_list_present) {
    j.start_obj();
    j.write_int("id", 52);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : scell_to_be_remd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srbs_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 76);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : srbs_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 37);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drbs_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drbs_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srbs_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : srbs_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 33);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drbs_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (inactivity_monitoring_request_present) {
    j.start_obj();
    j.write_int("id", 97);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (rat_freq_prio_info_present) {
    j.start_obj();
    j.write_int("id", 108);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    rat_freq_prio_info.to_json(j);
    j.end_obj();
  }
  if (drx_cfg_ind_present) {
    j.start_obj();
    j.write_int("id", 159);
    j.write_str("criticality", "ignore");
    j.write_str("value", "release");
    j.end_obj();
  }
  if (rlc_fail_ind_present) {
    j.start_obj();
    j.write_int("id", 174);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    rlc_fail_ind.to_json(j);
    j.end_obj();
  }
  if (ul_tx_direct_current_list_info_present) {
    j.start_obj();
    j.write_int("id", 175);
    j.write_str("criticality", "ignore");
    j.write_str("value", ul_tx_direct_current_list_info.to_string());
    j.end_obj();
  }
  if (gnb_du_cfg_query_present) {
    j.start_obj();
    j.write_int("id", 162);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (gnb_du_ue_ambr_ul_present) {
    j.start_obj();
    j.write_int("id", 158);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_ue_ambr_ul);
    j.end_obj();
  }
  if (execute_dupl_present) {
    j.start_obj();
    j.write_int("id", 109);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (rrc_delivery_status_request_present) {
    j.start_obj();
    j.write_int("id", 184);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (res_coordination_transfer_info_present) {
    j.start_obj();
    j.write_int("id", 195);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    res_coordination_transfer_info.to_json(j);
    j.end_obj();
  }
  if (serving_cell_mo_present) {
    j.start_obj();
    j.write_int("id", 182);
    j.write_str("criticality", "ignore");
    j.write_int("value", serving_cell_mo);
    j.end_obj();
  }
  if (needfor_gap_present) {
    j.start_obj();
    j.write_int("id", 215);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (full_cfg_present) {
    j.start_obj();
    j.write_int("id", 94);
    j.write_str("criticality", "reject");
    j.write_str("value", "full");
    j.end_obj();
  }
  if (add_rrm_prio_idx_present) {
    j.start_obj();
    j.write_int("id", 248);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    add_rrm_prio_idx.to_json(j);
    j.end_obj();
  }
  if (lower_layer_presence_status_change_present) {
    j.start_obj();
    j.write_int("id", 253);
    j.write_str("criticality", "ignore");
    j.write_str("value", lower_layer_presence_status_change.to_string());
    j.end_obj();
  }
  if (bh_chs_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 267);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : bh_chs_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (bh_chs_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 263);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : bh_chs_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (bh_chs_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 265);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : bh_chs_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (nr_v2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 306);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_v2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltev2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 307);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltev2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 308);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 309);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (pc5_link_ambr_present) {
    j.start_obj();
    j.write_int("id", 340);
    j.write_str("criticality", "ignore");
    j.write_uint("value", pc5_link_ambr);
    j.end_obj();
  }
  if (sl_drbs_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 332);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 326);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 328);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (conditional_intra_du_mob_info_present) {
    j.start_obj();
    j.write_int("id", 374);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    conditional_intra_du_mob_info.to_json(j);
    j.end_obj();
  }
  if (f1_c_transfer_path_present) {
    j.start_obj();
    j.write_int("id", 428);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    f1_c_transfer_path.to_json(j);
    j.end_obj();
  }
  if (scg_ind_present) {
    j.start_obj();
    j.write_int("id", 432);
    j.write_str("criticality", "ignore");
    j.write_str("value", "released");
    j.end_obj();
  }
  if (ul_tx_direct_current_two_carrier_list_info_present) {
    j.start_obj();
    j.write_int("id", 684);
    j.write_str("criticality", "ignore");
    j.write_str("value", ul_tx_direct_current_two_carrier_list_info.to_string());
    j.end_obj();
  }
  if (iab_conditional_rrc_msg_delivery_ind_present) {
    j.start_obj();
    j.write_int("id", 510);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (f1_c_transfer_path_nr_dc_present) {
    j.start_obj();
    j.write_int("id", 511);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    f1_c_transfer_path_nr_dc.to_json(j);
    j.end_obj();
  }
  if (mdt_polluted_meas_ind_present) {
    j.start_obj();
    j.write_int("id", 536);
    j.write_str("criticality", "ignore");
    j.write_str("value", mdt_polluted_meas_ind.to_string());
    j.end_obj();
  }
  if (scg_activation_request_present) {
    j.start_obj();
    j.write_int("id", 547);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_request.to_string());
    j.end_obj();
  }
  if (cg_sdt_query_ind_present) {
    j.start_obj();
    j.write_int("id", 586);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (five_g_pro_se_authorized_present) {
    j.start_obj();
    j.write_int("id", 594);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_authorized.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 595);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_ue_pc5_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_pc5_link_ambr_present) {
    j.start_obj();
    j.write_int("id", 596);
    j.write_str("criticality", "ignore");
    j.write_uint("value", five_g_pro_se_pc5_link_ambr);
    j.end_obj();
  }
  if (upd_remote_ue_local_id_present) {
    j.start_obj();
    j.write_int("id", 618);
    j.write_str("criticality", "ignore");
    j.write_int("value", upd_remote_ue_local_id);
    j.end_obj();
  }
  if (uu_rlc_ch_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 599);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 600);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 601);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 608);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 609);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 610);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (path_switch_cfg_present) {
    j.start_obj();
    j.write_int("id", 619);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    path_switch_cfg.to_json(j);
    j.end_obj();
  }
  if (gnb_du_ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 626);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_du_ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_mbs_session_setup_list_present) {
    j.start_obj();
    j.write_int("id", 632);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : multicast_mbs_session_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_mbs_session_rem_list_present) {
    j.start_obj();
    j.write_int("id", 633);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : multicast_mbs_session_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_multicast_m_rbs_to_be_setup_at_modify_list_present) {
    j.start_obj();
    j.write_int("id", 685);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ue_multicast_m_rbs_to_be_setup_at_modify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_multicast_m_rbs_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 628);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ue_multicast_m_rbs_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sldrx_cycle_list_present) {
    j.start_obj();
    j.write_int("id", 643);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sldrx_cycle_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (management_based_mdt_plmn_mod_list_present) {
    j.start_obj();
    j.write_int("id", 647);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_mod_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (sdt_bearer_cfg_query_ind_present) {
    j.start_obj();
    j.write_int("id", 675);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (daps_ho_status_present) {
    j.start_obj();
    j.write_int("id", 683);
    j.write_str("criticality", "ignore");
    j.write_str("value", "initiation");
    j.end_obj();
  }
  if (serving_cell_mo_list_present) {
    j.start_obj();
    j.write_int("id", 695);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : serving_cell_mo_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ul_tx_direct_current_more_carrier_info_present) {
    j.start_obj();
    j.write_int("id", 690);
    j.write_str("criticality", "ignore");
    j.write_str("value", ul_tx_direct_current_more_carrier_info.to_string());
    j.end_obj();
  }
  if (cp_acmcg_info_present) {
    j.start_obj();
    j.write_int("id", 691);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cp_acmcg_info.to_json(j);
    j.end_obj();
  }
  if (network_ctrl_repeater_authorized_present) {
    j.start_obj();
    j.write_int("id", 712);
    j.write_str("criticality", "ignore");
    j.write_str("value", network_ctrl_repeater_authorized.to_string());
    j.end_obj();
  }
  if (sdt_volume_thres_present) {
    j.start_obj();
    j.write_int("id", 716);
    j.write_str("criticality", "ignore");
    j.write_int("value", sdt_volume_thres);
    j.end_obj();
  }
  if (ltm_info_modify_present) {
    j.start_obj();
    j.write_int("id", 722);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ltm_info_modify.to_json(j);
    j.end_obj();
  }
  if (ltmcfra_res_cfg_list_present) {
    j.start_obj();
    j.write_int("id", 845);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ltmcfra_res_cfg_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ltm_cfg_id_map_list_present) {
    j.start_obj();
    j.write_int("id", 721);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ltm_cfg_id_map_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (early_sync_info_request_present) {
    j.start_obj();
    j.write_int("id", 726);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    early_sync_info_request.to_json(j);
    j.end_obj();
  }
  if (early_sync_candidate_cell_info_list_present) {
    j.start_obj();
    j.write_int("id", 728);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : early_sync_candidate_cell_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (early_sync_serving_cell_info_present) {
    j.start_obj();
    j.write_int("id", 843);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    early_sync_serving_cell_info.to_json(j);
    j.end_obj();
  }
  if (ltm_cells_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 723);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ltm_cells_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (path_addition_info_present) {
    j.start_obj();
    j.write_int("id", 741);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    path_addition_info.to_json(j);
    j.end_obj();
  }
  if (nr_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 779);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltea2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 780);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltea2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present) {
    j.start_obj();
    j.write_int("id", 781);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_ue_sidelink_aggr_max_bitrate_for_a2_x.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present) {
    j.start_obj();
    j.write_int("id", 782);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_ue_sidelink_aggr_max_bitrate_for_a2_x.to_json(j);
    j.end_obj();
  }
  if (dl_lbt_fail_info_request_present) {
    j.start_obj();
    j.write_int("id", 794);
    j.write_str("criticality", "ignore");
    j.write_str("value", "inquiry");
    j.end_obj();
  }
  if (sl_positioning_ranging_service_info_present) {
    j.start_obj();
    j.write_int("id", 801);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sl_positioning_ranging_service_info.to_json(j);
    j.end_obj();
  }
  if (non_integer_drx_cycle_present) {
    j.start_obj();
    j.write_int("id", 838);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    non_integer_drx_cycle.to_json(j);
    j.end_obj();
  }
  if (ltm_reset_info_present) {
    j.start_obj();
    j.write_int("id", 855);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltm_reset_info.to_json(j);
    j.end_obj();
  }
  if (ltmtci_states_cfgs_list_present) {
    j.start_obj();
    j.write_int("id", 863);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ltmtci_states_cfgs_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// UEContextModificationRequiredIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_mod_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {
      40, 41, 49, 39, 23, 70, 25, 0, 277, 320, 322, 375, 606, 607, 614, 615, 655, 657, 723};
  return map_enum_number(names, 19, idx, "id");
}
bool ue_context_mod_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {
      40, 41, 49, 39, 23, 70, 25, 0, 277, 320, 322, 375, 606, 607, 614, 615, 655, 657, 723};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_mod_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 49:
      return crit_e::ignore;
    case 39:
      return crit_e::reject;
    case 23:
      return crit_e::reject;
    case 70:
      return crit_e::reject;
    case 25:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 277:
      return crit_e::reject;
    case 320:
      return crit_e::reject;
    case 322:
      return crit_e::reject;
    case 375:
      return crit_e::reject;
    case 606:
      return crit_e::reject;
    case 607:
      return crit_e::reject;
    case 614:
      return crit_e::reject;
    case 615:
      return crit_e::reject;
    case 655:
      return crit_e::reject;
    case 657:
      return crit_e::reject;
    case 723:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_mod_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 49:
      return presence_e::optional;
    case 39:
      return presence_e::optional;
    case 23:
      return presence_e::optional;
    case 70:
      return presence_e::optional;
    case 25:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 277:
      return presence_e::optional;
    case 320:
      return presence_e::optional;
    case 322:
      return presence_e::optional;
    case 375:
      return presence_e::optional;
    case 606:
      return presence_e::optional;
    case 607:
      return presence_e::optional;
    case 614:
      return presence_e::optional;
    case 615:
      return presence_e::optional;
    case 655:
      return presence_e::optional;
    case 657:
      return presence_e::optional;
    case 723:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_mod_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += res_coordination_transfer_container_present ? 1 : 0;
  nof_ies += du_to_cu_rrc_info_present ? 1 : 0;
  nof_ies += drbs_required_to_be_modified_list_present ? 1 : 0;
  nof_ies += srbs_required_to_be_released_list_present ? 1 : 0;
  nof_ies += drbs_required_to_be_released_list_present ? 1 : 0;
  nof_ies += bh_chs_required_to_be_released_list_present ? 1 : 0;
  nof_ies += sl_drbs_required_to_be_modified_list_present ? 1 : 0;
  nof_ies += sl_drbs_required_to_be_released_list_present ? 1 : 0;
  nof_ies += target_cells_to_cancel_present ? 1 : 0;
  nof_ies += uu_rlc_ch_required_to_be_modified_list_present ? 1 : 0;
  nof_ies += uu_rlc_ch_required_to_be_released_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_required_to_be_modified_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_required_to_be_released_list_present ? 1 : 0;
  nof_ies += ue_multicast_m_rbs_required_to_be_modified_list_present ? 1 : 0;
  nof_ies += ue_multicast_m_rbs_required_to_be_released_list_present ? 1 : 0;
  nof_ies += ltm_cells_to_be_released_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (res_coordination_transfer_container_present) {
    HANDLE_CODE(pack_ie(
        bref, 49, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_container.pack(bref); }));
  }
  if (du_to_cu_rrc_info_present) {
    HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() { return du_to_cu_rrc_info.pack(bref); }));
  }
  if (drbs_required_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_required_to_be_modified_list, 1, 64, true);
    }));
  }
  if (srbs_required_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 70, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_required_to_be_released_list, 1, 8, true);
    }));
  }
  if (drbs_required_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 25, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_required_to_be_released_list, 1, 64, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (bh_chs_required_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 277, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_required_to_be_released_list, 1, 65536, true);
    }));
  }
  if (sl_drbs_required_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 320, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_required_to_be_modified_list, 1, 512, true);
    }));
  }
  if (sl_drbs_required_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 322, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_required_to_be_released_list, 1, 512, true);
    }));
  }
  if (target_cells_to_cancel_present) {
    HANDLE_CODE(pack_ie(bref, 375, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, target_cells_to_cancel, 1, 8, true);
    }));
  }
  if (uu_rlc_ch_required_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 606, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_required_to_be_modified_list, 1, 32, true);
    }));
  }
  if (uu_rlc_ch_required_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 607, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_required_to_be_released_list, 1, 32, true);
    }));
  }
  if (pc5_rlc_ch_required_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 614, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_required_to_be_modified_list, 1, 512, true);
    }));
  }
  if (pc5_rlc_ch_required_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 615, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_required_to_be_released_list, 1, 512, true);
    }));
  }
  if (ue_multicast_m_rbs_required_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 655, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_multicast_m_rbs_required_to_be_modified_list, 1, 64, true);
    }));
  }
  if (ue_multicast_m_rbs_required_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 657, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_multicast_m_rbs_required_to_be_released_list, 1, 64, true);
    }));
  }
  if (ltm_cells_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 723, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ltm_cells_to_be_released_list, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_mod_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_mod_required_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 49: {
        res_coordination_transfer_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_container.unpack(bref); }));
        break;
      }
      case 39: {
        du_to_cu_rrc_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return du_to_cu_rrc_info.unpack(bref); }));
        break;
      }
      case 23: {
        drbs_required_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_required_to_be_modified_list, bref, 1, 64, true); }));
        break;
      }
      case 70: {
        srbs_required_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_required_to_be_released_list, bref, 1, 8, true); }));
        break;
      }
      case 25: {
        drbs_required_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_required_to_be_released_list, bref, 1, 64, true); }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 277: {
        bh_chs_required_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(bh_chs_required_to_be_released_list, bref, 1, 65536, true);
        }));
        break;
      }
      case 320: {
        sl_drbs_required_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(sl_drbs_required_to_be_modified_list, bref, 1, 512, true);
        }));
        break;
      }
      case 322: {
        sl_drbs_required_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(sl_drbs_required_to_be_released_list, bref, 1, 512, true);
        }));
        break;
      }
      case 375: {
        target_cells_to_cancel_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(target_cells_to_cancel, bref, 1, 8, true); }));
        break;
      }
      case 606: {
        uu_rlc_ch_required_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(uu_rlc_ch_required_to_be_modified_list, bref, 1, 32, true);
        }));
        break;
      }
      case 607: {
        uu_rlc_ch_required_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(uu_rlc_ch_required_to_be_released_list, bref, 1, 32, true);
        }));
        break;
      }
      case 614: {
        pc5_rlc_ch_required_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pc5_rlc_ch_required_to_be_modified_list, bref, 1, 512, true);
        }));
        break;
      }
      case 615: {
        pc5_rlc_ch_required_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pc5_rlc_ch_required_to_be_released_list, bref, 1, 512, true);
        }));
        break;
      }
      case 655: {
        ue_multicast_m_rbs_required_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ue_multicast_m_rbs_required_to_be_modified_list, bref, 1, 64, true);
        }));
        break;
      }
      case 657: {
        ue_multicast_m_rbs_required_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ue_multicast_m_rbs_required_to_be_released_list, bref, 1, 64, true);
        }));
        break;
      }
      case 723: {
        ltm_cells_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ltm_cells_to_be_released_list, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_mod_required_ies_container::to_json(json_writer& j) const
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
  if (res_coordination_transfer_container_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_str("value", res_coordination_transfer_container.to_string());
    j.end_obj();
  }
  if (du_to_cu_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 39);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    du_to_cu_rrc_info.to_json(j);
    j.end_obj();
  }
  if (drbs_required_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 23);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drbs_required_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srbs_required_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 70);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : srbs_required_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_required_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 25);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drbs_required_to_be_released_list) {
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
  if (bh_chs_required_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 277);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : bh_chs_required_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_required_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 320);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_required_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_required_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 322);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_required_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (target_cells_to_cancel_present) {
    j.start_obj();
    j.write_int("id", 375);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : target_cells_to_cancel) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_required_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 606);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_required_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_required_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 607);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_required_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_required_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 614);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_required_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_required_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 615);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_required_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_multicast_m_rbs_required_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 655);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ue_multicast_m_rbs_required_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_multicast_m_rbs_required_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 657);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ue_multicast_m_rbs_required_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ltm_cells_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 723);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ltm_cells_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// UEContextModificationResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40,  41,  49,  39,  29,  21,  68,  17,  85,  13,  98,  7,   95,  198, 204,
                                   206, 94,  275, 273, 271, 269, 333, 318, 334, 314, 376, 548, 602, 603, 604,
                                   605, 611, 612, 616, 613, 676, 679, 697, 708, 709, 727, 725, 792};
  return map_enum_number(names, 43, idx, "id");
}
bool ue_context_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40,  41,  49,  39,  29,  21,  68,  17,  85,  13,  98,  7,   95,  198, 204,
                                   206, 94,  275, 273, 271, 269, 333, 318, 334, 314, 376, 548, 602, 603, 604,
                                   605, 611, 612, 616, 613, 676, 679, 697, 708, 709, 727, 725, 792};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 49:
      return crit_e::ignore;
    case 39:
      return crit_e::reject;
    case 29:
      return crit_e::ignore;
    case 21:
      return crit_e::ignore;
    case 68:
      return crit_e::ignore;
    case 17:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 13:
      return crit_e::ignore;
    case 98:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 95:
      return crit_e::ignore;
    case 198:
      return crit_e::ignore;
    case 204:
      return crit_e::ignore;
    case 206:
      return crit_e::ignore;
    case 94:
      return crit_e::reject;
    case 275:
      return crit_e::ignore;
    case 273:
      return crit_e::ignore;
    case 271:
      return crit_e::ignore;
    case 269:
      return crit_e::ignore;
    case 333:
      return crit_e::ignore;
    case 318:
      return crit_e::ignore;
    case 334:
      return crit_e::ignore;
    case 314:
      return crit_e::ignore;
    case 376:
      return crit_e::reject;
    case 548:
      return crit_e::ignore;
    case 602:
      return crit_e::ignore;
    case 603:
      return crit_e::ignore;
    case 604:
      return crit_e::ignore;
    case 605:
      return crit_e::ignore;
    case 611:
      return crit_e::ignore;
    case 612:
      return crit_e::ignore;
    case 616:
      return crit_e::ignore;
    case 613:
      return crit_e::ignore;
    case 676:
      return crit_e::ignore;
    case 679:
      return crit_e::reject;
    case 697:
      return crit_e::ignore;
    case 708:
      return crit_e::ignore;
    case 709:
      return crit_e::ignore;
    case 727:
      return crit_e::ignore;
    case 725:
      return crit_e::ignore;
    case 792:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 49:
      return presence_e::optional;
    case 39:
      return presence_e::optional;
    case 29:
      return presence_e::optional;
    case 21:
      return presence_e::optional;
    case 68:
      return presence_e::optional;
    case 17:
      return presence_e::optional;
    case 85:
      return presence_e::optional;
    case 13:
      return presence_e::optional;
    case 98:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 95:
      return presence_e::optional;
    case 198:
      return presence_e::optional;
    case 204:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 94:
      return presence_e::optional;
    case 275:
      return presence_e::optional;
    case 273:
      return presence_e::optional;
    case 271:
      return presence_e::optional;
    case 269:
      return presence_e::optional;
    case 333:
      return presence_e::optional;
    case 318:
      return presence_e::optional;
    case 334:
      return presence_e::optional;
    case 314:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 548:
      return presence_e::optional;
    case 602:
      return presence_e::optional;
    case 603:
      return presence_e::optional;
    case 604:
      return presence_e::optional;
    case 605:
      return presence_e::optional;
    case 611:
      return presence_e::optional;
    case 612:
      return presence_e::optional;
    case 616:
      return presence_e::optional;
    case 613:
      return presence_e::optional;
    case 676:
      return presence_e::optional;
    case 679:
      return presence_e::optional;
    case 697:
      return presence_e::optional;
    case 708:
      return presence_e::optional;
    case 709:
      return presence_e::optional;
    case 727:
      return presence_e::optional;
    case 725:
      return presence_e::optional;
    case 792:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += res_coordination_transfer_container_present ? 1 : 0;
  nof_ies += du_to_cu_rrc_info_present ? 1 : 0;
  nof_ies += drbs_setup_mod_list_present ? 1 : 0;
  nof_ies += drbs_modified_list_present ? 1 : 0;
  nof_ies += srbs_failed_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += drbs_failed_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += scell_failedto_setup_mod_list_present ? 1 : 0;
  nof_ies += drbs_failed_to_be_modified_list_present ? 1 : 0;
  nof_ies += inactivity_monitoring_resp_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += c_rnti_present ? 1 : 0;
  nof_ies += associated_scell_list_present ? 1 : 0;
  nof_ies += srbs_setup_mod_list_present ? 1 : 0;
  nof_ies += srbs_modified_list_present ? 1 : 0;
  nof_ies += full_cfg_present ? 1 : 0;
  nof_ies += bh_chs_setup_mod_list_present ? 1 : 0;
  nof_ies += bh_chs_modified_list_present ? 1 : 0;
  nof_ies += bh_chs_failed_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += bh_chs_failed_to_be_modified_list_present ? 1 : 0;
  nof_ies += sl_drbs_setup_mod_list_present ? 1 : 0;
  nof_ies += sl_drbs_modified_list_present ? 1 : 0;
  nof_ies += sl_drbs_failed_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += sl_drbs_failed_to_be_modified_list_present ? 1 : 0;
  nof_ies += requested_target_cell_global_id_present ? 1 : 0;
  nof_ies += scg_activation_status_present ? 1 : 0;
  nof_ies += uu_rlc_ch_setup_list_present ? 1 : 0;
  nof_ies += uu_rlc_ch_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += uu_rlc_ch_modified_list_present ? 1 : 0;
  nof_ies += uu_rlc_ch_failed_to_be_modified_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_setup_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_modified_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_failed_to_be_modified_list_present ? 1 : 0;
  nof_ies += sdt_bearer_cfg_info_present ? 1 : 0;
  nof_ies += ue_multicast_m_rbs_setup_list_present ? 1 : 0;
  nof_ies += serving_cell_mo_encoded_in_cgc_list_present ? 1 : 0;
  nof_ies += ded_si_delivery_ind_present ? 1 : 0;
  nof_ies += cfg_bwp_list_present ? 1 : 0;
  nof_ies += early_sync_info_present ? 1 : 0;
  nof_ies += ltm_cfg_present ? 1 : 0;
  nof_ies += s_cp_ac_cfg_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (res_coordination_transfer_container_present) {
    HANDLE_CODE(pack_ie(
        bref, 49, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_container.pack(bref); }));
  }
  if (du_to_cu_rrc_info_present) {
    HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() { return du_to_cu_rrc_info.pack(bref); }));
  }
  if (drbs_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_setup_mod_list, 1, 64, true);
    }));
  }
  if (drbs_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 21, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_modified_list, 1, 64, true);
    }));
  }
  if (srbs_failed_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 68, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_failed_to_be_setup_mod_list, 1, 8, true);
    }));
  }
  if (drbs_failed_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 17, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_failed_to_be_setup_mod_list, 1, 64, true);
    }));
  }
  if (scell_failedto_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, scell_failedto_setup_mod_list, 1, 32, true);
    }));
  }
  if (drbs_failed_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 13, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_failed_to_be_modified_list, 1, 64, true);
    }));
  }
  if (inactivity_monitoring_resp_present) {
    HANDLE_CODE(
        pack_ie(bref, 98, crit_e{crit_e::reject}, [this, &bref]() { return inactivity_monitoring_resp.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (c_rnti_present) {
    HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, c_rnti, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  if (associated_scell_list_present) {
    HANDLE_CODE(pack_ie(bref, 198, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, associated_scell_list, 1, 32, true);
    }));
  }
  if (srbs_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 204, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_setup_mod_list, 1, 8, true);
    }));
  }
  if (srbs_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_modified_list, 1, 8, true);
    }));
  }
  if (full_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 94, crit_e{crit_e::reject}, [this, &bref]() { return full_cfg.pack(bref); }));
  }
  if (bh_chs_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 275, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_setup_mod_list, 1, 65536, true);
    }));
  }
  if (bh_chs_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 273, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_modified_list, 1, 65536, true);
    }));
  }
  if (bh_chs_failed_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 271, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_failed_to_be_setup_mod_list, 1, 65536, true);
    }));
  }
  if (bh_chs_failed_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 269, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_failed_to_be_modified_list, 1, 65536, true);
    }));
  }
  if (sl_drbs_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 333, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_setup_mod_list, 1, 512, true);
    }));
  }
  if (sl_drbs_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 318, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_modified_list, 1, 512, true);
    }));
  }
  if (sl_drbs_failed_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 334, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_failed_to_be_setup_mod_list, 1, 512, true);
    }));
  }
  if (sl_drbs_failed_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 314, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_failed_to_be_modified_list, 1, 512, true);
    }));
  }
  if (requested_target_cell_global_id_present) {
    HANDLE_CODE(pack_ie(
        bref, 376, crit_e{crit_e::reject}, [this, &bref]() { return requested_target_cell_global_id.pack(bref); }));
  }
  if (scg_activation_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 548, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_status.pack(bref); }));
  }
  if (uu_rlc_ch_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 602, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_setup_list, 1, 32, true);
    }));
  }
  if (uu_rlc_ch_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 603, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_failed_to_be_setup_list, 1, 32, true);
    }));
  }
  if (uu_rlc_ch_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 604, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_modified_list, 1, 32, true);
    }));
  }
  if (uu_rlc_ch_failed_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 605, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_failed_to_be_modified_list, 1, 32, true);
    }));
  }
  if (pc5_rlc_ch_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 611, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_setup_list, 1, 512, true);
    }));
  }
  if (pc5_rlc_ch_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 612, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_failed_to_be_setup_list, 1, 512, true);
    }));
  }
  if (pc5_rlc_ch_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 616, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_modified_list, 1, 512, true);
    }));
  }
  if (pc5_rlc_ch_failed_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 613, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_failed_to_be_modified_list, 1, 512, true);
    }));
  }
  if (sdt_bearer_cfg_info_present) {
    HANDLE_CODE(pack_ie(bref, 676, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_bearer_cfg_info.pack(bref); }));
  }
  if (ue_multicast_m_rbs_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 679, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_multicast_m_rbs_setup_list, 1, 64, true);
    }));
  }
  if (serving_cell_mo_encoded_in_cgc_list_present) {
    HANDLE_CODE(pack_ie(bref, 697, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, serving_cell_mo_encoded_in_cgc_list, 1, 8, true);
    }));
  }
  if (ded_si_delivery_ind_present) {
    HANDLE_CODE(pack_ie(bref, 708, crit_e{crit_e::ignore}, [this, &bref]() { return ded_si_delivery_ind.pack(bref); }));
  }
  if (cfg_bwp_list_present) {
    HANDLE_CODE(pack_ie(bref, 709, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, cfg_bwp_list, 1, 8, true);
    }));
  }
  if (early_sync_info_present) {
    HANDLE_CODE(pack_ie(bref, 727, crit_e{crit_e::ignore}, [this, &bref]() { return early_sync_info.pack(bref); }));
  }
  if (ltm_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 725, crit_e{crit_e::ignore}, [this, &bref]() { return ltm_cfg.pack(bref); }));
  }
  if (s_cp_ac_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 792, crit_e{crit_e::ignore}, [this, &bref]() { return s_cp_ac_cfg.pack(bref); }));
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
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 49: {
        res_coordination_transfer_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_container.unpack(bref); }));
        break;
      }
      case 39: {
        du_to_cu_rrc_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return du_to_cu_rrc_info.unpack(bref); }));
        break;
      }
      case 29: {
        drbs_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_setup_mod_list, bref, 1, 64, true); }));
        break;
      }
      case 21: {
        drbs_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_modified_list, bref, 1, 64, true); }));
        break;
      }
      case 68: {
        srbs_failed_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_failed_to_be_setup_mod_list, bref, 1, 8, true); }));
        break;
      }
      case 17: {
        drbs_failed_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_failed_to_be_setup_mod_list, bref, 1, 64, true); }));
        break;
      }
      case 85: {
        scell_failedto_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(scell_failedto_setup_mod_list, bref, 1, 32, true); }));
        break;
      }
      case 13: {
        drbs_failed_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_failed_to_be_modified_list, bref, 1, 64, true); }));
        break;
      }
      case 98: {
        inactivity_monitoring_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return inactivity_monitoring_resp.unpack(bref); }));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 95: {
        c_rnti_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(c_rnti, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 198: {
        associated_scell_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(associated_scell_list, bref, 1, 32, true); }));
        break;
      }
      case 204: {
        srbs_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_setup_mod_list, bref, 1, 8, true); }));
        break;
      }
      case 206: {
        srbs_modified_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_modified_list, bref, 1, 8, true); }));
        break;
      }
      case 94: {
        full_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return full_cfg.unpack(bref); }));
        break;
      }
      case 275: {
        bh_chs_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bh_chs_setup_mod_list, bref, 1, 65536, true); }));
        break;
      }
      case 273: {
        bh_chs_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bh_chs_modified_list, bref, 1, 65536, true); }));
        break;
      }
      case 271: {
        bh_chs_failed_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(bh_chs_failed_to_be_setup_mod_list, bref, 1, 65536, true);
        }));
        break;
      }
      case 269: {
        bh_chs_failed_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(bh_chs_failed_to_be_modified_list, bref, 1, 65536, true);
        }));
        break;
      }
      case 333: {
        sl_drbs_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_setup_mod_list, bref, 1, 512, true); }));
        break;
      }
      case 318: {
        sl_drbs_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_modified_list, bref, 1, 512, true); }));
        break;
      }
      case 334: {
        sl_drbs_failed_to_be_setup_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(sl_drbs_failed_to_be_setup_mod_list, bref, 1, 512, true);
        }));
        break;
      }
      case 314: {
        sl_drbs_failed_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(sl_drbs_failed_to_be_modified_list, bref, 1, 512, true);
        }));
        break;
      }
      case 376: {
        requested_target_cell_global_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_target_cell_global_id.unpack(bref); }));
        break;
      }
      case 548: {
        scg_activation_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_status.unpack(bref); }));
        break;
      }
      case 602: {
        uu_rlc_ch_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 603: {
        uu_rlc_ch_failed_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_failed_to_be_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 604: {
        uu_rlc_ch_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_modified_list, bref, 1, 32, true); }));
        break;
      }
      case 605: {
        uu_rlc_ch_failed_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(uu_rlc_ch_failed_to_be_modified_list, bref, 1, 32, true);
        }));
        break;
      }
      case 611: {
        pc5_rlc_ch_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pc5_rlc_ch_setup_list, bref, 1, 512, true); }));
        break;
      }
      case 612: {
        pc5_rlc_ch_failed_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pc5_rlc_ch_failed_to_be_setup_list, bref, 1, 512, true);
        }));
        break;
      }
      case 616: {
        pc5_rlc_ch_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pc5_rlc_ch_modified_list, bref, 1, 512, true); }));
        break;
      }
      case 613: {
        pc5_rlc_ch_failed_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pc5_rlc_ch_failed_to_be_modified_list, bref, 1, 512, true);
        }));
        break;
      }
      case 676: {
        sdt_bearer_cfg_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_bearer_cfg_info.unpack(bref); }));
        break;
      }
      case 679: {
        ue_multicast_m_rbs_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_multicast_m_rbs_setup_list, bref, 1, 64, true); }));
        break;
      }
      case 697: {
        serving_cell_mo_encoded_in_cgc_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(serving_cell_mo_encoded_in_cgc_list, bref, 1, 8, true);
        }));
        break;
      }
      case 708: {
        ded_si_delivery_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ded_si_delivery_ind.unpack(bref); }));
        break;
      }
      case 709: {
        cfg_bwp_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(cfg_bwp_list, bref, 1, 8, true); }));
        break;
      }
      case 727: {
        early_sync_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return early_sync_info.unpack(bref); }));
        break;
      }
      case 725: {
        ltm_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltm_cfg.unpack(bref); }));
        break;
      }
      case 792: {
        s_cp_ac_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_cp_ac_cfg.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  if (res_coordination_transfer_container_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_str("value", res_coordination_transfer_container.to_string());
    j.end_obj();
  }
  if (du_to_cu_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 39);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    du_to_cu_rrc_info.to_json(j);
    j.end_obj();
  }
  if (drbs_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 29);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_modified_list_present) {
    j.start_obj();
    j.write_int("id", 21);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srbs_failed_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 68);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : srbs_failed_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_failed_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 17);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_failed_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (scell_failedto_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 85);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : scell_failedto_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_failed_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 13);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_failed_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (inactivity_monitoring_resp_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "reject");
    j.write_str("value", "not-supported");
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
  if (c_rnti_present) {
    j.start_obj();
    j.write_int("id", 95);
    j.write_str("criticality", "ignore");
    j.write_int("value", c_rnti);
    j.end_obj();
  }
  if (associated_scell_list_present) {
    j.start_obj();
    j.write_int("id", 198);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : associated_scell_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srbs_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 204);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : srbs_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srbs_modified_list_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : srbs_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (full_cfg_present) {
    j.start_obj();
    j.write_int("id", 94);
    j.write_str("criticality", "reject");
    j.write_str("value", "full");
    j.end_obj();
  }
  if (bh_chs_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 275);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bh_chs_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (bh_chs_modified_list_present) {
    j.start_obj();
    j.write_int("id", 273);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bh_chs_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (bh_chs_failed_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 271);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bh_chs_failed_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (bh_chs_failed_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 269);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bh_chs_failed_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 333);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_modified_list_present) {
    j.start_obj();
    j.write_int("id", 318);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_failed_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 334);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_failed_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_failed_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 314);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_failed_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (requested_target_cell_global_id_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    requested_target_cell_global_id.to_json(j);
    j.end_obj();
  }
  if (scg_activation_status_present) {
    j.start_obj();
    j.write_int("id", 548);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_status.to_string());
    j.end_obj();
  }
  if (uu_rlc_ch_setup_list_present) {
    j.start_obj();
    j.write_int("id", 602);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 603);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_modified_list_present) {
    j.start_obj();
    j.write_int("id", 604);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_failed_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 605);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_failed_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_setup_list_present) {
    j.start_obj();
    j.write_int("id", 611);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 612);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_modified_list_present) {
    j.start_obj();
    j.write_int("id", 616);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_failed_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 613);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_failed_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sdt_bearer_cfg_info_present) {
    j.start_obj();
    j.write_int("id", 676);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sdt_bearer_cfg_info.to_json(j);
    j.end_obj();
  }
  if (ue_multicast_m_rbs_setup_list_present) {
    j.start_obj();
    j.write_int("id", 679);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ue_multicast_m_rbs_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (serving_cell_mo_encoded_in_cgc_list_present) {
    j.start_obj();
    j.write_int("id", 697);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : serving_cell_mo_encoded_in_cgc_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ded_si_delivery_ind_present) {
    j.start_obj();
    j.write_int("id", 708);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (cfg_bwp_list_present) {
    j.start_obj();
    j.write_int("id", 709);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cfg_bwp_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (early_sync_info_present) {
    j.start_obj();
    j.write_int("id", 727);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    early_sync_info.to_json(j);
    j.end_obj();
  }
  if (ltm_cfg_present) {
    j.start_obj();
    j.write_int("id", 725);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltm_cfg.to_json(j);
    j.end_obj();
  }
  if (s_cp_ac_cfg_present) {
    j.start_obj();
    j.write_int("id", 792);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    s_cp_ac_cfg.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextReleaseCommandIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 0, 50, 64, 47, 109, 184, 375, 576, 588, 723, 794};
  return map_enum_number(names, 13, idx, "id");
}
bool ue_context_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 0, 50, 64, 47, 109, 184, 375, 576, 588, 723, 794};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 50:
      return crit_e::ignore;
    case 64:
      return crit_e::ignore;
    case 47:
      return crit_e::ignore;
    case 109:
      return crit_e::ignore;
    case 184:
      return crit_e::ignore;
    case 375:
      return crit_e::reject;
    case 576:
      return crit_e::ignore;
    case 588:
      return crit_e::ignore;
    case 723:
      return crit_e::reject;
    case 794:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 50:
      return presence_e::optional;
    case 64:
      return presence_e::conditional;
    case 47:
      return presence_e::optional;
    case 109:
      return presence_e::optional;
    case 184:
      return presence_e::optional;
    case 375:
      return presence_e::optional;
    case 576:
      return presence_e::optional;
    case 588:
      return presence_e::optional;
    case 723:
      return presence_e::optional;
    case 794:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += rrc_container_present ? 1 : 0;
  nof_ies += srb_id_present ? 1 : 0;
  nof_ies += old_gnb_du_ue_f1ap_id_present ? 1 : 0;
  nof_ies += execute_dupl_present ? 1 : 0;
  nof_ies += rrc_delivery_status_request_present ? 1 : 0;
  nof_ies += target_cells_to_cancel_present ? 1 : 0;
  nof_ies += pos_context_rev_ind_present ? 1 : 0;
  nof_ies += cg_sdt_kept_ind_present ? 1 : 0;
  nof_ies += ltm_cells_to_be_released_list_present ? 1 : 0;
  nof_ies += dl_lbt_fail_info_request_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (rrc_container_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_container.pack(bref); }));
  }
  if (srb_id_present) {
    HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, srb_id, (uint8_t)0u, (uint8_t)3u, true, true);
    }));
  }
  if (old_gnb_du_ue_f1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 47, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, old_gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (execute_dupl_present) {
    HANDLE_CODE(pack_ie(bref, 109, crit_e{crit_e::ignore}, [this, &bref]() { return execute_dupl.pack(bref); }));
  }
  if (rrc_delivery_status_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 184, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_delivery_status_request.pack(bref); }));
  }
  if (target_cells_to_cancel_present) {
    HANDLE_CODE(pack_ie(bref, 375, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, target_cells_to_cancel, 1, 8, true);
    }));
  }
  if (pos_context_rev_ind_present) {
    HANDLE_CODE(pack_ie(bref, 576, crit_e{crit_e::ignore}, [this, &bref]() { return pos_context_rev_ind.pack(bref); }));
  }
  if (cg_sdt_kept_ind_present) {
    HANDLE_CODE(pack_ie(bref, 588, crit_e{crit_e::ignore}, [this, &bref]() { return cg_sdt_kept_ind.pack(bref); }));
  }
  if (ltm_cells_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 723, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ltm_cells_to_be_released_list, 1, 8, true);
    }));
  }
  if (dl_lbt_fail_info_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 794, crit_e{crit_e::ignore}, [this, &bref]() { return dl_lbt_fail_info_request.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_release_cmd_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 50: {
        rrc_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_container.unpack(bref); }));
        break;
      }
      case 64: {
        srb_id_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_integer(srb_id, bref, (uint8_t)0u, (uint8_t)3u, true, true); }));
        break;
      }
      case 47: {
        old_gnb_du_ue_f1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 109: {
        execute_dupl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return execute_dupl.unpack(bref); }));
        break;
      }
      case 184: {
        rrc_delivery_status_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_delivery_status_request.unpack(bref); }));
        break;
      }
      case 375: {
        target_cells_to_cancel_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(target_cells_to_cancel, bref, 1, 8, true); }));
        break;
      }
      case 576: {
        pos_context_rev_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pos_context_rev_ind.unpack(bref); }));
        break;
      }
      case 588: {
        cg_sdt_kept_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cg_sdt_kept_ind.unpack(bref); }));
        break;
      }
      case 723: {
        ltm_cells_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ltm_cells_to_be_released_list, bref, 1, 8, true); }));
        break;
      }
      case 794: {
        dl_lbt_fail_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return dl_lbt_fail_info_request.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  if (rrc_container_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_container.to_string());
    j.end_obj();
  }
  if (srb_id_present) {
    j.start_obj();
    j.write_int("id", 64);
    j.write_str("criticality", "ignore");
    j.write_int("value", srb_id);
    j.end_obj();
  }
  if (old_gnb_du_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 47);
    j.write_str("criticality", "ignore");
    j.write_uint("value", old_gnb_du_ue_f1ap_id);
    j.end_obj();
  }
  if (execute_dupl_present) {
    j.start_obj();
    j.write_int("id", 109);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (rrc_delivery_status_request_present) {
    j.start_obj();
    j.write_int("id", 184);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (target_cells_to_cancel_present) {
    j.start_obj();
    j.write_int("id", 375);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : target_cells_to_cancel) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pos_context_rev_ind_present) {
    j.start_obj();
    j.write_int("id", 576);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (cg_sdt_kept_ind_present) {
    j.start_obj();
    j.write_int("id", 588);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (ltm_cells_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 723);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ltm_cells_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (dl_lbt_fail_info_request_present) {
    j.start_obj();
    j.write_int("id", 794);
    j.write_str("criticality", "ignore");
    j.write_str("value", "inquiry");
    j.end_obj();
  }
  j.end_array();
}

// UEContextReleaseCompleteIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_release_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 7, 743};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_context_release_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 7, 743};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 743:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_release_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    case 743:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_release_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += recommended_ss_bs_for_paging_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (recommended_ss_bs_for_paging_list_present) {
    HANDLE_CODE(pack_ie(bref, 743, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, recommended_ss_bs_for_paging_list, 1, 512, true);
    }));
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
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 743: {
        recommended_ss_bs_for_paging_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(recommended_ss_bs_for_paging_list, bref, 1, 512, true);
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
void ue_context_release_complete_ies_container::to_json(json_writer& j) const
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
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (recommended_ss_bs_for_paging_list_present) {
    j.start_obj();
    j.write_int("id", 743);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : recommended_ss_bs_for_paging_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// UEContextReleaseRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 0, 375, 723};
  return map_enum_number(names, 5, idx, "id");
}
bool ue_context_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 0, 375, 723};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 375:
      return crit_e::reject;
    case 723:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 375:
      return presence_e::optional;
    case 723:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += target_cells_to_cancel_present ? 1 : 0;
  nof_ies += ltm_cells_to_be_released_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (target_cells_to_cancel_present) {
    HANDLE_CODE(pack_ie(bref, 375, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, target_cells_to_cancel, 1, 8, true);
    }));
  }
  if (ltm_cells_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 723, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ltm_cells_to_be_released_list, 1, 8, true);
    }));
  }

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
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 375: {
        target_cells_to_cancel_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(target_cells_to_cancel, bref, 1, 8, true); }));
        break;
      }
      case 723: {
        ltm_cells_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ltm_cells_to_be_released_list, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  if (target_cells_to_cancel_present) {
    j.start_obj();
    j.write_int("id", 375);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : target_cells_to_cancel) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ltm_cells_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 723);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ltm_cells_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// UEContextSetupFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 0, 7, 92, 376};
  return map_enum_number(names, 6, idx, "id");
}
bool ue_context_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 0, 7, 92, 376};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 92:
      return crit_e::ignore;
    case 376:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    case 92:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += gnb_du_ue_f1ap_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += potential_sp_cell_list_present ? 1 : 0;
  nof_ies += requested_target_cell_global_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (gnb_du_ue_f1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (potential_sp_cell_list_present) {
    HANDLE_CODE(pack_ie(bref, 92, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, potential_sp_cell_list, 0, 64, true);
    }));
  }
  if (requested_target_cell_global_id_present) {
    HANDLE_CODE(pack_ie(
        bref, 376, crit_e{crit_e::reject}, [this, &bref]() { return requested_target_cell_global_id.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_setup_fail_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        gnb_du_ue_f1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 92: {
        potential_sp_cell_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(potential_sp_cell_list, bref, 0, 64, true); }));
        break;
      }
      case 376: {
        requested_target_cell_global_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_target_cell_global_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  if (gnb_du_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 41);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_ue_f1ap_id);
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
  if (potential_sp_cell_list_present) {
    j.start_obj();
    j.write_int("id", 92);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : potential_sp_cell_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (requested_target_cell_global_id_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    requested_target_cell_global_id.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextSetupRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40,  41,  63,  107, 96,  9,   90,  38,  49,  54,  74,  35,  97,  108, 50,  126,
                                   165, 158, 184, 195, 182, 217, 226, 242, 248, 258, 282, 306, 307, 308, 309, 340,
                                   330, 373, 377, 382, 428, 511, 536, 547, 591, 594, 595, 596, 599, 608, 619, 626,
                                   632, 630, 695, 712, 716, 720, 721, 726, 741, 779, 780, 781, 782, 794, 801, 838};
  return map_enum_number(names, 64, idx, "id");
}
bool ue_context_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40,  41,  63,  107, 96,  9,   90,  38,  49,  54,  74,  35,  97,  108, 50,  126,
                                   165, 158, 184, 195, 182, 217, 226, 242, 248, 258, 282, 306, 307, 308, 309, 340,
                                   330, 373, 377, 382, 428, 511, 536, 547, 591, 594, 595, 596, 599, 608, 619, 626,
                                   632, 630, 695, 712, 716, 720, 721, 726, 741, 779, 780, 781, 782, 794, 801, 838};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::ignore;
    case 63:
      return crit_e::reject;
    case 107:
      return crit_e::reject;
    case 96:
      return crit_e::ignore;
    case 9:
      return crit_e::reject;
    case 90:
      return crit_e::ignore;
    case 38:
      return crit_e::ignore;
    case 49:
      return crit_e::ignore;
    case 54:
      return crit_e::ignore;
    case 74:
      return crit_e::reject;
    case 35:
      return crit_e::reject;
    case 97:
      return crit_e::reject;
    case 108:
      return crit_e::reject;
    case 50:
      return crit_e::ignore;
    case 126:
      return crit_e::ignore;
    case 165:
      return crit_e::ignore;
    case 158:
      return crit_e::ignore;
    case 184:
      return crit_e::ignore;
    case 195:
      return crit_e::ignore;
    case 182:
      return crit_e::ignore;
    case 217:
      return crit_e::reject;
    case 226:
      return crit_e::ignore;
    case 242:
      return crit_e::ignore;
    case 248:
      return crit_e::ignore;
    case 258:
      return crit_e::reject;
    case 282:
      return crit_e::reject;
    case 306:
      return crit_e::ignore;
    case 307:
      return crit_e::ignore;
    case 308:
      return crit_e::ignore;
    case 309:
      return crit_e::ignore;
    case 340:
      return crit_e::ignore;
    case 330:
      return crit_e::reject;
    case 373:
      return crit_e::reject;
    case 377:
      return crit_e::ignore;
    case 382:
      return crit_e::reject;
    case 428:
      return crit_e::reject;
    case 511:
      return crit_e::reject;
    case 536:
      return crit_e::ignore;
    case 547:
      return crit_e::ignore;
    case 591:
      return crit_e::ignore;
    case 594:
      return crit_e::ignore;
    case 595:
      return crit_e::ignore;
    case 596:
      return crit_e::ignore;
    case 599:
      return crit_e::reject;
    case 608:
      return crit_e::reject;
    case 619:
      return crit_e::ignore;
    case 626:
      return crit_e::ignore;
    case 632:
      return crit_e::reject;
    case 630:
      return crit_e::reject;
    case 695:
      return crit_e::ignore;
    case 712:
      return crit_e::ignore;
    case 716:
      return crit_e::ignore;
    case 720:
      return crit_e::reject;
    case 721:
      return crit_e::reject;
    case 726:
      return crit_e::ignore;
    case 741:
      return crit_e::reject;
    case 779:
      return crit_e::ignore;
    case 780:
      return crit_e::ignore;
    case 781:
      return crit_e::ignore;
    case 782:
      return crit_e::ignore;
    case 794:
      return crit_e::ignore;
    case 801:
      return crit_e::ignore;
    case 838:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::optional;
    case 63:
      return presence_e::mandatory;
    case 107:
      return presence_e::mandatory;
    case 96:
      return presence_e::optional;
    case 9:
      return presence_e::mandatory;
    case 90:
      return presence_e::optional;
    case 38:
      return presence_e::optional;
    case 49:
      return presence_e::optional;
    case 54:
      return presence_e::optional;
    case 74:
      return presence_e::optional;
    case 35:
      return presence_e::optional;
    case 97:
      return presence_e::optional;
    case 108:
      return presence_e::optional;
    case 50:
      return presence_e::optional;
    case 126:
      return presence_e::optional;
    case 165:
      return presence_e::optional;
    case 158:
      return presence_e::conditional;
    case 184:
      return presence_e::optional;
    case 195:
      return presence_e::optional;
    case 182:
      return presence_e::optional;
    case 217:
      return presence_e::optional;
    case 226:
      return presence_e::optional;
    case 242:
      return presence_e::optional;
    case 248:
      return presence_e::optional;
    case 258:
      return presence_e::optional;
    case 282:
      return presence_e::optional;
    case 306:
      return presence_e::optional;
    case 307:
      return presence_e::optional;
    case 308:
      return presence_e::optional;
    case 309:
      return presence_e::optional;
    case 340:
      return presence_e::optional;
    case 330:
      return presence_e::optional;
    case 373:
      return presence_e::optional;
    case 377:
      return presence_e::optional;
    case 382:
      return presence_e::optional;
    case 428:
      return presence_e::optional;
    case 511:
      return presence_e::optional;
    case 536:
      return presence_e::optional;
    case 547:
      return presence_e::optional;
    case 591:
      return presence_e::optional;
    case 594:
      return presence_e::optional;
    case 595:
      return presence_e::optional;
    case 596:
      return presence_e::optional;
    case 599:
      return presence_e::optional;
    case 608:
      return presence_e::optional;
    case 619:
      return presence_e::optional;
    case 626:
      return presence_e::optional;
    case 632:
      return presence_e::optional;
    case 630:
      return presence_e::optional;
    case 695:
      return presence_e::optional;
    case 712:
      return presence_e::optional;
    case 716:
      return presence_e::optional;
    case 720:
      return presence_e::optional;
    case 721:
      return presence_e::optional;
    case 726:
      return presence_e::optional;
    case 741:
      return presence_e::optional;
    case 779:
      return presence_e::optional;
    case 780:
      return presence_e::optional;
    case 781:
      return presence_e::optional;
    case 782:
      return presence_e::optional;
    case 794:
      return presence_e::optional;
    case 801:
      return presence_e::optional;
    case 838:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += gnb_du_ue_f1ap_id_present ? 1 : 0;
  nof_ies += sp_cell_ul_cfg_present ? 1 : 0;
  nof_ies += candidate_sp_cell_list_present ? 1 : 0;
  nof_ies += drx_cycle_present ? 1 : 0;
  nof_ies += res_coordination_transfer_container_present ? 1 : 0;
  nof_ies += scell_to_be_setup_list_present ? 1 : 0;
  nof_ies += srbs_to_be_setup_list_present ? 1 : 0;
  nof_ies += drbs_to_be_setup_list_present ? 1 : 0;
  nof_ies += inactivity_monitoring_request_present ? 1 : 0;
  nof_ies += rat_freq_prio_info_present ? 1 : 0;
  nof_ies += rrc_container_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += serving_plmn_present ? 1 : 0;
  nof_ies += gnb_du_ue_ambr_ul_present ? 1 : 0;
  nof_ies += rrc_delivery_status_request_present ? 1 : 0;
  nof_ies += res_coordination_transfer_info_present ? 1 : 0;
  nof_ies += serving_cell_mo_present ? 1 : 0;
  nof_ies += new_gnb_cu_ue_f1ap_id_present ? 1 : 0;
  nof_ies += ran_ue_id_present ? 1 : 0;
  nof_ies += trace_activation_present ? 1 : 0;
  nof_ies += add_rrm_prio_idx_present ? 1 : 0;
  nof_ies += bh_chs_to_be_setup_list_present ? 1 : 0;
  nof_ies += cfg_bap_address_present ? 1 : 0;
  nof_ies += nr_v2x_services_authorized_present ? 1 : 0;
  nof_ies += ltev2x_services_authorized_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += pc5_link_ambr_present ? 1 : 0;
  nof_ies += sl_drbs_to_be_setup_list_present ? 1 : 0;
  nof_ies += conditional_inter_du_mob_info_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_list_present ? 1 : 0;
  nof_ies += serving_n_id_present ? 1 : 0;
  nof_ies += f1_c_transfer_path_present ? 1 : 0;
  nof_ies += f1_c_transfer_path_nr_dc_present ? 1 : 0;
  nof_ies += mdt_polluted_meas_ind_present ? 1 : 0;
  nof_ies += scg_activation_request_present ? 1 : 0;
  nof_ies += cg_sdt_session_info_old_present ? 1 : 0;
  nof_ies += five_g_pro_se_authorized_present ? 1 : 0;
  nof_ies += five_g_pro_se_ue_pc5_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += five_g_pro_se_pc5_link_ambr_present ? 1 : 0;
  nof_ies += uu_rlc_ch_to_be_setup_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_to_be_setup_list_present ? 1 : 0;
  nof_ies += path_switch_cfg_present ? 1 : 0;
  nof_ies += gnb_du_ue_slice_max_bit_rate_list_present ? 1 : 0;
  nof_ies += multicast_mbs_session_setup_list_present ? 1 : 0;
  nof_ies += ue_multicast_m_rbs_to_be_setup_list_present ? 1 : 0;
  nof_ies += serving_cell_mo_list_present ? 1 : 0;
  nof_ies += network_ctrl_repeater_authorized_present ? 1 : 0;
  nof_ies += sdt_volume_thres_present ? 1 : 0;
  nof_ies += ltm_info_setup_present ? 1 : 0;
  nof_ies += ltm_cfg_id_map_list_present ? 1 : 0;
  nof_ies += early_sync_info_request_present ? 1 : 0;
  nof_ies += path_addition_info_present ? 1 : 0;
  nof_ies += nr_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += ltea2_x_services_authorized_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present ? 1 : 0;
  nof_ies += dl_lbt_fail_info_request_present ? 1 : 0;
  nof_ies += sl_positioning_ranging_service_info_present ? 1 : 0;
  nof_ies += non_integer_drx_cycle_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (gnb_du_ue_f1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 63, crit_e{crit_e::reject}, [this, &bref]() { return sp_cell_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 107, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, serv_cell_idx, (uint8_t)0u, (uint8_t)31u, true, true);
  }));
  if (sp_cell_ul_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 96, crit_e{crit_e::ignore}, [this, &bref]() { return sp_cell_ul_cfg.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 9, crit_e{crit_e::reject}, [this, &bref]() { return cu_to_du_rrc_info.pack(bref); }));
  if (candidate_sp_cell_list_present) {
    HANDLE_CODE(pack_ie(bref, 90, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, candidate_sp_cell_list, 1, 64, true);
    }));
  }
  if (drx_cycle_present) {
    HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::ignore}, [this, &bref]() { return drx_cycle.pack(bref); }));
  }
  if (res_coordination_transfer_container_present) {
    HANDLE_CODE(pack_ie(
        bref, 49, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_container.pack(bref); }));
  }
  if (scell_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 54, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, scell_to_be_setup_list, 1, 32, true);
    }));
  }
  if (srbs_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 74, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_to_be_setup_list, 1, 8, true);
    }));
  }
  if (drbs_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 35, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_to_be_setup_list, 1, 64, true);
    }));
  }
  if (inactivity_monitoring_request_present) {
    HANDLE_CODE(pack_ie(
        bref, 97, crit_e{crit_e::reject}, [this, &bref]() { return inactivity_monitoring_request.pack(bref); }));
  }
  if (rat_freq_prio_info_present) {
    HANDLE_CODE(pack_ie(bref, 108, crit_e{crit_e::reject}, [this, &bref]() { return rat_freq_prio_info.pack(bref); }));
  }
  if (rrc_container_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_container.pack(bref); }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 126, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  if (serving_plmn_present) {
    HANDLE_CODE(pack_ie(bref, 165, crit_e{crit_e::ignore}, [this, &bref]() { return serving_plmn.pack(bref); }));
  }
  if (gnb_du_ue_ambr_ul_present) {
    HANDLE_CODE(pack_ie(bref, 158, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, gnb_du_ue_ambr_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (rrc_delivery_status_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 184, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_delivery_status_request.pack(bref); }));
  }
  if (res_coordination_transfer_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 195, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_info.pack(bref); }));
  }
  if (serving_cell_mo_present) {
    HANDLE_CODE(pack_ie(bref, 182, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, serving_cell_mo, (uint16_t)1u, (uint16_t)64u, true, true);
    }));
  }
  if (new_gnb_cu_ue_f1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 217, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, new_gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (ran_ue_id_present) {
    HANDLE_CODE(pack_ie(bref, 226, crit_e{crit_e::ignore}, [this, &bref]() { return ran_ue_id.pack(bref); }));
  }
  if (trace_activation_present) {
    HANDLE_CODE(pack_ie(bref, 242, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));
  }
  if (add_rrm_prio_idx_present) {
    HANDLE_CODE(pack_ie(bref, 248, crit_e{crit_e::ignore}, [this, &bref]() { return add_rrm_prio_idx.pack(bref); }));
  }
  if (bh_chs_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 258, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_to_be_setup_list, 1, 65536, true);
    }));
  }
  if (cfg_bap_address_present) {
    HANDLE_CODE(pack_ie(bref, 282, crit_e{crit_e::reject}, [this, &bref]() { return cfg_bap_address.pack(bref); }));
  }
  if (nr_v2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 306, crit_e{crit_e::ignore}, [this, &bref]() { return nr_v2x_services_authorized.pack(bref); }));
  }
  if (ltev2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 307, crit_e{crit_e::ignore}, [this, &bref]() { return ltev2x_services_authorized.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 308, crit_e{crit_e::ignore}, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 309, crit_e{crit_e::ignore}, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (pc5_link_ambr_present) {
    HANDLE_CODE(pack_ie(bref, 340, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, pc5_link_ambr, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (sl_drbs_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 330, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_to_be_setup_list, 1, 512, true);
    }));
  }
  if (conditional_inter_du_mob_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 373, crit_e{crit_e::reject}, [this, &bref]() { return conditional_inter_du_mob_info.pack(bref); }));
  }
  if (management_based_mdt_plmn_list_present) {
    HANDLE_CODE(pack_ie(bref, 377, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_list, 1, 16, true);
    }));
  }
  if (serving_n_id_present) {
    HANDLE_CODE(pack_ie(bref, 382, crit_e{crit_e::reject}, [this, &bref]() { return serving_n_id.pack(bref); }));
  }
  if (f1_c_transfer_path_present) {
    HANDLE_CODE(pack_ie(bref, 428, crit_e{crit_e::reject}, [this, &bref]() { return f1_c_transfer_path.pack(bref); }));
  }
  if (f1_c_transfer_path_nr_dc_present) {
    HANDLE_CODE(
        pack_ie(bref, 511, crit_e{crit_e::reject}, [this, &bref]() { return f1_c_transfer_path_nr_dc.pack(bref); }));
  }
  if (mdt_polluted_meas_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 536, crit_e{crit_e::ignore}, [this, &bref]() { return mdt_polluted_meas_ind.pack(bref); }));
  }
  if (scg_activation_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 547, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_request.pack(bref); }));
  }
  if (cg_sdt_session_info_old_present) {
    HANDLE_CODE(
        pack_ie(bref, 591, crit_e{crit_e::ignore}, [this, &bref]() { return cg_sdt_session_info_old.pack(bref); }));
  }
  if (five_g_pro_se_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 594, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_authorized.pack(bref); }));
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(bref, 595, crit_e{crit_e::ignore}, [this, &bref]() {
      return five_g_pro_se_ue_pc5_aggr_max_bitrate.pack(bref);
    }));
  }
  if (five_g_pro_se_pc5_link_ambr_present) {
    HANDLE_CODE(pack_ie(bref, 596, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, five_g_pro_se_pc5_link_ambr, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (uu_rlc_ch_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 599, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_to_be_setup_list, 1, 32, true);
    }));
  }
  if (pc5_rlc_ch_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 608, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_to_be_setup_list, 1, 512, true);
    }));
  }
  if (path_switch_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 619, crit_e{crit_e::ignore}, [this, &bref]() { return path_switch_cfg.pack(bref); }));
  }
  if (gnb_du_ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 626, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, gnb_du_ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }
  if (multicast_mbs_session_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 632, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, multicast_mbs_session_setup_list, 1, 256, true);
    }));
  }
  if (ue_multicast_m_rbs_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 630, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_multicast_m_rbs_to_be_setup_list, 1, 64, true);
    }));
  }
  if (serving_cell_mo_list_present) {
    HANDLE_CODE(pack_ie(bref, 695, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, serving_cell_mo_list, 1, 16, true);
    }));
  }
  if (network_ctrl_repeater_authorized_present) {
    HANDLE_CODE(pack_ie(
        bref, 712, crit_e{crit_e::ignore}, [this, &bref]() { return network_ctrl_repeater_authorized.pack(bref); }));
  }
  if (sdt_volume_thres_present) {
    HANDLE_CODE(pack_ie(bref, 716, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sdt_volume_thres, (uint32_t)1u, (uint32_t)192000u, true, true);
    }));
  }
  if (ltm_info_setup_present) {
    HANDLE_CODE(pack_ie(bref, 720, crit_e{crit_e::reject}, [this, &bref]() { return ltm_info_setup.pack(bref); }));
  }
  if (ltm_cfg_id_map_list_present) {
    HANDLE_CODE(pack_ie(bref, 721, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ltm_cfg_id_map_list, 1, 8, true);
    }));
  }
  if (early_sync_info_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 726, crit_e{crit_e::ignore}, [this, &bref]() { return early_sync_info_request.pack(bref); }));
  }
  if (path_addition_info_present) {
    HANDLE_CODE(pack_ie(bref, 741, crit_e{crit_e::reject}, [this, &bref]() { return path_addition_info.pack(bref); }));
  }
  if (nr_a2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 779, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_services_authorized.pack(bref); }));
  }
  if (ltea2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 780, crit_e{crit_e::ignore}, [this, &bref]() { return ltea2_x_services_authorized.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present) {
    HANDLE_CODE(pack_ie(bref, 781, crit_e{crit_e::ignore}, [this, &bref]() {
      return nr_ue_sidelink_aggr_max_bitrate_for_a2_x.pack(bref);
    }));
  }
  if (lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present) {
    HANDLE_CODE(pack_ie(bref, 782, crit_e{crit_e::ignore}, [this, &bref]() {
      return lte_ue_sidelink_aggr_max_bitrate_for_a2_x.pack(bref);
    }));
  }
  if (dl_lbt_fail_info_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 794, crit_e{crit_e::ignore}, [this, &bref]() { return dl_lbt_fail_info_request.pack(bref); }));
  }
  if (sl_positioning_ranging_service_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 801, crit_e{crit_e::ignore}, [this, &bref]() { return sl_positioning_ranging_service_info.pack(bref); }));
  }
  if (non_integer_drx_cycle_present) {
    HANDLE_CODE(
        pack_ie(bref, 838, crit_e{crit_e::ignore}, [this, &bref]() { return non_integer_drx_cycle.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_setup_request_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        gnb_du_ue_f1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 63: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sp_cell_id.unpack(bref); }));
        break;
      }
      case 107: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(serv_cell_idx, bref, (uint8_t)0u, (uint8_t)31u, true, true);
        }));
        break;
      }
      case 96: {
        sp_cell_ul_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sp_cell_ul_cfg.unpack(bref); }));
        break;
      }
      case 9: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cu_to_du_rrc_info.unpack(bref); }));
        break;
      }
      case 90: {
        candidate_sp_cell_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(candidate_sp_cell_list, bref, 1, 64, true); }));
        break;
      }
      case 38: {
        drx_cycle_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return drx_cycle.unpack(bref); }));
        break;
      }
      case 49: {
        res_coordination_transfer_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_container.unpack(bref); }));
        break;
      }
      case 54: {
        scell_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(scell_to_be_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 74: {
        srbs_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_to_be_setup_list, bref, 1, 8, true); }));
        break;
      }
      case 35: {
        drbs_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_to_be_setup_list, bref, 1, 64, true); }));
        break;
      }
      case 97: {
        inactivity_monitoring_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return inactivity_monitoring_request.unpack(bref); }));
        break;
      }
      case 108: {
        rat_freq_prio_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rat_freq_prio_info.unpack(bref); }));
        break;
      }
      case 50: {
        rrc_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_container.unpack(bref); }));
        break;
      }
      case 126: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 165: {
        serving_plmn_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return serving_plmn.unpack(bref); }));
        break;
      }
      case 158: {
        gnb_du_ue_ambr_ul_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_ambr_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 184: {
        rrc_delivery_status_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_delivery_status_request.unpack(bref); }));
        break;
      }
      case 195: {
        res_coordination_transfer_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_info.unpack(bref); }));
        break;
      }
      case 182: {
        serving_cell_mo_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(serving_cell_mo, bref, (uint16_t)1u, (uint16_t)64u, true, true);
        }));
        break;
      }
      case 217: {
        new_gnb_cu_ue_f1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 226: {
        ran_ue_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_ue_id.unpack(bref); }));
        break;
      }
      case 242: {
        trace_activation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_activation.unpack(bref); }));
        break;
      }
      case 248: {
        add_rrm_prio_idx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return add_rrm_prio_idx.unpack(bref); }));
        break;
      }
      case 258: {
        bh_chs_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bh_chs_to_be_setup_list, bref, 1, 65536, true); }));
        break;
      }
      case 282: {
        cfg_bap_address_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cfg_bap_address.unpack(bref); }));
        break;
      }
      case 306: {
        nr_v2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_v2x_services_authorized.unpack(bref); }));
        break;
      }
      case 307: {
        ltev2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltev2x_services_authorized.unpack(bref); }));
        break;
      }
      case 308: {
        nr_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 309: {
        lte_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 340: {
        pc5_link_ambr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(pc5_link_ambr, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 330: {
        sl_drbs_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_to_be_setup_list, bref, 1, 512, true); }));
        break;
      }
      case 373: {
        conditional_inter_du_mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return conditional_inter_du_mob_info.unpack(bref); }));
        break;
      }
      case 377: {
        management_based_mdt_plmn_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(management_based_mdt_plmn_list, bref, 1, 16, true); }));
        break;
      }
      case 382: {
        serving_n_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return serving_n_id.unpack(bref); }));
        break;
      }
      case 428: {
        f1_c_transfer_path_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return f1_c_transfer_path.unpack(bref); }));
        break;
      }
      case 511: {
        f1_c_transfer_path_nr_dc_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return f1_c_transfer_path_nr_dc.unpack(bref); }));
        break;
      }
      case 536: {
        mdt_polluted_meas_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mdt_polluted_meas_ind.unpack(bref); }));
        break;
      }
      case 547: {
        scg_activation_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_request.unpack(bref); }));
        break;
      }
      case 591: {
        cg_sdt_session_info_old_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cg_sdt_session_info_old.unpack(bref); }));
        break;
      }
      case 594: {
        five_g_pro_se_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_authorized.unpack(bref); }));
        break;
      }
      case 595: {
        five_g_pro_se_ue_pc5_aggr_max_bitrate_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_ue_pc5_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 596: {
        five_g_pro_se_pc5_link_ambr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(five_g_pro_se_pc5_link_ambr, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 599: {
        uu_rlc_ch_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_to_be_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 608: {
        pc5_rlc_ch_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pc5_rlc_ch_to_be_setup_list, bref, 1, 512, true); }));
        break;
      }
      case 619: {
        path_switch_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return path_switch_cfg.unpack(bref); }));
        break;
      }
      case 626: {
        gnb_du_ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(gnb_du_ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      case 632: {
        multicast_mbs_session_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(multicast_mbs_session_setup_list, bref, 1, 256, true); }));
        break;
      }
      case 630: {
        ue_multicast_m_rbs_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ue_multicast_m_rbs_to_be_setup_list, bref, 1, 64, true);
        }));
        break;
      }
      case 695: {
        serving_cell_mo_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(serving_cell_mo_list, bref, 1, 16, true); }));
        break;
      }
      case 712: {
        network_ctrl_repeater_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return network_ctrl_repeater_authorized.unpack(bref); }));
        break;
      }
      case 716: {
        sdt_volume_thres_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sdt_volume_thres, bref, (uint32_t)1u, (uint32_t)192000u, true, true);
        }));
        break;
      }
      case 720: {
        ltm_info_setup_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltm_info_setup.unpack(bref); }));
        break;
      }
      case 721: {
        ltm_cfg_id_map_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ltm_cfg_id_map_list, bref, 1, 8, true); }));
        break;
      }
      case 726: {
        early_sync_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return early_sync_info_request.unpack(bref); }));
        break;
      }
      case 741: {
        path_addition_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return path_addition_info.unpack(bref); }));
        break;
      }
      case 779: {
        nr_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 780: {
        ltea2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltea2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 781: {
        nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate_for_a2_x.unpack(bref); }));
        break;
      }
      case 782: {
        lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate_for_a2_x.unpack(bref); }));
        break;
      }
      case 794: {
        dl_lbt_fail_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return dl_lbt_fail_info_request.unpack(bref); }));
        break;
      }
      case 801: {
        sl_positioning_ranging_service_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sl_positioning_ranging_service_info.unpack(bref); }));
        break;
      }
      case 838: {
        non_integer_drx_cycle_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return non_integer_drx_cycle.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  if (gnb_du_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 41);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_ue_f1ap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 63);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  sp_cell_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 107);
  j.write_str("criticality", "reject");
  j.write_int("value", serv_cell_idx);
  j.end_obj();
  if (sp_cell_ul_cfg_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "ignore");
    j.write_str("value", sp_cell_ul_cfg.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 9);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cu_to_du_rrc_info.to_json(j);
  j.end_obj();
  if (candidate_sp_cell_list_present) {
    j.start_obj();
    j.write_int("id", 90);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : candidate_sp_cell_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drx_cycle_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    drx_cycle.to_json(j);
    j.end_obj();
  }
  if (res_coordination_transfer_container_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_str("value", res_coordination_transfer_container.to_string());
    j.end_obj();
  }
  if (scell_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : scell_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srbs_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 74);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : srbs_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 35);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : drbs_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (inactivity_monitoring_request_present) {
    j.start_obj();
    j.write_int("id", 97);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (rat_freq_prio_info_present) {
    j.start_obj();
    j.write_int("id", 108);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    rat_freq_prio_info.to_json(j);
    j.end_obj();
  }
  if (rrc_container_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_container.to_string());
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 126);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  if (serving_plmn_present) {
    j.start_obj();
    j.write_int("id", 165);
    j.write_str("criticality", "ignore");
    j.write_str("value", serving_plmn.to_string());
    j.end_obj();
  }
  if (gnb_du_ue_ambr_ul_present) {
    j.start_obj();
    j.write_int("id", 158);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_ue_ambr_ul);
    j.end_obj();
  }
  if (rrc_delivery_status_request_present) {
    j.start_obj();
    j.write_int("id", 184);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (res_coordination_transfer_info_present) {
    j.start_obj();
    j.write_int("id", 195);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    res_coordination_transfer_info.to_json(j);
    j.end_obj();
  }
  if (serving_cell_mo_present) {
    j.start_obj();
    j.write_int("id", 182);
    j.write_str("criticality", "ignore");
    j.write_int("value", serving_cell_mo);
    j.end_obj();
  }
  if (new_gnb_cu_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 217);
    j.write_str("criticality", "reject");
    j.write_uint("value", new_gnb_cu_ue_f1ap_id);
    j.end_obj();
  }
  if (ran_ue_id_present) {
    j.start_obj();
    j.write_int("id", 226);
    j.write_str("criticality", "ignore");
    j.write_str("value", ran_ue_id.to_string());
    j.end_obj();
  }
  if (trace_activation_present) {
    j.start_obj();
    j.write_int("id", 242);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    trace_activation.to_json(j);
    j.end_obj();
  }
  if (add_rrm_prio_idx_present) {
    j.start_obj();
    j.write_int("id", 248);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    add_rrm_prio_idx.to_json(j);
    j.end_obj();
  }
  if (bh_chs_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 258);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : bh_chs_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (cfg_bap_address_present) {
    j.start_obj();
    j.write_int("id", 282);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    cfg_bap_address.to_json(j);
    j.end_obj();
  }
  if (nr_v2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 306);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_v2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltev2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 307);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltev2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 308);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 309);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (pc5_link_ambr_present) {
    j.start_obj();
    j.write_int("id", 340);
    j.write_str("criticality", "ignore");
    j.write_uint("value", pc5_link_ambr);
    j.end_obj();
  }
  if (sl_drbs_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 330);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (conditional_inter_du_mob_info_present) {
    j.start_obj();
    j.write_int("id", 373);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    conditional_inter_du_mob_info.to_json(j);
    j.end_obj();
  }
  if (management_based_mdt_plmn_list_present) {
    j.start_obj();
    j.write_int("id", 377);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (serving_n_id_present) {
    j.start_obj();
    j.write_int("id", 382);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    serving_n_id.to_json(j);
    j.end_obj();
  }
  if (f1_c_transfer_path_present) {
    j.start_obj();
    j.write_int("id", 428);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    f1_c_transfer_path.to_json(j);
    j.end_obj();
  }
  if (f1_c_transfer_path_nr_dc_present) {
    j.start_obj();
    j.write_int("id", 511);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    f1_c_transfer_path_nr_dc.to_json(j);
    j.end_obj();
  }
  if (mdt_polluted_meas_ind_present) {
    j.start_obj();
    j.write_int("id", 536);
    j.write_str("criticality", "ignore");
    j.write_str("value", mdt_polluted_meas_ind.to_string());
    j.end_obj();
  }
  if (scg_activation_request_present) {
    j.start_obj();
    j.write_int("id", 547);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_request.to_string());
    j.end_obj();
  }
  if (cg_sdt_session_info_old_present) {
    j.start_obj();
    j.write_int("id", 591);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cg_sdt_session_info_old.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_authorized_present) {
    j.start_obj();
    j.write_int("id", 594);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_authorized.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 595);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_ue_pc5_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_pc5_link_ambr_present) {
    j.start_obj();
    j.write_int("id", 596);
    j.write_str("criticality", "ignore");
    j.write_uint("value", five_g_pro_se_pc5_link_ambr);
    j.end_obj();
  }
  if (uu_rlc_ch_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 599);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 608);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (path_switch_cfg_present) {
    j.start_obj();
    j.write_int("id", 619);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    path_switch_cfg.to_json(j);
    j.end_obj();
  }
  if (gnb_du_ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 626);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_du_ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_mbs_session_setup_list_present) {
    j.start_obj();
    j.write_int("id", 632);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : multicast_mbs_session_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_multicast_m_rbs_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 630);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ue_multicast_m_rbs_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (serving_cell_mo_list_present) {
    j.start_obj();
    j.write_int("id", 695);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : serving_cell_mo_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (network_ctrl_repeater_authorized_present) {
    j.start_obj();
    j.write_int("id", 712);
    j.write_str("criticality", "ignore");
    j.write_str("value", network_ctrl_repeater_authorized.to_string());
    j.end_obj();
  }
  if (sdt_volume_thres_present) {
    j.start_obj();
    j.write_int("id", 716);
    j.write_str("criticality", "ignore");
    j.write_int("value", sdt_volume_thres);
    j.end_obj();
  }
  if (ltm_info_setup_present) {
    j.start_obj();
    j.write_int("id", 720);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ltm_info_setup.to_json(j);
    j.end_obj();
  }
  if (ltm_cfg_id_map_list_present) {
    j.start_obj();
    j.write_int("id", 721);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ltm_cfg_id_map_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (early_sync_info_request_present) {
    j.start_obj();
    j.write_int("id", 726);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    early_sync_info_request.to_json(j);
    j.end_obj();
  }
  if (path_addition_info_present) {
    j.start_obj();
    j.write_int("id", 741);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    path_addition_info.to_json(j);
    j.end_obj();
  }
  if (nr_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 779);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltea2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 780);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltea2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present) {
    j.start_obj();
    j.write_int("id", 781);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_ue_sidelink_aggr_max_bitrate_for_a2_x.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present) {
    j.start_obj();
    j.write_int("id", 782);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_ue_sidelink_aggr_max_bitrate_for_a2_x.to_json(j);
    j.end_obj();
  }
  if (dl_lbt_fail_info_request_present) {
    j.start_obj();
    j.write_int("id", 794);
    j.write_str("criticality", "ignore");
    j.write_str("value", "inquiry");
    j.end_obj();
  }
  if (sl_positioning_ranging_service_info_present) {
    j.start_obj();
    j.write_int("id", 801);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sl_positioning_ranging_service_info.to_json(j);
    j.end_obj();
  }
  if (non_integer_drx_cycle_present) {
    j.start_obj();
    j.write_int("id", 838);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    non_integer_drx_cycle.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextSetupResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_context_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40,  41,  39,  95,  49,  94,  27,  66,  15,  83,  98,  7,   202, 260, 279,
                                   324, 316, 376, 548, 602, 603, 611, 612, 697, 699, 708, 709, 727, 725, 792};
  return map_enum_number(names, 30, idx, "id");
}
bool ue_context_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40,  41,  39,  95,  49,  94,  27,  66,  15,  83,  98,  7,   202, 260, 279,
                                   324, 316, 376, 548, 602, 603, 611, 612, 697, 699, 708, 709, 727, 725, 792};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 39:
      return crit_e::reject;
    case 95:
      return crit_e::ignore;
    case 49:
      return crit_e::ignore;
    case 94:
      return crit_e::reject;
    case 27:
      return crit_e::ignore;
    case 66:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 83:
      return crit_e::ignore;
    case 98:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 202:
      return crit_e::ignore;
    case 260:
      return crit_e::ignore;
    case 279:
      return crit_e::ignore;
    case 324:
      return crit_e::ignore;
    case 316:
      return crit_e::ignore;
    case 376:
      return crit_e::reject;
    case 548:
      return crit_e::ignore;
    case 602:
      return crit_e::ignore;
    case 603:
      return crit_e::ignore;
    case 611:
      return crit_e::ignore;
    case 612:
      return crit_e::ignore;
    case 697:
      return crit_e::ignore;
    case 699:
      return crit_e::reject;
    case 708:
      return crit_e::ignore;
    case 709:
      return crit_e::ignore;
    case 727:
      return crit_e::ignore;
    case 725:
      return crit_e::ignore;
    case 792:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 39:
      return presence_e::mandatory;
    case 95:
      return presence_e::optional;
    case 49:
      return presence_e::optional;
    case 94:
      return presence_e::optional;
    case 27:
      return presence_e::optional;
    case 66:
      return presence_e::optional;
    case 15:
      return presence_e::optional;
    case 83:
      return presence_e::optional;
    case 98:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 202:
      return presence_e::optional;
    case 260:
      return presence_e::optional;
    case 279:
      return presence_e::optional;
    case 324:
      return presence_e::optional;
    case 316:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 548:
      return presence_e::optional;
    case 602:
      return presence_e::optional;
    case 603:
      return presence_e::optional;
    case 611:
      return presence_e::optional;
    case 612:
      return presence_e::optional;
    case 697:
      return presence_e::optional;
    case 699:
      return presence_e::optional;
    case 708:
      return presence_e::optional;
    case 709:
      return presence_e::optional;
    case 727:
      return presence_e::optional;
    case 725:
      return presence_e::optional;
    case 792:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += c_rnti_present ? 1 : 0;
  nof_ies += res_coordination_transfer_container_present ? 1 : 0;
  nof_ies += full_cfg_present ? 1 : 0;
  nof_ies += drbs_setup_list_present ? 1 : 0;
  nof_ies += srbs_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += drbs_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += scell_failedto_setup_list_present ? 1 : 0;
  nof_ies += inactivity_monitoring_resp_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += srbs_setup_list_present ? 1 : 0;
  nof_ies += bh_chs_setup_list_present ? 1 : 0;
  nof_ies += bh_chs_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += sl_drbs_setup_list_present ? 1 : 0;
  nof_ies += sl_drbs_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += requested_target_cell_global_id_present ? 1 : 0;
  nof_ies += scg_activation_status_present ? 1 : 0;
  nof_ies += uu_rlc_ch_setup_list_present ? 1 : 0;
  nof_ies += uu_rlc_ch_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_setup_list_present ? 1 : 0;
  nof_ies += pc5_rlc_ch_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += serving_cell_mo_encoded_in_cgc_list_present ? 1 : 0;
  nof_ies += ue_multicast_m_rbs_setupnew_list_present ? 1 : 0;
  nof_ies += ded_si_delivery_ind_present ? 1 : 0;
  nof_ies += cfg_bwp_list_present ? 1 : 0;
  nof_ies += early_sync_info_present ? 1 : 0;
  nof_ies += ltm_cfg_present ? 1 : 0;
  nof_ies += s_cp_ac_cfg_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() { return du_to_cu_rrc_info.pack(bref); }));
  if (c_rnti_present) {
    HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, c_rnti, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  if (res_coordination_transfer_container_present) {
    HANDLE_CODE(pack_ie(
        bref, 49, crit_e{crit_e::ignore}, [this, &bref]() { return res_coordination_transfer_container.pack(bref); }));
  }
  if (full_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 94, crit_e{crit_e::reject}, [this, &bref]() { return full_cfg.pack(bref); }));
  }
  if (drbs_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_setup_list, 1, 64, true);
    }));
  }
  if (srbs_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 66, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_failed_to_be_setup_list, 1, 8, true);
    }));
  }
  if (drbs_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_failed_to_be_setup_list, 1, 64, true);
    }));
  }
  if (scell_failedto_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 83, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, scell_failedto_setup_list, 1, 32, true);
    }));
  }
  if (inactivity_monitoring_resp_present) {
    HANDLE_CODE(
        pack_ie(bref, 98, crit_e{crit_e::reject}, [this, &bref]() { return inactivity_monitoring_resp.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (srbs_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 202, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srbs_setup_list, 1, 8, true);
    }));
  }
  if (bh_chs_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 260, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_setup_list, 1, 65536, true);
    }));
  }
  if (bh_chs_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 279, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, bh_chs_failed_to_be_setup_list, 1, 65536, true);
    }));
  }
  if (sl_drbs_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 324, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_setup_list, 1, 512, true);
    }));
  }
  if (sl_drbs_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 316, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sl_drbs_failed_to_be_setup_list, 1, 512, true);
    }));
  }
  if (requested_target_cell_global_id_present) {
    HANDLE_CODE(pack_ie(
        bref, 376, crit_e{crit_e::reject}, [this, &bref]() { return requested_target_cell_global_id.pack(bref); }));
  }
  if (scg_activation_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 548, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_status.pack(bref); }));
  }
  if (uu_rlc_ch_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 602, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_setup_list, 1, 32, true);
    }));
  }
  if (uu_rlc_ch_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 603, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, uu_rlc_ch_failed_to_be_setup_list, 1, 32, true);
    }));
  }
  if (pc5_rlc_ch_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 611, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_setup_list, 1, 512, true);
    }));
  }
  if (pc5_rlc_ch_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 612, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pc5_rlc_ch_failed_to_be_setup_list, 1, 512, true);
    }));
  }
  if (serving_cell_mo_encoded_in_cgc_list_present) {
    HANDLE_CODE(pack_ie(bref, 697, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, serving_cell_mo_encoded_in_cgc_list, 1, 8, true);
    }));
  }
  if (ue_multicast_m_rbs_setupnew_list_present) {
    HANDLE_CODE(pack_ie(bref, 699, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_multicast_m_rbs_setupnew_list, 1, 64, true);
    }));
  }
  if (ded_si_delivery_ind_present) {
    HANDLE_CODE(pack_ie(bref, 708, crit_e{crit_e::ignore}, [this, &bref]() { return ded_si_delivery_ind.pack(bref); }));
  }
  if (cfg_bwp_list_present) {
    HANDLE_CODE(pack_ie(bref, 709, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, cfg_bwp_list, 1, 8, true);
    }));
  }
  if (early_sync_info_present) {
    HANDLE_CODE(pack_ie(bref, 727, crit_e{crit_e::ignore}, [this, &bref]() { return early_sync_info.pack(bref); }));
  }
  if (ltm_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 725, crit_e{crit_e::ignore}, [this, &bref]() { return ltm_cfg.pack(bref); }));
  }
  if (s_cp_ac_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 792, crit_e{crit_e::ignore}, [this, &bref]() { return s_cp_ac_cfg.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_setup_resp_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 39: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return du_to_cu_rrc_info.unpack(bref); }));
        break;
      }
      case 95: {
        c_rnti_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(c_rnti, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 49: {
        res_coordination_transfer_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return res_coordination_transfer_container.unpack(bref); }));
        break;
      }
      case 94: {
        full_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return full_cfg.unpack(bref); }));
        break;
      }
      case 27: {
        drbs_setup_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_setup_list, bref, 1, 64, true); }));
        break;
      }
      case 66: {
        srbs_failed_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_failed_to_be_setup_list, bref, 1, 8, true); }));
        break;
      }
      case 15: {
        drbs_failed_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(drbs_failed_to_be_setup_list, bref, 1, 64, true); }));
        break;
      }
      case 83: {
        scell_failedto_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(scell_failedto_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 98: {
        inactivity_monitoring_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return inactivity_monitoring_resp.unpack(bref); }));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 202: {
        srbs_setup_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(srbs_setup_list, bref, 1, 8, true); }));
        break;
      }
      case 260: {
        bh_chs_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bh_chs_setup_list, bref, 1, 65536, true); }));
        break;
      }
      case 279: {
        bh_chs_failed_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bh_chs_failed_to_be_setup_list, bref, 1, 65536, true); }));
        break;
      }
      case 324: {
        sl_drbs_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_setup_list, bref, 1, 512, true); }));
        break;
      }
      case 316: {
        sl_drbs_failed_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sl_drbs_failed_to_be_setup_list, bref, 1, 512, true); }));
        break;
      }
      case 376: {
        requested_target_cell_global_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_target_cell_global_id.unpack(bref); }));
        break;
      }
      case 548: {
        scg_activation_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_status.unpack(bref); }));
        break;
      }
      case 602: {
        uu_rlc_ch_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 603: {
        uu_rlc_ch_failed_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(uu_rlc_ch_failed_to_be_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 611: {
        pc5_rlc_ch_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pc5_rlc_ch_setup_list, bref, 1, 512, true); }));
        break;
      }
      case 612: {
        pc5_rlc_ch_failed_to_be_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pc5_rlc_ch_failed_to_be_setup_list, bref, 1, 512, true);
        }));
        break;
      }
      case 697: {
        serving_cell_mo_encoded_in_cgc_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(serving_cell_mo_encoded_in_cgc_list, bref, 1, 8, true);
        }));
        break;
      }
      case 699: {
        ue_multicast_m_rbs_setupnew_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_multicast_m_rbs_setupnew_list, bref, 1, 64, true); }));
        break;
      }
      case 708: {
        ded_si_delivery_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ded_si_delivery_ind.unpack(bref); }));
        break;
      }
      case 709: {
        cfg_bwp_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(cfg_bwp_list, bref, 1, 8, true); }));
        break;
      }
      case 727: {
        early_sync_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return early_sync_info.unpack(bref); }));
        break;
      }
      case 725: {
        ltm_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltm_cfg.unpack(bref); }));
        break;
      }
      case 792: {
        s_cp_ac_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_cp_ac_cfg.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_setup_resp_ies_container::to_json(json_writer& j) const
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
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  du_to_cu_rrc_info.to_json(j);
  j.end_obj();
  if (c_rnti_present) {
    j.start_obj();
    j.write_int("id", 95);
    j.write_str("criticality", "ignore");
    j.write_int("value", c_rnti);
    j.end_obj();
  }
  if (res_coordination_transfer_container_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_str("value", res_coordination_transfer_container.to_string());
    j.end_obj();
  }
  if (full_cfg_present) {
    j.start_obj();
    j.write_int("id", 94);
    j.write_str("criticality", "reject");
    j.write_str("value", "full");
    j.end_obj();
  }
  if (drbs_setup_list_present) {
    j.start_obj();
    j.write_int("id", 27);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srbs_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 66);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : srbs_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drbs_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 15);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (scell_failedto_setup_list_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : scell_failedto_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (inactivity_monitoring_resp_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "reject");
    j.write_str("value", "not-supported");
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
  if (srbs_setup_list_present) {
    j.start_obj();
    j.write_int("id", 202);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : srbs_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (bh_chs_setup_list_present) {
    j.start_obj();
    j.write_int("id", 260);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bh_chs_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (bh_chs_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 279);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bh_chs_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_setup_list_present) {
    j.start_obj();
    j.write_int("id", 324);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_drbs_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 316);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sl_drbs_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (requested_target_cell_global_id_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    requested_target_cell_global_id.to_json(j);
    j.end_obj();
  }
  if (scg_activation_status_present) {
    j.start_obj();
    j.write_int("id", 548);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_status.to_string());
    j.end_obj();
  }
  if (uu_rlc_ch_setup_list_present) {
    j.start_obj();
    j.write_int("id", 602);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (uu_rlc_ch_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 603);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : uu_rlc_ch_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_setup_list_present) {
    j.start_obj();
    j.write_int("id", 611);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pc5_rlc_ch_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 612);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pc5_rlc_ch_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (serving_cell_mo_encoded_in_cgc_list_present) {
    j.start_obj();
    j.write_int("id", 697);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : serving_cell_mo_encoded_in_cgc_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_multicast_m_rbs_setupnew_list_present) {
    j.start_obj();
    j.write_int("id", 699);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ue_multicast_m_rbs_setupnew_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ded_si_delivery_ind_present) {
    j.start_obj();
    j.write_int("id", 708);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (cfg_bwp_list_present) {
    j.start_obj();
    j.write_int("id", 709);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cfg_bwp_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (early_sync_info_present) {
    j.start_obj();
    j.write_int("id", 727);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    early_sync_info.to_json(j);
    j.end_obj();
  }
  if (ltm_cfg_present) {
    j.start_obj();
    j.write_int("id", 725);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltm_cfg.to_json(j);
    j.end_obj();
  }
  if (s_cp_ac_cfg_present) {
    j.start_obj();
    j.write_int("id", 792);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    s_cp_ac_cfg.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEInactivityNotificationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_inactivity_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 100, 635};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_inactivity_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 100, 635};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_inactivity_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 100:
      return crit_e::reject;
    case 635:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_inactivity_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 100:
      return presence_e::mandatory;
    case 635:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_inactivity_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += sdt_termination_request_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 100, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, drb_activity_list, 1, 64, true);
  }));
  if (sdt_termination_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 635, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_termination_request.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_inactivity_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_inactivity_notif_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 100: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(drb_activity_list, bref, 1, 64, true); }));
        break;
      }
      case 635: {
        sdt_termination_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_termination_request.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_inactivity_notif_ies_container::to_json(json_writer& j) const
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
  j.write_int("id", 100);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : drb_activity_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (sdt_termination_request_present) {
    j.start_obj();
    j.write_int("id", 635);
    j.write_str("criticality", "ignore");
    j.write_str("value", sdt_termination_request.to_string());
    j.end_obj();
  }
  j.end_array();
}

// ULRRCMessageTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ul_rrc_msg_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 64, 50, 224, 219};
  return map_enum_number(names, 6, idx, "id");
}
bool ul_rrc_msg_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 64, 50, 224, 219};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_rrc_msg_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 64:
      return crit_e::reject;
    case 50:
      return crit_e::reject;
    case 224:
      return crit_e::reject;
    case 219:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ul_rrc_msg_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 64:
      return presence_e::mandatory;
    case 50:
      return presence_e::mandatory;
    case 224:
      return presence_e::optional;
    case 219:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ul_rrc_msg_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += sel_plmn_id_present ? 1 : 0;
  nof_ies += new_gnb_du_ue_f1ap_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, srb_id, (uint8_t)0u, (uint8_t)3u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::reject}, [this, &bref]() { return rrc_container.pack(bref); }));
  if (sel_plmn_id_present) {
    HANDLE_CODE(pack_ie(bref, 224, crit_e{crit_e::reject}, [this, &bref]() { return sel_plmn_id.pack(bref); }));
  }
  if (new_gnb_du_ue_f1ap_id_present) {
    HANDLE_CODE(pack_ie(bref, 219, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, new_gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_rrc_msg_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_rrc_msg_transfer_ies_o::get_crit));

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 64: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_integer(srb_id, bref, (uint8_t)0u, (uint8_t)3u, true, true); }));
        break;
      }
      case 50: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_container.unpack(bref); }));
        break;
      }
      case 224: {
        sel_plmn_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sel_plmn_id.unpack(bref); }));
        break;
      }
      case 219: {
        new_gnb_du_ue_f1ap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
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
void ul_rrc_msg_transfer_ies_container::to_json(json_writer& j) const
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
  j.write_int("id", 64);
  j.write_str("criticality", "reject");
  j.write_int("value", srb_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 50);
  j.write_str("criticality", "reject");
  j.write_str("value", rrc_container.to_string());
  j.end_obj();
  if (sel_plmn_id_present) {
    j.start_obj();
    j.write_int("id", 224);
    j.write_str("criticality", "reject");
    j.write_str("value", sel_plmn_id.to_string());
    j.end_obj();
  }
  if (new_gnb_du_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 219);
    j.write_str("criticality", "reject");
    j.write_uint("value", new_gnb_du_ue_f1ap_id);
    j.end_obj();
  }
  j.end_array();
}
