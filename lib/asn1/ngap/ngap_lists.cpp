// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/ngap/ngap_ies.h"

using namespace asn1;
using namespace asn1::ngap;

template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::a2_x_pc5_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::amf_tnl_assoc_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::amf_tnl_assoc_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::amf_tnl_assoc_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::amf_tnl_assoc_to_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::activ_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::add_cancelledlocation_report_ref_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::qos_flow_list_with_data_forwarding_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::add_dl_up_tnl_info_for_ho_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::allowed_pni_npn_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::allowed_nssai_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::alt_qos_para_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::area_of_interest_tai_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::area_of_interest_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::area_of_interest_ran_node_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::area_of_interest_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cell_id_listfor_mdt_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cell_id_listfor_mdt_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cag_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::sn_pn_cell_id_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::sn_pn_tai_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::sn_pn_listfor_mdt_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::nr_freq_band_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::area_scope_of_neigh_cells_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::recommended_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::associated_mbs_qos_flow_setup_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::associated_mbs_qos_flow_setupor_modify_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::associated_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::bluetooth_meas_cfg_name_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cell_id_cancelled_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cancelled_cells_in_tai_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_cancelled_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cancelled_cells_in_eai_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::emergency_area_id_cancelled_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cell_id_cancelled_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cancelled_cells_in_tai_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_cancelled_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cancelled_cells_in_eai_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::emergency_area_id_cancelled_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cell_id_broadcast_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::completed_cells_in_tai_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_broadcast_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::completed_cells_in_eai_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::emergency_area_id_broadcast_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::cell_id_broadcast_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::completed_cells_in_tai_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_broadcast_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::completed_cells_in_eai_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::emergency_area_id_broadcast_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::slice_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_nsag_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::broadcast_plmn_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::crit_diagnostics_ie_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_session_tnl_info_ngran_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::expected_ue_moving_trajectory_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::cn_type_restricts_for_equivalent_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::candidate_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::candidate_relay_ue_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_list_for_inactive_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::daps_resp_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::drbs_subject_to_dl_discarding_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::drbs_subject_to_early_status_transfer_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::drbs_subject_to_status_transfer_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::drbs_to_qos_flows_map_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::data_forwarding_resp_drb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::data_forwarding_resp_erab_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::erab_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::ecn_markingor_congestion_info_report_status_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::eutran_cell_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::eutran_cell_to_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::equivalent_sn_p_ns_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::excess_packet_delay_thres_cfg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::five_g_pro_se_pc5_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::forbidden_area_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::qos_flow_to_be_forwarded_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_per_tnl_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::up_transport_layer_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_list_with_cause_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::wlan_meas_cfg_name_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::sensor_meas_cfg_name_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::recommended_ran_node_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::notif_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::ngran_cell_to_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::ngran_cell_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::last_visited_pscell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::mbs_service_area_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_qos_flows_to_be_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_mapand_data_forwarding_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_active_session_info_sourceto_target_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_data_forwarding_resp_mrb_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_active_session_info_targetto_source_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_mbs_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::mbs_up_fail_ind_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_session_tnl_info5_gc_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_session_failedto_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_session_setup_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::mbs_session_setup_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::mbs_session_setupor_modify_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::mbs_session_to_release_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::rat_restricts_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::service_area_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::ue_paging_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::multicast_group_paging_area_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::ngap_ie_support_info_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::ngap_ie_support_info_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::ngran_tnl_assoc_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::slice_overload_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::overload_start_nssai_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::pc5_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::paging_policy_diff_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::pdu_session_for_paging_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_list_mt_comm_h_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_admitted_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_modify_list_mod_cfm_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_modify_list_mod_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_resume_list_res_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_resume_list_res_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_setup_list_cxt_fail_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_setup_list_cxt_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_setup_list_ho_ack_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_setup_list_ps_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_failed_to_setup_list_su_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::pdu_session_res_ho_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::pdu_session_res_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_list_cxt_rel_cpl_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_list_cxt_rel_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::pdu_session_res_list_ho_rqd_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_modify_confirm_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::up_transport_layer_info_pair_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::volume_timed_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flows_usage_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_modify_list_mod_cfm_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_modify_list_mod_ind_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_modify_list_mod_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_modify_list_mod_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::ul_ngu_up_tnl_modify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::qos_flow_add_or_modify_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::qos_flow_add_or_modify_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::pdu_session_res_notify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_notify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_feedback_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::qos_flow_add_info_list_rel_com_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::qos_flow_add_info_list_rel_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_released_list_not_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_released_list_ps_ack_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_released_list_ps_fail_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_released_list_rel_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_resume_list_res_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_resume_list_res_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_secondary_rat_usage_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_setup_list_cxt_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_setup_list_cxt_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_setup_list_ho_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_setup_list_su_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_setup_list_su_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_setup_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_tsc_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_suspend_list_sus_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_switched_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_to_be_switched_dl_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_to_release_list_ho_cmd_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::pdu_session_res_to_release_list_rel_cmd_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::plmn_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::partially_allowed_nssai_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_params_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::qos_flow_accepted_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::slice_support_list_qmc_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::ue_app_layer_meas_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::rants_scell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::rspp_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::ue_associated_lc_ng_conn_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::xn_ext_tlas_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::successful_ho_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::successful_pscell_change_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::served_guami_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::ue_history_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::supported_ta_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tai_list_for_paging_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::tnl_assoc_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::target_nssai_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::ngap::ue_presence_in_area_of_interest_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::ue_slice_max_bit_rate_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::ngap::unavailable_guami_list_l::value_type>::ops;
