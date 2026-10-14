// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/slotted_array.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/du_cell_index.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/rrm/ue_capability_summary.h"
#include <optional>

namespace ocudu {

struct ran_cell_config;
struct ue_cell_config;

/// \brief Manages the CSI report configuration of DU UEs.
///
/// Applies the codebook that every UE supports on UE creation and selects the codebook offered by the cell once the UE
/// capabilities are known.
class csi_resource_manager
{
public:
  /// \param type2_without_ue_caps_ Set it to configure the Type-II codebook for UEs without capabilities (test mode).
  explicit csi_resource_manager(bool type2_without_ue_caps_);

  /// Add a new DU cell.
  void add_cell(du_cell_index_t cell_idx, const ran_cell_config& cell_cfg);

  /// \brief Sets the CSI codebook of a newly created UE (before capabilities are decoded).
  void alloc_resources(ue_cell_config& ue_cell_cfg, du_ue_index_t ue_index);

  /// \brief Updates the CSI codebook based on decoded UE capabilities.
  void update_resources(ue_cell_config& ue_cell_cfg, const ue_capability_summary& ue_caps, du_ue_index_t ue_index);

private:
  /// Parameters of a cell that offers the Type-II codebook.
  struct type2_cell_context {
    nr_band                                    band;
    csi_helper::csi_meas_config_builder_params csi_params;
  };

  void apply_config(ue_cell_config&                             ue_cell_cfg,
                    const std::optional<ue_capability_summary>& ue_caps,
                    du_ue_index_t                               ue_index);

  /// \brief Determines whether the Type-II codebook offered by a cell can be configured for a UE.
  ///
  /// The support is reported per band in field \e type2 of \e codebookParameters, in Information Element
  /// \e MIMO-ParametersPerBand.
  ///
  /// \return \c nullptr if the codebook can be configured, otherwise the reason why it cannot.
  const char* type2_unsupported_reason(const type2_cell_context&                   cell_ctx,
                                       const std::optional<ue_capability_summary>& ue_caps) const;

  const bool              type2_without_ue_caps;
  ocudulog::basic_logger& logger;

  /// Cells that offer the Type-II codebook.
  slotted_id_table<du_cell_index_t, type2_cell_context, MAX_NOF_DU_CELLS> type2_cells;
};

} // namespace ocudu
