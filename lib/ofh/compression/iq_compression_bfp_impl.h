// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "iq_scaling_table.h"
#include "quantizer.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/compression/iq_compressor.h"
#include "ocudu/ofh/compression/iq_decompressor.h"
#include "ocudu/ran/resource_block.h"
#include <optional>

namespace ocudu {
namespace ofh {

/// Decodes the BFP shared exponent, the low 4 bits of a udCompParam octet (O-RAN.WG4.CUS clause 8.3.3.15).
constexpr uint8_t decode_bfp_exponent(uint8_t comp_param)
{
  return comp_param & 0x0fU;
}

/// Implementation of the Block Floating Point IQ data compression.
class iq_compression_bfp_impl : public iq_compressor, public iq_decompressor
{
public:
  // Constructor.
  explicit iq_compression_bfp_impl(ocudulog::basic_logger& logger_,
                                   float                   iq_scaling_ = 1.0,
                                   std::optional<unsigned> fs_offset   = std::nullopt) :
    scaling_table(compression_type::BFP, iq_scaling_, fs_offset), logger(logger_)
  {
  }

  // See interface for the documentation.
  virtual void
  compress(span<uint8_t> buffer, span<const cbf16_t> iq_data, const ru_compression_params& params) override;

  // See interface for the documentation.
  virtual bool
  decompress(span<cbf16_t> iq_data, span<const uint8_t> compressed_data, const ru_compression_params& params) override;

protected:
  /// Number of quantized samples per resource block.
  static constexpr size_t NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;

  /// \brief Determines the exponent to be used in BFP compression.
  ///
  /// First, it determines the maximum left shift that will still preserve the original value (implemented as a counter
  /// of leading zeros without a sign bit). Then, it determines an exponent given the requested \c data_width.
  ///
  /// \param[in] in_sample  16bit input value for which the exponent is calculated.
  /// \param[in] data_width BFP compression bit width.
  /// \return An exponent to be applied to the input sample to compress it to \c data_width bits.
  static unsigned determine_exponent(uint16_t x, unsigned data_width)
  {
    ocudu_assert(data_width != 0, "Invalid data width");
    ocudu_assert(data_width <= MAX_IQ_WIDTH, "Passed IQ data width exceeds 16 bits");

    unsigned max_shift       = MAX_IQ_WIDTH - data_width;
    unsigned lz_without_sign = max_shift;

    if ((x > 0) && (max_shift > 0)) {
      // TODO: use a wrapper that checks whether this builtin is actually available and provides a fallback option.
      lz_without_sign = __builtin_clz(x) - 16U - 1U;
    }
    int raw_exp = std::min(max_shift, lz_without_sign);
    return std::max(0, static_cast<int>(MAX_IQ_WIDTH - data_width) - raw_exp);
  }

  /// \brief Compresses one resource block using the generic implementation of the algorithm
  /// from O-RAN.WG4.CUS, Annex A.1.2.
  ///
  /// \param[out] c_prb          Compressed PRB.
  /// \param[in] input_quantized Span of quantized IQ samples of a resource block to be compressed.
  /// \param[in] data_width      BFP compression bit width.
  /// \param[in] exponent_offset Offset added to the serialized exponent.
  static void compress_prb_generic(span<uint8_t>       c_prb,
                                   span<const int16_t> input_quantized,
                                   unsigned            data_width,
                                   uint8_t             exponent_offset);

  /// \brief Decompresses one resource block using a generic implementation of the algorithm
  /// from O-RAN.WG4.CUS, Annex A.1.3.
  ///
  /// \param[out] output    Span of decompressed complex samples of a resource block (12 samples).
  /// \param[in] comp_prb   Compressed PRB IQ samples and compression parameter.
  /// \param[in] q          Quantizer object.
  /// \param[in] data_width Bit width of compressed samples.
  /// \param[in] iq_scaling Scaling factor applied to the decompressed samples.
  static void decompress_prb_generic(span<cbf16_t>       output,
                                     span<const uint8_t> comp_prb,
                                     const quantizer&    q,
                                     unsigned            data_width,
                                     float               iq_scaling);

  /// Quantizes complex float samples using the specified bit width.
  ///
  /// \param[out] out       Quantized samples.
  /// \param[in] in         Span of input float samples.
  /// \param[in] data_width BFP compression bit width, used to select the IQ scaling.
  void quantize_input(span<int16_t> out, span<const bf16_t> in, unsigned data_width) const;

  /// \brief Prints to the log the root mean square (RMS) value of the given samples once quantized.
  ///
  /// The samples are only quantized if the debug level of the logger is enabled, so that the implementations that
  /// quantize the samples on the fly do not need to store the quantized values.
  ///
  /// \param[in] in         Brain float samples to quantize.
  /// \param[in] data_width BFP compression bit width, used to select the IQ scaling.
  void log_quantized_input_rms(span<const bf16_t> in, unsigned data_width) const;

  /// IQ data scaling for every IQ bit width.
  const iq_scaling_table scaling_table;

private:
  /// Prints to the log the root mean square (RMS) value of the given quantized samples.
  void log_post_quantization_rms(span<const int16_t> samples) const;

  ocudulog::basic_logger& logger;
};

} // namespace ofh
} // namespace ocudu
