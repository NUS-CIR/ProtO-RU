// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief Tests that check the setup/teardown, addition/removal of UEs in the DU-high class.

#include "tests/integrationtests/du_high/test_utils/du_high_env_simulator.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/du_high/rrm_policy_remote_command_helpers.h"
#include "tests/test_doubles/f1ap/f1ap_test_message_validators.h"
#include "tests/test_doubles/f1ap/f1ap_test_messages.h"
#include "tests/test_doubles/mac/mac_test_messages.h"
#include "tests/test_doubles/pdcp/pdcp_pdu_generator.h"
#include "tests/test_doubles/scheduler/scheduler_result_finder.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "tests/unittests/f1ap/du/f1ap_du_test_helpers.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents_ue.h"
#include "ocudu/asn1/rrc_nr/cell_group_config.h"
#include "ocudu/support/executors/task_worker.h"

using namespace ocudu;
using namespace odu;
using namespace asn1::f1ap;

class du_high_tester : public du_high_env_simulator, public testing::Test
{
protected:
  void push_pdcp_pdus_to_all_f1u_bearers(unsigned nof_pdcp_pdus, unsigned pdcp_pdu_size)
  {
    for (unsigned i = 0; i < nof_pdcp_pdus; ++i) {
      nru_dl_message f1u_pdu{.t_pdu = test_helpers::create_pdcp_pdu(
                                 pdcp_sn_size::size12bits, /* is_srb = */ false, i, pdcp_pdu_size, i & 0xffU)};
      for (auto& bearer : cu_up_sim.bearers) {
        bearer.second.rx_notifier->on_new_pdu(f1u_pdu);
      }
    }
  }

  void push_pdcp_pdus_to_ue_f1u_bearers(du_ue_index_t ue_index, unsigned nof_pdcp_pdus, unsigned pdcp_pdu_size)
  {
    for (unsigned i = 0; i < nof_pdcp_pdus; ++i) {
      nru_dl_message f1u_pdu{.t_pdu = test_helpers::create_pdcp_pdu(
                                 pdcp_sn_size::size12bits, /* is_srb = */ false, i, pdcp_pdu_size, i & 0xffU)};
      for (auto& [key, bearer] : cu_up_sim.bearers) {
        if (key.first == ue_index) {
          bearer.rx_notifier->on_new_pdu(f1u_pdu);
        }
      }
    }
  }
};

TEST_F(du_high_tester, when_ccch_msg_is_received_then_ue_context_is_created)
{
  // Reset the last sent F1AP PDU (e.g. F1 setup).
  cu_notifier.f1ap_ul_msgs.clear();

  // Add UE
  du_hi->get_pdu_handler().handle_rx_data_indication(
      test_helpers::create_ccch_message(next_slot.without_hyper_sfn(), to_rnti(0x4601)));

  // TEST CASE: DU sends Initial UL RRC Message
  this->run_until([this]() { return not cu_notifier.f1ap_ul_msgs.empty(); });
  ASSERT_EQ(cu_notifier.f1ap_ul_msgs.size(), 1);
  ASSERT_TRUE(
      test_helpers::is_init_ul_rrc_msg_transfer_valid(cu_notifier.f1ap_ul_msgs.rbegin()->second, to_rnti(0x4601)));
}

TEST_F(du_high_tester, when_two_concurrent_ccch_msg_are_received_then_two_ue_contexts_are_created)
{
  cu_notifier.f1ap_ul_msgs.clear();

  // Add two UEs.
  du_hi->get_pdu_handler().handle_rx_data_indication(
      test_helpers::create_ccch_message(next_slot.without_hyper_sfn(), to_rnti(0x4601)));
  du_hi->get_pdu_handler().handle_rx_data_indication(
      test_helpers::create_ccch_message(next_slot.without_hyper_sfn(), to_rnti(0x4602)));

  this->run_until([this]() { return cu_notifier.f1ap_ul_msgs.size() >= 2; });

  // TEST CASE: DU sends two Initial UL RRC Messages, one for each UE.
  ASSERT_EQ(cu_notifier.f1ap_ul_msgs.size(), 2);
  auto it = cu_notifier.f1ap_ul_msgs.begin();
  ASSERT_TRUE(test_helpers::is_init_ul_rrc_msg_transfer_valid(it->second, to_rnti(0x4601)));
  ++it;
  ASSERT_TRUE(test_helpers::is_init_ul_rrc_msg_transfer_valid(it->second, to_rnti(0x4602)));
}

TEST_F(du_high_tester, when_ue_context_setup_completes_then_drb_is_active)
{
  // Create UE.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));
  ASSERT_TRUE(run_ue_context_setup(rnti));

  // TEST CASE: Ensure DU<->CU-UP tunnel was created.
  ASSERT_EQ(cu_up_sim.last_drb_id.value(), drb_id_t::drb1);

  // Event: Forward several DRB PDUs.
  const unsigned nof_pdcp_pdus = 100, pdcp_pdu_size = 128;
  push_pdcp_pdus_to_all_f1u_bearers(nof_pdcp_pdus, pdcp_pdu_size);

  // TEST CASE: Ensure DRB is active by verifying that the DRB PDUs are scheduled.
  unsigned bytes_sched = 0;
  phy.cells[0].last_dl_data.reset();
  while (bytes_sched < nof_pdcp_pdus * pdcp_pdu_size and this->run_until([this]() {
    return phy.cells[0].last_dl_data.has_value() and not phy.cells[0].last_dl_data.value().ue_pdus.empty();
  })) {
    const auto ue_grants = phy.cells[0].last_dl_res->dl_res.ue_grants();
    for (unsigned i = 0; i != phy.cells[0].last_dl_data.value().ue_pdus.size(); ++i) {
      if (ue_grants[i].pdsch_cfg.codewords[0].new_data) {
        bytes_sched += phy.cells[0].last_dl_data.value().ue_pdus[i].pdu.get_buffer().size();
      }
    }
    phy.cells[0].last_dl_data.reset();
  }
  ASSERT_GE(bytes_sched, nof_pdcp_pdus * pdcp_pdu_size)
      << "Not enough PDSCH grants were scheduled to meet the enqueued PDCP PDUs";
}

