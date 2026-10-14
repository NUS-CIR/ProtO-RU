// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/slotted_array.h"
#include "ocudu/scheduler/config/cell_bwp_res_config.h"
#include "ocudu/scheduler/config/serving_cell_config.h"
#include "ocudu/scheduler/rrm/ue_capability_summary.h"
#include "ocudu/scheduler/scheduler_configurator.h"
#include <optional>
#include <set>

namespace ocudu {

/// \brief This class manages the allocation of RAN PUCCH Resources to DU UEs.
///
/// \remark No PUCCH resources for CSI will be managed for cells configured with aperiodic CSI reporting.
class pucch_resource_manager
{
public:
  explicit pucch_resource_manager(unsigned max_pucch_grants_per_slot_);
  pucch_resource_manager(const pucch_resource_manager&)            = delete;
  pucch_resource_manager(pucch_resource_manager&&)                 = default;
  pucch_resource_manager& operator=(const pucch_resource_manager&) = delete;
  pucch_resource_manager& operator=(pucch_resource_manager&&)      = delete;

  /// Add resources for a new DU cell.
  void add_cell(du_cell_index_t cell_idx, const ran_cell_config& cell_cfg);

  /// Remove all resources for a DU cell.
  void rem_cell(du_cell_index_t cell_idx);

  /// \brief Allocate PUCCH resources for a given UE. The resources are stored in the UE's cell configuration.
  ///
  /// \remark PUCCH repetition is disabled until \c update_resources is called with the UE's reported capabilities.
  /// \return true if allocation was successful. On failure, the UE's cell configuration is not changed.
  bool alloc_resources(ue_cell_config& ue_cell_cfg);

  /// \brief Deallocate PUCCH resources previously given to a UE. The resources are returned back to a pool.
  /// \remark Call it only for a UE whose \c alloc_resources call succeeded.
  void dealloc_resources(const ue_cell_config& ue_cell_cfg);

  /// \brief Update the UE's PUCCH resources based on its reported capabilities.
  ///
  /// Enables PUCCH repetition for the resources the UE has indicated support for.
  /// \remark Call it only for a UE whose \c alloc_resources call succeeded.
  void update_resources(ue_cell_config& ue_cell_cfg, const ue_capability_summary& ue_caps);

  /// Gets the current number of free SR configurations for a given cell.
  unsigned get_nof_free_sr_configs(du_cell_index_t cell_idx) const { return cells[cell_idx].free_sr_configs.size(); }

  /// Gets the current number of free periodic CSI configurations for a given cell.
  unsigned get_nof_free_csi_configs(du_cell_index_t cell_idx) const { return cells[cell_idx].free_csi_configs.size(); }

  /// Whether a given cell is configured.
  bool contains(du_cell_index_t cell_index) const { return cells.contains(cell_index); }

private:
  struct periodic_pucch_config {
    unsigned res;
    unsigned offset;
  };

  struct cell_resource_context {
    ran_cell_config     cell_params;
    cell_bwp_res_config cell_bwp_cfg;
    unsigned            lcm_csi_sr_period;
    unsigned            sr_period_slots;
    unsigned            csi_period_slots;
    /// Pool of SR configurations currently available to be allocated to UEs.
    std::vector<periodic_pucch_config> free_sr_configs;
    /// Pool of periodic CSI configurations currently available to be allocated to UEs.
    std::vector<periodic_pucch_config> free_csi_configs;
    /// UE index for randomization of resources.
    unsigned              ue_idx = 0;
    std::vector<unsigned> periodic_pucchs_per_slot;

    /// CSI report on PUCCH. Null if the cell configures no periodic CSI reports.
    const csi_report_configuration* pucch_csi_report() const
    {
      const auto& csi = cell_bwp_cfg.dl.csi;
      return csi.has_value() and csi->pucch_report.has_value() ? &*csi->pucch_report : nullptr;
    }
  };

  const unsigned max_pucch_grants_per_slot;

  /// Resources for the different cells of the DU.
  slotted_id_table<du_cell_index_t, cell_resource_context, MAX_NOF_DU_CELLS> cells;

  /// \brief Get a free CSI configuration compatible with the given SR configuration, if any exists.
  ///
  /// For a CSI configuration to be compatible, the following must hold:
  /// - The max PUCCH grants limits are respected.
  /// - If the SR and CSI offsets collide, then the resulting UCI payload (SR + CSI) can still be transmitted within a
  ///   single PUCCH transmission (i.e., it doesn't exceed the max PUCCH payload limits).
  /// - If PUCCH is configured with Formats 0 and 2, the SR and CSI resources must collide in OFDM symbols.
  ///
  /// \return An iterator to a compatible CSI configuration in the free CSI list, or \c free_csi_list.end() if no
  ///         compatible configuration is found.
  std::vector<periodic_pucch_config>::const_iterator
  get_compatible_csi_cfg(const cell_resource_context&              cell_ctx,
                         const periodic_pucch_config&              sr_cfg,
                         const std::vector<periodic_pucch_config>& free_csi_list,
                         unsigned                                  csi_report_size) const;

  /// Check whether allocating a UE with the given SR or CSI offset will exceed the max PUCCH grants limit in any slot.
  bool offset_exceeds_limit(const cell_resource_context& cell_ctx, unsigned offset, bool csi) const;

  /// \brief Get the list of slot offsets where the UE will be scheduled for periodic UCI.
  ///
  /// \remark The offsets are computed within a period lcm(sr_period, csi_period).
  /// \remark Slot offsets common to SR and CSI are counted only once, as they will be sent together in one PUCCH.
  static std::set<unsigned>
  compute_periodic_uci_slot_offsets(const cell_resource_context& cell_ctx, unsigned sr_offset, unsigned csi_offset = 0);

  /// \brief Enable PUCCH repetition conditionally based on the UE's reported capabilities.
  ///
  /// \remark If \c ue_caps is not provided, repetition is disabled for all resources.
  void apply_rep_factor_capabilities(ue_cell_config& cell_cfg, const std::optional<ue_capability_summary>& ue_caps);
};

} // namespace ocudu
