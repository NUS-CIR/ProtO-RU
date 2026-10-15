// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/cuda/adt/cuda_copy.h"
#include "ocudu/cuda/adt/cuda_stream.h"
#include "ocudu/cuda/adt/device_vector.h"
#include "ocudu/cuda/phy/upper/channel_processors/pdsch/pdsch_fused_ratematcher_modulator.h"
#include "ocudu/phy/upper/channel_coding/channel_coding_factories.h"
#include "ocudu/phy/upper/channel_coding/ldpc/ldpc_encoder_buffer.h"
#include "ocudu/phy/upper/channel_modulation/channel_modulation_factories.h"
#include "ocudu/phy/upper/sequence_generators/sequence_generator_factories.h"
#include <cuda_runtime.h>
#include <gtest/gtest.h>
#include <random>
#include <vector>

using namespace ocudu;

namespace {

/// Returns true if a CUDA device is available to run the test.
bool has_cuda_device()
{
  int nof_devices = 0;
  return (::cudaGetDeviceCount(&nof_devices) == cudaSuccess) && (nof_devices > 0);
}

/// Shape a case encodes.
struct fused_case {
  ldpc_base_graph_type base_graph;
  ldpc::lifting_size_t lifting_size;
  unsigned             rv;
  unsigned             nof_filler_bits;
  modulation_scheme    mod;
  /// Length the circular buffer is limited to, or zero for the whole of it.
  unsigned    Nref;
  const char* reaches;
};

const std::vector<fused_case> cases = {
    {ldpc_base_graph_type::BG1, ldpc::LS384, 0, 0, modulation_scheme::QPSK, 0, "base graph 1, QPSK"},
    {ldpc_base_graph_type::BG1, ldpc::LS384, 0, 0, modulation_scheme::QAM16, 0, "base graph 1, 16QAM"},
    {ldpc_base_graph_type::BG1, ldpc::LS384, 2, 0, modulation_scheme::QAM64, 0, "base graph 1, 64QAM, rv 2"},
    {ldpc_base_graph_type::BG1, ldpc::LS256, 0, 128, modulation_scheme::QAM256, 0, "filler bits, 256QAM"},
    {ldpc_base_graph_type::BG2, ldpc::LS384, 0, 0, modulation_scheme::QPSK, 0, "base graph 2, QPSK"},
    {ldpc_base_graph_type::BG2, ldpc::LS128, 1, 64, modulation_scheme::QAM16, 0, "base graph 2, filler bits, rv 1"},
    // A buffer short enough that the selection passes its end, at the redundancy versions whose
    // starting position sits late in the buffer. Without the limit the kernel wraps at the whole
    // buffer and takes bits the limited one does not hold.
    {ldpc_base_graph_type::BG1, ldpc::LS384, 2, 0, modulation_scheme::QAM64, 12288, "limited buffer, rv 2"},
    {ldpc_base_graph_type::BG1, ldpc::LS384, 3, 0, modulation_scheme::QAM64, 12288, "limited buffer, rv 3"},
    {ldpc_base_graph_type::BG1, ldpc::LS384, 0, 0, modulation_scheme::QPSK, 10752, "limited buffer, rv 0"},
    {ldpc_base_graph_type::BG2,
     ldpc::LS384,
     2,
     128,
     modulation_scheme::QAM16,
     8192,
     "limited buffer, filler bits, rv 2"},
};
class pdsch_fused_kernel_test : public ::testing::Test
{
protected:
  void SetUp() override
  {
    if (!has_cuda_device()) {
      GTEST_SKIP() << "No CUDA device available";
    }
    cuda::cuda_expected<cuda::cuda_stream> created_stream = cuda::cuda_stream::create();
    ASSERT_TRUE(created_stream.has_value());
    stream = std::move(created_stream.value());

    cuda::cuda_expected<cuda::pdsch_fused_ratematcher_modulator> created =
        cuda::pdsch_fused_ratematcher_modulator::create();
    ASSERT_TRUE(created.has_value()) << created.error();
    gpu = std::move(created.value());

    encoder      = create_ldpc_encoder_factory_sw("generic")->create();
    rate_matcher = create_ldpc_rate_matcher_factory_sw()->create();
    scrambler    = create_pseudo_random_generator_sw_factory()->create();
    modulator    = create_modulation_mapper_factory()->create();
    ASSERT_NE(encoder, nullptr);
    ASSERT_NE(rate_matcher, nullptr);
    ASSERT_NE(scrambler, nullptr);
    ASSERT_NE(modulator, nullptr);
  }

