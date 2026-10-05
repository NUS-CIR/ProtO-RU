// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief AVX2 pseudo-random generator implementation declaration.

#pragma once

#include "pseudo_random_generator_impl.h"

namespace ocudu {

/// Pseudo-random generator based on AVX2 instruction sets.
class pseudo_random_generator_avx2_impl : public pseudo_random_generator_impl
{
public:
  using pseudo_random_generator_impl::apply_xor;
  using pseudo_random_generator_impl::generate;

  // See interface for the documentation.
  void apply_xor(bit_buffer& out, const bit_buffer& in) override;

  // See interface for the documentation.
  void generate(bit_buffer& data) override;
};

} // namespace ocudu
