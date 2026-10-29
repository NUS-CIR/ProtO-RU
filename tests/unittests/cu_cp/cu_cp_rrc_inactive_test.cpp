// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_cp_test_environment.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/e1ap/e1ap_cu_cp_test_messages.h"
#include "tests/test_doubles/e1ap/e1ap_test_message_validators.h"
#include "tests/test_doubles/f1ap/f1ap_test_message_validators.h"
#include "tests/test_doubles/f1ap/f1ap_test_messages.h"
#include "tests/test_doubles/ngap/ngap_test_message_validators.h"
#include "tests/test_doubles/rrc/rrc_test_message_validators.h"
#include "tests/test_doubles/rrc/rrc_test_messages.h"
#include "tests/test_doubles/xnap/xnap_test_message_validators.h"
#include "tests/unittests/cu_cp/test_helpers.h"
#include "tests/unittests/ngap/ngap_test_messages.h"
#include "tests/unittests/xnap/xnap_test_messages.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/asn1/ngap/ngap_ies.h"
#include "ocudu/asn1/ngap/ngap_pdu_contents.h"
#include "ocudu/asn1/rrc_nr/dl_ccch_msg.h"
#include "ocudu/asn1/xnap/xnap_pdu_contents.h"
#include "ocudu/cu_cp/cu_cp_configuration.h"
#include "ocudu/e1ap/common/e1ap_message.h"
#include "ocudu/ngap/ngap_message.h"
#include <chrono>
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;

class cu_cp_rrc_inactive_test : public cu_cp_test_environment, public ::testing::Test
{
public:
  cu_cp_rrc_inactive_test() : cu_cp_rrc_inactive_test(false) {}

  explicit cu_cp_rrc_inactive_test(bool enable_xnc_peer) :
    cu_cp_test_environment({/* max nof cu-ups */ 8,
                            /* max nof dus */ 8,
                            /* max nof ues */ 8192,
                            /* max nof drbs per ue */ 8,
                            /* amf config */ {{default_supported_tracking_area}},
                            /* trigger ho from measurements */ true,
                            /* enable rrc inactive */ true,
                            enable_xnc_peer})
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MOB-10");

    // Run NG setup to completion.
    run_ng_setup();

    if (enable_xnc_peer) {
      // Run XN setup to completion.
      run_xn_setup();
    }

    // Setup DU.
    std::optional<unsigned> ret = connect_new_du();
    EXPECT_TRUE(ret.has_value());
    du_idx = ret.value();
    EXPECT_TRUE(this->run_f1_setup(du_idx));

