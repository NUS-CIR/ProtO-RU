// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/scheduler/common_scheduling/csi_rs_scheduler.h"
#include "lib/scheduler/common_scheduling/ra_scheduler.h"
#include "lib/scheduler/common_scheduling/sib1_scheduler.h"
#include "lib/scheduler/ue_context/ue_cell_repository.h"
#include "lib/scheduler/ue_context/ue_repository.h"
#include "sub_scheduler_test_environment.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/scheduler/cell_config_builder_profiles.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "tests/test_doubles/scheduler/scheduler_result_finder.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "tests/unittests/scheduler/test_utils/dummy_test_components.h"
#include "tests/unittests/scheduler/test_utils/indication_generators.h"
#include "tests/unittests/scheduler/test_utils/scheduler_test_suite.h"
#include "ocudu/ran/prach/prach_time_mapping.h"
#include "ocudu/ran/prach/ra_helper.h"
#include "ocudu/ran/prach/ssb_to_ro_mapping.h"
#include "ocudu/scheduler/config/time_domain_resource_helper.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <limits>

using namespace ocudu;
using namespace cell_config_builder_profiles;

namespace {

class ra_scheduler_setup : public sub_scheduler_test_environment
{
  static constexpr unsigned CRNTI_RANGE = to_underlying(rnti_t::MAX_CRNTI) - to_underlying(rnti_t::MIN_CRNTI);

public:
  ra_scheduler_setup(const sched_cell_configuration_request_message& req, bool sched_csi, bool sched_sib1) :
    ra_scheduler_setup({}, req, sched_csi, sched_sib1)
  {
  }
  ra_scheduler_setup(const scheduler_expert_config&                  sched_cfg_,
                     const sched_cell_configuration_request_message& req,
                     bool                                            sched_csi,
                     bool                                            sched_sib1) :
    sub_scheduler_test_environment(sched_cfg_, req)
  {
    ue_db.register_cell(ue_cell_db);

    if (sched_csi) {
      csi_rs_sch.emplace(cell_cfg);
    }
    if (sched_sib1) {
      sib1_sch.emplace(cell_cfg, *pdcch_alloc, units::bytes{108});
    }

    rnti_count = test_rng::uniform_int<unsigned>(0, CRNTI_RANGE);
    rnti_inc   = test_rng::uniform_int<unsigned>(1, 5);

    // Run slot once so that the resource grid gets initialized with the initial slot.
    this->run_slot();
    for (unsigned i = 0; i != res_grid.max_dl_slot_alloc_delay - 1; ++i) {
      if (csi_rs_sch.has_value()) {
        csi_rs_sch->run_slot(res_grid[i]);
      }
      if (sib1_sch.has_value()) {
        sib1_sch->run_slot(res_grid[i]);
      }
    }
  }
  ~ra_scheduler_setup() override { this->flush_events(); }

  void do_run_slot() override
  {
    if (csi_rs_sch.has_value()) {
      csi_rs_sch->run_slot(res_grid[res_grid.max_dl_slot_alloc_delay]);
    }
    if (sib1_sch.has_value()) {
      sib1_sch->run_slot(res_grid[res_grid.max_dl_slot_alloc_delay]);
    }
    ra_sch.run_slot(res_grid);
    ASSERT_NO_FATAL_FAILURE(tracker.on_new_result(res_grid[0].slot, res_grid[0].result_reader()));
  }

  /// \param occasion_slot_delay Slots between the PRACH occasion and the slot the lower layers report it in.
  void handle_rach_indication(rach_indication_message ind, unsigned occasion_slot_delay = 0)
  {
    // Advance the simulator to a slot that has a valid PRACH occasion. The ra_scheduler only
    // prereserves MsgA PUSCH for slots whose corresponding PRACH slot is a valid occasion per
    // the configured PRACH config index; injecting a PRACH at an arbitrary slot would miss the
    // prereservation and fail the sanity check in handle_msga_occasion.
    const prach_helper::preamble_slot_mapping prach_mapper{
        cell_cfg.band(),
        cell_cfg.init_bwp.ul.cfg().scs,
        cell_cfg.init_bwp.ul.rach_common()->rach_cfg_generic.prach_config_index};
    run_slot_until([this, &prach_mapper]() { return prach_mapper.has_prach_occasion(next_slot_rx()); });
    // The UE transmits its preamble in an uplink occasion, which reaches the gNB k_mac slots later on its downlink
    // clock. The conversion is the identity in a terrestrial cell.
    const slot_point ul_occasion_slot = next_slot_rx();
    ind.slot_rx                       = cell_cfg.ue_ul_to_dl_slot(ul_occasion_slot);
    if (not ind.occasions.empty()) {
      // The lower layers report the t_id of the occasion, which may precede the slot they report it in.
      ind.occasions[0].slot_index =
          test_helper::compute_prach_occasion_slot_index(cell_cfg, ul_occasion_slot - occasion_slot_delay);
    }
    ra_sch.handle_rach_indication(ind);
    tracker.on_new_rach_ind(ind);
  }

  void handle_crc_indication(const ul_crc_indication& crc)
  {
    ra_sch.handle_crc_indication(crc);
    tracker.on_crc_indication(crc);
  }

  rach_indication_message::preamble create_random_preamble()
  {
    const auto next_rnti = rnti_count + to_underlying(rnti_t::MIN_CRNTI);

    rach_indication_message::preamble preamble =
        test_helper::create_preamble(test_rng::uniform_int<unsigned>(0, 63), to_rnti(next_rnti));
    preamble.time_advance =
        phy_time_unit::from_seconds(std::uniform_real_distribution<double>{0, 2005e-6}(test_rng::tls_gen()));

    rnti_count = (rnti_count + rnti_inc) % CRNTI_RANGE;
    return preamble;
  }

  rach_indication_message create_rach_indication(unsigned nof_preambles)
  {
    std::vector<rach_indication_message::preamble> preambles;
    for (unsigned i = 0; i != nof_preambles; ++i) {
      preambles.push_back(create_random_preamble());
    }
    return test_helper::create_rach_indication(cell_cfg, next_slot_rx(), preambles);
  }

  bool grants_scheduled_in_next_slots(unsigned nof_slots_to_check)
  {
    for (unsigned i = 0; i != nof_slots_to_check; ++i) {
      this->run_slot();

      const auto dl_res = res_grid[0].result_reader().dl();
      const auto ul_res = res_grid[0].result_reader().ul();

      const auto dl_pdcchs = dl_res.dl_pdcchs();
      const bool has_ra_dl_pdcch =
          std::any_of(dl_pdcchs.begin(), dl_pdcchs.end(), [](const pdcch_dl_information& pdcch) {
            return pdcch.dci.type() == dci_dl_rnti_config_type::ra_f1_0;
          });
      const auto ul_pdcchs = dl_res.ul_pdcchs();
      const bool has_msg3_retx_pdcch =
          std::any_of(ul_pdcchs.begin(), ul_pdcchs.end(), [](const pdcch_ul_information& pdcch) {
            return pdcch.dci.type() == dci_ul_rnti_config_type::tc_rnti_f0_0;
          });
      const auto puschs         = ul_res.puschs();
      const bool has_msg3_pusch = std::any_of(puschs.begin(), puschs.end(), [](const ul_sched_info& pusch) {
        return pusch.context.ue_index == INVALID_DU_UE_INDEX;
      });

      if (not dl_res.rar_grants().empty() or has_ra_dl_pdcch or has_msg3_retx_pdcch or has_msg3_pusch) {
        return true;
      }
    }
    return false;
  }

  void handle_crc_for_pending_puschs(bool crc)
  {
    const auto puschs = this->res_grid[0].result_reader().ul().puschs();
    for (const ul_sched_info& pusch : puschs) {
      if (pusch.context.ue_index == INVALID_DU_UE_INDEX) {
        // Msg3 PUSCH. "rapid" is a MsgA-only field; if set here, the PHY will wrongly apply the MsgA
        // descrambling sequence (TS38.211 6.3.1.1) to a regular Msg3 PUSCH and fail to decode it.
        ASSERT_FALSE(pusch.context.rapid.has_value()) << "Msg3 PUSCH context.rapid must not be set";
      }
    }
    if (not puschs.empty()) {
      handle_crc_indication(test_helper::create_crc_indication(this->res_grid[0].slot, puschs, crc));
    }
  }

  ra_ue_repository   ra_ue_repo{cell_cfg, mac_logger};
  ue_cell_repository ue_cell_db{cell_cfg, nullptr};
  ue_repository      ue_db{sched_cfg.ue};
  ra_scheduler ra_sch{cell_cfg, *pdcch_alloc, pucch_alloc, uci_alloc, ra_ue_repo, ue_cell_db, ev_logger, metrics_hdlr};
  std::optional<csi_rs_scheduler>   csi_rs_sch;
  std::optional<sib1_scheduler>     sib1_sch;
  test_helper::ra_scheduler_tracker tracker{cell_cfg};
  unsigned                          rnti_count = 0;
  unsigned                          rnti_inc   = 1;
};

struct test_params {
  frequency_range                        fr;
  unsigned                               min_k;
  std::optional<tdd_ul_dl_config_common> tdd_cfg;
  bool                                   sched_csi_rs = false;
  bool                                   sched_sib1   = false;
};

void PrintTo(const test_params& p, std::ostream* os)
{
  *os << "fr=" << to_string(p.fr) << " min_k=" << p.min_k << " tdd=" << (p.tdd_cfg.has_value() ? "yes" : "no")
      << " csi_rs=" << p.sched_csi_rs << " sib1=" << p.sched_sib1;
}

/// Test suite common to different FRs, duplex modes, k values.
class ra_scheduler_common_test : public ra_scheduler_setup, public ::testing::TestWithParam<test_params>
{
public:
  ra_scheduler_common_test() :
    ra_scheduler_setup(get_sched_req(GetParam()), GetParam().sched_csi_rs, GetParam().sched_sib1)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-a");
  }

  static sched_cell_configuration_request_message get_sched_req(const test_params& t_params)
  {
    cell_config_builder_params builder_params =
        create(t_params.tdd_cfg.has_value() ? duplex_mode::TDD : duplex_mode::FDD, t_params.fr);
    builder_params.min_k2               = t_params.min_k;
    builder_params.min_k1               = t_params.min_k;
    builder_params.tdd_ul_dl_cfg_common = t_params.tdd_cfg;
    return sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
  }
};

/// This test verifies that the cell resource grid remains empty when no RACH indications arrive to the RA scheduler.
TEST_P(ra_scheduler_common_test, when_no_rach_indication_received_then_no_rar_allocated)
{
  ASSERT_FALSE(grants_scheduled_in_next_slots(10));
}

/// \brief The RA-RNTI comes from the reported occasion slot index, not from the indication slot.
///
/// The lower layers own t_id, as per TS 38.321, Section 5.1.3. The RAR is only matched if the scheduler and the tracker
/// both take t_id from the occasion.
TEST_P(ra_scheduler_common_test, when_occasion_slot_index_differs_from_rx_slot_then_ra_rnti_uses_the_occasion)
{
  // A one subframe shift changes t_id in any numerology, so it never matches the one derived from slot_rx.
  handle_rach_indication(create_rach_indication(1), get_nof_slots_per_subframe(cell_cfg.scs_common()));

  for (unsigned slot_count = 0, max_slot_count = 1000; slot_count < max_slot_count and tracker.nof_msg3_acked() == 0;
       ++slot_count) {
    run_slot();
    handle_crc_for_pending_puschs(true);
  }

  ASSERT_EQ(tracker.nof_rars(), 1);
  ASSERT_EQ(tracker.nof_msg3_acked(), 1);
}

TEST_P(ra_scheduler_common_test,
       when_rach_indication_with_single_preamble_received_then_one_rar_and_one_msg3_are_allocated)
{
  // Forward single RACH occasion with multiple preambles.
  rach_indication_message one_rach = create_rach_indication(1);
  handle_rach_indication(one_rach);

  for (unsigned slot_count = 0, max_slot_count = 1000; slot_count < max_slot_count and tracker.nof_msg3_acked() == 0;
       ++slot_count) {
    run_slot();

    ASSERT_EQ(this->res_grid[0].result_reader().dl().ue_grants().size(), 0);
    ASSERT_EQ(this->res_grid[0].result_reader().dl().ul_pdcchs().size(), 0);
    handle_crc_for_pending_puschs(true);
  }

  ASSERT_FALSE(grants_scheduled_in_next_slots(10));
  ASSERT_EQ(tracker.nof_ra_dl_pdcchs(), 1);
  ASSERT_EQ(tracker.nof_rars(), 1);
  ASSERT_EQ(tracker.nof_msg3_acked(), 1);
}

/// This test verifies the correct scheduling of a RAR and Msg3 when multiple RACH Preambles are received, all in a
/// single RACH occasion.
/// The scheduler is expected to allocate one or more RARs (all with the same RA-RNTI), and containing one or multiple
/// Msg3s.
TEST_P(ra_scheduler_common_test,
       when_rach_indication_with_multiple_preambles_received_then_multiple_msg3s_are_allocated)
{
  // Forward single RACH occasion with multiple preambles.
  const unsigned          nof_preambles = test_rng::uniform_int<unsigned>(2, 8);
  rach_indication_message one_rach      = create_rach_indication(nof_preambles);
  handle_rach_indication(one_rach);

  unsigned         max_msg3_per_rar = 0;
  std::set<rnti_t> ra_rntis;
  for (unsigned slot_count = 0, max_slots = 1000; slot_count != max_slots and tracker.nof_msg3_acked() < nof_preambles;
       ++slot_count) {
    run_slot();

    const auto rars = this->res_grid[0].result_reader().dl().rar_grants();
    for (const rar_information& rar : rars) {
      ra_rntis.emplace(rar.pdsch_cfg.rnti);
      max_msg3_per_rar = std::max<unsigned>(max_msg3_per_rar, rar.grants.size());
    }

    handle_crc_for_pending_puschs(true);
  }

  ASSERT_FALSE(grants_scheduled_in_next_slots(10));
  ASSERT_EQ(ra_rntis.size(), 1) << "Only one RA-RNTI was expected";
  ASSERT_GT(max_msg3_per_rar, 1) << "The RA scheduler should try to schedule multiple Msg3s per RAR";
  ASSERT_EQ(tracker.nof_ra_dl_pdcchs(), tracker.nof_rars());
  ASSERT_LE(tracker.nof_rars(), one_rach.occasions[0].preambles.size());
  ASSERT_EQ(tracker.nof_msg3_newtxs(), one_rach.occasions[0].preambles.size());
  ASSERT_EQ(tracker.nof_msg3_acked(), tracker.nof_msg3_newtxs());
}

