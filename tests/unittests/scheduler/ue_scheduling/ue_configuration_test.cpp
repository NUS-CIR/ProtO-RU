// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/scheduler/support/pdcch_aggregation_level_calculator.h"
#include "lib/scheduler/ue_context/ue_cell_repository.h"
#include "lib/scheduler/ue_context/ue_repository.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "ocudu/scheduler/config/logical_channel_config_factory.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include <gtest/gtest.h>

using namespace ocudu;

class ue_configuration_test : public ::testing::Test
{
protected:
  scheduler_expert_config                  sched_cfg = config_helpers::make_default_scheduler_expert_config();
  sched_cell_configuration_request_message msg = sched_config_helper::make_default_sched_cell_configuration_request();
  sched_ue_creation_request_message        ue_create_msg =
      sched_config_helper::create_default_sched_ue_creation_request(msg.ran);

  const cell_configuration& add_cell()
  {
    cell_configuration& cell_cfg = cfg_pool.add_cell(sched_cfg, msg);
    cell_cfg_db.emplace(msg.cell_index, &cell_cfg);
    return cell_cfg;
  }

  du_cell_group_config_pool                      cfg_pool;
  cell_common_configuration_list                 cell_cfg_db;
  std::vector<std::unique_ptr<ue_configuration>> ue_cfg_pool;
  std::optional<ue_cell_repository>              cell_ues;
  ue_repository                                  ue_db{sched_cfg.ue};

  ue& add_ue(const sched_ue_creation_request_message& ue_req)
  {
    ue_cfg_pool.push_back(
        std::make_unique<ue_configuration>(ue_req.ue_index, ue_req.crnti, cell_cfg_db, cfg_pool.add_ue(ue_req)));
    ue_db.add_ue(*ue_cfg_pool.back(), {sched_config_helper::to_ue_creation_mode(ue_req), ue_req.ul_ccch_slot_rx});
    return ue_db[ue_req.ue_index];
  }
};

