// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/mac/mac_ctrl/mac_controller.h"
#include "lib/mac/rnti_manager.h"
#include "mac_ctrl_test_dummies.h"
#include "mac_test_helpers.h"
#include "tests/test_doubles/mac/dummy_mac_metrics_notifier.h"
#include "ocudu/du/du_high/du_high_clock_controller.h"
#include "ocudu/support/async/async_test_utils.h"
#include "ocudu/support/executors/manual_task_worker.h"
#include <gtest/gtest.h>

using namespace ocudu;

class mac_controller_test : public ::testing::Test
{
protected:
  mac_controller_test()
  {
    ocudulog::fetch_basic_logger("MAC", true).set_level(ocudulog::basic_levels::debug);
    ocudulog::init();
  }

  void start_ue_creation(mac_ue_create_request req)
  {
    t = mac_ctrl.handle_ue_create_request(req);
    t_launcher.emplace(t);
  }

  manual_task_worker                       worker{128};
  dummy_ue_executor_mapper                 ul_exec_mapper{worker};
  dummy_dl_executor_mapper                 dl_exec_mapper{&worker};
  dummy_mac_event_indicator                du_mng_notifier;
  timer_manager                            timers;
  test_helpers::dummy_mac_clock_controller clock_ctrl{timers};
  mac_scheduler_dummy_adapter              sched_cfg_adapter;
  dummy_mac_metrics_notifier               mac_notifier;
  mac_control_config                       maccfg{du_mng_notifier,
                            worker,
                            clock_ctrl,
                            mac_control_config::metrics_config{std::chrono::milliseconds{1000}, mac_notifier}};
  mac_ul_dummy_configurer                  ul_unit;
  mac_dl_dummy_configurer                  dl_unit;
  rnti_manager                             rnti_table;

  mac_controller mac_ctrl{maccfg, ul_unit, dl_unit, rnti_table, sched_cfg_adapter};

  async_task<mac_ue_create_response>                        t;
  std::optional<lazy_task_launcher<mac_ue_create_response>> t_launcher;
};

TEST_F(mac_controller_test, when_cg_cs_rnti_deallocated_then_rnti_value_is_reused)
{
  // Step 1: Create a UE directly via mac_ctrl_configurator::add_ue (bypasses async procedure).
  auto&  ctrl_cfg = static_cast<mac_ctrl_configurator&>(mac_ctrl);
  rnti_t ue_crnti = ctrl_cfg.add_ue(to_du_ue_index(0), to_du_cell_index(0), rnti_t::INVALID_RNTI);
  ASSERT_NE(ue_crnti, rnti_t::INVALID_RNTI);
  ASSERT_NE(mac_ctrl.find_ue(to_du_ue_index(0)), nullptr);
  ASSERT_EQ(mac_ctrl.find_ue(to_du_ue_index(0))->cs_rnti, rnti_t::INVALID_RNTI);

  // Step 2: Reconfigure UE with configured grant (cs_rnti_requested = true) → allocates CS-RNTI.
  mac_ue_reconfiguration_request reconfig_cg{};
  reconfig_cg.ue_index          = to_du_ue_index(0);
  reconfig_cg.pcell_index       = to_du_cell_index(0);
  reconfig_cg.crnti             = ue_crnti;
  reconfig_cg.cs_rnti_requested = true;

  async_task<mac_ue_reconfiguration_response>         t_cg = mac_ctrl.handle_ue_reconfiguration_request(reconfig_cg);
  lazy_task_launcher<mac_ue_reconfiguration_response> launcher_cg(t_cg);
  ASSERT_TRUE(t_cg.ready());
  ASSERT_TRUE(t_cg.get().result);
  ASSERT_TRUE(t_cg.get().cs_rnti_allocated.has_value());
  rnti_t saved_cs_rnti = t_cg.get().cs_rnti_allocated.value();
  ASSERT_NE(saved_cs_rnti, rnti_t::INVALID_RNTI);
  ASSERT_EQ(mac_ctrl.find_ue(to_du_ue_index(0))->cs_rnti, saved_cs_rnti);

  // Step 3: Reconfigure UE without configured grant (cs_rnti_requested = false) → deallocates CS-RNTI.
  mac_ue_reconfiguration_request reconfig_no_cg{};
  reconfig_no_cg.ue_index          = to_du_ue_index(0);
  reconfig_no_cg.pcell_index       = to_du_cell_index(0);
  reconfig_no_cg.crnti             = ue_crnti;
  reconfig_no_cg.cs_rnti_requested = false;

  async_task<mac_ue_reconfiguration_response> t_no_cg = mac_ctrl.handle_ue_reconfiguration_request(reconfig_no_cg);
  lazy_task_launcher<mac_ue_reconfiguration_response> launcher_no_cg(t_no_cg);
  ASSERT_TRUE(t_no_cg.ready());
  ASSERT_TRUE(t_no_cg.get().result);
  ASSERT_FALSE(t_no_cg.get().cs_rnti_allocated.has_value());
  ASSERT_EQ(mac_ctrl.find_ue(to_du_ue_index(0))->cs_rnti, rnti_t::INVALID_RNTI);

  // Step 4: Loop add/remove UEs until the freed CS-RNTI value is reused as a C-RNTI. To ensure having a reuse of the
  // CS-RNTI, we need to add/remove more than 65519 UEs, which is the C-RNTI range of the RNTI manager.
  bool found_reuse = false;
  for (unsigned i = 0; i != 70000; ++i) {
    rnti_t new_rnti = ctrl_cfg.add_ue(to_du_ue_index(1), to_du_cell_index(0), rnti_t::INVALID_RNTI);
    ASSERT_NE(new_rnti, rnti_t::INVALID_RNTI);
    ctrl_cfg.remove_ue(to_du_ue_index(1));
    if (new_rnti == saved_cs_rnti) {
      found_reuse = true;
      break;
    }
  }
  ASSERT_TRUE(found_reuse);
}

