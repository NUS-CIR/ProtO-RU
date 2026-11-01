// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/rrc_nr/bwp_cfg.h"
#include "ocudu/asn1/rrc_nr/cell_group_config.h"
#include "ocudu/asn1/rrc_nr/dl_ccch_msg_ies.h"
#include "ocudu/asn1/rrc_nr/dl_dcch_msg_ies.h"
#include "ocudu/asn1/rrc_nr/nr_sidelink_preconf.h"
#include "ocudu/asn1/rrc_nr/nr_ue_variables.h"
#include "ocudu/asn1/rrc_nr/pc5_rrc_definitions.h"
#include "ocudu/asn1/rrc_nr/pcch_msg_ies.h"
#include "ocudu/asn1/rrc_nr/radio_bearer_cfg.h"
#include "ocudu/asn1/rrc_nr/rrc_nr.h"
#include "ocudu/asn1/rrc_nr/serving_cell.h"
#include "ocudu/asn1/rrc_nr/sys_info.h"
#include "ocudu/asn1/rrc_nr/ue_cap.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg_ies.h"

using namespace asn1;
using namespace asn1::rrc_nr;

template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::srs_pos_cfg_r17_s::srs_pos_res_set_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::tx_hop_cfg_r18_s::slot_offset_for_remaining_hops_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::srs_pos_cfg_r17_s::srs_pos_res_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::srs_pos_rrc_inactive_agg_bw_add_carriers_r18_s::aggr_pos_srs_carrier_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::srs_inactive_pos_res_set_linked_for_agg_bw_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::srs_pos_rrc_inactive_validity_area_pre_cfg_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::reestab_ncell_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::band_combination_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::allowed_aggr_bw_sn_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::plmn_ran_area_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::plmn_ran_area_cfg_s::ran_area_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::plmn_ran_area_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::need_for_gaps_intra_freq_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::need_for_gaps_band_list_nr_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::need_for_ncsg_intra_freq_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::need_for_ncsg_band_list_nr_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::need_for_gap_ncsg_info_eutra_r17_s::need_for_ncsg_eutra_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::need_for_interruption_intra_freq_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::flight_path_info_report_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cg_candidate_list_r17_ies_s::cg_candidate_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cg_candidate_list_r17_ies_s::cg_candidate_to_release_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_cfg_sn_s::measured_frequencies_sn_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::fr_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ph_type_list_scg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::serv_cell_info_list_scg_nr_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::serv_cell_info_list_scg_eutra_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::candidate_cell_info_r17_s::candidate_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::candidate_cell_info_list_cp_c_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::candidate_serving_freq_range_list_nr_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::results_per_ssb_idx_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::results_per_csi_rs_idx_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::plmn_id_info_s::plmn_id_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::plmn_id_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::npn_id_r16_c::pni_npn_r16_s_::cag_id_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::npn_id_info_r16_s::npn_id_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::npn_id_info_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_nr_s::cho_cfg_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_result_list_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_result_list2_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_cell_list_sftd_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mrdc_assist_info_s::affected_carrier_freq_comb_info_list_mrdc_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::affected_carrier_freq_range_comb_r18_s::affected_carrier_freq_range_comb_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::affected_carrier_freq_range_comb_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::affected_carrier_freq_comb_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ph_type_list_mcg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cgi_info_eutra_s::cgi_info_epc_s_::cgi_info_epc_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::plmn_id_list_eutra_5_gc_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cgi_info_eutra_s::cgi_info_5_gc_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_cell_list_sftd_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::candidate_cell_list_cp_c_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mu_si_m_gap_cfg_r17_s::musim_gap_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mu_si_m_gap_cfg_r17_s::musim_gap_to_add_mod_list_ext_v1820_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mu_si_m_cell_to_affect_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::mu_si_m_affected_bands_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_excess_delay_cfg_r17_s::excess_delay_drb_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::event_trigger_cfg_s::cell_individual_offset_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::cand_pscell_execution_condition_info_r18_s::cand_pscell_exec_cond_report_cfg_nr_r18_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ue_cap_rat_container_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_serv_freq_list_eutra_scg_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_timing_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_rs_cell_mob_s::csi_rs_res_list_mob_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::atg_neigh_cell_cfg_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ra_prioritization_slice_info_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::rach_cfg_common_s::feature_combination_preambs_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::add_rach_cfg_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::affected_carrier_freq_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::affected_carrier_freq_range_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::cell_global_id_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::tracking_area_id_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::app_layer_meas_cfg_r17_s::meas_cfg_app_layer_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::area_cfg_r16_s::inter_freq_target_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::cag_cfg_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sn_pn_cfg_cell_id_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sn_pn_cfg_tai_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sn_pn_cfg_id_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::availability_combination_rb_groups_r17_s::rb_set_groups_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::availability_combinations_per_cell_r16_s::availability_combinations_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::availability_combinations_per_cell_r16_s::availability_combinations_rb_groups_r17_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::availability_ind_r16_s::available_comb_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::nr_ns_pmax_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::multi_freq_band_list_nr_sib_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ssb_mtc4_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::nr_ns_pmax_list_v1760_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::multi_freq_band_list_nr_sib_v1760_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::nr_ns_pmax_list_aerial_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::multi_freq_band_list_nr_aerial_sib_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::intra_freq_neigh_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::intra_freq_excluded_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::intra_freq_neigh_cell_list_v1610_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sib3_s::intra_freq_cag_cell_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::intra_freq_neigh_cell_list_v1710_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::inter_freq_neigh_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_carrier_freq_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_neigh_cell_list_v1610_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::inter_freq_carrier_freq_info_v1610_s::inter_freq_cag_cell_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_carrier_freq_list_v1610_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_neigh_cell_list_v1710_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_carrier_freq_list_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_carrier_freq_list_v1720_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_carrier_freq_list_v1730_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_carrier_freq_list_v1760_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::inter_freq_carrier_freq_list_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::eutra_ns_pmax_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::eutra_multi_band_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::eutra_freq_neigh_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::eutra_freq_excluded_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::carrier_freq_list_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::carrier_freq_list_eutra_v1610_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::carrier_freq_list_eutra_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::eutra_multi_band_info_list_aerial_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::carrier_freq_list_eutra_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::hrnn_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_idle_cfg_sib_r16_s::meas_idle_carrier_list_nr_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_idle_cfg_sib_r16_s::meas_idle_carrier_list_eutra_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_idle_cfg_sib_r16_s::meas_resel_carrier_list_nr_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sib15_r17_s::applicable_disaster_info_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::slice_info_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::freq_prio_list_slicing_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sib18_r17_s::gin_elem_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sib18_r17_s::gins_per_sn_pn_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ntn_neigh_cell_cfg_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdsch_cfg_broadcast_r17_s::pdsch_cfg_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdsch_time_domain_res_alloc_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::pdsch_cfg_broadcast_r17_s::rate_match_pattern_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::eutra_mbsfn_sf_cfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mbs_fsai_inter_freq_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::coverage_area_info_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sys_info_ies_s::sib_type_and_info_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pos_sys_info_r16_ies_s::pos_sib_type_and_info_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cell_access_related_info_s::snpn_access_info_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sib_map_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::si_sched_info_s::sched_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::si_request_cfg_s::si_request_res_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::freq_info_dl_sib_s::scs_specific_carrier_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdcch_cfg_common_s::common_search_space_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdcch_cfg_common_s::common_search_space_list_ext_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdcch_cfg_common_s::common_search_space_list_ext2_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdcch_cfg_common_s::common_search_space_list_ext_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdsch_time_domain_res_alloc_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_time_domain_res_alloc_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::bwp_ul_common_s::add_rach_per_pci_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::uac_barr_per_cat_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::uac_barr_per_plmn_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::uac_barr_info_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::pos_sib_map_info_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pos_si_sched_info_r16_s::pos_sched_info_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::si_request_cfg_repeat_r18_s::si_request_res_repeat_num2_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::uac_barr_info_set_list_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sib_map_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::si_sched_info_v1700_s::sched_info_list2_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdcch_cfg_s::coreset_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::dl_preemption_s::int_cfg_per_serving_cell_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_cancellation_r16_s::ci_cfg_per_serving_cell_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdsch_cfg_s::tci_states_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::rate_match_pattern_group_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdsch_cfg_s::zp_csi_rs_res_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdsch_cfg_s::aperiodic_zp_csi_rs_res_sets_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::dummy_tdra_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::radio_link_monitoring_cfg_s::fail_detection_res_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::beam_fail_detection_set_r17_s::bfd_res_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sps_cfg_to_add_mod_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::beam_fail_recovery_rs_cfg_r16_s::candidate_beam_rs_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::dl_ppw_pre_cfg_to_add_mod_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::res_set_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::res_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::sched_request_res_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::spatial_relation_info_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::pucch_pwr_ctrl_s::p0_set_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_pwr_ctrl_s::pathloss_ref_rss_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::pathloss_ref_rss_v1610_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::res_to_add_mod_list_ext_v1610_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::spatial_relation_info_to_add_mod_list_ext_v1610_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::res_group_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sps_pucch_an_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::sched_request_res_to_add_mod_list_ext_v1610_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::pwr_ctrl_set_info_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pucch_cfg_s::sched_request_res_to_add_mod_list_ext_v1700_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_pwr_ctrl_s::p0_alpha_sets_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_pwr_ctrl_s::pathloss_ref_rs_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_pwr_ctrl_s::sri_pusch_map_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::uci_on_pusch_list_dci_0_2_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_time_domain_res_alloc_r16_s::pusch_alloc_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_time_domain_res_alloc_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::uci_on_pusch_list_dci_0_1_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::pusch_pwr_ctrl_v1610_s::pathloss_ref_rs_to_add_mod_list_size_ext_v1610_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_pwr_ctrl_v1610_s::p0_pusch_set_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_pwr_ctrl_v1610_s::dummy_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pusch_cfg_s::mpe_res_pool_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::cg_uci_on_pusch_c::dyn_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cfg_grant_cfg_s::cg_cot_sharing_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cfg_grant_cfg_s::cg_cot_sharing_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::pathloss_ref_rs_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::srs_cfg_s::srs_res_set_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::srs_cfg_s::srs_res_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::beam_fail_recovery_cfg_s::candidate_beam_rs_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::pucch_cfg_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cfg_grant_cfg_to_add_mod_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::bwp_ul_ded_s::ul_tci_state_list_r17_c_::explicitlist_s_::ul_tci_to_add_mod_list_r17_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::bwp_ul_ded_s::pathloss_ref_rs_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_s::band_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_params_v1540_s::srs_carrier_switch_c_::nr_s_::srs_switching_times_list_nr_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_params_v1540_s::srs_carrier_switch_c_::eutra_s_::srs_switching_times_list_eutra_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_v1540_s::band_list_v1540_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_v1610_s::band_list_v1610_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_combination_ul_tx_switch_r16_s::supported_band_pair_list_nr_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_v1630_s::scaling_factor_tx_sidelink_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1640_s::two_pucch_grp_cfgs_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ca_params_nr_v1690_s::csi_report_cross_pucch_grp_r16_s_::carrier_type_pair_list_r16_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v16a0_s::pdcch_blind_detection_mixed_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1700_s::mtrp_csi_enhancement_per_bc_r17_s_::
                        supported_combo_across_ccs_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_v1700_s::band_list_v1710_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_combination_ul_tx_switch_v1700_s::supported_band_pair_list_nr_v1700_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_combination_ul_tx_switch_v1700_s::ul_tx_switching_band_params_list_v1700_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ca_params_nr_v1720_s::semi_static_pucch_cell_switch_two_groups_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1720_s::pdcch_blind_detection_mcg_scg_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1720_s::pdcch_blind_detection_mixed_list1_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1720_s::pdcch_blind_detection_mixed_list3_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_v1730_s::band_list_v1730_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_v1770_s::band_list_v1770_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_v1780_s::band_list_v1780_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_combination_v1790_s::supported_intra_endc_band_combination_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::codebook_paramsetype2_doppler_csi_r18_s::etype2_doppler_n4_r18_s_::
                        supported_csi_rs_report_setting_list1_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1800_s::non_codebook_csi_rs_srs_per_bc_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1800_s::multi_cell_pdsch_dci_1_3_diff_scs_r18_s_::
                        combination_carrier_type_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1800_s::pdcch_blind_detection_ca_mixed_ext_r18_c_::
                        pdcch_blind_detection_ca_mixed_r18_s_::blind_detection_ca_mixed_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nr_v1800_s::pdcch_blind_detection_mcg_scg_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ca_params_nrdc_v1800_s::pdcch_blind_detection_nrdc_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_v1800_s::band_list_v1810_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
                                           asn1::dyn_array<asn1::rrc_nr::ul_tx_switching_band_pair_r18_s::
                                                               ul_tx_switching_period_unaffected_band_dual_ul_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_combination_ul_tx_switch_v1800_s::supported_band_pair_list_nr_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_combination_ul_tx_switch_v1800_s::ul_tx_switching_add_period_dual_ul_list_r18_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_combination_ul_tx_switch_v1840_s::supported_band_pair_list_nr_v1840_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::band_combination_v1680_s::intraband_concurrent_operation_pwr_class_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::band_combination_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1630_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1640_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1650_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1670_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1690_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v16a0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v16e0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v16j0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1720_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1730_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1740_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1760_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1770_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1780_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1790_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v17b0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1830_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1840_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_ul_tx_switch_v1860_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1540_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1550_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1560_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1570_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1580_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1590_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v15g0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v15n0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1610_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1630_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1640_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1650_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1680_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1690_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v16a0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v16j0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1720_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1730_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1740_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1760_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1770_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1780_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1790_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v17b0_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1830_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1840_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_v1860_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_list_sl_discovery_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_params_sidelink_eutra_nr_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_params_sidelink_eutra_nr_v1630_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_params_sidelink_eutra_nr_v1710_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_nr_s::prs_processing_cap_outside_m_gin_ppw_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::lower_msd_r18_s::msd_info_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::band_nr_s::lower_msd_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cc_group_r17_s::offset_to_default_r17_c_::offsetlist_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cfra_s::res_c_::ssb_s_::ssb_res_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cfra_s::res_c_::csirs_s_::csirs_res_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::srs_res_list_cfg_cli_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::rssi_res_list_cfg_cli_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_aperiodic_trigger_state_s::associated_report_cfg_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_aperiodic_trigger_state_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::nzp_csi_rs_res_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::nzp_csi_rs_res_set_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::csi_im_res_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::csi_im_res_set_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::csi_ssb_res_set_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::csi_res_cfg_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::csi_report_cfg_s::report_cfg_type_c_::periodic_s_::pucch_csi_res_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_report_cfg_s::non_pmi_port_ind_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_report_cfg_s::csi_report_sub_cfg_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::csi_report_cfg_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_semi_persistent_on_pusch_trigger_state_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::scell_activation_rs_cfg_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_meas_cfg_s::ltm_csi_report_cfg_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::csi_rs_res_cfg_mob_s::csi_rs_cell_list_mob_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cell_group_cfg_s::rlc_bearer_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sched_request_cfg_s::sched_request_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::tag_cfg_s::tag_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sched_request_cfg_v1700_s::sched_request_to_add_mod_list_ext_v1700_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mac_cell_group_cfg_s::g_rnti_cfg_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mac_cell_group_cfg_s::dsr_cfg_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::phys_cell_group_cfg_s::pdsch_harq_ack_enh_type3_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::tdd_ul_dl_cfg_ded_s::slot_specific_cfgs_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::serving_cell_cfg_s::dl_bwp_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_cfg_s::ul_bwp_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::srs_tpc_pdcch_cfg_s::srs_cc_set_idxlist_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::srs_carrier_switching_s::srs_tpc_pdcch_group_c_::type_a_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::slot_format_combinations_per_cell_s::slot_format_combinations_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::slot_format_ind_s::slot_format_comb_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::slot_format_ind_s::available_rb_sets_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::slot_format_ind_s::switch_trigger_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::slot_format_ind_s::co_durs_per_cell_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::slot_format_ind_s::co_durs_per_cell_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pdsch_code_block_group_tx_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::tdd_ul_dl_cfg_ded_iab_mt_r16_s::slot_specific_cfgs_to_add_mod_list_iab_mt_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::intra_cell_guard_bands_per_scs_r16_s::intra_cell_guard_bands_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::serving_cell_cfg_s::intra_cell_guard_bands_dl_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::lte_crs_pattern_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::nr_dl_prs_pdc_res_set_r17_s::res_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mimo_param_r17_s::add_pci_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mimo_param_r17_s::ul_pwr_ctrl_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::lte_neigh_cells_crs_assist_info_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::serving_cell_cfg_s::mc_dci_set_of_cells_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mimo_param_v1850_s::add_tdd_cfg_per_pci_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cell_group_cfg_s::scell_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cell_group_cfg_s::bh_rlc_ch_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cell_group_cfg_s::uu_relay_rlc_ch_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cell_group_cfg_s::iab_res_cfg_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::intra_band_cc_combination_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::report_ul_tx_direct_current_more_carrier_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ncr_periodic_fwd_res_set_r18_s::periodic_fwd_rsrc_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ncr_fwd_cfg_r18_s::periodic_fwd_rsrc_set_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ncr_aperiodic_fwd_cfg_r18_s::aperiodic_fwd_time_rsrc_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ncr_semi_persistent_fwd_res_set_r18_s::semi_persistent_fwd_rsrc_to_add_mod_list_r18_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ncr_fwd_cfg_r18_s::semi_persistent_fwd_rsrc_set_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_tx_switching_band_pair_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_tx_switching_associated_band_dual_ul_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::freq_prio_list_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::freq_prio_list_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::slice_info_list_ded_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::freq_prio_list_ded_slicing_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::cells_to_add_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cells_to_add_mod_list_ext_v1710_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cells_to_add_mod_list_ext_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cells_to_add_mod_list_utra_fdd_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cho_candidate_cell_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cond_execution_cond_to_add_mod_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::cond_recfg_to_add_mod_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sk_counter_configuration_r18_s::sk_counter_cfg_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::log_meas_result_list_bt_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::log_meas_result_list_wlan_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_result_list2_nr_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_result_list_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_list2_eutra_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::per_ra_attempt_info_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::per_ra_info_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::conn_est_fail_report_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::drb_count_msb_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::drb_count_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::srb_to_add_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::drb_to_add_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::mrb_to_add_mod_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_cg_max_trans_num_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sl_cfg_grant_cfg_list_r16_s::sl_cfg_grant_cfg_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
                                           asn1::dyn_array<asn1::rrc_nr::sl_cfg_grant_cfg_ded_sl_prs_rp_list_r18_s::
                                                               sl_cfg_grant_cfg_ded_sl_prs_rp_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_min_max_mcs_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_pssch_tx_cfg_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_cbr_common_tx_cfg_list_r16_s::sl_cbr_pssch_tx_cfg_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sl_cbr_common_tx_ded_sl_prs_rp_list_r18_s::sl_cbr_sl_prs_tx_cfg_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_unlicensed_r18_s::sl_rb_set_cfg_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_cbr_prio_tx_cfg_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_ue_sel_cfg_rp_r16_s::sl_res_reserve_period_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_cbr_prio_tx_cfg_list_v1650_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_inter_ue_coordination_scheme1_r17_s::
                        sl_thres_rsrp_condition1_b_1_option1_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_res_pool_r16_s::sl_prs_res_shared_sl_prs_rp_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_bwp_pool_cfg_r16_s::sl_rx_pool_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_pool_ded_r16_s::sl_pool_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_prs_res_pool_r18_s::sl_prs_res_reserve_period_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_prs_res_pool_r18_s::sl_prs_res_ded_sl_prs_rp_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_prs_res_pool_r18_s::sl_cbr_prio_tx_cfg_ded_sl_prs_rp_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_bwp_prs_pool_cfg_r18_s::sl_prs_rx_pool_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_prs_tx_pool_ded_r18_s::sl_prs_pool_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_freq_cfg_r16_s::sl_bwp_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_sync_cfg_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_phy_mac_rlc_cfg_r16_s::sl_freq_info_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_phy_mac_rlc_cfg_r16_s::sl_rlc_bearer_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sl_sdap_cfg_r16_s::sl_mapped_qos_flows_r16_c_::sl_mapped_qos_flows_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_cfg_ded_nr_r16_s::sl_radio_bearer_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_meas_obj_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_report_cfg_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_meas_id_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_cfg_ded_nr_r16_s::sl_meas_cfg_info_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_drx_cfg_gc_bc_r17_s::sl_drx_gc_bc_per_qos_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_drx_cfg_r17_s::sl_drx_cfg_uc_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_phy_mac_rlc_cfg_v1700_s::sl_rlc_ch_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_freq_cfg_ext_v1800_s::sl_freq_sel_cfg_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sl_phy_mac_rlc_cfg_v1700_s::sl_freq_info_to_add_mod_list_ext_v1800_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_s_cch_carrier_set_cfg_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_srap_cfg_r17_s::sl_map_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_srap_cfg_r17_s::sl_map_to_release_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_srap_cfg_u2_u_r18_s::sl_map_to_add_mod_u2_u_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sl_u2_u_relay_ue_cfg_r18_s::sl_target_remote_ue_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_l2_remote_ue_cfg_r17_s::sl_u2_u_relay_ue_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ssb_position_qcl_cells_to_add_mod_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ssb_position_qcl_cell_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ssb_to_measure_altitude_based_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_obj_nr_s::excluded_cells_to_add_mod_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ssb_mtc3_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_obj_eutra_s::cells_to_add_mod_list_eutran_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_obj_eutra_s::excluded_cells_to_add_mod_list_eutran_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_obj_to_add_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::event_trigger_cfg_inter_rat_s::cell_individual_offset_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::report_cfg_to_add_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_id_to_add_mod_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::quant_cfg_s::quant_cfg_nr_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_gap_cfg_s::gap_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::pos_meas_gap_pre_cfg_to_add_mod_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::iab_ip_address_cfg_list_r16_s::iab_ip_address_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::prop_delay_diff_report_cfg_r17_s::neigh_cell_info_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_l2_relay_ue_cfg_r17_s::sl_remote_ue_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sl_u2_u_remote_ue_cfg_r18_s::sl_source_remote_ue_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_l2_relay_ue_cfg_r17_s::sl_u2_u_remote_ue_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::n3_c_indirect_path_cfg_relay_r18_s::n3c_map_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ul_traffic_info_report_cfg_r18_s::pdu_sessions_to_report_ul_traffic_info_list_r18_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ltm_tci_info_r18_s::ltm_dl_or_joint_tci_state_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ltm_tci_info_r18_s::ltm_ul_tci_state_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ltm_cfg_r18_s::ltm_candidate_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ltm_cfg_r18_s::ltm_csi_res_cfg_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::validity_area_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ue_cap_rat_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ded_sib_request_r16_ies_s::on_demand_sib_request_list_r16_s_::
                        requested_pos_sib_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::eutra_rstd_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::fail_report_indirect_path_r18_s::n3c_relay_ue_info_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_list_utra_fdd_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_result_list2_utra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::per_ra_info_list_v1660_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::per_ra_info_list_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::feature_sets_per_band_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::feature_set_dl_s::dummy4_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::feature_set_dl_s::dummy5_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::feature_set_dl_s::dummy6_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::feature_set_dl_s::dummy7_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_set_dl_v1800_s::dummy_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::feature_set_dl_v1830_s::pdcch_rach_affected_bands_target_band_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::feature_set_dl_v1830_s::pdcch_rach_switching_time_target_band_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::feature_set_dl_v1830_s::pdcch_rach_prep_time_target_band_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_per_cc_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_per_cc_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v1540_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_v1540_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_per_cc_v1540_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v15a0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v1610_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_v1610_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_set_dl_per_cc_v1620_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_v1630_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_v1640_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v1700_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_per_cc_v1700_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_v1710_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_per_cc_v1700_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v1720_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_per_cc_v1720_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_v1720_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v1730_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_per_cc_v1730_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_per_cc_v1780_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_per_cc_v1780_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v1800_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_per_cc_v1800_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_v1800_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_per_cc_v1800_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v1830_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_per_cc_v1840_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_per_cc_v1840_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_v1850_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_ul_per_cc_v1850_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_s::feature_sets_dl_v1860_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_v15t0_s::feature_sets_dl_v15t0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_v16d0_s::feature_sets_ul_v16d0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_v16k0_s::feature_sets_dl_v16k0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::feature_sets_v17d0_s::feature_sets_dl_v17d0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::freq_band_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::iab_ip_address_and_traffic_r16_s::all_traffic_iab_ip_address_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::utra_fdd_params_r16_s::supported_band_list_utra_fdd_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::nr_prs_meas_info_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_list_logging_nr_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_list_logging2_nr_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::log_meas_info_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mbs_neighbour_cell_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mbs_non_serving_info_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::mbs_service_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::mrb_list_broadcast_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::mbs_session_info_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::mrb_list_multicast_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mbs_session_info_list_multicast_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mbs_broadcast_cfg_r17_ies_s::drx_cfg_ptm_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::mtch_ssb_map_win_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::mbs_multicast_cfg_r18_ies_s::thres_mbs_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::mu_si_m_gap_pref_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_list_srs_rsrp_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_list_cli_rssi_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::meas_results_per_carrier_idle_eutra_r16_s::meas_results_per_cell_list_idle_eutra_r16_l_::value_type>::
    ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::meas_result_idle_eutra_r16_s::meas_results_per_carrier_list_idle_eutra_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::results_per_ssb_idx_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::meas_results_per_carrier_idle_nr_r16_s::meas_results_per_cell_list_idle_nr_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::meas_result_idle_nr_r16_s::meas_results_per_carrier_list_idle_nr_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_nr_sl_r16_s::meas_result_list_cbr_nr_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_nr_sl_r18_s::meas_result_list_cbr_ded_sl_prs_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_result_neigh_freq_list_rssi_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::meas_result_serv_mo_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_pdcp_delay_value_result_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_pdcp_excess_delay_result_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_report_app_layer_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::meas_report_app_layer_list_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::visited_pscell_info_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::visited_cell_info_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::paging_record_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::paging_record_list_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::paging_group_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::paging_record_list_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::paging_group_list_v1800_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::pdu_session_ul_traffic_info_r18_s::qos_flow_ul_traffic_info_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::phy_params_mrdc_s::naics_cap_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::reported_feature_combination_r18_s::triggered_s_nssai_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ra_info_common_r16_s::attempted_bwp_info_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ra_report_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::rf_params_s::supported_band_list_nr_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::rf_params_v16c0_s::supported_band_list_nr_v16c0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::rf_params_v17b0_s::supported_band_list_nr_v17b0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sl_phy_mac_rlc_cfg_v16k0_s::sl_freq_info_to_add_mod_list_ext_v16k0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_tx_direct_current_cell_s::ul_direct_current_bwp_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ul_tx_direct_current_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_tx_direct_current_two_carrier_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ul_tx_direct_current_more_carrier_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sdt_mac_phy_cg_cfg_r17_s::cg_sdt_cfg_lch_restrict_to_add_mod_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sdt_mac_phy_cg_cfg_r17_s::cg_sdt_cfg_lch_restrict_to_add_mod_list_ext_v1800_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_freq_cfg_common_r16_s::sl_bwp_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_freq_cfg_common_r16_s::sl_pos_bwp_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_cfg_common_nr_r16_s::sl_freq_info_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_cfg_common_nr_v16k0_s::sl_freq_info_list_ext_v16k0_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sib17_ies_r17_s::trs_res_set_cfg_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sib17bis_ies_r18_s::trs_res_set_cfg_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_carrier_fail_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_fail_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_prs_ue_assist_info_nr_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_pos_tx_res_req_r18_s::sl_pos_qos_info_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_pos_tx_res_req_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_rlc_mode_ind_r16_s::sl_qos_info_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_rx_drx_report_list_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_rx_interested_gc_bc_dest_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_r16_s::sl_rlc_mode_ind_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_v1700_s::sl_drx_info_from_rx_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_v1700_s::sl_qos_info_list_v1800_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_u2_u_info_r18_s::sl_per_hop_qos_info_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_u2_u_info_r18_s::sl_per_slrb_qos_info_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_l2_u2_u_r18_s::sl_u2_u_info_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_l2_u2_u_r18_s::sl_rlc_mode_ind_list_l2_u2_u_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_list_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_list_v1700_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_list_comm_relay_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_res_req_list_disc_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_ue_assist_info_nr_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sidelink_params_nr_r16_s::supported_band_list_sidelink_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::sidelink_params_eutra_r16_s::supported_band_list_sidelink_eutra_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sidelink_ue_info_nr_v1800_ies_s::sl_tx_res_req_list_l2_u2_u_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ue_cap_request_filt_common_s::requested_cell_grouping_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ue_tx_teg_assoc_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::ul_traffic_info_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::sl_tx_profile_list_r17_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::cells_triggered_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::var_app_layer_idle_cfg_r18_s::app_layer_idle_cfg_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::var_app_layer_plmn_list_cfg_r18_s::plmn_cfg_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::var_conn_est_fail_report_list_r17_s::conn_est_fail_report_list_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::rrc_nr::var_meas_report_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::var_meas_report_list_sl_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::var_ra_report_r16_s::id_list_r18_c_::snpn_id_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_params_sidelink_nr_r16_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::band_combination_params_sidelink_nr_v1710_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::rrc_recfg_sidelink_r16_ies_s::slrb_cfg_to_add_mod_list_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::rrc_recfg_sidelink_v1700_ies_s::sl_rlc_ch_to_add_mod_list_pc5_r17_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::rrc_recfg_sidelink_v1800_ies_s::sl_carrier_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::rrc_recfg_sidelink_v1800_ies_s::sl_rlc_bearer_to_add_mod_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::rrc_recfg_sidelink_v1800_ies_s::sl_local_id_pair_list_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_requested_pos_sib_list_r18_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ue_cap_info_sidelink_r16_ies_s::supported_band_list_sidelink_r16_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::rrc_nr::ue_info_request_sidelink_r18_ies_s::sl_e2_e_qos_info_list_pc5_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::ue_info_resp_sidelink_r18_ies_s::sl_split_qos_info_list_pc5_r18_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_meas_result_list_relay_r17_l::value_type>::ops;
