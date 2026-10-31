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
#include "tests/test_doubles/xnap/xnap_test_message_validators.h"
#include "tests/unittests/ngap/ngap_test_messages.h"
#include "tests/unittests/xnap/xnap_test_messages.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents_ue.h"
#include "ocudu/asn1/xnap/xnap_pdu_contents.h"
#include "ocudu/e1ap/common/e1ap_types.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/f1ap/f1ap_ue_id_types.h"
#include "ocudu/ngap/ngap_message.h"
#include "ocudu/ngap/ngap_types.h"
#include "ocudu/ran/cu_types.h"
#include "ocudu/ran/up_transport_layer_info.h"
#include "ocudu/xnap/xnap_types.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;

class cu_cp_inter_cu_xn_handover_test : public cu_cp_test_environment, public ::testing::Test
{
public:
  cu_cp_inter_cu_xn_handover_test() :
    cu_cp_test_environment({/* max nof cu-ups */ 8,
                            /* max nof dus */ 8,
                            /* max nof ues */ 8192,
                            /* max nof drbs per ue */ 8,
                            /* amf config */ {{default_supported_tracking_area}},
                            /* trigger ho from measurements */ true,
                            /* enable rrc inactive */ false,
                            /* enable xnc peer */ true})
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MOB-3", "MVP-FUNC-MOB-1-e");

    // Run NG setup to completion.
    run_ng_setup();

    // Run XN setup to completion.
    run_xn_setup();

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
  send_handover_request_and_await_bearer_context_setup_request(local_xnap_ue_id_t                local_xnap_ue_id,
                                                               const xn_handover_request_params& ho_params = {})
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");
    report_fatal_error_if_not(not this->get_xnc_cu_cp(xnc_peer_idx).try_pop_rx_pdu(xnap_pdu),
                              "there are still XNAP messages to pop from Xn-C peer CU-CP");

