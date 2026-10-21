// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ofh/ethernet/ethernet_unique_buffer.h"
#include "ocudu/support/ocudu_assert.h"
#include <rte_ethdev.h>
#include <utility>

namespace ocudu {
namespace ether {

/// Receive buffer wrapper dedicated for DPDK Ethernet receiver.
class dpdk_rx_buffer_impl : public rx_buffer
{
public:
  explicit dpdk_rx_buffer_impl(::rte_mbuf* mbuf_) : mbuf(mbuf_)
  {
    ocudu_assert(mbuf, "Invalid DPDK mbuf was passed to dpdk_rx_buffer_impl");

    // Some PMDs (e.g., net_memif) deliver a frame as a chain of segments, NIC ports always deliver a single segment. A
    // frame that cannot be gathered in its first segment is discarded.
    if (OCUDU_UNLIKELY(mbuf->nb_segs > 1) && ::rte_pktmbuf_linearize(mbuf) != 0) {
      return;
    }
    frame = span<const uint8_t>(rte_pktmbuf_mtod(mbuf, const uint8_t*), mbuf->data_len);
  }

  /// Copy constructor is deleted.
  dpdk_rx_buffer_impl(dpdk_rx_buffer_impl& /**/) = delete;

  /// Move constructor.
  dpdk_rx_buffer_impl(dpdk_rx_buffer_impl&& other) noexcept :
    mbuf(std::exchange(other.mbuf, nullptr)), frame(std::exchange(other.frame, {}))
  {
  }

  /// Move assignment operator.
  dpdk_rx_buffer_impl& operator=(dpdk_rx_buffer_impl&& other) noexcept
  {
    // Free stored mbuf.
    ::rte_pktmbuf_free(mbuf);

    mbuf  = std::exchange(other.mbuf, nullptr);
    frame = std::exchange(other.frame, {});
    return *this;
  }

  ~dpdk_rx_buffer_impl() { ::rte_pktmbuf_free(mbuf); }

  // See interface for documentation.
  span<const uint8_t> data() const override
  {
    ocudu_assert(mbuf != nullptr, "Invalid dpdk_rx_buffer_impl accessed");
    return frame;
  }

private:
  ::rte_mbuf*         mbuf;
  span<const uint8_t> frame;
};

} // namespace ether
} // namespace ocudu
