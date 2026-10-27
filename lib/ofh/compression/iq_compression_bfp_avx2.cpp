// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_bfp_avx2.h"
#include "avx2_helpers.h"
#include "packing_utils_avx2.h"
#include "quantizer.h"
#include "ocudu/ocuduvec/prod.h"
#include "ocudu/ofh/compression/compression_properties.h"
#include "ocudu/support/math/math_utils.h"

using namespace ocudu;
using namespace ofh;

void iq_compression_bfp_avx2::compress(span<uint8_t>                buffer,
                                       span<const cbf16_t>          iq_data,
                                       const ru_compression_params& params)
{
  // Use generic implementation if AVX2 utils don't support requested bit width.
  if (!mm256::iq_width_packing_supported(params.data_width)) {
    iq_compression_bfp_impl::compress(buffer, iq_data, params);
    return;
  }

  // AVX2 register size in a number of 16bit words.
  static constexpr size_t AVX2_REG_SIZE = 16;

  // Compression bit width. It is kept in a local variable, as otherwise it is reloaded after every write to the output
  // buffer.
  const unsigned data_width = params.data_width;

  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  // Offset added to the transmitted exponents.
  uint8_t exponent_offset = scaling_table.get_compression_exponent_offset(params.data_width);

  log_quantization_rms({reinterpret_cast<const bf16_t*>(iq_data.data()), iq_data.size() * 2U}, params.data_width);

  // Scaling applied to the input floats before the conversion to fixed point, including the quantizer gain.
  const float  quantizer_gain = (1 << (Q_BIT_WIDTH - 1)) - 1.0F;
  const __m256 scale_ps = _mm256_set1_ps(quantizer_gain * scaling_table.get_compression_iq_scaling(params.data_width));

  // Compression algorithm implemented according to O-RAN.WG4.CUS Annex A.1.2.
  const auto* input_it = reinterpret_cast<const bf16_t*>(iq_data.data());
  unsigned    rb       = 0;

  unsigned nof_prb_pairs = nof_prbs / 2;
  if (data_width == MAX_IQ_WIDTH) {
    // Samples that already use all the bits of the data width are not shifted, so the exponent is always zero.
    mm256::compress_prb_pairs_16b<true>(buffer.data(), input_it, nof_prb_pairs, prb_size, scale_ps, exponent_offset);
    rb += nof_prb_pairs * 2;
    input_it += NOF_SAMPLES_PER_PRB * 2 * nof_prb_pairs;
  } else if (nof_prb_pairs != 0) {
    // The PRBs are processed in pairs, as 2 PRBs fill exactly 3 AVX2 registers of 16 samples each. The loop below is
    // software pipelined in two stages, written as lambdas so that they share the parameters of the call:
    // - prepare_prb_pair(): loads a pair of PRBs, quantizes it and finds the exponents (long dependency chain);
    // - emit_prb_pair(): shifts the quantized samples by the exponents and packs them into the output buffer.
    // The next pair is prepared before emitting the current one, so that the CPU overlaps the two chains.

    // State of two PRBs after quantization and exponent calculation. The samples use the register layout of
    // mm256::calculate_exponents(), which also gives the format of the exponents.
    struct prb_pair_state {
      __m256i rb0_epi16;
      __m256i rb01_epi16;
      __m256i rb1_epi16;
      __m256i exponents;
    };

    // Loads the given pair of PRBs, converts it to fixed point and finds its exponents.
    auto prepare_prb_pair = [&](unsigned pair) {
      const bf16_t*  in = input_it + pair * 2 * NOF_SAMPLES_PER_PRB;
      prb_pair_state state;
      state.rb0_epi16 =
          mm256::quantize_bf16_even_odd(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in)), scale_ps);
      state.rb01_epi16 = mm256::quantize_bf16_even_odd(
          _mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + AVX2_REG_SIZE)), scale_ps);
      state.rb1_epi16 = mm256::quantize_bf16_even_odd(
          _mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + AVX2_REG_SIZE * 2)), scale_ps);
      state.exponents = mm256::calculate_exponents(state.rb0_epi16, state.rb01_epi16, state.rb1_epi16, data_width);
      return state;
    };

    // Shifts and packs the given pair of PRBs.
    auto emit_prb_pair = [&](const prb_pair_state& state, unsigned pair) {
      uint8_t* prb0 = &buffer[pair * 2 * prb_size];
      uint8_t* prb1 = prb0 + prb_size;

      // Shift the samples right by the exponents. The register shared by both PRBs takes its lower lane shifted by the
      // first exponent and its upper lane by the second one.
      __m128i exponent_0      = _mm256_castsi256_si128(state.exponents);
      __m128i exponent_1      = _mm256_extracti128_si256(state.exponents, 1);
      __m256i rb0_sr_epi16    = _mm256_sra_epi16(state.rb0_epi16, exponent_0);
      __m256i rb1_sr_epi16    = _mm256_sra_epi16(state.rb1_epi16, exponent_1);
      __m256i rb01_sr_0_epi16 = _mm256_sra_epi16(state.rb01_epi16, exponent_0);
      __m256i rb01_sr_1_epi16 = _mm256_sra_epi16(state.rb01_epi16, exponent_1);
      __m256i rb01_sr_epi16   = _mm256_blend_epi32(rb01_sr_0_epi16, rb01_sr_1_epi16, 0xf0);

      // Write the compression parameters followed by the packed samples.
      prb0[0] = static_cast<uint8_t>(_mm_cvtsi128_si32(exponent_0) + exponent_offset);
      prb1[0] = static_cast<uint8_t>(_mm_cvtsi128_si32(exponent_1) + exponent_offset);
      mm256::avx2_pack_prbs_9b_big_endian(prb0 + 1, prb1 + 1, rb0_sr_epi16, rb01_sr_epi16, rb1_sr_epi16);
    };

    // Software pipelined loop over the pairs of PRBs (see above), two pairs are prepared ahead of the emitted one. The
    // loop is unrolled three times so that the states rotate without copies.
    if (nof_prb_pairs == 1) {
      emit_prb_pair(prepare_prb_pair(0), 0);
    } else {
      prb_pair_state s0 = prepare_prb_pair(0);
      prb_pair_state s1 = prepare_prb_pair(1);
      prb_pair_state s2;
      unsigned       pair = 2;
      for (; pair + 2 < nof_prb_pairs; pair += 3) {
        s2 = prepare_prb_pair(pair);
        emit_prb_pair(s0, pair - 2);
        s0 = prepare_prb_pair(pair + 1);
        emit_prb_pair(s1, pair - 1);
        s1 = prepare_prb_pair(pair + 2);
        emit_prb_pair(s2, pair);
      }
      for (; pair != nof_prb_pairs; ++pair) {
        s2 = prepare_prb_pair(pair);
        emit_prb_pair(s0, pair - 2);
        s0 = s1;
        s1 = s2;
      }
      emit_prb_pair(s0, pair - 2);
      emit_prb_pair(s1, pair - 1);
    }
    rb += nof_prb_pairs * 2;
    input_it += nof_prb_pairs * 2 * NOF_SAMPLES_PER_PRB;
  }

  // Use generic implementation for the remaining resource blocks.
  for (; rb != nof_prbs; ++rb) {
    // Get view over buffer bytes corresponding to one PRB.
    span<uint8_t> comp_prb_buffer(&buffer[rb * prb_size], prb_size);

    std::array<int16_t, NOF_SAMPLES_PER_PRB> input_quantized;
    quantizer(Q_BIT_WIDTH)
        .to_fixed_point(input_quantized,
                        span<const bf16_t>(input_it, NOF_SAMPLES_PER_PRB),
                        scaling_table.get_compression_iq_scaling(params.data_width));
    compress_prb_generic(comp_prb_buffer, input_quantized, params.data_width, exponent_offset);
    input_it += NOF_SAMPLES_PER_PRB;
  }
}

bool iq_compression_bfp_avx2::decompress(span<cbf16_t>                iq_data,
                                         span<const uint8_t>          compressed_data,
                                         const ru_compression_params& params)
{
  // Use generic implementation if AVX2 utils don't support requested bit width.
  if (!mm256::iq_width_packing_supported(params.data_width)) {
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

  // Determine array size so that AVX2 store operation doesn't write the data out of array bounds.
  constexpr size_t avx2_size_iqs = 16;
  constexpr size_t prb_size      = divide_ceil(NOF_SUBCARRIERS_PER_RB * 2, avx2_size_iqs) * avx2_size_iqs;

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
    mm256::unpack_prb_big_endian(unpacked_prb_span, comp_prb_buffer, params.data_width);

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