TEST_F(ue_configuration_test, configuration_valid_on_creation)
{
  const cell_configuration& cell_cfg = add_cell();
  ue_cell_configuration ue_cfg{to_rnti(0x4601), cell_cfg, cfg_pool.add_ue(ue_create_msg).cells[cell_cfg.cell_index]};

  // Test Common Config.
  ASSERT_TRUE(ue_cfg.find_bwp(to_bwp_id(0)) != nullptr);
  ASSERT_TRUE(ue_cfg.bwp(to_bwp_id(0)).dl.cfg() == cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params);
  ASSERT_TRUE(ue_cfg.coreset(to_coreset_id(0)).id() ==
              cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0->get_id());
  ASSERT_EQ(0, fmt::underlying(ue_cfg.search_space(to_search_space_id(0)).cfg->get_id()));
  ASSERT_TRUE(*ue_cfg.search_space(to_search_space_id(0)).cfg ==
              cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[0]);
  ASSERT_EQ(1, fmt::underlying(ue_cfg.search_space(to_search_space_id(1)).cfg->get_id()));
  ASSERT_TRUE(*ue_cfg.search_space(to_search_space_id(1)).cfg ==
              cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[1]);

  // Test Dedicated Config.
  ASSERT_TRUE(ue_cfg.find_coreset(to_coreset_id(2)) == nullptr);
  ASSERT_EQ(2, fmt::underlying(ue_cfg.search_space(to_search_space_id(2)).cfg->get_id()));
  ASSERT_TRUE(*ue_cfg.search_space(to_search_space_id(2)).cfg ==
              (*ue_create_msg.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg->search_spaces[0]);
  ASSERT_TRUE(ue_cfg.find_search_space(to_search_space_id(3)) == nullptr);
}

TEST_F(ue_configuration_test, configuration_valid_on_reconfiguration)
{
  const cell_configuration& cell_cfg = cfg_pool.add_cell(sched_cfg, msg);
  ue_cell_configuration ue_cfg{to_rnti(0x4601), cell_cfg, cfg_pool.add_ue(ue_create_msg).cells[to_du_cell_index(0)]};
  ASSERT_EQ(ue_cfg.init_bwp().dl.pdsch().ded()->mcs_table, pdsch_mcs_table::qam256);

  sched_ue_reconfiguration_message recfg_req;
  recfg_req.ue_index = ue_create_msg.ue_index;
  recfg_req.crnti    = ue_create_msg.crnti;
  recfg_req.cfg.cells.emplace();
  recfg_req.cfg.cells.value().push_back(ue_create_msg.cfg.cells->at(0));
  recfg_req.cfg.cells.value()[0].ded->bwps        = ue_create_msg.cfg.cells->at(0).ded->bwps;
  serving_cell_config& ue_cell_reconf             = recfg_req.cfg.cells.value()[0].ded->serv_cell_cfg;
  ue_cell_reconf.init_dl_bwp.pdsch_cfg->mcs_table = pdsch_mcs_table::qam64;
  ue_cfg.reconfigure(cfg_pool.reconf_ue(recfg_req).cells[to_du_cell_index(0)]);

  ASSERT_EQ(ue_cfg.init_bwp().dl.pdsch().ded()->mcs_table, pdsch_mcs_table::qam64);
}

TEST_F(ue_configuration_test, when_td_alloc_list_r16_is_configured_then_dci_tdra_field_size_follows_it)
{
  const cell_configuration& cell_cfg = add_cell();
  ue_cell_configuration ue_cfg{to_rnti(0x4601), cell_cfg, cfg_pool.add_ue(ue_create_msg).cells[cell_cfg.cell_index]};

  // Without Rel-16 TDRA lists, the DCI 1_1/0_1 TDRA field size follows the applied legacy/common lists.
  {
    const search_space_info& ss = ue_cfg.search_space(to_search_space_id(2));
    ASSERT_TRUE(ss.dci_sz.format1_1_ue_size.has_value());
    ASSERT_EQ(ss.dci_sz.format1_1_ue_size->time_resource,
              units::bits(log2_ceil(ss.bwp->dl.td_mapper().dedicated_pdsch_td_resources().size())));
    ASSERT_TRUE(ss.dci_sz.format0_1_ue_size.has_value());
    ASSERT_EQ(ss.dci_sz.format0_1_ue_size->time_resource,
              units::bits(log2_ceil(ss.bwp->ul.td_mapper().dedicated_pusch_td_resources().size())));
  }

  // Reconfigure the UE with Rel-16 TDRA lists that mirror the applied lists and append repetition entries.
  sched_ue_reconfiguration_message recfg_req;
  recfg_req.ue_index = ue_create_msg.ue_index;
  recfg_req.crnti    = ue_create_msg.crnti;
  recfg_req.cfg.cells.emplace();
  recfg_req.cfg.cells.value().push_back(ue_create_msg.cfg.cells->at(0));
  recfg_req.cfg.cells.value()[0].ded->bwps = ue_create_msg.cfg.cells->at(0).ded->bwps;
  serving_cell_config& serv_cell           = recfg_req.cfg.cells.value()[0].ded->serv_cell_cfg;

  auto&                        pdsch_cfg = serv_cell.init_dl_bwp.pdsch_cfg.value();
  const dl_time_domain_mapper& dl_mapper = ue_cfg.search_space(to_search_space_id(2)).bwp->dl.td_mapper();
  for (const auto& alloc : dl_mapper.dedicated_pdsch_td_resources()) {
    pdsch_cfg.pdsch_td_alloc_list.push_back(alloc);
  }
  for (uint8_t rep : {2, 4, 8}) {
    pdsch_time_domain_resource_allocation rep_alloc = pdsch_cfg.pdsch_td_alloc_list.front();
    rep_alloc.rep_number                            = rep;
    pdsch_cfg.pdsch_td_alloc_list.push_back(rep_alloc);
  }
  auto& pusch_cfg = serv_cell.ul_config->init_ul_bwp.pusch_cfg.value();
  for (const auto& alloc :
       ue_cfg.search_space(to_search_space_id(2)).bwp->ul.td_mapper().dedicated_pusch_td_resources()) {
    pusch_cfg.pusch_td_alloc_list.push_back(alloc);
  }
  for (uint8_t rep : {2, 4, 8}) {
    pusch_time_domain_resource_allocation rep_alloc = pusch_cfg.pusch_td_alloc_list.front();
    rep_alloc.nof_repetitions                       = rep;
    pusch_cfg.pusch_td_alloc_list.push_back(rep_alloc);
  }
  ue_cfg.reconfigure(cfg_pool.reconf_ue(recfg_req).cells[to_du_cell_index(0)]);

  // The TDRA field size of the non-fallback DCI formats now follows the Rel-16 lists, which the scheduler selects rows
  // from and signals directly in the DCI.
  {
    const search_space_info& ss = ue_cfg.search_space(to_search_space_id(2));
    ASSERT_TRUE(ss.dci_sz.format1_1_ue_size.has_value());
    ASSERT_EQ(ss.dci_sz.format1_1_ue_size->time_resource, units::bits(log2_ceil(pdsch_cfg.pdsch_td_alloc_list.size())));
    ASSERT_TRUE(ss.dci_sz.format0_1_ue_size.has_value());
    ASSERT_EQ(ss.dci_sz.format0_1_ue_size->time_resource, units::bits(log2_ceil(pusch_cfg.pusch_td_alloc_list.size())));
  }
}

TEST_F(ue_configuration_test, when_resource_allocation_type_0_is_configured_then_dci_frequency_field_size_is_nof_rbgs)
{
  serving_cell_config& serv_cell                        = ue_create_msg.cfg.cells->at(0).ded->serv_cell_cfg;
  serv_cell.init_dl_bwp.pdsch_cfg->res_alloc            = pdsch_config::resource_allocation::resource_allocation_type_0;
  serv_cell.init_dl_bwp.pdsch_cfg->rbg_sz               = rbg_size::config1;
  serv_cell.ul_config->init_ul_bwp.pusch_cfg->res_alloc = pusch_config::resource_allocation::resource_allocation_type_0;

  const cell_configuration& cell_cfg = add_cell();
  ue_cell_configuration ue_cfg{to_rnti(0x4601), cell_cfg, cfg_pool.add_ue(ue_create_msg).cells[cell_cfg.cell_index]};

  const search_space_info& ss          = ue_cfg.search_space(to_search_space_id(2));
  const crb_interval       dl_bwp_crbs = ss.bwp->dl.cfg().crbs;
  const crb_interval       ul_bwp_crbs = ss.bwp->ul.cfg().crbs;
  // Ensure the test discriminates between the number of RBGs and the nominal RBG size.
  ASSERT_NE(get_nof_rbgs(dl_bwp_crbs, rbg_size::config1),
            to_underlying(get_nominal_rbg_size(dl_bwp_crbs.length(), rbg_size::config1)));
  ASSERT_TRUE(ss.dci_sz.format1_1_ue_size.has_value());
  ASSERT_EQ(ss.dci_sz.format1_1_ue_size->frequency_resource, units::bits(get_nof_rbgs(dl_bwp_crbs, rbg_size::config1)));
  ASSERT_TRUE(ss.dci_sz.format0_1_ue_size.has_value());
  ASSERT_EQ(ss.dci_sz.format0_1_ue_size->frequency_resource, units::bits(get_nof_rbgs(ul_bwp_crbs, rbg_size::config1)));
}

TEST_F(ue_configuration_test, when_reconfiguration_is_received_then_ue_updates_logical_channel_states)
{
  // Test Preamble.
  const cell_configuration& cell_cfg = add_cell();
  cell_ues.emplace(cell_cfg, nullptr);
  ue_db.register_cell(*cell_ues);
  auto& u = add_ue(ue_create_msg);

  // Pass Reconfiguration to UE with an new Logical Channel.
  sched_ue_reconfiguration_message recfg{};
  recfg.ue_index = ue_create_msg.ue_index;
  recfg.crnti    = ue_create_msg.crnti;
  recfg.cfg      = ue_create_msg.cfg;
  recfg.cfg.lc_config_list->push_back(config_helpers::create_default_logical_channel_config(uint_to_lcid(4)));
  ue_configuration ue_ded_cfg2{*u.ue_cfg_dedicated()};
  ue_ded_cfg2.update(cell_cfg_db, cfg_pool.reconf_ue(recfg));
  ue_db.reconfigure_ue(ue_ded_cfg2, sched_ue_config_request::causes::other_rrc_proc);

  // Confirm that the UE is in fallback.
  ASSERT_TRUE(u.get_pcell().is_in_fallback_mode());
  ASSERT_TRUE(u.get_pcell().get_pcell_state().config_st == ue_config_state::pending_reconf);

  // While in fallback, DL buffer status that are not for SRB0/SRB1, do not get represented in pending bytes.
  ASSERT_FALSE(u.logical_channels().has_dl_pending_bytes());
  for (const auto& lc : *recfg.cfg.lc_config_list) {
    u.handle_dl_buffer_state_indication(lc.lcid, 10);
    if (lc.lcid <= LCID_SRB1) {
      ASSERT_TRUE(u.logical_channels().has_dl_pending_bytes());
    } else {
      ASSERT_FALSE(u.logical_channels().has_dl_pending_bytes());
    }
    u.handle_dl_buffer_state_indication(lc.lcid, 0);
  }

  // Confirm that UE config applied config.
  ue_db.ue_config_applied(ue_create_msg.ue_index);

  // Verify that DL buffer state indications affect newly active logical channels.
  for (const auto& lc : *recfg.cfg.lc_config_list) {
    if (lc.lcid == uint_to_lcid(0)) {
      // LCID0 is a special case.
      continue;
    }
    u.handle_dl_buffer_state_indication(lc.lcid, 10);
    ASSERT_TRUE(u.logical_channels().has_dl_pending_bytes());
    u.handle_dl_buffer_state_indication(lc.lcid, 0);
    ASSERT_FALSE(u.logical_channels().has_dl_pending_bytes());
  }

  // Verify that inactive logical channels do not affect pending bytes.
  u.handle_dl_buffer_state_indication(uint_to_lcid(6), 10);
  ASSERT_FALSE(u.logical_channels().has_dl_pending_bytes());
}

class ue_ul_meas_gap_test : public ue_configuration_test
{
protected:
  ue_ul_meas_gap_test() { OCUDU_TEST_REQUIREMENTS("DU-GEN-9"); }

  /// 6ms gap every 80ms, starting at subframe 10. At 15kHz there is one slot per subframe.
  static constexpr meas_gap_config test_gap{10, meas_gap_length::ms6, meas_gap_repetition_period::ms80};

  /// Builds a slot whose position within the gap period is \c phase subframes after the gap offset.
  static slot_point slot_at_gap_phase(unsigned phase)
  {
    const unsigned slot_idx = 70 * static_cast<unsigned>(test_gap.mgrp) + test_gap.offset + phase;
    return slot_point{subcarrier_spacing::kHz15, slot_idx / 10, slot_idx % 10};
  }

  /// Absence of a UE Timing Advance Report, which \c ue_ta_report_tracker tracks outside the configuration.
  static constexpr std::optional<std::chrono::microseconds> no_report{};

  /// Creates a UE cell configuration with the measurement gap above, on a cell with the given reference location T_TA.
  ue_cell_configuration make_ue_cfg(std::optional<std::chrono::microseconds> cell_ul_ta)
  {
    cell_configuration& cell_cfg = cfg_pool.add_cell(sched_cfg, msg);
    cell_cfg_db.emplace(msg.cell_index, &cell_cfg);
    cell_cfg.ntn_ref_location_ul_ta = cell_ul_ta;
    return ue_cell_configuration{
        to_rnti(0x4601), cell_cfg, cfg_pool.add_ue(ue_create_msg).cells[cell_cfg.cell_index], test_gap};
  }
};

TEST_F(ue_ul_meas_gap_test, when_no_report_was_received_then_the_cell_estimate_places_the_window)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-MOB-1");

  // T_TA of 14 slots, as in an NTN cell with a feeder link. The window sits at the gap offset plus T_TA, guarded by one
  // slot on each side, so at phases 13..21.
  ue_cell_configuration ue_cfg = make_ue_cfg(std::chrono::microseconds{14600});

  ASSERT_TRUE(ue_cfg.is_ul_enabled(slot_at_gap_phase(0), no_report))
      << "the gap offset itself is usable, the UE transmits early";
  ASSERT_FALSE(ue_cfg.is_ul_enabled(slot_at_gap_phase(14), no_report));
  ASSERT_TRUE(ue_cfg.is_ul_enabled(slot_at_gap_phase(30), no_report));
}

