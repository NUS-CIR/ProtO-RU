// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "asn1_test_utils.h"
#include "ocudu/asn1/ngap/common.h"
#include "ocudu/asn1/ngap/ngap.h"
#include "ocudu/asn1/ngap/ngap_pdu_contents.h"
#include "ocudu/support/test_utils.h"
#include <gtest/gtest.h>

using namespace asn1;
using namespace asn1::ngap;

#define JSON_OUTPUT 1

class asn1_ngap_test : public ::testing::Test
{
public:
  ~asn1_ngap_test() override
  {
    // flush logger after each test
    ocudulog::flush();
  }

protected:
  asn1_ngap_test()
  {
    ocudulog::fetch_basic_logger("ASN1").set_level(ocudulog::basic_levels::debug);
    ocudulog::fetch_basic_logger("ASN1").set_hex_dump_max_size(-1);

    test_logger.set_level(ocudulog::basic_levels::debug);
    test_logger.set_hex_dump_max_size(-1);

    // Start the log backend.
    ocudulog::init();
  }

  ocudulog::basic_logger& test_logger = ocudulog::fetch_basic_logger("TEST");
};

// initiating message NGSetupRequest
TEST_F(asn1_ngap_test, when_setup_message_correct_then_packing_successful)
{
  asn1::ngap::ngap_pdu_c pdu;

  pdu.set_init_msg();
  pdu.init_msg().load_info_obj(ASN1_NGAP_ID_NG_SETUP);

  auto& setup_req = pdu.init_msg().value.ng_setup_request();
  setup_req->global_ran_node_id.set_global_gnb_id();
  setup_req->global_ran_node_id.global_gnb_id().gnb_id.set_gnb_id();
  setup_req->global_ran_node_id.global_gnb_id().gnb_id.gnb_id().from_number(411);
  setup_req->global_ran_node_id.global_gnb_id().plmn_id.from_string("00f110");

  setup_req->ran_node_name_present = true;
  setup_req->ran_node_name.from_string("gnb01");

  asn1::ngap::supported_ta_item_s   supported_ta_item   = {};
  asn1::ngap::broadcast_plmn_item_s broadcast_plmn_item = {};
  broadcast_plmn_item.plmn_id.from_string("00f110");
  asn1::ngap::slice_support_item_s slice_support_item = {};
  slice_support_item.s_nssai.sst.from_number(1);
  broadcast_plmn_item.tai_slice_support_list.push_back(slice_support_item);
  supported_ta_item.broadcast_plmn_list.push_back(broadcast_plmn_item);

  setup_req->supported_ta_list.push_back(supported_ta_item);

  setup_req->default_paging_drx.value = asn1::ngap::paging_drx_opts::v256;

  ocudu::byte_buffer tx_buffer;
  asn1::bit_ref      bref(tx_buffer);
  ASSERT_EQ(pdu.pack(bref), OCUDUASN_SUCCESS);

  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);

  std::vector<uint8_t> bytes{tx_buffer.begin(), tx_buffer.end()};
#if JSON_OUTPUT
  asn1::json_writer json_writer1;
  pdu.to_json(json_writer1);
  test_logger.info(
      bytes.data(), bytes.size(), "NGAP PDU unpacked ({} B): \n {}", bytes.size(), json_writer1.to_string().c_str());
#endif
}

// successful outcome NGSetupResponse
TEST_F(asn1_ngap_test, when_setup_response_correct_then_packing_successful)
{
  asn1::ngap::ngap_pdu_c pdu;

  pdu.set_successful_outcome();
  pdu.successful_outcome().load_info_obj(ASN1_NGAP_ID_NG_SETUP);

  auto& setup_res = pdu.successful_outcome().value.ng_setup_resp();
  setup_res->amf_name.from_string("open5gs-amf0");

  asn1::ngap::served_guami_item_s served_guami_item;
  served_guami_item.guami.plmn_id.from_string("00f110");
  served_guami_item.guami.amf_region_id.from_number(2);
  served_guami_item.guami.amf_set_id.from_number(1);
  served_guami_item.guami.amf_pointer.from_number(0);
  setup_res->served_guami_list.push_back(served_guami_item);

  setup_res->relative_amf_capacity = 255;

  asn1::ngap::plmn_support_item_s plmn_support_item = {};
  plmn_support_item.plmn_id.from_string("00f110");

  asn1::ngap::slice_support_item_s slice_support_item = {};
  slice_support_item.s_nssai.sst.from_number(1);
  plmn_support_item.slice_support_list.push_back(slice_support_item);

  setup_res->plmn_support_list.push_back(plmn_support_item);

  ocudu::byte_buffer tx_pdu;
  asn1::bit_ref      bref(tx_pdu);
  ASSERT_EQ(pdu.pack(bref), OCUDUASN_SUCCESS);

  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);

  std::vector<uint8_t> tx_buffer{tx_pdu.begin(), tx_pdu.end()};
