// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ue_scheduler_impl.h"
#include "../configured_grant/configured_grant_type1_sched_impl.h"
#include "../configured_grant/configured_grant_type2_sched_impl.h"
#include "../logging/cell_metrics_handler.h"

using namespace ocudu;

static std::unique_ptr<configured_grant_scheduler> create_cg_scheduler(const cell_configuration& cell_cfg,
                                                                       uci_allocator&            uci_alloc,
                                                                       ue_repository&            ues,
                                                                       pdcch_resource_allocator& pdcch_alloc)
{
  if (not cell_cfg.params.init_bwp.cg_cfg.has_value()) {
    return nullptr;
  }
  const auto& cell_cg_cfg = *cell_cfg.params.init_bwp.cg_cfg;
  if (cell_cg_cfg.is_type2()) {
    return std::make_unique<configured_grant_type2_sched_impl>(cell_cfg, cell_cg_cfg, uci_alloc, pdcch_alloc, ues);
  }
  return std::make_unique<configured_grant_type1_sched_impl>(cell_cfg, cell_cg_cfg, uci_alloc, ues);
}

ue_scheduler_impl::ue_scheduler_impl(const scheduler_ue_expert_config& expert_cfg_) :
  expert_cfg(expert_cfg_), logger(ocudulog::fetch_basic_logger("SCHED")), ue_db(expert_cfg), event_mng(ue_db)
{
}

ue_cell_scheduler* ue_scheduler_impl::do_add_cell(const ue_cell_scheduler_creation_request& params)
{
  // Register the cell UE repository first, so the cell can accept UEs as soon as it exists.
  ue_db.register_cell(*params.ue_cell_db);

  cells.emplace(params.cell_index, *this, params);
  auto& cell = cells[params.cell_index];

  // Create a cell-specific UE event manager.
  cell.ev_handler = event_mng.add_cell(cell_creation_event{*params.cell_res_alloc,
                                                           cell.ue_cell_db,
                                                           cell.fallback_sched,
                                                           *params.uci_sched,
                                                           cell.slice_sched,
                                                           *params.srs_sched,
                                                           cell.cg_sched.get(),
                                                           *params.ra_ue_repo});

  return &cell;
}

void ue_scheduler_impl::do_start_cell(du_cell_index_t cell_index)
{
  ocudu_assert(cells.contains(cell_index), "Cell reference not found in the scheduler");
}

void ue_scheduler_impl::do_stop_cell(du_cell_index_t cell_index)
{
  ocudu_assert(cells.contains(cell_index), "Cell reference not found in the scheduler");
  auto& c = cells[cell_index];

  // The other cells of the group may be running the UE scheduling, which reads the UEs and slices modified below.
  std::unique_lock<std::mutex> lock(cell_group_mutex, std::defer_lock);
  if (cells.size() > 1) {
    lock.lock();
  }

  // Stop sub-schedulers.
  c.fallback_sched.stop();
  if (c.cg_sched != nullptr) {
    c.cg_sched->stop();
  }

  // The UE repository drops every UE that has this cell, even as an SCell, without going through the UE removal event
  // that detaches it from the slice schedulers. Detach it here from the slices and the pending triggered UL grants of
  // each of its cells, or they would keep referencing a destroyed UE.
  for (const ue& u : ue_db) {
    if (u.find_cell(cell_index) == nullptr) {
      continue;
    }
    for (unsigned i = 0, e = u.nof_cells(); i != e; ++i) {
      const du_cell_index_t ue_cell_idx = u.get_cell(to_serv_cell_index(i)).cell_index;
      if (cells.contains(ue_cell_idx)) {
        cells[ue_cell_idx].slice_sched.rem_ue(u.ue_index);
        cells[ue_cell_idx].trig_ul_sched.rem_ue(u.ue_index);
      }
    }
  }

  // Remove UEs from the UE repository associated with this cell.
  ue_db.handle_cell_deactivation(cell_index);
}

void ue_scheduler_impl::do_rem_cell(du_cell_index_t cell_index)
{
  ocudu_assert(cells.contains(cell_index), "Cell reference not found in the scheduler");

  do_stop_cell(cell_index);

  // Remove cell from UE scheduler.
  cells.erase(cell_index);

  ue_db.deregister_cell(cell_index);
}

void ue_scheduler_impl::run_sched_strategy(du_cell_index_t cell_index)
{
  auto& cell = cells[cell_index];

  // Schedule DL first.
  // Note: DL should be scheduled first so that the right DAI value is picked in DCI format 0_1.
  while (auto dl_slice_candidate = cell.slice_sched.get_next_dl_candidate()) {
    scheduler_policy& policy = cell.slice_sched.get_policy(dl_slice_candidate->id());
    cell.intra_slice_sched.dl_sched(dl_slice_candidate.value(), policy);
  }

  while (auto ul_slice_candidate = cell.slice_sched.get_next_ul_candidate()) {
    scheduler_policy& policy = cell.slice_sched.get_policy(ul_slice_candidate->id());
    cell.intra_slice_sched.ul_sched(ul_slice_candidate.value(), policy);
  }
}

