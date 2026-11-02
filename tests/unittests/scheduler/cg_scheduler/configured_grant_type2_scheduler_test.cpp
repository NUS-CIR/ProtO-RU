// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief Unit tests for the CG Type 2 scheduler: activation DCI, confirmation, re-activation and CG resource pool.

#include "configured_grant_scheduler_test_fixture.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/scheduler/scheduler_test_message_validators.h"
#include "tests/unittests/scheduler/test_utils/result_test_helpers.h"
#include "ocudu/ran/du_types.h"
#include <algorithm>
#include <limits>
#include <set>

using namespace ocudu;

namespace {

/// Safety margin, in slots, added on top of the CG period when waiting for a CG event to show up.
constexpr unsigned safety_margin = 20U;

/// Mirrors configured_grant_type2_sched_impl::max_expected_crc_delay: slots after the first CG transmission of an
/// activation DCI by which the UE must have confirmed it.
constexpr unsigned max_expected_crc_delay = 50U;

/// Grant parameters carried by a CG Type 2 activation DCI, decoded into the representation the CG PUSCH uses.
struct cg_activation_grant_params {
  /// VRBs the UE is told to transmit the CG PUSCH on.
  vrb_interval vrbs;
  /// Index in the cell PUSCH time domain resource allocation list.
  unsigned time_resource;
  uint8_t  mcs;
};

/// CG Type 2 activation DCI seen in the scheduler output.
struct cg_activation {
  /// Slot of the PDCCH that carried the DCI.
  slot_point                 pdcch_slot;
  cg_activation_grant_params grant;
};

/// Base fixture of the CG Type 2 tests, with a Type 2 CG configured in the cell.
class configured_grant_type2_scheduler_test : public configured_grant_scheduler_test
{
protected:
  static constexpr rnti_t ue2_crnti = to_rnti(0x4602);
  static constexpr rnti_t cs_rnti_2 = to_rnti(0xe0f0);

  explicit configured_grant_type2_scheduler_test(cg_test_params                    params         = {},
                                                 const cell_config_builder_params& builder_params = {}) :
    configured_grant_scheduler_test(with_type2(params), builder_params)
  {
  }

  static cg_test_params with_type2(cg_test_params params)
  {
    params.type = cg_builder_params::cg_type::type2;
    return params;
  }

  /// \brief Upper bound, in slots, from an activation DCI to the re-activation DCI that follows when it is not
  /// confirmed.
  ///
  /// The deadline is k2 + max_expected_crc_delay slots after the DCI, and the re-activation DCI then waits for a PDCCH
  /// slot from which a TDRA row lands on the CG offset, at most one CG period later.
  unsigned reactivation_window() const
  {
    return SCHEDULER_MAX_K2 + max_expected_crc_delay + cg_params.period_slots + 2 * safety_margin;
  }

  /// \brief Creates the UE without any CG configuration, and waits for the creation to be processed.
  ///
  /// The CG is installed separately by install_type2_cg(), which lets a test install it on several UEs in one slot.
  void create_ue_without_cg(du_ue_index_t ue_idx = to_du_ue_index(0), rnti_t crnti = ue_crnti)
  {
    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = ue_idx;
    ue_req.crnti    = crnti;
    ASSERT_TRUE(ue_req.cfg.cells.has_value() and not ue_req.cfg.cells->empty());
    ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();
    add_ue(ue_req, /*wait_notification=*/true);
  }

  /// \brief Installs the Type 2 CG configuration on an existing UE, which triggers add_reconf_ue() in the scheduler.
  ///
  /// \param notify_config_applied When true, the configuration-applied indication is sent, which makes the scheduler
  /// consider the UE ready for Type 2 activation.
  void install_type2_cg(bool          notify_config_applied,
                        du_ue_index_t ue_idx      = to_du_ue_index(0),
                        rnti_t        crnti       = ue_crnti,
                        rnti_t        cs_rnti_val = cs_rnti)
  {
    auto ue_req = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ASSERT_TRUE(ue_req.cfg.cells.has_value() and not ue_req.cfg.cells->empty());
    ASSERT_TRUE(ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value());

    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = ue_idx;
    reconf.crnti    = crnti;
    reconf.cfg      = ue_req.cfg;
    reconf.cs_rnti  = cs_rnti_val;
    sched->handle_ue_reconfiguration_request(reconf);

    if (notify_config_applied) {
      sched->handle_ue_config_applied(ue_idx);
    }
  }

  /// Adds a UE with a Type 2 CG configuration: creation followed by the CG-installing reconfiguration.
  void add_type2_cg_ue(bool          notify_config_applied,
                       du_ue_index_t ue_idx      = to_du_ue_index(0),
                       rnti_t        crnti       = ue_crnti,
                       rnti_t        cs_rnti_val = cs_rnti)
  {
    ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(ue_idx, crnti));
    ASSERT_NO_FATAL_FAILURE(install_type2_cg(notify_config_applied, ue_idx, crnti, cs_rnti_val));
  }

