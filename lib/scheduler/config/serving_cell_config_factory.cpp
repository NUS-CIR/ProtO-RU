// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/pusch/pusch_antenna_ports_mapping.h"
#include "ocudu/ran/ssb/ssb_mapping.h"
#include "ocudu/scheduler/config/bwp_builder_params.h"
#include "ocudu/scheduler/config/cell_bwp_res_config.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/pucch_resource_builder_params.h"
#include "ocudu/scheduler/config/pucch_resource_generator.h"
#include "ocudu/scheduler/config/ran_cell_config.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/rlm_helper.h"
#include "ocudu/scheduler/config/sched_cell_config_helpers.h"
#include "ocudu/scheduler/config/serving_cell_config_builder.h"
#include "ocudu/scheduler/config/ue_bwp_config.h"
#include <vector>

using namespace ocudu;
using namespace ocudu::config_helpers;

static pdsch_config make_default_pdsch_config(const ran_cell_config& cell_cfg)
{
  pdsch_config pdsch_cfg;

  dmrs_downlink_config& dmrs_type_a = pdsch_cfg.pdsch_mapping_type_a_dmrs.emplace();
  dmrs_type_a.additional_positions  = cell_cfg.init_bwp.pdsch.additional_positions;

  pdsch_cfg.tci_states.push_back(tci_state{
      .state_id  = static_cast<tci_state_id_t>(0),
      .qcl_type1 = {.ref_sig  = {.type = qcl_info::reference_signal::reference_signal_type::ssb, .ssb = 0},
                    .qcl_type = qcl_info::qcl_type::type_d},
  });

  pdsch_cfg.res_alloc = pdsch_config::resource_allocation::resource_allocation_type_1;
  pdsch_cfg.rbg_sz    = rbg_size::config1;
  pdsch_cfg.prb_bndlg.bundling.emplace<prb_bundling::static_bundling>(
      prb_bundling::static_bundling({.sz = prb_bundling::static_bundling::bundling_size::wideband}));

  pdsch_cfg.vrb_to_prb_interleaving = cell_cfg.init_bwp.pdsch.interleaving_bundle_size;

  // According to TS 38.214 Section 5.1.2.3, prb-BundlingType size must match the VRB-to-PRB mapping type.
  switch (pdsch_cfg.vrb_to_prb_interleaving) {
    case vrb_to_prb::mapping_type::non_interleaved:
      // > If $P'_{BWP,i}$ is determined as "wideband", the UE is not expected to be scheduled with non-contiguous
      // > PRBs and the UE may assume that the same precoding is applied to the allocated resource.
      pdsch_cfg.prb_bndlg.bundling.emplace<prb_bundling::static_bundling>(
          prb_bundling::static_bundling({.sz = prb_bundling::static_bundling::bundling_size::wideband}));
      break;
    case vrb_to_prb::mapping_type::interleaved_n2:
      // > When a UE is configured with nominal RBG size = 2 for bandwidth part i according to clause 5.1.2.2.1, or
      // > when a UE is configured with interleaving unit of 2 for VRB to PRB mapping provided by the higher layer
      // > parameter vrb-ToPRB-Interleaver given by PDSCH-Config for bandwidth part i, the UE is not expected to be
      // > configured with $P'_{BWP,i} = 4$.
      pdsch_cfg.prb_bndlg.bundling.emplace<prb_bundling::static_bundling>(
          prb_bundling::static_bundling({.sz = std::nullopt}));
      break;
    case vrb_to_prb::mapping_type::interleaved_n4:
      pdsch_cfg.prb_bndlg.bundling.emplace<prb_bundling::static_bundling>(
          prb_bundling::static_bundling({.sz = prb_bundling::static_bundling::bundling_size::n4}));
      break;
  }

  if (cell_cfg.init_bwp.csi.has_value()) {
    csi_helper::csi_meas_config_builder_params csi_params =
        config_helpers::make_csi_meas_config_builder_params(cell_cfg);

    // zp-CSI-RS resources.
    pdsch_cfg.zp_csi_rs_res_list = csi_helper::make_periodic_zp_csi_rs_resource_list(csi_params);

    // periodic set of zp-CSI-RS resources.
    pdsch_cfg.p_zp_csi_rs_res = csi_helper::make_periodic_zp_csi_rs_resource_set(csi_params);
  }

  pdsch_cfg.mcs_table = cell_cfg.init_bwp.pdsch.mcs_table;

  pdsch_cfg.harq_process_num_size_dci_1_1 = pdsch_config::harq_process_num_dci_1_1_size::n4;
  pdsch_cfg.harq_process_num_size_dci_1_2 = pdsch_config::harq_process_num_dci_1_2_size::n4;

  return pdsch_cfg;
}

