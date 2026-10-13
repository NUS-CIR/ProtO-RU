// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "radio_difi_tx_channel.h"
#include "ocudu/gateways/baseband/baseband_gateway_timestamp.h"
#include "ocudu/gateways/baseband/baseband_gateway_transmitter.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/radio/radio_event_notifier.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace ocudu {

/// \brief Implements a baseband gateway transmitter that sends each channel of a buffer as its own DIFI stream.
///
/// Channel \c i of every buffer goes to transmit channel \c i, each on its own UDP port.
class radio_difi_tx_stream : public baseband_gateway_transmitter
{
public:
  /// Per-channel parameters of a DIFI transmit stream.
  struct channel_description {
    /// Destination UDP port for the DIFI packets of this channel.
    uint16_t port = 0;
    /// DIFI 32-bit stream identifier embedded in every packet header of this channel.
    uint32_t difi_stream_id = 0;
    /// RF gain in dB, encoded in the lower 16 bits of the context packet gains field.
    double rf_gain_dB = 0.0;
    /// IF gain in dB, encoded in the upper 16 bits of the context packet gains field.
    double if_gain_dB = 0.0;
  };

  /// Parameters required to create a DIFI transmit stream.
  struct stream_description {
    /// Destination IP address for outgoing DIFI packets, shared by every channel.
    std::string ip;
    /// Channels of the stream, one per antenna port.
    std::vector<channel_description> channels;
    /// Radio stream identifier reported in radio events.
    unsigned stream_id = 0;
    /// IQ sample bit depth: 8 or 16.
    unsigned bit_depth = 16;
    /// Byte order of 16-bit IQ components in transmitted packets.
    difi_iq_byte_order iq_byte_order = difi_iq_byte_order::big;
    /// Sample rate in Hz, used for timestamp conversion (sample ticks <-> seconds).
    double sample_rate_Hz = 0.0;
    /// RF centre frequency in Hz, encoded in the DIFI context packet rf_ref_freq field.
    double center_freq_Hz = 0.0;
    /// Stream identifier string.
    std::string stream_id_str;
    /// Logging level.
    ocudulog::basic_levels log_level = ocudulog::basic_levels::info;
  };

  /// Maximum IQ samples per data packet; larger transmit buffers are fragmented.
  static constexpr unsigned MAX_PKT_SAMPLES = radio_difi_tx_channel::MAX_PKT_SAMPLES;

  radio_difi_tx_stream(const stream_description& config, radio_event_notifier& notifier_);

  /// Returns true if the stream was constructed successfully, including opening the sockets of all its channels.
  bool is_successful() const { return successful; }

  // See baseband_gateway_transmitter for documentation.
  void transmit(const baseband_gateway_buffer_reader&        data,
                const baseband_gateway_transmitter_metadata& metadata) override;

  /// \brief Starts the stream.
  ///
  /// \p epoch_offset_ticks is added to every timestamp put on the wire, mapping the baseband
  /// timeline onto UTC. Zero keeps the timeline as-is, which is what the loopback tests expect.
  void start(baseband_gateway_timestamp init_time, int64_t epoch_offset_ticks = 0);

  /// Stops the stream.
  void stop();

  /// Updates the RF centre frequency of every channel. Each channel announces it in a context packet ahead of its next
  /// transmitted data.
  void set_freq(double center_freq_Hz);

  /// \brief Updates the RF and IF gains of one channel. The channel announces them in a context packet ahead of its
  /// next transmitted data.
  ///
  /// \return False if \p channel_id does not name a channel of this stream.
  bool set_gain(unsigned channel_id, double rf_gain_dB, double if_gain_dB);

private:
  /// Radio event notifier.
  radio_event_notifier& notifier;
  /// Logger.
  ocudulog::basic_logger& logger;
  /// Radio stream identifier reported in radio events.
  unsigned stream_id;
  /// Indicates whether the class was initialized successfully.
  bool successful = false;
  /// Transmit channels, one per antenna port.
  std::vector<std::unique_ptr<radio_difi_tx_channel>> channels;
  /// Expected next transmit timestamp (sample ticks); used to detect underflow gaps.
  uint64_t last_tx_end_ts = 0;
  /// Indicates whether the stream has been started.
  bool started = false;
};

} // namespace ocudu
