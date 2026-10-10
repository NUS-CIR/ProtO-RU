// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "../test_utils/dummy_test_components.h"
#include "lib/scheduler/config/sched_config_manager.h"
#include "lib/scheduler/logging/cell_metrics_handler.h"
#include "lib/scheduler/logging/scheduler_result_logger.h"
#include "lib/scheduler/pdcch_scheduling/pdcch_resource_allocator_impl.h"
#include "lib/scheduler/pucch_scheduling/pucch_allocator_impl.h"
#include "lib/scheduler/slicing/ran_slice_instance.h"
#include "lib/scheduler/srs/srs_allocator_impl.h"
#include "lib/scheduler/uci_scheduling/uci_allocator_impl.h"
#include "lib/scheduler/ue_context/ue.h"
#include "lib/scheduler/ue_context/ue_cell_repository.h"
#include "lib/scheduler/ue_scheduling/ue_cell_grid_allocator.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/scheduler/cell_config_builder_profiles.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "tests/test_doubles/scheduler/scheduler_result_finder.h"
#include "tests/test_doubles/scheduler/scheduler_test_message_validators.h"
#include "ocudu/adt/unique_function.h"
#include "ocudu/ran/csi_rs/csi_rs_pattern.h"
#include "ocudu/ran/csi_rs/frequency_allocation_type.h"
#include "ocudu/ran/dmrs/dmrs.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/duplex_mode.h"
#include "ocudu/ran/pdcch/search_space.h"
#include "ocudu/ran/sch/sch_segmentation.h"
#include "ocudu/scheduler/config/logical_channel_config_factory.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include "ocudu/scheduler/support/rb_helper.h"
#include <gtest/gtest.h>

using namespace ocudu;

struct test_params {
  scheduler_expert_config sched_cfg;
  std::variant<search_space_configuration::common_dci_format, search_space_configuration::ue_specific_dci_format>
      ss2_dci_fmt = search_space_configuration::ue_specific_dci_format::f0_1_and_1_1;
  /// Cell-specific K-offset of the cell, which turns it into an NTN one when set.
  std::chrono::milliseconds ntn_cs_koffset{0};
  /// k_mac of the cell, which turns it into an NTN one when set.
  std::chrono::milliseconds ntn_k_mac{0};
};

class ue_grid_allocator_test : public ::testing::TestWithParam<duplex_mode>
{
  static sched_cell_configuration_request_message
  make_cell_config_request(const cell_config_builder_params& builder_params, const test_params& params)
  {
    sched_cell_configuration_request_message req;
    req = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    req.ran.init_bwp.pdcch_cfg->search_spaces[0].set_non_ss0_monitored_dci_formats(params.ss2_dci_fmt);
    if (params.ntn_cs_koffset.count() > 0 or params.ntn_k_mac.count() > 0) {
      req.ran.ntn_params.emplace();
      if (params.ntn_cs_koffset.count() > 0) {
        req.ran.ntn_params->ntn_cfg.cell_specific_koffset = params.ntn_cs_koffset;
      }
      if (params.ntn_k_mac.count() > 0) {
        req.ran.ntn_params->ntn_cfg.k_mac = params.ntn_k_mac;
      }
      // A HARQ process of the cell is allowed to operate in UL HARQ mode B.
      req.ran.ntn_params->ul_harq_mode_b = true;
    }
    return req;
  }

protected:
  ue_grid_allocator_test(const test_params& params) :
    sched_cfg(params.sched_cfg),
    cfg_builder_params(cell_config_builder_profiles::create(GetParam())),
    cell_cfg(*[this, &params]() {
      const auto* cfg = cfg_mng.add_cell(make_cell_config_request(cfg_builder_params, params));
      ocudu_assert(cfg != nullptr, "Cell configuration failed");
      return cfg;
    }()),
    cell_ues(cell_cfg, nullptr),
    ues(sched_cfg.ue),
    slice_ues(ran_slice_id_t{0}, to_du_cell_index(0), ues),
    alloc(expert_cfg, ues, pdcch_alloc, uci_alloc, srs_alloc, res_grid, logger),
    current_slot(cfg_builder_params.scs_common, 0)
  {
    ues.register_cell(cell_ues);
    logger.set_level(ocudulog::basic_levels::debug);
    ocudulog::init();

    // Initialize resource grid.
    slot_indication();
  }

  slot_point get_next_ul_slot(const slot_point starting_slot) const
  {
    // The cell-specific K-offset delays every UL transmission the DCI grants, as per TS 38.213, Section 4.2, and the
    // transmission reaches the gNB k_mac slots later still.
    slot_point next_slot = starting_slot + cell_cfg.ul_delay(cfg_builder_params.min_k2 + cell_cfg.ntn_cs_koffset);
    while (not cell_cfg.is_fully_ul_enabled(next_slot)) {
      ++next_slot;
    }
    return next_slot;
  }

  void slot_indication(const std::function<void()>& on_each_slot = []() {})
  {
    ++current_slot;
    logger.set_context(current_slot.sfn(), current_slot.slot_index());

    res_grid.slot_indication(current_slot);
    pdcch_alloc.slot_indication(current_slot);
    pucch_alloc.slot_indication(current_slot);
    uci_alloc.slot_indication(current_slot);
    ues.slot_indication(current_slot);

    // Prepare CRB bitmask that will be used to find available CRBs.
    const auto& init_dl_bwp = cell_cfg.params.dl_cfg_common.init_dl_bwp;
    const auto  prb_lims    = crb_to_prb(init_dl_bwp.generic_params.crbs,
                                     cell_cfg.expert_cfg.ue.pdsch_crb_limits & init_dl_bwp.generic_params.crbs);
    auto        used_prbs   = res_grid[0].dl_res_grid.used_prbs(init_dl_bwp.generic_params.scs,
                                                       init_dl_bwp.generic_params.crbs,
                                                       init_dl_bwp.pdsch_common.pdsch_td_alloc_list[0].symbols);
    if (not prb_lims.empty()) {
      used_prbs.fill(0, prb_lims.start());
      used_prbs.fill(prb_lims.stop(), used_prbs.size());
    }
    // Note: VRB-to-PRB interleaving is not supported in this test.
    used_dl_vrbs = used_prbs.convert_to<vrb_bitmap>();

    on_each_slot();

    alloc.post_process_results();

    // Log scheduler results.
    res_logger.on_scheduler_result(res_grid[0].result_reader());
  }

  bool run_until(const std::function<void()>& to_run, unique_function<bool()> until, unsigned max_slot_count = 1000)
  {
    if (until()) {
      return true;
    }
    for (unsigned count = 0; count != max_slot_count; ++count) {
      slot_indication(to_run);
      if (until()) {
        return true;
      }
    }
    return false;
  }

  ue& add_ue(du_ue_index_t ue_index, const std::initializer_list<lcid_t>& lcids_to_activate)
  {
    sched_ue_creation_request_message ue_creation_req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    ue_creation_req.ue_index = ue_index;
    ue_creation_req.crnti    = to_rnti(0x4601 + (unsigned)ue_index);
    for (lcid_t lcid : lcids_to_activate) {
      ue_creation_req.cfg.lc_config_list->push_back(config_helpers::create_default_logical_channel_config(lcid));
    }
    // Set same dedicated PDCCH config.
    (*ue_creation_req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg =
        cell_cfg.bwp_res[to_bwp_id(0)].dl().ded_pdcchs[0];

    return add_ue(ue_creation_req);
  }

  ue& add_ue(const sched_ue_creation_request_message& ue_creation_req)
  {
    auto ev = cfg_mng.add_ue(ue_creation_req);
    report_fatal_error_if_not(ev.valid(), "UE creation failed");
    ues.add_ue(ev.next_config(),
               {sched_config_helper::to_ue_creation_mode(ue_creation_req), ue_creation_req.ul_ccch_slot_rx});
    for (const auto& lc_cfg : *ue_creation_req.cfg.lc_config_list) {
      slice_ues.add_logical_channel(ues[ue_creation_req.ue_index], lc_cfg.lcid, lc_cfg.lc_group);
    }
    ev.notify_completion();
    return ues[ue_creation_req.ue_index];
  }

  void push_dl_bs(du_ue_index_t ue_index, lcid_t lcid, unsigned bytes)
  {
    ues[ue_index].handle_dl_buffer_state_indication(lcid, bytes);
  }

  void allocate_dl_newtx_grant(const slice_ue&         user,
                               units::bytes            pending_bytes,
                               bool                    interleaving_enabled,
                               std::optional<unsigned> max_nof_rbs = std::nullopt)
  {
    auto result = alloc.allocate_dl_grant(ue_newtx_dl_grant_request{
        user, current_slot, pending_bytes, interleaving_enabled, user.select_dl_transport_block().restrictions});
    if (not result.has_value()) {
      return;
    }
    auto& builder = result.value();

    const vrb_alloc vrbs = builder.recommended_vrbs(used_dl_vrbs, MAX_NOF_PRBS, MAX_NOF_PRBS, interleaving_enabled);
    builder.set_pdsch_params(vrbs, interleaving_enabled);
    mark_used_dl_vrbs(vrbs);
  }

  void allocate_dl_retx_grant(const slice_ue& user, dl_harq_process_handle h_dl)
  {
    auto result = alloc.allocate_dl_grant(ue_retx_dl_grant_request{user, current_slot, h_dl, used_dl_vrbs});
    if (result.has_value()) {
      mark_used_dl_vrbs(result.value().vrbs);
    }
  }

  void mark_used_dl_vrbs(const vrb_alloc& vrbs)
  {
    if (vrbs.is_type0()) {
      used_dl_vrbs |= vrbs.type0();
    } else {
      used_dl_vrbs |= vrbs.type1();
    }
  }

  alloc_status allocate_ul_newtx_grant(const slice_ue&         user,
                                       units::bytes            pending_bytes,
                                       std::optional<unsigned> max_nof_rbs = std::nullopt)
  {
    return allocate_ul_newtx_grant(get_next_ul_slot(current_slot), user, pending_bytes, max_nof_rbs);
  }

  alloc_status allocate_ul_newtx_grant(slot_point              pusch_slot,
                                       const slice_ue&         user,
                                       units::bytes            pending_bytes,
                                       std::optional<unsigned> max_nof_rbs = std::nullopt)
  {
    const auto& init_ul_bwp = cell_cfg.params.ul_cfg_common.init_ul_bwp;
    auto        result      = alloc.allocate_ul_grant(
        ue_newtx_ul_grant_request{.user            = user,
                                              .pusch_slot      = pusch_slot,
                                              .pending_bytes   = pending_bytes,
                                              .allowed_symbols = ofdm_symbol_range{0, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP},
                                              .restrictions    = user.select_ul_grant().restrictions});
    if (not result.has_value()) {
      return result.error();
    }
    auto& builder = result.value();

    // Note: VRB-to-PRB interleaving is not supported in this test.
    auto used_ul_vrbs = res_grid[pusch_slot]
                            .ul_res_grid
                            .used_prbs(init_ul_bwp.generic_params.scs,
                                       init_ul_bwp.generic_params.crbs,
                                       init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list[0].symbols)
                            .convert_to<vrb_bitmap>();
    vrb_interval vrbs = builder.recommended_vrbs(used_ul_vrbs, max_nof_rbs.value_or(MAX_NOF_PRBS));
    builder.set_pusch_params(vrbs);
    used_ul_vrbs.fill(vrbs.start(), vrbs.stop());
    return alloc_status::success;
  }

  scheduler_expert_config                 sched_cfg;
  scheduler_ue_expert_config              expert_cfg{sched_cfg.ue};
  sched_cfg_dummy_notifier                mac_notif;
  scheduler_ue_metrics_dummy_notifier     metrics_notif;
  scheduler_ue_metrics_dummy_configurator metrics_ue_handler;

  cell_config_builder_params cfg_builder_params;
  sched_config_manager       cfg_mng{scheduler_config{sched_cfg, mac_notif}};
  const cell_configuration&  cell_cfg;

  cell_resource_allocator res_grid{cell_cfg};

  pdcch_resource_allocator_impl pdcch_alloc{cell_cfg};
  pucch_allocator_impl pucch_alloc{cell_cfg, expert_cfg.max_pucchs_per_slot, expert_cfg.max_ul_grants_per_slot};
  uci_allocator_impl   uci_alloc{cell_cfg, pucch_alloc};
  srs_allocator_impl   srs_alloc{cell_cfg, std::nullopt};

  ocudulog::basic_logger& logger{ocudulog::fetch_basic_logger("SCHED")};
  scheduler_result_logger res_logger{false, cell_cfg.params.pci};

  ue_cell_repository      cell_ues;
  ue_repository           ues;
  slice_ue_repository     slice_ues;
  slice_rrm_policy_config rrm_policy;
  ran_slice_instance      slice_inst{ran_slice_id_t{0}, cell_cfg, rrm_policy, ues};
  ue_cell_grid_allocator  alloc;

  slot_point current_slot;
  vrb_bitmap used_dl_vrbs;
};

class ue_grid_allocator_css_test : public ue_grid_allocator_test
{
protected:
  ue_grid_allocator_css_test() :
    ue_grid_allocator_test(test_params{config_helpers::make_default_scheduler_expert_config(),
                                       search_space_configuration::common_dci_format{.f0_0_and_f1_0 = true}})
  {
  }
};

TEST_P(ue_grid_allocator_css_test,
       when_ue_dedicated_ss_is_css_then_allocation_is_within_coreset_start_crb_and_coreset0_end_crb)
{
  static const units::bytes nof_bytes_to_schedule{40U};

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.cfg.cells->front().ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg =
      cell_cfg.bwp_res[to_bwp_id(0)].dl().ded_pdcchs[0];
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  ue& u                    = add_ue(ue_creation_req);

  const auto&        cs1_cfg  = u.ue_cfg_dedicated()->pcell_cfg().coreset(to_coreset_id(1)).cfg();
  const crb_interval cs1_crbs = get_coreset_crbs(cs1_cfg);
  const crb_interval crb_lims = {
      cs1_crbs.start(),
      cs1_crbs.start() + cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0->coreset0_crbs().length()};

  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  const auto& prb_alloc =
      res_grid[0].result_reader().dl().ue_grants().back().pdsch_cfg.rbs.type1().convert_to<prb_interval>();
  const auto crb_alloc = prb_to_crb(cs1_crbs, prb_alloc);
  ASSERT_TRUE(crb_lims.contains(crb_alloc));
}

class ue_grid_allocator_default_cfg_test : public ue_grid_allocator_test
{
protected:
  ue_grid_allocator_default_cfg_test(
      const scheduler_expert_config& sched_cfg_ = config_helpers::make_default_scheduler_expert_config()) :
    ue_grid_allocator_test(test_params{sched_cfg_})
  {
  }
};

TEST_P(ue_grid_allocator_default_cfg_test, when_using_non_fallback_dci_format_use_mcs_table_set_in_pdsch_cfg)
{
  static const units::bytes nof_bytes_to_schedule{40U};

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  // Change PDSCH MCS table to be used when using non-fallback DCI format.
  (*ue_creation_req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdsch_cfg->mcs_table = ocudu::pdsch_mcs_table::qam256;
  ue_creation_req.ue_index                                                            = to_du_ue_index(0);
  ue_creation_req.crnti                                                               = to_rnti(0x4601);

  const ue& u = add_ue(ue_creation_req);

  // SearchSpace#2 uses non-fallback DCI format hence the MCS table set in dedicated PDSCH configuration must be used.
  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  ASSERT_EQ(res_grid[0].result_reader().dl().ue_grants().back().pdsch_cfg.mcs_table, ocudu::pdsch_mcs_table::qam256);
}

TEST_P(ue_grid_allocator_default_cfg_test, allocates_pdsch_restricted_to_recommended_max_nof_rbs)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  const ue& u1             = add_ue(ue_creation_req);

  static const units::bytes sched_bytes{2000U};
  const unsigned            max_nof_rbs_to_schedule = 10U;

  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u1.ue_index], sched_bytes, false, max_nof_rbs_to_schedule); },
                [&]() { return find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  // Successfully allocates PDSCH corresponding to the grant.
  ASSERT_GE(find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants())->pdsch_cfg.rbs.type1().length(),
            max_nof_rbs_to_schedule);
}