    // Inject Handover Request and wait for Bearer Context Setup Request.
    get_xnc_cu_cp(xnc_peer_idx).push_tx_pdu(generate_handover_request(local_xnap_ue_id, ho_params));
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
    report_fatal_error_if_not(this->wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu),
                              "Failed to receive Handover Request Ack");
    report_fatal_error_if_not(test_helpers::is_valid_handover_request_ack(xnap_pdu), "Invalid Handover Request Ack");
    return true;
  }

  [[nodiscard]] bool send_bearer_context_modification_response()
  {
    // Inject Bearer Context Modification Response.
    get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_response(cu_cp_e1ap_id, cu_up_e1ap_id));
    return true;
  }

  [[nodiscard]] bool
  send_sn_status_transfer_and_await_bearer_context_modification_request(local_xnap_ue_id_t           local_xnap_ue_id,
                                                                        peer_xnap_ue_id_t            peer_xnap_ue_id,
                                                                        const std::vector<drb_id_t>& extra_drb_ids = {})
  {
    get_xnc_cu_cp(xnc_peer_idx)
        .push_tx_pdu(generate_sn_status_transfer(local_xnap_ue_id, peer_xnap_ue_id, extra_drb_ids));
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive Bearer Context Modification Request");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool send_rrc_reconfiguration_complete_and_await_path_switch_request()
  {
    // Inject UL RRC Message (containing RRC Reconfiguration Complete) and wait for Path Switch Request.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
        du_ue_id, cu_ue_id, srb_id_t::srb1, make_byte_buffer("800008006cfadbf2").value()));
    return await_path_switch_request();
  }

  [[nodiscard]] bool send_rrc_reconfiguration_complete()
  {
    // Inject UL RRC Message containing RRC Reconfiguration Complete.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
        du_ue_id, cu_ue_id, srb_id_t::srb1, make_byte_buffer("800008006cfadbf2").value()));
    return true;
  }

  [[nodiscard]] bool await_path_switch_request()
  {
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu), "Failed to receive Path Switch Request");
    report_fatal_error_if_not(test_helpers::is_valid_path_switch_request(ngap_pdu), "Invalid Path Switch Request");

    return true;
  }

  [[nodiscard]] bool send_path_switch_request_ack_and_await_ue_context_modification_request()
  {
    // Inject Path Switch Request Ack and await UE Context Modification Request.
    get_amf().push_tx_pdu(generate_path_switch_request_ack(uint_to_amf_ue_id(1), ran_ue_id_t::min));
    report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                              "Failed to receive F1AP UE Context Modification Request (containing RRC Release)");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu),
                              "Invalid UE Context Modification Request");
    return true;
  }

  [[nodiscard]] bool send_path_switch_request_ack_with_ul_tunnel_and_await_bearer_context_modification_request()
  {
    // Inject Path Switch Request Ack carrying a new UL tunnel endpoint and await Bearer Context Modification Request.
    get_amf().push_tx_pdu(
        generate_path_switch_request_ack_with_ul_tunnel(uint_to_amf_ue_id(1),
                                                        ran_ue_id_t::min,
                                                        psi,
                                                        transport_layer_address::create_from_string("127.0.1.1"),
                                                        int_to_gtpu_teid(0x00000001)));
    report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu),
                              "Failed to receive E1AP Bearer Context Modification Request (tunnel update)");
    report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                              "Invalid Bearer Context Modification Request");
    return true;
  }

  [[nodiscard]] bool await_ngap_ue_context_release_request()
  {
    report_fatal_error_if_not(this->wait_for_ngap_tx_pdu(ngap_pdu),
                              "Failed to receive NGAP UE Context Release Request");
    report_fatal_error_if_not(test_helpers::is_valid_ue_context_release_request(ngap_pdu),
                              "Invalid NGAP UE Context Release Request");
    return true;
  }

  [[nodiscard]] bool send_rrc_measurement_report_and_await_handover_request(
      gnb_cu_ue_f1ap_id_t f1ap_cu_ue_id,
      gnb_du_ue_f1ap_id_t f1ap_du_ue_id,
      local_xnap_ue_id_t& local_xnap_ue_id,
      peer_xnap_ue_id_t&  peer_xnap_ue_id,
      byte_buffer         rrc_meas_report = make_byte_buffer("000800420004015f741fe0808bf183fc0789117e").value())
  {
    // Inject UL RRC Message (containing RRC Measurement Report) and wait for Handover Required.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
        f1ap_du_ue_id, f1ap_cu_ue_id, srb_id_t::srb1, std::move(rrc_meas_report)));
    report_fatal_error_if_not(this->wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu),
                              "Failed to receive XN Handover Request");
    // report_fatal_error_if_not(test_helpers::is_valid_handover_request(xnap_pdu), "Invalid XN Handover Request");
    report_fatal_error_if_not(
        test_helpers::is_valid_rrc_handover_preparation_info(test_helpers::get_rrc_container(xnap_pdu)),
        "Invalid Handover Preparation Info");

    // The peer has to learn which AMF serves the UE, so that it can address the same one over NG-C
    // (TS 38.423 section 9.2.1.13).
    const auto& asn1_ue_context_info = xnap_pdu.pdu.init_msg().value.ho_request()->ue_context_info_ho_request;
    report_fatal_error_if_not(tla_from_asn1_bitstring(asn1_ue_context_info.cp_tnl_info_source.endpoint_ip_address()) ==
                                  amf_addr,
                              "The Handover Request does not carry the address of the AMF association");

    local_xnap_ue_id =
        uint_to_local_xnap_ue_id(xnap_pdu.pdu.init_msg().value.ho_request()->source_ng_ran_node_ue_xnap_id);
    peer_xnap_ue_id =
        uint_to_peer_xnap_ue_id(xnap_pdu.pdu.init_msg().value.ho_request()->source_ng_ran_node_ue_xnap_id);

    return true;
  }

  [[nodiscard]] bool send_handover_preparation_failure(peer_xnap_ue_id_t peer_xnap_ue_id)
  {
    // Inject Handover Preparation Failure.
    get_xnc_cu_cp(xnc_peer_idx).push_tx_pdu(generate_handover_preparation_failure(peer_xnap_ue_id));
    return true;
  }

  [[nodiscard]] bool timeout_handover_command_and_await_handover_cancel()
  {
    // Fail Handover Preparation (Xn-C peer CU-CP doesn't respond) and await Handover Cancel.
    if (tick_until(std::chrono::milliseconds(1000), [&]() { return false; })) {
      return false;
    }
    report_fatal_error_if_not(this->wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu), "Failed to receive Handover Cancel");
    report_fatal_error_if_not(test_helpers::is_valid_handover_cancel(xnap_pdu), "Invalid Handover Cancel");
    return true;
  }

  [[nodiscard]] bool
  send_handover_request_ack_and_await_ue_context_modification_request(local_xnap_ue_id_t local_xnap_ue_id,
                                                                      peer_xnap_ue_id_t  peer_xnap_ue_id)
  {
    // Inject Handover Request Ack and wait for UE Context Modification Request (containing RRC Reconfiguration).
    get_xnc_cu_cp(xnc_peer_idx).push_tx_pdu(generate_handover_request_ack(local_xnap_ue_id, peer_xnap_ue_id));

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
  send_bearer_context_modification_response_and_await_sn_status_transfer(gnb_cu_cp_ue_e1ap_id_t cu_cp_ue_e1ap_id,
                                                                         gnb_cu_up_ue_e1ap_id_t cu_up_ue_e1ap_id)
  {
    get_cu_up(cu_up_idx).push_tx_pdu(
        generate_bearer_context_modification_response_with_pdcp_status(cu_cp_ue_e1ap_id, cu_up_ue_e1ap_id));

    report_fatal_error_if_not(this->wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu),
                              "Failed to transmist XNAP SN Status transfer to Xn-C peer CU-CP");
    report_fatal_error_if_not(test_helpers::is_valid_sn_status_transfer(xnap_pdu), "Invalid XNAP SN Status Transfer");

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

  [[nodiscard]] bool
  send_xnap_ue_context_release_and_await_bearer_context_release_command(local_xnap_ue_id_t local_xnap_ue_id,
                                                                        peer_xnap_ue_id_t  peer_xnap_ue_id)
  {
    report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                              "there are still NGAP messages to pop from AMF");
    report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                              "there are still F1AP DL messages to pop from DU");
    report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                              "there are still E1AP messages to pop from CU-UP");
    report_fatal_error_if_not(not this->get_xnc_cu_cp(xnc_peer_idx).try_pop_rx_pdu(xnap_pdu),
                              "there are still XNAP messages to pop from Xn-C peer CU-CP");

    // Inject XNAP UE Context Release and wait for Bearer Context Release Command.
    get_xnc_cu_cp(xnc_peer_idx).push_tx_pdu(generate_ue_context_release(local_xnap_ue_id, peer_xnap_ue_id));
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

  [[nodiscard]] bool send_f1ap_ue_context_release_complete(gnb_cu_ue_f1ap_id_t f1ap_cu_ue_id,
                                                           gnb_du_ue_f1ap_id_t f1ap_du_ue_id)
  {
    // Inject F1AP UE Context Release Complete and wait for N1AP UE Context Release Command.
    get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_release_complete(f1ap_cu_ue_id, f1ap_du_ue_id));
    return true;
  }

  unsigned du_idx       = 0;
  unsigned cu_up_idx    = 0;
  unsigned xnc_peer_idx = 0;

  gnb_du_ue_f1ap_id_t du_ue_id = gnb_du_ue_f1ap_id_t::min;
  gnb_cu_ue_f1ap_id_t cu_ue_id;
  rnti_t              crnti     = to_rnti(0x4601);
  amf_ue_id_t         amf_ue_id = uint_to_amf_ue_id(
      test_rng::uniform_int<uint64_t>(to_underlying(amf_ue_id_t::min), to_underlying(amf_ue_id_t::max)));
  gnb_cu_up_ue_e1ap_id_t cu_up_e1ap_id = gnb_cu_up_ue_e1ap_id_t::min;
  gnb_cu_cp_ue_e1ap_id_t cu_cp_e1ap_id;

  local_xnap_ue_id_t source_local_xnap_ue_id = local_xnap_ue_id_t::min;
  peer_xnap_ue_id_t  source_peer_xnap_ue_id  = peer_xnap_ue_id_t::min;

  local_xnap_ue_id_t target_local_xnap_ue_id = local_xnap_ue_id_t::min;
  peer_xnap_ue_id_t  target_peer_xnap_ue_id  = peer_xnap_ue_id_t::min;

  std::optional<uint8_t> ue_context_setup_req_serving_cell_mo = std::nullopt;

  const ue_context* ue_ctx = nullptr;

  pdu_session_id_t psi = uint_to_pdu_session_id(1);
  qos_flow_id_t    qfi = uint_to_qos_flow_id(1);

  ngap_message ngap_pdu;
  xnap_message xnap_pdu;
  f1ap_message f1ap_pdu;
  e1ap_message e1ap_pdu;
};

