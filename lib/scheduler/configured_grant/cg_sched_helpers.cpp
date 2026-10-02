// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cg_sched_helpers.h"
#include "../config/cell_configuration.h"
#include "../config/time_domain_mapper.h"
#include "../config/ue_configuration.h"
#include "../support/dmrs_helpers.h"

using namespace ocudu;

harq_id_t ocudu::get_cg_harq_id(slot_point                      pusch_slot,
                                unsigned                        symbol,
                                cg_configuration::periodicity_t periodicity,
                                uint8_t                         nof_harq_processes)
{
  // As per TS 38.321, Section 5.4.1.
  const unsigned current_symbol =
      static_cast<unsigned>(pusch_slot.system_slot()) * NOF_OFDM_SYM_PER_SLOT_NORMAL_CP + symbol;
  const unsigned periodicity_sym = static_cast<unsigned>(periodicity) * NOF_OFDM_SYM_PER_SLOT_NORMAL_CP;
  return to_harq_id((current_symbol / periodicity_sym) % static_cast<unsigned>(nof_harq_processes));
}

pusch_config_params ocudu::build_cg_pusch_cfg_params(const ue_cell_configuration& ue_cell_cfg,
                                                     uint8_t                      time_domain_allocation)
{
  const cell_configuration& cell_cfg = ue_cell_cfg.cell_cfg_common;

  // NOTE: one of the consumers of this function calls it while a UE reconfiguration is being applied, when \ref
  // ue_cell_cfg is the new configuration but the UE cell context still holds the old one. Everything below must
  // therefore be read off \ref ue_cell_cfg, never off the \ref ue_cell.

  const auto* ul_ded = ue_cell_cfg.init_bwp().ul.ded();
  const auto& cg_cfg = ul_ded->cg_cfg.value();

  // TS 38.214, Section 6.1.2.3. A Type 1 CG selects its TDRA table as for DCI format 0_0 in a UE-specific SearchSpace,
  // i.e. by Table 6.1.2.1.1-1, whose only dedicated option is pusch-TimeDomainAllocationList in pusch-Config; that IE
  // is not modelled here, so a Type 1 CG always lands on the cell's common list. A Type 2 CG instead uses the table of
  // the DCI that activated it, which is format 0_1, i.e. pusch-TimeDomainAllocationListDCI-0-1 when configured and the
  // common list otherwise (Table 6.1.2.1.1-1A) - which is what the mapper returns for f0_1.
  const bool  is_type1 = cg_cfg.rrc_configured_ul_grant_cfg.has_value();
  const auto& pusch_td_list =
      ue_cell_cfg.init_bwp().ul.td_mapper().pusch_td_resources(is_type1 ? dci_ul_format::f0_0 : dci_ul_format::f0_1);

  const pusch_time_domain_resource_allocation& pusch_td_cfg = pusch_td_list[time_domain_allocation];
  pusch_config_params                          pusch_params;
  pusch_params.symbols = pusch_td_cfg.symbols;

  // Build DMRS information from the CG-specific DMRS configuration.
  static constexpr unsigned nof_layers           = 1;
  static constexpr bool     are_both_cws_enabled = false;
  const dmrs_information    dmrs                 = make_dmrs_info_dedicated(pusch_td_cfg,
                                                         cell_cfg.params.pci,
                                                         cell_cfg.params.dmrs_typeA_pos,
                                                         cg_cfg.cg_dmrs_cfg,
                                                         nof_layers,
                                                         cell_cfg.params.ul_carrier.nof_ant,
                                                         are_both_cws_enabled);

  // Build PUSCH configuration parameters for TBS computation.
  pusch_params.dmrs       = dmrs;
  pusch_params.mcs_table  = cg_cfg.mcs_table;
  pusch_params.nof_layers = nof_layers;
  // TODO: Import p_pi2bpsk_present from PUSCH Config once it will have been added there.
  pusch_params.tp_pi2bpsk_present = false;
  // Transform precoding not currently supported for CG. The mcs_table_transform_precoder field is separate.
  pusch_params.use_transform_precoder = false;
  // As per TS 38.214, Section 5.1.3.2 and 6.1.4.2, and TS 38.212, Section 7.3.1.1 and 7.3.1.2, TB scaling filed is only
  // used for DCI Format 1-0 (in the DL). Therefore, for the PUSCH this is set to 0.
  pusch_params.tb_scaling_field = 0;
  // As per TS 38.214, Section 6.1.4.2, nof_oh_prb equals xOverhead when configured; otherwise 0.
  pusch_params.nof_oh_prb = ue_cell_cfg.pusch_serving_cell_cfg() != nullptr
                                ? static_cast<unsigned>(ue_cell_cfg.pusch_serving_cell_cfg()->x_ov_head)
                                : static_cast<unsigned>(x_overhead::not_set);
  // If aperiodic CSI is configured, we'll trigger it only for dynamic grants.
  pusch_params.aperiodic_csi = false;

  // NOTE: pusch_params.nof_harq_ack_bits can only be built at scheduling time, as it needs the pusch slot.
  return pusch_params;
}
