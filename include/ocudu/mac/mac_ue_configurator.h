// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/mac/bsr_config.h"
#include "ocudu/mac/mac_cell_group_config.h"
#include "ocudu/mac/mac_sdu_handler.h"
#include "ocudu/mac/msg3_mac_ce.h"
#include "ocudu/mac/phr_config.h"
#include "ocudu/ran/harq_id.h"
#include "ocudu/ran/logical_channel/lcid.h"
#include "ocudu/ran/physical_cell_group.h"
#include "ocudu/ran/rnti.h"
#include "ocudu/ran/sr_configuration.h"
#include "ocudu/ran/time_alignment_config.h"
#include "ocudu/scheduler/scheduler_configurator.h"
#include "ocudu/support/async/async_task.h"

namespace ocudu {

class unique_timer;

/// Cause of a radio link failure detected in the MAC.
enum class mac_rlf_cause { max_consecutive_harq_kos, max_consecutive_crc_kos, max_consecutive_csi_dtx };

/// Interface used to notify detected radio link failures in the MAC (e.g. due to max KOs reached) for a given UE.
class mac_ue_radio_link_notifier
{
public:
  virtual ~mac_ue_radio_link_notifier() = default;

  /// \brief Notifies that a radio link failure has been detected for a given UE.
  virtual void on_rlf_detected(mac_rlf_cause cause) = 0;

  /// \brief Notifies that a MAC C-RNTI CE was received with old C-RNTI set to equal to the given UE.
  ///
  /// The detection of a MAC C-RNTI CE should cancel the handling of any previously detected RLF due to out-of-sync
  /// (e.g. PUCCH/PUSCH KOs). It should not have an effect on RLFs due the RLC max retransmissions or protocol failure
  /// as those problems are not possible to recover from without Reestablishment.
  virtual void on_crnti_ce_received() = 0;
};

/// Parameters passed to MAC concerning a created logical channel.
struct mac_logical_channel_config {
  lcid_t               lcid;
  mac_sdu_rx_notifier* ul_bearer;
  mac_sdu_tx_builder*  dl_bearer;
};

/// Input parameters used to create a UE in the scheduler.
struct mac_ue_create_request {
  /// Serving cell of the UE to be created.
  du_cell_index_t cell_index;
  /// \brief DU-specific UE index for the UE to be created.
  du_ue_index_t ue_index;
  /// \brief C-RNTI to assign to the newly created UE. The C-RNTI can be set to be equal to a previously allocated
  /// TC-RNTI, in case the UE went through RA procedure. If this field is equal INVALID_RNTI, the MAC will generate a
  /// new C-RNTI.
  rnti_t                                  crnti;
  mac_ue_radio_link_notifier*             rlf_notifier;
  std::vector<mac_logical_channel_config> bearers;
  mac_cell_group_config                   mac_cell_group_cfg;
  physical_cell_group_config              phy_cell_group_cfg;
  bool                                    initial_fallback = true;
  const byte_buffer*                      ul_ccch_msg      = nullptr;
  std::optional<slot_point>               ul_ccch_slot_rx;
  /// RA preamble to be used within a Contention-free RA procedure context.
  std::optional<unsigned> cfra_preamble_index;
  /// Number of DL HARQ processes configured for the UE on its PCell. Used by MAC to dimension DL HARQ buffers.
  unsigned pdsch_harqs_per_cell = MAX_NOF_HARQS;

  // Scheduler-only params.
  sched_ue_config_request sched_cfg;
};

/// Outcome of a MAC UE creation request procedure.
struct mac_ue_create_response {
  du_cell_index_t cell_index = INVALID_DU_CELL_INDEX;
  du_ue_index_t   ue_index   = INVALID_DU_UE_INDEX;
  /// C-RNTI allocated to the created UE in the MAC. INVALID_RNTI if the UE was not created.
  rnti_t allocated_crnti = rnti_t::INVALID_RNTI;
};

/// Input parameters used to reconfigure a UE in the scheduler.
struct mac_ue_reconfiguration_request {
  du_ue_index_t   ue_index;
  du_cell_index_t pcell_index;
  rnti_t          crnti;
  /// When true, the UE has a configured grant and needs a CS-RNTI. The MAC controller generates one if the UE does not
  /// hold one yet. When false, the MAC controller releases the CS-RNTI the UE holds, if any.
  bool cs_rnti_requested = false;
  /// When set, carries the generated CS-RNTI for this UE that needs to be passed to the scheduler.
  /// If not set, the scheduler won't associate any CS-RNTI with this UE.
  std::optional<rnti_t>                     assigned_cs_rnti;
  std::vector<mac_logical_channel_config>   bearers_to_addmod;
  std::vector<lcid_t>                       bearers_to_rem;
  std::optional<mac_cell_group_config>      mac_cell_group_cfg;
  std::optional<physical_cell_group_config> phy_cell_group_cfg;
  // Scheduler-only params.
  sched_ue_config_request sched_cfg;
};

/// \brief Outcome of a MAC UE reconfiguration request procedure.
struct mac_ue_reconfiguration_response {
  du_ue_index_t ue_index;
  bool          result;
  /// When set, indicates the CS-RNTI allocated to this UE, either by this reconfiguration or by a previous one.
  /// If not set, the UE holds no CS-RNTI.
  std::optional<rnti_t> cs_rnti_allocated;
};

/// Input parameters used to delete a UE in the scheduler.
struct mac_ue_delete_request {
  /// Serving cell of the UE to be deleted.
  du_cell_index_t cell_index;
  /// DU-specific UE index of the UE to be deleted.
  du_ue_index_t ue_index;
  /// C-RNTI of the UE to be deleted.
  rnti_t rnti;
  /// \brief Minimum wait period between the UE being marked as inactive and actually being removed from MAC.
  ///
  /// This delay allows to absorb any in-flight UL transmissions from the UE while it is being released.
  std::chrono::milliseconds min_removal_delay{0};
};

/// \brief Outcome of a MAC UE deletion request procedure.
struct mac_ue_delete_response {
  bool result;
};

/// \brief Interface used to manage the creation, reconfiguration and deletion of UEs in MAC
class mac_ue_configurator
{
public:
  virtual ~mac_ue_configurator() = default;

  /// \brief Creates a UE context in the MAC.
  virtual async_task<mac_ue_create_response> handle_ue_create_request(const mac_ue_create_request& cfg) = 0;

  /// \brief Reconfigures an existing UE context in the MAC.
  virtual async_task<mac_ue_reconfiguration_response>
  handle_ue_reconfiguration_request(const mac_ue_reconfiguration_request& cfg) = 0;

  /// \brief Deletes an existing UE context in the MAC.
  virtual async_task<mac_ue_delete_response> handle_ue_delete_request(const mac_ue_delete_request& cfg) = 0;

  /// \brief Forwards the UL-CCCH message to upper layers and the MAC CEs of the same Msg3 to the scheduler.
  ///
  /// Both are decoded before the UE is created, but cannot be acted upon until the UE context exists.
  ///
  /// \return false if the Msg3 content could not be handed over to the MAC UL. Losing the MAC CEs alone is not
  /// reported, as it is not fatal to UE creation.
  virtual bool handle_ul_ccch_msg(du_ue_index_t    ue_index,
                                  du_cell_index_t  cell_index,
                                  slot_point       slot_rx,
                                  byte_buffer      ul_ccch_msg,
                                  msg3_mac_ce_list mac_ces) = 0;

  /// Handle the confirmation that the UE received the last UE dedicated RRC configuration.
  virtual void handle_ue_config_applied(du_ue_index_t ue_index) = 0;
};

} // namespace ocudu
