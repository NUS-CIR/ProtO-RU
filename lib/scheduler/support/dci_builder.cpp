// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "dci_builder.h"
#include "../cell/cell_harq_manager.h"
#include "../config/time_domain_mapper.h"
#include "ocudu/ran/pdcch/dci_packing.h"
#include "ocudu/ran/pdcch/search_space.h"
#include "ocudu/ran/pdsch/pdsch_antenna_ports_mapping.h"
#include "ocudu/ran/pusch/pusch_antenna_ports_mapping.h"
#include "ocudu/scheduler/config/bwp_configuration.h"
#include "ocudu/scheduler/support/rb_helper.h"
#include <algorithm>
#include <type_traits>
#include <variant>

using namespace ocudu;

// dci_dl_info::type() and dci_ul_info::type() convert the payload variant index into the DCI type. Each enumerator must
// therefore index the payload alternative it names, and each payload alternative must have an enumerator.
namespace {

template <dci_dl_rnti_config_type Type, typename Payload>
constexpr bool dl_dci_type_matches_payload =
    std::is_same_v<std::variant_alternative_t<static_cast<std::size_t>(Type), dci_dl_info::payload_type>, Payload>;

template <dci_ul_rnti_config_type Type, typename Payload>
constexpr bool ul_dci_type_matches_payload =
    std::is_same_v<std::variant_alternative_t<static_cast<std::size_t>(Type), dci_ul_info::payload_type>, Payload>;

} // namespace

static_assert(dl_dci_type_matches_payload<dci_dl_rnti_config_type::si_f1_0, dci_1_0_si_rnti_configuration>);
static_assert(dl_dci_type_matches_payload<dci_dl_rnti_config_type::ra_f1_0, dci_1_0_ra_rnti_configuration>);
static_assert(dl_dci_type_matches_payload<dci_dl_rnti_config_type::c_rnti_f1_0, dci_1_0_c_rnti_configuration>);
static_assert(dl_dci_type_matches_payload<dci_dl_rnti_config_type::tc_rnti_f1_0, dci_1_0_tc_rnti_configuration>);
static_assert(dl_dci_type_matches_payload<dci_dl_rnti_config_type::p_rnti_f1_0, dci_1_0_p_rnti_configuration>);
static_assert(dl_dci_type_matches_payload<dci_dl_rnti_config_type::c_rnti_f1_1, dci_1_1_configuration>);
static_assert(std::variant_size_v<dci_dl_info::payload_type> ==
                  static_cast<std::size_t>(dci_dl_rnti_config_type::c_rnti_f1_1) + 1,
              "dci_dl_info::payload_type has alternatives with no matching dci_dl_rnti_config_type");

static_assert(ul_dci_type_matches_payload<dci_ul_rnti_config_type::tc_rnti_f0_0, dci_0_0_tc_rnti_configuration>);
static_assert(ul_dci_type_matches_payload<dci_ul_rnti_config_type::c_rnti_f0_0, dci_0_0_c_rnti_configuration>);
static_assert(ul_dci_type_matches_payload<dci_ul_rnti_config_type::c_rnti_f0_1, dci_0_1_configuration>);
static_assert(ul_dci_type_matches_payload<dci_ul_rnti_config_type::cs_rnti_f0_1, dci_0_1_cs_rnti_configuration>);
static_assert(std::variant_size_v<dci_ul_info::payload_type> ==
                  static_cast<std::size_t>(dci_ul_rnti_config_type::cs_rnti_f0_1) + 1,
              "dci_ul_info::payload_type has alternatives with no matching dci_ul_rnti_config_type");

/// Get PDSCH-to-HARQ-timing-indicator field as per TS38.213, 9.2.3.
static unsigned get_dci_1_0_pdsch_to_harq_timing_indicator(unsigned k1)
{
  // PDSCH-to-HARQ-timing-indicator maps to {1,2,3,4,5,6,7,8} for DCI 1_0.
  ocudu_assert(k1 <= 8, "Invalid k1 value");
  return k1 - 1;
}

