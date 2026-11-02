// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "support/compare_sequences.h"
#include "ocudu/adt/format.h"
#include "ocudu/ocuduvec/conversion.h"
#include "ocudu/ocuduvec/zero.h"
#include "ocudu/phy/generic_functions/precoding/precoding_factories.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "fmt/ostream.h"
#include <gtest/gtest.h>
#include <random>

using namespace ocudu;

// Pseudo-random number generator.
static std::mt19937 rgen;

using MultiplePRGParams = std::tuple<
    // Precoder type: generic, avx2, avx512
    std::string,
    // Number of RB.
    unsigned,
    // Number of antenna ports.
    unsigned>;

namespace {
class PrecodingFixture : public ::testing::TestWithParam<MultiplePRGParams>
{
protected:
  static std::shared_ptr<channel_precoder_factory> precoder_factory;
  static std::unique_ptr<channel_precoder>         precoder;

  void SetUp() override
  {
    const std::string& type = std::get<0>(GetParam());

    // Create channel precoder factory.
    precoder_factory = create_channel_precoder_factory(type);
    ASSERT_NE(precoder_factory, nullptr) << "Cannot create channel precoder factory";

    ASSERT_NE(precoder_factory, nullptr) << "Cannot create channel precoder factory";

    // Create channel precoder.
    precoder = precoder_factory->create();

    // If the precoder type is not supported, skip the test.
    if (!precoder) {
      GTEST_SKIP();
    }
  }

  // Generates and returns random RE values, as many as the specified number of layers and RE.
  const re_buffer_reader<>& generate_random_data(unsigned nof_layers, unsigned nof_re)
  {
    // Resize buffer.
    random_data.resize(nof_layers, nof_re);

    // Fill buffer with random values.
    for (unsigned i_layer = 0; i_layer != nof_layers; ++i_layer) {
      for (cf_t& val : random_data.get_slice(i_layer)) {
        val = {data_dist(rgen), data_dist(rgen)};
      }
    }

    return random_data;
  }

  // Generates and returns random RE values, as many as the specified number of layers and RE.
  span<const ci8_t> generate_random_data_ci8(unsigned nof_layers, unsigned nof_re)
  {
    // Resize buffer.
    random_data_ci8.resize(nof_layers * nof_re);

    // Fill buffer with random values.
    std::generate(random_data_ci8.begin(), random_data_ci8.end(), [this]() {
      return ci8_t(data_dist_i8(rgen), data_dist_i8(rgen));
    });

    return random_data_ci8;
  }

  // Generates the golden RE sequence, with the dimensions specified by the input buffer and precoding matrix.
  const re_buffer_reader<>& generate_golden(const re_buffer_reader<>&      input,
                                            const precoding_weight_matrix& precoding_matrix)
  {
    // Get dimensions.
    unsigned nof_re     = input.get_nof_re();
    unsigned nof_layers = precoding_matrix.get_nof_layers();
    unsigned nof_ports  = precoding_matrix.get_nof_ports();

    // Resize buffer.
    golden_data.resize(nof_ports, nof_re);

    EXPECT_EQ(nof_layers, input.get_nof_slices()) << "Input and precoding matrix number of layers don't match.";

    for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
      // View over the golden sequence for a single PRG and antenna port.
      span<cf_t> golden_re_port = golden_data.get_slice(i_port);

      // Set the output REs to zero.
      ocuduvec::zero(golden_re_port);

      for (unsigned i_layer = 0; i_layer != nof_layers; ++i_layer) {
        // View over the input data for a single layer.
        span<const cf_t> input_re_layer = input.get_slice(i_layer);

        for (unsigned i_re = 0; i_re != nof_re; ++i_re) {
          // Accumulate the contributions of all layers, scaled by each respective precoding weight.
          golden_re_port[i_re] += precoding_matrix.get_coefficient(i_layer, i_port) * input_re_layer[i_re];
        }
      }
    }

    return golden_data;
  }

