// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "sch_pdu_builder.h"
#include "dmrs_helpers.h"
#include "pdsch/pdsch_default_time_allocation.h"
#include "ocudu/ran/csi_report/csi_report_on_pucch_helpers.h"
#include "ocudu/ran/csi_report/csi_report_on_pusch_helpers.h"
#include "ocudu/ran/csi_report/csi_report_size.h"
#include "ocudu/scheduler/config/serving_cell_config.h"

using namespace ocudu;

// See TS 38.211, 7.3.1.1. - Scrambling.
static unsigned get_pdsch_n_id(pci_t pci, const sched_bwp_config& bwp, dci_dl_format dci_fmt, bool is_common_ss)
{
  if (bwp.dl.pdsch().ded() != nullptr and bwp.dl.pdsch().ded()->data_scrambling_id_pdsch.has_value() and
      (dci_fmt != dci_dl_format::f1_0 or not is_common_ss)) {
    return *bwp.dl.pdsch().ded()->data_scrambling_id_pdsch;
  }
  return pci;
}

/// Fills the power offsets of a PDSCH that is scheduled without UE-dedicated configuration.
static void populate_pdsch_pwr_offsets(pdsch_information& pdsch, const cell_configuration& cell_cfg)
{
  const auto& csi_cfg = cell_cfg.bwp_res[to_bwp_id(0)].dl().csi;
  if (csi_cfg.has_value()) {
    // [Implementation-defined] It is assumed that same powerControlOffset is configured in NZP-CSI-RS-Resource across
    // all resources. The powerControlOffsetSS is always 0 dB.
    pdsch.tx_pwr_info.pwr_ctrl_offset = csi_cfg->nzp_csi_rs_list.front().pwr_ctrl_offset;
  }
}

pusch_config_params ocudu::get_pusch_config_f0_0_tc_rnti(const cell_configuration&                    cell_cfg,
                                                         const pusch_time_domain_resource_allocation& pusch_td_cfg)
{
  constexpr pusch_mcs_table mcs_table  = pusch_mcs_table::qam64;
  constexpr unsigned        nof_layers = 1;
  // As per TS 38.214, Section 6.1.4.2, nof_oh_prb by \c xOverhead, defined in \c PUSCH-ServingCellConfig, TS 38.331; it
  // is in the dedicated resources, configured the after DCI Format 0-0 TC-RNTI is used. Hence, nof_oh_prb is here set
  // as 0.
  static constexpr unsigned nof_oh_prb = 0;
  // As per TS 38.214, Section 5.1.3.2 and 6.1.4.2, and TS 38.212, Section 7.3.1.1 and 7.3.1.2, TB scaling filed is only
  // used for DCI Format 1-0 (in the DL). Therefore, for the PUSCH this is set to 0.
  constexpr unsigned tb_scaling_field = 0;
  // Parameter \c tp-pi2BPSK enabled is not supported yet.
  constexpr bool tp_pi2bpsk_present = false;
  // As with 1 or 2 bits the corresponding beta-offset won't change, we can over-allocate the number of HARQ-ACK bits
  // to 2.
  constexpr unsigned nof_harq_ack_bits = 2;

  pusch_config_params pusch;

  pusch.dmrs = make_dmrs_info_common(pusch_td_cfg, cell_cfg.params.pci, cell_cfg.params.dmrs_typeA_pos);

  pusch.symbols = pusch_td_cfg.symbols;

  pusch.mcs_table              = mcs_table;
  pusch.nof_layers             = nof_layers;
  pusch.tp_pi2bpsk_present     = tp_pi2bpsk_present;
  pusch.use_transform_precoder = cell_cfg.use_msg3_transform_precoder();
  pusch.tb_scaling_field       = tb_scaling_field;

  pusch.nof_oh_prb        = nof_oh_prb;
  pusch.nof_harq_ack_bits = nof_harq_ack_bits;

  return pusch;
}

