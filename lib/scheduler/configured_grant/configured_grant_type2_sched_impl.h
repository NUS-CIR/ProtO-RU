// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "cg_type2_resource_manager.h"
#include "configured_grant_scheduler_base.h"
#include "ocudu/adt/slotted_array.h"
#include "ocudu/scheduler/result/pdcch_info.h"

namespace ocudu {

class pdcch_resource_allocator;

/// \brief CG Type 2 scheduler.
///
/// The grant only becomes usable once an activation DCI scrambled with the CS-RNTI has been sent and the UE has
/// confirmed it, so this class adds the resource pool, the activation procedure and the detection of a UE that never
/// received the activation. Once a UE is activated, its CG occasions are scheduled by
/// \ref configured_grant_scheduler_base like a Type 1 one.
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
  // UEs whose CG configuration has been added or updated in between the last and current slot indications. Their CG
  // resources still need to be pre-reserved over the whole resource grid.
  struct ue_pending_activation {
    explicit ue_pending_activation(du_ue_index_t ue_index_) : ue_index(ue_index_) {}

    du_ue_index_t ue_index;
    // Set once an activation DCI has been allocated for this UE and cleared when that attempt is abandoned, so that
    // run_ues_activation() knows whether an activation is still outstanding.
    bool activation_triggered = false;
    // Slot in which the activation PDCCH was allocated. Needed to detect that the activation DCI never reached the UE,
    // because the lower layers discarded the PDCCHs of that slot.
    slot_point activation_pdcch_slot;

    // Number of PUSCH occasions for which an expected CRC indication never arrived. Reset when the activation attempt
    // is abandoned, so that each attempt starts from a clean state.
    unsigned missed_grants_cnt = 0;

    // [Implementation-defined] After 5 PUSCH grants expecting non-DTX CRC, we assume the UE hasn't received the
    // activation DCI.
    static constexpr unsigned max_nof_missed_crc_events = 5U;
    // Container that stores the slot_points at which the GNB expects CRC indications; missing these without activation
    // confirmation may indicate the UE never receives the DCI for CG activation. Entries are removed either when the
    // corresponding CRC arrives or when they expire, and their position carries no meaning.
    slotted_array<slot_point, max_nof_missed_crc_events> slots_expecting_crc;
  };

  // Adds a UE's CG grant positions to the slot wheel, one every CG period. Used by add_reconf_ue().
  void add_ue_to_wheel(const ue_cell_configuration& ue_cfg, unsigned time_domain_offset);

  // Scans the list of UEs pending activation and proceeds with activation procedure.
  void run_ues_activation(cell_resource_allocator& cell_alloc);

  // Schedules DCI for CG type 2 activation, containing all the required information for CG grant (i.e., VRBs, MCS,
  // TDRA, CG offset, etc.).
  /// Outcome of an attempt to activate one UE, which tells run_ues_activation() whether the UEs queued behind it are
  /// worth trying in this slot.
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

  activation_outcome activate_ue(cell_resource_allocator& cell_alloc, ue& ue_index);

  // Scans the list of UEs that got CG activation initiated, but never acked it; for those UEs, proceed with releasing
  // the previously allocated resources.
  void ues_resource_release(slot_point slot);

  // Undoes an activation attempt: returns the CG resource to the pool, removes the UE from the periodic wheel and
  // drops its grant info, leaving the UE in the pending list so that run_ues_activation() retries it.
  void release_ue_activation(ue_pending_activation& u);

  pdcch_ul_information* alloc_pdcch_ul_information(cell_slot_resource_allocator& pdcch_alloc,
                                                   du_ue_index_t                 ue_idx) const;

  // Any CG configuration is handled here: for CG Type 2 the grant is established by the activation DCI, not by RRC.
  bool has_cg_grant_cfg(const ue_cell_configuration& ue_cfg) const override;

  // Records the slot as one in which a CRC indication is expected, which is how ues_resource_release() detects a UE
  // that never received the activation DCI.
  void on_cg_pusch_allocated(const ue& u, slot_point pusch_slot) override;

  pdcch_resource_allocator& pdcch_sch;
  cg_type2_resource_manager res_mng;

  // [Implementation-defined] Max expected delay with which lower layers report CRC for a given PUSCH.
  static constexpr unsigned max_expected_crc_delay = 50U;

  // List of UEs that are waiting for to be activated for CG type2.
  std::vector<ue_pending_activation> ues_pending_activation;
};

} // namespace ocudu
