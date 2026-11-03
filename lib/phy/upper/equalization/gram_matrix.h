// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief Gram matrix calculation for SIMD channel matrices.

#pragma once

#include "ocudu/adt/complex.h"
#include "ocudu/ocuduvec/simd.h"

namespace ocudu {

#if OCUDU_SIMD_CF_SIZE

/// \brief Calculates the Gram matrix \f$G = A^H A\f$.
///
/// \tparam     N   Number of rows of the input matrix.
/// \tparam     M   Number of columns of the input matrix.
/// \param[out] out Gram matrix, \f$G(i, j) = \sum_k A^*(k, i) A(k, j)\f$.
/// \param[in]  in  Input matrix \f$A\f$.
template <unsigned N, unsigned M>
inline void squared_gram_matrix(simd_cf_t out[M][M], const simd_cf_t in[N][M])
{
  // The Gram matrix is Hermitian: the diagonal is real and the lower triangle is the conjugate of the upper one, so
  // only the upper triangle takes complex products.
  for (unsigned i = 0; i != M; ++i) {
    simd_f_t diagonal = ocudu_simd_f_zero();
    for (unsigned k = 0; k != N; ++k) {
      diagonal += ocudu_simd_cf_norm_sq(in[k][i]);
    }
    out[i][i] = ocudu_simd_cf_make(diagonal);

    for (unsigned j = i + 1; j != M; ++j) {
      simd_cf_t result = ocudu_simd_cf_set1(0);
      for (unsigned k = 0; k != N; ++k) {
        result += ocudu_simd_cf_conjprod(in[k][j], in[k][i]);
      }
      out[i][j] = result;
      out[j][i] = ocudu_simd_cf_conj(result);
    }
  }
}

#endif // OCUDU_SIMD_CF_SIZE

} // namespace ocudu
