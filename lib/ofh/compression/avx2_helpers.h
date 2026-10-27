// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief AVX2 support for OFH Compression algorithms.
///
/// The compressors process two PRBs at a time, as their 48 samples fill exactly three AVX2 registers of 16 16bit
/// samples. Each 128bit lane holds four REs of a single PRB, so the registers hold the following PRBs:
/// |                 | lane 0 | lane 1 |
/// | --------------- | ------ | ------ |
/// | first register  | RB0    | RB0    |
/// | second register | RB0    | RB1    |
/// | third register  | RB1    | RB1    |

#pragma once

#include "packing_utils_avx2.h"
#include "ocudu/adt/bf16.h"
#include "ocudu/adt/complex.h"
#include <cstdint>
#include <immintrin.h>
#include <type_traits>

namespace ocudu {
namespace ofh {
namespace mm256 {

/// \brief Quantizes 16 brain float values to 16bit integers, grouping the even and odd elements.
///
/// The values are scaled, rounded to the nearest integer (ties to even) and saturated to the 16bit range. Each 128bit
/// lane of the result holds the four even elements of the respective input lane followed by the four odd ones, that is
/// the I samples followed by the Q samples:
/// |                 | 128bit lane 0                     | 128bit lane 1                     |
/// | --------------- | --------------------------------- | --------------------------------- |
/// | \c bf16_epi16:  | I0 Q0 I1 Q1 I2 Q2 I3 Q3           | I4 Q4 I5 Q5 I6 Q6 I7 Q7           |
/// | return value:   | I0 I1 I2 I3 Q0 Q1 Q2 Q3           | I4 I5 I6 I7 Q4 Q5 Q6 Q7           |
///
/// It gives the same values as \c ocuduvec::convert, whose vector types cannot be used here as they widen to AVX512
/// when the build enables it. The callers fold the reordering of the samples into their own shuffles.
///
/// \note The conversion to integers rounds according to the MXCSR register, it assumes the default rounding mode.
///
/// \param[in] bf16_epi16 AVX2 register storing 16 brain floats.
/// \param[in] scale_ps   AVX2 register storing the scaling factor applied before the conversion in every element.
/// \param[in] max_ps     AVX2 register storing the largest scaled value in every element, up to the 16bit maximum.
/// Larger values saturate at it, but there is no lower limit other than the one of the 16bit range.
inline __m256i quantize_bf16_even_odd(__m256i bf16_epi16, __m256 scale_ps, __m256 max_ps = _mm256_set1_ps(INT16_MAX))
{
  // Unpack brain floats into two vectors of floats: even elements and odd ones. Same as ocudu_simd_bf16_loadu().
  __m256 even_ps = _mm256_castsi256_ps(_mm256_slli_epi32(bf16_epi16, 16));
  __m256 odd_ps  = _mm256_castsi256_ps(_mm256_and_si256(bf16_epi16, _mm256_set1_epi32(0xffff0000)));

  // The conversion gives the most negative integer for the values above the 32bit range, clamp them to saturate.
  even_ps = _mm256_min_ps(_mm256_mul_ps(even_ps, scale_ps), max_ps);
  odd_ps  = _mm256_min_ps(_mm256_mul_ps(odd_ps, scale_ps), max_ps);

  // Convert to integers (rounds to nearest even in the default rounding mode) and saturate to 16 bits.
  return _mm256_packs_epi32(_mm256_cvtps_epi32(even_ps), _mm256_cvtps_epi32(odd_ps));
}

/// \brief Quantizes and packs pairs of PRBs using 16bit big-endian samples.
///
/// A single byte shuffle restores the order of the samples quantized by \ref quantize_bf16_even_odd and converts them
/// to big-endian.
///
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte.
/// \param[out] out          Output bytes of the PRBs.
/// \param[in] in            Input samples of the PRBs.
/// \param[in] nof_prb_pairs Number of pairs of PRBs to process.
/// \param[in] prb_size      Size in bytes of one compressed PRB, including the compression parameter if present.
/// \param[in] scale_ps      AVX2 register storing the scaling factor applied before the conversion in every element.
/// \param[in] comp_param    Compression parameter written at the start of each PRB, if present.
template <bool HAS_COMP_PARAM>
inline void compress_prb_pairs_16b(uint8_t*      out,
                                   const bf16_t* in,
                                   unsigned      nof_prb_pairs,
                                   unsigned      prb_size,
                                   __m256        scale_ps,
                                   uint8_t       comp_param)
{
  // AVX2 register size in a number of 16bit words.
  static constexpr unsigned AVX2_REG_SIZE = 16;
  // Offset of the samples inside a PRB.
  static constexpr unsigned SAMPLES_OFFSET = HAS_COMP_PARAM ? 1 : 0;

  // Takes the 16bit elements 0, 4, 1, 5, 2, 6, 3, 7 of each lane with their two bytes swapped:
  // input lane:  I0 I1 I2 I3 Q0 Q1 Q2 Q3 (little-endian)
  // output lane: I0 Q0 I1 Q1 I2 Q2 I3 Q3 (big-endian)
  const __m256i bswap_mask_epi8 =
      _mm256_setr_epi64x(0x0a0b020308090001, 0x0e0f06070c0d0405, 0x0a0b020308090001, 0x0e0f06070c0d0405);
  for (unsigned pair = 0; pair != nof_prb_pairs; ++pair) {
    // Same register layout as calculate_exponents(): RB0 only, RB0 and RB1 (one per 128bit lane), RB1 only.
    __m256i rb0_epi16 = _mm256_shuffle_epi8(
        quantize_bf16_even_odd(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in)), scale_ps), bswap_mask_epi8);
    __m256i rb01_epi16 = _mm256_shuffle_epi8(
        quantize_bf16_even_odd(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + AVX2_REG_SIZE)), scale_ps),
        bswap_mask_epi8);
    __m256i rb1_epi16 = _mm256_shuffle_epi8(
        quantize_bf16_even_odd(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + AVX2_REG_SIZE * 2)), scale_ps),
        bswap_mask_epi8);

    // Write the 48 bytes of each PRB, preceded by the compression parameter if present: the first PRB takes the 8 REs
    // of rb0 and the 4 REs of the lower lane of rb01, the second PRB the 4 REs of the upper lane of rb01 and the 8 REs
    // of rb1.
    uint8_t* prb0 = out;
    uint8_t* prb1 = prb0 + prb_size;
    if (HAS_COMP_PARAM) {
      prb0[0] = comp_param;
      prb1[0] = comp_param;
    }
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(prb0 + SAMPLES_OFFSET), rb0_epi16);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0 + SAMPLES_OFFSET + 32), _mm256_castsi256_si128(rb01_epi16));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1 + SAMPLES_OFFSET), _mm256_extracti128_si256(rb01_epi16, 1));
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(prb1 + SAMPLES_OFFSET + 16), rb1_epi16);

    in += AVX2_REG_SIZE * 3;
    out += prb_size * 2;
  }
}

