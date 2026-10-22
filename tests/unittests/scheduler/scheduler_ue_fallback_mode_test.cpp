// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief In this file, we test the correct behaviour of the scheduler when handling UE contention resolution, and
/// TC-RNTI to C-RNTI transitions.

#include "test_utils/indication_generators.h"
#include "test_utils/result_test_helpers.h"
#include "test_utils/scheduler_test_simulator.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/scheduler/cell_config_builder_profiles.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/duplex_mode.h"
#include "ocudu/ran/harq_id.h"
#include "ocudu/ran/prach/prach_time_mapping.h"
#include "ocudu/scheduler/config/cg_builder_params.h"
#include <algorithm>
#include <functional>
#include <gtest/gtest.h>

using namespace ocudu;

ocudulog::basic_levels sched_log_level = ocudulog::basic_levels::warning;

static bool is_f1_pucch_with_harq_bits(const pucch_info& pucch, bool is_common, bool has_sr)
{
  return pucch.format() == pucch_format::FORMAT_1 and ((pucch.uci_bits.sr_bits != sr_nof_bits::no_sr) == has_sr) and
         pucch.uci_bits.harq_ack_nof_bits > 0 and (not pucch.res->second_hop_prb.has_value() != is_common);
}

class base_scheduler_conres_test : public scheduler_test_simulator
{
public:
  base_scheduler_conres_test(duplex_mode duplx_mode) :
    scheduler_test_simulator(4, duplx_mode == duplex_mode::FDD ? subcarrier_spacing::kHz15 : subcarrier_spacing::kHz30)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-a");

    ocudulog::fetch_basic_logger("SCHED", true).set_level(sched_log_level);
    builder_params = cell_config_builder_profiles::create(duplx_mode);
  }

  /// Runs slots until a common PUCCH with HARQ bits is scheduled for \c rnti_to_find. The returned pointer stays
  /// valid until the next slot runs.
  const pucch_info* run_until_common_pucch_with_harq(rnti_t rnti_to_find)
  {
    const pucch_info* first_conres_msg4_pucch = nullptr;
    run_slot_until([this, &first_conres_msg4_pucch, rnti_to_find]() {
      span<const pucch_info> pucchs = this->last_sched_result()->ul.pucchs;
      auto                   it = std::find_if(pucchs.begin(), pucchs.end(), [rnti_to_find](const pucch_info& pucch) {
        return pucch.crnti == rnti_to_find and is_f1_pucch_with_harq_bits(pucch, true, false);
      });
      if (it == pucchs.end()) {
        return false;
      }
      first_conres_msg4_pucch = &*it;
      return true;
    });
    return first_conres_msg4_pucch;
  }

  bool run_until_conres_msg4_scheduled(rnti_t rnti_to_find, lcid_dl_sch_t grant_lcid)
  {
    bool grant_allocated = false;
    this->run_slot_until([this, &grant_allocated, rnti_to_find, grant_lcid]() {
      const pdcch_dl_information* dl_pdcch = find_ue_dl_pdcch(rnti_to_find);
      if (dl_pdcch != nullptr) {
        const dl_msg_alloc* pdsch_alloc = find_ue_pdsch(rnti_to_find, *this->last_sched_result());
        // These conditions must be verified to consider the ConRes correctly scheduled.
        const bool conres_tc_rnti_check =
            grant_lcid != lcid_dl_sch_t::UE_CON_RES_ID or dl_pdcch->dci.type() == dci_dl_rnti_config_type::tc_rnti_f1_0;
        if (not conres_tc_rnti_check or pdsch_alloc == nullptr or not pdsch_alloc->pdsch_cfg.codewords[0].new_data or
            pdsch_alloc->tb_list.size() != 1) {
          return false;
        }
        for (const auto& lg_ch : pdsch_alloc->tb_list[0].lc_chs_to_sched) {
          if (lg_ch.lcid == grant_lcid) {
            grant_allocated = true;
            return true;
          }
        }
      }

      return false;
    });
    return grant_allocated;
  }

  bool run_until_pucch_scheduled_and_received(rnti_t rnti_to_find, du_ue_index_t ue_index, bool ack)
  {
    // Check whether PUCCH (i) has been allocated and (ii) on common resources.
    const pucch_info* first_conres_pucch = run_until_common_pucch_with_harq(rnti_to_find);
    if (first_conres_pucch == nullptr) {
      return false;
    }

    // If only the ConRes was scheduled, we need to wait for the PUCCH to be sent and inject an ACK, to confirm the
    // ConRes was received by the UE. Without this, the scheduler won't allocate any other PDSCH grant.
    uci_indication uci =
        test_helper::create_uci_indication(next_slot.without_hyper_sfn() - 1, ue_index, *first_conres_pucch);
    std::get<uci_indication::uci_pdu::uci_pucch_f0_or_f1_pdu>(uci.ucis[0].pdu).harqs[0] =
        ack ? mac_harq_ack_report_status::ack : mac_harq_ack_report_status::nack;
    this->sched->handle_uci_indication(uci);

    return true;
  }

  const dl_msg_lc_info* get_msg4_msg_lc_info(rnti_t rnti_to_find, lcid_t msg4_lcid) const
  {
    const dl_msg_alloc* pdsch_alloc = find_ue_pdsch(rnti_to_find, *this->last_sched_result());
    if (pdsch_alloc == nullptr or not pdsch_alloc->pdsch_cfg.codewords[0].new_data or
        pdsch_alloc->tb_list.size() != 1) {
      return nullptr;
    }

    for (const auto& lg_ch : pdsch_alloc->tb_list[0].lc_chs_to_sched) {
      if (lg_ch.lcid == msg4_lcid) {
        if (lg_ch.lcid == LCID_SRB0 or lg_ch.lcid == LCID_SRB1) {
          return &lg_ch;
        }
      }
    }
    return nullptr;
  }

  cell_config_builder_params builder_params{};
};

class base_single_ue_scheduler_conres_test : public base_scheduler_conres_test
{
public:
  base_single_ue_scheduler_conres_test(duplex_mode duplx_mode = duplex_mode::FDD) :
    base_scheduler_conres_test(duplx_mode)
  {
    // Create cell config with space for two PDCCHs in the SearchSpace#1.
    add_cell(sched_config_helper::make_default_sched_cell_configuration_request(builder_params));

    const auto& csi_cfg = this->cell_cfg(to_du_cell_index(0)).bwp_res[to_bwp_id(0)].dl().csi;
    ocudu_assert(csi_cfg.has_value() and not csi_cfg->nzp_csi_rs_list.empty(),
                 "This test assumes a setup with NZP CSI-RS enabled");
    ocudu_assert(csi_cfg.has_value() and not csi_cfg->zp_csi_rs_list.empty(),
                 "This test assumes a setup with ZP CSI-RS enabled");

    // Create a UE with a DRB active.
    auto ue_cfg =
        sched_config_helper::create_default_sched_ue_creation_request(cell_cfg(to_du_cell_index(0)).params, {});
    ue_cfg.ue_index           = ue_index;
    ue_cfg.crnti              = rnti;
    ue_cfg.starts_in_fallback = true;
    ue_cfg.ul_ccch_slot_rx    = this->next_slot.without_hyper_sfn();
    scheduler_test_simulator::add_ue(ue_cfg, false);
  }

  du_ue_index_t ue_index = to_du_ue_index(0);
  rnti_t        rnti     = to_rnti(0x4601);
};

/// \brief Test to verify that when a PDSCH with ConRes CE is scheduled even when there is no LCID0/LCID1 PDU passed
/// down to the scheduler.
class scheduler_conres_without_pdu_test : public base_single_ue_scheduler_conres_test, public ::testing::Test
{};

TEST_F(scheduler_conres_without_pdu_test, when_conres_ce_is_enqueued_and_no_msg4_is_enqueued_then_pdsch_is_scheduled)
{
  // Ensure the ConRes CE is scheduled without a Msg4 SDU.
  ASSERT_TRUE(this->run_slot_until([this]() { return find_ue_pdsch(rnti, *this->last_sched_result()) != nullptr; }));
  const dl_msg_alloc* conres_alloc = find_ue_pdsch(rnti, *this->last_sched_result());
  ASSERT_EQ(conres_alloc->tb_list.size(), 1);
  ASSERT_EQ(conres_alloc->tb_list[0].lc_chs_to_sched.size(), 1);
  ASSERT_EQ(conres_alloc->tb_list[0].lc_chs_to_sched[0].lcid, lcid_dl_sch_t::UE_CON_RES_ID);
  ASSERT_EQ(find_ue_dl_pdcch(rnti)->dci.type(), dci_dl_rnti_config_type::tc_rnti_f1_0);
  ASSERT_TRUE(this->last_sched_result()->dl.csi_rs.empty());
}

/// \brief Test to verify that the UE leaves fallback mode and is scheduled with dedicated resources regardless of the
/// order in which the ConRes CE ACK and the UE config applied events are processed.
class scheduler_fallback_exit_test : public base_single_ue_scheduler_conres_test, public ::testing::TestWithParam<bool>
{
protected:
  bool config_applied_before_conres_ack() const { return GetParam(); }
};

