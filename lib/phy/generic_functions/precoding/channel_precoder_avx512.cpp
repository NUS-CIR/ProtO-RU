// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "channel_precoder_avx512.h"
#include "ocudu/adt/format.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocuduvec/copy.h"
#include <algorithm>
#include <immintrin.h>

using namespace ocudu;

/// Size of an AVX512 register in complex numbers with 32-bit floating point precision.
static constexpr unsigned AVX512_CF_SIZE = 8;

/// Number of resource elements processed per iteration by the layer mapping and precoding loops.
static constexpr unsigned AVX512_LAYER_MAP_NOF_RE = 16;

/// Scaling applied to the ci8_t symbols when they are placed in the most significant byte of a 32-bit integer.
static constexpr float CI8_MSB_SCALING = 1.0F / static_cast<float>(1U << 24U);

namespace {

/// Representation of a set of complex numbers using a pair of AVX512 registers, for the real and imaginary parts.
struct simd_cf_t {
  __m512 re;
  __m512 im;

  /// Sets the registers using a complex constant.
  void set1(const cf_t a)
  {
    re = _mm512_set1_ps(a.real());
    im = _mm512_set1_ps(a.imag());
  }
};

/// Type to hold a set of complex numbers using an AVX512 register, with interleaved real and imaginary parts.
using simd_cf_interleaved = __m512;

} // namespace

/// Multiplies two sets of complex numbers stored in split real and imaginary registers.
static simd_cf_t operator*(const simd_cf_t& in, const simd_cf_t& weight)
{
  return {_mm512_fmsub_ps(in.re, weight.re, _mm512_mul_ps(in.im, weight.im)),
          _mm512_fmadd_ps(in.re, weight.im, _mm512_mul_ps(in.im, weight.re))};
}

/// Multiplies two sets of complex numbers stored in split registers and accumulates the result.
static void simd_cmac(simd_cf_t& acc, const simd_cf_t& in, const simd_cf_t& weight)
{
  acc.re = _mm512_fnmadd_ps(in.im, weight.im, _mm512_fmadd_ps(in.re, weight.re, acc.re));
  acc.im = _mm512_fmadd_ps(in.im, weight.re, _mm512_fmadd_ps(in.re, weight.im, acc.im));
}

#ifndef __AVX512BF16__
/// Rounds single precision floating point numbers to the nearest even brain floating point in the upper 16 bits.
static __m512i simd_f_round_to_bf16(__m512 in)
{
  const __m512i bias = _mm512_set1_epi32(0x7fff);
  const __m512i one  = _mm512_set1_epi32(0x1);

  __m512i a_i32 = _mm512_castps_si512(in);
  return _mm512_add_epi32(a_i32, _mm512_add_epi32(bias, _mm512_and_si512(_mm512_srli_epi32(a_i32, 16), one)));
}
#endif // __AVX512BF16__

/// Converts a register with interleaved complex numbers into eight consecutive cbf16_t.
static __m256i simd_cf_to_cbf16(simd_cf_interleaved in)
{
#ifdef __AVX512BF16__
  return (__m256i)_mm512_cvtneps_pbh(in);
#else  // __AVX512BF16__
  // Round to nearest even, shift right 16 bits and pack.
  return _mm512_cvtepi32_epi16(_mm512_srli_epi32(simd_f_round_to_bf16(in), 16));
#endif // __AVX512BF16__
}

/// Converts two registers with interleaved complex numbers into sixteen consecutive cbf16_t.
static __m512i simd_cf_to_cbf16(simd_cf_interleaved in0, simd_cf_interleaved in1)
{
#ifdef __AVX512BF16__
  // The first operand is converted into the upper half of the result.
  return (__m512i)_mm512_cvtne2ps_pbh(in1, in0);
#else  // __AVX512BF16__
  // The logical shift leaves values within the unsigned 16-bit range, so the unsigned saturation is lossless.
  __m512i packed = _mm512_packus_epi32(_mm512_srli_epi32(simd_f_round_to_bf16(in0), 16),
                                       _mm512_srli_epi32(simd_f_round_to_bf16(in1), 16));

  // Restore the order of the 64-bit words after the in-lane packing.
  return _mm512_permutexvar_epi64(_mm512_setr_epi64(0, 2, 4, 6, 1, 3, 5, 7), packed);
#endif // __AVX512BF16__
}