  /// \brief Adds the two test UEs so that they become ready for activation in the same slot.
  ///
  /// Both UEs are created before the CG is installed on either, with no slot run in between. Otherwise the first UE
  /// could be activated while the second is still being created.
  void add_both_ues()
  {
    ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(to_du_ue_index(0), ue_crnti));
    ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(to_du_ue_index(1), ue2_crnti));
    ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, to_du_ue_index(0), ue_crnti, cs_rnti));
    ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, to_du_ue_index(1), ue2_crnti, cs_rnti_2));
  }

  /// \brief Advances the scheduler until an UL PDCCH for \c search_rnti shows up in the output.
  ///
  /// \return The slot the PDCCH was found in, or an invalid slot_point if \c max_slots was exhausted.
  /// \note The CG Type 2 activation DCI is addressed to the CS-RNTI, as per TS 38.213, Section 10.2, which isolates it
  /// from any dynamic grant addressed to the C-RNTI.
  slot_point run_until_next_ul_pdcch(unsigned max_slots, rnti_t search_rnti = cs_rnti)
  {
    for (unsigned i = 0; i != max_slots; ++i) {
      run_slot();
      if (find_ue_ul_pdcch(search_rnti) != nullptr) {
        return last_result_slot();
      }
    }
    return {};
  }

  /// \brief Advances the scheduler until the next activation DCI for \c search_rnti, and checks that it is one.
  ///
  /// \return The activation DCI, or nullopt if none showed up within \c max_slots.
  std::optional<cg_activation> run_until_cg_activation(unsigned max_slots, rnti_t search_rnti = cs_rnti)
  {
    const slot_point pdcch_slot = run_until_next_ul_pdcch(max_slots, search_rnti);
    if (not pdcch_slot.valid()) {
      return std::nullopt;
    }
    const pdcch_ul_information& pdcch = *find_ue_ul_pdcch(search_rnti);
    check_cg_activation_dci(pdcch);
    return cg_activation{pdcch_slot, decode_cg_activation_dci(pdcch)};
  }

  /// \brief Adds a Type 2 CG UE and runs until its first activation DCI.
  ///
  /// \param[out] activation The activation DCI, when not null.
  void activate_ue(cg_activation* activation  = nullptr,
                   du_ue_index_t  ue_idx      = to_du_ue_index(0),
                   rnti_t         crnti       = ue_crnti,
                   rnti_t         cs_rnti_val = cs_rnti)
  {
    ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true, ue_idx, crnti, cs_rnti_val));
    const std::optional<cg_activation> act =
        run_until_cg_activation(cg_params.period_slots + safety_margin, cs_rnti_val);
    ASSERT_TRUE(act.has_value()) << "No CG Type 2 activation DCI was allocated";
    if (activation != nullptr) {
      *activation = *act;
    }
  }

  /// \brief Reconfigures the test UE with a modified Type 2 CG configuration, and confirms the new configuration.
  ///
  /// A new number of CG HARQ processes is enough to make the new CG configuration differ from the installed one.
  void reconf_type2_cg(unsigned new_nof_harq_procs)
  {
    auto ue_req = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ASSERT_TRUE(ue_req.cfg.cells.has_value() and not ue_req.cfg.cells->empty());
    ASSERT_TRUE(ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value());
    ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg->nof_harq_processes = new_nof_harq_procs;

    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = to_du_ue_index(0);
    reconf.crnti    = ue_crnti;
    reconf.cfg      = ue_req.cfg;
    reconf.cs_rnti  = cs_rnti;
    sched->handle_ue_reconfiguration_request(reconf);
    sched->handle_ue_config_applied(to_du_ue_index(0));
  }

  /// \brief Sends the CG activation confirmation, which stops the activation from being retried.
  ///
  /// \param slot_rx Slot of the PUSCH that carried the MAC CE; the last scheduled slot when invalid.
  void confirm_cg_activation(du_ue_index_t ue_idx  = to_du_ue_index(0),
                             rnti_t        crnti   = ue_crnti,
                             slot_point    slot_rx = {}) const
  {
    cg_confirmation_indication_message cg_conf{};
    cg_conf.cell_index = to_du_cell_index(0);
    cg_conf.ue_index   = ue_idx;
    cg_conf.rnti       = crnti;
    cg_conf.slot_rx    = slot_rx.valid() ? slot_rx : last_result_slot();
    sched->handle_cg_confirmation_message(cg_conf);
  }

  /// \brief Reports UL pending bytes for the test UE.
  ///
  /// A single report is enough: CG PUSCH allocations leave the logical channel buffers untouched, so the pending bytes
  /// persist across occasions.
  void push_ul_data(rnti_t crnti = ue_crnti, du_ue_index_t ue_idx = to_du_ue_index(0))
  {
    push_bsr(ul_bsr_indication_message{
        to_du_cell_index(0), ue_idx, crnti, bsr_format::SHORT_BSR, {{uint_to_lcg_id(0), 200}}});
  }

  /// Removes the UE and runs slots until the scheduler confirms the removal, so that the UE index can be reused.
  void rem_ue_and_wait(du_ue_index_t ue_idx = to_du_ue_index(0))
  {
    schedule_task(launch_rem_ue_task(ue_idx));
    run_until_all_pending_tasks_completion();
  }

  /// \brief Checks that the next \c nof_periods CG PUSCHs of the test UE are one CG period apart.
  ///
  /// Starts from the next CG PUSCH occasion.
  void check_cg_puschs_are_periodic(unsigned nof_periods = 4)
  {
    ASSERT_NE(run_until_next_cg_pusch(), nullptr) << "No CG PUSCH occasion was scheduled";
    slot_point prev_slot = last_result_slot();
    for (unsigned i = 0; i != nof_periods; ++i) {
      ASSERT_NE(run_until_next_cg_pusch(cg_params.period_slots + safety_margin), nullptr)
          << "CG PUSCH occasion " << i + 1 << " never arrived";
      EXPECT_EQ(last_result_slot() - prev_slot, static_cast<int>(cg_params.period_slots))
          << "CG PUSCH occasion " << i + 1 << " is not one CG period after the previous one";
      prev_slot = last_result_slot();
    }
  }

  /// Checks that \c pdcch carries a CG Type 2 activation, as per TS 38.213, Section 10.2, Table 10.2-1.
  static void check_cg_activation_dci(const pdcch_ul_information& pdcch)
  {
    ASSERT_EQ(pdcch.dci.type(), dci_ul_rnti_config_type::cs_rnti_f0_1);
    const dci_0_1_cs_rnti_configuration& dci = pdcch.dci.as_cs_rnti_f0_1();
    EXPECT_FALSE(dci.new_data_indicator);
    EXPECT_EQ(dci.harq_process_number, 0);
    EXPECT_EQ(dci.redundancy_version, 0);
  }

  /// \brief Decodes the grant parameters carried by a CG Type 2 activation DCI.
  ///
  /// The frequency allocation is a type-1 RIV over the UE's active UL BWP, which is the cell's initial UL BWP here.
  cg_activation_grant_params decode_cg_activation_dci(const pdcch_ul_information& pdcch) const
  {
    const dci_0_1_cs_rnti_configuration& dci = pdcch.dci.as_cs_rnti_f0_1();

    cg_activation_grant_params params{};
    params.time_resource = dci.time_resource;
    params.mcs           = dci.modulation_coding_scheme;

    const unsigned bwp_nof_rbs = cell_cfg().params.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
    const ra_frequency_type1_configuration freq = ra_frequency_type1_from_riv(bwp_nof_rbs, dci.frequency_resource);
    params.vrbs                                 = vrb_interval{freq.start_vrb, freq.start_vrb + freq.length_vrb};
    return params;
  }

  /// Returns the entry of the cell PUSCH time domain resource list an activation DCI points to.
  const pusch_time_domain_resource_allocation& pusch_td_res(const cg_activation_grant_params& grant) const
  {
    const auto& pusch_td_list = cell_cfg().params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
    // Checked here rather than by the caller: an index out of the list is a scheduler bug, not a test setup issue.
    EXPECT_LT(grant.time_resource, pusch_td_list.size()) << "the activation DCI points outside the PUSCH TD list";
    return pusch_td_list[std::min<unsigned>(grant.time_resource, pusch_td_list.size() - 1)];
  }
};

