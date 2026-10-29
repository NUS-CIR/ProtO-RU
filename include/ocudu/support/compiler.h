// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief Defines generic compiler macros that get translated into compiler specific directives simplifying portability
/// between different compilers.

#pragma once

/// Provides a hint to the compiler that a condition is likely to be true.
#define OCUDU_LIKELY(EXPR) __builtin_expect((bool)(EXPR), true)
/// Provides a hint to the compiler that a condition is likely to be false.
#define OCUDU_UNLIKELY(EXPR) __builtin_expect((bool)(EXPR), false)

/// States that it is undefined behavior for the compiler to reach this point.
#define OCUDU_UNREACHABLE __builtin_unreachable()

/// Verifies if compile-time symbol is defined.
#define OCUDU_IS_DEFINED(x) OCUDU_IS_DEFINED2(x)
#define OCUDU_IS_DEFINED2(x) (#x[0] == 0 || (#x[0] >= '1' && #x[0] <= '9'))

/// Ensures a function is always inlinable.
#define OCUDU_FORCE_INLINE __attribute__((always_inline)) inline

/// Inlines every call inside a function, including the calls of the inlined callees, regardless of the size limits.
#define OCUDU_FLATTEN __attribute__((flatten))