TEST_F(du_high_tester, when_ue_context_release_received_then_ue_gets_deleted)
{
  // Create UE.
  ASSERT_TRUE(add_ue(to_rnti(0x4601)));

  // EVENT: Receive UE Context Release Command.
  cu_notifier.f1ap_ul_msgs.clear();
  f1ap_message msg = generate_ue_context_release_command();
  this->du_hi->get_f1ap_pdu_handler().handle_message(msg);

  for (unsigned i = 0, max_count = 1000; i != max_count; ++i) {
    this->run_slot();

    // Stop loop, When UE Context Release complete has been sent to CU.
    if (not cu_notifier.f1ap_ul_msgs.empty()) {
      break;
    }
  }

  // TEST CASE: Ensure UE Context Release Complete is sent.
  ASSERT_EQ(cu_notifier.f1ap_ul_msgs.size(), 1);
  ASSERT_TRUE(test_helpers::is_valid_ue_context_release_complete(cu_notifier.f1ap_ul_msgs.rbegin()->second, msg));
}

/// Test: While UE Context Release is ongoing (i.e. RRC Release has been sent, the UE may have ACKed it, but it has
/// not yet shut down), ensure that:
/// - space is still reserved for the UE periodic resources (e.g. PUCCH for CSI/ACK) and other UE grants do not
/// interfere with it;
/// - no new grants are scheduled for the UE being removed.
TEST_F(du_high_tester,
       while_ue_context_release_is_on_going_then_grants_are_not_scheduled_and_ue_periodic_resources_are_still_scheduled)
{
  // EVENT: Create two UEs.
  rnti_t rnti1 = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti1));
  ASSERT_TRUE(run_rrc_setup(rnti1));
  ASSERT_TRUE(run_ue_context_setup(rnti1));
  rnti_t rnti2 = to_rnti(0x4602);
  ASSERT_TRUE(add_ue(rnti2));
  ASSERT_TRUE(run_rrc_setup(rnti2));
  ASSERT_TRUE(run_ue_context_setup(rnti2));

  // EVENT: CU-UP forwards many DRB PDUs to both UEs.
  const unsigned nof_pdcp_pdus = 100, pdcp_pdu_size = 128;
  push_pdcp_pdus_to_all_f1u_bearers(nof_pdcp_pdus, pdcp_pdu_size);

  // Sanity Check: Ensure DRB is active by verifying that some DRB PDUs are scheduled.
  EXPECT_TRUE(this->run_until([this, rnti1]() {
    return phy.cells[0].last_dl_res.has_value() and
           find_ue_pdsch_with_lcid(rnti1, LCID_MIN_DRB, phy.cells[0].last_dl_res->dl_res.ue_grants()) != nullptr;
  }));

  // EVENT: DU receives F1AP UE Context Release Command (Containing RRC Release).
  cu_notifier.f1ap_ul_msgs.clear();
  ues.at(rnti1).sim->pop_pending_dl_mac_sdus();
  f1ap_message msg = generate_ue_context_release_command();
  this->du_hi->get_f1ap_pdu_handler().handle_message(msg);

  // TEST CASE: Ensure that SRB1 (RRC Release) is scheduled.
  this->test_logger.info("STATUS: UEContextReleaseCommand received by DU. Waiting for rrcRelease being transmitted...");
  unsigned srb1_bytes     = msg.pdu.init_msg().value.ue_context_release_cmd()->rrc_container.size();
  auto     srb1_scheduled = [this, rnti1, &srb1_bytes]() mutable {
    if (not phy.cells[0].last_dl_res.has_value()) {
      return srb1_bytes == 0;
    }
    auto* pdsch = find_ue_pdsch_with_lcid(rnti1, LCID_SRB1, phy.cells[0].last_dl_res->dl_res.ue_grants());
    if (pdsch != nullptr) {
      for (auto& lc : pdsch->tb_list[0].lc_chs_to_sched) {
        if (lc.lcid == LCID_SRB1) {
          srb1_bytes -= std::min(lc.sched_bytes, srb1_bytes);
        }
      }
    }
    return srb1_bytes == 0;
  };
  EXPECT_TRUE(this->run_until(srb1_scheduled));
  ASSERT_TRUE(this->run_until([this, rnti1]() {
    auto pdus = ues.at(rnti1).sim->pop_pending_dl_mac_sdus();
    return std::any_of(pdus.begin(), pdus.end(), [](const auto& pair) { return pair.first == LCID_SRB1; });
  }));
  this->test_logger.info("STATUS: UE received RRC Release");

  // UE2 may have drained its DRB buffer while the RRC Release was being scheduled.
  push_pdcp_pdus_to_ue_f1u_bearers(to_du_ue_index(1), nof_pdcp_pdus, pdcp_pdu_size);

  // While UE Context Release Complete has not been sent...
  unsigned ue2_pdsch_count = 0;
  unsigned ue1_csi_count   = 0;
  unsigned ue1_sr_count    = 0;
  for (unsigned count = 0, max_count = 1000; count < max_count and cu_notifier.f1ap_ul_msgs.empty(); ++count) {
    run_slot();

    if (phy.cells[0].last_dl_res.has_value()) {
      const auto dl_res = phy.cells[0].last_dl_res->dl_res;

      // TEST CASE: Ensure no PDSCH/PUSCH grants are scheduled for UE1.
      ASSERT_EQ(find_ue_dl_pdcch(rnti1, dl_res), nullptr)
          << "New DL grants were scheduled for UE1 while its release was ongoing";
      ASSERT_EQ(find_ue_ul_pdcch(rnti1, dl_res), nullptr)
          << "New UL grants were scheduled for UE1 while its release was ongoing";

      // TEST CASE: Ensure that UE2 activity is unaffected.
      if (find_ue_pdsch_with_lcid(rnti2, LCID_MIN_DRB, dl_res.ue_grants()) != nullptr) {
        ue2_pdsch_count++;
      }
    }

    if (not phy.cells[0].last_ul_res.has_value()) {
      continue;
    }
    const span<const pucch_info> pucchs = phy.cells[0].last_ul_res->ul_res.pucchs().unsorted();

    // TEST CASE: Ensure UE1 UCI periodic resources are still scheduled.
    if (find_ue_pucch_with_csi(rnti1, pucchs) != nullptr) {
      ue1_csi_count++;
    }
    auto* pucch = find_ue_pucch_with_sr(rnti1, pucchs);
    if (pucch != nullptr) {
      // Push SR indication to MAC. It should be ignored.
      mac_uci_indication_message uci;
      uci.sl_rx = next_slot.without_hyper_sfn() - 1;
      uci.ucis.push_back(test_helpers::create_uci_pdu(*pucch, true));
      du_hi->get_control_info_handler(to_du_cell_index(0)).handle_uci(uci);
      ue1_sr_count++;
    }
  }
  ASSERT_FALSE(cu_notifier.f1ap_ul_msgs.empty()) << "UE Context Release Complete was not sent within the expected time";
  ASSERT_GT(ue2_pdsch_count, 0) << "No periodic PDSCH resources were scheduled for UE2";
  ASSERT_GT(ue1_csi_count, 0) << "No periodic CSI resources were scheduled for UE1 while its release was ongoing";
  ASSERT_GT(ue1_sr_count, 0) << "No periodic SR resources were scheduled for UE1 while its release was ongoing";
}

