// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "adapters/cell_meas_manager_adapters.h"
#include "adapters/du_processor_adapters.h"
#include "adapters/e1ap_adapters.h"
#include "adapters/mobility_manager_adapters.h"
#include "adapters/nrppa_adapters.h"
#include "cu_configurator_impl.h"
#include "cu_cp_controller/cu_cp_controller.h"
#include "cu_cp_impl_interface.h"
#include "cu_up_processor/cu_up_processor_repository.h"
#include "du_processor/du_processor_repository.h"
#include "logical_cell_controller.h"
#include "metrics_handler/metrics_handler_impl.h"
#include "ngap_repository.h"
#include "routines/mobility/inter_cu_handover_target_routine.h"
#include "ue_manager/ue_manager_impl.h"
#include "xnap_repository.h"
#include "ocudu/cu_cp/cu_configurator.h"
#include "ocudu/cu_cp/cu_cp.h"
#include "ocudu/cu_cp/cu_cp_cell_command_handler.h"
#include "ocudu/cu_cp/cu_cp_command_handler.h"
#include "ocudu/cu_cp/cu_cp_configuration.h"
#include "ocudu/cu_cp/cu_cp_ntn_ref_time_store.h"
#include "ocudu/nrppa/nrppa.h"
#include "ocudu/ran/inter_cu_handover_messages.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/support/async/async_task_scheduler.h"
#include <memory>

namespace ocudu {

class cu_cp_ntn_ref_time_store;

namespace ocudu_ntn {
class ntn_configuration_manager;
} // namespace ocudu_ntn

namespace ocucp {

class cu_cp_common_task_scheduler : public async_task_scheduler
{
public:
  cu_cp_common_task_scheduler() : main_ctrl_loop(128) {}

