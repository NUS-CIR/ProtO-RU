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
#include "ocudu/f1ap/gateways/f1c_connection_client.h"
#include "ocudu/gtpu/gtpu_teid_pool.h"
#include "ocudu/mac/mac_cell_result.h"
#include "ocudu/mac/mac_config.h"
#include "ocudu/ntn/ntn_configuration_manager_config.h"
#include "ocudu/pcap/dlt_pcap.h"
#include "ocudu/pcap/rlc_pcap.h"
#include "ocudu/ran/gnb_du_id.h"
#include "ocudu/rlc/rlc_metrics.h"
#include "ocudu/rlc/rlc_resource_defaults.h"
#include "ocudu/scheduler/config/scheduler_expert_config.h"
#include "ocudu/scheduler/scheduler_metrics.h"
#include <map>
#include <optional>

namespace ocudu {

class phy_cell_operation_controller;
class timer_manager;

namespace ocudu_ntn {
class ntn_doppler_compensation_handler;
}

namespace odu {

class f1u_du_gateway;
class du_metrics_notifier;

/// RAN-specific parameters of the DU-high.
struct du_high_ran_config {
  std::string                        gnb_du_name = "odu";
  gnb_du_id_t                        gnb_du_id   = gnb_du_id_t::min;
  std::vector<du_cell_config>        cells;
  std::map<srb_id_t, du_srb_config>  srbs;
  std::map<five_qi_t, du_qos_config> qos;
  mac_expert_config                  mac_cfg;
  scheduler_expert_config            sched_cfg;
};

/// Configuration passed to DU-High.
struct du_high_configuration {
  struct rlc_config {
    std::size_t drb_rx_window_seg_size      = rlc_drb_rx_window_seg_size;
    std::size_t drb_rx_window_seg_pool_size = rlc_drb_rx_window_seg_pool_size;
    std::size_t drb_tx_window_seg_size      = rlc_drb_tx_window_seg_size;
    std::size_t drb_tx_window_seg_pool_size = rlc_drb_tx_window_seg_pool_size;
    std::size_t srb_rx_window_seg_size      = rlc_srb_rx_window_seg_size;
    std::size_t srb_rx_window_seg_pool_size = rlc_srb_rx_window_seg_pool_size;
    std::size_t srb_tx_window_seg_size      = rlc_srb_tx_window_seg_size;
    std::size_t srb_tx_window_seg_pool_size = rlc_srb_tx_window_seg_pool_size;
    std::size_t pdu_recycler_size           = rlc_pdu_recycler_size;
  };

  struct f1ap_config {
    /// Whether a failed F1-C TNL connection is retried indefinitely on startup, so that the DU does not require the
    /// CU-CP to be reachable when it is started. When disabled, the DU makes a single attempt and closes the
    /// application if it fails. The reconnection that follows a lost F1-C connection is retried either way.
    bool retry_tnl_connection = false;
    /// Time to wait for the CU-CP response to an F1 Setup Request.
    std::chrono::milliseconds f1_setup_response_timeout{3000};
  };

  struct metrics_config {
    bool                      enable_sched    = false;
    bool                      enable_sched_ue = true;
    bool                      enable_mac      = false;
    bool                      enable_rlc      = false;
    bool                      enable_f1ap     = false;
    bool                      enable_du_proc  = false;
    std::chrono::milliseconds period{1000};
    unsigned                  max_nof_sched_ue_events = 64;
  };

  rlc_config          rlc;
  du_high_ran_config  ran;
  f1ap_config         f1ap;
  metrics_config      metrics;
  du_test_mode_config test_cfg;
  /// Optional NTN configuration. When present with at least one cell, the DU-high creates an NTN configuration
  /// manager that periodically refreshes SIB19 and the feeder link Doppler pre- and post-compensation.
  std::optional<ocudu_ntn::ntn_configuration_manager_config> ntn;
};

/// DU high dependencies
struct du_high_dependencies {
  du_high_executor_mapper* exec_mapper        = nullptr;
  f1c_connection_client*   f1c_client         = nullptr;
  gtpu_teid_pool*          f1u_teid_allocator = nullptr;
  f1u_du_gateway*          f1u_gw             = nullptr;
  mac_result_notifier*     phy_adapter        = nullptr;
  mac_clock_controller*    timer_ctrl         = nullptr;
  du_metrics_notifier*     du_notifier        = nullptr;
  rlc_metrics_notifier*    rlc_metrics_notif  = nullptr;
  mac_pcap*                mac_p              = nullptr;
  rlc_pcap*                rlc_p              = nullptr;
  /// Optional notifier invoked once after a successful F1 Setup.
  du_f1_setup_complete_notifier* f1_setup_notifier = nullptr;
  /// Handler applying the feeder link Doppler compensation computed by the NTN configuration manager. Left unset by
  /// deployments that apply no Doppler compensation, in which case the computed values are simply not applied. It
  /// must outlive the DU-high.
  ocudu_ntn::ntn_doppler_compensation_handler* ntn_doppler_handler = nullptr;
  /// \brief Per-cell PHY operation controllers, indexed by du_cell_index: exactly one non-null entry per cell.
  ///
  /// Supplied by the layer that owns the PHY operation controllers, which must outlive the DU-high. The DU hands
  /// the controller of cell index i to the MAC as mac_cell_creation_request.phy_cell_op_controller, so MAC cell
  /// start/stop drives the PHY. The DU aborts its setup when a cell has no controller.
  std::vector<phy_cell_operation_controller*> phy_cell_op_controllers;
};

} // namespace odu
} // namespace ocudu
