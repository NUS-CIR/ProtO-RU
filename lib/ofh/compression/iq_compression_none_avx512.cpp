// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_none_avx512.h"
#include "avx512_helpers.h"
#include "packing_utils_avx512.h"
#include "quantizer.h"
#include "ocudu/ofh/compression/compression_properties.h"
#include <algorithm>

using namespace ocudu;
using namespace ofh;

/// \brief Compresses the given PRBs without compression, four at a time.
///
/// \tparam BITS         Bit width of the packed samples.
/// \param[out] out      Output bytes of the PRBs.
/// \param[in] in        Input samples of the PRBs.
/// \param[in] nof_prbs  Number of PRBs.
/// \param[in] scale_ps  AVX512 register storing the scaling factor applied before the conversion to fixed point.
/// \param[in] max_ps    AVX512 register storing the largest scaled value in every element.
/// \param[in] min_epi16 AVX512 register storing the smallest quantized value in every element.
template <unsigned BITS>
static void
compress_prb_quads(uint8_t* out, const bf16_t* in, unsigned nof_prbs, __m512 scale_ps, __m512 max_ps, __m512i min_epi16)
{
  // Number of quantized samples per resource block.
  static constexpr unsigned NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;
  // Size in bytes of one packed PRB.
  static constexpr unsigned PRB_SIZE = 3 * BITS;
  // Number of samples of four PRBs, which fill three AVX512 registers.
  static constexpr unsigned NOF_SAMPLES_PER_QUAD = 4 * NOF_SAMPLES_PER_PRB;
  static constexpr unsigned AVX512_REG_SIZE      = 32;

  // The loop below is software pipelined in two stages, written as lambdas so that they share the parameters:
  // - prepare_quad(): loads four PRBs and quantizes them;
  // - emit_quad(): packs the quantized samples into the output buffer.
  // The next quad is prepared before emitting the current one, so that the CPU overlaps the two chains.

  // Quantized samples of four PRBs, see the file brief of avx512_helpers.h.
  struct quad_state {
    __m512i r0_epi16;
    __m512i r1_epi16;
    __m512i r2_epi16;
  };

  // Loads up to four PRBs and converts them to fixed point. The samples beyond the given number of PRBs are not read.
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
    // The scaled samples above the full scale saturate in the float domain, and the ones below the negative full scale
    // in the 16bit domain, as the conversion to integers gives the most negative value if it overflows. The 16bit
    // samples use the whole range, so they are not clamped.
    auto quantize = [&](unsigned reg) {
      __m512i samples_epi16 = mm512::quantize_bf16_even_odd(load(reg), scale_ps, max_ps);
      if constexpr (BITS != MAX_IQ_WIDTH) {
        samples_epi16 = _mm512_max_epi16(samples_epi16, min_epi16);
      }
      return samples_epi16;
    };
    return quad_state{quantize(0), quantize(1), quantize(2)};
  };

  // Packs up to four PRBs.
  auto emit_quad = [&](const quad_state& state, uint8_t* quad_out, unsigned quad_nof_prbs) {
    const __m512i no_comp_param = _mm512_setzero_si512();
    if constexpr (BITS == MAX_IQ_WIDTH) {
      mm512::pack_prb_quad_16b_big_endian<false>(
          quad_out, state.r0_epi16, state.r1_epi16, state.r2_epi16, no_comp_param, quad_nof_prbs);
    } else {
      mm512::pack_prb_quad_big_endian<BITS, false>(
          quad_out, state.r0_epi16, state.r1_epi16, state.r2_epi16, no_comp_param, quad_nof_prbs);
    }
  };

  // Software pipelined loop over the complete quads of PRBs (see above).
  unsigned nof_quads = nof_prbs / 4;
  if (nof_quads != 0) {
    quad_state current = prepare_quad(in, 4);
    for (unsigned quad = 1; quad != nof_quads; ++quad) {
      quad_state next = prepare_quad(in + quad * NOF_SAMPLES_PER_QUAD, 4);
      emit_quad(current, out + (quad - 1) * 4 * PRB_SIZE, 4);
      current = next;
    }
    emit_quad(current, out + (nof_quads - 1) * 4 * PRB_SIZE, 4);
    in += nof_quads * NOF_SAMPLES_PER_QUAD;
    out += nof_quads * 4 * PRB_SIZE;
  }

  // Incomplete quad at the end.
  if (nof_prbs % 4 != 0) {
    emit_quad(prepare_quad(in, nof_prbs % 4), out, nof_prbs % 4);
  }
}

