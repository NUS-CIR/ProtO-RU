// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/scheduler/config/sched_config_manager.h"
#include "lib/scheduler/support/sch_pdu_builder.h"
#include "lib/scheduler/ue_context/ue_cell_repository.h"
#include "lib/scheduler/ue_context/ue_repository.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "tests/unittests/scheduler/test_utils/config_generators.h"
#include "tests/unittests/scheduler/test_utils/dummy_test_components.h"
#include "ocudu/mac/mac_pdu_format.h"
#include "ocudu/scheduler/config/cg_builder_params.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include <gtest/gtest.h>
#include <limits>

using namespace ocudu;

/// Test suit to hold context which are required to test a UE cell implementation.
class sched_ue_test : public ::testing::Test
{
protected:
  /// \param[in] cg_cfg Configured Grant of the cell, if any. The UE gets one too when the cell has it.
  explicit sched_ue_test(const std::optional<cg_builder_params>& cg_cfg = std::nullopt) :
    expert_cfg(config_helpers::make_default_scheduler_expert_config()),
    cfg_mng(scheduler_config{expert_cfg, metric_notif}),
    sched_cfg([this, &cg_cfg]() {
      auto req                = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
      req.ran.init_bwp.cg_cfg = cg_cfg;
      return req;
    }()),
    cell_cfg(*cfg_mng.add_cell(sched_cfg)),
    serv_cell_cfg(config_helpers::make_default_ue_cell_config(cell_cfg.params).serv_cell_cfg),
    ue_db(expert_cfg.ue)
  {
    ue_db.register_cell(cell_ues);

    sched_ue_creation_request_message ue_req = sched_config_helper::create_default_sched_ue_creation_request(
        cell_cfg.params, std::array<lcid_t, 3>{lcid_t::LCID_SRB1, lcid_t::LCID_SRB2, lcid_t::LCID_MIN_DRB});
    ue_config_update_event  ev     = cfg_mng.add_ue(ue_req);
    const ue_configuration& ue_cfg = ev.next_config();
    ue_db.add_ue(ue_cfg, {sched_config_helper::to_ue_creation_mode(ue_req), ue_req.ul_ccch_slot_rx});
    ev.notify_completion();
  }

  pdsch_config_params get_pdsch_cfg_params(const ue_cell&                               ue_cc,
                                           const pdsch_time_domain_resource_allocation& pdsch_td_cfg,
                                           dci_dl_rnti_config_type                      dci_type)
  {
    switch (dci_type) {
      case dci_dl_rnti_config_type::c_rnti_f1_0:
        return sched_helper::get_pdsch_config_f1_0_c_rnti(cell_cfg, ue_cc.cfg().pdsch_serving_cell_cfg(), pdsch_td_cfg);
      case dci_dl_rnti_config_type::c_rnti_f1_1:
        return sched_helper::get_pdsch_config_f1_1_c_rnti(cell_cfg,
                                                          *ue_cc.cfg().bwp(to_bwp_id(0)).dl.pdsch().ded(),
                                                          ue_cc.cfg().pdsch_serving_cell_cfg(),
                                                          pdsch_td_cfg,
                                                          ue_cc.channel_state_manager().get_nof_dl_layers());
      default:
        report_fatal_error("Unsupported PDCCH DCI DL format");
    }
  }

