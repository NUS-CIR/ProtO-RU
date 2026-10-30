// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "configured_grant_type2_sched_impl.h"
#include "../../scheduler/support/dci_builder.h"
#include "../cell/resource_grid.h"
#include "../pdcch_scheduling/pdcch_resource_allocator.h"
#include "../support/bwp_helpers.h"
#include "../support/dmrs_helpers.h"
#include "../support/mcs_tbs_calculator.h"
#include "../support/repetition_helpers.h"
#include "../support/sch_pdu_builder.h"
#include "../uci_scheduling/uci_allocator_impl.h"
#include "cg_sched_helpers.h"
#include "cg_type2_resource_manager.h"
#include "ocudu/ran/csi_report/csi_report_config_helpers.h"
#include "ocudu/ran/direct_current_offset.h"
#include "ocudu/ran/slot_pdu_capacity_constants.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/support/rb_helper.h"
#include <optional>

using namespace ocudu;

// Returns the SearchSpace in which the CG Type 2 activation DCI must be sent, or nullptr if the UE has no suitable
// one configured.
static const search_space_info* get_cg_activation_search_space(const ue_cell_configuration& ue_cfg)
{
  // The CG Type 2 activation DCI is scrambled with the CS-RNTI, which a UE only monitors unconditionally in a
  // UE-specific SearchSpace or in a Type3 common SearchSpace, both configured in PDCCH-Config (TS 38.213,
  // Section 10.1). The common SearchSpaces of PDCCH-ConfigCommon are not usable: CS-RNTI monitoring there is granted
  // only in slots where the UE also monitors SI-RNTI, RA-RNTI, MsgB-RNTI or P-RNTI, which the scheduler cannot target.
  // Of the two UL DCI formats that TS 38.213, Table 10.2-1 allows for the activation, only 0_1 is supported, so a
  // SearchSpace monitored with format 0_0 cannot carry it either.
  for (const sched_search_space_config* ss_cfg : ue_cfg.init_bwp().dl.pdcch().search_spaces()) {
    if (ss_cfg->cfg().is_common_search_space()) {
      continue;
    }
    const search_space_info& ss_info = ue_cfg.search_space(ss_cfg->cfg().get_id());
    if (ss_info.get_ul_dci_format() == dci_ul_format::f0_1) {
      return &ss_info;
    }
  }
  return nullptr;
}

static bool are_grants_and_rbs_free(cell_resource_allocator&                          cell_alloc,
                                    span<const pusch_time_domain_resource_allocation> pusch_td_list,
                                    unsigned                                          cg_period_slots,
                                    uint8_t                                           td_alloc,
                                    const crb_interval&                               crbs,
                                    subcarrier_spacing                                scs,
                                    rnti_t                                            ue_rnti)
{
  // The scan covers the whole UL window of the grid, as a UL grant may already sit in any slot of it (e.g., dynamic
  // PUSCH repetition bundles).
  // NOTE: the scan starts at k2, which is where the UE's first CG transmission is.
  for (unsigned delay = pusch_td_list[td_alloc].k2; delay <= cell_alloc.max_ul_slot_alloc_delay;
       delay += cg_period_slots) {
    auto& pdcch_alloc = cell_alloc[delay];

    // Check whether there is space for another PUSCH in the list.
    if (pdcch_alloc.result_builder().ul().puschs().full()) {
      return false;
    }

    // Check whether the REs/symbols on the grid are available.
    if (pdcch_alloc.ul_res_grid.collides(scs, pusch_td_list[td_alloc].symbols, crbs)) {
      return false;
    }

    // The UE must not already hold a PUSCH in this slot, even on other RBs. As per TS 38.321, Section 5.8.2, a UE only
    // uses its configured grant when the PUSCH duration does not overlap one of an uplink grant received on the PDCCH.
    const auto puschs = pdcch_alloc.result_reader().ul().puschs();
    if (std::any_of(puschs.begin(), puschs.end(), [ue_rnti](const ul_sched_info& pusch) {
          return pusch.pusch_cfg.rnti == ue_rnti;
        })) {
      return false;
    }
  }

  return true;
}

