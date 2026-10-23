// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/config/scheduler_ue_config_validator.h"
#include "../support/pdcch/search_space_helper.h"
#include "cell_configuration.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/rrc_nr.h"
#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_validator.h"
#include "ocudu/support/config/validator_helpers.h"

using namespace ocudu;
using namespace config_validators;

static validator_result validate_cg_cfg(const serving_cell_config& serv_cell_cfg, const cell_configuration& cell_cfg)
{
  const uplink_config& ul_cfg         = serv_cell_cfg.ul_config.value();
  const auto&          bwp_ul_ded_cfg = ul_cfg.init_ul_bwp;
  VERIFY(bwp_ul_ded_cfg.cg_cfg.has_value(), "Configured Grant configuration not set");
  const auto& cg_cfg = bwp_ul_ded_cfg.cg_cfg.value();

  VERIFY(cell_cfg.params.init_bwp.cg_cfg.has_value(),
         "UE is configured with a Configured Grant, but the cell has no Configured Grant configured");
  const auto& cell_cg_cfg = cell_cfg.params.init_bwp.cg_cfg.value();

  // Every UE of a cell must use the cell's CG period: the CG scheduler's slot wheel is sized to that period and derives
  // every UE's grant positions from it alone. The per-UE periodicity exists because it has to be signalled to the UE
  // over RRC, not because it may differ from the cell's.
  VERIFY(cg_cfg.periodicity == cell_cg_cfg.periodicity,
         "Configured Grant period of the UE ({} slots) differs from the period of the cell ({} slots)",
         static_cast<unsigned>(cg_cfg.periodicity),
         static_cast<unsigned>(cell_cg_cfg.periodicity));

  // NOTE: the actual HARQ timeout (in slots) is the configured_grant_timer multiplied by the CG period in slot.
  VERIFY(cg_configuration::configured_grant_timer <= cg_cfg.nof_harq_processes,
         "configured_grant_timer ({}) must be <= nof_harq_processes ({}) so the CG HARQ timeout expires before the "
         "next CG occasion for the same HARQ ID",
         cg_configuration::configured_grant_timer,
         cg_cfg.nof_harq_processes);

  // As per TS 38.214, Section 6.1.3, the CG PUSCH waveform is set by transformPrecoder in configuredGrantConfig, or by
  // msg3-transformPrecoder when absent. The dedicated pusch-Config transformPrecoder does not apply to it.
  const bool cg_tp_enabled = cg_cfg.trans_precoder.value_or(cell_cfg.use_msg3_transform_precoder());
  // The CG path assumes a CP-OFDM waveform (i.e., no transform precoding) throughout.
  VERIFY(not cg_tp_enabled, "Configured Grant is not supported with PUSCH transform precoding enabled");

  const bool type2 = cell_cg_cfg.is_type2();
  if (type2) {
    // The DCI format 0_1 size, including the Antenna ports field, follows the dedicated pusch-Config transformPrecoder
    // (or msg3-transformPrecoder when not set), while build_dci_f0_1_cg_activation() picks the antenna-port row from
    // the CP-OFDM table. As TS 38.212, Section 7.3.1.1.2 does not state which table the UE applies to an activation
    // DCI when the two waveforms differ, the dedicated PUSCH must be CP-OFDM too.
    const bool ded_tp_enabled =
        use_pusch_transform_precoding(bwp_ul_ded_cfg.pusch_cfg.has_value() ? &*bwp_ul_ded_cfg.pusch_cfg : nullptr,
                                      cell_cfg.use_msg3_transform_precoder());
    VERIFY(not ded_tp_enabled,
           "Configured Grant Type 2 is not supported with transform precoding enabled for the dynamic PUSCH");
  }

  // The CG scheduler requires all UL HARQ processes to operate in mode A, i.e. \c ul_harq_mode must keep its default
  // value (all-ones mask).
  VERIFY(not ul_cfg.pusch_serv_cell_cfg.has_value() or
             ul_cfg.pusch_serv_cell_cfg->ul_harq_mode == ~harq_ul_mode_mask(MAX_NOF_HARQS),
         "Configured Grant requires the default uplinkHARQ-mode (all UL HARQ processes in mode A)");

  if (cell_cfg.params.tdd_cfg.has_value()) {
    // If every full UL slot is a CG opportunity, the scheduler couldn't allocate any dynamic PUSCHs, which is necessary
    // to complete the RRC reconfiguration.
    const unsigned nof_tdd_slots_per_period     = nof_slots_per_tdd_period(cell_cfg.params.tdd_cfg.value());
    const unsigned nof_full_ul_slots_per_period = nof_full_ul_slots_per_tdd_period(cell_cfg.params.tdd_cfg.value());
    VERIFY(nof_tdd_slots_per_period < static_cast<unsigned>(cg_cfg.periodicity) or
               (nof_tdd_slots_per_period == static_cast<unsigned>(cg_cfg.periodicity) and
                nof_full_ul_slots_per_period > 1U),
           "TDD configuration is not compatible with CG period");
  }

  VERIFY(cg_cfg.rrc_configured_ul_grant_cfg.has_value() != type2,
         "Configured Grant type {} requires rrc_configured_ul_grant to be {}",
         type2 ? 2 : 1,
         type2 ? "absent" : "present");

  if (type2) {
    // The activation DCI is scrambled with the CS-RNTI, which a UE only monitors unconditionally in a UE-specific
    // SearchSpace (TS 38.213, Section 10.1), and of the UL formats TS 38.213, Table 10.2-1 allows for the activation
    // only 0_1 is supported. A UE without such a SearchSpace could never be activated, and since it stays queued,
    // run_ues_activation() would retry it on every slot and hold back every UE behind it. Type 1 needs no activation
    // DCI, hence no such SearchSpace.
    const auto is_uss_with_dci_f0_1 = [](const search_space_configuration& ss) {
      return not ss.is_common_search_space() and pdcch_helper::get_ul_dci_format(ss) == dci_ul_format::f0_1;
    };
    VERIFY(serv_cell_cfg.init_dl_bwp.pdcch_cfg.has_value() and
               std::any_of(serv_cell_cfg.init_dl_bwp.pdcch_cfg->search_spaces.begin(),
                           serv_cell_cfg.init_dl_bwp.pdcch_cfg->search_spaces.end(),
                           is_uss_with_dci_f0_1),
           "Configured Grant type 2 requires a UE-specific SearchSpace monitored with DCI format 0_1, to carry the "
           "activation DCI");

    // A Type 2 CG has no configured time-domain row: activate_ue() picks one at activation, from the same list the
    // activation DCI is resolved against, and skips the rows that overlap the SRS region or ask for slot repetitions.
    // If none survives, activation fails on every attempt, and since the UE stays queued it holds back every UE behind
    // it - so check here that at least one usable row exists rather than let the cell come up with a CG that can never
    // be activated.
    const bool has_ded_td_list =
        bwp_ul_ded_cfg.pusch_cfg.has_value() and not bwp_ul_ded_cfg.pusch_cfg->pusch_td_alloc_list.empty();
    const span<const pusch_time_domain_resource_allocation> activation_td_list =
        has_ded_td_list
            ? span<const pusch_time_domain_resource_allocation>(bwp_ul_ded_cfg.pusch_cfg->pusch_td_alloc_list)
            : span<const pusch_time_domain_resource_allocation>(
                  cell_cfg.init_bwp.ul.pusch_common()->pusch_td_alloc_list);

    const unsigned last_non_srs_symbol = config_helpers::compute_last_non_srs_symbol(cell_cfg.params);
    VERIFY(std::any_of(activation_td_list.begin(),
                       activation_td_list.end(),
                       [last_non_srs_symbol](const pusch_time_domain_resource_allocation& td) {
                         return config_helpers::is_cg_pusch_td_res_usable(td, last_non_srs_symbol);
                       }),
           "Configured Grant type 2 requires a PUSCH time domain resource that both ends at or before symbol {}, where "
           "the SRS region starts, and requests no slot repetitions, but none of the {} entries of the {} list "
           "qualifies",
           last_non_srs_symbol,
           activation_td_list.size(),
           has_ded_td_list ? "dedicated" : "common");

    // [Implementation-defined] Reconfiguration of TDRA list while CG is active is not supported. As a safe-guard
    // for future refactors, we enforce that the dedicated PUSCH time domain resource list starts with the entries of
    // the common list.
    if (has_ded_td_list) {
      const span<const pusch_time_domain_resource_allocation> common_td_list =
          cell_cfg.init_bwp.ul.pusch_common()->pusch_td_alloc_list;
      VERIFY(activation_td_list.size() >= common_td_list.size() and
                 std::equal(common_td_list.begin(), common_td_list.end(), activation_td_list.begin()),
             "Configured Grant type 2 requires the dedicated PUSCH time domain resource list to start with the {} "
             "entries of the common list",
             common_td_list.size());
    }

    return {};
  }

  const auto& cg_ul_grant = cg_cfg.rrc_configured_ul_grant_cfg.value();

  // TS 38.214, Section 6.1.2.3: a Type 1 CG with PUSCH repetition Type A selects its TDRA table as for DCI format 0_0
  // in a UE-specific search space, i.e. by Table 6.1.2.1.1-1. The only dedicated list that table admits is
  // pusch-TimeDomainAllocationList in pusch-Config, which is not modelled here: pusch_config::pusch_td_alloc_list
  // carries pusch-TimeDomainAllocationListDCI-0-1, which Table 6.1.2.1.1-1 never mentions and which only applies to
  // DCI format 0_1. A Type 1 CG index therefore always refers to the cell's common list, as reserve_cg_resources()
  // reads it.
  const span<const pusch_time_domain_resource_allocation> pusch_td_list =
      span<const pusch_time_domain_resource_allocation>(cell_cfg.init_bwp.ul.pusch_common()->pusch_td_alloc_list);
  VERIFY(
      cg_ul_grant.time_domain_allocation < pusch_td_list.size(),
      "Configured Grant time domain allocation index ({}) is out of range for the common PUSCH time domain allocation "
      "list ({} entries)",
      cg_ul_grant.time_domain_allocation,
      pusch_td_list.size());

  VERIFY(cg_ul_grant.time_domain_offset < static_cast<uint16_t>(cg_cfg.periodicity),
         "Configured Grant Type 1 time domain offset ({}) exceeds CG periodicity ({} slots)",
         cg_ul_grant.time_domain_offset,
         fmt::underlying(cg_cfg.periodicity));

  return {};
}