TEST_F(configured_grant_type2_scheduler_test, add_reconf_ue_does_not_schedule_cg_before_ue_is_ready)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // No configuration-applied indication, so start_ue_cg_activation() is never called.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/false));

  for (unsigned i = 0; i != cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    EXPECT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "CG PUSCH was scheduled before Type 2 activation";
  }
}

TEST_F(configured_grant_type2_scheduler_test, activation_dci_sets_the_ul_sch_indicator)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // An UL-SCH indicator of 0, with no CSI or SRS request, is a combination the UE is not expected to receive and may
  // discard, as per TS 38.212, Section 7.3.1.1.2.
  ASSERT_NO_FATAL_FAILURE(activate_ue());
  EXPECT_EQ(find_ue_ul_pdcch(cs_rnti)->dci.as_cs_rnti_f0_1().ul_sch_indicator, 1U);
  ASSERT_NE(run_until_next_cg_pusch(), nullptr) << "No CG PUSCH followed the activation DCI";
}

/// Type 2 tests run for a CG period above and below max_expected_crc_delay.
class configured_grant_type2_period_test : public configured_grant_type2_scheduler_test,
                                           public ::testing::WithParamInterface<cg_period_test_params>
{
protected:
  configured_grant_type2_period_test() :
    configured_grant_type2_scheduler_test(
        cg_test_params{.periodicity = GetParam().periodicity, .period_slots = GetParam().period_slots})
  {
  }
};

TEST_P(configured_grant_type2_period_test, unconfirmed_activation_is_resent_for_the_same_grant)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // No UL data is reported: the re-activation must not depend on it.
  cg_activation first;
  ASSERT_NO_FATAL_FAILURE(activate_ue(&first));

  const std::optional<cg_activation> second = run_until_cg_activation(reactivation_window());
  ASSERT_TRUE(second.has_value()) << "the unconfirmed CG Type 2 activation was never re-sent";
  EXPECT_GT(second->pdcch_slot, first.pdcch_slot + static_cast<int>(max_expected_crc_delay))
      << "the activation was re-sent before its confirmation deadline";
  EXPECT_EQ(second->grant.vrbs, first.grant.vrbs) << "the re-activation DCI moved the UE to other RBs";
  EXPECT_EQ(second->grant.mcs, first.grant.mcs) << "the re-activation DCI changed the MCS";
}

INSTANTIATE_TEST_SUITE_P(cg_periods,
                         configured_grant_type2_period_test,
                         ::testing::Values(cg_period_test_params{cg_configuration::periodicity_t::sl10, 10},
                                           cg_period_test_params{cg_configuration::periodicity_t::sl40, 40}),
                         [](const auto& p) { return fmt::format("period_{}_slots", p.param.period_slots); });

TEST_F(configured_grant_type2_scheduler_test, cg_puschs_keep_their_offset_while_the_activation_is_unconfirmed)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // The UE may already transmit on the grant, so its occasions continue, and a re-activation must not move them.
  ASSERT_NO_FATAL_FAILURE(activate_ue());

  std::vector<slot_point> pdcch_slots;
  std::vector<slot_point> pusch_slots;
  for (unsigned i = 0, e = 2 * reactivation_window(); i != e; ++i) {
    run_slot();
    if (find_ue_ul_pdcch(cs_rnti) != nullptr) {
      pdcch_slots.push_back(last_result_slot());
    }
    if (find_ue_pusch(cs_rnti, *last_sched_result()) != nullptr) {
      pusch_slots.push_back(last_result_slot());
    }
  }
  ASSERT_FALSE(pdcch_slots.empty()) << "the unconfirmed CG Type 2 activation was never re-sent";
  ASSERT_FALSE(pusch_slots.empty()) << "no CG PUSCH was allocated while the activation was unconfirmed";

  for (unsigned i = 1; i != pusch_slots.size(); ++i) {
    EXPECT_EQ(pusch_slots[i] - pusch_slots[i - 1], static_cast<int>(cg_params.period_slots))
        << fmt::format("the CG PUSCH of slot {} is not one CG period after the previous one", pusch_slots[i]);
  }
}

TEST_F(configured_grant_type2_scheduler_test, unconfirmed_activation_is_resent_until_confirmed)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  ASSERT_NO_FATAL_FAILURE(activate_ue());

  const slot_point second_pdcch_slot = run_until_next_ul_pdcch(reactivation_window());
  ASSERT_TRUE(second_pdcch_slot.valid()) << "CG Type 2 activation was not re-sent a first time";

  const slot_point third_pdcch_slot = run_until_next_ul_pdcch(reactivation_window());
  ASSERT_TRUE(third_pdcch_slot.valid()) << "CG Type 2 activation was not re-sent a second time";
  EXPECT_GT(third_pdcch_slot, second_pdcch_slot + static_cast<int>(max_expected_crc_delay))
      << "the second re-activation did not wait for the deadline of the first one";

  confirm_cg_activation();
  EXPECT_FALSE(run_until_next_ul_pdcch(2 * reactivation_window()).valid())
      << "CG Type 2 activation was re-sent after the UE confirmed it";
}

TEST_F(configured_grant_type2_scheduler_test, late_confirmation_of_an_earlier_activation_dci_confirms_the_grant)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // Every activation DCI carries the same grant, so confirming an earlier one confirms the grant.
  cg_activation first;
  ASSERT_NO_FATAL_FAILURE(activate_ue(&first));
  ASSERT_TRUE(run_until_next_ul_pdcch(reactivation_window()).valid())
      << "the unconfirmed CG Type 2 activation was never re-sent";

  // The CG Confirmation MAC CE drawn by the first DCI, carried by a PUSCH that precedes the re-activation.
  confirm_cg_activation(to_du_ue_index(0), ue_crnti, first.pdcch_slot + 1);

  EXPECT_FALSE(run_until_next_ul_pdcch(2 * reactivation_window()).valid())
      << "a late confirmation did not confirm the grant";
}

/// A CRC outcome for a CG PUSCH occasion, together with whether it confirms the activation.
struct cg_crc_confirmation_params {
  std::string          name;
  bool                 crc_ok;
  std::optional<float> sinr_dB;
  bool                 confirms;
};

/// Formatter for CRC confirmation params, used by gtest. Avoids the fallback raw-byte printer, which reads
/// uninitialized padding bytes.
void PrintTo(const cg_crc_confirmation_params& value, ::std::ostream* os)
{
  *os << value.name;
}

