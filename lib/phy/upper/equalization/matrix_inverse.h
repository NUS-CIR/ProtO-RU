// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief Hermitian positive definite matrix inversion for SIMD matrices.

#pragma once

#include "ocudu/ocuduvec/simd.h"

namespace ocudu {

#if OCUDU_SIMD_CF_SIZE

/// \brief Inverts a Hermitian positive definite matrix, such as a Gram matrix.
///
/// Each SIMD lane holds an independent matrix, so the function inverts \c OCUDU_SIMD_CF_SIZE matrices simultaneously.
///
/// The input matrix \f$A\f$ is factorized as
/// \f[
/// A = L D L^H
/// \f]
/// with \f$L\f$ unit lower triangular and \f$D\f$ real diagonal. The inverse is then
/// \f[
/// A^{-1} = W^H D^{-1} W, \qquad W = L^{-1}
/// \f]
///
/// Only the diagonal and the lower triangle of the input are read.
///
/// \tparam     N   Matrix dimensions.
/// \param[out] out Inverse matrix.
/// \param[in]  in  Hermitian input matrix.
/// \return A mask of the lanes whose matrix is positive definite.
template <unsigned N>
simd_sel_t hpd_matrix_inverse(simd_cf_t out[N][N], const simd_cf_t in[N][N])
{
  const simd_f_t zero  = ocudu_simd_f_zero();
  simd_sel_t     is_pd = ocudu_simd_sel_set_ones();

  // Factorization in = L D L^H, with d(j) = D(j, j), d_inv(j) = 1 / d(j) and e(i, j) = L(i, j) d(j). The following
  // columns reuse e.
  simd_f_t  d_inv[N];
  simd_cf_t lo[N][N];
  simd_cf_t e[N][N];
  for (unsigned j = 0; j != N; ++j) {
    // d(j) = in(j, j) - sum_k |L(j, k)|^2 d(k), where |L(j, k)|^2 d(k) = Re(L(j, k) conj(e(j, k))).
    simd_f_t d_j = ocudu_simd_cf_re(in[j][j]);
    for (unsigned k = 0; k != j; ++k) {
      d_j -= ocudu_simd_cf_re(lo[j][k]) * ocudu_simd_cf_re(e[j][k]) +
             ocudu_simd_cf_im(lo[j][k]) * ocudu_simd_cf_im(e[j][k]);
    }
    is_pd    = ocudu_simd_sel_and(is_pd, ocudu_simd_f_gt(d_j, zero));
    d_inv[j] = ocudu_simd_f_precise_rcp(d_j);

    // L(i, j) = (in(i, j) - sum_k L(i, k) conj(L(j, k)) d(k)) / d(j).
    for (unsigned i = j + 1; i != N; ++i) {
      e[i][j] = in[i][j];
      for (unsigned k = 0; k != j; ++k) {
        e[i][j] -= ocudu_simd_cf_conjprod(lo[i][k], e[j][k]);
      }
      lo[i][j] = e[i][j] * d_inv[j];
    }
  }

  // W = L^-1, unit lower triangular: W(i, j) = -L(i, j) - sum_{k = j + 1}^{i - 1} L(i, k) W(k, j).
  simd_cf_t w[N][N];
  for (unsigned j = 0; j != N; ++j) {
    for (unsigned i = j + 1; i != N; ++i) {
      w[i][j] = -lo[i][j];
      for (unsigned k = j + 1; k != i; ++k) {
        w[i][j] -= lo[i][k] * w[k][j];
      }
    }
  }

  // in^-1 = W^H D^-1 W: out(i, j) = sum_{k >= i} conj(W(k, i)) W(k, j) / d(k) for i >= j, with W(k, k) = 1. The upper
  // triangle is the conjugate of the lower one, and the diagonal is real.
  for (unsigned i = 0; i != N; ++i) {
    simd_f_t diag = d_inv[i];
    for (unsigned k = i + 1; k != N; ++k) {
      diag += ocudu_simd_cf_norm_sq(w[k][i]) * d_inv[k];
    }
    out[i][i] = ocudu_simd_cf_make(diag);

    for (unsigned j = 0; j != i; ++j) {
      out[i][j] = w[i][j] * d_inv[i];
      for (unsigned k = i + 1; k != N; ++k) {
        out[i][j] += ocudu_simd_cf_conjprod(w[k][j], w[k][i]) * d_inv[k];
      }
      out[j][i] = ocudu_simd_cf_conj(out[i][j]);
    }
  }

  return is_pd;
}

#endif // OCUDU_SIMD_CF_SIZE

} // namespace ocudu
