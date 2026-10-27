// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/span.h"
#include "ocudu/support/error_handling.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <immintrin.h>

namespace ocudu {
namespace ofh {
namespace mm256 {

/// \brief Packs the 16bit IQ samples of one AVX2 register using 9bit big-endian format.
///
/// Each 128bit lane of the input holds the four I samples followed by the four Q samples of four REs (the layout of
/// \ref quantize_bf16_even_odd). The output is the bit stream I0 Q0 I1 Q1 I2 Q2 I3 Q3, 9 bits per sample and most
/// significant bit first, stored in the first 9 bytes of the respective lane of the output register. The remaining 7
/// bytes of each lane are zero.
///
/// \param[in] samples_epi16 AVX2 register storing 16bit IQ samples, only the lower 9 bits of each are packed.
/// \return An AVX2 register containing 9 packed bytes in each 128bit lane.
inline __m256i pack_avx2_register_9b_be(__m256i samples_epi16)
{
  // Keep the 9 data bits of each sample and shift the j-th sample of the stream left by 7 - j, so that its two bytes
  // become the bits it contributes to the output bytes j and j + 1. AVX2 has no 16bit variable shift, but a
  // multiplication by a power of two is equivalent as the shifted values fit in 16 bits.
  //                        I0   I1  I2  I3  Q0  Q1  Q2  Q3
  const __m256i shift_epi16 = _mm256_setr_epi16(128, 32, 8, 2, 64, 16, 4, 1, 128, 32, 8, 2, 64, 16, 4, 1);
  __m256i shifted_epi16 = _mm256_mullo_epi16(_mm256_and_si256(samples_epi16, _mm256_set1_epi16(0x01ff)), shift_epi16);

  // Output byte j is the high byte of the j-th sample of the stream OR the low byte of the previous one, byte 8 is the
  // low byte of the last sample.
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
/// The three input registers hold the samples of the first RB, the samples of the first and second RBs (one per 128bit
/// lane) and the samples of the second RB, with the layout of \ref quantize_bf16_even_odd in each lane.
///
/// \param[out] prb0 Output bytes of the first PRB, 27 bytes.
/// \param[out] prb1 Output bytes of the second PRB, 27 bytes.
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

  // The first two lanes of each PRB are stored with full 16 byte writes, as the following write overwrites their 7
  // padding bytes and stays inside of the PRB. The last lane only writes its 9 bytes.
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0), _mm256_castsi256_si128(rb0_packed_epi8));
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0 + BYTES_PER_LANE), _mm256_extracti128_si256(rb0_packed_epi8, 1));
  store_9_bytes(prb0 + 2 * BYTES_PER_LANE, _mm256_castsi256_si128(rb01_packed_epi8));

  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1), _mm256_extracti128_si256(rb01_packed_epi8, 1));
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1 + BYTES_PER_LANE), _mm256_castsi256_si128(rb1_packed_epi8));
  store_9_bytes(prb1 + 2 * BYTES_PER_LANE, _mm256_extracti128_si256(rb1_packed_epi8, 1));
}

