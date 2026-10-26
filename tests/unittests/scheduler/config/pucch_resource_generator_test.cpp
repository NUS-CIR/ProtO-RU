// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/scheduler/support/pucch/pucch_collision.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/pucch/pucch_configuration.h"
#include "ocudu/ran/pucch/pucch_constants.h"
#include "ocudu/ran/pucch/pucch_mapping.h"
#include "ocudu/scheduler/config/cell_bwp_res_config.h"
#include "ocudu/scheduler/config/pucch_default_resource.h"
#include "ocudu/scheduler/config/pucch_resource_builder_params.h"
#include "ocudu/scheduler/config/pucch_resource_generator.h"
#include "ocudu/scheduler/config/ran_cell_config.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_builder.h"
#include "ocudu/scheduler/config/ue_bwp_config.h"
#include "fmt/format.h"
#include <gtest/gtest.h>

using namespace ocudu;

static ran_cell_config make_custom_cell_config(const pucch_resource_builder_params& builder_params)
{
  // Standard BWP configuration: 15 kHz SCS, 5 MHz BW.
  cell_config_builder_params cell_params{};
  cell_params.dl_carrier.carrier_bw = bs_channel_bandwidth::MHz5;
  cell_params.scs_common            = subcarrier_spacing::kHz15;
  auto cfg                          = config_helpers::make_default_ran_cell_config(cell_params);
  cfg.init_bwp.pucch.resources      = builder_params;
  return cfg;
}

class pucch_resource_generator_test : public ::testing::TestWithParam<pucch_resource_builder_params>
{
public:
  pucch_resource_generator_test() :
    cell_cfg(make_custom_cell_config(GetParam())),
    params(cell_cfg.init_bwp.pucch.resources),
    bwp_cfg(cell_cfg.ul_cfg_common.init_ul_bwp.generic_params),
    pucch_res_common(cell_cfg.ul_cfg_common.init_ul_bwp.pucch_cfg_common.value().pucch_resource_common),
    dedicated_pucch_rb_start(get_pucch_default_nof_edge_prbs(pucch_res_common, bwp_cfg.crbs.length()))
  {
  }

protected:
  const ran_cell_config               cell_cfg;
  const pucch_resource_builder_params params;
  const bwp_configuration             bwp_cfg;
  const unsigned                      pucch_res_common;
  const unsigned                      dedicated_pucch_rb_start;
};

TEST_P(pucch_resource_generator_test, generated_resources_are_consistent_with_parameters)
{
  std::vector<pucch_resource> res_list =
      config_helpers::generate_cell_pucch_res_list(params, bwp_cfg.crbs.length(), dedicated_pucch_rb_start);
  const unsigned nof_res_f0_f1 =
      params.nof_cell_sr_resources + params.nof_cell_res_set_configs * params.res_set_size.value();
  const unsigned nof_res_f2_f3_f4 =
      params.nof_cell_csi_resources + params.nof_cell_res_set_configs * params.res_set_size.value();
  if (params.format_01() == pucch_format::FORMAT_0 and params.format_234() == pucch_format::FORMAT_2) {
    ASSERT_EQ(nof_res_f0_f1 + nof_res_f2_f3_f4 + params.nof_cell_sr_resources + params.nof_cell_csi_resources,
              res_list.size());
  } else {
    ASSERT_EQ(nof_res_f0_f1 + nof_res_f2_f3_f4, res_list.size());
  }

  for (const auto& res : span<const pucch_resource>(res_list.data(), nof_res_f0_f1 + nof_res_f2_f3_f4)) {
    if (res.format() == params.format_01()) {
      ASSERT_EQ(params.nof_syms_01(), res.syms.length());
      ASSERT_EQ(params.intraslot_freq_hopping_01(), res.second_hop_prb.has_value());
    } else if (res.format() == params.format_234()) {
      ASSERT_EQ(params.nof_syms_234(), res.syms.length());
      ASSERT_EQ(params.intraslot_freq_hopping_234(), res.second_hop_prb.has_value());
    } else {
      FAIL() << "Unexpected PUCCH format in generated resource list.";
    }
  }
}

