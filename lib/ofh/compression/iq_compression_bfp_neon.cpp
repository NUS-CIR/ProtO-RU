// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_bfp_neon.h"
#include "neon_helpers.h"
#include "packing_utils_neon.h"
#include "quantizer.h"
#include "ocudu/ofh/compression/compression_properties.h"
#include "ocudu/support/math/math_utils.h"

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
  log_quantization_rms(float_samples, params.data_width);

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
  if (!neon::iq_width_unpacking_supported(params.data_width)) {
    return iq_compression_bfp_impl::decompress(iq_data, compressed_data, params);
  }

  // Number of output PRBs.
  unsigned nof_prbs = iq_data.size() / NOF_SUBCARRIERS_PER_RB;

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned comp_prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(compressed_data.size() >= nof_prbs * comp_prb_size,
               "Input does not contain enough bytes to decompress {} PRBs",
               nof_prbs);

  quantizer q_out(Q_BIT_WIDTH);

  // Scaling of the decompressed samples.
  float iq_scaling = scaling_table.get_decompression_iq_scaling(params.data_width);

  unsigned out_idx = 0;
  for (unsigned c_prb_idx = 0; c_prb_idx != nof_prbs; ++c_prb_idx) {
    // Get view over compressed PRB bytes.
    span<const uint8_t> comp_prb_buffer(&compressed_data[c_prb_idx * comp_prb_size], comp_prb_size);

    uint8_t exponent = decode_bfp_exponent(comp_prb_buffer[0]);
    float   scaler   = 1 << exponent;

    // Get view over the bytes following the compression parameter.
    comp_prb_buffer = comp_prb_buffer.last(comp_prb_buffer.size() - sizeof(exponent));

    // Determine array size so that NEON store operation doesn't write the data out of array bounds.
    constexpr size_t neon_size_iqs = 8;
    constexpr size_t arr_size      = divide_ceil(NOF_SAMPLES_PER_PRB, neon_size_iqs) * neon_size_iqs;
    alignas(64) std::array<int16_t, arr_size> unpacked_iq_data;

    // Unpack resource block.
    neon::unpack_prb_big_endian(unpacked_iq_data, comp_prb_buffer, params.data_width);

    span<cbf16_t>       output_span = iq_data.subspan(out_idx, NOF_SUBCARRIERS_PER_RB);
    span<const int16_t> unpacked_span(unpacked_iq_data.data(), NOF_SUBCARRIERS_PER_RB * 2);

    // Convert to complex samples.
    q_out.to_brain_float(output_span, unpacked_span, scaler * iq_scaling);
    out_idx += NOF_SUBCARRIERS_PER_RB;
  }

  return true;
}
