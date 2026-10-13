// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/complex.h"
#include "ocudu/adt/span.h"
#include "ocudu/support/units.h"
#include <cstdint>

namespace ocudu {

/// Fixed size of the DIFI data packet header in bytes.
constexpr units::bytes DIFI_DATA_HEADER_SIZE{28};

/// Number of payload bytes per 16-bit IQ sample, two per component.
constexpr size_t DIFI_BYTES_PER_IQ16_SAMPLE = 4;

/// Largest UDP payload, and so the largest DIFI packet.
constexpr units::bytes DIFI_MAX_UDP_PAYLOAD{65507};

/// \brief [Implementation-defined] Default number of IQ samples per transmitted data packet.
///
/// A 7708-byte packet at 16-bit fits a 9000-byte receive buffer, and 1920 samples divide every slot.
constexpr unsigned DIFI_DEFAULT_PKT_SAMPLES = 1920;

/// Largest number of IQ samples a transmitted data packet may carry, at the widest (16-bit) sample format.
constexpr unsigned DIFI_MAX_PKT_SAMPLES =
    (DIFI_MAX_UDP_PAYLOAD.value() - DIFI_DATA_HEADER_SIZE.value()) / DIFI_BYTES_PER_IQ16_SAMPLE;

/// \brief Byte order of the 16-bit IQ components in a DIFI data packet payload.
enum class difi_iq_byte_order { big, little };

/// Parameters for the fixed DIFI data packet header fields.
struct difi_data_packet_params {
  /// Stream identifier.
  uint32_t stream_id;
  /// Timestamp whole seconds.
  uint32_t full_secs;
  /// Timestamp sub-second part in picoseconds.
  uint64_t frac_ps;
  /// IQ sample bit depth, either 8 or 16.
  unsigned bit_depth;
  /// Data packet sequence number, a mod-16 counter.
  uint8_t pkt_n;
  /// Byte order of the 16-bit IQ components.
  difi_iq_byte_order iq_byte_order = difi_iq_byte_order::big;
};

/// \brief Writes a DIFI data packet into \p buf and returns the total byte count.
///
/// A 28-byte big-endian header, then interleaved IQ in the byte order of \c p.iq_byte_order, zero-padded to a
/// 32-bit word.
///
/// \p buf must hold at least difi_data_packet_size() bytes, and the packet must not exceed the 65535-word
/// maximum the VRT size field can express; either violation trips an assertion.
units::bytes build_difi_data_packet(span<uint8_t> buf, const difi_data_packet_params& p, span<const ci16_t> samples);

/// \brief Serializes 16-bit IQ samples into a data packet payload in the given byte order.
///
/// \param [out] payload Payload bytes, four per sample.
/// \param [in]  samples IQ samples.
/// \param [in]  order   Byte order of each 16-bit component on the wire.
void difi_pack_iq16(span<uint8_t> payload, span<const ci16_t> samples, difi_iq_byte_order order);

/// \brief Deserializes 16-bit IQ samples from a data packet payload in the given byte order.
///
/// \param [out] samples IQ samples.
/// \param [in]  payload Payload bytes, four per sample.
/// \param [in]  order   Byte order of each 16-bit component on the wire.
void difi_unpack_iq16(span<ci16_t> samples, span<const uint8_t> payload, difi_iq_byte_order order);

/// Returns the packet byte count for \p nof_samples, always a multiple of 4.
units::bytes difi_data_packet_size(unsigned bit_depth, unsigned nof_samples);

/// \brief Converts a sample-tick timestamp into the DIFI whole seconds and picoseconds pair.
///
/// Integer arithmetic: a double loses precision at Unix-epoch tick counts, costing several samples.
void difi_ticks_to_time(uint64_t ticks, double sample_rate_Hz, uint32_t& full_secs, uint64_t& frac_ps);

/// Inverse of difi_ticks_to_time(), round-tripping its output exactly.
uint64_t difi_time_to_ticks(uint32_t full_secs, uint64_t frac_ps, double sample_rate_Hz);

} // namespace ocudu
