// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "lib/cu_cp/cu_cp_controller/node_connection_notifier.h"
#include "lib/cu_cp/cu_cp_impl_interface.h"
#include "lib/cu_cp/cu_up_processor/cu_up_processor.h"
#include "lib/cu_cp/du_processor/du_processor.h"
#include "lib/cu_cp/ue_manager/ue_manager_impl.h"
#include "ocudu/asn1/f1ap/f1ap.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg_ies.h"
#include "ocudu/cu_cp/cu_cp_ref_time_report_notifier.h"
#include "ocudu/ran/cause/f1ap_cause.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/support/async/async_no_op_task.h"
#include "ocudu/support/async/async_task.h"
#include "ocudu/support/async/fifo_async_task_scheduler.h"
#include <list>
#include <set>
#include <variant>

namespace ocudu::ocucp {

/// \brief Generate DU-to-CU RRC Container with CellGroupConfig.
byte_buffer generate_container_with_cell_group_config();

/// \brief Generate RRC Container with RRC Setup Complete message.
byte_buffer generate_rrc_setup_complete();

// Generate an UL-DCCH PDU on SRB1, with the PDCP header and the MAC-I of the attached test UE.
byte_buffer generate_protected_ul_dcch_pdu(const asn1::rrc_nr::ul_dcch_msg_s& msg, uint8_t count);

// Generate RRC Reconfiguration Complete PDU.
byte_buffer generate_rrc_reconfiguration_complete_pdu(unsigned transaction_id, uint8_t count);

// Extract RRC timers from F1 Setup Request.
rrc_timers_t get_timers(const asn1::f1ap::f1_setup_request_s& f1_setup_req);

struct dummy_du_processor_cu_cp_notifier : public du_processor_cu_cp_notifier {
public:
  explicit dummy_du_processor_cu_cp_notifier(ue_manager* ue_mng_ = nullptr) : ue_mng(ue_mng_) {}

  void attach_handler(cu_cp_du_event_handler*   cu_cp_handler_,
                      cu_cp_ue_removal_handler* ue_removal_handler_,
                      du_processor*             du_proc_)
  {
    cu_cp_handler      = cu_cp_handler_;
    ue_removal_handler = ue_removal_handler_;
    du_proc            = du_proc_;
  }

  bool on_cell_config_update_request(nr_cell_identity nci, const serving_cell_meas_config& serv_cell_cfg) override
  {
    logger.info("Received a cell config update request for nci={}", nci);

    return true;
  }

  std::vector<nr_cell_identity> on_du_cells_reported(cu_cp_du_index_t             du_index,
                                                     span<const du_reported_cell> cells) override
  {
    // Accept and activate every reported cell, preserving the pre-logical-cell behaviour of these tests.
    std::vector<nr_cell_identity> activate;
    activate.reserve(cells.size());
    for (const du_reported_cell& cell : cells) {
      activate.push_back(cell.cgi.nci);
    }
    return activate;
  }

  void on_du_cells_removed(span<const nr_cell_identity> cells) override {}

  bool on_ue_resume_admission_request(cu_cp_ue_index_t ue_index, resume_cause_t resume_cause) override { return true; }

  void on_rrc_ue_created(cu_cp_ue_index_t ue_index, rrc_ue_interface& rrc_ue) override
  {
    logger.info("ue={}: Received a RRC UE creation notification", ue_index);

    if (cu_cp_handler != nullptr) {
      cu_cp_handler->handle_rrc_ue_creation(ue_index, rrc_ue);
    }
  }

  byte_buffer on_target_cell_sib1_required(cu_cp_du_index_t du_index, nr_cell_global_id_t cgi) override
  {
    return make_byte_buffer("deadbeef").value();
  }

  void on_e_cid_measurement_report(const e_cid_measurement_report_t& report) override
  {
    last_e_cid_measurement_report = report;
  }

  void on_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind) override
  {
    last_e_cid_measurement_failure_indication = ind;
  }

  std::optional<e_cid_measurement_report_t>             last_e_cid_measurement_report;
  std::optional<e_cid_measurement_failure_indication_t> last_e_cid_measurement_failure_indication;

  async_task<void> on_ue_removal_required(cu_cp_ue_index_t ue_index) override
  {
    logger.info("ue={}: Received a UE removal request", ue_index);

    return launch_async([this, ue_index](coro_context<async_task<void>>& ctx) mutable {
      CORO_BEGIN(ctx);
      if (ue_removal_handler != nullptr) {
        ue_removal_handler->handle_ue_removal_request(ue_index);
      } else {
        if (ue_mng != nullptr) {
          ue_mng->remove_ue(ue_index);
        }

        if (rrc_removal_handler != nullptr) {
          rrc_removal_handler->remove_ue(ue_index);
        }
      }
      CORO_RETURN();
    });
  }

  async_task<void> on_ue_release_required(const cu_cp_ue_context_release_request& request) override
  {
    logger.info("ue={}: Received UE release required", request.ue_index);

    if (du_proc != nullptr) {
      return launch_async([this, request](coro_context<async_task<void>>& ctx) mutable {
        CORO_BEGIN(ctx);
        CORO_AWAIT(du_proc->get_f1ap_handler().handle_ue_context_release_command(
            {.ue_index = request.ue_index,
             .cause    = f1ap_cause_radio_network_t::no_radio_res_available,
             .rrc_pdu  = byte_buffer::create({0x0, 0x0}).value(),
             .srb_id   = srb_id_t::srb0}));

        if (ue_mng != nullptr) {
          ue_mng->remove_ue(request.ue_index);
        }
        du_proc->get_f1ap_handler().get_f1ap_ue_context_removal_handler().remove_ue_context(request.ue_index);
        CORO_RETURN();
      });
    }

    return launch_no_op_task();
  }

