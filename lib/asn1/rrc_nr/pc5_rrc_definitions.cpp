// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/rrc_nr/pc5_rrc_definitions.h"

using namespace asn1;
using namespace asn1::rrc_nr;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// AccessStratumReleaseSidelink-r16 ::= ENUMERATED
const char* access_stratum_release_sidelink_r16_opts::to_string() const
{
  static const char* names[] = {"rel16", "rel17", "rel18", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "access_stratum_release_sidelink_r16_e");
}
uint8_t access_stratum_release_sidelink_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {16, 17, 18};
  return map_enum_number(numbers, 3, value, "access_stratum_release_sidelink_r16_e");
}

// BandParametersSidelink-v1710 ::= SEQUENCE
OCUDUASN_CODE band_params_sidelink_v1710_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(tx_iuc_scheme1_mode2_sidelink_r17_present, 1));
  HANDLE_CODE(bref.pack(tx_iuc_scheme2_mode2_sidelink_r17_present, 1));

  if (tx_iuc_scheme2_mode2_sidelink_r17_present) {
    HANDLE_CODE(tx_iuc_scheme2_mode2_sidelink_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_params_sidelink_v1710_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(tx_iuc_scheme1_mode2_sidelink_r17_present, 1));
  HANDLE_CODE(bref.unpack(tx_iuc_scheme2_mode2_sidelink_r17_present, 1));

  if (tx_iuc_scheme2_mode2_sidelink_r17_present) {
    HANDLE_CODE(tx_iuc_scheme2_mode2_sidelink_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void band_params_sidelink_v1710_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (tx_iuc_scheme1_mode2_sidelink_r17_present) {
    j.write_str("tx-IUC-Scheme1-Mode2Sidelink-r17", "supported");
  }
  if (tx_iuc_scheme2_mode2_sidelink_r17_present) {
    j.write_str("tx-IUC-Scheme2-Mode2Sidelink-r17", tx_iuc_scheme2_mode2_sidelink_r17.to_string());
  }
  j.end_obj();
}

const char* band_params_sidelink_v1710_s::tx_iuc_scheme2_mode2_sidelink_r17_opts::to_string() const
{
  static const char* names[] = {"n4", "n8", "n16"};
  return convert_enum_idx(names, 3, value, "band_params_sidelink_v1710_s::tx_iuc_scheme2_mode2_sidelink_r17_e_");
}
uint8_t band_params_sidelink_v1710_s::tx_iuc_scheme2_mode2_sidelink_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {4, 8, 16};
  return map_enum_number(numbers, 3, value, "band_params_sidelink_v1710_s::tx_iuc_scheme2_mode2_sidelink_r17_e_");
}

// BandSidelinkPC5-r16 ::= SEQUENCE
OCUDUASN_CODE band_sidelink_pc5_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= csi_report_sidelink_r16.is_present();
  group_flags[0] |= rank_two_reception_r16_present;
  group_flags[0] |= sl_open_loop_pc_rsrp_report_sidelink_r16_present;
  group_flags[0] |= sl_rx_256_qam_r16_present;
  group_flags[1] |= rx_iuc_scheme1_preferred_mode2_sidelink_r17_present;
  group_flags[1] |= rx_iuc_scheme1_non_preferred_mode2_sidelink_r17_present;
  group_flags[1] |= rx_iuc_scheme2_mode2_sidelink_r17_present;
  group_flags[1] |= rx_iuc_scheme1_sci_r17_present;
  group_flags[1] |= rx_iuc_scheme1_sci_explicit_req_r17_present;
  group_flags[1] |= scheme2_conflict_determination_rsrp_r17_present;
  group_flags[2] |= sl_pathloss_based_olpc_sl_rsrp_report_r18_present;
  group_flags[2] |= sl_ue_cot_sharing_r18_present;
  group_flags[2] |= sl_psfch_multi_contiguous_rb_r18_present;
  group_flags[2] |= sl_psfch_multi_non_contiguous_rb_r18_present;
  group_flags[2] |= sl_ca_communication_r18.is_present();
  group_flags[2] |= sl_reception_intra_carrier_guard_band_r18_present;
  group_flags[2] |= sl_pwr_class_unlicensed_r18_present;
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(bref.pack(sl_reception_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_tx_256_qam_r16_present, 1));
  HANDLE_CODE(bref.pack(low_se_64_qam_mcs_table_sidelink_r16_present, 1));

  HANDLE_CODE(pack_integer(bref, freq_band_sidelink_r16, (uint16_t)1u, (uint16_t)1024u));
  if (sl_reception_r16_present) {
    HANDLE_CODE(bref.pack(sl_reception_r16.scs_cp_pattern_rx_sidelink_r16_present, 1));
    HANDLE_CODE(bref.pack(sl_reception_r16.extended_cp_rx_sidelink_r16_present, 1));
    HANDLE_CODE(sl_reception_r16.harq_rx_process_sidelink_r16.pack(bref));
    HANDLE_CODE(sl_reception_r16.pscch_rx_sidelink_r16.pack(bref));
    if (sl_reception_r16.scs_cp_pattern_rx_sidelink_r16_present) {
      HANDLE_CODE(sl_reception_r16.scs_cp_pattern_rx_sidelink_r16.pack(bref));
    }
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(csi_report_sidelink_r16.is_present(), 1));
      HANDLE_CODE(bref.pack(rank_two_reception_r16_present, 1));
      HANDLE_CODE(bref.pack(sl_open_loop_pc_rsrp_report_sidelink_r16_present, 1));
      HANDLE_CODE(bref.pack(sl_rx_256_qam_r16_present, 1));
      if (csi_report_sidelink_r16.is_present()) {
        HANDLE_CODE(csi_report_sidelink_r16->csi_rs_ports_sidelink_r16.pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(rx_iuc_scheme1_preferred_mode2_sidelink_r17_present, 1));
      HANDLE_CODE(bref.pack(rx_iuc_scheme1_non_preferred_mode2_sidelink_r17_present, 1));
      HANDLE_CODE(bref.pack(rx_iuc_scheme2_mode2_sidelink_r17_present, 1));
      HANDLE_CODE(bref.pack(rx_iuc_scheme1_sci_r17_present, 1));
      HANDLE_CODE(bref.pack(rx_iuc_scheme1_sci_explicit_req_r17_present, 1));
      HANDLE_CODE(bref.pack(scheme2_conflict_determination_rsrp_r17_present, 1));
      if (rx_iuc_scheme2_mode2_sidelink_r17_present) {
        HANDLE_CODE(rx_iuc_scheme2_mode2_sidelink_r17.pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_pathloss_based_olpc_sl_rsrp_report_r18_present, 1));
      HANDLE_CODE(bref.pack(sl_ue_cot_sharing_r18_present, 1));
      HANDLE_CODE(bref.pack(sl_psfch_multi_contiguous_rb_r18_present, 1));
      HANDLE_CODE(bref.pack(sl_psfch_multi_non_contiguous_rb_r18_present, 1));
      HANDLE_CODE(bref.pack(sl_ca_communication_r18.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_reception_intra_carrier_guard_band_r18_present, 1));
      HANDLE_CODE(bref.pack(sl_pwr_class_unlicensed_r18_present, 1));
      if (sl_ca_communication_r18.is_present()) {
        HANDLE_CODE(pack_integer(bref, sl_ca_communication_r18->nof_carriers_r18, (uint8_t)2u, (uint8_t)8u));
        HANDLE_CODE(
            pack_integer(bref, sl_ca_communication_r18->nof_pscch_decode_value_z_r18, (uint8_t)1u, (uint8_t)2u));
        HANDLE_CODE(sl_ca_communication_r18->total_bw_r18.pack(bref));
      }
      if (sl_pwr_class_unlicensed_r18_present) {
        HANDLE_CODE(sl_pwr_class_unlicensed_r18.pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_sidelink_pc5_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_reception_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_tx_256_qam_r16_present, 1));
  HANDLE_CODE(bref.unpack(low_se_64_qam_mcs_table_sidelink_r16_present, 1));

  HANDLE_CODE(unpack_integer(freq_band_sidelink_r16, bref, (uint16_t)1u, (uint16_t)1024u));
  if (sl_reception_r16_present) {
    HANDLE_CODE(bref.unpack(sl_reception_r16.scs_cp_pattern_rx_sidelink_r16_present, 1));
    HANDLE_CODE(bref.unpack(sl_reception_r16.extended_cp_rx_sidelink_r16_present, 1));
    HANDLE_CODE(sl_reception_r16.harq_rx_process_sidelink_r16.unpack(bref));
    HANDLE_CODE(sl_reception_r16.pscch_rx_sidelink_r16.unpack(bref));
    if (sl_reception_r16.scs_cp_pattern_rx_sidelink_r16_present) {
      HANDLE_CODE(sl_reception_r16.scs_cp_pattern_rx_sidelink_r16.unpack(bref));
    }
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(csi_report_sidelink_r16, bref);
      HANDLE_CODE(bref.unpack(rank_two_reception_r16_present, 1));
      HANDLE_CODE(bref.unpack(sl_open_loop_pc_rsrp_report_sidelink_r16_present, 1));
      HANDLE_CODE(bref.unpack(sl_rx_256_qam_r16_present, 1));
      if (csi_report_sidelink_r16.is_present()) {
        HANDLE_CODE(csi_report_sidelink_r16->csi_rs_ports_sidelink_r16.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(rx_iuc_scheme1_preferred_mode2_sidelink_r17_present, 1));
      HANDLE_CODE(bref.unpack(rx_iuc_scheme1_non_preferred_mode2_sidelink_r17_present, 1));
      HANDLE_CODE(bref.unpack(rx_iuc_scheme2_mode2_sidelink_r17_present, 1));
      HANDLE_CODE(bref.unpack(rx_iuc_scheme1_sci_r17_present, 1));
      HANDLE_CODE(bref.unpack(rx_iuc_scheme1_sci_explicit_req_r17_present, 1));
      HANDLE_CODE(bref.unpack(scheme2_conflict_determination_rsrp_r17_present, 1));
      if (rx_iuc_scheme2_mode2_sidelink_r17_present) {
        HANDLE_CODE(rx_iuc_scheme2_mode2_sidelink_r17.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(sl_pathloss_based_olpc_sl_rsrp_report_r18_present, 1));
      HANDLE_CODE(bref.unpack(sl_ue_cot_sharing_r18_present, 1));
      HANDLE_CODE(bref.unpack(sl_psfch_multi_contiguous_rb_r18_present, 1));
      HANDLE_CODE(bref.unpack(sl_psfch_multi_non_contiguous_rb_r18_present, 1));
      unpack_presence_flag(sl_ca_communication_r18, bref);
      HANDLE_CODE(bref.unpack(sl_reception_intra_carrier_guard_band_r18_present, 1));
      HANDLE_CODE(bref.unpack(sl_pwr_class_unlicensed_r18_present, 1));
      if (sl_ca_communication_r18.is_present()) {
        HANDLE_CODE(unpack_integer(sl_ca_communication_r18->nof_carriers_r18, bref, (uint8_t)2u, (uint8_t)8u));
        HANDLE_CODE(
            unpack_integer(sl_ca_communication_r18->nof_pscch_decode_value_z_r18, bref, (uint8_t)1u, (uint8_t)2u));
        HANDLE_CODE(sl_ca_communication_r18->total_bw_r18.unpack(bref));
      }
      if (sl_pwr_class_unlicensed_r18_present) {
        HANDLE_CODE(sl_pwr_class_unlicensed_r18.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void band_sidelink_pc5_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("freqBandSidelink-r16", freq_band_sidelink_r16);
  if (sl_reception_r16_present) {
    j.write_fieldname("sl-Reception-r16");
    j.start_obj();
    j.write_str("harq-RxProcessSidelink-r16", sl_reception_r16.harq_rx_process_sidelink_r16.to_string());
    j.write_str("pscch-RxSidelink-r16", sl_reception_r16.pscch_rx_sidelink_r16.to_string());
    if (sl_reception_r16.scs_cp_pattern_rx_sidelink_r16_present) {
      j.write_fieldname("scs-CP-PatternRxSidelink-r16");
      sl_reception_r16.scs_cp_pattern_rx_sidelink_r16.to_json(j);
    }
    if (sl_reception_r16.extended_cp_rx_sidelink_r16_present) {
      j.write_str("extendedCP-RxSidelink-r16", "supported");
    }
    j.end_obj();
  }
  if (sl_tx_256_qam_r16_present) {
    j.write_str("sl-Tx-256QAM-r16", "supported");
  }
  if (low_se_64_qam_mcs_table_sidelink_r16_present) {
    j.write_str("lowSE-64QAM-MCS-TableSidelink-r16", "supported");
  }
  if (csi_report_sidelink_r16.is_present()) {
    j.write_fieldname("csi-ReportSidelink-r16");
    j.start_obj();
    j.write_str("csi-RS-PortsSidelink-r16", csi_report_sidelink_r16->csi_rs_ports_sidelink_r16.to_string());
    j.end_obj();
  }
  if (rank_two_reception_r16_present) {
    j.write_str("rankTwoReception-r16", "supported");
  }
  if (sl_open_loop_pc_rsrp_report_sidelink_r16_present) {
    j.write_str("sl-openLoopPC-RSRP-ReportSidelink-r16", "supported");
  }
  if (sl_rx_256_qam_r16_present) {
    j.write_str("sl-Rx-256QAM-r16", "supported");
  }
  if (rx_iuc_scheme1_preferred_mode2_sidelink_r17_present) {
    j.write_str("rx-IUC-Scheme1-PreferredMode2Sidelink-r17", "supported");
  }
  if (rx_iuc_scheme1_non_preferred_mode2_sidelink_r17_present) {
    j.write_str("rx-IUC-Scheme1-NonPreferredMode2Sidelink-r17", "supported");
  }
  if (rx_iuc_scheme2_mode2_sidelink_r17_present) {
    j.write_str("rx-IUC-Scheme2-Mode2Sidelink-r17", rx_iuc_scheme2_mode2_sidelink_r17.to_string());
  }
  if (rx_iuc_scheme1_sci_r17_present) {
    j.write_str("rx-IUC-Scheme1-SCI-r17", "supported");
  }
  if (rx_iuc_scheme1_sci_explicit_req_r17_present) {
    j.write_str("rx-IUC-Scheme1-SCI-ExplicitReq-r17", "supported");
  }
  if (scheme2_conflict_determination_rsrp_r17_present) {
    j.write_str("scheme2-ConflictDeterminationRSRP-r17", "supported");
  }
  if (sl_pathloss_based_olpc_sl_rsrp_report_r18_present) {
    j.write_str("sl-PathlossBasedOLPC-SL-RSRP-Report-r18", "supported");
  }
  if (sl_ue_cot_sharing_r18_present) {
    j.write_str("sl-UE-COT-Sharing-r18", "supported");
  }
  if (sl_psfch_multi_contiguous_rb_r18_present) {
    j.write_str("sl-PSFCH-MultiContiguousRB-r18", "supported");
  }
  if (sl_psfch_multi_non_contiguous_rb_r18_present) {
    j.write_str("sl-PSFCH-MultiNonContiguousRB-r18", "supported");
  }
  if (sl_ca_communication_r18.is_present()) {
    j.write_fieldname("sl-CA-Communication-r18");
    j.start_obj();
    j.write_int("numberOfCarriers-r18", sl_ca_communication_r18->nof_carriers_r18);
    j.write_int("numberOfPSCCH-DecodeValueZ-r18", sl_ca_communication_r18->nof_pscch_decode_value_z_r18);
    j.write_str("totalBandwidth-r18", sl_ca_communication_r18->total_bw_r18.to_string());
    j.end_obj();
  }
  if (sl_reception_intra_carrier_guard_band_r18_present) {
    j.write_str("sl-ReceptionIntraCarrierGuardBand-r18", "supported");
  }
  if (sl_pwr_class_unlicensed_r18_present) {
    j.write_str("sl-PowerClassUnlicensed-r18", sl_pwr_class_unlicensed_r18.to_string());
  }
  j.end_obj();
}

const char* band_sidelink_pc5_r16_s::sl_reception_r16_s_::harq_rx_process_sidelink_r16_opts::to_string() const
{
  static const char* names[] = {"n16", "n24", "n32", "n64"};
  return convert_enum_idx(
      names, 4, value, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::harq_rx_process_sidelink_r16_e_");
}
uint8_t band_sidelink_pc5_r16_s::sl_reception_r16_s_::harq_rx_process_sidelink_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {16, 24, 32, 64};
  return map_enum_number(
      numbers, 4, value, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::harq_rx_process_sidelink_r16_e_");
}

const char* band_sidelink_pc5_r16_s::sl_reception_r16_s_::pscch_rx_sidelink_r16_opts::to_string() const
{
  static const char* names[] = {"value1", "value2"};
  return convert_enum_idx(names, 2, value, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::pscch_rx_sidelink_r16_e_");
}
uint8_t band_sidelink_pc5_r16_s::sl_reception_r16_s_::pscch_rx_sidelink_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::pscch_rx_sidelink_r16_e_");
}

void band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::destroy_()
{
  switch (type_) {
    case types::fr1_r16:
      c.destroy<fr1_r16_s_>();
      break;
    case types::fr2_r16:
      c.destroy<fr2_r16_s_>();
      break;
    default:
      break;
  }
}
void band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::fr1_r16:
      c.init<fr1_r16_s_>();
      break;
    case types::fr2_r16:
      c.init<fr2_r16_s_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_");
  }
}
band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::scs_cp_pattern_rx_sidelink_r16_c_(
    const band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::fr1_r16:
      c.init(other.c.get<fr1_r16_s_>());
      break;
    case types::fr2_r16:
      c.init(other.c.get<fr2_r16_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_");
  }
}
band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_&
band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::operator=(
    const band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::fr1_r16:
      c.set(other.c.get<fr1_r16_s_>());
      break;
    case types::fr2_r16:
      c.set(other.c.get<fr2_r16_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_");
  }

  return *this;
}
band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::fr1_r16_s_&
band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::set_fr1_r16()
{
  set(types::fr1_r16);
  return c.get<fr1_r16_s_>();
}
band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::fr2_r16_s_&
band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::set_fr2_r16()
{
  set(types::fr2_r16);
  return c.get<fr2_r16_s_>();
}
void band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::fr1_r16:
      j.write_fieldname("fr1-r16");
      j.start_obj();
      if (c.get<fr1_r16_s_>().scs_15k_hz_r16_present) {
        j.write_str("scs-15kHz-r16", c.get<fr1_r16_s_>().scs_15k_hz_r16.to_string());
      }
      if (c.get<fr1_r16_s_>().scs_30k_hz_r16_present) {
        j.write_str("scs-30kHz-r16", c.get<fr1_r16_s_>().scs_30k_hz_r16.to_string());
      }
      if (c.get<fr1_r16_s_>().scs_60k_hz_r16_present) {
        j.write_str("scs-60kHz-r16", c.get<fr1_r16_s_>().scs_60k_hz_r16.to_string());
      }
      j.end_obj();
      break;
    case types::fr2_r16:
      j.write_fieldname("fr2-r16");
      j.start_obj();
      if (c.get<fr2_r16_s_>().scs_60k_hz_r16_present) {
        j.write_str("scs-60kHz-r16", c.get<fr2_r16_s_>().scs_60k_hz_r16.to_string());
      }
      if (c.get<fr2_r16_s_>().scs_120k_hz_r16_present) {
        j.write_str("scs-120kHz-r16", c.get<fr2_r16_s_>().scs_120k_hz_r16.to_string());
      }
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::fr1_r16:
      HANDLE_CODE(bref.pack(c.get<fr1_r16_s_>().scs_15k_hz_r16_present, 1));
      HANDLE_CODE(bref.pack(c.get<fr1_r16_s_>().scs_30k_hz_r16_present, 1));
      HANDLE_CODE(bref.pack(c.get<fr1_r16_s_>().scs_60k_hz_r16_present, 1));
      if (c.get<fr1_r16_s_>().scs_15k_hz_r16_present) {
        HANDLE_CODE(c.get<fr1_r16_s_>().scs_15k_hz_r16.pack(bref));
      }
      if (c.get<fr1_r16_s_>().scs_30k_hz_r16_present) {
        HANDLE_CODE(c.get<fr1_r16_s_>().scs_30k_hz_r16.pack(bref));
      }
      if (c.get<fr1_r16_s_>().scs_60k_hz_r16_present) {
        HANDLE_CODE(c.get<fr1_r16_s_>().scs_60k_hz_r16.pack(bref));
      }
      break;
    case types::fr2_r16:
      HANDLE_CODE(bref.pack(c.get<fr2_r16_s_>().scs_60k_hz_r16_present, 1));
      HANDLE_CODE(bref.pack(c.get<fr2_r16_s_>().scs_120k_hz_r16_present, 1));
      if (c.get<fr2_r16_s_>().scs_60k_hz_r16_present) {
        HANDLE_CODE(c.get<fr2_r16_s_>().scs_60k_hz_r16.pack(bref));
      }
      if (c.get<fr2_r16_s_>().scs_120k_hz_r16_present) {
        HANDLE_CODE(c.get<fr2_r16_s_>().scs_120k_hz_r16.pack(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::fr1_r16:
      HANDLE_CODE(bref.unpack(c.get<fr1_r16_s_>().scs_15k_hz_r16_present, 1));
      HANDLE_CODE(bref.unpack(c.get<fr1_r16_s_>().scs_30k_hz_r16_present, 1));
      HANDLE_CODE(bref.unpack(c.get<fr1_r16_s_>().scs_60k_hz_r16_present, 1));
      if (c.get<fr1_r16_s_>().scs_15k_hz_r16_present) {
        HANDLE_CODE(c.get<fr1_r16_s_>().scs_15k_hz_r16.unpack(bref));
      }
      if (c.get<fr1_r16_s_>().scs_30k_hz_r16_present) {
        HANDLE_CODE(c.get<fr1_r16_s_>().scs_30k_hz_r16.unpack(bref));
      }
      if (c.get<fr1_r16_s_>().scs_60k_hz_r16_present) {
        HANDLE_CODE(c.get<fr1_r16_s_>().scs_60k_hz_r16.unpack(bref));
      }
      break;
    case types::fr2_r16:
      HANDLE_CODE(bref.unpack(c.get<fr2_r16_s_>().scs_60k_hz_r16_present, 1));
      HANDLE_CODE(bref.unpack(c.get<fr2_r16_s_>().scs_120k_hz_r16_present, 1));
      if (c.get<fr2_r16_s_>().scs_60k_hz_r16_present) {
        HANDLE_CODE(c.get<fr2_r16_s_>().scs_60k_hz_r16.unpack(bref));
      }
      if (c.get<fr2_r16_s_>().scs_120k_hz_r16_present) {
        HANDLE_CODE(c.get<fr2_r16_s_>().scs_120k_hz_r16.unpack(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char*
band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::types_opts::to_string() const
{
  static const char* names[] = {"fr1-r16", "fr2-r16"};
  return convert_enum_idx(
      names, 2, value, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::types");
}
uint8_t band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(
      numbers, 2, value, "band_sidelink_pc5_r16_s::sl_reception_r16_s_::scs_cp_pattern_rx_sidelink_r16_c_::types");
}

const char* band_sidelink_pc5_r16_s::csi_report_sidelink_r16_s_::csi_rs_ports_sidelink_r16_opts::to_string() const
{
  static const char* names[] = {"p1", "p2"};
  return convert_enum_idx(
      names, 2, value, "band_sidelink_pc5_r16_s::csi_report_sidelink_r16_s_::csi_rs_ports_sidelink_r16_e_");
}
uint8_t band_sidelink_pc5_r16_s::csi_report_sidelink_r16_s_::csi_rs_ports_sidelink_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(
      numbers, 2, value, "band_sidelink_pc5_r16_s::csi_report_sidelink_r16_s_::csi_rs_ports_sidelink_r16_e_");
}

const char* band_sidelink_pc5_r16_s::rx_iuc_scheme2_mode2_sidelink_r17_opts::to_string() const
{
  static const char* names[] = {"n5", "n15", "n25", "n32", "n35", "n45", "n50", "n64"};
  return convert_enum_idx(names, 8, value, "band_sidelink_pc5_r16_s::rx_iuc_scheme2_mode2_sidelink_r17_e_");
}
uint8_t band_sidelink_pc5_r16_s::rx_iuc_scheme2_mode2_sidelink_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {5, 15, 25, 32, 35, 45, 50, 64};
  return map_enum_number(numbers, 8, value, "band_sidelink_pc5_r16_s::rx_iuc_scheme2_mode2_sidelink_r17_e_");
}

const char* band_sidelink_pc5_r16_s::sl_ca_communication_r18_s_::total_bw_r18_opts::to_string() const
{
  static const char* names[] = {"mhz20", "mhz30", "mhz40", "mhz50", "mhz60", "mhz70"};
  return convert_enum_idx(names, 6, value, "band_sidelink_pc5_r16_s::sl_ca_communication_r18_s_::total_bw_r18_e_");
}
uint8_t band_sidelink_pc5_r16_s::sl_ca_communication_r18_s_::total_bw_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {20, 30, 40, 50, 60, 70};
  return map_enum_number(numbers, 6, value, "band_sidelink_pc5_r16_s::sl_ca_communication_r18_s_::total_bw_r18_e_");
}

const char* band_sidelink_pc5_r16_s::sl_pwr_class_unlicensed_r18_opts::to_string() const
{
  static const char* names[] = {"pc5", "spare7", "spare6", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "band_sidelink_pc5_r16_s::sl_pwr_class_unlicensed_r18_e_");
}
uint8_t band_sidelink_pc5_r16_s::sl_pwr_class_unlicensed_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {5};
  return map_enum_number(numbers, 1, value, "band_sidelink_pc5_r16_s::sl_pwr_class_unlicensed_r18_e_");
}

// MAC-ParametersSidelink-r17 ::= SEQUENCE
OCUDUASN_CODE mac_params_sidelink_r17_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(drx_on_sidelink_r17_present, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mac_params_sidelink_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(drx_on_sidelink_r17_present, 1));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mac_params_sidelink_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (drx_on_sidelink_r17_present) {
    j.write_str("drx-OnSidelink-r17", "supported");
  }
  j.end_obj();
}

// MasterInformationBlockSidelink ::= SEQUENCE
OCUDUASN_CODE mib_sidelink_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(sl_tdd_cfg_r16.pack(bref));
  HANDLE_CODE(bref.pack(in_coverage_r16, 1));
  HANDLE_CODE(direct_frame_num_r16.pack(bref));
  HANDLE_CODE(slot_idx_r16.pack(bref));
  HANDLE_CODE(reserved_bits_r16.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mib_sidelink_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(sl_tdd_cfg_r16.unpack(bref));
  HANDLE_CODE(bref.unpack(in_coverage_r16, 1));
  HANDLE_CODE(direct_frame_num_r16.unpack(bref));
  HANDLE_CODE(slot_idx_r16.unpack(bref));
  HANDLE_CODE(reserved_bits_r16.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void mib_sidelink_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("sl-TDD-Config-r16", sl_tdd_cfg_r16.to_string());
  j.write_bool("inCoverage-r16", in_coverage_r16);
  j.write_str("directFrameNumber-r16", direct_frame_num_r16.to_string());
  j.write_str("slotIndex-r16", slot_idx_r16.to_string());
  j.write_str("reservedBits-r16", reserved_bits_r16.to_string());
  j.end_obj();
}

// SL-MeasQuantityResult-r16 ::= SEQUENCE
OCUDUASN_CODE sl_meas_quant_result_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= sl_rsrp_ded_sl_prs_rp_r18_present;
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(bref.pack(sl_rsrp_r16_present, 1));

  if (sl_rsrp_r16_present) {
    HANDLE_CODE(pack_integer(bref, sl_rsrp_r16, (uint8_t)0u, (uint8_t)127u));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_rsrp_ded_sl_prs_rp_r18_present, 1));
      if (sl_rsrp_ded_sl_prs_rp_r18_present) {
        HANDLE_CODE(pack_integer(bref, sl_rsrp_ded_sl_prs_rp_r18, (uint8_t)0u, (uint8_t)13u));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_meas_quant_result_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_rsrp_r16_present, 1));

  if (sl_rsrp_r16_present) {
    HANDLE_CODE(unpack_integer(sl_rsrp_r16, bref, (uint8_t)0u, (uint8_t)127u));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(sl_rsrp_ded_sl_prs_rp_r18_present, 1));
      if (sl_rsrp_ded_sl_prs_rp_r18_present) {
        HANDLE_CODE(unpack_integer(sl_rsrp_ded_sl_prs_rp_r18, bref, (uint8_t)0u, (uint8_t)13u));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_meas_quant_result_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_rsrp_r16_present) {
    j.write_int("sl-RSRP-r16", sl_rsrp_r16);
  }
  if (sl_rsrp_ded_sl_prs_rp_r18_present) {
    j.write_int("sl-RSRP-DedicatedSL-PRS-RP-r18", sl_rsrp_ded_sl_prs_rp_r18);
  }
  j.end_obj();
}

// SL-MeasResult-r16 ::= SEQUENCE
OCUDUASN_CODE sl_meas_result_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= sl_result_sl_prs_r18.is_present();
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(bref.pack(sl_result_dmrs_r16_present, 1));

  if (sl_result_dmrs_r16_present) {
    HANDLE_CODE(sl_result_dmrs_r16.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_result_sl_prs_r18.is_present(), 1));
      if (sl_result_sl_prs_r18.is_present()) {
        HANDLE_CODE(sl_result_sl_prs_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_meas_result_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_result_dmrs_r16_present, 1));

  if (sl_result_dmrs_r16_present) {
    HANDLE_CODE(sl_result_dmrs_r16.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_result_sl_prs_r18, bref);
      if (sl_result_sl_prs_r18.is_present()) {
        HANDLE_CODE(sl_result_sl_prs_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_meas_result_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_result_dmrs_r16_present) {
    j.write_fieldname("sl-ResultDMRS-r16");
    sl_result_dmrs_r16.to_json(j);
  }
  if (sl_result_sl_prs_r18.is_present()) {
    j.write_fieldname("sl-Result-SL-PRS-r18");
    sl_result_sl_prs_r18->to_json(j);
  }
  j.end_obj();
}

// SL-MeasResults-r16 ::= SEQUENCE
OCUDUASN_CODE sl_meas_results_r16_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, sl_meas_id_r16, (uint8_t)1u, (uint8_t)64u));
  HANDLE_CODE(sl_meas_result_r16.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_meas_results_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(sl_meas_id_r16, bref, (uint8_t)1u, (uint8_t)64u));
  HANDLE_CODE(sl_meas_result_r16.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_meas_results_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sl-MeasId-r16", sl_meas_id_r16);
  j.write_fieldname("sl-MeasResult-r16");
  sl_meas_result_r16.to_json(j);
  j.end_obj();
}

// MeasurementReportSidelink-r16-IEs ::= SEQUENCE
OCUDUASN_CODE meas_report_sidelink_r16_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  HANDLE_CODE(sl_meas_results_r16.pack(bref));
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_report_sidelink_r16_ies_s::unpack(cbit_ref& bref)
{
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  HANDLE_CODE(sl_meas_results_r16.unpack(bref));
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void meas_report_sidelink_r16_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-MeasResults-r16");
  sl_meas_results_r16.to_json(j);
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// MeasurementReportSidelink ::= SEQUENCE
OCUDUASN_CODE meas_report_sidelink_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_report_sidelink_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void meas_report_sidelink_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void meas_report_sidelink_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
meas_report_sidelink_r16_ies_s& meas_report_sidelink_s::crit_exts_c_::set_meas_report_sidelink_r16()
{
  set(types::meas_report_sidelink_r16);
  return c;
}
void meas_report_sidelink_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void meas_report_sidelink_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::meas_report_sidelink_r16:
      j.write_fieldname("measurementReportSidelink-r16");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "meas_report_sidelink_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_report_sidelink_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::meas_report_sidelink_r16:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "meas_report_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_report_sidelink_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::meas_report_sidelink_r16:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "meas_report_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_report_sidelink_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"measurementReportSidelink-r16", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "meas_report_sidelink_s::crit_exts_c_::types");
}

// NotificationMessageSidelink-v1800-IEs ::= SEQUENCE
OCUDUASN_CODE notif_msg_sidelink_v1800_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_ind_type_r18_present, 1));
  HANDLE_CODE(bref.pack(sl_dest_id_remote_ue_r18_present, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_ind_type_r18_present) {
    HANDLE_CODE(sl_ind_type_r18.pack(bref));
  }
  if (sl_dest_id_remote_ue_r18_present) {
    HANDLE_CODE(sl_dest_id_remote_ue_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE notif_msg_sidelink_v1800_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sl_ind_type_r18_present, 1));
  HANDLE_CODE(bref.unpack(sl_dest_id_remote_ue_r18_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_ind_type_r18_present) {
    HANDLE_CODE(sl_ind_type_r18.unpack(bref));
  }
  if (sl_dest_id_remote_ue_r18_present) {
    HANDLE_CODE(sl_dest_id_remote_ue_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void notif_msg_sidelink_v1800_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_ind_type_r18_present) {
    j.write_str("sl-IndicationType-r18", sl_ind_type_r18.to_string());
  }
  if (sl_dest_id_remote_ue_r18_present) {
    j.write_str("sl-DestinationIdentityRemoteUE-r18", sl_dest_id_remote_ue_r18.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

const char* notif_msg_sidelink_v1800_ies_s::sl_ind_type_r18_opts::to_string() const
{
  static const char* names[] = {"relayUE-PC5-RLF", "spare1"};
  return convert_enum_idx(names, 2, value, "notif_msg_sidelink_v1800_ies_s::sl_ind_type_r18_e_");
}
uint8_t notif_msg_sidelink_v1800_ies_s::sl_ind_type_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {5};
  return map_enum_number(numbers, 1, value, "notif_msg_sidelink_v1800_ies_s::sl_ind_type_r18_e_");
}

// NotificationMessageSidelink-r17-IEs ::= SEQUENCE
OCUDUASN_CODE notif_msg_sidelink_r17_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ind_type_r17_present, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (ind_type_r17_present) {
    HANDLE_CODE(ind_type_r17.pack(bref));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE notif_msg_sidelink_r17_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ind_type_r17_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (ind_type_r17_present) {
    HANDLE_CODE(ind_type_r17.unpack(bref));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void notif_msg_sidelink_r17_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ind_type_r17_present) {
    j.write_str("indicationType-r17", ind_type_r17.to_string());
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

const char* notif_msg_sidelink_r17_ies_s::ind_type_r17_opts::to_string() const
{
  static const char* names[] = {"relayUE-Uu-RLF", "relayUE-HO", "relayUE-CellReselection", "relayUE-Uu-RRC-Failure"};
  return convert_enum_idx(names, 4, value, "notif_msg_sidelink_r17_ies_s::ind_type_r17_e_");
}

// NotificationMessageSidelink-r17 ::= SEQUENCE
OCUDUASN_CODE notif_msg_sidelink_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE notif_msg_sidelink_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void notif_msg_sidelink_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void notif_msg_sidelink_r17_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
notif_msg_sidelink_r17_ies_s& notif_msg_sidelink_r17_s::crit_exts_c_::set_notif_msg_sidelink_r17()
{
  set(types::notif_msg_sidelink_r17);
  return c;
}
void notif_msg_sidelink_r17_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void notif_msg_sidelink_r17_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      j.write_fieldname("notificationMessageSidelink-r17");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "notif_msg_sidelink_r17_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE notif_msg_sidelink_r17_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "notif_msg_sidelink_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE notif_msg_sidelink_r17_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "notif_msg_sidelink_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* notif_msg_sidelink_r17_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"notificationMessageSidelink-r17", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "notif_msg_sidelink_r17_s::crit_exts_c_::types");
}

// PDCP-ParametersSidelink-r16 ::= SEQUENCE
OCUDUASN_CODE pdcp_params_sidelink_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= pdcp_dupl_srb_sidelink_r18_present;
  group_flags[0] |= pdcp_dupl_drb_sidelink_r18_present;
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(bref.pack(out_of_order_delivery_sidelink_r16_present, 1));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(pdcp_dupl_srb_sidelink_r18_present, 1));
      HANDLE_CODE(bref.pack(pdcp_dupl_drb_sidelink_r18_present, 1));
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcp_params_sidelink_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(out_of_order_delivery_sidelink_r16_present, 1));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(pdcp_dupl_srb_sidelink_r18_present, 1));
      HANDLE_CODE(bref.unpack(pdcp_dupl_drb_sidelink_r18_present, 1));
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdcp_params_sidelink_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (out_of_order_delivery_sidelink_r16_present) {
    j.write_str("outOfOrderDeliverySidelink-r16", "supported");
  }
  if (pdcp_dupl_srb_sidelink_r18_present) {
    j.write_str("pdcp-DuplicationSRB-sidelink-r18", "supported");
  }
  if (pdcp_dupl_drb_sidelink_r18_present) {
    j.write_str("pdcp-DuplicationDRB-sidelink-r18", "supported");
  }
  j.end_obj();
}

// RRCReconfigurationCompleteSidelink-v1720-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_complete_sidelink_v1720_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_drx_cfg_reject_v1720_present, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_complete_sidelink_v1720_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sl_drx_cfg_reject_v1720_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_complete_sidelink_v1720_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_drx_cfg_reject_v1720_present) {
    j.write_str("sl-DRX-ConfigReject-v1720", "true");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// RRCReconfigurationCompleteSidelink-v1710-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_complete_sidelink_v1710_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_complete_sidelink_v1710_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_complete_sidelink_v1710_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("dummy", "true");
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// RRCReconfigurationCompleteSidelink-r16-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_complete_sidelink_r16_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_complete_sidelink_r16_ies_s::unpack(cbit_ref& bref)
{
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_complete_sidelink_r16_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// RRCReconfigurationCompleteSidelink ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_complete_sidelink_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, rrc_transaction_id_r16, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_complete_sidelink_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(rrc_transaction_id_r16, bref, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_complete_sidelink_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("rrc-TransactionIdentifier-r16", rrc_transaction_id_r16);
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void rrc_recfg_complete_sidelink_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
rrc_recfg_complete_sidelink_r16_ies_s&
rrc_recfg_complete_sidelink_s::crit_exts_c_::set_rrc_recfg_complete_sidelink_r16()
{
  set(types::rrc_recfg_complete_sidelink_r16);
  return c;
}
void rrc_recfg_complete_sidelink_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void rrc_recfg_complete_sidelink_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::rrc_recfg_complete_sidelink_r16:
      j.write_fieldname("rrcReconfigurationCompleteSidelink-r16");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_complete_sidelink_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE rrc_recfg_complete_sidelink_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::rrc_recfg_complete_sidelink_r16:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_complete_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_complete_sidelink_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::rrc_recfg_complete_sidelink_r16:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_complete_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* rrc_recfg_complete_sidelink_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"rrcReconfigurationCompleteSidelink-r16", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "rrc_recfg_complete_sidelink_s::crit_exts_c_::types");
}

// RRCReconfigurationFailureSidelink-r16-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_fail_sidelink_r16_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_fail_sidelink_r16_ies_s::unpack(cbit_ref& bref)
{
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_fail_sidelink_r16_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// RRCReconfigurationFailureSidelink ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_fail_sidelink_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, rrc_transaction_id_r16, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_fail_sidelink_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(rrc_transaction_id_r16, bref, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_fail_sidelink_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("rrc-TransactionIdentifier-r16", rrc_transaction_id_r16);
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void rrc_recfg_fail_sidelink_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
rrc_recfg_fail_sidelink_r16_ies_s& rrc_recfg_fail_sidelink_s::crit_exts_c_::set_rrc_recfg_fail_sidelink_r16()
{
  set(types::rrc_recfg_fail_sidelink_r16);
  return c;
}
void rrc_recfg_fail_sidelink_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void rrc_recfg_fail_sidelink_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::rrc_recfg_fail_sidelink_r16:
      j.write_fieldname("rrcReconfigurationFailureSidelink-r16");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_fail_sidelink_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE rrc_recfg_fail_sidelink_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::rrc_recfg_fail_sidelink_r16:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_fail_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_fail_sidelink_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::rrc_recfg_fail_sidelink_r16:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_fail_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* rrc_recfg_fail_sidelink_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"rrcReconfigurationFailureSidelink-r16", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "rrc_recfg_fail_sidelink_s::crit_exts_c_::types");
}

