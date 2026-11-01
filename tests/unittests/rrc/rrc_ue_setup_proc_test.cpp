// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "rrc_ue_test_helpers.h"
#include "tests/test_doubles/rrc/rrc_test_messages.h"
#include "ocudu/adt/format.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;

/// Fixture class RRC Setup tests preparation.
class rrc_ue_setup : public rrc_ue_test_helper, public ::testing::Test
{
protected:
  static void SetUpTestSuite() { ocudulog::init(); }

  void SetUp() override { init(); }

  void TearDown() override
  {
    // Flush logger after each test.
    ocudulog::flush();
  }
};

/// Test the RRC setup with disconnected AMF.
TEST_F(rrc_ue_setup, when_amf_disconnected_then_rrc_reject_sent)
{
  this->rrc_ue_cu_cp_notifier.next_ue_setup_response = false;

  receive_setup_request();

  check_ue_release_requested();
}

/// Test the RRC setup with connected AMF.
TEST_F(rrc_ue_setup, when_amf_connected_then_rrc_setup_sent)
{
  receive_setup_request();

  // Check if the RRC setup message was generated.
  ASSERT_EQ(get_srb0_pdu_type(), asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types::rrc_setup);

  // Check if SRB1 was created.
  check_srb1_exists();

  receive_setup_complete();
}

/// Test the RRC setup with disconnected AMF.
TEST_F(rrc_ue_setup, when_rrc_setup_invalid_then_rrc_reject_sent)
{
  receive_invalid_setup_request();

  check_ue_release_requested();
}

/// Test the correct handling of missing RRC setup complete message.
TEST_F(rrc_ue_setup, when_setup_complete_timeout_then_ue_deleted)
{
  receive_setup_request();

  // Check that UE has been created and was not requested to be released.
  check_ue_release_not_requested();
  ASSERT_EQ(rrc_ue_rrc_du_notifier.nof_failed_rrc_connection_establishments, 0);

  // Tick timer until RRC setup complete timer fires.
  tick_timer();

  // Verify that RRC requested UE context release.
  check_ue_release_requested();

  // Verify that the failure was counted as NoReply, per TS 28.552 section 5.1.1.15.3.
  ASSERT_EQ(rrc_ue_rrc_du_notifier.nof_failed_rrc_connection_establishments, 1);
  ASSERT_EQ(rrc_ue_rrc_du_notifier.last_establishment_fail_cause, establishment_fail_cause_t::no_reply);
}

/// Test that a UE-specific failure, which unreceives RRC Setup Complete, is counted as NoReply, TS 28.552
/// section 5.1.1.15.3.
TEST_F(rrc_ue_setup, when_setup_transaction_cancelled_abnormally_then_counted_as_no_reply)
{
  receive_setup_request();

  check_ue_release_not_requested();
  ASSERT_EQ(rrc_ue_rrc_du_notifier.nof_failed_rrc_connection_establishments, 0);

  // Simulate a DU-reported UE-specific failure cancelling the pending RRC Setup transaction.
  get_rrc_ue_control_message_handler()->cancel_all_transactions(protocol_transaction_failure::abnormal);
  ctrl_worker.run_pending_tasks();

  ASSERT_EQ(rrc_ue_rrc_du_notifier.nof_failed_rrc_connection_establishments, 1);
  ASSERT_EQ(rrc_ue_rrc_du_notifier.last_establishment_fail_cause, establishment_fail_cause_t::no_reply);
}

TEST_F(rrc_ue_setup, when_setup_complete_received_initial_ue_message_sent)
{
  receive_setup_request();

  // Check if the RRC setup message was generated.
  ASSERT_EQ(get_srb0_pdu_type(), asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types::rrc_setup);

  // Check if SRB1 was created.
  check_srb1_exists();

  receive_setup_complete();

  check_initial_ue_message_sent();
}

/// Packs an RRC Setup Complete for SRB1, which has no integrity protection yet.
static byte_buffer pack_rrc_setup_complete(const asn1::rrc_nr::ul_dcch_msg_s& msg)
{
  byte_buffer pdu = byte_buffer::create({0x00, 0x00}).value();
  report_fatal_error_if_not(pdu.append(test_helpers::pack_ul_dcch_msg(msg)), "Failed to append the RRC Setup Complete");
  report_fatal_error_if_not(pdu.append(std::array<uint8_t, 4>{}), "Failed to append the MAC-I");
  return pdu;
}