static std::optional<radio_link_monitoring_config> make_default_rlm_config(const ran_cell_config& cell_cfg)
{
  const rlm_resource_type rlm_type = cell_cfg.init_bwp.rlm.type;
  if (rlm_type == rlm_resource_type::default_type) {
    return std::nullopt;
  }

  const uint8_t l_max = ssb_get_L_max(cell_cfg.ssb_cfg.scs, cell_cfg.dl_carrier.arfcn_f_ref, cell_cfg.dl_carrier.band);

  rlm_helper::rlm_builder_params rlm_params;
  if (rlm_type == rlm_resource_type::ssb || rlm_type == rlm_resource_type::ssb_and_csi_rs) {
    rlm_params = rlm_helper::rlm_builder_params(rlm_type, l_max, cell_cfg.ssb_cfg.ssb_beams.get_ssb_bitmap());
  } else {
    rlm_params = rlm_helper::rlm_builder_params(rlm_type, l_max);
  }

  span<const nzp_csi_rs_resource> csi_rs_resources;
  if (cell_cfg.init_bwp.csi.has_value()) {
    csi_helper::csi_meas_config_builder_params csi_params =
        config_helpers::make_csi_meas_config_builder_params(cell_cfg);

    csi_rs_resources = csi_helper::make_nzp_csi_rs_resource_list(csi_params);
  }

  radio_link_monitoring_config rlm_cfg = rlm_helper::make_radio_link_monitoring_config(rlm_params, csi_rs_resources);
  if (rlm_cfg.rlm_resources.empty()) {
    return std::nullopt;
  }
  return rlm_cfg;
}

static beta_offsets make_default_beta_uci_on_pusch()
{
  return beta_offsets{.beta_offset_ack_idx_1    = 9,
                      .beta_offset_ack_idx_2    = 9,
                      .beta_offset_ack_idx_3    = 9,
                      .beta_offset_csi_p1_idx_1 = 9,
                      .beta_offset_csi_p1_idx_2 = 9,
                      .beta_offset_csi_p2_idx_1 = 9,
                      .beta_offset_csi_p2_idx_2 = 9};
}

static pusch_config make_default_pusch_config(const ran_cell_config& cell_cfg, const ue_bwp_config& ue_bwp_cfg)
{
  pusch_config cfg{};
  cfg.res_alloc                     = pusch_config::resource_allocation::resource_allocation_type_1;
  cfg.harq_process_num_size_dci_0_1 = pusch_config::harq_process_num_dci_0_1_size::n4;
  cfg.harq_process_num_size_dci_0_2 = pusch_config::harq_process_num_dci_0_2_size::n4;

  const auto& cell_pusch_cfg = cell_cfg.init_bwp.pusch;
  cfg.mcs_table              = cell_pusch_cfg.mcs_table;
  cfg.tx_cfg                 = ue_bwp_cfg.ul.pusch.tx_cfg;

  cfg.pusch_mapping_type_a_dmrs.emplace();
  cfg.pusch_mapping_type_a_dmrs->trans_precoder_disabled.emplace();
  cfg.pusch_mapping_type_a_dmrs->additional_positions = cell_pusch_cfg.additional_positions;

  // > Transform precoding config.
  if (cell_pusch_cfg.transform_precoding_enabled) {
    cfg.trans_precoder = pusch_config::transform_precoder::enabled;
    cfg.pusch_mapping_type_a_dmrs->trans_precoder_enabled.emplace(
        dmrs_uplink_config::transform_precoder_enabled{std::nullopt, false, false});
  } else {
    cfg.trans_precoder = pusch_config::transform_precoder::disabled;
  }

  // > UCI on PUSCH config.
  auto& uci_cfg = cfg.uci_cfg.emplace();
  if (cell_pusch_cfg.uci_beta_offsets.has_value()) {
    uci_cfg.scaling          = alpha_scaling_opt::f1;
    uci_cfg.beta_offsets_cfg = uci_on_pusch::beta_offsets_semi_static{cell_pusch_cfg.uci_beta_offsets.value()};
  } else {
    uci_cfg.scaling          = alpha_scaling_opt::f1;
    uci_cfg.beta_offsets_cfg = make_default_beta_uci_on_pusch();
  }

  // > PUSCH power control config.
  cfg.pusch_pwr_ctrl =
      pusch_config::pusch_power_control{.msg3_alpha               = alpha::alpha1,
                                        .p0_nominal_without_grant = cell_pusch_cfg.p0_nominal_without_grant,
                                        .p0_alphasets             = {},
                                        .pathloss_ref_rs          = {},
                                        .sri_pusch_mapping        = {}};
  cfg.pusch_pwr_ctrl->p0_alphasets.emplace_back(pusch_config::pusch_power_control::p0_pusch_alphaset{
      .id = static_cast<p0_pusch_alphaset_id>(0), .p0 = 0, .p0_pusch_alpha = alpha::alpha1});
  cfg.pusch_pwr_ctrl->pathloss_ref_rs.emplace_back(pusch_config::pusch_power_control::pusch_pathloss_ref_rs{
      .id = static_cast<pusch_config::pusch_power_control::pusch_pathloss_ref_rs_id>(0), .rs = 0});
  cfg.pusch_pwr_ctrl->sri_pusch_mapping.emplace_back(pusch_config::pusch_power_control::sri_pusch_pwr_ctrl{
      .id                           = static_cast<pusch_config::pusch_power_control::sri_pusch_pwr_ctrl_id>(0),
      .sri_pusch_pathloss_ref_rs_id = static_cast<pusch_config::pusch_power_control::pusch_pathloss_ref_rs_id>(0),
      .sri_p0_pusch_alphaset_id     = static_cast<p0_pusch_alphaset_id>(0),
      .closed_loop_idx = pusch_config::pusch_power_control::sri_pusch_pwr_ctrl::sri_pusch_closed_loop_index::i0});
  if (cell_pusch_cfg.p0_pusch_alpha.has_value()) {
    cfg.pusch_pwr_ctrl->p0_alphasets.front().p0_pusch_alpha = *cell_pusch_cfg.p0_pusch_alpha;
  }

  return cfg;
}

