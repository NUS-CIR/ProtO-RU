// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "channel_precoder_avx2.h"
#include "ocudu/adt/format.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocuduvec/copy.h"
#include <immintrin.h>

using namespace ocudu;

/// Size of an AVX2 register in complex numbers with 32-bit floating point precision.
static constexpr unsigned AVX2_CF_SIZE = 4;

/// Number of resource elements processed per iteration by the layer mapping and precoding loops.
static constexpr unsigned AVX2_LAYER_MAP_NOF_RE = 8;

/// Scaling applied to the ci8_t symbols when they are placed in the most significant byte of a 32-bit integer.
static constexpr float CI8_MSB_SCALING = 1.0F / static_cast<float>(1U << 24U);

namespace {

/// Representation of a set of complex numbers using a pair of AVX2 registers, for the real and imaginary parts.
struct simd_cf_t {
  __m256 re;
  __m256 im;

  /// Sets the registers using a complex constant.
  void set1(const cf_t a)
  {
    re = _mm256_set1_ps(a.real());
    im = _mm256_set1_ps(a.imag());
  }
};

/// Type to hold a set of complex numbers using an AVX2 register, with interleaved real and imaginary parts.
using simd_cf_interleaved = __m256;

} // namespace

/// Multiplies two sets of complex numbers stored in split real and imaginary registers.
static simd_cf_t operator*(const simd_cf_t& in, const simd_cf_t& weight)
{
  return {_mm256_fmsub_ps(in.re, weight.re, _mm256_mul_ps(in.im, weight.im)),
          _mm256_fmadd_ps(in.re, weight.im, _mm256_mul_ps(in.im, weight.re))};
}

/// Multiplies two sets of complex numbers stored in split registers and accumulates the result.
static void simd_cmac(simd_cf_t& acc, const simd_cf_t& in, const simd_cf_t& weight)
{
  acc.re = _mm256_fnmadd_ps(in.im, weight.im, _mm256_fmadd_ps(in.re, weight.re, acc.re));
  acc.im = _mm256_fmadd_ps(in.im, weight.re, _mm256_fmadd_ps(in.re, weight.im, acc.im));
}

/// Rounds single precision floating point numbers to the nearest even brain floating point in the upper 16 bits.
static __m256i simd_f_round_to_bf16(__m256 in)
{
  const __m256i bias = _mm256_set1_epi32(0x7fff);
  const __m256i one  = _mm256_set1_epi32(0x1);

  __m256i a_i32 = _mm256_castps_si256(in);
  return _mm256_add_epi32(a_i32, _mm256_add_epi32(bias, _mm256_and_si256(_mm256_srli_epi32(a_i32, 16), one)));
}

/// Converts a register with interleaved complex numbers into four consecutive c.
static __m128i simd_cf_to_cbf16(simd_cf_interleaved in)
{
  // Round to nearest even and shift right 16 bits.
  __m256i a_i32 = _mm256_srai_epi32(simd_f_round_to_bf16(in), 16);

  // Pack both parts in 32-bit registers.
  return _mm_packs_epi32(_mm256_extractf128_si256(a_i32, 0), _mm256_extractf128_si256(a_i32, 1));
}

/// Converts two registers with interleaved complex numbers into eight consecutive cbf16_t.
static __m256i simd_cf_to_cbf16(simd_cf_interleaved in0, simd_cf_interleaved in1)
{
  // The logical shift leaves values within the unsigned 16-bit range, so the unsigned saturation is lossless.
  __m256i packed = _mm256_packus_epi32(_mm256_srli_epi32(simd_f_round_to_bf16(in0), 16),
                                       _mm256_srli_epi32(simd_f_round_to_bf16(in1), 16));

  // Restore the order of the 64-bit words after the in-lane packing.
  return _mm256_permute4x64_epi64(packed, 0xd8);
}

