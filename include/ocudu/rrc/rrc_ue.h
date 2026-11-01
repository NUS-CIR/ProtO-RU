// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/byte_buffer.h"
#include "ocudu/adt/span.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/cu_cp/cu_cp_ue_messages.h"
#include "ocudu/ran/amf_selection_request.h"
#include "ocudu/ran/cause/common.h"
#include "ocudu/ran/cause/ngap_cause.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/ran/cu_cp_ue_context_release.h"
#include "ocudu/ran/i_rnti.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/ran/rb_id.h"
#include "ocudu/ran/rnti.h"
#include "ocudu/rrc/rrc_cell_context.h"
#include "ocudu/rrc/rrc_resume.h"
#include "ocudu/rrc/rrc_types.h"
#include "ocudu/rrc/rrc_ue_capabilities.h"
#include "ocudu/rrc/rrc_ue_config.h"
#include "ocudu/security/security.h"
#include "ocudu/support/async/async_task.h"
#include "ocudu/support/async/protocol_transaction_manager.h"
#include <chrono>
#include <variant>

namespace asn1::rrc_nr {

// ASN.1 forward declarations
struct dl_ccch_msg_s;
struct dl_dcch_msg_s;

} // namespace asn1::rrc_nr

namespace ocudu::ocucp {

/// RRC states (3GPP 38.331 v15.5.1 Sec 4.2.1)
enum class rrc_state { idle = 0, connected, inactive };

class rrc_ue_controller
{
public:
  virtual ~rrc_ue_controller() = default;

  /// \brief Cancel currently running transactions.
  virtual void stop() = 0;

  /// \brief Trigger an asynchronous UE release with the given cause.
  virtual void on_ue_release_required(const ngap_cause_t& cause) = 0;
};

enum ue_context_release_cause : uint16_t {
  radio_network = 0,
  transport     = 1,
  protocol      = 2,
  misc          = 3,
  choice_ext    = 4,
  nulltype      = 5
};

/// This interface represents the data entry point for the RRC UE receiving UL PDUs on the CCCH and DCCH logical
/// channel. The lower-layers will use this class to pass PDUs into the RRC.
class rrc_ul_pdu_handler
{
public:
  virtual ~rrc_ul_pdu_handler() = default;

  /// Handle the incoming PDU on the UL-CCCH logical channel.
  virtual void handle_ul_ccch_pdu(byte_buffer pdu, rnti_t c_rnti) = 0;

  /// Handle the incoming SRB PDCP PDU on the UL-DCCH logical channel.
  virtual void handle_ul_dcch_pdu(srb_id_t srb_id, byte_buffer rrc_pdu, bool integrity_verified) = 0;
};

/// This interface represents the data entry point for the RRC receiving NAS and control messages from the NGAP.
/// The higher-layers will use this class to pass PDUs into the RRC.
class rrc_ngap_message_handler
{
public:
  virtual ~rrc_ngap_message_handler() = default;

  /// \brief Handle the received Downlink NAS Transport message.
  /// \param[in] nas_pdu The received NAS PDU.
  virtual void handle_dl_nas_transport_message(byte_buffer nas_pdu) = 0;

  /// \brief Get the packed UE Radio Access Cap Info.
  /// \returns The packed UE Radio Access Cap Info.
  virtual byte_buffer get_packed_ue_radio_access_cap_info() const = 0;

  /// \brief Get the packed Handover Preparation Message.
  virtual byte_buffer get_packed_handover_preparation_message() = 0;
};

/// Interface to notify F1AP about a new SRB PDU.
class rrc_pdu_f1ap_notifier
{
public:
  virtual ~rrc_pdu_f1ap_notifier() = default;

  /// \brief Notify the PDCP about a new RRC PDU that needs ciphering and integrity protection.
  /// \param[in] pdu The RRC PDU.
  /// \param[in] srb_id The SRB ID of the PDU.
  virtual void on_new_rrc_pdu(srb_id_t srb_id, const byte_buffer& pdu) = 0;
};

/// Interface used by the RRC Setup procedure to notify the RRC UE.
class rrc_ue_setup_proc_notifier
{
public:
  virtual ~rrc_ue_setup_proc_notifier() = default;

  /// \brief Notify about a DL CCCH message.
  /// \param[in] dl_ccch_msg The DL CCCH message.
  virtual void on_new_dl_ccch(const asn1::rrc_nr::dl_ccch_msg_s& dl_ccch_msg) = 0;

  /// \brief Notify about the need to release a UE.
  virtual void on_ue_release_required(const ngap_cause_t& cause) = 0;
};

struct srb_creation_message {
  cu_cp_ue_index_t ue_index        = cu_cp_ue_index_t::invalid;
  cu_cp_ue_index_t old_ue_index    = cu_cp_ue_index_t::invalid;
  srb_id_t         srb_id          = srb_id_t::nulltype;
  bool             enable_security = false; // Activate security upon SRB creation.
  srb_pdcp_config  pdcp_cfg;
};

/// Interface used by the RRC reconfiguration procedure to
/// invoke actions carried out by the main RRC UE class (i.e. send DL message, remove UE).
class rrc_ue_reconfiguration_proc_notifier
{
public:
  rrc_ue_reconfiguration_proc_notifier()          = default;
  virtual ~rrc_ue_reconfiguration_proc_notifier() = default;

