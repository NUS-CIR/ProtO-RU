// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ocuduvec/conversion.h"
#include "ocudu/ocuduvec/simd.h"
#include "ocudu/ofh/compression/compression_params.h"
#include "ocudu/support/ocudu_assert.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace ocudu {
namespace ofh {

/// \brief Returns the largest brain float that the SIMD quantizers accept for the given scale, as the bits of bf16.
///
/// The SIMD quantizers scale the input and convert it to 32bit integers before saturating it to 16 bits. The
/// conversion does not saturate by itself: any value outside the 32bit range becomes INT32_MIN. A large positive
/// input would therefore end up as the most negative sample instead of the most positive one.
///
/// Clamping the input to the value returned here before scaling avoids it. The scaled value then stays below
/// \f$2^{30}\f$, which the conversion handles, and it is still far above the 16bit maximum, so the final saturation
/// clips it as expected. The clamp does not change the result for any input that fits in 16 bits after scaling.
///
/// Positive brain floats order like 16bit integers, so the clamp is applied directly on the raw bits with a single
/// integer minimum, before the widening to floats. Only the upper bound is needed: large negative values convert to
/// INT32_MIN, which saturates to the correct 16bit minimum.
///
/// \param[in] scale Scaling factor applied to the brain floats before the conversion, must be positive.
/// \return The limit \f$2^{30} / scale\f$, truncated to a brain float, as a 16bit integer.
inline int16_t get_bf16_saturation_clamp(float scale)
{
  float limit = 0x1p30F / scale;
  ocudu_assert(std::isfinite(limit) && (limit > 0.0F), "Invalid quantization scale {}", scale);
  // Truncating the float to a brain float keeps the clamp below the limit.
  uint32_t limit_bits = 0;
  std::memcpy(&limit_bits, &limit, sizeof(limit_bits));
  return static_cast<int16_t>(limit_bits >> 16);
}

/// \brief Describes quantization of floating point number in the range [-1; +1] to a fixed point number of a given bit
/// width.
///
/// Values are rounded to the nearest integer with ties to even, and values outside the range saturate at the symmetric
/// full scale \f$\pm(2^{w-1}-1)\f$. The only exception is the quantization of sequences into 16 bits, where the
/// negative values saturate at -32768.
class quantizer
{
public:
  /// Constructor receives width in bits of a resulting fixed point number.
  explicit quantizer(unsigned bit_width_) :
    bit_width(bit_width_), full_scale((1 << (bit_width - 1)) - 1), gain(static_cast<float>(full_scale))
  {
  }

  /// \brief Quantizes a floating point value into 16-bit integer.
  ///
  /// \param[in] value A floating point value.
  /// \return Resultant quantized value.
  int16_t to_fixed_point(float value) const
  {
    float clipped = value;
    if (std::abs(value) > 1.0) {
      clipped = std::copysign(1.0, value);
    }
    return static_cast<int16_t>(std::nearbyint(clipped * gain));
  }

  /// \brief Quantizes a brain float value into a 16-bit integer.
  ///
  /// \param[in] value A brain float value.
  /// \return Resultant quantized value.
  int16_t to_fixed_point(bf16_t value) const
  {
    float input_value = ocudu::to_float(value);
    return to_fixed_point(input_value);
  }

  /// \brief Quantizes a sequence of floating point values into 16bit integers.
  ///
  /// \param[out] z       Resultant quantized values.
  /// \param[in] x        Sequence of floating point values.
  /// \param[in] in_scale Scaling factor applied to the input prior conversion.
  ///
  /// \remark \c z and \c x must have the same size.
  void to_fixed_point(span<int16_t> z, span<const float> x, float in_scale) const
  {
    float scale = gain * in_scale;
    ocuduvec::convert(z, x, scale);
    saturate(z);
  }

  /// \brief Quantizes a sequence of brain float values into 16-bit integers.
  ///
  /// \param[out] z       Resultant quantized values.
  /// \param[in] x        Sequence of brain float values.
  /// \param[in] in_scale Scaling factor applied to the input prior conversion.
  ///
  /// \remark \c z and \c x must have the same size.
  void to_fixed_point(span<int16_t> z, span<const bf16_t> x, float in_scale) const
  {
    float scale = gain * in_scale;
    ocuduvec::convert(z, x, scale);
    saturate(z);
  }

  /// \brief Converts fixed point value to floating point.
  ///
  /// \param[in] p 16-bit integer value.
  /// \return Resultant floating point value.
  float to_float(int p) const { return (p / gain); }

  /// \brief Converts a sequence of fixed point values to complex brain floating point values.
  ///
  /// \param[out] z       Resultant complex brain floating point values.
  /// \param[in] x        Sequence of 16-bit integer values.
  /// \param[in] in_scale Scaling factor applied to the input prior conversion.
  ///
  /// \remark The size of \c x must be twice the size of \c z as \x is comprised by the quantized pairs of real and
  /// imaginary parts of complex values.
  void to_brain_float(span<cbf16_t> z, span<const int16_t> x, float in_scale) const
  {
    float scale = gain / in_scale;
    ocuduvec::convert(z, x, scale);
  }

  /// Sign extends the input value.
  int16_t sign_extend(int16_t value) const
  {
    // Bit holding the sign of a \c bit_width-bit mantissa.
    uint16_t sign = uint16_t{1} << (bit_width - 1U);
    // Mask the input value to keep only \c bit_width number of bits.
    uint16_t bits = static_cast<uint16_t>(value) & static_cast<uint16_t>((1U << bit_width) - 1U);
    // Avoid the common shift-left/shift-right sign-extension trick because left-shifting into the sign bit of a
    // signed type is undefined in C++17.
    return static_cast<int16_t>(static_cast<int32_t>(bits ^ sign) - static_cast<int32_t>(sign));
  }

private:
  /// \brief Saturates the converted values at the full scale of the bit width.
  ///
  /// A scaled input value outside [-1; +1] converts to a value beyond the full scale of the bit width. The conversion
  /// only saturates at the 16-bit range, so for narrower bit widths such a value would wrap around when packed.
  void saturate(span<int16_t> z) const
  {
    // For 16 bits, the saturation during the conversion is enough.
    if (bit_width == MAX_IQ_WIDTH) {
      return;
    }

    unsigned i = 0;
#if OCUDU_SIMD_S_SIZE
    simd_i16_t upper = ocudu_simd_s_set1(full_scale);
    simd_i16_t lower = ocudu_simd_s_set1(-full_scale);
    for (unsigned i_end = (z.size() / OCUDU_SIMD_S_SIZE) * OCUDU_SIMD_S_SIZE; i != i_end; i += OCUDU_SIMD_S_SIZE) {
      simd_i16_t value = ocudu_simd_s_loadu(z.data() + i);
      ocudu_simd_s_storeu(z.data() + i, ocudu_simd_s_max(ocudu_simd_s_min(value, upper), lower));
    }
#endif // OCUDU_SIMD_S_SIZE
    for (unsigned i_end = z.size(); i != i_end; ++i) {
      z[i] = std::clamp(z[i], static_cast<int16_t>(-full_scale), full_scale);
    }
  }

  /// Width of fixed point number in bits.
  const unsigned bit_width;
  /// Largest quantized magnitude.
  const int16_t full_scale;
  /// Quantization gain.
  const float gain;
};

} // namespace ofh
} // namespace ocudu
