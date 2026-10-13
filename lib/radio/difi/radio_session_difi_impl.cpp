// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_session_difi_impl.h"
#include "radio_difi_stream_config.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/error_handling.h"
#include <chrono>

using namespace ocudu;

/// Bit depth used when otw_format is DEFAULT or SC16.
static constexpr unsigned DEFAULT_BIT_DEPTH = 16;
/// Bit depth used when otw_format is SC8.
static constexpr unsigned SC8_BIT_DEPTH = 8;

radio_session_difi_impl::radio_session_difi_impl(const radio_configuration::radio& config,
                                                 task_executor&                    async_task_executor,
                                                 radio_event_notifier&             notifier) :
  logger(ocudulog::fetch_basic_logger("RF", false))
{
  ocudu_assert(config.tx_streams.size() == config.rx_streams.size(),
               "The number of transmit streams (i.e., {}) must equal the number of receive streams (i.e., {}).",
               config.tx_streams.size(),
               config.rx_streams.size());

  // Resolve bit depth from OTW format.
  unsigned bit_depth =
      (config.otw_format == radio_configuration::over_the_wire_format::SC8) ? SC8_BIT_DEPTH : DEFAULT_BIT_DEPTH;

  sample_rate_Hz = config.sampling_rate_Hz;

  // The gNB configuration validator rejects an invalid byte order before the session is created.
  const std::optional<difi_iq_byte_order> parsed_byte_order = parse_difi_iq_byte_order(config.args);
  ocudu_assert(parsed_byte_order.has_value(), "Invalid DIFI IQ byte order in the device arguments '{}'.", config.args);
  const difi_iq_byte_order iq_byte_order = parsed_byte_order.value_or(difi_iq_byte_order::big);

  unsigned nof_streams = config.tx_streams.size();
  bb_gateways.reserve(nof_streams);

  // DIFI stream identifiers on the wire are the flat port index of each direction, numbered by stream and then by
  // channel, so they are unique across the radio and equal the stream index when every stream has one channel.
  unsigned tx_difi_stream_id = 0;
  unsigned rx_difi_stream_id = 0;

  for (unsigned stream_id = 0; stream_id != nof_streams; ++stream_id) {
    const radio_configuration::stream& tx_stream = config.tx_streams[stream_id];
    const radio_configuration::stream& rx_stream = config.rx_streams[stream_id];

    // Defaults: transmit to localhost:4991.
    auto tx_parsed = parse_difi_stream_args(tx_stream.args, "127.0.0.1", 4991);
    if (!tx_parsed) {
      logger.error("Failed to parse DIFI Tx stream {} arguments.", stream_id);
      return;
    }

    // Defaults: bind all interfaces on port 4992.
    auto rx_parsed = parse_difi_stream_args(rx_stream.args, "0.0.0.0", 4992);
    if (!rx_parsed) {
      logger.error("Failed to parse DIFI Rx stream {} arguments.", stream_id);
      return;
    }

    const auto tx_ports = resolve_difi_channel_ports(tx_stream, tx_parsed->port, stream_id);
    if (!tx_ports.has_value() || tx_ports->empty()) {
      logger.error("Failed to resolve the DIFI Tx channel ports of stream {}.", stream_id);
      return;
    }

    const auto rx_ports = resolve_difi_channel_ports(rx_stream, rx_parsed->port, stream_id);
    if (!rx_ports.has_value() || rx_ports->empty()) {
      logger.error("Failed to resolve the DIFI Rx channel ports of stream {}.", stream_id);
      return;
    }

    // Advertise the configured centre frequency and gains in the context packets. set_tx_freq() and set_tx_gain()
    // update them later, but nothing calls those during start-up.
    const double tx_center_freq_Hz = tx_stream.channels.front().freq.center_frequency_Hz;

    radio_difi_tx_stream::stream_description tx_config = {.ip             = tx_parsed->ip,
                                                          .channels       = {},
                                                          .stream_id      = stream_id,
                                                          .bit_depth      = bit_depth,
                                                          .iq_byte_order  = iq_byte_order,
                                                          .sample_rate_Hz = config.sampling_rate_Hz,
                                                          .center_freq_Hz = tx_center_freq_Hz,
                                                          .stream_id_str  = "difi:tx:" + std::to_string(stream_id),
                                                          .log_level      = config.log_level};

    for (unsigned channel_id = 0, nof_channels = tx_stream.channels.size(); channel_id != nof_channels; ++channel_id) {
      const radio_difi_tx_stream::channel_description& channel = tx_config.channels.emplace_back(
          radio_difi_tx_stream::channel_description{.port           = (*tx_ports)[channel_id],
                                                    .difi_stream_id = tx_difi_stream_id++,
                                                    .rf_gain_dB     = tx_stream.channels[channel_id].gain_dB});
      tx_port_map.emplace_back(stream_id, channel_id);

      logger.info("stream={} channel={}: Creating DIFI transmit channel. port={} difi_stream_id={} gain={}dB",
                  stream_id,
                  channel_id,
                  channel.port,
                  channel.difi_stream_id,
                  channel.rf_gain_dB);
    }

    radio_difi_rx_stream::stream_description rx_config = {.ip             = rx_parsed->ip,
                                                          .channels       = {},
                                                          .stream_id      = stream_id,
                                                          .bit_depth      = bit_depth,
                                                          .iq_byte_order  = iq_byte_order,
                                                          .sample_rate_Hz = config.sampling_rate_Hz,
                                                          .stream_id_str  = "difi:rx:" + std::to_string(stream_id),
                                                          .log_level      = config.log_level};

    for (unsigned channel_id = 0, nof_channels = rx_stream.channels.size(); channel_id != nof_channels; ++channel_id) {
      const radio_difi_rx_stream::channel_description& channel =
          rx_config.channels.emplace_back(radio_difi_rx_stream::channel_description{
              .port = (*rx_ports)[channel_id], .difi_stream_id = rx_difi_stream_id++});

      logger.info("stream={} channel={}: Creating DIFI receive channel. port={} difi_stream_id={}",
                  stream_id,
                  channel_id,
                  channel.port,
                  channel.difi_stream_id);
    }

    logger.info("stream={}: Creating DIFI stream. tx_ip={} rx_ip={} bit_depth={} iq_byte_order={} srate={}Hz freq={}Hz",
                stream_id,
                tx_config.ip,
                rx_config.ip,
                bit_depth,
                (iq_byte_order == difi_iq_byte_order::big) ? "big" : "little",
                config.sampling_rate_Hz,
                tx_center_freq_Hz);

    auto& gateway =
        bb_gateways.emplace_back(std::make_unique<radio_difi_baseband_gateway>(notifier, tx_config, rx_config));

    if (!gateway->is_successful()) {
      logger.error("Failed to create DIFI baseband gateway for stream {}.", stream_id);
      return;
    }
  }

  successful = true;
}