/// Get PDSCH-to-HARQ-timing-indicator field as per TS38.213, 9.2.3.
static unsigned get_dci_1_1_pdsch_to_harq_timing_indicator(unsigned k1, span<const uint8_t> dl_data_to_ul_ack)
{
  // PDSCH-to-HARQ-timing-indicator maps to dl-DataToUL-ACK for DCI 1_1.
  const auto* it = std::find_if(
      dl_data_to_ul_ack.begin(), dl_data_to_ul_ack.end(), [k1](const auto& k1_cfg) { return k1 == k1_cfg; });
  ocudu_assert(it != dl_data_to_ul_ack.end(), "Invalid k1 value");
  return it - dl_data_to_ul_ack.begin();
}

void ocudu::build_dci_f1_0_si_rnti(dci_dl_info&               dci,
                                   const bwp_downlink_common& init_dl_bwp,
                                   crb_interval               crbs,
                                   unsigned                   time_resource,
                                   sch_mcs_index              mcs_index,
                                   uint8_t                    si_indicator)
{
  dci_1_0_si_rnti_configuration& si_dci = dci.set_si_rnti_f1_0();
  // as per TS38.212, clause 7.3.1.2.1 - N^{DL,BWP}_RB.
  si_dci.N_rb_dl_bwp = init_dl_bwp.pdcch_common.coreset0->coreset0_crbs().length();
  const vrb_interval si_vrbs =
      crb_to_vrb_f1_0_common_ss_non_interleaved(crbs, init_dl_bwp.pdcch_common.coreset0->get_coreset_start_crb());
  si_dci.frequency_resource = ra_frequency_type1_get_riv(
      ra_frequency_type1_configuration{si_dci.N_rb_dl_bwp, si_vrbs.start(), si_vrbs.length()});
  // As per Table 7.3.1.2.2-5, TS 38.212, 0 = non-interleaved, 1 = interleaved.
  // TODO: Verify if interleaved is suitable for SIB1.
  si_dci.interleaved_vrb_prb_mapping = false;
  si_dci.time_resource               = time_resource;
  si_dci.modulation_coding_scheme    = mcs_index.value();
  // Redundancy version for first transmission.
  // TODO: Check what is the best RV for SIB1.
  si_dci.redundancy_version           = 0;
  si_dci.system_information_indicator = si_indicator;
}

void ocudu::build_dci_f1_0_p_rnti(dci_dl_info&               dci,
                                  const bwp_downlink_common& init_dl_bwp,
                                  crb_interval               crbs,
                                  unsigned                   time_resource,
                                  sch_mcs_index              mcs_index)
{
  auto& p_dci = dci.set_p_rnti_f1_0();
  // See 38.212, clause 7.3.1.2.1 - N^{DL,BWP}_RB for P-RNTI.
  p_dci.N_rb_dl_bwp = init_dl_bwp.pdcch_common.coreset0->coreset0_crbs().length();
  const vrb_interval paging_vrbs =
      crb_to_vrb_f1_0_common_ss_non_interleaved(crbs, init_dl_bwp.pdcch_common.coreset0->get_coreset_start_crb());
  p_dci.frequency_resource = ra_frequency_type1_get_riv(
      ra_frequency_type1_configuration{p_dci.N_rb_dl_bwp, paging_vrbs.start(), paging_vrbs.length()});
  p_dci.time_resource = time_resource;
  // As per Table 7.3.1.2.2-5, TS 38.212, 0 = non-interleaved, 1 = interleaved.
  // TODO: Verify if interleaved is suitable for Paging.
  p_dci.interleaved_vrb_prb_mapping = false;
  p_dci.short_messages_indicator    = dci_1_0_p_rnti_configuration::payload_info::scheduling_information;
  p_dci.modulation_coding_scheme    = mcs_index.value();
}

