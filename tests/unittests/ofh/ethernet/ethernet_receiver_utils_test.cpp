// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ethernet_constants.h"
#include "ethernet_receiver_utils.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include <array>
#include <cerrno>
#include <cstring>
#include <gtest/gtest.h>
#include <linux/if_packet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

using namespace ocudu;
using namespace ether;

namespace {

std::vector<uint8_t> make_frame(uint16_t outer_type, std::optional<uint16_t> inner_type = std::nullopt)
{
  std::vector<uint8_t> frame(64, 0);
  frame[12] = outer_type >> 8U;
  frame[13] = outer_type & 0xffU;
  if (inner_type.has_value()) {
    frame[16] = *inner_type >> 8U;
    frame[17] = *inner_type & 0xffU;
  }
  return frame;
}

bool filter_accepts(span<const uint8_t> frame)
{
  std::array<int, 2> sockets{};
  EXPECT_EQ(::socketpair(AF_UNIX, SOCK_DGRAM, 0, sockets.data()), 0);
  EXPECT_TRUE(attach_ecpri_rx_filter(sockets[1])) << ::strerror(errno);
  EXPECT_EQ(::send(sockets[0], frame.data(), frame.size(), 0), static_cast<ssize_t>(frame.size())) << ::strerror(errno);

  std::array<uint8_t, 128> received{};
  const ssize_t            result = ::recv(sockets[1], received.data(), received.size(), MSG_DONTWAIT);
  const int                error  = errno;
  ::close(sockets[0]);
  ::close(sockets[1]);
  EXPECT_TRUE(result >= 0 || error == EAGAIN || error == EWOULDBLOCK);
  return result >= 0;
}

} // namespace

TEST(ethernet_receiver_filter_test, accepts_only_ecpri_frames)
{
  EXPECT_TRUE(filter_accepts(make_frame(ECPRI_ETH_TYPE)));
  EXPECT_TRUE(filter_accepts(make_frame(VLAN_TPID, ECPRI_ETH_TYPE)));
  EXPECT_FALSE(filter_accepts(make_frame(0x0800)));
  EXPECT_FALSE(filter_accepts(make_frame(VLAN_TPID, 0x0800)));
  EXPECT_FALSE(filter_accepts(std::vector<uint8_t>(13, 0)));
}

TEST(ethernet_rx_frame_normalizer_test, leaves_untagged_frame_unchanged_without_metadata)
{
  std::vector<uint8_t> storage    = make_frame(ECPRI_ETH_TYPE);
  const auto           original   = storage;
  std::size_t          frame_size = storage.size();

  ASSERT_EQ(normalize_rx_frame(storage, frame_size, std::nullopt), rx_frame_normalization_result::success);
  EXPECT_EQ(frame_size, original.size());
  EXPECT_EQ(storage, original);
}

TEST(ethernet_rx_vlan_metadata_test, extracts_packet_auxdata)
{
  alignas(::cmsghdr) std::array<uint8_t, CMSG_SPACE(sizeof(::tpacket_auxdata))> control{};
  ::msghdr                                                                      message = {};
  message.msg_control                                                                   = control.data();
  message.msg_controllen                                                                = control.size();

  ::cmsghdr* cmsg = CMSG_FIRSTHDR(&message);
  ASSERT_NE(cmsg, nullptr);
  cmsg->cmsg_level = SOL_PACKET;
  cmsg->cmsg_type  = PACKET_AUXDATA;
  cmsg->cmsg_len   = CMSG_LEN(sizeof(::tpacket_auxdata));

  auto* auxdata        = reinterpret_cast<::tpacket_auxdata*>(CMSG_DATA(cmsg));
  auxdata->tp_status   = TP_STATUS_VLAN_VALID;
  auxdata->tp_vlan_tci = 0x607b;

  auto metadata = extract_rx_vlan_metadata(message);
  ASSERT_TRUE(metadata.has_value());
  EXPECT_EQ(metadata->tci, 0x607b);
  EXPECT_EQ(metadata->tpid, VLAN_TPID);

  auxdata->tp_status = 0;
  EXPECT_FALSE(extract_rx_vlan_metadata(message).has_value());
}