  /// \brief Notify about a DL DCCH message.
  /// \param[in] dl_dcch_msg The DL DCCH message.
  virtual void on_new_dl_dcch(srb_id_t srb_id, const asn1::rrc_nr::dl_dcch_msg_s& dl_dcch_msg) = 0;

  /// \brief Notify about the need to release a UE.
  virtual void on_ue_release_required(const ngap_cause_t& cause) = 0;
};

/// Interface used by the RRC security mode procedure
/// to notify the RRC UE of the security mode context update.
class rrc_ue_security_mode_command_proc_notifier
{
public:
  rrc_ue_security_mode_command_proc_notifier()          = default;
  virtual ~rrc_ue_security_mode_command_proc_notifier() = default;

  /// \brief Notify about a DL DCCH message.
  /// \param[in] dl_dcch_msg The DL DCCH message.
  virtual void on_new_dl_dcch(srb_id_t srb_id, const asn1::rrc_nr::dl_dcch_msg_s& dl_dcch_msg) = 0;
};

/// Interface used by the RRC reestablishment and RRC resume procedure to
/// invoke actions carried out by the main RRC UE class (i.e. send DL message, remove UE).
class rrc_ue_msg4_proc_notifier
{
public:
  rrc_ue_msg4_proc_notifier()          = default;
  virtual ~rrc_ue_msg4_proc_notifier() = default;

  /// \brief Notify about a DL DCCH message.
  /// \param[in] dl_dcch_msg The DL DCCH message.
  virtual void on_new_dl_dcch(srb_id_t srb_id, const asn1::rrc_nr::dl_dcch_msg_s& dl_dcch_msg) = 0;

  /// \brief Refresh AS security keys after horizontal key derivation.
  ///
  /// This includes configuring the PDCP entity security on SRB1 with the new AS keys.
  /// If \p security_mode_active is \c true, the SRB1 will be configured with strict integrity check. Otherwise the
  /// PDCP RX of SRB1 will be configured in SMC transition mode to also allow reception of in-flight PDUs without
  /// integrity protection. The strict integrity check is activated later upon reception of security mode complete.
  ///
  /// \param[in] security_mode_present Indicates whether the UE is already in security mode or not (yet).
  virtual void on_new_as_security_context(bool security_mode_active) = 0;

  /// \brief Notify that AS security is active on a UE the procedure brought back on a new RRC UE.
  ///
  /// Resume and re-establishment activate security without a Security Mode Command, so this is what tells the RRC UE
  /// that anything gated on AS security may now run.
  virtual void on_as_security_activated() = 0;

  /// \brief Send a NAS PDU from the UE to the AMF.
  /// \param[in] nas_pdu The NAS PDU.
  virtual void on_ul_nas_pdu(byte_buffer nas_pdu) = 0;
};

/// Interface to notify about NGAP messages.
class rrc_ue_ngap_notifier
{
public:
  virtual ~rrc_ue_ngap_notifier() = default;

  /// \brief Notify about the Initial UE Message.
  /// \param[in] msg The initial UE message.
  virtual void on_initial_ue_message(const cu_cp_initial_ue_message& msg) = 0;

  /// \brief Notify about an Uplink NAS Transport message.
  /// \param[in] msg The Uplink NAS Transport message.
  /// \returns True if the message was forwarded successfully, false otherwise.
  virtual bool on_ul_nas_transport_message(const cu_cp_ul_nas_transport& msg) = 0;
};

struct rrc_reconfiguration_response_message {
  cu_cp_ue_index_t ue_index = cu_cp_ue_index_t::invalid;
  bool             success  = false;
};

struct rrc_ue_security_mode_command_context {
  unsigned            transaction_id;
  nr_cell_global_id_t sp_cell_id;
  byte_buffer         rrc_ue_security_mode_command_pdu;
};

struct rrc_ue_release_context {
  cu_cp_user_location_info_nr user_location_info;
  byte_buffer                 rrc_pdu;
  srb_id_t                    srb_id = srb_id_t::nulltype;
};

struct rrc_ue_handover_reconfiguration_context {
  unsigned    transaction_id;
  byte_buffer rrc_ue_handover_reconfiguration_pdu;
};

struct rrc_ue_cond_reconfiguration_context {
  unsigned    transaction_id;
  byte_buffer rrc_ue_cond_reconfiguration_pdu; // PDCP-protected outer RRC Reconfiguration
};

struct rrc_inactivity_context {
  i_rntis_t                        i_rntis;
  uint8_t                          next_hop_chaining_count;
  uint16_t                         ran_paging_cycle;
  rrc_ran_notification_area_info_t ran_notification_area_info;
  std::chrono::minutes             t380 = std::chrono::minutes(5);
};

/// Handle control messages.
class rrc_ue_control_message_handler
{
public:
  virtual ~rrc_ue_control_message_handler() = default;