configured_grant_type2_sched_impl::configured_grant_type2_sched_impl(const cell_configuration& cell_cfg_,
                                                                     const cg_builder_params&  cell_cg_cfg,
                                                                     uci_allocator&            uci_alloc_,
                                                                     pdcch_resource_allocator& pdcch_alloc_,
                                                                     ue_repository&            ues_) :
  configured_grant_scheduler_base(cell_cfg_, cell_cg_cfg, uci_alloc_, ues_), pdcch_sch(pdcch_alloc_), res_mng(cell_cfg)
{
  ues_pending_activation.reserve(MAX_NOF_DU_UES);
}

bool configured_grant_type2_sched_impl::has_cg_grant_cfg(const ue_cell_configuration& ue_cfg) const
{
  const auto* ul_ded = ue_cfg.init_bwp().ul.ded();
  return ul_ded != nullptr and ul_ded->cg_cfg.has_value();
}

std::vector<configured_grant_type2_sched_impl::ue_pending_activation>::iterator
configured_grant_type2_sched_impl::find_pending(du_ue_index_t ue_idx)
{
  return std::find_if(ues_pending_activation.begin(),
                      ues_pending_activation.end(),
                      [ue_idx](const ue_pending_activation& u) { return u.ue_index() == ue_idx; });
}

void configured_grant_type2_sched_impl::add_ue_to_wheel(const ue_cell_configuration& ue_cfg,
                                                        unsigned                     time_domain_offset)
{
  auto* u = ues.find_by_rnti(ue_cfg.crnti);
  if (u == nullptr) {
    logger.error("rnti={}: UE not found in the CG scheduler UEs repo", ue_cfg.crnti);
    return;
  }

  if (not has_cg_grant_cfg(ue_cfg)) {
    return;
  }

  // Fill the slot wheel at every slot where a CG PUSCH opportunity occurs.
  const auto wheel_res = periodic_pusch_slot_wheel.add_ue(ue_cfg.crnti, time_domain_offset);
  if (wheel_res.nof_dup != 0) {
    logger.error("rnti={}: CG grant already present in slot wheel at {} offsets", ue_cfg.crnti, wheel_res.nof_dup);
  }
  if (wheel_res.nof_full != 0) {
    logger.error("rnti={}: CG grant dropped at {} slot wheel offsets with no room left; the RRM admitted a UE the "
                 "scheduler cannot hold",
                 ue_cfg.crnti,
                 wheel_res.nof_full);
  }

  // Register the UE in the list of recently updated UEs, so that its CG resources get pre-reserved over the whole
  // resource grid at the next run_slot().
  updated_ues.push_back(ue_cfg.crnti);
}

void configured_grant_type2_sched_impl::rem_ue(const ue_cell_configuration& ue_cfg)
{
  if (not has_cg_grant_cfg(ue_cfg)) {
    return;
  }

  // Remove UE from updated UEs list. Keyed on the RNTI alone, so it is done before anything that needs the UE context.
  rem_ue_from_updated_ues(ue_cfg.crnti);

  // The UE removal and the UE reconfiguration both call this function while the UE is still in the repository, with
  // its current configuration, whose C-RNTI is the one the UE is registered with.
  auto* u = ues.find_by_rnti(ue_cfg.crnti);
  ocudu_assert(u != nullptr, "rnti={}: UE not found in the CG scheduler UEs repo during UE removal", ue_cfg.crnti);

  // UE only added to the UEs pending activation list.
  auto ue_to_rem_it = find_pending(u->ue_index);
  if (ue_to_rem_it != ues_pending_activation.end()) {
    std::swap(*ue_to_rem_it, ues_pending_activation.back());
    ues_pending_activation.pop_back();
  }

  auto&       ue_cg_state_mng = u->get_pcell().get_conf_grant_state_manager();
  const auto& cg_params       = ue_cg_state_mng.get_grant_params();
  if (not cg_params.has_value()) {
    // UE hasn't started yet the activation process and hasn't been added yet to the periodic wheel, we can safely
    // return here.
    return;
  }

  // Return the CG resource acquired at activation time to the resource manager pool. The UE keeps it across
  // re-activations, so this, add_reconf_ue() and stop() are the only places it is returned.
  res_mng.deallocate_resource(cg_params->time_offset, cg_params->resource_id, ue_cfg.crnti);

  // Remove the UE from the periodic wheel using the CG offset recorded at activation time, so that the entries removed
  // are exactly the ones add_ue_to_wheel() inserted.
  // It is possible that the UE has not been added to the wheel yet, e.g. if it is still pending activation, so a
  // missing entry is not reported as an error here.
  (void)periodic_pusch_slot_wheel.rem_ue(ue_cfg.crnti, cg_params->time_offset);

  ue_cg_state_mng.reset_state();
}