TEST_F(ue_ul_meas_gap_test, when_the_cell_has_an_estimate_then_a_report_does_not_displace_it)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-MOB-1");

  // The cell estimate is recomputed as the satellite moves, while a UE report only refreshes while tar-Config is
  // configured. Letting a report win would freeze the window at the value last reported, which ages at tens of
  // microseconds per second.
  //
  // A T_TA of 14 slots blocks phases 13..21, one of 20 slots blocks 19..27. The phases that tell the two apart are
  // those only one of them blocks: 14 for the estimate, 25 for the report.
  ue_cell_configuration ue_cfg = make_ue_cfg(std::chrono::microseconds{14600});
  ASSERT_FALSE(ue_cfg.is_ul_enabled(slot_at_gap_phase(14), no_report));
  ASSERT_TRUE(ue_cfg.is_ul_enabled(slot_at_gap_phase(25), no_report));

  const std::optional<std::chrono::microseconds> report{20000};
  ASSERT_FALSE(ue_cfg.is_ul_enabled(slot_at_gap_phase(14), report)) << "the window must stay on the cell estimate";
  ASSERT_TRUE(ue_cfg.is_ul_enabled(slot_at_gap_phase(25), report)) << "the report must not displace the cell estimate";
}

TEST_F(ue_ul_meas_gap_test, when_the_cell_does_not_track_the_timing_advance_then_a_report_still_places_the_window)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-MOB-1");

  // A cell without reference_location produces no estimate, so the window would stay unshifted at phases 0..6.
  ue_cell_configuration ue_cfg = make_ue_cfg(std::nullopt);
  ASSERT_FALSE(ue_cfg.is_ul_enabled(slot_at_gap_phase(0), no_report));
  ASSERT_TRUE(ue_cfg.is_ul_enabled(slot_at_gap_phase(14), no_report));

  // The UE report is then the only source of T_TA, and it is enough to place the window correctly.
  const std::optional<std::chrono::microseconds> report{14600};
  ASSERT_TRUE(ue_cfg.is_ul_enabled(slot_at_gap_phase(0), report));
  ASSERT_FALSE(ue_cfg.is_ul_enabled(slot_at_gap_phase(14), report));
}

