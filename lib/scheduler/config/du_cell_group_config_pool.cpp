// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "du_cell_group_config_pool.h"
#include "cell_configuration.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/bwp/bwp_id.h"
#include "ocudu/scheduler/config/ue_bwp_config.h"
#include "ocudu/scheduler/scheduler_configurator.h"

using namespace ocudu;

bwp_config_pool::bwp_config_pool(const ran_cell_config&     cell_ran_cfg,
                                 bwp_id_t                   bwpid,
                                 const cell_bwp_res_config& bwp_ded_res) :
  bwp_id(bwpid),
  bwp_dl_cmn(cell_ran_cfg.dl_cfg_common.init_dl_bwp),
  bwp_ul_cmn(cell_ran_cfg.ul_cfg_common.init_ul_bwp),
  tdd_cfg(cell_ran_cfg.tdd_cfg),
  pucch_min_k1(cell_ran_cfg.init_bwp.pucch.min_k1),
  pdcch_pool(cell_ran_cfg.pci, bwp_dl_cmn.generic_params, bwp_dl_cmn.pdcch_common, bwp_ded_res.dl),
  common_bwp_cfg{bwp_id,
                 sched_bwp_dl_config{bwp_dl_cmn, nullptr, pdcch_pool.init_cfg(), get_dl_td_mapper(nullptr)},
                 sched_bwp_ul_config{bwp_ul_cmn, std::nullopt, std::nullopt, get_ul_td_mapper(nullptr)}}
{
}

sched_bwp_config bwp_config_pool::add_ded_cfg(const bwp_downlink_dedicated* dl_ded,
                                              const bwp_uplink_dedicated*   ul_ded,
                                              const ue_bwp_config&          ue_bwp_cfg)
{
  config_ptr<bwp_downlink_dedicated> dl_ptr;
  if (dl_ded != nullptr) {
    dl_ptr = dl_ded_config_pool.create(*dl_ded);
  }
  // The UL dedicated config (PUCCH/SR/SRS) is high-cardinality (differs per UE), so it is owned by value here rather
  // than interned in a shared pool that would grow unbounded.
  std::optional<bwp_uplink_dedicated> ul_owned;
  if (ul_ded != nullptr) {
    ul_owned = *ul_ded;
  }

  return sched_bwp_config{bwp_id,
                          sched_bwp_dl_config{bwp_dl_cmn,
                                              dl_ptr.get(),
                                              dl_ptr.has_value() and dl_ptr->pdcch_cfg.has_value()
                                                  ? pdcch_pool.ded_cfgs()[0]
                                                  : pdcch_pool.init_cfg(),
                                              get_dl_td_mapper(dl_ptr.get())},
                          sched_bwp_ul_config{bwp_ul_cmn, ul_owned, ue_bwp_cfg.ul, get_ul_td_mapper(ul_ded)}};
}

const dl_time_domain_mapper& bwp_config_pool::get_dl_td_mapper(const bwp_downlink_dedicated* dl_ded)
{
  // Get TDRA dedicated list, resolved to the common list if absent, matching dl_time_domain_mapper's own fallback
  // behavior for DCI format 1_1. May carry repetition rows (TS 38.214, Table 5.1.2.1.1-1).
  span<const pdsch_time_domain_resource_allocation> ded_res = bwp_dl_cmn.pdsch_common.pdsch_td_alloc_list;
  if (dl_ded != nullptr and dl_ded->pdsch_cfg.has_value() and not dl_ded->pdsch_cfg->pdsch_td_alloc_list.empty()) {
    ded_res = dl_ded->pdsch_cfg->pdsch_td_alloc_list;
  }

  // Check if mapper already exists in the pool in an efficient manner, without creating a full dl_time_domain_mapper.
  auto obj = dl_td_mapper_pool.find_if(
      [ded_res](const dl_time_domain_mapper& mapper) { return mapper.dedicated_pdsch_td_resources() == ded_res; });
  if (obj.has_value()) {
    return *obj;
  }

  // Build unique TDRA mapper.
  dl_time_domain_builder_params params;
  params.cp      = bwp_dl_cmn.generic_params.cp;
  params.tdd_cfg = tdd_cfg;
  dl_time_domain_builder_params::explicit_resources explicit_res;
  explicit_res.common_pdsch_td_res_list = bwp_dl_cmn.pdsch_common.pdsch_td_alloc_list;
  if (dl_ded != nullptr and dl_ded->pdsch_cfg.has_value()) {
    // If empty, DCI format 1_1 falls back to the common list.
    explicit_res.dedicated_pdsch_td_res_list = dl_ded->pdsch_cfg->pdsch_td_alloc_list;
  }
  params.params = std::move(explicit_res);

  return *dl_td_mapper_pool.create(dl_time_domain_mapper{params});
}

