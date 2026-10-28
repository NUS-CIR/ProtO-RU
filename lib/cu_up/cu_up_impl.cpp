// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_up_impl.h"
#include "cu_up_manager_impl.h"
#include "ngu_session_manager_impl.h"
#include "routines/cu_up_setup_routine.h"
#include "xnu_session_manager_impl.h"
#include "ocudu/e1ap/cu_up/e1ap_cu_up_factory.h"
#include "ocudu/gtpu/gtpu_demux_factory.h"
#include "ocudu/gtpu/gtpu_echo_factory.h"
#include "ocudu/gtpu/gtpu_teid_pool_factory.h"
#include "ocudu/support/executors/execute_until_success.h"

using namespace ocudu;
using namespace ocuup;

/// Asserts that the CU-UP dependencies are valid for the given configuration.
static void assert_cu_up_dependencies_valid(const cu_up_config& config, const cu_up_dependencies& dependencies)
{
  ocudu_assert(!dependencies.e1_conn_clients.empty(), "Invalid E1 connection client(s)");
  ocudu_assert(!dependencies.ngu_gws.empty(), "Invalid NG-U gateway list");
  ocudu_assert(config.ngu_cfg.gw_snssais.size() <= dependencies.ngu_gws.size(),
               "More NG-U gateway S-NSSAIs ({}) than NG-U gateways ({})",
               config.ngu_cfg.gw_snssais.size(),
               dependencies.ngu_gws.size());

  for (auto* e1 : dependencies.e1_conn_clients) {
    ocudu_assert(e1, "Invalid E1 gateway");
  }

  for (const auto& gw : dependencies.ngu_gws) {
    ocudu_assert(gw, "Invalid NG-U gateway");
  }

  for (const auto& gw : dependencies.xnu_gws) {
    ocudu_assert(gw, "Invalid Xn-U gateway");
  }
}