///////////////////////////////////////////////////////////////////////////////
//                             Source CU-CP
///////////////////////////////////////////////////////////////////////////////
TEST_F(cu_cp_inter_cu_xn_handover_test, when_handover_preparation_failure_is_received_then_handover_fails)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject RRC Measurement Report and await Handover Request.
  ASSERT_TRUE(send_rrc_measurement_report_and_await_handover_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value(), source_local_xnap_ue_id, target_peer_xnap_ue_id));

  // Inject Handover Preparation Failure.
  ASSERT_TRUE(send_handover_preparation_failure(target_peer_xnap_ue_id));

  // Check that the metrics report contains the requested handover preparation only.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1U);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 0U);

  // STATUS: Handover Preparation failed and no further messages are sent to the AMF.
  report_fatal_error_if_not(not this->get_amf().try_pop_rx_pdu(ngap_pdu),
                            "there are still NGAP messages to pop from AMF");
}

TEST_F(cu_cp_inter_cu_xn_handover_test, when_handover_request_is_sent_then_it_carries_the_guami_of_the_serving_amf)
{
  // Attach UE. The Initial Context Setup Request assigns the GUAMI with AMF Region ID 4, while the first GUAMI of the
  // NG Setup Response has AMF Region ID 2.
  ASSERT_TRUE(attach_ue());

  // Inject RRC Measurement Report and await Handover Request.
  ASSERT_TRUE(send_rrc_measurement_report_and_await_handover_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value(), source_local_xnap_ue_id, target_peer_xnap_ue_id));

  // The Handover Request carries the GUAMI that the AMF assigned to the UE.
  const auto& guami = xnap_pdu.pdu.init_msg().value.ho_request()->guami;
  ASSERT_EQ(guami.amf_region_id.to_number(), 4);
  ASSERT_EQ(guami.amf_set_id.to_number(), 1);
  ASSERT_EQ(guami.amf_pointer.to_number(), 0);
}

