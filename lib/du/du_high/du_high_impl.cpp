// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "du_high_impl.h"
#include "adapters/adapters.h"
#include "adapters/f1ap_adapters.h"
#include "test_mode/du_test_mode_controller.h"
#include "ocudu/du/du_high/du_high_clock_controller.h"
#include "ocudu/du/du_high/du_manager/du_manager_factory.h"
#include "ocudu/f1ap/du/f1ap_du_factory.h"
#include "ocudu/mac/mac_factory.h"
#include "ocudu/mac/mac_metrics_notifier.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/timers.h"

using namespace ocudu;
using namespace odu;

/// \brief This class is responsible for providing the necessary adapters to connect layers in the DU-high.
class du_high_impl::layer_connector
{
public:
  explicit layer_connector(timer_manager& timers, task_executor& executor) :
    f1_to_du_notifier(timer_factory{timers, executor})
  {
  }

  /// Connect layers of the DU-high.
  void connect(du_manager& du_mng_, mac_interface& mac_inst)
  {
    mac_ev_notifier.connect(du_mng_.get_mac_event_handler(), du_mng_.get_metrics_aggregator());
    f1_to_du_notifier.connect(du_mng_.get_f1ap_event_handler());
    f1_to_du_notifier.connect_time_mapper(mac_inst.get_subframe_time_mapper());
    f1ap_paging_notifier.connect(mac_inst.get_cell_paging_info_handler());
  }

  /// Notifier MAC -> DU manager.
  du_manager_mac_event_indicator mac_ev_notifier;

  /// Notifier F1AP -> DU manager.
  f1ap_du_configurator_adapter f1_to_du_notifier;

  /// Notifier F1AP -> MAC for paging PDUs.
  mac_f1ap_paging_handler f1ap_paging_notifier;
};

du_high_impl::du_high_impl(const du_high_configuration& config_, const du_high_dependencies& dependencies) :
  cfg(config_),
  logger(ocudulog::fetch_basic_logger("DU")),
  timers(dependencies.timer_ctrl->get_timer_manager()),
  adapters(std::make_unique<layer_connector>(timers, dependencies.exec_mapper->du_control_executor()))
{
  if (cfg.test_cfg.test_ue.has_value()) {
    test_mode_ctrl = std::make_unique<du_test_mode_controller>(*cfg.test_cfg.test_ue,
                                                               *dependencies.f1c_client,
                                                               *dependencies.phy_adapter,
                                                               timers,
                                                               dependencies.exec_mapper->du_control_executor(),
                                                               cfg.ran.cells.size());
  }

  f1ap = create_f1ap(test_mode_ctrl ? test_mode_ctrl->get_f1c_client() : *dependencies.f1c_client,
                     adapters->f1_to_du_notifier,
                     dependencies.exec_mapper->du_control_executor(),
                     dependencies.exec_mapper->ue_mapper(),
                     adapters->f1ap_paging_notifier,
                     adapters->f1_to_du_notifier,
                     timers);

  mac = create_mac(mac_config{adapters->mac_ev_notifier,
                              dependencies.exec_mapper->ue_mapper(),
                              dependencies.exec_mapper->cell_mapper(),
                              dependencies.exec_mapper->du_control_executor(),
                              test_mode_ctrl ? test_mode_ctrl->get_phy_notifier() : *dependencies.phy_adapter,
                              cfg.ran.mac_cfg,
                              *dependencies.mac_p,
                              *dependencies.timer_ctrl,
                              mac_config::metrics_config{cfg.metrics.period,
                                                         cfg.metrics.enable_mac,
                                                         cfg.metrics.enable_sched,
                                                         cfg.metrics.enable_sched_ue,
                                                         cfg.metrics.max_nof_sched_ue_events,
                                                         adapters->mac_ev_notifier},
                              cfg.ran.sched_cfg});

  if (test_mode_ctrl != nullptr) {
    // In test mode, an adapting layer is placed in between MAC and other components.
    mac = test_mode_ctrl->decorate(std::move(mac));
  }

  du_mng = create_du_manager(
      du_manager_params{{cfg.ran.gnb_du_name, cfg.ran.gnb_du_id, 1, cfg.ran.cells, cfg.ran.srbs, cfg.ran.qos},
                        {timers,
                         dependencies.exec_mapper->du_control_executor(),
                         dependencies.exec_mapper->ue_mapper(),
                         dependencies.exec_mapper->cell_mapper()},
                        {*f1ap,
                         *f1ap,
                         f1ap->get_metrics_collector(),
                         dependencies.f1_setup_notifier,
                         cfg.f1ap.retry_tnl_connection,
                         cfg.f1ap.f1_setup_response_timeout},
                        {*dependencies.f1u_teid_allocator, *dependencies.f1u_gw},
                        {mac->get_ue_control_info_handler(),
                         *f1ap,
                         *f1ap,
                         *dependencies.rlc_p,
                         dependencies.rlc_metrics_notif,
                         cfg.rlc.drb_rx_window_seg_size,
                         cfg.rlc.drb_rx_window_seg_pool_size,
                         cfg.rlc.drb_tx_window_seg_size,
                         cfg.rlc.drb_tx_window_seg_pool_size,
                         cfg.rlc.srb_rx_window_seg_size,
                         cfg.rlc.srb_rx_window_seg_pool_size,
                         cfg.rlc.srb_tx_window_seg_size,
                         cfg.rlc.srb_tx_window_seg_pool_size,
                         cfg.rlc.pdu_recycler_size},
                        {*mac, cfg.ran.sched_cfg, dependencies.phy_cell_op_controllers},
                        {cfg.metrics.period,
                         dependencies.du_notifier,
                         cfg.metrics.enable_f1ap,
                         cfg.metrics.enable_mac,
                         cfg.metrics.enable_sched,
                         cfg.metrics.enable_du_proc},
                        cfg.test_cfg,
                        {cfg.ntn, dependencies.ntn_doppler_handler}});

  // Connect Layer<->DU manager adapters.
  adapters->connect(*du_mng, *mac);
}

