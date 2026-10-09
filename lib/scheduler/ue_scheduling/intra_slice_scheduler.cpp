// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "intra_slice_scheduler.h"
#include "../logging/cell_metrics_handler.h"
#include "../ue_context/ue_cell_repository.h"
#include "ocudu/ran/pdcch/search_space.h"
#include "ocudu/support/math/mod_math_utils.h"

using namespace ocudu;

/// \brief Helper function to determine the expected number of PDSCHs that can be allocated per slot in a manner that
/// ensures fair distribution of PDSCHs across slots.
///
/// To ensure fair distribution of PDSCHs across slots, we need to consider the number of UL slots available and how
/// many PUCCHs can be scheduled in each of the UL slots.
static unsigned compute_expected_pdschs_per_slot(const cell_configuration& cell_cfg)
{
  unsigned pdschs_per_slot =
      std::min(static_cast<unsigned>(MAX_PDSCH_PDUS_PER_SLOT), cell_cfg.expert_cfg.ue.max_pdschs_per_slot);
  // Note: We subtract to -1 to max_ul_grants_per_slot to always leave a grant for PUSCH which gets scheduled
  // afterwards.
  const unsigned max_pucchs_per_slot =
      std::min(cell_cfg.expert_cfg.ue.max_pucchs_per_slot, cell_cfg.expert_cfg.ue.max_ul_grants_per_slot - 1);

  if (cell_cfg.is_tdd()) {
    // TDD.
    const unsigned nof_dl_slots = nof_dl_slots_per_tdd_period(*cell_cfg.params.tdd_cfg);
    // Note: use all UL slots once partial UL slots are used for UCI.
    const unsigned nof_ul_slots = nof_full_ul_slots_per_tdd_period(*cell_cfg.params.tdd_cfg);
    const unsigned max_pucchs   = nof_ul_slots * max_pucchs_per_slot;
    pdschs_per_slot             = std::min(pdschs_per_slot, max_pucchs / nof_dl_slots);
  } else {
    // FDD.
    pdschs_per_slot = std::min(max_pucchs_per_slot, pdschs_per_slot);
  }

  pdschs_per_slot = std::max(1U, pdschs_per_slot);
  return pdschs_per_slot;
}

/// \brief Determines whether there is any pending UL retx in the cell, or any UE with pending UL newTx data or SR.
/// \remark UEs with pending UL newTx data or SR are not filtered by cell, to avoid traversing them.
static bool has_pending_ul(const ue_repository& ues, cell_harq_manager& cell_harqs, const cell_configuration& cell_cfg)
{
  auto retxs = cell_harqs.pending_ul_retxs();
  if (retxs.begin() != retxs.end()) {
    return true;
  }
  // Note: The RAN slice IDs follow the ones created by the inter-slice scheduler, i.e. SRB and default DRB slices,
  // followed by one slice per RRM policy member.
  const unsigned nof_slices = 2 + cell_cfg.rrm_policy_members.size();
  for (unsigned i = 0; i != nof_slices; ++i) {
    if (ues.get_ues_with_pending_newtx_data(ran_slice_id_t{static_cast<uint8_t>(i)}, false).any()) {
      return true;
    }
  }
  return false;
}

// class intra_slice_scheduler::slice_ue_group_scheduler

void intra_slice_scheduler::slice_ue_group_scheduler::fill_ue_dl_candidate_group(
    std::vector<ue_newtx_candidate>& candidates,
    const dl_ran_slice_candidate&    slice)
{
  fill_ue_candidate_group(candidates, true, slice.get_slice_ues());
}

void intra_slice_scheduler::slice_ue_group_scheduler::fill_ue_ul_candidate_group(
    std::vector<ue_newtx_candidate>& candidates,
    const ul_ran_slice_candidate&    slice)
{
  fill_ue_candidate_group(candidates, false, slice.get_slice_ues());
}

void intra_slice_scheduler::slice_ue_group_scheduler::fill_ue_candidate_group(
    std::vector<ue_newtx_candidate>& candidates,
    bool                             is_dl,
    const slice_ue_repository&       slice_ues)
{
  candidates.clear();

  const unsigned nof_ues = slice_ues.size();
  if (nof_ues > parent->expert_cfg.pre_policy_rr_ue_group_size) {
    if (not last_pdcch_tx.valid() or last_pdcch_tx.sfn() != parent->pdcch_slot.sfn()) {
      // Update group_offset whenever we enter a new SFN.
      // The new group offset is chosen as the UE index after the last UE index considered in the previous group.
      // > Compute max ue index + 1, which will be used to perform the wrap-around of the group offset.
      const unsigned ue_idx_mod = (--slice_ues.end())->ue_index() + 1;
      // > Given that DL and UL UE groups are independent, we choose the minimum of the two last UE indexes, to ensure
      // that no UE is left unconsidered in any direction.
      auto jump = std::max(std::min(max_dl_ue_count, max_ul_ue_count), parent->expert_cfg.pre_policy_rr_ue_group_size);
      group_offset    = (group_offset + jump) % ue_idx_mod;
      max_dl_ue_count = 0;
      max_ul_ue_count = 0;
    }
  } else {
    // The number of UEs is less than the group size, so we consider all UEs from the beginning.
    group_offset = 0;
  }
  last_pdcch_tx = parent->pdcch_slot;

  // Build list of candidates, starting from the UE group offset.
  unsigned                             count                = 0;
  unsigned                             last_candidate_count = 0;
  const unsigned                       group_size = std::min(nof_ues, parent->expert_cfg.pre_policy_rr_ue_group_size);
  const bounded_bitset<MAX_NOF_DU_UES> ues_with_data = slice_ues.get_ues_with_pending_newtx_data(is_dl);
  for (unsigned sz = ues_with_data.size(), ueidx = group_offset % sz; count != sz; ++ueidx, ++count) {
    if (ueidx == sz) {
      // wrap-around.
      ueidx = 0;
    }
    if (not ues_with_data.test(ueidx)) {
      // No pending data for this UE in this slice.
      continue;
    }

    auto ue_it = slice_ues.find(to_du_ue_index(ueidx));
    if (ue_it == slice_ues.end()) {
      // ues_with_data may return true due to pending SR/CE even if the slice is not active.
      continue;
    }
    std::optional<ue_newtx_candidate> ue_candidate =
        is_dl ? parent->create_newtx_dl_candidate(*ue_it) : parent->create_newtx_ul_candidate(*ue_it);
    if (ue_candidate.has_value()) {
      candidates.push_back(ue_candidate.value());
      last_candidate_count = count;
      if (candidates.size() >= group_size) {
        break;
      }
    }
  }

  // Update the next UE group offset, if the last UE index went beyond next group offset.
  auto& max_count = is_dl ? max_dl_ue_count : max_ul_ue_count;
  max_count       = std::max(max_count, last_candidate_count);
}

