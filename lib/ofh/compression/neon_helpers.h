// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief NEON support for OFH Compression algorithms.
///
/// A PRB holds 24 samples, which fill three NEON registers of 8 16bit samples. The compressors quantize the brain
/// floats on the fly, find the BFP exponents of four PRBs at a time in vector lanes and pack each register with the
/// tables of \ref packed_register_layout.
///
/// The conversion of the scaled floats to integers goes through the mantissa of a float near \f$2^{23}\f$ rather than
/// the conversion instructions, which have a lower throughput than the arithmetic ones on many Arm cores: a float
/// rounds to the nearest integer (ties to even) by adding \f$1.5 \cdot 2^{23}\f$, and the result is held by the
/// mantissa bits.

#pragma once

#include "packing_utils_neon.h"
#include "ocudu/adt/bf16.h"
#include "ocudu/adt/complex.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/support/compiler.h"
#include <algorithm>
#include <arm_neon.h>
#include <cstdint>
#include <utility>

namespace ocudu {
namespace ofh {
namespace neon {

/// Number of 16bit samples per PRB.
static constexpr unsigned NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;
/// Number of NEON registers of samples per PRB.
static constexpr unsigned NOF_REGS_PER_PRB = NOF_SAMPLES_PER_PRB / 8;

/// Float that rounds the integers added to it to the nearest one (ties to even), as \f$1.5 \cdot 2^{23}\f$.
static constexpr float ROUNDING_BIAS = 0x1.8p23F;

/// Calls the given function with \c std::integral_constant arguments from 0 to \c N - 1.
template <unsigned N, typename Func, unsigned... Is>
OCUDU_FORCE_INLINE void for_each_index(Func&& func, std::integer_sequence<unsigned, Is...> /* unused */)
{
  (func(std::integral_constant<unsigned, Is>()), ...);
}

/// Calls the given function with \c std::integral_constant arguments from 0 to \c N - 1.
template <unsigned N, typename Func>
OCUDU_FORCE_INLINE void for_each_index(Func&& func)
{
  for_each_index<N>(std::forward<Func>(func), std::make_integer_sequence<unsigned, N>());
}

/// Pair of NEON registers storing 8 floats.
struct float32x8 {
  float32x4_t first;
  float32x4_t last;
};

/// \brief Quantizes registers of 8 brain float values to 16bit integers.
///
/// The values are scaled, clamped to the given range and rounded to the nearest integer (ties to even), in the same
/// way as \c quantizer::to_fixed_point. The registers are processed stage by stage, so that the dependency chains of
/// the registers interleave.
///
/// \tparam NOF_REGS Number of registers.
/// \param[out] out      Registers storing the quantized values in the order of the input.
/// \param[in] in        Input brain floats, 8 per register.
/// \param[in] scale_f32 NEON register storing the scaling factor applied before the conversion in every element.
/// \param[in] min_f32   NEON register storing the smallest quantized value in every element.
/// \param[in] max_f32   NEON register storing the largest quantized value in every element.
template <unsigned NOF_REGS>
OCUDU_FORCE_INLINE void
quantize_bf16(int16x8_t* out, const bf16_t* in, float32x4_t scale_f32, float32x4_t min_f32, float32x4_t max_f32)
{
  const uint16x8_t  zero_u16 = vdupq_n_u16(0);
  const float32x4_t bias_f32 = vdupq_n_f32(ROUNDING_BIAS);

  float32x8 values[NOF_REGS];

  // Widen the brain floats to floats by placing them above zeros.
  for_each_index<NOF_REGS>([&](auto reg) {
    uint16x8_t bf16_u16 = vld1q_u16(reinterpret_cast<const uint16_t*>(in) + reg * 8);
    values[reg].first   = vreinterpretq_f32_u16(vzip1q_u16(zero_u16, bf16_u16));
    values[reg].last    = vreinterpretq_f32_u16(vzip2q_u16(zero_u16, bf16_u16));
  });

  // Scale, clamp and round. The rounding bias makes the floating point addition round to the nearest integer, which
  // is then held by the low 16 bits of the mantissa in two's complement, as the clamps keep it within the 16bit range.
  for_each_index<NOF_REGS>([&](auto reg) {
    values[reg].first = vmulq_f32(values[reg].first, scale_f32);
    values[reg].last  = vmulq_f32(values[reg].last, scale_f32);
  });
  for_each_index<NOF_REGS>([&](auto reg) {
    values[reg].first = vminq_f32(values[reg].first, max_f32);
    values[reg].last  = vminq_f32(values[reg].last, max_f32);
  });
  for_each_index<NOF_REGS>([&](auto reg) {
    values[reg].first = vmaxq_f32(values[reg].first, min_f32);
    values[reg].last  = vmaxq_f32(values[reg].last, min_f32);
  });
  for_each_index<NOF_REGS>([&](auto reg) {
    values[reg].first = vaddq_f32(values[reg].first, bias_f32);
    values[reg].last  = vaddq_f32(values[reg].last, bias_f32);
  });
  for_each_index<NOF_REGS>([&](auto reg) {
    out[reg] = vreinterpretq_s16_u16(
        vuzp1q_u16(vreinterpretq_u16_f32(values[reg].first), vreinterpretq_u16_f32(values[reg].last)));
  });
}

/// \brief Calculates the BFP exponents of up to four PRBs.
///
/// The exponent of a PRB is the right shift required for its samples to fit in \c data_width bits, as given by
/// \c iq_compression_bfp_impl::determine_exponent. A sample needs as many bits besides the sign as the ones below its
/// leading sign bits: a positive \f$x\f$ needs as many bits as \f$x\f$, and a negative one as many as \f$|x|-1\f$ (for
/// example, -256 fits in 9 bits while 256 does not), see O-RAN.WG4.CUS Annex A.1.2. The PRB needs the maximum of them,
/// that is the samples share the minimum count of leading sign bits.
///
/// \tparam NOF_PRBS Number of PRBs, from one to four.
/// \param[in] samples    Registers storing the 16bit samples of the PRBs, three per PRB.
/// \param[in] data_width BFP compression bit width.
/// \return A NEON register whose lane \c i stores the exponent of the PRB \c i, for \c i below \c NOF_PRBS.
template <unsigned NOF_PRBS>
OCUDU_FORCE_INLINE int16x8_t calculate_exponents(const int16x8_t* samples, unsigned data_width)
{
  static_assert((NOF_PRBS >= 1) && (NOF_PRBS <= 4), "Unsupported number of PRBs");

  int16x8_t cls_s16[4];
  for_each_index<4>([&](auto prb) {
    static constexpr unsigned PRB = std::min<unsigned>(decltype(prb)::value, NOF_PRBS - 1);

    const int16x8_t* prb_samples = samples + PRB * NOF_REGS_PER_PRB;
    cls_s16[prb] =
        vminq_s16(vminq_s16(vclsq_s16(prb_samples[0]), vclsq_s16(prb_samples[1])), vclsq_s16(prb_samples[2]));
  });

  // Pairwise minimums reduce the 8 lanes of each PRB to one, in the order of the PRBs.
  int16x8_t cls_min_s16 = vpminq_s16(vpminq_s16(cls_s16[0], cls_s16[1]), vpminq_s16(cls_s16[2], cls_s16[3]));
  cls_min_s16           = vpminq_s16(cls_min_s16, cls_min_s16);

  // A sample with p leading sign bits needs 16 - p bits including the sign, so the exponent is
  // max(0, (16 - p) - data_width).
  uint16x8_t max_shift_u16 = vdupq_n_u16(static_cast<uint16_t>(16 - data_width));
  return vreinterpretq_s16_u16(vqsubq_u16(max_shift_u16, vreinterpretq_u16_s16(cls_min_s16)));
}

/// \brief Compresses a batch of up to four PRBs.
///
/// \tparam BITS     Bit width of the packed samples.
/// \tparam IS_BFP   Whether the PRBs start with the BFP compression parameter and are shifted by their exponent.
/// \tparam NOF_PRBS Number of PRBs of the batch, from one to four.
/// \param[out] out            Output bytes of the PRBs.
/// \param[in] in              Input samples of the PRBs.
/// \param[in] scale_f32       NEON register storing the scaling factor applied before the conversion in every element.
/// \param[in] exponent_offset Offset added to the BFP exponents.
template <unsigned BITS, bool IS_BFP, unsigned NOF_PRBS>
OCUDU_FORCE_INLINE void
compress_prb_batch(uint8_t* out, const bf16_t* in, float32x4_t scale_f32, uint8_t exponent_offset)
{
  static_assert((NOF_PRBS >= 1) && (NOF_PRBS <= 4), "Unsupported number of PRBs");
  // Samples that use all the bits of the data width are not shifted, so the exponent is always zero.
  static constexpr bool HAS_EXPONENT = IS_BFP && (BITS != 16);
  // Size in bytes of one compressed PRB.
  static constexpr unsigned PRB_SIZE = NOF_REGS_PER_PRB * BITS + (IS_BFP ? 1 : 0);
  // Offset of the samples inside a PRB.
  static constexpr unsigned SAMPLES_OFFSET = IS_BFP ? 1 : 0;
  // Quantization range: BFP quantizes to 16 bits, otherwise the samples saturate at the full scale of the bit width,
  // except that the negative 16bit values saturate at -32768 like quantizer::to_fixed_point.
  static constexpr unsigned QUANTIZATION_BITS = IS_BFP ? 16 : BITS;
  static constexpr float    MAX_VALUE         = (1 << (QUANTIZATION_BITS - 1)) - 1.0F;
  static constexpr float    MIN_VALUE         = (QUANTIZATION_BITS == 16) ? -32768.0F : -MAX_VALUE;
  // Number of registers of the batch and number of them quantized in one go.
  static constexpr unsigned NOF_REGS           = NOF_PRBS * NOF_REGS_PER_PRB;
  static constexpr unsigned QUANTIZATION_BLOCK = std::min(NOF_PRBS, 2U) * NOF_REGS_PER_PRB;

  const float32x4_t min_f32 = vdupq_n_f32(MIN_VALUE);
  const float32x4_t max_f32 = vdupq_n_f32(MAX_VALUE);

  int16x8_t samples_s16[NOF_REGS];
  for (unsigned reg = 0; reg != NOF_REGS; reg += QUANTIZATION_BLOCK) {
    quantize_bf16<QUANTIZATION_BLOCK>(samples_s16 + reg, in + reg * 8, scale_f32, min_f32, max_f32);
  }

  // Shift counts of the packing, minus the exponent of each PRB.
  int16x8_t shift_counts_s16 = get_pack_shift_counts<BITS>();
  int16x8_t exponents_s16    = vdupq_n_s16(0);
  if constexpr (HAS_EXPONENT) {
    exponents_s16 = calculate_exponents<NOF_PRBS>(samples_s16, BITS);
  }

  // Compression parameters of the PRBs, as bytes at the even positions.
  uint16x8_t comp_params_u16 = vaddq_u16(vreinterpretq_u16_s16(exponents_s16), vdupq_n_u16(exponent_offset));

  uint8x16_t packed_u8[NOF_REGS];
  for_each_index<NOF_PRBS>([&](auto prb) {
    static constexpr unsigned PRB = decltype(prb)::value;

    int16x8_t shift_s16 = shift_counts_s16;
    if constexpr (HAS_EXPONENT) {
      shift_s16 = vsubq_s16(shift_counts_s16, vdupq_laneq_s16(exponents_s16, PRB));
    }
    for (unsigned reg = 0; reg != NOF_REGS_PER_PRB; ++reg) {
      packed_u8[PRB * NOF_REGS_PER_PRB + reg] =
          pack_register<BITS, HAS_EXPONENT>(samples_s16[PRB * NOF_REGS_PER_PRB + reg], shift_s16);
    }
  });

  for_each_index<NOF_PRBS>([&](auto prb) {
    static constexpr unsigned PRB = decltype(prb)::value;

    uint8_t* prb_out = out + PRB * PRB_SIZE;
    if constexpr (IS_BFP) {
      vst1q_lane_u8(prb_out, vreinterpretq_u8_u16(comp_params_u16), 2 * PRB);
    }

    // The first two registers are stored with full 16 byte writes, as the following write overwrites their padding
    // bytes and stays inside of the PRB. The last one only writes its packed bytes.
    uint8_t* samples_out = prb_out + SAMPLES_OFFSET;
    vst1q_u8(samples_out, packed_u8[PRB * NOF_REGS_PER_PRB]);
    vst1q_u8(samples_out + BITS, packed_u8[PRB * NOF_REGS_PER_PRB + 1]);
    store_bytes<BITS>(samples_out + 2 * BITS, packed_u8[PRB * NOF_REGS_PER_PRB + 2]);
  });
}

/// \brief Compresses the given PRBs, four at a time.
///
/// \tparam BITS   Bit width of the packed samples.
/// \tparam IS_BFP Whether the PRBs start with the BFP compression parameter and are shifted by their exponent.
/// \param[out] out            Output bytes of the PRBs.
/// \param[in] in              Input samples of the PRBs.
/// \param[in] nof_prbs        Number of PRBs.
/// \param[in] scale           Scaling factor applied to the samples before the conversion to integers.
/// \param[in] exponent_offset Offset added to the BFP exponents.
template <unsigned BITS, bool IS_BFP>
inline void compress_prbs(uint8_t* out, const bf16_t* in, unsigned nof_prbs, float scale, uint8_t exponent_offset)
{
  static constexpr unsigned PRB_SIZE = NOF_REGS_PER_PRB * BITS + (IS_BFP ? 1 : 0);

  const float32x4_t scale_f32 = vdupq_n_f32(scale);

  unsigned prb = 0;
  for (unsigned prb_end = (nof_prbs / 4) * 4; prb != prb_end; prb += 4) {
    compress_prb_batch<BITS, IS_BFP, 4>(
        out + prb * PRB_SIZE, in + prb * NOF_SAMPLES_PER_PRB, scale_f32, exponent_offset);
  }
  for (; prb != nof_prbs; ++prb) {
    compress_prb_batch<BITS, IS_BFP, 1>(
        out + prb * PRB_SIZE, in + prb * NOF_SAMPLES_PER_PRB, scale_f32, exponent_offset);
  }
}

/// Compresses the given PRBs with the given bit width, see \ref compress_prbs.
template <bool IS_BFP>
inline void compress_prbs(uint8_t*      out,
                          const bf16_t* in,
                          unsigned      nof_prbs,
                          unsigned      data_width,
                          float         scale,
                          uint8_t       exponent_offset)
{
  switch (data_width) {
    case 8:
      return compress_prbs<8, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    case 9:
      return compress_prbs<9, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    case 10:
      return compress_prbs<10, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    case 11:
      return compress_prbs<11, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    case 12:
      return compress_prbs<12, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    case 13:
      return compress_prbs<13, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    case 14:
      return compress_prbs<14, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    case 15:
      return compress_prbs<15, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    case 16:
      return compress_prbs<16, IS_BFP>(out, in, nof_prbs, scale, exponent_offset);
    default:
      break;
  }
}

} // namespace neon
} // namespace ofh
} // namespace ocudu
