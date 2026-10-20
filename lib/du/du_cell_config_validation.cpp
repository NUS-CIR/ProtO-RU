// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/du/du_cell_config_validation.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/sys_info.h"
#include "ocudu/du/du_update_config_helpers.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/pdcch/pdcch_candidates.h"
#include "ocudu/ran/pdcch/pdcch_type0_css_occasions.h"
#include "ocudu/ran/prach/prach_configuration.h"
#include "ocudu/ran/prach/prach_frequency_mapping.h"
#include "ocudu/ran/prach/prach_preamble_information.h"
#include "ocudu/ran/prs/prs.h"
#include "ocudu/ran/prs/prs_constants.h"
#include "ocudu/ran/srs/srs_bandwidth_configuration.h"
#include "ocudu/ran/ssb/ssb_mapping.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/periodic_resource_sched_validator.h"
#include "ocudu/scheduler/config/pucch_guardbands.h"
#include "ocudu/scheduler/config/pucch_resource_generator.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/sched_cell_config_helpers.h"
#include "ocudu/scheduler/config/serving_cell_config.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include "ocudu/scheduler/config/serving_cell_config_validator.h"
#include "ocudu/scheduler/sched_consts.h"
#include "ocudu/support/config/validator_helpers.h"

using namespace ocudu;
using namespace odu;

#define CHECK_TRUE(cond, ...)                                                                                          \
  if (not(cond)) {                                                                                                     \
    return make_unexpected(fmt::format(__VA_ARGS__));                                                                  \
  }

