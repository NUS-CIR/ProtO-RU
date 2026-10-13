// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "radio_difi_data_packet.h"
#include "radio_difi_udp_socket.h"
#include "ocudu/adt/complex.h"
#include "ocudu/adt/span.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/radio/radio_event_notifier.h"
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace ocudu {

/// \brief Receives the DIFI stream of one antenna port over UDP and places its samples in a buffer by timestamp.
///
/// The channel owns everything that belongs to a single DIFI stream: the socket, the datagram parsing, the placement
/// of samples by timestamp and the tail of a packet that runs past the end of a buffer. Pacing and the mapping of wire
/// timestamps onto the baseband timeline belong to the owning stream, which shares them across its channels.
class radio_difi_rx_channel
{
public:
  /// Parameters required to create a DIFI receive channel.
  struct channel_description {
    /// Address to bind the receive socket on. Use "0.0.0.0" for any interface.
    std::string ip;
    /// Port to listen on.
    uint16_t port = 0;
    /// DIFI stream identifier. Packets with a different identifier are discarded.
    uint32_t difi_stream_id = 0;
    /// Radio stream identifier reported in radio events.
    unsigned stream_id = 0;
    /// Channel identifier within the radio stream, reported in radio events.
    unsigned channel_id = 0;
    /// IQ sample bit depth, either 8 or 16.
    unsigned bit_depth = 16;
    /// Byte order of 16-bit IQ components in received packets.
    difi_iq_byte_order iq_byte_order = difi_iq_byte_order::big;
    /// Sample rate in Hz, used to convert between timestamps and sample ticks.
    double sample_rate_Hz = 0.0;
  };

  /// A data packet of this channel, as read from the socket.
  struct data_packet {
    /// Wire timestamp of the first sample, in sample ticks.
    uint64_t timestamp = 0;
    /// Packed IQ samples, valid until the next read.
    span<const uint8_t> payload;
    /// Number of IQ samples in \ref payload.
    unsigned nof_samples = 0;
    /// When the kernel received the datagram, if it said.
    std::optional<std::chrono::system_clock::time_point> arrival;
  };

  /// Outcome of an attempt to read one datagram.
  enum class read_status {
    /// No datagram is queued.
    empty,
    /// The socket reported an error.
    error,
    /// A datagram was read but carries no samples of this channel.
    discarded,
    /// A data packet of this channel was read.
    data
  };

  /// \brief Creates the channel and binds its socket.
  ///
  /// Binding here makes a port already in use fail construction, instead of leaving the channel silently zero-filled.
  radio_difi_rx_channel(const channel_description& config_,
                        radio_event_notifier&      notifier_,
                        ocudulog::basic_logger&    logger_);

  /// Returns true if the channel was constructed successfully, including binding its socket.
  bool is_successful() const { return successful; }

  /// \brief Starts the channel.
  ///
  /// Discards the samples carried over from a previous run and the datagrams queued since construction.
  void start();

  /// Closes the socket.
  void stop();

  /// \brief Starts filling \p buffer, whose first sample stands for \p base_ts.
  ///
  /// Places the tail of a packet that ran past the end of the previous buffer first.
  void begin_buffer(span<ci16_t> buffer, uint64_t base_ts);

  /// Returns true once every sample of the current buffer is written.
  bool is_buffer_full() const { return nof_filled == current_buffer.size(); }

  /// Returns the timestamp of the next sample to be written to the current buffer.
  uint64_t get_next_timestamp() const { return buffer_ts + nof_filled; }

  /// \brief Reads one datagram without blocking.
  ///
  /// \param[out] packet Receives the data packet when the result is read_status::data.
  read_status try_read(data_packet& packet);

  /// Returns the receive socket.
  radio_difi_udp_socket& get_socket() { return socket; }

  /// \brief Writes \p packet, whose first sample stands for \p local_ts on the baseband timeline.
  ///
  /// Zero-fills a gap ahead of it, drops samples already behind the buffer, and keeps whatever runs past the end of
  /// the buffer for the next one.
  void write_packet(const data_packet& packet, int64_t local_ts);

  /// Zero-fills whatever the transmitter did not supply, so the buffer is always complete.
  void end_buffer();

private:
  /// \brief Resolves where a block of samples starting at \p local_ts belongs in the current buffer.
  ///
  /// Zero-fills any gap ahead of it, and returns how many leading samples are stale, equal to the block size if all
  /// are.
  unsigned position_block(int64_t local_ts, unsigned nof_block);

  /// Channel configuration.
  channel_description config;
  /// Radio event notifier.
  radio_event_notifier& notifier;
  /// Indicates whether the channel was initialized successfully.
  bool successful = false;
  /// Receive socket.
  radio_difi_udp_socket socket;
  /// Reusable datagram buffer, sized for the largest possible UDP payload.
  std::vector<uint8_t> rx_buf;
  /// Receive buffer size requested from the kernel for the socket.
  units::bytes rx_socket_buffer_size;
  /// \brief Samples of a packet that ran past the end of the previous buffer.
  ///
  /// A transmitter packet size need not divide the buffer size; holding the tail avoids losing it.
  std::vector<ci16_t> carry_samples;
  /// Timestamp of the first sample in \ref carry_samples.
  uint64_t carry_ts = 0;
  /// Buffer being filled.
  span<ci16_t> current_buffer;
  /// Timestamp of the first sample of \ref current_buffer.
  uint64_t buffer_ts = 0;
  /// Number of samples of \ref current_buffer already written.
  unsigned nof_filled = 0;
  /// Indicates whether a gap in the current buffer has been reported, so a buffer raises one event at most.
  bool gap_reported = false;
};

} // namespace ocudu
