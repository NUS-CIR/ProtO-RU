// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "../test_utils/config_generators.h"
#include "lib/scheduler/pdcch_scheduling/pdcch_resource_allocator_impl.h"
#include "lib/scheduler/pdcch_scheduling/pdcch_slot_resource_allocator.h"
#include "lib/scheduler/support/pdcch/pdcch_mapping.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/pdcch/pdcch_candidates.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/sched_pdcch_config.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include "ocudu/scheduler/config/scheduler_ue_config_validator.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include "ocudu/scheduler/scheduler_configurator.h"
#include "fmt/std.h"
#include <array>
#include <gtest/gtest.h>
#include <initializer_list>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace ocudu;

enum class alloc_type { si_rnti, ra_rnti, dl_crnti, ul_crnti };
const char* to_string(alloc_type a)
{
  switch (a) {
    case alloc_type::si_rnti:
      return "si-rnti";
    case alloc_type::ra_rnti:
      return "ra-rnti";
    case alloc_type::dl_crnti:
      return "DL c-rnti";
    case alloc_type::ul_crnti:
      return "UL c-rnti";
    default:
      break;
  }
  return "invalid";
}

using cell_bw = bs_channel_bandwidth;

namespace pdcch_test {

class base_pdcch_resource_allocator_tester
{
protected:
  struct test_ue {
    rnti_t                       rnti;
    const ue_configuration*      cfg;
    const ue_cell_configuration* pcell_cfg;

    test_ue(const ue_configuration& ue_cfg) : rnti(ue_cfg.crnti), cfg(&ue_cfg), pcell_cfg(&cfg->pcell_cfg()) {}
  };

  base_pdcch_resource_allocator_tester(rnti_t                                          crnti = to_rnti(0x4601),
                                       const sched_cell_configuration_request_message& msg =
                                           sched_config_helper::make_default_sched_cell_configuration_request(),
                                       ue_cell_config ue_cell = config_helpers::make_default_ue_cell_config(
                                           sched_config_helper::make_default_sched_cell_configuration_request().ran)) :
    cell_cfg(*sched_cfg_mng.add_cell(msg)), default_ue_cell_req(std::move(ue_cell))
  {
    ocudulog::fetch_basic_logger("SCHED", true).set_level(ocudulog::basic_levels::debug);
    test_logger.set_level(ocudulog::basic_levels::debug);
    ocudulog::init();

    run_slot();
  }

  void run_slot()
  {
    logger.set_context(next_slot.sfn(), next_slot.slot_index());
    res_grid.slot_indication(next_slot);
    pdcch_sch.slot_indication(next_slot);
    ++next_slot;
  }

  test_ue* add_ue(const sched_ue_creation_request_message& ue_creation_req)
  {
    auto result = config_validators::validate_sched_ue_creation_request_message(ue_creation_req, cell_cfg);
    report_error_if_not(result.has_value(), "UE creation request validation failed: {}", result.error());
    const ue_configuration* ue_cfg = sched_cfg_mng.add_ue(ue_creation_req);
    report_error_if_not(ue_cfg != nullptr, "Failed to create UE configuration");
    return &test_ues.insert(std::make_pair(ue_creation_req.crnti, test_ue{*ue_cfg})).first->second;
  }

  sched_ue_creation_request_message create_ue_cfg(rnti_t rnti)
  {
    sched_ue_creation_request_message ue_creation_req = sched_cfg_mng.get_default_ue_config_request();
    ue_creation_req.ue_index                          = to_du_ue_index((unsigned)rnti - 0x4601);
    ue_creation_req.crnti                             = rnti;
    ue_creation_req.starts_in_fallback                = false;
    (*ue_creation_req.cfg.cells)[0]                   = make_sched_ue_cell_config(default_ue_cell_req);
    return ue_creation_req;
  }

  bool is_pdcch_monitored(rnti_t crnti, search_space_id ss_id) const
  {
    const ue_cell_configuration& uecfg = *test_ues.at(crnti).pcell_cfg;
    return pdcch_helper::is_pdcch_monitoring_active(res_grid[0].slot, *uecfg.search_space(ss_id).cfg) and
           not uecfg.search_space(ss_id).get_pdcch_candidates(aggregation_level::n4, res_grid[0].slot).empty();
  }

  void verify_pdcch_context(const dci_context_information& pdcch_ctx, const test_ue& u, search_space_id ss_id) const
  {
    ASSERT_EQ(pdcch_ctx.rnti, u.rnti);
    const search_space_configuration& ss_cfg = *u.pcell_cfg->search_space(ss_id).cfg;
    const coreset_configuration&      cs_cfg = u.pcell_cfg->coreset(ss_cfg.get_coreset_id()).cfg();
    ASSERT_EQ(*pdcch_ctx.coreset_cfg, cs_cfg);
    ASSERT_EQ(pdcch_ctx.n_id_pdcch_dmrs,
              cs_cfg.get_pdcch_dmrs_scrambling_id().has_value() ? *cs_cfg.get_pdcch_dmrs_scrambling_id()
                                                                : cell_cfg.params.pci)
        << "Invalid N_{ID} (see TS38.211, 7.4.1.3.1)";
    ASSERT_EQ(pdcch_ctx.n_rnti_pdcch_data,
              cs_cfg.get_pdcch_dmrs_scrambling_id().has_value() and (not ss_cfg.is_common_search_space())
                  ? to_underlying(u.rnti)
                  : 0)
        << "Invalid n_{RNTI} (see TS38.211, 7.3.2.3)";
    unsigned expected_n_id = cs_cfg.get_pdcch_dmrs_scrambling_id().has_value() and (not ss_cfg.is_common_search_space())
                                 ? *cs_cfg.get_pdcch_dmrs_scrambling_id()
                                 : cell_cfg.params.pci;
    ASSERT_EQ(pdcch_ctx.n_id_pdcch_data, expected_n_id) << "Invalid n_{ID} (see TS38.211, 7.3.2.3)";

    // The PDCCH is neither precoded nor beamformed.
    ASSERT_TRUE(std::holds_alternative<precoding_matrix_indicator>(pdcch_ctx.precoding_and_beamforming));
    ASSERT_TRUE(std::holds_alternative<std::monostate>(
        std::get<precoding_matrix_indicator>(pdcch_ctx.precoding_and_beamforming)));

    auto ncce_candidates =
        get_ue_pdcch_candidates((next_slot - 1).slot_index(), u.rnti, cs_cfg, ss_cfg, pdcch_ctx.cces.aggr_lvl);
    ASSERT_TRUE(std::any_of(ncce_candidates.begin(), ncce_candidates.end(), [&pdcch_ctx](auto ncce) {
      return ncce == pdcch_ctx.cces.ncce;
    })) << "Chosen ncce is not in the list of candidates";
  }