du_high_impl::~du_high_impl()
{
  stop();
}

void du_high_impl::start()
{
  if (std::exchange(is_running, true)) {
    logger.warning("Discarding DU start request. Cause: DU already started.");
    return;
  }

  logger.info("Starting DU-High...");
  du_mng->get_controller().start();
  logger.info("DU-High started successfully");
}

void du_high_impl::stop()
{
  if (not std::exchange(is_running, false)) {
    return;
  }

  logger.info("Stopping DU-High...");
  du_mng->get_controller().stop();
  logger.info("DU-High stopped successfully");
}

f1ap_message_handler& du_high_impl::get_f1ap_pdu_handler()
{
  return *f1ap;
}

f1ap_ue_id_translator& du_high_impl::get_f1ap_ue_id_translator()
{
  return *f1ap;
}

mac_pdu_handler& du_high_impl::get_pdu_handler()
{
  return mac->get_pdu_handler();
}

mac_cell_slot_handler& du_high_impl::get_slot_handler(du_cell_index_t cell_idx)
{
  return mac->get_slot_handler(cell_idx);
}

mac_cell_rach_handler& du_high_impl::get_rach_handler(du_cell_index_t cell_index)
{
  return mac->get_rach_handler(cell_index);
}

mac_cell_control_information_handler& du_high_impl::get_control_info_handler(du_cell_index_t cell_index)
{
  return mac->get_control_info_handler(cell_index);
}

du_configurator& du_high_impl::get_du_configurator()
{
  return du_mng->get_operation_configurator();
}

mac_subframe_time_mapper& du_high_impl::get_subframe_time_mapper()
{
  return mac->get_subframe_time_mapper();
}

ocudu_ntn::ntn_configuration_manager* du_high_impl::get_ntn_configuration_manager()
{
  return du_mng->get_ntn_configuration_manager();
}
