// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "uci_scheduler_impl.h"
#include "../cell/resource_grid.h"
#include "../support/sched_result_helpers.h"
#include "uci_allocator.h"
#include "ocudu/ocudulog/ocudulog.h"

using namespace ocudu;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uci_scheduler_impl::uci_scheduler_impl(const cell_configuration& cell_cfg_,
                                       uci_allocator&            uci_alloc_,
                                       ue_cell_repository&       ue_cell_db_) :
  cell_cfg(cell_cfg_), uci_alloc(uci_alloc_), ue_cell_db(ue_cell_db_), logger(ocudulog::fetch_basic_logger("SCHED"))
{
  // Max size of the UCI resource slot wheel, dimensioned based on the UCI periods.
  periodic_uci_slot_wheel.resize(std::max(MAX_SR_PERIOD, MAX_CSI_REPORT_PERIOD));

  // Pre-reserve space for the UEs that will be added.
  updated_ues.reserve(cell_cfg.max_nof_ue_contexts);
}

void uci_scheduler_impl::run_slot(cell_resource_allocator& res_alloc)
{
  // Initial allocation: we allocate opportunities all over the grid.
  schedule_updated_ues_ucis(res_alloc);

  // Only allocate in the farthest slot in the grid, as the previous part of the allocation grid has been completed
  // at the first this function was called.
  schedule_slot_ucis(res_alloc[res_alloc.max_ul_slot_alloc_delay]);
}

void uci_scheduler_impl::stop()
{
  updated_ues.clear();
  for (auto& sl : periodic_uci_slot_wheel) {
    sl.clear();
  }
}

void uci_scheduler_impl::add_resource(rnti_t crnti, unsigned res_offset, unsigned res_period, bool is_sr)
{
  // The wheel is indexed by DL slot, the SR/CSI offsets by UE UL slot.
  const unsigned wheel_start = cell_cfg.ue_ul_to_dl_slot_offset(res_offset, res_period);
  // For each offset in the periodic UCI slot wheel.
  for (unsigned wheel_offset = wheel_start; wheel_offset < periodic_uci_slot_wheel.size(); wheel_offset += res_period) {
    auto& slot_wheel = periodic_uci_slot_wheel[wheel_offset];

    // Check if the UE is already in the slot wheel.
    auto* it = std::find_if(slot_wheel.begin(), slot_wheel.end(), [crnti](const auto& r) { return r.rnti == crnti; });

    if (it == slot_wheel.end()) {
      // New UE. Create a new element in the list with either SR or CSI resource.
      slot_wheel.push_back(periodic_uci_info{crnti, is_sr ? 1U : 0U, is_sr ? 0U : 1U});
    } else {
      // Resource for UE already exists. Increment the resource counter.
      if (is_sr) {
        it->sr_counter++;
      } else {
        it->csi_counter++;
      }
    }
  }
}

void uci_scheduler_impl::rem_resource(rnti_t crnti, unsigned res_offset, unsigned res_period, bool is_sr)
{
  auto log_error = [&]() {
    logger.error("cell={} c-rnti={}: Unable to remove {} PUCCH resource for period={} offset={}",
                 cell_cfg.cell_index,
                 crnti,
                 is_sr ? "SR" : "CSI",
                 res_period,
                 res_offset);
  };

  const unsigned wheel_start = cell_cfg.ue_ul_to_dl_slot_offset(res_offset, res_period);
  for (unsigned wheel_offset = wheel_start; wheel_offset < periodic_uci_slot_wheel.size(); wheel_offset += res_period) {
    auto& slot_wheel = periodic_uci_slot_wheel[wheel_offset];

    auto* it = std::find_if(slot_wheel.begin(), slot_wheel.end(), [crnti](const auto& r) { return r.rnti == crnti; });
    if (it != slot_wheel.end()) {
      if (is_sr) {
        if (it->sr_counter == 0) {
          log_error();
          continue;
        }
        it->sr_counter--;
      } else {
        if (it->csi_counter == 0) {
          log_error();
          continue;
        }
        it->csi_counter--;
      }

      if (it->sr_counter == 0 and it->csi_counter == 0) {
        // Move resource to last position and delete it to avoid O(N) removal.
        if (it != slot_wheel.end() - 1) {
          auto* last_it = slot_wheel.end() - 1;
          std::swap(*it, *last_it);
        }
        slot_wheel.pop_back();
      }
    } else {
      log_error();
    }
  }
}

void uci_scheduler_impl::add_ue(const ue_cell_configuration& ue_cfg)
{
  add_ue_to_grid(ue_cfg, false);
}

