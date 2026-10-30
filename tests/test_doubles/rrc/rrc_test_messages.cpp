// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "rrc_test_messages.h"
#include "lib/rrc/ue/rrc_asn1_converters.h"
#include "lib/rrc/ue/rrc_asn1_helpers.h"
#include "ocudu/asn1/rrc_nr/common.h"
#include "ocudu/asn1/rrc_nr/ue_cap.h"
#include "ocudu/asn1/rrc_nr/ul_ccch_msg_ies.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg_ies.h"
#include "ocudu/ran/plmn_identity.h"

using namespace ocudu;
using namespace asn1::rrc_nr;

ul_ccch_msg_s ocudu::test_helpers::create_rrc_setup_request()
{
  ul_ccch_msg_s msg;

  rrc_setup_request_s& req = msg.msg.set_c1().set_rrc_setup_request();

  req.rrc_setup_request.establishment_cause.value = establishment_cause_opts::mo_sig;
  req.rrc_setup_request.ue_id.set_random_value().from_string("111000010100010001111111000010001100111");

  return msg;
}

ul_ccch_msg_s
ocudu::test_helpers::create_rrc_reestablishment_request(rnti_t old_crnti, pci_t old_pci, const std::string& short_mac_i)
{
  ul_ccch_msg_s msg;

  rrc_reest_request_s& req = msg.msg.set_c1().set_rrc_reest_request();

  req.rrc_reest_request.ue_id.c_rnti = (uint32_t)old_crnti;
  req.rrc_reest_request.ue_id.pci    = (uint16_t)old_pci;
  req.rrc_reest_request.ue_id.short_mac_i.from_string(short_mac_i);
  req.rrc_reest_request.reest_cause.value = reest_cause_opts::other_fail;

  return msg;
}

ul_ccch_msg_s ocudu::test_helpers::create_rrc_resume_request(uint64_t           resume_id,
                                                             const std::string& resume_mac_i,
                                                             resume_cause_e     resume_cause)
{
  ul_ccch_msg_s msg;

  rrc_resume_request_s& req = msg.msg.set_c1().set_rrc_resume_request();

  req.rrc_resume_request.resume_id.from_number(resume_id);
  req.rrc_resume_request.resume_mac_i.from_string(resume_mac_i);
  req.rrc_resume_request.resume_cause = resume_cause;

  return msg;
}

ul_ccch1_msg_s ocudu::test_helpers::create_rrc_resume_request1(uint64_t           resume_id,
                                                               const std::string& resume_mac_i,
                                                               resume_cause_e     resume_cause)
{
  ul_ccch1_msg_s msg;

  rrc_resume_request1_s& req = msg.msg.set_c1().set_rrc_resume_request1();

  req.rrc_resume_request1.resume_id.from_number(resume_id);
  req.rrc_resume_request1.resume_mac_i.from_string(resume_mac_i);
  req.rrc_resume_request1.resume_cause = resume_cause;

  return msg;
}

ul_dcch_msg_s ocudu::test_helpers::create_rrc_resume_complete(uint8_t transaction_id, uint8_t sel_plmn_id)
{
  ul_dcch_msg_s msg;

  rrc_resume_complete_s& res = msg.msg.set_c1().set_rrc_resume_complete();
  res.rrc_transaction_id     = transaction_id;

  rrc_resume_complete_ies_s& res_cmplt = res.crit_exts.set_rrc_resume_complete();
  res_cmplt.sel_plmn_id_present        = true;
  res_cmplt.sel_plmn_id                = sel_plmn_id;

  return msg;
}

ul_dcch_msg_s ocudu::test_helpers::create_rrc_setup_complete(uint8_t sel_plmn_id, std::optional<uint64_t> ng_5g_s_tmsi)
{
  ul_dcch_msg_s msg;

  rrc_setup_complete_s& req = msg.msg.set_c1().set_rrc_setup_complete();

  req.rrc_transaction_id        = 0;
  rrc_setup_complete_ies_s& ies = req.crit_exts.set_rrc_setup_complete();

  ies.sel_plmn_id = sel_plmn_id;
  ies.ded_nas_msg.from_string("0123456789abcdef"); // Dummy NAS message.
  if (ng_5g_s_tmsi.has_value()) {
    ies.ng_5_g_s_tmsi_value_present = true;
    ies.ng_5_g_s_tmsi_value.set_ng_5_g_s_tmsi().from_number(ng_5g_s_tmsi.value());
  }

  return msg;
}

