// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_cp_test_environment.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/e1ap/e1ap_cu_cp_test_messages.h"
#include "tests/test_doubles/e1ap/e1ap_test_message_validators.h"
#include "tests/test_doubles/f1ap/f1ap_test_message_validators.h"
#include "tests/test_doubles/ngap/ngap_test_message_validators.h"
#include "tests/test_doubles/rrc/rrc_test_message_validators.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "tests/unittests/ngap/ngap_test_messages.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents_ue.h"
#include "ocudu/asn1/ngap/ngap_pdu_contents.h"
#include "ocudu/e1ap/common/e1ap_types.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/f1ap/f1ap_ue_id_types.h"
#include "ocudu/ngap/ngap_message.h"
#include "ocudu/ngap/ngap_types.h"
#include "ocudu/ran/cu_types.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;

// Returns the single PDU session of an E1AP Bearer Context Setup Request.
static const asn1::e1ap::pdu_session_res_to_setup_item_s& get_pdu_session_res_to_setup_item(const e1ap_message& msg)
{
  const auto& ng_ran_bearer_ctxt = msg.pdu.init_msg()
                                       .value.bearer_context_setup_request()
                                       ->sys_bearer_context_setup_request.ng_ran_bearer_context_setup_request();
  return ng_ran_bearer_ctxt.pdu_session_res_to_setup_list[0];
}

class cu_cp_inter_cu_ng_handover_test : public cu_cp_test_environment, public ::testing::Test
{
public:
  explicit cu_cp_inter_cu_ng_handover_test(cu_cp_test_env_params env_params = {}) :
    cu_cp_test_environment(std::move(env_params))
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MOB-4", "MVP-FUNC-MOB-1-e");

    // Run NG setup to completion.
    run_ng_setup();

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

  [[nodiscard]] bool attach_ue()
  {
    if (!cu_cp_test_environment::attach_ue(
            du_idx, cu_up_idx, du_ue_id, crnti, amf_ue_id, cu_up_e1ap_id, psi, drb_id_t::drb1, qfi)) {
      return false;
    }
    ue_ctx = this->find_ue_context(du_idx, du_ue_id);

    return ue_ctx != nullptr;
  }

  [[nodiscard]] bool
  send_handover_request_and_await_bearer_context_setup_request(const handover_request_params& ho_params = {})
  {
    report_fatal_error_if_not(not this->get_amf(target_amf_idx).try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");

    // Inject Handover Request and wait for Bearer Context Setup Request.
    get_amf(target_amf_idx).push_tx_pdu(generate_valid_handover_request(amf_ue_id, ho_params));
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Setup Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_setup_request(e1ap_pdu),
                              "Invalid Bearer Context Setup Request");

    cu_cp_e1ap_id =
        int_to_gnb_cu_cp_ue_e1ap_id(e1ap_pdu.pdu.init_msg().value.bearer_context_setup_request()->gnb_cu_cp_ue_e1ap_id);
    return true;
  }

