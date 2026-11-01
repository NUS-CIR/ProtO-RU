// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/nrppa/nrppa_ies.h"
#include "ocudu/asn1/nrppa/nrppa_pdu_contents.h"

using namespace asn1;
using namespace asn1::nrppa;

template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::arp_location_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::combined_pos_srs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::pos_srs_res_set_aggregation_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::srs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::slot_offset_for_remaining_hops_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_srs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::srs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_srs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::multiple_ul_ao_a_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::add_path_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::dl_prs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::aggr_prs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::aggr_pos_srs_res_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_sib_segments_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_sibs_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::sys_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::assist_info_fail_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trp_elevation_angle_list_item_s::trp_beam_pwr_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trp_beam_ant_angles_list_item_s::trp_elevation_angle_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_beam_ant_angles_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::crit_diagnostics_ie_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::dl_prsbw_aggregation_request_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::dl_prs_res_set_arp_s::listof_dl_prs_res_arp_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::dl_prs_res_coordinates_s::listof_dl_prs_res_set_arp_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_rsrp_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_rsrq_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_ss_rsrp_per_ssb_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_ss_rsrp_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_ss_rsrq_per_ssb_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_ss_rsrq_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_csi_rsrp_per_csi_rs_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_csi_rsrp_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_csi_rsrq_per_csi_rs_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_csi_rsrq_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::measured_results_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::measured_results_associated_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::extended_add_path_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::meas_quantities_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::nr_prs_beam_info_item_s::prs_angle_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::nr_prs_beam_info_s::nr_prs_beam_info_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::nr_prs_beam_info_s::lcs_to_gcs_translation_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::otdoa_cell_info_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::otdoa_cells_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_geran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_utran_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_nr_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_eutra_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::other_rat_meas_quantities_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::other_rat_meas_result_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_meass_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::prsbw_aggregation_request_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::requested_dl_prs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::requested_dl_prs_res_set_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_tx_off_per_res_set_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::prs_tx_off_per_res_item_s::prs_tx_off_ind_per_res_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_tx_off_per_res_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prstrp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_tx_trp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_validity_area_cell_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::positioning_broadcast_cells_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::spatial_relationfor_res_id_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::spatial_relation_per_srs_res_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::requested_srs_tx_characteristics_s::list_of_srs_res_set_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::ssb_info_s::list_of_ssb_info_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::requested_srs_precfg_characteristics_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::ul_ch_bw_per_scs_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::srs_carrier_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::srs_precfg_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_result_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_prs_info_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trpteg_item_s::dl_prs_res_id_list_l_::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_tx_teg_assoc_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_info_type_resp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_info_list_trp_resp_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_info_type_list_trp_req_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_quantities_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::time_win_info_meas_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::time_win_info_srs_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::ue_tx_teg_assoc_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::wlan_meas_quantities_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::wlan_meas_result_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::otdoa_info_type_l::value_type>::ops;