TEST(ethernet_rx_frame_normalizer_test, leaves_an_intact_vlan_header_unchanged)
{
  std::vector<uint8_t> storage = make_frame(VLAN_TPID, ECPRI_ETH_TYPE);
  storage[14]                  = 0x60;
  storage[15]                  = 0x7b;
  const auto  original         = storage;
  std::size_t frame_size       = storage.size();

  ASSERT_EQ(normalize_rx_frame(storage, frame_size, rx_vlan_metadata{.tci = 0x1234, .tpid = VLAN_TPID}),
            rx_frame_normalization_result::success);
  EXPECT_EQ(frame_size, original.size());
  EXPECT_EQ(storage, original);
}

TEST(ethernet_rx_frame_normalizer_test, restores_a_vlan_header_from_metadata)
{
  std::vector<uint8_t> storage(68, 0);
  const auto           frame = make_frame(ECPRI_ETH_TYPE);
  std::copy(frame.begin(), frame.end(), storage.begin());
  std::size_t frame_size = frame.size();

  ASSERT_EQ(normalize_rx_frame(storage, frame_size, rx_vlan_metadata{.tci = 0x607b, .tpid = VLAN_TPID}),
            rx_frame_normalization_result::success);
  EXPECT_EQ(frame_size, frame.size() + ETH_VLAN_TAG_SIZE.value());
  EXPECT_EQ((span<const uint8_t>(storage).first(12)), (span<const uint8_t>(frame).first(12)));
  EXPECT_EQ(storage[12], 0x81);
  EXPECT_EQ(storage[13], 0x00);
  EXPECT_EQ(storage[14], 0x60);
  EXPECT_EQ(storage[15], 0x7b);
  EXPECT_EQ(storage[16], 0xae);
  EXPECT_EQ(storage[17], 0xfe);
  EXPECT_EQ((span<const uint8_t>(storage).subspan(18, frame.size() - 14)),
            (span<const uint8_t>(frame).subspan(14, frame.size() - 14)));
}

TEST(ethernet_rx_frame_normalizer_test, rejects_malformed_or_oversized_frames)
{
  std::array<uint8_t, 13> short_storage{};
  std::size_t             short_size = short_storage.size();
  EXPECT_EQ(normalize_rx_frame(short_storage, short_size, rx_vlan_metadata{.tci = 1, .tpid = VLAN_TPID}),
            rx_frame_normalization_result::malformed_frame);

  std::array<uint8_t, 14> truncated_vlan_storage{};
  truncated_vlan_storage[12]      = VLAN_TPID >> 8U;
  truncated_vlan_storage[13]      = VLAN_TPID & 0xffU;
  std::size_t truncated_vlan_size = truncated_vlan_storage.size();
  EXPECT_EQ(
      normalize_rx_frame(truncated_vlan_storage, truncated_vlan_size, rx_vlan_metadata{.tci = 1, .tpid = VLAN_TPID}),
      rx_frame_normalization_result::malformed_frame);

  std::array<uint8_t, 64> full_storage{};
  full_storage[12]      = ECPRI_ETH_TYPE >> 8U;
  full_storage[13]      = ECPRI_ETH_TYPE & 0xffU;
  std::size_t full_size = full_storage.size();
  EXPECT_EQ(normalize_rx_frame(full_storage, full_size, rx_vlan_metadata{.tci = 1, .tpid = VLAN_TPID}),
            rx_frame_normalization_result::insufficient_storage);

  std::size_t impossible_size = full_storage.size() + 1;
  EXPECT_EQ(normalize_rx_frame(full_storage, impossible_size, std::nullopt),
            rx_frame_normalization_result::insufficient_storage);
}