TEST_P(ue_grid_allocator_default_cfg_test, allocates_pusch_restricted_to_recommended_max_nof_rbs)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  const ue& u1             = add_ue(ue_creation_req);

  const units::bytes recommended_nof_bytes_to_schedule{2000U};
  const unsigned     max_nof_rbs_to_schedule = 10U;

  ASSERT_TRUE(run_until(
      [&]() {
        allocate_ul_newtx_grant(slice_ues[u1.ue_index], recommended_nof_bytes_to_schedule, max_nof_rbs_to_schedule);
      },
      [&]() { return find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; }));
  // Successfully allocates PUSCH corresponding to the grant.
  ASSERT_EQ(find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs())->pusch_cfg.rbs.type1().length(),
            max_nof_rbs_to_schedule);
}

// TS 38.321 clause 5.3.2.2: the UE considers a TB a new transmission only if the NDI "has been toggled compared to
// the value of the previous received transmission" for the HARQ process; otherwise it soft-combines it with the
// buffered TB. A newTx whose DCI is never sent (stage 2 found no RBs for it) must therefore not change the NDI that the
// next newTx on the same HARQ process is compared against.
TEST_P(ue_grid_allocator_default_cfg_test,
       when_dl_newtx_is_cancelled_before_its_dci_is_sent_then_next_newtx_toggles_ndi)
{
  const ue& u     = add_ue(to_du_ue_index(0), {LCID_MIN_DRB});
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();
  push_dl_bs(u.ue_index, LCID_MIN_DRB, 100000);
  const units::bytes newtx_bytes{1000};

  // A newTx that is sent: its NDI is the one the UE received for this HARQ process. ACK it to free the process.
  ASSERT_TRUE(run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], newtx_bytes, false); },
                        [&]() { return find_ue_dl_pdcch(u.crnti, res_grid[0].result_reader().dl()) != nullptr; }));
  const dci_1_1_configuration& sent_dci =
      find_ue_dl_pdcch(u.crnti, res_grid[0].result_reader().dl())->dci.as_c_rnti_f1_1();
  const unsigned sent_h_id = sent_dci.harq_process_number;
  const bool     sent_ndi  = sent_dci.tb1_new_data_indicator;
  ASSERT_TRUE(ue_cc.harqs.dl_harq(to_harq_id(sent_h_id))->dl_ack_info(mac_harq_ack_report_status::ack, std::nullopt));

  // A newTx that stage 2 cancels for lack of RBs: its PDCCH and PDSCH are removed and nothing is sent.
  bool cancelled = false;
  ASSERT_TRUE(run_until(
      [&]() {
        auto grant =
            alloc.allocate_dl_grant(ue_newtx_dl_grant_request{slice_ues[u.ue_index], current_slot, newtx_bytes, false});
        if (grant.has_value()) {
          grant.value().set_pdsch_params(vrb_interval{}, false);
          cancelled = true;
        }
      },
      [&]() { return cancelled; }));
  ASSERT_EQ(find_ue_dl_pdcch(u.crnti, res_grid[0].result_reader().dl()), nullptr);

  // The next newTx reuses the HARQ process and must toggle the NDI relative to the DCI the UE actually received.
  ASSERT_TRUE(run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], newtx_bytes, false); },
                        [&]() { return find_ue_dl_pdcch(u.crnti, res_grid[0].result_reader().dl()) != nullptr; }));
  const dci_1_1_configuration& next_dci =
      find_ue_dl_pdcch(u.crnti, res_grid[0].result_reader().dl())->dci.as_c_rnti_f1_1();
  ASSERT_EQ(next_dci.harq_process_number, sent_h_id)
      << "Expected the newTx to reuse the HARQ process of the cancelled grant";
  ASSERT_NE(next_dci.tb1_new_data_indicator, sent_ndi)
      << "The UE would take the new TB for a retransmission of the previous one";
}

// TS 38.321 clause 5.4.2.1: the UE obtains a new MAC PDU only if the NDI "has been toggled compared to the value in
// the previous transmission of this TB of this HARQ process"; otherwise it retransmits the PDU in its HARQ buffer. A
// newTx whose DCI is never sent must not change the NDI the next newTx on the same HARQ process is compared against.
TEST_P(ue_grid_allocator_default_cfg_test,
       when_ul_newtx_is_cancelled_before_its_dci_is_sent_then_next_newtx_toggles_ndi)
{
  const ue&          u     = add_ue(to_du_ue_index(0), {LCID_MIN_DRB});
  ue_cell&           ue_cc = ues[u.ue_index].get_pcell();
  const units::bytes newtx_bytes{1000};

  // A newTx that is sent: its NDI is the one the UE received for this HARQ process. Report CRC OK to free the process.
  ASSERT_TRUE(run_until([&]() { allocate_ul_newtx_grant(slice_ues[u.ue_index], newtx_bytes); },
                        [&]() { return find_ue_ul_pdcch(u.crnti, res_grid[0].result_reader().dl()) != nullptr; }));
  const dci_0_1_configuration& sent_dci =
      find_ue_ul_pdcch(u.crnti, res_grid[0].result_reader().dl())->dci.as_c_rnti_f0_1();
  const unsigned sent_h_id = sent_dci.harq_process_number;
  const bool     sent_ndi  = sent_dci.new_data_indicator;
  ASSERT_TRUE(ue_cc.harqs.ul_harq(to_harq_id(sent_h_id))->ul_crc_info(true).has_value());

  // A newTx that stage 2 cancels for lack of RBs: its PDCCH and PUSCH are removed and nothing is sent.
  bool cancelled = false;
  ASSERT_TRUE(run_until(
      [&]() {
        auto grant = alloc.allocate_ul_grant(
            ue_newtx_ul_grant_request{.user            = slice_ues[u.ue_index],
                                      .pusch_slot      = get_next_ul_slot(current_slot),
                                      .pending_bytes   = newtx_bytes,
                                      .allowed_symbols = ofdm_symbol_range{0, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP},
                                      .restrictions    = slice_ues[u.ue_index].select_ul_grant().restrictions});
        if (grant.has_value()) {
          grant.value().set_pusch_params({});
          cancelled = true;
        }
      },
      [&]() { return cancelled; }));
  ASSERT_EQ(find_ue_ul_pdcch(u.crnti, res_grid[0].result_reader().dl()), nullptr);

  // The next newTx reuses the HARQ process and must toggle the NDI relative to the DCI the UE actually received.
  ASSERT_TRUE(run_until([&]() { allocate_ul_newtx_grant(slice_ues[u.ue_index], newtx_bytes); },
                        [&]() { return find_ue_ul_pdcch(u.crnti, res_grid[0].result_reader().dl()) != nullptr; }));
  const dci_0_1_configuration& next_dci =
      find_ue_ul_pdcch(u.crnti, res_grid[0].result_reader().dl())->dci.as_c_rnti_f0_1();
  ASSERT_EQ(next_dci.harq_process_number, sent_h_id)
      << "Expected the newTx to reuse the HARQ process of the cancelled grant";
  ASSERT_NE(next_dci.new_data_indicator, sent_ndi)
      << "The UE would retransmit its previous MAC PDU instead of a new one";
}

TEST_P(ue_grid_allocator_default_cfg_test, does_not_allocate_pusch_with_all_remaining_rbs_if_its_a_sr_indication)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  ue& u1                   = add_ue(ue_creation_req);
  // Trigger a SR indication.
  u1.handle_sr_indication(current_slot);

  const crb_interval cell_crbs = {cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.start(),
                                  cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.stop()};

  ASSERT_TRUE(
      run_until([&]() { allocate_ul_newtx_grant(slice_ues[u1.ue_index], u1.pending_ul_newtx_bytes()); },
                [&]() { return find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; }));
  // Successfully allocates PUSCH corresponding to the grant.
  ASSERT_LT(find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs())->pusch_cfg.rbs.type1().length(),
            cell_crbs.length());
}

TEST_P(ue_grid_allocator_default_cfg_test, no_two_pdschs_are_allocated_in_same_slot_for_a_ue)
{
  static const units::bytes nof_bytes_to_schedule{400U};

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);

  const ue& u = add_ue(ue_creation_req);

  ASSERT_TRUE(run_until(
      [&]() {
        allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false);
        allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false);
      },
      [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));

  // Only one PDSCH per slot per UE.
  ASSERT_EQ(res_grid[0].result_reader().dl().ue_grants().size(), 1);
}

TEST_P(ue_grid_allocator_default_cfg_test, no_two_puschs_are_allocated_in_same_slot_for_a_ue)
{
  static const units::bytes nof_bytes_to_schedule{400U};

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);

  const ue& u = add_ue(ue_creation_req);

  ASSERT_TRUE(run_until(
      [&]() {
        allocate_ul_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule);
        allocate_ul_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule);
      },
      [&]() { return find_ue_pusch(u.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; }));

  // Only one PUSCH per slot per UE.
  ASSERT_EQ(res_grid[0].result_reader().ul().puschs().size(), 1);
}

TEST_P(ue_grid_allocator_default_cfg_test, consecutive_puschs_for_a_ue_are_allocated_in_increasing_order_of_time)
{
  static const units::bytes nof_bytes_to_schedule{400U};

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);

  const ue& u = add_ue(ue_creation_req);

  // First PUSCH grant for the UE.
  slot_point pusch_slot;
  ASSERT_TRUE(run_until(
      [&]() {
        pusch_slot = get_next_ul_slot(current_slot);
        allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], nof_bytes_to_schedule);
      },
      [&]() { return find_ue_pusch(u.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; }));

  // Second PUSCH grant for the UE trying to allocate PUSCH in a slot previous to grant1.
  alloc_status result = alloc_status::invalid_params;
  ASSERT_FALSE(run_until(
      [&]() { result = allocate_ul_newtx_grant(pusch_slot - 1, slice_ues[u.ue_index], nof_bytes_to_schedule); },
      [&]() { return result == alloc_status::success; },
      1));
}

TEST_P(ue_grid_allocator_default_cfg_test, consecutive_pdschs_for_a_ue_are_allocated_in_increasing_order_of_time)
{
  static const units::bytes nof_bytes_to_schedule{400U};

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);

  const ue& u = add_ue(ue_creation_req);

  // First PDSCH grant for the UE.
  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  slot_point last_pdsch_slot = current_slot;

  // Second PDSCH grant in the same slot for the UE.
  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  ASSERT_GE(current_slot, last_pdsch_slot);
}

TEST_P(ue_grid_allocator_default_cfg_test,
       ack_slot_of_consecutive_pdschs_for_a_ue_must_be_greater_than_or_equal_to_last_ack_slot_allocated)
{
  static const units::bytes nof_bytes_to_schedule{400U};

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);

  const ue& u = add_ue(ue_creation_req);

  // First PDSCH grant for the UE.
  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  slot_point last_pdsch_ack_slot =
      current_slot + find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants())->context.k1;

  // Second PDSCH grant in the same slot for the UE.
  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  ASSERT_GE(current_slot + find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants())->context.k1,
            last_pdsch_ack_slot);
}

TEST_P(ue_grid_allocator_default_cfg_test,
       successfully_allocated_pdsch_even_with_large_gap_to_last_pdsch_slot_allocated)
{
  static const units::bytes nof_bytes_to_schedule{8U};
  const unsigned            nof_slot_until_pdsch_is_allocated_threshold = SCHEDULER_MAX_K0;

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);

  const ue& u = add_ue(ue_creation_req);

  // Ensure current slot is the middle of 1024 SFNs. i.e. current slot=511.0
  while (current_slot.sfn() != NOF_SFNS / 2) {
    slot_indication();
  }

  // First PDSCH grant for the UE.
  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));

  // Ensure next PDSCH to be allocated slot is after wrap around of 1024 SFNs (large gap to last allocated PDSCH slot)
  // and current slot value is less than last allocated PDSCH slot. e.g. next PDSCH to be allocated slot=SFN 2, slot 2
  // after wrap around of 1024 SFNs.
  for (unsigned i = 0; i < current_slot.nof_slots_per_hyper_system_frame() / 2 + current_slot.nof_slots_per_frame();
       ++i) {
    slot_indication();
  }

  // Next PDSCH grant to be allocated.
  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; },
                nof_slot_until_pdsch_is_allocated_threshold));
}

TEST_P(ue_grid_allocator_default_cfg_test, successfully_allocates_pdsch_with_gbr_lc_prioritized_over_non_gbr_lc)
{
  OCUDU_TEST_REQUIREMENTS("DU-QOS-1");

  const lcg_id_t lcg_id              = uint_to_lcg_id(2);
  const lcid_t   gbr_bearer_lcid     = uint_to_lcid(6);
  const lcid_t   non_gbr_bearer_lcid = uint_to_lcid(5);

  // Add UE.
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  ue& u1                   = add_ue(ue_creation_req);

  // Reconfigure UE to include non-GBR bearer and GBR bearer.
  sched_ue_reconfiguration_message reconf_msg{
      .ue_index = ue_creation_req.ue_index, .crnti = ue_creation_req.crnti, .cfg = ue_creation_req.cfg};
  sched_ue_config_request& cfg_req = reconf_msg.cfg;
  cfg_req.lc_config_list.emplace();
  cfg_req.lc_config_list->resize(4);
  (*cfg_req.lc_config_list)[0]          = config_helpers::create_default_logical_channel_config(lcid_t::LCID_SRB0);
  (*cfg_req.lc_config_list)[1]          = config_helpers::create_default_logical_channel_config(lcid_t::LCID_SRB1);
  (*cfg_req.lc_config_list)[2]          = config_helpers::create_default_logical_channel_config(non_gbr_bearer_lcid);
  (*cfg_req.lc_config_list)[2].lc_group = lcg_id;
  (*cfg_req.lc_config_list)[2].qos.emplace();
  (*cfg_req.lc_config_list)[2].qos->qos          = *get_5qi_to_qos_characteristics_mapping(uint_to_five_qi(9));
  (*cfg_req.lc_config_list)[2].qos->arp_priority = arp_prio_level_t::max();
  (*cfg_req.lc_config_list)[3] = config_helpers::create_default_logical_channel_config(gbr_bearer_lcid);
  // Put GBR bearer in a different LCG than non-GBR bearer.
  (*cfg_req.lc_config_list)[3].lc_group = uint_to_lcg_id(lcg_id - 1);
  (*cfg_req.lc_config_list)[3].qos.emplace();
  (*cfg_req.lc_config_list)[3].qos->qos          = *get_5qi_to_qos_characteristics_mapping(uint_to_five_qi(1));
  (*cfg_req.lc_config_list)[3].qos->arp_priority = arp_prio_level_t::max();
  (*cfg_req.lc_config_list)[3].qos->gbr_qos_info = gbr_qos_flow_information{128000, 128000, 128000, 128000};
  ue_config_update_event ev                      = cfg_mng.update_ue(reconf_msg);
  ues.reconfigure_ue(ev.next_config(), sched_ue_config_request::causes::other_rrc_proc);
  ues.ue_config_applied(ev.get_ue_index());

  // Add LCID to the bearers of the UE belonging to this slice.
  for (const auto& lc_cfg : *cfg_req.lc_config_list) {
    slice_ues.add_logical_channel(u1, lc_cfg.lcid, lc_cfg.lc_group);
  }

  // Push buffer state update to both bearers.
  push_dl_bs(u1.ue_index, gbr_bearer_lcid, 200);
  push_dl_bs(u1.ue_index, non_gbr_bearer_lcid, 1500);

  static const units::bytes sched_bytes{2000U};

  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u1.ue_index], sched_bytes, false); },
                [&]() { return find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));

  const auto* ue_pdsch = find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants());
  ASSERT_TRUE(not ue_pdsch->tb_list.empty());
  ASSERT_TRUE(not ue_pdsch->tb_list.back().lc_chs_to_sched.empty());
  // TB info contains GBR LC channel first and then non-GBR LC channel.
  ASSERT_EQ(ue_pdsch->tb_list.back().lc_chs_to_sched.front().lcid, gbr_bearer_lcid);
}

