// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/gtpu/gtpu_teid_pool_impl.h"
#include "tests/integrationtests/du_high/test_utils/du_high_worker_manager.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/f1ap/f1c_test_local_gateway.h"
#include "tests/test_doubles/mac/dummy_phy_cell_operation_controller.h"
#include "tests/unittests/cu_cp/test_doubles/mock_amf.h"
#include "tests/unittests/ngap/ngap_test_messages.h"
#include "tests/unittests/ngap/test_helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/cu_cp/cu_cp.h"
#include "ocudu/cu_cp/cu_cp_configuration_helpers.h"
#include "ocudu/cu_cp/cu_cp_factory.h"
#include "ocudu/du/du_cell_config_helpers.h"
#include "ocudu/du/du_high/du_high_clock_controller.h"
#include "ocudu/du/du_high/du_high_factory.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include "ocudu/support/io/io_broker_factory.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace odu;

// Dummy classes required by DU
struct phy_cell_dummy : public mac_cell_result_notifier {
  void on_new_downlink_scheduler_results(const mac_dl_sched_result& dl_res) override {}
  void on_new_downlink_data(const mac_dl_data_result& dl_data) override {}
  void on_new_uplink_scheduler_results(const mac_ul_sched_result& ul_res) override {}
  void on_cell_results_completion(slot_point slot) override {}
};

struct phy_dummy : public mac_result_notifier {
public:
  mac_cell_result_notifier& get_cell(du_cell_index_t cell_index) override { return cell; }
  phy_cell_dummy            cell;
};

/// Fixture class for successful F1Setup
class cu_du_test : public ::testing::Test
{
protected:
  void SetUp() override
  {
    ocudulog::fetch_basic_logger("TEST").set_level(ocudulog::basic_levels::debug);
    ocudulog::init();

    // Create CU-CP config.
    ocucp::cu_cp_configuration cu_cfg;

    cu_cfg.ngap.amfs.push_back(ocucp::cu_cp_configuration::amf_configuration{
        {{7, {{plmn_identity::test_value(), {{slice_service_type{1}}}}}}}});
    cu_cfg.metrics.layers_cfg.enable_ngap_metrics = true;
    cu_cfg.metrics.layers_cfg.enable_rrc_metrics  = true;

    std::vector<ocucp::n2_connection_client*> n2_gws;
    n2_gws.push_back(&*amf);

    // Create CU-CP dependencies.
    ocucp::cu_cp_dependencies cu_deps{.n2_gws            = std::move(n2_gws),
                                      .ng_setup_notifier = nullptr,
                                      .xnc_gws           = {},
                                      .cu_cp_executor    = workers.exec_mapper->du_control_executor(),
                                      .timers            = timers,
                                      .metrics_notifier  = nullptr};

    // create CU-CP.
    cu_cp_obj = create_cu_cp(cu_cfg, cu_deps);

    // Create AMF response to NG Setup.
    amf->enqueue_next_tx_pdu(ocucp::generate_ng_setup_response());

    // Start CU-CP.
    cu_cp_obj->start();

    // Attach F1-C gateway to CU-CP.
    f1c_gw.attach_cu_cp_du_repo(cu_cp_obj->get_f1c_handler());

    // create and start DU
    phy_dummy phy;

    du_high_configuration du_cfg{};
    du_cfg.ran.cells       = {config_helpers::make_default_du_cell_config()};
    du_cfg.ran.sched_cfg   = config_helpers::make_default_scheduler_expert_config();
    du_cfg.ran.gnb_du_name = "test_du";
    // Real-time timers expire before the CU-CP responds when the test runs under valgrind.
    du_cfg.f1ap.f1_setup_response_timeout = std::chrono::seconds{60};

    du_high_dependencies du_dependencies;
    du_dependencies.exec_mapper             = workers.exec_mapper.get();
    du_dependencies.f1c_client              = &f1c_gw;
    du_dependencies.f1u_teid_allocator      = &f1u_teid_allocator;
    du_dependencies.f1u_gw                  = &f1u_gw;
    du_dependencies.phy_adapter             = &phy;
    du_dependencies.timer_ctrl              = timer_ctrl.get();
    du_dependencies.phy_cell_op_controllers = {&phy_cell_op_ctrl};

    // create DU object
    du_obj = make_du_high(du_cfg, du_dependencies);

    // start CU and DU
    du_obj->start();
  }

  ~cu_du_test() override
  {
    // flush logger after each test
    ocudulog::flush();
  }

public:
  du_high_worker_manager                workers;
  timer_manager                         timers;
  std::unique_ptr<io_broker>            broker{create_io_broker(io_broker_type::epoll)};
  std::unique_ptr<mac_clock_controller> timer_ctrl{
      odu::create_du_high_clock_controller(timers, *broker, *workers.time_exec)};
  ocudulog::basic_logger& test_logger = ocudulog::fetch_basic_logger("TEST");
  f1c_test_local_gateway  f1c_gw;
  gtpu_teid_pool_impl f1u_teid_allocator{MAX_NOF_DU_UES * MAX_NOF_DRBS, GTPU_DEFAULT_TEID_RELEASE_LINGER_TIME, timers};
  f1u_test_local_gateway f1u_gw{};

  std::unique_ptr<ocucp::mock_amf>    amf{ocucp::create_mock_amf()};
  std::unique_ptr<ocucp::cu_cp>       cu_cp_obj;
  dummy_phy_cell_operation_controller phy_cell_op_ctrl;
  std::unique_ptr<du_high>            du_obj;
};

/// Test the f1 setup procedure was successful
TEST_F(cu_du_test, when_f1setup_successful_then_du_connected)
{
  OCUDU_TEST_REQUIREMENTS("MVP-ARCH-INTF-6", "MVP-ARCH-INTF-9");

  // check that DU has been added
  auto report = cu_cp_obj->get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus.size(), 1);
  ASSERT_EQ(report.dus[0].cells.size(), 1);
}
