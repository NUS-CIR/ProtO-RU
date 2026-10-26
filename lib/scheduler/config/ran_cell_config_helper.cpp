// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "../support/dmrs_helpers.h"
#include "../support/prbs_calculator.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/pdcch/pdcch_candidates.h"
#include "ocudu/ran/pdcch/pdcch_type0_css_coreset_config.h"
#include "ocudu/ran/pdsch/pdsch_constants.h"
#include "ocudu/ran/prach/prach_helper.h"
#include "ocudu/ran/prach/prach_time_mapping.h"
#include "ocudu/ran/sib/sib_helper.h"
#include "ocudu/ran/slot_pdu_capacity_constants.h"
#include "ocudu/ran/ssb/ssb_helper.h"
#include "ocudu/ran/ssb/ssb_mapping.h"
#include "ocudu/ran/tdd/tdd_ul_dl_config.h"
#include "ocudu/scheduler/config/pucch_guardbands.h"
#include "ocudu/scheduler/config/pucch_resource_generator.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include "ocudu/scheduler/config/time_domain_resource_helper.h"
#include "ocudu/scheduler/result/dmrs_info.h"
#include <algorithm>
#include <numeric>

using namespace ocudu;

config_helpers::cell_config_builder_params_extended::cell_config_builder_params_extended(
    const cell_config_builder_params& source) :
  cell_config_builder_params(source)
{
  auto_derive_params();

  // Derive the maximum number of DL layers: use the configured max_rank if set (already validated against the number
  // of DL antenna ports and the single-codeword layer limit), otherwise bound the number of DL antenna ports by the
  // single-codeword layer limit (a cell may have more ports, e.g. 8T8R, than usable layers).
  max_nof_layers =
      max_rank.value_or(std::min<unsigned>(dl_carrier.nof_ant, pdsch_constants::MAX_NOF_LAYERS_PER_CODEWORD));

  cell_nof_crbs =
      band_helper::get_n_rbs_from_bw(dl_carrier.carrier_bw, scs_common, band_helper::get_freq_range(dl_carrier.band));

  // If TDD band and TDD pattern has not been specified, generate a default one.
  if (not tdd_ul_dl_cfg_common.has_value() and band_helper::get_duplex_mode(dl_carrier.band) == duplex_mode::TDD) {
    tdd_ul_dl_cfg_common.emplace();
    tdd_ul_dl_cfg_common->ref_scs                            = scs_common;
    tdd_ul_dl_cfg_common->pattern1.dl_ul_tx_period_nof_slots = 10;
    tdd_ul_dl_cfg_common->pattern1.nof_dl_slots              = 6;
    tdd_ul_dl_cfg_common->pattern1.nof_dl_symbols            = 8;
    tdd_ul_dl_cfg_common->pattern1.nof_ul_slots              = 3;
    tdd_ul_dl_cfg_common->pattern1.nof_ul_symbols            = 0;
  } else if (tdd_ul_dl_cfg_common.has_value() and band_helper::get_duplex_mode(dl_carrier.band) != duplex_mode::TDD) {
    report_error("TDD pattern has been set for non-TDD band\n");
  }

  // Compute and store final SSB position based on (selected) values.
  ssb_arfcn = band_helper::get_ssb_arfcn(dl_carrier.arfcn_f_ref,
                                         dl_carrier.band,
                                         cell_nof_crbs,
                                         scs_common,
                                         *scs_ssb,
                                         offset_to_point_a.value(),
                                         k_ssb.value());
  ocudu_assert(ssb_arfcn.has_value(), "Unable to derive SSB location correctly");
}

template <bool IsDl>
static carrier_configuration
make_default_carrier_configuration(const config_helpers::cell_config_builder_params_extended& params)
{
  carrier_configuration cfg = params.dl_carrier;
  if constexpr (not IsDl) {
    cfg.arfcn_f_ref = band_helper::get_ul_arfcn_from_dl_arfcn(params.dl_carrier.arfcn_f_ref, cfg.band);
  }
  const min_channel_bandwidth min_channel_bw = band_helper::get_min_channel_bw(cfg.band, params.scs_common);
  ocudu_assert(bs_channel_bandwidth_to_MHz(cfg.carrier_bw) >= min_channel_bandwidth_to_MHz(min_channel_bw),
               "Carrier BW {}Mhz must be greater than or equal to minimum channel BW {}Mhz",
               bs_channel_bandwidth_to_MHz(cfg.carrier_bw),
               min_channel_bandwidth_to_MHz(min_channel_bw));
  return cfg;
}

static bwp_configuration make_default_init_bwp(const config_helpers::cell_config_builder_params_extended& params)
{
  bwp_configuration cfg{};
  cfg.scs  = params.scs_common;
  cfg.crbs = {0, params.cell_nof_crbs};
  cfg.cp   = cyclic_prefix::NORMAL;
  return cfg;
}

/// Generates a default CORESET#0 configuration. The default CORESET#0 table index value used is equal to 6.
/// \remark See TS 38.213, Table 13-1.
static coreset_configuration
make_default_coreset0_config(const config_helpers::cell_config_builder_params_extended& params)
{
  return coreset_configuration{params.dl_carrier.band,
                               *params.scs_ssb,
                               params.scs_common,
                               *params.cs0_index,
                               *params.k_ssb,
                               *params.offset_to_point_a,
                               params.pci};
}

