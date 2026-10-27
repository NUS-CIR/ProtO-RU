// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ran/resource_block.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <immintrin.h>

namespace ocudu {
namespace ofh {
namespace mm512 {

/// \brief Packs the 16bit IQ samples of one AVX512 register using 9bit big-endian format.
///
/// Each 128bit lane of the input holds the four I samples followed by the four Q samples of four REs (the layout of
/// \ref quantize_bf16_even_odd). The output is the bit stream I0 Q0 I1 Q1 I2 Q2 I3 Q3, 9 bits per sample and most
/// significant bit first, stored in the first 9 bytes of the respective lane of the output register. The remaining 7
/// bytes of each lane are zero.
///
/// \param[in] samples_epi16 AVX512 register storing 16bit IQ samples, only the lower 9 bits of each are packed.
/// \return An AVX512 register containing 9 packed bytes in each 128bit lane.
inline __m512i pack_avx512_register_9b_be(__m512i samples_epi16)
{
  // Shift the j-th sample of the stream left by 7 - j, so that its two bytes become the bits it contributes to the
  // output bytes j and j + 1. The bits above the ninth one are discarded by the selection below.
  //                                                 I0 I1 I2 I3 Q0 Q1 Q2 Q3
  const __m512i shift_epi16   = _mm512_set4_epi32(0x00000002, 0x00040006, 0x00010003, 0x00050007);
  __m512i       shifted_epi16 = _mm512_sllv_epi16(samples_epi16, shift_epi16);

  // Output byte j takes its lowest 8 - j bits from the high byte of the j-th sample of the stream and the rest from the
  // low byte of the previous one, byte 8 is the low byte of the last sample.
  const __m512i high_bytes_epi8 =
      _mm512_set4_epi64(0xffffffffffffffff, 0x0f070d050b030901, 0xffffffffffffffff, 0x0f070d050b030901);
  const __m512i low_bytes_epi8 =
      _mm512_set4_epi64(0xffffffffffffff0e, 0x060c040a020800ff, 0xffffffffffffff0e, 0x060c040a020800ff);
  const __m512i select_low_epi8 =
      _mm512_set4_epi64(0x00000000000000ff, 0xfefcf8f0e0c08000, 0x00000000000000ff, 0xfefcf8f0e0c08000);
  return _mm512_ternarylogic_epi64(_mm512_shuffle_epi8(shifted_epi16, high_bytes_epi8),
                                   _mm512_shuffle_epi8(shifted_epi16, low_bytes_epi8),
                                   select_low_epi8,
                                   0xd8);
}

/// \brief Packs the 16bit IQ samples of one AVX512 register using 14bit big-endian format.
///
/// Same input layout and output format as \ref pack_avx512_register_9b_be, with 14 packed bytes per 128bit lane.
///
/// \param[in] samples_epi16 AVX512 register storing 16bit IQ samples, only the lower 14 bits of each are packed.
/// \return An AVX512 register containing 14 packed bytes in each 128bit lane.
inline __m512i pack_avx512_register_14b_be(__m512i samples_epi16)
{
  // Restore the stream order of the samples, so that each 32bit element holds one RE as I in its lower half and Q in
  // its upper half:
  // |            byte3          |            byte2        |               byte1        |           byte0          |
  // | s s q13 q12 q11 q10 q9 q8 | q7 q6 q5 q4 q3 q2 q1 q0 | s  s i13 i12 i11 i10 i9 i8 | i7 i6 i5 i4 i3 i2  i1 i0 |
  const __m512i interleave_epi8 =
      _mm512_set4_epi64(0x0f0e07060d0c0504, 0x0b0a030209080100, 0x0f0e07060d0c0504, 0x0b0a030209080100);
  __m512i re_epi32 = _mm512_shuffle_epi8(samples_epi16, interleave_epi8);

  // Each sample of the stream starts 14 bits after the previous one, so the bits of consecutive samples are aligned to
  // the output bytes by two shifts, one per element of the RE:
  // - shift left by 2 (first RE) or 14 (second RE), giving the I component;
  // - shift right by 4 (first RE) or 16 (second RE), giving the Q component.
  // | 0 0 0 0 0   0   0   0   | 0   0   0  0  0  0  0  0  |i13 i12 i11 i10 i9  i8  i7 i6 | i5 i4 i3 i2 i1 i0 0  0 |
  // | 0 0 0 0 0   0   q13 q12 | q11 q10 q9 q8 q7 q6 q5 q4 |q3  q2  q1  q0  0   0   0  0  | 0  0  0  0  0  0  0  0 |
  // | 0 0 0 0 i13 i12 i11 i10 | i9  i8  i7 i6 i5 i4 i3 i2 |i1  i0  0   0   0   0   0  0  | 0  0  0  0  0  0  0  0 |
  // | 0 0 0 0 0   0   0   0   | 0   0   0  0  0  0  0  0  |0   0   q13 q12 q11 q10 q9 q8 | q7 q6 q5 q4 q3 q2 q1 q0|
  const __m512i shl_epi32 = _mm512_set4_epi32(14, 2, 14, 2);
  const __m512i shr_epi32 = _mm512_set4_epi32(16, 4, 16, 4);
  __m512i       i_epi32   = _mm512_sllv_epi32(re_epi32, shl_epi32);
  __m512i       q_epi32   = _mm512_srlv_epi32(re_epi32, shr_epi32);

  // Gather the bytes of both in big-endian order and select the bits of each from the component they belong to, which
  // discards the bits of the other component left by the shifts.
  const __m512i i_bytes_epi8 =
      _mm512_set4_epi64(0xffff0c0d0e0fff08, 0x0904050607ff0001, 0xffff0c0d0e0fff08, 0x0904050607ff0001);
  const __m512i q_bytes_epi8 =
      _mm512_set4_epi64(0xffff0c0d0e090a0b, 0x0804050601020300, 0xffff0c0d0e090a0b, 0x0804050601020300);
  const __m512i select_q_epi8 =
      _mm512_set4_epi64(0x0000ff3f00f0ff03, 0x00ff3f00f0ff0300, 0x0000ff3f00f0ff03, 0x00ff3f00f0ff0300);
  return _mm512_ternarylogic_epi64(
      _mm512_shuffle_epi8(i_epi32, i_bytes_epi8), _mm512_shuffle_epi8(q_epi32, q_bytes_epi8), select_q_epi8, 0xd8);
}

/// \brief Byte layout of four packed PRBs in the output buffer.
///
/// The packed bytes of the 128bit lanes of the three registers (see the file brief of avx512_helpers.h) are stored
/// consecutively, each PRB being preceded by its compression parameter if present:
/// \code
///              PRB0 (RB0)            |             PRB1 (RB1)            |
/// [param] lane0 lane1 lane2 of reg 0 | [param] lane3 of reg 0, lane0 lane1 of reg 1 | ...
/// \endcode
///
/// \tparam BYTES_PER_LANE Number of packed bytes of each 128bit lane (eight samples).
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte.
template <unsigned BYTES_PER_LANE, bool HAS_COMP_PARAM>
struct prb_quad_layout {
  /// Size in bytes of one packed PRB.
  static constexpr unsigned PRB_SIZE = (HAS_COMP_PARAM ? 1 : 0) + 3 * BYTES_PER_LANE;
  /// Index of the first byte to permute, so that the compression parameter is taken from the second source.
  static constexpr uint8_t COMP_PARAM_INDEX = 64;

