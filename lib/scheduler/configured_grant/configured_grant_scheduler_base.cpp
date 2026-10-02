// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "configured_grant_scheduler_base.h"
#include "../cell/resource_grid.h"
#include "../support/bwp_helpers.h"
#include "../support/repetition_helpers.h"
#include "../support/sch_pdu_builder.h"
#include "../uci_scheduling/uci_allocator_impl.h"
#include "cg_sched_helpers.h"

using namespace ocudu;

configured_grant_scheduler_base::configured_grant_scheduler_base(const cell_configuration& cell_cfg_,
                                                                 const cg_builder_params&  cell_cg_cfg,
                                                                 uci_allocator&            uci_alloc_,
                                                                 ue_repository&            ues_) :
  cell_cfg(cell_cfg_),
  uci_alloc(uci_alloc_),
  ues(ues_),
  logger(ocudulog::fetch_basic_logger("SCHED")),
  cg_periodicity(cell_cg_cfg.periodicity),
  periodic_pusch_slot_wheel(static_cast<unsigned>(cg_periodicity))
{
  // Pre-reserve space for the UEs that will be added.
  // Note: The list is cleared every slot, so it generally holds far fewer UEs than the cell can have contexts for.
  updated_ues.reserve(cell_cfg.max_nof_ue_contexts);
}

const ue_cell* configured_grant_scheduler_base::get_ue_cell(rnti_t rnti) const
{
  const auto* u = ues.find_by_rnti(rnti);
  if (u != nullptr) {
    return u->find_cell(cell_cfg.cell_index);
  }
  return nullptr;
}

void configured_grant_scheduler_base::rem_ue_from_updated_ues(rnti_t rnti)
{
  auto it = std::find(updated_ues.begin(), updated_ues.end(), rnti);
  if (it != updated_ues.end()) {
    std::swap(*it, updated_ues.back());
    updated_ues.pop_back();
  }
}

void configured_grant_scheduler_base::run_slot(cell_resource_allocator& res_alloc)
{
  // For UEs whose CG configuration was recently added/updated, pre-reserve their CG PUSCH resources over the whole
  // resource grid, so that dynamic PUSCH grants scheduled in advance do not use these resources.
  reserve_updated_ues_resources(res_alloc);

  // Only pre-reserve in the farthest slot in the grid, as the previous part of the grid has been filled in earlier
  // calls to this function.
  reserve_slot_cg_resources(res_alloc[res_alloc.max_ul_slot_alloc_delay]);

  // We only allocate CG PUSCH for the current slot; else, we incur the possibility that the PUSCH gets allocated before
  // the PUCCH.
  static constexpr unsigned look_ahead_alloc_slots = 0U;
  allocate_slot_cg_opportunities(res_alloc[look_ahead_alloc_slots]);
}

void configured_grant_scheduler_base::stop()
{
  updated_ues.clear();
  periodic_pusch_slot_wheel.clear();
}

void configured_grant_scheduler_base::reserve_updated_ues_resources(cell_resource_allocator& res_alloc)
{
  // For all UEs whose CG config has been recently updated, reserve their CG resources up until one slot before the
  // farthest slot in the resource grid (the farthest slot is handled by reserve_slot_cg_resources()).
  for (const rnti_t rnti : updated_ues) {
    const ue_cell* ue_cc = get_ue_cell(rnti);
    if (ue_cc == nullptr) {
      logger.error("rnti={}: UE for which CG resources are being reserved was not found", rnti);
      continue;
    }

    for (unsigned n = 0; n != res_alloc.max_ul_slot_alloc_delay; ++n) {
      cell_slot_resource_allocator& slot_alloc = res_alloc[n];
      if (periodic_pusch_slot_wheel.contains(slot_alloc.slot, rnti)) {
        reserve_cg_resources(slot_alloc, *ue_cc);
      }
    }
  }

  // Clear the list of updated UEs.
  updated_ues.clear();
}

void configured_grant_scheduler_base::reserve_slot_cg_resources(cell_slot_resource_allocator& slot_alloc) const
{
  for (const rnti_t rnti : periodic_pusch_slot_wheel[slot_alloc.slot]) {
    const ue_cell* ue_cc = get_ue_cell(rnti);
    if (ue_cc == nullptr) {
      continue;
    }
    reserve_cg_resources(slot_alloc, *ue_cc);
  }
}