TEST_F(cu_cp_inter_cu_xn_handover_test, when_handover_command_times_out_then_handover_cancel_is_sent)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject RRC Measurement Report and await Handover Request.
  ASSERT_TRUE(send_rrc_measurement_report_and_await_handover_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value(), source_local_xnap_ue_id, target_peer_xnap_ue_id));

  // Timeout Handover Command and await Handover Cancel.
  ASSERT_TRUE(timeout_handover_command_and_await_handover_cancel());
}

TEST_F(cu_cp_inter_cu_xn_handover_test, when_handover_succeeds_then_amf_releases_ue)
{
  // Attach UE.
  ASSERT_TRUE(attach_ue());

  // Inject RRC Measurement Report and await Handover Request.
  ASSERT_TRUE(send_rrc_measurement_report_and_await_handover_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value(), source_local_xnap_ue_id, target_peer_xnap_ue_id));

  // Check that the metrics report contains the requested handover preparation.
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1U);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 0U);

  // Inject Handover Request Ack and await UE Context Modification Request (with RRC Reconfiguration).
  ASSERT_TRUE(send_handover_request_ack_and_await_ue_context_modification_request(target_local_xnap_ue_id,
                                                                                  target_peer_xnap_ue_id));

  // Check that the metrics report contains the successful handover preparation.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_handover_preparations_requested, 1U);
  ASSERT_EQ(report.mobility.nof_successful_handover_preparations, 1U);

  // Inject Handover UE Context Modification Response and await for Bearer Context Modification Request (to query PDCP
  // state).
  ASSERT_TRUE(send_ue_context_modification_response_and_await_bearer_context_modification_request(
      ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // Check that the metrics report contains the requested handover execution.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_inter_gnb_handover_executions_requested, 1U);
  ASSERT_EQ(report.mobility.nof_successful_inter_gnb_handover_executions, 0U);

  ASSERT_TRUE(send_bearer_context_modification_response_and_await_sn_status_transfer(ue_ctx->cu_cp_e1ap_id.value(),
                                                                                     ue_ctx->cu_up_e1ap_id.value()));

  // Inject XNAP UE Context Release and await Bearer Context Release Command.
  ASSERT_TRUE(send_xnap_ue_context_release_and_await_bearer_context_release_command(target_local_xnap_ue_id,
                                                                                    target_peer_xnap_ue_id));

  // Check that the metrics report contains the successful handover execution.
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.mobility.nof_successful_inter_gnb_handover_executions, 1U);

  // Inject Bearer Context Release Complete and await F1AP UE Context Release Command.
  ASSERT_TRUE(send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command(
      ue_ctx->cu_cp_e1ap_id.value(), ue_ctx->cu_up_e1ap_id.value()));

  // Inject F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete(ue_ctx->cu_ue_id.value(), ue_ctx->du_ue_id.value()));

  // STATUS: UE should be removed at this stage
  report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0) << "UE should be removed";
}