/// This test verifies the correct scheduling of a RAR and Msg3 when multiple RACH Preambles are received, each in a
/// different PRACH occasion.
/// The scheduler is expected to allocate several RARs (with different RA-RNTIs), each composed by one Msg3.
TEST_P(ra_scheduler_common_test, when_rach_indication_with_multiple_occasions_received_then_multiple_rars_are_allocated)
{
  auto                    nof_occasions = test_rng::uniform_int<unsigned>(1, MAX_PRACH_OCCASIONS_PER_SLOT);
  rach_indication_message rach_ind      = create_rach_indication(0);
  for (unsigned i = 0; i != nof_occasions; ++i) {
    rach_ind.occasions.emplace_back();
    rach_ind.occasions.back().start_symbol    = 0;
    rach_ind.occasions.back().frequency_index = i;
    rach_ind.occasions.back().preambles.emplace_back(create_random_preamble());
  }
  handle_rach_indication(rach_ind);

  for (unsigned slot_count = 0, max_slots = 1000; slot_count != max_slots and tracker.nof_msg3_acked() < nof_occasions;
       ++slot_count) {
    run_slot();

    handle_crc_for_pending_puschs(true);
  }

  ASSERT_FALSE(grants_scheduled_in_next_slots(10));
  ASSERT_EQ(tracker.nof_ra_dl_pdcchs(), tracker.nof_rars());
  ASSERT_LE(tracker.nof_rars(), nof_occasions);
  ASSERT_EQ(tracker.nof_msg3_newtxs(), tracker.nof_rars());
  ASSERT_EQ(tracker.nof_msg3_acked(), tracker.nof_msg3_newtxs());
}

TEST_P(ra_scheduler_common_test, when_crc_is_ko_then_msg3_retx_is_scheduled)
{
  // Forward single RACH occasion with multiple preambles.
  const unsigned          nof_preambles = test_rng::uniform_int<unsigned>(2, 8);
  rach_indication_message one_rach      = create_rach_indication(nof_preambles);
  handle_rach_indication(one_rach);

  // Run until Msg3s get NACKed.
  for (unsigned slot_count = 0, max_slots = 1000; slot_count != max_slots and tracker.nof_msg3_newtxs() < nof_preambles;
       ++slot_count) {
    run_slot();
    handle_crc_for_pending_puschs(false);
  }

  // ACK following Msg3s.
  ASSERT_EQ(tracker.nof_msg3_acked(), 0);
  for (unsigned slot_count = 0, max_slots = 1000; slot_count != max_slots and tracker.nof_msg3_acked() < nof_preambles;
       ++slot_count) {
    run_slot();
    handle_crc_for_pending_puschs(true);
  }

  ASSERT_FALSE(grants_scheduled_in_next_slots(20));
  ASSERT_EQ(tracker.nof_msg3_acked(), nof_preambles);
  ASSERT_EQ(tracker.nof_msg3_newtxs(), nof_preambles);
  ASSERT_GE(tracker.nof_msg3_retxs(), nof_preambles);
}

using tdd_fr1_30khz = tdd_pattern_profile_fr1_30khz;

static std::vector<test_params> get_test_params()
{
  return {// FR1, FDD.
          test_params{frequency_range::FR1, 2},
          test_params{frequency_range::FR1, 4},
          test_params{frequency_range::FR1, 4, std::nullopt, true},
          // FR1, TDD.
          test_params{frequency_range::FR1, 2, create_tdd_pattern(tdd_fr1_30khz::DDDDDDDSUU)},
          test_params{frequency_range::FR1, 4, create_tdd_pattern(tdd_fr1_30khz::DDDDDDDSUU)},
          test_params{frequency_range::FR1, 2, create_tdd_pattern(tdd_fr1_30khz::DDDSU)},
          test_params{frequency_range::FR1, 1, create_tdd_pattern(tdd_fr1_30khz::DSUU)},
          test_params{frequency_range::FR1, 2, create_tdd_pattern(tdd_fr1_30khz::DDDDDDDSUU), true},
          test_params{frequency_range::FR1, 2, create_tdd_pattern(tdd_fr1_30khz::DDDDDDDSUU), true, true},
          test_params{frequency_range::FR1, 2, create_tdd_pattern(tdd_pattern_profile_fr1_30khz::DDDSU), true, true},
          // FR2, TDD.
          test_params{frequency_range::FR2, 1, create_tdd_pattern(tdd_pattern_profile_fr2_120khz::DDDSU)},
          test_params{frequency_range::FR2, 1, create_tdd_pattern(tdd_pattern_profile_fr2_120khz::DDDSU), true}};
}

INSTANTIATE_TEST_SUITE_P(ra_scheduler, ra_scheduler_common_test, ::testing::ValuesIn(get_test_params()));

/// RA procedure in an NTN cell, where every DL-signalled UL transmission is delayed by the cell-specific Koffset.
class ra_scheduler_ntn_test : public ra_scheduler_setup, public ::testing::Test
{
public:
  ra_scheduler_ntn_test() : ra_scheduler_setup(get_sched_req(), false, false) {}

  static sched_cell_configuration_request_message get_sched_req()
  {
    sched_cell_configuration_request_message req = sched_config_helper::make_default_sched_cell_configuration_request(
        create(duplex_mode::FDD, frequency_range::FR1));
    req.ran.ntn_params.emplace();
    req.ran.ntn_params->ntn_cfg.cell_specific_koffset = std::chrono::milliseconds{20};
    return req;
  }
};

TEST_F(ra_scheduler_ntn_test, msg3_is_scheduled_the_cell_specific_koffset_after_its_msg3_delay)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-TIM-4");

  ASSERT_GT(cell_cfg.ntn_cs_koffset, 0U);
  handle_rach_indication(create_rach_indication(1));

  const rar_information* rar = nullptr;
  for (unsigned slot_count = 0; slot_count != 100 and rar == nullptr; ++slot_count) {
    run_slot();
    if (not res_grid[0].result_reader().dl().rar_grants().empty()) {
      rar = &res_grid[0].result_reader().dl().rar_grants().front();
    }
  }
  ASSERT_NE(rar, nullptr) << "No RAR was scheduled";
  ASSERT_EQ(rar->grants.size(), 1U);
  const rar_ul_grant& grant = rar->grants.front();

  // TS 38.213, Section 8.3: the Msg3 PUSCH is transmitted K2 + delta slots after the RAR, extended by K_offset in NTN.
  const auto&    ul_bwp     = cell_cfg.params.ul_cfg_common.init_ul_bwp;
  const unsigned k2         = ul_bwp.pusch_cfg_common->pusch_td_alloc_list[grant.time_resource_assignment].k2;
  const unsigned msg3_delay = ra_helper::get_msg3_delay(ul_bwp.generic_params.scs, k2);

  const auto has_msg3 = [&grant](const cell_slot_resource_allocator& alloc) {
    const auto puschs = alloc.result_reader().ul().puschs();
    return std::any_of(puschs.begin(), puschs.end(), [&grant](const ul_sched_info& p) {
      return p.pusch_cfg.rnti == grant.temp_crnti;
    });
  };
  ASSERT_TRUE(has_msg3(res_grid[msg3_delay + cell_cfg.ntn_cs_koffset]))
      << "Msg3 must be scheduled K_offset slots after the Msg3 delay";
  ASSERT_FALSE(has_msg3(res_grid[msg3_delay])) << "Msg3 must not be scheduled at the terrestrial Msg3 delay";
}

TEST_F(ra_scheduler_ntn_test, ra_procedure_completes_with_msg3_retransmissions)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-TIM-4");

  const unsigned nof_preambles = 2;
  handle_rach_indication(create_rach_indication(nof_preambles));

  // NACK the first Msg3 transmissions, so that their retransmissions are scheduled over the NTN delay too.
  for (unsigned slot_count = 0; slot_count != 1000 and tracker.nof_msg3_newtxs() < nof_preambles; ++slot_count) {
    run_slot();
    handle_crc_for_pending_puschs(false);
  }
  for (unsigned slot_count = 0; slot_count != 1000 and tracker.nof_msg3_acked() < nof_preambles; ++slot_count) {
    run_slot();
    handle_crc_for_pending_puschs(true);
  }

  ASSERT_EQ(tracker.nof_msg3_newtxs(), nof_preambles);
  ASSERT_GE(tracker.nof_msg3_retxs(), nof_preambles);
  ASSERT_EQ(tracker.nof_msg3_acked(), nof_preambles);
}

class ra_scheduler_failed_rar_test : public ra_scheduler_setup, public ::testing::TestWithParam<test_params>
{
public:
  ra_scheduler_failed_rar_test() :
    ra_scheduler_setup(get_sched_req(GetParam()), GetParam().sched_csi_rs, GetParam().sched_sib1)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-a");
  }

  static sched_cell_configuration_request_message get_sched_req(const test_params& t_params)
  {
    cell_config_builder_params builder_params =
        create(t_params.tdd_cfg.has_value() ? duplex_mode::TDD : duplex_mode::FDD, t_params.fr);
    builder_params.min_k2               = t_params.min_k;
    builder_params.min_k1               = t_params.min_k;
    builder_params.tdd_ul_dl_cfg_common = t_params.tdd_cfg;
    return sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
  }

  void fill_dl_rbs(unsigned lookahead_slots)
  {
    if (cell_cfg.is_dl_enabled(res_grid[lookahead_slots].slot)) {
      const grant_info grant{
          cell_cfg.scs_common(),
          ofdm_symbol_range{cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0->duration(),
                            NOF_OFDM_SYM_PER_SLOT_NORMAL_CP},
          crb_interval{0, cell_cfg.nof_dl_prbs}};
      res_grid[lookahead_slots].dl_res_grid.fill(grant);
    }
  }
};

TEST_P(ra_scheduler_failed_rar_test, when_no_dl_rbs_available_then_rar_is_not_scheduled)
{
  // Enqueue RACH indication.
  handle_rach_indication(create_rach_indication(test_rng::uniform_int<unsigned>(1, MAX_PREAMBLES_PER_PRACH_OCCASION)));

  // Mark all DL RBs of the first lookahead_dl_rbs slots as busy.
  const unsigned lookahead_dl_rbs = 16;
  for (unsigned i = 0; i != lookahead_dl_rbs; ++i) {
    fill_dl_rbs(i);
  }

  // Keep marking DL RBs as busy until RAR expiry.
  for (unsigned i = 0, nof_slots = 100; i != nof_slots and tracker.has_pending_ra(); ++i) {
    fill_dl_rbs(lookahead_dl_rbs);
    run_slot();
  }

  ASSERT_FALSE(grants_scheduled_in_next_slots(lookahead_dl_rbs * 2))
      << "After RAR window expiry, nothing should be scheduled";
  ASSERT_EQ(tracker.nof_ra_dl_pdcchs(), 0);
  ASSERT_EQ(tracker.nof_rars(), 0);
}

TEST_P(ra_scheduler_failed_rar_test, when_no_ul_rbs_available_then_rar_is_not_scheduled)
{
  // Enqueue RACH indication.
  handle_rach_indication(create_rach_indication(test_rng::uniform_int<unsigned>(1, MAX_PREAMBLES_PER_PRACH_OCCASION)));

  // Mark all UL RBs of the first lookahead_ul_rbs slots as busy.
  const unsigned   lookahead_ul_rbs = res_grid.max_ul_slot_alloc_delay;
  const grant_info marked_res{cell_cfg.scs_common(),
                              ofdm_symbol_range{0, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP},
                              crb_interval{0, cell_cfg.nof_ul_prbs}};
  for (unsigned i = 0; i != lookahead_ul_rbs; ++i) {
    if (cell_cfg.is_ul_enabled(res_grid[i].slot)) {
      res_grid[i].ul_res_grid.fill(marked_res);
    }
  }

  // Keep marking UL RBs as busy until RAR expiry.
  for (unsigned i = 0, nof_slots = 100; i != nof_slots and tracker.has_pending_ra(); ++i) {
    if (cell_cfg.is_ul_enabled(res_grid[lookahead_ul_rbs].slot)) {
      res_grid[lookahead_ul_rbs].ul_res_grid.fill(marked_res);
    }
    run_slot();
  }

  ASSERT_FALSE(grants_scheduled_in_next_slots(lookahead_ul_rbs * 2))
      << "After RAR window expiry, nothing should be scheduled";
  ASSERT_EQ(tracker.nof_ra_dl_pdcchs(), 0);
  ASSERT_EQ(tracker.nof_rars(), 0);
}

