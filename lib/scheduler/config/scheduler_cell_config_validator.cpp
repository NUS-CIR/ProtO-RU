// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/config/scheduler_cell_config_validator.h"
#include "../support/dmrs_helpers.h"
#include "../support/pdcch/search_space_helper.h"
#include "../support/pdsch/pdsch_default_time_allocation.h"
#include "../support/prbs_calculator.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/csi_report/csi_report_config_helpers.h"
#include "ocudu/ran/csi_report/csi_report_on_pucch_helpers.h"
#include "ocudu/ran/duplex_mode.h"
#include "ocudu/ran/prach/prach_configuration.h"
#include "ocudu/ran/prach/prach_helper.h"
#include "ocudu/ran/prach/prach_time_mapping.h"
#include "ocudu/ran/tdd/tdd_ul_dl_config.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include "ocudu/scheduler/config/serving_cell_config_validator.h"
#include "ocudu/scheduler/config/time_domain_resource_helper.h"
#include "ocudu/scheduler/sched_consts.h"

using namespace ocudu;
using namespace config_validators;

#define VERIFY(cond, ...)                                                                                              \
  if (not(cond)) {                                                                                                     \
    return make_unexpected(fmt::format(__VA_ARGS__));                                                                  \
  }
#define HANDLE_CODE(cond)                                                                                              \
  {                                                                                                                    \
    auto ret = cond;                                                                                                   \
    if (not ret.has_value()) {                                                                                         \
      return ret;                                                                                                      \
    }                                                                                                                  \
  }

static error_type<std::string> validate_pdcch_cfg_common(const sched_cell_configuration_request_message& msg)
{
  for (const auto& ss : msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces) {
    bool cset_id_exits_in_common =
        msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.common_coreset.has_value()
            ? ss.get_coreset_id() == msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.common_coreset->get_id()
            : false;
    bool cset_id_exits_in_cset0 =
        msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0.has_value() ? ss.get_coreset_id() == 0 : false;
    VERIFY(cset_id_exits_in_common or cset_id_exits_in_cset0,
           "Coreset Id. {} indexed by SearchSpace Id. {} not found within the configured Common Coresets",
           fmt::underlying(ss.get_coreset_id()),
           fmt::underlying(ss.get_id()));
  }

  return {};
}