///////////////////////////////////////////////////////////////////////////////
//                             Target CU-CP
///////////////////////////////////////////////////////////////////////////////
TEST_F(cu_cp_inter_cu_xn_handover_test, when_handover_request_carries_guami_of_unknown_amf_then_handover_fails)
{
  // The CU-CP is connected to no AMF of the AMF Set 2.
  xn_handover_request_params ho_params;
  ho_params.guami.amf_set_id = 2;
  get_xnc_cu_cp(xnc_peer_idx).push_tx_pdu(generate_handover_request(source_local_xnap_ue_id, ho_params));

  // The CU-CP cannot send the Path Switch Request to the AMF of the UE, so it rejects the handover.
  ASSERT_TRUE(this->wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu));
  ASSERT_EQ(xnap_pdu.pdu.type().value, asn1::xnap::xnap_pdu_c::types_opts::unsuccessful_outcome);
  ASSERT_EQ(xnap_pdu.pdu.unsuccessful_outcome().value.type().value,
            asn1::xnap::xnap_elem_procs_o::unsuccessful_outcome_c::types_opts::ho_prep_fail);
  const auto& cause = xnap_pdu.pdu.unsuccessful_outcome().value.ho_prep_fail()->cause;
  ASSERT_EQ(cause.type().value, asn1::xnap::cause_c::types_opts::radio_network);
  ASSERT_EQ(cause.radio_network().value, asn1::xnap::cause_radio_network_layer_opts::unknown_guami_id);
  ASSERT_FALSE(this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu));
}

TEST_F(cu_cp_inter_cu_xn_handover_test, when_handover_request_received_then_path_switch_request_is_sent)
{
  // Inject Handover Request and await Bearer Context Setup Request.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id));

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

  // Inject XNAP SN RAN Status Transfer and Bearer Context Modification Response.
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));

  // Inject Bearer Context Modification Response and ACK the PDCP state modification.
  ASSERT_TRUE(send_bearer_context_modification_response());

  // Inject RRC Reconfiguration Complete and await Path Switch Request.
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());

  // Inject Path Switch Request Ack and await UE Context Modification Request.
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());

  // Inject UE Context Modification Response to ACK the RRC reconfiguration complete indicator.
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// Per TS 38.423 Section 8.2.2.1, the DRBs Subject to Status Transfer List IE reflects the source's DRB
// configuration and is not guaranteed to be a subset of the DRBs admitted at this target (e.g. if a DRB's
// QoS flows were not admitted). The target must ignore such DRBs.
TEST_F(cu_cp_inter_cu_xn_handover_test, when_sn_status_transfer_contains_drb_not_admitted_at_target_then_it_is_ignored)
{
  // Inject Handover Request and await Bearer Context Setup Request.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id));

  // Inject Bearer Context Setup Response and await UE Context Setup Request.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject XNAP SN Status Transfer that additionally reports DRB2, which was never admitted at this target
  // (the UE was only set up with DRB1).
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(
      source_local_xnap_ue_id, source_peer_xnap_ue_id, {drb_id_t::drb2}));

  // The Bearer Context Modification Request must only cover the admitted DRB1; DRB2 must be skipped.
  const auto& bearer_ctxt_mod_req = e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request();
  ASSERT_TRUE(bearer_ctxt_mod_req->sys_bearer_context_mod_request_present);
  const auto& pdu_sessions = bearer_ctxt_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request()
                                 .pdu_session_res_to_modify_list;
  ASSERT_EQ(pdu_sessions.size(), 1U);
  const auto& drbs_to_modify = pdu_sessions[0].drb_to_modify_list_ng_ran;
  ASSERT_EQ(drbs_to_modify.size(), 1U);
  EXPECT_EQ(drbs_to_modify[0].drb_id, 1);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// The source's DRB-to-QoS-flow mapping may instead be reported through AS-Config in the RRC
// HandoverPreparationInformation (TS 38.331 Section 11.2.3), which is how this node signals it. The target must
// derive the source's DRB numbering from there too, so a matching DRB ID is confirmed and can be trusted with the
// source's PDCP state.
TEST_F(cu_cp_inter_cu_xn_handover_test, when_drb_to_qos_flow_mapping_is_signalled_via_as_config_then_it_is_confirmed)
{
  // Inject Handover Request reporting the mapping only via AS-Config, not via the XnAP-native IE.
  xn_handover_request_params ho_params;
  ho_params.include_drb_to_qos_flow_mapping = false;
  ho_params.include_as_config_drb_mapping   = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id, ho_params));

  // Inject Bearer Context Setup Response and await UE Context Setup Request.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject XNAP SN Status Transfer reporting DRB1.
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));

  // DRB1's numbering was confirmed from AS-Config, so the source's PDCP state must be applied to it.
  const auto& bearer_ctxt_mod_req = e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request();
  ASSERT_TRUE(bearer_ctxt_mod_req->sys_bearer_context_mod_request_present);
  const auto& pdu_sessions = bearer_ctxt_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request()
                                 .pdu_session_res_to_modify_list;
  ASSERT_EQ(pdu_sessions.size(), 1U);
  const auto& drbs_to_modify = pdu_sessions[0].drb_to_modify_list_ng_ran;
  ASSERT_EQ(drbs_to_modify.size(), 1U);
  EXPECT_EQ(drbs_to_modify[0].drb_id, 1);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// DRB IDs are allocated independently by each RAN node (TS 38.300 Section 9.2.3.2.3). A numeric ID match is
