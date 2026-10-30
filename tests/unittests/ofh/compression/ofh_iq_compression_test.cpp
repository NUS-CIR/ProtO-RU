// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "compare_sequences.h"
#include "ofh_compression_test_helpers.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/compression/compression_factory.h"
#include "ocudu/support/cpu_features.h"
#include <array>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include <random>

using namespace ocudu;
using namespace ocudu::ofh;
using namespace ocudu::ofh::testing;

/// Numbers of resource blocks compressed in one call. They reach every remainder of the SIMD batches of 2 and 4 PRBs.
static const std::vector<unsigned> nof_prbs_list = {1, 2, 3, 25, 273, 275};

/// Requesting a SIMD implementation not supported by the CPU silently falls back to the generic one, so only list the
/// ones the compression factory selects.
static std::vector<std::string> get_testable_implementations()
{
  std::vector<std::string> implementations = {"generic"};
#ifdef __x86_64__
  if (cpu_supports_feature(cpu_feature::avx2)) {
    implementations.emplace_back("avx2");
  }
  if (cpu_supports_feature(cpu_feature::avx512f) && cpu_supports_feature(cpu_feature::avx512vl) &&
      cpu_supports_feature(cpu_feature::avx512bw) && cpu_supports_feature(cpu_feature::avx512dq) &&
      cpu_supports_feature(cpu_feature::avx512cd) && cpu_supports_feature(cpu_feature::avx512vbmi)) {
    implementations.emplace_back("avx512");
  }
#endif // __x86_64__
#ifdef __ARM_NEON
  implementations.emplace_back("neon");
#endif // __ARM_NEON
  return implementations;
}

/// Compresses the IQ data with the given implementation.
static std::vector<uint8_t> compress(span<const cbf16_t>          iq_data,
                                     const ru_compression_params& params,
                                     float                        iq_scaling,
                                     std::optional<unsigned>      fs_offset,
                                     const std::string&           implementation)
{
  std::unique_ptr<iq_compressor> compressor = create_iq_compressor(
      params.type, ocudulog::fetch_basic_logger("TEST", false), iq_scaling, fs_offset, implementation);

  // The compressors process at most the PRBs of a carrier in one call.
  unsigned prb_size = get_compressed_prb_size_reference(params);
  unsigned nof_prbs = iq_data.size() / NOF_SUBCARRIERS_PER_RB;

  std::vector<uint8_t> compressed(nof_prbs * prb_size);
  for (unsigned i_prb = 0; i_prb < nof_prbs; i_prb += MAX_NOF_PRBS) {
    unsigned prbs_per_call = std::min<unsigned>(MAX_NOF_PRBS, nof_prbs - i_prb);
    compressor->compress(span<uint8_t>(compressed).subspan(i_prb * prb_size, prbs_per_call * prb_size),
                         iq_data.subspan(i_prb * NOF_SUBCARRIERS_PER_RB, prbs_per_call * NOF_SUBCARRIERS_PER_RB),
                         params);
  }
  return compressed;
}

/// Decompresses the compressed resource blocks with the given implementation.
static std::vector<cbf16_t> decompress(span<const uint8_t>          compressed,
                                       const ru_compression_params& params,
                                       std::optional<unsigned>      fs_offset,
                                       const std::string&           implementation)
{
  std::unique_ptr<iq_decompressor> decompressor =
      create_iq_decompressor(params.type, ocudulog::fetch_basic_logger("TEST", false), fs_offset, implementation);

  // The decompressors process at most the PRBs of a carrier in one call.
  unsigned prb_size = get_compressed_prb_size_reference(params);
  unsigned nof_prbs = compressed.size() / prb_size;

  std::vector<cbf16_t> iq_data(nof_prbs * NOF_SUBCARRIERS_PER_RB);
  for (unsigned i_prb = 0; i_prb < nof_prbs; i_prb += MAX_NOF_PRBS) {
    unsigned prbs_per_call = std::min<unsigned>(MAX_NOF_PRBS, nof_prbs - i_prb);

    EXPECT_TRUE(decompressor->decompress(
        span<cbf16_t>(iq_data).subspan(i_prb * NOF_SUBCARRIERS_PER_RB, prbs_per_call * NOF_SUBCARRIERS_PER_RB),
        compressed.subspan(i_prb * prb_size, prbs_per_call * prb_size),
        params));
  }
  return iq_data;
}

