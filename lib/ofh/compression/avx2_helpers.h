// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief AVX2 support for OFH Compression algorithms.

#pragma once

#include "ocudu/adt/span.h"
#include <immintrin.h>
#include <limits>

namespace ocudu {
namespace ofh {
namespace mm256 {

/// \brief Quantizes 16 brain float values to 16bit integers, grouping even and odd elements.
///
/// Values are scaled, rounded to the nearest integer (ties to even) and saturated to the 16bit range. Each 128bit lane
/// of the result contains the four even elements of the respective input lane followed by the four odd ones.
///
/// \note The rounding is performed by the conversion to integers, so it assumes the default rounding mode of the MXCSR
/// register (round to nearest even).
///
/// \param[in] bf16_epi16 AVX2 register storing 16 brain floats.
/// \param[in] scale_ps   AVX2 register storing the scaling factor applied before the conversion in every element.
inline __m256i quantize_bf16_even_odd(__m256i bf16_epi16, __m256 scale_ps)
{
  // Unpack brain floats into two vectors of floats: even elements and odd ones.
  __m256 even_ps = _mm256_castsi256_ps(_mm256_slli_epi32(bf16_epi16, 16));
  __m256 odd_ps  = _mm256_castsi256_ps(_mm256_and_si256(bf16_epi16, _mm256_set1_epi32(0xffff0000)));

  // The conversion gives the most negative integer for the values above the 32bit range, clamp them to saturate.
  const __m256 max_ps = _mm256_set1_ps(static_cast<float>(std::numeric_limits<int16_t>::max()));
  even_ps             = _mm256_min_ps(_mm256_mul_ps(even_ps, scale_ps), max_ps);
  odd_ps              = _mm256_min_ps(_mm256_mul_ps(odd_ps, scale_ps), max_ps);

  // Convert to integers (rounds to nearest even in the default rounding mode) and saturate to 16 bits.
  return _mm256_packs_epi32(_mm256_cvtps_epi32(even_ps), _mm256_cvtps_epi32(odd_ps));
}

/// \brief Quantizes 16 brain float values to 16bit integers.
///
/// Same as \ref quantize_bf16_even_odd, but the result keeps the order of the input values.
///
/// \param[in] bf16_epi16 AVX2 register storing 16 brain floats.
/// \param[in] scale_ps   AVX2 register storing the scaling factor applied before the conversion in every element.
inline __m256i quantize_bf16(__m256i bf16_epi16, __m256 scale_ps)
{
  // Interleave even and odd elements to restore the original order.
  const __m256i interleave_mask =
      _mm256_setr_epi64x(0x0b0a030209080100, 0x0f0e07060d0c0504, 0x0b0a030209080100, 0x0f0e07060d0c0504);
  return _mm256_shuffle_epi8(quantize_bf16_even_odd(bf16_epi16, scale_ps), interleave_mask);
}

/// \brief Calculates the BFP exponents of the two input resource blocks passed in three AVX2 registers.
///
/// The content of the three input AVX2 registers is represented in the table below. Here RBx means one unique RE
/// (pair of IQ samples, 32 bits long) pertaining to a respective RB:
/// |       |         |         |         |         |
/// | ----- | ------- | ------- | ------- | ------- |
/// | \c rb0_epi16:  | RB0 RB0 | RB0 RB0 | RB0 RB0 | RB0 RB0 |
/// | \c rb01_epi16: | RB0 RB0 | RB0 RB0 | RB1 RB1 | RB1 RB1 |
/// | \c rb1_epi16:  | RB1 RB1 | RB1 RB1 | RB1 RB1 | RB1 RB1 |
///
/// The exponent of a RB is the right shift required for its samples to fit in \c data_width bits (see
/// \c iq_compression_bfp_impl::determine_exponent). It only depends on the position of the most significant bit of the
/// maximum absolute value of the RB, so instead of the maximum the function uses the bitwise OR of the magnitudes of
/// all samples of the RB, which has the same most significant bit and is cheaper to compute. The magnitude of a sample
/// is its absolute value, except for negative ones, whose magnitude is one lower (O-RAN.WG4.CUS, Annex A.1.2: the most
/// significant bit of a negative value can be one higher). Both cases are covered by xor-ing the sample with its sign.
///
/// The position of the most significant bit is obtained from the exponent field of the float conversion of the value,
/// so that the exponents are calculated without leaving the vector domain.
///
/// \param[in] rb0_epi16  AVX2 register storing 16bit IQ pairs of the first RB.
/// \param[in] rb01_epi16 AVX2 register storing 16bit IQ pairs of the first and second RBs.
/// \param[in] rb1_epi16  AVX2 register storing 16bit IQ pairs of the second RB.
/// \param[in] data_width BFP compression bit width.
/// \return An AVX2 register whose lower 64 bits store the exponent of the first RB, and the upper 128bit lane the one
/// of the second RB. Each exponent is stored as a 64bit integer, which allows using it as a shift count.
inline __m256i calculate_exponents(__m256i rb0_epi16, __m256i rb01_epi16, __m256i rb1_epi16, unsigned data_width)
{
  // Magnitude of each sample.
  __m256i rb0_mag_epi16  = _mm256_xor_si256(rb0_epi16, _mm256_srai_epi16(rb0_epi16, 15));
  __m256i rb01_mag_epi16 = _mm256_xor_si256(rb01_epi16, _mm256_srai_epi16(rb01_epi16, 15));
  __m256i rb1_mag_epi16  = _mm256_xor_si256(rb1_epi16, _mm256_srai_epi16(rb1_epi16, 15));

  // Reorganize vectors so that each 128bit lane contains samples of the same RB, and combine them: the lower lane
  // pertains to the first RB and the upper lane to the second one.
  __m256i rb0_lo_epi16 = _mm256_permute2x128_si256(rb0_mag_epi16, rb1_mag_epi16, 0x20);
  __m256i rb1_hi_epi16 = _mm256_permute2x128_si256(rb0_mag_epi16, rb1_mag_epi16, 0x31);
  __m256i or_epi16     = _mm256_or_si256(_mm256_or_si256(rb0_lo_epi16, rb1_hi_epi16), rb01_mag_epi16);

  // Horizontal OR inside each 128bit lane, the result is stored in the lowest 16 bits.
  or_epi16 = _mm256_or_si256(or_epi16, _mm256_srli_si256(or_epi16, 8));
  or_epi16 = _mm256_or_si256(or_epi16, _mm256_srli_si256(or_epi16, 4));
  or_epi16 = _mm256_or_si256(or_epi16, _mm256_srli_si256(or_epi16, 2));

  // Keep only the result in each lane, the rest of elements are zeroed.
  const __m256i keep_first_epi32 = _mm256_setr_epi32(0xffff, 0, 0, 0, 0xffff, 0, 0, 0);
  __m256i       max_abs_epi32    = _mm256_and_si256(or_epi16, keep_first_epi32);

  // The biased exponent of the float representation of a positive integer is 127 plus the position of its most
  // significant bit, and zero for a null value.
  __m256i float_exp_epi32 = _mm256_srli_epi32(_mm256_castps_si256(_mm256_cvtepi32_ps(max_abs_epi32)), 23);

  // The exponent is the number of bits above the data width, taking into account the sign bit, or zero. Elements that
  // were zeroed above end up as zero.
  const int32_t exp_offset = 127 + static_cast<int32_t>(data_width) - 2;
  return _mm256_max_epi32(_mm256_sub_epi32(float_exp_epi32, _mm256_set1_epi32(exp_offset)), _mm256_setzero_si256());
}

} // namespace mm256
} // namespace ofh
} // namespace ocudu