TEST_P(pucch_resource_generator_test, successful_generation_results_in_no_collisions)
{
  std::vector<pucch_resource> res_list =
      config_helpers::generate_cell_pucch_res_list(params, bwp_cfg.crbs.length(), dedicated_pucch_rb_start);
  ASSERT_FALSE(res_list.empty());

  // Not all resources are checked for the F0+F2 case, since the extra resources there (SR_F0 and CSI_F2) are generated
  // so that they collide with their respective SR/CSI resources.
  const unsigned nof_res_f0_f1 =
      params.nof_cell_sr_resources + params.nof_cell_res_set_configs * params.res_set_size.value();
  const unsigned nof_res_f2_f3_f4 =
      params.nof_cell_csi_resources + params.nof_cell_res_set_configs * params.res_set_size.value();
  for (unsigned i = 0; i != nof_res_f0_f1 + nof_res_f2_f3_f4; ++i) {
    for (unsigned j = i + 1; j != nof_res_f0_f1 + nof_res_f2_f3_f4; ++j) {
      ASSERT_FALSE(pucch_resources_collide(res_list[i], res_list[j]));
    }
  }
}

/// Returns the PRBs (first and second hop) occupied by a PUCCH resource.
static std::vector<unsigned> get_res_prbs(const pucch_resource& res)
{
  std::vector<unsigned> prbs;
  const prb_interval    first_hop = res.prbs();
  for (unsigned prb = first_hop.start(); prb != first_hop.stop(); ++prb) {
    prbs.push_back(prb);
  }
  if (res.second_hop_prb.has_value()) {
    for (unsigned prb = *res.second_hop_prb; prb != *res.second_hop_prb + first_hop.length(); ++prb) {
      prbs.push_back(prb);
    }
  }
  return prbs;
}

/// Checks that no dedicated resource shares PRBs with the common resources.
static void assert_no_prb_overlap_with_common(span<const pucch_resource> ded_res_list,
                                              unsigned                   pucch_res_common,
                                              unsigned                   bwp_size_rbs)
{
  std::vector<bool> common_prbs(bwp_size_rbs, false);
  for (const auto& res : config_helpers::generate_cell_common_pucch_res_list(pucch_res_common, bwp_size_rbs)) {
    for (unsigned prb : get_res_prbs(res)) {
      common_prbs[prb] = true;
    }
  }
  for (const auto& res : ded_res_list) {
    for (unsigned prb : get_res_prbs(res)) {
      ASSERT_LT(prb, bwp_size_rbs);
      ASSERT_FALSE(common_prbs[prb]) << fmt::format(
          "Dedicated resource cell_res_id={} overlaps common prb={}", res.res_id.ded().cell_res_id, prb);
    }
  }
}

TEST_P(pucch_resource_generator_test, dedicated_resources_do_not_overlap_common_resources)
{
  const std::vector<pucch_resource> res_list =
      config_helpers::generate_cell_pucch_res_list(params, bwp_cfg.crbs.length(), dedicated_pucch_rb_start);
  ASSERT_FALSE(res_list.empty());
  assert_no_prb_overlap_with_common(res_list, pucch_res_common, bwp_cfg.crbs.length());
}

TEST(pucch_resource_generator_common_res_test, dedicated_resources_do_not_overlap_any_common_resource_row)
{
  static constexpr unsigned bwp_size_rbs = 106;
  for (unsigned pucch_res_common = 0; pucch_res_common != 16; ++pucch_res_common) {
    const std::vector<pucch_resource> res_list = config_helpers::generate_cell_pucch_res_list(
        pucch_resource_builder_params{}, bwp_size_rbs, get_pucch_default_nof_edge_prbs(pucch_res_common, bwp_size_rbs));
    ASSERT_FALSE(res_list.empty());
    assert_no_prb_overlap_with_common(res_list, pucch_res_common, bwp_size_rbs);
  }
}