static search_space_configuration
make_default_search_space_zero_config(const config_helpers::cell_config_builder_params_extended& params)
{
  return search_space_configuration{
      params.dl_carrier.band, params.scs_common, *params.scs_ssb, *params.cs0_index, params.ss0_index};
}

static search_space_configuration
make_default_ue_search_space_config(const config_helpers::cell_config_builder_params_extended& params)
{
  search_space_configuration cfg = make_default_common_search_space_config(params);
  cfg.set_non_ss0_coreset_id(to_coreset_id(1));
  cfg.set_non_ss0_id(to_search_space_id(2));
  cfg.set_non_ss0_monitored_dci_formats(search_space_configuration::ue_specific_dci_format::f0_1_and_1_1);
  return cfg;
}

static pdcch_config make_default_pdcch_config(const config_helpers::cell_config_builder_params_extended& params)
{
  pdcch_config pdcch_cfg{};
  // >> Add CORESET#1.
  pdcch_cfg.coresets.push_back(make_default_coreset_config(params));
  // >> Add SearchSpace#2.
  pdcch_cfg.search_spaces.push_back(make_default_ue_search_space_config(params));
  pdcch_cfg.search_spaces[0].set_non_ss0_nof_candidates(
      {0,
       config_helpers::compute_max_nof_candidates(aggregation_level::n2, pdcch_cfg.coresets[0]),
       config_helpers::compute_max_nof_candidates(aggregation_level::n4, pdcch_cfg.coresets[0]),
       0,
       0});

  return pdcch_cfg;
}

static dl_config_common make_default_dl_config_common(const config_helpers::cell_config_builder_params_extended& params)
{
  dl_config_common cfg{};

  // Configure FrequencyInfoDL.
  cfg.freq_info_dl.freq_band_list.emplace_back();
  cfg.freq_info_dl.freq_band_list.back().band = params.dl_carrier.band;
  cfg.freq_info_dl.offset_to_point_a          = params.offset_to_point_a->value();
  cfg.freq_info_dl.scs_carrier_list.emplace_back();
  cfg.freq_info_dl.scs_carrier_list.back().scs               = params.scs_common;
  cfg.freq_info_dl.scs_carrier_list.back().offset_to_carrier = 0;
  cfg.freq_info_dl.scs_carrier_list.back().carrier_bandwidth = params.cell_nof_crbs;

  cfg.freq_info_dl.absolute_frequency_ssb = params.ssb_arfcn.value();

  // \c params.dl_f_ref_arfcn refers to the ARFCN of the DL f_ref, as per TS 38.104, Section 5.4.2.1.
  const double dl_absolute_freq_point_a = band_helper::get_abs_freq_point_a_from_f_ref(
      band_helper::nr_arfcn_to_freq(params.dl_carrier.arfcn_f_ref), params.cell_nof_crbs, params.scs_common);
  // \c absolute_freq_point_a needs to be expressed as in ARFCN, as per \c absoluteFrequencyPointA definition in
  // TS 38.211, Section 4.4.4.2.
  cfg.freq_info_dl.absolute_freq_point_a = band_helper::freq_to_nr_arfcn(dl_absolute_freq_point_a);

  // Configure initial DL BWP.
  cfg.init_dl_bwp.generic_params = make_default_init_bwp(params);
  cfg.init_dl_bwp.pdcch_common.coreset0.emplace(make_default_coreset0_config(params));
  cfg.init_dl_bwp.pdcch_common.search_spaces.push_back(make_default_search_space_zero_config(params));
  cfg.init_dl_bwp.pdcch_common.search_spaces.push_back(make_default_common_search_space_config(params));
  cfg.init_dl_bwp.pdcch_common.sib1_search_space_id   = to_search_space_id(0);
  cfg.init_dl_bwp.pdcch_common.paging_search_space_id = to_search_space_id(1);
  cfg.init_dl_bwp.pdcch_common.ra_search_space_id     = to_search_space_id(1);
  cfg.init_dl_bwp.pdsch_common.pdsch_td_alloc_list = time_domain_resource_helper::generate_dedicated_pdsch_td_res_list(
      params.tdd_ul_dl_cfg_common,
      cfg.init_dl_bwp.generic_params.cp,
      time_domain_resource_helper::calculate_minimum_pdsch_symbol(cfg.init_dl_bwp.pdcch_common,
                                                                  make_default_pdcch_config(params)));

  // Configure PCCH.
  cfg.pcch_cfg.default_paging_cycle = paging_cycle::rf128;
  cfg.pcch_cfg.nof_pf               = pcch_config::nof_pf_per_drx_cycle::oneT;
  cfg.pcch_cfg.paging_frame_offset  = 0;
  cfg.pcch_cfg.ns                   = pcch_config::nof_po_per_pf::one;

  return cfg;
}