/// Converts complex numbers stored in split real and imaginary registers into sixteen consecutive cbf16_t.
static __m512i simd_cf_to_cbf16(const simd_cf_t& in)
{
#ifdef __AVX512BF16__
  // Convert the real parts into the lower half and the imaginary parts into the upper half, then interleave them.
  alignas(64) static constexpr int16_t idx[32] = {0, 16, 1, 17, 2,  18, 3,  19, 4,  20, 5,  21, 6,  22, 7,  23,
                                                  8, 24, 9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31};
  return _mm512_permutexvar_epi16(_mm512_load_si512(idx), (__m512i)_mm512_cvtne2ps_pbh(in.im, in.re));
#else  // __AVX512BF16__
  // The real part goes to the lower 16 bits and the imaginary part to the upper 16 bits of each 32-bit word.
  return _mm512_mask_blend_epi16(
      0xaaaaaaaa, _mm512_srli_epi32(simd_f_round_to_bf16(in.re), 16), simd_f_round_to_bf16(in.im));
#endif // __AVX512BF16__
}

void channel_precoder_avx512::apply_precoding_port(span<cbf16_t>             port_re,
                                                   const re_buffer_reader<>& input_re,
                                                   span<const cf_t>          port_weights) const
{
  unsigned nof_re     = input_re.get_nof_re();
  unsigned nof_layers = input_re.get_nof_slices();

  // Create a list of all the input layer RE views.
  std::array<span<const cf_t>, precoding_constants::MAX_NOF_LAYERS> layer_re_view_list;

  // Array holding SIMD registers initialized with the precoding weights. The sign of the imaginary part of the weights
  // is negated in the even positions, so the products by the swapped input are added to the result.
  std::array<simd_cf_t, precoding_constants::MAX_NOF_LAYERS> port_weights_simd;

  for (unsigned i_layer = 0; i_layer != nof_layers; ++i_layer) {
    // Fill the RE view list.
    layer_re_view_list[i_layer] = input_re.get_slice(i_layer);

    // Set the SIMD registers with the port weights.
    port_weights_simd[i_layer].set1(port_weights[i_layer]);
    port_weights_simd[i_layer].im = _mm512_castsi512_ps(_mm512_xor_si512(
        _mm512_castps_si512(port_weights_simd[i_layer].im), _mm512_set1_epi64(static_cast<int64_t>(0x80000000))));
  }

  // Applies the port weights to AVX512_CF_SIZE consecutive RE. The products by the real and imaginary parts of the
  // weights are accumulated separately and combined at the end.
  auto precode = [&layer_re_view_list, &port_weights_simd, nof_layers](unsigned i_re) {
    simd_cf_interleaved re_in  = _mm512_loadu_ps(reinterpret_cast<const float*>(&layer_re_view_list[0][i_re]));
    simd_cf_interleaved acc_re = _mm512_mul_ps(re_in, port_weights_simd[0].re);
    simd_cf_interleaved acc_im = _mm512_mul_ps(_mm512_permute_ps(re_in, 0xb1), port_weights_simd[0].im);

    for (unsigned i_layer = 1; i_layer != nof_layers; ++i_layer) {
      re_in  = _mm512_loadu_ps(reinterpret_cast<const float*>(&layer_re_view_list[i_layer][i_re]));
      acc_re = _mm512_fmadd_ps(re_in, port_weights_simd[i_layer].re, acc_re);
      acc_im = _mm512_fmadd_ps(_mm512_permute_ps(re_in, 0xb1), port_weights_simd[i_layer].im, acc_im);
    }

    return _mm512_add_ps(acc_re, acc_im);
  };

  unsigned i_re = 0;

  for (unsigned max_re = (nof_re / (2 * AVX512_CF_SIZE)) * (2 * AVX512_CF_SIZE); i_re != max_re;
       i_re += 2 * AVX512_CF_SIZE) {
    _mm512_storeu_si512(reinterpret_cast<__m512i*>(&port_re[i_re]),
                        simd_cf_to_cbf16(precode(i_re), precode(i_re + AVX512_CF_SIZE)));
  }

  if (nof_re - i_re >= AVX512_CF_SIZE) {
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(&port_re[i_re]), simd_cf_to_cbf16(precode(i_re)));
    i_re += AVX512_CF_SIZE;
  }

  for (; i_re != nof_re; ++i_re) {
    cf_t sum = layer_re_view_list[0][i_re] * port_weights[0];
    for (unsigned i_layer = 1; i_layer != nof_layers; ++i_layer) {
      sum += layer_re_view_list[i_layer][i_re] * port_weights[i_layer];
    }
    port_re[i_re] = sum;
  }
}