TEST(pucch_resource_generator_edge_prbs_test, dedicated_resources_leave_configured_edge_prbs_free)
{
  ran_cell_config cell_cfg          = config_helpers::make_default_ran_cell_config(cell_config_builder_params{});
  const unsigned  bwp_size_rbs      = cell_cfg.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  const unsigned  default_edge_prbs = get_pucch_default_nof_edge_prbs(
      cell_cfg.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common, bwp_size_rbs);
  const unsigned nof_edge_prbs                    = default_edge_prbs + 3;
  cell_cfg.init_bwp.pucch.resources.nof_edge_prbs = nof_edge_prbs;

  const std::vector<pucch_resource> res_list = make_cell_bwp_res_config(cell_cfg).ul.pucch.dedicated;
  ASSERT_FALSE(res_list.empty());
  unsigned min_prb = bwp_size_rbs;
  unsigned max_prb = 0;
  for (const auto& res : res_list) {
    for (unsigned prb : get_res_prbs(res)) {
      min_prb = std::min(min_prb, prb);
      max_prb = std::max(max_prb, prb);
    }
  }
  ASSERT_EQ(min_prb, nof_edge_prbs);
  ASSERT_EQ(max_prb, bwp_size_rbs - 1 - nof_edge_prbs);
}

TEST_P(pucch_resource_generator_test, f1_resources_have_valid_occ_indices)
{
  const std::vector<pucch_resource> res_list =
      config_helpers::generate_cell_pucch_res_list(params, bwp_cfg.crbs.length(), dedicated_pucch_rb_start);
  ASSERT_FALSE(res_list.empty());

  for (const auto& res : res_list) {
    if (res.format() != pucch_format::FORMAT_1) {
      continue;
    }
    // As per Table 6.3.2.4.1-1, TS 38.211, the OCC index must be lower than the spreading factor of every hop.
    const unsigned nof_syms = res.syms.length();
    const unsigned nof_occs = res.second_hop_prb.has_value() ? nof_syms / 4 : nof_syms / 2;
    const auto&    f1_cfg   = std::get<pucch_resource::f1_config>(res.format_params);
    ASSERT_LT(f1_cfg.time_domain_occ, nof_occs)
        << fmt::format("Invalid OCC index for cell_res_id={} with nof_syms={}", res.res_id.ded().cell_res_id, nof_syms);
  }
}