  pdcch_candidate_list get_common_pdcch_candidates(const coreset_configuration&      cs_cfg,
                                                   const search_space_configuration& ss_cfg,
                                                   aggregation_level                 aggr_lvl) const
  {
    return pdcch_candidates_common_ss_get_lowest_cce(pdcch_candidates_common_ss_configuration{
        aggr_lvl, ss_cfg.get_nof_candidates()[to_aggregation_level_index(aggr_lvl)], cs_cfg.get_nof_cces()});
  }

  pdcch_candidate_list get_ue_pdcch_candidates(unsigned                          slot_index,
                                               rnti_t                            rnti,
                                               const coreset_configuration&      cs_cfg,
                                               const search_space_configuration& ss_cfg,
                                               aggregation_level                 aggr_lvl) const
  {
    if (ss_cfg.is_common_search_space()) {
      return get_common_pdcch_candidates(cs_cfg, ss_cfg, aggr_lvl);
    }
    return pdcch_candidates_ue_ss_get_lowest_cce(
        pdcch_candidates_ue_ss_configuration{aggr_lvl,
                                             ss_cfg.get_nof_candidates()[to_aggregation_level_index(aggr_lvl)],
                                             cs_cfg.get_nof_cces(),
                                             cs_cfg.get_id(),
                                             rnti,
                                             slot_index});
  }

  bool pdcchs_collide(const dci_context_information& pdcch_ctx1, const dci_context_information& pdcch_ctx2) const
  {
    prb_index_list pdcch_prbs1 = pdcch_helper::cce_to_prb_mapping(*pdcch_ctx1.bwp_cfg,
                                                                  *pdcch_ctx1.coreset_cfg,
                                                                  cell_cfg.params.pci,
                                                                  pdcch_ctx1.cces.aggr_lvl,
                                                                  pdcch_ctx1.cces.ncce);
    prb_index_list pdcch_prbs2 = pdcch_helper::cce_to_prb_mapping(*pdcch_ctx2.bwp_cfg,
                                                                  *pdcch_ctx2.coreset_cfg,
                                                                  cell_cfg.params.pci,
                                                                  pdcch_ctx2.cces.aggr_lvl,
                                                                  pdcch_ctx2.cces.ncce);
    for (unsigned prb1 : pdcch_prbs1) {
      if (std::find(pdcch_prbs2.begin(), pdcch_prbs2.end(), prb1) != pdcch_prbs2.end()) {
        return true;
      }
    }
    return false;
  }

  bool collisions_found() const
  {
    const auto dl_pdcchs = res_grid[0].result_reader().dl().dl_pdcchs();
    const auto ul_pdcchs = res_grid[0].result_reader().dl().ul_pdcchs();
    for (unsigned i = 0, e = dl_pdcchs.size(); i != e; ++i) {
      for (unsigned j = i + 1; j != e; ++j) {
        if (pdcchs_collide(dl_pdcchs[i].ctx, dl_pdcchs[j].ctx)) {
          return true;
        }
      }
      for (const auto& ul_pdcch : ul_pdcchs) {
        if (pdcchs_collide(dl_pdcchs[i].ctx, ul_pdcch.ctx)) {
          return true;
        }
      }
    }
    return false;
  }

  std::optional<unsigned> find_available_ncce(const bwp_configuration&          bwp_cfg,
                                              const coreset_configuration&      cs_cfg,
                                              const search_space_configuration& ss_cfg,
                                              aggregation_level                 aggr_lvl)
  {
    auto ncce_candidates = this->get_common_pdcch_candidates(cs_cfg, ss_cfg, aggr_lvl);
    for (auto ncce : ncce_candidates) {
      prb_index_list pdcch_prbs =
          pdcch_helper::cce_to_prb_mapping(bwp_cfg, cs_cfg, cell_cfg.params.pci, aggr_lvl, ncce);
      bool success = true;
      for (unsigned prb : pdcch_prbs) {
        unsigned crb = prb_to_crb(bwp_cfg, prb);
        if (res_grid[0].dl_res_grid.collides(bwp_cfg.scs, {0, cs_cfg.duration()}, crb_interval{crb, crb + 1})) {
          success = false;
          break;
        }
      }
      if (success) {
        return ncce;
      }
    }
    return std::nullopt;
  }

  void print_cfg()
  {
    fmt::memory_buffer fmtbuf;
    fmt::format_to(std::back_inserter(fmtbuf), "\nTest config:");
    fmt::format_to(std::back_inserter(fmtbuf),
                   "\n- initial BWP: RBs={}",
                   cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.crbs);
    const sched_bwp_config&      init_bwp = cell_cfg.init_bwp;
    const coreset_configuration& cs0_cfg  = init_bwp.dl.pdcch().coresets()[to_coreset_id(0)]->cfg();
    fmt::format_to(
        std::back_inserter(fmtbuf), "\n- CORESET#0: RBs={}, duration={}", cs0_cfg.coreset0_crbs(), cs0_cfg.duration());
    const auto& ue_ded_pdcch = *default_ue_cell_req.serv_cell_cfg.init_dl_bwp.pdcch_cfg;
    const auto& cs1_cfg      = ue_ded_pdcch.coresets.front();
    fmt::format_to(std::back_inserter(fmtbuf),
                   "\n- CORESET#1: RBs={}, duration={}",
                   get_coreset_crbs(cs1_cfg),
                   cs1_cfg.duration());
    fmt::format_to(
        std::back_inserter(fmtbuf),
        "\n- SearchSpace#0: nof_candidates={}",
        fmt::join(init_bwp.dl.pdcch().search_spaces()[to_search_space_id(0)]->cfg().get_nof_candidates(), ", "));
    fmt::format_to(
        std::back_inserter(fmtbuf),
        "\n- SearchSpace#1: nof_candidates={}",
        fmt::join(init_bwp.dl.pdcch().search_spaces()[to_search_space_id(1)]->cfg().get_nof_candidates(), ", "));
    fmt::format_to(std::back_inserter(fmtbuf),
                   "\n- SearchSpace#2: nof_candidates={}",
                   fmt::join(ue_ded_pdcch.search_spaces[0].get_nof_candidates(), ", "));
    test_logger.info("{}", to_string(fmtbuf));
  }