  /// \brief Get the packed Security Mode Command.
  /// \returns The Security Mode Command context.
  virtual rrc_ue_security_mode_command_context get_security_mode_command_context() = 0;

  /// \brief Await a RRC Security Mode Complete.
  /// \param[in] transaction_id The transaction ID of the RRC Security Mode Complete.
  /// \returns True if the RRC Security Mode Complete was received, false otherwise.
  virtual async_task<bool> handle_security_mode_complete_expected(uint8_t transaction_id) = 0;

  /// \brief Get the packed UE Capability RAT Container List.
  /// \returns The packed UE Capability RAT Container List.
  virtual byte_buffer get_packed_ue_capability_rat_container_list() const = 0;

  /// \brief Verify a ShortMAC-I this UE computed for a reestablishment attempt at another NG-RAN node.
  /// The token is computed with the AS keys of this UE, so only this node can verify it
  /// (TS 38.331 section 5.3.7.4, TS 33.501 section 6.11).
  /// \param[in] short_mac_i The ShortMAC-I received from the peer NG-RAN node.
  /// \param[in] source_pci PCI of the cell the UE declared a failure on.
  /// \param[in] source_c_rnti C-RNTI the UE had in that cell.
  /// \param[in] target_nci Identity of the cell the UE is reestablishing on.
  /// \returns True if the ShortMAC-I matches, false otherwise.
  virtual bool verify_reestablishment_short_mac_i(const security::sec_short_mac_i& short_mac_i,
                                                  pci_t                            source_pci,
                                                  rnti_t                           source_c_rnti,
                                                  nr_cell_identity                 target_nci) = 0;

  /// \brief Verify a ResumeMAC-I this UE computed for a resume attempt at another NG-RAN node.
  /// The token is computed with the AS keys of this UE, so only this node can verify it
  /// (TS 38.331 section 5.3.13.3, TS 33.501 section 6.11). The peer identifies the UE by its I-RNTI, so the source
  /// cell and C-RNTI are taken from the context of the cell this UE was suspended in.
  /// \param[in] resume_mac_i The ResumeMAC-I received from the peer NG-RAN node.
  /// \param[in] target_nci Identity of the cell the UE is resuming on.
  /// \returns True if the ResumeMAC-I matches, false otherwise.
  virtual bool verify_resume_mac_i(const security::sec_short_mac_i& resume_mac_i, nr_cell_identity target_nci) = 0;

  /// \brief Handle an RRC Reconfiguration Request.
  /// \param[in] msg The new RRC Reconfiguration Request.
  /// \returns The result of the rrc reconfiguration.
  virtual async_task<bool> handle_rrc_reconfiguration_request(const rrc_reconfiguration_procedure_request& msg) = 0;

  /// \brief Get the RRC Reconfiguration context for a handover.
  /// \param[in] msg The new RRC Reconfiguration Request.
  /// \returns The RRC handover reconfiguration context.
  virtual rrc_ue_handover_reconfiguration_context
  get_rrc_ue_handover_reconfiguration_context(const rrc_reconfiguration_procedure_request& msg) = 0;

  /// \brief Get the RRC UE conditional reconfiguration context (conditionalReconfiguration).
  /// \param[in] request The RRC reconfiguration request containing CHO candidates.
  /// \returns The conditional reconfiguration context with transaction ID and PDCP-protected PDU.
  virtual rrc_ue_cond_reconfiguration_context
  get_rrc_ue_cond_reconfiguration_context(const rrc_reconfiguration_procedure_request& request) = 0;

  /// \brief Await a RRC Reconfiguration Complete for a handover.
  ///
  /// A UE that is not in connected state enters connected state on reception and counts as a new RRC connection.
  /// \param[in] transaction_id The transaction ID of the RRC Reconfiguration Complete.
  /// \param[in] timeout_ms The timeout for the RRC Reconfiguration Complete.
  /// \param[in] release_on_failure If true, trigger a UE release when the transaction fails.
  /// \returns True if the RRC Reconfiguration Complete was received, false otherwise.
  virtual async_task<bool> handle_handover_reconfiguration_complete_expected(uint8_t                   transaction_id,
                                                                             std::chrono::milliseconds timeout_ms,
                                                                             bool release_on_failure = true) = 0;

  /// \brief Store UE capabilities received from the NGAP.
  /// \param[in] ue_capabilities The UE capabilities.
  /// \returns True if the UE capabilities were stored successfully, false otherwise.
  virtual bool store_ue_capabilities(byte_buffer ue_capabilities) = 0;

