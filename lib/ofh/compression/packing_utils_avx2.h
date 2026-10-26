// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/span.h"
#include "ocudu/support/error_handling.h"
#include <immintrin.h>

namespace ocudu {
namespace ofh {
namespace mm256 {

/// \brief Packs 16bit IQ samples in one AVX2 register using 9bit big-endian format.
///
/// \param[in] cmp_data_epi16 AVX2 register storing 16bit IQ samples.
/// \return An AVX2 register containing 18 packed bytes.
///
/// \note Each 128bit lane of the input contains eight I and Q values, each of them with 9 bits of data (the LSB ones)
/// and 7 bits of sign extension. The function packs the eight IQ samples (only the data bits) into the first 9 bytes of
/// the respective 128bit lane of the output register (the remaining 7 bytes are padding). Thus, the return value
/// contains 18 packed data bytes in total.
inline __m256i pack_avx2_register_9b_be(__m256i cmp_data_epi16)
{
  // Keep only the 9 data bits of each sample.
  __m256i data_epi16 = _mm256_and_si256(cmp_data_epi16, _mm256_set1_epi16(0x01ff));

  // Merge each I/Q pair into one 18bit word (I in the upper bits) stored in a 32bit element: (I * 2^9) + Q.
  __m256i re_epi32 = _mm256_madd_epi16(data_epi16, _mm256_set1_epi32(0x00010200));

  // Merge each pair of 18bit words into one 36bit word (first RE in the upper bits) stored in a 64bit element:
  // (RE0 * 2^18) + RE1. The multiplication takes the even 32bit elements, the shift brings the odd ones.
  __m256i re_pair_epi64 =
      _mm256_add_epi64(_mm256_mul_epu32(re_epi32, _mm256_set1_epi64x(1 << 18)), _mm256_srli_epi64(re_epi32, 32));

  // Concatenate the two 36bit words of each 128bit lane, left-aligned in 72 bits: the upper 64 bits go to the lower
  // 64bit element and the remaining 8 bits to the top of the upper 64bit element.
  __m256i concat_hi_epi64 = _mm256_sllv_epi64(re_pair_epi64, _mm256_setr_epi64x(28, 56, 28, 56));
  __m256i concat_lo_epi64 = _mm256_srli_epi64(_mm256_srli_si256(re_pair_epi64, 8), 8);
  __m256i concat_epi64    = _mm256_or_si256(concat_hi_epi64, concat_lo_epi64);

  // Convert each 64bit element to big-endian.
  const __m256i bswap_epi64 =
      _mm256_setr_epi64x(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x0001020304050607, 0x08090a0b0c0d0e0f);
  return _mm256_shuffle_epi8(concat_epi64, bswap_epi64);
}

/// Stores the first 9 bytes of the given register at the given address, writing at most the 9 bytes after it.
inline void store_9_bytes(uint8_t* dest, __m128i reg)
{
  _mm_storel_epi64(reinterpret_cast<__m128i*>(dest), reg);
  dest[8] = static_cast<uint8_t>(_mm_extract_epi8(reg, 8));
}

/// \brief Packs 16bit IQ values of the two input RBs using 9bit big-endian format.
///
/// The following diagram shows the input format. Here RBx stands for one unique RE (pair of IQ samples, 32 bits long)
/// pertaining to a respective RB:
/// |       |         |         |         |         |
/// | ----- | ------- | ------- | ------- | ------- |
/// | \c r0: | RB0 RB0 | RB0 RB0 | RB0 RB0 | RB0 RB0 |
/// | \c r1: | RB0 RB0 | RB0 RB0 | RB1 RB1 | RB1 RB1 |
/// | \c r2: | RB1 RB1 | RB1 RB1 | RB1 RB1 | RB1 RB1 |
///
/// \param[out] comp_prb0_buffer Buffer dedicated for storing compressed packed bytes of the first PRB.
/// \param[out] comp_prb1_buffer Buffer dedicated for storing compressed packed bytes of the second PRB.
/// \param[in] r0                AVX2 register storing 16bit IQ samples of the first RB.
/// \param[in] r1                AVX2 register storing 16bit IQ samples of the first and second RB.
/// \param[in] r2                AVX2 register storing 16bit IQ samples of the second RB.
inline void avx2_pack_prbs_9b_big_endian(span<uint8_t> comp_prb0_buffer,
                                         span<uint8_t> comp_prb1_buffer,
                                         __m256i       r0,
                                         __m256i       r1,
                                         __m256i       r2)
{
  /// Number of bytes used by 1 packed resource block with IQ samples compressed to 9 bits.
  static constexpr unsigned BYTES_PER_PRB_9BIT_COMPRESSION = 27;
  /// Number of bytes packed from one 128bit lane (8 samples of 9 bits).
  static constexpr unsigned BYTES_PER_LANE = 9;

  ocudu_assert(comp_prb0_buffer.size() == BYTES_PER_PRB_9BIT_COMPRESSION,
               "Output buffer corresponding to the first PRB has incorrect size");
  ocudu_assert(comp_prb1_buffer.size() == BYTES_PER_PRB_9BIT_COMPRESSION,
               "Output buffer corresponding to the second PRB has incorrect size");

  // Each 128bit lane contains 9 packed bytes. Lanes are stored in order: 3 lanes per PRB.
  __m256i reg0_packed_epi8 = pack_avx2_register_9b_be(r0);
  __m256i reg1_packed_epi8 = pack_avx2_register_9b_be(r1);
  __m256i reg2_packed_epi8 = pack_avx2_register_9b_be(r2);

  // The first two lanes of each PRB are stored with full 16-byte writes. Each write is followed by another one that
  // overwrites its 7 padding bytes, and stays inside of the PRB. The last lane only writes its 9 bytes.
  uint8_t* prb0 = comp_prb0_buffer.data();
  uint8_t* prb1 = comp_prb1_buffer.data();
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0), _mm256_castsi256_si128(reg0_packed_epi8));
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0 + BYTES_PER_LANE), _mm256_extracti128_si256(reg0_packed_epi8, 1));
  store_9_bytes(prb0 + 2 * BYTES_PER_LANE, _mm256_castsi256_si128(reg1_packed_epi8));

  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1), _mm256_extracti128_si256(reg1_packed_epi8, 1));
  _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1 + BYTES_PER_LANE), _mm256_castsi256_si128(reg2_packed_epi8));
  store_9_bytes(prb1 + 2 * BYTES_PER_LANE, _mm256_extracti128_si256(reg2_packed_epi8, 1));
}