ul_dcch_msg_s ocudu::test_helpers::create_ue_capability_info(uint8_t transaction_id, bool rrc_inactive_supported)
{
  ul_dcch_msg_s msg;

  ue_cap_info_s& req = msg.msg.set_c1().set_ue_cap_info();

  req.rrc_transaction_id = transaction_id;
  ue_cap_info_ies_s& ies = req.crit_exts.set_ue_cap_info();

  ies.ue_cap_rat_container_list_present = true;
  ies.ue_cap_rat_container_list.resize(1);
  ue_cap_rat_container_s& rat_container = ies.ue_cap_rat_container_list[0];
  rat_container.rat_type                = rat_type_opts::options::nr;

  // Fill UE NR capabilities.
  ue_nr_cap_s nr_cap;

  // Fill access stratum release.
  nr_cap.access_stratum_release = access_stratum_release_opts::options::rel15;

  // Fill PDCP parameters.
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0000 = true;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0001 = true;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0002 = true;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0003 = false;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0004 = true;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0006 = false;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0101 = false;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0102 = false;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0103 = false;
  nr_cap.pdcp_params.supported_rohc_profiles.profile0x0104 = false;
  nr_cap.pdcp_params.max_num_rohc_context_sessions = pdcp_params_s::max_num_rohc_context_sessions_opts::options::cs16;
  nr_cap.pdcp_params.out_of_order_delivery_present = true;
  nr_cap.pdcp_params.short_sn_present              = true;

  // Fill RLC parameters.
  nr_cap.rlc_params_present                  = true;
  nr_cap.rlc_params.am_with_short_sn_present = true;
  nr_cap.rlc_params.um_with_short_sn_present = true;
  nr_cap.rlc_params.um_with_long_sn_present  = true;

  // Fill MAC parameters.
  nr_cap.mac_params_present                                     = true;
  nr_cap.mac_params.mac_params_xdd_diff_present                 = true;
  nr_cap.mac_params.mac_params_xdd_diff.long_drx_cycle_present  = true;
  nr_cap.mac_params.mac_params_xdd_diff.short_drx_cycle_present = true;

  // Fill PHY parameters.
  // > Fill PHY params common.
  nr_cap.phy_params.phy_params_common_present                                     = true;
  nr_cap.phy_params.phy_params_common.dyn_harq_ack_codebook_present               = true;
  nr_cap.phy_params.phy_params_common.dyn_beta_offset_ind_harq_ack_csi_present    = true;
  nr_cap.phy_params.phy_params_common.ra_type0_pusch_present                      = true;
  nr_cap.phy_params.phy_params_common.dyn_switch_ra_type0_1_pdsch_present         = true;
  nr_cap.phy_params.phy_params_common.dyn_switch_ra_type0_1_pusch_present         = true;
  nr_cap.phy_params.phy_params_common.pdsch_map_type_a_present                    = true;
  nr_cap.phy_params.phy_params_common.pdsch_map_type_b_present                    = true;
  nr_cap.phy_params.phy_params_common.interleaving_vrb_to_prb_pdsch_present       = true;
  nr_cap.phy_params.phy_params_common.pusch_repeat_multi_slots_present            = true;
  nr_cap.phy_params.phy_params_common.pdsch_repeat_multi_slots_present            = true;
  nr_cap.phy_params.phy_params_common.cfg_ul_grant_type1_present                  = true;
  nr_cap.phy_params.phy_params_common.cfg_ul_grant_type2_present                  = true;
  nr_cap.phy_params.phy_params_common.rate_matching_resrc_set_semi_static_present = true;
  nr_cap.phy_params.phy_params_common.rate_matching_resrc_set_dyn_present         = true;
  nr_cap.phy_params.phy_params_common.bwp_switching_delay_present                 = true;
  nr_cap.phy_params.phy_params_common.bwp_switching_delay =
      phy_params_common_s::bwp_switching_delay_opts::options::type1;
  nr_cap.phy_params.phy_params_common.rate_matching_ctrl_resrc_set_dyn_present = true;
  nr_cap.phy_params.phy_params_common.max_layers_mimo_ind_present              = true;
  // > Fill PHY params frx diff.
  nr_cap.phy_params.phy_params_frx_diff_present             = true;
  nr_cap.phy_params.phy_params_frx_diff.two_fl_dmrs_present = true;
  nr_cap.phy_params.phy_params_frx_diff.two_fl_dmrs.from_number(3);
  nr_cap.phy_params.phy_params_frx_diff.supported_dmrs_type_dl_present = true;
  nr_cap.phy_params.phy_params_frx_diff.supported_dmrs_type_dl =
      phy_params_frx_diff_s::supported_dmrs_type_dl_opts::options::type1_and2;
  nr_cap.phy_params.phy_params_frx_diff.supported_dmrs_type_ul_present = true;
  nr_cap.phy_params.phy_params_frx_diff.supported_dmrs_type_ul =
      phy_params_frx_diff_s::supported_dmrs_type_ul_opts::options::type1_and2;
  nr_cap.phy_params.phy_params_frx_diff.one_ports_ptrs_present = true;
  nr_cap.phy_params.phy_params_frx_diff.one_ports_ptrs.from_number(3);
  nr_cap.phy_params.phy_params_frx_diff.pucch_f2_with_fh_present        = true;
  nr_cap.phy_params.phy_params_frx_diff.pucch_f3_with_fh_present        = true;
  nr_cap.phy_params.phy_params_frx_diff.pucch_f4_with_fh_present        = true;
  nr_cap.phy_params.phy_params_frx_diff.pusch_half_pi_bpsk_present      = true;
  nr_cap.phy_params.phy_params_frx_diff.pucch_f3_4_half_pi_bpsk_present = true;
  nr_cap.phy_params.phy_params_frx_diff.mux_sr_harq_ack_csi_pucch_once_per_slot.set_present();
  nr_cap.phy_params.phy_params_frx_diff.mux_sr_harq_ack_csi_pucch_once_per_slot->same_symbol_present = true;
  nr_cap.phy_params.phy_params_frx_diff.mux_sr_harq_ack_pucch_present                                = true;
  nr_cap.phy_params.phy_params_frx_diff.dl_sched_offset_pdsch_type_a_present                         = true;
  nr_cap.phy_params.phy_params_frx_diff.dl_sched_offset_pdsch_type_b_present                         = true;
  nr_cap.phy_params.phy_params_frx_diff.ul_sched_offset_present                                      = true;
  nr_cap.phy_params.phy_params_frx_diff.dl_64_qam_mcs_table_alt_present                              = true;
  nr_cap.phy_params.phy_params_frx_diff.ul_64_qam_mcs_table_alt_present                              = true;
  nr_cap.phy_params.phy_params_frx_diff.cqi_table_alt_present                                        = true;
  nr_cap.phy_params.phy_params_frx_diff.one_fl_dmrs_two_add_dmrs_ul_present                          = true;
  nr_cap.phy_params.phy_params_frx_diff.two_fl_dmrs_two_add_dmrs_ul_present                          = true;
  nr_cap.phy_params.phy_params_frx_diff.one_fl_dmrs_three_add_dmrs_ul_present                        = true;
  // > Fill PHY params fr1.
  nr_cap.phy_params.phy_params_fr1_present                   = true;
  nr_cap.phy_params.phy_params_fr1.pdsch_256_qam_fr1_present = true;

  // Fill RF parameters.
  // > Fill supported band list NR.
  nr_cap.rf_params.supported_band_list_nr.resize(1);
  nr_cap.rf_params.supported_band_list_nr[0].band_nr                      = 78;
  nr_cap.rf_params.supported_band_list_nr[0].bwp_without_restrict_present = true;
  nr_cap.rf_params.supported_band_list_nr[0].bwp_same_numerology_present  = true;
  nr_cap.rf_params.supported_band_list_nr[0].bwp_same_numerology = band_nr_s::bwp_same_numerology_opts::options::upto4;
  nr_cap.rf_params.supported_band_list_nr[0].pusch_256_qam_present         = true;
  nr_cap.rf_params.supported_band_list_nr[0].rate_matching_lte_crs_present = true;
  nr_cap.rf_params.supported_band_list_nr[0].mimo_params_per_band_present  = true;
  nr_cap.rf_params.supported_band_list_nr[0].mimo_params_per_band.codebook_params.set_present();
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.codebook_params->type1.single_panel.supported_csi_rs_res_list.resize(1);
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.codebook_params->type1.single_panel.supported_csi_rs_res_list[0]
      .max_num_tx_ports_per_res = supported_csi_rs_res_s::max_num_tx_ports_per_res_opts::options::p32;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.codebook_params->type1.single_panel.supported_csi_rs_res_list[0]
      .max_num_res_per_band = 1;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.codebook_params->type1.single_panel.supported_csi_rs_res_list[0]
      .total_num_tx_ports_per_band = 32;
  nr_cap.rf_params.supported_band_list_nr[0].mimo_params_per_band.codebook_params->type1.single_panel.modes =
      codebook_params_s::type1_s_::single_panel_s_::modes_opts::options::mode1and_mode2;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.codebook_params->type1.single_panel.max_num_csi_rs_per_res_set = 1;
  nr_cap.rf_params.supported_band_list_nr[0].mimo_params_per_band.csi_rs_im_reception_for_feedback.set_present();
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_rs_im_reception_for_feedback->max_cfg_num_nzp_csi_rs_per_cc = 8;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_rs_im_reception_for_feedback->max_cfg_num_ports_across_nzp_csi_rs_per_cc = 32;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_rs_im_reception_for_feedback->max_cfg_num_csi_im_per_cc =
      csi_rs_im_reception_for_feedback_s::max_cfg_num_csi_im_per_cc_opts::options::n4;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_rs_im_reception_for_feedback->max_num_simul_nzp_csi_rs_per_cc = 8;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_rs_im_reception_for_feedback->total_num_ports_simul_nzp_csi_rs_per_cc = 64;
  nr_cap.rf_params.supported_band_list_nr[0].mimo_params_per_band.csi_report_framework.set_present();
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_report_framework->max_num_periodic_csi_per_bwp_for_csi_report = 1;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_report_framework->max_num_aperiodic_csi_per_bwp_for_csi_report = 1;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_report_framework->max_num_semi_persistent_csi_per_bwp_for_csi_report = 0;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_report_framework->max_num_periodic_csi_per_bwp_for_beam_report = 1;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_report_framework->max_num_aperiodic_csi_per_bwp_for_beam_report = 1;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_report_framework->max_num_aperiodic_csi_trigger_state_per_cc =
      csi_report_framework_s::max_num_aperiodic_csi_trigger_state_per_cc_opts::options::n3;
  nr_cap.rf_params.supported_band_list_nr[0]
      .mimo_params_per_band.csi_report_framework->max_num_semi_persistent_csi_per_bwp_for_beam_report            = 0;
  nr_cap.rf_params.supported_band_list_nr[0].mimo_params_per_band.csi_report_framework->simul_csi_reports_per_cc = 5;
  // > Fill supported band combination list.
  nr_cap.rf_params.supported_band_combination_list.resize(2);
  nr_cap.rf_params.supported_band_combination_list[0].feature_set_combination = 0;
  nr_cap.rf_params.supported_band_combination_list[0].band_list.resize(2);
  auto& nr_band_0_0                     = nr_cap.rf_params.supported_band_combination_list[0].band_list[0].set_nr();
  nr_band_0_0.band_nr                   = 78;
  nr_band_0_0.ca_bw_class_dl_nr_present = true;
  nr_band_0_0.ca_bw_class_dl_nr         = ca_bw_class_nr_opts::options::a;
  nr_band_0_0.ca_bw_class_ul_nr_present = true;
  nr_band_0_0.ca_bw_class_ul_nr         = ca_bw_class_nr_opts::options::a;
  auto& nr_band_0_1                     = nr_cap.rf_params.supported_band_combination_list[0].band_list[1].set_nr();
  nr_band_0_1.band_nr                   = 78;
  nr_band_0_1.ca_bw_class_dl_nr_present = true;
  nr_band_0_1.ca_bw_class_dl_nr         = ca_bw_class_nr_opts::options::a;
  nr_band_0_1.ca_bw_class_ul_nr_present = true;
  nr_band_0_1.ca_bw_class_ul_nr         = ca_bw_class_nr_opts::options::a;
  nr_cap.rf_params.supported_band_combination_list[1].feature_set_combination = 1;
  nr_cap.rf_params.supported_band_combination_list[1].band_list.resize(1);
  auto& nr_band_1_0                     = nr_cap.rf_params.supported_band_combination_list[1].band_list[0].set_nr();
  nr_band_1_0.band_nr                   = 78;
  nr_band_1_0.ca_bw_class_dl_nr_present = true;
  nr_band_1_0.ca_bw_class_dl_nr         = ca_bw_class_nr_opts::options::b;
  nr_band_1_0.ca_bw_class_ul_nr_present = true;
  nr_band_1_0.ca_bw_class_ul_nr         = ca_bw_class_nr_opts::options::b;
  // > Fill applied frequency band list filter.
  nr_cap.rf_params.applied_freq_band_list_filt.resize(1);
  nr_cap.rf_params.applied_freq_band_list_filt[0].set_band_info_nr().band_nr = 78;
  // > Fill supported band combination list v1540.
  band_combination_v1540_s band_combination_v1540_0;
  band_combination_v1540_0.band_list_v1540.resize(2);
  band_combination_v1540_0.band_list_v1540[0].srs_tx_switch_present = true;
  band_combination_v1540_0.band_list_v1540[0].srs_tx_switch.supported_srs_tx_port_switch =
      band_params_v1540_s::srs_tx_switch_s_::supported_srs_tx_port_switch_opts::options::t1r1;
  band_combination_v1540_0.band_list_v1540[1].srs_tx_switch_present = true;
  band_combination_v1540_0.band_list_v1540[1].srs_tx_switch.supported_srs_tx_port_switch =
      band_params_v1540_s::srs_tx_switch_s_::supported_srs_tx_port_switch_opts::options::t1r1;
  nr_cap.rf_params.supported_band_combination_list_v1540.set_present();
  nr_cap.rf_params.supported_band_combination_list_v1540->push_back(band_combination_v1540_0);
  band_combination_v1540_s band_combination_v1540_1;
  band_combination_v1540_1.band_list_v1540.resize(1);
  band_combination_v1540_1.band_list_v1540[0].srs_tx_switch_present = true;
  band_combination_v1540_1.band_list_v1540[0].srs_tx_switch.supported_srs_tx_port_switch =
      band_params_v1540_s::srs_tx_switch_s_::supported_srs_tx_port_switch_opts::options::t1r1;
  nr_cap.rf_params.supported_band_combination_list_v1540->push_back(band_combination_v1540_1);

  // Fill meas and mob parameters.
  nr_cap.meas_and_mob_params_present = true;
  nr_cap.meas_and_mob_params.meas_and_mob_params_common.supported_gap_pattern.from_number(4194303);
  nr_cap.meas_and_mob_params.meas_and_mob_params_common.nr_cgi_report_present                       = true;
  nr_cap.meas_and_mob_params.meas_and_mob_params_frx_diff_present                                   = true;
  nr_cap.meas_and_mob_params.meas_and_mob_params_frx_diff.ss_sinr_meas_present                      = true;
  nr_cap.meas_and_mob_params.meas_and_mob_params_frx_diff.simul_rx_data_ssb_diff_numerology_present = true;

  // Fill feature sets.
  nr_cap.feature_sets_present = true;
  nr_cap.feature_sets.feature_sets_dl.resize(2);
  nr_cap.feature_sets.feature_sets_dl[0].feature_set_list_per_dl_cc.push_back(1);
  nr_cap.feature_sets.feature_sets_dl[1].feature_set_list_per_dl_cc.push_back(1);
  nr_cap.feature_sets.feature_sets_dl[1].feature_set_list_per_dl_cc.push_back(1);
  nr_cap.feature_sets.feature_sets_dl_per_cc.resize(1);
  nr_cap.feature_sets.feature_sets_dl_per_cc[0].supported_subcarrier_spacing_dl = subcarrier_spacing_opts::khz30;
  nr_cap.feature_sets.feature_sets_dl_per_cc[0].supported_mod_order_dl_present  = true;
  nr_cap.feature_sets.feature_sets_dl_per_cc[0].supported_mod_order_dl          = mod_order_opts::options::qam64;
  nr_cap.feature_sets.feature_sets_dl_per_cc[0].supported_bw_dl.set_fr1() = supported_bw_c::fr1_opts::options::mhz20;
  nr_cap.feature_sets.feature_sets_ul.resize(2);
  nr_cap.feature_sets.feature_sets_ul[0].feature_set_list_per_ul_cc.push_back(1);
  nr_cap.feature_sets.feature_sets_ul[0].supported_srs_res_features_present = true;
  nr_cap.feature_sets.feature_sets_ul[0].supported_srs_res_features.max_num_aperiodic_srs_per_bwp =
      srs_resources_s::max_num_aperiodic_srs_per_bwp_opts::options::n16;
  nr_cap.feature_sets.feature_sets_ul[0].supported_srs_res_features.max_num_aperiodic_srs_per_bwp_per_slot = 6;
  nr_cap.feature_sets.feature_sets_ul[0].supported_srs_res_features.max_num_periodic_srs_per_bwp =
      srs_resources_s::max_num_periodic_srs_per_bwp_opts::options::n16;
  nr_cap.feature_sets.feature_sets_ul[0].supported_srs_res_features.max_num_periodic_srs_per_bwp_per_slot = 6;
  nr_cap.feature_sets.feature_sets_ul[0].supported_srs_res_features.max_num_semi_persistent_srs_per_bwp =
      srs_resources_s::max_num_semi_persistent_srs_per_bwp_opts::options::n16;
  nr_cap.feature_sets.feature_sets_ul[0].supported_srs_res_features.max_num_semi_persistent_srs_per_bwp_per_slot = 6;
  nr_cap.feature_sets.feature_sets_ul[0].supported_srs_res_features.max_num_srs_ports_per_res =
      srs_resources_s::max_num_srs_ports_per_res_opts::options::n1;
  nr_cap.feature_sets.feature_sets_ul[1].feature_set_list_per_ul_cc.push_back(1);
  nr_cap.feature_sets.feature_sets_ul[1].feature_set_list_per_ul_cc.push_back(1);
  nr_cap.feature_sets.feature_sets_ul[1].supported_srs_res_features_present = true;
  nr_cap.feature_sets.feature_sets_ul[1].supported_srs_res_features.max_num_aperiodic_srs_per_bwp =
      srs_resources_s::max_num_aperiodic_srs_per_bwp_opts::options::n16;
  nr_cap.feature_sets.feature_sets_ul[1].supported_srs_res_features.max_num_aperiodic_srs_per_bwp_per_slot = 6;
  nr_cap.feature_sets.feature_sets_ul[1].supported_srs_res_features.max_num_periodic_srs_per_bwp =
      srs_resources_s::max_num_periodic_srs_per_bwp_opts::options::n16;
  nr_cap.feature_sets.feature_sets_ul[1].supported_srs_res_features.max_num_periodic_srs_per_bwp_per_slot = 6;
  nr_cap.feature_sets.feature_sets_ul[1].supported_srs_res_features.max_num_semi_persistent_srs_per_bwp =
      srs_resources_s::max_num_semi_persistent_srs_per_bwp_opts::options::n16;
  nr_cap.feature_sets.feature_sets_ul[1].supported_srs_res_features.max_num_semi_persistent_srs_per_bwp_per_slot = 6;
  nr_cap.feature_sets.feature_sets_ul[1].supported_srs_res_features.max_num_srs_ports_per_res =
      srs_resources_s::max_num_srs_ports_per_res_opts::options::n1;
  nr_cap.feature_sets.feature_sets_ul_per_cc.resize(1);
  nr_cap.feature_sets.feature_sets_ul_per_cc[0].supported_subcarrier_spacing_ul = subcarrier_spacing_opts::khz30;
  nr_cap.feature_sets.feature_sets_ul_per_cc[0].supported_bw_ul.set_fr1() = supported_bw_c::fr1_opts::options::mhz20;
  nr_cap.feature_sets.feature_sets_ul_per_cc[0].mimo_cb_pusch_present     = true;
  nr_cap.feature_sets.feature_sets_ul_per_cc[0].mimo_cb_pusch.max_num_mimo_layers_cb_pusch_present = true;
  nr_cap.feature_sets.feature_sets_ul_per_cc[0].mimo_cb_pusch.max_num_mimo_layers_cb_pusch =
      mimo_layers_ul_opts::options::one_layer;
  nr_cap.feature_sets.feature_sets_ul_per_cc[0].mimo_cb_pusch.max_num_srs_res_per_set = 1;
  nr_cap.feature_sets.feature_sets_ul_per_cc[0].supported_mod_order_ul_present        = true;
  nr_cap.feature_sets.feature_sets_ul_per_cc[0].supported_mod_order_ul                = mod_order_opts::options::qam64;
  feature_set_dl_v1540_s feature_set_dl_v1540_0;
  feature_set_dl_v1540_0.one_fl_dmrs_two_add_dmrs_dl_present   = true;
  feature_set_dl_v1540_0.add_dmrs_dl_alt_present               = true;
  feature_set_dl_v1540_0.two_fl_dmrs_two_add_dmrs_dl_present   = true;
  feature_set_dl_v1540_0.one_fl_dmrs_three_add_dmrs_dl_present = true;
  nr_cap.feature_sets.feature_sets_dl_v1540.set_present();
  nr_cap.feature_sets.feature_sets_dl_v1540->push_back(feature_set_dl_v1540_0);
  feature_set_dl_v1540_s feature_set_dl_v1540_1;
  feature_set_dl_v1540_1.one_fl_dmrs_two_add_dmrs_dl_present   = true;
  feature_set_dl_v1540_1.add_dmrs_dl_alt_present               = true;
  feature_set_dl_v1540_1.two_fl_dmrs_two_add_dmrs_dl_present   = true;
  feature_set_dl_v1540_1.one_fl_dmrs_three_add_dmrs_dl_present = true;
  nr_cap.feature_sets.feature_sets_dl_v1540->push_back(feature_set_dl_v1540_1);

  // Fill feature set combinations.
  nr_cap.feature_set_combinations.resize(2);
  nr_cap.feature_set_combinations[0].resize(2);
  nr_cap.feature_set_combinations[0][0].resize(1);
  auto& feature_set_nr_0_0_0     = nr_cap.feature_set_combinations[0][0][0].set_nr();
  feature_set_nr_0_0_0.dl_set_nr = 1;
  feature_set_nr_0_0_0.ul_set_nr = 1;
  nr_cap.feature_set_combinations[0][1].resize(1);
  auto& feature_set_nr_0_1_0     = nr_cap.feature_set_combinations[0][1][0].set_nr();
  feature_set_nr_0_1_0.dl_set_nr = 1;
  feature_set_nr_0_1_0.ul_set_nr = 1;
  nr_cap.feature_set_combinations[1].resize(1);
  nr_cap.feature_set_combinations[1][0].resize(1);
  auto& feature_set_nr_1_0_0     = nr_cap.feature_set_combinations[1][0][0].set_nr();
  feature_set_nr_1_0_0.dl_set_nr = 2;
  feature_set_nr_1_0_0.ul_set_nr = 2;

  // Fill non critical extension.
  nr_cap.non_crit_ext_present                = true;
  nr_cap.non_crit_ext.inactive_state_present = rrc_inactive_supported;
  // > Fill non critical extension.
  nr_cap.non_crit_ext.non_crit_ext_present                                              = true;
  nr_cap.non_crit_ext.non_crit_ext.ims_params_present                                   = true;
  nr_cap.non_crit_ext.non_crit_ext.ims_params.ims_params_frx_diff_present               = true;
  nr_cap.non_crit_ext.non_crit_ext.ims_params.ims_params_frx_diff.voice_over_nr_present = true;

  rat_container.ue_cap_rat_container = ocucp::pack_into_pdu(nr_cap, "UE NR Capability");

  return msg;
}

