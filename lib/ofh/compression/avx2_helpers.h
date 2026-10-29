// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief AVX2 support for OFH Compression algorithms.
///
/// The compressors and decompressors process two PRBs at a time, as their 48 samples fill exactly three AVX2 registers
/// of 16 16bit samples. Each 128bit lane holds four REs of a single PRB, so the registers hold the following PRBs:
/// |                 | lane 0 | lane 1 |
/// | --------------- | ------ | ------ |
/// | first register  | RB0    | RB0    |
/// | second register | RB0    | RB1    |
/// | third register  | RB1    | RB1    |
///
/// The compression of a pair of PRBs goes through the following steps:
/// 1. the brain floats are scaled and converted to 16bit integers in registers, see \ref quantize_bf16_even_odd;
/// 2. with BFP, the exponent of each PRB is found from the largest magnitude of its samples, see
///    \ref calculate_exponents, and the samples are shifted right by it;
/// 3. the samples are packed in big-endian into the output bytes, see packing_utils_avx2.h.
/// The decompression unpacks the samples into 32bit integers, scales them in single precision and rounds them to brain
/// float in registers, see \ref decompress_prb_block.

#pragma once

#include "packing_utils_avx2.h"
#include "simd_loop_utils.h"
#include "ocudu/adt/bf16.h"
#include "ocudu/adt/complex.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/support/compiler.h"
#include <cstdint>
#include <immintrin.h>
#include <type_traits>

