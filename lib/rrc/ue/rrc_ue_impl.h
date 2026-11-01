// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "procedures/rrc_ue_event_manager.h"
#include "rrc_ue_context.h"
#include "rrc_ue_logger.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg_ies.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/rrc/rrc_cell_context.h"
#include "ocudu/rrc/rrc_ue.h"

namespace ocudu::ocucp {

/// Main UE representation in RRC
class rrc_ue_impl final : public rrc_ue_interface, public rrc_ue_controller
{
public:
  rrc_ue_impl(rrc_pdu_f1ap_notifier&                 f1ap_pdu_notifier_,
              rrc_ue_ngap_notifier&                  ngap_notifier_,
              rrc_ue_context_update_notifier&        cu_cp_notifier_,
              rrc_ue_measurement_notifier&           measurement_notifier_,
              rrc_ue_cu_cp_ue_notifier&              cu_cp_ue_notifier_,
              rrc_ue_event_notifier&                 metrics_notifier_,
              rrc_ue_srb_pdcp_manager&               pdcp_manager_,
              cu_cp_ue_index_t                       ue_index_,
              rnti_t                                 c_rnti_,
              const rrc_cell_context&                cell_,
              const rrc_ue_cfg_t&                    cfg_,
              const byte_buffer&                     du_to_cu_container_,
              std::optional<rrc_ue_transfer_context> rrc_context,
              std::optional<rrc_resume_context_t>    remote_resume_context = std::nullopt);
  ~rrc_ue_impl();

  // rrc_ul_pdu_handler
  void handle_ul_ccch_pdu(byte_buffer pdu, rnti_t c_rnti) override;
  void handle_ul_dcch_pdu(srb_id_t srb_id, byte_buffer rrc_pdu, bool integrity_verified) override;

  // rrc_ue_interface
  rrc_ue_controller&              get_controller() override { return *this; }
  rrc_ul_pdu_handler&             get_ul_pdu_handler() override { return *this; }
  rrc_ngap_message_handler&       get_rrc_ngap_message_handler() override { return *this; }
  rrc_ue_control_message_handler& get_rrc_ue_control_message_handler() override { return *this; }
  rrc_ue_context_handler&         get_rrc_ue_context_handler() override { return *this; }
  rrc_ue_capability_handler&      get_rrc_ue_capability_handler() override { return *this; }

  // rrc_ngap_message_handler
  void        handle_dl_nas_transport_message(byte_buffer nas_pdu) override;
  byte_buffer get_packed_handover_preparation_message() override;

  // rrc_ue_control_message_handler
  rrc_ue_security_mode_command_context get_security_mode_command_context() override;
  async_task<bool>                     handle_security_mode_complete_expected(uint8_t transaction_id) override;
  byte_buffer                          get_packed_ue_capability_rat_container_list() const override;
  bool                                 verify_reestablishment_short_mac_i(const security::sec_short_mac_i& short_mac_i,
                                                                          pci_t                            source_pci,
                                                                          rnti_t                           source_c_rnti,
                                                                          nr_cell_identity                 target_nci) override;
  bool        verify_resume_mac_i(const security::sec_short_mac_i& resume_mac_i, nr_cell_identity target_nci) override;
  byte_buffer get_packed_ue_radio_access_cap_info() const override;
  async_task<bool> handle_rrc_reconfiguration_request(const rrc_reconfiguration_procedure_request& msg) override;
  rrc_ue_handover_reconfiguration_context
  get_rrc_ue_handover_reconfiguration_context(const rrc_reconfiguration_procedure_request& request) override;
  rrc_ue_cond_reconfiguration_context
  get_rrc_ue_cond_reconfiguration_context(const rrc_reconfiguration_procedure_request& request) override;
  async_task<bool> handle_handover_reconfiguration_complete_expected(uint8_t                   transaction_id,
                                                                     std::chrono::milliseconds timeout_ms,
                                                                     bool release_on_failure = true) override;
  bool             store_ue_capabilities(byte_buffer ue_capabilities) override;
  async_task<bool> handle_rrc_ue_capability_transfer_request(const rrc_ue_capability_transfer_request& msg) override;
  void             fill_ue_derived_location(cu_cp_user_location_info_nr& user_location_info) const override;
  void             request_coarse_ue_location() override;

  /// Returns the TAC of the configured area holding the coarse position the UE reported, if any.
  std::optional<tac_t> get_ue_location_derived_tac() const;

  /// Returns the Mapped Cell ID of the configured area holding the coarse position the UE reported, if any.
  std::optional<nr_cell_identity> get_ue_mapped_cell_id() const;
  rrc_ue_release_context
                          get_rrc_ue_release_context(bool                                          requires_rrc_message,
                                                     std::optional<std::chrono::seconds>           release_wait_time = std::nullopt,
                                                     std::optional<rrc_inactivity_context>         inactivity_context = std::nullopt,
                                                     std::optional<cu_cp_release_redirect_nr_info> redirect_nr_info = std::nullopt) override;
  rrc_ue_transfer_context get_transfer_context() override;
  std::optional<rrc_meas_cfg>
              generate_meas_config(const std::optional<rrc_meas_cfg>& current_meas_config = std::nullopt,
                                   bool                               cond_meas           = false,
                                   span<const pci_t>                  candidate_pcis      = {}) override;
  byte_buffer get_packed_meas_config(span<const pci_t> candidate_pcis = {}) override;
  void        update_meas_config(const rrc_meas_cfg& cfg) override;
  std::optional<rrc_meas_cfg>           get_meas_config() override { return context.meas_cfg; }
  std::optional<byte_buffer>            get_packed_location_meas_info() const override;
  std::optional<uint8_t>                get_serving_cell_mo() override;
  byte_buffer                           get_rrc_handover_command(const rrc_reconfiguration_procedure_request& request,
                                                                 unsigned                                     transaction_id) override;
  byte_buffer                           handle_rrc_handover_command(byte_buffer cmd) override;
  bool                                  handle_rrc_handover_preparation_info(byte_buffer pdu) override;
  void                                  create_srb(const srb_creation_message& msg) override;
  static_vector<srb_id_t, MAX_NOF_SRBS> get_srbs() override;
  void                                  set_rrc_state(rrc_state state) override { context.state = state; }
  rrc_state                             get_rrc_state() const override;
  void                                  cancel_handover_reconfiguration_transaction(uint8_t transaction_id) override;
  void cancel_all_transactions(protocol_transaction_failure cause = protocol_transaction_failure::cancel) override;

