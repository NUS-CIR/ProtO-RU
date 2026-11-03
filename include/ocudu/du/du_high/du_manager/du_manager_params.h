// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/du/du_cell_config.h"
#include "ocudu/du/du_high/du_high_executor_mapper.h"
#include "ocudu/du/du_high/du_manager/du_f1_setup_notifier.h"
#include "ocudu/du/du_high/du_qos_config.h"
#include "ocudu/du/du_high/du_srb_config.h"
#include "ocudu/du/du_high/du_test_mode_config.h"
#include "ocudu/f1ap/du/f1ap_du.h"
#include "ocudu/f1u/du/f1u_gateway.h"
#include "ocudu/mac/mac_manager.h"
#include "ocudu/mac/mac_ue_control_information_handler.h"
#include "ocudu/ntn/ntn_configuration_manager_config.h"
#include "ocudu/pcap/rlc_pcap.h"
#include "ocudu/ran/gnb_du_id.h"
#include "ocudu/rlc/rlc_metrics.h"
#include "ocudu/rlc/rlc_resource_defaults.h"
#include "ocudu/scheduler/config/scheduler_expert_config.h"
#include <map>
#include <optional>

namespace ocudu {

class timer_manager;
class mac_metrics_notifier;
class scheduler_metrics_notifier;
class phy_cell_operation_controller;

namespace ocudu_ntn {
class ntn_doppler_compensation_handler;
}

namespace odu {

class du_metrics_notifier;

struct du_manager_params {
  struct ran_params {
    std::string                        gnb_du_name;
    gnb_du_id_t                        gnb_du_id;
    uint8_t                            rrc_version;
    std::vector<du_cell_config>        cells;
    std::map<srb_id_t, du_srb_config>  srbs;
    std::map<five_qi_t, du_qos_config> qos;
  };

  struct service_params {
    timer_manager&                timers;
    task_executor&                du_mng_exec;
    du_high_ue_executor_mapper&   ue_execs;
    du_high_cell_executor_mapper& cell_execs;
  };

  struct f1ap_config_params {
    f1ap_connection_manager&       conn_mng;
    f1ap_ue_context_manager&       ue_mng;
    f1ap_metrics_collector&        metrics;
    du_f1_setup_complete_notifier* f1_setup_complete_notifier = nullptr;
    /// Whether a failed F1-C TNL connection is retried indefinitely on startup, instead of closing the application
    /// after a single attempt. The reconnection that follows a lost F1-C connection is retried either way.
    bool retry_tnl_connection = false;
    /// Time to wait for the CU-CP response to an F1 Setup Request.
    std::chrono::milliseconds f1_setup_response_timeout{3000};
  };

  struct f1u_config_params {
    gtpu_teid_pool& f1u_teid_allocator;
    f1u_du_gateway& f1u_gw;
  };

  struct rlc_config_params {
    mac_ue_control_information_handler&          mac_ue_info_handler;
    f1ap_message_handler&                        f1ap_rx_msg_handler;
    f1ap_rrc_message_transfer_procedure_handler& f1ap_rx_proc_handler;
    rlc_pcap&                                    pcap_writer;
    rlc_metrics_notifier*                        rlc_metrics_notif           = nullptr;
    std::size_t                                  drb_rx_window_seg_size      = rlc_drb_rx_window_seg_size;
    std::size_t                                  drb_rx_window_seg_pool_size = rlc_drb_rx_window_seg_pool_size;
    std::size_t                                  drb_tx_window_seg_size      = rlc_drb_tx_window_seg_size;
    std::size_t                                  drb_tx_window_seg_pool_size = rlc_drb_tx_window_seg_pool_size;
    std::size_t                                  srb_rx_window_seg_size      = rlc_srb_rx_window_seg_size;
    std::size_t                                  srb_rx_window_seg_pool_size = rlc_srb_rx_window_seg_pool_size;
    std::size_t                                  srb_tx_window_seg_size      = rlc_srb_tx_window_seg_size;
    std::size_t                                  srb_tx_window_seg_pool_size = rlc_srb_tx_window_seg_pool_size;
    std::size_t                                  pdu_recycler_size           = rlc_pdu_recycler_size;
  };

  struct mac_config_params {
    /// Interface to configure the MAC layer.
    mac_manager&            mgr;
    scheduler_expert_config sched_cfg;
    /// \brief Per-cell PHY operation controllers, indexed by du_cell_index: exactly one non-null entry per cell.
    ///
    /// Forwarded by du_setup_procedure into mac_cell_creation_request.phy_cell_op_controller when adding cells, so
    /// MAC cell start/stop drives the PHY operation controller. The setup aborts when a cell has no controller.
    std::vector<phy_cell_operation_controller*> phy_cell_op_controllers;
  };

  struct metrics_config_params {
    std::chrono::milliseconds period{1000};
    du_metrics_notifier*      du_metrics    = nullptr;
    bool                      f1ap_enabled  = false;
    bool                      mac_enabled   = false;
    bool                      sched_enabled = false;
    bool                      proc_enabled  = false;
  };

  struct ntn_config_params {
    /// NTN configuration. When present with at least one cell, the DU manager creates an NTN configuration manager
    /// that periodically refreshes SIB19 and the feeder link Doppler pre- and post-compensation.
    std::optional<ocudu_ntn::ntn_configuration_manager_config> cfg;
    /// Handler applying the feeder link Doppler compensation. Left unset by deployments that apply no Doppler
    /// compensation, in which case the computed values are simply not applied. It must outlive the DU manager.
    ocudu_ntn::ntn_doppler_compensation_handler* doppler_handler = nullptr;
  };

  ran_params            ran;
  service_params        services;
  f1ap_config_params    f1ap;
  f1u_config_params     f1u;
  rlc_config_params     rlc;
  mac_config_params     mac;
  metrics_config_params metrics;
  du_test_mode_config   test_cfg;
  ntn_config_params     ntn;
};

} // namespace odu
} // namespace ocudu