  async_task<void> on_transaction_info_loss(const ue_transaction_info_loss_event& ev) override
  {
    return launch_no_op_task();
  }

  void on_served_cells_updated() override { served_cells_updated = true; }

  async_task<void> on_access_success(const cu_cp_access_success_indication& msg) override
  {
    return launch_no_op_task();
  }

  bool served_cells_updated = false;

private:
  ocudulog::basic_logger&   logger              = ocudulog::fetch_basic_logger("TEST");
  ue_manager*               ue_mng              = nullptr;
  cu_cp_du_event_handler*   cu_cp_handler       = nullptr;
  cu_cp_ue_removal_handler* ue_removal_handler  = nullptr;
  rrc_ue_handler*           rrc_removal_handler = nullptr;
  du_processor*             du_proc             = nullptr;
};

struct dummy_cu_cp_ue_context_manipulation_handler : public cu_cp_ue_context_manipulation_handler {
public:
  async_task<void> handle_ue_context_release(const cu_cp_ue_context_release_request& request) override
  {
    logger.info("ue={}: Received UE release request", request.ue_index);
    return launch_no_op_task();
  }

  void handle_handover_reconfiguration_sent(const cu_cp_intra_cu_handover_target_request& request) override
  {
    logger.info("ue={}: Awaiting a RRC Reconfiguration Complete (transaction_id={})",
                request.target_ue_index,
                request.transaction_id);
    last_transaction_id = request.transaction_id;
  }

  void handle_cho_reconfiguration_sent(const cu_cp_cho_target_request& request) override
  {
    logger.info("target_ue={} source_ue={}: CHO reconfiguration sent, awaiting completion",
                request.target_ue_index,
                request.source_ue_index);
  }

  async_task<bool> handle_handover_ue_context_push(cu_cp_ue_index_t source_ue_index,
                                                   cu_cp_ue_index_t target_ue_index) override
  {
    logger.info("source_ue={} target_ue={}: Received handover ue context push", source_ue_index, target_ue_index);
    return launch_no_op_task(true);
  }

  bool handle_handover_target_ue_amf_binding(cu_cp_ue_index_t source_ue_index,
                                             cu_cp_ue_index_t target_ue_index) override
  {
    return true;
  }

  void initialize_handover_ue_release_timer(cu_cp_ue_index_t                        ue_index,
                                            std::chrono::milliseconds               handover_ue_release_timeout,
                                            const cu_cp_ue_context_release_request& ue_context_release_request) override
  {
    logger.info("ue={}: Initializing UE release timer", ue_index);
    last_handover_ue_release_timer_ue_index = ue_index;
  }

  void initialize_rna_update_timer(cu_cp_ue_index_t ue_index) override
  {
    logger.info("ue={}: Initializing RNA update timer", ue_index);
  }

  void initialize_cho_execution_timer(cu_cp_ue_index_t source_ue_index, std::chrono::milliseconds timeout) override
  {
    logger.info("ue={}: Initializing CHO execution timer (timeout={}ms)", source_ue_index, timeout.count());
  }

  async_task<void> handle_access_success(const cu_cp_access_success_indication& msg) override
  {
    return launch_no_op_task();
  }

  unsigned                        last_transaction_id = 99999;
  std::optional<cu_cp_ue_index_t> last_handover_ue_release_timer_ue_index;

private:
  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("TEST");
};

class dummy_cu_cp_ue_removal_handler : public cu_cp_ue_removal_handler
{
public:
  explicit dummy_cu_cp_ue_removal_handler(ue_manager* ue_mng_ = nullptr) : ue_mng(ue_mng_) {}

  async_task<void> handle_ue_removal_request(cu_cp_ue_index_t ue_index) override
  {
    if (ue_mng != nullptr) {
      ue_mng->remove_ue(ue_index);
    }
    return launch_no_op_task();
  }

  void handle_pending_ue_task_cancellation(cu_cp_ue_index_t ue_index) override {}

private:
  ue_manager* ue_mng = nullptr;
};

class dummy_du_connection_notifier : public du_connection_notifier
{
public:
  std::set<plmn_identity> on_connected_plmns_required(const std::set<plmn_identity>& plmn_ids) override
  {
    return plmn_ids;
  }
};

class dummy_cu_cp_ref_time_report_notifier : public cu_cp_ref_time_report_notifier
{
public:
  void on_ref_time_info_report(span<const nr_cell_global_id_t> served_cells,
                               const cu_cp_ref_time_report&    report) override
  {
  }
};

struct dummy_ngap_ue_context_removal_handler : public ngap_ue_context_removal_handler {
public:
  void remove_ue_context(cu_cp_ue_index_t ue_index) override { logger.info("ue={}: Removing UE", ue_index); }

private:
  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("TEST");
};

// Configuration struct to parameterize the modification outcome.
struct pdu_session_modified_outcome_t {
  pdu_session_id_t      psi;
  std::vector<drb_id_t> drb_added;
  std::vector<drb_id_t> drb_removed;
};