cu_up::cu_up(const cu_up_config& config_, cu_up_dependencies dependencies) :
  cfg(config_),
  ctrl_executor(dependencies.exec_mapper.ctrl_executor()),
  timers(dependencies.timers),
  e1_setup_notifier(std::move(dependencies.e1_setup_notifier)),
  logger(dependencies.logger),
  main_ctrl_loop(128)
{
  assert_cu_up_dependencies_valid(cfg, dependencies);

  // > Create and connect upper layers.

  // Create NG-U global rate limiter, if required.
  if (cfg.ngu_cfg.global_rate_limiter_bps) {
    uint64_t limit_bytes = *cfg.ngu_cfg.global_rate_limiter_bps / 8;
    ngu_limiter.emplace(limit_bytes, limit_bytes, timers);
  }

  // Create NG-U TEID allocator.
  ngu_teid_allocator = create_gtpu_allocator(
      gtpu_allocator_creation_request{.max_nof_teids            = cfg.max_nof_ues * MAX_NOF_PDU_SESSIONS,
                                      .teid_release_linger_time = cfg.ngu_cfg.gtpu_teid_release_linger_time,
                                      .timers                   = timers});

  // Create Xn-U TEID allocator. The DL data forwarding tunnels of an Xn handover are numbered out of it, apart from
  // the NG-U tunnels, because the two interfaces bind to different addresses (TS 38.401 section 6.1.2).
  xnu_teid_allocator = create_gtpu_allocator(
      gtpu_allocator_creation_request{.max_nof_teids            = cfg.max_nof_ues * MAX_NOF_PDU_SESSIONS,
                                      .teid_release_linger_time = cfg.ngu_cfg.gtpu_teid_release_linger_time,
                                      .timers                   = timers});

  // Create NG-U GTP-U demux.
  ngu_demux = create_gtpu_demux(
      gtpu_demux_creation_request{.cfg                 = gtpu_demux_cfg_t{.lif          = gtpu_logical_interface::ngu,
                                                                          .name         = "NG-U-DEMUX",
                                                                          .warn_on_drop = cfg.ngu_cfg.warn_on_drop,
                                                                          .test_mode    = cfg.test_mode_cfg.enabled,
                                                                          .queue_size   = cfg.ngu_cfg.gtpu_queue_size,
                                                                          .batch_size   = cfg.ngu_cfg.gtpu_batch_size},
                                  .teid_linger_checker = *ngu_teid_allocator,
                                  .gtpu_pcap           = dependencies.gtpu_pcap,
                                  .rate_limiter        = ngu_limiter.has_value() ? &ngu_limiter.value() : nullptr});

  echo_exec_mapper = dependencies.exec_mapper.create_ue_executor_mapper();
  report_error_if_not(echo_exec_mapper, "Could not create CU-UP executor for control TEID");

  // Create N3 GTP-U echo and register it at demux.
  auto ngu_echo_msg = gtpu_echo_creation_message{.gtpu_pcap = dependencies.gtpu_pcap, .tx_upper = gtpu_gw_adapter};
  ngu_echo          = create_gtpu_echo(ngu_echo_msg);

  expected<std::unique_ptr<gtpu_demux_dispatch_queue>> batch_q = ngu_demux->add_tunnel(
      GTPU_PATH_MANAGEMENT_TEID, echo_exec_mapper->dl_pdu_executor(), ngu_echo->get_rx_upper_layer_interface());
  report_error_if_not(batch_q.has_value(), "Could not create GTP-U echo tunnel.");
  echo_batched_queue = std::move(*batch_q);

  // Connect GTP-U DEMUX to adapter.
  gw_data_gtpu_demux_adapter = std::make_unique<network_gateway_data_gtpu_demux_adapter>(*ngu_demux);

  // Establish new NG-U session and connect the instantiated session to the GTP-U DEMUX adapter, so that the latter
  // is called when new NG-U DL PDUs are received.
  for (const auto& gw : dependencies.ngu_gws) {
    std::unique_ptr<gtpu_tnl_pdu_session> ngu_session = gw->create(*gw_data_gtpu_demux_adapter);
    if (!ngu_session) {
      report_error("Unable to allocate the required NG-U network resources");
    }
    ngu_sessions.push_back(std::move(ngu_session));
  }
  ngu_session_mngr = std::make_unique<ngu_session_manager_impl>(ngu_sessions, cfg.ngu_cfg.gw_snssais);

  // Connect GTPU GW adapter to NG-U session in order to send UL PDUs.
  // We use the first UDP GW for UL.
  gtpu_gw_adapter.connect_network_gateway(*ngu_sessions[0]);

  // Establish the Xn-U sessions and connect them to the GTP-U DEMUX adapter, so that the data a source NG-RAN node
  // forwards over Xn-U reaches the same demux as the DL PDUs of the 5GC.
  // TODO the demux keys the tunnels on the TEID alone, so it cannot hold an NG-U and an Xn-U tunnel with the same
  // TEID. Give it the interface as well before the data forwarding tunnels are registered.
  for (const auto& gw : dependencies.xnu_gws) {
    std::unique_ptr<gtpu_tnl_pdu_session> xnu_session = gw->create(*gw_data_gtpu_demux_adapter);
    if (!xnu_session) {
      report_error("Unable to allocate the required Xn-U network resources");
    }
    xnu_sessions.push_back(std::move(xnu_session));
  }

  // The DL data forwarding tunnels of a direct path are reported on the Xn-U sockets. Without one this CU-UP offers
  // no data forwarding over a direct path.
  if (not xnu_sessions.empty()) {
    xnu_session_mngr = std::make_unique<xnu_session_manager_impl>(xnu_sessions);
  }

  // Configure GTP-U Error Indication TX on the demux.
  {
    std::string ngu_bind_addr;
    if (ngu_sessions[0]->get_bind_address(ngu_bind_addr)) {
      ngu_demux->set_error_indication_tx(gtpu_gw_adapter, ngu_bind_addr);
    }
  }

  // > Create E1AP(s).
  e1ap_cu_up_mng_adapters.reserve(dependencies.e1_conn_clients.size());
  std::vector<std::reference_wrapper<e1ap_interface>> e1ap_refs;
  for (uint16_t e1_index = 0, e = dependencies.e1_conn_clients.size(); e1_index != e; e1_index++) {
    auto* e1_gw = dependencies.e1_conn_clients[e1_index];
    e1ap_cu_up_mng_adapters.emplace_back();
    e1ap_cu_up_manager_adapter&     e1ap_cu_up_mng_adapter = e1ap_cu_up_mng_adapters.back();
    std::unique_ptr<e1ap_interface> e1ap =
        create_e1ap(e1ap_configuration{.max_nof_ues      = cfg.max_nof_ues,
                                       .json_log_enabled = cfg.e1ap_json_log_enabled,
                                       .metrics_period   = cfg.e1ap_metrics_period,
                                       .e1_index         = cu_up_e1_index_t{e1_index}},
                    e1ap_cu_up_impl_dependencies{.e1_client_handler = *e1_gw,
                                                 .cu_up_notifier    = e1ap_cu_up_mng_adapter,
                                                 .timers            = dependencies.timers,
                                                 .cu_up_exec        = dependencies.exec_mapper.ctrl_executor()});
    e1ap_refs.emplace_back(*e1ap);
    e1aps.push_back(std::move(e1ap));
  }

  // > Create CU-UP manager.
  cu_up_mng = std::make_unique<cu_up_manager_impl>(
      cu_up_manager_impl_config{.cu_up_id      = cfg.cu_up_id,
                                .cu_up_name    = cfg.cu_up_name,
                                .max_nof_ues   = cfg.max_nof_ues,
                                .plmns         = cfg.plmns,
                                .qos           = cfg.qos,
                                .ngu_cfg       = cfg.ngu_cfg,
                                .test_mode_cfg = cfg.test_mode_cfg},
      cu_up_manager_impl_dependencies{.stop_command         = stop_command,
                                      .e1aps                = std::move(e1ap_refs),
                                      .ngu_demux            = *ngu_demux,
                                      .ngu_session_mngr     = *ngu_session_mngr,
                                      .ngu_teid_allocator   = *ngu_teid_allocator,
                                      .xnu_teid_allocator   = *xnu_teid_allocator,
                                      .xnu_session_mngr     = xnu_session_mngr.get(),
                                      .f1u_teid_allocator   = dependencies.f1u_teid_allocator,
                                      .exec_mapper          = dependencies.exec_mapper,
                                      .f1u_gateway          = dependencies.f1u_gateway,
                                      .timers               = dependencies.timers,
                                      .gtpu_pcap            = dependencies.gtpu_pcap,
                                      .cu_up_task_scheduler = main_ctrl_loop,
                                      .e1_setup_notifier    = e1_setup_notifier.get()});

  // > Connect E1AP(s) to CU-UP manager.
  for (auto& e1ap_cu_up_mng_adapter : e1ap_cu_up_mng_adapters) {
    e1ap_cu_up_mng_adapter.connect_cu_up_manager(*cu_up_mng);
  }

  // Start statistics report timer.
  if (cfg.statistics_report_period.count() > 0) {
    statistics_report_timer = dependencies.timers.create_unique_timer(dependencies.exec_mapper.ctrl_executor());
    statistics_report_timer.set(cfg.statistics_report_period, [this]() { on_statistics_report_timer_expired(); });
    statistics_report_timer.run();
  }
}