TEST_F(du_high_tester, when_f1ap_reset_received_then_ues_are_removed)
{
  // Create UE.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));
  ASSERT_TRUE(run_ue_context_setup(rnti));

  // EVENT: CU-UP forwards many DRB PDUs.
  const unsigned nof_pdcp_pdus = 100, pdcp_pdu_size = 128;
  push_pdcp_pdus_to_all_f1u_bearers(nof_pdcp_pdus, pdcp_pdu_size);

  // EVENT: DU receives F1 RESET.
  cu_notifier.f1ap_ul_msgs.clear();
  f1ap_message msg = test_helpers::generate_f1ap_reset_message();
  this->du_hi->get_f1ap_pdu_handler().handle_message(msg);
  this->test_logger.info("STATUS: RESET received by DU. Waiting for F1AP RESET ACK...");

  // Wait for F1 RESET ACK to be sent to the CU.
  EXPECT_TRUE(this->run_until([this]() { return not cu_notifier.f1ap_ul_msgs.empty(); }));

  // TEST CASE: Ensure F1 RESET ACK is valid.
  ASSERT_TRUE(test_helpers::is_valid_f1_reset_ack(msg, cu_notifier.f1ap_ul_msgs.rbegin()->second));

  // TEST CASE: Confirm that no traffic is being sent for the reset UE.
  const unsigned nof_slots_check = 100;
  ASSERT_FALSE(this->run_until(
      [this, rnti]() {
        if (not phy.cells[0].last_dl_res.has_value()) {
          return false;
        }
        const dl_msg_alloc* pdsch = find_ue_pdsch(rnti, phy.cells[0].last_dl_res->dl_res.ue_grants());
        return pdsch != nullptr;
      },
      nof_slots_check));
}

TEST_F(du_high_tester, when_ue_removal_races_with_cell_stop_reset_sweep_then_du_does_not_crash)
{
  // Create UE.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));
  ASSERT_TRUE(run_ue_context_setup(rnti));

  auto& u = ues.at(rnti);

  // EVENT: CU-CP removes the (only) cell served by this DU via gNB-CU Configuration Update. This makes the DU
  // start du_cell_stop_procedure::await_cu_to_release_ues(), which asks the CU to release the UE and then polls
  // every 10ms, for up to 500ms, before giving up on the CU and forcing the UE removal itself.
  cu_notifier.f1ap_ul_msgs.clear();
  std::vector<nr_cell_global_id_t> cgis_to_deactivate = {
      nr_cell_global_id_t{plmn_identity::test_value(), nr_cell_identity::create(0).value()}};
  f1ap_message cu_cfg_update = test_helpers::generate_gnb_cu_configuration_update_request(0, {}, cgis_to_deactivate);
  this->du_hi->get_f1ap_pdu_handler().handle_message(cu_cfg_update);

  // TEST CASE: DU asks the CU to release the UE.
  ASSERT_TRUE(this->run_until([this]() { return not cu_notifier.f1ap_ul_msgs.empty(); }));
  ASSERT_TRUE(
      test_helpers::is_valid_ue_context_release_request(cu_notifier.f1ap_ul_msgs.rbegin()->second, *u.du_ue_id));

  // Deliberately do not respond to the UE CONTEXT RELEASE REQUEST for several slots, to race the UE CONTEXT RELEASE
  // COMMAND with the gNB-CU Configuration Update procedure forced UE removal.
  const unsigned slots_before_ue_release_cmd = test_rng::uniform_int(0, 500);
  for (unsigned i = 0; i != slots_before_ue_release_cmd; ++i) {
    run_slot();
  }

  // EVENT: Independently of the above, and racing with it, the CU decides to release the UE directly via a UE
  // CONTEXT RELEASE COMMAND. This starts a *separate* UE deletion procedure for the same UE, which occupies the
  // UE's own task queue until it completes. Unless the UE has already been force-removed by the cell-stop
  // procedure above, in which case the DU discards the command (unrecognized gNB-DU UE F1AP ID) and no UE CONTEXT
  // RELEASE COMPLETE is sent back. Both outcomes are valid, depending on how the race above resolved.
  cu_notifier.f1ap_ul_msgs.clear();
  f1ap_message rel_cmd = test_helpers::generate_ue_context_release_command(*u.cu_ue_id, *u.du_ue_id, srb_id_t::srb1);
  this->du_hi->get_f1ap_pdu_handler().handle_message(rel_cmd);

  // EVENT: The gNB-CU Configuration Update always eventually completes, regardless of how the race
  // above resolved, so waiting for its acknowledgement is a safe, non-flaky way to know the DU has settled.
  ASSERT_TRUE(this->run_until([this]() {
    for (const auto& [msg_id, ul_msg] : cu_notifier.f1ap_ul_msgs) {
      if (test_helpers::is_valid_gnb_cu_configuration_update_acknowledge(ul_msg)) {
        return true;
      }
    }
    return false;
  }));
}

