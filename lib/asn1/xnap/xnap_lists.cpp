// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/xnap/xnap_ies.h"
#include "ocudu/asn1/xnap/xnap_pdu_contents.h"

using namespace asn1;
using namespace asn1::xnap;

template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::a2_xpc5_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::amf_region_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::alt_qos_para_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbs_qos_flows_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbs_service_area_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbs_service_area_tai_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbs_service_area_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_mapand_data_forwarding_request_infofrom_source_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::csi_rs_mtc_neighbour_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::csi_rs_neighbour_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::csi_rs_mtc_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::add_meas_timing_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::add_pdcp_dupl_tnl_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::add_ul_ng_u_tnl_at_up_f_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_f_lows_accepted_to_be_forwarded_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::data_forwarding_resp_drb_item_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::add_listof_pdu_session_res_change_confirm_info_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::allowed_pni_npn_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::list_of_tai_sin_ao_i_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::list_of_cells_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::list_of_ran_nodesin_ao_i_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::area_of_interest_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cell_id_listfor_mdt_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tai_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cag_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sn_pn_cell_id_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sn_pn_tai_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sn_pn_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::nr_carrier_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::supported_sul_band_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::nr_freq_band_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::area_scope_of_neigh_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cell_id_listfor_qmc_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tai_listfor_qmc_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::associated_qos_flow_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::bap_ctrl_pdu_rlc_ch_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::bh_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::bplmn_id_info_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::broadcast_n_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::broadcast_sn_pn_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::broadcast_cag_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::broadcast_pni_npn_id_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::bplmn_id_info_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slice_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tai_nsag_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::tai_slice_unavailable_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::qos_flows_admitted_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::qos_flows_list_with_cause_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::secondarydata_forwarding_info_from_target_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_admitted_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cho_candidate_p_scells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cho_target_sn_node_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cho_execution_condition_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cho_candidate_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cn_type_restricts_for_equivalent_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_ac_execution_condition_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cp_ac_candidate_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_a_ccandidate_p_scells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_a_ccandidate_p_scells_wother_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::s_cp_ac_security_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::s_cp_ac_multi_target_sn_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cp_c_target_sn_confirm_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::c_pci_nformation_upd_p_scells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cp_c_target_sn_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cp_c_target_sn_required_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::candidate_relay_ue_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ssb_area_capacity_value_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_based_ue_trajectory_prediction_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ssb_area_rr_status_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::snssai_rr_status_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slice_rr_status_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cell_meas_fail_cause_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_meas_initiation_result_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::snssai_available_capacity_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slice_available_capacity_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::nr_u_ch_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cell_meas_result_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_meas_result_for_data_collection_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::replacing_cells_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ssb_to_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::snssai_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slice_to_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cell_to_report_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_to_report_for_data_collection_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::conditional_recfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ssb_coverage_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::coverage_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::crit_diagnostics_ie_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::daps_resp_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::dl_lbt_fail_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::drb_list_with_cause_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::qos_flows_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::drb_to_qos_flow_map_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::up_transport_params_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_admitted_list_mod_confirm_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_mappedto_drb_setup_resp_m_nterminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_admitted_list_mod_resp_m_nterminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_admitted_list_setup_resp_m_nterminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_subject_to_dl_discarding_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_subject_to_early_status_transfer_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_subject_to_status_transfer_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_mod_rqd_m_nterminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::rlc_dupl_state_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_mod_rqd_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_modified_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_mappedto_drb_setup_m_nterminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_mod_m_nterminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_mappedto_drb_setup_resp_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_mod_resp_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_setup_list_mod_rqd_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_setup_list_setup_m_nterminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_setup_list_setup_resp_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::du_f_slot_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_to_be_forwarded_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::data_forwarding_info_from_target_e_utran_node_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_f_lows_to_be_forwarded_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::equivalent_sn_p_ns_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::excess_packet_delay_thres_cfg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::expected_ue_moving_trajectory_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::gtp_tlas_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ext_tlas_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::f1_terminating_bh_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::five_g_pro_se_pc5_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::flows_mapped_to_drb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::forbidden_area_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::freq_domain_slot_h_sn_acfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::freq_domain_h_sn_acfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::hs_nas_lot_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::na_cell_res_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::iab_mt_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::iab_stc_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::iab_tnl_address_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::iab_allocated_tnl_address_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::iab_tnl_address_exception_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sensor_meas_cfg_name_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slot_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::last_visited_pscell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_data_forwarding_resp_infofrom_target_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbs_session_associated_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbs_session_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbs_session_info_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbsfn_sf_info_e_utra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mn_to_sn_q_mcc_oord_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mn_to_sn_q_mcc_oord_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::measured_ue_trajectory_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::rat_restricts_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::service_area_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ng_ran_cell_id_listin_ran_paging_area_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ng_ran_node2_ssb_offsets_mod_range_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::non_anchor_carrier_freqlist_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::nr_u_ch_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::neighbour_ng_ran_node_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::neighbour_info_e_utra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::neighbour_info_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::node_meas_initiation_result_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::non_f1_terminating_bh_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::pc5_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::pdu_session_list_with_cause_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_list_with_data_forwarding_from_target_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_list_with_data_forwarding_request_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::xnap::pdu_session_res_bearer_setup_complete_info_sn_terminated_s::drbs_to_be_setup_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_to_be_setup_list_setup_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_to_be_setup_list_modified_sn_terminated_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::volume_timed_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::qos_flows_usage_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_secondary_rat_usage_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_not_admitted_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::qos_flows_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_sessions_list_to_be_released_up_error_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::protected_e_utra_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ue_app_layer_meas_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sn_to_mn_q_mcc_oord_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sn_to_mn_q_mcc_oord_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::qos_flow_notif_ctrl_ind_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ran_area_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ra_report_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::rspp_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ra_report_ind_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::reset_request_partial_release_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::reset_resp_partial_release_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sdt_drbs_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sdt_srbs_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sdt_data_forwarding_drb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ssb_offsets_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::served_cell_info_e_utra_s::broadcast_plmns_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::served_cell_specific_info_req_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::served_cells_e_utra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::served_cells_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::served_cells_to_modify_e_utra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::served_cells_to_modify_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::successful_ho_report_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::successful_pscell_change_report_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::tai_support_item_s::broadcast_plmns_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tai_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_failed_to_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_to_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::target_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_to_be_release_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ue_associated_info_result_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ue_slice_max_bit_rate_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ue_history_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ue_id_idx_list_mbs_group_paging_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::xn_u_address_infoper_pdu_session_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::activ_nr_cells_and_ss_bs_item_s::ss_bs_activ_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::activ_nr_cells_and_ss_bs_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_activity_notify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_activity_notify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::bearers_subject_to_counter_check_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::boundary_node_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::to_be_activ_nr_cells_and_ss_bs_item_s::ss_bstobe_activ_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::to_be_activ_nr_cells_and_ss_bs_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::parent_node_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::iab_tnl_address_to_be_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_to_be_added_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_added_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_not_added_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_not_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_released_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_required_to_be_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_required_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::pdu_session_res_notify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_sn_change_confirm_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_sn_change_required_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_admitted_to_be_added_sn_mod_resp_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_admitted_to_be_modified_sn_mod_resp_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_admitted_added_add_req_ack_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_admitted_mod_sn_mod_confirm_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_to_be_added_add_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_to_be_modified_sn_mod_required_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_sessions_to_be_added_sn_mod_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_sessions_to_be_modified_sn_mod_request_list_l::value_type>::ops;