pusch_config_params ocudu::get_pusch_config_f0_0_c_rnti(const cell_configuration&                    cell_cfg,
                                                        const ue_cell_configuration*                 ue_cell_cfg,
                                                        const bwp_uplink_common&                     ul_bwp,
                                                        const pusch_time_domain_resource_allocation& pusch_td_cfg,
                                                        const unsigned                               nof_harq_ack_bits,
                                                        bool                                         is_csi_report_slot)
{
  const pusch_mcs_table mcs_table  = pusch_mcs_table::qam64;
  constexpr unsigned    nof_layers = 1;
  // As per TS 38.214, Section 5.1.3.2 and 6.1.4.2, and TS 38.212, Section 7.3.1.1 and 7.3.1.2, TB scaling filed is only
  // used for DCI Format 1-0 (in the DL). Therefore, for the PUSCH this is set to 0.
  constexpr unsigned tb_scaling_field = 0;
  // Parameter \c tp-pi2BPSK enabled is not supported yet.
  constexpr bool tp_pi2bpsk_present = false;

  pusch_config_params pusch;

  pusch.dmrs = make_dmrs_info_common(pusch_td_cfg, cell_cfg.params.pci, cell_cfg.params.dmrs_typeA_pos);

  pusch.symbols = pusch_td_cfg.symbols;

  pusch.mcs_table              = mcs_table;
  pusch.nof_layers             = nof_layers;
  pusch.tp_pi2bpsk_present     = tp_pi2bpsk_present;
  pusch.use_transform_precoder = cell_cfg.use_msg3_transform_precoder();
  pusch.tb_scaling_field       = tb_scaling_field;

  // According to TS 38.214, Section 6.1.4.2, nof_oh_prb is set equal to xOverhead, when set; else nof_oh_prb = 0.
  // NOTE: x_overhead::not_set is mapped to 0.
  pusch.nof_oh_prb = ue_cell_cfg != nullptr and ue_cell_cfg->pusch_serving_cell_cfg() != nullptr
                         ? static_cast<unsigned>(ue_cell_cfg->pusch_serving_cell_cfg()->x_ov_head)
                         : static_cast<unsigned>(x_overhead::not_set);

  // TODO: verify if this needs to be set depending on some configuration.
  pusch.nof_harq_ack_bits = nof_harq_ack_bits;
  if (is_csi_report_slot and ue_cell_cfg != nullptr) {
    const csi_report_configuration& csi_rep_cfg = ue_cell_cfg->csi_report();
    // NOTE: The CSI size depends on whether the CSI is configured on PUSCH or PUCCH, as per Section 5.2.3, TS 38.214:
    // "For both Type I and Type II reports configured for PUCCH but transmitted on PUSCH, the determination of the
    // payload for CSI part 1 and CSI part 2 follows that of PUCCH as described in clause 5.2.4."
    if (not ue_cell_cfg->init_bwp().ul.ue_cfg()->periodic_csi_report.has_value()) {
      csi_report_size csi_size     = get_csi_report_pusch_size(csi_rep_cfg);
      pusch.nof_csi_part1_bits     = csi_size.part1_size.value();
      pusch.max_nof_csi_part2_bits = csi_size.part2_max_size.value();
    } else {
      pusch.nof_csi_part1_bits = get_csi_report_pucch_size(csi_rep_cfg).part1_size.value();
    }
  }

  return pusch;
}

pusch_config_params ocudu::get_pusch_config_f0_1_c_rnti(const ue_cell_configuration&                 ue_cell_cfg,
                                                        const pusch_time_domain_resource_allocation& pusch_td_cfg,
                                                        unsigned                                     nof_layers,
                                                        const unsigned                               nof_harq_ack_bits,
                                                        bool                                         is_csi_report_slot)
{
  const pusch_mcs_table mcs_table = ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg->mcs_table;
  // As per TS 38.214, Section 5.1.3.2 and 6.1.4.2, and TS 38.212, Section 7.3.1.1 and 7.3.1.2, TB scaling filed is only
  // used for DCI Format 1-0 (in the DL). Therefore, for the PUSCH this is set to 0.
  constexpr unsigned tb_scaling_field = 0;
  // Parameter \c tp-pi2BPSK enabled is not supported yet.
  constexpr bool tp_pi2bpsk_present = false;
  // TODO: Update the value based on nof. CWs enabled.
  static constexpr bool are_both_cws_enabled = false;

  pusch_config_params pusch;

  // TODO: Consider DMRS configured in PUSCH-Config. Need helpers from Phy.
  ocudu_assert(ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg->pusch_mapping_type_a_dmrs.has_value(),
               "No DMRS configured in PUSCH configuration");
  pusch.dmrs = make_dmrs_info_dedicated(pusch_td_cfg,
                                        ue_cell_cfg.cell_cfg_common.params.pci,
                                        ue_cell_cfg.cell_cfg_common.params.dmrs_typeA_pos,
                                        ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg->pusch_mapping_type_a_dmrs.value(),
                                        nof_layers,
                                        ue_cell_cfg.cell_cfg_common.params.ul_carrier.nof_ant,
                                        are_both_cws_enabled);

  pusch.symbols = pusch_td_cfg.symbols;

  pusch.mcs_table              = mcs_table;
  pusch.nof_layers             = nof_layers;
  pusch.tp_pi2bpsk_present     = tp_pi2bpsk_present;
  pusch.use_transform_precoder = ue_cell_cfg.use_pusch_transform_precoding_dci_0_1();
  pusch.tb_scaling_field       = tb_scaling_field;

  // According to TS 38.214, Section 6.1.4.2, nof_oh_prb is set equal to xOverhead, when set; else nof_oh_prb = 0.
  // NOTE: x_overhead::not_set is mapped to 0.
  pusch.nof_oh_prb = ue_cell_cfg.pusch_serving_cell_cfg() != nullptr
                         ? static_cast<unsigned>(ue_cell_cfg.pusch_serving_cell_cfg()->x_ov_head)
                         : static_cast<unsigned>(x_overhead::not_set);

  // TODO: verify if this needs to be set depending on some configuration.
  pusch.nof_harq_ack_bits = nof_harq_ack_bits;
  pusch.aperiodic_csi =
      ue_cell_cfg.has_csi_cfg() and not ue_cell_cfg.init_bwp().ul.ue_cfg()->periodic_csi_report.has_value();
  if (is_csi_report_slot) {
    const csi_report_configuration& csi_rep_cfg = ue_cell_cfg.csi_report();
    // NOTE: The CSI size depends on whether the CSI is configured on PUSCH or PUCCH, as per Section 5.2.3, TS 38.214:
    // "For both Type I and Type II reports configured for PUCCH but transmitted on PUSCH, the determination of the
    // payload for CSI part 1 and CSI part 2 follows that of PUCCH as described in clause 5.2.4."
    if (not ue_cell_cfg.init_bwp().ul.ue_cfg()->periodic_csi_report.has_value()) {
      csi_report_size csi_size     = get_csi_report_pusch_size(csi_rep_cfg);
      pusch.nof_csi_part1_bits     = csi_size.part1_size.value();
      pusch.max_nof_csi_part2_bits = csi_size.part2_max_size.value();
    } else {
      pusch.nof_csi_part1_bits = get_csi_report_pucch_size(csi_rep_cfg).part1_size.value();
    }
  }

  return pusch;
}