static ul_config_common make_default_ul_config_common(const config_helpers::cell_config_builder_params_extended& params)
{
  ul_config_common cfg{};
  // This is the ARFCN of the UL f_ref, as per TS 38.104, Section 5.4.2.1.
  const arfcn_t ul_arfcn =
      band_helper::get_ul_arfcn_from_dl_arfcn(params.dl_carrier.arfcn_f_ref, params.dl_carrier.band);
  const double ul_absolute_freq_point_a = band_helper::get_abs_freq_point_a_from_f_ref(
      band_helper::nr_arfcn_to_freq(ul_arfcn), params.cell_nof_crbs, params.scs_common);
  // \c absolute_freq_point_a needs to be expressed as in ARFCN, as per \c absoluteFrequencyPointA definition in
  // TS 38.211, Section 4.4.4.2.
  cfg.freq_info_ul.absolute_freq_point_a     = band_helper::freq_to_nr_arfcn(ul_absolute_freq_point_a);
  cfg.freq_info_ul.freq_shift_7p5khz_present = params.freq_shift_7p5khz;
  cfg.freq_info_ul.scs_carrier_list.resize(1);
  cfg.freq_info_ul.scs_carrier_list[0].scs               = params.scs_common;
  cfg.freq_info_ul.scs_carrier_list[0].offset_to_carrier = 0;
  cfg.freq_info_ul.scs_carrier_list[0].carrier_bandwidth = params.cell_nof_crbs;
  cfg.freq_info_ul.freq_band_list.emplace_back();
  cfg.freq_info_ul.freq_band_list.back().band = params.dl_carrier.band;
  cfg.init_ul_bwp.generic_params              = make_default_init_bwp(params);
  cfg.init_ul_bwp.rach_cfg_common.emplace();
  cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.zero_correlation_zone_config = 0;
  cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.prach_config_index           = 16;
  if (band_helper::get_duplex_mode(params.dl_carrier.band) == duplex_mode::TDD) {
    std::optional<uint8_t> idx_found = prach_helper::find_valid_prach_config_index(
        params.scs_common,
        cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.zero_correlation_zone_config,
        *params.tdd_ul_dl_cfg_common);
    ocudu_assert(idx_found.has_value(), "Unable to find a PRACH config index for the given TDD pattern");
    cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.prach_config_index = idx_found.value();
  }

  // Although this is not specified in the TS, from our tests, the UE expects Msg1-SCS to be given when using short
  // PRACH Preambles formats. With long formats, we can set Msg1-SCS as \c invalid, in which case the UE derives the
  // PRACH SCS from \c prach-ConfigurationIndex in RACH-ConfigGeneric.
  const frequency_range freq_range = band_helper::get_freq_range(params.dl_carrier.band);
  const duplex_mode     duplex     = band_helper::get_duplex_mode(params.dl_carrier.band);
  cfg.init_ul_bwp.rach_cfg_common->msg1_scs =
      is_long_preamble(prach_configuration_get(
                           freq_range, duplex, cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.prach_config_index)
                           .format)
          ? subcarrier_spacing::invalid
          : params.scs_common;
  cfg.init_ul_bwp.rach_cfg_common->restricted_set = restricted_set_config::UNRESTRICTED;
  // Set l839 for long preamble formats, l139 for short preamble formats, as per Tables 6.3.3.1-1 and 6.3.3.1-2,
  // TS 38.211.
  cfg.init_ul_bwp.rach_cfg_common->is_prach_root_seq_index_l839 = is_long_preamble(
      prach_configuration_get(freq_range, duplex, cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.prach_config_index)
          .format);
  cfg.init_ul_bwp.rach_cfg_common->prach_root_seq_index      = 1;
  cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.msg1_fdm = 1;

  cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.ra_resp_window = 10u << to_numerology_value(params.scs_common);
  cfg.init_ul_bwp.rach_cfg_common->rach_cfg_generic.preamble_rx_target_pw = -100;

  cfg.init_ul_bwp.pusch_cfg_common.emplace();
  // - [Implementation-defined] Ensure k2 value which is less than or equal to minimum value of k1(s) exist in the
  // first entry of list. This way PDSCH(s) are scheduled before PUSCH and all DL slots are filled with PDSCH and all
  // UL slots are filled with PUSCH under heavy load. It also ensures that correct DAI value goes in the UL PDCCH of
  // DCI Format 0_1.
  cfg.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list =
      time_domain_resource_helper::generate_dedicated_pusch_td_res_list(
          params.tdd_ul_dl_cfg_common, cfg.init_ul_bwp.generic_params.cp, params.min_k2);

  cfg.init_ul_bwp.pucch_cfg_common.emplace();
  cfg.init_ul_bwp.pucch_cfg_common->pucch_resource_common        = 11;
  cfg.init_ul_bwp.pucch_cfg_common->group_hopping                = pucch_group_hopping::NEITHER;
  cfg.init_ul_bwp.pucch_cfg_common->p0_nominal                   = -90;
  cfg.init_ul_bwp.pusch_cfg_common.value().p0_nominal_with_grant = -76;
  cfg.init_ul_bwp.pusch_cfg_common.value().msg3_delta_preamble   = 6;
  cfg.init_ul_bwp.pusch_cfg_common.value().msg3_delta_power      = 8;

  return cfg;
}

