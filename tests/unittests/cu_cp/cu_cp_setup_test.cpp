// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_cp_test_environment.h"
#include "test_helpers.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/f1ap/f1ap_test_message_validators.h"
#include "tests/test_doubles/f1ap/f1ap_test_messages.h"
#include "tests/test_doubles/ngap/ngap_test_message_validators.h"
#include "tests/test_doubles/rrc/rrc_test_messages.h"
#include "ocudu/adt/byte_buffer.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents_ue.h"
#include "ocudu/asn1/rrc_nr/dl_ccch_msg.h"
#include "ocudu/asn1/rrc_nr/dl_dcch_msg.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/ngap/ngap_message.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/support/error_handling.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;

class cu_cp_setup_test : public cu_cp_test_environment, public ::testing::Test
{
public:
  explicit cu_cp_setup_test(cu_cp_test_env_params prms = {}) : cu_cp_test_environment(std::move(prms))
  {
    // Run NG setup to completion.
    run_ng_setup();

    // Setup DU.
    auto ret = connect_new_du();
    EXPECT_TRUE(ret.has_value());
    du_idx = ret.value();
    EXPECT_TRUE(this->run_f1_setup(du_idx));

    // Setup CU-UP.
    ret = connect_new_cu_up();
    EXPECT_TRUE(ret.has_value());
    cu_up_idx = ret.value();
    EXPECT_TRUE(this->run_e1_setup(cu_up_idx));
  }

