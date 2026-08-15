// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/ru/ofh_ru_message_receiver.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

namespace {

class vlan_frame_decoder_spy : public ether::vlan_frame_decoder
{
  std::vector<uint8_t> pdu = {0xaa};

public:
  size_t                                last_frame_size = 0;
  uint16_t                              eth_type        = ether::ECPRI_ETH_TYPE;
  ether::mac_address                    src_mac         = {};
  ether::mac_address                    dst_mac         = {};
  std::optional<ether::vlan_parameters> vlan_config     = ether::vlan_parameters{.tci_vid = 1, .tci_pcp = 0};

  span<const uint8_t> decode(span<const uint8_t> frame, ether::vlan_frame_params& eth_params) override
  {
    last_frame_size            = frame.size();
    eth_params.eth_type        = eth_type;
    eth_params.mac_src_address = src_mac;
    eth_params.mac_dst_address = dst_mac;
    eth_params.vlan_config     = vlan_config;
    return pdu;
  }
};

class packet_decoder_spy : public ecpri::packet_decoder
{
  ecpri::packet_parameters params;
  std::vector<uint8_t>     ofh_pdu = {0xbb};

public:
  void set_params(const ecpri::packet_parameters& p) { params = p; }
  void set_ofh_pdu(std::vector<uint8_t> pdu) { ofh_pdu = std::move(pdu); }

  span<const uint8_t> decode(span<const uint8_t>, ecpri::packet_parameters& out_params) override
  {
    out_params = params;
    return ofh_pdu;
  }
};

class rx_message_handler_spy : public ru_rx_message_handler
{
public:
  bool     called = false;
  unsigned eaxc   = 0;
  void     decode_message(unsigned eaxc_, span<const uint8_t>) override
  {
    called = true;
    eaxc   = eaxc_;
  }
};

/// Sequence-id checker spy returning a configured comparison result.
class seq_id_checker_spy : public sequence_id_checker
{
  int result = 0;

public:
  unsigned nof_updates = 0;
  unsigned last_eaxc   = 0;
  uint8_t  last_seq_id = 0;

  void set_result(int r) { result = r; }
  int  update_and_compare_seq_id(unsigned eaxc, uint8_t seq_id) override
  {
    ++nof_updates;
    last_eaxc   = eaxc;
    last_seq_id = seq_id;
    return result;
  }
};

ru_message_receiver make_receiver(packet_decoder_spy*&     ecpri_out,
                                  rx_message_handler_spy&  uplane,
                                  rx_message_handler_spy&  cplane,
                                  sequence_id_checker*     uplane_seq_id_checker    = nullptr,
                                  vlan_frame_decoder_spy** eth_out                  = nullptr,
                                  sequence_id_checker*     cplane_dl_seq_id_checker = nullptr,
                                  sequence_id_checker*     cplane_ul_seq_id_checker = nullptr)
{
  auto ecpri = std::make_unique<packet_decoder_spy>();
  ecpri_out  = ecpri.get();

  auto eth = std::make_unique<vlan_frame_decoder_spy>();
  if (eth_out) {
    *eth_out = eth.get();
  }

  ru_message_receiver_config config;
  config.sector                      = 0;
  config.vlan_params.eth_type        = ether::ECPRI_ETH_TYPE;
  config.vlan_params.mac_src_address = {};
  config.vlan_params.mac_dst_address = {};
  config.vlan_params.vlan_config     = ether::vlan_parameters{.tci_vid = 1, .tci_pcp = 0};
  config.dl_eaxc                     = {7};

  ru_message_receiver_dependencies dependencies;
  dependencies.logger                   = &ocudulog::fetch_basic_logger("TEST");
  dependencies.eth_decoder              = std::move(eth);
  dependencies.ecpri_decoder            = std::move(ecpri);
  dependencies.uplane_handler           = &uplane;
  dependencies.cplane_handler           = &cplane;
  dependencies.uplane_seq_id_checker    = uplane_seq_id_checker;
  dependencies.cplane_dl_seq_id_checker = cplane_dl_seq_id_checker;
  dependencies.cplane_ul_seq_id_checker = cplane_ul_seq_id_checker;

  return ru_message_receiver(config, std::move(dependencies));
}

} // namespace

TEST(ru_message_receiver_test, iq_data_frame_routed_to_uplane_handler)
{
  packet_decoder_spy*    ecpri = nullptr;
  rx_message_handler_spy uplane;
  rx_message_handler_spy cplane;
  ru_message_receiver    receiver = make_receiver(ecpri, uplane, cplane);

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::iq_data;
  params.type_params     = ecpri::iq_data_parameters{/*pc_id=*/7, /*seq_id=*/0};
  ecpri->set_params(params);

  const uint8_t frame[] = {0x00};
  receiver.process_frame(frame);

  ASSERT_TRUE(uplane.called);
  ASSERT_EQ(7, uplane.eaxc);
  ASSERT_FALSE(cplane.called);
}