  /// Offset of the packed bytes of one of the twelve lanes of the quad.
  static constexpr unsigned lane_offset(unsigned lane)
  {
    return (lane / 3) * PRB_SIZE + (HAS_COMP_PARAM ? 1 : 0) + (lane % 3) * BYTES_PER_LANE;
  }

  /// Offset of the first byte written from the given register.
  static constexpr unsigned reg_begin(unsigned reg) { return (reg == 0) ? 0 : lane_offset(4 * reg); }

  /// Offset after the last byte written from the given register.
  static constexpr unsigned reg_end(unsigned reg) { return (reg == 2) ? 4 * PRB_SIZE : reg_begin(reg + 1); }

  /// \brief Byte permutation that gathers the output bytes of the given register.
  ///
  /// The output bytes of a register are the ones from \ref reg_begin to \ref reg_end. The permutation takes the packed
  /// bytes from the lanes of the register (first source) and the compression parameters from the first byte of the
  /// respective lane of the second source.
  static constexpr std::array<uint8_t, 64> make_permutation(unsigned reg)
  {
    std::array<uint8_t, 64> index = {};
    for (unsigned i = 0, end = reg_end(reg) - reg_begin(reg); i != end; ++i) {
      unsigned offset = reg_begin(reg) + i;
      if (HAS_COMP_PARAM && (offset % PRB_SIZE == 0)) {
        index[i] = COMP_PARAM_INDEX + 16 * (offset / PRB_SIZE);
        continue;
      }
      for (unsigned lane = 4 * reg; lane != 4 * reg + 4; ++lane) {
        if ((offset >= lane_offset(lane)) && (offset < lane_offset(lane) + BYTES_PER_LANE)) {
          index[i] = 16 * (lane - 4 * reg) + (offset - lane_offset(lane));
        }
      }
    }
    return index;
  }