namespace ocudu {
namespace ofh {
namespace mm256 {

/// Number of 16bit samples per PRB.
constexpr unsigned NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;
/// Number of 16bit samples per AVX2 register.
constexpr unsigned NOF_SAMPLES_PER_REG = 16;

/// \brief Quantizes 16 brain float values to 16bit integers, grouping the even and odd elements.
///
/// The values are scaled, rounded to the nearest integer (ties to even) and saturated to the 16bit range, like
/// \c quantizer::to_fixed_point. Each 128bit lane of the result holds the four even elements of the respective input
/// lane followed by the four odd ones, that is the I samples followed by the Q samples:
/// |                 | 128bit lane 0                     | 128bit lane 1                     |
/// | --------------- | --------------------------------- | --------------------------------- |
/// | \c bf16_epi16:  | I0 Q0 I1 Q1 I2 Q2 I3 Q3           | I4 Q4 I5 Q5 I6 Q6 I7 Q7           |
/// | return value:   | I0 I1 I2 I3 Q0 Q1 Q2 Q3           | I4 I5 I6 I7 Q4 Q5 Q6 Q7           |
/// The callers fold the reordering of the samples into their own shuffles.
///
/// \note The conversion to integers rounds according to the MXCSR register, it assumes the default rounding mode.
///
/// \param[in] bf16_epi16  AVX2 register storing 16 brain floats.
/// \param[in] scale_ps    AVX2 register storing the scaling factor applied before the conversion in every element.
/// \param[in] clamp_epi16 AVX2 register storing \ref get_bf16_saturation_clamp of the scale in every element.
/// \return An AVX2 register storing the quantized values.
inline __m256i quantize_bf16_even_odd(__m256i bf16_epi16, __m256 scale_ps, __m256i clamp_epi16)
{
  // Clamp the large positive values, so that the conversion to integers below does not wrap around.
  bf16_epi16 = _mm256_min_epi16(bf16_epi16, clamp_epi16);

  // Widen the brain floats to floats by placing them in the upper half of 32bit elements: the even elements are
  // shifted up and the odd ones are already there, their lower half is masked out.
  __m256 even_ps = _mm256_castsi256_ps(_mm256_slli_epi32(bf16_epi16, 16));
  __m256 odd_ps  = _mm256_castsi256_ps(_mm256_and_si256(bf16_epi16, _mm256_set1_epi32(0xffff0000)));

  // Scale, convert to integers and saturate to 16 bits. The pack works inside each 128bit lane.
  return _mm256_packs_epi32(_mm256_cvtps_epi32(_mm256_mul_ps(even_ps, scale_ps)),
                            _mm256_cvtps_epi32(_mm256_mul_ps(odd_ps, scale_ps)));
}

/// \brief Quantizes 16 brain float values to 16bit integers saturating at the given full scale.
///
/// Same as \ref quantize_bf16_even_odd, with the quantized values clamped to the range [-full_scale, full_scale] of a
/// bit width below 16 bits. The upper clamp is applied in the floating point domain, which schedules better here than
/// clamping twice in the integer domain.
///
/// \param[in] bf16_epi16           AVX2 register storing 16 brain floats.
/// \param[in] scale_ps             AVX2 register storing the scaling factor applied before the conversion in every
///                                 element.
/// \param[in] full_scale_ps        AVX2 register storing the full scale in every element.
/// \param[in] neg_full_scale_epi16 AVX2 register storing the negated full scale in every element.
/// \return An AVX2 register storing the quantized values, with the layout of \ref quantize_bf16_even_odd.
inline __m256i quantize_bf16_even_odd_full_scale(__m256i bf16_epi16,
                                                 __m256  scale_ps,
                                                 __m256  full_scale_ps,
                                                 __m256i neg_full_scale_epi16)
{
  __m256 even_ps = _mm256_castsi256_ps(_mm256_slli_epi32(bf16_epi16, 16));
  __m256 odd_ps  = _mm256_castsi256_ps(_mm256_and_si256(bf16_epi16, _mm256_set1_epi32(0xffff0000)));

  // The scaled values above the full scale saturate in the float domain. The ones below the negative full scale
  // saturate at -32768 in the conversion and pack (the conversion gives the most negative integer if it overflows),
  // and then at the negative full scale.
  even_ps = _mm256_min_ps(_mm256_mul_ps(even_ps, scale_ps), full_scale_ps);
  odd_ps  = _mm256_min_ps(_mm256_mul_ps(odd_ps, scale_ps), full_scale_ps);
  return _mm256_max_epi16(_mm256_packs_epi32(_mm256_cvtps_epi32(even_ps), _mm256_cvtps_epi32(odd_ps)),
                          neg_full_scale_epi16);
}

/// \brief Calculates the BFP exponents of two resource blocks passed in three AVX2 registers.
///
/// The exponent of a RB is the right shift required for its samples to fit in \c data_width bits, as given by
/// \c iq_compression_bfp_impl::determine_exponent. It depends only on the position of the most significant bit of the
/// largest magnitude of the RB. The magnitude is the number of bits that a sample needs besides the sign: a positive
/// \f$x\f$ needs as many bits as \f$x\f$, and a negative one as many as \f$|x|-1\f$ (for example, -256 fits in 9 bits
/// while 256 does not), see O-RAN.WG4.CUS Annex A.1.2.
///
/// \param[in] rb0_epi16  AVX2 register storing 16bit IQ samples of the first RB (see the file brief).
/// \param[in] rb01_epi16 AVX2 register storing 16bit IQ samples of the first and second RBs.
/// \param[in] rb1_epi16  AVX2 register storing 16bit IQ samples of the second RB.
/// \param[in] data_width BFP compression bit width.
/// \return An AVX2 register whose 128bit lanes hold the exponent of the respective RB in their lowest 32bit element,
/// the other elements are zero. Each lane is then a valid shift count for \c _mm256_sra_epi16, which shifts all the
/// elements by the 64bit value in the lower half of its count operand.
inline __m256i calculate_exponents(__m256i rb0_epi16, __m256i rb01_epi16, __m256i rb1_epi16, unsigned data_width)
{
  // Reorganize the registers so that each 128bit lane holds samples of the same RB:
  // [ RB0 | RB1 ], [ RB0 | RB1 ] and rb01_epi16 itself.
  __m256i v0_epi16 = _mm256_permute2x128_si256(rb0_epi16, rb1_epi16, 0x20);
  __m256i v1_epi16 = _mm256_permute2x128_si256(rb0_epi16, rb1_epi16, 0x31);

  // The magnitude of a sample x is max(x, ~x), where ~x = -x - 1:
  //  - for x >= 0, ~x is negative, so the maximum is x itself;
  //  - for x < 0, ~x = |x| - 1 is non-negative and larger than x, the magnitude of a negative value given above.
  // The maximum magnitude of a lane is therefore the maximum of its largest sample and the bitwise inverse of its
  // smallest one.
  __m256i max_epi16 = _mm256_max_epi16(_mm256_max_epi16(v0_epi16, v1_epi16), rb01_epi16);
  __m256i min_epi16 = _mm256_min_epi16(_mm256_min_epi16(v0_epi16, v1_epi16), rb01_epi16);
  __m256i mag_epi16 = _mm256_max_epi16(max_epi16, _mm256_xor_si256(min_epi16, _mm256_set1_epi32(-1)));

  // The bitwise OR of the magnitudes of a lane has the same most significant bit as their maximum and is cheaper to
  // reduce. The result ends up in the lowest 16 bits of the lane, the other elements are masked out.
  __m256i or_epi16               = _mm256_or_si256(mag_epi16, _mm256_srli_si256(mag_epi16, 8));
  or_epi16                       = _mm256_or_si256(or_epi16, _mm256_srli_si256(or_epi16, 4));
  or_epi16                       = _mm256_or_si256(or_epi16, _mm256_srli_si256(or_epi16, 2));
  const __m256i keep_first_epi32 = _mm256_setr_epi32(0xffff, 0, 0, 0, 0xffff, 0, 0, 0);
  __m256i       max_abs_epi32    = _mm256_and_si256(or_epi16, keep_first_epi32);

  // Find the position p of the most significant bit of the magnitude through its conversion to float, as AVX2 has no
  // leading zero count: the exponent field (bits 30 to 23) of the float of an integer in [2^p, 2^(p+1)) is 127 + p.
  // The field is zero for a null magnitude.
  __m256i float_exp_epi32 = _mm256_srli_epi32(_mm256_castps_si256(_mm256_cvtepi32_ps(max_abs_epi32)), 23);

  // A magnitude with p + 1 bits needs a signed integer of p + 2 bits, so the exponent is max(0, (p + 2) - data_width).
  // The null values and the masked elements end up as zero.
  const int32_t exponent_offset = 127 + static_cast<int32_t>(data_width) - 2;
  return _mm256_max_epi32(_mm256_sub_epi32(float_exp_epi32, _mm256_set1_epi32(exponent_offset)),
                          _mm256_setzero_si256());
}

/// Quantized samples of a pair of PRBs, with the register layout described in the file brief.
struct prb_pair_samples {
  __m256i rb0_epi16;
  __m256i rb01_epi16;
  __m256i rb1_epi16;
};

/// Quantized samples of a pair of PRBs and their BFP exponents, see \ref calculate_exponents.
struct bfp_prb_pair {
  prb_pair_samples samples;
  __m256i          exponents;
};

/// \brief Loads the brain floats of a pair of PRBs and quantizes them.
///
/// \tparam Quantizer   Type of the quantizer, callable with the AVX2 register of 16 brain floats to quantize.
/// \param[in] in       Input samples of the PRBs.
/// \param[in] quantize Quantizer, see \ref quantize_bf16_even_odd.
/// \return The quantized samples.
template <typename Quantizer>
inline prb_pair_samples quantize_prb_pair(const bf16_t* in, Quantizer&& quantize)
{
  auto load = [&](unsigned reg) {
    return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + reg * NOF_SAMPLES_PER_REG));
  };
  return {quantize(load(0)), quantize(load(1)), quantize(load(2))};
}

/// \brief Compresses pairs of PRBs with 9 bits per sample and no BFP exponent.
///
/// The loop is software pipelined, see \ref run_pipelined_loop: a pair is quantized while the previous ones are
/// packed.
///
/// \param[out] out          Output bytes of the PRBs.
/// \param[in] in            Input samples of the PRBs.
/// \param[in] nof_prb_pairs Number of pairs of PRBs to process.
/// \param[in] scale_ps      AVX2 register storing the scaling factor applied before the conversion in every element.
/// \param[in] full_scale    Largest quantized value, the samples saturate at it and at its negation.
OCUDU_FLATTEN inline void
compress_prb_pairs_9b(uint8_t* out, const bf16_t* in, unsigned nof_prb_pairs, __m256 scale_ps, float full_scale)
{
  // Size in bytes of a packed PRB.
  static constexpr unsigned PRB_SIZE = 27;

  const __m256  full_scale_ps        = _mm256_set1_ps(full_scale);
  const __m256i neg_full_scale_epi16 = _mm256_set1_epi16(static_cast<int16_t>(-full_scale));

  auto quantize_pair = [&](unsigned pair) {
    return quantize_prb_pair(in + pair * 2 * NOF_SAMPLES_PER_PRB, [&](__m256i bf16_epi16) {
      return quantize_bf16_even_odd_full_scale(bf16_epi16, scale_ps, full_scale_ps, neg_full_scale_epi16);
    });
  };
  auto pack_pair = [&](const prb_pair_samples& samples, unsigned pair) {
    uint8_t* prb0 = out + pair * 2 * PRB_SIZE;
    avx2_pack_prbs_9b_big_endian(prb0, prb0 + PRB_SIZE, samples.rb0_epi16, samples.rb01_epi16, samples.rb1_epi16);
  };
  run_pipelined_loop(nof_prb_pairs, quantize_pair, pack_pair);
}

/// \brief Compresses pairs of PRBs with BFP and 9 bits per sample.
///
/// The loop is software pipelined, see \ref run_pipelined_loop: a pair is quantized and its exponents found while
/// the previous ones are shifted and packed.
///
/// \param[out] out            Output bytes of the PRBs.
/// \param[in] in              Input samples of the PRBs.
/// \param[in] nof_prb_pairs   Number of pairs of PRBs to process.
/// \param[in] scale_ps        AVX2 register storing the scaling factor applied before the conversion in every element.
/// \param[in] clamp_epi16     AVX2 register storing \ref get_bf16_saturation_clamp of the scale in every element.
/// \param[in] exponent_offset Offset added to the exponent in the compression parameter of each PRB.
OCUDU_FLATTEN inline void compress_prb_pairs_bfp_9b(uint8_t*      out,
                                                    const bf16_t* in,
                                                    unsigned      nof_prb_pairs,
                                                    __m256        scale_ps,
                                                    __m256i       clamp_epi16,
                                                    uint8_t       exponent_offset)
{
  // Bit width of the packed samples.
  static constexpr unsigned DATA_WIDTH = 9;
  // Size in bytes of a packed PRB, including the compression parameter.
  static constexpr unsigned PRB_SIZE = 28;

  auto quantize_pair = [&](unsigned pair) {
    bfp_prb_pair state;
    state.samples = quantize_prb_pair(in + pair * 2 * NOF_SAMPLES_PER_PRB, [&](__m256i bf16_epi16) {
      return quantize_bf16_even_odd(bf16_epi16, scale_ps, clamp_epi16);
    });
    state.exponents =
        calculate_exponents(state.samples.rb0_epi16, state.samples.rb01_epi16, state.samples.rb1_epi16, DATA_WIDTH);
    return state;
  };

  auto pack_pair = [&](const bfp_prb_pair& state, unsigned pair) {
    uint8_t* prb0 = out + pair * 2 * PRB_SIZE;
    uint8_t* prb1 = prb0 + PRB_SIZE;

    // Shift the samples right by the exponent of their RB. The register shared by both RBs takes its lower lane
    // shifted by the first exponent and its upper lane by the second one.
    __m128i exponent_0      = _mm256_castsi256_si128(state.exponents);
    __m128i exponent_1      = _mm256_extracti128_si256(state.exponents, 1);
    __m256i rb0_sr_epi16    = _mm256_sra_epi16(state.samples.rb0_epi16, exponent_0);
    __m256i rb1_sr_epi16    = _mm256_sra_epi16(state.samples.rb1_epi16, exponent_1);
    __m256i rb01_sr_0_epi16 = _mm256_sra_epi16(state.samples.rb01_epi16, exponent_0);
    __m256i rb01_sr_1_epi16 = _mm256_sra_epi16(state.samples.rb01_epi16, exponent_1);
    __m256i rb01_sr_epi16   = _mm256_blend_epi32(rb01_sr_0_epi16, rb01_sr_1_epi16, 0xf0);

    // Write the compression parameter of each PRB followed by its packed samples.
    prb0[0] = static_cast<uint8_t>(_mm_cvtsi128_si32(exponent_0) + exponent_offset);
    prb1[0] = static_cast<uint8_t>(_mm_cvtsi128_si32(exponent_1) + exponent_offset);
    avx2_pack_prbs_9b_big_endian(prb0 + 1, prb1 + 1, rb0_sr_epi16, rb01_sr_epi16, rb1_sr_epi16);
  };

  run_pipelined_loop(nof_prb_pairs, quantize_pair, pack_pair);
}

/// \brief Compresses pairs of PRBs with 16 bits per sample.
///
/// The samples use all the bits of the data width, so the BFP exponent is always zero and the samples are not
/// shifted. A single byte shuffle restores the order of the samples quantized by \ref quantize_bf16_even_odd and
/// converts them to big-endian.
///
/// \tparam HAS_COMP_PARAM   Whether each PRB starts with a compression parameter byte.
/// \param[out] out          Output bytes of the PRBs.
/// \param[in] in            Input samples of the PRBs.
/// \param[in] nof_prb_pairs Number of pairs of PRBs to process.
/// \param[in] scale_ps      AVX2 register storing the scaling factor applied before the conversion in every element.
/// \param[in] clamp_epi16   AVX2 register storing \ref get_bf16_saturation_clamp of the scale in every element.
/// \param[in] comp_param    Compression parameter written at the start of each PRB, if present.
template <bool HAS_COMP_PARAM>
OCUDU_FLATTEN inline void compress_prb_pairs_16b(uint8_t*      out,
                                                 const bf16_t* in,
                                                 unsigned      nof_prb_pairs,
                                                 __m256        scale_ps,
                                                 __m256i       clamp_epi16,
                                                 uint8_t       comp_param)
{
  // Offset of the samples inside a PRB.
  static constexpr unsigned SAMPLES_OFFSET = HAS_COMP_PARAM ? 1 : 0;
  // Size in bytes of a packed PRB, including the compression parameter if present.
  static constexpr unsigned PRB_SIZE = SAMPLES_OFFSET + 48;

  // Takes the 16bit elements 0, 4, 1, 5, 2, 6, 3, 7 of each lane with their two bytes swapped:
  // input lane:  I0 I1 I2 I3 Q0 Q1 Q2 Q3 (little-endian)
  // output lane: I0 Q0 I1 Q1 I2 Q2 I3 Q3 (big-endian)
  const __m256i bswap_mask_epi8 =
      _mm256_setr_epi64x(0x0a0b020308090001, 0x0e0f06070c0d0405, 0x0a0b020308090001, 0x0e0f06070c0d0405);

  for (unsigned pair = 0; pair != nof_prb_pairs; ++pair) {
    prb_pair_samples samples = quantize_prb_pair(in, [&](__m256i bf16_epi16) {
      return _mm256_shuffle_epi8(quantize_bf16_even_odd(bf16_epi16, scale_ps, clamp_epi16), bswap_mask_epi8);
    });

    // Write the 48 bytes of each PRB, preceded by the compression parameter if present: the first PRB takes the 8 REs
    // of rb0 and the 4 REs of the lower lane of rb01, the second PRB the 4 REs of the upper lane of rb01 and the 8 REs
    // of rb1.
    uint8_t* prb0 = out;
    uint8_t* prb1 = prb0 + PRB_SIZE;
    if (HAS_COMP_PARAM) {
      prb0[0] = comp_param;
      prb1[0] = comp_param;
    }
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(prb0 + SAMPLES_OFFSET), samples.rb0_epi16);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0 + SAMPLES_OFFSET + 32),
                     _mm256_castsi256_si128(samples.rb01_epi16));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1 + SAMPLES_OFFSET),
                     _mm256_extracti128_si256(samples.rb01_epi16, 1));
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(prb1 + SAMPLES_OFFSET + 16), samples.rb1_epi16);

    in += 2 * NOF_SAMPLES_PER_PRB;
    out += 2 * PRB_SIZE;
  }
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
/// The samples are unpacked into 32bit integers with the register layout of \ref prb_unpack_layout, scaled in single
/// precision and rounded to brain float.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte.
/// \tparam NOF_PRBS       Number of PRBs of the block, one or two.
/// \param[out] out        Output samples of the PRBs.
/// \param[in] in          Packed bytes of the PRBs.
/// \param[in] scale_0_ps  AVX2 register storing the scaling factor of the first PRB in every element.
/// \param[in] scale_1_ps  AVX2 register storing the scaling factor of the second PRB in every element.
template <unsigned BITS, bool HAS_COMP_PARAM, unsigned NOF_PRBS>
inline void decompress_prb_block(cbf16_t* out, const uint8_t* in, __m256 scale_0_ps, __m256 scale_1_ps)
{
  using layout = prb_unpack_layout<BITS, HAS_COMP_PARAM, NOF_PRBS>;
  // Number of complex samples of each PRB held by the first register pairs.
  static constexpr unsigned HEAD_NOF_RE = layout::NOF_HEAD_SAMPLES / 2;

  // Unpacks the samples of the given register pair and converts them to brain float.
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
  _mm_storeu_si128(reinterpret_cast<__m128i*>(out + HEAD_NOF_RE), _mm256_castsi256_si128(tail_epi16));
  if constexpr (NOF_PRBS == 2) {
    _mm_storeu_si128(reinterpret_cast<__m128i*>(out + NOF_SUBCARRIERS_PER_RB + HEAD_NOF_RE),
                     _mm256_extracti128_si256(tail_epi16, 1));
  }
}

