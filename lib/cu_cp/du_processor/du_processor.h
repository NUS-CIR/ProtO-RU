// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../pdcp/pdcp_ue_context_removal_handler.h"
#include "du_configuration_handler.h"
#include "du_metrics_handler.h"
#include "du_reported_cell.h"
#include "ocudu/cu_cp/cell_meas_manager_config.h"
#include "ocudu/f1ap/cu_cp/f1ap_cu.h"
#include "ocudu/rrc/rrc_du.h"

namespace ocudu::ocucp {

/// Forward declared messages.
struct rrc_ue_creation_message;

/// Interface to notify UE context management procedures.
class du_processor_f1ap_ue_context_notifier
{
public:
  virtual ~du_processor_f1ap_ue_context_notifier() = default;

  /// \brief Notify the F1AP to initiate the UE Context Release procedure.
  /// \param[in] msg The UE Context Release message to transmit.
  /// \return Returns the index of the released UE.
  virtual async_task<cu_cp_ue_index_t> on_ue_context_release_command(const f1ap_ue_context_release_command& msg) = 0;

  /// \brief Notify the F1AP to initiate the UE Context Modification procedure.
  /// \param[in] request The UE Context Modification message to transmit.
  /// \return Returns a f1ap_ue_context_modification_response_message struct with the success member set to
  /// 'true' in case of a successful outcome, 'false' otherwise.
  virtual async_task<f1ap_ue_context_modification_response>
  on_ue_context_modification_request(const f1ap_ue_context_modification_request& request) = 0;

  /// \brief Notify the F1AP that the given UE corresponds to a reestablishment session of the old UE.
  /// \param[in] ue_index The index of the UE that is performing a reestablishment.
  /// \param[in] old_ue_index The index of the UE that
  virtual bool on_intra_du_reestablishment(cu_cp_ue_index_t ue_index, cu_cp_ue_index_t old_ue_index) = 0;
};

/// Interface to notify Paging procedures.
class du_processor_f1ap_paging_notifier
{
public:
  virtual ~du_processor_f1ap_paging_notifier() = default;

  /// Notify F1AP to send paging message.
  virtual void on_paging_message(const cu_cp_paging_message& msg) = 0;
};

/// \brief Helpers to query information about DU.
class du_processor_cell_info_interface
{
public:
  virtual ~du_processor_cell_info_interface() = default;

  /// \brief Checks whether a cell with the specified PCI is served by the DU.
  virtual bool has_cell(pci_t pci) = 0;

  /// \brief Checks whether a cell with the specified NR cell global id is served by the DU.
  virtual bool has_cell(nr_cell_global_id_t cgi) = 0;

  /// \brief Whether the DU knows about this cell, in either served or deactivated state.
  ///
  /// has_cell() returns false once a cell is locked because it only checks served_cells. The
  /// activate_cell path needs to find cells that were previously deactivated, hence this variant.
  virtual bool has_cell_any_state(nr_cell_global_id_t cgi) = 0;

  /// \brief PCI-keyed variant of has_cell_any_state(). Lets the mobility path recognize a handover
  /// target that this CU-CP owns but keeps administratively deactivated, instead of mistaking it
  /// for another gNB's cell.
  virtual bool has_cell_any_state(pci_t pci) = 0;

  /// \brief Get DU configuration context.
  virtual const du_configuration_context* get_context() const = 0;
};

/// Interface used by mobility manager to trigger handover routines.
class du_processor_mobility_handler
{
public:
  virtual ~du_processor_mobility_handler() = default;

  /// \brief Retrieve CGI for a given PCI of a DU.
  virtual std::optional<nr_cell_global_id_t> get_cgi(pci_t pci) = 0;

  /// \brief Retrieve the SIB1 for a given PCI of a DU.
  virtual byte_buffer get_packed_sib1(nr_cell_global_id_t cgi) = 0;
};

/// Methods used by DU processor to notify about DU specific events.
class du_processor_cu_cp_notifier
{
public:
  virtual ~du_processor_cu_cp_notifier() = default;

  /// \brief Request to update the measurement related parameters for the given cell id.
  /// \param[in] nci The cell id of the serving cell to update.
  /// \param[in] serv_cell_cfg_ The serving cell meas config to update.
  virtual bool on_cell_config_update_request(nr_cell_identity nci, const serving_cell_meas_config& serv_cell_cfg_) = 0;

  /// \brief Notify the CU-CP about the cells reported by the DU in the F1 Setup procedure, so the
  /// corresponding logical cells are realized.
  /// \return NCIs of the reported cells the CU-CP activates; reported cells absent from it stay dormant
  /// (admin-locked).
  virtual std::vector<nr_cell_identity> on_du_cells_reported(cu_cp_du_index_t             du_index,
                                                             span<const du_reported_cell> cells) = 0;

