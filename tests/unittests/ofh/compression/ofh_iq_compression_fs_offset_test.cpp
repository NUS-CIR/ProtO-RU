// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ocudu/adt/complex.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/compression/compression_factory.h"
#include "ocudu/ofh/compression/compression_properties.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/support/cpu_features.h"
#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <vector>

using namespace ocudu;
using namespace ocudu::ofh;

// An odd number of PRBs exercises the tail handling of the SIMD implementations.
static constexpr unsigned nof_prbs = 11;

// Requesting a SIMD name the CPU lacks silently falls back to generic, so gate each on the factory's check.
static std::vector<std::string> testable_impls()
{
  std::vector<std::string> impls = {"generic"};
#ifdef __x86_64__
  if (cpu_supports_feature(cpu_feature::avx2)) {
    impls.push_back("avx2");
  }
  if (cpu_supports_feature(cpu_feature::avx512f) && cpu_supports_feature(cpu_feature::avx512vl) &&
      cpu_supports_feature(cpu_feature::avx512bw) && cpu_supports_feature(cpu_feature::avx512dq) &&
      cpu_supports_feature(cpu_feature::avx512cd) && cpu_supports_feature(cpu_feature::avx512vbmi)) {
    impls.push_back("avx512");
  }
#endif // __x86_64__
#ifdef __ARM_NEON
  impls.push_back("neon");
#endif // __ARM_NEON
  return impls;
}

// Generates random IQ samples with components uniformly distributed in [-amplitude, amplitude].
static std::vector<cbf16_t> generate_iq(float amplitude)
{
  std::mt19937                          rgen(1234);
  std::uniform_real_distribution<float> dist(-amplitude, amplitude);

  std::vector<cbf16_t> iq(nof_prbs * NOF_SUBCARRIERS_PER_RB);
  for (cbf16_t& sample : iq) {
    sample = to_cbf16(cf_t(dist(rgen), dist(rgen)));
  }
  return iq;
}

// Returns the FS_Offset values to test. Without compression, the full scale can only be moved above the OCUDU one,
// which quickly leaves the IQ data with no resolution.
static std::vector<unsigned> get_fs_offsets(const ru_compression_params& params)
{
  if (params.type == compression_type::none) {
    return {0, 1, 2, 3};
  }
  unsigned native = get_native_fs_offset(params);
  return {0, 1, 5, native, native + 1};
}

// Returns the quantization step of the IQ data relative to the OCUDU IQ full scale.
static float get_quantization_step(const ru_compression_params& params)
{
  unsigned width = (params.type == compression_type::BFP) ? MAX_IQ_WIDTH : params.data_width;
  return 1.0F / static_cast<float>((1U << (width - 1)) - 1);
}

using fs_offset_test_params = std::tuple<std::string, ru_compression_params>;

class ofh_iq_compression_fs_offset_fixture : public ::testing::TestWithParam<fs_offset_test_params>
{
protected:
  std::vector<uint8_t> compress(span<const cbf16_t> iq, std::optional<unsigned> fs_offset) const
  {
    const auto& [impl, params] = GetParam();

    std::vector<uint8_t> buffer(get_compressed_prb_size(params).value() * nof_prbs);
    create_iq_compressor(params.type, ocudulog::fetch_basic_logger("TEST"), 1.0F, fs_offset, impl)
        ->compress(buffer, iq, params);
    return buffer;
  }

  std::vector<cbf16_t> decompress(span<const uint8_t> buffer, std::optional<unsigned> fs_offset) const
  {
    const auto& [impl, params] = GetParam();

    std::vector<cbf16_t> iq(nof_prbs * NOF_SUBCARRIERS_PER_RB);
    EXPECT_TRUE(create_iq_decompressor(params.type, ocudulog::fetch_basic_logger("TEST"), fs_offset, impl)
                    ->decompress(iq, buffer, params));
    return iq;
  }
};

// Fixture for the tests that only apply to BFP.
class ofh_iq_compression_fs_offset_bfp_fixture : public ofh_iq_compression_fs_offset_fixture
{};

// Configuring the FS_Offset of the OCUDU IQ full scale must not change the compressed or decompressed data.
TEST_P(ofh_iq_compression_fs_offset_fixture, native_fs_offset_is_transparent)
{
  const ru_compression_params& params = std::get<1>(GetParam());
  unsigned                     native = get_native_fs_offset(params);
  std::vector<cbf16_t>         iq     = generate_iq(0.5F);

  std::vector<uint8_t> buffer = compress(iq, native);
  ASSERT_EQ(buffer, compress(iq, std::nullopt));
  ASSERT_EQ(decompress(buffer, native), decompress(buffer, std::nullopt));
}

