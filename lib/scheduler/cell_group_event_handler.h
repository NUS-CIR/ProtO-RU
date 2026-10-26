// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "config/sched_config_manager.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/phy_time_unit.h"
#include "ocudu/ran/slot_point.h"
#include "ocudu/ran/time_alignment_config.h"
#include "ocudu/scheduler/scheduler_dl_buffer_state_indication_handler.h"
#include "ocudu/scheduler/scheduler_feedback_handler.h"
#include "ocudu/scheduler/scheduler_slot_handler.h"

namespace ocudu {

/// \brief Interface used to apply to a cell group the UE configuration requests that a cell dispatched.
///
/// The methods return whether the request was applied, so that the cell that dispatched it can report the failure.
class cell_group_ue_config_handler
{
public:
  virtual ~cell_group_ue_config_handler() = default;

  /// Create a UE in the cell group.
  virtual bool handle_ue_creation(ue_config_update_event ev) = 0;

  /// Reconfigure a UE of the cell group.
  virtual bool handle_ue_reconfiguration(ue_config_update_event ev) = 0;

  /// Delete a UE of the cell group.
  virtual bool handle_ue_deletion(ue_config_delete_event ev) = 0;

  /// Confirm that the UE applied the last configuration sent to it.
  virtual bool handle_ue_config_applied(du_ue_index_t ue_index) = 0;

  /// Deactivate a UE of the cell group.
  virtual bool handle_ue_deactivation_request(du_ue_index_t ue_index) = 0;

  /// Apply a slice reconfiguration to the cell group.
  virtual void handle_slice_reconfiguration(const du_cell_slice_reconfig_request& req) = 0;
};

/// \brief Interface used to apply to a cell group the UE indications that a cell dispatched, and the outcomes that a
/// cell derived for one of its UEs.
///
/// The methods that return a bool report whether the indication was applied, so that the cell that dispatched it can
/// log it and account for it in its metrics.
class cell_group_ue_indication_handler
{
public:
  virtual ~cell_group_ue_indication_handler() = default;

  /// Apply a buffer status report of a UE of the cell group.
  virtual bool handle_ul_bsr_indication(const ul_bsr_indication_message& bsr) = 0;

  /// Apply a power headroom report of a UE of the cell group.
  virtual bool handle_ul_phr_indication(const ul_phr_indication_message& phr) = 0;

  /// Apply a timing advance report of a UE of the cell group.
  virtual bool handle_ul_ta_report_indication(const ul_ta_report_indication_message& ta_report) = 0;

  /// Queue a MAC CE for transmission to a UE of the cell group.
  virtual bool handle_dl_mac_ce_indication(const dl_mac_ce_indication& ce) = 0;

  /// Complete the contention resolution of a UE of the cell group with a received C-RNTI MAC CE.
  virtual bool handle_crnti_ce_received(du_ue_index_t ue_index) = 0;

  /// Apply a downlink buffer occupancy update of a bearer of a UE of the cell group.
  virtual bool handle_dl_buffer_state_indication(const dl_buffer_state_indication_message& dl_bo) = 0;

  /// Complete the contention resolution of a UE of the cell group whose contention-free Msg3 was ACKed.
  virtual void handle_cfra_msg3_acked(du_ue_index_t ue_index) = 0;

  /// Complete the contention resolution of a UE of the cell group whose contention resolution CE was ACKed.
  virtual void handle_conres_ce_acked(du_ue_index_t ue_index) = 0;

  /// Apply a scheduling request of a UE of the cell group, detected in the given UCI slot.
  virtual void handle_sr_detected(du_ue_index_t ue_index, slot_point uci_slot) = 0;

  /// Apply an N_TA update measured for a time alignment group of a UE of the cell group.
  virtual void handle_ul_n_ta_update(du_ue_index_t              ue_index,
                                     time_alignment_group::id_t tag_id,
                                     phy_time_unit              n_ta_diff,
                                     float                      ul_sinr) = 0;

  /// Forwards a cell group's UE's Configured Grant activation complete indication to the scheduler.
  virtual void handle_cg_activation_complete_indication(du_ue_index_t ue_index) = 0;

  /// Forwards an error indication to the cell group for a past slot of the cell.
  virtual void handle_error_indication(slot_point sl_tx, scheduler_slot_handler::error_outcome event) = 0;
};

/// Interface that a cell uses to hand over to its cell group the events that the cell group has to handle.
class cell_group_event_handler : public cell_group_ue_config_handler, public cell_group_ue_indication_handler
{};

} // namespace ocudu
