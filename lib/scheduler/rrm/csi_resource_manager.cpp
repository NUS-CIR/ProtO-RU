// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/rrm/csi_resource_manager.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/scheduler_configurator.h"

using namespace ocudu;

csi_resource_manager::csi_resource_manager(bool type2_without_ue_caps_) :
  type2_without_ue_caps(type2_without_ue_caps_), logger(ocudulog::fetch_basic_logger("DU-MNG"))
{
}

void csi_resource_manager::add_cell(du_cell_index_t cell_idx, const ran_cell_config& cell_cfg)
{
  // The cell decides whether the Type-II codebook is offered.
  if (cell_cfg.init_bwp.csi.has_value() and cell_cfg.init_bwp.csi->type2_codebook.has_value()) {
    type2_cells.emplace(cell_idx,
                        type2_cell_context{
                            .band       = cell_cfg.dl_carrier.band,
                            .csi_params = config_helpers::make_csi_meas_config_builder_params(cell_cfg),
                        });
  }
}

void csi_resource_manager::alloc_resources(ue_cell_config& ue_cell_cfg, du_ue_index_t ue_index)
{
  apply_config(ue_cell_cfg, std::nullopt, ue_index);
}

void csi_resource_manager::update_resources(ue_cell_config&              ue_cell_cfg,
                                            const ue_capability_summary& ue_caps,
                                            du_ue_index_t                ue_index)
{
  apply_config(ue_cell_cfg, ue_caps, ue_index);
}

void csi_resource_manager::apply_config(ue_cell_config&                             ue_cell_cfg,
                                        const std::optional<ue_capability_summary>& ue_caps,
                                        du_ue_index_t                               ue_index)
{
  serving_cell_config& serv_cell_cfg = ue_cell_cfg.serv_cell_cfg;
  if (not serv_cell_cfg.csi_meas_cfg.has_value() or not type2_cells.contains(serv_cell_cfg.cell_index)) {
    // The cell configures the Type-I codebook for every UE, so the capabilities do not change the outcome.
    return;
  }
  const type2_cell_context& cell_ctx = type2_cells[serv_cell_cfg.cell_index];

  const char* unsupported_reason = type2_unsupported_reason(cell_ctx, ue_caps);

  // The scheduler reads the codebook from the UE BWP configuration, and RRC reads it from the CSI-MeasConfig.
  ue_cell_cfg.init_bwp().ul.csi.codebook =
      unsupported_reason == nullptr ? csi_codebook_type::type2 : csi_codebook_type::type1;

  for (csi_report_config& report_cfg : serv_cell_cfg.csi_meas_cfg->csi_report_cfg_list) {
    if (not report_cfg.codebook_cfg.has_value()) {
      continue;
    }

    if (unsupported_reason == nullptr) {
      report_cfg.codebook_cfg->codebook_type = csi_helper::make_type2_codebook_config(cell_ctx.csi_params);
    } else {
      report_cfg.codebook_cfg->codebook_type = csi_helper::make_type1_codebook_config(cell_ctx.csi_params);
    }
  }

  if (unsupported_reason != nullptr) {
    // Logged because the cell offers the Type-II codebook and the operator otherwise has no way to tell why this UE
    // ended up with the Type-I one.
    logger.info("ue={}: Configuring the Type-I codebook. Cause: {}", ue_index, unsupported_reason);
  }
}

const char* csi_resource_manager::type2_unsupported_reason(const type2_cell_context&                   cell_ctx,
                                                           const std::optional<ue_capability_summary>& ue_caps) const
{
  if (not ue_caps.has_value()) {
    if (type2_without_ue_caps) {
      // Has no capabilities but the UE is in test mode.
      return nullptr;
    }

    return "UE capabilities have not been decoded yet";
  }

  const auto band_it = ue_caps->bands.find(cell_ctx.band);
  if (band_it == ue_caps->bands.end() or not band_it->second.type2_codebook.has_value()) {
    return "UE does not support the Type-II codebook";
  }

  const ue_type2_codebook_params& ue_type2 = band_it->second.type2_codebook.value();
  if (ue_type2.max_nof_beams < cell_ctx.csi_params.csi_params.type2_codebook->nof_beams) {
    return "UE supports fewer beams than the cell configures";
  }
  if (ue_type2.max_nof_tx_ports_per_resource < cell_ctx.csi_params.nof_ports) {
    return "UE supports fewer CSI-RS ports than the cell configures";
  }

  return nullptr;
}
