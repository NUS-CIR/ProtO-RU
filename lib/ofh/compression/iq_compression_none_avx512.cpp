// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_none_avx512.h"
#include "avx512_helpers.h"
#include "packing_utils_avx512.h"
#include "quantizer.h"
#include "ocudu/ofh/compression/compression_properties.h"
#include "ocudu/support/cpu_features.h"

using namespace ocudu;
using namespace ofh;

void iq_compression_none_avx512::compress(span<uint8_t>                buffer,
                                          span<const cbf16_t>          iq_data,
                                          const ru_compression_params& params)
{
  // Use generic implementation if AVX512 utils don't support requested bit width.
  if (!mm512::iq_width_packing_supported(params.data_width)) {
    iq_compression_none_impl::compress(buffer, iq_data, params);
    return;
  }

  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  span<const bf16_t> float_samples(reinterpret_cast<const bf16_t*>(iq_data.data()), iq_data.size() * 2U);
  log_quantized_input_rms(float_samples, params.data_width);

  // Scaling applied to the input floats before the conversion to fixed point, including the quantizer gain, and the
  // largest quantized value.
  const float   full_scale  = static_cast<float>((1 << (params.data_width - 1)) - 1);
  const float   scale       = full_scale * scaling_table.get_compression_iq_scaling(params.data_width);
  const __m512  scale_ps    = _mm512_set1_ps(scale);
  const __m512i clamp_epi16 = _mm512_set1_epi16(get_bf16_saturation_clamp(scale));

  switch (params.data_width) {
    case 9:
      mm512::compress_prb_quads<9>(buffer.data(), float_samples.data(), nof_prbs, scale_ps, clamp_epi16, full_scale);
      break;
    case 14:
      mm512::compress_prb_quads<14>(buffer.data(), float_samples.data(), nof_prbs, scale_ps, clamp_epi16, full_scale);
      break;
    default:
      mm512::compress_prb_quads<16>(buffer.data(), float_samples.data(), nof_prbs, scale_ps, clamp_epi16, full_scale);
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

  // Scaling of the decompressed samples, including the quantizer gain.
  const float scale =
      quantizer(params.data_width).to_float(1) * scaling_table.get_decompression_iq_scaling(params.data_width);

  // The rounding to brain float is faster with the AVX512_BF16 instruction set, which is checked at run time.
  static const bool use_bf16 = cpu_supports_feature(cpu_feature::avx512bf16);

  auto decompress = [&](auto bits) {
    static constexpr unsigned BITS = decltype(bits)::value;
    if (use_bf16) {
      mm512::decompress_prb_quads_bf16<BITS, false>(iq_data.data(), compressed_data.data(), nof_prbs, scale);
    } else {
      mm512::decompress_prb_quads<BITS, false, false>(iq_data.data(), compressed_data.data(), nof_prbs, scale);
    }
  };
  switch (params.data_width) {
    case 9:
      decompress(std::integral_constant<unsigned, 9>());
      break;
    case 14:
      decompress(std::integral_constant<unsigned, 14>());
      break;
    default:
      decompress(std::integral_constant<unsigned, 16>());
      break;
  }
  return true;
}