void ocudu::build_dci_f1_0_p_rnti(dci_dl_info& dci, const bwp_downlink_common& init_dl_bwp, unsigned short_messages)
{
  auto& p_dci = dci.set_p_rnti_f1_0();
  // See 38.212, clause 7.3.1.2.1 - N^{DL,BWP}_RB for P-RNTI.
  p_dci.N_rb_dl_bwp              = init_dl_bwp.pdcch_common.coreset0->coreset0_crbs().length();
  p_dci.short_messages_indicator = dci_1_0_p_rnti_configuration::payload_info::short_messages;
  p_dci.short_messages           = short_messages;
}

/// Builds DCI f1_0 shared by RA-RNTI and MsgB-RNTI, as per TS38.212, clause 7.3.1.2.1.
static void build_dci_f1_0_ra_or_msgb_rnti(dci_dl_info&               dci,
                                           const bwp_downlink_common& init_dl_bwp,
                                           crb_interval               crbs,
                                           unsigned                   time_resource,
                                           sch_mcs_index              mcs_index,
                                           unsigned                   lsb_sfn)
{
  dci_1_0_ra_rnti_configuration& ra_dci = dci.set_ra_rnti_f1_0();
  // as per TS38.212, clause 7.3.1.2.1 - N^{DL,BWP}_RB.
  ra_dci.N_rb_dl_bwp = init_dl_bwp.pdcch_common.coreset0.has_value()
                           ? init_dl_bwp.pdcch_common.coreset0->coreset0_crbs().length()
                           : init_dl_bwp.generic_params.crbs.length();
  const vrb_interval rar_vrbs =
      crb_to_vrb_f1_0_common_ss_non_interleaved(crbs, init_dl_bwp.pdcch_common.coreset0->get_coreset_start_crb());
  ra_dci.frequency_resource = ra_frequency_type1_get_riv(
      ra_frequency_type1_configuration{ra_dci.N_rb_dl_bwp, rar_vrbs.start(), rar_vrbs.length()});
  ra_dci.interleaved_vrb_prb_mapping = false;
  ra_dci.time_resource               = time_resource;
  ra_dci.modulation_coding_scheme    = mcs_index.value();
  ra_dci.tb_scaling                  = 0; // TODO.
  ra_dci.lsb_sfn                     = lsb_sfn;
}

void ocudu::build_dci_f1_0_ra_rnti(dci_dl_info&               dci,
                                   const bwp_downlink_common& init_dl_bwp,
                                   crb_interval               crbs,
                                   unsigned                   time_resource,
                                   sch_mcs_index              mcs_index)
{
  build_dci_f1_0_ra_or_msgb_rnti(dci, init_dl_bwp, crbs, time_resource, mcs_index, 0);
}

void ocudu::build_dci_f1_0_msgb_rnti(dci_dl_info&               dci,
                                     const bwp_downlink_common& init_dl_bwp,
                                     crb_interval               crbs,
                                     unsigned                   time_resource,
                                     sch_mcs_index              mcs_index,
                                     unsigned                   msgb_resp_window_ms,
                                     unsigned                   prach_sfn)
{
  // TS38.213, Section 8.2A: the LSBs of SFN field is only present if msgB-ResponseWindow is configured larger than
  // 10 msec; otherwise these bits are reserved.
  const unsigned lsb_sfn = msgb_resp_window_ms > 10 ? prach_sfn & 0b11U : 0U;
  build_dci_f1_0_ra_or_msgb_rnti(dci, init_dl_bwp, crbs, time_resource, mcs_index, lsb_sfn);
}

