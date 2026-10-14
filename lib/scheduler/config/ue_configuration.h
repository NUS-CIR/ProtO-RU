// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../support/pdcch/search_space_helper.h"
#include "../support/pdsch/pdsch_config_params.h"
#include "cell_configuration.h"
#include "logical_channel_list_config.h"
#include "sched_config_params.h"
#include "ocudu/adt/slotted_array.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/pdcch/pdcch_candidates.h"
#include "ocudu/ran/resource_allocation/vrb_to_prb.h"
#include "ocudu/ran/serv_cell_index.h"
#include "ocudu/scheduler/config/bwp_configuration.h"
#include <optional>

namespace ocudu {

struct ue_creation_params;
struct ue_reconfig_params;
struct ue_cell_res_config;
class ue_cell_configuration;
struct search_space_info;

/// List of CRBs for a given PDCCH candidate.
using crb_index_list = static_vector<uint16_t, pdcch_constants::MAX_NOF_RB_PDCCH>;

/// \brief Grouping of common and UE-dedicated information associated with a given search space.
struct search_space_info {
  const search_space_configuration*              cfg     = nullptr;
  const sched_coreset_config*                    coreset = nullptr;
  const sched_bwp_config*                        bwp     = nullptr;
  crb_interval                                   dl_crb_lims;
  crb_interval                                   ul_crb_lims;
  std::optional<vrb_to_prb::interleaved_mapping> interleaved_mapping;
  dci_size_config                                dci_sz_cfg;
  dci_sizes                                      dci_sz;

  /// \brief Gets DL DCI format type to use based on SearchSpace configuration.
  /// \return DL DCI format.
  dci_dl_format get_dl_dci_format() const { return pdcch_helper::get_dl_dci_format(*cfg); }

  /// \brief Gets UL DCI format type to use based on SearchSpace configuration.
  /// \return UL DCI format.
  dci_ul_format get_ul_dci_format() const { return pdcch_helper::get_ul_dci_format(*cfg); }

  /// \brief Retrieve all the PDCCH candidates for a given aggregation level and slot for this SearchSpace.
  span<const uint8_t> get_pdcch_candidates(aggregation_level aggr_lvl, slot_point pdcch_slot) const
  {
    return ss_pdcch_candidates[pdcch_slot.count() % ss_pdcch_candidates.size()][to_aggregation_level_index(aggr_lvl)];
  }

  /// \brief Retrieve all the CRBs for a given aggregation level and searchSpace candidate.
  span<const crb_index_list_span> get_crb_list_of_pdcch_candidates(aggregation_level aggr_lvl,
                                                                   slot_point        pdcch_slot) const
  {
    return crbs_of_candidates[pdcch_slot.count() % crbs_of_candidates.size()][to_aggregation_level_index(aggr_lvl)];
  }

  /// \brief Returns the maximum number of DL layers for which a PDSCH config exists for the given time-domain resource.
  /// \c pdsch_cfg_list holds one \ref pdsch_config_params per number of layers, so this is also the valid upper bound
  /// for the \c nof_layers argument of \ref get_pdsch_config.
  unsigned get_nof_pdsch_layer_configs(unsigned pdsch_td_res_index) const
  {
    ocudu_assert(pdsch_cfg_list.size() > pdsch_td_res_index, "nof_pdsch_layer_configs: invalid time-domain index");
    return pdsch_cfg_list[pdsch_td_res_index].size();
  }

  const pdsch_config_params& get_pdsch_config(unsigned pdsch_td_res_index, unsigned nof_layers) const
  {
    ocudu_assert(pdsch_cfg_list.size() > pdsch_td_res_index and pdsch_cfg_list[pdsch_td_res_index].size() >= nof_layers,
                 "get_pdsch_config: Invalid params");
    return pdsch_cfg_list[pdsch_td_res_index][nof_layers - 1];
  }

  /// \brief Assigns computed PDCCH candidates to a SearchSpace.
  void update_pdcch_candidates(const std::vector<std::array<pdcch_candidate_list, NOF_AGGREGATION_LEVELS>>& candidates);