// class intra_slice_scheduler

intra_slice_scheduler::intra_slice_scheduler(const scheduler_ue_expert_config& expert_cfg_,
                                             ue_repository&                    ues_,
                                             ue_cell_repository&               cell_ues,
                                             pdcch_resource_allocator&         pdcch_alloc,
                                             uci_allocator&                    uci_alloc_,
                                             srs_allocator&                    srs_alloc_,
                                             cell_resource_allocator&          cell_alloc_,
                                             cell_metrics_handler&             cell_metrics_,
                                             ocudulog::basic_logger&           logger_) :
  expert_cfg(expert_cfg_),
  cell_alloc(cell_alloc_),
  cell_metrics(cell_metrics_),
  cell_harqs(cell_ues.get_cell_harqs()),
  uci_alloc(uci_alloc_),
  srs_alloc(srs_alloc_),
  ues(ues_),
  logger(logger_),
  expected_pdschs_per_slot(compute_expected_pdschs_per_slot(cell_alloc.cfg)),
  ue_alloc(expert_cfg, ues, pdcch_alloc, uci_alloc, srs_alloc, cell_alloc_, logger_),
  pdcch_cce_budget(cell_alloc_)
{
  // Pre-reserve memory for UE candidates.
  newtx_candidates.reserve(MAX_NOF_DU_UES);
  pending_dl_newtxs.reserve(MAX_UE_PDUS_PER_SLOT);
  pending_ul_newtxs.reserve(MAX_UE_PDUS_PER_SLOT);

  // Pre-reserve memory for slice context.
  constexpr unsigned max_expected_ran_slices = 8;
  slice_ctxt_list.reserve(max_expected_ran_slices);
}

void intra_slice_scheduler::slot_indication(slot_point sl_tx)
{
  pdcch_slot = sl_tx;
  // Reset slots to ensure used RB bitmaps are updated when scheduling the next slot, as allocations could have been
  // made by other scheduler components in the meantime (e.g., fallback scheduler).
  pdsch_slot.clear();
  pusch_slot.clear();
  dl_attempts_count = 0;
  ul_attempts_count = 0;
  pdcch_cce_budget.slot_indication(sl_tx, has_pending_ul(ues, cell_harqs, cell_alloc.cfg));
}

void intra_slice_scheduler::post_process_results()
{
  ue_alloc.post_process_results();
}

void intra_slice_scheduler::dl_sched(dl_ran_slice_candidate slice, scheduler_policy& policy)
{
  ocudu_sanity_check(slice.remaining_rbs() > 0, "Invalid slice");
  if (slice.get_slice_ues().empty()) {
    return;
  }

  if (slice.get_slot_tx() != pdsch_slot) {
    pdsch_slot = slice.get_slot_tx();
    update_used_dl_vrbs(slice);
  }

  // Determine max number of UE grants that can be scheduled in this slot.
  unsigned pdschs_to_alloc = max_pdschs_to_alloc(slice);
  if (pdschs_to_alloc == 0) {
    return;
  }

  // Schedule reTxs.
  unsigned nof_retxs_alloc = schedule_dl_retx_candidates(slice, pdschs_to_alloc);
  pdschs_to_alloc -= std::min(pdschs_to_alloc, nof_retxs_alloc);
  if (pdschs_to_alloc == 0) {
    return;
  }

  // Allocate UE newTx grants.
  schedule_dl_newtx_candidates(slice, policy, pdschs_to_alloc);
}

void intra_slice_scheduler::ul_sched(ul_ran_slice_candidate slice, scheduler_policy& ul_policy)
{
  ocudu_sanity_check(slice.remaining_rbs() > 0, "Invalid slice slice");
  if (slice.get_slice_ues().empty()) {
    return;
  }

  // The used VRBs follow from the symbols of the candidate k2, so a candidate reaching the same slot from another k2
  // needs them computed again.
  if (slice.get_slot_tx() != pusch_slot or slice.get_k2() != pusch_k2) {
    pusch_slot        = slice.get_slot_tx();
    pusch_k2          = slice.get_k2();
    pusch_ntn_koffset = slice.get_ntn_koffset();
    update_used_ul_vrbs(slice);
  }

  // Determine max number of UE grants that can be scheduled in this slot.
  unsigned puschs_to_alloc = max_puschs_to_alloc(slice);
  if (puschs_to_alloc == 0) {
    return;
  }

  // Schedule reTxs.
  unsigned nof_retxs_alloc = schedule_ul_retx_candidates(slice, puschs_to_alloc);
  puschs_to_alloc -= std::min(puschs_to_alloc, nof_retxs_alloc);
  if (puschs_to_alloc == 0) {
    return;
  }

  // Allocate UE newTx grants.
  schedule_ul_newtx_candidates(slice, ul_policy, puschs_to_alloc);
}