// Stuct to configure Bearer Context Setup/Modification result content.
struct bearer_context_outcome_t {
  bool                outcome = false;
  std::list<unsigned> pdu_sessions_setup_list;  // List of PDU session IDs that were successful to set up.
  std::list<unsigned> pdu_sessions_failed_list; // List of PDU sessions IDs that failed to be setup.
  std::list<pdu_session_modified_outcome_t>
      pdu_sessions_modified_list; // List of PDU session IDs that were successfully modified.
};

struct dummy_e1ap_bearer_context_manager : public e1ap_bearer_context_manager {
public:
  dummy_e1ap_bearer_context_manager() = default;

  void set_first_message_outcome(const bearer_context_outcome_t& outcome) { first_e1ap_response = outcome; }

  void set_second_message_outcome(const bearer_context_outcome_t& outcome) { second_e1ap_response = outcome; }

  static void fill_pdu_session_setup_list(
      slotted_id_vector<pdu_session_id_t, e1ap_pdu_session_resource_setup_modification_item>& e1ap_setup_list,
      const std::list<unsigned>&                                                              outcome_setup_list)
  {
    for (const auto& psi : outcome_setup_list) {
      // Add only the most relevant items.
      e1ap_pdu_session_resource_setup_modification_item res_setup_item;
      res_setup_item.pdu_session_id = uint_to_pdu_session_id(psi);

      // Add a single DRB with the same ID as the PDU session it belongs to.
      drb_id_t                   drb_id = uint_to_drb_id(psi); // Note: we use the PDU session ID here also as DRB ID
      e1ap_drb_setup_item_ng_ran drb_item;
      drb_item.drb_id = drb_id;

      // Add a QoS flow.
      e1ap_qos_flow_item qos_item;
      qos_item.qos_flow_id = uint_to_qos_flow_id(psi); // Note: use the PSI again as QoS flow ID
      drb_item.flow_setup_list.emplace(qos_item.qos_flow_id, qos_item);

      // Add one UP transport item.
      e1ap_up_params_item up_item;
      up_item.cell_group_id = 0;
      up_item.up_tnl_info   = {transport_layer_address::create_from_string("127.0.0.1"), int_to_gtpu_teid(0x1)};
      drb_item.ul_up_transport_params.push_back(up_item);
      res_setup_item.drb_setup_list_ng_ran.emplace(drb_id, drb_item);

      e1ap_setup_list.emplace(res_setup_item.pdu_session_id, res_setup_item);
    }
  }

  static void fill_pdu_session_failed_list(
      slotted_id_vector<pdu_session_id_t, e1ap_pdu_session_resource_failed_item>& e1ap_failed_list,
      const std::list<unsigned>&                                                  outcome_failed_list)
  {
    for (const auto& id : outcome_failed_list) {
      e1ap_pdu_session_resource_failed_item res_failed_item;
      res_failed_item.pdu_session_id = uint_to_pdu_session_id(id);
      e1ap_failed_list.emplace(res_failed_item.pdu_session_id, res_failed_item);
    }
  }

  static void fill_bearer_context_response(e1ap_bearer_context_setup_response& result,
                                           const bearer_context_outcome_t&     outcome)
  {
    result.success = outcome.outcome;
    fill_pdu_session_setup_list(result.pdu_session_resource_setup_list, outcome.pdu_sessions_setup_list);
    fill_pdu_session_failed_list(result.pdu_session_resource_failed_list, outcome.pdu_sessions_failed_list);
  }

  static void fill_pdu_session_modified_list(
      slotted_id_vector<pdu_session_id_t, e1ap_pdu_session_resource_modified_item>& e1ap_modified_list,
      const std::list<pdu_session_modified_outcome_t>&                              outcome_modified_list)
  {
    for (const auto& modified_item : outcome_modified_list) {
      // Add only the most relevant items.
      e1ap_pdu_session_resource_modified_item res_mod_item;
      res_mod_item.pdu_session_id = modified_item.psi;

      for (const auto& drb_id : modified_item.drb_added) {
        // Add a single DRB with the same ID as the PDU session it belongs to.
        e1ap_drb_setup_item_ng_ran drb_item;
        drb_item.drb_id = drb_id;

        // Add a QoS flow.
        e1ap_qos_flow_item qos_item;
        qos_item.qos_flow_id = uint_to_qos_flow_id(to_underlying(drb_id)); // QoS flow has same ID like DRB
        drb_item.flow_setup_list.emplace(qos_item.qos_flow_id, qos_item);

        // Add one UP transport item.
        e1ap_up_params_item up_item;
        drb_item.ul_up_transport_params.push_back(up_item);
        res_mod_item.drb_setup_list_ng_ran.emplace(drb_id, drb_item);
      }

      e1ap_modified_list.emplace(res_mod_item.pdu_session_id, res_mod_item);
    }
  }

  static void fill_bearer_context_response(e1ap_bearer_context_modification_response& result,
                                           const bearer_context_outcome_t&            outcome)
  {
    result.success = outcome.outcome;
    fill_pdu_session_setup_list(result.pdu_session_resource_setup_list, outcome.pdu_sessions_setup_list);
    fill_pdu_session_failed_list(result.pdu_session_resource_failed_list, outcome.pdu_sessions_failed_list);
    fill_pdu_session_modified_list(result.pdu_session_resource_modified_list, outcome.pdu_sessions_modified_list);
  }

