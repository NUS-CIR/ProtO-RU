// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_bfp_avx2.h"
#include "avx2_helpers.h"
#include "packing_utils_avx2.h"
#include "quantizer.h"
#include "ocudu/ofh/compression/compression_properties.h"

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

  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  span<const bf16_t> float_samples(reinterpret_cast<const bf16_t*>(iq_data.data()), iq_data.size() * 2U);
  log_quantized_input_rms(float_samples, params.data_width);

  // Offset added to the transmitted exponents.
  uint8_t exponent_offset = scaling_table.get_compression_exponent_offset(params.data_width);

  // Scaling applied to the input floats before the conversion to fixed point, including the quantizer gain.
  const float   quantizer_gain = (1 << (Q_BIT_WIDTH - 1)) - 1.0F;
  const float   scale          = quantizer_gain * scaling_table.get_compression_iq_scaling(params.data_width);
  const __m256  scale_ps       = _mm256_set1_ps(scale);
  const __m256i clamp_epi16    = _mm256_set1_epi16(get_bf16_saturation_clamp(scale));

  // Compression algorithm implemented according to O-RAN.WG4.CUS Annex A.1.2. The PRBs are processed in pairs, as two
  // PRBs fill exactly three AVX2 registers.
  unsigned nof_prb_pairs = nof_prbs / 2;
  if (params.data_width == MAX_IQ_WIDTH) {
    mm256::compress_prb_pairs_16b<true>(
        buffer.data(), float_samples.data(), nof_prb_pairs, scale_ps, clamp_epi16, exponent_offset);
  } else {
    mm256::compress_prb_pairs_bfp_9b(
        buffer.data(), float_samples.data(), nof_prb_pairs, scale_ps, clamp_epi16, exponent_offset);
  }

  // Use generic implementation for the remaining resource block.
  if (nof_prbs % 2 != 0) {
    unsigned last_prb = nof_prbs - 1;

    std::array<int16_t, NOF_SAMPLES_PER_PRB> input_quantized;
    quantize_input(
        input_quantized, float_samples.subspan(last_prb * NOF_SAMPLES_PER_PRB, NOF_SAMPLES_PER_PRB), params.data_width);
    compress_prb_generic(
        buffer.subspan(last_prb * prb_size, prb_size), input_quantized, params.data_width, exponent_offset);
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

  // Scaling of the decompressed samples before the exponent, including the quantizer gain, as in
  // decompress_prb_generic().
  const float scale =
      quantizer(Q_BIT_WIDTH).to_float(1) * scaling_table.get_decompression_iq_scaling(params.data_width);

  if (params.data_width == MAX_IQ_WIDTH) {
    mm256::decompress_prbs<16, true>(iq_data.data(), compressed_data.data(), nof_prbs, scale);
  } else {
    mm256::decompress_prbs<9, true>(iq_data.data(), compressed_data.data(), nof_prbs, scale);
  }
  return true;
}
