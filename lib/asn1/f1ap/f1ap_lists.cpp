// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_items.h"

using namespace asn1;
using namespace asn1::f1ap;

template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::arp_location_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::combined_pos_srs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::aggr_pos_srs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::du_f_slot_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::hs_nas_lot_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::freq_domain_h_sn_a_slot_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::freq_domain_h_sn_a_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::na_res_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::child_iab_nodes_na_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::parent_iab_nodes_na_res_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_carrier_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::freq_band_nr_item_s::supported_sul_band_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::nr_freq_info_s::freq_band_list_nr_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::activ_cells_to_be_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::slot_offset_for_remaining_hops_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_srs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_res_set_list2_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_srs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::egress_bh_rlc_ch_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::add_pdcp_dupl_tnl_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::multiple_ul_ao_a_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::add_path_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::add_sib_msg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::affected_ssb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::affected_cells_and_beams_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::dl_prs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::aggr_prs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::aggr_pos_srs_res_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::aggressor_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::alt_qos_para_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::spatial_relationfor_res_id_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::spatial_relation_per_srs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::available_plmn_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::available_sn_pn_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ba_player_bh_rlc_ch_map_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::location_dependent_mbsf1_u_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ip_tolayer2_traffic_map_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_rlc_ch_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::extended_available_plmn_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::broadcast_sn_pn_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::broadcast_pni_npn_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bplmn_id_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::broadcast_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::mbs_flows_mapped_to_mrb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pscell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cu_to_du_tai_nformation_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_idx_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::candidate_cellwith_beam_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_idxwith_meass_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::candidate_cellwith_meass_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_area_capacity_value_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_area_rr_status_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::snssai_rr_status_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::slice_rr_status_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::snssai_available_capacity_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::slice_available_capacity_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_u_ch_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cell_meas_result_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_to_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::snssai_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::slice_to_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cell_to_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cells_with_ss_bs_activ_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::gnb_cu_sys_info_s::sibtypetobeupdlist_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::iab_stc_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_prach_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::neighbour_nr_cells_for_son_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cells_for_son_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::iab_mt_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::child_node_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::child_nodes_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::trp_elevation_angle_list_item_s::trp_beam_pwr_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::trp_beam_ant_angles_list_item_s::trp_elevation_angle_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_beam_ant_angles_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::target_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cfg_bwp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_coverage_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::coverage_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::crit_diagnostics_ie_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::dl_prsbw_aggregation_request_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::dl_lbt_fail_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::dl_prs_res_set_arp_s::listof_dl_prs_res_arp_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::dl_prs_res_coordinates_s::listof_dl_prs_res_set_arp_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::dl_up_tnl_info_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::flows_mapped_to_drb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ul_up_tnl_info_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::rlc_dupl_state_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::du_to_cu_tai_nformation_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::deactivation_ind_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::e_c_id_measured_results_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::e_c_id_measured_results_associated_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::e_c_id_meas_quantities_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::eutra_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::preamb_idx_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ltm_gnb_du_ids_preamb_idx_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::early_sync_candidate_cell_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltm_gnb_du_ids_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::extended_add_path_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::slice_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nsag_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::extended_served_plmns_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::flows_mapped_to_sl_drb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::served_plmns_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::slot_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_u_ch_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::gnb_du_ue_slice_max_bit_rate_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::gtp_tlas_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::iab_congestion_ind_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::iab_tnl_address_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::lc_sto_gcs_translation_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltmcfra_res_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ltm_cells_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltm_cfg_id_map_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltml2_reset_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltmtci_states_cfgs_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::mbs_broadcast_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::mbs_broadcast_mrb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_multicast_cu2_du_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_multicast_du2_cu_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::mbs_multicast_mrb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::mbs_service_area_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::mbs_service_area_tai_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::mbs_service_area_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mtch_neighbour_cell_session_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::thres_idx_session_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::multicast_common_cu2_du_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::multicast_mbs_session_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_angle_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_prs_beam_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::neighbour_node_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_failed_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_failed_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_required_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_required_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pdc_measured_results_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_meas_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::prsbw_aggregation_request_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::requested_dl_prs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::requested_dl_prs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_tx_off_per_res_set_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::prs_tx_off_per_res_item_s::prs_tx_off_ind_per_res_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_tx_off_per_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prstrp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_tx_trp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::recommended_ss_bs_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::periodicity_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_meas_quantities_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_meas_result_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_meas_result_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_sitype_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_validity_area_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::qo_e_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ra_report_ind_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ra_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::rlf_report_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::rspp_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::recommended_ss_bs_for_paging_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::requested_srs_precfg_characteristics_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sdt_bearer_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sitype_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sldrx_cycle_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ul_ch_bw_per_scs_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_carrier_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_precfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::serving_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::serving_cell_mo_encoded_in_cgc_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::successful_ho_report_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::successful_pscell_change_report_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::supported_ue_type_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::tai_nformation_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_meas_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_meas_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_prs_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::trpteg_item_s::dl_prs_res_id_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_tx_teg_assoc_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_info_type_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::time_win_info_meas_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::time_win_info_srs_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::transport_up_layer_address_info_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::transport_up_layer_address_info_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::uac_type_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::uac_plmn_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ul_bh_non_up_traffic_map_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_failed_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_failed_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::uu_rlc_ch_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_required_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_required_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::uu_rlc_ch_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::uu_rlc_ch_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::activ_cells_map_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bap_hdr_rewriting_added_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bap_hdr_rewriting_remd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_routing_info_added_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_routing_info_remd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::bh_chs_failed_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::bh_chs_failed_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::bh_chs_failed_to_be_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_chs_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::bh_chs_required_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_chs_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_chs_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_chs_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_chs_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_chs_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_chs_to_be_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::broadcast_to_be_cancelled_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::candidate_sp_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::cells_allowed_to_be_deactiv_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::cells_broadcast_cancelled_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::cells_broadcast_completed_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::cells_failed_to_be_activ_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cells_status_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cells_to_be_broadcast_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cells_to_be_activ_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cells_to_be_barred_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cells_to_be_deactiv_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drb_activity_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drb_notify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::drbs_failed_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_failed_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::drbs_failed_to_be_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_modified_conf_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::drbs_required_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::drbs_required_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drbs_to_be_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ded_si_delivery_needed_ue_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::gnb_du_served_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ncgi_to_be_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::protected_eutra_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::neighbour_cell_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::served_cells_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::served_cells_to_modify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::served_cells_to_delete_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::iab_allocated_tnl_address_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::iab_tnl_addresses_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ul_up_tnl_info_to_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::mc_paging_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ue_id_list_for_paging_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_cgi_list_for_restart_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pws_failed_nr_cgi_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::paging_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::potential_sp_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_associated_lc_f1_conn_list_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_associated_lc_f1_conn_list_res_ack_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::sl_drbs_failed_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::sl_drbs_failed_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::sl_drbs_failed_to_be_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sl_drbs_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sl_drbs_modified_conf_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::sl_drbs_required_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::sl_drbs_required_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sl_drbs_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sl_drbs_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sl_drbs_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sl_drbs_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sl_drbs_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sl_drbs_to_be_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_info_list_trp_resp_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_info_type_list_trp_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_multicast_m_rbs_confirmed_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_multicast_m_rbs_required_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_multicast_m_rbs_required_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_multicast_m_rbs_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_multicast_m_rbs_setupnew_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_multicast_m_rbs_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_multicast_m_rbs_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ue_multicast_m_rbs_to_be_setup_at_modify_list_l::value_type>::ops;
