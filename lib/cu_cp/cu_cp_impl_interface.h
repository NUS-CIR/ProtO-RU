// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "du_processor/du_reported_cell.h"
#include "ocudu/adt/expected.h"
#include "ocudu/cu_cp/cell_meas_manager_config.h"
#include "ocudu/cu_cp/cu_cp_intra_cu_ho_types.h"
#include "ocudu/e1ap/cu_cp/e1ap_cu_cp_bearer_context_update.h"
#include "ocudu/e1ap/cu_cp/inactivity_notification.h"
#include "ocudu/ngap/ngap.h"
#include "ocudu/ngap/ngap_pdu_session.h"
#include "ocudu/nrppa/nrppa.h"
#include "ocudu/ran/cu_cp_cell_configuration.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/rrc/rrc_ue.h"
#include "ocudu/xnap/xnap_handover.h"
#include "ocudu/xnap/xnap_ue_context_retrieval.h"

namespace ocudu::ocucp {

class cu_cp_ue_context_release_handler
{
public:
  virtual ~cu_cp_ue_context_release_handler() = default;

  /// \brief Handle the reception of a new UE Context Release Command.
  /// \param[in] command The UE Context Release Command.
  /// \returns The UE Context Release Complete.
  virtual async_task<cu_cp_ue_context_release_complete>
  handle_ue_context_release_command(const cu_cp_ue_context_release_command& command) = 0;
};

/// Interface for the CU-CP to schedule tasks for UEs.
class cu_cp_task_scheduler_handler
{
public:
  virtual ~cu_cp_task_scheduler_handler() = default;

  /// \brief Schedule a task for a UE.
  /// \param[in] ue_index The index of the UE.
  /// \param[in] task The task to schedule.
  /// \returns True if the task was successfully scheduled, false otherwise.
  virtual bool schedule_ue_task(cu_cp_ue_index_t ue_index, async_task<void> task) = 0;
};

/// Interface for the inter-CU handover notifier to communicate with the CU-CP.
class cu_cp_inter_cu_handover_handler
{
public:
  virtual ~cu_cp_inter_cu_handover_handler() = default;

  /// \brief Handle the reception of a new RRC Handover Command.
  /// \param[in] command The received Handover Command, including the data forwarding tunnels of the target.
  /// \param[in] xnc_index The Xn-C index if the handover is a Xn-C handover, std::nullopt otherwise.
  /// \returns True if the RRC Handover Command was successfully handled, false otherwise.
  virtual async_task<bool>
  handle_new_rrc_handover_command(cu_cp_rrc_handover_command      command,
                                  std::optional<xnc_peer_index_t> xnc_index = std::nullopt) = 0;

  /// \brief Handles UE index allocation request for a handover target UE that the AMF of a GUAMI serves.
  /// \param[in] cgi The target cell of the handover.
  /// \param[in] guami The GUAMI of the AMF that serves the UE.
  /// \return The index of the new UE, or the cause to reject the handover with.
  virtual expected<cu_cp_ue_index_t, xnap_cause_t>
  handle_ue_index_allocation_request_for_guami(const nr_cell_global_id_t& cgi, const guami_t& guami) = 0;

  /// \brief Handles UE index allocation request for a handover target UE that a given AMF serves.
  /// \param[in] cgi The target cell of the handover.
  /// \param[in] plmn The PLMN of the UE.
  /// \param[in] amf_index The AMF that serves the UE.
  virtual cu_cp_ue_index_t handle_ue_index_allocation_request_for_amf(const nr_cell_global_id_t& cgi,
                                                                      const plmn_identity&       plmn,
                                                                      cu_cp_amf_index_t          amf_index) = 0;

  /// \brief Handle a received handover request.
  /// \param[in] ue_index Index of the UE.
  /// \param[in] sec_ctxt The received security context.
  /// \return True if the handover request was successfully handled, false otherwise.
  virtual bool handle_handover_request(cu_cp_ue_index_t ue_index, const security::security_context& sec_ctxt) = 0;