  ocudulog::basic_logger&                 logger      = ocudulog::fetch_basic_logger("SCHED");
  ocudulog::basic_logger&                 test_logger = ocudulog::fetch_basic_logger("TEST");
  const scheduler_expert_config           sched_cfg   = config_helpers::make_default_scheduler_expert_config();
  test_helpers::test_sched_config_manager sched_cfg_mng{{}, sched_cfg};
  const cell_configuration&               cell_cfg;
  const ue_cell_config                    default_ue_cell_req;

  cell_resource_allocator res_grid{cell_cfg};

  pdcch_resource_allocator_impl pdcch_sch{cell_cfg};

  slot_point next_slot{0, test_rng::uniform_int<unsigned>(0, 10239)};

  std::unordered_map<rnti_t, test_ue> test_ues;
};

class common_pdcch_allocator_tester : public base_pdcch_resource_allocator_tester, public ::testing::Test
{};

struct test_scrambling_params {
  search_space_id                    ss_id;
  search_space_configuration::type_t ss2_type;
  std::optional<unsigned>            cs1_pdcch_dmrs_scrambling_id;
};

// Dummy function overload of template <typename T> void testing::internal::PrintTo(const T& value, ::std::ostream* os).
// This prevents valgrind from complaining about uninitialized variables.
void PrintTo(const test_scrambling_params& value, ::std::ostream* os)
{
  return;
}
} // namespace pdcch_test

using namespace pdcch_test;

class pdcch_backtracking_test : public base_pdcch_resource_allocator_tester, public ::testing::Test
{
protected:
  struct request {
    dci_context_information           ctx{};
    std::vector<pdcch_candidate_type> candidates;

    request(const bwp_configuration&       bwp,
            const sched_coreset_config&    cs,
            std::initializer_list<uint8_t> starts,
            aggregation_level              level = aggregation_level::n2) :
      candidates(starts)
    {
      ctx.bwp_cfg       = &bwp;
      ctx.coreset_cfg   = &cs.cfg();
      ctx.cces.aggr_lvl = level;
    }
  };

  static sched_coreset_config make_coreset(const bwp_configuration& bwp, pci_t pci)
  {
    freq_resource_bitmap resources(pdcch_constants::MAX_NOF_FREQ_RESOURCES);
    resources.fill(0, 16, true);
    coreset_configuration cs{to_coreset_id(1),
                             resources,
                             1,
                             std::nullopt,
                             coreset_configuration::precoder_granularity_type::same_as_reg_bundle};

    std::array<std::vector<crb_index_list>, NOF_AGGREGATION_LEVELS> candidate_crbs;
    for (unsigned i = 0; i != NOF_AGGREGATION_LEVELS; ++i) {
      const aggregation_level level = aggregation_index_to_level(i);
      for (unsigned ncce = 0; ncce < cs.get_nof_cces(); ncce += to_nof_cces(level)) {
        candidate_crbs[i].push_back(pdcch_helper::cce_to_prb_mapping(bwp, cs, pci, level, ncce));
        for (uint16_t& prb : candidate_crbs[i].back()) {
          prb = prb_to_crb(bwp.crbs, prb);
        }
      }
    }
    return sched_coreset_config{cs, candidate_crbs};
  }

  pdcch_backtracking_test() :
    base_pdcch_resource_allocator_tester(to_rnti(0x4601),
                                         sched_config_helper::make_default_sched_cell_configuration_request(
                                             cell_config_builder_params{.dl_carrier{.carrier_bw = cell_bw::MHz20}}))
  {
    res_grid[0].dl_res_grid.fill({bwp.scs, {0, 1}, {96, 98}});
    res_grid[0].dl_res_grid.fill({bwp.scs, {3, 4}, {2, 4}});
  }

  bool allocate(request& r, bool movable = true)
  {
    return allocator.alloc_pdcch(r.ctx, res_grid[0], cs, ss, r.candidates, movable);
  }

  cell_slot_resource_grid snapshot() { return res_grid[0].dl_res_grid; }

  const bwp_configuration&          bwp = cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params;
  const sched_coreset_config        cs  = make_coreset(bwp, cell_cfg.params.pci);
  const search_space_configuration& ss  = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[0];
  pdcch_slot_allocator              allocator;
};

TEST_F(pdcch_backtracking_test, moves_existing_grant_and_releases_old_position)
{
  request a(bwp, cs, {0, 2});
  request b(bwp, cs, {0}, aggregation_level::n1);
  auto    expected = snapshot();
  ASSERT_TRUE(allocate(a));
  ASSERT_EQ(a.ctx.cces.ncce, 0U);
  ASSERT_TRUE(allocate(b));
  EXPECT_EQ(a.ctx.cces.ncce, 2U);
  EXPECT_EQ(b.ctx.cces.ncce, 0U);
  EXPECT_FALSE(res_grid[0].dl_res_grid.collides(bwp.scs, {0, 1}, crb_interval{6, 12}));
  expected.fill(bwp.scs, {0, 1}, cs.candidate_crbs(2, aggregation_level::n2));
  expected.fill(bwp.scs, {0, 1}, cs.candidate_crbs(0, aggregation_level::n1));
  EXPECT_EQ(snapshot(), expected);
}

TEST_F(pdcch_backtracking_test, failed_repacking_restores_entire_grid_and_context)
{
  request a(bwp, cs, {0, 2});
  request b(bwp, cs, {4, 6});
  request impossible(bwp, cs, {0}, aggregation_level::n8);
  request later(bwp, cs, {2});
  ASSERT_TRUE(allocate(a));
  ASSERT_TRUE(allocate(b));
  const auto before = snapshot();
  ASSERT_FALSE(allocate(impossible));
  EXPECT_EQ(snapshot(), before);
  EXPECT_EQ(a.ctx.cces.ncce, 0U);
  EXPECT_EQ(b.ctx.cces.ncce, 4U);
  ASSERT_TRUE(allocate(later));
}