  [[nodiscard]] bool
  send_bearer_context_setup_response_and_await_ue_context_setup_request(bool with_data_forwarding_info = false)
  {
    // Inject Bearer Context Setup Response and wait for UE Context Setup Request.
    get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_setup_response(
        cu_cp_e1ap_id,
        cu_up_e1ap_id,
        {{uint_to_pdu_session_id(1), {{drb_id_t::drb1, uint_to_qos_flow_id(1)}}}},
        {},
        with_data_forwarding_info));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive UE Context Setup Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_setup_request(f1ap_pdu),
                              "Invalid UE Context Setup Request");

    report_fatal_error_if_not(f1ap_pdu.pdu.init_msg().value.ue_context_setup_request()->serving_cell_mo_present,
                              "ServingCellMO not present in UE Context Setup Request");

    ue_context_setup_req_serving_cell_mo = f1ap_pdu.pdu.init_msg().value.ue_context_setup_request()->serving_cell_mo;

    cu_ue_id = int_to_gnb_cu_ue_f1ap_id(f1ap_pdu.pdu.init_msg().value.ue_context_setup_request()->gnb_cu_ue_f1ap_id);
    return true;
  }

  [[nodiscard]] bool send_ue_context_setup_response_and_await_bearer_context_modification_request()
  {
    // Inject UE Context Setup Response and wait for Bearer Context Modification Request.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_setup_response(
        cu_ue_id,
        du_ue_id,
        crnti,
        make_byte_buffer(
            "5c04c00604b0701f00811a0f020001273b8c02692f30004d25e24040008c8040a26418d6d8d76006e08040000101000083446a48d8"
            "02692f1200000464e35b63224f8060664abff0124e9106e28dc61b8e372c6fbf56c70eb00442c0680182c4601c020521004930b2a0"
            "003fe00000000060dc2108000780594008300000020698101450a000e3890000246aac90838002081840a1839389142c60d1c3c811"
            "00000850000800b50001000850101800b50102000850202800b50203000850303800b503040c885010480504014014120580505018"
            "01416068050601c0141a0780507020314100880905204963028711d159e26f2681d2083c5df81821c00000038ffd294a5294f28160"
            "00021976000000000000000000108ad5450047001800082000e21009c400e0202108001c420138401c080441000388402708038180"
            "842000710804e18070401104000e21009c300080000008218081018201c1a0001c71000000080100020180020240088029800008c4"
            "0089c7001800")
            .value()));

    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Modification Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool send_bearer_context_modification_response_and_await_handover_request_ack()
  {
    // Inject Bearer Context Modification Response and wait for Handover Request Ack.
    get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_response(cu_cp_e1ap_id, cu_up_e1ap_id));
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu, std::chrono::milliseconds{500}, target_amf_idx),
                              "Failed to receive Handover Request Ack");
    report_fatal_error_if_not(test_helpers::is_valid_handover_request_ack(ngap_pdu), "Invalid Handover Request Ack");
    return true;
  }

  [[nodiscard]] bool send_bearer_context_modification_response()
  {
    // Inject Bearer Context Modification Response.
    get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_response(cu_cp_e1ap_id, cu_up_e1ap_id));
    return true;
  }

  [[nodiscard]]
  bool
  send_dl_ran_status_transfer_and_await_bearer_context_modification_request(const std::vector<drb_id_t>& drb_ids = {})
  {
    get_amf(target_amf_idx).push_tx_pdu(generate_valid_dl_ran_status_transfer(amf_ue_id, {}, drb_ids));
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Modification Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request()
  {
    // Inject UL RRC Message (containing RRC Reconfiguration Complete) and wait for Handover Notify.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
        du_ue_id, cu_ue_id, srb_id_t::srb1, make_byte_buffer("800008005b7d9d03").value()));
    return await_handover_notify_and_ue_context_modification_request();
  }

  [[nodiscard]] bool send_rrc_reconfiguration_complete()
  {
    // Inject UL RRC Message containing RRC Reconfiguration Complete.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
        du_ue_id, cu_ue_id, srb_id_t::srb1, make_byte_buffer("800008005b7d9d03").value()));
    return true;
  }

  [[nodiscard]] bool await_handover_notify_and_ue_context_modification_request()
  {
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu, std::chrono::milliseconds{500}, target_amf_idx),
                              "Failed to receive Handover Notify");
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive UE context modification response");
    report_fatal_error_if_not(test_helpers::is_valid_handover_notify(ngap_pdu), "Invalid Handover Notify");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu),
                              "Invalid F1AP UE Context Modification Request");
    return true;
  }

  [[nodiscard]] bool send_rrc_measurement_report_and_await_handover_required(
      gnb_cu_ue_f1ap_id_t f1ap_cu_ue_id,
      gnb_du_ue_f1ap_id_t f1ap_du_ue_id,
      byte_buffer         rrc_meas_report = make_byte_buffer("000800410004015f741fe0804bf183fcaa6e9699").value())
  {
    // Inject UL RRC Message (containing RRC Measurement Report) and wait for Handover Required.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
        f1ap_du_ue_id, f1ap_cu_ue_id, srb_id_t::srb1, std::move(rrc_meas_report)));
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive Handover Required");
    report_fatal_error_if_not(test_helpers::is_valid_handover_required(ngap_pdu), "Invalid Handover Required");
    report_fatal_error_if_not(
        test_helpers::is_valid_rrc_handover_preparation_info(test_helpers::get_rrc_container(ngap_pdu)),
        "Invalid Handover Preparation Info");

    report_fatal_error_if_not(ngap_pdu.pdu.init_msg()
                                      .value.ho_required()
                                      ->target_id.target_ran_node_id()
                                      .global_ran_node_id.global_gnb_id()
                                      .gnb_id.gnb_id()
                                      .to_number() == 412U,
                              "Wrong target gNB-id in Handover Required");

    amf_ue_id_2 = uint_to_amf_ue_id(ngap_pdu.pdu.init_msg().value.ho_required()->amf_ue_ngap_id);
    ran_ue_id_2 = uint_to_ran_ue_id(ngap_pdu.pdu.init_msg().value.ho_required()->ran_ue_ngap_id);

    return true;
  }

  [[nodiscard]] bool send_rrc_measurement_report(
      gnb_cu_ue_f1ap_id_t f1ap_cu_ue_id,
      gnb_du_ue_f1ap_id_t f1ap_du_ue_id,
      byte_buffer         rrc_meas_report = make_byte_buffer("000800410004015f741fe0804bf183fcaa6e9699").value())
  {
    // Inject UL RRC Message (containing RRC Measurement Report).
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
        f1ap_du_ue_id, f1ap_cu_ue_id, srb_id_t::srb1, std::move(rrc_meas_report)));

    return true;
  }

  [[nodiscard]] bool send_handover_preparation_failure()
  {
    // Inject Handover Preparation Failure.
    get_amf().push_tx_pdu(generate_handover_preparation_failure(ue_ctx->amf_ue_id.value(), ue_ctx->ran_ue_id.value()));
    return true;
  }

  [[nodiscard]] bool timeout_handover_command_and_await_handover_cancel()
  {
    // Fail Handover Preparation (AMF doesn't respond) and await Handover Cancel.
    if (tick_until(std::chrono::milliseconds(1000), [&]() { return false; })) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive Handover Cancel");
    report_fatal_error_if_not(test_helpers::is_valid_handover_cancel(ngap_pdu), "Invalid Handover Cancel");
    return true;
  }

  [[nodiscard]] bool send_handover_cancel_ack()
  {
    // Inject Handover Cancel Ack.
    get_amf().push_tx_pdu(generate_handover_cancel_ack(ue_ctx->amf_ue_id.value(), ue_ctx->ran_ue_id.value()));
    return true;
  }

  [[nodiscard]] bool
  send_handover_command_and_await_ue_context_modification_request(amf_ue_id_t ngap_amf_ue_id,
                                                                  ran_ue_id_t ngap_ran_ue_id,
                                                                  bool        with_data_forwarding_info = false,
                                                                  bool        with_forwarding_tunnel    = true)
  {
    // Inject Handover Command and wait for UE Context Modification Request (containing RRC Reconfiguration).
    get_amf().push_tx_pdu(generate_valid_handover_command(
        ngap_amf_ue_id, ngap_ran_ue_id, with_data_forwarding_info, with_forwarding_tunnel));

    report_fatal_error_if_not(
        this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
        "Failed to receive F1AP UE Context Modification Request (containing RRC Reconfiguration)");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu),
                              "Invalid UE Context Modification Request");

    // Make sure RRC Reconfiguration is valid
    const byte_buffer& rrc_container = test_helpers::get_rrc_container(f1ap_pdu);
    report_fatal_error_if_not(
        test_helpers::is_valid_rrc_reconfiguration(test_helpers::extract_dl_dcch_msg(rrc_container),
                                                   false,
                                                   std::vector<srb_id_t>{srb_id_t::srb1, srb_id_t::srb2},
                                                   std::vector<drb_id_t>{drb_id_t::drb1},
                                                   {},
                                                   ue_context_setup_req_serving_cell_mo),
        "Invalid RRC Reconfiguration");

    return true;
  }

  [[nodiscard]] bool timeout_tng_reloc_overall_and_await_ue_context_release_request()
  {
    if (tick_until(std::chrono::milliseconds(1200), [&]() { return false; }, false)) {
      return false;
    }

    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu),
                              "Failed to receive NGAP UE Context Release Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_request(ngap_pdu),
                              "Invalid NGAP UE Context Release Request");

    const auto& release_request = ngap_pdu.pdu.init_msg().value.ue_context_release_request();
    report_fatal_error_if_not(release_request->cause.type() == asn1::ngap::cause_c::types_opts::radio_network,
                              "Invalid UE Context Release Request cause type");
    report_fatal_error_if_not(release_request->cause.radio_network() ==
                                  asn1::ngap::cause_radio_network_opts::tngrelocoverall_expiry,
                              "Invalid UE Context Release Request radio network cause");
    return true;
  }

  [[nodiscard]] bool
  send_ue_context_modification_response_and_await_bearer_context_modification_request(gnb_cu_ue_f1ap_id_t f1ap_cu_ue_id,
                                                                                      gnb_du_ue_f1ap_id_t f1ap_du_ue_id)
  {
    get_du(du_idx).push_ul_pdu(
        test_helpers::generate_ue_context_modification_response(f1ap_du_ue_id, f1ap_cu_ue_id, crnti));

    report_fatal_error_if_not(
        this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
        "Failed to receive E1AP Bearer Context Modification Request (containing PDCP SN status query)");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool
  send_bearer_context_modification_response_and_await_ul_status_transfer(gnb_cu_cp_ue_e1ap_id_t cu_cp_ue_e1ap_id,
                                                                         gnb_cu_up_ue_e1ap_id_t cu_up_ue_e1ap_id)
  {
    get_cu_up(cu_up_idx).push_tx_pdu(
        generate_bearer_context_modification_response_with_pdcp_status(cu_cp_ue_e1ap_id, cu_up_ue_e1ap_id));

    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu),
                              "Failed to transmist NGAP UL RAN Status transfer to AMF");

    report_fatal_error_if_not(test_helpers::is_valid_ul_ran_status_transfer(ngap_pdu),
                              "Invalid NGAP UL RAN Status Transfer");

    return true;
  }

  [[nodiscard]] bool send_ue_context_modification_response(gnb_cu_ue_f1ap_id_t f1ap_cu_ue_id,
                                                           gnb_du_ue_f1ap_id_t f1ap_du_ue_id)
  {
    // Inject UE Context Modification Response and wait for UE Context Release Command.
    get_du(du_idx).push_ul_pdu(
        test_helpers::generate_ue_context_modification_response(f1ap_du_ue_id, f1ap_cu_ue_id, crnti));
    return true;
  }

  [[nodiscard]] bool send_ue_context_modification_response_empty(gnb_cu_ue_f1ap_id_t f1ap_cu_ue_id,
                                                                 gnb_du_ue_f1ap_id_t f1ap_du_ue_id)
  {
    // Send an UE Context Modification Response without any DRBs.
    get_du(du_idx).push_ul_pdu(
        test_helpers::generate_ue_context_modification_response(f1ap_du_ue_id, f1ap_cu_ue_id, crnti, {}));
    return true;
  }

  [[nodiscard]] bool send_ngap_ue_context_release_command_and_await_bearer_context_release_command(
      amf_ue_id_t                       ngap_amf_ue_id,
      asn1::ngap::cause_radio_network_e cause = asn1::ngap::cause_radio_network_opts::radio_conn_with_ue_lost)
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");

    // Inject NGAP UE Context Release Command and wait for Bearer Context Release Command.
    get_amf().push_tx_pdu(generate_valid_ue_context_release_command_with_amf_ue_ngap_id(ngap_amf_ue_id, cause));
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_release_command(e1ap_pdu),
                              "Invalid Bearer Context Release Command");
    return true;
  }

  [[nodiscard]] bool send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command(
      gnb_cu_cp_ue_e1ap_id_t e1ap_cu_cp_ue_id,
      gnb_cu_up_ue_e1ap_id_t e1ap_cu_up_ue_id)
  {
    // Inject Bearer Context Release Complete and wait for F1AP UE Context Release Command.
    get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_release_complete(e1ap_cu_cp_ue_id, e1ap_cu_up_ue_id));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive UE Context Release Command");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_command(f1ap_pdu),
                              "Invalid UE Context Release Command");
    return true;
  }

  [[nodiscard]] bool
  send_f1ap_ue_context_release_complete_and_await_ngap_ue_context_release_complete(gnb_cu_ue_f1ap_id_t f1ap_cu_ue_id,
                                                                                   gnb_du_ue_f1ap_id_t f1ap_du_ue_id)
  {
    // Inject F1AP UE Context Release Complete and wait for N1AP UE Context Release Command.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_release_complete(f1ap_cu_ue_id, f1ap_du_ue_id));
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive UE Context Release Complete");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_complete(ngap_pdu),
                              "Invalid UE Context Release Complete");
    return true;
  }

  unsigned du_idx    = 0;
  unsigned cu_up_idx = 0;

  gnb_du_ue_f1ap_id_t du_ue_id = gnb_du_ue_f1ap_id_t::min;
  gnb_cu_ue_f1ap_id_t cu_ue_id;
  rnti_t              crnti     = to_rnti(0x4601);
  amf_ue_id_t         amf_ue_id = uint_to_amf_ue_id(
      test_rng::uniform_int<uint64_t>(to_underlying(amf_ue_id_t::min), to_underlying(amf_ue_id_t::max)));
  gnb_cu_up_ue_e1ap_id_t cu_up_e1ap_id = gnb_cu_up_ue_e1ap_id_t::min;
  gnb_cu_cp_ue_e1ap_id_t cu_cp_e1ap_id;

  std::optional<uint8_t> ue_context_setup_req_serving_cell_mo = std::nullopt;

  const ue_context* ue_ctx = nullptr;

  amf_ue_id_t amf_ue_id_2;
  ran_ue_id_t ran_ue_id_2;

  pdu_session_id_t psi = uint_to_pdu_session_id(1);
  qos_flow_id_t    qfi = uint_to_qos_flow_id(1);

  /// AMF that sends the Handover Request when this CU-CP is the target.
  unsigned target_amf_idx = 0;

  ngap_message ngap_pdu;
  f1ap_message f1ap_pdu;
  e1ap_message e1ap_pdu;
};