  // Generates the golden RE sequence, with the dimensions specified by the input buffer and precoding matrix.
  const re_buffer_reader<>& generate_golden(span<const ci8_t> input, const precoding_weight_matrix& precoding_matrix)
  {
    // Get dimensions.
    unsigned nof_layers = precoding_matrix.get_nof_layers();
    unsigned nof_ports  = precoding_matrix.get_nof_ports();
    unsigned nof_re     = input.size() / nof_layers;

    // Resize buffer.
    golden_data.resize(nof_ports, nof_re);

    EXPECT_EQ(input.size() % nof_layers, 0) << "Input and precoding matrix number of layers are not consistent.";

    for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
      // View over the golden sequence for a single PRG and antenna port.
      span<cf_t> golden_re_port = golden_data.get_slice(i_port);

      for (unsigned i_re = 0; i_re != nof_re; ++i_re) {
        cf_t sum;
        for (unsigned i_layer = 0; i_layer != nof_layers; ++i_layer) {
          // Accumulate the contributions of all layers, scaled by each respective precoding weight.
          sum += precoding_matrix.get_coefficient(i_layer, i_port) * to_cf(input[nof_layers * i_re + i_layer]);
        }
        golden_re_port[i_re] = sum;
      }
    }

    return golden_data;
  }

  // Generates a precoding configuration with the specified dimensions and random precoding coefficients.
  precoding_weight_matrix generate_random_precoding(unsigned nof_layers, unsigned nof_ports)
  {
    // Create configuration.
    precoding_weight_matrix config(nof_layers, nof_ports);

    // Fill the precoding matrix with random values.
    for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
      for (unsigned i_layer = 0; i_layer != nof_layers; ++i_layer) {
        cf_t weight = {weight_dist(rgen), weight_dist(rgen)};
        config.set_coefficient(weight, i_layer, i_port);
      }
    }

    return config;
  }

private:
  // Buffer for holding floating point randomly generated data.
  static_re_buffer<precoding_constants::MAX_NOF_LAYERS, MAX_NOF_SUBCARRIERS * MAX_NSYMB_PER_SLOT> random_data;

  // Buffer for holding ci8_t randomly generated data.
  std::vector<ci8_t> random_data_ci8;

  // Buffer for holding the golden sequence.
  static_re_buffer<precoding_constants::MAX_NOF_PORTS, MAX_NOF_SUBCARRIERS * MAX_NSYMB_PER_SLOT> golden_data;

  // Uniform real distribution to generate data samples.
  std::uniform_real_distribution<float> data_dist{-10.0, +10.0};

  // Uniform integer distribution to generate data samples.
  std::uniform_int_distribution<int8_t> data_dist_i8{-120, 120};

  // Uniform distribution to generate precoding coefficients.
  std::uniform_real_distribution<float> weight_dist{-1, +1};
};

std::shared_ptr<channel_precoder_factory> PrecodingFixture::precoder_factory = nullptr;
std::unique_ptr<channel_precoder>         PrecodingFixture::precoder         = nullptr;

// Tests a precoding configuration with parametrized dimensions and random weights.
TEST_P(PrecodingFixture, RandomWeightsCft)
{
  const MultiplePRGParams& test_case = GetParam();

  unsigned nof_rb    = std::get<1>(test_case);
  unsigned nof_ports = std::get<2>(test_case);

  // Number of resource elements per layer.
  unsigned nof_re = nof_rb * NOF_SUBCARRIERS_PER_RB;

  // Buffer to hold the precoded RE.
  static_re_buffer<precoding_constants::MAX_NOF_PORTS, MAX_NOF_SUBCARRIERS * MAX_NSYMB_PER_SLOT, cbf16_t>
      precoding_buffer(nof_ports, nof_re);
  for (unsigned nof_layers = 1; nof_layers <= nof_ports; ++nof_layers) {
    // Generate random RE arranged by layers.
    const re_buffer_reader<>& input_data = generate_random_data(nof_layers, nof_re);

    // Create a random precoding configuration
    precoding_weight_matrix precoding_matrix = generate_random_precoding(nof_layers, nof_ports);

    // Generate the golden precoded data.
    const re_buffer_reader<>& golden = generate_golden(input_data, precoding_matrix);

    // Apply precoding.
    precoder->apply_precoding(precoding_buffer, input_data, precoding_matrix);

    // For each antenna port, compare the precoded RE with the golden sequence for all RE and PRG.
    for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
      error_type<std::string> compare_result = compare_sequences(
          span<const cf_t>(golden.get_slice(i_port)),
          span<const cbf16_t>(precoding_buffer.get_slice(i_port)),
          [](const cf_t& lhs_cf, const cbf16_t& rhs_val) {
            cf_t rhs_cf = to_cf(rhs_val);
            // Check against division by zero: when the golden value is not normal, the actual
            // value must be exactly zero.
            if (!std::isnormal(std::abs(lhs_cf))) {
              return rhs_cf == cf_t(0.0F, 0.0F) ? 0.0F : 1.0F;
            }
            return std::abs(lhs_cf - rhs_cf) / std::abs(lhs_cf);
          },
          1.0F / 256.0F);
      ASSERT_TRUE(compare_result.has_value()) << compare_result.error();
    }
  }
}