const ul_time_domain_mapper& bwp_config_pool::get_ul_td_mapper(const bwp_uplink_dedicated* ul_ded)
{
  // Get TDRA dedicated list, resolved to the common list if absent, matching ul_time_domain_mapper's own fallback
  // behavior for DCI format 0_1. May carry repetition rows (TS 38.214, Table 6.1.2.1.1-1).
  span<const pusch_time_domain_resource_allocation> ded_res = bwp_ul_cmn.pusch_cfg_common->pusch_td_alloc_list;
  if (ul_ded != nullptr and ul_ded->pusch_cfg.has_value() and not ul_ded->pusch_cfg->pusch_td_alloc_list.empty()) {
    ded_res = ul_ded->pusch_cfg->pusch_td_alloc_list;
  }

  // Check if mapper already exists in the pool in an efficient manner, without creating a full ul_time_domain_mapper.
  auto obj = ul_td_mapper_pool.find_if(
      [ded_res](const ul_time_domain_mapper& mapper) { return mapper.dedicated_pusch_td_resources() == ded_res; });
  if (obj.has_value()) {
    return *obj;
  }

  // Build unique TDRA mapper.
  ul_time_domain_builder_params params;
  params.scs     = bwp_ul_cmn.generic_params.scs;
  params.cp      = bwp_ul_cmn.generic_params.cp;
  params.tdd_cfg = tdd_cfg;
  ul_time_domain_builder_params::pusch_explicit_resources pusch_explicit_res;
  pusch_explicit_res.common_pusch_td_res_list = bwp_ul_cmn.pusch_cfg_common->pusch_td_alloc_list;
  if (ul_ded != nullptr and ul_ded->pusch_cfg.has_value()) {
    // If empty, DCI format 0_1 falls back to the common list.
    pusch_explicit_res.dedicated_pusch_td_res_list = ul_ded->pusch_cfg->pusch_td_alloc_list;
  }
  params.pusch_params = std::move(pusch_explicit_res);

  ul_time_domain_builder_params::pucch_auto_resources pucch_auto_res;
  pucch_auto_res.min_k1 = pucch_min_k1;
  params.pucch_params   = pucch_auto_res;

  return *ul_td_mapper_pool.create(ul_time_domain_mapper{params});
}

static std::vector<std::unique_ptr<bwp_config_pool>>
make_cell_bwp_pools(const sched_cell_configuration_request_message& req)
{
  std::vector<std::unique_ptr<bwp_config_pool>> bwps;
  // Note: Create a pool for a single BWP for now.
  bwps.push_back(std::make_unique<bwp_config_pool>(req.ran, to_bwp_id(0), make_cell_bwp_res_config(req.ran)));
  return bwps;
}

du_cell_config_pool::du_cell_config_pool(const scheduler_expert_config&                  sched_cfg_,
                                         const sched_cell_configuration_request_message& req) :
  cell_bwps(make_cell_bwp_pools(req)), cell_cfg_inst(sched_cfg_, req, cell_bwps[0]->cell_cfg())
{
}

ue_cell_res_config& du_cell_config_pool::acquire_ue_cell_cfg()
{
  // Reuse a slot whose references have all been dropped (ref_cnt == 0), else grow the pool. Both happen on the
  // (non-RT) config path, so the allocation on growth is fine; the pool plateaus at the peak concurrent UE count.
  for (ue_cell_res_config& pooled_cfg : ue_cell_cfg_pool) {
    if (pooled_cfg.is_unreferenced()) {
      pooled_cfg.clear();
      return pooled_cfg;
    }
  }
  return ue_cell_cfg_pool.emplace_back();
}