  void update_pdsch_time_domain_list(const ue_cell_configuration& ue_cell_cfg);

  void update_pdsch_mappings(vrb_to_prb::mapping_type interleaving_bundle_size);

private:
  // PDCCH candidates of the SearchSpace for different slot offsets and aggregation levels. Indexed by
  // ss_pddch_candidates[slot_offset % ss_pdcch_candidates.size()][aggr_level_index][candidate_index].
  // We need to keep separate lists for different slot offsets because PDCCH candidates change with the slot index,
  // may have a monitoring periodicity above 1 slot, and may be affected by the candidates of other search spaces.
  std::vector<std::array<pdcch_candidate_list, NOF_AGGREGATION_LEVELS>> ss_pdcch_candidates;

  // List of CRBs used by each PDCCH candidate.
  std::vector<std::array<std::vector<crb_index_list_span>, NOF_AGGREGATION_LEVELS>> crbs_of_candidates;

  // Mapping of PDSCH time domain resources to lists of PDSCH Configs. Each index corresponds to a different PDSCH
  // time resource index, which points to a list whose indexes correspond to different numbers of DL layers.
  std::vector<std::vector<pdsch_config_params>> pdsch_cfg_list;
};

/// UE-dedicated configuration for a given cell.
class ue_cell_configuration
{
public:
  ue_cell_configuration(rnti_t                                crnti_,
                        const cell_configuration&             cell_cfg_common_,
                        const ue_cell_config_ptr&             ue_cell_params,
                        const std::optional<meas_gap_config>& meas_gap_cfg_          = std::nullopt,
                        bool                                  multi_cells_configured = false);
  ue_cell_configuration(const ue_cell_configuration& other);
  ue_cell_configuration(ue_cell_configuration&&)                 = delete;
  ue_cell_configuration& operator=(const ue_cell_configuration&) = delete;
  ue_cell_configuration& operator=(ue_cell_configuration&&)      = delete;

  void reconfigure(const ue_cell_config_ptr&             ue_cell_params,
                   const std::optional<meas_gap_config>& meas_gaps = std::nullopt,
                   const std::optional<drx_config>&      drx_cfg_  = std::nullopt);

  void set_rrm_config(const sched_ue_resource_alloc_config& ue_res_alloc_cfg);

  const rnti_t              crnti;
  const cell_configuration& cell_cfg_common;

  /// Retrieve the parameters relative to the RRM of a UE in the scheduler.
  const sched_ue_resource_alloc_config& rrm_cfg() const { return ue_res_alloc_cfg; }

  const std::optional<drx_config>& get_drx_cfg() const { return drx_cfg; }

  /// \brief Returns whether the UE has a dedicated configuration in this cell.
  /// \remark A UE without dedicated configuration uses only the common configuration of the cell, in fallback mode.
  bool has_dedicated_cfg() const;

  /// Get BWP information given a BWP-Id.
  bool                    has_bwp_id(bwp_id_t bwp_id) const { return cell_ded->bwps.contains(bwp_id); }
  const sched_bwp_config* find_bwp(bwp_id_t bwp_id) const { return has_bwp_id(bwp_id) ? &bwp(bwp_id) : nullptr; }
  const sched_bwp_config& bwp(bwp_id_t bwp_id) const { return cell_ded->bwps[bwp_id]; }
  const sched_bwp_config& init_bwp() const { return cell_ded->bwps[to_bwp_id(0)]; }
  const bwp_config_list&  bwps() const { return cell_ded->bwps; }

  /// Fetches CORESET configuration based on Coreset-Id.
  const sched_coreset_config* find_coreset(coreset_id cs_id) const
  {
    return cell_ded->coresets.contains(cs_id) ? cell_ded->coresets[cs_id] : nullptr;
  }
  const sched_coreset_config& coreset(coreset_id cs_id) const { return *cell_ded->coresets[cs_id]; }

  /// Fetches SearchSpace configuration based on SearchSpace-Id.
  /// Note: The ID space of SearchSpaceIds is common across all the BWPs of a Serving Cell.
  const search_space_info* find_search_space(search_space_id ss_id) const
  {
    return search_spaces.contains(ss_id) ? &search_spaces[ss_id] : nullptr;
  }
  const search_space_info& search_space(search_space_id ss_id) const { return search_spaces[ss_id]; }

