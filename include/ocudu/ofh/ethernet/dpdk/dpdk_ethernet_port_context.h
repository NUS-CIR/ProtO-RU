// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/support/units.h"
#include <memory>

struct rte_mempool;

namespace ocudu {
namespace ether {

/// DPDK configuration settings.
constexpr unsigned MAX_BURST_SIZE = 64;

/// DPDK port configuration.
struct dpdk_port_config {
  /// Device identifier.
  std::string id;
  /// Promiscuous mode flag.
  bool is_promiscuous_mode_enabled;
  /// Ethernet link status checking flag.
  bool is_link_status_check_enabled;
  /// MTU size.
  units::bytes mtu_size;
};

/// \brief DPDK Ethernet port context.
///
/// Encapsulates and manages the lifetime of the internal DPDK resources of an Ethernet port.
class dpdk_port_context
{
  dpdk_port_context(const std::string& port_id_, unsigned dpdk_port_id_, std::shared_ptr<::rte_mempool> mem_pool_) :
    port_id(port_id_), dpdk_port_id(dpdk_port_id_), mem_pool(std::move(mem_pool_))
  {
  }

public:
  /// Creates and initializes a new DPDK port context with the given configuration.
  static std::shared_ptr<dpdk_port_context> create(const dpdk_port_config& config);

  ~dpdk_port_context();

  /// Returns the DPDK port identifier of this context .
  unsigned get_dpdk_port_id() const { return dpdk_port_id; }

  /// Returns the string port identifier of this context.
  const std::string& get_port_id() const { return port_id; }

  /// Returns the mbuf memory pool of this context.
  ::rte_mempool* get_mempool() { return mem_pool.get(); }

  /// Returns the mbuf memory pool of this context.
  const ::rte_mempool* get_mempool() const { return mem_pool.get(); }

private:
  const std::string                    port_id;
  const unsigned                       dpdk_port_id;
  const std::shared_ptr<::rte_mempool> mem_pool;
};

} // namespace ether
} // namespace ocudu
