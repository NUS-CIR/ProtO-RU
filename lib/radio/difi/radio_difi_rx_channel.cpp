// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_difi_rx_channel.h"
#include "ocudu/ocuduvec/copy.h"
#include "ocudu/ocuduvec/zero.h"
#include "ocudu/support/math/endianess_convert.h"
#include <algorithm>
#include <chrono>

using namespace ocudu;

/// \brief Span of samples the receive socket buffer holds.
///
/// The socket is the only slack while the lower physical layer is busy, so it is sized in time rather than
/// bytes: 50 ms covers a stall of many slots at any bandwidth, about 98 MB at 491.52 Msps.
static constexpr std::chrono::milliseconds RX_SOCKET_BUFFER_DURATION{50};

/// DIFI packet type for IF data with stream identifier.
static constexpr uint8_t DIFI_PKT_TYPE_DATA = 0x1U;

/// Largest number of IQ samples one datagram can carry, at the 8 bit depth where a sample takes two bytes.
static constexpr size_t MAX_SAMPLES_PER_DATAGRAM = (DIFI_MAX_UDP_PAYLOAD.value() - DIFI_DATA_HEADER_SIZE.value()) / 2;

/// Returns the number of payload bytes of one IQ sample at \p bit_depth.
static unsigned get_bytes_per_sample(unsigned bit_depth)
{
  return (bit_depth == 8) ? 2U : 4U;
}

/// Unpacks \c nof_samples interleaved IQ samples from \c payload into \c dst.
static void unpack_samples(const uint8_t*     payload,
                           unsigned           nof_samples,
                           unsigned           bit_depth,
                           difi_iq_byte_order byte_order,
                           span<ci16_t>       dst)
{
  if (bit_depth == 16) {
    difi_unpack_iq16(
        dst.first(nof_samples), span<const uint8_t>(payload, nof_samples * DIFI_BYTES_PER_IQ16_SAMPLE), byte_order);
    return;
  }

  // 8-bit samples are signed bytes; scale back to int16 by multiplying by 256.
  for (unsigned i = 0; i != nof_samples; ++i) {
    const int16_t re = static_cast<int16_t>(static_cast<int8_t>(payload[i * 2 + 0]) * 256);
    const int16_t im = static_cast<int16_t>(static_cast<int8_t>(payload[i * 2 + 1]) * 256);
    dst[i]           = ci16_t(re, im);
  }
}

radio_difi_rx_channel::radio_difi_rx_channel(const channel_description& config_,
                                             radio_event_notifier&      notifier_,
                                             ocudulog::basic_logger&    logger_) :
  config(config_), notifier(notifier_), socket(logger_), rx_buf(DIFI_MAX_UDP_PAYLOAD.value())
{
  // Reserve the largest tail a datagram can leave behind, so the receive path never allocates.
  carry_samples.reserve(MAX_SAMPLES_PER_DATAGRAM);

  const units::byterate stream_rate(config.sample_rate_Hz * get_bytes_per_sample(config.bit_depth));
  rx_socket_buffer_size =
      std::max(stream_rate * RX_SOCKET_BUFFER_DURATION, radio_difi_udp_socket::RX_SOCKET_BUFFER_BYTES);
  if (!socket.open_rx(config.ip, config.port, rx_socket_buffer_size)) {
    return;
  }

  successful = true;
}

void radio_difi_rx_channel::start()
{
  carry_samples.clear();
  carry_ts = 0;

  // The socket is bound from construction. Discard what queued since, so the channel begins at the start time. The
  // bound is the most datagrams the socket buffer can hold, so a peer that is already streaming cannot hold start() up.
  const unsigned max_queued_datagrams = rx_socket_buffer_size.value() / DIFI_DATA_HEADER_SIZE.value();
  for (unsigned i = 0; (i != max_queued_datagrams) && socket.try_recv(rx_buf).has_value(); ++i) {
  }
}

void radio_difi_rx_channel::stop()
{
  socket.close();
}

unsigned radio_difi_rx_channel::position_block(int64_t local_ts, unsigned nof_block)
{
  const unsigned nof_requested = current_buffer.size();
  const int64_t  delta         = local_ts - static_cast<int64_t>(buffer_ts + nof_filled);

  if (delta < 0) {
    const auto stale = static_cast<uint64_t>(-delta);
    return (stale >= nof_block) ? nof_block : static_cast<unsigned>(stale);
  }

  if (delta > 0) {
    // Datagrams were lost. Zero-fill the missing span so what did arrive stays at its correct position.
    const auto gap = static_cast<unsigned>(std::min<uint64_t>(delta, nof_requested - nof_filled));
    ocuduvec::zero(current_buffer.subspan(nof_filled, gap));
    if (!gap_reported) {
      radio_event_notifier::event_description ev;
      ev.stream_id  = config.stream_id;
      ev.channel_id = config.channel_id;
      ev.source     = radio_event_source::RECEIVE;
      ev.type       = radio_event_type::OVERFLOW;
      ev.timestamp  = buffer_ts + nof_filled;
      notifier.on_radio_rt_event(ev);
      gap_reported = true;
    }
    nof_filled += gap;
  }

  return 0;
}