/// \brief Helper function that returns a pair with the remaining number of RBs to allocate in this slice scheduling
/// opportunity and the recommended number of RBs per UE grant.
template <typename SliceCandidate>
static std::pair<unsigned, unsigned> get_max_grants_and_rb_grant_size(span<const ue_newtx_candidate>  ue_candidates,
                                                                      const pdcch_cce_budget_tracker& pdcch_cce_budget,
                                                                      const SliceCandidate&           slice,
                                                                      const vrb_bitmap&               used_vrbs,
                                                                      unsigned max_ue_grants_to_alloc)
{
  constexpr bool is_dl = std::is_same_v<dl_ran_slice_candidate, SliceCandidate>;
  static_assert(is_dl or std::is_same_v<ul_ran_slice_candidate, SliceCandidate>, "Invalid slice candidate");

  unsigned ues_to_alloc = max_ue_grants_to_alloc;

  // [Implementation-defined] We use the same searchSpace config to determine the number of RBs available.
  const ue_cell_configuration& ue_cfg  = ue_candidates[0].ue->get_cc().cfg();
  const search_space_id        ss_id   = ue_cfg.init_bwp().dl.pdcch().search_spaces()[to_search_space_id(2)]->id();
  const auto*                  ss_info = ue_cfg.find_search_space(ss_id);
  if (ss_info == nullptr) {
    return std::make_pair(0, 0);
  }

  // Determine how many UE grants to allocate in the slot (assuming full buffer). As a heuristic, we divide the number
  // of candidates by 4 and set 8 as the maximum number of UEs to be scheduled per slot, assuming full buffer. This
  // heuristic is a result of a tradeoff between minimizing latency and ensuring we don't deplete the PDCCH resources.
  static constexpr unsigned MAX_UE_GRANT_PER_SLOT = 8;
  ues_to_alloc =
      std::min({ues_to_alloc, std::max(static_cast<unsigned>(ue_candidates.size()) / 4U, 1U), MAX_UE_GRANT_PER_SLOT});

  // [Implementation-defined] Assume aggregation level 2 to derive the nof. PDCCHs that fit in the remaining CCEs.
  const unsigned remaining_cces =
      is_dl ? pdcch_cce_budget.remaining_dl_cces() : pdcch_cce_budget.remaining_ul_cces(slice.get_slot_tx());
  const unsigned max_nof_candidates = remaining_cces / to_nof_cces(aggregation_level::n2);
  ues_to_alloc                      = std::min(max_nof_candidates, ues_to_alloc);
  if (ues_to_alloc == 0) {
    return std::make_pair(0, 0);
  }

  const crb_interval& bwp_crb_limits = is_dl ? ss_info->dl_crb_lims : ss_info->ul_crb_lims;
  unsigned            max_nof_rbs    = std::min(bwp_crb_limits.length(), slice.remaining_rbs());
  // Count how many RBs are still available for allocation.
  max_nof_rbs = std::min(max_nof_rbs, static_cast<unsigned>((~used_vrbs).count()));
  if (max_nof_rbs == 0) {
    return std::make_pair(0, 0);
  }

  static constexpr unsigned MIN_RB_PER_GRANT = 4;
  return std::make_pair(max_nof_rbs, std::max(max_nof_rbs / ues_to_alloc, MIN_RB_PER_GRANT));
}

unsigned intra_slice_scheduler::schedule_dl_retx_candidates(dl_ran_slice_candidate& slice,
                                                            unsigned                max_ue_grants_to_alloc)
{
  const slice_ue_repository& slice_ues     = slice.get_slice_ues();
  dl_harq_pending_retx_list  pending_harqs = cell_harqs.pending_dl_retxs();

  unsigned alloc_count = 0;
  for (auto it = pending_harqs.begin(); it != pending_harqs.end();) {
    // Note: During retx alloc, the pending HARQ list will mutate. So, we prefetch the next node.
    auto prev_it = it++;
    auto h       = *prev_it;

    if (h.get_grant_params().slice_id != slice.id()) {
      // HARQ ReTx is for another slice.
      continue;
    }
    const slice_ue& u = slice_ues[h.ue_index()];
    if (not can_allocate_pdsch(u, u.get_cc())) {
      continue;
    }

    // Perform DL grant allocation, including PDCCH, PDSCH and UCI.
    // Use the slice's live remaining budget, not its static config max, which can be stale once other slices have
    // already used some of the slot's PRBs.
    auto result = ue_alloc.allocate_dl_grant(
        ue_retx_dl_grant_request{u, pdsch_slot, h, used_dl_vrbs, enable_pdsch_interleaving, slice.remaining_rbs()});
    if (not result.has_value() and result.error() == dl_alloc_failure_cause::skip_slot) {
      // Received signal to stop allocations in the slot.
      break;
    }

    if (result.has_value()) {
      const vrb_alloc& alloc_vrbs = result.value().vrbs;
      mark_used_dl_vrbs(alloc_vrbs);
      slice.store_grant(alloc_vrbs.nof_vrbs());
      // PDSCH repetition occasions land in future slots that are not covered by this slice candidate's own
      // remaining_rbs() budget (scoped to pdsch_slot); register their RB usage against the slice's budget for those
      // slots directly.
      for (slot_point occasion_slot : result.value().repetition_slots) {
        slice.store_grant(alloc_vrbs.nof_vrbs(), occasion_slot);
      }
      if (++alloc_count >= max_ue_grants_to_alloc or slice.remaining_rbs() == 0) {
        // Maximum number of allocations reached.
        break;
      }
    }

    dl_attempts_count++;
    if (dl_attempts_count >= expert_cfg.max_pdcch_alloc_attempts_per_slot) {
      // Maximum number of attempts per slot reached.
      break;
    }
  }

  return alloc_count;
}

unsigned intra_slice_scheduler::schedule_ul_retx_candidates(ul_ran_slice_candidate& slice,
                                                            unsigned                max_ue_grants_to_alloc)
{
  const slice_ue_repository& slice_ues     = slice.get_slice_ues();
  ul_harq_pending_retx_list  pending_harqs = cell_harqs.pending_ul_retxs();

  unsigned alloc_count = 0;
  for (auto it = pending_harqs.begin(); it != pending_harqs.end();) {
    // Note: During retx alloc, the pending HARQ list will mutate. So, we prefetch the next node.
    auto prev_it = it++;
    auto h       = *prev_it;

    if (h.get_grant_params().slice_id != slice.id()) {
      continue;
    }
    const slice_ue& u = slice_ues[h.ue_index()];

    if (not can_allocate_pusch(u, u.get_cc())) {
      continue;
    }

    // Allocate PDCCH and PUSCH.
    // NOTE: the symbols passed to the grant are the symbols that are available for PUSCH and for which the used VRBs
    // have been computed.
    // Use the slice's live remaining budget, not its static config max, which can be stale once PUCCH or other
    // slices have already used some of the slot's PRBs.
    auto result = ue_alloc.allocate_ul_grant(ue_retx_ul_grant_request{
        u, pusch_slot, h, used_ul_vrbs, ofdm_symbol_range{0, min_srs_symbol}, slice.remaining_rbs()});
    if (not result.has_value() and result.error() == alloc_status::skip_slot) {
      // Received signal to stop allocations in the slot.
      break;
    }

    if (result.has_value()) {
      vrb_interval alloc_vrbs = result.value().vrbs;
      used_ul_vrbs.fill(alloc_vrbs.start(), alloc_vrbs.stop());
      slice.store_grant(alloc_vrbs.length());
      // PUSCH repetition occasions land in future slots that are not covered by this slice candidate's own
      // remaining_rbs() budget (scoped to pusch_slot); register their RB usage against the slice's budget for those
      // slots directly.
      for (slot_point occasion_slot : result.value().repetition_slots) {
        slice.store_grant(alloc_vrbs.length(), occasion_slot);
      }
      if (++alloc_count >= max_ue_grants_to_alloc or slice.remaining_rbs() == 0) {
        // Maximum number of allocations reached.
        break;
      }
    }

    ul_attempts_count++;
    if (ul_attempts_count >= expert_cfg.max_pdcch_alloc_attempts_per_slot) {
      // Maximum number of attempts per slot reached.
      break;
    }
  }

  return alloc_count;
}