/// Converts complex numbers stored in split real and imaginary registers into eight consecutive cbf16_t.
static __m256i simd_cf_to_cbf16(const simd_cf_t& in)
{
  // The real part goes to the lower 16 bits and the imaginary part to the upper 16 bits of each 32-bit word.
  return _mm256_blend_epi16(_mm256_srli_epi32(simd_f_round_to_bf16(in.re), 16), simd_f_round_to_bf16(in.im), 0xaa);
}

void channel_precoder_avx2::apply_precoding_port(span<cbf16_t>             port_re,
                                                 const re_buffer_reader<>& input_re,
                                                 span<const cf_t>          port_weights) const
{
  unsigned nof_re     = input_re.get_nof_re();
  unsigned nof_layers = input_re.get_nof_slices();

  // Create a list of all the input layer RE views.
  std::array<span<const cf_t>, precoding_constants::MAX_NOF_LAYERS> layer_re_view_list;

  // Array holding SIMD registers initialized with the precoding weights.
  std::array<simd_cf_t, precoding_constants::MAX_NOF_LAYERS> port_weights_simd;

  for (unsigned i_layer = 0; i_layer != nof_layers; ++i_layer) {
    // Fill the RE view list.
    layer_re_view_list[i_layer] = input_re.get_slice(i_layer);

    // Set the SIMD registers with the port weights.
    port_weights_simd[i_layer].set1(port_weights[i_layer]);
  }

  // Applies the port weights to AVX2_CF_SIZE consecutive RE. The products by the real and imaginary parts of the
  // weights are accumulated separately and combined at the end.
  auto precode = [&layer_re_view_list, &port_weights_simd, nof_layers](unsigned i_re) {
    simd_cf_interleaved re_in  = _mm256_loadu_ps(reinterpret_cast<const float*>(&layer_re_view_list[0][i_re]));
    simd_cf_interleaved acc_re = _mm256_mul_ps(re_in, port_weights_simd[0].re);
    simd_cf_interleaved acc_im = _mm256_mul_ps(_mm256_shuffle_ps(re_in, re_in, 0xb1), port_weights_simd[0].im);

    for (unsigned i_layer = 1; i_layer != nof_layers; ++i_layer) {
      re_in  = _mm256_loadu_ps(reinterpret_cast<const float*>(&layer_re_view_list[i_layer][i_re]));
      acc_re = _mm256_fmadd_ps(re_in, port_weights_simd[i_layer].re, acc_re);
      acc_im = _mm256_fmadd_ps(_mm256_shuffle_ps(re_in, re_in, 0xb1), port_weights_simd[i_layer].im, acc_im);
    }

    return _mm256_addsub_ps(acc_re, acc_im);
  };

  unsigned i_re = 0;

  for (unsigned max_re = (nof_re / (2 * AVX2_CF_SIZE)) * (2 * AVX2_CF_SIZE); i_re != max_re; i_re += 2 * AVX2_CF_SIZE) {
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(&port_re[i_re]),
                        simd_cf_to_cbf16(precode(i_re), precode(i_re + AVX2_CF_SIZE)));
  }

  if (nof_re - i_re >= AVX2_CF_SIZE) {
    _mm_storeu_si128(reinterpret_cast<__m128i*>(&port_re[i_re]), simd_cf_to_cbf16(precode(i_re)));
    i_re += AVX2_CF_SIZE;
  }

  for (; i_re != nof_re; ++i_re) {
    cf_t sum = layer_re_view_list[0][i_re] * port_weights[0];

    for (unsigned i_layer = 1; i_layer != nof_layers; ++i_layer) {
      sum += layer_re_view_list[i_layer][i_re] * port_weights[i_layer];
    }
    port_re[i_re] = sum;
  }
}

/// Loads two unaligned 128-bit words into the lower and higher lanes of an AVX2 register.
static __m256i simd_loadu2_si128(const int8_t* hi, const int8_t* lo)
{
  return _mm256_inserti128_si256(_mm256_castsi128_si256(_mm_loadu_si128(reinterpret_cast<const __m128i*>(lo))),
                                 _mm_loadu_si128(reinterpret_cast<const __m128i*>(hi)),
                                 1);
}