TEST_P(pucch_resource_generator_test, ue_pucch_config_builder_test)
{
  const auto cell_res_list =
      config_helpers::generate_cell_pucch_res_list(params, bwp_cfg.crbs.length(), dedicated_pucch_rb_start);
  const bool using_02 = params.format_01() == pucch_format::FORMAT_0 and params.format_234() == pucch_format::FORMAT_2;

  for (auto res_set_cfg_id     = pucch_resource_set_config_id(0),
            res_set_cfg_id_end = pucch_resource_set_config_id(params.nof_cell_res_set_configs);
       res_set_cfg_id != res_set_cfg_id_end;
       ++res_set_cfg_id) {
    for (auto sr_res_id = pucch_sr_resource_id(0), sr_res_id_end = pucch_sr_resource_id(params.nof_cell_sr_resources);
         sr_res_id != sr_res_id_end;
         ++sr_res_id) {
      for (auto csi_res_id     = pucch_csi_resource_id(0),
                csi_res_id_end = pucch_csi_resource_id(params.nof_cell_csi_resources);
           csi_res_id != csi_res_id_end;
           ++csi_res_id) {
        const ue_bwp_config ue_bwp_cfg{
            .ul =
                {
                    .pucch = {.res_set_cfg_id = res_set_cfg_id, .sr_res_id = sr_res_id},
                    .csi   = {.periodic_report = ue_periodic_csi_config{.pucch_res_id = csi_res_id}},
                },
        };

        const auto pucch_cfg =
            config_helpers::build_pucch_config(cell_cfg, make_cell_bwp_res_config(cell_cfg).ul, ue_bwp_cfg);

        static constexpr unsigned nof_sr_res_per_ue  = 1;
        const unsigned            nof_csi_res_per_ue = params.nof_cell_csi_resources != 0 ? 1 : 0;
        // For F0+F2 case:
        // - Extra resources are added to the UE resource list (SR_F2 and optionally CSI_F0).
        // - Extra resources are added to Resource Set ID 0 (SR_F0 and optionally CSI_F0).
        // - Extra resources are added to Resource Set ID 1 (SR_F2 and optionally CSI_F2).
        const unsigned extra_res_in_res_list = using_02 ? nof_sr_res_per_ue + nof_csi_res_per_ue : 0;
        const unsigned extra_res_per_res_set = using_02 ? nof_sr_res_per_ue + nof_csi_res_per_ue : 0;

        ASSERT_EQ(params.res_set_size.value() + nof_sr_res_per_ue + params.res_set_size.value() + nof_csi_res_per_ue +
                      extra_res_in_res_list,
                  pucch_cfg.pucch_res_list.size());

        auto are_cell_and_ue_resources_equal = [&cell_res_list, &pucch_cfg](const pucch_res_id_t& res_id) {
          return cell_res_list[res_id.ded().cell_res_id] == pucch_cfg.pucch_res_list[res_id.ded().ue_res_id];
        };

        // Check the SR resource.
        const pucch_res_id_t expected_sr_res_id = params.sr_res_id(sr_res_id);
        ASSERT_EQ(1U, pucch_cfg.sr_res_list.size());
        ASSERT_EQ(pucch_cfg.sr_res_list.front().pucch_res_id, expected_sr_res_id);
        ASSERT_TRUE(are_cell_and_ue_resources_equal(expected_sr_res_id));

        // Check the CSI resource (if configured).
        const auto csi_meas_cfg =
            config_helpers::build_csi_meas_config(cell_cfg, make_cell_bwp_res_config(cell_cfg).ul, ue_bwp_cfg);
        ASSERT_EQ(params.nof_cell_csi_resources != 0, csi_meas_cfg.has_value());
        if (params.nof_cell_csi_resources != 0) {
          const pucch_res_id_t expected_csi_res_id = params.csi_res_id(csi_res_id);
          ASSERT_EQ(1U, csi_meas_cfg->csi_report_cfg_list.size());
          const auto& periodic_report_cfg = std::get<csi_report_config::periodic_or_semi_persistent_report_on_pucch>(
              csi_meas_cfg->csi_report_cfg_list.front().report_cfg_type);
          ASSERT_EQ(1U, periodic_report_cfg.pucch_csi_res_list.size());
          ASSERT_EQ(expected_csi_res_id, periodic_report_cfg.pucch_csi_res_list.front().pucch_res_id);
          ASSERT_TRUE(are_cell_and_ue_resources_equal(expected_csi_res_id));
        }

        // Check Resource Set ID 0.
        const auto& res_set_0 = pucch_cfg.pucch_res_set[0];
        ASSERT_EQ(pucch_res_set_id::set_0, res_set_0.id);
        ASSERT_EQ(params.res_set_size.value() + extra_res_per_res_set, res_set_0.resources.size());

        for (unsigned pri = 0; pri != params.res_set_size.value(); ++pri) {
          const pucch_res_id_t expected_res_id = params.harq_res_id<0>(res_set_cfg_id, pri);
          ASSERT_EQ(expected_res_id, res_set_0.resources[pri]);
          ASSERT_TRUE(are_cell_and_ue_resources_equal(expected_res_id));
        }

        // Check Resource Set ID 1.
        const auto& res_set_1 = pucch_cfg.pucch_res_set[1];
        ASSERT_EQ(pucch_res_set_id::set_1, res_set_1.id);
        ASSERT_EQ(params.res_set_size.value() + extra_res_per_res_set, res_set_1.resources.size());

        for (unsigned pri = 0; pri != params.res_set_size.value(); ++pri) {
          const pucch_res_id_t expected_res_id = params.harq_res_id<1>(res_set_cfg_id, pri);
          ASSERT_EQ(expected_res_id, res_set_1.resources[pri]);
          ASSERT_TRUE(are_cell_and_ue_resources_equal(expected_res_id));
        }

        if (using_02) {
          // Check SR_F0 and SR_F2.
          ASSERT_EQ(expected_sr_res_id, res_set_0.resources[params.res_set_size.value()]);
          const pucch_res_id_t expected_id_sr_f2 = params.sr_f2_res_id(sr_res_id);
          ASSERT_EQ(expected_id_sr_f2, res_set_1.resources[params.res_set_size.value()]);
          ASSERT_TRUE(are_cell_and_ue_resources_equal(expected_id_sr_f2));

          if (params.nof_cell_csi_resources != 0) {
            // Check CSI_F0 and CSI_F2.
            const pucch_res_id_t expected_id_csi_f0 = params.csi_f0_res_id(csi_res_id);
            ASSERT_EQ(expected_id_csi_f0, res_set_0.resources[params.res_set_size.value() + 1U]);
            ASSERT_TRUE(are_cell_and_ue_resources_equal(expected_id_csi_f0));

            const pucch_res_id_t expected_csi_res_id = params.csi_res_id(csi_res_id);
            ASSERT_EQ(expected_csi_res_id, res_set_1.resources[params.res_set_size.value() + 1U]);
          }
        }
      }
    }
  }
}

