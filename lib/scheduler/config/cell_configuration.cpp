// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cell_configuration.h"
#include "time_domain_mapper.h"
#include "ocudu/adt/format.h"
#include "ocudu/ntn/ntn_cell_params.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"

using namespace ocudu;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell_configuration::cell_configuration(const scheduler_expert_config&                  expert_cfg_,
                                       const sched_cell_configuration_request_message& msg,
                                       const sched_bwp_config&                         init_bwp_) :
  expert_cfg(expert_cfg_),
  params(msg.ran),
  cell_index(msg.cell_index),
  cell_group_index(msg.cell_group_index),
  rrm_policy_members(msg.rrm_policy_members),
  max_nof_ue_contexts(msg.max_nof_ue_contexts),
  // Derived parameters.
  nof_dl_prbs(
      get_max_Nprb(params.dl_carrier.carrier_bw, scs_common(), band_helper::get_freq_range(params.dl_carrier.band))),
  nof_ul_prbs(
      get_max_Nprb(params.ul_carrier.carrier_bw, scs_common(), band_helper::get_freq_range(params.ul_carrier.band))),
  nof_slots_per_frame(get_nof_slots_per_subframe(scs_common()) * NOF_SUBFRAMES_PER_FRAME),
  csi_report_trigger_size(
      params.init_bwp.csi.has_value() ? csi_helper::get_csi_report_trigger_size(*params.init_bwp.csi) : 0),
  init_bwp(init_bwp_),
  // NTN parameters.
  ntn_cs_koffset(get_ntn_cs_koffset_slots(params.ntn_params, scs_common())),
  ntn_k_mac_slots(get_ntn_k_mac_slots(params.ntn_params, scs_common())),
  ntn_ref_location_ul_ta(get_ref_location_ul_ta(params.ntn_params))
{
  // Initialize BWP resources.
  bwp_res.emplace(to_bwp_id(0), params, to_bwp_id(0));

  if (is_tdd()) {
    // Cache list of DL and UL slots in case of TDD
    const unsigned tdd_period_slots = nof_slots_per_tdd_period(*params.tdd_cfg);
    dl_symbols_per_slot_lst.resize(tdd_period_slots);
    ul_symbols_per_slot_lst.resize(tdd_period_slots);
    for (unsigned slot_period_idx = 0; slot_period_idx < dl_symbols_per_slot_lst.size(); ++slot_period_idx) {
      dl_symbols_per_slot_lst[slot_period_idx] =
          get_active_tdd_dl_symbols(
              *params.tdd_cfg, slot_period_idx, params.dl_cfg_common.init_dl_bwp.generic_params.cp)
              .length();
      ul_symbols_per_slot_lst[slot_period_idx] =
          get_active_tdd_ul_symbols(
              *params.tdd_cfg, slot_period_idx, params.ul_cfg_common.init_ul_bwp.generic_params.cp)
              .length();
    }
  }
}