TEST_F(pdcch_backtracking_test, cancellation_after_repacking_releases_only_cancelled_grant)
{
  request a(bwp, cs, {0, 2});
  request b(bwp, cs, {0}, aggregation_level::n1);
  auto    expected = snapshot();
  auto    dl       = res_grid[0].result_builder().dl().dl_pdcchs();
  auto&   ca       = dl.emplace_back()->ctx;
  ca               = a.ctx;
  ASSERT_TRUE(allocator.alloc_pdcch(ca, res_grid[0], cs, ss, a.candidates, true));
  auto& cb = dl.emplace_back()->ctx;
  cb       = b.ctx;
  ASSERT_TRUE(allocator.alloc_pdcch(cb, res_grid[0], cs, ss, b.candidates, true));
  ASSERT_TRUE(allocator.cancel_last_pdcch(res_grid[0]));
  ASSERT_EQ(dl.size(), 1U);
  EXPECT_EQ(ca.cces.ncce, 2U);
  EXPECT_FALSE(res_grid[0].dl_res_grid.collides(bwp.scs, {0, 1}, crb_interval{0, 12}));
  expected.fill(bwp.scs, {0, 1}, cs.candidate_crbs(2, aggregation_level::n2));
  EXPECT_EQ(snapshot(), expected);
}

TEST_F(pdcch_backtracking_test, immovable_grant_is_never_relocated)
{
  request a(bwp, cs, {0, 2});
  request b(bwp, cs, {0}, aggregation_level::n1);
  request later(bwp, cs, {2});
  ASSERT_TRUE(allocate(a, false));
  const auto before = snapshot();
  ASSERT_FALSE(allocate(b));
  EXPECT_EQ(a.ctx.cces.ncce, 0U);
  EXPECT_EQ(snapshot(), before);
  ASSERT_TRUE(allocate(later));
}

TEST_F(pdcch_backtracking_test, bounded_search_failure_preserves_existing_allocations)
{
  std::array<request, 4> accepted{request(bwp, cs, {0, 2, 4, 6, 8}),
                                  request(bwp, cs, {0, 2, 4, 6}),
                                  request(bwp, cs, {0, 2, 4, 6}),
                                  request(bwp, cs, {0, 2, 4, 6})};
  for (request& r : accepted) {
    ASSERT_TRUE(allocate(r));
  }
  const auto before = snapshot();
  // An unbounded search moves the first grant to CCE 8 after exploring permutations of CCEs 0..6.
  request limited(bwp, cs, {0});
  for (unsigned repeat = 0; repeat != 10; ++repeat) {
    ASSERT_FALSE(allocate(limited));
    EXPECT_EQ(snapshot(), before);
    for (unsigned i = 0; i != accepted.size(); ++i) {
      EXPECT_EQ(accepted[i].ctx.cces.ncce, 2U * i);
    }
  }
  request later(bwp, cs, {8});
  ASSERT_TRUE(allocate(later));
}

TEST_F(common_pdcch_allocator_tester, no_pdcch_allocation)
{
  ASSERT_TRUE(res_grid[0].result_reader().dl().dl_pdcchs().empty());
  ASSERT_TRUE(res_grid[0].result_reader().dl().ul_pdcchs().empty());
}

TEST_F(common_pdcch_allocator_tester, single_pdcch_sib1_allocation)
{
  // Run until PDCCH monitoring occasion for SIB1 is active (i.e. every 20ms)
  while (not pdcch_helper::is_pdcch_monitoring_active(
      next_slot, cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[0])) {
    run_slot();
  }
  // Since we schedule SIB1 on (n0 + 1)th slot we need to run once more.
  run_slot();

  pdcch_dl_information* pdcch = pdcch_sch.alloc_dl_pdcch_common(
      res_grid[0], rnti_t::SI_RNTI, to_search_space_id(0), aggregation_level::n4, std::nullopt);

  ASSERT_TRUE(res_grid[0].result_reader().dl().ul_pdcchs().empty());
  ASSERT_EQ(res_grid[0].result_reader().dl().dl_pdcchs().size(), 1);
  ASSERT_EQ(pdcch, &res_grid[0].result_reader().dl().dl_pdcchs()[0])
      << "Returned PDCCH ptr does not match allocated ptr";
  ASSERT_EQ(rnti_t::SI_RNTI, pdcch->ctx.rnti);
  ASSERT_EQ(pdcch->ctx.bwp_cfg, &cell_cfg.init_bwp.dl.cfg());
  ASSERT_EQ(*pdcch->ctx.coreset_cfg, *cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0);
  ASSERT_EQ(pdcch->ctx.n_id_pdcch_dmrs, cell_cfg.params.pci) << "Invalid N_{ID} (see TS38.211, 7.4.1.3.1)";
  ASSERT_EQ(pdcch->ctx.n_rnti_pdcch_data, 0) << "Invalid n_{RNTI} (see TS38.211, 7.3.2.3)";
  ASSERT_EQ(pdcch->ctx.n_id_pdcch_data, cell_cfg.params.pci) << "Invalid n_{ID} (see TS38.211, 7.3.2.3)";
  ASSERT_EQ(pdcch->ctx.cces.aggr_lvl, aggregation_level::n4);
  ASSERT_EQ(pdcch->ctx.cces.ncce, 0);
  ASSERT_EQ(pdcch->ctx.starting_symbol, 0);
}

TEST_F(common_pdcch_allocator_tester, single_pdcch_rar_allocation)
{
  rnti_t                ra_rnti = to_rnti(test_rng::uniform_int<unsigned>(1, 9));
  pdcch_dl_information* pdcch =
      pdcch_sch.alloc_dl_pdcch_common(res_grid[0], ra_rnti, to_search_space_id(1), aggregation_level::n4, std::nullopt);

  ASSERT_TRUE(res_grid[0].result_reader().dl().ul_pdcchs().empty());
  ASSERT_EQ(res_grid[0].result_reader().dl().dl_pdcchs().size(), 1);
  ASSERT_EQ(pdcch, &res_grid[0].result_reader().dl().dl_pdcchs()[0])
      << "Returned PDCCH ptr does not match allocated ptr";
  ASSERT_EQ(ra_rnti, pdcch->ctx.rnti);
  ASSERT_EQ(pdcch->ctx.bwp_cfg, &cell_cfg.init_bwp.dl.cfg());
  ASSERT_EQ(*pdcch->ctx.coreset_cfg, *cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0);
  ASSERT_EQ(pdcch->ctx.n_id_pdcch_dmrs, cell_cfg.params.pci) << "Invalid N_{ID} (see TS38.211, 7.4.1.3.1)";
  ASSERT_EQ(pdcch->ctx.n_rnti_pdcch_data, 0) << "Invalid n_{RNTI} (see TS38.211, 7.3.2.3)";
  ASSERT_EQ(pdcch->ctx.n_id_pdcch_data, cell_cfg.params.pci) << "Invalid n_{ID} (see TS38.211, 7.3.2.3)";
  ASSERT_EQ(pdcch->ctx.cces.aggr_lvl, aggregation_level::n4);
  ASSERT_EQ(pdcch->ctx.cces.ncce, 0);
  ASSERT_EQ(pdcch->ctx.starting_symbol, 0);
}