// For BFP, a full scale above the OCUDU one is applied through the exponent, leaving the mantissas untouched.
TEST_P(ofh_iq_compression_fs_offset_bfp_fixture, bfp_exponent_offset)
{
  const ru_compression_params& params = std::get<1>(GetParam());

  unsigned             prb_size  = get_compressed_prb_size(params).value();
  std::vector<cbf16_t> iq        = generate_iq(0.5F);
  std::vector<uint8_t> reference = compress(iq, std::nullopt);
  // Two FS_Offset steps below the OCUDU full scale correspond to one exponent step.
  unsigned             nof_exponent_steps = get_native_fs_offset(params) / 2;
  std::vector<uint8_t> buffer             = compress(iq, 0);

  for (unsigned i_prb = 0; i_prb != nof_prbs; ++i_prb) {
    unsigned offset = i_prb * prb_size;
    ASSERT_EQ(buffer[offset], reference[offset] + nof_exponent_steps) << "PRB " << i_prb;
    ASSERT_TRUE(
        std::equal(buffer.begin() + offset + 1, buffer.begin() + offset + prb_size, reference.begin() + offset + 1))
        << "PRB " << i_prb;
  }
}

// Interpreting the data with the OCUDU full scale must show the configured full scale difference.
TEST_P(ofh_iq_compression_fs_offset_fixture, full_scale_level)
{
  const ru_compression_params& params = std::get<1>(GetParam());
  std::vector<cbf16_t>         iq     = generate_iq(0.5F);

  for (unsigned fs_offset : get_fs_offsets(params)) {
    std::vector<cbf16_t> out_iq = decompress(compress(iq, fs_offset), std::nullopt);
    float gain = std::exp2(0.5F * (static_cast<float>(get_native_fs_offset(params)) - static_cast<float>(fs_offset)));

    for (unsigned i = 0, e = iq.size(); i != e; ++i) {
      cf_t expected = to_cf(iq[i]) * gain;
      cf_t actual   = to_cf(out_iq[i]);
      // Allow for the BFP error relative to the scaled amplitude, and for the quantization error.
      float tolerance = 0.02F * gain + get_quantization_step(params);
      ASSERT_NEAR(actual.real(), expected.real(), tolerance) << "fs_offset=" << fs_offset << " i=" << i;
      ASSERT_NEAR(actual.imag(), expected.imag(), tolerance) << "fs_offset=" << fs_offset << " i=" << i;
    }
  }
}

// Compressing and decompressing with the same FS_Offset must restore the IQ data.
TEST_P(ofh_iq_compression_fs_offset_fixture, round_trip)
{
  const ru_compression_params& params = std::get<1>(GetParam());
  std::vector<cbf16_t>         iq     = generate_iq(0.5F);

  for (unsigned fs_offset : get_fs_offsets(params)) {
    std::vector<cbf16_t> out_iq = decompress(compress(iq, fs_offset), fs_offset);

    // Attenuation applied to the IQ data before quantization, including the half-bit not covered by the exponent.
    float nof_steps   = static_cast<float>(get_native_fs_offset(params)) - static_cast<float>(fs_offset);
    float attenuation = std::exp2(0.5F * (std::min(nof_steps, 0.0F) - 1.0F));

    for (unsigned i = 0, e = iq.size(); i != e; ++i) {
      cf_t expected = to_cf(iq[i]);
      cf_t actual   = to_cf(out_iq[i]);
      // Allow for the BFP error, and for the quantization error that the attenuation increases.
      float tolerance = 0.02F + get_quantization_step(params) / attenuation;
      ASSERT_NEAR(actual.real(), expected.real(), tolerance) << "fs_offset=" << fs_offset << " i=" << i;
      ASSERT_NEAR(actual.imag(), expected.imag(), tolerance) << "fs_offset=" << fs_offset << " i=" << i;
    }
  }
}

// Returns the test name for the given parameters.
static std::string get_test_name(const ::testing::TestParamInfo<fs_offset_test_params>& info)
{
  const auto& [impl, params] = info.param;
  return impl + "_" + to_string(params.type) + "_" + std::to_string(params.data_width);
}

INSTANTIATE_TEST_SUITE_P(ofh_iq_compression_fs_offset,
                         ofh_iq_compression_fs_offset_fixture,
                         ::testing::Combine(::testing::ValuesIn(testable_impls()),
                                            ::testing::Values(ru_compression_params{compression_type::BFP, 8},
                                                              ru_compression_params{compression_type::BFP, 9},
                                                              ru_compression_params{compression_type::BFP, 12},
                                                              ru_compression_params{compression_type::BFP, 14},
                                                              ru_compression_params{compression_type::BFP, 16},
                                                              ru_compression_params{compression_type::none, 9},
                                                              ru_compression_params{compression_type::none, 16})),
                         get_test_name);

INSTANTIATE_TEST_SUITE_P(ofh_iq_compression_fs_offset_bfp,
                         ofh_iq_compression_fs_offset_bfp_fixture,
                         ::testing::Combine(::testing::ValuesIn(testable_impls()),
                                            ::testing::Values(ru_compression_params{compression_type::BFP, 8},
                                                              ru_compression_params{compression_type::BFP, 9},
                                                              ru_compression_params{compression_type::BFP, 12},
                                                              ru_compression_params{compression_type::BFP, 14},
                                                              ru_compression_params{compression_type::BFP, 16})),
                         get_test_name);
