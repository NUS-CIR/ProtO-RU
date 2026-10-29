// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief AVX2 packing and unpacking of IQ samples in big-endian format.

#pragma once

#include "ocudu/ran/resource_block.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <immintrin.h>

namespace ocudu {
namespace ofh {
namespace mm256 {

/// \brief Packs the 16bit IQ samples of one AVX2 register using 9bit big-endian format.
///
/// Each 128bit lane of the input holds the four I samples followed by the four Q samples of four REs, the layout
/// given by \ref quantize_bf16_even_odd. The lane is packed into the bit stream I0 Q0 I1 Q1 I2 Q2 I3 Q3, 9 bits per
/// sample and most significant bit first, stored in the first 9 bytes of the respective lane of the output register.
/// The remaining 7 bytes of each lane are zero.
///
/// \param[in] samples_epi16 AVX2 register storing 16bit IQ samples, only the lower 9 bits of each are packed.
/// \return An AVX2 register containing 9 packed bytes in each 128bit lane.
inline __m256i pack_avx2_register_9b_be(__m256i samples_epi16)
{
  // The j-th sample of the stream starts at the bit j of the output byte j (counted from the most significant one),
  // so it is shifted left by 7 - j to place its bits where they land in the two bytes it spans:
  //   stream  element  shift  16bit lane after the shift (bits 15 to 0)
  //   I0      0        7      i8 i7 i6 i5 i4 i3 i2 i1 | i0  0  0  0  0  0  0  0
  //   Q0      4        6       0 q8 q7 q6 q5 q4 q3 q2 | q1 q0  0  0  0  0  0  0
  //   I1      1        5       0  0 i8 i7 i6 i5 i4 i3 | i2 i1 i0  0  0  0  0  0
  //   Q1      5        4       0  0  0 q8 q7 q6 q5 q4 | q3 q2 q1 q0  0  0  0  0
  //   I2      2        3       0  0  0  0 i8 i7 i6 i5 | i4 i3 i2 i1 i0  0  0  0
  //   Q2      6        2       0  0  0  0  0 q8 q7 q6 | q5 q4 q3 q2 q1 q0  0  0
  //   I3      3        1       0  0  0  0  0  0 i8 i7 | i6 i5 i4 i3 i2 i1 i0  0
  //   Q3      7        0       0  0  0  0  0  0  0 q8 | q7 q6 q5 q4 q3 q2 q1 q0
  // AVX2 has no 16bit variable shift, but a multiplication by a power of two is equivalent once the padding bits of
  // the samples are masked out.
  const __m256i shift_epi16 = _mm256_setr_epi16(128, 32, 8, 2, 64, 16, 4, 1, 128, 32, 8, 2, 64, 16, 4, 1);
  __m256i shifted_epi16 = _mm256_mullo_epi16(_mm256_and_si256(samples_epi16, _mm256_set1_epi16(0x01ff)), shift_epi16);

  // The output byte j is the high byte of the j-th sample of the stream OR the low byte of the previous one, and the
  // byte 8 is the low byte of the last sample. The bits of the two bytes do not overlap, so two byte shuffles gather
  // them and an OR merges them.
  const __m256i high_bytes_epi8 =
      _mm256_setr_epi64x(0x0f070d050b030901, 0xffffffffffffffff, 0x0f070d050b030901, 0xffffffffffffffff);
  const __m256i low_bytes_epi8 =
      _mm256_setr_epi64x(0x060c040a020800ff, 0xffffffffffffff0e, 0x060c040a020800ff, 0xffffffffffffff0e);
  return _mm256_or_si256(_mm256_shuffle_epi8(shifted_epi16, high_bytes_epi8),
                         _mm256_shuffle_epi8(shifted_epi16, low_bytes_epi8));
}

/// Stores the first 9 bytes of the given register at the given address.
inline void store_9_bytes(uint8_t* dest, __m128i reg)
{
  // Two overlapping 8 byte stores, the second one shifted by one byte.
  _mm_storel_epi64(reinterpret_cast<__m128i*>(dest), reg);
  _mm_storel_epi64(reinterpret_cast<__m128i*>(dest + 1), _mm_srli_si128(reg, 1));
}

/// \brief Packs the 16bit IQ samples of two RBs using 9bit big-endian format.
///
/// The three input registers hold the samples of the two RBs with the register layout described in avx2_helpers.h,
/// each 128bit lane with the layout of \ref quantize_bf16_even_odd.
///
/// \param[out] prb0      Output bytes of the first PRB, 27 bytes.
/// \param[out] prb1      Output bytes of the second PRB, 27 bytes.
/// \param[in] rb0_epi16  AVX2 register storing the first 16 samples of the first RB.
/// \param[in] rb01_epi16 AVX2 register storing the last 8 samples of the first RB and the first 8 of the second RB.
/// \param[in] rb1_epi16  AVX2 register storing the last 16 samples of the second RB.
inline void
avx2_pack_prbs_9b_big_endian(uint8_t* prb0, uint8_t* prb1, __m256i rb0_epi16, __m256i rb01_epi16, __m256i rb1_epi16)
{
  // Number of bytes packed from one 128bit lane (8 samples of 9 bits).
  static constexpr unsigned BYTES_PER_LANE = 9;

  __m256i rb0_packed_epi8  = pack_avx2_register_9b_be(rb0_epi16);
  __m256i rb01_packed_epi8 = pack_avx2_register_9b_be(rb01_epi16);
  __m256i rb1_packed_epi8  = pack_avx2_register_9b_be(rb1_epi16);

  // The 9 bytes of each lane are stored one after the other. The first two lanes of a PRB are stored with full 16
  // byte writes, as the following write overwrites their 7 padding bytes and stays inside of the PRB. The last lane
  // only writes its 9 bytes.
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0), _mm256_castsi256_si128(rb0_packed_epi8));
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0 + BYTES_PER_LANE), _mm256_extracti128_si256(rb0_packed_epi8, 1));
  store_9_bytes(prb0 + 2 * BYTES_PER_LANE, _mm256_castsi256_si128(rb01_packed_epi8));

  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1), _mm256_extracti128_si256(rb01_packed_epi8, 1));
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1 + BYTES_PER_LANE), _mm256_castsi256_si128(rb1_packed_epi8));
  store_9_bytes(prb1 + 2 * BYTES_PER_LANE, _mm256_extracti128_si256(rb1_packed_epi8, 1));
}