TEST_P(scheduler_fallback_exit_test, when_ue_exits_fallback_then_it_is_scheduled_with_dedicated_resources)
{
  static constexpr unsigned msg4_size = 128;
  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, LCID_SRB1, msg4_size});

  ASSERT_TRUE(run_until_conres_msg4_scheduled(rnti, lcid_dl_sch_t::UE_CON_RES_ID)) << "ConRes not scheduled";
  if (get_msg4_msg_lc_info(rnti, LCID_SRB1) == nullptr) {
    // The ConRes CE was scheduled alone, so the Msg4 needs its own PDSCH, which requires the ConRes CE ACK.
    ASSERT_TRUE(run_until_pucch_scheduled_and_received(rnti, ue_index, /* ACK */ true));
    ASSERT_TRUE(run_until_conres_msg4_scheduled(rnti, LCID_SRB1)) << "Msg4 not scheduled";
  }

  if (config_applied_before_conres_ack()) {
    this->sched->handle_ue_config_applied(ue_index);
  }
  ASSERT_TRUE(run_until_pucch_scheduled_and_received(rnti, ue_index, /* ACK */ true));
  if (not config_applied_before_conres_ack()) {
    this->sched->handle_ue_config_applied(ue_index);
  }

  // Outside fallback mode, SRB1 data is scheduled with a UE-dedicated DCI format.
  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, LCID_SRB1, msg4_size});
  ASSERT_TRUE(this->run_slot_until([this]() {
    const pdcch_dl_information* dl_pdcch = find_ue_dl_pdcch(rnti);
    return dl_pdcch != nullptr and dl_pdcch->dci.type() == dci_dl_rnti_config_type::c_rnti_f1_1;
  })) << "UE not scheduled with dedicated resources after leaving fallback";
}

INSTANTIATE_TEST_SUITE_P(scheduler_fallback_exit_test,
                         scheduler_fallback_exit_test,
                         ::testing::Values(false, true),
                         [](const ::testing::TestParamInfo<bool>& param_info) {
                           return param_info.param ? "config_applied_first" : "conres_ack_first";
                         });

// ------------------------------------------------------------------------------------------------------------------ //

struct conres_test_params {
  lcid_t      msg4_lcid;
  duplex_mode duplx_mode;
};

/// Formatter for test params.
void PrintTo(const conres_test_params& value, ::std::ostream* os)
{
  *os << fmt::format("LCID={}, mode={}", value.msg4_lcid, value.duplx_mode == duplex_mode::TDD ? "TDD" : "FDD");
}

/// \brief Test to verify the correct scheduling of the ConRes CE and Msg4 LCID0/1 PDU even when multiple PRACH
/// preambles are detected.
class scheduler_con_res_msg4_test : public base_single_ue_scheduler_conres_test,
                                    public ::testing::TestWithParam<conres_test_params>
{
public:
  scheduler_con_res_msg4_test() : base_single_ue_scheduler_conres_test(GetParam().duplx_mode), params(GetParam()) {}

  /// Enqueues several RACH indications with RNTI different than the created UE.
  void enqueue_random_number_of_rach_indications()
  {
    rach_indication_message rach_ind{to_du_cell_index(0), next_slot_rx(), {{0, 0, {}}}};
    auto                    nof_preambles = test_rng::uniform_int<unsigned>(1, 10);
    for (uint8_t i = 0; i != nof_preambles; ++i) {
      rach_ind.occasions[0].preambles.push_back({i, to_rnti(static_cast<uint16_t>(rnti) + 1 + i), phy_time_unit{}});
    }
    this->sched->handle_rach_indication(rach_ind);
  }

  const unsigned msg4_size = 128;

  conres_test_params params;
};

TEST_P(scheduler_con_res_msg4_test,
       when_conres_ce_and_srb_pdu_are_enqueued_then_tc_rnti_is_used_and_multiplexing_with_csi_rs_is_avoided)
{
  // Enqueue several RACH indications, so that RARs that need to be scheduled may fight for RB space with the Msg4.
  enqueue_random_number_of_rach_indications();

  // Enqueue Msg4 in SRB0/SRB1.
  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, params.msg4_lcid, msg4_size});

  // Wait for ConRes (and optionally Msg4) to be scheduled.
  ASSERT_TRUE(run_until_conres_msg4_scheduled(rnti, lcid_dl_sch_t::UE_CON_RES_ID)) << "ConRes not scheduled";
  const dl_msg_alloc* conres_alloc = find_ue_pdsch(rnti, *this->last_sched_result());
  ASSERT_EQ(conres_alloc->tb_list.size(), 1);
  ASSERT_FALSE(conres_alloc->tb_list[0].lc_chs_to_sched.empty());
  ASSERT_EQ(conres_alloc->tb_list[0].lc_chs_to_sched[0].lcid, lcid_dl_sch_t::UE_CON_RES_ID);
  ASSERT_TRUE(this->last_sched_result()->dl.csi_rs.empty());

  // If the allocation is done in a slot where the SSB is also allocated, then there is only space for ConRes. The Msg4
  // will be sent in a separate PDSCH grant.
  const auto* msg4_lc_info = get_msg4_msg_lc_info(rnti, params.msg4_lcid);
  if (msg4_lc_info == nullptr) {
    // If only the ConRes was scheduled, we need to wait for the PUCCH to be sent and inject an ACK, to confirm the
    // ConRes was received by the UE. Without this, the scheduler won't allocate any other PDSCH grant.
    ASSERT_TRUE(run_until_pucch_scheduled_and_received(rnti, ue_index, /* ACK */ true));
    ASSERT_TRUE(run_until_conres_msg4_scheduled(rnti, GetParam().msg4_lcid)) << "Msg4 not scheduled";
    msg4_lc_info = get_msg4_msg_lc_info(rnti, params.msg4_lcid);
    ASSERT_TRUE(msg4_lc_info != nullptr);
    ASSERT_TRUE(this->last_sched_result()->dl.csi_rs.empty());
  }

  if (params.msg4_lcid == LCID_SRB0) {
    ASSERT_GE(msg4_lc_info->sched_bytes, msg4_size);
  }
}

TEST_P(scheduler_con_res_msg4_test, while_ue_is_in_fallback_then_common_pucch_is_used)
{
  // TODO: Increase the crnti message size, once PUCCH scheduler handles multiple HARQ-ACKs falling in the same slot
  //  in fallback mode.
  static constexpr unsigned crnti_msg_size = 8;

  // Enqueue ConRes CE + Msg4.
  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, params.msg4_lcid, msg4_size});

  ASSERT_TRUE(run_until_conres_msg4_scheduled(rnti, lcid_dl_sch_t::UE_CON_RES_ID)) << "ConRes not scheduled";
  const dl_msg_alloc* conres_alloc = find_ue_pdsch(rnti, *this->last_sched_result());
  ASSERT_EQ(conres_alloc->tb_list.size(), 1);
  ASSERT_FALSE(conres_alloc->tb_list[0].lc_chs_to_sched.empty());
  ASSERT_EQ(conres_alloc->tb_list[0].lc_chs_to_sched[0].lcid, lcid_dl_sch_t::UE_CON_RES_ID);

  // If the allocation is done in a slot where the SSB is also allocated, then there is only space for ConRes. The Msg4
  // will be sent in a separate PDSCH grant.
  const auto* msg4_lc_info = get_msg4_msg_lc_info(rnti, params.msg4_lcid);
  if (msg4_lc_info == nullptr) {
    // If only the ConRes was scheduled, we need to wait for the PUCCH to be sent and inject an ACK, to confirm the
    // ConRes was received by the UE. Without this, the scheduler won't allocate any other PDSCH grant.
    ASSERT_TRUE(run_until_pucch_scheduled_and_received(rnti, ue_index, /* ACK */ true));
    ASSERT_TRUE(run_until_conres_msg4_scheduled(rnti, GetParam().msg4_lcid)) << "Msg4 not scheduled";
  }

  ASSERT_TRUE(run_until_pucch_scheduled_and_received(rnti, ue_index, /* ACK */ true));

  // Enqueue SRB1 data; with the UE in fallback mode, and after the MSG4 has been delivered, both common and dedicated
  // resources should be used.
  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, LCID_SRB1, crnti_msg_size});

  // Ensure common resources for PDCCH and PDSCH are used rather than UE-dedicated.
  ASSERT_TRUE(this->run_slot_until([this]() { return find_ue_pdsch(rnti, *this->last_sched_result()) != nullptr; }));
  const pdcch_dl_information& dl_pdcch = *find_ue_dl_pdcch(rnti);
  ASSERT_EQ(dl_pdcch.dci.type(), dci_dl_rnti_config_type::c_rnti_f1_0) << "Invalid format used for UE in fallback mode";
  ASSERT_EQ(dl_pdcch.ctx.coreset_cfg->get_id(), to_coreset_id(0));
  const dl_msg_alloc& pdsch = *find_ue_pdsch(rnti, *this->last_sched_result());
  ASSERT_EQ(pdsch.pdsch_cfg.dci_fmt, dci_dl_format::f1_0);

  // Ensure both common and PUCCH resources are used.
  struct pucch_ptrs {
    const pucch_info* f1_common_ptr = nullptr;
    const pucch_info* f1_ded_ptr    = nullptr;
    const pucch_info* f1_ded_sr_ptr = nullptr;
    const pucch_info* f2_ptr        = nullptr;
  } pucch_res_ptrs;

  ASSERT_TRUE(this->run_slot_until([this, &pucch_res_ptrs]() {
    // Depending on the SR and CSI slots, we can have different combinations of PUCCH grants. There must be at least one
    // PUCCH F1 grant using common resources, and:
    // - No PUCCH F1 ded.
    // - 1 PUCCH F1 ded. with 1 HARQ-ACK bit and NO SR.
    // - 1 PUCCH F1 ded. with 1 HARQ-ACK bit and NO SR and 1 PUCCH F1 ded. with 1 HARQ-ACK bit and SR.
    // - 1 PUCCH F2 ded. with 1 HARQ-ACK bit and CSI, with optional SR.

    const auto& pucchs     = this->last_sched_result()->ul.pucchs;
    unsigned    nof_pucchs = pucchs.size();

    if (nof_pucchs == 1) {
      if (pucchs[0].crnti == rnti and is_f1_pucch_with_harq_bits(pucchs[0], true, false)) {
        pucch_res_ptrs.f1_common_ptr = &pucchs[0];
      }
    } else if (nof_pucchs == 2) {
      for (const auto& pucch : pucchs) {
        if (pucch.crnti != rnti) {
          continue;
        }
        if (is_f1_pucch_with_harq_bits(pucch, true, false)) {
          pucch_res_ptrs.f1_common_ptr = &pucch;
        } else if (is_f1_pucch_with_harq_bits(pucch, false, false)) {
          pucch_res_ptrs.f1_ded_ptr = &pucch;
        } else if (pucch.format() == pucch_format::FORMAT_2 and pucch.uci_bits.harq_ack_nof_bits > 0) {
          pucch_res_ptrs.f2_ptr = &pucch;
        }
      }
    } else if (nof_pucchs == 3) {
      for (const auto& pucch : pucchs) {
        if (pucch.crnti == rnti and pucch.format() == pucch_format::FORMAT_1) {
          if (is_f1_pucch_with_harq_bits(pucch, true, false)) {
            pucch_res_ptrs.f1_common_ptr = &pucch;
          } else if (is_f1_pucch_with_harq_bits(pucch, false, true)) {
            pucch_res_ptrs.f1_ded_sr_ptr = &pucch;
          } else if (is_f1_pucch_with_harq_bits(pucch, false, false)) {
            pucch_res_ptrs.f1_ded_ptr = &pucch;
          }
        }
      }
    }

    return pucch_res_ptrs.f1_common_ptr != nullptr;
  }));

  ASSERT_TRUE(pucch_res_ptrs.f1_common_ptr != nullptr);
  if (pucch_res_ptrs.f1_ded_sr_ptr != nullptr) {
    ASSERT_TRUE(pucch_res_ptrs.f1_ded_ptr != nullptr);
  } else {
    ASSERT_FALSE((pucch_res_ptrs.f1_ded_ptr != nullptr) and (pucch_res_ptrs.f2_ptr != nullptr));
  }
}

