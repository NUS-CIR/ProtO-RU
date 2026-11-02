// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "channel_precoder_neon.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocuduvec/copy.h"
#include <arm_neon.h>

using namespace ocudu;

/// Size of a NEON register in complex numbers with 32-bit floating point precision.
static constexpr unsigned NEON_CF_SIZE = 4;

/// Size of a NEON register in complex numbers with 8-bit fixed point precision.
static constexpr unsigned NEON_CI8_SIZE = 16;

namespace {

/// Representation of a set of complex numbers using a pair of NEON registers, for the real and imaginary parts.
struct simd_cf_t {
  float32x4_t re;
  float32x4_t im;

  /// Sets the registers using a complex constant.
  void set1(const cf_t a)
  {
    re = vdupq_n_f32(a.real());
    im = vdupq_n_f32(a.imag());
  }
};

/// Type to hold a set of complex numbers using a NEON register, with interleaved real and imaginary parts.
using simd_cf_interleaved = float32x4x2_t;

} // namespace

static simd_cf_interleaved operator+(simd_cf_interleaved a, simd_cf_interleaved b)
{
  simd_cf_interleaved ret;
  ret.val[0] = vaddq_f32(a.val[0], b.val[0]);
  ret.val[1] = vaddq_f32(a.val[1], b.val[1]);
  return ret;
}

/// Multiplication operator for the precoding weights.
static simd_cf_interleaved operator*(const simd_cf_interleaved& re, const simd_cf_t& weight)
{
  simd_cf_interleaved ret;
  ret.val[0] = vmlsq_f32(vmulq_f32(re.val[0], weight.re), re.val[1], weight.im);
  ret.val[1] = vmlaq_f32(vmulq_f32(re.val[0], weight.im), re.val[1], weight.re);
  return ret;
}

static simd_cf_interleaved
add_mul(const simd_cf_interleaved& sum, const simd_cf_interleaved& value, const simd_cf_t& weight)
{
  simd_cf_interleaved ret;
  ret.val[0] = vmlsq_f32(vmlaq_f32(sum.val[0], value.val[0], weight.re), value.val[1], weight.im);
  ret.val[1] = vmlaq_f32(vmlaq_f32(sum.val[1], value.val[0], weight.im), value.val[1], weight.re);
  return ret;
}

static uint16x8_t cf_to_cbf16(simd_cf_interleaved in)
{
  const uint32x4_t bias = vdupq_n_u32(0x7fff);
  const uint32x4_t one  = vdupq_n_u32(0x1);

  // Reinterpret the 32-bit single-precision input as unsigned 32-bit integer.
  uint32x4_t a_u32 = vreinterpretq_u32_f32(in.val[0]);
  uint32x4_t b_u32 = vreinterpretq_u32_f32(in.val[1]);

  // Round to nearest even.
  a_u32 = vaddq_u32(a_u32, vaddq_u32(bias, vandq_u32(vshrq_n_u32(a_u32, 16), one)));
  b_u32 = vaddq_u32(b_u32, vaddq_u32(bias, vandq_u32(vshrq_n_u32(b_u32, 16), one)));

  // Remove the 16 least significant bits of the fractional part.
  a_u32 = vshrq_n_u32(a_u32, 16);
  b_u32 = vandq_u32(b_u32, vdupq_n_u32(0xffff0000));

  // Combine real and imaginary parts.
  return vreinterpretq_u16_u32(vorrq_u32(a_u32, b_u32));
}

