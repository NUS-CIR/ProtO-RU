// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ocudu/ofh/compression/compression_factory.h"
#include "iq_compression_bfp_impl.h"
#include "iq_compression_none_impl.h"
#include "iq_compression_unsupported_impl.h"
#include "iq_compressor_selector.h"
#include "iq_decompressor_selector.h"
#include "ocudu/support/cpu_features.h"
#include "ocudu/support/error_handling.h"

#ifdef __x86_64__
#include "iq_compression_bfp_avx2.h"
#include "iq_compression_bfp_avx512.h"
#include "iq_compression_none_avx2.h"
#include "iq_compression_none_avx512.h"
#endif

#ifdef __ARM_NEON
#include "iq_compression_bfp_neon.h"
#include "iq_compression_none_neon.h"
#endif // __ARM_NEON

using namespace ocudu;
using namespace ofh;

#ifdef __x86_64__
/// Checks whether the CPU supports the instruction sets that the AVX512 implementations are compiled with, see the
/// compile options in CMakeLists.txt.
static bool cpu_supports_avx512_compression()
{
  return cpu_supports_feature(cpu_feature::avx512f) && cpu_supports_feature(cpu_feature::avx512bw) &&
         cpu_supports_feature(cpu_feature::avx512vl) && cpu_supports_feature(cpu_feature::avx512cd) &&
         cpu_supports_feature(cpu_feature::avx512dq) && cpu_supports_feature(cpu_feature::avx512vbmi);
}
#endif // __x86_64__

std::unique_ptr<iq_compressor> ocudu::ofh::create_iq_compressor(compression_type        type,
                                                                ocudulog::basic_logger& logger,
                                                                float                   iq_scaling,
                                                                std::optional<unsigned> fs_offset,
                                                                const std::string&      impl_type)
{
  switch (type) {
    case compression_type::none:
#ifdef __x86_64__
    {
      bool supports_avx2   = cpu_supports_feature(cpu_feature::avx2);
      bool supports_avx512 = cpu_supports_avx512_compression();
      if (((impl_type == "avx512") || (impl_type == "auto")) && supports_avx512) {
        return std::make_unique<iq_compression_none_avx512>(logger, iq_scaling, fs_offset);
      }
      if (((impl_type == "avx2") || (impl_type == "auto")) && supports_avx2) {
        return std::make_unique<iq_compression_none_avx2>(logger, iq_scaling, fs_offset);
      }
    }
#endif
#ifdef __ARM_NEON
      if ((impl_type == "neon") || (impl_type == "auto")) {
        return std::make_unique<iq_compression_none_neon>(logger, iq_scaling, fs_offset);
      }
#endif // __ARM_NEON
      return std::make_unique<iq_compression_none_impl>(logger, iq_scaling, fs_offset);
    case compression_type::BFP:
#ifdef __x86_64__
    {
      bool supports_avx2   = cpu_supports_feature(cpu_feature::avx2);
      bool supports_avx512 = cpu_supports_avx512_compression();
      if (((impl_type == "avx512") || (impl_type == "auto")) && supports_avx512) {
        return std::make_unique<iq_compression_bfp_avx512>(logger, iq_scaling, fs_offset);
      }
      if (((impl_type == "avx2") || (impl_type == "auto")) && supports_avx2) {
        return std::make_unique<iq_compression_bfp_avx2>(logger, iq_scaling, fs_offset);
      }
    }
#endif
#ifdef __ARM_NEON
      if ((impl_type == "neon") || (impl_type == "auto")) {
        return std::make_unique<iq_compression_bfp_neon>(logger, iq_scaling, fs_offset);
      }
#endif // __ARM_NEON
      return std::make_unique<iq_compression_bfp_impl>(logger, iq_scaling, fs_offset);
    case compression_type::block_scaling:
      return std::make_unique<iq_compression_unsupported_impl>();
    case compression_type::mu_law:
      return std::make_unique<iq_compression_unsupported_impl>();
    case compression_type::modulation:
      return std::make_unique<iq_compression_unsupported_impl>();
    case compression_type::bfp_selective:
      return std::make_unique<iq_compression_unsupported_impl>();
    case compression_type::mod_selective:
      return std::make_unique<iq_compression_unsupported_impl>();
    default:
      report_fatal_error("Compression type '{}' is not implemented", to_string(type));
  }
}

