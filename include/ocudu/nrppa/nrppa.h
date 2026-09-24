// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/ran/meas_types.h"
#include "ocudu/ran/positioning/e_cid_measurement.h"
#include "ocudu/ran/positioning/positioning_messages.h"
#include "ocudu/support/async/async_task.h"
#include <map>

namespace ocudu::ocucp {

struct cell_measurement_positioning_info {
  struct cell_measurement_item_t {
    nr_cell_global_id_t nr_cgi;
    arfcn_t             nr_arfcn;
    rrc_meas_result_nr  meas_result;
  };

  nr_cell_global_id_t                                 serving_cell_id;
  std::map<nr_cell_identity, cell_measurement_item_t> cell_measurements;
};

/// NRPPA notifier to the CU-CP UE
class nrppa_cu_cp_ue_notifier
{
public:
  virtual ~nrppa_cu_cp_ue_notifier() = default;

  /// \brief Get the UE index of the UE.
  virtual cu_cp_ue_index_t get_ue_index() const = 0;

  /// \brief Get the index of the DU where the UE is connected.
  virtual cu_cp_du_index_t get_du_index() const = 0;

  /// \brief Get the global identity of the cell serving the UE.
  virtual std::optional<nr_cell_global_id_t> get_serving_cell_id() const = 0;

  /// \brief Get the measurement results of the UE.
  virtual std::optional<cell_measurement_positioning_info>& on_measurement_results_required() = 0;

  /// \brief Schedule an async task for the UE.
  virtual bool schedule_async_task(async_task<void> task) = 0;
};

/// Methods used by NRPPa to signal events to the F1AP.
class nrppa_f1ap_notifier
{
public:
  virtual ~nrppa_f1ap_notifier() = default;

  /// \brief Notifies the F1AP about a positioning information request.
  /// \returns The outcome of the procedure.
  virtual async_task<expected<positioning_information_response_t, positioning_information_failure_t>>
  on_positioning_information_request(const positioning_information_request_t& request) = 0;

  /// \brief Notifies the F1AP about a positioning activation request.
  /// \returns The outcome of the procedure.
  virtual async_task<expected<positioning_activation_response_t, positioning_activation_failure_t>>
  on_positioning_activation_request(const positioning_activation_request_t& request) = 0;

  /// \brief Notifies the F1AP about a measurement information request.
  /// \returns The outcome of the procedure.
  virtual async_task<expected<measurement_response_t, measurement_failure_t>>
  on_measurement_information_request(const measurement_request_t& request) = 0;

  /// \brief Notifies the F1AP about an E-CID measurement initiation request.
  /// \returns The outcome of the procedure.
  virtual async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>
  on_e_cid_measurement_request(const e_cid_measurement_request_t& request) = 0;

  /// \brief Notifies the F1AP about an E-CID measurement termination command.
  virtual void on_e_cid_measurement_termination_command(const e_cid_measurement_termination_command_t& command) = 0;
};

// TRP information CU-CP response, containing information for all available TRPs at all DUs.
struct trp_information_cu_cp_response_t {
  std::map<cu_cp_du_index_t, trp_information_response_t> trp_info_responses;
};

/// Methods used by NRPPa to signal events to the CU-CP.
class nrppa_cu_cp_notifier
{
public:
  virtual ~nrppa_cu_cp_notifier() = default;

  /// \brief Notifies the CU-CP about a new NRPPA UE.
  /// \param[in] ue_index The index of the new NRPPA UE.
  /// \returns Pointer to the NRPPA UE notifier.
  virtual nrppa_cu_cp_ue_notifier* on_new_nrppa_ue(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Notifies about a NRPPa PDU.
  /// \param[in] nrppa_pdu The NRPPa PDU.
  /// \param[in] ue_or_amf_index The UE index for UE associated NRPPa messages or the AMF index for non UE associated
  virtual void on_ul_nrppa_pdu(const byte_buffer&                                nrppa_pdu,
                               std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index) = 0;

  /// \brief Notifies the CU-CP about a TRP information request.
  /// \param[in] request The TRP information request.
  /// \returns The TRP information CU-CP response.
  virtual async_task<trp_information_cu_cp_response_t>
  on_trp_information_request(const trp_information_request_t& request) = 0;
};

/// This interface is used to push NRPPA messages to the NRPPA interface.
class nrppa_message_handler
{
public:
  virtual ~nrppa_message_handler() = default;

  /// Handle the incoming NRPPA message.
  /// \param[in] nrppa_pdu The NRPPA message.
  /// \param[in] ue_or_amf_index The UE index for UE associated NRPPa messages or the AMF index for non UE associated
  /// NRPPa messages.
  virtual void handle_new_nrppa_pdu(const byte_buffer&                                nrppa_pdu,
                                    std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index) = 0;
};

/// Handle UE context removal and index updates.
class nrppa_ue_context_removal_handler
{
public:
  virtual ~nrppa_ue_context_removal_handler() = default;

  /// \brief Remove the context of an UE.
  /// \param[in] ue_index The index of the UE to remove.
  virtual void remove_ue_context(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Transfer the NRPPA context of a UE (if any) to a new UE index, e.g. following RRC re-establishment or
  /// handover. A no-op if the UE has no NRPPA context.
  /// \param[in] new_ue_index The new index of the UE.
  /// \param[in] old_ue_index The old index of the UE.
  /// \param[in] new_ue_notifier The notifier of the new UE.
  virtual void update_ue_index(cu_cp_ue_index_t         new_ue_index,
                               cu_cp_ue_index_t         old_ue_index,
                               nrppa_cu_cp_ue_notifier& new_ue_notifier) = 0;
};

/// Handler of the E-CID measurement events the gNB-DU reports.
class nrppa_e_cid_report_handler
{
public:
  virtual ~nrppa_e_cid_report_handler() = default;

  /// \brief Handle an E-CID measurement report of the gNB-DU, as per TS 38.473 section 8.13.14.
  /// \param[in] report The report of the gNB-DU.
  virtual void handle_e_cid_measurement_report(const e_cid_measurement_report_t& report) = 0;

  /// \brief Handle an E-CID measurement failure indication of the gNB-DU, as per TS 38.473 section 8.13.13.
  /// \param[in] ind The failure indication of the gNB-DU.
  virtual void handle_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind) = 0;
};

/// Handler of the DU lifecycle events NRPPa tracks.
class nrppa_du_context_handler
{
public:
  virtual ~nrppa_du_context_handler() = default;

  /// \brief Register a DU, giving NRPPa a route to its F1AP.
  /// \param[in] du_index The index of the DU.
  /// \param[in] f1ap_notifier The notifier used to send F1AP messages to the DU.
  virtual void handle_du_addition(cu_cp_du_index_t du_index, nrppa_f1ap_notifier& f1ap_notifier) = 0;

  /// \brief Drop the context of a DU together with the TRPs it hosts.
  /// \param[in] du_index The index of the DU to remove.
  virtual void handle_du_removal(cu_cp_du_index_t du_index) = 0;
};

/// Combined entry point for the NRPPA object.
class nrppa_interface
{
public:
  virtual ~nrppa_interface() = default;

  virtual nrppa_message_handler&            get_nrppa_message_handler()            = 0;
  virtual nrppa_ue_context_removal_handler& get_nrppa_ue_context_removal_handler() = 0;
  virtual nrppa_du_context_handler&         get_nrppa_du_context_handler()         = 0;
  virtual nrppa_e_cid_report_handler&       get_nrppa_e_cid_report_handler()       = 0;
};

} // namespace ocudu::ocucp
