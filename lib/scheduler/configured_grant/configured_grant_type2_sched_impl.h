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
  // UE whose CG Type 2 activation is not confirmed yet, with the state of its activation.
  //
  // The allowed transitions are:
  // - awaiting_first_activation -> awaiting_confirmation: the first activation DCI is allocated.
  // - awaiting_confirmation -> reactivation_due: the deadline passes, or the activation PDCCH is discarded.
  // - reactivation_due -> awaiting_confirmation: a new activation DCI for the same grant is allocated.
  // A confirmed UE leaves the list of UEs pending activation, so "confirmed" is not a state of its own. The UE can be
  // confirmed from awaiting_confirmation or reactivation_due, as every activation DCI carries the same grant.
  class ue_pending_activation
  {
  public:
    enum class state_t {
      // No activation DCI was sent yet for the UE's current CG configuration.
      awaiting_first_activation,
      // An activation DCI was sent, whose confirmation is awaited until the deadline.
      awaiting_confirmation,
      // The latest activation DCI was not confirmed, so a new one is due for the grant the UE already holds.
      reactivation_due
    };

    explicit ue_pending_activation(du_ue_index_t ue_index_) : ue_idx(ue_index_) {}

    du_ue_index_t ue_index() const { return ue_idx; }
    state_t       state() const { return st; }
    // Slot of the PDCCH of the latest activation DCI; invalid in awaiting_first_activation.
    slot_point activation_pdcch_slot() const { return pdcch_slot; }
    // Slot by which the latest activation DCI must be confirmed; invalid in awaiting_first_activation.
    slot_point confirmation_deadline() const { return deadline; }

    // Records an activation DCI sent in \c pdcch_slot_, which must be confirmed by \c deadline_.
    void on_activation_dci_allocated(slot_point pdcch_slot_, slot_point deadline_);

    // Moves to reactivation_due if the activation DCI is awaiting confirmation past its deadline. Returns whether it
    // did.
    bool check_confirmation_deadline(slot_point slot);

    // Moves to reactivation_due if the activation DCI awaiting confirmation was sent in \c sl_tx, whose PDCCH was
    // discarded. Returns whether it did.
    bool on_pdcch_discarded(slot_point sl_tx);

    // Whether a CG activation confirmation can be taken for this UE, i.e. an activation DCI was sent.
    bool can_be_confirmed() const { return st != state_t::awaiting_first_activation; }

  private:
    du_ue_index_t ue_idx;
    state_t       st = state_t::awaiting_first_activation;
    slot_point    pdcch_slot;
    slot_point    deadline;
  };

  // Adds a UE's CG grant positions to the slot wheel, one every CG period.
  void add_ue_to_wheel(const ue_cell_configuration& ue_cfg, unsigned time_domain_offset);

  // Scans the list of UEs pending activation and sends the activation DCIs of this slot.
  void run_ues_activation(cell_resource_allocator& cell_alloc);

  // Outcome of an attempt to send an activation DCI to one UE.
  enum class activation_outcome {
    // The activation DCI was allocated.
    success,
    // This UE could not be activated but another UE may well succeed.
    skip_ue,
    // No UE can be activated in this slot, as the resource that ran out is shared by all of them.
    stop_slot
  };

  // Picks a CG resource, offset and TDRA row for a UE with no grant yet, and sends the activation DCI for them.
  activation_outcome activate_ue(cell_resource_allocator& cell_alloc, ue_pending_activation& pending, ue& u);

  // Sends a new activation DCI for the grant the UE already holds: same resource, offset, VRBs and MCS.
  activation_outcome reactivate_ue(cell_resource_allocator& cell_alloc, ue_pending_activation& pending, ue& u);

  // Marks for re-activation the UEs whose latest activation DCI was not confirmed by its deadline.
  void check_confirmation_deadlines(slot_point slot);

  // Allocates the activation PDCCH in the UE's activation SearchSpace and fills in the activation DCI. Returns false
  // if the UE has no such SearchSpace or the PDCCH does not fit.
  bool alloc_activation_dci(cell_slot_resource_allocator& pdcch_alloc,
                            ue&                           u,
                            vrb_interval                  vrbs,
                            uint8_t                       td_alloc,
                            uint8_t                       mcs);

  // Returns the pending activation entry of a UE, or the end of the list if it has none.
  std::vector<ue_pending_activation>::iterator find_pending(du_ue_index_t ue_idx);

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