std::unique_ptr<iq_decompressor> ocudu::ofh::create_iq_decompressor(compression_type        type,
                                                                    ocudulog::basic_logger& logger,
                                                                    std::optional<unsigned> fs_offset,
                                                                    const std::string&      impl_type)
{
  switch (type) {
    case compression_type::none:
#ifdef __x86_64__
    {
      bool supports_avx2   = cpu_supports_feature(cpu_feature::avx2);
      bool supports_avx512 = cpu_supports_avx512_compression();
      if (((impl_type == "avx512") || (impl_type == "auto")) && supports_avx512) {
        return std::make_unique<iq_compression_none_avx512>(logger, 1.0F, fs_offset);
      }
      if (((impl_type == "avx2") || (impl_type == "auto")) && supports_avx2) {
        return std::make_unique<iq_compression_none_avx2>(logger, 1.0F, fs_offset);
      }
    }
#endif
#ifdef __ARM_NEON
      if ((impl_type == "neon") || (impl_type == "auto")) {
        return std::make_unique<iq_compression_none_neon>(logger, 1.0F, fs_offset);
      }
#endif // __ARM_NEON
      return std::make_unique<iq_compression_none_impl>(logger, 1.0F, fs_offset);
    case compression_type::BFP:
#ifdef __x86_64__
    {
      bool supports_avx2   = cpu_supports_feature(cpu_feature::avx2);
      bool supports_avx512 = cpu_supports_avx512_compression();
      if (((impl_type == "avx512") || (impl_type == "auto")) && supports_avx512) {
        return std::make_unique<iq_compression_bfp_avx512>(logger, 1.0F, fs_offset);
      }
      if (((impl_type == "avx2") || (impl_type == "auto")) && supports_avx2) {
        return std::make_unique<iq_compression_bfp_avx2>(logger, 1.0F, fs_offset);
      }
    }
#endif
#ifdef __ARM_NEON
      if ((impl_type == "neon") || (impl_type == "auto")) {
        return std::make_unique<iq_compression_bfp_neon>(logger, 1.0F, fs_offset);
      }
#endif // __ARM_NEON
      return std::make_unique<iq_compression_bfp_impl>(logger, 1.0F, fs_offset);
    case compression_type::block_scaling:
      return std::make_unique<iq_compression_unsupported_impl>();
    case compression_type::mu_law:
      return std::make_unique<iq_compression_unsupported_impl>();
    case compression_type::modulation:
      return std::make_unique<iq_compression_unsupported_impl>();
    case compression_type::bfp_selective:
      return std::make_unique<iq_compression_unsupported_impl>();
    case compression_type::mod_selective:
      return std::make_unique<iq_compression_unsupported_impl>();
    default:
      report_fatal_error("Compression type '{}' is not implemented", to_string(type));
  }
}

std::unique_ptr<iq_decompressor> ocudu::ofh::create_iq_decompressor_selector(
    std::array<std::unique_ptr<iq_decompressor>, NOF_COMPRESSION_TYPES_SUPPORTED> decompressors)
{
  return std::make_unique<iq_decompressor_selector>(std::move(decompressors));
}

std::unique_ptr<iq_compressor> ocudu::ofh::create_iq_compressor_selector(
    std::array<std::unique_ptr<iq_compressor>, NOF_COMPRESSION_TYPES_SUPPORTED> compressors)
{
  return std::make_unique<iq_compressor_selector>(std::move(compressors));
}