static csi_helper::csi_meas_config_builder_params
make_default_csi_meas_builder_params(const config_helpers::cell_config_builder_params_extended& params)
{
  // Parameters used to generate list of CSI resources.
  csi_helper::csi_meas_config_builder_params csi_params{};
  csi_params.pci                      = params.pci;
  csi_params.nof_rbs                  = params.cell_nof_crbs;
  csi_params.nof_ports                = params.dl_carrier.nof_ant;
  csi_params.max_nof_layers           = params.max_nof_layers;
  csi_params.mcs_table                = pdsch_mcs_table::qam64;
  csi_params.csi_params.csi_rs_period = csi_helper::get_max_csi_rs_period(params.scs_common);

  if (band_helper::get_duplex_mode(params.dl_carrier.band) == duplex_mode::TDD) {
    // Set a default CSI report slot offset that falls in an UL slot.
    const auto& tdd_pattern = *params.tdd_ul_dl_cfg_common;

    const unsigned max_csi_symbol = *std::max_element(csi_params.csi_params.tracking_csi_ofdm_symbol_indices.begin(),
                                                      csi_params.csi_params.tracking_csi_ofdm_symbol_indices.end());

    const ssb_configuration ssb_cfg = make_default_ssb_config(params);
    const auto ssb_slots = ssb_helper::get_occupied_slot_offsets(ssb_cfg, params.dl_carrier.band, tdd_pattern.ref_scs);
    const auto sib1_occ  = sib_helper::get_occupied_slot_offsets(
        ssb_cfg, params.dl_carrier.band, tdd_pattern.ref_scs, params.ss0_index, params.cs0_index->value());
    if (not csi_helper::derive_valid_csi_rs_slot_offsets(csi_params.csi_params,
                                                         std::nullopt,
                                                         std::nullopt,
                                                         {},
                                                         tdd_pattern,
                                                         max_csi_symbol,
                                                         ssb_cfg.ssb_period,
                                                         ssb_slots,
                                                         sib1_occ.window_period_slots,
                                                         sib1_occ.slot_offsets)) {
      report_fatal_error("Failed to find valid csi-MeasConfig");
    }

    for (unsigned i = 0; i != nof_slots_per_tdd_period(tdd_pattern); ++i) {
      // TODO: Support reports in the special slot.
      if (is_tdd_full_ul_slot(tdd_pattern, i)) {
        csi_params.csi_params.csi_report_slot_offset = i;
      }
    }
  } else {
    csi_params.csi_params.meas_csi_slot_offsets = {2U};
  }

  return csi_params;
}

/// Builds a BWP config from a cell config builder parameters.
static bwp_builder_params
make_default_bwp_builder_params(const config_helpers::cell_config_builder_params_extended& params)
{
  bwp_builder_params bwp;
  bwp.pdcch_cfg            = make_default_pdcch_config(params);
  bwp.pdsch.max_nof_layers = params.max_nof_layers;
  bwp.pusch.min_k2         = params.min_k2;
  bwp.pucch.min_k1         = params.min_k1;
  bwp.pucch.sr_period      = static_cast<sr_periodicity>(get_nof_slots_per_subframe(params.scs_common) * 40U);
  if (params.dl_carrier.nof_ant == 4) {
    std::get<pucch_f2_params>(bwp.pucch.resources.f2_or_f3_or_f4_params).max_code_rate = max_pucch_code_rate::dot_35;
  }
  if (params.csi_rs_enabled) {
    bwp.csi = make_default_csi_meas_builder_params(params).csi_params;
  }
  return bwp;
}

carrier_configuration
config_helpers::make_default_dl_carrier_configuration(const cell_config_builder_params_extended& params)
{
  return make_default_carrier_configuration<true>(params);
}

carrier_configuration
config_helpers::make_default_ul_carrier_configuration(const cell_config_builder_params_extended& params)
{
  return make_default_carrier_configuration<false>(params);
}

ssb_configuration
config_helpers::make_default_ssb_config(const config_helpers::cell_config_builder_params_extended& params)
{
  ssb_configuration cfg{};

  cfg.scs               = *params.scs_ssb;
  cfg.offset_to_point_A = *params.offset_to_point_a;
  cfg.ssb_period        = ssb_periodicity::ms10;
  cfg.k_ssb             = *params.k_ssb;

  // Transmit SSB candidate 0 on beam 0.
  cfg.ssb_beams =
      ssb_beam_mapping(ssb_get_L_max(*params.scs_ssb, params.dl_carrier.arfcn_f_ref, params.dl_carrier.band));
  cfg.ssb_beams.set_beam(0, beam_identifier::n0);

  // The values we assign to these parameters are implementation-defined.
  cfg.ssb_block_power = -16;
  cfg.pss_to_sss_epre = ssb_pss_to_sss_epre::dB_0;

  return cfg;
}