/// Compares the compressed resource blocks with the expected ones, and reports the first mismatching one.
static error_type<std::string> compare_compressed_prbs(span<const uint8_t>          actual,
                                                       span<const uint8_t>          expected,
                                                       const ru_compression_params& params,
                                                       const std::string&           context)
{
  if (actual.size() != expected.size()) {
    return make_unexpected(
        fmt::format("{}: size mismatch, actual={} expected={}", context, actual.size(), expected.size()));
  }

  unsigned prb_size = get_compressed_prb_size_reference(params);
  for (unsigned i_prb = 0, nof_prbs = expected.size() / prb_size; i_prb != nof_prbs; ++i_prb) {
    span<const uint8_t> actual_prb   = actual.subspan(i_prb * prb_size, prb_size);
    span<const uint8_t> expected_prb = expected.subspan(i_prb * prb_size, prb_size);

    error_type<std::string> result = compare_sequences(actual_prb, expected_prb);
    if (!result.has_value()) {
      return make_unexpected(fmt::format("{}: PRB {} mismatch\n{}", context, i_prb, result.error()));
    }
  }
  return default_success_t{};
}

/// \brief Checks that the decompressed IQ data match the expected ones.
///
/// The decompressor rounds a single precision product to brain float, so the tolerance is half a brain float step plus
/// the single precision rounding. A zero is exact.
static void check_decompressed_iq(span<const cbf16_t> actual, span<const cf_t> expected, const std::string& context)
{
  static constexpr float relative_tolerance = 1.0F / 256.0F;

  ASSERT_EQ(actual.size(), expected.size()) << context;
  for (unsigned i = 0, i_end = expected.size(); i != i_end; ++i) {
    cf_t value = to_cf(actual[i]);
    ASSERT_LE(std::abs(value.real() - expected[i].real()), std::abs(expected[i].real()) * relative_tolerance)
        << fmt::format("{}: RE {} (PRB {}), actual={}, expected={}",
                       context,
                       i,
                       i / NOF_SUBCARRIERS_PER_RB,
                       value.real(),
                       expected[i].real());
    ASSERT_LE(std::abs(value.imag() - expected[i].imag()), std::abs(expected[i].imag()) * relative_tolerance)
        << fmt::format("{}: RE {} (PRB {}), actual={}, expected={}",
                       context,
                       i,
                       i / NOF_SUBCARRIERS_PER_RB,
                       value.imag(),
                       expected[i].imag());
  }
}

/// \brief Generates random IQ data, in the way of the former MATLAB test vectors.
///
/// The I and Q components are uniformly distributed in [-1, +1], except for two random resource blocks set to +/-0.01,
/// which results in small BFP exponents.
static std::vector<cbf16_t> generate_random_iq_data(std::mt19937& rgen, unsigned nof_prbs)
{
  std::uniform_real_distribution<float>   uniform_dist(-1.0F, 1.0F);
  std::bernoulli_distribution             sign_dist(0.5);
  std::uniform_int_distribution<unsigned> prb_dist(0, nof_prbs - 1);

  std::vector<cbf16_t> iq_data(nof_prbs * NOF_SUBCARRIERS_PER_RB);
  for (cbf16_t& sample : iq_data) {
    sample = to_cbf16(cf_t(uniform_dist(rgen), uniform_dist(rgen)));
  }

  for (unsigned i = 0; i != 2; ++i) {
    unsigned i_prb = prb_dist(rgen);
    for (unsigned i_re = 0; i_re != NOF_SUBCARRIERS_PER_RB; ++i_re) {
      float re = sign_dist(rgen) ? 0.01F : -0.01F;
      float im = sign_dist(rgen) ? 0.01F : -0.01F;

      iq_data[i_prb * NOF_SUBCARRIERS_PER_RB + i_re] = to_cbf16(cf_t(re, im));
    }
  }

  return iq_data;
}