    // Setup CU-UP.
    ret = connect_new_cu_up();
    EXPECT_TRUE(ret.has_value());
    cu_up_idx = ret.value();
    EXPECT_TRUE(this->run_e1_setup(cu_up_idx));
  }

  void connect_ue_with_rrc_inactive_support(
      std::optional<cu_cp_core_network_assist_info_for_inactive> cn_assist_info_for_inactive =
          cu_cp_core_network_assist_info_for_inactive{.ue_id_idx_value = 0xc0})
  {
    // Connect UE 0x4601 with RRC inactive support.
    report_fatal_error_if_not(attach_ue(du_idx,
                                        cu_up_idx,
                                        du_ue_id,
                                        crnti,
                                        amf_ue_id,
                                        cu_up_e1ap_id,
                                        psi,
                                        drb_id_t::drb1,
                                        qfi,
                                        test_helpers::pack_ul_dcch_msg(test_helpers::create_rrc_setup_complete()),
                                        make_byte_buffer("00070e00cc6fcda5").value(),
                                        std::move(cn_assist_info_for_inactive),
                                        true),
                              "Failed to attach UE with RRC Inactive support");
    ue_ctx = this->find_ue_context(du_idx, du_ue_id);
    report_fatal_error_if_not(ue_ctx != nullptr,
                              "Failed to find UE context after attaching UE with RRC Inactive support");
  }

  void connect_ue_without_rrc_inactive_support()
  {
    // Connect UE 0x4601 without RRC inactive support.
    report_fatal_error_if_not(attach_ue(du_idx,
                                        cu_up_idx,
                                        du_ue_id,
                                        crnti,
                                        amf_ue_id,
                                        cu_up_e1ap_id,
                                        psi,
                                        drb_id_t::drb1,
                                        qfi,
                                        test_helpers::pack_ul_dcch_msg(test_helpers::create_rrc_setup_complete()),
                                        make_byte_buffer("00070e00cc6fcda5").value(),
                                        std::nullopt,
                                        false),
                              "Failed to attach UE without RRC Inactive support");
    ue_ctx = this->find_ue_context(du_idx, du_ue_id);
    report_fatal_error_if_not(ue_ctx != nullptr,
                              "Failed to find UE context after attaching UE with RRC Inactive support");
  }

  [[nodiscard]] bool send_bearer_context_inactivity_notification(const e1ap_message& inactivity_notification)
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");

    // Inject inactivity notification.
    get_cu_up(cu_up_idx).push_tx_pdu(inactivity_notification);
    return true;
  }

  [[nodiscard]] bool send_ue_level_bearer_context_inactivity_notification_and_await_ue_context_release_request()
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");

    // Inject inactivity notification and wait for UE Context Release Request.
    if (!send_bearer_context_inactivity_notification(generate_bearer_context_inactivity_notification_with_ue_level(
            ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id))) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive UE Context Release Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_request(ngap_pdu),
                              "Invalid UE Context Release Request");
    return true;
  }

  [[nodiscard]] bool
  send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request()
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");

    // Inject inactivity notification and wait for Bearer Context Modification Request.
    if (!send_bearer_context_inactivity_notification(generate_bearer_context_inactivity_notification_with_ue_level(
            ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id))) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Modification Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool
  send_drb_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request()
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");

    // Inject inactivity notification and wait for Bearer Context Modification Request.
    if (!send_bearer_context_inactivity_notification(generate_bearer_context_inactivity_notification_with_drb_level(
            ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id, {}, {drb_id_t::drb1}))) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Modification Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool
  send_pdu_session_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request()
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");

    // Inject inactivity notification and wait for Bearer Context Modification Request.
    if (!send_bearer_context_inactivity_notification(
            generate_bearer_context_inactivity_notification_with_pdu_session_level(
                ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id, {}, {psi}))) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Modification Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool send_bearer_context_modification_response_and_await_ue_context_release_command()
  {
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject Bearer Context Modification Response and wait for PDU Session Resource Setup Response.
    get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_response(
        ue_ctx->cu_cp_e1ap_id.value(), ue_ctx->cu_up_e1ap_id.value(), {}, {}));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid UE Context Release Command");

    const byte_buffer& rrc_container = test_helpers::get_rrc_container(f1ap_pdu);

    report_fatal_error_if_not(
        test_helpers::is_valid_rrc_release_with_suspend(test_helpers::extract_dl_dcch_msg(rrc_container)),
        "Invalid RRC Release");

    return true;
  }

  [[nodiscard]] bool send_f1ap_ue_context_release_complete(gnb_du_ue_f1ap_id_t f1ap_du_ue_id = gnb_du_ue_f1ap_id_t::min)
  {
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject F1AP UE Context Release Complete.
    get_du(du_idx).push_ul_pdu(
        test_helpers::generate_ue_context_release_complete(ue_ctx->cu_ue_id.value(), f1ap_du_ue_id));
    return true;
  }

  [[nodiscard]] bool send_init_ul_rrc_message_transfer_and_await_ue_context_setup_request()
  {
    // Inject Initial UL RRC message and await UE context setup request.
    f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(
        du_ue_id_2,
        crnti_2,
        plmn_identity::test_value(),
        {},
        test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(0x4d8000)));
    test_logger.info("c-rnti={} du_ue={}: Injecting Initial UL RRC message", crnti_2, fmt::underlying(du_ue_id_2));
    get_du(du_idx).push_ul_pdu(init_ul_rrc_msg);
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive UE Context Setup Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_setup_request(f1ap_pdu, true),
                              "Invalid UE Context Setup Request");

    return true;
  }

  [[nodiscard]] bool send_init_ul_rrc_message_transfer_with_rna_upd_and_await_ue_context_release_command()
  {
    // Inject Initial UL RRC message and await UE context setup request.
    f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(
        du_ue_id_2,
        crnti_2,
        plmn_identity::test_value(),
        {},
        test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(
            0x4d8000, "1111010001000010", asn1::rrc_nr::resume_cause_opts::rna_upd)));
    test_logger.info("c-rnti={} du_ue={}: Injecting Initial UL RRC message", crnti_2, fmt::underlying(du_ue_id_2));
    get_du(du_idx).push_ul_pdu(init_ul_rrc_msg);
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid UE Context Release Command");

    return true;
  }

  [[nodiscard]] bool
  send_init_ul_rrc_message_transfer_and_await_rrc_setup(uint64_t           resume_id    = 0x4d8000,
                                                        const std::string& resume_mac_i = "1111010001000010")
  {
    // Inject Initial UL RRC message and await DL RRC Message Transfer with RRC Setup.
    f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(
        du_ue_id_2,
        crnti_2,
        plmn_identity::test_value(),
        {},
        test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(resume_id, resume_mac_i)));
    test_logger.info("c-rnti={} du_ue={}: Injecting Initial UL RRC message", crnti_2, fmt::underlying(du_ue_id_2));
    get_du(du_idx).push_ul_pdu(init_ul_rrc_msg);
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive DL RRC Message Transfer");
    report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer_with_msg4(f1ap_pdu),
                              "Invalid DL RRC Message Transfer");

    return true;
  }

  [[nodiscard]] bool send_init_ul_rrc_message_transfer_and_await_ngap_ue_context_release_request(
      uint64_t           resume_id    = 0x4d8000,
      const std::string& resume_mac_i = "1111010001000010")
  {
    // Inject Initial UL RRC message and await NGAP UE Context Release Request.
    f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(
        du_ue_id_2,
        crnti_2,
        plmn_identity::test_value(),
        {},
        test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(resume_id, resume_mac_i)));
    test_logger.info("c-rnti={} du_ue={}: Injecting Initial UL RRC message", crnti_2, fmt::underlying(du_ue_id_2));
    get_du(du_idx).push_ul_pdu(init_ul_rrc_msg);
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu),
                              "Failed to receive NGAP UE Context Release Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_request(ngap_pdu),
                              "Invalid NGAP UE Context Release Request");

    return true;
  }

  [[nodiscard]] bool send_ue_context_setup_failure_and_await_ngap_ue_context_release_request()
  {
    // Inject UE Context Setup Failure and wait for NGAP UE Context Release Request.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_setup_failure(ue_ctx->cu_ue_id.value(), du_ue_id_2));
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu),
                              "Failed to receive NGAP UE Context Release Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_request(ngap_pdu),
                              "Invalid NGAP UE Context Release Request");
    return true;
  }

  [[nodiscard]] bool send_ue_context_setup_response_and_await_bearer_context_modification_request()
  {
    // Inject UE Context Setup Response and wait for Bearer Context Modification Request.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_setup_response(
        ue_ctx->cu_ue_id.value(),
        du_ue_id_2,
        crnti_2,
        make_byte_buffer(
            "5c04c00604b0701f00811a0f020001273b8c02692f30004d25e24040008c8040a26418d6d8d76006e08040000101000083446a48d8"
            "02692f1200000464e35b63224f8060664abff0124e9106e28dc61b8e372c6fbf56c70eb00442c0680182c4601c020521004930b2a0"
            "003fe00000000060dc2108000780594008300000020698101450a000e3890000246aac90838002081840a1839389142c60d1c3c811"
            "00000850000800b50001000850101800b50102000850202800b50203000850303800b503040c885010480504014014120580505018"
            "01416068050601c0141a0780507020314100880905204963028711d159e26f2681d2083c5df81821c00000038ffd294a5294f28160"
            "00021976000000000000000000108ad5450047001800082000e21009c400e0202108001c420138401c080441000388402708038180"
            "842000710804e18070401104000e21009c300080000008218081018201c1a0001c71000000080100020180020240088029800008c4"
            "0089c7001800")
            .value(),
        {drb_id_t::drb1}));

    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Modification Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool send_bearer_context_modification_failure_and_await_ngap_ue_context_release_request()
  {
    // Inject Bearer Context Modification Failure and wait for NGAP UE Context Release Request.
    get_cu_up(cu_up_idx).push_tx_pdu(
        generate_bearer_context_modification_failure(ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id));
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu),
                              "Failed to receive NGAP UE Context Release Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_request(ngap_pdu),
                              "Invalid NGAP UE Context Release Request");
    return true;
  }

  [[nodiscard]] bool send_bearer_context_modification_response_and_await_dl_rrc_message_transfer()
  {
    // Inject Bearer Context Modification Response and wait for DL RRC Message (containing RRC Resume).
    get_cu_up(cu_up_idx).push_tx_pdu(
        generate_bearer_context_modification_response(ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive F1AP DL RRC Message (containing RRC Resume)");
    report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                              "Invalid DL RRC Message Transfer");
    {
      const byte_buffer& rrc_container = test_helpers::get_rrc_container(f1ap_pdu);

      report_fatal_error_if_not(test_helpers::is_valid_rrc_resume(test_helpers::extract_dl_dcch_msg(rrc_container)),
                                "Invalid RRC Resume");
    }
    return true;
  }

  [[nodiscard]] bool send_rrc_resume_complete()
  {
    // Inject UL RRC Message (containing RRC Resume Complete).
    f1ap_message init_ul_rrc_msg = test_helpers::generate_ul_rrc_message_transfer(
        du_ue_id_2, ue_ctx->cu_ue_id.value(), srb_id_t::srb1, make_byte_buffer("000020400033b01cab").value());
    get_du(du_idx).push_ul_pdu(init_ul_rrc_msg);
    return true;
  }

  [[nodiscard]] bool trigger_rrc_inactive(gnb_du_ue_f1ap_id_t du_ue_id_)
  {
    // Inject Inactivity Notification and await Bearer Context Modification Request.
    if (!send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request()) {
      return false;
    }

    // Send Bearer Context Modification Response and await UE Context Release Command.
    if (!send_bearer_context_modification_response_and_await_ue_context_release_command()) {
      return false;
    }

    // Send F1AP UE Context Release Complete.
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject F1AP UE Context Release Complete.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_release_complete(ue_ctx->cu_ue_id.value(), du_ue_id_));

    return true;
  }

  [[nodiscard]] bool resume_ue(gnb_du_ue_f1ap_id_t du_ue_id_,
                               rnti_t              crnti_,
                               unsigned            i_rnti,
                               const std::string&  resume_mac_i,
                               byte_buffer         rrc_resume_complete)
  {
    // Inject Initial UL RRC message and await UE context setup request.
    f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(
        du_ue_id_,
        crnti_,
        plmn_identity::test_value(),
        {},
        test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(i_rnti, resume_mac_i)));
    test_logger.info("c-rnti={} du_ue={}: Injecting Initial UL RRC message", crnti_, fmt::underlying(du_ue_id_));
    get_du(du_idx).push_ul_pdu(init_ul_rrc_msg);
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive UE Context Setup Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_setup_request(f1ap_pdu, true),
                              "Invalid UE Context Setup Request");

    // Inject UE Context Setup Response and wait for Bearer Context Modification Request.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_setup_response(
        ue_ctx->cu_ue_id.value(),
        du_ue_id_,
        crnti_,
        make_byte_buffer(
            "5c04c00604b0701f00811a0f020001273b8c02692f30004d25e24040008c8040a26418d6d8d76006e08040000101000083446a48d8"
            "02692f1200000464e35b63224f8060664abff0124e9106e28dc61b8e372c6fbf56c70eb00442c0680182c4601c020521004930b2a0"
            "003fe00000000060dc2108000780594008300000020698101450a000e3890000246aac90838002081840a1839389142c60d1c3c811"
            "00000850000800b50001000850101800b50102000850202800b50203000850303800b503040c885010480504014014120580505018"
            "01416068050601c0141a0780507020314100880905204963028711d159e26f2681d2083c5df81821c00000038ffd294a5294f28160"
            "00021976000000000000000000108ad5450047001800082000e21009c400e0202108001c420138401c080441000388402708038180"
            "842000710804e18070401104000e21009c300080000008218081018201c1a0001c71000000080100020180020240088029800008c4"
            "0089c7001800")
            .value(),
        {drb_id_t::drb1}));

    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Modification Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");

    // Send Bearer Context Modification Response and await DL RRC Message Transfer.
    if (!send_bearer_context_modification_response_and_await_dl_rrc_message_transfer()) {
      return false;
    }

    // Inject UL RRC Message (containing RRC Resume Complete).
    f1ap_message ul_rrc_msg = test_helpers::generate_ul_rrc_message_transfer(
        du_ue_id_, ue_ctx->cu_ue_id.value(), srb_id_t::srb1, std::move(rrc_resume_complete));
    get_du(du_idx).push_ul_pdu(ul_rrc_msg);

    return true;
  }

  [[nodiscard]] bool finish_rrc_setup()
  {
    // Send RRC Setup Complete.
    // > Generate UL RRC Message (containing RRC Setup Complete) with PDCP SN=0.
    get_du(du_idx).push_rrc_ul_dcch_message(
        du_ue_id_2, srb_id_t::srb1, test_helpers::pack_ul_dcch_msg(test_helpers::create_rrc_setup_complete()));

    // CU-CP should send an NGAP Initial UE Message.
    if (!this->wait_for_ngap_tx_pdu(ngap_pdu)) {
      return false;
    }

    report_fatal_error_if_not(test_helpers::is_valid_init_ue_message(ngap_pdu), "Invalid init UE message");

    return true;
  }

  [[nodiscard]] bool send_ngap_ue_context_release_command_and_await_bearer_context_release_command()
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");

    // Inject NGAP UE Context Release Command and wait for Bearer Context Release Command.
    get_amf().push_tx_pdu(generate_valid_ue_context_release_command_with_amf_ue_ngap_id(ue_ctx->amf_ue_id.value()));
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_release_command(e1ap_pdu),
                              "Invalid Bearer Context Release Command");
    return true;
  }

  [[nodiscard]] bool send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command()
  {
    // Inject Bearer Context Release Complete and wait for F1AP UE Context Release Command.
    get_cu_up(cu_up_idx).push_tx_pdu(
        generate_bearer_context_release_complete(ue_ctx->cu_cp_e1ap_id.value(), ue_ctx->cu_up_e1ap_id.value()));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid UE Context Release Command");
    return true;
  }

  [[nodiscard]] bool send_f1ap_ue_context_release_complete_and_await_ngap_ue_context_release_complete()
  {
    // Inject F1AP UE Context Release Complete and wait for N1AP UE Context Release Command.
    if (!send_f1ap_ue_context_release_complete()) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive UE Context Release Complete");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_complete(ngap_pdu),
                              "Invalid UE Context Release Complete");
    return true;
  }

  [[nodiscard]] bool send_e1ap_dl_data_notification_and_await_f1ap_paging(unsigned               du_idx_,
                                                                          gnb_cu_cp_ue_e1ap_id_t cu_cp_e1ap_id_,
                                                                          gnb_cu_up_ue_e1ap_id_t cu_up_e1ap_id_)
  {
    report_fatal_error_if_not(not this->get_cu_up(0).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");
    report_fatal_error_if_not(not this->get_du(du_idx_).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject E1AP DL Data Notification and wait for F1AP Paging.
    get_cu_up(0).push_tx_pdu(generate_dl_data_notification_message(cu_cp_e1ap_id_, cu_up_e1ap_id_));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx_, f1ap_pdu), "Failed to receive F1AP Paging");
    report_fatal_error_if_not(test_helpers::is_valid_paging(f1ap_pdu), "Invalid F1AP Paging");
    return true;
  }

  /// Get the cells in the Paging Cell List of the last F1AP Paging.
  std::vector<nr_cell_global_id_t> get_paged_cells() const
  {
    std::vector<nr_cell_global_id_t> cells;
    for (const auto& item : f1ap_pdu.pdu.init_msg().value.paging()->paging_cell_list) {
      const asn1::f1ap::nr_cgi_s& nr_cgi = item.value().paging_cell_item().nr_cgi;
      cells.emplace_back(plmn_identity::from_bytes(nr_cgi.plmn_id.to_bytes()).value(),
                         nr_cell_identity::create(nr_cgi.nr_cell_id.to_number()).value());
    }
    return cells;
  }

  [[nodiscard]] bool timeout_ran_paging_and_await_ngap_ue_context_release_request()
  {
    // Fail RAN paging (UE doesn't respond) and wait for NGAP UE Context Release Request.
    if (tick_until(std::chrono::milliseconds{10240}, [&]() { return false; }, false)) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive UE Context Release Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_request(ngap_pdu),
                              "Invalid UE Context Release Request");
    return true;
  }

  [[nodiscard]] bool timeout_rna_update_and_await_ngap_ue_context_release_request()
  {
    std::chrono::seconds rna_guard_time{1};

    // Fail RNA update (UE doesn't resume) and wait for NGAP UE Context Release Request.
    if (tick_until(
            std::chrono::duration_cast<std::chrono::milliseconds>(get_cu_cp_cfg().ue.t380 + rna_guard_time),
            [&]() { return false; },
            false)) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive UE Context Release Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_request(ngap_pdu),
                              "Invalid UE Context Release Request");
    return true;
  }

  [[nodiscard]] bool send_dl_nas_transport_and_await_f1ap_paging(unsigned du_idx_)
  {
    report_fatal_error_if_not(not this->get_cu_up(0).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");
    report_fatal_error_if_not(not this->get_du(du_idx_).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject NGAP DL message (dummy message) and wait for F1AP Paging.
    get_amf().push_tx_pdu(
        ocucp::generate_downlink_nas_transport_message(ue_ctx->amf_ue_id.value(), ue_ctx->ran_ue_id.value()));

    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx_, f1ap_pdu), "Failed to receive F1AP Paging");
    report_fatal_error_if_not(test_helpers::is_valid_paging(f1ap_pdu), "Invalid F1AP Paging");
    return true;
  }

  [[nodiscard]] bool send_dl_nas_transport(unsigned du_idx_)
  {
    report_fatal_error_if_not(not this->get_cu_up(0).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");
    report_fatal_error_if_not(not this->get_du(du_idx_).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    // Inject NGAP DL message (dummy message) and wait for F1AP Paging.
    get_amf().push_tx_pdu(
        ocucp::generate_downlink_nas_transport_message(ue_ctx->amf_ue_id.value(), ue_ctx->ran_ue_id.value()));

    report_fatal_error_if_not(not this->get_du(du_idx_).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");

    return true;
  }

  [[nodiscard]] bool await_dl_rrc_message_transfer(unsigned du_idx_)
  {
    // Wait for F1AP DL RRC message transfer.
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx_, f1ap_pdu),
                              "Failed to receive F1AP DL RRC message transfer");
    report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                              "Invalid F1AP DL RRC message transfer");
    return true;
  }

  unsigned du_idx    = 0;
  unsigned cu_up_idx = 0;

  /// NR CGI of the cell that the DU at du_idx serves.
  const nr_cell_global_id_t du_cell_cgi{test_helpers::served_cell_item_info{}.plmn_id,
                                        test_helpers::served_cell_item_info{}.nci};

  gnb_du_ue_f1ap_id_t    du_ue_id      = gnb_du_ue_f1ap_id_t::min;
  rnti_t                 crnti         = to_rnti(0x4601);
  amf_ue_id_t            amf_ue_id     = amf_ue_id_t::min;
  gnb_cu_up_ue_e1ap_id_t cu_up_e1ap_id = gnb_cu_up_ue_e1ap_id_t::min;

  gnb_du_ue_f1ap_id_t du_ue_id_2 = int_to_gnb_du_ue_f1ap_id(1);
  rnti_t              crnti_2    = to_rnti(0x4602);

  const cu_cp_test_environment::ue_context* ue_ctx = nullptr;

  pdu_session_id_t psi  = uint_to_pdu_session_id(1);
  pdu_session_id_t psi2 = uint_to_pdu_session_id(2);
  qos_flow_id_t    qfi  = uint_to_qos_flow_id(1);
  qos_flow_id_t    qfi2 = uint_to_qos_flow_id(2);

  e1ap_message e1ap_pdu;
  f1ap_message f1ap_pdu;
  ngap_message ngap_pdu;
};

