// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/rrc_nr/nr_sidelink_preconf.h"

using namespace asn1;
using namespace asn1::rrc_nr;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// SL-BWP-PoolConfigCommon-r16 ::= SEQUENCE
OCUDUASN_CODE sl_bwp_pool_cfg_common_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_rx_pool_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_tx_pool_sel_normal_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_tx_pool_exceptional_r16_present, 1));

  if (sl_rx_pool_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_rx_pool_r16, 1, 16));
  }
  if (sl_tx_pool_sel_normal_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_tx_pool_sel_normal_r16, 1, 8));
  }
  if (sl_tx_pool_exceptional_r16_present) {
    HANDLE_CODE(sl_tx_pool_exceptional_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_bwp_pool_cfg_common_r16_s::unpack(cbit_ref& bref)
{
  bool sl_rx_pool_r16_present;
  HANDLE_CODE(bref.unpack(sl_rx_pool_r16_present, 1));
  bool sl_tx_pool_sel_normal_r16_present;
  HANDLE_CODE(bref.unpack(sl_tx_pool_sel_normal_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_tx_pool_exceptional_r16_present, 1));

  if (sl_rx_pool_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_rx_pool_r16, bref, 1, 16));
  }
  if (sl_tx_pool_sel_normal_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_tx_pool_sel_normal_r16, bref, 1, 8));
  }
  if (sl_tx_pool_exceptional_r16_present) {
    HANDLE_CODE(sl_tx_pool_exceptional_r16.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void sl_bwp_pool_cfg_common_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_rx_pool_r16.size() > 0) {
    j.start_array("sl-RxPool-r16");
    for (const auto& e1 : sl_rx_pool_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_tx_pool_sel_normal_r16.size() > 0) {
    j.start_array("sl-TxPoolSelectedNormal-r16");
    for (const auto& e1 : sl_tx_pool_sel_normal_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_tx_pool_exceptional_r16_present) {
    j.write_fieldname("sl-TxPoolExceptional-r16");
    sl_tx_pool_exceptional_r16.to_json(j);
  }
  j.end_obj();
}

// SL-BWP-DiscPoolConfigCommon-r17 ::= SEQUENCE
OCUDUASN_CODE sl_bwp_disc_pool_cfg_common_r17_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_disc_rx_pool_r17.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_disc_tx_pool_sel_r17.size() > 0, 1));

  if (sl_disc_rx_pool_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_disc_rx_pool_r17, 1, 16));
  }
  if (sl_disc_tx_pool_sel_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_disc_tx_pool_sel_r17, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_bwp_disc_pool_cfg_common_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool sl_disc_rx_pool_r17_present;
  HANDLE_CODE(bref.unpack(sl_disc_rx_pool_r17_present, 1));
  bool sl_disc_tx_pool_sel_r17_present;
  HANDLE_CODE(bref.unpack(sl_disc_tx_pool_sel_r17_present, 1));

  if (sl_disc_rx_pool_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_disc_rx_pool_r17, bref, 1, 16));
  }
  if (sl_disc_tx_pool_sel_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_disc_tx_pool_sel_r17, bref, 1, 8));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_bwp_disc_pool_cfg_common_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_disc_rx_pool_r17.size() > 0) {
    j.start_array("sl-DiscRxPool-r17");
    for (const auto& e1 : sl_disc_rx_pool_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_disc_tx_pool_sel_r17.size() > 0) {
    j.start_array("sl-DiscTxPoolSelected-r17");
    for (const auto& e1 : sl_disc_tx_pool_sel_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// SL-BWP-ConfigCommon-r16 ::= SEQUENCE
OCUDUASN_CODE sl_bwp_cfg_common_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= sl_bwp_pool_cfg_common_ps_r17.is_present();
  group_flags[0] |= sl_bwp_disc_pool_cfg_common_r17.is_present();
  group_flags[1] |= sl_bwp_pool_cfg_common_a2_x_r18.is_present();
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(bref.pack(sl_bwp_generic_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_bwp_pool_cfg_common_r16_present, 1));

  if (sl_bwp_generic_r16_present) {
    HANDLE_CODE(sl_bwp_generic_r16.pack(bref));
  }
  if (sl_bwp_pool_cfg_common_r16_present) {
    HANDLE_CODE(sl_bwp_pool_cfg_common_r16.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_bwp_pool_cfg_common_ps_r17.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_bwp_disc_pool_cfg_common_r17.is_present(), 1));
      if (sl_bwp_pool_cfg_common_ps_r17.is_present()) {
        HANDLE_CODE(sl_bwp_pool_cfg_common_ps_r17->pack(bref));
      }
      if (sl_bwp_disc_pool_cfg_common_r17.is_present()) {
        HANDLE_CODE(sl_bwp_disc_pool_cfg_common_r17->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_bwp_pool_cfg_common_a2_x_r18.is_present(), 1));
      if (sl_bwp_pool_cfg_common_a2_x_r18.is_present()) {
        HANDLE_CODE(sl_bwp_pool_cfg_common_a2_x_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_bwp_cfg_common_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_bwp_generic_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_bwp_pool_cfg_common_r16_present, 1));

  if (sl_bwp_generic_r16_present) {
    HANDLE_CODE(sl_bwp_generic_r16.unpack(bref));
  }
  if (sl_bwp_pool_cfg_common_r16_present) {
    HANDLE_CODE(sl_bwp_pool_cfg_common_r16.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_bwp_pool_cfg_common_ps_r17, bref);
      unpack_presence_flag(sl_bwp_disc_pool_cfg_common_r17, bref);
      if (sl_bwp_pool_cfg_common_ps_r17.is_present()) {
        HANDLE_CODE(sl_bwp_pool_cfg_common_ps_r17->unpack(bref));
      }
      if (sl_bwp_disc_pool_cfg_common_r17.is_present()) {
        HANDLE_CODE(sl_bwp_disc_pool_cfg_common_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_bwp_pool_cfg_common_a2_x_r18, bref);
      if (sl_bwp_pool_cfg_common_a2_x_r18.is_present()) {
        HANDLE_CODE(sl_bwp_pool_cfg_common_a2_x_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_bwp_cfg_common_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_bwp_generic_r16_present) {
    j.write_fieldname("sl-BWP-Generic-r16");
    sl_bwp_generic_r16.to_json(j);
  }
  if (sl_bwp_pool_cfg_common_r16_present) {
    j.write_fieldname("sl-BWP-PoolConfigCommon-r16");
    sl_bwp_pool_cfg_common_r16.to_json(j);
  }
  if (sl_bwp_pool_cfg_common_ps_r17.is_present()) {
    j.write_fieldname("sl-BWP-PoolConfigCommonPS-r17");
    sl_bwp_pool_cfg_common_ps_r17->to_json(j);
  }
  if (sl_bwp_disc_pool_cfg_common_r17.is_present()) {
    j.write_fieldname("sl-BWP-DiscPoolConfigCommon-r17");
    sl_bwp_disc_pool_cfg_common_r17->to_json(j);
  }
  if (sl_bwp_pool_cfg_common_a2_x_r18.is_present()) {
    j.write_fieldname("sl-BWP-PoolConfigCommonA2X-r18");
    sl_bwp_pool_cfg_common_a2_x_r18->to_json(j);
  }
  j.end_obj();
}

// SL-BWP-PRS-PoolConfigCommon-r18 ::= SEQUENCE
OCUDUASN_CODE sl_bwp_prs_pool_cfg_common_r18_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_prs_rx_pool_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_prs_tx_pool_sel_normal_r18.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_prs_tx_pool_exceptional_r18_present, 1));

  if (sl_prs_rx_pool_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_prs_rx_pool_r18, 1, 16));
  }
  if (sl_prs_tx_pool_sel_normal_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_prs_tx_pool_sel_normal_r18, 1, 8));
  }
  if (sl_prs_tx_pool_exceptional_r18_present) {
    HANDLE_CODE(sl_prs_tx_pool_exceptional_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_bwp_prs_pool_cfg_common_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool sl_prs_rx_pool_r18_present;
  HANDLE_CODE(bref.unpack(sl_prs_rx_pool_r18_present, 1));
  bool sl_prs_tx_pool_sel_normal_r18_present;
  HANDLE_CODE(bref.unpack(sl_prs_tx_pool_sel_normal_r18_present, 1));
  HANDLE_CODE(bref.unpack(sl_prs_tx_pool_exceptional_r18_present, 1));

  if (sl_prs_rx_pool_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_prs_rx_pool_r18, bref, 1, 16));
  }
  if (sl_prs_tx_pool_sel_normal_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_prs_tx_pool_sel_normal_r18, bref, 1, 8));
  }
  if (sl_prs_tx_pool_exceptional_r18_present) {
    HANDLE_CODE(sl_prs_tx_pool_exceptional_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_bwp_prs_pool_cfg_common_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_prs_rx_pool_r18.size() > 0) {
    j.start_array("sl-PRS-RxPool-r18");
    for (const auto& e1 : sl_prs_rx_pool_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_prs_tx_pool_sel_normal_r18.size() > 0) {
    j.start_array("sl-PRS-TxPoolSelectedNormal-r18");
    for (const auto& e1 : sl_prs_tx_pool_sel_normal_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_prs_tx_pool_exceptional_r18_present) {
    j.write_fieldname("sl-PRS-TxPoolExceptional-r18");
    sl_prs_tx_pool_exceptional_r18.to_json(j);
  }
  j.end_obj();
}

// SL-PosBWP-ConfigCommon-r18 ::= SEQUENCE
OCUDUASN_CODE sl_pos_bwp_cfg_common_r18_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_bwp_generic_r18_present, 1));
  HANDLE_CODE(bref.pack(sl_bwp_prs_pool_cfg_common_r18_present, 1));

  if (sl_bwp_generic_r18_present) {
    HANDLE_CODE(sl_bwp_generic_r18.pack(bref));
  }
  if (sl_bwp_prs_pool_cfg_common_r18_present) {
    HANDLE_CODE(sl_bwp_prs_pool_cfg_common_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_pos_bwp_cfg_common_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_bwp_generic_r18_present, 1));
  HANDLE_CODE(bref.unpack(sl_bwp_prs_pool_cfg_common_r18_present, 1));

  if (sl_bwp_generic_r18_present) {
    HANDLE_CODE(sl_bwp_generic_r18.unpack(bref));
  }
  if (sl_bwp_prs_pool_cfg_common_r18_present) {
    HANDLE_CODE(sl_bwp_prs_pool_cfg_common_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_pos_bwp_cfg_common_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_bwp_generic_r18_present) {
    j.write_fieldname("sl-BWP-Generic-r18");
    sl_bwp_generic_r18.to_json(j);
  }
  if (sl_bwp_prs_pool_cfg_common_r18_present) {
    j.write_fieldname("sl-BWP-PRS-PoolConfigCommon-r18");
    sl_bwp_prs_pool_cfg_common_r18.to_json(j);
  }
  j.end_obj();
}