  static dynamic_bit_buffer make_bits(unsigned nof_bits, unsigned seed)
  {
    dynamic_bit_buffer                      bits(nof_bits);
    std::mt19937                            rgen(seed);
    std::uniform_int_distribution<unsigned> dist(0, 1);
    for (unsigned i = 0; i != nof_bits; ++i) {
      bits.insert(dist(rgen), i, 1);
    }
    return bits;
  }

  /// Position bit \c index occupies inside its word: most significant bit first inside a byte,
  /// bytes least significant first inside the word.
  static unsigned bit_position(unsigned index)
  {
    unsigned bit_in_word  = index % 32;
    unsigned byte_in_word = bit_in_word / 8;
    unsigned bit_in_byte  = 7 - (bit_in_word % 8);
    return byte_in_word * 8 + bit_in_byte;
  }

  /// Rebuilds the codeword the fused rate matcher and modulator reads, the punctured columns included.
  static std::vector<uint8_t> full_codeword(const bit_buffer& message, const ldpc_encoder_buffer& encoded, unsigned Z)
  {
    unsigned             nof_punctured = 2 * Z;
    std::vector<uint8_t> transmitted(encoded.get_codeblock_length());
    encoded.write_codeblock(transmitted, 0);

    std::vector<uint8_t> full(nof_punctured + transmitted.size());
    for (unsigned i = 0; i != nof_punctured; ++i) {
      full[i] = message.extract(i, 1);
    }
    std::copy(transmitted.begin(), transmitted.end(), full.begin() + nof_punctured);
    return full;
  }

  static std::vector<uint32_t> pack(const std::vector<uint8_t>& bits, unsigned nof_words)
  {
    std::vector<uint32_t> words(nof_words, 0);
    for (unsigned i = 0, end = bits.size(); i != end; ++i) {
      if (bits[i] != 0) {
        words[i / 32] |= (1U << bit_position(i));
      }
    }
    return words;
  }

  /// Packs a sequence the way the generator writes it: bit \c n at position <tt>31 - n modulo
  /// 32</tt> of word <tt>n / 32</tt>.
  static std::vector<uint32_t> pack_sequence(const bit_buffer& bits)
  {
    std::vector<uint32_t> words((bits.size() + 31) / 32, 0);
    for (unsigned i = 0, end = bits.size(); i != end; ++i) {
      if (bits.extract(i, 1) != 0) {
        words[i / 32] |= (1U << (31U - (i % 32U)));
      }
    }
    return words;
  }

  static codeblock_metadata make_metadata(const fused_case& c, unsigned E)
  {
    codeblock_metadata cfg;
    cfg.tb_common.base_graph        = c.base_graph;
    cfg.tb_common.lifting_size      = c.lifting_size;
    cfg.tb_common.rv                = c.rv;
    cfg.tb_common.mod               = c.mod;
    cfg.tb_common.Nref              = c.Nref;
    cfg.cb_specific.nof_filler_bits = c.nof_filler_bits;
    cfg.cb_specific.rm_length       = E;
    cfg.cb_specific.nof_crc_bits    = 16;
    return cfg;
  }

  /// \brief What the library produces for 1 code block, stage by stage.
  ///
  /// The software rate matcher interleaves as part of matching, so the chain here is the same
  /// sequence of stages the fused kernel runs in 1 pass.
  std::vector<ci8_t> library_chain(const fused_case&          c,
                                   const bit_buffer&          message,
                                   const ldpc_encoder_buffer& encoded,
                                   const codeblock_metadata&  cfg,
                                   uint32_t                   c_init)
  {
    unsigned E = cfg.cb_specific.rm_length;

    dynamic_bit_buffer matched(E);
    rate_matcher->rate_match(matched, encoded, cfg);

    dynamic_bit_buffer scrambled(E);
    scrambler->init(c_init);
    scrambler->apply_xor(scrambled, matched);

    std::vector<ci8_t> symbols(E / static_cast<unsigned>(c.mod));
    modulator->modulate(symbols, scrambled, c.mod);
    return symbols;
  }
  cuda::cuda_stream stream;

  cuda::pdsch_fused_ratematcher_modulator   gpu;
  std::unique_ptr<ocudu::ldpc_encoder>      encoder;
  std::unique_ptr<ocudu::ldpc_rate_matcher> rate_matcher;
  std::unique_ptr<pseudo_random_generator>  scrambler;
  std::unique_ptr<modulation_mapper>        modulator;
};

} // namespace

