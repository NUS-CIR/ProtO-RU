// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_difi_tx_channel.h"
#include "radio_difi_context_packet.h"
#include "ocudu/support/ocudu_assert.h"
#include <algorithm>
#include <array>

using namespace ocudu;

radio_difi_tx_channel::radio_difi_tx_channel(const channel_description& config_, ocudulog::basic_logger& logger_) :
  config(config_),
  center_freq_Hz(config_.center_freq_Hz),
  rf_gain_dB(config_.rf_gain_dB),
  if_gain_dB(config_.if_gain_dB),
  socket(logger_)
{
  // difi_data_packet_size() sizes anything that is not 8 as 16 bit, while the builder fills any
  // non-16 depth as 8 bit, so an unsupported depth would size and fill the payload inconsistently.
  // The depth comes from user configuration, so it is rejected here rather than asserted: an assert
  // is compiled out unless ASSERTS_ENABLED is set, which would leave a release build emitting
  // malformed packets.
  if (config.bit_depth != 8 && config.bit_depth != 16) {
    logger_.error("Unsupported DIFI IQ bit depth {}, expected 8 or 16.", config.bit_depth);
    return;
  }

  // Size the packet buffer once for the largest fragment, so that transmit() never allocates on the radio thread.
  tx_buf.resize(difi_data_packet_size(config.bit_depth, MAX_PKT_SAMPLES).value());

  if (!socket.open_tx(config.ip, config.port)) {
    return;
  }

  successful = true;
}

void radio_difi_tx_channel::transmit(span<const ci16_t> samples, uint64_t timestamp)
{
  if (!socket.is_open()) {
    return;
  }

  // Fragment across consecutive data packets of at most MAX_PKT_SAMPLES each: a whole slot exceeds both
  // the maximum UDP payload and the receive buffer of typical DIFI implementations.
  for (unsigned offset = 0, nof_samples = samples.size(); offset != nof_samples;) {
    const unsigned nof_chunk = std::min(MAX_PKT_SAMPLES, nof_samples - offset);
    // Each fragment carries the timestamp of its own first sample.
    const uint64_t chunk_ts = timestamp + offset;

    uint32_t full_secs = 0;
    uint64_t frac_ps   = 0;
    difi_ticks_to_time(static_cast<uint64_t>(static_cast<int64_t>(chunk_ts) + epoch_offset),
                       config.sample_rate_Hz,
                       full_secs,
                       frac_ps);

    // Send the context packet before the data packet at each interval boundary, and as soon as a new frequency or gain
    // is waiting. Only this thread sends context packets once the channel is started.
    const bool update_pending = ctx_update_pending.exchange(false, std::memory_order_acquire);
    if (update_pending || (pkts_since_ctx >= CTX_INTERVAL)) {
      send_context_packet(full_secs, frac_ps);
      pkts_since_ctx = 0;
    }

    difi_data_packet_params p;
    p.stream_id     = config.difi_stream_id;
    p.full_secs     = full_secs;
    p.frac_ps       = frac_ps;
    p.bit_depth     = config.bit_depth;
    p.pkt_n         = data_pkt_n;
    p.iq_byte_order = config.iq_byte_order;

    const units::bytes pkt_bytes = difi_data_packet_size(config.bit_depth, nof_chunk);
    ocudu_assert(tx_buf.size() >= pkt_bytes.value(),
                 "Packet buffer of '{}' bytes is too small for a fragment of '{}' bytes",
                 tx_buf.size(),
                 pkt_bytes.value());

    build_difi_data_packet(tx_buf, p, samples.subspan(offset, nof_chunk));

    if (!socket.send(span<const uint8_t>(tx_buf).first(pkt_bytes.value()))) {
      // Drop the rest of this buffer. The receiver detects the timestamp gap and zero-fills.
      return;
    }

    data_pkt_n = (data_pkt_n + 1U) & 0xfU;
    ++pkts_since_ctx;
    offset += nof_chunk;
  }
}

void radio_difi_tx_channel::send_context_packet(uint32_t full_secs, uint64_t frac_ps)
{
  difi_context_packet_params p;
  p.stream_id      = config.difi_stream_id;
  p.full_secs      = full_secs;
  p.frac_ps        = frac_ps;
  p.sample_rate_Hz = config.sample_rate_Hz;
  p.center_freq_Hz = center_freq_Hz.load(std::memory_order_relaxed);
  p.bit_depth      = config.bit_depth;
  p.rf_gain_dB     = rf_gain_dB.load(std::memory_order_relaxed);
  p.if_gain_dB     = if_gain_dB.load(std::memory_order_relaxed);
  p.ctx_pkt_n      = ctx_pkt_n;

  std::array<uint8_t, DIFI_CONTEXT_PACKET_SIZE.value()> buf;
  build_difi_context_packet(buf, p);

  if (!socket.send(buf)) {
    return;
  }

  ctx_pkt_n = (ctx_pkt_n + 1U) & 0xfU;
}

void radio_difi_tx_channel::start(baseband_gateway_timestamp init_time, int64_t epoch_offset_ticks)
{
  // A channel that failed construction must never put a packet on the wire.
  if (!successful) {
    return;
  }

  epoch_offset = epoch_offset_ticks;

  uint32_t full_secs = 0;
  uint64_t frac_ps   = 0;
  difi_ticks_to_time(
      static_cast<uint64_t>(static_cast<int64_t>(init_time) + epoch_offset), config.sample_rate_Hz, full_secs, frac_ps);

  // This context packet carries the current frequency and gains, so any change still waiting is already announced.
  ctx_update_pending.store(false, std::memory_order_relaxed);
  send_context_packet(full_secs, frac_ps);
}

void radio_difi_tx_channel::set_freq(double new_center_freq_Hz)
{
  center_freq_Hz.store(new_center_freq_Hz, std::memory_order_relaxed);
  ctx_update_pending.store(true, std::memory_order_release);
}

void radio_difi_tx_channel::set_gain(double new_rf_gain_dB, double new_if_gain_dB)
{
  rf_gain_dB.store(new_rf_gain_dB, std::memory_order_relaxed);
  if_gain_dB.store(new_if_gain_dB, std::memory_order_relaxed);
  ctx_update_pending.store(true, std::memory_order_release);
}

void radio_difi_tx_channel::stop()
{
  socket.close();
}