// therefore only trustworthy when the source's DRB-to-QoS-flow mapping was actually signalled and confirmed during
// admission (TS 38.423 Section 9.2.1.17); an unconfirmed match, even for a DRB that legitimately exists at the
// target, must not be trusted with the source's PDCP state.
TEST_F(cu_cp_inter_cu_xn_handover_test, when_sn_status_transfer_reports_an_unconfirmed_drb_id_then_it_is_ignored)
{
  // Inject Handover Request (without the source's DRB-to-QoS-flow mapping) and await Bearer Context Setup Request.
  xn_handover_request_params ho_params;
  ho_params.include_drb_to_qos_flow_mapping = false;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id, ho_params));

  // Inject Bearer Context Setup Response and await UE Context Setup Request.
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request.
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject XNAP SN Status Transfer reporting DRB1, which was admitted at this target (matching by pure numeric
  // coincidence), but the source never confirmed that mapping during admission.
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));

  // DRB1 must still be skipped: existing here is not enough, it must be confirmed. Nothing is left to modify.
  const auto& bearer_ctxt_mod_req = e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request();
  ASSERT_TRUE(bearer_ctxt_mod_req->sys_bearer_context_mod_request_present);
  const auto& pdu_sessions = bearer_ctxt_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request()
                                 .pdu_session_res_to_modify_list;
  ASSERT_EQ(pdu_sessions.size(), 0U);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

TEST_F(cu_cp_inter_cu_xn_handover_test,
       when_rrc_reconfiguration_complete_arrives_before_sn_status_transfer_then_path_switch_request_is_sent)
{
  // Bring handover up to the point where either event can arrive first.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject RRC Reconfiguration Complete first.
  ASSERT_TRUE(send_rrc_reconfiguration_complete());

  // Inject SN Status Transfer and continue normal execution.
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(await_path_switch_request());
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

TEST_F(cu_cp_inter_cu_xn_handover_test,
       when_path_switch_ack_contains_new_ul_tunnel_then_bearer_context_modification_is_sent)
{
  // Bring handover up to the point where the Path Switch Request has been sent.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());

  // Inject Path Switch Request Ack with a new UL tunnel and await E1AP Bearer Context Modification carrying it.
  ASSERT_TRUE(send_path_switch_request_ack_with_ul_tunnel_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response; expect UE Context Modification Request (RRC release indicator).
  ASSERT_TRUE(send_bearer_context_modification_response());
  report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu),
                            "Failed to receive F1AP UE Context Modification Request after tunnel update");
  report_fatal_error_if_not(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu),
                            "Invalid UE Context Modification Request");
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

TEST_F(cu_cp_inter_cu_xn_handover_test, when_path_switch_request_is_rejected_by_amf_then_ue_is_released)
{
  // Bring handover up to the point where the Path Switch Request has been sent.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());

  // Inject Path Switch Request Failure; the routine should trigger UE release.
  get_amf().push_tx_pdu(generate_path_switch_request_failure(
      uint_to_amf_ue_id(1), ran_ue_id_t::min, ngap_cause_t{ngap_cause_radio_network_t::ho_target_not_allowed}));

  // Verify UE release is requested to AMF.
  ASSERT_TRUE(await_ngap_ue_context_release_request());

  // Verify that no RRC reconfiguration complete indicator is sent on path switch failure.
  // Instead, UE release should be triggered.
  report_fatal_error_if_not(
      not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
      "Unexpected F1AP message after path switch failure (RRC reconf complete should not be sent)");
  report_fatal_error_if_not(not this->get_xnc_cu_cp(xnc_peer_idx).try_pop_rx_pdu(xnap_pdu),
                            "Unexpected XNAP message after path switch failure");
  report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                            "Unexpected E1AP message after path switch failure");
}

TEST_F(cu_cp_inter_cu_xn_handover_test, when_path_switch_succeeds_then_rrc_reconfiguration_complete_indicator_is_sent)
{
  // Bring handover up to the point where the Path Switch Request has been sent.
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());

  // Inject Path Switch Request Ack; routine should send RRC reconfiguration complete indicator to DU.
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());

  // Verify the F1AP UE Context Modification Request contains the RRC reconfiguration complete indicator.
  report_fatal_error_if_not(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu),
                            "Invalid F1AP UE Context Modification Request");

  // Inject UE Context Modification Response to ACK the RRC reconfiguration complete indicator.
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));

  // Verify XNAP UE Context Release was sent to source CU-CP.
  report_fatal_error_if_not(this->wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu),
                            "Failed to receive XNAP UE Context Release after path switch success");
  report_fatal_error_if_not(test_helpers::is_valid_ue_context_release(xnap_pdu),
                            "Invalid XNAP UE Context Release after path switch success");

  // Verify no unexpected messages remain.
  report_fatal_error_if_not(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu),
                            "Unexpected F1AP message after path switch success");
  report_fatal_error_if_not(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
                            "Unexpected E1AP message after path switch success");
}

