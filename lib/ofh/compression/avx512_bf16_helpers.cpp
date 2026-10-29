// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief AVX512_BF16 instantiations of the OFH decompressors.
///
/// This translation unit is compiled with the AVX512_BF16 instruction set, the rest of the AVX512 code keeps running
/// on CPUs without it. The decompressors check the CPU support at run time before calling into it.

#include "avx512_helpers.h"

using namespace ocudu;
using namespace ofh;

template <unsigned BITS, bool HAS_COMP_PARAM>
OCUDU_FLATTEN void mm512::decompress_prb_quads_bf16(cbf16_t* out, const uint8_t* in, unsigned nof_prbs, float scale)
{
  decompress_prb_quads<BITS, HAS_COMP_PARAM, true>(out, in, nof_prbs, scale);
}

template void mm512::decompress_prb_quads_bf16<9, false>(cbf16_t*, const uint8_t*, unsigned, float);
template void mm512::decompress_prb_quads_bf16<9, true>(cbf16_t*, const uint8_t*, unsigned, float);
template void mm512::decompress_prb_quads_bf16<14, false>(cbf16_t*, const uint8_t*, unsigned, float);
template void mm512::decompress_prb_quads_bf16<14, true>(cbf16_t*, const uint8_t*, unsigned, float);
template void mm512::decompress_prb_quads_bf16<16, false>(cbf16_t*, const uint8_t*, unsigned, float);
template void mm512::decompress_prb_quads_bf16<16, true>(cbf16_t*, const uint8_t*, unsigned, float);