TEST_F(mac_controller_test, when_cg_cs_rnti_deallocated_then_nof_crnti_counter_is_correct)
{
  // Step 1: Create a UE directly via mac_ctrl_configurator::add_ue (bypasses async procedure).
  auto&  ctrl_cfg = static_cast<mac_ctrl_configurator&>(mac_ctrl);
  rnti_t ue_crnti = ctrl_cfg.add_ue(to_du_ue_index(0), to_du_cell_index(0), rnti_t::INVALID_RNTI);
  ASSERT_NE(ue_crnti, rnti_t::INVALID_RNTI);
  ASSERT_NE(mac_ctrl.find_ue(to_du_ue_index(0)), nullptr);
  ASSERT_EQ(mac_ctrl.find_ue(to_du_ue_index(0))->cs_rnti, rnti_t::INVALID_RNTI);

  // Step 2: Reconfigure UE with configured grant (cs_rnti_requested = true) → allocates CS-RNTI.
  mac_ue_reconfiguration_request reconfig_cg{};
  reconfig_cg.ue_index          = to_du_ue_index(0);
  reconfig_cg.pcell_index       = to_du_cell_index(0);
  reconfig_cg.crnti             = ue_crnti;
  reconfig_cg.cs_rnti_requested = true;

  async_task<mac_ue_reconfiguration_response>         t_cg = mac_ctrl.handle_ue_reconfiguration_request(reconfig_cg);
  lazy_task_launcher<mac_ue_reconfiguration_response> launcher_cg(t_cg);
  ASSERT_TRUE(t_cg.ready());
  ASSERT_TRUE(t_cg.get().result);
  ASSERT_TRUE(t_cg.get().cs_rnti_allocated.has_value());
  rnti_t saved_cs_rnti = t_cg.get().cs_rnti_allocated.value();
  ASSERT_NE(saved_cs_rnti, rnti_t::INVALID_RNTI);
  ASSERT_EQ(mac_ctrl.find_ue(to_du_ue_index(0))->cs_rnti, saved_cs_rnti);

  // Step 3: Reconfigure UE without configured grant (cs_rnti_requested = false) → deallocates CS-RNTI.
  mac_ue_reconfiguration_request reconfig_no_cg{};
  reconfig_no_cg.ue_index          = to_du_ue_index(0);
  reconfig_no_cg.pcell_index       = to_du_cell_index(0);
  reconfig_no_cg.crnti             = ue_crnti;
  reconfig_no_cg.cs_rnti_requested = false;

  async_task<mac_ue_reconfiguration_response> t_no_cg = mac_ctrl.handle_ue_reconfiguration_request(reconfig_no_cg);
  lazy_task_launcher<mac_ue_reconfiguration_response> launcher_no_cg(t_no_cg);
  ASSERT_TRUE(t_no_cg.ready());
  ASSERT_TRUE(t_no_cg.get().result);
  ASSERT_FALSE(t_no_cg.get().cs_rnti_allocated.has_value());
  ASSERT_EQ(mac_ctrl.find_ue(to_du_ue_index(0))->cs_rnti, rnti_t::INVALID_RNTI);

  // Step 4: Loop add (without removing) UEs until total number of MAX_NOF_DU_UES are added. If the mac_controller
  // incorrectly called rnti_table.add_ue() without is_cs_rnti=true for the CS-RNTI, the nof_crnti_ counter would be
  // inflated, causing an early RNTI allocation failure before all du_ue_indices are filled. Equivalently, if the
  // mac_controller incorrectly called rnti_table.rem_ue() without is_cs_rnti=true for the CS-RNTI, the nof_crnti_
  // counter would be incorrectly decreased, allowing an extra RNTI beyond its MAX_NOF_DU_UES capacity.
  for (unsigned i = 1; i != MAX_NOF_DU_UES; ++i) {
    rnti_t new_rnti = ctrl_cfg.add_ue(to_du_ue_index(i), to_du_cell_index(0), rnti_t::INVALID_RNTI);
    ASSERT_NE(new_rnti, rnti_t::INVALID_RNTI) << "Failed to add UE at du_ue_index=" << i;
  }

  // Verify that adding one more UE is rejected (all du_ue_indices and RNTIs are exhausted).
  rnti_t rejected_rnti = ctrl_cfg.add_ue(to_du_ue_index(0), to_du_cell_index(0), rnti_t::INVALID_RNTI);
  ASSERT_EQ(rejected_rnti, rnti_t::INVALID_RNTI);
}

