// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_difi_rx_stream.h"
#include "ocudu/gateways/baseband/buffer/baseband_gateway_buffer_writer.h"
#include "ocudu/support/ocudu_assert.h"
#include <algorithm>
#include <cstdlib>
#include <thread>

using namespace ocudu;

/// \brief Fixed offset by which the receive timeline trails the transmitter, applied once when anchored.
///
/// Keeps a backlog in the socket queue to absorb burstiness: a shortfall is zero-filled, not deferred.
static constexpr std::chrono::microseconds RX_LAG{2000};

/// \brief Maximum drift behind schedule before the timeline is re-anchored.
///
/// Bounds the catch-up debt taken on across a stall. Ordinary lateness is absorbed by not waiting.
static constexpr std::chrono::milliseconds MAX_TIMELINE_DEBT{50};

/// \brief Largest offset between the transmitter epoch and ours that is treated as a shared clock.
///
/// A peer locked onto our timestamps lands within milliseconds; its own epoch is seconds or years away.
static constexpr double SHARED_EPOCH_TOLERANCE_S = 0.1;

/// How often the arrival margin of received datagrams is logged.
static constexpr std::chrono::seconds TIMING_REPORT_PERIOD{1};

radio_difi_rx_stream::radio_difi_rx_stream(const stream_description& config_, radio_event_notifier& notifier) :
  config(config_), logger(ocudulog::fetch_basic_logger(config_.stream_id_str, false))
{
  logger.set_level(config.log_level);

  if (config.channels.empty() || (config.channels.size() > RADIO_MAX_NOF_CHANNELS)) {
    logger.error("A DIFI receive stream takes between 1 and {} channels, but {} were given.",
                 RADIO_MAX_NOF_CHANNELS,
                 config.channels.size());
    return;
  }

  channels.reserve(config.channels.size());
  for (unsigned channel_id = 0, nof_channels = config.channels.size(); channel_id != nof_channels; ++channel_id) {
    const channel_description&                       channel        = config.channels[channel_id];
    const radio_difi_rx_channel::channel_description channel_config = {.ip             = config.ip,
                                                                       .port           = channel.port,
                                                                       .difi_stream_id = channel.difi_stream_id,
                                                                       .stream_id      = config.stream_id,
                                                                       .channel_id     = channel_id,
                                                                       .bit_depth      = config.bit_depth,
                                                                       .iq_byte_order  = config.iq_byte_order,
                                                                       .sample_rate_Hz = config.sample_rate_Hz};
    const auto&                                      rx_channel =
        channels.emplace_back(std::make_unique<radio_difi_rx_channel>(channel_config, notifier, logger));
    if (!rx_channel->is_successful()) {
      return;
    }
  }
  channel_synchronized.resize(channels.size(), false);

  successful = true;
}