/// Computes the PRACH frequency start right after the PUCCH resources at the lower edge of the UL BWP.
static unsigned compute_default_msg1_frequency_start(const ran_cell_config& cfg)
{
  const auto&    ul_bwp           = cfg.ul_cfg_common.init_ul_bwp;
  const unsigned bwp_size         = ul_bwp.generic_params.crbs.length();
  const unsigned pucch_res_common = ul_bwp.pucch_cfg_common->pucch_resource_common;
  const unsigned ded_rb_start =
      config_helpers::get_dedicated_pucch_rb_start(cfg.init_bwp.pucch.resources, pucch_res_common, bwp_size);

  // If the dedicated PUCCH resources do not fit in the BWP, only account for the common ones.
  std::vector<pucch_resource> ded_res;
  if (config_helpers::pucch_parameters_validator(cfg.init_bwp.pucch.resources, bwp_size, ded_rb_start).has_value()) {
    ded_res = config_helpers::generate_cell_pucch_res_list(cfg.init_bwp.pucch.resources, bwp_size, ded_rb_start);
  }
  const crb_bitmap pucch_prbs     = compute_pucch_crbs(crb_interval{0, bwp_size}, pucch_res_common, ded_res);
  const int        last_pucch_prb = pucch_prbs.find_highest(0, bwp_size / 2, true);

  // Guardband between the PUCCH and the short PRACH formats.
  const unsigned guardband = ul_bwp.rach_cfg_common->is_prach_root_seq_index_l839 ? 0U : 3U;
  return static_cast<unsigned>(last_pucch_prb + 1) + guardband;
}

ran_cell_config config_helpers::make_default_ran_cell_config(const cell_config_builder_params_extended& params)
{
  ran_cell_config cfg;
  cfg.pci           = params.pci;
  cfg.dl_carrier    = make_default_dl_carrier_configuration(params);
  cfg.ul_carrier    = make_default_ul_carrier_configuration(params);
  cfg.dl_cfg_common = make_default_dl_config_common(params);
  cfg.ul_cfg_common = make_default_ul_config_common(params);
  cfg.ssb_cfg       = make_default_ssb_config(params);
  // The CORESET duration of 3 symbols is only permitted if dmrs-typeA-Position is set to 3. Refer TS 38.211, 7.3.2.2.
  cfg.dmrs_typeA_pos = cfg.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0.value().duration() >= 3U
                           ? dmrs_typeA_position::pos3
                           : dmrs_typeA_position::pos2;
  cfg.tdd_cfg        = params.tdd_ul_dl_cfg_common;
  cfg.init_bwp       = make_default_bwp_builder_params(params);
  cfg.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.msg1_frequency_start =
      compute_default_msg1_frequency_start(cfg);
  return cfg;
}

csi_helper::csi_meas_config_builder_params
config_helpers::make_csi_meas_config_builder_params(const ran_cell_config& cell_cfg)
{
  ocudu_assert(cell_cfg.init_bwp.csi.has_value(), "CSI parameters are required to build CSI resources");

  csi_helper::csi_meas_config_builder_params csi_params;
  csi_params.pci            = cell_cfg.pci;
  csi_params.nof_rbs        = cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  csi_params.nof_ports      = cell_cfg.dl_carrier.nof_ant;
  csi_params.max_nof_layers = cell_cfg.init_bwp.pdsch.max_nof_layers;
  csi_params.mcs_table      = cell_cfg.init_bwp.pdsch.mcs_table;
  csi_params.csi_params     = cell_cfg.init_bwp.csi.value();
  return csi_params;
}

coreset_configuration
config_helpers::make_default_coreset_config(const config_helpers::cell_config_builder_params_extended& params,
                                            coreset_id                                                 cs_id)
{
  // PRBs spanning the maximnum number of CRBs possible.
  freq_resource_bitmap freq_resources(pdcch_constants::MAX_NOF_FREQ_RESOURCES);
  const unsigned       coreset_nof_resources = params.cell_nof_crbs / pdcch_constants::NOF_RB_PER_FREQ_RESOURCE;
  freq_resources.fill(0, coreset_nof_resources, true);

  // Number of symbols equal to CORESET#0 duration.
  const pdcch_type0_css_coreset_description desc = pdcch_type0_css_coreset_get(
      params.dl_carrier.band, *params.scs_ssb, params.scs_common, params.cs0_index->value(), params.k_ssb->value());

  return coreset_configuration{cs_id,
                               freq_resources,
                               desc.nof_symb_coreset,
                               std::nullopt,
                               coreset_configuration::precoder_granularity_type::same_as_reg_bundle};
}

search_space_configuration config_helpers::make_default_common_search_space_config(
    const config_helpers::cell_config_builder_params_extended& params)
{
  search_space_configuration::monitoring_symbols_within_slot_t monitoring_symbols_within_slot(
      NOF_OFDM_SYM_PER_SLOT_NORMAL_CP);
  monitoring_symbols_within_slot.set(0, true);
  return search_space_configuration{to_search_space_id(1),
                                    to_coreset_id(0),
                                    {0, 0, 1, 0, 0},
                                    search_space_configuration::common_dci_format{.f0_0_and_f1_0 = true},
                                    1,
                                    0,
                                    params.scs_common,
                                    1,
                                    monitoring_symbols_within_slot};
}

uint8_t config_helpers::compute_max_nof_candidates(aggregation_level aggr_lvl, const coreset_configuration& cs_cfg)
{
  // 1 REG = 1 RB and 1 symbol.
  // 1 CCE = 6 {PRB, symbol}. e.g. 3 PRBs over 2 symbols or 6 PRBs over 1 symbol, etc.
  // Example: 3 Frequency domain resources, 2 symbol duration contains 6 CCEs.
  const unsigned max_coreset_cces   = cs_cfg.freq_domain_resources().count() * cs_cfg.duration();
  unsigned       max_nof_candidates = max_coreset_cces / to_nof_cces(aggr_lvl);
  // See TS 38.331, SearchSpace IE.
  // aggregationLevelX - ENUMERATED {n0, n1, n2, n3, n4, n5, n6, n8}.
  if (max_nof_candidates == 7) {
    max_nof_candidates = 6;
  }
  return max_nof_candidates > PDCCH_MAX_NOF_CANDIDATES_SS ? PDCCH_MAX_NOF_CANDIDATES_SS : max_nof_candidates;
}