/// \brief Layout of a block of one or two packed PRBs unpacked into 32bit elements.
///
/// The decompressor unpacks a block of PRBs into pairs of AVX2 registers of eight 32bit elements, each one holding a
/// sample. The two registers of a pair hold the even and the odd samples (that is, the I and the Q components) of 16
/// consecutive samples:
/// | register pair | lane 0                | lane 1                |
/// | ------------- | --------------------- | --------------------- |
/// | 0             | PRB0 samples 0 to 7   | PRB0 samples 8 to 15  |
/// | 1             | PRB1 samples 0 to 7   | PRB1 samples 8 to 15  |
/// | last          | PRB0 samples 16 to 23 | PRB1 samples 16 to 23 |
/// The pair 1 is absent when the block has a single PRB, and the second lane of the last pair repeats the first PRB.
///
/// Each lane of a pair gathers its eight samples from 16 bytes loaded from the block: a byte shuffle moves the two
/// bytes holding the most significant bits of each sample into its 16bit word, in big-endian, and a multiplication
/// by a power of two aligns the most significant bit of the sample with the one of the word. The sample is then sign
/// extended from the top of the word, which drops the bits of the following sample. This class computes the position
/// of every sample in the block and generates the shuffles and multipliers at compile time.
///
/// As an example, with 9bit samples preceded by a compression parameter the PRB takes 28 bytes and the sample \c s
/// of the PRB \c p starts at the bit <tt>(28 p + 1) 8 + 9 s</tt>, so the lane 0 of the pair 0 loads the bytes 1 to
/// 16 and gathers the bytes {1, 2}, {2, 3}, {3, 4}, ... {8, 9} with the multipliers 1, 2, 4, ... 128.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte.
/// \tparam NOF_PRBS       Number of PRBs of the block, one or two.
template <unsigned BITS, bool HAS_COMP_PARAM, unsigned NOF_PRBS>
struct prb_unpack_layout {
  static_assert((NOF_PRBS == 1) || (NOF_PRBS == 2), "Unsupported number of PRBs");
  static_assert(BITS <= 16, "The samples must fit in 16bit words");

