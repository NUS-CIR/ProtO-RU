// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "du_pdsch_resource_manager.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/support/error_handling.h"
#include "ocudu/support/math/math_utils.h"
#include <algorithm>

using namespace ocudu;
using namespace odu;

static void set_pdsch_mcs_table(serving_cell_config& cell_cfg, pdsch_mcs_table mcs_table)
{
  const cqi_table_t cqi_table = get_cqi_table(mcs_table);

  // Set MCS index table for PDSCH. See TS 38.214, Table 5.1.3.1-[1-3].
  if (cell_cfg.init_dl_bwp.pdsch_cfg.has_value()) {
    cell_cfg.init_dl_bwp.pdsch_cfg->mcs_table = mcs_table;
  }

  if (cell_cfg.csi_meas_cfg.has_value()) {
    for (auto& csi_report_cfg : cell_cfg.csi_meas_cfg.value().csi_report_cfg_list) {
      // Set CQI table. See TS 38.214, Table 5.2.2.1-[1-4].
      csi_report_cfg.cqi_table = cqi_table;
    }
  }
}

static void set_pdsch_interleaving(serving_cell_config& cell_cfg, vrb_to_prb::mapping_type interleaving)
{
  // Set \c vrb-ToPRB-Interleaver for PDSCH, as per TS 38.331, \c PDSCH-Config.
  if (cell_cfg.init_dl_bwp.pdsch_cfg.has_value()) {
    cell_cfg.init_dl_bwp.pdsch_cfg->vrb_to_prb_interleaving = interleaving;
  }
}

static void
set_pdsch_res_alloc(serving_cell_config& cell_cfg, pdsch_config::resource_allocation res_alloc, rbg_size rbg_sz)
{
  // Set \c resourceAllocation and \c rbg-Size for PDSCH, as per TS 38.331, \c PDSCH-Config.
  if (cell_cfg.init_dl_bwp.pdsch_cfg.has_value()) {
    cell_cfg.init_dl_bwp.pdsch_cfg->res_alloc = res_alloc;
    cell_cfg.init_dl_bwp.pdsch_cfg->rbg_sz    = rbg_sz;
  }
}

static void set_max_dl_nof_harqs(serving_cell_config& cell_cfg, unsigned nof_harqs)
{
  if (cell_cfg.pdsch_serv_cell_cfg.has_value()) {
    cell_cfg.pdsch_serv_cell_cfg->nof_harq_proc =
        static_cast<pdsch_serving_cell_config::nof_harq_proc_for_pdsch>(nof_harqs);
  }
}

static void set_dl_dci_harq_field_size(serving_cell_config& cell_cfg, unsigned field_size)
{
  if (cell_cfg.init_dl_bwp.pdsch_cfg.has_value()) {
    cell_cfg.init_dl_bwp.pdsch_cfg->harq_process_num_size_dci_1_1 =
        static_cast<pdsch_config::harq_process_num_dci_1_1_size>(field_size);
    cell_cfg.init_dl_bwp.pdsch_cfg->harq_process_num_size_dci_1_2 =
        static_cast<pdsch_config::harq_process_num_dci_1_2_size>(field_size);
  }
}

static void set_dl_harq_feedback_disabled(serving_cell_config& cell_cfg, const harq_dl_feedback_disabled_mask& mask)
{
  if (cell_cfg.pdsch_serv_cell_cfg.has_value()) {
    cell_cfg.pdsch_serv_cell_cfg->dl_harq_feedback_disabled = mask;
  }
}

static void set_pdsch_td_alloc_list(serving_cell_config&                               cell_cfg,
                                    std::vector<pdsch_time_domain_resource_allocation> td_alloc_list)
{
  if (cell_cfg.init_dl_bwp.pdsch_cfg.has_value()) {
    cell_cfg.init_dl_bwp.pdsch_cfg->pdsch_td_alloc_list = std::move(td_alloc_list);
    // The UE applies repetitionNumber-r16 of the Rel-16 TDRA list only when the slot-based repetition scheme is
    // configured.
    cell_cfg.init_dl_bwp.pdsch_cfg->slot_based_repetition_enabled =
        std::any_of(cell_cfg.init_dl_bwp.pdsch_cfg->pdsch_td_alloc_list.begin(),
                    cell_cfg.init_dl_bwp.pdsch_cfg->pdsch_td_alloc_list.end(),
                    [](const auto& alloc) { return alloc.rep_number.has_value(); });
  }
}