unsigned config_helpers::compute_last_non_srs_symbol(const ran_cell_config& cell_cfg)
{
  // The cell's SRS resources are whole blocks of srs_cfg.nof_symbols anchored at the last symbol of the slot, so the
  // region actually occupied is the largest multiple of that length that fits in the configured budget: what is left of
  // max_nof_symbols below it is too short for another resource and stays free.
  const srs_builder_params& srs_cfg         = cell_cfg.init_bwp.srs_cfg;
  const unsigned            srs_res_syms    = static_cast<unsigned>(srs_cfg.nof_symbols);
  const unsigned            nof_srs_symbols = srs_cfg.srs_type_enabled != srs_type::disabled
                                                  ? (srs_cfg.max_nof_symbols.value() / srs_res_syms) * srs_res_syms
                                                  : 0U;

  return NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - nof_srs_symbols;
}

bool config_helpers::is_cg_pusch_td_res_usable(const pusch_time_domain_resource_allocation& td,
                                               unsigned                                     last_non_srs_symbol)
{
  // The CG scheduler emits a single PUSCH per occasion, so a row asking for slot repetitions cannot be used.
  return td.symbols.stop() <= last_non_srs_symbol and td.nof_repetitions.value_or(1) == 1;
}

std::optional<unsigned> config_helpers::find_cg_pusch_td_res_idx(const ran_cell_config& cell_cfg)
{
  ocudu_assert(cell_cfg.init_bwp.cg_cfg.has_value(), "This function cannot be called if CG is not set");
  ocudu_assert(cell_cfg.ul_cfg_common.init_ul_bwp.pusch_cfg_common.has_value(),
               "PUSCH Config Common must be configured.");

  /// The list is sorted by increasing k2 first, then by decreasing \c symbols.stop(), so the first qualifying entry is
  /// the one with both the lowest k2 and the most symbols among those that avoid the SRS.

  // Symbols left free by the SRS, which sits at the end of the slot.
  const unsigned last_non_srs_symbol = compute_last_non_srs_symbol(cell_cfg);

  // PUSCH time-domain resources are sorted by increasing k2 first, then by decreasing \c symbols.stop(), so the first
  // qualifying entry is the one with both the lowest k2 and the most symbols among those that avoid the SRS.
  const auto& td_res = cell_cfg.ul_cfg_common.init_ul_bwp.pusch_cfg_common.value().pusch_td_alloc_list;
  for (unsigned n = 0, sz = td_res.size(); n != sz; ++n) {
    if (is_cg_pusch_td_res_usable(td_res[n], last_non_srs_symbol)) {
      return n;
    }
  }
  return std::nullopt;
}

cg_configuration config_helpers::make_default_cell_cg_config(const ran_cell_config& cell_cfg)
{
  ocudu_assert(cell_cfg.init_bwp.cg_cfg.has_value(), "This function cannot be called if CG is not set");

  // Build the full CG configuration from the cell-level defaults. This populates all fields of cg_configuration,
  // including rrc_configured_ul_grant_cfg, from the cg_builder_params stored in the cell config.
  // NOTE: the DU cell index is not needed for the CG configuration.
  cg_configuration default_cg_cfg = config_helpers::make_default_ue_cell_config(cell_cfg, to_du_cell_index(0U))
                                        .serv_cell_cfg.ul_config.value()
                                        .init_ul_bwp.cg_cfg.value();

  // Set Beta_offset for UCI-on-CG.
  if (cell_cfg.init_bwp.cg_cfg->uci_beta_offsets.has_value()) {
    default_cg_cfg.uci_on_pusch_cfg.beta_offsets_cfg.emplace(cell_cfg.init_bwp.cg_cfg->uci_beta_offsets.value());
  }

  if (cell_cfg.init_bwp.cg_cfg.value().is_type2()) {
    ocudu_assert(not default_cg_cfg.rrc_configured_ul_grant_cfg.has_value(),
                 "For type2 CG, rrc_configured_ul_grant_cfg must not be configured.");
    return default_cg_cfg;
  }

  // Pick the PUSCH time-domain resource that does not overlap the SRS. The cell configuration validator rejects a cell
  // in which no such resource exists, so falling back to index 0 here would silently place the CG PUSCH on top of the
  // SRS instead.
  const std::optional<unsigned> cg_td_res_idx = config_helpers::find_cg_pusch_td_res_idx(cell_cfg);
  ocudu_assert(cg_td_res_idx.has_value(),
               "No PUSCH time domain resource of the cell avoids the SRS symbols, so no CG PUSCH can be configured");
  default_cg_cfg.rrc_configured_ul_grant_cfg.value().time_domain_allocation = cg_td_res_idx.value_or(0U);

  return default_cg_cfg;
}

