// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_cu_test_helpers.h"
#include "lib/f1ap/asn1_helpers.h"
#include "tests/test_doubles/f1ap/f1ap_test_messages.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/cu_cp/f1ap_cu.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;
using namespace asn1::f1ap;

static du_setup_result create_du_setup_result_accept(const f1ap_message& f1_msg)
{
  du_setup_result resp;
  auto&           accepted    = resp.result.emplace<du_setup_result::accepted>();
  accepted.gnb_cu_name        = "dummy_gnb_cu_name";
  accepted.gnb_cu_rrc_version = 2;

  const auto& cells = f1_msg.pdu.init_msg().value.f1_setup_request()->gnb_du_served_cells_list;
  accepted.cells_to_be_activ_list.resize(cells.size());
  for (unsigned i = 0; i != cells.size(); ++i) {
    auto& cell  = accepted.cells_to_be_activ_list[i];
    cell.nr_cgi = cgi_from_asn1(cells[i]->gnb_du_served_cells_item().served_cell_info.nr_cgi).value();
    cell.nr_pci = cells[i]->gnb_du_served_cells_item().served_cell_info.nr_pci;
  }
  return resp;
}

static du_setup_result create_du_setup_result_reject(const f1ap_message& f1_msg)
{
  du_setup_result resp;
  auto&           rejected = resp.result.emplace<du_setup_result::rejected>();
  rejected.cause           = cause_misc_t::unspecified;
  rejected.cause_str       = "dummy reason";
  return resp;
}

//////////////////////////////////////////////////////////////////////////////////////
/* Handling of unsupported messages                                                 */
//////////////////////////////////////////////////////////////////////////////////////

TEST_F(f1ap_cu_test, when_unsupported_f1ap_pdu_received_then_message_ignored)
{
  // Set last message of PDU notifier to init_msg.
  f1ap_pdu_notifier.last_f1ap_msg.pdu.set_init_msg();

  // Generate unsupported F1AP PDU.
  f1ap_message unsupported_msg = {};
  unsupported_msg.pdu.set_choice_ext();

  f1ap->handle_message(unsupported_msg);

  // Check that PDU has not been forwarded (last PDU is still init_msg).
  EXPECT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.type(), asn1::f1ap::f1ap_pdu_c::types_opts::options::init_msg);
}

TEST_F(f1ap_cu_test, when_unsupported_init_msg_received_then_message_ignored)
{
  // Set last message of PDU notifier to successful outcome.
  f1ap_pdu_notifier.last_f1ap_msg.pdu.set_successful_outcome();

  // Generate unupported F1AP PDU.
  f1ap_message unsupported_msg = {};
  unsupported_msg.pdu.set_init_msg();

  f1ap->handle_message(unsupported_msg);

  // Check that PDU has not been forwarded (last PDU is still successful_outcome).
  EXPECT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.type(),
            asn1::f1ap::f1ap_pdu_c::types_opts::options::successful_outcome);
}

TEST_F(f1ap_cu_test, when_unsupported_successful_outcome_received_then_message_ignored)
{
  // Set last message of PDU notifier to init_msg.
  f1ap_pdu_notifier.last_f1ap_msg.pdu.set_init_msg();

  // Generate unupported F1AP PDU.
  f1ap_message unsupported_msg = {};
  unsupported_msg.pdu.set_successful_outcome();

  f1ap->handle_message(unsupported_msg);

  // Check that PDU has not been forwarded (last PDU is still init_msg).
  EXPECT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.type(), asn1::f1ap::f1ap_pdu_c::types_opts::options::init_msg);
}

TEST_F(f1ap_cu_test, when_unsupported_unsuccessful_outcome_received_then_message_ignored)
{
  // Set last message of PDU notifier to init_msg.
  f1ap_pdu_notifier.last_f1ap_msg.pdu.set_init_msg();

  // Generate unupported F1AP PDU.
  f1ap_message unsupported_msg = {};
  unsupported_msg.pdu.set_unsuccessful_outcome();

  f1ap->handle_message(unsupported_msg);

  // Check that PDU has not been forwarded (last PDU is still init_msg).
  EXPECT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.type(), asn1::f1ap::f1ap_pdu_c::types_opts::options::init_msg);
}

//////////////////////////////////////////////////////////////////////////////////////
/* F1 Setup handling                                                                */
//////////////////////////////////////////////////////////////////////////////////////

