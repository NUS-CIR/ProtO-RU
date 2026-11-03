// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/rrc_nr/rrc_nr.h"

using namespace asn1;
using namespace asn1::rrc_nr;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// AreaValidityTA-Config-r18 ::= SEQUENCE
OCUDUASN_CODE area_validity_ta_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(
      pack_bits(bref, {inactive_pos_srs_validity_area_rsrp_r18_present, autonomous_ta_adjustment_enabled_r18_present}));

  HANDLE_CODE(inactive_pos_srs_validity_area_tat_r18.pack(bref));
  if (inactive_pos_srs_validity_area_rsrp_r18_present) {
    HANDLE_CODE(inactive_pos_srs_validity_area_rsrp_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE area_validity_ta_cfg_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(
      bref, {&inactive_pos_srs_validity_area_rsrp_r18_present, &autonomous_ta_adjustment_enabled_r18_present}));

  HANDLE_CODE(inactive_pos_srs_validity_area_tat_r18.unpack(bref));
  if (inactive_pos_srs_validity_area_rsrp_r18_present) {
    HANDLE_CODE(inactive_pos_srs_validity_area_rsrp_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void area_validity_ta_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("inactivePosSRS-ValidityAreaTAT-r18", inactive_pos_srs_validity_area_tat_r18.to_string());
  if (inactive_pos_srs_validity_area_rsrp_r18_present) {
    j.write_str("inactivePosSRS-ValidityAreaRSRP-r18", inactive_pos_srs_validity_area_rsrp_r18.to_string());
  }
  if (autonomous_ta_adjustment_enabled_r18_present) {
    j.write_str("autonomousTA-AdjustmentEnabled-r18", "true");
  }
  j.end_obj();
}

const char* area_validity_ta_cfg_r18_s::inactive_pos_srs_validity_area_tat_r18_opts::to_string() const
{
  static const char* names[] = {"ms1280", "ms1920", "ms2560", "ms5120", "ms10240", "ms20480", "ms40960", "infinity"};
  return convert_enum_idx(names, 8, value, "area_validity_ta_cfg_r18_s::inactive_pos_srs_validity_area_tat_r18_e_");
}
int32_t area_validity_ta_cfg_r18_s::inactive_pos_srs_validity_area_tat_r18_opts::to_number() const
{
  static const int32_t numbers[] = {1280, 1920, 2560, 5120, 10240, 20480, 40960, -1};
  return map_enum_number(numbers, 8, value, "area_validity_ta_cfg_r18_s::inactive_pos_srs_validity_area_tat_r18_e_");
}

// SRS-PosConfigPerULCarrier-r18 ::= SEQUENCE
OCUDUASN_CODE srs_pos_cfg_per_ul_carrier_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, scs_specific_carrier_r18_present, bwp_r18_present}));

  HANDLE_CODE(pack_integer(bref, freq_info_r18, (uint32_t)0u, (uint32_t)3279165u));
  HANDLE_CODE(srs_pos_cfg_r18.pack(bref));
  if (scs_specific_carrier_r18_present) {
    HANDLE_CODE(scs_specific_carrier_r18.pack(bref));
  }
  if (bwp_r18_present) {
    HANDLE_CODE(bwp_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_pos_cfg_per_ul_carrier_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &scs_specific_carrier_r18_present, &bwp_r18_present}));

  HANDLE_CODE(unpack_integer(freq_info_r18, bref, (uint32_t)0u, (uint32_t)3279165u));
  HANDLE_CODE(srs_pos_cfg_r18.unpack(bref));
  if (scs_specific_carrier_r18_present) {
    HANDLE_CODE(scs_specific_carrier_r18.unpack(bref));
  }
  if (bwp_r18_present) {
    HANDLE_CODE(bwp_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_pos_cfg_per_ul_carrier_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("freqInfo-r18", freq_info_r18);
  j.write_fieldname("srs-PosConfig-r18");
  srs_pos_cfg_r18.to_json(j);
  if (scs_specific_carrier_r18_present) {
    j.write_fieldname("scs-SpecificCarrier-r18");
    scs_specific_carrier_r18.to_json(j);
  }
  if (bwp_r18_present) {
    j.write_fieldname("bwp-r18");
    bwp_r18.to_json(j);
  }
  j.end_obj();
}

// SRS-PosRRC-InactiveAggBW-AdditionalCarriers-r18 ::= SEQUENCE
OCUDUASN_CODE srs_pos_rrc_inactive_agg_bw_add_carriers_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, aggr_pos_srs_carrier_list_r18.size() > 0}));

  if (aggr_pos_srs_carrier_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, aggr_pos_srs_carrier_list_r18, 1, 2));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_pos_rrc_inactive_agg_bw_add_carriers_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool aggr_pos_srs_carrier_list_r18_present;
  HANDLE_CODE(unpack_bits(bref, {&ext, &aggr_pos_srs_carrier_list_r18_present}));

  if (aggr_pos_srs_carrier_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(aggr_pos_srs_carrier_list_r18, bref, 1, 2));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_pos_rrc_inactive_agg_bw_add_carriers_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (aggr_pos_srs_carrier_list_r18.size() > 0) {
    j.start_array("aggregatedPosSRS-CarrierList-r18");
    for (const auto& e1 : aggr_pos_srs_carrier_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// SRS-PosRRC-InactiveValidityAreaConfig-r18 ::= SEQUENCE
OCUDUASN_CODE srs_pos_rrc_inactive_validity_area_cfg_r18_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(2);
  group_flags[0] |= srs_pos_cfg_validity_area_ext_v1830.is_present();
  group_flags[1] |= srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18.is_present();
  group_flags[1] |= srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18.is_present();
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         srs_pos_cfg_n_ul_r18_present,
                         srs_pos_cfg_sul_r18_present,
                         bwp_n_ul_r18_present,
                         bwp_sul_r18_present,
                         area_validity_ta_cfg_r18_present}));

  HANDLE_CODE(pack_dyn_seq_of(bref, srs_pos_cfg_validity_area_r18, 1, 16));
  if (srs_pos_cfg_n_ul_r18_present) {
    HANDLE_CODE(srs_pos_cfg_n_ul_r18.pack(bref));
  }
  if (srs_pos_cfg_sul_r18_present) {
    HANDLE_CODE(srs_pos_cfg_sul_r18.pack(bref));
  }
  if (bwp_n_ul_r18_present) {
    HANDLE_CODE(bwp_n_ul_r18.pack(bref));
  }
  if (bwp_sul_r18_present) {
    HANDLE_CODE(bwp_sul_r18.pack(bref));
  }
  if (area_validity_ta_cfg_r18_present) {
    HANDLE_CODE(area_validity_ta_cfg_r18.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(srs_pos_cfg_validity_area_ext_v1830.is_present(), 1));
      if (srs_pos_cfg_validity_area_ext_v1830.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *srs_pos_cfg_validity_area_ext_v1830, 1, 16));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18.is_present(),
                             srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18.is_present()}));
      if (srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18.is_present()) {
        HANDLE_CODE(srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18->pack(bref));
      }
      if (srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18.is_present()) {
        HANDLE_CODE(srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_pos_rrc_inactive_validity_area_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &srs_pos_cfg_n_ul_r18_present,
                           &srs_pos_cfg_sul_r18_present,
                           &bwp_n_ul_r18_present,
                           &bwp_sul_r18_present,
                           &area_validity_ta_cfg_r18_present}));

  HANDLE_CODE(unpack_dyn_seq_of(srs_pos_cfg_validity_area_r18, bref, 1, 16));
  if (srs_pos_cfg_n_ul_r18_present) {
    HANDLE_CODE(srs_pos_cfg_n_ul_r18.unpack(bref));
  }
  if (srs_pos_cfg_sul_r18_present) {
    HANDLE_CODE(srs_pos_cfg_sul_r18.unpack(bref));
  }
  if (bwp_n_ul_r18_present) {
    HANDLE_CODE(bwp_n_ul_r18.unpack(bref));
  }
  if (bwp_sul_r18_present) {
    HANDLE_CODE(bwp_sul_r18.unpack(bref));
  }
  if (area_validity_ta_cfg_r18_present) {
    HANDLE_CODE(area_validity_ta_cfg_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(srs_pos_cfg_validity_area_ext_v1830, bref);
      if (srs_pos_cfg_validity_area_ext_v1830.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*srs_pos_cfg_validity_area_ext_v1830, bref, 1, 16));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18, bref);
      unpack_presence_flag(srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18, bref);
      if (srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18.is_present()) {
        HANDLE_CODE(srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18->unpack(bref));
      }
      if (srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18.is_present()) {
        HANDLE_CODE(srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_pos_rrc_inactive_validity_area_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("srs-PosConfigValidityArea-r18");
  for (const auto& e1 : srs_pos_cfg_validity_area_r18) {
    e1.to_json(j);
  }
  j.end_array();
  if (srs_pos_cfg_n_ul_r18_present) {
    j.write_fieldname("srs-PosConfigNUL-r18");
    srs_pos_cfg_n_ul_r18.to_json(j);
  }
  if (srs_pos_cfg_sul_r18_present) {
    j.write_fieldname("srs-PosConfigSUL-r18");
    srs_pos_cfg_sul_r18.to_json(j);
  }
  if (bwp_n_ul_r18_present) {
    j.write_fieldname("bwp-NUL-r18");
    bwp_n_ul_r18.to_json(j);
  }
  if (bwp_sul_r18_present) {
    j.write_fieldname("bwp-SUL-r18");
    bwp_sul_r18.to_json(j);
  }
  if (area_validity_ta_cfg_r18_present) {
    j.write_fieldname("areaValidityTA-Config-r18");
    area_validity_ta_cfg_r18.to_json(j);
  }
  if (srs_pos_cfg_validity_area_ext_v1830.is_present()) {
    j.start_array("srs-PosConfigValidityAreaExt-v1830");
    for (const auto& e1 : *srs_pos_cfg_validity_area_ext_v1830) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18.is_present()) {
    j.write_fieldname("srs-PosRRC-InactiveAggBW-AdditionalCarriersPerVA-r18");
    srs_pos_rrc_inactive_agg_bw_add_carriers_per_va_r18->to_json(j);
  }
  if (srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18.is_present()) {
    j.write_fieldname("srs-PosRRC-InactiveAggBW-ConfigListPerVA-r18");
    srs_pos_rrc_inactive_agg_bw_cfg_list_per_va_r18->to_json(j);
  }
  j.end_obj();
}

// AS-Config ::= SEQUENCE
OCUDUASN_CODE as_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(4);
  group_flags[0] |= source_rb_sn_cfg.size() > 0;
  group_flags[0] |= source_scg_nr_cfg.size() > 0;
  group_flags[0] |= source_scg_eutra_cfg.size() > 0;
  group_flags[1] |= source_scg_cfg_present;
  group_flags[2] |= sdt_cfg_r17.is_present();
  group_flags[3] |= srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18.is_present();
  HANDLE_CODE(bref.pack(group_flags.any(), 1));

  HANDLE_CODE(rrc_recfg.pack(bref));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(
          bref, {source_rb_sn_cfg.size() > 0, source_scg_nr_cfg.size() > 0, source_scg_eutra_cfg.size() > 0}));
      if (source_rb_sn_cfg.size() > 0) {
        HANDLE_CODE(source_rb_sn_cfg.pack(bref));
      }
      if (source_scg_nr_cfg.size() > 0) {
        HANDLE_CODE(source_scg_nr_cfg.pack(bref));
      }
      if (source_scg_eutra_cfg.size() > 0) {
        HANDLE_CODE(source_scg_eutra_cfg.pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(source_scg_cfg_present, 1));
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sdt_cfg_r17.is_present(), 1));
      if (sdt_cfg_r17.is_present()) {
        HANDLE_CODE(sdt_cfg_r17->pack(bref));
      }
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18.is_present(), 1));
      if (srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18, 1, 16));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE as_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(rrc_recfg.unpack(bref));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      bool source_rb_sn_cfg_present;
      bool source_scg_nr_cfg_present;
      bool source_scg_eutra_cfg_present;
      HANDLE_CODE(
          unpack_bits(bref, {&source_rb_sn_cfg_present, &source_scg_nr_cfg_present, &source_scg_eutra_cfg_present}));
      if (source_rb_sn_cfg_present) {
        HANDLE_CODE(source_rb_sn_cfg.unpack(bref));
      }
      if (source_scg_nr_cfg_present) {
        HANDLE_CODE(source_scg_nr_cfg.unpack(bref));
      }
      if (source_scg_eutra_cfg_present) {
        HANDLE_CODE(source_scg_eutra_cfg.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(source_scg_cfg_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sdt_cfg_r17, bref);
      if (sdt_cfg_r17.is_present()) {
        HANDLE_CODE(sdt_cfg_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18, bref);
      if (srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18, bref, 1, 16));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void as_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("rrcReconfiguration", rrc_recfg.to_string());
  if (source_rb_sn_cfg.size() > 0) {
    j.write_str("sourceRB-SN-Config", source_rb_sn_cfg.to_string());
  }
  if (source_scg_nr_cfg.size() > 0) {
    j.write_str("sourceSCG-NR-Config", source_scg_nr_cfg.to_string());
  }
  if (source_scg_eutra_cfg.size() > 0) {
    j.write_str("sourceSCG-EUTRA-Config", source_scg_eutra_cfg.to_string());
  }
  if (source_scg_cfg_present) {
    j.write_str("sourceSCG-Configured", "true");
  }
  if (sdt_cfg_r17.is_present()) {
    j.write_fieldname("sdt-Config-r17");
    sdt_cfg_r17->to_json(j);
  }
  if (srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18.is_present()) {
    j.start_array("srs-PosRRC-InactiveValidityAreaPreConfigList-r18");
    for (const auto& e1 : *srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// ReestabNCellInfo ::= SEQUENCE
OCUDUASN_CODE reestab_ncell_info_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(cell_id.pack(bref));
  HANDLE_CODE(key_g_node_b_star.pack(bref));
  HANDLE_CODE(short_mac_i.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reestab_ncell_info_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(cell_id.unpack(bref));
  HANDLE_CODE(key_g_node_b_star.unpack(bref));
  HANDLE_CODE(short_mac_i.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void reestab_ncell_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("cellIdentity");
  cell_id.to_json(j);
  j.write_fieldname("key-gNodeB-Star");
  key_g_node_b_star.to_json(j);
  j.write_fieldname("shortMAC-I");
  short_mac_i.to_json(j);
  j.end_obj();
}

// ReestablishmentInfo ::= SEQUENCE
OCUDUASN_CODE reest_info_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(add_reestab_info_list.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, source_pci, (uint16_t)0u, (uint16_t)1007u));
  HANDLE_CODE(target_cell_short_mac_i.pack(bref));
  if (add_reestab_info_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, add_reestab_info_list, 1, 32));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reest_info_s::unpack(cbit_ref& bref)
{
  bool add_reestab_info_list_present;
  HANDLE_CODE(bref.unpack(add_reestab_info_list_present, 1));

  HANDLE_CODE(unpack_integer(source_pci, bref, (uint16_t)0u, (uint16_t)1007u));
  HANDLE_CODE(target_cell_short_mac_i.unpack(bref));
  if (add_reestab_info_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(add_reestab_info_list, bref, 1, 32));
  }

  return OCUDUASN_SUCCESS;
}
void reest_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sourcePhysCellId", source_pci);
  j.write_fieldname("targetCellShortMAC-I");
  target_cell_short_mac_i.to_json(j);
  if (add_reestab_info_list.size() > 0) {
    j.start_array("additionalReestabInfoList");
    for (const auto& e1 : add_reestab_info_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// BandCombinationInfoSN ::= SEQUENCE
OCUDUASN_CODE band_combination_info_sn_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, band_combination_idx, (uint32_t)1u, (uint32_t)65536u));
  HANDLE_CODE(pack_integer(bref, requested_feature_sets, (uint8_t)1u, (uint8_t)128u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_combination_info_sn_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(band_combination_idx, bref, (uint32_t)1u, (uint32_t)65536u));
  HANDLE_CODE(unpack_integer(requested_feature_sets, bref, (uint8_t)1u, (uint8_t)128u));

  return OCUDUASN_SUCCESS;
}
void band_combination_info_sn_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("bandCombinationIndex", band_combination_idx);
  j.write_int("requestedFeatureSets", requested_feature_sets);
  j.end_obj();
}

// ConfigRestrictInfoDAPS-r16 ::= SEQUENCE
OCUDUASN_CODE cfg_restrict_info_daps_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(pwr_coordination_r16_present, 1));

  if (pwr_coordination_r16_present) {
    HANDLE_CODE(pack_integer(bref, pwr_coordination_r16.p_daps_source_r16, (int8_t)-30, (int8_t)33));
    HANDLE_CODE(pack_integer(bref, pwr_coordination_r16.p_daps_target_r16, (int8_t)-30, (int8_t)33));
    HANDLE_CODE(pwr_coordination_r16.ul_pwr_sharing_daps_mode_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_restrict_info_daps_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(pwr_coordination_r16_present, 1));

  if (pwr_coordination_r16_present) {
    HANDLE_CODE(unpack_integer(pwr_coordination_r16.p_daps_source_r16, bref, (int8_t)-30, (int8_t)33));
    HANDLE_CODE(unpack_integer(pwr_coordination_r16.p_daps_target_r16, bref, (int8_t)-30, (int8_t)33));
    HANDLE_CODE(pwr_coordination_r16.ul_pwr_sharing_daps_mode_r16.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cfg_restrict_info_daps_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pwr_coordination_r16_present) {
    j.write_fieldname("powerCoordination-r16");
    j.start_obj();
    j.write_int("p-DAPS-Source-r16", pwr_coordination_r16.p_daps_source_r16);
    j.write_int("p-DAPS-Target-r16", pwr_coordination_r16.p_daps_target_r16);
    j.write_str("uplinkPowerSharingDAPS-Mode-r16", pwr_coordination_r16.ul_pwr_sharing_daps_mode_r16.to_string());
    j.end_obj();
  }
  j.end_obj();
}

const char* cfg_restrict_info_daps_r16_s::pwr_coordination_r16_s_::ul_pwr_sharing_daps_mode_r16_opts::to_string() const
{
  static const char* names[] = {"semi-static-mode1", "semi-static-mode2", "dynamic"};
  return convert_enum_idx(
      names, 3, value, "cfg_restrict_info_daps_r16_s::pwr_coordination_r16_s_::ul_pwr_sharing_daps_mode_r16_e_");
}
uint8_t cfg_restrict_info_daps_r16_s::pwr_coordination_r16_s_::ul_pwr_sharing_daps_mode_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(
      numbers, 2, value, "cfg_restrict_info_daps_r16_s::pwr_coordination_r16_s_::ul_pwr_sharing_daps_mode_r16_e_");
}

// ConfigRestrictInfoDAPS-v1640 ::= SEQUENCE
OCUDUASN_CODE cfg_restrict_info_daps_v1640_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, source_feature_set_per_dl_cc_r16, (uint16_t)1u, (uint16_t)1024u));
  HANDLE_CODE(pack_integer(bref, source_feature_set_per_ul_cc_r16, (uint16_t)1u, (uint16_t)1024u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_restrict_info_daps_v1640_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(source_feature_set_per_dl_cc_r16, bref, (uint16_t)1u, (uint16_t)1024u));
  HANDLE_CODE(unpack_integer(source_feature_set_per_ul_cc_r16, bref, (uint16_t)1u, (uint16_t)1024u));

  return OCUDUASN_SUCCESS;
}
void cfg_restrict_info_daps_v1640_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sourceFeatureSetPerDownlinkCC-r16", source_feature_set_per_dl_cc_r16);
  j.write_int("sourceFeatureSetPerUplinkCC-r16", source_feature_set_per_ul_cc_r16);
  j.end_obj();
}

// AS-Context ::= SEQUENCE
OCUDUASN_CODE as_context_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(7);
  group_flags[0] |= ran_notif_area_info.is_present();
  group_flags[1] |= ue_assist_info.size() > 0;
  group_flags[2] |= sel_band_combination_sn.is_present();
  group_flags[3] |= cfg_restrict_info_daps_r16.is_present();
  group_flags[3] |= sidelink_ue_info_nr_r16.size() > 0;
  group_flags[3] |= sidelink_ue_info_eutra_r16.size() > 0;
  group_flags[3] |= ue_assist_info_eutra_r16.size() > 0;
  group_flags[3] |= ue_assist_info_scg_r16.size() > 0;
  group_flags[3] |= need_for_gaps_info_nr_r16.is_present();
  group_flags[4] |= cfg_restrict_info_daps_v1640.is_present();
  group_flags[5] |= need_for_gap_ncsg_info_nr_r17.is_present();
  group_flags[5] |= need_for_gap_ncsg_info_eutra_r17.is_present();
  group_flags[5] |= mbs_interest_ind_r17.size() > 0;
  group_flags[6] |= need_for_interruption_info_nr_r18.is_present();
  group_flags[6] |= flight_path_info_report_r18.is_present();
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), reest_info_present, cfg_restrict_info_present}));

  if (reest_info_present) {
    HANDLE_CODE(reest_info.pack(bref));
  }
  if (cfg_restrict_info_present) {
    HANDLE_CODE(cfg_restrict_info.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(ran_notif_area_info.is_present(), 1));
      if (ran_notif_area_info.is_present()) {
        HANDLE_CODE(ran_notif_area_info->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(ue_assist_info.size() > 0, 1));
      if (ue_assist_info.size() > 0) {
        HANDLE_CODE(ue_assist_info.pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sel_band_combination_sn.is_present(), 1));
      if (sel_band_combination_sn.is_present()) {
        HANDLE_CODE(sel_band_combination_sn->pack(bref));
      }
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {cfg_restrict_info_daps_r16.is_present(),
                             sidelink_ue_info_nr_r16.size() > 0,
                             sidelink_ue_info_eutra_r16.size() > 0,
                             ue_assist_info_eutra_r16.size() > 0,
                             ue_assist_info_scg_r16.size() > 0,
                             need_for_gaps_info_nr_r16.is_present()}));
      if (cfg_restrict_info_daps_r16.is_present()) {
        HANDLE_CODE(cfg_restrict_info_daps_r16->pack(bref));
      }
      if (sidelink_ue_info_nr_r16.size() > 0) {
        HANDLE_CODE(sidelink_ue_info_nr_r16.pack(bref));
      }
      if (sidelink_ue_info_eutra_r16.size() > 0) {
        HANDLE_CODE(sidelink_ue_info_eutra_r16.pack(bref));
      }
      if (ue_assist_info_eutra_r16.size() > 0) {
        HANDLE_CODE(ue_assist_info_eutra_r16.pack(bref));
      }
      if (ue_assist_info_scg_r16.size() > 0) {
        HANDLE_CODE(ue_assist_info_scg_r16.pack(bref));
      }
      if (need_for_gaps_info_nr_r16.is_present()) {
        HANDLE_CODE(need_for_gaps_info_nr_r16->pack(bref));
      }
    }
    if (group_flags[4]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(cfg_restrict_info_daps_v1640.is_present(), 1));
      if (cfg_restrict_info_daps_v1640.is_present()) {
        HANDLE_CODE(cfg_restrict_info_daps_v1640->pack(bref));
      }
    }
    if (group_flags[5]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {need_for_gap_ncsg_info_nr_r17.is_present(),
                             need_for_gap_ncsg_info_eutra_r17.is_present(),
                             mbs_interest_ind_r17.size() > 0}));
      if (need_for_gap_ncsg_info_nr_r17.is_present()) {
        HANDLE_CODE(need_for_gap_ncsg_info_nr_r17->pack(bref));
      }
      if (need_for_gap_ncsg_info_eutra_r17.is_present()) {
        HANDLE_CODE(need_for_gap_ncsg_info_eutra_r17->pack(bref));
      }
      if (mbs_interest_ind_r17.size() > 0) {
        HANDLE_CODE(mbs_interest_ind_r17.pack(bref));
      }
    }
    if (group_flags[6]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(
          pack_bits(bref, {need_for_interruption_info_nr_r18.is_present(), flight_path_info_report_r18.is_present()}));
      if (need_for_interruption_info_nr_r18.is_present()) {
        HANDLE_CODE(need_for_interruption_info_nr_r18->pack(bref));
      }
      if (flight_path_info_report_r18.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *flight_path_info_report_r18, 0, 20));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE as_context_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &reest_info_present, &cfg_restrict_info_present}));

  if (reest_info_present) {
    HANDLE_CODE(reest_info.unpack(bref));
  }
  if (cfg_restrict_info_present) {
    HANDLE_CODE(cfg_restrict_info.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(ran_notif_area_info, bref);
      if (ran_notif_area_info.is_present()) {
        HANDLE_CODE(ran_notif_area_info->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      bool ue_assist_info_present;
      HANDLE_CODE(bref.unpack(ue_assist_info_present, 1));
      if (ue_assist_info_present) {
        HANDLE_CODE(ue_assist_info.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sel_band_combination_sn, bref);
      if (sel_band_combination_sn.is_present()) {
        HANDLE_CODE(sel_band_combination_sn->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(cfg_restrict_info_daps_r16, bref);
      bool sidelink_ue_info_nr_r16_present;
      bool sidelink_ue_info_eutra_r16_present;
      bool ue_assist_info_eutra_r16_present;
      bool ue_assist_info_scg_r16_present;
      HANDLE_CODE(unpack_bits(bref,
                              {&sidelink_ue_info_nr_r16_present,
                               &sidelink_ue_info_eutra_r16_present,
                               &ue_assist_info_eutra_r16_present,
                               &ue_assist_info_scg_r16_present}));
      unpack_presence_flag(need_for_gaps_info_nr_r16, bref);
      if (cfg_restrict_info_daps_r16.is_present()) {
        HANDLE_CODE(cfg_restrict_info_daps_r16->unpack(bref));
      }
      if (sidelink_ue_info_nr_r16_present) {
        HANDLE_CODE(sidelink_ue_info_nr_r16.unpack(bref));
      }
      if (sidelink_ue_info_eutra_r16_present) {
        HANDLE_CODE(sidelink_ue_info_eutra_r16.unpack(bref));
      }
      if (ue_assist_info_eutra_r16_present) {
        HANDLE_CODE(ue_assist_info_eutra_r16.unpack(bref));
      }
      if (ue_assist_info_scg_r16_present) {
        HANDLE_CODE(ue_assist_info_scg_r16.unpack(bref));
      }
      if (need_for_gaps_info_nr_r16.is_present()) {
        HANDLE_CODE(need_for_gaps_info_nr_r16->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(cfg_restrict_info_daps_v1640, bref);
      if (cfg_restrict_info_daps_v1640.is_present()) {
        HANDLE_CODE(cfg_restrict_info_daps_v1640->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(need_for_gap_ncsg_info_nr_r17, bref);
      unpack_presence_flag(need_for_gap_ncsg_info_eutra_r17, bref);
      bool mbs_interest_ind_r17_present;
      HANDLE_CODE(bref.unpack(mbs_interest_ind_r17_present, 1));
      if (need_for_gap_ncsg_info_nr_r17.is_present()) {
        HANDLE_CODE(need_for_gap_ncsg_info_nr_r17->unpack(bref));
      }
      if (need_for_gap_ncsg_info_eutra_r17.is_present()) {
        HANDLE_CODE(need_for_gap_ncsg_info_eutra_r17->unpack(bref));
      }
      if (mbs_interest_ind_r17_present) {
        HANDLE_CODE(mbs_interest_ind_r17.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(need_for_interruption_info_nr_r18, bref);
      unpack_presence_flag(flight_path_info_report_r18, bref);
      if (need_for_interruption_info_nr_r18.is_present()) {
        HANDLE_CODE(need_for_interruption_info_nr_r18->unpack(bref));
      }
      if (flight_path_info_report_r18.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*flight_path_info_report_r18, bref, 0, 20));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void as_context_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (reest_info_present) {
    j.write_fieldname("reestablishmentInfo");
    reest_info.to_json(j);
  }
  if (cfg_restrict_info_present) {
    j.write_fieldname("configRestrictInfo");
    cfg_restrict_info.to_json(j);
  }
  if (ran_notif_area_info.is_present()) {
    j.write_fieldname("ran-NotificationAreaInfo");
    ran_notif_area_info->to_json(j);
  }
  if (ue_assist_info.size() > 0) {
    j.write_str("ueAssistanceInformation", ue_assist_info.to_string());
  }
  if (sel_band_combination_sn.is_present()) {
    j.write_fieldname("selectedBandCombinationSN");
    sel_band_combination_sn->to_json(j);
  }
  if (cfg_restrict_info_daps_r16.is_present()) {
    j.write_fieldname("configRestrictInfoDAPS-r16");
    cfg_restrict_info_daps_r16->to_json(j);
  }
  if (sidelink_ue_info_nr_r16.size() > 0) {
    j.write_str("sidelinkUEInformationNR-r16", sidelink_ue_info_nr_r16.to_string());
  }
  if (sidelink_ue_info_eutra_r16.size() > 0) {
    j.write_str("sidelinkUEInformationEUTRA-r16", sidelink_ue_info_eutra_r16.to_string());
  }
  if (ue_assist_info_eutra_r16.size() > 0) {
    j.write_str("ueAssistanceInformationEUTRA-r16", ue_assist_info_eutra_r16.to_string());
  }
  if (ue_assist_info_scg_r16.size() > 0) {
    j.write_str("ueAssistanceInformationSCG-r16", ue_assist_info_scg_r16.to_string());
  }
  if (need_for_gaps_info_nr_r16.is_present()) {
    j.write_fieldname("needForGapsInfoNR-r16");
    need_for_gaps_info_nr_r16->to_json(j);
  }
  if (cfg_restrict_info_daps_v1640.is_present()) {
    j.write_fieldname("configRestrictInfoDAPS-v1640");
    cfg_restrict_info_daps_v1640->to_json(j);
  }
  if (need_for_gap_ncsg_info_nr_r17.is_present()) {
    j.write_fieldname("needForGapNCSG-InfoNR-r17");
    need_for_gap_ncsg_info_nr_r17->to_json(j);
  }
  if (need_for_gap_ncsg_info_eutra_r17.is_present()) {
    j.write_fieldname("needForGapNCSG-InfoEUTRA-r17");
    need_for_gap_ncsg_info_eutra_r17->to_json(j);
  }
  if (mbs_interest_ind_r17.size() > 0) {
    j.write_str("mbsInterestIndication-r17", mbs_interest_ind_r17.to_string());
  }
  if (need_for_interruption_info_nr_r18.is_present()) {
    j.write_fieldname("needForInterruptionInfoNR-r18");
    need_for_interruption_info_nr_r18->to_json(j);
  }
  if (flight_path_info_report_r18.is_present()) {
    j.start_array("flightPathInfoReport-r18");
    for (const auto& e1 : *flight_path_info_report_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// AggregatedBandwidthSN-r17 ::= SEQUENCE
OCUDUASN_CODE aggr_bw_sn_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {agg_bw_fdd_dl_r17_present,
                         agg_bw_fdd_ul_r17_present,
                         agg_bw_tdd_dl_r17_present,
                         agg_bw_tdd_ul_r17_present,
                         agg_bw_total_dl_r17_present,
                         agg_bw_total_ul_r17_present}));

  if (agg_bw_fdd_dl_r17_present) {
    HANDLE_CODE(agg_bw_fdd_dl_r17.pack(bref));
  }
  if (agg_bw_fdd_ul_r17_present) {
    HANDLE_CODE(agg_bw_fdd_ul_r17.pack(bref));
  }
  if (agg_bw_tdd_dl_r17_present) {
    HANDLE_CODE(agg_bw_tdd_dl_r17.pack(bref));
  }
  if (agg_bw_tdd_ul_r17_present) {
    HANDLE_CODE(agg_bw_tdd_ul_r17.pack(bref));
  }
  if (agg_bw_total_dl_r17_present) {
    HANDLE_CODE(agg_bw_total_dl_r17.pack(bref));
  }
  if (agg_bw_total_ul_r17_present) {
    HANDLE_CODE(agg_bw_total_ul_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aggr_bw_sn_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&agg_bw_fdd_dl_r17_present,
                           &agg_bw_fdd_ul_r17_present,
                           &agg_bw_tdd_dl_r17_present,
                           &agg_bw_tdd_ul_r17_present,
                           &agg_bw_total_dl_r17_present,
                           &agg_bw_total_ul_r17_present}));

  if (agg_bw_fdd_dl_r17_present) {
    HANDLE_CODE(agg_bw_fdd_dl_r17.unpack(bref));
  }
  if (agg_bw_fdd_ul_r17_present) {
    HANDLE_CODE(agg_bw_fdd_ul_r17.unpack(bref));
  }
  if (agg_bw_tdd_dl_r17_present) {
    HANDLE_CODE(agg_bw_tdd_dl_r17.unpack(bref));
  }
  if (agg_bw_tdd_ul_r17_present) {
    HANDLE_CODE(agg_bw_tdd_ul_r17.unpack(bref));
  }
  if (agg_bw_total_dl_r17_present) {
    HANDLE_CODE(agg_bw_total_dl_r17.unpack(bref));
  }
  if (agg_bw_total_ul_r17_present) {
    HANDLE_CODE(agg_bw_total_ul_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void aggr_bw_sn_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (agg_bw_fdd_dl_r17_present) {
    j.write_fieldname("aggBW-FDD-DL-r17");
    agg_bw_fdd_dl_r17.to_json(j);
  }
  if (agg_bw_fdd_ul_r17_present) {
    j.write_fieldname("aggBW-FDD-UL-r17");
    agg_bw_fdd_ul_r17.to_json(j);
  }
  if (agg_bw_tdd_dl_r17_present) {
    j.write_fieldname("aggBW-TDD-DL-r17");
    agg_bw_tdd_dl_r17.to_json(j);
  }
  if (agg_bw_tdd_ul_r17_present) {
    j.write_fieldname("aggBW-TDD-UL-r17");
    agg_bw_tdd_ul_r17.to_json(j);
  }
  if (agg_bw_total_dl_r17_present) {
    j.write_fieldname("aggBW-TotalDL-r17");
    agg_bw_total_dl_r17.to_json(j);
  }
  if (agg_bw_total_ul_r17_present) {
    j.write_fieldname("aggBW-TotalUL-r17");
    agg_bw_total_ul_r17.to_json(j);
  }
  j.end_obj();
}

// CG-CandidateInfoId-r17 ::= SEQUENCE
OCUDUASN_CODE cg_candidate_info_id_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, ssb_freq_r17, (uint32_t)0u, (uint32_t)3279165u));
  HANDLE_CODE(pack_integer(bref, pci_r17, (uint16_t)0u, (uint16_t)1007u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_candidate_info_id_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(ssb_freq_r17, bref, (uint32_t)0u, (uint32_t)3279165u));
  HANDLE_CODE(unpack_integer(pci_r17, bref, (uint16_t)0u, (uint16_t)1007u));

  return OCUDUASN_SUCCESS;
}
void cg_candidate_info_id_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ssbFrequency-r17", ssb_freq_r17);
  j.write_int("physCellId-r17", pci_r17);
  j.end_obj();
}

// CG-CandidateInfo-r17 ::= SEQUENCE
OCUDUASN_CODE cg_candidate_info_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(cg_candidate_info_id_r17.pack(bref));
  HANDLE_CODE(candidate_cg_cfg_r17.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_candidate_info_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(cg_candidate_info_id_r17.unpack(bref));
  HANDLE_CODE(candidate_cg_cfg_r17.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void cg_candidate_info_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("cg-CandidateInfoId-r17");
  cg_candidate_info_id_r17.to_json(j);
  j.write_str("candidateCG-Config-r17", candidate_cg_cfg_r17.to_string());
  j.end_obj();
}

// CG-CandidateList-r17-IEs ::= SEQUENCE
OCUDUASN_CODE cg_candidate_list_r17_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {cg_candidate_to_add_mod_list_r17.size() > 0,
                         cg_candidate_to_release_list_r17.size() > 0,
                         non_crit_ext_present}));

  if (cg_candidate_to_add_mod_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cg_candidate_to_add_mod_list_r17, 1, 8));
  }
  if (cg_candidate_to_release_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cg_candidate_to_release_list_r17, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_candidate_list_r17_ies_s::unpack(cbit_ref& bref)
{
  bool cg_candidate_to_add_mod_list_r17_present;
  bool cg_candidate_to_release_list_r17_present;
  HANDLE_CODE(unpack_bits(
      bref,
      {&cg_candidate_to_add_mod_list_r17_present, &cg_candidate_to_release_list_r17_present, &non_crit_ext_present}));

  if (cg_candidate_to_add_mod_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cg_candidate_to_add_mod_list_r17, bref, 1, 8));
  }
  if (cg_candidate_to_release_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cg_candidate_to_release_list_r17, bref, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
void cg_candidate_list_r17_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (cg_candidate_to_add_mod_list_r17.size() > 0) {
    j.start_array("cg-CandidateToAddModList-r17");
    for (const auto& e1 : cg_candidate_to_add_mod_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cg_candidate_to_release_list_r17.size() > 0) {
    j.start_array("cg-CandidateToReleaseList-r17");
    for (const auto& e1 : cg_candidate_to_release_list_r17) {
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

// CG-CandidateList ::= SEQUENCE
OCUDUASN_CODE cg_candidate_list_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_candidate_list_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void cg_candidate_list_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void cg_candidate_list_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
cg_candidate_list_s::crit_exts_c_::c1_c_& cg_candidate_list_s::crit_exts_c_::set_c1()
{
  set(types::c1);
  return c;
}
void cg_candidate_list_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void cg_candidate_list_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "cg_candidate_list_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE cg_candidate_list_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "cg_candidate_list_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_candidate_list_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "cg_candidate_list_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void cg_candidate_list_s::crit_exts_c_::c1_c_::set(types::options e)
{
  type_ = e;
}
cg_candidate_list_r17_ies_s& cg_candidate_list_s::crit_exts_c_::c1_c_::set_cg_candidate_list_r17()
{
  set(types::cg_candidate_list_r17);
  return c;
}
void cg_candidate_list_s::crit_exts_c_::c1_c_::set_spare3()
{
  set(types::spare3);
}
void cg_candidate_list_s::crit_exts_c_::c1_c_::set_spare2()
{
  set(types::spare2);
}
void cg_candidate_list_s::crit_exts_c_::c1_c_::set_spare1()
{
  set(types::spare1);
}
void cg_candidate_list_s::crit_exts_c_::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cg_candidate_list_r17:
      j.write_fieldname("cg-CandidateList-r17");
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
      log_invalid_choice_id(type_, "cg_candidate_list_s::crit_exts_c_::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE cg_candidate_list_s::crit_exts_c_::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::cg_candidate_list_r17:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "cg_candidate_list_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_candidate_list_s::crit_exts_c_::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::cg_candidate_list_r17:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "cg_candidate_list_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cg_candidate_list_s::crit_exts_c_::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"cg-CandidateList-r17", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 4, value, "cg_candidate_list_s::crit_exts_c_::c1_c_::types");
}

const char* cg_candidate_list_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"c1", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "cg_candidate_list_s::crit_exts_c_::types");
}
uint8_t cg_candidate_list_s::crit_exts_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "cg_candidate_list_s::crit_exts_c_::types");
}

// ConfigRestrictModReqSCG ::= SEQUENCE
OCUDUASN_CODE cfg_restrict_mod_req_scg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(7);
  group_flags[0] |= requested_pdcch_blind_detection_scg_present;
  group_flags[0] |= requested_p_max_eutra_present;
  group_flags[1] |= requested_p_max_fr2_r16_present;
  group_flags[1] |= requested_max_inter_freq_meas_id_scg_r16_present;
  group_flags[1] |= requested_max_intra_freq_meas_id_scg_r16_present;
  group_flags[1] |= requested_toffset_r16_present;
  group_flags[2] |= reserved_res_cfg_nrdc_r17.is_present();
  group_flags[3] |= aggr_bw_sn_r17.is_present();
  group_flags[4] |= requested_max_ltm_candidate_id_scg_r18_present;
  group_flags[5] |= requested_l1_meas_cfg_nrdc_r18.is_present();
  group_flags[6] |= requested_ltm_res_cfg_nrdc_r18.is_present();
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), requested_bc_mrdc_present, requested_p_max_fr1_present}));

  if (requested_bc_mrdc_present) {
    HANDLE_CODE(requested_bc_mrdc.pack(bref));
  }
  if (requested_p_max_fr1_present) {
    HANDLE_CODE(pack_integer(bref, requested_p_max_fr1, (int8_t)-30, (int8_t)33));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {requested_pdcch_blind_detection_scg_present, requested_p_max_eutra_present}));
      if (requested_pdcch_blind_detection_scg_present) {
        HANDLE_CODE(pack_integer(bref, requested_pdcch_blind_detection_scg, (uint8_t)1u, (uint8_t)15u));
      }
      if (requested_p_max_eutra_present) {
        HANDLE_CODE(pack_integer(bref, requested_p_max_eutra, (int8_t)-30, (int8_t)33));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {requested_p_max_fr2_r16_present,
                             requested_max_inter_freq_meas_id_scg_r16_present,
                             requested_max_intra_freq_meas_id_scg_r16_present,
                             requested_toffset_r16_present}));
      if (requested_p_max_fr2_r16_present) {
        HANDLE_CODE(pack_integer(bref, requested_p_max_fr2_r16, (int8_t)-30, (int8_t)33));
      }
      if (requested_max_inter_freq_meas_id_scg_r16_present) {
        HANDLE_CODE(pack_integer(bref, requested_max_inter_freq_meas_id_scg_r16, (uint8_t)1u, (uint8_t)62u));
      }
      if (requested_max_intra_freq_meas_id_scg_r16_present) {
        HANDLE_CODE(pack_integer(bref, requested_max_intra_freq_meas_id_scg_r16, (uint8_t)1u, (uint8_t)62u));
      }
      if (requested_toffset_r16_present) {
        HANDLE_CODE(requested_toffset_r16.pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(reserved_res_cfg_nrdc_r17.is_present(), 1));
      if (reserved_res_cfg_nrdc_r17.is_present()) {
        HANDLE_CODE(reserved_res_cfg_nrdc_r17->pack(bref));
      }
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(aggr_bw_sn_r17.is_present(), 1));
      if (aggr_bw_sn_r17.is_present()) {
        HANDLE_CODE(aggr_bw_sn_r17->pack(bref));
      }
    }
    if (group_flags[4]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(requested_max_ltm_candidate_id_scg_r18_present, 1));
      if (requested_max_ltm_candidate_id_scg_r18_present) {
        HANDLE_CODE(pack_integer(bref, requested_max_ltm_candidate_id_scg_r18, (uint8_t)0u, (uint8_t)8u));
      }
    }
    if (group_flags[5]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(requested_l1_meas_cfg_nrdc_r18.is_present(), 1));
      if (requested_l1_meas_cfg_nrdc_r18.is_present()) {
        HANDLE_CODE(requested_l1_meas_cfg_nrdc_r18->pack(bref));
      }
    }
    if (group_flags[6]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(requested_ltm_res_cfg_nrdc_r18.is_present(), 1));
      if (requested_ltm_res_cfg_nrdc_r18.is_present()) {
        HANDLE_CODE(requested_ltm_res_cfg_nrdc_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_restrict_mod_req_scg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &requested_bc_mrdc_present, &requested_p_max_fr1_present}));

  if (requested_bc_mrdc_present) {
    HANDLE_CODE(requested_bc_mrdc.unpack(bref));
  }
  if (requested_p_max_fr1_present) {
    HANDLE_CODE(unpack_integer(requested_p_max_fr1, bref, (int8_t)-30, (int8_t)33));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref, {&requested_pdcch_blind_detection_scg_present, &requested_p_max_eutra_present}));
      if (requested_pdcch_blind_detection_scg_present) {
        HANDLE_CODE(unpack_integer(requested_pdcch_blind_detection_scg, bref, (uint8_t)1u, (uint8_t)15u));
      }
      if (requested_p_max_eutra_present) {
        HANDLE_CODE(unpack_integer(requested_p_max_eutra, bref, (int8_t)-30, (int8_t)33));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref,
                              {&requested_p_max_fr2_r16_present,
                               &requested_max_inter_freq_meas_id_scg_r16_present,
                               &requested_max_intra_freq_meas_id_scg_r16_present,
                               &requested_toffset_r16_present}));
      if (requested_p_max_fr2_r16_present) {
        HANDLE_CODE(unpack_integer(requested_p_max_fr2_r16, bref, (int8_t)-30, (int8_t)33));
      }
      if (requested_max_inter_freq_meas_id_scg_r16_present) {
        HANDLE_CODE(unpack_integer(requested_max_inter_freq_meas_id_scg_r16, bref, (uint8_t)1u, (uint8_t)62u));
      }
      if (requested_max_intra_freq_meas_id_scg_r16_present) {
        HANDLE_CODE(unpack_integer(requested_max_intra_freq_meas_id_scg_r16, bref, (uint8_t)1u, (uint8_t)62u));
      }
      if (requested_toffset_r16_present) {
        HANDLE_CODE(requested_toffset_r16.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(reserved_res_cfg_nrdc_r17, bref);
      if (reserved_res_cfg_nrdc_r17.is_present()) {
        HANDLE_CODE(reserved_res_cfg_nrdc_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(aggr_bw_sn_r17, bref);
      if (aggr_bw_sn_r17.is_present()) {
        HANDLE_CODE(aggr_bw_sn_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(requested_max_ltm_candidate_id_scg_r18_present, 1));
      if (requested_max_ltm_candidate_id_scg_r18_present) {
        HANDLE_CODE(unpack_integer(requested_max_ltm_candidate_id_scg_r18, bref, (uint8_t)0u, (uint8_t)8u));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(requested_l1_meas_cfg_nrdc_r18, bref);
      if (requested_l1_meas_cfg_nrdc_r18.is_present()) {
        HANDLE_CODE(requested_l1_meas_cfg_nrdc_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(requested_ltm_res_cfg_nrdc_r18, bref);
      if (requested_ltm_res_cfg_nrdc_r18.is_present()) {
        HANDLE_CODE(requested_ltm_res_cfg_nrdc_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cfg_restrict_mod_req_scg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (requested_bc_mrdc_present) {
    j.write_fieldname("requestedBC-MRDC");
    requested_bc_mrdc.to_json(j);
  }
  if (requested_p_max_fr1_present) {
    j.write_int("requestedP-MaxFR1", requested_p_max_fr1);
  }
  if (requested_pdcch_blind_detection_scg_present) {
    j.write_int("requestedPDCCH-BlindDetectionSCG", requested_pdcch_blind_detection_scg);
  }
  if (requested_p_max_eutra_present) {
    j.write_int("requestedP-MaxEUTRA", requested_p_max_eutra);
  }
  if (requested_p_max_fr2_r16_present) {
    j.write_int("requestedP-MaxFR2-r16", requested_p_max_fr2_r16);
  }
  if (requested_max_inter_freq_meas_id_scg_r16_present) {
    j.write_int("requestedMaxInterFreqMeasIdSCG-r16", requested_max_inter_freq_meas_id_scg_r16);
  }
  if (requested_max_intra_freq_meas_id_scg_r16_present) {
    j.write_int("requestedMaxIntraFreqMeasIdSCG-r16", requested_max_intra_freq_meas_id_scg_r16);
  }
  if (requested_toffset_r16_present) {
    j.write_str("requestedToffset-r16", requested_toffset_r16.to_string());
  }
  if (reserved_res_cfg_nrdc_r17.is_present()) {
    j.write_fieldname("reservedResourceConfigNRDC-r17");
    reserved_res_cfg_nrdc_r17->to_json(j);
  }
  if (aggr_bw_sn_r17.is_present()) {
    j.write_fieldname("aggregatedBandwidthSN-r17");
    aggr_bw_sn_r17->to_json(j);
  }
  if (requested_max_ltm_candidate_id_scg_r18_present) {
    j.write_int("requestedMaxLTM-CandidateIdSCG-r18", requested_max_ltm_candidate_id_scg_r18);
  }
  if (requested_l1_meas_cfg_nrdc_r18.is_present()) {
    j.write_fieldname("requestedL1-MeasConfigNRDC-r18");
    requested_l1_meas_cfg_nrdc_r18->to_json(j);
  }
  if (requested_ltm_res_cfg_nrdc_r18.is_present()) {
    j.write_fieldname("requestedLTM-ResourceConfigNRDC-r18");
    requested_ltm_res_cfg_nrdc_r18->to_json(j);
  }
  j.end_obj();
}

// MeasConfigSN ::= SEQUENCE
OCUDUASN_CODE meas_cfg_sn_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, measured_frequencies_sn.size() > 0}));

  if (measured_frequencies_sn.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, measured_frequencies_sn, 1, 32));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_cfg_sn_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool measured_frequencies_sn_present;
  HANDLE_CODE(unpack_bits(bref, {&ext, &measured_frequencies_sn_present}));

  if (measured_frequencies_sn_present) {
    HANDLE_CODE(unpack_dyn_seq_of(measured_frequencies_sn, bref, 1, 32));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void meas_cfg_sn_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (measured_frequencies_sn.size() > 0) {
    j.start_array("measuredFrequenciesSN");
    for (const auto& e1 : measured_frequencies_sn) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// PH-UplinkCarrierSCG ::= SEQUENCE
OCUDUASN_CODE ph_ul_carrier_scg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(ph_type1or3.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ph_ul_carrier_scg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(ph_type1or3.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ph_ul_carrier_scg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("ph-Type1or3", ph_type1or3.to_string());
  j.end_obj();
}

const char* ph_ul_carrier_scg_s::ph_type1or3_opts::to_string() const
{
  static const char* names[] = {"type1", "type3"};
  return convert_enum_idx(names, 2, value, "ph_ul_carrier_scg_s::ph_type1or3_e_");
}
uint8_t ph_ul_carrier_scg_s::ph_type1or3_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 3};
  return map_enum_number(numbers, 2, value, "ph_ul_carrier_scg_s::ph_type1or3_e_");
}

// PH-InfoSCG ::= SEQUENCE
OCUDUASN_CODE ph_info_scg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(2);
  group_flags[0] |= two_srs_pusch_repeat_r17_present;
  group_flags[1] |= two_srs_multipanel_scheme_r18_present;
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), ph_supplementary_ul_present}));

  HANDLE_CODE(pack_integer(bref, serv_cell_idx, (uint8_t)0u, (uint8_t)31u));
  HANDLE_CODE(ph_ul.pack(bref));
  if (ph_supplementary_ul_present) {
    HANDLE_CODE(ph_supplementary_ul.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(two_srs_pusch_repeat_r17_present, 1));
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(two_srs_multipanel_scheme_r18_present, 1));
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ph_info_scg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &ph_supplementary_ul_present}));

  HANDLE_CODE(unpack_integer(serv_cell_idx, bref, (uint8_t)0u, (uint8_t)31u));
  HANDLE_CODE(ph_ul.unpack(bref));
  if (ph_supplementary_ul_present) {
    HANDLE_CODE(ph_supplementary_ul.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(two_srs_pusch_repeat_r17_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(two_srs_multipanel_scheme_r18_present, 1));
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ph_info_scg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("servCellIndex", serv_cell_idx);
  j.write_fieldname("ph-Uplink");
  ph_ul.to_json(j);
  if (ph_supplementary_ul_present) {
    j.write_fieldname("ph-SupplementaryUplink");
    ph_supplementary_ul.to_json(j);
  }
  if (two_srs_pusch_repeat_r17_present) {
    j.write_str("twoSRS-PUSCH-Repetition-r17", "enabled");
  }
  if (two_srs_multipanel_scheme_r18_present) {
    j.write_str("twoSRS-MultipanelScheme-r18", "enabled");
  }
  j.end_obj();
}

// CandidateCell-r17 ::= SEQUENCE
OCUDUASN_CODE candidate_cell_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(cond_execution_cond_scg_r17.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, pci_r17, (uint16_t)0u, (uint16_t)1007u));
  if (cond_execution_cond_scg_r17.size() > 0) {
    HANDLE_CODE(cond_execution_cond_scg_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE candidate_cell_r17_s::unpack(cbit_ref& bref)
{
  bool cond_execution_cond_scg_r17_present;
  HANDLE_CODE(bref.unpack(cond_execution_cond_scg_r17_present, 1));

  HANDLE_CODE(unpack_integer(pci_r17, bref, (uint16_t)0u, (uint16_t)1007u));
  if (cond_execution_cond_scg_r17_present) {
    HANDLE_CODE(cond_execution_cond_scg_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void candidate_cell_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("physCellId-r17", pci_r17);
  if (cond_execution_cond_scg_r17.size() > 0) {
    j.write_str("condExecutionCondSCG-r17", cond_execution_cond_scg_r17.to_string());
  }
  j.end_obj();
}

// CandidateCellInfo-r17 ::= SEQUENCE
OCUDUASN_CODE candidate_cell_info_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, ssb_freq_r17, (uint32_t)0u, (uint32_t)3279165u));
  HANDLE_CODE(pack_dyn_seq_of(bref, candidate_list_r17, 1, 8));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE candidate_cell_info_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(ssb_freq_r17, bref, (uint32_t)0u, (uint32_t)3279165u));
  HANDLE_CODE(unpack_dyn_seq_of(candidate_list_r17, bref, 1, 8));

  return OCUDUASN_SUCCESS;
}
void candidate_cell_info_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ssbFrequency-r17", ssb_freq_r17);
  j.start_array("candidateList-r17");
  for (const auto& e1 : candidate_list_r17) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// CG-Config-v1800-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1800_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {candidate_serving_freq_range_list_nr_r18.size() > 0,
                         candidate_serving_freq_list_nr_r18.size() > 0,
                         idc_tdm_assist_cfg_r18_present,
                         candidate_cell_info_list_subsequent_cp_c_r18.size() > 0,
                         scpac_ref_cfg_scg_r18.size() > 0,
                         subsequent_cp_ac_info_r18.size() > 0,
                         success_pscell_cfg_r18_present,
                         non_crit_ext_present}));

  if (candidate_serving_freq_range_list_nr_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, candidate_serving_freq_range_list_nr_r18, 1, 128));
  }
  if (candidate_serving_freq_list_nr_r18.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, candidate_serving_freq_list_nr_r18, 1, 128, integer_packer<uint32_t>(0, 3279165)));
  }
  if (candidate_cell_info_list_subsequent_cp_c_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, candidate_cell_info_list_subsequent_cp_c_r18, 1, 8));
  }
  if (scpac_ref_cfg_scg_r18.size() > 0) {
    HANDLE_CODE(scpac_ref_cfg_scg_r18.pack(bref));
  }
  if (subsequent_cp_ac_info_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, subsequent_cp_ac_info_r18, 1, 8));
  }
  if (success_pscell_cfg_r18_present) {
    HANDLE_CODE(success_pscell_cfg_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1800_ies_s::unpack(cbit_ref& bref)
{
  bool candidate_serving_freq_range_list_nr_r18_present;
  bool candidate_serving_freq_list_nr_r18_present;
  bool candidate_cell_info_list_subsequent_cp_c_r18_present;
  bool scpac_ref_cfg_scg_r18_present;
  bool subsequent_cp_ac_info_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&candidate_serving_freq_range_list_nr_r18_present,
                           &candidate_serving_freq_list_nr_r18_present,
                           &idc_tdm_assist_cfg_r18_present,
                           &candidate_cell_info_list_subsequent_cp_c_r18_present,
                           &scpac_ref_cfg_scg_r18_present,
                           &subsequent_cp_ac_info_r18_present,
                           &success_pscell_cfg_r18_present,
                           &non_crit_ext_present}));

  if (candidate_serving_freq_range_list_nr_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(candidate_serving_freq_range_list_nr_r18, bref, 1, 128));
  }
  if (candidate_serving_freq_list_nr_r18_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(candidate_serving_freq_list_nr_r18, bref, 1, 128, integer_packer<uint32_t>(0, 3279165)));
  }
  if (candidate_cell_info_list_subsequent_cp_c_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(candidate_cell_info_list_subsequent_cp_c_r18, bref, 1, 8));
  }
  if (scpac_ref_cfg_scg_r18_present) {
    HANDLE_CODE(scpac_ref_cfg_scg_r18.unpack(bref));
  }
  if (subsequent_cp_ac_info_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(subsequent_cp_ac_info_r18, bref, 1, 8));
  }
  if (success_pscell_cfg_r18_present) {
    HANDLE_CODE(success_pscell_cfg_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1800_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (candidate_serving_freq_range_list_nr_r18.size() > 0) {
    j.start_array("candidateServingFreqRangeListNR-r18");
    for (const auto& e1 : candidate_serving_freq_range_list_nr_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (candidate_serving_freq_list_nr_r18.size() > 0) {
    j.start_array("candidateServingFreqListNR-r18");
    for (const auto& e1 : candidate_serving_freq_list_nr_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (idc_tdm_assist_cfg_r18_present) {
    j.write_str("idc-TDM-AssistanceConfig-r18", "enabled");
  }
  if (candidate_cell_info_list_subsequent_cp_c_r18.size() > 0) {
    j.start_array("candidateCellInfoListSubsequentCPC-r18");
    for (const auto& e1 : candidate_cell_info_list_subsequent_cp_c_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (scpac_ref_cfg_scg_r18.size() > 0) {
    j.write_str("scpac-ReferenceConfigurationSCG-r18", scpac_ref_cfg_scg_r18.to_string());
  }
  if (subsequent_cp_ac_info_r18.size() > 0) {
    j.start_array("subsequentCPAC-Information-r18");
    for (const auto& e1 : subsequent_cp_ac_info_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (success_pscell_cfg_r18_present) {
    j.write_fieldname("successPSCell-Config-r18");
    success_pscell_cfg_r18.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// CG-Config-v1730-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1730_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {fr1_carriers_scg_r17_present, fr2_carriers_scg_r17_present, non_crit_ext_present}));

  if (fr1_carriers_scg_r17_present) {
    HANDLE_CODE(pack_integer(bref, fr1_carriers_scg_r17, (uint8_t)1u, (uint8_t)32u));
  }
  if (fr2_carriers_scg_r17_present) {
    HANDLE_CODE(pack_integer(bref, fr2_carriers_scg_r17, (uint8_t)1u, (uint8_t)32u));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1730_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&fr1_carriers_scg_r17_present, &fr2_carriers_scg_r17_present, &non_crit_ext_present}));

  if (fr1_carriers_scg_r17_present) {
    HANDLE_CODE(unpack_integer(fr1_carriers_scg_r17, bref, (uint8_t)1u, (uint8_t)32u));
  }
  if (fr2_carriers_scg_r17_present) {
    HANDLE_CODE(unpack_integer(fr2_carriers_scg_r17, bref, (uint8_t)1u, (uint8_t)32u));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1730_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (fr1_carriers_scg_r17_present) {
    j.write_int("fr1-Carriers-SCG-r17", fr1_carriers_scg_r17);
  }
  if (fr2_carriers_scg_r17_present) {
    j.write_int("fr2-Carriers-SCG-r17", fr2_carriers_scg_r17);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// CG-Config-v1700-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1700_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref, {candidate_cell_info_list_cp_c_r17.size() > 0, two_phr_mode_scg_r17_present, non_crit_ext_present}));

  if (candidate_cell_info_list_cp_c_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, candidate_cell_info_list_cp_c_r17, 1, 8));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1700_ies_s::unpack(cbit_ref& bref)
{
  bool candidate_cell_info_list_cp_c_r17_present;
  HANDLE_CODE(unpack_bits(
      bref, {&candidate_cell_info_list_cp_c_r17_present, &two_phr_mode_scg_r17_present, &non_crit_ext_present}));

  if (candidate_cell_info_list_cp_c_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(candidate_cell_info_list_cp_c_r17, bref, 1, 8));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1700_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (candidate_cell_info_list_cp_c_r17.size() > 0) {
    j.start_array("candidateCellInfoListCPC-r17");
    for (const auto& e1 : candidate_cell_info_list_cp_c_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (two_phr_mode_scg_r17_present) {
    j.write_str("twoPHRModeSCG-r17", "enabled");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// CG-Config-v1640-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1640_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref,
      {serv_cell_info_list_scg_nr_r16.size() > 0, serv_cell_info_list_scg_eutra_r16.size() > 0, non_crit_ext_present}));

  if (serv_cell_info_list_scg_nr_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, serv_cell_info_list_scg_nr_r16, 1, 32));
  }
  if (serv_cell_info_list_scg_eutra_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, serv_cell_info_list_scg_eutra_r16, 1, 32));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1640_ies_s::unpack(cbit_ref& bref)
{
  bool serv_cell_info_list_scg_nr_r16_present;
  bool serv_cell_info_list_scg_eutra_r16_present;
  HANDLE_CODE(unpack_bits(
      bref,
      {&serv_cell_info_list_scg_nr_r16_present, &serv_cell_info_list_scg_eutra_r16_present, &non_crit_ext_present}));

  if (serv_cell_info_list_scg_nr_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(serv_cell_info_list_scg_nr_r16, bref, 1, 32));
  }
  if (serv_cell_info_list_scg_eutra_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(serv_cell_info_list_scg_eutra_r16, bref, 1, 32));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1640_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (serv_cell_info_list_scg_nr_r16.size() > 0) {
    j.start_array("servCellInfoListSCG-NR-r16");
    for (const auto& e1 : serv_cell_info_list_scg_nr_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (serv_cell_info_list_scg_eutra_r16.size() > 0) {
    j.start_array("servCellInfoListSCG-EUTRA-r16");
    for (const auto& e1 : serv_cell_info_list_scg_eutra_r16) {
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

// CG-Config-v1630-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1630_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {sel_toffset_r16_present, non_crit_ext_present}));

  if (sel_toffset_r16_present) {
    HANDLE_CODE(sel_toffset_r16.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1630_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&sel_toffset_r16_present, &non_crit_ext_present}));

  if (sel_toffset_r16_present) {
    HANDLE_CODE(sel_toffset_r16.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1630_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sel_toffset_r16_present) {
    j.write_str("selectedToffset-r16", sel_toffset_r16.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// CG-Config-v1620-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1620_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {ue_assist_info_scg_r16.size() > 0, non_crit_ext_present}));

  if (ue_assist_info_scg_r16.size() > 0) {
    HANDLE_CODE(ue_assist_info_scg_r16.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1620_ies_s::unpack(cbit_ref& bref)
{
  bool ue_assist_info_scg_r16_present;
  HANDLE_CODE(unpack_bits(bref, {&ue_assist_info_scg_r16_present, &non_crit_ext_present}));

  if (ue_assist_info_scg_r16_present) {
    HANDLE_CODE(ue_assist_info_scg_r16.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1620_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ue_assist_info_scg_r16.size() > 0) {
    j.write_str("ueAssistanceInformationSCG-r16", ue_assist_info_scg_r16.to_string());
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// CG-Config-v1610-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1610_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {drx_info_scg2_present, non_crit_ext_present}));

  if (drx_info_scg2_present) {
    HANDLE_CODE(drx_info_scg2.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1610_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&drx_info_scg2_present, &non_crit_ext_present}));

  if (drx_info_scg2_present) {
    HANDLE_CODE(drx_info_scg2.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1610_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (drx_info_scg2_present) {
    j.write_fieldname("drx-InfoSCG2");
    drx_info_scg2.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// CG-Config-v1590-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1590_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref, {scell_frequencies_sn_nr.size() > 0, scell_frequencies_sn_eutra.size() > 0, non_crit_ext_present}));

  if (scell_frequencies_sn_nr.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, scell_frequencies_sn_nr, 1, 31, integer_packer<uint32_t>(0, 3279165)));
  }
  if (scell_frequencies_sn_eutra.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, scell_frequencies_sn_eutra, 1, 31, integer_packer<uint32_t>(0, 262143)));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1590_ies_s::unpack(cbit_ref& bref)
{
  bool scell_frequencies_sn_nr_present;
  bool scell_frequencies_sn_eutra_present;
  HANDLE_CODE(unpack_bits(
      bref, {&scell_frequencies_sn_nr_present, &scell_frequencies_sn_eutra_present, &non_crit_ext_present}));

  if (scell_frequencies_sn_nr_present) {
    HANDLE_CODE(unpack_dyn_seq_of(scell_frequencies_sn_nr, bref, 1, 31, integer_packer<uint32_t>(0, 3279165)));
  }
  if (scell_frequencies_sn_eutra_present) {
    HANDLE_CODE(unpack_dyn_seq_of(scell_frequencies_sn_eutra, bref, 1, 31, integer_packer<uint32_t>(0, 262143)));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1590_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (scell_frequencies_sn_nr.size() > 0) {
    j.start_array("scellFrequenciesSN-NR");
    for (const auto& e1 : scell_frequencies_sn_nr) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (scell_frequencies_sn_eutra.size() > 0) {
    j.start_array("scellFrequenciesSN-EUTRA");
    for (const auto& e1 : scell_frequencies_sn_eutra) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// CG-Config-v1560-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1560_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {pscell_freq_eutra_present,
                         scg_cell_group_cfg_eutra.size() > 0,
                         candidate_cell_info_list_sn_eutra.size() > 0,
                         candidate_serving_freq_list_eutra.size() > 0,
                         need_for_gaps_present,
                         drx_cfg_scg_present,
                         report_cgi_request_eutra_present,
                         non_crit_ext_present}));

  if (pscell_freq_eutra_present) {
    HANDLE_CODE(pack_integer(bref, pscell_freq_eutra, (uint32_t)0u, (uint32_t)262143u));
  }
  if (scg_cell_group_cfg_eutra.size() > 0) {
    HANDLE_CODE(scg_cell_group_cfg_eutra.pack(bref));
  }
  if (candidate_cell_info_list_sn_eutra.size() > 0) {
    HANDLE_CODE(candidate_cell_info_list_sn_eutra.pack(bref));
  }
  if (candidate_serving_freq_list_eutra.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, candidate_serving_freq_list_eutra, 1, 32, integer_packer<uint32_t>(0, 262143)));
  }
  if (drx_cfg_scg_present) {
    HANDLE_CODE(drx_cfg_scg.pack(bref));
  }
  if (report_cgi_request_eutra_present) {
    HANDLE_CODE(bref.pack(report_cgi_request_eutra.requested_cell_info_eutra_present, 1));
    if (report_cgi_request_eutra.requested_cell_info_eutra_present) {
      HANDLE_CODE(pack_integer(
          bref, report_cgi_request_eutra.requested_cell_info_eutra.eutra_freq, (uint32_t)0u, (uint32_t)262143u));
      HANDLE_CODE(pack_integer(bref,
                               report_cgi_request_eutra.requested_cell_info_eutra.cell_for_which_to_report_cgi_eutra,
                               (uint16_t)0u,
                               (uint16_t)503u));
    }
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1560_ies_s::unpack(cbit_ref& bref)
{
  bool scg_cell_group_cfg_eutra_present;
  bool candidate_cell_info_list_sn_eutra_present;
  bool candidate_serving_freq_list_eutra_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&pscell_freq_eutra_present,
                           &scg_cell_group_cfg_eutra_present,
                           &candidate_cell_info_list_sn_eutra_present,
                           &candidate_serving_freq_list_eutra_present,
                           &need_for_gaps_present,
                           &drx_cfg_scg_present,
                           &report_cgi_request_eutra_present,
                           &non_crit_ext_present}));

  if (pscell_freq_eutra_present) {
    HANDLE_CODE(unpack_integer(pscell_freq_eutra, bref, (uint32_t)0u, (uint32_t)262143u));
  }
  if (scg_cell_group_cfg_eutra_present) {
    HANDLE_CODE(scg_cell_group_cfg_eutra.unpack(bref));
  }
  if (candidate_cell_info_list_sn_eutra_present) {
    HANDLE_CODE(candidate_cell_info_list_sn_eutra.unpack(bref));
  }
  if (candidate_serving_freq_list_eutra_present) {
    HANDLE_CODE(unpack_dyn_seq_of(candidate_serving_freq_list_eutra, bref, 1, 32, integer_packer<uint32_t>(0, 262143)));
  }
  if (drx_cfg_scg_present) {
    HANDLE_CODE(drx_cfg_scg.unpack(bref));
  }
  if (report_cgi_request_eutra_present) {
    HANDLE_CODE(bref.unpack(report_cgi_request_eutra.requested_cell_info_eutra_present, 1));
    if (report_cgi_request_eutra.requested_cell_info_eutra_present) {
      HANDLE_CODE(unpack_integer(
          report_cgi_request_eutra.requested_cell_info_eutra.eutra_freq, bref, (uint32_t)0u, (uint32_t)262143u));
      HANDLE_CODE(unpack_integer(report_cgi_request_eutra.requested_cell_info_eutra.cell_for_which_to_report_cgi_eutra,
                                 bref,
                                 (uint16_t)0u,
                                 (uint16_t)503u));
    }
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1560_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pscell_freq_eutra_present) {
    j.write_int("pSCellFrequencyEUTRA", pscell_freq_eutra);
  }
  if (scg_cell_group_cfg_eutra.size() > 0) {
    j.write_str("scg-CellGroupConfigEUTRA", scg_cell_group_cfg_eutra.to_string());
  }
  if (candidate_cell_info_list_sn_eutra.size() > 0) {
    j.write_str("candidateCellInfoListSN-EUTRA", candidate_cell_info_list_sn_eutra.to_string());
  }
  if (candidate_serving_freq_list_eutra.size() > 0) {
    j.start_array("candidateServingFreqListEUTRA");
    for (const auto& e1 : candidate_serving_freq_list_eutra) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (need_for_gaps_present) {
    j.write_str("needForGaps", "true");
  }
  if (drx_cfg_scg_present) {
    j.write_fieldname("drx-ConfigSCG");
    drx_cfg_scg.to_json(j);
  }
  if (report_cgi_request_eutra_present) {
    j.write_fieldname("reportCGI-RequestEUTRA");
    j.start_obj();
    if (report_cgi_request_eutra.requested_cell_info_eutra_present) {
      j.write_fieldname("requestedCellInfoEUTRA");
      j.start_obj();
      j.write_int("eutraFrequency", report_cgi_request_eutra.requested_cell_info_eutra.eutra_freq);
      j.write_int("cellForWhichToReportCGI-EUTRA",
                  report_cgi_request_eutra.requested_cell_info_eutra.cell_for_which_to_report_cgi_eutra);
      j.end_obj();
    }
    j.end_obj();
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// CG-Config-v1540-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_v1540_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref, {pscell_freq_present, report_cgi_request_nr_present, ph_info_scg.size() > 0, non_crit_ext_present}));

  if (pscell_freq_present) {
    HANDLE_CODE(pack_integer(bref, pscell_freq, (uint32_t)0u, (uint32_t)3279165u));
  }
  if (report_cgi_request_nr_present) {
    HANDLE_CODE(bref.pack(report_cgi_request_nr.requested_cell_info_present, 1));
    if (report_cgi_request_nr.requested_cell_info_present) {
      HANDLE_CODE(
          pack_integer(bref, report_cgi_request_nr.requested_cell_info.ssb_freq, (uint32_t)0u, (uint32_t)3279165u));
      HANDLE_CODE(pack_integer(
          bref, report_cgi_request_nr.requested_cell_info.cell_for_which_to_report_cgi, (uint16_t)0u, (uint16_t)1007u));
    }
  }
  if (ph_info_scg.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ph_info_scg, 1, 32));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_v1540_ies_s::unpack(cbit_ref& bref)
{
  bool ph_info_scg_present;
  HANDLE_CODE(unpack_bits(
      bref, {&pscell_freq_present, &report_cgi_request_nr_present, &ph_info_scg_present, &non_crit_ext_present}));

  if (pscell_freq_present) {
    HANDLE_CODE(unpack_integer(pscell_freq, bref, (uint32_t)0u, (uint32_t)3279165u));
  }
  if (report_cgi_request_nr_present) {
    HANDLE_CODE(bref.unpack(report_cgi_request_nr.requested_cell_info_present, 1));
    if (report_cgi_request_nr.requested_cell_info_present) {
      HANDLE_CODE(
          unpack_integer(report_cgi_request_nr.requested_cell_info.ssb_freq, bref, (uint32_t)0u, (uint32_t)3279165u));
      HANDLE_CODE(unpack_integer(
          report_cgi_request_nr.requested_cell_info.cell_for_which_to_report_cgi, bref, (uint16_t)0u, (uint16_t)1007u));
    }
  }
  if (ph_info_scg_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ph_info_scg, bref, 1, 32));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_v1540_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pscell_freq_present) {
    j.write_int("pSCellFrequency", pscell_freq);
  }
  if (report_cgi_request_nr_present) {
    j.write_fieldname("reportCGI-RequestNR");
    j.start_obj();
    if (report_cgi_request_nr.requested_cell_info_present) {
      j.write_fieldname("requestedCellInfo");
      j.start_obj();
      j.write_int("ssbFrequency", report_cgi_request_nr.requested_cell_info.ssb_freq);
      j.write_int("cellForWhichToReportCGI", report_cgi_request_nr.requested_cell_info.cell_for_which_to_report_cgi);
      j.end_obj();
    }
    j.end_obj();
  }
  if (ph_info_scg.size() > 0) {
    j.start_array("ph-InfoSCG");
    for (const auto& e1 : ph_info_scg) {
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

// CG-Config-IEs ::= SEQUENCE
OCUDUASN_CODE cg_cfg_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {scg_cell_group_cfg.size() > 0,
                         scg_rb_cfg.size() > 0,
                         cfg_restrict_mod_req_present,
                         drx_info_scg_present,
                         candidate_cell_info_list_sn.size() > 0,
                         meas_cfg_sn_present,
                         sel_band_combination_present,
                         fr_info_list_scg.size() > 0,
                         candidate_serving_freq_list_nr.size() > 0,
                         non_crit_ext_present}));

  if (scg_cell_group_cfg.size() > 0) {
    HANDLE_CODE(scg_cell_group_cfg.pack(bref));
  }
  if (scg_rb_cfg.size() > 0) {
    HANDLE_CODE(scg_rb_cfg.pack(bref));
  }
  if (cfg_restrict_mod_req_present) {
    HANDLE_CODE(cfg_restrict_mod_req.pack(bref));
  }
  if (drx_info_scg_present) {
    HANDLE_CODE(drx_info_scg.pack(bref));
  }
  if (candidate_cell_info_list_sn.size() > 0) {
    HANDLE_CODE(candidate_cell_info_list_sn.pack(bref));
  }
  if (meas_cfg_sn_present) {
    HANDLE_CODE(meas_cfg_sn.pack(bref));
  }
  if (sel_band_combination_present) {
    HANDLE_CODE(sel_band_combination.pack(bref));
  }
  if (fr_info_list_scg.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, fr_info_list_scg, 1, 31));
  }
  if (candidate_serving_freq_list_nr.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, candidate_serving_freq_list_nr, 1, 32, integer_packer<uint32_t>(0, 3279165)));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_ies_s::unpack(cbit_ref& bref)
{
  bool scg_cell_group_cfg_present;
  bool scg_rb_cfg_present;
  bool candidate_cell_info_list_sn_present;
  bool fr_info_list_scg_present;
  bool candidate_serving_freq_list_nr_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&scg_cell_group_cfg_present,
                           &scg_rb_cfg_present,
                           &cfg_restrict_mod_req_present,
                           &drx_info_scg_present,
                           &candidate_cell_info_list_sn_present,
                           &meas_cfg_sn_present,
                           &sel_band_combination_present,
                           &fr_info_list_scg_present,
                           &candidate_serving_freq_list_nr_present,
                           &non_crit_ext_present}));

  if (scg_cell_group_cfg_present) {
    HANDLE_CODE(scg_cell_group_cfg.unpack(bref));
  }
  if (scg_rb_cfg_present) {
    HANDLE_CODE(scg_rb_cfg.unpack(bref));
  }
  if (cfg_restrict_mod_req_present) {
    HANDLE_CODE(cfg_restrict_mod_req.unpack(bref));
  }
  if (drx_info_scg_present) {
    HANDLE_CODE(drx_info_scg.unpack(bref));
  }
  if (candidate_cell_info_list_sn_present) {
    HANDLE_CODE(candidate_cell_info_list_sn.unpack(bref));
  }
  if (meas_cfg_sn_present) {
    HANDLE_CODE(meas_cfg_sn.unpack(bref));
  }
  if (sel_band_combination_present) {
    HANDLE_CODE(sel_band_combination.unpack(bref));
  }
  if (fr_info_list_scg_present) {
    HANDLE_CODE(unpack_dyn_seq_of(fr_info_list_scg, bref, 1, 31));
  }
  if (candidate_serving_freq_list_nr_present) {
    HANDLE_CODE(unpack_dyn_seq_of(candidate_serving_freq_list_nr, bref, 1, 32, integer_packer<uint32_t>(0, 3279165)));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_cfg_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (scg_cell_group_cfg.size() > 0) {
    j.write_str("scg-CellGroupConfig", scg_cell_group_cfg.to_string());
  }
  if (scg_rb_cfg.size() > 0) {
    j.write_str("scg-RB-Config", scg_rb_cfg.to_string());
  }
  if (cfg_restrict_mod_req_present) {
    j.write_fieldname("configRestrictModReq");
    cfg_restrict_mod_req.to_json(j);
  }
  if (drx_info_scg_present) {
    j.write_fieldname("drx-InfoSCG");
    drx_info_scg.to_json(j);
  }
  if (candidate_cell_info_list_sn.size() > 0) {
    j.write_str("candidateCellInfoListSN", candidate_cell_info_list_sn.to_string());
  }
  if (meas_cfg_sn_present) {
    j.write_fieldname("measConfigSN");
    meas_cfg_sn.to_json(j);
  }
  if (sel_band_combination_present) {
    j.write_fieldname("selectedBandCombination");
    sel_band_combination.to_json(j);
  }
  if (fr_info_list_scg.size() > 0) {
    j.start_array("fr-InfoListSCG");
    for (const auto& e1 : fr_info_list_scg) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (candidate_serving_freq_list_nr.size() > 0) {
    j.start_array("candidateServingFreqListNR");
    for (const auto& e1 : candidate_serving_freq_list_nr) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// CG-Config ::= SEQUENCE
OCUDUASN_CODE cg_cfg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void cg_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void cg_cfg_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
cg_cfg_s::crit_exts_c_::c1_c_& cg_cfg_s::crit_exts_c_::set_c1()
{
  set(types::c1);
  return c;
}
void cg_cfg_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void cg_cfg_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "cg_cfg_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE cg_cfg_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "cg_cfg_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "cg_cfg_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void cg_cfg_s::crit_exts_c_::c1_c_::set(types::options e)
{
  type_ = e;
}
cg_cfg_ies_s& cg_cfg_s::crit_exts_c_::c1_c_::set_cg_cfg()
{
  set(types::cg_cfg);
  return c;
}
void cg_cfg_s::crit_exts_c_::c1_c_::set_spare3()
{
  set(types::spare3);
}
void cg_cfg_s::crit_exts_c_::c1_c_::set_spare2()
{
  set(types::spare2);
}
void cg_cfg_s::crit_exts_c_::c1_c_::set_spare1()
{
  set(types::spare1);
}
void cg_cfg_s::crit_exts_c_::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cg_cfg:
      j.write_fieldname("cg-Config");
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
      log_invalid_choice_id(type_, "cg_cfg_s::crit_exts_c_::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE cg_cfg_s::crit_exts_c_::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::cg_cfg:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "cg_cfg_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_cfg_s::crit_exts_c_::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::cg_cfg:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "cg_cfg_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cg_cfg_s::crit_exts_c_::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"cg-Config", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 4, value, "cg_cfg_s::crit_exts_c_::c1_c_::types");
}

const char* cg_cfg_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"c1", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "cg_cfg_s::crit_exts_c_::types");
}
uint8_t cg_cfg_s::crit_exts_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "cg_cfg_s::crit_exts_c_::types");
}

// CandPSCellExecutionConditionInfo-r18 ::= SEQUENCE
OCUDUASN_CODE cand_pscell_execution_condition_info_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, cand_pscell_exec_cond_report_cfg_nr_r18, 1, 2));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cand_pscell_execution_condition_info_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_dyn_seq_of(cand_pscell_exec_cond_report_cfg_nr_r18, bref, 1, 2));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cand_pscell_execution_condition_info_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("candPSCellExecCondReportConfigNR-r18");
  for (const auto& e1 : cand_pscell_exec_cond_report_cfg_nr_r18) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// HandoverCommand-v1800-IEs ::= SEQUENCE
OCUDUASN_CODE ho_cmd_v1800_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {cand_pscell_execution_condition_info_r18_present, non_crit_ext_present}));

  if (cand_pscell_execution_condition_info_r18_present) {
    HANDLE_CODE(cand_pscell_execution_condition_info_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cmd_v1800_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&cand_pscell_execution_condition_info_r18_present, &non_crit_ext_present}));

  if (cand_pscell_execution_condition_info_r18_present) {
    HANDLE_CODE(cand_pscell_execution_condition_info_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ho_cmd_v1800_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (cand_pscell_execution_condition_info_r18_present) {
    j.write_fieldname("candPSCellExecutionConditionInfo-r18");
    cand_pscell_execution_condition_info_r18.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// HandoverCommand-IEs ::= SEQUENCE
OCUDUASN_CODE ho_cmd_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  HANDLE_CODE(ho_cmd_msg.pack(bref));
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cmd_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  HANDLE_CODE(ho_cmd_msg.unpack(bref));
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ho_cmd_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("handoverCommandMessage", ho_cmd_msg.to_string());
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// HandoverCommand ::= SEQUENCE
OCUDUASN_CODE ho_cmd_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cmd_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ho_cmd_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ho_cmd_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ho_cmd_s::crit_exts_c_::c1_c_& ho_cmd_s::crit_exts_c_::set_c1()
{
  set(types::c1);
  return c;
}
void ho_cmd_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ho_cmd_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "ho_cmd_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ho_cmd_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ho_cmd_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cmd_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ho_cmd_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void ho_cmd_s::crit_exts_c_::c1_c_::set(types::options e)
{
  type_ = e;
}
ho_cmd_ies_s& ho_cmd_s::crit_exts_c_::c1_c_::set_ho_cmd()
{
  set(types::ho_cmd);
  return c;
}
void ho_cmd_s::crit_exts_c_::c1_c_::set_spare3()
{
  set(types::spare3);
}
void ho_cmd_s::crit_exts_c_::c1_c_::set_spare2()
{
  set(types::spare2);
}
void ho_cmd_s::crit_exts_c_::c1_c_::set_spare1()
{
  set(types::spare1);
}
void ho_cmd_s::crit_exts_c_::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ho_cmd:
      j.write_fieldname("handoverCommand");
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
      log_invalid_choice_id(type_, "ho_cmd_s::crit_exts_c_::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ho_cmd_s::crit_exts_c_::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ho_cmd:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "ho_cmd_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cmd_s::crit_exts_c_::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ho_cmd:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "ho_cmd_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ho_cmd_s::crit_exts_c_::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"handoverCommand", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 4, value, "ho_cmd_s::crit_exts_c_::c1_c_::types");
}

const char* ho_cmd_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"c1", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ho_cmd_s::crit_exts_c_::types");
}
uint8_t ho_cmd_s::crit_exts_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ho_cmd_s::crit_exts_c_::types");
}

// RRM-Config ::= SEQUENCE
OCUDUASN_CODE rrm_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= candidate_cell_info_list_sn_eutra.is_present();
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), ue_inactive_time_present, candidate_cell_info_list.size() > 0}));

  if (ue_inactive_time_present) {
    HANDLE_CODE(ue_inactive_time.pack(bref));
  }
  if (candidate_cell_info_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, candidate_cell_info_list, 1, 8));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(candidate_cell_info_list_sn_eutra.is_present(), 1));
      if (candidate_cell_info_list_sn_eutra.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *candidate_cell_info_list_sn_eutra, 1, 32));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrm_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool candidate_cell_info_list_present;
  HANDLE_CODE(unpack_bits(bref, {&ext, &ue_inactive_time_present, &candidate_cell_info_list_present}));

  if (ue_inactive_time_present) {
    HANDLE_CODE(ue_inactive_time.unpack(bref));
  }
  if (candidate_cell_info_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(candidate_cell_info_list, bref, 1, 8));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(candidate_cell_info_list_sn_eutra, bref);
      if (candidate_cell_info_list_sn_eutra.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*candidate_cell_info_list_sn_eutra, bref, 1, 32));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void rrm_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ue_inactive_time_present) {
    j.write_str("ue-InactiveTime", ue_inactive_time.to_string());
  }
  if (candidate_cell_info_list.size() > 0) {
    j.start_array("candidateCellInfoList");
    for (const auto& e1 : candidate_cell_info_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (candidate_cell_info_list_sn_eutra.is_present()) {
    j.start_array("candidateCellInfoListSN-EUTRA");
    for (const auto& e1 : *candidate_cell_info_list_sn_eutra) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

const char* rrm_cfg_s::ue_inactive_time_opts::to_string() const
{
  static const char* names[] = {
      "s1",    "s2",       "s3",    "s5",           "s7",      "s10",   "s15",     "s20",      "s25",     "s30",
      "s40",   "s50",      "min1",  "min1s20",      "min1s40", "min2",  "min2s30", "min3",     "min3s30", "min4",
      "min5",  "min6",     "min7",  "min8",         "min9",    "min10", "min12",   "min14",    "min17",   "min20",
      "min24", "min28",    "min33", "min38",        "min44",   "min50", "hr1",     "hr1min30", "hr2",     "hr2min30",
      "hr3",   "hr3min30", "hr4",   "hr5",          "hr6",     "hr8",   "hr10",    "hr13",     "hr16",    "hr20",
      "day1",  "day1hr12", "day2",  "day2hr12",     "day3",    "day4",  "day5",    "day7",     "day10",   "day14",
      "day19", "day24",    "day30", "dayMoreThan30"};
  return convert_enum_idx(names, 64, value, "rrm_cfg_s::ue_inactive_time_e_");
}

// HandoverPreparationInformation-IEs ::= SEQUENCE
OCUDUASN_CODE ho_prep_info_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {source_cfg_present, rrm_cfg_present, as_context_present, non_crit_ext_present}));

  HANDLE_CODE(pack_dyn_seq_of(bref, ue_cap_rat_list, 0, 8));
  if (source_cfg_present) {
    HANDLE_CODE(source_cfg.pack(bref));
  }
  if (rrm_cfg_present) {
    HANDLE_CODE(rrm_cfg.pack(bref));
  }
  if (as_context_present) {
    HANDLE_CODE(as_context.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_prep_info_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&source_cfg_present, &rrm_cfg_present, &as_context_present, &non_crit_ext_present}));

  HANDLE_CODE(unpack_dyn_seq_of(ue_cap_rat_list, bref, 0, 8));
  if (source_cfg_present) {
    HANDLE_CODE(source_cfg.unpack(bref));
  }
  if (rrm_cfg_present) {
    HANDLE_CODE(rrm_cfg.unpack(bref));
  }
  if (as_context_present) {
    HANDLE_CODE(as_context.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ho_prep_info_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("ue-CapabilityRAT-List");
  for (const auto& e1 : ue_cap_rat_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (source_cfg_present) {
    j.write_fieldname("sourceConfig");
    source_cfg.to_json(j);
  }
  if (rrm_cfg_present) {
    j.write_fieldname("rrm-Config");
    rrm_cfg.to_json(j);
  }
  if (as_context_present) {
    j.write_fieldname("as-Context");
    as_context.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// HandoverPreparationInformation ::= SEQUENCE
OCUDUASN_CODE ho_prep_info_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_prep_info_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ho_prep_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ho_prep_info_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ho_prep_info_s::crit_exts_c_::c1_c_& ho_prep_info_s::crit_exts_c_::set_c1()
{
  set(types::c1);
  return c;
}
void ho_prep_info_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ho_prep_info_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "ho_prep_info_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ho_prep_info_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ho_prep_info_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_prep_info_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ho_prep_info_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void ho_prep_info_s::crit_exts_c_::c1_c_::set(types::options e)
{
  type_ = e;
}
ho_prep_info_ies_s& ho_prep_info_s::crit_exts_c_::c1_c_::set_ho_prep_info()
{
  set(types::ho_prep_info);
  return c;
}
void ho_prep_info_s::crit_exts_c_::c1_c_::set_spare3()
{
  set(types::spare3);
}
void ho_prep_info_s::crit_exts_c_::c1_c_::set_spare2()
{
  set(types::spare2);
}
void ho_prep_info_s::crit_exts_c_::c1_c_::set_spare1()
{
  set(types::spare1);
}
void ho_prep_info_s::crit_exts_c_::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ho_prep_info:
      j.write_fieldname("handoverPreparationInformation");
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
      log_invalid_choice_id(type_, "ho_prep_info_s::crit_exts_c_::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ho_prep_info_s::crit_exts_c_::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ho_prep_info:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "ho_prep_info_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_prep_info_s::crit_exts_c_::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ho_prep_info:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "ho_prep_info_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ho_prep_info_s::crit_exts_c_::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"handoverPreparationInformation", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 4, value, "ho_prep_info_s::crit_exts_c_::c1_c_::types");
}

const char* ho_prep_info_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"c1", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ho_prep_info_s::crit_exts_c_::types");
}
uint8_t ho_prep_info_s::crit_exts_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ho_prep_info_s::crit_exts_c_::types");
}

// MeasTiming ::= SEQUENCE
OCUDUASN_CODE meas_timing_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= ssb_to_measure.is_present();
  group_flags[0] |= pci_present;
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), freq_and_timing_present}));

  if (freq_and_timing_present) {
    HANDLE_CODE(bref.pack(freq_and_timing.ss_rssi_meas_present, 1));
    HANDLE_CODE(pack_integer(bref, freq_and_timing.carrier_freq, (uint32_t)0u, (uint32_t)3279165u));
    HANDLE_CODE(freq_and_timing.ssb_subcarrier_spacing.pack(bref));
    HANDLE_CODE(freq_and_timing.ssb_meas_timing_cfg.pack(bref));
    if (freq_and_timing.ss_rssi_meas_present) {
      HANDLE_CODE(freq_and_timing.ss_rssi_meas.pack(bref));
    }
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {ssb_to_measure.is_present(), pci_present}));
      if (ssb_to_measure.is_present()) {
        HANDLE_CODE(ssb_to_measure->pack(bref));
      }
      if (pci_present) {
        HANDLE_CODE(pack_integer(bref, pci, (uint16_t)0u, (uint16_t)1007u));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_timing_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &freq_and_timing_present}));

  if (freq_and_timing_present) {
    HANDLE_CODE(bref.unpack(freq_and_timing.ss_rssi_meas_present, 1));
    HANDLE_CODE(unpack_integer(freq_and_timing.carrier_freq, bref, (uint32_t)0u, (uint32_t)3279165u));
    HANDLE_CODE(freq_and_timing.ssb_subcarrier_spacing.unpack(bref));
    HANDLE_CODE(freq_and_timing.ssb_meas_timing_cfg.unpack(bref));
    if (freq_and_timing.ss_rssi_meas_present) {
      HANDLE_CODE(freq_and_timing.ss_rssi_meas.unpack(bref));
    }
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(ssb_to_measure, bref);
      HANDLE_CODE(bref.unpack(pci_present, 1));
      if (ssb_to_measure.is_present()) {
        HANDLE_CODE(ssb_to_measure->unpack(bref));
      }
      if (pci_present) {
        HANDLE_CODE(unpack_integer(pci, bref, (uint16_t)0u, (uint16_t)1007u));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void meas_timing_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (freq_and_timing_present) {
    j.write_fieldname("frequencyAndTiming");
    j.start_obj();
    j.write_int("carrierFreq", freq_and_timing.carrier_freq);
    j.write_str("ssbSubcarrierSpacing", freq_and_timing.ssb_subcarrier_spacing.to_string());
    j.write_fieldname("ssb-MeasurementTimingConfiguration");
    freq_and_timing.ssb_meas_timing_cfg.to_json(j);
    if (freq_and_timing.ss_rssi_meas_present) {
      j.write_fieldname("ss-RSSI-Measurement");
      freq_and_timing.ss_rssi_meas.to_json(j);
    }
    j.end_obj();
  }
  if (ssb_to_measure.is_present()) {
    j.write_fieldname("ssb-ToMeasure");
    ssb_to_measure->to_json(j);
  }
  if (pci_present) {
    j.write_int("physCellId", pci);
  }
  j.end_obj();
}

// MeasurementTimingConfiguration-v1610-IEs ::= SEQUENCE
OCUDUASN_CODE meas_timing_cfg_v1610_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  HANDLE_CODE(csi_rs_cfg_r16.csi_rs_subcarrier_spacing_r16.pack(bref));
  HANDLE_CODE(csi_rs_cfg_r16.csi_rs_cell_mob_r16.pack(bref));
  HANDLE_CODE(pack_integer(bref, csi_rs_cfg_r16.ref_ssb_freq_r16, (uint32_t)0u, (uint32_t)3279165u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_timing_cfg_v1610_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  HANDLE_CODE(csi_rs_cfg_r16.csi_rs_subcarrier_spacing_r16.unpack(bref));
  HANDLE_CODE(csi_rs_cfg_r16.csi_rs_cell_mob_r16.unpack(bref));
  HANDLE_CODE(unpack_integer(csi_rs_cfg_r16.ref_ssb_freq_r16, bref, (uint32_t)0u, (uint32_t)3279165u));

  return OCUDUASN_SUCCESS;
}
void meas_timing_cfg_v1610_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("csi-RS-Config-r16");
  j.start_obj();
  j.write_str("csi-RS-SubcarrierSpacing-r16", csi_rs_cfg_r16.csi_rs_subcarrier_spacing_r16.to_string());
  j.write_fieldname("csi-RS-CellMobility-r16");
  csi_rs_cfg_r16.csi_rs_cell_mob_r16.to_json(j);
  j.write_int("refSSBFreq-r16", csi_rs_cfg_r16.ref_ssb_freq_r16);
  j.end_obj();
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// MeasurementTimingConfiguration-v1550-IEs ::= SEQUENCE
OCUDUASN_CODE meas_timing_cfg_v1550_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  HANDLE_CODE(bref.pack(camp_on_first_ssb, 1));
  HANDLE_CODE(bref.pack(ps_cell_only_on_first_ssb, 1));
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_timing_cfg_v1550_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  HANDLE_CODE(bref.unpack(camp_on_first_ssb, 1));
  HANDLE_CODE(bref.unpack(ps_cell_only_on_first_ssb, 1));
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void meas_timing_cfg_v1550_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_bool("campOnFirstSSB", camp_on_first_ssb);
  j.write_bool("psCellOnlyOnFirstSSB", ps_cell_only_on_first_ssb);
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// MeasurementTimingConfiguration-IEs ::= SEQUENCE
OCUDUASN_CODE meas_timing_cfg_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {meas_timing.size() > 0, non_crit_ext_present}));

  if (meas_timing.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, meas_timing, 1, 32));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_timing_cfg_ies_s::unpack(cbit_ref& bref)
{
  bool meas_timing_present;
  HANDLE_CODE(unpack_bits(bref, {&meas_timing_present, &non_crit_ext_present}));

  if (meas_timing_present) {
    HANDLE_CODE(unpack_dyn_seq_of(meas_timing, bref, 1, 32));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void meas_timing_cfg_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (meas_timing.size() > 0) {
    j.start_array("measTiming");
    for (const auto& e1 : meas_timing) {
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

// MeasurementTimingConfiguration ::= SEQUENCE
OCUDUASN_CODE meas_timing_cfg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_timing_cfg_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void meas_timing_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void meas_timing_cfg_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
meas_timing_cfg_s::crit_exts_c_::c1_c_& meas_timing_cfg_s::crit_exts_c_::set_c1()
{
  set(types::c1);
  return c;
}
void meas_timing_cfg_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void meas_timing_cfg_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "meas_timing_cfg_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_timing_cfg_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "meas_timing_cfg_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_timing_cfg_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "meas_timing_cfg_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void meas_timing_cfg_s::crit_exts_c_::c1_c_::set(types::options e)
{
  type_ = e;
}
meas_timing_cfg_ies_s& meas_timing_cfg_s::crit_exts_c_::c1_c_::set_meas_timing_conf()
{
  set(types::meas_timing_conf);
  return c;
}
void meas_timing_cfg_s::crit_exts_c_::c1_c_::set_spare3()
{
  set(types::spare3);
}
void meas_timing_cfg_s::crit_exts_c_::c1_c_::set_spare2()
{
  set(types::spare2);
}
void meas_timing_cfg_s::crit_exts_c_::c1_c_::set_spare1()
{
  set(types::spare1);
}
void meas_timing_cfg_s::crit_exts_c_::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::meas_timing_conf:
      j.write_fieldname("measTimingConf");
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
      log_invalid_choice_id(type_, "meas_timing_cfg_s::crit_exts_c_::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_timing_cfg_s::crit_exts_c_::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::meas_timing_conf:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "meas_timing_cfg_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_timing_cfg_s::crit_exts_c_::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::meas_timing_conf:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "meas_timing_cfg_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_timing_cfg_s::crit_exts_c_::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"measTimingConf", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 4, value, "meas_timing_cfg_s::crit_exts_c_::c1_c_::types");
}

const char* meas_timing_cfg_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"c1", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "meas_timing_cfg_s::crit_exts_c_::types");
}
uint8_t meas_timing_cfg_s::crit_exts_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "meas_timing_cfg_s::crit_exts_c_::types");
}

// UERadioAccessCapabilityInformation-IEs ::= SEQUENCE
OCUDUASN_CODE ue_radio_access_cap_info_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(non_crit_ext_present, 1));

  HANDLE_CODE(ue_radio_access_cap_info.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_access_cap_info_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(non_crit_ext_present, 1));

  HANDLE_CODE(ue_radio_access_cap_info.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ue_radio_access_cap_info_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("ue-RadioAccessCapabilityInfo", ue_radio_access_cap_info.to_string());
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UERadioAccessCapabilityInformation ::= SEQUENCE
OCUDUASN_CODE ue_radio_access_cap_info_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_access_cap_info_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ue_radio_access_cap_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ue_radio_access_cap_info_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ue_radio_access_cap_info_s::crit_exts_c_::c1_c_& ue_radio_access_cap_info_s::crit_exts_c_::set_c1()
{
  set(types::c1);
  return c;
}
void ue_radio_access_cap_info_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ue_radio_access_cap_info_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "ue_radio_access_cap_info_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_radio_access_cap_info_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_radio_access_cap_info_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_access_cap_info_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_radio_access_cap_info_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set(types::options e)
{
  type_ = e;
}
ue_radio_access_cap_info_ies_s& ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set_ue_radio_access_cap_info()
{
  set(types::ue_radio_access_cap_info);
  return c;
}
void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set_spare7()
{
  set(types::spare7);
}
void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set_spare6()
{
  set(types::spare6);
}
void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set_spare5()
{
  set(types::spare5);
}
void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set_spare4()
{
  set(types::spare4);
}
void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set_spare3()
{
  set(types::spare3);
}
void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set_spare2()
{
  set(types::spare2);
}
void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::set_spare1()
{
  set(types::spare1);
}
void ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ue_radio_access_cap_info:
      j.write_fieldname("ueRadioAccessCapabilityInformation");
      c.to_json(j);
      break;
    case types::spare7:
      j.write_null("spare7");
      break;
    case types::spare6:
      j.write_null("spare6");
      break;
    case types::spare5:
      j.write_null("spare5");
      break;
    case types::spare4:
      j.write_null("spare4");
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
      log_invalid_choice_id(type_, "ue_radio_access_cap_info_s::crit_exts_c_::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ue_radio_access_cap_info:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare7:
      break;
    case types::spare6:
      break;
    case types::spare5:
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
      log_invalid_choice_id(type_, "ue_radio_access_cap_info_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ue_radio_access_cap_info:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare7:
      break;
    case types::spare6:
      break;
    case types::spare5:
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
      log_invalid_choice_id(type_, "ue_radio_access_cap_info_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {
      "ueRadioAccessCapabilityInformation", "spare7", "spare6", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "ue_radio_access_cap_info_s::crit_exts_c_::c1_c_::types");
}

const char* ue_radio_access_cap_info_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"c1", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ue_radio_access_cap_info_s::crit_exts_c_::types");
}
uint8_t ue_radio_access_cap_info_s::crit_exts_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ue_radio_access_cap_info_s::crit_exts_c_::types");
}

// UERadioPagingInformation-v1840-IEs ::= SEQUENCE
OCUDUASN_CODE ue_radio_paging_info_v1840_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {dl_sched_offset_pdsch_type_a_fdd_fr2_ntn_r18_present,
                         dl_sched_offset_pdsch_type_b_fdd_fr2_ntn_r18_present,
                         non_crit_ext_present}));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_paging_info_v1840_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&dl_sched_offset_pdsch_type_a_fdd_fr2_ntn_r18_present,
                           &dl_sched_offset_pdsch_type_b_fdd_fr2_ntn_r18_present,
                           &non_crit_ext_present}));

  return OCUDUASN_SUCCESS;
}
void ue_radio_paging_info_v1840_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (dl_sched_offset_pdsch_type_a_fdd_fr2_ntn_r18_present) {
    j.write_str("dl-SchedulingOffset-PDSCH-TypeA-FDD-FR2-NTN-r18", "supported");
  }
  if (dl_sched_offset_pdsch_type_b_fdd_fr2_ntn_r18_present) {
    j.write_str("dl-SchedulingOffset-PDSCH-TypeB-FDD-FR2-NTN-r18", "supported");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UERadioPagingInformation-v1800-IEs ::= SEQUENCE
OCUDUASN_CODE ue_radio_paging_info_v1800_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {nof_rx_e_red_cap_r18_present, support_of2_rx_xr_r18_present, non_crit_ext_present}));

  if (nof_rx_e_red_cap_r18_present) {
    HANDLE_CODE(nof_rx_e_red_cap_r18.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_paging_info_v1800_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(
      unpack_bits(bref, {&nof_rx_e_red_cap_r18_present, &support_of2_rx_xr_r18_present, &non_crit_ext_present}));

  if (nof_rx_e_red_cap_r18_present) {
    HANDLE_CODE(nof_rx_e_red_cap_r18.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_radio_paging_info_v1800_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (nof_rx_e_red_cap_r18_present) {
    j.write_str("numberOfRxERedCap-r18", nof_rx_e_red_cap_r18.to_string());
  }
  if (support_of2_rx_xr_r18_present) {
    j.write_str("supportOf2RxXR-r18", "supported");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

const char* ue_radio_paging_info_v1800_ies_s::nof_rx_e_red_cap_r18_opts::to_string() const
{
  static const char* names[] = {"one", "two"};
  return convert_enum_idx(names, 2, value, "ue_radio_paging_info_v1800_ies_s::nof_rx_e_red_cap_r18_e_");
}
uint8_t ue_radio_paging_info_v1800_ies_s::nof_rx_e_red_cap_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "ue_radio_paging_info_v1800_ies_s::nof_rx_e_red_cap_r18_e_");
}

// UERadioPagingInformation-v1700-IEs ::= SEQUENCE
OCUDUASN_CODE ue_radio_paging_info_v1700_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {ue_radio_paging_info_r17.size() > 0,
                         inactive_state_po_determination_r17_present,
                         nof_rx_red_cap_r17_present,
                         half_duplex_fdd_type_a_red_cap_r17.size() > 0,
                         non_crit_ext_present}));

  if (ue_radio_paging_info_r17.size() > 0) {
    HANDLE_CODE(ue_radio_paging_info_r17.pack(bref));
  }
  if (nof_rx_red_cap_r17_present) {
    HANDLE_CODE(nof_rx_red_cap_r17.pack(bref));
  }
  if (half_duplex_fdd_type_a_red_cap_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, half_duplex_fdd_type_a_red_cap_r17, 1, 1024, integer_packer<uint16_t>(1, 1024)));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_paging_info_v1700_ies_s::unpack(cbit_ref& bref)
{
  bool ue_radio_paging_info_r17_present;
  bool half_duplex_fdd_type_a_red_cap_r17_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ue_radio_paging_info_r17_present,
                           &inactive_state_po_determination_r17_present,
                           &nof_rx_red_cap_r17_present,
                           &half_duplex_fdd_type_a_red_cap_r17_present,
                           &non_crit_ext_present}));

  if (ue_radio_paging_info_r17_present) {
    HANDLE_CODE(ue_radio_paging_info_r17.unpack(bref));
  }
  if (nof_rx_red_cap_r17_present) {
    HANDLE_CODE(nof_rx_red_cap_r17.unpack(bref));
  }
  if (half_duplex_fdd_type_a_red_cap_r17_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(half_duplex_fdd_type_a_red_cap_r17, bref, 1, 1024, integer_packer<uint16_t>(1, 1024)));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_radio_paging_info_v1700_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ue_radio_paging_info_r17.size() > 0) {
    j.write_str("ue-RadioPagingInfo-r17", ue_radio_paging_info_r17.to_string());
  }
  if (inactive_state_po_determination_r17_present) {
    j.write_str("inactiveStatePO-Determination-r17", "supported");
  }
  if (nof_rx_red_cap_r17_present) {
    j.write_str("numberOfRxRedCap-r17", nof_rx_red_cap_r17.to_string());
  }
  if (half_duplex_fdd_type_a_red_cap_r17.size() > 0) {
    j.start_array("halfDuplexFDD-TypeA-RedCap-r17");
    for (const auto& e1 : half_duplex_fdd_type_a_red_cap_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

const char* ue_radio_paging_info_v1700_ies_s::nof_rx_red_cap_r17_opts::to_string() const
{
  static const char* names[] = {"one", "two"};
  return convert_enum_idx(names, 2, value, "ue_radio_paging_info_v1700_ies_s::nof_rx_red_cap_r17_e_");
}
uint8_t ue_radio_paging_info_v1700_ies_s::nof_rx_red_cap_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "ue_radio_paging_info_v1700_ies_s::nof_rx_red_cap_r17_e_");
}

// UERadioPagingInformation-v15e0-IEs ::= SEQUENCE
OCUDUASN_CODE ue_radio_paging_info_v15e0_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {dl_sched_offset_pdsch_type_a_fdd_fr1_present,
                         dl_sched_offset_pdsch_type_a_tdd_fr1_present,
                         dl_sched_offset_pdsch_type_a_tdd_fr2_present,
                         dl_sched_offset_pdsch_type_b_fdd_fr1_present,
                         dl_sched_offset_pdsch_type_b_tdd_fr1_present,
                         dl_sched_offset_pdsch_type_b_tdd_fr2_present,
                         non_crit_ext_present}));

  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_paging_info_v15e0_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&dl_sched_offset_pdsch_type_a_fdd_fr1_present,
                           &dl_sched_offset_pdsch_type_a_tdd_fr1_present,
                           &dl_sched_offset_pdsch_type_a_tdd_fr2_present,
                           &dl_sched_offset_pdsch_type_b_fdd_fr1_present,
                           &dl_sched_offset_pdsch_type_b_tdd_fr1_present,
                           &dl_sched_offset_pdsch_type_b_tdd_fr2_present,
                           &non_crit_ext_present}));

  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_radio_paging_info_v15e0_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (dl_sched_offset_pdsch_type_a_fdd_fr1_present) {
    j.write_str("dl-SchedulingOffset-PDSCH-TypeA-FDD-FR1", "supported");
  }
  if (dl_sched_offset_pdsch_type_a_tdd_fr1_present) {
    j.write_str("dl-SchedulingOffset-PDSCH-TypeA-TDD-FR1", "supported");
  }
  if (dl_sched_offset_pdsch_type_a_tdd_fr2_present) {
    j.write_str("dl-SchedulingOffset-PDSCH-TypeA-TDD-FR2", "supported");
  }
  if (dl_sched_offset_pdsch_type_b_fdd_fr1_present) {
    j.write_str("dl-SchedulingOffset-PDSCH-TypeB-FDD-FR1", "supported");
  }
  if (dl_sched_offset_pdsch_type_b_tdd_fr1_present) {
    j.write_str("dl-SchedulingOffset-PDSCH-TypeB-TDD-FR1", "supported");
  }
  if (dl_sched_offset_pdsch_type_b_tdd_fr2_present) {
    j.write_str("dl-SchedulingOffset-PDSCH-TypeB-TDD-FR2", "supported");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UERadioPagingInformation-IEs ::= SEQUENCE
OCUDUASN_CODE ue_radio_paging_info_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {supported_band_list_nr_for_paging.size() > 0, non_crit_ext_present}));

  if (supported_band_list_nr_for_paging.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_list_nr_for_paging, 1, 1024, integer_packer<uint16_t>(1, 1024)));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_paging_info_ies_s::unpack(cbit_ref& bref)
{
  bool supported_band_list_nr_for_paging_present;
  HANDLE_CODE(unpack_bits(bref, {&supported_band_list_nr_for_paging_present, &non_crit_ext_present}));

  if (supported_band_list_nr_for_paging_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_list_nr_for_paging, bref, 1, 1024, integer_packer<uint16_t>(1, 1024)));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_radio_paging_info_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (supported_band_list_nr_for_paging.size() > 0) {
    j.start_array("supportedBandListNRForPaging");
    for (const auto& e1 : supported_band_list_nr_for_paging) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UERadioPagingInformation ::= SEQUENCE
OCUDUASN_CODE ue_radio_paging_info_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_paging_info_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ue_radio_paging_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void ue_radio_paging_info_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
ue_radio_paging_info_s::crit_exts_c_::c1_c_& ue_radio_paging_info_s::crit_exts_c_::set_c1()
{
  set(types::c1);
  return c;
}
void ue_radio_paging_info_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void ue_radio_paging_info_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "ue_radio_paging_info_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_radio_paging_info_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_radio_paging_info_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_paging_info_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "ue_radio_paging_info_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void ue_radio_paging_info_s::crit_exts_c_::c1_c_::set(types::options e)
{
  type_ = e;
}
ue_radio_paging_info_ies_s& ue_radio_paging_info_s::crit_exts_c_::c1_c_::set_ue_radio_paging_info()
{
  set(types::ue_radio_paging_info);
  return c;
}
void ue_radio_paging_info_s::crit_exts_c_::c1_c_::set_spare7()
{
  set(types::spare7);
}
void ue_radio_paging_info_s::crit_exts_c_::c1_c_::set_spare6()
{
  set(types::spare6);
}
void ue_radio_paging_info_s::crit_exts_c_::c1_c_::set_spare5()
{
  set(types::spare5);
}
void ue_radio_paging_info_s::crit_exts_c_::c1_c_::set_spare4()
{
  set(types::spare4);
}
void ue_radio_paging_info_s::crit_exts_c_::c1_c_::set_spare3()
{
  set(types::spare3);
}
void ue_radio_paging_info_s::crit_exts_c_::c1_c_::set_spare2()
{
  set(types::spare2);
}
void ue_radio_paging_info_s::crit_exts_c_::c1_c_::set_spare1()
{
  set(types::spare1);
}
void ue_radio_paging_info_s::crit_exts_c_::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ue_radio_paging_info:
      j.write_fieldname("ueRadioPagingInformation");
      c.to_json(j);
      break;
    case types::spare7:
      j.write_null("spare7");
      break;
    case types::spare6:
      j.write_null("spare6");
      break;
    case types::spare5:
      j.write_null("spare5");
      break;
    case types::spare4:
      j.write_null("spare4");
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
      log_invalid_choice_id(type_, "ue_radio_paging_info_s::crit_exts_c_::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ue_radio_paging_info_s::crit_exts_c_::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ue_radio_paging_info:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare7:
      break;
    case types::spare6:
      break;
    case types::spare5:
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
      log_invalid_choice_id(type_, "ue_radio_paging_info_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_paging_info_s::crit_exts_c_::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ue_radio_paging_info:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare7:
      break;
    case types::spare6:
      break;
    case types::spare5:
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
      log_invalid_choice_id(type_, "ue_radio_paging_info_s::crit_exts_c_::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ue_radio_paging_info_s::crit_exts_c_::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {
      "ueRadioPagingInformation", "spare7", "spare6", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "ue_radio_paging_info_s::crit_exts_c_::c1_c_::types");
}

const char* ue_radio_paging_info_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"c1", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "ue_radio_paging_info_s::crit_exts_c_::types");
}
uint8_t ue_radio_paging_info_s::crit_exts_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ue_radio_paging_info_s::crit_exts_c_::types");
}

// PDCCH-BlindDetectionMixedList-r16 ::= SEQUENCE
OCUDUASN_CODE pdcch_blind_detection_mixed_list_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref, {pdcch_blind_detection_ca_mixed_ext_r16_present, pdcch_blind_detection_cg_ue_mixed_ext_r16_present}));

  if (pdcch_blind_detection_ca_mixed_ext_r16_present) {
    HANDLE_CODE(pdcch_blind_detection_ca_mixed_ext_r16.pack(bref));
  }
  if (pdcch_blind_detection_cg_ue_mixed_ext_r16_present) {
    HANDLE_CODE(pdcch_blind_detection_cg_ue_mixed_ext_r16.pdcch_blind_detection_mcg_ue_mixed_v16a0.pack(bref));
    HANDLE_CODE(pdcch_blind_detection_cg_ue_mixed_ext_r16.pdcch_blind_detection_scg_ue_mixed_v16a0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcch_blind_detection_mixed_list_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(
      bref, {&pdcch_blind_detection_ca_mixed_ext_r16_present, &pdcch_blind_detection_cg_ue_mixed_ext_r16_present}));

  if (pdcch_blind_detection_ca_mixed_ext_r16_present) {
    HANDLE_CODE(pdcch_blind_detection_ca_mixed_ext_r16.unpack(bref));
  }
  if (pdcch_blind_detection_cg_ue_mixed_ext_r16_present) {
    HANDLE_CODE(pdcch_blind_detection_cg_ue_mixed_ext_r16.pdcch_blind_detection_mcg_ue_mixed_v16a0.unpack(bref));
    HANDLE_CODE(pdcch_blind_detection_cg_ue_mixed_ext_r16.pdcch_blind_detection_scg_ue_mixed_v16a0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void pdcch_blind_detection_mixed_list_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdcch_blind_detection_ca_mixed_ext_r16_present) {
    j.write_fieldname("pdcch-BlindDetectionCA-MixedExt-r16");
    pdcch_blind_detection_ca_mixed_ext_r16.to_json(j);
  }
  if (pdcch_blind_detection_cg_ue_mixed_ext_r16_present) {
    j.write_fieldname("pdcch-BlindDetectionCG-UE-MixedExt-r16");
    j.start_obj();
    j.write_fieldname("pdcch-BlindDetectionMCG-UE-Mixed-v16a0");
    pdcch_blind_detection_cg_ue_mixed_ext_r16.pdcch_blind_detection_mcg_ue_mixed_v16a0.to_json(j);
    j.write_fieldname("pdcch-BlindDetectionSCG-UE-Mixed-v16a0");
    pdcch_blind_detection_cg_ue_mixed_ext_r16.pdcch_blind_detection_scg_ue_mixed_v16a0.to_json(j);
    j.end_obj();
  }
  j.end_obj();
}

void pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::destroy_()
{
  switch (type_) {
    case types::pdcch_blind_detection_ca_mixed_v16a0:
      c.destroy<pdcch_blind_detection_ca_mixed_ext_r16_s>();
      break;
    case types::pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0:
      c.destroy<pdcch_blind_detection_ca_mixed_ext_r16_s>();
      break;
    default:
      break;
  }
}
void pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::pdcch_blind_detection_ca_mixed_v16a0:
      c.init<pdcch_blind_detection_ca_mixed_ext_r16_s>();
      break;
    case types::pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0:
      c.init<pdcch_blind_detection_ca_mixed_ext_r16_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_");
  }
}
pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::
    pdcch_blind_detection_ca_mixed_ext_r16_c_(
        const pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::pdcch_blind_detection_ca_mixed_v16a0:
      c.init(other.c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>());
      break;
    case types::pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0:
      c.init(other.c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_");
  }
}
pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_&
pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::operator=(
    const pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::pdcch_blind_detection_ca_mixed_v16a0:
      c.set(other.c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>());
      break;
    case types::pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0:
      c.set(other.c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_");
  }

  return *this;
}
pdcch_blind_detection_ca_mixed_ext_r16_s& pdcch_blind_detection_mixed_list_r16_s::
    pdcch_blind_detection_ca_mixed_ext_r16_c_::set_pdcch_blind_detection_ca_mixed_v16a0()
{
  set(types::pdcch_blind_detection_ca_mixed_v16a0);
  return c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>();
}
pdcch_blind_detection_ca_mixed_ext_r16_s& pdcch_blind_detection_mixed_list_r16_s::
    pdcch_blind_detection_ca_mixed_ext_r16_c_::set_pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0()
{
  set(types::pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0);
  return c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>();
}
void pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::pdcch_blind_detection_ca_mixed_v16a0:
      j.write_fieldname("pdcch-BlindDetectionCA-Mixed-v16a0");
      c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>().to_json(j);
      break;
    case types::pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0:
      j.write_fieldname("pdcch-BlindDetectionCA-Mixed-NonAlignedSpan-v16a0");
      c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE
pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::pdcch_blind_detection_ca_mixed_v16a0:
      HANDLE_CODE(c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>().pack(bref));
      break;
    case types::pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0:
      HANDLE_CODE(c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::pdcch_blind_detection_ca_mixed_v16a0:
      HANDLE_CODE(c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>().unpack(bref));
      break;
    case types::pdcch_blind_detection_ca_mixed_non_aligned_span_v16a0:
      HANDLE_CODE(c.get<pdcch_blind_detection_ca_mixed_ext_r16_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char*
pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::types_opts::to_string() const
{
  static const char* names[] = {"pdcch-BlindDetectionCA-Mixed-v16a0",
                                "pdcch-BlindDetectionCA-Mixed-NonAlignedSpan-v16a0"};
  return convert_enum_idx(
      names, 2, value, "pdcch_blind_detection_mixed_list_r16_s::pdcch_blind_detection_ca_mixed_ext_r16_c_::types");
}

// CA-ParametersNR-v16a0 ::= SEQUENCE
OCUDUASN_CODE ca_params_nr_v16a0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_dyn_seq_of(bref, pdcch_blind_detection_mixed_list_r16, 1, 7));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ca_params_nr_v16a0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_dyn_seq_of(pdcch_blind_detection_mixed_list_r16, bref, 1, 7));

  return OCUDUASN_SUCCESS;
}
void ca_params_nr_v16a0_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("pdcch-BlindDetectionMixedList-r16");
  for (const auto& e1 : pdcch_blind_detection_mixed_list_r16) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// CA-ParametersNRDC-v16a0 ::= SEQUENCE
OCUDUASN_CODE ca_params_nrdc_v16a0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ca_params_nr_for_dc_v16a0_present, 1));

  if (ca_params_nr_for_dc_v16a0_present) {
    HANDLE_CODE(ca_params_nr_for_dc_v16a0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ca_params_nrdc_v16a0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ca_params_nr_for_dc_v16a0_present, 1));

  if (ca_params_nr_for_dc_v16a0_present) {
    HANDLE_CODE(ca_params_nr_for_dc_v16a0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ca_params_nrdc_v16a0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ca_params_nr_for_dc_v16a0_present) {
    j.write_fieldname("ca-ParametersNR-ForDC-v16a0");
    ca_params_nr_for_dc_v16a0.to_json(j);
  }
  j.end_obj();
}

// BandCombination-v16a0 ::= SEQUENCE
OCUDUASN_CODE band_combination_v16a0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {ca_params_nr_v16a0_present, ca_params_nrdc_v16a0_present}));

  if (ca_params_nr_v16a0_present) {
    HANDLE_CODE(ca_params_nr_v16a0.pack(bref));
  }
  if (ca_params_nrdc_v16a0_present) {
    HANDLE_CODE(ca_params_nrdc_v16a0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_combination_v16a0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&ca_params_nr_v16a0_present, &ca_params_nrdc_v16a0_present}));

  if (ca_params_nr_v16a0_present) {
    HANDLE_CODE(ca_params_nr_v16a0.unpack(bref));
  }
  if (ca_params_nrdc_v16a0_present) {
    HANDLE_CODE(ca_params_nrdc_v16a0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void band_combination_v16a0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ca_params_nr_v16a0_present) {
    j.write_fieldname("ca-ParametersNR-v16a0");
    ca_params_nr_v16a0.to_json(j);
  }
  if (ca_params_nrdc_v16a0_present) {
    j.write_fieldname("ca-ParametersNRDC-v16a0");
    ca_params_nrdc_v16a0.to_json(j);
  }
  j.end_obj();
}

// BandCombination-UplinkTxSwitch-v16a0 ::= SEQUENCE
OCUDUASN_CODE band_combination_ul_tx_switch_v16a0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(band_combination_v16a0_present, 1));

  if (band_combination_v16a0_present) {
    HANDLE_CODE(band_combination_v16a0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_combination_ul_tx_switch_v16a0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(band_combination_v16a0_present, 1));

  if (band_combination_v16a0_present) {
    HANDLE_CODE(band_combination_v16a0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void band_combination_ul_tx_switch_v16a0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (band_combination_v16a0_present) {
    j.write_fieldname("bandCombination-v16a0");
    band_combination_v16a0.to_json(j);
  }
  j.end_obj();
}

// CA-ParametersNRDC-v16j0 ::= SEQUENCE
OCUDUASN_CODE ca_params_nrdc_v16j0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ca_params_nr_for_dc_v16j0_present, 1));

  if (ca_params_nr_for_dc_v16j0_present) {
    HANDLE_CODE(ca_params_nr_for_dc_v16j0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ca_params_nrdc_v16j0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ca_params_nr_for_dc_v16j0_present, 1));

  if (ca_params_nr_for_dc_v16j0_present) {
    HANDLE_CODE(ca_params_nr_for_dc_v16j0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ca_params_nrdc_v16j0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ca_params_nr_for_dc_v16j0_present) {
    j.write_fieldname("ca-ParametersNR-ForDC-v16j0");
    ca_params_nr_for_dc_v16j0.to_json(j);
  }
  j.end_obj();
}

// BandCombination-v16j0 ::= SEQUENCE
OCUDUASN_CODE band_combination_v16j0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {ca_params_nr_v16j0_present, ca_params_nrdc_v16j0_present}));

  if (ca_params_nr_v16j0_present) {
    HANDLE_CODE(ca_params_nr_v16j0.pack(bref));
  }
  if (ca_params_nrdc_v16j0_present) {
    HANDLE_CODE(ca_params_nrdc_v16j0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_combination_v16j0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&ca_params_nr_v16j0_present, &ca_params_nrdc_v16j0_present}));

  if (ca_params_nr_v16j0_present) {
    HANDLE_CODE(ca_params_nr_v16j0.unpack(bref));
  }
  if (ca_params_nrdc_v16j0_present) {
    HANDLE_CODE(ca_params_nrdc_v16j0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void band_combination_v16j0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ca_params_nr_v16j0_present) {
    j.write_fieldname("ca-ParametersNR-v16j0");
    ca_params_nr_v16j0.to_json(j);
  }
  if (ca_params_nrdc_v16j0_present) {
    j.write_fieldname("ca-ParametersNRDC-v16j0");
    ca_params_nrdc_v16j0.to_json(j);
  }
  j.end_obj();
}

// BandCombination-UplinkTxSwitch-v16j0 ::= SEQUENCE
OCUDUASN_CODE band_combination_ul_tx_switch_v16j0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(band_combination_v16j0_present, 1));

  if (band_combination_v16j0_present) {
    HANDLE_CODE(band_combination_v16j0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_combination_ul_tx_switch_v16j0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(band_combination_v16j0_present, 1));

  if (band_combination_v16j0_present) {
    HANDLE_CODE(band_combination_v16j0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void band_combination_ul_tx_switch_v16j0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (band_combination_v16j0_present) {
    j.write_fieldname("bandCombination-v16j0");
    band_combination_v16j0.to_json(j);
  }
  j.end_obj();
}

// CA-ParametersNRDC-v17b0 ::= SEQUENCE
OCUDUASN_CODE ca_params_nrdc_v17b0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ca_params_nr_for_dc_v17b0_present, 1));

  if (ca_params_nr_for_dc_v17b0_present) {
    HANDLE_CODE(ca_params_nr_for_dc_v17b0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ca_params_nrdc_v17b0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ca_params_nr_for_dc_v17b0_present, 1));

  if (ca_params_nr_for_dc_v17b0_present) {
    HANDLE_CODE(ca_params_nr_for_dc_v17b0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ca_params_nrdc_v17b0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ca_params_nr_for_dc_v17b0_present) {
    j.write_fieldname("ca-ParametersNR-ForDC-v17b0");
    ca_params_nr_for_dc_v17b0.to_json(j);
  }
  j.end_obj();
}

// BandCombination-v17b0 ::= SEQUENCE
OCUDUASN_CODE band_combination_v17b0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {ca_params_nr_v17b0_present, ca_params_nrdc_v17b0_present}));

  if (ca_params_nr_v17b0_present) {
    HANDLE_CODE(ca_params_nr_v17b0.pack(bref));
  }
  if (ca_params_nrdc_v17b0_present) {
    HANDLE_CODE(ca_params_nrdc_v17b0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_combination_v17b0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&ca_params_nr_v17b0_present, &ca_params_nrdc_v17b0_present}));

  if (ca_params_nr_v17b0_present) {
    HANDLE_CODE(ca_params_nr_v17b0.unpack(bref));
  }
  if (ca_params_nrdc_v17b0_present) {
    HANDLE_CODE(ca_params_nrdc_v17b0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void band_combination_v17b0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ca_params_nr_v17b0_present) {
    j.write_fieldname("ca-ParametersNR-v17b0");
    ca_params_nr_v17b0.to_json(j);
  }
  if (ca_params_nrdc_v17b0_present) {
    j.write_fieldname("ca-ParametersNRDC-v17b0");
    ca_params_nrdc_v17b0.to_json(j);
  }
  j.end_obj();
}

// BandCombination-UplinkTxSwitch-v17b0 ::= SEQUENCE
OCUDUASN_CODE band_combination_ul_tx_switch_v17b0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(band_combination_v17b0_present, 1));

  if (band_combination_v17b0_present) {
    HANDLE_CODE(band_combination_v17b0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_combination_ul_tx_switch_v17b0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(band_combination_v17b0_present, 1));

  if (band_combination_v17b0_present) {
    HANDLE_CODE(band_combination_v17b0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void band_combination_ul_tx_switch_v17b0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (band_combination_v17b0_present) {
    j.write_fieldname("bandCombination-v17b0");
    band_combination_v17b0.to_json(j);
  }
  j.end_obj();
}

// BandNR-v16c0 ::= SEQUENCE
OCUDUASN_CODE band_nr_v16c0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, pusch_repeat_type_a_v16c0_present}));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_nr_v16c0_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &pusch_repeat_type_a_v16c0_present}));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void band_nr_v16c0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pusch_repeat_type_a_v16c0_present) {
    j.write_str("pusch-RepetitionTypeA-v16c0", "supported");
  }
  j.end_obj();
}

// MIMO-ParametersPerBand-v17b0 ::= SEQUENCE
OCUDUASN_CODE mimo_params_per_band_v17b0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(
      pack_bits(bref, {unified_joint_tci_multi_mac_ce_v17b0_present, unified_separate_tci_multi_mac_ce_v17b0_present}));

  if (unified_joint_tci_multi_mac_ce_v17b0_present) {
    HANDLE_CODE(unified_joint_tci_multi_mac_ce_v17b0.min_beam_application_time_joint_tci_v17b0.pack(bref));
    HANDLE_CODE(unified_joint_tci_multi_mac_ce_v17b0.max_num_mac_ce_per_cc_v17b0.pack(bref));
  }
  if (unified_separate_tci_multi_mac_ce_v17b0_present) {
    HANDLE_CODE(unified_separate_tci_multi_mac_ce_v17b0.min_beam_application_time_separate_tci_v17b0.pack(bref));
    HANDLE_CODE(pack_integer(
        bref, unified_separate_tci_multi_mac_ce_v17b0.max_activ_dl_tci_per_cc_v17b0, (uint8_t)2u, (uint8_t)8u));
    HANDLE_CODE(pack_integer(
        bref, unified_separate_tci_multi_mac_ce_v17b0.max_activ_ul_tci_per_cc_v17b0, (uint8_t)2u, (uint8_t)8u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mimo_params_per_band_v17b0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(
      bref, {&unified_joint_tci_multi_mac_ce_v17b0_present, &unified_separate_tci_multi_mac_ce_v17b0_present}));

  if (unified_joint_tci_multi_mac_ce_v17b0_present) {
    HANDLE_CODE(unified_joint_tci_multi_mac_ce_v17b0.min_beam_application_time_joint_tci_v17b0.unpack(bref));
    HANDLE_CODE(unified_joint_tci_multi_mac_ce_v17b0.max_num_mac_ce_per_cc_v17b0.unpack(bref));
  }
  if (unified_separate_tci_multi_mac_ce_v17b0_present) {
    HANDLE_CODE(unified_separate_tci_multi_mac_ce_v17b0.min_beam_application_time_separate_tci_v17b0.unpack(bref));
    HANDLE_CODE(unpack_integer(
        unified_separate_tci_multi_mac_ce_v17b0.max_activ_dl_tci_per_cc_v17b0, bref, (uint8_t)2u, (uint8_t)8u));
    HANDLE_CODE(unpack_integer(
        unified_separate_tci_multi_mac_ce_v17b0.max_activ_ul_tci_per_cc_v17b0, bref, (uint8_t)2u, (uint8_t)8u));
  }

  return OCUDUASN_SUCCESS;
}
void mimo_params_per_band_v17b0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (unified_joint_tci_multi_mac_ce_v17b0_present) {
    j.write_fieldname("unifiedJointTCI-multiMAC-CE-v17b0");
    j.start_obj();
    j.write_fieldname("minBeamApplicationTimeJointTCI-v17b0");
    unified_joint_tci_multi_mac_ce_v17b0.min_beam_application_time_joint_tci_v17b0.to_json(j);
    j.write_str("maxNumMAC-CE-PerCC-v17b0",
                unified_joint_tci_multi_mac_ce_v17b0.max_num_mac_ce_per_cc_v17b0.to_string());
    j.end_obj();
  }
  if (unified_separate_tci_multi_mac_ce_v17b0_present) {
    j.write_fieldname("unifiedSeparateTCI-multiMAC-CE-v17b0");
    j.start_obj();
    j.write_fieldname("minBeamApplicationTimeSeparateTCI-v17b0");
    unified_separate_tci_multi_mac_ce_v17b0.min_beam_application_time_separate_tci_v17b0.to_json(j);
    j.write_int("maxActivatedDL-TCIPerCC-v17b0", unified_separate_tci_multi_mac_ce_v17b0.max_activ_dl_tci_per_cc_v17b0);
    j.write_int("maxActivatedUL-TCIPerCC-v17b0", unified_separate_tci_multi_mac_ce_v17b0.max_activ_ul_tci_per_cc_v17b0);
    j.end_obj();
  }
  j.end_obj();
}

void mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::destroy_()
{
  switch (type_) {
    case types::fr1_v17b0:
      c.destroy<fr1_v17b0_s_>();
      break;
    case types::fr2_v17b0:
      c.destroy<fr2_v17b0_s_>();
      break;
    default:
      break;
  }
}
void mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::fr1_v17b0:
      c.init<fr1_v17b0_s_>();
      break;
    case types::fr2_v17b0:
      c.init<fr2_v17b0_s_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_joint_tci_v17b0_c_");
  }
}
mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_time_joint_tci_v17b0_c_::
    min_beam_application_time_joint_tci_v17b0_c_(
        const mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
            min_beam_application_time_joint_tci_v17b0_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::fr1_v17b0:
      c.init(other.c.get<fr1_v17b0_s_>());
      break;
    case types::fr2_v17b0:
      c.init(other.c.get<fr2_v17b0_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_joint_tci_v17b0_c_");
  }
}
mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_time_joint_tci_v17b0_c_&
mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_time_joint_tci_v17b0_c_::
operator=(const mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
              min_beam_application_time_joint_tci_v17b0_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::fr1_v17b0:
      c.set(other.c.get<fr1_v17b0_s_>());
      break;
    case types::fr2_v17b0:
      c.set(other.c.get<fr2_v17b0_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_joint_tci_v17b0_c_");
  }

  return *this;
}
mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_time_joint_tci_v17b0_c_::
    fr1_v17b0_s_&
    mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
        min_beam_application_time_joint_tci_v17b0_c_::set_fr1_v17b0()
{
  set(types::fr1_v17b0);
  return c.get<fr1_v17b0_s_>();
}
mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_time_joint_tci_v17b0_c_::
    fr2_v17b0_s_&
    mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
        min_beam_application_time_joint_tci_v17b0_c_::set_fr2_v17b0()
{
  set(types::fr2_v17b0);
  return c.get<fr2_v17b0_s_>();
}
void mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::fr1_v17b0:
      j.write_fieldname("fr1-v17b0");
      j.start_obj();
      if (c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present) {
        j.write_str("scs-15kHz-v17b0", c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0.to_string());
      }
      if (c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present) {
        j.write_str("scs-30kHz-v17b0", c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0.to_string());
      }
      if (c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present) {
        j.write_str("scs-60kHz-v17b0", c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0.to_string());
      }
      j.end_obj();
      break;
    case types::fr2_v17b0:
      j.write_fieldname("fr2-v17b0");
      j.start_obj();
      if (c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present) {
        j.write_str("scs-60kHz-v17b0", c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0.to_string());
      }
      if (c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present) {
        j.write_str("scs-120kHz-v17b0", c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0.to_string());
      }
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_joint_tci_v17b0_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::fr1_v17b0:
      HANDLE_CODE(pack_bits(bref,
                            {c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present,
                             c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present,
                             c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present}));
      if (c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0.pack(bref));
      }
      if (c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0.pack(bref));
      }
      if (c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0.pack(bref));
      }
      break;
    case types::fr2_v17b0:
      HANDLE_CODE(pack_bits(
          bref, {c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present, c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present}));
      if (c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0.pack(bref));
      }
      if (c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0.pack(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_joint_tci_v17b0_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::fr1_v17b0:
      HANDLE_CODE(unpack_bits(bref,
                              {&c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present,
                               &c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present,
                               &c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present}));
      if (c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0.unpack(bref));
      }
      if (c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0.unpack(bref));
      }
      if (c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0.unpack(bref));
      }
      break;
    case types::fr2_v17b0:
      HANDLE_CODE(unpack_bits(
          bref, {&c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present, &c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present}));
      if (c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0.unpack(bref));
      }
      if (c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0.unpack(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_joint_tci_v17b0_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_15k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1", "sym2", "sym4", "sym7", "sym14", "sym28", "sym42", "sym56", "sym70"};
  return convert_enum_idx(names,
                          9,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                          "time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_15k_hz_v17b0_e_");
}
uint8_t mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_15k_hz_v17b0_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70};
  return map_enum_number(numbers,
                         9,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                         "time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_15k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_30k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1", "sym2", "sym4", "sym7", "sym14", "sym28", "sym42", "sym56", "sym70"};
  return convert_enum_idx(names,
                          9,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                          "time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_30k_hz_v17b0_e_");
}
uint8_t mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_30k_hz_v17b0_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70};
  return map_enum_number(numbers,
                         9,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                         "time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_30k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_60k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1", "sym2", "sym4", "sym7", "sym14", "sym28", "sym42", "sym56", "sym70"};
  return convert_enum_idx(names,
                          9,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                          "time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_60k_hz_v17b0_e_");
}
uint8_t mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_60k_hz_v17b0_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70};
  return map_enum_number(numbers,
                         9,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                         "time_joint_tci_v17b0_c_::fr1_v17b0_s_::scs_60k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr2_v17b0_s_::scs_60k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1",
                                "sym2",
                                "sym4",
                                "sym7",
                                "sym14",
                                "sym28",
                                "sym42",
                                "sym56",
                                "sym70",
                                "sym84",
                                "sym98",
                                "sym112",
                                "sym224",
                                "sym336"};
  return convert_enum_idx(names,
                          14,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                          "time_joint_tci_v17b0_c_::fr2_v17b0_s_::scs_60k_hz_v17b0_e_");
}
uint16_t mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr2_v17b0_s_::scs_60k_hz_v17b0_opts::to_number() const
{
  static const uint16_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70, 84, 98, 112, 224, 336};
  return map_enum_number(numbers,
                         14,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                         "time_joint_tci_v17b0_c_::fr2_v17b0_s_::scs_60k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr2_v17b0_s_::scs_120k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1",
                                "sym2",
                                "sym4",
                                "sym7",
                                "sym14",
                                "sym28",
                                "sym42",
                                "sym56",
                                "sym70",
                                "sym84",
                                "sym98",
                                "sym112",
                                "sym224",
                                "sym336"};
  return convert_enum_idx(names,
                          14,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                          "time_joint_tci_v17b0_c_::fr2_v17b0_s_::scs_120k_hz_v17b0_e_");
}
uint16_t mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::fr2_v17b0_s_::scs_120k_hz_v17b0_opts::to_number() const
{
  static const uint16_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70, 84, 98, 112, 224, 336};
  return map_enum_number(numbers,
                         14,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                         "time_joint_tci_v17b0_c_::fr2_v17b0_s_::scs_120k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::types_opts::to_string() const
{
  static const char* names[] = {"fr1-v17b0", "fr2-v17b0"};
  return convert_enum_idx(names,
                          2,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                          "time_joint_tci_v17b0_c_::types");
}
uint8_t mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_joint_tci_v17b0_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers,
                         2,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::min_beam_application_"
                         "time_joint_tci_v17b0_c_::types");
}

const char*
mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::max_num_mac_ce_per_cc_v17b0_opts::to_string()
    const
{
  static const char* names[] = {"n2", "n3", "n4", "n5", "n6", "n7", "n8"};
  return convert_enum_idx(
      names,
      7,
      value,
      "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::max_num_mac_ce_per_cc_v17b0_e_");
}
uint8_t
mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::max_num_mac_ce_per_cc_v17b0_opts::to_number()
    const
{
  static const uint8_t numbers[] = {2, 3, 4, 5, 6, 7, 8};
  return map_enum_number(
      numbers,
      7,
      value,
      "mimo_params_per_band_v17b0_s::unified_joint_tci_multi_mac_ce_v17b0_s_::max_num_mac_ce_per_cc_v17b0_e_");
}

void mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::destroy_()
{
  switch (type_) {
    case types::fr1_v17b0:
      c.destroy<fr1_v17b0_s_>();
      break;
    case types::fr2_v17b0:
      c.destroy<fr2_v17b0_s_>();
      break;
    default:
      break;
  }
}
void mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::fr1_v17b0:
      c.init<fr1_v17b0_s_>();
      break;
    case types::fr2_v17b0:
      c.init<fr2_v17b0_s_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_separate_tci_v17b0_c_");
  }
}
mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::min_beam_application_time_separate_tci_v17b0_c_(
        const mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
            min_beam_application_time_separate_tci_v17b0_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::fr1_v17b0:
      c.init(other.c.get<fr1_v17b0_s_>());
      break;
    case types::fr2_v17b0:
      c.init(other.c.get<fr2_v17b0_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_separate_tci_v17b0_c_");
  }
}
mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_&
    mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
        min_beam_application_time_separate_tci_v17b0_c_::operator=(
            const mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
                min_beam_application_time_separate_tci_v17b0_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::fr1_v17b0:
      c.set(other.c.get<fr1_v17b0_s_>());
      break;
    case types::fr2_v17b0:
      c.set(other.c.get<fr2_v17b0_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_separate_tci_v17b0_c_");
  }

  return *this;
}
mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr1_v17b0_s_&
    mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
        min_beam_application_time_separate_tci_v17b0_c_::set_fr1_v17b0()
{
  set(types::fr1_v17b0);
  return c.get<fr1_v17b0_s_>();
}
mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr2_v17b0_s_&
    mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
        min_beam_application_time_separate_tci_v17b0_c_::set_fr2_v17b0()
{
  set(types::fr2_v17b0);
  return c.get<fr2_v17b0_s_>();
}
void mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::fr1_v17b0:
      j.write_fieldname("fr1-v17b0");
      j.start_obj();
      if (c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present) {
        j.write_str("scs-15kHz-v17b0", c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0.to_string());
      }
      if (c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present) {
        j.write_str("scs-30kHz-v17b0", c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0.to_string());
      }
      if (c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present) {
        j.write_str("scs-60kHz-v17b0", c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0.to_string());
      }
      j.end_obj();
      break;
    case types::fr2_v17b0:
      j.write_fieldname("fr2-v17b0");
      j.start_obj();
      if (c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present) {
        j.write_str("scs-60kHz-v17b0", c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0.to_string());
      }
      if (c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present) {
        j.write_str("scs-120kHz-v17b0", c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0.to_string());
      }
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_separate_tci_v17b0_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::fr1_v17b0:
      HANDLE_CODE(pack_bits(bref,
                            {c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present,
                             c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present,
                             c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present}));
      if (c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0.pack(bref));
      }
      if (c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0.pack(bref));
      }
      if (c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0.pack(bref));
      }
      break;
    case types::fr2_v17b0:
      HANDLE_CODE(pack_bits(
          bref, {c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present, c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present}));
      if (c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0.pack(bref));
      }
      if (c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0.pack(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_separate_tci_v17b0_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::fr1_v17b0:
      HANDLE_CODE(unpack_bits(bref,
                              {&c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present,
                               &c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present,
                               &c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present}));
      if (c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_15k_hz_v17b0.unpack(bref));
      }
      if (c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_30k_hz_v17b0.unpack(bref));
      }
      if (c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr1_v17b0_s_>().scs_60k_hz_v17b0.unpack(bref));
      }
      break;
    case types::fr2_v17b0:
      HANDLE_CODE(unpack_bits(
          bref, {&c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present, &c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present}));
      if (c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr2_v17b0_s_>().scs_60k_hz_v17b0.unpack(bref));
      }
      if (c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0_present) {
        HANDLE_CODE(c.get<fr2_v17b0_s_>().scs_120k_hz_v17b0.unpack(bref));
      }
      break;
    default:
      log_invalid_choice_id(type_,
                            "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                            "application_time_separate_tci_v17b0_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_15k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1", "sym2", "sym4", "sym7", "sym14", "sym28", "sym42", "sym56", "sym70"};
  return convert_enum_idx(names,
                          9,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                          "application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_15k_hz_v17b0_e_");
}
uint8_t mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_15k_hz_v17b0_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70};
  return map_enum_number(numbers,
                         9,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                         "application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_15k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_30k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1", "sym2", "sym4", "sym7", "sym14", "sym28", "sym42", "sym56", "sym70"};
  return convert_enum_idx(names,
                          9,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                          "application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_30k_hz_v17b0_e_");
}
uint8_t mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_30k_hz_v17b0_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70};
  return map_enum_number(numbers,
                         9,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                         "application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_30k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_60k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1", "sym2", "sym4", "sym7", "sym14", "sym28", "sym42", "sym56", "sym70"};
  return convert_enum_idx(names,
                          9,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                          "application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_60k_hz_v17b0_e_");
}
uint8_t mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_60k_hz_v17b0_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70};
  return map_enum_number(numbers,
                         9,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                         "application_time_separate_tci_v17b0_c_::fr1_v17b0_s_::scs_60k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr2_v17b0_s_::scs_60k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1",
                                "sym2",
                                "sym4",
                                "sym7",
                                "sym14",
                                "sym28",
                                "sym42",
                                "sym56",
                                "sym70",
                                "sym84",
                                "sym98",
                                "sym112",
                                "sym224",
                                "sym336"};
  return convert_enum_idx(names,
                          14,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                          "application_time_separate_tci_v17b0_c_::fr2_v17b0_s_::scs_60k_hz_v17b0_e_");
}
uint16_t mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr2_v17b0_s_::scs_60k_hz_v17b0_opts::to_number() const
{
  static const uint16_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70, 84, 98, 112, 224, 336};
  return map_enum_number(numbers,
                         14,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                         "application_time_separate_tci_v17b0_c_::fr2_v17b0_s_::scs_60k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr2_v17b0_s_::scs_120k_hz_v17b0_opts::to_string() const
{
  static const char* names[] = {"sym1",
                                "sym2",
                                "sym4",
                                "sym7",
                                "sym14",
                                "sym28",
                                "sym42",
                                "sym56",
                                "sym70",
                                "sym84",
                                "sym98",
                                "sym112",
                                "sym224",
                                "sym336"};
  return convert_enum_idx(names,
                          14,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                          "application_time_separate_tci_v17b0_c_::fr2_v17b0_s_::scs_120k_hz_v17b0_e_");
}
uint16_t mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::fr2_v17b0_s_::scs_120k_hz_v17b0_opts::to_number() const
{
  static const uint16_t numbers[] = {1, 2, 4, 7, 14, 28, 42, 56, 70, 84, 98, 112, 224, 336};
  return map_enum_number(numbers,
                         14,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                         "application_time_separate_tci_v17b0_c_::fr2_v17b0_s_::scs_120k_hz_v17b0_e_");
}

const char* mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::types_opts::to_string() const
{
  static const char* names[] = {"fr1-v17b0", "fr2-v17b0"};
  return convert_enum_idx(names,
                          2,
                          value,
                          "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                          "application_time_separate_tci_v17b0_c_::types");
}
uint8_t mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::
    min_beam_application_time_separate_tci_v17b0_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers,
                         2,
                         value,
                         "mimo_params_per_band_v17b0_s::unified_separate_tci_multi_mac_ce_v17b0_s_::min_beam_"
                         "application_time_separate_tci_v17b0_c_::types");
}

// BandNR-v17b0 ::= SEQUENCE
OCUDUASN_CODE band_nr_v17b0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, mimo_params_per_band_v17b0_present}));

  if (mimo_params_per_band_v17b0_present) {
    HANDLE_CODE(mimo_params_per_band_v17b0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE band_nr_v17b0_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &mimo_params_per_band_v17b0_present}));

  if (mimo_params_per_band_v17b0_present) {
    HANDLE_CODE(mimo_params_per_band_v17b0.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void band_nr_v17b0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mimo_params_per_band_v17b0_present) {
    j.write_fieldname("mimo-ParametersPerBand-v17b0");
    mimo_params_per_band_v17b0.to_json(j);
  }
  j.end_obj();
}

// CG-SDT-ConfigLCH-Restriction-r17 ::= SEQUENCE
OCUDUASN_CODE cg_sdt_cfg_lch_restrict_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {cfg_grant_type1_allowed_r17_present, allowed_cg_list_r17_present}));

  HANDLE_CODE(pack_integer(bref, lc_ch_id_r17, (uint8_t)1u, (uint8_t)32u));
  if (allowed_cg_list_r17_present) {
    HANDLE_CODE(pack_dyn_seq_of(bref, allowed_cg_list_r17, 0, 31, integer_packer<uint8_t>(0, 31)));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_sdt_cfg_lch_restrict_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&cfg_grant_type1_allowed_r17_present, &allowed_cg_list_r17_present}));

  HANDLE_CODE(unpack_integer(lc_ch_id_r17, bref, (uint8_t)1u, (uint8_t)32u));
  if (allowed_cg_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(allowed_cg_list_r17, bref, 0, 31, integer_packer<uint8_t>(0, 31)));
  }

  return OCUDUASN_SUCCESS;
}
void cg_sdt_cfg_lch_restrict_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("logicalChannelIdentity-r17", lc_ch_id_r17);
  if (cfg_grant_type1_allowed_r17_present) {
    j.write_str("configuredGrantType1Allowed-r17", "true");
  }
  if (allowed_cg_list_r17_present) {
    j.start_array("allowedCG-List-r17");
    for (const auto& e1 : allowed_cg_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  j.end_obj();
}

// CG-SDT-ConfigLCH-RestrictionExt-v1800 ::= SEQUENCE
OCUDUASN_CODE cg_sdt_cfg_lch_restrict_ext_v1800_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(cg_sdt_max_dur_to_next_cg_occasion_r18_present, 1));

  if (cg_sdt_max_dur_to_next_cg_occasion_r18_present) {
    HANDLE_CODE(cg_sdt_max_dur_to_next_cg_occasion_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_sdt_cfg_lch_restrict_ext_v1800_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(cg_sdt_max_dur_to_next_cg_occasion_r18_present, 1));

  if (cg_sdt_max_dur_to_next_cg_occasion_r18_present) {
    HANDLE_CODE(cg_sdt_max_dur_to_next_cg_occasion_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cg_sdt_cfg_lch_restrict_ext_v1800_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (cg_sdt_max_dur_to_next_cg_occasion_r18_present) {
    j.write_str("cg-SDT-MaxDurationToNextCG-Occasion-r18", cg_sdt_max_dur_to_next_cg_occasion_r18.to_string());
  }
  j.end_obj();
}

const char* cg_sdt_cfg_lch_restrict_ext_v1800_s::cg_sdt_max_dur_to_next_cg_occasion_r18_opts::to_string() const
{
  static const char* names[] = {"ms10",
                                "ms100",
                                "sec1",
                                "sec10",
                                "sec60",
                                "sec100",
                                "sec300",
                                "sec600",
                                "sec1200",
                                "sec1800",
                                "sec3600",
                                "spare5",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(
      names, 16, value, "cg_sdt_cfg_lch_restrict_ext_v1800_s::cg_sdt_max_dur_to_next_cg_occasion_r18_e_");
}

// CG-SDT-TA-ValidationConfig-r17 ::= SEQUENCE
OCUDUASN_CODE cg_sdt_ta_validation_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(cg_sdt_rsrp_change_thres_r17.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cg_sdt_ta_validation_cfg_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(cg_sdt_rsrp_change_thres_r17.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void cg_sdt_ta_validation_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("cg-SDT-RSRP-ChangeThreshold-r17", cg_sdt_rsrp_change_thres_r17.to_string());
  j.end_obj();
}

const char* cg_sdt_ta_validation_cfg_r17_s::cg_sdt_rsrp_change_thres_r17_opts::to_string() const
{
  static const char* names[] = {"dB2",
                                "dB4",
                                "dB6",
                                "dB8",
                                "dB10",
                                "dB14",
                                "dB18",
                                "dB22",
                                "dB26",
                                "dB30",
                                "dB34",
                                "spare5",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(names, 16, value, "cg_sdt_ta_validation_cfg_r17_s::cg_sdt_rsrp_change_thres_r17_e_");
}
uint8_t cg_sdt_ta_validation_cfg_r17_s::cg_sdt_rsrp_change_thres_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 6, 8, 10, 14, 18, 22, 26, 30, 34};
  return map_enum_number(numbers, 11, value, "cg_sdt_ta_validation_cfg_r17_s::cg_sdt_rsrp_change_thres_r17_e_");
}

// CellGrouping-r16 ::= SEQUENCE
OCUDUASN_CODE cell_grouping_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_dyn_seq_of(bref, mcg_r16, 1, 1024, integer_packer<uint16_t>(1, 1024)));
  HANDLE_CODE(pack_dyn_seq_of(bref, scg_r16, 1, 1024, integer_packer<uint16_t>(1, 1024)));
  HANDLE_CODE(mode_r16.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_grouping_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_dyn_seq_of(mcg_r16, bref, 1, 1024, integer_packer<uint16_t>(1, 1024)));
  HANDLE_CODE(unpack_dyn_seq_of(scg_r16, bref, 1, 1024, integer_packer<uint16_t>(1, 1024)));
  HANDLE_CODE(mode_r16.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void cell_grouping_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("mcg-r16");
  for (const auto& e1 : mcg_r16) {
    j.write_int(e1);
  }
  j.end_array();
  j.start_array("scg-r16");
  for (const auto& e1 : scg_r16) {
    j.write_int(e1);
  }
  j.end_array();
  j.write_str("mode-r16", mode_r16.to_string());
  j.end_obj();
}

const char* cell_grouping_r16_s::mode_r16_opts::to_string() const
{
  static const char* names[] = {"sync", "async"};
  return convert_enum_idx(names, 2, value, "cell_grouping_r16_s::mode_r16_e_");
}

// EarlyUL-SyncConfig-r18 ::= SEQUENCE
OCUDUASN_CODE early_ul_sync_cfg_r18_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= ltm_tdd_ul_dl_cfg_common_r18.is_present();
  group_flags[0] |= ltm_restricted_set_cfg_r18_present;
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         ssb_per_rach_occasion_r18_present,
                         prach_root_seq_idx_r18_present,
                         ltm_prach_subcarrier_spacing_r18_present,
                         n_timing_advance_offset_r18_present}));

  HANDLE_CODE(freq_info_ul_r18.pack(bref));
  HANDLE_CODE(rach_cfg_generic_r18.pack(bref));
  HANDLE_CODE(bwp_generic_params_r18.pack(bref));
  if (ssb_per_rach_occasion_r18_present) {
    HANDLE_CODE(ssb_per_rach_occasion_r18.pack(bref));
  }
  if (prach_root_seq_idx_r18_present) {
    HANDLE_CODE(prach_root_seq_idx_r18.pack(bref));
  }
  if (ltm_prach_subcarrier_spacing_r18_present) {
    HANDLE_CODE(ltm_prach_subcarrier_spacing_r18.pack(bref));
  }
  if (n_timing_advance_offset_r18_present) {
    HANDLE_CODE(n_timing_advance_offset_r18.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {ltm_tdd_ul_dl_cfg_common_r18.is_present(), ltm_restricted_set_cfg_r18_present}));
      if (ltm_tdd_ul_dl_cfg_common_r18.is_present()) {
        HANDLE_CODE(ltm_tdd_ul_dl_cfg_common_r18->pack(bref));
      }
      if (ltm_restricted_set_cfg_r18_present) {
        HANDLE_CODE(ltm_restricted_set_cfg_r18.pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE early_ul_sync_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &ssb_per_rach_occasion_r18_present,
                           &prach_root_seq_idx_r18_present,
                           &ltm_prach_subcarrier_spacing_r18_present,
                           &n_timing_advance_offset_r18_present}));

  HANDLE_CODE(freq_info_ul_r18.unpack(bref));
  HANDLE_CODE(rach_cfg_generic_r18.unpack(bref));
  HANDLE_CODE(bwp_generic_params_r18.unpack(bref));
  if (ssb_per_rach_occasion_r18_present) {
    HANDLE_CODE(ssb_per_rach_occasion_r18.unpack(bref));
  }
  if (prach_root_seq_idx_r18_present) {
    HANDLE_CODE(prach_root_seq_idx_r18.unpack(bref));
  }
  if (ltm_prach_subcarrier_spacing_r18_present) {
    HANDLE_CODE(ltm_prach_subcarrier_spacing_r18.unpack(bref));
  }
  if (n_timing_advance_offset_r18_present) {
    HANDLE_CODE(n_timing_advance_offset_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(ltm_tdd_ul_dl_cfg_common_r18, bref);
      HANDLE_CODE(bref.unpack(ltm_restricted_set_cfg_r18_present, 1));
      if (ltm_tdd_ul_dl_cfg_common_r18.is_present()) {
        HANDLE_CODE(ltm_tdd_ul_dl_cfg_common_r18->unpack(bref));
      }
      if (ltm_restricted_set_cfg_r18_present) {
        HANDLE_CODE(ltm_restricted_set_cfg_r18.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void early_ul_sync_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("frequencyInfoUL-r18");
  freq_info_ul_r18.to_json(j);
  j.write_fieldname("rach-ConfigGeneric-r18");
  rach_cfg_generic_r18.to_json(j);
  j.write_fieldname("bwp-GenericParameters-r18");
  bwp_generic_params_r18.to_json(j);
  if (ssb_per_rach_occasion_r18_present) {
    j.write_str("ssb-PerRACH-Occasion-r18", ssb_per_rach_occasion_r18.to_string());
  }
  if (prach_root_seq_idx_r18_present) {
    j.write_fieldname("prach-RootSequenceIndex-r18");
    prach_root_seq_idx_r18.to_json(j);
  }
  if (ltm_prach_subcarrier_spacing_r18_present) {
    j.write_str("ltm-PRACH-SubcarrierSpacing-r18", ltm_prach_subcarrier_spacing_r18.to_string());
  }
  if (n_timing_advance_offset_r18_present) {
    j.write_str("n-TimingAdvanceOffset-r18", n_timing_advance_offset_r18.to_string());
  }
  if (ltm_tdd_ul_dl_cfg_common_r18.is_present()) {
    j.write_fieldname("ltm-tdd-UL-DL-ConfigurationCommon-r18");
    ltm_tdd_ul_dl_cfg_common_r18->to_json(j);
  }
  if (ltm_restricted_set_cfg_r18_present) {
    j.write_str("ltm-restrictedSetConfig-r18", ltm_restricted_set_cfg_r18.to_string());
  }
  j.end_obj();
}

const char* early_ul_sync_cfg_r18_s::ssb_per_rach_occasion_r18_opts::to_string() const
{
  static const char* names[] = {"oneEighth", "oneFourth", "oneHalf", "one", "two", "four", "eight", "sixteen"};
  return convert_enum_idx(names, 8, value, "early_ul_sync_cfg_r18_s::ssb_per_rach_occasion_r18_e_");
}
float early_ul_sync_cfg_r18_s::ssb_per_rach_occasion_r18_opts::to_number() const
{
  static const float numbers[] = {0.125, 0.25, 0.5, 1.0, 2.0, 4.0, 8.0, 6.0};
  return map_enum_number(numbers, 8, value, "early_ul_sync_cfg_r18_s::ssb_per_rach_occasion_r18_e_");
}
const char* early_ul_sync_cfg_r18_s::ssb_per_rach_occasion_r18_opts::to_number_string() const
{
  static const char* number_strs[] = {"1/8", "1/4", "1/2", "1", "2", "4", "8", "6"};
  return convert_enum_idx(number_strs, 8, value, "early_ul_sync_cfg_r18_s::ssb_per_rach_occasion_r18_e_");
}

void early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::destroy_() {}
void early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::prach_root_seq_idx_r18_c_(
    const early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::l839:
      c.init(other.c.get<uint16_t>());
      break;
    case types::l139:
      c.init(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_");
  }
}
early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_& early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::operator=(
    const early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::l839:
      c.set(other.c.get<uint16_t>());
      break;
    case types::l139:
      c.set(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_");
  }

  return *this;
}
uint16_t& early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::set_l839()
{
  set(types::l839);
  return c.get<uint16_t>();
}
uint8_t& early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::set_l139()
{
  set(types::l139);
  return c.get<uint8_t>();
}
void early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::l839:
      j.write_int("l839", c.get<uint16_t>());
      break;
    case types::l139:
      j.write_int("l139", c.get<uint8_t>());
      break;
    default:
      log_invalid_choice_id(type_, "early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::l839:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)837u));
      break;
    case types::l139:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)137u));
      break;
    default:
      log_invalid_choice_id(type_, "early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::l839:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)837u));
      break;
    case types::l139:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)137u));
      break;
    default:
      log_invalid_choice_id(type_, "early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"l839", "l139"};
  return convert_enum_idx(names, 2, value, "early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::types");
}
uint16_t early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::types_opts::to_number() const
{
  static const uint16_t numbers[] = {839, 139};
  return map_enum_number(numbers, 2, value, "early_ul_sync_cfg_r18_s::prach_root_seq_idx_r18_c_::types");
}

const char* early_ul_sync_cfg_r18_s::n_timing_advance_offset_r18_opts::to_string() const
{
  static const char* names[] = {"n0", "n25600", "n39936", "spare1"};
  return convert_enum_idx(names, 4, value, "early_ul_sync_cfg_r18_s::n_timing_advance_offset_r18_e_");
}
uint16_t early_ul_sync_cfg_r18_s::n_timing_advance_offset_r18_opts::to_number() const
{
  static const uint16_t numbers[] = {0, 25600, 39936};
  return map_enum_number(numbers, 3, value, "early_ul_sync_cfg_r18_s::n_timing_advance_offset_r18_e_");
}

const char* early_ul_sync_cfg_r18_s::ltm_restricted_set_cfg_r18_opts::to_string() const
{
  static const char* names[] = {"unrestrictedSet", "restrictedSetTypeA", "restrictedSetTypeB"};
  return convert_enum_idx(names, 3, value, "early_ul_sync_cfg_r18_s::ltm_restricted_set_cfg_r18_e_");
}

// FeatureSetDownlink-v15t0 ::= SEQUENCE
OCUDUASN_CODE feature_set_dl_v15t0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(zero_slot_offset_aperiodic_srs_present, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE feature_set_dl_v15t0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(zero_slot_offset_aperiodic_srs_present, 1));

  return OCUDUASN_SUCCESS;
}
void feature_set_dl_v15t0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (zero_slot_offset_aperiodic_srs_present) {
    j.write_str("zeroSlotOffsetAperiodicSRS", "supported");
  }
  j.end_obj();
}

// FeatureSetDownlink-v16k0 ::= SEQUENCE
OCUDUASN_CODE feature_set_dl_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {offset_srs_cb_pusch_ant_switch_fr1_r16_present,
                         offset_srs_cb_pusch_pdcch_monitor_single_occ_fr1_r16_present,
                         offset_srs_cb_pusch_pdcch_monitor_any_occ_without_gap_fr1_r16_present,
                         offset_srs_cb_pusch_pdcch_monitor_any_occ_with_gap_fr1_r16_present,
                         offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_present}));

  if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_present) {
    HANDLE_CODE(pack_bits(bref,
                          {offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_15k_hz_r16_present,
                           offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_30k_hz_r16_present,
                           offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_60k_hz_r16_present}));
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_15k_hz_r16_present) {
      HANDLE_CODE(offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_15k_hz_r16.pack(bref));
    }
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_30k_hz_r16_present) {
      HANDLE_CODE(offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_30k_hz_r16.pack(bref));
    }
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_60k_hz_r16_present) {
      HANDLE_CODE(offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_60k_hz_r16.pack(bref));
    }
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE feature_set_dl_v16k0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&offset_srs_cb_pusch_ant_switch_fr1_r16_present,
                           &offset_srs_cb_pusch_pdcch_monitor_single_occ_fr1_r16_present,
                           &offset_srs_cb_pusch_pdcch_monitor_any_occ_without_gap_fr1_r16_present,
                           &offset_srs_cb_pusch_pdcch_monitor_any_occ_with_gap_fr1_r16_present,
                           &offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_present}));

  if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_present) {
    HANDLE_CODE(unpack_bits(bref,
                            {&offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_15k_hz_r16_present,
                             &offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_30k_hz_r16_present,
                             &offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_60k_hz_r16_present}));
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_15k_hz_r16_present) {
      HANDLE_CODE(offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_15k_hz_r16.unpack(bref));
    }
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_30k_hz_r16_present) {
      HANDLE_CODE(offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_30k_hz_r16.unpack(bref));
    }
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_60k_hz_r16_present) {
      HANDLE_CODE(offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_60k_hz_r16.unpack(bref));
    }
  }

  return OCUDUASN_SUCCESS;
}
void feature_set_dl_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (offset_srs_cb_pusch_ant_switch_fr1_r16_present) {
    j.write_str("offsetSRS-CB-PUSCH-Ant-Switch-fr1-r16", "supported");
  }
  if (offset_srs_cb_pusch_pdcch_monitor_single_occ_fr1_r16_present) {
    j.write_str("offsetSRS-CB-PUSCH-PDCCH-MonitorSingleOcc-fr1-r16", "supported");
  }
  if (offset_srs_cb_pusch_pdcch_monitor_any_occ_without_gap_fr1_r16_present) {
    j.write_str("offsetSRS-CB-PUSCH-PDCCH-MonitorAnyOccWithoutGap-fr1-r16", "supported");
  }
  if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_gap_fr1_r16_present) {
    j.write_str("offsetSRS-CB-PUSCH-PDCCH-MonitorAnyOccWithGap-fr1-r16", "supported");
  }
  if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_present) {
    j.write_fieldname("offsetSRS-CB-PUSCH-PDCCH-MonitorAnyOccWithSpanGap-fr1-r16");
    j.start_obj();
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_15k_hz_r16_present) {
      j.write_str("scs-15kHz-r16",
                  offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_15k_hz_r16.to_string());
    }
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_30k_hz_r16_present) {
      j.write_str("scs-30kHz-r16",
                  offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_30k_hz_r16.to_string());
    }
    if (offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_60k_hz_r16_present) {
      j.write_str("scs-60kHz-r16",
                  offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16.scs_60k_hz_r16.to_string());
    }
    j.end_obj();
  }
  j.end_obj();
}

const char* feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::
    scs_15k_hz_r16_opts::to_string() const
{
  static const char* names[] = {"set1", "set2", "set3"};
  return convert_enum_idx(
      names,
      3,
      value,
      "feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::scs_15k_hz_r16_e_");
}
uint8_t feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::
    scs_15k_hz_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3};
  return map_enum_number(
      numbers,
      3,
      value,
      "feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::scs_15k_hz_r16_e_");
}

const char* feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::
    scs_30k_hz_r16_opts::to_string() const
{
  static const char* names[] = {"set1", "set2", "set3"};
  return convert_enum_idx(
      names,
      3,
      value,
      "feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::scs_30k_hz_r16_e_");
}
uint8_t feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::
    scs_30k_hz_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3};
  return map_enum_number(
      numbers,
      3,
      value,
      "feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::scs_30k_hz_r16_e_");
}

const char* feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::
    scs_60k_hz_r16_opts::to_string() const
{
  static const char* names[] = {"set1", "set2", "set3"};
  return convert_enum_idx(
      names,
      3,
      value,
      "feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::scs_60k_hz_r16_e_");
}
uint8_t feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::
    scs_60k_hz_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3};
  return map_enum_number(
      numbers,
      3,
      value,
      "feature_set_dl_v16k0_s::offset_srs_cb_pusch_pdcch_monitor_any_occ_with_span_gap_fr1_r16_s_::scs_60k_hz_r16_e_");
}

// FeatureSetDownlink-v17d0 ::= SEQUENCE
OCUDUASN_CODE feature_set_dl_v17d0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref,
      {srs_ant_switching2_sp_1_periodic_r17_present, srs_ext_aperiodic_srs_r17_present, srs_one_ap_srs_r17_present}));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE feature_set_dl_v17d0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&srs_ant_switching2_sp_1_periodic_r17_present,
                           &srs_ext_aperiodic_srs_r17_present,
                           &srs_one_ap_srs_r17_present}));

  return OCUDUASN_SUCCESS;
}
void feature_set_dl_v17d0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_ant_switching2_sp_1_periodic_r17_present) {
    j.write_str("srs-AntennaSwitching2SP-1Periodic-r17", "supported");
  }
  if (srs_ext_aperiodic_srs_r17_present) {
    j.write_str("srs-ExtensionAperiodicSRS-r17", "supported");
  }
  if (srs_one_ap_srs_r17_present) {
    j.write_str("srs-OneAP-SRS-r17", "supported");
  }
  j.end_obj();
}

// FeatureSetUplink-v16d0 ::= SEQUENCE
OCUDUASN_CODE feature_set_ul_v16d0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(pusch_repeat_type_b_v16d0_present, 1));

  if (pusch_repeat_type_b_v16d0_present) {
    HANDLE_CODE(pusch_repeat_type_b_v16d0.max_num_pusch_tx_cap1_r16.pack(bref));
    HANDLE_CODE(pusch_repeat_type_b_v16d0.max_num_pusch_tx_cap2_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE feature_set_ul_v16d0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(pusch_repeat_type_b_v16d0_present, 1));

  if (pusch_repeat_type_b_v16d0_present) {
    HANDLE_CODE(pusch_repeat_type_b_v16d0.max_num_pusch_tx_cap1_r16.unpack(bref));
    HANDLE_CODE(pusch_repeat_type_b_v16d0.max_num_pusch_tx_cap2_r16.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void feature_set_ul_v16d0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pusch_repeat_type_b_v16d0_present) {
    j.write_fieldname("pusch-RepetitionTypeB-v16d0");
    j.start_obj();
    j.write_str("maxNumberPUSCH-Tx-Cap1-r16", pusch_repeat_type_b_v16d0.max_num_pusch_tx_cap1_r16.to_string());
    j.write_str("maxNumberPUSCH-Tx-Cap2-r16", pusch_repeat_type_b_v16d0.max_num_pusch_tx_cap2_r16.to_string());
    j.end_obj();
  }
  j.end_obj();
}

const char* feature_set_ul_v16d0_s::pusch_repeat_type_b_v16d0_s_::max_num_pusch_tx_cap1_r16_opts::to_string() const
{
  static const char* names[] = {"n2", "n3", "n4", "n7", "n8", "n12"};
  return convert_enum_idx(
      names, 6, value, "feature_set_ul_v16d0_s::pusch_repeat_type_b_v16d0_s_::max_num_pusch_tx_cap1_r16_e_");
}
uint8_t feature_set_ul_v16d0_s::pusch_repeat_type_b_v16d0_s_::max_num_pusch_tx_cap1_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 3, 4, 7, 8, 12};
  return map_enum_number(
      numbers, 6, value, "feature_set_ul_v16d0_s::pusch_repeat_type_b_v16d0_s_::max_num_pusch_tx_cap1_r16_e_");
}

const char* feature_set_ul_v16d0_s::pusch_repeat_type_b_v16d0_s_::max_num_pusch_tx_cap2_r16_opts::to_string() const
{
  static const char* names[] = {"n2", "n3", "n4", "n7", "n8", "n12"};
  return convert_enum_idx(
      names, 6, value, "feature_set_ul_v16d0_s::pusch_repeat_type_b_v16d0_s_::max_num_pusch_tx_cap2_r16_e_");
}
uint8_t feature_set_ul_v16d0_s::pusch_repeat_type_b_v16d0_s_::max_num_pusch_tx_cap2_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 3, 4, 7, 8, 12};
  return map_enum_number(
      numbers, 6, value, "feature_set_ul_v16d0_s::pusch_repeat_type_b_v16d0_s_::max_num_pusch_tx_cap2_r16_e_");
}

// FeatureSets-v15t0 ::= SEQUENCE
OCUDUASN_CODE feature_sets_v15t0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(feature_sets_dl_v15t0.size() > 0, 1));

  if (feature_sets_dl_v15t0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, feature_sets_dl_v15t0, 1, 1024));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE feature_sets_v15t0_s::unpack(cbit_ref& bref)
{
  bool feature_sets_dl_v15t0_present;
  HANDLE_CODE(bref.unpack(feature_sets_dl_v15t0_present, 1));

  if (feature_sets_dl_v15t0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(feature_sets_dl_v15t0, bref, 1, 1024));
  }

  return OCUDUASN_SUCCESS;
}
void feature_sets_v15t0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (feature_sets_dl_v15t0.size() > 0) {
    j.start_array("featureSetsDownlink-v15t0");
    for (const auto& e1 : feature_sets_dl_v15t0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// FeatureSets-v16d0 ::= SEQUENCE
OCUDUASN_CODE feature_sets_v16d0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(feature_sets_ul_v16d0.size() > 0, 1));

  if (feature_sets_ul_v16d0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, feature_sets_ul_v16d0, 1, 1024));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE feature_sets_v16d0_s::unpack(cbit_ref& bref)
{
  bool feature_sets_ul_v16d0_present;
  HANDLE_CODE(bref.unpack(feature_sets_ul_v16d0_present, 1));

  if (feature_sets_ul_v16d0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(feature_sets_ul_v16d0, bref, 1, 1024));
  }

  return OCUDUASN_SUCCESS;
}
void feature_sets_v16d0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (feature_sets_ul_v16d0.size() > 0) {
    j.start_array("featureSetsUplink-v16d0");
    for (const auto& e1 : feature_sets_ul_v16d0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// FeatureSets-v16k0 ::= SEQUENCE
OCUDUASN_CODE feature_sets_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(feature_sets_dl_v16k0.size() > 0, 1));

  if (feature_sets_dl_v16k0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, feature_sets_dl_v16k0, 1, 1024));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE feature_sets_v16k0_s::unpack(cbit_ref& bref)
{
  bool feature_sets_dl_v16k0_present;
  HANDLE_CODE(bref.unpack(feature_sets_dl_v16k0_present, 1));

  if (feature_sets_dl_v16k0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(feature_sets_dl_v16k0, bref, 1, 1024));
  }

  return OCUDUASN_SUCCESS;
}
void feature_sets_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (feature_sets_dl_v16k0.size() > 0) {
    j.start_array("featureSetsDownlink-v16k0");
    for (const auto& e1 : feature_sets_dl_v16k0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// FeatureSets-v17d0 ::= SEQUENCE
OCUDUASN_CODE feature_sets_v17d0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(feature_sets_dl_v17d0.size() > 0, 1));

  if (feature_sets_dl_v17d0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, feature_sets_dl_v17d0, 1, 1024));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE feature_sets_v17d0_s::unpack(cbit_ref& bref)
{
  bool feature_sets_dl_v17d0_present;
  HANDLE_CODE(bref.unpack(feature_sets_dl_v17d0_present, 1));

  if (feature_sets_dl_v17d0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(feature_sets_dl_v17d0, bref, 1, 1024));
  }

  return OCUDUASN_SUCCESS;
}
void feature_sets_v17d0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (feature_sets_dl_v17d0.size() > 0) {
    j.start_array("featureSetsDownlink-v17d0");
    for (const auto& e1 : feature_sets_dl_v17d0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// MAC-Parameters-v17b0 ::= SEQUENCE
OCUDUASN_CODE mac_params_v17b0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(mtrp_pusch_phr_type1_report_r17_present, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mac_params_v17b0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(mtrp_pusch_phr_type1_report_r17_present, 1));

  return OCUDUASN_SUCCESS;
}
void mac_params_v17b0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mtrp_pusch_phr_type1_report_r17_present) {
    j.write_str("mTRP-PUSCH-PHR-Type1-Reporting-r17", "supported");
  }
  j.end_obj();
}

// MAC-Parameters-v17c0 ::= SEQUENCE
OCUDUASN_CODE mac_params_v17c0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(direct_scell_activation_with_tci_r17_present, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mac_params_v17c0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(direct_scell_activation_with_tci_r17_present, 1));

  return OCUDUASN_SUCCESS;
}
void mac_params_v17c0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (direct_scell_activation_with_tci_r17_present) {
    j.write_str("directSCellActivationWithTCI-r17", "supported");
  }
  j.end_obj();
}

// MBS-NeighbourCell-r17 ::= SEQUENCE
OCUDUASN_CODE mbs_neighbour_cell_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(carrier_freq_r17_present, 1));

  HANDLE_CODE(pack_integer(bref, pci_r17, (uint16_t)0u, (uint16_t)1007u));
  if (carrier_freq_r17_present) {
    HANDLE_CODE(pack_integer(bref, carrier_freq_r17, (uint32_t)0u, (uint32_t)3279165u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_neighbour_cell_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(carrier_freq_r17_present, 1));

  HANDLE_CODE(unpack_integer(pci_r17, bref, (uint16_t)0u, (uint16_t)1007u));
  if (carrier_freq_r17_present) {
    HANDLE_CODE(unpack_integer(carrier_freq_r17, bref, (uint32_t)0u, (uint32_t)3279165u));
  }

  return OCUDUASN_SUCCESS;
}
void mbs_neighbour_cell_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("physCellId-r17", pci_r17);
  if (carrier_freq_r17_present) {
    j.write_int("carrierFreq-r17", carrier_freq_r17);
  }
  j.end_obj();
}

// MRB-PDCP-ConfigBroadcast-r17 ::= SEQUENCE
OCUDUASN_CODE mrb_pdcp_cfg_broadcast_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {pdcp_sn_size_dl_r17_present, t_reordering_r17_present}));

  HANDLE_CODE(hdr_compress_r17.pack(bref));
  if (t_reordering_r17_present) {
    HANDLE_CODE(t_reordering_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_pdcp_cfg_broadcast_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&pdcp_sn_size_dl_r17_present, &t_reordering_r17_present}));

  HANDLE_CODE(hdr_compress_r17.unpack(bref));
  if (t_reordering_r17_present) {
    HANDLE_CODE(t_reordering_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void mrb_pdcp_cfg_broadcast_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdcp_sn_size_dl_r17_present) {
    j.write_str("pdcp-SN-SizeDL-r17", "len12bits");
  }
  j.write_fieldname("headerCompression-r17");
  hdr_compress_r17.to_json(j);
  if (t_reordering_r17_present) {
    j.write_str("t-Reordering-r17", t_reordering_r17.to_string());
  }
  j.end_obj();
}

void mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::set(types::options e)
{
  type_ = e;
}
void mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::set_not_used()
{
  set(types::not_used);
}
mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::rohc_s_&
mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::set_rohc()
{
  set(types::rohc);
  return c;
}
void mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::not_used:
      j.write_null("notUsed");
      break;
    case types::rohc:
      j.write_fieldname("rohc");
      j.start_obj();
      if (c.max_c_id_r17_present) {
        j.write_int("maxCID-r17", c.max_c_id_r17);
      }
      j.write_fieldname("profiles-r17");
      j.start_obj();
      j.write_bool("profile0x0000-r17", c.profiles_r17.profile0x0000_r17);
      j.write_bool("profile0x0001-r17", c.profiles_r17.profile0x0001_r17);
      j.write_bool("profile0x0002-r17", c.profiles_r17.profile0x0002_r17);
      j.end_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::not_used:
      break;
    case types::rohc:
      HANDLE_CODE(bref.pack(c.max_c_id_r17_present, 1));
      if (c.max_c_id_r17_present) {
        HANDLE_CODE(pack_integer(bref, c.max_c_id_r17, (uint8_t)1u, (uint8_t)16u));
      }
      HANDLE_CODE(bref.pack(c.profiles_r17.profile0x0000_r17, 1));
      HANDLE_CODE(bref.pack(c.profiles_r17.profile0x0001_r17, 1));
      HANDLE_CODE(bref.pack(c.profiles_r17.profile0x0002_r17, 1));
      break;
    default:
      log_invalid_choice_id(type_, "mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::not_used:
      break;
    case types::rohc:
      HANDLE_CODE(bref.unpack(c.max_c_id_r17_present, 1));
      if (c.max_c_id_r17_present) {
        HANDLE_CODE(unpack_integer(c.max_c_id_r17, bref, (uint8_t)1u, (uint8_t)16u));
      }
      HANDLE_CODE(bref.unpack(c.profiles_r17.profile0x0000_r17, 1));
      HANDLE_CODE(bref.unpack(c.profiles_r17.profile0x0001_r17, 1));
      HANDLE_CODE(bref.unpack(c.profiles_r17.profile0x0002_r17, 1));
      break;
    default:
      log_invalid_choice_id(type_, "mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::types_opts::to_string() const
{
  static const char* names[] = {"notUsed", "rohc"};
  return convert_enum_idx(names, 2, value, "mrb_pdcp_cfg_broadcast_r17_s::hdr_compress_r17_c_::types");
}

const char* mrb_pdcp_cfg_broadcast_r17_s::t_reordering_r17_opts::to_string() const
{
  static const char* names[] = {"ms1", "ms10", "ms40", "ms160", "ms500", "ms1000", "ms1250", "ms2750"};
  return convert_enum_idx(names, 8, value, "mrb_pdcp_cfg_broadcast_r17_s::t_reordering_r17_e_");
}
uint16_t mrb_pdcp_cfg_broadcast_r17_s::t_reordering_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {1, 10, 40, 160, 500, 1000, 1250, 2750};
  return map_enum_number(numbers, 8, value, "mrb_pdcp_cfg_broadcast_r17_s::t_reordering_r17_e_");
}

// MRB-RLC-ConfigBroadcast-r17 ::= SEQUENCE
OCUDUASN_CODE mrb_rlc_cfg_broadcast_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {sn_field_len_r17_present, t_reassembly_r17_present}));

  HANDLE_CODE(pack_integer(bref, lc_ch_id_r17, (uint8_t)1u, (uint8_t)32u));
  if (t_reassembly_r17_present) {
    HANDLE_CODE(t_reassembly_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_rlc_cfg_broadcast_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&sn_field_len_r17_present, &t_reassembly_r17_present}));

  HANDLE_CODE(unpack_integer(lc_ch_id_r17, bref, (uint8_t)1u, (uint8_t)32u));
  if (t_reassembly_r17_present) {
    HANDLE_CODE(t_reassembly_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void mrb_rlc_cfg_broadcast_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("logicalChannelIdentity-r17", lc_ch_id_r17);
  if (sn_field_len_r17_present) {
    j.write_str("sn-FieldLength-r17", "size6");
  }
  if (t_reassembly_r17_present) {
    j.write_str("t-Reassembly-r17", t_reassembly_r17.to_string());
  }
  j.end_obj();
}

// MRB-InfoBroadcast-r17 ::= SEQUENCE
OCUDUASN_CODE mrb_info_broadcast_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pdcp_cfg_r17.pack(bref));
  HANDLE_CODE(rlc_cfg_r17.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_info_broadcast_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(pdcp_cfg_r17.unpack(bref));
  HANDLE_CODE(rlc_cfg_r17.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mrb_info_broadcast_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("pdcp-Config-r17");
  pdcp_cfg_r17.to_json(j);
  j.write_fieldname("rlc-Config-r17");
  rlc_cfg_r17.to_json(j);
  j.end_obj();
}

// MBS-SessionInfo-r17 ::= SEQUENCE
OCUDUASN_CODE mbs_session_info_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {mtch_sched_info_r17_present,
                         mtch_neighbour_cell_r17_present,
                         pdsch_cfg_idx_r17_present,
                         mtch_ssb_map_win_idx_r17_present}));

  HANDLE_CODE(mbs_session_id_r17.pack(bref));
  HANDLE_CODE(pack_integer(bref, g_rnti_r17, (uint32_t)0u, (uint32_t)65535u));
  HANDLE_CODE(pack_dyn_seq_of(bref, mrb_list_broadcast_r17, 1, 4));
  if (mtch_sched_info_r17_present) {
    HANDLE_CODE(pack_integer(bref, mtch_sched_info_r17, (uint8_t)0u, (uint8_t)63u));
  }
  if (mtch_neighbour_cell_r17_present) {
    HANDLE_CODE(mtch_neighbour_cell_r17.pack(bref));
  }
  if (pdsch_cfg_idx_r17_present) {
    HANDLE_CODE(pack_integer(bref, pdsch_cfg_idx_r17, (uint8_t)0u, (uint8_t)15u));
  }
  if (mtch_ssb_map_win_idx_r17_present) {
    HANDLE_CODE(pack_integer(bref, mtch_ssb_map_win_idx_r17, (uint8_t)0u, (uint8_t)15u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_session_info_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&mtch_sched_info_r17_present,
                           &mtch_neighbour_cell_r17_present,
                           &pdsch_cfg_idx_r17_present,
                           &mtch_ssb_map_win_idx_r17_present}));

  HANDLE_CODE(mbs_session_id_r17.unpack(bref));
  HANDLE_CODE(unpack_integer(g_rnti_r17, bref, (uint32_t)0u, (uint32_t)65535u));
  HANDLE_CODE(unpack_dyn_seq_of(mrb_list_broadcast_r17, bref, 1, 4));
  if (mtch_sched_info_r17_present) {
    HANDLE_CODE(unpack_integer(mtch_sched_info_r17, bref, (uint8_t)0u, (uint8_t)63u));
  }
  if (mtch_neighbour_cell_r17_present) {
    HANDLE_CODE(mtch_neighbour_cell_r17.unpack(bref));
  }
  if (pdsch_cfg_idx_r17_present) {
    HANDLE_CODE(unpack_integer(pdsch_cfg_idx_r17, bref, (uint8_t)0u, (uint8_t)15u));
  }
  if (mtch_ssb_map_win_idx_r17_present) {
    HANDLE_CODE(unpack_integer(mtch_ssb_map_win_idx_r17, bref, (uint8_t)0u, (uint8_t)15u));
  }

  return OCUDUASN_SUCCESS;
}
void mbs_session_info_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mbs-SessionId-r17");
  mbs_session_id_r17.to_json(j);
  j.write_int("g-RNTI-r17", g_rnti_r17);
  j.start_array("mrb-ListBroadcast-r17");
  for (const auto& e1 : mrb_list_broadcast_r17) {
    e1.to_json(j);
  }
  j.end_array();
  if (mtch_sched_info_r17_present) {
    j.write_int("mtch-SchedulingInfo-r17", mtch_sched_info_r17);
  }
  if (mtch_neighbour_cell_r17_present) {
    j.write_fieldname("mtch-NeighbourCell-r17");
    mtch_neighbour_cell_r17.to_json(j);
  }
  if (pdsch_cfg_idx_r17_present) {
    j.write_int("pdsch-ConfigIndex-r17", pdsch_cfg_idx_r17);
  }
  if (mtch_ssb_map_win_idx_r17_present) {
    j.write_int("mtch-SSB-MappingWindowIndex-r17", mtch_ssb_map_win_idx_r17);
  }
  j.end_obj();
}

// MRB-PDCP-ConfigMulticast-r18 ::= SEQUENCE
OCUDUASN_CODE mrb_pdcp_cfg_multicast_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(t_reordering_r17_present, 1));

  HANDLE_CODE(pdcp_sn_size_dl_r18.pack(bref));
  HANDLE_CODE(hdr_compress_r18.pack(bref));
  if (t_reordering_r17_present) {
    HANDLE_CODE(t_reordering_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_pdcp_cfg_multicast_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(t_reordering_r17_present, 1));

  HANDLE_CODE(pdcp_sn_size_dl_r18.unpack(bref));
  HANDLE_CODE(hdr_compress_r18.unpack(bref));
  if (t_reordering_r17_present) {
    HANDLE_CODE(t_reordering_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void mrb_pdcp_cfg_multicast_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pdcp-SN-SizeDL-r18", pdcp_sn_size_dl_r18.to_string());
  j.write_fieldname("headerCompression-r18");
  hdr_compress_r18.to_json(j);
  if (t_reordering_r17_present) {
    j.write_str("t-Reordering-r17", t_reordering_r17.to_string());
  }
  j.end_obj();
}

const char* mrb_pdcp_cfg_multicast_r18_s::pdcp_sn_size_dl_r18_opts::to_string() const
{
  static const char* names[] = {"len12bits", "len18bits"};
  return convert_enum_idx(names, 2, value, "mrb_pdcp_cfg_multicast_r18_s::pdcp_sn_size_dl_r18_e_");
}
uint8_t mrb_pdcp_cfg_multicast_r18_s::pdcp_sn_size_dl_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {12, 18};
  return map_enum_number(numbers, 2, value, "mrb_pdcp_cfg_multicast_r18_s::pdcp_sn_size_dl_r18_e_");
}

void mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::set(types::options e)
{
  type_ = e;
}
void mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::set_not_used()
{
  set(types::not_used);
}
mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::rohc_s_&
mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::set_rohc()
{
  set(types::rohc);
  return c;
}
void mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::not_used:
      j.write_null("notUsed");
      break;
    case types::rohc:
      j.write_fieldname("rohc");
      j.start_obj();
      if (c.max_c_id_r18_present) {
        j.write_int("maxCID-r18", c.max_c_id_r18);
      }
      j.write_fieldname("profiles-r18");
      j.start_obj();
      j.write_bool("profile0x0000-r18", c.profiles_r18.profile0x0000_r18);
      j.write_bool("profile0x0001-r18", c.profiles_r18.profile0x0001_r18);
      j.write_bool("profile0x0002-r18", c.profiles_r18.profile0x0002_r18);
      j.end_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::not_used:
      break;
    case types::rohc:
      HANDLE_CODE(bref.pack(c.max_c_id_r18_present, 1));
      if (c.max_c_id_r18_present) {
        HANDLE_CODE(pack_integer(bref, c.max_c_id_r18, (uint8_t)1u, (uint8_t)16u));
      }
      HANDLE_CODE(bref.pack(c.profiles_r18.profile0x0000_r18, 1));
      HANDLE_CODE(bref.pack(c.profiles_r18.profile0x0001_r18, 1));
      HANDLE_CODE(bref.pack(c.profiles_r18.profile0x0002_r18, 1));
      break;
    default:
      log_invalid_choice_id(type_, "mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::not_used:
      break;
    case types::rohc:
      HANDLE_CODE(bref.unpack(c.max_c_id_r18_present, 1));
      if (c.max_c_id_r18_present) {
        HANDLE_CODE(unpack_integer(c.max_c_id_r18, bref, (uint8_t)1u, (uint8_t)16u));
      }
      HANDLE_CODE(bref.unpack(c.profiles_r18.profile0x0000_r18, 1));
      HANDLE_CODE(bref.unpack(c.profiles_r18.profile0x0001_r18, 1));
      HANDLE_CODE(bref.unpack(c.profiles_r18.profile0x0002_r18, 1));
      break;
    default:
      log_invalid_choice_id(type_, "mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"notUsed", "rohc"};
  return convert_enum_idx(names, 2, value, "mrb_pdcp_cfg_multicast_r18_s::hdr_compress_r18_c_::types");
}

const char* mrb_pdcp_cfg_multicast_r18_s::t_reordering_r17_opts::to_string() const
{
  static const char* names[] = {"ms1", "ms10", "ms40", "ms160", "ms500", "ms1000", "ms1250", "ms2750"};
  return convert_enum_idx(names, 8, value, "mrb_pdcp_cfg_multicast_r18_s::t_reordering_r17_e_");
}
uint16_t mrb_pdcp_cfg_multicast_r18_s::t_reordering_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {1, 10, 40, 160, 500, 1000, 1250, 2750};
  return map_enum_number(numbers, 8, value, "mrb_pdcp_cfg_multicast_r18_s::t_reordering_r17_e_");
}

// MRB-RLC-ConfigMulticast-r18 ::= SEQUENCE
OCUDUASN_CODE mrb_rlc_cfg_multicast_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(t_reassembly_r18_present, 1));

  HANDLE_CODE(lc_ch_id_r18.pack(bref));
  HANDLE_CODE(sn_field_len_r18.pack(bref));
  if (t_reassembly_r18_present) {
    HANDLE_CODE(t_reassembly_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_rlc_cfg_multicast_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(t_reassembly_r18_present, 1));

  HANDLE_CODE(lc_ch_id_r18.unpack(bref));
  HANDLE_CODE(sn_field_len_r18.unpack(bref));
  if (t_reassembly_r18_present) {
    HANDLE_CODE(t_reassembly_r18.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void mrb_rlc_cfg_multicast_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("logicalChannelIdentity-r18");
  lc_ch_id_r18.to_json(j);
  j.write_str("sn-FieldLength-r18", sn_field_len_r18.to_string());
  if (t_reassembly_r18_present) {
    j.write_str("t-Reassembly-r18", t_reassembly_r18.to_string());
  }
  j.end_obj();
}

void mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::destroy_() {}
void mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::lc_ch_id_r18_c_(const mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::lc_ch_idmulticast_r18:
      c.init(other.c.get<uint8_t>());
      break;
    case types::lc_ch_id_ext_r18:
      c.init(other.c.get<uint32_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_");
  }
}
mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_&
mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::operator=(const mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::lc_ch_idmulticast_r18:
      c.set(other.c.get<uint8_t>());
      break;
    case types::lc_ch_id_ext_r18:
      c.set(other.c.get<uint32_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_");
  }

  return *this;
}
uint8_t& mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::set_lc_ch_idmulticast_r18()
{
  set(types::lc_ch_idmulticast_r18);
  return c.get<uint8_t>();
}
uint32_t& mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::set_lc_ch_id_ext_r18()
{
  set(types::lc_ch_id_ext_r18);
  return c.get<uint32_t>();
}
void mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lc_ch_idmulticast_r18:
      j.write_int("logicalChannelIdentitymulticast-r18", c.get<uint8_t>());
      break;
    case types::lc_ch_id_ext_r18:
      j.write_int("logicalChannelIdentityExt-r18", c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::lc_ch_idmulticast_r18:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)32u));
      break;
    case types::lc_ch_id_ext_r18:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)320u, (uint32_t)65855u));
      break;
    default:
      log_invalid_choice_id(type_, "mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::lc_ch_idmulticast_r18:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)32u));
      break;
    case types::lc_ch_id_ext_r18:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)320u, (uint32_t)65855u));
      break;
    default:
      log_invalid_choice_id(type_, "mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"logicalChannelIdentitymulticast-r18", "logicalChannelIdentityExt-r18"};
  return convert_enum_idx(names, 2, value, "mrb_rlc_cfg_multicast_r18_s::lc_ch_id_r18_c_::types");
}

const char* mrb_rlc_cfg_multicast_r18_s::sn_field_len_r18_opts::to_string() const
{
  static const char* names[] = {"size6", "size12"};
  return convert_enum_idx(names, 2, value, "mrb_rlc_cfg_multicast_r18_s::sn_field_len_r18_e_");
}
uint8_t mrb_rlc_cfg_multicast_r18_s::sn_field_len_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {6, 12};
  return map_enum_number(numbers, 2, value, "mrb_rlc_cfg_multicast_r18_s::sn_field_len_r18_e_");
}

// MRB-InfoMulticast-r18 ::= SEQUENCE
OCUDUASN_CODE mrb_info_multicast_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pdcp_cfg_r18.pack(bref));
  HANDLE_CODE(rlc_cfg_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_info_multicast_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(pdcp_cfg_r18.unpack(bref));
  HANDLE_CODE(rlc_cfg_r18.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mrb_info_multicast_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("pdcp-Config-r18");
  pdcp_cfg_r18.to_json(j);
  j.write_fieldname("rlc-Config-r18");
  rlc_cfg_r18.to_json(j);
  j.end_obj();
}

// MBS-SessionInfoMulticast-r18 ::= SEQUENCE
OCUDUASN_CODE mbs_session_info_multicast_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {false,
                         g_rnti_r18_present,
                         mrb_list_multicast_r18.size() > 0,
                         mtch_sched_info_r18_present,
                         mtch_neighbour_cell_r18_present,
                         pdsch_cfg_idx_r18_present,
                         mtch_ssb_map_win_idx_r18_present,
                         thres_idx_r18_present,
                         pdcp_sync_ind_r18_present,
                         stop_monitoring_rnti_r18_present}));

  HANDLE_CODE(mbs_session_id_r18.pack(bref));
  if (g_rnti_r18_present) {
    HANDLE_CODE(pack_integer(bref, g_rnti_r18, (uint32_t)0u, (uint32_t)65535u));
  }
  if (mrb_list_multicast_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mrb_list_multicast_r18, 1, 32));
  }
  if (mtch_sched_info_r18_present) {
    HANDLE_CODE(pack_integer(bref, mtch_sched_info_r18, (uint8_t)0u, (uint8_t)63u));
  }
  if (mtch_neighbour_cell_r18_present) {
    HANDLE_CODE(mtch_neighbour_cell_r18.pack(bref));
  }
  if (pdsch_cfg_idx_r18_present) {
    HANDLE_CODE(pack_integer(bref, pdsch_cfg_idx_r18, (uint8_t)0u, (uint8_t)15u));
  }
  if (mtch_ssb_map_win_idx_r18_present) {
    HANDLE_CODE(pack_integer(bref, mtch_ssb_map_win_idx_r18, (uint8_t)0u, (uint8_t)15u));
  }
  if (thres_idx_r18_present) {
    HANDLE_CODE(pack_integer(bref, thres_idx_r18, (uint8_t)0u, (uint8_t)7u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_session_info_multicast_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool mrb_list_multicast_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &g_rnti_r18_present,
                           &mrb_list_multicast_r18_present,
                           &mtch_sched_info_r18_present,
                           &mtch_neighbour_cell_r18_present,
                           &pdsch_cfg_idx_r18_present,
                           &mtch_ssb_map_win_idx_r18_present,
                           &thres_idx_r18_present,
                           &pdcp_sync_ind_r18_present,
                           &stop_monitoring_rnti_r18_present}));

  HANDLE_CODE(mbs_session_id_r18.unpack(bref));
  if (g_rnti_r18_present) {
    HANDLE_CODE(unpack_integer(g_rnti_r18, bref, (uint32_t)0u, (uint32_t)65535u));
  }
  if (mrb_list_multicast_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mrb_list_multicast_r18, bref, 1, 32));
  }
  if (mtch_sched_info_r18_present) {
    HANDLE_CODE(unpack_integer(mtch_sched_info_r18, bref, (uint8_t)0u, (uint8_t)63u));
  }
  if (mtch_neighbour_cell_r18_present) {
    HANDLE_CODE(mtch_neighbour_cell_r18.unpack(bref));
  }
  if (pdsch_cfg_idx_r18_present) {
    HANDLE_CODE(unpack_integer(pdsch_cfg_idx_r18, bref, (uint8_t)0u, (uint8_t)15u));
  }
  if (mtch_ssb_map_win_idx_r18_present) {
    HANDLE_CODE(unpack_integer(mtch_ssb_map_win_idx_r18, bref, (uint8_t)0u, (uint8_t)15u));
  }
  if (thres_idx_r18_present) {
    HANDLE_CODE(unpack_integer(thres_idx_r18, bref, (uint8_t)0u, (uint8_t)7u));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbs_session_info_multicast_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mbs-SessionId-r18");
  mbs_session_id_r18.to_json(j);
  if (g_rnti_r18_present) {
    j.write_int("g-RNTI-r18", g_rnti_r18);
  }
  if (mrb_list_multicast_r18.size() > 0) {
    j.start_array("mrb-ListMulticast-r18");
    for (const auto& e1 : mrb_list_multicast_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mtch_sched_info_r18_present) {
    j.write_int("mtch-SchedulingInfo-r18", mtch_sched_info_r18);
  }
  if (mtch_neighbour_cell_r18_present) {
    j.write_fieldname("mtch-NeighbourCell-r18");
    mtch_neighbour_cell_r18.to_json(j);
  }
  if (pdsch_cfg_idx_r18_present) {
    j.write_int("pdsch-ConfigIndex-r18", pdsch_cfg_idx_r18);
  }
  if (mtch_ssb_map_win_idx_r18_present) {
    j.write_int("mtch-SSB-MappingWindowIndex-r18", mtch_ssb_map_win_idx_r18);
  }
  if (thres_idx_r18_present) {
    j.write_int("thresholdIndex-r18", thres_idx_r18);
  }
  if (pdcp_sync_ind_r18_present) {
    j.write_str("pdcp-SyncIndicator-r18", "true");
  }
  if (stop_monitoring_rnti_r18_present) {
    j.write_str("stopMonitoringRNTI-r18", "true");
  }
  j.end_obj();
}

// MTCH-SSB-MappingWindowCycleOffset-r17 ::= CHOICE
void mtch_ssb_map_win_cycle_offset_r17_c::destroy_() {}
void mtch_ssb_map_win_cycle_offset_r17_c::set(types::options e)
{
  destroy_();
  type_ = e;
}
mtch_ssb_map_win_cycle_offset_r17_c::mtch_ssb_map_win_cycle_offset_r17_c(
    const mtch_ssb_map_win_cycle_offset_r17_c& other)
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
    case types::ms64:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms128:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms256:
      c.init(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mtch_ssb_map_win_cycle_offset_r17_c");
  }
}
mtch_ssb_map_win_cycle_offset_r17_c&
mtch_ssb_map_win_cycle_offset_r17_c::operator=(const mtch_ssb_map_win_cycle_offset_r17_c& other)
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
    case types::ms64:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms128:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms256:
      c.set(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mtch_ssb_map_win_cycle_offset_r17_c");
  }

  return *this;
}
uint8_t& mtch_ssb_map_win_cycle_offset_r17_c::set_ms10()
{
  set(types::ms10);
  return c.get<uint8_t>();
}
uint8_t& mtch_ssb_map_win_cycle_offset_r17_c::set_ms20()
{
  set(types::ms20);
  return c.get<uint8_t>();
}
uint8_t& mtch_ssb_map_win_cycle_offset_r17_c::set_ms32()
{
  set(types::ms32);
  return c.get<uint8_t>();
}
uint8_t& mtch_ssb_map_win_cycle_offset_r17_c::set_ms64()
{
  set(types::ms64);
  return c.get<uint8_t>();
}
uint8_t& mtch_ssb_map_win_cycle_offset_r17_c::set_ms128()
{
  set(types::ms128);
  return c.get<uint8_t>();
}
uint16_t& mtch_ssb_map_win_cycle_offset_r17_c::set_ms256()
{
  set(types::ms256);
  return c.get<uint16_t>();
}
void mtch_ssb_map_win_cycle_offset_r17_c::to_json(json_writer& j) const
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
    case types::ms64:
      j.write_int("ms64", c.get<uint8_t>());
      break;
    case types::ms128:
      j.write_int("ms128", c.get<uint8_t>());
      break;
    case types::ms256:
      j.write_int("ms256", c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "mtch_ssb_map_win_cycle_offset_r17_c");
  }
  j.end_obj();
}
OCUDUASN_CODE mtch_ssb_map_win_cycle_offset_r17_c::pack(bit_ref& bref) const
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
    case types::ms64:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)63u));
      break;
    case types::ms128:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)127u));
      break;
    case types::ms256:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u));
      break;
    default:
      log_invalid_choice_id(type_, "mtch_ssb_map_win_cycle_offset_r17_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mtch_ssb_map_win_cycle_offset_r17_c::unpack(cbit_ref& bref)
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
    case types::ms64:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)63u));
      break;
    case types::ms128:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)127u));
      break;
    case types::ms256:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u));
      break;
    default:
      log_invalid_choice_id(type_, "mtch_ssb_map_win_cycle_offset_r17_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mtch_ssb_map_win_cycle_offset_r17_c::types_opts::to_string() const
{
  static const char* names[] = {"ms10", "ms20", "ms32", "ms64", "ms128", "ms256"};
  return convert_enum_idx(names, 6, value, "mtch_ssb_map_win_cycle_offset_r17_c::types");
}
uint16_t mtch_ssb_map_win_cycle_offset_r17_c::types_opts::to_number() const
{
  static const uint16_t numbers[] = {10, 20, 32, 64, 128, 256};
  return map_enum_number(numbers, 6, value, "mtch_ssb_map_win_cycle_offset_r17_c::types");
}

// MBSBroadcastConfiguration-r17-IEs ::= SEQUENCE
OCUDUASN_CODE mbs_broadcast_cfg_r17_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {mbs_session_info_list_r17.size() > 0,
                         mbs_neighbour_cell_list_r17_present,
                         drx_cfg_ptm_list_r17.size() > 0,
                         pdsch_cfg_mtch_r17_present,
                         mtch_ssb_map_win_list_r17.size() > 0,
                         late_non_crit_ext.size() > 0,
                         non_crit_ext_present}));

  if (mbs_session_info_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_session_info_list_r17, 1, 1024));
  }
  if (mbs_neighbour_cell_list_r17_present) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_neighbour_cell_list_r17, 0, 8));
  }
  if (drx_cfg_ptm_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drx_cfg_ptm_list_r17, 1, 64));
  }
  if (pdsch_cfg_mtch_r17_present) {
    HANDLE_CODE(pdsch_cfg_mtch_r17.pack(bref));
  }
  if (mtch_ssb_map_win_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mtch_ssb_map_win_list_r17, 1, 16));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_broadcast_cfg_r17_ies_s::unpack(cbit_ref& bref)
{
  bool mbs_session_info_list_r17_present;
  bool drx_cfg_ptm_list_r17_present;
  bool mtch_ssb_map_win_list_r17_present;
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&mbs_session_info_list_r17_present,
                           &mbs_neighbour_cell_list_r17_present,
                           &drx_cfg_ptm_list_r17_present,
                           &pdsch_cfg_mtch_r17_present,
                           &mtch_ssb_map_win_list_r17_present,
                           &late_non_crit_ext_present,
                           &non_crit_ext_present}));

  if (mbs_session_info_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_session_info_list_r17, bref, 1, 1024));
  }
  if (mbs_neighbour_cell_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_neighbour_cell_list_r17, bref, 0, 8));
  }
  if (drx_cfg_ptm_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drx_cfg_ptm_list_r17, bref, 1, 64));
  }
  if (pdsch_cfg_mtch_r17_present) {
    HANDLE_CODE(pdsch_cfg_mtch_r17.unpack(bref));
  }
  if (mtch_ssb_map_win_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mtch_ssb_map_win_list_r17, bref, 1, 16));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void mbs_broadcast_cfg_r17_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mbs_session_info_list_r17.size() > 0) {
    j.start_array("mbs-SessionInfoList-r17");
    for (const auto& e1 : mbs_session_info_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mbs_neighbour_cell_list_r17_present) {
    j.start_array("mbs-NeighbourCellList-r17");
    for (const auto& e1 : mbs_neighbour_cell_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drx_cfg_ptm_list_r17.size() > 0) {
    j.start_array("drx-ConfigPTM-List-r17");
    for (const auto& e1 : drx_cfg_ptm_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdsch_cfg_mtch_r17_present) {
    j.write_fieldname("pdsch-ConfigMTCH-r17");
    pdsch_cfg_mtch_r17.to_json(j);
  }
  if (mtch_ssb_map_win_list_r17.size() > 0) {
    j.start_array("mtch-SSB-MappingWindowList-r17");
    for (const auto& e1 : mtch_ssb_map_win_list_r17) {
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

// MBSBroadcastConfiguration-r17 ::= SEQUENCE
OCUDUASN_CODE mbs_broadcast_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_broadcast_cfg_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void mbs_broadcast_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void mbs_broadcast_cfg_r17_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
mbs_broadcast_cfg_r17_ies_s& mbs_broadcast_cfg_r17_s::crit_exts_c_::set_mbs_broadcast_cfg_r17()
{
  set(types::mbs_broadcast_cfg_r17);
  return c;
}
void mbs_broadcast_cfg_r17_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void mbs_broadcast_cfg_r17_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::mbs_broadcast_cfg_r17:
      j.write_fieldname("mbsBroadcastConfiguration-r17");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "mbs_broadcast_cfg_r17_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mbs_broadcast_cfg_r17_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::mbs_broadcast_cfg_r17:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_broadcast_cfg_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_broadcast_cfg_r17_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::mbs_broadcast_cfg_r17:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_broadcast_cfg_r17_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mbs_broadcast_cfg_r17_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"mbsBroadcastConfiguration-r17", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "mbs_broadcast_cfg_r17_s::crit_exts_c_::types");
}

// ThresholdMBS-r18 ::= SEQUENCE
OCUDUASN_CODE thres_mbs_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {rsrp_r18_present, rsrq_r18_present}));

  if (rsrp_r18_present) {
    HANDLE_CODE(pack_integer(bref, rsrp_r18, (uint8_t)0u, (uint8_t)127u));
  }
  if (rsrq_r18_present) {
    HANDLE_CODE(pack_integer(bref, rsrq_r18, (uint8_t)0u, (uint8_t)127u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE thres_mbs_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&rsrp_r18_present, &rsrq_r18_present}));

  if (rsrp_r18_present) {
    HANDLE_CODE(unpack_integer(rsrp_r18, bref, (uint8_t)0u, (uint8_t)127u));
  }
  if (rsrq_r18_present) {
    HANDLE_CODE(unpack_integer(rsrq_r18, bref, (uint8_t)0u, (uint8_t)127u));
  }

  return OCUDUASN_SUCCESS;
}
void thres_mbs_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (rsrp_r18_present) {
    j.write_int("rsrp-r18", rsrp_r18);
  }
  if (rsrq_r18_present) {
    j.write_int("rsrq-r18", rsrq_r18);
  }
  j.end_obj();
}

// MBSMulticastConfiguration-r18-IEs ::= SEQUENCE
OCUDUASN_CODE mbs_multicast_cfg_r18_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {mbs_session_info_list_multicast_r18.size() > 0,
                         mbs_neighbour_cell_list_r18_present,
                         drx_cfg_ptm_list_r18.size() > 0,
                         pdsch_cfg_mtch_r18_present,
                         mtch_ssb_map_win_list_r18.size() > 0,
                         thres_mbs_list_r18.size() > 0,
                         late_non_crit_ext.size() > 0,
                         non_crit_ext_present}));

  if (mbs_session_info_list_multicast_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_session_info_list_multicast_r18, 1, 1024));
  }
  if (mbs_neighbour_cell_list_r18_present) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_neighbour_cell_list_r18, 0, 8));
  }
  if (drx_cfg_ptm_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drx_cfg_ptm_list_r18, 1, 64));
  }
  if (pdsch_cfg_mtch_r18_present) {
    HANDLE_CODE(pdsch_cfg_mtch_r18.pack(bref));
  }
  if (mtch_ssb_map_win_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mtch_ssb_map_win_list_r18, 1, 16));
  }
  if (thres_mbs_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, thres_mbs_list_r18, 1, 8));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_multicast_cfg_r18_ies_s::unpack(cbit_ref& bref)
{
  bool mbs_session_info_list_multicast_r18_present;
  bool drx_cfg_ptm_list_r18_present;
  bool mtch_ssb_map_win_list_r18_present;
  bool thres_mbs_list_r18_present;
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&mbs_session_info_list_multicast_r18_present,
                           &mbs_neighbour_cell_list_r18_present,
                           &drx_cfg_ptm_list_r18_present,
                           &pdsch_cfg_mtch_r18_present,
                           &mtch_ssb_map_win_list_r18_present,
                           &thres_mbs_list_r18_present,
                           &late_non_crit_ext_present,
                           &non_crit_ext_present}));

  if (mbs_session_info_list_multicast_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_session_info_list_multicast_r18, bref, 1, 1024));
  }
  if (mbs_neighbour_cell_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_neighbour_cell_list_r18, bref, 0, 8));
  }
  if (drx_cfg_ptm_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drx_cfg_ptm_list_r18, bref, 1, 64));
  }
  if (pdsch_cfg_mtch_r18_present) {
    HANDLE_CODE(pdsch_cfg_mtch_r18.unpack(bref));
  }
  if (mtch_ssb_map_win_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mtch_ssb_map_win_list_r18, bref, 1, 16));
  }
  if (thres_mbs_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(thres_mbs_list_r18, bref, 1, 8));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void mbs_multicast_cfg_r18_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mbs_session_info_list_multicast_r18.size() > 0) {
    j.start_array("mbs-SessionInfoListMulticast-r18");
    for (const auto& e1 : mbs_session_info_list_multicast_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mbs_neighbour_cell_list_r18_present) {
    j.start_array("mbs-NeighbourCellList-r18");
    for (const auto& e1 : mbs_neighbour_cell_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drx_cfg_ptm_list_r18.size() > 0) {
    j.start_array("drx-ConfigPTM-List-r18");
    for (const auto& e1 : drx_cfg_ptm_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdsch_cfg_mtch_r18_present) {
    j.write_fieldname("pdsch-ConfigMTCH-r18");
    pdsch_cfg_mtch_r18.to_json(j);
  }
  if (mtch_ssb_map_win_list_r18.size() > 0) {
    j.start_array("mtch-SSB-MappingWindowList-r18");
    for (const auto& e1 : mtch_ssb_map_win_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (thres_mbs_list_r18.size() > 0) {
    j.start_array("thresholdMBS-List-r18");
    for (const auto& e1 : thres_mbs_list_r18) {
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

// MBSMulticastConfiguration-r18 ::= SEQUENCE
OCUDUASN_CODE mbs_multicast_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(crit_exts.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_multicast_cfg_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(crit_exts.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void mbs_multicast_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("criticalExtensions");
  crit_exts.to_json(j);
  j.end_obj();
}

void mbs_multicast_cfg_r18_s::crit_exts_c_::set(types::options e)
{
  type_ = e;
}
mbs_multicast_cfg_r18_ies_s& mbs_multicast_cfg_r18_s::crit_exts_c_::set_mbs_multicast_cfg_r18()
{
  set(types::mbs_multicast_cfg_r18);
  return c;
}
void mbs_multicast_cfg_r18_s::crit_exts_c_::set_crit_exts_future()
{
  set(types::crit_exts_future);
}
void mbs_multicast_cfg_r18_s::crit_exts_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::mbs_multicast_cfg_r18:
      j.write_fieldname("mbsMulticastConfiguration-r18");
      c.to_json(j);
      break;
    case types::crit_exts_future:
      j.write_fieldname("criticalExtensionsFuture");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "mbs_multicast_cfg_r18_s::crit_exts_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mbs_multicast_cfg_r18_s::crit_exts_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::mbs_multicast_cfg_r18:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_multicast_cfg_r18_s::crit_exts_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_multicast_cfg_r18_s::crit_exts_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::mbs_multicast_cfg_r18:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::crit_exts_future:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_multicast_cfg_r18_s::crit_exts_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mbs_multicast_cfg_r18_s::crit_exts_c_::types_opts::to_string() const
{
  static const char* names[] = {"mbsMulticastConfiguration-r18", "criticalExtensionsFuture"};
  return convert_enum_idx(names, 2, value, "mbs_multicast_cfg_r18_s::crit_exts_c_::types");
}

// MCCH-MessageType-r17 ::= CHOICE
void mcch_msg_type_r17_c::set(types::options e)
{
  type_ = e;
}
mcch_msg_type_r17_c::c1_c_& mcch_msg_type_r17_c::set_c1()
{
  set(types::c1);
  return c;
}
void mcch_msg_type_r17_c::set_msg_class_ext()
{
  set(types::msg_class_ext);
}
void mcch_msg_type_r17_c::to_json(json_writer& j) const
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
      log_invalid_choice_id(type_, "mcch_msg_type_r17_c");
  }
  j.end_obj();
}
OCUDUASN_CODE mcch_msg_type_r17_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::msg_class_ext:
      break;
    default:
      log_invalid_choice_id(type_, "mcch_msg_type_r17_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcch_msg_type_r17_c::unpack(cbit_ref& bref)
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
      log_invalid_choice_id(type_, "mcch_msg_type_r17_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void mcch_msg_type_r17_c::c1_c_::set(types::options e)
{
  type_ = e;
}
mbs_broadcast_cfg_r17_s& mcch_msg_type_r17_c::c1_c_::set_mbs_broadcast_cfg_r17()
{
  set(types::mbs_broadcast_cfg_r17);
  return c;
}
void mcch_msg_type_r17_c::c1_c_::set_spare1()
{
  set(types::spare1);
}
void mcch_msg_type_r17_c::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::mbs_broadcast_cfg_r17:
      j.write_fieldname("mbsBroadcastConfiguration-r17");
      c.to_json(j);
      break;
    case types::spare1:
      j.write_null("spare1");
      break;
    default:
      log_invalid_choice_id(type_, "mcch_msg_type_r17_c::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mcch_msg_type_r17_c::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::mbs_broadcast_cfg_r17:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "mcch_msg_type_r17_c::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcch_msg_type_r17_c::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::mbs_broadcast_cfg_r17:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "mcch_msg_type_r17_c::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mcch_msg_type_r17_c::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"mbsBroadcastConfiguration-r17", "spare1"};
  return convert_enum_idx(names, 2, value, "mcch_msg_type_r17_c::c1_c_::types");
}

const char* mcch_msg_type_r17_c::types_opts::to_string() const
{
  static const char* names[] = {"c1", "messageClassExtension"};
  return convert_enum_idx(names, 2, value, "mcch_msg_type_r17_c::types");
}
uint8_t mcch_msg_type_r17_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "mcch_msg_type_r17_c::types");
}

// MCCH-Message-r17 ::= SEQUENCE
OCUDUASN_CODE mcch_msg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(msg.pack(bref));

  bref.align_bytes_zero();

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcch_msg_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(msg.unpack(bref));

  bref.align_bytes();

  return OCUDUASN_SUCCESS;
}
void mcch_msg_r17_s::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.start_obj("MCCH-Message-r17");
  j.write_fieldname("message");
  msg.to_json(j);
  j.end_obj();
  j.end_obj();
  j.end_array();
}

// MeasAndMobParametersCommon-v15t0 ::= SEQUENCE
OCUDUASN_CODE meas_and_mob_params_common_v15t0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(intra_f_neigh_meas_for_scell_without_ssb_present, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_and_mob_params_common_v15t0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(intra_f_neigh_meas_for_scell_without_ssb_present, 1));

  return OCUDUASN_SUCCESS;
}
void meas_and_mob_params_common_v15t0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (intra_f_neigh_meas_for_scell_without_ssb_present) {
    j.write_str("intraF-NeighMeasForSCellWithoutSSB", "supported");
  }
  j.end_obj();
}

// MeasAndMobParameters-v15t0 ::= SEQUENCE
OCUDUASN_CODE meas_and_mob_params_v15t0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(meas_and_mob_params_common_v15t0_present, 1));

  if (meas_and_mob_params_common_v15t0_present) {
    HANDLE_CODE(meas_and_mob_params_common_v15t0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_and_mob_params_v15t0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(meas_and_mob_params_common_v15t0_present, 1));

  if (meas_and_mob_params_common_v15t0_present) {
    HANDLE_CODE(meas_and_mob_params_common_v15t0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void meas_and_mob_params_v15t0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (meas_and_mob_params_common_v15t0_present) {
    j.write_fieldname("measAndMobParametersCommon-v15t0");
    meas_and_mob_params_common_v15t0.to_json(j);
  }
  j.end_obj();
}

// MeasResultSCG-Failure ::= SEQUENCE
OCUDUASN_CODE meas_result_scg_fail_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= location_info_r16.is_present();
  HANDLE_CODE(bref.pack(group_flags.any(), 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, meas_result_per_mo_list, 1, 8));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(location_info_r16.is_present(), 1));
      if (location_info_r16.is_present()) {
        HANDLE_CODE(location_info_r16->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_result_scg_fail_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_dyn_seq_of(meas_result_per_mo_list, bref, 1, 8));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(location_info_r16, bref);
      if (location_info_r16.is_present()) {
        HANDLE_CODE(location_info_r16->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void meas_result_scg_fail_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("measResultPerMOList");
  for (const auto& e1 : meas_result_per_mo_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (location_info_r16.is_present()) {
    j.write_fieldname("locationInfo-r16");
    location_info_r16->to_json(j);
  }
  j.end_obj();
}

// MulticastMCCH-MessageType-r18 ::= CHOICE
void multicast_mcch_msg_type_r18_c::set(types::options e)
{
  type_ = e;
}
multicast_mcch_msg_type_r18_c::c1_c_& multicast_mcch_msg_type_r18_c::set_c1()
{
  set(types::c1);
  return c;
}
void multicast_mcch_msg_type_r18_c::set_msg_class_ext()
{
  set(types::msg_class_ext);
}
void multicast_mcch_msg_type_r18_c::to_json(json_writer& j) const
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
      log_invalid_choice_id(type_, "multicast_mcch_msg_type_r18_c");
  }
  j.end_obj();
}
OCUDUASN_CODE multicast_mcch_msg_type_r18_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::msg_class_ext:
      break;
    default:
      log_invalid_choice_id(type_, "multicast_mcch_msg_type_r18_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_mcch_msg_type_r18_c::unpack(cbit_ref& bref)
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
      log_invalid_choice_id(type_, "multicast_mcch_msg_type_r18_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void multicast_mcch_msg_type_r18_c::c1_c_::set(types::options e)
{
  type_ = e;
}
mbs_multicast_cfg_r18_s& multicast_mcch_msg_type_r18_c::c1_c_::set_mbs_multicast_cfg_r18()
{
  set(types::mbs_multicast_cfg_r18);
  return c;
}
void multicast_mcch_msg_type_r18_c::c1_c_::set_spare1()
{
  set(types::spare1);
}
void multicast_mcch_msg_type_r18_c::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::mbs_multicast_cfg_r18:
      j.write_fieldname("mbsMulticastConfiguration-r18");
      c.to_json(j);
      break;
    case types::spare1:
      j.write_null("spare1");
      break;
    default:
      log_invalid_choice_id(type_, "multicast_mcch_msg_type_r18_c::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE multicast_mcch_msg_type_r18_c::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::mbs_multicast_cfg_r18:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "multicast_mcch_msg_type_r18_c::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_mcch_msg_type_r18_c::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::mbs_multicast_cfg_r18:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "multicast_mcch_msg_type_r18_c::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* multicast_mcch_msg_type_r18_c::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"mbsMulticastConfiguration-r18", "spare1"};
  return convert_enum_idx(names, 2, value, "multicast_mcch_msg_type_r18_c::c1_c_::types");
}

const char* multicast_mcch_msg_type_r18_c::types_opts::to_string() const
{
  static const char* names[] = {"c1", "messageClassExtension"};
  return convert_enum_idx(names, 2, value, "multicast_mcch_msg_type_r18_c::types");
}
uint8_t multicast_mcch_msg_type_r18_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "multicast_mcch_msg_type_r18_c::types");
}

// MulticastMCCH-Message-r18 ::= SEQUENCE
OCUDUASN_CODE multicast_mcch_msg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(msg.pack(bref));

  bref.align_bytes_zero();

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_mcch_msg_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(msg.unpack(bref));

  bref.align_bytes();

  return OCUDUASN_SUCCESS;
}
void multicast_mcch_msg_r18_s::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.start_obj("MulticastMCCH-Message-r18");
  j.write_fieldname("message");
  msg.to_json(j);
  j.end_obj();
  j.end_obj();
  j.end_array();
}

// NRDC-Parameters-v15c0 ::= SEQUENCE
OCUDUASN_CODE nrdc_params_v15c0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {pdcp_dupl_split_srb_present, pdcp_dupl_split_drb_present}));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE nrdc_params_v15c0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&pdcp_dupl_split_srb_present, &pdcp_dupl_split_drb_present}));

  return OCUDUASN_SUCCESS;
}
void nrdc_params_v15c0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdcp_dupl_split_srb_present) {
    j.write_str("pdcp-DuplicationSplitSRB", "supported");
  }
  if (pdcp_dupl_split_drb_present) {
    j.write_str("pdcp-DuplicationSplitDRB", "supported");
  }
  j.end_obj();
}

// Phy-ParametersCommon-v16a0 ::= SEQUENCE
OCUDUASN_CODE phy_params_common_v16a0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(srs_periodicity_and_offset_ext_r16_present, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE phy_params_common_v16a0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(srs_periodicity_and_offset_ext_r16_present, 1));

  return OCUDUASN_SUCCESS;
}
void phy_params_common_v16a0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_periodicity_and_offset_ext_r16_present) {
    j.write_str("srs-PeriodicityAndOffsetExt-r16", "supported");
  }
  j.end_obj();
}

// Phy-Parameters-v16a0 ::= SEQUENCE
OCUDUASN_CODE phy_params_v16a0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(phy_params_common_v16a0_present, 1));

  if (phy_params_common_v16a0_present) {
    HANDLE_CODE(phy_params_common_v16a0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE phy_params_v16a0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(phy_params_common_v16a0_present, 1));

  if (phy_params_common_v16a0_present) {
    HANDLE_CODE(phy_params_common_v16a0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void phy_params_v16a0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (phy_params_common_v16a0_present) {
    j.write_fieldname("phy-ParametersCommon-v16a0");
    phy_params_common_v16a0.to_json(j);
  }
  j.end_obj();
}

// RF-Parameters-v15g0 ::= SEQUENCE
OCUDUASN_CODE rf_params_v15g0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(supported_band_combination_list_v15g0.size() > 0, 1));

  if (supported_band_combination_list_v15g0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_combination_list_v15g0, 1, 65536));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rf_params_v15g0_s::unpack(cbit_ref& bref)
{
  bool supported_band_combination_list_v15g0_present;
  HANDLE_CODE(bref.unpack(supported_band_combination_list_v15g0_present, 1));

  if (supported_band_combination_list_v15g0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_combination_list_v15g0, bref, 1, 65536));
  }

  return OCUDUASN_SUCCESS;
}
void rf_params_v15g0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (supported_band_combination_list_v15g0.size() > 0) {
    j.start_array("supportedBandCombinationList-v15g0");
    for (const auto& e1 : supported_band_combination_list_v15g0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RF-Parameters-v16a0 ::= SEQUENCE
OCUDUASN_CODE rf_params_v16a0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {supported_band_combination_list_v16a0.size() > 0,
                         supported_band_combination_list_ul_tx_switch_v16a0.size() > 0}));

  if (supported_band_combination_list_v16a0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_combination_list_v16a0, 1, 65536));
  }
  if (supported_band_combination_list_ul_tx_switch_v16a0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_combination_list_ul_tx_switch_v16a0, 1, 65536));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rf_params_v16a0_s::unpack(cbit_ref& bref)
{
  bool supported_band_combination_list_v16a0_present;
  bool supported_band_combination_list_ul_tx_switch_v16a0_present;
  HANDLE_CODE(unpack_bits(
      bref,
      {&supported_band_combination_list_v16a0_present, &supported_band_combination_list_ul_tx_switch_v16a0_present}));

  if (supported_band_combination_list_v16a0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_combination_list_v16a0, bref, 1, 65536));
  }
  if (supported_band_combination_list_ul_tx_switch_v16a0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_combination_list_ul_tx_switch_v16a0, bref, 1, 65536));
  }

  return OCUDUASN_SUCCESS;
}
void rf_params_v16a0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (supported_band_combination_list_v16a0.size() > 0) {
    j.start_array("supportedBandCombinationList-v16a0");
    for (const auto& e1 : supported_band_combination_list_v16a0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (supported_band_combination_list_ul_tx_switch_v16a0.size() > 0) {
    j.start_array("supportedBandCombinationList-UplinkTxSwitch-v16a0");
    for (const auto& e1 : supported_band_combination_list_ul_tx_switch_v16a0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RF-Parameters-v16c0 ::= SEQUENCE
OCUDUASN_CODE rf_params_v16c0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_list_nr_v16c0, 1, 1024));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rf_params_v16c0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_dyn_seq_of(supported_band_list_nr_v16c0, bref, 1, 1024));

  return OCUDUASN_SUCCESS;
}
void rf_params_v16c0_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("supportedBandListNR-v16c0");
  for (const auto& e1 : supported_band_list_nr_v16c0) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// RF-Parameters-v16j0 ::= SEQUENCE
OCUDUASN_CODE rf_params_v16j0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {supported_band_combination_list_v16j0.size() > 0,
                         supported_band_combination_list_ul_tx_switch_v16j0.size() > 0}));

  if (supported_band_combination_list_v16j0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_combination_list_v16j0, 1, 65536));
  }
  if (supported_band_combination_list_ul_tx_switch_v16j0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_combination_list_ul_tx_switch_v16j0, 1, 65536));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rf_params_v16j0_s::unpack(cbit_ref& bref)
{
  bool supported_band_combination_list_v16j0_present;
  bool supported_band_combination_list_ul_tx_switch_v16j0_present;
  HANDLE_CODE(unpack_bits(
      bref,
      {&supported_band_combination_list_v16j0_present, &supported_band_combination_list_ul_tx_switch_v16j0_present}));

  if (supported_band_combination_list_v16j0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_combination_list_v16j0, bref, 1, 65536));
  }
  if (supported_band_combination_list_ul_tx_switch_v16j0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_combination_list_ul_tx_switch_v16j0, bref, 1, 65536));
  }

  return OCUDUASN_SUCCESS;
}
void rf_params_v16j0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (supported_band_combination_list_v16j0.size() > 0) {
    j.start_array("supportedBandCombinationList-v16j0");
    for (const auto& e1 : supported_band_combination_list_v16j0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (supported_band_combination_list_ul_tx_switch_v16j0.size() > 0) {
    j.start_array("supportedBandCombinationList-UplinkTxSwitch-v16j0");
    for (const auto& e1 : supported_band_combination_list_ul_tx_switch_v16j0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RF-Parameters-v17b0 ::= SEQUENCE
OCUDUASN_CODE rf_params_v17b0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {supported_band_list_nr_v17b0.size() > 0,
                         supported_band_combination_list_v17b0.size() > 0,
                         supported_band_combination_list_ul_tx_switch_v17b0.size() > 0}));

  if (supported_band_list_nr_v17b0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_list_nr_v17b0, 1, 1024));
  }
  if (supported_band_combination_list_v17b0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_combination_list_v17b0, 1, 65536));
  }
  if (supported_band_combination_list_ul_tx_switch_v17b0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_band_combination_list_ul_tx_switch_v17b0, 1, 65536));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rf_params_v17b0_s::unpack(cbit_ref& bref)
{
  bool supported_band_list_nr_v17b0_present;
  bool supported_band_combination_list_v17b0_present;
  bool supported_band_combination_list_ul_tx_switch_v17b0_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&supported_band_list_nr_v17b0_present,
                           &supported_band_combination_list_v17b0_present,
                           &supported_band_combination_list_ul_tx_switch_v17b0_present}));

  if (supported_band_list_nr_v17b0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_list_nr_v17b0, bref, 1, 1024));
  }
  if (supported_band_combination_list_v17b0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_combination_list_v17b0, bref, 1, 65536));
  }
  if (supported_band_combination_list_ul_tx_switch_v17b0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(supported_band_combination_list_ul_tx_switch_v17b0, bref, 1, 65536));
  }

  return OCUDUASN_SUCCESS;
}
void rf_params_v17b0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (supported_band_list_nr_v17b0.size() > 0) {
    j.start_array("supportedBandListNR-v17b0");
    for (const auto& e1 : supported_band_list_nr_v17b0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (supported_band_combination_list_v17b0.size() > 0) {
    j.start_array("supportedBandCombinationList-v17b0");
    for (const auto& e1 : supported_band_combination_list_v17b0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (supported_band_combination_list_ul_tx_switch_v17b0.size() > 0) {
    j.start_array("supportedBandCombinationList-UplinkTxSwitch-v17b0");
    for (const auto& e1 : supported_band_combination_list_ul_tx_switch_v17b0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// SL-FreqConfigExt-v16k0 ::= SEQUENCE
OCUDUASN_CODE sl_freq_cfg_ext_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(add_spec_emission_r16_present, 1));

  if (add_spec_emission_r16_present) {
    HANDLE_CODE(pack_integer(bref, add_spec_emission_r16, (uint8_t)0u, (uint8_t)7u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_freq_cfg_ext_v16k0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(add_spec_emission_r16_present, 1));

  if (add_spec_emission_r16_present) {
    HANDLE_CODE(unpack_integer(add_spec_emission_r16, bref, (uint8_t)0u, (uint8_t)7u));
  }

  return OCUDUASN_SUCCESS;
}
void sl_freq_cfg_ext_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (add_spec_emission_r16_present) {
    j.write_int("additionalSpectrumEmission-r16", add_spec_emission_r16);
  }
  j.end_obj();
}

// SL-PHY-MAC-RLC-Config-v16k0 ::= SEQUENCE
OCUDUASN_CODE sl_phy_mac_rlc_cfg_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_freq_info_to_add_mod_list_ext_v16k0.size() > 0, 1));

  if (sl_freq_info_to_add_mod_list_ext_v16k0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_freq_info_to_add_mod_list_ext_v16k0, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_phy_mac_rlc_cfg_v16k0_s::unpack(cbit_ref& bref)
{
  bool sl_freq_info_to_add_mod_list_ext_v16k0_present;
  HANDLE_CODE(bref.unpack(sl_freq_info_to_add_mod_list_ext_v16k0_present, 1));

  if (sl_freq_info_to_add_mod_list_ext_v16k0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_freq_info_to_add_mod_list_ext_v16k0, bref, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
void sl_phy_mac_rlc_cfg_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_freq_info_to_add_mod_list_ext_v16k0.size() > 0) {
    j.start_array("sl-FreqInfoToAddModListExt-v16k0");
    for (const auto& e1 : sl_freq_info_to_add_mod_list_ext_v16k0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// SL-ConfigDedicatedNR-v16k0 ::= SEQUENCE
OCUDUASN_CODE sl_cfg_ded_nr_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_phy_mac_rlc_cfg_v16k0_present, 1));

  if (sl_phy_mac_rlc_cfg_v16k0_present) {
    HANDLE_CODE(sl_phy_mac_rlc_cfg_v16k0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_cfg_ded_nr_v16k0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sl_phy_mac_rlc_cfg_v16k0_present, 1));

  if (sl_phy_mac_rlc_cfg_v16k0_present) {
    HANDLE_CODE(sl_phy_mac_rlc_cfg_v16k0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void sl_cfg_ded_nr_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_phy_mac_rlc_cfg_v16k0_present) {
    j.write_fieldname("sl-PHY-MAC-RLC-Config-v16k0");
    sl_phy_mac_rlc_cfg_v16k0.to_json(j);
  }
  j.end_obj();
}

// RRCReconfiguration-v16k0-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_v16k0_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {sl_cfg_ded_nr_v16k0_present, non_crit_ext_present}));

  if (sl_cfg_ded_nr_v16k0_present) {
    HANDLE_CODE(sl_cfg_ded_nr_v16k0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_v16k0_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&sl_cfg_ded_nr_v16k0_present, &non_crit_ext_present}));

  if (sl_cfg_ded_nr_v16k0_present) {
    HANDLE_CODE(sl_cfg_ded_nr_v16k0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_v16k0_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_cfg_ded_nr_v16k0_present) {
    j.write_fieldname("sl-ConfigDedicatedNR-v16k0");
    sl_cfg_ded_nr_v16k0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// RRCReconfiguration-v15t0-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_recfg_v15t0_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {late_non_crit_ext.size() > 0, non_crit_ext_present}));

  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_recfg_v15t0_ies_s::unpack(cbit_ref& bref)
{
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref, {&late_non_crit_ext_present, &non_crit_ext_present}));

  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_recfg_v15t0_ies_s::to_json(json_writer& j) const
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

// RRCSetupComplete-v17b0-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_setup_complete_v17b0_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {ul_rrc_max_capa_segments_r17_present, non_crit_ext_present}));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_setup_complete_v17b0_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&ul_rrc_max_capa_segments_r17_present, &non_crit_ext_present}));

  return OCUDUASN_SUCCESS;
}
void rrc_setup_complete_v17b0_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ul_rrc_max_capa_segments_r17_present) {
    j.write_str("ul-RRC-MaxCapaSegments-r17", "true");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// RRCSetupComplete-v16j0-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_setup_complete_v16j0_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {late_non_crit_ext.size() > 0, non_crit_ext_present}));

  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_setup_complete_v16j0_ies_s::unpack(cbit_ref& bref)
{
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref, {&late_non_crit_ext_present, &non_crit_ext_present}));

  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_setup_complete_v16j0_ies_s::to_json(json_writer& j) const
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

// RRCSetupComplete-v15s0-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_setup_complete_v15s0_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {late_non_crit_ext.size() > 0, non_crit_ext_present}));

  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_setup_complete_v15s0_ies_s::unpack(cbit_ref& bref)
{
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref, {&late_non_crit_ext_present, &non_crit_ext_present}));

  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rrc_setup_complete_v15s0_ies_s::to_json(json_writer& j) const
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

// SDT-MAC-PHY-CG-Config-r17 ::= SEQUENCE
OCUDUASN_CODE sdt_mac_phy_cg_cfg_r17_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800.is_present();
  group_flags[0] |= cg_mt_sdt_max_dur_to_next_cg_occasion_r18_present;
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         cg_sdt_cfg_lch_restrict_to_add_mod_list_r17.size() > 0,
                         cg_sdt_cfg_lch_restrict_to_release_list_r17.size() > 0,
                         cg_sdt_cfg_init_bwp_n_ul_r17_present,
                         cg_sdt_cfg_init_bwp_sul_r17_present,
                         cg_sdt_cfg_init_bwp_dl_r17_present,
                         cg_sdt_time_align_timer_r17_present,
                         cg_sdt_rsrp_thres_ssb_r17_present,
                         cg_sdt_ta_validation_cfg_r17_present,
                         cg_sdt_cs_rnti_r17_present}));

  if (cg_sdt_cfg_lch_restrict_to_add_mod_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cg_sdt_cfg_lch_restrict_to_add_mod_list_r17, 1, 32));
  }
  if (cg_sdt_cfg_lch_restrict_to_release_list_r17.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, cg_sdt_cfg_lch_restrict_to_release_list_r17, 1, 32, integer_packer<uint8_t>(1, 32)));
  }
  if (cg_sdt_cfg_init_bwp_n_ul_r17_present) {
    HANDLE_CODE(cg_sdt_cfg_init_bwp_n_ul_r17.pack(bref));
  }
  if (cg_sdt_cfg_init_bwp_sul_r17_present) {
    HANDLE_CODE(cg_sdt_cfg_init_bwp_sul_r17.pack(bref));
  }
  if (cg_sdt_cfg_init_bwp_dl_r17_present) {
    HANDLE_CODE(cg_sdt_cfg_init_bwp_dl_r17.pack(bref));
  }
  if (cg_sdt_time_align_timer_r17_present) {
    HANDLE_CODE(cg_sdt_time_align_timer_r17.pack(bref));
  }
  if (cg_sdt_rsrp_thres_ssb_r17_present) {
    HANDLE_CODE(pack_integer(bref, cg_sdt_rsrp_thres_ssb_r17, (uint8_t)0u, (uint8_t)127u));
  }
  if (cg_sdt_ta_validation_cfg_r17_present) {
    HANDLE_CODE(cg_sdt_ta_validation_cfg_r17.pack(bref));
  }
  if (cg_sdt_cs_rnti_r17_present) {
    HANDLE_CODE(pack_integer(bref, cg_sdt_cs_rnti_r17, (uint32_t)0u, (uint32_t)65535u));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800.is_present(),
                             cg_mt_sdt_max_dur_to_next_cg_occasion_r18_present}));
      if (cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800, 1, 32));
      }
      if (cg_mt_sdt_max_dur_to_next_cg_occasion_r18_present) {
        HANDLE_CODE(cg_mt_sdt_max_dur_to_next_cg_occasion_r18.pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sdt_mac_phy_cg_cfg_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool cg_sdt_cfg_lch_restrict_to_add_mod_list_r17_present;
  bool cg_sdt_cfg_lch_restrict_to_release_list_r17_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &cg_sdt_cfg_lch_restrict_to_add_mod_list_r17_present,
                           &cg_sdt_cfg_lch_restrict_to_release_list_r17_present,
                           &cg_sdt_cfg_init_bwp_n_ul_r17_present,
                           &cg_sdt_cfg_init_bwp_sul_r17_present,
                           &cg_sdt_cfg_init_bwp_dl_r17_present,
                           &cg_sdt_time_align_timer_r17_present,
                           &cg_sdt_rsrp_thres_ssb_r17_present,
                           &cg_sdt_ta_validation_cfg_r17_present,
                           &cg_sdt_cs_rnti_r17_present}));

  if (cg_sdt_cfg_lch_restrict_to_add_mod_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cg_sdt_cfg_lch_restrict_to_add_mod_list_r17, bref, 1, 32));
  }
  if (cg_sdt_cfg_lch_restrict_to_release_list_r17_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(cg_sdt_cfg_lch_restrict_to_release_list_r17, bref, 1, 32, integer_packer<uint8_t>(1, 32)));
  }
  if (cg_sdt_cfg_init_bwp_n_ul_r17_present) {
    HANDLE_CODE(cg_sdt_cfg_init_bwp_n_ul_r17.unpack(bref));
  }
  if (cg_sdt_cfg_init_bwp_sul_r17_present) {
    HANDLE_CODE(cg_sdt_cfg_init_bwp_sul_r17.unpack(bref));
  }
  if (cg_sdt_cfg_init_bwp_dl_r17_present) {
    HANDLE_CODE(cg_sdt_cfg_init_bwp_dl_r17.unpack(bref));
  }
  if (cg_sdt_time_align_timer_r17_present) {
    HANDLE_CODE(cg_sdt_time_align_timer_r17.unpack(bref));
  }
  if (cg_sdt_rsrp_thres_ssb_r17_present) {
    HANDLE_CODE(unpack_integer(cg_sdt_rsrp_thres_ssb_r17, bref, (uint8_t)0u, (uint8_t)127u));
  }
  if (cg_sdt_ta_validation_cfg_r17_present) {
    HANDLE_CODE(cg_sdt_ta_validation_cfg_r17.unpack(bref));
  }
  if (cg_sdt_cs_rnti_r17_present) {
    HANDLE_CODE(unpack_integer(cg_sdt_cs_rnti_r17, bref, (uint32_t)0u, (uint32_t)65535u));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800, bref);
      HANDLE_CODE(bref.unpack(cg_mt_sdt_max_dur_to_next_cg_occasion_r18_present, 1));
      if (cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800, bref, 1, 32));
      }
      if (cg_mt_sdt_max_dur_to_next_cg_occasion_r18_present) {
        HANDLE_CODE(cg_mt_sdt_max_dur_to_next_cg_occasion_r18.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sdt_mac_phy_cg_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (cg_sdt_cfg_lch_restrict_to_add_mod_list_r17.size() > 0) {
    j.start_array("cg-SDT-ConfigLCH-RestrictionToAddModList-r17");
    for (const auto& e1 : cg_sdt_cfg_lch_restrict_to_add_mod_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cg_sdt_cfg_lch_restrict_to_release_list_r17.size() > 0) {
    j.start_array("cg-SDT-ConfigLCH-RestrictionToReleaseList-r17");
    for (const auto& e1 : cg_sdt_cfg_lch_restrict_to_release_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (cg_sdt_cfg_init_bwp_n_ul_r17_present) {
    j.write_fieldname("cg-SDT-ConfigInitialBWP-NUL-r17");
    cg_sdt_cfg_init_bwp_n_ul_r17.to_json(j);
  }
  if (cg_sdt_cfg_init_bwp_sul_r17_present) {
    j.write_fieldname("cg-SDT-ConfigInitialBWP-SUL-r17");
    cg_sdt_cfg_init_bwp_sul_r17.to_json(j);
  }
  if (cg_sdt_cfg_init_bwp_dl_r17_present) {
    j.write_fieldname("cg-SDT-ConfigInitialBWP-DL-r17");
    cg_sdt_cfg_init_bwp_dl_r17.to_json(j);
  }
  if (cg_sdt_time_align_timer_r17_present) {
    j.write_str("cg-SDT-TimeAlignmentTimer-r17", cg_sdt_time_align_timer_r17.to_string());
  }
  if (cg_sdt_rsrp_thres_ssb_r17_present) {
    j.write_int("cg-SDT-RSRP-ThresholdSSB-r17", cg_sdt_rsrp_thres_ssb_r17);
  }
  if (cg_sdt_ta_validation_cfg_r17_present) {
    j.write_fieldname("cg-SDT-TA-ValidationConfig-r17");
    cg_sdt_ta_validation_cfg_r17.to_json(j);
  }
  if (cg_sdt_cs_rnti_r17_present) {
    j.write_int("cg-SDT-CS-RNTI-r17", cg_sdt_cs_rnti_r17);
  }
  if (cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800.is_present()) {
    j.start_array("cg-SDT-ConfigLCH-RestrictionToAddModListExt-v1800");
    for (const auto& e1 : *cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cg_mt_sdt_max_dur_to_next_cg_occasion_r18_present) {
    j.write_str("cg-MT-SDT-MaxDurationToNextCG-Occasion-r18", cg_mt_sdt_max_dur_to_next_cg_occasion_r18.to_string());
  }
  j.end_obj();
}

const char* sdt_mac_phy_cg_cfg_r17_s::cg_mt_sdt_max_dur_to_next_cg_occasion_r18_opts::to_string() const
{
  static const char* names[] = {"ms10",
                                "ms100",
                                "sec1",
                                "sec10",
                                "sec60",
                                "sec100",
                                "sec300",
                                "sec600",
                                "sec1200",
                                "sec1800",
                                "sec3600",
                                "spare5",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(names, 16, value, "sdt_mac_phy_cg_cfg_r17_s::cg_mt_sdt_max_dur_to_next_cg_occasion_r18_e_");
}

// SL-ConfigCommonNR-r16 ::= SEQUENCE
OCUDUASN_CODE sl_cfg_common_nr_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {sl_freq_info_list_r16.size() > 0,
                         sl_ue_sel_cfg_r16_present,
                         sl_nr_anchor_carrier_freq_list_r16.size() > 0,
                         sl_eutra_anchor_carrier_freq_list_r16.size() > 0,
                         sl_radio_bearer_cfg_list_r16.size() > 0,
                         sl_rlc_bearer_cfg_list_r16.size() > 0,
                         sl_meas_cfg_common_r16_present,
                         sl_csi_acquisition_r16_present,
                         sl_offset_dfn_r16_present,
                         t400_r16_present,
                         sl_max_num_consecutive_dtx_r16_present,
                         sl_ssb_prio_nr_r16_present}));

  if (sl_freq_info_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_freq_info_list_r16, 1, 8));
  }
  if (sl_ue_sel_cfg_r16_present) {
    HANDLE_CODE(sl_ue_sel_cfg_r16.pack(bref));
  }
  if (sl_nr_anchor_carrier_freq_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_nr_anchor_carrier_freq_list_r16, 1, 8, integer_packer<uint32_t>(0, 3279165)));
  }
  if (sl_eutra_anchor_carrier_freq_list_r16.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, sl_eutra_anchor_carrier_freq_list_r16, 1, 8, integer_packer<uint32_t>(0, 262143)));
  }
  if (sl_radio_bearer_cfg_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_radio_bearer_cfg_list_r16, 1, 512));
  }
  if (sl_rlc_bearer_cfg_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_rlc_bearer_cfg_list_r16, 1, 512));
  }
  if (sl_meas_cfg_common_r16_present) {
    HANDLE_CODE(sl_meas_cfg_common_r16.pack(bref));
  }
  if (sl_offset_dfn_r16_present) {
    HANDLE_CODE(pack_integer(bref, sl_offset_dfn_r16, (uint16_t)1u, (uint16_t)1000u));
  }
  if (t400_r16_present) {
    HANDLE_CODE(t400_r16.pack(bref));
  }
  if (sl_max_num_consecutive_dtx_r16_present) {
    HANDLE_CODE(sl_max_num_consecutive_dtx_r16.pack(bref));
  }
  if (sl_ssb_prio_nr_r16_present) {
    HANDLE_CODE(pack_integer(bref, sl_ssb_prio_nr_r16, (uint8_t)1u, (uint8_t)8u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_cfg_common_nr_r16_s::unpack(cbit_ref& bref)
{
  bool sl_freq_info_list_r16_present;
  bool sl_nr_anchor_carrier_freq_list_r16_present;
  bool sl_eutra_anchor_carrier_freq_list_r16_present;
  bool sl_radio_bearer_cfg_list_r16_present;
  bool sl_rlc_bearer_cfg_list_r16_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&sl_freq_info_list_r16_present,
                           &sl_ue_sel_cfg_r16_present,
                           &sl_nr_anchor_carrier_freq_list_r16_present,
                           &sl_eutra_anchor_carrier_freq_list_r16_present,
                           &sl_radio_bearer_cfg_list_r16_present,
                           &sl_rlc_bearer_cfg_list_r16_present,
                           &sl_meas_cfg_common_r16_present,
                           &sl_csi_acquisition_r16_present,
                           &sl_offset_dfn_r16_present,
                           &t400_r16_present,
                           &sl_max_num_consecutive_dtx_r16_present,
                           &sl_ssb_prio_nr_r16_present}));

  if (sl_freq_info_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_freq_info_list_r16, bref, 1, 8));
  }
  if (sl_ue_sel_cfg_r16_present) {
    HANDLE_CODE(sl_ue_sel_cfg_r16.unpack(bref));
  }
  if (sl_nr_anchor_carrier_freq_list_r16_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(sl_nr_anchor_carrier_freq_list_r16, bref, 1, 8, integer_packer<uint32_t>(0, 3279165)));
  }
  if (sl_eutra_anchor_carrier_freq_list_r16_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(sl_eutra_anchor_carrier_freq_list_r16, bref, 1, 8, integer_packer<uint32_t>(0, 262143)));
  }
  if (sl_radio_bearer_cfg_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_radio_bearer_cfg_list_r16, bref, 1, 512));
  }
  if (sl_rlc_bearer_cfg_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_rlc_bearer_cfg_list_r16, bref, 1, 512));
  }
  if (sl_meas_cfg_common_r16_present) {
    HANDLE_CODE(sl_meas_cfg_common_r16.unpack(bref));
  }
  if (sl_offset_dfn_r16_present) {
    HANDLE_CODE(unpack_integer(sl_offset_dfn_r16, bref, (uint16_t)1u, (uint16_t)1000u));
  }
  if (t400_r16_present) {
    HANDLE_CODE(t400_r16.unpack(bref));
  }
  if (sl_max_num_consecutive_dtx_r16_present) {
    HANDLE_CODE(sl_max_num_consecutive_dtx_r16.unpack(bref));
  }
  if (sl_ssb_prio_nr_r16_present) {
    HANDLE_CODE(unpack_integer(sl_ssb_prio_nr_r16, bref, (uint8_t)1u, (uint8_t)8u));
  }

  return OCUDUASN_SUCCESS;
}
void sl_cfg_common_nr_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_freq_info_list_r16.size() > 0) {
    j.start_array("sl-FreqInfoList-r16");
    for (const auto& e1 : sl_freq_info_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_ue_sel_cfg_r16_present) {
    j.write_fieldname("sl-UE-SelectedConfig-r16");
    sl_ue_sel_cfg_r16.to_json(j);
  }
  if (sl_nr_anchor_carrier_freq_list_r16.size() > 0) {
    j.start_array("sl-NR-AnchorCarrierFreqList-r16");
    for (const auto& e1 : sl_nr_anchor_carrier_freq_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_eutra_anchor_carrier_freq_list_r16.size() > 0) {
    j.start_array("sl-EUTRA-AnchorCarrierFreqList-r16");
    for (const auto& e1 : sl_eutra_anchor_carrier_freq_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_radio_bearer_cfg_list_r16.size() > 0) {
    j.start_array("sl-RadioBearerConfigList-r16");
    for (const auto& e1 : sl_radio_bearer_cfg_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_rlc_bearer_cfg_list_r16.size() > 0) {
    j.start_array("sl-RLC-BearerConfigList-r16");
    for (const auto& e1 : sl_rlc_bearer_cfg_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_meas_cfg_common_r16_present) {
    j.write_fieldname("sl-MeasConfigCommon-r16");
    sl_meas_cfg_common_r16.to_json(j);
  }
  if (sl_csi_acquisition_r16_present) {
    j.write_str("sl-CSI-Acquisition-r16", "enabled");
  }
  if (sl_offset_dfn_r16_present) {
    j.write_int("sl-OffsetDFN-r16", sl_offset_dfn_r16);
  }
  if (t400_r16_present) {
    j.write_str("t400-r16", t400_r16.to_string());
  }
  if (sl_max_num_consecutive_dtx_r16_present) {
    j.write_str("sl-MaxNumConsecutiveDTX-r16", sl_max_num_consecutive_dtx_r16.to_string());
  }
  if (sl_ssb_prio_nr_r16_present) {
    j.write_int("sl-SSB-PriorityNR-r16", sl_ssb_prio_nr_r16);
  }
  j.end_obj();
}

const char* sl_cfg_common_nr_r16_s::t400_r16_opts::to_string() const
{
  static const char* names[] = {"ms100", "ms200", "ms300", "ms400", "ms600", "ms1000", "ms1500", "ms2000"};
  return convert_enum_idx(names, 8, value, "sl_cfg_common_nr_r16_s::t400_r16_e_");
}
uint16_t sl_cfg_common_nr_r16_s::t400_r16_opts::to_number() const
{
  static const uint16_t numbers[] = {100, 200, 300, 400, 600, 1000, 1500, 2000};
  return map_enum_number(numbers, 8, value, "sl_cfg_common_nr_r16_s::t400_r16_e_");
}

const char* sl_cfg_common_nr_r16_s::sl_max_num_consecutive_dtx_r16_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n3", "n4", "n6", "n8", "n16", "n32"};
  return convert_enum_idx(names, 8, value, "sl_cfg_common_nr_r16_s::sl_max_num_consecutive_dtx_r16_e_");
}
uint8_t sl_cfg_common_nr_r16_s::sl_max_num_consecutive_dtx_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3, 4, 6, 8, 16, 32};
  return map_enum_number(numbers, 8, value, "sl_cfg_common_nr_r16_s::sl_max_num_consecutive_dtx_r16_e_");
}

// SL-DiscConfigCommon-r17 ::= SEQUENCE
OCUDUASN_CODE sl_disc_cfg_common_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(sl_relay_ue_cfg_common_r17.pack(bref));
  HANDLE_CODE(sl_remote_ue_cfg_common_r17.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_disc_cfg_common_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(sl_relay_ue_cfg_common_r17.unpack(bref));
  HANDLE_CODE(sl_remote_ue_cfg_common_r17.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void sl_disc_cfg_common_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-RelayUE-ConfigCommon-r17");
  sl_relay_ue_cfg_common_r17.to_json(j);
  j.write_fieldname("sl-RemoteUE-ConfigCommon-r17");
  sl_remote_ue_cfg_common_r17.to_json(j);
  j.end_obj();
}

// UE-TimersAndConstantsRemoteUE-r17 ::= SEQUENCE
OCUDUASN_CODE ue_timers_and_consts_remote_ue_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(
      pack_bits(bref, {false, t300_remote_ue_r17_present, t301_remote_ue_r17_present, t319_remote_ue_r17_present}));

  if (t300_remote_ue_r17_present) {
    HANDLE_CODE(t300_remote_ue_r17.pack(bref));
  }
  if (t301_remote_ue_r17_present) {
    HANDLE_CODE(t301_remote_ue_r17.pack(bref));
  }
  if (t319_remote_ue_r17_present) {
    HANDLE_CODE(t319_remote_ue_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_timers_and_consts_remote_ue_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(
      unpack_bits(bref, {&ext, &t300_remote_ue_r17_present, &t301_remote_ue_r17_present, &t319_remote_ue_r17_present}));

  if (t300_remote_ue_r17_present) {
    HANDLE_CODE(t300_remote_ue_r17.unpack(bref));
  }
  if (t301_remote_ue_r17_present) {
    HANDLE_CODE(t301_remote_ue_r17.unpack(bref));
  }
  if (t319_remote_ue_r17_present) {
    HANDLE_CODE(t319_remote_ue_r17.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_timers_and_consts_remote_ue_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (t300_remote_ue_r17_present) {
    j.write_str("t300-RemoteUE-r17", t300_remote_ue_r17.to_string());
  }
  if (t301_remote_ue_r17_present) {
    j.write_str("t301-RemoteUE-r17", t301_remote_ue_r17.to_string());
  }
  if (t319_remote_ue_r17_present) {
    j.write_str("t319-RemoteUE-r17", t319_remote_ue_r17.to_string());
  }
  j.end_obj();
}

const char* ue_timers_and_consts_remote_ue_r17_s::t300_remote_ue_r17_opts::to_string() const
{
  static const char* names[] = {"ms100", "ms200", "ms300", "ms400", "ms600", "ms1000", "ms1500", "ms2000"};
  return convert_enum_idx(names, 8, value, "ue_timers_and_consts_remote_ue_r17_s::t300_remote_ue_r17_e_");
}
uint16_t ue_timers_and_consts_remote_ue_r17_s::t300_remote_ue_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {100, 200, 300, 400, 600, 1000, 1500, 2000};
  return map_enum_number(numbers, 8, value, "ue_timers_and_consts_remote_ue_r17_s::t300_remote_ue_r17_e_");
}

const char* ue_timers_and_consts_remote_ue_r17_s::t301_remote_ue_r17_opts::to_string() const
{
  static const char* names[] = {"ms100", "ms200", "ms300", "ms400", "ms600", "ms1000", "ms1500", "ms2000"};
  return convert_enum_idx(names, 8, value, "ue_timers_and_consts_remote_ue_r17_s::t301_remote_ue_r17_e_");
}
uint16_t ue_timers_and_consts_remote_ue_r17_s::t301_remote_ue_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {100, 200, 300, 400, 600, 1000, 1500, 2000};
  return map_enum_number(numbers, 8, value, "ue_timers_and_consts_remote_ue_r17_s::t301_remote_ue_r17_e_");
}

const char* ue_timers_and_consts_remote_ue_r17_s::t319_remote_ue_r17_opts::to_string() const
{
  static const char* names[] = {"ms100", "ms200", "ms300", "ms400", "ms600", "ms1000", "ms1500", "ms2000"};
  return convert_enum_idx(names, 8, value, "ue_timers_and_consts_remote_ue_r17_s::t319_remote_ue_r17_e_");
}
uint16_t ue_timers_and_consts_remote_ue_r17_s::t319_remote_ue_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {100, 200, 300, 400, 600, 1000, 1500, 2000};
  return map_enum_number(numbers, 8, value, "ue_timers_and_consts_remote_ue_r17_s::t319_remote_ue_r17_e_");
}

// SL-DiscConfigCommon-v1800 ::= SEQUENCE
OCUDUASN_CODE sl_disc_cfg_common_v1800_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(sl_relay_ue_cfg_common_u2_u_r18.pack(bref));
  HANDLE_CODE(sl_remote_ue_cfg_common_u2_u_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_disc_cfg_common_v1800_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(sl_relay_ue_cfg_common_u2_u_r18.unpack(bref));
  HANDLE_CODE(sl_remote_ue_cfg_common_u2_u_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void sl_disc_cfg_common_v1800_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-RelayUE-ConfigCommonU2U-r18");
  sl_relay_ue_cfg_common_u2_u_r18.to_json(j);
  j.write_fieldname("sl-RemoteUE-ConfigCommonU2U-r18");
  sl_remote_ue_cfg_common_u2_u_r18.to_json(j);
  j.end_obj();
}

// SL-DiscConfigCommon-v1840 ::= SEQUENCE
OCUDUASN_CODE sl_disc_cfg_common_v1840_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(sl_relay_ue_cfg_common_u2_u_v1840.pack(bref));
  HANDLE_CODE(sl_remote_ue_cfg_common_u2_u_v1840.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_disc_cfg_common_v1840_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(sl_relay_ue_cfg_common_u2_u_v1840.unpack(bref));
  HANDLE_CODE(sl_remote_ue_cfg_common_u2_u_v1840.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void sl_disc_cfg_common_v1840_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-RelayUE-ConfigCommonU2U-v1840");
  sl_relay_ue_cfg_common_u2_u_v1840.to_json(j);
  j.write_fieldname("sl-RemoteUE-ConfigCommonU2U-v1840");
  sl_remote_ue_cfg_common_u2_u_v1840.to_json(j);
  j.end_obj();
}

// SIB12-IEs-r16 ::= SEQUENCE
OCUDUASN_CODE sib12_ies_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(3);
  group_flags[0] |= sl_drx_cfg_common_gc_bc_r17.is_present();
  group_flags[0] |= sl_disc_cfg_common_r17.is_present();
  group_flags[0] |= sl_l2_u2_n_relay_r17_present;
  group_flags[0] |= sl_non_relay_discovery_r17_present;
  group_flags[0] |= sl_l3_u2_n_relay_discovery_r17_present;
  group_flags[0] |= sl_timers_and_consts_remote_ue_r17.is_present();
  group_flags[1] |= sl_freq_info_list_size_ext_v1800.is_present();
  group_flags[1] |= sl_rlc_bearer_cfg_list_size_ext_v1800.is_present();
  group_flags[1] |= sl_sync_freq_list_r18.is_present();
  group_flags[1] |= sl_sync_tx_multi_freq_r18_present;
  group_flags[1] |= sl_max_trans_pwr_ca_r18_present;
  group_flags[1] |= sl_disc_cfg_common_v1800.is_present();
  group_flags[1] |= sl_l2_u2_u_relay_r18_present;
  group_flags[1] |= sl_l3_u2_u_relay_discovery_r18_present;
  group_flags[1] |= t400_u2_u_r18_present;
  group_flags[2] |= sl_disc_cfg_common_v1840.is_present();
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), late_non_crit_ext.size() > 0}));

  HANDLE_CODE(sl_cfg_common_nr_r16.pack(bref));
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {sl_drx_cfg_common_gc_bc_r17.is_present(),
                             sl_disc_cfg_common_r17.is_present(),
                             sl_l2_u2_n_relay_r17_present,
                             sl_non_relay_discovery_r17_present,
                             sl_l3_u2_n_relay_discovery_r17_present,
                             sl_timers_and_consts_remote_ue_r17.is_present()}));
      if (sl_drx_cfg_common_gc_bc_r17.is_present()) {
        HANDLE_CODE(sl_drx_cfg_common_gc_bc_r17->pack(bref));
      }
      if (sl_disc_cfg_common_r17.is_present()) {
        HANDLE_CODE(sl_disc_cfg_common_r17->pack(bref));
      }
      if (sl_timers_and_consts_remote_ue_r17.is_present()) {
        HANDLE_CODE(sl_timers_and_consts_remote_ue_r17->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {sl_freq_info_list_size_ext_v1800.is_present(),
                             sl_rlc_bearer_cfg_list_size_ext_v1800.is_present(),
                             sl_sync_freq_list_r18.is_present(),
                             sl_sync_tx_multi_freq_r18_present,
                             sl_max_trans_pwr_ca_r18_present,
                             sl_disc_cfg_common_v1800.is_present(),
                             sl_l2_u2_u_relay_r18_present,
                             sl_l3_u2_u_relay_discovery_r18_present,
                             t400_u2_u_r18_present}));
      if (sl_freq_info_list_size_ext_v1800.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_freq_info_list_size_ext_v1800, 1, 7));
      }
      if (sl_rlc_bearer_cfg_list_size_ext_v1800.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_rlc_bearer_cfg_list_size_ext_v1800, 1, 512));
      }
      if (sl_sync_freq_list_r18.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_sync_freq_list_r18, 1, 8, integer_packer<uint8_t>(1, 8)));
      }
      if (sl_max_trans_pwr_ca_r18_present) {
        HANDLE_CODE(pack_integer(bref, sl_max_trans_pwr_ca_r18, (int8_t)-30, (int8_t)33));
      }
      if (sl_disc_cfg_common_v1800.is_present()) {
        HANDLE_CODE(sl_disc_cfg_common_v1800->pack(bref));
      }
      if (t400_u2_u_r18_present) {
        HANDLE_CODE(t400_u2_u_r18.pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_disc_cfg_common_v1840.is_present(), 1));
      if (sl_disc_cfg_common_v1840.is_present()) {
        HANDLE_CODE(sl_disc_cfg_common_v1840->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sib12_ies_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref, {&ext, &late_non_crit_ext_present}));

  HANDLE_CODE(sl_cfg_common_nr_r16.unpack(bref));
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_drx_cfg_common_gc_bc_r17, bref);
      unpack_presence_flag(sl_disc_cfg_common_r17, bref);
      HANDLE_CODE(unpack_bits(bref,
                              {&sl_l2_u2_n_relay_r17_present,
                               &sl_non_relay_discovery_r17_present,
                               &sl_l3_u2_n_relay_discovery_r17_present}));
      unpack_presence_flag(sl_timers_and_consts_remote_ue_r17, bref);
      if (sl_drx_cfg_common_gc_bc_r17.is_present()) {
        HANDLE_CODE(sl_drx_cfg_common_gc_bc_r17->unpack(bref));
      }
      if (sl_disc_cfg_common_r17.is_present()) {
        HANDLE_CODE(sl_disc_cfg_common_r17->unpack(bref));
      }
      if (sl_timers_and_consts_remote_ue_r17.is_present()) {
        HANDLE_CODE(sl_timers_and_consts_remote_ue_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_freq_info_list_size_ext_v1800, bref);
      unpack_presence_flag(sl_rlc_bearer_cfg_list_size_ext_v1800, bref);
      unpack_presence_flag(sl_sync_freq_list_r18, bref);
      HANDLE_CODE(unpack_bits(bref, {&sl_sync_tx_multi_freq_r18_present, &sl_max_trans_pwr_ca_r18_present}));
      unpack_presence_flag(sl_disc_cfg_common_v1800, bref);
      HANDLE_CODE(unpack_bits(
          bref, {&sl_l2_u2_u_relay_r18_present, &sl_l3_u2_u_relay_discovery_r18_present, &t400_u2_u_r18_present}));
      if (sl_freq_info_list_size_ext_v1800.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_freq_info_list_size_ext_v1800, bref, 1, 7));
      }
      if (sl_rlc_bearer_cfg_list_size_ext_v1800.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_rlc_bearer_cfg_list_size_ext_v1800, bref, 1, 512));
      }
      if (sl_sync_freq_list_r18.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_sync_freq_list_r18, bref, 1, 8, integer_packer<uint8_t>(1, 8)));
      }
      if (sl_max_trans_pwr_ca_r18_present) {
        HANDLE_CODE(unpack_integer(sl_max_trans_pwr_ca_r18, bref, (int8_t)-30, (int8_t)33));
      }
      if (sl_disc_cfg_common_v1800.is_present()) {
        HANDLE_CODE(sl_disc_cfg_common_v1800->unpack(bref));
      }
      if (t400_u2_u_r18_present) {
        HANDLE_CODE(t400_u2_u_r18.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_disc_cfg_common_v1840, bref);
      if (sl_disc_cfg_common_v1840.is_present()) {
        HANDLE_CODE(sl_disc_cfg_common_v1840->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sib12_ies_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-ConfigCommonNR-r16");
  sl_cfg_common_nr_r16.to_json(j);
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  if (sl_drx_cfg_common_gc_bc_r17.is_present()) {
    j.write_fieldname("sl-DRX-ConfigCommonGC-BC-r17");
    sl_drx_cfg_common_gc_bc_r17->to_json(j);
  }
  if (sl_disc_cfg_common_r17.is_present()) {
    j.write_fieldname("sl-DiscConfigCommon-r17");
    sl_disc_cfg_common_r17->to_json(j);
  }
  if (sl_l2_u2_n_relay_r17_present) {
    j.write_str("sl-L2U2N-Relay-r17", "enabled");
  }
  if (sl_non_relay_discovery_r17_present) {
    j.write_str("sl-NonRelayDiscovery-r17", "enabled");
  }
  if (sl_l3_u2_n_relay_discovery_r17_present) {
    j.write_str("sl-L3U2N-RelayDiscovery-r17", "enabled");
  }
  if (sl_timers_and_consts_remote_ue_r17.is_present()) {
    j.write_fieldname("sl-TimersAndConstantsRemoteUE-r17");
    sl_timers_and_consts_remote_ue_r17->to_json(j);
  }
  if (sl_freq_info_list_size_ext_v1800.is_present()) {
    j.start_array("sl-FreqInfoListSizeExt-v1800");
    for (const auto& e1 : *sl_freq_info_list_size_ext_v1800) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_rlc_bearer_cfg_list_size_ext_v1800.is_present()) {
    j.start_array("sl-RLC-BearerConfigListSizeExt-v1800");
    for (const auto& e1 : *sl_rlc_bearer_cfg_list_size_ext_v1800) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_sync_freq_list_r18.is_present()) {
    j.start_array("sl-SyncFreqList-r18");
    for (const auto& e1 : *sl_sync_freq_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_sync_tx_multi_freq_r18_present) {
    j.write_str("sl-SyncTxMultiFreq-r18", "true");
  }
  if (sl_max_trans_pwr_ca_r18_present) {
    j.write_int("sl-MaxTransPowerCA-r18", sl_max_trans_pwr_ca_r18);
  }
  if (sl_disc_cfg_common_v1800.is_present()) {
    j.write_fieldname("sl-DiscConfigCommon-v1800");
    sl_disc_cfg_common_v1800->to_json(j);
  }
  if (sl_l2_u2_u_relay_r18_present) {
    j.write_str("sl-L2-U2U-Relay-r18", "enabled");
  }
  if (sl_l3_u2_u_relay_discovery_r18_present) {
    j.write_str("sl-L3-U2U-RelayDiscovery-r18", "enabled");
  }
  if (t400_u2_u_r18_present) {
    j.write_str("t400-U2U-r18", t400_u2_u_r18.to_string());
  }
  if (sl_disc_cfg_common_v1840.is_present()) {
    j.write_fieldname("sl-DiscConfigCommon-v1840");
    sl_disc_cfg_common_v1840->to_json(j);
  }
  j.end_obj();
}

const char* sib12_ies_r16_s::t400_u2_u_r18_opts::to_string() const
{
  static const char* names[] = {"ms200", "ms400", "ms600", "ms800", "ms1200", "ms2000", "ms3000", "ms4000"};
  return convert_enum_idx(names, 8, value, "sib12_ies_r16_s::t400_u2_u_r18_e_");
}
uint16_t sib12_ies_r16_s::t400_u2_u_r18_opts::to_number() const
{
  static const uint16_t numbers[] = {200, 400, 600, 800, 1200, 2000, 3000, 4000};
  return map_enum_number(numbers, 8, value, "sib12_ies_r16_s::t400_u2_u_r18_e_");
}

// SL-ConfigCommonNR-v16k0 ::= SEQUENCE
OCUDUASN_CODE sl_cfg_common_nr_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_freq_info_list_ext_v16k0.size() > 0, 1));

  if (sl_freq_info_list_ext_v16k0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_freq_info_list_ext_v16k0, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_cfg_common_nr_v16k0_s::unpack(cbit_ref& bref)
{
  bool sl_freq_info_list_ext_v16k0_present;
  HANDLE_CODE(bref.unpack(sl_freq_info_list_ext_v16k0_present, 1));

  if (sl_freq_info_list_ext_v16k0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_freq_info_list_ext_v16k0, bref, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
void sl_cfg_common_nr_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_freq_info_list_ext_v16k0.size() > 0) {
    j.start_array("sl-FreqInfoListExt-v16k0");
    for (const auto& e1 : sl_freq_info_list_ext_v16k0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// SIB12-IEs-v16k0 ::= SEQUENCE
OCUDUASN_CODE sib12_ies_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {sl_cfg_common_nr_v16k0_present, non_crit_ext_present}));

  if (sl_cfg_common_nr_v16k0_present) {
    HANDLE_CODE(sl_cfg_common_nr_v16k0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sib12_ies_v16k0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&sl_cfg_common_nr_v16k0_present, &non_crit_ext_present}));

  if (sl_cfg_common_nr_v16k0_present) {
    HANDLE_CODE(sl_cfg_common_nr_v16k0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void sib12_ies_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_cfg_common_nr_v16k0_present) {
    j.write_fieldname("sl-ConfigCommonNR-v16k0");
    sl_cfg_common_nr_v16k0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// TRS-ResourceSet-r17 ::= SEQUENCE
OCUDUASN_CODE trs_res_set_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pwr_ctrl_offset_ss_r17.pack(bref));
  HANDLE_CODE(scrambling_id_info_r17.pack(bref));
  HANDLE_CODE(pack_integer(bref, first_ofdm_symbol_in_time_domain_r17, (uint8_t)0u, (uint8_t)9u));
  HANDLE_CODE(pack_integer(bref, start_rb_r17, (uint16_t)0u, (uint16_t)274u));
  HANDLE_CODE(pack_integer(bref, nrof_rbs_r17, (uint16_t)24u, (uint16_t)276u));
  HANDLE_CODE(pack_integer(bref, ssb_idx_r17, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(periodicity_and_offset_r17.pack(bref));
  HANDLE_CODE(freq_domain_alloc_r17.pack(bref));
  HANDLE_CODE(pack_integer(bref, ind_bit_id_r17, (uint8_t)0u, (uint8_t)5u));
  HANDLE_CODE(nrof_res_r17.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trs_res_set_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(pwr_ctrl_offset_ss_r17.unpack(bref));
  HANDLE_CODE(scrambling_id_info_r17.unpack(bref));
  HANDLE_CODE(unpack_integer(first_ofdm_symbol_in_time_domain_r17, bref, (uint8_t)0u, (uint8_t)9u));
  HANDLE_CODE(unpack_integer(start_rb_r17, bref, (uint16_t)0u, (uint16_t)274u));
  HANDLE_CODE(unpack_integer(nrof_rbs_r17, bref, (uint16_t)24u, (uint16_t)276u));
  HANDLE_CODE(unpack_integer(ssb_idx_r17, bref, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(periodicity_and_offset_r17.unpack(bref));
  HANDLE_CODE(freq_domain_alloc_r17.unpack(bref));
  HANDLE_CODE(unpack_integer(ind_bit_id_r17, bref, (uint8_t)0u, (uint8_t)5u));
  HANDLE_CODE(nrof_res_r17.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void trs_res_set_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("powerControlOffsetSS-r17", pwr_ctrl_offset_ss_r17.to_string());
  j.write_fieldname("scramblingID-Info-r17");
  scrambling_id_info_r17.to_json(j);
  j.write_int("firstOFDMSymbolInTimeDomain-r17", first_ofdm_symbol_in_time_domain_r17);
  j.write_int("startingRB-r17", start_rb_r17);
  j.write_int("nrofRBs-r17", nrof_rbs_r17);
  j.write_int("ssb-Index-r17", ssb_idx_r17);
  j.write_fieldname("periodicityAndOffset-r17");
  periodicity_and_offset_r17.to_json(j);
  j.write_fieldname("frequencyDomainAllocation-r17");
  freq_domain_alloc_r17.to_json(j);
  j.write_int("indBitID-r17", ind_bit_id_r17);
  j.write_str("nrofResources-r17", nrof_res_r17.to_string());
  j.end_obj();
}

const char* trs_res_set_r17_s::pwr_ctrl_offset_ss_r17_opts::to_string() const
{
  static const char* names[] = {"db-3", "db0", "db3", "db6"};
  return convert_enum_idx(names, 4, value, "trs_res_set_r17_s::pwr_ctrl_offset_ss_r17_e_");
}
int8_t trs_res_set_r17_s::pwr_ctrl_offset_ss_r17_opts::to_number() const
{
  static const int8_t numbers[] = {-3, 0, 3, 6};
  return map_enum_number(numbers, 4, value, "trs_res_set_r17_s::pwr_ctrl_offset_ss_r17_e_");
}

void trs_res_set_r17_s::scrambling_id_info_r17_c_::destroy_()
{
  switch (type_) {
    case types::scrambling_id_per_res_list_with2_r17:
      c.destroy<scrambling_id_per_res_list_with2_r17_l_>();
      break;
    case types::scrambling_id_per_res_list_with4_r17:
      c.destroy<scrambling_id_per_res_list_with4_r17_l_>();
      break;
    default:
      break;
  }
}
void trs_res_set_r17_s::scrambling_id_info_r17_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::scrambling_id_for_common_r17:
      break;
    case types::scrambling_id_per_res_list_with2_r17:
      c.init<scrambling_id_per_res_list_with2_r17_l_>();
      break;
    case types::scrambling_id_per_res_list_with4_r17:
      c.init<scrambling_id_per_res_list_with4_r17_l_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::scrambling_id_info_r17_c_");
  }
}
trs_res_set_r17_s::scrambling_id_info_r17_c_::scrambling_id_info_r17_c_(
    const trs_res_set_r17_s::scrambling_id_info_r17_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::scrambling_id_for_common_r17:
      c.init(other.c.get<uint16_t>());
      break;
    case types::scrambling_id_per_res_list_with2_r17:
      c.init(other.c.get<scrambling_id_per_res_list_with2_r17_l_>());
      break;
    case types::scrambling_id_per_res_list_with4_r17:
      c.init(other.c.get<scrambling_id_per_res_list_with4_r17_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::scrambling_id_info_r17_c_");
  }
}
trs_res_set_r17_s::scrambling_id_info_r17_c_&
trs_res_set_r17_s::scrambling_id_info_r17_c_::operator=(const trs_res_set_r17_s::scrambling_id_info_r17_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::scrambling_id_for_common_r17:
      c.set(other.c.get<uint16_t>());
      break;
    case types::scrambling_id_per_res_list_with2_r17:
      c.set(other.c.get<scrambling_id_per_res_list_with2_r17_l_>());
      break;
    case types::scrambling_id_per_res_list_with4_r17:
      c.set(other.c.get<scrambling_id_per_res_list_with4_r17_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::scrambling_id_info_r17_c_");
  }

  return *this;
}
uint16_t& trs_res_set_r17_s::scrambling_id_info_r17_c_::set_scrambling_id_for_common_r17()
{
  set(types::scrambling_id_for_common_r17);
  return c.get<uint16_t>();
}
trs_res_set_r17_s::scrambling_id_info_r17_c_::scrambling_id_per_res_list_with2_r17_l_&
trs_res_set_r17_s::scrambling_id_info_r17_c_::set_scrambling_id_per_res_list_with2_r17()
{
  set(types::scrambling_id_per_res_list_with2_r17);
  return c.get<scrambling_id_per_res_list_with2_r17_l_>();
}
trs_res_set_r17_s::scrambling_id_info_r17_c_::scrambling_id_per_res_list_with4_r17_l_&
trs_res_set_r17_s::scrambling_id_info_r17_c_::set_scrambling_id_per_res_list_with4_r17()
{
  set(types::scrambling_id_per_res_list_with4_r17);
  return c.get<scrambling_id_per_res_list_with4_r17_l_>();
}
void trs_res_set_r17_s::scrambling_id_info_r17_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::scrambling_id_for_common_r17:
      j.write_int("scramblingIDforCommon-r17", c.get<uint16_t>());
      break;
    case types::scrambling_id_per_res_list_with2_r17:
      j.start_array("scramblingIDperResourceListWith2-r17");
      for (const auto& e1 : c.get<scrambling_id_per_res_list_with2_r17_l_>()) {
        j.write_int(e1);
      }
      j.end_array();
      break;
    case types::scrambling_id_per_res_list_with4_r17:
      j.start_array("scramblingIDperResourceListWith4-r17");
      for (const auto& e1 : c.get<scrambling_id_per_res_list_with4_r17_l_>()) {
        j.write_int(e1);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::scrambling_id_info_r17_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE trs_res_set_r17_s::scrambling_id_info_r17_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::scrambling_id_for_common_r17:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::scrambling_id_per_res_list_with2_r17:
      HANDLE_CODE(pack_fixed_seq_of(bref,
                                    c.get<scrambling_id_per_res_list_with2_r17_l_>(),
                                    c.get<scrambling_id_per_res_list_with2_r17_l_>().size(),
                                    integer_packer<uint16_t>(0, 1023)));
      break;
    case types::scrambling_id_per_res_list_with4_r17:
      HANDLE_CODE(pack_fixed_seq_of(bref,
                                    c.get<scrambling_id_per_res_list_with4_r17_l_>(),
                                    c.get<scrambling_id_per_res_list_with4_r17_l_>().size(),
                                    integer_packer<uint16_t>(0, 1023)));
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::scrambling_id_info_r17_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trs_res_set_r17_s::scrambling_id_info_r17_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::scrambling_id_for_common_r17:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::scrambling_id_per_res_list_with2_r17:
      HANDLE_CODE(unpack_fixed_seq_of(c.get<scrambling_id_per_res_list_with2_r17_l_>(),
                                      bref,
                                      c.get<scrambling_id_per_res_list_with2_r17_l_>().size(),
                                      integer_packer<uint16_t>(0, 1023)));
      break;
    case types::scrambling_id_per_res_list_with4_r17:
      HANDLE_CODE(unpack_fixed_seq_of(c.get<scrambling_id_per_res_list_with4_r17_l_>(),
                                      bref,
                                      c.get<scrambling_id_per_res_list_with4_r17_l_>().size(),
                                      integer_packer<uint16_t>(0, 1023)));
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::scrambling_id_info_r17_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trs_res_set_r17_s::scrambling_id_info_r17_c_::types_opts::to_string() const
{
  static const char* names[] = {
      "scramblingIDforCommon-r17", "scramblingIDperResourceListWith2-r17", "scramblingIDperResourceListWith4-r17"};
  return convert_enum_idx(names, 3, value, "trs_res_set_r17_s::scrambling_id_info_r17_c_::types");
}
uint8_t trs_res_set_r17_s::scrambling_id_info_r17_c_::types_opts::to_number() const
{
  switch (value) {
    case scrambling_id_per_res_list_with2_r17:
      return 2;
    case scrambling_id_per_res_list_with4_r17:
      return 4;
    default:
      invalid_enum_number(value, "trs_res_set_r17_s::scrambling_id_info_r17_c_::types");
  }
  return 0;
}

void trs_res_set_r17_s::periodicity_and_offset_r17_c_::destroy_() {}
void trs_res_set_r17_s::periodicity_and_offset_r17_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
trs_res_set_r17_s::periodicity_and_offset_r17_c_::periodicity_and_offset_r17_c_(
    const trs_res_set_r17_s::periodicity_and_offset_r17_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::slots10:
      c.init(other.c.get<uint8_t>());
      break;
    case types::slots20:
      c.init(other.c.get<uint8_t>());
      break;
    case types::slots40:
      c.init(other.c.get<uint8_t>());
      break;
    case types::slots80:
      c.init(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::periodicity_and_offset_r17_c_");
  }
}
trs_res_set_r17_s::periodicity_and_offset_r17_c_& trs_res_set_r17_s::periodicity_and_offset_r17_c_::operator=(
    const trs_res_set_r17_s::periodicity_and_offset_r17_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::slots10:
      c.set(other.c.get<uint8_t>());
      break;
    case types::slots20:
      c.set(other.c.get<uint8_t>());
      break;
    case types::slots40:
      c.set(other.c.get<uint8_t>());
      break;
    case types::slots80:
      c.set(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::periodicity_and_offset_r17_c_");
  }

  return *this;
}
uint8_t& trs_res_set_r17_s::periodicity_and_offset_r17_c_::set_slots10()
{
  set(types::slots10);
  return c.get<uint8_t>();
}
uint8_t& trs_res_set_r17_s::periodicity_and_offset_r17_c_::set_slots20()
{
  set(types::slots20);
  return c.get<uint8_t>();
}
uint8_t& trs_res_set_r17_s::periodicity_and_offset_r17_c_::set_slots40()
{
  set(types::slots40);
  return c.get<uint8_t>();
}
uint8_t& trs_res_set_r17_s::periodicity_and_offset_r17_c_::set_slots80()
{
  set(types::slots80);
  return c.get<uint8_t>();
}
void trs_res_set_r17_s::periodicity_and_offset_r17_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::slots10:
      j.write_int("slots10", c.get<uint8_t>());
      break;
    case types::slots20:
      j.write_int("slots20", c.get<uint8_t>());
      break;
    case types::slots40:
      j.write_int("slots40", c.get<uint8_t>());
      break;
    case types::slots80:
      j.write_int("slots80", c.get<uint8_t>());
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::periodicity_and_offset_r17_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE trs_res_set_r17_s::periodicity_and_offset_r17_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::slots10:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)9u));
      break;
    case types::slots20:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)19u));
      break;
    case types::slots40:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)39u));
      break;
    case types::slots80:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)79u));
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::periodicity_and_offset_r17_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trs_res_set_r17_s::periodicity_and_offset_r17_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::slots10:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)9u));
      break;
    case types::slots20:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)19u));
      break;
    case types::slots40:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)39u));
      break;
    case types::slots80:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)79u));
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r17_s::periodicity_and_offset_r17_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trs_res_set_r17_s::periodicity_and_offset_r17_c_::types_opts::to_string() const
{
  static const char* names[] = {"slots10", "slots20", "slots40", "slots80"};
  return convert_enum_idx(names, 4, value, "trs_res_set_r17_s::periodicity_and_offset_r17_c_::types");
}
uint8_t trs_res_set_r17_s::periodicity_and_offset_r17_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {10, 20, 40, 80};
  return map_enum_number(numbers, 4, value, "trs_res_set_r17_s::periodicity_and_offset_r17_c_::types");
}

const char* trs_res_set_r17_s::nrof_res_r17_opts::to_string() const
{
  static const char* names[] = {"n2", "n4"};
  return convert_enum_idx(names, 2, value, "trs_res_set_r17_s::nrof_res_r17_e_");
}
uint8_t trs_res_set_r17_s::nrof_res_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4};
  return map_enum_number(numbers, 2, value, "trs_res_set_r17_s::nrof_res_r17_e_");
}

// SIB17-IEs-r17 ::= SEQUENCE
OCUDUASN_CODE sib17_ies_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, validity_dur_r17_present, late_non_crit_ext.size() > 0}));

  HANDLE_CODE(pack_dyn_seq_of(bref, trs_res_set_cfg_r17, 1, 64));
  if (validity_dur_r17_present) {
    HANDLE_CODE(validity_dur_r17.pack(bref));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sib17_ies_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref, {&ext, &validity_dur_r17_present, &late_non_crit_ext_present}));

  HANDLE_CODE(unpack_dyn_seq_of(trs_res_set_cfg_r17, bref, 1, 64));
  if (validity_dur_r17_present) {
    HANDLE_CODE(validity_dur_r17.unpack(bref));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sib17_ies_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("trs-ResourceSetConfig-r17");
  for (const auto& e1 : trs_res_set_cfg_r17) {
    e1.to_json(j);
  }
  j.end_array();
  if (validity_dur_r17_present) {
    j.write_str("validityDuration-r17", validity_dur_r17.to_string());
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  j.end_obj();
}

const char* sib17_ies_r17_s::validity_dur_r17_opts::to_string() const
{
  static const char* names[] = {"t1",
                                "t2",
                                "t4",
                                "t8",
                                "t16",
                                "t32",
                                "t64",
                                "t128",
                                "t256",
                                "t512",
                                "infinity",
                                "spare5",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(names, 16, value, "sib17_ies_r17_s::validity_dur_r17_e_");
}
int16_t sib17_ies_r17_s::validity_dur_r17_opts::to_number() const
{
  static const int16_t numbers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, -1};
  return map_enum_number(numbers, 11, value, "sib17_ies_r17_s::validity_dur_r17_e_");
}

// TRS-ResourceSet-r18 ::= SEQUENCE
OCUDUASN_CODE trs_res_set_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pwr_ctrl_offset_ss_r18.pack(bref));
  HANDLE_CODE(scrambling_id_info_r18.pack(bref));
  HANDLE_CODE(pack_integer(bref, first_ofdm_symbol_in_time_domain_r18, (uint8_t)0u, (uint8_t)9u));
  HANDLE_CODE(pack_integer(bref, start_rb_r18, (uint16_t)0u, (uint16_t)274u));
  HANDLE_CODE(pack_integer(bref, nrof_rbs_r18, (uint16_t)24u, (uint16_t)276u));
  HANDLE_CODE(pack_integer(bref, ssb_idx_r18, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(periodicity_and_offset_r18.pack(bref));
  HANDLE_CODE(freq_domain_alloc_r18.pack(bref));
  HANDLE_CODE(pack_integer(bref, ind_bit_id_r18, (uint8_t)0u, (uint8_t)5u));
  HANDLE_CODE(nrof_res_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trs_res_set_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(pwr_ctrl_offset_ss_r18.unpack(bref));
  HANDLE_CODE(scrambling_id_info_r18.unpack(bref));
  HANDLE_CODE(unpack_integer(first_ofdm_symbol_in_time_domain_r18, bref, (uint8_t)0u, (uint8_t)9u));
  HANDLE_CODE(unpack_integer(start_rb_r18, bref, (uint16_t)0u, (uint16_t)274u));
  HANDLE_CODE(unpack_integer(nrof_rbs_r18, bref, (uint16_t)24u, (uint16_t)276u));
  HANDLE_CODE(unpack_integer(ssb_idx_r18, bref, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(periodicity_and_offset_r18.unpack(bref));
  HANDLE_CODE(freq_domain_alloc_r18.unpack(bref));
  HANDLE_CODE(unpack_integer(ind_bit_id_r18, bref, (uint8_t)0u, (uint8_t)5u));
  HANDLE_CODE(nrof_res_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void trs_res_set_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("powerControlOffsetSS-r18", pwr_ctrl_offset_ss_r18.to_string());
  j.write_fieldname("scramblingID-Info-r18");
  scrambling_id_info_r18.to_json(j);
  j.write_int("firstOFDMSymbolInTimeDomain-r18", first_ofdm_symbol_in_time_domain_r18);
  j.write_int("startingRB-r18", start_rb_r18);
  j.write_int("nrofRBs-r18", nrof_rbs_r18);
  j.write_int("ssb-Index-r18", ssb_idx_r18);
  j.write_fieldname("periodicityAndOffset-r18");
  periodicity_and_offset_r18.to_json(j);
  j.write_fieldname("frequencyDomainAllocation-r18");
  freq_domain_alloc_r18.to_json(j);
  j.write_int("indBitID-r18", ind_bit_id_r18);
  j.write_str("nrofResources-r18", nrof_res_r18.to_string());
  j.end_obj();
}

const char* trs_res_set_r18_s::pwr_ctrl_offset_ss_r18_opts::to_string() const
{
  static const char* names[] = {"db-3", "db0", "db3", "db6"};
  return convert_enum_idx(names, 4, value, "trs_res_set_r18_s::pwr_ctrl_offset_ss_r18_e_");
}
int8_t trs_res_set_r18_s::pwr_ctrl_offset_ss_r18_opts::to_number() const
{
  static const int8_t numbers[] = {-3, 0, 3, 6};
  return map_enum_number(numbers, 4, value, "trs_res_set_r18_s::pwr_ctrl_offset_ss_r18_e_");
}

void trs_res_set_r18_s::scrambling_id_info_r18_c_::destroy_()
{
  switch (type_) {
    case types::scrambling_id_per_res_list_with2_r18:
      c.destroy<scrambling_id_per_res_list_with2_r18_l_>();
      break;
    case types::scrambling_id_per_res_list_with4_r18:
      c.destroy<scrambling_id_per_res_list_with4_r18_l_>();
      break;
    default:
      break;
  }
}
void trs_res_set_r18_s::scrambling_id_info_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::scrambling_id_for_common_r18:
      break;
    case types::scrambling_id_per_res_list_with2_r18:
      c.init<scrambling_id_per_res_list_with2_r18_l_>();
      break;
    case types::scrambling_id_per_res_list_with4_r18:
      c.init<scrambling_id_per_res_list_with4_r18_l_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::scrambling_id_info_r18_c_");
  }
}
trs_res_set_r18_s::scrambling_id_info_r18_c_::scrambling_id_info_r18_c_(
    const trs_res_set_r18_s::scrambling_id_info_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::scrambling_id_for_common_r18:
      c.init(other.c.get<uint16_t>());
      break;
    case types::scrambling_id_per_res_list_with2_r18:
      c.init(other.c.get<scrambling_id_per_res_list_with2_r18_l_>());
      break;
    case types::scrambling_id_per_res_list_with4_r18:
      c.init(other.c.get<scrambling_id_per_res_list_with4_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::scrambling_id_info_r18_c_");
  }
}
trs_res_set_r18_s::scrambling_id_info_r18_c_&
trs_res_set_r18_s::scrambling_id_info_r18_c_::operator=(const trs_res_set_r18_s::scrambling_id_info_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::scrambling_id_for_common_r18:
      c.set(other.c.get<uint16_t>());
      break;
    case types::scrambling_id_per_res_list_with2_r18:
      c.set(other.c.get<scrambling_id_per_res_list_with2_r18_l_>());
      break;
    case types::scrambling_id_per_res_list_with4_r18:
      c.set(other.c.get<scrambling_id_per_res_list_with4_r18_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::scrambling_id_info_r18_c_");
  }

  return *this;
}
uint16_t& trs_res_set_r18_s::scrambling_id_info_r18_c_::set_scrambling_id_for_common_r18()
{
  set(types::scrambling_id_for_common_r18);
  return c.get<uint16_t>();
}
trs_res_set_r18_s::scrambling_id_info_r18_c_::scrambling_id_per_res_list_with2_r18_l_&
trs_res_set_r18_s::scrambling_id_info_r18_c_::set_scrambling_id_per_res_list_with2_r18()
{
  set(types::scrambling_id_per_res_list_with2_r18);
  return c.get<scrambling_id_per_res_list_with2_r18_l_>();
}
trs_res_set_r18_s::scrambling_id_info_r18_c_::scrambling_id_per_res_list_with4_r18_l_&
trs_res_set_r18_s::scrambling_id_info_r18_c_::set_scrambling_id_per_res_list_with4_r18()
{
  set(types::scrambling_id_per_res_list_with4_r18);
  return c.get<scrambling_id_per_res_list_with4_r18_l_>();
}
void trs_res_set_r18_s::scrambling_id_info_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::scrambling_id_for_common_r18:
      j.write_int("scramblingIDforCommon-r18", c.get<uint16_t>());
      break;
    case types::scrambling_id_per_res_list_with2_r18:
      j.start_array("scramblingIDperResourceListWith2-r18");
      for (const auto& e1 : c.get<scrambling_id_per_res_list_with2_r18_l_>()) {
        j.write_int(e1);
      }
      j.end_array();
      break;
    case types::scrambling_id_per_res_list_with4_r18:
      j.start_array("scramblingIDperResourceListWith4-r18");
      for (const auto& e1 : c.get<scrambling_id_per_res_list_with4_r18_l_>()) {
        j.write_int(e1);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::scrambling_id_info_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE trs_res_set_r18_s::scrambling_id_info_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::scrambling_id_for_common_r18:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::scrambling_id_per_res_list_with2_r18:
      HANDLE_CODE(pack_fixed_seq_of(bref,
                                    c.get<scrambling_id_per_res_list_with2_r18_l_>(),
                                    c.get<scrambling_id_per_res_list_with2_r18_l_>().size(),
                                    integer_packer<uint16_t>(0, 1023)));
      break;
    case types::scrambling_id_per_res_list_with4_r18:
      HANDLE_CODE(pack_fixed_seq_of(bref,
                                    c.get<scrambling_id_per_res_list_with4_r18_l_>(),
                                    c.get<scrambling_id_per_res_list_with4_r18_l_>().size(),
                                    integer_packer<uint16_t>(0, 1023)));
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::scrambling_id_info_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trs_res_set_r18_s::scrambling_id_info_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::scrambling_id_for_common_r18:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::scrambling_id_per_res_list_with2_r18:
      HANDLE_CODE(unpack_fixed_seq_of(c.get<scrambling_id_per_res_list_with2_r18_l_>(),
                                      bref,
                                      c.get<scrambling_id_per_res_list_with2_r18_l_>().size(),
                                      integer_packer<uint16_t>(0, 1023)));
      break;
    case types::scrambling_id_per_res_list_with4_r18:
      HANDLE_CODE(unpack_fixed_seq_of(c.get<scrambling_id_per_res_list_with4_r18_l_>(),
                                      bref,
                                      c.get<scrambling_id_per_res_list_with4_r18_l_>().size(),
                                      integer_packer<uint16_t>(0, 1023)));
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::scrambling_id_info_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trs_res_set_r18_s::scrambling_id_info_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {
      "scramblingIDforCommon-r18", "scramblingIDperResourceListWith2-r18", "scramblingIDperResourceListWith4-r18"};
  return convert_enum_idx(names, 3, value, "trs_res_set_r18_s::scrambling_id_info_r18_c_::types");
}
uint8_t trs_res_set_r18_s::scrambling_id_info_r18_c_::types_opts::to_number() const
{
  switch (value) {
    case scrambling_id_per_res_list_with2_r18:
      return 2;
    case scrambling_id_per_res_list_with4_r18:
      return 4;
    default:
      invalid_enum_number(value, "trs_res_set_r18_s::scrambling_id_info_r18_c_::types");
  }
  return 0;
}

void trs_res_set_r18_s::periodicity_and_offset_r18_c_::destroy_() {}
void trs_res_set_r18_s::periodicity_and_offset_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
trs_res_set_r18_s::periodicity_and_offset_r18_c_::periodicity_and_offset_r18_c_(
    const trs_res_set_r18_s::periodicity_and_offset_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::slots10:
      c.init(other.c.get<uint8_t>());
      break;
    case types::slots20:
      c.init(other.c.get<uint8_t>());
      break;
    case types::slots40:
      c.init(other.c.get<uint8_t>());
      break;
    case types::slots80:
      c.init(other.c.get<uint8_t>());
      break;
    case types::slots160:
      c.init(other.c.get<uint8_t>());
      break;
    case types::slots320:
      c.init(other.c.get<uint16_t>());
      break;
    case types::slots640:
      c.init(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::periodicity_and_offset_r18_c_");
  }
}
trs_res_set_r18_s::periodicity_and_offset_r18_c_& trs_res_set_r18_s::periodicity_and_offset_r18_c_::operator=(
    const trs_res_set_r18_s::periodicity_and_offset_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::slots10:
      c.set(other.c.get<uint8_t>());
      break;
    case types::slots20:
      c.set(other.c.get<uint8_t>());
      break;
    case types::slots40:
      c.set(other.c.get<uint8_t>());
      break;
    case types::slots80:
      c.set(other.c.get<uint8_t>());
      break;
    case types::slots160:
      c.set(other.c.get<uint8_t>());
      break;
    case types::slots320:
      c.set(other.c.get<uint16_t>());
      break;
    case types::slots640:
      c.set(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::periodicity_and_offset_r18_c_");
  }

  return *this;
}
uint8_t& trs_res_set_r18_s::periodicity_and_offset_r18_c_::set_slots10()
{
  set(types::slots10);
  return c.get<uint8_t>();
}
uint8_t& trs_res_set_r18_s::periodicity_and_offset_r18_c_::set_slots20()
{
  set(types::slots20);
  return c.get<uint8_t>();
}
uint8_t& trs_res_set_r18_s::periodicity_and_offset_r18_c_::set_slots40()
{
  set(types::slots40);
  return c.get<uint8_t>();
}
uint8_t& trs_res_set_r18_s::periodicity_and_offset_r18_c_::set_slots80()
{
  set(types::slots80);
  return c.get<uint8_t>();
}
uint8_t& trs_res_set_r18_s::periodicity_and_offset_r18_c_::set_slots160()
{
  set(types::slots160);
  return c.get<uint8_t>();
}
uint16_t& trs_res_set_r18_s::periodicity_and_offset_r18_c_::set_slots320()
{
  set(types::slots320);
  return c.get<uint16_t>();
}
uint16_t& trs_res_set_r18_s::periodicity_and_offset_r18_c_::set_slots640()
{
  set(types::slots640);
  return c.get<uint16_t>();
}
void trs_res_set_r18_s::periodicity_and_offset_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::slots10:
      j.write_int("slots10", c.get<uint8_t>());
      break;
    case types::slots20:
      j.write_int("slots20", c.get<uint8_t>());
      break;
    case types::slots40:
      j.write_int("slots40", c.get<uint8_t>());
      break;
    case types::slots80:
      j.write_int("slots80", c.get<uint8_t>());
      break;
    case types::slots160:
      j.write_int("slots160", c.get<uint8_t>());
      break;
    case types::slots320:
      j.write_int("slots320", c.get<uint16_t>());
      break;
    case types::slots640:
      j.write_int("slots640", c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::periodicity_and_offset_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE trs_res_set_r18_s::periodicity_and_offset_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::slots10:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)9u));
      break;
    case types::slots20:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)19u));
      break;
    case types::slots40:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)39u));
      break;
    case types::slots80:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)79u));
      break;
    case types::slots160:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)159u));
      break;
    case types::slots320:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)319u));
      break;
    case types::slots640:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)639u));
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::periodicity_and_offset_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trs_res_set_r18_s::periodicity_and_offset_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::slots10:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)9u));
      break;
    case types::slots20:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)19u));
      break;
    case types::slots40:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)39u));
      break;
    case types::slots80:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)79u));
      break;
    case types::slots160:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)159u));
      break;
    case types::slots320:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)319u));
      break;
    case types::slots640:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)639u));
      break;
    default:
      log_invalid_choice_id(type_, "trs_res_set_r18_s::periodicity_and_offset_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trs_res_set_r18_s::periodicity_and_offset_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"slots10", "slots20", "slots40", "slots80", "slots160", "slots320", "slots640"};
  return convert_enum_idx(names, 7, value, "trs_res_set_r18_s::periodicity_and_offset_r18_c_::types");
}
uint16_t trs_res_set_r18_s::periodicity_and_offset_r18_c_::types_opts::to_number() const
{
  static const uint16_t numbers[] = {10, 20, 40, 80, 160, 320, 640};
  return map_enum_number(numbers, 7, value, "trs_res_set_r18_s::periodicity_and_offset_r18_c_::types");
}

const char* trs_res_set_r18_s::nrof_res_r18_opts::to_string() const
{
  static const char* names[] = {"n2", "n4"};
  return convert_enum_idx(names, 2, value, "trs_res_set_r18_s::nrof_res_r18_e_");
}
uint8_t trs_res_set_r18_s::nrof_res_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4};
  return map_enum_number(numbers, 2, value, "trs_res_set_r18_s::nrof_res_r18_e_");
}

// SIB17bis-IEs-r18 ::= SEQUENCE
OCUDUASN_CODE sib17bis_ies_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(
      pack_bits(bref, {false, trs_res_set_cfg_r18.size() > 0, validity_dur_r18_present, late_non_crit_ext.size() > 0}));

  if (trs_res_set_cfg_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, trs_res_set_cfg_r18, 1, 64));
  }
  if (validity_dur_r18_present) {
    HANDLE_CODE(validity_dur_r18.pack(bref));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sib17bis_ies_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool trs_res_set_cfg_r18_present;
  bool late_non_crit_ext_present;
  HANDLE_CODE(
      unpack_bits(bref, {&ext, &trs_res_set_cfg_r18_present, &validity_dur_r18_present, &late_non_crit_ext_present}));

  if (trs_res_set_cfg_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(trs_res_set_cfg_r18, bref, 1, 64));
  }
  if (validity_dur_r18_present) {
    HANDLE_CODE(validity_dur_r18.unpack(bref));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sib17bis_ies_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (trs_res_set_cfg_r18.size() > 0) {
    j.start_array("trs-ResourceSetConfig-r18");
    for (const auto& e1 : trs_res_set_cfg_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (validity_dur_r18_present) {
    j.write_str("validityDuration-r18", validity_dur_r18.to_string());
  }
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  j.end_obj();
}

const char* sib17bis_ies_r18_s::validity_dur_r18_opts::to_string() const
{
  static const char* names[] = {"t1",
                                "t2",
                                "t4",
                                "t8",
                                "t16",
                                "t32",
                                "t64",
                                "t128",
                                "t256",
                                "t512",
                                "infinity",
                                "spare5",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(names, 16, value, "sib17bis_ies_r18_s::validity_dur_r18_e_");
}
int16_t sib17bis_ies_r18_s::validity_dur_r18_opts::to_number() const
{
  static const int16_t numbers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, -1};
  return map_enum_number(numbers, 11, value, "sib17bis_ies_r18_s::validity_dur_r18_e_");
}

// SL-PosConfigCommonNR-r18 ::= SEQUENCE
OCUDUASN_CODE sl_pos_cfg_common_nr_r18_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= sl_pos_freq_info_list_ext_v1880.is_present();
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         sl_pos_freq_info_list_r18.size() > 0,
                         sl_pos_ue_sel_cfg_r18_present,
                         sl_pos_nr_anchor_carrier_freq_list_r18.size() > 0,
                         sl_pos_meas_cfg_common_r18_present,
                         sl_pos_offset_dfn_r18_present,
                         sl_pos_ssb_prio_nr_r18_present}));

  if (sl_pos_freq_info_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_pos_freq_info_list_r18, 1, 8));
  }
  if (sl_pos_ue_sel_cfg_r18_present) {
    HANDLE_CODE(sl_pos_ue_sel_cfg_r18.pack(bref));
  }
  if (sl_pos_nr_anchor_carrier_freq_list_r18.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, sl_pos_nr_anchor_carrier_freq_list_r18, 1, 8, integer_packer<uint32_t>(0, 3279165)));
  }
  if (sl_pos_meas_cfg_common_r18_present) {
    HANDLE_CODE(sl_pos_meas_cfg_common_r18.pack(bref));
  }
  if (sl_pos_offset_dfn_r18_present) {
    HANDLE_CODE(pack_integer(bref, sl_pos_offset_dfn_r18, (uint16_t)1u, (uint16_t)1000u));
  }
  if (sl_pos_ssb_prio_nr_r18_present) {
    HANDLE_CODE(pack_integer(bref, sl_pos_ssb_prio_nr_r18, (uint8_t)1u, (uint8_t)8u));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_pos_freq_info_list_ext_v1880.is_present(), 1));
      if (sl_pos_freq_info_list_ext_v1880.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_pos_freq_info_list_ext_v1880, 1, 8));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_pos_cfg_common_nr_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool sl_pos_freq_info_list_r18_present;
  bool sl_pos_nr_anchor_carrier_freq_list_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &sl_pos_freq_info_list_r18_present,
                           &sl_pos_ue_sel_cfg_r18_present,
                           &sl_pos_nr_anchor_carrier_freq_list_r18_present,
                           &sl_pos_meas_cfg_common_r18_present,
                           &sl_pos_offset_dfn_r18_present,
                           &sl_pos_ssb_prio_nr_r18_present}));

  if (sl_pos_freq_info_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_pos_freq_info_list_r18, bref, 1, 8));
  }
  if (sl_pos_ue_sel_cfg_r18_present) {
    HANDLE_CODE(sl_pos_ue_sel_cfg_r18.unpack(bref));
  }
  if (sl_pos_nr_anchor_carrier_freq_list_r18_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(sl_pos_nr_anchor_carrier_freq_list_r18, bref, 1, 8, integer_packer<uint32_t>(0, 3279165)));
  }
  if (sl_pos_meas_cfg_common_r18_present) {
    HANDLE_CODE(sl_pos_meas_cfg_common_r18.unpack(bref));
  }
  if (sl_pos_offset_dfn_r18_present) {
    HANDLE_CODE(unpack_integer(sl_pos_offset_dfn_r18, bref, (uint16_t)1u, (uint16_t)1000u));
  }
  if (sl_pos_ssb_prio_nr_r18_present) {
    HANDLE_CODE(unpack_integer(sl_pos_ssb_prio_nr_r18, bref, (uint8_t)1u, (uint8_t)8u));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_pos_freq_info_list_ext_v1880, bref);
      if (sl_pos_freq_info_list_ext_v1880.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_pos_freq_info_list_ext_v1880, bref, 1, 8));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_pos_cfg_common_nr_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_pos_freq_info_list_r18.size() > 0) {
    j.start_array("sl-PosFreqInfoList-r18");
    for (const auto& e1 : sl_pos_freq_info_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_pos_ue_sel_cfg_r18_present) {
    j.write_fieldname("sl-PosUE-SelectedConfig-r18");
    sl_pos_ue_sel_cfg_r18.to_json(j);
  }
  if (sl_pos_nr_anchor_carrier_freq_list_r18.size() > 0) {
    j.start_array("sl-PosNR-AnchorCarrierFreqList-r18");
    for (const auto& e1 : sl_pos_nr_anchor_carrier_freq_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_pos_meas_cfg_common_r18_present) {
    j.write_fieldname("sl-PosMeasConfigCommon-r18");
    sl_pos_meas_cfg_common_r18.to_json(j);
  }
  if (sl_pos_offset_dfn_r18_present) {
    j.write_int("sl-PosOffsetDFN-r18", sl_pos_offset_dfn_r18);
  }
  if (sl_pos_ssb_prio_nr_r18_present) {
    j.write_int("sl-PosSSB-PriorityNR-r18", sl_pos_ssb_prio_nr_r18);
  }
  if (sl_pos_freq_info_list_ext_v1880.is_present()) {
    j.start_array("sl-PosFreqInfoListExt-v1880");
    for (const auto& e1 : *sl_pos_freq_info_list_ext_v1880) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// SIB23-IEs-r18 ::= SEQUENCE
OCUDUASN_CODE sib23_ies_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, late_non_crit_ext.size() > 0}));

  HANDLE_CODE(sl_pos_cfg_common_nr_r18.pack(bref));
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sib23_ies_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref, {&ext, &late_non_crit_ext_present}));

  HANDLE_CODE(sl_pos_cfg_common_nr_r18.unpack(bref));
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sib23_ies_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-PosConfigCommonNR-r18");
  sl_pos_cfg_common_nr_r18.to_json(j);
  if (late_non_crit_ext.size() > 0) {
    j.write_str("lateNonCriticalExtension", late_non_crit_ext.to_string());
  }
  j.end_obj();
}

// SL-ServingCellInfo-r17 ::= SEQUENCE
OCUDUASN_CODE sl_serving_cell_info_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, sl_pci_r17, (uint16_t)0u, (uint16_t)1007u));
  HANDLE_CODE(pack_integer(bref, sl_carrier_freq_nr_r17, (uint32_t)0u, (uint32_t)3279165u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_serving_cell_info_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(sl_pci_r17, bref, (uint16_t)0u, (uint16_t)1007u));
  HANDLE_CODE(unpack_integer(sl_carrier_freq_nr_r17, bref, (uint32_t)0u, (uint32_t)3279165u));

  return OCUDUASN_SUCCESS;
}
void sl_serving_cell_info_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sl-PhysCellId-r17", sl_pci_r17);
  j.write_int("sl-CarrierFreqNR-r17", sl_carrier_freq_nr_r17);
  j.end_obj();
}

// SRS-PosRRC-InactiveConfig-r17 ::= SEQUENCE
OCUDUASN_CODE srs_pos_rrc_inactive_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {srs_pos_cfg_n_ul_r17_present,
                         srs_pos_cfg_sul_r17_present,
                         bwp_n_ul_r17_present,
                         bwp_sul_r17_present,
                         inactive_pos_srs_time_align_timer_r17_present,
                         inactive_pos_srs_rsrp_change_thres_r17_present}));

  if (srs_pos_cfg_n_ul_r17_present) {
    HANDLE_CODE(srs_pos_cfg_n_ul_r17.pack(bref));
  }
  if (srs_pos_cfg_sul_r17_present) {
    HANDLE_CODE(srs_pos_cfg_sul_r17.pack(bref));
  }
  if (bwp_n_ul_r17_present) {
    HANDLE_CODE(bwp_n_ul_r17.pack(bref));
  }
  if (bwp_sul_r17_present) {
    HANDLE_CODE(bwp_sul_r17.pack(bref));
  }
  if (inactive_pos_srs_time_align_timer_r17_present) {
    HANDLE_CODE(inactive_pos_srs_time_align_timer_r17.pack(bref));
  }
  if (inactive_pos_srs_rsrp_change_thres_r17_present) {
    HANDLE_CODE(inactive_pos_srs_rsrp_change_thres_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_pos_rrc_inactive_cfg_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&srs_pos_cfg_n_ul_r17_present,
                           &srs_pos_cfg_sul_r17_present,
                           &bwp_n_ul_r17_present,
                           &bwp_sul_r17_present,
                           &inactive_pos_srs_time_align_timer_r17_present,
                           &inactive_pos_srs_rsrp_change_thres_r17_present}));

  if (srs_pos_cfg_n_ul_r17_present) {
    HANDLE_CODE(srs_pos_cfg_n_ul_r17.unpack(bref));
  }
  if (srs_pos_cfg_sul_r17_present) {
    HANDLE_CODE(srs_pos_cfg_sul_r17.unpack(bref));
  }
  if (bwp_n_ul_r17_present) {
    HANDLE_CODE(bwp_n_ul_r17.unpack(bref));
  }
  if (bwp_sul_r17_present) {
    HANDLE_CODE(bwp_sul_r17.unpack(bref));
  }
  if (inactive_pos_srs_time_align_timer_r17_present) {
    HANDLE_CODE(inactive_pos_srs_time_align_timer_r17.unpack(bref));
  }
  if (inactive_pos_srs_rsrp_change_thres_r17_present) {
    HANDLE_CODE(inactive_pos_srs_rsrp_change_thres_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void srs_pos_rrc_inactive_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_pos_cfg_n_ul_r17_present) {
    j.write_fieldname("srs-PosConfigNUL-r17");
    srs_pos_cfg_n_ul_r17.to_json(j);
  }
  if (srs_pos_cfg_sul_r17_present) {
    j.write_fieldname("srs-PosConfigSUL-r17");
    srs_pos_cfg_sul_r17.to_json(j);
  }
  if (bwp_n_ul_r17_present) {
    j.write_fieldname("bwp-NUL-r17");
    bwp_n_ul_r17.to_json(j);
  }
  if (bwp_sul_r17_present) {
    j.write_fieldname("bwp-SUL-r17");
    bwp_sul_r17.to_json(j);
  }
  if (inactive_pos_srs_time_align_timer_r17_present) {
    j.write_str("inactivePosSRS-TimeAlignmentTimer-r17", inactive_pos_srs_time_align_timer_r17.to_string());
  }
  if (inactive_pos_srs_rsrp_change_thres_r17_present) {
    j.write_str("inactivePosSRS-RSRP-ChangeThreshold-r17", inactive_pos_srs_rsrp_change_thres_r17.to_string());
  }
  j.end_obj();
}

// SRS-PosRRC-InactiveEnhancedConfig-r18 ::= SEQUENCE
OCUDUASN_CODE srs_pos_rrc_inactive_enhanced_cfg_r18_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= srs_pos_rrc_inactive_agg_bw_add_carriers_r18.is_present();
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         srs_pos_rrc_inactive_agg_bw_cfg_list_r18_present,
                         srs_pos_tx_hop_r18_present,
                         srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18_present,
                         srs_pos_rrc_inactive_validity_area_non_pre_cfg_r18_present}));

  if (srs_pos_rrc_inactive_agg_bw_cfg_list_r18_present) {
    HANDLE_CODE(srs_pos_rrc_inactive_agg_bw_cfg_list_r18.pack(bref));
  }
  if (srs_pos_tx_hop_r18_present) {
    HANDLE_CODE(srs_pos_tx_hop_r18.pack(bref));
  }
  if (srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18_present) {
    HANDLE_CODE(srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18.pack(bref));
  }
  if (srs_pos_rrc_inactive_validity_area_non_pre_cfg_r18_present) {
    HANDLE_CODE(srs_pos_rrc_inactive_validity_area_non_pre_cfg_r18.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(srs_pos_rrc_inactive_agg_bw_add_carriers_r18.is_present(), 1));
      if (srs_pos_rrc_inactive_agg_bw_add_carriers_r18.is_present()) {
        HANDLE_CODE(srs_pos_rrc_inactive_agg_bw_add_carriers_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_pos_rrc_inactive_enhanced_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &srs_pos_rrc_inactive_agg_bw_cfg_list_r18_present,
                           &srs_pos_tx_hop_r18_present,
                           &srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18_present,
                           &srs_pos_rrc_inactive_validity_area_non_pre_cfg_r18_present}));

  if (srs_pos_rrc_inactive_agg_bw_cfg_list_r18_present) {
    HANDLE_CODE(srs_pos_rrc_inactive_agg_bw_cfg_list_r18.unpack(bref));
  }
  if (srs_pos_tx_hop_r18_present) {
    HANDLE_CODE(srs_pos_tx_hop_r18.unpack(bref));
  }
  if (srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18_present) {
    HANDLE_CODE(srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18.unpack(bref));
  }
  if (srs_pos_rrc_inactive_validity_area_non_pre_cfg_r18_present) {
    HANDLE_CODE(srs_pos_rrc_inactive_validity_area_non_pre_cfg_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(srs_pos_rrc_inactive_agg_bw_add_carriers_r18, bref);
      if (srs_pos_rrc_inactive_agg_bw_add_carriers_r18.is_present()) {
        HANDLE_CODE(srs_pos_rrc_inactive_agg_bw_add_carriers_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_pos_rrc_inactive_enhanced_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_pos_rrc_inactive_agg_bw_cfg_list_r18_present) {
    j.write_fieldname("srs-PosRRC-InactiveAggBW-ConfigList-r18");
    srs_pos_rrc_inactive_agg_bw_cfg_list_r18.to_json(j);
  }
  if (srs_pos_tx_hop_r18_present) {
    j.write_fieldname("srs-PosTx-Hopping-r18");
    srs_pos_tx_hop_r18.to_json(j);
  }
  if (srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18_present) {
    j.write_fieldname("srs-PosRRC-InactiveValidityAreaPreConfigList-r18");
    srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18.to_json(j);
  }
  if (srs_pos_rrc_inactive_validity_area_non_pre_cfg_r18_present) {
    j.write_fieldname("srs-PosRRC-InactiveValidityAreaNonPreConfig-r18");
    srs_pos_rrc_inactive_validity_area_non_pre_cfg_r18.to_json(j);
  }
  if (srs_pos_rrc_inactive_agg_bw_add_carriers_r18.is_present()) {
    j.write_fieldname("srs-PosRRC-InactiveAggBW-AdditionalCarriers-r18");
    srs_pos_rrc_inactive_agg_bw_add_carriers_r18->to_json(j);
  }
  j.end_obj();
}

// UE-CapabilityRequestFilterCommon ::= SEQUENCE
OCUDUASN_CODE ue_cap_request_filt_common_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(4);
  group_flags[0] |= codebook_type_request_r16.is_present();
  group_flags[0] |= ul_tx_switch_request_r16_present;
  group_flags[1] |= requested_cell_grouping_r16.is_present();
  group_flags[2] |= fallback_group_five_request_r17_present;
  group_flags[3] |= lower_msd_request_r18.is_present();
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), mrdc_request_present}));

  if (mrdc_request_present) {
    HANDLE_CODE(pack_bits(
        bref,
        {mrdc_request.omit_en_dc_present, mrdc_request.include_nr_dc_present, mrdc_request.include_ne_dc_present}));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {codebook_type_request_r16.is_present(), ul_tx_switch_request_r16_present}));
      if (codebook_type_request_r16.is_present()) {
        HANDLE_CODE(pack_bits(bref,
                              {codebook_type_request_r16->type1_single_panel_r16_present,
                               codebook_type_request_r16->type1_multi_panel_r16_present,
                               codebook_type_request_r16->type2_r16_present,
                               codebook_type_request_r16->type2_port_sel_r16_present}));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(requested_cell_grouping_r16.is_present(), 1));
      if (requested_cell_grouping_r16.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *requested_cell_grouping_r16, 1, 32));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(fallback_group_five_request_r17_present, 1));
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(lower_msd_request_r18.is_present(), 1));
      if (lower_msd_request_r18.is_present()) {
        HANDLE_CODE(pack_bits(bref,
                              {lower_msd_request_r18->pc1dot5_r18_present,
                               lower_msd_request_r18->pc2_r18_present,
                               lower_msd_request_r18->pc3_r18_present}));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_request_filt_common_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &mrdc_request_present}));

  if (mrdc_request_present) {
    HANDLE_CODE(unpack_bits(
        bref,
        {&mrdc_request.omit_en_dc_present, &mrdc_request.include_nr_dc_present, &mrdc_request.include_ne_dc_present}));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(codebook_type_request_r16, bref);
      HANDLE_CODE(bref.unpack(ul_tx_switch_request_r16_present, 1));
      if (codebook_type_request_r16.is_present()) {
        HANDLE_CODE(unpack_bits(bref,
                                {&codebook_type_request_r16->type1_single_panel_r16_present,
                                 &codebook_type_request_r16->type1_multi_panel_r16_present,
                                 &codebook_type_request_r16->type2_r16_present,
                                 &codebook_type_request_r16->type2_port_sel_r16_present}));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(requested_cell_grouping_r16, bref);
      if (requested_cell_grouping_r16.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*requested_cell_grouping_r16, bref, 1, 32));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(fallback_group_five_request_r17_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(lower_msd_request_r18, bref);
      if (lower_msd_request_r18.is_present()) {
        HANDLE_CODE(unpack_bits(bref,
                                {&lower_msd_request_r18->pc1dot5_r18_present,
                                 &lower_msd_request_r18->pc2_r18_present,
                                 &lower_msd_request_r18->pc3_r18_present}));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_cap_request_filt_common_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mrdc_request_present) {
    j.write_fieldname("mrdc-Request");
    j.start_obj();
    if (mrdc_request.omit_en_dc_present) {
      j.write_str("omitEN-DC", "true");
    }
    if (mrdc_request.include_nr_dc_present) {
      j.write_str("includeNR-DC", "true");
    }
    if (mrdc_request.include_ne_dc_present) {
      j.write_str("includeNE-DC", "true");
    }
    j.end_obj();
  }
  if (codebook_type_request_r16.is_present()) {
    j.write_fieldname("codebookTypeRequest-r16");
    j.start_obj();
    if (codebook_type_request_r16->type1_single_panel_r16_present) {
      j.write_str("type1-SinglePanel-r16", "true");
    }
    if (codebook_type_request_r16->type1_multi_panel_r16_present) {
      j.write_str("type1-MultiPanel-r16", "true");
    }
    if (codebook_type_request_r16->type2_r16_present) {
      j.write_str("type2-r16", "true");
    }
    if (codebook_type_request_r16->type2_port_sel_r16_present) {
      j.write_str("type2-PortSelection-r16", "true");
    }
    j.end_obj();
  }
  if (ul_tx_switch_request_r16_present) {
    j.write_str("uplinkTxSwitchRequest-r16", "true");
  }
  if (requested_cell_grouping_r16.is_present()) {
    j.start_array("requestedCellGrouping-r16");
    for (const auto& e1 : *requested_cell_grouping_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (fallback_group_five_request_r17_present) {
    j.write_str("fallbackGroupFiveRequest-r17", "true");
  }
  if (lower_msd_request_r18.is_present()) {
    j.write_fieldname("lowerMSDRequest-r18");
    j.start_obj();
    if (lower_msd_request_r18->pc1dot5_r18_present) {
      j.write_str("pc1dot5-r18", "true");
    }
    if (lower_msd_request_r18->pc2_r18_present) {
      j.write_str("pc2-r18", "true");
    }
    if (lower_msd_request_r18->pc3_r18_present) {
      j.write_str("pc3-r18", "true");
    }
    j.end_obj();
  }
  j.end_obj();
}

// UE-CapabilityRequestFilterNR-v1710 ::= SEQUENCE
OCUDUASN_CODE ue_cap_request_filt_nr_v1710_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {sidelink_request_r17_present, non_crit_ext_present}));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_request_filt_nr_v1710_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&sidelink_request_r17_present, &non_crit_ext_present}));

  return OCUDUASN_SUCCESS;
}
void ue_cap_request_filt_nr_v1710_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sidelink_request_r17_present) {
    j.write_str("sidelinkRequest-r17", "true");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UE-CapabilityRequestFilterNR-v1540 ::= SEQUENCE
OCUDUASN_CODE ue_cap_request_filt_nr_v1540_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {srs_switching_time_request_present, non_crit_ext_present}));

  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_request_filt_nr_v1540_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&srs_switching_time_request_present, &non_crit_ext_present}));

  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_cap_request_filt_nr_v1540_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_switching_time_request_present) {
    j.write_str("srs-SwitchingTimeRequest", "true");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UE-CapabilityRequestFilterNR ::= SEQUENCE
OCUDUASN_CODE ue_cap_request_filt_nr_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {freq_band_list_filt.size() > 0, non_crit_ext_present}));

  if (freq_band_list_filt.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, freq_band_list_filt, 1, 1280));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_request_filt_nr_s::unpack(cbit_ref& bref)
{
  bool freq_band_list_filt_present;
  HANDLE_CODE(unpack_bits(bref, {&freq_band_list_filt_present, &non_crit_ext_present}));

  if (freq_band_list_filt_present) {
    HANDLE_CODE(unpack_dyn_seq_of(freq_band_list_filt, bref, 1, 1280));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_cap_request_filt_nr_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (freq_band_list_filt.size() > 0) {
    j.start_array("frequencyBandListFilter");
    for (const auto& e1 : freq_band_list_filt) {
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

// UE-NR-Capability-v17d0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v17d0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {feature_sets_v17d0_present, non_crit_ext_present}));

  if (feature_sets_v17d0_present) {
    HANDLE_CODE(feature_sets_v17d0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v17d0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&feature_sets_v17d0_present, &non_crit_ext_present}));

  if (feature_sets_v17d0_present) {
    HANDLE_CODE(feature_sets_v17d0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v17d0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (feature_sets_v17d0_present) {
    j.write_fieldname("featureSets-v17d0");
    feature_sets_v17d0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UE-NR-Capability-v17c0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v17c0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {mac_params_v17c0_present, non_crit_ext_present}));

  if (mac_params_v17c0_present) {
    HANDLE_CODE(mac_params_v17c0.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v17c0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&mac_params_v17c0_present, &non_crit_ext_present}));

  if (mac_params_v17c0_present) {
    HANDLE_CODE(mac_params_v17c0.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v17c0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mac_params_v17c0_present) {
    j.write_fieldname("mac-Parameters-v17c0");
    mac_params_v17c0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UE-NR-Capability-v17b0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v17b0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref,
      {mac_params_v17b0_present, rf_params_v17b0_present, ul_rrc_max_capa_segments_r17_present, non_crit_ext_present}));

  if (mac_params_v17b0_present) {
    HANDLE_CODE(mac_params_v17b0.pack(bref));
  }
  if (rf_params_v17b0_present) {
    HANDLE_CODE(rf_params_v17b0.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v17b0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&mac_params_v17b0_present,
                           &rf_params_v17b0_present,
                           &ul_rrc_max_capa_segments_r17_present,
                           &non_crit_ext_present}));

  if (mac_params_v17b0_present) {
    HANDLE_CODE(mac_params_v17b0.unpack(bref));
  }
  if (rf_params_v17b0_present) {
    HANDLE_CODE(rf_params_v17b0.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v17b0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mac_params_v17b0_present) {
    j.write_fieldname("mac-Parameters-v17b0");
    mac_params_v17b0.to_json(j);
  }
  if (rf_params_v17b0_present) {
    j.write_fieldname("rf-Parameters-v17b0");
    rf_params_v17b0.to_json(j);
  }
  if (ul_rrc_max_capa_segments_r17_present) {
    j.write_str("ul-RRC-MaxCapaSegments-r17", "supported");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UE-NR-Capability-v16j0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v16j0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {rf_params_v16j0_present, late_non_crit_ext.size() > 0, non_crit_ext_present}));

  if (rf_params_v16j0_present) {
    HANDLE_CODE(rf_params_v16j0.pack(bref));
  }
  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v16j0_s::unpack(cbit_ref& bref)
{
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref, {&rf_params_v16j0_present, &late_non_crit_ext_present, &non_crit_ext_present}));

  if (rf_params_v16j0_present) {
    HANDLE_CODE(rf_params_v16j0.unpack(bref));
  }
  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v16j0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (rf_params_v16j0_present) {
    j.write_fieldname("rf-Parameters-v16j0");
    rf_params_v16j0.to_json(j);
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

// UE-NR-Capability-v16d0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v16d0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {feature_sets_v16d0_present, non_crit_ext_present}));

  if (feature_sets_v16d0_present) {
    HANDLE_CODE(feature_sets_v16d0.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v16d0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&feature_sets_v16d0_present, &non_crit_ext_present}));

  if (feature_sets_v16d0_present) {
    HANDLE_CODE(feature_sets_v16d0.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v16d0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (feature_sets_v16d0_present) {
    j.write_fieldname("featureSets-v16d0");
    feature_sets_v16d0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UE-NR-Capability-v16c0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v16c0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {rf_params_v16c0_present, non_crit_ext_present}));

  if (rf_params_v16c0_present) {
    HANDLE_CODE(rf_params_v16c0.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v16c0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&rf_params_v16c0_present, &non_crit_ext_present}));

  if (rf_params_v16c0_present) {
    HANDLE_CODE(rf_params_v16c0.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v16c0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (rf_params_v16c0_present) {
    j.write_fieldname("rf-Parameters-v16c0");
    rf_params_v16c0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UE-NR-Capability-v16a0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v16a0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {phy_params_v16a0_present, rf_params_v16a0_present, non_crit_ext_present}));

  if (phy_params_v16a0_present) {
    HANDLE_CODE(phy_params_v16a0.pack(bref));
  }
  if (rf_params_v16a0_present) {
    HANDLE_CODE(rf_params_v16a0.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v16a0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&phy_params_v16a0_present, &rf_params_v16a0_present, &non_crit_ext_present}));

  if (phy_params_v16a0_present) {
    HANDLE_CODE(phy_params_v16a0.unpack(bref));
  }
  if (rf_params_v16a0_present) {
    HANDLE_CODE(rf_params_v16a0.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v16a0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (phy_params_v16a0_present) {
    j.write_fieldname("phy-Parameters-v16a0");
    phy_params_v16a0.to_json(j);
  }
  if (rf_params_v16a0_present) {
    j.write_fieldname("rf-Parameters-v16a0");
    rf_params_v16a0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UE-NR-Capability-v15j0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v15j0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {late_non_crit_ext.size() > 0, non_crit_ext_present}));

  if (late_non_crit_ext.size() > 0) {
    HANDLE_CODE(late_non_crit_ext.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v15j0_s::unpack(cbit_ref& bref)
{
  bool late_non_crit_ext_present;
  HANDLE_CODE(unpack_bits(bref, {&late_non_crit_ext_present, &non_crit_ext_present}));

  if (late_non_crit_ext_present) {
    HANDLE_CODE(late_non_crit_ext.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v15j0_s::to_json(json_writer& j) const
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

// UE-NR-Capability-v15g0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v15g0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {rf_params_v15g0_present, non_crit_ext_present}));

  if (rf_params_v15g0_present) {
    HANDLE_CODE(rf_params_v15g0.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v15g0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&rf_params_v15g0_present, &non_crit_ext_present}));

  if (rf_params_v15g0_present) {
    HANDLE_CODE(rf_params_v15g0.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v15g0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (rf_params_v15g0_present) {
    j.write_fieldname("rf-Parameters-v15g0");
    rf_params_v15g0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UE-NR-Capability-v15c0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v15c0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {nrdc_params_v15c0_present, partial_fr2_fallback_rx_req_present, non_crit_ext_present}));

  if (nrdc_params_v15c0_present) {
    HANDLE_CODE(nrdc_params_v15c0.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v15c0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(
      unpack_bits(bref, {&nrdc_params_v15c0_present, &partial_fr2_fallback_rx_req_present, &non_crit_ext_present}));

  if (nrdc_params_v15c0_present) {
    HANDLE_CODE(nrdc_params_v15c0.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v15c0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (nrdc_params_v15c0_present) {
    j.write_fieldname("nrdc-Parameters-v15c0");
    nrdc_params_v15c0.to_json(j);
  }
  if (partial_fr2_fallback_rx_req_present) {
    j.write_str("partialFR2-FallbackRX-Req", "true");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UE-NR-Capability-v15t0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v15t0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {feature_sets_v15t0_present, meas_and_mob_params_v15t0_present, non_crit_ext_present}));

  if (feature_sets_v15t0_present) {
    HANDLE_CODE(feature_sets_v15t0.pack(bref));
  }
  if (meas_and_mob_params_v15t0_present) {
    HANDLE_CODE(meas_and_mob_params_v15t0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v15t0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(
      unpack_bits(bref, {&feature_sets_v15t0_present, &meas_and_mob_params_v15t0_present, &non_crit_ext_present}));

  if (feature_sets_v15t0_present) {
    HANDLE_CODE(feature_sets_v15t0.unpack(bref));
  }
  if (meas_and_mob_params_v15t0_present) {
    HANDLE_CODE(meas_and_mob_params_v15t0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v15t0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (feature_sets_v15t0_present) {
    j.write_fieldname("featureSets-v15t0");
    feature_sets_v15t0.to_json(j);
  }
  if (meas_and_mob_params_v15t0_present) {
    j.write_fieldname("measAndMobParameters-v15t0");
    meas_and_mob_params_v15t0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UE-NR-Capability-v16k0 ::= SEQUENCE
OCUDUASN_CODE ue_nr_cap_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {feature_sets_v16k0_present, non_crit_ext_present}));

  if (feature_sets_v16k0_present) {
    HANDLE_CODE(feature_sets_v16k0.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_nr_cap_v16k0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&feature_sets_v16k0_present, &non_crit_ext_present}));

  if (feature_sets_v16k0_present) {
    HANDLE_CODE(feature_sets_v16k0.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_nr_cap_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (feature_sets_v16k0_present) {
    j.write_fieldname("featureSets-v16k0");
    feature_sets_v16k0.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UECapabilityEnquiry-v17b0-IEs ::= SEQUENCE
OCUDUASN_CODE ue_cap_enquiry_v17b0_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {rrc_max_capa_seg_allowed_r17_present, non_crit_ext_present}));

  if (rrc_max_capa_seg_allowed_r17_present) {
    HANDLE_CODE(pack_integer(bref, rrc_max_capa_seg_allowed_r17, (uint8_t)2u, (uint8_t)16u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_enquiry_v17b0_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&rrc_max_capa_seg_allowed_r17_present, &non_crit_ext_present}));

  if (rrc_max_capa_seg_allowed_r17_present) {
    HANDLE_CODE(unpack_integer(rrc_max_capa_seg_allowed_r17, bref, (uint8_t)2u, (uint8_t)16u));
  }

  return OCUDUASN_SUCCESS;
}
void ue_cap_enquiry_v17b0_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (rrc_max_capa_seg_allowed_r17_present) {
    j.write_int("rrc-MaxCapaSegAllowed-r17", rrc_max_capa_seg_allowed_r17);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    j.start_obj();
    j.end_obj();
  }
  j.end_obj();
}

// UECapabilityEnquiry-v1610-IEs ::= SEQUENCE
OCUDUASN_CODE ue_cap_enquiry_v1610_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {rrc_seg_allowed_r16_present, non_crit_ext_present}));

  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_enquiry_v1610_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&rrc_seg_allowed_r16_present, &non_crit_ext_present}));

  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_cap_enquiry_v1610_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (rrc_seg_allowed_r16_present) {
    j.write_str("rrc-SegAllowed-r16", "enabled");
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// UECapabilityEnquiry-v1560-IEs ::= SEQUENCE
OCUDUASN_CODE ue_cap_enquiry_v1560_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {cap_request_filt_common_present, non_crit_ext_present}));

  if (cap_request_filt_common_present) {
    HANDLE_CODE(cap_request_filt_common.pack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_cap_enquiry_v1560_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&cap_request_filt_common_present, &non_crit_ext_present}));

  if (cap_request_filt_common_present) {
    HANDLE_CODE(cap_request_filt_common.unpack(bref));
  }
  if (non_crit_ext_present) {
    HANDLE_CODE(non_crit_ext.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ue_cap_enquiry_v1560_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (cap_request_filt_common_present) {
    j.write_fieldname("capabilityRequestFilterCommon");
    cap_request_filt_common.to_json(j);
  }
  if (non_crit_ext_present) {
    j.write_fieldname("nonCriticalExtension");
    non_crit_ext.to_json(j);
  }
  j.end_obj();
}

// SL-AccessInfo-L2U2N-r17 ::= SEQUENCE
OCUDUASN_CODE sl_access_info_l2_u2_n_r17_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= sl_relay_ind_r18_present;
  HANDLE_CODE(bref.pack(group_flags.any(), 1));

  HANDLE_CODE(cell_access_related_info_r17.pack(bref));
  HANDLE_CODE(sl_serving_cell_info_r17.pack(bref));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_relay_ind_r18_present, 1));
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_access_info_l2_u2_n_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(cell_access_related_info_r17.unpack(bref));
  HANDLE_CODE(sl_serving_cell_info_r17.unpack(bref));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(sl_relay_ind_r18_present, 1));
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_access_info_l2_u2_n_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("cellAccessRelatedInfo-r17");
  cell_access_related_info_r17.to_json(j);
  j.write_fieldname("sl-ServingCellInfo-r17");
  sl_serving_cell_info_r17.to_json(j);
  if (sl_relay_ind_r18_present) {
    j.write_str("sl-RelayIndication-r18", "support");
  }
  j.end_obj();
}
