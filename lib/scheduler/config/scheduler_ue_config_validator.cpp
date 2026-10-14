// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/config/scheduler_ue_config_validator.h"
#include "cell_configuration.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/rrc_nr.h"
#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/scheduler/config/serving_cell_config_validator.h"
#include "ocudu/support/config/validator_helpers.h"

using namespace ocudu;
using namespace config_validators;

static validator_result validate_cg_cfg(const uplink_config& ul_cfg, const cell_configuration& cell_cfg)
{
  const auto& bwp_ul_ded_cfg = ul_cfg.init_ul_bwp;
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
    return {};
  }

  const auto& cg_ul_grant = cg_cfg.rrc_configured_ul_grant_cfg.value();

  // TS 38.214, Section 6.1.2.3: a Type 1 CG with PUSCH repetition Type A selects its TDRA table as for DCI format 0_0
  // in a UE-specific search space, i.e. the UE's dedicated pusch-TimeDomainAllocationList when configured, and the
  // common one otherwise (Table 6.1.2.1.1-1).
  // NOTE: [Implementation-defined] \c pusch-TimeDomainAllocationList is never set in the code, therefore the TDRA list
  // resolves to the common one (what can be set is \c PUSCH-TimeDomainResourceAllocation-r16, but that is not used for
  // CG type 1 and PUSCH repetition type A).
  // TODO: revise this once CG type 2 is supported, as the different rules apply in that case.
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

error_type<std::string>
ocudu::config_validators::validate_sched_ue_creation_request_message(const sched_ue_creation_request_message& msg,
                                                                     const cell_configuration&                cell_cfg)
{
  // Verify the list of ServingCellConfig contains spCellConfig.
  VERIFY(msg.cfg.cells.has_value() and not msg.cfg.cells->empty(), "Empty list of ServingCellConfig");

  for (const sched_ue_cell_config& cell : *msg.cfg.cells) {
    if (not cell.ded.has_value()) {
      // The UE uses only the common configuration of its PCell, e.g. a UE that gets an RRC Reject.
      VERIFY(msg.cfg.cells->size() == 1,
             "cell={}: A UE without dedicated configuration cannot have SCells",
             cell.cell_index);
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
        HANDLE_ERROR(validate_cg_cfg(serv_cell_cfg.ul_config.value(), cell_cfg));
      }
    }
  }

  // TODO: Validate other parameters.
  return {};
}