//----------------------------------------------------------------------------------//
// UE without RRC Inactive support                                                  //
//----------------------------------------------------------------------------------//

TEST_F(cu_cp_rrc_inactive_test,
       when_ue_level_inactivity_message_received_and_rrc_inactive_not_supported_then_ue_is_released)
{
  // Connect UE without RRC Inactive support.
  connect_ue_without_rrc_inactive_support();

  // Inject Inactivity Notification and await UE Context Release Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_ue_context_release_request());
}

//----------------------------------------------------------------------------------//
// UE with RRC Inactive support                                                     //
//----------------------------------------------------------------------------------//

TEST_F(
    cu_cp_rrc_inactive_test,
    when_ue_level_inactivity_message_received_and_cn_assist_info_for_inactive_is_not_present_then_ue_is_not_set_to_rrc_inactive)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support(std::nullopt);

  // Inject Inactivity Notification and await UE Context Release Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_ue_context_release_request());
}

TEST_F(cu_cp_rrc_inactive_test, when_ue_level_inactivity_message_received_then_ue_is_set_to_rrc_inactive)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);
}

TEST_F(cu_cp_rrc_inactive_test, when_amf_rejects_the_resume_cause_then_resume_is_rejected_and_ue_stays_inactive)
{
  // Connect UE with RRC Inactive support and set it to RRC inactive.
  connect_ue_with_rrc_inactive_support();
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // The AMF rejects non-emergency mobile originated data (TS 38.413 section 8.7.7). The UE resumes with cause mo-Data.
  get_amf().push_tx_pdu(generate_overload_start(asn1::ngap::overload_action_opts::reject_non_emergency_mo_dt));
  get_du(du_idx).push_ul_pdu(test_helpers::generate_init_ul_rrc_message_transfer(
      du_ue_id_2,
      crnti_2,
      plmn_identity::test_value(),
      {},
      test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(0x4d8000))));
  ASSERT_TRUE(wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu)) << "No response was sent to the UE";
  ASSERT_TRUE(test_helpers::is_valid_ue_context_release_command(f1ap_pdu)) << "The resume was not rejected";
  const auto& release_cmd = f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd();
  ASSERT_EQ(release_cmd->srb_id, 0);
  asn1::rrc_nr::dl_ccch_msg_s ccch;
  asn1::cbit_ref              bref{release_cmd->rrc_container};
  ASSERT_EQ(ccch.unpack(bref), asn1::OCUDUASN_SUCCESS);
  ASSERT_EQ(ccch.msg.c1().type().value, asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types_opts::rrc_reject);
  ASSERT_TRUE(ccch.msg.c1().rrc_reject().crit_exts.rrc_reject().wait_time_present);
  get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_release_complete(f1ap_pdu));
  ASSERT_FALSE(get_amf().try_pop_rx_pdu(ngap_pdu)) << "The context of the UE was released towards the AMF";

  // After the overload, the UE resumes with the same I-RNTI. The ResumeMAC-I check passes only if the CU-CP kept the
  // keys of the suspended UE.
  get_amf().push_tx_pdu(generate_overload_stop());
  get_du(du_idx).push_ul_pdu(test_helpers::generate_init_ul_rrc_message_transfer(
      int_to_gnb_du_ue_f1ap_id(2),
      to_rnti(0x4603),
      plmn_identity::test_value(),
      {},
      test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(0x4d8000))));
  ASSERT_TRUE(wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu)) << "The UE was not resumed";
  ASSERT_TRUE(test_helpers::is_valid_ue_context_setup_request(f1ap_pdu, true)) << "The UE was not resumed";
}