// SL-SDAP-ConfigPC5-r16 ::= SEQUENCE
OCUDUASN_CODE sl_sdap_cfg_pc5_r16_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_mapped_qos_flows_to_add_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_mapped_qos_flows_to_release_list_r16.size() > 0, 1));

  if (sl_mapped_qos_flows_to_add_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_mapped_qos_flows_to_add_list_r16, 1, 64, integer_packer<uint8_t>(1, 64)));
  }
  if (sl_mapped_qos_flows_to_release_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_mapped_qos_flows_to_release_list_r16, 1, 64, integer_packer<uint8_t>(1, 64)));
  }
  HANDLE_CODE(sl_sdap_hdr_r16.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_sdap_cfg_pc5_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool sl_mapped_qos_flows_to_add_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_mapped_qos_flows_to_add_list_r16_present, 1));
  bool sl_mapped_qos_flows_to_release_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_mapped_qos_flows_to_release_list_r16_present, 1));

  if (sl_mapped_qos_flows_to_add_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_mapped_qos_flows_to_add_list_r16, bref, 1, 64, integer_packer<uint8_t>(1, 64)));
  }
  if (sl_mapped_qos_flows_to_release_list_r16_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(sl_mapped_qos_flows_to_release_list_r16, bref, 1, 64, integer_packer<uint8_t>(1, 64)));
  }
  HANDLE_CODE(sl_sdap_hdr_r16.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_sdap_cfg_pc5_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_mapped_qos_flows_to_add_list_r16.size() > 0) {
    j.start_array("sl-MappedQoS-FlowsToAddList-r16");
    for (const auto& e1 : sl_mapped_qos_flows_to_add_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_mapped_qos_flows_to_release_list_r16.size() > 0) {
    j.start_array("sl-MappedQoS-FlowsToReleaseList-r16");
    for (const auto& e1 : sl_mapped_qos_flows_to_release_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  j.write_str("sl-SDAP-Header-r16", sl_sdap_hdr_r16.to_string());
  j.end_obj();
}

const char* sl_sdap_cfg_pc5_r16_s::sl_sdap_hdr_r16_opts::to_string() const
{
  static const char* names[] = {"present", "absent"};
  return convert_enum_idx(names, 2, value, "sl_sdap_cfg_pc5_r16_s::sl_sdap_hdr_r16_e_");
}

// SL-PDCP-ConfigPC5-r16 ::= SEQUENCE
OCUDUASN_CODE sl_pdcp_cfg_pc5_r16_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_pdcp_sn_size_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_out_of_order_delivery_r16_present, 1));

  if (sl_pdcp_sn_size_r16_present) {
    HANDLE_CODE(sl_pdcp_sn_size_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_pdcp_cfg_pc5_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_pdcp_sn_size_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_out_of_order_delivery_r16_present, 1));

  if (sl_pdcp_sn_size_r16_present) {
    HANDLE_CODE(sl_pdcp_sn_size_r16.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_pdcp_cfg_pc5_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_pdcp_sn_size_r16_present) {
    j.write_str("sl-PDCP-SN-Size-r16", sl_pdcp_sn_size_r16.to_string());
  }
  if (sl_out_of_order_delivery_r16_present) {
    j.write_str("sl-OutOfOrderDelivery-r16", "true");
  }
  j.end_obj();
}

