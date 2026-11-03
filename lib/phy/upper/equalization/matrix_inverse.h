// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ocuduvec/simd.h"

namespace ocudu {

#if OCUDU_SIMD_CF_SIZE

template <unsigned N, unsigned Index>
struct squared_matrix_inverse_s {
  static simd_sel_t compute(simd_cf_t out[N][N], simd_cf_t in[N][N])
  {
    static constexpr unsigned i         = Index;
    const simd_f_t            zero_simd = ocudu_simd_f_set1(0);

    // Find the pivot row.
    simd_cf_t pivot = in[i][i];

    // Calculate pivot squared norm and determine if it is possible to invert.
    simd_f_t   pivot_norm    = ocudu_simd_cf_norm_sq(pivot);
    simd_sel_t isnormal_mask = ocudu_simd_f_gt(pivot_norm, zero_simd);

    // Invert pivot.
    simd_cf_t pivot_inv = ocudu_simd_cf_mul(ocudu_simd_cf_conj(pivot), ocudu_simd_f_precise_rcp(pivot_norm));

    // Normalize the pivot row.
    out[i][i] = ocudu_simd_cf_set1(1);
    for (unsigned j = i + 1; j != N; ++j) {
      in[i][j] *= pivot_inv;
    }
    for (unsigned j = 0; j != i + 1; ++j) {
      out[i][j] *= pivot_inv;
    }

    // Make other rows zero in the current column.
    for (unsigned k = 0; k != N; ++k) {
      simd_cf_t factor = in[k][i];
      if (k != i) {
        for (unsigned j = i; j != N; ++j) {
          in[k][j] -= factor * in[i][j];
        }
        for (unsigned j = 0; j != i; ++j) {
          out[k][j] -= factor * out[i][j];
        }
        out[k][i] = -factor * out[i][i];
      }
    }

    // Advance to the next row.
    simd_sel_t isnormal_mask_next = squared_matrix_inverse_s<N, Index + 1>::compute(out, in);

    // Combine the current row mask with the next.
    return ocudu_simd_sel_and(isnormal_mask, isnormal_mask_next);
  }
};

template <unsigned N>
struct squared_matrix_inverse_s<N, N> {
  static simd_sel_t compute(simd_cf_t[N][N], simd_cf_t[N][N]) { return ocudu_simd_sel_set_ones(); }
};

/// \brief Perform a matrix inversion.
/// \tparam        N    Matrix dimensions.
/// \param[out]    out  Resultant matrix.
/// \param[in,out] in   Input matrix, it becomes an identity matrix.
/// \return \c true if the matrix is invertible, otherwise \c false.
template <unsigned N>
simd_sel_t squared_matrix_inverse(simd_cf_t out[N][N], simd_cf_t in[N][N])
{
  // Create an identity matrix on the output.
  for (unsigned i = 0; i != N; ++i) {
    for (unsigned j = 0; j != N; ++j) {
      out[i][j] = ocudu_simd_cf_set1((i == j) ? 1 : 0);
    }
  }

  return squared_matrix_inverse_s<N, 0>::compute(out, in);
}

#endif // OCUDU_SIMD_CF_SIZE

} // namespace ocudu