TEST_F(cu_cp_rrc_inactive_test, when_the_allowed_slice_of_the_ue_is_overloaded_then_resume_is_rejected)
{
  // Connect UE with RRC Inactive support and set it to RRC inactive.
  connect_ue_with_rrc_inactive_support();
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // The AMF overloads the slice that the Initial Context Setup Request allowed for the UE (TS 23.501 section 5.19.5.2).
  ngap_message                            overload_start = generate_overload_start();
  asn1::ngap::overload_start_nssai_item_s item;
  asn1::ngap::slice_overload_item_s       slice;
  slice.s_nssai.sst.from_number(1);
  slice.s_nssai.sd_present = true;
  slice.s_nssai.sd.from_string("db2700");
  item.slice_overload_list.push_back(slice);
  overload_start.pdu.init_msg().value.overload_start()->overload_start_nssai_list_present = true;
  overload_start.pdu.init_msg().value.overload_start()->overload_start_nssai_list.push_back(item);
  get_amf().push_tx_pdu(overload_start);

  get_du(du_idx).push_ul_pdu(test_helpers::generate_init_ul_rrc_message_transfer(
      du_ue_id_2,
      crnti_2,
      plmn_identity::test_value(),
      {},
      test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(0x4d8000))));
  ASSERT_TRUE(wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu)) << "No response was sent to the UE";
  ASSERT_TRUE(test_helpers::is_valid_ue_context_release_command(f1ap_pdu)) << "The resume was not rejected";
  ASSERT_EQ(f1ap_pdu.pdu.init_msg().value.ue_context_release_cmd()->srb_id, 0);
}