  /// \brief Notify the CU-CP about the cells a DU stopped serving, so their logical cells are de-realized.
  virtual void on_du_cells_removed(span<const nr_cell_identity> cells) = 0;

  /// \brief Asks the CU-CP if an RRC_INACTIVE UE can resume.
  /// \param[in] ue_index The index of the RRC_INACTIVE UE.
  /// \param[in] resume_cause The resume cause of the RRC Resume Request.
  /// \return True if the UE is accepted.
  virtual bool on_ue_resume_admission_request(cu_cp_ue_index_t ue_index, resume_cause_t resume_cause) = 0;

  /// \brief Notifies about a successful RRC UE creation.
  /// \param[in] ue_index The index of the UE.
  /// \param[in] rrc_ue The created RRC UE.
  virtual void on_rrc_ue_created(cu_cp_ue_index_t ue_index, rrc_ue_interface& rrc_ue) = 0;

  /// \brief Notify the CU-CP that the SIB1 for a given PCI of a DU is required.
  /// \param[in] du_index The index of the DU the cell is connected to.
  /// \param[in] cgi The cell global id of the target cell.
  /// \returns The packed SIB1 for the target cell, if available. An empty byte_buffer otherwise.
  virtual byte_buffer on_target_cell_sib1_required(cu_cp_du_index_t du_index, nr_cell_global_id_t cgi) = 0;

  /// \brief Notify the CU-CP to completly remove a UE from the CU-CP.
  /// \param[in] ue_index The index of the UE to remove.
  virtual async_task<void> on_ue_removal_required(cu_cp_ue_index_t ue_index) = 0;

  /// \brief Notify the CU-CP about an E-CID measurement report of the gNB-DU.
  virtual void on_e_cid_measurement_report(const e_cid_measurement_report_t& report) = 0;

  /// \brief Notify the CU-CP about an E-CID measurement failure indication of the gNB-DU.
  virtual void on_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind) = 0;

  /// \brief Notify the CU-CP to release a UE.
  /// \param[in] request The release request.
  virtual async_task<void> on_ue_release_required(const cu_cp_ue_context_release_request& request) = 0;

  /// \brief Notify the CU-CP that a UE has successfully accessed a CHO target cell.
  /// \param[in] msg The access success indication.
  virtual async_task<void> on_access_success(const cu_cp_access_success_indication& msg) = 0;

  /// \brief Notify that the F1 transaction information was lost for some UEs.
  virtual async_task<void> on_transaction_info_loss(const ue_transaction_info_loss_event& ev) = 0;

  /// \brief Notify the CU-CP that the NR cells served by this DU changed.
  virtual void on_served_cells_updated() = 0;
};

class du_processor_configuration_update_interface
{
public:
  virtual ~du_processor_configuration_update_interface() = default;

  /// \brief Apply neccesary configuration changes and forward the GNB CU Configuration Update to the F1AP.
  /// \param[in] request The F1AP GNB CU Configuration Update.
  /// \return The F1AP GNB CU Configuration Update response.
  virtual async_task<f1ap_gnb_cu_configuration_update_response>
  handle_configuration_update(const f1ap_gnb_cu_configuration_update& request) = 0;
};

/// Handler to remove per-UE F1AP to PDCP and RRC to F1AP adapter state held by the DU processor.
class du_processor_ue_context_removal_handler
{
public:
  virtual ~du_processor_ue_context_removal_handler() = default;

  /// \brief Remove all DU-processor adapter state for the given UE.
  /// \param[in] ue_index The index of the UE to remove.
  virtual void remove_ue_context(cu_cp_ue_index_t ue_index) = 0;
};

class du_processor : public du_processor_cell_info_interface, public du_processor_configuration_update_interface
{
public:
  virtual ~du_processor() = default;

  /// \brief Retrieve F1AP handler for the respective DU.
  virtual f1ap_cu& get_f1ap_handler() = 0;

  /// \brief Retrieve RRC DU handler for the respective DU.
  virtual rrc_du& get_rrc_du_handler() = 0;

  /// \brief Retrieve the handler for removing per-UE SRB PDCP entities.
  virtual pdcp_ue_context_removal_handler& get_pdcp_ue_removal_handler() = 0;

  /// \brief Retrieve the handler for removing per-UE DU processor adapter state.
  virtual du_processor_ue_context_removal_handler& get_du_processor_ue_removal_handler() = 0;

  virtual du_processor_mobility_handler& get_mobility_handler() = 0;

  /// \brief Retrieve the DU-specific metrics handler.
  virtual du_metrics_handler& get_metrics_handler() = 0;
};

} // namespace ocudu::ocucp