validator_result validate_bwp_ded_cfg(const serving_cell_config& ue_cell_cfg, const cell_configuration& cell_cfg)
{
  VERIFY(ue_cell_cfg.dl_bwps.empty(), "Only init DL BWP is supported");
  const auto& ue_bwp_ded              = ue_cell_cfg.init_dl_bwp;
  const auto& expected_bwp_ded_pdcchs = cell_cfg.bwp_res[to_bwp_id(0)].dl().ded_pdcchs;
  if (ue_bwp_ded.pdcch_cfg.has_value()) {
    // If UE has been provided a dedicated PDCCH config, verify it matches one of the base ones.
    auto pdcch_it =
        std::find(expected_bwp_ded_pdcchs.begin(), expected_bwp_ded_pdcchs.end(), ue_bwp_ded.pdcch_cfg.value());
    if (pdcch_it == expected_bwp_ded_pdcchs.end()) {
      return make_unexpected("Inconsistent PDCCH config");
    }
  }
  // [Implementation-defined] Resource allocation type 0 and interleaved VRB-to-PRB mapping are not used in the same
  // cell.
  VERIFY(not ue_bwp_ded.pdsch_cfg.has_value() or
             ue_bwp_ded.pdsch_cfg->res_alloc == pdsch_config::resource_allocation::resource_allocation_type_1 or
             cell_cfg.expert_cfg.ue.pdsch_interleaving_bundle_size == vrb_to_prb::mapping_type::non_interleaved,
         "PDSCH resource allocation type 0 is not supported with interleaved VRB-to-PRB mapping");
  return {};
}

