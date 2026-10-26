// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/scheduler/scheduler_slot_handler.h"

namespace ocudu {

struct cell_resource_allocator;
class ue_cell_configuration;

/// Configured grant scheduler interface, which handles the scheduling of configured grant opportunities.
class configured_grant_scheduler
{
public:
  virtual ~configured_grant_scheduler() = default;

  /// Runs the configured grant scheduler for the current slot.
  /// \param[out,in] res_alloc struct with scheduling results.
  virtual void run_slot(cell_resource_allocator& res_alloc) = 0;

  /// Removes the UE from the internal list of UEs to be scheduled.
  /// \param[in] ue_cfg UE dedicated configuration of the UE to be removed.
  virtual void rem_ue(const ue_cell_configuration& ue_cfg) = 0;

  /// Updates the configured grant configuration of this UE, if there are any changes w.r.t. the previous configuration.
  /// \param[in] new_ue_cfg New UE dedicated configuration of the UE to be reconfigured.
  /// \param[in] old_ue_cfg If set, old UE dedicated configuration of the UE to be reconfigured.
  /// \remark nullptr is passed to \ref old_ue_cfg when the UE is created through F1AP.
  virtual void add_reconf_ue(const ue_cell_configuration& new_ue_cfg, const ue_cell_configuration* old_ue_cfg) = 0;

  /// Called on cell deactivation.
  virtual void stop() = 0;

  /// \brief Triggers CG Type 2 activation of the UE.
  ///
  /// \remark Leaves a UE whose CG is already active untouched. No-op for Type 1 Configured Grant.
  virtual void start_ue_cg_activation(du_ue_index_t ue_idx) = 0;

  /// \brief Triggers the removal of this UE from the list of UEs with pending activation.
  ///
  /// Called upon CG activation acknowledgement, either via a Configured Grant Confirmation MAC CE or a CRC OK of a CG
  /// PUSCH.
  /// \remark Idempotent. A call for a UE without a pending activation has no effect. No-op for Type 1 Configured Grant.
  virtual void handle_cg_activation_complete_indication(du_ue_index_t ue_idx) = 0;

  /// Notifies the scheduler that the results for a given slot were partially or fully discarded by the lower layers.
  /// \param[in] sl_tx Slot whose results were discarded.
  /// \param[in] event Which parts of the slot result were discarded.
  virtual void handle_error_indication(slot_point sl_tx, scheduler_slot_handler::error_outcome event) = 0;
};

} // namespace ocudu