void intra_slice_scheduler::prepare_newtx_dl_candidates(const dl_ran_slice_candidate& slice,
                                                        scheduler_policy&             dl_policy)
{
  // Build list of UE candidates for newTx.
  if (not slice_ctxt_list.contains(slice.id())) {
    slice_ctxt_list.emplace(slice.id(), *this);
  }
  auto& slice_sched = slice_ctxt_list[slice.id()];
  slice_sched.fill_ue_dl_candidate_group(newtx_candidates, slice);
  if (newtx_candidates.empty()) {
    return;
  }

  // Compute priorities using the provided policy.
  dl_policy.compute_ue_dl_priorities(pdcch_slot, pdsch_slot, newtx_candidates);

  // Sort candidates by priority in descending order.
  std::sort(newtx_candidates.begin(), newtx_candidates.end(), [](const auto& a, const auto& b) {
    return a.priority > b.priority;
  });

  // Remove candidates with forbid priority.
  auto rit = std::find_if(newtx_candidates.rbegin(), newtx_candidates.rend(), [](const auto& cand) {
    return cand.priority != forbid_sched_priority;
  });
  newtx_candidates.erase(rit.base(), newtx_candidates.end());
}

void intra_slice_scheduler::prepare_newtx_ul_candidates(const ul_ran_slice_candidate& slice,
                                                        scheduler_policy&             ul_policy)
{
  // Build list of UE candidates for newTx.
  if (not slice_ctxt_list.contains(slice.id())) {
    slice_ctxt_list.emplace(slice.id(), *this);
  }
  auto& slice_sched = slice_ctxt_list[slice.id()];
  slice_sched.fill_ue_ul_candidate_group(newtx_candidates, slice);
  if (newtx_candidates.empty()) {
    return;
  }

  // Compute priorities using the provided policy.
  ul_policy.compute_ue_ul_priorities(pdcch_slot, pusch_slot, newtx_candidates);

  // Sort candidates by priority in descending order.
  std::sort(newtx_candidates.begin(), newtx_candidates.end(), [](const auto& a, const auto& b) {
    return a.priority > b.priority;
  });

  // Remove candidates with forbid priority.
  auto rit = std::find_if(newtx_candidates.rbegin(), newtx_candidates.rend(), [](const auto& cand) {
    return cand.priority != forbid_sched_priority;
  });
  newtx_candidates.erase(rit.base(), newtx_candidates.end());
}