  bool schedule(async_task<void> task) override { return main_ctrl_loop.schedule(std::move(task)); }

private:
  // cu-cp task event loop
  fifo_async_task_scheduler main_ctrl_loop;
};

class cu_cp_impl final : public cu_cp,
                         public cu_cp_impl_interface,
                         public cu_cp_ng_handler,
                         public cu_cp_command_handler,
                         public cu_cp_ue_release_command_handler,
                         public cu_cp_ntn_meas_update_handler,
                         public cu_cp_mobility_config_handler,
                         public cu_cp_mobility_command_handler
{
public:
  cu_cp_impl(const cu_cp_configuration& config_, const cu_cp_dependencies& dependencies);
  ~cu_cp_impl() override;

  void start() override;
  void stop() override;

  // NGAP interface.
  ngap_message_handler* get_ngap_message_handler(cu_cp_amf_index_t amf_index) override;

  bool amfs_are_connected() override;

  // CU-UP handler.
  void handle_bearer_context_release_request(const cu_cp_bearer_context_release_request& msg) override;
  void handle_bearer_context_inactivity_notification(const e1ap_inactivity_notification& msg) override;
  void handle_dl_data_notification(cu_cp_ue_index_t ue_index) override;
  void handle_e1_release_request(cu_cp_cu_up_index_t cu_up_index) override;

  // cu_cp_rrc_ue_interface.
  bool handle_ue_setup_request(cu_cp_ue_index_t ue_index, establishment_cause_t establishment_cause) override;
  bool handle_ue_setup_complete_received(cu_cp_ue_index_t ue_index, const amf_selection_request& request) override;
  rrc_ue_reestablishment_context_response
  handle_rrc_reestablishment_request(pci_t old_pci, rnti_t old_c_rnti, cu_cp_ue_index_t ue_index) override;
  async_task<rrc_ue_context_retrieval_response>
                   handle_ue_context_retrieval_required(cu_cp_ue_index_t                        ue_index,
                                                        const rrc_ue_context_retrieval_request& request) override;
  async_task<bool> handle_rrc_reestablishment_context_modification_required(cu_cp_ue_index_t ue_index) override;
  async_task<bool> handle_retrieved_context_path_switch_required(cu_cp_ue_index_t ue_index) override;

  void             handle_rrc_reestablishment_failure(const cu_cp_ue_context_release_request& request) override;
  void             handle_rrc_reestablishment_complete(cu_cp_ue_index_t old_ue_index) override;
  async_task<bool> handle_ue_context_transfer(cu_cp_ue_index_t ue_index, cu_cp_ue_index_t old_ue_index) override;
  async_task<void> handle_ue_context_release(const cu_cp_ue_context_release_request& request) override;
  async_task<void> handle_access_success(const cu_cp_access_success_indication& msg) override;
  async_task<rrc_resume_request_response> handle_rrc_resume_request(const rrc_resume_request& request) override;
  void                                    handle_ran_paging_required(cu_cp_ue_index_t ue_index) override;
  void                                    handle_location_measurement_indication(cu_cp_ue_index_t ue_index) override;

  // cu_cp_ue_context_manipulation_handler.
  void             handle_handover_reconfiguration_sent(const cu_cp_intra_cu_handover_target_request& request) override;
  void             handle_cho_reconfiguration_sent(const cu_cp_cho_target_request& request) override;
  async_task<bool> handle_handover_ue_context_push(cu_cp_ue_index_t source_ue_index,
                                                   cu_cp_ue_index_t target_ue_index) override;
  bool             handle_handover_target_ue_amf_binding(cu_cp_ue_index_t source_ue_index,
                                                         cu_cp_ue_index_t target_ue_index) override;
  void
       initialize_handover_ue_release_timer(cu_cp_ue_index_t                        ue_index,
                                            std::chrono::milliseconds               handover_ue_release_timeout,
                                            const cu_cp_ue_context_release_request& ue_context_release_request) override;
  void initialize_rna_update_timer(cu_cp_ue_index_t ue_index) override;
  void initialize_cho_execution_timer(cu_cp_ue_index_t source_ue_index, std::chrono::milliseconds timeout) override;

  // cu_cp_location_manager_handler.
  void handle_location_update(cu_cp_ue_index_t ue_index) override;

  // cu_cp_ngap_handler.
  async_task<expected<ngap_init_context_setup_response, ngap_init_context_setup_failure>>
  handle_new_initial_context_setup_request(const ngap_init_context_setup_request& request) override;
  async_task<expected<ngap_ue_context_modification_response, ngap_ue_context_modification_failure>>
  handle_new_ue_context_modification_request(const ngap_ue_context_modification_request& request) override;
  async_task<ngap_pdu_session_resource_setup_response>
  handle_new_pdu_session_resource_setup_request(ngap_pdu_session_resource_setup_request& request) override;
  async_task<ngap_pdu_session_resource_modify_response>
  handle_new_pdu_session_resource_modify_request(const ngap_pdu_session_resource_modify_request& request) override;
  async_task<ngap_pdu_session_resource_release_response>
  handle_new_pdu_session_resource_release_command(const ngap_pdu_session_resource_release_command& command) override;
  async_task<cu_cp_ue_context_release_complete>
  handle_ue_context_release_command(const cu_cp_ue_context_release_command& command) override;
  async_task<cu_cp_handover_resource_allocation_response>
       handle_ngap_handover_request(const ngap_handover_request& request) override;
  void handle_transmission_of_handover_required() override;
  void handle_dl_ue_associated_nrppa_transport_pdu(cu_cp_ue_index_t ue_index, const byte_buffer& nrppa_pdu) override;
  void handle_dl_non_ue_associated_nrppa_transport_pdu(cu_cp_amf_index_t  amf_index,
                                                       const byte_buffer& nrppa_pdu) override;
  void handle_location_reporting_control_message(cu_cp_ue_index_t               ue_index,
                                                 const location_report_request& msg) override;
  void handle_n2_disconnection(cu_cp_amf_index_t amf_index) override;
  async_task<ngap_write_replace_warning_response>
  handle_write_replace_warning_request(const ngap_write_replace_warning_request& request) override;

  // cu_cp_inter_cu_handover_handler.
  async_task<bool>                         handle_new_rrc_handover_command(cu_cp_rrc_handover_command      command,
                                                                           std::optional<xnc_peer_index_t> xnc_index = std::nullopt) override;
  expected<cu_cp_ue_index_t, xnap_cause_t> handle_ue_index_allocation_request_for_guami(const nr_cell_global_id_t& cgi,
                                                                                        const guami_t& guami) override;
  cu_cp_ue_index_t                         handle_ue_index_allocation_request_for_amf(const nr_cell_global_id_t& cgi,
                                                                                      const plmn_identity&       plmn,
                                                                                      cu_cp_amf_index_t          amf_index) override;
  bool handle_handover_request(cu_cp_ue_index_t ue_index, const security::security_context& sec_ctxt) override;
  void handle_inter_cu_target_handover_execution(cu_cp_ue_index_t ue_index,
                                                 const std::optional<xnap_handover_target_execution_context>&
                                                     xnap_ho_target_execution_ctxt = std::nullopt) override;

  // cu_cp_xnap_handler.
  async_task<cu_cp_handover_resource_allocation_response>
                                      handle_xnap_handover_request(const xnap_handover_request& request) override;
  void                                handle_transmission_of_xnap_handover_request() override;
  void                                handle_handover_cancel_received(cu_cp_ue_index_t ue_index) override;
  void                                handle_xnap_handover_success_received(cu_cp_ue_index_t           source_ue_index,
                                                                            const nr_cell_global_id_t& winner_cgi) override;
  void                                handle_xnap_ue_context_release_received(cu_cp_ue_index_t ue_index) override;
  std::vector<cu_cp_served_cell_info> handle_served_cells_required() override;
  cu_cp_ue_index_t handle_xnap_ue_context_id_lookup(const xnap_ue_context_id& ue_context_id) override;
  async_task<xnap_retrieve_ue_context_response>
  handle_xnap_retrieve_ue_context_request(const xnap_retrieve_ue_context_request& request) override;

  // cu_cp_nrppa_handler.
  nrppa_cu_cp_ue_notifier* handle_new_nrppa_ue(cu_cp_ue_index_t ue_index) override;
  void                     handle_ul_nrppa_pdu(const byte_buffer&                                nrppa_pdu,
                                               std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index) override;
  async_task<trp_information_cu_cp_response_t>
  handle_trp_information_request(const trp_information_request_t& request) override;

  // cu_cp_measurement_handler.
  std::optional<rrc_meas_cfg>
       handle_measurement_config_request(cu_cp_ue_index_t                   ue_index,
                                         nr_cell_identity                   nci,
                                         const std::optional<rrc_meas_cfg>& current_meas_config = std::nullopt,
                                         bool                               cond_meas           = false,
                                         span<const pci_t>                  candidate_pcis      = {}) override;
  void handle_measurement_report(cu_cp_ue_index_t ue_index, const rrc_meas_results& meas_results) override;

  // cu_cp_measurement_config_handler.
  bool handle_cell_config_update_request(nr_cell_identity nci, const serving_cell_meas_config& serv_cell_cfg) override;

  // cu_cp_mobility_manager_handler.
  async_task<cu_cp_intra_cu_handover_response>
  handle_intra_cu_handover_request(const cu_cp_intra_cu_handover_request& request,
                                   cu_cp_du_index_t&                      source_du_index,
                                   cu_cp_du_index_t&                      target_du_index) override;

  async_task<cu_cp_intra_cu_cho_response>
  handle_intra_cu_cho_request(const cu_cp_intra_cu_cho_request& request) override;

  void handle_intra_cell_handover_required(cu_cp_ue_index_t ue_index) override;

  // cu_cp_ue_removal_handler.
  async_task<void> handle_ue_removal_request(cu_cp_ue_index_t ue_index) override;
  void             handle_pending_ue_task_cancellation(cu_cp_ue_index_t ue_index) override;

  // cu_cp_ue_release_command_handler.
  void trigger_release(pci_t                                         source_pci,
                       rnti_t                                        rnti,
                       std::optional<cu_cp_release_redirect_nr_info> redirect_info = std::nullopt) override;

  // cu_cp_ntn_meas_update_handler.
  void update_ntn_neighbour_info(nr_cell_identity                              serving_nci,
                                 std::vector<rrc_ntn_neighbour_cell_info_item> ncells) override;

  cu_cp_mobility_command_handler&   get_mobility_command_handler() override { return *this; }
  cu_cp_ue_release_command_handler& get_ue_release_command_handler() override { return *this; }
  cu_cp_ntn_meas_update_handler&    get_ntn_meas_update_handler() override { return *this; }
  cu_cp_cell_command_handler&       get_cell_command_handler() override { return cell_ctrl; }
  cu_cp_mobility_config_handler&    get_mobility_config_handler() override { return *this; }
  metrics_handler&                  get_metrics_handler() override { return metrics_hdlr; }

  // cu_cp_mobility_command_handler.
  bool trigger_handover(pci_t         source_pci,
                        rnti_t        rnti,
                        pci_t         target_pci,
                        plmn_identity target_plmn,
                        tac_t         target_tac) override;
  bool trigger_conditional_handover(pci_t                                                source_pci,
                                    rnti_t                                               rnti,
                                    span<const pci_t>                                    target_pcis,
                                    std::chrono::milliseconds                            timeout,
                                    std::optional<std::chrono::system_clock::time_point> t1_thres_override) override;

  // cu_cp_mobility_config_handler.
  bool update_mobility_cell(const serving_cell_meas_config& cell_cfg) override;
  bool remove_mobility_cell(nr_cell_identity nci) override;
  bool update_neighbor(nr_cell_identity             serving_nci,
                       nr_cell_identity             neighbor_nci,
                       std::vector<report_cfg_id_t> report_cfg_ids) override;
  bool remove_neighbor(nr_cell_identity serving_nci, nr_cell_identity neighbor_nci) override;
  bool update_report_config(report_cfg_id_t report_cfg_id, const rrc_report_cfg_nr& report_cfg) override;
  bool remove_report_config(report_cfg_id_t report_cfg_id) override;
  bool set_periodic_report(nr_cell_identity nci, std::optional<report_cfg_id_t> report_cfg_id) override;

  // cu_cp_amf_reconnection_handler.
  void handle_amf_reconnection(cu_cp_amf_index_t amf_index) override;

  // cu_cp public interface.
  cu_cp_f1c_handler&                     get_f1c_handler() override { return controller.get_f1c_handler(); }
  cu_cp_e1_handler&                      get_e1_handler() override { return controller.get_e1_handler(); }
  cu_cp_xnc_handler&                     get_xnc_handler() override { return controller.get_xnc_handler(); }
  cu_cp_e1ap_event_handler&              get_cu_cp_e1ap_handler() override { return *this; }
  cu_cp_ng_handler&                      get_ng_handler() override { return *this; }
  cu_cp_ngap_handler&                    get_cu_cp_ngap_handler() override { return *this; }
  cu_cp_nrppa_handler&                   get_cu_cp_nrppa_handler() override { return *this; }
  cu_cp_command_handler&                 get_command_handler() override { return *this; }
  cu_cp_rrc_ue_interface&                get_cu_cp_rrc_ue_interface() override { return *this; }
  cu_cp_measurement_handler&             get_cu_cp_measurement_handler() override { return *this; }
  cu_cp_measurement_config_handler&      get_cu_cp_measurement_config_handler() override { return *this; }
  cu_cp_mobility_manager_handler&        get_cu_cp_mobility_manager_handler() override { return *this; }
  cu_cp_location_manager_handler&        get_cu_cp_location_manager_handler() override { return *this; }
  cu_cp_ue_removal_handler&              get_cu_cp_ue_removal_handler() override { return *this; }
  cu_cp_ue_context_manipulation_handler& get_cu_cp_ue_context_handler() override { return *this; }
  cu_cp_amf_reconnection_handler&        get_cu_cp_amf_reconnection_handler() override { return *this; }
  cu_cp_xnap_handler&                    get_cu_cp_xnap_handler() override { return *this; }
  cu_configurator&                       get_cu_configurator() override { return cu_cp_cfgtr; }

private:
  // Handling of DU events.
  bool handle_ue_resume_admission_request(cu_cp_ue_index_t ue_index, resume_cause_t resume_cause) override;
  void handle_rrc_ue_creation(cu_cp_ue_index_t ue_index, rrc_ue_interface& rrc_ue) override;

  byte_buffer handle_target_cell_sib1_required(cu_cp_du_index_t du_index, nr_cell_global_id_t cgi) override;

  std::vector<nr_cell_identity> handle_du_cells_reported(cu_cp_du_index_t             du_index,
                                                         span<const du_reported_cell> cells) override;
  void                          handle_du_cells_removed(span<const nr_cell_identity> cells) override;

  void handle_du_added(cu_cp_du_index_t du_index) override;

  void handle_du_removed(cu_cp_du_index_t du_index) override;

  async_task<void> handle_transaction_info_loss(const ue_transaction_info_loss_event& ev) override;

  void handle_served_cells_updated() override;

  void handle_e_cid_measurement_report(const e_cid_measurement_report_t& report) override;

  void handle_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind) override;