TEST_P(ue_grid_allocator_default_cfg_test,
       successfully_allocated_pusch_even_with_large_gap_to_last_pusch_slot_allocated)
{
  static const units::bytes nof_bytes_to_schedule{400U};
  const unsigned            nof_slot_until_pusch_is_allocated_threshold = SCHEDULER_MAX_K2;

  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);

  const ue& u = add_ue(ue_creation_req);

  // Ensure current slot is the middle of 1024 SFNs. i.e. current slot=511.0
  while (current_slot.sfn() != NOF_SFNS / 2) {
    slot_indication();
  }

  // First PUSCH grant for the UE.
  ASSERT_TRUE(
      run_until([&]() { allocate_ul_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule); },
                [&]() { return find_ue_pusch(u.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; }));

  // Ensure next PUSCH to be allocated slot is after wrap around of 1024 SFNs (large gap to last allocated PUSCH slot)
  // and current slot value is less than last allocated PUSCH slot. e.g. next PUSCH to be allocated slot=SFN 2, slot 2
  // after wrap around of 1024 SFNs.
  for (unsigned i = 0; i < current_slot.nof_slots_per_hyper_system_frame() / 2 + current_slot.nof_slots_per_frame();
       ++i) {
    slot_indication();
  }

  // Second PUSCH grant for the UE.
  ASSERT_TRUE(run_until([&]() { allocate_ul_newtx_grant(slice_ues[u.ue_index], nof_bytes_to_schedule); },
                        [&]() { return find_ue_pusch(u.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; },
                        nof_slot_until_pusch_is_allocated_threshold));
}

class ue_grid_allocator_expert_cfg_pxsch_nof_rbs_limits_tester : public ue_grid_allocator_default_cfg_test
{
public:
  ue_grid_allocator_expert_cfg_pxsch_nof_rbs_limits_tester() :
    ue_grid_allocator_default_cfg_test(([]() {
      scheduler_expert_config sched_cfg_ = config_helpers::make_default_scheduler_expert_config();
      sched_cfg_.ue.pdsch_nof_rbs        = {20, 40};
      sched_cfg_.ue.pusch_nof_rbs        = {20, 40};
      return sched_cfg_;
    }()))
  {
  }
};

TEST_P(ue_grid_allocator_expert_cfg_pxsch_nof_rbs_limits_tester,
       allocates_pdsch_with_expert_cfg_min_nof_rbs_even_if_rbs_required_to_schedule_is_low)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  const ue& u1             = add_ue(ue_creation_req);

  // Ensure the buffer status is low enough such that < 20 RBs (configured in constructor) are required to schedule.
  static const units::bytes sched_bytes{20U};
  const unsigned            max_nof_rbs_to_schedule = 10U;

  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u1.ue_index], sched_bytes, false, max_nof_rbs_to_schedule); },
                [&]() { return find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  // Successfully allocates PDSCH.
  ASSERT_EQ(find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants())->pdsch_cfg.rbs.type1().length(),
            std::max(expert_cfg.pdsch_nof_rbs.start(), max_nof_rbs_to_schedule));
}

TEST_P(ue_grid_allocator_expert_cfg_pxsch_nof_rbs_limits_tester,
       allocates_pdsch_with_expert_cfg_max_nof_rbs_even_if_rbs_required_to_schedule_is_high)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  const ue& u1             = add_ue(ue_creation_req);

  // Ensure the buffer status is high enough such that > 40 RBs (configured in constructor) are required to schedule.
  static const units::bytes sched_bytes{20000U};
  const unsigned            max_nof_rbs_to_schedule = 273U;

  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u1.ue_index], sched_bytes, false, max_nof_rbs_to_schedule); },
                [&]() { return find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  // Successfully allocates PDSCH.
  ASSERT_EQ(find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants())->pdsch_cfg.rbs.type1().length(),
            std::min(expert_cfg.pdsch_nof_rbs.stop(), max_nof_rbs_to_schedule));
}

TEST_P(ue_grid_allocator_expert_cfg_pxsch_nof_rbs_limits_tester,
       allocates_pusch_with_expert_cfg_min_nof_rbs_even_if_rbs_required_to_schedule_is_low)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  const ue& u1             = add_ue(ue_creation_req);

  // Ensure the buffer status is low enough such that < 20 RBs (configured in constructor) are required to schedule.
  const units::bytes recommended_nof_bytes_to_schedule{20U};
  const unsigned     max_nof_rbs_to_schedule = 10U;

  ASSERT_TRUE(run_until(
      [&]() {
        allocate_ul_newtx_grant(slice_ues[u1.ue_index], recommended_nof_bytes_to_schedule, max_nof_rbs_to_schedule);
      },
      [&]() { return find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; }));
  // Successfully allocates PUSCH.
  ASSERT_EQ(find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs())->pusch_cfg.rbs.type1().length(),
            std::max(expert_cfg.pdsch_nof_rbs.start(), max_nof_rbs_to_schedule));
}

TEST_P(ue_grid_allocator_expert_cfg_pxsch_nof_rbs_limits_tester,
       allocates_pusch_with_expert_cfg_max_nof_rbs_even_if_rbs_required_to_schedule_is_high)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  const ue& u1             = add_ue(ue_creation_req);

  // Ensure the buffer status is high enough such that > 40 RBs (configured in constructor) are required to schedule.
  const units::bytes recommended_nof_bytes_to_schedule{200000U};
  const unsigned     max_nof_rbs_to_schedule = 273U;

  ASSERT_TRUE(run_until(
      [&]() {
        allocate_ul_newtx_grant(slice_ues[u1.ue_index], recommended_nof_bytes_to_schedule, max_nof_rbs_to_schedule);
      },
      [&]() { return find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; }));
  // Successfully allocates PUSCH.
  ASSERT_EQ(find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs())->pusch_cfg.rbs.type1().length(),
            std::min(expert_cfg.pdsch_nof_rbs.stop(), max_nof_rbs_to_schedule));
}

class ue_grid_allocator_expert_cfg_pxsch_crb_limits_tester : public ue_grid_allocator_default_cfg_test
{
public:
  ue_grid_allocator_expert_cfg_pxsch_crb_limits_tester() :
    ue_grid_allocator_default_cfg_test(([]() {
      scheduler_expert_config sched_cfg_ = config_helpers::make_default_scheduler_expert_config();
      sched_cfg_.ue.pdsch_crb_limits     = {20, 40};
      sched_cfg_.ue.pusch_crb_limits     = {20, 40};
      return sched_cfg_;
    }()))
  {
    // Assume SS#2 is USS configured with DCI format 1_1/0_1 and is the only SS used for UE PDSCH/PUSCH scheduling.
    const prb_interval pdsch_prbs =
        crb_to_prb(cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.crbs, sched_cfg.ue.pdsch_crb_limits);
    pdsch_vrb_limits = vrb_interval{pdsch_prbs.start(), pdsch_prbs.stop()};
    pusch_vrb_limits = rb_helper::crb_to_vrb_ul_non_interleaved(
        sched_cfg.ue.pusch_crb_limits, cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.start());
  }

protected:
  vrb_interval pdsch_vrb_limits;
  vrb_interval pusch_vrb_limits;
};

TEST_P(ue_grid_allocator_expert_cfg_pxsch_crb_limits_tester, allocates_pdsch_within_expert_cfg_pdsch_rb_limits)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  const ue& u1             = add_ue(ue_creation_req);

  // Ensure the buffer status is high enough such that > 20 RBs (configured in constructor) are required to schedule.
  static const units::bytes sched_bytes{20000U};
  const unsigned            max_nof_rbs_to_schedule = 273U;

  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u1.ue_index], sched_bytes, false, max_nof_rbs_to_schedule); },
                [&]() { return find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  // Successfully allocates PDSCH within RB limits.
  ASSERT_EQ(find_ue_pdsch(u1.crnti, res_grid[0].result_reader().dl().ue_grants())->pdsch_cfg.rbs.type1(),
            pdsch_vrb_limits);
}

TEST_P(ue_grid_allocator_expert_cfg_pxsch_crb_limits_tester, allocates_pusch_within_expert_cfg_pusch_rb_limits)
{
  sched_ue_creation_request_message ue_creation_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
  ue_creation_req.ue_index = to_du_ue_index(0);
  ue_creation_req.crnti    = to_rnti(0x4601);
  const ue& u1             = add_ue(ue_creation_req);

  // Ensure the buffer status is high enough such that > 20 RBs (configured in constructor) are required to schedule.
  const units::bytes recommended_nof_bytes_to_schedule{200000U};
  const unsigned     max_nof_rbs_to_schedule = 273U;

  ASSERT_TRUE(run_until(
      [&]() {
        allocate_ul_newtx_grant(slice_ues[u1.ue_index], recommended_nof_bytes_to_schedule, max_nof_rbs_to_schedule);
      },
      [&]() { return find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs()) != nullptr; }));
  // Successfully allocates PUSCH within RB limits.
  ASSERT_EQ(find_ue_pusch(u1.crnti, res_grid[0].result_reader().ul().puschs())->pusch_cfg.rbs.type1(),
            pusch_vrb_limits);
}

class ue_grid_allocator_pdsch_repetition_test : public ue_grid_allocator_test
{
protected:
  static constexpr uint8_t nof_reps = 4;
  const lcid_t             drb_lcid = uint_to_lcid(4);

  ue_grid_allocator_pdsch_repetition_test() :
    ue_grid_allocator_test(test_params{[]() {
      auto cfg                       = config_helpers::make_default_scheduler_expert_config();
      cfg.ue.pdsch_cqi_rep_threshold = 6.0F;
      // Disable OLLA so the effective CQI equals the reported wideband CQI, making the repetition trigger
      // deterministic.
      cfg.ue.olla_cqi_inc = 0;
      return cfg;
    }()})
  {
  }

  // Adds a UE configured with a Rel-16 PDSCH TDRA list that mirrors the common list and appends a repetition entry.
  const ue& add_repetition_ue()
  {
    sched_ue_creation_request_message req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    req.ue_index = to_du_ue_index(0);
    req.crnti    = to_rnti(0x4601);
    req.cfg.lc_config_list->push_back(config_helpers::create_default_logical_channel_config(drb_lcid));
    (*req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg = cell_cfg.bwp_res[to_bwp_id(0)].dl().ded_pdcchs[0];

    auto&       pdsch_cfg   = (*req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdsch_cfg.value();
    const auto& common_list = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list;
    for (const auto& common_alloc : common_list) {
      pdsch_cfg.pdsch_td_alloc_list.push_back(common_alloc);
    }
    rep_time_resource                               = pdsch_cfg.pdsch_td_alloc_list.size();
    pdsch_time_domain_resource_allocation rep_alloc = common_list.front();
    rep_alloc.rep_number                            = nof_reps;
    pdsch_cfg.pdsch_td_alloc_list.push_back(rep_alloc);

    return add_ue(req);
  }

  void set_reported_cqi(ue_cell& ue_cc, uint8_t cqi)
  {
    csi_report_data csi{};
    csi.first_tb_wideband_cqi = csi_report_data::wideband_cqi_type{cqi};
    csi.valid                 = true;
    ue_cc.handle_csi_report(csi);
  }

  // Time-domain resource of the DL DCI (format 1_1) scheduled for the UE in the current slot, if any.
  std::optional<unsigned> current_dl_dci_time_resource(rnti_t rnti) const
  {
    const pdcch_dl_information* pdcch = find_ue_dl_pdcch(rnti, res_grid[0].result_reader().dl());
    if (pdcch == nullptr or pdcch->dci.type() != dci_dl_rnti_config_type::c_rnti_f1_1) {
      return std::nullopt;
    }
    return pdcch->dci.as_c_rnti_f1_1().time_resource;
  }

  uint8_t rep_time_resource = 0;
};

// A HARQ reTx reuses the transmission scheme of the original transmission (like the number of layers), so the number
// of PDSCH repetitions is taken from the HARQ grant parameters, not re-decided from the current link quality. Verify
// that once the CQI recovers above the threshold, a reTx of a grant started at low CQI is still scheduled as a
// repetition bundle, while a fresh newTx is a single transmission.
TEST_P(ue_grid_allocator_pdsch_repetition_test, retx_reuses_original_repetition_scheme_after_cqi_recovers)
{
  const ue& u     = add_repetition_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  auto ue_dl_grant = [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()); };

  // Low CQI: the newTx must be scheduled as a repetition bundle.
  set_reported_cqi(ue_cc, 3);
  push_dl_bs(u.ue_index, drb_lcid, 100000);
  ASSERT_TRUE(run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{1000}, false); },
                        [&]() {
                          const dl_msg_alloc* g = ue_dl_grant();
                          return g != nullptr and g->context.nof_retxs == 0;
                        }));
  ASSERT_EQ(current_dl_dci_time_resource(u.crnti).value(), rep_time_resource)
      << "newTx at low CQI was not scheduled as a repetition bundle";

  // NACK the bundle to force a reTx.
  std::optional<dl_harq_process_handle> h_dl = ue_cc.harqs.find_dl_harq_waiting_ack();
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));

  // CQI recovers above the threshold. A fresh newTx would now be a single transmission, but the reTx must keep the
  // repetition scheme of the original transmission.
  set_reported_cqi(ue_cc, 15);
  ASSERT_TRUE(run_until(
      [&]() {
        std::optional<dl_harq_process_handle> h_retx = ue_cc.harqs.find_pending_dl_retx();
        if (h_retx.has_value()) {
          allocate_dl_retx_grant(slice_ues[u.ue_index], *h_retx);
        }
      },
      [&]() {
        const dl_msg_alloc* g = ue_dl_grant();
        return g != nullptr and g->context.nof_retxs > 0;
      }));
  ASSERT_EQ(current_dl_dci_time_resource(u.crnti).value(), rep_time_resource)
      << "reTx did not keep its repetition scheme after CQI recovered (reTx repetitions were incorrectly re-decided "
         "from the current CQI)";
}

// If a repetition bundle's grant is aborted (e.g. RB allocation failure) after the HARQ was allocated but before
// save_grant_params ever ran, the UE's last known PDSCH slot must be released immediately rather than needlessly
// kept reserved for the rest of the nominal bundle window (see dl_harq_process_impl::last_occasion_slot /
// cell_harq_repository::dealloc_harq): nothing was ever committed to the grid, so the UE must be schedulable again
// right away.
TEST_P(ue_grid_allocator_pdsch_repetition_test,
       when_newtx_bundle_aborts_before_commit_then_ue_is_immediately_schedulable_again)
{
  const ue& u     = add_repetition_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  // Low CQI: the newTx qualifies for a repetition bundle.
  set_reported_cqi(ue_cc, 3);

  // Allocate the HARQ (and PDCCH/UCI) for a bundle, then abort it by committing empty VRBs, before the repetition
  // occasions are ever written to the grid.
  auto result = alloc.allocate_dl_grant(
      ue_newtx_dl_grant_request{slice_ues[u.ue_index], current_slot, units::bytes{1000}, false});
  ASSERT_TRUE(result.has_value());
  result.value().set_pdsch_params(vrb_interval{}, false);

  ASSERT_FALSE(ue_cc.harqs.last_pdsch_slot().valid());

  // The very next slot -- still nominally within the aborted bundle's window -- is immediately schedulable.
  slot_indication();
  auto retry = alloc.allocate_dl_grant(
      ue_newtx_dl_grant_request{slice_ues[u.ue_index], current_slot, units::bytes{1000}, false});
  ASSERT_TRUE(retry.has_value());
  retry.value().set_pdsch_params(vrb_interval{}, false);
}

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_pdsch_repetition_test,
                         testing::Values(duplex_mode::FDD));

// An NTN cell with a UE whose HARQ processes split between DL HARQ feedback enabled and disabled, so that the
// allocator has to pick a process by the feedback the logical channels of the grant need.
class ue_grid_allocator_ra_type0_test : public ue_grid_allocator_default_cfg_test
{
protected:
  const crb_interval     bwp_crbs = cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.crbs;
  const nominal_rbg_size P        = get_nominal_rbg_size(bwp_crbs.length(), rbg_size::config1);