// SL-FreqConfigCommon-r16 ::= SEQUENCE
OCUDUASN_CODE sl_freq_cfg_common_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= sl_unlicensed_freq_cfg_common_r18.is_present();
  group_flags[0] |= sl_pos_bwp_list_r18.is_present();
  group_flags[1] |= add_spec_emission_v1860_present;
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(bref.pack(sl_absolute_freq_ssb_r16_present, 1));
  HANDLE_CODE(bref.pack(freq_shift7p5khz_sl_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_bwp_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_sync_prio_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_nb_as_sync_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_sync_cfg_list_r16.size() > 0, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, sl_scs_specific_carrier_list_r16, 1, 5));
  HANDLE_CODE(pack_integer(bref, sl_absolute_freq_point_a_r16, (uint32_t)0u, (uint32_t)3279165u));
  if (sl_absolute_freq_ssb_r16_present) {
    HANDLE_CODE(pack_integer(bref, sl_absolute_freq_ssb_r16, (uint32_t)0u, (uint32_t)3279165u));
  }
  HANDLE_CODE(pack_integer(bref, value_n_r16, (int8_t)-1, (int8_t)1));
  if (sl_bwp_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_bwp_list_r16, 1, 4));
  }
  if (sl_sync_prio_r16_present) {
    HANDLE_CODE(sl_sync_prio_r16.pack(bref));
  }
  if (sl_nb_as_sync_r16_present) {
    HANDLE_CODE(bref.pack(sl_nb_as_sync_r16, 1));
  }
  if (sl_sync_cfg_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_sync_cfg_list_r16, 1, 16));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_unlicensed_freq_cfg_common_r18.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_pos_bwp_list_r18.is_present(), 1));
      if (sl_unlicensed_freq_cfg_common_r18.is_present()) {
        HANDLE_CODE(bref.pack(sl_unlicensed_freq_cfg_common_r18->absence_of_any_other_technology_r18_present, 1));
        HANDLE_CODE(bref.pack(sl_unlicensed_freq_cfg_common_r18->sl_freq_sel_cfg_list_r18.size() > 0, 1));
        HANDLE_CODE(bref.pack(sl_unlicensed_freq_cfg_common_r18->sl_sync_tx_disabled_r18_present, 1));
        HANDLE_CODE(bref.pack(sl_unlicensed_freq_cfg_common_r18->sl_energy_detection_cfg_r18_present, 1));
        HANDLE_CODE(bref.pack(sl_unlicensed_freq_cfg_common_r18->ue_to_ue_cot_sharing_ed_thres_r18_present, 1));
        HANDLE_CODE(bref.pack(
            sl_unlicensed_freq_cfg_common_r18->harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18_present, 1));
        if (sl_unlicensed_freq_cfg_common_r18->sl_freq_sel_cfg_list_r18.size() > 0) {
          HANDLE_CODE(pack_dyn_seq_of(bref, sl_unlicensed_freq_cfg_common_r18->sl_freq_sel_cfg_list_r18, 1, 8));
        }
        if (sl_unlicensed_freq_cfg_common_r18->sl_energy_detection_cfg_r18_present) {
          HANDLE_CODE(sl_unlicensed_freq_cfg_common_r18->sl_energy_detection_cfg_r18.pack(bref));
        }
        if (sl_unlicensed_freq_cfg_common_r18->ue_to_ue_cot_sharing_ed_thres_r18_present) {
          HANDLE_CODE(pack_integer(
              bref, sl_unlicensed_freq_cfg_common_r18->ue_to_ue_cot_sharing_ed_thres_r18, (int8_t)-85, (int8_t)-52));
        }
        if (sl_unlicensed_freq_cfg_common_r18->harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18_present) {
          HANDLE_CODE(
              pack_integer(bref,
                           sl_unlicensed_freq_cfg_common_r18->harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18,
                           (uint8_t)10u,
                           (uint8_t)100u));
        }
      }
      if (sl_pos_bwp_list_r18.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_pos_bwp_list_r18, 1, 4));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(add_spec_emission_v1860_present, 1));
      if (add_spec_emission_v1860_present) {
        HANDLE_CODE(pack_integer(bref, add_spec_emission_v1860, (uint8_t)8u, (uint8_t)39u));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_freq_cfg_common_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_absolute_freq_ssb_r16_present, 1));
  HANDLE_CODE(bref.unpack(freq_shift7p5khz_sl_r16_present, 1));
  bool sl_bwp_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_bwp_list_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_sync_prio_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_nb_as_sync_r16_present, 1));
  bool sl_sync_cfg_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_sync_cfg_list_r16_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(sl_scs_specific_carrier_list_r16, bref, 1, 5));
  HANDLE_CODE(unpack_integer(sl_absolute_freq_point_a_r16, bref, (uint32_t)0u, (uint32_t)3279165u));
  if (sl_absolute_freq_ssb_r16_present) {
    HANDLE_CODE(unpack_integer(sl_absolute_freq_ssb_r16, bref, (uint32_t)0u, (uint32_t)3279165u));
  }
  HANDLE_CODE(unpack_integer(value_n_r16, bref, (int8_t)-1, (int8_t)1));
  if (sl_bwp_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_bwp_list_r16, bref, 1, 4));
  }
  if (sl_sync_prio_r16_present) {
    HANDLE_CODE(sl_sync_prio_r16.unpack(bref));
  }
  if (sl_nb_as_sync_r16_present) {
    HANDLE_CODE(bref.unpack(sl_nb_as_sync_r16, 1));
  }
  if (sl_sync_cfg_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_sync_cfg_list_r16, bref, 1, 16));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_unlicensed_freq_cfg_common_r18, bref);
      unpack_presence_flag(sl_pos_bwp_list_r18, bref);
      if (sl_unlicensed_freq_cfg_common_r18.is_present()) {
        HANDLE_CODE(bref.unpack(sl_unlicensed_freq_cfg_common_r18->absence_of_any_other_technology_r18_present, 1));
        bool sl_freq_sel_cfg_list_r18_present;
        HANDLE_CODE(bref.unpack(sl_freq_sel_cfg_list_r18_present, 1));
        HANDLE_CODE(bref.unpack(sl_unlicensed_freq_cfg_common_r18->sl_sync_tx_disabled_r18_present, 1));
        HANDLE_CODE(bref.unpack(sl_unlicensed_freq_cfg_common_r18->sl_energy_detection_cfg_r18_present, 1));
        HANDLE_CODE(bref.unpack(sl_unlicensed_freq_cfg_common_r18->ue_to_ue_cot_sharing_ed_thres_r18_present, 1));
        HANDLE_CODE(bref.unpack(
            sl_unlicensed_freq_cfg_common_r18->harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18_present, 1));
        if (sl_freq_sel_cfg_list_r18_present) {
          HANDLE_CODE(unpack_dyn_seq_of(sl_unlicensed_freq_cfg_common_r18->sl_freq_sel_cfg_list_r18, bref, 1, 8));
        }
        if (sl_unlicensed_freq_cfg_common_r18->sl_energy_detection_cfg_r18_present) {
          HANDLE_CODE(sl_unlicensed_freq_cfg_common_r18->sl_energy_detection_cfg_r18.unpack(bref));
        }
        if (sl_unlicensed_freq_cfg_common_r18->ue_to_ue_cot_sharing_ed_thres_r18_present) {
          HANDLE_CODE(unpack_integer(
              sl_unlicensed_freq_cfg_common_r18->ue_to_ue_cot_sharing_ed_thres_r18, bref, (int8_t)-85, (int8_t)-52));
        }
        if (sl_unlicensed_freq_cfg_common_r18->harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18_present) {
          HANDLE_CODE(
              unpack_integer(sl_unlicensed_freq_cfg_common_r18->harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18,
                             bref,
                             (uint8_t)10u,
                             (uint8_t)100u));
        }
      }
      if (sl_pos_bwp_list_r18.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_pos_bwp_list_r18, bref, 1, 4));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(add_spec_emission_v1860_present, 1));
      if (add_spec_emission_v1860_present) {
        HANDLE_CODE(unpack_integer(add_spec_emission_v1860, bref, (uint8_t)8u, (uint8_t)39u));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_freq_cfg_common_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("sl-SCS-SpecificCarrierList-r16");
  for (const auto& e1 : sl_scs_specific_carrier_list_r16) {
    e1.to_json(j);
  }
  j.end_array();
  j.write_int("sl-AbsoluteFrequencyPointA-r16", sl_absolute_freq_point_a_r16);
  if (sl_absolute_freq_ssb_r16_present) {
    j.write_int("sl-AbsoluteFrequencySSB-r16", sl_absolute_freq_ssb_r16);
  }
  if (freq_shift7p5khz_sl_r16_present) {
    j.write_str("frequencyShift7p5khzSL-r16", "true");
  }
  j.write_int("valueN-r16", value_n_r16);
  if (sl_bwp_list_r16.size() > 0) {
    j.start_array("sl-BWP-List-r16");
    for (const auto& e1 : sl_bwp_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_sync_prio_r16_present) {
    j.write_str("sl-SyncPriority-r16", sl_sync_prio_r16.to_string());
  }
  if (sl_nb_as_sync_r16_present) {
    j.write_bool("sl-NbAsSync-r16", sl_nb_as_sync_r16);
  }
  if (sl_sync_cfg_list_r16.size() > 0) {
    j.start_array("sl-SyncConfigList-r16");
    for (const auto& e1 : sl_sync_cfg_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_unlicensed_freq_cfg_common_r18.is_present()) {
    j.write_fieldname("sl-UnlicensedFreqConfigCommon-r18");
    j.start_obj();
    if (sl_unlicensed_freq_cfg_common_r18->absence_of_any_other_technology_r18_present) {
      j.write_str("absenceOfAnyOtherTechnology-r18", "true");
    }
    if (sl_unlicensed_freq_cfg_common_r18->sl_freq_sel_cfg_list_r18.size() > 0) {
      j.start_array("sl-FreqSelectionConfigList-r18");
      for (const auto& e1 : sl_unlicensed_freq_cfg_common_r18->sl_freq_sel_cfg_list_r18) {
        e1.to_json(j);
      }
      j.end_array();
    }
    if (sl_unlicensed_freq_cfg_common_r18->sl_sync_tx_disabled_r18_present) {
      j.write_str("sl-SyncTxDisabled-r18", "true");
    }
    if (sl_unlicensed_freq_cfg_common_r18->sl_energy_detection_cfg_r18_present) {
      j.write_fieldname("sl-EnergyDetectionConfig-r18");
      sl_unlicensed_freq_cfg_common_r18->sl_energy_detection_cfg_r18.to_json(j);
    }
    if (sl_unlicensed_freq_cfg_common_r18->ue_to_ue_cot_sharing_ed_thres_r18_present) {
      j.write_int("ue-ToUE-COT-SharingED-Threshold-r18",
                  sl_unlicensed_freq_cfg_common_r18->ue_to_ue_cot_sharing_ed_thres_r18);
    }
    if (sl_unlicensed_freq_cfg_common_r18->harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18_present) {
      j.write_int("harq-ACK-FeedbackRatioforCW-AdjustmentGC-Option2-r18",
                  sl_unlicensed_freq_cfg_common_r18->harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18);
    }
    j.end_obj();
  }
  if (sl_pos_bwp_list_r18.is_present()) {
    j.start_array("sl-PosBWP-List-r18");
    for (const auto& e1 : *sl_pos_bwp_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (add_spec_emission_v1860_present) {
    j.write_int("additionalSpectrumEmission-v1860", add_spec_emission_v1860);
  }
  j.end_obj();
}

const char* sl_freq_cfg_common_r16_s::sl_sync_prio_r16_opts::to_string() const
{
  static const char* names[] = {"gnss", "gnbEnb"};
  return convert_enum_idx(names, 2, value, "sl_freq_cfg_common_r16_s::sl_sync_prio_r16_e_");
}

void sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::destroy_() {}
void sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::set(
    types::options e)
{
  destroy_();
  type_ = e;
}
sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::
    sl_energy_detection_cfg_r18_c_(
        const sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::sl_max_energy_detection_thres_r18:
      c.init(other.c.get<int8_t>());
      break;
    case types::sl_energy_detection_thres_offset_r18:
      c.init(other.c.get<int8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(
          type_, "sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_");
  }
}
sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_&
sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::operator=(
    const sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::sl_max_energy_detection_thres_r18:
      c.set(other.c.get<int8_t>());
      break;
    case types::sl_energy_detection_thres_offset_r18:
      c.set(other.c.get<int8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(
          type_, "sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_");
  }

  return *this;
}
int8_t& sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::
    set_sl_max_energy_detection_thres_r18()
{
  set(types::sl_max_energy_detection_thres_r18);
  return c.get<int8_t>();
}
int8_t& sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::
    set_sl_energy_detection_thres_offset_r18()
{
  set(types::sl_energy_detection_thres_offset_r18);
  return c.get<int8_t>();
}
void sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::to_json(
    json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::sl_max_energy_detection_thres_r18:
      j.write_int("sl-MaxEnergyDetectionThreshold-r18", c.get<int8_t>());
      break;
    case types::sl_energy_detection_thres_offset_r18:
      j.write_int("sl-EnergyDetectionThresholdOffset-r18", c.get<int8_t>());
      break;
    default:
      log_invalid_choice_id(
          type_, "sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::pack(
    bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::sl_max_energy_detection_thres_r18:
      HANDLE_CODE(pack_integer(bref, c.get<int8_t>(), (int8_t)-85, (int8_t)-52));
      break;
    case types::sl_energy_detection_thres_offset_r18:
      HANDLE_CODE(pack_integer(bref, c.get<int8_t>(), (int8_t)-13, (int8_t)20));
      break;
    default:
      log_invalid_choice_id(
          type_, "sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE
sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::sl_max_energy_detection_thres_r18:
      HANDLE_CODE(unpack_integer(c.get<int8_t>(), bref, (int8_t)-85, (int8_t)-52));
      break;
    case types::sl_energy_detection_thres_offset_r18:
      HANDLE_CODE(unpack_integer(c.get<int8_t>(), bref, (int8_t)-13, (int8_t)20));
      break;
    default:
      log_invalid_choice_id(
          type_, "sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char*
sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::types_opts::to_string()
    const
{
  static const char* names[] = {"sl-MaxEnergyDetectionThreshold-r18", "sl-EnergyDetectionThresholdOffset-r18"};
  return convert_enum_idx(
      names,
      2,
      value,
      "sl_freq_cfg_common_r16_s::sl_unlicensed_freq_cfg_common_r18_s_::sl_energy_detection_cfg_r18_c_::types");
}

// SL-MeasConfigCommon-r16 ::= SEQUENCE
OCUDUASN_CODE sl_meas_cfg_common_r16_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_meas_obj_list_common_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_report_cfg_list_common_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_meas_id_list_common_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_quant_cfg_common_r16_present, 1));

  if (sl_meas_obj_list_common_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_meas_obj_list_common_r16, 1, 64));
  }
  if (sl_report_cfg_list_common_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_report_cfg_list_common_r16, 1, 64));
  }
  if (sl_meas_id_list_common_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_meas_id_list_common_r16, 1, 64));
  }
  if (sl_quant_cfg_common_r16_present) {
    HANDLE_CODE(sl_quant_cfg_common_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_meas_cfg_common_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool sl_meas_obj_list_common_r16_present;
  HANDLE_CODE(bref.unpack(sl_meas_obj_list_common_r16_present, 1));
  bool sl_report_cfg_list_common_r16_present;
  HANDLE_CODE(bref.unpack(sl_report_cfg_list_common_r16_present, 1));
  bool sl_meas_id_list_common_r16_present;
  HANDLE_CODE(bref.unpack(sl_meas_id_list_common_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_quant_cfg_common_r16_present, 1));

  if (sl_meas_obj_list_common_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_meas_obj_list_common_r16, bref, 1, 64));
  }
  if (sl_report_cfg_list_common_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_report_cfg_list_common_r16, bref, 1, 64));
  }
  if (sl_meas_id_list_common_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_meas_id_list_common_r16, bref, 1, 64));
  }
  if (sl_quant_cfg_common_r16_present) {
    HANDLE_CODE(sl_quant_cfg_common_r16.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_meas_cfg_common_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_meas_obj_list_common_r16.size() > 0) {
    j.start_array("sl-MeasObjectListCommon-r16");
    for (const auto& e1 : sl_meas_obj_list_common_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_report_cfg_list_common_r16.size() > 0) {
    j.start_array("sl-ReportConfigListCommon-r16");
    for (const auto& e1 : sl_report_cfg_list_common_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_meas_id_list_common_r16.size() > 0) {
    j.start_array("sl-MeasIdListCommon-r16");
    for (const auto& e1 : sl_meas_id_list_common_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_quant_cfg_common_r16_present) {
    j.write_fieldname("sl-QuantityConfigCommon-r16");
    sl_quant_cfg_common_r16.to_json(j);
  }
  j.end_obj();
}

// SL-FreqConfigCommonExt-v16k0 ::= SEQUENCE
OCUDUASN_CODE sl_freq_cfg_common_ext_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(add_spec_emission_r16_present, 1));

  if (add_spec_emission_r16_present) {
    HANDLE_CODE(pack_integer(bref, add_spec_emission_r16, (uint8_t)0u, (uint8_t)7u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_freq_cfg_common_ext_v16k0_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(add_spec_emission_r16_present, 1));

  if (add_spec_emission_r16_present) {
    HANDLE_CODE(unpack_integer(add_spec_emission_r16, bref, (uint8_t)0u, (uint8_t)7u));
  }

  return OCUDUASN_SUCCESS;
}
void sl_freq_cfg_common_ext_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (add_spec_emission_r16_present) {
    j.write_int("additionalSpectrumEmission-r16", add_spec_emission_r16);
  }
  j.end_obj();
}

// SL-PreconfigDiscConfig-v1800 ::= SEQUENCE
OCUDUASN_CODE sl_precfg_disc_cfg_v1800_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(sl_relay_ue_precfg_u2_u_r18.pack(bref));
  HANDLE_CODE(sl_remote_ue_precfg_u2_u_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_precfg_disc_cfg_v1800_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(sl_relay_ue_precfg_u2_u_r18.unpack(bref));
  HANDLE_CODE(sl_remote_ue_precfg_u2_u_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void sl_precfg_disc_cfg_v1800_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-RelayUE-PreconfigU2U-r18");
  sl_relay_ue_precfg_u2_u_r18.to_json(j);
  j.write_fieldname("sl-RemoteUE-PreconfigU2U-r18");
  sl_remote_ue_precfg_u2_u_r18.to_json(j);
  j.end_obj();
}

// SL-PreconfigDiscConfig-v1840 ::= SEQUENCE
OCUDUASN_CODE sl_precfg_disc_cfg_v1840_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(sl_relay_ue_precfg_u2_u_v1840.pack(bref));
  HANDLE_CODE(sl_remote_ue_precfg_u2_u_v1840.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_precfg_disc_cfg_v1840_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(sl_relay_ue_precfg_u2_u_v1840.unpack(bref));
  HANDLE_CODE(sl_remote_ue_precfg_u2_u_v1840.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void sl_precfg_disc_cfg_v1840_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sl-RelayUE-PreconfigU2U-v1840");
  sl_relay_ue_precfg_u2_u_v1840.to_json(j);
  j.write_fieldname("sl-RemoteUE-PreconfigU2U-v1840");
  sl_remote_ue_precfg_u2_u_v1840.to_json(j);
  j.end_obj();
}

// SL-PreconfigGeneral-r16 ::= SEQUENCE
OCUDUASN_CODE sl_precfg_general_r16_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sl_tdd_cfg_r16_present, 1));
  HANDLE_CODE(bref.pack(reserved_bits_r16_present, 1));

  if (sl_tdd_cfg_r16_present) {
    HANDLE_CODE(sl_tdd_cfg_r16.pack(bref));
  }
  if (reserved_bits_r16_present) {
    HANDLE_CODE(reserved_bits_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_precfg_general_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sl_tdd_cfg_r16_present, 1));
  HANDLE_CODE(bref.unpack(reserved_bits_r16_present, 1));

  if (sl_tdd_cfg_r16_present) {
    HANDLE_CODE(sl_tdd_cfg_r16.unpack(bref));
  }
  if (reserved_bits_r16_present) {
    HANDLE_CODE(reserved_bits_r16.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_precfg_general_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_tdd_cfg_r16_present) {
    j.write_fieldname("sl-TDD-Configuration-r16");
    sl_tdd_cfg_r16.to_json(j);
  }
  if (reserved_bits_r16_present) {
    j.write_fieldname("reservedBits-r16");
    reserved_bits_r16.to_json(j);
  }
  j.end_obj();
}

// SL-RoHC-Profiles-r16 ::= SEQUENCE
OCUDUASN_CODE sl_ro_hc_profiles_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(profile0x0001_r16, 1));
  HANDLE_CODE(bref.pack(profile0x0002_r16, 1));
  HANDLE_CODE(bref.pack(profile0x0003_r16, 1));
  HANDLE_CODE(bref.pack(profile0x0004_r16, 1));
  HANDLE_CODE(bref.pack(profile0x0006_r16, 1));
  HANDLE_CODE(bref.pack(profile0x0101_r16, 1));
  HANDLE_CODE(bref.pack(profile0x0102_r16, 1));
  HANDLE_CODE(bref.pack(profile0x0103_r16, 1));
  HANDLE_CODE(bref.pack(profile0x0104_r16, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_ro_hc_profiles_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(profile0x0001_r16, 1));
  HANDLE_CODE(bref.unpack(profile0x0002_r16, 1));
  HANDLE_CODE(bref.unpack(profile0x0003_r16, 1));
  HANDLE_CODE(bref.unpack(profile0x0004_r16, 1));
  HANDLE_CODE(bref.unpack(profile0x0006_r16, 1));
  HANDLE_CODE(bref.unpack(profile0x0101_r16, 1));
  HANDLE_CODE(bref.unpack(profile0x0102_r16, 1));
  HANDLE_CODE(bref.unpack(profile0x0103_r16, 1));
  HANDLE_CODE(bref.unpack(profile0x0104_r16, 1));

  return OCUDUASN_SUCCESS;
}
void sl_ro_hc_profiles_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_bool("profile0x0001-r16", profile0x0001_r16);
  j.write_bool("profile0x0002-r16", profile0x0002_r16);
  j.write_bool("profile0x0003-r16", profile0x0003_r16);
  j.write_bool("profile0x0004-r16", profile0x0004_r16);
  j.write_bool("profile0x0006-r16", profile0x0006_r16);
  j.write_bool("profile0x0101-r16", profile0x0101_r16);
  j.write_bool("profile0x0102-r16", profile0x0102_r16);
  j.write_bool("profile0x0103-r16", profile0x0103_r16);
  j.write_bool("profile0x0104-r16", profile0x0104_r16);
  j.end_obj();
}