  pusch_config_params get_pusch_cfg_params(const ue_cell&                               ue_cc,
                                           const pusch_time_domain_resource_allocation& pusch_td_cfg,
                                           dci_ul_rnti_config_type                      dci_type)
  {
    // In the following, we allocate extra bits to account for the possible UCI overhead. At this point, we don't
    // differentiate between HARQ-ACK bits and CSI bits, which would be necessary to compute the beta-offset values.
    // Here, we only need to allocate some extra space.
    const unsigned uci_bits_overallocation = 20U;
    const bool     is_csi_report_slot      = false;

    switch (dci_type) {
      case dci_ul_rnti_config_type::c_rnti_f0_0:
        return get_pusch_config_f0_0_c_rnti(cell_cfg,
                                            &ue_cc.cfg(),
                                            ue_cc.cfg().bwp(ue_cc.active_bwp_id()).ul.common(),
                                            pusch_td_cfg,
                                            uci_bits_overallocation,
                                            is_csi_report_slot);
      case dci_ul_rnti_config_type::c_rnti_f0_1:
        return get_pusch_config_f0_1_c_rnti(ue_cc.cfg(),
                                            pusch_td_cfg,
                                            ue_cc.channel_state_manager().get_nof_ul_layers(),
                                            uci_bits_overallocation,
                                            is_csi_report_slot);
      default:
        report_fatal_error("Unsupported PDCCH DCI UL format");
    }
  }

  scheduler_expert_config                  expert_cfg;
  sched_cfg_dummy_notifier                 metric_notif;
  cell_config_builder_params               builder_params;
  sched_config_manager                     cfg_mng;
  sched_cell_configuration_request_message sched_cfg;
  const cell_configuration&                cell_cfg;
  serving_cell_config                      serv_cell_cfg;
  ocudulog::basic_logger&                  logger = ocudulog::fetch_basic_logger("SCHED");
  ue_cell_repository                       cell_ues{cell_cfg, nullptr};
  ue_repository                            ue_db;
};

TEST_F(sched_ue_test, when_dl_nof_prb_allocated_increases_estimated_dl_rate_increases)
{
  auto&  ue_cc        = ue_db[to_du_ue_index(0)].get_pcell();
  double current_rate = 0;

  // We keep MCS constant for this test.
  const sch_mcs_index ue_mcs = 28;
  // Assume the DCI type to be format 1_1.
  const dci_dl_rnti_config_type dci_type = dci_dl_rnti_config_type::c_rnti_f1_1;

  for (unsigned nof_prbs = 0; nof_prbs < MAX_NOF_PRBS; ++nof_prbs) {
    for (const auto& pdsch_td_cfg :
         ue_cc.cfg().cell_cfg_common.params.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list) {
      const pdsch_config_params pdsch_cfg      = get_pdsch_cfg_params(ue_cc, pdsch_td_cfg, dci_type);
      const double              estimated_rate = ue_cc.get_estimated_dl_rate(pdsch_cfg, ue_mcs, nof_prbs);
      ASSERT_GE(estimated_rate, current_rate);
      current_rate = estimated_rate;
    }
  }
}

TEST_F(sched_ue_test, when_mcs_increases_estimated_dl_rate_increases)
{
  auto& ue_cc = ue_db[to_du_ue_index(0)].get_pcell();

  // Maximum MCS value for 64QAM MCS table.
  const sch_mcs_index max_mcs = 28;

  double current_rate = 0;

  // We keep nof. PRBs allocated constant for this test.
  const unsigned nof_prbs = 20;
  // Assume the DCI type to be format 1_1.
  const dci_dl_rnti_config_type dci_type = dci_dl_rnti_config_type::c_rnti_f1_1;

  for (sch_mcs_index ue_mcs = 1; ue_mcs < max_mcs; ++ue_mcs) {
    for (const auto& pdsch_td_cfg :
         ue_cc.cfg().cell_cfg_common.params.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list) {
      const pdsch_config_params pdsch_cfg      = get_pdsch_cfg_params(ue_cc, pdsch_td_cfg, dci_type);
      const double              estimated_rate = ue_cc.get_estimated_dl_rate(pdsch_cfg, ue_mcs, nof_prbs);
      // NOTE: In case of 64QAM MCS table MCS 17 has lower spectral density than MCS 16 but the estimated bitrate will
      // remain equal hence its required to use the check greater than or equal below.
      ASSERT_GE(estimated_rate, current_rate);
      current_rate = estimated_rate;
    }
  }
}