  /// Store mask of the output bytes of the given register, limited to the first \c nof_prbs PRBs of the quad.
  static __mmask64 store_mask(unsigned reg, unsigned nof_prbs)
  {
    unsigned begin = reg_begin(reg);
    unsigned end   = std::min(reg_end(reg), nof_prbs * PRB_SIZE);
    return (end > begin) ? (~uint64_t{0} >> (64 - (end - begin))) : 0;
  }
};

/// \brief Packs four PRBs using 9bit or 14bit big-endian format.
///
/// \tparam BITS           Bit width of the packed samples, 9 or 14.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte.
/// \param[out] out             Output bytes of the quad of PRBs, see \ref prb_quad_layout.
/// \param[in] r0_epi16         AVX512 register storing the samples of the first and second RBs (see the file brief of
///                             avx512_helpers.h), with the layout of \ref quantize_bf16_even_odd in each lane.
/// \param[in] r1_epi16         AVX512 register storing the samples of the second and third RBs.
/// \param[in] r2_epi16         AVX512 register storing the samples of the third and fourth RBs.
/// \param[in] comp_param_epi16 AVX512 register whose 128bit lanes hold the compression parameter of the respective
///                             PRB in their first byte. Ignored if \c HAS_COMP_PARAM is false.
/// \param[in] nof_prbs         Number of PRBs of the quad to write, from 1 to 4.
template <unsigned BITS, bool HAS_COMP_PARAM>
inline void pack_prb_quad_big_endian(uint8_t* out,
                                     __m512i  r0_epi16,
                                     __m512i  r1_epi16,
                                     __m512i  r2_epi16,
                                     __m512i  comp_param_epi16,
                                     unsigned nof_prbs)
{
  static_assert((BITS == 9) || (BITS == 14), "Unsupported bit width");
  using layout = prb_quad_layout<(BITS == 9) ? 9 : 14, HAS_COMP_PARAM>;

  static constexpr std::array<uint8_t, 64> index0 = layout::make_permutation(0);
  static constexpr std::array<uint8_t, 64> index1 = layout::make_permutation(1);
  static constexpr std::array<uint8_t, 64> index2 = layout::make_permutation(2);

  // Pack the samples of each lane into its first bytes.
  auto pack = [](__m512i samples_epi16) {
    return (BITS == 9) ? pack_avx512_register_9b_be(samples_epi16) : pack_avx512_register_14b_be(samples_epi16);
  };
  __m512i p0_epi8 = pack(r0_epi16);
  __m512i p1_epi8 = pack(r1_epi16);
  __m512i p2_epi8 = pack(r2_epi16);

  // Gather the output bytes of each register, including the compression parameters, and store them.
  __m512i o0_epi8 = _mm512_permutex2var_epi8(p0_epi8, _mm512_loadu_si512(index0.data()), comp_param_epi16);
  __m512i o1_epi8 = _mm512_permutex2var_epi8(p1_epi8, _mm512_loadu_si512(index1.data()), comp_param_epi16);
  __m512i o2_epi8 = _mm512_permutex2var_epi8(p2_epi8, _mm512_loadu_si512(index2.data()), comp_param_epi16);
  _mm512_mask_storeu_epi8(out + layout::reg_begin(0), layout::store_mask(0, nof_prbs), o0_epi8);
  _mm512_mask_storeu_epi8(out + layout::reg_begin(1), layout::store_mask(1, nof_prbs), o1_epi8);
  _mm512_mask_storeu_epi8(out + layout::reg_begin(2), layout::store_mask(2, nof_prbs), o2_epi8);
}

/// \brief Byte permutation that restores the stream order of 16bit samples and converts them to big-endian.
///
/// It applies to the first three 128bit lanes of a register (one PRB) with the layout of \ref quantize_bf16_even_odd,
/// and leaves room for the compression parameter in the first byte of the output, taken from the second source.
///
/// \tparam HAS_COMP_PARAM Whether the PRB starts with a compression parameter byte.
/// \param[in] prb Index of the PRB in the quad, which selects the lane of the compression parameter.
template <bool HAS_COMP_PARAM>
constexpr std::array<uint8_t, 64> make_permutation_16b(unsigned prb)
{
  std::array<uint8_t, 64> index = {};
  unsigned                i     = 0;
  if (HAS_COMP_PARAM) {
    index[i++] = 64 + 16 * prb;
  }
  for (unsigned lane = 0; lane != 3; ++lane) {
    for (unsigned sample = 0; sample != 8; ++sample) {
      // The even samples of the stream are the I components, the odd ones the Q components.
      unsigned element = (sample % 2 == 0) ? (sample / 2) : (4 + sample / 2);
      index[i++]       = 16 * lane + 2 * element + 1;
      index[i++]       = 16 * lane + 2 * element;
    }
  }
  return index;
}

/// \brief Packs four PRBs using 16bit big-endian format.
///
/// Same parameters as \ref pack_prb_quad_big_endian.
template <bool HAS_COMP_PARAM>
inline void pack_prb_quad_16b_big_endian(uint8_t* out,
                                         __m512i  r0_epi16,
                                         __m512i  r1_epi16,
                                         __m512i  r2_epi16,
                                         __m512i  comp_param_epi16,
                                         unsigned nof_prbs)
{
  static constexpr unsigned                PRB_SIZE = (HAS_COMP_PARAM ? 1 : 0) + 48;
  static constexpr std::array<uint8_t, 64> index0   = make_permutation_16b<HAS_COMP_PARAM>(0);
  static constexpr std::array<uint8_t, 64> index1   = make_permutation_16b<HAS_COMP_PARAM>(1);
  static constexpr std::array<uint8_t, 64> index2   = make_permutation_16b<HAS_COMP_PARAM>(2);
  static constexpr std::array<uint8_t, 64> index3   = make_permutation_16b<HAS_COMP_PARAM>(3);
  static constexpr __mmask64               mask     = (uint64_t{1} << PRB_SIZE) - 1;

  // Move the samples of each PRB to the first three lanes of a register, and write the PRBs one by one.
  __m512i rb1_epi16 = _mm512_alignr_epi64(r1_epi16, r0_epi16, 6);
  __m512i rb2_epi16 = _mm512_alignr_epi64(r2_epi16, r1_epi16, 4);
  __m512i rb3_epi16 = _mm512_alignr_epi64(r2_epi16, r2_epi16, 2);
  _mm512_mask_storeu_epi8(
      out, mask, _mm512_permutex2var_epi8(r0_epi16, _mm512_loadu_si512(index0.data()), comp_param_epi16));
  if (nof_prbs > 1) {
    _mm512_mask_storeu_epi8(
        out + PRB_SIZE, mask, _mm512_permutex2var_epi8(rb1_epi16, _mm512_loadu_si512(index1.data()), comp_param_epi16));
  }
  if (nof_prbs > 2) {
    _mm512_mask_storeu_epi8(out + 2 * PRB_SIZE,
                            mask,
                            _mm512_permutex2var_epi8(rb2_epi16, _mm512_loadu_si512(index2.data()), comp_param_epi16));
  }
  if (nof_prbs > 3) {
    _mm512_mask_storeu_epi8(out + 3 * PRB_SIZE,
                            mask,
                            _mm512_permutex2var_epi8(rb3_epi16, _mm512_loadu_si512(index3.data()), comp_param_epi16));
  }
}

/// \brief Layout of a quad of PRBs unpacked into 32bit elements, see \ref unpack_prb_quad_pair.
///
/// The packed bytes of the quad are loaded into source registers of 64 bytes. The 96 samples of the quad are spread
/// over three pairs of AVX512 registers of 16 32bit elements, each one holding a sample. The two registers of a pair
/// hold the even and odd samples of the same 32 consecutive samples of the quad. The 32 samples of a pair are gathered
/// from at most two source registers, as 16bit words unless some sample spans more than two bytes.
///
/// \tparam BITS           Bit width of the packed samples.
/// \tparam HAS_COMP_PARAM Whether each PRB starts with a compression parameter byte.
template <unsigned BITS, bool HAS_COMP_PARAM>
struct prb_quad_unpack_layout {
  static_assert((BITS > 8) && (BITS <= 16), "The samples must fit in 16bit words and span at least two bytes");

