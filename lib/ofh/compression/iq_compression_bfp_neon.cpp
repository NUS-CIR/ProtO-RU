// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_bfp_neon.h"
#include "neon_helpers.h"
#include "packing_utils_neon.h"
#include "quantizer.h"
#include "ocudu/ofh/compression/compression_properties.h"

using namespace ocudu;
using namespace ofh;

void iq_compression_bfp_neon::compress(span<uint8_t>                buffer,
                                       span<const cbf16_t>          iq_data,
                                       const ru_compression_params& params)
{
  // Use generic implementation if NEON utils don't support requested bit width.
  if (!neon::iq_width_packing_supported(params.data_width)) {
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
  const float quantizer_gain = (1 << (Q_BIT_WIDTH - 1)) - 1.0F;
  const float scale          = quantizer_gain * scaling_table.get_compression_iq_scaling(params.data_width);

  neon::compress_prbs<true>(buffer.data(), float_samples.data(), nof_prbs, params.data_width, scale, exponent_offset);
}

bool iq_compression_bfp_neon::decompress(span<cbf16_t>                iq_data,
                                         span<const uint8_t>          compressed_data,
                                         const ru_compression_params& params)
{
  // Use generic implementation if NEON utils don't support requested bit width.
  if (!neon::iq_width_packing_supported(params.data_width)) {
    return iq_compression_bfp_impl::decompress(iq_data, compressed_data, params);
  }

  // Number of output PRBs.
  unsigned nof_prbs = iq_data.size() / NOF_SUBCARRIERS_PER_RB;

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned comp_prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(compressed_data.size() >= nof_prbs * comp_prb_size,
               "Input does not contain enough bytes to decompress {} PRBs",
               nof_prbs);

  // Scaling of the decompressed samples before the exponent, including the quantizer gain.
  float scale = quantizer(Q_BIT_WIDTH).to_float(1) * scaling_table.get_decompression_iq_scaling(params.data_width);

  neon::decompress_prbs<true>(iq_data.data(), compressed_data.data(), nof_prbs, params.data_width, scale);
  return true;
}