/// \brief Converts the ci8_t symbol in the lower 16 bits of each 32-bit word into split complex floats.
///
/// Each byte is moved to the most significant byte of the 32-bit word and converted to float. The result is scaled
/// by \ref CI8_MSB_SCALING, which is compensated in the precoding weights.
static simd_cf_t simd_ci8_lo_to_cf(__m512i in)
{
  const __m512i mask = _mm512_set1_epi32(0xff000000);
  return {_mm512_cvtepi32_ps(_mm512_slli_epi32(in, 24)),
          _mm512_cvtepi32_ps(_mm512_and_si512(_mm512_slli_epi32(in, 16), mask))};
}

/// Converts the ci8_t symbol in the upper 16 bits of each 32-bit word into split complex floats.
static simd_cf_t simd_ci8_hi_to_cf(__m512i in)
{
  const __m512i mask = _mm512_set1_epi32(0xff000000);
  return {_mm512_cvtepi32_ps(_mm512_and_si512(_mm512_slli_epi32(in, 8), mask)),
          _mm512_cvtepi32_ps(_mm512_and_si512(in, mask))};
}

namespace {

/// \brief Index tables that gather the symbols of each pair of layers, for five to eight layers.
///
/// A block of sixteen RE holds up to 128 ci8_t symbols, one per layer and RE, loaded into four registers. For each
/// pair of layers, the output register holds one RE per 32-bit word: the even layer in the lower 16 bits and the odd
/// layer in the upper 16 bits. Output slot <tt>2 * i_re + i_half</tt> takes input symbol
/// <tt>NofLayers * i_re + 2 * i_pair + i_half</tt>.
///
/// A two-source permutation addresses only 64 symbols. The index selects the symbol within the first two or the last
/// two registers, and the mask selects which of the two.
///
/// The AVX2 implementation uses a fixed transpose of unpack instructions. See AVX2 \ref simd_layer_n_map_and_ci8_to_cf
/// as a reference.
template <unsigned NofLayers>
struct layer_n_permutation {
  /// Number of output registers, each containing a pair of layers.
  static constexpr unsigned nof_pairs = (NofLayers + 1) / 2;

  /// Symbol indices for each pair of layers.
  alignas(64) int16_t idx[nof_pairs][32] = {};
  /// Mask of the output symbols that are taken from the upper 64 input symbols.
  __mmask32 upper_mask[nof_pairs] = {};

  constexpr layer_n_permutation()
  {
    for (unsigned i_pair = 0; i_pair != nof_pairs; ++i_pair) {
      for (unsigned i_re = 0; i_re != AVX512_LAYER_MAP_NOF_RE; ++i_re) {
        for (unsigned i_half = 0; i_half != 2; ++i_half) {
          unsigned i_symbol = NofLayers * i_re + 2 * i_pair + i_half;
          unsigned i_out    = 2 * i_re + i_half;
          // The last pair of an odd number of layers picks a symbol of the next RE, which is not used.
          i_symbol           = std::min(i_symbol, NofLayers * AVX512_LAYER_MAP_NOF_RE - 1);
          idx[i_pair][i_out] = static_cast<int16_t>(i_symbol % 64);
          if (i_symbol >= 64) {
            upper_mask[i_pair] |= (1U << i_out);
          }
        }
      }
    }
  }
};

} // namespace