TEST_F(ue_ul_meas_gap_test, when_the_cell_configures_k_mac_then_the_window_follows_the_gnb_clock)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-MOB-1");

  // The slot tested is the one the grant occupies on the gNB downlink clock, which the UE transmission reaches k_mac
  // slots after the UE sends it, while the gap stays anchored to the UE downlink timing. A T_TA of 14 slots on its own
  // blocks the phases 13..21, and a k_mac of 20 slots carries that window to the phases 33..41. The cell configures no
  // Koffset, which the UE applies itself and which therefore leaves the window where it is.
  //
  // k_mac displaces the window modulo the gap period, so a larger value does not make the test stricter: the two
  // windows only stay apart for a k_mac of 9..71 slots, and a multiple of the 80 slot period puts the window back
  // where a cell without k_mac has it.
  msg.ran.ntn_params.emplace();
  msg.ran.ntn_params->ntn_cfg.k_mac = std::chrono::milliseconds{20};
  ue_cell_configuration ue_cfg      = make_ue_cfg(std::chrono::microseconds{14600});
  ASSERT_EQ(ue_cfg.cell_cfg_common.ntn_k_mac_slots, 20U) << "the phases below assume a k_mac of 20 slots";

  ASSERT_TRUE(ue_cfg.is_ul_enabled(slot_at_gap_phase(14), no_report))
      << "the window must not stay where T_TA alone puts it";
  ASSERT_FALSE(ue_cfg.is_ul_enabled(slot_at_gap_phase(34), no_report));
  ASSERT_TRUE(ue_cfg.is_ul_enabled(slot_at_gap_phase(0), no_report))
      << "the window must not travel towards the gNB clock instead";
}

