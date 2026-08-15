// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/span.h"
#include <cstddef>
#include <cstdint>
#include <optional>

struct msghdr;

namespace ocudu {
namespace ether {

/// VLAN information reported out-of-band by the Linux packet socket.
struct rx_vlan_metadata {
  uint16_t tci;
  uint16_t tpid;
};

enum class rx_frame_normalization_result { success, malformed_frame, insufficient_storage };

/// Attaches a kernel filter that accepts only untagged eCPRI and single-tagged 802.1Q eCPRI frames.
bool attach_ecpri_rx_filter(int socket_fd);

/// Extracts VLAN information reported through PACKET_AUXDATA control data.
std::optional<rx_vlan_metadata> extract_rx_vlan_metadata(::msghdr& message);

/// Restores a VLAN header removed by the NIC or Linux networking stack.
rx_frame_normalization_result
normalize_rx_frame(span<uint8_t> storage, std::size_t& frame_size, std::optional<rx_vlan_metadata> vlan_metadata);

} // namespace ether
} // namespace ocudu