#if JSON_OUTPUT
  asn1::json_writer json_writer1;
  pdu.to_json(json_writer1);
  test_logger.info(tx_buffer.data(),
                   tx_buffer.size(),
                   "NGAP PDU unpacked ({} B): \n {}",
                   tx_buffer.size(),
                   json_writer1.to_string().c_str());
#endif
}

// unsuccessful outcome NGSetupFailure
TEST_F(asn1_ngap_test, when_setup_failure_correct_then_packing_successful)
{
  asn1::ngap::ngap_pdu_c pdu;

  pdu.set_unsuccessful_outcome();
  pdu.unsuccessful_outcome().load_info_obj(ASN1_NGAP_ID_NG_SETUP);

  auto& setup_fail = pdu.unsuccessful_outcome().value.ng_setup_fail();
  setup_fail->cause.set_radio_network();
  setup_fail->cause.radio_network() = asn1::ngap::cause_radio_network_opts::options::unspecified;
  setup_fail->time_to_wait_present  = true;
  setup_fail->time_to_wait.value    = asn1::ngap::time_to_wait_opts::v10s;
  // add critical diagnostics

  ocudu::byte_buffer tx_pdu;
  asn1::bit_ref      bref(tx_pdu);
  ASSERT_EQ(pdu.pack(bref), OCUDUASN_SUCCESS);

  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);

  std::vector<uint8_t> tx_buffer{tx_pdu.begin(), tx_pdu.end()};
#if JSON_OUTPUT
  asn1::json_writer json_writer1;
  pdu.to_json(json_writer1);
  test_logger.info(tx_buffer.data(),
                   tx_buffer.size(),
                   "NGAP PDU unpacked ({} B): \n {}",
                   tx_buffer.size(),
                   json_writer1.to_string().c_str());
#endif
}

TEST_F(asn1_ngap_test, ng_setup_request_pack_and_unpack)
{
  const uint8_t ngap_msg[] = {0x00, 0x15, 0x00, 0x30, 0x00, 0x00, 0x04, 0x00, 0x1b, 0x00, 0x09, 0x00, 0x00,
                              0xf1, 0x10, 0x50, 0x00, 0x00, 0x00, 0x01, 0x00, 0x52, 0x40, 0x06, 0x01, 0x80,
                              0x67, 0x6e, 0x62, 0x31, 0x00, 0x66, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x75,
                              0x00, 0x00, 0xf1, 0x10, 0x00, 0x00, 0x00, 0x08, 0x00, 0x15, 0x40, 0x01, 0x60};
  // 00150030000004001b00090000f1105000000001005240060180676e62310066000d00000000750000f110000000080015400160
  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);

  ngap_pdu_c pdu;
  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);

  ASSERT_EQ(pdu.type().value, ngap_pdu_c::types_opts::init_msg);
  ASSERT_EQ(pdu.init_msg().proc_code, 21);
  ASSERT_EQ(pdu.init_msg().crit.value, crit_opts::reject);
  ASSERT_EQ(pdu.init_msg().value.type().value, ngap_elem_procs_o::init_msg_c::types_opts::ng_setup_request);
  ng_setup_request_s& ngsetup = pdu.init_msg().value.ng_setup_request();
  // Field 0
  ASSERT_EQ(ngsetup->global_ran_node_id.type().value, global_ran_node_id_c::types_opts::global_gnb_id);
  global_gnb_id_s& global_gnb = ngsetup->global_ran_node_id.global_gnb_id();
  ASSERT_EQ(global_gnb.plmn_id.to_number(), 0xf110);
  ASSERT_EQ(global_gnb.gnb_id.type().value, gnb_id_c::types_opts::gnb_id);
  ASSERT_EQ(global_gnb.gnb_id.gnb_id().to_number(), 1);
  // Field 1
  ASSERT_TRUE(ngsetup->ran_node_name_present);
  // Field 2
  ASSERT_EQ(ngsetup->supported_ta_list.size(), 1);
  ASSERT_EQ(ngsetup->supported_ta_list[0].tac.to_number(), 0x75);
  ASSERT_EQ(ngsetup->supported_ta_list[0].broadcast_plmn_list.size(), 1);
  auto& bcast_item = ngsetup->supported_ta_list[0].broadcast_plmn_list[0];
  ASSERT_EQ(bcast_item.plmn_id.to_number(), 0xf110);
  ASSERT_GT(bcast_item.tai_slice_support_list.size(), 0);
  ASSERT_EQ(bcast_item.tai_slice_support_list[0].s_nssai.sst.to_number(), 1);
  // Field 3
  ASSERT_EQ(ngsetup->default_paging_drx.value, paging_drx_opts::v256);

  ASSERT_EQ(bref.distance_bytes(), sizeof(ngap_msg));
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);

  json_writer js;
  pdu.to_json(js);
  printf("PDU json: %s\n", js.to_string().c_str());
}