/// Draws an IQ scaling in [0.25, 1], where one is four times more likely than any other value, as the former MATLAB
/// test vectors did.
static float generate_random_iq_scaling(std::mt19937& rgen)
{
  return std::min(std::uniform_real_distribution<float>(0.25F, 1.25F)(rgen), 1.0F);
}

namespace {

/// Compression parameters and FS_Offset of a test case.
struct compression_case {
  ru_compression_params   params;
  std::optional<unsigned> fs_offset;
};

class ofh_iq_compression_test_fixture : public ::testing::TestWithParam<std::tuple<std::string, compression_case>>
{
protected:
  const std::string&           implementation = std::get<0>(GetParam());
  const ru_compression_params& params         = std::get<1>(GetParam()).params;
  std::optional<unsigned>      fs_offset      = std::get<1>(GetParam()).fs_offset;
  std::mt19937                 rgen{0};
};

} // namespace

/// Returns a readable name of a test case.
static std::string get_case_name(const std::string& implementation, const compression_case& test_case)
{
  return fmt::format("{}_{}{}_fs_offset_{}",
                     implementation,
                     test_case.params.type == compression_type::BFP ? "bfp" : "none",
                     test_case.params.data_width,
                     test_case.fs_offset.has_value() ? std::to_string(*test_case.fs_offset) : "native");
}

TEST_P(ofh_iq_compression_test_fixture, compression_matches_reference)
{
  for (unsigned nof_prbs : nof_prbs_list) {
    std::vector<cbf16_t> iq_data    = generate_random_iq_data(rgen, nof_prbs);
    float                iq_scaling = generate_random_iq_scaling(rgen);

    {
      error_type<std::string> result =
          compare_compressed_prbs(compress(iq_data, params, iq_scaling, fs_offset, implementation),
                                  compress_iq_reference(iq_data, params, iq_scaling, fs_offset),
                                  params,
                                  fmt::format("nof_prbs={} iq_scaling={}", nof_prbs, iq_scaling));
      ASSERT_TRUE(result.has_value()) << result.error();
    }
  }
}

TEST_P(ofh_iq_compression_test_fixture, decompression_matches_reference)
{
  std::uniform_int_distribution<unsigned> byte_dist(0, 255);

  for (unsigned nof_prbs : nof_prbs_list) {
    // Any byte sequence is a valid compressed resource block, including any exponent and reserved bits.
    std::vector<uint8_t> compressed(nof_prbs * get_compressed_prb_size_reference(params));
    std::generate(compressed.begin(), compressed.end(), [&]() { return byte_dist(rgen); });

    check_decompressed_iq(decompress(compressed, params, fs_offset, implementation),
                          decompress_iq_reference(compressed, params, fs_offset),
                          fmt::format("nof_prbs={}", nof_prbs));
  }
}