  /// Bit width of the packed samples.
  static constexpr unsigned SAMPLE_BITS = BITS;
  /// Scaling of the unpacked samples: the 16bit ones are left at the top of their 32bit elements.
  static constexpr float SAMPLE_SCALE = (BITS == 16) ? 0x1p-16F : 1.0F;
  /// Number of samples of a PRB.
  static constexpr unsigned NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;
  /// Size in bytes of one packed PRB, including the compression parameter if present.
  static constexpr unsigned PRB_SIZE = 3 * BITS + (HAS_COMP_PARAM ? 1 : 0);
  /// Size in bytes of the block.
  static constexpr unsigned BLOCK_SIZE = NOF_PRBS * PRB_SIZE;
  /// Number of register pairs of the block.
  static constexpr unsigned NOF_PAIRS = NOF_PRBS + 1;
  /// Number of samples of each PRB held by the first pairs of registers.
  static constexpr unsigned NOF_HEAD_SAMPLES = 16;
  /// Byte offset of the samples inside of a PRB.
  static constexpr unsigned SAMPLES_OFFSET = HAS_COMP_PARAM ? 1 : 0;
  /// Size in bytes of a lane load.
  static constexpr unsigned LANE_SIZE = 16;

  /// Position of a packed sample in the block.
  struct sample_position {
    /// Index of the first byte holding bits of the sample.
    unsigned first_byte;
    /// Number of bytes holding bits of the sample.
    unsigned nof_bytes;
    /// Offset of the most significant bit of the sample inside of the first byte.
    unsigned bit_offset;
  };

  /// Returns the position of the given sample of the given PRB.
  static constexpr sample_position locate_sample(unsigned prb, unsigned sample)
  {
    unsigned bit = (prb * PRB_SIZE + SAMPLES_OFFSET) * 8 + sample * BITS;
    return {bit / 8, (bit % 8 + BITS + 7) / 8, bit % 8};
  }

  /// Returns the position of the sample held by the given word of the given lane of the given register pair.
  static constexpr sample_position locate_word(unsigned pair, unsigned lane, unsigned word)
  {
    if (pair < NOF_PRBS) {
      return locate_sample(pair, lane * 8 + word);
    }
    return locate_sample(std::min(lane, NOF_PRBS - 1), NOF_HEAD_SAMPLES + word);
  }

  /// \brief Returns the offset in the block of the 16 bytes loaded for the given lane of the given register pair.
  ///
  /// The bytes start at the first sample of the lane, unless that would read beyond the block, in which case they end
  /// at the last byte of the block.
  static constexpr unsigned get_lane_load_offset(unsigned pair, unsigned lane)
  {
    return std::min(locate_word(pair, lane, 0).first_byte, BLOCK_SIZE - LANE_SIZE);
  }

  /// Returns true if the loaded bytes of every lane of the given register pair cover all its samples.
  static constexpr bool are_lane_loads_sufficient(unsigned pair)
  {
    for (unsigned lane = 0; lane != 2; ++lane) {
      sample_position last = locate_word(pair, lane, 7);
      if (last.first_byte + last.nof_bytes > get_lane_load_offset(pair, lane) + LANE_SIZE) {
        return false;
      }
    }
    return true;
  }

  /// \brief Returns the byte shuffle gathering the two bytes holding the most significant bits of each sample of the
  /// given register pair into its 16bit word, in big-endian.
  ///
  /// The samples held by a single byte take a zero as least significant byte.
  static constexpr std::array<uint8_t, 32> make_shuffle(unsigned pair)
  {
    std::array<uint8_t, 32> shuffle = {};
    for (unsigned lane = 0; lane != 2; ++lane) {
      unsigned load_offset = get_lane_load_offset(pair, lane);
      for (unsigned word = 0; word != 8; ++word) {
        sample_position position = locate_word(pair, lane, word);
        for (unsigned byte = 0; byte != 2; ++byte) {
          shuffle[lane * LANE_SIZE + word * 2 + 1 - byte] =
              (byte < position.nof_bytes) ? (position.first_byte + byte - load_offset) : 0x80;
        }
      }
    }
    return shuffle;
  }