void ocudu::build_pdsch_f1_0_si_rnti(pdsch_information&                   pdsch,
                                     const cell_configuration&            cell_cfg,
                                     units::bytes                         tbs_bytes,
                                     const dci_1_0_si_rnti_configuration& dci_cfg,
                                     const crb_interval&                  crbs,
                                     const ofdm_symbol_range&             symbols,
                                     const dmrs_information&              dmrs_info)
{
  const bwp_downlink_common& bwp_dl = cell_cfg.params.dl_cfg_common.init_dl_bwp;
  const vrb_interval         vrbs =
      crb_to_vrb_f1_0_common_ss_non_interleaved(crbs, bwp_dl.pdcch_common.coreset0->get_coreset_start_crb());

  pdsch.rnti        = rnti_t::SI_RNTI;
  pdsch.bwp_cfg     = &bwp_dl.generic_params;
  pdsch.coreset_cfg = &*bwp_dl.pdcch_common.coreset0;
  pdsch.symbols     = symbols;
  pdsch.rbs         = vrbs;
  // As per TS 38.211, Section 7.3.1.1, n_ID is set to Physical Cell ID for SIB1.
  pdsch.n_id       = cell_cfg.params.pci;
  pdsch.nof_layers = 1;

  pdsch.mcs_table = pdsch_mcs_table::qam64;

  pdsch_codeword& cw    = pdsch.codewords.emplace_back();
  cw.rv_index           = dci_cfg.redundancy_version;
  cw.mcs_index          = dci_cfg.modulation_coding_scheme;
  cw.mcs_descr          = pdsch_mcs_get_config(pdsch.mcs_table, cw.mcs_index);
  cw.tb_size_bytes      = tbs_bytes;
  pdsch.dmrs            = dmrs_info;
  pdsch.vrb_prb_mapping = vrb_to_prb::mapping_type::non_interleaved;
  pdsch.ss_set_type =
      dci_cfg.system_information_indicator == 0 ? search_space_set_type::type0 : search_space_set_type::type0A;
  pdsch.dci_fmt = dci_dl_format::f1_0;

  // [Implementation-defined] A PDSCH scheduled with DCI format 1_0 is neither precoded nor beamformed.
  pdsch.precoding_and_beamforming = make_default_precoding();

  populate_pdsch_pwr_offsets(pdsch, cell_cfg);
}