  [[nodiscard]] bool send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer(byte_buffer rrc_container = {})
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject Initial UL RRC Message and wait for F1AP DL RRC Message Transfer containing RRC Setup.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_init_ul_rrc_message_transfer(
        du_ue_f1ap_id, crnti, plmn_identity::test_value(), {}, std::move(rrc_container)));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive DL RRC Message Transfer");

    // Check if DL RRC Message Transfer (containing RRC Setup) is valid.
    report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer_with_msg4(f1ap_pdu),
                              "Invalid DL RRC Message Transfer");
    const auto& dl_rrc_msg = f1ap_pdu.pdu.init_msg().value.dl_rrc_msg_transfer();
    report_fatal_error_if_not(int_to_gnb_du_ue_f1ap_id(dl_rrc_msg->gnb_du_ue_f1ap_id) == du_ue_f1ap_id,
                              "DL RRC Message Transfer contains unexpected DU UE F1AP ID");
    report_fatal_error_if_not(int_to_srb_id(dl_rrc_msg->srb_id) == srb_id_t::srb0,
                              "DL RRC Message Transfer contains unexpected SRB ID");
    cu_ue_id = int_to_gnb_cu_ue_f1ap_id(dl_rrc_msg->gnb_cu_ue_f1ap_id);
    return true;
  }

  [[nodiscard]] bool send_f1_removal_request_and_await_f1_removal_response()
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject F1 Removal Request and wait for F1 Removal Response.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_f1_removal_request(0));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu), "Failed to receive F1 Removal Response");

    return true;
  }

  [[nodiscard]] bool send_initial_ul_rrc_message_without_rrc_container_and_await_ue_context_release_command()
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject Initial UL RRC Message without RRC container and wait for F1AP UE Context Release Command.
    f1ap_message init_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(du_ue_f1ap_id, crnti);
    init_rrc_msg.pdu.init_msg().value.init_ul_rrc_msg_transfer()->rrc_container.clear();
    get_du(du_idx).push_ul_pdu(init_rrc_msg);

    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive F1AP UE Context Release Command");
    report_fatal_error_if_not(f1ap_pdu.pdu.type() == asn1::f1ap::f1ap_pdu_c::types_opts::options::init_msg,
                              "Received unexpected F1AP PDU type");
    report_fatal_error_if_not(f1ap_pdu.pdu.init_msg().value.type().value ==
                                  asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::ue_context_release_cmd,
                              "Received unexpected F1AP Init Msg type");

    const auto& ue_rel = f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd();
    report_fatal_error_if_not(ue_rel->srb_id == 0, "Unexpected SRB ID");

    // Check that the UE Context Release command contains an RRC Reject.
    asn1::rrc_nr::dl_ccch_msg_s ccch;
    {
      asn1::cbit_ref bref{f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd()->rrc_container};
      report_fatal_error_if_not(ccch.unpack(bref) == asn1::OCUDUASN_SUCCESS, "Failed to unpack RRC container");
    }
    report_fatal_error_if_not(ccch.msg.c1().type().value ==
                                  asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types_opts::rrc_reject,
                              "Unexpected RRC message type");

    return true;
  }

  [[nodiscard]] bool send_initial_ul_rrc_message_and_await_ue_context_release_command()
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject Initial UL RRC Message and wait for F1AP UE Context Release Command containing RRC Reject.
    get_du(du_idx).push_ul_pdu(
        test_helpers::generate_init_ul_rrc_message_transfer(du_ue_f1ap_id, crnti, plmn_identity::test_value()));

    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive F1AP UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid F1AP UE Context Release Command");

    const auto& ue_rel = f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd();
    cu_ue_id           = int_to_gnb_cu_ue_f1ap_id(ue_rel->gnb_cu_ue_f1ap_id);
    report_fatal_error_if_not(ue_rel->rrc_container_present, "RRC container not present");
    report_fatal_error_if_not(ue_rel->srb_id_present, "SRB ID not present");
    report_fatal_error_if_not(ue_rel->srb_id == 0, "Unexpected SRB ID");

    asn1::rrc_nr::dl_ccch_msg_s ccch;
    {
      asn1::cbit_ref bref{ue_rel->rrc_container};
      report_fatal_error_if_not(ccch.unpack(bref) == asn1::OCUDUASN_SUCCESS, "Failed to unpack RRC container");
    }
    report_fatal_error_if_not(ccch.msg.c1().type().value ==
                                  asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types_opts::rrc_reject,
                              "Unexpected RRC message type");

    return true;
  }

  [[nodiscard]] bool
  send_initial_ul_rrc_message_without_du_to_cu_rrc_container_and_await_f1ap_ue_context_release_command()
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject Initial UL RRC Message without RRC container and wait for F1AP UE Context Release Command.
    f1ap_message init_rrc_msg =
        test_helpers::generate_init_ul_rrc_message_transfer_without_du_to_cu_container(du_ue_f1ap_id, crnti);
    get_du(du_idx).push_ul_pdu(init_rrc_msg);

    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive F1AP UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid F1AP UE Context Release Command");

    const auto& ue_rel = f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd();
    cu_ue_id           = int_to_gnb_cu_ue_f1ap_id(ue_rel->gnb_cu_ue_f1ap_id);
    report_fatal_error_if_not(ue_rel->rrc_container_present, "RRC container not present");
    // Check that the SRB ID is set if the RRC Container is included.
    report_fatal_error_if_not(ue_rel->srb_id_present, "SRB ID not present");
    report_fatal_error_if_not(ue_rel->srb_id == 0, "Unexpected SRB ID");

    // Check that the UE Context Release command contains an RRC Reject.
    asn1::rrc_nr::dl_ccch_msg_s ccch;
    {
      asn1::cbit_ref bref{f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd()->rrc_container};
      report_fatal_error_if_not(ccch.unpack(bref) == asn1::OCUDUASN_SUCCESS, "Failed to unpack RRC container");
    }
    report_fatal_error_if_not(ccch.msg.c1().type().value ==
                                  asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types_opts::rrc_reject,
                              "Unexpected RRC message type");

    return true;
  }

  [[nodiscard]] bool send_rrc_setup_complete_and_await_initial_ue_message()
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");

    // Inject UL RRC Message with RRC Setup Complete and wait for Initial UE message.
    f1ap_message ul_rrc_msg = test_helpers::generate_ul_rrc_message_transfer(
        du_ue_f1ap_id, cu_ue_id.value(), srb_id_t::srb1, generate_rrc_setup_complete());
    test_logger.info("Injecting UL RRC message (RRC Setup Complete)");
    get_du(du_idx).push_ul_pdu(ul_rrc_msg);

    // Verify AMF is notified of UE attach.
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive Initial UE message");
    report_fatal_error_if_not(test_helpers::is_valid_init_ue_message(ngap_pdu), "Invalid init UE message");
    return true;
  }

  [[nodiscard]] bool send_ul_rrc_message_transfer_and_await_f1ap_ue_context_release_command()
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // UE sends unexpected UL RRC Message Transfer with NAS Transport Message.
    f1ap_message ul_rrc_msg = test_helpers::generate_ul_rrc_message_transfer(
        du_ue_f1ap_id,
        gnb_cu_ue_f1ap_id_t::min,
        srb_id_t::srb1,
        make_byte_buffer("00003a0abf002b96882dac46355c4f34464ddaf7b43fde37ae8000000000").value());
    test_logger.info("Injecting unexpected UL RRC Message Transfer with UL NAS Transport message");
    get_du(du_idx).push_ul_pdu(ul_rrc_msg);

    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive F1AP UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid F1AP UE Context Release Command");

    const auto& ue_rel = f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd();
    cu_ue_id           = int_to_gnb_cu_ue_f1ap_id(ue_rel->gnb_cu_ue_f1ap_id);
    report_fatal_error_if_not(ue_rel->rrc_container_present, "RRC container not present");
    // Check that the SRB ID is set if the RRC Container is included.
    report_fatal_error_if_not(ue_rel->srb_id_present, "SRB ID not present");
    report_fatal_error_if_not(ue_rel->srb_id == 1, "Unexpected SRB ID");

    // Check that the UE Context Release command contains an RRC Reject.
    asn1::rrc_nr::dl_ccch_msg_s ccch;
    {
      asn1::cbit_ref bref{f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd()->rrc_container};
      report_fatal_error_if_not(ccch.unpack(bref) == asn1::OCUDUASN_SUCCESS, "Failed to unpack RRC container");
    }
    report_fatal_error_if_not(ccch.msg.c1().type().value ==
                                  asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types_opts::rrc_reject,
                              "Unexpected RRC message type");

    return true;
  }

  [[nodiscard]] bool send_f1ap_ue_context_release_request_and_await_f1ap_ue_context_release_command()
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // DU reports a UE-specific radio link failure while RRC Setup is still pending.
    test_logger.info("Injecting F1AP UE Context Release Request (DU-initiated, e.g. RLF)");
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_release_request(cu_ue_id.value(), du_ue_f1ap_id));

    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive F1AP UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid F1AP UE Context Release Command");

    return true;
  }

  [[nodiscard]] bool send_rrc_setup_complete_and_await_f1ap_ue_context_release_command()
  {
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");

    // Inject UL RRC Message with RRC Setup Complete and wait for F1AP UE Context Release Command.
    f1ap_message ul_rrc_msg = test_helpers::generate_ul_rrc_message_transfer(
        du_ue_f1ap_id,
        cu_ue_id.value(),
        srb_id_t::srb1,
        make_byte_buffer("00001000465f80105e400340667e483c3fc00000001826160b813c1c3c3c0000000000").value());
    test_logger.info("Injecting UL RRC message (RRC Setup Complete)");
    get_du(du_idx).push_ul_pdu(ul_rrc_msg);

    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive F1AP UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid F1AP UE Context Release Command");

    return true;
  }

  // Extracts the waitTime IE from the RRC Reject carried in the last received
  // F1AP UE context release command. Returns nullopt if the IE is absent.
  [[nodiscard]] std::optional<uint8_t> get_wait_time_from_last_rrc_reject()
  {
    const auto& ue_rel = f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd();
    report_fatal_error_if_not(ue_rel->rrc_container_present, "RRC container not present");

    // Check that the UE Context Release command contains an RRC Reject.
    asn1::rrc_nr::dl_ccch_msg_s ccch;
    {
      asn1::cbit_ref bref{f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd()->rrc_container};
      report_fatal_error_if_not(ccch.unpack(bref) == asn1::OCUDUASN_SUCCESS, "Failed to unpack RRC container");
    }
    report_fatal_error_if_not(ccch.msg.c1().type().value ==
                                  asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types_opts::rrc_reject,
                              "Unexpected RRC message type");

    const auto& reject = ccch.msg.c1().rrc_reject().crit_exts.rrc_reject();
    if (not reject.wait_time_present) {
      return std::nullopt;
    }
    return reject.wait_time;
  }

