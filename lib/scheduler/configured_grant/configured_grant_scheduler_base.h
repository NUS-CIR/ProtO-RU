// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../config/cell_configuration.h"
#include "../ue_context/ue_repository.h"
#include "cg_slot_wheel.h"
#include "configured_grant_scheduler.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/rnti.h"
#include <vector>

namespace ocudu {

struct cell_slot_resource_allocator;
class uci_allocator;

/// \brief Behaviour shared by the CG Type 1 and Type 2 schedulers.
///
/// Once a UE has a CG grant, the two types are scheduled identically: the grant parameters (VRBs, MCS, TBS, time domain
/// allocation and slot offset) are read from \ref ue_conf_grant_state_manager, which CG Type 1 fills from the
/// RRC-configured grant and CG Type 2 from the activation DCI. This class owns that common part — the periodic slot
/// wheel, the resource pre-reservation and the per-slot PUSCH allocation — leaving each derived scheduler with only
/// the part that establishes the grant in the first place.
class configured_grant_scheduler_base : public configured_grant_scheduler
{
public:
  configured_grant_scheduler_base(const cell_configuration& cell_cfg_,
                                  const cg_builder_params&  cell_cg_cfg,
                                  uci_allocator&            uci_alloc_,
                                  ue_repository&            ues_);

  /// Pre-reserves the CG resources in the resource grid and allocates the CG PUSCH occasions of the current slot.
  void run_slot(cell_resource_allocator& res_alloc) override;

  /// Called on cell deactivation.
  void stop() override;

protected:
  /// Whether the given UE configuration carries a CG grant this scheduler is responsible for. CG Type 1 additionally
  /// requires an RRC-configured grant, as a CG configuration without one is activated by DCI, i.e. Type 2.
  virtual bool has_cg_grant_cfg(const ue_cell_configuration& ue_cfg) const = 0;

  /// \brief Hook invoked after a CG PUSCH occasion has been added to the slot result.
  ///
  /// Used by CG Type 2 to record the slots in which a CRC indication is expected, which is how a UE that never
  /// received the activation DCI is detected. CG Type 1 needs no activation and leaves it unimplemented.
  virtual void on_cg_pusch_allocated(const ue& u, slot_point pusch_slot) {}

  /// Helper to fetch a UE cell by RNTI. Returns nullptr if either the UE or its cell is not found.
  const ue_cell* get_ue_cell(rnti_t rnti) const;

  /// Removes a UE from the list of recently updated UEs, if present. Keyed on the RNTI alone, so it can be called even
  /// when the UE is no longer in the repository.
  void rem_ue_from_updated_ues(rnti_t rnti);

  const cell_configuration& cell_cfg;
  uci_allocator&            uci_alloc;
  ue_repository&            ues;
  ocudulog::basic_logger&   logger;

  /// \brief CG periodicity of the cell, shared by every UE of the cell.
  const cg_configuration::periodicity_t cg_periodicity;

  /// Periodic CG PUSCH opportunities to be scheduled in the resource grid.
  cg_slot_wheel periodic_pusch_slot_wheel;

  /// UEs whose CG configuration has been added or updated in between the last and current slot indications. Their CG
  /// resources still need to be pre-reserved over the whole resource grid.
  std::vector<rnti_t> updated_ues;

private:
  // Pre-reserves the CG PUSCH resources over the whole resource grid for the UEs that were recently added or
  // reconfigured, and clears the list of updated UEs.
  void reserve_updated_ues_resources(cell_resource_allocator& res_alloc);

  // Pre-reserves the CG PUSCH RBs and symbols in the UL resource grid for the given slot, for all the UEs with a CG
  // opportunity in that slot, before any dynamic PUSCH grants are allocated in it.
  void reserve_slot_cg_resources(cell_slot_resource_allocator& slot_alloc) const;

  // Pre-reserves the CG PUSCH RBs and symbols of a single UE in the UL resource grid for the given slot.
  void reserve_cg_resources(cell_slot_resource_allocator& slot_alloc, const ue_cell& ue_cc) const;

  // Processes all CG PUSCH opportunities for the given slot.
  void allocate_slot_cg_opportunities(cell_slot_resource_allocator& slot_alloc);

  // Helper that runs preliminary checks on the CG opportunity validity.
  bool validate_cg_opportunity(cell_slot_resource_allocator& slot_alloc, rnti_t rnti) const;

  // Allocates a single CG PUSCH opportunity for the given RNTI.
  bool allocate_cg_opportunity(cell_slot_resource_allocator& slot_alloc, rnti_t rnti);
};

} // namespace ocudu