TEST_F(sched_ue_test, when_ul_nof_prb_allocated_increases_estimated_ul_rate_increases)
{
  auto&  ue_cc        = ue_db[to_du_ue_index(0)].get_pcell();
  double current_rate = 0;

  // We keep MCS constant for this test.
  const sch_mcs_index ue_mcs = 15;
  // Assume the DCI type to be format 0_1.
  const dci_ul_rnti_config_type dci_type = dci_ul_rnti_config_type::c_rnti_f0_1;

  for (unsigned nof_prbs = 0; nof_prbs < MAX_NOF_PRBS; ++nof_prbs) {
    for (const auto& pusch_td_cfg :
         ue_cc.cfg().cell_cfg_common.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list) {
      const pusch_config_params pusch_cfg      = get_pusch_cfg_params(ue_cc, pusch_td_cfg, dci_type);
      const double              estimated_rate = ue_cc.get_estimated_ul_rate(pusch_cfg, ue_mcs, nof_prbs);
      ASSERT_GE(estimated_rate, current_rate);
      current_rate = estimated_rate;
    }
  }
}

TEST_F(sched_ue_test, when_mcs_increases_estimated_ul_rate_increases)
{
  auto& ue_cc = ue_db[to_du_ue_index(0)].get_pcell();

  // Maximum MCS value for 64QAM MCS table.
  const sch_mcs_index max_mcs = 28;

  double current_rate = 0;

  // We keep nof. PRBs allocated constant for this test.
  const unsigned nof_prbs = 20;
  // Assume the DCI type to be format 0_1.
  const dci_ul_rnti_config_type dci_type = dci_ul_rnti_config_type::c_rnti_f0_1;

  for (sch_mcs_index ue_mcs = 1; ue_mcs < max_mcs; ++ue_mcs) {
    for (const auto& pusch_td_cfg :
         ue_cc.cfg().cell_cfg_common.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list) {
      const pusch_config_params pusch_cfg      = get_pusch_cfg_params(ue_cc, pusch_td_cfg, dci_type);
      const double              estimated_rate = ue_cc.get_estimated_ul_rate(pusch_cfg, ue_mcs, nof_prbs);
      // NOTE: In case of 64QAM MCS table MCS 17 has lower spectral density than MCS 16 but the estimated bitrate will
      // remain equal hence its required to use the check greater than or equal below.
      ASSERT_GE(estimated_rate, current_rate);
      current_rate = estimated_rate;
    }
  }
}

/// A CRC report for a PUSCH, together with whether the UE cell should take the PUSCH as transmitted.
struct crc_dtx_test_params {
  std::string          name;
  bool                 is_cg;
  bool                 crc_ok;
  std::optional<float> sinr_dB;
  /// False if the CRC should be taken as a DTX, i.e. the UE did not transmit.
  bool pusch_transmitted;
};

/// Formatter for CRC DTX params, used by gtest. Avoids the fallback raw-byte printer, which reads uninitialized padding
/// bytes.
void PrintTo(const crc_dtx_test_params& value, ::std::ostream* os)
{
  *os << value.name;
}

class ue_cell_crc_dtx_test : public sched_ue_test, public ::testing::WithParamInterface<crc_dtx_test_params>
{
protected:
  ue_cell_crc_dtx_test() : sched_ue_test(cg_builder_params{}) {}
};