/// Loads the input symbols of a register, without reading past the input of sixteen RE.
template <unsigned NofLayers, unsigned IndexRegister>
static __m512i simd_layer_n_load(const ci8_t* in)
{
  static constexpr unsigned nof_symbols = NofLayers * AVX512_LAYER_MAP_NOF_RE;
  static constexpr unsigned offset      = 32 * IndexRegister;
  // Number of input symbols in the register, from zero to 32.
  static constexpr unsigned  nof_valid = (offset < nof_symbols) ? std::min(32U, nof_symbols - offset) : 0;
  static constexpr __mmask32 mask      = (nof_valid == 32) ? 0xffffffffU : ((1U << nof_valid) - 1U);

  // The masked load does not access the memory of the masked-out symbols.
  return _mm512_maskz_loadu_epi16(mask, in + offset);
}

/// \brief Applies layer mapping and converts sixteen RE to split complex floats.
///
/// The generic implementation supports five to eight layers. One to four layers use the specializations below.
template <unsigned NofLayers>
static void simd_layer_n_map_and_ci8_to_cf(simd_cf_t (&out)[NofLayers], const ci8_t* in)
{
  static_assert((NofLayers >= 5) && (NofLayers <= 8), "Invalid number of layers.");
  alignas(64) static constexpr layer_n_permutation<NofLayers> perm;

  __m512i a = simd_layer_n_load<NofLayers, 0>(in);
  __m512i b = simd_layer_n_load<NofLayers, 1>(in);
  __m512i c = simd_layer_n_load<NofLayers, 2>(in);
  __m512i d = simd_layer_n_load<NofLayers, 3>(in);

  for (unsigned i_pair = 0; i_pair != perm.nof_pairs; ++i_pair) {
    __m512i idx   = _mm512_load_si512(perm.idx[i_pair]);
    __m512i lower = _mm512_permutex2var_epi16(a, idx, b);
    __m512i upper = _mm512_permutex2var_epi16(c, idx, d);
    __m512i pair  = _mm512_mask_blend_epi16(perm.upper_mask[i_pair], lower, upper);

    out[2 * i_pair] = simd_ci8_lo_to_cf(pair);
    if (2 * i_pair + 1 < NofLayers) {
      out[2 * i_pair + 1] = simd_ci8_hi_to_cf(pair);
    }
  }
}

/// Applies layer mapping for one layer and converts sixteen symbols to split complex floats.
template <>
void simd_layer_n_map_and_ci8_to_cf<1>(simd_cf_t (&out)[1], const ci8_t* in)
{
  out[0] = simd_ci8_lo_to_cf(_mm512_cvtepu16_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(in))));
}

/// Applies layer mapping for two layers and converts sixteen RE to split complex floats.
template <>
void simd_layer_n_map_and_ci8_to_cf<2>(simd_cf_t (&out)[2], const ci8_t* in)
{
  // Each 32-bit word already contains the symbols of both layers for one RE.
  __m512i tmp = _mm512_loadu_si512(in);

  out[0] = simd_ci8_lo_to_cf(tmp);
  out[1] = simd_ci8_hi_to_cf(tmp);
}