/// \brief Calculates the BFP exponents of two resource blocks passed in three AVX2 registers.
///
/// The exponent of a RB is the right shift required for its samples to fit in \c data_width bits, as given by
/// \c iq_compression_bfp_impl::determine_exponent. It depends only on the position of the most significant bit of the
/// largest magnitude of the RB, so the function computes the bitwise OR of the magnitudes instead of the maximum where
/// it is cheaper. The magnitude is the number of bits needed by a sample besides the sign: a positive \f$x\f$ needs as
/// many bits as \f$x\f$, and a negative one as many as \f$|x|-1\f$ (for example, -256 fits in 9 bits while 256 does
/// not), see O-RAN.WG4.CUS Annex A.1.2.
///
/// \param[in] rb0_epi16  AVX2 register storing 16bit IQ samples of the first RB (see the file brief).
/// \param[in] rb01_epi16 AVX2 register storing 16bit IQ samples of the first and second RBs.
/// \param[in] rb1_epi16  AVX2 register storing 16bit IQ samples of the second RB.
/// \param[in] data_width BFP compression bit width.
/// \return An AVX2 register whose lower 64 bits store the exponent of the first RB, and the lower 64 bits of the upper
/// 128bit lane the one of the second RB, the rest of bits are zero. Each 128bit lane is a shift count for
/// \c _mm256_sra_epi16, which shifts all the elements by the 64bit value in the lower half of its count operand.
inline __m256i calculate_exponents(__m256i rb0_epi16, __m256i rb01_epi16, __m256i rb1_epi16, unsigned data_width)
{
  // Reorganize the registers so that each 128bit lane contains samples of the same RB. The integer permute avoids a
  // bypass delay between the floating point and integer domains on some CPUs.
  __m256i rb0_lo_epi16 = _mm256_permute2x128_si256(rb0_epi16, rb1_epi16, 0x20);
  __m256i rb1_hi_epi16 = _mm256_permute2x128_si256(rb0_epi16, rb1_epi16, 0x31);

  // The magnitude of a sample x is max(x, ~x): ~x = -x - 1 is negative for positive values and |x| - 1 for negative
  // ones. The maximum magnitude of each lane is then the maximum of its largest sample and the inverted smallest one.
  __m256i max_epi16 = _mm256_max_epi16(_mm256_max_epi16(rb0_lo_epi16, rb1_hi_epi16), rb01_epi16);
  __m256i min_epi16 = _mm256_min_epi16(_mm256_min_epi16(rb0_lo_epi16, rb1_hi_epi16), rb01_epi16);
  __m256i mag_epi16 = _mm256_max_epi16(max_epi16, _mm256_xor_si256(min_epi16, _mm256_set1_epi32(-1)));

  // Horizontal OR inside each 128bit lane, the result is stored in the lowest 16 bits.
  __m256i or_epi16 = _mm256_or_si256(mag_epi16, _mm256_srli_si256(mag_epi16, 8));
  or_epi16         = _mm256_or_si256(or_epi16, _mm256_srli_si256(or_epi16, 4));
  or_epi16         = _mm256_or_si256(or_epi16, _mm256_srli_si256(or_epi16, 2));

  // Keep only the result in each lane, the rest of elements are zeroed.
  const __m256i keep_first_epi32 = _mm256_setr_epi32(0xffff, 0, 0, 0, 0xffff, 0, 0, 0);
  __m256i       max_abs_epi32    = _mm256_and_si256(or_epi16, keep_first_epi32);

  // Position of the most significant bit: the exponent field (bits 30 to 23) of the float conversion of an integer in
  // [2^p, 2^(p+1)) is 127 + p, and it is zero for a null value.
  __m256i float_exp_epi32 = _mm256_srli_epi32(_mm256_castps_si256(_mm256_cvtepi32_ps(max_abs_epi32)), 23);

  // The magnitude has p + 1 bits and a signed integer of data_width bits holds data_width - 1 of them, so the exponent
  // is max(0, (p + 1) - (data_width - 1)). Null values and the zeroed elements end up as zero.
  const int32_t exp_offset = 127 + static_cast<int32_t>(data_width) - 2;
  return _mm256_max_epi32(_mm256_sub_epi32(float_exp_epi32, _mm256_set1_epi32(exp_offset)), _mm256_setzero_si256());
}