TEST_F(cu_cp_rrc_inactive_test, when_duplicate_ue_level_inactivity_message_is_received_then_it_is_ignored)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics after the (single) RRC inactive transition. max_nof_rrc_connections still reflects the UE being
  // connected for part of this reporting window, so only the inactive counters are checked here.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Inject a duplicate Inactivity Notification for the same, already inactive UE. It must be ignored, since the UE
  // is no longer in RRC connected state. Otherwise the RRC inactive transition would be counted a second time,
  // decrementing the already zero active connection count below zero.
  ASSERT_TRUE(this->send_bearer_context_inactivity_notification(
      generate_bearer_context_inactivity_notification_with_ue_level(ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id)));
  ASSERT_FALSE(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu, std::chrono::milliseconds{200}));

  // Metrics must remain unchanged.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 0);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 0);
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);
}

TEST_F(cu_cp_rrc_inactive_test,
       when_drb_level_inactivity_message_with_inactivity_for_all_drbs_received_then_ue_is_set_to_rrc_inactive)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_drb_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);
}

TEST_F(cu_cp_rrc_inactive_test,
       when_pdu_session_level_inactivity_message_with_inactivity_for_all_drbs_received_then_ue_is_set_to_rrc_inactive)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(
      send_pdu_session_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);
}

//----------------------------------------------------------------------------------//
// UE initiated resume                                                              //
//----------------------------------------------------------------------------------//

TEST_F(cu_cp_rrc_inactive_test, when_ue_rna_update_timer_expires_then_ue_release_is_requested)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Check metrics for active RRC UE.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 1);

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics for RRC inactive transition.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  ASSERT_TRUE(timeout_rna_update_and_await_ngap_ue_context_release_request());
}

TEST_F(cu_cp_rrc_inactive_test, when_rrc_resume_request_is_received_then_existing_ue_is_found_and_resumed)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Check metrics for active RRC UE.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 1);

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics for RRC inactive transition.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Send Initial UL RRC Message containing RRC Resume Request.
  ASSERT_TRUE(send_init_ul_rrc_message_transfer_and_await_ue_context_setup_request());

  // Check metrics for attempted RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);

  // Send UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await DL RRC Message Transfer.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_dl_rrc_message_transfer());

  // Send RRC Resume Complete.
  ASSERT_TRUE(send_rrc_resume_complete());

  // Check metrics for successful RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
}

TEST_F(cu_cp_rrc_inactive_test, when_rrc_resume_complete_carries_nas_then_nas_is_sent_to_the_amf)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and handle it.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Resume the UE. The RRC Resume Complete contains the NAS PDU 7e0045010bf2 (Deregistration Request).
  ASSERT_TRUE(resume_ue(
      du_ue_id_2, crnti_2, 0x4d8000, "1111010001000010", make_byte_buffer("000020c033f00228085f90001cddc4c6").value()));

  ASSERT_TRUE(this->wait_for_ngap_tx_pdu(ngap_pdu)) << "The NAS message was not sent to the AMF";
  ASSERT_TRUE(test_helpers::is_valid_ul_nas_transport_message(ngap_pdu));
  ASSERT_EQ(ngap_pdu.pdu.init_msg().value.ul_nas_transport()->nas_pdu, make_byte_buffer("7e0045010bf2").value())
      << "The AMF did not get the NAS message of the RRC Resume Complete";
}

TEST_F(cu_cp_rrc_inactive_test, when_ue_becomes_inactive_after_resume_then_resume_is_successful_again)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // First Active -> Inactive -> Resume cycle.

  // Check metrics for active RRC UE.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 1);

  // Inject Inactivity Notification and handle it.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Check metrics for RRC inactive transition.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Successfully resume UE.
  ASSERT_TRUE(
      resume_ue(du_ue_id_2, crnti_2, 0x4d8000, "1111010001000010", make_byte_buffer("000020400033b01cab").value()));

  // Check metrics for attempted/successful RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);

  // Second Active -> Inactive -> Resume cycle.

  gnb_du_ue_f1ap_id_t du_ue_id_3 = int_to_gnb_du_ue_f1ap_id(2);
  rnti_t              crnti_3    = to_rnti(0x4603);

  // Check metrics for active RRC UE.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 1);

  // Inject Inactivity Notification and handle it.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id_2));

  // Check metrics for RRC inactive transition.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Successfully resume UE.
  ASSERT_TRUE(
      resume_ue(du_ue_id_3, crnti_3, 0x4d8001, "1010101010011111", make_byte_buffer("00002240006f0cba6b").value()));

  // Check metrics for successful RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  // Check metrics for attempted/successful RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 2);
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 2);
}

TEST_F(cu_cp_rrc_inactive_test,
       when_rrc_resume_request_is_received_before_f1ap_ue_context_release_complete_then_resume_is_successful)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // The UE sends the RRC Resume Request before the DU confirms the release of the old F1-connection.
  get_du(du_idx).push_ul_pdu(test_helpers::generate_init_ul_rrc_message_transfer(
      du_ue_id_2,
      crnti_2,
      plmn_identity::test_value(),
      {},
      test_helpers::pack_ul_ccch_msg(test_helpers::create_rrc_resume_request(0x4d8000))));

  // The DU confirms the release of the old F1-connection.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete(du_ue_id));

  // The CU-CP continues the resume of the UE.
  ASSERT_TRUE(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_ue_context_setup_request(f1ap_pdu, true));
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_dl_rrc_message_transfer());
  ASSERT_TRUE(send_rrc_resume_complete());

  // The CU-CP keeps the UE, so it sends no UE Context Release Request to the AMF.
  ASSERT_FALSE(this->get_amf().try_pop_rx_pdu(ngap_pdu));

  // The CU-CP releases the new F1-connection when the UE becomes inactive again.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id_2));

  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
}

TEST_F(cu_cp_rrc_inactive_test, when_bearer_context_modification_for_suspend_fails_then_ue_release_is_requested)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Failure and await NGAP UE Context Release Request.
  ASSERT_TRUE(send_bearer_context_modification_failure_and_await_ngap_ue_context_release_request());
}

TEST_F(cu_cp_rrc_inactive_test, when_f1ap_ue_context_release_for_suspend_fails_then_ue_release_is_requested)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete with a gNB-DU UE F1AP ID that does not match the command.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete(du_ue_id_2));

  // Await NGAP UE Context Release Request.
  ASSERT_TRUE(this->wait_for_ngap_tx_pdu(ngap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_ue_context_release_request(ngap_pdu));
}