TEST_F(common_pdcch_allocator_tester, when_no_pdcch_space_for_rar_then_allocation_fails)
{
  rnti_t                ra_rnti1 = to_rnti(test_rng::uniform_int<unsigned>(1, 9));
  rnti_t                ra_rnti2 = to_rnti(test_rng::uniform_int<unsigned>(1, 9));
  pdcch_dl_information* pdcch1   = pdcch_sch.alloc_dl_pdcch_common(
      res_grid[0], ra_rnti1, to_search_space_id(1), aggregation_level::n4, std::nullopt);
  pdcch_dl_information* pdcch2 = pdcch_sch.alloc_dl_pdcch_common(
      res_grid[0], ra_rnti2, to_search_space_id(1), aggregation_level::n4, std::nullopt);

  ASSERT_EQ(1, res_grid[0].result_reader().dl().dl_pdcchs().size());
  ASSERT_EQ(pdcch1, &res_grid[0].result_reader().dl().dl_pdcchs()[0]);
  ASSERT_EQ(pdcch1->ctx.rnti, ra_rnti1);
  ASSERT_EQ(pdcch2, nullptr);

  // RAR PDCCH uses a common SearchSpace, so the failed attempt counts towards both the DL and common-DL counters.
  const failed_alloc_attempts& failed = res_grid[0].result_reader().failed_attempts();
  ASSERT_EQ(failed.dl_pdcch, 1);
  ASSERT_EQ(failed.common_dl_pdcch, 1);
  ASSERT_EQ(failed.ul_pdcch, 0);
  ASSERT_EQ(failed.common_ul_pdcch, 0);
}

class ue_pdcch_resource_allocator_scrambling_tester : public base_pdcch_resource_allocator_tester,
                                                      public ::testing::TestWithParam<test_scrambling_params>
{
  static sched_cell_configuration_request_message make_cell_req(const test_scrambling_params& params)
  {
    sched_cell_configuration_request_message msg = sched_config_helper::make_default_sched_cell_configuration_request();
    auto&                                    pdcch_cfg = *msg.ran.init_bwp.pdcch_cfg;
    pdcch_cfg.coresets[0].set_non_coreset0_pdcch_dmrs_scrambling_id(params.cs1_pdcch_dmrs_scrambling_id);
    auto& ss2 = pdcch_cfg.search_spaces[0];
    if (params.ss2_type == ocudu::search_space_type::common) {
      ss2.set_non_ss0_monitored_dci_formats(search_space_configuration::common_dci_format{.f0_0_and_f1_0 = true});
    } else {
      ss2.set_non_ss0_monitored_dci_formats(search_space_configuration::ue_specific_dci_format::f0_1_and_1_1);
    }
    return msg;
  }
  static ue_cell_config make_ue_base_req(const test_scrambling_params& params)
  {
    ue_cell_config ue_cell   = config_helpers::make_default_ue_cell_config(make_cell_req(params).ran);
    auto&          pdcch_cfg = *ue_cell.serv_cell_cfg.init_dl_bwp.pdcch_cfg;
    pdcch_cfg.coresets[0].set_non_coreset0_pdcch_dmrs_scrambling_id(params.cs1_pdcch_dmrs_scrambling_id);
    if (params.ss2_type == ocudu::search_space_type::common) {
      pdcch_cfg.search_spaces[0].set_non_ss0_monitored_dci_formats(
          search_space_configuration::common_dci_format{.f0_0_and_f1_0 = true});
    } else {
      pdcch_cfg.search_spaces[0].set_non_ss0_monitored_dci_formats(
          search_space_configuration::ue_specific_dci_format::f0_1_and_1_1);
    }
    return ue_cell;
  }

protected:
  ue_pdcch_resource_allocator_scrambling_tester() :
    base_pdcch_resource_allocator_tester(to_rnti(0x4601), make_cell_req(GetParam()), make_ue_base_req(GetParam())),
    params(GetParam())
  {
  }

  sched_ue_creation_request_message
  create_ue_cfg(rnti_t                             rnti,
                search_space_configuration::type_t ss2_type      = search_space_configuration::type_t::ue_dedicated,
                std::optional<unsigned>            cs1_n_id_dmrs = {})
  {
    return base_pdcch_resource_allocator_tester::create_ue_cfg(rnti);
  }

  test_scrambling_params params;
};