  async_task<e1ap_bearer_context_setup_response>
  handle_bearer_context_setup_request(const e1ap_bearer_context_setup_request& msg) override
  {
    logger.info("Received a new bearer context setup request");

    // Store msg to verify content in TC.
    first_e1ap_request = msg;

    return launch_async([res = e1ap_bearer_context_setup_response{},
                         this](coro_context<async_task<e1ap_bearer_context_setup_response>>& ctx) mutable {
      CORO_BEGIN(ctx);

      if (first_e1ap_response.has_value()) {
        const auto& response = first_e1ap_response.value();
        fill_bearer_context_response(res, response);

        // Invalidate E1 response so it's not consumed again.
        first_e1ap_response.reset();
      }

      CORO_RETURN(res);
    });
  }

  async_task<e1ap_bearer_context_modification_response>
  handle_bearer_context_modification_request(const e1ap_bearer_context_modification_request& msg) override
  {
    logger.info("Received a new bearer context modification request");

    // Store msg to verify content in TC.
    if (first_e1ap_request.has_value()) {
      second_e1ap_request = msg;
    } else {
      first_e1ap_request = msg;
    }

    return launch_async([res = e1ap_bearer_context_modification_response{},
                         this](coro_context<async_task<e1ap_bearer_context_modification_response>>& ctx) mutable {
      CORO_BEGIN(ctx);

      if (first_e1ap_response.has_value()) {
        // First E1AP message is already a bearer modification.
        const auto& response = first_e1ap_response.value();
        fill_bearer_context_response(res, response);
        first_e1ap_response.reset(); // Make sure it's not consumed again.
      } else if (second_e1ap_response.has_value()) {
        // Second E1AP message is a bearer modification.
        const auto& response = second_e1ap_response.value();
        fill_bearer_context_response(res, response);
        second_e1ap_response.reset(); // Make sure it's not consumed again.
      }
      CORO_RETURN(res);
    });
  }

  async_task<void> handle_bearer_context_release_command(const e1ap_bearer_context_release_command& cmd) override
  {
    logger.info("Received a new bearer context release command");
    last_release_command = cmd;
    return launch_no_op_task();
  }

  void reset()
  {
    first_e1ap_request.reset();
    second_e1ap_request.reset();
  }

  e1ap_bearer_context_release_command last_release_command;

  std::optional<std::variant<e1ap_bearer_context_setup_request, e1ap_bearer_context_modification_request>>
                                                          first_e1ap_request;
  std::optional<e1ap_bearer_context_modification_request> second_e1ap_request;

private:
  ocudulog::basic_logger&                 logger = ocudulog::fetch_basic_logger("TEST");
  std::optional<bearer_context_outcome_t> first_e1ap_response;
  std::optional<bearer_context_outcome_t> second_e1ap_response;
};

struct dummy_ngap_control_message_handler : public ngap_control_message_handler {
public:
  dummy_ngap_control_message_handler() = default;

  async_task<bool> handle_ue_context_release_request(const cu_cp_ue_context_release_request& msg) override
  {
    logger.info("Received a UE Context Release Request");
    return launch_no_op_task(release_request_outcome);
  }

  async_task<ngap_handover_preparation_response>
  handle_handover_preparation_request(const ngap_handover_preparation_request& request) override
  {
    return launch_no_op_task(ngap_handover_preparation_response{false});
  }

  void handle_inter_cu_ho_rrc_recfg_complete(const cu_cp_ue_index_t             ue_index,
                                             const cu_cp_user_location_info_nr& user_location_info) override
  {
    logger.info("Received a RRC Reconfiguration Complete for Inter-CU Handover");
  }

  void set_ue_context_release_request_outcome(bool outcome_) { release_request_outcome = outcome_; }

private:
  bool                    release_request_outcome = true;
  ocudulog::basic_logger& logger                  = ocudulog::fetch_basic_logger("TEST");
};

struct ue_context_outcome_t {
  bool                outcome = false;
  std::list<unsigned> drb_success_list; // List of DRB IDs that were successful to set up.
  std::list<unsigned> drb_failed_list;  // List of DRB IDs that failed to be setup.
  std::list<unsigned> drb_removed_list; // List of DRB IDs that were removed.
  byte_buffer         cell_group_cfg = make_byte_buffer("5800b24223c853a0120c7c080408c008").value();
};

struct dummy_f1ap_ue_context_manager : public f1ap_ue_context_manager {
public:
  dummy_f1ap_ue_context_manager() = default;

  void set_ue_context_setup_outcome(bool outcome) { ue_context_setup_outcome = outcome; }

  void set_ue_context_modification_outcome(ue_context_outcome_t outcome)
  {
    ue_context_modification_outcome = std::move(outcome);
  }

  async_task<f1ap_ue_context_setup_response>
  handle_ue_context_setup_request(const f1ap_ue_context_setup_request&          request,
                                  const std::optional<rrc_ue_transfer_context>& rrc_context) override
  {
    logger.info("Received a new UE context setup request");

    return launch_async([res = f1ap_ue_context_setup_response{},
                         this](coro_context<async_task<f1ap_ue_context_setup_response>>& ctx) mutable {
      CORO_BEGIN(ctx);

      res.success                          = ue_context_setup_outcome;
      res.du_to_cu_rrc_info.cell_group_cfg = make_byte_buffer("5800b24223c853a0120c7c080408c008").value();

      CORO_RETURN(res);
    });
  }