TEST_F(ue_configuration_test, search_spaces_pdcch_candidate_lists_does_not_surpass_limit)
{
  cell_config_builder_params params{};
  params.scs_common                      = subcarrier_spacing::kHz30;
  params.dl_carrier.arfcn_f_ref          = 520002;
  params.dl_carrier.band                 = nr_band::n41;
  params.dl_carrier.carrier_bw           = bs_channel_bandwidth::MHz50;
  msg                                    = sched_config_helper::make_default_sched_cell_configuration_request(params);
  auto&                        pdcch_cfg = *msg.ran.init_bwp.pdcch_cfg;
  const coreset_configuration& cset_cfg  = pdcch_cfg.coresets[0];
  search_space_configuration&  ss_cfg    = pdcch_cfg.search_spaces[0];
  ss_cfg.set_non_ss0_nof_candidates({config_helpers::compute_max_nof_candidates(aggregation_level::n1, cset_cfg),
                                     config_helpers::compute_max_nof_candidates(aggregation_level::n2, cset_cfg),
                                     config_helpers::compute_max_nof_candidates(aggregation_level::n4, cset_cfg),
                                     config_helpers::compute_max_nof_candidates(aggregation_level::n8, cset_cfg),
                                     config_helpers::compute_max_nof_candidates(aggregation_level::n16, cset_cfg)});
  ue_create_msg = sched_config_helper::create_default_sched_ue_creation_request(msg.ran);

  const cell_configuration& cell_cfg = add_cell();
  rnti_t                    crnti =
      to_rnti(test_rng::uniform_int<uint16_t>(to_underlying(rnti_t::MIN_CRNTI), to_underlying(rnti_t::MAX_CRNTI)));
  ue_cell_configuration ue_cfg{crnti, cell_cfg, cfg_pool.add_ue(ue_create_msg).cells[cell_cfg.cell_index]};

  const sched_bwp_config& bwp            = ue_cfg.bwp(to_bwp_id(0));
  const unsigned          max_candidates = max_nof_monitored_pdcch_candidates(bwp.dl.cfg().scs);

  unsigned       sfn = test_rng::uniform_int<unsigned>(0, 1023);
  const unsigned slots_to_test =
      msg.ran.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces[0].get_monitoring_slot_periodicity();
  slot_point start_slot{params.scs_common, sfn, 0}, end_slot = start_slot + slots_to_test;
  for (slot_point pdcch_slot = start_slot; pdcch_slot != end_slot; ++pdcch_slot) {
    unsigned pdcch_candidates_count = 0;
    for (unsigned l = 0; l != NOF_AGGREGATION_LEVELS; ++l) {
      const aggregation_level aggr_lvl = aggregation_index_to_level(l);

      for (const sched_search_space_config* ss : bwp.dl.pdcch().search_spaces()) {
        ASSERT_GE(ss->cfg().get_nof_candidates()[l],
                  ue_cfg.search_space(ss->id()).get_pdcch_candidates(aggr_lvl, pdcch_slot).size())
            << "The generated candidates cannot exceed the number of candidates passed in the SearchSpace config";

        pdcch_candidates_count += ue_cfg.search_space(ss->id()).get_pdcch_candidates(aggr_lvl, pdcch_slot).size();
      }
    }

    // The number of PDCCH candidates in each SearchSpace must not exceed the max number of PDCCH candidates for the
    // given numerology as per TS 38.213 Table 10.1-2.
    ASSERT_LE(pdcch_candidates_count, max_candidates);
  }
}