TEST_P(ue_pdcch_resource_allocator_scrambling_tester,
       if_no_candidates_are_monitored_in_the_slot_then_dl_pdcch_allocation_should_fail)
{
  const rnti_t rnti = to_rnti(0x4601 + test_rng::uniform_int<unsigned>(0, 1000));
  test_ue*     u    = this->add_ue(create_ue_cfg(rnti, params.ss2_type, params.cs1_pdcch_dmrs_scrambling_id));

  // If the PDCCH monitoring is not active or there are no candidates for this slot for this searchSpace, then the
  // allocation should fail.
  static constexpr unsigned MAX_COUNT = 100;
  for (unsigned count = 0; not is_pdcch_monitored(rnti, params.ss_id); ++count) {
    if (count >= MAX_COUNT) {
      // No valid PDCCH candidates for the given configuration.
      return;
    }
    ASSERT_EQ(pdcch_sch.alloc_dl_pdcch_ue(res_grid[0], rnti, *u->pcell_cfg, params.ss_id, aggregation_level::n4),
              nullptr);

    // The failed DL allocation counts as common only when the SearchSpace is common.
    const bool                   is_common = u->pcell_cfg->search_space(params.ss_id).cfg->is_common_search_space();
    const failed_alloc_attempts& failed    = res_grid[0].result_reader().failed_attempts();
    ASSERT_EQ(failed.dl_pdcch, 1);
    ASSERT_EQ(failed.common_dl_pdcch, is_common ? 1U : 0U);
    ASSERT_EQ(failed.ul_pdcch, 0);

    run_slot();
  }

  pdcch_dl_information* pdcch =
      pdcch_sch.alloc_dl_pdcch_ue(res_grid[0], rnti, *u->pcell_cfg, params.ss_id, aggregation_level::n4);

  ASSERT_NE(pdcch, nullptr);
  ASSERT_TRUE(res_grid[0].result_reader().dl().ul_pdcchs().empty());
  ASSERT_EQ(res_grid[0].result_reader().dl().dl_pdcchs().size(), 1);
  ASSERT_EQ(pdcch, &res_grid[0].result_reader().dl().dl_pdcchs()[0])
      << "Returned PDCCH ptr does not match allocated ptr";
  ASSERT_EQ(*pdcch->ctx.bwp_cfg, cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params);
  ASSERT_NO_FATAL_FAILURE(verify_pdcch_context(pdcch->ctx, *u, params.ss_id));
  ASSERT_EQ(pdcch->ctx.cces.aggr_lvl, aggregation_level::n4);
  ASSERT_EQ(pdcch->ctx.starting_symbol, 0);
}

TEST_P(ue_pdcch_resource_allocator_scrambling_tester,
       if_no_candidates_are_monitored_in_the_slot_then_ul_pdcch_allocation_should_fail)
{
  rnti_t   rnti = to_rnti(0x4601 + test_rng::uniform_int<unsigned>(0, 1000));
  test_ue* u    = this->add_ue(create_ue_cfg(rnti, params.ss2_type, params.cs1_pdcch_dmrs_scrambling_id));

  // If the PDCCH monitoring is not active or there are no candidates for this slot for this searchSpace, then the
  // allocation should fail.
  static constexpr unsigned MAX_COUNT = 100;
  for (unsigned count = 0; not is_pdcch_monitored(rnti, params.ss_id); ++count) {
    if (count >= MAX_COUNT) {
      // No valid PDCCH candidates for the given configuration.
      return;
    }
    ASSERT_EQ(pdcch_sch.alloc_ul_pdcch_ue(res_grid[0], rnti, *u->pcell_cfg, params.ss_id, aggregation_level::n4),
              nullptr);

    // The failed UL allocation counts as common only when the SearchSpace is common.
    const bool                   is_common = u->pcell_cfg->search_space(params.ss_id).cfg->is_common_search_space();
    const failed_alloc_attempts& failed    = res_grid[0].result_reader().failed_attempts();
    ASSERT_EQ(failed.ul_pdcch, 1);
    ASSERT_EQ(failed.common_ul_pdcch, is_common ? 1U : 0U);
    ASSERT_EQ(failed.dl_pdcch, 0);

    run_slot();
  }

  pdcch_ul_information* pdcch =
      pdcch_sch.alloc_ul_pdcch_ue(res_grid[0], rnti, *u->pcell_cfg, params.ss_id, aggregation_level::n4);

  ASSERT_TRUE(res_grid[0].result_reader().dl().dl_pdcchs().empty());
  ASSERT_EQ(res_grid[0].result_reader().dl().ul_pdcchs().size(), 1);
  ASSERT_EQ(pdcch, &res_grid[0].result_reader().dl().ul_pdcchs()[0])
      << "Returned PDCCH ptr does not match allocated ptr";
  ASSERT_EQ(*pdcch->ctx.bwp_cfg, cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params);
  ASSERT_NO_FATAL_FAILURE(verify_pdcch_context(pdcch->ctx, *u, params.ss_id));
  ASSERT_EQ(pdcch->ctx.cces.aggr_lvl, aggregation_level::n4);
  ASSERT_EQ(pdcch->ctx.starting_symbol, 0);
}

TEST(pdcch_resource_allocator_test, monitoring_period)
{
  scheduler_expert_config sched_cfg = config_helpers::make_default_scheduler_expert_config();

  for (unsigned period : {20, 40}) {
    for (unsigned duration = 1; duration < period - 1; ++duration) {
      for (unsigned offset = 0; offset < period - 1; ++offset) {
        // Note: First slot is one slot before the PDCCH monitoring window.
        unsigned randint  = test_rng::uniform_int<unsigned>(0, 10239);
        unsigned first_sl = (((randint / period) * period) + offset + 10239) % 10240;

        sched_cell_configuration_request_message msg =
            sched_config_helper::make_default_sched_cell_configuration_request();
        msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[1].set_non_ss0_monitoring_slot_periodicity(period);
        msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[1].set_non_ss0_monitoring_slot_offset(
            offset, msg.ran.dl_cfg_common.init_dl_bwp.generic_params.scs);
        msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[1].set_non_ss0_duration(duration);

        test_helpers::test_sched_config_manager cfg_mng{sched_cfg};
        const cell_configuration&               cfg = *cfg_mng.add_cell(msg);
        cell_resource_allocator                 res_grid{cfg};

        pdcch_resource_allocator_impl pdcch_sch(cfg);

        // Action: Attempt allocation of one RAR outside PDCCH monitoring window.
        slot_point sl_tx{0, first_sl};

        // Action: Attempt to allocate PDCCHs outside and inside PDCCH monitoring window.
        rnti_t            ra_rnti = to_rnti(1);
        std::vector<bool> expected_result(duration + 2, true);
        expected_result[0]     = false;
        expected_result.back() = false;
        for (unsigned i = 0; i < duration + 2; ++i) {
          res_grid.slot_indication(sl_tx);
          pdcch_sch.slot_indication(sl_tx);

          pdcch_dl_information* pdcch = pdcch_sch.alloc_dl_pdcch_common(
              res_grid[0], ra_rnti, to_search_space_id(1), aggregation_level::n4, std::nullopt);

          if (expected_result[i]) {
            // Inside PDCCH monitoring window.
            ASSERT_TRUE(pdcch != nullptr);
            ASSERT_TRUE(pdcch->ctx.rnti == ra_rnti);
          } else {
            // Outside PDCCH monitoring window.
            ASSERT_TRUE(pdcch == nullptr);
          }

          sl_tx++;
        }
      }
    }
  }
}