void ocudu::build_pdsch_f1_0_p_rnti(pdsch_information&                  pdsch,
                                    const cell_configuration&           cell_cfg,
                                    units::bytes                        tbs_bytes,
                                    const dci_1_0_p_rnti_configuration& dci_cfg,
                                    const crb_interval&                 crbs,
                                    const ofdm_symbol_range&            symbols,
                                    const dmrs_information&             dmrs_info)
{
  const bwp_downlink_common&        bwp_dl = cell_cfg.params.dl_cfg_common.init_dl_bwp;
  const search_space_configuration& ss_cfg =
      bwp_dl.pdcch_common.search_spaces[*bwp_dl.pdcch_common.paging_search_space_id];
  const coreset_configuration& cs_cfg =
      ss_cfg.get_coreset_id() == to_coreset_id(0) ? *bwp_dl.pdcch_common.coreset0 : *bwp_dl.pdcch_common.common_coreset;
  const vrb_interval vrbs =
      crb_to_vrb_f1_0_common_ss_non_interleaved(crbs, bwp_dl.pdcch_common.coreset0->get_coreset_start_crb());

  pdsch.rnti        = rnti_t::P_RNTI;
  pdsch.bwp_cfg     = &bwp_dl.generic_params;
  pdsch.coreset_cfg = &cs_cfg;
  pdsch.symbols     = symbols;
  pdsch.rbs         = vrbs;
  // As per TS 38.211, Section 7.3.1.1, n_ID is set to Physical Cell ID.
  pdsch.n_id       = cell_cfg.params.pci;
  pdsch.nof_layers = 1;

  pdsch.mcs_table = pdsch_mcs_table::qam64;

  pdsch_codeword& cw    = pdsch.codewords.emplace_back();
  cw.mcs_index          = dci_cfg.modulation_coding_scheme;
  cw.mcs_descr          = pdsch_mcs_get_config(pdsch.mcs_table, cw.mcs_index);
  cw.tb_size_bytes      = tbs_bytes;
  pdsch.dmrs            = dmrs_info;
  pdsch.vrb_prb_mapping = vrb_to_prb::mapping_type::non_interleaved;
  pdsch.ss_set_type     = search_space_set_type::type2;
  pdsch.dci_fmt         = dci_dl_format::f1_0;

  // [Implementation-defined] A PDSCH scheduled with DCI format 1_0 is neither precoded nor beamformed.
  pdsch.precoding_and_beamforming = make_default_precoding();

  populate_pdsch_pwr_offsets(pdsch, cell_cfg);
}

void ocudu::build_pdsch_f1_0_ra_rnti(pdsch_information&                   pdsch,
                                     units::bytes                         tbs_bytes,
                                     rnti_t                               rnti,
                                     const cell_configuration&            cell_cfg,
                                     const dci_1_0_ra_rnti_configuration& dci_cfg,
                                     const crb_interval&                  crbs,
                                     const dmrs_information&              dmrs_info)
{
  const bwp_downlink_common&        bwp_dl = cell_cfg.params.dl_cfg_common.init_dl_bwp;
  const search_space_configuration& ss_cfg = bwp_dl.pdcch_common.search_spaces[bwp_dl.pdcch_common.ra_search_space_id];
  const coreset_configuration&      cs_cfg =
      ss_cfg.get_coreset_id() == to_coreset_id(0) ? *bwp_dl.pdcch_common.coreset0 : *bwp_dl.pdcch_common.common_coreset;
  const vrb_interval vrbs = crb_to_vrb_f1_0_common_ss_non_interleaved(
      crbs, cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0->get_coreset_start_crb());
  const auto& pdsch_td_res_alloc_list =
      get_ra_rnti_pdsch_time_domain_list(cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common,
                                         cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.cp,
                                         cell_cfg.params.dmrs_typeA_pos);

  pdsch.rnti        = rnti;
  pdsch.bwp_cfg     = &bwp_dl.generic_params;
  pdsch.coreset_cfg = &cs_cfg;
  pdsch.rbs         = vrbs;
  pdsch.symbols     = pdsch_td_res_alloc_list[dci_cfg.time_resource].symbols;

  pdsch.mcs_table = pdsch_mcs_table::qam64;

  pdsch_codeword& cw = pdsch.codewords.emplace_back();
  cw.mcs_index       = dci_cfg.modulation_coding_scheme;
  cw.mcs_descr       = pdsch_mcs_get_config(pdsch.mcs_table, cw.mcs_index);
  cw.tb_size_bytes   = tbs_bytes;
  // Implementation-defined.
  cw.rv_index = 0;

  pdsch.dmrs = dmrs_info;
  // As per TS 38.211, Section 7.3.1.1, n_ID is set to Physical Cell ID for RA-RNTI.
  pdsch.n_id            = cell_cfg.params.pci;
  pdsch.nof_layers      = 1;
  pdsch.vrb_prb_mapping = vrb_to_prb::mapping_type::non_interleaved;
  pdsch.ss_set_type     = search_space_set_type::type1;
  pdsch.dci_fmt         = dci_dl_format::f1_0;

  // [Implementation-defined] A PDSCH scheduled with DCI format 1_0 is neither precoded nor beamformed.
  pdsch.precoding_and_beamforming = make_default_precoding();

  populate_pdsch_pwr_offsets(pdsch, cell_cfg);
}