void channel_precoder_neon::apply_precoding_port(span<cbf16_t>             port_re,
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

  unsigned i_re = 0;

  for (unsigned max_re = (nof_re / NEON_CF_SIZE) * NEON_CF_SIZE; i_re != max_re; i_re += NEON_CF_SIZE) {
    // Load layer 0 RE.
    simd_cf_interleaved re_in = vld2q_f32(reinterpret_cast<const float*>(&layer_re_view_list[0][i_re]));

    // Multiply the input REs by the precoding coefficient.
    simd_cf_interleaved re_out = re_in * port_weights_simd[0];

    for (unsigned i_layer = 1; i_layer != nof_layers; ++i_layer) {
      // Load layer RE.
      re_in = vld2q_f32(reinterpret_cast<const float*>(&layer_re_view_list[i_layer][i_re]));

      // Multiply the input REs by the coefficient and add to the contributions of other layers.
      re_out = add_mul(re_out, re_in, port_weights_simd[i_layer]);
    }

    // Store.
    vst1q_u16(reinterpret_cast<uint16_t*>(&port_re[i_re]), cf_to_cbf16(re_out));
  }

  for (; i_re != nof_re; ++i_re) {
    cf_t sum = layer_re_view_list[0][i_re] * port_weights[0];

    for (unsigned i_layer = 1; i_layer != nof_layers; ++i_layer) {
      sum += layer_re_view_list[i_layer][i_re] * port_weights[i_layer];
    }

    port_re[i_re] = sum;
  }
}

static void from_ci8_to_ci16(int16x8x2_t& out0, int16x8x2_t& out1, int8x16x2_t in)
{
  out0.val[0] = vmovl_s8(vget_low_s8(in.val[0]));
  out0.val[1] = vmovl_s8(vget_low_s8(in.val[1]));

  out1.val[0] = vmovl_high_s8(in.val[0]);
  out1.val[1] = vmovl_high_s8(in.val[1]);
}

static void from_ci16_to_ci32(int32x4x2_t& out0, int32x4x2_t& out1, int16x8x2_t in)
{
  out0.val[0] = vmovl_s16(vget_low_s16(in.val[0]));
  out0.val[1] = vmovl_s16(vget_low_s16(in.val[1]));

  out1.val[0] = vmovl_high_s16(in.val[0]);
  out1.val[1] = vmovl_high_s16(in.val[1]);
}

static void from_ci32_to_cf(float32x4x2_t& out, int32x4x2_t in)
{
  out.val[0] = vcvtq_f32_s32(in.val[0]);
  out.val[1] = vcvtq_f32_s32(in.val[1]);
}

/// Converts ci8_t symbols into cf_t and stores them on four NEON registers.
static void from_ci8_to_cf(simd_cf_interleaved& out0,
                           simd_cf_interleaved& out1,
                           simd_cf_interleaved& out2,
                           simd_cf_interleaved& out3,
                           const int8x16x2_t    in)
{
  // Convert input to ci16.
  int16x8x2_t in_ci16_0, in_ci16_1;
  from_ci8_to_ci16(in_ci16_0, in_ci16_1, in);

  // Convert input to ci32.
  int32x4x2_t in_ci32_0, in_ci32_1, in_ci32_2, in_ci32_3;
  from_ci16_to_ci32(in_ci32_0, in_ci32_1, in_ci16_0);
  from_ci16_to_ci32(in_ci32_2, in_ci32_3, in_ci16_1);

  // Convert input to cf.
  from_ci32_to_cf(out0, in_ci32_0);
  from_ci32_to_cf(out1, in_ci32_1);
  from_ci32_to_cf(out2, in_ci32_2);
  from_ci32_to_cf(out3, in_ci32_3);
}

/// \brief Number of RE processed by \ref layer_n_map_and_ci8_to_cf.
///
/// The RE are processed in blocks of \ref NEON_CF_SIZE RE. The number of RE fills the loaded registers for one and two
/// layers.
template <unsigned NofLayers>
static constexpr unsigned neon_layer_map_nof_re = (NofLayers == 1) ? NEON_CI8_SIZE : ((NofLayers == 2) ? 8 : 4);

/// Number of bytes read by \ref layer_n_map_and_ci8_to_cf.
template <unsigned NofLayers>
static constexpr unsigned neon_layer_map_load_size = (NofLayers <= 4) ? 32 : 64;