TEST_F(du_high_tester, when_du_high_is_stopped_then_ues_are_removed)
{
  // Create UE.
  ASSERT_TRUE(add_ue(to_rnti(0x4601)));

  // Stop DU from another thread, because stop() call is blocking.
  std::atomic<bool> running{true};
  task_worker       worker("phy_worker", 2048);
  bool              ret = worker.push_task([this, &running]() {
    this->du_hi->stop();
    running = false;
  });
  ASSERT_TRUE(ret);

  // Keep running slot indications until DU-high is stopped.
  for (unsigned count = 0; count < 1000 and running; ++count) {
    this->run_slot();
  }
  ASSERT_FALSE(running) << "stop() call is hanging";
}

TEST_F(du_high_tester, when_ue_context_setup_received_for_inexistent_ue_then_ue_is_created)
{
  cu_notifier.f1ap_ul_msgs.clear();

  gnb_cu_ue_f1ap_id_t cu_ue_id =
      int_to_gnb_cu_ue_f1ap_id(test_rng::uniform_int<uint64_t>(0, (uint64_t)gnb_cu_ue_f1ap_id_t::max));
  f1ap_message cu_cp_msg = test_helpers::generate_ue_context_setup_request(
      cu_ue_id, std::nullopt, 0, {drb_id_t::drb1}, {plmn_identity::test_value(), nr_cell_identity::create(0).value()});
  this->du_hi->get_f1ap_pdu_handler().handle_message(cu_cp_msg);

  ASSERT_TRUE(this->run_until([this]() { return not cu_notifier.f1ap_ul_msgs.empty(); }));

  ASSERT_EQ(cu_notifier.f1ap_ul_msgs.size(), 1);
  ASSERT_TRUE(test_helpers::is_valid_ue_context_setup_response(cu_notifier.f1ap_ul_msgs.begin()->second, cu_cp_msg));
  auto& resp = cu_notifier.f1ap_ul_msgs.rbegin()->second.pdu.successful_outcome().value.ue_context_setup_resp();
  ASSERT_TRUE(resp->c_rnti_present);
}

TEST_F(du_high_tester, when_ue_context_modification_with_rem_drbs_is_received_then_drbs_are_removed)
{
  // Create UE.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));
  ASSERT_TRUE(run_ue_context_setup(rnti));

  // CU-UP forwards many DRB PDUs.
  const unsigned nof_pdcp_pdus = 100, pdcp_pdu_size = 128;
  push_pdcp_pdus_to_all_f1u_bearers(nof_pdcp_pdus, pdcp_pdu_size);

  // DU receives F1AP UE Context Modification Command.
  cu_notifier.f1ap_ul_msgs.clear();
  auto&        u   = ues[rnti];
  f1ap_message msg = test_helpers::generate_ue_context_modification_request(
      u.du_ue_id.value(), u.cu_ue_id.value(), {}, {}, {drb_id_t::drb1}, {});
  this->du_hi->get_f1ap_pdu_handler().handle_message(msg);

  // Wait for DU to send F1AP UE Context Modification Response.
  this->run_until([this]() { return not cu_notifier.f1ap_ul_msgs.empty(); });

  // Test Case: Check that the F1AP UE Context Modification Response is valid.
  const f1ap_message& f1ap_pdu = cu_notifier.f1ap_ul_msgs.rbegin()->second;
  ASSERT_EQ(f1ap_pdu.pdu.type().value, f1ap_pdu_c::types::options::successful_outcome);
  ASSERT_EQ(f1ap_pdu.pdu.successful_outcome().proc_code, ASN1_F1AP_ID_UE_CONTEXT_MOD);
  const ue_context_mod_resp_s& resp = f1ap_pdu.pdu.successful_outcome().value.ue_context_mod_resp();
  ASSERT_TRUE(resp->du_to_cu_rrc_info_present);
  ASSERT_FALSE(resp->du_to_cu_rrc_info.cell_group_cfg.empty());
  {
    asn1::cbit_ref                 bref{resp->du_to_cu_rrc_info.cell_group_cfg};
    asn1::rrc_nr::cell_group_cfg_s cell_grp_cfg;
    ASSERT_EQ(cell_grp_cfg.unpack(bref), asn1::OCUDUASN_SUCCESS);
    ASSERT_EQ(cell_grp_cfg.rlc_bearer_to_release_list.size(), 1);
    ASSERT_EQ(cell_grp_cfg.rlc_bearer_to_release_list[0], 4) << "DRB1 with LCID=4 should have been removed";
  }
}

