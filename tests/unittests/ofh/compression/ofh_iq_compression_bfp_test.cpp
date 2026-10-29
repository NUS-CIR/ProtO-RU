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
#include <array>
#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

using namespace ocudu;
using namespace ocudu::ofh;

// Fills the mantissa bytes of a PRB where the test only needs the payload to be non-zero.
static constexpr uint8_t mantissa_fill = 0xa5;

// Requesting a SIMD name the CPU lacks silently falls back to generic, so gate each on the factory's check.
static std::vector<const char*> testable_impls()
{
  std::vector<const char*> impls = {"generic"};
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

// Decompresses one PRB built from a uniform mantissa fill.
static std::array<cbf16_t, NOF_SUBCARRIERS_PER_RB>
decompress_one(uint8_t comp_param, uint8_t mantissa, const ru_compression_params& params, const char* impl)
{
  auto decompressor = create_iq_decompressor(params.type, ocudulog::fetch_basic_logger("TEST"), std::nullopt, impl);
  std::vector<uint8_t> prb(get_compressed_prb_size(params).value(), mantissa);
  prb[0] = comp_param;

  std::array<cbf16_t, NOF_SUBCARRIERS_PER_RB> out = {};
  EXPECT_TRUE(decompressor->decompress(out, prb, params));
  return out;
}

// Decompresses a width-16 PRB from explicit big-endian mantissas; a uniform fill can not reach 0x7fff.
static std::array<cbf16_t, NOF_SUBCARRIERS_PER_RB>
decompress_width16(uint8_t                                                comp_param,
                   const std::array<int16_t, NOF_SUBCARRIERS_PER_RB * 2>& mantissas,
                   const char*                                            impl)
{
  const ru_compression_params params{compression_type::BFP, 16};
  std::vector<uint8_t>        prb;
  prb.reserve(get_compressed_prb_size(params).value());
  prb.push_back(comp_param);
  for (int16_t m : mantissas) {
    auto bits = static_cast<uint16_t>(m);
    prb.push_back(static_cast<uint8_t>(bits >> 8U));
    prb.push_back(static_cast<uint8_t>(bits & 0xffU));
  }
  auto decompressor = create_iq_decompressor(params.type, ocudulog::fetch_basic_logger("TEST"), std::nullopt, impl);
  std::array<cbf16_t, NOF_SUBCARRIERS_PER_RB> out = {};
  EXPECT_TRUE(decompressor->decompress(out, prb, params));
  return out;
}

static bool is_all_zero(const std::array<cbf16_t, NOF_SUBCARRIERS_PER_RB>& iq)
{
  return std::all_of(iq.begin(), iq.end(), [](cbf16_t s) { return to_cf(s) == cf_t{}; });
}

// Returns the value expected after the BFP decompression: mantissa scaled by 2^exponent, normalised by the
// quantizer gain.
static float get_expected_bfp_decompr_value(int mantissa, unsigned exponent)
{
  return static_cast<float>(mantissa) * static_cast<float>(uint32_t{1} << exponent) / 32767.0F;
}

// Checks one component. A relative bound alone, because an absolute one large enough for the biggest sample
// also accepts zero and a flipped sign for the smallest ones.
static void expect_component_near(float actual, float expected)
{
  if (expected == 0.0F) {
    EXPECT_EQ(actual, 0.0F);
    return;
  }

  EXPECT_EQ(std::signbit(actual), std::signbit(expected));
  EXPECT_NEAR(actual, expected, std::abs(expected) * 0x1p-7F);
}

// Checks every I and Q component against expected.
static void expect_all_near(const std::array<cbf16_t, NOF_SUBCARRIERS_PER_RB>& out, float expected)
{
  for (const cbf16_t sample : out) {
    expect_component_near(to_cf(sample).real(), expected);
    expect_component_near(to_cf(sample).imag(), expected);
  }
}

namespace {

TEST(ofh_iq_compression_bfp_decompressor, backends_under_test)
{
  // Logs the backends the other tests exercise here, so a silent fallback to generic stays visible.
  const std::vector<const char*> impls = testable_impls();
  ASSERT_FALSE(impls.empty());
  for (const char* impl : impls) {
    GTEST_LOG_(INFO) << "BFP decompressor backend under test: " << impl;
  }
}

TEST(ofh_iq_compression_bfp_decompressor, reserved_nibble_does_not_change_output)
{
  const ru_compression_params params{compression_type::BFP, 9};
  for (const char* impl : testable_impls()) {
    SCOPED_TRACE(impl);
    // Both compression parameters hold exponent 3 and differ only in the reserved nibble. The first output
    // must not be all zeros, or the equality would also hold for a decompressor that writes nothing.
    std::array<cbf16_t, NOF_SUBCARRIERS_PER_RB> out = decompress_one(0x03, mantissa_fill, params, impl);
    EXPECT_FALSE(is_all_zero(out));
    EXPECT_EQ(out, decompress_one(0xf3, mantissa_fill, params, impl));
  }
}

TEST(ofh_iq_compression_bfp_decompressor, reserved_nibble_ignored_for_every_param_byte)
{
  // An unmasked byte would drive an out-of-range shift, so every value must match its low nibble.
  const ru_compression_params params{compression_type::BFP, 9};
  for (const char* impl : testable_impls()) {
    SCOPED_TRACE(impl);
    for (unsigned byte = 0; byte != 256; ++byte) {
      SCOPED_TRACE(byte);
      EXPECT_EQ(decompress_one(static_cast<uint8_t>(byte), mantissa_fill, params, impl),
                decompress_one(static_cast<uint8_t>(byte & 0x0fU), mantissa_fill, params, impl));
    }
  }
}

TEST(ofh_iq_compression_bfp_decompressor, max_exponent_keeps_sign_and_magnitude)
{
  // Exponent 15 is the boundary the fix targets: an int16_t scaler would flip -1 to +1 here.
  const ru_compression_params params{compression_type::BFP, 9};
  for (const char* impl : testable_impls()) {
    SCOPED_TRACE(impl);
    expect_all_near(decompress_one(0x0f, 0xff, params, impl), get_expected_bfp_decompr_value(-1, 15));
  }
}

TEST(ofh_iq_compression_bfp_decompressor, exponent_and_mantissa_matrix)
{
  // Exponent 15 with INT16_MIN or INT16_MAX is the worst-case product the widened int32 must hold.
  const std::array<int16_t, 5> mantissas = {0, 1, -1, INT16_MAX, INT16_MIN};
  const std::array<uint8_t, 4> exponents = {0, 1, 14, 15};
  for (const char* impl : testable_impls()) {
    SCOPED_TRACE(impl);
    for (uint8_t exponent : exponents) {
      SCOPED_TRACE(exponent);
      for (int16_t mantissa : mantissas) {
        SCOPED_TRACE(mantissa);
        std::array<int16_t, NOF_SUBCARRIERS_PER_RB * 2> payload;
        payload.fill(mantissa);
        expect_all_near(decompress_width16(exponent, payload, impl),
                        get_expected_bfp_decompr_value(mantissa, exponent));
      }
    }
  }
}

TEST(ofh_iq_compression_bfp_decompressor, mixed_iq_components_decode_per_lane)
{
  // Every component has its own magnitude and sign, so a lane swap changes the result. A repeating pattern
  // would hide a swap between two equal components.
  std::array<int16_t, NOF_SUBCARRIERS_PER_RB * 2> payload;
  for (unsigned i = 0; i != payload.size(); ++i) {
    int value  = 1024 + 256 * static_cast<int>(i);
    payload[i] = static_cast<int16_t>((i % 2U == 0U) ? value : -value);
  }
  for (const char* impl : testable_impls()) {
    SCOPED_TRACE(impl);
    std::array<cbf16_t, NOF_SUBCARRIERS_PER_RB> out = decompress_width16(0x00, payload, impl);
    for (unsigned k = 0; k != out.size(); ++k) {
      SCOPED_TRACE(k);
      expect_component_near(to_cf(out[k]).real(), get_expected_bfp_decompr_value(payload[2 * k], 0));
      expect_component_near(to_cf(out[k]).imag(), get_expected_bfp_decompr_value(payload[2 * k + 1], 0));
    }
  }
}

} // namespace
