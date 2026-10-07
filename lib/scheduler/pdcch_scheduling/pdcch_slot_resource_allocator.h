// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../support/pdcch/pdcch_mapping.h"
#include "ocudu/ran/pdcch/pdcch_candidates.h"
#include "ocudu/scheduler/result/pdcch_info.h"

namespace ocudu {

struct cell_slot_resource_allocator;
class cell_configuration;
class sched_coreset_config;

/// List of CRBs for a given PDCCH candidate.
using crb_index_list      = static_vector<uint16_t, pdcch_constants::MAX_NOF_RB_PDCCH>;
using crb_index_list_span = span<const uint16_t>;

class pdcch_slot_allocator
{
public:
  /// PDCCH grant allocation in a given slot.
  struct alloc_record {
    bool                              is_dl;
    dci_context_information*          pdcch_ctx;
    const sched_coreset_config*       cs_cfg;
    const search_space_configuration* ss_cfg;
    span<const uint8_t>               pdcch_candidates;
  };

  /// DFS decision tree node.
  struct tree_node {
    unsigned dci_iter_index;
    unsigned ncce;
    unsigned record_index;
  };

  pdcch_slot_allocator();
  ~pdcch_slot_allocator();

  /// Erase the current PDCCH allocations and stored context for this slot.
  void clear();

  /// \brief Searches for an available CCEs in the PDCCH to allocate a DCI. If it finds it, it reserves the CCEs, so
  /// they cannot be reused in this same slot.
  /// \param movable Whether later allocations in this slot may move this PDCCH to another candidate. A PDCCH whose
  /// CCE position other resources are derived from must not be moved.
  bool alloc_pdcch(dci_context_information&          pdcch_ctx,
                   cell_slot_resource_allocator&     slot_alloc,
                   const sched_coreset_config&       cs_cfg,
                   const search_space_configuration& ss_cfg,
                   span<const pdcch_candidate_type>  ss_candidates,
                   bool                              movable);

  /// Deallocates the last PDCCH CCE space reservation.
  bool cancel_last_pdcch(cell_slot_resource_allocator& slot_alloc);

private:
  /// \brief Maximum number of PDCCH candidate positions checked by a single alloc_pdcch() call.
  ///
  /// Moving the PDCCHs already allocated in the slot makes the search exponential in their number. The bound keeps
  /// the search time on the real-time scheduler thread small; an exhausted search fails and reverts as any other.
  static constexpr unsigned max_nof_candidate_checks = 64;

  bool alloc_dfs_node(cell_slot_resource_allocator& slot_alloc,
                      const alloc_record&           record,
                      unsigned                      dci_iter_index,
                      unsigned&                     attempts_left);
  bool get_next_dfs(cell_slot_resource_allocator& slot_alloc, unsigned& attempts_left);

  /// Allocate CCEs of a given PDCCH.
  bool allocate_cce(cell_slot_resource_allocator& slot_alloc, const alloc_record& record, unsigned dci_iter_index);
  /// Release the CCEs of a given PDCCH.
  void release_cce(cell_slot_resource_allocator& slot_alloc, const alloc_record& record, unsigned dci_iter_index);

  /// list of grants in a given slot.
  static_vector<alloc_record, MAX_DL_PDCCH_PDUS_PER_SLOT + MAX_UL_PDCCH_PDUS_PER_SLOT> records;

  /// dfs decision tree for the given slot.
  std::vector<tree_node> dfs_tree, saved_dfs_tree;
};

} // namespace ocudu
