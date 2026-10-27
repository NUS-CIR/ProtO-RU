// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_bfp_avx512.h"
#include "avx512_helpers.h"
#include "packing_utils_avx512.h"
#include "quantizer.h"
#include "ocudu/ocuduvec/prod.h"
#include "ocudu/ofh/compression/compression_properties.h"
#include "ocudu/support/math/math_utils.h"
#include "ocudu/support/units.h"
#include <algorithm>

using namespace ocudu;
using namespace ofh;

/// \brief Compresses the given PRBs with BFP, four at a time.
///
/// \tparam BITS                Bit width of the compressed samples.
/// \param[out] out             Output bytes of the PRBs.
/// \param[in] in               Input samples of the PRBs.
/// \param[in] nof_prbs         Number of PRBs.
/// \param[in] scale_ps         AVX512 register storing the scaling factor applied before the conversion to fixed point.
/// \param[in] exponent_offset  Offset added to the transmitted exponents.
template <unsigned BITS>
static void
compress_prb_quads(uint8_t* out, const bf16_t* in, unsigned nof_prbs, __m512 scale_ps, uint8_t exponent_offset)
{
  // Number of quantized samples per resource block.
  static constexpr unsigned NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;
  // Size in bytes of one compressed PRB, including the compression parameter.
  static constexpr unsigned PRB_SIZE = 1 + 3 * BITS;
  // Number of samples of four PRBs, which fill three AVX512 registers.
  static constexpr unsigned NOF_SAMPLES_PER_QUAD = 4 * NOF_SAMPLES_PER_PRB;
  static constexpr unsigned AVX512_REG_SIZE      = 32;

  const __m512i exponent_offset_epi16 = _mm512_set1_epi16(exponent_offset);

  // The loop below is software pipelined in two stages, written as lambdas so that they share the parameters:
  // - prepare_quad(): loads four PRBs, quantizes them and finds their exponents (long dependency chain);
  // - emit_quad(): shifts the quantized samples by the exponents and packs them into the output buffer.
  // The next quad is prepared before emitting the current one, so that the CPU overlaps the two chains.

  // State of four PRBs after quantization and exponent calculation, see the file brief of avx512_helpers.h.
  struct quad_state {
    __m512i r0_epi16;
    __m512i r1_epi16;
    __m512i r2_epi16;
    __m512i exponents_epi16;
  };

  // Loads up to four PRBs, converts them to fixed point and finds their exponents. The samples beyond the given number
  // of PRBs are not read.
  auto prepare_quad = [&](const bf16_t* quad_in, unsigned quad_nof_prbs) {
    // Load the samples, masking out the ones of the missing PRBs if the quad is not complete.
    auto load = [&](unsigned reg) {
      const void* address = quad_in + reg * AVX512_REG_SIZE;
      if (quad_nof_prbs == 4) {
        return _mm512_loadu_si512(address);
      }
      int       nof_samples = static_cast<int>(quad_nof_prbs * NOF_SAMPLES_PER_PRB - reg * AVX512_REG_SIZE);
      __mmask32 mask        = (nof_samples > 0) ? (~uint32_t{0} >> std::max(0, 32 - nof_samples)) : 0;
      return _mm512_maskz_loadu_epi16(mask, address);
    };
    quad_state state;
    state.r0_epi16 = mm512::quantize_bf16_even_odd(load(0), scale_ps);
    state.r1_epi16 = mm512::quantize_bf16_even_odd(load(1), scale_ps);
    state.r2_epi16 = mm512::quantize_bf16_even_odd(load(2), scale_ps);
    // Samples that already use all the bits of the data width are not shifted, so the exponent is always zero.
    if constexpr (BITS != MAX_IQ_WIDTH) {
      state.exponents_epi16 = mm512::calculate_exponents(state.r0_epi16, state.r1_epi16, state.r2_epi16, BITS);
    }
    return state;
  };

  // Shifts and packs up to four PRBs.
  auto emit_quad = [&](const quad_state& state, uint8_t* quad_out, unsigned quad_nof_prbs) {
    if constexpr (BITS == MAX_IQ_WIDTH) {
      mm512::pack_prb_quad_16b_big_endian<true>(
          quad_out, state.r0_epi16, state.r1_epi16, state.r2_epi16, exponent_offset_epi16, quad_nof_prbs);
    } else {
      // Shift the samples of each RB right by its exponent. The shift counts of a register take the exponent of the RB
      // held by each of its lanes.
      __m512i e_epi16  = state.exponents_epi16;
      __m512i r0_epi16 = _mm512_srav_epi16(
          state.r0_epi16, _mm512_shuffle_i64x2(e_epi16, e_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(1, 0, 0, 0)));
      __m512i r1_epi16 = _mm512_srav_epi16(
          state.r1_epi16, _mm512_shuffle_i64x2(e_epi16, e_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(2, 2, 1, 1)));
      __m512i r2_epi16 = _mm512_srav_epi16(
          state.r2_epi16, _mm512_shuffle_i64x2(e_epi16, e_epi16, (_MM_PERM_ENUM)_MM_SHUFFLE(3, 3, 3, 2)));

      // Pack the samples together with the compression parameters.
      __m512i comp_param_epi16 = _mm512_add_epi16(e_epi16, exponent_offset_epi16);
      mm512::pack_prb_quad_big_endian<BITS, true>(
          quad_out, r0_epi16, r1_epi16, r2_epi16, comp_param_epi16, quad_nof_prbs);
    }
  };

  // Software pipelined loop over the complete quads of PRBs (see above), two quads are prepared ahead of the emitted
  // one. The loop is unrolled three times so that the states rotate without copies.
  unsigned nof_quads = nof_prbs / 4;
  if (nof_quads == 1) {
    emit_quad(prepare_quad(in, 4), out, 4);
  } else if (nof_quads != 0) {
    auto quad_in  = [&](unsigned quad) { return in + quad * NOF_SAMPLES_PER_QUAD; };
    auto quad_out = [&](unsigned quad) { return out + quad * 4 * PRB_SIZE; };

    quad_state s0 = prepare_quad(quad_in(0), 4);
    quad_state s1 = prepare_quad(quad_in(1), 4);
    quad_state s2;
    unsigned   quad = 2;
    for (; quad + 2 < nof_quads; quad += 3) {
      s2 = prepare_quad(quad_in(quad), 4);
      emit_quad(s0, quad_out(quad - 2), 4);
      s0 = prepare_quad(quad_in(quad + 1), 4);
      emit_quad(s1, quad_out(quad - 1), 4);
      s1 = prepare_quad(quad_in(quad + 2), 4);
      emit_quad(s2, quad_out(quad), 4);
    }
    for (; quad != nof_quads; ++quad) {
      s2 = prepare_quad(quad_in(quad), 4);
      emit_quad(s0, quad_out(quad - 2), 4);
      s0 = s1;
      s1 = s2;
    }
    emit_quad(s0, quad_out(quad - 2), 4);
    emit_quad(s1, quad_out(quad - 1), 4);
  }
  in += nof_quads * NOF_SAMPLES_PER_QUAD;
  out += nof_quads * 4 * PRB_SIZE;

  // Incomplete quad at the end.
  if (nof_prbs % 4 != 0) {
    emit_quad(prepare_quad(in, nof_prbs % 4), out, nof_prbs % 4);
  }
}