/// Number of RE past the processed RE that \ref layer_n_map_and_ci8_to_cf may read.
template <unsigned NofLayers>
static constexpr unsigned neon_layer_map_nof_guard_re =
    (neon_layer_map_load_size<NofLayers> - 2 * NofLayers * neon_layer_map_nof_re<NofLayers> + 2 * NofLayers - 1) /
    (2 * NofLayers);

/// Number of registers produced by \ref layer_n_map_and_ci8_to_cf: one per layer and block of \ref NEON_CF_SIZE RE.
template <unsigned NofLayers>
static constexpr unsigned neon_layer_map_nof_out = (neon_layer_map_nof_re<NofLayers> / NEON_CF_SIZE) * NofLayers;

namespace {

/// \brief Table lookup indices for the layer mapping of five to eight layers.
///
/// Each table gathers the real or imaginary parts of four layers for four RE, ordered by layer and then by RE. Layers
/// beyond the number of layers use an out-of-range index, so the lookup sets them to zero.
template <unsigned NofLayers>
struct layer_n_lookup {
  /// Indices of the real (0) and imaginary (1) parts for layers 0 to 3 (0) and layers 4 to 7 (1).
  uint8_t idx[2][2][NEON_CI8_SIZE] = {};

  constexpr layer_n_lookup()
  {
    for (unsigned i_group = 0; i_group != 2; ++i_group) {
      for (unsigned i_part = 0; i_part != 2; ++i_part) {
        for (unsigned i_layer = 0; i_layer != 4; ++i_layer) {
          for (unsigned i_re = 0; i_re != NEON_CF_SIZE; ++i_re) {
            unsigned layer = 4 * i_group + i_layer;
            idx[i_group][i_part][NEON_CF_SIZE * i_layer + i_re] =
                (layer < NofLayers) ? static_cast<uint8_t>(2 * (NofLayers * i_re + layer) + i_part) : 0xff;
          }
        }
      }
    }
  }
};

} // namespace

/// \brief Applies layer mapping and converts \ref neon_layer_map_nof_re RE to cf_t.
///
/// The output contains one register per layer for each block of \ref NEON_CF_SIZE RE, ordered by block and then by
/// layer. The generic implementation supports five to eight layers. One to four layers use the specializations below.
template <unsigned NofLayers>
static void layer_n_map_and_ci8_to_cf(simd_cf_interleaved (&out)[neon_layer_map_nof_out<NofLayers>], const ci8_t* in)
{
  static_assert((NofLayers >= 5) && (NofLayers <= 8), "Invalid number of layers.");
  static constexpr layer_n_lookup<NofLayers> lookup;

  int8x16x4_t in8 = vld1q_s8_x4(reinterpret_cast<const int8_t*>(in));

  // Layers 0 to 3.
  int8x16x2_t l0123 = {{vqtbl4q_s8(in8, vld1q_u8(lookup.idx[0][0])), vqtbl4q_s8(in8, vld1q_u8(lookup.idx[0][1]))}};

  // Layers 4 to 7.
  int8x16x2_t l4567 = {{vqtbl4q_s8(in8, vld1q_u8(lookup.idx[1][0])), vqtbl4q_s8(in8, vld1q_u8(lookup.idx[1][1]))}};

  simd_cf_interleaved tmp[precoding_constants::MAX_NOF_LAYERS];
  from_ci8_to_cf(tmp[0], tmp[1], tmp[2], tmp[3], l0123);
  from_ci8_to_cf(tmp[4], tmp[5], tmp[6], tmp[7], l4567);
  for (unsigned i_layer = 0; i_layer != NofLayers; ++i_layer) {
    out[i_layer] = tmp[i_layer];
  }
}