class configured_grant_type2_crc_confirmation_test : public configured_grant_type2_scheduler_test,
                                                     public ::testing::WithParamInterface<cg_crc_confirmation_params>
{};

TEST_P(configured_grant_type2_crc_confirmation_test, only_a_crc_ok_confirms_the_activation)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // Only this UE transmits on its CG resource, so a CRC OK proves it holds the grant. A CRC KO does not, whatever its
  // SINR: the UE may not have transmitted at all.
  ASSERT_NO_FATAL_FAILURE(activate_ue());

  const ul_sched_info* grant = run_until_next_cg_pusch();
  ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion followed the activation DCI";
  send_cg_crc(*grant, last_result_slot(), GetParam().crc_ok, GetParam().sinr_dB);

  const bool reactivated = run_until_next_ul_pdcch(reactivation_window()).valid();
  EXPECT_NE(reactivated, GetParam().confirms)
      << (GetParam().confirms ? "CG Type 2 activation was re-sent although a CRC OK confirmed it"
                              : "CG Type 2 activation was taken as confirmed by a CRC KO");
}

INSTANTIATE_TEST_SUITE_P(
    cg_crc_outcome,
    configured_grant_type2_crc_confirmation_test,
    ::testing::Values(
        cg_crc_confirmation_params{"crc_ok", true, 10.0F, true},
        cg_crc_confirmation_params{"crc_ko_above_sinr_threshold", false, 10.0F, false},
        cg_crc_confirmation_params{"crc_ko_without_sinr", false, std::nullopt, false},
        cg_crc_confirmation_params{"crc_ko_dtx", false, -20.0F, false},
        cg_crc_confirmation_params{"crc_ko_infinite_sinr", false, std::numeric_limits<float>::infinity(), false}),
    [](const auto& p) { return p.param.name; });

TEST_F(configured_grant_type2_scheduler_test, ue_reconfiguration_without_cg_is_handled)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // CG is DRB-gated, so a UE goes through reconfigurations that carry no CG config, e.g. the SRB2/DRB setup.
  ASSERT_NO_FATAL_FAILURE(create_ue_without_cg());
  // Sends a reconfiguration with no CG config to a UE that has none.
  remove_cg_via_reconfig();
  sched->handle_ue_config_applied(to_du_ue_index(0));

  for (unsigned i = 0; i != cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr) << "CG PUSCH scheduled for a UE without CG";
  }
}

TEST_F(configured_grant_type2_scheduler_test, cg_resources_are_returned_to_the_pool_when_ue_is_removed)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // The pool holds one resource per usable slot offset (36 for this cell), so more cycles than that turn a resource
  // leaked by rem_ue() into a failed activation.
  constexpr unsigned nof_cycles = 60;

  for (unsigned i = 0; i != nof_cycles; ++i) {
    ASSERT_NO_FATAL_FAILURE(activate_ue()) << "cycle=" << i << ": the resource pool was likely drained";
    // Remove the UE once activated, rather than while still pending activation.
    confirm_cg_activation();
    run_slot();
    rem_ue_and_wait();
  }
}

TEST_F(configured_grant_type2_scheduler_test, config_applied_after_confirmation_does_not_activate_twice)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // A configuration-applied indication can arrive with no CG reconfiguration in between. A second activation would put
  // the UE in the wheel at two offsets and leak the resource of the first one.
  ASSERT_NO_FATAL_FAILURE(activate_ue());
  confirm_cg_activation();
  run_slot();

  const unsigned window_slots         = 3 * cg_params.period_slots;
  unsigned       nof_cg_puschs_before = 0;
  for (unsigned i = 0; i != window_slots; ++i) {
    run_slot();
    if (find_ue_pusch(cs_rnti, *last_sched_result()) != nullptr) {
      ++nof_cg_puschs_before;
    }
  }
  ASSERT_GT(nof_cg_puschs_before, 0U) << "the activated UE was given no CG PUSCH occasion to begin with";

  sched->handle_ue_config_applied(to_du_ue_index(0));

  unsigned nof_cg_puschs_after = 0;
  for (unsigned i = 0; i != window_slots; ++i) {
    run_slot();
    if (find_ue_pusch(cs_rnti, *last_sched_result()) != nullptr) {
      ++nof_cg_puschs_after;
    }
    EXPECT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr) << "a second activation DCI was sent for an already-activated UE";
  }
  EXPECT_EQ(nof_cg_puschs_after, nof_cg_puschs_before) << "the CG PUSCH rate changed: the UE was activated twice";
}

/// \brief Fixture for the contract between the activation DCI and the CG PUSCH occasions that follow.
///
/// The UE only sees the DCI, while the scheduler builds every occasion from its own copy of the grant. Any mismatch
/// means the UE transmits on resources the gNB does not decode.
class configured_grant_type2_dci_contract_test : public configured_grant_type2_scheduler_test
{
protected:
  configured_grant_type2_dci_contract_test() :
    configured_grant_type2_scheduler_test(cg_test_params{.extra_pusch_td_alloc = true})
  {
  }
};

TEST_F(configured_grant_type2_dci_contract_test, cg_pusch_matches_the_activation_dci)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  cg_activation act;
  ASSERT_NO_FATAL_FAILURE(activate_ue(&act));
  const ul_sched_info* grant = run_until_next_cg_pusch();
  ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion followed the activation DCI";

  ASSERT_TRUE(grant->pusch_cfg.rbs.is_type1()) << "Expected a type-1 (contiguous) VRB allocation";
  EXPECT_EQ(grant->pusch_cfg.rbs.type1(), act.grant.vrbs) << "the CG PUSCH VRBs differ from the activation DCI";
  EXPECT_EQ(grant->pusch_cfg.symbols, pusch_td_res(act.grant).symbols)
      << "the CG PUSCH symbols differ from the TD allocation of the activation DCI";
  EXPECT_EQ(grant->pusch_cfg.mcs_index.value(), act.grant.mcs) << "the CG PUSCH MCS differs from the activation DCI";
  // A Type 2 grant takes its MCS from the cell-level CG configuration.
  EXPECT_EQ(act.grant.mcs, cg_params.mcs) << "the activation DCI does not carry the cell-configured CG MCS";

  // The TBS is computed once at activation and reused by every occasion. is_valid_ul_sched_info() recomputes it from
  // the grant's fields, so a stale or wrong TBS fails here.
  EXPECT_GT(grant->pusch_cfg.tb_size_bytes.value(), 0U);
  EXPECT_TRUE(test_helper::is_valid_ul_sched_info(*grant))
      << "CG PUSCH TBS/code-rate inconsistent with the grant parameters announced in the activation DCI";
}

