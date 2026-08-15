// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/ethernet/ethernet_factories.h"
#include <algorithm>
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ether;

TEST(vlan_ethernet_frame_decoder_impl_test, decode_valid_vlan_ethernet_frame_should_pass)
{
  std::vector<uint8_t> packet = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x80, 0x61, 0x5f, 0x0d, 0xdf, 0xaa, 0x81, 0x00, 0x60, 0x7b, 0xaa, 0xbb, 0x66,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  std::unique_ptr<vlan_frame_decoder> decoder = create_vlan_frame_decoder(ocudulog::fetch_basic_logger("TEST"), 0);

  vlan_frame_params params;
  auto              payload = decoder->decode(packet, params);
  ASSERT_TRUE((params.mac_src_address == mac_address{0x80, 0x61, 0x5f, 0x0d, 0xdf, 0xaa}));
  ASSERT_TRUE((params.mac_dst_address == mac_address{0x00, 0x11, 0x22, 0x33, 0x44, 0x55}));
  ASSERT_EQ(params.vlan_config, (vlan_parameters{.tci_vid = 123, .tci_pcp = 3}));
  ASSERT_EQ(params.eth_type, 0xaabb);

  constexpr unsigned HEADER_SIZE = 18;
  ASSERT_EQ(payload.size(), packet.size() - HEADER_SIZE);
  ASSERT_EQ(payload[0], 0x66);
}

TEST(vlan_ethernet_frame_decoder_impl_test, decode_valid_untagged_ethernet_frame_should_pass)
{
  std::vector<uint8_t> packet(64, 0);
  const mac_address    dst_mac{0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
  const mac_address    src_mac{0x80, 0x61, 0x5f, 0x0d, 0xdf, 0xaa};
  std::copy(dst_mac.begin(), dst_mac.end(), packet.begin());
  std::copy(src_mac.begin(), src_mac.end(), packet.begin() + 6);
  packet[12] = 0xaa;
  packet[13] = 0xbb;
  packet[14] = 0x66;

  std::unique_ptr<vlan_frame_decoder> decoder = create_vlan_frame_decoder(ocudulog::fetch_basic_logger("TEST"), 0);

  vlan_frame_params params;
  params.vlan_config = vlan_parameters{.tci_vid = 999};
  auto payload       = decoder->decode(packet, params);

  ASSERT_FALSE(params.vlan_config.has_value());
  ASSERT_EQ(params.eth_type, 0xaabb);
  ASSERT_EQ(payload.size(), packet.size() - 14);
  ASSERT_EQ(payload[0], 0x66);
}

TEST(vlan_ethernet_frame_decoder_impl_test, decode_small_vlan_ethernet_frame_should_fail)
{
  std::vector<uint8_t> packet = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x80, 0x61, 0x5f, 0x0d,
                                 0xdf, 0xaa, 0xaa, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  std::unique_ptr<vlan_frame_decoder> decoder = create_vlan_frame_decoder(ocudulog::fetch_basic_logger("TEST"), 0);

  vlan_frame_params params;
  auto              payload = decoder->decode(packet, params);

  ASSERT_TRUE(payload.empty());
}