  const ue& add_ue_with_res_alloc(pdsch_config::resource_allocation res_alloc,
                                  unsigned                          max_pdsch_nof_rbs = MAX_NOF_PRBS)
  {
    sched_ue_creation_request_message req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    req.ue_index            = to_du_ue_index(0);
    req.crnti               = to_rnti(0x4601);
    pdsch_config& pdsch_cfg = (*req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdsch_cfg.value();
    pdsch_cfg.res_alloc     = res_alloc;
    pdsch_cfg.rbg_sz        = rbg_size::config1;
    req.cfg.res_alloc_cfg.emplace().pdsch_grant_size_limits = {0, max_pdsch_nof_rbs};
    return add_ue(req);
  }

  // Marks the first VRB of every even RBG as used, so that only the odd RBGs are free.
  vrb_bitmap block_even_rbgs()
  {
    vrb_bitmap blocked(bwp_crbs.length());
    for (unsigned rbg_idx = 0, nof_rbgs = get_nof_rbgs(bwp_crbs, P); rbg_idx < nof_rbgs; rbg_idx += 2) {
      rbg_bitmap rbg(nof_rbgs);
      rbg.set(rbg_idx);
      blocked.set(convert_rbgs_to_vrbs(rbg, bwp_crbs, P).find_lowest());
    }
    used_dl_vrbs |= blocked;
    return blocked;
  }

  const dl_msg_alloc* ue_pdsch(rnti_t rnti) const
  {
    return find_ue_pdsch(rnti, res_grid[0].result_reader().dl().ue_grants());
  }

  const dci_1_1_configuration& ue_dci(rnti_t rnti) const
  {
    return find_ue_dl_pdcch(rnti, res_grid[0].result_reader().dl())->dci.as_c_rnti_f1_1();
  }
};

TEST_P(ue_grid_allocator_ra_type0_test,
       when_free_rbgs_fit_more_vrbs_than_any_free_interval_then_dynamic_switch_selects_type0)
{
  const ue&  u = add_ue_with_res_alloc(pdsch_config::resource_allocation::dynamic_switch);
  vrb_bitmap blocked;
  ASSERT_TRUE(run_until(
      [&]() {
        blocked = block_even_rbgs();
        allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{100000}, false);
      },
      [&]() { return ue_pdsch(u.crnti) != nullptr; }));

  const vrb_alloc& rbs = ue_pdsch(u.crnti)->pdsch_cfg.rbs;
  ASSERT_TRUE(rbs.is_type0());
  // The largest free VRB interval spans the end of an even RBG and a full odd RBG.
  ASSERT_GT(rbs.nof_vrbs(), 2 * to_underlying(P) - 1);
  vrb_bitmap overlap = rbs.type0();
  overlap &= blocked;
  ASSERT_TRUE(overlap.none());
  const rbg_bitmap rbgs = convert_vrbs_to_rbgs(rbs.type0(), bwp_crbs, P);
  ASSERT_EQ(convert_rbgs_to_vrbs(rbgs, bwp_crbs, P), rbs.type0()) << "Type 0 grants must span whole RBGs";

  const dci_1_1_configuration& dci = ue_dci(u.crnti);
  ASSERT_EQ(dci.dynamic_pdsch_res_allocation_type, dynamic_resource_allocation::type_0);
  ASSERT_EQ(dci.frequency_resource, rbgs.to_uint64());
}

TEST_P(ue_grid_allocator_ra_type0_test, when_a_free_interval_fits_the_grant_then_dynamic_switch_selects_type1)
{
  const ue& u = add_ue_with_res_alloc(pdsch_config::resource_allocation::dynamic_switch);
  ASSERT_TRUE(run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{20}, false); },
                        [&]() { return ue_pdsch(u.crnti) != nullptr; }));

  const vrb_alloc& rbs = ue_pdsch(u.crnti)->pdsch_cfg.rbs;
  ASSERT_TRUE(rbs.is_type1());
  const dci_1_1_configuration& dci = ue_dci(u.crnti);
  ASSERT_EQ(dci.dynamic_pdsch_res_allocation_type, dynamic_resource_allocation::type_1);
  ASSERT_EQ(dci.frequency_resource,
            ra_frequency_type1_get_riv(
                ra_frequency_type1_configuration{bwp_crbs.length(), rbs.type1().start(), rbs.type1().length()}));
}

TEST_P(ue_grid_allocator_ra_type0_test, when_only_type0_is_configured_then_small_grants_use_whole_rbgs)
{
  const ue& u = add_ue_with_res_alloc(pdsch_config::resource_allocation::resource_allocation_type_0);
  ASSERT_TRUE(run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{20}, false); },
                        [&]() { return ue_pdsch(u.crnti) != nullptr; }));

  const vrb_alloc& rbs = ue_pdsch(u.crnti)->pdsch_cfg.rbs;
  ASSERT_TRUE(rbs.is_type0());
  const rbg_bitmap rbgs = convert_vrbs_to_rbgs(rbs.type0(), bwp_crbs, P);
  ASSERT_EQ(convert_rbgs_to_vrbs(rbgs, bwp_crbs, P), rbs.type0()) << "Type 0 grants must span whole RBGs";
  const dci_1_1_configuration& dci = ue_dci(u.crnti);
  ASSERT_FALSE(dci.dynamic_pdsch_res_allocation_type.has_value());
  ASSERT_EQ(dci.frequency_resource, rbgs.to_uint64());
}

TEST_P(ue_grid_allocator_ra_type0_test, when_type0_newtx_is_retransmitted_then_retx_keeps_nof_vrbs_and_can_use_type1)
{
  const ue& u     = add_ue_with_res_alloc(pdsch_config::resource_allocation::dynamic_switch);
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();
  ASSERT_TRUE(run_until(
      [&]() {
        block_even_rbgs();
        allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{100000}, false);
      },
      [&]() { return ue_pdsch(u.crnti) != nullptr; }));
  const dl_msg_alloc& newtx = *ue_pdsch(u.crnti);
  ASSERT_TRUE(newtx.pdsch_cfg.rbs.is_type0());
  const unsigned nof_vrbs = newtx.pdsch_cfg.rbs.nof_vrbs();

  std::optional<dl_harq_process_handle> h_dl = ue_cc.harqs.dl_harq(newtx.pdsch_cfg.harq_id);
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));

  // Without fragmentation, the reTx fits in a contiguous VRB interval.
  ASSERT_TRUE(run_until(
      [&]() {
        std::optional<dl_harq_process_handle> h_retx = ue_cc.harqs.find_pending_dl_retx();
        if (h_retx.has_value()) {
          allocate_dl_retx_grant(slice_ues[u.ue_index], *h_retx);
        }
      },
      [&]() {
        const dl_msg_alloc* grant = ue_pdsch(u.crnti);
        return grant != nullptr and grant->context.nof_retxs > 0;
      }));
  const vrb_alloc& retx_rbs = ue_pdsch(u.crnti)->pdsch_cfg.rbs;
  ASSERT_TRUE(retx_rbs.is_type1());
  ASSERT_EQ(retx_rbs.nof_vrbs(), nof_vrbs);
  ASSERT_EQ(ue_dci(u.crnti).dynamic_pdsch_res_allocation_type, dynamic_resource_allocation::type_1);
}

TEST_P(ue_grid_allocator_ra_type0_test, when_type0_only_grant_budget_is_below_rbg_size_then_a_whole_rbg_is_allocated)
{
  const ue& u = add_ue_with_res_alloc(pdsch_config::resource_allocation::resource_allocation_type_0);

  ASSERT_TRUE(run_until(
      [&]() {
        auto result = alloc.allocate_dl_grant(
            ue_newtx_dl_grant_request{slice_ues[u.ue_index], current_slot, units::bytes{100000}, false});
        if (not result.has_value()) {
          return;
        }
        // The grant budget is below the RBG size, but the slice still has RBs left for a whole RBG.
        const vrb_alloc vrbs = result.value().recommended_vrbs(used_dl_vrbs, 1, MAX_NOF_PRBS, false);
        result.value().set_pdsch_params(vrbs, false);
      },
      [&]() { return ue_pdsch(u.crnti) != nullptr; }));

  const vrb_alloc& rbs = ue_pdsch(u.crnti)->pdsch_cfg.rbs;
  ASSERT_TRUE(rbs.is_type0());
  const rbg_bitmap rbgs = convert_vrbs_to_rbgs(rbs.type0(), bwp_crbs, P);
  ASSERT_EQ(rbgs.count(), 1);
  ASSERT_EQ(convert_rbgs_to_vrbs(rbgs, bwp_crbs, P), rbs.type0());
}

TEST_P(ue_grid_allocator_ra_type0_test, when_ue_rb_limit_is_not_a_multiple_of_rbg_size_then_type0_grant_and_retx_fit_it)
{
  const unsigned max_ue_rbs = 2 * to_underlying(P) + 1;
  const ue&      u = add_ue_with_res_alloc(pdsch_config::resource_allocation::resource_allocation_type_0, max_ue_rbs);
  ue_cell&       ue_cc = ues[u.ue_index].get_pcell();
  ASSERT_TRUE(run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{100000}, false); },
                        [&]() { return ue_pdsch(u.crnti) != nullptr; }));
  const dl_msg_alloc& newtx = *ue_pdsch(u.crnti);
  ASSERT_TRUE(newtx.pdsch_cfg.rbs.is_type0());
  ASSERT_LE(newtx.pdsch_cfg.rbs.nof_vrbs(), max_ue_rbs);

  std::optional<dl_harq_process_handle> h_dl = ue_cc.harqs.dl_harq(newtx.pdsch_cfg.harq_id);
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));

  ASSERT_TRUE(run_until(
      [&]() {
        std::optional<dl_harq_process_handle> h_retx = ue_cc.harqs.find_pending_dl_retx();
        if (h_retx.has_value()) {
          allocate_dl_retx_grant(slice_ues[u.ue_index], *h_retx);
        }
      },
      [&]() {
        const dl_msg_alloc* grant = ue_pdsch(u.crnti);
        return grant != nullptr and grant->context.nof_retxs > 0;
      }));
}

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_ra_type0_test,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));

class ue_grid_allocator_dl_harq_feedback_test : public ue_grid_allocator_test
{
protected:
  // The SRB outranks the DRB, so the feedback the slice needs follows whichever of the two holds data.
  const lcid_t srb_lcid = lcid_t::LCID_SRB2;
  const lcid_t drb_lcid = uint_to_lcid(4);

  ue_grid_allocator_dl_harq_feedback_test() :
    ue_grid_allocator_test(test_params{config_helpers::make_default_scheduler_expert_config(),
                                       search_space_configuration::ue_specific_dci_format::f0_1_and_1_1,
                                       std::chrono::milliseconds{5}})
  {
  }

  // Adds a UE with one HARQ process carrying the DL HARQ feedback and one with it disabled, and a logical channel
  // bound to each of them.
  ue& add_feedback_restricted_ue()
  {
    sched_ue_creation_request_message req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    req.ue_index                                                 = to_du_ue_index(0);
    req.crnti                                                    = to_rnti(0x4601);
    (*req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg = cell_cfg.bwp_res[to_bwp_id(0)].dl().ded_pdcchs[0];

    auto& pdsch_cell_cfg         = (*req.cfg.cells)[0].ded->serv_cell_cfg.pdsch_serv_cell_cfg.value();
    pdsch_cell_cfg.nof_harq_proc = pdsch_serving_cell_config::nof_harq_proc_for_pdsch::n2;
    // Process 0 carries the feedback and process 1 has it disabled.
    pdsch_cell_cfg.dl_harq_feedback_disabled.set(1);

    req.cfg.lc_config_list->push_back(config_helpers::create_default_logical_channel_config(srb_lcid));
    req.cfg.lc_config_list->back().allowed_dl_harq_feedback = dl_harq_feedback::enabled;
    req.cfg.lc_config_list->push_back(config_helpers::create_default_logical_channel_config(drb_lcid));
    req.cfg.lc_config_list->back().allowed_dl_harq_feedback = dl_harq_feedback::disabled;

    ue& u = add_ue(req);

    // A UE is created before its dedicated PDSCH configuration exists, so its HARQ processes only take a mode once it
    // is reconfigured.
    sched_ue_reconfiguration_message reconf_msg{.ue_index = req.ue_index, .crnti = req.crnti, .cfg = req.cfg};
    ue_config_update_event           ev = cfg_mng.update_ue(reconf_msg);
    ues.reconfigure_ue(ev.next_config(), sched_ue_config_request::causes::other_rrc_proc);
    ues.ue_config_applied(ev.get_ue_index());
    ev.notify_completion();

    return u;
  }

  // Number of HARQ-ACK bits the UE has booked across the resource grid.
  unsigned booked_harq_ack_bits(rnti_t rnti) const
  {
    unsigned nof_bits = 0;
    for (unsigned sl = 0; sl != res_grid.max_ul_slot_alloc_delay + 1; ++sl) {
      const pucch_info* pucch =
          find_ue_pucch_with_harq_ack(rnti, res_grid[sl].result_reader().ul().pucchs().unsorted());
      if (pucch != nullptr) {
        nof_bits += pucch->uci_bits.harq_ack_nof_bits;
      }
    }
    return nof_bits;
  }
};

// A reTx is bound to the process it already holds, so it is scheduled even once every process is busy and a new
// transmission can no longer find one.
TEST_P(ue_grid_allocator_dl_harq_feedback_test, retx_is_scheduled_when_every_harq_process_is_busy)
{
  const ue& u     = add_feedback_restricted_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  auto allocate_newtx = [&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{200}, false); };
  auto ue_dl_grant    = [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()); };

  // The channel with the feedback disabled takes the process that carries none.
  push_dl_bs(u.ue_index, drb_lcid, 1000);
  ASSERT_TRUE(run_until(allocate_newtx, [&]() { return ue_cc.harqs.nof_empty_dl_harqs() == 1; }));

  // The channel carrying the feedback outranks it and takes the last free process.
  push_dl_bs(u.ue_index, srb_lcid, 1000);
  ASSERT_TRUE(run_until(allocate_newtx, [&]() { return not ue_cc.harqs.has_empty_dl_harqs(); }));

  std::optional<dl_harq_process_handle> h_dl = ue_cc.harqs.dl_harq(to_harq_id(0));
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_EQ(h_dl->mode(), harq_utils::harq_mode_t::normal);
  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));

  ASSERT_TRUE(run_until(
      [&]() {
        std::optional<dl_harq_process_handle> h_retx = ue_cc.harqs.find_pending_dl_retx();
        if (h_retx.has_value()) {
          allocate_dl_retx_grant(slice_ues[u.ue_index], *h_retx);
        }
      },
      [&]() {
        const dl_msg_alloc* grant = ue_dl_grant();
        return grant != nullptr and grant->context.nof_retxs > 0;
      }))
      << "reTx was refused for the lack of a free HARQ process";
}

// The feedback a reTx carries comes from the process it holds, not from the data waiting in the slice, so its PUCCH is
// booked even while a channel with the feedback disabled holds more data.
TEST_P(ue_grid_allocator_dl_harq_feedback_test, retx_of_a_feedback_enabled_process_books_a_pucch)
{
  const ue& u     = add_feedback_restricted_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  // The channel carrying the feedback takes the process that carries it too.
  push_dl_bs(u.ue_index, srb_lcid, 200);
  ASSERT_TRUE(
      run_until([&]() { allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{200}, false); },
                [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; }));
  std::optional<dl_harq_process_handle> h_dl = ue_cc.harqs.dl_harq(to_harq_id(0));
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_EQ(h_dl->mode(), harq_utils::harq_mode_t::normal);

  // The channel with the feedback disabled is left as the only one holding data, so a new transmission would pick a
  // process that carries no feedback.
  push_dl_bs(u.ue_index, srb_lcid, 0);
  push_dl_bs(u.ue_index, drb_lcid, 100000);
  ASSERT_EQ(slice_ues[u.ue_index].select_dl_transport_block().restrictions.harq_feedback, dl_harq_feedback::disabled);

  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));

  // Bits booked in the slot that carries the reTx, before its own PUCCH is booked.
  unsigned nof_bits_before = 0;
  ASSERT_TRUE(run_until(
      [&]() {
        std::optional<dl_harq_process_handle> h_retx = ue_cc.harqs.find_pending_dl_retx();
        if (h_retx.has_value()) {
          nof_bits_before = booked_harq_ack_bits(u.crnti);
          allocate_dl_retx_grant(slice_ues[u.ue_index], *h_retx);
        }
      },
      [&]() {
        const dl_msg_alloc* grant = find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants());
        return grant != nullptr and grant->context.nof_retxs > 0;
      }));
  ASSERT_GT(booked_harq_ack_bits(u.crnti), nof_bits_before)
      << "no PUCCH was booked for the reTx of a process carrying the DL HARQ feedback";
}

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_dl_harq_feedback_test,
                         testing::Values(duplex_mode::FDD));