/// \brief Fixture whose CG period is shorter than the k2 the activation DCI announces.
///
/// The occasion phase is (activation slot + k2) % period, so the phase recurs between the activation PDCCH and the
/// UE's first transmission. The UE does not transmit on those earlier occasions.
class configured_grant_type2_short_period_vs_k2_test : public configured_grant_type2_scheduler_test
{
protected:
  configured_grant_type2_short_period_vs_k2_test() :
    configured_grant_type2_scheduler_test(
        cg_test_params{.periodicity = cg_configuration::periodicity_t::sl4, .period_slots = 4})
  {
  }
};

TEST_F(configured_grant_type2_short_period_vs_k2_test, no_cg_pusch_is_scheduled_before_the_first_transmission)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  cg_activation act;
  ASSERT_NO_FATAL_FAILURE(activate_ue(&act));
  const unsigned k2 = pusch_td_res(act.grant).k2;

  // Guard against a vacuous pass: with k2 below the period, no occasion precedes the first transmission.
  ASSERT_GE(k2, cg_params.period_slots) << "this test needs a k2 of at least the CG period";

  // The activation slot is checked first: with k2 equal to the period, the occasion phase lands on it.
  EXPECT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
      << "a CG PUSCH was scheduled in the very slot that carried the activation DCI";
  for (unsigned i = 1; i != k2; ++i) {
    run_slot();
    EXPECT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "a CG PUSCH was scheduled " << i << " slots after the activation DCI, before the first transmission at "
        << "k2=" << k2;
  }

  run_slot();
  EXPECT_NE(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
      << "no CG PUSCH at the first transmission slot, k2=" << k2 << " slots after the activation DCI";
  EXPECT_EQ(last_result_slot() - act.pdcch_slot, static_cast<int>(k2));
}

/// \brief Fixture for CG Type 2 tests with two UEs on the same slot offset.
///
/// The CG band holds four CG resources per slot offset.
class configured_grant_type2_multi_ue_test : public configured_grant_type2_scheduler_test
{
protected:
  configured_grant_type2_multi_ue_test() :
    configured_grant_type2_scheduler_test(cg_test_params{.max_nof_cell_cg_rbs = 40})
  {
  }

  /// \brief Advances until a slot in which both UEs have a CG PUSCH, and reports their VRBs.
  /// \return False if no such slot came up within \c max_slots.
  bool run_until_both_ues_have_cg_pusch(unsigned max_slots, vrb_interval& vrbs_ue1, vrb_interval& vrbs_ue2)
  {
    for (unsigned i = 0; i != max_slots; ++i) {
      run_slot();
      const ul_sched_info* pusch_ue1 = find_ue_pusch(cs_rnti, *last_sched_result());
      const ul_sched_info* pusch_ue2 = find_ue_pusch(cs_rnti_2, *last_sched_result());
      if (pusch_ue1 == nullptr or pusch_ue2 == nullptr) {
        continue;
      }
      if (not pusch_ue1->pusch_cfg.rbs.is_type1() or not pusch_ue2->pusch_cfg.rbs.is_type1()) {
        return false;
      }
      vrbs_ue1 = pusch_ue1->pusch_cfg.rbs.type1();
      vrbs_ue2 = pusch_ue2->pusch_cfg.rbs.type1();
      return true;
    }
    return false;
  }
};

TEST_F(configured_grant_type2_multi_ue_test, ues_activated_on_the_same_slot_offset_get_disjoint_vrbs)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  ASSERT_NO_FATAL_FAILURE(add_both_ues());

  vrb_interval vrbs_ue1;
  vrb_interval vrbs_ue2;
  ASSERT_TRUE(run_until_both_ues_have_cg_pusch(2 * (cg_params.period_slots + safety_margin), vrbs_ue1, vrbs_ue2))
      << "No slot was found in which both activated UEs have a CG PUSCH";

  EXPECT_FALSE(vrbs_ue1.overlaps(vrbs_ue2))
      << "the two UEs were given overlapping CG VRBs in the same slot: ue1=[" << vrbs_ue1.start() << ".."
      << vrbs_ue1.stop() << ") ue2=[" << vrbs_ue2.start() << ".." << vrbs_ue2.stop() << ")";
}

TEST_F(configured_grant_type2_multi_ue_test, removing_one_activated_ue_preserves_the_cg_grants_of_the_other)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // rem_ue() unwinds the removed UE's wheel entries, which must leave the other UE's entries in place.
  ASSERT_NO_FATAL_FAILURE(add_both_ues());

  vrb_interval vrbs_ue1;
  vrb_interval vrbs_ue2;
  ASSERT_TRUE(run_until_both_ues_have_cg_pusch(2 * (cg_params.period_slots + safety_margin), vrbs_ue1, vrbs_ue2))
      << "Pre-condition: both UEs must have CG PUSCH occasions before one of them is removed";

  rem_ue_and_wait(to_du_ue_index(0));

  unsigned nof_ue2_grants = 0;
  for (unsigned i = 0; i != 2 * cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "a CG PUSCH was still scheduled for the removed UE";
    if (find_ue_pusch(cs_rnti_2, *last_sched_result()) != nullptr) {
      ++nof_ue2_grants;
    }
  }
  EXPECT_GE(nof_ue2_grants, 2U) << "removing one UE took the other UE's CG occasions out of the slot wheel too";
}

TEST_F(configured_grant_type2_scheduler_test, second_ue_is_activated_on_a_different_slot_offset)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // The default CG band holds one resource per slot offset, so the second UE cannot take the first UE's offset. Its
  // activation must be retried on a later slot, at another offset.
  ASSERT_NO_FATAL_FAILURE(add_both_ues());

  bool ue1_has_cg_pusch = false;
  bool ue2_has_cg_pusch = false;
  for (unsigned i = 0; i != 2 * (cg_params.period_slots + safety_margin); ++i) {
    run_slot();
    const bool ue1_pusch = find_ue_pusch(cs_rnti, *last_sched_result()) != nullptr;
    const bool ue2_pusch = find_ue_pusch(cs_rnti_2, *last_sched_result()) != nullptr;
    ASSERT_FALSE(ue1_pusch and ue2_pusch)
        << "both UEs were given a CG PUSCH in the same slot, although the offset holds a single CG resource";
    ue1_has_cg_pusch |= ue1_pusch;
    ue2_has_cg_pusch |= ue2_pusch;
  }

  ASSERT_TRUE(ue1_has_cg_pusch) << "the first UE was never activated";
  ASSERT_TRUE(ue2_has_cg_pusch) << "the second UE was never activated after its first slot offset was exhausted";
}