  /// \brief Initiate the UE capability transfer procedure.
  virtual async_task<bool> handle_rrc_ue_capability_transfer_request(const rrc_ue_capability_transfer_request& msg) = 0;

  /// \brief Fills what this gNB derived from the coarse position the UE reported, TS 38.300 sec. 16.14.5.
  virtual void fill_ue_derived_location(cu_cp_user_location_info_nr& user_location_info) const = 0;

  /// \brief Asks the UE for its coarse location, if the serving cell is one whose location is worth asking for.
  ///
  /// TS 38.331 sec. 5.7.10.2 allows the request from AS security onwards, leaving the caller to pick the point.
  virtual void request_coarse_ue_location() = 0;

  /// \brief Get the RRC UE release context.
  /// \returns The release context of the UE. If SRB1 is not created yet, a RrcReject message is contained in the
  /// release context, see section 5.3.15 in TS 38.331. Otherwise, a RrcRelease message is contained in the release
  /// context.
  virtual rrc_ue_release_context
  get_rrc_ue_release_context(bool                                          requires_rrc_msg,
                             std::optional<std::chrono::seconds>           release_wait_time  = std::nullopt,
                             std::optional<rrc_inactivity_context>         inactivity_context = std::nullopt,
                             std::optional<cu_cp_release_redirect_nr_info> redirect_nr_info   = std::nullopt) = 0;

  /// \brief Retrieve RRC context of a UE to perform mobility (handover, reestablishment).
  /// \return Transfer context including UP context, security, SRBs, HO preparation, etc.
  virtual rrc_ue_transfer_context get_transfer_context() = 0;

  /// \brief Get the RRC measurement config for the current serving cell of the UE.
  /// \param[in] current_meas_config The current meas config of the UE (if applicable).
  /// \param[in] cond_meas True if this is a conditional measurement config request (e.g. CHO).
  /// \param[in] candidate_pcis List of candidate target PCIs (when cond_meas is true); if empty, use all neighbors.
  /// \return The measurement config, if present.
  virtual std::optional<rrc_meas_cfg>
  generate_meas_config(const std::optional<rrc_meas_cfg>& current_meas_config = std::nullopt,
                       bool                               cond_meas           = false,
                       span<const pci_t>                  candidate_pcis      = {}) = 0;

  /// \brief Get the packed RRC MeasConfig IE for the UE.
  ///
  /// When called with an empty (default) candidate_pcis, returns the regular serving-cell
  /// measurement config and updates the stored context. When called with a non-empty
  /// candidate_pcis, returns the CHO-specific config filtered to those candidates without
  /// touching the stored context.
  virtual byte_buffer get_packed_meas_config(span<const pci_t> candidate_pcis = {}) = 0;

  /// \brief Update the stored measurement config to reflect a config that has been applied at the UE.
  ///
  /// Call this after the UE acknowledges an RRCReconfiguration that carried a measConfig (e.g. the
  /// outer CHO RRCReconfiguration), so that context.meas_cfg stays in sync with VarMeasConfig.
  virtual void update_meas_config(const rrc_meas_cfg& cfg) = 0;

  /// \brief Get the measurement config currently stored for the UE, if any.
  virtual std::optional<rrc_meas_cfg> get_meas_config() = 0;

  /// \brief Get the packed LocationMeasurementInfo of the ongoing location measurements of the UE (TS 38.331 section
  /// 5.5.6), if any.
  virtual std::optional<byte_buffer> get_packed_location_meas_info() const = 0;

  /// \brief Get the serving cell measurement object for the current serving cell of the UE.
  virtual std::optional<uint8_t> get_serving_cell_mo() = 0;

  /// \brief Handle the handover command RRC PDU.
  /// \param[in] cmd The handover command RRC PDU.
  /// \returns The handover RRC Reconfiguration PDU. If the handover command is invalid, the PDU is empty.
  virtual byte_buffer handle_rrc_handover_command(byte_buffer cmd) = 0;

  /// \brief Handle the handover preparation info RRC PDU.
  /// \param[in] pdu The handover preparation info RRC PDU.
  /// \returns True if the handover preparation info was successfully handled, false otherwise.
  virtual bool handle_rrc_handover_preparation_info(byte_buffer pdu) = 0;

  /// \brief Get the packed RRC Handover Command.
  /// \param[in] msg The new RRC Reconfiguration Request.
  /// \returns The RRC Handover Command.
  virtual byte_buffer get_rrc_handover_command(const rrc_reconfiguration_procedure_request& request,
                                               unsigned                                     transaction_id) = 0;

  /// \brief Get the packed RRC Handover Preparation Message.
  virtual byte_buffer get_packed_handover_preparation_message() = 0;

  /// \brief Instruct the RRC UE to create a new SRB. It creates all
  /// required intermediate objects (e.g. PDCP) and connects them with one another.
  /// \param[in] msg The UE index, SRB ID and config.
  virtual void create_srb(const srb_creation_message& msg) = 0;