// An NTN cell with a UE whose UL HARQ processes split between mode A and mode B, so that the allocator has to pick a
// process by the mode the logical channels of the grant require.
class ue_grid_allocator_ul_harq_mode_test : public ue_grid_allocator_test
{
protected:
  // The mode follows the buffer status, which is reported per LCG, so each channel takes an LCG of its own.
  const lcid_t   mode_a_lcid = uint_to_lcid(4);
  const lcg_id_t mode_a_lcg  = uint_to_lcg_id(1);
  const lcid_t   mode_b_lcid = uint_to_lcid(5);
  const lcg_id_t mode_b_lcg  = uint_to_lcg_id(2);

  ue_grid_allocator_ul_harq_mode_test() :
    ue_grid_allocator_test(test_params{config_helpers::make_default_scheduler_expert_config(),
                                       search_space_configuration::ue_specific_dci_format::f0_1_and_1_1,
                                       std::chrono::milliseconds{5}})
  {
  }

  // Adds a UE whose first UL HARQ process operates in mode A and whose remaining ones operate in mode B, with a
  // logical channel bound to each mode.
  ue& add_mode_restricted_ue()
  {
    sched_ue_creation_request_message req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    req.ue_index                                                 = to_du_ue_index(0);
    req.crnti                                                    = to_rnti(0x4601);
    (*req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg = cell_cfg.bwp_res[to_bwp_id(0)].dl().ded_pdcchs[0];

    // A bit set to one identifies a process in mode A, so only process 0 operates in that mode.
    auto& ul_harq_mode = (*req.cfg.cells)[0].ded->serv_cell_cfg.ul_config->pusch_serv_cell_cfg->ul_harq_mode;
    ul_harq_mode       = harq_ul_mode_mask(ul_harq_mode.size());
    ul_harq_mode.set(0);

    req.cfg.lc_config_list->push_back(config_helpers::create_default_logical_channel_config(mode_a_lcid));
    req.cfg.lc_config_list->back().lc_group          = mode_a_lcg;
    req.cfg.lc_config_list->back().allowed_harq_mode = ul_harq_mode::mode_a;
    req.cfg.lc_config_list->push_back(config_helpers::create_default_logical_channel_config(mode_b_lcid));
    req.cfg.lc_config_list->back().lc_group          = mode_b_lcg;
    req.cfg.lc_config_list->back().allowed_harq_mode = ul_harq_mode::mode_b;

    ue& u = add_ue(req);

    // A UE is created before its dedicated PUSCH configuration exists, so its HARQ processes only take a mode once it
    // is reconfigured.
    sched_ue_reconfiguration_message reconf_msg{.ue_index = req.ue_index, .crnti = req.crnti, .cfg = req.cfg};
    ue_config_update_event           ev = cfg_mng.update_ue(reconf_msg);
    ues.reconfigure_ue(ev.next_config(), sched_ue_config_request::causes::other_rrc_proc);
    ues.ue_config_applied(ev.get_ue_index());
    ev.notify_completion();

    return u;
  }

  // Reports the UL buffer status of a single LCG.
  void push_ul_bsr(du_ue_index_t ue_index, lcg_id_t lcg_id, unsigned bytes)
  {
    ul_bsr_indication_message msg{
        to_du_cell_index(0), ue_index, to_rnti(0x4601 + (unsigned)ue_index), bsr_format::SHORT_BSR, {}};
    msg.reported_lcgs.push_back(ul_bsr_lcg_report{lcg_id, bytes});
    ues[ue_index].handle_bsr_indication(msg);
  }

  void allocate_ul_retx_grant(const slice_ue& user, ul_harq_process_handle h_ul)
  {
    const slot_point pusch_slot   = get_next_ul_slot(current_slot);
    const auto&      init_ul_bwp  = cell_cfg.params.ul_cfg_common.init_ul_bwp;
    auto             used_ul_vrbs = res_grid[pusch_slot]
                            .ul_res_grid
                            .used_prbs(init_ul_bwp.generic_params.scs,
                                       init_ul_bwp.generic_params.crbs,
                                       init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list[0].symbols)
                            .convert_to<vrb_bitmap>();
    auto result = alloc.allocate_ul_grant(ue_retx_ul_grant_request{
        user, pusch_slot, h_ul, used_ul_vrbs, ofdm_symbol_range{0, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP}});
    if (result.has_value()) {
      used_ul_vrbs.fill(result.value().vrbs.start(), result.value().vrbs.stop());
    }
  }
};

// A grant takes a process operating in the mode its logical channels require, as per \c allowedHARQ-mode, TS 38.331.
TEST_P(ue_grid_allocator_ul_harq_mode_test, newtx_takes_a_process_in_the_mode_its_channels_require)
{
  const ue& u     = add_mode_restricted_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  const size_t nof_mode_b = ue_cc.harqs.nof_empty_ul_harqs(harq_utils::harq_mode_t::feedback_disabled_or_mode_b);
  ASSERT_EQ(ue_cc.harqs.nof_empty_ul_harqs(harq_utils::harq_mode_t::normal), 1U);

  // Only the channel bound to mode B holds data, so the grant must ride a process operating in that mode.
  push_ul_bsr(u.ue_index, mode_b_lcg, 1000);
  ASSERT_TRUE(run_until([&]() { allocate_ul_newtx_grant(slice_ues[u.ue_index], units::bytes{200}); },
                        [&]() {
                          return ue_cc.harqs.nof_empty_ul_harqs(harq_utils::harq_mode_t::feedback_disabled_or_mode_b) ==
                                 nof_mode_b - 1;
                        }));
  ASSERT_EQ(ue_cc.harqs.nof_empty_ul_harqs(harq_utils::harq_mode_t::normal), 1U)
      << "the grant of a channel bound to mode B took the process operating in mode A";

  // The channel bound to mode A outranks it as soon as it holds data.
  push_ul_bsr(u.ue_index, mode_a_lcg, 1000);
  ASSERT_TRUE(run_until([&]() { allocate_ul_newtx_grant(slice_ues[u.ue_index], units::bytes{200}); },
                        [&]() { return ue_cc.harqs.nof_empty_ul_harqs(harq_utils::harq_mode_t::normal) == 0; }));
}

// A reTx is bound to the process it already holds, so it is scheduled even once the mode its logical channels require
// has no free process left.
TEST_P(ue_grid_allocator_ul_harq_mode_test, retx_is_scheduled_when_its_mode_has_no_free_process)
{
  const ue& u     = add_mode_restricted_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  // The channel bound to mode A takes the only process operating in that mode.
  push_ul_bsr(u.ue_index, mode_a_lcg, 1000);
  ASSERT_TRUE(run_until([&]() { allocate_ul_newtx_grant(slice_ues[u.ue_index], units::bytes{200}); },
                        [&]() { return ue_cc.harqs.nof_empty_ul_harqs(harq_utils::harq_mode_t::normal) == 0; }));
  ASSERT_EQ(slice_ues[u.ue_index].select_ul_grant().restrictions.harq_mode, ul_harq_mode::mode_a);

  std::optional<ul_harq_process_handle> h_ul = ue_cc.harqs.find_ul_harq_waiting_ack();
  ASSERT_TRUE(h_ul.has_value());
  ASSERT_EQ(h_ul->mode(), harq_utils::harq_mode_t::normal);
  ASSERT_TRUE(h_ul->ul_crc_info(false).has_value());

  ASSERT_TRUE(run_until(
      [&]() {
        std::optional<ul_harq_process_handle> h_retx = ue_cc.harqs.find_pending_ul_retx();
        if (h_retx.has_value()) {
          allocate_ul_retx_grant(slice_ues[u.ue_index], *h_retx);
        }
      },
      [&]() {
        const ul_sched_info* grant = find_ue_pusch(u.crnti, res_grid[0].result_reader().ul().puschs());
        return grant != nullptr and grant->context.nof_retxs > 0;
      }))
      << "reTx was refused for the lack of a free UL HARQ process";
}

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_ul_harq_mode_test,
                         testing::Values(duplex_mode::FDD));

class ue_grid_allocator_pusch_repetition_test : public ue_grid_allocator_test
{
protected:
  static constexpr uint8_t nof_reps = 4;
  const lcid_t             drb_lcid = uint_to_lcid(4);

  /// \param pusch_crb_limits CRB window the UE PUSCH is confined to.
  /// \param force_rep         Request repetitions regardless of the estimated SINR.
  static test_params make_test_params(crb_interval pusch_crb_limits, bool force_rep, std::chrono::milliseconds k_mac)
  {
    test_params params;
    params.sched_cfg                             = config_helpers::make_default_scheduler_expert_config();
    params.sched_cfg.ue.pusch_sinr_rep_threshold = 10.0F;
    // Disable UL OLLA so the effective SNR equals the value set on the channel state manager.
    params.sched_cfg.ue.olla_ul_snr_inc  = 0;
    params.sched_cfg.ue.pusch_crb_limits = pusch_crb_limits;
    params.sched_cfg.ue.pusch_force_rep  = force_rep;
    params.ntn_k_mac                     = k_mac;
    return params;
  }

  explicit ue_grid_allocator_pusch_repetition_test(crb_interval              pusch_crb_limits,
                                                   bool                      force_rep = false,
                                                   std::chrono::milliseconds ntn_k_mac = std::chrono::milliseconds{0}) :
    ue_grid_allocator_test(make_test_params(pusch_crb_limits, force_rep, ntn_k_mac))
  {
  }

  // Shifts the PUSCH grant away from CRB 0, so an occasion's repeated PRBs stay clear of the cell's PUCCH.
  ue_grid_allocator_pusch_repetition_test() : ue_grid_allocator_pusch_repetition_test(crb_interval{10, 40}) {}

  // Adds a UE configured with a Rel-16 PUSCH TDRA list that mirrors the common list and appends a repetition entry.
  const ue& add_repetition_ue(std::optional<meas_gap_config> meas_gap = std::nullopt)
  {
    sched_ue_creation_request_message req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    req.ue_index         = to_du_ue_index(0);
    req.crnti            = to_rnti(0x4601);
    req.cfg.meas_gap_cfg = meas_gap;
    req.cfg.lc_config_list->push_back(config_helpers::create_default_logical_channel_config(drb_lcid));
    (*req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg = cell_cfg.bwp_res[to_bwp_id(0)].dl().ded_pdcchs[0];

    auto&       pusch_cfg   = (*req.cfg.cells)[0].ded->serv_cell_cfg.ul_config->init_ul_bwp.pusch_cfg.value();
    const auto& common_list = cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
    for (const auto& common_alloc : common_list) {
      pusch_cfg.pusch_td_alloc_list.push_back(common_alloc);
    }
    rep_time_resource                               = pusch_cfg.pusch_td_alloc_list.size();
    pusch_time_domain_resource_allocation rep_alloc = common_list.front();
    rep_alloc.nof_repetitions                       = nof_reps;
    rep_k2                                          = rep_alloc.k2;
    pusch_cfg.pusch_td_alloc_list.push_back(rep_alloc);

    return add_ue(req);
  }

  /// Adds a repetition UE whose only CG occasion within its period is \c cg_slot.
  ///
  /// The schedulers read the UE's CG occasions off its \c ue_conf_grant_state_manager, which the Configured Grant
  /// scheduler primes on UE creation. That scheduler is not part of this fixture, so the state is set here the way
  /// \c configured_grant_scheduler_impl::add_ue_to_wheel() sets it. Only the periodicity and the time offset carry
  /// meaning: the grant allocator reaches the state manager through \c is_cg_slot() alone.
  ///
  /// NOTE: no CG configuration is attached to the UE itself. The cell of this fixture has no cell-level CG, and the UE
  /// config validator rejects a UE carrying a CG that its cell does not have.
  const ue& add_cg_repetition_ue(slot_point cg_slot)
  {
    constexpr auto    period      = cg_configuration::periodicity_t::sl80;
    constexpr uint8_t cg_mcs      = 10;
    constexpr uint8_t cg_td_alloc = 0;
    const unsigned    time_offset = cg_slot.count() % static_cast<unsigned>(period);

    const ue& u = add_repetition_ue();
    ues[u.ue_index].get_pcell().get_conf_grant_state_manager().update_state(
        static_cast<unsigned>(period), units::bytes{0}, vrb_interval{0, 0}, cg_mcs, cg_td_alloc, time_offset, 0U);
    return u;
  }

  void set_pusch_snr(ue_cell& ue_cc, float snr_db) { ue_cc.channel_state_manager().update_pusch_snr(snr_db); }

  // Time-domain resource of the UL DCI (format 0_1) scheduled for the UE in the current slot, if any.
  std::optional<unsigned> current_ul_dci_time_resource(rnti_t rnti) const
  {
    const pdcch_ul_information* pdcch = find_ue_ul_pdcch(rnti, res_grid[0].result_reader().dl());
    if (pdcch == nullptr or pdcch->dci.type() != dci_ul_rnti_config_type::c_rnti_f0_1) {
      return std::nullopt;
    }
    return pdcch->dci.as_c_rnti_f0_1().time_resource;
  }

  void allocate_ul_retx_grant(const slice_ue& user, ul_harq_process_handle h_ul)
  {
    // The repetition row's k2 is fixed (unlike DL's k0, commonly 0), so the candidate PUSCH slot is rep_k2 ahead
    // of the PDCCH slot, not current_slot itself.
    const slot_point pusch_slot   = current_slot + rep_k2;
    const auto&      init_ul_bwp  = cell_cfg.params.ul_cfg_common.init_ul_bwp;
    auto             used_ul_vrbs = res_grid[pusch_slot]
                            .ul_res_grid
                            .used_prbs(init_ul_bwp.generic_params.scs,
                                       init_ul_bwp.generic_params.crbs,
                                       init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list[0].symbols)
                            .convert_to<vrb_bitmap>();
    auto result = alloc.allocate_ul_grant(ue_retx_ul_grant_request{
        user, pusch_slot, h_ul, used_ul_vrbs, ofdm_symbol_range{0, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP}});
    if (result.has_value()) {
      used_ul_vrbs.fill(result.value().vrbs.start(), result.value().vrbs.stop());
    }
  }

  uint8_t rep_time_resource = 0;
  /// PDCCH-to-PUSCH delay (k2) of the repetition TDRA row, i.e. the offset from the PDCCH slot to occasion 0.
  uint8_t rep_k2 = 0;
};

// A reTx reuses the original transmission's scheme (like the number of layers), so its repetition count comes from
// the HARQ grant params, not the current link quality. Once the SINR recovers, a reTx of a grant started at low SINR
// must still be a bundle, while a fresh newTx is a single transmission.
TEST_P(ue_grid_allocator_pusch_repetition_test, retx_reuses_original_repetition_scheme_after_sinr_recovers)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  const ue& u     = add_repetition_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  auto ue_ul_grant = [&]() { return find_ue_pusch(u.crnti, res_grid[0].result_reader().ul().puschs()); };

  // The PUSCH repetition row's k2 is never 0, so the PDCCH (written to slot 0) and the base occasion's PUSCH
  // (written k2 ahead) are never both visible at res_grid[0] as run_until() advances one slot at a time. Capture the
  // DCI's time_resource when the PDCCH is created rather than re-deriving it later.
  std::optional<unsigned> newtx_time_resource;

  // Low SINR: the newTx must be scheduled as a repetition bundle.
  set_pusch_snr(ue_cc, 0.0F);
  ASSERT_TRUE(run_until(
      [&]() {
        allocate_ul_newtx_grant(slice_ues[u.ue_index], units::bytes{1000});
        if (auto tr = current_ul_dci_time_resource(u.crnti); tr.has_value()) {
          newtx_time_resource = tr;
        }
      },
      [&]() {
        const ul_sched_info* g = ue_ul_grant();
        return g != nullptr and g->context.nof_retxs == 0;
      }));
  ASSERT_TRUE(newtx_time_resource.has_value());
  ASSERT_EQ(newtx_time_resource.value(), rep_time_resource)
      << "newTx at low SINR was not scheduled as a repetition bundle";

  // NACK the bundle to force a reTx.
  std::optional<ul_harq_process_handle> h_ul = ue_cc.harqs.find_ul_harq_waiting_ack();
  ASSERT_TRUE(h_ul.has_value());
  ASSERT_TRUE(h_ul->ul_crc_info(false).has_value());

