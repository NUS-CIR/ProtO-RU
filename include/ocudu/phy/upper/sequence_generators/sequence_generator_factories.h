// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/phy/upper/sequence_generators/low_papr_sequence_collection.h"
#include "ocudu/phy/upper/sequence_generators/low_papr_sequence_generator.h"
#include "ocudu/phy/upper/sequence_generators/pseudo_random_generator.h"
#include <memory>
#include <string>

namespace ocudu {

class low_papr_sequence_generator_factory;

class low_papr_sequence_collection_factory
{
public:
  virtual ~low_papr_sequence_collection_factory() = default;
  virtual std::unique_ptr<low_papr_sequence_collection>
  create(unsigned m, unsigned delta, span<const float> alphas) = 0;
};

std::shared_ptr<low_papr_sequence_collection_factory>
create_low_papr_sequence_collection_sw_factory(std::shared_ptr<low_papr_sequence_generator_factory> lpg_factory);

class low_papr_sequence_generator_factory
{
public:
  virtual ~low_papr_sequence_generator_factory()                = default;
  virtual std::unique_ptr<low_papr_sequence_generator> create() = 0;
};

std::shared_ptr<low_papr_sequence_generator_factory> create_low_papr_sequence_generator_sw_factory();

class pseudo_random_generator_factory
{
public:
  virtual ~pseudo_random_generator_factory()                = default;
  virtual std::unique_ptr<pseudo_random_generator> create() = 0;
};

/// \brief Creates a pseudo-random generator factory.
///
/// \param[in] generator_type Implementation selector. Values: generic, avx2, auto.
std::shared_ptr<pseudo_random_generator_factory>
create_pseudo_random_generator_sw_factory(const std::string& generator_type = "auto");

} // namespace ocudu