  /// \brief Get all SRBs of the UE.
  virtual static_vector<srb_id_t, MAX_NOF_SRBS> get_srbs() = 0;

  /// \brief Set the RRC connection state of the UE.
  /// \param[in] state The new RRC state.
  virtual void set_rrc_state(rrc_state state) = 0;

  /// \brief Get the RRC connection state of the UE.
  virtual rrc_state get_rrc_state() const = 0;

  /// \brief Cancel an ongoing handover reconfiguration transaction.
  /// \param[in] transaction_id The transaction ID of the handover reconfiguration transaction.
  virtual void cancel_handover_reconfiguration_transaction(uint8_t transaction_id) = 0;

  /// \brief Cancel all ongoing transactions.
  /// \param[in] cause Reason recorded for the cancellation. Defaults to \c cancel when no input is given.
  virtual void cancel_all_transactions(protocol_transaction_failure cause = protocol_transaction_failure::cancel) = 0;
};

class rrc_ue_cu_cp_ue_notifier
{
public:
  virtual ~rrc_ue_cu_cp_ue_notifier() = default;

  /// \brief Get the timer factory for the UE.
  virtual timer_factory get_timer_factory() = 0;

  /// \brief Get the task executor for the UE.
  virtual task_executor& get_executor() = 0;

  /// \brief Schedule an async task for the UE.
  virtual bool schedule_async_task(async_task<void> task) = 0;

  /// \brief Get the AS configuration for the RRC domain
  virtual security::sec_as_config get_rrc_as_config() = 0;

  /// \brief Get the AS configuration for the RRC domain with 128-bit keys
  virtual security::sec_128_as_config get_rrc_128_as_config() = 0;

  /// \brief Get the current security context
  virtual security::security_context get_security_context() = 0;

  /// \brief Get the selected security algorithms
  virtual security::sec_selected_algos get_security_algos() = 0;

  /// \brief Update the security context
  /// \param[in] sec_ctxt The new security context
  virtual void update_security_context(const security::security_context& sec_ctxt) = 0;

  /// \brief Initialize the security context from one retrieved from a peer NG-RAN node. A context coming from a peer
  /// carries KgNB*, so this takes over the algorithms the peer selected and derives the AS keys below it.
  /// \param[in] sec_ctxt The security context retrieved from the peer.
  /// \param[in] algos The AS algorithms the peer signalled, or std::nullopt to select them locally.
  /// \return True on success, false if no algorithm could be selected.
  virtual bool init_retrieved_security_context(const security::security_context&                  sec_ctxt,
                                               const std::optional<security::sec_selected_algos>& algos) = 0;

  /// \brief Perform horizontal key derivation
  virtual void perform_horizontal_key_derivation(pci_t target_pci, unsigned target_ssb_arfcn) = 0;
};

/// Struct containing all information needed from the old RRC UE for Reestablishment.
struct rrc_ue_reestablishment_context_response {
  cu_cp_ue_index_t                               ue_index = cu_cp_ue_index_t::invalid;
  security::security_context                     sec_context;
  std::optional<rrc_ue_cap_rat_container_list_t> capabilities_list;
  up_context                                     up_ctx;
  bool                                           old_ue_fully_attached   = false;
  bool                                           reestablishment_ongoing = false;
  /// Packed LocationMeasurementInfo of the ongoing location measurements of the UE (TS 38.331 section 5.5.6).
  std::optional<byte_buffer> location_meas_info;
};

/// \brief Identity of a UE that reestablished at this node, as it appeared in the RRCReestablishmentRequest. The peer
/// holding the context resolves the UE from it.
struct rrc_ue_context_retrieval_id_for_reest {
  /// PCI of the cell the UE declared the failure on, i.e. a cell served by the peer holding the context.
  pci_t old_pci = INVALID_PCI;
  /// C-RNTI the UE had in that cell.
  rnti_t old_c_rnti = rnti_t::INVALID_RNTI;
};

/// \brief Identity of a UE that resumed at this node, as it appeared in the RRCResumeRequest. The peer holding the
/// context resolves the UE from the I-RNTI it allocated when it suspended the UE.
struct rrc_ue_context_retrieval_id_for_resume {
  /// I-RNTI the UE included in the RRCResumeRequest. The I-RNTI types have no default constructor, so the variant is
  /// explicitly initialized to keep the identity default-constructible.
  std::variant<short_i_rnti_t, full_i_rnti_t> i_rnti = short_i_rnti_t{short_i_rnti_profile::profile_0, 0, 0};
  /// C-RNTI this node allocated for the resuming UE.
  rnti_t allocated_c_rnti = rnti_t::INVALID_RNTI;
  /// PCI of the cell the UE accessed at this node.
  pci_t access_pci = INVALID_PCI;
};

/// \brief Identity the peer resolves the UE by, following the procedure the UE used to reach this node.
using rrc_ue_context_retrieval_id =
    std::variant<rrc_ue_context_retrieval_id_for_reest, rrc_ue_context_retrieval_id_for_resume>;

/// \brief Request to retrieve a UE context from the peer NG-RAN node that still holds it (TS 38.423 section 8.2.4),
/// for a UE that reestablished or resumed at this node after leaving a cell served by that peer.
struct rrc_ue_context_retrieval_request {
  /// Identity of the UE at the peer.
  rrc_ue_context_retrieval_id ue_id;
  /// ShortMAC-I (reestablishment, TS 38.331 section 5.3.7.4) or ResumeMAC-I (resume, section 5.3.13.3) the UE computed
  /// with the AS keys it had at the peer. Only the peer can verify it, as only the peer holds those keys.
  security::sec_short_mac_i mac_i = {};
  /// Identity of the cell the UE accessed at this node, as the UE used it in VarShortMAC-Input/VarResumeMAC-Input. The
  /// peer verifies the MAC-I against exactly this value and derives KgNB* for this cell.
  nr_cell_identity target_nci = nr_cell_identity::min();
  /// How long to wait for the peer before giving up. The wait happens before anything is sent to the UE, so it is
  /// spent out of the timer the UE runs until Msg4 -- T301 for a reestablishment, T319 for a resume -- and must leave
  /// room for the RRC Setup fallback to still reach the UE. Defaults to the guard the Handover Preparation procedure
  /// uses, for the case where that timer is not known.
  std::chrono::milliseconds max_response_time{1000};
};

/// \brief Result of a UE context retrieval from a peer NG-RAN node.
struct rrc_ue_context_retrieval_response {
  bool success = false;
  /// Security context the peer derived for the cell the UE accessed here (KgNB*, TS 33.501 section 6.11). This is
  /// already the key for this node's cell, so no further horizontal key derivation must be performed on it.
  security::security_context sec_context;
  /// Packed RRC HandoverPreparationInformation, carrying the UE capabilities and the source AS configuration.
  byte_buffer rrc_context;
};

/// Interface to notify about UE context updates.
class rrc_ue_context_update_notifier
{
public:
  virtual ~rrc_ue_context_update_notifier() = default;