const char* sl_pdcp_cfg_pc5_r16_s::sl_pdcp_sn_size_r16_opts::to_string() const
{
  static const char* names[] = {"len12bits", "len18bits"};
  return convert_enum_idx(names, 2, value, "sl_pdcp_cfg_pc5_r16_s::sl_pdcp_sn_size_r16_e_");
}
uint8_t sl_pdcp_cfg_pc5_r16_s::sl_pdcp_sn_size_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {12, 18};
  return map_enum_number(numbers, 2, value, "sl_pdcp_cfg_pc5_r16_s::sl_pdcp_sn_size_r16_e_");
}

// SL-RLC-ConfigPC5-r16 ::= CHOICE
void sl_rlc_cfg_pc5_r16_c::destroy_()
{
  switch (type_) {
    case types::sl_am_rlc_r16:
      c.destroy<sl_am_rlc_r16_s_>();
      break;
    case types::sl_um_bi_dir_rlc_r16:
      c.destroy<sl_um_bi_dir_rlc_r16_s_>();
      break;
    case types::sl_um_uni_dir_rlc_r16:
      c.destroy<sl_um_uni_dir_rlc_r16_s_>();
      break;
    default:
      break;
  }
}
void sl_rlc_cfg_pc5_r16_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::sl_am_rlc_r16:
      c.init<sl_am_rlc_r16_s_>();
      break;
    case types::sl_um_bi_dir_rlc_r16:
      c.init<sl_um_bi_dir_rlc_r16_s_>();
      break;
    case types::sl_um_uni_dir_rlc_r16:
      c.init<sl_um_uni_dir_rlc_r16_s_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_cfg_pc5_r16_c");
  }
}
sl_rlc_cfg_pc5_r16_c::sl_rlc_cfg_pc5_r16_c(const sl_rlc_cfg_pc5_r16_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::sl_am_rlc_r16:
      c.init(other.c.get<sl_am_rlc_r16_s_>());
      break;
    case types::sl_um_bi_dir_rlc_r16:
      c.init(other.c.get<sl_um_bi_dir_rlc_r16_s_>());
      break;
    case types::sl_um_uni_dir_rlc_r16:
      c.init(other.c.get<sl_um_uni_dir_rlc_r16_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_cfg_pc5_r16_c");
  }
}
sl_rlc_cfg_pc5_r16_c& sl_rlc_cfg_pc5_r16_c::operator=(const sl_rlc_cfg_pc5_r16_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::sl_am_rlc_r16:
      c.set(other.c.get<sl_am_rlc_r16_s_>());
      break;
    case types::sl_um_bi_dir_rlc_r16:
      c.set(other.c.get<sl_um_bi_dir_rlc_r16_s_>());
      break;
    case types::sl_um_uni_dir_rlc_r16:
      c.set(other.c.get<sl_um_uni_dir_rlc_r16_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_cfg_pc5_r16_c");
  }

  return *this;
}
sl_rlc_cfg_pc5_r16_c::sl_am_rlc_r16_s_& sl_rlc_cfg_pc5_r16_c::set_sl_am_rlc_r16()
{
  set(types::sl_am_rlc_r16);
  return c.get<sl_am_rlc_r16_s_>();
}
sl_rlc_cfg_pc5_r16_c::sl_um_bi_dir_rlc_r16_s_& sl_rlc_cfg_pc5_r16_c::set_sl_um_bi_dir_rlc_r16()
{
  set(types::sl_um_bi_dir_rlc_r16);
  return c.get<sl_um_bi_dir_rlc_r16_s_>();
}
sl_rlc_cfg_pc5_r16_c::sl_um_uni_dir_rlc_r16_s_& sl_rlc_cfg_pc5_r16_c::set_sl_um_uni_dir_rlc_r16()
{
  set(types::sl_um_uni_dir_rlc_r16);
  return c.get<sl_um_uni_dir_rlc_r16_s_>();
}
void sl_rlc_cfg_pc5_r16_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::sl_am_rlc_r16:
      j.write_fieldname("sl-AM-RLC-r16");
      j.start_obj();
      if (c.get<sl_am_rlc_r16_s_>().sl_sn_field_len_am_r16_present) {
        j.write_str("sl-SN-FieldLengthAM-r16", c.get<sl_am_rlc_r16_s_>().sl_sn_field_len_am_r16.to_string());
      }
      j.end_obj();
      break;
    case types::sl_um_bi_dir_rlc_r16:
      j.write_fieldname("sl-UM-Bi-Directional-RLC-r16");
      j.start_obj();
      if (c.get<sl_um_bi_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present) {
        j.write_str("sl-SN-FieldLengthUM-r16", c.get<sl_um_bi_dir_rlc_r16_s_>().sl_sn_field_len_um_r16.to_string());
      }
      j.end_obj();
      break;
    case types::sl_um_uni_dir_rlc_r16:
      j.write_fieldname("sl-UM-Uni-Directional-RLC-r16");
      j.start_obj();
      if (c.get<sl_um_uni_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present) {
        j.write_str("sl-SN-FieldLengthUM-r16", c.get<sl_um_uni_dir_rlc_r16_s_>().sl_sn_field_len_um_r16.to_string());
      }
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_cfg_pc5_r16_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sl_rlc_cfg_pc5_r16_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::sl_am_rlc_r16:
      bref.pack(false, 1);
      HANDLE_CODE(bref.pack(c.get<sl_am_rlc_r16_s_>().sl_sn_field_len_am_r16_present, 1));
      if (c.get<sl_am_rlc_r16_s_>().sl_sn_field_len_am_r16_present) {
        HANDLE_CODE(c.get<sl_am_rlc_r16_s_>().sl_sn_field_len_am_r16.pack(bref));
      }
      break;
    case types::sl_um_bi_dir_rlc_r16:
      bref.pack(false, 1);
      HANDLE_CODE(bref.pack(c.get<sl_um_bi_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present, 1));
      if (c.get<sl_um_bi_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present) {
        HANDLE_CODE(c.get<sl_um_bi_dir_rlc_r16_s_>().sl_sn_field_len_um_r16.pack(bref));
      }
      break;
    case types::sl_um_uni_dir_rlc_r16:
      bref.pack(false, 1);
      HANDLE_CODE(bref.pack(c.get<sl_um_uni_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present, 1));
      if (c.get<sl_um_uni_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present) {
        HANDLE_CODE(c.get<sl_um_uni_dir_rlc_r16_s_>().sl_sn_field_len_um_r16.pack(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_cfg_pc5_r16_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_rlc_cfg_pc5_r16_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::sl_am_rlc_r16:
      bool c_get_sl_am_rlc_r16_s_ext;
      HANDLE_CODE(bref.unpack(c_get_sl_am_rlc_r16_s_ext, 1));
      HANDLE_CODE(bref.unpack(c.get<sl_am_rlc_r16_s_>().sl_sn_field_len_am_r16_present, 1));
      if (c.get<sl_am_rlc_r16_s_>().sl_sn_field_len_am_r16_present) {
        HANDLE_CODE(c.get<sl_am_rlc_r16_s_>().sl_sn_field_len_am_r16.unpack(bref));
      }
      if (c_get_sl_am_rlc_r16_s_ext) {
        ext_groups_unpacker unknown_ext_unpacker(bref);
        HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
      }
      break;
    case types::sl_um_bi_dir_rlc_r16:
      bool c_get_sl_um_bi_dir_rlc_r16_s_ext;
      HANDLE_CODE(bref.unpack(c_get_sl_um_bi_dir_rlc_r16_s_ext, 1));
      HANDLE_CODE(bref.unpack(c.get<sl_um_bi_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present, 1));
      if (c.get<sl_um_bi_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present) {
        HANDLE_CODE(c.get<sl_um_bi_dir_rlc_r16_s_>().sl_sn_field_len_um_r16.unpack(bref));
      }
      if (c_get_sl_um_bi_dir_rlc_r16_s_ext) {
        ext_groups_unpacker unknown_ext_unpacker(bref);
        HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
      }
      break;
    case types::sl_um_uni_dir_rlc_r16:
      bool c_get_sl_um_uni_dir_rlc_r16_s_ext;
      HANDLE_CODE(bref.unpack(c_get_sl_um_uni_dir_rlc_r16_s_ext, 1));
      HANDLE_CODE(bref.unpack(c.get<sl_um_uni_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present, 1));
      if (c.get<sl_um_uni_dir_rlc_r16_s_>().sl_sn_field_len_um_r16_present) {
        HANDLE_CODE(c.get<sl_um_uni_dir_rlc_r16_s_>().sl_sn_field_len_um_r16.unpack(bref));
      }
      if (c_get_sl_um_uni_dir_rlc_r16_s_ext) {
        ext_groups_unpacker unknown_ext_unpacker(bref);
        HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_cfg_pc5_r16_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sl_rlc_cfg_pc5_r16_c::types_opts::to_string() const
{
  static const char* names[] = {"sl-AM-RLC-r16", "sl-UM-Bi-Directional-RLC-r16", "sl-UM-Uni-Directional-RLC-r16"};
  return convert_enum_idx(names, 3, value, "sl_rlc_cfg_pc5_r16_c::types");
}

// SL-LogicalChannelConfigPC5-r16 ::= SEQUENCE
OCUDUASN_CODE sl_lc_ch_cfg_pc5_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= sl_lc_ch_id_v1800_present;
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(pack_integer(bref, sl_lc_ch_id_r16, (uint8_t)1u, (uint8_t)32u));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_lc_ch_id_v1800_present, 1));
      if (sl_lc_ch_id_v1800_present) {
        HANDLE_CODE(pack_integer(bref, sl_lc_ch_id_v1800, (uint8_t)33u, (uint8_t)38u));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_lc_ch_cfg_pc5_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(sl_lc_ch_id_r16, bref, (uint8_t)1u, (uint8_t)32u));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(sl_lc_ch_id_v1800_present, 1));
      if (sl_lc_ch_id_v1800_present) {
        HANDLE_CODE(unpack_integer(sl_lc_ch_id_v1800, bref, (uint8_t)33u, (uint8_t)38u));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_lc_ch_cfg_pc5_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sl-LogicalChannelIdentity-r16", sl_lc_ch_id_r16);
  if (sl_lc_ch_id_v1800_present) {
    j.write_int("sl-LogicalChannelIdentity-v1800", sl_lc_ch_id_v1800);
  }
  j.end_obj();
}

// SLRB-Config-r16 ::= SEQUENCE
OCUDUASN_CODE slrb_cfg_r16_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_sdap_cfg_pc5_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_pdcp_cfg_pc5_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_rlc_cfg_pc5_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_mac_lc_ch_cfg_pc5_r16_present, 1));

  HANDLE_CODE(pack_integer(bref, slrb_pc5_cfg_idx_r16, (uint16_t)1u, (uint16_t)512u));
  if (sl_sdap_cfg_pc5_r16_present) {
    HANDLE_CODE(sl_sdap_cfg_pc5_r16.pack(bref));
  }
  if (sl_pdcp_cfg_pc5_r16_present) {
    HANDLE_CODE(sl_pdcp_cfg_pc5_r16.pack(bref));
  }
  if (sl_rlc_cfg_pc5_r16_present) {
    HANDLE_CODE(sl_rlc_cfg_pc5_r16.pack(bref));
  }
  if (sl_mac_lc_ch_cfg_pc5_r16_present) {
    HANDLE_CODE(sl_mac_lc_ch_cfg_pc5_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE slrb_cfg_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_sdap_cfg_pc5_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_pdcp_cfg_pc5_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_rlc_cfg_pc5_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_mac_lc_ch_cfg_pc5_r16_present, 1));

  HANDLE_CODE(unpack_integer(slrb_pc5_cfg_idx_r16, bref, (uint16_t)1u, (uint16_t)512u));
  if (sl_sdap_cfg_pc5_r16_present) {
    HANDLE_CODE(sl_sdap_cfg_pc5_r16.unpack(bref));
  }
  if (sl_pdcp_cfg_pc5_r16_present) {
    HANDLE_CODE(sl_pdcp_cfg_pc5_r16.unpack(bref));
  }
  if (sl_rlc_cfg_pc5_r16_present) {
    HANDLE_CODE(sl_rlc_cfg_pc5_r16.unpack(bref));
  }
  if (sl_mac_lc_ch_cfg_pc5_r16_present) {
    HANDLE_CODE(sl_mac_lc_ch_cfg_pc5_r16.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void slrb_cfg_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("slrb-PC5-ConfigIndex-r16", slrb_pc5_cfg_idx_r16);
  if (sl_sdap_cfg_pc5_r16_present) {
    j.write_fieldname("sl-SDAP-ConfigPC5-r16");
    sl_sdap_cfg_pc5_r16.to_json(j);
  }
  if (sl_pdcp_cfg_pc5_r16_present) {
    j.write_fieldname("sl-PDCP-ConfigPC5-r16");
    sl_pdcp_cfg_pc5_r16.to_json(j);
  }
  if (sl_rlc_cfg_pc5_r16_present) {
    j.write_fieldname("sl-RLC-ConfigPC5-r16");
    sl_rlc_cfg_pc5_r16.to_json(j);
  }
  if (sl_mac_lc_ch_cfg_pc5_r16_present) {
    j.write_fieldname("sl-MAC-LogicalChannelConfigPC5-r16");
    sl_mac_lc_ch_cfg_pc5_r16.to_json(j);
  }
  j.end_obj();
}

// SL-CSI-RS-Config-r16 ::= SEQUENCE
OCUDUASN_CODE sl_csi_rs_cfg_r16_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_csi_rs_freq_alloc_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_csi_rs_first_symbol_r16_present, 1));

  if (sl_csi_rs_freq_alloc_r16_present) {
    HANDLE_CODE(sl_csi_rs_freq_alloc_r16.pack(bref));
  }
  if (sl_csi_rs_first_symbol_r16_present) {
    HANDLE_CODE(pack_integer(bref, sl_csi_rs_first_symbol_r16, (uint8_t)3u, (uint8_t)12u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_csi_rs_cfg_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_csi_rs_freq_alloc_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_csi_rs_first_symbol_r16_present, 1));

  if (sl_csi_rs_freq_alloc_r16_present) {
    HANDLE_CODE(sl_csi_rs_freq_alloc_r16.unpack(bref));
  }
  if (sl_csi_rs_first_symbol_r16_present) {
    HANDLE_CODE(unpack_integer(sl_csi_rs_first_symbol_r16, bref, (uint8_t)3u, (uint8_t)12u));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_csi_rs_cfg_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_csi_rs_freq_alloc_r16_present) {
    j.write_fieldname("sl-CSI-RS-FreqAllocation-r16");
    sl_csi_rs_freq_alloc_r16.to_json(j);
  }
  if (sl_csi_rs_first_symbol_r16_present) {
    j.write_int("sl-CSI-RS-FirstSymbol-r16", sl_csi_rs_first_symbol_r16);
  }
  j.end_obj();
}

void sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::destroy_()
{
  switch (type_) {
    case types::sl_one_ant_port_r16:
      c.destroy<fixed_bitstring<12>>();
      break;
    case types::sl_two_ant_port_r16:
      c.destroy<fixed_bitstring<6>>();
      break;
    default:
      break;
  }
}
void sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::sl_one_ant_port_r16:
      c.init<fixed_bitstring<12>>();
      break;
    case types::sl_two_ant_port_r16:
      c.init<fixed_bitstring<6>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_");
  }
}
sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::sl_csi_rs_freq_alloc_r16_c_(
    const sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::sl_one_ant_port_r16:
      c.init(other.c.get<fixed_bitstring<12>>());
      break;
    case types::sl_two_ant_port_r16:
      c.init(other.c.get<fixed_bitstring<6>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_");
  }
}
sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_& sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::operator=(
    const sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::sl_one_ant_port_r16:
      c.set(other.c.get<fixed_bitstring<12>>());
      break;
    case types::sl_two_ant_port_r16:
      c.set(other.c.get<fixed_bitstring<6>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_");
  }

  return *this;
}
fixed_bitstring<12>& sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::set_sl_one_ant_port_r16()
{
  set(types::sl_one_ant_port_r16);
  return c.get<fixed_bitstring<12>>();
}
fixed_bitstring<6>& sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::set_sl_two_ant_port_r16()
{
  set(types::sl_two_ant_port_r16);
  return c.get<fixed_bitstring<6>>();
}
void sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::sl_one_ant_port_r16:
      j.write_str("sl-OneAntennaPort-r16", c.get<fixed_bitstring<12>>().to_string());
      break;
    case types::sl_two_ant_port_r16:
      j.write_str("sl-TwoAntennaPort-r16", c.get<fixed_bitstring<6>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::sl_one_ant_port_r16:
      HANDLE_CODE(c.get<fixed_bitstring<12>>().pack(bref));
      break;
    case types::sl_two_ant_port_r16:
      HANDLE_CODE(c.get<fixed_bitstring<6>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::sl_one_ant_port_r16:
      HANDLE_CODE(c.get<fixed_bitstring<12>>().unpack(bref));
      break;
    case types::sl_two_ant_port_r16:
      HANDLE_CODE(c.get<fixed_bitstring<6>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::types_opts::to_string() const
{
  static const char* names[] = {"sl-OneAntennaPort-r16", "sl-TwoAntennaPort-r16"};
  return convert_enum_idx(names, 2, value, "sl_csi_rs_cfg_r16_s::sl_csi_rs_freq_alloc_r16_c_::types");
}

// SL-RLC-ChannelConfigPC5-r17 ::= SEQUENCE
OCUDUASN_CODE sl_rlc_ch_cfg_pc5_r17_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_rlc_cfg_pc5_r17_present, 1));
  HANDLE_CODE(bref.pack(sl_mac_lc_ch_cfg_pc5_r17_present, 1));

  HANDLE_CODE(pack_integer(bref, sl_rlc_ch_id_pc5_r17, (uint16_t)1u, (uint16_t)512u));
  if (sl_rlc_cfg_pc5_r17_present) {
    HANDLE_CODE(sl_rlc_cfg_pc5_r17.pack(bref));
  }
  if (sl_mac_lc_ch_cfg_pc5_r17_present) {
    HANDLE_CODE(sl_mac_lc_ch_cfg_pc5_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_rlc_ch_cfg_pc5_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_rlc_cfg_pc5_r17_present, 1));
  HANDLE_CODE(bref.unpack(sl_mac_lc_ch_cfg_pc5_r17_present, 1));

  HANDLE_CODE(unpack_integer(sl_rlc_ch_id_pc5_r17, bref, (uint16_t)1u, (uint16_t)512u));
  if (sl_rlc_cfg_pc5_r17_present) {
    HANDLE_CODE(sl_rlc_cfg_pc5_r17.unpack(bref));
  }
  if (sl_mac_lc_ch_cfg_pc5_r17_present) {
    HANDLE_CODE(sl_mac_lc_ch_cfg_pc5_r17.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_rlc_ch_cfg_pc5_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sl-RLC-ChannelID-PC5-r17", sl_rlc_ch_id_pc5_r17);
  if (sl_rlc_cfg_pc5_r17_present) {
    j.write_fieldname("sl-RLC-ConfigPC5-r17");
    sl_rlc_cfg_pc5_r17.to_json(j);
  }
  if (sl_mac_lc_ch_cfg_pc5_r17_present) {
    j.write_fieldname("sl-MAC-LogicalChannelConfigPC5-r17");
    sl_mac_lc_ch_cfg_pc5_r17.to_json(j);
  }
  j.end_obj();
}

// SL-SFN-DFN-Offset-r18 ::= SEQUENCE
OCUDUASN_CODE sl_sfn_dfn_offset_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, sl_frame_offset_r18, (uint16_t)0u, (uint16_t)1023u));
  HANDLE_CODE(pack_integer(bref, sl_sf_offset_r18, (uint8_t)0u, (uint8_t)9u));
  HANDLE_CODE(pack_integer(bref, sl_slot_offset_r18, (uint8_t)0u, (uint8_t)31u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_sfn_dfn_offset_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(sl_frame_offset_r18, bref, (uint16_t)0u, (uint16_t)1023u));
  HANDLE_CODE(unpack_integer(sl_sf_offset_r18, bref, (uint8_t)0u, (uint8_t)9u));
  HANDLE_CODE(unpack_integer(sl_slot_offset_r18, bref, (uint8_t)0u, (uint8_t)31u));

  return OCUDUASN_SUCCESS;
}
void sl_sfn_dfn_offset_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sl-FrameOffset-r18", sl_frame_offset_r18);
  j.write_int("sl-SubframeOffset-r18", sl_sf_offset_r18);
  j.write_int("sl-SlotOffset-r18", sl_slot_offset_r18);
  j.end_obj();
}

// SL-CarrierConfig-r18 ::= SEQUENCE
OCUDUASN_CODE sl_carrier_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, sl_carrier_id_r18, (uint8_t)1u, (uint8_t)7u));
  HANDLE_CODE(pack_integer(bref, sl_offset_to_carrier_r18, (uint16_t)0u, (uint16_t)2199u));
  HANDLE_CODE(subcarrier_spacing_r18.pack(bref));
  HANDLE_CODE(pack_integer(bref, carrier_bw_r18, (uint16_t)1u, (uint16_t)275u));
  HANDLE_CODE(pack_integer(bref, sl_absolute_freq_point_a_r18, (uint32_t)0u, (uint32_t)3279165u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_carrier_cfg_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(sl_carrier_id_r18, bref, (uint8_t)1u, (uint8_t)7u));
  HANDLE_CODE(unpack_integer(sl_offset_to_carrier_r18, bref, (uint16_t)0u, (uint16_t)2199u));
  HANDLE_CODE(subcarrier_spacing_r18.unpack(bref));
  HANDLE_CODE(unpack_integer(carrier_bw_r18, bref, (uint16_t)1u, (uint16_t)275u));
  HANDLE_CODE(unpack_integer(sl_absolute_freq_point_a_r18, bref, (uint32_t)0u, (uint32_t)3279165u));

  return OCUDUASN_SUCCESS;
}
void sl_carrier_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sl-CarrierId-r18", sl_carrier_id_r18);
  j.write_int("sl-OffsetToCarrier-r18", sl_offset_to_carrier_r18);
  j.write_str("subcarrierSpacing-r18", subcarrier_spacing_r18.to_string());
  j.write_int("carrierBandwidth-r18", carrier_bw_r18);
  j.write_int("sl-AbsoluteFrequencyPointA-r18", sl_absolute_freq_point_a_r18);
  j.end_obj();
}