void ocudu::build_dci_f1_0_tc_rnti(dci_dl_info&                  dci,
                                   const bwp_downlink_common&    init_dl_bwp,
                                   vrb_interval                  vrbs,
                                   unsigned                      time_resource,
                                   unsigned                      k1,
                                   unsigned                      pucch_res_indicator,
                                   sch_mcs_index                 mcs_index,
                                   uint8_t                       rv,
                                   const dl_harq_process_handle& h_dl)
{
  dci_1_0_tc_rnti_configuration& f1_0 = dci.set_tc_rnti_f1_0();

  f1_0.tpc_command                 = 1;
  f1_0.interleaved_vrb_prb_mapping = false;

  // PDSCH resources.
  // See 38.212, clause 7.3.1.2.1 - N^{DL,BWP}_RB is the size of CORESET 0 for TC-RNTI.
  const crb_interval cs0_crbs = init_dl_bwp.pdcch_common.coreset0->coreset0_crbs();
  f1_0.N_rb_dl_bwp            = cs0_crbs.length();
  // See TS38.211 7.3.1.6 - Mapping from VRBs to PRBs.
  f1_0.frequency_resource =
      ra_frequency_type1_get_riv(ra_frequency_type1_configuration{f1_0.N_rb_dl_bwp, vrbs.start(), vrbs.length()});
  f1_0.time_resource = time_resource;

  // UCI resources.
  f1_0.pucch_resource_indicator       = pucch_res_indicator;
  f1_0.pdsch_harq_fb_timing_indicator = get_dci_1_0_pdsch_to_harq_timing_indicator(k1);

  f1_0.modulation_coding_scheme = mcs_index.value();

  // HARQ params.
  f1_0.harq_process_number = h_dl.id();
  f1_0.new_data_indicator  = h_dl.ndi();
  f1_0.redundancy_version  = rv;
}

void ocudu::build_dci_f1_0_c_rnti(dci_dl_info&                  dci,
                                  const search_space_info&      ss_info,
                                  const bwp_downlink_common&    init_dl_bwp,
                                  vrb_interval                  vrbs,
                                  unsigned                      time_resource,
                                  unsigned                      k1,
                                  unsigned                      pucch_res_indicator,
                                  unsigned                      dai,
                                  sch_mcs_index                 mcs_index,
                                  uint8_t                       rv,
                                  const dl_harq_process_handle& h_dl)
{
  const bwp_configuration& active_dl_bwp = ss_info.bwp->dl.cfg();

  dci_1_0_c_rnti_configuration& f1_0 = dci.set_c_rnti_f1_0();

  f1_0.tpc_command                 = 1;
  f1_0.interleaved_vrb_prb_mapping = false;

  // PDSCH resources.
  // See 38.212, clause 7.3.1.2.1 - N^{DL,BWP}_RB for C-RNTI.
  unsigned N_rb_dl_bwp = active_dl_bwp.crbs.length();
  if (ss_info.cfg->is_common_search_space() and init_dl_bwp.pdcch_common.coreset0.has_value()) {
    N_rb_dl_bwp = init_dl_bwp.pdcch_common.coreset0->coreset0_crbs().length();
  }
  f1_0.frequency_resource =
      ra_frequency_type1_get_riv(ra_frequency_type1_configuration{N_rb_dl_bwp, vrbs.start(), vrbs.length()});
  f1_0.time_resource = time_resource;

  const dci_sizes& dci_sz = ss_info.dci_sz;

  // Compute DCI size.
  f1_0.payload_size =
      not ss_info.cfg->is_common_search_space() ? dci_sz.format1_0_ue_size.value() : dci_sz.format1_0_common_size;

  // UCI resources.
  f1_0.pucch_resource_indicator       = pucch_res_indicator;
  f1_0.pdsch_harq_fb_timing_indicator = get_dci_1_0_pdsch_to_harq_timing_indicator(k1);
  f1_0.dl_assignment_index            = dai;

  f1_0.modulation_coding_scheme = mcs_index.value();

  // HARQ params.
  f1_0.harq_process_number = h_dl.id();
  f1_0.new_data_indicator  = h_dl.ndi();
  f1_0.redundancy_version  = rv;
}