static error_type<std::string> validate_rach_cfg_common(const sched_cell_configuration_request_message& msg,
                                                        const scheduler_expert_config&                  expert_cfg)
{
  if (not msg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common.has_value()) {
    return {};
  }
  const rach_config_common& rach_cfg_cmn = msg.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common.value();

  static constexpr pdsch_mcs_table mcs_table = ocudu::pdsch_mcs_table::qam64;
  const sch_mcs_description        mcs_descr = pdsch_mcs_get_config(mcs_table, expert_cfg.ra.rar_mcs_index);
  // See TS 38.214, 5.1.3.1, Modulation order and target code rate determination.
  VERIFY((unsigned)mcs_descr.modulation < (unsigned)modulation_scheme::QAM64,
         "Modulation order for PDSCH scheduled with RA-RNTI cannot be > 2");

  frequency_range freq_range = band_helper::get_freq_range(msg.ran.dl_carrier.band);
  duplex_mode     dplx_mode  = band_helper::get_duplex_mode(msg.ran.dl_carrier.band);

  // Check PRACH config index.
  auto code = prach_helper::prach_config_index_is_valid(
      rach_cfg_cmn.rach_cfg_generic.prach_config_index, freq_range, dplx_mode);
  if (not code.has_value()) {
    return code;
  }

  const prach_configuration prach_cfg =
      prach_configuration_get(freq_range, dplx_mode, rach_cfg_cmn.rach_cfg_generic.prach_config_index);
  VERIFY(prach_cfg.format < prach_format_type::invalid, "Invalid PRACH format");

  // Check PRACH root sequence index.
  code = prach_helper::prach_root_sequence_index_is_valid(rach_cfg_cmn.prach_root_seq_index, prach_cfg.format);
  if (not code.has_value()) {
    return code;
  }

  subcarrier_spacing pusch_scs = msg.ran.ul_cfg_common.init_ul_bwp.generic_params.scs;

  // Check if the PRACH preambles fall into UL slots
  if (msg.ran.tdd_cfg.has_value()) {
    auto ret = prach_helper::prach_fits_in_tdd_pattern(
        pusch_scs, rach_cfg_cmn.rach_cfg_generic.prach_config_index, *msg.ran.tdd_cfg);
    if (not ret.has_value()) {
      std::string s = fmt::format("PRACH configuration index {} not supported with current TDD pattern.",
                                  rach_cfg_cmn.rach_cfg_generic.prach_config_index);
      if (ret.error().empty()) {
        return make_unexpected(s + fmt::format(" Cause: PRACH configuration is not valid"));
      } else {
        return make_unexpected(
            s + fmt::format(" Cause: Slot indices used for PRACH {} fall outside TDD UL slots", ret.error()));
      }
    }
  }

  // Check zero correlation zone validity.
  code = prach_helper::zero_correlation_zone_is_valid(rach_cfg_cmn.rach_cfg_generic.zero_correlation_zone_config,
                                                      rach_cfg_cmn.rach_cfg_generic.prach_config_index,
                                                      freq_range,
                                                      dplx_mode);
  if (not code.has_value()) {
    return code;
  }

  // Check that the MsgA PUSCH slot falls in a UL slot for TDD cells.
  if (rach_cfg_cmn.two_step_rach_cfg.has_value() and msg.ran.tdd_cfg.has_value()) {
    const uint8_t                             td_offset  = rach_cfg_cmn.two_step_rach_cfg->pusch.td_offset;
    const unsigned                            tdd_period = nof_slots_per_tdd_period(*msg.ran.tdd_cfg);
    const prach_helper::preamble_slot_mapping preamble_td_mapper(
        msg.ran.dl_carrier.band, pusch_scs, rach_cfg_cmn.rach_cfg_generic.prach_config_index);
    for (unsigned slot_idx = 0; slot_idx != tdd_period; ++slot_idx) {
      if (not preamble_td_mapper.has_slot_index_prach_occasion(slot_idx)) {
        continue;
      }
      const unsigned pusch_slot = (slot_idx + td_offset) % tdd_period;
      VERIFY(is_tdd_full_ul_slot(*msg.ran.tdd_cfg, pusch_slot),
             "Two-step RACH: MsgA PUSCH slot {} (PRACH slot {} + td_offset {}) is not a full UL slot",
             pusch_slot,
             slot_idx,
             td_offset);
    }
  }

  return {};
}

static error_type<std::string> validade_pusch_td_res_list(span<const pusch_time_domain_resource_allocation> pusch_lst,
                                                          const std::optional<tdd_ul_dl_config_common>&     tdd_cfg,
                                                          uint8_t                                           min_k2)
{
  using res_t = pusch_time_domain_resource_allocation;

  // Verify that PUSCH TD resource list is sorted by k2.
  bool sorted = std::is_sorted(pusch_lst.begin(), pusch_lst.end(), [](const res_t& l, const res_t& r) {
    return l.k2 < r.k2 or (l.k2 == r.k2 and l.symbols.length() >= r.symbols.length());
  });
  VERIFY(sorted, "List of PUSCH TD resources should be sorted by k2 values");

  // The list needs to be a subset of the auto-generated based on the TDD pattern and min k2.
  const auto superset_lst =
      time_domain_resource_helper::generate_dedicated_pusch_td_res_list(tdd_cfg, cyclic_prefix::NORMAL, min_k2);
  for (const auto& pusch : pusch_lst) {
    const auto it = std::find_if(
        superset_lst.begin(), superset_lst.end(), [&pusch](const pusch_time_domain_resource_allocation& res) {
          return res.k2 == pusch.k2 && res.map_type == pusch.map_type && res.symbols.contains(pusch.symbols);
        });
    if (it == superset_lst.end()) {
      return make_unexpected(fmt::format("Invalid PUSCH TD resource (k2={})", pusch.k2));
    }
  }

  if (tdd_cfg.has_value()) {
    const auto&    tdd          = tdd_cfg.value();
    const unsigned period_slots = nof_slots_per_tdd_period(tdd);

    // For each PUSCH slot, verify that there is a valid k2.
    unsigned                next_slot = 0;
    std::optional<unsigned> ul_slot   = find_next_tdd_full_ul_slot(tdd, next_slot);
    while (ul_slot.has_value()) {
      if (std::none_of(pusch_lst.begin(), pusch_lst.end(), [&tdd, period_slots, sl = ul_slot.value()](const auto& p) {
            unsigned slot_idx = period_slots + sl - p.k2;
            return has_active_tdd_dl_symbols(tdd, slot_idx);
          })) {
        return make_unexpected(fmt::format("Failed to find valid k2 candidate for slot={}", ul_slot.value()));
      }
      next_slot = ul_slot.value() + 1;
      ul_slot   = find_next_tdd_full_ul_slot(tdd, next_slot);
    }
  }

  return {};
}