unsigned intra_slice_scheduler::schedule_dl_newtx_candidates(dl_ran_slice_candidate& slice,
                                                             scheduler_policy&       dl_policy,
                                                             unsigned                max_ue_grants_to_alloc)
{
  // Prepare candidate list.
  prepare_newtx_dl_candidates(slice, dl_policy);
  if (newtx_candidates.empty()) {
    return 0;
  }

  // Recompute max number of UE grants that can be scheduled in this slot and the number of RBs per grant.
  auto [rbs_to_alloc, max_rbs_per_grant] =
      get_max_grants_and_rb_grant_size(newtx_candidates,
                                       pdcch_cce_budget,
                                       slice,
                                       used_dl_vrbs,
                                       std::min(max_ue_grants_to_alloc, expected_pdschs_per_slot));
  if (max_rbs_per_grant == 0) {
    return 0;
  }

  // Stage 1: Pre-select UEs with the highest priority and reserve control-plane space for their DL grants.
  unsigned rb_count                   = 0;
  bool     pucch_grant_limit_exceeded = false;
  for (const auto& ue_candidate : newtx_candidates) {
    if (pucch_grant_limit_exceeded) {
      // The PUCCH is likely saturated and there is no space for new PUCCHs.
      // As a heuristic, we only allocate DL grants to UEs which already have a PUCCH or a PUSCH in a future slot that
      // can likely accommodate more HARQ-ACK bits.
      const auto& harqs = ue_candidate.ue->get_cc().harqs;
      if (not(harqs.last_ack_slot().valid() and harqs.last_ack_slot() > pdsch_slot) and
          not(harqs.last_pusch_slot().valid() and harqs.last_pusch_slot() > pdsch_slot)) {
        continue;
      }
    }

    // A grant with resource allocation type 0 only takes whole RBGs, which may not fit in the RBs left in the slice.
    // [Implementation-defined] UE DL grants of this scheduler always use SearchSpace 2.
    static constexpr search_space_id ue_ded_ss_id      = to_search_space_id(2);
    const unsigned                   min_grant_nof_rbs = sched_helper::get_min_newtx_dl_nof_vrbs(
        ue_candidate.ue->get_cc().cfg().search_space(ue_ded_ss_id), enable_pdsch_interleaving);
    if (rbs_to_alloc - rb_count < min_grant_nof_rbs) {
      continue;
    }

    // Create DL grant builder.
    auto result = ue_alloc.allocate_dl_grant(ue_newtx_dl_grant_request{.user          = *ue_candidate.ue,
                                                                       .pdsch_slot    = pdsch_slot,
                                                                       .pending_bytes = ue_candidate.pending_bytes,
                                                                       .restrictions  = ue_candidate.dl_restrictions});

    if (result.has_value()) {
      // Allocation was successful. Move grant builder to list of pending newTx grants.
      auto& grant_builder = result.value();
      rb_count += std::max(std::min(grant_builder.context().expected_nof_rbs, max_rbs_per_grant), min_grant_nof_rbs);

      pending_dl_newtxs.push_back(std::move(grant_builder));
      if (rb_count >= rbs_to_alloc) {
        // Enough UEs have been allocated to ensure that the grid is filled. Move to stage 2.
        break;
      }
      if (pending_dl_newtxs.size() >= max_ue_grants_to_alloc) {
        // Maximum number of allocations reached. Move to stage 2.
        break;
      }
      if (pdcch_cce_budget.remaining_dl_cces() == 0) {
        // DL PDCCH CCE budget exhausted. Move to stage 2.
        // Note: The last grant may exceed the budget, in which case it borrows CCEs from the reserved UL shares.
        break;
      }
    } else if (result.error() == dl_alloc_failure_cause::skip_slot) {
      // Received signal to stop allocations in the slot. Move to stage 2.
      break;
    } else if (result.error() == dl_alloc_failure_cause::uci_alloc_failed) {
      // The scheduler likely ran out of PUCCH resources. Prioritize UEs which have a UCI already pending.
      pucch_grant_limit_exceeded = true;
    }

    dl_attempts_count++;
    if (dl_attempts_count >= expert_cfg.max_pdcch_alloc_attempts_per_slot) {
      // Maximum number of attempts per slot reached.
      break;
    }
  }

  if (pending_dl_newtxs.empty()) {
    return 0;
  }

  // Stage 2: Derive CRBs, MCS, and number of layers for each grant.
  max_rbs_per_grant = rbs_to_alloc / pending_dl_newtxs.size();
  rb_count          = 0;
  int rbs_missing   = 0;
  for (unsigned alloc_count = 0, nof_grants = pending_dl_newtxs.size(); alloc_count != nof_grants; ++alloc_count) {
    auto& grant_builder = pending_dl_newtxs[alloc_count];
    // Determine the max grant size in RBs for this grant.
    int max_grant_size;
    if (alloc_count == nof_grants - 1) {
      // For the last UE, we also account for the remainder.
      max_grant_size = rbs_to_alloc - rb_count;
    } else {
      max_grant_size = max_rbs_per_grant + rbs_missing;
    }
    if (max_grant_size <= 0) {
      // Earlier type 0 grants exceeded their share by more than this grant's share, to fit whole RBGs.
      logger.debug("ue={} c-rnti={}: Skipping PDSCH grant at slot={}. Cause: RB share taken by earlier RBG grants",
                   grant_builder.ue().ue_index(),
                   grant_builder.ue().crnti(),
                   slice.get_slot_tx());
      grant_builder.set_pdsch_params(vrb_interval{}, enable_pdsch_interleaving);
      // The skipped grant takes no RBs, so its share carries over to the next grants.
      rbs_missing = max_grant_size;
      continue;
    }

    // Derive recommended parameters for the DL newTx grant. A type 0 grant may exceed its share to fit whole RBGs, as
    // long as it fits in the RBs left in the slice.
    const vrb_alloc alloc_vrbs = grant_builder.recommended_vrbs(
        used_dl_vrbs, max_grant_size, rbs_to_alloc - rb_count, enable_pdsch_interleaving);
    if (alloc_vrbs.empty()) {
      logger.warning("ue={} c-rnti={}: Failed to allocate RBs for PDSCH grant at slot={}",
                     grant_builder.ue().ue_index(),
                     grant_builder.ue().crnti(),
                     slice.get_slot_tx());
      // We let the grant be empty. It will be skipped in the post-processing scheduling step.
      grant_builder.set_pdsch_params(vrb_interval{}, enable_pdsch_interleaving);
      continue;
    }

    // Save CRBs, MCS and RI.
    auto committed_repetition_slots = grant_builder.set_pdsch_params(alloc_vrbs, enable_pdsch_interleaving);

    // Fill used VRBs.
    const unsigned nof_rbs_alloc = alloc_vrbs.nof_vrbs();
    mark_used_dl_vrbs(alloc_vrbs);

    // Update slice state.
    slice.store_grant(nof_rbs_alloc);
    // PDSCH repetition occasions land in future slots that are not covered by this slice candidate's own
    // remaining_rbs() budget (scoped to pdsch_slot); register their RB usage against the slice's budget for those
    // slots directly.
    for (slot_point occasion_slot : committed_repetition_slots) {
      slice.store_grant(nof_rbs_alloc, occasion_slot);
    }
    rb_count += nof_rbs_alloc;
    rbs_missing = max_grant_size - static_cast<int>(nof_rbs_alloc);
  }

  // Clear grant builders.
  const unsigned alloc_count = pending_dl_newtxs.size();
  pending_dl_newtxs.clear();

  // Update policy with final allocation results.
  const auto pdschs = cell_alloc[pdsch_slot].result_reader().dl().ue_grants();
  dl_policy.save_dl_newtx_grants(pdschs.last(alloc_count));

  return alloc_count;
}