///////////////////////////////////////////////////////////////////////////////
//                             Source CU-CP
///////////////////////////////////////////////////////////////////////////////
TEST_F(cu_cp_inter_cu_ng_handover_test, when_handover_preparation_failure_is_received_then_handover_fails)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject RRC Measurement Report and await Handover Required.
  ASSERT_TRUE(
      send_rrc_measurement_report_and_await_handover_required(ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // Inject Handover Preparation Failure.
  ASSERT_TRUE(send_handover_preparation_failure());

  // Check that metrics contain the requested handover preparation.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 0);

  // STATUS: Handover Preparation failed and no further messages are sent to the AMF.
  report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                            "there are still NGAP messages to pop from AMF");
}

TEST_F(cu_cp_inter_cu_ng_handover_test, when_handover_command_times_out_then_handover_cancel_is_sent)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject RRC Measurement Report and await Handover Required.
  ASSERT_TRUE(
      send_rrc_measurement_report_and_await_handover_required(ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // timeout Handover Command and await Handover Cancel.
  ASSERT_TRUE(timeout_handover_command_and_await_handover_cancel());

  // Inject Handover Cancel Ack.
  ASSERT_TRUE(send_handover_cancel_ack());
}

TEST_F(cu_cp_inter_cu_ng_handover_test, when_handover_succeeds_then_amf_releases_ue)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject RRC Measurement Report and await Handover Required.
  ASSERT_TRUE(
      send_rrc_measurement_report_and_await_handover_required(ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // Check that metrics contain the requested handover preparation.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 0);

  // Send Handover Command and await UE Context Modification Request (with RRC Reconfiguration).
  ASSERT_TRUE(send_handover_command_and_await_ue_context_modification_request(ue_ctx->amf_ue_id.value(),
                                                                              ue_ctx->ran_ue_id.value()));

  // Send Handover UE Context Modification Response and await for Bearer Context Modification Request (to query PDCP
  // state).
  ASSERT_TRUE(send_ue_context_modification_response_and_await_bearer_context_modification_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // Check that the metrics report contains the requested handover execution.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_inter_gnb_handover_executions_requested, 1U);
  ASSERT_EQ(report.mobility.nof_successful_inter_gnb_handover_executions, 0U);

  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ul_status_transfer(ue_ctx->cu_cp_e1ap_id.value(),
                                                                                     ue_ctx->cu_up_e1ap_id.value()));

  // Check that metrics contain the requested and successful handover preparation.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 1);

  // Inject NGAP UE Context Release Command and await Bearer Context Release Command.
  ASSERT_TRUE(send_ngap_ue_context_release_command_and_await_bearer_context_release_command(
      ue_ctx->amf_ue_id.value(), asn1::ngap::cause_radio_network_opts::successful_ho));

  // Check that the metrics report contains the successful handover execution.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_successful_inter_gnb_handover_executions, 1U);

  // Inject Bearer Context Release Complete and await F1AP UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command(
      ue_ctx->cu_cp_e1ap_id.value(), ue_ctx->cu_up_e1ap_id.value()));

  // Inject F1AP UE Context Release Complete and await NGAP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete_and_await_ngap_ue_context_release_complete(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // STATUS: UE should be removed at this stage
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0) << "UE should be removed";
}