/// \brief Layout of a block of one or two PRBs unpacked into 32bit elements, see \ref unpack_prb_block_pair.
///
/// The samples of the block are spread over pairs of AVX2 registers of eight 32bit elements, each one holding a
/// sample. The two registers of a pair hold the even and odd samples of the same 16bit output register: the first
/// pair holds the first 16 samples of the first PRB, the next pair the ones of the second PRB (if present) and the
/// last pair the remaining 8 samples of each PRB, one PRB per 128bit lane. The 16 samples of a pair are gathered as
/// 16bit words from 16 bytes loaded for each lane.
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
  /// Size in bytes of one packed PRB, including the compression parameter if present.
  static constexpr unsigned PRB_SIZE = 3 * BITS + (HAS_COMP_PARAM ? 1 : 0);
  /// Size in bytes of the block.
  static constexpr unsigned BLOCK_SIZE = NOF_PRBS * PRB_SIZE;
  /// Number of register pairs of the block.
  static constexpr unsigned NOF_PAIRS = NOF_PRBS + 1;
  /// Number of samples held by the first pair of registers of each PRB.
  static constexpr unsigned NOF_HEAD_SAMPLES = 16;
  /// Byte offset of the samples inside of a PRB.
  static constexpr unsigned SAMPLES_OFFSET = HAS_COMP_PARAM ? 1 : 0;
  /// Size in bytes of a 128bit load.
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

  /// Returns the offset of the 16 bytes loaded for the given lane of the given register pair, inside of the block.
  static constexpr unsigned lane_load_offset(unsigned pair, unsigned lane)
  {
    unsigned        offset = std::min(locate_word(pair, lane, 0).first_byte, BLOCK_SIZE - LANE_SIZE);
    sample_position last   = locate_word(pair, lane, 7);
    return (last.first_byte + last.nof_bytes <= offset + LANE_SIZE) ? offset : throw "Samples exceed the loaded bytes";
  }

  /// Returns the byte shuffle gathering the two bytes holding the most significant bits of each sample into its
  /// 16bit word, in big-endian.
  static constexpr std::array<uint8_t, 32> make_shuffle(unsigned pair)
  {
    std::array<uint8_t, 32> shuffle = {};
    for (unsigned lane = 0; lane != 2; ++lane) {
      unsigned load_offset = lane_load_offset(pair, lane);
      for (unsigned word = 0; word != 8; ++word) {
        sample_position position = locate_word(pair, lane, word);
        for (unsigned byte = 0; byte != 2; ++byte) {
          // Samples held by a single byte take a zero as the least significant one.
          shuffle[lane * LANE_SIZE + word * 2 + 1 - byte] =
              (byte < position.nof_bytes) ? (position.first_byte + byte - load_offset) : 0x80;
        }
      }
    }
    return shuffle;
  }

  /// Returns the power of two aligning the most significant bit of each sample with the one of its 16bit word.
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
/// \tparam LAYOUT Layout of the block, see \ref prb_unpack_layout.
/// \tparam PAIR   Index of the register pair.
/// \param[out] even_epi32 AVX2 register storing the eight even samples, multiplied by the inverse of
///                        \c LAYOUT::SAMPLE_SCALE.
/// \param[out] odd_epi32  AVX2 register storing the eight odd samples, multiplied by the inverse of
///                        \c LAYOUT::SAMPLE_SCALE.
/// \param[in] in          Packed bytes of the block.
template <typename LAYOUT, unsigned PAIR>
inline void unpack_prb_block_pair(__m256i& even_epi32, __m256i& odd_epi32, const uint8_t* in)
{
  static constexpr unsigned LOAD_OFFSET_0 = LAYOUT::lane_load_offset(PAIR, 0);
  static constexpr unsigned LOAD_OFFSET_1 = LAYOUT::lane_load_offset(PAIR, 1);
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

/// \brief Unpacks packed 9bit IQ samples stored as bytes in big-endian format to an array of 16bit signed values.
///
/// \param[out] unpacked_iq_data A span of 16bit integers, corresponding to \c NOF_CARRIERS_PER_RB unpacked IQ pairs.
/// \param[in]  packed_data      A span of 27 packed bytes.
///
/// \note The \c unpacked_iq_data parameter should be sized to store 32 output IQ samples: it is 24 IQ samples of one RB
/// rounded up to 32-byte boundary required by AVX2 intrinsics.
inline void avx2_unpack_prb_9b_be(span<int16_t> unpacked_iq_data, span<const uint8_t> packed_data)
{
  constexpr size_t avx2_size_short_words = 16;
  ocudu_assert(unpacked_iq_data.size() >= avx2_size_short_words * 2, "Wrong unpacked data span size");

  // Load input, 27 bytes (fits in one AVX2 register).
  // Mask-load for epi8 is not available, so we do in a few steps.
  __m128i packed_data_high =
      _mm_srli_si128(_mm_loadu_si128(reinterpret_cast<const __m128i*>(packed_data.data() + 11)), 5);
  __m128i packed_data_low  = _mm_loadu_si128(reinterpret_cast<const __m128i*>(packed_data.data()));
  __m256i packed_data_epi8 = _mm256_permute2f128_si256(
      _mm256_castsi128_si256(packed_data_low), _mm256_castsi128_si256(packed_data_high), 0x20);

  // Duplicate input words (it is required since below in the code every byte will be used twice:
  // to provide MSB bits of the current IQ sample and LSB bits of the previous IQ sample).
  __m256i packed_data_0_epi8 = _mm256_permute4x64_epi64(packed_data_epi8, 0b10010100);
  __m256i packed_data_1_epi8 = _mm256_permute4x64_epi64(packed_data_epi8, 0b00001110);

  // Swap bytes for little-endian representation.
  // As a result each pair of bytes will contain all bits of a single I or Q sample.
  const __m256i shuffle_mask_0_epi8 =
      _mm256_setr_epi64x(0x0304020301020001, 0x0708060705060405, 0x0405030402030102, 0x0809070806070506);
  const __m256i shuffle_mask_1_epi8 =
      _mm256_setr_epi64x(0x0506040503040203, 0x090a080907080607, 0xffffffffffffffff, 0xffffffffffffffff);
  __m256i packed_data_0_le_epi8 = _mm256_shuffle_epi8(packed_data_0_epi8, shuffle_mask_0_epi8);
  __m256i packed_data_1_le_epi8 = _mm256_shuffle_epi8(packed_data_1_epi8, shuffle_mask_1_epi8);

  // Do logical shift left, then arithmetical shift right in order to sign extend values.
  // Note, AVX2 only allows to shift 32bit words using vector of indexes, so we do it twice.
  const __m256i shl_0_epi32 = _mm256_setr_epi32(0, 2, 4, 6, 0, 2, 4, 6);
  const __m256i shl_1_epi32 = _mm256_setr_epi32(1, 3, 5, 7, 1, 3, 5, 7);
  // Mask 16bit words to use only shifted bits.
  const __m256i mask_even_epi16 = _mm256_set1_epi32(0x0000ffff);
  const __m256i mask_odd_epi16  = _mm256_set1_epi32(0xffff0000);

  __m256i unpacked_data_00_epi16 = _mm256_sllv_epi32(packed_data_0_le_epi8, shl_0_epi32);
  __m256i unpacked_data_01_epi16 = _mm256_sllv_epi32(packed_data_0_le_epi8, shl_1_epi32);
  __m256i unpacked_data_10_epi16 = _mm256_sllv_epi32(packed_data_1_le_epi8, shl_0_epi32);
  __m256i unpacked_data_11_epi16 = _mm256_sllv_epi32(packed_data_1_le_epi8, shl_1_epi32);
  unpacked_data_00_epi16         = _mm256_and_si256(unpacked_data_00_epi16, mask_even_epi16);
  unpacked_data_01_epi16         = _mm256_and_si256(unpacked_data_01_epi16, mask_odd_epi16);
  unpacked_data_10_epi16         = _mm256_and_si256(unpacked_data_10_epi16, mask_even_epi16);
  unpacked_data_11_epi16         = _mm256_and_si256(unpacked_data_11_epi16, mask_odd_epi16);

  // Merge two vectors to get vector of shifted 16bit IQ samples, right-shift it to sign-extend values.
  __m256i unpacked_data_0_epi16 = _mm256_srai_epi16(_mm256_or_si256(unpacked_data_00_epi16, unpacked_data_01_epi16), 7);
  __m256i unpacked_data_1_epi16 = _mm256_srai_epi16(_mm256_or_si256(unpacked_data_10_epi16, unpacked_data_11_epi16), 7);

  // Write results.
  _mm256_storeu_si256(reinterpret_cast<__m256i*>(unpacked_iq_data.data()), unpacked_data_0_epi16);
  _mm256_storeu_si256(reinterpret_cast<__m256i*>(unpacked_iq_data.subspan(16, 8).data()), unpacked_data_1_epi16);
}

/// \brief Unpacks packed 16bit IQ samples stored as bytes in big-endian format to an array of 16bit signed values.
///
/// \param[out] unpacked_iq_data A span of 16bit integers, corresponding to \c NOF_CARRIERS_PER_RB unpacked IQ pairs.
/// \param[in]  packed_data      A span of 48 packed bytes.
inline void avx2_unpack_prb_16b_be(span<int16_t> unpacked_iq_data, span<const uint8_t> packed_data)
{
  // Load input, 48 bytes (need two AVX2 registers).
  const __m256i rd_mask_last_bytes = _mm256_setr_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000, 0, 0, 0, 0);

  __m256i reg0_epi16 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(packed_data.data()));
  __m256i reg1_epi16 = _mm256_maskload_epi32(reinterpret_cast<const int*>(packed_data.data() + 32), rd_mask_last_bytes);

  // Swap bytes for Little-endian representation.
  const __m256i shuffle_mask_epi8 =
      _mm256_setr_epi64x(0x0607040502030001, 0x0e0f0c0d0a0b0809, 0x0607040502030001, 0x0e0f0c0d0a0b0809);

  __m256i reg0_unpacked_data_epi16 = _mm256_shuffle_epi8(reg0_epi16, shuffle_mask_epi8);
  __m256i reg1_unpacked_data_epi16 = _mm256_shuffle_epi8(reg1_epi16, shuffle_mask_epi8);

  _mm256_storeu_si256(reinterpret_cast<__m256i*>(unpacked_iq_data.data()), reg0_unpacked_data_epi16);
  _mm256_maskstore_epi32(reinterpret_cast<int*>(unpacked_iq_data.data() + 16),
                         _mm256_setr_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0, 0, 0, 0),
                         reg1_unpacked_data_epi16);
}

/// \brief Unpacks packed IQ samples stored as bytes in big-endian format to an array of 16bit signed values.
///
/// \param[out] unpacked_iq_data A span of 32 16bit integers, corresponding to \c NOF_CARRIERS_PER_RB unpacked IQ pairs.
/// \param[in]  packed_data      A span of input packed bytes.
/// \param[in]  iq_width         Bit width of the packed IQ samples.
///
/// \note The \c unpacked_iq_data parameter should be sized to store 32 output IQ samples: it is 24 IQ samples of one RB
/// rounded up to 32-byte boundary required by AVX2 intrinsics.
inline void unpack_prb_big_endian(span<int16_t> unpacked_iq_data, span<const uint8_t> packed_data, unsigned iq_width)
{
  switch (iq_width) {
    case 9:
      return avx2_unpack_prb_9b_be(unpacked_iq_data, packed_data);
    case 16:
      return avx2_unpack_prb_16b_be(unpacked_iq_data, packed_data);
    default:
      report_fatal_error("Unsupported bit width");
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
