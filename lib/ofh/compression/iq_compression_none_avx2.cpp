// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_none_avx2.h"
#include "avx2_helpers.h"
#include "packing_utils_avx2.h"
#include "packing_utils_generic.h"
#include "quantizer.h"
#include "ocudu/ofh/compression/compression_properties.h"

using namespace ocudu;
using namespace ofh;

void iq_compression_none_avx2::compress(span<uint8_t>                buffer,
                                        span<const cbf16_t>          iq_data,
                                        const ru_compression_params& params)
{
  // Use generic implementation if AVX2 utils don't support requested bit width.
  if (!mm256::iq_width_packing_supported(params.data_width)) {
    iq_compression_none_impl::compress(buffer, iq_data, params);
    return;
  }

  // Number of quantized samples per resource block.
  static constexpr size_t NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;
  // AVX2 register size in a number of 16-bit words.
  static constexpr size_t AVX2_REG_SIZE = 16;

  // Compression bit width. It is kept in a local variable, as otherwise it is reloaded after every write to the output
  // buffer.
  const unsigned data_width = params.data_width;

  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  log_quantization_rms({reinterpret_cast<const bf16_t*>(iq_data.data()), iq_data.size() * 2U}, data_width);

  // Quantizer object, which defines the gain and the largest quantized value.
  const quantizer q(data_width);

  // Scaling applied to the input floats before the conversion to fixed point, including the quantizer gain.
  const float  full_scale = static_cast<float>((1 << (data_width - 1)) - 1);
  const float  iq_scaling = scaling_table.get_compression_iq_scaling(data_width);
  const __m256 scale_ps   = _mm256_set1_ps(full_scale * iq_scaling);

  const auto* input_it = reinterpret_cast<const bf16_t*>(iq_data.data());
  unsigned    rb       = 0;

  // One AVX2 register stores 8 16-bit IQ pairs. We can process 2 PRBs at a time by using 3 AVX2 registers.
  unsigned nof_prb_pairs = nof_prbs / 2;
  if (data_width == MAX_IQ_WIDTH) {
    mm256::compress_prb_pairs_16b<false>(buffer.data(), input_it, nof_prb_pairs, prb_size, scale_ps, 0);
    rb += nof_prb_pairs * 2;
    input_it += NOF_SAMPLES_PER_PRB * 2 * nof_prb_pairs;
  } else {
    // The scaled samples above the full scale saturate in the float domain, and the ones below the negative full scale
    // in the 16-bit domain, as the conversion to integers gives the most negative value if it overflows.
    const __m256  max_ps           = _mm256_set1_ps(full_scale);
    const __m256i min_epi16        = _mm256_set1_epi16(static_cast<int16_t>(-full_scale));
    auto          quantize_prb_reg = [&](const bf16_t* in) {
      return _mm256_max_epi16(
          mm256::quantize_bf16_even_odd(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in)), scale_ps, max_ps),
          min_epi16);
    };

    // Quantized samples of a pair of PRBs, with the register layout of mm256::avx2_pack_prbs_9b_big_endian().
    struct prb_pair_state {
      __m256i rb0_epi16;
      __m256i rb01_epi16;
      __m256i rb1_epi16;
    };
    auto prepare_prb_pair = [&](unsigned pair) {
      const bf16_t* in = input_it + pair * 2 * NOF_SAMPLES_PER_PRB;
      return prb_pair_state{
          quantize_prb_reg(in), quantize_prb_reg(in + AVX2_REG_SIZE), quantize_prb_reg(in + AVX2_REG_SIZE * 2)};
    };
    auto emit_prb_pair = [&](const prb_pair_state& state, unsigned pair) {
      uint8_t* prb0 = &buffer[pair * 2 * prb_size];
      mm256::avx2_pack_prbs_9b_big_endian(prb0, prb0 + prb_size, state.rb0_epi16, state.rb01_epi16, state.rb1_epi16);
    };

    // Software pipelined loop: the next pair is quantized before packing the current one, so that the CPU overlaps
    // the two dependency chains.
    if (nof_prb_pairs != 0) {
      prb_pair_state current = prepare_prb_pair(0);
      for (unsigned pair = 1; pair != nof_prb_pairs; ++pair) {
        prb_pair_state next = prepare_prb_pair(pair);
        emit_prb_pair(current, pair - 1);
        current = next;
      }
      emit_prb_pair(current, nof_prb_pairs - 1);
    }
    rb += nof_prb_pairs * 2;
    input_it += nof_prb_pairs * 2 * NOF_SAMPLES_PER_PRB;
  }

  // Use generic implementation for the remaining resource blocks.
  for (; rb != nof_prbs; ++rb) {
    std::array<int16_t, NOF_SAMPLES_PER_PRB> input_quantized;
    q.to_fixed_point(input_quantized, span<const bf16_t>(input_it, NOF_SAMPLES_PER_PRB), iq_scaling);

    // View over output bytes corresponding to the current PRB.
    span<uint8_t> out_compr_prb(&buffer[rb * prb_size], prb_size);

    bit_buffer bit_buf = bit_buffer::from_bytes(out_compr_prb);
    pack_bytes(bit_buf, input_quantized, data_width);

    input_it += NOF_SAMPLES_PER_PRB;
  }
}

bool iq_compression_none_avx2::decompress(span<cbf16_t>                iq_data,
                                          span<const uint8_t>          compressed_data,
                                          const ru_compression_params& params)
{
  // Use generic implementation if AVX2 utils don't support requested bit width.
  if (!mm256::iq_width_packing_supported(params.data_width)) {
    return iq_compression_none_impl::decompress(iq_data, compressed_data, params);
  }

  // Number of output PRBs.
  unsigned nof_prbs = iq_data.size() / NOF_SUBCARRIERS_PER_RB;

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned comp_prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(compressed_data.size() >= nof_prbs * comp_prb_size,
               "Input does not contain enough bytes to decompress {} PRBs",
               nof_prbs);

  // Scaling of the decompressed samples, including the quantizer gain.
  const float  iq_scaling   = scaling_table.get_decompression_iq_scaling(params.data_width);
  const __m256 scale_ps     = _mm256_set1_ps(quantizer(params.data_width).to_float(1) * iq_scaling);
  auto         get_exponent = [](const uint8_t* /* prb */) { return 0; };

  if (params.data_width == MAX_IQ_WIDTH) {
    mm256::decompress_prbs<16, false>(iq_data.data(), compressed_data.data(), nof_prbs, scale_ps, get_exponent);
  } else {
    mm256::decompress_prbs<9, false>(iq_data.data(), compressed_data.data(), nof_prbs, scale_ps, get_exponent);
  }
  return true;
}
