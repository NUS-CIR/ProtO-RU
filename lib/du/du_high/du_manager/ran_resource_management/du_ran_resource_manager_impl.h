// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "du_bearer_resource_manager.h"
#include "du_drx_resource_manager.h"
#include "du_meas_config_manager.h"
#include "du_pdsch_resource_manager.h"
#include "du_pusch_resource_manager.h"
#include "du_ran_resource_manager.h"
#include "ra_resource_manager.h"
#include "ue_capability_manager.h"
#include "ocudu/ran/qos/five_qi.h"
#include "ocudu/scheduler/rrm/configured_grant_type1_rrm.h"
#include "ocudu/scheduler/rrm/csi_resource_manager.h"
#include "ocudu/scheduler/rrm/pucch_resource_manager.h"
#include "ocudu/scheduler/rrm/srs_resource_manager.h"
#include <vector>

namespace ocudu {
namespace odu {

class du_ran_resource_manager_impl;

class du_ue_ran_resource_updater_impl final : public ue_ran_resource_configurator::resource_updater
{
public:
  du_ue_ran_resource_updater_impl(du_ue_resource_config*                      cell_grp_cfg_,
                                  const std::optional<ue_capability_summary>& ue_caps_,
                                  du_ran_resource_manager_impl&               parent_,
                                  du_ue_index_t                               ue_index_);
  du_ue_ran_resource_updater_impl(const du_ue_ran_resource_updater_impl&)            = delete;
  du_ue_ran_resource_updater_impl(du_ue_ran_resource_updater_impl&&)                 = delete;
  du_ue_ran_resource_updater_impl& operator=(const du_ue_ran_resource_updater_impl&) = delete;
  du_ue_ran_resource_updater_impl& operator=(du_ue_ran_resource_updater_impl&&)      = delete;
  ~du_ue_ran_resource_updater_impl() override;

  du_ue_resource_update_response update(du_cell_index_t                       pcell_index,
                                        const f1ap_ue_context_update_request& upd_req,
                                        const du_ue_resource_config*          reestablished_context,
                                        const ue_capability_summary*          reestablished_ue_caps) override;

  void config_applied() override;

  const du_ue_resource_config& get() override { return *cell_grp; }

  void set_cs_rnti(rnti_t cs_rnti) override;

  void clear_cs_rnti() override;

  const std::optional<ue_capability_summary>& ue_capabilities() const override { return *ue_caps; }

private:
  du_ue_resource_config*                      cell_grp;
  const std::optional<ue_capability_summary>* ue_caps;
  du_ran_resource_manager_impl*               parent;
  du_ue_index_t                               ue_index;
};

class du_ran_resource_manager_impl : public du_ran_resource_manager
{
  /// Maximum number of UE contexts that a single cell can hold.
  static constexpr unsigned max_nof_ue_ctxts_per_cell = MAX_NOF_DU_UES_PER_CELL;

  /// Number of UE contexts per cell reserved for UEs that need to be RRC Rejected.
  static constexpr unsigned max_nof_rejected_ue_ctxts = 64;

public:
  du_ran_resource_manager_impl(span<const du_cell_config>                cell_cfg_list_,
                               const scheduler_expert_config&            scheduler_cfg,
                               const std::map<srb_id_t, du_srb_config>&  srbs,
                               const std::map<five_qi_t, du_qos_config>& qos,
                               const du_test_mode_config&                test_cfg_);
  du_ran_resource_manager_impl(du_ran_resource_manager_impl&&)                 = delete;
  du_ran_resource_manager_impl(const du_ran_resource_manager_impl&)            = delete;
  du_ran_resource_manager_impl& operator=(du_ran_resource_manager_impl&&)      = delete;
  du_ran_resource_manager_impl& operator=(const du_ran_resource_manager_impl&) = delete;

  expected<ue_ran_resource_configurator, std::string>
  create_ue_resource_configurator(du_ue_index_t ue_index, du_cell_index_t pcell_index, bool has_tc_rnti) override;

  unsigned get_max_nof_established_ue_contexts(du_cell_index_t cell_index) const override;

  unsigned get_max_nof_rejected_ue_contexts(du_cell_index_t cell_index) const override;