cu_up::~cu_up()
{
  stop();
}

void cu_up::start()
{
  logger.info("CU-UP starting...");

  std::unique_lock<std::mutex> lock(mutex);
  if (std::exchange(running, true)) {
    logger.warning("CU-UP already started. Ignoring start request");
    return;
  }

  std::promise<void> p;
  std::future<void>  fut = p.get_future();

  if (not ctrl_executor.execute([this, &p]() {
        main_ctrl_loop.schedule(
            [this, &p, connected = bool{false}, e1ap = e1aps.end()](coro_context<async_task<void>>& ctx) mutable {
              CORO_BEGIN(ctx);

              // Connect to CU-CP and send E1 Setup Request and await for E1 setup response.
              for (e1ap = e1aps.begin(); e1ap != e1aps.end(); ++e1ap) {
                CORO_AWAIT_VALUE(connected,
                                 launch_async<cu_up_setup_routine>(
                                     cu_up_setup_routine_config{
                                         .cu_up_id = cfg.cu_up_id, .cu_up_name = cfg.cu_up_name, .plmns = cfg.plmns},
                                     cu_up_setup_routine_dependencies{.logger            = logger,
                                                                      .e1ap_conn_mng     = **e1ap,
                                                                      .e1_setup_notifier = e1_setup_notifier.get()}));

                if (not connected) {
                  // Keep trying in the background, so that the CU-UP does not require the CU-CP to be reachable on
                  // startup. Note: the connection loss handler also covers a connection that was never established.
                  logger.info("e1={}: CU-CP is not reachable. Retrying the E1 connection in the background",
                              fmt::underlying((*e1ap)->get_e1_index()));
                  cu_up_mng->handle_e1ap_connection_drop((*e1ap)->get_e1_index());
                }
              }

              if (cfg.test_mode_cfg.enabled) {
                logger.info("enabling test mode...");
                CORO_AWAIT(cu_up_mng->enable_test_mode());
                logger.info("test mode enabled");
              }

              // Signal start() caller thread that the operation is complete.
              p.set_value();

              CORO_RETURN();
            });
      })) {
    report_fatal_error("Unable to initiate CU-UP setup routine");
  }

  // Block waiting for the first CU-CP connection attempt of every E1 interface to complete.
  fut.wait();
  logger.info("CU-UP started successfully");
}