// Tests a precoding configuration with parametrized dimensions and random weights.
TEST_P(PrecodingFixture, RandomWeightsCi8)
{
  const MultiplePRGParams& test_case = GetParam();

  unsigned nof_rb    = std::get<1>(test_case);
  unsigned nof_ports = std::get<2>(test_case);

  // Number of resource elements per layer.
  unsigned nof_re = nof_rb * NOF_SUBCARRIERS_PER_RB;

  // Buffer to hold the precoded RE.
  static_re_buffer<precoding_constants::MAX_NOF_PORTS, MAX_NOF_SUBCARRIERS * MAX_NSYMB_PER_SLOT, cbf16_t>
      precoding_buffer(nof_ports, nof_re);
  for (unsigned nof_layers = 1; nof_layers <= nof_ports; ++nof_layers) {
    // Generate random RE arranged by layers.
    span<const ci8_t> input_data = generate_random_data_ci8(nof_layers, nof_re);

    // Create a random precoding configuration
    precoding_weight_matrix precoding_matrix = generate_random_precoding(nof_layers, nof_ports);

    // Generate the golden precoded data.
    const re_buffer_reader<>& golden = generate_golden(input_data, precoding_matrix);

    // Apply precoding.
    precoder->apply_layer_map_and_precoding(precoding_buffer, input_data, precoding_matrix);

    // For each antenna port, compare the precoded RE with the golden sequence for all RE and PRG.
    for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
      span<const cbf16_t> actual = precoding_buffer.get_slice(i_port);
      // The precoder output is quantized to cbf16_t, so a tolerance comparison is required (same as
      // RandomWeightsCft). The previous operator==(span<const cf_t>, span<const cbf16_t>) applied a
      // relative error of 1/256; keep the same semantics here.
      error_type<std::string> result = compare_sequences(
          actual,
          golden.get_slice(i_port),
          [](const cbf16_t& lhs_val, const cf_t& rhs_cf) {
            cf_t lhs_cf = to_cf(lhs_val);
            // Check against division by zero: when the golden value is not normal, the actual
            // value must be exactly zero.
            if (!std::isnormal(std::abs(rhs_cf))) {
              return lhs_cf == cf_t(0.0F, 0.0F) ? 0.0F : 1.0F;
            }
            return std::abs(lhs_cf - rhs_cf) / std::abs(rhs_cf);
          },
          1.0F / 256.0F);
      ASSERT_TRUE(result.has_value()) << result.error() << " nof_ports=" << nof_ports << " nof_layers=" << nof_layers;
    }
  }
}

INSTANTIATE_TEST_SUITE_P(RandomWeights,
                         PrecodingFixture,
                         ::testing::Combine(
                             //
                             ::testing::Values("generic"
#ifdef __x86_64__
                                               // Precoder types for x86.
                                               ,
                                               "avx2",
                                               "avx512"
#endif // __x86_64__
#ifdef __aarch64__
                                               // Precoder types for ARM.
                                               ,
                                               "neon"
#endif // __aarch64__
                                               ),
                             // Number of RB.
                             ::testing::Values(13, 51, MAX_NOF_PRBS),
                             // Number of antenna ports.
                             ::testing::Values(1, 2, 4, 8)));
} // namespace