TEST_F(pdsch_fused_kernel_test, the_fused_pass_matches_the_library_chain)
{
  // The kernel runs rate matching, interleaving, scrambling and modulation in 1 pass. The library
  // runs the same 4 stages one after another, so the symbols must come out the same.
  uint32_t c_init = (0x4601U << 15U) + 321U;

  for (const fused_case& c : cases) {
    unsigned Z  = static_cast<unsigned>(c.lifting_size);
    unsigned Kb = (c.base_graph == ldpc_base_graph_type::BG1) ? 22 : 10;
    unsigned K  = Kb * Z;
    unsigned Qm = static_cast<unsigned>(c.mod);
    // A rate matched length that is a whole number of symbols, as the specification requires.
    unsigned E = (K * 2 / Qm) * Qm;

    dynamic_bit_buffer                 message = make_bits(K, 0x5eed);
    ocudu::ldpc_encoder::configuration enc_cfg;
    enc_cfg.base_graph                 = c.base_graph;
    enc_cfg.lifting_size               = c.lifting_size;
    const ldpc_encoder_buffer& encoded = encoder->encode(message, enc_cfg);

    codeblock_metadata   cfg               = make_metadata(c, E);
    std::vector<uint8_t> full              = full_codeword(message, encoded, Z);
    unsigned             nof_encoded_words = (static_cast<unsigned>(full.size()) + 31) / 32;

    std::vector<ci8_t> expected = library_chain(c, message, encoded, cfg, c_init);

    cuda::pdsch_fused_transport_block tb;
    tb.nof_codeblocks       = 1;
    tb.nof_short_codeblocks = 0;
    tb.short_length         = 0;
    tb.long_length          = E;
    tb.encoded_word_stride  = nof_encoded_words;

    ASSERT_TRUE(gpu.configure(cfg, tb).has_value()) << c.reaches;
    ASSERT_EQ(gpu.get_nof_symbols(), E / Qm) << c.reaches;

    std::vector<uint32_t> words = pack(full, nof_encoded_words);

    cuda::cuda_expected<cuda::device_vector<uint32_t>> d_in = cuda::device_vector<uint32_t>::create(words.size());
    cuda::cuda_expected<cuda::device_vector<int8_t>>   d_out =
        cuda::device_vector<int8_t>::create(2 * static_cast<size_t>(E / Qm));
    ASSERT_TRUE(d_in.has_value());
    ASSERT_TRUE(d_out.has_value());
    ASSERT_TRUE(cuda::copy_to_device(d_in.value(), span<const uint32_t>(words.data(), words.size())).has_value());

    ASSERT_TRUE(gpu.map_to_symbols(d_out.value().data(), d_in.value().data(), c_init, stream).has_value()) << c.reaches;
    ASSERT_EQ(::cudaDeviceSynchronize(), cudaSuccess) << c.reaches;

    std::vector<int8_t> actual(2 * static_cast<size_t>(E / Qm));
    ASSERT_TRUE(cuda::copy_to_host(span<int8_t>(actual.data(), actual.size()), d_out.value()).has_value());

    for (unsigned i = 0, end = E / Qm; i != end; ++i) {
      ASSERT_EQ(actual[2 * i], expected[i].real()) << c.reaches << ", symbol " << i << ", real part";
      ASSERT_EQ(actual[2 * i + 1], expected[i].imag()) << c.reaches << ", symbol " << i << ", imaginary part";
    }
  }
}