TEST(ru_message_receiver_test, rt_control_frame_routed_to_cplane_handler)
{
  packet_decoder_spy*    ecpri = nullptr;
  rx_message_handler_spy uplane;
  rx_message_handler_spy cplane;
  ru_message_receiver    receiver = make_receiver(ecpri, uplane, cplane);

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0};
  ecpri->set_params(params);

  const uint8_t frame[] = {0x00};
  receiver.process_frame(frame);

  ASSERT_TRUE(cplane.called);
  ASSERT_EQ(9, cplane.eaxc);
  ASSERT_FALSE(uplane.called);
}

TEST(ru_message_receiver_test, short_frame_is_padded_to_the_minimum_ethernet_length)
{
  packet_decoder_spy*     ecpri = nullptr;
  vlan_frame_decoder_spy* eth   = nullptr;
  rx_message_handler_spy  uplane;
  rx_message_handler_spy  cplane;
  ru_message_receiver     receiver = make_receiver(ecpri, uplane, cplane, nullptr, &eth);

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0};
  ecpri->set_params(params);

  // A minimum-size Control-Plane frame delivered without the FCS (60 bytes) must be padded back up to the 64-byte
  // minimum the Ethernet decoder enforces, and still be processed.
  std::vector<uint8_t> frame(60, 0x11);
  receiver.process_frame(frame);

  ASSERT_EQ(eth->last_frame_size, 64U);
  ASSERT_TRUE(cplane.called);
}

TEST(ru_message_receiver_test, frame_with_unexpected_source_mac_is_dropped)
{
  packet_decoder_spy*     ecpri = nullptr;
  vlan_frame_decoder_spy* eth   = nullptr;
  rx_message_handler_spy  uplane;
  rx_message_handler_spy  cplane;
  ru_message_receiver     receiver = make_receiver(ecpri, uplane, cplane, nullptr, &eth);

  // A frame whose source is not the O-DU (e.g. the O-RU's own transmission observed on a shared interface).
  eth->src_mac = {0x0e, 0x42, 0xa3, 0xef, 0xfd, 0x71};

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0};
  ecpri->set_params(params);

  std::vector<uint8_t> frame(100, 0x11);
  receiver.process_frame(frame);

  ASSERT_FALSE(cplane.called);
  ASSERT_FALSE(uplane.called);
}

TEST(ru_message_receiver_test, frame_with_unexpected_destination_mac_is_dropped)
{
  packet_decoder_spy*     ecpri = nullptr;
  vlan_frame_decoder_spy* eth   = nullptr;
  rx_message_handler_spy  uplane;
  rx_message_handler_spy  cplane;
  ru_message_receiver     receiver = make_receiver(ecpri, uplane, cplane, nullptr, &eth);

  // A frame addressed to a different O-RU must not reach either Open Fronthaul data flow.
  eth->dst_mac = {0x0e, 0x42, 0xa3, 0xef, 0xfd, 0x72};

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0};
  ecpri->set_params(params);

  std::vector<uint8_t> frame(100, 0x11);
  receiver.process_frame(frame);

  ASSERT_FALSE(cplane.called);
  ASSERT_FALSE(uplane.called);
}

TEST(ru_message_receiver_test, frame_with_unexpected_vlan_is_dropped)
{
  packet_decoder_spy*     ecpri = nullptr;
  vlan_frame_decoder_spy* eth   = nullptr;
  rx_message_handler_spy  uplane;
  rx_message_handler_spy  cplane;
  ru_message_receiver     receiver = make_receiver(ecpri, uplane, cplane, nullptr, &eth);

  eth->vlan_config = ether::vlan_parameters{.tci_vid = 2, .tci_pcp = 0};

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0};
  ecpri->set_params(params);

  std::vector<uint8_t> frame(100, 0x11);
  receiver.process_frame(frame);

  ASSERT_FALSE(cplane.called);
  ASSERT_FALSE(uplane.called);
}

TEST(ru_message_receiver_test, untagged_frame_is_accepted_when_vlan_is_configured)
{
  packet_decoder_spy*     ecpri = nullptr;
  vlan_frame_decoder_spy* eth   = nullptr;
  rx_message_handler_spy  uplane;
  rx_message_handler_spy  cplane;
  ru_message_receiver     receiver = make_receiver(ecpri, uplane, cplane, nullptr, &eth);

  eth->vlan_config.reset();

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0};
  ecpri->set_params(params);

  std::vector<uint8_t> frame(100, 0x11);
  receiver.process_frame(frame);

  ASSERT_TRUE(cplane.called);
  ASSERT_FALSE(uplane.called);
}