struct multi_alloc_test_params {
  struct alloc {
    alloc_type              type;
    rnti_t                  rnti;
    aggregation_level       aggr_lvl;
    search_space_id         ss_id; // C-RNTI only
    std::optional<unsigned> expected_ncce;
  };

  bs_channel_bandwidth                  cell_bw;
  std::optional<std::array<uint8_t, 5>> ss2_nof_candidates;
  std::vector<alloc>                    allocs;
};

template <>
struct fmt::formatter<multi_alloc_test_params> {
  template <typename ParseContext>
  auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }
  template <typename FormatContext>
  auto format(const multi_alloc_test_params& params, FormatContext& ctx) const
  {
    fmt::format_to(ctx.out(), "bw={}MHz allocs=[", bs_channel_bandwidth_to_MHz(params.cell_bw));
    for (const auto& a : params.allocs) {
      fmt::format_to(ctx.out(),
                     "{}{{{}={} al={}, ss_id={} -> cce={}}}",
                     &a == params.allocs.data() ? "" : ", ",
                     to_string(a.type),
                     a.rnti,
                     to_nof_cces(a.aggr_lvl),
                     fmt::underlying(a.ss_id),
                     a.expected_ncce);
    }
    return format_to(ctx.out(), "]");
  }
};

// Dummy function overload of template <typename T> void testing::internal::PrintTo(const T& value, ::std::ostream* os).
// This prevents valgrind from complaining about uninitialized variables.
void PrintTo(const multi_alloc_test_params& value, ::std::ostream* os)
{
  return;
}