unsigned intra_slice_scheduler::schedule_ul_newtx_candidates(ul_ran_slice_candidate& slice,
                                                             scheduler_policy&       ul_policy,
                                                             unsigned                max_ue_grants_to_alloc)
{
  // Prepare candidate list.
  prepare_newtx_ul_candidates(slice, ul_policy);
  if (newtx_candidates.empty()) {
    return 0;
  }

  // Recompute max number of UE grants that can be scheduled in this slot and the number of RBs per grant.
  auto [rbs_to_alloc, expected_rbs_per_grant] =
      get_max_grants_and_rb_grant_size(newtx_candidates, pdcch_cce_budget, slice, used_ul_vrbs, max_ue_grants_to_alloc);
  if (expected_rbs_per_grant == 0) {
    return 0;
  }

  // Stage 1: Pre-select UEs with the highest priority and reserve control-plane space for their UL grants.
  unsigned rb_count = 0;
  for (const auto& ue_candidate : newtx_candidates) {
    // Create UL grant builder.
    // NOTE: the symbols passed to the grant are the symbols that are available for PUSCH and for which the used VRBs
    // have been computed.
    auto result = ue_alloc.allocate_ul_grant(ue_newtx_ul_grant_request{
        *ue_candidate.ue, pusch_slot, ue_candidate.pending_bytes, ofdm_symbol_range{0, min_srs_symbol}});

    if (result.has_value()) {
      // Allocation was successful. Move grant builder to list of pending newTx grants.
      auto& grant_builder = result.value();
      rb_count += std::min(grant_builder.context().expected_nof_rbs, expected_rbs_per_grant);
      pending_ul_newtxs.push_back(std::move(grant_builder));
      if (rb_count >= rbs_to_alloc) {
        // Enough UEs have been allocated to ensure that the grid is filled. Move to stage 2.
        break;
      }
      if (pending_ul_newtxs.size() >= max_ue_grants_to_alloc) {
        // Maximum number of allocations reached. Move to stage 2.
        break;
      }
      if (pdcch_cce_budget.remaining_ul_cces(pusch_slot) == 0) {
        // UL PDCCH CCE budget for this PUSCH slot exhausted. Move to stage 2.
        // Note: The last grant may exceed the budget, in which case it borrows CCEs from the later PUSCH slots.
        break;
      }
    } else if (result.error() == alloc_status::skip_slot) {
      // Received signal to stop allocations in the slot. Move to stage 2.
      break;
    }

    ul_attempts_count++;
    if (ul_attempts_count >= expert_cfg.max_pdcch_alloc_attempts_per_slot) {
      // Maximum number of attempts per slot reached.
      break;
    }
  }

  if (pending_ul_newtxs.empty()) {
    return 0;
  }

  // Stage 2: Derive CRBs, MCS, and number of layers for each grant.
  expected_rbs_per_grant = rbs_to_alloc / pending_ul_newtxs.size();
  rb_count               = 0;
  int rbs_missing        = 0;
  for (unsigned alloc_count = 0, nof_grants = pending_ul_newtxs.size(); alloc_count != nof_grants; ++alloc_count) {
    auto& grant_builder = pending_ul_newtxs[alloc_count];
    // Determine the max grant size in RBs for this grant.
    int max_grant_size;
    if (alloc_count == nof_grants - 1) {
      // For the last UE, we also account for the remaining RBs.
      max_grant_size = rbs_to_alloc - rb_count;
    } else {
      max_grant_size = expected_rbs_per_grant + rbs_missing;
    }
    ocudu_assert(max_grant_size > 0, "Invalid grant size.");

    // Derive recommended parameters for the DL newTx grant.
    vrb_interval alloc_vrbs = grant_builder.recommended_vrbs(used_ul_vrbs, max_grant_size);
    if (alloc_vrbs.empty()) {
      logger.error("ue={} c-rnti={}: Failed to allocate RBs for PUSCH grant at slot={}",
                   grant_builder.ue().ue_index(),
                   grant_builder.ue().crnti(),
                   slice.get_slot_tx());
      // We let the grant be empty. It will be skipped in the post-processing scheduling step.
      grant_builder.set_pusch_params({});
      continue;
    }

    // Save CRBs, MCS and RI.
    auto committed_repetition_slots = grant_builder.set_pusch_params(alloc_vrbs);

    // Fill used CRBs.
    unsigned nof_rbs_alloc = alloc_vrbs.length();
    used_ul_vrbs.fill(alloc_vrbs.start(), alloc_vrbs.stop());

    // Update slice state.
    slice.store_grant(nof_rbs_alloc);
    // Register the occasions' RB usage against the slice's budget for their own (future) slots, as above.
    for (slot_point occasion_slot : committed_repetition_slots) {
      slice.store_grant(nof_rbs_alloc, occasion_slot);
    }
    rb_count += nof_rbs_alloc;
    rbs_missing = max_grant_size - nof_rbs_alloc;
  }

  // Clear grant builders.
  unsigned alloc_count = pending_ul_newtxs.size();
  pending_ul_newtxs.clear();

  // Update policy with allocation results.
  const auto puschs = cell_alloc[pusch_slot].result_reader().ul().puschs();
  ul_policy.save_ul_newtx_grants(puschs.last(alloc_count));

  return alloc_count;
}

bool intra_slice_scheduler::can_allocate_pdsch(const slice_ue& u, const ue_cell& ue_cc) const
{
  // Check if PDCCH/PDSCH is possible for this slot (e.g. not in UL slot or measGap). This also rejects a UE that
  // already has a PDSCH in this slot (e.g. a PDSCH repetition occasion carried over from a bundle scheduled in a
  // preceding slot),
  ocudu_assert(not ue_cc.is_in_fallback_mode(), "Slice UE cannot be in fallback mode");
  return ue_cc.is_pdsch_enabled(pdcch_slot, pdsch_slot);
}