TEST_P(scheduler_con_res_msg4_test, while_ue_is_in_fallback_then_common_ss_is_used)
{
  // Enqueue Msg4.
  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, params.msg4_lcid, msg4_size});

  // Wait for ConRes + Msg4 PDCCH to be scheduled.
  ASSERT_TRUE(this->run_slot_until([this]() { return find_ue_dl_pdcch(rnti) != nullptr; }));

  const pdcch_dl_information&       dl_pdcch          = *find_ue_dl_pdcch(rnti);
  bool                              is_common_ss_used = false;
  const search_space_configuration* ss_used           = nullptr;
  for (const search_space_configuration& ss :
       cell_cfg(to_du_cell_index(0)).params.dl_cfg_common.init_dl_bwp.pdcch_common.search_spaces) {
    if (dl_pdcch.ctx.context.ss_id == ss.get_id()) {
      is_common_ss_used = true;
      ss_used           = &ss;
      break;
    }
  }
  ASSERT_TRUE(is_common_ss_used) << "UE in fallback should use common SS";
  // PDCCH monitoring must be active in this slot.
  ASSERT_TRUE(ss_used != nullptr and pdcch_helper::is_pdcch_monitoring_active(next_slot.without_hyper_sfn(), *ss_used))
      << fmt::format(
             "Common SS id={} is not monitored at slot={}", fmt::underlying(ss_used->get_id()), next_slot.slot_index());
}

TEST_P(scheduler_con_res_msg4_test, when_msg4_gets_retxed_then_tc_rnti_is_used_and_csi_rs_avoided)
{
  // Enqueue Msg4.
  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, params.msg4_lcid, msg4_size});

  // Wait for ConRes + Msg4 PDCCH, PDSCH and PUCCH to be scheduled.
  const pucch_info* first_msg4_pucch = run_until_common_pucch_with_harq(rnti);
  ASSERT_NE(first_msg4_pucch, nullptr)
      << "Failed to schedule ConRes CE and Msg4 PDCCH, PDSCH and PUCCH for UE in fallback mode";

  // Enqueue HARQ NACK for Msg4.
  uci_indication uci =
      test_helper::create_uci_indication(next_slot.without_hyper_sfn() - 1, ue_index, *first_msg4_pucch);
  std::get<uci_indication::uci_pdu::uci_pucch_f0_or_f1_pdu>(uci.ucis[0].pdu).harqs[0] =
      mac_harq_ack_report_status::nack;
  this->sched->handle_uci_indication(uci);

  // Msg4 re-transmission should be scheduled.
  ASSERT_TRUE(this->run_slot_until([this]() { return find_ue_pdsch(rnti, *this->last_sched_result()) != nullptr; }));
  const dl_msg_alloc* msg4_alloc = find_ue_pdsch(rnti, *this->last_sched_result());
  ASSERT_EQ(msg4_alloc->tb_list.size(), 0) << "Retransmissions should not have a TB list";
  ASSERT_EQ(msg4_alloc->pdsch_cfg.dci_fmt, dci_dl_format::f1_0);
  ASSERT_EQ(find_ue_dl_pdcch(rnti)->dci.type(), dci_dl_rnti_config_type::tc_rnti_f1_0);
  ASSERT_TRUE(this->last_sched_result()->dl.csi_rs.empty());

  // Ensure the retransmission uses common PUCCH.
  const pucch_info* msg4_retx_pucch = run_until_common_pucch_with_harq(rnti);
  ASSERT_NE(msg4_retx_pucch, nullptr) << "Failed to schedule Msg4 retransmission PUCCH using common resources";
  bool found_common_pucch = false;
  for (const auto& pucch : this->last_sched_result()->ul.pucchs) {
    found_common_pucch |= is_f1_pucch_with_harq_bits(pucch, true, false);
  }
  ASSERT_TRUE(found_common_pucch) << "Failed to schedule Msg4 retransmission PUCCH using common resources";
}

INSTANTIATE_TEST_SUITE_P(scheduler_con_res_msg4_test_with_different_srb,
                         scheduler_con_res_msg4_test,
                         ::testing::Values(conres_test_params{LCID_SRB0, duplex_mode::TDD},
                                           conres_test_params{LCID_SRB0, duplex_mode::FDD},
                                           conres_test_params{LCID_SRB1, duplex_mode::FDD},
                                           conres_test_params{LCID_SRB1, duplex_mode::TDD}));

struct conres_expiry_params {
  subcarrier_spacing max_scs        = subcarrier_spacing::kHz15;
  unsigned           ntn_cs_koffset = 0; // in ms (slots at 15 kHz)
  unsigned           ntn_k_mac      = 0; // in ms (slots at 15 kHz)
};

/// Formatter for test params.
void PrintTo(const conres_expiry_params& value, ::std::ostream* os)
{
  *os << fmt::format(
      "SCS={}, Cell-Specific-K-Offset={}, K-mac={}", to_string(value.max_scs), value.ntn_cs_koffset, value.ntn_k_mac);
}

class scheduler_conres_expiry_test : public scheduler_test_simulator,
                                     public ::testing::TestWithParam<conres_expiry_params>
{
protected:
  scheduler_conres_expiry_test() :
    scheduler_test_simulator(
        scheduler_test_sim_config{.max_scs        = GetParam().max_scs,
                                  .ntn_cs_koffset = std::chrono::milliseconds(GetParam().ntn_cs_koffset)})
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-a");

    // Create cell.
    auto cell_cfg_req = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    if (ntn_cs_koffset != std::chrono::milliseconds{0} || GetParam().ntn_k_mac != 0) {
      cell_cfg_req.ran.ntn_params.emplace();
      if (ntn_cs_koffset != std::chrono::milliseconds{0}) {
        cell_cfg_req.ran.ntn_params->ntn_cfg.cell_specific_koffset = ntn_cs_koffset;
      }
      if (GetParam().ntn_k_mac != 0) {
        cell_cfg_req.ran.ntn_params->ntn_cfg.k_mac = std::chrono::milliseconds{GetParam().ntn_k_mac};
      }
    }
    add_cell(cell_cfg_req);
    // No PRACH slot for this UE, so the window is anchored on the Msg3 reception slot, already on the gNB DL clock,
    // and is the plain timer. With a PRACH slot it would move with the K-offset and k_mac.
    conres_win_slots = cell_cfg(cell_index).params.ul_cfg_common.init_ul_bwp.rach_cfg_common->ra_con_res_timer.count() *
                       next_slot.nof_slots_per_subframe();
  }

  void create_ue(unsigned ue_creation_delay_slots = 0)
  {
    auto ue_cfg     = sched_config_helper::create_default_sched_ue_creation_request(cell_cfg(cell_index).params, {});
    ue_cfg.ue_index = ue_index;
    ue_cfg.crnti    = rnti;
    ue_cfg.starts_in_fallback = true;
    ul_ccch_slot_rx           = next_slot.without_hyper_sfn() - ue_creation_delay_slots;
    ue_cfg.ul_ccch_slot_rx    = ul_ccch_slot_rx;
    scheduler_test_simulator::add_ue(ue_cfg, false);
    conres_expiry_slot = ul_ccch_slot_rx + conres_win_slots;
  }

  cell_config_builder_params builder_params{cell_config_builder_profiles::create(duplex_mode::TDD)};
  const du_cell_index_t      cell_index = to_du_cell_index(0);
  const du_ue_index_t        ue_index   = to_du_ue_index(0);
  const rnti_t               rnti       = to_rnti(0x4601);
  unsigned                   conres_win_slots;
  slot_point                 ul_ccch_slot_rx;
  slot_point                 conres_expiry_slot;
};

TEST_P(scheduler_conres_expiry_test, when_conres_ce_arrives_after_conres_timer_expires_then_no_pdsch_is_scheduled)
{
  // Create UE with a delay large enough that the ConRes timer is already expired at creation time. The
  // auto-injected ConRes CE should be immediately discarded by the scheduler.
  create_ue(conres_win_slots + test_rng::uniform_int<unsigned>(0, 10));

  // Ensure the ConRes CE is not scheduled.
  ASSERT_FALSE(this->run_slot_until(
      [this]() { return find_ue_pdsch(rnti, *this->last_sched_result(cell_index)) != nullptr; }, 100));
}