  async_task<f1ap_ue_context_modification_response>
  handle_ue_context_modification_request(const f1ap_ue_context_modification_request& request) override
  {
    logger.info("Received a new UE context modification request");

    // Store request so it can be verified in the test code.
    make_partial_copy(ue_context_modification_request, request);

    return launch_async([res = f1ap_ue_context_modification_response{},
                         this](coro_context<async_task<f1ap_ue_context_modification_response>>& ctx) mutable {
      CORO_BEGIN(ctx);

      res.success = ue_context_modification_outcome.outcome;
      for (const auto& drb_id : ue_context_modification_outcome.drb_success_list) {
        // Add only the most relevant items.
        f1ap_drb_setupmod drb_item;
        drb_item.drb_id = uint_to_drb_id(drb_id); // set ID
        res.drbs_setup_list.push_back(drb_item);
      }
      res.du_to_cu_rrc_info.cell_group_cfg = ue_context_modification_outcome.cell_group_cfg.copy();
      // TODO: add failed list and other fields here ..

      CORO_RETURN(res);
    });
  }

  async_task<cu_cp_ue_index_t> handle_ue_context_release_command(const f1ap_ue_context_release_command& msg) override
  {
    logger.info("Received a new UE context release command");

    last_release_command.ue_index = msg.ue_index;
    last_release_command.cause    = msg.cause;
    last_release_command.rrc_pdu  = msg.rrc_pdu.copy();
    last_release_command.srb_id   = msg.srb_id;
    return launch_no_op_task(msg.ue_index);
  }

  bool handle_ue_id_update(cu_cp_ue_index_t ue_index, cu_cp_ue_index_t old_ue_index) override { return true; }

  const f1ap_ue_context_modification_request& get_ctxt_mod_request() { return ue_context_modification_request; }

  f1ap_ue_context_release_command last_release_command;

private:
  static void make_partial_copy(f1ap_ue_context_modification_request&       target,
                                const f1ap_ue_context_modification_request& source)
  {
    // Only copy fields that are actually checked in unit tests.
    target.drbs_to_be_setup_mod_list = source.drbs_to_be_setup_mod_list;
    target.drbs_to_be_released_list  = source.drbs_to_be_released_list;
  }

  ocudulog::basic_logger& logger                   = ocudulog::fetch_basic_logger("TEST");
  bool                    ue_context_setup_outcome = false;
  ue_context_outcome_t    ue_context_modification_outcome;

  f1ap_ue_context_modification_request ue_context_modification_request;
};

struct dummy_cu_up_processor_cu_up_management_notifier : public cu_up_processor_cu_up_management_notifier {
public:
  dummy_cu_up_processor_cu_up_management_notifier() = default;

  void on_new_cu_up_connection() override { logger.info("Received a new CU-UP connection notification"); }

  void on_cu_up_remove_request_received(const cu_cp_cu_up_index_t cu_up_index) override
  {
    logger.info("Received a CU-UP remove request for cu_up_index={}", cu_up_index);
    last_cu_up_index_to_remove = cu_up_index;
  }

  cu_cp_cu_up_index_t last_cu_up_index_to_remove;

private:
  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("TEST");
};

struct dummy_ue_task_scheduler : public ue_task_scheduler {
public:
  dummy_ue_task_scheduler(timer_manager& timers_, task_executor& exec_) : timer_db(timers_), exec(exec_) {}
  bool           schedule_async_task(async_task<void> task) override { return ctrl_loop.schedule(std::move(task)); }
  unique_timer   create_timer() override { return timer_db.create_unique_timer(exec); }
  timer_factory  get_timer_factory() override { return timer_factory{timer_db, exec}; }
  task_executor& get_executor() override { return exec; }

  void tick_timer() const { timer_db.tick(); }

private:
  fifo_async_task_scheduler ctrl_loop{16};
  timer_manager&            timer_db;
  task_executor&            exec;
};

class dummy_cu_cp_rrc_ue_interface : public cu_cp_rrc_ue_interface
{
public:
  void add_ue_context(rrc_ue_reestablishment_context_response context) { reest_context = std::move(context); }

  bool next_ue_setup_response = true;

  rrc_ue_reestablishment_context_response
  handle_rrc_reestablishment_request(pci_t old_pci, rnti_t old_c_rnti, cu_cp_ue_index_t ue_index) override
  {
    logger.info("ue={} old_pci={} old_c-rnti={}: Received RRC Reestablishment Request", ue_index, old_pci, old_c_rnti);
    return reest_context;
  }

  async_task<rrc_ue_context_retrieval_response>
  handle_ue_context_retrieval_required(cu_cp_ue_index_t                        ue_index,
                                       const rrc_ue_context_retrieval_request& request) override
  {
    logger.info("ue={}: Received UE Context Retrieval Required", ue_index);
    return launch_no_op_task(rrc_ue_context_retrieval_response{});
  }

  async_task<bool> handle_retrieved_context_path_switch_required(cu_cp_ue_index_t ue_index) override
  {
    logger.info("ue={}: Received Retrieved Context Path Switch Required", ue_index);
    return launch_no_op_task(true);
  }

  async_task<bool> handle_rrc_reestablishment_context_modification_required(cu_cp_ue_index_t ue_index) override
  {
    logger.info("ue={}: Received Reestablishment Context Modification Required");
    return launch_no_op_task(true);
  }

  void handle_rrc_reestablishment_failure(const cu_cp_ue_context_release_request& request) override
  {
    logger.info("ue={}: Received RRC Reestablishment failure notification", request.ue_index);
  }

  void handle_rrc_reestablishment_complete(cu_cp_ue_index_t old_ue_index) override
  {
    logger.info("ue={}: Received RRC Reestablishment complete notification", old_ue_index);
  }

