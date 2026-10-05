// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "pseudo_random_generator_avx2_impl.h"
#include "pseudo_random_generator_sequence.h"
#include "ocudu/support/ocudu_assert.h"
#include <array>
#include <immintrin.h>

using namespace ocudu;

/// Number of bytes in an AVX2 register.
static constexpr unsigned AVX2_SIZE_BYTE = 32;

/// Number of bits in an AVX2 register.
static constexpr unsigned AVX2_SIZE_BIT = AVX2_SIZE_BYTE * 8;

/// Minimum buffer size for the AVX2 word-wise implementation to be used.
/// 4096 bits keeps the bit-wise path for short buffers and lets long sequences amortize bootstrap overhead.
static constexpr unsigned min_nof_bits = 4096;

namespace ocudu {
namespace detail {

/// Sliding window of the last 32 words of a state sequence.
class pseudo_random_word_window
{
public:
  /// Returns the oldest eight words in the window.
  __m256i oldest() const { return registers[0]; }

  /// Returns eight consecutive words offset by \c K words from the start of the window.
  template <unsigned K>
  __m256i shifted() const
  {
    static_assert((K > 0) && (K <= 4), "Invalid word shift.");
    __m256i shifted_4 = _mm256_permute2x128_si256(registers[0], registers[1], 0x21);
    if constexpr (K == 4) {
      return shifted_4;
    } else {
      return _mm256_alignr_epi8(shifted_4, registers[0], 4 * K);
    }
  }

  /// Appends eight words and discards the oldest ones.
  void push(__m256i words)
  {
    registers[0] = registers[1];
    registers[1] = registers[2];
    registers[2] = registers[3];
    registers[3] = words;
  }

private:
  /// AVX2 registers holding the window in chronological order.
  __m256i registers[4] = {};
};

/// Word-wise pseudo-random sequence generator for packed bit buffers.
class pseudo_random_generator_sequence_avx2
{
public:
  /// Creates the generator from the reversed internal state.
  explicit pseudo_random_generator_sequence_avx2(const pseudo_random_generator::state_s& state)
  {
    // A zero state yields a zero sequence, so each bit-wise engine tracks one state sequence.
    pseudo_random_generator_sequence sequence_x1(state.x1, 0);
    pseudo_random_generator_sequence sequence_x2(0, state.x2);

    // Interleaves both engines to overlap their dependency chains.
    for (unsigned i_reg = 0; i_reg != 4; ++i_reg) {
      std::array<uint64_t, 4> bits_x1;
      std::array<uint64_t, 4> bits_x2;
      for (unsigned i_word = 0; i_word != 4; ++i_word) {
        bits_x1[i_word] = __builtin_bswap64(sequence_x1.step64());
        bits_x2[i_word] = __builtin_bswap64(sequence_x2.step64());
      }
      window_x1.push(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(bits_x1.data())));
      window_x2.push(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(bits_x2.data())));
    }
  }

  /// Returns the next sequence block in \ref bit_buffer byte order.
  __m256i step()
  {
    __m256i c = _mm256_xor_si256(window_x1.oldest(), window_x2.oldest());

    window_x1.push(_mm256_xor_si256(window_x1.shifted<1>(), window_x1.shifted<4>()));
    window_x2.push(_mm256_xor_si256(_mm256_xor_si256(window_x2.shifted<1>(), window_x2.shifted<2>()),
                                    _mm256_xor_si256(window_x2.shifted<3>(), window_x2.shifted<4>())));

    return c;
  }

  /// Returns the reversed internal state at the current sequence position.
  pseudo_random_generator::state_s get_state() const
  {
    return {get_reversed_state(window_x1.oldest()), get_reversed_state(window_x2.oldest())};
  }

private:
  /// Extracts the reversed 31-bit state from the leading bits of a sequence block.
  static uint32_t get_reversed_state(__m256i words)
  {
    return __builtin_bswap32(_mm_cvtsi128_si32(_mm256_castsi256_si128(words))) & ~1U;
  }

  /// Window for \f$x_1(n)\f$.
  pseudo_random_word_window window_x1;
  /// Window for \f$x_2(n)\f$.
  pseudo_random_word_window window_x2;
};

} // namespace detail
} // namespace ocudu

void pseudo_random_generator_avx2_impl::apply_xor(bit_buffer& out, const bit_buffer& in)
{
  ocudu_assert(in.size() == out.size(),
               "Input size (i.e., {}) and output size (i.e., {}) must be equal.",
               in.size(),
               out.size());

  if (in.size() < min_nof_bits) {
    pseudo_random_generator_impl::apply_xor(out, in);
    return;
  }

  detail::pseudo_random_generator_sequence_avx2 sequence(get_state());
  const uint8_t*                                in_ptr  = in.get_buffer().data();
  uint8_t*                                      out_ptr = out.get_buffer().data();

  // Processes batches of 256 bits in parallel.
  unsigned nof_bytes = (in.size() / AVX2_SIZE_BIT) * AVX2_SIZE_BYTE;
  for (unsigned i_byte = 0; i_byte != nof_bytes; i_byte += AVX2_SIZE_BYTE) {
    __m256i c = sequence.step();
    __m256i v = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(in_ptr + i_byte));
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(out_ptr + i_byte), _mm256_xor_si256(v, c));
  }

  // Hands off to the bit-wise implementation, which reads x1 and x2.
  init(sequence.get_state());

  // Process spare bits in a batch of the remainder bits.
  bit_buffer out_remainder = out.last(out.size() - (nof_bytes / AVX2_SIZE_BYTE) * AVX2_SIZE_BIT);
  pseudo_random_generator_impl::apply_xor(out_remainder, in.last(out_remainder.size()));
}

void pseudo_random_generator_avx2_impl::generate(bit_buffer& data)
{
  if (data.size() < min_nof_bits) {
    pseudo_random_generator_impl::generate(data);
    return;
  }

  detail::pseudo_random_generator_sequence_avx2 sequence(get_state());
  uint8_t*                                      data_ptr = data.get_buffer().data();

  // Processes batches of 256 bits in parallel.
  unsigned nof_bytes = (data.size() / AVX2_SIZE_BIT) * AVX2_SIZE_BYTE;
  for (unsigned i_byte = 0; i_byte != nof_bytes; i_byte += AVX2_SIZE_BYTE) {
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(data_ptr + i_byte), sequence.step());
  }

  // Hands off to the bit-wise implementation, which reads x1 and x2.
  init(sequence.get_state());

  // Process spare bits in a batch of the remainder bits.
  bit_buffer data_remainder = data.last(data.size() - (nof_bytes / AVX2_SIZE_BYTE) * AVX2_SIZE_BIT);
  pseudo_random_generator_impl::generate(data_remainder);
}