TEST_P(ofh_iq_compression_test_fixture, round_trip_restores_scaled_input)
{
  bool                        is_bfp     = (params.type == compression_type::BFP);
  fs_offset_scaling_reference fs_scaling = get_fs_offset_scaling_reference(params, fs_offset);
  float                       gain       = get_quantizer_gain_reference(is_bfp ? MAX_IQ_WIDTH : params.data_width);
  unsigned                    prb_size   = get_compressed_prb_size_reference(params);

  for (unsigned nof_prbs : nof_prbs_list) {
    std::vector<cbf16_t> iq_data    = generate_random_iq_data(rgen, nof_prbs);
    float                iq_scaling = generate_random_iq_scaling(rgen);

    std::vector<uint8_t> compressed = compress(iq_data, params, iq_scaling, fs_offset, implementation);
    std::vector<cbf16_t> restored   = decompress(compressed, params, fs_offset, implementation);

    for (unsigned i = 0, i_end = iq_data.size(); i != i_end; ++i) {
      unsigned i_prb = i / NOF_SUBCARRIERS_PER_RB;

      // The quantization rounds to half a step, and the BFP mantissa drops the bits below the shared exponent.
      unsigned exponent = is_bfp ? (compressed[i_prb * prb_size] & 0x0fU) - fs_scaling.exponent_offset : 0;
      float    error    = (std::exp2(static_cast<float>(exponent)) + 0.5F) / (gain * fs_scaling.dl_amplitude);

      cf_t expected = to_cf(iq_data[i]) * iq_scaling;
      cf_t actual   = to_cf(restored[i]);
      ASSERT_LE(std::abs(actual.real() - expected.real()), error + std::abs(expected.real()) / 256.0F)
          << fmt::format("nof_prbs={}: RE {} (PRB {})", nof_prbs, i, i_prb);
      ASSERT_LE(std::abs(actual.imag() - expected.imag()), error + std::abs(expected.imag()) / 256.0F)
          << fmt::format("nof_prbs={}: RE {} (PRB {})", nof_prbs, i, i_prb);
    }
  }
}

/// Generates the test cases: every compression bit width with the native and a set of configured FS_Offset values.
static std::vector<compression_case> generate_compression_cases()
{
  std::vector<compression_case> cases;
  for (unsigned data_width = 8; data_width <= MAX_IQ_WIDTH; ++data_width) {
    // FS_Offset values at, around and above the native one.
    cases.push_back({{compression_type::none, data_width}, std::nullopt});
    for (unsigned fs_offset : {0U, 1U, 4U}) {
      cases.push_back({{compression_type::none, data_width}, fs_offset});
    }

    unsigned native_fs_offset = 2 * (data_width - 1);
    cases.push_back({{compression_type::BFP, data_width}, std::nullopt});
    for (unsigned fs_offset : {0U, 1U, native_fs_offset, native_fs_offset + 4}) {
      cases.push_back({{compression_type::BFP, data_width}, fs_offset});
    }
  }
  return cases;
}

INSTANTIATE_TEST_SUITE_P(ofh_iq_compression_test,
                         ofh_iq_compression_test_fixture,
                         ::testing::Combine(::testing::ValuesIn(get_testable_implementations()),
                                            ::testing::ValuesIn(generate_compression_cases())),
                         [](const auto& param_info) {
                           return get_case_name(std::get<0>(param_info.param), std::get<1>(param_info.param));
                         });

namespace {

/// Resource blocks with a name, to report which one fails.
struct named_prbs {
  std::string          name;
  std::vector<cbf16_t> iq_data;
};

class ofh_iq_compression_edge_case_fixture
  : public ::testing::TestWithParam<std::tuple<std::string, ru_compression_params>>
{
protected:
  const std::string&           implementation = std::get<0>(GetParam());
  const ru_compression_params& params         = std::get<1>(GetParam());
};

/// Fixture for the edge cases that only apply to BFP.
class ofh_iq_compression_bfp_edge_case_fixture : public ofh_iq_compression_edge_case_fixture
{};

} // namespace

/// Returns a resource block where all the I and Q components take the given value.
static std::vector<cbf16_t> generate_constant_prb(float value)
{
  return std::vector<cbf16_t>(NOF_SUBCARRIERS_PER_RB, to_cbf16(cf_t(value, value)));
}

/// Returns resource blocks where all the I and Q components take the given values in turns.
static std::vector<cbf16_t> generate_pattern_prbs(span<const float> values, unsigned nof_prbs)
{
  std::vector<cbf16_t> iq_data(nof_prbs * NOF_SUBCARRIERS_PER_RB);
  for (unsigned i = 0, i_end = iq_data.size(); i != i_end; ++i) {
    iq_data[i] = to_cbf16(cf_t(values[(2 * i) % values.size()], values[(2 * i + 1) % values.size()]));
  }
  return iq_data;
}

