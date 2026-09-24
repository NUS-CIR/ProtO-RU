// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "procedures/f1ap_cu_event_manager.h"
#include "ue_context/f1ap_cu_ue_context.h"
#include "ocudu/asn1/f1ap/f1ap.h"
#include "ocudu/f1ap/cu_cp/f1ap_configuration.h"
#include "ocudu/f1ap/cu_cp/f1ap_cu.h"
#include "ocudu/f1ap/cu_cp/f1ap_cu_configuration_update.h"
#include "ocudu/f1ap/cu_cp/f1ap_nrppa_msg_handling.h"
#include "ocudu/f1ap/f1ap_message_notifier.h"
#include "ocudu/f1ap/f1ap_ue_id_types.h"
#include "ocudu/support/executors/task_executor.h"
#include <set>

namespace ocudu::ocucp {

class f1ap_cu_impl final : public f1ap_cu
{
public:
  f1ap_cu_impl(const f1ap_configuration&    f1ap_cfg_,
               f1ap_message_notifier&       tx_pdu_notifier_,
               f1ap_du_processor_notifier&  f1ap_du_processor_notifier_,
               gnb_cu_ue_f1ap_id_allocator& cu_ue_id_alloc_,
               timer_manager&               timers_,
               task_executor&               ctrl_exec_);
  ~f1ap_cu_impl() override;

  const f1ap_du_context& get_context() const override;

  async_task<void> stop() override;

  // f1ap_rrc_message_handler functions.
  void handle_dl_rrc_message_transfer(const f1ap_dl_rrc_message& msg) override;

  // f1ap_ue_context_manager functions.
  async_task<f1ap_ue_context_setup_response>
  handle_ue_context_setup_request(const f1ap_ue_context_setup_request&          request,
                                  const std::optional<rrc_ue_transfer_context>& rrc_context) override;

  async_task<cu_cp_ue_index_t> handle_ue_context_release_command(const f1ap_ue_context_release_command& msg) override;

  async_task<f1ap_ue_context_modification_response>
  handle_ue_context_modification_request(const f1ap_ue_context_modification_request& request) override;

  bool handle_ue_id_update(cu_cp_ue_index_t ue_index, cu_cp_ue_index_t old_ue_index) override;

  // f1ap_paging_handler functions.
  void handle_paging(const cu_cp_paging_message& msg) override;

  // f1ap_message_handler functions.
  void handle_message(const f1ap_message& msg) override;

  // f1ap_statistics_handler functions.
  size_t get_nof_ues() const override { return ue_ctxt_list.size(); }

  // f1ap_ue_context_removal_handler functions.
  void remove_ue_context(cu_cp_ue_index_t ue_index) override;

  // f1ap_nrppa_message_handler functions.
  async_task<expected<trp_information_response_t, trp_information_failure_t>>
  handle_trp_information_request(const trp_information_request_t& request) override;
  async_task<expected<positioning_information_response_t, positioning_information_failure_t>>
  handle_positioning_information_request(const positioning_information_request_t& request) override;
  async_task<expected<positioning_activation_response_t, positioning_activation_failure_t>>
  handle_positioning_activation_request(const positioning_activation_request_t& request) override;
  async_task<expected<measurement_response_t, measurement_failure_t>>
  handle_positioning_measurement_request(const measurement_request_t& request) override;

  async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>
  handle_e_cid_measurement_request(const e_cid_measurement_request_t& request) override;

  void handle_e_cid_measurement_termination_command(const e_cid_measurement_termination_command_t& command) override;

  // f1ap_interface_management_handler functions.
  async_task<f1ap_gnb_cu_configuration_update_response>
  handle_gnb_cu_configuration_update(const f1ap_gnb_cu_configuration_update& request) override;

  void handle_ref_time_info_report_ctrl(const f1ap_ref_time_report_ctrl_request& request) override;

  // f1ap_warning_manager functions.
  async_task<f1ap_write_replace_warning_response>
  handle_write_replace_warning_request(const f1ap_write_replace_warning_request& request) override;

  // f1ap_cu_interface
  f1ap_message_handler&              get_f1ap_message_handler() override { return *this; }
  f1ap_rrc_message_handler&          get_f1ap_rrc_message_handler() override { return *this; }
  f1ap_ue_context_manager&           get_f1ap_ue_context_manager() override { return *this; }
  f1ap_statistics_handler&           get_f1ap_statistics_handler() override { return *this; }
  f1ap_paging_manager&               get_f1ap_paging_manager() override { return *this; }
  f1ap_ue_context_removal_handler&   get_f1ap_ue_context_removal_handler() override { return *this; }
  f1ap_nrppa_message_handler&        get_f1ap_nrppa_message_handler() override { return *this; }
  f1ap_interface_management_handler& get_f1ap_interface_management_handler() override { return *this; }
  f1ap_warning_manager&              get_f1ap_warning_manager() override { return *this; }

private:
  class tx_pdu_notifier_with_logging final : public f1ap_message_notifier
  {
  public:
    tx_pdu_notifier_with_logging(f1ap_cu_impl& parent_, f1ap_message_notifier& decorated_) :
      parent(parent_), decorated(decorated_)
    {
    }

