// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "asn1_test_utils.h"
#include "ocudu/asn1/e1ap/common.h"
#include "ocudu/asn1/e1ap/e1ap.h"
#include "ocudu/asn1/e1ap/e1ap_pdu_contents.h"
#include "ocudu/pcap/dlt_pcap.h"
#include "ocudu/support/executors/task_worker.h"
#include "ocudu/support/test_utils.h"
#include <gtest/gtest.h>

using namespace asn1;
using namespace ocudu;

#define JSON_OUTPUT 1

class asn1_e1ap_test : public ::testing::Test
{
protected:
  asn1_e1ap_test()
  {
    ocudulog::fetch_basic_logger("ASN1").set_level(ocudulog::basic_levels::debug);
    ocudulog::fetch_basic_logger("ASN1").set_hex_dump_max_size(-1);

    test_logger.set_level(ocudulog::basic_levels::debug);
    test_logger.set_hex_dump_max_size(-1);

    ocudulog::init();

    // Start the log backend.
    ocudulog::init();
  }

  ~asn1_e1ap_test()
  {
    // flush logger after each test
    ocudulog::flush();
  }

#if JSON_OUTPUT
  task_worker                    worker{"pcap_worker", 128};
  std::unique_ptr<task_executor> pcap_exec   = make_task_executor_ptr(worker);
  std::unique_ptr<dlt_pcap>      pcap_writer = create_e1ap_pcap("/tmp/e1ap.pcap", *pcap_exec);
#endif
  ocudulog::basic_logger& test_logger = ocudulog::fetch_basic_logger("TEST");
};

TEST_F(asn1_e1ap_test, when_gnb_cu_up_e1_setup_correct_then_packing_successful)
{
  auto& logger = ocudulog::fetch_basic_logger("ASN1", false);
  logger.set_level(ocudulog::basic_levels::debug);
  logger.set_hex_dump_max_size(-1);

  asn1::e1ap::e1ap_pdu_c pdu;
  pdu.set_init_msg();
  pdu.init_msg().value.set(asn1::e1ap::e1ap_elem_procs_o::init_msg_c::types::gnb_cu_up_e1_setup_request);
  pdu.init_msg().proc_code = ASN1_E1AP_ID_GNB_CU_UP_E1_SETUP;
  pdu.init_msg().crit      = crit_opts::reject;

  auto& setup_request             = pdu.init_msg().value.gnb_cu_up_e1_setup_request();
  setup_request->cn_support.value = asn1::e1ap::cn_support_opts::options::both;

  asn1::e1ap::supported_plmns_item_s plmn;
  plmn.plmn_id.from_string("214002");
  setup_request->supported_plmns.push_back(plmn);

  setup_request->gnb_cu_up_id           = 1;
  setup_request->gnb_cu_up_name_present = true;
  setup_request->gnb_cu_up_name.from_string("srs-cu-cp");

  ocudu::byte_buffer buffer;
  asn1::bit_ref      bref(buffer);
  ASSERT_EQ(pdu.pack(bref), OCUDUASN_SUCCESS);

  // TODO: Accept byte buffer in pcap and log.
  std::vector<uint8_t> bytes{buffer.begin(), buffer.end()};
#if JSON_OUTPUT
  pcap_writer->push_pdu(bytes);
#endif

  logger.info(bytes.data(), bytes.size(), "Packed PDU ({} bytes):", bref.distance_bytes());

#if JSON_OUTPUT
  int               unpacked_len = bref.distance_bytes();
  asn1::json_writer json_writer1;
  pdu.to_json(json_writer1);
  test_logger.info(
      bytes.data(), unpacked_len, "E1AP unpacked ({} B): \n {}", unpacked_len, json_writer1.to_string().c_str());
#endif
}