TEST_P(ue_cell_crc_dtx_test, crc_ko_on_cg_pusch_is_dtx_only_when_the_sinr_is_low_or_invalid)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // The SINR values of the test cases are chosen around the default threshold.
  ASSERT_EQ(cell_cfg.expert_cfg.ue.cg_pusch_sinr_threshold_dB, -8.0F);

  ue_cell&    ue_cc  = ue_db[to_du_ue_index(0)].get_pcell();
  const auto* ul_ded = ue_cc.cfg().init_bwp().ul.ded();
  ASSERT_TRUE(ul_ded != nullptr and ul_ded->cg_cfg.has_value()) << "the UE has no CG configured";
  // The CG HARQ processes are only reserved by a reconfiguration, as a UE gets its CG together with its first DRB.
  ue_cc.handle_reconfiguration_request(ue_cc.cfg());

  // A CG PUSCH is never retransmitted, so its HARQ process is allocated with no retransmissions.
  const slot_point pdcch_slot{cell_cfg.scs_common(), 0};
  const slot_point pusch_slot = pdcch_slot + 4;
  const unsigned   max_retxs  = 0;
  const auto       h_ul =
      GetParam().is_cg
                ? ue_cc.harqs.alloc_ul_harq(pdcch_slot,
                                      pusch_slot,
                                      max_retxs,
                                      cg_harq_alloc_params{.harq_id = to_harq_id(0), .cg_harq_timeout = 160})
                : ue_cc.harqs.alloc_ul_harq(pdcch_slot, pusch_slot, max_retxs);
  ASSERT_TRUE(h_ul.has_value());
  ASSERT_EQ(h_ul->is_cg(), GetParam().is_cg);

  const ul_crc_pdu_indication crc{.rnti           = ue_cc.rnti(),
                                  .ue_index       = ue_cc.ue_index,
                                  .harq_id        = h_ul->id(),
                                  .tb_crc_success = GetParam().crc_ok,
                                  .ul_sinr_dB     = GetParam().sinr_dB};
  const auto                  outcome = ue_cc.handle_crc_pdu(pusch_slot, crc);
  ASSERT_TRUE(outcome.has_value()) << "the CRC was not attributed to the HARQ process of the PUSCH";
  EXPECT_EQ(outcome->is_cg, GetParam().is_cg);
  EXPECT_EQ(outcome->pusch_transmitted, GetParam().pusch_transmitted)
      << (GetParam().pusch_transmitted ? "the CRC was wrongly taken as a DTX" : "the CRC was not taken as a DTX");
  if (not GetParam().pusch_transmitted) {
    EXPECT_EQ(outcome->tbs, units::bytes{0U}) << "a DTX carries no TBS";
  }
}

INSTANTIATE_TEST_SUITE_P(
    crc_report,
    ue_cell_crc_dtx_test,
    ::testing::Values(
        crc_dtx_test_params{"cg_crc_ko_below_sinr_threshold", true, false, -20.0F, false},
        crc_dtx_test_params{"cg_crc_ko_infinite_sinr", true, false, std::numeric_limits<float>::infinity(), false},
        crc_dtx_test_params{"cg_crc_ko_nan_sinr", true, false, std::numeric_limits<float>::quiet_NaN(), false},
        crc_dtx_test_params{"cg_crc_ko_at_sinr_threshold", true, false, -8.0F, true},
        crc_dtx_test_params{"cg_crc_ko_above_sinr_threshold", true, false, 10.0F, true},
        crc_dtx_test_params{"cg_crc_ko_without_sinr", true, false, std::nullopt, true},
        crc_dtx_test_params{"cg_crc_ok_below_sinr_threshold", true, true, -20.0F, true},
        crc_dtx_test_params{"dynamic_crc_ko_below_sinr_threshold", false, false, -20.0F, true}),
    [](const auto& p) { return p.param.name; });

class cfra_ue_repository_test : public ::testing::Test
{
protected:
  cfra_ue_repository_test() :
    expert_cfg(config_helpers::make_default_scheduler_expert_config()),
    cfg_mng(scheduler_config{expert_cfg, metric_notif}),
    sched_cfg(sched_config_helper::make_default_sched_cell_configuration_request(builder_params)),
    cell_cfg(*cfg_mng.add_cell(sched_cfg)),
    ue_db(expert_cfg.ue)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-b");