baseband_gateway_receiver::metadata radio_difi_rx_stream::receive(baseband_gateway_buffer_writer& data)
{
  const unsigned nof_requested = data.get_nof_samples();
  const uint64_t base_ts       = sample_count.load(std::memory_order_relaxed);
  const unsigned nof_channels  = channels.size();

  ocudu_assert(data.get_nof_channels() >= nof_channels,
               "The buffer has {} channels, fewer than the {} of the stream.",
               data.get_nof_channels(),
               nof_channels);

  // Wall-clock span the requested samples represent. This call must occupy that span: receive() is the
  // only thing pacing the lower physical layer, whose uplink chain reschedules itself with no throttle
  // and whose downlink waits on the timestamp published here. Returning late throttles the downlink too.
  const auto start_time = std::chrono::steady_clock::now();
  const auto budget     = std::chrono::nanoseconds(
      (config.sample_rate_Hz > 0.0)
          ? static_cast<int64_t>(static_cast<double>(nof_requested) * 1e9 / config.sample_rate_Hz)
          : 0);

  // Deadlines derive from an anchor rather than from the start of the call, so the timeline advances by
  // exactly one budget per buffer and a call that finishes early leaves slack for the next one.
  auto compute_deadline = [&]() -> std::chrono::steady_clock::time_point {
    if (!clock_anchor.has_value() || config.sample_rate_Hz <= 0.0) {
      return start_time + budget;
    }
    const double target_s = static_cast<double>(base_ts + nof_requested - clock_anchor_ts) / config.sample_rate_Hz;
    return *clock_anchor + std::chrono::nanoseconds(static_cast<int64_t>(target_s * 1e9));
  };

  // Anchor on the first call, so the timeline paces correctly before any transmitter appears.
  if (!clock_anchor.has_value() && (config.sample_rate_Hz > 0.0)) {
    clock_anchor    = start_time;
    clock_anchor_ts = base_ts;
  }

  auto deadline = compute_deadline();

  // Falling behind is normal, and the way to catch up is to stop waiting. Re-anchor only past a stall so
  // long that the debt would otherwise be repaid by returning a burst of buffers back to back.
  if (clock_anchor.has_value() && start_time > deadline + MAX_TIMELINE_DEBT) {
    clock_anchor    = start_time;
    clock_anchor_ts = base_ts;
    deadline        = compute_deadline();
  }

  // Place the tail of a packet that straddled the end of the previous buffer before reading anything new.
  for (unsigned channel_id = 0; channel_id != nof_channels; ++channel_id) {
    channels[channel_id]->begin_buffer(data[channel_id].first(nof_requested), base_ts);
  }

  const auto plausible_offset = static_cast<int64_t>(config.sample_rate_Hz * SHARED_EPOCH_TOLERANCE_S);

  bool peer_active = false;
  // Places a data packet of a channel on the timeline shared by every channel of the stream.
  auto handle_packet = [&](unsigned channel_id, const radio_difi_rx_channel::data_packet& packet) {
    radio_difi_rx_channel& channel = *channels[channel_id];

    if (!peer_active) {
      peer_active = true;
      if (!clock_anchor_locked_to_peer) {
        // Re-anchor to the transmitter first packet, so the schedule is expressed relative to when it
        // started streaming. RX_LAG is applied here, once, and every deadline is then one budget on.
        clock_anchor                = std::chrono::steady_clock::now() + RX_LAG;
        clock_anchor_ts             = channel.get_next_timestamp();
        clock_anchor_locked_to_peer = true;
        deadline                    = compute_deadline();
      }
    }

    // Bring the wire timestamp back onto the baseband timeline before anything compares it, so the
    // shared-clock test below sees like for like rather than a whole UTC epoch of difference.
    const uint64_t pkt_ts = static_cast<uint64_t>(static_cast<int64_t>(packet.timestamp) - epoch_offset);

    if (!ts_offset.has_value()) {
      // Trust absolute time when the transmitter is plausibly on our clock. Translating a peer that is
      // already on it would place its uplink an arbitrary distance away, since a receiver derives the
      // subframe of a buffer purely from its timestamp.
      //
      // The first packet of any channel sets the mapping for all of them: the ports of one transmitter share a clock,
      // and a mapping per channel could leave them samples apart.
      const int64_t delta = static_cast<int64_t>(channel.get_next_timestamp()) - static_cast<int64_t>(pkt_ts);
      ts_offset           = (config.latch_peer_epoch || (std::abs(delta) > plausible_offset)) ? delta : 0;
      channel_synchronized[channel_id] = true;
    }

    const int64_t local_ts = static_cast<int64_t>(pkt_ts) + *ts_offset;

    if (!channel_synchronized[channel_id]) {
      // The first packet of every other channel must land on the shared mapping as close as a peer locked to our
      // clock would. Further away, the transmitter stamps this port against another clock; the samples are kept
      // where the timestamps put them, since shifting them would hide the fault rather than fix it.
      channel_synchronized[channel_id] = true;
      const int64_t distance           = local_ts - static_cast<int64_t>(channel.get_next_timestamp());
      if (std::abs(distance) > plausible_offset) {
        logger.warning("stream={} channel={}: DIFI channel timestamps disagree with the other channels of the "
                       "stream. distance={} samples",
                       config.stream_id,
                       channel_id,
                       distance);
      }
    }

    // Arrival margin: how long before the last sample of the packet is due on the anchored timeline it arrived. The
    // kernel stamps arrival on the system clock, which is brought onto the steady clock of the timeline here; a
    // datagram without a stamp falls back on the time it was read, which overstates its lateness.
    if (clock_anchor.has_value() && (config.sample_rate_Hz > 0.0)) {
      const double due_s = static_cast<double>(local_ts + packet.nof_samples - static_cast<int64_t>(clock_anchor_ts)) /
                           config.sample_rate_Hz;
      const auto due     = *clock_anchor + std::chrono::nanoseconds(static_cast<int64_t>(due_s * 1e9));
      auto       arrival = std::chrono::steady_clock::now();
      if (packet.arrival.has_value()) {
        arrival -= std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::system_clock::now() -
                                                                                   *packet.arrival);
      }
      timing_stats.record(due - arrival);
    }
    // A packet starting behind what the channel already holds was dropped, at least in part, for arriving late.
    if (local_ts < static_cast<int64_t>(channel.get_next_timestamp())) {
      timing_stats.record_late();
    }

    channel.write_packet(packet, local_ts);
  };

  // Service every channel in turn, one datagram at a time, so a busy port cannot starve the others. Take queued
  // datagrams first and poll only when every unfilled channel is empty: under load the queues are rarely empty, so
  // this saves most of the system calls per datagram.
  static_vector<bool, RADIO_MAX_NOF_CHANNELS>                   channel_failed(nof_channels, false);
  static_vector<radio_difi_udp_socket*, RADIO_MAX_NOF_CHANNELS> idle_sockets;
  for (;;) {
    bool progressed = false;
    idle_sockets.clear();

    for (unsigned channel_id = 0; channel_id != nof_channels; ++channel_id) {
      radio_difi_rx_channel& channel = *channels[channel_id];
      if (channel_failed[channel_id] || channel.is_buffer_full()) {
        continue;
      }

      radio_difi_rx_channel::data_packet       packet;
      const radio_difi_rx_channel::read_status status = channel.try_read(packet);
      if (status == radio_difi_rx_channel::read_status::empty) {
        idle_sockets.push_back(&channel.get_socket());
        continue;
      }
      if (status == radio_difi_rx_channel::read_status::error) {
        // Leave a failing socket alone for the rest of the buffer, rather than spinning on it.
        channel_failed[channel_id] = true;
        continue;
      }

      progressed = true;
      if (status == radio_difi_rx_channel::read_status::data) {
        handle_packet(channel_id, packet);
      }
    }

    if (progressed) {
      continue;
    }
    // Every channel is full or has failed.
    if (idle_sockets.empty()) {
      break;
    }

    // The deadline bounds the whole call. Past it the wait drops to zero, so a receiver that has fallen
    // behind drains what is queued and stops, instead of waiting for more.
    const auto now  = std::chrono::steady_clock::now();
    auto       wait = std::chrono::microseconds(0);
    if (now < deadline) {
      wait = std::chrono::duration_cast<std::chrono::microseconds>(deadline - now);
    }
    if (!radio_difi_udp_socket::wait_any_readable(idle_sockets, wait)) {
      break;
    }
  }

  // Zero-fill whatever the transmitter did not supply, so the caller always sees a full buffer.
  for (auto& channel : channels) {
    channel->end_buffer();
  }

  // Never hand a buffer over before the wall-clock time its last sample stands for. A peer that is sample-locked to
  // this gNB would otherwise let the pair run faster than real time, shrinking every processing deadline measured
  // in slots, and building a lead that a later stall of the peer repays as a freeze. The deadline is anchored, so
  // sleep overshoot is absorbed by the next call rather than accumulating.
  if (clock_anchor.has_value()) {
    std::this_thread::sleep_until(deadline);
  }

  // The timeline advances by the whole buffer regardless of how much arrived, matching how the caller
  // computes the next expected timestamp.
  sample_count.store(base_ts + nof_requested, std::memory_order_relaxed);

  report_timing();

  return metadata{.ts = base_ts};
}

