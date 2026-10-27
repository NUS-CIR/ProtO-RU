// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_none_neon.h"
#include "neon_helpers.h"
#include "packing_utils_neon.h"
#include "quantizer.h"
#include "ocudu/ofh/compression/compression_properties.h"

using namespace ocudu;
using namespace ofh;

void iq_compression_none_neon::compress(span<uint8_t>                buffer,
                                        span<const cbf16_t>          iq_data,
                                        const ru_compression_params& params)
{
  // Use generic implementation if NEON utils don't support requested bit width.
  if (!neon::iq_width_packing_supported(params.data_width)) {
    iq_compression_none_impl::compress(buffer, iq_data, params);
    return;
  }

  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  span<const bf16_t> float_samples(reinterpret_cast<const bf16_t*>(iq_data.data()), iq_data.size() * 2U);
  log_quantization_rms(float_samples, params.data_width);

  // Scaling applied to the input floats before the conversion to fixed point, including the quantizer gain.
  const float full_scale = static_cast<float>((1 << (params.data_width - 1)) - 1);
  const float scale      = full_scale * scaling_table.get_compression_iq_scaling(params.data_width);

  neon::compress_prbs<false>(buffer.data(), float_samples.data(), nof_prbs, params.data_width, scale, 0);
}

bool iq_compression_none_neon::decompress(span<cbf16_t>                iq_data,
                                          span<const uint8_t>          compressed_data,
                                          const ru_compression_params& params)
{
  // Use generic implementation if NEON utils don't support requested bit width.
  if (!neon::iq_width_unpacking_supported(params.data_width)) {
    return iq_compression_none_impl::decompress(iq_data, compressed_data, params);
  }

  // Number of output PRBs.
  unsigned nof_prbs = iq_data.size() / NOF_SUBCARRIERS_PER_RB;

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned comp_prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(compressed_data.size() >= nof_prbs * comp_prb_size,
               "Input does not contain enough bytes to decompress {} PRBs",
               nof_prbs);

  quantizer q_out(params.data_width);

  std::array<int16_t, MAX_NOF_SUBCARRIERS * 2> unpacked_iq_data;

  unsigned idx = 0;
  for (unsigned rb = 0; rb != nof_prbs; ++rb) {
    span<const uint8_t> comp_prb_buffer(&compressed_data[rb * comp_prb_size], comp_prb_size);

    // Unpack resource block.
    span<int16_t> unpacked_prb_span(&unpacked_iq_data[idx], NOF_SUBCARRIERS_PER_RB * 2);
    neon::unpack_prb_big_endian(unpacked_prb_span, comp_prb_buffer, params.data_width);

    idx += (NOF_SUBCARRIERS_PER_RB * 2);
  }

  span<int16_t> unpacked_iq_int16_span(unpacked_iq_data.data(), iq_data.size() * 2);
  // Convert to complex brain float samples.
  q_out.to_brain_float(iq_data, unpacked_iq_int16_span, scaling_table.get_decompression_iq_scaling(params.data_width));

  return true;
}
