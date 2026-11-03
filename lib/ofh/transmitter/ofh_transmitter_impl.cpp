// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_transmitter_impl.h"
#include "ofh_uplane_fragment_size_calculator.h"
#include "ocudu/adt/format.h"
#include "ocudu/ofh/ofh_sector_executor_mapper.h"

using namespace ocudu;
using namespace ofh;

static uplink_request_handler_impl_config generate_uplink_request_handler_config(const transmitter_config& tx_config)
{
  uplink_request_handler_impl_config config;
  config.sector                        = tx_config.sector;
  config.is_prach_cp_enabled           = tx_config.is_prach_cp_enabled;
  config.prach_eaxc                    = tx_config.prach_eaxc;
  config.ul_data_eaxc                  = tx_config.ul_eaxc;
  config.tdd_config                    = tx_config.tdd_config;
  config.cp                            = tx_config.cp;
  config.scs                           = tx_config.scs;
  config.tx_timing_params              = tx_config.tx_timing_params;
  config.enable_log_warnings_for_lates = tx_config.enable_log_warnings_for_lates;

  return config;
}

static uplink_request_handler_impl_dependencies
resolve_uplink_request_handler_dependencies(transmitter_impl_dependencies& tx_dependencies)
{
  ocudu_assert(tx_dependencies.err_notifier, "Invalid error notifier");
  ocudu_assert(tx_dependencies.logger, "Invalid logger");

  return {*tx_dependencies.logger,
          *tx_dependencies.err_notifier,
          std::move(tx_dependencies.ul_slot_repo),
          std::move(tx_dependencies.ul_prach_repo),
          std::move(tx_dependencies.notifier_symbol_repo),
          std::move(tx_dependencies.ul_df_cplane),
          tx_dependencies.frame_pool_ul_cp};
}

static downlink_handler_impl_config generate_downlink_handler_config(const transmitter_config& tx_config)
{
  downlink_handler_impl_config out_cfg;
  out_cfg.sector                        = tx_config.sector;
  out_cfg.dl_eaxc                       = tx_config.dl_eaxc;
  out_cfg.tdd_config                    = tx_config.tdd_config;
  out_cfg.cp                            = tx_config.cp;
  out_cfg.scs                           = tx_config.scs;
  out_cfg.tx_timing_params              = tx_config.tx_timing_params;
  out_cfg.enable_log_warnings_for_lates = tx_config.enable_log_warnings_for_lates;
  out_cfg.is_cat_b_enabled              = tx_config.beamforming.has_value();

  return out_cfg;
}

static downlink_handler_impl_dependencies
resolve_downlink_handler_impl_dependencies(transmitter_impl_dependencies& tx_dependencies)
{
  ocudu_assert(tx_dependencies.err_notifier, "Invalid error notifier");
  ocudu_assert(tx_dependencies.logger, "Invalid logger");

  return {*tx_dependencies.logger,
          *tx_dependencies.err_notifier,
          std::move(tx_dependencies.dl_df_cplane),
          std::move(tx_dependencies.dl_df_uplane),
          tx_dependencies.frame_pool_dl_cp,
          tx_dependencies.frame_pool_dl_up};
}

transmitter_impl::transmitter_impl(const transmitter_config& config, transmitter_impl_dependencies&& dependencies) :
  dl_handler(generate_downlink_handler_config(config), resolve_downlink_handler_impl_dependencies(dependencies)),
  ul_request_handler(generate_uplink_request_handler_config(config),
                     resolve_uplink_request_handler_dependencies(dependencies)),
  ul_task_dispatcher(config.sector,
                     *dependencies.logger,
                     ul_request_handler,
                     dependencies.exec_mapper.get_ul_cp_executor(),
                     ul_request_handler.get_metrics_collector()),
  msg_transmitter(*dependencies.logger,
                  config.tx_timing_params,
                  config.are_metrics_enabled,
                  std::move(dependencies.eth_transmitter),
                  std::move(dependencies.frame_pool_dl_cp),
                  std::move(dependencies.frame_pool_ul_cp),
                  std::move(dependencies.frame_pool_dl_up)),
  ota_dispatcher(config.sector,
                 *dependencies.logger,
                 *dependencies.executor,
                 dl_handler.get_ota_symbol_boundary_notifier(),
                 ul_request_handler.get_ota_symbol_boundary_notifier(),
                 msg_transmitter),
  metrics_collector(config.are_metrics_enabled,
                    dl_handler.get_metrics_collector(),
                    ul_request_handler.get_metrics_collector(),
                    msg_transmitter.get_metrics_collector(),
                    msg_transmitter.get_ethernet_transmitter().get_metrics_collector())
{
}

void transmitter_impl::start()
{
  ota_dispatcher.start();
  ul_task_dispatcher.start();
  dl_handler.start();
}

void transmitter_impl::stop()
{
  dl_handler.stop();
  ul_task_dispatcher.stop();
  ota_dispatcher.stop();
}

uplink_request_handler& transmitter_impl::get_uplink_request_handler()
{
  return ul_task_dispatcher;
}

downlink_handler& transmitter_impl::get_downlink_handler()
{
  return dl_handler;
}

ota_symbol_boundary_notifier& transmitter_impl::get_ota_symbol_boundary_notifier()
{
  return ota_dispatcher;
}

transmitter_metrics_collector* transmitter_impl::get_metrics_collector()
{
  return metrics_collector.disabled() ? nullptr : &metrics_collector;
}