void iq_compression_none_avx512::compress(span<uint8_t>                buffer,
                                          span<const cbf16_t>          iq_data,
                                          const ru_compression_params& params)
{
  // Use generic implementation if AVX512 utils don't support requested bit width.
  if (!mm512::iq_width_packing_supported(params.data_width)) {
    iq_compression_none_impl::compress(buffer, iq_data, params);
    return;
  }

  const unsigned data_width = params.data_width;

  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  const auto* input_it = reinterpret_cast<const bf16_t*>(iq_data.data());
  log_quantization_rms({input_it, iq_data.size() * 2U}, data_width);

  // Scaling applied to the input floats before the conversion to fixed point, including the quantizer gain.
  const float   full_scale = static_cast<float>((1 << (data_width - 1)) - 1);
  const __m512  scale_ps   = _mm512_set1_ps(full_scale * scaling_table.get_compression_iq_scaling(data_width));
  const __m512  max_ps     = _mm512_set1_ps(full_scale);
  const __m512i min_epi16  = _mm512_set1_epi16(static_cast<int16_t>(-full_scale));

  switch (data_width) {
    case 9:
      compress_prb_quads<9>(buffer.data(), input_it, nof_prbs, scale_ps, max_ps, min_epi16);
      break;
    case 14:
      compress_prb_quads<14>(buffer.data(), input_it, nof_prbs, scale_ps, max_ps, min_epi16);
      break;
    default:
      compress_prb_quads<16>(buffer.data(), input_it, nof_prbs, scale_ps, max_ps, min_epi16);
      break;
  }
}

bool iq_compression_none_avx512::decompress(span<cbf16_t>                iq_data,
                                            span<const uint8_t>          compressed_data,
                                            const ru_compression_params& params)
{
  // Use generic implementation if AVX512 utils don't support requested bit width.
  if (!mm512::iq_width_packing_supported(params.data_width)) {
    return iq_compression_none_impl::decompress(iq_data, compressed_data, params);
  }

  // Number of output PRBs.
  unsigned nof_prbs = iq_data.size() / NOF_SUBCARRIERS_PER_RB;

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned comp_prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(compressed_data.size() >= nof_prbs * comp_prb_size,
               "Input does not contain enough bytes to decompress {} PRBs",
               nof_prbs);

  // Quantizer object.
  quantizer q_out(params.data_width);

  std::array<int16_t, MAX_NOF_SUBCARRIERS * 2> unpacked_iq_data;

  unsigned idx = 0;
  for (unsigned rb = 0; rb != nof_prbs; ++rb) {
    span<const uint8_t> comp_prb_buffer(&compressed_data[rb * comp_prb_size], comp_prb_size);

    // Unpack resource block.
    span<int16_t> unpacked_prb_span(&unpacked_iq_data[idx], NOF_SUBCARRIERS_PER_RB * 2);
    mm512::unpack_prb_big_endian(unpacked_prb_span, comp_prb_buffer, params.data_width);

    idx += (NOF_SUBCARRIERS_PER_RB * 2);
  }

  span<int16_t> unpacked_iq_int16_span(unpacked_iq_data.data(), iq_data.size() * 2);
  // Convert to complex brain float samples.
  q_out.to_brain_float(iq_data, unpacked_iq_int16_span, scaling_table.get_decompression_iq_scaling(params.data_width));

  return true;
}