TEST_F(pdsch_fused_kernel_test, generating_the_sequence_gives_what_reusing_one_gives)
{
  // The kernel can generate the scrambling sequence where each thread stands, or read one prepared
  // in advance. The generator it carries is a second implementation of the one the library ships,
  // so the 2 arrangements must agree bit for bit.
  const fused_case& c      = cases.front();
  uint32_t          c_init = (0x4601U << 15U) + 321U;
  unsigned          Z      = static_cast<unsigned>(c.lifting_size);
  unsigned          K      = 22 * Z;
  unsigned          Qm     = static_cast<unsigned>(c.mod);
  unsigned          E      = (K * 2 / Qm) * Qm;

  dynamic_bit_buffer                 message = make_bits(K, 0x5eed);
  ocudu::ldpc_encoder::configuration enc_cfg;
  enc_cfg.base_graph                 = c.base_graph;
  enc_cfg.lifting_size               = c.lifting_size;
  const ldpc_encoder_buffer& encoded = encoder->encode(message, enc_cfg);

  codeblock_metadata   cfg               = make_metadata(c, E);
  std::vector<uint8_t> full              = full_codeword(message, encoded, Z);
  unsigned             nof_encoded_words = (static_cast<unsigned>(full.size()) + 31) / 32;

  cuda::pdsch_fused_transport_block tb;
  tb.nof_codeblocks      = 1;
  tb.long_length         = E;
  tb.encoded_word_stride = nof_encoded_words;
  ASSERT_TRUE(gpu.configure(cfg, tb).has_value());

  std::vector<uint32_t> words = pack(full, nof_encoded_words);

  // The sequence the library's own generator produces, packed the way the generator writes it.
  dynamic_bit_buffer sequence(E);
  scrambler->init(c_init);
  scrambler->generate(sequence);
  std::vector<uint32_t> sequence_words = pack_sequence(sequence);

  cuda::cuda_expected<cuda::device_vector<uint32_t>> d_in = cuda::device_vector<uint32_t>::create(words.size());
  cuda::cuda_expected<cuda::device_vector<uint32_t>> d_seq =
      cuda::device_vector<uint32_t>::create(sequence_words.size());
  cuda::cuda_expected<cuda::device_vector<int8_t>> d_gen = cuda::device_vector<int8_t>::create(2 * (E / Qm));
  cuda::cuda_expected<cuda::device_vector<int8_t>> d_pre = cuda::device_vector<int8_t>::create(2 * (E / Qm));
  ASSERT_TRUE(d_in.has_value());
  ASSERT_TRUE(d_seq.has_value());
  ASSERT_TRUE(d_gen.has_value());
  ASSERT_TRUE(d_pre.has_value());
  ASSERT_TRUE(cuda::copy_to_device(d_in.value(), span<const uint32_t>(words.data(), words.size())).has_value());
  ASSERT_TRUE(cuda::copy_to_device(d_seq.value(), span<const uint32_t>(sequence_words.data(), sequence_words.size()))
                  .has_value());

  ASSERT_TRUE(gpu.map_to_symbols(d_gen.value().data(), d_in.value().data(), c_init, stream).has_value());
  ASSERT_TRUE(gpu.map_to_symbols(d_pre.value().data(), d_in.value().data(), d_seq.value().data(), stream).has_value());
  ASSERT_EQ(::cudaDeviceSynchronize(), cudaSuccess);

  std::vector<int8_t> generated(2 * (E / Qm));
  std::vector<int8_t> prepared(2 * (E / Qm));
  ASSERT_TRUE(cuda::copy_to_host(span<int8_t>(generated.data(), generated.size()), d_gen.value()).has_value());
  ASSERT_TRUE(cuda::copy_to_host(span<int8_t>(prepared.data(), prepared.size()), d_pre.value()).has_value());

  EXPECT_EQ(generated, prepared);
}

TEST_F(pdsch_fused_kernel_test, an_unimplemented_modulation_scheme_is_reported)
{
  codeblock_metadata cfg;
  cfg.tb_common.base_graph   = ldpc_base_graph_type::BG1;
  cfg.tb_common.lifting_size = ldpc::LS384;
  cfg.tb_common.mod          = modulation_scheme::BPSK;
  cfg.cb_specific.rm_length  = 1024;

  cuda::pdsch_fused_transport_block tb;
  tb.nof_codeblocks      = 1;
  tb.long_length         = 1024;
  tb.encoded_word_stride = 816;

  cuda::cuda_result status = gpu.configure(cfg, tb);
  ASSERT_FALSE(status.has_value());
  EXPECT_FALSE(status.error().empty());
}

TEST_F(pdsch_fused_kernel_test, a_length_that_is_not_a_whole_number_of_symbols_is_reported)
{
  // Without the check the interleaver drops the bits of the last, incomplete symbol in silence.
  codeblock_metadata cfg;
  cfg.tb_common.base_graph   = ldpc_base_graph_type::BG1;
  cfg.tb_common.lifting_size = ldpc::LS384;
  cfg.tb_common.mod          = modulation_scheme::QAM64;
  cfg.cb_specific.rm_length  = 1000;

  cuda::pdsch_fused_transport_block tb;
  tb.nof_codeblocks      = 1;
  tb.long_length         = 1000;
  tb.encoded_word_stride = 816;

  cuda::cuda_result status = gpu.configure(cfg, tb);
  ASSERT_FALSE(status.has_value());
  EXPECT_FALSE(status.error().empty());
}