void ocudu::build_pdsch_f1_0_tc_rnti(pdsch_information&                   pdsch,
                                     const pdsch_config_params&           pdsch_cfg,
                                     units::bytes                         tbs_bytes,
                                     rnti_t                               rnti,
                                     const cell_configuration&            cell_cfg,
                                     const dci_1_0_tc_rnti_configuration& dci_cfg,
                                     const vrb_interval&                  vrbs,
                                     bool                                 is_new_data)
{
  const bwp_downlink_common& bwp_dl = cell_cfg.params.dl_cfg_common.init_dl_bwp;

  pdsch.rnti = rnti;

  // PDSCH resources.
  pdsch.bwp_cfg = &bwp_dl.generic_params;
  // See 3GPP TS 38.213, clause 10.1 - CSS set configured by ra-SearchSpace is used for CRC scrambled by TC-RNTI.
  const search_space_configuration& ss = bwp_dl.pdcch_common.search_spaces[bwp_dl.pdcch_common.ra_search_space_id];
  if (ss.get_coreset_id() == to_coreset_id(0)) {
    pdsch.coreset_cfg = &*bwp_dl.pdcch_common.coreset0;
  } else {
    pdsch.coreset_cfg = &*bwp_dl.pdcch_common.common_coreset;
  }
  pdsch.rbs     = vrbs;
  pdsch.symbols = pdsch_cfg.symbols;

  pdsch.dmrs = pdsch_cfg.dmrs;
  // See TS 38.211, 7.3.1.1. - Scrambling.
  pdsch.n_id            = cell_cfg.params.pci;
  pdsch.vrb_prb_mapping = vrb_to_prb::mapping_type::non_interleaved;
  // See TS38.213, 10.1. - Type1-PDCCH CSS set for CRC scrambled by a TC-RNTI on the PCell.
  pdsch.ss_set_type = search_space_set_type::type1;
  pdsch.dci_fmt     = dci_dl_format::f1_0;
  pdsch.harq_id     = to_harq_id(dci_cfg.harq_process_number);
  pdsch.nof_layers  = 1U;

  // [Implementation-defined] A PDSCH scheduled with DCI format 1_0 is neither precoded nor beamformed.
  pdsch.precoding_and_beamforming = make_default_precoding();

  populate_pdsch_pwr_offsets(pdsch, cell_cfg);

  pdsch.mcs_table = pdsch_cfg.mcs_table;

  // One Codeword.
  pdsch_codeword& cw = pdsch.codewords.emplace_back();
  cw.new_data        = is_new_data;
  cw.rv_index        = dci_cfg.redundancy_version;
  cw.mcs_index       = dci_cfg.modulation_coding_scheme;
  cw.mcs_descr       = pdsch_mcs_get_config(pdsch.mcs_table, cw.mcs_index);
  cw.tb_size_bytes   = tbs_bytes;
}

void ocudu::build_pdsch_f1_0_c_rnti(pdsch_information&                  pdsch,
                                    const pdsch_config_params&          pdsch_cfg,
                                    units::bytes                        tbs_bytes,
                                    rnti_t                              rnti,
                                    const cell_configuration&           cell_cfg,
                                    const search_space_info&            ss_info,
                                    const dci_1_0_c_rnti_configuration& dci_cfg,
                                    const vrb_interval&                 vrbs,
                                    bool                                is_new_data)
{
  const coreset_configuration& cs_cfg     = ss_info.coreset->cfg();
  const sched_bwp_config&      active_bwp = *ss_info.bwp;

  pdsch.rnti        = rnti;
  pdsch.bwp_cfg     = &active_bwp.dl.cfg();
  pdsch.coreset_cfg = &cs_cfg;

  pdsch.rbs             = vrbs;
  pdsch.symbols         = pdsch_cfg.symbols;
  pdsch.dmrs            = pdsch_cfg.dmrs;
  pdsch.vrb_prb_mapping = vrb_to_prb::mapping_type::non_interleaved;
  // See TS38.213, 10.1.
  pdsch.ss_set_type =
      not ss_info.cfg->is_common_search_space() ? search_space_set_type::ue_specific : search_space_set_type::type3;
  pdsch.dci_fmt = dci_dl_format::f1_0;
  pdsch.harq_id = to_harq_id(dci_cfg.harq_process_number);
  // See TS 38.211, 7.3.1.1. - Scrambling.
  pdsch.n_id =
      get_pdsch_n_id(cell_cfg.params.pci, active_bwp, dci_dl_format::f1_0, ss_info.cfg->is_common_search_space());
  pdsch.nof_layers = 1;

  // [Implementation-defined] A PDSCH scheduled with DCI format 1_0 is neither precoded nor beamformed.
  pdsch.precoding_and_beamforming = make_default_precoding();

  populate_pdsch_pwr_offsets(pdsch, cell_cfg);

  pdsch.mcs_table = pdsch_cfg.mcs_table;

  // One Codeword.
  pdsch_codeword& cw = pdsch.codewords.emplace_back();
  cw.new_data        = is_new_data;
  cw.rv_index        = dci_cfg.redundancy_version;
  cw.mcs_index       = dci_cfg.modulation_coding_scheme;
  cw.mcs_descr       = pdsch_mcs_get_config(pdsch.mcs_table, cw.mcs_index);
  cw.tb_size_bytes   = tbs_bytes;
}