/// \brief Converts the ci8_t symbol in the lower 16 bits of each 32-bit word into split complex floats.
///
/// Each byte is moved to the most significant byte of the 32-bit word and converted to float. The result is scaled
/// by \ref CI8_MSB_SCALING, which is compensated in the precoding weights.
static simd_cf_t simd_ci8_lo_to_cf(__m256i in)
{
  const __m256i mask = _mm256_set1_epi32(0xff000000);
  return {_mm256_cvtepi32_ps(_mm256_slli_epi32(in, 24)),
          _mm256_cvtepi32_ps(_mm256_and_si256(_mm256_slli_epi32(in, 16), mask))};
}

/// Converts the ci8_t symbol in the upper 16 bits of each 32-bit word into split complex floats.
static simd_cf_t simd_ci8_hi_to_cf(__m256i in)
{
  const __m256i mask = _mm256_set1_epi32(0xff000000);
  return {_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_slli_epi32(in, 8), mask)),
          _mm256_cvtepi32_ps(_mm256_and_si256(in, mask))};
}

/// \brief Applies layer mapping and converts eight RE to split complex floats.
///
/// The generic implementation supports five to eight layers. The symbols of each RE are loaded with a 16-byte load.
/// For fewer than eight layers, the load also reads the first symbols of the next RE, see
/// \ref simd_layer_map_nof_guard_re. The specializations for one to four layers do not read past the eight RE.
///
/// \tparam     NofLayers Number of layers.
/// \param[out] out       Split complex floats, one register per layer, each holding the eight RE of that layer. The
///                       values are scaled by \ref CI8_MSB_SCALING.
/// \param[in]  in        Layer-interleaved ci8_t symbols of the eight RE, layer index varying fastest.
template <unsigned NofLayers>
static void simd_layer_n_map_and_ci8_to_cf(simd_cf_t (&out)[NofLayers], const ci8_t* in)
{
  static_assert((NofLayers >= 5) && (NofLayers <= 8), "Invalid number of layers.");

  // Number of bytes per RE in the input array.
  static constexpr unsigned nof_bytes_per_re = sizeof(ci8_t) * NofLayers;

  const int8_t* in8    = reinterpret_cast<const int8_t*>(in);
  auto          get_re = [in8](unsigned i_re) { return in8 + nof_bytes_per_re * i_re; };

  // Load RE j in the lower lane and RE j + 4 in the upper lane, so the in-lane transpose returns the RE in order.
  __m256i v0 = simd_loadu2_si128(get_re(4), get_re(0));
  __m256i v1 = simd_loadu2_si128(get_re(5), get_re(1));
  __m256i v2 = simd_loadu2_si128(get_re(6), get_re(2));
  __m256i v3 = simd_loadu2_si128(get_re(7), get_re(3));

  // Transpose the 32-bit words, each word containing one symbol for two consecutive layers.
  __m256i t0 = _mm256_unpacklo_epi32(v0, v1);
  __m256i t1 = _mm256_unpackhi_epi32(v0, v1);
  __m256i t2 = _mm256_unpacklo_epi32(v2, v3);
  __m256i t3 = _mm256_unpackhi_epi32(v2, v3);

  __m256i l01 = _mm256_unpacklo_epi64(t0, t2);
  __m256i l23 = _mm256_unpackhi_epi64(t0, t2);
  __m256i l45 = _mm256_unpacklo_epi64(t1, t3);

  out[0] = simd_ci8_lo_to_cf(l01);
  out[1] = simd_ci8_hi_to_cf(l01);
  out[2] = simd_ci8_lo_to_cf(l23);
  out[3] = simd_ci8_hi_to_cf(l23);
  out[4] = simd_ci8_lo_to_cf(l45);
  if (NofLayers > 5) {
    out[5] = simd_ci8_hi_to_cf(l45);
  }
  if (NofLayers > 6) {
    __m256i l67 = _mm256_unpackhi_epi64(t1, t3);
    out[6]      = simd_ci8_lo_to_cf(l67);
    if (NofLayers == 8) {
      out[7] = simd_ci8_hi_to_cf(l67);
    }
  }
}