void iq_compression_bfp_avx512::compress(span<uint8_t>                buffer,
                                         span<const cbf16_t>          iq_data,
                                         const ru_compression_params& params)
{
  // Use generic implementation if AVX512 utils don't support requested bit width.
  if (!mm512::iq_width_packing_supported(params.data_width)) {
    iq_compression_bfp_impl::compress(buffer, iq_data, params);
    return;
  }

  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  // Offset added to the transmitted exponents.
  uint8_t exponent_offset = scaling_table.get_compression_exponent_offset(params.data_width);

  const auto* input_it = reinterpret_cast<const bf16_t*>(iq_data.data());
  log_quantization_rms({input_it, iq_data.size() * 2U}, params.data_width);

  // Scaling applied to the input floats before the conversion to fixed point, including the quantizer gain.
  const float  quantizer_gain = (1 << (Q_BIT_WIDTH - 1)) - 1.0F;
  const __m512 scale_ps = _mm512_set1_ps(quantizer_gain * scaling_table.get_compression_iq_scaling(params.data_width));

  // Compression algorithm implemented according to O-RAN.WG4.CUS Annex A.1.2.
  switch (params.data_width) {
    case 9:
      compress_prb_quads<9>(buffer.data(), input_it, nof_prbs, scale_ps, exponent_offset);
      break;
    case 14:
      compress_prb_quads<14>(buffer.data(), input_it, nof_prbs, scale_ps, exponent_offset);
      break;
    default:
      compress_prb_quads<16>(buffer.data(), input_it, nof_prbs, scale_ps, exponent_offset);
      break;
  }
}