TEST_F(cu_cp_rrc_inactive_test, when_rrc_resume_request_for_unknown_ue_is_received_then_fallback_succeeds)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 0);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 0);

  // Send Initial UL RRC Message containing RRC Resume Request (no UE is inactive at the moment) and await RRC Setup.
  ASSERT_TRUE(send_init_ul_rrc_message_transfer_and_await_rrc_setup());

  // Finish RRC Setup.
  ASSERT_TRUE(finish_rrc_setup());

  // Check metrics for RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 0);
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes_followed_by_rrc_setup.get_count(
                resume_cause_t::mo_data),
            1);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.successful_rrc_connection_resumes_with_fallback.get_count(resume_cause_t::mo_data), 1);
}

TEST_F(cu_cp_rrc_inactive_test, when_rrc_resume_request1_for_unknown_ue_is_received_then_resume_attempt_is_counted)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 0);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 0);

  // Send an Initial UL RRC Message containing an RRCResumeRequest1 with a Full-I-RNTI (TS 38.331) that does not match
  // any local UE. Because rrc_ue_impl does not handle RRCResumeRequest1, only the resume attempt is counted. This test
  // does not assert the complete CU-CP path.
  f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(
      du_ue_id_2,
      crnti_2,
      plmn_identity::test_value(),
      {},
      test_helpers::pack_ul_ccch1_msg(test_helpers::create_rrc_resume_request1(0x1122334455ULL)));
  test_logger.info("c-rnti={} du_ue={}: Injecting Initial UL RRC message", crnti_2, fmt::underlying(du_ue_id_2));
  get_du(du_idx).push_ul_pdu(init_ul_rrc_msg);
  ASSERT_TRUE(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu));

  // Check metrics for RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
}

TEST_F(cu_cp_rrc_inactive_test, when_rrc_resume_request_with_invalid_resume_mac_is_received_then_release_is_sent)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and handle it.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Send Initial UL RRC Message containing RRC Resume Request with invalid ResumeMAC-I and await NGAP UE Context
  // Release Request.
  ASSERT_TRUE(
      send_init_ul_rrc_message_transfer_and_await_ngap_ue_context_release_request(0x4d8000, "0000000000000000"));

  // Inject NGAP UE Context Release Command and await Bearer Context Release Command.
  ASSERT_TRUE(send_ngap_ue_context_release_command_and_await_bearer_context_release_command());

  // Inject Bearer Context Release Complete and await F1AP UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command());

  // Inject F1AP UE Context Release Complete and await NGAP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete_and_await_ngap_ue_context_release_complete());

  // STATUS: UE should be removed at this stage.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0) << "UE should be removed";
  // Check metrics for RRC resume.
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 0);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.rrc_connection_resumes_followed_by_network_release.get_count(resume_cause_t::mo_data),
      1);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.successful_rrc_connection_resumes_with_fallback.get_count(resume_cause_t::mo_data), 0);
}

TEST_F(cu_cp_rrc_inactive_test, when_ue_context_setup_for_rrc_resume_fails_then_release_is_sent)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Check metrics for active RRC UE.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 1);

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics for RRC inactive transition.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Send Initial UL RRC Message containing RRC Resume Request.
  ASSERT_TRUE(send_init_ul_rrc_message_transfer_and_await_ue_context_setup_request());

  // Check metrics for attempted RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);

  // Send UE Context Setup Failure and await NGAP UE Context Release Request.
  ASSERT_TRUE(send_ue_context_setup_failure_and_await_ngap_ue_context_release_request());

  // Inject NGAP UE Context Release Command and await Bearer Context Release Command.
  ASSERT_TRUE(send_ngap_ue_context_release_command_and_await_bearer_context_release_command());

  // Inject Bearer Context Release Complete and await F1AP UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command());

  // Inject F1AP UE Context Release Complete and await NGAP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete_and_await_ngap_ue_context_release_complete());

  // STATUS: UE should be removed at this stage.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0) << "UE should be removed";
  // Check metrics for RRC resume.
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 0);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.rrc_connection_resumes_followed_by_network_release.get_count(resume_cause_t::mo_data),
      1);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.successful_rrc_connection_resumes_with_fallback.get_count(resume_cause_t::mo_data), 0);
}

TEST_F(cu_cp_rrc_inactive_test, when_bearer_context_modification_for_rrc_resume_fails_then_release_is_sent)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Check metrics for active RRC UE.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 1);

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics for RRC inactive transition.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Send Initial UL RRC Message containing RRC Resume Request.
  ASSERT_TRUE(send_init_ul_rrc_message_transfer_and_await_ue_context_setup_request());

  // Check metrics for attempted RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);

  // Send UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Failure and await NGAP UE Context Release Request.
  ASSERT_TRUE(send_bearer_context_modification_failure_and_await_ngap_ue_context_release_request());

  // Inject NGAP UE Context Release Command and await Bearer Context Release Command.
  ASSERT_TRUE(send_ngap_ue_context_release_command_and_await_bearer_context_release_command());

  // Inject Bearer Context Release Complete and await F1AP UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command());

  // Inject F1AP UE Context Release Complete and await NGAP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete_and_await_ngap_ue_context_release_complete());

  // STATUS: UE should be removed at this stage.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0) << "UE should be removed";
  // Check metrics for RRC resume.
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 0);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.rrc_connection_resumes_followed_by_network_release.get_count(resume_cause_t::mo_data),
      1);
  ASSERT_EQ(
      report.dus[0].rrc_metrics.successful_rrc_connection_resumes_with_fallback.get_count(resume_cause_t::mo_data), 0);
}

TEST_F(cu_cp_rrc_inactive_test, when_rrc_resume_request_with_cause_rna_upd_is_received_then_ue_is_set_to_inactive_again)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Check metrics for active RRC UE.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_rrc_connections, 1);

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Check metrics for RRC inactive transition.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Send Initial UL RRC Message containing RRC Resume Request with resume cause RNA_UPD.
  ASSERT_TRUE(send_init_ul_rrc_message_transfer_with_rna_upd_and_await_ue_context_release_command());

  // Check metrics for attempted RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::rna_upd), 1);

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete(int_to_gnb_du_ue_f1ap_id(1)));

  // Check metrics for RRC inactive transition and for the RRC resume followed by suspension.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.rrc_connection_resumes_followed_by_suspension.get_count(resume_cause_t::rna_upd),
            1);
}

TEST_F(cu_cp_rrc_inactive_test, when_ue_resumes_after_rna_update_resume_then_second_resume_succeeds)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());

  // Send Bearer Context Modification Response and await UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // Resume the UE with cause RNA Update. The UE is assigned crnti_2 for this resume attempt, even though no F1AP
  // UE Context Setup is performed for this resume cause.
  ASSERT_TRUE(send_init_ul_rrc_message_transfer_with_rna_upd_and_await_ue_context_release_command());

  // Send F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete(int_to_gnb_du_ue_f1ap_id(1)));

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Resume the UE again, this time with cause mo_data. The UE computes its ResumeMAC-I using the C-RNTI it was
  // assigned during the RNA update resume above (crnti_2). If the CU-CP did not update its record of the UE's
  // C-RNTI after the RNA update resume, this ResumeMAC-I verification would incorrectly fail.
  gnb_du_ue_f1ap_id_t du_ue_id_3 = int_to_gnb_du_ue_f1ap_id(2);
  rnti_t              crnti_3    = to_rnti(0x4603);
  ASSERT_TRUE(
      resume_ue(du_ue_id_3, crnti_3, 0x4d8001, "1010101010011111", make_byte_buffer("00002040001dca1c36").value()));

  // Check metrics for successful RRC resume.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::rna_upd), 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.attempted_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.successful_rrc_connection_resumes.get_count(resume_cause_t::mo_data), 1);
}