// The forwarding tunnels the Handover Command reports must be handed to the source CU-UP, so that it sends the data it
// still holds for the UE to them (TS 38.413 section 9.3.4.10, TS 37.483 section 9.3.2.6). For indirect data forwarding
// these endpoints are the ones the 5GC inserted, so they must be taken from the Handover Command and never from what
// this node itself advertised.
TEST_F(cu_cp_inter_cu_ng_handover_test, when_handover_command_reports_fwd_tunnels_then_source_up_is_pointed_at_them)
{
  ASSERT_TRUE(attach_ue());

  ASSERT_TRUE(
      send_rrc_measurement_report_and_await_handover_required(ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // Inject a Handover Command carrying a PDU session level forwarding tunnel.
  ASSERT_TRUE(send_handover_command_and_await_ue_context_modification_request(
      ue_ctx->amf_ue_id.value(), ue_ctx->ran_ue_id.value(), true));

  ASSERT_TRUE(send_ue_context_modification_response_and_await_bearer_context_modification_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // The Bearer Context Modification Request points the CU-UP at the forwarding tunnel.
  const auto& bearer_ctxt_mod_req = e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request();
  ASSERT_TRUE(bearer_ctxt_mod_req->sys_bearer_context_mod_request_present);
  const auto& ng_ran_mod_req = bearer_ctxt_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request();
  ASSERT_TRUE(ng_ran_mod_req.pdu_session_res_to_modify_list_present);
  ASSERT_EQ(ng_ran_mod_req.pdu_session_res_to_modify_list.size(), 1U);
  const asn1::e1ap::pdu_session_res_to_modify_item_s& pdu_session = ng_ran_mod_req.pdu_session_res_to_modify_list[0];

  ASSERT_TRUE(pdu_session.pdu_session_data_forwarding_info_present);
  ASSERT_TRUE(pdu_session.pdu_session_data_forwarding_info.dl_data_forwarding_present);
  ASSERT_FALSE(pdu_session.pdu_session_data_forwarding_info.ul_data_forwarding_present);
  ASSERT_TRUE(pdu_session.pdu_session_data_forwarding_info.ie_exts.data_forwardingto_ng_ran_qos_flow_info_list_present);
  ASSERT_EQ(pdu_session.pdu_session_data_forwarding_info.ie_exts.data_forwardingto_ng_ran_qos_flow_info_list.size(),
            1U);

  // A DRB level tunnel the 5GC relayed is programmed too, so that this DRB's PDCP SDUs keep their sequence numbers.
  ASSERT_EQ(pdu_session.drb_to_modify_list_ng_ran.size(), 1U);
  const auto& drb_to_modify = pdu_session.drb_to_modify_list_ng_ran[0];
  ASSERT_TRUE(drb_to_modify.drb_data_forwarding_info_present);
  ASSERT_TRUE(drb_to_modify.drb_data_forwarding_info.dl_data_forwarding_present);
  ASSERT_FALSE(drb_to_modify.drb_data_forwarding_info.ul_data_forwarding_present);

  // The DRB level and the PDU session level endpoints are the distinct ones the Handover Command reported.
  ASSERT_NE(drb_to_modify.drb_data_forwarding_info.dl_data_forwarding.gtp_tunnel().gtp_teid.to_number(),
            pdu_session.pdu_session_data_forwarding_info.dl_data_forwarding.gtp_tunnel().gtp_teid.to_number());

  // The PDCP SN status is still queried alongside the forwarding tunnel.
  ASSERT_TRUE(drb_to_modify.pdcp_sn_status_request_present);

  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ul_status_transfer(ue_ctx->cu_cp_e1ap_id.value(),
                                                                                     ue_ctx->cu_up_e1ap_id.value()));
}

// A 5GC may report QoS flows as accepted for data forwarding without providing an endpoint to send them to, which
// Amarisoft LTEMME does when only DRB level tunnels are advertised. The source must not program a forwarding tunnel
// for such a PDU session, and must still query the PDCP SN status of its DRBs.
TEST_F(cu_cp_inter_cu_ng_handover_test,
       when_handover_command_reports_no_fwd_tunnel_then_source_up_is_not_pointed_at_one)
{
  ASSERT_TRUE(attach_ue());

  ASSERT_TRUE(
      send_rrc_measurement_report_and_await_handover_required(ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // Inject a Handover Command listing the QoS flows to be forwarded but carrying no forwarding tunnel.
  ASSERT_TRUE(send_handover_command_and_await_ue_context_modification_request(
      ue_ctx->amf_ue_id.value(), ue_ctx->ran_ue_id.value(), true, false));

  ASSERT_TRUE(send_ue_context_modification_response_and_await_bearer_context_modification_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  const auto& bearer_ctxt_mod_req = e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request();
  ASSERT_TRUE(bearer_ctxt_mod_req->sys_bearer_context_mod_request_present);
  const auto& ng_ran_mod_req = bearer_ctxt_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request();
  ASSERT_TRUE(ng_ran_mod_req.pdu_session_res_to_modify_list_present);
  ASSERT_EQ(ng_ran_mod_req.pdu_session_res_to_modify_list.size(), 1U);
  const asn1::e1ap::pdu_session_res_to_modify_item_s& pdu_session = ng_ran_mod_req.pdu_session_res_to_modify_list[0];

  // No forwarding tunnel is programmed at either level.
  ASSERT_FALSE(pdu_session.pdu_session_data_forwarding_info_present);
  ASSERT_EQ(pdu_session.drb_to_modify_list_ng_ran.size(), 1U);
  const auto& drb_to_modify = pdu_session.drb_to_modify_list_ng_ran[0];
  ASSERT_FALSE(drb_to_modify.drb_data_forwarding_info_present);

  // The PDCP SN status is still queried, so the handover itself is unaffected.
  ASSERT_TRUE(drb_to_modify.pdcp_sn_status_request_present);

  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ul_status_transfer(ue_ctx->cu_cp_e1ap_id.value(),
                                                                                     ue_ctx->cu_up_e1ap_id.value()));
}

TEST_F(cu_cp_inter_cu_ng_handover_test,
       when_tng_reloc_overall_expires_after_handover_command_then_source_requests_ue_context_release)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject RRC Measurement Report and await Handover Required.
  ASSERT_TRUE(
      send_rrc_measurement_report_and_await_handover_required(ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // Send Handover Command and await UE Context Modification Request (with RRC Reconfiguration).
  ASSERT_TRUE(send_handover_command_and_await_ue_context_modification_request(ue_ctx->amf_ue_id.value(),
                                                                              ue_ctx->ran_ue_id.value()));

  // Complete the source-side UE context modification and PDCP status transfer steps.
  ASSERT_TRUE(send_ue_context_modification_response_and_await_bearer_context_modification_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ul_status_transfer(ue_ctx->cu_cp_e1ap_id.value(),
                                                                                     ue_ctx->cu_up_e1ap_id.value()));

  // Let TNGRELOCoverall expire and await the source UE Context Release Request.
  ASSERT_TRUE(timeout_tng_reloc_overall_and_await_ue_context_release_request());
}

TEST_F(cu_cp_inter_cu_ng_handover_test, when_ncell_is_not_strong_enough_then_ho_from_periodic_report_is_not_triggered)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject periodic RRC Measurement Report.
  ASSERT_TRUE(send_rrc_measurement_report(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value(), make_byte_buffer("0008004000014fc0806a1c9cfdaee8").value()));

  // STATUS: No message is sent to the AMF.
  report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                            "there are still NGAP messages to pop from AMF");

  // Check that metrics don't contain a requested or successful handover preparation.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 0);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 0);
}

TEST_F(cu_cp_inter_cu_ng_handover_test, when_ncell_is_strong_enough_then_ho_from_periodic_report_is_triggered)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject periodic RRC Measurement Report.
  ASSERT_TRUE(send_rrc_measurement_report_and_await_handover_required(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value(), make_byte_buffer("00080040000147c0806a42830aecee").value()));

  // Check that metrics contain the requested handover preparation.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 0);

  // Send Handover Command and await UE Context Modification Request (with RRC Reconfiguration).
  ASSERT_TRUE(send_handover_command_and_await_ue_context_modification_request(ue_ctx->amf_ue_id.value(),
                                                                              ue_ctx->ran_ue_id.value()));

  // Send Handover UE Context Modification Response and await for Bearer Context Modification Request (to query PDCP
  // state).
  ASSERT_TRUE(send_ue_context_modification_response_and_await_bearer_context_modification_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ul_status_transfer(ue_ctx->cu_cp_e1ap_id.value(),
                                                                                     ue_ctx->cu_up_e1ap_id.value()));

  // Check that metrics contain the requested and successful handover preparation.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 1);

  // Inject NGAP UE Context Release Command and await Bearer Context Release Command.
  ASSERT_TRUE(send_ngap_ue_context_release_command_and_await_bearer_context_release_command(ue_ctx->amf_ue_id.value()));

  // Inject Bearer Context Release Complete and await F1AP UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command(
      ue_ctx->cu_cp_e1ap_id.value(), ue_ctx->cu_up_e1ap_id.value()));

  // Inject F1AP UE Context Release Complete and await NGAP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete_and_await_ngap_ue_context_release_complete(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // STATUS: UE should be removed at this stage
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0) << "UE should be removed";
}