  /// Bit width of the packed samples.
  static constexpr unsigned SAMPLE_BITS = BITS;
  /// Scaling of the unpacked samples: the 16bit ones are left at the top of their 32bit elements.
  static constexpr float SAMPLE_SCALE = (BITS == 16) ? 0x1p-16F : 1.0F;
  /// Number of samples of a PRB.
  static constexpr unsigned NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;
  /// Size in bytes of one packed PRB, including the compression parameter if present.
  static constexpr unsigned PRB_SIZE = 3 * BITS + (HAS_COMP_PARAM ? 1 : 0);
  /// Size in bytes of the quad.
  static constexpr unsigned QUAD_SIZE = 4 * PRB_SIZE;
  /// Size in bytes of a source register.
  static constexpr unsigned SOURCE_SIZE = 64;
  /// Number of source registers.
  static constexpr unsigned NOF_SOURCES = (QUAD_SIZE + SOURCE_SIZE - 1) / SOURCE_SIZE;
  /// Number of register pairs.
  static constexpr unsigned NOF_PAIRS = 3;
  /// Number of samples of a register pair.
  static constexpr unsigned NOF_SAMPLES_PER_PAIR = 32;
  /// Byte offset of the samples inside of a PRB.
  static constexpr unsigned SAMPLES_OFFSET = HAS_COMP_PARAM ? 1 : 0;