/// Test the successful f1 setup procedure.
TEST_F(f1ap_cu_test, when_f1_setup_request_valid_then_connect_du)
{
  // Create F1SetupRequest.
  f1ap_message f1setup_msg = test_helpers::generate_f1_setup_request();

  // Prepare the DU processor response.
  du_processor_notifier.next_du_setup_resp = create_du_setup_result_accept(f1setup_msg);

  test_logger.info("TEST: Receive F1SetupRequest message...");
  f1ap->handle_message(f1setup_msg);

  // Check if F1SetupRequest was forwarded to DU processor.
  ASSERT_EQ(du_processor_notifier.last_f1_setup_request_msg.gnb_du_id, int_to_gnb_du_id(0x11U));

  // Check the F1 Tx PDU is indeed the F1 Setup response.
  ASSERT_EQ(asn1::f1ap::f1ap_pdu_c::types_opts::options::successful_outcome,
            f1ap_pdu_notifier.last_f1ap_msg.pdu.type());
  ASSERT_EQ(asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::options::f1_setup_resp,
            f1ap_pdu_notifier.last_f1ap_msg.pdu.successful_outcome().value.type());
}

/// Test the f1 setup failure.
TEST_F(f1ap_cu_test, when_f1_setup_request_invalid_then_reject_du)
{
  // Generate Invalid F1SetupRequest.
  f1ap_message f1setup_msg = test_helpers::generate_f1_setup_request();
  auto&        setup_req   = f1setup_msg.pdu.init_msg().value.f1_setup_request();
  // Set CGI PLMN to invalid value.
  setup_req->gnb_du_served_cells_list[0].value().gnb_du_served_cells_item().served_cell_info.nr_cgi.plmn_id.from_string(
      "ffffff");

  // Prepare the DU processor response.
  du_processor_notifier.next_du_setup_resp = create_du_setup_result_reject(f1setup_msg);

  f1ap->handle_message(f1setup_msg);

  // Check the generated PDU is indeed the F1 Setup failure.
  ASSERT_EQ(asn1::f1ap::f1ap_pdu_c::types_opts::options::unsuccessful_outcome,
            f1ap_pdu_notifier.last_f1ap_msg.pdu.type());
  ASSERT_EQ(asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::f1_setup_fail,
            f1ap_pdu_notifier.last_f1ap_msg.pdu.unsuccessful_outcome().value.type());
}

//////////////////////////////////////////////////////////////////////////////////////
/* Initial UL RRC Message handling                                                  */
//////////////////////////////////////////////////////////////////////////////////////

TEST_F(f1ap_cu_test, when_init_ul_rrc_correct_then_ue_added)
{
  // Generate F1 Initial UL RRC Message.
  f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(int_to_gnb_du_ue_f1ap_id(41255));

  // Pass message to F1AP.
  f1ap->handle_message(init_ul_rrc_msg);

  EXPECT_EQ(f1ap->get_nof_ues(), 1);
}

TEST_F(f1ap_cu_test, when_cgi_invalid_then_ue_not_added)
{
  // Generate F1 Initial UL RRC Message.
  f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(int_to_gnb_du_ue_f1ap_id(41255));
  // Set PLMN to invalid value.
  init_ul_rrc_msg.pdu.init_msg().value.init_ul_rrc_msg_transfer()->nr_cgi.plmn_id.from_number(0xa);

  // Pass message to F1AP.
  f1ap->handle_message(init_ul_rrc_msg);

  EXPECT_EQ(f1ap->get_nof_ues(), 0);
}

TEST_F(f1ap_cu_test, when_rnti_invalid_then_ue_not_added)
{
  // Generate F1 Initial UL RRC Message.
  f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(int_to_gnb_du_ue_f1ap_id(41255));
  // Set RNTI to invalid value.
  init_ul_rrc_msg.pdu.init_msg().value.init_ul_rrc_msg_transfer()->c_rnti = 0;

  // Pass message to F1AP.
  f1ap->handle_message(init_ul_rrc_msg);

  EXPECT_EQ(f1ap->get_nof_ues(), 0);
}

TEST_F(f1ap_cu_test, when_max_nof_ues_exceeded_then_ue_not_added)
{
  // Reduce F1AP and TEST logger loglevel to warning to reduce console output.
  ocudulog::fetch_basic_logger("CU-CP-F1").set_level(ocudulog::basic_levels::warning);
  ocudulog::fetch_basic_logger("TEST").set_level(ocudulog::basic_levels::warning);

  // Add the maximum number of UEs.
  for (unsigned du_ue_id = 0; du_ue_id < max_nof_ues; du_ue_id++) {
    // Generate ue_creation message.
    f1ap_message init_ul_rrc_msg =
        test_helpers::generate_init_ul_rrc_message_transfer(int_to_gnb_du_ue_f1ap_id(du_ue_id));

    // Pass message to F1AP.
    f1ap->handle_message(init_ul_rrc_msg);
  }

  // Reset F1AP and TEST logger loglevel.
  ocudulog::fetch_basic_logger("CU-CP-F1").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("TEST").set_level(ocudulog::basic_levels::debug);

  EXPECT_EQ(f1ap->get_nof_ues(), max_nof_ues);

  // Add one more UE to F1AP.
  // Generate ue_creation message.
  f1ap_message init_ul_rrc_msg =
      test_helpers::generate_init_ul_rrc_message_transfer(int_to_gnb_du_ue_f1ap_id(max_nof_ues));

  // Pass message to F1AP.
  f1ap->handle_message(init_ul_rrc_msg);

  EXPECT_TRUE(was_rrc_reject_sent());

  // Inject UE context release complete from DU to complete UE context release procedure in F1AP.
  f1ap_message ue_context_release_complete = test_helpers::generate_ue_context_release_complete(
      int_to_gnb_cu_ue_f1ap_id(max_nof_ues), int_to_gnb_du_ue_f1ap_id(max_nof_ues));
  f1ap->handle_message(ue_context_release_complete);

  EXPECT_EQ(f1ap->get_nof_ues(), max_nof_ues);
}

