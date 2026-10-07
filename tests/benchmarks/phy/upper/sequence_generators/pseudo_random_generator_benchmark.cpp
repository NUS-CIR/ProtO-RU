// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/adt/format.h"
#include "ocudu/ocuduvec/bit.h"
#include "ocudu/phy/upper/sequence_generators/sequence_generator_factories.h"
#include "ocudu/support/benchmark_utils.h"
#include "ocudu/support/error_handling.h"
#include <algorithm>
#include <array>
#include <getopt.h>

using namespace ocudu;

static std::string generator_type  = "auto";
static unsigned    nof_repetitions = 10000;
static bool        silent          = false;

/// Bit lengths around the AVX2 threshold and typical PDSCH codeblock sizes.
static constexpr std::array<unsigned, 12> nof_bits_list =
    {128, 512, 1024, 1536, 2048, 3072, 4096, 8192, 16384, 65536, 131072, 524288};

static void usage(const char* prog)
{
  fmt::print("Usage: {} [-R repetitions] [-T generator type] [-s silent]\n", prog);
  fmt::print("\t-R Repetitions [Default {}]\n", nof_repetitions);
  fmt::print("\t-T Generator type (generic, avx2, auto) [Default {}]\n", generator_type);
  fmt::print("\t-s Toggle silent operation [Default {}]\n", silent);
  fmt::print("\t-h Show this message\n");
}

static void parse_args(int argc, char** argv)
{
  int opt = 0;
  while ((opt = getopt(argc, argv, "R:T:sh")) != -1) {
    switch (opt) {
      case 'R':
        nof_repetitions = std::strtol(optarg, nullptr, 10);
        break;
      case 'T':
        generator_type = std::string(optarg);
        break;
      case 's':
        silent = (!silent);
        break;
      case 'h':
      default:
        usage(argv[0]);
        std::exit(0);
    }
  }
}

int main(int argc, char** argv)
{
  parse_args(argc, argv);

  std::shared_ptr<pseudo_random_generator_factory> factory = create_pseudo_random_generator_sw_factory(generator_type);
  report_fatal_error_if_not(factory, "pseudo_random_generator_factory");

  std::unique_ptr<pseudo_random_generator> generator = factory->create();
  report_fatal_error_if_not(generator, "pseudo_random_generator");

  static constexpr unsigned c_init = 0x12345678U;

  benchmarker apply_xor_meas(generator_type + " apply_xor", nof_repetitions);
  benchmarker generate_meas(generator_type + " generate", nof_repetitions);

  for (unsigned nof_bits : nof_bits_list) {
    dynamic_bit_buffer in_bits(nof_bits);
    dynamic_bit_buffer out_bits(nof_bits);
    std::fill(in_bits.get_buffer().begin(), in_bits.get_buffer().end(), uint8_t{0x5a});

    const std::string meas_descr = fmt::format("{:>6} bits", nof_bits);

    apply_xor_meas.new_measure(meas_descr, nof_bits, [&]() {
      generator->init(c_init);
      generator->apply_xor(out_bits, in_bits);
      do_not_optimize(out_bits);
    });

    dynamic_bit_buffer data_bits(nof_bits);
    generate_meas.new_measure(meas_descr, nof_bits, [&]() {
      generator->init(c_init);
      generator->generate(data_bits);
      do_not_optimize(data_bits);
    });
  }

  if (!silent) {
    apply_xor_meas.print_percentiles_throughput("bits");
    generate_meas.print_percentiles_throughput("bits");
  }

  return 0;
}