/// Converts sixteen symbols of one layer to cf_t.
template <>
void layer_n_map_and_ci8_to_cf<1>(simd_cf_interleaved (&out)[neon_layer_map_nof_out<1>], const ci8_t* in)
{
  int8x16x2_t in8 = vld2q_s8(reinterpret_cast<const int8_t*>(in));
  from_ci8_to_cf(out[0], out[1], out[2], out[3], in8);
}

/// Applies layer mapping for two layers and converts eight RE to cf_t.
template <>
void layer_n_map_and_ci8_to_cf<2>(simd_cf_interleaved (&out)[neon_layer_map_nof_out<2>], const ci8_t* in)
{
  int8x16x2_t in8 = vld2q_s8(reinterpret_cast<const int8_t*>(in));

  // The input is de-interleaved in real and imaginary, this step interleaves them again to produce the linear input in
  // temp in an array of words of 16bits.
  int16_t temp[NEON_CI8_SIZE];
  vst2q_s8(reinterpret_cast<int8_t*>(temp), in8);

  // Load the temporary data with de-interleaving words of 16 bits and store them de-interleaved.
  int16x8x2_t deinterleaved0 = vld2q_s16(temp);
  vst1q_s16(&temp[0], deinterleaved0.val[0]);
  vst1q_s16(&temp[8], deinterleaved0.val[1]);

  // Load back to de-interleaved real and imaginary.
  int8x16x2_t layers = vld2q_s8(reinterpret_cast<const int8_t*>(temp));

  // Layer 0 of both blocks precedes layer 1 of both blocks.
  from_ci8_to_cf(out[0], out[2], out[1], out[3], layers);
}

/// Applies layer mapping for three layers and converts four RE to cf_t.
template <>
void layer_n_map_and_ci8_to_cf<3>(simd_cf_interleaved (&out)[neon_layer_map_nof_out<3>], const ci8_t* in)
{
  simd_cf_interleaved unused;

  int8x16x2_t in8 = vld2q_s8(reinterpret_cast<const int8_t*>(in));

  ci8_t temp_in[NEON_CI8_SIZE];
  ci8_t temp_out[NEON_CI8_SIZE];
  vst2q_s8(reinterpret_cast<int8_t*>(temp_in), in8);
  unsigned k = 0;
  for (unsigned i_layer = 0; i_layer != 3; ++i_layer) {
    for (unsigned i_re = 0; i_re != NEON_CF_SIZE; ++i_re) {
      temp_out[k++] = temp_in[3 * i_re + i_layer];
    }
  }
  for (unsigned i_re = 0; i_re != NEON_CF_SIZE; ++i_re) {
    temp_out[k++] = 0;
  }
  int8x16x2_t layers = vld2q_s8(reinterpret_cast<const int8_t*>(temp_out));

  from_ci8_to_cf(out[0], out[1], out[2], unused, layers);
}

/// Applies layer mapping for four layers and converts four RE to cf_t.
template <>
void layer_n_map_and_ci8_to_cf<4>(simd_cf_interleaved (&out)[neon_layer_map_nof_out<4>], const ci8_t* in)
{
  int8x16x2_t in8 = vld2q_s8(reinterpret_cast<const int8_t*>(in));

  // The input is de-interleaved in real and imaginary, this step interleaves them again to produce the linear input in
  // temp in an array of words of 16bits.
  int16_t temp[NEON_CI8_SIZE];
  vst2q_s8(reinterpret_cast<int8_t*>(temp), in8);

  // Load the temporary data with de-interleaving words of 16 bits and store them de-interleaved.
  int16x8x2_t deinterleaved0 = vld2q_s16(temp);
  vst1q_s16(&temp[0], deinterleaved0.val[0]);
  vst1q_s16(&temp[8], deinterleaved0.val[1]);

  // Repeat de-interleaving.
  int16x8x2_t deinterleaved1 = vld2q_s16(temp);
  vst1q_s16(&temp[0], deinterleaved1.val[0]);
  vst1q_s16(&temp[8], deinterleaved1.val[1]);

  // Load back to de-interleaved real and imaginary.
  int8x16x2_t layers = vld2q_s8(reinterpret_cast<const int8_t*>(temp));

  from_ci8_to_cf(out[0], out[1], out[2], out[3], layers);
}

