// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "vlan_ethernet_frame_decoder_impl.h"
#include "../support/network_order_binary_deserializer.h"
#include "ethernet_constants.h"

using namespace ocudu;
using namespace ether;

static constexpr unsigned MIN_ETH_LEN = 64;

span<const uint8_t> vlan_frame_decoder_impl::decode(span<const uint8_t> frame, vlan_frame_params& eth_params)
{
  // Ethernet frames should include padding bytes up to the minimum length.
  if (OCUDU_UNLIKELY(frame.size() < MIN_ETH_LEN)) {
    logger.debug("Sector #{}: Dropped received Ethernet frame of size '{}' bytes as it is below the minimum allowed "
                 "size of '{}' bytes",
                 sector_id,
                 frame.size(),
                 MIN_ETH_LEN);
    return {};
  }

  ofh::network_order_binary_deserializer deserializer(frame);

  deserializer.read(eth_params.mac_dst_address);
  deserializer.read(eth_params.mac_src_address);

  eth_params.vlan_config.reset();
  eth_params.eth_type = deserializer.read<uint16_t>();

  if (eth_params.eth_type == VLAN_TPID) {
    const uint16_t tci     = deserializer.read<uint16_t>();
    eth_params.vlan_config = vlan_parameters{.tci_vid = static_cast<uint16_t>(tci & 0x0fffU),
                                             .tci_pcp = static_cast<uint8_t>((tci >> 13U) & 0x07U)};
    eth_params.eth_type    = deserializer.read<uint16_t>();
  }

  return frame.last(frame.size() - deserializer.get_offset());
}