TEST_F(asn1_ngap_test, ng_setup_response_pack_and_unpack)
{
  const uint8_t ngap_msg[] = {0x20, 0x15, 0x00, 0x5e, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x3a, 0x1b, 0x80, 0x61,
                              0x6d, 0x66, 0x31, 0x2e, 0x63, 0x6c, 0x75, 0x73, 0x74, 0x65, 0x72, 0x31, 0x2e, 0x6e,
                              0x65, 0x74, 0x32, 0x2e, 0x61, 0x6d, 0x66, 0x2e, 0x35, 0x67, 0x63, 0x2e, 0x6d, 0x6e,
                              0x63, 0x30, 0x30, 0x31, 0x2e, 0x6d, 0x63, 0x63, 0x30, 0x30, 0x31, 0x2e, 0x33, 0x67,
                              0x70, 0x70, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x2e, 0x6f, 0x72, 0x67, 0x00,
                              0x60, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf1, 0x10, 0x38, 0x08, 0x97, 0x00, 0x56, 0x40,
                              0x01, 0x05, 0x00, 0x50, 0x00, 0x08, 0x00, 0x00, 0xf1, 0x10, 0x00, 0x00, 0x00, 0x08};
  // 2015005e0000040001003a1b80616d66312e636c7573746572312e6e6574322e616d662e3567632e6d6e633030312e6d63633030312e336770706e6574776f726b2e6f726700600008000000f1103808970056400105005000080000f11000000008
  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);

  ngap_pdu_c pdu;
  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);

  // Check Fields
  ASSERT_EQ(pdu.type().value, ngap_pdu_c::types_opts::successful_outcome);
  ASSERT_EQ(pdu.successful_outcome().proc_code, 21);
  ASSERT_EQ(pdu.successful_outcome().crit.value, crit_opts::reject);
  ASSERT_EQ(pdu.successful_outcome().value.type().value,
            ngap_elem_procs_o::successful_outcome_c::types_opts::ng_setup_resp);
  ng_setup_resp_s& resp = pdu.successful_outcome().value.ng_setup_resp();
  // field 0
  ASSERT_EQ(resp->amf_name.size(), 56);
  ASSERT_EQ(resp->amf_name.to_string(), "amf1.cluster1.net2.amf.5gc.mnc001.mcc001.3gppnetwork.org");
  // field 1
  ASSERT_EQ(resp->served_guami_list.size(), 1);
  ASSERT_EQ(resp->served_guami_list[0].guami.plmn_id.to_number(), 0xf110);
  ASSERT_EQ(resp->served_guami_list[0].guami.amf_region_id.to_number(), 0b111000);
  ASSERT_EQ(resp->served_guami_list[0].guami.amf_set_id.to_number(), 0b100010);
  ASSERT_EQ(resp->served_guami_list[0].guami.amf_pointer.to_number(), 0b10111);
  // field 2
  // ...

  ASSERT_EQ(bref.distance_bytes(), sizeof(ngap_msg));
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);
}