  /// \brief Notifies that a new RRC UE needs to be setup.
  /// \param[in] establishment_cause The establishment cause of the RRC Setup Request.
  /// \return True if the UE is accepted.
  virtual bool on_ue_setup_request(establishment_cause_t establishment_cause) = 0;

  /// \brief Notifies that the RRC UE setup complete is received.
  /// \param[in] request The selected PLMN of the UE and the identities that the AMF selection uses.
  /// \return True if the UE setup complete is accepted.
  virtual bool on_ue_setup_complete_received(const amf_selection_request& request) = 0;

  /// \brief Notify about the reception of an RRC Reestablishment Request.
  /// \param[in] old_pci The old PCI contained in the RRC Reestablishment Request.
  /// \param[in] old_c_rnti The old C-RNTI contained in the RRC Reestablishment Request.
  /// \param[in] ue_index The new UE index of the UE that sent the Reestablishment Request.
  /// \returns The RRC Reestablishment UE context for the old UE.
  virtual rrc_ue_reestablishment_context_response on_rrc_reestablishment_request(pci_t old_pci, rnti_t old_c_rnti) = 0;

  /// \brief Notify the CU-CP to retrieve the UE context from the peer NG-RAN node that still holds it, over Xn
  /// (TS 38.423 section 8.2.4). Used when no local UE context matches the reestablishment identity, but a peer serves
  /// the cell the UE declared the failure on.
  /// \param[in] request The retrieval request.
  /// \returns The retrieved context, or a failure if no peer serves that cell or the peer rejected the retrieval.
  virtual async_task<rrc_ue_context_retrieval_response>
  on_ue_context_retrieval_required(const rrc_ue_context_retrieval_request& request) = 0;

  /// \brief Notify about a required reestablishment context modification.
  virtual async_task<bool> on_rrc_reestablishment_context_modification_required() = 0;

  /// \brief Notify that a UE whose context was retrieved from a peer has confirmed Msg4, so the user plane can be
  /// moved to this node and the context released at the peer.
  /// \return True if the path was switched, false otherwise.
  virtual async_task<bool> on_retrieved_context_path_switch_required() = 0;

  /// \brief Notify the CU-CP to release the old UE after a reestablishment failure.
  /// \param[in] request The release request.
  virtual void on_rrc_reestablishment_failure(const cu_cp_ue_context_release_request& request) = 0;

  /// \brief Notify the CU-CP to remove the old UE from the CU-CP after an successful reestablishment.
  /// \param[in] old_ue_index The index of the old UE to remove.
  virtual void on_rrc_reestablishment_complete(cu_cp_ue_index_t old_ue_index) = 0;

  /// \brief Notify the CU-CP to transfer and remove ue contexts.
  /// \param[in] old_ue_index The old UE index of the UE that sent the Reestablishment Request.
  virtual async_task<bool> on_ue_transfer_required(cu_cp_ue_index_t old_ue_index) = 0;