TEST_P(scheduler_conres_expiry_test, when_conres_retx_goes_after_conres_timer_expiry_it_is_not_scheduled)
{
  // Create UE with a delay close to the ConRes window so the first newTx is scheduled near expiry.
  create_ue(conres_win_slots - 10);
  auto pdsch_is_sched = [this]() { return find_ue_pdsch(rnti, *this->last_sched_result(cell_index)) != nullptr; };

  // Get closer to the conRes expiry slot (0 iterations with the chosen delay).
  while (next_slot.without_hyper_sfn() < conres_expiry_slot - 10) {
    run_slot();
    ASSERT_FALSE(pdsch_is_sched());
  }

  // Wait for newTx with ConRes CE to be scheduled (auto-injected at UE creation).
  ASSERT_TRUE(this->run_slot_until(pdsch_is_sched, 100));

  // Wait for common PUCCH.
  ASSERT_TRUE(this->run_slot_until(
      [this]() { return find_ue_pucch(rnti, *this->last_sched_result(cell_index)) != nullptr; }, 100));

  // Enqueue a NACK after ConRes timer expires.
  uci_indication uci_ind;
  uci_ind.cell_index = cell_index;
  uci_ind.slot_rx    = next_slot.without_hyper_sfn() - 1;
  for (const pucch_info& pucch : this->last_sched_result(cell_index)->ul.pucchs) {
    uci_ind.ucis.push_back(test_helper::create_uci_indication_pdu(ue_index, pucch, mac_harq_ack_report_status::nack));
  }
  while (next_slot.without_hyper_sfn() < conres_expiry_slot) {
    run_slot();
    ASSERT_FALSE(pdsch_is_sched());
  }
  this->sched->handle_uci_indication(uci_ind);

  // No PDSCH should be scheduled, as the ConRes timer has expired.
  ASSERT_FALSE(this->run_slot_until(pdsch_is_sched, 100));
}

TEST_P(scheduler_conres_expiry_test, when_conres_ce_arrives_within_conres_timer_then_pdsch_is_scheduled)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-TIM-1");

  // Create the UE with no delay, so the ConRes window is wide open in both terrestrial and NTN cells.
  create_ue();
  static constexpr unsigned msg4_size = 128;
  auto pdsch_is_sched = [this]() { return find_ue_pdsch(rnti, *this->last_sched_result(cell_index)) != nullptr; };

  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, LCID_SRB1, msg4_size});

  // ConRes CE was auto-injected at UE creation; PDSCH should be scheduled within the remaining timer window.
  ASSERT_TRUE(this->run_slot_until(pdsch_is_sched, 100));
}

INSTANTIATE_TEST_SUITE_P(scheduler_conres_expiry_test,
                         scheduler_conres_expiry_test,
                         ::testing::Values(
                             // Non-NTN baseline.
                             conres_expiry_params{subcarrier_spacing::kHz30, 0, 0},
                             // NTN with koffset only.
                             conres_expiry_params{subcarrier_spacing::kHz30, 240, 0},
                             conres_expiry_params{subcarrier_spacing::kHz30, 480, 0},
                             // NTN with k_mac only (DL/UL timing offset at gNB, koffset absent).
                             conres_expiry_params{subcarrier_spacing::kHz30, 0, 10},
                             // NTN with both koffset and k_mac.
                             conres_expiry_params{subcarrier_spacing::kHz30, 240, 10}));

/// Fallback DL scheduling in an NTN cell, with HARQ-ACKs k1 + Koffset slots after the PDSCH, as per TS 38.213,
/// Section 9.
class scheduler_ntn_fallback_test : public scheduler_test_simulator, public ::testing::Test
{
protected:
  scheduler_ntn_fallback_test() :
    scheduler_test_simulator(
        scheduler_test_sim_config{.auto_uci = true, .ntn_cs_koffset = std::chrono::milliseconds{koffset_ms}})
  {
    auto cell_cfg_req = sched_config_helper::make_default_sched_cell_configuration_request(
        cell_config_builder_profiles::create(duplex_mode::FDD));
    cell_cfg_req.ran.ntn_params.emplace();
    cell_cfg_req.ran.ntn_params->ntn_cfg.cell_specific_koffset = std::chrono::milliseconds{koffset_ms};
    add_cell(cell_cfg_req);

    auto ue_cfg               = sched_config_helper::create_default_sched_ue_creation_request(cell_cfg().params, {});
    ue_cfg.ue_index           = ue_index;
    ue_cfg.crnti              = rnti;
    ue_cfg.starts_in_fallback = true;
    ue_cfg.ul_ccch_slot_rx    = next_slot.without_hyper_sfn();
    scheduler_test_simulator::add_ue(ue_cfg, false);
  }

  /// Koffset of a GEO cell.
  static constexpr unsigned koffset_ms = 240;
  const du_ue_index_t       ue_index   = to_du_ue_index(0);
  const rnti_t              rnti       = to_rnti(0x4601);
};

TEST_F(scheduler_ntn_fallback_test, pdsch_is_not_held_back_by_the_koffset_of_the_previous_harq_ack)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-TIM-1");

  // Enough SRB1 data for several fallback PDSCHs.
  static constexpr unsigned srb1_bytes = 10000;
  static constexpr unsigned nof_pdschs = 4;
  // Slack for slots taken by SSB, SIB or CSI-RS.
  static constexpr int    max_pdsch_gap = 10;
  const unsigned          koffset       = cell_cfg().ntn_cs_koffset;
  const unsigned          max_nof_slots = 4 * koffset + 1000;
  std::vector<slot_point> pdsch_slots;
  // HARQ-ACK slots the DCIs indicate, and slots with a HARQ-ACK PUCCH of the UE.
  std::vector<slot_point> harq_ack_slots;
  std::vector<slot_point> harq_ack_pucch_slots;
  bool                    first_pdsch_has_conres = false;
  this->push_dl_buffer_state(dl_buffer_state_indication_message{ue_index, LCID_SRB1, srb1_bytes});

  for (unsigned count = 0;
       count != max_nof_slots and (pdsch_slots.size() != nof_pdschs or last_result_slot() < harq_ack_slots.back());
       ++count) {
    run_slot();
    const auto& pucchs = last_sched_result()->ul.pucchs;
    if (std::any_of(pucchs.begin(), pucchs.end(), [this](const pucch_info& pucch) {
          return pucch.crnti == rnti and pucch.uci_bits.harq_ack_nof_bits > 0;
        })) {
      harq_ack_pucch_slots.push_back(last_result_slot());
    }

    const dl_msg_alloc* pdsch = find_ue_pdsch(rnti, *last_sched_result());
    if (pdsch == nullptr or pdsch_slots.size() == nof_pdschs) {
      continue;
    }
    if (pdsch_slots.empty()) {
      const auto& lcs        = pdsch->tb_list.front().lc_chs_to_sched;
      first_pdsch_has_conres = std::any_of(
          lcs.begin(), lcs.end(), [](const dl_msg_lc_info& lc) { return lc.lcid == lcid_dl_sch_t::UE_CON_RES_ID; });
    }

    // DCI 1_0 shares the slot of its PDSCH. Its PDSCH-to-HARQ_feedback timing indicator maps to k1 in {1, ..., 8}, as
    // per TS 38.213, Section 9.2.3.
    const pdcch_dl_information* pdcch = find_ue_dl_pdcch(rnti);
    ASSERT_NE(pdcch, nullptr) << fmt::format("No DCI for the PDSCH in slot {}", last_result_slot());
    const dci_dl_rnti_config_type dci_type = pdcch->dci.type();
    ASSERT_TRUE(dci_type == dci_dl_rnti_config_type::tc_rnti_f1_0 or dci_type == dci_dl_rnti_config_type::c_rnti_f1_0);
    const unsigned k1 = 1 + (dci_type == dci_dl_rnti_config_type::tc_rnti_f1_0
                                 ? pdcch->dci.as_tc_rnti_f1_0().pdsch_harq_fb_timing_indicator
                                 : pdcch->dci.as_c_rnti_f1_0().pdsch_harq_fb_timing_indicator);
    pdsch_slots.push_back(last_result_slot());
    harq_ack_slots.push_back(last_result_slot() + k1 + koffset);
  }
  ASSERT_EQ(pdsch_slots.size(), nof_pdschs) << "The SRB1 data was not scheduled";

  // Every indicated HARQ-ACK has its PUCCH.
  for (const slot_point harq_ack_slot : harq_ack_slots) {
    ASSERT_NE(std::find(harq_ack_pucch_slots.begin(), harq_ack_pucch_slots.end(), harq_ack_slot),
              harq_ack_pucch_slots.end())
        << fmt::format("No HARQ-ACK PUCCH in slot {}, which the DCI indicates", harq_ack_slot);
  }

  // Nothing follows the ConRes CE before its HARQ-ACK.
  ASSERT_TRUE(first_pdsch_has_conres) << "The first PDSCH does not carry the ConRes CE";
  ASSERT_GT(pdsch_slots[1], harq_ack_slots[0]) << fmt::format(
      "PDSCH in slot {} scheduled before the HARQ-ACK in slot {} of the ConRes CE", pdsch_slots[1], harq_ack_slots[0]);

  // HARQ-ACKs keep the PDSCH order, as per TS 38.214, Section 5.1, one per slot in fallback, as per TS 38.213,
  // Section 9.2.1.
  for (unsigned i = 1; i != harq_ack_slots.size(); ++i) {
    ASSERT_GT(harq_ack_slots[i], harq_ack_slots[i - 1])
        << fmt::format("HARQ-ACK of the PDSCH in slot {} does not follow the one of the PDSCH in slot {}",
                       pdsch_slots[i],
                       pdsch_slots[i - 1]);
  }

  // The Koffset delays every HARQ-ACK alike, so keeping their order never holds a PDSCH back.
  for (unsigned i = 2; i != pdsch_slots.size(); ++i) {
    ASSERT_LE(pdsch_slots[i] - pdsch_slots[i - 1], max_pdsch_gap)
        << fmt::format("PDSCH in slot {} was held back since the PDSCH in slot {}", pdsch_slots[i], pdsch_slots[i - 1]);
  }
}