class multi_alloc_pdcch_resource_allocator_tester : public base_pdcch_resource_allocator_tester,
                                                    public ::testing::TestWithParam<multi_alloc_test_params>
{
  using super_type = base_pdcch_resource_allocator_tester;

protected:
  multi_alloc_pdcch_resource_allocator_tester() :
    base_pdcch_resource_allocator_tester(
        to_rnti(0x4601),
        [tparams = GetParam()]() {
          sched_cell_configuration_request_message msg =
              sched_config_helper::make_default_sched_cell_configuration_request(
                  cell_config_builder_params{.dl_carrier{.carrier_bw = tparams.cell_bw}});
          if (tparams.ss2_nof_candidates.has_value()) {
            msg.ran.init_bwp.pdcch_cfg->search_spaces[0].set_non_ss0_nof_candidates(*tparams.ss2_nof_candidates);
          }
          return msg;
        }(),
        [tparams = GetParam()]() {
          ue_cell_config ue_cell =
              config_helpers::make_default_ue_cell_config(config_helpers::make_default_ran_cell_config(
                  cell_config_builder_params{.dl_carrier{.carrier_bw = tparams.cell_bw}}));
          if (tparams.ss2_nof_candidates.has_value()) {
            ue_cell.serv_cell_cfg.init_dl_bwp.pdcch_cfg->search_spaces[0].set_non_ss0_nof_candidates(
                *tparams.ss2_nof_candidates);
          }
          return ue_cell;
        }()),
    params(GetParam())
  {
  }

  sched_ue_creation_request_message create_ue_cfg(rnti_t rnti)
  {
    auto ue_creation_req = super_type::create_ue_cfg(rnti);
    (*ue_creation_req.cfg.cells)[0]
        .ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg->search_spaces[0]
        .set_non_ss0_monitored_dci_formats(search_space_configuration::ue_specific_dci_format::f0_1_and_1_1);
    cell_config_builder_params builder_params{};
    builder_params.dl_carrier.carrier_bw = params.cell_bw;
    (*ue_creation_req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg->coresets[0] =
        config_helpers::make_default_coreset_config(builder_params);
    return ue_creation_req;
  }

  const dci_context_information* allocate_pdcch(multi_alloc_test_params::alloc alloc)
  {
    test_ue* u = nullptr;
    if (alloc.type == alloc_type::dl_crnti or alloc.type == alloc_type::ul_crnti) {
      if (this->test_ues.count(alloc.rnti) == 0) {
        u = this->add_ue(create_ue_cfg(alloc.rnti));
      } else {
        u = &this->test_ues.at(alloc.rnti);
      }
    }

    switch (alloc.type) {
      case alloc_type::si_rnti: {
        // Run until PDCCH monitoring occasion for SIB1 is active (i.e. every 20ms)
        while (not pdcch_helper::is_pdcch_monitoring_active(
            next_slot, cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[0])) {
          run_slot();
        }
        // Since we schedule SIB1 on (n0 + 1)th slot we need to run once more.
        run_slot();
        pdcch_dl_information* sib_pdcch = pdcch_sch.alloc_dl_pdcch_common(
            res_grid[0], rnti_t::SI_RNTI, to_search_space_id(0), alloc.aggr_lvl, std::nullopt);
        return sib_pdcch != nullptr ? &sib_pdcch->ctx : nullptr;
      } break;
      case alloc_type::ra_rnti: {
        pdcch_dl_information* rar_pdcch = pdcch_sch.alloc_dl_pdcch_common(
            res_grid[0], alloc.rnti, to_search_space_id(1), alloc.aggr_lvl, std::nullopt);
        return rar_pdcch != nullptr ? &rar_pdcch->ctx : nullptr;

      } break;
      case alloc_type::dl_crnti: {
        pdcch_dl_information* dl_pdcch =
            pdcch_sch.alloc_dl_pdcch_ue(res_grid[0], alloc.rnti, *u->pcell_cfg, alloc.ss_id, alloc.aggr_lvl);
        return dl_pdcch != nullptr ? &dl_pdcch->ctx : nullptr;

      } break;
      case alloc_type::ul_crnti: {
        pdcch_ul_information* ul_pdcch =
            pdcch_sch.alloc_ul_pdcch_ue(res_grid[0], alloc.rnti, *u->pcell_cfg, alloc.ss_id, alloc.aggr_lvl);
        return ul_pdcch != nullptr ? &ul_pdcch->ctx : nullptr;

      } break;
      default:
        report_fatal_error("Invalid RNTI type");
    }
    return nullptr;
  }

  const dci_context_information* find_pdcch_alloc(multi_alloc_test_params::alloc alloc) const
  {
    if (alloc.type == alloc_type::ul_crnti) {
      const auto ul_pdcchs = res_grid[0].result_reader().dl().ul_pdcchs();
      const auto ul_it     = std::find_if(
          ul_pdcchs.begin(), ul_pdcchs.end(), [&alloc](const auto& pdcch) { return pdcch.ctx.rnti == alloc.rnti; });
      return ul_it != ul_pdcchs.end() ? &ul_it->ctx : nullptr;
    }
    const auto dl_pdcchs = res_grid[0].result_reader().dl().dl_pdcchs();
    const auto dl_it     = std::find_if(
        dl_pdcchs.begin(), dl_pdcchs.end(), [&alloc](const auto& pdcch) { return pdcch.ctx.rnti == alloc.rnti; });
    return dl_it != dl_pdcchs.end() ? &dl_it->ctx : nullptr;
  }

  multi_alloc_test_params params;
};

TEST_P(multi_alloc_pdcch_resource_allocator_tester, pdcch_allocation_outcome)
{
  test_logger.info("Test params: {}", params);
  print_cfg();

  for (const multi_alloc_test_params::alloc& a : params.allocs) {
    this->allocate_pdcch(a);
  }

  ASSERT_FALSE(collisions_found());
  for (const multi_alloc_test_params::alloc& a : params.allocs) {
    const dci_context_information* ctx = this->find_pdcch_alloc(a);
    ASSERT_EQ(ctx != nullptr, a.expected_ncce.has_value())
        << fmt::format("For rnti={} expected cce={}", a.rnti, a.expected_ncce);
    if (ctx != nullptr) {
      if (a.type == alloc_type::dl_crnti or a.type == alloc_type::ul_crnti) {
        ASSERT_NO_FATAL_FAILURE(verify_pdcch_context(*ctx, test_ues.at(a.rnti), a.ss_id));
      }
    }
  }
}

INSTANTIATE_TEST_SUITE_P(n_id_scrambling_derivation_test,
                         ue_pdcch_resource_allocator_scrambling_tester,
                         testing::Values(test_scrambling_params{.ss_id    = to_search_space_id(0),
                                                                .ss2_type = search_space_type::common,
                                                                .cs1_pdcch_dmrs_scrambling_id = std::nullopt},
                                         test_scrambling_params{.ss_id    = to_search_space_id(2),
                                                                .ss2_type = search_space_type::common,
                                                                .cs1_pdcch_dmrs_scrambling_id = std::nullopt},
                                         test_scrambling_params{.ss_id    = to_search_space_id(2),
                                                                .ss2_type = search_space_type::common,
                                                                .cs1_pdcch_dmrs_scrambling_id = 5},
                                         test_scrambling_params{.ss_id    = to_search_space_id(2),
                                                                .ss2_type = search_space_type::ue_dedicated,
                                                                .cs1_pdcch_dmrs_scrambling_id = std::nullopt},
                                         test_scrambling_params{.ss_id    = to_search_space_id(2),
                                                                .ss2_type = search_space_type::ue_dedicated,
                                                                .cs1_pdcch_dmrs_scrambling_id = 5}));

INSTANTIATE_TEST_SUITE_P(
    multi_alloc_pdcch_allocation_test,
    multi_alloc_pdcch_resource_allocator_tester,
    testing::Values(
        // clang-format off
  //    allocation type        RNTI            aggregation level      search space        expected NCCE (nullopt=no alloc)
  multi_alloc_test_params{cell_bw::MHz10, std::nullopt,
    {{alloc_type::dl_crnti, to_rnti(0x4601), aggregation_level::n4, to_search_space_id(2), 0}}},
  multi_alloc_test_params{cell_bw::MHz10, std::nullopt,
    {{alloc_type::dl_crnti, to_rnti(0x4601), aggregation_level::n4, to_search_space_id(2), 0},
     {alloc_type::ul_crnti, to_rnti(0x4601), aggregation_level::n4, to_search_space_id(2), 4}}},
  multi_alloc_test_params{cell_bw::MHz10, std::nullopt,
    {{alloc_type::si_rnti,  rnti_t::SI_RNTI,         aggregation_level::n4, to_search_space_id(0), 0},
     {alloc_type::ul_crnti, to_rnti(0x4601), aggregation_level::n4, to_search_space_id(1), std::nullopt}}},
  multi_alloc_test_params{cell_bw::MHz10, std::nullopt,
    {{alloc_type::si_rnti,  rnti_t::SI_RNTI,         aggregation_level::n4, to_search_space_id(0), 0},
     {alloc_type::ul_crnti, to_rnti(0x4601), aggregation_level::n4, to_search_space_id(2), 4}}},
  multi_alloc_test_params{cell_bw::MHz20, std::nullopt,
    {{alloc_type::si_rnti,  rnti_t::SI_RNTI,         aggregation_level::n4, to_search_space_id(0), 0},
     {alloc_type::ul_crnti, to_rnti(0x4601), aggregation_level::n4, to_search_space_id(2), 8}}},
  multi_alloc_test_params{cell_bw::MHz10, std::array<uint8_t, 5>{0,5,0,0,0},
    {{alloc_type::dl_crnti, to_rnti(0x4601), aggregation_level::n2, to_search_space_id(2), 0},
     {alloc_type::dl_crnti, to_rnti(0x4602), aggregation_level::n2, to_search_space_id(2), 2},
     {alloc_type::dl_crnti, to_rnti(0x4603), aggregation_level::n2, to_search_space_id(2), 4},
     {alloc_type::ul_crnti, to_rnti(0x4604), aggregation_level::n2, to_search_space_id(2), 6},
     {alloc_type::ul_crnti, to_rnti(0x4605), aggregation_level::n2, to_search_space_id(2), 8}}},
  multi_alloc_test_params{cell_bw::MHz20, std::array<uint8_t, 5>{0,0,5,0,0},
    {{alloc_type::dl_crnti, to_rnti(0x4601), aggregation_level::n4, to_search_space_id(2), 0},
     {alloc_type::dl_crnti, to_rnti(0x4602), aggregation_level::n4, to_search_space_id(2), 4},
     {alloc_type::dl_crnti, to_rnti(0x4603), aggregation_level::n4, to_search_space_id(2), 8},
     {alloc_type::dl_crnti, to_rnti(0x4604), aggregation_level::n4, to_search_space_id(2), 12},
     {alloc_type::dl_crnti, to_rnti(0x4605), aggregation_level::n4, to_search_space_id(2), 14}}}
));
// clang-format on