/// \brief CG Type 2 in a cell with periodic SRS, which reserves the last symbols of every UL slot.
///
/// The extra PUSCH TD allocation fits in the symbols SRS leaves free. With 12 symbols instead of 14, the grant needs
/// 12 RBs.
class configured_grant_type2_srs_test : public configured_grant_type2_scheduler_test
{
protected:
  configured_grant_type2_srs_test() :
    configured_grant_type2_scheduler_test(
        cg_test_params{.max_nof_cell_cg_rbs = 12, .srs_enabled = true, .extra_pusch_td_alloc = true})
  {
  }
};

TEST_F(configured_grant_type2_srs_test, activation_picks_a_td_allocation_that_avoids_the_srs_symbols)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // The CG symbols are fixed at activation, so a TD allocation reaching into SRS would collide on every occasion.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  const ul_sched_info* grant = run_until_next_cg_pusch();
  ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion was scheduled with SRS enabled";

  // SRS resources always sit at the end of the slot, as per TS 38.211, Section 6.4.1.4.1.
  const unsigned first_srs_symbol =
      NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - cell_cfg().params.init_bwp.srs_cfg.max_nof_symbols.value();
  EXPECT_LE(grant->pusch_cfg.symbols.stop(), first_srs_symbol)
      << "the CG PUSCH reaches into the symbols reserved for SRS";

  // Guard against a vacuous pass: some TD allocation of the cell must collide with SRS.
  const auto& pusch_td_list = cell_cfg().params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
  EXPECT_TRUE(std::any_of(pusch_td_list.begin(),
                          pusch_td_list.end(),
                          [first_srs_symbol](const pusch_time_domain_resource_allocation& td) {
                            return td.symbols.stop() > first_srs_symbol;
                          }))
      << "every TD allocation of this cell already avoids SRS, so nothing was skipped";
}

TEST_F(configured_grant_type2_scheduler_test, discarded_activation_pdcch_is_resent_without_waiting_for_the_deadline)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  cg_activation first;
  ASSERT_NO_FATAL_FAILURE(activate_ue(&first));

  scheduler_slot_handler::error_outcome err{};
  err.pdcch_discarded = true;
  sched->handle_error_indication(first.pdcch_slot, to_du_cell_index(0), err);

  const std::optional<cg_activation> retry = run_until_cg_activation(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(retry.has_value()) << "the CG Type 2 activation was never re-sent after its PDCCH was discarded";
  EXPECT_LT(retry->pdcch_slot - first.pdcch_slot, static_cast<int>(max_expected_crc_delay))
      << "the re-activation did not follow the error indication promptly";
  EXPECT_EQ(retry->grant.vrbs, first.grant.vrbs) << "the re-activation DCI moved the UE to other RBs";
}

TEST_F(configured_grant_type2_scheduler_test, removing_a_ue_still_pending_activation_is_clean)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // Such a UE holds no CG resource and no wheel entry, so its removal must not return anything to the pool.
  constexpr unsigned nof_cycles = 10;

  for (unsigned i = 0; i != nof_cycles; ++i) {
    // No slot is run before the removal, so the UE never reaches activation.
    ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true)) << "cycle=" << i;
    rem_ue_and_wait();

    for (unsigned n = 0; n != safety_margin; ++n) {
      run_slot();
      ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
          << "an activation DCI was sent for a UE that was removed while pending activation, at cycle=" << i;
      ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
          << "a CG PUSCH was scheduled for a UE that was removed while pending activation, at cycle=" << i;
    }
  }

  // The pool and the wheel are untouched: a new UE still gets activated and served.
  ASSERT_NO_FATAL_FAILURE(activate_ue());
  confirm_cg_activation();
  EXPECT_NE(run_until_next_cg_pusch(), nullptr) << "the newly activated UE was given no CG PUSCH occasion";
}

TEST_F(configured_grant_type2_scheduler_test, reconfiguration_of_an_activated_ue_requires_a_new_activation)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // A CG reconfiguration tears the CG down, so the UE needs a new activation DCI, and the wheel entries of the first
  // activation must be gone. Otherwise the UE sits at two offsets and transmits twice per period.
  ASSERT_NO_FATAL_FAILURE(activate_ue());
  confirm_cg_activation();
  ASSERT_NE(run_until_next_cg_pusch(), nullptr) << "Pre-condition: the UE must have CG occasions before the reconfig";

  ASSERT_NO_FATAL_FAILURE(reconf_type2_cg(/*new_nof_harq_procs=*/8));

  ASSERT_TRUE(run_until_cg_activation(cg_params.period_slots + safety_margin).has_value())
      << "the reconfigured UE was never activated again";
  confirm_cg_activation();
  ASSERT_NO_FATAL_FAILURE(check_cg_puschs_are_periodic());
}

TEST_F(configured_grant_type2_scheduler_test, reconfiguration_that_leaves_the_cg_unchanged_does_not_disturb_it)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // Most reconfigurations do not touch the CG, e.g. a DRB setup. Tearing the grant down would cost a re-activation and
  // a gap in the CG occasions every time.
  ASSERT_NO_FATAL_FAILURE(activate_ue());
  confirm_cg_activation();

  const ul_sched_info* grant_before = run_until_next_cg_pusch();
  ASSERT_NE(grant_before, nullptr) << "Pre-condition: the UE must have CG occasions before the reconfiguration";
  ASSERT_TRUE(grant_before->pusch_cfg.rbs.is_type1());
  const vrb_interval vrbs_before = grant_before->pusch_cfg.rbs.type1();
  // The phase is fixed at activation, so it changes only if the UE was re-activated.
  const unsigned phase_before = last_result_slot().count() % cg_params.period_slots;

  // Reconfigure the UE with the configuration it already has, CG included.
  ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true));

  unsigned nof_cg_puschs = 0;
  for (unsigned i = 0; i != 3 * cg_params.period_slots; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
        << "the UE was re-activated by a reconfiguration that left its CG configuration untouched";
    const ul_sched_info* grant = find_ue_pusch(cs_rnti, *last_sched_result());
    if (grant == nullptr) {
      continue;
    }
    ++nof_cg_puschs;
    ASSERT_TRUE(grant->pusch_cfg.rbs.is_type1());
    EXPECT_EQ(grant->pusch_cfg.rbs.type1(), vrbs_before) << "the CG grant moved to a different set of VRBs";
    EXPECT_EQ(last_result_slot().count() % cg_params.period_slots, phase_before)
        << "the CG occasions moved to a different slot offset";
  }
  EXPECT_GE(nof_cg_puschs, 3U) << "the CG occasions stopped after a reconfiguration that did not change the CG";
}

