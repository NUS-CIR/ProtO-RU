// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "du_high_cu_test_simulator.h"
#include "tests/test_doubles/du/test_du_high_worker_manager.h"
#include "tests/test_doubles/f1ap/f1ap_test_message_validators.h"
#include "tests/test_doubles/mac/mac_test_messages.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "tests/unittests/ngap/ngap_test_messages.h"
#include "ocudu/cu_cp/cu_cp_configuration_helpers.h"
#include "ocudu/cu_cp/cu_cp_factory.h"
#include "ocudu/du/du_high/du_high_clock_controller.h"
#include "ocudu/du/du_high/du_high_factory.h"
#include "ocudu/mac/mac_cell_timing_context.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include "ocudu/support/io/io_broker_factory.h"
#include <gtest/gtest.h>

using namespace ocudu;

static constexpr uint32_t task_worker_queue_size = 10000;

static void init_loggers()
{
  ocudulog::fetch_basic_logger("MAC", true).set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("SCHED", true).set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("RLC").set_level(ocudulog::basic_levels::info);
  ocudulog::fetch_basic_logger("DU-MNG").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("DU-F1").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("CU-CP-F1").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("CU-CP").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("RRC").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("NGAP").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("ASN1").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("TEST").set_level(ocudulog::basic_levels::debug);
  ocudulog::init();
}

du_high_cu_cp_worker_manager::du_high_cu_cp_worker_manager(unsigned nof_dus, timer_manager& timers) :
  test_worker(task_worker_queue_size)
{
  init_loggers();

  for (unsigned i = 0; i != nof_dus; ++i) {
    dus.push_back(test_helpers::create_multi_threaded_du_high_executor_mapper(
        test_helpers::du_high_worker_config{1, false, {}, timers}));
  }

  // CU-CP specific executor.
  // Note: Reuse one of the DU-high control executors.
  cu_cp_exec = &dus[0]->get_exec_mapper().du_control_executor();
}

du_high_cu_cp_worker_manager::~du_high_cu_cp_worker_manager()
{
  stop();
}

void du_high_cu_cp_worker_manager::stop()
{
  for (auto& w : dus) {
    w->stop();
  }
  test_worker.stop();
}

du_high_cu_test_simulator::du_high_cu_test_simulator(const du_high_cu_cp_test_simulator_config& cfg_) :
  cfg(cfg_),
  logger(ocudulog::fetch_basic_logger("TEST")),
  workers(cfg.dus.size(), timers),
  broker(create_io_broker(io_broker_type::epoll)),
  timer_ctrl(odu::create_du_high_clock_controller(timers, *broker, workers.dus[0]->timer_executor()))
{
  // Prepare CU-CP config.
  ocucp::cu_cp_configuration cu_cfg;
  cu_cfg.ngap.amfs.push_back(
      ocucp::cu_cp_configuration::amf_configuration{{{7, {{plmn_identity::test_value(), {{slice_service_type{1}}}}}}}});

  std::vector<ocucp::n2_connection_client*> n2_gws;
  n2_gws.push_back(&n2_gw);

  // Prepare CU-CP dependencies.
  ocucp::cu_cp_dependencies cu_deps{.n2_gws            = std::move(n2_gws),
                                    .ng_setup_notifier = nullptr,
                                    .xnc_gws           = {},
                                    .cu_cp_executor    = *workers.cu_cp_exec,
                                    .timers            = timers,
                                    .metrics_notifier  = nullptr};

  // Instantiate CU-CP.
  cu_cp_inst = create_cu_cp(cu_cfg, cu_deps);

  // Start CU-CP.
  cu_cp_inst->start();

  // Connect AMF by injecting a ng_setup_response
  cu_cp_inst->get_ng_handler()
      .get_ngap_message_handler(uint_to_cu_cp_amf_index(0))
      ->handle_message(ocucp::generate_ng_setup_response());

  // Connect F1-C to CU-CP.
  f1c_gw.attach_cu_cp_du_repo(cu_cp_inst->get_f1c_handler());
}

du_high_cu_test_simulator::~du_high_cu_test_simulator()
{
  logger.info("Stopping test simulator.");

  for (auto& du : dus) {
    du->du_high_inst->stop();
  }

  workers.stop();
}