TEST_F(du_high_tester, when_dl_rrc_message_with_old_du_ue_id_received_then_old_ue_traffic_stops)
{
  OCUDU_TEST_REQUIREMENTS("CU-GEN-3");

  // Create UE1.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));
  ASSERT_TRUE(run_ue_context_setup(rnti));

  // CU-UP forwards many DRB PDUs.
  const unsigned nof_pdcp_pdus = 100, pdcp_pdu_size = 32;
  push_pdcp_pdus_to_all_f1u_bearers(nof_pdcp_pdus, pdcp_pdu_size);

  // Send DL RRC Message Transfer with old gNB-DU-UE-F1AP-ID.
  rnti_t rnti2 = to_rnti(0x4602);
  ASSERT_TRUE(add_ue(rnti2));
  ASSERT_TRUE(run_rrc_reestablishment(rnti2, rnti, reestablishment_stage::reest_complete));

  ASSERT_FALSE(this->run_until(
      [this, rnti]() {
        if (not phy.cells[0].last_dl_res.has_value()) {
          return false;
        }
        const dl_msg_alloc* pdsch = find_ue_pdsch(rnti, phy.cells[0].last_dl_res->dl_res.ue_grants());
        if (pdsch != nullptr and pdsch->pdsch_cfg.codewords[0].new_data) {
          return true;
        }
        return false;
      },
      100));
}

TEST_F(du_high_tester, when_dl_rrc_message_with_old_du_ue_id_received_then_drbs_are_reestablished)
{
  OCUDU_TEST_REQUIREMENTS("CU-GEN-3");

  // Create UE1.
  rnti_t rnti1 = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti1));
  ASSERT_TRUE(run_rrc_setup(rnti1));
  ASSERT_TRUE(run_ue_context_setup(rnti1));

  // Run Reestablishment
  rnti_t rnti2 = to_rnti(0x4602);
  ASSERT_TRUE(add_ue(rnti2));
  ASSERT_TRUE(run_rrc_reestablishment(rnti2, rnti1));

  // Check that DRBs are active for the new C-RNTI.
  const unsigned nof_pdcp_pdus = 10, pdcp_pdu_size = 128;
  for (unsigned i = 0; i < nof_pdcp_pdus; ++i) {
    nru_dl_message f1u_pdu{
        .t_pdu = test_helpers::create_pdcp_pdu(pdcp_sn_size::size12bits, /* is_srb = */ false, i, pdcp_pdu_size, i)};
    cu_up_sim.bearers.at(std::make_pair(1, drb_id_t::drb1)).rx_notifier->on_new_pdu(f1u_pdu);
  }
  ASSERT_TRUE(this->run_until(
      [this, rnti2]() {
        return phy.cells[0].last_dl_res.has_value() and
               find_ue_pdsch_with_lcid(rnti2, LCID_MIN_DRB, phy.cells[0].last_dl_res->dl_res.ue_grants()) != nullptr;
      },
      100));
}

TEST_F(du_high_tester,
       when_dl_rrc_message_with_old_du_ue_id_received_but_same_cu_ue_f1ap_id_then_cu_ue_f1ap_id_is_reused)
{
  OCUDU_TEST_REQUIREMENTS("CU-GEN-3");

  // Create UE1.
  rnti_t rnti1 = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti1));
  ASSERT_TRUE(run_rrc_setup(rnti1));
  ASSERT_TRUE(run_ue_context_setup(rnti1));

  // Run Reestablishment, keeping the same gNB-CU-UE-F1AP-ID.
  this->next_cu_ue_id--;
  rnti_t rnti2 = to_rnti(0x4602);
  ASSERT_TRUE(add_ue(rnti2));
  ASSERT_TRUE(run_rrc_reestablishment(rnti2, rnti1));

  // Check that DRBs are active for the new C-RNTI.
  const unsigned nof_pdcp_pdus = 10, pdcp_pdu_size = 128;
  for (unsigned i = 0; i < nof_pdcp_pdus; ++i) {
    nru_dl_message f1u_pdu{
        .t_pdu = test_helpers::create_pdcp_pdu(pdcp_sn_size::size12bits, /* is_srb = */ false, i, pdcp_pdu_size, i)};
    cu_up_sim.bearers.at(std::make_pair(1, drb_id_t::drb1)).rx_notifier->on_new_pdu(f1u_pdu);
  }
  ASSERT_TRUE(this->run_until(
      [this, rnti2]() {
        return phy.cells[0].last_dl_res.has_value() and
               find_ue_pdsch_with_lcid(rnti2, LCID_MIN_DRB, phy.cells[0].last_dl_res->dl_res.ue_grants()) != nullptr;
      },
      100));
}