TEST_F(asn1_ngap_test, amf_config_update)
{
  const uint8_t ngap_msg[] = {
      0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x08, 0x02, 0x80, 0x74, 0x73, 0x74, 0x72, 0x61, 0x6e};
  // 0000000F00000100010008028074737472616E
  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);

  ngap_pdu_c pdu;
  ASSERT_EQ(OCUDUASN_SUCCESS, pdu.unpack(bref));

  ASSERT_EQ(ngap_pdu_c::types_opts::init_msg, pdu.type().value);
  ASSERT_EQ(0, pdu.init_msg().proc_code);
  ASSERT_EQ(crit_opts::reject, pdu.init_msg().crit.value);
  ngap_elem_procs_o::init_msg_c& init_choice = pdu.init_msg().value;
  ASSERT_EQ(ngap_elem_procs_o::init_msg_c::types_opts::amf_cfg_upd, init_choice.type().value);
  amf_cfg_upd_s& amf_upd  = init_choice.amf_cfg_upd();
  auto&          amf_name = amf_upd->amf_name;
  ASSERT_TRUE(amf_upd->amf_name_present);
  ASSERT_EQ("tstran", amf_name.to_string());

  ASSERT_EQ(sizeof(ngap_msg), std::ceil(bref.distance_bytes()));
  ASSERT_EQ(OCUDUASN_SUCCESS, test_pack_roundtrip(pdu));

  json_writer js;
  pdu.to_json(js);
  printf("PDU json: %s\n", js.to_string().c_str());
}

TEST_F(asn1_ngap_test, init_ue_msg_pack_and_unpack)
{
  const uint8_t ngap_msg[] = {
      0x00, 0x0f, 0x40, 0x80, 0xa2, 0x00, 0x00, 0x04, 0x00, 0x55, 0x00, 0x02, 0x00, 0x01, 0x00, 0x26, 0x00, 0x7d, 0x7c,
      0x7e, 0x00, 0x41, 0x71, 0x00, 0x76, 0x01, 0x00, 0xf1, 0x10, 0x00, 0x00, 0x01, 0x01, 0x4d, 0x43, 0x6f, 0x77, 0x42,
      0x51, 0x59, 0x44, 0x4b, 0x32, 0x56, 0x75, 0x41, 0x79, 0x45, 0x41, 0x6e, 0x36, 0x36, 0x48, 0x39, 0x6b, 0x7a, 0x48,
      0x54, 0x61, 0x46, 0x5a, 0x4b, 0x30, 0x35, 0x37, 0x41, 0x49, 0x72, 0x37, 0x41, 0x2b, 0x6e, 0x6c, 0x73, 0x61, 0x49,
      0x58, 0x78, 0x52, 0x33, 0x4e, 0x69, 0x73, 0x36, 0x4c, 0x56, 0x6f, 0x75, 0x46, 0x69, 0x42, 0x34, 0x3d, 0xdf, 0xab,
      0xf5, 0xcd, 0x65, 0x2e, 0xb2, 0x54, 0x14, 0x91, 0x48, 0x4d, 0x41, 0x43, 0x2d, 0x53, 0x48, 0x41, 0x00, 0x85, 0x8b,
      0xbb, 0x1f, 0x42, 0xf1, 0x25, 0x6f, 0x9a, 0x37, 0x53, 0x1a, 0x77, 0x2a, 0x2c, 0xf2, 0xb7, 0x8f, 0xf1, 0x60, 0x48,
      0x84, 0x02, 0xed, 0x48, 0x93, 0x99, 0xb6, 0xb7, 0x37, 0x42, 0x00, 0x79, 0x00, 0x0f, 0x40, 0x00, 0xf1, 0x10, 0x00,
      0x00, 0x00, 0x00, 0x10, 0x00, 0xf1, 0x10, 0x00, 0x00, 0x75, 0x00, 0x5a, 0x40, 0x01, 0x18};
  // 000f4080a20000040055000200010026007d7c7e00417100760100f110000001014d436f77425159444b325675417945416e363648396b7a485461465a4b30353741497237412b6e6c736149587852334e6973364c566f75466942343ddfabf5cd652eb2541491484d41432d53484100858bbb1f42f1256f9a37531a772a2cf2b78ff160488402ed489399b6b737420079000f4000f110000000001000f110000075005a400118

  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);
  ngap_pdu_c               pdu;
  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);

  // Check Fields
  ASSERT_EQ(pdu.type().value, ngap_pdu_c::types_opts::init_msg);
  ASSERT_EQ(pdu.init_msg().proc_code, 15);
  ASSERT_EQ(pdu.init_msg().crit.value, crit_opts::ignore);
  ASSERT_EQ(pdu.init_msg().value.type().value, ngap_elem_procs_o::init_msg_c::types_opts::init_ue_msg);
  auto& container = *pdu.init_msg().value.init_ue_msg();
  // Field 0
  ASSERT_EQ(container.ran_ue_ngap_id, 1);
  // Field 1
  ASSERT_EQ(container.nas_pdu.size(), 124);
  // Field 2
  ASSERT_EQ(container.user_location_info.type().value, user_location_info_c::types_opts::user_location_info_nr);
  ASSERT_EQ(container.user_location_info.user_location_info_nr().nr_cgi.plmn_id.to_number(), 0xf110);

  ASSERT_EQ(bref.distance_bytes(), sizeof(ngap_msg));
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);
}