  async_task<bool> handle_ue_context_transfer(cu_cp_ue_index_t ue_index, cu_cp_ue_index_t old_ue_index) override
  {
    logger.info("ue={}: Requested a UE context transfer from old_ue={}", ue_index, old_ue_index);
    return launch_no_op_task(true);
  }

  async_task<void> handle_ue_context_release(const cu_cp_ue_context_release_request& request) override
  {
    logger.info("ue={}: Requested a UE release", request.ue_index);
    last_cu_cp_ue_context_release_request = request;

    return launch_no_op_task();
  }

  cu_cp_ue_context_release_request last_cu_cp_ue_context_release_request;

private:
  rrc_ue_reestablishment_context_response reest_context = {};
  ocudulog::basic_logger&                 logger        = ocudulog::fetch_basic_logger("TEST");
};

struct dummy_rrc_ue : public rrc_ue_interface, public rrc_ue_controller {
public:
  dummy_rrc_ue() = default;

  void set_rrc_reconfiguration_outcome(bool outcome) { rrc_reconfiguration_outcome = outcome; }

  void set_transaction_id(unsigned transaction_id_) { test_transaction_id = transaction_id_; }

  void set_rrc_inactive_supported(bool supported) { rrc_inactive_supported = supported; }

  // RRC UE Controller.
  void stop() override {}

  // RRC UL PDU handler
  void handle_ul_ccch_pdu(byte_buffer pdu, rnti_t rnti) override {}
  void handle_ul_dcch_pdu(srb_id_t srb_id, byte_buffer rrc_pdu, bool integrity_verified) override {}

  // RRC NGAP Message handler.
  void        handle_dl_nas_transport_message(byte_buffer nas_pdu) override {}
  byte_buffer get_packed_ue_radio_access_cap_info() const override { return byte_buffer{}; }
  byte_buffer get_packed_handover_preparation_message() override { return byte_buffer{}; }

  // RRC UE Control Message handler.

  rrc_ue_security_mode_command_context get_security_mode_command_context() override { return {}; }

  async_task<bool> handle_security_mode_complete_expected(uint8_t transaction_id) override
  {
    logger.info("Awaiting a RRC Security Mode Complete (transaction_id={})", transaction_id);
    return launch_no_op_task(true);
  }

  byte_buffer get_packed_ue_capability_rat_container_list() const override
  {
    logger.info("Received a new request to get packed UE capabilities");
    return byte_buffer{};
  }

  bool verify_reestablishment_short_mac_i(const security::sec_short_mac_i& short_mac_i,
                                          pci_t                            source_pci,
                                          rnti_t                           source_c_rnti,
                                          nr_cell_identity                 target_nci) override
  {
    logger.info("Received a request to verify a ShortMAC-I");
    short_mac_i_verified = true;
    return mac_i_valid;
  }

  bool verify_resume_mac_i(const security::sec_short_mac_i& resume_mac_i, nr_cell_identity target_nci) override
  {
    logger.info("Received a request to verify a ResumeMAC-I");
    resume_mac_i_verified = true;
    return mac_i_valid;
  }

  async_task<bool> handle_rrc_reconfiguration_request(const rrc_reconfiguration_procedure_request& msg) override
  {
    logger.info("Received a new RRC reconfiguration request");
    last_radio_bearer_cfg = msg.radio_bearer_cfg;

    return launch_no_op_task(rrc_reconfiguration_outcome);
  }

  rrc_ue_handover_reconfiguration_context
  get_rrc_ue_handover_reconfiguration_context(const rrc_reconfiguration_procedure_request& request) override
  {
    logger.info("Received a new handover reconfiguration request (transaction_id={})", test_transaction_id);
    last_radio_bearer_cfg = request.radio_bearer_cfg;
    return {test_transaction_id, byte_buffer{}};
  }

  rrc_ue_cond_reconfiguration_context
  get_rrc_ue_cond_reconfiguration_context(const rrc_reconfiguration_procedure_request& request) override
  {
    logger.info("Received a new CHO reconfiguration request (transaction_id={})", test_transaction_id);
    return {test_transaction_id, byte_buffer{}};
  }

  async_task<bool> handle_handover_reconfiguration_complete_expected(uint8_t                   transaction_id_,
                                                                     std::chrono::milliseconds timeout_ms,
                                                                     bool release_on_cancel = true) override
  {
    logger.info("Awaiting a RRC Reconfiguration Complete (transaction_id={})", transaction_id_);
    last_transaction_id = transaction_id_;
    return launch_no_op_task(rrc_reconfiguration_outcome);
  }

  bool store_ue_capabilities(byte_buffer ue_capabilities) override
  {
    last_ue_capabilities = std::move(ue_capabilities);
    return true;
  }

  async_task<bool> handle_rrc_ue_capability_transfer_request(const rrc_ue_capability_transfer_request& msg) override
  {
    logger.info("Received a new UE capability transfer request");

    return launch_no_op_task(ue_cap_transfer_outcome);
  }

  void fill_ue_derived_location(cu_cp_user_location_info_nr& user_location_info) const override
  {
    user_location_info.ue_location_derived_tac = ue_location_derived_tac;
    user_location_info.mapped_nci              = mapped_nci;
  }

  void request_coarse_ue_location() override {}