static error_type<std::string> validate_pusch_cfg_common(const sched_cell_configuration_request_message& msg)
{
  if (not msg.ran.ul_cfg_common.init_ul_bwp.pusch_cfg_common.has_value()) {
    return {};
  }

  const auto& pusch_lst = msg.ran.ul_cfg_common.init_ul_bwp.pusch_cfg_common.value().pusch_td_alloc_list;
  HANDLE_CODE(validade_pusch_td_res_list(pusch_lst, msg.ran.tdd_cfg, msg.ran.init_bwp.pusch.min_k2));

  return {};
}

static error_type<std::string> validate_pucch_cfg_common(const sched_cell_configuration_request_message& msg)
{
  VERIFY(msg.ran.ul_cfg_common.init_ul_bwp.pucch_cfg_common.has_value(),
         "Cells without PUCCH-ConfigCommon are not supported");

  return {};
}

static error_type<std::string> validate_sib1_cfg(const sched_cell_configuration_request_message& msg,
                                                 const scheduler_expert_config&                  expert_cfg)
{
  static constexpr unsigned        nof_layers = 1;
  static constexpr unsigned        nof_oh_prb = 0;
  static constexpr pdsch_mcs_table mcs_table  = ocudu::pdsch_mcs_table::qam64;

  // TODO: Revise the value set for time_resource in case of partial slots where nof. OFDM symbols maybe be less.
  static constexpr unsigned time_resource = 0;
  const auto&               pdsch_td_res_alloc_list =
      get_si_rnti_pdsch_time_domain_list(msg.ran.dl_cfg_common.init_dl_bwp.generic_params.cp, msg.ran.dmrs_typeA_pos);
  const ofdm_symbol_range sib1_symbols = pdsch_td_res_alloc_list[time_resource].symbols;

  // Mapping type B is not supported yet, so a mapping type A resource must fit after CORESET#0 for every SSB beam.
  const auto&    pdcch_common      = msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common;
  const auto&    sib1_ss_cfg       = pdcch_common.search_spaces[pdcch_common.sib1_search_space_id];
  const unsigned coreset0_duration = pdcch_common.coreset0->duration();
  const unsigned L_max             = msg.ran.ssb_cfg.ssb_beams.get_L_max();
  for (unsigned ssb_idx = 0; ssb_idx != L_max; ++ssb_idx) {
    if (not msg.ran.ssb_cfg.ssb_beams.is_transmitted(ssb_idx)) {
      continue;
    }
    const unsigned required_start = sib1_ss_cfg.get_first_symbol_index(ssb_idx) + coreset0_duration;
    const bool     has_valid_resource =
        std::any_of(pdsch_td_res_alloc_list.begin(), pdsch_td_res_alloc_list.end(), [required_start](const auto& r) {
          return r.map_type != sch_mapping_type::typeB and r.symbols.start() >= required_start;
        });
    VERIFY(has_valid_resource,
           "No valid PDSCH time-domain resource to schedule SIB1 for SSB beam {} (PDSCH mapping type B not "
           "supported)",
           ssb_idx);
  }

  const dmrs_information dmrs_info =
      make_dmrs_info_common(pdsch_td_res_alloc_list, 0, msg.ran.pci, msg.ran.dmrs_typeA_pos);
  const sch_mcs_description mcs_descr = pdsch_mcs_get_config(mcs_table, expert_cfg.si.sib1_mcs_index);
  // See TS 38.214, 5.1.3.1, Modulation order and target code rate determination.
  VERIFY((unsigned)mcs_descr.modulation < (unsigned)modulation_scheme::QAM64,
         "Modulation order for PDSCH scheduled with SI-RNTI cannot be > 2");
  const sch_prbs_tbs sib1_prbs_tbs =
      get_nof_prbs(prbs_calculator_sch_config{msg.si_scheduling.sib1_payload_size.value(),
                                              (unsigned)sib1_symbols.length(),
                                              calculate_nof_dmrs_per_rb(dmrs_info),
                                              nof_oh_prb,
                                              mcs_descr,
                                              nof_layers});

  VERIFY(sib1_prbs_tbs.nof_prbs <= msg.ran.dl_cfg_common.init_dl_bwp.generic_params.crbs.length(),
         "Not enough initial DL BWP PRBs ({} > {}) to send SIB1, given the chosen MCS={} and SIB1 payload size={}. "
         "Consider increasing SIB1 MCS or decrease the SIB1 payload size.",
         msg.ran.dl_cfg_common.init_dl_bwp.generic_params.crbs.length(),
         sib1_prbs_tbs.nof_prbs,
         expert_cfg.si.sib1_mcs_index,
         msg.si_scheduling.sib1_payload_size);

  return {};
}