TEST_F(configured_grant_type2_scheduler_test, unconfirmed_activation_is_paused_while_the_ue_is_in_fallback)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // In fallback, the size of the DCI format 0_1 the UE decodes is not known.
  ASSERT_NO_FATAL_FAILURE(activate_ue());

  // A reconfiguration that leaves the CG unchanged, not yet applied by the UE.
  ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/false));
  EXPECT_FALSE(run_until_next_ul_pdcch(2 * reactivation_window()).valid())
      << "a CG Type 2 activation DCI was sent while the UE was in fallback";

  sched->handle_ue_config_applied(to_du_ue_index(0));
  EXPECT_TRUE(run_until_cg_activation(reactivation_window()).has_value())
      << "the activation did not resume after the UE applied its configuration";
}

TEST_F(configured_grant_type2_scheduler_test, cell_deactivation_returns_cg_resources_to_the_pool)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // Cell deactivation drops the UEs without rem_ue(), so the CG scheduler frees their resources itself. More cycles
  // than the pool size (36) turn a leak into a failed activation.
  constexpr unsigned nof_cycles = 60;

  for (unsigned i = 0; i != nof_cycles; ++i) {
    // New RNTIs every cycle: the resource manager hands a UE back the resource it already holds, which would hide a
    // leak.
    const rnti_t cycle_crnti   = to_rnti(to_underlying(ue_crnti) + i);
    const rnti_t cycle_cs_rnti = to_rnti(to_underlying(cs_rnti) - i);
    ASSERT_NO_FATAL_FAILURE(activate_ue(nullptr, to_du_ue_index(0), cycle_crnti, cycle_cs_rnti))
        << "cycle=" << i << ": the resource pool was likely drained";

    // Deactivate the cell while the UE is activated rather than pending activation.
    confirm_cg_activation(to_du_ue_index(0), cycle_crnti);
    run_slot();

    // No slot is run in between: a deactivated cell scheduler expects no slot indication.
    sched->handle_cell_deactivation_request(to_du_cell_index(0));
    sched->handle_cell_activation_request(to_du_cell_index(0));
    // The UE removal that the DU sends once the cell is stopped frees the UE index for the next cycle.
    rem_ue(to_du_ue_index(0));
    run_slot();
  }
}

/// \brief CG Type 2 on an F1AP-created UE that is still awaiting its C-RNTI MAC CE.
///
/// While contention resolution is pending, the CG reconfiguration and the configuration-applied indication do not reach
/// the CG scheduler. The C-RNTI CE catches the setup up, which is the only path to add_reconf_ue() with no previous
/// configuration.
class configured_grant_type2_fallback_ue_test : public configured_grant_type2_scheduler_test
{
protected:
  static constexpr du_ue_index_t ue_idx = to_du_ue_index(0);

  /// \brief Creates the UE in fallback via the F1AP path (no UL-CCCH slot, no CFRA), without CG configuration.
  ///
  /// \note scheduler_test_simulator::add_ue() is bypassed, as it sends the C-RNTI CE right after the creation request.
  void add_fallback_crnti_ce_ue()
  {
    auto ue_req               = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index           = ue_idx;
    ue_req.crnti              = ue_crnti;
    ue_req.starts_in_fallback = true;
    ASSERT_EQ(sched_config_helper::to_ue_creation_mode(ue_req), ue_creation_mode::high_layers);
    ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();

    sched->handle_ue_creation_request(ue_req);
    notif.last_ue_index_cfg.reset();
    for (unsigned i = 0; i != 100 and notif.last_ue_index_cfg != ue_idx; ++i) {
      run_slot();
    }
    ASSERT_EQ(notif.last_ue_index_cfg, ue_idx) << "UE creation was not completed";
  }
};

TEST_F(configured_grant_type2_fallback_ue_test, cg_activation_is_deferred_until_the_crnti_ce_is_received)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  ASSERT_NO_FATAL_FAILURE(add_fallback_crnti_ce_ue());
  ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, ue_idx));

  for (unsigned i = 0; i != cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
        << "a CG Type 2 activation DCI was sent while contention resolution was still pending";
    ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "a CG PUSCH was scheduled while contention resolution was still pending";
  }

  sched->handle_crnti_ce_received(ue_idx);

  ASSERT_TRUE(run_until_cg_activation(cg_params.period_slots + safety_margin).has_value())
      << "the UE was never activated after its C-RNTI CE was received";
  confirm_cg_activation(ue_idx);
  EXPECT_NE(run_until_next_cg_pusch(), nullptr) << "No CG PUSCH followed the activation";

  rem_ue_and_wait(ue_idx);
}

TEST_F(configured_grant_type2_fallback_ue_test, cg_and_dynamic_grants_use_disjoint_harq_processes)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // The first HARQ processes of a UE with CG are reserved for its CG occasions, by
  // ue_cell::handle_reconfiguration_request(). A dynamic grant on one of them would overwrite a CG transmission still
  // awaiting its CRC.
  ASSERT_NO_FATAL_FAILURE(add_fallback_crnti_ce_ue());
  ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, ue_idx));
  sched->handle_crnti_ce_received(ue_idx);

  ASSERT_TRUE(run_until_cg_activation(cg_params.period_slots + safety_margin).has_value())
      << "the UE was never activated after its C-RNTI CE was received";
  confirm_cg_activation(ue_idx);

  // UL data, so that dynamic grants compete for the same HARQ entity.
  push_ul_data(ue_crnti, ue_idx);

  unsigned nof_cg_grants      = 0;
  unsigned nof_dynamic_grants = 0;
  for (unsigned i = 0; i != 3 * cg_params.period_slots; ++i) {
    run_slot();
    if (const ul_sched_info* cg_grant = find_ue_pusch(cs_rnti, *last_sched_result()); cg_grant != nullptr) {
      ++nof_cg_grants;
      EXPECT_LT(cg_grant->pusch_cfg.harq_id, to_harq_id(cg_params.nof_harq_procs))
          << "a CG PUSCH used a HARQ process outside the range reserved for CG";
    }
    if (const ul_sched_info* dyn_grant = find_ue_pusch(ue_crnti, *last_sched_result()); dyn_grant != nullptr) {
      ++nof_dynamic_grants;
      EXPECT_GE(dyn_grant->pusch_cfg.harq_id, to_harq_id(cg_params.nof_harq_procs))
          << "a dynamic PUSCH was given a HARQ process reserved for CG";
    }
  }

  // Guard against a vacuous pass: both kinds of grant must have been seen.
  EXPECT_GT(nof_cg_grants, 0U) << "the UE was given no CG PUSCH occasion";
  EXPECT_GT(nof_dynamic_grants, 0U) << "the UE was given no dynamic UL grant, so no HARQ contention was exercised";
}

