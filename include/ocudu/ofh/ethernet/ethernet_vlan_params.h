// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include <stdint.h>

namespace ocudu {
namespace ether {

/// VLAN Ethernet configuration parameters.
struct vlan_parameters {
  /// Tag control information VLAN identifier field.
  uint16_t tci_vid;
  /// Tag control information Priority code point (PCP) field.
  uint8_t tci_pcp = 0;
};

constexpr bool operator==(const vlan_parameters& lhs, const vlan_parameters& rhs)
{
  return lhs.tci_vid == rhs.tci_vid && lhs.tci_pcp == rhs.tci_pcp;
}

constexpr bool operator!=(const vlan_parameters& lhs, const vlan_parameters& rhs)
{
  return !(lhs == rhs);
}

} // namespace ether
} // namespace ocudu