    ue_db.register_cell(cell_ues);
  }

  du_ue_index_t add_ue(bool cfra_enabled, bool starts_in_fallback, rnti_t crnti = to_rnti(0x4601))
  {
    sched_ue_creation_request_message ue_req = sched_config_helper::create_default_sched_ue_creation_request(
        cell_cfg.params, std::array<lcid_t, 3>{lcid_t::LCID_SRB1, lcid_t::LCID_SRB2, lcid_t::LCID_MIN_DRB});
    ue_req.crnti                   = crnti;
    ue_req.starts_in_fallback      = starts_in_fallback;
    ue_req.cfra_enabled            = cfra_enabled;
    ue_req.ul_ccch_slot_rx         = std::nullopt;
    ue_config_update_event  ev     = cfg_mng.add_ue(ue_req);
    const ue_configuration& ue_cfg = ev.next_config();
    du_ue_index_t           ue_idx = ue_req.ue_index;
    ue_db.add_ue(ue_cfg, {sched_config_helper::to_ue_creation_mode(ue_req), ue_req.ul_ccch_slot_rx});
    ev.notify_completion();
    return ue_idx;
  }

  scheduler_expert_config                  expert_cfg;
  sched_cfg_dummy_notifier                 metric_notif;
  cell_config_builder_params               builder_params;
  sched_config_manager                     cfg_mng;
  sched_cell_configuration_request_message sched_cfg;
  const cell_configuration&                cell_cfg;
  ue_cell_repository                       cell_ues{cell_cfg, nullptr};
  ue_repository                            ue_db;
};

TEST_F(cfra_ue_repository_test, reestablishment_returns_the_ue_to_the_cell_specific_koffset)
{
  const du_ue_index_t ue_idx = add_ue(false, false);
  const slot_point    sl_tx{cell_cfg.scs_common(), 0};
  ue_koffset_tracker& koffset = ue_db[ue_idx].koffset_tracker();

  koffset.handle_ce_sent(std::chrono::milliseconds{7}, sl_tx);
  koffset.handle_ce_acked();
  koffset.slot_indication(sl_tx, {});
  ASSERT_EQ(koffset.applied_koffset(), std::chrono::milliseconds{7});

  // Reestablishment resets the UE MAC, which clears the Differential Koffset, as per TS 38.321, Section 5.12.
  sched_ue_creation_request_message ue_req = sched_config_helper::create_default_sched_ue_creation_request(
      cell_cfg.params, std::array<lcid_t, 3>{lcid_t::LCID_SRB1, lcid_t::LCID_SRB2, lcid_t::LCID_MIN_DRB});
  ue_req.cfg.cause = sched_ue_config_request::causes::rrc_reconf_after_reest;

  ue_config_update_event ev = cfg_mng.update_ue(sched_ue_reconfiguration_message{ue_idx, ue_req.crnti, ue_req.cfg});
  ASSERT_TRUE(ev.valid());
  ue_db.reconfigure_ue(ev.next_config(), sched_ue_config_request::causes::rrc_reconf_after_reest);
  ev.notify_completion();

  ASSERT_EQ(ue_db[ue_idx].koffset_tracker().applied_koffset(), std::chrono::milliseconds{0});
  ASSERT_FALSE(ue_db[ue_idx].koffset_tracker().has_pending_ce());
}

TEST_F(cfra_ue_repository_test, cfra_ue_starts_in_pending_cfra_state)
{
  du_ue_index_t         ue_idx   = add_ue(true, true);
  const ue_pcell_state& pcell_st = ue_db[ue_idx].get_pcell().get_pcell_state();
  ASSERT_EQ(pcell_st.conres_st, ue_conres_state::pending_cfra);
  ASSERT_TRUE(is_in_fallback(pcell_st.config_st, pcell_st.conres_st));
}