/// \brief Packs 16bit IQ values of the two input RBs using 16bit big-endian format.
///
/// The following diagram shows the input format. Here RBx stands for one unique RE (pair of IQ samples, 32 bits long)
/// pertaining to a respective RB:
/// |       |         |         |         |         |
/// | ----- | ------- | ------- | ------- | ------- |
/// | \c r0: | RB0 RB0 | RB0 RB0 | RB0 RB0 | RB0 RB0 |
/// | \c r1: | RB0 RB0 | RB0 RB0 | RB1 RB1 | RB1 RB1 |
/// | \c r2: | RB1 RB1 | RB1 RB1 | RB1 RB1 | RB1 RB1 |
///
/// \param[out] comp_prb0_buffer Buffer dedicated for storing compressed packed bytes of the first PRB.
/// \param[out] comp_prb1_buffer Buffer dedicated for storing compressed packed bytes of the second PRB.
/// \param[in] r0                AVX2 register storing 16bit IQ samples of the first RB.
/// \param[in] r1                AVX2 register storing 16bit IQ samples of the first and second RB.
/// \param[in] r2                AVX2 register storing 16bit IQ samples of the second RB.
inline void avx2_pack_prbs_16b_big_endian(span<uint8_t> comp_prb0_buffer,
                                          span<uint8_t> comp_prb1_buffer,
                                          __m256i       r0,
                                          __m256i       r1,
                                          __m256i       r2)
{
  static constexpr unsigned BYTES_PER_PRB_NO_COMPRESSION = 48;

  ocudu_assert(comp_prb0_buffer.size() == BYTES_PER_PRB_NO_COMPRESSION,
               "Output buffer corresponding to the first PRB has incorrect size");
  ocudu_assert(comp_prb1_buffer.size() == BYTES_PER_PRB_NO_COMPRESSION,
               "Output buffer corresponding to the second PRB has incorrect size");

  // Swap bytes to convert from big-endian format and write them directly to the output memory.
  const __m256i shuffle_mask_epi8 =
      _mm256_setr_epi64x(0x0607040502030001, 0x0e0f0c0d0a0b0809, 0x0607040502030001, 0x0e0f0c0d0a0b0809);

  __m256i reg0_swp_epi16 = _mm256_shuffle_epi8(r0, shuffle_mask_epi8);
  __m256i reg1_swp_epi16 = _mm256_shuffle_epi8(r1, shuffle_mask_epi8);
  __m256i reg2_swp_epi16 = _mm256_shuffle_epi8(r2, shuffle_mask_epi8);

  uint8_t* data = comp_prb0_buffer.data();
  _mm256_storeu_si256(reinterpret_cast<__m256i*>(data), reg0_swp_epi16);
  _mm256_maskstore_epi32(reinterpret_cast<int*>(data + 32),
                         _mm256_setr_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0, 0, 0, 0),
                         reg1_swp_epi16);

  data = comp_prb1_buffer.data();
  _mm_maskstore_epi32(reinterpret_cast<int*>(data),
                      _mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff),
                      _mm256_extracti128_si256(reg1_swp_epi16, 0x1));
  _mm256_storeu_si256(reinterpret_cast<__m256i*>(data + 16), reg2_swp_epi16);
}