void configured_grant_type2_sched_impl::add_reconf_ue(const ue_cell_configuration& new_ue_cfg,
                                                      const ue_cell_configuration* old_ue_cfg)
{
  // NOTE: in CG type2, a UE is not added to the periodic wheel until it has been activated, and it is queued for
  // activation by the configuration-applied indication, so there is nothing to do for a UE with no previous
  // configuration. This is reached when an F1AP-created UE completes contention resolution.
  if (old_ue_cfg == nullptr) {
    return;
  }

  const auto* new_ul_ded = new_ue_cfg.init_bwp().ul.ded();
  const auto* old_ul_ded = old_ue_cfg->init_bwp().ul.ded();

  if (new_ul_ded != nullptr and old_ul_ded != nullptr and new_ul_ded->cg_cfg.has_value() and
      old_ul_ded->cg_cfg.has_value() and new_ul_ded->cg_cfg.value() == old_ul_ded->cg_cfg.value()) {
    // CG configuration unchanged, so there is nothing to do.
    // NOTE: the DCI 0_1 PUSCH TDRA list can change in this reconfiguration, as the dedicated list replaces the common
    // one once the UE capabilities enable PUSCH repetitions. The index keeps selecting the same row, as the dedicated
    // list starts with the common one (checked by the UE config validator) and the only row appended asks for
    // repetitions, which activate_ue() never picks.
    return;
  }

  // The CG configuration did change, so the UE has to be given a new activation DCI. Removing it first cleans all the
  // state the previous configuration left behind: its CG resource, its entries in the periodic wheel and its pending
  // activation, if any.
  rem_ue(*old_ue_cfg);
}

void configured_grant_type2_sched_impl::run_slot(cell_resource_allocator& cell_alloc)
{
  static constexpr unsigned look_ahead_alloc_slots = 0U;
  auto&                     pdcch_alloc            = cell_alloc[look_ahead_alloc_slots];
  const slot_point          pdcch_slot             = pdcch_alloc.slot;

  // Mark for re-activation the UEs that did not confirm their latest activation DCI in time; they are served below.
  check_confirmation_deadlines(pdcch_slot);

  // Scan the UEs that are ready for activation and allocate the activation DCIs.
  run_ues_activation(cell_alloc);

  // Reserve the CG resources in the grid and allocate the CG PUSCH occasions of this slot.
  configured_grant_scheduler_base::run_slot(cell_alloc);
}

void configured_grant_type2_sched_impl::stop()
{
  ues_pending_activation.clear();
  // Return the CG resources held by the activated UEs to the pool. On cell deactivation the UEs are dropped straight
  // from the UE repository by ue_repository::handle_cell_deactivation(), which does not go through rem_ue(), so
  // without this the resources would stay marked as taken and the pool would drain over deactivation cycles.
  res_mng.reset();
  configured_grant_scheduler_base::stop();
}