static pusch_serving_cell_config make_default_pusch_serving_cell_config(const pusch_builder_params& pusch_params)
{
  pusch_serving_cell_config cfg{};
  if (pusch_params.cbg_tx) {
    cfg.cbg_tx.emplace();
    cfg.cbg_tx->max_cgb_per_tb = static_cast<
        pusch_serving_cell_config::pusch_code_block_group_transmission::max_code_block_groups_per_transport_block>(
        *pusch_params.cbg_tx);
  }
  cfg.x_ov_head     = pusch_params.x_ov_head;
  cfg.nof_harq_proc = static_cast<pusch_serving_cell_config::nof_harq_proc_for_pusch>(pusch_params.max_harq_procs);
  cfg.ul_harq_mode  = pusch_params.ul_harq_mode;
  return cfg;
}

static srs_config make_default_srs_config(const ran_cell_config& cell_cfg, const ue_bwp_config& ue_bwp_cfg)
{
  srs_config cfg{};

  const auto& cell_srs_cfg = cell_cfg.init_bwp.srs_cfg;

  cfg.srs_res_list.emplace_back();
  // TODO: Verify correctness of the config based on what we support.
  srs_config::srs_resource& res    = cfg.srs_res_list.back();
  res.id                           = {0, static_cast<srs_config::srs_res_id>(0)};
  res.nof_ports                    = ue_bwp_cfg.ul.srs.nof_ports;
  res.tx_comb.size                 = cell_srs_cfg.tx_comb;
  res.tx_comb.tx_comb_offset       = 0;
  res.tx_comb.tx_comb_cyclic_shift = 0;
  res.res_mapping.start_pos        = static_cast<uint8_t>(cell_srs_cfg.nof_symbols) - 1U;
  res.res_mapping.nof_symb         = cell_srs_cfg.nof_symbols;
  res.res_mapping.rept_factor      = srs_nof_symbols::n1;
  res.freq_domain_pos              = 0;
  res.freq_domain_shift            = cell_srs_cfg.c_srs.has_value() ? cell_srs_cfg.freq_domain_shift.value() : 0;
  // NOTE: C_SRS, B_SRS and B_hop are chosen to disable SRS frequency hopping and to monitor SRS over smallest
  // possible BW i.e. 4 RBs. See TS 38.211, Table 6.4.1.4.3-1.
  // This is done to cater to setups of all BWs until SRS is supported in scheduler.
  // TODO: Support SRS in scheduler.
  res.freq_hop.c_srs = cell_srs_cfg.c_srs.value_or(0);
  // We assume that the frequency hopping is disabled and that the SRS occupies all possible RBs within the BWP. Refer
  // to Section 6.4.1.4.3, TS 38.211.
  res.freq_hop.b_srs = 0;
  res.freq_hop.b_hop = 0;
  res.grp_or_seq_hop = srs_group_or_sequence_hopping::neither;
  res.sequence_id    = cell_cfg.pci;

  srs_config::srs_resource_set& res_set = cfg.srs_res_set_list.emplace_back();
  // Set the SRS resource set ID to 0, as there is only 1 SRS resource set per UE.
  res_set.id = static_cast<srs_config::srs_res_set_id>(0);
  res_set.srs_res_id_list.emplace_back(static_cast<srs_config::srs_res_id>(0));
  res_set.res_type =
      srs_config::srs_resource_set::aperiodic_resource_type{.aperiodic_srs_res_trigger = 1, .slot_offset = 7};

  res_set.srs_res_set_usage = srs_usage::codebook;
  res_set.p0                = cell_srs_cfg.p0;
  res_set.pathloss_ref_rs   = 0;

  if (cell_srs_cfg.srs_type_enabled == srs_type::periodic) {
    res.res_type = srs_resource_type::periodic;
    // Set offset to 0. The offset will be updated later on, when the UE is allocated the SRS resources.
    res.periodicity_and_offset.emplace(
        srs_config::srs_periodicity_and_offset{.period = cell_srs_cfg.srs_period_prohib_time, .offset = 0});
    res_set.res_type.emplace<srs_config::srs_resource_set::periodic_resource_type>(
        srs_config::srs_resource_set::periodic_resource_type{});
  } else {
    // In case of aperiodic or disabled, we create an aperiodic SRS resource.
    res.res_type = srs_resource_type::aperiodic;
    res_set.res_type =
        srs_config::srs_resource_set::aperiodic_resource_type{.aperiodic_srs_res_trigger = 1, .slot_offset = 7};
  }

  return cfg;
}