TEST_F(mac_controller_test, when_cell_is_added_then_returned_controller_matches_the_one_later_fetched)
{
  mac_cell_creation_request cell_req = test_helpers::make_default_mac_cell_config();
  cell_req.cell_index                = to_du_cell_index(0);

  mac_cell_controller& added = mac_ctrl.add_cell(cell_req);

  ASSERT_EQ(&added, &mac_ctrl.get_cell_controller(cell_req.cell_index));
}

TEST_F(mac_controller_test, when_cell_is_added_then_initial_system_information_is_forwarded_to_the_mac_dl_cell)
{
  mac_cell_creation_request cell_req = test_helpers::make_default_mac_cell_config();
  cell_req.cell_index                = to_du_cell_index(0);

  mac_ctrl.add_cell(cell_req);

  ASSERT_EQ(dl_unit.cell_ctrls.size(), 1);
  ASSERT_EQ(dl_unit.cell_ctrls[0]->last_si_version, 0);
}

TEST_F(mac_controller_test, when_cell_is_removed_and_readded_then_controller_points_to_the_new_mac_dl_cell)
{
  mac_cell_creation_request cell_req = test_helpers::make_default_mac_cell_config();
  cell_req.cell_index                = to_du_cell_index(0);

  mac_ctrl.add_cell(cell_req);
  mac_ctrl.remove_cell(cell_req.cell_index);
  mac_ctrl.add_cell(cell_req);

  // The re-added cell must not forward to the MAC DL cell created for the previous incarnation.
  ASSERT_EQ(dl_unit.cell_ctrls.size(), 2);
  async_task<void> start_task = mac_ctrl.get_cell_controller(cell_req.cell_index).start();
  ASSERT_EQ(dl_unit.cell_ctrls[0]->nof_starts, 0);
  ASSERT_EQ(dl_unit.cell_ctrls[1]->nof_starts, 1);
}

