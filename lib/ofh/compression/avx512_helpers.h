// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief AVX512 support for OFH Compression algorithms.
///
/// The compressors and decompressors process four PRBs at a time (a quad), as their 96 samples fill exactly three
/// AVX512 registers of 32 16bit samples. Each 128bit lane holds four REs of a single PRB, so the registers hold the
/// following PRBs:
/// |                 | lane 0 | lane 1 | lane 2 | lane 3 |
/// | --------------- | ------ | ------ | ------ | ------ |
/// | first register  | RB0    | RB0    | RB0    | RB1    |
/// | second register | RB1    | RB1    | RB2    | RB2    |
/// | third register  | RB2    | RB3    | RB3    | RB3    |
/// An incomplete quad at the end of the input is processed with masked loads and stores.
///
/// The compression of a quad goes through the following steps:
/// 1. the brain floats are scaled and converted to 16bit integers in registers, see \ref quantize_bf16_even_odd;
/// 2. with BFP, the exponent of each PRB is found from the largest magnitude of its samples, see
///    \ref calculate_exponents, and the samples are shifted right by it;
/// 3. the samples are packed in big-endian into the output bytes, see packing_utils_avx512.h.
/// The decompression unpacks the samples into 32bit integers, scales them in single precision and rounds them to brain
/// float in registers, see \ref unpack_prb_quad and \ref convert_prb_quad.

#pragma once

#include "packing_utils_avx512.h"
#include "simd_loop_utils.h"
#include "ocudu/adt/bf16.h"
#include "ocudu/adt/complex.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/support/compiler.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <immintrin.h>
#include <type_traits>

namespace ocudu {
namespace ofh {
namespace mm512 {

/// Number of 16bit samples per PRB.
constexpr unsigned NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;
/// Number of 16bit samples per AVX512 register.
constexpr unsigned NOF_SAMPLES_PER_REG = 32;
/// Number of PRBs of a quad.
constexpr unsigned NOF_PRBS_PER_QUAD = 4;
/// Number of 16bit samples per quad of PRBs, which fill three AVX512 registers.
constexpr unsigned NOF_SAMPLES_PER_QUAD = NOF_PRBS_PER_QUAD * NOF_SAMPLES_PER_PRB;
/// Number of PRBs of a complete quad, as a compile time constant.
constexpr std::integral_constant<unsigned, NOF_PRBS_PER_QUAD> COMPLETE_QUAD = {};

/// \brief Quantizes 32 brain float values to 16bit integers, grouping the even and odd elements.
///
/// The values are scaled, rounded to the nearest integer (ties to even) and saturated to the 16bit range, like
/// \c quantizer::to_fixed_point. Each 128bit lane of the result holds the four even elements of the respective input
/// lane followed by the four odd ones, that is the I samples followed by the Q samples:
/// |                 | 128bit lane 0                     | 128bit lane 1                     | ...
/// | --------------- | --------------------------------- | --------------------------------- | ---
/// | \c bf16_epi16:  | I0 Q0 I1 Q1 I2 Q2 I3 Q3           | I4 Q4 I5 Q5 I6 Q6 I7 Q7           | ...
/// | return value:   | I0 I1 I2 I3 Q0 Q1 Q2 Q3           | I4 I5 I6 I7 Q4 Q5 Q6 Q7           | ...
/// The callers fold the reordering of the samples into their own shuffles.
///
/// \note The conversion to integers rounds according to the MXCSR register, it assumes the default rounding mode.
///
/// \param[in] bf16_epi16  AVX512 register storing 32 brain floats.
/// \param[in] scale_ps    AVX512 register storing the scaling factor applied before the conversion in every element.
/// \param[in] clamp_epi16 AVX512 register storing \ref get_bf16_saturation_clamp of the scale in every element.
/// \return An AVX512 register storing the quantized values.
inline __m512i quantize_bf16_even_odd(__m512i bf16_epi16, __m512 scale_ps, __m512i clamp_epi16)
{
  // Clamp the large positive values, so that the conversion to integers below does not wrap around.
  bf16_epi16 = _mm512_min_epi16(bf16_epi16, clamp_epi16);

  // Widen the brain floats to floats by placing them in the upper half of 32bit elements: the even elements are
  // shifted up and the odd ones are already there, their lower half is masked out.
  __m512 even_ps = _mm512_castsi512_ps(_mm512_slli_epi32(bf16_epi16, 16));
  __m512 odd_ps  = _mm512_castsi512_ps(_mm512_and_si512(bf16_epi16, _mm512_set1_epi32(0xffff0000)));

  // Scale, convert to integers and saturate to 16 bits. The pack works inside each 128bit lane.
  return _mm512_packs_epi32(_mm512_cvtps_epi32(_mm512_mul_ps(even_ps, scale_ps)),
                            _mm512_cvtps_epi32(_mm512_mul_ps(odd_ps, scale_ps)));
}

/// \brief Calculates the BFP exponents of four resource blocks passed in three AVX512 registers.
///
/// The exponent of a RB is the right shift required for its samples to fit in \c data_width bits, as given by
/// \c iq_compression_bfp_impl::determine_exponent. It depends only on the position of the most significant bit of the
/// largest magnitude of the RB. The magnitude is the number of bits that a sample needs besides the sign: a positive
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
  // Reorganize the registers so that the n-th lane of each one holds samples of the n-th RB:
  // [ RB0 | RB1 | RB2 | RB3 ] three times.
  __m512i v0_epi16 =
      _mm512_mask_shuffle_i64x2(r0_epi16, 0b11111100, r1_epi16, r2_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(1, 0, 0, 0));
  __m512i v1_epi16 =
      _mm512_mask_shuffle_i64x2(r1_epi16, 0b11000011, r0_epi16, r2_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(2, 0, 0, 1));
  __m512i v2_epi16 =
      _mm512_mask_shuffle_i64x2(r2_epi16, 0b00111111, r0_epi16, r1_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(0, 3, 3, 2));