  rrc_ue_release_context
  get_rrc_ue_release_context(bool                                          requires_rrc_msg,
                             std::optional<std::chrono::seconds>           release_wait_time,
                             std::optional<rrc_inactivity_context>         inactivity_context = std::nullopt,
                             std::optional<cu_cp_release_redirect_nr_info> redirect_nr_info   = std::nullopt) override
  {
    logger.info("Received a new request to get RRC UE release context");
    rrc_ue_release_context release_context;
    return release_context;
  }

  rrc_ue_transfer_context get_transfer_context() override
  {
    logger.info("Received a new request to get RRC UE transfer context");
    return rrc_ue_transfer_context{};
  }

  std::optional<rrc_meas_cfg>
  generate_meas_config(const std::optional<rrc_meas_cfg>& current_meas_config = std::nullopt,
                       bool                               cond_meas           = false,
                       span<const pci_t>                  candidate_pcis      = {}) override
  {
    logger.info("Received a new request to generate RRC UE meas config");
    std::optional<rrc_meas_cfg> meas_config;
    return meas_config;
  }

  byte_buffer get_packed_meas_config(span<const pci_t> /* candidate_pcis */ = {}) override
  {
    logger.info("Received a new request to get packed RRC UE meas config");
    return {};
  }

  void update_meas_config(const rrc_meas_cfg& /* cfg */) override {}

  std::optional<rrc_meas_cfg> get_meas_config() override { return std::nullopt; }

  std::optional<byte_buffer> get_packed_location_meas_info() const override { return std::nullopt; }

  std::optional<uint8_t> get_serving_cell_mo() override
  {
    logger.info("Received a new request to get serving cell measurement object");
    return {};
  }

  byte_buffer handle_rrc_handover_command(byte_buffer cmd) override { return byte_buffer{}; }

  byte_buffer get_rrc_handover_command(const rrc_reconfiguration_procedure_request& request,
                                       unsigned                                     transaction_id_) override
  {
    logger.info("Received a new request to get a RRC Handover Command");
    return byte_buffer{};
  }

  bool handle_rrc_handover_preparation_info(byte_buffer pdu) override
  {
    logger.info("Received HandoverPreparationInfo");
    return true;
  }

  void create_srb(const srb_creation_message& msg) override
  {
    logger.info("ue={} Creating {}", msg.ue_index, msg.srb_id);
    last_srb_id = msg.srb_id;
    srb_vec.push_back(msg.srb_id);
  }

  static_vector<srb_id_t, MAX_NOF_SRBS> get_srbs() override { return srb_vec; }

  void set_rrc_state(rrc_state state) override {}

  rrc_state get_rrc_state() const override { return rrc_state::connected; }

  void cancel_handover_reconfiguration_transaction(uint8_t transaction_id) override
  {
    logger.info("Received a new request to cancel RRC UE handover reconfiguration transaction");
  }

  void cancel_all_transactions(protocol_transaction_failure cause = protocol_transaction_failure::cancel) override
  {
    logger.info("Cancelling all ongoing RRC UE transactions");
  }

  // RRC UE Setup proc notifier.
  void on_new_dl_ccch(const asn1::rrc_nr::dl_ccch_msg_s& dl_ccch_msg) override {}
  void on_ue_release_required(const ngap_cause_t& cause) override {}

  // RRC UE Security Mode Command proc notifier.
  void on_new_dl_dcch(srb_id_t srb_id, const asn1::rrc_nr::dl_dcch_msg_s& dl_dcch_msg) override {}

  // RRC UE Context handler.
  rrc_ue_reestablishment_context_response get_context() override
  {
    logger.info("Received a new request to get RRC UE reestablishment context");
    return rrc_ue_reestablishment_context_response{};
  }

  void update_c_rnti(rnti_t crnti) override {}

  rrc_cell_context get_cell_context() const override
  {
    logger.info("Received a new request to get RRC UE cell context");
    return rrc_cell_context{};
  }

  void update_cell_group_config(byte_buffer cell_group_config) override
  {
    logger.info("Received a new request to update packed cell group config");
    cell_group_cfg = cell_group_config.copy();
  }

  byte_buffer& get_cell_group_config() override
  {
    logger.info("Received a new request to get packed cell group config");
    return cell_group_cfg;
  }

  // RRC UE capability handler.
  bool is_rrc_inactive_supported() const override { return rrc_inactive_supported; }
  bool is_conditional_handover_supported() const override { return true; }
  bool is_conditional_handover_two_trigger_events_supported() const override { return true; }
  bool is_conditional_handover_event_a4_supported() const override { return true; }
  bool is_conditional_handover_location_based_supported() const override { return true; }
  bool is_conditional_handover_time_based_supported() const override { return true; }

  // RRC UE Reestablishment proc notifier.
  void on_new_as_security_context(bool security_mode_active) override {}
  void on_as_security_activated() override {}
  void on_ul_nas_pdu(byte_buffer nas_pdu) override {}

  // Interface functions.
  rrc_ue_controller&              get_controller() override { return *this; }
  rrc_ul_pdu_handler&             get_ul_pdu_handler() override { return *this; }
  rrc_ngap_message_handler&       get_rrc_ngap_message_handler() override { return *this; }
  rrc_ue_control_message_handler& get_rrc_ue_control_message_handler() override { return *this; }
  rrc_ue_context_handler&         get_rrc_ue_context_handler() override { return *this; }
  rrc_ue_capability_handler&      get_rrc_ue_capability_handler() override { return *this; }