protected:
  unsigned du_idx    = 0;
  unsigned cu_up_idx = 0;

  gnb_du_ue_f1ap_id_t                du_ue_f1ap_id = gnb_du_ue_f1ap_id_t::min;
  std::optional<gnb_cu_ue_f1ap_id_t> cu_ue_id;
  rnti_t                             crnti = to_rnti(0x4601);

  f1ap_message f1ap_pdu;
  ngap_message ngap_pdu;
};

TEST_F(cu_cp_setup_test, when_new_ue_sends_rrc_setup_request_then_dl_rrc_message_sent_with_rrc_setup)
{
  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer());

  // Check UE is created.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 1);
  ASSERT_EQ(report.ues[0].rnti, crnti);
  ASSERT_EQ(report.ues[0].du_id, report.dus[0].id);
  ASSERT_EQ(report.ues[0].pci, report.dus[0].cells[0].pci);
}

TEST_F(cu_cp_setup_test, when_new_ue_sends_rrc_setup_request_with_unknown_cause_then_dl_rrc_message_sent_with_rrc_setup)
{
  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer(
      byte_buffer::create({0x1d, 0xa9, 0xe1, 0xb9, 0xf1, 0x18}).value()));

  // Check UE is created.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 1);
  ASSERT_EQ(report.ues[0].rnti, crnti);
  ASSERT_EQ(report.ues[0].du_id, report.dus[0].id);
  ASSERT_EQ(report.ues[0].pci, report.dus[0].cells[0].pci);
}

