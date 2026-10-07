// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "pdcch_slot_resource_allocator.h"
#include "../cell/resource_grid.h"
#include "../support/pdcch/pdcch_mapping.h"
#include "ocudu/ran/pdcch/cce_to_prb_mapping.h"

using namespace ocudu;

pdcch_slot_allocator::pdcch_slot_allocator()
{
  dfs_tree.reserve(MAX_DL_PDCCH_PDUS_PER_SLOT + MAX_UL_PDCCH_PDUS_PER_SLOT);
  saved_dfs_tree.reserve(MAX_DL_PDCCH_PDUS_PER_SLOT + MAX_UL_PDCCH_PDUS_PER_SLOT);
}

pdcch_slot_allocator::~pdcch_slot_allocator() {}

void pdcch_slot_allocator::clear()
{
  records.clear();
  dfs_tree.clear();
  saved_dfs_tree.clear();
}

bool pdcch_slot_allocator::alloc_pdcch(dci_context_information&          pdcch_ctx,
                                       cell_slot_resource_allocator&     slot_alloc,
                                       const sched_coreset_config&       cs_cfg,
                                       const search_space_configuration& ss_cfg,
                                       span<const pdcch_candidate_type>  ss_candidates,
                                       bool                              movable)
{
  saved_dfs_tree.clear();
  unsigned attempts_left = max_nof_candidate_checks;

  // Create an DL Allocation Record.
  alloc_record record{};
  record.is_dl            = true;
  record.pdcch_ctx        = &pdcch_ctx;
  record.cs_cfg           = &cs_cfg;
  record.ss_cfg           = &ss_cfg;
  record.pdcch_candidates = ss_candidates;

  // Try to allocate PDCCH for one of the possible CCE positions. If this operation fails, retry it, but using a
  // different permutation of past grant CCE positions.
  do {
    if (alloc_dfs_node(slot_alloc, record, 0, attempts_left)) {
      // Set CCE start index.
      record.pdcch_ctx->cces.ncce = dfs_tree.back().ncce;
      if (not movable) {
        // Keep only the chosen candidate, so that the search for later PDCCHs cannot move this one.
        record.pdcch_candidates        = record.pdcch_candidates.subspan(dfs_tree.back().dci_iter_index, 1);
        dfs_tree.back().dci_iter_index = 0;
      }
      // DCI record was successfully allocated.
      records.push_back(record);
      // If the previous branches of the DFS changed in order for this new PDCCH to be allocated, update previous
      // PDCCH ncces
      if (not saved_dfs_tree.empty()) {
        for (unsigned i = 0; i != dfs_tree.size() - 1; ++i) {
          records[dfs_tree[i].record_index].pdcch_ctx->cces.ncce = dfs_tree[i].ncce;
        }
      }
      return true;
    }
    if (saved_dfs_tree.empty()) {
      saved_dfs_tree = dfs_tree;
    }
  } while (get_next_dfs(slot_alloc, attempts_left));

  // Revert steps to initial state, before dci record allocation was attempted
  for (const tree_node& node : dfs_tree) {
    release_cce(slot_alloc, records[node.record_index], node.dci_iter_index);
  }
  dfs_tree.swap(saved_dfs_tree);
  for (const tree_node& node : dfs_tree) {
    const bool restored = allocate_cce(slot_alloc, records[node.record_index], node.dci_iter_index);
    ocudu_assert(restored, "Failed to restore original PDCCH reservation");
    (void)restored;
  }

  return false;
}

bool pdcch_slot_allocator::cancel_last_pdcch(cell_slot_resource_allocator& slot_alloc)
{
  if (records.empty()) {
    return false;
  }
  auto& last_rec = records.back();

  auto       dl_pdcchs   = slot_alloc.result_builder().dl().dl_pdcchs();
  auto       ul_pdcchs   = slot_alloc.result_builder().dl().ul_pdcchs();
  const bool is_dl_pdcch = not dl_pdcchs.empty() and last_rec.pdcch_ctx == &dl_pdcchs.back().ctx;
  const bool is_ul_pdcch = not is_dl_pdcch and not ul_pdcchs.empty() and last_rec.pdcch_ctx == &ul_pdcchs.back().ctx;
  if (not is_dl_pdcch and not is_ul_pdcch) {
    return false;
  }

  // Read the resources taken by the PDCCH while its PDU is still alive.
  const sched_coreset_config& cs_cfg   = *last_rec.cs_cfg;
  const crb_index_list_span pdcch_crbs = cs_cfg.candidate_crbs(dfs_tree.back().ncce, last_rec.pdcch_ctx->cces.aggr_lvl);
  const ofdm_symbol_range   symbols{0, (uint8_t)cs_cfg.cfg().duration()};
  const subcarrier_spacing  scs = last_rec.pdcch_ctx->bwp_cfg->scs;

  if (is_dl_pdcch) {
    dl_pdcchs.pop_back();
  } else {
    ul_pdcchs.pop_back();
  }

  // Clear allocation on resource grid.
  slot_alloc.dl_res_grid.clear(scs, symbols, pdcch_crbs);

  dfs_tree.pop_back();
  records.pop_back();
  return true;
}