//----------------------------------------------------------------------------------//
// RAN paging                                                                       //
//----------------------------------------------------------------------------------//

TEST_F(cu_cp_rrc_inactive_test, when_dl_data_notification_for_unknown_ue_is_received_then_no_paging_is_sent)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BAS-16-1-a");

  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 0);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 0);

  // Inject E1AP DL Data Notification for unknown UE.
  get_cu_up(0).push_tx_pdu(generate_dl_data_notification_message(ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id));

  // No paging should be sent.
  ASSERT_FALSE(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu)) << "Received F1AP Paging";
}

TEST_F(cu_cp_rrc_inactive_test, when_dl_data_notification_for_inactive_ue_is_received_then_paging_is_sent_to_du)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BAS-16-1-a");

  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and handle it.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Inject E1AP DL Data Notification and await F1AP Paging.
  ASSERT_TRUE(
      send_e1ap_dl_data_notification_and_await_f1ap_paging(du_idx, ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id));

  // The RNA holds the one cell of the DU, so the F1AP Paging pages this cell.
  EXPECT_EQ(get_paged_cells(), std::vector<nr_cell_global_id_t>{du_cell_cgi});
}

TEST_F(cu_cp_rrc_inactive_test, when_dl_data_notification_for_inactive_ue_is_received_then_only_rna_cells_are_paged)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BAS-16-1-a");

  // Connect a second DU. Its cell is in the same tracking area, but not in the RNA of the UE.
  const test_helpers::served_cell_item_info cell_du2{.nci = nr_cell_identity::create(gnb_id_t{411, 22}, 1U).value(),
                                                     .pci = 1};
  std::optional<unsigned>                   ret = connect_new_du();
  ASSERT_TRUE(ret.has_value());
  unsigned du_idx2 = ret.value();
  ASSERT_TRUE(this->run_f1_setup(du_idx2, int_to_gnb_du_id(0x12), {cell_du2}));

  // Connect UE with RRC Inactive support on the first DU.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and handle it. The RNA holds the cells of the first DU.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Inject E1AP DL Data Notification and await F1AP Paging at the first DU.
  ASSERT_TRUE(
      send_e1ap_dl_data_notification_and_await_f1ap_paging(du_idx, ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id));
  EXPECT_EQ(get_paged_cells(), std::vector<nr_cell_global_id_t>{du_cell_cgi});

  // The second DU serves no cell of the RNA, so it gets no F1AP Paging.
  ASSERT_FALSE(this->wait_for_f1ap_tx_pdu(du_idx2, f1ap_pdu, std::chrono::milliseconds{100}))
      << "Received F1AP Paging at the DU outside the RNA";
}

TEST_F(
    cu_cp_rrc_inactive_test,
    when_dl_nas_transport_for_inactive_ue_is_received_then_paging_is_sent_to_du_and_after_successful_resume_dl_nas_transport_is_forwarded_to_ue)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BAS-16-1-a");

  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and handle it.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Inject DL NAS Transport and await F1AP Paging.
  ASSERT_TRUE(send_dl_nas_transport_and_await_f1ap_paging(du_idx));

  // Successfully resume UE.
  ASSERT_TRUE(
      resume_ue(du_ue_id_2, crnti_2, 0x4d8000, "1111010001000010", make_byte_buffer("000020400033b01cab").value()));

  // Await DL RRC Message Transfer containing the DL NAS Transport.
  ASSERT_TRUE(await_dl_rrc_message_transfer(du_idx));
}

TEST_F(
    cu_cp_rrc_inactive_test,
    when_multiple_dl_nas_transports_for_inactive_ue_are_received_then_paging_is_only_sent_once_and_after_successful_resume_dl_nas_transports_are_forwarded_to_ue)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BAS-16-1-a");

  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and handle it.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Inject first DL NAS Transport and await F1AP Paging.
  ASSERT_TRUE(send_dl_nas_transport_and_await_f1ap_paging(du_idx));

  // Inject second DL NAS Transport (no paging should be sent).
  ASSERT_TRUE(send_dl_nas_transport(du_idx));

  // Successfully resume UE.
  ASSERT_TRUE(
      resume_ue(du_ue_id_2, crnti_2, 0x4d8000, "1111010001000010", make_byte_buffer("000020400033b01cab").value()));

  // Await first DL RRC Message Transfer containing the DL NAS Transport.
  ASSERT_TRUE(await_dl_rrc_message_transfer(du_idx));

  // Await second DL RRC Message Transfer containing the DL NAS Transport.
  ASSERT_TRUE(await_dl_rrc_message_transfer(du_idx));
}

TEST_F(cu_cp_rrc_inactive_test, when_ran_paging_timer_expires_then_ue_release_is_requested)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BAS-16-1-a");

  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Inject Inactivity Notification and handle it.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Check metrics for RRC inactive transition.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.dus[0].rrc_metrics.mean_nof_inactive_rrc_connections, 1);
  ASSERT_EQ(report.dus[0].rrc_metrics.max_nof_inactive_rrc_connections, 1);

  // Inject E1AP DL Data Notification and await F1AP Paging.
  ASSERT_TRUE(
      send_e1ap_dl_data_notification_and_await_f1ap_paging(du_idx, ue_ctx->cu_cp_e1ap_id.value(), cu_up_e1ap_id));

  // Timeout RAN paging and await NGAP UE Context Release Request.
  ASSERT_TRUE(timeout_ran_paging_and_await_ngap_ue_context_release_request());
}

TEST_F(cu_cp_rrc_inactive_test, when_ue_resumes_during_location_measurements_then_du_receives_the_location_meas_info)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-3-b");

  connect_ue_with_rrc_inactive_support();

  // The UE starts location measurements. The first UL PDCP COUNT on SRB1 after the attach is 8.
  get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id,
      ue_ctx->cu_ue_id.value(),
      srb_id_t::srb1,
      generate_protected_ul_dcch_pdu(
          test_helpers::create_location_meas_ind(test_helpers::create_nr_prs_location_meas_info()), 8)));
  ASSERT_TRUE(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu));
  get_du(du_idx).push_ul_pdu(
      test_helpers::generate_ue_context_modification_response(du_ue_id, ue_ctx->cu_ue_id.value(), crnti, {}, {}));

  // The UE goes to RRC Inactive.
  ASSERT_TRUE(send_ue_level_bearer_context_inactivity_notification_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ue_context_release_command());
  ASSERT_TRUE(send_f1ap_ue_context_release_complete());

  // The UE resumes. The UE Context Setup Request carries the location measurements of the UE.
  ASSERT_TRUE(send_init_ul_rrc_message_transfer_and_await_ue_context_setup_request());
  const auto& req = f1ap_pdu.pdu.init_msg().value.ue_context_setup_request();
  ASSERT_TRUE(req->cu_to_du_rrc_info.ie_exts_present);
  ASSERT_TRUE(req->cu_to_du_rrc_info.ie_exts.location_meas_info_present);
  byte_buffer   expected_info;
  asn1::bit_ref bref{expected_info};
  ASSERT_EQ(test_helpers::create_nr_prs_location_meas_info().pack(bref), asn1::OCUDUASN_SUCCESS);
  EXPECT_EQ(req->cu_to_du_rrc_info.ie_exts.location_meas_info, expected_info);
}