asn1::rrc_nr::ul_dcch_msg_s ocudu::test_helpers::create_security_mode_complete(uint8_t transaction_id)
{
  ul_dcch_msg_s ul_dcch_msg;

  security_mode_complete_s& smc_complete = ul_dcch_msg.msg.set_c1().set_security_mode_complete();
  smc_complete.rrc_transaction_id        = transaction_id;
  smc_complete.crit_exts.set_security_mode_complete();

  return ul_dcch_msg;
}

ul_dcch_msg_s ocudu::test_helpers::create_rrc_reestablishment_complete()
{
  ul_dcch_msg_s msg;

  rrc_reest_complete_s& req = msg.msg.set_c1().set_rrc_reest_complete();
  req.rrc_transaction_id    = 0;
  req.crit_exts.set_rrc_reest_complete();

  return msg;
}

asn1::rrc_nr::ul_dcch_msg_s ocudu::test_helpers::create_rrc_reconfiguration_complete(uint8_t transaction_id)
{
  ul_dcch_msg_s msg;

  rrc_recfg_complete_s& req = msg.msg.set_c1().set_rrc_recfg_complete();
  req.rrc_transaction_id    = transaction_id;
  req.crit_exts.set_rrc_recfg_complete();

  return msg;
}

location_meas_info_c ocudu::test_helpers::create_nr_prs_location_meas_info()
{
  nr_prs_meas_info_r16_s prs_info;
  prs_info.dl_prs_point_a_r16                               = 643296;
  prs_info.nr_meas_prs_repeat_and_offset_r16.set_ms40_r16() = 7;
  prs_info.nr_meas_prs_len_r16.value                        = nr_prs_meas_info_r16_s::nr_meas_prs_len_r16_opts::ms6;

  location_meas_info_c info;
  info.set_nr_prs_meas_r16().push_back(prs_info);
  return info;
}

