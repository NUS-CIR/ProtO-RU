// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_none_impl.h"
#include "packing_utils_generic.h"
#include "quantizer.h"
#include "ocudu/ocuduvec/dot_prod.h"
#include "ocudu/ofh/compression/compression_properties.h"
#include "ocudu/support/units.h"

using namespace ocudu;
using namespace ofh;

void iq_compression_none_impl::compress(span<uint8_t>                buffer,
                                        span<const cbf16_t>          iq_data,
                                        const ru_compression_params& params)
{
  // Quantizer object.
  quantizer q(params.data_width);

  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  span<const bf16_t> float_samples(reinterpret_cast<const bf16_t*>(iq_data.data()), iq_data.size() * 2);

  // Scaling factor applied to IQ data prior to quantization.
  float iq_scaling = scaling_table.get_compression_iq_scaling(params.data_width);

  unsigned in_sample_idx = 0;
  for (unsigned rb = 0; rb != nof_prbs; ++rb) {
    // Auxiliary buffer used for brain float to int16_t conversion.
    std::array<int16_t, NOF_SUBCARRIERS_PER_RB * 2> conv_buffer;

    // Scale input IQ data to the range [-1: +1) and convert it to int16_t.
    q.to_fixed_point(conv_buffer, float_samples.subspan(in_sample_idx, NOF_SUBCARRIERS_PER_RB * 2), iq_scaling);

    // View over output bytes corresponding to the current PRB.
    span<uint8_t> out_compr_prb(&buffer[rb * prb_size], prb_size);

    bit_buffer bit_buf = bit_buffer::from_bytes(out_compr_prb);
    pack_bytes(bit_buf, conv_buffer, params.data_width);

    in_sample_idx += (NOF_SUBCARRIERS_PER_RB * 2);
  }
}

bool iq_compression_none_impl::decompress(span<cbf16_t>                output,
                                          span<const uint8_t>          input,
                                          const ru_compression_params& params)
{
  // The width arrives from the wire in the dynamic case, and the quantizer has no gain to divide by below this.
  if (OCUDU_UNLIKELY(params.data_width < MIN_IQ_WIDTH)) {
    return false;
  }

  // Quantizer object.
  quantizer q(params.data_width);

  // Scaling of the decompressed samples, including the quantizer gain.
  float iq_scaling = q.to_float(1) * scaling_table.get_decompression_iq_scaling(params.data_width);

  // Number of output PRBs.
  unsigned nof_prbs = output.size() / NOF_SUBCARRIERS_PER_RB;

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(
      input.size() >= nof_prbs * prb_size, "Input does not contain enough bytes to decompress {} PRBs", nof_prbs);

  unsigned out_idx = 0;
  for (unsigned c_prb_idx = 0, e = nof_prbs; c_prb_idx != e; ++c_prb_idx) {
    span<const uint8_t> c_prb(&input[c_prb_idx * prb_size], prb_size);

    bit_buffer_reader bits_reader = bit_buffer_reader::from_bytes(c_prb);
    for (unsigned i = 0, read_pos = 0; i != NOF_SUBCARRIERS_PER_RB; ++i) {
      int16_t re = q.sign_extend(unpack_bits(bits_reader, read_pos, params.data_width));
      int16_t im = q.sign_extend(unpack_bits(bits_reader, read_pos + params.data_width, params.data_width));
      read_pos += (params.data_width * 2);
      output[out_idx++] = {re * iq_scaling, im * iq_scaling};
    }
  }

  return true;
}

void iq_compression_none_impl::log_quantization_rms(span<const bf16_t> in, unsigned data_width)
{
  if (OCUDU_LIKELY(!logger.debug.enabled())) {
    return;
  }

  static constexpr size_t MAX_NOF_SAMPLES = 2 * NOF_SUBCARRIERS_PER_RB * MAX_NOF_PRBS;
  ocudu_assert(in.size() <= MAX_NOF_SAMPLES, "Number of samples exceeds the maximum number of PRBs");

  std::array<int16_t, MAX_NOF_SAMPLES> quantized;
  span<int16_t>                        quantized_span(quantized.data(), in.size());
  quantizer(data_width).to_fixed_point(quantized_span, in, scaling_table.get_compression_iq_scaling(data_width));
  log_post_quantization_rms(quantized_span);
}

void iq_compression_none_impl::log_post_quantization_rms(span<const int16_t> samples)
{
  if (OCUDU_UNLIKELY(logger.debug.enabled() && !samples.empty())) {
    // Calculate and print RMS of quantized samples.
    float sum_squares = ocuduvec::dot_prod(samples, samples, 0.0F);
    float rms         = std::sqrt(sum_squares / samples.size());
    if (std::isnormal(rms)) {
      logger.debug("Quantized IQ samples RMS value of '{}'", rms);
    }
  }
}