TEST_F(cu_cp_rrc_inactive_test, when_location_reporting_is_configured_and_ue_resumes_then_location_report_is_sent)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Drain any pending NGAP messages.
  while (get_amf().try_pop_rx_pdu(ngap_pdu)) {
  }

  // Configure change_of_serving_cell_and_ue_presence_in_the_area_of_interest reporting, so that the report is not
  // suppressed when the UE resumes on the same cell (cell-change-only suppresses same-cell duplicate reports).
  get_amf().push_tx_pdu(generate_location_reporting_control_message_with_cell_change_and_ue_presence(
      ue_ctx->amf_ue_id.value(), ue_ctx->ran_ue_id.value(), {1}));
  ASSERT_TRUE(this->wait_for_ngap_tx_pdu(ngap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_location_report(ngap_pdu));

  // Transition UE to RRC Inactive state.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Resume UE from RRC Inactive (UE connects from a new cell with new C-RNTI).
  ASSERT_TRUE(
      resume_ue(du_ue_id_2, crnti_2, 0x4d8000, "1111010001000010", make_byte_buffer("000020400033b01cab").value()));

  // Expect a Location Report to be sent to the AMF after the resume.
  ASSERT_TRUE(this->wait_for_ngap_tx_pdu(ngap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_location_report(ngap_pdu));

  const auto& location_report = ngap_pdu.pdu.init_msg().value.location_report();
  ASSERT_EQ(location_report->amf_ue_ngap_id, to_underlying(ue_ctx->amf_ue_id.value()));
  ASSERT_EQ(location_report->ran_ue_ngap_id, to_underlying(ue_ctx->ran_ue_id.value()));
  ASSERT_EQ(location_report->location_report_request_type.event_type,
            asn1::ngap::event_type_opts::options::change_of_serving_cell_and_ue_presence_in_the_area_of_interest);
}

TEST_F(cu_cp_rrc_inactive_test,
       when_only_cell_change_reporting_is_configured_and_ue_resumes_to_same_cell_then_no_report_is_sent)
{
  // Connect UE with RRC Inactive support.
  connect_ue_with_rrc_inactive_support();

  // Configure change_of_serve_cell reporting. An immediate report is sent upon configuration.
  get_amf().push_tx_pdu(generate_location_reporting_control_message_with_cell_change(ue_ctx->amf_ue_id.value(),
                                                                                     ue_ctx->ran_ue_id.value()));
  ASSERT_TRUE(this->wait_for_ngap_tx_pdu(ngap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_location_report(ngap_pdu));

  // Transition UE to RRC Inactive state.
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // Resume UE from RRC Inactive on the same cell — no cell change occurred.
  ASSERT_TRUE(
      resume_ue(du_ue_id_2, crnti_2, 0x4d8000, "1111010001000010", make_byte_buffer("000020400033b01cab").value()));

  // No Location Report should be sent since the serving cell did not change.
  ASSERT_FALSE(this->wait_for_ngap_tx_pdu(ngap_pdu, std::chrono::milliseconds{5}));
}

/// Fixture for a peer retrieving one of our suspended UEs over Xn (TS 38.423 section 8.2.4), i.e. this node answering
/// a Retrieve UE Context Request that identifies the UE by the I-RNTI it was suspended with.
class cu_cp_rrc_inactive_xn_test : public cu_cp_rrc_inactive_test
{
public:
  cu_cp_rrc_inactive_xn_test() : cu_cp_rrc_inactive_test(true) {}

  /// Injects a Retrieve UE Context Request identifying the UE by an I-RNTI and returns the cause the CU-CP rejected it
  /// with. The ResumeMAC-I is not the one the UE would have computed, so a resolved UE is always rejected -- but with
  /// a different cause than an unresolved one.
  [[nodiscard]] std::optional<asn1::xnap::cause_c> send_retrieve_request_and_get_rejection_cause(uint32_t i_rnti_value)
  {
    get_xnc_cu_cp(xnc_peer_idx)
        .push_tx_pdu(generate_retrieve_ue_context_request_for_resume(
            peer_xnap_ue_id_t::min, short_i_rnti_t::from_uint(i_rnti_value).value(), xnc_peer_served_nci()));

    if (!this->wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu)) {
      return std::nullopt;
    }
    if (!test_helpers::is_valid_retrieve_ue_context_failure(xnap_pdu)) {
      return std::nullopt;
    }

    return xnap_pdu.pdu.unsuccessful_outcome().value.retrieve_ue_context_fail()->cause;
  }

  static constexpr unsigned xnc_peer_idx = 0;

  xnap_message xnap_pdu;
};

TEST_F(cu_cp_rrc_inactive_xn_test, when_peer_retrieves_suspended_ue_by_i_rnti_then_ue_context_is_resolved)
{
  connect_ue_with_rrc_inactive_support();
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  const std::optional<asn1::xnap::cause_c> cause = send_retrieve_request_and_get_rejection_cause(0x4d8000);
  ASSERT_TRUE(cause.has_value()) << "CU-CP did not answer the Retrieve UE Context Request";

  // The UE was resolved from the I-RNTI and rejected on the ResumeMAC-I, not for being unknown.
  ASSERT_EQ(cause->type(), asn1::xnap::cause_c::types_opts::radio_network);
  ASSERT_EQ(cause->radio_network(), asn1::xnap::cause_radio_network_layer_opts::unspecified)
      << "The suspended UE was not resolved from its I-RNTI";
}

TEST_F(cu_cp_rrc_inactive_xn_test, when_peer_retrieves_ue_by_unknown_i_rnti_then_retrieval_is_rejected)
{
  connect_ue_with_rrc_inactive_support();
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  const std::optional<asn1::xnap::cause_c> cause = send_retrieve_request_and_get_rejection_cause(0x668000);
  ASSERT_TRUE(cause.has_value()) << "CU-CP did not answer the Retrieve UE Context Request";

  ASSERT_EQ(cause->type(), asn1::xnap::cause_c::types_opts::radio_network);
  ASSERT_EQ(cause->radio_network(), asn1::xnap::cause_radio_network_layer_opts::ue_context_id_not_known);
}

TEST_F(cu_cp_rrc_inactive_xn_test, when_i_rnti_was_allocated_locally_then_resume_is_handled_without_a_retrieval)
{
  connect_ue_with_rrc_inactive_support();
  ASSERT_TRUE(trigger_rrc_inactive(du_ue_id));

  // The I-RNTI carries the node identifier of this node, so the context is here and no peer has to be asked for it.
  ASSERT_TRUE(
      resume_ue(du_ue_id_2, crnti_2, 0x4d8000, "1111010001000010", make_byte_buffer("000020400033b01cab").value()));

  ASSERT_FALSE(this->get_xnc_cu_cp(xnc_peer_idx).try_pop_rx_pdu(xnap_pdu))
      << "A retrieval was attempted for an I-RNTI this node allocated itself";
}