TEST_F(cfra_ue_repository_test, cfra_msg3_acked_transitions_to_conres_completed)
{
  du_ue_index_t ue_idx = add_ue(true, true);
  ASSERT_TRUE(ue_db.cfra_msg3_acked(ue_idx));
  const ue_pcell_state& pcell_st = ue_db[ue_idx].get_pcell().get_pcell_state();
  ASSERT_EQ(pcell_st.conres_st, ue_conres_state::conres_completed);
  ASSERT_FALSE(is_in_fallback(pcell_st.config_st, pcell_st.conres_st));
}

TEST_F(cfra_ue_repository_test, crnti_ce_in_pending_cfra_transitions_to_conres_completed)
{
  du_ue_index_t ue_idx = add_ue(true, true);
  ASSERT_TRUE(ue_db.crnti_ce_received(ue_idx));
  const ue_pcell_state& pcell_st = ue_db[ue_idx].get_pcell().get_pcell_state();
  ASSERT_EQ(pcell_st.conres_st, ue_conres_state::conres_completed);
  ASSERT_FALSE(is_in_fallback(pcell_st.config_st, pcell_st.conres_st));
}

TEST_F(cfra_ue_repository_test, cfra_msg3_acked_is_noop_when_not_in_pending_cfra)
{
  du_ue_index_t ue_idx = add_ue(false, false);
  ASSERT_EQ(ue_db[ue_idx].get_pcell().get_pcell_state().conres_st, ue_conres_state::conres_completed);
  ASSERT_FALSE(ue_db.cfra_msg3_acked(ue_idx));
  ASSERT_EQ(ue_db[ue_idx].get_pcell().get_pcell_state().conres_st, ue_conres_state::conres_completed);
}

/// \brief Test fixture of an NTN cell, where an uplink transmission of the UE reaches the gNB the cell-specific
/// Koffset and k_mac after the downlink that granted it.
class ntn_ue_repository_test : public ::testing::Test
{
protected:
  /// Cell-specific Koffset, which the UE adds to its own uplink.
  static constexpr std::chrono::milliseconds cell_koffset{30};
  /// \brief Slots the uplink of the UE takes to reach the gNB on top of the Koffset.
  ///
  /// Larger than the processing time of a TA command, so that the slots it adds to the window of stale N_TA updates
  /// are told apart from the ones the Koffset adds.
  static constexpr std::chrono::milliseconds k_mac{20};
  /// TA command the reported N_TA updates lead to, far enough from the zero offset of 31 to cross the threshold.
  static constexpr uint8_t reported_ta_cmd = 33;

  ntn_ue_repository_test() :
    expert_cfg(config_helpers::make_default_scheduler_expert_config()),
    cfg_mng(scheduler_config{expert_cfg, metric_notif}),
    sched_cfg(make_ntn_sched_cfg()),
    cell_cfg(*cfg_mng.add_cell(sched_cfg)),
    ue_db(expert_cfg.ue),
    next_sl_tx(cell_cfg.scs_common(), 0)
  {
    ue_db.register_cell(cell_ues);
  }

  static sched_cell_configuration_request_message make_ntn_sched_cfg()
  {
    cell_config_builder_params params;
    auto                       req = sched_config_helper::make_default_sched_cell_configuration_request(params);
    req.ran.ntn_params.emplace();
    req.ran.ntn_params->ntn_cfg.cell_specific_koffset = cell_koffset;
    req.ran.ntn_params->ntn_cfg.k_mac                 = k_mac;
    return req;
  }

  du_ue_index_t add_ue()
  {
    sched_ue_creation_request_message ue_req = sched_config_helper::create_default_sched_ue_creation_request(
        cell_cfg.params, std::array<lcid_t, 3>{lcid_t::LCID_SRB1, lcid_t::LCID_SRB2, lcid_t::LCID_MIN_DRB});
    ue_config_update_event  ev     = cfg_mng.add_ue(ue_req);
    const ue_configuration& ue_cfg = ev.next_config();
    ue_db.add_ue(ue_cfg, {sched_config_helper::to_ue_creation_mode(ue_req), ue_req.ul_ccch_slot_rx});
    ev.notify_completion();
    return ue_req.ue_index;
  }