  // The magnitude of a sample x is max(x, ~x), where ~x = -x - 1:
  //  - for x >= 0, ~x is negative, so the maximum is x itself;
  //  - for x < 0, ~x = |x| - 1 is non-negative and larger than x, following the formula in WG4 Annex A1.2.
  // The maximum magnitude of a lane is therefore the maximum of its largest sample and the bitwise inverse of its
  // smallest one.
  __m512i max_epi16 = _mm512_max_epi16(_mm512_max_epi16(v0_epi16, v1_epi16), v2_epi16);
  __m512i min_epi16 = _mm512_min_epi16(_mm512_min_epi16(v0_epi16, v1_epi16), v2_epi16);
  __m512i mag_epi16 = _mm512_max_epi16(max_epi16, _mm512_xor_si512(min_epi16, _mm512_set1_epi32(-1)));

  // The bitwise OR of the magnitudes of a lane has the same most significant bit as their maximum and is cheaper to
  // reduce. The result is replicated in all the elements of the lane.
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

/// Quantized samples of a quad of PRBs, with the register layout described in the file brief.
struct prb_quad_samples {
  __m512i r0_epi16;
  __m512i r1_epi16;
  __m512i r2_epi16;
};

/// Quantized samples of a quad of PRBs and their BFP exponents, see \ref calculate_exponents.
struct bfp_prb_quad {
  prb_quad_samples samples;
  __m512i          exponents_epi16;
};

/// \brief Loads the brain floats of up to four PRBs and quantizes them.
///
/// \tparam Quantizer   Type of the quantizer, callable with the AVX512 register of 32 brain floats to quantize.
/// \param[in] in       Input samples of the PRBs.
/// \param[in] nof_prbs Number of PRBs of the quad, up to four. The samples beyond them are not read and quantize as
///                     zeros.
/// \param[in] quantize Quantizer, see \ref quantize_bf16_even_odd.
/// \return The quantized samples.
template <typename Quantizer>
inline prb_quad_samples quantize_prb_quad(const bf16_t* in, unsigned nof_prbs, Quantizer&& quantize)
{
  // Loads the samples of the given register, masking out the ones of the missing PRBs if the quad is not complete.
  auto load = [&](unsigned reg) {
    const void* address = in + reg * NOF_SAMPLES_PER_REG;
    if (nof_prbs == NOF_PRBS_PER_QUAD) {
      return _mm512_loadu_si512(address);
    }
    // Number of samples of the quad from the start of this register on: more than a register for the registers that
    // are fully used, negative for the ones beyond the last PRB.
    int nof_samples = static_cast<int>(nof_prbs * NOF_SAMPLES_PER_PRB) - static_cast<int>(reg * NOF_SAMPLES_PER_REG);
    // Mask of the elements of this register that belong to the quad.
    __mmask32 mask =
        (nof_samples > 0) ? (~uint32_t{0} >> std::max(0, static_cast<int>(NOF_SAMPLES_PER_REG) - nof_samples)) : 0;
    return _mm512_maskz_loadu_epi16(mask, address);
  };
  return {quantize(load(0)), quantize(load(1)), quantize(load(2))};
}

/// \brief Packs the quantized samples of up to four PRBs.
///
/// \tparam BITS                Bit width of the packed samples, 9, 14 or 16.
/// \tparam HAS_COMP_PARAM      Whether each PRB starts with a compression parameter byte.
/// \param[out] out             Output bytes of the quad of PRBs.
/// \param[in] samples          Quantized samples of the quad, already shifted by the BFP exponents if any.
/// \param[in] comp_param_epi16 AVX512 register whose 128bit lanes hold the compression parameter of the respective
///                             PRB in their first byte. Ignored if \c HAS_COMP_PARAM is false.
/// \param[in] nof_prbs         Number of PRBs of the quad to write, from 1 to 4.
template <unsigned BITS, bool HAS_COMP_PARAM>
inline void pack_prb_quad(uint8_t* out, const prb_quad_samples& samples, __m512i comp_param_epi16, unsigned nof_prbs)
{
  if constexpr (BITS == 16) {
    pack_prb_quad_16b_big_endian<HAS_COMP_PARAM>(
        out, samples.r0_epi16, samples.r1_epi16, samples.r2_epi16, comp_param_epi16, nof_prbs);
  } else {
    pack_prb_quad_big_endian<BITS, HAS_COMP_PARAM>(
        out, samples.r0_epi16, samples.r1_epi16, samples.r2_epi16, comp_param_epi16, nof_prbs);
  }
}

/// \brief Compresses the given PRBs without BFP exponent, four at a time.
///
/// The loop is software pipelined, see \ref run_pipelined_loop: a quad is quantized while the previous ones are
/// packed.
///
/// \tparam BITS           Bit width of the packed samples.
/// \param[out] out        Output bytes of the PRBs.
/// \param[in] in          Input samples of the PRBs.
/// \param[in] nof_prbs    Number of PRBs.
/// \param[in] scale_ps    AVX512 register storing the scaling factor applied before the conversion in every element.
/// \param[in] clamp_epi16 AVX512 register storing \ref get_bf16_saturation_clamp of the scale in every element.
/// \param[in] full_scale  Largest quantized value, the samples saturate at it and at its negation.
template <unsigned BITS>
OCUDU_FLATTEN inline void compress_prb_quads(uint8_t*      out,
                                             const bf16_t* in,
                                             unsigned      nof_prbs,
                                             __m512        scale_ps,
                                             __m512i       clamp_epi16,
                                             float         full_scale)
{
  // Size in bytes of a packed PRB.
  static constexpr unsigned PRB_SIZE = 3 * BITS;

  // The quantized samples saturate at the symmetric full scale, except the 16bit ones, which use the whole range.
  const __m512i max_epi16 = _mm512_set1_epi16(static_cast<int16_t>(full_scale));
  const __m512i min_epi16 = _mm512_set1_epi16(static_cast<int16_t>(-full_scale));
  auto          quantize  = [&](__m512i bf16_epi16) {
    __m512i samples_epi16 = quantize_bf16_even_odd(bf16_epi16, scale_ps, clamp_epi16);
    if constexpr (BITS != 16) {
      samples_epi16 = _mm512_min_epi16(_mm512_max_epi16(samples_epi16, min_epi16), max_epi16);
    }
    return samples_epi16;
  };

  // The number of PRBs of the quad is a compile time constant for the complete quads, so that the load and store
  // masks fold away.
  auto quantize_quad = [&](unsigned quad, auto quad_nof_prbs) {
    return quantize_prb_quad(in + quad * NOF_SAMPLES_PER_QUAD, quad_nof_prbs, quantize);
  };
  auto pack_quad = [&](const prb_quad_samples& samples, unsigned quad, auto quad_nof_prbs) {
    pack_prb_quad<BITS, false>(
        out + quad * NOF_PRBS_PER_QUAD * PRB_SIZE, samples, _mm512_setzero_si512(), quad_nof_prbs);
  };

  // Complete quads, followed by the incomplete one at the end if any.
  unsigned nof_quads = nof_prbs / NOF_PRBS_PER_QUAD;
  run_pipelined_loop(
      nof_quads,
      [&](unsigned quad) { return quantize_quad(quad, COMPLETE_QUAD); },
      [&](const prb_quad_samples& samples, unsigned quad) { pack_quad(samples, quad, COMPLETE_QUAD); });
  if (unsigned remainder = nof_prbs % NOF_PRBS_PER_QUAD; remainder != 0) {
    pack_quad(quantize_quad(nof_quads, remainder), nof_quads, remainder);
  }
}

/// \brief Compresses the given PRBs with BFP, four at a time.
///
/// The loop is software pipelined, see \ref run_pipelined_loop: a quad is quantized and its exponents found while the
/// previous ones are shifted and packed.
///
/// \tparam BITS               Bit width of the packed samples.
/// \param[out] out            Output bytes of the PRBs.
/// \param[in] in              Input samples of the PRBs.
/// \param[in] nof_prbs        Number of PRBs.
/// \param[in] scale_ps        AVX512 register storing the scaling factor applied before the conversion in every
///                            element.
/// \param[in] clamp_epi16     AVX512 register storing \ref get_bf16_saturation_clamp of the scale in every element.
/// \param[in] exponent_offset Offset added to the exponent in the compression parameter of each PRB.
template <unsigned BITS>
OCUDU_FLATTEN inline void compress_prb_quads_bfp(uint8_t*      out,
                                                 const bf16_t* in,
                                                 unsigned      nof_prbs,
                                                 __m512        scale_ps,
                                                 __m512i       clamp_epi16,
                                                 uint8_t       exponent_offset)
{
  // Size in bytes of a packed PRB, including the compression parameter.
  static constexpr unsigned PRB_SIZE = 3U * BITS + 1U;
  // Samples that use all the bits of the data width are not shifted, so the exponent is always zero.
  static constexpr bool HAS_EXPONENT = (BITS != 16);

  const __m512i exponent_offset_epi16 = _mm512_set1_epi16(exponent_offset);

  // The number of PRBs of the quad is a compile time constant for the complete quads, so that the load and store
  // masks fold away.
  auto quantize_quad = [&](unsigned quad, auto quad_nof_prbs) {
    bfp_prb_quad state;
    state.samples         = quantize_prb_quad(in + quad * NOF_SAMPLES_PER_QUAD, quad_nof_prbs, [&](__m512i bf16_epi16) {
      return quantize_bf16_even_odd(bf16_epi16, scale_ps, clamp_epi16);
    });
    state.exponents_epi16 = _mm512_setzero_si512();
    if constexpr (HAS_EXPONENT) {
      state.exponents_epi16 =
          calculate_exponents(state.samples.r0_epi16, state.samples.r1_epi16, state.samples.r2_epi16, BITS);
    }
    return state;
  };

  auto pack_quad = [&](const bfp_prb_quad& state, unsigned quad, auto quad_nof_prbs) {
    // Shift the samples of each RB right by its exponent. The shift counts of a register take the exponent of the RB
    // held by each of its lanes (see the file brief).
    prb_quad_samples shifted = state.samples;
    if constexpr (HAS_EXPONENT) {
      __m512i e_epi16  = state.exponents_epi16;
      shifted.r0_epi16 = _mm512_srav_epi16(
          state.samples.r0_epi16, _mm512_shuffle_i64x2(e_epi16, e_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(1, 0, 0, 0)));
      shifted.r1_epi16 = _mm512_srav_epi16(
          state.samples.r1_epi16, _mm512_shuffle_i64x2(e_epi16, e_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(2, 2, 1, 1)));
      shifted.r2_epi16 = _mm512_srav_epi16(
          state.samples.r2_epi16, _mm512_shuffle_i64x2(e_epi16, e_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(3, 3, 3, 2)));
    }

    // Pack the samples together with the compression parameters.
    __m512i comp_param_epi16 = _mm512_add_epi16(state.exponents_epi16, exponent_offset_epi16);
    pack_prb_quad<BITS, true>(out + quad * NOF_PRBS_PER_QUAD * PRB_SIZE, shifted, comp_param_epi16, quad_nof_prbs);
  };

  // Complete quads, followed by the incomplete one at the end if any.
  unsigned nof_quads = nof_prbs / NOF_PRBS_PER_QUAD;
  run_pipelined_loop(
      nof_quads,
      [&](unsigned quad) { return quantize_quad(quad, COMPLETE_QUAD); },
      [&](const bfp_prb_quad& state, unsigned quad) { pack_quad(state, quad, COMPLETE_QUAD); });
  if (unsigned remainder = nof_prbs % NOF_PRBS_PER_QUAD; remainder != 0) {
    pack_quad(quantize_quad(nof_quads, remainder), nof_quads, remainder);
  }
}

/// \brief Rounds 16 floats to brain float, nearest with ties to even.
///
/// \param[in] value_ps AVX512 register storing 16 floats.
/// \return An AVX512 register whose 32bit elements hold the brain float in their upper 16 bits.
inline __m512i round_to_bf16_epi32(__m512 value_ps)
{
  // Same as to_bf16(): add 0x7fff plus the least significant bit of the result, so that ties round to even.
  __m512i value_epi32 = _mm512_castps_si512(value_ps);
  __m512i lsb_epi32   = _mm512_and_si512(_mm512_srli_epi32(value_epi32, 16), _mm512_set1_epi32(1));
  return _mm512_add_epi32(_mm512_add_epi32(value_epi32, _mm512_set1_epi32(0x7fff)), lsb_epi32);
}

/// \brief Converts the floats of two AVX512 registers to brain float, nearest with ties to even.
///
/// It gives the same values as \ref round_to_bf16_epi32 with a single AVX512_BF16 instruction, so it is only defined in
/// the translation units compiled with the AVX512_BF16 instruction set (see \ref decompress_prb_quads_bf16). The caller
/// must check the CPU support at run time.
///
/// \param[in] high_ps AVX512 register storing the 16 floats converted to the upper 16 elements of the result.
/// \param[in] low_ps  AVX512 register storing the 16 floats converted to the lower 16 elements of the result.
/// \return An AVX512 register storing the 32 brain floats.
inline __m512i convert_to_bf16_epi16(__m512 high_ps, __m512 low_ps);

#ifdef __AVX512BF16__
inline __m512i convert_to_bf16_epi16(__m512 high_ps, __m512 low_ps)
{
  // The vector cast reinterprets the brain float elements as 16bit integers.
  return (__m512i)_mm512_cvtne2ps_pbh(high_ps, low_ps);
}
#endif // __AVX512BF16__

/// Makes the 16bit permutation interleaving the lower and upper halves of a register: the element 2k takes the element
/// k and the element 2k + 1 the element 16 + k.
constexpr std::array<int16_t, 32> make_interleave_halves_permutation()
{
  std::array<int16_t, 32> permutation = {};
  for (unsigned k = 0; k != 16; ++k) {
    permutation[2 * k]     = static_cast<int16_t>(k);
    permutation[2 * k + 1] = static_cast<int16_t>(16 + k);
  }
  return permutation;
}

/// \brief Converts two AVX512 registers of 16 floats to interleaved brain floats.
///
/// \tparam USE_BF16   Whether to round with the AVX512_BF16 instruction set.
/// \param[in] even_ps AVX512 register storing the even samples.
/// \param[in] odd_ps  AVX512 register storing the odd samples.
/// \return An AVX512 register storing the 32 brain floats, interleaving the even and odd samples.
template <bool USE_BF16>
inline __m512i convert_to_interleaved_bf16(__m512 even_ps, __m512 odd_ps)
{
  if constexpr (USE_BF16) {
    // The instruction converts the even samples to the lower half of the register and the odd ones to the upper half,
    // a 16bit permutation then interleaves them.
    static constexpr std::array<int16_t, 32> INTERLEAVE = make_interleave_halves_permutation();
    return _mm512_permutexvar_epi16(_mm512_loadu_si512(INTERLEAVE.data()), convert_to_bf16_epi16(odd_ps, even_ps));
  } else {
    // The brain floats are in the upper half of the 32bit elements: shift the even ones down and blend the odd ones.
    return _mm512_mask_blend_epi16(
        0xaaaaaaaa, _mm512_srli_epi32(round_to_bf16_epi32(even_ps), 16), round_to_bf16_epi32(odd_ps));
  }
}

/// Unpacked quad of PRBs: the even and odd samples of each register pair (see \ref prb_quad_unpack_layout) and the
/// scaling factor of their elements, as the bits of a float.
struct unpacked_prb_quad {
  __m512i even_epi32[3];
  __m512i odd_epi32[3];
  __m512i scale_epi32[3];
};

/// \brief Unpacks a quad of PRBs into 32bit integers.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte holding its BFP exponent.
/// \param[in] in          Packed bytes of the PRBs.
/// \param[in] nof_prbs    Number of PRBs of the quad, up to four. The bytes beyond them are not read.
/// \param[in] scale_epi32 Bits of the float scaling factor of the unpacked samples, before the BFP exponent.
/// \return The unpacked quad.
template <unsigned BITS, bool HAS_COMP_PARAM>
inline unpacked_prb_quad unpack_prb_quad(const uint8_t* in, unsigned nof_prbs, __m512i scale_epi32)
{
  using layout = prb_quad_unpack_layout<BITS, HAS_COMP_PARAM>;

  prb_quad_sources<layout> sources;
  load_prb_quad<layout>(sources, in, nof_prbs);

  // Scaling factor of each PRB in the first four elements. The exponent of each PRB is added to the exponent field of
  // the float, which is the same as multiplying by two to the power of the exponent.
  __m512 prb_scales_ps = _mm512_castsi512_ps(scale_epi32);
  if constexpr (HAS_COMP_PARAM) {
    // The exponent takes the lower four bits of the compression parameter, as in decode_bfp_exponent().
    __m512i exponents_epi32 = _mm512_and_si512(gather_prb_quad_comp_params<layout>(sources), _mm512_set1_epi32(0x0f));
    prb_scales_ps = _mm512_castsi512_ps(_mm512_add_epi32(scale_epi32, _mm512_slli_epi32(exponents_epi32, 23)));
  }

  // Unpack each register pair and spread the scaling factors of its PRBs over its elements.
  unpacked_prb_quad quad;
  for_each_index<layout::NOF_PAIRS>([&](auto pair) {
    static constexpr unsigned PAIR        = decltype(pair)::value;
    static constexpr auto     PRB_INDICES = layout::make_prb_indices(PAIR);
    unpack_prb_quad_pair<layout, PAIR>(quad.even_epi32[PAIR], quad.odd_epi32[PAIR], sources);
    __m512 pair_scale_ps =
        HAS_COMP_PARAM ? _mm512_permutexvar_ps(_mm512_loadu_si512(PRB_INDICES.data()), prb_scales_ps) : prb_scales_ps;
    quad.scale_epi32[PAIR] = _mm512_castps_si512(pair_scale_ps);
  });
  return quad;
}

/// \brief Converts an unpacked quad of PRBs to brain float.
///
/// \tparam USE_BF16    Whether to round with the AVX512_BF16 instruction set.
/// \param[out] out     Output samples of the PRBs.
/// \param[in] quad     Unpacked quad.
/// \param[in] nof_prbs Number of PRBs of the quad, up to four. The samples beyond them are not written.
template <bool USE_BF16>
inline void convert_prb_quad(cbf16_t* out, const unpacked_prb_quad& quad, unsigned nof_prbs)
{
  // Number of 16bit samples of a register pair.
  static constexpr unsigned NOF_SAMPLES_PER_PAIR = 32;

  for_each_index<3>([&](auto pair) {
    static constexpr unsigned PAIR = decltype(pair)::value;

    __m512  scale_ps      = _mm512_castsi512_ps(quad.scale_epi32[PAIR]);
    __m512  even_ps       = _mm512_mul_ps(_mm512_cvtepi32_ps(quad.even_epi32[PAIR]), scale_ps);
    __m512  odd_ps        = _mm512_mul_ps(_mm512_cvtepi32_ps(quad.odd_epi32[PAIR]), scale_ps);
    __m512i samples_epi16 = convert_to_interleaved_bf16<USE_BF16>(even_ps, odd_ps);

    // Store the samples up to the number of PRBs.
    cbf16_t* address = out + PAIR * NOF_SAMPLES_PER_PAIR / 2;
    if (nof_prbs == NOF_PRBS_PER_QUAD) {
      _mm512_storeu_si512(address, samples_epi16);
      return;
    }
    int nof_samples = static_cast<int>(nof_prbs * NOF_SAMPLES_PER_PRB) - static_cast<int>(PAIR * NOF_SAMPLES_PER_PAIR);
    __mmask32 mask  = (nof_samples > 0) ? (~uint32_t{0} >> std::max(0, 32 - nof_samples)) : 0;
    _mm512_mask_storeu_epi16(address, mask, samples_epi16);
  });
}

/// \brief Decompresses the given PRBs, four at a time.
///
/// The loop is software pipelined, see \ref run_pipelined_loop: a quad is unpacked while the previous ones are
/// converted to brain float.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte holding its BFP exponent.
/// \tparam USE_BF16       Whether to round with the AVX512_BF16 instruction set.
/// \param[out] out        Output samples of the PRBs.
/// \param[in] in          Packed bytes of the PRBs.
/// \param[in] nof_prbs    Number of PRBs.
/// \param[in] scale       Scaling factor of the samples, before the BFP exponent.
template <unsigned BITS, bool HAS_COMP_PARAM, bool USE_BF16>
OCUDU_FLATTEN inline void decompress_prb_quads(cbf16_t* out, const uint8_t* in, unsigned nof_prbs, float scale)
{
  using layout = prb_quad_unpack_layout<BITS, HAS_COMP_PARAM>;

  const __m512i scale_epi32 = _mm512_castps_si512(_mm512_set1_ps(scale * layout::SAMPLE_SCALE));

  // The number of PRBs of the quad is a compile time constant for the complete quads, so that the load and store
  // masks fold away.
  auto unpack_quad = [&](unsigned quad, auto quad_nof_prbs) {
    return unpack_prb_quad<BITS, HAS_COMP_PARAM>(in + quad * layout::QUAD_SIZE, quad_nof_prbs, scale_epi32);
  };
  auto convert_quad = [&](const unpacked_prb_quad& quad_state, unsigned quad, auto quad_nof_prbs) {
    convert_prb_quad<USE_BF16>(out + quad * NOF_PRBS_PER_QUAD * NOF_SUBCARRIERS_PER_RB, quad_state, quad_nof_prbs);
  };

  // Complete quads, followed by the incomplete one at the end if any.
  unsigned nof_quads = nof_prbs / NOF_PRBS_PER_QUAD;
  run_pipelined_loop(
      nof_quads,
      [&](unsigned quad) { return unpack_quad(quad, COMPLETE_QUAD); },
      [&](const unpacked_prb_quad& quad_state, unsigned quad) { convert_quad(quad_state, quad, COMPLETE_QUAD); });
  if (unsigned remainder = nof_prbs % NOF_PRBS_PER_QUAD; remainder != 0) {
    convert_quad(unpack_quad(nof_quads, remainder), nof_quads, remainder);
  }
}

/// \brief Decompresses the given PRBs rounding with the AVX512_BF16 instruction set.
///
/// It is \ref decompress_prb_quads with the AVX512_BF16 rounding, defined in the only translation unit compiled with
/// the instruction set so that the rest of the AVX512 code stays free of it. The caller must check the CPU support at
/// run time.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte holding its BFP exponent.
/// \param[out] out        Output samples of the PRBs.
/// \param[in] in          Packed bytes of the PRBs.
/// \param[in] nof_prbs    Number of PRBs.
/// \param[in] scale       Scaling factor of the samples, before the BFP exponent.
template <unsigned BITS, bool HAS_COMP_PARAM>
void decompress_prb_quads_bf16(cbf16_t* out, const uint8_t* in, unsigned nof_prbs, float scale);

} // namespace mm512
} // namespace ofh
} // namespace ocudu