TEST_P(ra_scheduler_failed_rar_test, when_rbs_available_then_new_rars_can_be_scheduled)
{
  // Enqueue first RACH (which is going to fail to be allocated).
  auto first_rach = create_rach_indication(test_rng::uniform_int<unsigned>(1, MAX_PREAMBLES_PER_PRACH_OCCASION));
  handle_rach_indication(first_rach);

  // Let RAR window expire.
  const unsigned lookahead_dl_rbs = 16;
  for (unsigned i = 0; i != lookahead_dl_rbs; ++i) {
    fill_dl_rbs(i);
  }
  for (unsigned i = 0, nof_slots = 100; i != nof_slots and tracker.has_pending_ra(); ++i) {
    fill_dl_rbs(lookahead_dl_rbs);
    run_slot();
  }
  ASSERT_FALSE(grants_scheduled_in_next_slots(lookahead_dl_rbs));
  ASSERT_EQ(tracker.nof_rars(), 0);

  // New RACH is scheduled, but this time it should be possible to schedule new RARs.
  auto second_rach = create_rach_indication(test_rng::uniform_int<unsigned>(1, 8));
  handle_rach_indication(second_rach);
  for (unsigned i = 0, nof_slots = 100; i != nof_slots and tracker.has_pending_ra(); ++i) {
    run_slot();
    handle_crc_for_pending_puschs(true);
  }
  ASSERT_GT(tracker.nof_rars(), 0);
  ASSERT_EQ(tracker.nof_rars(), tracker.nof_ra_dl_pdcchs());
  ASSERT_EQ(tracker.nof_msg3_newtxs(), second_rach.occasions[0].preambles.size());
  ASSERT_EQ(tracker.nof_msg3_newtxs(), tracker.nof_msg3_acked());
}

INSTANTIATE_TEST_SUITE_P(ra_scheduler,
                         ra_scheduler_failed_rar_test,
                         ::testing::Values(
                             // FR1, FDD.
                             test_params{frequency_range::FR1, 2},
                             // FR1, TDD.
                             test_params{frequency_range::FR1, 2, create_tdd_pattern(tdd_fr1_30khz::DDDDDDDSUU)},
                             test_params{frequency_range::FR1, 2, create_tdd_pattern(tdd_fr1_30khz::DDDSU)}));

/// Test class for congestion-control-driven Backoff Indicator (BI) scheduling. Preamble SNR/count thresholds are set
/// via the scheduler_ra_expert_config passed to the constructor.
class ra_scheduler_backoff_indicator_test : public ra_scheduler_setup, public ::testing::Test
{
public:
  ra_scheduler_backoff_indicator_test(std::optional<float> snr_threshold_dB,
                                      unsigned             max_preambles = MAX_PREAMBLES_PER_PRACH_OCCASION,
                                      unsigned             duration_ms   = 40) :
    ra_scheduler_setup(make_sched_cfg(snr_threshold_dB, max_preambles, duration_ms), get_sched_req(), false, false)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-a");
  }

  static scheduler_expert_config
  make_sched_cfg(std::optional<float> snr_threshold_dB, unsigned max_preambles, unsigned duration_ms)
  {
    scheduler_expert_config cfg               = config_helpers::make_default_scheduler_expert_config();
    cfg.ra.backoff_indicator_snr_threshold_dB = snr_threshold_dB;
    cfg.ra.backoff_indicator_max_preambles    = max_preambles;
    cfg.ra.backoff_indicator_duration         = std::chrono::milliseconds{duration_ms};
    return cfg;
  }

  static sched_cell_configuration_request_message get_sched_req()
  {
    cell_config_builder_params builder_params = create(duplex_mode::FDD, frequency_range::FR1);
    builder_params.min_k1                     = 2;
    builder_params.min_k2                     = 2;
    return sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
  }

  /// Creates a random preamble with an explicit SNR value.
  rach_indication_message::preamble create_preamble_with_snr(float snr_dB)
  {
    rach_indication_message::preamble p = create_random_preamble();
    p.snr_dB                            = snr_dB;
    return p;
  }
};

class ra_scheduler_snr_backoff_test : public ra_scheduler_backoff_indicator_test
{
public:
  ra_scheduler_snr_backoff_test() : ra_scheduler_backoff_indicator_test(/* snr_threshold_dB */ -5.0F) {}
};

class ra_scheduler_count_backoff_test : public ra_scheduler_backoff_indicator_test
{
public:
  ra_scheduler_count_backoff_test() :
    ra_scheduler_backoff_indicator_test(/* snr_threshold_dB */ std::nullopt, /* max_preambles */ 2U)
  {
  }
};

class ra_scheduler_backoff_only_test : public ra_scheduler_backoff_indicator_test
{
public:
  ra_scheduler_backoff_only_test() : ra_scheduler_backoff_indicator_test(/* snr_threshold_dB */ -5.0F) {}
};

class ra_scheduler_backoff_duration_test : public ra_scheduler_backoff_indicator_test
{
public:
  ra_scheduler_backoff_duration_test() :
    ra_scheduler_backoff_indicator_test(
        /* snr_threshold_dB */ -5.0F,
        /* max_preambles */ MAX_PREAMBLES_PER_PRACH_OCCASION,
        /* duration_ms */ 80U)
  {
  }
};

/// This test verifies that a preamble with SNR below the configured threshold is excluded from the RAR/Msg3 grant,
/// while a preamble with SNR above the threshold is still granted, and that the RAR carries a Backoff Indicator.
TEST_F(ra_scheduler_snr_backoff_test, weak_preamble_excluded_and_backoff_indicator_set)
{
  rach_indication_message::preamble strong = create_preamble_with_snr(0.0F);
  rach_indication_message::preamble weak   = create_preamble_with_snr(-10.0F);

  handle_rach_indication(test_helper::create_rach_indication(cell_cfg, next_slot_rx(), {strong, weak}));

  const bool found = run_slot_until([this]() { return not res_grid[0].result_reader().dl().rar_grants().empty(); });
  ASSERT_TRUE(found);

  const rar_information& rar = res_grid[0].result_reader().dl().rar_grants().front();
  ASSERT_TRUE(rar.backoff_indicator.has_value());
  ASSERT_EQ(rar.grants.size(), 1U);
  EXPECT_EQ(rar.grants.front().temp_crnti, strong.tc_rnti);
}

/// This test verifies that, when more preambles are detected in an occasion than the configured maximum, only the
/// strongest-SNR preambles are granted a Msg3, and that the RAR carries a Backoff Indicator.
TEST_F(ra_scheduler_count_backoff_test, excess_preambles_excluded_and_backoff_indicator_set)
{
  const float                                    snrs[] = {-2.0F, 10.0F, 3.0F, -8.0F};
  std::vector<rach_indication_message::preamble> preambles;
  for (float snr : snrs) {
    preambles.push_back(create_preamble_with_snr(snr));
  }

  handle_rach_indication(test_helper::create_rach_indication(cell_cfg, next_slot_rx(), preambles));

  bool bi_seen = false;
  for (unsigned slot_count = 0, max_slots = 1000; slot_count != max_slots and tracker.has_pending_ra(); ++slot_count) {
    run_slot();
    for (const rar_information& rar : res_grid[0].result_reader().dl().rar_grants()) {
      bi_seen |= rar.backoff_indicator.has_value();
    }
    handle_crc_for_pending_puschs(true);
  }

  EXPECT_TRUE(bi_seen);
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 2U) << "Only the 2 strongest preambles should have been granted a Msg3";
}

/// This test verifies that, when every detected preamble in an occasion is excluded by congestion control, the RA
/// scheduler still transmits a RAR containing only a Backoff Indicator subheader (no RAPID subPDUs), and no Msg3 is
/// granted.
TEST_F(ra_scheduler_backoff_only_test, all_preambles_below_threshold_yields_backoff_only_rar)
{
  rach_indication_message::preamble weak1 = create_preamble_with_snr(-10.0F);
  rach_indication_message::preamble weak2 = create_preamble_with_snr(-20.0F);

  handle_rach_indication(test_helper::create_rach_indication(cell_cfg, next_slot_rx(), {weak1, weak2}));

  const rar_information* backoff_rar = nullptr;
  const bool             found       = run_slot_until([this, &backoff_rar]() {
    for (const rar_information& rar : res_grid[0].result_reader().dl().rar_grants()) {
      if (rar.backoff_indicator.has_value()) {
        backoff_rar = &rar;
        return true;
      }
    }
    return false;
  });

  ASSERT_TRUE(found);
  ASSERT_NE(backoff_rar, nullptr);
  EXPECT_TRUE(backoff_rar->grants.empty());
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 0U) << "No preamble should have been granted a Msg3";
}

/// This test verifies that the configured Backoff Indicator duration, in ms, is mapped to its index in TS38.321,
/// Table 7.2-1. 80ms is table index 6.
TEST_F(ra_scheduler_backoff_duration_test, duration_is_mapped_to_table_index)
{
  rach_indication_message::preamble weak = create_preamble_with_snr(-10.0F);

  handle_rach_indication(test_helper::create_rach_indication(cell_cfg, next_slot_rx(), {weak}));

  const rar_information* backoff_rar = nullptr;
  const bool             found       = run_slot_until([this, &backoff_rar]() {
    for (const rar_information& rar : res_grid[0].result_reader().dl().rar_grants()) {
      if (rar.backoff_indicator.has_value()) {
        backoff_rar = &rar;
        return true;
      }
    }
    return false;
  });

  ASSERT_TRUE(found);
  ASSERT_NE(backoff_rar, nullptr);
  EXPECT_EQ(*backoff_rar->backoff_indicator, 6U);
}

/// Beam that the RA scheduler is expected to map the SS/PBCH block of the test cell onto.
constexpr beam_identifier test_ssb_beam = static_cast<beam_identifier>(3);

/// Beam that a transmission is mapped onto, or \c beam_identifier::invalid if it is not beamformed.
static beam_identifier beam_of(const precoding_and_beamforming_info& info)
{
  const beam_identifier* beam = std::get_if<beam_identifier>(&info);
  return beam != nullptr ? *beam : beam_identifier::invalid;
}

/// \brief Test suite for the beam that the RA procedure maps its downlink transmissions onto.
///
/// The cell transmits a single SS/PBCH block on \c test_ssb_beam, so that every PRACH occasion is associated with it
/// and every RA transmission is expected to be carried by that beam.
class ra_scheduler_beam_test : public ra_scheduler_setup, public ::testing::Test
{
public:
  ra_scheduler_beam_test() : ra_scheduler_setup(make_beam_req(), /*sched_csi=*/false, /*sched_sib1=*/false)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-12");
  }

  static sched_cell_configuration_request_message make_beam_req()
  {
    cell_config_builder_params builder_params = create(duplex_mode::FDD, frequency_range::FR1);
    builder_params.min_k1                     = 2;
    builder_params.min_k2                     = 2;
    auto  req     = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    auto& ssb_cfg = req.ran.ssb_cfg;
    // A beam other than the first one, so that the assertions discriminate against a hardcoded default.
    ssb_cfg.ssb_beams.reset();
    ssb_cfg.ssb_beams.set_beam(0, test_ssb_beam);
    return req;
  }
};

/// Verifies that the RAR PDSCH and its PDCCH are carried by the beam of the SS/PBCH block that the UE reached the
/// cell on.
TEST_F(ra_scheduler_beam_test, rar_pdsch_and_pdcch_use_the_beam_of_the_ssb)
{
  handle_rach_indication(create_rach_indication(1));

  const rar_information* rar   = nullptr;
  bool                   found = run_slot_until([&]() {
    if (res_grid[0].result_reader().dl().rar_grants().empty()) {
      return false;
    }
    rar = &res_grid[0].result_reader().dl().rar_grants().front();
    return true;
  });

  ASSERT_TRUE(found) << "No RAR was scheduled";
  ASSERT_EQ(beam_of(rar->pdsch_cfg.precoding_and_beamforming), test_ssb_beam);
  ASSERT_EQ(res_grid[0].result_reader().dl().dl_pdcchs().size(), 1);
  ASSERT_EQ(beam_of(res_grid[0].result_reader().dl().dl_pdcchs().front().ctx.precoding_and_beamforming), test_ssb_beam);
}

/// Verifies that the PDCCH scheduling a Msg3 retransmission is carried by the beam of the SS/PBCH block, so that the
/// UE can receive its retransmission grant.
TEST_F(ra_scheduler_beam_test, msg3_retx_pdcch_uses_the_beam_of_the_ssb)
{
  handle_rach_indication(create_rach_indication(1));

  // Drive the Msg3 to a NACK, which triggers a retransmission scheduled over an uplink DCI.
  for (unsigned i = 0, max_slots = 1000; i != max_slots and tracker.nof_msg3_retxs() == 0; ++i) {
    run_slot();
    handle_crc_for_pending_puschs(false);
    if (not res_grid[0].result_reader().dl().ul_pdcchs().empty()) {
      ASSERT_EQ(beam_of(res_grid[0].result_reader().dl().ul_pdcchs().front().ctx.precoding_and_beamforming),
                test_ssb_beam);
    }
  }

  ASSERT_GE(tracker.nof_msg3_retxs(), 1) << "No Msg3 retransmission was scheduled";
}

/// \brief Test suite for the beam selection across several SS/PBCH blocks.
///
/// The cell transmits two SS/PBCH blocks, each on its own beam. The association of TS 38.213, Section 8.1 alternates
/// them over consecutive PRACH occasions, so RARs answering different occasions are expected to use different beams.
class ra_scheduler_multi_beam_test : public ra_scheduler_setup, public ::testing::Test
{
public:
  static constexpr beam_identifier first_beam  = static_cast<beam_identifier>(1);
  static constexpr beam_identifier second_beam = static_cast<beam_identifier>(2);