  /// \brief Updates a UE's cell configuration context based on the F1 UE Context Update request.
  ///
  /// UE Context updates may include the addition of new bearers, reservation of PUCCH Resources for Scheduling
  /// Requests or CSI, etc.
  /// \param ue_index Id of the UE whose context is being updated.
  /// \param pcell_idx DU Cell Id of the UE's PCell.
  /// \param upd_req UE Context Update Request received by the F1AP-DU from the CU.
  /// \param reestablished_context Optional parameter to provide the previous context of the UE, in case of an RRC
  /// Reestablishment.
  /// \param reestablished_ue_caps Optional parameter to provide UE capabilities.
  /// \return Result of the context update.
  du_ue_resource_update_response update_context(du_ue_index_t                         ue_index,
                                                du_cell_index_t                       pcell_idx,
                                                const f1ap_ue_context_update_request& upd_req,
                                                const du_ue_resource_config*          reestablished_context,
                                                const ue_capability_summary*          reestablished_ue_caps);

  /// \brief Deallocates the RAN resources taken by the UE, so that they can be used by future UEs.
  ///
  /// \param ue_index Id of the UE whose context is being deallocated.
  void deallocate_context(du_ue_index_t ue_index);

  /// The UE has confirmed that correct application of the new configuration.
  void ue_config_applied(du_ue_index_t ue_index);

private:
  // Tracks the UE context capacity and usage of a cell.
  struct cell_ue_context_count {
    /// Upper-bound on the number of UEs that the cell can support with dedicated resources.
    unsigned max_nof_established = 0;

    /// Number of UE contexts currently allocated in the cell.
    unsigned nof_allocated = 0;

    unsigned max_nof_ue_ctxts() const { return max_nof_established + max_nof_rejected_ue_ctxts; }

    bool full() const { return nof_allocated >= max_nof_ue_ctxts(); }
  };

  error_type<std::string>
       allocate_cell_resources(du_ue_index_t ue_index, du_cell_index_t cell_index, serv_cell_index_t serv_cell_index);
  void deallocate_cell_resources(du_ue_index_t ue_index, serv_cell_index_t serv_cell_index);

  // Computes the upper-bound on the number of UEs that a cell can support with dedicated resources.
  unsigned compute_max_nof_established_ue_ctxts(du_cell_index_t cell_index) const;

  struct ue_resource_context;

  // Reattributes an allocated UE context from one cell to another, on PCell change.
  void move_ue_ctxt_count(ue_resource_context& u, du_cell_index_t new_pcell_index);

  span<const du_cell_config> cell_cfg_list;
  ocudulog::basic_logger&    logger;
  const du_test_mode_config& test_cfg;

  struct ue_resource_context {
    du_ue_resource_config cg_cfg;

    /// Cell whose UE context count includes this UE.
    du_cell_index_t counted_cell_index = INVALID_DU_CELL_INDEX;

    /// Processor of UE capabilities.
    ue_capability_manager ue_cap_manager;

    ue_resource_context(du_ran_resource_manager_impl& parent_);
  };

  // Current UE Resource Allocations.
  slotted_array<ue_resource_context, MAX_NOF_DU_UES, false> ue_res_pool;

  // Allocator of UE PUCCH resources.
  pucch_resource_manager pucch_res_mng;

  // Manager of UE PDSCH configuration.
  du_pdsch_resource_manager pdsch_res_mng;

  // Manager of UE PUSCH configuration.
  du_pusch_resource_manager pusch_res_mng;

  // Manager of UE CSI configuration.
  csi_resource_manager csi_res_mng;

  // Allocator of UE bearer resources.
  du_bearer_resource_manager bearer_res_mng;

  std::unique_ptr<srs_resource_manager> srs_res_mng;

  // Allocator of Configured Grant resources.
  std::unique_ptr<configured_grant_rrm> cg_res_mng;

  // measConfig resources.
  du_meas_config_manager meas_cfg_mng;

  // Allocator of DRX and measGap resources for the DU.
  du_drx_resource_manager drx_res_mng;

  // Allocator of RA resources.
  ra_resource_manager ra_res_alloc;

  // UE context capacity and usage of each cell of the DU.
  std::vector<cell_ue_context_count> cell_ue_ctxts;
};

} // namespace odu
} // namespace ocudu
