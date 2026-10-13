// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "radio_difi_data_packet.h"
#include "radio_difi_rx_channel.h"
#include "radio_difi_rx_timing_stats.h"
#include "radio_difi_stream_config.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/gateways/baseband/baseband_gateway_receiver.h"
#include "ocudu/gateways/baseband/baseband_gateway_timestamp.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/radio/radio_constants.h"
#include "ocudu/radio/radio_event_notifier.h"
#include <atomic>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace ocudu {

/// \brief Implements a gateway receiver that unpacks IQ samples from DIFI data packets received over UDP.
///
/// Each channel of the buffer is filled by its own receive channel, a DIFI stream on its own UDP port. The stream
/// paces the calls to the wall clock and maps wire timestamps onto the baseband timeline once for all channels.
class radio_difi_rx_stream : public baseband_gateway_receiver
{
public:
  /// Per-channel parameters of a DIFI receive stream.
  struct channel_description {
    /// Port to listen on for this channel.
    uint16_t port = 0;
    /// DIFI stream identifier of this channel. Packets with a different identifier are discarded.
    uint32_t difi_stream_id = 0;
  };

  /// Describes the necessary parameters to create a DIFI Rx stream.
  struct stream_description {
    /// Address to bind the receive sockets on, shared by every channel. Use "0.0.0.0" for any interface.
    std::string ip;
    /// Channels of the stream, one per antenna port.
    std::vector<channel_description> channels;
    /// Radio stream identifier reported in radio events.
    unsigned stream_id = 0;
    /// IQ sample bit depth, either 8 or 16.
    unsigned bit_depth = 16;
    /// Byte order of 16-bit IQ components in received packets.
    difi_iq_byte_order iq_byte_order = difi_iq_byte_order::big;
    /// Sample rate in Hz, used to convert between timestamps and sample ticks.
    double sample_rate_Hz = 0.0;
    /// Stream identifier string.
    std::string stream_id_str;
    /// Logging level.
    ocudulog::basic_levels log_level = ocudulog::basic_levels::info;
    /// \brief Fixed offset by which the receive timeline trails the transmitter, applied once when anchored.
    ///
    /// Keeps a backlog in the socket queue to absorb burstiness: a shortfall is zero-filled, not deferred.
    std::chrono::microseconds rx_lag = DIFI_DEFAULT_RX_LAG;
    /// \brief Latches the transmitter epoch onto this stream timeline instead of trusting it.
    ///
    /// Enable only for a peer running an unrelated epoch, such as equipment timestamping against UTC.
    bool latch_peer_epoch = false;
  };

  radio_difi_rx_stream(const stream_description& config_, radio_event_notifier& notifier);

  // See interface for documentation.
  metadata receive(baseband_gateway_buffer_writer& data) override;

  /// Returns true if the stream was constructed successfully, including binding the sockets of all its channels.
  bool is_successful() const { return successful; }

  uint64_t get_sample_count() const { return sample_count.load(std::memory_order_relaxed); }

  /// \brief Starts the stream.
  ///
  /// \p epoch_offset_ticks is subtracted from every timestamp received, mapping the UTC wire
  /// timeline back onto the baseband one. Must match what the paired transmit stream adds.
  void start(baseband_gateway_timestamp init_time, int64_t epoch_offset_ticks = 0);

  void stop();

private:
  /// Logs the arrival margin of the datagrams received over the last report period, once a period.
  void report_timing();

  /// Stream configuration.
  stream_description config;
  /// Logger.
  ocudulog::basic_logger& logger;
  /// Indicates whether the class was initialized successfully.
  bool successful = false;
  /// Counts the number of received samples.
  std::atomic<uint64_t> sample_count = {0};
  /// Receive channels, one per antenna port.
  std::vector<std::unique_ptr<radio_difi_rx_channel>> channels;
  /// Offset subtracted from a wire timestamp to bring it onto the baseband timeline.
  int64_t epoch_offset = 0;
  /// Offset added to a packet timestamp to place it on this stream timeline. Empty until the first packet.
  std::optional<int64_t> ts_offset;
  /// Indicates, per channel, whether its first packet since start() has been checked against \ref ts_offset.
  static_vector<bool, RADIO_MAX_NOF_CHANNELS> channel_synchronized;
  /// \brief Reference system time associated with a timestamp.
  ///
  /// Wall clock anchor of the real-time timeline, and the sample position it maps to. This is necessary
  /// for throttling the baseband IQ streaming.
  std::optional<std::chrono::steady_clock::time_point> clock_anchor;
  /// Timestamp associated with the clock anchor.
  uint64_t clock_anchor_ts = 0;
  /// Indicates whether \ref clock_anchor has been re-taken against the transmitter first packet.
  bool clock_anchor_locked_to_peer = false;
  /// How early datagrams arrive ahead of the time their samples are due.
  radio_difi_rx_timing_stats timing_stats;
  /// When \ref timing_stats was last logged.
  std::optional<std::chrono::steady_clock::time_point> last_timing_report;
};

} // namespace ocudu