  ra_scheduler_multi_beam_test() : ra_scheduler_setup(make_multi_beam_req(), /*sched_csi=*/false, /*sched_sib1=*/false)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-12");
  }

  static sched_cell_configuration_request_message make_multi_beam_req()
  {
    cell_config_builder_params builder_params = create(duplex_mode::FDD, frequency_range::FR1);
    builder_params.min_k1                     = 2;
    builder_params.min_k2                     = 2;
    auto  req     = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    auto& ssb_cfg = req.ran.ssb_cfg;
    ssb_cfg.ssb_beams.reset();
    ssb_cfg.ssb_beams.set_beam(0, first_beam);
    ssb_cfg.ssb_beams.set_beam(1, second_beam);
    return req;
  }
};

/// Verifies that RARs answering different PRACH occasions are carried by the beams of the different SS/PBCH blocks
/// those occasions are associated with.
///
/// The assertion is on the set of beams observed rather than on the beam of a given occasion: the association order
/// is the subject of \c ssb_to_ro_mapping_test, and recomputing it here would pass even against a constant beam.
TEST_F(ra_scheduler_multi_beam_test, rars_of_different_occasions_use_different_beams)
{
  // Each attempt drains less than one system frame, so that the next indication lands on the PRACH occasion right
  // after the previous one. Draining a whole number of frames would keep hitting the same point of the association
  // period and observe a single SS/PBCH block.
  static constexpr unsigned nof_slots_drained = 8;

  std::set<unsigned> observed_beams;
  for (unsigned attempt = 0; attempt != 6 and observed_beams.size() < 2; ++attempt) {
    handle_rach_indication(create_rach_indication(1));

    for (unsigned i = 0; i != nof_slots_drained and observed_beams.size() < 2; ++i) {
      run_slot();
      for (const rar_information& rar : res_grid[0].result_reader().dl().rar_grants()) {
        const beam_identifier beam = beam_of(rar.pdsch_cfg.precoding_and_beamforming);
        ASSERT_NE(beam, beam_identifier::invalid);
        observed_beams.emplace(to_underlying(beam));
      }
    }
  }

  ASSERT_EQ(observed_beams, (std::set<unsigned>{to_underlying(first_beam), to_underlying(second_beam)}))
      << "The RARs must be carried by the beams of the SS/PBCH blocks their occasions map onto";
}

/// \brief Test suite for the beam of a RAR that carries only a Backoff Indicator.
///
/// Reuses the two SS/PBCH block cell, and configures an SNR threshold that drops every detected preamble, so the RAR
/// carries no RAPID subheader and its beam cannot be inferred from a granted UE.
class ra_scheduler_backoff_only_beam_test : public ra_scheduler_setup, public ::testing::Test
{
public:
  static constexpr beam_identifier first_beam  = ra_scheduler_multi_beam_test::first_beam;
  static constexpr beam_identifier second_beam = ra_scheduler_multi_beam_test::second_beam;

  ra_scheduler_backoff_only_beam_test() :
    ra_scheduler_setup(make_sched_cfg(),
                       ra_scheduler_multi_beam_test::make_multi_beam_req(),
                       /*sched_csi=*/false,
                       /*sched_sib1=*/false)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-12");
  }

  static scheduler_expert_config make_sched_cfg()
  {
    scheduler_expert_config cfg               = config_helpers::make_default_scheduler_expert_config();
    cfg.ra.backoff_indicator_snr_threshold_dB = -5.0F;
    return cfg;
  }

  /// Creates a PRACH occasion whose single preamble is below the SNR threshold.
  rach_indication_message create_weak_rach_indication()
  {
    rach_indication_message::preamble weak = create_random_preamble();
    weak.snr_dB                            = -10.0F;
    return test_helper::create_rach_indication(cell_cfg, next_slot_rx(), {weak});
  }
};

/// Verifies that a RAR carrying only a Backoff Indicator is beamformed towards the SS/PBCH block of its PRACH
/// occasion, so that the UEs told to back off can receive it.
///
/// The beam cannot be taken from a granted UE here, so a RAR that ignored the occasion association would fall back to
/// the first SS/PBCH block and only ever be seen on \c first_beam.
TEST_F(ra_scheduler_backoff_only_beam_test, backoff_only_rars_use_the_beams_of_their_occasions)
{
  // Each attempt drains less than one system frame, so that the next indication lands on the PRACH occasion right
  // after the previous one. Draining a whole number of frames would keep hitting the same point of the association
  // period and observe a single SS/PBCH block.
  static constexpr unsigned nof_slots_drained = 8;

  std::set<unsigned> observed_beams;
  for (unsigned attempt = 0; attempt != 6 and observed_beams.size() < 2; ++attempt) {
    handle_rach_indication(create_weak_rach_indication());

    for (unsigned i = 0; i != nof_slots_drained and observed_beams.size() < 2; ++i) {
      run_slot();
      for (const rar_information& rar : res_grid[0].result_reader().dl().rar_grants()) {
        ASSERT_TRUE(rar.backoff_indicator.has_value());
        ASSERT_TRUE(rar.grants.empty());
        const beam_identifier beam = beam_of(rar.pdsch_cfg.precoding_and_beamforming);
        ASSERT_NE(beam, beam_identifier::invalid);
        observed_beams.emplace(to_underlying(beam));
      }
    }
  }

  ASSERT_EQ(observed_beams, (std::set<unsigned>{to_underlying(first_beam), to_underlying(second_beam)}))
      << "The backoff-only RARs must be carried by the beams of the SS/PBCH blocks their occasions map onto";
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 0U) << "No preamble should have been granted a Msg3";
}

struct two_step_test_params {
  /// MsgA PUSCH TD offset.
  uint8_t                                td_offset;
  std::optional<tdd_ul_dl_config_common> tdd_cfg;
  /// k_mac of the cell, which turns it into an NTN one when set.
  std::chrono::milliseconds ntn_k_mac{0};
  /// Cell-specific Koffset of the cell, which turns it into an NTN one when set.
  std::chrono::milliseconds ntn_koffset{0};
};

void PrintTo(const two_step_test_params& p, std::ostream* os)
{
  *os << "td_offset=" << +p.td_offset << " tdd=" << (p.tdd_cfg.has_value() ? "yes" : "no")
      << " k_mac=" << p.ntn_k_mac.count() << " koffset=" << p.ntn_koffset.count();
}

/// Test class for 2-step RACH (MsgA/MsgB) procedures.
///
/// The test cell is configured with a mix of 4-step (preamble IDs [0, 60)) and 2-step CB (preamble IDs [60, 64))
/// preambles on shared RACH occasions.  \c td_offset is parameterized and must be >= \c delay_tx_rx_slots (default=2)
/// so that the MsgA PUSCH slot (prach_slot_rx + td_offset) falls within the allocatable resource grid.
class ra_scheduler_two_step_rach_test : public ra_scheduler_setup, public ::testing::TestWithParam<two_step_test_params>
{
  static constexpr unsigned MSGA_PREAMBLE_OFFSET = 60;

public:
  ra_scheduler_two_step_rach_test() :
    ra_scheduler_setup(make_two_step_rach_req(GetParam()), /*sched_csi=*/false, /*sched_sib1=*/false)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-2", "DU-GEN-2-a");
  }

  static sched_cell_configuration_request_message make_two_step_rach_req(const two_step_test_params& params)
  {
    cell_config_builder_params builder_params =
        create(params.tdd_cfg.has_value() ? duplex_mode::TDD : duplex_mode::FDD, frequency_range::FR1);
    builder_params.min_k1               = 2;
    builder_params.min_k2               = 2;
    builder_params.tdd_ul_dl_cfg_common = params.tdd_cfg;
    auto req = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    // A beam other than the first one, so that the assertions discriminate against a hardcoded default.
    req.ran.ssb_cfg.ssb_beams.reset();
    req.ran.ssb_cfg.ssb_beams.set_beam(0, test_ssb_beam);
    auto& rach = *req.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common;
    // Reserve preamble IDs [60, 64) for 2-step CB RACH.
    rach.nof_cb_preambles_per_ssb = MSGA_PREAMBLE_OFFSET;
    rach.two_step_rach_cfg.emplace();
    rach.two_step_rach_cfg->pusch.td_offset = params.td_offset;
    if (params.ntn_k_mac.count() != 0 or params.ntn_koffset.count() != 0) {
      req.ran.ntn_params.emplace();
      if (params.ntn_k_mac.count() != 0) {
        req.ran.ntn_params->ntn_cfg.k_mac = params.ntn_k_mac;
      }
      if (params.ntn_koffset.count() != 0) {
        req.ran.ntn_params->ntn_cfg.cell_specific_koffset = params.ntn_koffset;
      }
    }
    return req;
  }

  static rach_indication_message::preamble make_msga_preamble(unsigned preamble_idx, rnti_t tc_rnti)
  {
    return test_helper::create_preamble(MSGA_PREAMBLE_OFFSET + preamble_idx, tc_rnti);
  }

  rach_indication_message
  create_msga_rach_indication(std::initializer_list<rach_indication_message::preamble> preambles) const
  {
    return test_helper::create_rach_indication(cell_cfg, next_slot_rx(), {preambles.begin(), preambles.end()});
  }

  /// \brief Builds and forwards a MsgA RACH indication, and records the slots it lands on.
  ///
  /// \c handle_rach_indication() (from \c ra_scheduler_setup) advances the simulator to the next slot with a valid
  /// PRACH occasion, so the slot passed to \c create_msga_rach_indication is not necessarily the one actually used.
  /// Both slots are captured right after, while \c next_slot_rx() still reflects that same (unadvanced) occasion.
  void send_msga_rach(std::initializer_list<rach_indication_message::preamble> preambles)
  {
    handle_rach_indication(create_msga_rach_indication(preambles));
    last_msga_occasion_slot = next_slot_rx();
    last_prach_slot_rx      = cell_cfg.ue_ul_to_dl_slot(last_msga_occasion_slot);
  }

  /// \brief Inject a CRC result for a MsgA PUSCH by preamble index (as passed to \c make_msga_preamble).
  ///
  /// The MsgA PUSCH grant is scheduled and decoded using the (shared, per-occasion) RA-RNTI, not the preamble's
  /// TC-RNTI (TS 38.211, 6.3.1.1). The RA-RNTI is recomputed from the original PRACH occasion (rather than looked
  /// up in the current resource grid) so this also works after the grid has slid past the MsgA PUSCH slot. Its t_id
  /// names the uplink occasion of the UE, as per TS 38.213, Section 8.1.
  void send_msga_crc(unsigned preamble_idx, bool success)
  {
    const rnti_t ra_rnti = test_helper::compute_ra_rnti(cell_cfg,
                                                        last_msga_occasion_slot,
                                                        /*start_symbol=*/0,
                                                        /*frequency_index=*/0);

    ul_crc_indication crc_ind;
    crc_ind.cell_index = cell_cfg.cell_index;
    crc_ind.sl_rx      = res_grid[0].slot;
    auto& pdu          = crc_ind.crcs.emplace_back();
    pdu.rnti           = ra_rnti;
    pdu.ue_index       = INVALID_DU_UE_INDEX;
    pdu.harq_id        = to_harq_id(0);
    pdu.rapid          = static_cast<uint8_t>(MSGA_PREAMBLE_OFFSET + preamble_idx);
    pdu.tb_crc_success = success;
    handle_crc_indication(crc_ind);
  }

  /// Slot the gNB captured the last MsgA preamble in, on its downlink clock.
  slot_point last_prach_slot_rx;
  /// Uplink occasion slot the UE transmitted the last MsgA preamble in.
  slot_point last_msga_occasion_slot;

  /// Returns true if the current slot result contains at least one MsgB grant of either type.
  bool has_msgb_grant() const
  {
    const auto rars = res_grid[0].result_reader().dl().rar_grants();
    return std::any_of(rars.begin(), rars.end(), [](const rar_information& rar) {
      return std::any_of(rar.grants.begin(), rar.grants.end(), [](const rar_ul_grant& g) {
        return std::holds_alternative<rar_ul_grant::two_step_success_info>(g.type) or
               std::holds_alternative<rar_ul_grant::two_step_fallback_info>(g.type);
      });
    });
  }

  /// Returns true if there is a Msg3 PUSCH grant (created by FallbackRAR) for the given TC-RNTI anywhere in the
  /// lookahead portion of the resource grid.
  bool has_msg3_pusch_for(rnti_t tc_rnti) const
  {
    for (unsigned i = 0; i <= res_grid.max_ul_slot_alloc_delay; ++i) {
      for (const auto& pusch : res_grid[i].result_reader().ul().puschs()) {
        if (pusch.context.ue_index == INVALID_DU_UE_INDEX and pusch.context.msg3_delay.has_value() and
            pusch.pusch_cfg.rnti == tc_rnti) {
          return true;
        }
      }
    }
    return false;
  }

  /// Returns the pre-reserved MsgA PUSCH grant spanning all FDM occasions.
  grant_info get_msga_pusch_grant() const
  {
    const auto&    rach_cfg  = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
    const auto&    msga      = rach_cfg.two_step_rach_cfg->pusch;
    const auto&    ul_bwp    = cell_cfg.params.ul_cfg_common.init_ul_bwp;
    const auto&    td_alloc  = ul_bwp.pusch_cfg_common->pusch_td_alloc_list[msga.pusch_td_res_index];
    const unsigned crb_start = ul_bwp.generic_params.crbs.start() + msga.prb_start;
    return grant_info{ul_bwp.generic_params.scs,
                      td_alloc.symbols,
                      crb_interval{crb_start, crb_start + msga.po_fdm * msga.nof_prbs_per_msgA_po}};
  }

  /// Scans the lookahead grid and returns the nearest slot where the MsgA PUSCH grant is pre-reserved.
  std::optional<slot_point> find_msga_preresv_slot(const grant_info& grant) const
  {
    for (unsigned i = 1; i <= res_grid.max_ul_slot_alloc_delay; ++i) {
      if (res_grid[i].ul_res_grid.collides(grant)) {
        return res_grid[i].slot;
      }
    }
    return std::nullopt;
  }
};

