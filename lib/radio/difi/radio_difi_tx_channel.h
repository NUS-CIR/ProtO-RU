// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "radio_difi_data_packet.h"
#include "radio_difi_udp_socket.h"
#include "ocudu/adt/complex.h"
#include "ocudu/adt/span.h"
#include "ocudu/gateways/baseband/baseband_gateway_timestamp.h"
#include "ocudu/ocudulog/ocudulog.h"
#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

namespace ocudu {

/// \brief Sends the IQ samples of one antenna port as a DIFI stream of data packets over UDP.
///
/// Each channel is a complete DIFI stream of its own: it has its own socket, stream identifier, packet counters and
/// context packets.
class radio_difi_tx_channel
{
public:
  /// Parameters required to create a DIFI transmit channel.
  struct channel_description {
    /// Destination IP address for outgoing DIFI packets.
    std::string ip;
    /// Destination UDP port for outgoing DIFI packets.
    uint16_t port = 0;
    /// DIFI 32-bit stream identifier embedded in every packet header.
    uint32_t difi_stream_id = 0;
    /// IQ sample bit depth: 8 or 16.
    unsigned bit_depth = 16;
    /// Byte order of 16-bit IQ components in transmitted packets.
    difi_iq_byte_order iq_byte_order = difi_iq_byte_order::big;
    /// Sample rate in Hz, used for timestamp conversion (sample ticks <-> seconds).
    double sample_rate_Hz = 0.0;
    /// RF centre frequency in Hz, encoded in the DIFI context packet rf_ref_freq field.
    double center_freq_Hz = 0.0;
    /// RF gain in dB, encoded in the lower 16 bits of the context packet gains field.
    double rf_gain_dB = 0.0;
    /// IF gain in dB, encoded in the upper 16 bits of the context packet gains field.
    double if_gain_dB = 0.0;
  };

  /// \brief Maximum IQ samples per data packet; larger transmit buffers are fragmented.
  ///
  /// 7708 bytes at 16-bit: inside one datagram and a typical 9000-byte receive buffer, and divides every slot.
  static constexpr unsigned MAX_PKT_SAMPLES = 1920;

  /// \brief Creates the channel and opens its socket.
  ///
  /// Opening here makes an unusable address fail construction instead of leaving start() silent.
  radio_difi_tx_channel(const channel_description& config_, ocudulog::basic_logger& logger_);

  /// Returns true if the channel was constructed successfully, including opening its socket.
  bool is_successful() const { return successful; }

  /// Sends \p samples as data packets, the first one stamped with \p timestamp.
  void transmit(span<const ci16_t> samples, uint64_t timestamp);

  /// \brief Starts the channel and sends its first context packet.
  ///
  /// \p epoch_offset_ticks is added to every timestamp put on the wire, mapping the baseband timeline onto UTC.
  void start(baseband_gateway_timestamp init_time, int64_t epoch_offset_ticks);

  /// Stops the channel.
  void stop();

  /// \brief Updates the RF centre frequency.
  ///
  /// Safe to call from any thread while the channel transmits. The context packet announcing the change goes out ahead
  /// of the data packets of the next transmit() call.
  void set_freq(double new_center_freq_Hz);

  /// \brief Updates the RF and IF gains.
  ///
  /// Safe to call from any thread while the channel transmits. The context packet announcing the change goes out ahead
  /// of the data packets of the next transmit() call.
  void set_gain(double new_rf_gain_dB, double new_if_gain_dB);

private:
  /// How often (in data packets) to re-send the context packet.
  static constexpr unsigned CTX_INTERVAL = 100;

  /// Builds and sends a context packet with the supplied timestamp. Called from start() and then only from transmit().
  void send_context_packet(uint32_t full_secs, uint64_t frac_ps);

  /// Channel configuration. The frequency and gains it holds are only the initial values; see the atomics below.
  channel_description config;
  /// RF centre frequency in Hz. Written by set_freq() and read by the transmit thread.
  std::atomic<double> center_freq_Hz;
  /// RF gain in dB. Written by set_gain() and read by the transmit thread.
  std::atomic<double> rf_gain_dB;
  /// IF gain in dB. Written by set_gain() and read by the transmit thread.
  std::atomic<double> if_gain_dB;
  /// Set when the frequency or a gain changed and the context packet has not been re-sent yet.
  std::atomic<bool> ctx_update_pending{false};
  /// Indicates whether the channel was initialized successfully.
  bool successful = false;
  /// Transmit socket.
  radio_difi_udp_socket socket;
  /// Context packet sequence counter (mod-16, independent of data packet counter).
  uint8_t ctx_pkt_n = 0;
  /// Data packet sequence counter (mod-16).
  uint8_t data_pkt_n = 0;
  /// Number of data packets sent since the last context packet. Initialised to 1 so the first transmit() does not
  /// immediately re-send the context packet that start() already sent.
  unsigned pkts_since_ctx = 1;
  /// Packet buffer, sized at construction for the largest fragment so that transmit() never allocates.
  std::vector<uint8_t> tx_buf;
  /// Offset added to a local timestamp to express it on the UTC wire timeline.
  int64_t epoch_offset = 0;
};

} // namespace ocudu