TEST_F(cu_cp_setup_test, when_cu_cp_receives_f1_removal_request_then_rrc_setup_procedure_is_cancelled)
{
  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer());

  // Inject F1 Removal Request and await F1 Removal Response.
  ASSERT_TRUE(send_f1_removal_request_and_await_f1_removal_response());

  // Check UE is deleted.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0);
}

TEST_F(cu_cp_setup_test, when_du_hangs_up_then_rrc_setup_procedure_is_cancelled)
{
  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer());

  // DU hangs up.
  ASSERT_TRUE(drop_du_connection(du_idx));

  // Check UE is deleted.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0);
}

TEST_F(cu_cp_setup_test, when_initial_ul_rrc_message_has_no_rrc_container_then_ue_is_rejected)
{
  // Inject Initial UL RRC Message without RRC container and await UE Context Release Command containing RRC Reject.
  ASSERT_TRUE(send_initial_ul_rrc_message_without_rrc_container_and_await_ue_context_release_command());

  const auto& ue_rel = f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd();

  // Inject F1AP UE Context Release Complete.
  auto rel_complete = test_helpers::generate_ue_context_release_complete(
      int_to_gnb_cu_ue_f1ap_id(ue_rel->gnb_cu_ue_f1ap_id), du_ue_f1ap_id);
  get_du(du_idx).push_ul_pdu(rel_complete);

  // TEST: UE is removed in CU-CP.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0);
}

TEST_F(cu_cp_setup_test, when_du_sends_initial_ul_rrc_message_without_du_to_cu_container_then_ue_is_rejected)
{
  // Inject Initial UL RRC Message without DU-to-CU-RRC container and await F1AP UE Context Release Command.
  ASSERT_TRUE(send_initial_ul_rrc_message_without_du_to_cu_rrc_container_and_await_f1ap_ue_context_release_command());

  // TEST: UE is not destroyed in CU-CP until UE Context Release Complete is received.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 1);
  ASSERT_EQ(report.ues[0].rnti, to_rnti(0x4601));

  // Inject F1AP UE Context Release Complete.
  auto rel_complete = test_helpers::generate_ue_context_release_complete(cu_ue_id.value(), du_ue_f1ap_id);
  get_du(du_idx).push_ul_pdu(rel_complete);

  // TEST: UE context removed from CU-CP.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());
}

TEST_F(cu_cp_setup_test, when_unexpected_ul_nas_transport_message_is_received_during_rrc_setup_then_ue_is_released)
{
  // Check no UEs.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());

  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer());

  // Inject unexpected UL RRC Message Transfer with NAS Transport Message and await F1AP UE Context Release Command.
  ASSERT_TRUE(send_ul_rrc_message_transfer_and_await_f1ap_ue_context_release_command());

  // Verify AMF is not notified of NAS Transport Message.
  ASSERT_FALSE(this->wait_for_ngap_tx_pdu(ngap_pdu));

  // Injects F1AP UE Context Release Complete.
  auto rel_complete = test_helpers::generate_ue_context_release_complete(cu_ue_id.value(), du_ue_f1ap_id);
  get_du(du_idx).push_ul_pdu(rel_complete);

  // TEST: UE context removed from CU-CP.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_establishments.get_count(establishment_cause_t::mo_sig),
            1u);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.failed_rrc_connection_establishments.get_count(establishment_fail_cause_t::no_reply),
      1u);
}