TEST_P(ra_scheduler_two_step_rach_test, when_two_step_rach_enqueued_then_msga_pusch_is_scheduled)
{
  // Event: Enqueue RACH indication with two-step RACH preamble.
  const rnti_t tc_rnti  = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  auto         rach_ind = create_msga_rach_indication({make_msga_preamble(0, tc_rnti)});
  handle_rach_indication(rach_ind);

  // Test Case: MsgA PUSCH is scheduled in the slot td_offset after PRACH.
  ASSERT_TRUE(run_slot_until([this]() { return tracker.nof_msga_puschs() > 0; }, 10));
  ASSERT_EQ(tracker.nof_msga_puschs(), 1);
}

/// When MsgA PUSCH is decoded successfully (CRC=OK), the scheduler must respond with a SuccessRAR and must
/// not allocate a Msg3 PUSCH.
TEST_P(ra_scheduler_two_step_rach_test, when_msga_crc_ok_then_msgb_with_success_rar_scheduled_and_no_msg3)
{
  // Event: Enqueue RACH indication with two-step RACH preamble.
  const rnti_t tc_rnti = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  send_msga_rach({make_msga_preamble(0, tc_rnti)});

  // Event: MsgA PUSCH scheduled and forward CRC=OK.
  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  send_msga_crc(0, true);

  // Test: MsgB with SuccessRAR scheduled.
  ASSERT_TRUE(run_slot_until([this]() { return tracker.nof_success_rars() > 0; }));
  ASSERT_EQ(tracker.nof_success_rars(), 1);
  ASSERT_EQ(tracker.nof_fallback_rars(), 0);

  // Test: No Msg3 scheduled.
  ASSERT_FALSE(run_slot_until([this]() { return tracker.nof_msg3_newtxs() > 0; }));
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 0) << "SuccessRAR must not allocate a Msg3 PUSCH";
}

/// Verifies that the MsgB PDSCH and its PDCCH are carried by the beam of the SS/PBCH block that the UE reached the
/// cell on, the way the 4-step RAR is.
TEST_P(ra_scheduler_two_step_rach_test, msgb_pdsch_and_pdcch_use_the_beam_of_the_ssb)
{
  const rnti_t tc_rnti = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  send_msga_rach({make_msga_preamble(0, tc_rnti)});

  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  send_msga_crc(0, true);

  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().dl().rar_grants().empty(); }));

  ASSERT_EQ(beam_of(res_grid[0].result_reader().dl().rar_grants().front().pdsch_cfg.precoding_and_beamforming),
            test_ssb_beam);

  ASSERT_EQ(res_grid[0].result_reader().dl().dl_pdcchs().size(), 1);
  ASSERT_EQ(beam_of(res_grid[0].result_reader().dl().dl_pdcchs().front().ctx.precoding_and_beamforming), test_ssb_beam);
}

/// When MsgA PUSCH decoding fails (CRC=KO), the scheduler must respond with a FallbackRAR and allocate a
/// Msg3 PUSCH for the UE to fall back to the 4-step procedure.
TEST_P(ra_scheduler_two_step_rach_test, when_msga_crc_ko_then_fallback_rar_and_msg3_scheduled)
{
  const rnti_t tc_rnti = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  send_msga_rach({make_msga_preamble(0, tc_rnti)});
  run_slot();

  // Event: MsgA PUSCH scheduled and forward CRC=KO.
  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  send_msga_crc(0, false);

  ASSERT_TRUE(run_slot_until([this]() { return tracker.nof_fallback_rars() > 0; }));
  ASSERT_EQ(tracker.nof_fallback_rars(), 1);
  ASSERT_EQ(tracker.nof_success_rars(), 0) << "No SuccessRAR expected for CRC=KO preamble";

  std::optional<bool> msg3_rapid_set;
  ASSERT_TRUE(run_slot_until([this, &msg3_rapid_set]() {
    const auto puschs = res_grid[0].result_reader().ul().puschs();
    auto       it     = std::find_if(
        puschs.begin(), puschs.end(), [](const ul_sched_info& p) { return p.context.msg3_delay.has_value(); });
    if (it != puschs.end()) {
      msg3_rapid_set = it->context.rapid.has_value();
    }
    return tracker.nof_msg3_newtxs() > 0;
  }));
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 1) << "FallbackRAR must allocate a Msg3 PUSCH";
  ASSERT_TRUE(msg3_rapid_set.has_value()) << "Msg3 PUSCH was never observed in the resource grid";
  // "rapid" is a MsgA-only field; if set here, the PHY will wrongly apply the MsgA descrambling sequence
  // (TS38.211 6.3.1.1) to this regular Msg3 PUSCH and fail to decode it.
  ASSERT_FALSE(*msg3_rapid_set) << "FallbackRAR Msg3 PUSCH context.rapid must not be set";
}

/// While the MsgA PUSCH CRC indication has not yet arrived, MsgB scheduling must be postponed to allow
/// time for the CRC to be received.  Once the CRC arrives the MsgB must be scheduled promptly.
TEST_P(ra_scheduler_two_step_rach_test, when_crc_pending_then_msgb_scheduling_is_postponed)
{
  const rnti_t tc_rnti = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  send_msga_rach({make_msga_preamble(0, tc_rnti)});
  run_slot();

  // Run several slots without a CRC indication; MsgB must not be scheduled.
  ASSERT_FALSE(run_slot_until([this]() { return not res_grid[0].result_reader().dl().rar_grants().empty(); }, 5));

  // CRC=OK arrives; MsgB must now be scheduled.
  send_msga_crc(0, true);
  ASSERT_TRUE(run_slot_until([this]() { return tracker.nof_success_rars() > 0; }));
  ASSERT_EQ(tracker.nof_success_rars(), 1);
}

/// When multiple MsgA preambles arrive in the same PRACH occasion and their CRC outcomes differ, the scheduler
/// must send a SuccessRAR for the decoded preamble and a FallbackRAR (with Msg3) for the failed one, both
/// within the same MsgB response.
TEST_P(ra_scheduler_two_step_rach_test, when_mixed_crc_outcomes_both_rar_types_scheduled_together)
{
  const rnti_t tc_rnti_ok = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  const rnti_t tc_rnti_ko = to_rnti(to_underlying(rnti_t::MIN_CRNTI) + 1);
  send_msga_rach({make_msga_preamble(0, tc_rnti_ok), make_msga_preamble(1, tc_rnti_ko)});

  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  send_msga_crc(0, true);
  send_msga_crc(1, false);

  ASSERT_TRUE(run_slot_until([this]() { return tracker.nof_success_rars() > 0 and tracker.nof_fallback_rars() > 0; }));
  ASSERT_EQ(tracker.nof_success_rars(), 1);
  ASSERT_EQ(tracker.nof_fallback_rars(), 1);

  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  ASSERT_EQ(res_grid[0].result_reader().ul().puschs().size(), 1);
  ASSERT_EQ(res_grid[0].result_reader().ul().puschs()[0].pusch_cfg.rnti, tc_rnti_ko);
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 1) << "FallbackRAR preamble must have a Msg3 PUSCH";
}

/// TS38.213, Section 8.2A: the successRAR/fallbackRAR PDCCH (MsgB-RNTI DCI format 1_0) must carry the 2 LSBs of the
/// SFN where the UE transmitted PRACH whenever msgB-ResponseWindow is configured larger than 10 msec, or a
/// MsgB-conformant UE discards the DCI/PDSCH. Otherwise, these bits are just reserved (0).
TEST_P(ra_scheduler_two_step_rach_test, msgb_dci_carries_prach_sfn_lsbs_per_response_window_applicability)
{
  const rnti_t tc_rnti = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  send_msga_rach({make_msga_preamble(0, tc_rnti)});

  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  send_msga_crc(0, true);

  const pdcch_dl_information* msgb_pdcch = nullptr;
  ASSERT_TRUE(run_slot_until([this, &msgb_pdcch]() {
    const auto dl_pdcchs = res_grid[0].result_reader().dl().dl_pdcchs();
    const auto it        = std::find_if(dl_pdcchs.begin(), dl_pdcchs.end(), [](const pdcch_dl_information& p) {
      return p.dci.type() == dci_dl_rnti_config_type::ra_f1_0 and ra_helper::is_valid_msgb_rnti(p.ctx.rnti);
    });
    if (it != dl_pdcchs.end()) {
      msgb_pdcch = &*it;
      return true;
    }
    return false;
  }));
  ASSERT_NE(msgb_pdcch, nullptr);

  const auto&    rach_cfg       = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  const unsigned resp_window_ms = rach_cfg.two_step_rach_cfg->msgB_response_window_slots >>
                                  to_numerology_value(cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.scs);
  const unsigned expected_lsb_sfn = resp_window_ms > 10 ? (last_prach_slot_rx.sfn() & 0b11U) : 0U;
  EXPECT_EQ(msgb_pdcch->dci.as_ra_rnti_f1_0().lsb_sfn, expected_lsb_sfn);
}

/// The MsgA PUSCH resources must be blocked in the UL resource grid before any PRACH preamble is detected,
/// so that the UE PUSCH scheduler cannot steal them.
TEST_P(ra_scheduler_two_step_rach_test, msga_pusch_rbs_are_pre_reserved_before_preamble_detection)
{
  const grant_info grant = get_msga_pusch_grant();

  // No RACH indication has been sent. The scheduler must still pre-reserve the MsgA PUSCH resources.
  ASSERT_TRUE(run_slot_until([&]() { return find_msga_preresv_slot(grant).has_value(); }, 400))
      << "MsgA PUSCH pre-reservation not found within lookahead grid";
}

INSTANTIATE_TEST_SUITE_P(two_step_rach,
                         ra_scheduler_two_step_rach_test,
                         ::testing::Values(two_step_test_params{2},
                                           two_step_test_params{4},
                                           two_step_test_params{10, create_tdd_pattern(tdd_fr1_30khz::DDDDDDDSUU)},
                                           two_step_test_params{4, create_tdd_pattern(tdd_fr1_30khz::DSUU)}));

/// \brief Fixture for the 2-step RA of an NTN cell whose uplink reaches the gNB k_mac slots after the UE transmits it.
///
/// k_mac is longer than a radio frame, so the frame the UE transmitted its preamble in is always an earlier one than
/// the frame the gNB captured it in. The cell-specific Koffset the UE adds to its own uplink transmissions is
/// configured on top, with a different value, so that neither half of the uplink delay can stand in for the other.
class ra_scheduler_two_step_rach_ntn_kmac_test : public ra_scheduler_two_step_rach_test
{};

/// TS 38.213, Section 8.2A: the UE keeps the MsgB only if the LSBs of the SFN field match the SFN it transmitted the
/// PRACH in, so the field has to name the uplink frame of the UE, not the frame the gNB captured the preamble in.
TEST_P(ra_scheduler_two_step_rach_ntn_kmac_test, msgb_dci_carries_the_sfn_lsbs_of_the_uplink_frame_of_the_ue)
{
  const unsigned k_mac_slots = cell_cfg.ntn_k_mac_slots;
  ASSERT_GT(k_mac_slots, 0U);

  const rnti_t tc_rnti = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  send_msga_rach({make_msga_preamble(0, tc_rnti)});

  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  send_msga_crc(0, true);

  const pdcch_dl_information* msgb_pdcch = nullptr;
  ASSERT_TRUE(run_slot_until([this, &msgb_pdcch]() {
    const pdu_span<pdcch_dl_information> dl_pdcchs = res_grid[0].result_reader().dl().dl_pdcchs();
    auto it = std::find_if(dl_pdcchs.begin(), dl_pdcchs.end(), [](const pdcch_dl_information& p) {
      return p.dci.type() == dci_dl_rnti_config_type::ra_f1_0 and ra_helper::is_valid_msgb_rnti(p.ctx.rnti);
    });
    if (it != dl_pdcchs.end()) {
      msgb_pdcch = &*it;
      return true;
    }
    return false;
  }));
  ASSERT_NE(msgb_pdcch, nullptr);

  const auto&    rach_cfg       = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  const unsigned resp_window_ms = rach_cfg.two_step_rach_cfg->msgB_response_window_slots >>
                                  to_numerology_value(cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.scs);
  ASSERT_GT(resp_window_ms, 10U) << "the field is reserved below that window, so the test would prove nothing";

  const unsigned expected_lsb_sfn = last_msga_occasion_slot.sfn() & 0b11U;
  ASSERT_NE(expected_lsb_sfn, last_prach_slot_rx.sfn() & 0b11U)
      << "the capture frame and the uplink frame of the UE share their SFN LSBs, so the test cannot tell them apart";
  ASSERT_EQ(msgb_pdcch->dci.as_ra_rnti_f1_0().lsb_sfn, expected_lsb_sfn);
}

/// TS 38.213, Section 8.1A: msgA-PUSCH-TimeDomainOffset counts from the PRACH slot in the uplink BWP of the UE, so the
/// occasion is self-timed against the preamble. The gNB captures both k_mac later, which leaves the offset between the
/// two slots of the grid untouched.
TEST_P(ra_scheduler_two_step_rach_ntn_kmac_test, msga_pusch_keeps_its_td_offset_from_the_prach_capture_slot)
{
  ASSERT_GT(cell_cfg.ntn_k_mac_slots, 0U);

  const rnti_t tc_rnti = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  send_msga_rach({make_msga_preamble(0, tc_rnti)});

  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  ASSERT_EQ(res_grid[0].slot, last_prach_slot_rx + GetParam().td_offset)
      << "the MsgA PUSCH occasion was shifted, although it is timed against the PRACH the UE sent k_mac earlier too";
}