static constexpr pucch_f0_params f0_max_density{
    .nof_syms = 1,
};
static constexpr pucch_f0_params f0_freq_hop{
    .nof_syms               = 2,
    .intraslot_freq_hopping = true,
};

static constexpr pucch_f1_params f1_low_density{
    .nof_syms       = pucch_constants::f1::MAX_NOF_SYMS,
    .nof_cyc_shifts = pucch_nof_cyclic_shifts::no_cyclic_shift,
    .occ_supported  = false,

};
static constexpr pucch_f1_params f1_high_density{
    .nof_syms       = pucch_constants::f1::MIN_NOF_SYMS,
    .nof_cyc_shifts = pucch_nof_cyclic_shifts::twelve,
    .occ_supported  = true,
};
static constexpr pucch_f1_params f1_freq_hop{
    .nof_syms               = pucch_constants::f1::MAX_NOF_SYMS,
    .intraslot_freq_hopping = true,
    .nof_cyc_shifts         = pucch_nof_cyclic_shifts::two,
    .occ_supported          = true,
};
static constexpr pucch_f1_params f1_12_syms{
    .nof_syms       = 12,
    .nof_cyc_shifts = pucch_nof_cyclic_shifts::twelve,
    .occ_supported  = true,
};

static constexpr pucch_f2_params f2_multiple_rbs{
    .max_nof_rbs = 3,
};
static constexpr pucch_f2_params f2_freq_hop{
    .nof_syms               = 2,
    .intraslot_freq_hopping = true,
};

static constexpr pucch_f3_params f3_low_density{.nof_syms = 7, .max_nof_rbs = 1, .intraslot_freq_hopping = true};
static constexpr pucch_f3_params f3_high_density{.nof_syms = pucch_constants::f3::MIN_NOF_SYMS};

static constexpr pucch_f4_params f4_low_density{.nof_syms = 7, .intraslot_freq_hopping = true};
static constexpr pucch_f4_params f4_high_density{.nof_syms      = pucch_constants::f3::MIN_NOF_SYMS,
                                                 .occ_supported = true,
                                                 .occ_length    = pucch_f4_occ_len::n4};

namespace ocudu {

void PrintTo(const pucch_resource_builder_params& value, ::std::ostream* os)
{
  if (std::holds_alternative<pucch_f0_params>(value.f0_or_f1_params)) {
    const auto& f0_params = std::get<pucch_f0_params>(value.f0_or_f1_params);
    if (f0_params == f0_max_density) {
      *os << "F0 (max density)";
    } else if (f0_params == f0_freq_hop) {
      *os << "F0 (freq hop)";
    } else {
      *os << "F0 (unknown)";
    }
  } else {
    const auto& f1_params = std::get<pucch_f1_params>(value.f0_or_f1_params);
    if (f1_params == f1_low_density) {
      *os << "F1 (low density)";
    } else if (f1_params == f1_high_density) {
      *os << "F1 (high density)";
    } else if (f1_params == f1_freq_hop) {
      *os << "F1 (freq hop)";
    } else if (f1_params == f1_12_syms) {
      *os << "F1 (12 symbols)";
    } else {
      *os << "F1 (unknown)";
    }
  }

  *os << " + ";

  if (std::holds_alternative<pucch_f2_params>(value.f2_or_f3_or_f4_params)) {
    const auto& f2_params = std::get<pucch_f2_params>(value.f2_or_f3_or_f4_params);
    if (f2_params == f2_multiple_rbs) {
      *os << "F2 (multiple RBs)";
    } else if (f2_params == f2_freq_hop) {
      *os << "F2 (freq hop)";
    } else {
      *os << "F2 (unknown)";
    }
  } else if (std::holds_alternative<pucch_f3_params>(value.f2_or_f3_or_f4_params)) {
    const auto& f3_params = std::get<pucch_f3_params>(value.f2_or_f3_or_f4_params);
    if (f3_params == f3_low_density) {
      *os << "F3 (low density)";
    } else if (f3_params == f3_high_density) {
      *os << "F3 (high density)";
    } else {
      *os << "F3 (unknown)";
    }
  } else {
    const auto& f4_params = std::get<pucch_f4_params>(value.f2_or_f3_or_f4_params);
    if (f4_params == f4_low_density) {
      *os << "F4 (low density)";
    } else if (f4_params == f4_high_density) {
      *os << "F4 (high density)";
    } else {
      *os << "F4 (unknown)";
    }
  }
}

} // namespace ocudu