/// \brief Decompresses the given PRBs, two at a time.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte holding its BFP exponent.
/// \param[out] out        Output samples of the PRBs.
/// \param[in] in          Packed bytes of the PRBs.
/// \param[in] nof_prbs    Number of PRBs.
/// \param[in] scale       Scaling factor of the samples, before the BFP exponent.
template <unsigned BITS, bool HAS_COMP_PARAM>
OCUDU_FLATTEN inline void decompress_prbs(cbf16_t* out, const uint8_t* in, unsigned nof_prbs, float scale)
{
  using layout                       = prb_unpack_layout<BITS, HAS_COMP_PARAM, 1>;
  static constexpr unsigned PRB_SIZE = layout::PRB_SIZE;

  // Scaling of the unpacked samples. The exponent of each PRB is added to the exponent field of the float, which is
  // the same as multiplying by two to the power of the exponent.
  const __m256i scaling_epi32 = _mm256_castps_si256(_mm256_set1_ps(scale * layout::SAMPLE_SCALE));
  auto          get_scale     = [&](const uint8_t* prb) {
    if constexpr (HAS_COMP_PARAM) {
      // The exponent takes the lower four bits of the compression parameter, as in decode_bfp_exponent().
      int32_t exponent = prb[0] & 0x0f;
      return _mm256_castsi256_ps(_mm256_add_epi32(scaling_epi32, _mm256_set1_epi32(exponent << 23)));
    }
    return _mm256_castsi256_ps(scaling_epi32);
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