void uci_scheduler_impl::add_ue_to_grid(const ue_cell_configuration& ue_cfg, bool is_reconf)
{
  const ue_uplink_bwp_config* ue_ul_cfg = ue_cfg.init_bwp().ul.ue_cfg();
  if (ue_ul_cfg == nullptr) {
    return;
  }

  // Save SR resource in the slot wheel.
  const unsigned sr_period_slots = sr_periodicity_to_slot(cell_cfg.params.init_bwp.pucch.sr_period);
  add_resource(ue_cfg.crnti, ue_ul_cfg->pucch.sr_offset, sr_period_slots, true);

  if (ue_ul_cfg->csi.periodic_report.has_value()) {
    const unsigned csi_period_slots = to_underlying(cell_cfg.params.init_bwp.csi->csi_rs_period);
    add_resource(ue_cfg.crnti, ue_ul_cfg->csi.periodic_report->offset, csi_period_slots, false);
  }

  // Register the UE in the list of recently configured UEs.
  // Note: We skip this step during RRC Reconfiguration because it would involve cancelling already scheduled UCIs
  // in the grid. While we don't fully support this feature, we leave the old SR/CSI UCIs in the grid. The worst that
  // can happen is some missed SRs or CSI in a short period of time.
  if (not is_reconf) {
    updated_ues.push_back(ue_cfg.crnti);
  }
}

void uci_scheduler_impl::reconf_ue(const ue_cell_configuration& new_ue_cfg, const ue_cell_configuration& old_ue_cfg)
{
  // Detect whether there are any differences in the old and new UE cell config that affect periodic UCI scheduling.
  const ue_uplink_bwp_config* new_ul_cfg = new_ue_cfg.init_bwp().ul.ue_cfg();
  const ue_uplink_bwp_config* old_ul_cfg = old_ue_cfg.init_bwp().ul.ue_cfg();
  if (new_ul_cfg != nullptr and old_ul_cfg != nullptr) {
    if (new_ul_cfg->pucch == old_ul_cfg->pucch and new_ul_cfg->csi.periodic_report == old_ul_cfg->csi.periodic_report) {
      // Nothing changed.
      return;
    }
  }

  rem_ue(old_ue_cfg);
  add_ue_to_grid(new_ue_cfg, true);
}

void uci_scheduler_impl::rem_ue(const ue_cell_configuration& ue_cfg)
{
  const ue_uplink_bwp_config* ue_ul_cfg = ue_cfg.init_bwp().ul.ue_cfg();
  if (ue_ul_cfg == nullptr) {
    return;
  }

  const unsigned sr_period_slots = sr_periodicity_to_slot(cell_cfg.params.init_bwp.pucch.sr_period);
  rem_resource(ue_cfg.crnti, ue_ul_cfg->pucch.sr_offset, sr_period_slots, true);

  if (ue_ul_cfg->csi.periodic_report.has_value()) {
    const unsigned csi_period_slots = to_underlying(cell_cfg.params.init_bwp.csi->csi_rs_period);
    rem_resource(ue_cfg.crnti, ue_ul_cfg->csi.periodic_report->offset, csi_period_slots, false);
  }
}

const ue_cell* uci_scheduler_impl::get_ue_cell(rnti_t rnti) const
{
  return ue_cell_db.find_by_rnti(rnti);
}

