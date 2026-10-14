// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "du_csi_resource_manager.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"

using namespace ocudu;
using namespace odu;

du_csi_resource_manager::du_csi_resource_manager(span<const du_cell_config> cell_cfg_list_,
                                                 const du_test_mode_config& test_cfg_) :
  cell_cfg_list(cell_cfg_list_), test_cfg(test_cfg_), logger(ocudulog::fetch_basic_logger("DU-MNG"))
{
}

void du_csi_resource_manager::alloc_resources(cell_group_config& cell_grp_cfg, du_ue_index_t ue_index)
{
  apply_config(cell_grp_cfg, std::nullopt, ue_index);
}

void du_csi_resource_manager::update_resources(cell_group_config&           cell_grp_cfg,
                                               const ue_capability_summary& ue_caps,
                                               du_ue_index_t                ue_index)
{
  apply_config(cell_grp_cfg, ue_caps, ue_index);
}

void du_csi_resource_manager::dealloc_resources(cell_group_config& /* cell_grp_cfg */)
{
  // No pool resources to release.
}

void du_csi_resource_manager::apply_config(cell_group_config&                          cell_grp_cfg,
                                           const std::optional<ue_capability_summary>& ue_caps,
                                           du_ue_index_t                               ue_index)
{
  if (not cell_grp_cfg.cells.contains(SERVING_PCELL_IDX)) {
    return;
  }

  ue_cell_config&       ue_cell_cfg   = cell_grp_cfg.cells.at(SERVING_PCELL_IDX);
  serving_cell_config&  serv_cell_cfg = ue_cell_cfg.serv_cell_cfg;
  const du_cell_config& cell_cfg      = cell_cfg_list[serv_cell_cfg.cell_index];
  if (not serv_cell_cfg.csi_meas_cfg.has_value() or not cell_cfg.ran.init_bwp.csi.has_value()) {
    return;
  }

  // The cell decides whether the Type-II codebook is offered.
  if (not cell_cfg.ran.init_bwp.csi->type2_codebook.has_value()) {
    // The cell configures the Type-I codebook for every UE, so the capabilities do not change the outcome.
    return;
  }

  const char* unsupported_reason = type2_unsupported_reason(serv_cell_cfg.cell_index, ue_caps);

  // The scheduler reads the codebook from the UE BWP configuration, and RRC reads it from the CSI-MeasConfig.
  ue_cell_cfg.init_bwp().ul.csi_codebook =
      unsupported_reason == nullptr ? csi_codebook_type::type2 : csi_codebook_type::type1;

  const csi_helper::csi_meas_config_builder_params csi_params =
      config_helpers::make_csi_meas_config_builder_params(cell_cfg.ran);

  for (csi_report_config& report_cfg : serv_cell_cfg.csi_meas_cfg->csi_report_cfg_list) {
    if (not report_cfg.codebook_cfg.has_value()) {
      continue;
    }

    if (unsupported_reason == nullptr) {
      report_cfg.codebook_cfg->codebook_type = csi_helper::make_type2_codebook_config(csi_params);
    } else {
      report_cfg.codebook_cfg->codebook_type = csi_helper::make_type1_codebook_config(csi_params);
    }
  }

  if (unsupported_reason != nullptr) {
    // Logged because the cell offers the Type-II codebook and the operator otherwise has no way to tell why this UE
    // ended up with the Type-I one.
    logger.info("ue={}: Configuring the Type-I codebook. Cause: {}", ue_index, unsupported_reason);
  }
}

const char* du_csi_resource_manager::type2_unsupported_reason(du_cell_index_t                             cell_idx,
                                                              const std::optional<ue_capability_summary>& ue_caps) const
{
  const du_cell_config&           cell_cfg   = cell_cfg_list[cell_idx];
  const du_type2_codebook_params& cell_type2 = cell_cfg.ran.init_bwp.csi->type2_codebook.value();

  if (not ue_caps.has_value()) {
    if (test_cfg.test_ue.has_value() and test_cfg.test_ue->rnti != rnti_t::INVALID_RNTI) {
      // Has no capabilities but the UE is in test mode.
      return nullptr;
    }

    return "UE capabilities have not been decoded yet";
  }

  const auto band_it = ue_caps->bands.find(cell_cfg.ran.dl_carrier.band);
  if (band_it == ue_caps->bands.end() or not band_it->second.type2_codebook.has_value()) {
    return "UE does not support the Type-II codebook";
  }

  const ue_type2_codebook_params& ue_type2 = band_it->second.type2_codebook.value();
  if (ue_type2.max_nof_beams < cell_type2.nof_beams) {
    return "UE supports fewer beams than the cell configures";
  }
  if (ue_type2.max_nof_tx_ports_per_resource < cell_cfg.ran.dl_carrier.nof_ant) {
    return "UE supports fewer CSI-RS ports than the cell configures";
  }

  return nullptr;
}