/// TS 38.213, Section 8.2A: the UE sends the successRAR's HARQ-ACK PUCCH in its own uplink slot n + k + Delta +
/// K_cell,offset, which reaches the gNB k_mac slots later. The slot tracked for that feedback is compared against the
/// slot of the UCI indication, which the gNB reports on its downlink clock, so it names the reception slot.
TEST_P(ra_scheduler_two_step_rach_ntn_kmac_test, msgb_harq_ack_slot_is_tracked_where_its_pucch_is_reserved)
{
  ASSERT_GT(cell_cfg.ntn_cs_koffset, 0U);
  ASSERT_GT(cell_cfg.ntn_k_mac_slots, 0U);

  const rnti_t tc_rnti = to_rnti(to_underlying(rnti_t::MIN_CRNTI));
  send_msga_rach({make_msga_preamble(0, tc_rnti)});

  ASSERT_TRUE(run_slot_until([this]() { return not res_grid[0].result_reader().ul().puschs().empty(); }));
  send_msga_crc(0, true);

  ASSERT_TRUE(run_slot_until([this]() { return tracker.nof_success_rars() > 0; }));

  std::optional<slot_point> pucch_slot;
  for (unsigned i = 0; i <= res_grid.max_ul_slot_alloc_delay and not pucch_slot.has_value(); ++i) {
    const span<const pucch_info> pucchs = res_grid[i].result_reader().ul().pucchs().unsorted();
    if (std::any_of(pucchs.begin(), pucchs.end(), [](const pucch_info& p) { return p.crnti == tc_rnti; })) {
      pucch_slot = res_grid[i].slot;
    }
  }
  ASSERT_TRUE(pucch_slot.has_value()) << "The successRAR was granted without a HARQ-ACK PUCCH";

  const auto ra_it = ra_ue_repo.find(tc_rnti);
  ASSERT_NE(ra_it, ra_ue_repo.end());
  ASSERT_TRUE(ra_it->msgb_slot_tx.has_value());
  ASSERT_TRUE(ra_it->msgb_ack_slot_tx.has_value());
  ASSERT_GE(*pucch_slot, *ra_it->msgb_slot_tx + cell_cfg.ntn_ul_alloc_delay_slots())
      << "The PUCCH was reserved without the uplink delay, so the test cannot tell the two clocks apart";
  ASSERT_EQ(*ra_it->msgb_ack_slot_tx, *pucch_slot)
      << "The tracked HARQ-ACK slot must name the slot the PUCCH is received in, not the slot the UE transmits it in";
}

INSTANTIATE_TEST_SUITE_P(two_step_rach_ntn_kmac,
                         ra_scheduler_two_step_rach_ntn_kmac_test,
                         ::testing::Values(two_step_test_params{.td_offset   = 2,
                                                                .tdd_cfg     = std::nullopt,
                                                                .ntn_k_mac   = std::chrono::milliseconds{13},
                                                                .ntn_koffset = std::chrono::milliseconds{9}},
                                           two_step_test_params{.td_offset   = 4,
                                                                .tdd_cfg     = std::nullopt,
                                                                .ntn_k_mac   = std::chrono::milliseconds{13},
                                                                .ntn_koffset = std::chrono::milliseconds{9}}));

/// Test fixture for CFRA-specific RA scheduler behaviour.
///
/// The cell is configured with 4 dedicated CFRA preambles (IDs [60, 64)), leaving 60 preambles for CBRA.
class ra_scheduler_cfra_test : public ra_scheduler_setup, public ::testing::Test
{
  static constexpr unsigned NOF_CB_PREAMBLES = 60;

public:
  ra_scheduler_cfra_test(const scheduler_expert_config& sched_cfg_ = {}) :
    ra_scheduler_setup(sched_cfg_, make_cfra_sched_req(), false, false)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-b");

    // The RA scheduler classifies a CRC as a CFRA Msg3 by looking the C-RNTI up in the cell UE repository, so the
    // CFRA UE must be registered there.
    auto ue_req                    = sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    ue_req.ue_index                = cfra_ue_index;
    ue_req.crnti                   = cfra_crnti;
    ue_req.cfra_enabled            = true;
    ue_req.starts_in_fallback      = true;
    ue_req.ul_ccch_slot_rx         = std::nullopt;
    const ue_configuration* ue_cfg = cfg_mng.add_ue(ue_req);
    report_error_if_not(ue_cfg != nullptr, "Failed to create the CFRA UE configuration");
    ue_db.add_ue(*ue_cfg, {sched_config_helper::to_ue_creation_mode(ue_req)});
  }

  static sched_cell_configuration_request_message make_cfra_sched_req()
  {
    cell_config_builder_params bparams;
    auto                       req = sched_config_helper::make_default_sched_cell_configuration_request(bparams);
    req.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb = NOF_CB_PREAMBLES;
    return req;
  }

  rach_indication_message create_cfra_rach_indication(rnti_t tc_rnti) const
  {
    const unsigned cfra_preamble_id =
        cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb;
    auto preamble = test_helper::create_preamble(cfra_preamble_id, tc_rnti);
    return test_helper::create_rach_indication(cell_cfg, next_slot_rx(), {preamble});
  }

  rach_indication_message create_cbra_rach_indication(rnti_t tc_rnti) const
  {
    auto preamble = test_helper::create_preamble(0, tc_rnti);
    return test_helper::create_rach_indication(cell_cfg, next_slot_rx(), {preamble});
  }

  void send_cfra_crc(rnti_t tc_rnti, bool success, std::optional<float> sinr_dB = std::nullopt)
  {
    ul_crc_indication crc_ind;
    crc_ind.cell_index = cell_cfg.cell_index;
    crc_ind.sl_rx      = res_grid[0].slot;
    auto& pdu          = crc_ind.crcs.emplace_back();
    pdu.rnti           = tc_rnti;
    pdu.ue_index       = cfra_ue_index;
    pdu.harq_id        = to_harq_id(0);
    pdu.tb_crc_success = success;
    pdu.ul_sinr_dB     = sinr_dB;
    handle_crc_indication(crc_ind);
  }

  void send_cbra_crc(rnti_t tc_rnti, bool success, std::optional<float> sinr_dB = std::nullopt)
  {
    ul_crc_indication crc_ind;
    crc_ind.cell_index = cell_cfg.cell_index;
    crc_ind.sl_rx      = res_grid[0].slot;
    auto& pdu          = crc_ind.crcs.emplace_back();
    pdu.rnti           = tc_rnti;
    pdu.ue_index       = INVALID_DU_UE_INDEX;
    pdu.harq_id        = to_harq_id(0);
    pdu.tb_crc_success = success;
    pdu.ul_sinr_dB     = sinr_dB;
    handle_crc_indication(crc_ind);
  }

  const du_ue_index_t cfra_ue_index = to_du_ue_index(5);
  const rnti_t        cfra_crnti    = to_rnti(0x4601);
};

/// \brief Test fixture for a CFRA UE that holds a PUCCH in every UL slot, so that every candidate Msg3 slot
/// collides with its UCI.
///
/// Parameterized on \c scheduler_ra_expert_config::multiplex_uci_on_cfra_msg3.
class ra_scheduler_cfra_uci_on_msg3_test : public ra_scheduler_setup, public ::testing::TestWithParam<bool>
{
  static constexpr unsigned NOF_CB_PREAMBLES = 60;

public:
  ra_scheduler_cfra_uci_on_msg3_test() :
    ra_scheduler_setup(make_expert_cfg(GetParam()), make_cfra_sched_req(), false, false)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-b");

    auto ue_req                    = sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    ue_req.ue_index                = cfra_ue_index;
    ue_req.crnti                   = cfra_crnti;
    ue_req.cfra_enabled            = true;
    ue_req.starts_in_fallback      = true;
    ue_req.ul_ccch_slot_rx         = std::nullopt;
    const ue_configuration* ue_cfg = cfg_mng.add_ue(ue_req);
    report_error_if_not(ue_cfg != nullptr, "Failed to create the CFRA UE configuration");
    ue_db.add_ue(*ue_cfg, {sched_config_helper::to_ue_creation_mode(ue_req)});
  }

  static scheduler_expert_config make_expert_cfg(bool multiplex_uci_on_cfra_msg3)
  {
    auto cfg                                = config_helpers::make_default_scheduler_expert_config();
    cfg.ra.multiplex_uci_on_cf_rar_ul_grant = multiplex_uci_on_cfra_msg3;
    return cfg;
  }

  static sched_cell_configuration_request_message make_cfra_sched_req()
  {
    cell_config_builder_params bparams;
    auto                       req = sched_config_helper::make_default_sched_cell_configuration_request(bparams);
    req.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb = NOF_CB_PREAMBLES;
    return req;
  }

  rach_indication_message create_cfra_rach_indication() const
  {
    const unsigned cfra_preamble_id =
        cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb;
    auto preamble = test_helper::create_preamble(cfra_preamble_id, cfra_crnti);
    return test_helper::create_rach_indication(cell_cfg, next_slot_rx(), {preamble});
  }

  void do_run_slot() override
  {
    // Reserve a CSI PUCCH for the CFRA UE in every UL slot the RA scheduler could pick for its Msg3. CSI is used
    // rather than SR because only HARQ-ACK and CSI bits are carried over to the PUSCH (SR is dropped instead).
    const ue_cell_configuration& ue_cc_cfg = ue_cell_db[cfra_ue_index].cfg();
    for (unsigned i = 0; i <= res_grid.max_ul_slot_alloc_delay; ++i) {
      cell_slot_resource_allocator& slot_alloc = res_grid[i];
      if (not cell_cfg.is_ul_enabled(slot_alloc.slot)) {
        continue;
      }
      // Skip slots that already hold a grant for the UE: re-adding a PUCCH next to its Msg3 PUSCH would recreate
      // the very overlap the UCI multiplexing is meant to resolve.
      span<const pucch_info> pucchs = slot_alloc.result_reader().ul().pucchs().unsorted();
      const auto             puschs = slot_alloc.result_reader().ul().puschs();
      const bool             has_grant =
          std::any_of(pucchs.begin(), pucchs.end(), [this](const pucch_info& p) { return p.crnti == cfra_crnti; }) or
          std::any_of(
              puschs.begin(), puschs.end(), [this](const ul_sched_info& p) { return p.pusch_cfg.rnti == cfra_crnti; });
      if (not has_grant) {
        pucch_alloc.alloc_csi_opportunity(slot_alloc, ue_cc_cfg);
      }
    }
    ra_scheduler_setup::do_run_slot();
  }

  const du_ue_index_t cfra_ue_index = to_du_ue_index(5);
  const rnti_t        cfra_crnti    = to_rnti(0x4601);
};

TEST_P(ra_scheduler_cfra_uci_on_msg3_test, msg3_is_only_scheduled_over_a_pucch_when_uci_multiplexing_is_enabled)
{
  handle_rach_indication(create_cfra_rach_indication());

  bool msg3_seen     = false;
  bool msg3_with_uci = false;
  for (unsigned slot_count = 0, max_slots = 200; slot_count != max_slots; ++slot_count) {
    run_slot();
    for (const ul_sched_info& pusch : res_grid[0].result_reader().ul().puschs()) {
      if (pusch.pusch_cfg.rnti != cfra_crnti) {
        continue;
      }
      msg3_seen     = true;
      msg3_with_uci = msg3_with_uci or pusch.uci.has_value();
    }
  }

  if (GetParam()) {
    ASSERT_TRUE(msg3_seen) << "Msg3 should be placed over the UE PUCCH when UCI multiplexing is enabled";
    ASSERT_TRUE(msg3_with_uci) << "Msg3 should carry the UCI moved off the PUCCH";
  } else {
    ASSERT_FALSE(msg3_seen) << "Msg3 should avoid slots where the CFRA UE has a PUCCH";
  }
}

INSTANTIATE_TEST_SUITE_P(uci_on_msg3, ra_scheduler_cfra_uci_on_msg3_test, ::testing::Bool());

/// Verify that a Msg3 CRC with a valid UE index (CFRA path) is accepted by the RA scheduler.
TEST_F(ra_scheduler_cfra_test, cfra_msg3_crc_with_valid_ue_index_is_accepted)
{
  const rnti_t tc_rnti = cfra_crnti;
  handle_rach_indication(create_cfra_rach_indication(tc_rnti));

  for (unsigned slot_count = 0, max_slots = 1000; slot_count < max_slots and tracker.nof_msg3_acked() == 0;
       ++slot_count) {
    run_slot();
    if (not res_grid[0].result_reader().ul().puschs().empty()) {
      send_cfra_crc(tc_rnti, true);
    }
  }

  ASSERT_EQ(tracker.nof_msg3_acked(), 1);
}

/// Verify the Msg3 retransmission flow for a CFRA UE: CRC KO triggers retx, CRC OK completes the procedure.
TEST_F(ra_scheduler_cfra_test, cfra_msg3_crc_ko_causes_retx_then_ok_completes)
{
  const rnti_t tc_rnti = cfra_crnti;
  handle_rach_indication(create_cfra_rach_indication(tc_rnti));

  // NACK the first Msg3 new-tx.
  for (unsigned slot_count = 0, max_slots = 1000; slot_count < max_slots and tracker.nof_msg3_newtxs() == 0;
       ++slot_count) {
    run_slot();
    if (not res_grid[0].result_reader().ul().puschs().empty()) {
      send_cfra_crc(tc_rnti, false);
    }
  }
  ASSERT_GE(tracker.nof_msg3_newtxs(), 1);
  ASSERT_EQ(tracker.nof_msg3_acked(), 0);

  // ACK the retx.
  for (unsigned slot_count = 0, max_slots = 1000; slot_count < max_slots and tracker.nof_msg3_acked() == 0;
       ++slot_count) {
    run_slot();
    if (not res_grid[0].result_reader().ul().puschs().empty()) {
      send_cfra_crc(tc_rnti, true);
    }
  }

  ASSERT_GE(tracker.nof_msg3_retxs(), 1);
  ASSERT_EQ(tracker.nof_msg3_acked(), 1);
}