///////////////////////////////////////////////////////////////////////////////
//                             ZigZag Handover
///////////////////////////////////////////////////////////////////////////////
TEST_F(cu_cp_inter_cu_xn_handover_test, when_zigzag_handover_is_performed_then_handovers_are_successful)
{
  // This CU-CP is the target...

  // Inject Handover Request and await Bearer Context Setup Request
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id));

  // Inject Bearer Context Setup Response and await UE Context Setup Request
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());

  // Inject UE Context Setup Response and await Bearer Context Modification Request
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());

  // Inject Bearer Context Modification Response and await Handover Request Ack
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  // Inject XNAP SN Status Transfer and Bearer Context Modification Response.
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));

  // Inject Bearer Context Modification Response and ACK the PDCP state modification.
  ASSERT_TRUE(send_bearer_context_modification_response());

  // Inject RRC Reconfiguration Complete and await Path Switch Request.
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());

  // Inject Path Switch Request Ack and await UE Context Modification Request.
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());

  // Inject UE Context Modification Response to ACK the RRC reconfiguration complete indicator.
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));

  // ... and now this CU-CP is the source.

  // Pop messages from Xn-C peer CU-CP before injecting the RRC Measurement Report.
  this->get_xnc_cu_cp(xnc_peer_idx).try_pop_rx_pdu(xnap_pdu);

  // Inject RRC Measurement Report and await Handover Request.
  ASSERT_TRUE(send_rrc_measurement_report_and_await_handover_request(
      cu_ue_id,
      du_ue_id,
      source_local_xnap_ue_id,
      target_peer_xnap_ue_id,
      make_byte_buffer("000100420004015f741fe0808bf183fcf868e7c5").value()));

  // Inject Handover Request Ack and await UE Context Modification Request (with RRC Reconfiguration).
  ASSERT_TRUE(send_handover_request_ack_and_await_ue_context_modification_request(target_local_xnap_ue_id,
                                                                                  target_peer_xnap_ue_id));

  // Inject Handover UE Context Modification Response and await for Bearer Context Modification Request (to query PDCP
  // state).
  ASSERT_TRUE(send_ue_context_modification_response_and_await_bearer_context_modification_request(cu_ue_id, du_ue_id));

  // Inject Bearer Context Modification Response and await SN Status Transfer.
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_sn_status_transfer(cu_cp_e1ap_id, cu_up_e1ap_id));

  // Inject XNAP UE Context Release and await Bearer Context Release Command.
  ASSERT_TRUE(send_xnap_ue_context_release_and_await_bearer_context_release_command(target_local_xnap_ue_id,
                                                                                    target_peer_xnap_ue_id));

  // Inject Bearer Context Release Complete and await F1AP UE Context Release Command.
  ASSERT_TRUE(
      send_bearer_context_release_complete_and_await_f1ap_ue_context_release_command(cu_cp_e1ap_id, cu_up_e1ap_id));

  // Inject F1AP UE Context Release Complete.
  ASSERT_TRUE(send_f1ap_ue_context_release_complete(cu_ue_id, du_ue_id));

  // STATUS: UE should be removed at this stage
  auto report = this->get_cu_cp().get_metrics_handler().request_metrics_report();
  ASSERT_EQ(report.ues.size(), 0) << "UE should be removed";
}

TEST_F(cu_cp_inter_cu_xn_handover_test, when_ue_arrived_by_xn_handover_then_next_handover_request_carries_its_guami)
{
  // This CU-CP is the target. The Handover Request carries a GUAMI with AMF Pointer 1 of the AMF Set that the AMF
  // serves, while the first GUAMI of the NG Setup Response has AMF Pointer 0.
  xn_handover_request_params ho_params;
  ho_params.guami.amf_pointer = 1;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id, ho_params));
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request());
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));

  // Now this CU-CP is the source.
  this->get_xnc_cu_cp(xnc_peer_idx).try_pop_rx_pdu(xnap_pdu);
  ASSERT_TRUE(send_rrc_measurement_report_and_await_handover_request(
      cu_ue_id,
      du_ue_id,
      source_local_xnap_ue_id,
      target_peer_xnap_ue_id,
      make_byte_buffer("000100420004015f741fe0808bf183fcf868e7c5").value()));

  // The Handover Request carries the GUAMI that the previous source reported.
  const auto& guami = xnap_pdu.pdu.init_msg().value.ho_request()->guami;
  ASSERT_EQ(guami.amf_region_id.to_number(), 2);
  ASSERT_EQ(guami.amf_set_id.to_number(), 1);
  ASSERT_EQ(guami.amf_pointer.to_number(), 1);
}

