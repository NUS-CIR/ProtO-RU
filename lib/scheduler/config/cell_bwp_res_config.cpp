// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/config/cell_bwp_res_config.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/csi_report/csi_report_config_helpers.h"
#include "ocudu/scheduler/config/bwp_builder_params.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/pucch_default_resource.h"
#include "ocudu/scheduler/config/pucch_resource_generator.h"
#include "ocudu/scheduler/config/ran_cell_config.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"

using namespace ocudu;

static std::optional<cell_bwp_csi_config> make_cell_bwp_csi_config(const ran_cell_config& cell_cfg)
{
  if (not cell_cfg.init_bwp.csi.has_value()) {
    return std::nullopt;
  }

  // The UE resources of the CSI-MeasConfig do not change the cell parameters.
  csi_helper::csi_meas_config_builder_params params = config_helpers::make_csi_meas_config_builder_params(cell_cfg);
  const auto&           pusch_td_alloc_list = cell_cfg.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
  const csi_meas_config csi_meas            = csi_helper::make_csi_meas_config(params, pusch_td_alloc_list);

  cell_bwp_csi_config csi{
      .cqi_table    = csi_meas.csi_report_cfg_list.front().cqi_table.value(),
      .report_type  = is_pusch_configured(csi_meas) ? csi_report_type::aperiodic : csi_report_type::periodic,
      .type1_report = {},
  };
  if (params.csi_params.type2_codebook.has_value()) {
    csi.type2_report = create_csi_report_configuration(csi_meas);
    params.csi_params.type2_codebook.reset();
    csi.type1_report = create_csi_report_configuration(csi_helper::make_csi_meas_config(params, pusch_td_alloc_list));
  } else {
    csi.type1_report = create_csi_report_configuration(csi_meas);
  }
  return csi;
}

static cell_dl_bwp_res_config make_cell_dl_bwp_res_config(const ran_cell_config& cell_cfg)
{
  cell_dl_bwp_res_config res;
  if (cell_cfg.init_bwp.pdcch_cfg.has_value()) {
    res.ded_pdcchs.push_back(*cell_cfg.init_bwp.pdcch_cfg);
  }
  res.csi = make_cell_bwp_csi_config(cell_cfg);
  return res;
}

cell_bwp_res_config ocudu::make_cell_bwp_res_config(const ran_cell_config& cell_cfg)
{
  const unsigned bwp_size_rbs     = cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  const unsigned pucch_res_common = cell_cfg.ul_cfg_common.init_ul_bwp.pucch_cfg_common.value().pucch_resource_common;
  return cell_bwp_res_config{
      .dl = make_cell_dl_bwp_res_config(cell_cfg),
      .ul = {.pucch = {
                 .common    = config_helpers::generate_cell_common_pucch_res_list(pucch_res_common, bwp_size_rbs),
                 .dedicated = config_helpers::generate_cell_pucch_res_list(
                     cell_cfg.init_bwp.pucch.resources,
                     bwp_size_rbs,
                     config_helpers::get_dedicated_pucch_rb_start(
                         cell_cfg.init_bwp.pucch.resources, pucch_res_common, bwp_size_rbs)),

             }}};
}