void configured_grant_type2_sched_impl::start_ue_cg_activation(du_ue_index_t ue_idx)
{
  auto* u = ues.find(ue_idx);
  if (u == nullptr) {
    logger.error("ue={}: UE not found in the CG scheduler UEs repo", ue_idx);
    return;
  }

  if (u->get_pcell().cfg().init_bwp().ul.cg_cfg() == nullptr) {
    // CG not configured.
    return;
  }

  if (u->get_pcell().get_conf_grant_state_manager().get_grant_params().has_value()) {
    // The UE already holds a grant: its activation is either in progress, with the UE already in the pending list, or
    // confirmed. A confirmed UE gets here after a reconfiguration that leaves its CG unchanged; queuing it again would
    // have activate_ue() acquire a second resource and add the UE to the wheel at a second offset.
    // NOTE: a reconfiguration that changes the CG goes through add_reconf_ue() -> rem_ue(), which clears the grant, so
    // the UE is queued for a new activation.
    return;
  }

  if (find_pending(ue_idx) == ues_pending_activation.end()) {
    ues_pending_activation.emplace_back(ue_idx);
  }
}

void configured_grant_type2_sched_impl::run_ues_activation(cell_resource_allocator& cell_alloc)
{
  const slot_point pdcch_slot = cell_alloc[0U].slot;

  if (not cell_cfg.is_dl_enabled(pdcch_slot)) {
    return;
  }

  // Every activation of this slot is issued from this function, so a local counter is enough to ration them; no state
  // has to survive the call.
  unsigned                      nof_activations = 0;
  cell_slot_resource_allocator& pdcch_alloc     = cell_alloc[0U];

  // Returns whether the UE can be sent an activation DCI in this slot. A UE in fallback is skipped until it applies its
  // new configuration: until then, the size of the DCI format 0_1 it decodes is not known.
  const auto can_receive_activation_dci = [pdcch_slot](const ue& u) {
    return u.get_pcell().is_pdcch_enabled(pdcch_slot) and not u.get_pcell().is_in_fallback_mode();
  };

  // Returns whether no activation DCI can be added to this slot, for any UE.
  const auto is_slot_full = [&]() {
    if (nof_activations == MAX_CG_ACTIVATION_PDCCHS_PER_SLOT) {
      // Ration the activation DCIs, so that they do not consume the PDCCH space the dynamic grants need. The UEs left
      // in the list keep their place and are served over the next slots.
      return true;
    }
    if (pdcch_alloc.result_builder().dl().ul_pdcchs().full()) {
      logger.info("Stopping CG Type 2 activations at slot={}. Cause: max PDCCH grants reached", pdcch_slot);
      return true;
    }
    return false;
  };

  // Re-activations go first: each one can only be served in the slots from which a TDRA row lands on the UE's CG
  // offset, whereas a first activation can be served in any slot.
  for (ue_pending_activation& u : ues_pending_activation) {
    // A UE leaves this list before it leaves the repository: rem_ue() takes it out on UE removal, and stop() clears the
    // list on cell deactivation.
    ocudu_assert(ues.contains(u.ue_index()),
                 "ue={}: UE pending CG Type 2 activation not found in the CG scheduler UEs repo",
                 fmt::underlying(u.ue_index()));

    if (u.state() != ue_pending_activation::state_t::reactivation_due) {
      continue;
    }
    if (is_slot_full()) {
      return;
    }
    ue& ue_ref = ues[u.ue_index()];
    if (not can_receive_activation_dci(ue_ref)) {
      continue;
    }
    if (reactivate_ue(cell_alloc, u, ue_ref) == activation_outcome::success) {
      ++nof_activations;
    }
  }

  // First activations, for the UEs that have no activation DCI outstanding or due.
  for (ue_pending_activation& u : ues_pending_activation) {
    if (u.state() != ue_pending_activation::state_t::awaiting_first_activation) {
      continue;
    }
    if (is_slot_full()) {
      return;
    }
    ue& ue_ref = ues[u.ue_index()];
    if (not can_receive_activation_dci(ue_ref)) {
      continue;
    }
    const activation_outcome outcome = activate_ue(cell_alloc, u, ue_ref);
    if (outcome == activation_outcome::stop_slot) {
      // No more resources available for any UE; exit here.
      return;
    }
    if (outcome == activation_outcome::success) {
      ++nof_activations;
    }
    // On skip_ue the UE keeps its place in the list and is retried on a later slot, while the ones behind it get their
    // turn now: its failure says nothing about whether they can be activated.
  }
}