void ocudu::build_dci_f1_1_c_rnti(dci_dl_info&                  dci,
                                  const ue_cell_configuration&  ue_cell_cfg,
                                  search_space_id               ss_id,
                                  const vrb_alloc&              vrbs,
                                  unsigned                      time_resource,
                                  unsigned                      k1,
                                  unsigned                      pucch_res_indicator,
                                  unsigned                      dai,
                                  sch_mcs_index                 tb1_mcs_index,
                                  uint8_t                       rv,
                                  const dl_harq_process_handle& h_dl,
                                  unsigned                      nof_layers,
                                  uint8_t                       tpc,
                                  bool                          enable_interleaving,
                                  unsigned                      srs_request)
{
  const search_space_info& ss_info = ue_cell_cfg.search_space(ss_id);
  ocudu_assert(not ss_info.cfg->is_common_search_space(), "SearchSpace must be of type UE-Specific SearchSpace");
  ocudu_sanity_check(ss_info.get_dl_dci_format() == dci_dl_format::f1_1, "Invalid DCI format");

  // Two codewords require maxLength=2 for nof_layers > 4 per 3GPP TS 38.212 Table 7.3.1.2.2; the two-codeword table
  // does not exist otherwise.
  const auto&           dmrs_cfg             = ss_info.bwp->dl.pdsch().ded()->pdsch_mapping_type_a_dmrs.value();
  const bool            has_len2             = dmrs_cfg.is_max_length_len2;
  const bool            are_both_cws_enabled = (nof_layers > 4) && has_len2;
  const dmrs_max_length max_len              = has_len2 ? dmrs_max_length::len2 : dmrs_max_length::len1;

  const bwp_configuration& active_dl_bwp = ss_info.bwp->dl.cfg();
  const auto               k1_candidates = ss_info.bwp->ul.td_mapper().dedicated_k1_candidates();

  dci_1_1_configuration& f1_1 = dci.set_c_rnti_f1_1();

  f1_1.tpc_command             = static_cast<unsigned>(tpc);
  f1_1.srs_request             = srs_request;
  f1_1.dmrs_seq_initialization = 0;

  f1_1.antenna_ports = get_pdsch_antenna_port_mapping_row_index(nof_layers,
                                                                ue_cell_cfg.cell_cfg_common.params.dl_carrier.nof_ant,
                                                                dmrs_cfg.is_dmrs_type2 ? dmrs_config_type::type2
                                                                                       : dmrs_config_type::type1,
                                                                max_len,
                                                                are_both_cws_enabled);

  const dci_size_config& dci_sz_cfg = ss_info.dci_sz_cfg;
  const dci_sizes&       dci_sz     = ss_info.dci_sz;

  if (vrbs.is_type0()) {
    ocudu_assert(dci_sz_cfg.pdsch_res_allocation_type != resource_allocation::resource_allocation_type_1,
                 "Resource allocation type 0 is not configured");
    ocudu_assert(not enable_interleaving, "Resource allocation type 0 requires non-interleaved VRB-to-PRB mapping");
    // See 38.214, clause 5.1.2.2.1 - The RBG bitmap maps RBG 0 to the MSB.
    const nominal_rbg_size P = get_nominal_rbg_size(active_dl_bwp.crbs.length(), ss_info.bwp->dl.pdsch().ded()->rbg_sz);
    f1_1.frequency_resource  = convert_vrbs_to_rbgs(vrbs.type0(), active_dl_bwp.crbs, P).to_uint64();
  } else {
    ocudu_assert(dci_sz_cfg.pdsch_res_allocation_type != resource_allocation::resource_allocation_type_0,
                 "Resource allocation type 1 is not configured");
    // See 38.212, clause 7.3.1.2.2 - N^{DL,BWP}_RB for C-RNTI.
    f1_1.frequency_resource = ra_frequency_type1_get_riv(
        ra_frequency_type1_configuration{active_dl_bwp.crbs.length(), vrbs.type1().start(), vrbs.type1().length()});
  }
  if (dci_sz_cfg.pdsch_res_allocation_type == resource_allocation::dynamic_switch) {
    f1_1.dynamic_pdsch_res_allocation_type =
        vrbs.is_type0() ? dynamic_resource_allocation::type_0 : dynamic_resource_allocation::type_1;
  }
  f1_1.time_resource = time_resource;

  // Compute DCI size.
  f1_1.payload_size = dci_sz.format1_1_ue_size.value();

  if (dci_sz_cfg.interleaved_vrb_prb_mapping.has_value() and dci_sz_cfg.interleaved_vrb_prb_mapping.value()) {
    f1_1.vrb_prb_mapping = enable_interleaving;
  }

  // TODO:
  //  1. When Dynamic PRB bundling is used, set prb_bundling_size_indicator.
  //  2. When Maximum number of code-block-groups (CBGs) is configured, set cbg_transmission_info.
  //  3. When DMRS configuration is set in PDSCH-Config, set dmrs_seq_initialization based on whether scramblingID0 or
  //  scramblingID1 to be used.

  // UCI resources.
  f1_1.pucch_resource_indicator       = pucch_res_indicator;
  f1_1.pdsch_harq_fb_timing_indicator = get_dci_1_1_pdsch_to_harq_timing_indicator(k1, k1_candidates);
  f1_1.downlink_assignment_index      = dai;

  f1_1.tb1_modulation_coding_scheme = tb1_mcs_index.value();

  // HARQ params.
  f1_1.harq_process_number    = h_dl.id();
  f1_1.tb1_new_data_indicator = h_dl.ndi();
  f1_1.tb1_redundancy_version = rv;
}

