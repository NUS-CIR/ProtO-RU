// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief Loop utilities shared by the SIMD compression implementations.

#pragma once

#include "ocudu/support/compiler.h"
#include <utility>

namespace ocudu {
namespace ofh {

/// Calls the given function with \c std::integral_constant arguments from 0 to \c N - 1, see the overload below.
template <unsigned N, typename Func, unsigned... Is>
OCUDU_FORCE_INLINE void for_each_index(Func&& func, std::integer_sequence<unsigned, Is...> /* unused */)
{
  (func(std::integral_constant<unsigned, Is>()), ...);
}

/// \brief Calls the given function with \c std::integral_constant arguments from 0 to \c N - 1.
///
/// It unrolls a loop over registers at compile time, so that the index can be used as a template argument or an
/// immediate operand inside the function, which takes it as <tt>auto index</tt> and reads it as
/// <tt>decltype(index)::value</tt>.
template <unsigned N, typename Func>
OCUDU_FORCE_INLINE void for_each_index(Func&& func)
{
  for_each_index<N>(std::forward<Func>(func), std::make_integer_sequence<unsigned, N>());
}

/// \brief Runs a loop of two stages, with the first stage two iterations ahead of the second one.
///
/// The SIMD compressors and decompressors process each block of PRBs in two stages, for example quantizing the samples
/// and then packing them. The second stage needs the result of the first one, which comes out of a long chain of
/// dependent instructions, so if both stages of a block run back to back the CPU idles waiting for the chain.
/// Preparing the following blocks ahead gives the CPU independent instructions to overlap the chains: the loop
/// prepares the block \c i + 2 before emitting the block \c i. Preparing two blocks ahead instead of one is
/// measurably faster for the compressors.
///
/// Three blocks are in flight, whose states rotate through three local variables so that they stay in registers. The
/// main loop is unrolled three times to rotate the variables without copying them.
///
/// \tparam Prepare       Type of the first stage, callable as <tt>prepare(i)</tt> and returning the state of the
///                       block \c i.
/// \tparam Emit          Type of the second stage, callable as <tt>emit(state, i)</tt>.
/// \param[in] nof_blocks Number of blocks to process.
/// \param[in] prepare    First stage.
/// \param[in] emit       Second stage.
template <typename Prepare, typename Emit>
inline void run_pipelined_loop(unsigned nof_blocks, Prepare&& prepare, Emit&& emit)
{
  using state_type = decltype(prepare(0U));

  if (nof_blocks == 0) {
    return;
  }
  if (nof_blocks == 1) {
    emit(prepare(0), 0);
    return;
  }

  state_type s0 = prepare(0);
  state_type s1 = prepare(1);
  state_type s2;
  unsigned   i = 2;
  for (; i + 2 < nof_blocks; i += 3) {
    s2 = prepare(i);
    emit(s0, i - 2);
    s0 = prepare(i + 1);
    emit(s1, i - 1);
    s1 = prepare(i + 2);
    emit(s2, i);
  }
  // At most two blocks are left to prepare, the states are copied rather than unrolling further.
  for (; i != nof_blocks; ++i) {
    s2 = prepare(i);
    emit(s0, i - 2);
    s0 = s1;
    s1 = s2;
  }
  emit(s0, i - 2);
  emit(s1, i - 1);
}

} // namespace ofh
} // namespace ocudu