/// CG Type 2 in FDD and TDD cells. In TDD, the activation needs a DL slot for the PDCCH and a TDRA row whose k2 lands
/// on a UL slot.
class configured_grant_type2_duplex_test : public configured_grant_type2_scheduler_test,
                                           public ::testing::WithParamInterface<cg_duplex_test_params>
{
protected:
  configured_grant_type2_duplex_test() :
    configured_grant_type2_scheduler_test(cg_test_params{}, make_cell_builder_params(GetParam()))
  {
  }
};

TEST_P(configured_grant_type2_duplex_test, cg_puschs_start_k2_slots_after_the_activation_dci_and_recur_every_period)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  // The occasion phase is (activation slot + k2) % period. With k2 below the period, the first occasion lands exactly
  // k2 slots after the activation DCI. Any other phase means the UE transmits in slots the gNB does not reserve.
  cg_activation act;
  ASSERT_NO_FATAL_FAILURE(activate_ue(&act));
  const unsigned k2 = pusch_td_res(act.grant).k2;
  ASSERT_LT(k2, cg_params.period_slots) << "this test assumes a k2 shorter than the CG period";

  const ul_sched_info* grant = run_until_next_cg_pusch();
  ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion followed the activation DCI";
  EXPECT_EQ(last_result_slot() - act.pdcch_slot, static_cast<int>(k2))
      << "the first CG PUSCH occasion is not k2 slots after the activation DCI";
  EXPECT_EQ(grant->pusch_cfg.symbols, pusch_td_res(act.grant).symbols);

  // Confirm the activation, so that a re-activation cannot interfere with the occasions checked below.
  confirm_cg_activation();
  ASSERT_NO_FATAL_FAILURE(check_cg_puschs_are_periodic());
}

INSTANTIATE_TEST_SUITE_P(
    cg_duplex,
    configured_grant_type2_duplex_test,
    ::testing::Values(cg_duplex_test_params{"FDD", duplex_mode::FDD, std::nullopt, 0},
                      cg_duplex_test_params{"TDD_6D1S3U",
                                            duplex_mode::TDD,
                                            tdd_ul_dl_config_common{subcarrier_spacing::kHz30, {10, 6, 10, 3, 0}},
                                            0},
                      cg_duplex_test_params{"TDD_3D1S6U",
                                            duplex_mode::TDD,
                                            tdd_ul_dl_config_common{subcarrier_spacing::kHz30, {10, 3, 10, 6, 0}},
                                            0}),
    [](const auto& p) { return p.param.name; });

/// \brief Fixture in which more UEs become ready for activation in one slot than the per-slot cap allows.
///
/// run_ues_activation() caps the activation DCIs per slot to MAX_CG_ACTIVATION_PDCCHS_PER_SLOT, on which
/// CG_CONFIRMATIONS_POOL_SIZE is sized.
class configured_grant_type2_activation_cap_test : public configured_grant_type2_scheduler_test
{
protected:
  /// Enough UEs that the cap has to defer at least one of them.
  static constexpr unsigned nof_ues = MAX_CG_ACTIVATION_PDCCHS_PER_SLOT + 2;

  /// One CG resource per UE, so that only the cap holds UEs back.
  configured_grant_type2_activation_cap_test() :
    configured_grant_type2_scheduler_test(cg_test_params{.max_nof_cell_cg_rbs = 10 * nof_ues})
  {
  }

  static rnti_t crnti_of(unsigned i) { return to_rnti(static_cast<uint16_t>(fmt::underlying(ue_crnti) + i)); }
  static rnti_t cs_rnti_of(unsigned i) { return to_rnti(static_cast<uint16_t>(fmt::underlying(cs_rnti) + i)); }

  /// Adds every UE so that they all become ready for activation in the same slot (see add_both_ues()).
  void add_all_ues()
  {
    for (unsigned i = 0; i != nof_ues; ++i) {
      ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(to_du_ue_index(i), crnti_of(i)));
    }
    for (unsigned i = 0; i != nof_ues; ++i) {
      ASSERT_NO_FATAL_FAILURE(
          install_type2_cg(/*notify_config_applied=*/true, to_du_ue_index(i), crnti_of(i), cs_rnti_of(i)));
    }
  }

  /// Counts the CG Type 2 activation DCIs in the last scheduler result.
  unsigned count_activation_dcis() const
  {
    const auto& ul_pdcchs = last_sched_result()->dl.ul_pdcchs;
    return std::count_if(ul_pdcchs.begin(), ul_pdcchs.end(), [](const pdcch_ul_information& pdcch) {
      return pdcch.dci.type() == dci_ul_rnti_config_type::cs_rnti_f0_1;
    });
  }
};

TEST_F(configured_grant_type2_activation_cap_test, no_slot_carries_more_activation_dcis_than_the_cap)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-BW-16-3");

  ASSERT_NO_FATAL_FAILURE(add_all_ues());

  unsigned         max_activations_in_a_slot = 0;
  std::set<rnti_t> activated_ues;
  for (unsigned i = 0; i != 2 * cg_params.period_slots; ++i) {
    run_slot();

    const unsigned nof_activations = count_activation_dcis();
    ASSERT_LE(nof_activations, MAX_CG_ACTIVATION_PDCCHS_PER_SLOT)
        << "more CG activation DCIs were sent in one slot than the cap the CG confirmation pool is sized on";
    max_activations_in_a_slot = std::max(max_activations_in_a_slot, nof_activations);

    for (unsigned u = 0; u != nof_ues; ++u) {
      if (find_ue_ul_pdcch(cs_rnti_of(u)) != nullptr) {
        activated_ues.emplace(cs_rnti_of(u));
      }
    }
  }

  // Guard against a vacuous pass: some slot must reach the cap.
  ASSERT_EQ(max_activations_in_a_slot, MAX_CG_ACTIVATION_PDCCHS_PER_SLOT)
      << "no slot ever reached the activation cap, so the limit was never the binding constraint";
  // The cap must defer activations, not starve them.
  ASSERT_EQ(activated_ues.size(), nof_ues) << "some UEs were never activated after the cap deferred them";
}

} // namespace