  void run_slot() { ue_db.slot_indication(++next_sl_tx); }

  /// Reports an N_TA update of the UE that the TA manager turns into \c reported_ta_cmd.
  void report_n_ta_update(du_ue_index_t ue_idx)
  {
    // N_TA update in T_C units, as per TS 38.213, Section 4.2, where 31 is the TA command leaving the offset in place.
    const int64_t n_ta_diff = (static_cast<int64_t>(reported_ta_cmd) - 31) * 16 * 64 /
                              static_cast<int64_t>(1U << to_numerology_value(cell_cfg.scs_common()));
    ue_db[ue_idx].handle_ul_n_ta_update_indication(time_alignment_group::id_t{0},
                                                   phy_time_unit::from_units_of_Tc(n_ta_diff),
                                                   expert_cfg.ue.ta_control.update_measurement_ul_sinr_threshold + 10);
  }

  /// Returns whether the UE holds a TA command, which the call consumes.
  bool fetch_ta_cmd(du_ue_index_t ue_idx)
  {
    const lcid_dl_sch_t lcid            = lcid_dl_sch_t::TA_CMD;
    const unsigned      remaining_bytes = lcid.sizeof_ce() + FIXED_SIZED_MAC_CE_SUBHEADER_SIZE + 3;
    dl_msg_lc_info      subpdu{};
    return ue_db[ue_idx].logical_channels().allocate_mac_ce(subpdu, remaining_bytes) > 0;
  }

  scheduler_expert_config                  expert_cfg;
  sched_cfg_dummy_notifier                 metric_notif;
  sched_config_manager                     cfg_mng;
  sched_cell_configuration_request_message sched_cfg;
  const cell_configuration&                cell_cfg;
  ue_cell_repository                       cell_ues{cell_cfg, nullptr};
  ue_repository                            ue_db;
  slot_point                               next_sl_tx;
};

/// \brief The measurements the TA manager discards after a TA command cover k_mac.
///
/// The UE applies a TA command in an uplink slot past its Koffset, as per TS 38.213, Section 4.2, and the first
/// uplink it sends with the new timing reaches the gNB k_mac slots later still. Every N_TA update measured until
/// then carries the timing the command already corrects, so acting on one corrects the same offset twice.
TEST_F(ntn_ue_repository_test, n_ta_updates_are_discarded_until_an_uplink_with_the_new_timing_reaches_the_gnb)
{
  ASSERT_GT(cell_cfg.ntn_cs_koffset, 0U);
  ASSERT_GT(cell_cfg.ntn_k_mac_slots, 0U);

  const du_ue_index_t ue_idx = add_ue();
  run_slot();

  report_n_ta_update(ue_idx);
  bool ta_cmd_sent = false;
  for (unsigned count = 0; count != expert_cfg.ue.ta_control.measurement_period and not ta_cmd_sent; ++count) {
    run_slot();
    ta_cmd_sent = fetch_ta_cmd(ue_idx);
  }
  ASSERT_TRUE(ta_cmd_sent) << "No TA command was sent for the UE";

  const unsigned stale_slots =
      expert_cfg.ue.ta_control.measurement_prohibit_period + cell_cfg.ntn_cs_koffset + cell_cfg.ntn_k_mac_slots;
  for (unsigned count = 0; count != stale_slots; ++count) {
    report_n_ta_update(ue_idx);
    run_slot();
    ASSERT_FALSE(fetch_ta_cmd(ue_idx)) << "A TA command was sent for an N_TA update measured before the uplink of the "
                                          "UE carried the previous one";
  }
  for (unsigned count = 0; count != expert_cfg.ue.ta_control.measurement_period; ++count) {
    run_slot();
    ASSERT_FALSE(fetch_ta_cmd(ue_idx))
        << "N_TA updates measured before the uplink of the UE carried the previous TA command were not discarded";
  }
}
