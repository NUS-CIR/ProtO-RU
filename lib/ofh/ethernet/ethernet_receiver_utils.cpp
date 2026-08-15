// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ethernet_receiver_utils.h"
#include "ethernet_constants.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include <cstring>
#include <linux/filter.h>
#include <linux/if_packet.h>
#include <sys/socket.h>

using namespace ocudu;
using namespace ether;

bool ocudu::ether::attach_ecpri_rx_filter(int socket_fd)
{
  // Accept an untagged eCPRI frame or an 802.1Q frame whose inner EtherType is eCPRI. Out-of-bounds loads reject
  // truncated frames automatically.
  ::sock_filter filter[] = {
      BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 12),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, ECPRI_ETH_TYPE, 3, 0),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, VLAN_TPID, 0, 3),
      BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 16),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, ECPRI_ETH_TYPE, 0, 1),
      BPF_STMT(BPF_RET | BPF_K, 0xffffffffU),
      BPF_STMT(BPF_RET | BPF_K, 0),
  };
  const ::sock_fprog program = {.len    = static_cast<unsigned short>(sizeof(filter) / sizeof(filter[0])),
                                .filter = filter};

  return ::setsockopt(socket_fd, SOL_SOCKET, SO_ATTACH_FILTER, &program, sizeof(program)) == 0;
}

std::optional<rx_vlan_metadata> ocudu::ether::extract_rx_vlan_metadata(::msghdr& message)
{
  for (::cmsghdr* cmsg = CMSG_FIRSTHDR(&message); cmsg != nullptr; cmsg = CMSG_NXTHDR(&message, cmsg)) {
    if (cmsg->cmsg_level != SOL_PACKET || cmsg->cmsg_type != PACKET_AUXDATA ||
        cmsg->cmsg_len < CMSG_LEN(sizeof(::tpacket_auxdata))) {
      continue;
    }

    const auto* auxdata = reinterpret_cast<const ::tpacket_auxdata*>(CMSG_DATA(cmsg));
    if ((auxdata->tp_status & TP_STATUS_VLAN_VALID) == 0) {
      return std::nullopt;
    }

    uint16_t vlan_tpid = VLAN_TPID;
#ifdef TP_STATUS_VLAN_TPID_VALID
    if ((auxdata->tp_status & TP_STATUS_VLAN_TPID_VALID) != 0) {
      vlan_tpid = auxdata->tp_vlan_tpid;
    }
#endif
    return rx_vlan_metadata{.tci = auxdata->tp_vlan_tci, .tpid = vlan_tpid};
  }

  return std::nullopt;
}

rx_frame_normalization_result ocudu::ether::normalize_rx_frame(span<uint8_t>                   storage,
                                                               std::size_t&                    frame_size,
                                                               std::optional<rx_vlan_metadata> vlan_metadata)
{
  if (frame_size > storage.size()) {
    return rx_frame_normalization_result::insufficient_storage;
  }

  if (!vlan_metadata.has_value()) {
    return rx_frame_normalization_result::success;
  }

  if (frame_size < ETH_HEADER_SIZE.value()) {
    return rx_frame_normalization_result::malformed_frame;
  }

  const uint16_t outer_type = (uint16_t(storage[12]) << 8U) | storage[13];
  if (outer_type == VLAN_TPID) {
    return frame_size < (ETH_HEADER_SIZE + ETH_VLAN_TAG_SIZE).value() ? rx_frame_normalization_result::malformed_frame
                                                                      : rx_frame_normalization_result::success;
  }

  if (frame_size + ETH_VLAN_TAG_SIZE.value() > storage.size()) {
    return rx_frame_normalization_result::insufficient_storage;
  }

  std::memmove(storage.data() + 16, storage.data() + 12, frame_size - 12);
  storage[12] = vlan_metadata->tpid >> 8U;
  storage[13] = vlan_metadata->tpid & 0xffU;
  storage[14] = vlan_metadata->tci >> 8U;
  storage[15] = vlan_metadata->tci & 0xffU;
  frame_size += ETH_VLAN_TAG_SIZE.value();

  return rx_frame_normalization_result::success;
}