void ocudu::build_dci_f0_0_tc_rnti(dci_ul_info&               dci,
                                   const bwp_downlink_common& init_dl_bwp,
                                   const bwp_configuration&   ul_bwp,
                                   const crb_interval&        crbs,
                                   unsigned                   time_resource,
                                   sch_mcs_index              mcs_index,
                                   uint8_t                    rv)
{
  dci_0_0_tc_rnti_configuration& f0_0 = dci.set_tc_rnti_f0_0();

  // TODO.
  f0_0.N_ul_hop               = 0;
  f0_0.hopping_offset         = 0;
  f0_0.frequency_hopping_flag = 0;
  f0_0.tpc_command            = 1;

  // PUSCH params.
  const auto dci_sz_cfg = dci_size_config{
      false,
      false,
      init_dl_bwp.generic_params.crbs.length(),
      init_dl_bwp.generic_params.crbs.length(),
      ul_bwp.crbs.length(),
      ul_bwp.crbs.length(),
      init_dl_bwp.pdcch_common.coreset0.has_value() ? init_dl_bwp.pdcch_common.coreset0->coreset0_crbs().length() : 0,
      false};
  ocudu_assert(validate_dci_size_config(dci_sz_cfg), "Invalid DCI size configuration for DCI Format 0_0 (TC-RNTI)");
  const dci_sizes dci_sz  = get_dci_sizes(dci_sz_cfg);
  f0_0.payload_size       = dci_sz.format0_0_common_size;
  const vrb_interval vrbs = rb_helper::crb_to_vrb_ul_non_interleaved(crbs, ul_bwp.crbs.start());
  f0_0.frequency_resource =
      ra_frequency_type1_get_riv(ra_frequency_type1_configuration{ul_bwp.crbs.length(), vrbs.start(), vrbs.length()});
  f0_0.time_resource = time_resource;

  f0_0.modulation_coding_scheme = mcs_index.value();

  // HARQ params.
  f0_0.redundancy_version = rv;
}

void ocudu::build_dci_f0_0_c_rnti(dci_ul_info&                  dci,
                                  const search_space_info&      ss_info,
                                  const bwp_uplink_common&      init_ul_bwp,
                                  const vrb_interval&           vrbs,
                                  unsigned                      time_resource,
                                  sch_mcs_index                 mcs_index,
                                  uint8_t                       rv,
                                  const ul_harq_process_handle& h_ul,
                                  uint8_t                       tpc_command)
{
  const bwp_configuration& active_ul_bwp = ss_info.bwp->ul.cfg();

  dci_0_0_c_rnti_configuration& f0_0 = dci.set_c_rnti_f0_0();

  // TODO.
  f0_0.N_ul_hop               = 0;
  f0_0.hopping_offset         = 0;
  f0_0.frequency_hopping_flag = 0;
  f0_0.tpc_command            = tpc_command;
  f0_0.ul_sul_indicator       = {};

  // PDCCH params.
  const dci_sizes& dci_sz = ss_info.dci_sz;

  f0_0.payload_size =
      not ss_info.cfg->is_common_search_space() ? dci_sz.format0_0_ue_size.value() : dci_sz.format0_0_common_size;

  // PUSCH resources.
  // See 38.212, clause 7.3.1.1.1 - N^{UL,BWP}_RB for C-RNTI.
  const unsigned N_ul_bwp_rb =
      ss_info.cfg->is_common_search_space() ? init_ul_bwp.generic_params.crbs.length() : active_ul_bwp.crbs.length();
  f0_0.frequency_resource =
      ra_frequency_type1_get_riv(ra_frequency_type1_configuration{N_ul_bwp_rb, vrbs.start(), vrbs.length()});

  f0_0.time_resource = time_resource;

  f0_0.modulation_coding_scheme = mcs_index.value();

  // HARQ params.
  f0_0.harq_process_number = h_ul.id();
  f0_0.new_data_indicator  = h_ul.ndi();
  f0_0.redundancy_version  = rv;
}