  // SINR recovers: a fresh newTx would now be a single transmission, but the reTx must keep the bundle.
  std::optional<unsigned> retx_time_resource;
  set_pusch_snr(ue_cc, 30.0F);
  ASSERT_TRUE(run_until(
      [&]() {
        std::optional<ul_harq_process_handle> h_retx = ue_cc.harqs.find_pending_ul_retx();
        if (h_retx.has_value()) {
          allocate_ul_retx_grant(slice_ues[u.ue_index], *h_retx);
          if (auto tr = current_ul_dci_time_resource(u.crnti); tr.has_value()) {
            retx_time_resource = tr;
          }
        }
      },
      [&]() {
        const ul_sched_info* g = ue_ul_grant();
        return g != nullptr and g->context.nof_retxs > 0;
      }));
  ASSERT_TRUE(retx_time_resource.has_value());
  ASSERT_EQ(retx_time_resource.value(), rep_time_resource)
      << "reTx did not keep its repetition scheme after SINR recovered (reTx repetitions were incorrectly re-decided "
         "from the current SINR)";
}

// As per TS 38.213 Section 9.2.5.2 (confirmed against a real UE), a PUCCH overlapping a PUSCH with repetitions is
// not transmitted: the UE multiplexes its UCI onto the single overlapping occasion, and the remaining occasions
// carry UL-SCH only. So a dedicated HARQ-ACK PUCCH already scheduled in an occasion's slot -- for a DL PDSCH
// allocated independently of this bundle, say -- must end up on that occasion, with the PUCCH grant dropped.
TEST_P(ue_grid_allocator_pusch_repetition_test, uci_colliding_with_repetition_occasion_is_multiplexed_onto_it)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  const ue&                    u           = add_repetition_ue();
  ue_cell&                     ue_cc       = ues[u.ue_index].get_pcell();
  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();

  // Reach a known, ready slot first: with no run_until below, every occasion's slot is then known in advance.
  slot_indication();

  // Occasion 3 (last of 4) lands at current_slot + rep_k2 + 3 -- FDD, so the offsets are consecutive.
  // Pre-allocate a dedicated HARQ-ACK PUCCH there.
  constexpr unsigned            target_occasion_offset = 3;
  const std::optional<unsigned> pucch_res_ind =
      pucch_alloc.alloc_ded_harq_ack(res_grid, ue_cell_cfg, 0, rep_k2 + target_occasion_offset, 0);
  ASSERT_TRUE(pucch_res_ind.has_value());
  const slot_point occasion_slot = current_slot + rep_k2 + target_occasion_offset;
  ASSERT_FALSE(res_grid[occasion_slot].result_reader().ul().pucchs().empty());

  set_pusch_snr(ue_cc, 0.0F);
  const slot_point pusch_slot = current_slot + rep_k2;
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  // The PUCCH must be gone: the UE does not transmit it, the UCI rides on the occasion instead.
  ASSERT_TRUE(res_grid[occasion_slot].result_reader().ul().pucchs().empty())
      << "The PUCCH was left in place even though its UCI moved to the repetition occasion";

  // The colliding occasion must have been written, and must carry the HARQ-ACK bits taken off the PUCCH.
  const ul_sched_info* occ = find_ue_pusch(u.crnti, res_grid[occasion_slot].result_reader().ul().puschs());
  ASSERT_NE(occ, nullptr) << "The repetition occasion colliding with the PUCCH was dropped instead of carrying the UCI";
  ASSERT_TRUE(occ->uci.has_value()) << "The occasion overlapping the PUCCH carries no UCI";
  ASSERT_TRUE(occ->uci->harq.has_value());
  ASSERT_GT(occ->uci->harq->harq_ack_nof_bits, 0);

  // The base occasion and every other occasion must be present and carry UL-SCH only.
  const ul_sched_info* base_occ = find_ue_pusch(u.crnti, res_grid[pusch_slot].result_reader().ul().puschs());
  ASSERT_NE(base_occ, nullptr);
  ASSERT_FALSE(base_occ->uci.has_value());
  for (unsigned offset = 1; offset != nof_reps; ++offset) {
    if (offset == target_occasion_offset) {
      continue;
    }
    const ul_sched_info* other_occ =
        find_ue_pusch(u.crnti, res_grid[pusch_slot + offset].result_reader().ul().puschs());
    ASSERT_NE(other_occ, nullptr);
    ASSERT_FALSE(other_occ->uci.has_value()) << "UCI was multiplexed onto more than one repetition occasion";
  }
}

// The UL DAI covers the whole transmission, so a HARQ-ACK booked in a *repetition occasion's* slot must reach the
// DCI like one in the base slot. Counting only the base slot leaves the DAI at its "no HARQ-ACK" default (3, i.e.
// V_T_DAI_UL=4) while the gNB demaps a bit off the occasion: the codebooks disagree and the UCI is lost, silently.
TEST_P(ue_grid_allocator_pusch_repetition_test, harq_ack_booked_on_a_repetition_occasion_is_counted_in_the_ul_dai)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  const ue& u     = add_repetition_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  slot_indication();

  // Book a HARQ-ACK through the UCI allocator (it maintains the per-slot counter the DAI derives from), forcing it
  // onto the slot of occasion 3.
  constexpr unsigned           target_occasion_offset = 3;
  const std::array<uint8_t, 1> k1_list                = {static_cast<uint8_t>(rep_k2 + target_occasion_offset)};
  ASSERT_TRUE(uci_alloc.alloc_harq_ack(res_grid, ue_cc, 0, k1_list, pucch_repetition_factor::n1).has_value());
  const slot_point occasion_slot = current_slot + rep_k2 + target_occasion_offset;
  ASSERT_EQ(uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(occasion_slot, u.crnti), 1);

  set_pusch_snr(ue_cc, 0.0F);
  const slot_point pusch_slot = current_slot + rep_k2;
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  // Find the UL grant's PDCCH and check the DAI it carries.
  const pdcch_ul_information* ul_pdcch = nullptr;
  for (const pdcch_ul_information& pdcch : res_grid[0].result_reader().dl().ul_pdcchs()) {
    if (pdcch.ctx.rnti == u.crnti) {
      ul_pdcch = &pdcch;
      break;
    }
  }
  ASSERT_NE(ul_pdcch, nullptr);
  ASSERT_EQ(ul_pdcch->dci.type(), dci_ul_rnti_config_type::c_rnti_f0_1);
  // One HARQ-ACK bit on the bundle: TS 38.213 Table 9.1.3-2 leftmost column, (1 - 1) % 4 == 0.
  ASSERT_EQ(ul_pdcch->dci.as_c_rnti_f0_1().first_dl_assignment_index, 0)
      << "The HARQ-ACK booked on a repetition occasion was not counted in the UL DAI";
}

// An occasion repeats the base grant's RBs -- that is what lets the PHY combine them -- so RBs busy in any occasion
// slot are unusable by the whole bundle. If the base grant ignores them the occasion is dropped, and the UE transmits
// there regardless: a collision with whoever got those RBs. The bundle must route around them.
TEST_P(ue_grid_allocator_pusch_repetition_test, bundle_avoids_rbs_busy_in_an_occasion_slot)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  const ue& u     = add_repetition_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  slot_indication();

  // Occupy the lower half of the UE's PUSCH CRB window ({10, 40}) in the slot of occasion 3, standing in for another
  // UE's grant landing there first.
  constexpr unsigned target_occasion_offset = 3;
  const slot_point   occasion_slot          = current_slot + rep_k2 + target_occasion_offset;
  const crb_interval blocked_crbs{10, 25};
  res_grid[occasion_slot].ul_res_grid.fill(
      grant_info{cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs, ofdm_symbol_range{0, 14}, blocked_crbs});

  set_pusch_snr(ue_cc, 0.0F);
  const slot_point pusch_slot = current_slot + rep_k2;
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  const ul_sched_info* base_occ = find_ue_pusch(u.crnti, res_grid[pusch_slot].result_reader().ul().puschs());
  ASSERT_NE(base_occ, nullptr);
  // The RBs picked must not touch the busy ones. Checking the base grant is what matters: the occasions repeat its
  // RBs verbatim (asserted below).
  const vrb_interval blocked_vrbs = rb_helper::crb_to_vrb_ul_non_interleaved(
      blocked_crbs, cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.start());
  ASSERT_FALSE(base_occ->pusch_cfg.rbs.type1().overlaps(blocked_vrbs))
      << fmt::format("PUSCH VRBs {} overlap the VRBs {} already busy in the slot of occasion {}",
                     base_occ->pusch_cfg.rbs.type1(),
                     blocked_vrbs,
                     target_occasion_offset);

  // Every occasion must be present, and must reuse the base grant's RBs.
  for (unsigned offset = 1; offset != nof_reps; ++offset) {
    const ul_sched_info* occ = find_ue_pusch(u.crnti, res_grid[pusch_slot + offset].result_reader().ul().puschs());
    ASSERT_NE(occ, nullptr) << "Occasion at offset " << offset
                            << " was dropped: the bundle was sized against the base slot only";
    ASSERT_EQ(occ->pusch_cfg.rbs.type1(), base_occ->pusch_cfg.rbs.type1())
        << "Occasion at offset " << offset << " does not repeat the base grant's RBs";
  }
}

// The base slot is refused if the UE cannot transmit in it, and an occasion's slot has to be held to the same rule.
// A UE inside an uplink measurement gap leaves the uplink to measure (TS 38.133, Section 9.1C.2), while still
// counting the slot as available for repetition -- that count follows the semi-static UL/DL configuration alone --
// so the occasion is simply lost and the ones after it do not move up. The bundle must give way instead.
TEST_P(ue_grid_allocator_pusch_repetition_test, bundle_whose_occasion_falls_in_an_ul_meas_gap_gives_way_to_a_single_tx)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  // Reach a known, ready slot first: every occasion's slot is then known in advance.
  slot_indication();

  // Place a 6ms measurement gap on the slot of occasion 3. With no T_TA tracked the uplink window is neither shifted
  // nor guarded, so it covers the gap offset and the 6 subframes after it -- at the 15kHz of this suite, one slot per
  // subframe, which leaves the base slot and the earlier occasions outside.
  constexpr unsigned target_occasion_offset = 3;
  const uint8_t      common_k2 = cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list[0].k2;
  const slot_point   gap_slot  = current_slot + common_k2 + target_occasion_offset;
  const unsigned     gap_offset =
      (gap_slot.count() / gap_slot.nof_slots_per_subframe()) % static_cast<unsigned>(meas_gap_repetition_period::ms80);

  const ue& u = add_repetition_ue(meas_gap_config{gap_offset, meas_gap_length::ms6, meas_gap_repetition_period::ms80});
  ue_cell&  ue_cc             = ues[u.ue_index].get_pcell();
  const slot_point pusch_slot = current_slot + rep_k2;
  ASSERT_EQ(pusch_slot + target_occasion_offset, gap_slot);
  ASSERT_TRUE(ue_cc.is_ul_enabled(pusch_slot)) << "the base slot itself fell in the gap, the test proves nothing";
  ASSERT_FALSE(ue_cc.is_ul_enabled(gap_slot));

  set_pusch_snr(ue_cc, 0.0F);
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  const std::optional<unsigned> time_resource = current_ul_dci_time_resource(u.crnti);
  ASSERT_TRUE(time_resource.has_value());
  ASSERT_NE(time_resource.value(), rep_time_resource)
      << "a bundle was scheduled over a slot the UE spends in a measurement gap";

  // A single transmission occupies its own slot alone, so no occasion reaches the grid.
  ASSERT_NE(find_ue_pusch(u.crnti, res_grid[pusch_slot].result_reader().ul().puschs()), nullptr);
  for (unsigned offset = 1; offset != nof_reps; ++offset) {
    ASSERT_EQ(find_ue_pusch(u.crnti, res_grid[pusch_slot + offset].result_reader().ul().puschs()), nullptr)
        << "Occasion at offset " << offset << " was scheduled for a grant downgraded to a single transmission";
  }
}

// Likewise for a slot the UE holds for a Configured Grant: the base slot never takes one (the UL scheduling context
// gives up on it), and an occasion must not either, or the dynamic bundle and the UE's own CG PUSCH would land in the
// same slot.
TEST_P(ue_grid_allocator_pusch_repetition_test, bundle_whose_occasion_falls_on_a_cg_slot_gives_way_to_a_single_tx)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  slot_indication();

  // Hold the slot of occasion 3 for a Configured Grant of this UE.
  constexpr unsigned target_occasion_offset = 3;
  const uint8_t      common_k2 = cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list[0].k2;
  const slot_point   cg_slot   = current_slot + common_k2 + target_occasion_offset;

  const ue&        u          = add_cg_repetition_ue(cg_slot);
  ue_cell&         ue_cc      = ues[u.ue_index].get_pcell();
  const slot_point pusch_slot = current_slot + rep_k2;
  ASSERT_EQ(pusch_slot + target_occasion_offset, cg_slot);

  ASSERT_FALSE(ue_cc.get_conf_grant_state_manager().is_cg_slot(pusch_slot))
      << "the base slot itself is a CG slot, the test proves nothing";
  ASSERT_TRUE(ue_cc.get_conf_grant_state_manager().is_cg_slot(cg_slot));

  set_pusch_snr(ue_cc, 0.0F);
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  const std::optional<unsigned> time_resource = current_ul_dci_time_resource(u.crnti);
  ASSERT_TRUE(time_resource.has_value());
  ASSERT_NE(time_resource.value(), rep_time_resource)
      << "a bundle was scheduled over a slot the UE holds for a Configured Grant";

  // A single transmission occupies its own slot alone, so no occasion reaches the grid.
  ASSERT_NE(find_ue_pusch(u.crnti, res_grid[pusch_slot].result_reader().ul().puschs()), nullptr);
  for (unsigned offset = 1; offset != nof_reps; ++offset) {
    ASSERT_EQ(find_ue_pusch(u.crnti, res_grid[pusch_slot + offset].result_reader().ul().puschs()), nullptr)
        << "Occasion at offset " << offset << " was scheduled for a grant downgraded to a single transmission";
  }
}

/// The repetition fixture in an NTN cell whose uplink reaches the gNB k_mac slots after the UE transmits it.
class ue_grid_allocator_pusch_repetition_ntn_kmac_test : public ue_grid_allocator_pusch_repetition_test
{
protected:
  /// k_mac of the cell. At the 15kHz of this suite, one slot per subframe.
  static constexpr std::chrono::milliseconds ntn_k_mac{19};

  ue_grid_allocator_pusch_repetition_ntn_kmac_test() :
    ue_grid_allocator_pusch_repetition_test(crb_interval{10, 40}, /*force_rep=*/false, ntn_k_mac)
  {
  }
};

// The UE holds its Configured Grant occasions in its own uplink slot numbering, while the grid this scheduler fills is
// on the gNB downlink clock: the occasion that collides with the UE's CG PUSCH is the one whose uplink slot the UE
// holds, k_mac before the grid slot the bundle would occupy.
TEST_P(ue_grid_allocator_pusch_repetition_ntn_kmac_test, bundle_whose_occasion_falls_on_a_cg_uplink_slot_gives_way)
{
  slot_indication();

  // The repetition row of the UE carries the k2 of the first common resource, and the bundle it starts reaches the gNB
  // k_mac slots later. The slot has to be known before the UE exists, because the CG occasion is registered with it.
  constexpr unsigned target_occasion_offset = 3;
  const uint8_t      common_k2 = cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list[0].k2;
  const slot_point   occasion_slot =
      current_slot + cell_cfg.ul_delay(common_k2 + cell_cfg.ntn_cs_koffset) + target_occasion_offset;

  // Hold the uplink slot the UE transmits that occasion in for a Configured Grant of this UE.
  const ue& u     = add_cg_repetition_ue(cell_cfg.dl_to_ue_ul_slot(occasion_slot));
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  const slot_point pusch_slot = current_slot + cell_cfg.ul_delay(rep_k2 + cell_cfg.ntn_cs_koffset);
  ASSERT_EQ(pusch_slot + target_occasion_offset, occasion_slot);
  ASSERT_EQ(cell_cfg.ntn_k_mac_slots,
            static_cast<unsigned>(ntn_k_mac.count()) * get_nof_slots_per_subframe(cell_cfg.scs_common()));
  ASSERT_TRUE(ue_cc.get_conf_grant_state_manager().is_cg_slot(cell_cfg.dl_to_ue_ul_slot(occasion_slot)));
  ASSERT_FALSE(ue_cc.get_conf_grant_state_manager().is_cg_slot(occasion_slot))
      << "the grid slot of the occasion is a CG slot too, so the test cannot tell the two clocks apart";
  ASSERT_FALSE(ue_cc.get_conf_grant_state_manager().is_cg_slot(cell_cfg.dl_to_ue_ul_slot(pusch_slot)))
      << "the base slot itself is a CG slot, the test proves nothing";

  set_pusch_snr(ue_cc, 0.0F);
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  const std::optional<unsigned> time_resource = current_ul_dci_time_resource(u.crnti);
  ASSERT_TRUE(time_resource.has_value());
  ASSERT_NE(time_resource.value(), rep_time_resource)
      << "a bundle was scheduled over an uplink slot the UE holds for a Configured Grant";

  // A single transmission occupies its own slot alone, so no occasion reaches the grid.
  ASSERT_NE(find_ue_pusch(u.crnti, res_grid[pusch_slot].result_reader().ul().puschs()), nullptr);
  for (unsigned offset = 1; offset != nof_reps; ++offset) {
    ASSERT_EQ(find_ue_pusch(u.crnti, res_grid[pusch_slot + offset].result_reader().ul().puschs()), nullptr)
        << "Occasion at offset " << offset << " was scheduled for a grant downgraded to a single transmission";
  }
}