void configured_grant_type2_sched_impl::check_confirmation_deadlines(slot_point slot)
{
  // The UE confirms every activation DCI it receives with a CG Confirmation MAC CE, sent in its first UL transmission
  // after the DCI (TS 38.321, Sections 5.4.1 and 5.8.2). With no confirmation by the deadline, either the UE missed the
  // DCI or its confirmation was lost; in both cases a new activation DCI for the same grant is the remedy.
  // NOTE: the deadline is checked on every slot and lies at most k2 + max_expected_crc_delay slots after the DCI, so
  // the slot_point comparison stays within half a hyperframe.
  for (ue_pending_activation& u : ues_pending_activation) {
    if (u.check_confirmation_deadline(slot)) {
      logger.info("ue={}: Re-activating CG Type 2. Cause: the activation DCI of slot={} was not confirmed by slot={}",
                  fmt::underlying(u.ue_index()),
                  u.activation_pdcch_slot(),
                  u.confirmation_deadline());
    }
  }
}

void configured_grant_type2_sched_impl::ue_pending_activation::on_activation_dci_allocated(slot_point pdcch_slot_,
                                                                                           slot_point deadline_)
{
  ocudu_assert(st != state_t::awaiting_confirmation,
               "ue={}: CG Type 2 activation DCI allocated while the previous one awaits confirmation",
               fmt::underlying(ue_idx));
  st         = state_t::awaiting_confirmation;
  pdcch_slot = pdcch_slot_;
  deadline   = deadline_;
}

bool configured_grant_type2_sched_impl::ue_pending_activation::check_confirmation_deadline(slot_point slot)
{
  if (st != state_t::awaiting_confirmation or slot <= deadline) {
    return false;
  }
  st = state_t::reactivation_due;
  return true;
}

bool configured_grant_type2_sched_impl::ue_pending_activation::on_pdcch_discarded(slot_point sl_tx)
{
  if (st != state_t::awaiting_confirmation or pdcch_slot != sl_tx) {
    return false;
  }
  st = state_t::reactivation_due;
  return true;
}