TEST_F(du_high_tester, when_reestablishment_takes_place_then_previous_ue_capabilities_are_considered_in_config)
{
  OCUDU_TEST_REQUIREMENTS("CU-GEN-3");

  // Create UE1.
  rnti_t rnti1 = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti1));
  ASSERT_TRUE(run_rrc_setup(rnti1));
  ASSERT_TRUE(run_ue_context_setup(rnti1));

  // CU-UP forwards many DRB PDUs.
  const unsigned nof_pdcp_pdus = 100, pdcp_pdu_size = 32;
  push_pdcp_pdus_to_all_f1u_bearers(nof_pdcp_pdus, pdcp_pdu_size);

  // Check QAM256 MCS table is used.
  const unsigned nof_slots_test = 1000;
  ASSERT_TRUE(this->run_until(
      [this, rnti1]() {
        if (not phy.cells[0].last_dl_res.has_value()) {
          return false;
        }
        auto* pdsch = find_ue_pdsch_with_lcid(rnti1, LCID_MIN_DRB, phy.cells[0].last_dl_res->dl_res.ue_grants());
        return pdsch != nullptr and pdsch->pdsch_cfg.mcs_table == pdsch_mcs_table::qam256;
      },
      nof_slots_test));

  // Run Reestablishment
  rnti_t rnti2 = to_rnti(0x4602);
  ASSERT_TRUE(add_ue(rnti2));
  ASSERT_TRUE(run_rrc_reestablishment(rnti2, rnti1));

  for (unsigned i = 0; i < nof_pdcp_pdus; ++i) {
    nru_dl_message f1u_pdu{
        .t_pdu = test_helpers::create_pdcp_pdu(pdcp_sn_size::size12bits, /* is_srb = */ false, i, pdcp_pdu_size, i)};
    cu_up_sim.bearers.at(std::make_pair(1, drb_id_t::drb1)).rx_notifier->on_new_pdu(f1u_pdu);
  }

  // Check QAM256 MCS table is used after RRC Reestablishment (the UE capabilities were not forgotten).
  ASSERT_TRUE(this->run_until(
      [this, rnti2]() {
        if (not phy.cells[0].last_dl_res.has_value()) {
          return false;
        }
        auto* pdsch = find_ue_pdsch_with_lcid(rnti2, LCID_MIN_DRB, phy.cells[0].last_dl_res->dl_res.ue_grants());
        return pdsch != nullptr and pdsch->pdsch_cfg.mcs_table == pdsch_mcs_table::qam256;
      },
      nof_slots_test));
}

class du_high_rrm_policy_tester : public du_high_env_simulator, public testing::Test
{
public:
  du_high_rrm_policy_tester(plmn_identity plmn_id_ = plmn_identity{mobile_country_code::from_string("001").value(),
                                                                   mobile_network_code::from_string("01").value()},
                            s_nssai_t     s_nssai_ = s_nssai_t{.sst = slice_service_type{1}}) :
    du_high_env_simulator([plmn_id_, s_nssai_]() {
      du_high_configuration du_cfg = create_du_high_configuration();
      du_cfg.ran.cells[0].rrm_policy_members.emplace_back(
          slice_rrm_policy_config{.rrc_member = rrm_policy_member{.plmn_id = plmn_id_, .s_nssai = s_nssai_}});
      return du_cfg;
    }()),
    plmn_id(plmn_id_),
    s_nssai(s_nssai_)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-17-3-e");
  }

  void apply_rrm_reconfiguration(unsigned max_rbs, unsigned min_rbs)
  {
    error_type<std::string> cmd_exec_res = test_helpers::apply_rrm_policy_reconfiguration(
        this->du_hi->get_du_configurator(), plmn_id, s_nssai, min_rbs, max_rbs);
    ASSERT_TRUE(cmd_exec_res.has_value()) << cmd_exec_res.error();
  }

  void push_dl_pdus()
  {
    const unsigned nof_pdcp_pdus = 100, pdcp_pdu_size = 128;
    for (unsigned i = 0; i < nof_pdcp_pdus; ++i) {
      nru_dl_message f1u_pdu{
          .t_pdu = test_helpers::create_pdcp_pdu(pdcp_sn_size::size12bits, /* is_srb = */ false, i, pdcp_pdu_size, i)};
      cu_up_sim.bearers.begin()->second.rx_notifier->on_new_pdu(f1u_pdu);
    }
  }

protected:
  plmn_identity plmn_id;
  /// Single Network Slice Selection Assistance Information (S-NSSAI).
  s_nssai_t s_nssai;
};

TEST_F(du_high_rrm_policy_tester, when_rrm_policy_remote_command_is_received_then_it_is_handled)
{
  // Create UE.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));
  ASSERT_TRUE(run_ue_context_setup(rnti));

  // Ensure DU<->CU-UP tunnel was created.
  ASSERT_EQ(cu_up_sim.last_drb_id.value(), drb_id_t::drb1);

  unsigned max_rbs = 30;
  unsigned min_rbs = 5;

  auto run_sched_and_assess_rbs = [this](unsigned max_slice_rbs) {
    phy.cells[0].last_dl_data.reset();
    constexpr unsigned nof_slots_to_assess_rrm_cfg = 10;
    for (unsigned sl = 0; sl != nof_slots_to_assess_rrm_cfg; ++sl) {
      run_slot();

      if (phy.cells[0].last_dl_res.has_value()) {
        unsigned   rbs       = 0;
        const auto ue_grants = phy.cells[0].last_dl_res->dl_res.ue_grants();
        for (const auto& ue_grant : ue_grants) {
          rbs += ue_grant.pdsch_cfg.rbs.type1().length();
        }
        ASSERT_LT(rbs, max_slice_rbs);
      }
      phy.cells[0].last_dl_data.reset();
    }
  };

  // 1. Apply RRM config first.
  apply_rrm_reconfiguration(max_rbs, min_rbs);

  // 1-A. Run the scheduler for a few slots, to clear previous allocations.
  run_until([]() { return false; }, 10);

  // 1-B. Forward several DRB PDUs.
  push_dl_pdus();

  // 1-C. Run the scheduler and assess RBs are according to the RRM config.
  run_sched_and_assess_rbs(max_rbs);

  // 2. Apply new RRM config.
  max_rbs = 100;
  min_rbs = 10;
  apply_rrm_reconfiguration(max_rbs, min_rbs);

  // 2-A Run the scheduler for a few slots, to clear previous allocations.
  run_until([]() { return false; }, 10);

  // 2-B. Forward several DRB PDUs.
  push_dl_pdus();

  // 2-C. Run the scheduler and assess RBs are according to the RRM config.
  run_sched_and_assess_rbs(max_rbs);
}