static cg_configuration make_default_cg_config(const cg_builder_params& cg_params)
{
  cg_configuration cfg{};

  cfg.periodicity        = cg_params.periodicity;
  cfg.nof_harq_processes = static_cast<uint8_t>(cg_params.nof_harq_processes);
  cfg.mcs_table          = pusch_mcs_table::qam64;

  // TODO: add other DMRS options.
  cfg.cg_dmrs_cfg = dmrs_uplink_config{};
  cfg.cg_dmrs_cfg.trans_precoder_disabled.emplace();
  cfg.trans_precoder.emplace(false);

  // > UCI on PUSCH config.
  auto& uci_cfg            = cfg.uci_on_pusch_cfg;
  uci_cfg.scaling          = alpha_scaling_opt::f1;
  uci_cfg.beta_offsets_cfg = make_default_beta_uci_on_pusch();

  // > RRC-configured uplink grant Type 2 CG: skip rrc_configured_ul_grant.
  if (cg_params.is_type2()) {
    return cfg;
  }

  // > RRC-configured uplink grant (Type 1 CG).
  cg_configuration::rrc_configured_ul_grant grant{};
  constexpr unsigned                        nof_layers      = 1U;
  constexpr bool                            trans_precoding = false;
  grant.antenna_port                                        = get_pusch_antenna_port_mapping_row_index(
      nof_layers, trans_precoding, dmrs_config_type::type1, dmrs_max_length::len1);
  grant.dmrs_seq_initialization  = 0;
  grant.precoding_and_nof_layers = 0;
  // The values below will be overwritten when at the moment we create the UE specific config.
  grant.time_domain_offset     = 0;
  grant.time_domain_allocation = 0;
  grant.mcs                    = static_cast<uint8_t>(cg_params.mcs);
  // This is only a default value, which is overwritten by the scheduler when the actual configuration is generated.
  constexpr unsigned default_nof_rbs = 10U;
  grant.freq_domain_res              = ra_frequency_type1_configuration{0, 10, default_nof_rbs};
  cfg.rrc_configured_ul_grant_cfg.emplace(grant);

  return cfg;
}