// The reTx counterpart of the test above, and a stricter one: a reTx must repeat the original transmission's RB
// count exactly, so it cannot shrink around the busy RBs the way a newTx can -- it has to move aside as a whole.
TEST_P(ue_grid_allocator_pusch_repetition_test, retx_bundle_avoids_rbs_busy_in_an_occasion_slot)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  const ue& u     = add_repetition_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  slot_indication();

  // Low SINR: the newTx is scheduled as a bundle. Cap its RBs, so that the reTx, which needs the very same number of
  // them, still has room to land elsewhere in the UE's PUSCH CRB window ({10, 40}).
  constexpr unsigned nof_grant_rbs = 10;
  set_pusch_snr(ue_cc, 0.0F);
  ASSERT_EQ(allocate_ul_newtx_grant(current_slot + rep_k2, slice_ues[u.ue_index], units::bytes{1000}, nof_grant_rbs),
            alloc_status::success);
  const ul_sched_info* newtx_grant =
      find_ue_pusch(u.crnti, res_grid[current_slot + rep_k2].result_reader().ul().puschs());
  ASSERT_NE(newtx_grant, nullptr);
  ASSERT_EQ(newtx_grant->pusch_cfg.rbs.type1().length(), nof_grant_rbs);

  // NACK the bundle and step past it, so the reTx is searched on a grid clear of the original transmission.
  std::optional<ul_harq_process_handle> h_ul = ue_cc.harqs.find_ul_harq_waiting_ack();
  ASSERT_TRUE(h_ul.has_value());
  ASSERT_TRUE(h_ul->ul_crc_info(false).has_value());
  for (unsigned i = 0; i != nof_reps; ++i) {
    slot_indication();
  }
  std::optional<ul_harq_process_handle> h_retx = ue_cc.harqs.find_pending_ul_retx();
  ASSERT_TRUE(h_retx.has_value());

  // Occupy, in the slot of occasion 3, the RBs at the bottom of the UE's PUSCH CRB window -- exactly where the reTx
  // would land, its own slot being free.
  constexpr unsigned target_occasion_offset = 3;
  const slot_point   pusch_slot             = current_slot + rep_k2;
  const crb_interval blocked_crbs{10, 10 + nof_grant_rbs};
  res_grid[pusch_slot + target_occasion_offset].ul_res_grid.fill(
      grant_info{cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs, ofdm_symbol_range{0, 14}, blocked_crbs});
  const vrb_interval blocked_vrbs = rb_helper::crb_to_vrb_ul_non_interleaved(
      blocked_crbs, cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.start());
  ASSERT_EQ(newtx_grant->pusch_cfg.rbs.type1(), blocked_vrbs)
      << "the blocked RBs are not the ones the reTx would pick, so the test would pass without moving anything";

  allocate_ul_retx_grant(slice_ues[u.ue_index], *h_retx);

  const ul_sched_info* retx_grant = find_ue_pusch(u.crnti, res_grid[pusch_slot].result_reader().ul().puschs());
  ASSERT_NE(retx_grant, nullptr) << "the reTx bundle was not scheduled at all";
  ASSERT_EQ(retx_grant->context.nof_retxs, 1);
  ASSERT_EQ(retx_grant->pusch_cfg.rbs.type1().length(), nof_grant_rbs);
  ASSERT_FALSE(retx_grant->pusch_cfg.rbs.type1().overlaps(blocked_vrbs))
      << fmt::format("reTx VRBs {} overlap the VRBs {} already busy in the slot of occasion {}",
                     retx_grant->pusch_cfg.rbs.type1(),
                     blocked_vrbs,
                     target_occasion_offset);

  // Every occasion must be present, and must reuse the reTx grant's RBs.
  for (unsigned offset = 1; offset != nof_reps; ++offset) {
    const ul_sched_info* occ = find_ue_pusch(u.crnti, res_grid[pusch_slot + offset].result_reader().ul().puschs());
    ASSERT_NE(occ, nullptr) << "Occasion at offset " << offset
                            << " was dropped: the reTx bundle was sized against the base slot only";
    ASSERT_EQ(occ->pusch_cfg.rbs.type1(), retx_grant->pusch_cfg.rbs.type1())
        << "Occasion at offset " << offset << " does not repeat the reTx grant's RBs";
  }
}

// The UE multiplexes in each slot of a bundle the HARQ-ACKs booked for that slot, and the single UL DAI of the
// bundle's only DCI applies to every one of those slots alike (TS 38.213, clause 9). Slots whose codebooks need
// different DAI values therefore cannot be served by one bundle: the grant gives way to a single transmission, which
// answers for its own slot only. Scheduling the bundle anyway would size the UE's codebook wrongly in at least one
// slot, and the UCI there would be lost silently.
TEST_P(ue_grid_allocator_pusch_repetition_test, bundle_whose_slots_need_different_ul_dai_gives_way_to_a_single_tx)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  const ue& u     = add_repetition_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  // Reach a known, ready slot first: with no run_until below, every occasion's slot is then known in advance.
  slot_indication();

  const slot_point pusch_slot = current_slot + rep_k2;
  // One HARQ-ACK on the base occasion's slot and two on occasion 3's: their codebooks need DAI values
  // (1 - 1) % 4 == 0 and (2 - 1) % 4 == 1, which one DCI cannot carry at once.
  constexpr unsigned           target_occasion_offset = 3;
  const std::array<uint8_t, 1> base_k1                = {rep_k2};
  const std::array<uint8_t, 1> occasion_k1            = {static_cast<uint8_t>(rep_k2 + target_occasion_offset)};
  ASSERT_TRUE(uci_alloc.alloc_harq_ack(res_grid, ue_cc, 0, base_k1, pucch_repetition_factor::n1).has_value());
  for (unsigned i = 0; i != 2; ++i) {
    ASSERT_TRUE(uci_alloc.alloc_harq_ack(res_grid, ue_cc, 0, occasion_k1, pucch_repetition_factor::n1).has_value());
  }
  const slot_point occasion_slot = pusch_slot + target_occasion_offset;
  ASSERT_EQ(uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(pusch_slot, u.crnti), 1);
  ASSERT_EQ(uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(occasion_slot, u.crnti), 2);

  set_pusch_snr(ue_cc, 0.0F);
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  const std::optional<unsigned> time_resource = current_ul_dci_time_resource(u.crnti);
  ASSERT_TRUE(time_resource.has_value());
  ASSERT_NE(time_resource.value(), rep_time_resource)
      << "A bundle was scheduled over slots that cannot share one UL DAI";

  // A single transmission occupies its own slot alone, so no occasion reaches the grid.
  ASSERT_NE(find_ue_pusch(u.crnti, res_grid[pusch_slot].result_reader().ul().puschs()), nullptr);
  for (unsigned offset = 1; offset != nof_reps; ++offset) {
    ASSERT_EQ(find_ue_pusch(u.crnti, res_grid[pusch_slot + offset].result_reader().ul().puschs()), nullptr)
        << "Occasion at offset " << offset << " was scheduled for a grant downgraded to a single transmission";
  }
}

// The counterpart of the test above: slots agreeing on the UL DAI keep their bundle. Together they pin down that it
// is the DAI that decides, not the mere presence of HARQ-ACK in several slots of the bundle.
TEST_P(ue_grid_allocator_pusch_repetition_test, bundle_whose_slots_agree_on_the_ul_dai_is_scheduled)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  const ue& u     = add_repetition_ue();
  ue_cell&  ue_cc = ues[u.ue_index].get_pcell();

  slot_indication();

  const slot_point pusch_slot = current_slot + rep_k2;
  // Two HARQ-ACKs in each of the two slots: both codebooks need a DAI of (2 - 1) % 4 == 1.
  constexpr unsigned           target_occasion_offset = 3;
  const std::array<uint8_t, 1> base_k1                = {rep_k2};
  const std::array<uint8_t, 1> occasion_k1            = {static_cast<uint8_t>(rep_k2 + target_occasion_offset)};
  for (unsigned i = 0; i != 2; ++i) {
    ASSERT_TRUE(uci_alloc.alloc_harq_ack(res_grid, ue_cc, 0, base_k1, pucch_repetition_factor::n1).has_value());
    ASSERT_TRUE(uci_alloc.alloc_harq_ack(res_grid, ue_cc, 0, occasion_k1, pucch_repetition_factor::n1).has_value());
  }
  const slot_point occasion_slot = pusch_slot + target_occasion_offset;
  ASSERT_EQ(uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(pusch_slot, u.crnti), 2);
  ASSERT_EQ(uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(occasion_slot, u.crnti), 2);

  set_pusch_snr(ue_cc, 0.0F);
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  const std::optional<unsigned> time_resource = current_ul_dci_time_resource(u.crnti);
  ASSERT_TRUE(time_resource.has_value());
  ASSERT_EQ(time_resource.value(), rep_time_resource)
      << "The bundle was given up even though its slots agree on the UL DAI";
  for (unsigned offset = 1; offset != nof_reps; ++offset) {
    ASSERT_NE(find_ue_pusch(u.crnti, res_grid[pusch_slot + offset].result_reader().ul().puschs()), nullptr)
        << "Occasion at offset " << offset << " is missing from the bundle";
  }

  // The DAI describes one slot's codebook, which both slots share -- not their sum.
  const pdcch_ul_information* ul_pdcch = nullptr;
  for (const pdcch_ul_information& pdcch : res_grid[0].result_reader().dl().ul_pdcchs()) {
    if (pdcch.ctx.rnti == u.crnti) {
      ul_pdcch = &pdcch;
      break;
    }
  }
  ASSERT_NE(ul_pdcch, nullptr);
  ASSERT_EQ(ul_pdcch->dci.as_c_rnti_f0_1().first_dl_assignment_index, 1)
      << "The UL DAI does not describe the 2 HARQ-ACK bits each slot of the bundle reports";
}

// Same setup, but with the UE PUSCH free to span the whole BWP, as in a real deployment. Every repetition occasion
// then overlaps in RBs with the UE's own PUCCH resources, which sit at the BWP edges.
class ue_grid_allocator_pusch_repetition_wideband_test : public ue_grid_allocator_pusch_repetition_test
{
protected:
  ue_grid_allocator_pusch_repetition_wideband_test() :
    ue_grid_allocator_pusch_repetition_test(crb_interval{0, MAX_NOF_PRBS})
  {
  }
};