// When the source proposes DL data forwarding for its QoS flows in the Data Forwarding and Offloading Info from
// source NG-RAN node IE (TS 38.423 section 9.2.1.17), the target asks its CU-UP for a forwarding tunnel carrying
// those flows (TS 37.483 section 9.3.2.5).
TEST_F(cu_cp_inter_cu_xn_handover_test, when_source_proposes_dl_data_forwarding_then_target_requests_fwd_tunnels)
{
  xn_handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id, ho_params));

  const auto& ng_ran_bearer_ctxt = e1ap_pdu.pdu.init_msg()
                                       .value.bearer_context_setup_request()
                                       ->sys_bearer_context_setup_request.ng_ran_bearer_context_setup_request();
  const asn1::e1ap::pdu_session_res_to_setup_item_s& e1ap_pdu_session =
      ng_ran_bearer_ctxt.pdu_session_res_to_setup_list[0];

  // The source forwards straight to this target over Xn, and it reused the source's DRB ID for every proposed QoS
  // flow of the DRB, so the DRB gets its own tunnel and keeps the PDCP sequence numbers.
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
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}

// The forwarding tunnel endpoints the CU-UP allocated must reach the source node, so the target reports them in the
// Data Forwarding Info from target NG-RAN node IE of the Handover Request Acknowledge, together with the QoS flows it
// accepts for forwarding (TS 38.423 section 9.2.1.19).
TEST_F(cu_cp_inter_cu_xn_handover_test, when_target_allocated_fwd_tunnels_then_they_are_advertised_to_the_source)
{
  xn_handover_request_params ho_params;
  ho_params.propose_dl_data_forwarding = true;
  ASSERT_TRUE(send_handover_request_and_await_bearer_context_setup_request(source_local_xnap_ue_id, ho_params));
  ASSERT_TRUE(send_bearer_context_setup_response_and_await_ue_context_setup_request(true));
  ASSERT_TRUE(send_ue_context_setup_response_and_await_bearer_context_modification_request());
  ASSERT_TRUE(send_bearer_context_modification_response_and_await_handover_request_ack());

  const auto& ho_request_ack = xnap_pdu.pdu.successful_outcome().value.ho_request_ack();
  ASSERT_EQ(ho_request_ack->pdu_session_res_admitted_list.size(), 1U);
  const auto& admitted_info = ho_request_ack->pdu_session_res_admitted_list[0].pdu_session_res_admitted_info;

  ASSERT_TRUE(admitted_info.data_forwarding_info_from_target_present);
  const auto& forwarding_info = admitted_info.data_forwarding_info_from_target;
  ASSERT_EQ(forwarding_info.qos_flows_accepted_for_data_forwarding_list.size(), 1U);
  EXPECT_EQ(forwarding_info.qos_flows_accepted_for_data_forwarding_list[0].qos_flow_id, 1U);

  // The DRB level tunnel the CU-UP allocated is reported per DRB. The DRB covers every proposed QoS flow, so the PDU
  // session level tunnel the CU-UP offered on top is left unadvertised.
  ASSERT_EQ(forwarding_info.data_forwarding_resp_drb_item_list.size(), 1U);
  EXPECT_EQ(forwarding_info.data_forwarding_resp_drb_item_list[0].drb_id, 1U);
  EXPECT_TRUE(forwarding_info.data_forwarding_resp_drb_item_list[0].dl_forwarding_up_tnl_present);
  ASSERT_FALSE(forwarding_info.pdu_session_level_dl_data_forwarding_info_present);

  // The handover must still complete normally afterwards.
  ASSERT_TRUE(send_sn_status_transfer_and_await_bearer_context_modification_request(source_local_xnap_ue_id,
                                                                                    source_peer_xnap_ue_id));
  ASSERT_TRUE(send_bearer_context_modification_response());
  ASSERT_TRUE(send_rrc_reconfiguration_complete_and_await_path_switch_request());
  ASSERT_TRUE(send_path_switch_request_ack_and_await_ue_context_modification_request());
  ASSERT_TRUE(send_ue_context_modification_response_empty(cu_ue_id, du_ue_id));
}