// The test cases are designed with the following considerations:
// - Test different combinations of PUCCH Formats and parameters.
// - The number of resources are sized s.t. the PUCCH cover as much of the BWP as possible (<= 50%), so that we can:
//   - Increase the chance of detecting regressions in the resource generation logic.
//   - See the effect of the format parameters on the number of resources (and consequenty on the UE cell capacity).
INSTANTIATE_TEST_SUITE_P(,
                         pucch_resource_generator_test,
                         ::testing::Values(
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 1,
                                 .nof_cell_sr_resources    = 3,
                                 .nof_cell_csi_resources   = 1,
                                 .f0_or_f1_params          = f1_low_density,
                                 .f2_or_f3_or_f4_params    = f2_multiple_rbs,
                             },
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 10,
                                 .nof_cell_sr_resources    = 84,
                                 .nof_cell_csi_resources   = 10,
                                 .f0_or_f1_params          = f1_high_density,
                                 .f2_or_f3_or_f4_params    = f2_freq_hop,
                             },
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 2,
                                 .nof_cell_sr_resources    = 2,
                                 .nof_cell_csi_resources   = 9,
                                 .f0_or_f1_params          = f0_freq_hop,
                                 .f2_or_f3_or_f4_params    = f2_multiple_rbs,
                             },
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 8,
                                 .nof_cell_sr_resources    = 8,
                                 .nof_cell_csi_resources   = 8,
                                 .f0_or_f1_params          = f0_max_density,
                                 .f2_or_f3_or_f4_params    = f2_freq_hop,
                             },
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 1,
                                 .nof_cell_sr_resources    = 2,
                                 .nof_cell_csi_resources   = 2,
                                 .f0_or_f1_params          = f1_low_density,
                                 .f2_or_f3_or_f4_params    = f3_low_density,
                             },
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 5,
                                 .nof_cell_sr_resources    = 42,
                                 .nof_cell_csi_resources   = 3,
                                 .f0_or_f1_params          = f1_high_density,
                                 .f2_or_f3_or_f4_params    = f3_high_density,
                             },
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 1,
                                 .nof_cell_sr_resources    = 2,
                                 .nof_cell_csi_resources   = 2,
                                 .f0_or_f1_params          = f1_low_density,
                                 .f2_or_f3_or_f4_params    = f4_low_density,
                             },
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 20,
                                 .nof_cell_sr_resources    = 8,
                                 .nof_cell_csi_resources   = 8,
                                 .f0_or_f1_params          = f1_high_density,
                                 .f2_or_f3_or_f4_params    = f4_high_density,
                             },
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 2,
                                 .nof_cell_sr_resources    = 20,
                                 .nof_cell_csi_resources   = 2,
                                 .f0_or_f1_params          = f1_freq_hop,
                                 .f2_or_f3_or_f4_params    = f2_freq_hop,
                             },
                             // PUCCH limited to 12 symbols, e.g., when SRS takes the last symbols of the slot.
                             pucch_resource_builder_params{
                                 .nof_cell_res_set_configs = 2,
                                 .nof_cell_sr_resources    = 72,
                                 .nof_cell_csi_resources   = 2,
                                 .f0_or_f1_params          = f1_12_syms,
                                 .f2_or_f3_or_f4_params    = f2_multiple_rbs,
                                 .max_nof_symbols          = 12,
                             }));