  /// Get the number of active DL ports for this UE.
  unsigned get_nof_dl_ports() const { return nof_dl_ports; }

  /// Determines whether DL allocations are possible in the provided slot.
  bool is_dl_enabled(slot_point dl_slot) const
  {
    if (not cell_cfg_common.is_dl_enabled(dl_slot)) {
      return false;
    }
    if (meas_gap_cfg.has_value()) {
      if (is_inside_meas_gap(meas_gap_cfg.value(), dl_slot)) {
        return false;
      }
    }
    return true;
  }

  /// \brief Determines whether UL allocations are possible in the provided slot.
  ///
  /// \param ul_slot Slot for which the UE uplink is being tested.
  /// \param reported_ul_ta T_TA last reported by the UE, if any. Tracked outside the configuration, in
  /// \c ue_ta_report_tracker, and used to place the uplink measurement gap window when the cell has no estimate of its
  /// own. Callers holding a \c ue_cell should use \c ue_cell::is_ul_enabled, which supplies it.
  bool is_ul_enabled(slot_point ul_slot, std::optional<std::chrono::microseconds> reported_ul_ta) const;

  /// \brief Returns whether the UE has a CSI configuration in this cell.
  /// \remark A UE with a dedicated configuration has a CSI configuration if the cell configures CSI.
  bool has_csi_cfg() const
  {
    return has_dedicated_cfg() and cell_cfg_common.bwp_res[to_bwp_id(0)].dl().csi.has_value();
  }

  /// \brief Gets the CSI report configuration of the UE.
  /// \remark Call it only for a UE with a CSI configuration.
  const csi_report_configuration& csi_report() const
  {
    return cell_cfg_common.bwp_res[to_bwp_id(0)].dl().csi.value().report(init_bwp().ul.ue_cfg()->csi_codebook);
  }

  /// PDSCH parameters common to all BWPs.
  const pdsch_serving_cell_config* pdsch_serving_cell_cfg() const
  {
    return cell_ded->pdsch_serv_cell_cfg.has_value() ? &*cell_ded->pdsch_serv_cell_cfg.value() : nullptr;
  }

  /// PUSCH parameters common to all BWPs.
  const pusch_serving_cell_config* pusch_serving_cell_cfg() const
  {
    return cell_ded->pusch_serv_cell_cfg.has_value() ? &*cell_ded->pusch_serv_cell_cfg.value() : nullptr;
  }

  time_alignment_group::id_t tag_id() const { return cell_ded->tag_id; }

  /// Determines the use of transform precoding for DCI Format 0_1 for C-RNTI.
  bool use_pusch_transform_precoding_dci_0_1() const
  {
    return use_pusch_transform_precoding(init_bwp().ul.pusch_ded(), cell_cfg_common.use_msg3_transform_precoder());
  }

  /// \brief Gets the PUSCH transmit scheme codebook configuration.
  ///
  /// The codebook subset selection procedure is described in TS 38.214 Section 6.1.1.1.
  const tx_scheme_codebook& get_pusch_codebook_config() const { return init_bwp().ul.ue_cfg()->pusch.tx_cfg; }

  /// \brief Gets the SRS transmit number of ports.
  const auto& get_srs_nof_ports() const { return init_bwp().ul.ue_cfg()->srs.nof_ports; }

private:
  void configure_bwp_cfg(bwp_id_t bwpid, const sched_bwp_config& bwp);

  /// Dedicated cell configuration.
  ue_cell_config_ptr             cell_ded;
  std::optional<meas_gap_config> meas_gap_cfg;
  std::optional<drx_config>      drx_cfg;
  bool                           multi_cells_configured;

  /// This array maps SearchSpace-Ids (the array indexes) to SearchSpace parameters (the array values).
  slotted_array<search_space_info, MAX_NOF_SEARCH_SPACES> search_spaces;

  /// Number of DL ports for this UE.
  unsigned nof_dl_ports = 1;