TEST_F(rrc_ue_setup, when_setup_complete_contains_registered_amf_then_its_guami_is_given_to_the_amf_selection)
{
  receive_setup_request();

  // Registered AMF with AMF Region ID 2, AMF Set ID 1 and AMF Pointer 1, mapped from EPS.
  asn1::rrc_nr::ul_dcch_msg_s msg = test_helpers::create_rrc_setup_complete();
  auto&                       ies = msg.msg.c1().rrc_setup_complete().crit_exts.rrc_setup_complete();
  ies.registered_amf_present      = true;
  ies.registered_amf.amf_id.from_number(0x020041);
  ies.guami_type_present = true;
  ies.guami_type.value   = asn1::rrc_nr::rrc_setup_complete_ies_s::guami_type_opts::mapped;
  pdcp_ctx->handle_ul_dcch_pdu(srb_id_t::srb1, pack_rrc_setup_complete(msg));

  // The registered AMF has no PLMN identity, so its PLMN is the selected PLMN.
  const amf_selection_request& request = rrc_ue_cu_cp_notifier.last_amf_selection_request;
  ASSERT_EQ(request.plmn, plmn_identity::test_value());
  ASSERT_TRUE(request.registered_guami.has_value());
  ASSERT_EQ(request.registered_guami->plmn, plmn_identity::test_value());
  ASSERT_EQ(request.registered_guami->amf_region_id, 2);
  ASSERT_EQ(request.registered_guami->amf_set_id, 1);
  ASSERT_EQ(request.registered_guami->amf_pointer, 1);
  ASSERT_EQ(request.registered_guami_type, guami_type_t::mapped);
  ASSERT_FALSE(request.five_g_s_tmsi.has_value());
}

TEST_F(rrc_ue_setup, when_setup_complete_contains_5g_s_tmsi_then_it_is_given_to_the_amf_selection)
{
  receive_setup_request();

  // 5G-S-TMSI with AMF Set ID 1 and AMF Pointer 0.
  pdcp_ctx->handle_ul_dcch_pdu(srb_id_t::srb1,
                               pack_rrc_setup_complete(test_helpers::create_rrc_setup_complete(1, 0x0040c000011b)));

  const amf_selection_request& request = rrc_ue_cu_cp_notifier.last_amf_selection_request;
  ASSERT_TRUE(request.five_g_s_tmsi.has_value());
  ASSERT_EQ(request.five_g_s_tmsi->get_amf_set_id(), 1);
  ASSERT_EQ(request.five_g_s_tmsi->get_amf_pointer(), 0);
  ASSERT_FALSE(request.registered_guami.has_value());
}

TEST_F(rrc_ue_setup, when_5g_s_tmsi_is_sent_in_two_parts_then_both_parts_are_given_to_the_amf_selection)
{
  // 5G-S-TMSI with AMF Set ID 0x201, AMF Pointer 5 and 5G-TMSI 0x1234 (TS 23.003 section 2.11). The AMF Set ID has a
  // bit in each part.
  const uint64_t five_g_s_tmsi = (uint64_t{0x201} << 38U) + (uint64_t{5} << 32U) + 0x1234;

  // RRC Setup Request with the 39 rightmost bits of the 5G-S-TMSI (TS 38.331 section 5.3.3.3).
  asn1::rrc_nr::ul_ccch_msg_s ul_ccch_msg;
  auto&                       request_ies = ul_ccch_msg.msg.set_c1().set_rrc_setup_request().rrc_setup_request;
  request_ies.ue_id.set_ng_5_g_s_tmsi_part1().from_number(five_g_s_tmsi & ((uint64_t{1} << 39U) - 1));
  request_ies.establishment_cause.value = asn1::rrc_nr::establishment_cause_opts::mo_sig;
  rrc_ue->get_ul_pdu_handler().handle_ul_ccch_pdu(test_helpers::pack_ul_ccch_msg(ul_ccch_msg), to_rnti(0x1234));

  // RRC Setup Complete with the 9 leftmost bits of the 5G-S-TMSI.
  asn1::rrc_nr::ul_dcch_msg_s msg = test_helpers::create_rrc_setup_complete();
  auto&                       ies = msg.msg.c1().rrc_setup_complete().crit_exts.rrc_setup_complete();
  ies.ng_5_g_s_tmsi_value_present = true;
  ies.ng_5_g_s_tmsi_value.set_ng_5_g_s_tmsi_part2().from_number(five_g_s_tmsi >> 39U);
  pdcp_ctx->handle_ul_dcch_pdu(srb_id_t::srb1, pack_rrc_setup_complete(msg));

  const amf_selection_request& request = rrc_ue_cu_cp_notifier.last_amf_selection_request;
  ASSERT_TRUE(request.five_g_s_tmsi.has_value());
  ASSERT_EQ(request.five_g_s_tmsi->get_amf_set_id(), 0x201);
  ASSERT_EQ(request.five_g_s_tmsi->get_amf_pointer(), 5);
}