void ocudu::build_dci_f0_1_c_rnti(dci_ul_info&                  dci,
                                  const ue_cell_configuration&  ue_cell_cfg,
                                  search_space_id               ss_id,
                                  const vrb_interval&           vrbs,
                                  unsigned                      time_resource,
                                  sch_mcs_index                 mcs_index,
                                  uint8_t                       rv,
                                  const ul_harq_process_handle& h_ul,
                                  unsigned                      dai,
                                  unsigned                      nof_layers,
                                  unsigned                      tpmi,
                                  uint8_t                       tpc_command,
                                  std::optional<bool>           csi_request,
                                  unsigned                      srs_request)
{
  const search_space_info& ss_info = ue_cell_cfg.search_space(ss_id);
  ocudu_assert(not ss_info.cfg->is_common_search_space(), "SearchSpace must be of type UE-Specific SearchSpace");

  const bwp_configuration& active_ul_bwp = ss_info.bwp->ul.cfg();

  dci_0_1_configuration& f0_1 = dci.set_c_rnti_f0_1();

  const dci_sizes& dci_sz = ss_info.dci_sz;

  f0_1.payload_size = dci_sz.format0_1_ue_size.value();

  f0_1.ul_sul_indicator        = {};
  f0_1.tpc_command             = tpc_command;
  f0_1.dmrs_seq_initialization = 0;
  bool use_transform_precoder  = ue_cell_cfg.use_pusch_transform_precoding_dci_0_1();
  f0_1.antenna_ports           = get_pusch_antenna_port_mapping_row_index(
      nof_layers, use_transform_precoder, dmrs_config_type::type1, dmrs_max_length::len1);
  f0_1.precoding_info_nof_layers = get_pusch_precoding_info_row_index(
      nof_layers,
      ue_cell_cfg.get_pusch_codebook_config().max_rank.value(),
      static_cast<srs_resource_configuration::one_two_four_enum>(ue_cell_cfg.get_srs_nof_ports()),
      tpmi);

  // See 38.212, clause 7.3.1.1.2 - N^{UL,BWP}_RB for C-RNTI.
  f0_1.frequency_resource = ra_frequency_type1_get_riv(
      ra_frequency_type1_configuration{active_ul_bwp.crbs.length(), vrbs.start(), vrbs.length()});
  f0_1.time_resource = time_resource;

  f0_1.modulation_coding_scheme = mcs_index.value();

  // HARQ params.
  f0_1.harq_process_number = h_ul.id();
  f0_1.new_data_indicator  = h_ul.ndi();
  f0_1.redundancy_version  = rv;

  // TODO: Set values for -
  //  - srs_resource_indicator
  //  - beta_offset_indicator
  //  - ptrs_dmrs_association
  //  - cbg_transmission_info

  // DAI is set based on TS 38.213, clause 9.1.3.2.
  f0_1.first_dl_assignment_index = dai;
  f0_1.ul_sch_indicator          = 1;
  if (csi_request.has_value()) {
    // CSI request value of 1 corresponds to the first entry in aperiodicTriggerStateList.
    f0_1.csi_request = *csi_request ? 1 : 0;
  }

  f0_1.srs_request = srs_request;
}