  /// \brief Notify the CU-CP to release a UE.
  /// \param[in] request The release request.
  virtual async_task<void> on_ue_release_required(const cu_cp_ue_context_release_request& request) = 0;

  /// \brief Notify the CU-CP to setup an UP context.
  /// \param[in] ctxt The UP context to setup.
  virtual void on_up_context_setup_required(const up_context& ctxt) = 0;

  /// \brief Notifies the CU-CP that the location the UE reports has changed, so that a Location Report is sent when
  /// the AMF configured location reporting.
  virtual void on_ue_location_update() = 0;

  /// \brief Notifies the CU-CP that the UE starts or stops location measurements that need measurement gaps (TS 38.331
  /// section 5.5.6).
  virtual void on_location_measurement_indication() = 0;

  /// \brief Get the UP context of the UE.
  /// \returns The UP context of the UE.
  virtual up_context on_up_context_required() = 0;

  /// \brief Notify the CU-CP to remove a UE from the CU-CP.
  virtual async_task<void> on_ue_removal_required() = 0;

  /// \brief Notify the CU-CP about the reception of an RRC Resume Request.
  /// \param[in] request The resume request.
  /// \returns The RRC Resume Request response.
  virtual async_task<rrc_resume_request_response> on_rrc_resume_request(const rrc_resume_request& request) = 0;

  /// \brief Notify the CU-CP that a RAN paging for a UE in RRC Inactive state is required.
  virtual void on_ran_paging_required() = 0;
};

/// Interface to notify about measurements
class rrc_ue_measurement_notifier
{
public:
  virtual ~rrc_ue_measurement_notifier() = default;

  /// \brief Retrieve the measurement config (for any UE) connected to the given serving cell.
  /// \param[in] nci The cell id of the serving cell to update.
  /// \param[in] current_meas_config The current meas config of the UE (if applicable).
  /// \param[in] cond_meas True if this is a conditional measurement config request (e.g. CHO).
  /// \param[in] candidate_pcis List of candidate target PCIs (when cond_meas is true); if empty, use all neighbors.
  virtual std::optional<rrc_meas_cfg>
  on_measurement_config_request(nr_cell_identity                   nci,
                                const std::optional<rrc_meas_cfg>& current_meas_config = std::nullopt,
                                bool                               cond_meas           = false,
                                span<const pci_t>                  candidate_pcis      = {}) = 0;

  /// \brief Submit measurement report for given UE to cell manager.
  virtual void on_measurement_report(const rrc_meas_results& meas_results) = 0;
};

class rrc_ue_context_handler
{
public:
  virtual ~rrc_ue_context_handler() = default;

  /// \brief Get the RRC Reestablishment UE context to transfer it to new UE.
  /// \returns The RRC Reestablishment UE Context.
  virtual rrc_ue_reestablishment_context_response get_context() = 0;

  /// \brief Update the C-RNTI of the RRC UE, e.g. for RRC resume.
  /// \param[in] crnti The new C-RNTI of the RRC UE.
  virtual void update_c_rnti(rnti_t crnti) = 0;

  /// \brief Get the cell context of the RRC UE.
  /// \returns The cell context.
  virtual rrc_cell_context get_cell_context() const = 0;

  /// \brief Update the packed RRC cell group config.
  /// \param[in] cell_group_config The new packed RRC cell group config.
  virtual void update_cell_group_config(byte_buffer cell_group_config) = 0;

  /// \brief Get the packed RRC cell group config.
  virtual byte_buffer& get_cell_group_config() = 0;
};

/// Handler for UE capabilities.
class rrc_ue_capability_handler
{
public:
  virtual ~rrc_ue_capability_handler() = default;

  /// \brief Check if RRC Inactive is supported by the UE.
  virtual bool is_rrc_inactive_supported() const = 0;

  /// \brief Check if Conditional Handover (Rel-16) is supported by the UE.
  virtual bool is_conditional_handover_supported() const = 0;

  /// \brief Check if CHO with two trigger events (Rel-16) is supported by the UE.
  virtual bool is_conditional_handover_two_trigger_events_supported() const = 0;

  /// \brief Check if CHO with event-A4-based trigger (Rel-17) is supported by the UE.
  virtual bool is_conditional_handover_event_a4_supported() const = 0;

  /// \brief Check if location-based CHO (Rel-17) is supported by the UE.
  virtual bool is_conditional_handover_location_based_supported() const = 0;

  /// \brief Check if time-based CHO (Rel-17) is supported by the UE.
  virtual bool is_conditional_handover_time_based_supported() const = 0;
};

class rrc_ue_event_notifier
{
public:
  virtual ~rrc_ue_event_notifier() = default;

  /// \brief Notify the RRC DU about a new RRC connection.
  virtual void on_new_rrc_connection() = 0;

