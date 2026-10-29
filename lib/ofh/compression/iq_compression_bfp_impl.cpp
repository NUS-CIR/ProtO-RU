// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "iq_compression_bfp_impl.h"
#include "packing_utils_generic.h"
#include "ocudu/ocuduvec/dot_prod.h"
#include "ocudu/ofh/compression/compression_properties.h"
#include "ocudu/support/units.h"

using namespace ocudu;
using namespace ofh;

void iq_compression_bfp_impl::quantize_input(span<int16_t> out, span<const bf16_t> in, unsigned data_width) const
{
  ocudu_assert(in.size() == out.size(), "Input and output spans must have the same size");

  // Quantizer object.
  quantizer q(Q_BIT_WIDTH);

  // Convert input to int16_t representation.
  q.to_fixed_point(out, in, scaling_table.get_compression_iq_scaling(data_width));

  log_post_quantization_rms(out);
}

void iq_compression_bfp_impl::log_quantized_input_rms(span<const bf16_t> in, unsigned data_width) const
{
  if (OCUDU_LIKELY(!logger.debug.enabled())) {
    return;
  }

  ocudu_assert(in.size() <= NOF_SAMPLES_PER_PRB * MAX_NOF_PRBS, "Number of samples exceeds the maximum number of PRBs");

  std::array<int16_t, NOF_SAMPLES_PER_PRB * MAX_NOF_PRBS> quantized;
  span<int16_t>                                           quantized_span(quantized.data(), in.size());
  quantizer(Q_BIT_WIDTH).to_fixed_point(quantized_span, in, scaling_table.get_compression_iq_scaling(data_width));
  log_post_quantization_rms(quantized_span);
}

void iq_compression_bfp_impl::log_post_quantization_rms(span<const int16_t> samples) const
{
  if (OCUDU_LIKELY(!logger.debug.enabled() || samples.empty())) {
    return;
  }

  // Calculate and print RMS of quantized samples.
  float sum_squares = ocuduvec::dot_prod(samples, samples, 0.0F);
  float rms         = std::sqrt(sum_squares / samples.size());
  if (std::isnormal(rms)) {
    logger.debug("Quantized IQ samples RMS value of '{}'", rms);
  }
}

void iq_compression_bfp_impl::compress_prb_generic(span<uint8_t>       comp_prb_buffer,
                                                   span<const int16_t> input_quantized,
                                                   unsigned            data_width,
                                                   uint8_t             exponent_offset)
{
  // Determine maximum absolute value and the exponent.
  const auto* start_it = input_quantized.begin();
  const auto* max_it   = std::max_element(start_it, start_it + NOF_SAMPLES_PER_PRB);
  const auto* min_it   = std::min_element(start_it, start_it + NOF_SAMPLES_PER_PRB);
  // O-RAN.WG4.CUS, Annex A.1.2: the most significant bit of a negative value can be one higher. As the minimum does not
  // exceed the maximum, the result is not negative.
  unsigned max_abs = std::max(static_cast<int>(*max_it), std::abs(*min_it) - 1);

  uint8_t exponent = determine_exponent(max_abs, data_width);

  // Auxiliary arrays to store compressed samples before packing.
  std::array<int16_t, NOF_SAMPLES_PER_PRB> compressed_samples;

  // Compress data.
  for (unsigned i = 0; i != NOF_SAMPLES_PER_PRB; ++i) {
    compressed_samples[i] = input_quantized[i] >> exponent;
  }

  // Save exponent.
  uint8_t comp_param = exponent + exponent_offset;
  std::memcpy(comp_prb_buffer.data(), &comp_param, sizeof(comp_param));
  comp_prb_buffer = comp_prb_buffer.last(comp_prb_buffer.size() - sizeof(exponent));

  bit_buffer buffer = bit_buffer::from_bytes(comp_prb_buffer);
  pack_bytes(buffer, compressed_samples, data_width);
}