void configured_grant_scheduler_base::reserve_cg_resources(cell_slot_resource_allocator& slot_alloc,
                                                           const ue_cell&                ue_cc) const
{
  // Skip if UL is not enabled in this slot (e.g., TDD DL slot, or the slot falls in the UL measurement gap window).
  if (not ue_cc.is_ul_enabled(slot_alloc.slot)) {
    return;
  }

  // For PUSCH repetition Type A (i.e. with no pusch-RepTypeIndicator configured), a Type 1 CG selects its TDRA table as
  // for DCI format 0_0 in a UE-specific SearchSpace (TS 38.214, Section 6.1.2.3), i.e. by Table 6.1.2.1.1-1. The only
  // dedicated list that table admits is pusch-TimeDomainAllocationList in pusch-Config, which is not modelled here -
  // pusch_config::pusch_td_alloc_list carries pusch-TimeDomainAllocationListDCI-0-1, which that table never mentions.
  // So the applicable list is always the cell's common one, which is what the mapper returns for f0_0.
  // NOTE: a Type 2 CG follows the table of its activation DCI instead, hence f0_1; the grant state manager this reads
  // from was filled from that list by activate_ue().
  const bool  is_type1 = ue_cc.cfg().init_bwp().ul.ded()->cg_cfg->rrc_configured_ul_grant_cfg.has_value();
  const auto& pusch_td_list =
      ue_cc.active_bwp().ul.td_mapper().pusch_td_resources(is_type1 ? dci_ul_format::f0_0 : dci_ul_format::f0_1);

  const auto& cg_params = ue_cc.get_conf_grant_state_manager().get_grant_params();
  if (not cg_params.has_value()) {
    // The UE has no active CG, so it has no resources to reserve in this slot.
    return;
  }

  // Compute CRBs: CG PUSCH uses non-interleaved VRB-to-PRB mapping, so VRBs = PRBs.
  const bwp_configuration& ul_bwp_cfg = cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params;
  const crb_interval       crbs       = prb_to_crb(ul_bwp_cfg, cg_params->vrbs.convert_to<prb_interval>());

  // Mark the CG PUSCH resources as allocated in the UL resource grid.
  // NOTE: the validity of the TD allocation w.r.t. pusch_td_list has been verified in the config validator.
  slot_alloc.ul_res_grid.fill(
      grant_info{ul_bwp_cfg.scs, pusch_td_list[cg_params->time_domain_allocation].symbols, crbs});
}

void configured_grant_scheduler_base::allocate_slot_cg_opportunities(cell_slot_resource_allocator& slot_alloc)
{
  for (const rnti_t rnti : periodic_pusch_slot_wheel[slot_alloc.slot]) {
    allocate_cg_opportunity(slot_alloc, rnti);
  }
}

bool configured_grant_scheduler_base::validate_cg_opportunity(cell_slot_resource_allocator& slot_alloc,
                                                              rnti_t                        rnti) const
{
  // Fetch UE and its cell context.
  const auto* u = ues.find_by_rnti(rnti);
  if (u == nullptr) {
    logger.error("rnti={}: resources reserved for CG opportunity, but corresponding UE not found", rnti);
    return false;
  }

  ocudu_assert(
      u->ue_cfg_dedicated()->get_cs_rnti().has_value(), "Missing CS-RNTI for UE={} with Configured Grant", rnti);

  const auto* ue_cc = u->find_cell(cell_cfg.cell_index);
  if (ue_cc == nullptr) {
    logger.error("rnti={}: resources reserved for CG opportunity, but corresponding UE not found", rnti);
    return false;
  }
  // Skip if UL is not enabled in this slot (e.g., TDD DL slot, or the slot falls in the UL measurement gap window).
  if (not ue_cc->is_ul_enabled(slot_alloc.slot)) {
    return false;
  }

  // As per TS 38.213, Section 9.2.6, if a PUCCH transmission with repetitions overlaps a PUSCH, the UE transmits the
  // PUCCH and does not transmit the PUSCH in the overlapping slots. The UCI cannot be moved onto the CG PUSCH either,
  // as Section 9.2.5 scopes UCI multiplexing on PUSCH to PUCCHs "over a single slot without repetitions". So the CG
  // occasion must be skipped in any slot of an in-flight repetition burst, or the UE would drop it.
  if (uci_alloc.has_pucch_repetition(rnti, slot_alloc.slot)) {
    logger.debug("rnti={}: CG PUSCH skipped at slot={}: the slot is part of a PUCCH repetition burst of the UE",
                 rnti,
                 slot_alloc.slot);
    return false;
  }

  // Check that the PUSCH result list has capacity.
  if (slot_alloc.result_builder().ul().puschs().full()) {
    logger.warning("rnti={}: CG PUSCH cannot be allocated at slot={}: PUSCH list is full", rnti, slot_alloc.slot);
    return false;
  }

  return true;
}