// SL-TxProfile-r17 ::= ENUMERATED
const char* sl_tx_profile_r17_opts::to_string() const
{
  static const char* names[] = {
      "drx-Compatible", "drx-Incompatible", "spare6", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "sl_tx_profile_r17_e");
}

// SidelinkPreconfigNR-r16 ::= SEQUENCE
OCUDUASN_CODE sidelink_precfg_nr_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= sl_drx_pre_cfg_gc_bc_r17.is_present();
  group_flags[0] |= sl_tx_profile_list_r17.is_present();
  group_flags[0] |= sl_precfg_disc_cfg_r17.is_present();
  group_flags[1] |= sl_precfg_freq_info_list_size_ext_v1800.is_present();
  group_flags[1] |= sl_rlc_bearer_cfg_list_size_ext_v1800.is_present();
  group_flags[1] |= sl_sync_freq_list_r18.is_present();
  group_flags[1] |= sl_sync_tx_multi_freq_r18_present;
  group_flags[1] |= sl_precfg_disc_cfg_v1800.is_present();
  group_flags[1] |= sl_pos_precfg_freq_info_list_r18.is_present();
  group_flags[2] |= t400_u2_u_r18_present;
  group_flags[3] |= sl_precfg_disc_cfg_v1840.is_present();
  group_flags[4] |= sl_pos_precfg_freq_info_list_ext_v1880.is_present();
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(bref.pack(sl_precfg_freq_info_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_precfg_nr_anchor_carrier_freq_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_precfg_eutra_anchor_carrier_freq_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_radio_bearer_pre_cfg_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_rlc_bearer_pre_cfg_list_r16.size() > 0, 1));
  HANDLE_CODE(bref.pack(sl_meas_pre_cfg_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_offset_dfn_r16_present, 1));
  HANDLE_CODE(bref.pack(t400_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_max_num_consecutive_dtx_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_ssb_prio_nr_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_precfg_general_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_ue_sel_pre_cfg_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_csi_acquisition_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_ro_hc_profiles_r16_present, 1));
  HANDLE_CODE(bref.pack(sl_max_c_id_r16_present, 1));

  if (sl_precfg_freq_info_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_precfg_freq_info_list_r16, 1, 8));
  }
  if (sl_precfg_nr_anchor_carrier_freq_list_r16.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, sl_precfg_nr_anchor_carrier_freq_list_r16, 1, 8, integer_packer<uint32_t>(0, 3279165)));
  }
  if (sl_precfg_eutra_anchor_carrier_freq_list_r16.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, sl_precfg_eutra_anchor_carrier_freq_list_r16, 1, 8, integer_packer<uint32_t>(0, 262143)));
  }
  if (sl_radio_bearer_pre_cfg_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_radio_bearer_pre_cfg_list_r16, 1, 512));
  }
  if (sl_rlc_bearer_pre_cfg_list_r16.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_rlc_bearer_pre_cfg_list_r16, 1, 512));
  }
  if (sl_meas_pre_cfg_r16_present) {
    HANDLE_CODE(sl_meas_pre_cfg_r16.pack(bref));
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
  if (sl_precfg_general_r16_present) {
    HANDLE_CODE(sl_precfg_general_r16.pack(bref));
  }
  if (sl_ue_sel_pre_cfg_r16_present) {
    HANDLE_CODE(sl_ue_sel_pre_cfg_r16.pack(bref));
  }
  if (sl_ro_hc_profiles_r16_present) {
    HANDLE_CODE(sl_ro_hc_profiles_r16.pack(bref));
  }
  if (sl_max_c_id_r16_present) {
    HANDLE_CODE(pack_integer(bref, sl_max_c_id_r16, (uint16_t)1u, (uint16_t)16383u));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_drx_pre_cfg_gc_bc_r17.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_tx_profile_list_r17.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_precfg_disc_cfg_r17.is_present(), 1));
      if (sl_drx_pre_cfg_gc_bc_r17.is_present()) {
        HANDLE_CODE(sl_drx_pre_cfg_gc_bc_r17->pack(bref));
      }
      if (sl_tx_profile_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_tx_profile_list_r17, 1, 256));
      }
      if (sl_precfg_disc_cfg_r17.is_present()) {
        HANDLE_CODE(sl_precfg_disc_cfg_r17->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_precfg_freq_info_list_size_ext_v1800.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_rlc_bearer_cfg_list_size_ext_v1800.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_sync_freq_list_r18.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_sync_tx_multi_freq_r18_present, 1));
      HANDLE_CODE(bref.pack(sl_precfg_disc_cfg_v1800.is_present(), 1));
      HANDLE_CODE(bref.pack(sl_pos_precfg_freq_info_list_r18.is_present(), 1));
      if (sl_precfg_freq_info_list_size_ext_v1800.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_precfg_freq_info_list_size_ext_v1800, 1, 7));
      }
      if (sl_rlc_bearer_cfg_list_size_ext_v1800.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_rlc_bearer_cfg_list_size_ext_v1800, 1, 512));
      }
      if (sl_sync_freq_list_r18.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_sync_freq_list_r18, 1, 8, integer_packer<uint8_t>(1, 8)));
      }
      if (sl_precfg_disc_cfg_v1800.is_present()) {
        HANDLE_CODE(sl_precfg_disc_cfg_v1800->pack(bref));
      }
      if (sl_pos_precfg_freq_info_list_r18.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_pos_precfg_freq_info_list_r18, 1, 8));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(t400_u2_u_r18_present, 1));
      if (t400_u2_u_r18_present) {
        HANDLE_CODE(t400_u2_u_r18.pack(bref));
      }
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_precfg_disc_cfg_v1840.is_present(), 1));
      if (sl_precfg_disc_cfg_v1840.is_present()) {
        HANDLE_CODE(sl_precfg_disc_cfg_v1840->pack(bref));
      }
    }
    if (group_flags[4]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_pos_precfg_freq_info_list_ext_v1880.is_present(), 1));
      if (sl_pos_precfg_freq_info_list_ext_v1880.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *sl_pos_precfg_freq_info_list_ext_v1880, 1, 8));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sidelink_precfg_nr_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool sl_precfg_freq_info_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_precfg_freq_info_list_r16_present, 1));
  bool sl_precfg_nr_anchor_carrier_freq_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_precfg_nr_anchor_carrier_freq_list_r16_present, 1));
  bool sl_precfg_eutra_anchor_carrier_freq_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_precfg_eutra_anchor_carrier_freq_list_r16_present, 1));
  bool sl_radio_bearer_pre_cfg_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_radio_bearer_pre_cfg_list_r16_present, 1));
  bool sl_rlc_bearer_pre_cfg_list_r16_present;
  HANDLE_CODE(bref.unpack(sl_rlc_bearer_pre_cfg_list_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_meas_pre_cfg_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_offset_dfn_r16_present, 1));
  HANDLE_CODE(bref.unpack(t400_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_max_num_consecutive_dtx_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_ssb_prio_nr_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_precfg_general_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_ue_sel_pre_cfg_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_csi_acquisition_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_ro_hc_profiles_r16_present, 1));
  HANDLE_CODE(bref.unpack(sl_max_c_id_r16_present, 1));

  if (sl_precfg_freq_info_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_precfg_freq_info_list_r16, bref, 1, 8));
  }
  if (sl_precfg_nr_anchor_carrier_freq_list_r16_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(sl_precfg_nr_anchor_carrier_freq_list_r16, bref, 1, 8, integer_packer<uint32_t>(0, 3279165)));
  }
  if (sl_precfg_eutra_anchor_carrier_freq_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(
        sl_precfg_eutra_anchor_carrier_freq_list_r16, bref, 1, 8, integer_packer<uint32_t>(0, 262143)));
  }
  if (sl_radio_bearer_pre_cfg_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_radio_bearer_pre_cfg_list_r16, bref, 1, 512));
  }
  if (sl_rlc_bearer_pre_cfg_list_r16_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_rlc_bearer_pre_cfg_list_r16, bref, 1, 512));
  }
  if (sl_meas_pre_cfg_r16_present) {
    HANDLE_CODE(sl_meas_pre_cfg_r16.unpack(bref));
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
  if (sl_precfg_general_r16_present) {
    HANDLE_CODE(sl_precfg_general_r16.unpack(bref));
  }
  if (sl_ue_sel_pre_cfg_r16_present) {
    HANDLE_CODE(sl_ue_sel_pre_cfg_r16.unpack(bref));
  }
  if (sl_ro_hc_profiles_r16_present) {
    HANDLE_CODE(sl_ro_hc_profiles_r16.unpack(bref));
  }
  if (sl_max_c_id_r16_present) {
    HANDLE_CODE(unpack_integer(sl_max_c_id_r16, bref, (uint16_t)1u, (uint16_t)16383u));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_drx_pre_cfg_gc_bc_r17, bref);
      unpack_presence_flag(sl_tx_profile_list_r17, bref);
      unpack_presence_flag(sl_precfg_disc_cfg_r17, bref);
      if (sl_drx_pre_cfg_gc_bc_r17.is_present()) {
        HANDLE_CODE(sl_drx_pre_cfg_gc_bc_r17->unpack(bref));
      }
      if (sl_tx_profile_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_tx_profile_list_r17, bref, 1, 256));
      }
      if (sl_precfg_disc_cfg_r17.is_present()) {
        HANDLE_CODE(sl_precfg_disc_cfg_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_precfg_freq_info_list_size_ext_v1800, bref);
      unpack_presence_flag(sl_rlc_bearer_cfg_list_size_ext_v1800, bref);
      unpack_presence_flag(sl_sync_freq_list_r18, bref);
      HANDLE_CODE(bref.unpack(sl_sync_tx_multi_freq_r18_present, 1));
      unpack_presence_flag(sl_precfg_disc_cfg_v1800, bref);
      unpack_presence_flag(sl_pos_precfg_freq_info_list_r18, bref);
      if (sl_precfg_freq_info_list_size_ext_v1800.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_precfg_freq_info_list_size_ext_v1800, bref, 1, 7));
      }
      if (sl_rlc_bearer_cfg_list_size_ext_v1800.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_rlc_bearer_cfg_list_size_ext_v1800, bref, 1, 512));
      }
      if (sl_sync_freq_list_r18.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_sync_freq_list_r18, bref, 1, 8, integer_packer<uint8_t>(1, 8)));
      }
      if (sl_precfg_disc_cfg_v1800.is_present()) {
        HANDLE_CODE(sl_precfg_disc_cfg_v1800->unpack(bref));
      }
      if (sl_pos_precfg_freq_info_list_r18.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_pos_precfg_freq_info_list_r18, bref, 1, 8));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(t400_u2_u_r18_present, 1));
      if (t400_u2_u_r18_present) {
        HANDLE_CODE(t400_u2_u_r18.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_precfg_disc_cfg_v1840, bref);
      if (sl_precfg_disc_cfg_v1840.is_present()) {
        HANDLE_CODE(sl_precfg_disc_cfg_v1840->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_pos_precfg_freq_info_list_ext_v1880, bref);
      if (sl_pos_precfg_freq_info_list_ext_v1880.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*sl_pos_precfg_freq_info_list_ext_v1880, bref, 1, 8));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sidelink_precfg_nr_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_precfg_freq_info_list_r16.size() > 0) {
    j.start_array("sl-PreconfigFreqInfoList-r16");
    for (const auto& e1 : sl_precfg_freq_info_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_precfg_nr_anchor_carrier_freq_list_r16.size() > 0) {
    j.start_array("sl-PreconfigNR-AnchorCarrierFreqList-r16");
    for (const auto& e1 : sl_precfg_nr_anchor_carrier_freq_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_precfg_eutra_anchor_carrier_freq_list_r16.size() > 0) {
    j.start_array("sl-PreconfigEUTRA-AnchorCarrierFreqList-r16");
    for (const auto& e1 : sl_precfg_eutra_anchor_carrier_freq_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (sl_radio_bearer_pre_cfg_list_r16.size() > 0) {
    j.start_array("sl-RadioBearerPreConfigList-r16");
    for (const auto& e1 : sl_radio_bearer_pre_cfg_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_rlc_bearer_pre_cfg_list_r16.size() > 0) {
    j.start_array("sl-RLC-BearerPreConfigList-r16");
    for (const auto& e1 : sl_rlc_bearer_pre_cfg_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sl_meas_pre_cfg_r16_present) {
    j.write_fieldname("sl-MeasPreConfig-r16");
    sl_meas_pre_cfg_r16.to_json(j);
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
  if (sl_precfg_general_r16_present) {
    j.write_fieldname("sl-PreconfigGeneral-r16");
    sl_precfg_general_r16.to_json(j);
  }
  if (sl_ue_sel_pre_cfg_r16_present) {
    j.write_fieldname("sl-UE-SelectedPreConfig-r16");
    sl_ue_sel_pre_cfg_r16.to_json(j);
  }
  if (sl_csi_acquisition_r16_present) {
    j.write_str("sl-CSI-Acquisition-r16", "enabled");
  }
  if (sl_ro_hc_profiles_r16_present) {
    j.write_fieldname("sl-RoHC-Profiles-r16");
    sl_ro_hc_profiles_r16.to_json(j);
  }
  if (sl_max_c_id_r16_present) {
    j.write_int("sl-MaxCID-r16", sl_max_c_id_r16);
  }
  if (sl_drx_pre_cfg_gc_bc_r17.is_present()) {
    j.write_fieldname("sl-DRX-PreConfigGC-BC-r17");
    sl_drx_pre_cfg_gc_bc_r17->to_json(j);
  }
  if (sl_tx_profile_list_r17.is_present()) {
    j.start_array("sl-TxProfileList-r17");
    for (const auto& e1 : *sl_tx_profile_list_r17) {
      j.write_str(e1.to_string());
    }
    j.end_array();
  }
  if (sl_precfg_disc_cfg_r17.is_present()) {
    j.write_fieldname("sl-PreconfigDiscConfig-r17");
    sl_precfg_disc_cfg_r17->to_json(j);
  }
  if (sl_precfg_freq_info_list_size_ext_v1800.is_present()) {
    j.start_array("sl-PreconfigFreqInfoListSizeExt-v1800");
    for (const auto& e1 : *sl_precfg_freq_info_list_size_ext_v1800) {
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
  if (sl_precfg_disc_cfg_v1800.is_present()) {
    j.write_fieldname("sl-PreconfigDiscConfig-v1800");
    sl_precfg_disc_cfg_v1800->to_json(j);
  }
  if (sl_pos_precfg_freq_info_list_r18.is_present()) {
    j.start_array("sl-PosPreconfigFreqInfoList-r18");
    for (const auto& e1 : *sl_pos_precfg_freq_info_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (t400_u2_u_r18_present) {
    j.write_str("t400-U2U-r18", t400_u2_u_r18.to_string());
  }
  if (sl_precfg_disc_cfg_v1840.is_present()) {
    j.write_fieldname("sl-PreconfigDiscConfig-v1840");
    sl_precfg_disc_cfg_v1840->to_json(j);
  }
  if (sl_pos_precfg_freq_info_list_ext_v1880.is_present()) {
    j.start_array("sl-PosPreconfigFreqInfoListExt-v1880");
    for (const auto& e1 : *sl_pos_precfg_freq_info_list_ext_v1880) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

const char* sidelink_precfg_nr_r16_s::t400_r16_opts::to_string() const
{
  static const char* names[] = {"ms100", "ms200", "ms300", "ms400", "ms600", "ms1000", "ms1500", "ms2000"};
  return convert_enum_idx(names, 8, value, "sidelink_precfg_nr_r16_s::t400_r16_e_");
}
uint16_t sidelink_precfg_nr_r16_s::t400_r16_opts::to_number() const
{
  static const uint16_t numbers[] = {100, 200, 300, 400, 600, 1000, 1500, 2000};
  return map_enum_number(numbers, 8, value, "sidelink_precfg_nr_r16_s::t400_r16_e_");
}

const char* sidelink_precfg_nr_r16_s::sl_max_num_consecutive_dtx_r16_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n3", "n4", "n6", "n8", "n16", "n32"};
  return convert_enum_idx(names, 8, value, "sidelink_precfg_nr_r16_s::sl_max_num_consecutive_dtx_r16_e_");
}
uint8_t sidelink_precfg_nr_r16_s::sl_max_num_consecutive_dtx_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3, 4, 6, 8, 16, 32};
  return map_enum_number(numbers, 8, value, "sidelink_precfg_nr_r16_s::sl_max_num_consecutive_dtx_r16_e_");
}

const char* sidelink_precfg_nr_r16_s::t400_u2_u_r18_opts::to_string() const
{
  static const char* names[] = {"ms200", "ms400", "ms600", "ms800", "ms1200", "ms2000", "ms3000", "ms4000"};
  return convert_enum_idx(names, 8, value, "sidelink_precfg_nr_r16_s::t400_u2_u_r18_e_");
}
uint16_t sidelink_precfg_nr_r16_s::t400_u2_u_r18_opts::to_number() const
{
  static const uint16_t numbers[] = {200, 400, 600, 800, 1200, 2000, 3000, 4000};
  return map_enum_number(numbers, 8, value, "sidelink_precfg_nr_r16_s::t400_u2_u_r18_e_");
}

// SidelinkPreconfigNR-v16k0 ::= SEQUENCE
OCUDUASN_CODE sidelink_precfg_nr_v16k0_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sl_precfg_freq_info_list_ext_v16k0.size() > 0, 1));

  if (sl_precfg_freq_info_list_ext_v16k0.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sl_precfg_freq_info_list_ext_v16k0, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sidelink_precfg_nr_v16k0_s::unpack(cbit_ref& bref)
{
  bool sl_precfg_freq_info_list_ext_v16k0_present;
  HANDLE_CODE(bref.unpack(sl_precfg_freq_info_list_ext_v16k0_present, 1));

  if (sl_precfg_freq_info_list_ext_v16k0_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sl_precfg_freq_info_list_ext_v16k0, bref, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
void sidelink_precfg_nr_v16k0_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sl_precfg_freq_info_list_ext_v16k0.size() > 0) {
    j.start_array("sl-PreconfigFreqInfoListExt-v16k0");
    for (const auto& e1 : sl_precfg_freq_info_list_ext_v16k0) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// SL-PreconfigurationNR-r16 ::= SEQUENCE
OCUDUASN_CODE sl_precfg_nr_r16_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= sidelink_precfg_nr_v16k0.is_present();
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(sidelink_precfg_nr_r16.pack(bref));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      if (not sidelink_precfg_nr_v16k0.is_present()) {
        asn1::log_error("Mandatory field sidelinkPreconfigNR-v16k0 is missing");
        return OCUDUASN_ERROR_ENCODE_FAIL;
      }
      HANDLE_CODE(sidelink_precfg_nr_v16k0->pack(bref));
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_precfg_nr_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(sidelink_precfg_nr_r16.unpack(bref));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      sidelink_precfg_nr_v16k0.set_present();
      HANDLE_CODE(sidelink_precfg_nr_v16k0->unpack(bref));
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_precfg_nr_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sidelinkPreconfigNR-r16");
  sidelink_precfg_nr_r16.to_json(j);
  if (sidelink_precfg_nr_v16k0.is_present()) {
    j.write_fieldname("sidelinkPreconfigNR-v16k0");
    sidelink_precfg_nr_v16k0->to_json(j);
  }
  j.end_obj();
}

template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_freq_cfg_common_r16_s::sl_bwp_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_freq_cfg_common_r16_s::sl_pos_bwp_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_tx_profile_list_r17_l::value_type>::ops;
