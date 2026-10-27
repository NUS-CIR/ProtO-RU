// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief NEON packing and unpacking of IQ samples in big-endian format.
///
/// A NEON register holds 8 16bit samples, which pack into exactly \c BITS bytes when each sample takes \c BITS bits.
/// The byte shuffles, shifts and masks of the packing of every bit width are generated at compile time from the
/// position of each sample in the packed bytes.

#pragma once

#include "ocudu/adt/span.h"
#include "ocudu/support/compiler.h"
#include "ocudu/support/error_handling.h"
#include <arm_neon.h>
#include <array>
#include <cstdint>

namespace ocudu {
namespace ofh {
namespace neon {

/// Byte shuffle table of a table lookup.
using shuffle_table = std::array<uint8_t, 16>;

/// Byte index that reads as zero in a table lookup.
static constexpr uint8_t SHUFFLE_ZERO = 0xff;

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
/// \tparam BITS         Bit width of the packed samples.
/// \tparam HAS_EXPONENT Whether the samples are shifted right by a BFP exponent before packing.
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

/// \brief Unpacks packed 9bit IQ samples stored as bytes in big-endian format to an array of 16bit signed values.
///
/// \param[out] unpacked_iq_data A sequence of 24 integers, corresponding to \c NOF_CARRIERS_PER_RB unpacked IQ pairs.
/// \param[in]  packed_data      A sequence of 27 packed bytes.
inline void unpack_prb_9b_big_endian(span<int16_t> unpacked_iq_data, span<const uint8_t> packed_data)
{
  // Load input (we need two NEON register to load 27 bytes).
  // The first 16 bytes are loaded directly.
  uint8x16x2_t packed_vec_u8x2;
  packed_vec_u8x2.val[0] = vld1q_u8(packed_data.data());
  // Load from 11th byte, which gives us the last 11 bytes plus 5 extra bytes without exceeding a read of 27 bytes.
  packed_vec_u8x2.val[1] = vld1q_u8(packed_data.data() + 11);
  // Discard the first 5 bytes.
  packed_vec_u8x2.val[1] = vextq_u8(packed_vec_u8x2.val[1], vdupq_n_u8(0), 5);

  // Duplicate input words (it is required since below in the code every byte will be used twice:
  // to provide MSB bits of the current IQ sample and LSB bits of the previous IQ sample).
  uint8x16_t tmp_packed_0_u8 =
      vqtbl2q_u8(packed_vec_u8x2, vcombine_u8(vcreate_u8(0x0304020301020001), vcreate_u8(0x0708060705060405)));
  uint8x16_t tmp_packed_1_u8 =
      vqtbl2q_u8(packed_vec_u8x2, vcombine_u8(vcreate_u8(0x0c0d0b0c0a0b090a), vcreate_u8(0x10110f100e0f0d0e)));
  uint8x16_t tmp_packed_2_u8 =
      vqtbl2q_u8(packed_vec_u8x2, vcombine_u8(vcreate_u8(0x1516141513141213), vcreate_u8(0x191a181917181617)));

  // Shift left to align to 16bit boundary.
  const int16x8_t shl_mask_s16        = vcombine_s16(vcreate_s16(0x0003000200010000), vcreate_s16(0x0007000600050004));
  uint16x8_t      shl_tmp_packed_0_u8 = vshlq_u16(vreinterpretq_u16_u8(tmp_packed_0_u8), shl_mask_s16);
  uint16x8_t      shl_tmp_packed_1_u8 = vshlq_u16(vreinterpretq_u16_u8(tmp_packed_1_u8), shl_mask_s16);
  uint16x8_t      shl_tmp_packed_2_u8 = vshlq_u16(vreinterpretq_u16_u8(tmp_packed_2_u8), shl_mask_s16);

  // Arithmetically shift right by 7 positions to put bits of interest into LSB positions while preserving the sign.
  int16x8_t unpacked_data_0_s16 = vshrq_n_s16(vreinterpretq_s16_u16(shl_tmp_packed_0_u8), 7);
  int16x8_t unpacked_data_1_s16 = vshrq_n_s16(vreinterpretq_s16_u16(shl_tmp_packed_1_u8), 7);
  int16x8_t unpacked_data_2_s16 = vshrq_n_s16(vreinterpretq_s16_u16(shl_tmp_packed_2_u8), 7);

  // Write results to the output buffer.
  vst1q_s16(unpacked_iq_data.data(), unpacked_data_0_s16);
  vst1q_s16(unpacked_iq_data.data() + 8, unpacked_data_1_s16);
  vst1q_s16(unpacked_iq_data.data() + 16, unpacked_data_2_s16);
}

/// \brief Unpacks the first 112 bits of the input NEON register stored in big-endian format.
///
/// \param[in] regs NEON register storing 14 packed bytes.
/// \return NEON register storing 8 16-bit IQ samples.
inline int16x8_t unpack_compressed_14bit_values(uint8x16_t reg)
{
  // Consider input bytes represented as 64-bit words:
  // b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0
  // As a first step we shuffle input data so that every byte is used twice.
  // The resulting 16-bit words, except two of them, will contain all bits of either I or Q component.
  //
  // Let's denote shuffled b5_b6 | b3_b4 | b1_b2 | b0_b1 vector as in_shuffled_1_epi16, where
  // b0_b1: | i13 i12 i11 i10 i9  i8  i7  i6  | i5  i4  i3 i2 i1 i0 q13 q12 |  Has all bits of I0 component.
  // b1_b2: | i5  i4  i3  i2  i1  i0  q13 q12 | q11 q10 q9 q8 q7 q6 q5  q4  |  Missing q3 q2 q1 q0 of Q0 component (b3).
  // b3_b4: | q3  q2  q1  q0  i13 i12 i11 i10 | i9  i8  i7 i6 i5 i4 i3  i2  |  Missing i1 i0 of I1 component (b5).
  // b5_b6: | i1  i0  q13 q12 q11 q10 q9  q8  | q7  q6  q5 q4 q3 q2 q1  q0  |  Has all bits of Q1 component.
  int8x16_t reg_s8 = vreinterpretq_s8_u8(reg);
  int8x16_t reg_shuffled_1_s8 =
      vqtbl1q_s8(reg_s8, vcombine_u8(vcreate_u8(0x0506030401020001), vcreate_u8(0x0c0d0a0b08090708)));

  // Shift left to align with 16-bit boundary, then shift right to sign-extend values.
  const int16x8_t lshift_mask_s16     = vcombine_s16(vcreate_s16(0x0002000400060000), vcreate_s16(0x0002000400060000));
  int16x8_t       in_reg_shuffled_s16 = vreinterpretq_s16_s8(reg_shuffled_1_s8);
  int16x8_t       out_reg_s16         = vshrq_n_s16(vshlq_s16(in_reg_shuffled_s16, lshift_mask_s16), 2);

  // Create a vector of missing LSB bits,
  // take them from input b3 and b5 of every 64-bit word:
  //
  // b3: | q3 q2 q1  q0  i13 i12 i11 i10 |  Right shift by 4 to put at LSB positions.
  // b5: | i1 i0 q13 q12 q11 q10 q9  q8  |  Right shift by 6 to put at LSB positions.
  int8x16_t reg_shuffled_2_s8 =
      vqtbl1q_s8(reg_s8, vcombine_u8(vcreate_u8(0xffffff05ff03ffff), vcreate_u8(0xffffff0cff0affff)));
  // Shift right permuted bytes to align with out_reg_s16.
  const int8x16_t rshift_mask_s8 =
      vnegq_s8(vcombine_s8(vcreate_s8(0x0000000600040000), vcreate_s8(0x0000000600040000)));
  const int8x16_t and_mask_s8 = vcombine_s8(vcreate_s8(0x03000f0000), vcreate_s8(0x03000f0000));
  reg_shuffled_2_s8           = vandq_s8(vshlq_s8(reg_shuffled_2_s8, rshift_mask_s8), and_mask_s8);

  // Perform logical OR between the two vectors.
  return vorrq_s16(out_reg_s16, vreinterpretq_s16_s8(reg_shuffled_2_s8));
}

/// \brief Unpacks packed 14bit IQ samples stored as bytes in big-endian format to an array of 16bit signed values.
///
/// \param[out] unpacked_iq_data A sequence of 24 integers, corresponding to \c NOF_CARRIERS_PER_RB unpacked IQ pairs.
/// \param[in]  packed_data      A sequence of 42 packed bytes.
inline void unpack_prb_14b_big_endian(span<int16_t> unpacked_iq_data, span<const uint8_t> packed_data)
{
  // Number of bytes loaded in a single NEON register. They will be unpacked to eight IQ samples occupy one full NEON
  // register.
  static constexpr unsigned COMPRESSED_BYTES_PER_NEON_REG = 14;
  // Number of bytes used by 1 packed PRB with IQ samples compressed to 14 bits.
  static constexpr unsigned BYTES_PER_PRB_14BIT_COMPRESSION = 42;
  // Size in bytes of a NEON register.
  static constexpr unsigned BYTES_IN_NEON_REG = 16;

  // Load input (we need three NEON register to load 42 bytes).
  uint8x16x3_t packed_vec_u8x3;
  // Load 16 bytes: the first 14 bytes are used, the last two bytes are ignored.
  packed_vec_u8x3.val[0] = vld1q_u8(packed_data.data());
  // Load next 16 bytes starting from position 14: the first 14 bytes are used, the last two bytes are ignored.
  packed_vec_u8x3.val[1] = vld1q_u8(packed_data.data() + COMPRESSED_BYTES_PER_NEON_REG);
  // Finally we need to load the last 14 bytes without exceeding the input data length.
  packed_vec_u8x3.val[2] = vld1q_u8(packed_data.data() + BYTES_PER_PRB_14BIT_COMPRESSION - BYTES_IN_NEON_REG);
  // Discard the first 2 bytes.
  packed_vec_u8x3.val[2] = vextq_u8(packed_vec_u8x3.val[2], vdupq_n_u8(0), 2);

  int16x8_t unpacked_data_p1_s16 = unpack_compressed_14bit_values(packed_vec_u8x3.val[0]);
  int16x8_t unpacked_data_p2_s16 = unpack_compressed_14bit_values(packed_vec_u8x3.val[1]);
  int16x8_t unpacked_data_p3_s16 = unpack_compressed_14bit_values(packed_vec_u8x3.val[2]);

  vst1q_s16(unpacked_iq_data.data(), unpacked_data_p1_s16);
  vst1q_s16(unpacked_iq_data.data() + 8, unpacked_data_p2_s16);
  vst1q_s16(unpacked_iq_data.data() + 16, unpacked_data_p3_s16);
}

/// \brief Unpacks packed 16bit IQ samples stored as bytes in big-endian format to an array of 16bit signed values.
///
/// \param[out] unpacked_iq_data A sequence of 24 integers, corresponding to \c NOF_CARRIERS_PER_RB unpacked IQ pairs.
/// \param[in]  packed_data      A sequence of 48 packed bytes.
inline void unpack_prb_16b_big_endian(span<int16_t> unpacked_iq_data, span<const uint8_t> packed_data)
{
  static const uint8x16_t shuffle_mask_u8 = vcombine_u8(vcreate_u8(0x0607040502030001), vcreate_u8(0x0e0f0c0d0a0b0809));

  // Load input (we need three NEON register to load 48 bytes).
  uint8x16x3_t packed_vec_u8x3 = vld1q_u8_x3(packed_data.data());

  uint8x16x3_t packed_shuffled_s16;
  packed_shuffled_s16.val[0] = vqtbl1q_u8(packed_vec_u8x3.val[0], shuffle_mask_u8);
  packed_shuffled_s16.val[1] = vqtbl1q_u8(packed_vec_u8x3.val[1], shuffle_mask_u8);
  packed_shuffled_s16.val[2] = vqtbl1q_u8(packed_vec_u8x3.val[2], shuffle_mask_u8);

  // Write results to the output buffer.
  vst1q_s16(unpacked_iq_data.data(), vreinterpretq_s16_u8(packed_shuffled_s16.val[0]));
  vst1q_s16(unpacked_iq_data.data() + 8, vreinterpretq_s16_u8(packed_shuffled_s16.val[1]));
  vst1q_s16(unpacked_iq_data.data() + 16, vreinterpretq_s16_u8(packed_shuffled_s16.val[2]));
}

/// \brief Unpacks packed IQ samples stored as bytes in big-endian format to an array of 16bit signed values.
///
/// \param[out] unpacked_iq_data A sequence of 24 integers, corresponding to \c NOF_CARRIERS_PER_RB unpacked IQ pairs.
/// \param[in]  packed_data      A sequence of input packed bytes.
/// \param[in] iq_width          Bit width of the packed IQ samples.
inline void unpack_prb_big_endian(span<int16_t> unpacked_iq_data, span<const uint8_t> packed_data, unsigned iq_width)
{
  if (iq_width == 9) {
    return unpack_prb_9b_big_endian(unpacked_iq_data, packed_data);
  }
  if (iq_width == 14) {
    return unpack_prb_14b_big_endian(unpacked_iq_data, packed_data);
  }
  if (iq_width == 16) {
    return unpack_prb_16b_big_endian(unpacked_iq_data, packed_data);
  }
  report_fatal_error("Unsupported bit width");
}

/// \brief Checks whether the requested bit width is supported by the NEON packing.
/// \param[in] iq_width Requested bit width.
///
/// \return True in case packing with the requested bit width is supported.
inline bool iq_width_packing_supported(unsigned iq_width)
{
  return (iq_width >= 8) && (iq_width <= 16);
}

/// \brief Checks whether the requested bit width is supported by the NEON unpacking.
/// \param[in] iq_width Requested bit width.
///
/// \return True in case unpacking with the requested bit width is supported.
inline bool iq_width_unpacking_supported(unsigned iq_width)
{
  return ((iq_width == 9) || (iq_width == 14) || (iq_width == 16));
}

} // namespace neon
} // namespace ofh
} // namespace ocudu