TEST_F(f1ap_cu_test, when_ue_creation_fails_then_ue_not_added)
{
  // Add maximum number of UEs to dummy DU processor.
  du_processor_notifier.set_ue_id(max_nof_ues);

  // Add one more UE to F1AP.
  // Generate F1 Initial UL RRC Message.
  f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(int_to_gnb_du_ue_f1ap_id(41255));

  // Pass message to F1AP.
  f1ap->handle_message(init_ul_rrc_msg);

  // Make sure UE Context Release Command is sent.
  ASSERT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.type().value, asn1::f1ap::f1ap_pdu_c::types::init_msg);
  ASSERT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.init_msg().value.type().value,
            asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types::ue_context_release_cmd);

  // Create UE Context Release Complete and inject it.
  f1ap_message ue_ctxt_release_cmplt =
      test_helpers::generate_ue_context_release_complete(f1ap_pdu_notifier.last_f1ap_msg);
  f1ap->handle_message(ue_ctxt_release_cmplt);

  EXPECT_TRUE(was_rrc_reject_sent());

  // Inject UE context release complete from DU to complete UE context release procedure in F1AP.
  f1ap_message ue_context_release_complete =
      test_helpers::generate_ue_context_release_complete(int_to_gnb_cu_ue_f1ap_id(0), int_to_gnb_du_ue_f1ap_id(41255));
  f1ap->handle_message(ue_context_release_complete);

  EXPECT_EQ(f1ap->get_nof_ues(), 0);
}

//////////////////////////////////////////////////////////////////////////////////////
/* F1 Removal Request handling                                                      */
//////////////////////////////////////////////////////////////////////////////////////

TEST_F(f1ap_cu_test, when_f1_removal_request_received_then_f1_removal_response_is_sent)
{
  // Generate F1 Removal Request Message.
  f1ap_message removal_request = {};
  removal_request.pdu.set_init_msg();
  removal_request.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_F1_REMOVAL);
  removal_request.pdu.init_msg().value.f1_removal_request()->transaction_id = 5;

  // Pass message to F1AP.
  f1ap->handle_message(removal_request);

  ASSERT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.type().value, asn1::f1ap::f1ap_pdu_c::types::successful_outcome);
  ASSERT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.successful_outcome().value.type().value,
            asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types::f1_removal_resp);
  ASSERT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.successful_outcome().value.f1_removal_resp()->transaction_id, 5);
}

TEST_F(f1ap_cu_test, when_f1_removal_request_without_transaction_id_received_then_decoding_fails)
{
  // F1 Removal Request with an empty IE container.
  const uint8_t rx_msg[] = {0x00, 0x1a, 0x00, 0x03, 0x00, 0x00, 0x00};
  byte_buffer   buffer   = byte_buffer::create(rx_msg).value();

  f1ap_message   rx_removal_request = {};
  asn1::cbit_ref bref(buffer);
  ASSERT_NE(rx_removal_request.pdu.unpack(bref), asn1::OCUDUASN_SUCCESS);
}

TEST_F(f1ap_cu_test, redcap_initial_ul_rrc_identity_reaches_ue_creation)
{
  auto  msg     = test_helpers::generate_init_ul_rrc_message_transfer(int_to_gnb_du_ue_f1ap_id(41255));
  auto& initial = msg.pdu.init_msg().value.init_ul_rrc_msg_transfer();
  initial->nr_red_cap_ue_ind_present = true;
  initial->nr_red_cap_ue_ind         = nr_red_cap_ue_ind_opts::true_value;
  f1ap->handle_message(msg);
  ASSERT_EQ(f1ap->get_nof_ues(), 1);
  EXPECT_TRUE(du_processor_notifier.last_ue_creation_msg.is_redcap);
}

TEST_F(f1ap_cu_test, absent_redcap_indication_keeps_ordinary_ue_creation)
{
  auto msg = test_helpers::generate_init_ul_rrc_message_transfer(int_to_gnb_du_ue_f1ap_id(41255));
  f1ap->handle_message(msg);
  ASSERT_EQ(f1ap->get_nof_ues(), 1);
  EXPECT_FALSE(du_processor_notifier.last_ue_creation_msg.is_redcap);
}