TEST_F(asn1_ngap_test, dl_nas_transport_pack_and_unpack)
{
  const uint8_t ngap_msg[] = {0x00, 0x04, 0x40, 0x3e, 0x00, 0x00, 0x03, 0x00, 0x0a, 0x00, 0x02, 0x00, 0x01, 0x00,
                              0x55, 0x00, 0x02, 0x00, 0x01, 0x00, 0x26, 0x00, 0x2b, 0x2a, 0x7e, 0x00, 0x56, 0x00,
                              0x02, 0x00, 0x00, 0x21, 0x68, 0x1c, 0xd4, 0x89, 0x65, 0x0f, 0xdc, 0xc7, 0xc7, 0x0e,
                              0xca, 0x8f, 0xa9, 0xbe, 0x44, 0x70, 0x20, 0x10, 0xc7, 0xf0, 0x79, 0x1f, 0xa8, 0x52,
                              0xe2, 0x82, 0x84, 0x7c, 0x9f, 0x4c, 0xe5, 0xc1, 0x94, 0x51};
  // 0004403e000003000a000200010055000200010026002b2a7e00560002000021681cd489650fdcc7c70eca8fa9be44702010c7f0791fa852e282847c9f4ce5c19451

  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);
  ngap_pdu_c               pdu;
  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);

  // Check Fields
  ASSERT_EQ(pdu.type().value, ngap_pdu_c::types_opts::init_msg);
  ASSERT_EQ(pdu.init_msg().proc_code, 4);
  ASSERT_EQ(pdu.init_msg().crit.value, crit_opts::ignore);
  ASSERT_EQ(pdu.init_msg().value.type().value, ngap_elem_procs_o::init_msg_c::types_opts::dl_nas_transport);
  auto& dl_nas = pdu.init_msg().value.dl_nas_transport();
  // Field 0
  // ...
  // Field 1
  ASSERT_EQ(dl_nas->nas_pdu.size(), 42);

  ASSERT_EQ(bref.distance_bytes(), sizeof(ngap_msg));
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);
}

TEST_F(asn1_ngap_test, ul_ran_status_transfer_pack_and_unpack)
{
  const uint8_t ngap_msg[] = {0x00, 0x2e, 0x40, 0x3c, 0x00, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x02, 0x00, 0x01,
                              0x00, 0x55, 0x00, 0x02, 0x00, 0x01, 0x00, 0x26, 0x00, 0x16, 0x15, 0x7e, 0x00,
                              0x57, 0x2d, 0x10, 0x5e, 0x86, 0x21, 0x9e, 0x7d, 0xda, 0x99, 0x95, 0xe3, 0x85,
                              0x03, 0x84, 0xcf, 0xbe, 0xa5, 0x3b, 0x00, 0x79, 0x40, 0x0f, 0x40, 0x00, 0xf1,
                              0x10, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xf1, 0x10, 0x00, 0x00, 0x75};
  // 002e403c000004000a0002000100550002000100260016157e00572d105e86219e7dda9995e3850384cfbea53b0079400f4000f110000000001000f110000075

  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);
  ngap_pdu_c               pdu;
  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);

  // Check Fields
  ASSERT_EQ(pdu.type().value, ngap_pdu_c::types_opts::init_msg);
  ASSERT_EQ(pdu.init_msg().proc_code, 46);
  ASSERT_EQ(pdu.init_msg().crit.value, crit_opts::ignore);
  ASSERT_EQ(pdu.init_msg().value.type().value, ngap_elem_procs_o::init_msg_c::types_opts::ul_nas_transport);
  auto& ul_nas = pdu.init_msg().value.ul_nas_transport();
  // Field 1
  ASSERT_EQ(ul_nas->nas_pdu.size(), 21);

  ASSERT_EQ(bref.distance_bytes(), sizeof(ngap_msg));
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);
}

