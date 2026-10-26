// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/adt/format.h"
#include "ocudu/scheduler/config/serving_cell_config_builder.h"
#include "ocudu/scheduler/config/time_domain_resource_helper.h"

using namespace ocudu;

/// Build the default PUCCH-PowerControl configuration.
static pucch_power_control build_pucch_power_control()
{
  pucch_power_control pw_ctrl;
  pw_ctrl.delta_pucch_f0 = 0;
  pw_ctrl.delta_pucch_f1 = 0;
  pw_ctrl.delta_pucch_f2 = 0;
  pw_ctrl.delta_pucch_f3 = 0;
  pw_ctrl.delta_pucch_f4 = 0;

  auto& pucch_pw_set = pw_ctrl.p0_set.emplace_back();
  pucch_pw_set.id    = 0U;
  pucch_pw_set.value = 0;

  return pw_ctrl;
}

pucch_config config_helpers::build_pucch_config(const ran_cell_config&        cell_cfg,
                                                const cell_ul_bwp_res_config& cell_ul_bwp_res,
                                                const ue_bwp_config&          ue_bwp_cfg)
{
  const pucch_resource_builder_params& res_params                 = cell_cfg.init_bwp.pucch.resources;
  const auto&                          cell_resources             = cell_ul_bwp_res.pucch;
  const auto&                          ue_pucch_cfg               = ue_bwp_cfg.ul.pucch;
  const auto&                          ue_periodic_csi_report_cfg = ue_bwp_cfg.ul.csi.periodic_report;

  const auto   max_code_rate = res_params.max_code_rate_234();
  pucch_config cfg{
      .format_1_common_param = pucch_common_all_formats{},
      .format_2_common_param = pucch_common_all_formats{.max_c_rate = max_code_rate, .simultaneous_harq_ack_csi = true},
      .format_3_common_param = pucch_common_all_formats{.max_c_rate = max_code_rate, .simultaneous_harq_ack_csi = true},
      .format_4_common_param = pucch_common_all_formats{.max_c_rate = max_code_rate, .simultaneous_harq_ack_csi = true},
      .dl_data_to_ul_ack =
          time_domain_resource_helper::generate_k1_candidates(cell_cfg.tdd_cfg, cell_cfg.init_bwp.pucch.min_k1),
      .pucch_pw_control = build_pucch_power_control(),
  };

  auto& res_list    = cfg.pucch_res_list;
  auto& res_set_0   = cfg.pucch_res_set.emplace_back();
  res_set_0.id      = pucch_res_set_id::set_0;
  auto& res_set_1   = cfg.pucch_res_set.emplace_back();
  res_set_1.id      = pucch_res_set_id::set_1;
  auto& sr_res_list = cfg.sr_res_list;

  const bool using_02 = cell_cfg.init_bwp.pucch.resources.format_01() == pucch_format::FORMAT_0 and
                        cell_cfg.init_bwp.pucch.resources.format_234() == pucch_format::FORMAT_2;

  // Add Resource Set ID 0 resources for HARQ-ACK.
  for (unsigned r_pucch = 0; r_pucch != res_params.res_set_size.value(); ++r_pucch) {
    const auto& cell_res = cell_resources.get_ded(res_params.harq_res_id<0>(ue_pucch_cfg.res_set_cfg_id, r_pucch));

    // Add resource to both the PUCCH resource list and Resource Set ID 0.
    res_list.emplace_back(cell_res);
    res_set_0.resources.emplace_back(cell_res.res_id);
  }

  // Add SR resource to both the PUCCH resource list and the SR resource list.
  const auto& sr_res = cell_resources.get_ded(res_params.sr_res_id(ue_pucch_cfg.sr_res_id));
  res_list.emplace_back(sr_res);
  sr_res_list.emplace_back(scheduling_request_resource_config{.sr_res_id    = 1,
                                                              .sr_id        = uint_to_sched_req_id(0),
                                                              .period       = cell_cfg.init_bwp.pucch.sr_period,
                                                              .offset       = ue_pucch_cfg.sr_offset,
                                                              .pucch_res_id = sr_res.res_id});

  // Add Resource Set ID 1 resources for HARQ-ACK.
  for (unsigned r_pucch = 0; r_pucch != res_params.res_set_size.value(); ++r_pucch) {
    const auto& res = cell_resources.get_ded(res_params.harq_res_id<1>(ue_pucch_cfg.res_set_cfg_id, r_pucch));

    // Add resource to both the PUCCH resource list and Resource Set ID 1.
    res_list.emplace_back(res);
    res_set_1.resources.emplace_back(res.res_id);
  }

  if (ue_periodic_csi_report_cfg.has_value()) {
    // Add CSI resource to the CSI resource list.
    const auto& csi_res = cell_resources.get_ded(res_params.csi_res_id(ue_periodic_csi_report_cfg->pucch_res_id));
    res_list.emplace_back(csi_res);
  }

  if (using_02) {
    // [Implementation-defined] We build the PUCCH-Config of all UEs in a way that will prevent the scheduling of
    // multiple PUCCH transmissions per UE per slot, since not all UEs are capable of that. To achieve this, we make
    // sure that there is always an option to schedule different UCI types on overlapping symbols, so that the UE will
    // multiplex the different UCI types on a single PUCCH.
    //
    // This is done differently depending on the configured PUCCH formats:
    // - F1 and F2/F3/F4: since we configure F1 resources to always take all available symbols in the slot, all F2/F3/F4
    //   resources will overlap in symbols with F1 resources.
    // - F0 and F3/F4: for now we do not allow this combination. TODO: figure out this case.
    // - F0 and F2: we reserve two resources in each Resource Set to be used for HARQ-ACK when CSI or SR is scheduled.
    //
    // The diagram below illustrates the configuration of the Resource Sets for this last case:
    //
    //  +--------+--------+--------+--------+--------+    +--------+--------+--------+--------+--------+
    //  | R_0    | ...    | R_N-1  | SR_F0  | CSI_F0 |    | R_0    | ...    | R_N-1  | SR_F2  | CSI_F2 |
    //  +--------+--------+--------+--------+--------+    +--------+--------+--------+--------+--------+
    //             PUCCH Resource Set ID 0                           PUCCH Resource Set ID 1
    //
    // Where:
    // - R_i are regular PUCCH resources for HARQ-ACK.
    // - SR_F0 is the same F0 resource in the PUCCH resource list as the SR resource.
    // - CSI_F0 is a F0 resource in Resource Set ID 0 that overlaps in symbols with the CSI (F2) resource.
    // - SR_F2 is a F2 resource in Resource Set ID 1 that overlaps in symbols with the SR (F0) resource.
    // - CSI_F2 is the same F2 resource in the PUCCH resource list as the CSI resource.
    res_set_0.resources.emplace_back(sr_res.res_id);

    const auto& sr_f2_res = cell_resources.get_ded(res_params.sr_f2_res_id(ue_pucch_cfg.sr_res_id));
    res_list.emplace_back(sr_f2_res);
    res_set_1.resources.emplace_back(sr_f2_res.res_id);

    if (ue_periodic_csi_report_cfg.has_value()) {
      const auto& csi_f0_res =
          cell_resources.get_ded(res_params.csi_f0_res_id(ue_periodic_csi_report_cfg->pucch_res_id));
      res_list.emplace_back(csi_f0_res);
      res_set_0.resources.emplace_back(csi_f0_res.res_id);

      const auto& csi_res = cell_resources.get_ded(res_params.csi_res_id(ue_periodic_csi_report_cfg->pucch_res_id));
      res_set_1.resources.emplace_back(csi_res.res_id);
    }
  }

  return cfg;
}