/// Applies layer mapping for three layers and converts sixteen RE to split complex floats.
template <>
void simd_layer_n_map_and_ci8_to_cf<3>(simd_cf_t (&out)[3], const ci8_t* in)
{
  // Load symbols 0 to 31 and symbols 16 to 47. The second load ends on the last input byte, so no data is read past
  // the input.
  __m512i a = _mm512_loadu_si512(in);
  __m512i b = _mm512_loadu_si512(in + 16);

  // Indices of the 16-bit symbols for layers 0 and 1, and for layer 2, of REs 0 to 15. Symbols 32 to 47 are selected
  // from the second register with indices 48 to 63. The upper 16 bits of each layer 2 word are not used.
  alignas(64) static constexpr int16_t idx_l01[32] = {0,  1,  3,  4,  6,  7,  9,  10, 12, 13, 15, 16, 18, 19, 21, 22,
                                                      24, 25, 27, 28, 30, 31, 49, 50, 52, 53, 55, 56, 58, 59, 61, 62};
  alignas(64) static constexpr int16_t idx_l2[32]  = {2,  2,  5,  5,  8,  8,  11, 11, 14, 14, 17, 17, 20, 20, 23, 23,
                                                      26, 26, 29, 29, 48, 48, 51, 51, 54, 54, 57, 57, 60, 60, 63, 63};

  __m512i l01 = _mm512_permutex2var_epi16(a, _mm512_load_si512(idx_l01), b);
  __m512i l2  = _mm512_permutex2var_epi16(a, _mm512_load_si512(idx_l2), b);

  out[0] = simd_ci8_lo_to_cf(l01);
  out[1] = simd_ci8_hi_to_cf(l01);
  out[2] = simd_ci8_lo_to_cf(l2);
}

/// Applies layer mapping for four layers and converts sixteen RE to split complex floats.
template <>
void simd_layer_n_map_and_ci8_to_cf<4>(simd_cf_t (&out)[4], const ci8_t* in)
{
  __m512i a = _mm512_loadu_si512(in);
  __m512i b = _mm512_loadu_si512(in + 32);

  // Gather layers 0 and 1, and layers 2 and 3, for REs 0 to 15.
  __m512i l01 =
      _mm512_permutex2var_epi32(a, _mm512_setr_epi32(0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30), b);
  __m512i l23 =
      _mm512_permutex2var_epi32(a, _mm512_setr_epi32(1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31), b);

  out[0] = simd_ci8_lo_to_cf(l01);
  out[1] = simd_ci8_hi_to_cf(l01);
  out[2] = simd_ci8_lo_to_cf(l23);
  out[3] = simd_ci8_hi_to_cf(l23);
}

/// \brief Applies layer mapping and precoding to one block of \ref AVX512_LAYER_MAP_NOF_RE RE.
///
/// \param[out] outputs Precoded RE of each port. The block is stored starting at \c i_re.
/// \param[in]  i_re    Index of the first RE of the block within each port.
/// \param[in]  input   Layer-interleaved symbols of the block.
/// \param[in]  weights Precoding weights for each port and layer.
template <unsigned NofLayers>
static void simd_layer_map_and_precode_block(span<span<cbf16_t>> outputs,
                                             unsigned            i_re,
                                             const ci8_t*        input,
                                             const simd_cf_t (*weights)[precoding_constants::MAX_NOF_LAYERS])
{
  // Layer map and convert to complex float.
  simd_cf_t in[NofLayers];
  simd_layer_n_map_and_ci8_to_cf<NofLayers>(in, input);

  for (unsigned i_port = 0, nof_ports = outputs.size(); i_port != nof_ports; ++i_port) {
    // Apply precoding.
    simd_cf_t result = in[0] * weights[i_port][0];
    for (unsigned i_layer = 1; i_layer != NofLayers; ++i_layer) {
      simd_cmac(result, in[i_layer], weights[i_port][i_layer]);
    }

    // Store.
    _mm512_storeu_si512(&outputs[i_port][i_re], simd_cf_to_cbf16(result));
  }
}