/// Number of RE past the processed RE that \ref simd_layer_n_map_and_ci8_to_cf may read.
template <unsigned NofLayers>
static constexpr unsigned simd_layer_map_nof_guard_re = ((NofLayers >= 5) && (NofLayers < 8)) ? 1 : 0;

/// Applies layer mapping for one layer and converts eight symbols to split complex floats.
template <>
void simd_layer_n_map_and_ci8_to_cf<1>(simd_cf_t (&out)[1], const ci8_t* in)
{
  out[0] = simd_ci8_lo_to_cf(_mm256_cvtepu16_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(in))));
}

/// Applies layer mapping for two layers and converts eight RE to split complex floats.
template <>
void simd_layer_n_map_and_ci8_to_cf<2>(simd_cf_t (&out)[2], const ci8_t* in)
{
  // Each 32-bit word already contains the symbols of both layers for one RE.
  __m256i tmp = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(in));

  out[0] = simd_ci8_lo_to_cf(tmp);
  out[1] = simd_ci8_hi_to_cf(tmp);
}

/// Applies layer mapping for three layers and converts eight RE to split complex floats.
template <>
void simd_layer_n_map_and_ci8_to_cf<3>(simd_cf_t (&out)[3], const ci8_t* in)
{
  // Number of bytes per RE in the input array.
  static constexpr unsigned nof_bytes_per_re = sizeof(ci8_t) * 3;

  const int8_t* in8    = reinterpret_cast<const int8_t*>(in);
  auto          get_re = [in8](unsigned i_re) { return in8 + nof_bytes_per_re * i_re; };

  // Load REs 0, 1, 4 and 5 starting at the beginning of each lane, and REs 2, 3, 6 and 7 starting four bytes into each
  // lane. The second load ends on the last input byte, so no data is read past the input.
  __m256i x = simd_loadu2_si128(get_re(4), get_re(0));
  __m256i y = simd_loadu2_si128(get_re(6) - 4, get_re(2) - 4);

  // Arrange each lane as the symbols of layers 0 and 1 of two consecutive RE followed by the symbols of layer 2. The
  // indices of y are offset by four bytes, and the zeroing indices keep the most significant bit set after the offset.
  const __m256i idx_x =
      _mm256_broadcastsi128_si256(_mm_setr_epi8(0, 1, 2, 3, 6, 7, 8, 9, 4, 5, -128, -128, 10, 11, -128, -128));
  const __m256i idx_y = _mm256_add_epi8(idx_x, _mm256_set1_epi8(4));
  x                   = _mm256_shuffle_epi8(x, idx_x);
  y                   = _mm256_shuffle_epi8(y, idx_y);

  // Gather layers 0 and 1, and layer 2, for REs 0 to 7.
  __m256  xf  = _mm256_castsi256_ps(x);
  __m256  yf  = _mm256_castsi256_ps(y);
  __m256i l01 = _mm256_castps_si256(_mm256_shuffle_ps(xf, yf, 0x44));
  __m256i l2  = _mm256_castps_si256(_mm256_shuffle_ps(xf, yf, 0xee));

  out[0] = simd_ci8_lo_to_cf(l01);
  out[1] = simd_ci8_hi_to_cf(l01);
  out[2] = simd_ci8_lo_to_cf(l2);
}

