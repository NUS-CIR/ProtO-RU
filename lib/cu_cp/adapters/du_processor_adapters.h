// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../cu_cp_controller/cu_cp_controller.h"
#include "../cu_cp_controller/node_connection_notifier.h"
#include "../cu_cp_impl_interface.h"
#include "../du_processor/du_processor.h"
#include "ocudu/support/ocudu_assert.h"
#include <set>

namespace ocudu::ocucp {

/// Adapter between DU processor and CU-CP.
class du_processor_cu_cp_adapter : public du_processor_cu_cp_notifier
{
public:
  void connect_cu_cp(cu_cp_du_event_handler&                cu_cp_mng_,
                     cu_cp_measurement_config_handler&      meas_config_handler_,
                     cu_cp_ue_removal_handler&              ue_removal_handler_,
                     cu_cp_ue_context_manipulation_handler& ue_context_handler_)
  {
    cu_cp_handler       = &cu_cp_mng_;
    meas_config_handler = &meas_config_handler_;
    ue_removal_handler  = &ue_removal_handler_;
    ue_context_handler  = &ue_context_handler_;
  }

  bool on_cell_config_update_request(nr_cell_identity nci, const serving_cell_meas_config& serv_cell_cfg) override
  {
    ocudu_assert(meas_config_handler != nullptr, "Measurement config handler must not be nullptr");
    return meas_config_handler->handle_cell_config_update_request(nci, serv_cell_cfg);
  }

  std::vector<nr_cell_identity> on_du_cells_reported(cu_cp_du_index_t             du_index,
                                                     span<const du_reported_cell> cells) override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    return cu_cp_handler->handle_du_cells_reported(du_index, cells);
  }

  void on_du_cells_removed(span<const nr_cell_identity> cells) override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    cu_cp_handler->handle_du_cells_removed(cells);
  }

  bool on_ue_resume_admission_request(cu_cp_ue_index_t ue_index, resume_cause_t resume_cause) override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    return cu_cp_handler->handle_ue_resume_admission_request(ue_index, resume_cause);
  }

  void on_rrc_ue_created(cu_cp_ue_index_t ue_index, rrc_ue_interface& rrc_ue) override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    cu_cp_handler->handle_rrc_ue_creation(ue_index, rrc_ue);
  }

  byte_buffer on_target_cell_sib1_required(cu_cp_du_index_t du_index, nr_cell_global_id_t cgi) override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    return cu_cp_handler->handle_target_cell_sib1_required(du_index, cgi);
  }

  async_task<void> on_ue_removal_required(cu_cp_ue_index_t ue_index) override
  {
    ocudu_assert(ue_removal_handler != nullptr, "CU-CP UE Removal handler must not be nullptr");
    return ue_removal_handler->handle_ue_removal_request(ue_index);
  }

  void on_e_cid_measurement_report(const e_cid_measurement_report_t& report) override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    cu_cp_handler->handle_e_cid_measurement_report(report);
  }

  void on_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind) override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    cu_cp_handler->handle_e_cid_measurement_failure_indication(ind);
  }

  async_task<void> on_ue_release_required(const cu_cp_ue_context_release_request& request) override
  {
    ocudu_assert(ue_context_handler != nullptr, "UE context handler must not be nullptr");
    return ue_context_handler->handle_ue_context_release(request);
  }

  async_task<void> on_access_success(const cu_cp_access_success_indication& msg) override
  {
    ocudu_assert(ue_context_handler != nullptr, "UE context handler must not be nullptr");
    return ue_context_handler->handle_access_success(msg);
  }

  async_task<void> on_transaction_info_loss(const ue_transaction_info_loss_event& ev) override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    return cu_cp_handler->handle_transaction_info_loss(ev);
  }

  void on_served_cells_updated() override
  {
    ocudu_assert(cu_cp_handler != nullptr, "CU-CP handler must not be nullptr");
    cu_cp_handler->handle_served_cells_updated();
  }

private:
  cu_cp_du_event_handler*                cu_cp_handler       = nullptr;
  cu_cp_measurement_config_handler*      meas_config_handler = nullptr;
  cu_cp_ue_removal_handler*              ue_removal_handler  = nullptr;
  cu_cp_ue_context_manipulation_handler* ue_context_handler  = nullptr;
};

class du_processor_cu_cp_connection_adapter final : public du_connection_notifier
{
public:
  void connect_node_connection_handler(cu_cp_controller& cu_ctrl_) { cu_ctrl = &cu_ctrl_; }

  std::set<plmn_identity> on_connected_plmns_required(const std::set<plmn_identity>& plmn_ids) override
  {
    ocudu_assert(cu_ctrl != nullptr, "CU-CP controller must not be nullptr");
    return cu_ctrl->get_connected_plmns(plmn_ids);
  }

private:
  cu_cp_controller* cu_ctrl = nullptr;
};

} // namespace ocudu::ocucp
