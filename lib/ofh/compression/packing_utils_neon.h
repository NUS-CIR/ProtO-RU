// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief NEON packing and unpacking of IQ samples in big-endian format.
///
/// A NEON register holds 8 16bit samples, which pack into exactly \c BITS bytes when each sample takes \c BITS bits.
/// The byte shuffles, shifts and masks of every bit width are generated at compile time from the position of each
/// sample in the packed bytes.

#pragma once

#include "ocudu/support/compiler.h"
#include <arm_neon.h>
#include <array>
#include <cstdint>

namespace ocudu {
namespace ofh {
namespace neon {

/// Byte shuffle table of a table lookup.
using shuffle_table = std::array<uint8_t, 16>;

/// Byte index that reads as zero in a table lookup.
constexpr uint8_t SHUFFLE_ZERO = 0xff;

/// \brief Positions of the 8 samples of one NEON register inside of their packed bytes.
///
/// The samples are packed with \c BITS bits each, most significant bit first, in the order of the lanes.
template <unsigned BITS>
struct packed_register_layout {
  static_assert((BITS >= 8) && (BITS <= 16), "Unsupported bit width");

  /// Number of packed bytes of the register.
  static constexpr unsigned NOF_BYTES = BITS;
  /// Number of samples of the register.
  static constexpr unsigned NOF_SAMPLES = 8;

  /// Position of a packed sample.
  struct sample_position {
    /// Index of the first byte holding bits of the sample.
    unsigned first_byte;
    /// Number of bytes holding bits of the sample.
    unsigned nof_bytes;
    /// Offset of the most significant bit of the sample inside of the first byte.
    unsigned bit_offset;
  };

  /// Returns the position of the given sample.
  static constexpr sample_position locate(unsigned sample)
  {
    unsigned bit = sample * BITS;
    return {bit / 8, (bit % 8 + BITS + 7) / 8, bit % 8};
  }

  /// Returns true if some sample does not start at a byte boundary.
  static constexpr bool has_bit_offsets()
  {
    for (unsigned sample = 0; sample != NOF_SAMPLES; ++sample) {
      if (locate(sample).bit_offset != 0) {
        return true;
      }
    }
    return false;
  }

  /// \brief Whether the samples are packed from 16bit lanes.
  ///
  /// A sample and the bits that precede it in its first byte fit in 16 bits up to 9 bits, and the 16bit samples are
  /// byte aligned. The other widths spread the samples over 32bit lanes.
  static constexpr bool PACK_16BIT_LANES = (BITS <= 9) || (BITS == 16);

  /// \brief Whether the samples are unpacked through 16bit lanes.
  ///
  /// A sample and the bits that precede it in its first byte fit in 16 bits up to 9 bits, and the 16bit samples are
  /// byte aligned. The other widths go through 32bit lanes.
  static constexpr bool UNPACK_16BIT_LANES = (BITS <= 9) || (BITS == 16);