configured_grant_type2_sched_impl::activation_outcome
configured_grant_type2_sched_impl::activate_ue(cell_resource_allocator& cell_alloc,
                                               ue_pending_activation&   pending,
                                               ue&                      u)
{
  if (res_mng.get_nof_free_resources() == 0) {
    logger.error("ue={}: Failed to activate CG PUSCH type2. Cause: no available resources",
                 fmt::underlying(u.ue_index));
    return activation_outcome::stop_slot;
  }

  auto& pdcch_alloc = cell_alloc[0U];

  const auto                                              slot_pdcch    = pdcch_alloc.slot;
  const span<const pusch_time_domain_resource_allocation> pusch_td_list = get_cg_pusch_td_list(u.get_pcell().cfg());

  // For type2 CG, the TD allocation is not defined yet at this point; the scheduler will choose the symbols that do
  // not collide with SRS. In the following, we proceed under that assumption.

  // Compute PUSCH symbols to avoid overlapping with SRS.
  const unsigned last_non_srs_symbol = config_helpers::compute_last_non_srs_symbol(cell_cfg.params);

  // Find k2 for which there is a CG resource available.
  std::optional<uint8_t>           candidate_td_allocation;
  std::optional<cg_type2_resource> candidate_resource;
  std::optional<unsigned>          candidate_cg_offset;

  const auto cg_period_slots =
      static_cast<unsigned>(u.get_pcell().cfg().init_bwp().ul.ded()->cg_cfg.value().periodicity);

  // The CG offset below is a phase within the UE CG period, but cg_type2_resource_manager tracks resource occupancy
  // in a wheel sized by the cell CG period. If the two differed, a UE whose period is shorter would recur on offsets
  // the resource manager never marks as used, and two UEs could be handed the same resource on the same slot.
  ocudu_assert(cg_period_slots == static_cast<unsigned>(cell_cfg.params.init_bwp.cg_cfg.value().periodicity),
               "ue={}: CG period of {} slots differs from the cell CG period of {} slots",
               fmt::underlying(u.ue_index),
               cg_period_slots,
               static_cast<unsigned>(cell_cfg.params.init_bwp.cg_cfg.value().periodicity));

  // NOTE: In the following, \c pusch_td_list might not be the same as the one used to compute the initial CG resources
  // (i.e, dedicated vs. common). It is assumed that the symbols intervals available in both lists are the same for each
  // of their k2 values.

  for (uint8_t td = 0, sz = pusch_td_list.size(); td != sz; ++td) {
    // Skip the TDRA entries whose symbols collide with the SRS ones, or that set repetitions.
    if (not config_helpers::is_cg_pusch_td_res_usable(pusch_td_list[td], last_non_srs_symbol)) {
      continue;
    }

    // The Resource manager doesn't assign a resource if the slot is not fully UL-enabled.
    const unsigned cg_offset = (slot_pdcch + pusch_td_list[td].k2).count() % cg_period_slots;
    candidate_resource       = res_mng.request_resource(cg_offset, u.crnti);
    if (not candidate_resource.has_value()) {
      continue;
    }

    // Check that there is room in the PUSCH list and on the grid for every CG PUSCH allocation from k2 until the end of
    // the UL grid window before committing to this candidate.
    if (are_grants_and_rbs_free(cell_alloc,
                                pusch_td_list,
                                cg_period_slots,
                                td,
                                candidate_resource.value().crbs,
                                cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs,
                                u.crnti)) {
      candidate_cg_offset.emplace(cg_offset);
      candidate_td_allocation.emplace(td);
      break;
    }

    // The resource is free as far as the resource manager is concerned, but the slots it would occupy are not. Return
    // it to the pool before moving on to the next TD candidate, or it is lost from the pool for good.
    res_mng.deallocate_resource(cg_offset, candidate_resource.value().id, u.crnti);
    candidate_resource.reset();
  }

  // No available resources.
  if (not candidate_cg_offset.has_value()) {
    // Do not stop the activation for all UEs at this point; \ref are_grants_and_rbs_free() can still fail a UE due to a
    // PUSCH allocated for it, which implies nothing for the other UEs.
    logger.info("ue={}: Failed to activate CG PUSCH type2. Cause: no usable PUSCH time domain resource",
                fmt::underlying(u.ue_index));
    return activation_outcome::skip_ue;
  }

  const unsigned     bwp_crb_start = cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.start();
  const vrb_interval vrbs = rb_helper::crb_to_vrb_ul_non_interleaved(candidate_resource.value().crbs, bwp_crb_start);
  const auto         mcs  = static_cast<uint8_t>(cell_cfg.params.init_bwp.cg_cfg.value().mcs);

  if (not alloc_activation_dci(pdcch_alloc, u, vrbs, candidate_td_allocation.value(), mcs)) {
    // The PDCCH list was checked by the caller, so a failure here is specific to this UE: either it monitors no
    // SearchSpace with DCI format 0_1, or its CQI calls for an aggregation level that does not fit in the CCEs left.
    res_mng.deallocate_resource(candidate_cg_offset.value(), candidate_resource.value().id, u.crnti);
    return activation_outcome::skip_ue;
  }

  pusch_config_params pusch_params = build_cg_pusch_cfg_params(u.get_pcell().cfg(), candidate_td_allocation.value());
  const units::bytes  tbs          = compute_ul_tbs_unsafe(pusch_params, sch_mcs_index{mcs}, vrbs.length());

  // NOTE: Set the first time CG transmission slot: this is to skip the allocations that fall in between PDCCH slot and
  // PDCCH slot + k2 (only applies to cases with cg_period_slots <= k2).
  const slot_point first_tx_slot = slot_pdcch + pusch_td_list[candidate_td_allocation.value()].k2;

  // Set "CG activated" (i.e., DCI has been sent) for the UE.
  u.get_pcell().get_conf_grant_state_manager().update_state(cg_period_slots,
                                                            tbs,
                                                            vrbs,
                                                            mcs,
                                                            candidate_td_allocation.value(),
                                                            candidate_cg_offset.value(),
                                                            candidate_resource.value().id,
                                                            first_tx_slot);

  // The UE must be added to the wheel at the same offset that was recorded in the CG state manager above, so that
  // cg_slot_wheel::rem_ue() removes exactly the entries inserted here.
  add_ue_to_wheel(u.get_pcell().cfg(), candidate_cg_offset.value());

  pending.on_activation_dci_allocated(slot_pdcch, first_tx_slot + max_expected_crc_delay);

  // NOTE: the HARQ reconfiguration is done during the UE reconfiguration, to avoid reconfiguring the HARQs while there
  // are in-flight PUSCHs.
  return activation_outcome::success;
}