TEST_F(rrc_ue_setup, when_registered_amf_has_a_plmn_identity_then_the_guami_has_this_plmn)
{
  receive_setup_request();

  asn1::rrc_nr::ul_dcch_msg_s msg = test_helpers::create_rrc_setup_complete();
  auto&                       ies = msg.msg.c1().rrc_setup_complete().crit_exts.rrc_setup_complete();
  ies.registered_amf_present      = true;
  ies.registered_amf.amf_id.from_number(0x020041);
  ies.registered_amf.plmn_id_present     = true;
  ies.registered_amf.plmn_id.mcc_present = true;
  ies.registered_amf.plmn_id.mcc         = {9, 9, 9};
  ies.registered_amf.plmn_id.mnc.resize(2);
  ies.registered_amf.plmn_id.mnc[0] = 0;
  ies.registered_amf.plmn_id.mnc[1] = 2;
  pdcp_ctx->handle_ul_dcch_pdu(srb_id_t::srb1, pack_rrc_setup_complete(msg));

  const amf_selection_request& request = rrc_ue_cu_cp_notifier.last_amf_selection_request;
  ASSERT_TRUE(request.registered_guami.has_value());
  ASSERT_EQ(request.registered_guami->plmn, plmn_identity::parse("99902").value());
}

/// Test the correct handling of corrupted RRC setup complete message.
TEST_F(rrc_ue_setup, when_setup_complete_has_s_nssais_then_they_are_given_to_the_amf_selection)
{
  receive_setup_request();

  asn1::rrc_nr::ul_dcch_msg_s msg = test_helpers::create_rrc_setup_complete();
  auto&                       ies = msg.msg.c1().rrc_setup_complete().crit_exts.rrc_setup_complete();
  ies.s_nssai_list.resize(2);
  ies.s_nssai_list[0].set_sst().from_number(1);
  ies.s_nssai_list[1].set_sst_sd().from_number((2U << 24U) + 0x10U);
  pdcp_ctx->handle_ul_dcch_pdu(srb_id_t::srb1, pack_rrc_setup_complete(msg));

  const amf_selection_request& request = rrc_ue_cu_cp_notifier.last_amf_selection_request;
  ASSERT_EQ(request.requested_nssai.size(), 2);
  ASSERT_EQ(request.requested_nssai[0].sst.value(), 1);
  ASSERT_FALSE(request.requested_nssai[0].sd.is_set());
  ASSERT_EQ(request.requested_nssai[1].sst.value(), 2);
  ASSERT_EQ(request.requested_nssai[1].sd.value(), 0x10);
}

TEST_F(rrc_ue_setup, when_integrity_failure_detected_then_ue_deleted)
{
  receive_setup_request();

  // Check if the RRC setup message was generated.
  ASSERT_EQ(get_srb0_pdu_type(), asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types::rrc_setup);

  // Check if SRB1 was created.
  check_srb1_exists();

  receive_corrupted_setup_complete();

  // Tick timer until RRC setup complete timer fires.
  tick_timer();

  // Verify that RRC requested UE context release.
  check_ue_release_requested();
}

/// Test the correct handling of corrupted RRC setup complete message.
TEST_F(rrc_ue_setup, when_malformed_setup_complete_received_then_ue_deleted)
{
  receive_setup_request();

  // Check if the RRC setup message was generated.
  ASSERT_EQ(get_srb0_pdu_type(), asn1::rrc_nr::dl_ccch_msg_type_c::c1_c_::types::rrc_setup);

  // Check if SRB1 was created.
  check_srb1_exists();

  // UL-DCCH with malformed RRC setup complete message.
  std::array<uint8_t, 28> malformed_rrc_setup_complete_pdu = {
      0x00, 0x00, 0x18, 0x00, 0x05, 0xdf, 0x80, 0x10, 0x5e, 0x40, 0x03, 0x40, 0x40, 0x3c,
      0x44, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0c, 0x95, 0x1d, 0x82, 0x0b, 0x80, 0xbc, 0x1c};

  // Inject malformed RRC setup complete.
  pdcp_ctx->handle_ul_dcch_pdu(srb_id_t::srb1, byte_buffer::create(malformed_rrc_setup_complete_pdu).value());
  // Tick timer until RRC setup complete timer fires.
  tick_timer();

  // Verify that RRC requested UE context release.
  check_ue_release_requested();
}