/// \brief Fallback DL scheduling in an NTN cell while an RRC Reconfiguration adds a Configured Grant.
///
/// Until the UE confirms the CG, its HARQ-ACKs, k1 + Koffset slots after the PDSCH, stay off the CG slots. The CG
/// scheduler moves a HARQ-ACK on a CG slot onto the CG PUSCH, so both channels are checked.
class scheduler_ntn_fallback_cg_test : public scheduler_test_simulator, public ::testing::Test
{
protected:
  scheduler_ntn_fallback_cg_test() :
    scheduler_test_simulator(
        scheduler_test_sim_config{.auto_uci = true, .ntn_cs_koffset = std::chrono::milliseconds{koffset_ms}})
  {
    auto cell_req = sched_config_helper::make_default_sched_cell_configuration_request(
        cell_config_builder_profiles::create(duplex_mode::FDD));
    cell_req.ran.ntn_params.emplace();
    cell_req.ran.ntn_params->ntn_cfg.cell_specific_koffset = std::chrono::milliseconds{koffset_ms};
    cell_req.ran.init_bwp.cg_cfg                           = cg_builder_params{.periodicity = cg_period};
    add_cell(cell_req);

    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = ue_index;
    ue_req.crnti    = rnti;
    auto& cg_cfg    = ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg;
    ocudu_assert(cg_cfg.has_value(), "The UE was not given the CG of the cell");
    cg_cfg->rrc_configured_ul_grant_cfg->time_domain_offset = cg_offset;

    // An RRC Reconfiguration adds the CG.
    auto ue_req_no_cg = ue_req;
    ue_req_no_cg.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();
    add_ue(ue_req_no_cg, /*wait_notification=*/true);

    // Left unconfirmed, so the UE stays in fallback.
    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = ue_index;
    reconf.crnti    = rnti;
    reconf.cfg      = ue_req.cfg;
    reconf.cs_rnti  = cs_rnti;
    sched->handle_ue_reconfiguration_request(reconf);
  }

  bool is_cg_slot(slot_point sl) const { return sl.count() % static_cast<unsigned>(cg_period) == cg_offset; }

  /// Returns whether the last slot has a HARQ-ACK of the UE, on PUCCH or PUSCH.
  bool has_harq_ack() const
  {
    const auto& ul       = last_sched_result()->ul;
    const auto& pucchs   = ul.pucchs;
    const bool  on_pucch = std::any_of(pucchs.begin(), pucchs.end(), [this](const pucch_info& pucch) {
      return pucch.crnti == rnti and pucch.uci_bits.harq_ack_nof_bits > 0;
    });
    const bool  on_pusch = std::any_of(ul.puschs.begin(), ul.puschs.end(), [this](const ul_sched_info& pusch) {
      return (pusch.pusch_cfg.rnti == rnti or pusch.pusch_cfg.rnti == cs_rnti) and pusch.uci.has_value() and
             pusch.uci->harq.has_value() and pusch.uci->harq->harq_ack_nof_bits > 0;
    });
    return on_pucch or on_pusch;
  }

  /// Not a multiple of the CG period, so the Koffset shifts the CG phase.
  static constexpr unsigned                        koffset_ms = 240;
  static constexpr cg_configuration::periodicity_t cg_period  = cg_configuration::periodicity_t::sl32;
  static constexpr unsigned                        cg_offset  = 3;
  const du_ue_index_t                              ue_index   = to_du_ue_index(0);
  const rnti_t                                     rnti       = to_rnti(0x4601);
  const rnti_t                                     cs_rnti    = to_rnti(0xe0ef);
};

TEST_F(scheduler_ntn_fallback_cg_test, harq_ack_is_not_placed_in_a_cg_slot_past_the_koffset)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-TIM-1");

  // Enough SRB1 data for HARQ-ACKs in every slot of a CG period.
  static constexpr unsigned srb1_bytes        = 200000;
  const unsigned            min_nof_harq_acks = 2 * static_cast<unsigned>(cg_period);
  const unsigned            max_nof_slots     = 8 * cell_cfg().ntn_cs_koffset + 2000;
  this->push_dl_buffer_state(dl_buffer_state_indication_message{ue_index, LCID_SRB1, srb1_bytes});

  unsigned nof_harq_acks = 0;
  for (unsigned count = 0; count != max_nof_slots and nof_harq_acks != min_nof_harq_acks; ++count) {
    run_slot();
    if (has_harq_ack()) {
      ASSERT_FALSE(is_cg_slot(last_result_slot()))
          << fmt::format("HARQ-ACK in slot {}, which is a CG slot", last_result_slot());
      ++nof_harq_acks;
    }
  }
  ASSERT_EQ(nof_harq_acks, min_nof_harq_acks) << "Too few HARQ-ACKs were scheduled";
}

/// Fallback DL scheduling in an NTN cell that disables the DL HARQ feedback of all but the first HARQ processes, as
/// per TS 38.331, \c downlinkHARQ-FeedbackDisabled.
class scheduler_ntn_fallback_dl_feedback_disabled_test : public scheduler_test_simulator, public ::testing::Test
{
protected:
  scheduler_ntn_fallback_dl_feedback_disabled_test() :
    scheduler_test_simulator(
        scheduler_test_sim_config{.auto_uci = true, .ntn_cs_koffset = std::chrono::milliseconds{koffset_ms}})
  {
    auto cell_cfg_req = sched_config_helper::make_default_sched_cell_configuration_request(
        cell_config_builder_profiles::create(duplex_mode::FDD));
    cell_cfg_req.ran.ntn_params.emplace();
    cell_cfg_req.ran.ntn_params->ntn_cfg.cell_specific_koffset = std::chrono::milliseconds{koffset_ms};
    add_cell(cell_cfg_req);

    auto ue_cfg               = sched_config_helper::create_default_sched_ue_creation_request(cell_cfg().params, {});
    ue_cfg.ue_index           = ue_index;
    ue_cfg.crnti              = rnti;
    ue_cfg.starts_in_fallback = true;
    ue_cfg.ul_ccch_slot_rx    = next_slot.without_hyper_sfn();
    scheduler_test_simulator::add_ue(ue_cfg, /*wait_notification=*/true);

    // An RRC Reconfiguration disables the DL HARQ feedback, which the UE reports it supports. Left unconfirmed, so the
    // UE stays in fallback.
    auto& pdsch_serv_cell = ue_cfg.cfg.cells->front().ded->serv_cell_cfg.pdsch_serv_cell_cfg;
    ocudu_assert(pdsch_serv_cell.has_value(), "The UE was not given a PDSCH serving cell config");
    pdsch_serv_cell->dl_harq_feedback_disabled.fill(true);
    pdsch_serv_cell->dl_harq_feedback_disabled.fill(0, nof_feedback_enabled_harqs, false);

    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = ue_index;
    reconf.crnti    = rnti;
    reconf.cfg      = ue_cfg.cfg;
    sched->handle_ue_reconfiguration_request(reconf);
  }

  /// Koffset of a GEO cell.
  static constexpr unsigned koffset_ms = 240;
  /// HARQ processes the configuration value "true" leaves with the feedback enabled.
  static constexpr unsigned nof_feedback_enabled_harqs = 4;
  const du_ue_index_t       ue_index                   = to_du_ue_index(0);
  const rnti_t              rnti                       = to_rnti(0x4601);
};

TEST_F(scheduler_ntn_fallback_dl_feedback_disabled_test, srb_grants_do_not_reuse_a_harq_process_awaiting_its_ack)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-2");

  // Enough SRB1 data to keep the fallback scheduler asking for HARQ processes.
  static constexpr unsigned srb1_bytes = 200000;
  // Several rounds of the feedback-enabled processes, so one has to be released and reused.
  const unsigned min_nof_pdschs = 3 * nof_feedback_enabled_harqs;
  const unsigned max_nof_slots  = 8 * cell_cfg().ntn_cs_koffset + 2000;
  // Slot of the HARQ-ACK of the last new transmission of each HARQ process, as its DCI indicates it.
  std::vector<slot_point> ack_slots(MAX_NOF_HARQS);
  this->push_dl_buffer_state(dl_buffer_state_indication_message{ue_index, LCID_SRB1, srb1_bytes});

  unsigned nof_pdschs = 0;
  for (unsigned count = 0; count != max_nof_slots and nof_pdschs != min_nof_pdschs; ++count) {
    run_slot();
    const dl_msg_alloc* pdsch = find_ue_pdsch(rnti, *last_sched_result());
    if (pdsch == nullptr or pdsch->context.nof_retxs != 0) {
      continue;
    }
    const auto h_id = static_cast<unsigned>(pdsch->pdsch_cfg.harq_id);
    ASSERT_FALSE(ack_slots[h_id].valid() and ack_slots[h_id] > last_result_slot())
        << fmt::format("New transmission in slot {} takes HARQ process {}, whose HARQ-ACK of a previous transmission "
                       "is due in slot {}",
                       last_result_slot(),
                       h_id,
                       ack_slots[h_id]);
    ack_slots[h_id] = last_result_slot() + pdsch->context.k1 + cell_cfg().ntn_cs_koffset;
    ++nof_pdschs;
  }
  ASSERT_EQ(nof_pdschs, min_nof_pdschs) << "The fallback scheduler stopped delivering the SRB1 data";
}

class scheduler_ue_no_config_test : public base_scheduler_conres_test, public ::testing::Test
{
protected:
  scheduler_ue_no_config_test() : base_scheduler_conres_test(duplex_mode::TDD)
  {
    // Create cell.
    auto cell_cfg_req = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    add_cell(cell_cfg_req);

    // Create a UE without dedicated config.
    auto ue_cfg     = sched_config_helper::create_empty_spcell_cfg_sched_ue_creation_request(cell_cfg_req.ran);
    ue_cfg.ue_index = ue_index;
    ue_cfg.crnti    = rnti;
    ue_cfg.starts_in_fallback = true;
    ue_cfg.ul_ccch_slot_rx    = next_slot.without_hyper_sfn();
    scheduler_test_simulator::add_ue(ue_cfg, false);
  }