TEST(ru_message_receiver_test, frame_with_different_vlan_pcp_is_accepted)
{
  packet_decoder_spy*     ecpri = nullptr;
  vlan_frame_decoder_spy* eth   = nullptr;
  rx_message_handler_spy  uplane;
  rx_message_handler_spy  cplane;
  ru_message_receiver     receiver = make_receiver(ecpri, uplane, cplane, nullptr, &eth);

  eth->vlan_config = ether::vlan_parameters{.tci_vid = 1, .tci_pcp = 5};

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0};
  ecpri->set_params(params);

  std::vector<uint8_t> frame(100, 0x11);
  receiver.process_frame(frame);

  ASSERT_TRUE(cplane.called);
  ASSERT_FALSE(uplane.called);
}

TEST(ru_message_receiver_test, iq_data_with_unconfigured_eaxc_is_dropped)
{
  packet_decoder_spy*    ecpri = nullptr;
  rx_message_handler_spy uplane;
  rx_message_handler_spy cplane;
  ru_message_receiver    receiver = make_receiver(ecpri, uplane, cplane);

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::iq_data;
  params.type_params     = ecpri::iq_data_parameters{/*pc_id=*/3, /*seq_id=*/0};
  ecpri->set_params(params);

  std::vector<uint8_t> frame(100, 0x11);
  receiver.process_frame(frame);

  ASSERT_FALSE(uplane.called);
}

TEST(ru_message_receiver_test, non_ecpri_ethertype_is_dropped)
{
  packet_decoder_spy*     ecpri = nullptr;
  vlan_frame_decoder_spy* eth   = nullptr;
  rx_message_handler_spy  uplane;
  rx_message_handler_spy  cplane;
  ru_message_receiver     receiver = make_receiver(ecpri, uplane, cplane, nullptr, &eth);

  eth->eth_type = 0x0806; // ARP

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0};
  ecpri->set_params(params);

  std::vector<uint8_t> frame(100, 0x11);
  receiver.process_frame(frame);

  ASSERT_FALSE(cplane.called);
  ASSERT_FALSE(uplane.called);
}

TEST(ru_message_receiver_test, cplane_sequence_ids_are_checked_per_data_direction)
{
  packet_decoder_spy*    ecpri = nullptr;
  rx_message_handler_spy uplane;
  rx_message_handler_spy cplane;
  seq_id_checker_spy     dl_checker;
  seq_id_checker_spy     ul_checker;
  // The downlink Control-Plane stream is behind and the uplink stream is in order. The O-DU generates them from
  // independent counters, so each message must be judged against its direction's checker without being discarded.
  dl_checker.set_result(-1);
  ul_checker.set_result(0);
  ru_message_receiver receiver = make_receiver(ecpri, uplane, cplane, nullptr, nullptr, &dl_checker, &ul_checker);

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::rt_control_data;
  params.type_params     = ecpri::realtime_control_parameters{/*rtc_id=*/9, /*seq_id=*/0x2a80};
  ecpri->set_params(params);

  // Downlink Control-Plane (data direction bit set): judged by the downlink checker and accepted despite being old.
  ecpri->set_ofh_pdu({0x90});
  const uint8_t frame[] = {0x00};
  receiver.process_frame(frame);
  ASSERT_TRUE(cplane.called);
  ASSERT_EQ(dl_checker.nof_updates, 1U);
  ASSERT_EQ(dl_checker.last_eaxc, 9U);
  ASSERT_EQ(dl_checker.last_seq_id, 0x2aU);
  ASSERT_EQ(ul_checker.nof_updates, 0U);

  // Uplink Control-Plane (data direction bit clear): judged by the uplink checker and accepted.
  cplane.called = false;
  ecpri->set_ofh_pdu({0x10});
  receiver.process_frame(frame);
  ASSERT_TRUE(cplane.called);
  ASSERT_EQ(dl_checker.nof_updates, 1U);
  ASSERT_EQ(ul_checker.nof_updates, 1U);
  ASSERT_EQ(ul_checker.last_eaxc, 9U);
  ASSERT_EQ(ul_checker.last_seq_id, 0x2aU);
}

TEST(ru_message_receiver_test, iq_data_with_past_sequence_id_is_checked_and_accepted)
{
  packet_decoder_spy*    ecpri = nullptr;
  rx_message_handler_spy uplane;
  rx_message_handler_spy cplane;
  seq_id_checker_spy     seq_checker;
  seq_checker.set_result(-1); // sequence id from the past
  ru_message_receiver receiver = make_receiver(ecpri, uplane, cplane, &seq_checker);

  ecpri::packet_parameters params;
  params.header.msg_type = ecpri::message_type::iq_data;
  params.type_params     = ecpri::iq_data_parameters{/*pc_id=*/7, /*seq_id=*/0x5380};
  ecpri->set_params(params);

  const uint8_t frame[] = {0x00};
  receiver.process_frame(frame);

  ASSERT_TRUE(uplane.called);
  ASSERT_EQ(seq_checker.nof_updates, 1U);
  ASSERT_EQ(seq_checker.last_eaxc, 7U);
  ASSERT_EQ(seq_checker.last_seq_id, 0x53U);
}