du_pdsch_resource_manager::du_pdsch_resource_manager(span<const du_cell_config> cell_cfg_list_,
                                                     const du_test_mode_config& test_cfg_) :
  cell_cfg_list(cell_cfg_list_), test_cfg(test_cfg_)
{
}

void du_pdsch_resource_manager::alloc_resources(cell_group_config& cell_grp_cfg)
{
  apply_config(cell_grp_cfg, std::nullopt);
}

void du_pdsch_resource_manager::update_resources(cell_group_config& cell_grp_cfg, const ue_capability_summary& ue_caps)
{
  apply_config(cell_grp_cfg, ue_caps);
}

void du_pdsch_resource_manager::dealloc_resources(cell_group_config& /*cell_grp_cfg*/)
{
  // No pool resources to release.
}

void du_pdsch_resource_manager::apply_config(cell_group_config&                          cell_grp_cfg,
                                             const std::optional<ue_capability_summary>& ue_caps)
{
  const du_cell_index_t cell_idx  = cell_grp_cfg.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.cell_index;
  serving_cell_config&  serv_cell = cell_grp_cfg.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  set_pdsch_mcs_table(serv_cell, select_mcs_table(cell_idx, ue_caps));
  set_pdsch_interleaving(serv_cell, select_interleaving(cell_idx, ue_caps));
  set_pdsch_res_alloc(
      serv_cell, select_res_alloc(cell_idx, ue_caps), cell_cfg_list[cell_idx].ran.init_bwp.pdsch.rbg_sz);
  set_max_dl_nof_harqs(serv_cell, select_max_nof_harqs(cell_idx, ue_caps));
  set_dl_dci_harq_field_size(serv_cell, select_dci_harq_field_size(cell_idx, ue_caps));
  set_dl_harq_feedback_disabled(serv_cell, select_disabled_harq_feedback(cell_idx, ue_caps));
  set_pdsch_td_alloc_list(serv_cell, select_td_alloc_list(cell_idx, ue_caps));
}

pdsch_mcs_table du_pdsch_resource_manager::select_mcs_table(du_cell_index_t                             cell_idx,
                                                            const std::optional<ue_capability_summary>& ue_caps) const
{
  const pdsch_mcs_table app_mcs_table = cell_cfg_list[cell_idx].ran.init_bwp.pdsch.mcs_table;
  if (ue_caps.has_value()) {
    if (app_mcs_table == pdsch_mcs_table::qam256 and ue_caps->pdsch_qam256_supported) {
      return pdsch_mcs_table::qam256;
    }
    if (app_mcs_table == pdsch_mcs_table::qam64LowSe and ue_caps->pdsch_qam64lowse_supported) {
      return pdsch_mcs_table::qam64LowSe;
    }
  } else if (test_cfg.test_ue.has_value() and test_cfg.test_ue->rnti != rnti_t::INVALID_RNTI) {
    // Has no capabilities but the UE is in test mode.
    return app_mcs_table;
  }

  // Default to QAM64 if the UE capabilities are not available.
  return pdsch_mcs_table::qam64;
}

vrb_to_prb::mapping_type
du_pdsch_resource_manager::select_interleaving(du_cell_index_t                             cell_idx,
                                               const std::optional<ue_capability_summary>& ue_caps) const
{
  const vrb_to_prb::mapping_type app_interleaving = cell_cfg_list[cell_idx].ran.init_bwp.pdsch.interleaving_bundle_size;
  if (ue_caps.has_value()) {
    if (ue_caps->pdsch_interleaving_vrb_to_prb_supported) {
      return app_interleaving;
    }
  } else if (test_cfg.test_ue.has_value() and test_cfg.test_ue->rnti != rnti_t::INVALID_RNTI) {
    // Has no capabilities but the UE is in test mode.
    return app_interleaving;
  }

  // Default to non-interleaved if the UE capabilities are not available.
  return vrb_to_prb::mapping_type::non_interleaved;
}