unsigned config_helpers::compute_nof_cg_prbs_per_ue(const ran_cell_config& cell_cfg, const cg_configuration& cg_cfg)
{
  ocudu_assert(cell_cfg.init_bwp.cg_cfg.has_value(), "This function cannot be called if CG is not set");

  const auto& pusch_td_list = cell_cfg.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;

  unsigned cg_td_res_idx = 0U;
  if (not cell_cfg.init_bwp.cg_cfg.value().is_type2()) {
    ocudu_assert(cg_cfg.rrc_configured_ul_grant_cfg.has_value(), "rrc_configured_ul_grant must be set for a Type 1 CG");
    cg_td_res_idx = cg_cfg.rrc_configured_ul_grant_cfg.value().time_domain_allocation;

    // For type1, the MCS is part of rrc_configured_ul_grant; ensure consistency between the user-set parameters and the
    // default constructed configuration.
    ocudu_assert(static_cast<uint8_t>(cell_cfg.init_bwp.cg_cfg.value().mcs) ==
                     cg_cfg.rrc_configured_ul_grant_cfg.value().mcs,
                 "MCS inconsistency detected between user-set CG parameters and default-constructed CG configuration");
  } else {
    // For type2 CG, the TD allocation is not defined yet at this point; the scheduler will choose the symbols that do
    // not collide with SRS. In the following, we proceed under that assumption, which the cell configuration validator
    // guarantees by rejecting a cell in which no PUSCH time domain resource avoids the SRS symbols.
    const std::optional<unsigned> non_srs_td_res_idx = config_helpers::find_cg_pusch_td_res_idx(cell_cfg);
    ocudu_assert(non_srs_td_res_idx.has_value(),
                 "No PUSCH time domain resource of the cell avoids the SRS symbols, so no CG PUSCH can be scheduled");
    cg_td_res_idx = non_srs_td_res_idx.value_or(0U);
  }

  const pusch_time_domain_resource_allocation& pusch_td_cfg = pusch_td_list[cg_td_res_idx];

  static constexpr unsigned nof_layers             = 1;
  static constexpr bool     are_both_cws_enabled   = false;
  const dmrs_information    dmrs                   = make_dmrs_info_dedicated(pusch_td_cfg,
                                                         cell_cfg.pci,
                                                         cell_cfg.dmrs_typeA_pos,
                                                         cg_cfg.cg_dmrs_cfg,
                                                         nof_layers,
                                                         cell_cfg.ul_carrier.nof_ant,
                                                         are_both_cws_enabled);
  const unsigned            nof_dmrs_prb           = calculate_nof_dmrs_per_rb(dmrs);
  constexpr bool            use_transform_precoder = false;
  // TODO: Import p_pi2bpsk_present from PUSCH Config once it will have been added there.
  constexpr bool            tp_pi2bpsk_present = false;
  const sch_mcs_description mcs_info =
      pusch_mcs_get_config(cg_cfg.mcs_table,
                           sch_mcs_index{static_cast<uint8_t>(cell_cfg.init_bwp.cg_cfg.value().mcs)},
                           use_transform_precoder,
                           tp_pi2bpsk_present);

  x_overhead nof_oh_prb = cell_cfg.init_bwp.pusch.x_ov_head;
  // As per TS 38.214, Section 5.1.3.2 and 6.1.4.2, and TS 38.212, Section 7.3.1.1 and 7.3.1.2, TB scaling filed is only
  // used for DCI Format 1-0 (in the DL). Therefore, for the PUSCH this is set to 0.
  constexpr unsigned tb_scaling_field = 0;

  unsigned payload_bytes = 0;
  if (std::holds_alternative<units::byterate>(cell_cfg.init_bwp.cg_cfg.value().grant_size_or_bitrate)) {
    const unsigned slots_per_sec =
        1000U * (1U << to_numerology_value(cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.scs));
    // Accumulate the requested rate (in bytes per second) over the CG periodicity (given in slots).
    payload_bytes = static_cast<unsigned>(
        std::ceil(std::get<units::byterate>(cell_cfg.init_bwp.cg_cfg.value().grant_size_or_bitrate).value() *
                  static_cast<unsigned>(cg_cfg.periodicity) / static_cast<double>(slots_per_sec)));
  } else {
    payload_bytes = std::get<units::bytes>(cell_cfg.init_bwp.cg_cfg.value().grant_size_or_bitrate).value();
  }

  const sch_prbs_tbs prbs_tbs = get_nof_prbs(prbs_calculator_sch_config{.payload_size_bytes = payload_bytes,
                                                                        .nof_symb_sh  = pusch_td_cfg.symbols.length(),
                                                                        .nof_dmrs_prb = nof_dmrs_prb,
                                                                        .nof_oh_prb = static_cast<unsigned>(nof_oh_prb),
                                                                        .mcs_descr  = mcs_info,
                                                                        .nof_layers = nof_layers,
                                                                        .tb_scaling_field = tb_scaling_field},
                                             cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.crbs.length());

  return prbs_tbs.nof_prbs;
}