void radio_difi_rx_stream::report_timing()
{
  const auto now = std::chrono::steady_clock::now();
  if (!last_timing_report.has_value()) {
    last_timing_report = now;
    return;
  }
  if (now - *last_timing_report < TIMING_REPORT_PERIOD) {
    return;
  }
  last_timing_report = now;

  const std::optional<radio_difi_rx_timing_stats::summary> summary = timing_stats.get_summary();
  timing_stats.reset();
  if (!summary.has_value()) {
    return;
  }

  logger.info("stream={}: DIFI receive timing. datagrams={} late={} margin_min={}us margin_p0.1={}us "
              "margin_p1={}us margin_p50={}us rx_lag={}us",
              config.stream_id,
              summary->nof_datagrams,
              summary->nof_late,
              summary->min.count(),
              summary->p0_1.count(),
              summary->p1.count(),
              summary->p50.count(),
              RX_LAG.count());
}

void radio_difi_rx_stream::start(baseband_gateway_timestamp init_time, int64_t epoch_offset_ticks)
{
  sample_count.store(init_time, std::memory_order_relaxed);
  epoch_offset = epoch_offset_ticks;

  // Drop any latched epoch: the next packet re-locks onto the transmitter.
  ts_offset.reset();
  std::fill(channel_synchronized.begin(), channel_synchronized.end(), false);
  timing_stats.reset();
  last_timing_report.reset();
  clock_anchor.reset();
  clock_anchor_ts             = 0;
  clock_anchor_locked_to_peer = false;

  for (auto& channel : channels) {
    channel->start();
  }
}

void radio_difi_rx_stream::stop()
{
  for (auto& channel : channels) {
    channel->stop();
  }
}
