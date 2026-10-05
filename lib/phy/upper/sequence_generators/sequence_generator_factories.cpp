// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/phy/upper/sequence_generators/sequence_generator_factories.h"
#include "low_papr_sequence_collection_impl.h"
#include "low_papr_sequence_generator_impl.h"
#include "pseudo_random_generator_impl.h"
#include "ocudu/support/cpu_features.h"
#include "ocudu/support/math/math_utils.h"

#ifdef __x86_64__
#include "pseudo_random_generator_avx2_impl.h"
#endif // __x86_64__

using namespace ocudu;

namespace {

class low_papr_sequence_collection_sw_factory : public low_papr_sequence_collection_factory
{
public:
  low_papr_sequence_collection_sw_factory(std::shared_ptr<low_papr_sequence_generator_factory>& lpg_factory_) :
    lpg_factory(std::move(lpg_factory_))
  {
    ocudu_assert(lpg_factory, "Invalid generator factory.");
  }

  std::unique_ptr<low_papr_sequence_collection> create(unsigned m, unsigned delta, span<const float> alphas) override
  {
    std::unique_ptr<low_papr_sequence_generator> generator = lpg_factory->create();
    ocudu_assert(generator, "Invalid generator.");

    return std::make_unique<low_papr_sequence_collection_impl>(*generator, m, delta, alphas);
  }

private:
  std::shared_ptr<low_papr_sequence_generator_factory> lpg_factory;
};

class low_papr_sequence_generator_sw_factory : public low_papr_sequence_generator_factory
{
public:
  std::unique_ptr<low_papr_sequence_generator> create() override
  {
    return std::make_unique<low_papr_sequence_generator_impl>();
  }
};

class pseudo_random_generator_sw_factory : public pseudo_random_generator_factory
{
public:
  std::unique_ptr<pseudo_random_generator> create() override
  {
#ifdef __x86_64__
    if (cpu_supports_feature(cpu_feature::avx2)) {
      return std::make_unique<pseudo_random_generator_avx2_impl>();
    }
#endif // __x86_64__

    return std::make_unique<pseudo_random_generator_impl>();
  }
};

} // namespace

std::shared_ptr<low_papr_sequence_collection_factory>
ocudu::create_low_papr_sequence_collection_sw_factory(std::shared_ptr<low_papr_sequence_generator_factory> lpg_factory)
{
  return std::make_shared<low_papr_sequence_collection_sw_factory>(lpg_factory);
}

std::shared_ptr<low_papr_sequence_generator_factory> ocudu::create_low_papr_sequence_generator_sw_factory()
{
  return std::make_shared<low_papr_sequence_generator_sw_factory>();
}

std::shared_ptr<pseudo_random_generator_factory> ocudu::create_pseudo_random_generator_sw_factory()
{
  return std::make_shared<pseudo_random_generator_sw_factory>();
}