bool pdcch_slot_allocator::alloc_dfs_node(cell_slot_resource_allocator& slot_alloc,
                                          const alloc_record&           record,
                                          unsigned                      dci_iter_index,
                                          unsigned&                     attempts_left)
{
  // Get CCE location table, i.e. the current node possible leaves.
  span<const uint8_t> cce_locs = record.pdcch_candidates;
  if (dci_iter_index >= cce_locs.size()) {
    // All possible CCE position leaves have been attempted. Early return.
    return false;
  }

  // Create new tree leave.
  tree_node node{};
  node.dci_iter_index = dci_iter_index;
  node.record_index   = dfs_tree.size();

  // Find in the list of possible CCEs, a CCE that does not cause PDCCH collisions.
  for (; node.dci_iter_index < cce_locs.size() and attempts_left > 0; ++node.dci_iter_index) {
    --attempts_left;
    node.ncce = cce_locs[node.dci_iter_index];

    // Attempt to allocate PDCCH with provided CCE position.
    if (not allocate_cce(slot_alloc, record, node.dci_iter_index)) {
      continue;
    }

    // Allocation successful. Add node to tree.
    dfs_tree.push_back(node);
    return true;
  }
  return false;
}

bool pdcch_slot_allocator::get_next_dfs(cell_slot_resource_allocator& slot_alloc, unsigned& attempts_left)
{
  do {
    if (dfs_tree.empty() or attempts_left == 0) {
      // The search reaches the root or exhausts its candidate budget.
      return false;
    }
    // Attempt to re-add last tree node, but with a higher leaf index.
    uint32_t start_child_idx = dfs_tree.back().dci_iter_index + 1;
    release_cce(slot_alloc, records[dfs_tree.back().record_index], dfs_tree.back().dci_iter_index);
    dfs_tree.pop_back();
    while (dfs_tree.size() < records.size() and
           alloc_dfs_node(slot_alloc, records[dfs_tree.size()], start_child_idx, attempts_left)) {
      start_child_idx = 0;
    }
  } while (dfs_tree.size() < records.size());

  // Finished computation of next DFS node.
  return true;
}

void pdcch_slot_allocator::release_cce(cell_slot_resource_allocator& slot_alloc,
                                       const alloc_record&           record,
                                       unsigned                      dci_iter_index)
{
  const dci_context_information& ctx = *record.pdcch_ctx;
  const crb_index_list_span      pdcch_crbs =
      record.cs_cfg->candidate_crbs(record.pdcch_candidates[dci_iter_index], ctx.cces.aggr_lvl);
  slot_alloc.dl_res_grid.clear(ctx.bwp_cfg->scs, {0, static_cast<uint8_t>(ctx.coreset_cfg->duration())}, pdcch_crbs);
}

bool pdcch_slot_allocator::allocate_cce(cell_slot_resource_allocator& slot_alloc,
                                        const alloc_record&           record,
                                        unsigned                      dci_iter_index)
{
  const bwp_configuration&     bwp_cfg = *record.pdcch_ctx->bwp_cfg;
  const coreset_configuration& cs_cfg  = *record.pdcch_ctx->coreset_cfg;
  const crb_index_list_span    pdcch_crbs =
      record.cs_cfg->candidate_crbs(record.pdcch_candidates[dci_iter_index], record.pdcch_ctx->cces.aggr_lvl);
  ocudu_sanity_check(pdcch_crbs.size() > 0, "PDCCH candidate CRB list is empty");
  grant_info grant;
  grant.scs = bwp_cfg.scs;

  // Check the current CCE position collides with an existing one.
  ofdm_symbol_range symbols{0, (uint8_t)cs_cfg.duration()};
  if (slot_alloc.dl_res_grid.collides(record.pdcch_ctx->bwp_cfg->scs, symbols, pdcch_crbs)) {
    // Collision detected. Try another CCE position.
    return false;
  }

  // Allocation successful.
  slot_alloc.dl_res_grid.fill(record.pdcch_ctx->bwp_cfg->scs, symbols, pdcch_crbs);

  return true;
}