TEST_F(asn1_ngap_test, ue_context_release_pack_and_unpack)
{
  const uint8_t ngap_msg[] = {0x00, 0x29, 0x00, 0x10, 0x00, 0x00, 0x02, 0x00, 0x72, 0x00,
                              0x04, 0x00, 0x01, 0x00, 0x01, 0x00, 0x0f, 0x40, 0x01, 0x48};
  // 002900100000020072000400010001000f400148

  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);
  ngap_pdu_c               pdu;
  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);

  // Check Fields
  ASSERT_EQ(pdu.type().value, ngap_pdu_c::types_opts::init_msg);
  ASSERT_EQ(pdu.init_msg().proc_code, 41);
  ASSERT_EQ(pdu.init_msg().crit.value, crit_opts::reject);
  ASSERT_EQ(pdu.init_msg().value.type().value, ngap_elem_procs_o::init_msg_c::types_opts::ue_context_release_cmd);

  ASSERT_EQ(bref.distance_bytes(), sizeof(ngap_msg));
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);
}

TEST_F(asn1_ngap_test, ue_context_release_complete_pack_and_unpack)
{
  const uint8_t ngap_msg[] = {
      0x20, 0x29, 0x00, 0x0f, 0x00, 0x00, 0x02, 0x00, 0x0a, 0x40, 0x02, 0x00, 0x01, 0x00, 0x55, 0x40, 0x02, 0x00, 0x01};
  // 2029000f000002000a40020001005540020001

  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);
  ngap_pdu_c               pdu;
  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);

  // Check Fields
  ASSERT_EQ(pdu.type().value, ngap_pdu_c::types_opts::successful_outcome);
  ASSERT_EQ(pdu.successful_outcome().proc_code, 41);
  ASSERT_EQ(pdu.successful_outcome().crit.value, crit_opts::reject);
  ASSERT_EQ(pdu.successful_outcome().value.type().value,
            ngap_elem_procs_o::successful_outcome_c::types_opts::ue_context_release_complete);

  ASSERT_EQ(bref.distance_bytes(), sizeof(ngap_msg));
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);
}