///////////////////////////////////////////////////////////////////////////////
//                             Target CU-CP
///////////////////////////////////////////////////////////////////////////////
TEST_F(cu_cp_inter_cu_ng_handover_test, when_handover_request_received_then_handover_notify_is_sent)
{
  // Inject Handover Request and await Bearer Context Setup Request.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request());

  // Check that the metrics report contains the requested handover resource allocation.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_inter_gnb_handover_resource_allocations_requested, 1U);
  ASSERT_EQ(report.mobility.nof_successful_inter_gnb_handover_resource_allocations, 0U);

  // Inject Bearer Context Setup Response and await UE Context Setup Request.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Check that the metrics report contains the successful handover resource allocation.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_inter_gnb_handover_resource_allocations_requested, 1U);
  ASSERT_EQ(report.mobility.nof_successful_inter_gnb_handover_resource_allocations, 1U);

  // Inject NGAP DL RAN Status Transfer and Bearer Context Modification Response.
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and ACK the PDCP state modification.
  ASSERT_TRUE(send_bearer_context_modification_response());

  // Inject RRC Reconfiguration Complete and await Handover Notify and UE Context Modification Request.
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());

  // Inject UE Context Modification Response to ACK the RRC reconfiguration complete indicator.
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));

  // Check that the UE is connected to the AMF by injecting a deregistration request and make sure its forwarded.
  f1ap_message ul_rrc_msg_transfer = test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id,
      cu_ue_id,
      srb_id_t::srb2,
      make_byte_buffer("00003a0c3f011ef64ea681bf0022888005f9007888010020600003458007c887be").value());
  get_du(du_idx).push_ul_pdu(ul_rrc_msg_transfer);
  ASSERT_TRUE(this->wait_for_ngap_tx_pdu(ngap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_ul_nas_transport_message(ngap_pdu));
}

