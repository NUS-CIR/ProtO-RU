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

static unsigned get_max_k2(span<const pusch_time_domain_resource_allocation> pusch_td_alloc_list)
{
  unsigned max_k2 = 0;
  for (const pusch_time_domain_resource_allocation& td : pusch_td_alloc_list) {
    max_k2 = std::max(max_k2, static_cast<unsigned>(td.k2));
  }
  return max_k2;
}

static bool are_grants_and_rbs_free(cell_resource_allocator&                          cell_alloc,
                                    span<const pusch_time_domain_resource_allocation> pusch_td_list,
                                    unsigned                                          cg_period_slots,
                                    uint8_t                                           td_alloc,
                                    const crb_interval&                               crbs,
                                    subcarrier_spacing                                scs)
{
  const unsigned max_k2 = get_max_k2(pusch_td_list);
  // In the worst case scenario, a dynamic PUSCH (which are allocated after CG PUSCHs) could have been allocated in the
  // previous slot with max k2; check the grid and grant list availability up until max_k2 - 1 slots from the current
  // one.
  for (unsigned n = 0U; n < max_k2; n += cg_period_slots) {
    auto& pdcch_alloc = cell_alloc[pusch_td_list[td_alloc].k2 + n];

    // Check whether there is space for another PUSCH in the list.
    if (pdcch_alloc.result_builder().ul().puschs().full()) {
      return false;
    }

    // Check whether the REs/symbols on the grid are available.
    if (pdcch_alloc.ul_res_grid.collides(scs, pusch_td_list[td_alloc].symbols, crbs)) {
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

  auto* u = ues.find_by_rnti(ue_cfg.crnti);
  if (u == nullptr) {
    // The offset and period the UE was added to the wheel with live in its CG state manager, which is gone with the UE
    // context, so fall back to a full sweep. Leaving entries behind would keep producing CG PUSCH allocations for an
    // RNTI that no longer exists, or for whichever UE is assigned that RNTI next.
    const unsigned nof_removed = periodic_pusch_slot_wheel.rem_ue_everywhere(ue_cfg.crnti);
    logger.error("rnti={}: UE not found in the CG scheduler UEs repo during UE removal. Swept {} stale CG grants from "
                 "the slot wheel; its CG resource and pending activation entry cannot be reclaimed",
                 ue_cfg.crnti,
                 nof_removed);
    return;
  }

  // UE only added to the UEs pending activation list.
  auto ue_to_rem_it =
      std::find_if(ues_pending_activation.begin(),
                   ues_pending_activation.end(),
                   [ue_idx = u->ue_index](const ue_pending_activation& ue_it) { return ue_idx == ue_it.ue_index; });
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

  // Return the CG resource acquired at activation time to the resource manager pool. Note that UEs whose activation
  // already timed out have been released by ues_resource_release(), which also erased their grant info; those return
  // above, so the resource cannot be deallocated twice.
  res_mng.deallocate_resource(cg_params->time_offset, cg_params->resource_id, ue_cfg.crnti);

  // Remove the UE from the periodic wheel using the CG offset recorded at activation time, so that the entries removed
  // are exactly the ones add_ue_to_wheel() inserted.
  // It is possible that the UE has not been added to the wheel yet, e.g. if it is still pending activation, so a
  // missing entry is not reported as an error here.
  (void)periodic_pusch_slot_wheel.rem_ue(ue_cfg.crnti, cg_params->time_offset);

  // This is the case of UEs that started the activation, which, however, failed; they had been already removed from the
  // periodic wheel.
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
    // CG configuration unchanged — nothing to do. Most reconfigurations a UE goes through do not touch its CG, and
    // tearing the grant down for those would cost it a re-activation round trip, a gap in its CG occasions and a move
    // to a different slot offset and set of RBs, all for no reason.
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

  // Release resources of UEs that initiated activation, but didn't ack the DCI reception or started CG transmission.
  // These UEs will be re-activated below.
  ues_resource_release(pdcch_slot);

  // Scan the UEs that are ready for activation and allocate the activation DCI.
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

void configured_grant_type2_sched_impl::on_cg_pusch_allocated(const ue& u, slot_point pusch_slot)
{
  // Save slots at which the UE has non-zero UL pending bytes, which will be used for missing activation detection.
  if (not u.logical_channels().has_ul_pending_bytes()) {
    return;
  }

  auto it =
      std::find_if(ues_pending_activation.begin(),
                   ues_pending_activation.end(),
                   [ue_idx = u.ue_index](const ue_pending_activation& ue_it) { return ue_idx == ue_it.ue_index; });
  if (it != ues_pending_activation.end() and it->slots_expecting_crc.size() != it->slots_expecting_crc.capacity()) {
    const size_t first_available = it->slots_expecting_crc.find_first_empty();
    it->slots_expecting_crc.emplace(first_available, pusch_slot);
  }
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
    // This handle the case of a UE that has an active CG configured and that goes through a reconfiguration (in which
    // it preserve its CG config, without modifying it). Starting a second activation here would add the UE to the
    // periodic wheel at a second offset, doubling its CG PUSCH rate, and would leak the resource acquired by the first
    // one.
    // A reconfiguration that actually changes the CG goes through add_reconf_ue() -> rem_ue(), which erases the
    // grant info, so the legitimate re-activation is not affected by this check.
    return;
  }

  auto ue_it = std::find_if(ues_pending_activation.begin(), ues_pending_activation.end(), [ue_idx](const auto& elem) {
    return elem.ue_index == ue_idx;
  });
  if (ue_it == ues_pending_activation.end()) {
    // TODO: add reserve in the constructor.
    ues_pending_activation.emplace_back(ue_idx);
  }
}

void configured_grant_type2_sched_impl::run_ues_activation(cell_resource_allocator& cell_alloc)
{
  const slot_point pdcch_slot = cell_alloc[0U].slot;

  if (not cell_cfg.is_dl_enabled(pdcch_slot)) {
    return;
  }

  for (size_t i = 0; i != ues_pending_activation.size();) {
    ue_pending_activation& u = ues_pending_activation[i];

    // Drop the entries of UEs that are no longer in the repository. rem_ue() normally takes a UE out of this list,
    // but it cannot do so when the UE context is already gone by the time it runs. Such an entry must not be left
    // behind: activate_ue() would keep failing on it, and the break below would stop every UE queued after it from
    // ever being activated.
    if (ues.find(u.ue_index) == nullptr) {
      logger.error("ue={}: Dropping stale CG Type 2 activation. Cause: UE not found in the CG scheduler UEs repo",
                   fmt::underlying(u.ue_index));
      std::swap(u, ues_pending_activation.back());
      ues_pending_activation.pop_back();
      continue;
    }

    // Activate UEs with no activation outstanding: either they have never been activated, or a previous attempt was
    // abandoned by release_ue_activation() because the UE gave no sign of having received the activation DCI.
    if (not u.activation_triggered) {
      bool cg_activated = activate_ue(cell_alloc, u.ue_index);
      if (not cg_activated) {
        // Failure due to lack of PDCCH space or CG resources would block all UEs. Stop here.
        break;
      }
      u.activation_triggered  = cg_activated;
      u.activation_pdcch_slot = pdcch_slot;
    }
    ++i;
  }
}

void configured_grant_type2_sched_impl::ues_resource_release(slot_point slot)
{
  // A UE that had CG PUSCH occasions allocated while it had UL data to send, and for which no CRC indication was ever
  // reported for any of them, likely never received the activation DCI; release its resources prior to activating it
  // again. Note that a CRC with either OK or KO confirms the activation, since both mean the UE did transmit.
  for (auto& u : ues_pending_activation) {
    if (not u.activation_triggered) {
      continue;
    }

    // Note: iterate by index rather than by iterator, since erase() trims the trailing empty slots of the container
    // and would invalidate the loop's end bound.
    for (size_t i = 0; i != u.slots_expecting_crc.capacity(); ++i) {
      if (u.slots_expecting_crc.contains(i) and slot > u.slots_expecting_crc[i] + max_expected_crc_delay) {
        ++u.missed_grants_cnt;
        (void)u.slots_expecting_crc.erase(i);
      }
    }

    if (u.missed_grants_cnt < u.max_nof_missed_crc_events) {
      continue;
    }

    // "UE missed activation" detected: the UE accumulated max_nof_missed_crc_events missing CRCs for slots in which
    // CRCs were expected.
    release_ue_activation(u);
  }
}

void configured_grant_type2_sched_impl::release_ue_activation(ue_pending_activation& u)
{
  auto* ue_to_reset = ues.find(u.ue_index);
  if (ue_to_reset == nullptr) {
    logger.error("ue={}: UE not found in the CG scheduler UEs repo while releasing it from activation",
                 fmt::underlying(u.ue_index));
    return;
  }

  auto&       ue_cg_state_mng = ue_to_reset->get_pcell().get_conf_grant_state_manager();
  const auto& cg_params       = ue_cg_state_mng.get_grant_params();
  if (cg_params.has_value()) {
    res_mng.deallocate_resource(cg_params->time_offset, cg_params->resource_id, ue_to_reset->crnti);

    // Remove the UE from the periodic wheel, as it will be assign a new resource and CG offset.
    const auto* u_to_rem = ues.find(u.ue_index);
    ocudu_assert(u_to_rem != nullptr, "UE={} not found in scheduler UE repo", u.ue_index);
    (void)periodic_pusch_slot_wheel.rem_ue(u_to_rem->crnti, cg_params->time_offset);
  }
  // Set the UE as "not CG activated".
  ue_cg_state_mng.reset_state();
  u.activation_triggered  = false;
  u.activation_pdcch_slot = {};
  u.slots_expecting_crc.clear();
  // Reset the miss counter, so that the next activation attempt starts from a clean state. Without this, the UE would
  // be released again on the very next slot, as the counter would still be at its threshold.
  u.missed_grants_cnt = 0;
}

bool configured_grant_type2_sched_impl::activate_ue(cell_resource_allocator& cell_alloc, du_ue_index_t ue_index)
{
  auto* u = ues.find(ue_index);
  if (u == nullptr) {
    // run_ues_activation() drops the pending entries of UEs that are gone before getting here, so reaching this means
    // the UE disappeared in between; there is nothing to activate.
    logger.error("ue={}: Failed to activate CG PUSCH type2. Cause: UE not found in the CG scheduler UEs repo",
                 fmt::underlying(ue_index));
    return false;
  }

  if (res_mng.get_nof_free_resources() == 0) {
    logger.error("ue={}: Failed to activate CG PUSCH type2. Cause: no available resources", fmt::underlying(ue_index));
    return false;
  }

  auto&       pdcch_alloc   = cell_alloc[0U];
  const auto  slot_pdcch    = pdcch_alloc.slot;
  const auto& pusch_td_list = cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common.value().pusch_td_alloc_list;

  // For type2 CG, the TD allocation is not defined yet at this point; the scheduler will choose the symbols that do
  // not collide with SRS. In the following, we proceed under that assumption.

  // Compute PUSCH symbols to avoid overlapping with SRS.
  const ofdm_symbol_range non_srs_symbols =
      cell_cfg.params.init_bwp.srs_cfg.srs_type_enabled != srs_type::disabled
          ? ofdm_symbol_range{0,
                              NOF_OFDM_SYM_PER_SLOT_NORMAL_CP -
                                  cell_cfg.params.init_bwp.srs_cfg.max_nof_symbols.value()}
          : ofdm_symbol_range{0, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP};

  // Find k2 for which there is a CG resource available.
  std::optional<uint8_t>           candidate_td_allocation;
  std::optional<cg_type2_resource> candidate_resource;
  std::optional<unsigned>          candidate_cg_offset;

  const auto cg_period_slots =
      static_cast<unsigned>(u->get_pcell().cfg().init_bwp().ul.ded()->cg_cfg.value().periodicity);

  // The CG offset below is a phase within the UE CG period, but cg_type2_resource_manager tracks resource occupancy
  // in a wheel sized by the cell CG period. If the two differed, a UE whose period is shorter would recur on offsets
  // the resource manager never marks as used, and two UEs could be handed the same resource on the same slot.
  ocudu_assert(cg_period_slots == static_cast<unsigned>(cell_cfg.params.init_bwp.cg_cfg.value().periodicity),
               "ue={}: CG period of {} slots differs from the cell CG period of {} slots",
               fmt::underlying(ue_index),
               cg_period_slots,
               static_cast<unsigned>(cell_cfg.params.init_bwp.cg_cfg.value().periodicity));

  for (uint8_t td = 0, sz = pusch_td_list.size(); td != sz; ++td) {
    // Make sure the CG PUSCH symbols do not collide with the SRS ones.
    if (pusch_td_list[td].symbols.stop() > non_srs_symbols.stop()) {
      continue;
    }

    // The Resource manager doesn't assign a resource if the slot not fully UL-enabled.
    const unsigned cg_offset = (slot_pdcch + pusch_td_list[td].k2).count() % cg_period_slots;
    candidate_resource       = res_mng.request_resource(cg_offset, u->crnti);
    if (not candidate_resource.has_value()) {
      continue;
    }

    // The resource manager only tracks the CG resources against one another, so it can hand out one whose slots are
    // already taken by dynamic PUSCHs scheduled in an earlier slot. Check that there is room in the PUSCH list and on
    // the grid for every CG PUSCH allocation from k2 until max_k2 before committing to this candidate.
    if (are_grants_and_rbs_free(cell_alloc,
                                pusch_td_list,
                                cg_period_slots,
                                td,
                                candidate_resource.value().crbs,
                                cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs)) {
      candidate_cg_offset.emplace(cg_offset);
      candidate_td_allocation.emplace(td);
      break;
    }

    // The resource is free as far as the resource manager is concerned, but the slots it would occupy are not. Return
    // it to the pool before moving on to the next TD candidate, or it is lost from the pool for good.
    res_mng.deallocate_resource(cg_offset, candidate_resource.value().id, u->crnti);
    candidate_resource.reset();
  }

  // No available resources.
  // NOTE: this tests the offset rather than the resource, as only the success path above sets it. \c
  // candidate_resource is written on every iteration, so it can hold a candidate that was rejected by the grid check.
  if (not candidate_cg_offset.has_value()) {
    return false;
  }

  // Check if DCI can be allocated.
  pdcch_ul_information* pdcch = alloc_pdcch_ul_information(pdcch_alloc, ue_index);

  if (pdcch == nullptr) {
    res_mng.deallocate_resource(candidate_cg_offset.value(), candidate_resource.value().id, u->crnti);
    return false;
  }

  // NOTE: existence was already validated by alloc_pdcch_ul_information(), which allocated the PDCCH in this
  // SearchSpace; the DCI must be built against the same one, as its size depends on it.
  const search_space_info& ss_info       = *get_cg_activation_search_space(u->get_pcell().cfg());
  const unsigned           bwp_crb_start = cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.start();
  const vrb_interval       vrbs          = rb_helper::crb_to_vrb_ul_non_interleaved(
      crb_interval{candidate_resource.value().crbs.start(), candidate_resource.value().crbs.stop()}, bwp_crb_start);

  const auto mcs = static_cast<uint8_t>(cell_cfg.params.init_bwp.cg_cfg.value().mcs);
  // The DCI size is derived from the SearchSpace it is sent in, and get_cg_activation_search_space() only returns
  // SearchSpaces monitored with DCI format 0_1.
  build_dci_f0_1_cg_activation(pdcch->dci,
                               u->get_pcell().cfg(),
                               ss_info.cfg->get_id(),
                               vrbs,
                               candidate_td_allocation.value(),
                               sch_mcs_index{mcs});

  pusch_config_params pusch_params = build_cg_pusch_cfg_params(u->get_pcell().cfg(), candidate_td_allocation.value());
  const units::bytes  tbs          = compute_ul_tbs_unsafe(pusch_params, sch_mcs_index{mcs}, vrbs.length());

  // Set "CG activated" (i.e., DCI has been sent) for the UE.
  u->get_pcell().get_conf_grant_state_manager().update_state(cg_period_slots,
                                                             tbs,
                                                             vrbs,
                                                             mcs,
                                                             candidate_td_allocation.value(),
                                                             candidate_cg_offset.value(),
                                                             candidate_resource.value().id);

  // The UE must be added to the wheel at the same offset that was recorded in the CG state manager above, so that
  // cg_slot_wheel::rem_ue() removes exactly the entries inserted here.
  add_ue_to_wheel(u->get_pcell().cfg(), candidate_cg_offset.value());

  // NOTE: the HARQ reconfiguration is done during the reconfiguration /ref add_reconf_ue(), to avoid reconfiguring the
  // HARQs while there are in-flight PUSCHs.
  return true;
}

pdcch_ul_information*
configured_grant_type2_sched_impl::alloc_pdcch_ul_information(cell_slot_resource_allocator& pdcch_alloc,
                                                              du_ue_index_t                 ue_idx) const
{
  if (pdcch_alloc.result_builder().dl().ul_pdcchs().full()) {
    // Early exit. No space for new PDCCH grants in the scheduler output.
    logger.info("ue={}: Failed to allocate CG DCI activation. Cause: Max PDCCH grants reached for slot={}",
                ue_idx,
                pdcch_alloc.slot);
    return nullptr;
  }

  ue&                          u           = ues[ue_idx];
  ue_cell&                     ue_cc       = *u.find_cell(cell_cfg.cell_index);
  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();

  const search_space_info* ss_info = get_cg_activation_search_space(ue_cell_cfg);
  if (ss_info == nullptr) {
    logger.error(
        "ue={}: Failed to allocate CG DCI activation. Cause: no UE-specific SearchSpace monitored with DCI format 0_1",
        ue_idx);
    return nullptr;
  }

  const aggregation_level aggr_lvl =
      ue_cc.get_aggregation_level(ue_cc.link_adaptation_controller().get_effective_cqi(), *ss_info, false);
  ocudu_assert(u.ue_cfg_dedicated() != nullptr and u.ue_cfg_dedicated()->get_cs_rnti().has_value(),
               "UE={} with CG config doesn't have CS-RNTI allocated",
               ue_idx);
  // The CS-RNTI only scrambles the DCI CRC: TS 38.213, Section 10.1 hashes the UE-specific SearchSpace candidates
  // with the C-RNTI whichever RNTI the CRC carries, so this is the ordinary UE PDCCH allocation.
  pdcch_ul_information* pdcch = pdcch_sch.alloc_ul_pdcch_ue(
      pdcch_alloc, u.ue_cfg_dedicated()->get_cs_rnti().value(), ue_cell_cfg, ss_info->cfg->get_id(), aggr_lvl);

  if (pdcch == nullptr) {
    logger.info("ue={} rnti={}: Failed to activate CG type2. Cause: No space in PDCCH for DCI activation",
                fmt::underlying(u.ue_index),
                u.crnti);
  }
  return pdcch;
}

void configured_grant_type2_sched_impl::handle_error_indication(slot_point                            sl_tx,
                                                                scheduler_slot_handler::error_outcome event)
{
  if (event.pdcch_discarded) {
    // A CG occasion needs no PDCCH of its own, so the only PDCCH this scheduler puts on the grid is the activation
    // DCI. If it was discarded, the UE never learnt about the grant and will never transmit on it, so no CRC will ever
    // arrive to reveal that. Waiting for the missing-CRC detector would not help either: it only arms while the UE has
    // pending UL bytes. Invalidate the attempt now, so that run_ues_activation() retries it in this same slot.
    for (ue_pending_activation& u : ues_pending_activation) {
      if (u.activation_pdcch_slot.valid() and u.activation_pdcch_slot == sl_tx) {
        logger.info("ue={}: Retrying CG Type 2 activation. Cause: the activation PDCCH of slot={} was discarded by the "
                    "lower layers",
                    fmt::underlying(u.ue_index),
                    sl_tx);
        release_ue_activation(u);
      }
    }
  }

  // Only a discarded PUSCH means no CRC indication will be reported for the slot. When just the PDCCH was discarded,
  // the CG PUSCH still reached the PHY, so a CRC is still expected.
  if (not event.pusch_and_pucch_discarded) {
    return;
  }

  // The CG PUSCH of this slot never reached the PHY, so its missing CRC indication says nothing about whether the UE
  // received the activation. Drop the expectation, otherwise it would count towards a spurious re-activation.
  for (ue_pending_activation& u : ues_pending_activation) {
    auto it = std::find(u.slots_expecting_crc.begin(), u.slots_expecting_crc.end(), sl_tx);
    if (it != u.slots_expecting_crc.end()) {
      u.slots_expecting_crc.erase(it);
      logger.debug("ue={}: Dropped expected CG CRC for slot={}. Cause: PUSCH was discarded by the lower layers",
                   fmt::underlying(u.ue_index),
                   sl_tx);
    }
  }
}

void configured_grant_type2_sched_impl::handle_cg_activation_complete_indication(du_ue_index_t ue_idx)
{
  const auto it =
      std::find_if(ues_pending_activation.begin(),
                   ues_pending_activation.end(),
                   [ue_idx](const ue_pending_activation& ue_to_rem) { return ue_idx == ue_to_rem.ue_index; });
  if (it == ues_pending_activation.end()) {
    // This UE has been already removed from the UE pending activation list.
    return;
  }

  if (not it->activation_triggered) {
    // The activation this confirms was already given up on by release_ue_activation(), which inferred from the CRCs
    // missing on the UE's CG PUSCHs that the UE had never received the activation DCI. The confirmation proves that
    // inference wrong: the UE did receive it and is transmitting on a CG resource that has since been returned to the
    // pool and may already have been handed to another UE. Nothing here can stop that, as the scheduler has no
    // deactivation DCI to send, so report it and leave the entry queued: re-activating the UE is what ends the
    // overlap, whereas dropping the entry here would strand it on the stale resource for good.
    // NOTE: a UE whose UL SINR is persistently too low for its CG PUSCHs to decode reaches this while receiving the
    // downlink perfectly well, so this is not limited to a UE that is losing PDCCHs.
    logger.error("ue={}: CG activation confirmed after its activation attempt had been released. Cause: the UE did "
                 "receive the activation DCI, but no CRC arrived in time for any of its CG PUSCHs",
                 fmt::underlying(ue_idx));
    return;
  }

  logger.info("ue={}: CG activation notified", fmt::underlying(ue_idx));

  std::swap(*it, ues_pending_activation.back());
  ues_pending_activation.pop_back();
}
