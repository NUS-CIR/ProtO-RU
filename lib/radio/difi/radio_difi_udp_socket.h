// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/expected.h"
#include "ocudu/adt/span.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/units.h"
#include <chrono>
#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>

namespace ocudu {

/// Reasons a DIFI socket receive did not yield a datagram.
enum class difi_recv_error {
  /// No datagram is queued; the caller may wait for one and retry.
  timeout,
  /// The receive failed and has been logged; the caller should stop.
  failure
};

/// \brief POSIX UDP socket wrapper for DIFI packet transport, not copyable or movable.
///
/// Opened either as a transmitter to a fixed remote address, or as a receiver bound to a local port.
class radio_difi_udp_socket
{
public:
  /// Default receive buffer size applied via SO_RCVBUF.
  static constexpr units::bytes RX_SOCKET_BUFFER_BYTES{2 * 1024 * 1024};

  explicit radio_difi_udp_socket(ocudulog::basic_logger& logger_) : logger(logger_) {}
  ~radio_difi_udp_socket();
  radio_difi_udp_socket(const radio_difi_udp_socket&)            = delete;
  radio_difi_udp_socket& operator=(const radio_difi_udp_socket&) = delete;

  /// \brief Opens a transmit socket sending to \p ip : \p port. Returns true on success.
  ///
  /// Any socket already held is closed first, so re-opening does not leak a descriptor.
  bool open_tx(const std::string& ip, uint16_t port);

  /// \brief Opens a receive socket bound to \p ip : \p port, "0.0.0.0" for any interface.
  ///
  /// Returns true on success. As with open_tx(), any socket already held is closed first. \p rx_buffer_size is
  /// requested via SO_RCVBUF. The kernel caps it at net.core.rmem_max, so the size it actually granted is logged.
  bool open_rx(const std::string& ip, uint16_t port, units::bytes rx_buffer_size = RX_SOCKET_BUFFER_BYTES);

  /// Closes the socket. Safe to call if already closed.
  void close();

  bool is_open() const { return fd >= 0; }

  /// Sends every byte of \p buf. Returns true if all bytes were sent, false on a closed socket.
  bool send(span<const uint8_t> buf);

  /// \brief Receives one queued datagram into \p buf, up to its size, without blocking.
  ///
  /// Returns the prefix of \p buf holding the datagram, which may be empty for a zero-length one. On failure
  /// returns difi_recv_error::timeout if none is queued, otherwise ::failure, which also covers a closed socket.
  /// Lets a receiver drain a backlog with one system call per datagram, polling with wait_readable() only when
  /// the queue is empty.
  expected<span<uint8_t>, difi_recv_error> try_recv(span<uint8_t> buf);

  /// \brief Waits for a readable datagram or \p timeout, zero polling without blocking.
  ///
  /// Lets the caller pace itself against wall-clock time rather than the fixed socket timeout.
  bool wait_readable(std::chrono::microseconds timeout);

  /// \brief Waits until any of \p sockets has a readable datagram, or \p timeout elapses.
  ///
  /// Closed sockets are skipped.
  /// \return True if at least one socket is readable.
  static bool wait_any_readable(span<radio_difi_udp_socket* const> sockets, std::chrono::microseconds timeout);

private:
  /// Logger, owned by the stream that created this socket.
  ocudulog::basic_logger& logger;
  /// Underlying file descriptor, -1 when closed.
  int fd = -1;
  /// Destination address used by send(), populated by open_tx().
  struct sockaddr_in dest_addr = {};
};

} // namespace ocudu