/// \brief Generates resource blocks that take every BFP exponent of the given bit width.
///
/// Each resource block holds a single non-zero I or Q value, which sweeps all the positions and both signs. Its 16-bit
/// quantized magnitude is 1.5 times the smallest one that results in the exponent.
static std::vector<cbf16_t> generate_exponent_sweep(unsigned data_width)
{
  std::vector<cbf16_t> iq_data;
  for (unsigned exponent = 0; exponent <= MAX_IQ_WIDTH - data_width; ++exponent) {
    // Smallest 16-bit quantized magnitude that results in the exponent, see the determine_exponent_reference function.
    float min_quantized = std::exp2(static_cast<float>(data_width + exponent) - 2.0F);
    // Multiply it by 1.5 to move away from the exponent boundaries.
    float magnitude = 1.5F * min_quantized / get_quantizer_gain_reference(MAX_IQ_WIDTH);

    for (unsigned re = 0; re != nof_values_per_prb; ++re) {
      for (float sign : {1.0F, -1.0F}) {
        std::vector<cbf16_t> prb(NOF_SUBCARRIERS_PER_RB, to_cbf16(cf_t()));

        cf_t value  = (re % 2 == 0) ? cf_t(sign * magnitude, 0) : cf_t(0, sign * magnitude);
        prb[re / 2] = to_cbf16(value);
        iq_data.insert(iq_data.end(), prb.begin(), prb.end());
      }
    }
  }
  return iq_data;
}

/// \brief Generates the edge cases.
///
/// BFP quantizes to 16 bits, so the 16-bit full scale is used to place the values at exponent boundaries.
static std::vector<named_prbs> generate_edge_cases()
{
  // 16-bit full scale.
  float full_scale_16 = get_quantizer_gain_reference(MAX_IQ_WIDTH);

  std::vector<named_prbs> cases;
  cases.push_back({"zero", generate_constant_prb(0.0F)});
  cases.push_back({"positive_full_scale", generate_constant_prb(1.0F)});
  cases.push_back({"negative_full_scale", generate_constant_prb(-1.0F)});
  cases.push_back({"alternating_full_scale", generate_pattern_prbs(std::vector<float>{1.0F, -1.0F, -1.0F, 1.0F}, 1)});

  // Real valued BPSK symbols of the former MATLAB test.
  static const std::array<float, NOF_SUBCARRIERS_PER_RB * 4> bpsk = {
      0, 0, 0, 0,  0, 0, 0,  0, 0, 1,  1,  1, -1, -1, 1, 1,  -1, 1,  1,  1, -1, 1,  1,  1,
      1, 1, 1, -1, 1, 1, -1, 1, 1, -1, -1, 1, -1, 1,  1, -1, -1, -1, -1, 1, -1, -1, -1, 1};
  std::vector<cbf16_t> bpsk_iq_data;
  for (float symbol : bpsk) {
    bpsk_iq_data.push_back(to_cbf16(cf_t(symbol, 0.0F)));
  }
  cases.push_back({"bpsk", bpsk_iq_data});

  // All values equal to a negative power of two. The negative full scale of a mantissa is one higher than the positive
  // one, so these blocks do not need a higher exponent than the next smaller magnitude.
  for (unsigned power = 7; power != MAX_IQ_WIDTH - 1; ++power) {
    cases.push_back({fmt::format("all_minus_2^{}", power),
                     generate_constant_prb(-std::exp2(static_cast<float>(power)) / full_scale_16)});
  }

  // Values that quantize to minus one in 16 bits, alone and mixed with zero. These are the only blocks whose maximum
  // absolute value is not positive.
  float minus_one = -1.0F / full_scale_16;
  cases.push_back({"all_minus_one", generate_constant_prb(minus_one)});
  cases.push_back({"minus_one_and_zero", generate_pattern_prbs(std::vector<float>{minus_one, 0.0F, 0.0F}, 1)});

  // Values above the full scale must saturate at the full scale.
  cases.push_back(
      {"clipping", generate_pattern_prbs(std::vector<float>{1.5F, -1.5F, 1.0078125F, -1.0078125F, 4.0F}, 2)});
  return cases;
}