ul_dcch_msg_s ocudu::test_helpers::create_location_meas_ind(const std::optional<location_meas_info_c>& info)
{
  ul_dcch_msg_s msg;

  location_meas_ind_ies_s& ies = msg.msg.set_c1().set_location_meas_ind().crit_exts.set_location_meas_ind();
  if (info.has_value()) {
    ies.meas_ind.set_setup() = info.value();
  } else {
    ies.meas_ind.set_release();
  }

  return msg;
}

byte_buffer ocudu::test_helpers::pack_ul_ccch_msg(const ul_ccch_msg_s& msg)
{
  byte_buffer   pdu;
  asn1::bit_ref bref{pdu};
  if (msg.pack(bref) == asn1::OCUDUASN_SUCCESS) {
    return pdu;
  }
  return byte_buffer{};
}

byte_buffer ocudu::test_helpers::pack_ul_ccch1_msg(const ul_ccch1_msg_s& msg)
{
  byte_buffer   pdu;
  asn1::bit_ref bref{pdu};
  if (msg.pack(bref) == asn1::OCUDUASN_SUCCESS) {
    return pdu;
  }
  return byte_buffer{};
}

byte_buffer ocudu::test_helpers::pack_ul_dcch_msg(const ul_dcch_msg_s& msg)
{
  byte_buffer   pdu;
  asn1::bit_ref bref{pdu};
  if (msg.pack(bref) == asn1::OCUDUASN_SUCCESS) {
    return pdu;
  }
  return byte_buffer{};
}