bool iq_compression_bfp_avx512::decompress(span<cbf16_t>                iq_data,
                                           span<const uint8_t>          compressed_data,
                                           const ru_compression_params& params)
{
  // Use generic implementation if AVX512 utils don't support requested bit width.
  if (!mm512::iq_width_packing_supported(params.data_width)) {
    return iq_compression_bfp_impl::decompress(iq_data, compressed_data, params);
  }

  // Number of output PRBs.
  unsigned nof_prbs = iq_data.size() / NOF_SUBCARRIERS_PER_RB;

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned comp_prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(compressed_data.size() >= nof_prbs * comp_prb_size,
               "Input does not contain enough bytes to decompress {} PRBs",
               nof_prbs);

  const float fixp_gain = (1 << (Q_BIT_WIDTH - 1)) - 1.0f;
  // Scaling of the decompressed samples, including the quantizer gain.
  const float iq_scaling = scaling_table.get_decompression_iq_scaling(params.data_width) / fixp_gain;

  // Determine array size so that AVX512 store operation doesn't write the data out of array bounds.
  constexpr size_t avx512_size_iqs = 32;
  constexpr size_t prb_size        = divide_ceil(NOF_SUBCARRIERS_PER_RB * 2, avx512_size_iqs) * avx512_size_iqs;

  alignas(64) std::array<int16_t, MAX_NOF_PRBS * prb_size> unpacked_iq_data;
  alignas(64) std::array<float, MAX_NOF_SUBCARRIERS * 2>   unpacked_iq_scaling;

  unsigned idx = 0;
  for (unsigned c_prb_idx = 0; c_prb_idx != nof_prbs; ++c_prb_idx) {
    // Get view over compressed PRB bytes.
    span<const uint8_t> comp_prb_buffer(&compressed_data[c_prb_idx * comp_prb_size], comp_prb_size);

    uint8_t exponent = decode_bfp_exponent(comp_prb_buffer[0]);
    float   scaler   = 1 << exponent;

    // Get view over the bytes following the compression parameter.
    comp_prb_buffer = comp_prb_buffer.last(comp_prb_buffer.size() - sizeof(exponent));

    // Unpack resource block.
    span<int16_t> unpacked_prb_span(&unpacked_iq_data[idx], prb_size);
    mm512::unpack_prb_big_endian(unpacked_prb_span, comp_prb_buffer, params.data_width);

    // Save scaling factor.
    std::fill(&unpacked_iq_scaling[idx], &unpacked_iq_scaling[idx] + (NOF_SUBCARRIERS_PER_RB * 2), scaler * iq_scaling);

    idx += (NOF_SUBCARRIERS_PER_RB * 2);
  }
  span<int16_t> unpacked_iq_int16_span(unpacked_iq_data.data(), iq_data.size() * 2);
  span<float>   unpacked_iq_scaling_span(unpacked_iq_scaling.data(), iq_data.size() * 2);

  // Scale unpacked IQ samples using saved exponents and convert to complex samples.
  ocuduvec::convert(iq_data, unpacked_iq_int16_span, unpacked_iq_scaling_span);

  return true;
}
