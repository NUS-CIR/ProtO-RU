// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/e2sm/e2sm.h"
#include "ocudu/asn1/e2sm/e2sm_common_ies.h"
#include "ocudu/asn1/e2sm/e2sm_kpm_ies.h"

using namespace asn1;
using namespace asn1::e2sm;

template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::supported_sul_band_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::nr_freq_band_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::ue_id_gnb_cu_f1ap_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::ue_id_gnb_cu_cp_e1ap_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::bin_range_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::dist_meas_bin_range_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::label_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::match_cond_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::meas_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::matching_cond_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::meas_cond_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::matching_ue_cond_per_sub_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::matching_ue_id_per_sub_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::multi_report_action_definition_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::event_param_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::event_triger_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::meas_record_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::meas_data_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::matching_ue_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::matching_ue_id_list_per_gp_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::matching_ue_id_per_gp_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::meas_cond_ue_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::ue_meas_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::multi_report_ind_msg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_kpm_ra_nfunction_description_s::ric_event_trigger_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::meas_info_action_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_kpm_ra_nfunction_description_s::ric_report_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ric_integrated_report_style_item_s::ric_fundamental_level_report_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::e2sm_kpm_ra_nfunction_description_s::ric_integrated_report_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::ran_param_testing_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::associated_ue_info_s::associated_ue_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_param_definition_choice_list_s::ran_param_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_param_definition_choice_structure_s::ran_param_structure_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_action_definition_format1_s::ran_p_to_be_reported_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_param_structure_s::seq_of_ran_params_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_param_list_s::list_of_ran_param_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ric_policy_action_s::ran_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_action_definition_format2_s::ric_policy_conditions_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_action_definition_format3_s::ran_p_insert_ind_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::e2sm_rc_action_definition_format4_ind_item_s::ran_p_insert_ind_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::e2sm_rc_action_definition_format4_style_item_s::ric_insert_ind_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_action_definition_format4_s::ric_insert_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ue_group_definition_s::ue_group_definition_id_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_msg_format1_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_msg_format2_style_item_s::ric_ctrl_action_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_msg_format2_s::ric_ctrl_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_entity_filt_s::entity_specific_ctrl_ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_msg_format3_s::list_of_entity_filts_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_msg_format3_s::entity_agnostic_ctrl_ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_msg_format4_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_outcome_format1_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_outcome_format2_ctrl_outcome_item_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_outcome_format2_style_item_s::ric_ctrl_outcome_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_outcome_format2_s::ric_ctrl_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ctrl_outcome_format3_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::event_trigger_ue_info_s::ue_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::event_trigger_ue_event_info_s::ue_event_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_event_trigger_format1_s::msg_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::event_trigger_cell_info_s::cell_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_event_trigger_format3_s::e2_node_info_change_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::trigger_type_choice_rrc_state_s::rrc_state_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_event_trigger_format4_s::ue_info_change_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ind_msg_format1_s::ran_p_reported_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ind_msg_format2_item_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ind_msg_format2_s::ue_param_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2sm::neighbour_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ind_msg_format3_s::cell_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ind_msg_format5_s::ran_p_requested_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ind_msg_format6_ind_item_s::ran_p_insert_ind_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ind_msg_format6_style_item_s::ric_insert_ind_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_ind_msg_format6_s::ric_insert_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_query_definition_format1_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_query_outcome_format1_item_cell_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_query_outcome_format1_s::cell_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_query_outcome_format2_item_ue_s::ran_p_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::e2sm_rc_query_outcome_format2_s::ue_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ran_function_definition_event_trigger_s::ric_event_trigger_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_event_trigger_s::ran_l2_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_event_trigger_breakpoint_item_s::
                        ran_call_process_breakpoint_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_event_trigger_call_process_item_s::
                        call_process_breakpoints_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ran_function_definition_event_trigger_s::ran_call_process_types_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ran_function_definition_event_trigger_s::ran_ue_identif_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ran_function_definition_event_trigger_s::ran_cell_identif_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_report_item_s::ran_report_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_report_s::ric_report_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ran_function_definition_insert_ind_item_s::ran_insert_ind_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_insert_item_s::ric_insert_ind_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_insert_s::ric_insert_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ran_function_definition_ctrl_action_item_s::ran_ctrl_action_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_ctrl_item_s::ric_ctrl_action_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_ctrl_item_s::ran_ctrl_outcome_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::list_of_add_supported_formats_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_ctrl_s::ric_ctrl_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ran_function_definition_policy_action_item_s::ran_policy_action_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::e2sm::ran_function_definition_policy_action_item_s::ran_policy_condition_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_policy_item_s::ric_policy_action_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_policy_s::ric_policy_style_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_query_item_s::ran_query_params_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2sm::ran_function_definition_query_s::ric_query_style_list_l_::value_type>::ops;