TEST_F(asn1_ngap_test, session_res_setup_request_pack_and_unpack)
{
  const uint8_t ngap_msg[] = {
      0x00, 0x1d, 0x00, 0x6c, 0x00, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x55, 0x00, 0x02, 0x00, 0x01,
      0x00, 0x26, 0x00, 0x2e, 0x2d, 0x7e, 0x00, 0x68, 0x01, 0x00, 0x25, 0x2e, 0x01, 0x00, 0xc2, 0x11, 0x00, 0x06, 0x01,
      0x00, 0x03, 0x30, 0x01, 0x01, 0x06, 0x06, 0x03, 0xe8, 0x06, 0x03, 0xe8, 0x29, 0x05, 0x01, 0xc0, 0xa8, 0x0c, 0x7b,
      0x25, 0x08, 0x07, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x12, 0x01, 0x00, 0x4a, 0x00, 0x27, 0x00, 0x00, 0x01,
      0x00, 0x00, 0x21, 0x00, 0x00, 0x03, 0x00, 0x8b, 0x00, 0x0a, 0x01, 0xf0, 0xc0, 0xa8, 0x11, 0xd2, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x86, 0x00, 0x01, 0x10, 0x00, 0x88, 0x00, 0x07, 0x00, 0x01, 0x00, 0x00, 0x09, 0x00, 0x00};
  // 001d006c000004000a000200010055000200010026002e2d7e00680100252e0100c2110006010003300101060603e80603e8290501c0a80c7b25080764656661756c741201004a0027000001000021000003008b000a01f0c0a811d20000000100860001100088000700010000090000

  const ocudu::byte_buffer byte_pdu = ocudu::byte_buffer::create(ngap_msg).value();
  cbit_ref                 bref(byte_pdu);
  ngap_pdu_c               pdu;
  ASSERT_EQ(pdu.unpack(bref), OCUDUASN_SUCCESS);

  // Check Fields
  ASSERT_EQ(pdu.type().value, ngap_pdu_c::types_opts::init_msg);
  ASSERT_EQ(pdu.init_msg().proc_code, 29);
  ASSERT_EQ(pdu.init_msg().crit.value, crit_opts::reject);
  ASSERT_EQ(pdu.init_msg().value.type().value,
            ngap_elem_procs_o::init_msg_c::types_opts::pdu_session_res_setup_request);
  auto& container = pdu.init_msg().value.pdu_session_res_setup_request();
  ASSERT_EQ(container->pdu_session_res_setup_list_su_req.size(), 1);
  auto& item = container->pdu_session_res_setup_list_su_req[0];
  ASSERT_EQ(item.pdu_session_id, 1);
  ASSERT_EQ(item.s_nssai.sst.to_number(), 0);
  ASSERT_EQ(item.pdu_session_res_setup_request_transfer.to_string(),
            "000003008b000a01f0c0a811d20000000100860001100088000700010000090000");

  cbit_ref                                 bref2(item.pdu_session_res_setup_request_transfer);
  pdu_session_res_setup_request_transfer_s req;
  ASSERT_EQ(req.unpack(bref2), OCUDUASN_SUCCESS);
  ASSERT_EQ(req->ul_ngu_up_tnl_info.type().value, up_transport_layer_info_c::types_opts::gtp_tunnel);
  ASSERT_EQ(req->ul_ngu_up_tnl_info.gtp_tunnel().transport_layer_address.to_string(),
            "11000000101010000001000111010010");

  ASSERT_EQ(bref2.distance_bytes(), (int)item.pdu_session_res_setup_request_transfer.size());
  ASSERT_EQ(test_pack_roundtrip(req), OCUDUASN_SUCCESS);

  ASSERT_EQ(bref.distance_bytes(), sizeof(ngap_msg));
  ASSERT_EQ(test_pack_roundtrip(pdu), OCUDUASN_SUCCESS);
}

TEST_F(asn1_ngap_test, overload_stop_has_an_empty_ie_container)
{
  ngap_pdu_c pdu;
  pdu.set_init_msg().load_info_obj(ASN1_NGAP_ID_OVERLOAD_STOP);

  ocudu::byte_buffer buf;
  bit_ref            bref(buf);
  ASSERT_EQ(pdu.pack(bref), OCUDUASN_SUCCESS);
  ASSERT_EQ(buf, ocudu::make_byte_buffer("00170003000000").value());
  ASSERT_EQ(test_unpack_roundtrip<ngap_pdu_c>(buf), OCUDUASN_SUCCESS);
}

TEST_F(asn1_ngap_test, value_range_union_is_one_root_range)
{
  // ExpectedActivityPeriod and ExpectedIdlePeriod are INTEGER (1..30|40|...|181, ...), whose root is 1..181.
  for (const auto& [period, hex] : {std::pair<uint8_t, const char*>{181, "62d168"},
                                    std::pair<uint8_t, const char*>{40, "609c4e"},
                                    std::pair<uint8_t, const char*>{30, "60743a"}}) {
    expected_ue_activity_behaviour_s behaviour;
    behaviour.expected_activity_period_present = true;
    behaviour.expected_activity_period         = period;
    behaviour.expected_idle_period_present     = true;
    behaviour.expected_idle_period             = period;

    ocudu::byte_buffer buf;
    bit_ref            bref(buf);
    ASSERT_EQ(behaviour.pack(bref), OCUDUASN_SUCCESS);
    ASSERT_EQ(buf, ocudu::make_byte_buffer(hex).value()) << (unsigned)period;
    ASSERT_EQ(test_unpack_roundtrip<expected_ue_activity_behaviour_s>(buf), OCUDUASN_SUCCESS);
  }
}

