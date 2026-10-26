// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "cg_type2_resource_manager.h"
#include "configured_grant_scheduler_base.h"
#include "ocudu/scheduler/result/pdcch_info.h"

namespace ocudu {

class pdcch_resource_allocator;

/// \brief CG Type 2 scheduler.
///
/// The grant only becomes usable once an activation DCI scrambled with the CS-RNTI has been sent and the UE has
/// confirmed it, so this class adds the resource pool, the activation procedure and the re-activation of a UE that does
/// not confirm it. From the activation DCI on, its CG occasions are scheduled by \ref configured_grant_scheduler_base
/// like a Type 1 one.
class configured_grant_type2_sched_impl : public configured_grant_scheduler_base
{
public:
  explicit configured_grant_type2_sched_impl(const cell_configuration& cell_cfg_,
                                             const cg_builder_params&  cell_cg_cfg,
                                             uci_allocator&            uci_alloc_,
                                             pdcch_resource_allocator& pdcch_alloc_,
                                             ue_repository&            ues_);

  void run_slot(cell_resource_allocator& res_alloc) override;

  void rem_ue(const ue_cell_configuration& ue_cfg) override;

  void add_reconf_ue(const ue_cell_configuration& new_ue_cfg, const ue_cell_configuration* old_ue_cfg) override;

  void start_ue_cg_activation(du_ue_index_t ue_idx) override;

  void handle_cg_activation_complete_indication(du_ue_index_t ue_idx) override;

  void handle_error_indication(slot_point sl_tx, scheduler_slot_handler::error_outcome event) override;

  void stop() override;

private:
  // UE whose CG Type 2 activation is not confirmed yet, with the state of its latest activation DCI.
  struct ue_pending_activation {
    explicit ue_pending_activation(du_ue_index_t ue_index_) : ue_index(ue_index_) {}

    du_ue_index_t ue_index;
    // Slot in which the PDCCH of the latest activation DCI was allocated; invalid until the first one is allocated.
    // Needed to detect that the activation DCI never reached the UE, because the lower layers discarded the PDCCHs of
    // that slot.
    slot_point activation_pdcch_slot;
    // Slot by which the latest activation DCI must be confirmed; invalid while no activation DCI is awaiting
    // confirmation.
    slot_point confirmation_deadline;
    // Set when the latest activation DCI was not confirmed by its deadline, or its PDCCH was discarded. The UE then
    // gets a new activation DCI for the same grant.
    bool reactivation_due = false;
  };

  // Adds a UE's CG grant positions to the slot wheel, one every CG period.
  void add_ue_to_wheel(const ue_cell_configuration& ue_cfg, unsigned time_domain_offset);

  // Scans the list of UEs pending activation and sends the activation DCIs of this slot.
  void run_ues_activation(cell_resource_allocator& cell_alloc);

  /// Outcome of an attempt to send an activation DCI to one UE, which tells run_ues_activation() whether the UEs queued
  /// behind it are worth trying in this slot.
  enum class activation_outcome {
    /// The activation DCI was allocated.
    success,
    /// This UE could not be activated, for a reason of its own: no PDCCH space at the aggregation level its CQI calls
    /// for, no SearchSpace carrying DCI format 0_1, or no time-domain resource it can use. Another UE may well
    /// succeed, so the caller moves on to the next one.
    skip_ue,
    /// No UE can be activated in this slot, as the resource that ran out is shared by all of them.
    stop_slot
  };

  // Picks a CG resource, offset and TDRA row for a UE with no grant yet, and sends the activation DCI for them.
  activation_outcome activate_ue(cell_resource_allocator& cell_alloc, ue_pending_activation& pending, ue& u);

  // Sends a new activation DCI for the grant the UE already holds: same resource, offset, VRBs and MCS.
  activation_outcome reactivate_ue(cell_resource_allocator& cell_alloc, ue_pending_activation& pending, ue& u);

  // Records an activation DCI sent in \c pdcch_slot, whose first CG transmission is in \c first_tx_slot.
  void on_activation_dci_allocated(ue_pending_activation& pending, slot_point pdcch_slot, slot_point first_tx_slot);

  // Marks for re-activation the UEs whose latest activation DCI was not confirmed by its deadline.
  void check_confirmation_deadlines(slot_point slot);

  pdcch_ul_information* alloc_pdcch_ul_information(cell_slot_resource_allocator& pdcch_alloc,
                                                   du_ue_index_t                 ue_idx) const;

  // Any CG configuration is handled here: for CG Type 2 the grant is established by the activation DCI, not by RRC.
  bool has_cg_grant_cfg(const ue_cell_configuration& ue_cfg) const override;

  pdcch_resource_allocator& pdcch_sch;
  cg_type2_resource_manager res_mng;

  // [Implementation-defined] Max expected delay with which lower layers report CRC for a given PUSCH.
  static constexpr unsigned max_expected_crc_delay = 50U;

  // List of UEs that are waiting for to be activated for CG type2.
  std::vector<ue_pending_activation> ues_pending_activation;
};

} // namespace ocudu