  // NGAP UE creation handler.
  ngap_cu_cp_ue_notifier* handle_new_ngap_ue(cu_cp_ue_index_t ue_index) override;
  cu_cp_ue_index_t        handle_ue_association_transfer(ran_ue_id_t ran_ue_id, cu_cp_amf_index_t amf_index) override;
  bool handle_reroute_nas_request(const ngap_reroute_nas_request& request, cu_cp_amf_index_t amf_index) override;

  // cu_cp_task_scheduler_handler.
  bool schedule_ue_task(cu_cp_ue_index_t ue_index, async_task<void> task) override;

  void request_ue_release(cu_cp_ue& ue, const ngap_cause_t& cause);

  /// \brief Initiate RAN paging of an inactive UE.
  /// \param[in] ue_index The index of the UE to page.
  /// \param[in] full_i_rnti Full I-RNTI of the UE to page.
  /// message.
  void send_ran_paging(cu_cp_ue_index_t ue_index, full_i_rnti_t full_i_rnti);

  /// \brief Binds a UE to the AMF that serves it, and connects the RRC UE to the NGAP of this AMF.
  /// \returns False if no NGAP exists for the AMF.
  bool bind_ue_to_amf(cu_cp_ue_index_t ue_index, cu_cp_amf_index_t amf_index);