// A UE that accesses this target through an inter-CU handover applies the AS security configuration signalled in the
// HandoverCommand, so the security context is active once the handover completed. The AMF may set up further PDU
// sessions right afterwards (TS 38.413 Section 8.2.1), which the target must admit.
TEST_F(cu_cp_inter_cu_ng_handover_test, when_handover_completed_at_target_then_pdu_session_setup_request_is_admitted)
{
  // Inject Handover Request and await Bearer Context Setup Request.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request());

  // Inject Bearer Context Setup Response and await UE Context Setup Request.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  const ran_ue_id_t ran_ue_id =
      uint_to_ran_ue_id(ngap_pdu.pdu.successful_outcome().value.ho_request_ack()->ran_ue_ngap_id);

  // Inject NGAP DL RAN Status Transfer and Bearer Context Modification Response.
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and ACK the PDCP state modification.
  ASSERT_TRUE(send_bearer_context_modification_response());

  // Inject RRC Reconfiguration Complete and await Handover Notify and UE Context Modification Request.
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());

  // Inject UE Context Modification Response to ACK the RRC reconfiguration complete indicator.
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));

  // Inject PDU Session Resource Setup Request for a new PDU session and await Bearer Context Modification Request.
  const pdu_session_id_t psi2 = uint_to_pdu_session_id(2);
  get_amf().push_tx_pdu(generate_valid_pdu_session_resource_setup_request_message(
      amf_ue_id, ran_ue_id, {{psi2, {pdu_session_type_t::ipv4, {{uint_to_qos_flow_id(2), 9}}}}}));
  ASSERT_TRUE(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu));

  // The new PDU session must be requested at the CU-UP.
  const auto& bearer_ctxt_mod_req = e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request();
  ASSERT_TRUE(bearer_ctxt_mod_req->sys_bearer_context_mod_request_present);
  const auto& pdu_sessions_to_setup =
      bearer_ctxt_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request()
          .pdu_session_res_to_setup_mod_list;
  ASSERT_EQ(pdu_sessions_to_setup.size(), 1U);
  ASSERT_EQ(pdu_sessions_to_setup[0].pdu_session_id, to_underlying(psi2));

  // Let the CU-UP reject the new PDU session to conclude the procedure.
  get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_failure(cu_cp_e1ap_id, cu_up_e1ap_id));
  ASSERT_TRUE(this->wait_for_ngap_tx_pdu(ngap_pdu));
  ASSERT_TRUE(test_helpers::is_valid_pdu_session_resource_setup_response(ngap_pdu));
  ASSERT_TRUE(test_helpers::is_expected_pdu_session_resource_setup_response(ngap_pdu, {}, {psi2}));
}

// When the source proposes DL data forwarding for its QoS flows, the target asks its CU-UP for a PDU session level
// forwarding tunnel carrying those flows (TS 38.413 section 9.3.1.33, TS 37.483 section 9.3.2.5).
TEST_F(cu_cp_inter_cu_ng_handover_test, when_source_proposes_dl_data_forwarding_then_target_requests_fwd_tunnels)
{
  // Inject a Handover Request proposing DL data forwarding and await the Bearer Context Setup Request.
  handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(ho_params));

  const asn1::e1ap::pdu_session_res_to_setup_item_s& e1ap_pdu_session = get_pdu_session_res_to_setup_item(e1ap_pdu);

  ASSERT_TRUE(e1ap_pdu_session.pdu_session_data_forwarding_info_request_present);
  ASSERT_EQ(e1ap_pdu_session.pdu_session_data_forwarding_info_request.data_forwarding_request.value,
            asn1::e1ap::data_forwarding_request_opts::dl);

  // The proposed QoS flow is listed as forwarded over that tunnel.
  const auto& flows_on_tunnel =
      e1ap_pdu_session.pdu_session_data_forwarding_info_request.qos_flows_forwarded_on_fwd_tunnels;
  ASSERT_EQ(flows_on_tunnel.size(), 1U);
  ASSERT_EQ(flows_on_tunnel[0].qos_flow_id, to_underlying(qfi));

  // No DRB level tunnel is requested. Per DRB forwarding tunnels arrive with the support for the direct
  // forwarding path.
  ASSERT_EQ(e1ap_pdu_session.drb_to_setup_list_ng_ran.size(), 1U);
  ASSERT_FALSE(e1ap_pdu_session.drb_to_setup_list_ng_ran[0].drb_data_forwarding_info_request_present);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// When the 5GC reports a direct forwarding path, the forwarded data reaches this target's own CU-UP, so the target
// asks for a DRB level tunnel that preserves the PDCP sequence numbers (TS 38.413 section 9.3.1.64,
// TS 38.300 section 9.2.3.2.3).
// The target tells the CU-UP that the source forwards over a direct path, so that the CU-UP takes the endpoints of
// the data forwarding tunnels from the address space of that path (TS 37.483 section 8.3.1.2).
TEST_F(cu_cp_inter_cu_ng_handover_test, when_direct_forwarding_path_is_available_then_it_is_signalled_to_the_cu_up)
{
  handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding       = true;
  ho_params.direct_forwarding_path_available = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(ho_params));

  const auto& bearer_context_setup = e1ap_pdu.pdu.init_msg().value.bearer_context_setup_request();
  ASSERT_TRUE(bearer_context_setup->direct_forwarding_path_availability_present);
  ASSERT_EQ(bearer_context_setup->direct_forwarding_path_availability.value,
            asn1::e1ap::direct_forwarding_path_availability_opts::intra_sys_direct_path_available);
}

// Without a direct path the IE is left out, so that the CU-UP keeps the NG-U address space, where the UPF forwards.
TEST_F(cu_cp_inter_cu_ng_handover_test, when_no_direct_forwarding_path_is_available_then_nothing_is_signalled)
{
  handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(ho_params));

  const auto& bearer_context_setup = e1ap_pdu.pdu.init_msg().value.bearer_context_setup_request();
  ASSERT_FALSE(bearer_context_setup->direct_forwarding_path_availability_present);
}