#define CHECK_COMPARE_(val, expected_val, comparison, ...)                                                             \
  CHECK_TRUE(                                                                                                          \
      (val comparison expected_val), "Invalid {} ({} " #comparison "{})", fmt::format(__VA_ARGS__), val, expected_val)

#define CHECK_EQ(val, expected_val, ...)                                                                               \
  CHECK_TRUE((val == expected_val), "Invalid {} ({} != {})", fmt::format(__VA_ARGS__), val, expected_val)

#define CHECK_NEQ(val, forbidden_val, ...)                                                                             \
  CHECK_TRUE((val != forbidden_val), "Invalid {} ({} == {})", fmt::format(__VA_ARGS__), val, forbidden_val)

/// Checks if "val" is below or equal to "max_val".
#define CHECK_EQ_OR_BELOW(val, max_val, ...)                                                                           \
  CHECK_TRUE((val <= max_val), "Invalid {} ({} > {})", fmt::format(__VA_ARGS__), val, max_val)

/// Checks if "val" is below "max_val".
#define CHECK_BELOW(val, max_val, ...)                                                                                 \
  CHECK_TRUE((val < max_val), "Invalid {} ({} >= {})", fmt::format(__VA_ARGS__), val, max_val)

/// Checks if "val" is above or equal to "max_val".
#define CHECK_EQ_OR_ABOVE(val, max_val, ...)                                                                           \
  CHECK_TRUE((val >= max_val), "Invalid {} ({} < {})", fmt::format(__VA_ARGS__), val, max_val)

/// Checks if "val" is above "max_val".
#define CHECK_ABOVE(val, max_val, ...)                                                                                 \
  CHECK_TRUE((val > max_val), "Invalid {} ({} <= {})", fmt::format(__VA_ARGS__), val, max_val)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper typedef.
using check_outcome = error_type<std::string>;

/// Helper function to verify if a number belongs to the list of possible values of an ASN.1 enumerated type.
template <typename ASN1Enumerated, typename Number>
static bool is_valid_enum_number(Number number)
{
  ASN1Enumerated e;
  return asn1::number_to_enum(e, number);
}

/// Checks whether CORESET#0 table index is valid as per TS38.213, Table 13-{1,...,10}.
static check_outcome is_coreset0_ss0_idx_valid(const du_cell_config& cell_cfg)
{
  const subcarrier_spacing scs_common = cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.scs;
  const auto               cs0_idx    = cell_cfg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.get_coreset0();
  CHECK_TRUE(cs0_idx.has_value(), "CORESET#0 index not found in common PDCCH configuration");
  const auto ss0_idx = cell_cfg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.get_searchspace0();
  CHECK_TRUE(ss0_idx.has_value(), "SearchSpace#0 not found in common SearchSpace list");

  // TODO: Check which table to use.
  // TODO: Add checks on minimum bandwidth.
  if (scs_common == subcarrier_spacing::kHz15 and cell_cfg.ran.ssb_cfg.scs == subcarrier_spacing::kHz15) {
    // As per TS38.213, Table 13-1.
    CHECK_BELOW(cs0_idx.value(), 15, "CORESET#0 index table");
  } else if (scs_common == subcarrier_spacing::kHz30 and cell_cfg.ran.ssb_cfg.scs == subcarrier_spacing::kHz30) {
    // As per TS38.213, Table 13-4.
    CHECK_BELOW(cs0_idx.value(), 16, "CORESET#0 index table");
  }
  // The remaining cases will trigger an assert in the SSB checks.

  // This constraint is implementation-defined and comes from the fact that our PDCCH scheduler only schedules PDCCH
  // starting from the symbol index 0.
  if (scs_common == subcarrier_spacing::kHz15 and cell_cfg.ran.ssb_cfg.scs == subcarrier_spacing::kHz15) {
    // As per TS38.213, Table 13-11.
    CHECK_EQ_OR_BELOW(ss0_idx.value(), 9, "SearchSpaceZero index table");
  }

  return {};
}

/// Checks whether CORESET#0 configuration matches the values specified in TS38.211 Section 7.3.2.2.
static check_outcome is_coreset0_params_valid(const du_cell_config& cell_cfg)
{
  const coreset_configuration& cs_cfg = *cell_cfg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0;
  CHECK_EQ(fmt::underlying(cs_cfg.get_id()), 0, "CORESET#0 ID");

  // Check if Coreset0 is within the Initial DL BWP CRBs.
  const crb_interval& initial_bwp_crbs = cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.crbs;
  crb_interval        coreset0_crbs    = cs_cfg.coreset0_crbs();
  CHECK_TRUE(initial_bwp_crbs.contains(coreset0_crbs),
             "The CORESET#0 CRBs [{}, {}) falls outside of the initial DL BWP CRBs [{}, {})",
             coreset0_crbs.start(),
             coreset0_crbs.stop(),
             initial_bwp_crbs.start(),
             initial_bwp_crbs.stop());

  return {};
}

static check_outcome is_search_space_valid(const search_space_configuration& ss_cfg)
{
  CHECK_EQ_OR_BELOW(fmt::underlying(ss_cfg.get_id()),
                    fmt::underlying(ocudu::MAX_SEARCH_SPACE_ID),
                    "SearchSpace Id={}",
                    fmt::underlying(ss_cfg.get_id()));
  CHECK_EQ_OR_BELOW(fmt::underlying(ss_cfg.get_coreset_id()),
                    fmt::underlying(ocudu::MAX_CORESET_ID),
                    "SearchSpace#{} CORESET Id={}",
                    fmt::underlying(ss_cfg.get_id()),
                    fmt::underlying(ss_cfg.get_coreset_id()));
  const bool valid_period =
      is_valid_enum_number<asn1::rrc_nr::search_space_s::monitoring_slot_periodicity_and_offset_c_::types>(
          ss_cfg.get_monitoring_slot_periodicity());
  CHECK_TRUE(valid_period,
             "Invalid SearchSpace#{} slot period={}",
             fmt::underlying(ss_cfg.get_id()),
             ss_cfg.get_monitoring_slot_periodicity());
  CHECK_BELOW(ss_cfg.get_monitoring_slot_offset(),
              ss_cfg.get_monitoring_slot_periodicity(),
              "SearchSpace#{} monitoring slot offset",
              fmt::underlying(ss_cfg.get_id()));
  CHECK_EQ_OR_BELOW(ss_cfg.get_duration(),
                    ss_cfg.get_monitoring_slot_periodicity(),
                    "SearchSpace#{} monitoring slot duration",
                    fmt::underlying(ss_cfg.get_id()));
  CHECK_NEQ(ss_cfg.get_duration(), 0, "SearchSpace#{} monitoring slot duration", fmt::underlying(ss_cfg.get_id()));
  return {};
}

static check_outcome check_dl_config_common(const du_cell_config& cell_cfg)
{
  const bwp_downlink_common& bwp = cell_cfg.ran.dl_cfg_common.init_dl_bwp;
  // PDCCH
  if (bwp.pdcch_common.coreset0.has_value()) {
    HANDLE_ERROR(is_coreset0_params_valid(cell_cfg));
  }
  if (bwp.pdcch_common.sib1_search_space_id != ocudu::MAX_NOF_SEARCH_SPACES) {
    CHECK_EQ(fmt::underlying(bwp.pdcch_common.sib1_search_space_id),
             0,
             "SearchSpaceSIB1 must be equal to 0 for initial DL BWP");
  }
  if (bwp.pdcch_common.common_coreset.has_value()) {
    CHECK_NEQ(fmt::underlying(bwp.pdcch_common.common_coreset->get_id()), 0, "Common CORESET");
  }
  for (const search_space_configuration& ss : bwp.pdcch_common.search_spaces) {
    HANDLE_ERROR(is_search_space_valid(ss));
    CHECK_TRUE(ss.is_common_search_space(), "Common SearchSpace#{} type", fmt::underlying(ss.get_id()));
    const auto& dci_format_variant = ss.get_monitored_dci_formats();
    const auto& dci_format         = std::get<search_space_configuration::common_dci_format>(dci_format_variant);
    CHECK_TRUE(dci_format.f0_0_and_f1_0,
               "Common SearchSpace#{} must enable DCI format1_0 and format0_0",
               fmt::underlying(ss.get_id()));
    if (ss.get_coreset_id() == 0) {
      CHECK_TRUE(bwp.pdcch_common.coreset0.has_value(),
                 "Common SearchSpace#{} points to CORESET#0 which is inactive",
                 fmt::underlying(ss.get_id()));
    } else {
      CHECK_TRUE(bwp.pdcch_common.common_coreset.has_value() and
                     ss.get_coreset_id() == bwp.pdcch_common.common_coreset->get_id(),
                 "common SearchSpace#{} points to CORESET#{} which is inactive",
                 fmt::underlying(ss.get_id()),
                 fmt::underlying(ss.get_coreset_id()));
    }
  }
  // PDSCH
  CHECK_TRUE(not bwp.pdsch_common.pdsch_td_alloc_list.empty(), "Empty PDSCH-TimeDomainAllocationList");
  for (const auto& pdsch : bwp.pdsch_common.pdsch_td_alloc_list) {
    CHECK_EQ_OR_BELOW(pdsch.k0, 32, "PDSCH k0");

    // These limits are expressed in TS 38.214 Table 5.1.2.1-1.
    if (pdsch.map_type == sch_mapping_type::typeA) {
      CHECK_EQ_OR_BELOW(pdsch.symbols.start(), pdcch_constants::MAX_CORESET_DURATION, "PDSCH S");
      CHECK_EQ_OR_ABOVE(pdsch.symbols.length(), pdcch_constants::MAX_CORESET_DURATION, "PDSCH L");
      CHECK_EQ_OR_BELOW(pdsch.symbols.length(), get_nsymb_per_slot(bwp.generic_params.cp), "PDSCH L");
    } else {
      // Mapping Type B.
      if (bwp.generic_params.cp == cyclic_prefix::NORMAL) {
        CHECK_EQ_OR_BELOW(pdsch.symbols.start(), 12, "PDSCH S");
        CHECK_TRUE(pdsch.symbols.length() == 2 or pdsch.symbols.length() == 4 or pdsch.symbols.length() == 7,
                   "Invalid PDSCH L (valid values are 2, 4, 7)");
      } else {
        // Extended Cyclic Prefix.
        CHECK_EQ_OR_BELOW(pdsch.symbols.start(), 10, "PDSCH S");
        CHECK_TRUE(pdsch.symbols.length() == 2 or pdsch.symbols.length() == 4 or pdsch.symbols.length() == 6,
                   "Invalid PDSCH L (valid values are 2, 4, 6)");
      }
    }
  }
  return {};
}

static check_outcome check_rlm_config(const du_cell_config& cell_cfg)
{
  const auto serv_cell_cfg = config_helpers::make_default_ue_cell_config(cell_cfg.ran).serv_cell_cfg;
  const auto rlm_cfg       = serv_cell_cfg.init_dl_bwp.rlm_cfg;
  if (not rlm_cfg.has_value()) {
    return {};
  }
  const bool                       csi_enabled = cell_cfg.ran.init_bwp.csi.has_value();
  std::vector<nzp_csi_rs_resource> nzp_csi_rs_res_list;
  if (csi_enabled) {
    nzp_csi_rs_res_list =
        csi_helper::make_nzp_csi_rs_resource_list(config_helpers::make_csi_meas_config_builder_params(cell_cfg.ran));
  }
  for (const auto& rlm_res : rlm_cfg->rlm_resources) {
    CHECK_TRUE(rlm_res.resource_purpose == radio_link_monitoring_config::radio_link_monitoring_rs::purpose::rlf,
               "Radio Link Failure is the only supported Radio Link Monitoring purpose");
    if (not csi_enabled) {
      CHECK_TRUE(not std::holds_alternative<nzp_csi_rs_res_id_t>(rlm_res.detection_resource),
                 "RLM resources cannot use CSI-RS resources if CSI is not enabled");
    } else {
      if (std::holds_alternative<nzp_csi_rs_res_id_t>(rlm_res.detection_resource)) {
        const nzp_csi_rs_res_id_t csi_rs_id = std::get<nzp_csi_rs_res_id_t>(rlm_res.detection_resource);
        const auto                csi_res_it =
            std::find_if(nzp_csi_rs_res_list.begin(),
                         nzp_csi_rs_res_list.end(),
                         [csi_rs_id](const nzp_csi_rs_resource& csi_r) { return csi_r.res_id == csi_rs_id; });
        CHECK_TRUE(csi_res_it != nzp_csi_rs_res_list.end(),
                   "RLM resource id={} points at CSI-RS resource id={}, which wasn't found in the CSI configuration",
                   fmt::underlying(rlm_res.res_id),
                   fmt::underlying(csi_rs_id));
        CHECK_TRUE(csi_res_it->res_mapping.cdm == csi_rs_cdm_type::no_CDM,
                   "Only CSI-RS resource with no_CDM can be used for RLM");
        CHECK_TRUE(csi_res_it->res_mapping.nof_ports == 1U,
                   "Only CSI-RS resource configured with 1 port can be used for RLM");
        CHECK_TRUE(csi_res_it->res_mapping.freq_density == csi_rs_freq_density_type::one or
                       csi_res_it->res_mapping.freq_density == csi_rs_freq_density_type::three,
                   "Only CSI-RS resource with frequency density one or three can be used for RLM");
      }
    }

    if (std::holds_alternative<ssb_id_t>(rlm_res.detection_resource)) {
      const ssb_id_t          ssb_rs_id = std::get<ssb_id_t>(rlm_res.detection_resource);
      const ssb_beam_mapping& ssb_beams = cell_cfg.ran.ssb_cfg.ssb_beams;
      CHECK_BELOW(
          ssb_rs_id.value(), ssb_beams.get_L_max(), "SSB index of RLM resource id={}", fmt::underlying(rlm_res.res_id));
      CHECK_TRUE(ssb_beams.is_transmitted(ssb_rs_id.value()),
                 "RLM resource id={} points at SSB index={}, which is not transmitted",
                 fmt::underlying(rlm_res.res_id),
                 ssb_rs_id);
    }
  }

  const uint8_t l_max =
      ssb_get_L_max(cell_cfg.ran.ssb_cfg.scs, cell_cfg.ran.dl_carrier.arfcn_f_ref, cell_cfg.ran.dl_carrier.band);
  // Check the constrains on N_RLM values in Table 5-1, TS 38.213, are met.
  if (l_max == 4U) {
    CHECK_TRUE(rlm_cfg->rlm_resources.size() <= 2, "With SSB L_max = 4, max 2 RLM resources can be configured");
  } else if (l_max == 8U) {
    CHECK_TRUE(rlm_cfg->rlm_resources.size() <= 4, "With SSB L_max = 8, max 4 RLM resources can be configured");
  } else if (l_max == 64U) {
    CHECK_TRUE(rlm_cfg->rlm_resources.size() <= 8, "With SSB L_max = 64, max 8 RLM resources can be configured");
  }

  return {};
}

static check_outcome check_dl_config_dedicated(const du_cell_config& cell_cfg)
{
  const subcarrier_spacing scs_common = cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.scs;
  const auto&              pdcch_cfg  = cell_cfg.ran.init_bwp.pdcch_cfg;
  const auto pdsch_cfg = config_helpers::make_default_ue_cell_config(cell_cfg.ran).serv_cell_cfg.init_dl_bwp.pdsch_cfg;

  // PDCCH
  if (pdcch_cfg.has_value()) {
    for (const search_space_configuration& ss : pdcch_cfg->search_spaces) {
      const bool fallback_dci_format_in_ss2 =
          ss.is_common_search_space() or
          not(std::get<search_space_configuration::ue_specific_dci_format>(ss.get_monitored_dci_formats()) ==
              search_space_configuration::ue_specific_dci_format::f0_1_and_1_1);

      if (fallback_dci_format_in_ss2) {
        CHECK_TRUE(cell_cfg.ran.dl_carrier.nof_ant == 1,
                   "Nof. DL antennas {} cannot be greater than 1 when using fallback DCI format\n",
                   cell_cfg.ran.dl_carrier.nof_ant);

        if (pdsch_cfg.has_value()) {
          CHECK_TRUE(pdsch_cfg->mcs_table != pdsch_mcs_table::qam256,
                     "256QAM MCS table cannot be used for PDSCH with fallback DCI format in SearchSpace#2");
        }
      }

      if (pdsch_cfg.has_value() and pdsch_cfg->mcs_table == pdsch_mcs_table::qam64LowSe) {
        // As per Section 5.1.3.1, TS 38.213 and assuming MCS-C-RNTI is not supported.
        CHECK_TRUE(not ss.is_common_search_space(),
                   "64QAM Low Se MCS table cannot be used for PDSCH with DCI in Common SearchSpace");
      }

      if (pdsch_cfg.has_value() and pdsch_cfg->pdsch_mapping_type_a_dmrs.has_value() and
          pdsch_cfg->pdsch_mapping_type_a_dmrs->additional_positions == dmrs_additional_positions::pos3) {
        CHECK_TRUE(
            cell_cfg.ran.dmrs_typeA_pos == dmrs_typeA_position::pos2,
            "PDSCH dmrs-Additional-Position of pos3 is only supported when dmrs-TypeA-Position is equal to pos2");
      }
    }

    // Checks whether nof. monitored PDCCH candidates per slot for a DL BWP does not exceed maximum allowed value as per
    // TS 38.213, Table 10.1-2.
    const unsigned total_nof_monitored_pdcch_candidates =
        config_helpers::compute_tot_nof_monitored_pdcch_candidates_per_slot(*pdcch_cfg, cell_cfg.ran.dl_cfg_common);
    CHECK_EQ_OR_BELOW(total_nof_monitored_pdcch_candidates,
                      max_nof_monitored_pdcch_candidates(scs_common),
                      "Nof. PDCCH candidates monitored per slot for a DL BWP={} exceeds maximum value={}\n",
                      total_nof_monitored_pdcch_candidates,
                      max_nof_monitored_pdcch_candidates(scs_common));
  }

  check_outcome rlm_validation = check_rlm_config(cell_cfg);
  CHECK_TRUE(rlm_validation.has_value(), "Invalid Radio Link Monitoring config: {}", rlm_validation.error());

  return {};
}

static check_outcome check_ssb_configuration(const du_cell_config& cell_cfg)
{
  const ssb_configuration& ssb_cfg    = cell_cfg.ran.ssb_cfg;
  const subcarrier_spacing scs_common = cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.scs;

  // No mixed numerologies supported (yet).
  CHECK_EQ(fmt::underlying(ssb_cfg.scs),
           fmt::underlying(scs_common),
           "SSB SCS must be equal to SCS common. Mixed numerologies are not supported.");

  // Only FR1 SCS supported (for now).
  if (band_helper::get_freq_range(cell_cfg.ran.dl_carrier.band) == frequency_range::FR1) {
    CHECK_EQ_OR_BELOW(fmt::underlying(ssb_cfg.scs),
                      fmt::underlying(subcarrier_spacing::kHz30),
                      "SSB SCS must be 15kHz or 30kHz for FR1.");
  } else {
    CHECK_EQ(
        fmt::underlying(ssb_cfg.scs), fmt::underlying(subcarrier_spacing::kHz120), "SSB SCS must be 120kHz for FR2.");
  }

  CHECK_TRUE(not ssb_cfg.ssb_beams.empty(), "At least one SSB candidate must be transmitted.");

  // SearchSpace#0 holds the Type0-PDCCH monitoring occasions of the first MAX_NOF_SS0_SSB_CANDIDATES candidates only,
  // and SIB1 needs them for every transmitted candidate.
  for (uint8_t ssb_idx : ssb_cfg.ssb_beams.transmitted_indexes()) {
    CHECK_BELOW(ssb_idx, MAX_NOF_SS0_SSB_CANDIDATES, "index of a transmitted SSB candidate");
  }

  if (ssb_cfg.ssb_beams.nof_transmitted() > 1) {
    // The SS/PBCH block associated with a detected preamble is derived from the position of its PRACH occasion in the
    // occasion ordering of TS 38.213, Section 8.1, which requires the occasion to be identified unambiguously. The
    // lower layers report neither the frequency-domain occasion index nor the time-domain one.
    const rach_config_common& rach_cfg = *cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common;
    const prach_configuration prach_cfg =
        prach_configuration_get(band_helper::get_freq_range(cell_cfg.ran.dl_carrier.band),
                                band_helper::get_duplex_mode(cell_cfg.ran.dl_carrier.band),
                                rach_cfg.rach_cfg_generic.prach_config_index);
    CHECK_EQ(rach_cfg.rach_cfg_generic.msg1_fdm,
             1,
             "Frequency multiplexed PRACH occasions are not supported with multiple SSB candidates.");
    CHECK_EQ_OR_BELOW(prach_cfg.nof_occasions_within_slot,
                      1,
                      "Time multiplexed PRACH occasions are not supported with multiple SSB candidates.");
    CHECK_EQ(fmt::underlying(rach_cfg.nof_ssb_per_ro),
             fmt::underlying(ssb_per_rach_occasions::one),
             "Only one SSB per RACH occasion is supported with multiple SSB candidates.");

    // TODO: repeat the SI messages in the Type0-PDCCH CSS occasion of every transmitted SSB candidate, the way the SIB1
    // scheduler does. The SI message scheduler resolves the monitoring occasions of SearchSpace#0 for SSB index 0 only.
    if (cell_cfg.si.si_config.has_value()) {
      const auto& pdcch_common = cell_cfg.ran.dl_cfg_common.init_dl_bwp.pdcch_common;
      CHECK_TRUE(pdcch_common.other_si_search_space_id.value_or(pdcch_common.sib1_search_space_id) !=
                     to_search_space_id(0),
                 "SI messages scheduled on SearchSpace#0 are not supported with multiple SSB candidates.");
    }
  }

  // Checks that SSB does not get located outside the band.
  if (scs_common == subcarrier_spacing::kHz15) {
    // Check if k_SSB is within limits, according to the SCScommon.
    CHECK_EQ_OR_BELOW(ssb_cfg.k_ssb.value(), 11, "For SCS common 15kHz, k_SSB must be within the range [0, 11].");

    // In the following, we assume the SSB is located inside the Transmission Bandwidth Configuration of the specified
    // band. Refer to TS38.104, Section 5.3.1 for the definition of Transmission Bandwidth Configuration.
    // We assume the Initial DL BWP ranges over the whole Transmission Bandwidth Configuration.
    unsigned nof_crbs               = cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.crbs.length();
    unsigned offset_p_A_upper_bound = ssb_cfg.k_ssb.value() > 0 ? nof_crbs - NOF_SSB_PRBS - 1 : nof_crbs - NOF_SSB_PRBS;
    CHECK_EQ_OR_BELOW(
        ssb_cfg.offset_to_point_A.value(),
        offset_p_A_upper_bound,
        "Offset to PointA must be such that the SSB is located inside the Initial DL BWP, i.e, offset_to_point_A <= {}",
        offset_p_A_upper_bound);

  } else if (scs_common == subcarrier_spacing::kHz30) {
    // Check if k_SSB is within limits, according to the SCScommon.
    CHECK_EQ_OR_BELOW(ssb_cfg.k_ssb.value(), 23, "For SCS common 30kHz, k_SSB must be within the range [0, 23].");
    // Check if k_SSB is an even number, as this is a requirement coming from PHY implementation limitation.
    CHECK_TRUE(ssb_cfg.k_ssb.value() % 2 == 0, "For SCS common 30kHz, k_SSB must be an even number.");

    // In the following, we assume the SSB is located inside the Transmission Bandwidth Configuration of the specified
    // band. Refer to TS38.104, Section 5.3.1 for the definition of Transmission Bandwidth Configuration.
    // We assume the Initial DL BWP ranges over the whole Transmission Bandwidth Configuration.
    unsigned nof_crbs = cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.crbs.length();
    unsigned offset_p_A_upper_bound =
        ssb_cfg.k_ssb.value() > 0 ? (nof_crbs - NOF_SSB_PRBS - 1) * 2 : (nof_crbs - NOF_SSB_PRBS) * 2;
    CHECK_EQ_OR_BELOW(
        ssb_cfg.offset_to_point_A.value(),
        offset_p_A_upper_bound,
        "Offset to PointA must be such that the SSB is located inside the Initial DL BWP, i.e, offset_to_point_A <= {}",
        offset_p_A_upper_bound);
    CHECK_TRUE(ssb_cfg.offset_to_point_A.value() % 2 == 0,
               "With SCScommon 30kHz, Offset to PointA must be an even number",
               offset_p_A_upper_bound);
  }

  ssb_pattern_case ssb_case = band_helper::get_ssb_pattern(cell_cfg.ran.dl_carrier.band, ssb_cfg.scs);
  const uint8_t L_max = ssb_get_L_max(ssb_cfg.scs, cell_cfg.ran.dl_carrier.arfcn_f_ref, cell_cfg.ran.dl_carrier.band);
  CHECK_TRUE(ssb_cfg.ssb_beams.get_L_max() == L_max, "Mismatch between the SSB beam mapping size and L_max");

  // (Only for Lmax = 64) It is assumed in \c inOneGroup, \c ssb-PositionsInBurst, \c ServingCellConfigCommonSIB,
  // TS 38.331 that, if a group of 8-bit bitmaps [n, n+8), with n=0, 8, 16, ...56 has non-zero bits, then these 8
  // bits are common to all non-zero 8-bit bitmaps starting with n=0, 8, 16, ...56.
  if (L_max == 64) {
    const ssb_bitmap_t     ssb_bitmap                 = ssb_cfg.ssb_beams.get_ssb_bitmap();
    constexpr uint8_t      nof_groups_and_bits_per_gr = 8U;
    std::optional<uint8_t> first_non_zero_group;
    for (uint8_t group_idx = 0; group_idx != nof_groups_and_bits_per_gr; ++group_idx) {
      const auto group_8_bits =
          ssb_bitmap.extract<uint8_t>(nof_groups_and_bits_per_gr * group_idx, nof_groups_and_bits_per_gr);
      if (group_8_bits != 0U and not first_non_zero_group.has_value()) {
        first_non_zero_group.emplace(group_8_bits);
      }
      CHECK_TRUE(group_8_bits == 0U or group_8_bits == first_non_zero_group.value(),
                 "Invalid SSB bitmap for L_max = 64");
    }
  }

  // Check whether the SSB beam bitmap and L_max are compatible with SSB case and DL band.
  if (ssb_case <= ssb_pattern_case::C) {
    double     cutoff_freq_mhz_case_a_b_c      = band_helper::nr_arfcn_to_freq(CUTOFF_FREQ_ARFCN_CASE_A_B_C) / 1e6;
    double     cutoff_freq_mhz_case_c_unpaired = band_helper::nr_arfcn_to_freq(CUTOFF_FREQ_ARFCN_CASE_C_UNPAIRED) / 1e6;
    const bool ssb_case_c_unpaired =
        ssb_case == ssb_pattern_case::C and not band_helper::is_paired_spectrum(cell_cfg.ran.dl_carrier.band);

    const bool    frequency_above_cutoff = ssb_case_c_unpaired
                                               ? cell_cfg.ran.dl_carrier.arfcn_f_ref > CUTOFF_FREQ_ARFCN_CASE_C_UNPAIRED
                                               : cell_cfg.ran.dl_carrier.arfcn_f_ref > CUTOFF_FREQ_ARFCN_CASE_A_B_C;
    const uint8_t expected_L_max         = frequency_above_cutoff ? 8U : 4U;

    CHECK_EQ(L_max,
             expected_L_max,
             "For SSB case {} and frequency {} {}MHz, L_max must be {}",
             ssb_case_c_unpaired ? "C (unpaired)" : "A, B, C (paired)",
             frequency_above_cutoff ? ">" : "<=",
             ssb_case_c_unpaired ? cutoff_freq_mhz_case_c_unpaired : cutoff_freq_mhz_case_a_b_c,
             expected_L_max);
  } else if (ssb_case == ssb_pattern_case::D) {
    CHECK_EQ(L_max, 64, "For SSB case D L_max must be 64");
  } else {
    CHECK_TRUE(false, "SSB case E not supported")
  }

  return {};
}

static check_outcome check_ul_config_common(const du_cell_config& cell_cfg)
{
  const bwp_uplink_common& bwp = cell_cfg.ran.ul_cfg_common.init_ul_bwp;
  if (bwp.pusch_cfg_common.has_value()) {
    const pusch_config_common& pusch = cell_cfg.ran.ul_cfg_common.init_ul_bwp.pusch_cfg_common.value();
    CHECK_TRUE(pusch.msg3_delta_power.valid(),
               "msg3_delta_power{} in pucch_config_common not in range [-6, 8]",
               pusch.msg3_delta_power.value());
    CHECK_TRUE(pusch.msg3_delta_power.value() % 2 == 0,
               "The value set {} for msg3_delta_power must be a multiple of 2",
               pusch.msg3_delta_power.value());
  }

  // \ref prach_scheduler for the derivation of this validation.
  const prach_configuration prach_cfg = prach_configuration_get(
      band_helper::get_freq_range(cell_cfg.ran.dl_carrier.band),
      band_helper::get_duplex_mode(cell_cfg.ran.dl_carrier.band),
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.prach_config_index);

  // The information we need are not related to whether it is the last PRACH occasion.
  constexpr bool                   is_last_prach_occasion = false;
  const prach_preamble_information info =
      is_long_preamble(prach_cfg.format)
          ? get_prach_preamble_long_info(prach_cfg.format)
          : get_prach_preamble_short_info(
                prach_cfg.format,
                to_ra_subcarrier_spacing(cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.scs),
                is_last_prach_occasion);
  const unsigned prach_nof_prbs =
      prach_frequency_mapping_get(info.scs, cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.scs).nof_rb_ra;
  const unsigned prach_prb_end =
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.msg1_frequency_start +
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.msg1_fdm * prach_nof_prbs;
  CHECK_TRUE(
      prach_prb_end <= cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.crbs.length(),
      "With the current PRACH configuration index {}, MSG1 frequency start {} and MSG1 FDM {}, the resulting PRACH "
      "RBs fall outside the BWP.",
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.prach_config_index,
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.msg1_frequency_start,
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.msg1_fdm);

  if (cell_cfg.ran.init_bwp.srs_cfg.srs_type_enabled == srs_type::aperiodic and cell_cfg.ran.tdd_cfg.has_value() and
      cell_cfg.ran.tdd_cfg.value().pattern2.has_value()) {
    CHECK_TRUE(cell_cfg.ran.tdd_cfg.value().pattern1.nof_ul_symbols ==
                   cell_cfg.ran.tdd_cfg.value().pattern2.value().nof_ul_symbols,
               "With aperiodic SRS, the TDD pattern 1 and 2 must have the same number of symbols.");
  }

  return {};
}

static check_outcome check_ul_config_dedicated(const du_cell_config& cell_cfg)
{
  const auto&                       pusch_params = cell_cfg.ran.init_bwp.pusch;
  const pusch_mcs_table             mcs_table    = pusch_params.mcs_table;
  const search_space_configuration& ss2          = cell_cfg.ran.init_bwp.pdcch_cfg->search_spaces.back();
  const bool                        fallback_dci_format_in_ss2 =
      ss2.is_common_search_space() or
      not(std::get<search_space_configuration::ue_specific_dci_format>(ss2.get_monitored_dci_formats()) ==
          search_space_configuration::ue_specific_dci_format::f0_1_and_1_1);
  if (fallback_dci_format_in_ss2) {
    CHECK_TRUE(mcs_table != pusch_mcs_table::qam256,
               "256QAM MCS table cannot be used for PUSCH with fallback DCI format in SearchSpace#2");
  }
  if (mcs_table == pusch_mcs_table::qam64LowSe) {
    // As per Section 5.1.3.1, TS 38.213 and assuming MCS-C-RNTI is not supported.
    CHECK_TRUE(not ss2.is_common_search_space(),
               "64QAM Low Se MCS table cannot be used for PDSCH with DCI in Common SearchSpace");
  }
  if (pusch_params.additional_positions == dmrs_additional_positions::pos3) {
    CHECK_TRUE(cell_cfg.ran.dmrs_typeA_pos == dmrs_typeA_position::pos2,
               "PUSCH dmrs-Additional-Position of pos3 is only supported when dmrs-TypeA-Position is equal to pos2");
  }
  if (cell_cfg.ran.tdd_cfg.has_value()) {
    CHECK_TRUE(sr_periodicity_to_slot(cell_cfg.ran.init_bwp.pucch.sr_period) %
                       nof_slots_per_tdd_period(cell_cfg.ran.tdd_cfg.value()) ==
                   0,
               "Scheduling request resource periodicity that is not a submultiple of the TDD "
               "configuration periodicity is not supported.");
  }
  return {};
}

static check_outcome check_tdd_ul_dl_config(const du_cell_config& cell_cfg)
{
  if (not cell_cfg.ran.tdd_cfg.has_value()) {
    return {};
  }

  // See TS 38.214, Table 5.1.2.1-1: Valid S and L combinations.
  static constexpr unsigned pdsch_mapping_typeA_min_L_value = 3;

  const pdcch_config_common&                  common_pdcch_cfg = cell_cfg.ran.dl_cfg_common.init_dl_bwp.pdcch_common;
  const pdcch_config&                         ded_pdcch_cfg    = cell_cfg.ran.init_bwp.pdcch_cfg.value();
  const std::optional<coreset_configuration>& coreset0         = common_pdcch_cfg.coreset0;
  const std::optional<coreset_configuration>& common_coreset   = common_pdcch_cfg.common_coreset;
  const auto                                  ss0_idx          = common_pdcch_cfg.get_searchspace0();
  CHECK_TRUE(ss0_idx.has_value(), "SearchSpace#0 not found in common SearchSpace list");
  CHECK_TRUE(coreset0.has_value(), "CORESET#0 not configured");

  const pdcch_type0_css_occasion_pattern1_description& ss0_occasion =
      pdcch_type0_css_occasions_get_pattern1(pdcch_type0_css_occasion_pattern1_configuration{
          .is_fr2 = false, .ss0_index = ss0_idx.value(), .nof_symb_coreset = coreset0->duration()});

  const auto& tdd_cfg = cell_cfg.ran.tdd_cfg.value();
  CHECK_TRUE(
      (get_nof_slots_per_subframe(tdd_cfg.ref_scs) * NOF_SUBFRAMES_PER_FRAME) % nof_slots_per_tdd_period(tdd_cfg) == 0,
      "TDD configuration periodicity that is not a submultiple of the number of slots in a radio frame is "
      "not supported.");
  const auto pattern1_period_slots    = tdd_cfg.pattern1.dl_ul_tx_period_nof_slots;
  auto       pattern1_additional_slot = tdd_cfg.pattern1.nof_ul_symbols + tdd_cfg.pattern1.nof_dl_symbols > 0 ? 1 : 0;
  pattern1_additional_slot += tdd_cfg.pattern1.nof_ul_symbols + tdd_cfg.pattern1.nof_dl_symbols > 14 ? 1 : 0;
  CHECK_EQ_OR_BELOW(tdd_cfg.pattern1.nof_dl_slots + tdd_cfg.pattern1.nof_ul_slots + pattern1_additional_slot,
                    pattern1_period_slots,
                    "TDD UL DL pattern 1 configuration. UL(slots + symbols) + DL(slots + symbols) configuration "
                    "exceeds TDD pattern period.");

  if (tdd_cfg.pattern2.has_value()) {
    const auto pattern2_period_slots = tdd_cfg.pattern2.value().dl_ul_tx_period_nof_slots;
    auto       pattern2_additional_slot =
        tdd_cfg.pattern2.value().nof_ul_symbols + tdd_cfg.pattern2.value().nof_dl_symbols > 0 ? 1 : 0;
    pattern2_additional_slot +=
        tdd_cfg.pattern2.value().nof_ul_symbols + tdd_cfg.pattern2.value().nof_dl_symbols > 14 ? 1 : 0;
    CHECK_EQ_OR_BELOW(tdd_cfg.pattern2.value().nof_dl_slots + tdd_cfg.pattern2.value().nof_ul_slots +
                          pattern2_additional_slot,
                      pattern2_period_slots,
                      "TDD UL DL pattern 2 configuration. UL(slots + symbols) + DL(slots + symbols) configuration "
                      "exceeds TDD pattern period.");
  }

  // NOTE1: Number of DL PDSCH symbols must be atleast greater than SearchSpace monitoring start symbol index + CORESET
  // duration for PDSCH allocation in partial slot. Otherwise, it can be used only for PDCCH allocations.
  // NOTE2: We don't support multiple monitoring occasions in a slot belonging to a SearchSpace.
  // TODO: Consider SearchSpace periodicity while validating DL symbols in special slots.
  std::optional<unsigned> cs_duration;
  unsigned                ss_start_symbol_idx;
  // SearchSpaces in Common PDCCH configuration.
  for (const search_space_configuration& ss_cfg : common_pdcch_cfg.search_spaces) {
    cs_duration         = {};
    ss_start_symbol_idx = 0;
    if (coreset0.has_value()) {
      if (ss_cfg.get_id() == to_search_space_id(0) and ss_cfg.get_coreset_id() == coreset0->get_id()) {
        cs_duration = coreset0->duration();
        // Consider the starting index of last PDCCH monitoring occasion to account for all SSB beams.
        ss_start_symbol_idx = *std::max_element(ss0_occasion.start_symbol.begin(), ss0_occasion.start_symbol.end());
      } else if (ss_cfg.get_id() != to_search_space_id(0) and ss_cfg.get_coreset_id() == coreset0->get_id()) {
        cs_duration         = coreset0->duration();
        ss_start_symbol_idx = ss_cfg.get_first_symbol_index();
      }
    }
    if (common_coreset.has_value()) {
      if (ss_cfg.get_id() == to_search_space_id(0) and ss_cfg.get_coreset_id() == common_coreset->get_id()) {
        cs_duration = common_coreset->duration();
        // Consider the starting index of last PDCCH monitoring occasion to account for all SSB beams.
        ss_start_symbol_idx = *std::max_element(ss0_occasion.start_symbol.begin(), ss0_occasion.start_symbol.end());
      } else if (ss_cfg.get_id() != to_search_space_id(0) and ss_cfg.get_coreset_id() == common_coreset->get_id()) {
        cs_duration         = common_coreset->duration();
        ss_start_symbol_idx = ss_cfg.get_first_symbol_index();
      }
    }

    if (tdd_cfg.pattern1.nof_dl_symbols > 0) {
      // Ensuring there is enough DL symbols for PDCCH.
      CHECK_EQ_OR_ABOVE(tdd_cfg.pattern1.nof_dl_symbols,
                        ss_start_symbol_idx + cs_duration.value(),
                        "TDD UL DL pattern 1 configuration. DL(symbols) configuration is less than CORESET symbols[{}, "
                        "{}) for SearchSpace#{}.",
                        ss_start_symbol_idx,
                        ss_start_symbol_idx + cs_duration.value(),
                        fmt::underlying(ss_cfg.get_id()));

      // Ensuring there is enough DL symbols for PDSCH.
      CHECK_EQ_OR_ABOVE(tdd_cfg.pattern1.nof_dl_symbols,
                        ss_start_symbol_idx + cs_duration.value() + pdsch_mapping_typeA_min_L_value,
                        "TDD UL DL pattern 1 configuration. DL(symbols) configuration is less than the minimum nof. "
                        "OFDM symbols required for PDSCH allocation of mapping typeA in SearchSpace#{}.",
                        fmt::underlying(ss_cfg.get_id()));
    }

    if (tdd_cfg.pattern2.has_value() and tdd_cfg.pattern2.value().nof_dl_symbols > 0) {
      // Ensuring there is enough DL symbols for PDCCH.
      CHECK_EQ_OR_ABOVE(tdd_cfg.pattern2.value().nof_dl_symbols,
                        ss_start_symbol_idx + cs_duration.value(),
                        "TDD UL DL pattern 2 configuration. DL(symbols) configuration is less than CORESET symbols[{}, "
                        "{}) for SearchSpace#{}.",
                        ss_start_symbol_idx,
                        ss_start_symbol_idx + cs_duration.value(),
                        fmt::underlying(ss_cfg.get_id()));

      // Ensuring there is enough DL symbols for PDSCH.
      CHECK_EQ_OR_ABOVE(tdd_cfg.pattern2.value().nof_dl_symbols,
                        ss_start_symbol_idx + cs_duration.value() + pdsch_mapping_typeA_min_L_value,
                        "TDD UL DL pattern 2 configuration. DL(symbols) configuration is less than the minimum nof. "
                        "OFDM symbols required for PDSCH allocation of mapping typeA in SearchSpace#{}.",
                        fmt::underlying(ss_cfg.get_id()));
    }
  }
  // SearchSpaces in Dedicated PDCCH configuration.
  for (const search_space_configuration& ss_cfg : ded_pdcch_cfg.search_spaces) {
    cs_duration         = {};
    ss_start_symbol_idx = ss_cfg.get_first_symbol_index();
    if (coreset0.has_value()) {
      if (ss_cfg.get_coreset_id() == coreset0->get_id()) {
        cs_duration = coreset0->duration();
      }
    }
    if ((not cs_duration.has_value()) and common_coreset.has_value()) {
      if (ss_cfg.get_coreset_id() == common_coreset->get_id()) {
        cs_duration = common_coreset->duration();
      }
    }

    if (not cs_duration.has_value()) {
      for (const coreset_configuration& cs_cfg : ded_pdcch_cfg.coresets) {
        if (ss_cfg.get_coreset_id() == cs_cfg.get_id()) {
          cs_duration = cs_cfg.duration();
          break;
        }
      }
    }

    CHECK_TRUE(cs_duration.has_value(), "CORESET not configured for SearchSpace#{}", fmt::underlying(ss_cfg.get_id()));

    if (tdd_cfg.pattern1.nof_dl_symbols > 0) {
      // Ensuring there is atleast 1 OFDM symbol for PDSCH.
      CHECK_ABOVE(tdd_cfg.pattern1.nof_dl_symbols,
                  ss_start_symbol_idx + cs_duration.value(),
                  "TDD UL DL pattern 1 configuration. DL(symbols) configuration is less than CORESET symbols[{}, "
                  "{}) for SearchSpace#{}.",
                  ss_start_symbol_idx,
                  ss_start_symbol_idx + cs_duration.value(),
                  fmt::underlying(ss_cfg.get_id()));

      // Ensuring there is enough DL symbols for PDSCH.
      CHECK_EQ_OR_ABOVE(tdd_cfg.pattern1.nof_dl_symbols,
                        ss_start_symbol_idx + cs_duration.value() + pdsch_mapping_typeA_min_L_value,
                        "TDD UL DL pattern 1 configuration. DL(symbols) configuration is less than the minimum nof. "
                        "OFDM symbols required for PDSCH allocation of mapping typeA in SearchSpace#{}.",
                        fmt::underlying(ss_cfg.get_id()));
    }

    if (tdd_cfg.pattern2.has_value() and tdd_cfg.pattern2.value().nof_dl_symbols > 0) {
      // Ensuring there is atleast 1 OFDM symbol for PDSCH.
      CHECK_ABOVE(tdd_cfg.pattern2.value().nof_dl_symbols,
                  ss_start_symbol_idx + cs_duration.value(),
                  "TDD UL DL pattern 2 configuration. DL(symbols) configuration is less than CORESET symbols[{}, "
                  "{}) for SearchSpace#{}.",
                  ss_start_symbol_idx,
                  ss_start_symbol_idx + cs_duration.value(),
                  fmt::underlying(ss_cfg.get_id()));

      // Ensuring there is enough DL symbols for PDSCH.
      CHECK_EQ_OR_ABOVE(tdd_cfg.pattern2.value().nof_dl_symbols,
                        ss_start_symbol_idx + cs_duration.value() + pdsch_mapping_typeA_min_L_value,
                        "TDD UL DL pattern 2 configuration. DL(symbols) configuration is less than the minimum nof. "
                        "OFDM symbols required for PDSCH allocation of mapping typeA in SearchSpace#{}.",
                        fmt::underlying(ss_cfg.get_id()));
    }
  }

  return {};
}

static check_outcome check_prach_config(const du_cell_config& cell_cfg)
{
  CHECK_TRUE(cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common.has_value(),
             "Rach config common not present in UL BWP");

  const rach_config_common& rach_cfg = cell_cfg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common.value();

  const auto prach_cfg = prach_configuration_get(band_helper::get_freq_range(cell_cfg.ran.dl_carrier.band),
                                                 band_helper::get_duplex_mode(cell_cfg.ran.dl_carrier.band),
                                                 rach_cfg.rach_cfg_generic.prach_config_index);
  CHECK_NEQ(fmt::underlying(prach_cfg.format),
            fmt::underlying(ocudu::prach_format_type::invalid),
            "The PRACH format is invalid");

  // Derive PRACH duration information.
  // The parameter \c is_last_prach_occasion is arbitrarily set to false, as it doesn't affect the PRACH number of PRBs.
  constexpr bool                   is_last_prach_occasion = false;
  const prach_preamble_information info =
      is_long_preamble(prach_cfg.format)
          ? get_prach_preamble_long_info(prach_cfg.format)
          : get_prach_preamble_short_info(
                prach_cfg.format,
                to_ra_subcarrier_spacing(cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.scs),
                is_last_prach_occasion);
  const unsigned prach_nof_prbs =
      prach_frequency_mapping_get(info.scs, cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.scs).nof_rb_ra;

  const uint8_t prach_prb_stop =
      rach_cfg.rach_cfg_generic.msg1_frequency_start + rach_cfg.rach_cfg_generic.msg1_fdm * prach_nof_prbs;

  const unsigned bwp_size                 = cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  const unsigned dedicated_pucch_rb_start = config_helpers::get_dedicated_pucch_rb_start(
      cell_cfg.ran.init_bwp.pucch.resources,
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common,
      bwp_size);
  prb_interval prb_interval_no_pucch = config_helpers::find_largest_prb_interval_without_pucch(
      cell_cfg.ran.init_bwp.pucch.resources, bwp_size, dedicated_pucch_rb_start);

  // This is to preserve a guardband between the PUCCH and PRACH.
  const unsigned pucch_to_prach_guardband = is_long_preamble(prach_cfg.format) ? 0U : 3U;

  CHECK_TRUE(prach_prb_stop + pucch_to_prach_guardband <= prb_interval_no_pucch.stop(),
             "With the given prach_frequency_start={}, the PRACH opportunities in prbs=[{}, {}) overlap with the PUCCH "
             "resources/guardband in PRBs=[{}, {})",
             rach_cfg.rach_cfg_generic.msg1_frequency_start,
             rach_cfg.rach_cfg_generic.msg1_frequency_start,
             prach_prb_stop,
             prb_interval_no_pucch.stop() - pucch_to_prach_guardband,
             cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.crbs.length());

  return {};
}

/// Fails if any dedicated PUCCH resource overlaps the PRBs reserved at the BWP edges, if the number of edge PRBs is
/// set. Otherwise, fails if any dedicated PUCCH resource overlaps the common PUCCH PRBs.
static check_outcome check_pucch_common_ded_overlap(const du_cell_config& cell_cfg)
{
  const auto&    pucch_params     = cell_cfg.ran.init_bwp.pucch.resources;
  const unsigned bwp_size         = cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  const unsigned pucch_res_common = cell_cfg.ran.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common;

  // With nof_edge_prbs set, the dedicated resources can overlap the common resources, but not the edge PRBs.
  // Without it, check the common PRBs directly. Row 15 puts the common resources away from the BWP edges, where many
  // dedicated resources can reach them.
  crb_bitmap  reserved_prbs;
  std::string reserved_prbs_desc;
  if (pucch_params.nof_edge_prbs.has_value()) {
    const unsigned nof_edge_prbs = *pucch_params.nof_edge_prbs;
    CHECK_BELOW(2 * nof_edge_prbs, bwp_size, "number of PUCCH edge PRBs at both BWP edges");
    reserved_prbs.resize(bwp_size);
    reserved_prbs.fill(0, nof_edge_prbs);
    reserved_prbs.fill(bwp_size - nof_edge_prbs, bwp_size);
    reserved_prbs_desc = fmt::format("{} PRBs reserved at each BWP edge", nof_edge_prbs);
  } else {
    reserved_prbs      = compute_pucch_crbs(crb_interval{0, bwp_size}, pucch_res_common, {});
    reserved_prbs_desc = fmt::format("common PUCCH resources of pucch-ResourceCommon={}", pucch_res_common);
  }

  const std::vector<pucch_resource> ded_res = config_helpers::generate_cell_pucch_res_list(
      pucch_params, bwp_size, config_helpers::get_dedicated_pucch_rb_start(pucch_params, pucch_res_common, bwp_size));
  for (const pucch_resource& res : ded_res) {
    const prb_interval hop1 = res.prbs();
    const prb_interval hop2 =
        res.second_hop_prb.has_value() ? prb_interval::start_and_len(*res.second_hop_prb, hop1.length()) : hop1;
    CHECK_TRUE(not reserved_prbs.any(hop1.start(), hop1.stop()) and not reserved_prbs.any(hop2.start(), hop2.stop()),
               "The dedicated PUCCH resource cell_res_id={} in PRBs={} (second hop {}) overlaps the {}",
               res.res_id.ded().cell_res_id,
               hop1,
               hop2,
               reserved_prbs_desc);
  }
  return {};
}

/// Fails if the dedicated PUCCH resources overlap the SRS symbols. Warns if a manually configured SRS bandwidth
/// overlaps with the common PUCCH resources, which the scheduler tolerates.
static check_outcome check_srs_config(const du_cell_config& cell_cfg)
{
  const auto& srs_cfg = cell_cfg.ran.init_bwp.srs_cfg;
  if (srs_cfg.srs_type_enabled == srs_type::disabled) {
    return {};
  }

  // The dedicated PUCCH resources are placed starting from the first symbol of the slot, so their symbol budget must
  // leave the last symbols of the slot free for the SRS. Otherwise, the PUCCH resources at the BWP edges overlap the
  // SRS RBs, and the two are transmitted on top of each other.
  const unsigned max_nof_pucch_symbols = config_helpers::compute_max_nof_pucch_symbols(srs_cfg).value();
  CHECK_TRUE(cell_cfg.ran.init_bwp.pucch.resources.max_nof_symbols.value() <= max_nof_pucch_symbols,
             "The cell PUCCH resources span up to {} symbols, which overlaps the last {} symbols of the slot reserved "
             "for the SRS. The PUCCH resources must be capped to {} symbols",
             cell_cfg.ran.init_bwp.pucch.resources.max_nof_symbols,
             srs_cfg.max_nof_symbols,
             max_nof_pucch_symbols);

  if (not srs_cfg.c_srs.has_value()) {
    return {};
  }

  const crb_interval srs_avail_crbs = compute_available_crbs_without_common_pucch(
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.crbs,
      cell_cfg.ran.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common);
  constexpr uint8_t                      b_srs_0    = 0;
  const std::optional<srs_configuration> srs_bw_cfg = srs_configuration_get(srs_cfg.c_srs.value(), b_srs_0);
  ocudu_assert(srs_bw_cfg.has_value(), "Invalid SRS bandwidth configuration");
  const crb_interval srs_crbs{srs_cfg.freq_domain_shift.value(),
                              srs_cfg.freq_domain_shift.value() + srs_bw_cfg.value().m_srs};
  if (not srs_avail_crbs.contains(srs_crbs)) {
    ocudulog::fetch_basic_logger("DU-MNG").warning(
        "The manually configured SRS bandwidth crbs={} overlaps with the common PUCCH resources; this may starve "
        "the common PUCCH resources of RBs. Consider restricting the SRS to crbs={}",
        srs_crbs,
        srs_avail_crbs);
  }

  return {};
}

/// Validates the mapping of SIBs to SI messages.
static check_outcome check_si_sched_config(const du_cell_config& cell_cfg)
{
  if (not cell_cfg.si.si_config.has_value()) {
    return {};
  }

  for (const si_message_sched_info& si_msg : cell_cfg.si.si_config->si_sched_info) {
    // An SI message carrying a warning has parameters of its own, and takes a position in the schedulingInfoList only
    // while the warning is on air, so it is no part of the SI scheduling info.
    const bool has_pws = std::any_of(si_msg.sib_mapping_info.begin(), si_msg.sib_mapping_info.end(), is_pws_sib);
    CHECK_TRUE(not has_pws, "SIB6/7/8 cannot be mapped to an SI message of the SI scheduling info");
  }

  return {};
}

/// Validates NTN cell configuration parameters.
static check_outcome check_ntn_config(const du_cell_config& cell_cfg)
{
  if (!cell_cfg.ran.ntn_params.has_value()) {
    return {}; // NTN not configured, skip validation
  }

  const auto& ntn = cell_cfg.ran.ntn_params.value().ntn_cfg;

  // Validate cell_specific_koffset (required for NTN).
  if (ntn.cell_specific_koffset.has_value()) {
    CHECK_EQ_OR_ABOVE(ntn.cell_specific_koffset.value().count(), 1, "cell_specific_koffset");
    CHECK_EQ_OR_BELOW(
        ntn.cell_specific_koffset.value().count(), NTN_CELL_SPECIFIC_KOFFSET_MAX, "cell_specific_koffset");
  }

  if (ntn.k_mac.has_value()) {
    CHECK_EQ_OR_ABOVE(ntn.k_mac.value().count(), 1, "k_mac");
    CHECK_EQ_OR_BELOW(ntn.k_mac.value().count(), NTN_K_MAC_MAX, "k_mac");
  }

  // ta_common and ta_common_offset are summed into the single taCommon-r17 field
  // (see make_asn1_rrc_cell_ntn_cfg), constrained to 0..66485757, i.e. <= 270730 us.
  // The two are bounded individually at the CLI but their sum is not.
  if (ntn.ta_info.has_value()) {
    const double ta_common_total = ntn.ta_info->ta_common + ntn.ta_info->ta_common_offset.value_or(0.0);
    CHECK_EQ_OR_ABOVE(ta_common_total, 0.0, "ta_common + ta_common_offset");
    CHECK_EQ_OR_BELOW(ta_common_total, 270730.0, "ta_common + ta_common_offset");
  }

  if (ntn.ntn_ul_sync_validity_dur.has_value()) {
    const bool valid_sync_dur = is_valid_enum_number<asn1::rrc_nr::ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_e_>(
        ntn.ntn_ul_sync_validity_dur.value());
    CHECK_TRUE(valid_sync_dur, "Invalid ntn_ul_sync_validity_dur value={}", ntn.ntn_ul_sync_validity_dur.value());
  }

  if (ntn.epoch_time.has_value()) {
    CHECK_EQ_OR_BELOW(ntn.epoch_time->sfn, 1023, "epoch_time.sfn");
    CHECK_EQ_OR_BELOW(ntn.epoch_time->subframe_number, 9, "epoch_time.subframe_number");
  }

  return {};
}

static check_outcome check_tac_list(const du_cell_config& cell_cfg)
{
  if (cell_cfg.tac_list.empty()) {
    // TN cell: SIB1 broadcasts trackingAreaCode.
    return {};
  }

  // A single-entry list carries no more than trackingAreaCode. The first entry mirrors du_cell_config::tac.
  CHECK_EQ_OR_ABOVE(cell_cfg.tac_list.size(), 2, "TAC list size");
  CHECK_EQ_OR_BELOW(cell_cfg.tac_list.size(), MAX_NOF_TACS_NTN, "TAC list size");
  CHECK_EQ(cell_cfg.tac_list.front(), cell_cfg.tac, "first entry of the TAC list");

  for (unsigned i = 0, e = cell_cfg.tac_list.size(); i != e; ++i) {
    CHECK_TRUE(is_valid(cell_cfg.tac_list[i]), "Invalid TAC {} in the TAC list", cell_cfg.tac_list[i]);
    for (unsigned j = i + 1; j != e; ++j) {
      CHECK_NEQ(cell_cfg.tac_list[j], cell_cfg.tac_list[i], "duplicate TAC in the TAC list");
    }
  }

  return {};
}

/// Determines whether the given value belongs to a list of valid values.
template <typename T, size_t N>
static bool is_one_of(unsigned value, const std::array<T, N>& valid_values)
{
  return std::find(valid_values.begin(), valid_values.end(), value) != valid_values.end();
}

static check_outcome check_prs_resource_set(const prs_resource_set&                       res_set,
                                            unsigned                                      set_id,
                                            const scs_specific_carrier&                   dl_carrier,
                                            const std::optional<tdd_ul_dl_config_common>& tdd_cfg)
{
  const unsigned comb_size         = static_cast<unsigned>(res_set.comb_size);
  const unsigned nof_symbols       = static_cast<unsigned>(res_set.nof_symbols);
  const unsigned repetition_factor = static_cast<unsigned>(res_set.repetition_factor);
  const unsigned time_gap          = static_cast<unsigned>(res_set.time_gap);
  const unsigned mu                = to_numerology_value(dl_carrier.scs);

  CHECK_TRUE(is_one_of(comb_size, prs_constants::VALID_COMB_SIZES),
             "Invalid comb size ({}) of PRS resource set {}",
             comb_size,
             set_id);
  CHECK_TRUE(is_one_of(nof_symbols, prs_constants::VALID_NOF_SYMBOLS),
             "Invalid number of symbols ({}) of PRS resource set {}",
             nof_symbols,
             set_id);
  CHECK_TRUE(is_one_of(repetition_factor, prs_constants::VALID_REPETITION_FACTORS),
             "Invalid repetition factor ({}) of PRS resource set {}",
             repetition_factor,
             set_id);
  CHECK_TRUE(is_one_of(time_gap, prs_constants::VALID_TIME_GAPS),
             "Invalid time gap ({} slots) of PRS resource set {}",
             time_gap,
             set_id);
  CHECK_TRUE(prs_valid_periodicity(res_set.periodicity_slots, mu),
             "Invalid periodicity ({} slots) of PRS resource set {} for numerology {}",
             res_set.periodicity_slots,
             set_id,
             mu);

  // The valid combinations are given in TS 38.211, Section 7.4.1.7.3.
  CHECK_TRUE(prs_valid_num_symbols_and_comb_size(res_set.nof_symbols, res_set.comb_size),
             "Invalid number of symbols ({}) and comb size ({}) combination of PRS resource set {}. See TS 38.211, "
             "Section 7.4.1.7.3",
             nof_symbols,
             comb_size,
             set_id);

  CHECK_TRUE(res_set.bandwidth_prbs % prs_constants::PRB_GRANULARITY == 0,
             "Invalid bandwidth ({} PRBs) of PRS resource set {}. It must be a multiple of {}",
             res_set.bandwidth_prbs,
             set_id,
             prs_constants::PRB_GRANULARITY);
  CHECK_EQ_OR_ABOVE(
      res_set.bandwidth_prbs, prs_constants::MIN_PRBS, "bandwidth, in PRBs, of PRS resource set {}", set_id);
  CHECK_EQ_OR_BELOW(
      res_set.bandwidth_prbs, prs_constants::MAX_PRBS, "bandwidth, in PRBs, of PRS resource set {}", set_id);
  CHECK_EQ_OR_ABOVE(res_set.power_offset_db,
                    prs_constants::MIN_POWER_OFFSET_DB,
                    "power offset, in dB, of PRS resource set {}",
                    set_id);
  CHECK_EQ_OR_BELOW(res_set.power_offset_db,
                    prs_constants::MAX_POWER_OFFSET_DB,
                    "power offset, in dB, of PRS resource set {}",
                    set_id);

  // The start PRB of the resource set is relative to Point A, as is the offset of the DL carrier.
  CHECK_EQ_OR_BELOW(res_set.start_prb, prs_constants::MAX_START_PRB, "start PRB of PRS resource set {}", set_id);
  CHECK_EQ_OR_ABOVE(res_set.start_prb, dl_carrier.offset_to_carrier, "start PRB of PRS resource set {}", set_id);
  CHECK_EQ_OR_BELOW(res_set.start_prb + res_set.bandwidth_prbs,
                    dl_carrier.offset_to_carrier + dl_carrier.carrier_bandwidth,
                    "last PRB of PRS resource set {}",
                    set_id);

  CHECK_BELOW(res_set.slot_offset, res_set.periodicity_slots, "slot offset of PRS resource set {}", set_id);

  // As per TS 38.214, Section 5.1.6.5, all the repetitions of a PRS resource must fit within one period.
  CHECK_EQ_OR_BELOW(repetition_factor * time_gap,
                    res_set.periodicity_slots,
                    "product of the repetition factor and the time gap, in slots, of PRS resource set {}",
                    set_id);

  // [Implementation-defined] Only PRS periodicities that are a multiple of the TDD period are supported, so that the
  // PRS occasions always fall in the same slots of the TDD pattern.
  if (tdd_cfg.has_value()) {
    const unsigned tdd_period_slots = nof_slots_per_tdd_period(tdd_cfg.value());
    CHECK_TRUE(res_set.periodicity_slots % tdd_period_slots == 0,
               "Invalid periodicity ({} slots) of PRS resource set {}. In TDD mode, it must be a multiple of the TDD "
               "period ({} slots)",
               res_set.periodicity_slots,
               set_id,
               tdd_period_slots);
  }

  if (res_set.muting_option1.has_value()) {
    const prs_muting_option1& muting_opt1           = res_set.muting_option1.value();
    const unsigned            muting_pattern_size   = muting_opt1.muting_pattern.size();
    const unsigned            muting_bit_rep_factor = static_cast<unsigned>(muting_opt1.muting_bit_repetition_factor);

    CHECK_TRUE(is_one_of(muting_pattern_size, prs_constants::VALID_MUTING_PATTERN_SIZES),
               "Invalid muting pattern size ({} bits) of Muting Option 1 of PRS resource set {}",
               muting_pattern_size,
               set_id);

    // As per TS 38.214, Section 5.1.6.5, the UE does not expect the product of the periodicity, the muting bit
    // repetition factor and the muting pattern size of Muting Option 1 to exceed 2^mu x 10240.
    CHECK_EQ_OR_BELOW(res_set.periodicity_slots * muting_bit_rep_factor * muting_pattern_size,
                      (1U << mu) * prs_constants::MAX_MUTING_OPTION1_PRODUCT_NUMEROLOGY_0,
                      "product of the periodicity, the muting bit repetition factor and the muting pattern size of "
                      "Muting Option 1 of PRS resource set {}",
                      set_id);

    CHECK_TRUE(res_set.muting_option1->muting_pattern.any(),
               "Every instance of PRS resource set {} is disabled by Muting Option 1",
               set_id);
  }

  if (res_set.muting_option2.has_value()) {
    const unsigned muting_pattern_size = res_set.muting_option2.value().muting_pattern.size();

    CHECK_EQ(muting_pattern_size,
             repetition_factor,
             "muting pattern size of Muting Option 2 of PRS resource set {}. It must be equal to the resource "
             "repetition factor",
             set_id);

    CHECK_TRUE(res_set.muting_option2->muting_pattern.any(),
               "Every repetition in PRS resource set {} is disabled by Muting Option 2.",
               set_id);
  }

  CHECK_TRUE(not res_set.resources.empty(), "No PRS resource configured in PRS resource set {}", set_id);
  CHECK_EQ_OR_BELOW(res_set.resources.size(),
                    prs_constants::MAX_NOF_RESOURCES_PER_SET,
                    "number of PRS resources of PRS resource set {}",
                    set_id);

  for (unsigned res_id = 0, nof_res = res_set.resources.size(); res_id != nof_res; ++res_id) {
    const prs_resource& res = res_set.resources[res_id];

    CHECK_EQ_OR_BELOW(res.sequence_id,
                      prs_constants::MAX_SEQUENCE_ID,
                      "sequence ID of PRS resource {} of resource set {}",
                      res_id,
                      set_id);
    CHECK_BELOW(res.re_offset, comb_size, "RE offset of PRS resource {} of resource set {}", res_id, set_id);
    CHECK_EQ_OR_BELOW(res.slot_offset,
                      prs_constants::MAX_RES_SLOT_OFFSET,
                      "slot offset of PRS resource {} of resource set {}",
                      res_id,
                      set_id);
    CHECK_EQ_OR_BELOW(res.symbol_offset + nof_symbols,
                      get_nsymb_per_slot(cyclic_prefix::NORMAL),
                      "last symbol of PRS resource {} of resource set {}",
                      res_id,
                      set_id);

    // Slot offset of the last repetition of the resource, relative to the beginning of the period.
    const unsigned last_rep_slot_offset = res_set.slot_offset + res.slot_offset + (repetition_factor - 1) * time_gap;
    CHECK_BELOW(last_rep_slot_offset,
                res_set.periodicity_slots,
                "slot offset of the last repetition of PRS resource {} of resource set {}",
                res_id,
                set_id);
  }

  // Two resources of the same set that share the slot offset, the symbol offset and the comb offset are mapped onto
  // exactly the same REs, as all the resources of a set have the same comb size and number of symbols.
  //
  // Note that resources with different symbol offsets are not necessarily in conflict, even if their symbols overlap:
  // the comb offset hops with the symbol index within the resource, as per TS 38.211, Table 7.4.1.7.3-1.
  CHECK_TRUE(has_unique_ids(res_set.resources,
                            [](const prs_resource& res) {
                              return std::make_tuple(res.slot_offset, res.symbol_offset, res.re_offset);
                            }),
             "Two PRS resources of resource set {} share the slot offset, the symbol offset and the RE offset, so they "
             "are mapped onto the same REs",
             set_id);

  return {};
}

static check_outcome check_prs_config(const du_cell_config& cell_cfg)
{
  const prs_config& prs_cfg = cell_cfg.ran.prs_cfg;

  // DL-PRS is disabled when no resource set is configured.
  if (prs_cfg.resource_sets.empty()) {
    return {};
  }

  CHECK_EQ_OR_BELOW(prs_cfg.resource_sets.size(), prs_constants::MAX_NOF_RESOURCE_SETS, "number of PRS resource sets");

  // DL-PRS is transmitted in the DL carrier of the cell, with the SCS common.
  const subcarrier_spacing scs        = cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.scs;
  const auto&              carriers   = cell_cfg.ran.dl_cfg_common.freq_info_dl.scs_carrier_list;
  const auto               dl_carrier = std::find_if(
      carriers.begin(), carriers.end(), [scs](const scs_specific_carrier& carrier) { return carrier.scs == scs; });
  CHECK_TRUE(dl_carrier != carriers.end(), "No DL carrier configured for the SCS common, required to transmit DL-PRS");

  for (unsigned set_id = 0, nof_sets = prs_cfg.resource_sets.size(); set_id != nof_sets; ++set_id) {
    HANDLE_ERROR(check_prs_resource_set(prs_cfg.resource_sets[set_id], set_id, *dl_carrier, cell_cfg.ran.tdd_cfg));
  }

  return {};
}

static check_outcome check_redcap_config(const du_cell_config& cell_cfg)
{
  const auto& ran = cell_cfg.ran;
  CHECK_TRUE(band_helper::get_freq_range(ran.dl_carrier.band) == frequency_range::FR1 and
                 band_helper::get_freq_range(ran.ul_carrier.band) == frequency_range::FR1,
             "RedCap requires FR1");
  CHECK_TRUE(ran.dl_carrier.carrier_bw <= bs_channel_bandwidth::MHz20 and
                 ran.ul_carrier.carrier_bw <= bs_channel_bandwidth::MHz20,
             "RedCap requires DL and UL carriers of at most 20 MHz");
  const auto& dl = ran.dl_cfg_common.init_dl_bwp.generic_params;
  const auto& ul = ran.ul_cfg_common.init_ul_bwp.generic_params;
  CHECK_TRUE(
      dl.crbs.length() <= band_helper::get_n_rbs_from_bw(bs_channel_bandwidth::MHz20, dl.scs, frequency_range::FR1) and
          ul.crbs.length() <= band_helper::get_n_rbs_from_bw(bs_channel_bandwidth::MHz20, ul.scs, frequency_range::FR1),
      "RedCap requires initial DL and UL BWPs of at most 20 MHz");
  CHECK_TRUE(not band_helper::is_ntn_band(ran.dl_carrier.band) and not band_helper::is_ntn_band(ran.ul_carrier.band) and
                 not ran.ntn_params.has_value() and not ran.init_bwp.cg_cfg.has_value(),
             "RedCap does not support NTN or configured grants");
  return {};
}

check_outcome odu::is_du_cell_config_valid(const du_cell_config& cell_cfg)
{
  CHECK_EQ_OR_BELOW(cell_cfg.ran.pci, MAX_PCI, "cell PCI");
  CHECK_EQ_OR_BELOW(fmt::underlying(cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.scs),
                    fmt::underlying(subcarrier_spacing::kHz120),
                    "SCS common");
  HANDLE_ERROR(is_coreset0_ss0_idx_valid(cell_cfg));
  HANDLE_ERROR(check_dl_config_common(cell_cfg));
  HANDLE_ERROR(check_ul_config_common(cell_cfg));
  HANDLE_ERROR(check_ssb_configuration(cell_cfg));
  HANDLE_ERROR(check_tdd_ul_dl_config(cell_cfg));
  const unsigned ul_bwp_size = cell_cfg.ran.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  HANDLE_ERROR(config_helpers::pucch_parameters_validator(
      cell_cfg.ran.init_bwp.pucch.resources,
      ul_bwp_size,
      config_helpers::get_dedicated_pucch_rb_start(
          cell_cfg.ran.init_bwp.pucch.resources,
          cell_cfg.ran.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common,
          ul_bwp_size)));
  HANDLE_ERROR(check_pucch_common_ded_overlap(cell_cfg));
  HANDLE_ERROR(check_prach_config(cell_cfg));
  HANDLE_ERROR(check_srs_config(cell_cfg));
  const serving_cell_config ue_serv_cell_cfg = config_helpers::make_default_ue_cell_config(cell_cfg.ran).serv_cell_cfg;
  HANDLE_ERROR(
      config_validators::validate_csi_meas_cfg(ue_serv_cell_cfg, cell_cfg.ran.tdd_cfg, cell_cfg.ran.ul_cfg_common));
  HANDLE_ERROR(check_dl_config_dedicated(cell_cfg));
  HANDLE_ERROR(check_ul_config_dedicated(cell_cfg));
  HANDLE_ERROR(check_si_sched_config(cell_cfg));
  HANDLE_ERROR(check_ntn_config(cell_cfg));
  HANDLE_ERROR(check_tac_list(cell_cfg));
  HANDLE_ERROR(check_prs_config(cell_cfg));
  HANDLE_ERROR(check_periodic_resource_collisions(cell_cfg.ran));
  if (cell_cfg.redcap.enabled) {
    HANDLE_ERROR(check_redcap_config(cell_cfg));
  }
  // TODO: Remaining.
  return {};
}