bool configured_grant_scheduler_base::allocate_cg_opportunity(cell_slot_resource_allocator& slot_alloc, rnti_t rnti)
{
  if (not validate_cg_opportunity(slot_alloc, rnti)) {
    return false;
  }

  // NOTE: the existence of u and ue_cc has been validated above.
  auto*                        u      = ues.find_by_rnti(rnti);
  auto*                        ue_cc  = u->find_cell(cell_cfg.cell_index);
  const ue_cell_configuration& ue_cfg = ue_cc->cfg();

  const slot_point pusch_slot = slot_alloc.slot;
  const auto&      cg_cfg     = ue_cfg.init_bwp().ul.ded()->cg_cfg.value();

  // The CG grant parameters were established when the UE was added to the wheel: from the RRC-configured grant for CG
  // Type 1, and from the activation DCI for CG Type 2.
  const auto& cg_params = ue_cc->get_conf_grant_state_manager().get_grant_params();
  ocudu_assert(cg_params.has_value(), "rnti={}: CG PUSCH scheduled for a UE with no active CG", rnti);

  pusch_config_params pusch_params = build_cg_pusch_cfg_params(ue_cfg, cg_params->time_domain_allocation);
  // The HARQ-ACK bits are the only element of pusch_params that need to be updated at scheduling time.
  pusch_params.nof_harq_ack_bits = uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(pusch_slot, u->crnti);

  // The CG HARQ processes are reserved by ue_cell::handle_reconfiguration_request(), which runs when the
  // reconfiguration carrying the CG configuration reaches the UE repository. Every production path that establishes a
  // CG grant goes through such a reconfiguration first, so a UE registered in the CG scheduler without the reservation
  // in place means the two went out of sync; assert here rather than on the bare HARQ-id below, which gives no hint of
  // the missing reconfiguration.
  ocudu_assert(ue_cc->harqs.nof_cg_reserved_ul_harqs() >= cg_cfg.nof_harq_processes,
               "rnti={}: CG PUSCH allocated with {} reserved UL HARQ processes, but the CG configuration requires {}",
               rnti,
               ue_cc->harqs.nof_cg_reserved_ul_harqs(),
               cg_cfg.nof_harq_processes);

  static constexpr unsigned nof_harq_retx = 0;
  // TS 38.321 Section 5.4.1 derives the HARQ process from CURRENT_symbol, counted in the UE uplink frame; pusch_slot
  // is the gNB grid slot, k_mac ahead of it. Undo k_mac so both ends pick the same process for the occasion.
  const harq_id_t h_id = get_cg_harq_id(
      cell_cfg.dl_to_ue_ul_slot(pusch_slot), pusch_params.symbols.start(), cg_periodicity, cg_cfg.nof_harq_processes);
  const unsigned cg_harq_timeout = cg_configuration::configured_grant_timer * static_cast<unsigned>(cg_periodicity);
  auto           h_ul =
      ue_cc->harqs.alloc_ul_harq(pusch_slot, pusch_slot, nof_harq_retx, cg_harq_alloc_params{h_id, cg_harq_timeout});
  ocudu_assert(h_ul.has_value(), "Failed to allocate UL HARQ id={}", fmt::underlying(h_id));

  // NOTE: the CG PUSCH RBs and symbols have already been pre-reserved in the UL resource grid, either by
  // reserve_slot_cg_resources() when this slot entered the resource grid, or by reserve_updated_ues_resources() right
  // after the UE was added; hence, no collision check nor grid fill is needed here.

  // Fill UL scheduling result.
  ul_sched_info* pusch_pdu = slot_alloc.result_builder().ul().puschs().emplace_back();
  if (pusch_pdu == nullptr) {
    logger.warning("rnti={}: CG PUSCH cannot be allocated at slot={}: PUSCH list is full", rnti, slot_alloc.slot);
    return false;
  }
  ul_sched_info& sched_info = *pusch_pdu;

  // Configured Grant repetitions are not currently supported.
  constexpr unsigned rep_idx = 0U;
  build_pusch_cs_rnti(sched_info.pusch_cfg,
                      u->ue_cfg_dedicated()->get_cs_rnti().value(),
                      pusch_params,
                      {sch_mcs_index{cg_params->mcs}, cg_params->tbs},
                      ue_cfg,
                      ue_cc->active_bwp(),
                      cg_params->vrbs,
                      get_cg_repetition_rv(cg_cfg.rep, rep_idx),
                      h_ul.value().id());

  // Check if there is any UCI grant allocated on the PUCCH that can be moved to the PUSCH.
  constexpr bool configured_grant = true;
  uci_alloc.multiplex_uci_on_pusch(sched_info, slot_alloc, ue_cfg, pusch_params.aperiodic_csi, configured_grant);

  // Fill decision context (informational; not forwarded to the PHY).
  sched_info.context.ue_index = u->ue_index;
  // Not applicable for a CG occasion, which carries no PDCCH of its own.
  sched_info.context.ss_id = to_search_space_id(0);
  // Not applicable for a CG occasion; k2 is only used for the CG Type 2 activation.
  sched_info.context.k2 = 0;
  // With CG, the periodic allocated grants always contained new-tx.
  sched_info.context.nof_retxs  = nof_harq_retx;
  sched_info.context.nof_oh_prb = pusch_params.nof_oh_prb;

  // NOTE 1: Do not reset the SR here, as it might be needed for SRB, which we exclude from CG.
  // NOTE 2: We should call ue_logical_channel_repository::handle_ul_grant() here, but that would not be safe, as we
  // might wrongly update the SRB in that. Unless we defined by design that SRBs are mapped to a specific LCG (not
  // defined by the standard), we should skip updating the LCG repository.

  on_cg_pusch_allocated(*u, pusch_slot);

  return true;
}