void iq_compression_bfp_impl::compress(span<uint8_t>                buffer,
                                       span<const cbf16_t>          iq_data,
                                       const ru_compression_params& params)
{
  // Number of input PRBs.
  unsigned nof_prbs = (iq_data.size() / NOF_SUBCARRIERS_PER_RB);

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(buffer.size() >= prb_size * nof_prbs, "Output buffer doesn't have enough space to decompress PRBs");

  // Auxiliary arrays used for float to fixed point conversion of the input data.
  std::array<int16_t, NOF_SAMPLES_PER_PRB * MAX_NOF_PRBS> input_quantized;

  span<const bf16_t> float_samples_span(reinterpret_cast<const bf16_t*>(iq_data.data()), iq_data.size() * 2U);
  span<int16_t>      input_quantized_span(input_quantized.data(), float_samples_span.size());
  // Performs conversion of input brain float values to signed 16-bit integers.
  quantize_input(input_quantized_span, float_samples_span, params.data_width);

  uint8_t exponent_offset = scaling_table.get_compression_exponent_offset(params.data_width);
  for (unsigned i = 0; i != nof_prbs; ++i) {
    const auto* in_start_it = input_quantized.begin() + NOF_SAMPLES_PER_PRB * i;
    auto*       out_it      = &buffer[i * prb_size];
    // Compress one resource block.
    compress_prb_generic({out_it, prb_size}, {in_start_it, NOF_SAMPLES_PER_PRB}, params.data_width, exponent_offset);
  }
}

void iq_compression_bfp_impl::decompress_prb_generic(span<cbf16_t>       output,
                                                     span<const uint8_t> comp_prb,
                                                     const quantizer&    q_in,
                                                     unsigned            data_width,
                                                     float               iq_scaling)
{
  // Quantizer.
  quantizer q_out(Q_BIT_WIDTH);

  uint8_t exponent = decode_bfp_exponent(comp_prb[0]);
  // A masked exponent reaches 15, so the shift result (up to 32768) does not fit int16_t.
  int32_t scaler = 1 << exponent;
  // Scaling of the decompressed samples, including the quantizer gain.
  float prb_scaling = q_out.to_float(scaler) * iq_scaling;

  comp_prb             = comp_prb.last(comp_prb.size() - sizeof(exponent));
  auto bit_buff_reader = bit_buffer_reader::from_bytes(comp_prb);

  for (unsigned i = 0, read_pos = 0; i != NOF_SUBCARRIERS_PER_RB; ++i) {
    int16_t re = q_in.sign_extend(unpack_bits(bit_buff_reader, read_pos, data_width));
    int16_t im = q_in.sign_extend(unpack_bits(bit_buff_reader, read_pos + data_width, data_width));
    read_pos += (data_width * 2);

    float scaled_re = re * prb_scaling;
    float scaled_im = im * prb_scaling;
    output[i]       = {scaled_re, scaled_im};
  }
}

bool iq_compression_bfp_impl::decompress(span<cbf16_t>                iq_data,
                                         span<const uint8_t>          compressed_data,
                                         const ru_compression_params& params)
{
  // Quantizer.
  quantizer q_in(params.data_width);

  // Number of output PRBs.
  unsigned nof_prbs = iq_data.size() / NOF_SUBCARRIERS_PER_RB;

  // Size in bytes of one compressed PRB using the given compression parameters.
  unsigned prb_size = get_compressed_prb_size(params).value();

  ocudu_assert(compressed_data.size() >= nof_prbs * prb_size,
               "Input does not contain enough bytes to decompress {} PRBs",
               nof_prbs);

  unsigned out_idx = 0;
  for (unsigned c_prb_idx = 0; c_prb_idx != nof_prbs; ++c_prb_idx) {
    span<const uint8_t> comp_prb(&compressed_data[c_prb_idx * prb_size], prb_size);

    span<cbf16_t> out_prb_samples = iq_data.subspan(out_idx, NOF_SUBCARRIERS_PER_RB);
    // Decompress resource block.
    decompress_prb_generic(out_prb_samples,
                           comp_prb,
                           q_in,
                           params.data_width,
                           scaling_table.get_decompression_iq_scaling(params.data_width));
    out_idx += NOF_SUBCARRIERS_PER_RB;
  }

  return true;
}