/// \brief Rounds 8 floats to brain float, nearest with ties to even.
///
/// \param[in] value_ps AVX2 register storing 8 floats.
/// \return An AVX2 register whose 32bit elements hold the brain float in their upper 16 bits.
inline __m256i round_to_bf16_epi32(__m256 value_ps)
{
  // Same as to_bf16(): add 0x7fff plus the least significant bit of the result, so that ties round to even.
  __m256i value_epi32 = _mm256_castps_si256(value_ps);
  __m256i lsb_epi32   = _mm256_and_si256(_mm256_srli_epi32(value_epi32, 16), _mm256_set1_epi32(1));
  return _mm256_add_epi32(_mm256_add_epi32(value_epi32, _mm256_set1_epi32(0x7fff)), lsb_epi32);
}

/// \brief Interleaves the brain floats of the even and odd samples given by \ref round_to_bf16_epi32.
///
/// \param[in] even_epi32 AVX2 register storing the even samples in the upper halves of its 32bit elements.
/// \param[in] odd_epi32  AVX2 register storing the odd samples in the upper halves of its 32bit elements.
/// \return An AVX2 register storing the 16 interleaved brain floats.
inline __m256i interleave_bf16(__m256i even_epi32, __m256i odd_epi32)
{
  return _mm256_blend_epi16(_mm256_srli_epi32(even_epi32, 16), odd_epi32, 0xaa);
}

