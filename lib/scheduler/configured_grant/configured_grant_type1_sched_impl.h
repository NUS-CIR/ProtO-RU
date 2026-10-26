// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "configured_grant_scheduler_base.h"

namespace ocudu {

/// \brief CG Type 1 scheduler.
///
/// The grant is fully described by the RRC-configured uplink grant, so a UE is placed in the periodic wheel as soon as
/// it is configured, with no activation procedure. Everything past that point is handled by
/// \ref configured_grant_scheduler_base.
class configured_grant_type1_sched_impl : public configured_grant_scheduler_base
{
public:
  configured_grant_type1_sched_impl(const cell_configuration& cell_cfg_,
                                    const cg_builder_params&  cell_cg_cfg,
                                    uci_allocator&            uci_alloc_,
                                    ue_repository&            ues_);

  void rem_ue(const ue_cell_configuration& ue_cfg) override;

  void add_reconf_ue(const ue_cell_configuration& new_ue_cfg, const ue_cell_configuration* old_ue_cfg) override;

  void start_ue_cg_activation(du_ue_index_t ue_idx) override
  {
    // No-op, only used for CG type2.
  }

  void handle_cg_activation_complete_indication(du_ue_index_t ue_idx) override
  {
    // No-op, only used for CG type2.
  }

  void handle_error_indication(slot_point sl_tx, scheduler_slot_handler::error_outcome event) override
  {
    // No-op: CG type1 needs no activation, so it does not track expected CRC indications.
  }

private:
  bool has_cg_grant_cfg(const ue_cell_configuration& ue_cfg) const override;

  // Adds a UE's CG grant positions to the slot wheel. Used by add_reconf_ue().
  void add_ue_to_wheel(const ue_cell_configuration& ue_cfg);
};

} // namespace ocudu