TEST_F(ue_configuration_test, when_ue_pdsch_does_not_use_qam256_then_aggregation_level_follows_the_cqi_table_1)
{
  // The cell MCS table is qam256, which maps to the CQI table 2.
  (*ue_create_msg.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdsch_cfg->mcs_table = pdsch_mcs_table::qam64;

  const cell_configuration& cell_cfg = add_cell();
  cell_ues.emplace(cell_cfg, nullptr);
  ue_db.register_cell(*cell_ues);
  ue_cell& ue_cc = add_ue(ue_create_msg).get_pcell();

  const search_space_info& ss       = ue_cc.cfg().search_space(to_search_space_id(2));
  const unsigned           dci_size = ss.dci_sz.format1_1_ue_size->total.value();
  const float              cqi      = 3;
  const aggregation_level  al_table1 =
      map_cqi_to_aggregation_level(cqi, cqi_table_t::table1, ss.cfg->get_nof_candidates(), dci_size);
  const aggregation_level al_table2 =
      map_cqi_to_aggregation_level(cqi, cqi_table_t::table2, ss.cfg->get_nof_candidates(), dci_size);
  ASSERT_NE(al_table1, al_table2) << "This test cannot tell the CQI tables apart";

  ASSERT_EQ(ue_cc.get_aggregation_level(cqi, ss, true), al_table1);
}