void ocudu::build_pdsch_f1_1_c_rnti(pdsch_information&              pdsch,
                                    const pdsch_config_params&      pdsch_cfg,
                                    sch_mcs_tbs                     mcs_tbs_info,
                                    rnti_t                          rnti,
                                    const ue_cell_configuration&    ue_cell_cfg,
                                    search_space_id                 ss_id,
                                    const dci_1_1_configuration&    dci_cfg,
                                    const vrb_alloc&                vrbs,
                                    bool                            is_new_data,
                                    const ue_channel_state_manager& cs_mgr)
{
  const cell_configuration&    cell_cfg       = ue_cell_cfg.cell_cfg_common;
  const search_space_info&     ss_info        = ue_cell_cfg.search_space(ss_id);
  const coreset_configuration& cs_cfg         = ss_info.coreset->cfg();
  const sched_bwp_config&      active_bwp     = *ss_info.bwp;
  const bwp_configuration&     active_bwp_cfg = active_bwp.dl.cfg();

  pdsch.rnti        = rnti;
  pdsch.bwp_cfg     = &active_bwp_cfg;
  pdsch.coreset_cfg = &cs_cfg;

  pdsch.rbs             = vrbs;
  pdsch.symbols         = pdsch_cfg.symbols;
  pdsch.dmrs            = pdsch_cfg.dmrs;
  pdsch.vrb_prb_mapping = dci_cfg.vrb_prb_mapping.has_value() and dci_cfg.vrb_prb_mapping.value()
                              ? ue_cell_cfg.init_bwp().dl.pdsch().ded()->vrb_to_prb_interleaving
                              : vrb_to_prb::mapping_type::non_interleaved;
  // See TS38.213, 10.1.
  pdsch.ss_set_type = search_space_set_type::ue_specific;
  pdsch.dci_fmt     = dci_dl_format::f1_1;
  pdsch.harq_id     = to_harq_id(dci_cfg.harq_process_number);
  // See TS 38.211, 7.3.1.1. - Scrambling.
  pdsch.n_id =
      get_pdsch_n_id(cell_cfg.params.pci, active_bwp, dci_dl_format::f1_1, ss_info.cfg->is_common_search_space());
  pdsch.nof_layers = pdsch_cfg.nof_layers;

  pdsch.mcs_table = pdsch_cfg.mcs_table;

  // TODO: Add second Codeword when supported.
  // One Codeword.
  pdsch_codeword& cw = pdsch.codewords.emplace_back();
  cw.new_data        = is_new_data;
  cw.rv_index        = dci_cfg.tb1_redundancy_version;
  cw.mcs_index       = dci_cfg.tb1_modulation_coding_scheme;
  cw.mcs_descr       = pdsch_mcs_get_config(pdsch.mcs_table, cw.mcs_index);
  cw.tb_size_bytes   = mcs_tbs_info.tbs;

  // Beamforming and precoding.
  pdsch.precoding_and_beamforming = cs_mgr.get_precoding(pdsch_cfg.nof_layers);

  populate_pdsch_pwr_offsets(pdsch, cell_cfg);
}

void ocudu::build_pusch_f0_0_tc_rnti(pusch_information&                   pusch,
                                     const pusch_config_params&           pusch_cfg,
                                     units::bytes                         tbs_bytes,
                                     rnti_t                               rnti,
                                     const cell_configuration&            cell_cfg,
                                     const dci_0_0_tc_rnti_configuration& dci_cfg,
                                     const vrb_interval&                  vrbs,
                                     bool                                 is_new_data)
{
  // TODO.
  pusch.intra_slot_freq_hopping = false;
  pusch.pusch_second_hop_prb    = 0;
  pusch.is_cg                   = false;
  pusch.tx_direct_current_location =
      dc_offset_helper::pack(cell_cfg.expert_cfg.ue.initial_ul_dc_offset, cell_cfg.nof_ul_prbs);
  pusch.ul_freq_shift_7p5khz = false;
  pusch.dmrs_hopping_mode    = pusch_information::dmrs_hopping_mode::no_hopping;

  pusch.rnti = rnti;

  // PUSCH resources.
  pusch.bwp_cfg = &cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params;
  pusch.rbs     = vrbs;
  pusch.symbols = pusch_cfg.symbols;

  // Determine transform precoding.
  pusch.transform_precoding = cell_cfg.use_msg3_transform_precoder();

  // MCS.
  pusch.mcs_table = pusch_cfg.mcs_table;
  pusch.mcs_index = dci_cfg.modulation_coding_scheme;
  pusch.mcs_descr = pusch_mcs_get_config(pusch.mcs_table, pusch.mcs_index, pusch.transform_precoding, false);

  // As per TS 38.211, Section 6.3.1.1, n_ID is set to Physical Cell ID for TC-RNTI.
  pusch.n_id       = cell_cfg.params.pci;
  pusch.nof_layers = pusch_cfg.nof_layers;
  pusch.dmrs       = pusch_cfg.dmrs;
  // TS 38.211, Section 6.4.1.1.1.2, n^RS_ID is set to to Physical Cell ID for TC-RNTI.
  pusch.pusch_dmrs_id = cell_cfg.params.pci;
  pusch.rv_index      = dci_cfg.redundancy_version;
  // TS 38.321, 5.4.2.1 - "For UL transmission with UL grant in RA Response or for UL transmission for MSGA payload,
  // HARQ process identifier 0 is used".
  pusch.harq_id  = to_harq_id(0);
  pusch.new_data = is_new_data;

  pusch.tb_size_bytes = tbs_bytes;
  // Set number of CB to zero if no CBs are being used.
  pusch.nof_cb = 0;
}

