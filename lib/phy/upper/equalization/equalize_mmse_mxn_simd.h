// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief Minimum Mean Square Error equalizer algorithm for a MIMO M X N channel.

#pragma once

#include "gram_matrix.h"
#include "matrix_inverse.h"
#include "ocudu/ocuduvec/simd.h"

namespace ocudu {

/// \brief Equalizes a SIMD block of resource elements with the Minimum Mean Square Error algorithm.
///
/// \tparam     NofLayers     Number of transmit layers.
/// \tparam     NofPorts      Number of receive ports.
/// \param[out] eq_symbols    Equalized symbols, one per layer, scaled to unit gain.
/// \param[out] noise_vars    Post-equalization noise variances, one per layer.
/// \param[in]  ch_symbols    Received symbols, one per port.
/// \param[in]  h             Channel matrix, indexed by port and layer.
/// \param[in]  noise_var_est Estimated noise variance.
template <unsigned NofLayers, unsigned NofPorts>
void equalize_mmse_mxn_simd(simd_cf_t eq_symbols[NofLayers],
                            simd_f_t  noise_vars[NofLayers],
                            simd_cf_t ch_symbols[NofPorts],
                            simd_cf_t h[NofPorts][NofLayers],
                            float     noise_var_est)
{
  const simd_f_t  infinity       = ocudu_simd_f_set1(std::numeric_limits<float>::infinity());
  const simd_cf_t complex_zero   = ocudu_simd_cf_set1(0);
  const simd_f_t  real_zero      = ocudu_simd_f_set1(0);
  const simd_f_t  real_one       = ocudu_simd_f_set1(1.0F);
  const simd_f_t  noise_var_simd = ocudu_simd_f_set1(noise_var_est);

  // Calculate Gram matrix, i.e., G = H^H H.
  simd_cf_t h_gram[NofLayers][NofLayers];
  squared_gram_matrix<NofPorts, NofLayers>(h_gram, h);

  // Add noise in the diagonal, i.e., G = H^H H + noise_var I. The diagonal is real.
  simd_sel_t isnormal_mask = ocudu_simd_sel_set_ones();
  for (unsigned i_layer = 0; i_layer != NofLayers; ++i_layer) {
    simd_f_t diagonal        = ocudu_simd_cf_re(h_gram[i_layer][i_layer]);
    isnormal_mask            = ocudu_simd_sel_and(isnormal_mask, ocudu_simd_f_gt(diagonal, real_zero));
    h_gram[i_layer][i_layer] = ocudu_simd_cf_make(diagonal + noise_var_simd);
  }

  // Invert the Gram matrix, which is Hermitian positive definite, i.e., G^-1 = (H^H H + noise_var I)^-1.
  simd_cf_t h_gram_inv[NofLayers][NofLayers];
  isnormal_mask = ocudu_simd_sel_and(isnormal_mask, hpd_matrix_inverse<NofLayers>(h_gram_inv, h_gram));

  // Matched filter output, i.e., z = H^H y.
  simd_cf_t z[NofLayers];
  for (unsigned i_layer = 0; i_layer != NofLayers; ++i_layer) {
    z[i_layer] = ocudu_simd_cf_set1(0);
    for (unsigned i_port = 0; i_port != NofPorts; ++i_port) {
      z[i_layer] += ocudu_simd_cf_conjprod(ch_symbols[i_port], h[i_port][i_layer]);
    }
  }

  // Calculate equalized symbols.
  for (unsigned i_layer = 0; i_layer != NofLayers; ++i_layer) {
    // Apply the inverse Gram matrix to the matched filter output, i.e., x = G^-1 z.
    simd_cf_t eq_symbols_res = ocudu_simd_cf_set1(0);
    for (unsigned k = 0; k != NofLayers; ++k) {
      eq_symbols_res += h_gram_inv[i_layer][k] * z[k];
    }

    // The gain of the layer is diag(G^-1 H^H H) = diag(G^-1 (G - noise_var I)) = 1 - noise_var diag(G^-1), that is,
    // SINR / (1 + SINR). It lies in (0, 1), but the subtraction can round to zero or below at very low SINR.
    simd_f_t   gain       = real_one - noise_var_simd * ocudu_simd_cf_re(h_gram_inv[i_layer][i_layer]);
    simd_sel_t layer_mask = ocudu_simd_sel_and(isnormal_mask, ocudu_simd_f_gt(gain, real_zero));
    simd_f_t   gain_inv   = ocudu_simd_f_precise_rcp(gain);

    // Rescale the estimated symbols for better LLR computation.
    eq_symbols_res *= gain_inv;

    // Calculate the noise for the layer, taking into account the LLR scaling, i.e., 1 / SINR.
    simd_f_t noise_var_res = gain_inv - real_one;

    // Discard invalid results.
    eq_symbols[i_layer] = ocudu_simd_cf_select(complex_zero, eq_symbols_res, layer_mask);
    noise_vars[i_layer] = ocudu_simd_f_select(infinity, noise_var_res, layer_mask);
  }
}

} // namespace ocudu