  /// Whether the unpacked samples carry bits of the next sample below them, see \ref unpack_register.
  static constexpr bool UNPACK_HAS_TAIL_BITS = (BITS % 8 != 0);
};

/// \brief Makes the shuffle tables packing a register of 16bit lanes.
///
/// Each lane holds its sample shifted to the position it takes in the two bytes it spans. The first table gathers the
/// most significant byte of each lane into the first byte of its sample, the second one the least significant byte
/// into the next byte. The second table is empty if the samples are byte aligned.
template <unsigned BITS>
constexpr std::array<shuffle_table, 2> make_pack_tables_16b()
{
  using layout = packed_register_layout<BITS>;

  std::array<shuffle_table, 2> tables = {};
  for (shuffle_table& table : tables) {
    for (uint8_t& index : table) {
      index = SHUFFLE_ZERO;
    }
  }
  for (unsigned sample = 0; sample != layout::NOF_SAMPLES; ++sample) {
    typename layout::sample_position position = layout::locate(sample);
    tables[0][position.first_byte]            = 2 * sample + 1;
    if (position.nof_bytes == 2) {
      tables[1][position.first_byte + 1] = 2 * sample;
    }
  }
  return tables;
}

/// Returns true if the two tables never write the same byte, so that they merge into a single one.
constexpr bool are_disjoint(const std::array<shuffle_table, 2>& tables)
{
  for (unsigned byte = 0; byte != 16; ++byte) {
    if ((tables[0][byte] != SHUFFLE_ZERO) && (tables[1][byte] != SHUFFLE_ZERO)) {
      return false;
    }
  }
  return true;
}

/// Merges two disjoint tables.
constexpr shuffle_table merge_tables(const std::array<shuffle_table, 2>& tables)
{
  shuffle_table merged = tables[0];
  for (unsigned byte = 0; byte != 16; ++byte) {
    if (tables[1][byte] != SHUFFLE_ZERO) {
      merged[byte] = tables[1][byte];
    }
  }
  return merged;
}

/// Makes the left shifts aligning each 16bit lane with the position of its sample, see \ref make_pack_tables_16b.
template <unsigned BITS>
constexpr std::array<int16_t, 8> make_pack_shifts_16b()
{
  using layout = packed_register_layout<BITS>;

  std::array<int16_t, 8> shifts = {};
  for (unsigned sample = 0; sample != layout::NOF_SAMPLES; ++sample) {
    shifts[sample] = static_cast<int16_t>(16 - BITS - layout::locate(sample).bit_offset);
  }
  return shifts;
}

/// Makes the masks keeping the \c BITS bits of each shifted 16bit lane.
template <unsigned BITS>
constexpr std::array<int16_t, 8> make_pack_masks_16b()
{
  std::array<int16_t, 8> shifts = make_pack_shifts_16b<BITS>();
  std::array<int16_t, 8> masks  = {};
  for (unsigned lane = 0; lane != 8; ++lane) {
    masks[lane] = static_cast<int16_t>(static_cast<uint16_t>(((1U << BITS) - 1U) << shifts[lane]));
  }
  return masks;
}

/// \brief Makes the shuffle tables packing a register whose samples are spread over two registers of 32bit lanes.
///
/// The first register holds the first four samples and the second one the others, each one shifted to the position
/// it takes in the three bytes it spans, which are the three least significant bytes of its lane. A packed byte takes
/// bits from up to two samples, the first table gathers the first contribution and the second one the other.
template <unsigned BITS>
constexpr std::array<shuffle_table, 2> make_pack_tables_32b()
{
  using layout = packed_register_layout<BITS>;

  std::array<shuffle_table, 2> tables = {};
  for (shuffle_table& table : tables) {
    for (uint8_t& index : table) {
      index = SHUFFLE_ZERO;
    }
  }
  for (unsigned sample = 0; sample != layout::NOF_SAMPLES; ++sample) {
    typename layout::sample_position position = layout::locate(sample);
    for (unsigned byte = 0; byte != position.nof_bytes; ++byte) {
      // Most significant byte of the sample first, at the top of the three bytes of its lane.
      uint8_t  source                                         = 4 * sample + (2 - byte);
      unsigned dest                                           = position.first_byte + byte;
      tables[(tables[0][dest] == SHUFFLE_ZERO) ? 0 : 1][dest] = source;
    }
  }
  return tables;
}

/// Makes the powers of two moving each sample to the position it takes in its 32bit lane, see \ref
/// make_pack_tables_32b.
template <unsigned BITS>
constexpr std::array<int16_t, 8> make_pack_multipliers_32b()
{
  using layout = packed_register_layout<BITS>;

  std::array<int16_t, 8> multipliers = {};
  for (unsigned sample = 0; sample != layout::NOF_SAMPLES; ++sample) {
    multipliers[sample] = static_cast<int16_t>(1 << (24 - BITS - layout::locate(sample).bit_offset));
  }
  return multipliers;
}

/// \brief Returns the per lane shift counts that the exponent is subtracted from before \ref pack_register.
///
/// They are the left shifts of the 16bit lanes, or zero when the samples are packed from 32bit lanes, so that in both
/// cases the register is shifted by the counts minus the exponent.
template <unsigned BITS>
OCUDU_FORCE_INLINE int16x8_t get_pack_shift_counts()
{
  if constexpr (packed_register_layout<BITS>::PACK_16BIT_LANES) {
    static constexpr std::array<int16_t, 8> SHIFTS = make_pack_shifts_16b<BITS>();
    return vld1q_s16(SHIFTS.data());
  }
  return vdupq_n_s16(0);
}

/// \brief Packs the 8 samples of a register with \c BITS bits each, most significant bit first.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_EXPONENT   Whether the samples are shifted right by a BFP exponent before packing.
/// \param[in] samples_s16 NEON register storing the samples.
/// \param[in] shift_s16   NEON register storing \ref get_pack_shift_counts minus the exponent in every lane, unused if
///                        there is no exponent.
/// \return A NEON register holding the \c BITS packed bytes in its first bytes, the rest are zero.
template <unsigned BITS, bool HAS_EXPONENT>
OCUDU_FORCE_INLINE uint8x16_t pack_register(int16x8_t samples_s16, int16x8_t shift_s16)
{
  using layout = packed_register_layout<BITS>;

  if constexpr (layout::PACK_16BIT_LANES) {
    static constexpr std::array<shuffle_table, 2> TABLES = make_pack_tables_16b<BITS>();
    static constexpr bool                         SINGLE = are_disjoint(TABLES);
    static constexpr shuffle_table                MERGED = merge_tables(TABLES);
    static constexpr std::array<int16_t, 8>       SHIFTS = make_pack_shifts_16b<BITS>();
    static constexpr std::array<int16_t, 8>       MASKS  = make_pack_masks_16b<BITS>();

    // Shift each sample to its position. A shift right by the exponent drops the low bits, and a shift left keeps the
    // sign extension and, with an exponent, the low bits above or below the sample: the mask clears them.
    if constexpr (HAS_EXPONENT) {
      samples_s16 = vshlq_s16(samples_s16, shift_s16);
    } else if constexpr (BITS != 16) {
      samples_s16 = vshlq_s16(samples_s16, vld1q_s16(SHIFTS.data()));
    }
    if constexpr (HAS_EXPONENT || layout::has_bit_offsets()) {
      samples_s16 = vandq_s16(samples_s16, vld1q_s16(MASKS.data()));
    }

    uint8x16_t bytes_u8 = vreinterpretq_u8_s16(samples_s16);
    if constexpr (SINGLE) {
      return vqtbl1q_u8(bytes_u8, vld1q_u8(MERGED.data()));
    } else {
      return vorrq_u8(vqtbl1q_u8(bytes_u8, vld1q_u8(TABLES[0].data())),
                      vqtbl1q_u8(bytes_u8, vld1q_u8(TABLES[1].data())));
    }
  } else {
    static constexpr std::array<shuffle_table, 2> TABLES      = make_pack_tables_32b<BITS>();
    static constexpr std::array<int16_t, 8>       MULTIPLIERS = make_pack_multipliers_32b<BITS>();

    // Shift right by the exponent and keep the data bits, the widening multiplication then moves them into place.
    if constexpr (HAS_EXPONENT) {
      samples_s16 = vshlq_s16(samples_s16, shift_s16);
    }
    samples_s16 = vandq_s16(samples_s16, vdupq_n_s16(static_cast<int16_t>((1U << BITS) - 1U)));

    int16x8_t    multipliers_s16 = vld1q_s16(MULTIPLIERS.data());
    uint8x16x2_t bytes_u8x2;
    bytes_u8x2.val[0] = vreinterpretq_u8_s32(vmull_s16(vget_low_s16(samples_s16), vget_low_s16(multipliers_s16)));
    bytes_u8x2.val[1] = vreinterpretq_u8_s32(vmull_high_s16(samples_s16, multipliers_s16));
    return vorrq_u8(vqtbl2q_u8(bytes_u8x2, vld1q_u8(TABLES[0].data())),
                    vqtbl2q_u8(bytes_u8x2, vld1q_u8(TABLES[1].data())));
  }
}

/// Stores the first \c NOF_BYTES bytes of a register, from 8 to 16.
template <unsigned NOF_BYTES>
OCUDU_FORCE_INLINE void store_bytes(uint8_t* out, uint8x16_t bytes_u8)
{
  static_assert((NOF_BYTES >= 8) && (NOF_BYTES <= 16), "Unsupported number of bytes");

  if constexpr (NOF_BYTES == 16) {
    vst1q_u8(out, bytes_u8);
    return;
  }
  vst1_u8(out, vget_low_u8(bytes_u8));
  if constexpr ((NOF_BYTES & 4) != 0) {
    vst1q_lane_u32(reinterpret_cast<uint32_t*>(out + 8), vreinterpretq_u32_u8(bytes_u8), 2);
  }
  if constexpr ((NOF_BYTES & 2) != 0) {
    static constexpr unsigned OFFSET = 8 + (NOF_BYTES & 4);
    vst1q_lane_u16(reinterpret_cast<uint16_t*>(out + OFFSET), vreinterpretq_u16_u8(bytes_u8), OFFSET / 2);
  }
  if constexpr ((NOF_BYTES & 1) != 0) {
    static constexpr unsigned OFFSET = 8 + (NOF_BYTES & 6);
    vst1q_lane_u8(out + OFFSET, bytes_u8, OFFSET);
  }
}

/// \brief Makes the shuffle table unpacking the 8 samples of a register into 16bit lanes.
///
/// Each lane takes the two bytes of its sample in big-endian, so that the sample starts at its bit offset from the top.
template <unsigned BITS, unsigned OFFSET>
constexpr shuffle_table make_unpack_table_16b()
{
  using layout = packed_register_layout<BITS>;

  shuffle_table table = {};
  for (unsigned sample = 0; sample != layout::NOF_SAMPLES; ++sample) {
    typename layout::sample_position position = layout::locate(sample);
    table[2 * sample + 1]                     = OFFSET + position.first_byte;
    table[2 * sample] = (position.nof_bytes == 2) ? (OFFSET + position.first_byte + 1) : SHUFFLE_ZERO;
  }
  return table;
}

/// \brief Makes the shuffle table unpacking 4 samples of a register into 32bit lanes.
///
/// Each lane takes the up to three bytes of its sample in big-endian at its top, so that the sample starts at its bit
/// offset from the top, and zeros below them.
template <unsigned BITS, unsigned OFFSET, unsigned FIRST_SAMPLE>
constexpr shuffle_table make_unpack_table_32b()
{
  using layout = packed_register_layout<BITS>;

  shuffle_table table = {};
  for (unsigned lane = 0; lane != 4; ++lane) {
    typename layout::sample_position position = layout::locate(FIRST_SAMPLE + lane);
    for (unsigned byte = 0; byte != 4; ++byte) {
      table[4 * lane + 3 - byte] = (byte < position.nof_bytes) ? (OFFSET + position.first_byte + byte) : SHUFFLE_ZERO;
    }
  }
  return table;
}

/// Makes the left shifts dropping the bits that precede each sample in its lane.
template <unsigned BITS, unsigned FIRST_SAMPLE, unsigned NOF_LANES, typename Int>
constexpr std::array<Int, NOF_LANES> make_unpack_shifts()
{
  std::array<Int, NOF_LANES> shifts = {};
  for (unsigned lane = 0; lane != NOF_LANES; ++lane) {
    shifts[lane] = static_cast<Int>(packed_register_layout<BITS>::locate(FIRST_SAMPLE + lane).bit_offset);
  }
  return shifts;
}

/// \brief Unpacks the 8 samples of a register from the given bytes to the top of 16bit lanes.
///
/// Each lane holds its sample multiplied by two to the power of \c 16 - \c BITS. If \c BITS is not a multiple of 8,
/// the bits below the sample belong to the next sample, see \c packed_register_layout::UNPACK_HAS_TAIL_BITS.
///
/// \tparam BITS        Bit width of the packed samples.
/// \tparam OFFSET      Offset of the first packed byte of the register inside of the given bytes.
/// \param[in] bytes_u8 NEON register storing 16 bytes that include the \c BITS packed bytes of the register.
/// \return A NEON register storing the samples.
template <unsigned BITS, unsigned OFFSET>
OCUDU_FORCE_INLINE int16x8_t unpack_register(uint8x16_t bytes_u8)
{
  using layout = packed_register_layout<BITS>;
  static_assert(OFFSET + BITS <= 16, "The packed bytes exceed the loaded ones");

  if constexpr (layout::UNPACK_16BIT_LANES) {
    static constexpr shuffle_table          TABLE  = make_unpack_table_16b<BITS, OFFSET>();
    static constexpr std::array<int16_t, 8> SHIFTS = make_unpack_shifts<BITS, 0, 8, int16_t>();

    int16x8_t samples_s16 = vreinterpretq_s16_u8(vqtbl1q_u8(bytes_u8, vld1q_u8(TABLE.data())));
    if constexpr (layout::has_bit_offsets()) {
      // Drop the bits of the previous sample.
      samples_s16 = vshlq_s16(samples_s16, vld1q_s16(SHIFTS.data()));
    }
    return samples_s16;
  } else {
    static constexpr shuffle_table          TABLE_FIRST  = make_unpack_table_32b<BITS, OFFSET, 0>();
    static constexpr shuffle_table          TABLE_LAST   = make_unpack_table_32b<BITS, OFFSET, 4>();
    static constexpr std::array<int32_t, 4> SHIFTS_FIRST = make_unpack_shifts<BITS, 0, 4, int32_t>();
    static constexpr std::array<int32_t, 4> SHIFTS_LAST  = make_unpack_shifts<BITS, 4, 4, int32_t>();

    // Drop the bits of the previous sample in the 32bit lanes and keep the top halves.
    int32x4_t first_s32 = vreinterpretq_s32_u8(vqtbl1q_u8(bytes_u8, vld1q_u8(TABLE_FIRST.data())));
    int32x4_t last_s32  = vreinterpretq_s32_u8(vqtbl1q_u8(bytes_u8, vld1q_u8(TABLE_LAST.data())));
    first_s32           = vshlq_s32(first_s32, vld1q_s32(SHIFTS_FIRST.data()));
    last_s32            = vshlq_s32(last_s32, vld1q_s32(SHIFTS_LAST.data()));
    return vuzp2q_s16(vreinterpretq_s16_s32(first_s32), vreinterpretq_s16_s32(last_s32));
  }
}

/// \brief Checks whether the requested bit width is supported by the NEON implementation.
/// \param[in] iq_width Requested bit width.
///
/// \return True in case packing/unpacking with the requested bit width is supported.
inline bool iq_width_packing_supported(unsigned iq_width)
{
  return (iq_width >= 8) && (iq_width <= 16);
}

} // namespace neon
} // namespace ofh
} // namespace ocudu