  /// Position of a packed sample in the quad.
  struct sample_position {
    /// Index of the first byte holding bits of the sample.
    unsigned first_byte;
    /// Offset of the most significant bit of the sample inside of the first byte.
    unsigned bit_offset;
  };

  /// Returns the position of the given sample of the quad.
  static constexpr sample_position locate_sample(unsigned sample)
  {
    unsigned prb = sample / NOF_SAMPLES_PER_PRB;
    unsigned bit = (prb * PRB_SIZE + SAMPLES_OFFSET) * 8 + (sample % NOF_SAMPLES_PER_PRB) * BITS;
    return {bit / 8, bit % 8};
  }

  /// Returns the mask of the bytes of the given source register that belong to the given number of PRBs.
  static constexpr __mmask64 source_mask(unsigned source, unsigned nof_prbs)
  {
    unsigned nof_bytes =
        std::min(SOURCE_SIZE, nof_prbs * PRB_SIZE - std::min(nof_prbs * PRB_SIZE, source * SOURCE_SIZE));
    return (nof_bytes == SOURCE_SIZE) ? ~__mmask64{0} : ((__mmask64{1} << nof_bytes) - 1);
  }

  /// Returns the indices of the two source registers holding the bytes of the given register pair, the second one
  /// equals the first if a single register is enough.
  static constexpr std::array<unsigned, 2> pair_sources(unsigned pair)
  {
    unsigned first = locate_sample(pair * NOF_SAMPLES_PER_PAIR).first_byte / SOURCE_SIZE;
    unsigned last  = (locate_sample((pair + 1) * NOF_SAMPLES_PER_PAIR - 1).first_byte + 1) / SOURCE_SIZE;
    return (last <= first + 1) ? std::array<unsigned, 2>{first, last} : throw "Pair spans more than two sources";
  }

