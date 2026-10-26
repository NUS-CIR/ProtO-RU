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

/// \brief Compresses pairs of PRBs using 16bit samples.
///
/// Samples that already use all the bits of the data width are not shifted, so the exponent is always zero. The
/// conversion to big-endian is merged with the reordering of the quantized samples.
static void compress_prb_pairs_16b(uint8_t*      out,
                                   const bf16_t* in,
                                   unsigned      nof_prb_pairs,
                                   unsigned      prb_size,
                                   __m256        scale_ps,
                                   uint8_t       comp_param)
{
  // AVX2 register size in a number of 16bit words.
  static constexpr unsigned AVX2_REG_SIZE = 16;

  const __m256i bswap_mask_epi8 =
      _mm256_setr_epi64x(0x0a0b020308090001, 0x0e0f06070c0d0405, 0x0a0b020308090001, 0x0e0f06070c0d0405);
  for (unsigned pair = 0; pair != nof_prb_pairs; ++pair) {
    __m256i rb0_epi16 = _mm256_shuffle_epi8(
        mm256::quantize_bf16_even_odd(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in)), scale_ps),
        bswap_mask_epi8);
    __m256i rb01_epi16 =
        _mm256_shuffle_epi8(mm256::quantize_bf16_even_odd(
                                _mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + AVX2_REG_SIZE)), scale_ps),
                            bswap_mask_epi8);
    __m256i rb1_epi16 =
        _mm256_shuffle_epi8(mm256::quantize_bf16_even_odd(
                                _mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + AVX2_REG_SIZE * 2)), scale_ps),
                            bswap_mask_epi8);

    // Write the compression parameter followed by the samples of each PRB.
    uint8_t* prb0 = out;
    uint8_t* prb1 = prb0 + prb_size;
    prb0[0]       = comp_param;
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(prb0 + 1), rb0_epi16);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(prb0 + 33), _mm256_castsi256_si128(rb01_epi16));
    prb1[0] = comp_param;
    _mm_storeu_si128(reinterpret_cast<__m128i*>(prb1 + 1), _mm256_extracti128_si256(rb01_epi16, 1));
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(prb1 + 17), rb1_epi16);

    in += AVX2_REG_SIZE * 3;
    out += prb_size * 2;
  }
}

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

  // State of two PRBs after quantization and exponent calculation.
  struct prb_pair_state {
    __m256i rb0_epi16;
    __m256i rb01_epi16;
    __m256i rb1_epi16;
    __m128i exponent_0;
    __m128i exponent_1;
  };

  // Loads two PRBs, converts them to fixed point and finds their exponents.
  auto prepare_prb_pair = [&](const bf16_t* in) {
    prb_pair_state state;
    // Load symbols and convert them to fixed point.
    state.rb0_epi16 = mm256::quantize_bf16(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in)), scale_ps);
    state.rb01_epi16 =
        mm256::quantize_bf16(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + AVX2_REG_SIZE)), scale_ps);
    state.rb1_epi16 =
        mm256::quantize_bf16(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in + AVX2_REG_SIZE * 2)), scale_ps);

    // Samples that already use all the bits of the data width are not shifted, so the exponent is always zero.
    if (params.data_width == MAX_IQ_WIDTH) {
      state.exponent_0 = _mm_setzero_si128();
      state.exponent_1 = _mm_setzero_si128();
      return state;
    }

    // Find exponents for each of the 2 PRBs.
    __m256i exponents =
        mm256::calculate_exponents(state.rb0_epi16, state.rb01_epi16, state.rb1_epi16, params.data_width);
    state.exponent_0 = _mm256_castsi256_si128(exponents);
    state.exponent_1 = _mm256_extracti128_si256(exponents, 1);
    return state;
  };

  // Shifts and packs two PRBs.
  auto emit_prb_pair = [&](const prb_pair_state& state, unsigned first_rb) {
    // Get view over bytes corresponding to the two PRBs.
    span<uint8_t> comp_prb0_buffer(&buffer[first_rb * prb_size], prb_size);
    span<uint8_t> comp_prb1_buffer(&buffer[(first_rb + 1) * prb_size], prb_size);

    // Shift original IQ samples right, unless the exponent is known to be zero.
    __m256i rb0_sr_epi16  = state.rb0_epi16;
    __m256i rb1_sr_epi16  = state.rb1_epi16;
    __m256i rb01_sr_epi16 = state.rb01_epi16;
    if (params.data_width != MAX_IQ_WIDTH) {
      rb0_sr_epi16            = _mm256_sra_epi16(state.rb0_epi16, state.exponent_0);
      rb1_sr_epi16            = _mm256_sra_epi16(state.rb1_epi16, state.exponent_1);
      __m256i rb01_sr_0_epi16 = _mm256_sra_epi16(state.rb01_epi16, state.exponent_0);
      __m256i rb01_sr_1_epi16 = _mm256_sra_epi16(state.rb01_epi16, state.exponent_1);
      rb01_sr_epi16           = _mm256_blend_epi32(rb01_sr_0_epi16, rb01_sr_1_epi16, 0xf0);
    }

    // Write compression parameters to the output buffer.
    comp_prb0_buffer[0] = static_cast<uint8_t>(_mm_cvtsi128_si32(state.exponent_0) + exponent_offset);
    comp_prb1_buffer[0] = static_cast<uint8_t>(_mm_cvtsi128_si32(state.exponent_1) + exponent_offset);

    // Pack 2 PRBs using utility function.
    mm256::pack_prbs_big_endian(comp_prb0_buffer.last(prb_size - sizeof(uint8_t)),
                                comp_prb1_buffer.last(prb_size - sizeof(uint8_t)),
                                rb0_sr_epi16,
                                rb01_sr_epi16,
                                rb1_sr_epi16,
                                params.data_width);
  };

  // One AVX2 register stores 8 16-bit IQ pairs. We can process 2 PRBs at a time by using 3 AVX2 registers. The
  // quantization of the next pair is done before packing the current one to overlap the long dependency chains.
  unsigned nof_prb_pairs = nof_prbs / 2;
  if (params.data_width == MAX_IQ_WIDTH) {
    compress_prb_pairs_16b(buffer.data(), input_it, nof_prb_pairs, prb_size, scale_ps, exponent_offset);
    rb += nof_prb_pairs * 2;
    input_it += NOF_SAMPLES_PER_PRB * 2 * nof_prb_pairs;
  } else if (nof_prb_pairs != 0) {
    prb_pair_state current = prepare_prb_pair(input_it);
    for (unsigned pair = 1; pair != nof_prb_pairs; ++pair) {
      input_it += NOF_SAMPLES_PER_PRB * 2;
      prb_pair_state next = prepare_prb_pair(input_it);
      emit_prb_pair(current, rb);
      current = next;
      rb += 2;
    }
    emit_prb_pair(current, rb);
    rb += 2;
    input_it += NOF_SAMPLES_PER_PRB * 2;
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