  const du_cell_index_t cell_index = to_du_cell_index(0);
  const du_ue_index_t   ue_index   = to_du_ue_index(0);
  const rnti_t          rnti       = to_rnti(0x4601);
  const unsigned        msg4_size  = 128;
};

TEST_F(scheduler_ue_no_config_test, when_ue_has_no_dedicated_cfg_then_msg4_and_conres_are_still_scheduled)
{
  this->push_dl_buffer_state(dl_buffer_state_indication_message{this->ue_index, LCID_SRB0, msg4_size});

  // Wait for ConRes (and optionally Msg4) to be scheduled.
  ASSERT_TRUE(run_until_conres_msg4_scheduled(rnti, lcid_dl_sch_t::UE_CON_RES_ID)) << "ConRes not scheduled";
  const dl_msg_alloc* conres_alloc = find_ue_pdsch(rnti, *this->last_sched_result());
  ASSERT_EQ(conres_alloc->tb_list.size(), 1);
  ASSERT_FALSE(conres_alloc->tb_list[0].lc_chs_to_sched.empty());
  ASSERT_EQ(conres_alloc->tb_list[0].lc_chs_to_sched[0].lcid, lcid_dl_sch_t::UE_CON_RES_ID);

  // If the allocation is done in a slot where the SSB is also allocated, then there is only space for ConRes. The Msg4
  // will be sent in a separate PDSCH grant.
  const auto* msg4_lc_info = get_msg4_msg_lc_info(rnti, LCID_SRB0);
  if (msg4_lc_info == nullptr) {
    // If only the ConRes was scheduled, we need to wait for the PUCCH to be sent and inject an ACK, to confirm the
    // ConRes was received by the UE. Without this, the scheduler won't allocate any other PDSCH grant.
    ASSERT_TRUE(run_until_pucch_scheduled_and_received(rnti, ue_index, /* ACK */ true));
    ASSERT_TRUE(run_until_conres_msg4_scheduled(rnti, LCID_SRB0)) << "Msg4 not scheduled";
  }

  // Enqueue HARQ NACK for Msg4.
  ASSERT_TRUE(run_until_pucch_scheduled_and_received(rnti, ue_index, /* ACK */ false));

  // Wait for Msg4 re-transmission to be scheduled.
  ASSERT_TRUE(this->run_slot_until([this]() { return find_ue_dl_pdcch(rnti) != nullptr; }));
  const dl_msg_alloc* msg4_retx_alloc = find_ue_pdsch(rnti, *this->last_sched_result());
  ASSERT_FALSE(msg4_retx_alloc->pdsch_cfg.codewords[0].new_data);
}

/// UE without dedicated configuration in an NTN cell. In an NTN cell, a reconfiguration of the HARQ processes reads
/// the HARQ modes of the dedicated configuration.
class scheduler_ue_no_config_ntn_test : public scheduler_test_simulator, public ::testing::Test
{
protected:
  scheduler_ue_no_config_ntn_test() : scheduler_test_simulator(scheduler_test_sim_config{.ntn_cs_koffset = koffset})
  {
    auto cell_cfg_req = sched_config_helper::make_default_sched_cell_configuration_request();
    cell_cfg_req.ran.ntn_params.emplace();
    cell_cfg_req.ran.ntn_params->ntn_cfg.cell_specific_koffset = koffset;
    add_cell(cell_cfg_req);

    auto ue_cfg     = sched_config_helper::create_empty_spcell_cfg_sched_ue_creation_request(cell_cfg_req.ran);
    ue_cfg.ue_index = ue_index;
    ue_cfg.crnti    = rnti;
    ue_cfg.starts_in_fallback = true;
    ue_cfg.ul_ccch_slot_rx    = next_slot.without_hyper_sfn();
    add_ue(ue_cfg, /*wait_notification=*/true);
  }

  static constexpr std::chrono::milliseconds koffset{20};
  const du_ue_index_t                        ue_index = to_du_ue_index(0);
  const rnti_t                               rnti     = to_rnti(0x4601);
};

TEST_F(scheduler_ue_no_config_ntn_test, when_ue_has_no_dedicated_cfg_then_a_reconfiguration_without_cells_is_applied)
{
  ASSERT_EQ(notif.last_ue_index_cfg, ue_index) << "UE not created";

  // A RIC request changes the RRM policy of a UE without a change to its cells.
  sched_ue_reconfiguration_message reconf;
  reconf.ue_index  = ue_index;
  reconf.crnti     = rnti;
  reconf.cfg.cause = sched_ue_config_request::causes::not_rrc_proc;
  reconf.cfg.res_alloc_cfg.emplace();
  notif.last_ue_index_cfg.reset();
  sched->handle_ue_reconfiguration_request(reconf);

  ASSERT_TRUE(run_slot_until([this]() { return notif.last_ue_index_cfg == ue_index; }))
      << "Reconfiguration not applied";
}

// ------------------------------------------------------------------------------------------------------------------ //

/// Fixture that wires together a full scheduler instance with a cell configured for CFRA (preamble IDs [60, 64))
/// and a pre-created CFRA UE.
class cfra_scheduler_test : public scheduler_test_simulator, public ::testing::Test
{
  static constexpr unsigned NOF_CB_PREAMBLES = 60;

public:
  cfra_scheduler_test()
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-b");

    cell_config_builder_params bparams;
    auto                       cell_req = sched_config_helper::make_default_sched_cell_configuration_request(bparams);
    cell_req.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb = NOF_CB_PREAMBLES;
    add_cell(cell_req);

    auto ue_req = sched_config_helper::create_default_sched_ue_creation_request(
        cell_cfg().params, std::array<lcid_t, 3>{lcid_t::LCID_SRB1, lcid_t::LCID_SRB2, lcid_t::LCID_MIN_DRB});
    ue_req.crnti              = cfra_tc_rnti;
    ue_req.ue_index           = cfra_ue_index;
    ue_req.starts_in_fallback = true;
    ue_req.cfra_enabled       = true;
    ue_req.ul_ccch_slot_rx    = std::nullopt;
    add_ue(ue_req);
  }

  rach_indication_message create_cfra_rach_indication() const
  {
    const unsigned cfra_preamble_id =
        cell_cfg().params.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb;
    auto preamble = test_helper::create_preamble(cfra_preamble_id, cfra_tc_rnti);
    return test_helper::create_rach_indication(cell_cfg(), next_slot_rx(), {preamble});
  }

  void send_cfra_crc(bool success)
  {
    ul_crc_indication crc_ind;
    crc_ind.cell_index = to_du_cell_index(0);
    crc_ind.sl_rx      = last_result_slot();
    auto& pdu          = crc_ind.crcs.emplace_back();
    pdu.rnti           = cfra_tc_rnti;
    pdu.ue_index       = cfra_ue_index;
    pdu.harq_id        = to_harq_id(0);
    pdu.tb_crc_success = success;
    sched->handle_crc_indication(crc_ind);
  }

  const du_ue_index_t cfra_ue_index = to_du_ue_index(0);
  const rnti_t        cfra_tc_rnti  = to_rnti(0x4601);
};

/// Verify that no PUCCH is scheduled for the CFRA UE in the same slot as its Msg3 PUSCH.
///
/// A CFRA UE has UCI scheduling active from creation (it holds its dedicated config from the handover command),
/// so it would multiplex a coincident periodic CSI report onto the Msg3 PUSCH, which the RA scheduler builds
/// without UCI. The scheduler also forbids a UE from holding a PUCCH and a PUSCH in the same slot. The RA
/// scheduler therefore keeps the Msg3 PUSCH off any slot carrying a PUCCH for the UE.
TEST_F(cfra_scheduler_test, no_pucch_in_msg3_slot_while_cfra_pending)
{
  sched->handle_rach_indication(create_cfra_rach_indication());

  bool msg3_found = false;
  ASSERT_TRUE(run_slot_until([this, &msg3_found]() {
    const auto& puschs = last_sched_result()->ul.puschs;
    for (const auto& pusch : puschs) {
      if (pusch.pusch_cfg.rnti == cfra_tc_rnti) {
        EXPECT_EQ(find_ue_pucch(cfra_tc_rnti, last_sched_result()->ul.pucchs), nullptr)
            << "PUCCH scheduled for CFRA UE in the same slot as Msg3 PUSCH";
        msg3_found = true;
        return true;
      }
    }
    return false;
  })) << "Msg3 PUSCH was not scheduled within the timeout";

  ASSERT_TRUE(msg3_found);
}

/// Verify that after the Msg3 ACK (cfra_msg3_acked), the scheduler runs stably without any resource conflicts.
///
/// The CFRA UE transitions from pending_cfra to conres_completed, and uci_sched.add_ue is called.
/// Subsequent slots must not trigger any scheduling assertions.
TEST_F(cfra_scheduler_test, scheduler_is_stable_after_msg3_ack)
{
  sched->handle_rach_indication(create_cfra_rach_indication());

  // Run until Msg3 PUSCH is scheduled.
  ASSERT_TRUE(run_slot_until([this]() {
    const auto& puschs = last_sched_result()->ul.puschs;
    return std::any_of(
        puschs.begin(), puschs.end(), [this](const ul_sched_info& p) { return p.pusch_cfg.rnti == cfra_tc_rnti; });
  })) << "Msg3 PUSCH was not scheduled within the timeout";

  // Send CFRA CRC ACK: routes through both RA scheduler (clears Msg3 HARQ) and UE event manager
  // (cfra_msg3_acked → conres_completed → uci_sched.add_ue).
  send_cfra_crc(true);

  // Run several more slots to confirm no assertions fire after UCI scheduling is activated.
  for (unsigned i = 0; i < 30; ++i) {
    run_slot();
  }
}

/// Expert config with the CFRA UCI-on-Msg3 multiplexing disabled, so that the Msg3 PUCCH avoidance is exercised
/// deterministically regardless of the default.
static scheduler_expert_config make_no_uci_on_msg3_sched_cfg()
{
  auto cfg                                = config_helpers::make_default_scheduler_expert_config();
  cfg.ra.multiplex_uci_on_cf_rar_ul_grant = false;
  return cfg;
}

