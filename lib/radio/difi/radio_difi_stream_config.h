// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "radio_difi_data_packet.h"
#include "ocudu/adt/span.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/radio/radio_configuration.h"
#include "ocudu/radio/radio_constants.h"
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace ocudu {

/// \brief Connection parameters for one stream direction, parsed from the stream arguments.
///
/// A comma-separated key=value list, "ip=1.2.3.4,port=4991". Tx names the destination, Rx the bind.
struct radio_difi_stream_config {
  /// IP address string (destination for Tx, bind address for Rx).
  std::string ip;
  /// UDP port (destination port for Tx, bind port for Rx).
  uint16_t port;
};

/// \brief Parse a DIFI stream args string into a radio_difi_stream_config.
///
/// \return The parsed configuration, or std::nullopt if \p args is malformed.
std::optional<radio_difi_stream_config> parse_difi_stream_args(const std::string& args,
                                                               const std::string& default_ip   = "127.0.0.1",
                                                               uint16_t           default_port = 4991);

/// \brief Resolves the UDP port of every channel of a DIFI stream.
///
/// Each channel names its port in its own arguments, as in \c port=4993. The first channel may leave them empty and
/// then takes \p stream_port, the port of the stream arguments, so a single-channel configuration needs no channel
/// arguments. \p stream_id only labels the error messages.
///
/// \return One port per channel, or std::nullopt if the channel arguments are malformed or a channel other than the
/// first has no port.
std::optional<static_vector<uint16_t, RADIO_MAX_NOF_CHANNELS>>
resolve_difi_channel_ports(const radio_configuration::stream& stream, uint16_t stream_port, unsigned stream_id);

/// \brief Checks that no UDP port appears twice among \p ports.
///
/// \p direction, "transmit" or "receive", only labels the error message.
bool has_unique_difi_ports(span<const uint16_t> ports, std::string_view direction);

/// \brief Parses the IQ byte order from the radio device arguments.
///
/// Looks for \c iq_byte_order=big or \c iq_byte_order=little in the comma-separated \p device_args, ignoring every
/// other key. Absent means \c big, the VITA-49.2 network byte order.
///
/// \return The byte order, or std::nullopt if the key has any other value.
std::optional<difi_iq_byte_order> parse_difi_iq_byte_order(const std::string& device_args);

/// \brief Parses the number of IQ samples per transmitted data packet from the radio device arguments.
///
/// Looks for \c pkt_samples=N in the comma-separated \p device_args, ignoring every other key. Absent means
/// DIFI_DEFAULT_PKT_SAMPLES.
///
/// \return The number of samples, or std::nullopt if the value is not an integer in [1, DIFI_MAX_PKT_SAMPLES].
std::optional<unsigned> parse_difi_pkt_samples(const std::string& device_args);

/// [Implementation-defined] Default lag of the receive timeline behind the transmitter.
constexpr std::chrono::microseconds DIFI_DEFAULT_RX_LAG{2000};

/// [Implementation-defined] Largest receive lag accepted.
constexpr std::chrono::microseconds DIFI_MAX_RX_LAG{20000};

/// \brief Parses the receive lag from the radio device arguments.
///
/// Looks for \c rx_lag_us=N in the comma-separated \p device_args, in microseconds, ignoring every other key. Absent
/// means DIFI_DEFAULT_RX_LAG.
///
/// \return The lag, or std::nullopt if the value is not an integer in [0, DIFI_MAX_RX_LAG].
std::optional<std::chrono::microseconds> parse_difi_rx_lag(const std::string& device_args);

} // namespace ocudu