void ocudu::build_pusch_f0_0_c_rnti(pusch_information&                  pusch,
                                    rnti_t                              rnti,
                                    const pusch_config_params&          pusch_cfg,
                                    units::bytes                        tbs_bytes,
                                    const cell_configuration&           cell_cfg,
                                    const bwp_uplink_common&            ul_bwp,
                                    const dci_0_0_c_rnti_configuration& dci_cfg,
                                    const vrb_interval&                 vrbs,
                                    bool                                is_new_data)
{
  // TODO.
  pusch.intra_slot_freq_hopping = false;
  pusch.is_cg                   = false;
  pusch.pusch_second_hop_prb    = 0;
  pusch.tx_direct_current_location =
      dc_offset_helper::pack(cell_cfg.expert_cfg.ue.initial_ul_dc_offset, cell_cfg.nof_ul_prbs);
  pusch.ul_freq_shift_7p5khz = false;
  pusch.dmrs_hopping_mode    = pusch_information::dmrs_hopping_mode::no_hopping;

  pusch.rnti = rnti;

  // PUSCH resources.
  pusch.bwp_cfg = &ul_bwp.generic_params;
  pusch.rbs     = vrbs;
  pusch.symbols = pusch_cfg.symbols;

  // The use of the transform precoder for PUSCH scheduled via DCI Format 0_0 is determined by the parameter
  // msg3-transformPrecoder.
  pusch.transform_precoding = cell_cfg.use_msg3_transform_precoder();

  // MCS.
  pusch.mcs_table = pusch_cfg.mcs_table;
  pusch.mcs_index = dci_cfg.modulation_coding_scheme;
  pusch.mcs_descr =
      pusch_mcs_get_config(pusch.mcs_table, pusch.mcs_index, pusch.transform_precoding, pusch_cfg.tp_pi2bpsk_present);

  pusch.n_id          = cell_cfg.params.pci;
  pusch.dmrs          = pusch_cfg.dmrs;
  pusch.pusch_dmrs_id = cell_cfg.params.pci;

  // TBS.
  pusch.nof_layers    = pusch_cfg.nof_layers;
  pusch.tb_size_bytes = tbs_bytes;
  pusch.nof_cb        = 0;

  // HARQ.
  pusch.rv_index = dci_cfg.redundancy_version;
  pusch.harq_id  = to_harq_id(dci_cfg.harq_process_number);
  pusch.new_data = is_new_data;
}

void ocudu::build_pusch_f0_1_c_rnti(pusch_information&           pusch,
                                    rnti_t                       rnti,
                                    const pusch_config_params&   pusch_cfg,
                                    sch_mcs_tbs                  mcs_tbs_info,
                                    const ue_cell_configuration& ue_cell_cfg,
                                    search_space_id              ss_id,
                                    const dci_0_1_configuration& dci_cfg,
                                    const vrb_interval&          vrbs,
                                    bool                         is_new_data)
{
  const cell_configuration&                cell_cfg      = ue_cell_cfg.cell_cfg_common;
  const search_space_info&                 ss_info       = ue_cell_cfg.search_space(ss_id);
  const sched_bwp_config&                  bwp_info      = *ss_info.bwp;
  const bwp_uplink_dedicated*              bwp_ul_ded    = bwp_info.ul.ded();
  const bwp_uplink_common&                 bwp_ul_cmn    = bwp_info.ul.common();
  const std::optional<rach_config_common>& opt_rach_cfg  = bwp_ul_cmn.rach_cfg_common;
  const std::optional<pusch_config>&       pusch_cfg_ded = bwp_ul_ded->pusch_cfg;

  // TODO: Populate based on config.
  pusch.intra_slot_freq_hopping = false;
  pusch.is_cg                   = false;
  pusch.pusch_second_hop_prb    = 0;
  pusch.tx_direct_current_location =
      dc_offset_helper::pack(cell_cfg.expert_cfg.ue.initial_ul_dc_offset, cell_cfg.nof_ul_prbs);
  pusch.ul_freq_shift_7p5khz = false;
  pusch.dmrs_hopping_mode    = pusch_information::dmrs_hopping_mode::no_hopping;

  pusch.rnti = rnti;

  // PUSCH resources.
  pusch.bwp_cfg = &bwp_info.ul.cfg();
  pusch.rbs     = vrbs;
  pusch.symbols = pusch_cfg.symbols;

  // MCS.
  pusch.mcs_table = pusch_cfg.mcs_table;
  pusch.mcs_index = dci_cfg.modulation_coding_scheme;
  pusch.mcs_descr = pusch_mcs_get_config(pusch.mcs_table,
                                         pusch.mcs_index,
                                         ue_cell_cfg.use_pusch_transform_precoding_dci_0_1(),
                                         pusch_cfg.tp_pi2bpsk_present);

  pusch.n_id          = cell_cfg.params.pci;
  pusch.pusch_dmrs_id = cell_cfg.params.pci;
  if (opt_rach_cfg.has_value()) {
    pusch.transform_precoding = opt_rach_cfg.value().msg3_transform_precoder;
  }

  // Dedicated config overrides previously set value.
  if (pusch_cfg_ded.has_value()) {
    pusch.transform_precoding = false;
    if (pusch_cfg_ded.value().trans_precoder != pusch_config::transform_precoder::not_set) {
      pusch.transform_precoding = pusch_cfg_ded.value().trans_precoder == pusch_config::transform_precoder::enabled;
    }

    if (pusch_cfg_ded.value().data_scrambling_id_pusch.has_value()) {
      pusch.n_id = pusch_cfg_ded.value().data_scrambling_id_pusch.value();
    }
  }

  pusch.dmrs = pusch_cfg.dmrs;

  // TBS.
  pusch.nof_layers    = pusch_cfg.nof_layers;
  pusch.tb_size_bytes = mcs_tbs_info.tbs;
  // TODO: Set based on CobeBook config.
  pusch.nof_cb = 0;

  // HARQ.
  pusch.rv_index = dci_cfg.redundancy_version;
  pusch.harq_id  = to_harq_id(dci_cfg.harq_process_number);
  pusch.new_data = is_new_data;
}