TEST_F(cu_cp_inter_cu_ng_handover_test, when_direct_forwarding_path_is_available_then_target_requests_drb_fwd_tunnels)
{
  handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding       = true;
  ho_params.direct_forwarding_path_available = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(ho_params));

  const asn1::e1ap::pdu_session_res_to_setup_item_s& e1ap_pdu_session = get_pdu_session_res_to_setup_item(e1ap_pdu);

  ASSERT_EQ(e1ap_pdu_session.drb_to_setup_list_ng_ran.size(), 1U);
  ASSERT_TRUE(e1ap_pdu_session.drb_to_setup_list_ng_ran[0].drb_data_forwarding_info_request_present);
  ASSERT_EQ(e1ap_pdu_session.drb_to_setup_list_ng_ran[0].drb_data_forwarding_info_request.data_forwarding_request.value,
            asn1::e1ap::data_forwarding_request_opts::dl);

  // No PDU session level tunnel is needed, since no proposed flow is left uncovered.
  ASSERT_FALSE(e1ap_pdu_session.pdu_session_data_forwarding_info_request_present);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// The 5GC can rule out data forwarding for a PDU session, in which case the target must not ask its CU-UP for any
// forwarding tunnel (TS 38.413 section 9.3.1.63).
TEST_F(cu_cp_inter_cu_ng_handover_test, when_data_forwarding_is_not_possible_then_no_fwd_tunnels_are_requested)
{
  // Inject a Handover Request proposing DL data forwarding but ruling it out for the PDU session.
  handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding   = true;
  ho_params.data_forwarding_not_possible = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(ho_params));

  const asn1::e1ap::pdu_session_res_to_setup_item_s& e1ap_pdu_session = get_pdu_session_res_to_setup_item(e1ap_pdu);
  ASSERT_FALSE(e1ap_pdu_session.pdu_session_data_forwarding_info_request_present);
  ASSERT_EQ(e1ap_pdu_session.drb_to_setup_list_ng_ran.size(), 1U);
  ASSERT_FALSE(e1ap_pdu_session.drb_to_setup_list_ng_ran[0].drb_data_forwarding_info_request_present);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// The forwarding tunnel endpoint the CU-UP allocated must reach the source node, so that it can send the data it still
// holds for the UE to it. The QoS flows carried on that tunnel are marked as accepted for data forwarding
// (TS 38.413 sections 9.3.4.11 and 9.3.2.13).
TEST_F(cu_cp_inter_cu_ng_handover_test, when_target_allocated_fwd_tunnels_then_they_are_advertised_to_the_amf)
{
  handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(ho_params));

  // Inject a Bearer Context Setup Response carrying the allocated forwarding endpoints.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request(true));
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  const auto& ho_request_ack = ngap_pdu.pdu.successful_outcome().value.ho_request_ack();
  ASSERT_EQ(ho_request_ack->pdu_session_res_admitted_list.size(), 1U);

  asn1::ngap::ho_request_ack_transfer_s ack_transfer;
  asn1::cbit_ref                        bref(ho_request_ack->pdu_session_res_admitted_list[0].ho_request_ack_transfer);
  ASSERT_EQ(ack_transfer.unpack(bref), asn1::OCUDUASN_SUCCESS);

  ASSERT_TRUE(ack_transfer.dl_forwarding_up_tnl_info_present);
  ASSERT_FALSE(ack_transfer.ie_exts.ul_forwarding_up_tnl_info_present);

  // No DRB item is reported. Per DRB forwarding tunnels arrive with the support for the direct forwarding path.
  ASSERT_EQ(ack_transfer.data_forwarding_resp_drb_list.size(), 0U);

  ASSERT_EQ(ack_transfer.qos_flow_setup_resp_list.size(), 1U);
  ASSERT_TRUE(ack_transfer.qos_flow_setup_resp_list[0].data_forwarding_accepted_present);

  // The DL forwarding endpoint differs from the DL NG-U endpoint of the PDU session.
  ASSERT_NE(ack_transfer.dl_forwarding_up_tnl_info.gtp_tunnel().gtp_teid.to_number(),
            ack_transfer.dl_ngu_up_tnl_info.gtp_tunnel().gtp_teid.to_number());

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// A PDU session the 5GC ruled out for data forwarding must be reported without any forwarding IE, rather than with an
// empty Data Forwarding Response DRB Item that would tell the source forwarding was accepted (TS 38.413 section
// 9.3.4.11).
TEST_F(cu_cp_inter_cu_ng_handover_test, when_data_forwarding_is_not_possible_then_no_fwd_tunnels_are_advertised)
{
  handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding   = true;
  ho_params.data_forwarding_not_possible = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(ho_params));

  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  const auto&                           ho_request_ack = ngap_pdu.pdu.successful_outcome().value.ho_request_ack();
  asn1::ngap::ho_request_ack_transfer_s ack_transfer;
  asn1::cbit_ref                        bref(ho_request_ack->pdu_session_res_admitted_list[0].ho_request_ack_transfer);
  ASSERT_EQ(ack_transfer.unpack(bref), asn1::OCUDUASN_SUCCESS);

  ASSERT_EQ(ack_transfer.data_forwarding_resp_drb_list.size(), 0U);
  ASSERT_FALSE(ack_transfer.dl_forwarding_up_tnl_info_present);
  ASSERT_EQ(ack_transfer.qos_flow_setup_resp_list.size(), 1U);
  ASSERT_FALSE(ack_transfer.qos_flow_setup_resp_list[0].data_forwarding_accepted_present);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// Per TS 38.413 Section 8.4.6 (Uplink RAN Status Transfer), the source includes its own DRB ID in the DRBs Subject to
// Status Transfer List IE, which the AMF then relays unchanged to the target via the Downlink RAN Status Transfer
// procedure (Section 8.4.7). A fresh target always starts allocating from DRB1, but the source may report a higher DRB
// ID for its (only) active DRB, e.g. because an earlier DRB was released on the source and its ID was never reused. The
// target must ignore such DRBs.
TEST_F(cu_cp_inter_cu_ng_handover_test,
       when_dl_ran_status_transfer_contains_drb_not_admitted_at_target_then_it_is_ignored)
{
  // Inject Handover Request and await Bearer Context Setup Request.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request());

  // Inject Bearer Context Setup Response and await UE Context Setup Request.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject NGAP DL RAN Status Transfer reporting only DRB2, which the source used for its (sole) active DRB
  // but was never admitted at this target (the target only ever allocated DRB1 for this single-flow handover).
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request({drb_id_t::drb2}));

  // DRB2 must be skipped. Since it's the only DRB the source reported, the resulting Bearer
  // Context Modification Request legitimately has nothing left to modify.
  const auto& bearer_ctxt_mod_req = e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request();
  ASSERT_TRUE(bearer_ctxt_mod_req->sys_bearer_context_mod_request_present);
  const auto& pdu_sessions = bearer_ctxt_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request()
                                 .pdu_session_res_to_modify_list;
  ASSERT_EQ(pdu_sessions.size(), 0U);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// DRB IDs are allocated independently by each RAN node (TS 38.300 Section 9.2.3.2.3). A numeric ID match is