asn1::rrc_nr::sib1_s ocudu::test_helpers::create_sib1(const plmn_identity& plmn, span<const tac_t> tac_list)
{
  asn1::rrc_nr::sib1_s sib1;

  // Fill cell selection info.
  sib1.cell_sel_info_present            = true;
  sib1.cell_sel_info.q_rx_lev_min       = -70;
  sib1.cell_sel_info.q_qual_min_present = true;
  sib1.cell_sel_info.q_qual_min         = -20;

  // Fill cell access related info.
  sib1.cell_access_related_info.plmn_id_info_list.resize(1);
  plmn_id_info_s& plmn_info = sib1.cell_access_related_info.plmn_id_info_list[0];
  plmn_info.plmn_id_list.resize(1);

  // Fill plmn id.
  plmn_info.plmn_id_list[0] = ocucp::plmn_to_asn1(plmn);

  // Fill ta and cell id.
  if (tac_list.empty()) {
    plmn_info.tac_present = true;
    plmn_info.tac.from_number(7);
  } else {
    // TS 38.331: trackingAreaList replaces trackingAreaCode; it sits in an extension group.
    plmn_info.tracking_area_list_r17.set_present();
    for (tac_t tac : tac_list) {
      asn1::fixed_bitstring<24> asn1_tac;
      asn1_tac.from_number(tac);
      plmn_info.tracking_area_list_r17->push_back(asn1_tac);
    }
  }
  plmn_info.cell_id.from_number(0x66c0000);
  plmn_info.cell_reserved_for_oper = asn1::rrc_nr::plmn_id_info_s::cell_reserved_for_oper_opts::options::not_reserved;

  // Fill connection establishment failure control.
  sib1.conn_est_fail_ctrl_present = true;
  sib1.conn_est_fail_ctrl.conn_est_fail_count =
      asn1::rrc_nr::conn_est_fail_ctrl_s::conn_est_fail_count_opts::options::n1;
  sib1.conn_est_fail_ctrl.conn_est_fail_offset_validity =
      asn1::rrc_nr::conn_est_fail_ctrl_s::conn_est_fail_offset_validity_opts::options::s30;
  sib1.conn_est_fail_ctrl.conn_est_fail_offset_present = true;
  sib1.conn_est_fail_ctrl.conn_est_fail_offset         = 1;

  // Fill serving cell config common.
  sib1.serving_cell_cfg_common_present = true;
  // > Fill DL config common.
  // >> Fill frequency info DL.
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.freq_band_list.resize(1);
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.freq_band_list[0].freq_band_ind_nr_present = true;
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.freq_band_list[0].freq_band_ind_nr         = 78;
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.offset_to_point_a                          = 0;
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.scs_specific_carrier_list.resize(1);
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.scs_specific_carrier_list[0].offset_to_carrier = 0;
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.scs_specific_carrier_list[0].subcarrier_spacing =
      asn1::rrc_nr::subcarrier_spacing_opts::options::khz30;
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.scs_specific_carrier_list[0].carrier_bw = 51;
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.scs_specific_carrier_list[0]
      .tx_direct_current_location_present = true;
  sib1.serving_cell_cfg_common.dl_cfg_common.freq_info_dl.scs_specific_carrier_list[0].tx_direct_current_location = 306;
  // >> Fill initial DL BWP.
  // >>> Fill generic parameters.
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.generic_params.location_and_bw = 13750;
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.generic_params.subcarrier_spacing =
      asn1::rrc_nr::subcarrier_spacing_opts::options::khz30;
  // >>> Fill PDCCH config common.
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common_present = true;
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.set_setup();
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().common_search_space_list.resize(1);
  // >>>> Fill common search space.
  search_space_s& css =
      sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().common_search_space_list[0];
  css.search_space_id                                = 1;
  css.coreset_id_present                             = true;
  css.coreset_id                                     = 0;
  css.monitoring_slot_periodicity_and_offset_present = true;
  css.monitoring_slot_periodicity_and_offset.set_sl1();
  css.monitoring_symbols_within_slot.from_number(8192);
  css.nrof_candidates_present = true;
  css.nrof_candidates.aggregation_level1 =
      asn1::rrc_nr::search_space_s::nrof_candidates_s_::aggregation_level1_opts::options::n0;
  css.nrof_candidates.aggregation_level2 =
      asn1::rrc_nr::search_space_s::nrof_candidates_s_::aggregation_level2_opts::options::n0;
  css.nrof_candidates.aggregation_level4 =
      asn1::rrc_nr::search_space_s::nrof_candidates_s_::aggregation_level4_opts::options::n1;
  css.nrof_candidates.aggregation_level8 =
      asn1::rrc_nr::search_space_s::nrof_candidates_s_::aggregation_level8_opts::options::n0;
  css.nrof_candidates.aggregation_level16 =
      asn1::rrc_nr::search_space_s::nrof_candidates_s_::aggregation_level16_opts::options::n0;
  css.search_space_type_present = true;
  css.search_space_type.set_common();
  css.search_space_type.common().dci_format0_0_and_format1_0_present                                          = true;
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().search_space_sib1_present   = true;
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().search_space_sib1           = 0;
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().paging_search_space_present = true;
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().paging_search_space         = 1;
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().ra_search_space_present     = true;
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().ra_search_space             = 1;
  // >>> Fill PDSCH config common.
  sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdsch_cfg_common_present = true;
  auto& pdsch_cfg = sib1.serving_cell_cfg_common.dl_cfg_common.init_dl_bwp.pdsch_cfg_common.set_setup();
  pdsch_cfg.pdsch_time_domain_alloc_list.resize(2);
  pdsch_cfg.pdsch_time_domain_alloc_list[0].map_type =
      asn1::rrc_nr::pdsch_time_domain_res_alloc_s::map_type_opts::options::type_a;
  pdsch_cfg.pdsch_time_domain_alloc_list[0].start_symbol_and_len = 53;
  pdsch_cfg.pdsch_time_domain_alloc_list[1].map_type =
      asn1::rrc_nr::pdsch_time_domain_res_alloc_s::map_type_opts::options::type_a;
  pdsch_cfg.pdsch_time_domain_alloc_list[1].start_symbol_and_len = 72;
  // >> Fill BCCH config.
  sib1.serving_cell_cfg_common.dl_cfg_common.bcch_cfg.mod_period_coeff =
      asn1::rrc_nr::bcch_cfg_s::mod_period_coeff_opts::options::n4;
  // >> Fill PCCH config.
  sib1.serving_cell_cfg_common.dl_cfg_common.pcch_cfg.default_paging_cycle =
      asn1::rrc_nr::paging_cycle_opts::options::rf128;
  sib1.serving_cell_cfg_common.dl_cfg_common.pcch_cfg.nand_paging_frame_offset.set_one_t();
  sib1.serving_cell_cfg_common.dl_cfg_common.pcch_cfg.ns = asn1::rrc_nr::pcch_cfg_s::ns_opts::options::one;
  // > Fill UL config common.
  sib1.serving_cell_cfg_common.ul_cfg_common_present = true;
  // >> Fill frequency info UL.
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.freq_band_list.resize(1);
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.freq_band_list[0].freq_band_ind_nr_present = true;
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.freq_band_list[0].freq_band_ind_nr         = 78;
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.absolute_freq_point_a_present              = true;
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.absolute_freq_point_a                      = 620688;
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.scs_specific_carrier_list.resize(1);
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.scs_specific_carrier_list[0].offset_to_carrier = 0;
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.scs_specific_carrier_list[0].subcarrier_spacing =
      asn1::rrc_nr::subcarrier_spacing_opts::options::khz30;
  sib1.serving_cell_cfg_common.ul_cfg_common.freq_info_ul.scs_specific_carrier_list[0].carrier_bw = 51;
  // >> Fill initial UL BWP.
  // >>> Fill generic parameters.
  sib1.serving_cell_cfg_common.ul_cfg_common.init_ul_bwp.generic_params.location_and_bw = 13750;
  sib1.serving_cell_cfg_common.ul_cfg_common.init_ul_bwp.generic_params.subcarrier_spacing =
      asn1::rrc_nr::subcarrier_spacing_opts::options::khz30;
  // >>> Fill RACH config common.
  sib1.serving_cell_cfg_common.ul_cfg_common.init_ul_bwp.rach_cfg_common_present = true;
  auto& rach_cfg = sib1.serving_cell_cfg_common.ul_cfg_common.init_ul_bwp.rach_cfg_common.set_setup();
  rach_cfg.rach_cfg_generic.prach_cfg_idx             = 159;
  rach_cfg.rach_cfg_generic.msg1_fdm                  = asn1::rrc_nr::rach_cfg_generic_s::msg1_fdm_opts::options::one;
  rach_cfg.rach_cfg_generic.msg1_freq_start           = 11;
  rach_cfg.rach_cfg_generic.zero_correlation_zone_cfg = 0;
  rach_cfg.rach_cfg_generic.preamb_rx_target_pwr      = -100;
  rach_cfg.rach_cfg_generic.preamb_trans_max = asn1::rrc_nr::rach_cfg_generic_s::preamb_trans_max_opts::options::n7;
  rach_cfg.rach_cfg_generic.pwr_ramp_step    = asn1::rrc_nr::rach_cfg_generic_s::pwr_ramp_step_opts::options::db4;
  rach_cfg.rach_cfg_generic.ra_resp_win      = asn1::rrc_nr::rach_cfg_generic_s::ra_resp_win_opts::options::sl20;
  rach_cfg.ssb_per_rach_occasion_and_cb_preambs_per_ssb_present = true;
  rach_cfg.ssb_per_rach_occasion_and_cb_preambs_per_ssb.set_one() =
      asn1::rrc_nr::rach_cfg_common_s::ssb_per_rach_occasion_and_cb_preambs_per_ssb_c_::one_opts::options::n64;
  rach_cfg.ra_contention_resolution_timer =
      asn1::rrc_nr::rach_cfg_common_s::ra_contention_resolution_timer_opts::options::sf64;
  rach_cfg.prach_root_seq_idx.set_l139()   = 1;
  rach_cfg.msg1_subcarrier_spacing_present = true;
  rach_cfg.msg1_subcarrier_spacing         = asn1::rrc_nr::subcarrier_spacing_opts::options::khz30;
  rach_cfg.restricted_set_cfg = asn1::rrc_nr::rach_cfg_common_s::restricted_set_cfg_opts::options::unrestricted_set;
  // >>> Fill PUSCH config common.
  sib1.serving_cell_cfg_common.ul_cfg_common.init_ul_bwp.pusch_cfg_common_present = true;
  auto& pusch_cfg = sib1.serving_cell_cfg_common.ul_cfg_common.init_ul_bwp.pusch_cfg_common.set_setup();
  pusch_cfg.pusch_time_domain_alloc_list.resize(5);
  pusch_cfg.pusch_time_domain_alloc_list[0].k2 = 4;
  pusch_cfg.pusch_time_domain_alloc_list[0].map_type =
      asn1::rrc_nr::pusch_time_domain_res_alloc_s::map_type_opts::options::type_a;
  pusch_cfg.pusch_time_domain_alloc_list[0].start_symbol_and_len = 27;
  pusch_cfg.pusch_time_domain_alloc_list[1].k2                   = 5;
  pusch_cfg.pusch_time_domain_alloc_list[1].map_type =
      asn1::rrc_nr::pusch_time_domain_res_alloc_s::map_type_opts::options::type_a;
  pusch_cfg.pusch_time_domain_alloc_list[1].start_symbol_and_len = 27;
  pusch_cfg.pusch_time_domain_alloc_list[2].k2                   = 6;
  pusch_cfg.pusch_time_domain_alloc_list[2].map_type =
      asn1::rrc_nr::pusch_time_domain_res_alloc_s::map_type_opts::options::type_a;
  pusch_cfg.pusch_time_domain_alloc_list[2].start_symbol_and_len = 27;
  pusch_cfg.pusch_time_domain_alloc_list[3].k2                   = 7;
  pusch_cfg.pusch_time_domain_alloc_list[3].map_type =
      asn1::rrc_nr::pusch_time_domain_res_alloc_s::map_type_opts::options::type_a;
  pusch_cfg.pusch_time_domain_alloc_list[3].start_symbol_and_len = 27;
  pusch_cfg.pusch_time_domain_alloc_list[4].k2                   = 11;
  pusch_cfg.pusch_time_domain_alloc_list[4].map_type =
      asn1::rrc_nr::pusch_time_domain_res_alloc_s::map_type_opts::options::type_a;
  pusch_cfg.pusch_time_domain_alloc_list[4].start_symbol_and_len = 27;
  pusch_cfg.msg3_delta_preamb_present                            = true;
  pusch_cfg.msg3_delta_preamb                                    = 6;
  pusch_cfg.p0_nominal_with_grant_present                        = true;
  pusch_cfg.p0_nominal_with_grant                                = -76;
  // >>> Fill PUCCH config common.
  sib1.serving_cell_cfg_common.ul_cfg_common.init_ul_bwp.pucch_cfg_common_present = true;
  auto& pucch_cfg = sib1.serving_cell_cfg_common.ul_cfg_common.init_ul_bwp.pucch_cfg_common.set_setup();
  pucch_cfg.pucch_res_common_present = true;
  pucch_cfg.pucch_res_common         = 11;
  pucch_cfg.pucch_group_hop          = asn1::rrc_nr::pucch_cfg_common_s::pucch_group_hop_opts::options::neither;
  pucch_cfg.p0_nominal_present       = true;
  pucch_cfg.p0_nominal               = -90;
  // >> Fill time alignment timer common.
  sib1.serving_cell_cfg_common.ul_cfg_common.time_align_timer_common =
      asn1::rrc_nr::time_align_timer_opts::options::infinity;
  // > Fill n-Timing advance offset.
  sib1.serving_cell_cfg_common.n_timing_advance_offset_present = true;
  sib1.serving_cell_cfg_common.n_timing_advance_offset =
      asn1::rrc_nr::serving_cell_cfg_common_sib_s::n_timing_advance_offset_opts::options::n25600;
  // > Fill SSB position in burst.
  sib1.serving_cell_cfg_common.ssb_positions_in_burst.in_one_group.from_number(128);
  // > Fill SSB periodicity serving cell.
  sib1.serving_cell_cfg_common.ssb_periodicity_serving_cell =
      asn1::rrc_nr::serving_cell_cfg_common_sib_s::ssb_periodicity_serving_cell_opts::options::ms10;
  // > Fill TDD UL DL configuration common.
  sib1.serving_cell_cfg_common.tdd_ul_dl_cfg_common_present = true;
  sib1.serving_cell_cfg_common.tdd_ul_dl_cfg_common.ref_subcarrier_spacing =
      asn1::rrc_nr::subcarrier_spacing_opts::options::khz30;
  sib1.serving_cell_cfg_common.tdd_ul_dl_cfg_common.pattern1.dl_ul_tx_periodicity =
      asn1::rrc_nr::tdd_ul_dl_pattern_s::dl_ul_tx_periodicity_opts::options::ms5;
  sib1.serving_cell_cfg_common.tdd_ul_dl_cfg_common.pattern1.nrof_dl_slots   = 6;
  sib1.serving_cell_cfg_common.tdd_ul_dl_cfg_common.pattern1.nrof_dl_symbols = 8;
  sib1.serving_cell_cfg_common.tdd_ul_dl_cfg_common.pattern1.nrof_ul_slots   = 3;
  sib1.serving_cell_cfg_common.tdd_ul_dl_cfg_common.pattern1.nrof_ul_symbols = 0;
  // > Fill ss PBCH block power.
  sib1.serving_cell_cfg_common.ss_pbch_block_pwr = -16;

  // Fill ue timers and constants.
  sib1.ue_timers_and_consts_present = true;
  sib1.ue_timers_and_consts.t300    = asn1::rrc_nr::ue_timers_and_consts_s::t300_opts::options::ms1000;
  sib1.ue_timers_and_consts.t301    = asn1::rrc_nr::ue_timers_and_consts_s::t301_opts::options::ms1000;
  sib1.ue_timers_and_consts.t310    = asn1::rrc_nr::ue_timers_and_consts_s::t310_opts::options::ms1000;
  sib1.ue_timers_and_consts.n310    = asn1::rrc_nr::ue_timers_and_consts_s::n310_opts::options::n1;
  sib1.ue_timers_and_consts.t311    = asn1::rrc_nr::ue_timers_and_consts_s::t311_opts::options::ms3000;
  sib1.ue_timers_and_consts.n311    = asn1::rrc_nr::ue_timers_and_consts_s::n311_opts::options::n1;
  sib1.ue_timers_and_consts.t319    = asn1::rrc_nr::ue_timers_and_consts_s::t319_opts::options::ms1000;

  return sib1;
}