/// \brief Decompresses a block of one or two PRBs.
///
/// The samples are unpacked into 32bit integers, scaled in single precision and rounded to brain float, see
/// \ref prb_unpack_layout for the register layout.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte.
/// \tparam NOF_PRBS       Number of PRBs of the block, one or two.
/// \param[out] out       Output samples of the PRBs.
/// \param[in] in         Packed bytes of the PRBs.
/// \param[in] scale_0_ps AVX2 register storing the scaling factor of the first PRB in every element.
/// \param[in] scale_1_ps AVX2 register storing the scaling factor of the second PRB in every element.
template <unsigned BITS, bool HAS_COMP_PARAM, unsigned NOF_PRBS>
inline void decompress_prb_block(cbf16_t* out, const uint8_t* in, __m256 scale_0_ps, __m256 scale_1_ps)
{
  using layout = prb_unpack_layout<BITS, HAS_COMP_PARAM, NOF_PRBS>;
  // Number of 16bit samples stored by the first pair of registers of each PRB, as complex samples.
  static constexpr unsigned HEAD_SIZE = layout::NOF_HEAD_SAMPLES / 2;

  // Decompresses the samples of the given register pair.
  auto decompress_pair = [&](auto pair, __m256 scale_ps) {
    __m256i even_epi32;
    __m256i odd_epi32;
    unpack_prb_block_pair<layout, decltype(pair)::value>(even_epi32, odd_epi32, in);
    return interleave_bf16(round_to_bf16_epi32(_mm256_mul_ps(_mm256_cvtepi32_ps(even_epi32), scale_ps)),
                           round_to_bf16_epi32(_mm256_mul_ps(_mm256_cvtepi32_ps(odd_epi32), scale_ps)));
  };

  // First 16 samples of each PRB.
  _mm256_storeu_si256(reinterpret_cast<__m256i*>(out),
                      decompress_pair(std::integral_constant<unsigned, 0>(), scale_0_ps));
  if constexpr (NOF_PRBS == 2) {
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(out + NOF_SUBCARRIERS_PER_RB),
                        decompress_pair(std::integral_constant<unsigned, 1>(), scale_1_ps));
  }

  // Remaining 8 samples of each PRB, one PRB per lane.
  __m256  tail_scale_ps = (NOF_PRBS == 2) ? _mm256_blend_ps(scale_0_ps, scale_1_ps, 0xf0) : scale_0_ps;
  __m256i tail_epi16    = decompress_pair(std::integral_constant<unsigned, NOF_PRBS>(), tail_scale_ps);
  _mm_storeu_si128(reinterpret_cast<__m128i*>(out + HEAD_SIZE), _mm256_castsi256_si128(tail_epi16));
  if constexpr (NOF_PRBS == 2) {
    _mm_storeu_si128(reinterpret_cast<__m128i*>(out + NOF_SUBCARRIERS_PER_RB + HEAD_SIZE),
                     _mm256_extracti128_si256(tail_epi16, 1));
  }
}

/// \brief Decompresses the given PRBs, two at a time.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte.
/// \param[out] out         Output samples of the PRBs.
/// \param[in] in           Packed bytes of the PRBs.
/// \param[in] nof_prbs     Number of PRBs.
/// \param[in] scale_ps     AVX2 register storing the scaling factor of the samples before the BFP exponent in every
///                         element.
/// \param[in] get_exponent Function returning the BFP exponent of a PRB, given a pointer to its packed bytes.
template <unsigned BITS, bool HAS_COMP_PARAM, typename ExponentFunc>
inline void
decompress_prbs(cbf16_t* out, const uint8_t* in, unsigned nof_prbs, __m256 scale_ps, ExponentFunc get_exponent)
{
  using layout                       = prb_unpack_layout<BITS, HAS_COMP_PARAM, 1>;
  static constexpr unsigned PRB_SIZE = layout::PRB_SIZE;

  // Scaling of the unpacked samples, the exponent of each PRB is added to the exponent field of the float.
  const __m256i scaling_epi32 = _mm256_castps_si256(_mm256_mul_ps(scale_ps, _mm256_set1_ps(layout::SAMPLE_SCALE)));
  auto          get_scale     = [&](const uint8_t* prb) {
    int32_t exponent = static_cast<int32_t>(get_exponent(prb));
    return _mm256_castsi256_ps(_mm256_add_epi32(scaling_epi32, _mm256_set1_epi32(exponent << 23)));
  };

  for (unsigned prb = 0, nof_pairs = nof_prbs / 2; prb != 2 * nof_pairs; prb += 2) {
    const uint8_t* in_pair = in + prb * PRB_SIZE;
    decompress_prb_block<BITS, HAS_COMP_PARAM, 2>(
        out + prb * NOF_SUBCARRIERS_PER_RB, in_pair, get_scale(in_pair), get_scale(in_pair + PRB_SIZE));
  }
  if (nof_prbs % 2 != 0) {
    const uint8_t* in_last       = in + (nof_prbs - 1) * PRB_SIZE;
    __m256         last_scale_ps = get_scale(in_last);
    decompress_prb_block<BITS, HAS_COMP_PARAM, 1>(
        out + (nof_prbs - 1) * NOF_SUBCARRIERS_PER_RB, in_last, last_scale_ps, last_scale_ps);
  }
}

} // namespace mm256
} // namespace ofh
} // namespace ocudu