  // Parameters relative to the RRM of a UE.
  sched_ue_resource_alloc_config ue_res_alloc_cfg;
};

/// Structure that holds all the information related with the configuration of a UE in the scheduler.
///
/// The information held by this structure includes configured cells and logical channels by the upper layers based on
/// the UE RRC configuration, and other RRM-related parameters that can be controlled by the app or via O1/E2.
class ue_configuration
{
public:
  ue_configuration(du_ue_index_t                         ue_index,
                   rnti_t                                crnti_,
                   const cell_common_configuration_list& common_cells,
                   const ue_creation_params&             params);
  ue_configuration(const ue_configuration& other);

  const du_ue_index_t ue_index;
  const rnti_t        crnti;

  /// Retrieve parameters set at the application level for the UEs instantiated in the gNB.
  const scheduler_ue_expert_config& expert_cfg() const { return pcell_common_cfg().expert_cfg.ue; }

  /// Checks whether the cell with the provided index is configured for the UE.
  bool contains(du_cell_index_t cell_index) const { return du_cells.contains(cell_index); }

  /// Number of configured cells in the UE cell group config.
  size_t nof_cells() const { return ue_cell_to_du_cell_index.size(); }

  std::optional<rnti_t> get_cs_rnti() const { return cs_rnti; }

  /// Get the configuration of a cell that is common to all UEs.
  const cell_configuration& common_cell_cfg(du_cell_index_t cell_index) const
  {
    ocudu_assert(du_cells.contains(cell_index), "Invalid cell_index={}", cell_index);
    return du_cells[cell_index]->cell_cfg_common;
  }
  const cell_configuration& pcell_common_cfg() const
  {
    return common_cell_cfg(ue_cell_to_du_cell_index[SERVING_PCELL_IDX]);
  }

  /// Get the configuration of a cell that is dedicated to the UE.
  const ue_cell_configuration& ue_cell_cfg(du_cell_index_t cell_index) const
  {
    ocudu_assert(du_cells.contains(cell_index), "Invalid cell_index={}", cell_index);
    return *du_cells[cell_index];
  }
  const ue_cell_configuration& ue_cell_cfg(serv_cell_index_t serv_cell_index) const
  {
    ocudu_assert(serv_cell_index < ue_cell_to_du_cell_index.size(), "Invalid cell_index={}", serv_cell_index);
    return ue_cell_cfg(ue_cell_to_du_cell_index[serv_cell_index]);
  }
  const ue_cell_configuration& pcell_cfg() const { return ue_cell_cfg(SERVING_PCELL_IDX); }

  /// Get logical channels configured for the UE.
  logical_channel_config_list_ptr logical_channels() const { return lc_list; }

  /// Update the UE dedicated configuration given a configuration request coming from outside the scheduler.
  void update(const cell_common_configuration_list& common_cells, const ue_reconfig_params& reconf_params);

  /// Returns whether UE configuration is considered complete or not for scheduling the UE as a non-fallback UE.
  /// \remark UE can be scheduled in fallback scheduler even if UE does not have a complete configuration.
  bool is_ue_cfg_complete() const;

  /// Get DRX configuration for the UE cell group.
  const std::optional<drx_config>& drx_cfg() const { return ue_drx_cfg; }

  /// Whether the UE accepts a Differential Koffset MAC CE, as per TS 38.321, Section 5.18.24.
  bool supports_ue_specific_koffset() const { return ue_specific_koffset_supported; }

private:
  // List of configured logical channels
  logical_channel_config_list_ptr lc_list;

  // List of cells configured for a UE.
  slotted_id_vector<du_cell_index_t, std::unique_ptr<ue_cell_configuration>> du_cells;

  // Mapping of UE Cell indexes to DU cell indexes.
  std::vector<du_cell_index_t> ue_cell_to_du_cell_index;

  // DRX config for the UE cell group.
  std::optional<drx_config> ue_drx_cfg;

  // Whether the UE accepts a Differential Koffset MAC CE.
  bool ue_specific_koffset_supported = false;

  // CS-RNTI for Configured Grants
  std::optional<rnti_t> cs_rnti;
};

} // namespace ocudu