void ocudu::build_dci_f0_1_cg_activation(dci_ul_info&                 dci,
                                         const ue_cell_configuration& ue_cell_cfg,
                                         search_space_id              ss_id,
                                         const vrb_interval&          vrbs,
                                         unsigned                     time_resource,
                                         sch_mcs_index                mcs_index)
{
  const search_space_info& ss_info = ue_cell_cfg.search_space(ss_id);
  ocudu_assert(not ss_info.cfg->is_common_search_space(), "SearchSpace must be of type UE-Specific SearchSpace");

  const bwp_configuration& active_ul_bwp = ss_info.bwp->ul.cfg();

  // The CG Type 2 activation DCI has its CRC scrambled with the CS-RNTI, as per TS 38.213, Section 10.2.
  dci_0_1_cs_rnti_configuration& f0_1 = dci.set_cs_rnti_f0_1();

  const dci_sizes& dci_sz = ss_info.dci_sz;

  f0_1.payload_size = dci_sz.format0_1_ue_size.value();

  f0_1.ul_sul_indicator                = {};
  static constexpr uint8_t default_tpc = 1;
  f0_1.tpc_command                     = default_tpc;
  f0_1.dmrs_seq_initialization         = 0;
  // NOTE: unlike the C-RNTI builder, this does not consult use_pusch_transform_precoding_dci_0_1(): the whole CG path
  // assumes CP-OFDM, and build_cg_pusch_cfg_params() sizes the grant on the same assumption, so the antenna-port row
  // has to be picked from the same table. validate_cg_cfg() rejects a CG combined with transform precoding.
  bool use_transform_precoder = false;
  // Only 1 layer currently supported for Configured Grant.
  static constexpr uint8_t nof_layers = 1;
  f0_1.antenna_ports                  = get_pusch_antenna_port_mapping_row_index(
      nof_layers, use_transform_precoder, dmrs_config_type::type1, dmrs_max_length::len1);
  // TODO: adapt this to more than 1 antenna.
  constexpr unsigned default_tpmi = 0U;
  f0_1.precoding_info_nof_layers  = get_pusch_precoding_info_row_index(
      nof_layers,
      ue_cell_cfg.get_pusch_codebook_config().max_rank.value(),
      static_cast<srs_resource_configuration::one_two_four_enum>(ue_cell_cfg.get_srs_nof_ports()),
      default_tpmi);

  // See 38.212, clause 7.3.1.1.2 - N^{UL,BWP}_RB for C-RNTI.
  f0_1.frequency_resource = ra_frequency_type1_get_riv(
      ra_frequency_type1_configuration{active_ul_bwp.crbs.length(), vrbs.start(), vrbs.length()});
  f0_1.time_resource = time_resource;

  f0_1.modulation_coding_scheme = mcs_index.value();

  // Values set to 0U as per TS for CG type 2 activation.
  static constexpr uint8_t cg_activation_fields = 0U;
  f0_1.harq_process_number                      = cg_activation_fields;
  f0_1.new_data_indicator                       = false;
  f0_1.redundancy_version                       = cg_activation_fields;

  // TODO: Set values for -
  //  - srs_resource_indicator
  //  - beta_offset_indicator
  //  - ptrs_dmrs_association
  //  - cbg_transmission_info

  // The CG PUSCH carries UL-SCH, so this must be set. A value of 0 tells the UE not to transmit UL-SCH on the PUSCH
  // and, combined with an all-zero CSI request and SRS request, forms a field combination that a UE "is not expected to
  // receive" and may discard outright, as per TS 38.212, Section 7.3.1.1.2.
  f0_1.ul_sch_indicator = 1;

  // Not applicable for CG activation.
  f0_1.first_dl_assignment_index = 0;
  f0_1.srs_request               = 0U;

  // The activation must not trigger an aperiodic CSI report, so the CSI request field is all zeros. It still has to be
  // set whenever the field has a non-zero size, since dci_0_1_pack() skips the field altogether when the optional is
  // empty, and the payload would then fall short of payload_size.total.
  if (f0_1.payload_size.csi_request.value() > 0) {
    f0_1.csi_request = 0;
  }
}