configured_grant_type2_sched_impl::activation_outcome
configured_grant_type2_sched_impl::reactivate_ue(cell_resource_allocator& cell_alloc,
                                                 ue_pending_activation&   pending,
                                                 ue&                      u)
{
  auto&       pdcch_alloc     = cell_alloc[0U];
  auto&       ue_cg_state_mng = u.get_pcell().get_conf_grant_state_manager();
  const auto& cg_params       = ue_cg_state_mng.get_grant_params();
  ocudu_assert(
      cg_params.has_value(), "ue={}: CG Type 2 re-activation of a UE with no grant", fmt::underlying(u.ue_index));

  // The new DCI keeps the grant the UE may already be transmitting on, so it must land on the same CG offset and use
  // the same symbols, which the grid reservations follow. The TDRA row may differ, which gives more than one PDCCH
  // slot per CG period to send it from.
  const slot_point                                        slot_pdcch    = pdcch_alloc.slot;
  const span<const pusch_time_domain_resource_allocation> pusch_td_list = get_cg_pusch_td_list(u.get_pcell().cfg());
  const ofdm_symbol_range symbols             = pusch_td_list[cg_params->time_domain_allocation].symbols;
  const unsigned          last_non_srs_symbol = config_helpers::compute_last_non_srs_symbol(cell_cfg.params);
  std::optional<uint8_t>  td_alloc;
  for (uint8_t td = 0, sz = pusch_td_list.size(); td != sz; ++td) {
    const pusch_time_domain_resource_allocation& td_res   = pusch_td_list[td];
    const slot_point                             first_tx = slot_pdcch + td_res.k2;
    // NOTE: an earlier first transmission than the one the first-transmission gate holds is skipped, as the grid
    // reservations before that gate are not in place. This can happen in the case of dropped PDCCH due to error
    // indication plus with small CG periodicity.
    if (config_helpers::is_cg_pusch_td_res_usable(td_res, last_non_srs_symbol) and td_res.symbols == symbols and
        first_tx.count() % cg_params->periodicity_slots == cg_params->time_offset and
        not ue_cg_state_mng.is_before_first_tx(first_tx)) {
      td_alloc = td;
      break;
    }
  }
  if (not td_alloc.has_value()) {
    // No row lands on the CG offset from this slot; a later slot of the CG period will do.
    return activation_outcome::skip_ue;
  }

  if (not alloc_activation_dci(pdcch_alloc, u, cg_params->vrbs, *td_alloc, cg_params->mcs)) {
    return activation_outcome::skip_ue;
  }

  // The row has the same symbols as the stored one, so the TBS and the grid reservations stay valid.
  ue_cg_state_mng.set_time_domain_allocation(*td_alloc);

  pending.on_activation_dci_allocated(slot_pdcch, slot_pdcch + pusch_td_list[*td_alloc].k2 + max_expected_crc_delay);

  logger.debug("ue={}: Re-sent CG Type 2 activation DCI at slot={}", fmt::underlying(u.ue_index), slot_pdcch);
  return activation_outcome::success;
}