std::vector<crb_interval> config_helpers::compute_cg_type2_freq_resources(const ran_cell_config& cell_cfg)
{
  ocudu_assert(cell_cfg.init_bwp.cg_cfg.has_value(), "This function cannot be called if CG is not set");

  // PUCCH occupies a block of CRBs at each edge of the UL BWP; the CG resources go in the gap in between.
  const crb_bitmap pucch_crbs          = compute_pucch_crbs(cell_cfg);
  const int        low_edge_last_crb   = pucch_crbs.find_highest(0, pucch_crbs.size() / 2, true);
  const int        high_edge_first_crb = pucch_crbs.find_lowest(pucch_crbs.size() / 2, pucch_crbs.size(), true);
  ocudu_assert(low_edge_last_crb >= 0 and high_edge_first_crb >= 0,
               "PUCCH is expected to occupy both edges of the UL BWP");
  ocudu_assert(high_edge_first_crb - low_edge_last_crb > 1, "No free PRBs for allocation that is not PUCCH");
  const unsigned nof_non_pucch_crbs = high_edge_first_crb - low_edge_last_crb - 1;

  // Calculate the RBs per CG resource and the number of CG resources that fit in the CG band.
  const cg_configuration default_cg_config = config_helpers::make_default_cell_cg_config(cell_cfg);
  const unsigned         nof_cg_rbs_per_ue = config_helpers::compute_nof_cg_prbs_per_ue(cell_cfg, default_cg_config);
  ocudu_assert(nof_cg_rbs_per_ue > 0, "The number of PRBs required per CG UE must be positive");
  if (nof_cg_rbs_per_ue == 0) {
    return {};
  }
  // Cap this the maximum number of CG resources per slot to MAX_CG_PUSCH_PDUS_PER_SLOT.
  const unsigned nof_cg_res = std::min<unsigned>(
      std::min(cell_cfg.init_bwp.cg_cfg->max_nof_cell_cg_rbs, nof_non_pucch_crbs) / nof_cg_rbs_per_ue,
      MAX_CG_PUSCH_PDUS_PER_SLOT);

  // Only considers the lower part of the spectrum, as we allocate CG resources on that side.
  std::vector<crb_interval> cg_crbs;
  cg_crbs.reserve(nof_cg_res);
  const unsigned bwp_crb_start = cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.crbs.start();
  unsigned       cg_crb_start  = static_cast<unsigned>(low_edge_last_crb) + 1;
  for (unsigned n = 0; n != nof_cg_res; ++n) {
    cg_crbs.push_back({cg_crb_start + bwp_crb_start, cg_crb_start + nof_cg_rbs_per_ue + bwp_crb_start});
    cg_crb_start += nof_cg_rbs_per_ue;
  }

  return cg_crbs;
}

std::vector<unsigned> config_helpers::compute_cg_usable_slot_offsets(const ran_cell_config& cell_cfg)
{
  ocudu_assert(cell_cfg.init_bwp.cg_cfg.has_value(), "Configured Grant must be configured");

  // Computes the PRACH periodicity, in slots.
  const auto                                ul_scs = cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.scs;
  const prach_helper::preamble_slot_mapping td_mapper(
      cell_cfg.dl_carrier.band,
      ul_scs,
      cell_cfg.ul_cfg_common.init_ul_bwp.rach_cfg_common.value().rach_cfg_generic.prach_config_index);

  const unsigned prach_period_sl =
      get_nof_slots_per_subframe(ul_scs) * static_cast<unsigned>(NOF_SUBFRAMES_PER_FRAME) * td_mapper.sfn_period();
  ocudu_assert(prach_period_sl > 0, "PRACH opportunities period must be positive");

  const auto cg_period_sl = static_cast<unsigned>(cell_cfg.init_bwp.cg_cfg.value().periodicity);

  // A CG offset recurs every CG period, and each of its occurrences falls at a different point of the TDD and PRACH
  // patterns. Both patterns realign with the CG period after the LCM of the three, so the occurrences within that
  // window are all the distinct slots the offset will ever land on.
  // NOTE: the CG period is not necessarily a multiple of the TDD period (e.g. sl16 against a 10-slot TDD period), so
  // the TDD condition has to be evaluated on every occurrence of the offset and not just on the first one.
  const unsigned tdd_period_sl  = cell_cfg.tdd_cfg.has_value() ? nof_slots_per_tdd_period(cell_cfg.tdd_cfg.value()) : 1;
  const unsigned fold_period_sl = std::lcm(std::lcm(prach_period_sl, cg_period_sl), tdd_period_sl);

  // An occurrence is usable if it is a full-UL slot and carries no PRACH occasion.
  // NOTE: is_tdd_full_ul_slot() wraps the slot index around the TDD period on its own.
  auto is_usable_occurrence = [&](unsigned n) {
    if (cell_cfg.tdd_cfg.has_value() and not is_tdd_full_ul_slot(cell_cfg.tdd_cfg.value(), n)) {
      return false;
    }
    return not td_mapper.has_prach_occasion(slot_point(ul_scs, n % prach_period_sl));
  };

  std::vector<unsigned> offsets;
  offsets.reserve(cg_period_sl);
  for (unsigned offset = 0; offset != cg_period_sl; ++offset) {
    bool usable = true;
    for (unsigned n = offset; n < fold_period_sl; n += cg_period_sl) {
      if (not is_usable_occurrence(n)) {
        usable = false;
        break;
      }
    }
    if (usable) {
      offsets.push_back(offset);
    }
  }

  return offsets;
}