  /// Returns the byte permutation gathering the two bytes holding the most significant bits of each sample of the
  /// given register pair into its 16bit word, in big-endian, from the concatenation of its two source registers.
  static constexpr std::array<uint8_t, 64> make_gather(unsigned pair)
  {
    std::array<uint8_t, 64> gather = {};
    unsigned                base   = pair_sources(pair)[0] * SOURCE_SIZE;
    for (unsigned word = 0; word != NOF_SAMPLES_PER_PAIR; ++word) {
      unsigned first_byte  = locate_sample(pair * NOF_SAMPLES_PER_PAIR + word).first_byte;
      gather[word * 2 + 1] = first_byte - base;
      gather[word * 2]     = first_byte + 1 - base;
    }
    return gather;
  }

  /// Returns the left shift aligning the most significant bit of each sample of the given register pair with the one
  /// of its 16bit word.
  static constexpr std::array<int16_t, 32> make_shifts(unsigned pair)
  {
    std::array<int16_t, 32> shifts = {};
    for (unsigned word = 0; word != NOF_SAMPLES_PER_PAIR; ++word) {
      shifts[word] = locate_sample(pair * NOF_SAMPLES_PER_PAIR + word).bit_offset;
    }
    return shifts;
  }

  /// Returns true if some sample of the given register pair does not start at a byte boundary.
  static constexpr bool needs_shift(unsigned pair)
  {
    for (int16_t shift : make_shifts(pair)) {
      if (shift != 0) {
        return true;
      }
    }
    return false;
  }

  /// Returns true if some sample of the given register pair spans more than two bytes, so that the samples must be
  /// gathered as 32bit elements.
  static constexpr bool needs_dword_gather(unsigned pair)
  {
    for (int16_t shift : make_shifts(pair)) {
      if (shift + BITS > 16) {
        return true;
      }
    }
    return false;
  }

  /// Returns the byte permutation gathering the four bytes starting at the first one of each even or odd sample of
  /// the given register pair into its 32bit element, in big-endian, from the concatenation of its two source
  /// registers. The bytes beyond the quad are replaced by the last one, they only hold bits below the sample.
  static constexpr std::array<uint8_t, 64> make_dword_gather(unsigned pair, unsigned parity)
  {
    std::array<uint8_t, 64> gather    = {};
    std::array<unsigned, 2> sources   = pair_sources(pair);
    unsigned                base      = sources[0] * SOURCE_SIZE;
    unsigned                last_byte = std::min((sources[1] + 1) * SOURCE_SIZE, QUAD_SIZE) - 1;
    for (unsigned element = 0; element != 16; ++element) {
      unsigned first_byte = locate_sample(pair * NOF_SAMPLES_PER_PAIR + 2 * element + parity).first_byte;
      for (unsigned byte = 0; byte != 4; ++byte) {
        gather[element * 4 + 3 - byte] = std::min(first_byte + byte, last_byte) - base;
      }
    }
    return gather;
  }

  /// Returns the left shift aligning the most significant bit of each even or odd sample of the given register pair
  /// with the one of its 32bit element.
  static constexpr std::array<int32_t, 16> make_dword_shifts(unsigned pair, unsigned parity)
  {
    std::array<int32_t, 16> shifts = {};
    for (unsigned element = 0; element != 16; ++element) {
      shifts[element] = locate_sample(pair * NOF_SAMPLES_PER_PAIR + 2 * element + parity).bit_offset;
    }
    return shifts;
  }

  /// Returns the index of the PRB of the sample held by each 32bit element of the registers of the given pair, which
  /// is the same for the even and odd samples.
  static constexpr std::array<int32_t, 16> make_prb_indices(unsigned pair)
  {
    std::array<int32_t, 16> indices = {};
    for (unsigned element = 0; element != 16; ++element) {
      indices[element] = (pair * NOF_SAMPLES_PER_PAIR + 2 * element) / NOF_SAMPLES_PER_PRB;
    }
    return indices;
  }