/// Verify that, when a NACKed Msg3 HARQ is never rescheduled for retransmission (starved until the retx timeout
/// fires), its pending_msg3s ring entry is released -- so a later PRACH reusing the same TC-RNTI is accepted
/// instead of being rejected as "already under use".
TEST_F(ra_scheduler_cfra_test, when_msg3_retx_starves_then_tc_rnti_is_released_for_reuse)
{
  const rnti_t tc_rnti = cfra_crnti;
  handle_rach_indication(create_cfra_rach_indication(tc_rnti));

  // NACK the first Msg3 new-tx, putting its HARQ into pending_retx state.
  for (unsigned slot_count = 0, max_slots = 1000; slot_count < max_slots and tracker.nof_msg3_newtxs() == 0;
       ++slot_count) {
    run_slot();
    if (not res_grid[0].result_reader().ul().puschs().empty()) {
      send_cfra_crc(tc_rnti, false);
    }
  }
  ASSERT_GE(tracker.nof_msg3_newtxs(), 1);

  // Block all UL resources so the scheduler can never grant the Msg3 retransmission, until the retx timeout fires
  // and the HARQ (and its pending_msg3s ring entry) is force-discarded.
  const grant_info marked_res{cell_cfg.scs_common(),
                              ofdm_symbol_range{0, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP},
                              crb_interval{0, cell_cfg.nof_ul_prbs}};
  const unsigned   lookahead_ul_rbs = res_grid.max_ul_slot_alloc_delay;
  for (unsigned i = 0; i != lookahead_ul_rbs; ++i) {
    if (cell_cfg.is_ul_enabled(res_grid[i].slot)) {
      res_grid[i].ul_res_grid.fill(marked_res);
    }
  }
  const unsigned nof_retxs_before_block = tracker.nof_msg3_retxs();
  for (unsigned i = 0, max_slots = 500; i != max_slots; ++i) {
    if (cell_cfg.is_ul_enabled(res_grid[lookahead_ul_rbs].slot)) {
      res_grid[lookahead_ul_rbs].ul_res_grid.fill(marked_res);
    }
    run_slot();
  }
  ASSERT_EQ(tracker.nof_msg3_retxs(), nof_retxs_before_block)
      << "No retx grant should have been possible while UL resources were blocked";

  // Drain the lookahead window of already-blocked slots (without refilling) before probing with a new preamble.
  for (unsigned i = 0; i != lookahead_ul_rbs; ++i) {
    run_slot();
  }

  // A new preamble reusing the SAME TC-RNTI must now be accepted. If pending_msg3s still held a stale entry for
  // this TC-RNTI, ra_scheduler's ring-collision check would silently drop this preamble instead.
  handle_rach_indication(create_cfra_rach_indication(tc_rnti));
  for (unsigned slot_count = 0, max_slots = 1000; slot_count < max_slots and tracker.nof_msg3_newtxs() < 2;
       ++slot_count) {
    run_slot();
  }
  ASSERT_GE(tracker.nof_msg3_newtxs(), 2)
      << "TC-RNTI reuse after the retx timeout was rejected -- pending_msg3s leaked";
}

/// Fixture for a CFRA UE whose Msg3 is never retransmitted.
class ra_scheduler_cfra_no_msg3_retx_test : public ra_scheduler_cfra_test
{
protected:
  ra_scheduler_cfra_no_msg3_retx_test() : ra_scheduler_cfra_test(make_expert_cfg()) {}

  static scheduler_expert_config make_expert_cfg()
  {
    scheduler_expert_config cfg{};
    cfg.ra.max_nof_msg3_harq_retxs = 0;
    return cfg;
  }
};

/// Verify that a CFRA UE that missed its RAR gets a new RAR for its retransmitted dedicated preamble, while the
/// ra-ContentionResolutionTimer of its previous attempt is still running.
TEST_F(ra_scheduler_cfra_no_msg3_retx_test, when_cfra_preamble_is_retransmitted_then_new_rar_is_scheduled)
{
  const rnti_t tc_rnti = cfra_crnti;
  handle_rach_indication(create_cfra_rach_indication(tc_rnti));
  const slot_point first_prach_slot = next_slot_rx();

  // The UE missed the RAR, so its Msg3 is never received.
  for (unsigned i = 0, max_slots = 1000; i < max_slots and tracker.nof_msg3_newtxs() == 0; ++i) {
    run_slot();
  }
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 1) << "Msg3 new-tx was not scheduled";
  ASSERT_EQ(tracker.nof_rars(), 1);
  send_cfra_crc(tc_rnti, false);

  handle_rach_indication(create_cfra_rach_indication(tc_rnti));
  const unsigned conres_timer_slots =
      cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common->ra_con_res_timer.count() *
      get_nof_slots_per_subframe(cell_cfg.scs_common());
  ASSERT_LT(next_slot_rx() - first_prach_slot, conres_timer_slots)
      << "The retransmitted preamble must arrive while the previous attempt is still alive";

  for (unsigned i = 0, max_slots = 1000; i < max_slots and tracker.nof_msg3_newtxs() < 2; ++i) {
    run_slot();
  }
  ASSERT_EQ(tracker.nof_rars(), 2) << "No RAR was scheduled for the retransmitted CFRA preamble";
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 2);
  ASSERT_EQ(tracker.nof_msg3_retxs(), 0);
}

/// Verify that a retransmitted CFRA preamble is discarded while the Msg3 of the previous attempt awaits its CRC.
TEST_F(ra_scheduler_cfra_no_msg3_retx_test, when_cfra_preamble_is_retransmitted_before_msg3_crc_then_it_is_discarded)
{
  const rnti_t tc_rnti = cfra_crnti;
  handle_rach_indication(create_cfra_rach_indication(tc_rnti));
  for (unsigned i = 0, max_slots = 1000; i < max_slots and tracker.nof_msg3_newtxs() == 0; ++i) {
    run_slot();
  }
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 1) << "Msg3 new-tx was not scheduled";

  // No CRC is reported for the Msg3, so its HARQ keeps awaiting it.
  handle_rach_indication(create_cfra_rach_indication(tc_rnti));
  // Outlasts the longest ra-ResponseWindow (sl80).
  for (unsigned i = 0, max_slots = 100; i != max_slots; ++i) {
    run_slot();
  }
  ASSERT_EQ(tracker.nof_rars(), 1) << "A RAR was scheduled while the previous Msg3 was awaiting its CRC";
}

/// Verify that a CRC carrying a UE index is filtered when its RNTI does not belong to a UE undergoing a CFRA.
///
/// This is the CBRA UE that has already completed RA: its C-RNTI is the TC-RNTI that the RA scheduler still holds
/// in \c ra_ue_repository, so an unfiltered CRC would free a Msg3 HARQ that the UE scheduler now owns.
TEST_F(ra_scheduler_cfra_test, crc_with_ue_index_of_non_cfra_rnti_is_filtered)
{
  const rnti_t tc_rnti            = to_rnti(0x4602);
  const auto   unrelated_ue_index = to_du_ue_index(10);
  handle_rach_indication(create_cbra_rach_indication(tc_rnti));

  // Wait for the Msg3 new-tx.
  for (unsigned slot_count = 0, max_slots = 1000; slot_count < max_slots and tracker.nof_msg3_newtxs() == 0;
       ++slot_count) {
    run_slot();
  }
  ASSERT_GE(tracker.nof_msg3_newtxs(), 1) << "Msg3 new-tx was not scheduled";

  // Send a CRC with a valid UE index directly to the RA scheduler, bypassing the tracker. The TC-RNTI is not that of
  // a UE undergoing a CFRA, so the CRC must be rejected.
  {
    ul_crc_indication bad_crc;
    bad_crc.cell_index = cell_cfg.cell_index;
    bad_crc.sl_rx      = res_grid[0].slot;
    auto& pdu          = bad_crc.crcs.emplace_back();
    pdu.rnti           = tc_rnti;
    pdu.ue_index       = unrelated_ue_index;
    pdu.harq_id        = to_harq_id(0);
    pdu.tb_crc_success = true;
    ra_sch.handle_crc_indication(bad_crc);
  }
  // Send a valid NACK to give the HARQ real feedback and trigger a retransmission.
  // If the bad CRC above was wrongly accepted, the HARQ would already be freed and no retx would appear.
  send_cbra_crc(tc_rnti, false);

  // The HARQ must not have been freed by the filtered CRC -- a retransmission must be scheduled.
  for (unsigned i = 0, max_slots = 1000; i < max_slots and tracker.nof_msg3_retxs() == 0; ++i) {
    run_slot();
  }
  ASSERT_GE(tracker.nof_msg3_retxs(), 1) << "Filtered CRC must not free the HARQ. Retx expected";
}

/// Fixture with a custom Msg3 DTX SINR threshold.
class ra_scheduler_msg3_dtx_test : public ra_scheduler_cfra_test
{
protected:
  static constexpr float dtx_sinr_threshold_dB = -5.0F;

  ra_scheduler_msg3_dtx_test(float threshold_dB = dtx_sinr_threshold_dB) :
    ra_scheduler_cfra_test(make_expert_cfg(threshold_dB))
  {
  }

  static scheduler_expert_config make_expert_cfg(float threshold_dB)
  {
    scheduler_expert_config cfg{};
    cfg.ra.msg3_dtx_sinr_threshold_dB = threshold_dB;
    return cfg;
  }

  /// Runs until the Msg3 new-tx is in the current slot and NACKs it with the given SINR.
  void nack_msg3_newtx(rnti_t tc_rnti, bool cfra, std::optional<float> sinr_dB)
  {
    for (unsigned i = 0, max_slots = 1000; i < max_slots and tracker.nof_msg3_newtxs() == 0; ++i) {
      run_slot();
    }
    ASSERT_EQ(tracker.nof_msg3_newtxs(), 1) << "Msg3 new-tx was not scheduled";
    if (cfra) {
      send_cfra_crc(tc_rnti, false, sinr_dB);
    } else {
      send_cbra_crc(tc_rnti, false, sinr_dB);
    }
  }

  bool msg3_retx_scheduled_in_next_slots(unsigned nof_slots)
  {
    for (unsigned i = 0; i != nof_slots and tracker.nof_msg3_retxs() == 0; ++i) {
      run_slot();
    }
    return tracker.nof_msg3_retxs() > 0;
  }
};

TEST_F(ra_scheduler_msg3_dtx_test, when_cbra_msg3_crc_ko_has_sinr_below_threshold_then_retxs_are_dropped)
{
  const rnti_t tc_rnti = to_rnti(0x4602);
  handle_rach_indication(create_cbra_rach_indication(tc_rnti));
  ASSERT_NO_FATAL_FAILURE(nack_msg3_newtx(tc_rnti, false, dtx_sinr_threshold_dB - 1));

  ASSERT_FALSE(msg3_retx_scheduled_in_next_slots(100));
}

/// The dropped RA attempt must release its TC-RNTI right away, rather than at the ra-ContentionResolutionTimer expiry.
TEST_F(ra_scheduler_msg3_dtx_test, when_cbra_msg3_is_dropped_as_dtx_then_tc_rnti_is_released_for_reuse)
{
  const rnti_t tc_rnti = to_rnti(0x4602);
  handle_rach_indication(create_cbra_rach_indication(tc_rnti));
  ASSERT_NO_FATAL_FAILURE(nack_msg3_newtx(tc_rnti, false, dtx_sinr_threshold_dB - 1));

  // A new preamble reusing the TC-RNTI would be discarded if the repository still held the previous RA attempt.
  ASSERT_EQ(ra_ue_repo.find(tc_rnti), ra_ue_repo.end()) << "TC-RNTI of the dropped Msg3 was not released";
}

TEST_F(ra_scheduler_msg3_dtx_test, when_cbra_msg3_crc_ko_has_sinr_above_threshold_then_retx_is_scheduled)
{
  const rnti_t tc_rnti = to_rnti(0x4602);
  handle_rach_indication(create_cbra_rach_indication(tc_rnti));
  ASSERT_NO_FATAL_FAILURE(nack_msg3_newtx(tc_rnti, false, dtx_sinr_threshold_dB + 1));

  ASSERT_TRUE(msg3_retx_scheduled_in_next_slots(100));
}

TEST_F(ra_scheduler_msg3_dtx_test, when_cbra_msg3_crc_ko_has_no_sinr_then_retx_is_scheduled)
{
  const rnti_t tc_rnti = to_rnti(0x4602);
  handle_rach_indication(create_cbra_rach_indication(tc_rnti));
  ASSERT_NO_FATAL_FAILURE(nack_msg3_newtx(tc_rnti, false, std::nullopt));

  ASSERT_TRUE(msg3_retx_scheduled_in_next_slots(100));
}

TEST_F(ra_scheduler_msg3_dtx_test, when_cfra_msg3_crc_ko_has_sinr_below_threshold_then_retx_is_scheduled)
{
  handle_rach_indication(create_cfra_rach_indication(cfra_crnti));
  ASSERT_NO_FATAL_FAILURE(nack_msg3_newtx(cfra_crnti, true, dtx_sinr_threshold_dB - 1));

  ASSERT_TRUE(msg3_retx_scheduled_in_next_slots(100));
}

