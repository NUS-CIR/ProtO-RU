// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../config/sched_config_manager.h"
#include "../configured_grant/configured_grant_scheduler.h"
#include "../slicing/inter_slice_scheduler.h"
#include "ue_fallback_scheduler.h"
#include "ue_scheduler.h"
#include "ocudu/ran/du_types.h"

namespace ocudu {

class uci_scheduler_impl;
class srs_scheduler;
class uci_indication_selector;
class ra_ue_repository;
class ue_cell_repository;

struct cell_creation_event {
  cell_resource_allocator& cell_res_grid;
  ue_cell_repository&      ue_cell_db;
  ue_fallback_scheduler&   fallback_sched;
  uci_scheduler_impl&      uci_sched;
  inter_slice_scheduler&   slice_sched;
  srs_scheduler&           srs_sched;
  /// Configured Grant scheduler. Nullptr if CG is not configured for the cell.
  configured_grant_scheduler* cg_sched;
  ra_ue_repository&           ra_ue_repo;
};

class cell_group_event_manager;

/// Applies to the cell group events that one of its cells dispatched.
class cell_group_event_handler_impl final : public cell_group_event_handler
{
public:
  cell_group_event_handler_impl(cell_group_event_manager&  parent_,
                                const cell_creation_event& cell_ev,
                                ue_repository&             ue_db,
                                ocudulog::basic_logger&    logger);
  ~cell_group_event_handler_impl() override;

  // cell_group_ue_config_handler methods. Handled synchronously, as the cell already deferred them.
  bool handle_ue_creation(ue_config_update_event ev) override;
  bool handle_ue_reconfiguration(ue_config_update_event ev) override;
  bool handle_ue_deletion(ue_config_delete_event ev) override;
  bool handle_ue_config_applied(du_ue_index_t ue_idx) override;
  bool handle_ue_deactivation_request(du_ue_index_t ue_idx) override;
  void handle_slice_reconfiguration(const du_cell_slice_reconfig_request& req) override;

  // cell_group_ue_indication_handler methods.
  bool handle_ul_bsr_indication(const ul_bsr_indication_message& bsr) override;
  bool handle_ul_phr_indication(const ul_phr_indication_message& phr_ind) override;
  bool handle_ul_ta_report_indication(const ul_ta_report_indication_message& ta_report_ind) override;
  bool handle_dl_mac_ce_indication(const dl_mac_ce_indication& mac_ce) override;
  bool handle_crnti_ce_received(du_ue_index_t ue_index) override;
  bool handle_dl_buffer_state_indication(const dl_buffer_state_indication_message& dl_bo) override;

  void handle_cfra_msg3_acked(du_ue_index_t ue_index) override;
  void handle_conres_ce_acked(du_ue_index_t ue_index) override;
  void handle_sr_detected(du_ue_index_t ue_index, slot_point uci_slot) override;
  void handle_ul_n_ta_update(du_ue_index_t              ue_index,
                             time_alignment_group::id_t tag_id,
                             phy_time_unit              n_ta_diff,
                             float                      ul_sinr) override;
  void handle_cg_activation_complete_indication(du_ue_index_t ue_index) override;
  void handle_error_indication(slot_point sl_tx, scheduler_slot_handler::error_outcome event) override;

private:
  /// Log event with invalid UE index.
  void log_invalid_ue_index(du_ue_index_t ue_index, const char* event_name, bool warn_if_ignored = true) const;

  /// Log event when UE does not have a carrier for this cell.
  void log_invalid_cc(du_ue_index_t ue_idx, const char* event_name, bool warn_if_ignored = true) const;

  // Shared parameters.
  cell_group_event_manager& parent;
  ue_repository&            ue_db;
  ocudulog::basic_logger&   logger;
  // Cell parameters.
  const cell_configuration& cfg;
  ue_fallback_scheduler&    fallback_sched;
  uci_scheduler_impl&       uci_sched;
  inter_slice_scheduler&    slice_sched;
  srs_scheduler&            srs_sched;
  /// Configured Grant scheduler. Nullptr if CG is not configured for the cell.
  configured_grant_scheduler* cg_sched;
  ra_ue_repository&           ra_ue_repo;
};

/// \brief Class used to manage events that arrive to the scheduler and concern a cell group state rather than just
/// an individual cell.
class cell_group_event_manager
{
public:
  cell_group_event_manager(ue_repository& ue_db);

  std::unique_ptr<cell_group_event_handler_impl> add_cell(const cell_creation_event& cell_ev);

private:
  friend class cell_group_event_handler_impl;

  bool cell_exists(du_cell_index_t cell_index) const;

  ue_repository&          ue_db;
  ocudulog::basic_logger& logger;

  std::array<cell_group_event_handler_impl*, MAX_NOF_DU_CELLS> cells;
};

} // namespace ocudu