TEST_F(asn1_e1ap_test, when_bearer_context_setup_request_correct_then_unpacking_successful)
{
  uint8_t rx_msg[] = {0x00, 0x08, 0x00, 0x69, 0x00, 0x00, 0x07, 0x00, 0x02, 0x00, 0x02, 0x00, 0x09, 0x00, 0x0d, 0x00,
                      0x13, 0x00, 0x00, 0x10, 0xa6, 0xae, 0x39, 0xef, 0xbe, 0x0d, 0x42, 0x4c, 0xd8, 0x5f, 0x4a, 0x9c,
                      0x3a, 0xee, 0x04, 0x14, 0x00, 0x0e, 0x00, 0x05, 0x30, 0x3b, 0x9a, 0xca, 0x00, 0x00, 0x3a, 0x40,
                      0x03, 0x02, 0xf8, 0x99, 0x00, 0x17, 0x00, 0x01, 0x40, 0x00, 0x0f, 0x00, 0x2a, 0x40, 0x00, 0x01,
                      0x00, 0x2a, 0x00, 0x23, 0x00, 0x40, 0x01, 0x00, 0x02, 0x12, 0x30, 0x05, 0xf5, 0xe1, 0x00, 0x01,
                      0xf0, 0xac, 0x15, 0x04, 0x86, 0x00, 0x00, 0x00, 0x36, 0x00, 0x03, 0x01, 0x30, 0x0a, 0x20, 0xef,
                      0x00, 0x00, 0x00, 0x80, 0x00, 0x09, 0x7a, 0x00, 0x4d, 0x40, 0x02, 0x00, 0x00};
  ocudu::byte_buffer rx_pdu = byte_buffer::create(rx_msg).value();

#if JSON_OUTPUT
  pcap_writer->push_pdu(rx_msg);
#endif

  asn1::cbit_ref         bref{rx_pdu};
  asn1::e1ap::e1ap_pdu_c pdu;

  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);
  ASSERT_EQ(pdu.init_msg().proc_code, ASN1_E1AP_ID_BEARER_CONTEXT_SETUP);
  ASSERT_EQ(pdu.init_msg().value.type(),
            asn1::e1ap::e1ap_elem_procs_o::init_msg_c::types_opts::bearer_context_setup_request);
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);

  // Get the message.
  const auto& msg = pdu.init_msg().value.bearer_context_setup_request();

  ASSERT_TRUE(msg->gnb_du_id_present);
  ASSERT_EQ(0, msg->gnb_du_id);

  // Check the SystemBearerContextSetupRequest.
  ASSERT_EQ(msg->sys_bearer_context_setup_request.type().value,
            asn1::e1ap::sys_bearer_context_setup_request_c::types_opts::options::ng_ran_bearer_context_setup_request);

  const auto& bearer_cont = msg->sys_bearer_context_setup_request.ng_ran_bearer_context_setup_request();

  // Get session list.
  const auto& session_list = bearer_cont.pdu_session_res_to_setup_list;
  ASSERT_EQ(1, session_list.size());

  // Get first session item and check basic fields.
  const auto& session_item1 = session_list[0];
  ASSERT_EQ(session_item1.pdu_session_id, 1);
  ASSERT_EQ(session_item1.pdu_session_type, asn1::e1ap::pdu_session_type_opts::ipv4);

#if JSON_OUTPUT
  int               unpacked_len = bref.distance_bytes();
  asn1::json_writer json_writer1;
  pdu.to_json(json_writer1);
  test_logger.info(
      rx_msg, unpacked_len, "E1AP PDU unpacked ({} B): \n {}", unpacked_len, json_writer1.to_string().c_str());

  asn1::json_writer json_writer2;
  session_item1.to_json(json_writer2);
  test_logger.info("PDU session item unpacked: \n {}", json_writer2.to_string().c_str());
#endif
}