void ocudu::build_pusch_cs_rnti(pusch_information&           pusch,
                                rnti_t                       cs_rnti,
                                const pusch_config_params&   pusch_cfg,
                                sch_mcs_tbs                  mcs_tbs_info,
                                const ue_cell_configuration& ue_cell_cfg,
                                const sched_bwp_config&      bwp_info,
                                const vrb_interval&          vrbs,
                                uint8_t                      rv_index,
                                harq_id_t                    harq_id)
{
  const cell_configuration& cell_cfg = ue_cell_cfg.cell_cfg_common;
  ocudu_assert(bwp_info.ul.ded() != nullptr and bwp_info.ul.ded()->cg_cfg.has_value(),
               "Configured Grant configuration doesn't exist");
  const cg_configuration& cg_cfg = bwp_info.ul.ded()->cg_cfg.value();

  pusch.intra_slot_freq_hopping = false;
  pusch.is_cg                   = true;
  pusch.pusch_second_hop_prb    = 0;
  pusch.tx_direct_current_location =
      dc_offset_helper::pack(cell_cfg.expert_cfg.ue.initial_ul_dc_offset, cell_cfg.nof_ul_prbs);
  pusch.ul_freq_shift_7p5khz = false;
  pusch.dmrs_hopping_mode    = pusch_information::dmrs_hopping_mode::no_hopping;
  pusch.rnti                 = cs_rnti;
  pusch.bwp_cfg              = &bwp_info.ul.cfg();
  pusch.rbs                  = vrbs;
  pusch.symbols              = pusch_cfg.symbols;

  // Backup to MSG3 transform precode in RACH-ConfigCommon if CG transform precoding is not set.
  pusch.transform_precoding = cg_cfg.trans_precoder.value_or(ue_cell_cfg.cell_cfg_common.use_msg3_transform_precoder());
  pusch.mcs_table           = pusch.transform_precoding ? cg_cfg.mcs_table_transform_precoder : cg_cfg.mcs_table;
  pusch.mcs_index           = mcs_tbs_info.mcs.value();
  // tp_pi2bpsk is only set with dynamic grants;
  constexpr bool tp_pi2bpsk_present = false;
  pusch.mcs_descr =
      pusch_mcs_get_config(pusch.mcs_table, pusch.mcs_index, pusch.transform_precoding, tp_pi2bpsk_present);

  // As per TS 38.211, Section 6.3.1.1, for Configured Grant (which do not have any \c dataScramblingIdentityPUSCH
  // field), n_ID defaults to the Physical Cell ID.
  pusch.n_id = cell_cfg.params.pci;
  // This is the only value we support, as \c nPUSCH-Identity in \c cg-DMRS-Configuration is not set at the moment (ref.
  // to Section 6.4.1.1.1.2, TS 38.211).
  pusch.pusch_dmrs_id = cell_cfg.params.pci;
  pusch.dmrs          = pusch_cfg.dmrs;
  pusch.nof_layers    = pusch_cfg.nof_layers;
  pusch.tb_size_bytes = mcs_tbs_info.tbs;
  // Only 1 Codeword supported.
  pusch.nof_cb = 1U;

  // HARQ.
  pusch.rv_index = rv_index;
  pusch.harq_id  = harq_id;
  // In CG, the periodic grants are always new transmissions.
  pusch.new_data = true;
}