[[maybe_unused]] static bool puxch_grant_sanitizer(cell_resource_allocator& cell_alloc, ocudulog::basic_logger& logger)
{
  const unsigned HARQ_SLOT_DELAY = 0;
  const auto&    slot_alloc      = cell_alloc[HARQ_SLOT_DELAY];

  if (not cell_alloc.cfg.is_ul_enabled(slot_alloc.slot)) {
    return true;
  }

  // Spans through the PUCCH grant list and check if there is any PUCCH grant scheduled for a UE that has a PUSCH.
  auto puschs = slot_alloc.result_reader().ul().puschs();
  for (const auto& pucch : slot_alloc.result_reader().ul().pucchs()) {
    const auto pusch_grant = std::find_if(puschs.begin(), puschs.end(), [&pucch](const ul_sched_info& pusch) {
      return pusch.pusch_cfg.rnti == pucch.crnti;
    });

    if (pusch_grant != puschs.end()) {
      logger.error("rnti={}: has both PUCCH and PUSCH grants scheduled at slot {}, PUCCH  format={} with uci_bits={}",
                   pucch.crnti,
                   slot_alloc.slot,
                   static_cast<unsigned>(pucch.format()),
                   pucch.uci_bits);
      return false;
    }
  }
  return true;
}

void ue_scheduler_impl::post_process_results(du_cell_index_t cell_index, slot_point sl_tx)
{
  auto& cell = cells[cell_index];

  // The post processing is done for DL and UL slots.
  cell.intra_slice_sched.post_process_results();

  // Update the PUCCH power control data.
  const auto& slot_alloc = (*cell.cell_res_alloc)[0];
  for (const auto& pucch : slot_alloc.result_reader().ul().pucchs()) {
    ue_cell* ue_cc = cell.ue_cell_db.find_by_rnti(pucch.crnti);
    // The UE may be removed between the PUCCH allocation and its transmission.
    if (ue_cc == nullptr) {
      logger.warning("rnti={}: No UE found in the cell repository. Skipping PUCCH power control update", pucch.crnti);
      continue;
    }
    ue_cc->get_pucch_power_controller().update_pucch_pw_ctrl_state(slot_alloc.slot, pucch);
  }

  // Record UEs needing triggered UL grants based on the finalized DL grant list.
  cell.trig_ul_sched.process_dl_results(sl_tx, (*cell.cell_res_alloc)[0].result_reader());

  ocudu_sanity_check(puxch_grant_sanitizer(*cell.cell_res_alloc, logger),
                     "PUCCH and PUSCH found for the same UE in the same slot");
}

void ue_scheduler_impl::run_slot_impl(slot_point sl_tx)
{
  std::unique_lock<std::mutex> lock(cell_group_mutex, std::defer_lock);
  if (cells.size() > 1) {
    // Only mutex if the cell group has more than one cell (Carrier Aggregation case).
    lock.lock();
  }
  if (last_sl_ind == sl_tx) {
    // This slot has already been processed by a cell of the same cell group.
    return;
  }
  last_sl_ind = sl_tx;

  // Update the state of every UE of the cell group. It also updates the state of the UEs of each of its cells.
  ue_db.slot_indication(sl_tx);

  for (auto& group_cell : cells) {
    du_cell_index_t cell_index = group_cell.cell_res_alloc->cfg.cell_index;

    // Schedule configured grant PUSCH opportunities.
    if (group_cell.cg_sched != nullptr) {
      group_cell.cg_sched->run_slot(*group_cell.cell_res_alloc);
    }

    // Run cell-specific SRB0 scheduler.
    group_cell.fallback_sched.run_slot(*group_cell.cell_res_alloc);

    // Update slice context and compute slice priorities.
    group_cell.slice_sched.slot_indication(sl_tx, *group_cell.cell_res_alloc);

    // Update intra-slice scheduler context.
    group_cell.intra_slice_sched.slot_indication(sl_tx);

    // Inject synthetic BSR for triggered UL grants due this slot.
    group_cell.trig_ul_sched.run_slot(sl_tx);

    // Run slice scheduler policies.
    run_sched_strategy(cell_index);

    // Post process sched results.
    post_process_results(cell_index, sl_tx);
  }
}

ue_scheduler_impl::cell_context::cell_context(ue_scheduler_impl&                        parent_,
                                              const ue_cell_scheduler_creation_request& params) :
  parent(parent_),
  cell_res_alloc(params.cell_res_alloc),
  ue_cell_db(*params.ue_cell_db),
  fallback_sched(parent.expert_cfg,
                 params.cell_res_alloc->cfg,
                 *params.pdcch_sched,
                 *params.pucch_alloc,
                 *params.uci_alloc,
                 parent.ue_db,
                 *params.ra_ue_repo,
                 *params.cell_metrics),
  slice_sched(params.cell_res_alloc->cfg, parent.ue_db),
  intra_slice_sched(parent.expert_cfg,
                    parent.ue_db,
                    *params.ue_cell_db,
                    *params.pdcch_sched,
                    *params.uci_alloc,
                    *params.srs_alloc,
                    *params.cell_res_alloc,
                    *params.cell_metrics,
                    ocudulog::fetch_basic_logger("SCHED")),
  cg_sched(create_cg_scheduler(params.cell_res_alloc->cfg, *params.uci_alloc, parent.ue_db, *params.pdcch_sched)),
  trig_ul_sched(parent.ue_db, params.cell_res_alloc->cfg.cell_index, params.cell_res_alloc->cfg.scs_common())
{
}

ue_scheduler_impl::cell_context::~cell_context() = default;