class du_high_cg_tester : public du_high_env_simulator, public testing::Test
{
protected:
  du_high_cg_tester() :
    du_high_env_simulator([]() {
      du_high_configuration du_cfg            = create_du_high_configuration();
      du_cfg.ran.cells[0].ran.init_bwp.cg_cfg = cg_builder_params{};
      return du_cfg;
    }())
  {
  }
};

TEST_F(du_high_cg_tester, when_ue_context_setup_with_drb_and_cg_enabled_then_cs_rnti_is_allocated)
{
  // Create UE.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));

  auto& u = ues[rnti];

  // DU receives UE Context Setup Request (inlined from run_ue_context_setup to capture the response).
  cu_notifier.f1ap_ul_msgs.clear();
  f1ap_message msg = test_helpers::generate_ue_context_setup_request(
      *u.cu_ue_id,
      u.du_ue_id,
      u.srbs[LCID_SRB1].next_pdcp_sn++,
      {drb_id_t::drb1},
      {plmn_identity::test_value(), nr_cell_identity::create(0).value()});
  asn1::f1ap::ue_context_setup_request_s& cmd      = msg.pdu.init_msg().value.ue_context_setup_request();
  auto&                                   drb_item = cmd->drbs_to_be_setup_list[0].value().drbs_to_be_setup_item();
  auto&                                   drb_info = drb_item.qos_info.choice_ext().value().drb_info();
  drb_info.drb_qos.qos_characteristics.non_dyn_5qi().five_qi = 7U;
  drb_item.rlc_mode.value                                    = asn1::f1ap::rlc_mode_opts::rlc_um_bidirectional;
  // UE supports 256QAM.
  cmd->cu_to_du_rrc_info.ue_cap_rat_container_list =
      byte_buffer::create({0x10, 0xc9, 0x83, 0x40, 0x67, 0x40, 0x8e, 0x8c, 0xb4, 0x04, 0xbf, 0x1b, 0x07, 0x0a, 0x40,
                           0x00, 0x2c, 0x12, 0x62, 0xe0, 0x00, 0x30, 0x7e, 0x16, 0x00, 0x31, 0xbf, 0xf0, 0x01, 0x70,
                           0x00, 0x98, 0xa0, 0x51, 0x00, 0x00, 0x00, 0x04, 0x40, 0x02, 0xc6, 0x80, 0x30, 0x03, 0xd0,
                           0x38, 0xf2, 0x1c, 0xf8, 0x00, 0x01, 0x20, 0x20, 0x07, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x08,
                           0x05, 0x03, 0x60, 0x00, 0x03, 0x80, 0x40, 0x4a, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                           0x39, 0x51, 0x40, 0x00, 0x40, 0x00, 0x02, 0x59, 0x65, 0x40, 0x0d, 0x2a, 0xaa, 0x1e, 0x00,
                           0x0e, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x20, 0x48, 0x14})
          .value();
  this->du_hi->get_f1ap_pdu_handler().handle_message(msg);

  // Wait until DU sends UE Context Setup Response and the whole RRC container is scheduled.
  constexpr unsigned MAX_SLOT_COUNT   = 1000;
  const unsigned     srb1_pdu_size    = cmd->rrc_container.size();
  unsigned           srb1_bytes_sched = 0;
  for (unsigned i = 0; i != MAX_SLOT_COUNT and (srb1_bytes_sched < srb1_pdu_size or cu_notifier.f1ap_ul_msgs.empty());
       ++i) {
    run_slot();

    if (not phy.cells[0].last_dl_res.has_value()) {
      continue;
    }
    const dl_msg_alloc* pdsch = find_ue_pdsch_with_lcid(rnti, LCID_SRB1, phy.cells[0].last_dl_res->dl_res.ue_grants());
    if (pdsch != nullptr) {
      for (const dl_msg_lc_info& lc_grant : pdsch->tb_list[0].lc_chs_to_sched) {
        if (lc_grant.lcid == LCID_SRB1) {
          srb1_bytes_sched += lc_grant.sched_bytes;
        }
      }
    }
  }
  ASSERT_EQ(cu_notifier.f1ap_ul_msgs.size(), 1);
  ASSERT_TRUE(test_helpers::is_valid_ue_context_setup_response(cu_notifier.f1ap_ul_msgs.rbegin()->second, msg));

  // Decode CellGroupConfig from the UE Context Setup Response and verify CS-RNTI is allocated.
  const auto& resp = cu_notifier.f1ap_ul_msgs.rbegin()->second.pdu.successful_outcome().value.ue_context_setup_resp();
  ASSERT_FALSE(resp->du_to_cu_rrc_info.cell_group_cfg.empty());
  {
    asn1::cbit_ref                 bref{resp->du_to_cu_rrc_info.cell_group_cfg};
    asn1::rrc_nr::cell_group_cfg_s cell_grp_cfg;
    ASSERT_EQ(cell_grp_cfg.unpack(bref), asn1::OCUDUASN_SUCCESS);
    ASSERT_TRUE(cell_grp_cfg.phys_cell_group_cfg_present) << "PhysCellGroupConfig should be present";
    ASSERT_TRUE(cell_grp_cfg.phys_cell_group_cfg.cs_rnti_present) << "CS-RNTI should be allocated when CG is enabled";
    ASSERT_EQ(cell_grp_cfg.phys_cell_group_cfg.cs_rnti.type(), asn1::setup_release_opts::setup)
        << "CS-RNTI should be set to 'setup'";
  }

  // Complete Reconfiguration Complete.
  cu_notifier.f1ap_ul_msgs.clear();
  u.sim->enqueue_ul_mac_sdu(LCID_SRB1, byte_buffer::create({0x1, 0x2, 0x3}).value());
  ASSERT_TRUE(run_until([this]() { return not cu_notifier.f1ap_ul_msgs.empty(); }));
  ASSERT_TRUE(test_helpers::is_ul_rrc_msg_transfer_valid(cu_notifier.f1ap_ul_msgs.rbegin()->second, srb_id_t::srb1));
}

