// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_config_difi_validator.h"
#include "radio_difi_stream_config.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/radio/radio_constants.h"
#include "fmt/base.h"
#include <cmath>

using namespace ocudu;

static bool validate_sampling_rate(double sampling_rate)
{
  if (!std::isnormal(sampling_rate)) {
    fmt::print("The sampling rate must be non-zero, NAN nor infinite.\n");
    return false;
  }

  if (sampling_rate < 0.0) {
    fmt::print("The sampling rate must be greater than zero.\n");
    return false;
  }

  return true;
}

static bool validate_otw_format(radio_configuration::over_the_wire_format otw_format)
{
  if (otw_format != radio_configuration::over_the_wire_format::DEFAULT &&
      otw_format != radio_configuration::over_the_wire_format::SC8 &&
      otw_format != radio_configuration::over_the_wire_format::SC16) {
    fmt::print("Only DEFAULT, SC8 and SC16 OTW formats are supported by the DIFI radio.\n");
    return false;
  }

  return true;
}

/// Validates the transmit streams and the ports of their channels.
static bool validate_tx_streams(span<const radio_configuration::stream> tx_streams)
{
  static_vector<uint16_t, RADIO_MAX_NOF_PORTS> ports;

  for (unsigned stream_id = 0, nof_streams = tx_streams.size(); stream_id != nof_streams; ++stream_id) {
    const radio_configuration::stream& tx_stream = tx_streams[stream_id];

    if (tx_stream.channels.empty()) {
      fmt::print("Every DIFI transmit stream needs at least one channel.\n");
      return false;
    }

    // Default Tx: send to localhost:4991.
    const std::optional<radio_difi_stream_config> parsed = parse_difi_stream_args(tx_stream.args, "127.0.0.1", 4991);
    if (!parsed.has_value()) {
      return false;
    }

    const auto channel_ports = resolve_difi_channel_ports(tx_stream, parsed->port, stream_id);
    if (!channel_ports.has_value()) {
      return false;
    }
    if (ports.size() + channel_ports->size() > ports.capacity()) {
      fmt::print("DIFI takes at most {} transmit ports.\n", ports.capacity());
      return false;
    }
    for (uint16_t port : *channel_ports) {
      ports.push_back(port);
    }
  }

  return has_unique_difi_ports(ports, "transmit");
}

static bool validate_rx_stream_args(const std::string& args)
{
  // Default Rx: bind on all interfaces, port 4992.
  return parse_difi_stream_args(args, "0.0.0.0", 4992).has_value();
}

bool radio_config_difi_validator::is_configuration_valid(const radio_configuration::radio& config) const
{
  if (config.tx_streams.size() != config.rx_streams.size()) {
    fmt::print("Transmit and receive number of streams must be equal.\n");
    return false;
  }

  if (config.tx_streams.empty()) {
    fmt::print("At least one transmit and one receive stream must be configured.\n");
    return false;
  }

  if (!validate_tx_streams(config.tx_streams)) {
    return false;
  }

  for (const radio_configuration::stream& rx_stream : config.rx_streams) {
    if (!validate_rx_stream_args(rx_stream.args)) {
      return false;
    }
  }

  if (!validate_sampling_rate(config.sampling_rate_Hz)) {
    return false;
  }

  if (!validate_otw_format(config.otw_format)) {
    return false;
  }

  return true;
}