  /// Returns the powers of two aligning the most significant bit of each sample of the given register pair with the
  /// one of its 16bit word.
  static constexpr std::array<int16_t, 16> make_multipliers(unsigned pair)
  {
    std::array<int16_t, 16> multipliers = {};
    for (unsigned lane = 0; lane != 2; ++lane) {
      for (unsigned word = 0; word != 8; ++word) {
        multipliers[lane * 8 + word] = 1 << locate_word(pair, lane, word).bit_offset;
      }
    }
    return multipliers;
  }

  /// Returns true if some sample of the given register pair does not start at a byte boundary.
  static constexpr bool needs_shift(unsigned pair)
  {
    for (int16_t multiplier : make_multipliers(pair)) {
      if (multiplier != 1) {
        return true;
      }
    }
    return false;
  }
};

/// \brief Unpacks the samples of one register pair of a block of PRBs into 32bit integers.
///
/// \tparam LAYOUT         Layout of the block, see \ref prb_unpack_layout.
/// \tparam PAIR           Index of the register pair.
/// \param[out] even_epi32 AVX2 register storing the eight even samples, multiplied by the inverse of
///                        \c LAYOUT::SAMPLE_SCALE.
/// \param[out] odd_epi32  AVX2 register storing the eight odd samples, multiplied by the inverse of
///                        \c LAYOUT::SAMPLE_SCALE.
/// \param[in] in          Packed bytes of the block.
template <typename LAYOUT, unsigned PAIR>
inline void unpack_prb_block_pair(__m256i& even_epi32, __m256i& odd_epi32, const uint8_t* in)
{
  static_assert(LAYOUT::are_lane_loads_sufficient(PAIR), "The samples exceed the loaded bytes");
  static constexpr unsigned LOAD_OFFSET_0 = LAYOUT::get_lane_load_offset(PAIR, 0);
  static constexpr unsigned LOAD_OFFSET_1 = LAYOUT::get_lane_load_offset(PAIR, 1);
  static constexpr auto     SHUFFLE       = LAYOUT::make_shuffle(PAIR);
  static constexpr auto     MULTIPLIERS   = LAYOUT::make_multipliers(PAIR);

  // Load the bytes of each lane and gather the two bytes holding the most significant bits of each sample into its
  // word.
  __m128i lane_0_epi8 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(in + LOAD_OFFSET_0));
  __m128i lane_1_epi8 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(in + LOAD_OFFSET_1));
  __m256i samples_epi16 =
      _mm256_shuffle_epi8(_mm256_inserti128_si256(_mm256_castsi128_si256(lane_0_epi8), lane_1_epi8, 1),
                          _mm256_loadu_si256(reinterpret_cast<const __m256i*>(SHUFFLE.data())));

  // Shift out the bits of the previous sample with a multiplication, as AVX2 has no 16bit variable shift.
  if constexpr (LAYOUT::needs_shift(PAIR)) {
    samples_epi16 =
        _mm256_mullo_epi16(samples_epi16, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(MULTIPLIERS.data())));
  }

  if constexpr (LAYOUT::SAMPLE_BITS == 16) {
    // The samples fill their words, keep them at the top of the elements.
    even_epi32 = _mm256_slli_epi32(samples_epi16, 16);
    odd_epi32  = _mm256_and_si256(samples_epi16, _mm256_set1_epi32(0xffff0000));
  } else {
    // Sign extend the samples from the top of their words, the bits of the following sample are shifted out.
    even_epi32 = _mm256_srai_epi32(_mm256_slli_epi32(samples_epi16, 16), 32 - LAYOUT::SAMPLE_BITS);
    odd_epi32  = _mm256_srai_epi32(samples_epi16, 32 - LAYOUT::SAMPLE_BITS);
  }
}

/// \brief Checks whether the requested bit width is supported by the AVX2 implementation.
/// \param[in] iq_width Requested bit width.
///
/// \return True in case packing/unpacking with the requested bit width is supported
inline bool iq_width_packing_supported(unsigned iq_width)
{
  return ((iq_width == 9) || (iq_width == 16));
}

} // namespace mm256
} // namespace ofh
} // namespace ocudu