  /// \brief Handle the handover execution phase of the inter-CU handover at target gNB.
  /// \param[in] ue_index The index of the UE that is performing the handover.
  /// \param[in] xnap_ho_target_execution_ctxt If the handover is a Xn-C handover, the information required for the
  /// target handover execution is included.
  virtual void handle_inter_cu_target_handover_execution(
      cu_cp_ue_index_t                                             ue_index,
      const std::optional<xnap_handover_target_execution_context>& xnap_ho_target_execution_ctxt = std::nullopt) = 0;
};

/// Interface for the NGAP notifier to communicate with the CU-CP.
class cu_cp_ngap_handler : public cu_cp_ue_context_release_handler,
                           public cu_cp_task_scheduler_handler,
                           public cu_cp_inter_cu_handover_handler
{
public:
  virtual ~cu_cp_ngap_handler() = default;

  /// \brief Handle the creation of a new NGAP UE. This will add the NGAP adapters to the UE manager.
  /// \param[in] ue_index The index of the new NGAP UE.
  /// \returns Pointer to the NGAP UE notifier.
  virtual ngap_cu_cp_ue_notifier* handle_new_ngap_ue(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Handle the transfer of a UE association to the NGAP of the AMF that takes over the UE.
  /// \param[in] ran_ue_id The RAN UE NGAP ID of the UE association.
  /// \param[in] amf_index The index of the AMF that takes over the UE.
  /// \returns The index of the UE, or cu_cp_ue_index_t::invalid if no other NGAP holds the UE association.
  virtual cu_cp_ue_index_t handle_ue_association_transfer(ran_ue_id_t ran_ue_id, cu_cp_amf_index_t amf_index) = 0;

  /// \brief Handle the request of the source AMF to reroute the INITIAL UE MESSAGE of a UE to a target AMF of an AMF
  /// Set.
  /// \param[in] request The REROUTE NAS REQUEST of the source AMF.
  /// \param[in] amf_index The index of the source AMF.
  /// \returns True if the NGAP of the target AMF sent the rerouted INITIAL UE MESSAGE.
  virtual bool handle_reroute_nas_request(const ngap_reroute_nas_request& request, cu_cp_amf_index_t amf_index) = 0;

  /// \brief Handle the reception of a new Initial Context Setup Request.
  /// \param[in] request The received Initial Context Setup Request.
  /// \returns The Initial Context Setup Response or the Initial Context Setup Failure.
  virtual async_task<expected<ngap_init_context_setup_response, ngap_init_context_setup_failure>>
  handle_new_initial_context_setup_request(const ngap_init_context_setup_request& request) = 0;

  /// \brief Handle the reception of a new UE Context Modification Request.
  /// \param[in] request The received UE Context Modification Request.
  /// \returns The UE Context Modification Response or the UE Context Modification Failure.
  virtual async_task<expected<ngap_ue_context_modification_response, ngap_ue_context_modification_failure>>
  handle_new_ue_context_modification_request(const ngap_ue_context_modification_request& request) = 0;

  /// \brief Handle the reception of a new PDU Session Resource Setup Request.
  /// \param[in] request The received PDU Session Resource Setup Request.
  /// \returns The PDU Session Resource Setup Response.
  virtual async_task<ngap_pdu_session_resource_setup_response>
  handle_new_pdu_session_resource_setup_request(ngap_pdu_session_resource_setup_request& request) = 0;

  /// \brief Handle the reception of a new PDU Session Resource Modify Request.
  /// \param[in] request The received PDU Session Resource Modify Request.
  /// \returns The PDU Session Resource Modify Response.
  virtual async_task<ngap_pdu_session_resource_modify_response>
  handle_new_pdu_session_resource_modify_request(const ngap_pdu_session_resource_modify_request& request) = 0;

  /// \brief Handle the reception of a new PDU Session Resource Release Command.
  /// \param[in] command The received PDU Session Resource Release Command.
  /// \returns The PDU Session Resource Release Response.
  virtual async_task<ngap_pdu_session_resource_release_response>
  handle_new_pdu_session_resource_release_command(const ngap_pdu_session_resource_release_command& command) = 0;

  /// \brief Handle the handover request of the handover resource allocation procedure handover procedure.
  /// See TS 38.413 section 8.4.2.2.
  virtual async_task<cu_cp_handover_resource_allocation_response>
  handle_ngap_handover_request(const ngap_handover_request& request) = 0;

  /// \brief Handle the transmission of a handover required message to the AMF.
  virtual void handle_transmission_of_handover_required() = 0;

  /// \brief Handles a DL UE associated NRPPa transport.
  virtual void handle_dl_ue_associated_nrppa_transport_pdu(cu_cp_ue_index_t ue_index, const byte_buffer& nrppa_pdu) = 0;

  /// \brief Handles a DL non UE associated NRPPa transport.
  /// \param[in] amf_index The index of the AMF that received the NRPPa transport.
  /// \param[in] nrppa_pdu The NRPPa transport PDU.
  virtual void handle_dl_non_ue_associated_nrppa_transport_pdu(cu_cp_amf_index_t  amf_index,
                                                               const byte_buffer& nrppa_pdu) = 0;

  /// \brief Handles Location Reporting Control message.
  virtual void handle_location_reporting_control_message(cu_cp_ue_index_t               ue_index,
                                                         const location_report_request& msg) = 0;

  /// \brief Handle N2 AMF connection drop.
  /// \param[in] amf_index The index of the dropped AMF.
  virtual void handle_n2_disconnection(cu_cp_amf_index_t amf_index) = 0;

  /// \brief Handle WRITE-REPLACE WARNING REQUEST received from the AMF (TS 38.413 section 8.9.1).
  /// \param[in] request The decoded request.
  /// \returns Async task resolving to the WRITE-REPLACE WARNING RESPONSE.
  virtual async_task<ngap_write_replace_warning_response>
  handle_write_replace_warning_request(const ngap_write_replace_warning_request& request) = 0;
};

/// Interface for the NRPPa notifier to communicate with the CU-CP.
class cu_cp_nrppa_handler
{
public:
  virtual ~cu_cp_nrppa_handler() = default;

  /// \brief Handle the creation of a new NRPPA UE. This will add the NRPPA adapters to the UE manager.
  /// \param[in] ue_index The index of the new NRPPA UE.
  /// \returns Pointer to the NRPPA UE notifier.
  virtual nrppa_cu_cp_ue_notifier* handle_new_nrppa_ue(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Handle a UL NRPPa PDU.
  /// \param[in] msg The NRPPa PDU.
  /// \param[in] ue_or_amf_index The UE index for UE associated NRPPa messages or the AMF index for non UE associated
  virtual void handle_ul_nrppa_pdu(const byte_buffer&                                nrppa_pdu,
                                   std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index) = 0;

  /// \brief Handle a TRP information request.
  /// \param[in] request The TRP information request.
  /// \returns The TRP information CU-CP response.
  virtual async_task<trp_information_cu_cp_response_t>
  handle_trp_information_request(const trp_information_request_t& request) = 0;
};

/// Handler of E1AP-CU-CP events.
class cu_cp_e1ap_event_handler : public cu_cp_task_scheduler_handler
{
public:
  virtual ~cu_cp_e1ap_event_handler() = default;

  /// \brief Handle the reception of an Bearer Context Release Request message.
  /// \param[in] msg The received Bearer Context Release Request message.
  virtual void handle_bearer_context_release_request(const cu_cp_bearer_context_release_request& msg) = 0;

  /// \brief Handle the reception of an Bearer Context Inactivity Notification message.
  /// \param[in] msg The received Bearer Context Inactivity Notification message.
  virtual void handle_bearer_context_inactivity_notification(const e1ap_inactivity_notification& msg) = 0;

  /// \brief Handles the reception of a DL Data Notification message.
  /// \param[in] ue_index The index of the UE.
  virtual void handle_dl_data_notification(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Handle a UE release request.
  /// \param[in] request The release request.
  virtual async_task<void> handle_ue_context_release(const cu_cp_ue_context_release_request& request) = 0;

  /// \brief Handles the reception of an E1 Release Request message.
  /// \param[in] cu_up_index The index of the CU-UP processor.
  virtual void handle_e1_release_request(cu_cp_cu_up_index_t cu_up_index) = 0;

  /// \brief Handle transaction information loss in the E1AP.
  virtual async_task<void> handle_transaction_info_loss(const ue_transaction_info_loss_event& ev) = 0;
};

/// Interface used to handle DU specific procedures.
class cu_cp_du_event_handler
{
public:
  virtual ~cu_cp_du_event_handler() = default;

  /// \brief Handle the request of an RRC_INACTIVE UE to resume.
  /// \param[in] ue_index The index of the RRC_INACTIVE UE.
  /// \param[in] resume_cause The resume cause of the RRC Resume Request.
  /// \return True if the UE is accepted.
  virtual bool handle_ue_resume_admission_request(cu_cp_ue_index_t ue_index, resume_cause_t resume_cause) = 0;

  /// \brief Handle a RRC UE creation notification from the DU processor.
  /// \param[in] ue_index The index of the UE.
  /// \param[in] rrc_ue The interface of the created RRC UE.
  virtual void handle_rrc_ue_creation(cu_cp_ue_index_t ue_index, rrc_ue_interface& rrc_ue) = 0;

  /// \brief Handle the cells reported by a DU in the F1 Setup procedure.
  ///
  /// Realizes the corresponding logical cells and decides, per reported cell, whether the CU-CP activates
  /// it. Undeclared NCIs get a dynamic logical cell: unlocked when no cells were declared in configuration,
  /// locked otherwise (the declared set acts as the activation whitelist).
  /// \return NCIs of the reported cells to include in the F1 Setup Response Cells to be Activated List;
  /// reported cells absent from it stay dormant (admin-locked).
  virtual std::vector<nr_cell_identity> handle_du_cells_reported(cu_cp_du_index_t             du_index,
                                                                 span<const du_reported_cell> cells) = 0;

  /// \brief Handle the cells a DU stopped serving, de-realizing their logical cells while keeping operator
  /// intent.
  virtual void handle_du_cells_removed(span<const nr_cell_identity> cells) = 0;

  /// \brief Handle the addition of a DU, giving the CU-CP-wide components a route to its F1AP.
  virtual void handle_du_added(cu_cp_du_index_t du_index) = 0;

  /// \brief Handle the removal of a DU, de-realizing its logical cells while keeping operator intent.
  virtual void handle_du_removed(cu_cp_du_index_t du_index) = 0;

  /// \brief Handle a SIB1 request for a given cell.
  /// \param[in] du_index The index of the DU the cell is connected to.
  /// \param[in] cgi The cell global id of the cell.
  /// \returns The packed SIB1 for the cell, if available. An empty byte_buffer otherwise.
  virtual byte_buffer handle_target_cell_sib1_required(cu_cp_du_index_t du_index, nr_cell_global_id_t cgi) = 0;

  /// \brief Handle transaction information loss in the F1AP.
  virtual async_task<void> handle_transaction_info_loss(const ue_transaction_info_loss_event& ev) = 0;

  /// \brief Handle a change of the NR cells the connected DUs serve.
  virtual void handle_served_cells_updated() = 0;

  /// \brief Handle an E-CID measurement report of the gNB-DU, as per TS 38.473 section 8.13.14.
  virtual void handle_e_cid_measurement_report(const e_cid_measurement_report_t& report) = 0;

  /// \brief Handle an E-CID measurement failure indication of the gNB-DU, as per TS 38.473 section 8.13.13.
  virtual void handle_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind) = 0;
};

/// Interface for an RRC UE entity to communicate with the CU-CP.
class cu_cp_rrc_ue_interface
{
public:
  virtual ~cu_cp_rrc_ue_interface() = default;

  /// \brief Handles the RRC Setup Request of a new UE: decides which AMFs reject the UE because of an overload.
  /// \param[in] ue_index The index of the UE.
  /// \param[in] establishment_cause The establishment cause of the RRC Setup Request.
  /// \return True if an AMF accepts the UE, false otherwise.
  virtual bool handle_ue_setup_request(cu_cp_ue_index_t ue_index, establishment_cause_t establishment_cause) = 0;

  /// \brief Handles a new UE connection: sets the selected PLMN of the UE and selects the AMF of the UE.
  /// \param[in] ue_index The index of the UE.
  /// \param[in] request The selected PLMN of the UE and the identities that the AMF selection uses.
  /// \return True if the UE connection is accepted, false otherwise.
  virtual bool handle_ue_setup_complete_received(cu_cp_ue_index_t ue_index, const amf_selection_request& request) = 0;

  /// \brief Handle the reception of an RRC Reestablishment Request by transfering UE Contexts at the RRC.
  /// \param[in] old_pci The old PCI contained in the RRC Reestablishment Request.
  /// \param[in] old_c_rnti The old C-RNTI contained in the RRC Reestablishment Request.
  /// \param[in] ue_index The new UE index of the UE that sent the Reestablishment Request.
  /// \returns The RRC Reestablishment UE context for the old UE.
  virtual rrc_ue_reestablishment_context_response
  handle_rrc_reestablishment_request(pci_t old_pci, rnti_t old_c_rnti, cu_cp_ue_index_t ue_index) = 0;

  /// \brief Handle a required UE context retrieval from a peer NG-RAN node over Xn (TS 38.423 section 8.2.4).
  /// \param[in] ue_index The index of the UE the context is retrieved for.
  /// \param[in] request The retrieval request.
  /// \returns The retrieved context, or a failure if no peer serves the cell or the peer rejected the retrieval.
  virtual async_task<rrc_ue_context_retrieval_response>
  handle_ue_context_retrieval_required(cu_cp_ue_index_t ue_index, const rrc_ue_context_retrieval_request& request) = 0;

  /// \brief Handle a required reestablishment context modification.
  /// \param[in] ue_index The index of the UE that needs the context modification.
  virtual async_task<bool> handle_rrc_reestablishment_context_modification_required(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Move the user plane of a UE whose context was retrieved from a peer over to this node and release the
  /// context at the peer.
  /// \param[in] ue_index Index of the UE.
  /// \return True if the path was switched, false otherwise.
  virtual async_task<bool> handle_retrieved_context_path_switch_required(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Handle reestablishment failure by releasing the old UE.
  /// \param[in] request The release request.
  virtual void handle_rrc_reestablishment_failure(const cu_cp_ue_context_release_request& request) = 0;

  /// \brief Handle an successful reestablishment by removing the old UE.
  /// \param[in] ue_index The index of the old UE to remove.
  virtual void handle_rrc_reestablishment_complete(cu_cp_ue_index_t old_ue_index) = 0;

  /// \brief Transfer and remove UE contexts for an ongoing Reestablishment.
  /// \param[in] ue_index The new UE index of the UE that sent the Reestablishment Request.
  /// \param[in] old_ue_index The old UE index of the UE that sent the Reestablishment Request.
  virtual async_task<bool> handle_ue_context_transfer(cu_cp_ue_index_t ue_index, cu_cp_ue_index_t old_ue_index) = 0;

  /// \brief Handle a UE release request.
  /// \param[in] request The release request.
  virtual async_task<void> handle_ue_context_release(const cu_cp_ue_context_release_request& request) = 0;

  /// \brief Handle the reception of an RRC Resume Request.
  /// \param[in] request The resume request.
  /// \returns The RRC Resume Request response.
  virtual async_task<rrc_resume_request_response> handle_rrc_resume_request(const rrc_resume_request& request) = 0;

  /// \brief Initiate RAN paging for a UE in RRC Inactive state.
  virtual void handle_ran_paging_required(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Request the measurement gaps for the ongoing location measurements of a UE from the DU (TS 38.331 section
  /// 5.5.6).
  /// \param[in] ue_index The index of the UE.
  virtual void handle_location_measurement_indication(cu_cp_ue_index_t ue_index) = 0;
};

// Request with information for the target handler of the intra cu handover.
struct cu_cp_intra_cu_handover_target_request {
  cu_cp_ue_index_t                         target_ue_index = cu_cp_ue_index_t::invalid;
  cu_cp_ue_index_t                         source_ue_index = cu_cp_ue_index_t::invalid;
  uint8_t                                  transaction_id;
  std::chrono::milliseconds                timeout;
  e1ap_bearer_context_modification_request bearer_context_modification_request;
};

/// Request with information for the target handler of conditional handover.
struct cu_cp_cho_target_request {
  cu_cp_ue_index_t          source_ue_index = cu_cp_ue_index_t::invalid;
  cu_cp_ue_index_t          target_ue_index = cu_cp_ue_index_t::invalid;
  cond_recfg_id_t           cond_recfg_id   = cond_recfg_id_t(bounded_integer_invalid_tag{});
  pci_t                     target_pci      = INVALID_PCI;
  unsigned                  transaction_id  = 0;
  std::chrono::milliseconds timeout;

  /// \brief E1AP bearer context modification request for CU-UP tunnel update after CHO completion.
  e1ap_bearer_context_modification_request bearer_context_mod_request;
};

/// Interface for entities (e.g. DU processor) that wish to manipulate the context of a UE.
class cu_cp_ue_context_manipulation_handler
{
public:
  virtual ~cu_cp_ue_context_manipulation_handler() = default;

  /// \brief Handle a UE release request.
  /// \param[in] request The release request.
  virtual async_task<void> handle_ue_context_release(const cu_cp_ue_context_release_request& request) = 0;

  /// \brief Handle an access success indication (UE has accessed a CHO target cell).
  /// \param[in] msg The access success indication.
  virtual async_task<void> handle_access_success(const cu_cp_access_success_indication& msg) = 0;

  /// \brief Handle the trasmission of the handover reconfiguration by notifying the target RRC UE to await a RRC
  /// Reconfiguration Complete.
  /// \param[in] request The intra CU handover target request.
  virtual void handle_handover_reconfiguration_sent(const cu_cp_intra_cu_handover_target_request& request) = 0;

  /// \brief Handle the transmission of the CHO reconfiguration by notifying a CHO target UE to await
  /// RRCReconfigurationComplete.
  ///
  /// This starts the conditional_handover_target_routine on the specified target UE. CHO completion and cleanup
  /// are finalized on the source side (via Access Success handling / conditional_handover_source_routine).
  ///
  /// \param[in] request The CHO target request.
  virtual void handle_cho_reconfiguration_sent(const cu_cp_cho_target_request& request) = 0;

  /// \brief Handle a UE context push during handover.
  ///
  /// The push runs in the source UE task scheduler. It does not run if the source UE is removed first.
  ///
  /// \param[in] source_ue_index The index of the UE that is the source of the handover.
  /// \param[in] target_ue_index The index of the UE that is the target of the handover.
  /// \return True if the UE context was pushed to the target UE, false otherwise.
  virtual async_task<bool> handle_handover_ue_context_push(cu_cp_ue_index_t source_ue_index,
                                                           cu_cp_ue_index_t target_ue_index) = 0;

  /// \brief Handle the binding of an intra-CU handover target UE to the PLMN and the AMF of the source UE.
  /// \param[in] source_ue_index The index of the UE that is the source of the handover.
  /// \param[in] target_ue_index The index of the UE that is the target of the handover.
  /// \return True if the target UE is bound to the AMF of the source UE, false otherwise.
  virtual bool handle_handover_target_ue_amf_binding(cu_cp_ue_index_t source_ue_index,
                                                     cu_cp_ue_index_t target_ue_index) = 0;

  /// \brief Initialize a handover UE release timer. When the timeout is reached, a release request is sent to the AMF.
  /// \param[in] ue_index The index of the UE.
  /// \param[in] handover_ue_release_timeout The timeout for the release.
  /// \param[in] ue_context_release_request The release request.
  virtual void
  initialize_handover_ue_release_timer(cu_cp_ue_index_t                        ue_index,
                                       std::chrono::milliseconds               handover_ue_release_timeout,
                                       const cu_cp_ue_context_release_request& ue_context_release_request) = 0;

  /// \brief Initialize a RNA update timer. When the timeout is reached, a release request is sent to the AMF.
  /// \param[in] ue_index The index of the UE.
  virtual void initialize_rna_update_timer(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Start CHO execution cancellation timer. Fires conditional_handover_cancellation_routine on expiry.
  /// \param[in] source_ue_index Index of the source UE.
  /// \param[in] timeout Duration; 0ms disables the timer (no-op).
  virtual void initialize_cho_execution_timer(cu_cp_ue_index_t source_ue_index, std::chrono::milliseconds timeout) = 0;
};

/// Methods used by CU-CP to transfer the RRC UE context e.g. for RRC Reestablishments.
class cu_cp_rrc_ue_context_transfer_notifier
{
public:
  virtual ~cu_cp_rrc_ue_context_transfer_notifier() = default;

  /// \brief Notifies the RRC UE to return the RRC Reestablishment UE context.
  virtual rrc_ue_reestablishment_context_response on_rrc_ue_context_transfer() = 0;
};

/// Interface to handle measurement requests.
class cu_cp_measurement_handler
{
public:
  virtual ~cu_cp_measurement_handler() = default;

  /// \brief Handle a measurement config request (for any UE) connected to the given serving cell.
  /// \param[in] ue_index The index of the UE to update the measurement config for.
  /// \param[in] nci The cell id of the serving cell to update.
  /// \param[in] current_meas_config The current meas config of the UE (if applicable).
  /// \param[in] cond_meas True if this is a conditional measurement config request (e.g. CHO).
  /// \param[in] candidate_pcis List of candidate target PCIs (when cond_meas is true); if empty, use all neighbors.
  virtual std::optional<rrc_meas_cfg>
  handle_measurement_config_request(cu_cp_ue_index_t                   ue_index,
                                    nr_cell_identity                   nci,
                                    const std::optional<rrc_meas_cfg>& current_meas_config = std::nullopt,
                                    bool                               cond_meas           = false,
                                    span<const pci_t>                  candidate_pcis      = {}) = 0;

  /// \brief Handle a measurement report for given UE.
  virtual void handle_measurement_report(cu_cp_ue_index_t ue_index, const rrc_meas_results& meas_results) = 0;
};

/// Interface to handle measurement config update requests.
class cu_cp_measurement_config_handler
{
public:
  virtual ~cu_cp_measurement_config_handler() = default;

  /// \brief Handle a request to update the measurement related parameters for the given cell id.
  /// \param[in] nci The cell id of the serving cell to update.
  /// \param[in] serv_cell_cfg_ The serving cell meas config to update.
  virtual bool handle_cell_config_update_request(nr_cell_identity                nci,
                                                 const serving_cell_meas_config& serv_cell_cfg) = 0;
};

/// Interface to request handover.
class cu_cp_mobility_manager_handler
{
public:
  virtual ~cu_cp_mobility_manager_handler() = default;

  /// \brief Handle an Intra CU handover.
  virtual async_task<cu_cp_intra_cu_handover_response>
  handle_intra_cu_handover_request(const cu_cp_intra_cu_handover_request& request,
                                   cu_cp_du_index_t&                      source_du_index,
                                   cu_cp_du_index_t&                      target_du_index) = 0;

  /// \brief Handle full intra-CU CHO coordinator request (prepare + execute/cancel).
  virtual async_task<cu_cp_intra_cu_cho_response>
  handle_intra_cu_cho_request(const cu_cp_intra_cu_cho_request& request) = 0;

  /// \brief Handle a intra-cell handover required to refresh KgNB key for the UE
  /// \param[in] ue_index The index of the UE that needs the intra-cell handover
  virtual void handle_intra_cell_handover_required(cu_cp_ue_index_t ue_index) = 0;
};

/// Interface to handle location reporting updates for UEs.
class cu_cp_location_manager_handler
{
public:
  virtual ~cu_cp_location_manager_handler() = default;

  /// \brief Handle a UE location change and send NGAP Location Report if needed.
  /// \param[in] ue_index The index of the UE.
  virtual void handle_location_update(cu_cp_ue_index_t ue_index) = 0;
};

/// Interface to handle ue removals.
class cu_cp_ue_removal_handler
{
public:
  virtual ~cu_cp_ue_removal_handler() = default;

  /// \brief Completly remove a UE from the CU-CP.
  /// \param[in] ue_index The index of the UE to remove.
  virtual async_task<void> handle_ue_removal_request(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Cancel pending UE tasks.
  virtual void handle_pending_ue_task_cancellation(cu_cp_ue_index_t ue_index) = 0;
};

/// Interface to handle AMF reconnections.
class cu_cp_amf_reconnection_handler
{
public:
  virtual ~cu_cp_amf_reconnection_handler() = default;

  /// \brief Handle AMF reconnections.
  /// \param[in] amf_index The index of the AMF that reconnected.
  virtual void handle_amf_reconnection(cu_cp_amf_index_t amf_index) = 0;
};

/// \brief Handler of the XNAP of the CU-CP. This interface is used to forward XNAP messages to the CU-CP.
class cu_cp_xnap_handler : public cu_cp_inter_cu_handover_handler, public cu_cp_task_scheduler_handler
{
public:
  virtual ~cu_cp_xnap_handler() = default;

  /// \brief Handle the received XNAP handover request of the handover preparation procedure.
  /// See TS 38.423 section 8.2.1.
  virtual async_task<cu_cp_handover_resource_allocation_response>
  handle_xnap_handover_request(const xnap_handover_request& request) = 0;

  /// \brief Handle the transmission of a Handover Request for an immediate handover to the Xn-C peer.
  virtual void handle_transmission_of_xnap_handover_request() = 0;

  /// \brief Handle the reception of a Handover Cancel message.
  /// \param[in] ue_index The index of the UE that is the target of the handover cancel.
  virtual void handle_handover_cancel_received(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Handle the reception of a HandoverSuccess message (TS 38.423 section 8.2.4).
  /// Indicates that the source UE has successfully executed CHO to a remote target CU-CP.
  /// \param[in] source_ue_index The source UE index resolved from the XNAP UE ID mapping.
  /// \param[in] winner_cgi The cell the UE accessed, identifying the winning candidate.
  virtual void handle_xnap_handover_success_received(cu_cp_ue_index_t           source_ue_index,
                                                     const nr_cell_global_id_t& winner_cgi) = 0;

  /// \brief Handle the reception of an XNAP UE Context Release message.
  /// \param[in] ue_index The index of the UE to be released.
  virtual void handle_xnap_ue_context_release_received(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Handle a request for the NR cells this node serves, to be advertised to a peer NG-RAN node.
  /// \returns The cells served by the connected DUs.
  virtual std::vector<cu_cp_served_cell_info> handle_served_cells_required() = 0;
  /// \brief Resolve the UE identified by a UE Context ID received in a Retrieve UE Context Request.
  /// See TS 38.423 section 8.2.4.
  /// \param[in] ue_context_id The received UE Context ID.
  /// \returns The index of the UE holding the context, or cu_cp_ue_index_t::invalid if it is not known.
  virtual cu_cp_ue_index_t handle_xnap_ue_context_id_lookup(const xnap_ue_context_id& ue_context_id) = 0;

  /// \brief Handle the received Retrieve UE Context Request. See TS 38.423 section 8.2.4.
  /// \param[in] request The received request, with the UE index already resolved.
  /// \returns The UE context to transfer, or a failed response carrying the rejection cause.
  virtual async_task<xnap_retrieve_ue_context_response>
  handle_xnap_retrieve_ue_context_request(const xnap_retrieve_ue_context_request& request) = 0;
};

class cu_cp_impl_interface : public cu_cp_e1ap_event_handler,
                             public cu_cp_du_event_handler,
                             public cu_cp_rrc_ue_interface,
                             public cu_cp_measurement_handler,
                             public cu_cp_measurement_config_handler,
                             public cu_cp_ngap_handler,
                             public cu_cp_nrppa_handler,
                             public cu_cp_ue_context_manipulation_handler,
                             public cu_cp_mobility_manager_handler,
                             public cu_cp_location_manager_handler,
                             public cu_cp_ue_removal_handler,
                             public cu_cp_amf_reconnection_handler,
                             public cu_cp_xnap_handler
{
public:
  virtual ~cu_cp_impl_interface() = default;

  virtual cu_cp_e1ap_event_handler&              get_cu_cp_e1ap_handler()               = 0;
  virtual cu_cp_ngap_handler&                    get_cu_cp_ngap_handler()               = 0;
  virtual cu_cp_nrppa_handler&                   get_cu_cp_nrppa_handler()              = 0;
  virtual cu_cp_rrc_ue_interface&                get_cu_cp_rrc_ue_interface()           = 0;
  virtual cu_cp_ue_context_manipulation_handler& get_cu_cp_ue_context_handler()         = 0;
  virtual cu_cp_measurement_handler&             get_cu_cp_measurement_handler()        = 0;
  virtual cu_cp_measurement_config_handler&      get_cu_cp_measurement_config_handler() = 0;
  virtual cu_cp_mobility_manager_handler&        get_cu_cp_mobility_manager_handler()   = 0;
  virtual cu_cp_location_manager_handler&        get_cu_cp_location_manager_handler()   = 0;
  virtual cu_cp_ue_removal_handler&              get_cu_cp_ue_removal_handler()         = 0;
  virtual cu_cp_amf_reconnection_handler&        get_cu_cp_amf_reconnection_handler()   = 0;
  virtual cu_cp_xnap_handler&                    get_cu_cp_xnap_handler()               = 0;
};

} // namespace ocudu::ocucp