// Regression: with the PUSCH free to span the BWP, a PUCCH in an occasion slot used to cost that occasion -- the
// grant was sized against the base slot alone, so its PRBs ran into the PUCCH and the occasion was dropped. The
// grant now routes around everything busy in every slot of the bundle.
TEST_P(ue_grid_allocator_pusch_repetition_wideband_test, pucch_in_an_occasion_slot_does_not_break_the_bundle)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  const ue&                    u           = add_repetition_ue();
  ue_cell&                     ue_cc       = ues[u.ue_index].get_pcell();
  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();

  slot_indication();

  constexpr unsigned            target_occasion_offset = 3;
  const std::optional<unsigned> pucch_res_ind =
      pucch_alloc.alloc_ded_harq_ack(res_grid, ue_cell_cfg, 0, rep_k2 + target_occasion_offset, 0);
  ASSERT_TRUE(pucch_res_ind.has_value());
  const slot_point occasion_slot = current_slot + rep_k2 + target_occasion_offset;
  ASSERT_FALSE(res_grid[occasion_slot].result_reader().ul().pucchs().empty());

  set_pusch_snr(ue_cc, 0.0F);
  const slot_point pusch_slot = current_slot + rep_k2;
  ASSERT_EQ(allocate_ul_newtx_grant(pusch_slot, slice_ues[u.ue_index], units::bytes{1000}), alloc_status::success);

  // The base grant must be wide enough to overlap the PUCCH, or this degenerates into the narrowband test.
  const ul_sched_info* base_occ = find_ue_pusch(u.crnti, res_grid[pusch_slot].result_reader().ul().puschs());
  ASSERT_NE(base_occ, nullptr);
  ASSERT_GT(base_occ->pusch_cfg.rbs.type1().length(), 40U) << "PUSCH is too narrow to exercise the RB overlap";

  ASSERT_TRUE(res_grid[occasion_slot].result_reader().ul().pucchs().empty());
  const ul_sched_info* occ = find_ue_pusch(u.crnti, res_grid[occasion_slot].result_reader().ul().puschs());
  ASSERT_NE(occ, nullptr) << "The occasion overlapping the UE's own PUCCH in RBs was dropped";
  ASSERT_TRUE(occ->uci.has_value());
  ASSERT_TRUE(occ->uci->harq.has_value());
  ASSERT_GT(occ->uci->harq->harq_ack_nof_bits, 0);
}

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_pusch_repetition_wideband_test,
                         testing::Values(duplex_mode::FDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_pusch_repetition_test,
                         testing::Values(duplex_mode::FDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_pusch_repetition_ntn_kmac_test,
                         testing::Values(duplex_mode::FDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_css_test,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_default_cfg_test,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_expert_cfg_pxsch_nof_rbs_limits_tester,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_expert_cfg_pxsch_crb_limits_tester,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));

/// \brief Allocation of PDSCHs in slots with CSI-RS.
///
/// The PDSCH is rate-matched around the CSI-RS of its slot, while the TBS does not account for them (TS 38.214, Section
/// 5.1.3.2). The UE may skip decoding a transport block in an initial transmission whose effective code rate exceeds
/// 0.95 (TS 38.214, Section 5.1.3.1). The UEs are scheduled with the highest MCS of their MCS table.
class ue_grid_allocator_csi_rs_test : public ue_grid_allocator_test
{
protected:
  /// Maximum effective channel code rate of an initial transmission, as per TS 38.214, Section 5.1.3.1.
  static constexpr double max_effective_code_rate = 0.95;

  ue_grid_allocator_csi_rs_test(pdsch_mcs_table mcs_table_, sch_mcs_index max_mcs) :
    ue_grid_allocator_test(test_params{[max_mcs]() {
      scheduler_expert_config cfg = config_helpers::make_default_scheduler_expert_config();
      cfg.ue.dl_mcs               = {max_mcs, max_mcs};
      return cfg;
    }()}),
    mcs_table(mcs_table_)
  {
  }

  const ue& add_ue(pdsch_config::resource_allocation res_alloc)
  {
    sched_ue_creation_request_message req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);
    req.ue_index                                                 = to_du_ue_index(0);
    req.crnti                                                    = to_rnti(0x4601);
    (*req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdcch_cfg = cell_cfg.bwp_res[to_bwp_id(0)].dl().ded_pdcchs[0];
    pdsch_config& pdsch_cfg = (*req.cfg.cells)[0].ded->serv_cell_cfg.init_dl_bwp.pdsch_cfg.value();
    pdsch_cfg.mcs_table     = mcs_table;
    pdsch_cfg.res_alloc     = res_alloc;
    pdsch_cfg.rbg_sz        = rbg_size::config1;
    return ue_grid_allocator_test::add_ue(req);
  }

  /// \brief CSI-RS of a CSI measurement slot of a cell with eight antenna ports.
  ///
  /// An eight-port NZP-CSI-RS and a ZP-CSI-RS with the same resource mapping in another OFDM symbol, as the cell
  /// configuration helpers generate them (TS 38.211, Table 7.4.1.5.3-1, row 6): eight resource elements per PRB each.
  std::vector<csi_rs_info> make_eight_port_csi_rs() const
  {
    csi_rs_info csi_rs{};
    csi_rs.bwp_cfg     = &cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params;
    csi_rs.crbs        = csi_rs.bwp_cfg->crbs;
    csi_rs.type        = csi_rs_type::CSI_RS_NZP;
    csi_rs.row         = 6;
    csi_rs.freq_domain = csi_rs::freq_allocation_mask_type(6);
    csi_rs.freq_domain.fill(0, 4);
    csi_rs.symbol0      = 4;
    csi_rs.symbol1      = 2;
    csi_rs.cdm_type     = csi_rs_cdm_type::fd_CDM2;
    csi_rs.freq_density = csi_rs_freq_density_type::one;

    csi_rs_info zp_csi_rs = csi_rs;
    zp_csi_rs.type        = csi_rs_type::CSI_RS_ZP;
    zp_csi_rs.symbol0     = 8;
    return {csi_rs, zp_csi_rs};
  }

  /// Single-port NZP-CSI-RS (TS 38.211, Table 7.4.1.5.3-1, row 2): one resource element per PRB.
  std::vector<csi_rs_info> make_single_port_csi_rs() const
  {
    csi_rs_info csi_rs{};
    csi_rs.bwp_cfg     = &cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params;
    csi_rs.crbs        = csi_rs.bwp_cfg->crbs;
    csi_rs.type        = csi_rs_type::CSI_RS_NZP;
    csi_rs.row         = 2;
    csi_rs.freq_domain = csi_rs::freq_allocation_mask_type(12);
    csi_rs.freq_domain.set(0);
    csi_rs.symbol0      = 4;
    csi_rs.symbol1      = 2;
    csi_rs.cdm_type     = csi_rs_cdm_type::no_CDM;
    csi_rs.freq_density = csi_rs_freq_density_type::one;
    return {csi_rs};
  }

  /// \brief Checks the PDSCH resource elements per PRB, from which the expected MCSs are derived.
  ///
  /// Twelve OFDM symbols with three DM-RS symbols of type 1 and one CDM group without data: 144 - 18 = 126 resource
  /// elements per PRB.
  static void check_nof_re_per_prb(const pdsch_information& pdsch)
  {
    ASSERT_EQ(pdsch.symbols.length(), 12);
    ASSERT_EQ(pdsch.dmrs.dmrs_symb_pos.count(), 3);
    ASSERT_EQ(pdsch.dmrs.config_type, dmrs_config_type::type1);
    ASSERT_EQ(pdsch.dmrs.num_dmrs_cdm_grps_no_data, 1);
  }

  /// \brief Schedules a newTx PDSCH for the UE in the next DL slot, in which the given CSI-RS are transmitted.
  ///
  /// \param[in] u                 UE.
  /// \param[in] csi_rs_list       CSI-RS transmitted in the slot of the PDSCH.
  /// \param[in] use_odd_rbgs_only Set to true to mark a VRB of every even RBG as used, so that a resource allocation
  ///                              type 0 grant is not contiguous.
  /// \return The PDSCH grant of the UE, if it was scheduled.
  const dl_msg_alloc*
  allocate_pdsch_with_csi_rs(const ue& u, span<const csi_rs_info> csi_rs_list, bool use_odd_rbgs_only = false)
  {
    const crb_interval     bwp_crbs = cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.crbs;
    const nominal_rbg_size P        = get_nominal_rbg_size(bwp_crbs.length(), rbg_size::config1);
    const bool             found    = run_until(
        [&]() {
          if (not cell_cfg.is_dl_enabled(current_slot)) {
            return;
          }
          auto csi_rs_pdus = res_grid[0].result_builder().dl().csi_rs();
          for (const csi_rs_info& csi_rs : csi_rs_list) {
            report_fatal_error_if_not(csi_rs_pdus.emplace_back(csi_rs) != nullptr,
                                      "The CSI-RS list of the slot is full");
          }
          for (unsigned rbg_idx = 0, nof_rbgs = get_nof_rbgs(bwp_crbs, P); use_odd_rbgs_only and rbg_idx < nof_rbgs;
               rbg_idx += 2) {
            rbg_bitmap rbg(nof_rbgs);
            rbg.set(rbg_idx);
            used_dl_vrbs.set(convert_rbgs_to_vrbs(rbg, bwp_crbs, P).find_lowest());
          }
          allocate_dl_newtx_grant(slice_ues[u.ue_index], units::bytes{100000}, false);
        },
        [&]() { return find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) != nullptr; });
    return found ? find_ue_pdsch(u.crnti, res_grid[0].result_reader().dl().ue_grants()) : nullptr;
  }

  /// \brief Effective channel code rate of a PDSCH, as per TS 38.214, Section 5.1.3.1.
  ///
  /// The number of physical channel bits is derived from the resource elements to which the PHY maps the PDSCH: all the
  /// resource elements of its PRBs and OFDM symbols, except the DM-RS ones (CDM groups without data included) and the
  /// ones of every CSI-RS of the slot, all ports included. The test does not use VRB-to-PRB interleaving.
  static double get_effective_code_rate(const pdsch_information& pdsch, span<const csi_rs_info> csi_rs_list)
  {
    // Mark the resource elements of the slot occupied by CSI-RS.
    std::array<bounded_bitset<MAX_NOF_PRBS * NOF_SUBCARRIERS_PER_RB>, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP> csi_rs_grid;
    for (auto& symbol_res : csi_rs_grid) {
      symbol_res.resize(MAX_NOF_PRBS * NOF_SUBCARRIERS_PER_RB);
    }
    for (const csi_rs_info& csi_rs : csi_rs_list) {
      csi_rs_pattern_configuration pattern_cfg{.start_rb                 = csi_rs.crbs.start(),
                                               .nof_rb                   = csi_rs.crbs.length(),
                                               .csi_rs_mapping_table_row = csi_rs.row,
                                               .freq_allocation_ref_idx  = {},
                                               .symbol_l0                = csi_rs.symbol0,
                                               .symbol_l1                = csi_rs.symbol1,
                                               .cdm                      = csi_rs.cdm_type,
                                               .freq_density             = csi_rs.freq_density};
      csi_rs::convert_freq_domain(pattern_cfg.freq_allocation_ref_idx, csi_rs.freq_domain, csi_rs.row);
      const csi_rs_pattern pattern = get_csi_rs_pattern(pattern_cfg);
      for (const csi_rs_pattern_port& port : pattern.prb_patterns) {
        for (unsigned symbol = 0; symbol != NOF_OFDM_SYM_PER_SLOT_NORMAL_CP; ++symbol) {
          if (not port.symbol_mask.test(symbol)) {
            continue;
          }
          for (unsigned crb = pattern.rb_begin; crb < pattern.rb_end; crb += pattern.rb_stride) {
            port.re_mask.for_each(0, NOF_SUBCARRIERS_PER_RB, [&](size_t re) {
              csi_rs_grid[symbol].set(crb * NOF_SUBCARRIERS_PER_RB + re);
            });
          }
        }
      }
    }

    // CRBs of the PDSCH.
    const crb_interval bwp_crbs = pdsch.bwp_cfg->crbs;
    prb_bitmap         prbs(bwp_crbs.length());
    if (pdsch.rbs.is_type0()) {
      prbs = pdsch.rbs.type0().convert_to<prb_bitmap>();
    } else {
      prbs |= pdsch.rbs.type1().convert_to<prb_interval>();
    }
    const crb_bitmap crbs = prb_to_crb(bwp_crbs, prbs);

    // Count the resource elements of the PDSCH.
    const unsigned nof_dmrs_re_per_rb =
        pdsch.dmrs.num_dmrs_cdm_grps_no_data * get_nof_re_per_prb(pdsch.dmrs.config_type);
    unsigned nof_re = 0;
    for (unsigned symbol = pdsch.symbols.start(); symbol != pdsch.symbols.stop(); ++symbol) {
      const bool is_dmrs_symbol = pdsch.dmrs.dmrs_symb_pos.test(symbol);
      crbs.for_each(0, crbs.size(), [&](size_t crb) {
        const unsigned nof_csi_rs_re =
            csi_rs_grid[symbol].slice(crb * NOF_SUBCARRIERS_PER_RB, (crb + 1) * NOF_SUBCARRIERS_PER_RB).count();
        // The PHY does not accept CSI-RS in OFDM symbols with DM-RS.
        EXPECT_FALSE(is_dmrs_symbol and nof_csi_rs_re != 0);
        nof_re += NOF_SUBCARRIERS_PER_RB - (is_dmrs_symbol ? nof_dmrs_re_per_rb : nof_csi_rs_re);
      });
    }

    // Information bits, CRC included, over physical channel bits.
    const pdsch_codeword& cw     = pdsch.codewords.front();
    const sch_information sch    = get_sch_segmentation_info(units::bytes{cw.tb_size_bytes}.to_bits(),
                                                          cw.mcs_descr.get_normalised_target_code_rate());
    const unsigned nof_info_bits = (sch.nof_bits_per_cb.value() - sch.nof_filler_bits_per_cb.value()) * sch.nof_cb;
    const unsigned nof_ch_bits   = nof_re * pdsch.nof_layers * get_bits_per_symbol(cw.mcs_descr.modulation);
    return static_cast<double>(nof_info_bits) / nof_ch_bits;
  }

  const pdsch_mcs_table mcs_table;
};

class ue_grid_allocator_csi_rs_qam64_test : public ue_grid_allocator_csi_rs_test
{
protected:
  /// TS 38.214, Table 5.1.3.1-1, MCS 28: 64QAM with a target code rate of 948/1024.
  ue_grid_allocator_csi_rs_qam64_test() : ue_grid_allocator_csi_rs_test(pdsch_mcs_table::qam64, sch_mcs_index{28}) {}
};

TEST_P(ue_grid_allocator_csi_rs_qam64_test, pdsch_rate_matched_around_csi_rs_does_not_exceed_max_effective_code_rate)
{
  const ue&                      u           = add_ue(pdsch_config::resource_allocation::resource_allocation_type_1);
  const std::vector<csi_rs_info> csi_rs_list = make_eight_port_csi_rs();

  const dl_msg_alloc* grant = allocate_pdsch_with_csi_rs(u, csi_rs_list);
  ASSERT_NE(grant, nullptr);
  ASSERT_LE(get_effective_code_rate(grant->pdsch_cfg, csi_rs_list), max_effective_code_rate);
}

TEST_P(ue_grid_allocator_csi_rs_qam64_test,
       non_contiguous_pdsch_rate_matched_around_csi_rs_does_not_exceed_max_effective_code_rate)
{
  const ue&                      u           = add_ue(pdsch_config::resource_allocation::resource_allocation_type_0);
  const std::vector<csi_rs_info> csi_rs_list = make_eight_port_csi_rs();

  const dl_msg_alloc* grant = allocate_pdsch_with_csi_rs(u, csi_rs_list, true);
  ASSERT_NE(grant, nullptr);
  ASSERT_TRUE(grant->pdsch_cfg.rbs.is_type0());
  ASSERT_LE(get_effective_code_rate(grant->pdsch_cfg, csi_rs_list), max_effective_code_rate);
}

class ue_grid_allocator_csi_rs_qam256_test : public ue_grid_allocator_csi_rs_test
{
protected:
  /// TS 38.214, Table 5.1.3.1-2, MCS 27: 256QAM with a target code rate of 948/1024.
  ue_grid_allocator_csi_rs_qam256_test() : ue_grid_allocator_csi_rs_test(pdsch_mcs_table::qam256, sch_mcs_index{27}) {}
};

TEST_P(ue_grid_allocator_csi_rs_qam256_test, pdsch_rate_matched_around_csi_rs_does_not_exceed_max_effective_code_rate)
{
  const ue&                      u           = add_ue(pdsch_config::resource_allocation::resource_allocation_type_1);
  const std::vector<csi_rs_info> csi_rs_list = make_eight_port_csi_rs();

  const dl_msg_alloc* grant = allocate_pdsch_with_csi_rs(u, csi_rs_list);
  ASSERT_NE(grant, nullptr);
  ASSERT_LE(get_effective_code_rate(grant->pdsch_cfg, csi_rs_list), max_effective_code_rate);
}

class ue_grid_allocator_csi_rs_qam256_mcs20_test : public ue_grid_allocator_csi_rs_test
{
protected:
  /// TS 38.214, Table 5.1.3.1-2, MCS 20: 256QAM with a spectral efficiency of 5.3320.
  ue_grid_allocator_csi_rs_qam256_mcs20_test() :
    ue_grid_allocator_csi_rs_test(pdsch_mcs_table::qam256, sch_mcs_index{20})
  {
  }
};

TEST_P(ue_grid_allocator_csi_rs_qam256_mcs20_test, mcs_is_lowered_according_to_the_csi_rs_overhead)
{
  const ue& u = add_ue(pdsch_config::resource_allocation::resource_allocation_type_1);

  const dl_msg_alloc* grant = allocate_pdsch_with_csi_rs(u, make_eight_port_csi_rs());
  ASSERT_NE(grant, nullptr);
  check_nof_re_per_prb(grant->pdsch_cfg);
  // 16 of the 126 resource elements per PRB are occupied by CSI-RS. MCS 17 (4.5234) is the highest MCS with a spectral
  // efficiency not above 5.3320 * 110 / 126 = 4.6549.
  ASSERT_EQ(grant->pdsch_cfg.codewords.front().mcs_index.value(), 17);
}

TEST_P(ue_grid_allocator_csi_rs_qam256_mcs20_test, mcs_is_lowered_with_any_csi_rs_overhead)
{
  const ue& u = add_ue(pdsch_config::resource_allocation::resource_allocation_type_1);

  const dl_msg_alloc* grant = allocate_pdsch_with_csi_rs(u, make_single_port_csi_rs());
  ASSERT_NE(grant, nullptr);
  check_nof_re_per_prb(grant->pdsch_cfg);
  // One of the 126 resource elements per PRB is occupied by CSI-RS. MCS 19 (5.1152) is the highest MCS with a spectral
  // efficiency not above 5.3320 * 125 / 126 = 5.2897.
  ASSERT_EQ(grant->pdsch_cfg.codewords.front().mcs_index.value(), 19);
}

class ue_grid_allocator_csi_rs_qam64_mcs17_test : public ue_grid_allocator_csi_rs_test
{
protected:
  /// TS 38.214, Table 5.1.3.1-1, MCS 17: 64QAM with a spectral efficiency of 2.5664, below that of MCS 16.
  ue_grid_allocator_csi_rs_qam64_mcs17_test() : ue_grid_allocator_csi_rs_test(pdsch_mcs_table::qam64, sch_mcs_index{17})
  {
  }
};

TEST_P(ue_grid_allocator_csi_rs_qam64_mcs17_test, mcs_with_a_higher_spectral_efficiency_is_skipped)
{
  const ue& u = add_ue(pdsch_config::resource_allocation::resource_allocation_type_1);

  const dl_msg_alloc* grant = allocate_pdsch_with_csi_rs(u, make_single_port_csi_rs());
  ASSERT_NE(grant, nullptr);
  check_nof_re_per_prb(grant->pdsch_cfg);
  // MCS 16 (16QAM, 2.5703) carries more bits per resource element than MCS 17 (64QAM, 2.5664). MCS 15 (2.3516) is the
  // highest MCS with a spectral efficiency not above 2.5664 * 125 / 126 = 2.5460.
  ASSERT_EQ(grant->pdsch_cfg.codewords.front().mcs_index.value(), 15);
}

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_csi_rs_qam256_mcs20_test,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_csi_rs_qam64_mcs17_test,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_csi_rs_qam64_test,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));

INSTANTIATE_TEST_SUITE_P(ue_grid_allocator_test,
                         ue_grid_allocator_csi_rs_qam256_test,
                         testing::Values(duplex_mode::FDD, duplex_mode::TDD));