// Real PDUs from lab captures.
static const std::vector<capture_pdu> ngap_captures = {
    {"DownlinkNASTransport",
     "00044047000003000a00032004de005500034004de00260032317e02ba58bbfb057951d39e1c5b66740d12d055d16016"
     "986ef0f1d2f1d8e2ab282340f69439f6a8e705338ab2bb22da341c"},
    {"ErrorIndication [Cause: RadioNetwork=interaction-with-other-procedure]",
     "00094017000003000a400320015500554003400155000f40020640"},
    {"InitialContextSetupRequest",
     "000e0080da00000c000a0002006400550002000000124018096cbe0000f110000007000101184002956c016d40021768"
     "001c00070000f1108001010000000502010000010077000918000c000000000000005e002055bd06a5c3b3ee82b6090d"
     "e6b71e2f30001d691bcab7516d52f01bc171c895fa002440040000f110002240080123456700ffff0100264048477e02"
     "d4227c0e017e0042010977000bf200f110800101be0d956c54070000f11000000715050401000001310a040100000104"
     "0200002a21020f005e01be3408031f19f1031f11f2005b4001000092400100"},
    {"InitialUEMessage, Registration request [RRCEstablishmentCause=mo-Signalling]",
     "000f407b0000060055000340077a00260045447e0181813156027e004109000bf200f110f511b2c00003f32e02e0e071"
     "0025d1b483df33730a5a596c4f22825e51c5f199e74c2657f9a9238dcc050164fa94389c3a37dd0079000f4000f11000"
     "0004000000f110000001005a400118001a0007046c80c00003f30070400100"},
    {"NGSetupRequest",
     "00150036000004001b00090000f110500000019b0052400903006f6375647530310066001000000000070000f1100000"
     "10080000010015400160"},
    {"Paging", "0018401900000200734007023640c0000001006740070000f110000001"},
    {"PDUSessionResourceSetupRequest",
     "001d0080d9000004000a00020064005500020000004a0080b6004001767e0213d95907037e00680100672e0101c21100"
     "0901000631300101ff010606271004b71b290501c0a80302220401000001817900060120410101097b00188080210a03"
     "00000a810608080808000d0408080808001100251c08696e7465726e6574066d6e63303031066d636330303104677072"
     "7312014020000001360000050082000b1002540be40030b2d05e00008b000a01f0ac1400482d36ef410086000100008a"
     "000209000088000700010000093800006e400b1002540be40030b2d05e00"},
    {"UEContextReleaseCommand [Cause: RadioNetwork=radio-connection-with-ue-lost]",
     "0029001300000200720006020105400105000f40020540"},
    {"UEContextReleaseRequest [Cause: RadioNetwork=radio-connection-with-ue-lost]",
     "002a401e000004000a00032001050055000340010500850003000001000f40020540"},
    {"UERadioCapabilityInfoIndication",
     "002c40809a000003000a000200640055000200000075408086808404040887fc1a033a0474e5a02dfbd8387200001609"
     "317000183f0d000c6ffc005c00a62e26c800000864000356830001801e81c790e7c000082e007fe003801ff800544000"
     "006cbe000be00000100389a00000000404d2f00000007008091f000000000000052aa0002000012cb28007954147c000"
     "02d003c00040000001002004090280"},
    {"UplinkNASTransport",
     "002e406d000004000a0002006400550002000000260047467e02f349ef1f027e006701002b2e0101c1ffff91b17b0020"
     "8080211001000010810600000000830600000000000a00001000000d00001100120181250908696e7465726e65740079"
     "400f4000f1100000019b0000f110000007"},
    {"InitialContextSetupResponse",
     "200e0026000003000a400320038e0055400340038e004840110000010d0003e0ac150183000003440005"},
    {"NGSetupResponse",
     "2015005d000004000100311700616d617269736f66742e616d662e3567632e6d6e633030312e6d63633030312e336770"
     "706e6574776f726b2e6f726700600008000000f1108001010056400132005000100000f1100001100800000110100000"
     "2a"},
    {"PDUSessionResourceSetupResponse",
     "201d0026000003000a400320010000554003400100004b40110000010d0003e0ac150183000001010005"},
    {"UEContextReleaseComplete",
     "2029002b000004000a4003200100005540034001000079400f4000f110000004000000f110000001003c0003000001"},
    {"InitialContextSetupFailure [Cause: RadioNetwork=no-radio-resources-available-in-target-cell] [Cause: "
     "Protocol=unspecified]",
     "400e0020000004000a40032004620055400340046200844006000001020068000f40016c"},
};

TEST_F(asn1_ngap_test, captured_pdus_repack_to_same_bytes)
{
  for (const capture_pdu& c : ngap_captures) {
    EXPECT_EQ(test_unpack_roundtrip<ngap_pdu_c>(hex_to_bytes(c.hex)), OCUDUASN_SUCCESS) << c.name;
  }
}