static uplink_config make_default_ue_uplink_config(const ran_cell_config&     cell_cfg,
                                                   const cell_bwp_res_config& cell_bwp_cfg,
                                                   const ue_bwp_config&       ue_bwp_cfg)
{
  // > UL Config.
  uplink_config ul_config{};

  // >> PUCCH.
  ul_config.init_ul_bwp.pucch_cfg = config_helpers::build_pucch_config(cell_cfg, cell_bwp_cfg.ul, ue_bwp_cfg);

  // > PUSCH config.
  ul_config.init_ul_bwp.pusch_cfg.emplace(make_default_pusch_config(cell_cfg, ue_bwp_cfg));

  // > pusch-ServingCellConfig.
  ul_config.pusch_serv_cell_cfg.emplace(make_default_pusch_serving_cell_config(cell_cfg.init_bwp.pusch));

  // > SRS config.
  ul_config.init_ul_bwp.srs_cfg.emplace(make_default_srs_config(cell_cfg, ue_bwp_cfg));

  // > CG-PUSCH config.
  if (cell_cfg.init_bwp.cg_cfg.has_value()) {
    auto cg_cfg = make_default_cg_config(*cell_cfg.init_bwp.cg_cfg);
    if (cg_cfg.rrc_configured_ul_grant_cfg.has_value()) {
      std::get<ra_frequency_type1_configuration>(cg_cfg.rrc_configured_ul_grant_cfg->freq_domain_res).N_bwp_size =
          cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
    }
    ul_config.init_ul_bwp.cg_cfg.emplace(cg_cfg);
  }

  return ul_config;
}

static pdsch_serving_cell_config make_default_pdsch_serving_cell_config(const pdsch_builder_params& pdsch_params)
{
  pdsch_serving_cell_config cfg;
  cfg.nof_harq_proc   = static_cast<pdsch_serving_cell_config::nof_harq_proc_for_pdsch>(pdsch_params.max_harq_procs);
  cfg.max_mimo_layers = pdsch_params.max_nof_layers;
  cfg.dl_harq_feedback_disabled = pdsch_params.dl_harq_feedback_disabled;

  return cfg;
}

static serving_cell_config make_default_serving_cell_config(const ran_cell_config& cell_cfg,
                                                            du_cell_index_t        cell_idx,
                                                            const ue_bwp_config&   ue_bwp_cfg)
{
  serving_cell_config serv_cell;
  serv_cell.cell_index = cell_idx;

  // > PDCCH-Config.
  serv_cell.init_dl_bwp.pdcch_cfg = cell_cfg.init_bwp.pdcch_cfg;

  // > PDSCH-Config.
  serv_cell.init_dl_bwp.pdsch_cfg = make_default_pdsch_config(cell_cfg);

  // > RadioLinkMonitoringConfig.
  serv_cell.init_dl_bwp.rlm_cfg = make_default_rlm_config(cell_cfg);

  const auto cell_bwp_cfg = make_cell_bwp_res_config(cell_cfg);

  // > UL Config.
  serv_cell.ul_config.emplace(make_default_ue_uplink_config(cell_cfg, cell_bwp_cfg, ue_bwp_cfg));

  // > pdsch-ServingCellConfig.
  serv_cell.pdsch_serv_cell_cfg.emplace(make_default_pdsch_serving_cell_config(cell_cfg.init_bwp.pdsch));

  // > CSI-MeasConfig.
  serv_cell.csi_meas_cfg = build_csi_meas_config(cell_cfg, cell_bwp_cfg.ul, ue_bwp_cfg);

  return serv_cell;
}

static ue_bwp_config make_default_ue_bwp_cfg(const ran_cell_config& cell_cfg)
{
  ue_bwp_config cfg{
      .ul =
          {
              .pucch =
                  {
                      .res_set_cfg_id = pucch_resource_set_config_id(0),
                      .sr_res_id      = pucch_sr_resource_id(0),
                      .sr_offset =
                          cell_cfg.tdd_cfg.has_value() ? find_next_tdd_full_ul_slot(*cell_cfg.tdd_cfg).value() : 0U,
                  },
              .pusch =
                  {
                      .tx_cfg =
                          tx_scheme_codebook{.max_rank = 1, .codebook_subset = tx_scheme_codebook_subset::non_coherent},
                  },
              .srs =
                  {
                      .nof_ports = srs_config::srs_resource::nof_srs_ports::port1,
                  },
          },
  };
  if (cell_cfg.init_bwp.csi.has_value() and cell_cfg.init_bwp.csi->csi_report_slot_offset.has_value()) {
    cfg.ul.csi.periodic_report.emplace(ue_periodic_csi_config{
        .pucch_res_id = pucch_csi_resource_id(0), .offset = *cell_cfg.init_bwp.csi->csi_report_slot_offset});
  }

  return cfg;
}

ue_cell_config ocudu::config_helpers::make_default_ue_cell_config(const ran_cell_config& cell_cfg,
                                                                  du_cell_index_t        cell_idx)
{
  ue_cell_config cfg{};
  cfg.bwps.emplace_back(make_default_ue_bwp_cfg(cell_cfg));
  cfg.serv_cell_cfg = make_default_serving_cell_config(cell_cfg, cell_idx, cfg.bwps.front());
  return cfg;
}