pdsch_config::resource_allocation
du_pdsch_resource_manager::select_res_alloc(du_cell_index_t                             cell_idx,
                                            const std::optional<ue_capability_summary>& ue_caps) const
{
  const pdsch_config::resource_allocation app_res_alloc = cell_cfg_list[cell_idx].ran.init_bwp.pdsch.res_alloc;
  if (ue_caps.has_value()) {
    // Resource allocation type 0 is mandatory, while the dynamic switch between types 0 and 1 is a UE capability.
    if (app_res_alloc != pdsch_config::resource_allocation::dynamic_switch or
        ue_caps->pdsch_dyn_switch_ra_type0_1_supported) {
      return app_res_alloc;
    }
  } else if (test_cfg.test_ue.has_value() and test_cfg.test_ue->rnti != rnti_t::INVALID_RNTI) {
    // Has no capabilities but the UE is in test mode.
    return app_res_alloc;
  }

  // Default to resource allocation type 1 if the UE capabilities are not available.
  return pdsch_config::resource_allocation::resource_allocation_type_1;
}

unsigned du_pdsch_resource_manager::select_max_nof_harqs(du_cell_index_t                             cell_idx,
                                                         const std::optional<ue_capability_summary>& ue_caps) const
{
  unsigned cell_max = cell_cfg_list[cell_idx].ran.init_bwp.pdsch.max_harq_procs;

  if (test_cfg.test_ue.has_value() and test_cfg.test_ue->rnti != rnti_t::INVALID_RNTI) {
    return cell_max;
  }

  nr_band band = cell_cfg_list[cell_idx].ran.dl_carrier.band;

  if (not ue_caps.has_value()) {
    return std::min(cell_max, ue_capability_summary::default_max_harq_process_num);
  }

  // UE capabilities available but band info not available, or UE does not support NTN, return at most 16.
  if (not band_helper::is_ntn_band(band) || ue_caps->bands.count(band) == 0 || not ue_caps->ntn_supported) {
    return std::min(cell_max, ue_capability_summary::default_max_harq_process_num);
  }

  // UE capabilities available and NTN supported.
  return std::min(cell_max, (unsigned)ue_caps->bands.at(band).max_dl_harq_process_num);
}

unsigned
du_pdsch_resource_manager::select_dci_harq_field_size(du_cell_index_t                             cell_idx,
                                                      const std::optional<ue_capability_summary>& ue_caps) const
{
  auto default_size = log2_ceil(ue_capability_summary::default_max_harq_process_num);

  if (test_cfg.test_ue.has_value() and test_cfg.test_ue->rnti != rnti_t::INVALID_RNTI) {
    return default_size;
  }

  unsigned cell_max  = cell_cfg_list[cell_idx].ran.init_bwp.pdsch.max_harq_procs;
  auto     cell_size = log2_ceil(cell_max);
  nr_band  band      = cell_cfg_list[cell_idx].ran.dl_carrier.band;

  // UE capabilities have not been decoded yet, band info not available, or UE does not support NTN.
  if (not band_helper::is_ntn_band(band) || not ue_caps.has_value() || ue_caps->bands.count(band) == 0 ||
      not ue_caps->ntn_supported) {
    return default_size;
  }

  auto band_size = log2_ceil((unsigned)ue_caps->bands.at(band).max_dl_harq_process_num);
  return std::max(std::min(cell_size, band_size), default_size);
}