bool configured_grant_type2_sched_impl::alloc_activation_dci(cell_slot_resource_allocator& pdcch_alloc,
                                                             ue&                           u,
                                                             vrb_interval                  vrbs,
                                                             uint8_t                       td_alloc,
                                                             uint8_t                       mcs)
{
  ue_cell&                     ue_cc       = u.get_pcell();
  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();

  const search_space_info* ss_info = get_cg_activation_search_space(ue_cell_cfg);
  if (ss_info == nullptr) {
    logger.error(
        "ue={}: Failed to allocate CG DCI activation. Cause: no UE-specific SearchSpace monitored with DCI format 0_1",
        fmt::underlying(u.ue_index));
    return false;
  }

  const aggregation_level aggr_lvl =
      ue_cc.get_aggregation_level(ue_cc.link_adaptation_controller().get_effective_cqi(), *ss_info, false);
  ocudu_assert(u.ue_cfg_dedicated() != nullptr and u.ue_cfg_dedicated()->get_cs_rnti().has_value(),
               "UE={} with CG config doesn't have CS-RNTI allocated",
               fmt::underlying(u.ue_index));
  // The CS-RNTI only scrambles the DCI CRC: TS 38.213, Section 10.1 hashes the UE-specific SearchSpace candidates
  // with the C-RNTI whichever RNTI the CRC carries, so this is the ordinary UE PDCCH allocation.
  pdcch_ul_information* pdcch = pdcch_sch.alloc_ul_pdcch_ue(
      pdcch_alloc, u.ue_cfg_dedicated()->get_cs_rnti().value(), ue_cell_cfg, ss_info->cfg->get_id(), aggr_lvl);
  if (pdcch == nullptr) {
    logger.info("ue={} rnti={}: Failed to activate CG type2. Cause: No space in PDCCH for DCI activation",
                fmt::underlying(u.ue_index),
                u.crnti);
    return false;
  }

  // The DCI size is derived from the SearchSpace it is sent in, so it must be the one the PDCCH was allocated in.
  build_dci_f0_1_cg_activation(pdcch->dci, ue_cell_cfg, ss_info->cfg->get_id(), vrbs, td_alloc, sch_mcs_index{mcs});
  return true;
}

void configured_grant_type2_sched_impl::handle_error_indication(slot_point                            sl_tx,
                                                                scheduler_slot_handler::error_outcome event)
{
  if (not event.pdcch_discarded) {
    return;
  }

  // A CG occasion needs no PDCCH of its own, so the only PDCCH this scheduler puts on the grid is the activation DCI.
  // If it was discarded, the UE never received it, so re-activate the UE without waiting for the confirmation deadline.
  for (ue_pending_activation& u : ues_pending_activation) {
    if (u.on_pdcch_discarded(sl_tx)) {
      logger.info("ue={}: Re-activating CG Type 2. Cause: the activation PDCCH of slot={} was discarded by the lower "
                  "layers",
                  fmt::underlying(u.ue_index()),
                  sl_tx);
    }
  }
}

void configured_grant_type2_sched_impl::handle_cg_activation_complete_indication(du_ue_index_t ue_idx)
{
  const auto it = find_pending(ue_idx);
  if (it == ues_pending_activation.end()) {
    // This UE has been already removed from the UE pending activation list.
    return;
  }

  if (not it->can_be_confirmed()) {
    // No activation DCI has been sent for the UE's current CG configuration, so this confirms an earlier one.
    logger.warning("ue={}: Ignoring CG activation confirmation. Cause: no activation DCI was sent for the current CG "
                   "configuration",
                   fmt::underlying(ue_idx));
    return;
  }

  // Every activation DCI sent to the UE carries the same grant, so the confirmation holds whichever DCI it answers.
  logger.info("ue={}: CG activation notified", fmt::underlying(ue_idx));

  std::swap(*it, ues_pending_activation.back());
  ues_pending_activation.pop_back();
}