static validator_result validate_ue_cells(span<const sched_ue_cell_config> cells, const cell_configuration& cell_cfg)
{
  for (const sched_ue_cell_config& cell : cells) {
    if (not cell.ded.has_value()) {
      // The UE uses only the common configuration of its PCell, e.g. a UE that gets an RRC Reject.
      VERIFY(cells.size() == 1, "cell={}: A UE without dedicated configuration cannot have SCells", cell.cell_index);
      continue;
    }
    const auto& serv_cell_cfg = cell.ded->serv_cell_cfg;
    VERIFY(serv_cell_cfg.cell_index == cell.cell_index,
           "cell={}: The ServingCellConfig is for cell={}",
           cell.cell_index,
           serv_cell_cfg.cell_index);
    HANDLE_ERROR(validate_pdcch_cfg(serv_cell_cfg, cell_cfg.params.dl_cfg_common));
    HANDLE_ERROR(validate_bwp_ded_cfg(serv_cell_cfg, cell_cfg));

    HANDLE_ERROR(validate_pdsch_cfg(serv_cell_cfg));

    if (serv_cell_cfg.ul_config.has_value()) {
      if (serv_cell_cfg.ul_config->init_ul_bwp.pucch_cfg.has_value() and
          serv_cell_cfg.ul_config->init_ul_bwp.srs_cfg.has_value() and
          cell_cfg.params.ul_cfg_common.init_ul_bwp.pucch_cfg_common.has_value()) {
        const pucch_config_common& pucch_cfg_common =
            cell_cfg.params.ul_cfg_common.init_ul_bwp.pucch_cfg_common.value();
        HANDLE_ERROR(validate_pucch_cfg(serv_cell_cfg,
                                        cell_cfg.params.init_bwp.pucch.resources,
                                        cell_cfg.bwp_res[to_bwp_id(0)].ul().pucch.dedicated,
                                        pucch_cfg_common,
                                        cell_cfg.params.dl_carrier.nof_ant));
        HANDLE_ERROR(validate_srs_cfg(serv_cell_cfg, cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs));
      }

      HANDLE_ERROR(validate_pusch_cfg(serv_cell_cfg.ul_config.value(), cell_cfg.params.init_bwp.csi.has_value()));

      if (serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value()) {
        HANDLE_ERROR(validate_cg_cfg(serv_cell_cfg, cell_cfg));
      }
    }
  }

  // TODO: Validate other parameters.
  return {};
}

error_type<std::string>
ocudu::config_validators::validate_sched_ue_creation_request_message(const sched_ue_creation_request_message& msg,
                                                                     const cell_configuration&                cell_cfg)
{
  // Verify the list of ServingCellConfig contains spCellConfig.
  VERIFY(msg.cfg.cells.has_value() and not msg.cfg.cells->empty(), "Empty list of ServingCellConfig");

  return validate_ue_cells(*msg.cfg.cells, cell_cfg);
}

error_type<std::string>
ocudu::config_validators::validate_sched_ue_reconfiguration_message(const sched_ue_reconfiguration_message& msg,
                                                                    const cell_configuration&               cell_cfg)
{
  // A reconfiguration carries a delta: an absent cell list leaves the UE's cell configuration untouched, so there is
  // nothing to check. The entries it does carry are complete ServingCellConfigs rather than deltas of their own -
  // du_cell_config_pool::update_ue() builds the new configuration from the entry alone - so the very same per-cell
  // checks as for a UE creation apply to them.
  if (not msg.cfg.cells.has_value()) {
    return {};
  }

  return validate_ue_cells(*msg.cfg.cells, cell_cfg);
}