TEST_F(asn1_e1ap_test, when_bearer_context_setup_response_correct_then_unpacking_successful)
{
  uint8_t rx_msg[]          = {0x20, 0x08, 0x00, 0x37, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x09, 0x00, 0x03,
                               0x00, 0x03, 0x40, 0x02, 0x80, 0x00, 0x10, 0x40, 0x23, 0x40, 0x00, 0x01, 0x00, 0x2e, 0x40,
                               0x1c, 0x00, 0x00, 0x01, 0x01, 0xf0, 0xac, 0x15, 0x06, 0x09, 0x00, 0x00, 0x02, 0x83, 0x00,
                               0x06, 0x00, 0x1f, 0xac, 0x15, 0x06, 0x09, 0x80, 0x00, 0x02, 0x83, 0x00, 0x00, 0x80};
  ocudu::byte_buffer rx_pdu = byte_buffer::create(rx_msg).value();

#if JSON_OUTPUT
  pcap_writer->push_pdu(rx_msg);
#endif

  asn1::cbit_ref         bref{rx_pdu};
  asn1::e1ap::e1ap_pdu_c pdu;

  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);
  ASSERT_EQ(pdu.init_msg().proc_code, ASN1_E1AP_ID_BEARER_CONTEXT_SETUP);
  ASSERT_EQ(pdu.init_msg().value.type(),
            asn1::e1ap::e1ap_elem_procs_o::successful_outcome_c::types_opts::bearer_context_setup_resp);
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);

  // Get the message.
  const auto& msg = pdu.successful_outcome().value.bearer_context_setup_resp();

  // Check IDs.
  ASSERT_EQ(msg->gnb_cu_cp_ue_e1ap_id, 9);
  ASSERT_EQ(msg->gnb_cu_up_ue_e1ap_id, 640);

  // Check the SystemBearerContextSetupResponse.
  ASSERT_EQ(msg->sys_bearer_context_setup_resp.type().value,
            asn1::e1ap::sys_bearer_context_setup_resp_c::types_opts::options::ng_ran_bearer_context_setup_resp);

  const auto& bearer_cont = msg->sys_bearer_context_setup_resp.ng_ran_bearer_context_setup_resp();

  ASSERT_FALSE(bearer_cont.pdu_session_res_failed_list_present);
  ASSERT_EQ(bearer_cont.pdu_session_res_setup_list.size(), 1);

  // Get first resource setup list item.
  for (const auto& item : bearer_cont.pdu_session_res_setup_list) {
    ASSERT_EQ(item.pdu_session_id, 1);
  }

  // TODO: unpack and check GTPu tunnel and DRB setup list

#if JSON_OUTPUT
  int               unpacked_len = bref.distance_bytes();
  asn1::json_writer json_writer1;
  pdu.to_json(json_writer1);
  test_logger.info(
      rx_msg, unpacked_len, "E1AP PDU unpacked ({} B): \n {}", unpacked_len, json_writer1.to_string().c_str());
#endif
}

// Real PDUs from lab captures.
static const std::vector<capture_pdu> e1ap_captures = {
    {"GNB-CU-UP-E1SetupRequest",
     "0003002a0000050039000200000007000200000008400903006f637575705f30000a000120000b0005000000f110"},
    {"E1ReleaseRequest", "0007000f000002003900020001000040020a00"},
    {"BearerContextSetupRequest",
     "000800809a000007000200020000000d0044104820dc7694dc2426b75051db0695a896d71d3de8a4a24415e2ce96a7c0"
     "032e94c9292044986a713e167e4c883922bcf8b6791484db34f72a90c43f436f207be0cc3c05000e00064002540be400"
     "003a400300f1100017000140003b0003000077000f0028400001002a002100000104020000010900f8ac1400482d36ef"
     "41000005300a214e00000080000978"},
    {"BearerContextModificationRequest",
     "0009002d0000030002000200000003000200000012001a400001002b001300004001001000000f807f000a0200000001"
     "00"},
    {"BearerContextReleaseCommand", "000b0015000003000200020000000300020000000040020a00"},
    {"GNB-CU-UP-E1SetupResponse", "20030009000001003900020000"},
    {"E1ReleaseResponse", "20070009000001003900020001"},
    {"BearerContextSetupResponse",
     "2008003600000300020002000000030002000000104023400001002e401c00000101f0ac140014000000010000001f7f"
     "000a0100000001000080"},
    {"BearerContextModificationResponse",
     "2009002c00000300020002000000030002000000134019400001003000120002000102000007c0000000000000000000"},
    {"BearerContextReleaseComplete", "200b000f000002000200020000000300020000"},
};

TEST_F(asn1_e1ap_test, captured_pdus_repack_to_same_bytes)
{
  for (const capture_pdu& c : e1ap_captures) {
    EXPECT_EQ(test_unpack_roundtrip<e1ap::e1ap_pdu_c>(hex_to_bytes(c.hex)), OCUDUASN_SUCCESS) << c.name;
  }
}