/// \brief Packs 16bit IQ values of the two RBs using the specified width and big-endian format.
///
/// The following diagram shows the input format. Here RBx stands for one unique RE (pair of IQ samples, 32 bits long)
/// pertaining to a respective RB):
/// |       |         |         |         |         |
/// | ----- | ------- | ------- | ------- | ------- |
/// | \c r0: | RB0 RB0 | RB0 RB0 | RB0 RB0 | RB0 RB0 |
/// | \c r1: | RB0 RB0 | RB0 RB0 | RB1 RB1 | RB1 RB1 |
/// | \c r2: | RB1 RB1 | RB1 RB1 | RB1 RB1 | RB1 RB1 |
///
/// \param[out] comp_prb0_buffer Buffer dedicated for storing compressed packed bytes of the first PRB.
/// \param[out] comp_prb1_buffer Buffer dedicated for storing compressed packed bytes of the second PRB.
/// \param[in] r0                AVX2 register storing 16bit IQ pairs of the first PRB.
/// \param[in] r1                AVX2 register storing 16bit IQ pairs of the first and second PRB.
/// \param[in] r2                AVX2 register storing 16bit IQ pairs of the second PRB.
/// \param[in] iq_width          Bit width of the resulting packed IQ samples.
inline void pack_prbs_big_endian(span<uint8_t> comp_prb0_buffer,
                                 span<uint8_t> comp_prb1_buffer,
                                 __m256i       r0,
                                 __m256i       r1,
                                 __m256i       r2,
                                 unsigned      iq_width)
{
  switch (iq_width) {
    case 9:
      return avx2_pack_prbs_9b_big_endian(comp_prb0_buffer, comp_prb1_buffer, r0, r1, r2);
    case 16:
      return avx2_pack_prbs_16b_big_endian(comp_prb0_buffer, comp_prb1_buffer, r0, r1, r2);
    default:
      report_fatal_error("Unsupported bit width");
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