void uci_scheduler_impl::schedule_slot_ucis(cell_slot_resource_allocator& slot_alloc)
{
  // For the provided slot, check if there are any pending UCI resources to allocate, and allocate them.
  auto& slot_ucis = periodic_uci_slot_wheel[slot_alloc.slot.count() % periodic_uci_slot_wheel.size()];
  for (auto* it = slot_ucis.begin(); it != slot_ucis.end();) {
    const periodic_uci_info& uci_info = *it;
    const ue_cell*           ue_cc    = get_ue_cell(uci_info.rnti);

    if (ue_cc == nullptr) {
      logger.error("cell={} c-rnti={}: UE for which {} is being scheduled was not found (slot={})",
                   cell_cfg.cell_index,
                   uci_info.rnti,
                   it->sr_counter > 0 ? "SR" : (it->csi_counter > 0 ? "CSI" : "invalid UCI"),
                   slot_alloc.slot);
      it = slot_ucis.erase(it);
      continue;
    }

    // The UE transmits no PUCCH in a slot that its own uplink timeline excludes, e.g. one covered by its measurement
    // gap (TS 38.321, Section 5.4.4 for the SR, TS 38.133, Section 9.1.2 for the interruption itself). In an NTN cell
    // the gap sits T_TA away from the downlink timing it is anchored to, so it can span tens of slots.
    if (not ue_cc->is_ul_enabled(slot_alloc.slot)) {
      logger.debug("cell={} c-rnti={}: Skipped UCI PUCCH for slot={}. Cause: slot is not UL enabled for this UE",
                   cell_cfg.cell_index,
                   uci_info.rnti,
                   slot_alloc.slot);
      ++it;
      continue;
    }

    // Schedule SR PUCCH first.
    // NOTE: Allocating the CSI after the SR helps the PUCCH allocation to compute the number of allocated UCI bits and
    // the corresponding number of PRBs for the PUCCH Format 2 over a PUCCH F2 grant is within PUCCH capacity.
    if (uci_info.sr_counter > 0) {
      if (not uci_alloc.alloc_sr_opportunity(slot_alloc, ue_cc->cfg())) {
        logger.warning("cell={} c-rnti={}: Failed to allocate SR PUCCH for slot={}",
                       cell_cfg.cell_index,
                       uci_info.rnti,
                       slot_alloc.slot);
      }
    }

    // Schedule CSI PUCCH.
    if (uci_info.csi_counter > 0) {
      if (not uci_alloc.alloc_csi_opportunity(slot_alloc, ue_cc->cfg())) {
        logger.warning("cell={} c-rnti={}: Failed to allocate CSI PUCCH for slot={}",
                       cell_cfg.cell_index,
                       uci_info.rnti,
                       slot_alloc.slot);
      }
    }

    ++it;
  }
}

void uci_scheduler_impl::schedule_updated_ues_ucis(cell_resource_allocator& res_alloc)
{
  // For all UEs whose config has been recently updated, schedule their UCIs up until one slot before the farthest
  // slot in the resource grid.
  for (rnti_t rnti : updated_ues) {
    const ue_cell* ue_cc = get_ue_cell(rnti);
    if (ue_cc == nullptr) {
      logger.error("cell={} c-rnti={}: UE for which UCI is being scheduled was not found.", cell_cfg.cell_index, rnti);
      continue;
    }

    // Schedule UCI up to the farthest slot.
    for (unsigned n = 0; n != res_alloc.max_ul_slot_alloc_delay; ++n) {
      auto& slot_ucis = periodic_uci_slot_wheel[(res_alloc.slot_tx() + n).count() % periodic_uci_slot_wheel.size()];

      // Skip UCI scheduling for this UE and slot, if they collide with other resources.
      if (not has_space_for_uci_pdu(res_alloc[n].result_reader(), rnti, cell_cfg.expert_cfg.ue)) {
        if (logger.debug.enabled()) {
          // If we want more detailed logs on the skipped allocations.
          for (const periodic_uci_info& uci_info : slot_ucis) {
            if (uci_info.rnti == rnti) {
              logger.debug("cell={} c-rnti={}: Skipped UCI scheduling for slot={}. Cause: Max PUCCHs has been reached",
                           cell_cfg.cell_index,
                           rnti,
                           res_alloc[n].slot);
            }
          }
        }
        continue;
      }

      for (const periodic_uci_info& uci_info : slot_ucis) {
        if (uci_info.rnti == rnti) {
          // Skip the slots that the UE uplink timeline excludes, see schedule_slot_ucis.
          if (not ue_cc->is_ul_enabled(res_alloc[n].slot)) {
            continue;
          }

          // Schedule SR PUCCHs first.
          // NOTE: Allocating the CSI after the SR helps the PUCCH allocation to compute the number of allocated UCI
          // bits and the corresponding number of PRBs for the PUCCH Format 2 over a PUCCH F2 grant is within PUCCH
          // capacity.

          if (uci_info.sr_counter > 0) {
            span<const pucch_info> pucchs          = res_alloc[n].result_reader().ul().pucchs().unsorted();
            bool                   existing_grants = std::any_of(
                pucchs.begin(), pucchs.end(), [rnti](const pucch_info& grant) { return grant.crnti == rnti; });
            if (not existing_grants) {
              // Only allocate SR if there are no existing PUCCH grants for this UE in this slot, as the PUCCH allocator
              // doesn't support multiplexing SR over other UCI.
              uci_alloc.alloc_sr_opportunity(res_alloc[n], ue_cc->cfg());
            }
          }

          // Schedule CSI
          if (uci_info.csi_counter > 0) {
            uci_alloc.alloc_csi_opportunity(res_alloc[n], ue_cc->cfg());
          }
        }
      }
    }
  }

  // Clear the list of updated UEs.
  updated_ues.clear();
}