/// \brief Applies layer mapping and precoding to one block of \ref neon_layer_map_nof_re RE.
///
/// \param[out] outputs Precoded RE of each port. The block is stored starting at \c i_re.
/// \param[in]  i_re    Index of the first RE of the block within each port.
/// \param[in]  input   Layer-interleaved symbols of the block, followed by \ref neon_layer_map_nof_guard_re RE.
/// \param[in]  weights Precoding weights for each port and layer.
template <unsigned NofLayers>
static void layer_n_map_and_precode_block(span<span<cbf16_t>> outputs,
                                          unsigned            i_re,
                                          const ci8_t*        input,
                                          const simd_cf_t (*weights)[precoding_constants::MAX_NOF_LAYERS])
{
  // Number of groups of NEON_CF_SIZE RE in the block.
  static constexpr unsigned nof_groups = neon_layer_map_nof_re<NofLayers> / NEON_CF_SIZE;

  // Layer map and convert to complex float.
  simd_cf_interleaved in[neon_layer_map_nof_out<NofLayers>];
  layer_n_map_and_ci8_to_cf<NofLayers>(in, input);

  for (unsigned i_port = 0, nof_ports = outputs.size(); i_port != nof_ports; ++i_port) {
    for (unsigned i_group = 0; i_group != nof_groups; ++i_group) {
      // Apply precoding. The even and odd layers are accumulated separately to shorten the dependency chain.
      const simd_cf_interleaved* group_in = &in[i_group * NofLayers];
      simd_cf_interleaved        result   = group_in[0] * weights[i_port][0];
      if (NofLayers > 1) {
        simd_cf_interleaved result_odd = group_in[1] * weights[i_port][1];
        for (unsigned i_layer = 2; i_layer + 1 < NofLayers; i_layer += 2) {
          result     = add_mul(result, group_in[i_layer], weights[i_port][i_layer]);
          result_odd = add_mul(result_odd, group_in[i_layer + 1], weights[i_port][i_layer + 1]);
        }
        if (NofLayers % 2 == 1) {
          result = add_mul(result, group_in[NofLayers - 1], weights[i_port][NofLayers - 1]);
        }
        result = result + result_odd;
      }

      // Store.
      vst1q_u16(reinterpret_cast<uint16_t*>(&outputs[i_port][i_re + i_group * NEON_CF_SIZE]), cf_to_cbf16(result));
    }
  }
}

