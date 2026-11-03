// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/e2/procedures/ric_connection_setup_routine.h"
#include "tests/unittests/e2/common/e2_test_helpers.h"
#include "ocudu/support/async/async_test_utils.h"
#include <gtest/gtest.h>

using namespace ocudu;

/// Connection manager stub: configurable TNL failures and sequential setup responses.
class dummy_ric_conn_mngr_setup : public e2_connection_manager
{
public:
  int tnl_fail_count   = 0;
  int tnl_call_count   = 0;
  int setup_fail_count = 0;
  int setup_call_count = 0;
  int disconnect_count = 0;

  bool handle_e2_tnl_connection_request() override
  {
    ++tnl_call_count;
    return tnl_call_count > tnl_fail_count;
  }

  async_task<void> handle_e2_node_initiated_removal_request() override
  {
    return launch_async([](coro_context<async_task<void>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN();
    });
  }

  async_task<void> handle_e2_disconnection_request() override
  {
    ++disconnect_count;
    return launch_async([](coro_context<async_task<void>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN();
    });
  }

  async_task<e2_setup_response_message> handle_e2_setup_request(const e2_setup_request_message&) override
  {
    ++setup_call_count;
    e2_setup_response_message resp = {};
    resp.success                   = (setup_call_count > setup_fail_count);
    return launch_async([resp](coro_context<async_task<e2_setup_response_message>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN(resp);
    });
  }
};

class ric_connection_setup_routine_test : public ::testing::Test
{
protected:
  void SetUp() override
  {
    ocudulog::fetch_basic_logger("TEST").set_level(ocudulog::basic_levels::debug);
    ocudulog::init();

    cfg                             = config_helpers::make_default_e2ap_config();
    cfg.gnb_du_id                   = int_to_gnb_du_id(1);
    cfg.e2sm_kpm_enabled            = true;
    cfg.ric_reconnection_retry_time = std::chrono::milliseconds{100};
    factory                         = timer_factory{timers, task_worker};

    du_meas_provider = std::make_unique<dummy_e2sm_kpm_du_meas_provider>();
    e2sm_kpm_packer  = std::make_unique<e2sm_kpm_asn1_packer>(*du_meas_provider);
    kpm_iface        = std::make_unique<e2sm_kpm_impl>(test_logger, *e2sm_kpm_packer, *du_meas_provider);
    e2sm_mngr        = std::make_unique<e2sm_manager>(test_logger);
    e2sm_mngr->add_e2sm_service(e2sm_kpm_asn1_packer::oid, std::move(kpm_iface));

    conn_mngr         = std::make_unique<dummy_ric_conn_mngr_setup>();
    node_cfg_event    = std::make_unique<manual_event<std::vector<e2_node_component_config>>>();
    node_cfg_provider = std::make_unique<dummy_e2_node_component_config_provider>(*node_cfg_event);

    e2_node_component_config ncfg;
    ncfg.interface_type = e2_node_component_interface_type::f1;
    node_cfg_event->set(std::vector<e2_node_component_config>{std::move(ncfg)});
  }

  void TearDown() override { ocudulog::flush(); }

  void tick()
  {
    timers.tick();
    task_worker.run_pending_tasks();
  }

  void advance_retry_periods(unsigned n)
  {
    for (unsigned i = 0; i < n; ++i) {
      for (unsigned ms = 0; ms <= 100; ++ms) {
        tick();
      }
    }
  }

  e2ap_config                                                          cfg;
  timer_manager                                                        timers;
  manual_task_worker                                                   task_worker{64};
  timer_factory                                                        factory;
  std::atomic<bool>                                                    stopped{false};
  std::atomic<bool>                                                    ric_connected{false};
  std::unique_ptr<dummy_e2sm_kpm_du_meas_provider>                     du_meas_provider;
  std::unique_ptr<e2sm_kpm_asn1_packer>                                e2sm_kpm_packer;
  std::unique_ptr<e2sm_interface>                                      kpm_iface;
  std::unique_ptr<e2sm_manager>                                        e2sm_mngr;
  std::unique_ptr<dummy_ric_conn_mngr_setup>                           conn_mngr;
  std::unique_ptr<manual_event<std::vector<e2_node_component_config>>> node_cfg_event;
  std::unique_ptr<dummy_e2_node_component_config_provider>             node_cfg_provider;
  ocudulog::basic_logger& test_logger = ocudulog::fetch_basic_logger("TEST");
};

/// When TNL succeeds and setup succeeds on the first attempt the routine exits immediately.
TEST_F(ric_connection_setup_routine_test, immediate_success_exits)
{
  async_task<void> t = launch_async<ric_connection_setup_routine>(
      cfg, *node_cfg_provider, *e2sm_mngr, *conn_mngr, factory, test_logger, stopped, ric_connected);
  lazy_task_launcher<void> launcher(t);
  task_worker.run_pending_tasks();

  ASSERT_TRUE(t.ready());
  ASSERT_TRUE(ric_connected.load());
  ASSERT_EQ(conn_mngr->tnl_call_count, 1);
  ASSERT_EQ(conn_mngr->setup_call_count, 1);
}

/// When TNL fails N times the routine retries after each period, then exits on success.
TEST_F(ric_connection_setup_routine_test, tnl_retries_then_exits)
{
  conn_mngr->tnl_fail_count = 2;

  async_task<void> t = launch_async<ric_connection_setup_routine>(
      cfg, *node_cfg_provider, *e2sm_mngr, *conn_mngr, factory, test_logger, stopped, ric_connected);
  lazy_task_launcher<void> launcher(t);
  task_worker.run_pending_tasks();

  ASSERT_FALSE(t.ready());

  advance_retry_periods(2); // two failing TNL attempts
  task_worker.run_pending_tasks();

  ASSERT_TRUE(t.ready());
  ASSERT_TRUE(ric_connected.load());
  ASSERT_EQ(conn_mngr->tnl_call_count, 3);
}

/// When setup is rejected the routine exits immediately without retrying.
TEST_F(ric_connection_setup_routine_test, setup_rejection_exits_immediately)
{
  conn_mngr->setup_fail_count = 1;

  async_task<void> t = launch_async<ric_connection_setup_routine>(
      cfg, *node_cfg_provider, *e2sm_mngr, *conn_mngr, factory, test_logger, stopped, ric_connected);
  lazy_task_launcher<void> launcher(t);
  task_worker.run_pending_tasks();

  ASSERT_TRUE(t.ready());
  ASSERT_FALSE(ric_connected.load());
  ASSERT_EQ(conn_mngr->setup_call_count, 1);
  ASSERT_EQ(conn_mngr->disconnect_count, 0);
}

/// When stopped is set the routine exits on the next loop-top check.
TEST_F(ric_connection_setup_routine_test, stopped_exits_cleanly)
{
  conn_mngr->tnl_fail_count = 1; // first attempt fails, enters wait

  async_task<void> t = launch_async<ric_connection_setup_routine>(
      cfg, *node_cfg_provider, *e2sm_mngr, *conn_mngr, factory, test_logger, stopped, ric_connected);
  lazy_task_launcher<void> launcher(t);
  task_worker.run_pending_tasks();

  ASSERT_FALSE(t.ready());
  stopped = true;

  advance_retry_periods(1); // timer fires, loop checks stopped -> breaks
  task_worker.run_pending_tasks();

  ASSERT_TRUE(t.ready());
}
