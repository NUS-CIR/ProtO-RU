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

#include "packing_utils_avx512.h"
#include "ocudu/adt/complex.h"
#include <cstdint>
#include <immintrin.h>
#include <type_traits>

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
/// \param[in] clamp_epi16 AVX512 register storing \ref get_bf16_saturation_clamp of the scale in every element.
inline __m512i quantize_bf16_even_odd(__m512i bf16_epi16, __m512 scale_ps, __m512i clamp_epi16)
{
  // Clamp the large positive values, so that the conversion to integers below does not wrap around.
  bf16_epi16 = _mm512_min_epi16(bf16_epi16, clamp_epi16);

  // Unpack brain floats into two vectors of floats: even elements and odd ones.
  __m512 even_ps = _mm512_castsi512_ps(_mm512_slli_epi32(bf16_epi16, 16));
  __m512 odd_ps  = _mm512_castsi512_ps(_mm512_and_si512(bf16_epi16, _mm512_set1_epi32(0xffff0000)));

  // Convert to integers and saturate to 16 bits, the pack works inside each 128bit lane.
  return _mm512_packs_epi32(_mm512_cvtps_epi32(_mm512_mul_ps(even_ps, scale_ps)),
                            _mm512_cvtps_epi32(_mm512_mul_ps(odd_ps, scale_ps)));
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
/// It gives the same values as \ref round_to_bf16_epi32 with a single AVX512_BF16 instruction. The instruction is
/// emitted as inline assembly because the file is compiled for CPUs without AVX512_BF16, the caller must check the
/// CPU support at run time.
///
/// \param[in] high_ps AVX512 register storing the 16 floats converted to the upper 16 elements of the result.
/// \param[in] low_ps  AVX512 register storing the 16 floats converted to the lower 16 elements of the result.
/// \return An AVX512 register storing the 32 brain floats.
inline __m512i convert_to_bf16_epi16(__m512 high_ps, __m512 low_ps)
{
  __m512i result_epi16;
  asm("vcvtne2ps2bf16 %2, %1, %0" : "=v"(result_epi16) : "v"(high_ps), "v"(low_ps));
  return result_epi16;
}

/// \brief Decompressor of quads of PRBs.
///
/// The samples are unpacked into 32bit integers, scaled in single precision and rounded to brain float, see
/// \ref prb_quad_unpack_layout for the register layout. The work is split in two stages, so that the caller can
/// unpack a quad while the previous one is converted.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte holding its BFP exponent.
/// \tparam USE_BF16       Whether to round with the AVX512_BF16 instruction set.
template <unsigned BITS, bool HAS_COMP_PARAM, bool USE_BF16>
class prb_quad_decompressor
{
  using layout = prb_quad_unpack_layout<BITS, HAS_COMP_PARAM>;

public:
  /// Unpacked quad: the even and odd samples of each register pair and the scaling factors of their elements, as
  /// floats.
  struct quad_state {
    std::array<avx512_register, layout::NOF_PAIRS> even_epi32;
    std::array<avx512_register, layout::NOF_PAIRS> odd_epi32;
    std::array<avx512_register, layout::NOF_PAIRS> scale_epi32;
  };

  /// \brief Creates a decompressor with the given scaling factor of the samples before the BFP exponent.
  explicit prb_quad_decompressor(float scale) :
    scale_epi32(_mm512_castps_si512(_mm512_set1_ps(scale * layout::SAMPLE_SCALE)))
  {
  }

  /// \brief Unpacks a quad of PRBs.
  ///
  /// \param[in] in       Packed bytes of the PRBs.
  /// \param[in] nof_prbs Number of PRBs of the quad, up to four. The bytes beyond them are not read.
  /// \return The unpacked quad.
  quad_state unpack(const uint8_t* in, unsigned nof_prbs) const
  {
    prb_quad_sources<layout> sources;
    load_prb_quad<layout>(sources, in, nof_prbs);

    // Scaling factor of each PRB: the exponent is added to the exponent field of the float.
    __m512 prb_scales_ps = _mm512_castsi512_ps(scale_epi32);
    if constexpr (HAS_COMP_PARAM) {
      // The exponent takes the lower four bits of the compression parameter, as in decode_bfp_exponent().
      __m512i exponents_epi32 = _mm512_and_si512(gather_prb_quad_comp_params<layout>(sources), _mm512_set1_epi32(0x0f));
      prb_scales_ps = _mm512_castsi512_ps(_mm512_add_epi32(scale_epi32, _mm512_slli_epi32(exponents_epi32, 23)));
    }

    quad_state state;
    auto       unpack_pair = [&](auto pair) {
      static constexpr unsigned PAIR = decltype(pair)::value;
      static constexpr auto     PRBS = layout::make_prb_indices(PAIR);
      unpack_prb_quad_pair<layout, PAIR>(state.even_epi32[PAIR].value, state.odd_epi32[PAIR].value, sources);
      __m512 pair_scale_ps =
          HAS_COMP_PARAM ? _mm512_permutexvar_ps(_mm512_loadu_si512(PRBS.data()), prb_scales_ps) : prb_scales_ps;
      state.scale_epi32[PAIR].value = _mm512_castps_si512(pair_scale_ps);
    };
    unpack_pair(std::integral_constant<unsigned, 0>());
    unpack_pair(std::integral_constant<unsigned, 1>());
    unpack_pair(std::integral_constant<unsigned, 2>());
    return state;
  }

  /// \brief Converts an unpacked quad of PRBs to brain float.
  ///
  /// \param[out] out      Output samples of the PRBs.
  /// \param[in] state     Unpacked quad.
  /// \param[in] nof_prbs  Number of PRBs of the quad, up to four. The samples beyond them are not written.
  static void convert(cbf16_t* out, const quad_state& state, unsigned nof_prbs)
  {
    auto convert_pair = [&](auto pair) {
      static constexpr unsigned PAIR     = decltype(pair)::value;
      __m512                    scale_ps = _mm512_castsi512_ps(state.scale_epi32[PAIR].value);
      __m512                    even_ps  = _mm512_mul_ps(_mm512_cvtepi32_ps(state.even_epi32[PAIR].value), scale_ps);
      __m512                    odd_ps   = _mm512_mul_ps(_mm512_cvtepi32_ps(state.odd_epi32[PAIR].value), scale_ps);

      __m512i samples_epi16;
      if constexpr (USE_BF16) {
        // Interleave the even and odd samples after the conversion.
        const __m512i interleave_epi16 = _mm512_set_epi16(31,
                                                          15,
                                                          30,
                                                          14,
                                                          29,
                                                          13,
                                                          28,
                                                          12,
                                                          27,
                                                          11,
                                                          26,
                                                          10,
                                                          25,
                                                          9,
                                                          24,
                                                          8,
                                                          23,
                                                          7,
                                                          22,
                                                          6,
                                                          21,
                                                          5,
                                                          20,
                                                          4,
                                                          19,
                                                          3,
                                                          18,
                                                          2,
                                                          17,
                                                          1,
                                                          16,
                                                          0);
        samples_epi16 = _mm512_permutexvar_epi16(interleave_epi16, convert_to_bf16_epi16(odd_ps, even_ps));
      } else {
        samples_epi16 = _mm512_mask_blend_epi16(
            0xaaaaaaaa, _mm512_srli_epi32(round_to_bf16_epi32(even_ps), 16), round_to_bf16_epi32(odd_ps));
      }

      // Store the samples up to the number of PRBs.
      cbf16_t* address = out + PAIR * layout::NOF_SAMPLES_PER_PAIR / 2;
      if (nof_prbs == 4) {
        _mm512_storeu_si512(address, samples_epi16);
        return;
      }
      int nof_samples = static_cast<int>(nof_prbs * layout::NOF_SAMPLES_PER_PRB) -
                        static_cast<int>(PAIR * layout::NOF_SAMPLES_PER_PAIR);
      __mmask32 mask = (nof_samples > 0) ? (~uint32_t{0} >> std::max(0, 32 - nof_samples)) : 0;
      _mm512_mask_storeu_epi16(address, mask, samples_epi16);
    };
    convert_pair(std::integral_constant<unsigned, 0>());
    convert_pair(std::integral_constant<unsigned, 1>());
    convert_pair(std::integral_constant<unsigned, 2>());
  }

private:
  /// Scaling of the unpacked samples before the BFP exponent, as a float.
  __m512i scale_epi32;
};

/// \brief Decompresses the given PRBs, four at a time.
///
/// The loop is software pipelined: the next quad is unpacked before converting the current one, so that the CPU
/// overlaps the two chains.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte holding its BFP exponent.
/// \tparam USE_BF16       Whether to round with the AVX512_BF16 instruction set.
/// \param[out] out     Output samples of the PRBs.
/// \param[in] in       Packed bytes of the PRBs.
/// \param[in] nof_prbs Number of PRBs.
/// \param[in] scale    Scaling factor of the samples before the BFP exponent.
template <unsigned BITS, bool HAS_COMP_PARAM, bool USE_BF16>
inline void decompress_prb_quads(cbf16_t* out, const uint8_t* in, unsigned nof_prbs, float scale)
{
  using decompressor = prb_quad_decompressor<BITS, HAS_COMP_PARAM, USE_BF16>;
  using layout       = prb_quad_unpack_layout<BITS, HAS_COMP_PARAM>;
  // Number of complex samples of a quad.
  static constexpr unsigned QUAD_NOF_RE = 4 * NOF_SUBCARRIERS_PER_RB;

  const decompressor dec(scale);
  auto               quad_in  = [&](unsigned quad) { return in + quad * layout::QUAD_SIZE; };
  auto               quad_out = [&](unsigned quad) { return out + quad * QUAD_NOF_RE; };

  unsigned nof_quads = nof_prbs / 4;
  if (nof_quads != 0) {
    typename decompressor::quad_state current = dec.unpack(quad_in(0), 4);
    for (unsigned quad = 1; quad != nof_quads; ++quad) {
      typename decompressor::quad_state next = dec.unpack(quad_in(quad), 4);
      decompressor::convert(quad_out(quad - 1), current, 4);
      current = next;
    }
    decompressor::convert(quad_out(nof_quads - 1), current, 4);
  }
  if (nof_prbs % 4 != 0) {
    decompressor::convert(quad_out(nof_quads), dec.unpack(quad_in(nof_quads), nof_prbs % 4), nof_prbs % 4);
  }
}

} // namespace mm512
} // namespace ofh
} // namespace ocudu