harq_dl_feedback_disabled_mask
du_pdsch_resource_manager::select_disabled_harq_feedback(du_cell_index_t                             cell_idx,
                                                         const std::optional<ue_capability_summary>& ue_caps) const
{
  // The bit(s) set to zero identify HARQ processes with enabled DL HARQ feedback.
  harq_dl_feedback_disabled_mask default_mask(MAX_NOF_HARQS);
  default_mask.reset();

  harq_dl_feedback_disabled_mask cell_mask = cell_cfg_list[cell_idx].ran.init_bwp.pdsch.dl_harq_feedback_disabled;

  if (test_cfg.test_ue.has_value() and test_cfg.test_ue->rnti != rnti_t::INVALID_RNTI) {
    return cell_mask;
  }

  const nr_band band = cell_cfg_list[cell_idx].ran.dl_carrier.band;

  // Not NTN band, UE capabilities not decoded yet, band info not available, or UE does not support NTN.
  if (not band_helper::is_ntn_band(band) || not ue_caps.has_value() || ue_caps->bands.count(band) == 0 ||
      not ue_caps->ntn_supported) {
    return default_mask;
  }

  return ue_caps->disabled_dl_harq_feedback_supported ? cell_mask : default_mask;
}

std::vector<pdsch_time_domain_resource_allocation>
du_pdsch_resource_manager::select_td_alloc_list(du_cell_index_t                             cell_idx,
                                                const std::optional<ue_capability_summary>& ue_caps) const
{
  // See TS 38.331, \c maxNrofDL-Allocations.
  static constexpr unsigned max_nof_dl_allocs = 16;

  const du_cell_config& cell_cfg = cell_cfg_list[cell_idx];

  // Verify if dedicated TDRA list with repetiotions (Rel-16) shall be generated.
  // 1. PDSCH repetitions disabled for the cell.
  const unsigned max_rep_cfg = cell_cfg.ran.init_bwp.pdsch.max_nof_repetitions;
  if (max_rep_cfg <= 1) {
    return {};
  }

  // 2 .The UE must support \e supportRepNumPDSCH-TDRA-r16 in the cell DL band.
  const nr_band band = cell_cfg.ran.dl_carrier.band;
  if (not ue_caps.has_value() or ue_caps->bands.count(band) == 0 or
      ue_caps->bands.at(band).max_pdsch_tdra_rep_number <= 1) {
    return {};
  }

  // 3. If Dl repetitons feature is enabled and UE supports repetitions, create Rel-16 TDRA list.
  const unsigned max_rep = std::min(max_rep_cfg, unsigned{ue_caps->bands.at(band).max_pdsch_tdra_rep_number});

  // From the UE perspective the dedicated Rel-16 TDRA list replaces the common list for DCI format 1_1. Mirror the
  // common list entries first and append the repetition entry after them.
  const auto& common_list = cell_cfg.ran.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list;
  if (common_list.empty()) {
    return {};
  }
  std::vector<pdsch_time_domain_resource_allocation> result;
  result.reserve(common_list.size() + 1);
  result.insert(result.end(), common_list.begin(), common_list.end());

  // Repetition entries use the worst-case (shortest) SLIV among the common list entries that span up to the last
  // symbol of the slot, i.e. the entries meant for fully-DL slots. Special slot entries are excluded, as repetitions
  // falling on slots that do not contain the allocated symbols are dropped.
  const unsigned symbols_per_slot = get_nsymb_per_slot(cell_cfg.ran.dl_cfg_common.init_dl_bwp.generic_params.cp);
  const pdsch_time_domain_resource_allocation* base_alloc = nullptr;
  for (const auto& alloc : common_list) {
    if (alloc.symbols.stop() == symbols_per_slot and
        (base_alloc == nullptr or alloc.symbols.length() < base_alloc->symbols.length())) {
      base_alloc = &alloc;
    }
  }
  if (base_alloc == nullptr) {
    return {};
  }

  // Append a single entry with \e repetitionNumber-r16 set to max_rep.
  if (result.size() < max_nof_dl_allocs) {
    pdsch_time_domain_resource_allocation rep_alloc = *base_alloc;
    rep_alloc.rep_number                            = static_cast<uint8_t>(max_rep);
    result.push_back(rep_alloc);
  }

  return result;
}