TEST_F(mac_controller_test, ue_procedures)
{
  // Action 1: Create UE
  mac_ue_create_request ue_create_msg{to_du_cell_index(0), to_du_ue_index(1), to_rnti(0x4601)};
  start_ue_creation(ue_create_msg);

  // Status: UE creation started concurrently in MAC UL and DL.
  ASSERT_TRUE(ul_unit.last_ue_create_request.has_value());
  ASSERT_EQ(ue_create_msg.ue_index, ul_unit.last_ue_create_request->ue_index);
  ASSERT_EQ(ue_create_msg.crnti, ul_unit.last_ue_create_request->crnti);
  ASSERT_TRUE(dl_unit.last_ue_create_request.has_value());
  ASSERT_EQ(ue_create_msg.ue_index, dl_unit.last_ue_create_request->ue_index);
  ASSERT_EQ(ue_create_msg.crnti, dl_unit.last_ue_create_request->crnti);
  ASSERT_FALSE(t.ready());

  // Action 2: MAC UL UE Creation finishes; DL still pending.
  ul_unit.expected_result = true;
  ul_unit.ue_created_ev.set();
  ASSERT_FALSE(t.ready());

  // Action 3: MAC DL UE Creation finishes
  dl_unit.expected_result = true;
  dl_unit.ue_created_ev.set();

  // Status: Scheduler UE Creation starts.
  ASSERT_TRUE(sched_cfg_adapter.last_ue_create_request.has_value());
  ASSERT_EQ(sched_cfg_adapter.last_ue_create_request->ue_index, ue_create_msg.ue_index);
  ASSERT_FALSE(t.ready());

  // Action 4: Scheduler UE Creation finishes
  sched_cfg_adapter.ue_created_ev.set(true);

  // Status: MAC DL UE Creation finished. MAC CTRL UE Creation finished as well.
  ASSERT_TRUE(t.ready());
  ASSERT_EQ(ue_create_msg.ue_index, t.get().ue_index);
  ASSERT_EQ(t.get().allocated_crnti, ue_create_msg.crnti);
  ASSERT_TRUE(mac_ctrl.find_ue(ue_create_msg.ue_index) != nullptr);
  ASSERT_TRUE(mac_ctrl.find_by_rnti(ue_create_msg.crnti) != nullptr);
  ASSERT_EQ(ue_create_msg.ue_index, mac_ctrl.find_ue(ue_create_msg.ue_index)->du_ue_index);

  // Action 4: Delete UE
  mac_ue_delete_request ue_delete_msg{};
  ue_delete_msg.ue_index                        = to_du_ue_index(1);
  ue_delete_msg.rnti                            = to_rnti(0x4601);
  ue_delete_msg.cell_index                      = to_du_cell_index(0);
  async_task<mac_ue_delete_response>         t2 = mac_ctrl.handle_ue_delete_request(ue_delete_msg);
  lazy_task_launcher<mac_ue_delete_response> t_launcher2(t2);

  // Status: UE deleted from MAC DL, UL and CTRL
  ASSERT_TRUE(dl_unit.last_ue_delete_request.has_value());
  ASSERT_EQ(ue_delete_msg.ue_index, dl_unit.last_ue_create_request->ue_index);
  ASSERT_TRUE(ul_unit.last_ue_delete_request.has_value());
  ASSERT_EQ(ue_delete_msg.ue_index, ul_unit.last_ue_create_request->ue_index);
  ASSERT_TRUE(t2.ready());
  ASSERT_TRUE(mac_ctrl.find_ue(ue_create_msg.ue_index) == nullptr);
  ASSERT_TRUE(mac_ctrl.find_by_rnti(ue_create_msg.crnti) == nullptr);
}

TEST_F(mac_controller_test, when_cg_is_kept_in_reconfiguration_then_cs_rnti_is_kept)
{
  // Step 1: Create a UE directly via mac_ctrl_configurator::add_ue (bypasses async procedure).
  auto&  ctrl_cfg = static_cast<mac_ctrl_configurator&>(mac_ctrl);
  rnti_t ue_crnti = ctrl_cfg.add_ue(to_du_ue_index(0), to_du_cell_index(0), rnti_t::INVALID_RNTI);
  ASSERT_NE(ue_crnti, rnti_t::INVALID_RNTI);

  // Step 2: Reconfigure UE with configured grant (cs_rnti_requested = true) → allocates CS-RNTI.
  mac_ue_reconfiguration_request reconfig_cg{};
  reconfig_cg.ue_index          = to_du_ue_index(0);
  reconfig_cg.pcell_index       = to_du_cell_index(0);
  reconfig_cg.crnti             = ue_crnti;
  reconfig_cg.cs_rnti_requested = true;

  async_task<mac_ue_reconfiguration_response>         t_cg = mac_ctrl.handle_ue_reconfiguration_request(reconfig_cg);
  lazy_task_launcher<mac_ue_reconfiguration_response> launcher_cg(t_cg);
  ASSERT_TRUE(t_cg.ready());
  ASSERT_TRUE(t_cg.get().result);
  ASSERT_TRUE(t_cg.get().cs_rnti_allocated.has_value());
  rnti_t saved_cs_rnti = t_cg.get().cs_rnti_allocated.value();

  // Step 3: Reconfigure UE again, keeping the configured grant (e.g., a new DRB) → the same CS-RNTI is kept and
  // reported back, so that the DU does not release it.
  async_task<mac_ue_reconfiguration_response>         t_kept = mac_ctrl.handle_ue_reconfiguration_request(reconfig_cg);
  lazy_task_launcher<mac_ue_reconfiguration_response> launcher_kept(t_kept);
  ASSERT_TRUE(t_kept.ready());
  ASSERT_TRUE(t_kept.get().result);
  ASSERT_EQ(t_kept.get().cs_rnti_allocated, saved_cs_rnti);
  ASSERT_EQ(mac_ctrl.find_ue(to_du_ue_index(0))->cs_rnti, saved_cs_rnti);
}
