// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/e1ap/e1ap_ies.h"
#include "ocudu/asn1/e1ap/e1ap_pdu_contents.h"

using namespace asn1;
using namespace asn1::e1ap;

template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_activity_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_activity_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::alt_qos_para_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::location_dependent_mbsf1_u_info_at_cu_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::location_dependent_mbsf1_u_info_at_du_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::location_dependent_mb_sn_gu_info_at5_gc_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::location_dependent_mb_sn_gu_info_at_ngran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::mb_sn_gu_info_at_ngran_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::qos_flow_qos_param_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::f1_u_tnl_info_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::bcmrb_setup_cfg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::f1_u_tnl_info_to_add_or_modify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::f1_u_tnl_info_to_release_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::bcmrb_modify_cfg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::qos_flow_failed_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::f1_u_tnl_info_added_or_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::bcmrb_setup_modify_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::bcmrb_failed_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::f1_u_tnl_info_added_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::bcmrb_setup_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::cell_group_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::crit_diagnostics_ie_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_confirm_modified_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_confirm_modified_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_failed_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_failed_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_failed_mod_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_failed_mod_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_failed_to_modify_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_failed_to_modify_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_meas_results_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::up_params_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_modified_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_modified_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_remd_item_s::qos_flow_remd_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::gnb_cu_up_cell_group_related_cfg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_required_to_modify_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_required_to_modify_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_required_to_rem_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drb_required_to_rem_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::data_forwardingto_ng_ran_qos_flow_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_setup_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_setup_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_setup_mod_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_setup_mod_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_to_modify_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_to_modify_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_to_rem_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_to_rem_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::qos_flow_map_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_to_setup_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_to_setup_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_to_setup_mod_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_to_setup_mod_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_usage_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drbs_subject_to_counter_check_list_eutran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drbs_subject_to_counter_check_list_ng_ran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::drbs_subject_to_early_forwarding_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::data_usage_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::data_usage_per_pdu_session_report_s::pdu_session_timed_report_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::data_usage_per_qos_flow_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::qos_flows_to_be_forwarded_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::data_forwardingto_e_utran_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::ecgi_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::eutran_qos_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::extended_nr_cgi_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::extended_slice_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mbs_support_info_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mbs_support_info_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::gtp_tlas_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mbs_service_area_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mbs_service_area_tai_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mbs_service_area_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::mbs_session_associated_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mcmrb_setup_modify_cfg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::mrb_forwarding_res_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mrb_forwarding_res_ind_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mcmrb_modify_confirm_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mcmrb_modify_required_cfg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mcmrb_setup_modify_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mcmrb_failed_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mcmrb_setup_cfg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mrb_forwarding_res_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::mcmrb_setup_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::ng_ran_qos_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::nr_cgi_support_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_confirm_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_data_usage_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::pdu_session_res_failed_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_failed_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_failed_to_modify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_required_to_modify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::pdu_session_res_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_to_modify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::pdu_session_res_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_to_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::pdu_session_res_to_setup_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::pdu_session_to_notify_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::retainability_meass_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::transport_up_layer_addresses_info_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::transport_up_layer_addresses_info_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::ue_slice_max_bit_rate_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::drb_status_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::dl_up_tnl_address_to_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::gnb_cu_cp_tnl_a_to_add_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::gnb_cu_cp_tnl_a_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::gnb_cu_cp_tnl_a_to_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::gnb_cu_cp_tnl_a_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::gnb_cu_cp_tnl_a_failed_to_setup_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::supported_plmns_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::gnb_cu_up_tnl_a_to_rem_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e1ap::iab_donor_cu_up_psk_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::ul_up_tnl_address_to_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::ue_associated_lc_e1_conn_list_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e1ap::ue_associated_lc_e1_conn_list_res_ack_l::value_type>::ops;