TEST_F(cu_cp_setup_test, when_du_initiated_ue_context_release_request_is_received_during_rrc_setup_then_ue_is_released)
{
  // Check no UEs.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());

  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer());

  // DU reports a UE-specific radio link failure while RRC Setup is still pending, and await F1AP UE Context Release
  // Command.
  ASSERT_TRUE(send_f1ap_ue_context_release_request_and_await_f1ap_ue_context_release_command());

  // Verify AMF is not notified (the UE never had an NGAP context).
  ASSERT_FALSE(this->wait_for_ngap_tx_pdu(ngap_pdu));

  // Injects F1AP UE Context Release Complete.
  auto rel_complete = test_helpers::generate_ue_context_release_complete(cu_ue_id.value(), du_ue_f1ap_id);
  get_du(du_idx).push_ul_pdu(rel_complete);

  // TEST: UE context removed from CU-CP.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_establishments.get_count(establishment_cause_t::mo_sig),
            1u);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.failed_rrc_connection_establishments.get_count(establishment_fail_cause_t::no_reply),
      1u);
}

TEST_F(cu_cp_setup_test, when_ue_selects_unsupported_plmn_during_rrc_setup_then_ue_is_released)
{
  // Check no UEs.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());

  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer());

  // AMF still not notified of UE attach.
  ASSERT_FALSE(this->get_amf().try_pop_rx_pdu(ngap_pdu));

  // Inject UL RRC Message with RRC Setup Complete (UE selects unsupported PLMN) and await F1AP UE Context Release
  // Command.
  ASSERT_TRUE(send_rrc_setup_complete_and_await_f1ap_ue_context_release_command());

  // Injects F1AP UE Context Release Complete.
  auto rel_complete = test_helpers::generate_ue_context_release_complete(cu_ue_id.value(), du_ue_f1ap_id);
  get_du(du_idx).push_ul_pdu(rel_complete);

  // TEST: UE context removed from CU-CP.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());

  // The CU-CP releases the UE with an RRCRelease and sends no RRCReject, so the failure cause is "other".
  ASSERT_EQ(report.dus[0].rrc_metrics.failed_rrc_connection_establishments.get_count(establishment_fail_cause_t::other),
            1U);
  ASSERT_EQ(report.dus[0].rrc_metrics.failed_rrc_connection_establishments.get_count(
                establishment_fail_cause_t::network_reject),
            0U);
}

TEST_F(cu_cp_setup_test, when_rrc_setup_completes_then_initial_message_sent_to_amf)
{
  // Check no UEs.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());

  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer());

  // AMF still not notified of UE attach.
  ASSERT_FALSE(this->get_amf().try_pop_rx_pdu(ngap_pdu));

  // Inject UL RRC Message with RRC Setup Complete and await Initial UE message.
  ASSERT_TRUE(send_rrc_setup_complete_and_await_initial_ue_message());

  // Check that the UE is in the RRC connections of the metrics report.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_FALSE(report.dus.empty());
  ASSERT_FALSE(report.ues.empty());
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 1);
  ASSERT_TRUE(report.dus[0].rrc_metrics.mean_rrc_connection_setup_time.has_value());
  ASSERT_TRUE(report.dus[0].rrc_metrics.max_rrc_connection_setup_time.has_value());
}

TEST_F(cu_cp_setup_test, when_rrc_setup_complete_contains_registered_amf_then_initial_ue_message_has_no_amf_set_id)
{
  // Create UE by sending Initial UL RRC Message and await DL RRC Message Transfer.
  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_dl_rrc_message_transfer());

  // Inject RRC Setup Complete with the registered AMF (AMF Region ID 2, AMF Set ID 1, AMF Pointer 0).
  asn1::rrc_nr::ul_dcch_msg_s rrc_setup_complete = test_helpers::create_rrc_setup_complete();
  auto&                       ies = rrc_setup_complete.msg.c1().rrc_setup_complete().crit_exts.rrc_setup_complete();
  ies.registered_amf_present      = true;
  ies.registered_amf.amf_id.from_number(0x020040);
  get_du(du_idx).push_rrc_ul_dcch_message(
      du_ue_f1ap_id, srb_id_t::srb1, test_helpers::pack_ul_dcch_msg(rrc_setup_complete));

  // Verify AMF is notified of UE attach.
  ASSERT_TRUE(this->wait_for_ngap_tx_pdu(ngap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_init_ue_message(ngap_pdu));

  // TS 38.413 section 8.6.1.2: The AMF Set ID IE marks a rerouted Initial UE Message.
  ASSERT_FALSE(ngap_pdu.pdu.init_msg().value.init_ue_msg()->amf_set_id_present);
}

///////////////////////////////////////////////////////////////////////////////
//                                 Admission
///////////////////////////////////////////////////////////////////////////////
class cu_cp_setup_admission_limit_test : public cu_cp_setup_test
{
public:
  cu_cp_setup_admission_limit_test() :
    cu_cp_setup_test(cu_cp_test_env_params{/*max_nof_cu_ups*/ 8,
                                           /*max_nof_dus*/ 8,
                                           /*max_nof_ues*/ 0})
  {
  }
};