  /// \brief Binds a UE with a context retrieved from a peer to the AMF that the peer reported.
  /// \returns False if the CU-CP is not connected to this AMF or to its AMF Set.
  bool bind_retrieved_ue_to_amf(cu_cp_ue_index_t ue_index);

  /// \brief Creates a UE in the target cell of a handover.
  /// \returns The index of the new UE, or cu_cp_ue_index_t::invalid if the UE cannot be served.
  cu_cp_ue_index_t add_handover_target_ue(const nr_cell_global_id_t& cgi);

  /// \brief Request the release of an inactive UE that failed to resume within the expected time.
  /// \param[in] ue_index The index of the UE to release.
  void request_release_of_inactive_ue(cu_cp_ue_index_t ue_index);

  /// \brief Handle the reception of an inter-CU handover request message from the source CU.
  /// \param[in] request The inter-CU handover request message.
  /// \returns An asynchronous task that will complete with the response to the inter-CU handover request. The response
  /// indicates whether the handover preparation was successful and, if not, the cause of the failure.
  async_task<cu_cp_handover_resource_allocation_response>
  handle_inter_cu_handover_request(const cu_cp_inter_cu_handover_request& request);

  void on_statistics_report_timer_expired();

  cu_cp_configuration cfg;

  std::vector<xnc_connection_gateway*> xnc_gws;