TEST_P(ofh_iq_compression_edge_case_fixture, compression_matches_reference)
{
  std::vector<named_prbs> edge_cases = generate_edge_cases();
  edge_cases.push_back({"exponent_sweep", generate_exponent_sweep(params.data_width)});

  std::vector<cbf16_t> all_iq_data;
  for (const named_prbs& edge_case : edge_cases) {
    // Alone, so that the PRBs go through the remainder paths.
    {
      error_type<std::string> result =
          compare_compressed_prbs(compress(edge_case.iq_data, params, 1.0F, std::nullopt, implementation),
                                  compress_iq_reference(edge_case.iq_data, params, 1.0F, std::nullopt),
                                  params,
                                  edge_case.name);
      ASSERT_TRUE(result.has_value()) << result.error();
    }
    all_iq_data.insert(all_iq_data.end(), edge_case.iq_data.begin(), edge_case.iq_data.end());
  }

  // Together, so that the PRBs also go through the SIMD batches.
  {
    error_type<std::string> result =
        compare_compressed_prbs(compress(all_iq_data, params, 1.0F, std::nullopt, implementation),
                                compress_iq_reference(all_iq_data, params, 1.0F, std::nullopt),
                                params,
                                "all edge cases");
    ASSERT_TRUE(result.has_value()) << result.error();
  }
}

TEST_P(ofh_iq_compression_bfp_edge_case_fixture, exponent_sweep_takes_every_exponent)
{
  std::vector<uint8_t> compressed =
      compress(generate_exponent_sweep(params.data_width), params, 1.0F, std::nullopt, implementation);

  // Each exponent covers all the positions, with both signs.
  unsigned prb_size          = get_compressed_prb_size_reference(params);
  unsigned nof_prbs_exponent = nof_values_per_prb * 2;
  for (unsigned i_prb = 0, nof_prbs = compressed.size() / prb_size; i_prb != nof_prbs; ++i_prb) {
    // Compare exponent per compressed PRB with the expected one.
    ASSERT_EQ(compressed[i_prb * prb_size], i_prb / nof_prbs_exponent) << fmt::format("PRB {}", i_prb);
  }
}

TEST_P(ofh_iq_compression_bfp_edge_case_fixture, negative_saturation_keeps_exponent_within_four_bits)
{
  // All values saturate at -32768 in 16 bits. Its magnitude must not raise the exponent above the one of the positive
  // full scale, or the FS_Offset exponent offset would overflow the four bits of the exponent.
  std::vector<cbf16_t> iq_data = generate_constant_prb(-1.5F);

  for (std::optional<unsigned> fs_offset : {std::optional<unsigned>(), std::optional<unsigned>(0)}) {
    std::string          context    = fmt::format("fs_offset={}", fs_offset.has_value() ? "0" : "native");
    std::vector<uint8_t> compressed = compress(iq_data, params, 1.0F, fs_offset, implementation);

    {
      error_type<std::string> result =
          compare_compressed_prbs(compressed, compress_iq_reference(iq_data, params, 1.0F, fs_offset), params, context);
      ASSERT_TRUE(result.has_value()) << result.error();
    }
    ASSERT_EQ(compressed[0] & 0xf0U, 0) << context << ": reserved bits of the compression parameter are set";
  }
}