bool du_high_cu_test_simulator::add_ue(unsigned du_index, rnti_t rnti)
{
  f1c_gw.clear_messages();

  du_sim& du_ctxt = *dus[du_index];

  // Send UL-CCCH message.
  du_ctxt.du_high_inst->get_pdu_handler().handle_rx_data_indication(
      test_helpers::create_ccch_message(du_ctxt.next_slot.without_hyper_sfn(), rnti));

  // Wait for Init UL RRC Message to come out of the F1AP.
  run_until([this, du_index]() { return not f1c_gw.get_last_cu_cp_rx_pdus(du_index).empty(); });

  ocudu_assert(f1c_gw.get_last_cu_cp_rx_pdus(du_index).size() == 1, "Too many messages sent by the DU");

  return test_helpers::is_init_ul_rrc_msg_transfer_valid(f1c_gw.get_last_cu_cp_rx_pdus(du_index)[0], rnti);
}

void du_high_cu_test_simulator::start_dus()
{
  for (unsigned du_idx = 0; du_idx != cfg.dus.size(); ++du_idx) {
    dus.emplace_back(std::make_unique<du_sim>(workers.test_worker));
    auto& du_ctxt = *dus.back();

    // Setup DU-specific slot index.
    du_ctxt.next_slot = {subcarrier_spacing::kHz15,
                         test_rng::uniform_int<unsigned>(0, NOF_HYPER_SFNS * NOF_SFNS * NOF_SUBFRAMES_PER_FRAME - 1)};

    // Instantiate DU-high.
    odu::du_high_configuration& du_hi_cfg = du_ctxt.du_high_cfg;
    du_hi_cfg.ran.gnb_du_name             = fmt::format("ocudu{}", du_idx + 1);
    du_hi_cfg.ran.gnb_du_id               = static_cast<gnb_du_id_t>(du_idx + 1);
    du_hi_cfg.ran.cells                   = cfg.dus[du_idx];
    du_hi_cfg.ran.sched_cfg               = config_helpers::make_default_scheduler_expert_config();
    // Real-time timers expire before the CU-CP responds when the test runs under valgrind.
    du_hi_cfg.f1ap.f1_setup_response_timeout = std::chrono::seconds{60};

    odu::du_high_dependencies du_dependencies;
    du_dependencies.exec_mapper        = &workers.dus[du_idx]->get_exec_mapper();
    du_dependencies.f1c_client         = &f1c_gw;
    du_dependencies.f1u_teid_allocator = &f1u_teid_allocator;
    du_dependencies.f1u_gw             = nullptr;
    du_dependencies.phy_adapter        = &du_ctxt.phy;
    du_dependencies.timer_ctrl         = timer_ctrl.get();
    du_dependencies.mac_p              = &du_ctxt.mac_pcap;
    du_dependencies.rlc_p              = &du_ctxt.rlc_pcap;
    du_ctxt.phy_cell_op_ctrls.resize(du_hi_cfg.ran.cells.size());
    for (auto& ctrl : du_ctxt.phy_cell_op_ctrls) {
      du_dependencies.phy_cell_op_controllers.push_back(&ctrl);
    }

    du_ctxt.du_high_inst = make_du_high(du_hi_cfg, du_dependencies);

    du_ctxt.du_high_inst->start();
  }
}

void du_high_cu_test_simulator::run_slot()
{
  for (unsigned i = 0; i != dus.size(); ++i) {
    odu::du_high& du_hi = *dus[i]->du_high_inst;

    // Signal slot indication to l2.
    du_hi.get_slot_handler(to_du_cell_index(0))
        .handle_slot_indication({dus[i]->next_slot, std::chrono::system_clock::now()});

    // Wait for slot indication to be processed and the l2 results to be sent back to the l1 (in this case, the test
    // main thread).
    constexpr unsigned                        MAX_COUNT = 1000;
    const std::optional<mac_dl_sched_result>& dl_result = dus[i]->phy.cells[0].last_dl_res;
    for (unsigned count = 0;
         count < MAX_COUNT and (not dl_result.has_value() or dl_result->slot != dus[i]->next_slot.without_hyper_sfn());
         ++count) {
      // Process tasks dispatched to the test main thread (e.g. L2 slot result)
      workers.test_worker.run_pending_tasks();

      // Wait for tasks to arrive to test thread.
      std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }
    EXPECT_TRUE(dl_result.has_value() and dl_result->slot == dus[i]->next_slot.without_hyper_sfn());

    // Increment the DU slot.
    ++dus[i]->next_slot;
  }
}

bool du_high_cu_test_simulator::run_until(unique_function<bool()> condition)
{
  constexpr unsigned MAX_SLOT_COUNT = 1000;
  for (unsigned count = 0; count != MAX_SLOT_COUNT; ++count) {
    if (condition()) {
      return true;
    }
    run_slot();
  }
  return false;
}