static error_type<std::string> validate_cg_cfg(const sched_cell_configuration_request_message& msg)
{
  if (not msg.ran.init_bwp.cg_cfg.has_value()) {
    return {};
  }

  // A Type 2 CG is activated by a DCI format 0_1 scrambled with the CS-RNTI, which needs a UE-specific SearchSpace
  // monitored with that format. The UE dedicated PDCCH configuration is taken from the cell one, so without such a
  // SearchSpace here, every UE given the CG would be rejected by the UE configuration validator.
  if (msg.ran.init_bwp.cg_cfg->is_type2()) {
    const auto& pdcch_cfg = msg.ran.init_bwp.pdcch_cfg;
    VERIFY(pdcch_cfg.has_value() and std::any_of(pdcch_cfg->search_spaces.begin(),
                                                 pdcch_cfg->search_spaces.end(),
                                                 [](const search_space_configuration& ss) {
                                                   return not ss.is_common_search_space() and
                                                          pdcch_helper::get_ul_dci_format(ss) == dci_ul_format::f0_1;
                                                 }),
           "Configured Grant type 2 requires a UE-specific SearchSpace monitored with DCI format 0_1, to carry the "
           "activation DCI");
  }

  // The CG PUSCH must leave the SRS region at the end of the slot free, so at least one PUSCH time domain resource has
  // to end before it. This is checked ahead of the CG configuration below, which picks that resource and cannot fall
  // back to one overlapping the SRS.
  VERIFY(config_helpers::find_cg_pusch_td_res_idx(msg.ran).has_value(),
         "No PUSCH time domain resource of the cell ends at or before symbol {}, where the SRS region starts, so a "
         "Configured Grant PUSCH would always collide with the SRS",
         config_helpers::compute_last_non_srs_symbol(msg.ran));

  // Verify that the CG parameters are consistent, i.e. the number of RBs required per UE (derived from the CG grant
  // size or bitrate and the MCS) does not exceed the maximum number of RBs reserved for CG in the cell. Otherwise, no
  // CG resources could ever be allocated to any UE.
  const cg_configuration default_cg_cfg = config_helpers::make_default_cell_cg_config(msg.ran);
  const unsigned         nof_rbs_per_ue = config_helpers::compute_nof_cg_prbs_per_ue(msg.ran, default_cg_cfg);
  const unsigned         max_cg_rbs     = msg.ran.init_bwp.cg_cfg.value().max_nof_cell_cg_rbs;
  VERIFY(nof_rbs_per_ue <= max_cg_rbs,
         "The Configured Grant size/bitrate requires {} RBs per UE at the configured MCS, which exceeds the maximum "
         "of {} RBs reserved for CG in the cell. No CG resources could be allocated to any UE",
         nof_rbs_per_ue,
         max_cg_rbs);

  return {};
}

