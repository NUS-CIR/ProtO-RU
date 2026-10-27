// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief AVX512 support for OFH Compression algorithms.
///
/// The compressors process four PRBs at a time, as their 96 samples fill exactly three AVX512 registers of 32 16bit
/// samples. Each 128bit lane holds four REs of a single PRB, so the registers hold the following PRBs:
/// |                 | lane 0 | lane 1 | lane 2 | lane 3 |
/// | --------------- | ------ | ------ | ------ | ------ |
/// | first register  | RB0    | RB0    | RB0    | RB1    |
/// | second register | RB1    | RB1    | RB2    | RB2    |
/// | third register  | RB2    | RB3    | RB3    | RB3    |

#pragma once

#include <cstdint>
#include <immintrin.h>

namespace ocudu {
namespace ofh {
namespace mm512 {

/// \brief Quantizes 32 brain float values to 16bit integers, grouping the even and odd elements.
///
/// The values are scaled, rounded to the nearest integer (ties to even) and saturated to the 16bit range. Each 128bit
/// lane of the result holds the four even elements of the respective input lane followed by the four odd ones, that is
/// the I samples followed by the Q samples:
/// |                 | 128bit lane 0                     | 128bit lane 1                     | ...
/// | --------------- | --------------------------------- | --------------------------------- | ---
/// | \c bf16_epi16:  | I0 Q0 I1 Q1 I2 Q2 I3 Q3           | I4 Q4 I5 Q5 I6 Q6 I7 Q7           | ...
/// | return value:   | I0 I1 I2 I3 Q0 Q1 Q2 Q3           | I4 I5 I6 I7 Q4 Q5 Q6 Q7           | ...
///
/// It gives the same values as \c ocuduvec::convert, the callers fold the reordering of the samples into their own
/// shuffles.
///
/// \note The conversion to integers rounds according to the MXCSR register, it assumes the default rounding mode.
///
/// \param[in] bf16_epi16 AVX512 register storing 32 brain floats.
/// \param[in] scale_ps   AVX512 register storing the scaling factor applied before the conversion in every element.
/// \param[in] max_ps     AVX512 register storing the largest scaled value in every element, up to the 16bit maximum.
/// Larger values saturate at it, but there is no lower limit other than the one of the 16bit range.
inline __m512i quantize_bf16_even_odd(__m512i bf16_epi16, __m512 scale_ps, __m512 max_ps = _mm512_set1_ps(INT16_MAX))
{
  // Unpack brain floats into two vectors of floats: even elements and odd ones.
  __m512 even_ps = _mm512_castsi512_ps(_mm512_slli_epi32(bf16_epi16, 16));
  __m512 odd_ps  = _mm512_castsi512_ps(_mm512_and_si512(bf16_epi16, _mm512_set1_epi32(0xffff0000)));

  // The conversion gives the most negative integer for the values above the 32bit range, clamp them to saturate.
  even_ps = _mm512_min_ps(_mm512_mul_ps(even_ps, scale_ps), max_ps);
  odd_ps  = _mm512_min_ps(_mm512_mul_ps(odd_ps, scale_ps), max_ps);

  // Convert to integers and saturate to 16 bits, the pack works inside each 128bit lane.
  return _mm512_packs_epi32(_mm512_cvtps_epi32(even_ps), _mm512_cvtps_epi32(odd_ps));
}

/// \brief Calculates the BFP exponents of four resource blocks passed in three AVX512 registers.
///
/// The exponent of a RB is the right shift required for its samples to fit in \c data_width bits, as given by
/// \c iq_compression_bfp_impl::determine_exponent. It depends only on the position of the most significant bit of the
/// largest magnitude of the RB. The magnitude is the number of bits needed by a sample besides the sign: a positive
/// \f$x\f$ needs as many bits as \f$x\f$, and a negative one as many as \f$|x|-1\f$ (for example, -256 fits in 9 bits
/// while 256 does not), see O-RAN.WG4.CUS Annex A.1.2.
///
/// \param[in] r0_epi16   AVX512 register storing 16bit IQ samples of the first and second RBs (see the file brief).
/// \param[in] r1_epi16   AVX512 register storing 16bit IQ samples of the second and third RBs.
/// \param[in] r2_epi16   AVX512 register storing 16bit IQ samples of the third and fourth RBs.
/// \param[in] data_width BFP compression bit width.
/// \return An AVX512 register whose 128bit lanes hold the exponent of the respective RB in all their 16bit elements.
inline __m512i calculate_exponents(__m512i r0_epi16, __m512i r1_epi16, __m512i r2_epi16, unsigned data_width)
{
  // Reorganize the registers so that the n-th lane of each one holds samples of the n-th RB.
  __m512i v0_epi16 =
      _mm512_mask_shuffle_i64x2(r0_epi16, 0b11111100, r1_epi16, r2_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(1, 0, 0, 0));
  __m512i v1_epi16 =
      _mm512_mask_shuffle_i64x2(r1_epi16, 0b11000011, r0_epi16, r2_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(2, 0, 0, 1));
  __m512i v2_epi16 =
      _mm512_mask_shuffle_i64x2(r2_epi16, 0b00111111, r0_epi16, r1_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(0, 3, 3, 2));

  // The magnitude of a sample x is max(x, ~x): ~x = -x - 1 is negative for positive values and |x| - 1 for negative
  // ones. The maximum magnitude of each lane is then the maximum of its largest sample and the inverted smallest one.
  __m512i max_epi16 = _mm512_max_epi16(_mm512_max_epi16(v0_epi16, v1_epi16), v2_epi16);
  __m512i min_epi16 = _mm512_min_epi16(_mm512_min_epi16(v0_epi16, v1_epi16), v2_epi16);
  __m512i mag_epi16 = _mm512_max_epi16(max_epi16, _mm512_xor_si512(min_epi16, _mm512_set1_epi32(-1)));

  // Bitwise OR of the magnitudes of each lane, which has the same most significant bit as their maximum. The result is
  // replicated in all the elements of the lane.
  mag_epi16 = _mm512_or_si512(mag_epi16, _mm512_shuffle_epi32(mag_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(1, 0, 3, 2)));
  mag_epi16 = _mm512_or_si512(mag_epi16, _mm512_shuffle_epi32(mag_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(2, 3, 0, 1)));
  mag_epi16 = _mm512_or_si512(mag_epi16, _mm512_rol_epi32(mag_epi16, 16));

  // Each 32bit element holds the 16bit magnitude twice, so its leading zero count is the one of the 16bit magnitude.
  __m512i lzcnt_epi32 = _mm512_lzcnt_epi32(mag_epi16);

  // A magnitude with 16 - lzcnt bits needs a signed integer of 17 - lzcnt bits, so the exponent is
  // max(0, (17 - lzcnt) - data_width). The saturated subtraction keeps the upper 16 bits of the elements zero.
  __m512i exponent_epi32 = _mm512_subs_epu16(_mm512_set1_epi32(17 - static_cast<int>(data_width)), lzcnt_epi32);

  // Replicate the exponent in all the 16bit elements of each lane.
  return _mm512_shuffle_epi8(exponent_epi32, _mm512_set1_epi16(0x0100));
}

} // namespace mm512
} // namespace ofh
} // namespace ocudu