  /// Returns the mask of the PRBs whose compression parameter is held by the given source register.
  static constexpr __mmask64 comp_param_mask(unsigned source)
  {
    __mmask64 mask = 0;
    for (unsigned prb = 0; prb != 4; ++prb) {
      if (prb * PRB_SIZE / SOURCE_SIZE == source) {
        mask |= __mmask64{1} << (4 * prb);
      }
    }
    return mask;
  }

  /// Returns the byte permutation gathering the compression parameter of each PRB held by the given source
  /// registers into the first byte of the respective 32bit element, from the concatenation of both registers.
  static constexpr std::array<uint8_t, 64> make_comp_param_gather(unsigned first_source, unsigned second_source)
  {
    std::array<uint8_t, 64> gather = {};
    for (unsigned prb = 0; prb != 4; ++prb) {
      unsigned source = prb * PRB_SIZE / SOURCE_SIZE;
      gather[4 * prb] = (prb * PRB_SIZE) % SOURCE_SIZE + ((source == second_source) ? SOURCE_SIZE : 0);
    }
    return gather;
  }
};

/// AVX512 register wrapper, as the vector types cannot be arguments of \c std::array.
struct avx512_register {
  __m512i value;
};

/// Source registers of a quad of PRBs, see \ref prb_quad_unpack_layout.
template <typename LAYOUT>
using prb_quad_sources = std::array<avx512_register, LAYOUT::NOF_SOURCES>;

/// \brief Loads the packed bytes of a quad of PRBs into its source registers.
///
/// \tparam LAYOUT Layout of the quad, see \ref prb_quad_unpack_layout.
/// \param[out] sources  Source registers of the quad.
/// \param[in] in        Packed bytes of the quad.
/// \param[in] nof_prbs  Number of PRBs of the quad, the bytes beyond them are not read.
template <typename LAYOUT>
inline void load_prb_quad(prb_quad_sources<LAYOUT>& sources, const uint8_t* in, unsigned nof_prbs)
{
  for (unsigned source = 0; source != LAYOUT::NOF_SOURCES; ++source) {
    const uint8_t* address = in + source * LAYOUT::SOURCE_SIZE;
    __mmask64      mask    = (nof_prbs == 4) ? LAYOUT::source_mask(source, 4) : LAYOUT::source_mask(source, nof_prbs);
    sources[source].value =
        (mask == ~__mmask64{0}) ? _mm512_loadu_si512(address) : _mm512_maskz_loadu_epi8(mask, address);
  }
}

/// \brief Unpacks the samples of one register pair of a quad of PRBs into 32bit integers.
///
/// \tparam LAYOUT Layout of the quad, see \ref prb_quad_unpack_layout.
/// \tparam PAIR   Index of the register pair.
/// \param[out] even_epi32 AVX512 register storing the 16 even samples, multiplied by the inverse of
///                        \c LAYOUT::SAMPLE_SCALE.
/// \param[out] odd_epi32  AVX512 register storing the 16 odd samples, multiplied by the inverse of
///                        \c LAYOUT::SAMPLE_SCALE.
/// \param[in] sources     Source registers of the quad, see \ref load_prb_quad.
template <typename LAYOUT, unsigned PAIR>
inline void unpack_prb_quad_pair(__m512i& even_epi32, __m512i& odd_epi32, const prb_quad_sources<LAYOUT>& sources)
{
  static constexpr auto SOURCES = LAYOUT::pair_sources(PAIR);

  // Gathers bytes from the two source registers of the pair.
  auto gather = [&](const std::array<uint8_t, 64>& indices) {
    __m512i indices_epi8 = _mm512_loadu_si512(indices.data());
    if constexpr (SOURCES[0] == SOURCES[1]) {
      return _mm512_permutexvar_epi8(indices_epi8, sources[SOURCES[0]].value);
    } else {
      return _mm512_permutex2var_epi8(sources[SOURCES[0]].value, indices_epi8, sources[SOURCES[1]].value);
    }
  };

  if constexpr (LAYOUT::needs_dword_gather(PAIR)) {
    // Gather the bytes of the even and odd samples into separate registers, align the most significant bit of each
    // sample with the one of its element and sign extend.
    static constexpr auto EVEN_GATHER = LAYOUT::make_dword_gather(PAIR, 0);
    static constexpr auto ODD_GATHER  = LAYOUT::make_dword_gather(PAIR, 1);
    static constexpr auto EVEN_SHIFTS = LAYOUT::make_dword_shifts(PAIR, 0);
    static constexpr auto ODD_SHIFTS  = LAYOUT::make_dword_shifts(PAIR, 1);
    even_epi32 = _mm512_srai_epi32(_mm512_sllv_epi32(gather(EVEN_GATHER), _mm512_loadu_si512(EVEN_SHIFTS.data())),
                                   32 - LAYOUT::SAMPLE_BITS);
    odd_epi32  = _mm512_srai_epi32(_mm512_sllv_epi32(gather(ODD_GATHER), _mm512_loadu_si512(ODD_SHIFTS.data())),
                                  32 - LAYOUT::SAMPLE_BITS);
    return;
  }

  // Gather the two bytes holding the most significant bits of each sample into its word.
  static constexpr auto GATHER        = LAYOUT::make_gather(PAIR);
  static constexpr auto SHIFTS        = LAYOUT::make_shifts(PAIR);
  __m512i               samples_epi16 = gather(GATHER);

  // Shift out the bits of the previous sample.
  if constexpr (LAYOUT::needs_shift(PAIR)) {
    samples_epi16 = _mm512_sllv_epi16(samples_epi16, _mm512_loadu_si512(SHIFTS.data()));
  }

  if constexpr (LAYOUT::SAMPLE_BITS == 16) {
    // The samples fill their words, keep them at the top of the elements.
    even_epi32 = _mm512_slli_epi32(samples_epi16, 16);
    odd_epi32  = _mm512_and_si512(samples_epi16, _mm512_set1_epi32(0xffff0000));
  } else {
    // Sign extend the samples from the top of their words, the bits of the following sample are shifted out.
    even_epi32 = _mm512_srai_epi32(_mm512_slli_epi32(samples_epi16, 16), 32 - LAYOUT::SAMPLE_BITS);
    odd_epi32  = _mm512_srai_epi32(samples_epi16, 32 - LAYOUT::SAMPLE_BITS);
  }
}

/// \brief Gathers the compression parameters of a quad of PRBs.
///
/// \tparam LAYOUT Layout of the quad, see \ref prb_quad_unpack_layout.
/// \param[in] sources Source registers of the quad, see \ref load_prb_quad.
/// \return An AVX512 register whose first four 32bit elements hold the compression parameter of the respective PRB.
template <typename LAYOUT>
inline __m512i gather_prb_quad_comp_params(const prb_quad_sources<LAYOUT>& sources)
{
  // The first two source registers are combined by a single permutation, the rest are merged one at a time.
  static constexpr __mmask64 FIRST_MASK        = LAYOUT::comp_param_mask(0) | LAYOUT::comp_param_mask(1);
  static constexpr auto      FIRST_GATHER      = LAYOUT::make_comp_param_gather(0, 1);
  __m512i                    comp_params_epi32 = _mm512_maskz_permutex2var_epi8(FIRST_MASK,
                                                             sources[0].value,
                                                             _mm512_loadu_si512(FIRST_GATHER.data()),
                                                             sources[std::min(1U, LAYOUT::NOF_SOURCES - 1)].value);
  for (unsigned source = 2; source < LAYOUT::NOF_SOURCES; ++source) {
    // The gather of each source register is a compile time constant, as the loop is unrolled.
    if (LAYOUT::comp_param_mask(source) != 0) {
      comp_params_epi32 =
          _mm512_mask_permutexvar_epi8(comp_params_epi32,
                                       LAYOUT::comp_param_mask(source),
                                       _mm512_loadu_si512(LAYOUT::make_comp_param_gather(source, source).data()),
                                       sources[source].value);
    }
  }
  return comp_params_epi32;
}

/// \brief Checks whether the requested bit width is supported by the AVX512 implementation.
/// \param[in] iq_width Requested bit width.
///
/// \return True in case packing/unpacking with the requested bit width is supported.
inline bool iq_width_packing_supported(unsigned iq_width)
{
  return ((iq_width == 9) || (iq_width == 14) || (iq_width == 16));
}

} // namespace mm512
} // namespace ofh
} // namespace ocudu