static error_type<std::string> validate_paging_cfg(const scheduler_expert_config& expert_cfg)
{
  static constexpr pdsch_mcs_table mcs_table = ocudu::pdsch_mcs_table::qam64;
  const sch_mcs_description        mcs_descr = pdsch_mcs_get_config(mcs_table, expert_cfg.pg.paging_mcs_index);
  // See TS 38.214, 5.1.3.1, Modulation order and target code rate determination.
  VERIFY((unsigned)mcs_descr.modulation < (unsigned)modulation_scheme::QAM64,
         "Modulation order for PDSCH scheduled with P-RNTI cannot be > 2");

  return {};
}

/// \brief Validates \c sched_cell_configuration_request_message used to add a cell.
/// \param[in] msg scheduler cell configuration message to be validated.
/// \param[in] expert_cfg Expert scheduler cell configuration.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
error_type<std::string> config_validators::validate_sched_cell_configuration_request_message(
    const sched_cell_configuration_request_message& msg,
    const scheduler_expert_config&                  expert_cfg)
{
  VERIFY(msg.cell_index < MAX_NOF_DU_CELLS, "cell index={} is not valid", msg.cell_index);

  VERIFY(msg.max_nof_ue_contexts > 0 and msg.max_nof_ue_contexts <= MAX_NOF_DU_UES_PER_CELL,
         "Number of UE contexts={} of cell={} is not valid",
         msg.max_nof_ue_contexts,
         msg.cell_index);

  const auto& dl_lst = msg.ran.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list;
  for (const auto& pdsch : dl_lst) {
    VERIFY(pdsch.k0 <= SCHEDULER_MAX_K0, "k0={} value exceeds maximum supported k0", pdsch.k0);
  }

  HANDLE_CODE(validate_pdcch_cfg_common(msg));

  HANDLE_CODE(validate_rach_cfg_common(msg, expert_cfg));

  HANDLE_CODE(validate_pusch_cfg_common(msg));

  HANDLE_CODE(validate_pucch_cfg_common(msg));

  HANDLE_CODE(validate_sib1_cfg(msg, expert_cfg));

  HANDLE_CODE(validate_paging_cfg(expert_cfg));

  HANDLE_CODE(validate_cg_cfg(msg));

  if (msg.ran.init_bwp.csi.has_value()) {
    const auto csi_helper          = config_helpers::make_csi_meas_config_builder_params(msg.ran);
    const auto nzp_csi_rs_res_list = csi_helper::make_nzp_csi_rs_resource_list(csi_helper);
    HANDLE_CODE(validate_nzp_csi_rs_list(nzp_csi_rs_res_list, msg.ran.tdd_cfg));

    // A periodic CSI report is sent in the PUCCH Format 2, 3 or 4 resource of the UE.
    const auto csi_meas = csi_helper::make_csi_meas_config(
        csi_helper, msg.ran.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list);
    if (not is_pusch_configured(csi_meas)) {
      const unsigned csi_report_size =
          get_csi_report_pucch_size(create_csi_report_configuration(csi_meas)).part1_size.value();
      const unsigned max_payload = msg.ran.init_bwp.pucch.resources.max_payload_234();
      VERIFY(csi_report_size <= max_payload,
             "The periodic CSI report of {} bits exceeds the maximum PUCCH Format 2/3/4 payload of {} bits",
             csi_report_size,
             max_payload);
    }
  }

  // TODO: Validate other parameters.
  return {};
}