  // Logger.
  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("CU-CP");

  task_executor&                 cu_cp_executor;
  timer_manager&                 timers;
  cu_cp_metrics_report_notifier* metrics_notifier = nullptr;

  // Components.
  // UE manager.
  ue_manager ue_mng;

  // Controller of the CU-CP's logical cells (operator intent + realization + cell commands). Declared
  // before du_db: DU teardown is forwarded here, so the controller must outlive the repository. Its
  // constructor only stores the du_db/scheduler references, so binding them before their construction is
  // safe.
  logical_cell_controller cell_ctrl;

  // Cell measurement manager.
  cell_meas_manager cell_meas_mng;

  cu_cp_common_task_scheduler common_task_sched;

  // DU repository to Node Manager adapter.
  du_processor_cu_cp_connection_adapter conn_notifier;

  // Cell Measurement Manager to mobility manager adapters.
  cell_meas_mobility_manager_adapter cell_meas_mobility_notifier;

  // E1AP to CU-CP adapter.
  e1ap_cu_cp_adapter e1ap_ev_notifier;

  // Mobility manager to CU-CP adapter.
  mobility_manager_adapter mobility_manager_ev_notifier;

  // NTN neighbour cell measurement info handling.
  cu_cp_ntn_ref_time_store ntn_ref_time_store;

  // DU connections being managed by the CU-CP.
  du_processor_repository du_db;

  // CU-UP connections being managed by the CU-CP.
  cu_up_processor_repository cu_up_db;

  // NRPPa to CU-CP adapter.
  nrppa_cu_cp_adapter nrppa_cu_cp_ev_notifier;

  // NRPPa to F1AP adapter.
  std::map<cu_cp_du_index_t, nrppa_f1ap_adapter> nrppa_f1ap_ev_notifiers;

  // NRPPA entity.
  std::unique_ptr<nrppa_interface> nrppa_entity;

  // Handler of paging messages.
  paging_message_handler paging_handler;

  // AMF connections beeing managed by the CU-CP.
  ngap_repository ngap_db;

  // XNAP connections beeing managed by the CU-CP.
  xnap_repository xnap_db;

  // Mobility manager.
  mobility_manager mobility_mng;

  // Handler of the CU-CP connections to other remote nodes (e.g. AMF, CU-UPs, DUs).
  cu_cp_controller controller;

  metrics_handler_impl metrics_hdlr;

  unique_timer statistics_report_timer;

  std::atomic<bool> stopped{false};

  cu_configurator_impl cu_cp_cfgtr;

  // Metrics report session for the lifetime of the CU-CP.
  // Used, e.g., for logging metrics and JSON metrics.
  std::unique_ptr<metrics_report_session> metrics_session;

  // Periodically refreshes the NTN neighbour cell info of the measurement configuration. References this CU-CP's
  // command handler and the reference time store, so it is declared last and destroyed first.
  std::unique_ptr<ocudu_ntn::ntn_configuration_manager> ntn_config_manager;
};

} // namespace ocucp

} // namespace ocudu