/// \brief Applies layer mapping and precoding to all the RE.
///
/// The RE that do not fill a complete block, including those the layer mapping may read past, are copied into
/// zero-padded blocks that are processed with the same SIMD instructions.
///
/// \param[out] outputs    Precoded RE of each port.
/// \param[in]  input      Layer-interleaved input symbols.
/// \param[in]  weights    Precoding weights for each port and layer.
/// \param[in]  nof_layers Actual number of layers. The call recurses down to the instantiation with \c NofLayers equal
///                        to \c nof_layers.
template <unsigned NofLayers>
static void layer_n_map_and_precode(span<span<cbf16_t>> outputs,
                                    span<const ci8_t>   input,
                                    const simd_cf_t (*weights)[precoding_constants::MAX_NOF_LAYERS],
                                    unsigned nof_layers)
{
  if (nof_layers != NofLayers) {
    layer_n_map_and_precode<NofLayers - 1>(outputs, input, weights, nof_layers);
    return;
  }

  static constexpr unsigned nof_re_block = neon_layer_map_nof_re<NofLayers>;
  static constexpr unsigned nof_guard_re = neon_layer_map_nof_guard_re<NofLayers>;
  // The guard can exceed one RE, so the remaining RE may span more than one block.
  static constexpr unsigned nof_tail_blocks = (nof_re_block + nof_guard_re - 1 + nof_re_block - 1) / nof_re_block;

  unsigned nof_re      = outputs.front().size();
  unsigned nof_re_simd = (nof_re > nof_guard_re) ? (nof_re - nof_guard_re) : 0;
  unsigned i_re_end    = (nof_re_simd / nof_re_block) * nof_re_block;
  unsigned nof_ports   = outputs.size();

  for (unsigned i_re = 0; i_re != i_re_end; i_re += nof_re_block) {
    layer_n_map_and_precode_block<NofLayers>(outputs, i_re, &input[NofLayers * i_re], weights);
  }

  // Number of RE left for processing.
  unsigned nof_re_tail = nof_re - i_re_end;
  if (nof_re_tail == 0) {
    return;
  }

  // Zero-padded input, the tail blocks plus the RE that the layer mapping may read past them.
  std::array<ci8_t, NofLayers*(nof_tail_blocks * nof_re_block + nof_guard_re)> input_padded = {};
  ocuduvec::copy(span<ci8_t>(input_padded).first(NofLayers * nof_re_tail),
                 input.subspan(NofLayers * i_re_end, NofLayers * nof_re_tail));

  // Padded output, the tail blocks for each port.
  static_vector<std::array<cbf16_t, nof_tail_blocks * nof_re_block>, precoding_constants::MAX_NOF_PORTS> outputs_padded(
      nof_ports);
  static_vector<span<cbf16_t>, precoding_constants::MAX_NOF_PORTS> outputs_padded_view(outputs_padded.begin(),
                                                                                       outputs_padded.end());

  // Layer map and precode the padded blocks.
  for (unsigned i_re = 0; i_re < nof_re_tail; i_re += nof_re_block) {
    layer_n_map_and_precode_block<NofLayers>(outputs_padded_view, i_re, &input_padded[NofLayers * i_re], weights);
  }

  // Copy the valid RE to the outputs.
  for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
    ocuduvec::copy(outputs[i_port].subspan(i_re_end, nof_re_tail),
                   span<const cbf16_t>(outputs_padded[i_port]).first(nof_re_tail));
  }
}

template <>
void layer_n_map_and_precode<0>(span<span<cbf16_t>> /* outputs */,
                                span<const ci8_t> /* input */,
                                const simd_cf_t (* /* weights */)[precoding_constants::MAX_NOF_LAYERS],
                                unsigned /* nof_layers */)
{
  // Do nothing.
}

void channel_precoder_neon::apply_layer_map_and_precoding(re_buffer_writer<cbf16_t>&     output,
                                                          span<const ci8_t>              input,
                                                          const precoding_weight_matrix& precoding) const
{
  unsigned nof_layers = precoding.get_nof_layers();
  unsigned nof_ports  = precoding.get_nof_ports();

  ocudu_assert((nof_layers >= 1) && (nof_layers <= 8), "The number of layers must be in the range [1, 8].");

  // Registers to load the precoding weights.
  simd_cf_t weights[precoding_constants::MAX_NOF_PORTS][precoding_constants::MAX_NOF_LAYERS];

  // Views to store the precoded symbols.
  std::array<span<cbf16_t>, precoding_constants::MAX_NOF_PORTS> outputs;

  // Convert weight matrix to SIMD types, and select output data views.
  for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
    span<const cf_t> port_coeff = precoding.get_port_coefficients(i_port);
    outputs[i_port]             = output.get_slice(i_port);
    for (unsigned i_layer = 0; i_layer != nof_layers; ++i_layer) {
      weights[i_port][i_layer].set1(port_coeff[i_layer]);
    }
  }

  span<span<cbf16_t>> port_outputs = span<span<cbf16_t>>(outputs).first(nof_ports);
  layer_n_map_and_precode<precoding_constants::MAX_NOF_LAYERS>(port_outputs, input, weights, nof_layers);
}