ue_cell_config_ptr du_cell_config_pool::update_ue(const sched_ue_cell_config& ue_cell)
{
  ue_cell_res_config& ret = acquire_ue_cell_cfg();
  ret.cell_index          = ue_cell.cell_index;

  // TODO: Support more than one BWP.
  if (not ue_cell.ded.has_value()) {
    // Common config only.
    add_bwp(ret, cell_bwps[0]->cell_cfg());
  } else {
    // Common and dedicated config.
    const serving_cell_config& serv_cell_cfg = ue_cell.ded->serv_cell_cfg;
    if (serv_cell_cfg.ul_config.has_value() and serv_cell_cfg.ul_config.value().pusch_serv_cell_cfg.has_value()) {
      ret.pusch_serv_cell_cfg.emplace(
          pusch_serv_cell_pool.create(serv_cell_cfg.ul_config.value().pusch_serv_cell_cfg.value()));
    }
    if (serv_cell_cfg.pdsch_serv_cell_cfg.has_value()) {
      ret.pdsch_serv_cell_cfg = pdsch_serv_cell_pool.create(serv_cell_cfg.pdsch_serv_cell_cfg.value());
    }
    ret.tag_id = serv_cell_cfg.tag_id;

    add_bwp(
        ret,
        cell_bwps[0]->add_ded_cfg(&serv_cell_cfg.init_dl_bwp,
                                  serv_cell_cfg.ul_config.has_value() ? &serv_cell_cfg.ul_config->init_ul_bwp : nullptr,
                                  ue_cell.ded->bwps[0]));
  }

  // Hand out an intrusive_ptr to the pool-owned slot. Dropping the last reference (possibly on the RT thread) only
  // decrements ref_cnt; the slot is reused on a later acquire, never freed off the config path.
  return ue_cell_config_ptr{&ret};
}

void du_cell_config_pool::add_bwp(ue_cell_res_config& out, sched_bwp_config bwp_cfg)
{
  // Generate cell-wide lookups of CORESETs and SearchSpaces for this UE. These reference the (shared, pooled) PDCCH
  // config, so they stay valid after the BWP config is moved into the owned list below.
  for (const auto& cs : bwp_cfg.dl.pdcch().coresets()) {
    out.coresets.emplace(cs->id(), cs);
  }
  for (const auto& ss : bwp_cfg.dl.pdcch().search_spaces()) {
    out.search_spaces.emplace(ss->id(), &ss->cfg());
  }

  out.bwps.emplace(bwp_cfg.id, std::move(bwp_cfg));
}

// class du_cell_group_config_pool

cell_configuration& du_cell_group_config_pool::add_cell(const scheduler_expert_config&                  expert_cfg,
                                                        const sched_cell_configuration_request_message& cell_cfg_req)
{
  ocudu_assert(not cells.contains(cell_cfg_req.cell_index), "Cell already exists");
  return cells.emplace(cell_cfg_req.cell_index, std::make_unique<du_cell_config_pool>(expert_cfg, cell_cfg_req))
      ->cell_cfg();
}

void du_cell_group_config_pool::rem_cell(du_cell_index_t cell_index)
{
  // Note: This function assumes that all the UEs have been removed from this cell.
  cells.erase(cell_index);
}

ue_creation_params du_cell_group_config_pool::add_ue(const sched_ue_creation_request_message& ue_creation_req)
{
  // Create logical channel config.
  auto lc_ch_list =
      lc_ch_pool.create(ue_creation_req.cfg.lc_config_list.has_value() ? ue_creation_req.cfg.lc_config_list.value()
                                                                       : std::vector<logical_channel_config>{});

  // Create UE dedicated cell configs.
  slotted_id_vector<du_cell_index_t, ue_cell_config_ptr> cell_cfgs;
  if (ue_creation_req.cfg.cells.has_value()) {
    for (const auto& cell : ue_creation_req.cfg.cells.value()) {
      cell_cfgs.emplace(cell.cell_index, cells[cell.cell_index]->update_ue(cell));
    }
  }

  return ue_creation_params{ue_creation_req.cfg, lc_ch_list, cell_cfgs};
}

ue_reconfig_params du_cell_group_config_pool::reconf_ue(const sched_ue_reconfiguration_message& ue_reconf_req)
{
  std::optional<logical_channel_config_list_ptr> lc_ch_list;
  if (ue_reconf_req.cfg.lc_config_list.has_value()) {
    lc_ch_list = lc_ch_pool.create(ue_reconf_req.cfg.lc_config_list.value());
  }

  // Create UE dedicated cell configs.
  slotted_id_vector<du_cell_index_t, ue_cell_config_ptr> cell_cfgs;
  if (ue_reconf_req.cfg.cells.has_value()) {
    for (const auto& cell : ue_reconf_req.cfg.cells.value()) {
      // An RRC Reconfiguration cannot remove the dedicated configuration of a serving cell.
      ocudu_assert(cell.ded.has_value(),
                   "ue={} cell={}: Reconfiguration without dedicated configuration",
                   ue_reconf_req.ue_index,
                   cell.cell_index);
      cell_cfgs.emplace(cell.cell_index, cells[cell.cell_index]->update_ue(cell));
    }
  }

  return ue_reconfig_params{ue_reconf_req.cfg, lc_ch_list, cell_cfgs, ue_reconf_req.cs_rnti};
}