TEST_F(du_high_cg_tester, when_all_drbs_removed_then_cs_rnti_is_deallocated)
{
  // Create UE with DRB.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));
  ASSERT_TRUE(run_ue_context_setup(rnti));

  // DU receives UE Context Modification removing DRB1.
  cu_notifier.f1ap_ul_msgs.clear();
  auto&        u   = ues[rnti];
  f1ap_message msg = test_helpers::generate_ue_context_modification_request(
      u.du_ue_id.value(), u.cu_ue_id.value(), {}, {}, {drb_id_t::drb1}, {});
  this->du_hi->get_f1ap_pdu_handler().handle_message(msg);

  // Wait for UE Context Modification Response.
  ASSERT_TRUE(this->run_until([this]() { return not cu_notifier.f1ap_ul_msgs.empty(); }));

  // Decode CellGroupConfig and verify CS-RNTI is deallocated.
  const f1ap_message& f1ap_pdu = cu_notifier.f1ap_ul_msgs.rbegin()->second;
  ASSERT_EQ(f1ap_pdu.pdu.type().value, asn1::f1ap::f1ap_pdu_c::types::options::successful_outcome);
  ASSERT_EQ(f1ap_pdu.pdu.successful_outcome().proc_code, ASN1_F1AP_ID_UE_CONTEXT_MOD);
  const asn1::f1ap::ue_context_mod_resp_s& resp = f1ap_pdu.pdu.successful_outcome().value.ue_context_mod_resp();
  ASSERT_TRUE(resp->du_to_cu_rrc_info_present);
  ASSERT_FALSE(resp->du_to_cu_rrc_info.cell_group_cfg.empty());
  {
    asn1::cbit_ref                 bref{resp->du_to_cu_rrc_info.cell_group_cfg};
    asn1::rrc_nr::cell_group_cfg_s cell_grp_cfg;
    ASSERT_EQ(cell_grp_cfg.unpack(bref), asn1::OCUDUASN_SUCCESS);
    ASSERT_TRUE(cell_grp_cfg.phys_cell_group_cfg_present) << "PhysCellGroupConfig should be present";
    ASSERT_TRUE(cell_grp_cfg.phys_cell_group_cfg.cs_rnti_present)
        << "CS-RNTI field should be present when CS-RNTI is deallocated";
    ASSERT_EQ(cell_grp_cfg.phys_cell_group_cfg.cs_rnti.type(), asn1::setup_release_opts::release)
        << "CS-RNTI should be released when all DRBs are removed";
  }
}

// A CG configuration that the scheduler rejects for the UE, which the app config validator would have refused at
// startup: fewer CG HARQ processes than the configured grant timer.
class du_high_invalid_cg_tester : public du_high_env_simulator, public testing::Test
{
protected:
  du_high_invalid_cg_tester() :
    du_high_env_simulator([]() {
      du_high_configuration du_cfg            = create_du_high_configuration();
      du_cfg.ran.cells[0].ran.init_bwp.cg_cfg = cg_builder_params{.nof_harq_processes = 2};
      return du_cfg;
    }())
  {
  }
};

TEST_F(du_high_invalid_cg_tester, when_scheduler_rejects_the_cg_config_then_ue_context_setup_fails)
{
  // Create UE.
  rnti_t rnti = to_rnti(0x4601);
  ASSERT_TRUE(add_ue(rnti));
  ASSERT_TRUE(run_rrc_setup(rnti));

  auto& u = ues[rnti];

  // DU receives UE Context Setup Request with a DRB, which gives the UE its CG.
  cu_notifier.f1ap_ul_msgs.clear();
  f1ap_message msg = test_helpers::generate_ue_context_setup_request(
      *u.cu_ue_id,
      u.du_ue_id,
      u.srbs[LCID_SRB1].next_pdcp_sn++,
      {drb_id_t::drb1},
      {plmn_identity::test_value(), nr_cell_identity::create(0).value()});
  asn1::f1ap::ue_context_setup_request_s& cmd      = msg.pdu.init_msg().value.ue_context_setup_request();
  auto&                                   drb_item = cmd->drbs_to_be_setup_list[0].value().drbs_to_be_setup_item();
  auto&                                   drb_info = drb_item.qos_info.choice_ext().value().drb_info();
  drb_info.drb_qos.qos_characteristics.non_dyn_5qi().five_qi = 7U;
  drb_item.rlc_mode.value                                    = asn1::f1ap::rlc_mode_opts::rlc_um_bidirectional;
  this->du_hi->get_f1ap_pdu_handler().handle_message(msg);

  // The scheduler rejects the UE reconfiguration, so the DU must report a failure rather than a DRB it never schedules.
  ASSERT_TRUE(run_until([this]() { return not cu_notifier.f1ap_ul_msgs.empty(); }));
  const f1ap_message& resp = cu_notifier.f1ap_ul_msgs.rbegin()->second;
  ASSERT_EQ(resp.pdu.type().value, asn1::f1ap::f1ap_pdu_c::types_opts::unsuccessful_outcome);
  ASSERT_EQ(resp.pdu.unsuccessful_outcome().value.type().value,
            asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::ue_context_setup_fail);
}
