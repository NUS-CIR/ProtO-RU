// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "du_context/nrppa_du_context.h"
#include "du_context/nrppa_trp_registry.h"
#include "meas_context/nrppa_meas_context.h"
#include "ue_context/nrppa_ue_context.h"
#include "ocudu/asn1/nrppa/nrppa.h"
#include "ocudu/nrppa/nrppa.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/ran/positioning/positioning_ids.h"
#include "ocudu/ran/supported_tracking_area.h"
#include "ocudu/support/async/async_task_scheduler.h"

namespace ocudu::ocucp {

class nrppa_impl : public nrppa_interface,
                   public nrppa_message_handler,
                   public nrppa_ue_context_removal_handler,
                   public nrppa_du_context_handler,
                   public nrppa_e_cid_report_handler
{
public:
  nrppa_impl(const std::vector<supported_tracking_area>& supported_tas_,
             nrppa_cu_cp_notifier&                       cu_cp_notifier_,
             async_task_scheduler&                       common_task_sched_,
             timer_manager&                              timers_,
             task_executor&                              task_exec_);
  ~nrppa_impl();

  // See nrppa_message_handler for documentation.
  void handle_new_nrppa_pdu(const byte_buffer&                                nrppa_pdu,
                            std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index) override;

  // See nrppa_ue_context_removal_handler for documentation.
  void remove_ue_context(cu_cp_ue_index_t ue_index) override;
  void update_ue_index(cu_cp_ue_index_t         new_ue_index,
                       cu_cp_ue_index_t         old_ue_index,
                       nrppa_cu_cp_ue_notifier& new_ue_notifier) override;

  // See nrppa_du_context_handler for documentation.
  void handle_du_addition(cu_cp_du_index_t du_index, nrppa_f1ap_notifier& f1ap_notifier) override;
  void handle_du_removal(cu_cp_du_index_t du_index) override;

  // See nrppa_e_cid_report_handler for documentation.
  void handle_e_cid_measurement_report(const e_cid_measurement_report_t& report) override;
  void handle_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind) override;

  /// \brief Tell the gNB-DU to stop the E-CID measurement it runs for the UE.
  /// \param[in] ue_index The UE index.
  void terminate_du_e_cid_measurement(cu_cp_ue_index_t ue_index);

  /// \brief Initialize the measurement report timer for the UE.
  /// \param[in] ue_index The UE index.
  /// \param[in] meas_periodicity_ms The measurement periodicity in milliseconds.
  void initialize_meas_report_timer(cu_cp_ue_index_t ue_index, std::chrono::milliseconds meas_periodicity_ms);

  nrppa_message_handler&            get_nrppa_message_handler() override { return *this; }
  nrppa_ue_context_removal_handler& get_nrppa_ue_context_removal_handler() override { return *this; }
  nrppa_du_context_handler&         get_nrppa_du_context_handler() override { return *this; }
  nrppa_e_cid_report_handler&       get_nrppa_e_cid_report_handler() override { return *this; }

private:
  /// \brief Send the measurement results to the LMF.
  void handle_e_cid_meas_result(cu_cp_ue_index_t ue_index, const nrppa_e_cid_meas_result& result);

  /// \brief Send an E-CID measurement failure indication to the LMF.
  void send_e_cid_meas_failure_indication(const nrppa_ue_context& ue_ctxt, nrppa_cause_t cause);

  /// \brief Callback function for the measurement report timer. This sends a measurement report to the LMF.
  void on_meas_report_timer_expired(cu_cp_ue_index_t ue_index);

  /// \brief Handle the reception of an initiating message.
  /// \param[in] msg The received initiating message.
  /// \param[in] ue_or_amf_index The UE index for UE associated NRPPa messages or the AMF index for non UE associated
  /// NRPPa messages.
  void handle_initiating_message(const asn1::nrppa::init_msg_s&                    msg,
                                 std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index);

  /// \brief Handle an E-CID measurement initiation request.
  void handle_e_cid_meas_initiation_request(const asn1::nrppa::e_c_id_meas_initiation_request_s& msg,
                                            cu_cp_ue_index_t                                     ue_index,
                                            uint16_t                                             transaction_id);

  /// \brief Handle an E-CID measurement termination command.
  void handle_e_cid_meas_termination_command(const asn1::nrppa::e_c_id_meas_termination_cmd_s& msg,
                                             cu_cp_ue_index_t                                  ue_index);

  /// \brief Handle a TRP information request.
  void handle_trp_information_request(const asn1::nrppa::trp_info_request_s& msg,
                                      cu_cp_amf_index_t                      amf_index,
                                      uint16_t                               transaction_id);

  /// \brief Handle a positioning information request.
  void handle_positioning_information_request(const asn1::nrppa::positioning_info_request_s& msg,
                                              cu_cp_ue_index_t                               ue_index,
                                              uint16_t                                       transaction_id);

  /// \brief Handle a positioning activation request.
  void handle_positioning_activation_request(const asn1::nrppa::positioning_activation_request_s& msg,
                                             cu_cp_ue_index_t                                     ue_index,
                                             uint16_t                                             transaction_id);

  /// \brief Handle a measurement request.
  void handle_measurement_request(const asn1::nrppa::meas_request_s& msg,
                                  cu_cp_amf_index_t                  amf_index,
                                  uint16_t                           transaction_id);

  /// \brief Handle the reception of a successful outcome message.
  /// \param[in] outcome The successful outcome message.
  void handle_successful_outcome(const asn1::nrppa::successful_outcome_s& outcome);

  /// \brief Handle the reception of an unsuccessful outcome message.
  /// \param[in] outcome The unsuccessful outcome message.
  void handle_unsuccessful_outcome(const asn1::nrppa::unsuccessful_outcome_s& outcome);

  ocudulog::basic_logger& logger;
  nrppa_ue_context_list   ue_ctxt_list;
  nrppa_du_context_list   du_ctxt_list;
  nrppa_meas_context_list meas_ctxt_list;
  nrppa_cu_cp_notifier&   cu_cp_notifier;
  async_task_scheduler&   common_task_sched;
  timer_manager&          timers;
  task_executor&          task_exec;

  std::map<plmn_identity, tac_t> plmn_to_tac;
  nrppa_trp_registry             trp_registry;
};

} // namespace ocudu::ocucp
