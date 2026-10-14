// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "logical_channel_list_config.h"
#include "ocudu/adt/detail/intrusive_ptr.h"
#include "ocudu/adt/slotted_vector.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/scheduler/config/sched_bwp_config.h"
#include "ocudu/scheduler/config/serving_cell_config.h"

namespace ocudu {

struct sched_ue_config_request;

/// List of UE-dedicated BWP configs.
using bwp_config_list = slotted_id_vector<bwp_id_t, sched_bwp_config>;

/// UE-dedicated resources for a given cell.
struct ue_cell_res_config {
  /// DU-specific cell identifier.
  du_cell_index_t cell_index;
  /// Container that maps Coreset-Ids to CORESET configurations for this BWP.
  /// Note: The ID space of CoresetIds is common among the BWPs of a Serving Cell as per TS 38.331.
  slotted_id_vector<coreset_id, const sched_coreset_config*> coresets;
  /// Container that maps searchSpaceIds to searchSpace configurations for this BWP.
  /// Note: The ID space of searchSpaceIds is common among the BWPs of a Serving Cell as per TS 38.331.
  slotted_id_vector<search_space_id, const search_space_configuration*> search_spaces;
  /// List of BWPs configured in this cell.
  bwp_config_list bwps;
  /// \brief \c pdsch-ServingCellConfig, used to configure UE specific PDSCH parameters that are common across the UE's
  /// BWPs of one serving cell.
  std::optional<config_ptr<pdsch_serving_cell_config>> pdsch_serv_cell_cfg;
  /// \brief \c pusch-ServingCellConfig, used to configure UE specific PUSCH parameters that are common across the UE's
  /// BWPs of one serving cell.
  std::optional<config_ptr<pusch_serving_cell_config>> pusch_serv_cell_cfg;
  /// Timing Advance Group ID to which this cell belongs to.
  time_alignment_group::id_t tag_id{0};

  /// Resets the payload for reuse from the pool. The reference counter is intentionally left untouched.
  void clear()
  {
    cell_index = du_cell_index_t{};
    coresets.clear();
    search_spaces.clear();
    bwps.clear();
    pdsch_serv_cell_cfg.reset();
    pusch_serv_cell_cfg.reset();
    tag_id = time_alignment_group::id_t{0};
  }

  /// Determines whether any UE is currently using this config.
  bool is_unreferenced() const { return ref_cnt.is_unreferenced(); }

private:
  /// \brief Intrusive ref-counting hook for \c ue_cell_res_config.
  friend void intrusive_ptr_inc_ref(const ue_cell_res_config* cfg) { cfg->ref_cnt.inc_ref(); }
  friend void intrusive_ptr_dec_ref(const ue_cell_res_config* cfg)
  {
    // Note: deliberately ignores the "reached zero" return — the pool owns the storage and reuses it; we never free
    // here.
    cfg->ref_cnt.dec_ref();
  }

  /// \brief Intrusive reference counter. The object lives in a pool (du_cell_config_pool); >0 means in use, 0 means
  /// free to be reused. Mutable so references can be taken/dropped through a const handle.
  mutable intrusive_ptr_atomic_ref_counter ref_cnt;
};

/// Reference-counted handle to a per-UE dedicated cell config. The config is owned by the scheduler config pool and
/// reused once no UE references it; the handle is an intrusive_ptr so that dropping it on the RT thread never frees.
using ue_cell_config_ptr = intrusive_ptr<const ue_cell_res_config>;

/// Parameters used to generate a UE configuration object.
struct ue_creation_params {
  const sched_ue_config_request&                         cfg_req;
  logical_channel_config_list_ptr                        lc_ch_list;
  slotted_id_vector<du_cell_index_t, ue_cell_config_ptr> cells;
};

/// Parameters used to reconfigure a UE configuration object.
struct ue_reconfig_params {
  const sched_ue_config_request&                         cfg_req;
  std::optional<logical_channel_config_list_ptr>         lc_ch_list;
  slotted_id_vector<du_cell_index_t, ue_cell_config_ptr> cells;
  const std::optional<rnti_t>                            cs_rnti;
};

} // namespace ocudu