  // rrc_ue_context_handler
  rrc_ue_reestablishment_context_response get_context() override;
  void                                    update_c_rnti(rnti_t crnti) override;
  rrc_cell_context                        get_cell_context() const override { return context.cell; }
  void                                    update_cell_group_config(byte_buffer cell_group_config) override;
  byte_buffer&                            get_cell_group_config() override { return context.cell_group_config; }

  // rrc_ue_capability_handler
  bool is_rrc_inactive_supported() const override { return context.capabilities.rrc_inactive_supported; }
  bool is_conditional_handover_supported() const override
  {
    return context.capabilities.conditional_handover_supported;
  }
  bool is_conditional_handover_two_trigger_events_supported() const override
  {
    return context.capabilities.conditional_handover_two_trigger_events_supported;
  }
  bool is_conditional_handover_event_a4_supported() const override
  {
    return context.capabilities.conditional_handover_event_a4_supported;
  }
  bool is_conditional_handover_location_based_supported() const override
  {
    return context.capabilities.conditional_handover_location_based_supported;
  }
  bool is_conditional_handover_time_based_supported() const override
  {
    return context.capabilities.conditional_handover_time_based_supported;
  }

private:
  void stop() override;

  // message handlers
  void handle_pdu(srb_id_t srb_id, byte_buffer rrc_pdu, bool integrity_verified);
  void handle_rrc_setup_request(const asn1::rrc_nr::rrc_setup_request_s& msg);
  void handle_rrc_reest_request(const asn1::rrc_nr::rrc_reest_request_s& msg);
  void handle_rrc_resume_request(const asn1::rrc_nr::rrc_resume_request_s& msg, rnti_t c_rnti);
  void handle_ul_info_transfer(const asn1::rrc_nr::ul_info_transfer_ies_s& ul_info_transfer);
  void handle_security_mode_complete(const asn1::rrc_nr::security_mode_complete_s& msg);

  void handle_measurement_report(const asn1::rrc_nr::meas_report_s& msg);
  void handle_location_measurement_indication(const asn1::rrc_nr::location_meas_ind_s& msg);
  void handle_rrc_transaction_complete(const asn1::rrc_nr::ul_dcch_msg_s& msg, uint8_t transaction_id_);
  /// Dispatches the UL-DCCH message class extension, which is where UEInformationResponse lives.
  void handle_ul_dcch_msg_class_ext(const asn1::rrc_nr::ul_dcch_msg_s& ul_dcch_msg, bool integrity_verified);
  void cancel_rrc_transaction(uint8_t transaction_id_);

  // message senders
  /// Packs a DL-CCCH message and logs the message
  void send_dl_ccch(const asn1::rrc_nr::dl_ccch_msg_s& dl_ccch_msg);
  void send_dl_dcch(srb_id_t srb_id, const asn1::rrc_nr::dl_dcch_msg_s& dl_dcch_msg);

  // rrc_ue_setup_proc_notifier
  void on_new_dl_ccch(const asn1::rrc_nr::dl_ccch_msg_s& dl_ccch_msg) override;
  void on_ue_release_required(const ngap_cause_t& cause) override;

  // rrc_ue_security_mode_command_proc_notifier
  void on_new_dl_dcch(srb_id_t srb_id, const asn1::rrc_nr::dl_dcch_msg_s& dl_dcch_msg) override;
  void on_new_as_security_context(bool security_mode_active) override;
  void on_as_security_activated() override;
  void on_ul_nas_pdu(byte_buffer nas_pdu) override;

  // helpers
  void handle_illegal_pdu_integrity(const char* msg, bool integrity_verified);

  rrc_ue_logger                   logger;
  rrc_ue_context_t                context;
  rrc_pdu_f1ap_notifier&          f1ap_pdu_notifier;    // PDU notifier to the F1AP
  rrc_ue_ngap_notifier&           ngap_notifier;        // notifier to the NGAP
  rrc_ue_context_update_notifier& cu_cp_notifier;       // notifier to the CU-CP
  rrc_ue_measurement_notifier&    measurement_notifier; // cell measurement notifier
  rrc_ue_cu_cp_ue_notifier&       cu_cp_ue_notifier;    // cu-cp ue notifier
  rrc_ue_event_notifier&          metrics_notifier;     // metrics notifier
  byte_buffer                     du_to_cu_container;   // initial RRC message from DU to CU

  // RRC procedures handling
  std::unique_ptr<rrc_ue_event_manager> event_mng;
};

} // namespace ocudu::ocucp