/// Fixture identical to \ref cfra_scheduler_test but with a dense periodic-CSI period, so that — once the RACH
/// trigger is swept across the CSI grid — a CSI report occasion reliably lands on a Msg3 candidate slot. This
/// exercises the RA-scheduler avoidance: Msg3 must never share a slot with the UE's CSI PUCCH.
class cfra_csi_collision_test : public scheduler_test_simulator, public ::testing::TestWithParam<unsigned>
{
  static constexpr unsigned NOF_CB_PREAMBLES = 60;

public:
  cfra_csi_collision_test() : scheduler_test_simulator(make_no_uci_on_msg3_sched_cfg())
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-b");

    cell_config_builder_params bparams;
    auto                       cell_req = sched_config_helper::make_default_sched_cell_configuration_request(bparams);
    cell_req.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb = NOF_CB_PREAMBLES;
    // Dense CSI so an occasion reliably collides with a swept Msg3 slot.
    if (cell_req.ran.init_bwp.csi.has_value()) {
      cell_req.ran.init_bwp.csi->csi_rs_period = csi_resource_periodicity::slots20;
    }
    add_cell(cell_req);

    auto ue_req = sched_config_helper::create_default_sched_ue_creation_request(
        cell_cfg().params, std::array<lcid_t, 3>{lcid_t::LCID_SRB1, lcid_t::LCID_SRB2, lcid_t::LCID_MIN_DRB});
    ue_req.crnti              = cfra_tc_rnti;
    ue_req.ue_index           = cfra_ue_index;
    ue_req.starts_in_fallback = true;
    ue_req.cfra_enabled       = true;
    ue_req.ul_ccch_slot_rx    = std::nullopt;
    add_ue(ue_req);
  }

  rach_indication_message create_cfra_rach_indication() const
  {
    const unsigned cfra_preamble_id =
        cell_cfg().params.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb;
    auto preamble = test_helper::create_preamble(cfra_preamble_id, cfra_tc_rnti);
    return test_helper::create_rach_indication(cell_cfg(), next_slot_rx(), {preamble});
  }

  void send_cfra_crc(bool success)
  {
    ul_crc_indication crc_ind;
    crc_ind.cell_index = to_du_cell_index(0);
    crc_ind.sl_rx      = last_result_slot();
    auto& pdu          = crc_ind.crcs.emplace_back();
    pdu.rnti           = cfra_tc_rnti;
    pdu.ue_index       = cfra_ue_index;
    pdu.harq_id        = to_harq_id(0);
    pdu.tb_crc_success = success;
    sched->handle_crc_indication(crc_ind);
  }

  const du_ue_index_t cfra_ue_index = to_du_ue_index(0);
  const rnti_t        cfra_tc_rnti  = to_rnti(0x4601);
};

TEST_P(cfra_csi_collision_test, msg3_pusch_never_shares_slot_with_csi_pucch)
{
  // Sweep the RACH trigger across the periodic-CSI grid so that, for some alignments, a CSI occasion would
  // otherwise fall on the Msg3 slot.
  for (unsigned i = 0; i != GetParam(); ++i) {
    run_slot();
  }

  sched->handle_rach_indication(create_cfra_rach_indication());

  bool msg3_seen      = false;
  bool csi_pucch_seen = false;
  for (unsigned i = 0; i != 40; ++i) {
    run_slot();
    const auto& res = *last_sched_result();
    if (find_ue_pucch_with_csi(cfra_tc_rnti, res.ul.pucchs) != nullptr) {
      csi_pucch_seen = true;
    }
    if (find_ue_pusch(cfra_tc_rnti, res.ul.puschs) != nullptr) {
      // While pending CFRA the only PUSCH for this RNTI is Msg3 (initial or retx). No PUCCH for the UE may
      // share its slot (a coincident CSI would otherwise be multiplexed onto the UCI-free Msg3 grant).
      msg3_seen = true;
      EXPECT_EQ(find_ue_pucch(cfra_tc_rnti, res.ul.pucchs), nullptr)
          << "PUCCH coincides with Msg3 PUSCH (rach_lead=" << GetParam() << ")";
    }
  }
  // Msg3 must still be scheduled (avoidance shifts it to a CSI-free slot rather than blocking it forever).
  EXPECT_TRUE(msg3_seen) << "Msg3 PUSCH was not scheduled (rach_lead=" << GetParam() << ")";
  // The early-activated periodic CSI PUCCH must be observable for the still-pending CFRA UE.
  EXPECT_TRUE(csi_pucch_seen) << "No CSI PUCCH observed for the pending CFRA UE (rach_lead=" << GetParam() << ")";
}

INSTANTIATE_TEST_SUITE_P(cfra_csi_sweep, cfra_csi_collision_test, ::testing::Range(0U, 20U));

/// Verify that a Msg3 *retransmission* for a CFRA UE also avoids slots carrying a PUCCH for it.
///
/// The initial Msg3 avoidance lives in schedule_rar; the retx path (schedule_msg3_retx) has its own guard. NACK
/// the initial Msg3 and check the retransmission never shares a slot with the UE's PUCCH.
TEST_P(cfra_csi_collision_test, msg3_retx_never_shares_slot_with_pucch)
{
  // Sweep the RACH trigger across the periodic-CSI grid to vary the retx/PUCCH alignment.
  for (unsigned i = 0; i != GetParam(); ++i) {
    run_slot();
  }

  sched->handle_rach_indication(create_cfra_rach_indication());

  // Wait for the initial Msg3 transmission.
  ASSERT_TRUE(run_slot_until([this]() {
    const ul_sched_info* pusch = find_ue_pusch(cfra_tc_rnti, last_sched_result()->ul.puschs);
    return pusch != nullptr and pusch->context.nof_retxs == 0;
  })) << "Initial Msg3 PUSCH was not scheduled (rach_lead="
      << GetParam() << ")";

  // NACK the initial Msg3 so a retransmission is scheduled.
  send_cfra_crc(false);

  // Wait for the retransmission and verify it does not share a slot with a PUCCH for the UE.
  bool retx_found = false;
  ASSERT_TRUE(run_slot_until([this, &retx_found]() {
    const ul_sched_info* pusch = find_ue_pusch(cfra_tc_rnti, last_sched_result()->ul.puschs);
    if (pusch != nullptr and pusch->context.nof_retxs > 0) {
      EXPECT_EQ(find_ue_pucch(cfra_tc_rnti, last_sched_result()->ul.pucchs), nullptr)
          << "PUCCH coincides with Msg3 retx (rach_lead=" << GetParam() << ")";
      retx_found = true;
      return true;
    }
    return false;
  })) << "Msg3 retx was not scheduled (rach_lead="
      << GetParam() << ")";

  ASSERT_TRUE(retx_found);
}

/// Bench for a single RAR grouping a CFRA UE (with dense periodic CSI) and a CBRA UE. Used to verify that the
/// per-UE Msg3 PUCCH avoidance does not push the CBRA UE off a slot just because the CFRA UE has a PUCCH there.
class cfra_multi_ue_rar_bench : public scheduler_test_simulator
{
  static constexpr unsigned NOF_CB_PREAMBLES = 60;

public:
  static constexpr unsigned CSI_PERIOD_SLOTS = 20;

  cfra_multi_ue_rar_bench() : scheduler_test_simulator(make_no_uci_on_msg3_sched_cfg())
  {
    cell_config_builder_params bparams;
    auto                       cell_req = sched_config_helper::make_default_sched_cell_configuration_request(bparams);
    cell_req.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb = NOF_CB_PREAMBLES;
    // Dense CSI so the CFRA UE reliably has a PUCCH coinciding with a Msg3 candidate slot.
    if (cell_req.ran.init_bwp.csi.has_value()) {
      cell_req.ran.init_bwp.csi->csi_rs_period = csi_resource_periodicity::slots20;
    }
    add_cell(cell_req);

    auto ue_req = sched_config_helper::create_default_sched_ue_creation_request(
        cell_cfg().params, std::array<lcid_t, 3>{lcid_t::LCID_SRB1, lcid_t::LCID_SRB2, lcid_t::LCID_MIN_DRB});
    ue_req.crnti              = cfra_tc_rnti;
    ue_req.ue_index           = cfra_ue_index;
    ue_req.starts_in_fallback = true;
    ue_req.cfra_enabled       = true;
    ue_req.ul_ccch_slot_rx    = std::nullopt;
    add_ue(ue_req);
  }