bool intra_slice_scheduler::can_allocate_pusch(const slice_ue& u, const ue_cell& ue_cc) const
{
  // A UE on another Koffset reaches this slot from a different k2, so the used VRBs, computed for the k2 of the
  // candidate, do not describe the grant it would be given. It waits for the candidate built for its own Koffset.
  if (ue_cc.ntn_koffset() != pusch_ntn_koffset) {
    return false;
  }
  // Check if PDCCH/PUSCH are possible for the provided slots (e.g. not in UL slot or measGap)
  return ue_cc.is_pusch_enabled(pdcch_slot, pusch_slot);
}
std::optional<ue_newtx_candidate> intra_slice_scheduler::create_newtx_dl_candidate(const slice_ue& u) const
{
  const ue_cell& ue_cc = u.get_cc();
  ocudu_assert(ue_cc.is_active() and not ue_cc.is_in_fallback_mode(), "Invalid slice UE state");

  // Check if the UE has pending data to transmit.
  // The block carries only the channels its restrictions allow, so the grant is sized for those alone.
  const auto   tb_sel = u.select_dl_transport_block();
  units::bytes pending_bytes{tb_sel.pending_bytes};
  if (pending_bytes.value() == 0) {
    return std::nullopt;
  }

  if (not can_allocate_pdsch(u, ue_cc)) {
    return std::nullopt;
  }

  // A process is only usable when it carries the feedback the logical channels of the slice need, so the grant is
  // dropped here rather than after a PDCCH and a PUCCH have been booked for it.
  if (not ue_cc.harqs.has_empty_dl_harqs(harq_utils::to_harq_mode(tb_sel.restrictions.harq_feedback))) {
    // No available HARQs. A process free in the other mode owes no HARQ-ACK, so the feedback only runs late once no
    // process at all is free.
    if (not ue_cc.harqs.has_empty_dl_harqs() and not ue_cc.harqs.find_pending_dl_retx().has_value()) {
      // A long PDSCH to HARQ-ACK round trip can leave every process waiting for its feedback.
      logger.info(
          "ue={} rnti={} PDSCH allocation skipped. Cause: All the HARQs are allocated and waiting for their "
          "respective HARQ-ACK. Check if any HARQ-ACK went missing in the lower layers or is arriving too late to "
          "the scheduler.",
          ue_cc.ue_index,
          ue_cc.rnti());
      cell_metrics.handle_late_dl_harqs();
    }
    return std::nullopt;
  }

  return ue_newtx_candidate{&u, forbid_sched_priority, pending_bytes, tb_sel.restrictions};
}
std::optional<ue_newtx_candidate> intra_slice_scheduler::create_newtx_ul_candidate(const slice_ue& u) const
{
  const ue_cell& ue_cc = u.get_cc();
  ocudu_assert(ue_cc.is_active() and not ue_cc.is_in_fallback_mode(), "Invalid slice UE state");

  units::bytes pending_bytes = u.pending_ul_newtx_bytes();
  if (pending_bytes.value() == 0) {
    return std::nullopt;
  }

  if (not can_allocate_pusch(u, ue_cc)) {
    return std::nullopt;
  }

  if (not ue_cc.harqs.has_empty_ul_harqs()) {
    // No available HARQs.
    if (not ue_cc.harqs.find_pending_ul_retx().has_value()) {
      // All HARQs are waiting for their respective CRC. This may be a symptom of a slow PUSCH processing chain.
      logger.info("ue={} rnti={} PUSCH allocation skipped. Cause: All the UE HARQs are busy waiting for "
                  "their respective CRC result. Check if any CRC PDU went missing in the lower layers or is "
                  "arriving too late to the scheduler.",
                  ue_cc.ue_index,
                  ue_cc.rnti());
      cell_metrics.handle_late_ul_harqs();
    }
    return std::nullopt;
  }

  return ue_newtx_candidate{&u, forbid_sched_priority, pending_bytes};
}

unsigned intra_slice_scheduler::max_pdschs_to_alloc(const dl_ran_slice_candidate& slice) const
{
  // We cannot allocate more than the number of UEs available.
  int pdschs_to_alloc = slice.get_slice_ues().size();

  // Determine how many UE DL PDUs can be allocated in this slot.
  const auto pdsch_res = cell_alloc[pdsch_slot].result_reader();
  pdschs_to_alloc =
      std::min(pdschs_to_alloc, static_cast<int>(MAX_UE_PDUS_PER_SLOT - pdsch_res.dl().ue_grants().size()));
  if (pdschs_to_alloc <= 0) {
    return 0;
  }

  // Determine how many PDCCHs can be allocated in this slot.
  const auto pdcch_res  = cell_alloc[pdcch_slot].result_reader();
  const int  max_pdcchs = std::min(static_cast<int>(MAX_DL_PDCCH_PDUS_PER_SLOT - pdcch_res.dl().dl_pdcchs().size()),
                                  static_cast<int>(expert_cfg.max_pdcch_alloc_attempts_per_slot - dl_attempts_count));
  pdschs_to_alloc       = std::min(pdschs_to_alloc, max_pdcchs);
  if (pdschs_to_alloc <= 0) {
    return 0;
  }

  // Determine how many PDSCHs can be allocated in this slot.
  const int max_pdschs =
      std::min(static_cast<int>(MAX_PDSCH_PDUS_PER_SLOT), static_cast<int>(expert_cfg.max_pdschs_per_slot));
  int allocated_pdschs = pdsch_res.dl().ue_grants().size() + pdsch_res.dl().sibs().size() +
                         pdsch_res.dl().rar_grants().size() + pdsch_res.dl().paging_grants().size();
  pdschs_to_alloc = std::min(pdschs_to_alloc, max_pdschs - allocated_pdschs);
  if (pdschs_to_alloc <= 0) {
    return 0;
  }

  // Assume at least one RB per UE.
  pdschs_to_alloc = std::min(pdschs_to_alloc, static_cast<int>(slice.remaining_rbs()));

  return std::max(pdschs_to_alloc, 0);
}

unsigned intra_slice_scheduler::max_puschs_to_alloc(const ul_ran_slice_candidate& slice) const
{
  if (not cell_alloc.cfg.is_ul_enabled(slice.get_slot_tx())) {
    return 0;
  }

  // We cannot allocate more than the number of UEs available.
  int puschs_to_alloc = slice.get_slice_ues().size();

  // Determine how many PUSCHs can be allocated in this slot.
  const auto pusch_res = cell_alloc[pusch_slot].result_reader();
  // The max PUSCHs-per-slot limit cannot be exceeded.
  // Note: We use signed integer to avoid unsigned overflow.
  const int max_puschs = std::min(static_cast<unsigned>(MAX_PUSCH_PDUS_PER_SLOT), expert_cfg.max_puschs_per_slot);
  puschs_to_alloc      = std::min(puschs_to_alloc, max_puschs - static_cast<int>(pusch_res.ul().puschs().size()));
  if (puschs_to_alloc <= 0) {
    return 0;
  }

  // The max UL grants-per-slot limit cannot be exceeded.
  puschs_to_alloc = std::min(puschs_to_alloc,
                             static_cast<int>(expert_cfg.max_ul_grants_per_slot - pusch_res.ul().puschs().size() -
                                              pusch_res.ul().pucchs().size()));
  if (puschs_to_alloc <= 0) {
    return 0;
  }

  // Determine how many PDCCHs can be allocated in this slot.
  const auto pdcch_res  = cell_alloc[pdcch_slot].result_reader();
  const auto max_pdcchs = std::min(static_cast<int>(MAX_UL_PDCCH_PDUS_PER_SLOT - pdcch_res.dl().ul_pdcchs().size()),
                                   static_cast<int>(expert_cfg.max_pdcch_alloc_attempts_per_slot - ul_attempts_count));
  puschs_to_alloc       = std::min(puschs_to_alloc, max_pdcchs);
  if (puschs_to_alloc <= 0) {
    return 0;
  }

  // Assume at least one RB per UE.
  puschs_to_alloc = std::min(puschs_to_alloc, static_cast<int>(slice.remaining_rbs()));

  return std::max(puschs_to_alloc, 0);
}