TEST_F(ra_scheduler_cfra_test, when_cbra_msg3_crc_ko_has_sinr_below_default_threshold_then_retxs_are_dropped)
{
  const rnti_t tc_rnti = to_rnti(0x4602);
  handle_rach_indication(create_cbra_rach_indication(tc_rnti));
  for (unsigned i = 0, max_slots = 1000; i < max_slots and tracker.nof_msg3_newtxs() == 0; ++i) {
    run_slot();
  }
  ASSERT_EQ(tracker.nof_msg3_newtxs(), 1);
  send_cbra_crc(tc_rnti, false, -16.0F);

  for (unsigned i = 0; i != 100; ++i) {
    run_slot();
  }
  ASSERT_EQ(tracker.nof_msg3_retxs(), 0);
}

/// Fixture with the Msg3 DTX detection disabled via the lowest possible SINR threshold.
class ra_scheduler_msg3_dtx_disabled_test : public ra_scheduler_msg3_dtx_test
{
protected:
  ra_scheduler_msg3_dtx_disabled_test() : ra_scheduler_msg3_dtx_test(std::numeric_limits<float>::lowest()) {}
};

TEST_F(ra_scheduler_msg3_dtx_disabled_test, when_msg3_dtx_threshold_is_lowest_then_low_sinr_crc_ko_triggers_retx)
{
  const rnti_t tc_rnti = to_rnti(0x4602);
  handle_rach_indication(create_cbra_rach_indication(tc_rnti));
  ASSERT_NO_FATAL_FAILURE(nack_msg3_newtx(tc_rnti, false, -100.0F));

  ASSERT_TRUE(msg3_retx_scheduled_in_next_slots(100));
}

// -------- NTN k_mac RAR window tests --------

/// Test fixture that configures a cell with NTN params (koffset=10ms, k_mac=5ms).
/// Verifies that the RA scheduler handles the full RA flow when k_mac is set.
class ra_scheduler_ntn_kmac_test : public ra_scheduler_setup, public ::testing::Test
{
public:
  static constexpr unsigned K_OFFSET_MS = 10;
  static constexpr unsigned K_MAC_MS    = 19;

  ra_scheduler_ntn_kmac_test() : ra_scheduler_setup(make_req(), false, false) {}

  static sched_cell_configuration_request_message make_req()
  {
    auto req = sched_config_helper::make_default_sched_cell_configuration_request(
        cell_config_builder_profiles::create(duplex_mode::FDD));
    req.ran.ntn_params.emplace();
    req.ran.ntn_params->ntn_cfg.cell_specific_koffset = std::chrono::milliseconds{K_OFFSET_MS};
    req.ran.ntn_params->ntn_cfg.k_mac                 = std::chrono::milliseconds{K_MAC_MS};
    return req;
  }

  /// Simulates the DL-time PRACH detection: after the PRACH PDU placement fix, the lower PHY
  /// reports prach_slot_rx = UL_occasion + k_mac (the DL clock time when samples arrive at gNB).
  /// Both the scheduler and tracker receive the k_mac-shifted slot so the RAR window check is
  /// consistent: expected window = [prach_slot_rx + 1, ...) with no additional k_mac offset.
  void handle_rach_indication_ntn(unsigned nof_preambles)
  {
    const prach_helper::preamble_slot_mapping prach_mapper{
        cell_cfg.band(),
        cell_cfg.init_bwp.ul.cfg().scs,
        cell_cfg.init_bwp.ul.rach_common()->rach_cfg_generic.prach_config_index};
    run_slot_until([this, &prach_mapper]() { return prach_mapper.has_prach_occasion(next_slot_rx()); });

    std::vector<rach_indication_message::preamble> preambles;
    for (unsigned i = 0; i != nof_preambles; ++i) {
      preambles.push_back(create_random_preamble());
    }
    // The UE transmits in UL occasion slot p, which the gNB detects on its DL clock at p + k_mac. The reported
    // occasion t_id names the UL occasion, since the FAPI adaptor rebases it by k_mac.
    const slot_point        ul_occasion_slot = next_slot_rx();
    rach_indication_message ind = test_helper::create_rach_indication(cell_cfg, ul_occasion_slot, preambles);
    ind.slot_rx                 = cell_cfg.ue_ul_to_dl_slot(ul_occasion_slot);
    ra_sch.handle_rach_indication(ind);
    tracker.on_new_rach_ind(ind);
  }
};

/// Verifies that the full RA flow (PRACH -> RAR -> Msg3) completes when k_mac is set.
/// The RACH indication is injected with slot_rx = UL_occasion + k_mac to model the DL-time
/// detection reported by the lower PHY after the PRACH PDU placement fix.
TEST_F(ra_scheduler_ntn_kmac_test, rach_flow_succeeds_with_kmac_prach_slot)
{
  handle_rach_indication_ntn(1);

  for (unsigned slot_count = 0, max_slots = 5000; slot_count < max_slots and tracker.nof_msg3_acked() == 0;
       ++slot_count) {
    run_slot();
    handle_crc_for_pending_puschs(true);
  }

  ASSERT_EQ(tracker.nof_rars(), 1) << "Expected exactly one RAR after k_mac-shifted PRACH";
  ASSERT_EQ(tracker.nof_msg3_acked(), 1) << "Msg3 was not ACKed";
}

/// Verifies that Msg3 is scheduled the cell-specific Koffset plus k_mac after its Msg3 delay.
TEST_F(ra_scheduler_ntn_kmac_test, msg3_is_scheduled_the_total_uplink_delay_after_its_msg3_delay)
{
  ASSERT_GT(cell_cfg.ntn_cs_koffset, 0U);
  ASSERT_GT(cell_cfg.ntn_k_mac_slots, 0U);

  handle_rach_indication_ntn(1);

  const rar_information* rar = nullptr;
  for (unsigned slot_count = 0; slot_count != 1000 and rar == nullptr; ++slot_count) {
    run_slot();
    if (not res_grid[0].result_reader().dl().rar_grants().empty()) {
      rar = &res_grid[0].result_reader().dl().rar_grants().front();
    }
  }
  ASSERT_NE(rar, nullptr) << "No RAR was scheduled";
  ASSERT_EQ(rar->grants.size(), 1U);
  const rar_ul_grant& grant = rar->grants.front();

  // TS 38.213, Section 8.3: the Msg3 PUSCH is transmitted K2 + delta slots after the RAR, extended by K_offset in NTN.
  // The gNB receives it k_mac slots later still, which is where the grid holds it.
  const auto&    ul_bwp     = cell_cfg.params.ul_cfg_common.init_ul_bwp;
  const unsigned k2         = ul_bwp.pusch_cfg_common->pusch_td_alloc_list[grant.time_resource_assignment].k2;
  const unsigned msg3_delay = ra_helper::get_msg3_delay(ul_bwp.generic_params.scs, k2);

  const auto has_msg3 = [&grant](const cell_slot_resource_allocator& alloc) {
    const pdu_span<ul_sched_info> puschs = alloc.result_reader().ul().puschs();
    return std::any_of(puschs.begin(), puschs.end(), [&grant](const ul_sched_info& p) {
      return p.pusch_cfg.rnti == grant.temp_crnti;
    });
  };
  ASSERT_TRUE(has_msg3(res_grid[msg3_delay + cell_cfg.ntn_ul_alloc_delay_slots()]))
      << "Msg3 must be scheduled K_offset plus k_mac slots after the Msg3 delay";
  ASSERT_FALSE(has_msg3(res_grid[msg3_delay + cell_cfg.ntn_cs_koffset]))
      << "Msg3 must not be scheduled at the K_offset delay alone, without k_mac";
  ASSERT_FALSE(has_msg3(res_grid[msg3_delay])) << "Msg3 must not be scheduled at the terrestrial Msg3 delay";
}

/// Verifies that the RA procedure completes when the Msg3 retransmissions are delayed by the Koffset and k_mac too.
TEST_F(ra_scheduler_ntn_kmac_test, ra_procedure_completes_with_msg3_retransmissions)
{
  const unsigned nof_preambles = 2;
  handle_rach_indication_ntn(nof_preambles);

  // NACK the first Msg3 transmissions, so that their retransmissions are scheduled over the NTN delay too.
  for (unsigned slot_count = 0; slot_count != 5000 and tracker.nof_msg3_newtxs() < nof_preambles; ++slot_count) {
    run_slot();
    handle_crc_for_pending_puschs(false);
  }
  for (unsigned slot_count = 0; slot_count != 5000 and tracker.nof_msg3_acked() < nof_preambles; ++slot_count) {
    run_slot();
    handle_crc_for_pending_puschs(true);
  }

  ASSERT_EQ(tracker.nof_msg3_newtxs(), nof_preambles);
  ASSERT_GE(tracker.nof_msg3_retxs(), nof_preambles);
  ASSERT_EQ(tracker.nof_msg3_acked(), nof_preambles);
}

// -------- NTN k_mac multi-SSB tests --------

/// Same NTN configuration as \c ra_scheduler_ntn_kmac_test, but with a second active SS/PBCH block.
///
/// With a single active beam, \c ssb_to_ro_mapping resolves every occasion to that beam without consulting its
/// occasion table, so the slot it is queried with does not matter. From two beams on, the table is built and indexed
/// by the PRACH slot: the occasions are laid out in the uplink frame the UE transmits in (TS 38.211 Section 6.3.3.2),
/// while the RACH indication reports the slot the preamble was captured in, k_mac later. Querying the table with the
/// capture slot lands on a slot that holds no PRACH occasion, so the preamble resolves to no SS/PBCH block and is
/// discarded before a RAR is ever scheduled.
class ra_scheduler_ntn_kmac_multi_ssb_test : public ra_scheduler_setup, public ::testing::Test
{
public:
  static constexpr unsigned K_OFFSET_MS = 10;
  static constexpr unsigned K_MAC_MS    = 19;
  /// Fixed so the expected SS/PBCH block can be derived with the same preamble the RACH indication carries.
  static constexpr unsigned PREAMBLE_ID = 0;
  static constexpr unsigned FREQ_IDX    = 0;
  static constexpr unsigned TD_OCCASION = 0;

  ra_scheduler_ntn_kmac_multi_ssb_test() : ra_scheduler_setup(make_req(), false, false) {}

  static sched_cell_configuration_request_message make_req()
  {
    auto req = sched_config_helper::make_default_sched_cell_configuration_request(
        cell_config_builder_profiles::create(duplex_mode::FDD));
    req.ran.ntn_params.emplace();
    req.ran.ntn_params->ntn_cfg.cell_specific_koffset = std::chrono::milliseconds{K_OFFSET_MS};
    req.ran.ntn_params->ntn_cfg.k_mac                 = std::chrono::milliseconds{K_MAC_MS};
    // The profile activates SS/PBCH block 0 only; add a second one so the association table is actually built.
    req.ran.ssb_cfg.ssb_beams.set_beam(1, to_beam_id(1));
    return req;
  }

  /// Association the scheduler resolves preambles against, rebuilt here to derive the expected SS/PBCH block.
  prach_helper::ssb_to_ro_mapping make_reference_mapping() const
  {
    const auto& ul_bwp = cell_cfg.params.ul_cfg_common.init_ul_bwp;
    return prach_helper::ssb_to_ro_mapping{prach_helper::prach_occasion_mapping_config{cell_cfg.params.dl_carrier.band,
                                                                                       ul_bwp.generic_params.scs,
                                                                                       ul_bwp.generic_params.cp,
                                                                                       *ul_bwp.rach_cfg_common,
                                                                                       cell_cfg.params.ssb_cfg,
                                                                                       cell_cfg.params.tdd_cfg}};
  }
};

/// Verifies that a preamble detected with more than one active SS/PBCH block resolves to the beam its uplink occasion
/// is associated with, rather than being discarded, when k_mac shifts the reported capture slot.
TEST_F(ra_scheduler_ntn_kmac_multi_ssb_test, preamble_resolves_to_the_ssb_of_its_uplink_occasion)
{
  const prach_helper::ssb_to_ro_mapping ref_map = make_reference_mapping();
  ASSERT_GT(cell_cfg.params.ssb_cfg.ssb_beams.nof_transmitted(), 1U)
      << "The association table is only built with more than one active SS/PBCH block";

  // Advance to an uplink occasion the association actually maps: the occasions left over after the last complete
  // mapping cycle of an association period carry no SS/PBCH block index.
  std::optional<ssb_id_t> expected_ssb;
  ASSERT_TRUE(run_slot_until([this, &ref_map, &expected_ssb]() {
    const slot_point ul_occasion_slot = next_slot_rx();
    if (not ref_map.occasions().td_slot_mapping().has_prach_occasion(ul_occasion_slot)) {
      return false;
    }
    expected_ssb = ref_map.get_ssb_index(ul_occasion_slot, TD_OCCASION, FREQ_IDX, PREAMBLE_ID);
    return expected_ssb.has_value();
  })) << "No uplink occasion associated with an SS/PBCH block was reached";

  // The UE transmits in uplink occasion slot p; the gNB captures it on its downlink clock at p + k_mac and reports
  // that slot in the RACH indication.
  const slot_point        ul_occasion_slot = next_slot_rx();
  rach_indication_message ind              = test_helper::create_rach_indication(
      cell_cfg, ul_occasion_slot, {test_helper::create_preamble(PREAMBLE_ID, to_rnti(0x4601))});
  ind.slot_rx = cell_cfg.ue_ul_to_dl_slot(ul_occasion_slot);

  ra_sch.handle_rach_indication(ind);
  // The result tracker matches the RAR this eventually produces against its pending preambles, and the fixture
  // teardown runs the slots that produce it.
  tracker.on_new_rach_ind(ind);

  ASSERT_EQ(ra_ue_repo.size(), 1U) << "Preamble was discarded: its occasion resolved to no SS/PBCH block, expected "
                                   << static_cast<unsigned>(expected_ssb->value());
  ASSERT_EQ(ra_ue_repo.begin()->ssb_index, *expected_ssb)
      << "Preamble resolved to the SS/PBCH block of the capture slot instead of that of its uplink occasion";
}

} // namespace
