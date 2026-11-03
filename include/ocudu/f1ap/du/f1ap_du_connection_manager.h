// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/byte_buffer.h"
#include "ocudu/f1ap/common/interface_management.h"
#include "ocudu/ran/carrier_configuration.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/duplex_mode.h"
#include "ocudu/ran/gnb_du_id.h"
#include "ocudu/ran/nr_cgi.h"
#include "ocudu/ran/pci.h"
#include "ocudu/ran/s_nssai.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include "ocudu/ran/tac.h"
#include "ocudu/support/async/async_task.h"
#include <chrono>
#include <optional>

namespace ocudu {
namespace odu {

struct f1_reset_request {
  enum class cause_type { cell_removal, other };

  /// UEs to reset. If empty, all UEs will be reset.
  std::vector<du_ue_index_t> ues_reset;
  /// Cause for the F1 Reset.
  cause_type cause;
};

struct f1_reset_acknowledgement {
  bool success = true;
};

/// System Information Update from the gNB-DU.
struct gnb_du_sys_info {
  byte_buffer              packed_mib;
  byte_buffer              packed_sib1;
  std::vector<byte_buffer> packed_si_msgs;
};

/// Information of served cell being added/modified.
struct du_served_cell_info {
  nr_cell_global_id_t                  nr_cgi;
  pci_t                                pci;
  tac_t                                tac;
  duplex_mode                          duplx_mode;
  subcarrier_spacing                   scs_common;
  carrier_configuration                dl_carrier;
  std::optional<carrier_configuration> ul_carrier;
  byte_buffer                          packed_meas_time_cfg;
  std::chrono::milliseconds            ntn_link_rtt = std::chrono::milliseconds(0);
};

/// \brief Served cell configuration that will be passed to CU-CP.
struct f1_cell_setup_params {
  // DU-internal identifier of the cell.
  du_cell_index_t        cell_index;
  du_served_cell_info    cell_info;
  gnb_du_sys_info        du_sys_info;
  std::vector<s_nssai_t> slices;
};

/// \brief Message that initiates a F1 Setup procedure.
struct f1_setup_request_message {
  gnb_du_id_t                       gnb_du_id;
  std::string                       gnb_du_name;
  uint8_t                           rrc_version;
  std::vector<f1_cell_setup_params> served_cells;
  /// Time to wait for the CU-CP response before the procedure fails.
  std::chrono::milliseconds response_timeout{3000};
};

/// Outcome of the F1 Setup procedure.
struct f1_setup_success {
  std::vector<f1ap_cell_to_activate> cells_to_activate;
  /// Packed F1AP PDUs captured during the setup exchange.
  byte_buffer packed_f1_setup_request;
  byte_buffer packed_f1_setup_response;
};
struct f1_setup_failure {
  /// Possible result outcomes for F1 Setup failure.
  enum class result_code { timeout, proc_failure, invalid_response, f1_setup_failure };

  /// Result outcome for F1 Setup failure.
  result_code result;
  /// Cause provided by CU-CP in case of F1 Setup Failure.
  std::string f1_setup_failure_cause;
  /// Time to wait before the F1 Setup is retried, if the CU-CP commanded one.
  std::optional<std::chrono::seconds> time_to_wait;
};
using f1_setup_result = expected<f1_setup_success, f1_setup_failure>;

/// Cell whose parameters need to be modified in the DU.
struct f1ap_cell_to_be_modified {
  /// New served Cell Information.
  du_served_cell_info cell_info;
  /// New System Information.
  std::optional<gnb_du_sys_info> du_sys_info;
};

/// gNB-DU initiated Config Update as per TS 38.473, Section 8.2.4.
struct gnbdu_config_update_request {
  std::vector<f1ap_cell_to_be_modified> cells_to_mod;
};

struct gnbdu_config_update_response {
  bool result;
};

/// gNB-CU initiated Config Update as per TS 38.473, Section 8.2.5.
/// Request sent by CU to DU via F1AP gNB-CU Configuration Update.
struct gnbcu_config_update_request {
  std::vector<f1ap_cell_to_activate> cells_to_activate;
  std::vector<nr_cell_global_id_t>   cells_to_deactivate;
  std::vector<f1ap_cell_to_bar>      cells_to_bar;
};

/// Result of gNB-CU initiated Config Update as per TS 38.473, Section 8.2.5.
struct gnbcu_config_update_response {
  std::vector<nr_cell_global_id_t> cells_failed_to_activate;
};

/// Handle F1AP interface management procedures as defined in TS 38.473 section 8.2.
class f1ap_connection_manager
{
public:
  virtual ~f1ap_connection_manager() = default;

  /// \brief Connect the DU to CU-CP via F1-C interface.
  [[nodiscard]] virtual bool connect_to_cu_cp() = 0;

  /// \brief Disconnect the DU from CU-CP after failed F1 Setup procedure.
  virtual async_task<void> disconnect_from_cu_cp() = 0;

  /// \brief Initiates the F1 Setup procedure as per TS 38.473, Section 8.2.3.
  /// \param[in] request The F1SetupRequest message to transmit.
  /// \return Returns a f1_setup_response_message struct with the success member set to 'true' in case of a
  /// successful outcome, 'false' otherwise. \remark The DU transmits the F1SetupRequest as per TS 38.473 section 8.2.3
  /// and awaits the response. If a F1SetupFailure is received the F1AP will handle the failure.
  virtual async_task<f1_setup_result> handle_f1_setup_request(const f1_setup_request_message& request) = 0;

  /// \brief Launches the F1 Removal procedure as per TS 38.473, Section 8.2.8.
  virtual async_task<void> handle_f1_removal_request() = 0;

  /// \brief Initiates F1AP reset procedure as per TS 38.473, Section 8.2.1.2.2.
  virtual async_task<f1_reset_acknowledgement> handle_f1_reset_request(const f1_reset_request& req) = 0;

  /// \brief Initiates F1AP gNB-DU config update procedure as per TS 38.473, Section 8.2.4.
  virtual async_task<gnbdu_config_update_response>
  handle_du_config_update(const gnbdu_config_update_request& request) = 0;

  /// \brief Query whether the F1-C interface has been setup with the CU-CP.
  virtual bool is_f1_setup() const = 0;
};

/// Notifier used by F1AP to signal to the DU any CU-initiated requests related with the F1AP interface management.
class f1ap_interface_update_notifier
{
public:
  virtual ~f1ap_interface_update_notifier() = default;

  /// \brief Notify the DU of an update requested initiated by the CU via F1AP, as per TS 38.473, Section 8.2.5.
  virtual async_task<gnbcu_config_update_response>
  request_cu_context_update(const gnbcu_config_update_request& request) = 0;
};

} // namespace odu
} // namespace ocudu
