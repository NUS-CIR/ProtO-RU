// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_difi_stream_config.h"
#include "ocudu/support/string_parsing_utils.h"
#include "fmt/base.h"
#include <arpa/inet.h>
#include <set>
#include <sstream>
#include <string_view>

using namespace ocudu;

/// Highest valid UDP port number.
static constexpr int MAX_UDP_PORT = 65535;

/// Parses a UDP port number from \p value. Prints the reason and returns std::nullopt if it is not valid.
static std::optional<uint16_t> parse_udp_port(const std::string& value)
{
  // parse_int() rejects trailing characters, so no separate check is needed.
  const expected<int, std::string> port = parse_int<int>(value);
  if (!port.has_value()) {
    fmt::print("Stream port '{}' is not a valid integer.\n", value);
    return std::nullopt;
  }
  if ((port.value() < 1) || (port.value() > MAX_UDP_PORT)) {
    fmt::print("Stream port {} is out of valid range [1, {}].\n", port.value(), MAX_UDP_PORT);
    return std::nullopt;
  }
  return static_cast<uint16_t>(port.value());
}

std::optional<radio_difi_stream_config>
ocudu::parse_difi_stream_args(const std::string& args, const std::string& default_ip, uint16_t default_port)
{
  radio_difi_stream_config config = {default_ip, default_port};

  if (args.empty()) {
    return config;
  }

  std::istringstream ss(args);
  std::string        token;

  while (std::getline(ss, token, ',')) {
    // Skip empty tokens caused by trailing commas.
    if (token.empty()) {
      continue;
    }

    const auto eq = token.find('=');
    if (eq == std::string::npos) {
      fmt::print("Stream argument '{}' is malformed, expected key=value.\n", token);
      return std::nullopt;
    }

    const std::string key = token.substr(0, eq);
    const std::string val = token.substr(eq + 1);

    if (key == "ip") {
      // Validate as a dotted-decimal IPv4 address.
      struct in_addr addr = {};
      if (::inet_pton(AF_INET, val.c_str(), &addr) != 1) {
        fmt::print("Invalid IP address '{}' in stream arguments.\n", val);
        return std::nullopt;
      }
      config.ip = val;

    } else if (key == "port") {
      const std::optional<uint16_t> port = parse_udp_port(val);
      if (!port.has_value()) {
        return std::nullopt;
      }
      config.port = *port;

    } else {
      fmt::print("Unknown stream argument key '{}'.\n", key);
      return std::nullopt;
    }
  }

  return config;
}

std::optional<difi_iq_byte_order> ocudu::parse_difi_iq_byte_order(const std::string& device_args)
{
  static constexpr std::string_view IQ_BYTE_ORDER_KEY = "iq_byte_order";
  static constexpr std::string_view WHITESPACE        = " \t\n";

  std::istringstream ss(device_args);
  std::string        token;
  while (std::getline(ss, token, ',')) {
    const auto eq = token.find('=');
    if (eq == std::string::npos) {
      continue;
    }

    // Device arguments may be folded across lines in the configuration file, so surrounding whitespace is ignored.
    std::string key   = token.substr(0, eq);
    std::string value = token.substr(eq + 1);
    key.erase(0, key.find_first_not_of(WHITESPACE));
    key.erase(key.find_last_not_of(WHITESPACE) + 1);
    value.erase(0, value.find_first_not_of(WHITESPACE));
    value.erase(value.find_last_not_of(WHITESPACE) + 1);
    if (key != IQ_BYTE_ORDER_KEY) {
      continue;
    }

    if (value == "big") {
      return difi_iq_byte_order::big;
    }
    if (value == "little") {
      return difi_iq_byte_order::little;
    }
    return std::nullopt;
  }

  return difi_iq_byte_order::big;
}

std::optional<static_vector<uint16_t, RADIO_MAX_NOF_CHANNELS>>
ocudu::resolve_difi_channel_ports(const radio_configuration::stream& stream, uint16_t stream_port, unsigned stream_id)
{
  static constexpr std::string_view PORT_KEY = "port";

  static_vector<uint16_t, RADIO_MAX_NOF_CHANNELS> ports;
  if (stream.channels.size() > ports.capacity()) {
    fmt::print("DIFI stream {} has {} channels, at most {} are supported.\n",
               stream_id,
               stream.channels.size(),
               ports.capacity());
    return std::nullopt;
  }

  for (unsigned channel_id = 0, nof_channels = stream.channels.size(); channel_id != nof_channels; ++channel_id) {
    std::optional<uint16_t> port;

    std::istringstream ss(stream.channels[channel_id].args);
    std::string        token;
    while (std::getline(ss, token, ',')) {
      // Skip empty tokens caused by trailing commas.
      if (token.empty()) {
        continue;
      }

      const auto eq = token.find('=');
      if (eq == std::string::npos) {
        fmt::print("Channel argument '{}' is malformed, expected key=value.\n", token);
        return std::nullopt;
      }

      const std::string key = token.substr(0, eq);
      if (key != PORT_KEY) {
        fmt::print("Unknown channel argument key '{}'.\n", key);
        return std::nullopt;
      }

      port = parse_udp_port(token.substr(eq + 1));
      if (!port.has_value()) {
        return std::nullopt;
      }
    }

    // The first channel falls back on the stream port, so a single-channel configuration needs no channel arguments.
    if (!port.has_value() && (channel_id == 0)) {
      port = stream_port;
    }
    if (!port.has_value()) {
      fmt::print("Channel {} of DIFI stream {} has no port, expected port=<number> in its arguments.\n",
                 channel_id,
                 stream_id);
      return std::nullopt;
    }

    ports.push_back(*port);
  }

  return ports;
}

bool ocudu::has_unique_difi_ports(span<const uint16_t> ports, std::string_view direction)
{
  std::set<uint16_t> seen;
  for (uint16_t port : ports) {
    if (!seen.insert(port).second) {
      fmt::print("DIFI {} port {} is used by more than one channel.\n", direction, port);
      return false;
    }
  }

  return true;
}