/// \brief Applies layer mapping and precoding to all the RE.
///
/// The RE that do not fill a complete block are copied into a zero-padded block that is processed with the same SIMD
/// instructions.
///
/// \param[out] outputs    Precoded RE of each port.
/// \param[in]  input      Layer-interleaved input symbols.
/// \param[in]  weights    Precoding weights for each port and layer.
/// \param[in]  nof_layers Actual number of layers. The call recurses down to the instantiation with \c NofLayers equal
///                        to \c nof_layers.
template <unsigned NofLayers>
static void simd_layer_map_and_precode(span<span<cbf16_t>> outputs,
                                       span<const ci8_t>   input,
                                       const simd_cf_t (*weights)[precoding_constants::MAX_NOF_LAYERS],
                                       unsigned nof_layers)
{
  if (nof_layers != NofLayers) {
    simd_layer_map_and_precode<NofLayers - 1>(outputs, input, weights, nof_layers);
    return;
  }

  unsigned nof_re    = outputs.front().size();
  unsigned i_re_end  = (nof_re / AVX512_LAYER_MAP_NOF_RE) * AVX512_LAYER_MAP_NOF_RE;
  unsigned nof_ports = outputs.size();

  for (unsigned i_re = 0; i_re != i_re_end; i_re += AVX512_LAYER_MAP_NOF_RE) {
    simd_layer_map_and_precode_block<NofLayers>(outputs, i_re, &input[NofLayers * i_re], weights);
  }

  // Number of RE left for processing, less than one block.
  unsigned nof_re_tail = nof_re - i_re_end;
  if (nof_re_tail == 0) {
    return;
  }

  // Zero-padded input, one block.
  std::array<ci8_t, NofLayers * AVX512_LAYER_MAP_NOF_RE> input_padded = {};
  ocuduvec::copy(span<ci8_t>(input_padded).first(NofLayers * nof_re_tail),
                 input.subspan(NofLayers * i_re_end, NofLayers * nof_re_tail));

  // Padded output, one block for each port.
  static_vector<std::array<cbf16_t, AVX512_LAYER_MAP_NOF_RE>, precoding_constants::MAX_NOF_PORTS> outputs_padded(
      nof_ports);
  static_vector<span<cbf16_t>, precoding_constants::MAX_NOF_PORTS> outputs_padded_view(outputs_padded.begin(),
                                                                                       outputs_padded.end());

  // Layer map and precode the padded block.
  simd_layer_map_and_precode_block<NofLayers>(outputs_padded_view, 0, input_padded.data(), weights);

  // Copy the valid RE to the outputs.
  for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
    ocuduvec::copy(outputs[i_port].subspan(i_re_end, nof_re_tail),
                   span<const cbf16_t>(outputs_padded[i_port]).first(nof_re_tail));
  }
}

template <>
void simd_layer_map_and_precode<0>(span<span<cbf16_t>> /* outputs */,
                                   span<const ci8_t> /* input */,
                                   const simd_cf_t (* /* weights */)[precoding_constants::MAX_NOF_LAYERS],
                                   unsigned /* nof_layers */)
{
  // Do nothing.
}

void channel_precoder_avx512::apply_layer_map_and_precoding(re_buffer_writer<cbf16_t>&     output,
                                                            span<const ci8_t>              input,
                                                            const precoding_weight_matrix& precoding) const
{
  unsigned nof_layers = precoding.get_nof_layers();
  unsigned nof_ports  = precoding.get_nof_ports();

  ocudu_assert((nof_layers >= 1) && (nof_layers <= 8), "The number of layers must be in the range [1, 8].");

  // Registers to load the precoding weights, scaled to compensate the ci8_t to float conversion.
  simd_cf_t weights[precoding_constants::MAX_NOF_PORTS][precoding_constants::MAX_NOF_LAYERS];

  // Views to store the precoded symbols.
  std::array<span<cbf16_t>, precoding_constants::MAX_NOF_PORTS> outputs;

  for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
    span<const cf_t> port_coeff = precoding.get_port_coefficients(i_port);
    outputs[i_port]             = output.get_slice(i_port);
    for (unsigned i_layer = 0; i_layer != nof_layers; ++i_layer) {
      weights[i_port][i_layer].set1(port_coeff[i_layer] * CI8_MSB_SCALING);
    }
  }

  span<span<cbf16_t>> port_outputs = span<span<cbf16_t>>(outputs).first(nof_ports);
  simd_layer_map_and_precode<precoding_constants::MAX_NOF_LAYERS>(port_outputs, input, weights, nof_layers);
}
