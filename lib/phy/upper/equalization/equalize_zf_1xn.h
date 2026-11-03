// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief Zero Forcing equalizer algorithm for a SIMO 1 X N channel.

#pragma once

#include "ocudu/ocuduvec/fill.h"
#include "ocudu/ocuduvec/simd.h"
#include "ocudu/ocuduvec/zero.h"
#include "ocudu/phy/antenna_ports.h"

namespace ocudu {

/// \brief Implementation of a Zero Forcing equalizer for a SIMO 1 X \c RX_PORTS channel.
/// \tparam RX_PORTS          Number of receive antenna ports.
/// \param[out] symbols_out   Resultant equalized symbols.
/// \param[out] nvars_out     Noise variances after equalization.
/// \param[in]  ch_symbols    Channel symbols, i.e., complex samples from the receive ports.
/// \param[in]  ch_estimates  Channel estimation coefficients.
/// \param[in]  noise_var_est Estimated noise variance for each port.
/// \param[in]  tx_scaling    Transmission gain scaling factor.
template <unsigned RX_PORTS>
void equalize_zf_1xn(span<cf_t>                            symbols_out,
                     span<float>                           nvars_out,
                     const re_buffer_reader<cbf16_t>&      ch_symbols,
                     const channel_equalizer::ch_est_list& ch_estimates,
                     span<const float>                     noise_var_est,
                     float                                 tx_scaling)
{
  // Number of RE to process.
  unsigned nof_re = ch_symbols.get_nof_re();

  unsigned i_re = 0;

  // Views over the input data.
  std::array<span<const cbf16_t>, MAX_PORTS> port_symbols;
  std::array<span<const cbf16_t>, MAX_PORTS> port_ests;

  for (unsigned i_port = 0; i_port != RX_PORTS; ++i_port) {
    port_symbols[i_port] = ch_symbols.get_slice(i_port);
    port_ests[i_port]    = ch_estimates.get_channel(i_port, 0);
  }

#if defined(__AVX2__) || defined(__ARM_NEON)

  // Create registers with zero and infinity values.
  simd_cf_t cf_zero  = ocudu_simd_cf_zero();
  simd_f_t  zero     = ocudu_simd_f_zero();
  simd_f_t  infinity = ocudu_simd_f_set1(std::numeric_limits<float>::infinity());

  for (unsigned i_re_end = (nof_re / OCUDU_SIMD_CF_SIZE) * OCUDU_SIMD_CF_SIZE; i_re != i_re_end;
       i_re += OCUDU_SIMD_CF_SIZE) {
    simd_f_t  ch_mod_sq = ocudu_simd_f_zero();
    simd_f_t  nvar_acc  = ocudu_simd_f_zero();
    simd_cf_t re_out    = ocudu_simd_cf_zero();

    for (unsigned i_port = 0; i_port != RX_PORTS; ++i_port) {
      // Get the input RE and channel estimate coefficients.
      simd_cf_t re_in  = ocudu_simd_loadu(port_symbols[i_port].data() + i_re);
      simd_cf_t ch_est = ocudu_simd_loadu(port_ests[i_port].data() + i_re);

      // Compute the channel square norm.
      simd_f_t ch_est_norm = ocudu_simd_cf_norm_sq(ch_est);

      // Load noise variance in a SIMD register.
      simd_f_t port_noise_var_est = ocudu_simd_f_set1(noise_var_est[i_port]);

      // Detect an abnormal computation parameter. This detects whether the channel estimate is NaN or infinite.
      simd_sel_t isnormal_mask = ocudu_simd_f_gt(infinity, ch_est_norm);

      // Compute the channel square norm, by accumulating the channel square absolute values.
      ch_mod_sq = ocudu_simd_f_add_sel(ch_mod_sq, ch_est_norm, isnormal_mask);

      // Accumulate the noise variance weighted with the channel estimate norm.
      nvar_acc = ocudu_simd_f_add_sel(nvar_acc, ocudu_simd_f_mul(ch_est_norm, port_noise_var_est), isnormal_mask);

      // Apply the matched channel filter to each received RE and accumulate the results.
      re_out = ocudu_simd_cf_add_sel(re_out, ocudu_simd_cf_conjprod(re_in, ch_est), isnormal_mask);
    }

    // Calculate the denominator of the pseudo-inverse.
    simd_f_t d_pinv = ocudu_simd_f_mul(ocudu_simd_f_set1(tx_scaling), ch_mod_sq);

    // Compute reciprocal of the denominator.
    simd_f_t d_pinv_rcp = ocudu_simd_f_rcp(d_pinv);

    // Detect abnormal computation parameters. This detects whenever the channel estimate is zero or NaN.
    simd_sel_t isnormal_mask = ocudu_simd_f_gt(d_pinv, zero);

    // Detect abnormal computation parameters. This detects whenever the channel estimate is infinity.
    isnormal_mask = ocudu_simd_sel_and(isnormal_mask, ocudu_simd_f_gt(infinity, d_pinv));

    // Calculate noise variances.
    simd_f_t vars_out = ocudu_simd_f_mul(nvar_acc, ocudu_simd_f_mul(d_pinv_rcp, d_pinv_rcp));

    // If abnormal calculation parameters are detected, the noise variances are set to infinity.
    ocudu_simd_f_storeu(nvars_out.data() + i_re, ocudu_simd_f_select(infinity, vars_out, isnormal_mask));

    // Normalize the gain of the channel combined with the equalization to unity.
    re_out = ocudu_simd_cf_mul(re_out, d_pinv_rcp);

    // If abnormal calculation parameters are detected, the equalized symbols are set to zero.
    ocudu_simd_storeu(symbols_out.data() + i_re, ocudu_simd_cf_select(cf_zero, re_out, isnormal_mask));
  }
#endif // __AVX2__ || __ARM_NEON

  for (; i_re != nof_re; ++i_re) {
    float ch_mod_sq = 0.0F;
    float nvar_acc  = 0.0F;
    cf_t  re_out    = cf_t();

    for (unsigned i_port = 0; i_port != RX_PORTS; ++i_port) {
      // Get the input RE and channel estimate coefficient.
      cf_t re_in  = to_cf(port_symbols[i_port][i_re]);
      cf_t ch_est = to_cf(port_ests[i_port][i_re]);

      // Compute the channel square norm.
      float ch_est_norm = std::norm(ch_est);

      if (std::isnormal(ch_est_norm) && std::isnormal(noise_var_est[i_port]) && (noise_var_est[i_port] > 0)) {
        // Accumulate the channel square absolute values.
        ch_mod_sq += ch_est_norm;

        // Accumulate the noise variance weighted with the channel estimate norm.
        nvar_acc += ch_est_norm * noise_var_est[i_port];

        // Apply the matched channel filter to each received RE and accumulate the results.
        re_out += re_in * std::conj(ch_est);
      }
    }

    // Return values in case of abnormal computation parameters. These include negative, zero, NAN or INF noise
    // variances and zero, NAN or INF channel estimation coefficients.
    symbols_out[i_re] = 0;
    nvars_out[i_re]   = std::numeric_limits<float>::infinity();

    float d_pinv = tx_scaling * ch_mod_sq;

    if (std::isnormal(d_pinv) && std::isnormal(nvar_acc)) {
      // Calculate the reciprocal of the denominator.
      float d_pinv_rcp = 1.0F / d_pinv;

      // Normalize the gain of the channel combined with the equalization to unity.
      symbols_out[i_re] = re_out * d_pinv_rcp;

      // Calculate noise variances.
      nvars_out[i_re] = nvar_acc * d_pinv_rcp * d_pinv_rcp;
    }
  }
}

} // namespace ocudu