void cu_up::stop()
{
  std::unique_lock<std::mutex> lock(mutex);
  if (!running) {
    return;
  }

  logger.debug("CU-UP stopping...");

  std::condition_variable cvar;

  auto stop_cu_up_main_loop = [this, &cvar]() mutable {
    // Dispatch coroutine to stop CU-UP.
    main_ctrl_loop.schedule(
        launch_async([this, &cvar, e1ap = e1aps.end()](coro_context<async_task<void>>& ctx) mutable {
          CORO_BEGIN(ctx);

          if (!running) {
            // Already stopped.
            CORO_EARLY_RETURN();
          }

          // Run E1 Release Procedure.
          for (e1ap = e1aps.begin(); e1ap != e1aps.end(); ++e1ap) {
            CORO_AWAIT((*e1ap)->handle_cu_up_e1ap_release_request());
          }

          // CU-UP stops listening to new GTPU Rx PDUs and stops pushing UL PDUs.
          CORO_AWAIT(handle_stop_command());

          // We defer main ctrl loop stop to let the current coroutine complete successfully.
          defer_until_success(ctrl_executor, timers, [this, &cvar]() {
            // Stop main control loop and communicate back with the caller thread.
            auto main_loop = main_ctrl_loop.request_stop();

            std::scoped_lock lock2(mutex);
            running = false;
            cvar.notify_all();
          });

          CORO_RETURN();
        }));
  };

  // Dispatch task to stop CU-UP main loop.
  stop_command = true;
  defer_until_success(ctrl_executor, timers, stop_cu_up_main_loop);

  // Wait until the all tasks of the main loop are completed and main loop has stopped.
  cvar.wait(lock, [this]() { return not running; });

  // CU-UP stops listening to new GTPU Rx PDUs.
  ngu_sessions.clear();

  logger.info("CU-UP stopped successfully");
}

async_task<void> cu_up::handle_stop_command()
{
  // Stop statistics report timer.
  statistics_report_timer.stop();

  gtpu_gw_adapter.disconnect();

  for (auto& e1ap_cu_up_mng_adapter : e1ap_cu_up_mng_adapters) {
    e1ap_cu_up_mng_adapter.disconnect();
  }
  // Do not disconnect GTP-U Demux as it is being concurrently accessed from the thread pool.
  // It will be safely stopped from inside the CU-UP manager.

  return launch_async([this](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);

    // Stop CU-UP manager and remove all UEs.
    CORO_AWAIT(cu_up_mng->stop());

    CORO_RETURN();
  });
}

void cu_up::on_statistics_report_timer_expired()
{
  // Log statistics.
  // TODO sum E1AP statistics.
  logger.debug("num_e1ap_ues={} num_cu_up_ues={}", e1aps[0]->get_nof_ues(), cu_up_mng->get_nof_ues());

  // Restart timer.
  statistics_report_timer.set(cfg.statistics_report_period, [this]() { on_statistics_report_timer_expired(); });
  statistics_report_timer.run();
}