baseband_gateway& radio_session_difi_impl::get_baseband_gateway(unsigned stream_id)
{
  ocudu_assert(stream_id < bb_gateways.size(),
               "Stream identifier (i.e., {}) exceeds the number of baseband gateways (i.e., {}).",
               stream_id,
               bb_gateways.size());
  return *bb_gateways[stream_id];
}

baseband_gateway_timestamp radio_session_difi_impl::read_current_time()
{
  if (bb_gateways.empty()) {
    return 0;
  }
  return bb_gateways[0]->get_rx_stream().get_sample_count();
}

/// \brief Offset in sample ticks that maps the baseband timeline onto UTC.
///
/// Integer arithmetic throughout: at epoch scale the tick count exceeds the 53-bit mantissa of a
/// double, and a rounding error there is a lost sample.
static int64_t utc_epoch_offset_ticks(baseband_gateway_timestamp init_time, double sample_rate_Hz)
{
  if (sample_rate_Hz <= 0.0) {
    return 0;
  }

  const auto     now   = std::chrono::system_clock::now().time_since_epoch();
  const auto     ns    = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now).count());
  const uint64_t srate = static_cast<uint64_t>(sample_rate_Hz);
  const uint64_t secs  = ns / 1000000000ULL;
  const uint64_t rem   = ns % 1000000000ULL;

  return static_cast<int64_t>(secs * srate + (rem * srate) / 1000000000ULL) - static_cast<int64_t>(init_time);
}

void radio_session_difi_impl::start(baseband_gateway_timestamp init_time)
{
  // DIFI timestamps are absolute UTC. One offset, read once, maps the baseband timeline onto UTC: transmit adds it
  // and receive subtracts it, so both directions stay consistent.
  const int64_t epoch_offset = utc_epoch_offset_ticks(init_time, sample_rate_Hz);

  for (auto& gateway : bb_gateways) {
    gateway->get_rx_stream().start(init_time, epoch_offset);
  }
  for (auto& gateway : bb_gateways) {
    gateway->get_tx_stream().start(init_time, epoch_offset);
  }
}

void radio_session_difi_impl::stop()
{
  for (auto& gateway : bb_gateways) {
    gateway->get_tx_stream().stop();
  }
  for (auto& gateway : bb_gateways) {
    gateway->get_rx_stream().stop();
  }
}

bool radio_session_difi_impl::set_tx_gain(unsigned port_id, double gain_dB)
{
  if (port_id >= tx_port_map.size()) {
    return false;
  }

  const auto [stream_id, channel_id] = tx_port_map[port_id];
  return bb_gateways[stream_id]->get_tx_stream().set_gain(channel_id, gain_dB, 0.0);
}

bool radio_session_difi_impl::set_tx_freq(unsigned stream_id, double center_freq_Hz)
{
  if (stream_id >= bb_gateways.size()) {
    return false;
  }
  bb_gateways[stream_id]->get_tx_stream().set_freq(center_freq_Hz);
  return true;
}