TEST_P(ofh_iq_compression_edge_case_fixture, rounding_ties_go_to_even)
{
  // Find an IQ scaling for which the scaled quantization gain is exactly one, so that the inputs below are ties.
  float gain = get_quantizer_gain_reference((params.type == compression_type::BFP) ? MAX_IQ_WIDTH : params.data_width);
  float iq_scaling = 1.0F / gain;
  for (unsigned i = 0; (i != 8) && (gain * iq_scaling != 1.0F); ++i) {
    iq_scaling = std::nextafter(iq_scaling, (gain * iq_scaling < 1.0F) ? 2.0F : 0.0F);
  }
  ASSERT_EQ(gain * iq_scaling, 1.0F) << "No IQ scaling results in a unit quantization gain";

  std::vector<cbf16_t> iq_data =
      generate_pattern_prbs(std::vector<float>{0.5F, -0.5F, 1.5F, -1.5F, 2.5F, -2.5F, 3.5F, -3.5F, 2.5F}, 25);

  {
    error_type<std::string> result =
        compare_compressed_prbs(compress(iq_data, params, iq_scaling, std::nullopt, implementation),
                                compress_iq_reference(iq_data, params, iq_scaling, std::nullopt),
                                params,
                                "ties");
    ASSERT_TRUE(result.has_value()) << result.error();
  }
}

TEST_P(ofh_iq_compression_edge_case_fixture, out_of_range_input_saturates_at_full_scale)
{
  // Twice the full scale, with both signs.
  std::vector<cbf16_t> iq_data = generate_pattern_prbs(std::vector<float>{2.0F, -2.0F, -2.0F}, 3);

  std::vector<cbf16_t> restored =
      decompress(compress(iq_data, params, 1.0F, std::nullopt, implementation), params, std::nullopt, implementation);

  // The sign must be kept and the magnitude must be the full scale, within the BFP and brain float resolution.
  float tolerance = std::exp2(1.0F - static_cast<float>(params.data_width)) + 1.0F / 256.0F;

  std::vector<cf_t> expected(iq_data.size());
  std::vector<cf_t> actual(restored.size());
  std::transform(iq_data.begin(), iq_data.end(), expected.begin(), [](cbf16_t value) { return to_cf(value) / 2.0F; });
  std::transform(restored.begin(), restored.end(), actual.begin(), [](cbf16_t value) { return to_cf(value); });

  // The distance is the largest of the I and Q errors, so the tolerance applies to each component.
  auto distance = [](cf_t actual_val, cf_t expected_val) -> float {
    return std::max(std::abs(actual_val.real() - expected_val.real()),
                    std::abs(actual_val.imag() - expected_val.imag()));
  };
  error_type<std::string> result = compare_sequences<cf_t, cf_t>(actual, expected, distance, tolerance);
  ASSERT_TRUE(result.has_value()) << result.error();
}

/// Generates the compression parameters of the edge cases for the given compression types.
static std::vector<ru_compression_params> generate_edge_case_params(std::initializer_list<compression_type> types)
{
  std::vector<ru_compression_params> params;
  for (compression_type type : types) {
    for (unsigned data_width = 8; data_width <= MAX_IQ_WIDTH; ++data_width) {
      params.push_back({type, data_width});
    }
  }
  return params;
}

INSTANTIATE_TEST_SUITE_P(
    ofh_iq_compression_edge_cases,
    ofh_iq_compression_edge_case_fixture,
    ::testing::Combine(::testing::ValuesIn(get_testable_implementations()),
                       ::testing::ValuesIn(generate_edge_case_params({compression_type::none, compression_type::BFP}))),
    [](const auto& param_info) {
      return get_case_name(std::get<0>(param_info.param), {std::get<1>(param_info.param), std::nullopt});
    });

INSTANTIATE_TEST_SUITE_P(ofh_iq_compression_bfp_edge_cases,
                         ofh_iq_compression_bfp_edge_case_fixture,
                         ::testing::Combine(::testing::ValuesIn(get_testable_implementations()),
                                            ::testing::ValuesIn(generate_edge_case_params({compression_type::BFP}))),
                         [](const auto& param_info) {
                           return get_case_name(std::get<0>(param_info.param),
                                                {std::get<1>(param_info.param), std::nullopt});
                         });