  /// \brief Notify the RRC DU about a successful RRC connection establishment.
  /// \param[in] cause The establishment cause of the RRC connection.
  /// \param[in] setup_time The time from the reception of the RRCSetupRequest to the reception of the RRCSetupComplete.
  virtual void on_successful_rrc_connection_establishment(establishment_cause_t     cause,
                                                          std::chrono::milliseconds setup_time) = 0;

  /// \brief Notify the RRC DU about a failed RRC connection establishment.
  virtual void on_failed_rrc_connection_establishment(establishment_fail_cause_t cause) = 0;

  /// \brief Notify the RRC DU about the successful RRC connection re-establishment.
  virtual void on_successful_rrc_connection_reestablishment() = 0;

  /// \brief Notify the RRC DU about the successful RRC connection re-establishment fallback.
  virtual void on_successful_rrc_connection_reestablishment_fallback() = 0;

  /// \brief Notify the RRC DU about the RRC connection re-establishment attempt that falls back to RRC Setup.
  virtual void on_attempted_rrc_connection_reestablishment_followed_by_rrc_setup() = 0;

  /// \brief Notify the RRC DU about the successful RRC connection resume.
  virtual void on_successful_rrc_connection_resume(resume_cause_t cause) = 0;

  /// \brief Notify the RRC DU about the successful RRC connection resume with fallback.
  virtual void on_successful_rrc_connection_resume_with_fallback(resume_cause_t cause) = 0;

  /// \brief Notify the RRC DU about the RRC connection resume followed by network release.
  virtual void on_rrc_connection_resume_followed_by_network_release(resume_cause_t cause) = 0;

  /// \brief Notify the RRC DU about the attempted RRC connection resume followed by RRC setup.
  virtual void on_attempted_rrc_connection_resume_followed_by_rrc_setup(resume_cause_t cause) = 0;
};

/// Result of a PDCP TX (encryption) operation on an RRC PDU.
struct pdcp_tx_result {
  std::variant<byte_buffer, ngap_cause_t> result;

  bool         is_successful() const { return std::holds_alternative<byte_buffer>(result); }
  ngap_cause_t get_failure_cause() const { return std::get<ngap_cause_t>(result); }
  byte_buffer  pop_pdu() { return std::move(std::get<byte_buffer>(result)); }
};

/// Per-SRB PDCP notifier.
class rrc_ue_pdcp_notifier
{
public:
  virtual ~rrc_ue_pdcp_notifier() = default;

  /// Pass a plaintext RRC PDU to PDCP for downlink transmission on this SRB.
  virtual pdcp_tx_result on_new_pdu(byte_buffer pdu) = 0;

  virtual void enable_tx_security(security::integrity_enabled int_enabled,
                                  security::ciphering_enabled ciph_enabled,
                                  security::sec_128_as_config sec_cfg) = 0;

  virtual void enable_rx_security(security::integrity_enabled int_enabled,
                                  security::ciphering_enabled ciph_enabled,
                                  security::sec_128_as_config sec_cfg) = 0;

  virtual void reestablish(security::sec_128_as_config sec_cfg) = 0;
};

/// Per-UE interface to create and track SRB PDCP entities.
class rrc_ue_srb_pdcp_manager
{
public:
  virtual ~rrc_ue_srb_pdcp_manager() = default;

  /// Create a PDCP entity for the given SRB and return its SRB notifier. Security is configured separately.
  virtual rrc_ue_pdcp_notifier& create_srb(srb_id_t srb_id) = 0;

  virtual bool has_srb(srb_id_t srb_id) const = 0;

  virtual static_vector<srb_id_t, MAX_NOF_SRBS> get_srb_ids() const = 0;
};

/// Combined entry point for the RRC UE handling.
/// It will contain getters for the interfaces for the various logical channels handled by RRC.
class rrc_ue_interface : public rrc_ul_pdu_handler,
                         public rrc_ngap_message_handler,
                         public rrc_ue_control_message_handler,
                         public rrc_ue_setup_proc_notifier,
                         public rrc_ue_security_mode_command_proc_notifier,
                         public rrc_ue_reconfiguration_proc_notifier,
                         public rrc_ue_context_handler,
                         public rrc_ue_msg4_proc_notifier,
                         public rrc_ue_capability_handler
{
public:
  rrc_ue_interface()          = default;
  virtual ~rrc_ue_interface() = default;

  virtual rrc_ue_controller&              get_controller()                     = 0;
  virtual rrc_ul_pdu_handler&             get_ul_pdu_handler()                 = 0;
  virtual rrc_ngap_message_handler&       get_rrc_ngap_message_handler()       = 0;
  virtual rrc_ue_control_message_handler& get_rrc_ue_control_message_handler() = 0;
  virtual rrc_ue_context_handler&         get_rrc_ue_context_handler()         = 0;
  virtual rrc_ue_capability_handler&      get_rrc_ue_capability_handler()      = 0;
};

} // namespace ocudu::ocucp
