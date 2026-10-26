// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "iq_scaling_table.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/compression/iq_compressor.h"
#include "ocudu/ofh/compression/iq_decompressor.h"
#include <optional>

namespace ocudu {
namespace ofh {

/// Implementation of the IQ data compression by quantizing floating point numbers without compression.
class iq_compression_none_impl : public iq_compressor, public iq_decompressor
{
public:
  // Constructor.
  explicit iq_compression_none_impl(ocudulog::basic_logger& logger_,
                                    float                   iq_scaling_ = 1.0,
                                    std::optional<unsigned> fs_offset   = std::nullopt) :
    logger(logger_), scaling_table(compression_type::none, iq_scaling_, fs_offset)
  {
  }

  // See interface for the documentation.
  virtual void
  compress(span<uint8_t> buffer, span<const cbf16_t> iq_data, const ru_compression_params& params) override;

  // See interface for the documentation.
  virtual bool
  decompress(span<cbf16_t> iq_data, span<const uint8_t> compressed_data, const ru_compression_params& params) override;

protected:
  /// \brief Prints to the log the root mean square (RMS) value of the given samples.
  ///
  /// \param[in] samples - Quantized samples.
  void log_post_quantization_rms(span<const int16_t> samples);

  /// \brief Prints to the log the root mean square (RMS) value of the given samples once quantized.
  ///
  /// The samples are only quantized if the debug level of the logger is enabled, so that implementations that quantize
  /// the samples on the fly do not need to store the quantized values.
  ///
  /// \param[in] in         Brain float samples to quantize.
  /// \param[in] data_width Bit width of the quantized samples.
  void log_quantization_rms(span<const bf16_t> in, unsigned data_width);

  ocudulog::basic_logger& logger;
  /// IQ data scaling for every IQ bit width.
  const iq_scaling_table scaling_table;
};

} // namespace ofh
} // namespace ocudu