    void on_new_message(const f1ap_message& msg) override;

  private:
    f1ap_cu_impl&          parent;
    f1ap_message_notifier& decorated;
  };

  /// \brief Handle the reception of an initiating message.
  /// \param[in] msg The received initiating message.
  void handle_initiating_message(const asn1::f1ap::init_msg_s& msg);

  /// \brief Handle the reception of an F1 Setup Request.
  /// \param[in] request The F1 Setup Request.
  void handle_f1_setup_request(const asn1::f1ap::f1_setup_request_s& request);

  /// \brief Handle the reception of an F1 Removal Request as per TS 38.473 section 8.2.8.
  /// \param[in] msg The initiating message that contains the F1 Removal Request.
  void handle_f1_removal_request(const asn1::f1ap::init_msg_s& msg);

  /// \brief Handle the reception of an Initial UL RRC Message Transfer message.
  /// This starts the UE and SRB creation at the DU processor, F1 and RRC UE.
  /// @see rrc_setup_procedure.
  /// \param[in] msg The F1AP initial UL RRC message.
  void handle_initial_ul_rrc_message(const asn1::f1ap::init_ul_rrc_msg_transfer_s& msg);

  /// \brief Handle the reception of an UL RRC Message Transfer message.
  /// \param[in] msg The F1AP UL RRC message.
  void handle_ul_rrc_message(const asn1::f1ap::ul_rrc_msg_transfer_s& msg);

  /// \brief Handle the reception of an successful outcome.
  /// \param[in] msg The received successful outcome message.
  void handle_successful_outcome(const asn1::f1ap::successful_outcome_s& outcome);

  /// \brief Handle the reception of an unsuccessful outcome.
  /// \param[in] msg The received unsuccessful outcome message.
  void handle_unsuccessful_outcome(const asn1::f1ap::unsuccessful_outcome_s& outcome);

  /// \brief Handle the reception of an UE Context Release Request.
  /// \param[in] msg The UE Context Release Request message.
  void handle_ue_context_release_request(const asn1::f1ap::ue_context_release_request_s& msg);

  /// \brief Handle the reception of an Access Success message from the DU.
  /// \param[in] msg The Access Success message, indicating that the UE has successfully accessed the target cell
  ///                during a Conditional Handover execution.
  void handle_access_success(const asn1::f1ap::access_success_s& msg);

  /// \brief Handle the reception of a gNB-DU Configuration Update.
  /// \param[in] msg The gNB-DU Configuration Update message.
  void handle_du_cfg_update(const asn1::f1ap::gnb_du_cfg_upd_s& request);

  /// \brief Handle REFERENCE TIME INFORMATION REPORT from the gNB-DU as per TS 38.473 section 8.12.2.
  void handle_ref_time_info_report(const asn1::f1ap::ref_time_info_report_s& msg);

  /// \brief Handle an E-CID Measurement Report from the gNB-DU, as per TS 38.473 section 8.13.14.
  void handle_e_cid_measurement_report(const asn1::f1ap::e_c_id_meas_report_s& msg);

  /// \brief Handle an E-CID Measurement Failure Indication from the gNB-DU, as per TS 38.473 section 8.13.13.
  void handle_e_cid_measurement_failure_indication(const asn1::f1ap::e_c_id_meas_fail_ind_s& msg);

  /// \brief Log F1AP PDU.
  void log_pdu(bool is_rx, const f1ap_message& pdu);

  const f1ap_configuration cfg;
  ocudulog::basic_logger&  logger;

  // DU context.
  f1ap_du_context du_ctxt;

  // Repository of UE Contexts.
  f1ap_ue_context_list ue_ctxt_list;

  // Nofifiers and handles.
  f1ap_du_processor_notifier& du_processor_notifier;
  task_executor&              ctrl_exec;

  tx_pdu_notifier_with_logging tx_pdu_notifier;
  f1ap_event_manager           ev_mng;

  // Counter used to allocate transaction IDs for CU-initiated Reference Time Information Reporting Control messages.
  uint8_t next_ref_time_transaction_id = 0;
};

} // namespace ocudu::ocucp