void radio_difi_rx_channel::begin_buffer(span<ci16_t> buffer, uint64_t base_ts)
{
  current_buffer = buffer;
  buffer_ts      = base_ts;
  nof_filled     = 0;
  gap_reported   = false;

  if (carry_samples.empty()) {
    return;
  }

  const unsigned nof_requested = current_buffer.size();
  const auto     nof_carry     = static_cast<unsigned>(carry_samples.size());
  const unsigned skip          = position_block(static_cast<int64_t>(carry_ts), nof_carry);
  if ((skip >= nof_carry) || (nof_filled >= nof_requested)) {
    carry_samples.clear();
    return;
  }

  const unsigned nof_write = std::min(nof_carry - skip, nof_requested - nof_filled);
  ocuduvec::copy(current_buffer.subspan(nof_filled, nof_write),
                 span<const ci16_t>(carry_samples).subspan(skip, nof_write));
  nof_filled += nof_write;
  if (skip + nof_write < nof_carry) {
    carry_samples.erase(carry_samples.begin(), carry_samples.begin() + skip + nof_write);
    carry_ts += skip + nof_write;
  } else {
    carry_samples.clear();
  }
}

radio_difi_rx_channel::read_status radio_difi_rx_channel::try_read(data_packet& packet)
{
  const expected<span<uint8_t>, difi_recv_error> received = socket.try_recv(packet.arrival, rx_buf);
  if (!received.has_value()) {
    return (received.error() == difi_recv_error::timeout) ? read_status::empty : read_status::error;
  }

  const span<const uint8_t> datagram = received.value();
  if (datagram.size() < DIFI_DATA_HEADER_SIZE.value()) {
    return read_status::discarded;
  }

  // Packet type lives in bits[31:28]. Context and unknown types carry no samples.
  const uint32_t header = read_u32_be(datagram.first(4));
  if (static_cast<uint8_t>((header >> 28) & 0xfU) != DIFI_PKT_TYPE_DATA) {
    return read_status::discarded;
  }

  if (read_u32_be(datagram.subspan(4, 4)) != config.difi_stream_id) {
    return read_status::discarded;
  }

  packet.timestamp = difi_time_to_ticks(
      read_u32_be(datagram.subspan(16, 4)), read_u64_be(datagram.subspan(20, 8)), config.sample_rate_Hz);
  packet.payload     = datagram.last(datagram.size() - DIFI_DATA_HEADER_SIZE.value());
  packet.nof_samples = static_cast<unsigned>(packet.payload.size() / get_bytes_per_sample(config.bit_depth));
  return read_status::data;
}

void radio_difi_rx_channel::write_packet(const data_packet& packet, int64_t local_ts)
{
  const unsigned nof_requested    = current_buffer.size();
  const unsigned bytes_per_sample = get_bytes_per_sample(config.bit_depth);
  const unsigned nof_pkt          = packet.nof_samples;
  const uint8_t* payload          = packet.payload.data();

  const unsigned skip = position_block(local_ts, nof_pkt);
  if (skip >= nof_pkt) {
    return;
  }
  if (nof_filled == nof_requested) {
    // The gap alone filled the buffer; this packet belongs to the next one.
    const unsigned nof_carry = nof_pkt - skip;
    carry_samples.resize(nof_carry);
    unpack_samples(payload + skip * bytes_per_sample, nof_carry, config.bit_depth, config.iq_byte_order, carry_samples);
    carry_ts = static_cast<uint64_t>(local_ts) + skip;
    return;
  }

  const unsigned nof_write = std::min(nof_pkt - skip, nof_requested - nof_filled);
  unpack_samples(payload + skip * bytes_per_sample,
                 nof_write,
                 config.bit_depth,
                 config.iq_byte_order,
                 current_buffer.subspan(nof_filled, nof_write));
  nof_filled += nof_write;

  if (skip + nof_write < nof_pkt) {
    // The packet runs past the end of this buffer. Keep the tail for the next call.
    const unsigned nof_carry = nof_pkt - skip - nof_write;
    carry_samples.resize(nof_carry);
    unpack_samples(payload + (skip + nof_write) * bytes_per_sample,
                   nof_carry,
                   config.bit_depth,
                   config.iq_byte_order,
                   carry_samples);
    carry_ts = static_cast<uint64_t>(local_ts) + skip + nof_write;
  }
}

void radio_difi_rx_channel::end_buffer()
{
  if (nof_filled != current_buffer.size()) {
    ocuduvec::zero(current_buffer.subspan(nof_filled, current_buffer.size() - nof_filled));
  }
}
