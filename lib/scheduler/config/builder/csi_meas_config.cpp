// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/ran/csi_rs/csi_meas_config.h"
#include "ocudu/adt/format.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_builder.h"

using namespace ocudu;

std::optional<csi_meas_config> config_helpers::build_csi_meas_config(const ran_cell_config&        cell_cfg,
                                                                     const cell_ul_bwp_res_config& cell_ul_bwp_res,
                                                                     const ue_bwp_config&          ue_bwp_cfg)
{
  if (not cell_cfg.init_bwp.csi.has_value()) {
    return std::nullopt;
  }

  csi_helper::csi_meas_config_builder_params csi_params;
  csi_params.pci            = cell_cfg.pci;
  csi_params.nof_rbs        = cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  csi_params.nof_ports      = cell_cfg.dl_carrier.nof_ant;
  csi_params.max_nof_layers = cell_cfg.init_bwp.pdsch.max_nof_layers;
  csi_params.mcs_table      = cell_cfg.init_bwp.pdsch.mcs_table;
  csi_params.csi_params     = cell_cfg.init_bwp.csi.value();
  csi_meas_config cfg       = csi_helper::make_csi_meas_config(
      csi_params, cell_cfg.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list);

  if (ue_bwp_cfg.ul.csi.periodic_report.has_value()) {
    auto& report_cfg = std::get<csi_report_config::periodic_or_semi_persistent_report_on_pucch>(
        cfg.csi_report_cfg_list[0].report_cfg_type);
    report_cfg.report_slot_offset = ue_bwp_cfg.ul.csi.periodic_report->offset;
    report_cfg.pucch_csi_res_list.front().pucch_res_id =
        cell_cfg.init_bwp.pucch.resources.csi_res_id(ue_bwp_cfg.ul.csi.periodic_report->pucch_res_id);
  }

  return cfg;
}