/// Applies layer mapping for four layers and converts eight RE to split complex floats.
template <>
void simd_layer_n_map_and_ci8_to_cf<4>(simd_cf_t (&out)[4], const ci8_t* in)
{
  // Number of bytes per RE in the input array.
  static constexpr unsigned nof_bytes_per_re = sizeof(ci8_t) * 4;

  const int8_t* in8    = reinterpret_cast<const int8_t*>(in);
  auto          get_re = [in8](unsigned i_re) { return in8 + nof_bytes_per_re * i_re; };

  // Load REs 0, 1, 4 and 5, and REs 2, 3, 6 and 7, so that the in-lane shuffles return the RE in order.
  __m256 a = _mm256_castsi256_ps(simd_loadu2_si128(get_re(4), get_re(0)));
  __m256 b = _mm256_castsi256_ps(simd_loadu2_si128(get_re(6), get_re(2)));

  // Gather layers 0 and 1, and layers 2 and 3, for REs 0 to 7.
  __m256i l01 = _mm256_castps_si256(_mm256_shuffle_ps(a, b, 0x88));
  __m256i l23 = _mm256_castps_si256(_mm256_shuffle_ps(a, b, 0xdd));

  out[0] = simd_ci8_lo_to_cf(l01);
  out[1] = simd_ci8_hi_to_cf(l01);
  out[2] = simd_ci8_lo_to_cf(l23);
  out[3] = simd_ci8_hi_to_cf(l23);
}

/// \brief Applies layer mapping and precoding to one block of \ref AVX2_LAYER_MAP_NOF_RE RE.
///
/// \param[out] outputs Precoded RE of each port. The block is stored starting at \c i_re.
/// \param[in]  i_re    Index of the first RE of the block within each port.
/// \param[in]  input   Layer-interleaved symbols of the block, followed by \ref simd_layer_map_nof_guard_re RE.
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
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(&outputs[i_port][i_re]), simd_cf_to_cbf16(result));
  }
}

/// \brief Applies layer mapping and precoding to all the RE.
///
/// The RE that do not fill a complete block, including those the layer mapping may read past, are copied into a
/// zero-padded block that is processed with the same SIMD instructions.
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

  static constexpr unsigned nof_guard_re = simd_layer_map_nof_guard_re<NofLayers>;

  unsigned nof_re      = outputs.front().size();
  unsigned nof_re_simd = (nof_re > nof_guard_re) ? (nof_re - nof_guard_re) : 0;
  unsigned i_re_end    = (nof_re_simd / AVX2_LAYER_MAP_NOF_RE) * AVX2_LAYER_MAP_NOF_RE;
  unsigned nof_ports   = outputs.size();

  for (unsigned i_re = 0; i_re != i_re_end; i_re += AVX2_LAYER_MAP_NOF_RE) {
    simd_layer_map_and_precode_block<NofLayers>(outputs, i_re, &input[NofLayers * i_re], weights);
  }

  // Number of RE left for processing, at most one block.
  unsigned nof_re_tail = nof_re - i_re_end;
  if (nof_re_tail == 0) {
    return;
  }

  // Zero-padded input, one block plus the RE that the layer mapping may read past the block.
  static constexpr unsigned             nof_padded_symbols = NofLayers * (AVX2_LAYER_MAP_NOF_RE + nof_guard_re);
  std::array<ci8_t, nof_padded_symbols> input_padded       = {};

  // Copy the remaining REs in the input to the padded input buffer.
  span<const ci8_t> input_tail = input.subspan(NofLayers * i_re_end, NofLayers * nof_re_tail);
  ocuduvec::copy(span<ci8_t>(input_padded).first(input_tail.size()), input_tail);

  // Padded output, one block for each port.
  using padded_block = std::array<cbf16_t, AVX2_LAYER_MAP_NOF_RE>;
  static_vector<padded_block, precoding_constants::MAX_NOF_PORTS> outputs_padded(nof_ports);

  // Build a vector containing a view of each output padded block per port.
  static_vector<span<cbf16_t>, precoding_constants::MAX_NOF_PORTS> outputs_padded_views(outputs_padded.begin(),
                                                                                        outputs_padded.end());

  // Layer map and precode the padded block.
  simd_layer_map_and_precode_block<NofLayers>(outputs_padded_views, 0, input_padded.data(), weights);

  // Copy the valid RE to the outputs.
  for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
    span<cbf16_t> port_tail = outputs[i_port].subspan(i_re_end, nof_re_tail);
    ocuduvec::copy(port_tail, outputs_padded_views[i_port].first(port_tail.size()));
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

void channel_precoder_avx2::apply_layer_map_and_precoding(re_buffer_writer<cbf16_t>&     output,
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
