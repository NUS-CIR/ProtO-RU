// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/*******************************************************************************
 *
 *                     3GPP TS ASN1 F1AP v18.8.0 (2025-12)
 *
 ******************************************************************************/

#pragma once

#include "ocudu/asn1/f1ap/f1ap_pdu_items.h"

namespace asn1 {
namespace f1ap {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// DLRRCMessageTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct dl_rrc_msg_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct dl_rrc_msg_transfer_ies_container {
  bool                                         old_gnb_du_ue_f1ap_id_present                      = false;
  bool                                         execute_dupl_present                               = false;
  bool                                         rat_freq_prio_info_present                         = false;
  bool                                         rrc_delivery_status_request_present                = false;
  bool                                         ue_context_not_retrievable_present                 = false;
  bool                                         redirected_rrc_msg_present                         = false;
  bool                                         plmn_assist_info_for_net_shar_present              = false;
  bool                                         new_gnb_cu_ue_f1ap_id_present                      = false;
  bool                                         add_rrm_prio_idx_present                           = false;
  bool                                         srb_map_info_present                               = false;
  bool                                         plmn_idx_nr_assist_info_for_net_shar_present       = false;
  bool                                         area_specific_semi_persistent_srs_pos_info_present = false;
  uint64_t                                     gnb_cu_ue_f1ap_id;
  uint64_t                                     gnb_du_ue_f1ap_id;
  uint64_t                                     old_gnb_du_ue_f1ap_id;
  uint8_t                                      srb_id;
  execute_dupl_e                               execute_dupl;
  unbounded_octstring<true>                    rrc_container;
  rat_freq_prio_info_c                         rat_freq_prio_info;
  rrc_delivery_status_request_e                rrc_delivery_status_request;
  ue_context_not_retrievable_e                 ue_context_not_retrievable;
  unbounded_octstring<true>                    redirected_rrc_msg;
  fixed_octstring<3, true>                     plmn_assist_info_for_net_shar;
  uint64_t                                     new_gnb_cu_ue_f1ap_id;
  fixed_bitstring<32, false, true>             add_rrm_prio_idx;
  uint8_t                                      srb_map_info;
  uint8_t                                      plmn_idx_nr_assist_info_for_net_shar;
  area_specific_semi_persistent_srs_pos_info_s area_specific_semi_persistent_srs_pos_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLRRCMessageTransfer ::= SEQUENCE
using dl_rrc_msg_transfer_s = elementary_procedure_option<dl_rrc_msg_transfer_ies_container>;

// InitialULRRCMessageTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct init_ul_rrc_msg_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct init_ul_rrc_msg_transfer_ies_container {
  bool                      du_to_cu_rrc_container_present           = false;
  bool                      sul_access_ind_present                   = false;
  bool                      ran_ue_id_present                        = false;
  bool                      rrc_container_rrc_setup_complete_present = false;
  bool                      nr_red_cap_ue_ind_present                = false;
  bool                      sdt_info_present                         = false;
  bool                      sidelink_relay_cfg_present               = false;
  bool                      nr_e_red_cap_ue_ind_present              = false;
  uint64_t                  gnb_du_ue_f1ap_id;
  nr_cgi_s                  nr_cgi;
  uint32_t                  c_rnti;
  unbounded_octstring<true> rrc_container;
  unbounded_octstring<true> du_to_cu_rrc_container;
  sul_access_ind_e          sul_access_ind;
  uint16_t                  transaction_id;
  fixed_octstring<8, true>  ran_ue_id;
  unbounded_octstring<true> rrc_container_rrc_setup_complete;
  nr_red_cap_ue_ind_e       nr_red_cap_ue_ind;
  sdt_info_s                sdt_info;
  sidelink_relay_cfg_s      sidelink_relay_cfg;
  nr_e_red_cap_ue_ind_e     nr_e_red_cap_ue_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// InitialULRRCMessageTransfer ::= SEQUENCE
using init_ul_rrc_msg_transfer_s = elementary_procedure_option<init_ul_rrc_msg_transfer_ies_container>;

// UEContextModificationConfirmIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_mod_confirm_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_mod_confirm_ies_container {
  bool                                               res_coordination_transfer_container_present              = false;
  bool                                               drbs_modified_conf_list_present                          = false;
  bool                                               rrc_container_present                                    = false;
  bool                                               crit_diagnostics_present                                 = false;
  bool                                               execute_dupl_present                                     = false;
  bool                                               res_coordination_transfer_info_present                   = false;
  bool                                               sl_drbs_modified_conf_list_present                       = false;
  bool                                               uu_rlc_ch_modified_list_present                          = false;
  bool                                               pc5_rlc_ch_modified_list_present                         = false;
  bool                                               ue_multicast_m_rbs_confirmed_to_be_modified_list_present = false;
  uint64_t                                           gnb_cu_ue_f1ap_id;
  uint64_t                                           gnb_du_ue_f1ap_id;
  unbounded_octstring<true>                          res_coordination_transfer_container;
  drbs_modified_conf_list_l                          drbs_modified_conf_list;
  unbounded_octstring<true>                          rrc_container;
  crit_diagnostics_s                                 crit_diagnostics;
  execute_dupl_e                                     execute_dupl;
  res_coordination_transfer_info_s                   res_coordination_transfer_info;
  sl_drbs_modified_conf_list_l                       sl_drbs_modified_conf_list;
  uu_rlc_ch_modified_list_l                          uu_rlc_ch_modified_list;
  pc5_rlc_ch_modified_list_l                         pc5_rlc_ch_modified_list;
  ue_multicast_m_rbs_confirmed_to_be_modified_list_l ue_multicast_m_rbs_confirmed_to_be_modified_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextModificationConfirm ::= SEQUENCE
using ue_context_mod_confirm_s = elementary_procedure_option<ue_context_mod_confirm_ies_container>;

// UEContextModificationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_mod_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_mod_fail_ies_container {
  bool               crit_diagnostics_present                = false;
  bool               requested_target_cell_global_id_present = false;
  uint64_t           gnb_cu_ue_f1ap_id;
  uint64_t           gnb_du_ue_f1ap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;
  nr_cgi_s           requested_target_cell_global_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextModificationFailure ::= SEQUENCE
using ue_context_mod_fail_s = elementary_procedure_option<ue_context_mod_fail_ies_container>;

// UEContextModificationRefuseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_mod_refuse_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_mod_refuse_ies_container {
  bool               crit_diagnostics_present = false;
  uint64_t           gnb_cu_ue_f1ap_id;
  uint64_t           gnb_du_ue_f1ap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextModificationRefuse ::= SEQUENCE
using ue_context_mod_refuse_s = elementary_procedure_option<ue_context_mod_refuse_ies_container>;

// UEContextModificationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_mod_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_mod_request_ies_container {
  bool                                            sp_cell_id_present                                    = false;
  bool                                            serv_cell_idx_present                                 = false;
  bool                                            sp_cell_ul_cfg_present                                = false;
  bool                                            drx_cycle_present                                     = false;
  bool                                            cu_to_du_rrc_info_present                             = false;
  bool                                            tx_action_ind_present                                 = false;
  bool                                            res_coordination_transfer_container_present           = false;
  bool                                            rrc_recfg_complete_ind_present                        = false;
  bool                                            rrc_container_present                                 = false;
  bool                                            scell_to_be_setup_mod_list_present                    = false;
  bool                                            scell_to_be_remd_list_present                         = false;
  bool                                            srbs_to_be_setup_mod_list_present                     = false;
  bool                                            drbs_to_be_setup_mod_list_present                     = false;
  bool                                            drbs_to_be_modified_list_present                      = false;
  bool                                            srbs_to_be_released_list_present                      = false;
  bool                                            drbs_to_be_released_list_present                      = false;
  bool                                            inactivity_monitoring_request_present                 = false;
  bool                                            rat_freq_prio_info_present                            = false;
  bool                                            drx_cfg_ind_present                                   = false;
  bool                                            rlc_fail_ind_present                                  = false;
  bool                                            ul_tx_direct_current_list_info_present                = false;
  bool                                            gnb_du_cfg_query_present                              = false;
  bool                                            gnb_du_ue_ambr_ul_present                             = false;
  bool                                            execute_dupl_present                                  = false;
  bool                                            rrc_delivery_status_request_present                   = false;
  bool                                            res_coordination_transfer_info_present                = false;
  bool                                            serving_cell_mo_present                               = false;
  bool                                            needfor_gap_present                                   = false;
  bool                                            full_cfg_present                                      = false;
  bool                                            add_rrm_prio_idx_present                              = false;
  bool                                            lower_layer_presence_status_change_present            = false;
  bool                                            bh_chs_to_be_setup_mod_list_present                   = false;
  bool                                            bh_chs_to_be_modified_list_present                    = false;
  bool                                            bh_chs_to_be_released_list_present                    = false;
  bool                                            nr_v2x_services_authorized_present                    = false;
  bool                                            ltev2x_services_authorized_present                    = false;
  bool                                            nr_ue_sidelink_aggr_max_bitrate_present               = false;
  bool                                            lte_ue_sidelink_aggr_max_bitrate_present              = false;
  bool                                            pc5_link_ambr_present                                 = false;
  bool                                            sl_drbs_to_be_setup_mod_list_present                  = false;
  bool                                            sl_drbs_to_be_modified_list_present                   = false;
  bool                                            sl_drbs_to_be_released_list_present                   = false;
  bool                                            conditional_intra_du_mob_info_present                 = false;
  bool                                            f1_c_transfer_path_present                            = false;
  bool                                            scg_ind_present                                       = false;
  bool                                            ul_tx_direct_current_two_carrier_list_info_present    = false;
  bool                                            iab_conditional_rrc_msg_delivery_ind_present          = false;
  bool                                            f1_c_transfer_path_nr_dc_present                      = false;
  bool                                            mdt_polluted_meas_ind_present                         = false;
  bool                                            scg_activation_request_present                        = false;
  bool                                            cg_sdt_query_ind_present                              = false;
  bool                                            five_g_pro_se_authorized_present                      = false;
  bool                                            five_g_pro_se_ue_pc5_aggr_max_bitrate_present         = false;
  bool                                            five_g_pro_se_pc5_link_ambr_present                   = false;
  bool                                            upd_remote_ue_local_id_present                        = false;
  bool                                            uu_rlc_ch_to_be_setup_list_present                    = false;
  bool                                            uu_rlc_ch_to_be_modified_list_present                 = false;
  bool                                            uu_rlc_ch_to_be_released_list_present                 = false;
  bool                                            pc5_rlc_ch_to_be_setup_list_present                   = false;
  bool                                            pc5_rlc_ch_to_be_modified_list_present                = false;
  bool                                            pc5_rlc_ch_to_be_released_list_present                = false;
  bool                                            path_switch_cfg_present                               = false;
  bool                                            gnb_du_ue_slice_max_bit_rate_list_present             = false;
  bool                                            multicast_mbs_session_setup_list_present              = false;
  bool                                            multicast_mbs_session_rem_list_present                = false;
  bool                                            ue_multicast_m_rbs_to_be_setup_at_modify_list_present = false;
  bool                                            ue_multicast_m_rbs_to_be_released_list_present        = false;
  bool                                            sldrx_cycle_list_present                              = false;
  bool                                            management_based_mdt_plmn_mod_list_present            = false;
  bool                                            sdt_bearer_cfg_query_ind_present                      = false;
  bool                                            daps_ho_status_present                                = false;
  bool                                            serving_cell_mo_list_present                          = false;
  bool                                            ul_tx_direct_current_more_carrier_info_present        = false;
  bool                                            cp_acmcg_info_present                                 = false;
  bool                                            network_ctrl_repeater_authorized_present              = false;
  bool                                            sdt_volume_thres_present                              = false;
  bool                                            ltm_info_modify_present                               = false;
  bool                                            ltmcfra_res_cfg_list_present                          = false;
  bool                                            ltm_cfg_id_map_list_present                           = false;
  bool                                            early_sync_info_request_present                       = false;
  bool                                            early_sync_candidate_cell_info_list_present           = false;
  bool                                            early_sync_serving_cell_info_present                  = false;
  bool                                            ltm_cells_to_be_released_list_present                 = false;
  bool                                            path_addition_info_present                            = false;
  bool                                            nr_a2_x_services_authorized_present                   = false;
  bool                                            ltea2_x_services_authorized_present                   = false;
  bool                                            nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present      = false;
  bool                                            lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present     = false;
  bool                                            dl_lbt_fail_info_request_present                      = false;
  bool                                            sl_positioning_ranging_service_info_present           = false;
  bool                                            non_integer_drx_cycle_present                         = false;
  bool                                            ltm_reset_info_present                                = false;
  bool                                            ltmtci_states_cfgs_list_present                       = false;
  uint64_t                                        gnb_cu_ue_f1ap_id;
  uint64_t                                        gnb_du_ue_f1ap_id;
  nr_cgi_s                                        sp_cell_id;
  uint8_t                                         serv_cell_idx;
  cell_ul_cfg_e                                   sp_cell_ul_cfg;
  drx_cycle_s                                     drx_cycle;
  cu_to_du_rrc_info_s                             cu_to_du_rrc_info;
  tx_action_ind_e                                 tx_action_ind;
  unbounded_octstring<true>                       res_coordination_transfer_container;
  rrc_recfg_complete_ind_e                        rrc_recfg_complete_ind;
  unbounded_octstring<true>                       rrc_container;
  scell_to_be_setup_mod_list_l                    scell_to_be_setup_mod_list;
  scell_to_be_remd_list_l                         scell_to_be_remd_list;
  srbs_to_be_setup_mod_list_l                     srbs_to_be_setup_mod_list;
  drbs_to_be_setup_mod_list_l                     drbs_to_be_setup_mod_list;
  drbs_to_be_modified_list_l                      drbs_to_be_modified_list;
  srbs_to_be_released_list_l                      srbs_to_be_released_list;
  drbs_to_be_released_list_l                      drbs_to_be_released_list;
  inactivity_monitoring_request_e                 inactivity_monitoring_request;
  rat_freq_prio_info_c                            rat_freq_prio_info;
  drx_cfg_ind_e                                   drx_cfg_ind;
  rlc_fail_ind_s                                  rlc_fail_ind;
  unbounded_octstring<true>                       ul_tx_direct_current_list_info;
  gnb_du_cfg_query_e                              gnb_du_cfg_query;
  uint64_t                                        gnb_du_ue_ambr_ul;
  execute_dupl_e                                  execute_dupl;
  rrc_delivery_status_request_e                   rrc_delivery_status_request;
  res_coordination_transfer_info_s                res_coordination_transfer_info;
  uint16_t                                        serving_cell_mo;
  needfor_gap_e                                   needfor_gap;
  full_cfg_e                                      full_cfg;
  fixed_bitstring<32, false, true>                add_rrm_prio_idx;
  lower_layer_presence_status_change_e            lower_layer_presence_status_change;
  bh_chs_to_be_setup_mod_list_l                   bh_chs_to_be_setup_mod_list;
  bh_chs_to_be_modified_list_l                    bh_chs_to_be_modified_list;
  bh_chs_to_be_released_list_l                    bh_chs_to_be_released_list;
  nr_v2x_services_authorized_s                    nr_v2x_services_authorized;
  ltev2x_services_authorized_s                    ltev2x_services_authorized;
  nr_ue_sidelink_aggr_max_bitrate_s               nr_ue_sidelink_aggr_max_bitrate;
  lte_ue_sidelink_aggr_max_bitrate_s              lte_ue_sidelink_aggr_max_bitrate;
  uint64_t                                        pc5_link_ambr;
  sl_drbs_to_be_setup_mod_list_l                  sl_drbs_to_be_setup_mod_list;
  sl_drbs_to_be_modified_list_l                   sl_drbs_to_be_modified_list;
  sl_drbs_to_be_released_list_l                   sl_drbs_to_be_released_list;
  conditional_intra_du_mob_info_s                 conditional_intra_du_mob_info;
  f1_c_transfer_path_s                            f1_c_transfer_path;
  scg_ind_e                                       scg_ind;
  unbounded_octstring<true>                       ul_tx_direct_current_two_carrier_list_info;
  iab_conditional_rrc_msg_delivery_ind_e          iab_conditional_rrc_msg_delivery_ind;
  f1_c_transfer_path_nr_dc_s                      f1_c_transfer_path_nr_dc;
  mdt_polluted_meas_ind_e                         mdt_polluted_meas_ind;
  scg_activation_request_e                        scg_activation_request;
  cg_sdt_query_ind_e                              cg_sdt_query_ind;
  five_g_pro_se_authorized_s                      five_g_pro_se_authorized;
  nr_ue_sidelink_aggr_max_bitrate_s               five_g_pro_se_ue_pc5_aggr_max_bitrate;
  uint64_t                                        five_g_pro_se_pc5_link_ambr;
  uint16_t                                        upd_remote_ue_local_id;
  uu_rlc_ch_to_be_setup_list_l                    uu_rlc_ch_to_be_setup_list;
  uu_rlc_ch_to_be_modified_list_l                 uu_rlc_ch_to_be_modified_list;
  uu_rlc_ch_to_be_released_list_l                 uu_rlc_ch_to_be_released_list;
  pc5_rlc_ch_to_be_setup_list_l                   pc5_rlc_ch_to_be_setup_list;
  pc5_rlc_ch_to_be_modified_list_l                pc5_rlc_ch_to_be_modified_list;
  pc5_rlc_ch_to_be_released_list_l                pc5_rlc_ch_to_be_released_list;
  path_switch_cfg_s                               path_switch_cfg;
  gnb_du_ue_slice_max_bit_rate_list_l             gnb_du_ue_slice_max_bit_rate_list;
  multicast_mbs_session_list_l                    multicast_mbs_session_setup_list;
  multicast_mbs_session_list_l                    multicast_mbs_session_rem_list;
  ue_multicast_m_rbs_to_be_setup_at_modify_list_l ue_multicast_m_rbs_to_be_setup_at_modify_list;
  ue_multicast_m_rbs_to_be_released_list_l        ue_multicast_m_rbs_to_be_released_list;
  sldrx_cycle_list_l                              sldrx_cycle_list;
  mdt_plmn_mod_list_l                             management_based_mdt_plmn_mod_list;
  sdt_bearer_cfg_query_ind_e                      sdt_bearer_cfg_query_ind;
  daps_ho_status_e                                daps_ho_status;
  serving_cell_mo_list_l                          serving_cell_mo_list;
  unbounded_octstring<true>                       ul_tx_direct_current_more_carrier_info;
  cp_acmcg_info_s                                 cp_acmcg_info;
  network_ctrl_repeater_authorized_e              network_ctrl_repeater_authorized;
  uint32_t                                        sdt_volume_thres;
  ltm_info_modify_s                               ltm_info_modify;
  ltmcfra_res_cfg_list_l                          ltmcfra_res_cfg_list;
  ltm_cfg_id_map_list_l                           ltm_cfg_id_map_list;
  early_sync_info_request_s                       early_sync_info_request;
  early_sync_candidate_cell_info_list_l           early_sync_candidate_cell_info_list;
  early_sync_serving_cell_info_s                  early_sync_serving_cell_info;
  ltm_cells_to_be_released_list_l                 ltm_cells_to_be_released_list;
  path_addition_info_c                            path_addition_info;
  nr_a2_x_services_authorized_s                   nr_a2_x_services_authorized;
  ltea2_x_services_authorized_s                   ltea2_x_services_authorized;
  nr_ue_sidelink_aggr_max_bitrate_s               nr_ue_sidelink_aggr_max_bitrate_for_a2_x;
  lte_ue_sidelink_aggr_max_bitrate_s              lte_ue_sidelink_aggr_max_bitrate_for_a2_x;
  dl_lbt_fail_info_request_e                      dl_lbt_fail_info_request;
  sl_positioning_ranging_service_info_s           sl_positioning_ranging_service_info;
  non_integer_drx_cycle_s                         non_integer_drx_cycle;
  ltm_reset_info_s                                ltm_reset_info;
  ltmtci_states_cfgs_list_l                       ltmtci_states_cfgs_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextModificationRequest ::= SEQUENCE
using ue_context_mod_request_s = elementary_procedure_option<ue_context_mod_request_ies_container>;

// UEContextModificationRequiredIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_mod_required_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_mod_required_ies_container {
  bool                                              res_coordination_transfer_container_present             = false;
  bool                                              du_to_cu_rrc_info_present                               = false;
  bool                                              drbs_required_to_be_modified_list_present               = false;
  bool                                              srbs_required_to_be_released_list_present               = false;
  bool                                              drbs_required_to_be_released_list_present               = false;
  bool                                              bh_chs_required_to_be_released_list_present             = false;
  bool                                              sl_drbs_required_to_be_modified_list_present            = false;
  bool                                              sl_drbs_required_to_be_released_list_present            = false;
  bool                                              target_cells_to_cancel_present                          = false;
  bool                                              uu_rlc_ch_required_to_be_modified_list_present          = false;
  bool                                              uu_rlc_ch_required_to_be_released_list_present          = false;
  bool                                              pc5_rlc_ch_required_to_be_modified_list_present         = false;
  bool                                              pc5_rlc_ch_required_to_be_released_list_present         = false;
  bool                                              ue_multicast_m_rbs_required_to_be_modified_list_present = false;
  bool                                              ue_multicast_m_rbs_required_to_be_released_list_present = false;
  bool                                              ltm_cells_to_be_released_list_present                   = false;
  uint64_t                                          gnb_cu_ue_f1ap_id;
  uint64_t                                          gnb_du_ue_f1ap_id;
  unbounded_octstring<true>                         res_coordination_transfer_container;
  du_to_cu_rrc_info_s                               du_to_cu_rrc_info;
  drbs_required_to_be_modified_list_l               drbs_required_to_be_modified_list;
  srbs_required_to_be_released_list_l               srbs_required_to_be_released_list;
  drbs_required_to_be_released_list_l               drbs_required_to_be_released_list;
  cause_c                                           cause;
  bh_chs_required_to_be_released_list_l             bh_chs_required_to_be_released_list;
  sl_drbs_required_to_be_modified_list_l            sl_drbs_required_to_be_modified_list;
  sl_drbs_required_to_be_released_list_l            sl_drbs_required_to_be_released_list;
  target_cell_list_l                                target_cells_to_cancel;
  uu_rlc_ch_required_to_be_modified_list_l          uu_rlc_ch_required_to_be_modified_list;
  uu_rlc_ch_required_to_be_released_list_l          uu_rlc_ch_required_to_be_released_list;
  pc5_rlc_ch_required_to_be_modified_list_l         pc5_rlc_ch_required_to_be_modified_list;
  pc5_rlc_ch_required_to_be_released_list_l         pc5_rlc_ch_required_to_be_released_list;
  ue_multicast_m_rbs_required_to_be_modified_list_l ue_multicast_m_rbs_required_to_be_modified_list;
  ue_multicast_m_rbs_required_to_be_released_list_l ue_multicast_m_rbs_required_to_be_released_list;
  ltm_cells_to_be_released_list_l                   ltm_cells_to_be_released_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextModificationRequired ::= SEQUENCE
using ue_context_mod_required_s = elementary_procedure_option<ue_context_mod_required_ies_container>;

// UEContextModificationResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_mod_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_mod_resp_ies_container {
  bool                                    res_coordination_transfer_container_present   = false;
  bool                                    du_to_cu_rrc_info_present                     = false;
  bool                                    drbs_setup_mod_list_present                   = false;
  bool                                    drbs_modified_list_present                    = false;
  bool                                    srbs_failed_to_be_setup_mod_list_present      = false;
  bool                                    drbs_failed_to_be_setup_mod_list_present      = false;
  bool                                    scell_failedto_setup_mod_list_present         = false;
  bool                                    drbs_failed_to_be_modified_list_present       = false;
  bool                                    inactivity_monitoring_resp_present            = false;
  bool                                    crit_diagnostics_present                      = false;
  bool                                    c_rnti_present                                = false;
  bool                                    associated_scell_list_present                 = false;
  bool                                    srbs_setup_mod_list_present                   = false;
  bool                                    srbs_modified_list_present                    = false;
  bool                                    full_cfg_present                              = false;
  bool                                    bh_chs_setup_mod_list_present                 = false;
  bool                                    bh_chs_modified_list_present                  = false;
  bool                                    bh_chs_failed_to_be_setup_mod_list_present    = false;
  bool                                    bh_chs_failed_to_be_modified_list_present     = false;
  bool                                    sl_drbs_setup_mod_list_present                = false;
  bool                                    sl_drbs_modified_list_present                 = false;
  bool                                    sl_drbs_failed_to_be_setup_mod_list_present   = false;
  bool                                    sl_drbs_failed_to_be_modified_list_present    = false;
  bool                                    requested_target_cell_global_id_present       = false;
  bool                                    scg_activation_status_present                 = false;
  bool                                    uu_rlc_ch_setup_list_present                  = false;
  bool                                    uu_rlc_ch_failed_to_be_setup_list_present     = false;
  bool                                    uu_rlc_ch_modified_list_present               = false;
  bool                                    uu_rlc_ch_failed_to_be_modified_list_present  = false;
  bool                                    pc5_rlc_ch_setup_list_present                 = false;
  bool                                    pc5_rlc_ch_failed_to_be_setup_list_present    = false;
  bool                                    pc5_rlc_ch_modified_list_present              = false;
  bool                                    pc5_rlc_ch_failed_to_be_modified_list_present = false;
  bool                                    sdt_bearer_cfg_info_present                   = false;
  bool                                    ue_multicast_m_rbs_setup_list_present         = false;
  bool                                    serving_cell_mo_encoded_in_cgc_list_present   = false;
  bool                                    ded_si_delivery_ind_present                   = false;
  bool                                    cfg_bwp_list_present                          = false;
  bool                                    early_sync_info_present                       = false;
  bool                                    ltm_cfg_present                               = false;
  bool                                    s_cp_ac_cfg_present                           = false;
  uint64_t                                gnb_cu_ue_f1ap_id;
  uint64_t                                gnb_du_ue_f1ap_id;
  unbounded_octstring<true>               res_coordination_transfer_container;
  du_to_cu_rrc_info_s                     du_to_cu_rrc_info;
  drbs_setup_mod_list_l                   drbs_setup_mod_list;
  drbs_modified_list_l                    drbs_modified_list;
  srbs_failed_to_be_setup_mod_list_l      srbs_failed_to_be_setup_mod_list;
  drbs_failed_to_be_setup_mod_list_l      drbs_failed_to_be_setup_mod_list;
  scell_failedto_setup_mod_list_l         scell_failedto_setup_mod_list;
  drbs_failed_to_be_modified_list_l       drbs_failed_to_be_modified_list;
  inactivity_monitoring_resp_e            inactivity_monitoring_resp;
  crit_diagnostics_s                      crit_diagnostics;
  uint32_t                                c_rnti;
  associated_scell_list_l                 associated_scell_list;
  srbs_setup_mod_list_l                   srbs_setup_mod_list;
  srbs_modified_list_l                    srbs_modified_list;
  full_cfg_e                              full_cfg;
  bh_chs_setup_mod_list_l                 bh_chs_setup_mod_list;
  bh_chs_modified_list_l                  bh_chs_modified_list;
  bh_chs_failed_to_be_setup_mod_list_l    bh_chs_failed_to_be_setup_mod_list;
  bh_chs_failed_to_be_modified_list_l     bh_chs_failed_to_be_modified_list;
  sl_drbs_setup_mod_list_l                sl_drbs_setup_mod_list;
  sl_drbs_modified_list_l                 sl_drbs_modified_list;
  sl_drbs_failed_to_be_setup_mod_list_l   sl_drbs_failed_to_be_setup_mod_list;
  sl_drbs_failed_to_be_modified_list_l    sl_drbs_failed_to_be_modified_list;
  nr_cgi_s                                requested_target_cell_global_id;
  scg_activation_status_e                 scg_activation_status;
  uu_rlc_ch_setup_list_l                  uu_rlc_ch_setup_list;
  uu_rlc_ch_failed_to_be_setup_list_l     uu_rlc_ch_failed_to_be_setup_list;
  uu_rlc_ch_modified_list_l               uu_rlc_ch_modified_list;
  uu_rlc_ch_failed_to_be_modified_list_l  uu_rlc_ch_failed_to_be_modified_list;
  pc5_rlc_ch_setup_list_l                 pc5_rlc_ch_setup_list;
  pc5_rlc_ch_failed_to_be_setup_list_l    pc5_rlc_ch_failed_to_be_setup_list;
  pc5_rlc_ch_modified_list_l              pc5_rlc_ch_modified_list;
  pc5_rlc_ch_failed_to_be_modified_list_l pc5_rlc_ch_failed_to_be_modified_list;
  sdt_bearer_cfg_info_s                   sdt_bearer_cfg_info;
  ue_multicast_m_rbs_setup_list_l         ue_multicast_m_rbs_setup_list;
  serving_cell_mo_encoded_in_cgc_list_l   serving_cell_mo_encoded_in_cgc_list;
  ded_si_delivery_ind_e                   ded_si_delivery_ind;
  cfg_bwp_list_l                          cfg_bwp_list;
  early_sync_info_s                       early_sync_info;
  ltm_cfg_s                               ltm_cfg;
  s_cp_ac_cfg_s                           s_cp_ac_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextModificationResponse ::= SEQUENCE
using ue_context_mod_resp_s = elementary_procedure_option<ue_context_mod_resp_ies_container>;

// UEContextReleaseCommandIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_release_cmd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_release_cmd_ies_container {
  bool                            rrc_container_present                 = false;
  bool                            srb_id_present                        = false;
  bool                            old_gnb_du_ue_f1ap_id_present         = false;
  bool                            execute_dupl_present                  = false;
  bool                            rrc_delivery_status_request_present   = false;
  bool                            target_cells_to_cancel_present        = false;
  bool                            pos_context_rev_ind_present           = false;
  bool                            cg_sdt_kept_ind_present               = false;
  bool                            ltm_cells_to_be_released_list_present = false;
  bool                            dl_lbt_fail_info_request_present      = false;
  uint64_t                        gnb_cu_ue_f1ap_id;
  uint64_t                        gnb_du_ue_f1ap_id;
  cause_c                         cause;
  unbounded_octstring<true>       rrc_container;
  uint8_t                         srb_id;
  uint64_t                        old_gnb_du_ue_f1ap_id;
  execute_dupl_e                  execute_dupl;
  rrc_delivery_status_request_e   rrc_delivery_status_request;
  target_cell_list_l              target_cells_to_cancel;
  pos_context_rev_ind_e           pos_context_rev_ind;
  cg_sdt_kept_ind_e               cg_sdt_kept_ind;
  ltm_cells_to_be_released_list_l ltm_cells_to_be_released_list;
  dl_lbt_fail_info_request_e      dl_lbt_fail_info_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextReleaseCommand ::= SEQUENCE
using ue_context_release_cmd_s = elementary_procedure_option<ue_context_release_cmd_ies_container>;

// UEContextReleaseCompleteIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_release_complete_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_release_complete_ies_container {
  bool                                crit_diagnostics_present                  = false;
  bool                                recommended_ss_bs_for_paging_list_present = false;
  uint64_t                            gnb_cu_ue_f1ap_id;
  uint64_t                            gnb_du_ue_f1ap_id;
  crit_diagnostics_s                  crit_diagnostics;
  recommended_ss_bs_for_paging_list_l recommended_ss_bs_for_paging_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextReleaseComplete ::= SEQUENCE
using ue_context_release_complete_s = elementary_procedure_option<ue_context_release_complete_ies_container>;

// UEContextReleaseRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_release_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_release_request_ies_container {
  bool                            target_cells_to_cancel_present        = false;
  bool                            ltm_cells_to_be_released_list_present = false;
  uint64_t                        gnb_cu_ue_f1ap_id;
  uint64_t                        gnb_du_ue_f1ap_id;
  cause_c                         cause;
  target_cell_list_l              target_cells_to_cancel;
  ltm_cells_to_be_released_list_l ltm_cells_to_be_released_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextReleaseRequest ::= SEQUENCE
using ue_context_release_request_s = elementary_procedure_option<ue_context_release_request_ies_container>;

// UEContextSetupFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_setup_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_setup_fail_ies_container {
  bool                     gnb_du_ue_f1ap_id_present               = false;
  bool                     crit_diagnostics_present                = false;
  bool                     potential_sp_cell_list_present          = false;
  bool                     requested_target_cell_global_id_present = false;
  uint64_t                 gnb_cu_ue_f1ap_id;
  uint64_t                 gnb_du_ue_f1ap_id;
  cause_c                  cause;
  crit_diagnostics_s       crit_diagnostics;
  potential_sp_cell_list_l potential_sp_cell_list;
  nr_cgi_s                 requested_target_cell_global_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextSetupFailure ::= SEQUENCE
using ue_context_setup_fail_s = elementary_procedure_option<ue_context_setup_fail_ies_container>;

// UEContextSetupRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_setup_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_setup_request_ies_container {
  bool                                  gnb_du_ue_f1ap_id_present                         = false;
  bool                                  sp_cell_ul_cfg_present                            = false;
  bool                                  candidate_sp_cell_list_present                    = false;
  bool                                  drx_cycle_present                                 = false;
  bool                                  res_coordination_transfer_container_present       = false;
  bool                                  scell_to_be_setup_list_present                    = false;
  bool                                  srbs_to_be_setup_list_present                     = false;
  bool                                  drbs_to_be_setup_list_present                     = false;
  bool                                  inactivity_monitoring_request_present             = false;
  bool                                  rat_freq_prio_info_present                        = false;
  bool                                  rrc_container_present                             = false;
  bool                                  masked_imeisv_present                             = false;
  bool                                  serving_plmn_present                              = false;
  bool                                  gnb_du_ue_ambr_ul_present                         = false;
  bool                                  rrc_delivery_status_request_present               = false;
  bool                                  res_coordination_transfer_info_present            = false;
  bool                                  serving_cell_mo_present                           = false;
  bool                                  new_gnb_cu_ue_f1ap_id_present                     = false;
  bool                                  ran_ue_id_present                                 = false;
  bool                                  trace_activation_present                          = false;
  bool                                  add_rrm_prio_idx_present                          = false;
  bool                                  bh_chs_to_be_setup_list_present                   = false;
  bool                                  cfg_bap_address_present                           = false;
  bool                                  nr_v2x_services_authorized_present                = false;
  bool                                  ltev2x_services_authorized_present                = false;
  bool                                  nr_ue_sidelink_aggr_max_bitrate_present           = false;
  bool                                  lte_ue_sidelink_aggr_max_bitrate_present          = false;
  bool                                  pc5_link_ambr_present                             = false;
  bool                                  sl_drbs_to_be_setup_list_present                  = false;
  bool                                  conditional_inter_du_mob_info_present             = false;
  bool                                  management_based_mdt_plmn_list_present            = false;
  bool                                  serving_n_id_present                              = false;
  bool                                  f1_c_transfer_path_present                        = false;
  bool                                  f1_c_transfer_path_nr_dc_present                  = false;
  bool                                  mdt_polluted_meas_ind_present                     = false;
  bool                                  scg_activation_request_present                    = false;
  bool                                  cg_sdt_session_info_old_present                   = false;
  bool                                  five_g_pro_se_authorized_present                  = false;
  bool                                  five_g_pro_se_ue_pc5_aggr_max_bitrate_present     = false;
  bool                                  five_g_pro_se_pc5_link_ambr_present               = false;
  bool                                  uu_rlc_ch_to_be_setup_list_present                = false;
  bool                                  pc5_rlc_ch_to_be_setup_list_present               = false;
  bool                                  path_switch_cfg_present                           = false;
  bool                                  gnb_du_ue_slice_max_bit_rate_list_present         = false;
  bool                                  multicast_mbs_session_setup_list_present          = false;
  bool                                  ue_multicast_m_rbs_to_be_setup_list_present       = false;
  bool                                  serving_cell_mo_list_present                      = false;
  bool                                  network_ctrl_repeater_authorized_present          = false;
  bool                                  sdt_volume_thres_present                          = false;
  bool                                  ltm_info_setup_present                            = false;
  bool                                  ltm_cfg_id_map_list_present                       = false;
  bool                                  early_sync_info_request_present                   = false;
  bool                                  path_addition_info_present                        = false;
  bool                                  nr_a2_x_services_authorized_present               = false;
  bool                                  ltea2_x_services_authorized_present               = false;
  bool                                  nr_ue_sidelink_aggr_max_bitrate_for_a2_x_present  = false;
  bool                                  lte_ue_sidelink_aggr_max_bitrate_for_a2_x_present = false;
  bool                                  dl_lbt_fail_info_request_present                  = false;
  bool                                  sl_positioning_ranging_service_info_present       = false;
  bool                                  non_integer_drx_cycle_present                     = false;
  uint64_t                              gnb_cu_ue_f1ap_id;
  uint64_t                              gnb_du_ue_f1ap_id;
  nr_cgi_s                              sp_cell_id;
  uint8_t                               serv_cell_idx;
  cell_ul_cfg_e                         sp_cell_ul_cfg;
  cu_to_du_rrc_info_s                   cu_to_du_rrc_info;
  candidate_sp_cell_list_l              candidate_sp_cell_list;
  drx_cycle_s                           drx_cycle;
  unbounded_octstring<true>             res_coordination_transfer_container;
  scell_to_be_setup_list_l              scell_to_be_setup_list;
  srbs_to_be_setup_list_l               srbs_to_be_setup_list;
  drbs_to_be_setup_list_l               drbs_to_be_setup_list;
  inactivity_monitoring_request_e       inactivity_monitoring_request;
  rat_freq_prio_info_c                  rat_freq_prio_info;
  unbounded_octstring<true>             rrc_container;
  fixed_bitstring<64, false, true>      masked_imeisv;
  fixed_octstring<3, true>              serving_plmn;
  uint64_t                              gnb_du_ue_ambr_ul;
  rrc_delivery_status_request_e         rrc_delivery_status_request;
  res_coordination_transfer_info_s      res_coordination_transfer_info;
  uint16_t                              serving_cell_mo;
  uint64_t                              new_gnb_cu_ue_f1ap_id;
  fixed_octstring<8, true>              ran_ue_id;
  trace_activation_s                    trace_activation;
  fixed_bitstring<32, false, true>      add_rrm_prio_idx;
  bh_chs_to_be_setup_list_l             bh_chs_to_be_setup_list;
  fixed_bitstring<10, false, true>      cfg_bap_address;
  nr_v2x_services_authorized_s          nr_v2x_services_authorized;
  ltev2x_services_authorized_s          ltev2x_services_authorized;
  nr_ue_sidelink_aggr_max_bitrate_s     nr_ue_sidelink_aggr_max_bitrate;
  lte_ue_sidelink_aggr_max_bitrate_s    lte_ue_sidelink_aggr_max_bitrate;
  uint64_t                              pc5_link_ambr;
  sl_drbs_to_be_setup_list_l            sl_drbs_to_be_setup_list;
  conditional_inter_du_mob_info_s       conditional_inter_du_mob_info;
  mdt_plmn_list_l                       management_based_mdt_plmn_list;
  fixed_bitstring<44, false, true>      serving_n_id;
  f1_c_transfer_path_s                  f1_c_transfer_path;
  f1_c_transfer_path_nr_dc_s            f1_c_transfer_path_nr_dc;
  mdt_polluted_meas_ind_e               mdt_polluted_meas_ind;
  scg_activation_request_e              scg_activation_request;
  cg_sdt_session_info_s                 cg_sdt_session_info_old;
  five_g_pro_se_authorized_s            five_g_pro_se_authorized;
  nr_ue_sidelink_aggr_max_bitrate_s     five_g_pro_se_ue_pc5_aggr_max_bitrate;
  uint64_t                              five_g_pro_se_pc5_link_ambr;
  uu_rlc_ch_to_be_setup_list_l          uu_rlc_ch_to_be_setup_list;
  pc5_rlc_ch_to_be_setup_list_l         pc5_rlc_ch_to_be_setup_list;
  path_switch_cfg_s                     path_switch_cfg;
  gnb_du_ue_slice_max_bit_rate_list_l   gnb_du_ue_slice_max_bit_rate_list;
  multicast_mbs_session_list_l          multicast_mbs_session_setup_list;
  ue_multicast_m_rbs_to_be_setup_list_l ue_multicast_m_rbs_to_be_setup_list;
  serving_cell_mo_list_l                serving_cell_mo_list;
  network_ctrl_repeater_authorized_e    network_ctrl_repeater_authorized;
  uint32_t                              sdt_volume_thres;
  ltm_info_setup_s                      ltm_info_setup;
  ltm_cfg_id_map_list_l                 ltm_cfg_id_map_list;
  early_sync_info_request_s             early_sync_info_request;
  path_addition_info_c                  path_addition_info;
  nr_a2_x_services_authorized_s         nr_a2_x_services_authorized;
  ltea2_x_services_authorized_s         ltea2_x_services_authorized;
  nr_ue_sidelink_aggr_max_bitrate_s     nr_ue_sidelink_aggr_max_bitrate_for_a2_x;
  lte_ue_sidelink_aggr_max_bitrate_s    lte_ue_sidelink_aggr_max_bitrate_for_a2_x;
  dl_lbt_fail_info_request_e            dl_lbt_fail_info_request;
  sl_positioning_ranging_service_info_s sl_positioning_ranging_service_info;
  non_integer_drx_cycle_s               non_integer_drx_cycle;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextSetupRequest ::= SEQUENCE
using ue_context_setup_request_s = elementary_procedure_option<ue_context_setup_request_ies_container>;

// UEContextSetupResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_context_setup_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_setup_resp_ies_container {
  bool                                  c_rnti_present                              = false;
  bool                                  res_coordination_transfer_container_present = false;
  bool                                  full_cfg_present                            = false;
  bool                                  drbs_setup_list_present                     = false;
  bool                                  srbs_failed_to_be_setup_list_present        = false;
  bool                                  drbs_failed_to_be_setup_list_present        = false;
  bool                                  scell_failedto_setup_list_present           = false;
  bool                                  inactivity_monitoring_resp_present          = false;
  bool                                  crit_diagnostics_present                    = false;
  bool                                  srbs_setup_list_present                     = false;
  bool                                  bh_chs_setup_list_present                   = false;
  bool                                  bh_chs_failed_to_be_setup_list_present      = false;
  bool                                  sl_drbs_setup_list_present                  = false;
  bool                                  sl_drbs_failed_to_be_setup_list_present     = false;
  bool                                  requested_target_cell_global_id_present     = false;
  bool                                  scg_activation_status_present               = false;
  bool                                  uu_rlc_ch_setup_list_present                = false;
  bool                                  uu_rlc_ch_failed_to_be_setup_list_present   = false;
  bool                                  pc5_rlc_ch_setup_list_present               = false;
  bool                                  pc5_rlc_ch_failed_to_be_setup_list_present  = false;
  bool                                  serving_cell_mo_encoded_in_cgc_list_present = false;
  bool                                  ue_multicast_m_rbs_setupnew_list_present    = false;
  bool                                  ded_si_delivery_ind_present                 = false;
  bool                                  cfg_bwp_list_present                        = false;
  bool                                  early_sync_info_present                     = false;
  bool                                  ltm_cfg_present                             = false;
  bool                                  s_cp_ac_cfg_present                         = false;
  uint64_t                              gnb_cu_ue_f1ap_id;
  uint64_t                              gnb_du_ue_f1ap_id;
  du_to_cu_rrc_info_s                   du_to_cu_rrc_info;
  uint32_t                              c_rnti;
  unbounded_octstring<true>             res_coordination_transfer_container;
  full_cfg_e                            full_cfg;
  drbs_setup_list_l                     drbs_setup_list;
  srbs_failed_to_be_setup_list_l        srbs_failed_to_be_setup_list;
  drbs_failed_to_be_setup_list_l        drbs_failed_to_be_setup_list;
  scell_failedto_setup_list_l           scell_failedto_setup_list;
  inactivity_monitoring_resp_e          inactivity_monitoring_resp;
  crit_diagnostics_s                    crit_diagnostics;
  srbs_setup_list_l                     srbs_setup_list;
  bh_chs_setup_list_l                   bh_chs_setup_list;
  bh_chs_failed_to_be_setup_list_l      bh_chs_failed_to_be_setup_list;
  sl_drbs_setup_list_l                  sl_drbs_setup_list;
  sl_drbs_failed_to_be_setup_list_l     sl_drbs_failed_to_be_setup_list;
  nr_cgi_s                              requested_target_cell_global_id;
  scg_activation_status_e               scg_activation_status;
  uu_rlc_ch_setup_list_l                uu_rlc_ch_setup_list;
  uu_rlc_ch_failed_to_be_setup_list_l   uu_rlc_ch_failed_to_be_setup_list;
  pc5_rlc_ch_setup_list_l               pc5_rlc_ch_setup_list;
  pc5_rlc_ch_failed_to_be_setup_list_l  pc5_rlc_ch_failed_to_be_setup_list;
  serving_cell_mo_encoded_in_cgc_list_l serving_cell_mo_encoded_in_cgc_list;
  ue_multicast_m_rbs_setupnew_list_l    ue_multicast_m_rbs_setupnew_list;
  ded_si_delivery_ind_e                 ded_si_delivery_ind;
  cfg_bwp_list_l                        cfg_bwp_list;
  early_sync_info_s                     early_sync_info;
  ltm_cfg_s                             ltm_cfg;
  s_cp_ac_cfg_s                         s_cp_ac_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextSetupResponse ::= SEQUENCE
using ue_context_setup_resp_s = elementary_procedure_option<ue_context_setup_resp_ies_container>;

// UEInactivityNotificationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ue_inactivity_notif_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_inactivity_notif_ies_container {
  bool                      sdt_termination_request_present = false;
  uint64_t                  gnb_cu_ue_f1ap_id;
  uint64_t                  gnb_du_ue_f1ap_id;
  drb_activity_list_l       drb_activity_list;
  sdt_termination_request_e sdt_termination_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEInactivityNotification ::= SEQUENCE
using ue_inactivity_notif_s = elementary_procedure_option<ue_inactivity_notif_ies_container>;

// ULRRCMessageTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ul_rrc_msg_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ul_rrc_msg_transfer_ies_container {
  bool                      sel_plmn_id_present           = false;
  bool                      new_gnb_du_ue_f1ap_id_present = false;
  uint64_t                  gnb_cu_ue_f1ap_id;
  uint64_t                  gnb_du_ue_f1ap_id;
  uint8_t                   srb_id;
  unbounded_octstring<true> rrc_container;
  fixed_octstring<3, true>  sel_plmn_id;
  uint64_t                  new_gnb_du_ue_f1ap_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ULRRCMessageTransfer ::= SEQUENCE
using ul_rrc_msg_transfer_s = elementary_procedure_option<ul_rrc_msg_transfer_ies_container>;

} // namespace f1ap
} // namespace asn1