  /// Triggers the RAR at slot \c rach_lead of the CSI period and checks that both Msg3s are scheduled. Returns whether
  /// the CBRA Msg3 was scheduled in a slot where the CFRA UE had a PUCCH, i.e. the CFRA UE's PUCCH skip did not block
  /// the CBRA UE.
  bool run(unsigned rach_lead)
  {
    // Let the scheduler allocate the CFRA UE's periodic PUCCHs ahead of the Msg3 candidate slots, then align to the CSI
    // period, as each bench starts at a random slot.
    for (unsigned i = 0; i != CSI_PERIOD_SLOTS; ++i) {
      run_slot();
    }
    while (next_slot_rx().count() % CSI_PERIOD_SLOTS != rach_lead) {
      run_slot();
    }

    // Single RAR grouping one CFRA and one CBRA preamble in the same occasion (same RA-RNTI).
    const unsigned cfra_preamble_id =
        cell_cfg().params.ul_cfg_common.init_ul_bwp.rach_cfg_common->nof_cb_preambles_per_ssb;
    const std::vector<rach_indication_message::preamble> preambles = {
        test_helper::create_preamble(cfra_preamble_id, cfra_tc_rnti),
        test_helper::create_preamble(cbra_preamble_id, cbra_tc_rnti)};
    sched->handle_rach_indication(test_helper::create_rach_indication(cell_cfg(), next_slot_rx(), preambles));

    bool cfra_msg3_seen                 = false;
    bool cbra_msg3_seen                 = false;
    bool cbra_msg3_seen_with_cfra_pucch = false;
    for (unsigned i = 0; i != 40; ++i) {
      run_slot();
      const auto& res            = *last_sched_result();
      const bool  cfra_has_pucch = find_ue_pucch(cfra_tc_rnti, res.ul.pucchs) != nullptr;
      if (find_ue_pusch(cfra_tc_rnti, res.ul.puschs) != nullptr) {
        cfra_msg3_seen = true;
        EXPECT_FALSE(cfra_has_pucch) << "CFRA Msg3 shares a slot with the CFRA UE's PUCCH (rach_lead=" << rach_lead
                                     << ")";
      }
      if (find_ue_pusch(cbra_tc_rnti, res.ul.puschs) != nullptr) {
        cbra_msg3_seen = true;
        // The CBRA UE has no dedicated config, so it never has a PUCCH; if it is scheduled in a slot where the CFRA
        // UE does have one, the per-UE avoidance correctly served the CBRA UE without being blocked by the CFRA UE.
        cbra_msg3_seen_with_cfra_pucch |= cfra_has_pucch;
      }
    }

    EXPECT_TRUE(cfra_msg3_seen) << "CFRA Msg3 was not scheduled (rach_lead=" << rach_lead << ")";
    EXPECT_TRUE(cbra_msg3_seen) << "CBRA Msg3 was not scheduled (rach_lead=" << rach_lead << ")";
    return cbra_msg3_seen_with_cfra_pucch;
  }

  const du_ue_index_t cfra_ue_index    = to_du_ue_index(0);
  const rnti_t        cfra_tc_rnti     = to_rnti(0x4601);
  const rnti_t        cbra_tc_rnti     = to_rnti(0x4602);
  const unsigned      cbra_preamble_id = 0;
};

TEST(cfra_multi_ue_rar_test, cfra_pucch_does_not_block_other_ue_msg3_in_same_rar)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-1", "DU-GEN-2-a", "DU-GEN-2-b");

  // Sweep the RACH trigger across the periodic-CSI grid to vary the CFRA-PUCCH/Msg3 alignment. Only some alignments
  // put the CBRA Msg3 in a slot where the CFRA UE has a PUCCH, so the sweep as a whole must reach one.
  bool saw_cbra_msg3_with_cfra_pucch = false;
  for (unsigned rach_lead = 0; rach_lead != cfra_multi_ue_rar_bench::CSI_PERIOD_SLOTS; ++rach_lead) {
    cfra_multi_ue_rar_bench bench;
    saw_cbra_msg3_with_cfra_pucch |= bench.run(rach_lead);
  }

  EXPECT_TRUE(saw_cbra_msg3_with_cfra_pucch)
      << "No iteration scheduled the CBRA Msg3 in a slot where the CFRA UE had a PUCCH; the per-UE avoidance "
         "evidence is missing";
}

/// Fixture for the 2-step RACH (MsgA/MsgB) successRAR gate in the fallback scheduler: a UE can be created before
/// its successRAR MsgB is transmitted, and the fallback scheduler must not send anything for it until then.
class two_step_ra_fallback_scheduler_test : public scheduler_test_simulator, public ::testing::TestWithParam<bool>
{
protected:
  static constexpr unsigned MSGA_PREAMBLE_ID = 60;

  two_step_ra_fallback_scheduler_test() : scheduler_test_simulator(4, subcarrier_spacing::kHz30)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RACH-16-2", "DU-GEN-2-a");

    cell_config_builder_params bparams =
        cell_config_builder_profiles::create(duplex_mode::TDD, frequency_range::FR1, bs_channel_bandwidth::MHz50);
    bparams.min_k1 = 2;
    bparams.min_k2 = 2;
    // 10-slot (6D-1S-3U) period, td_offset equal to the period, so the MsgA PUSCH lands on the same UL slot
    // position as the PRACH occasion one period later.
    bparams.tdd_ul_dl_cfg_common = cell_config_builder_profiles::create_tdd_pattern(
        cell_config_builder_profiles::tdd_pattern_profile_fr1_30khz::DDDDDDSUUU);
    auto  cell_req = sched_config_helper::make_default_sched_cell_configuration_request(bparams);
    auto& rach     = *cell_req.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common;
    // Reserve preamble MSGA_PREAMBLE_ID for 2-step CB RACH.
    rach.nof_cb_preambles_per_ssb = MSGA_PREAMBLE_ID;
    rach.two_step_rach_cfg.emplace();
    // td_offset must equal the TDD period (10 slots), and be >= the simulator's tx_rx_delay (4), so the MsgA PUSCH
    // lands on a valid UL slot within the allocatable resource grid.
    rach.two_step_rach_cfg->pusch.td_offset = 10;
    add_cell(cell_req);
  }

  bool is_srb0() const { return GetParam(); }

  /// Advances to a valid PRACH occasion (required for MsgA: the RA scheduler pre-reserves the MsgA PUSCH from the
  /// PRACH config index). Called before \c create_ue so its ra-ContentionResolutionTimer doesn't also cover this
  /// search.
  void seek_prach_occasion()
  {
    const prach_helper::preamble_slot_mapping prach_mapper{
        cell_cfg().band(),
        cell_cfg().init_bwp.ul.cfg().scs,
        cell_cfg().init_bwp.ul.rach_common()->rach_cfg_generic.prach_config_index};
    run_slot_until([this, &prach_mapper]() { return prach_mapper.has_prach_occasion(next_slot_rx()); });
  }

  /// Creates the UE in fallback (pending ConRes CE) with pending LCID0/LCID1 traffic, using the C-RNTI that will
  /// arrive as the MsgA preamble's TC-RNTI. Call right before \c send_msga_rach.
  void create_ue()
  {
    auto ue_req               = sched_config_helper::create_default_sched_ue_creation_request(cell_cfg().params, {});
    ue_req.crnti              = tc_rnti;
    ue_req.ue_index           = ue_index;
    ue_req.starts_in_fallback = true;
    ue_req.ul_ccch_slot_rx    = next_slot.without_hyper_sfn();
    add_ue(ue_req);

    push_dl_buffer_state(dl_buffer_state_indication_message{ue_index, is_srb0() ? LCID_SRB0 : LCID_SRB1, 100});
  }

  /// Sends the MsgA RACH indication at the PRACH occasion found by \c seek_prach_occasion.
  void send_msga_rach()
  {
    last_prach_slot_rx = next_slot_rx();
    auto preamble      = test_helper::create_preamble(MSGA_PREAMBLE_ID, tc_rnti);
    sched->handle_rach_indication(test_helper::create_rach_indication(cell_cfg(), last_prach_slot_rx, {preamble}));
  }

  /// Injects a CRC result for the MsgA PUSCH, addressed by the (shared, per-occasion) RA-RNTI, as per TS 38.211.
  void send_msga_crc(bool success)
  {
    const rnti_t ra_rnti = test_helper::compute_ra_rnti(cell_cfg(), last_prach_slot_rx, 0, 0);

    ul_crc_indication crc_ind;
    crc_ind.cell_index = to_du_cell_index(0);
    crc_ind.sl_rx      = last_result_slot();
    auto& pdu          = crc_ind.crcs.emplace_back();
    pdu.rnti           = ra_rnti;
    pdu.ue_index       = INVALID_DU_UE_INDEX;
    pdu.harq_id        = to_harq_id(0);
    pdu.rapid          = static_cast<uint8_t>(MSGA_PREAMBLE_ID);
    pdu.tb_crc_success = success;
    sched->handle_crc_indication(crc_ind);
  }

  bool has_success_rar_for(rnti_t rnti) const
  {
    for (const auto& rar : last_sched_result()->dl.rar_grants) {
      for (const auto& g : rar.grants) {
        if (g.temp_crnti == rnti and std::holds_alternative<rar_ul_grant::two_step_success_info>(g.type)) {
          return true;
        }
      }
    }
    return false;
  }

  bool anything_scheduled_for(rnti_t rnti) const
  {
    return find_ue_dl_pdcch(rnti) != nullptr or find_ue_pdsch(rnti, *last_sched_result()) != nullptr;
  }

  slot_point last_prach_slot_rx;

  const du_ue_index_t ue_index = to_du_ue_index(0);
  const rnti_t        tc_rnti  = to_rnti(0x4601);
};

/// A UE with ConRes CE and LCID0/LCID1 data pending must not be scheduled until its successRAR MsgB is
/// transmitted, even though the grant may be decided several slots ahead of that.
TEST_P(two_step_ra_fallback_scheduler_test, ue_with_pending_traffic_not_scheduled_until_msgb_sent)
{
  seek_prach_occasion();
  send_msga_rach();

  // MsgA PUSCH isn't addressed by tc-rnti, so detect it via any UL grant instead. Then confirm CRC=OK.
  ASSERT_TRUE(run_slot_until([this]() { return not last_sched_result()->ul.puschs.empty(); }));
  send_msga_crc(true);
  create_ue();

  // Nothing may be scheduled for this UE until its successRAR MsgB is transmitted.
  bool success_rar_seen = false;
  ASSERT_TRUE(run_slot_until([this, &success_rar_seen]() {
    if (has_success_rar_for(tc_rnti)) {
      success_rar_seen = true;
      return true;
    }
    EXPECT_FALSE(anything_scheduled_for(tc_rnti)) << "UE scheduled before its successRAR MsgB was transmitted";
    return false;
  }));
  ASSERT_TRUE(success_rar_seen) << "successRAR MsgB was not scheduled";

  // Gate releases once the successRAR is transmitted.
  ASSERT_TRUE(run_slot_until([this]() { return anything_scheduled_for(tc_rnti); }))
      << "UE was never scheduled after its successRAR MsgB was transmitted";
}

INSTANTIATE_TEST_SUITE_P(srb0_srb1_gating_test, two_step_ra_fallback_scheduler_test, ::testing::Values(true, false));