// therefore only trustworthy when the source's DRB-to-QoS-flow mapping was actually signalled and confirmed during
// admission (TS 38.413 Section 9.3.1.29); an unconfirmed match, even for a DRB that legitimately exists at the
// target, must not be trusted with the source's PDCP state.
TEST_F(cu_cp_inter_cu_ng_handover_test, when_dl_ran_status_transfer_reports_an_unconfirmed_drb_id_then_it_is_ignored)
{
  // Inject Handover Request (without the source's DRB-to-QoS-flow mapping) and await Bearer Context Setup Request.
  handover_request_params ho_params;
  ho_params.include_drb_to_qos_flow_mapping = false;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(ho_params));

  // Inject Bearer Context Setup Response and await UE Context Setup Request.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject NGAP DL RAN Status Transfer reporting DRB1, which was admitted at this target (matching by pure
  // numeric coincidence), but the source never confirmed that mapping during admission.
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request({drb_id_t::drb1}));

  // DRB1 must still be skipped: existing here is not enough, it must be confirmed. Nothing is left to modify.
  const auto& bearer_ctxt_mod_req = e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request();
  ASSERT_TRUE(bearer_ctxt_mod_req->sys_bearer_context_mod_request_present);
  const auto& pdu_sessions = bearer_ctxt_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request()
                                 .pdu_session_res_to_modify_list;
  ASSERT_EQ(pdu_sessions.size(), 0U);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

TEST_F(cu_cp_inter_cu_ng_handover_test,
       when_rrc_reconfiguration_complete_arrives_before_dl_ran_status_transfer_then_handover_notify_is_sent)
{
  // Bring handover up to the point where either event can arrive first.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request());
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject RRC Reconfiguration Complete first.
  ASSERT_TRUE(send_rrc_reconfiguration_complete());

  // Inject DL RAN Status Transfer and continue normal execution.
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(await_handover_notify_and_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

///////////////////////////////////////////////////////////////////////////////
//                             ZigZag Handover
///////////////////////////////////////////////////////////////////////////////
TEST_F(cu_cp_inter_cu_ng_handover_test, when_zigzag_handover_is_performed_then_handovers_are_successful)
{
  // This CU-CP is the target...

  // Inject Handover Request and await Bearer Context Setup Request
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request());

  // Inject Bearer Context Setup Response and await UE Context Setup Request
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject NGAP DL RAN Status Transfer and Bearer Context Modification Response.
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and ACK the PDCP state modification.
  ASSERT_TRUE(send_bearer_context_modification_response());

  // Inject RRC Reconfiguration Complete and await Handover Notify and UE Context Modification Request.
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());

  // Inject UE Context Modification Response to ACK the RRC reconfiguration complete indicator.
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));

  // ... and now this CU-CP is the source.

  // Inject RRC Measurement Report and await Handover Required.
  ASSERT_TRUE(send_rrc_measurement_report_and_await_handover_required(
      cu_ue_id, du_ue_id, make_byte_buffer("000100410004025d341920802baa834c215630c9").value()));

  // Check that metrics contain the requested handover preparation.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 0);

  // Send Handover Command and await UE Context Modification Request (with RRC Reconfiguration).
  ASSERT_TRUE(send_handover_command_and_await_ue_context_modification_request(amf_ue_id_2, ran_ue_id_2));

  // Send Handover UE Context Modification Response and await for Bearer Context Modification Request (to query PDCP
  // state).
  ASSERT_TRUE(send_ue_context_modification_response_and_await_bearer_context_modification_request(cu_ue_id, du_ue_id));

  ASSERT_TRUE(send_bearer_context_modification_response_and_await_ul_status_transfer(cu_cp_e1ap_id, cu_up_e1ap_id));

  // Check that metrics contain the requested and successful handover preparation.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 1);

  // Inject NGAP UE Context Release Command and await Bearer Context Release Command.
  ASSERT_TRUE(send_ngap_ue_context_release_command_and_await_bearer_context_release_command(amf_ue_id_2));

  // Inject Bearer Context Release Complete and await F1AP UE Context Release Command.
  ASSERT_TRUE(
      send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command(cu_cp_e1ap_id, cu_up_e1ap_id));

  // Inject F1AP UE Context Release Complete and await NGAP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete_and_await_ngap_ue_context_release_complete(cu_ue_id, du_ue_id));

  // STATUS: UE should be removed at this stage
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0) << "UE should be removed";
}

TEST_F(
    cu_cp_inter_cu_ng_handover_test,
    when_inter_cu_ng_handover_target_receives_rrc_reconfiguration_complete_then_handover_notify_and_rrc_indicator_are_sent)
{
  // Inject Handover Request and await Bearer Context Setup Request
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request());

  // Inject Bearer Context Setup Response and await UE Context Setup Request
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject NGAP DL RAN Status Transfer and await Bearer Context Modification Request
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response
  ASSERT_TRUE(send_bearer_context_modification_response());

  // Inject RRC Reconfiguration Complete and verify Handover Notify and UE Context Modification Request (with RRC
  // reconfiguration complete indicator) are sent.
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());

  // Verify the F1AP UE Context Modification Request contains the RRC reconfiguration complete indicator.
  report_fatal_error_if_not(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu),
                            "Invalid F1AP UE Context Modification Request");

  // Inject UE Context Modification Response to ACK the RRC reconfiguration complete indicator.
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));

  // Verify no unexpected messages remain.
  report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                            "Unexpected NGAP message after handover target success");
  report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                            "Unexpected F1AP message after handover target success");
  report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                            "Unexpected E1AP message after handover target success");
}

/// Two AMFs serve the PLMN of the CU-CP.
class cu_cp_inter_cu_ng_handover_two_amfs_test : public cu_cp_inter_cu_ng_handover_test
{
public:
  cu_cp_inter_cu_ng_handover_two_amfs_test() :
    cu_cp_inter_cu_ng_handover_test(
        cu_cp_test_env_params{8, 8, 8192, 8, {{default_supported_tracking_area}, {default_supported_tracking_area}}})
  {
  }
};

TEST_F(cu_cp_inter_cu_ng_handover_two_amfs_test, when_second_amf_sends_handover_request_then_ue_is_served_by_it)
{
  // The second AMF of the PLMN sends the Handover Request.
  target_amf_idx = 1;

  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request());
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  ASSERT_TRUE(send_dl_ran_status_transfer_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response());

  // The Handover Notify goes to the AMF that sent the Handover Request.
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_handover_notify_and_ue_context_modification_request());
}