// SL-RLC-BearerConfig-r18 ::= CHOICE
void sl_rlc_bearer_cfg_r18_c::destroy_()
{
  switch (type_) {
    case types::srb:
      c.destroy<srb_s_>();
      break;
    case types::drb:
      c.destroy<drb_s_>();
      break;
    default:
      break;
  }
}
void sl_rlc_bearer_cfg_r18_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::srb:
      c.init<srb_s_>();
      break;
    case types::drb:
      c.init<drb_s_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_bearer_cfg_r18_c");
  }
}
sl_rlc_bearer_cfg_r18_c::sl_rlc_bearer_cfg_r18_c(const sl_rlc_bearer_cfg_r18_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::srb:
      c.init(other.c.get<srb_s_>());
      break;
    case types::drb:
      c.init(other.c.get<drb_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_bearer_cfg_r18_c");
  }
}
sl_rlc_bearer_cfg_r18_c& sl_rlc_bearer_cfg_r18_c::operator=(const sl_rlc_bearer_cfg_r18_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::srb:
      c.set(other.c.get<srb_s_>());
      break;
    case types::drb:
      c.set(other.c.get<drb_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_bearer_cfg_r18_c");
  }

  return *this;
}
sl_rlc_bearer_cfg_r18_c::srb_s_& sl_rlc_bearer_cfg_r18_c::set_srb()
{
  set(types::srb);
  return c.get<srb_s_>();
}
sl_rlc_bearer_cfg_r18_c::drb_s_& sl_rlc_bearer_cfg_r18_c::set_drb()
{
  set(types::drb);
  return c.get<drb_s_>();
}
void sl_rlc_bearer_cfg_r18_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srb:
      j.write_fieldname("srb");
      j.start_obj();
      j.write_int("sl-SRB-IdentityWithDuplication", c.get<srb_s_>().sl_srb_id_with_dupl);
      j.write_int("sL-RLC-BearerConfigIndex-r18", c.get<srb_s_>().sl_rlc_bearer_cfg_idx_r18);
      j.end_obj();
      break;
    case types::drb:
      j.write_fieldname("drb");
      j.start_obj();
      j.write_int("slrb-PC5-ConfigIndex-r18", c.get<drb_s_>().slrb_pc5_cfg_idx_r18);
      j.write_int("sL-RLC-BearerConfigIndex-r18", c.get<drb_s_>().sl_rlc_bearer_cfg_idx_r18);
      if (c.get<drb_s_>().sl_rlc_cfg_pc5_r18_present) {
        j.write_fieldname("sl-RLC-ConfigPC5-r18");
        c.get<drb_s_>().sl_rlc_cfg_pc5_r18.to_json(j);
      }
      if (c.get<drb_s_>().sl_mac_lc_ch_cfg_pc5_r18_present) {
        j.write_fieldname("sl-MAC-LogicalChannelConfigPC5-r18");
        c.get<drb_s_>().sl_mac_lc_ch_cfg_pc5_r18.to_json(j);
      }
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_bearer_cfg_r18_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sl_rlc_bearer_cfg_r18_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::srb:
      bref.pack(false, 1);
      HANDLE_CODE(pack_integer(bref, c.get<srb_s_>().sl_srb_id_with_dupl, (uint8_t)1u, (uint8_t)3u));
      HANDLE_CODE(pack_integer(bref, c.get<srb_s_>().sl_rlc_bearer_cfg_idx_r18, (uint16_t)1u, (uint16_t)512u));
      break;
    case types::drb:
      bref.pack(false, 1);
      HANDLE_CODE(bref.pack(c.get<drb_s_>().sl_rlc_cfg_pc5_r18_present, 1));
      HANDLE_CODE(bref.pack(c.get<drb_s_>().sl_mac_lc_ch_cfg_pc5_r18_present, 1));
      HANDLE_CODE(pack_integer(bref, c.get<drb_s_>().slrb_pc5_cfg_idx_r18, (uint16_t)1u, (uint16_t)512u));
      HANDLE_CODE(pack_integer(bref, c.get<drb_s_>().sl_rlc_bearer_cfg_idx_r18, (uint16_t)1u, (uint16_t)512u));
      if (c.get<drb_s_>().sl_rlc_cfg_pc5_r18_present) {
        HANDLE_CODE(c.get<drb_s_>().sl_rlc_cfg_pc5_r18.pack(bref));
      }
      if (c.get<drb_s_>().sl_mac_lc_ch_cfg_pc5_r18_present) {
        HANDLE_CODE(c.get<drb_s_>().sl_mac_lc_ch_cfg_pc5_r18.pack(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_bearer_cfg_r18_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_rlc_bearer_cfg_r18_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::srb:
      bool c_get_srb_s_ext;
      HANDLE_CODE(bref.unpack(c_get_srb_s_ext, 1));
      HANDLE_CODE(unpack_integer(c.get<srb_s_>().sl_srb_id_with_dupl, bref, (uint8_t)1u, (uint8_t)3u));
      HANDLE_CODE(unpack_integer(c.get<srb_s_>().sl_rlc_bearer_cfg_idx_r18, bref, (uint16_t)1u, (uint16_t)512u));
      if (c_get_srb_s_ext) {
        ext_groups_unpacker unknown_ext_unpacker(bref);
        HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
      }
      break;
    case types::drb:
      bool c_get_drb_s_ext;
      HANDLE_CODE(bref.unpack(c_get_drb_s_ext, 1));
      HANDLE_CODE(bref.unpack(c.get<drb_s_>().sl_rlc_cfg_pc5_r18_present, 1));
      HANDLE_CODE(bref.unpack(c.get<drb_s_>().sl_mac_lc_ch_cfg_pc5_r18_present, 1));
      HANDLE_CODE(unpack_integer(c.get<drb_s_>().slrb_pc5_cfg_idx_r18, bref, (uint16_t)1u, (uint16_t)512u));
      HANDLE_CODE(unpack_integer(c.get<drb_s_>().sl_rlc_bearer_cfg_idx_r18, bref, (uint16_t)1u, (uint16_t)512u));
      if (c.get<drb_s_>().sl_rlc_cfg_pc5_r18_present) {
        HANDLE_CODE(c.get<drb_s_>().sl_rlc_cfg_pc5_r18.unpack(bref));
      }
      if (c.get<drb_s_>().sl_mac_lc_ch_cfg_pc5_r18_present) {
        HANDLE_CODE(c.get<drb_s_>().sl_mac_lc_ch_cfg_pc5_r18.unpack(bref));
      }
      if (c_get_drb_s_ext) {
        ext_groups_unpacker unknown_ext_unpacker(bref);
        HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_, "sl_rlc_bearer_cfg_r18_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sl_rlc_bearer_cfg_r18_c::types_opts::to_string() const
{
  static const char* names[] = {"srb", "drb"};
  return convert_enum_idx(names, 2, value, "sl_rlc_bearer_cfg_r18_c::types");
}

// SL-SRAP-ConfigPC5-r18 ::= SEQUENCE
OCUDUASN_CODE sl_srap_cfg_pc5_r18_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_peer_remote_ue_l2_id_r18_present, 1));
  HANDLE_CODE(bref.pack(sl_peer_remote_ue_local_id_r18_present, 1));
  HANDLE_CODE(bref.pack(sl_remote_ue_l2_id_r18_present, 1));
  HANDLE_CODE(bref.pack(sl_remote_ue_local_id_r18_present, 1));

  if (sl_peer_remote_ue_l2_id_r18_present) {
    HANDLE_CODE(sl_peer_remote_ue_l2_id_r18.pack(bref));
  }
  if (sl_peer_remote_ue_local_id_r18_present) {
    HANDLE_CODE(pack_integer(bref, sl_peer_remote_ue_local_id_r18, (uint16_t)0u, (uint16_t)255u));
  }
  if (sl_remote_ue_l2_id_r18_present) {
    HANDLE_CODE(sl_remote_ue_l2_id_r18.pack(bref));
  }
  if (sl_remote_ue_local_id_r18_present) {
    HANDLE_CODE(pack_integer(bref, sl_remote_ue_local_id_r18, (uint16_t)0u, (uint16_t)255u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_srap_cfg_pc5_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_peer_remote_ue_l2_id_r18_present, 1));
  HANDLE_CODE(bref.unpack(sl_peer_remote_ue_local_id_r18_present, 1));
  HANDLE_CODE(bref.unpack(sl_remote_ue_l2_id_r18_present, 1));
  HANDLE_CODE(bref.unpack(sl_remote_ue_local_id_r18_present, 1));

  if (sl_peer_remote_ue_l2_id_r18_present) {
    HANDLE_CODE(sl_peer_remote_ue_l2_id_r18.unpack(bref));
  }
  if (sl_peer_remote_ue_local_id_r18_present) {
    HANDLE_CODE(unpack_integer(sl_peer_remote_ue_local_id_r18, bref, (uint16_t)0u, (uint16_t)255u));
  }
  if (sl_remote_ue_l2_id_r18_present) {
    HANDLE_CODE(sl_remote_ue_l2_id_r18.unpack(bref));
  }
  if (sl_remote_ue_local_id_r18_present) {
    HANDLE_CODE(unpack_integer(sl_remote_ue_local_id_r18, bref, (uint16_t)0u, (uint16_t)255u));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_srap_cfg_pc5_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_peer_remote_ue_l2_id_r18_present) {
    j.write_str("sl-PeerRemoteUE-L2Identity-r18", sl_peer_remote_ue_l2_id_r18.to_string());
  }
  if (sl_peer_remote_ue_local_id_r18_present) {
    j.write_int("sl-PeerRemoteUE-LocalIdentity-r18", sl_peer_remote_ue_local_id_r18);
  }
  if (sl_remote_ue_l2_id_r18_present) {
    j.write_str("sl-RemoteUE-L2Identity-r18", sl_remote_ue_l2_id_r18.to_string());
  }
  if (sl_remote_ue_local_id_r18_present) {
    j.write_int("sl-RemoteUE-LocalIdentity-r18", sl_remote_ue_local_id_r18);
  }
  j.end_obj();
}

// RRCReconfigurationSidelink-v1800-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_sidelink_v1800_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_sfn_dfn_offset_r18_present, 1));
  HANDLE_CODE(bref.pack(sl_carrier_to_add_mod_list_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_carrier_to_release_list_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_rlc_bearer_to_add_mod_list_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_rlc_bearer_to_release_list_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_local_id_pair_list_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_sfn_dfn_offset_r18_present) {
    HANDLE_CODE(sl_sfn_dfn_offset_r18.pack(bref));
  }
  if (sl_carrier_to_add_mod_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_carrier_to_add_mod_list_r18, 1, 7));
  }
  if (sl_carrier_to_release_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_carrier_to_release_list_r18, 1, 7, integer_packer<uint8_t>(1, 7)));
  }
  if (sl_rlc_bearer_to_add_mod_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_rlc_bearer_to_add_mod_list_r18, 1, 512));
  }
  if (sl_rlc_bearer_to_release_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_rlc_bearer_to_release_list_r18, 1, 512, integer_packer<uint16_t>(1, 512)));
  }
  if (sl_local_id_pair_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_local_id_pair_list_r18, 1, 32));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_sidelink_v1800_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sl_sfn_dfn_offset_r18_present, 1));
  bool sl_carrier_to_add_mod_list_r18_present;
  HANDLE_CODE(bref.unpack(sl_carrier_to_add_mod_list_r18_present, 1));
  bool sl_carrier_to_release_list_r18_present;
  HANDLE_CODE(bref.unpack(sl_carrier_to_release_list_r18_present, 1));
  bool sl_rlc_bearer_to_add_mod_list_r18_present;
  HANDLE_CODE(bref.unpack(sl_rlc_bearer_to_add_mod_list_r18_present, 1));
  bool sl_rlc_bearer_to_release_list_r18_present;
  HANDLE_CODE(bref.unpack(sl_rlc_bearer_to_release_list_r18_present, 1));
  bool sl_local_id_pair_list_r18_present;
  HANDLE_CODE(bref.unpack(sl_local_id_pair_list_r18_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_sfn_dfn_offset_r18_present) {
    HANDLE_CODE(sl_sfn_dfn_offset_r18.unpack(bref));
  }
  if (sl_carrier_to_add_mod_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_carrier_to_add_mod_list_r18, bref, 1, 7));
  }
  if (sl_carrier_to_release_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_carrier_to_release_list_r18, bref, 1, 7, integer_packer<uint8_t>(1, 7)));
  }
  if (sl_rlc_bearer_to_add_mod_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_rlc_bearer_to_add_mod_list_r18, bref, 1, 512));
  }
  if (sl_rlc_bearer_to_release_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_rlc_bearer_to_release_list_r18, bref, 1, 512, integer_packer<uint16_t>(1, 512)));
  }
  if (sl_local_id_pair_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_local_id_pair_list_r18, bref, 1, 32));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_sidelink_v1800_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_sfn_dfn_offset_r18_present) {
    j.write_fieldname("sl-SFN-DFN-Offset-r18");
    sl_sfn_dfn_offset_r18.to_json(j);
  }
  if (sl_carrier_to_add_mod_list_r18.size() > 0) {
    j.start_array("sl-CarrierToAddModList-r18");
    for (const auto& e1 : sl_carrier_to_add_mod_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_carrier_to_release_list_r18.size() > 0) {
    j.start_array("sl-CarrierToReleaseList-r18");
    for (const auto& e1 : sl_carrier_to_release_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_rlc_bearer_to_add_mod_list_r18.size() > 0) {
    j.start_array("sl-RLC-BearerToAddModList-r18");
    for (const auto& e1 : sl_rlc_bearer_to_add_mod_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_rlc_bearer_to_release_list_r18.size() > 0) {
    j.start_array("sl-RLC-BearerToReleaseList-r18");
    for (const auto& e1 : sl_rlc_bearer_to_release_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_local_id_pair_list_r18.size() > 0) {
    j.start_array("sl-LocalID-PairList-r18");
    for (const auto& e1 : sl_local_id_pair_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// RRCReconfigurationSidelink-v1700-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_sidelink_v1700_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_drx_cfg_uc_pc5_r17_present, 1));
  HANDLE_CODE(bref.pack(sl_latency_bound_iuc_report_r17_present, 1));
  HANDLE_CODE(bref.pack(sl_rlc_ch_to_release_list_pc5_r17.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_rlc_ch_to_add_mod_list_pc5_r17.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_drx_cfg_uc_pc5_r17_present) {
    HANDLE_CODE(sl_drx_cfg_uc_pc5_r17.pack(bref));
  }
  if (sl_latency_bound_iuc_report_r17_present) {
    HANDLE_CODE(sl_latency_bound_iuc_report_r17.pack(bref));
  }
  if (sl_rlc_ch_to_release_list_pc5_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_rlc_ch_to_release_list_pc5_r17, 1, 512, integer_packer<uint16_t>(1, 512)));
  }
  if (sl_rlc_ch_to_add_mod_list_pc5_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_rlc_ch_to_add_mod_list_pc5_r17, 1, 512));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_sidelink_v1700_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sl_drx_cfg_uc_pc5_r17_present, 1));
  HANDLE_CODE(bref.unpack(sl_latency_bound_iuc_report_r17_present, 1));
  bool sl_rlc_ch_to_release_list_pc5_r17_present;
  HANDLE_CODE(bref.unpack(sl_rlc_ch_to_release_list_pc5_r17_present, 1));
  bool sl_rlc_ch_to_add_mod_list_pc5_r17_present;
  HANDLE_CODE(bref.unpack(sl_rlc_ch_to_add_mod_list_pc5_r17_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_drx_cfg_uc_pc5_r17_present) {
    HANDLE_CODE(sl_drx_cfg_uc_pc5_r17.unpack(bref));
  }
  if (sl_latency_bound_iuc_report_r17_present) {
    HANDLE_CODE(sl_latency_bound_iuc_report_r17.unpack(bref));
  }
  if (sl_rlc_ch_to_release_list_pc5_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_rlc_ch_to_release_list_pc5_r17, bref, 1, 512, integer_packer<uint16_t>(1, 512)));
  }
  if (sl_rlc_ch_to_add_mod_list_pc5_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_rlc_ch_to_add_mod_list_pc5_r17, bref, 1, 512));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_sidelink_v1700_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_drx_cfg_uc_pc5_r17_present) {
    j.write_fieldname("sl-DRX-ConfigUC-PC5-r17");
    sl_drx_cfg_uc_pc5_r17.to_json(j);
  }
  if (sl_latency_bound_iuc_report_r17_present) {
    j.write_fieldname("sl-LatencyBoundIUC-Report-r17");
    sl_latency_bound_iuc_report_r17.to_json(j);
  }
  if (sl_rlc_ch_to_release_list_pc5_r17.size() > 0) {
    j.start_array("sl-RLC-ChannelToReleaseListPC5-r17");
    for (const auto& e1 : sl_rlc_ch_to_release_list_pc5_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_rlc_ch_to_add_mod_list_pc5_r17.size() > 0) {
    j.start_array("sl-RLC-ChannelToAddModListPC5-r17");
    for (const auto& e1 : sl_rlc_ch_to_add_mod_list_pc5_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// RRCReconfigurationSidelink-r16-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_sidelink_r16_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(slrb_cfg_to_add_mod_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(slrb_cfg_to_release_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_meas_cfg_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_csi_rs_cfg_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_reset_cfg_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_latency_bound_csi_report_r16_present, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (slrb_cfg_to_add_mod_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, slrb_cfg_to_add_mod_list_r16, 1, 512));
  }
  if (slrb_cfg_to_release_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, slrb_cfg_to_release_list_r16, 1, 512, integer_packer<uint16_t>(1, 512)));
  }
  if (sl_meas_cfg_r16_present) {
    HANDLE_CODE(sl_meas_cfg_r16.pack(bref));
  }
  if (sl_csi_rs_cfg_r16_present) {
    HANDLE_CODE(sl_csi_rs_cfg_r16.pack(bref));
  }
  if (sl_latency_bound_csi_report_r16_present) {
    HANDLE_CODE(pack_integer(bref, sl_latency_bound_csi_report_r16, (uint8_t)3u, (uint8_t)160u));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_sidelink_r16_ies_s::unpack(cbit_ref& bref)
{
  bool slrb_cfg_to_add_mod_list_r16_present;
  HANDLE_CODE(bref.unpack(slrb_cfg_to_add_mod_list_r16_present, 1));
  bool slrb_cfg_to_release_list_r16_present;
  HANDLE_CODE(bref.unpack(slrb_cfg_to_release_list_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_meas_cfg_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_csi_rs_cfg_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_reset_cfg_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_latency_bound_csi_report_r16_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (slrb_cfg_to_add_mod_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(slrb_cfg_to_add_mod_list_r16, bref, 1, 512));
  }
  if (slrb_cfg_to_release_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(slrb_cfg_to_release_list_r16, bref, 1, 512, integer_packer<uint16_t>(1, 512)));
  }
  if (sl_meas_cfg_r16_present) {
    HANDLE_CODE(sl_meas_cfg_r16.unpack(bref));
  }
  if (sl_csi_rs_cfg_r16_present) {
    HANDLE_CODE(sl_csi_rs_cfg_r16.unpack(bref));
  }
  if (sl_latency_bound_csi_report_r16_present) {
    HANDLE_CODE(unpack_integer(sl_latency_bound_csi_report_r16, bref, (uint8_t)3u, (uint8_t)160u));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_sidelink_r16_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (slrb_cfg_to_add_mod_list_r16.size() > 0) {
    j.start_array("slrb-ConfigToAddModList-r16");
    for (const auto& e1 : slrb_cfg_to_add_mod_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (slrb_cfg_to_release_list_r16.size() > 0) {
    j.start_array("slrb-ConfigToReleaseList-r16");
    for (const auto& e1 : slrb_cfg_to_release_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_meas_cfg_r16_present) {
    j.write_fieldname("sl-MeasConfig-r16");
    sl_meas_cfg_r16.to_json(j);
  }
  if (sl_csi_rs_cfg_r16_present) {
    j.write_fieldname("sl-CSI-RS-Config-r16");
    sl_csi_rs_cfg_r16.to_json(j);
  }
  if (sl_reset_cfg_r16_present) {
    j.write_str("sl-ResetConfig-r16", "true");
  }
  if (sl_latency_bound_csi_report_r16_present) {
    j.write_int("sl-LatencyBoundCSI-Report-r16", sl_latency_bound_csi_report_r16);
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// RRCReconfigurationSidelink ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_sidelink_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, rrc_transaction_id_r16, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_sidelink_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(rrc_transaction_id_r16, bref, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_sidelink_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("rrc-TransactionIdentifier-r16", rrc_transaction_id_r16);
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void rrc_recfg_sidelink_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
rrc_recfg_sidelink_r16_ies_s& rrc_recfg_sidelink_s::crit_exts_c_::set_rrc_recfg_sidelink_r16()
{
  set(types::rrc_recfg_sidelink_r16);
  return c;
}
void rrc_recfg_sidelink_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void rrc_recfg_sidelink_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::rrc_recfg_sidelink_r16:
      j.write_fieldname("rrcReconfigurationSidelink-r16");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_sidelink_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE rrc_recfg_sidelink_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::rrc_recfg_sidelink_r16:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_sidelink_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::rrc_recfg_sidelink_r16:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "rrc_recfg_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* rrc_recfg_sidelink_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"rrcReconfigurationSidelink-r16", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "rrc_recfg_sidelink_s::crit_exts_c_::types");
}

// SL-SIB-ReqInfo-r17 ::= ENUMERATED
const char* sl_sib_req_info_r17_opts::to_string() const
{
  static const char* names[] = {"sib1",       "sib2",       "sib3",          "sib4",        "sib5",        "sib6",
                                "sib7",       "sib8",       "sib9",          "sib10",       "sib11",       "sib12",
                                "sib13",      "sib14",      "sib15",         "sib16",       "sib17",       "sib18",
                                "sib19",      "sib20",      "sib21",         "sibNotReq11", "sibNotReq10", "sibNotReq9",
                                "sibNotReq8", "sibNotReq7", "sibNotReq6",    "sibNotReq5",  "sibNotReq4",  "sibNotReq3",
                                "sibNotReq2", "sibNotReq1", "sib17bis-v1820"};
  return convert_enum_idx(names, 33, value, "sl_sib_req_info_r17_e");
}

// SL-PagingInfo-RemoteUE-r17 ::= SEQUENCE
OCUDUASN_CODE sl_paging_info_remote_ue_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_paging_cycle_remote_ue_r17_present, 1));

  HANDLE_CODE(sl_paging_id_remote_ue_r17.pack(bref));
  if (sl_paging_cycle_remote_ue_r17_present) {
    HANDLE_CODE(sl_paging_cycle_remote_ue_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_paging_info_remote_ue_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sl_paging_cycle_remote_ue_r17_present, 1));

  HANDLE_CODE(sl_paging_id_remote_ue_r17.unpack(bref));
  if (sl_paging_cycle_remote_ue_r17_present) {
    HANDLE_CODE(sl_paging_cycle_remote_ue_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void sl_paging_info_remote_ue_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-PagingIdentityRemoteUE-r17");
  sl_paging_id_remote_ue_r17.to_json(j);
  if (sl_paging_cycle_remote_ue_r17_present) {
    j.write_str("sl-PagingCycleRemoteUE-r17", sl_paging_cycle_remote_ue_r17.to_string());
  }
  j.end_obj();
}

// SL-PosSIB-ReqInfo-r18 ::= SEQUENCE
OCUDUASN_CODE sl_pos_sib_req_info_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(gnss_id_r18_present, 1));
  HANDLE_CODE(bref.pack(sbas_id_r18_present, 1));

  if (gnss_id_r18_present) {
    HANDLE_CODE(gnss_id_r18.pack(bref));
  }
  if (sbas_id_r18_present) {
    HANDLE_CODE(sbas_id_r18.pack(bref));
  }
  HANDLE_CODE(pos_sib_type_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_pos_sib_req_info_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(gnss_id_r18_present, 1));
  HANDLE_CODE(bref.unpack(sbas_id_r18_present, 1));

  if (gnss_id_r18_present) {
    HANDLE_CODE(gnss_id_r18.unpack(bref));
  }
  if (sbas_id_r18_present) {
    HANDLE_CODE(sbas_id_r18.unpack(bref));
  }
  HANDLE_CODE(pos_sib_type_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void sl_pos_sib_req_info_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (gnss_id_r18_present) {
    j.write_fieldname("gnss-id-r18");
    gnss_id_r18.to_json(j);
  }
  if (sbas_id_r18_present) {
    j.write_fieldname("sbas-id-r18");
    sbas_id_r18.to_json(j);
  }
  j.write_str("posSibType-r18", pos_sib_type_r18.to_string());
  j.end_obj();
}

const char* sl_pos_sib_req_info_r18_s::pos_sib_type_r18_opts::to_string() const
{
  static const char* names[] = {
      "posSibType1-1",  "posSibType1-2",   "posSibType1-3",  "posSibType1-4",  "posSibType1-5",   "posSibType1-6",
      "posSibType1-7",  "posSibType1-8",   "posSibType1-9",  "posSibType1-10", "posSibType1-11",  "posSibType1-12",
      "posSibType2-1",  "posSibType2-2",   "posSibType2-3",  "posSibType2-4",  "posSibType2-5",   "posSibType2-6",
      "posSibType2-7",  "posSibType2-8",   "posSibType2-9",  "posSibType2-10", "posSibType2-11",  "posSibType2-12",
      "posSibType2-13", "posSibType2-14",  "posSibType2-15", "posSibType2-16", "posSibType2-17",  "posSibType2-17a",
      "posSibType2-18", "posSibType2-18a", "posSibType2-19", "posSibType2-20", "posSibType2-20a", "posSibType2-21",
      "posSibType2-22", "posSibType2-23",  "posSibType2-24", "posSibType2-25", "posSibType2-26",  "posSibType2-27",
      "posSibType3-1",  "posSibType4-1",   "posSibType5-1",  "posSibType6-1",  "posSibType6-2",   "posSibType6-3",
      "posSibType6-4",  "posSibType6-5",   "posSibType6-6",  "posSibType6-7",  "posSibType7-1",   "posSibType7-2",
      "posSibType7-3",  "posSibType7-4",   "spare9",         "spare8",         "spare7",          "spare6",
      "spare5",         "spare4",          "spare3",         "spare2",         "spare1"};
  return convert_enum_idx(names, 65, value, "sl_pos_sib_req_info_r18_s::pos_sib_type_r18_e_");
}

// RemoteUEInformationSidelink-v1800-IEs ::= SEQUENCE
OCUDUASN_CODE remote_ue_info_sidelink_v1800_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_requested_pos_sib_list_r18_present, 1));
  HANDLE_CODE(bref.pack(sl_sfn_dfn_offset_requested_r18_present, 1));
  HANDLE_CODE(bref.pack(conn_for_mp_r18_present, 1));
  HANDLE_CODE(bref.pack(sl_dest_id_remote_ue_r18_present, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_requested_pos_sib_list_r18_present) {
    HANDLE_CODE(sl_requested_pos_sib_list_r18.pack(bref));
  }
  if (sl_dest_id_remote_ue_r18_present) {
    HANDLE_CODE(sl_dest_id_remote_ue_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE remote_ue_info_sidelink_v1800_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sl_requested_pos_sib_list_r18_present, 1));
  HANDLE_CODE(bref.unpack(sl_sfn_dfn_offset_requested_r18_present, 1));
  HANDLE_CODE(bref.unpack(conn_for_mp_r18_present, 1));
  HANDLE_CODE(bref.unpack(sl_dest_id_remote_ue_r18_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_requested_pos_sib_list_r18_present) {
    HANDLE_CODE(sl_requested_pos_sib_list_r18.unpack(bref));
  }
  if (sl_dest_id_remote_ue_r18_present) {
    HANDLE_CODE(sl_dest_id_remote_ue_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void remote_ue_info_sidelink_v1800_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_requested_pos_sib_list_r18_present) {
    j.write_fieldname("sl-RequestedPosSIB-List-r18");
    sl_requested_pos_sib_list_r18.to_json(j);
  }
  if (sl_sfn_dfn_offset_requested_r18_present) {
    j.write_str("sl-SFN-DFN-OffsetRequested-r18", "true");
  }
  if (conn_for_mp_r18_present) {
    j.write_str("connectionForMP-r18", "true");
  }
  if (sl_dest_id_remote_ue_r18_present) {
    j.write_str("sl-DestinationIdentityRemoteUE-r18", sl_dest_id_remote_ue_r18.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// RemoteUEInformationSidelink-r17-IEs ::= SEQUENCE
OCUDUASN_CODE remote_ue_info_sidelink_r17_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_requested_sib_list_r17_present, 1));
  HANDLE_CODE(bref.pack(sl_paging_info_remote_ue_r17_present, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_requested_sib_list_r17_present) {
    HANDLE_CODE(sl_requested_sib_list_r17.pack(bref));
  }
  if (sl_paging_info_remote_ue_r17_present) {
    HANDLE_CODE(sl_paging_info_remote_ue_r17.pack(bref));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE remote_ue_info_sidelink_r17_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sl_requested_sib_list_r17_present, 1));
  HANDLE_CODE(bref.unpack(sl_paging_info_remote_ue_r17_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_requested_sib_list_r17_present) {
    HANDLE_CODE(sl_requested_sib_list_r17.unpack(bref));
  }
  if (sl_paging_info_remote_ue_r17_present) {
    HANDLE_CODE(sl_paging_info_remote_ue_r17.unpack(bref));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void remote_ue_info_sidelink_r17_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_requested_sib_list_r17_present) {
    j.write_fieldname("sl-RequestedSIB-List-r17");
    sl_requested_sib_list_r17.to_json(j);
  }
  if (sl_paging_info_remote_ue_r17_present) {
    j.write_fieldname("sl-PagingInfo-RemoteUE-r17");
    sl_paging_info_remote_ue_r17.to_json(j);
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// RemoteUEInformationSidelink-r17 ::= SEQUENCE
OCUDUASN_CODE remote_ue_info_sidelink_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE remote_ue_info_sidelink_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void remote_ue_info_sidelink_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void remote_ue_info_sidelink_r17_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
remote_ue_info_sidelink_r17_ies_s& remote_ue_info_sidelink_r17_s::crit_exts_c_::set_remote_ue_info_sidelink_r17()
{
  set(types::remote_ue_info_sidelink_r17);
  return c;
}
void remote_ue_info_sidelink_r17_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void remote_ue_info_sidelink_r17_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::remote_ue_info_sidelink_r17:
      j.write_fieldname("remoteUEInformationSidelink-r17");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "remote_ue_info_sidelink_r17_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE remote_ue_info_sidelink_r17_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::remote_ue_info_sidelink_r17:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "remote_ue_info_sidelink_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE remote_ue_info_sidelink_r17_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::remote_ue_info_sidelink_r17:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "remote_ue_info_sidelink_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* remote_ue_info_sidelink_r17_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"remoteUEInformationSidelink-r17", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "remote_ue_info_sidelink_r17_s::crit_exts_c_::types");
}

// SBCCH-SL-BCH-MessageType ::= CHOICE
void sbcch_sl_bch_msg_type_c::set(types::options e)
{
  type_ = e;
}
sbcch_sl_bch_msg_type_c::c1_c_& sbcch_sl_bch_msg_type_c::set_c1()
{
  set(types::c1);
  return c;
}
void sbcch_sl_bch_msg_type_c::set_msg_class_ext()
{
  set(types::msg_class_ext);
}
void sbcch_sl_bch_msg_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::msg_class_ext:
      break;
    default:
      log_invalid_choice_id(type_, "sbcch_sl_bch_msg_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE sbcch_sl_bch_msg_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::msg_class_ext:
      break;
    default:
      log_invalid_choice_id(type_, "sbcch_sl_bch_msg_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sbcch_sl_bch_msg_type_c::unpack(cbit_ref& bref)
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
      log_invalid_choice_id(type_, "sbcch_sl_bch_msg_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void sbcch_sl_bch_msg_type_c::c1_c_::set(types::options e)
{
  type_ = e;
}
mib_sidelink_s& sbcch_sl_bch_msg_type_c::c1_c_::set_mib_sidelink()
{
  set(types::mib_sidelink);
  return c;
}
void sbcch_sl_bch_msg_type_c::c1_c_::set_spare1()
{
  set(types::spare1);
}
void sbcch_sl_bch_msg_type_c::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::mib_sidelink:
      j.write_fieldname("masterInformationBlockSidelink");
      c.to_json(j);
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "sbcch_sl_bch_msg_type_c::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE sbcch_sl_bch_msg_type_c::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::mib_sidelink:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "sbcch_sl_bch_msg_type_c::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sbcch_sl_bch_msg_type_c::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::mib_sidelink:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "sbcch_sl_bch_msg_type_c::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* sbcch_sl_bch_msg_type_c::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"masterInformationBlockSidelink", "spare1"};
  return convert_enum_idx(names, 2, value, "sbcch_sl_bch_msg_type_c::c1_c_::types");
}

const char* sbcch_sl_bch_msg_type_c::types_opts::to_string() const
{
  static const char* names[] = {"c1", "messageClassExtension"};
  return convert_enum_idx(names, 2, value, "sbcch_sl_bch_msg_type_c::types");
}
uint8_t sbcch_sl_bch_msg_type_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "sbcch_sl_bch_msg_type_c::types");
}

// SBCCH-SL-BCH-Message ::= SEQUENCE
OCUDUASN_CODE sbcch_sl_bch_msg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(msg.pack(bref));

  bref.align_bytes_zero();

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sbcch_sl_bch_msg_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(msg.unpack(bref));

  bref.align_bytes();

  return OCUDUASN_SUCCESS;
}
void sbcch_sl_bch_msg_s::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.start_obj("SBCCH-SL-BCH-Message");
  j.write_fieldname("message");
  msg.to_json(j);
  j.end_obj();
  j.end_obj();
  j.end_array();
}

// UECapabilityEnquirySidelink-r16-IEs ::= SEQUENCE
OCUDUASN_CODE ue_cap_enquiry_sidelink_r16_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(freq_band_list_filt_sidelink_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(ue_cap_info_sidelink_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (freq_band_list_filt_sidelink_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, freq_band_list_filt_sidelink_r16, 1, 1280));
  }
  if (ue_cap_info_sidelink_r16.size() > 0) {
    HANDLE_CODE(ue_cap_info_sidelink_r16.pack(bref));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_enquiry_sidelink_r16_ies_s::unpack(cbit_ref& bref)
{
  bool freq_band_list_filt_sidelink_r16_present;
  HANDLE_CODE(bref.unpack(freq_band_list_filt_sidelink_r16_present, 1));
  bool ue_cap_info_sidelink_r16_present;
  HANDLE_CODE(bref.unpack(ue_cap_info_sidelink_r16_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (freq_band_list_filt_sidelink_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(freq_band_list_filt_sidelink_r16, bref, 1, 1280));
  }
  if (ue_cap_info_sidelink_r16_present) {
    HANDLE_CODE(ue_cap_info_sidelink_r16.unpack(bref));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_cap_enquiry_sidelink_r16_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (freq_band_list_filt_sidelink_r16.size() > 0) {
    j.start_array("frequencyBandListFilterSidelink-r16");
    for (const auto& e1 : freq_band_list_filt_sidelink_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ue_cap_info_sidelink_r16.size() > 0) {
    j.write_str("ue-CapabilityInformationSidelink-r16", ue_cap_info_sidelink_r16.to_string());
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UECapabilityEnquirySidelink ::= SEQUENCE
OCUDUASN_CODE ue_cap_enquiry_sidelink_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, rrc_transaction_id_r16, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_enquiry_sidelink_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(rrc_transaction_id_r16, bref, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ue_cap_enquiry_sidelink_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("rrc-TransactionIdentifier-r16", rrc_transaction_id_r16);
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ue_cap_enquiry_sidelink_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ue_cap_enquiry_sidelink_r16_ies_s& ue_cap_enquiry_sidelink_s::crit_exts_c_::set_ue_cap_enquiry_sidelink_r16()
{
  set(types::ue_cap_enquiry_sidelink_r16);
  return c;
}
void ue_cap_enquiry_sidelink_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ue_cap_enquiry_sidelink_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ue_cap_enquiry_sidelink_r16:
      j.write_fieldname("ueCapabilityEnquirySidelink-r16");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_cap_enquiry_sidelink_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_cap_enquiry_sidelink_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ue_cap_enquiry_sidelink_r16:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_cap_enquiry_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_enquiry_sidelink_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ue_cap_enquiry_sidelink_r16:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_cap_enquiry_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ue_cap_enquiry_sidelink_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"ueCapabilityEnquirySidelink-r16", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ue_cap_enquiry_sidelink_s::crit_exts_c_::types");
}

// UECapabilityInformationSidelink-v1800-IEs ::= SEQUENCE
OCUDUASN_CODE ue_cap_info_sidelink_v1800_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sfn_dfn_offset_supported_r18_present, 1));
  HANDLE_CODE(bref.pack(pos_sib_forwarding_supported_r18_present, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_info_sidelink_v1800_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sfn_dfn_offset_supported_r18_present, 1));
  HANDLE_CODE(bref.unpack(pos_sib_forwarding_supported_r18_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  return OCUDUASN_SUCCESS;
}
void ue_cap_info_sidelink_v1800_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sfn_dfn_offset_supported_r18_present) {
    j.write_str("sfn-DFN-OffsetSupported-r18", "supported");
  }
  if (pos_sib_forwarding_supported_r18_present) {
    j.write_str("posSIB-ForwardingSupported-r18", "supported");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UECapabilityInformationSidelink-v1700-IEs ::= SEQUENCE
OCUDUASN_CODE ue_cap_info_sidelink_v1700_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(mac_params_sidelink_r17_present, 1));
  HANDLE_CODE(bref.pack(supported_band_combination_list_sidelink_nr_v1710.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (mac_params_sidelink_r17_present) {
    HANDLE_CODE(mac_params_sidelink_r17.pack(bref));
  }
  if (supported_band_combination_list_sidelink_nr_v1710.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(
        bref, supported_band_combination_list_sidelink_nr_v1710, 1, 65536, SeqOfPacker<Packer>(1, 32, Packer())));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_info_sidelink_v1700_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(mac_params_sidelink_r17_present, 1));
  bool supported_band_combination_list_sidelink_nr_v1710_present;
  HANDLE_CODE(bref.unpack(supported_band_combination_list_sidelink_nr_v1710_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (mac_params_sidelink_r17_present) {
    HANDLE_CODE(mac_params_sidelink_r17.unpack(bref));
  }
  if (supported_band_combination_list_sidelink_nr_v1710_present) {
    HANDLE_CODE(unpack_dyn_seq_of(
        supported_band_combination_list_sidelink_nr_v1710, bref, 1, 65536, SeqOfPacker<Packer>(1, 32, Packer())));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_cap_info_sidelink_v1700_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mac_params_sidelink_r17_present) {
    j.write_fieldname("mac-ParametersSidelink-r17");
    mac_params_sidelink_r17.to_json(j);
  }
  if (supported_band_combination_list_sidelink_nr_v1710.size() > 0) {
    j.start_array("supportedBandCombinationListSidelinkNR-v1710");
    for (const auto& e1 : supported_band_combination_list_sidelink_nr_v1710) {
      j.start_array();
      for (const auto& e2 : e1) {
        e2.to_json(j);
      }
      j.end_array();
    }
    j.end_array();
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UECapabilityInformationSidelink-r16-IEs ::= SEQUENCE
OCUDUASN_CODE ue_cap_info_sidelink_r16_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(pdcp_params_sidelink_r16_present, 1));
  HANDLE_CODE(bref.pack(rlc_params_sidelink_r16_present, 1));
  HANDLE_CODE(bref.pack(supported_band_combination_list_sidelink_nr_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(supported_band_list_sidelink_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(applied_freq_band_list_filt_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  HANDLE_CODE(access_stratum_release_sidelink_r16.pack(bref));
  if (pdcp_params_sidelink_r16_present) {
    HANDLE_CODE(pdcp_params_sidelink_r16.pack(bref));
  }
  if (rlc_params_sidelink_r16_present) {
    HANDLE_CODE(rlc_params_sidelink_r16.pack(bref));
  }
  if (supported_band_combination_list_sidelink_nr_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(
        bref, supported_band_combination_list_sidelink_nr_r16, 1, 65536, SeqOfPacker<Packer>(1, 32, Packer())));
  }
  if (supported_band_list_sidelink_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_list_sidelink_r16, 1, 1024));
  }
  if (applied_freq_band_list_filt_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, applied_freq_band_list_filt_r16, 1, 1280));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_info_sidelink_r16_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(pdcp_params_sidelink_r16_present, 1));
  HANDLE_CODE(bref.unpack(rlc_params_sidelink_r16_present, 1));
  bool supported_band_combination_list_sidelink_nr_r16_present;
  HANDLE_CODE(bref.unpack(supported_band_combination_list_sidelink_nr_r16_present, 1));
  bool supported_band_list_sidelink_r16_present;
  HANDLE_CODE(bref.unpack(supported_band_list_sidelink_r16_present, 1));
  bool applied_freq_band_list_filt_r16_present;
  HANDLE_CODE(bref.unpack(applied_freq_band_list_filt_r16_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  HANDLE_CODE(access_stratum_release_sidelink_r16.unpack(bref));
  if (pdcp_params_sidelink_r16_present) {
    HANDLE_CODE(pdcp_params_sidelink_r16.unpack(bref));
  }
  if (rlc_params_sidelink_r16_present) {
    HANDLE_CODE(rlc_params_sidelink_r16.unpack(bref));
  }
  if (supported_band_combination_list_sidelink_nr_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(
        supported_band_combination_list_sidelink_nr_r16, bref, 1, 65536, SeqOfPacker<Packer>(1, 32, Packer())));
  }
  if (supported_band_list_sidelink_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_list_sidelink_r16, bref, 1, 1024));
  }
  if (applied_freq_band_list_filt_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(applied_freq_band_list_filt_r16, bref, 1, 1280));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_cap_info_sidelink_r16_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("accessStratumReleaseSidelink-r16", access_stratum_release_sidelink_r16.to_string());
  if (pdcp_params_sidelink_r16_present) {
    j.write_fieldname("pdcp-ParametersSidelink-r16");
    pdcp_params_sidelink_r16.to_json(j);
  }
  if (rlc_params_sidelink_r16_present) {
    j.write_fieldname("rlc-ParametersSidelink-r16");
    rlc_params_sidelink_r16.to_json(j);
  }
  if (supported_band_combination_list_sidelink_nr_r16.size() > 0) {
    j.start_array("supportedBandCombinationListSidelinkNR-r16");
    for (const auto& e1 : supported_band_combination_list_sidelink_nr_r16) {
      j.start_array();
      for (const auto& e2 : e1) {
        e2.to_json(j);
      }
      j.end_array();
    }
    j.end_array();
  }
  if (supported_band_list_sidelink_r16.size() > 0) {
    j.start_array("supportedBandListSidelink-r16");
    for (const auto& e1 : supported_band_list_sidelink_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (applied_freq_band_list_filt_r16.size() > 0) {
    j.start_array("appliedFreqBandListFilter-r16");
    for (const auto& e1 : applied_freq_band_list_filt_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UECapabilityInformationSidelink ::= SEQUENCE
OCUDUASN_CODE ue_cap_info_sidelink_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, rrc_transaction_id_r16, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_info_sidelink_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(rrc_transaction_id_r16, bref, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ue_cap_info_sidelink_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("rrc-TransactionIdentifier-r16", rrc_transaction_id_r16);
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ue_cap_info_sidelink_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ue_cap_info_sidelink_r16_ies_s& ue_cap_info_sidelink_s::crit_exts_c_::set_ue_cap_info_sidelink_r16()
{
  set(types::ue_cap_info_sidelink_r16);
  return c;
}
void ue_cap_info_sidelink_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ue_cap_info_sidelink_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ue_cap_info_sidelink_r16:
      j.write_fieldname("ueCapabilityInformationSidelink-r16");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_cap_info_sidelink_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_cap_info_sidelink_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ue_cap_info_sidelink_r16:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_cap_info_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_info_sidelink_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ue_cap_info_sidelink_r16:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_cap_info_sidelink_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ue_cap_info_sidelink_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"ueCapabilityInformationSidelink-r16", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ue_cap_info_sidelink_s::crit_exts_c_::types");
}

// UuMessageTransferSidelink-v1800-IEs ::= SEQUENCE
OCUDUASN_CODE uu_msg_transfer_sidelink_v1800_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_paging_delivery_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_paging_delivery_r18.size() > 0) {
    HANDLE_CODE(sl_paging_delivery_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE uu_msg_transfer_sidelink_v1800_ies_s::unpack(cbit_ref& bref)
{
  bool sl_paging_delivery_r18_present;
  HANDLE_CODE(bref.unpack(sl_paging_delivery_r18_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_paging_delivery_r18_present) {
    HANDLE_CODE(sl_paging_delivery_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void uu_msg_transfer_sidelink_v1800_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_paging_delivery_r18.size() > 0) {
    j.write_str("sl-PagingDelivery-r18", sl_paging_delivery_r18.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UuMessageTransferSidelink-r17-IEs ::= SEQUENCE
OCUDUASN_CODE uu_msg_transfer_sidelink_r17_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_paging_delivery_r17.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_sib1_delivery_r17.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_sys_info_delivery_r17.size() > 0, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_paging_delivery_r17.size() > 0) {
    HANDLE_CODE(sl_paging_delivery_r17.pack(bref));
  }
  if (sl_sib1_delivery_r17.size() > 0) {
    HANDLE_CODE(sl_sib1_delivery_r17.pack(bref));
  }
  if (sl_sys_info_delivery_r17.size() > 0) {
    HANDLE_CODE(sl_sys_info_delivery_r17.pack(bref));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE uu_msg_transfer_sidelink_r17_ies_s::unpack(cbit_ref& bref)
{
  bool sl_paging_delivery_r17_present;
  HANDLE_CODE(bref.unpack(sl_paging_delivery_r17_present, 1));
  bool sl_sib1_delivery_r17_present;
  HANDLE_CODE(bref.unpack(sl_sib1_delivery_r17_present, 1));
  bool sl_sys_info_delivery_r17_present;
  HANDLE_CODE(bref.unpack(sl_sys_info_delivery_r17_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_paging_delivery_r17_present) {
    HANDLE_CODE(sl_paging_delivery_r17.unpack(bref));
  }
  if (sl_sib1_delivery_r17_present) {
    HANDLE_CODE(sl_sib1_delivery_r17.unpack(bref));
  }
  if (sl_sys_info_delivery_r17_present) {
    HANDLE_CODE(sl_sys_info_delivery_r17.unpack(bref));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void uu_msg_transfer_sidelink_r17_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_paging_delivery_r17.size() > 0) {
    j.write_str("sl-PagingDelivery-r17", sl_paging_delivery_r17.to_string());
  }
  if (sl_sib1_delivery_r17.size() > 0) {
    j.write_str("sl-SIB1-Delivery-r17", sl_sib1_delivery_r17.to_string());
  }
  if (sl_sys_info_delivery_r17.size() > 0) {
    j.write_str("sl-SystemInformationDelivery-r17", sl_sys_info_delivery_r17.to_string());
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UuMessageTransferSidelink-r17 ::= SEQUENCE
OCUDUASN_CODE uu_msg_transfer_sidelink_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE uu_msg_transfer_sidelink_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void uu_msg_transfer_sidelink_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void uu_msg_transfer_sidelink_r17_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
uu_msg_transfer_sidelink_r17_ies_s& uu_msg_transfer_sidelink_r17_s::crit_exts_c_::set_uu_msg_transfer_sidelink_r17()
{
  set(types::uu_msg_transfer_sidelink_r17);
  return c;
}
void uu_msg_transfer_sidelink_r17_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void uu_msg_transfer_sidelink_r17_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::uu_msg_transfer_sidelink_r17:
      j.write_fieldname("uuMessageTransferSidelink-r17");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "uu_msg_transfer_sidelink_r17_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE uu_msg_transfer_sidelink_r17_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::uu_msg_transfer_sidelink_r17:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "uu_msg_transfer_sidelink_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE uu_msg_transfer_sidelink_r17_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::uu_msg_transfer_sidelink_r17:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "uu_msg_transfer_sidelink_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* uu_msg_transfer_sidelink_r17_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"uuMessageTransferSidelink-r17", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "uu_msg_transfer_sidelink_r17_s::crit_exts_c_::types");
}

// UEAssistanceInformationSidelink-r17-IEs ::= SEQUENCE
OCUDUASN_CODE ue_assist_info_sidelink_r17_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_preferred_drx_cfg_list_r17.size() > 0, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_preferred_drx_cfg_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_preferred_drx_cfg_list_r17, 1, 4));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_assist_info_sidelink_r17_ies_s::unpack(cbit_ref& bref)
{
  bool sl_preferred_drx_cfg_list_r17_present;
  HANDLE_CODE(bref.unpack(sl_preferred_drx_cfg_list_r17_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_preferred_drx_cfg_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_preferred_drx_cfg_list_r17, bref, 1, 4));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_assist_info_sidelink_r17_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_preferred_drx_cfg_list_r17.size() > 0) {
    j.start_array("sl-PreferredDRX-ConfigList-r17");
    for (const auto& e1 : sl_preferred_drx_cfg_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UEAssistanceInformationSidelink-r17 ::= SEQUENCE
OCUDUASN_CODE ue_assist_info_sidelink_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_assist_info_sidelink_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ue_assist_info_sidelink_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ue_assist_info_sidelink_r17_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ue_assist_info_sidelink_r17_ies_s& ue_assist_info_sidelink_r17_s::crit_exts_c_::set_ue_assist_info_sidelink_r17()
{
  set(types::ue_assist_info_sidelink_r17);
  return c;
}
void ue_assist_info_sidelink_r17_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ue_assist_info_sidelink_r17_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ue_assist_info_sidelink_r17:
      j.write_fieldname("ueAssistanceInformationSidelink-r17");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_assist_info_sidelink_r17_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_assist_info_sidelink_r17_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ue_assist_info_sidelink_r17:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_assist_info_sidelink_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_assist_info_sidelink_r17_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ue_assist_info_sidelink_r17:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_assist_info_sidelink_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ue_assist_info_sidelink_r17_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"ueAssistanceInformationSidelink-r17", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ue_assist_info_sidelink_r17_s::crit_exts_c_::types");
}

// SL-E2E-QoS-InfoPC5-r18 ::= SEQUENCE
OCUDUASN_CODE sl_e2_e_qos_info_pc5_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(sl_dest_id_remote_ue_r18.pack(bref));
  HANDLE_CODE(pack_integer(bref, sl_e2_e_slrb_idx_r18, (uint16_t)1u, (uint16_t)512u));
  HANDLE_CODE(pack_dyn_seq_of(bref, sl_qos_info_list_r18, 1, 64));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_e2_e_qos_info_pc5_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(sl_dest_id_remote_ue_r18.unpack(bref));
  HANDLE_CODE(unpack_integer(sl_e2_e_slrb_idx_r18, bref, (uint16_t)1u, (uint16_t)512u));
  HANDLE_CODE(unpack_dyn_seq_of(sl_qos_info_list_r18, bref, 1, 64));

  return OCUDUASN_SUCCESS;
}
void sl_e2_e_qos_info_pc5_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("sl-DestinationIdentityRemoteUE-r18", sl_dest_id_remote_ue_r18.to_string());
  j.write_int("sl-E2E-SLRB-Index-r18", sl_e2_e_slrb_idx_r18);
  j.start_array("sl-QoS-InfoList-r18");
  for (const auto& e1 : sl_qos_info_list_r18) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// UEInformationRequestSidelink-r18-IEs ::= SEQUENCE
OCUDUASN_CODE ue_info_request_sidelink_r18_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_e2_e_qos_info_list_pc5_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_e2_e_qos_info_list_pc5_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_e2_e_qos_info_list_pc5_r18, 1, 512));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_info_request_sidelink_r18_ies_s::unpack(cbit_ref& bref)
{
  bool sl_e2_e_qos_info_list_pc5_r18_present;
  HANDLE_CODE(bref.unpack(sl_e2_e_qos_info_list_pc5_r18_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_e2_e_qos_info_list_pc5_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_e2_e_qos_info_list_pc5_r18, bref, 1, 512));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_info_request_sidelink_r18_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_e2_e_qos_info_list_pc5_r18.size() > 0) {
    j.start_array("sl-E2E-QoS-InfoListPC5-r18");
    for (const auto& e1 : sl_e2_e_qos_info_list_pc5_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UEInformationRequestSidelink-r18 ::= SEQUENCE
OCUDUASN_CODE ue_info_request_sidelink_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, rrc_transaction_id_r18, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_info_request_sidelink_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(rrc_transaction_id_r18, bref, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ue_info_request_sidelink_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("rrc-TransactionIdentifier-r18", rrc_transaction_id_r18);
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ue_info_request_sidelink_r18_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ue_info_request_sidelink_r18_ies_s& ue_info_request_sidelink_r18_s::crit_exts_c_::set_ue_info_request_sidelink_r18()
{
  set(types::ue_info_request_sidelink_r18);
  return c;
}
void ue_info_request_sidelink_r18_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ue_info_request_sidelink_r18_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ue_info_request_sidelink_r18:
      j.write_fieldname("ueInformationRequestSidelink-r18");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_info_request_sidelink_r18_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_info_request_sidelink_r18_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ue_info_request_sidelink_r18:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_info_request_sidelink_r18_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_info_request_sidelink_r18_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ue_info_request_sidelink_r18:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_info_request_sidelink_r18_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ue_info_request_sidelink_r18_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"ueInformationRequestSidelink-r18", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ue_info_request_sidelink_r18_s::crit_exts_c_::types");
}

// SL-SplitQoS-InfoPC5-r18 ::= SEQUENCE
OCUDUASN_CODE sl_split_qos_info_pc5_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, sl_qos_flow_id_r18, (uint16_t)1u, (uint16_t)2048u));
  HANDLE_CODE(pack_integer(bref, sl_split_packet_delay_budget_r18, (uint16_t)0u, (uint16_t)1023u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_split_qos_info_pc5_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(sl_qos_flow_id_r18, bref, (uint16_t)1u, (uint16_t)2048u));
  HANDLE_CODE(unpack_integer(sl_split_packet_delay_budget_r18, bref, (uint16_t)0u, (uint16_t)1023u));

  return OCUDUASN_SUCCESS;
}
void sl_split_qos_info_pc5_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sl-QoS-FlowIdentity-r18", sl_qos_flow_id_r18);
  j.write_int("sl-SplitPacketDelayBudget-r18", sl_split_packet_delay_budget_r18);
  j.end_obj();
}

// UEInformationResponseSidelink-r18-IEs ::= SEQUENCE
OCUDUASN_CODE ue_info_resp_sidelink_r18_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_split_qos_info_list_pc5_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(late_non_crit_ext.size() > 0, 1));
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  if (sl_split_qos_info_list_pc5_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_split_qos_info_list_pc5_r18, 1, 2048));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_info_resp_sidelink_r18_ies_s::unpack(cbit_ref& bref)
{
  bool sl_split_qos_info_list_pc5_r18_present;
  HANDLE_CODE(bref.unpack(sl_split_qos_info_list_pc5_r18_present, 1));
  bool late_non_crit_ext_present;
  HANDLE_CODE(bref.unpack(late_non_crit_ext_present, 1));
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  if (sl_split_qos_info_list_pc5_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_split_qos_info_list_pc5_r18, bref, 1, 2048));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_info_resp_sidelink_r18_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_split_qos_info_list_pc5_r18.size() > 0) {
    j.start_array("sl-SplitQoS-InfoListPC5-r18");
    for (const auto& e1 : sl_split_qos_info_list_pc5_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UEInformationResponseSidelink-r18 ::= SEQUENCE
OCUDUASN_CODE ue_info_resp_sidelink_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, rrc_transaction_id_r18, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_info_resp_sidelink_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(rrc_transaction_id_r18, bref, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ue_info_resp_sidelink_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("rrc-TransactionIdentifier-r18", rrc_transaction_id_r18);
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ue_info_resp_sidelink_r18_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ue_info_resp_sidelink_r18_ies_s& ue_info_resp_sidelink_r18_s::crit_exts_c_::set_ue_info_resp_sidelink_r18()
{
  set(types::ue_info_resp_sidelink_r18);
  return c;
}
void ue_info_resp_sidelink_r18_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ue_info_resp_sidelink_r18_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ue_info_resp_sidelink_r18:
      j.write_fieldname("ueInformationResponseSidelink-r18");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_info_resp_sidelink_r18_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_info_resp_sidelink_r18_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ue_info_resp_sidelink_r18:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_info_resp_sidelink_r18_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_info_resp_sidelink_r18_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ue_info_resp_sidelink_r18:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_info_resp_sidelink_r18_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ue_info_resp_sidelink_r18_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"ueInformationResponseSidelink-r18", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ue_info_resp_sidelink_r18_s::crit_exts_c_::types");
}

// SCCH-MessageType ::= CHOICE
void s_cch_msg_type_c::destroy_()
{
  switch (type_) {
    case types::c1:
      c.destroy<c1_c_>();
      break;
    case types::msg_class_ext:
      c.destroy<msg_class_ext_c_>();
      break;
    default:
      break;
  }
}
void s_cch_msg_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::c1:
      c.init<c1_c_>();
      break;
    case types::msg_class_ext:
      c.init<msg_class_ext_c_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c");
  }
}
s_cch_msg_type_c::s_cch_msg_type_c(const s_cch_msg_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::c1:
      c.init(other.c.get<c1_c_>());
      break;
    case types::msg_class_ext:
      c.init(other.c.get<msg_class_ext_c_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c");
  }
}
s_cch_msg_type_c& s_cch_msg_type_c::operator=(const s_cch_msg_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::c1:
      c.set(other.c.get<c1_c_>());
      break;
    case types::msg_class_ext:
      c.set(other.c.get<msg_class_ext_c_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c");
  }

  return *this;
}
s_cch_msg_type_c::c1_c_& s_cch_msg_type_c::set_c1()
{
  set(types::c1);
  return c.get<c1_c_>();
}
s_cch_msg_type_c::msg_class_ext_c_& s_cch_msg_type_c::set_msg_class_ext()
{
  set(types::msg_class_ext);
  return c.get<msg_class_ext_c_>();
}
void s_cch_msg_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.get<c1_c_>().to_json(j);
      break;
    case types::msg_class_ext:
      j.write_fieldname("messageClassExtension");
      c.get<msg_class_ext_c_>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE s_cch_msg_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.get<c1_c_>().pack(bref));
      break;
    case types::msg_class_ext:
      HANDLE_CODE(c.get<msg_class_ext_c_>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE s_cch_msg_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.get<c1_c_>().unpack(bref));
      break;
    case types::msg_class_ext:
      HANDLE_CODE(c.get<msg_class_ext_c_>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void s_cch_msg_type_c::c1_c_::destroy_()
{
  switch (type_) {
    case types::meas_report_sidelink:
      c.destroy<meas_report_sidelink_s>();
      break;
    case types::rrc_recfg_sidelink:
      c.destroy<rrc_recfg_sidelink_s>();
      break;
    case types::rrc_recfg_complete_sidelink:
      c.destroy<rrc_recfg_complete_sidelink_s>();
      break;
    case types::rrc_recfg_fail_sidelink:
      c.destroy<rrc_recfg_fail_sidelink_s>();
      break;
    case types::ue_cap_enquiry_sidelink:
      c.destroy<ue_cap_enquiry_sidelink_s>();
      break;
    case types::ue_cap_info_sidelink:
      c.destroy<ue_cap_info_sidelink_s>();
      break;
    case types::uu_msg_transfer_sidelink_r17:
      c.destroy<uu_msg_transfer_sidelink_r17_s>();
      break;
    case types::remote_ue_info_sidelink_r17:
      c.destroy<remote_ue_info_sidelink_r17_s>();
      break;
    default:
      break;
  }
}
void s_cch_msg_type_c::c1_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::meas_report_sidelink:
      c.init<meas_report_sidelink_s>();
      break;
    case types::rrc_recfg_sidelink:
      c.init<rrc_recfg_sidelink_s>();
      break;
    case types::rrc_recfg_complete_sidelink:
      c.init<rrc_recfg_complete_sidelink_s>();
      break;
    case types::rrc_recfg_fail_sidelink:
      c.init<rrc_recfg_fail_sidelink_s>();
      break;
    case types::ue_cap_enquiry_sidelink:
      c.init<ue_cap_enquiry_sidelink_s>();
      break;
    case types::ue_cap_info_sidelink:
      c.init<ue_cap_info_sidelink_s>();
      break;
    case types::uu_msg_transfer_sidelink_r17:
      c.init<uu_msg_transfer_sidelink_r17_s>();
      break;
    case types::remote_ue_info_sidelink_r17:
      c.init<remote_ue_info_sidelink_r17_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::c1_c_");
  }
}
s_cch_msg_type_c::c1_c_::c1_c_(const s_cch_msg_type_c::c1_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::meas_report_sidelink:
      c.init(other.c.get<meas_report_sidelink_s>());
      break;
    case types::rrc_recfg_sidelink:
      c.init(other.c.get<rrc_recfg_sidelink_s>());
      break;
    case types::rrc_recfg_complete_sidelink:
      c.init(other.c.get<rrc_recfg_complete_sidelink_s>());
      break;
    case types::rrc_recfg_fail_sidelink:
      c.init(other.c.get<rrc_recfg_fail_sidelink_s>());
      break;
    case types::ue_cap_enquiry_sidelink:
      c.init(other.c.get<ue_cap_enquiry_sidelink_s>());
      break;
    case types::ue_cap_info_sidelink:
      c.init(other.c.get<ue_cap_info_sidelink_s>());
      break;
    case types::uu_msg_transfer_sidelink_r17:
      c.init(other.c.get<uu_msg_transfer_sidelink_r17_s>());
      break;
    case types::remote_ue_info_sidelink_r17:
      c.init(other.c.get<remote_ue_info_sidelink_r17_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::c1_c_");
  }
}
s_cch_msg_type_c::c1_c_& s_cch_msg_type_c::c1_c_::operator=(const s_cch_msg_type_c::c1_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::meas_report_sidelink:
      c.set(other.c.get<meas_report_sidelink_s>());
      break;
    case types::rrc_recfg_sidelink:
      c.set(other.c.get<rrc_recfg_sidelink_s>());
      break;
    case types::rrc_recfg_complete_sidelink:
      c.set(other.c.get<rrc_recfg_complete_sidelink_s>());
      break;
    case types::rrc_recfg_fail_sidelink:
      c.set(other.c.get<rrc_recfg_fail_sidelink_s>());
      break;
    case types::ue_cap_enquiry_sidelink:
      c.set(other.c.get<ue_cap_enquiry_sidelink_s>());
      break;
    case types::ue_cap_info_sidelink:
      c.set(other.c.get<ue_cap_info_sidelink_s>());
      break;
    case types::uu_msg_transfer_sidelink_r17:
      c.set(other.c.get<uu_msg_transfer_sidelink_r17_s>());
      break;
    case types::remote_ue_info_sidelink_r17:
      c.set(other.c.get<remote_ue_info_sidelink_r17_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::c1_c_");
  }

  return *this;
}
meas_report_sidelink_s& s_cch_msg_type_c::c1_c_::set_meas_report_sidelink()
{
  set(types::meas_report_sidelink);
  return c.get<meas_report_sidelink_s>();
}
rrc_recfg_sidelink_s& s_cch_msg_type_c::c1_c_::set_rrc_recfg_sidelink()
{
  set(types::rrc_recfg_sidelink);
  return c.get<rrc_recfg_sidelink_s>();
}
rrc_recfg_complete_sidelink_s& s_cch_msg_type_c::c1_c_::set_rrc_recfg_complete_sidelink()
{
  set(types::rrc_recfg_complete_sidelink);
  return c.get<rrc_recfg_complete_sidelink_s>();
}
rrc_recfg_fail_sidelink_s& s_cch_msg_type_c::c1_c_::set_rrc_recfg_fail_sidelink()
{
  set(types::rrc_recfg_fail_sidelink);
  return c.get<rrc_recfg_fail_sidelink_s>();
}
ue_cap_enquiry_sidelink_s& s_cch_msg_type_c::c1_c_::set_ue_cap_enquiry_sidelink()
{
  set(types::ue_cap_enquiry_sidelink);
  return c.get<ue_cap_enquiry_sidelink_s>();
}
ue_cap_info_sidelink_s& s_cch_msg_type_c::c1_c_::set_ue_cap_info_sidelink()
{
  set(types::ue_cap_info_sidelink);
  return c.get<ue_cap_info_sidelink_s>();
}
uu_msg_transfer_sidelink_r17_s& s_cch_msg_type_c::c1_c_::set_uu_msg_transfer_sidelink_r17()
{
  set(types::uu_msg_transfer_sidelink_r17);
  return c.get<uu_msg_transfer_sidelink_r17_s>();
}
remote_ue_info_sidelink_r17_s& s_cch_msg_type_c::c1_c_::set_remote_ue_info_sidelink_r17()
{
  set(types::remote_ue_info_sidelink_r17);
  return c.get<remote_ue_info_sidelink_r17_s>();
}
void s_cch_msg_type_c::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::meas_report_sidelink:
      j.write_fieldname("measurementReportSidelink");
      c.get<meas_report_sidelink_s>().to_json(j);
      break;
    case types::rrc_recfg_sidelink:
      j.write_fieldname("rrcReconfigurationSidelink");
      c.get<rrc_recfg_sidelink_s>().to_json(j);
      break;
    case types::rrc_recfg_complete_sidelink:
      j.write_fieldname("rrcReconfigurationCompleteSidelink");
      c.get<rrc_recfg_complete_sidelink_s>().to_json(j);
      break;
    case types::rrc_recfg_fail_sidelink:
      j.write_fieldname("rrcReconfigurationFailureSidelink");
      c.get<rrc_recfg_fail_sidelink_s>().to_json(j);
      break;
    case types::ue_cap_enquiry_sidelink:
      j.write_fieldname("ueCapabilityEnquirySidelink");
      c.get<ue_cap_enquiry_sidelink_s>().to_json(j);
      break;
    case types::ue_cap_info_sidelink:
      j.write_fieldname("ueCapabilityInformationSidelink");
      c.get<ue_cap_info_sidelink_s>().to_json(j);
      break;
    case types::uu_msg_transfer_sidelink_r17:
      j.write_fieldname("uuMessageTransferSidelink-r17");
      c.get<uu_msg_transfer_sidelink_r17_s>().to_json(j);
      break;
    case types::remote_ue_info_sidelink_r17:
      j.write_fieldname("remoteUEInformationSidelink-r17");
      c.get<remote_ue_info_sidelink_r17_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE s_cch_msg_type_c::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::meas_report_sidelink:
      HANDLE_CODE(c.get<meas_report_sidelink_s>().pack(bref));
      break;
    case types::rrc_recfg_sidelink:
      HANDLE_CODE(c.get<rrc_recfg_sidelink_s>().pack(bref));
      break;
    case types::rrc_recfg_complete_sidelink:
      HANDLE_CODE(c.get<rrc_recfg_complete_sidelink_s>().pack(bref));
      break;
    case types::rrc_recfg_fail_sidelink:
      HANDLE_CODE(c.get<rrc_recfg_fail_sidelink_s>().pack(bref));
      break;
    case types::ue_cap_enquiry_sidelink:
      HANDLE_CODE(c.get<ue_cap_enquiry_sidelink_s>().pack(bref));
      break;
    case types::ue_cap_info_sidelink:
      HANDLE_CODE(c.get<ue_cap_info_sidelink_s>().pack(bref));
      break;
    case types::uu_msg_transfer_sidelink_r17:
      HANDLE_CODE(c.get<uu_msg_transfer_sidelink_r17_s>().pack(bref));
      break;
    case types::remote_ue_info_sidelink_r17:
      HANDLE_CODE(c.get<remote_ue_info_sidelink_r17_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE s_cch_msg_type_c::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::meas_report_sidelink:
      HANDLE_CODE(c.get<meas_report_sidelink_s>().unpack(bref));
      break;
    case types::rrc_recfg_sidelink:
      HANDLE_CODE(c.get<rrc_recfg_sidelink_s>().unpack(bref));
      break;
    case types::rrc_recfg_complete_sidelink:
      HANDLE_CODE(c.get<rrc_recfg_complete_sidelink_s>().unpack(bref));
      break;
    case types::rrc_recfg_fail_sidelink:
      HANDLE_CODE(c.get<rrc_recfg_fail_sidelink_s>().unpack(bref));
      break;
    case types::ue_cap_enquiry_sidelink:
      HANDLE_CODE(c.get<ue_cap_enquiry_sidelink_s>().unpack(bref));
      break;
    case types::ue_cap_info_sidelink:
      HANDLE_CODE(c.get<ue_cap_info_sidelink_s>().unpack(bref));
      break;
    case types::uu_msg_transfer_sidelink_r17:
      HANDLE_CODE(c.get<uu_msg_transfer_sidelink_r17_s>().unpack(bref));
      break;
    case types::remote_ue_info_sidelink_r17:
      HANDLE_CODE(c.get<remote_ue_info_sidelink_r17_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* s_cch_msg_type_c::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"measurementReportSidelink",
                                "rrcReconfigurationSidelink",
                                "rrcReconfigurationCompleteSidelink",
                                "rrcReconfigurationFailureSidelink",
                                "ueCapabilityEnquirySidelink",
                                "ueCapabilityInformationSidelink",
                                "uuMessageTransferSidelink-r17",
                                "remoteUEInformationSidelink-r17"};
  return convert_enum_idx(names, 8, value, "s_cch_msg_type_c::c1_c_::types");
}

void s_cch_msg_type_c::msg_class_ext_c_::set(types::options e)
{
  type_ = e;
}
s_cch_msg_type_c::msg_class_ext_c_::c2_c_& s_cch_msg_type_c::msg_class_ext_c_::set_c2()
{
  set(types::c2);
  return c;
}
void s_cch_msg_type_c::msg_class_ext_c_::set_msg_class_ext_future_r17()
{
  set(types::msg_class_ext_future_r17);
}
void s_cch_msg_type_c::msg_class_ext_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c2:
      j.write_fieldname("c2");
      c.to_json(j);
      break;
    case types::msg_class_ext_future_r17:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE s_cch_msg_type_c::msg_class_ext_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c2:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::msg_class_ext_future_r17:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE s_cch_msg_type_c::msg_class_ext_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c2:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::msg_class_ext_future_r17:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void s_cch_msg_type_c::msg_class_ext_c_::c2_c_::destroy_()
{
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      c.destroy<notif_msg_sidelink_r17_s>();
      break;
    case types::ue_assist_info_sidelink_r17:
      c.destroy<ue_assist_info_sidelink_r17_s>();
      break;
    case types::ue_info_request_sidelink_r18:
      c.destroy<ue_info_request_sidelink_r18_s>();
      break;
    case types::ue_info_resp_sidelink_r18:
      c.destroy<ue_info_resp_sidelink_r18_s>();
      break;
    default:
      break;
  }
}
void s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      c.init<notif_msg_sidelink_r17_s>();
      break;
    case types::ue_assist_info_sidelink_r17:
      c.init<ue_assist_info_sidelink_r17_s>();
      break;
    case types::ue_info_request_sidelink_r18:
      c.init<ue_info_request_sidelink_r18_s>();
      break;
    case types::ue_info_resp_sidelink_r18:
      c.init<ue_info_resp_sidelink_r18_s>();
      break;
    case types::spare4:
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_::c2_c_");
  }
}
s_cch_msg_type_c::msg_class_ext_c_::c2_c_::c2_c_(const s_cch_msg_type_c::msg_class_ext_c_::c2_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      c.init(other.c.get<notif_msg_sidelink_r17_s>());
      break;
    case types::ue_assist_info_sidelink_r17:
      c.init(other.c.get<ue_assist_info_sidelink_r17_s>());
      break;
    case types::ue_info_request_sidelink_r18:
      c.init(other.c.get<ue_info_request_sidelink_r18_s>());
      break;
    case types::ue_info_resp_sidelink_r18:
      c.init(other.c.get<ue_info_resp_sidelink_r18_s>());
      break;
    case types::spare4:
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_::c2_c_");
  }
}
s_cch_msg_type_c::msg_class_ext_c_::c2_c_&
s_cch_msg_type_c::msg_class_ext_c_::c2_c_::operator=(const s_cch_msg_type_c::msg_class_ext_c_::c2_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      c.set(other.c.get<notif_msg_sidelink_r17_s>());
      break;
    case types::ue_assist_info_sidelink_r17:
      c.set(other.c.get<ue_assist_info_sidelink_r17_s>());
      break;
    case types::ue_info_request_sidelink_r18:
      c.set(other.c.get<ue_info_request_sidelink_r18_s>());
      break;
    case types::ue_info_resp_sidelink_r18:
      c.set(other.c.get<ue_info_resp_sidelink_r18_s>());
      break;
    case types::spare4:
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_::c2_c_");
  }

  return *this;
}
notif_msg_sidelink_r17_s& s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set_notif_msg_sidelink_r17()
{
  set(types::notif_msg_sidelink_r17);
  return c.get<notif_msg_sidelink_r17_s>();
}
ue_assist_info_sidelink_r17_s& s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set_ue_assist_info_sidelink_r17()
{
  set(types::ue_assist_info_sidelink_r17);
  return c.get<ue_assist_info_sidelink_r17_s>();
}
ue_info_request_sidelink_r18_s& s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set_ue_info_request_sidelink_r18()
{
  set(types::ue_info_request_sidelink_r18);
  return c.get<ue_info_request_sidelink_r18_s>();
}
ue_info_resp_sidelink_r18_s& s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set_ue_info_resp_sidelink_r18()
{
  set(types::ue_info_resp_sidelink_r18);
  return c.get<ue_info_resp_sidelink_r18_s>();
}
void s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set_spare4()
{
  set(types::spare4);
}
void s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set_spare3()
{
  set(types::spare3);
}
void s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set_spare2()
{
  set(types::spare2);
}
void s_cch_msg_type_c::msg_class_ext_c_::c2_c_::set_spare1()
{
  set(types::spare1);
}
void s_cch_msg_type_c::msg_class_ext_c_::c2_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      j.write_fieldname("notificationMessageSidelink-r17");
      c.get<notif_msg_sidelink_r17_s>().to_json(j);
      break;
    case types::ue_assist_info_sidelink_r17:
      j.write_fieldname("ueAssistanceInformationSidelink-r17");
      c.get<ue_assist_info_sidelink_r17_s>().to_json(j);
      break;
    case types::ue_info_request_sidelink_r18:
      j.write_fieldname("ueInformationRequestSidelink-r18");
      c.get<ue_info_request_sidelink_r18_s>().to_json(j);
      break;
    case types::ue_info_resp_sidelink_r18:
      j.write_fieldname("ueInformationResponseSidelink-r18");
      c.get<ue_info_resp_sidelink_r18_s>().to_json(j);
      break;
    case types::spare4:
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_::c2_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE s_cch_msg_type_c::msg_class_ext_c_::c2_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      HANDLE_CODE(c.get<notif_msg_sidelink_r17_s>().pack(bref));
      break;
    case types::ue_assist_info_sidelink_r17:
      HANDLE_CODE(c.get<ue_assist_info_sidelink_r17_s>().pack(bref));
      break;
    case types::ue_info_request_sidelink_r18:
      HANDLE_CODE(c.get<ue_info_request_sidelink_r18_s>().pack(bref));
      break;
    case types::ue_info_resp_sidelink_r18:
      HANDLE_CODE(c.get<ue_info_resp_sidelink_r18_s>().pack(bref));
      break;
    case types::spare4:
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_::c2_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE s_cch_msg_type_c::msg_class_ext_c_::c2_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::notif_msg_sidelink_r17:
      HANDLE_CODE(c.get<notif_msg_sidelink_r17_s>().unpack(bref));
      break;
    case types::ue_assist_info_sidelink_r17:
      HANDLE_CODE(c.get<ue_assist_info_sidelink_r17_s>().unpack(bref));
      break;
    case types::ue_info_request_sidelink_r18:
      HANDLE_CODE(c.get<ue_info_request_sidelink_r18_s>().unpack(bref));
      break;
    case types::ue_info_resp_sidelink_r18:
      HANDLE_CODE(c.get<ue_info_resp_sidelink_r18_s>().unpack(bref));
      break;
    case types::spare4:
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "s_cch_msg_type_c::msg_class_ext_c_::c2_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* s_cch_msg_type_c::msg_class_ext_c_::c2_c_::types_opts::to_string() const
{
  static const char* names[] = {"notificationMessageSidelink-r17",
                                "ueAssistanceInformationSidelink-r17",
                                "ueInformationRequestSidelink-r18",
                                "ueInformationResponseSidelink-r18",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(names, 8, value, "s_cch_msg_type_c::msg_class_ext_c_::c2_c_::types");
}

const char* s_cch_msg_type_c::msg_class_ext_c_::types_opts::to_string() const
{
  static const char* names[] = {"c2", "messageClassExtensionFuture-r17"};
  return convert_enum_idx(names, 2, value, "s_cch_msg_type_c::msg_class_ext_c_::types");
}
uint8_t s_cch_msg_type_c::msg_class_ext_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "s_cch_msg_type_c::msg_class_ext_c_::types");
}

const char* s_cch_msg_type_c::types_opts::to_string() const
{
  static const char* names[] = {"c1", "messageClassExtension"};
  return convert_enum_idx(names, 2, value, "s_cch_msg_type_c::types");
}
uint8_t s_cch_msg_type_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "s_cch_msg_type_c::types");
}

// SCCH-Message ::= SEQUENCE
OCUDUASN_CODE s_cch_msg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(msg.pack(bref));

  bref.align_bytes_zero();

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE s_cch_msg_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(msg.unpack(bref));

  bref.align_bytes();

  return OCUDUASN_SUCCESS;
}
void s_cch_msg_s::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.start_obj("SCCH-Message");
  j.write_fieldname("message");
  msg.to_json(j);
  j.end_obj();
  j.end_obj();
  j.end_array();
}

// SL-MeasResultRelay-r17 ::= SEQUENCE
OCUDUASN_CODE sl_meas_result_relay_r17_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= sl_meas_quant_r18_present;
  group_flags[0] |= sl_relay_ind_mp_r18_present;
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(cell_id_r17.pack(bref));
  HANDLE_CODE(sl_relay_ue_id_r17.pack(bref));
  HANDLE_CODE(sl_meas_result_r17.pack(bref));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_meas_quant_r18_present, 1));
      HANDLE_CODE(bref.pack(sl_relay_ind_mp_r18_present, 1));
      if (sl_meas_quant_r18_present) {
        HANDLE_CODE(sl_meas_quant_r18.pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_meas_result_relay_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(cell_id_r17.unpack(bref));
  HANDLE_CODE(sl_relay_ue_id_r17.unpack(bref));
  HANDLE_CODE(sl_meas_result_r17.unpack(bref));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(sl_meas_quant_r18_present, 1));
      HANDLE_CODE(bref.unpack(sl_relay_ind_mp_r18_present, 1));
      if (sl_meas_quant_r18_present) {
        HANDLE_CODE(sl_meas_quant_r18.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_meas_result_relay_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("cellIdentity-r17");
  cell_id_r17.to_json(j);
  j.write_str("sl-RelayUE-Identity-r17", sl_relay_ue_id_r17.to_string());
  j.write_fieldname("sl-MeasResult-r17");
  sl_meas_result_r17.to_json(j);
  if (sl_meas_quant_r18_present) {
    j.write_str("sl-MeasQuantity-r18", sl_meas_quant_r18.to_string());
  }
  if (sl_relay_ind_mp_r18_present) {
    j.write_str("sl-RelayIndicationMP-r18", "support");
  }
  j.end_obj();
}

const char* sl_meas_result_relay_r17_s::sl_meas_quant_r18_opts::to_string() const
{
  static const char* names[] = {"sl-rsrp", "sd-rsrp"};
  return convert_enum_idx(names, 2, value, "sl_meas_result_relay_r17_s::sl_meas_quant_r18_e_");
}