  std::optional<rrc_radio_bearer_config> last_radio_bearer_cfg;
  void                                   reset() { last_radio_bearer_cfg.reset(); }

  /// Outcome the dummy reports for a ShortMAC-I or ResumeMAC-I verification.
  bool mac_i_valid = true;
  /// Set when the corresponding token was verified, so a test can tell the two verification paths apart.
  bool short_mac_i_verified  = false;
  bool resume_mac_i_verified = false;

  unsigned    last_transaction_id;
  srb_id_t    last_srb_id;
  byte_buffer last_ue_capabilities;

private:
  ocudulog::basic_logger&               logger                  = ocudulog::fetch_basic_logger("TEST");
  bool                                  ue_cap_transfer_outcome = true;
  std::optional<tac_t>                  ue_location_derived_tac;
  std::optional<nr_cell_identity>       mapped_nci;
  bool                                  rrc_reconfiguration_outcome = false;
  unsigned                              test_transaction_id;
  static_vector<srb_id_t, MAX_NOF_SRBS> srb_vec;
  bool                                  rrc_inactive_supported = false;
  byte_buffer                           cell_group_cfg;
};

struct dummy_cu_cp_xnap_handler : public cu_cp_xnap_handler {
public:
  dummy_cu_cp_xnap_handler(ue_manager& ue_mng_) : ue_mng(ue_mng_), logger(ocudulog::fetch_basic_logger("TEST")) {}

  async_task<bool> handle_new_rrc_handover_command(cu_cp_rrc_handover_command      command,
                                                   std::optional<xnc_peer_index_t> xnc_index) override
  {
    logger.info("ue={}: Received a new RRC Handover Command for {} handover",
                command.ue_index,
                xnc_index.has_value() ? "XN" : "NG");
    last_handover_command = std::move(command);
    return launch_no_op_task(true);
  }

  expected<cu_cp_ue_index_t, xnap_cause_t> handle_ue_index_allocation_request_for_guami(const nr_cell_global_id_t& cgi,
                                                                                        const guami_t& guami) override
  {
    return make_unexpected(xnap_cause_misc_t::not_enough_user_plane_processing_res);
  }

  cu_cp_ue_index_t handle_ue_index_allocation_request_for_amf(const nr_cell_global_id_t& cgi,
                                                              const plmn_identity&       plmn,
                                                              cu_cp_amf_index_t          amf_index) override
  {
    return cu_cp_ue_index_t::invalid;
  }

  bool handle_handover_request(cu_cp_ue_index_t ue_index, const security::security_context& sec_ctxt) override
  {
    ocudu_assert(ue_mng.find_ue(ue_index) != nullptr, "UE must be present");
    logger.info("Received a handover request");

    if (!ue_mng.find_ue(ue_index)->get_security_manager().init_handover_security_context(sec_ctxt)) {
      logger.info("Failed to initialize security context");
      return false;
    }

    return true;
  }

  bool schedule_ue_task(cu_cp_ue_index_t ue_index, async_task<void> task) override
  {
    ocudu_assert(ue_mng.find_ue_task_scheduler(ue_index) != nullptr, "UE task scheduler must be present");
    return ue_mng.find_ue_task_scheduler(ue_index)->schedule_async_task(std::move(task));
  }

  async_task<cu_cp_handover_resource_allocation_response>
  handle_xnap_handover_request(const xnap_handover_request& request) override
  {
    return launch_no_op_task(cu_cp_handover_resource_allocation_response{});
  }

  void handle_inter_cu_target_handover_execution(
      cu_cp_ue_index_t                                             ue_index,
      const std::optional<xnap_handover_target_execution_context>& xnap_ho_target_execution_ctxt) override
  {
    logger.info("ue={}: Received a new {} request to handle inter-CU target handover execution",
                ue_index,
                xnap_ho_target_execution_ctxt.has_value() ? "Xn-C" : "NG");
  }

  void handle_transmission_of_xnap_handover_request() override { logger.info("Sent a handover request message"); }

  void handle_handover_cancel_received(cu_cp_ue_index_t ue_index) override
  {
    logger.info("ue={}: Received a handover cancel message", ue_index);
  }

  void handle_xnap_handover_success_received(cu_cp_ue_index_t           source_ue_index,
                                             const nr_cell_global_id_t& winner_cgi) override
  {
    logger.info("ue={}: Received a HandoverSuccess message", source_ue_index);
  }

  void handle_xnap_ue_context_release_received(cu_cp_ue_index_t ue_index) override
  {
    logger.info("ue={}: Received a XNAP UE context release message", ue_index);
  }

  std::vector<cu_cp_served_cell_info> handle_served_cells_required() override { return served_cells; }

  cu_cp_ue_index_t handle_xnap_ue_context_id_lookup(const xnap_ue_context_id& ue_context_id) override
  {
    logger.info("Received a UE Context ID lookup request");
    return cu_cp_ue_index_t::invalid;
  }

  async_task<xnap_retrieve_ue_context_response>
  handle_xnap_retrieve_ue_context_request(const xnap_retrieve_ue_context_request& request) override
  {
    logger.info("ue={}: Received a Retrieve UE Context Request", request.ue_index);
    return launch_no_op_task(xnap_retrieve_ue_context_response{});
  }

  cu_cp_rrc_handover_command          last_handover_command;
  std::vector<cu_cp_served_cell_info> served_cells;

private:
  ue_manager&             ue_mng;
  ocudulog::basic_logger& logger;
};

} // namespace ocudu::ocucp