void intra_slice_scheduler::update_used_dl_vrbs(const dl_ran_slice_candidate& slice)
{
  // The mapping from CRBs to VRBs is different for UEs configured with a common SearchSpace that uses DCI 1_0.
  // [Implementation defined] We assume all the UEs of this cell are configured the same way in that regard.
  const slice_ue_repository&       slice_ues    = slice.get_slice_ues();
  static constexpr search_space_id ue_ded_ss_id = to_search_space_id(2);
  const search_space_info&         ss_info      = slice_ues.begin()->get_cc().cfg().search_space(ue_ded_ss_id);

  // [Implementation defined] We use the common PDSCH TD resources as a reference for the computation of RBs
  // unavailable for PDSCH. This assumes that these resources are not colliding with PDCCH.
  // Note: It is not a problem if the symbols go beyond the DL symbols in the special slot, because UL is reserved in
  // a separate grid.
  const auto&              init_dl_bwp = cell_alloc.cfg.params.dl_cfg_common.init_dl_bwp;
  const ofdm_symbol_range& symbols_to_check =
      cell_alloc.cfg.init_bwp.dl.td_mapper().common_pdsch_td_resources()[0].symbols;

  enable_pdsch_interleaving = ss_info.interleaved_mapping.has_value();
  if (enable_pdsch_interleaving) {
    for (const auto& pdsch : cell_alloc[pdsch_slot].result_reader().dl().ue_grants()) {
      // [Implementation defined] Disable interleaving if there are non-interleaved PDSCH grants already allocated on
      // that slot, to avoid wasting resources.
      if (pdsch.pdsch_cfg.vrb_prb_mapping == vrb_to_prb::mapping_type::non_interleaved) {
        enable_pdsch_interleaving = false;
        break;
      }
    }
  }

  prb_bitmap used_prbs = cell_alloc[pdsch_slot].dl_res_grid.used_prbs(
      init_dl_bwp.generic_params.scs, ss_info.dl_crb_lims, symbols_to_check);
  // Mark as used the PRBs that fall outside the CRB limits.
  const auto prb_lims = crb_to_prb(ss_info.dl_crb_lims,
                                   slice_ues.begin()->get_cc().cfg().cell_cfg_common.expert_cfg.ue.pdsch_crb_limits &
                                       ss_info.dl_crb_lims);
  if (not prb_lims.empty()) {
    used_prbs.fill(0, prb_lims.start());
    used_prbs.fill(prb_lims.stop(), used_prbs.size());
  }

  // Perform inverse VRB-to-PRB mapping when interleaving is enabled for this slice/BWP.
  if (enable_pdsch_interleaving) {
    const auto& interleaved_mapping = ss_info.interleaved_mapping.value();
    used_dl_vrbs                    = interleaved_mapping.prb_to_vrb(used_prbs);
  } else {
    used_dl_vrbs = used_prbs.convert_to<vrb_bitmap>();
  }
}

void intra_slice_scheduler::mark_used_dl_vrbs(const vrb_alloc& vrbs)
{
  if (vrbs.is_type0()) {
    used_dl_vrbs |= vrbs.type0();
  } else {
    used_dl_vrbs |= vrbs.type1();
  }
}

void intra_slice_scheduler::update_used_ul_vrbs(const ul_ran_slice_candidate& slice)
{
  // Update the min SRS symbol before checking the used VRBs.
  // NOTE: Reset to max symbol index before looking for the min.
  min_srs_symbol = NOF_OFDM_SYM_PER_SLOT_NORMAL_CP;
  for (const auto& srs : cell_alloc[pusch_slot - pdcch_slot].result_reader().ul().srss()) {
    min_srs_symbol = std::min(static_cast<unsigned>(srs.symbols.start()), min_srs_symbol);
  }

  // The mapping from CRBs to VRBs is different for UEs configured with a common SearchSpace that uses DCI 1_0.
  // [Implementation defined] We assume all the UEs of this cell are configured the same way in that regard.
  const slice_ue_repository&       slice_ues   = slice.get_slice_ues();
  static constexpr search_space_id ue_ded_ss   = to_search_space_id(2);
  const search_space_info&         ss_info     = slice_ues.begin()->get_cc().cfg().search_space(ue_ded_ss);
  const auto&                      ul_crb_lims = ss_info.ul_crb_lims;

  // (Implementation-defined) We use the common PUSCH TD resources as a reference for the computation of RBs unavailable
  // for PDSCH. This assumes that these resources are not colliding with SRS.
  const auto& init_ul_bwp = cell_alloc.cfg.params.ul_cfg_common.init_ul_bwp;
  ocudu_assert(slice.get_slot_tx() - cell_alloc[0].slot > 0, "PUSCH slot cannot precede its corresponding PDCCH slot");
  const unsigned    slice_candidate_k2 = slice.get_k2();
  ofdm_symbol_range symbols_to_check   = {0, 0};
  // Find the max symbols such that symbols.stop() <= min_srs_symbol;
  for (const auto& td_res : init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list) {
    // We can choose only the symbols for the same k2.
    if (td_res.k2 < slice_candidate_k2) {
      continue;
    }
    if (td_res.k2 == slice_candidate_k2) {
      if (td_res.symbols.stop() <= min_srs_symbol and td_res.symbols.stop() > symbols_to_check.stop()) {
        symbols_to_check = td_res.symbols;
      }
    }
    if (td_res.k2 > slice_candidate_k2) {
      break;
    }
  }

  if (symbols_to_check.empty()) {
    logger.debug(
        "No symbols available for PUSCH allocation on slot={} for slice_id={}", pusch_slot, slice.id().value());
    return;
  }

  used_ul_vrbs = cell_alloc[pusch_slot]
                     .ul_res_grid.used_prbs(init_ul_bwp.generic_params.scs, ul_crb_lims, symbols_to_check)
                     .convert_to<vrb_bitmap>();
}
