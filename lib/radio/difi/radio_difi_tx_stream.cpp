// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_difi_tx_stream.h"
#include "ocudu/gateways/baseband/buffer/baseband_gateway_buffer_reader.h"
#include "ocudu/support/ocudu_assert.h"

using namespace ocudu;

radio_difi_tx_stream::radio_difi_tx_stream(const stream_description& config, radio_event_notifier& notifier_) :
  notifier(notifier_), logger(ocudulog::fetch_basic_logger(config.stream_id_str, false)), stream_id(config.stream_id)
{
  logger.set_level(config.log_level);

  channels.reserve(config.channels.size());
  for (const channel_description& channel : config.channels) {
    const radio_difi_tx_channel::channel_description channel_config = {.ip             = config.ip,
                                                                       .port           = channel.port,
                                                                       .difi_stream_id = channel.difi_stream_id,
                                                                       .bit_depth      = config.bit_depth,
                                                                       .iq_byte_order  = config.iq_byte_order,
                                                                       .sample_rate_Hz = config.sample_rate_Hz,
                                                                       .center_freq_Hz = config.center_freq_Hz,
                                                                       .rf_gain_dB     = channel.rf_gain_dB,
                                                                       .if_gain_dB     = channel.if_gain_dB,
                                                                       .pkt_samples    = config.pkt_samples};
    const auto& tx_channel = channels.emplace_back(std::make_unique<radio_difi_tx_channel>(channel_config, logger));
    if (!tx_channel->is_successful()) {
      return;
    }
  }

  successful = true;
}

void radio_difi_tx_stream::transmit(const baseband_gateway_buffer_reader&        data,
                                    const baseband_gateway_transmitter_metadata& metadata)
{
  if (!started) {
    return;
  }

  const unsigned nof_samples = data.get_nof_samples();
  if (nof_samples == 0) {
    return;
  }

  ocudu_assert(data.get_nof_channels() >= channels.size(),
               "The buffer has {} channels, fewer than the {} of the stream.",
               data.get_nof_channels(),
               channels.size());

  // Underflow: gap between the expected next timestamp and the requested one.
  if (metadata.ts > last_tx_end_ts) {
    radio_event_notifier::event_description ev;
    ev.stream_id = stream_id;
    ev.source    = radio_event_source::TRANSMIT;
    ev.type      = radio_event_type::UNDERFLOW;
    ev.timestamp = metadata.ts;
    notifier.on_radio_rt_event(ev);
  }
  last_tx_end_ts = metadata.ts + nof_samples;

  // Transmit the whole buffer, silence included: DIFI is a continuous timestamped stream, so omitting the silence a
  // hardware DAC would idle through leaves holes in it. The lower PHY zeroes the regions outside the transmit window
  // before handing the buffer over, so it is sent as it stands and no intermediate copy is needed.
  for (unsigned channel_id = 0, nof_channels = channels.size(); channel_id != nof_channels; ++channel_id) {
    channels[channel_id]->transmit(data[channel_id].first(nof_samples), metadata.ts);
  }
}

void radio_difi_tx_stream::start(baseband_gateway_timestamp init_time, int64_t epoch_offset_ticks)
{
  // A stream that failed construction must never put a packet on the wire.
  if (!successful) {
    return;
  }

  for (auto& channel : channels) {
    channel->start(init_time, epoch_offset_ticks);
  }

  last_tx_end_ts = init_time;
  started        = true;
}

void radio_difi_tx_stream::stop()
{
  started = false;
  for (auto& channel : channels) {
    channel->stop();
  }
}

void radio_difi_tx_stream::set_freq(double center_freq_Hz)
{
  for (auto& channel : channels) {
    channel->set_freq(center_freq_Hz);
  }
}

bool radio_difi_tx_stream::set_gain(unsigned channel_id, double rf_gain_dB, double if_gain_dB)
{
  if (channel_id >= channels.size()) {
    return false;
  }

  channels[channel_id]->set_gain(rf_gain_dB, if_gain_dB);
  return true;
}