TEST_F(cu_cp_setup_admission_limit_test, when_initial_ul_rrc_message_is_rejected_by_admission_then_ue_is_released)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-1");

  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_ue_context_release_command());

  // Admission rejection must not progress into NGAP Initial UE Message flow.
  ASSERT_FALSE(this->get_amf().try_pop_rx_pdu(ngap_pdu));

  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 1);

  // Evaluate that metrics were updated correctly.
  ASSERT_FALSE(report.dus.empty());
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_establishments.get_count(establishment_cause_t::mo_sig),
            1u);
  ASSERT_EQ(report.dus[0].rrc_metrics.failed_rrc_connection_establishments.get_count(
                establishment_fail_cause_t::network_reject),
            1u);

  auto rel_complete = test_helpers::generate_ue_context_release_complete(cu_ue_id.value(), du_ue_f1ap_id);
  get_du(du_idx).push_ul_pdu(rel_complete);

  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());
}

TEST_F(cu_cp_setup_admission_limit_test,
       when_du_disconnects_while_ue_context_release_is_in_flight_then_teardown_does_not_crash)
{
  OCUDU_TEST_REQUIREMENTS("MVP-ARCH-INTF-9");

  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_ue_context_release_command());

  // Do not inject the F1AP UE Context Release Complete: the release transaction is left in flight.

  // The DU disconnects while the release procedure is still awaiting a response from it. This drives the F1AP
  // stop procedure to cancel a per-UE transaction whose completion synchronously runs the UE removal cascade.
  ASSERT_TRUE(drop_du_connection(du_idx));

  // The UE is removed as part of the DU teardown, despite the release response never arriving.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());
}

TEST_F(cu_cp_setup_admission_limit_test,
       when_rrc_reject_wait_time_is_not_configured_then_rrc_reject_carries_the_default_wait_time)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-1");

  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_ue_context_release_command());

  // The RRC Reject always includes the waitTime field (TS 38.331 section 6.2.2, RRCReject-IEs field descriptions).
  ASSERT_EQ(get_wait_time_from_last_rrc_reject(), 1);

  // Injects F1AP UE Context Release Complete.
  auto rel_complete = test_helpers::generate_ue_context_release_complete(cu_ue_id.value(), du_ue_f1ap_id);
  get_du(du_idx).push_ul_pdu(rel_complete);

  // TEST: UE context removed from CU-CP.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());
}

class cu_cp_setup_wait_time_test : public cu_cp_setup_test
{
public:
  static constexpr uint8_t reject_wait_time_s = 8;
  cu_cp_setup_wait_time_test() :
    cu_cp_setup_test(cu_cp_test_env_params{/*max_nof_cu_ups*/ 8,
                                           /*max_nof_dus*/ 8,
                                           /*max_nof_ues*/ 0,
                                           /*max_nof_drbs_per_ue*/ 8,
                                           /*amf_config*/ {{default_supported_tracking_area}},
                                           /*trigger_ho_from_measurements*/ true,
                                           /*enable_rrc_inactive*/ false,
                                           /*enable_xnc_peer*/ false,
                                           /*rrc_reject_wait_time*/ std::chrono::seconds{reject_wait_time_s}})
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-1");
  }
};

TEST_F(cu_cp_setup_wait_time_test, when_rrc_reject_wait_time_is_configured_then_rrc_reject_carries_wait_time_ie)
{
  // Check the configuratin matches with the provided value.
  ASSERT_EQ(get_cu_cp_cfg().rrc.rrc_reject_wait_time, std::chrono::seconds{reject_wait_time_s});

  ASSERT_TRUE(send_initial_ul_rrc_message_and_await_ue_context_release_command());

  // The RRC Reject carries the configured waitTime value.
  auto wait_time = get_wait_time_from_last_rrc_reject();
  ASSERT_TRUE(wait_time.has_value());
  ASSERT_EQ(wait_time.value(), reject_wait_time_s);

  // Injects F1AP UE Context Release Complete.
  auto rel_complete = test_helpers::generate_ue_context_release_complete(cu_ue_id.value(), du_ue_f1ap_id);
  get_du(du_idx).push_ul_pdu(rel_complete);

  // TEST: UE context removed from CU-CP.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_TRUE(report.ues.empty());
}
