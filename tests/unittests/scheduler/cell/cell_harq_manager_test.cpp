// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/scheduler/cell/cell_harq_manager.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "ocudu/scheduler/result/sched_result.h"
#include <gtest/gtest.h>

using namespace ocudu;

namespace {

/// Slots a HARQ process owing no report waits before it is released, counted from the slot it was used. Mirrors
/// NTN_ACK_WAIT_TIMEOUT in the HARQ manager.
constexpr unsigned ntn_ack_wait_timeout = 4;

dl_msg_alloc make_dummy_ue_pdsch_info()
{
  dl_msg_alloc       msg;
  pdsch_information& pdsch = msg.pdsch_cfg;
  pdsch.rnti               = to_rnti(0x4601);
  pdsch.harq_id            = to_harq_id(0);
  pdsch.mcs_table          = ocudu::pdsch_mcs_table::qam64;
  pdsch.nof_layers         = 1;
  pdsch.codewords.resize(1);
  pdsch.codewords[0].mcs_index     = 10;
  pdsch.codewords[0].tb_size_bytes = units::bytes{10000};
  pdsch.rbs                        = vrb_interval{5, 10};
  msg.context.ue_index             = to_du_ue_index(0);
  msg.context.ss_id                = to_search_space_id(2);
  msg.tb_list.push_back(
      dl_msg_tb_info{{dl_msg_lc_info{lcid_dl_sch_t{LCID_SRB1}, pdsch.codewords[0].tb_size_bytes.value() - 4}}});
  return msg;
}

pusch_information make_dummy_pusch_info()
{
  pusch_information pusch;
  pusch.rnti          = to_rnti(0x4601);
  pusch.harq_id       = to_harq_id(0);
  pusch.mcs_table     = pusch_mcs_table::qam64;
  pusch.mcs_index     = 10;
  pusch.tb_size_bytes = units::bytes{10000};
  pusch.rbs           = vrb_interval{5, 10};
  return pusch;
}

// Dummy HARQ timeout recorder
class dummy_harq_timeout_handler
{
public:
  enum class last_event_t { none, feedback_timeout, retx_timeout, feedback_disabled_timeout };

  du_ue_index_t last_ue_index  = INVALID_DU_UE_INDEX;
  bool          last_dir_is_dl = false;
  bool          last_was_ack   = false;
  units::bytes  last_tbs{0};
  last_event_t  last_event = last_event_t::none;

  void handle_harq_timeout(du_ue_index_t ue_index, bool is_dl, bool ack)
  {
    last_ue_index  = ue_index;
    last_dir_is_dl = is_dl;
    last_was_ack   = ack;
    last_event     = last_event_t::feedback_timeout;
  }
  void handle_harq_retx_timeout(du_ue_index_t ue_index, bool is_dl)
  {
    last_ue_index  = ue_index;
    last_dir_is_dl = is_dl;
    last_event     = last_event_t::retx_timeout;
  }
  void handle_harq_no_feedback_timeout(du_ue_index_t ue_index, bool is_dl, units::bytes tbs)
  {
    last_ue_index  = ue_index;
    last_dir_is_dl = is_dl;
    last_tbs       = tbs;
    last_event     = last_event_t::feedback_disabled_timeout;
  }

  std::unique_ptr<harq_timeout_notifier> make_notifier()
  {
    class dummy_notifier : public harq_timeout_notifier
    {
    public:
      dummy_notifier(dummy_harq_timeout_handler& parent_) : parent(parent_) {}

      void on_feedback_timeout(du_ue_index_t ue_index, bool is_dl, bool ack) override
      {
        parent.handle_harq_timeout(ue_index, is_dl, ack);
      }
      void on_retx_timeout(du_ue_index_t ue_index, bool is_dl) override
      {
        parent.handle_harq_retx_timeout(ue_index, is_dl);
      }
      void on_feedback_disabled_harq_timeout(du_ue_index_t ue_index, bool is_dl, units::bytes tbs) override
      {
        parent.handle_harq_no_feedback_timeout(ue_index, is_dl, tbs);
      }

    private:
      dummy_harq_timeout_handler& parent;
    };

    return std::make_unique<dummy_notifier>(*this);
  }
};

// Base test class that instantiates a cell HARQ manager.
class base_harq_manager_test
{
protected:
  base_harq_manager_test(unsigned nof_ues, unsigned ntn_cs_koffset = 0, bool ul_harq_mode_b = false) :
    max_harqs_per_ue(ntn_cs_koffset > 0 ? MAX_NOF_HARQS : MAX_NOF_HARQS_NON_NTN),
    cell_harqs(nof_ues,
               nof_ues,
               max_harqs_per_ue,
               timeout_handler.make_notifier(),
               timeout_handler.make_notifier(),
               max_harq_retx_timeout,
               max_harq_retx_timeout,
               max_ack_wait_timeout,
               ntn_cs_koffset,
               ul_harq_mode_b)
  {
    logger.set_level(ocudulog::basic_levels::warning);
    ocudulog::init();

    ocudulog::fetch_basic_logger("SCHED").set_context(current_slot.sfn(), current_slot.slot_index());
    cell_harqs.slot_indication(current_slot);
  }

  void run_slot()
  {
    ++current_slot;
    ocudulog::fetch_basic_logger("SCHED").set_context(current_slot.sfn(), current_slot.slot_index());
    cell_harqs.slot_indication(current_slot);
  }

  const unsigned             max_ack_wait_timeout  = 16;
  const unsigned             max_harq_retx_timeout = 100;
  const unsigned             max_harqs_per_ue      = 16;
  dummy_harq_timeout_handler timeout_handler;
  ocudulog::basic_logger&    logger = ocudulog::fetch_basic_logger("SCHED");

  cell_harq_manager cell_harqs;

  slot_point current_slot{0, test_rng::uniform_int<unsigned>(0, 10239)};
};

class base_single_harq_entity_test : public base_harq_manager_test
{
protected:
  base_single_harq_entity_test(unsigned max_retxs_ = 4, unsigned ntn_cs_koffset_ = 0, bool ul_harq_mode_b = false) :
    base_harq_manager_test(1, ntn_cs_koffset_, ul_harq_mode_b)
  {
    max_retxs      = max_retxs_;
    ntn_cs_koffset = ntn_cs_koffset_;
  }

  const du_ue_index_t   ue_index  = to_du_ue_index(0);
  const rnti_t          rnti      = to_rnti(0x4601);
  const unsigned        nof_harqs = 8;
  unique_ue_harq_entity harq_ent  = cell_harqs.add_ue(ue_index, rnti, nof_harqs, nof_harqs);

  unsigned max_retxs = 4;
  unsigned k1        = 4;
  unsigned k2        = 6;
  unsigned ntn_cs_koffset;
};

// Test for multiple UEs managed by a single HARQ manager instance.
class multi_ue_harq_manager_test : public base_harq_manager_test, public ::testing::Test
{
protected:
  multi_ue_harq_manager_test() : base_harq_manager_test(max_ues) {}

  static constexpr unsigned max_ues   = 4;
  const unsigned            nof_harqs = 8;
};

// Test for a single UE HARQ entity.
class single_ue_harq_entity_test : public base_single_harq_entity_test, public ::testing::Test
{};

// Test suite for a single HARQ process.
class single_harq_process_test : public base_single_harq_entity_test, public ::testing::Test
{
protected:
  single_harq_process_test(unsigned max_retxs_ = 4, unsigned ntn_cs_koffset_ = 0, bool ul_harq_mode_b = false) :
    base_single_harq_entity_test(max_retxs_, ntn_cs_koffset_, ul_harq_mode_b)
  {
    ue_pdsch = make_dummy_ue_pdsch_info();
    dl_harq_alloc_context harq_ctxt{dci_dl_rnti_config_type::c_rnti_f1_0};
    h_dl.save_grant_params(harq_ctxt, ue_pdsch);
    pusch_info = make_dummy_pusch_info();
    ul_harq_alloc_context ul_harq_ctxt{dci_ul_rnti_config_type::c_rnti_f0_0};
    h_ul.save_grant_params(ul_harq_ctxt, pusch_info);
  }

  dl_msg_alloc           ue_pdsch;
  pusch_information      pusch_info;
  dl_harq_process_handle h_dl{
      harq_ent.alloc_dl_harq(current_slot, current_slot, k1 + ntn_cs_koffset, max_retxs, 0).value()};
  ul_harq_process_handle h_ul{
      harq_ent.alloc_ul_harq(current_slot, current_slot + k2 + ntn_cs_koffset, max_retxs).value()};
};

// In this test suite, we test the scenario where 5 HARQ bits arrive in a single PUCCH PDU to the scheduler.
class single_ue_harq_entity_harq_5bit_tester : public base_single_harq_entity_test, public ::testing::Test
{};

// Test for a single UE HARQ process in NTN mode A.
class single_ntn_ue_harq_normal_mode_process_test : public single_harq_process_test
{
public:
  single_ntn_ue_harq_normal_mode_process_test() : single_harq_process_test(0, NTN_CELL_SPECIFIC_KOFFSET_MAX, false) {}
};

// Test for a single UE HARQ process in NTN mode B.
class single_ntn_ue_ul_harq_mode_b_process_test : public single_harq_process_test
{
public:
  single_ntn_ue_ul_harq_mode_b_process_test() : single_harq_process_test(0, NTN_CELL_SPECIFIC_KOFFSET_MAX, true)
  {
    h_ul.cancel_unsent_tx();
    // Note: DL Feedback Disabled and UL HARQ Mode B is be set during RRC Reconf if UE supports it.
    // Need to enable Mode B and request new harq process.
    harq_dl_feedback_disabled_mask dl_feedback_disabled(MAX_NOF_HARQS);
    dl_feedback_disabled.fill(false);
    harq_ul_mode_mask ul_harq_mode_mask(MAX_NOF_HARQS);
    ul_harq_mode_mask.fill(false);
    ul_harq_mode_mask.fill(0, nof_normal_mode_harqs, true);
    constexpr unsigned nof_cg_reserved_harq = 0;
    harq_ent.reconfigure(nof_harqs, nof_harqs, dl_feedback_disabled, ul_harq_mode_mask, nof_cg_reserved_harq);
    h_ul = harq_ent
               .alloc_ul_harq(current_slot,
                              current_slot + k2 + ntn_cs_koffset,
                              max_retxs,
                              std::nullopt,
                              harq_utils::harq_mode_t::feedback_disabled_or_mode_b)
               .value();
    ul_harq_alloc_context ul_harq_ctxt{dci_ul_rnti_config_type::c_rnti_f0_0};
    pusch_info.harq_id = h_ul.id();
    h_ul.save_grant_params(ul_harq_ctxt, pusch_info);
  }
  static constexpr unsigned nof_normal_mode_harqs = 4;
};

// Test for a single UE HARQ process in NTN with DL feedback disabled, and UL mode A.
class single_ntn_ue_harq_dl_feedback_disabled_process_test : public single_harq_process_test
{
public:
  single_ntn_ue_harq_dl_feedback_disabled_process_test() :
    single_harq_process_test(0, NTN_CELL_SPECIFIC_KOFFSET_MAX, false)
  {
    h_dl.cancel_unsent_tx();
    // Note: DL HARQ Feedback Disabled can be set during RRC Reconf if UE supports it.
    harq_dl_feedback_disabled_mask dl_feedback_disabled(MAX_NOF_HARQS);
    dl_feedback_disabled.fill(true);
    dl_feedback_disabled.fill(0, nof_normal_mode_harqs, false);
    harq_ul_mode_mask ul_harq_mode_mask(MAX_NOF_HARQS);
    ul_harq_mode_mask.fill(true);
    constexpr unsigned nof_cg_reserved_harq = 0;
    harq_ent.reconfigure(nof_harqs, nof_harqs, dl_feedback_disabled, ul_harq_mode_mask, nof_cg_reserved_harq);
    h_dl = harq_ent
               .alloc_dl_harq(current_slot,
                              current_slot,
                              k1 + ntn_cs_koffset,
                              max_retxs,
                              0,
                              harq_utils::harq_mode_t::feedback_disabled_or_mode_b)
               .value();
    dl_harq_alloc_context harq_ctxt{dci_dl_rnti_config_type::c_rnti_f1_0};
    ue_pdsch.pdsch_cfg.harq_id = h_dl.id();
    h_dl.save_grant_params(harq_ctxt, ue_pdsch);
  }
  static constexpr unsigned nof_normal_mode_harqs = 4;
};

} // namespace

// HARQ process tests

TEST_F(single_harq_process_test, when_harq_is_allocated_then_it_enters_waiting_ack_state)
{
  ASSERT_TRUE(h_dl.is_waiting_ack());
  ASSERT_TRUE(h_ul.is_waiting_ack());
  ASSERT_FALSE(h_dl.has_pending_retx());
  ASSERT_FALSE(h_ul.has_pending_retx());
}

TEST_F(single_harq_process_test, when_harq_is_allocated_then_harq_params_have_correct_values)
{
  ASSERT_EQ(h_dl.pdsch_slot(), current_slot);
  ASSERT_EQ(h_dl.uci_slot(), current_slot + k1);
  ASSERT_EQ(h_dl.max_nof_retxs(), max_retxs);
  ASSERT_EQ(h_dl.nof_retxs(), 0);
  ASSERT_EQ(h_ul.pusch_slot(), current_slot + k2);
  ASSERT_EQ(h_ul.max_nof_retxs(), max_retxs);
  ASSERT_EQ(h_ul.nof_retxs(), 0);
}

TEST_F(single_harq_process_test, when_harq_is_allocated_then_harq_grant_params_have_correct_values)
{
  ASSERT_EQ(h_dl.get_grant_params().mcs, ue_pdsch.pdsch_cfg.codewords[0].mcs_index);
  ASSERT_EQ(h_dl.get_grant_params().mcs_table, ue_pdsch.pdsch_cfg.mcs_table);
  ASSERT_EQ(h_dl.get_grant_params().tbs, ue_pdsch.pdsch_cfg.codewords[0].tb_size_bytes);
  ASSERT_EQ(h_dl.get_grant_params().rbs.type1(), ue_pdsch.pdsch_cfg.rbs.type1());
  ASSERT_EQ(h_dl.get_grant_params().dci_cfg_type, dci_dl_rnti_config_type::c_rnti_f1_0);
  ASSERT_EQ(h_ul.get_grant_params().tbs, harq_ent.total_ul_bytes_waiting_ack());
}

TEST_F(single_harq_process_test, positive_ack_sets_harq_to_empty)
{
  float pucch_snr = 5;
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::ack, pucch_snr));
  ASSERT_FALSE(h_dl.is_waiting_ack());
  ASSERT_FALSE(h_dl.has_pending_retx());
  ASSERT_EQ(h_ul.ul_crc_info(true).value(), pusch_info.tb_size_bytes);
  ASSERT_FALSE(h_ul.is_waiting_ack());
  ASSERT_FALSE(h_ul.has_pending_retx());

  ASSERT_EQ(harq_ent.total_ul_bytes_waiting_ack().value(), 0);
}

TEST_F(single_harq_process_test, negative_ack_sets_harq_to_pending_retx)
{
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::nack, 5));
  ASSERT_FALSE(h_dl.is_waiting_ack());
  ASSERT_TRUE(h_dl.has_pending_retx());
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(current_slot + k2), h_ul);
  ASSERT_EQ(h_ul.ul_crc_info(false).value().value(), 0);
  ASSERT_FALSE(h_ul.is_waiting_ack());
  ASSERT_TRUE(h_ul.has_pending_retx());
}

TEST_F(single_harq_process_test, ack_of_empty_harq_is_failure)
{
  float pucch_snr = 5;
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::ack, pucch_snr));
  ASSERT_GE(h_ul.ul_crc_info(true).value().value(), 0);

  ASSERT_FALSE(h_dl.dl_ack_info(mac_harq_ack_report_status::ack, pucch_snr));
  ASSERT_FALSE(h_ul.ul_crc_info(true).has_value());
}

TEST_F(single_harq_process_test, retx_of_empty_harq_is_failure)
{
  float pucch_snr = 5;
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::ack, pucch_snr));
  ASSERT_GE(h_ul.ul_crc_info(true).value().value(), 0);

  ASSERT_FALSE(h_dl.new_retx(current_slot, k1, 0));
  ASSERT_FALSE(h_ul.new_retx(current_slot + k2));
}

TEST_F(single_harq_process_test, retx_of_harq_waiting_ack_is_failure)
{
  ASSERT_FALSE(h_dl.new_retx(current_slot, k1, 0));
  ASSERT_FALSE(h_ul.new_retx(current_slot + k2));
}

TEST_F(single_harq_process_test, when_max_retxs_reached_then_harq_becomes_empty)
{
  bool old_dl_ndi = h_dl.ndi();
  bool old_ul_ndi = h_ul.ndi();
  for (unsigned i = 0; i != max_retxs; ++i) {
    ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::nack, 5));
    ASSERT_EQ(h_ul.ul_crc_info(false), units::bytes{0});
    ASSERT_FALSE(h_dl.is_waiting_ack());
    ASSERT_FALSE(h_ul.is_waiting_ack());
    ASSERT_TRUE(h_dl.has_pending_retx());
    ASSERT_TRUE(h_ul.has_pending_retx());

    run_slot();
    ASSERT_TRUE(h_dl.new_retx(current_slot, k1, 0));
    ASSERT_TRUE(h_ul.new_retx(current_slot + k2));
    ASSERT_EQ(h_dl.nof_retxs(), i + 1);
    ASSERT_EQ(h_ul.nof_retxs(), i + 1);
    ASSERT_EQ(old_dl_ndi, h_dl.ndi());
    ASSERT_EQ(old_ul_ndi, h_ul.ndi());
  }
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::nack, 5));
  ASSERT_EQ(h_ul.ul_crc_info(false), units::bytes{0});
  ASSERT_FALSE(h_dl.has_pending_retx());
  ASSERT_FALSE(h_ul.has_pending_retx());
  ASSERT_FALSE(h_dl.is_waiting_ack());
  ASSERT_FALSE(h_ul.is_waiting_ack());
}

TEST_F(single_harq_process_test, when_newtx_after_ack_then_ndi_flips)
{
  float pucch_snr = 5;
  bool  dl_ndi = h_dl.ndi(), ul_ndi = h_ul.ndi();
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::ack, pucch_snr));
  ASSERT_GE(h_ul.ul_crc_info(true).value().value(), 0);

  h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).value();
  h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).value();
  ASSERT_EQ(h_dl.nof_retxs(), 0);
  ASSERT_EQ(h_ul.nof_retxs(), 0);
  ASSERT_NE(dl_ndi, h_dl.ndi());
  ASSERT_NE(ul_ndi, h_ul.ndi());
}

// TS 38.321, clauses 5.3.2.2 and 5.4.2.1: the UE compares the NDI with the one of the last transmission it received
// for the HARQ process. A newTx whose DCI is never sent must therefore not count as a toggle.
TEST_F(single_harq_process_test, when_unsent_newtx_is_cancelled_then_next_newtx_flips_ndi_wrt_last_sent_newtx)
{
  const harq_id_t dl_id = h_dl.id(), ul_id = h_ul.id();
  const bool      sent_dl_ndi = h_dl.ndi(), sent_ul_ndi = h_ul.ndi();
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::ack, 5));
  ASSERT_GE(h_ul.ul_crc_info(true).value().value(), 0);

  // NewTx allocated in the same HARQ processes, but cancelled before its DCI is sent.
  h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).value();
  h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).value();
  ASSERT_EQ(h_dl.id(), dl_id);
  ASSERT_EQ(h_ul.id(), ul_id);
  h_dl.cancel_unsent_tx();
  h_ul.cancel_unsent_tx();
  ASSERT_TRUE(h_dl.empty());
  ASSERT_TRUE(h_ul.empty());
  ASSERT_EQ(h_dl.ndi(), sent_dl_ndi);
  ASSERT_EQ(h_ul.ndi(), sent_ul_ndi);

  // The next newTx toggles the NDI w.r.t. the last DCI the UE received.
  h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).value();
  h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).value();
  ASSERT_EQ(h_dl.id(), dl_id);
  ASSERT_EQ(h_ul.id(), ul_id);
  ASSERT_NE(h_dl.ndi(), sent_dl_ndi);
  ASSERT_NE(h_ul.ndi(), sent_ul_ndi);
}

TEST_F(single_harq_process_test, when_unsent_retx_is_cancelled_then_harq_is_released_and_ndi_is_kept)
{
  const bool sent_dl_ndi = h_dl.ndi(), sent_ul_ndi = h_ul.ndi();
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::nack, 5));
  ASSERT_EQ(h_ul.ul_crc_info(false), units::bytes{0});
  run_slot();
  ASSERT_TRUE(h_dl.new_retx(current_slot, k1, 0));
  ASSERT_TRUE(h_ul.new_retx(current_slot + k2));

  // A reTx does not toggle the NDI, so cancelling it leaves the NDI of the last sent newTx in place.
  h_dl.cancel_unsent_tx();
  h_ul.cancel_unsent_tx();
  ASSERT_TRUE(h_dl.empty());
  ASSERT_TRUE(h_ul.empty());
  ASSERT_EQ(h_dl.ndi(), sent_dl_ndi);
  ASSERT_EQ(h_ul.ndi(), sent_ul_ndi);
}

// A transmission dropped after its DCI was sent: the UE received the NDI toggle, so the next newTx must toggle again.
TEST_F(single_harq_process_test, when_sent_tx_is_discarded_then_next_newtx_flips_ndi_wrt_discarded_tx)
{
  const harq_id_t dl_id = h_dl.id(), ul_id = h_ul.id();
  const bool      sent_dl_ndi = h_dl.ndi(), sent_ul_ndi = h_ul.ndi();

  h_dl.discard_sent_tx();
  h_ul.discard_sent_tx();
  ASSERT_TRUE(h_dl.empty());
  ASSERT_TRUE(h_ul.empty());
  ASSERT_EQ(h_dl.ndi(), sent_dl_ndi);
  ASSERT_EQ(h_ul.ndi(), sent_ul_ndi);

  h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).value();
  h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).value();
  ASSERT_EQ(h_dl.id(), dl_id);
  ASSERT_EQ(h_ul.id(), ul_id);
  ASSERT_NE(h_dl.ndi(), sent_dl_ndi);
  ASSERT_NE(h_ul.ndi(), sent_ul_ndi);
}

TEST_F(single_harq_process_test, when_harq_not_waiting_ack_is_cancelled_then_it_is_left_untouched)
{
  // HARQ process with a pending reTx.
  const bool sent_dl_ndi = h_dl.ndi(), sent_ul_ndi = h_ul.ndi();
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::nack, 5));
  ASSERT_EQ(h_ul.ul_crc_info(false), units::bytes{0});
  h_dl.cancel_unsent_tx();
  h_ul.cancel_unsent_tx();
  ASSERT_TRUE(h_dl.has_pending_retx());
  ASSERT_TRUE(h_ul.has_pending_retx());
  ASSERT_EQ(h_dl.ndi(), sent_dl_ndi);
  ASSERT_EQ(h_ul.ndi(), sent_ul_ndi);

  // Empty HARQ process.
  h_dl.discard_sent_tx();
  h_ul.discard_sent_tx();
  h_dl.cancel_unsent_tx();
  h_ul.cancel_unsent_tx();
  ASSERT_TRUE(h_dl.empty());
  ASSERT_TRUE(h_ul.empty());
  ASSERT_EQ(h_dl.ndi(), sent_dl_ndi);
  ASSERT_EQ(h_ul.ndi(), sent_ul_ndi);
}

TEST_F(single_harq_process_test, when_ack_wait_timeout_reached_then_harq_is_available_for_newtx)
{
  harq_id_t h_dl_id = h_dl.id(), h_ul_id = h_ul.id();
  bool      dl_ndi = h_dl.ndi(), ul_ndi = h_ul.ndi();
  for (unsigned i = 0; i != this->max_ack_wait_timeout + k1; ++i) {
    ASSERT_TRUE(h_dl.is_waiting_ack() and not h_dl.has_pending_retx());
    ASSERT_TRUE(h_ul.is_waiting_ack() and not h_ul.has_pending_retx());
    run_slot();
  }
  // Note: k1 < k2, so there is an intermediate state where h_dl timed out but h_ul didn't.
  ASSERT_TRUE(not h_dl.is_waiting_ack() and not h_dl.has_pending_retx());
  ASSERT_FALSE(harq_ent.dl_harq(h_dl_id).has_value());
  for (unsigned i = 0; i != (k2 - k1); ++i) {
    ASSERT_TRUE(h_ul.is_waiting_ack() and not h_ul.has_pending_retx());
    run_slot();
  }
  ASSERT_TRUE(not h_ul.is_waiting_ack() and not h_ul.has_pending_retx());
  ASSERT_FALSE(harq_ent.ul_harq(h_ul_id).has_value());

  h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs - 1, 0).value();
  h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs - 1).value();
  ASSERT_TRUE(h_dl.is_waiting_ack() and not h_dl.has_pending_retx());
  ASSERT_TRUE(h_ul.is_waiting_ack() and not h_ul.has_pending_retx());
  ASSERT_EQ(h_dl.nof_retxs(), 0);
  ASSERT_EQ(h_dl.max_nof_retxs(), max_retxs - 1);
  ASSERT_EQ(h_ul.nof_retxs(), 0);
  ASSERT_EQ(h_ul.max_nof_retxs(), max_retxs - 1);
  if (h_dl.id() == h_dl_id) {
    ASSERT_NE(dl_ndi, h_dl.ndi());
  }
  if (h_ul.id() == h_ul_id) {
    ASSERT_NE(ul_ndi, h_ul.ndi());
  }
}

TEST_F(single_harq_process_test, when_harq_retx_is_cancelled_while_harq_waits_ack_then_harq_nack_empties_it)
{
  h_dl.cancel_retxs();
  h_ul.cancel_retxs();
  ASSERT_TRUE(h_dl.is_waiting_ack());
  ASSERT_TRUE(h_ul.is_waiting_ack());

  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));
  ASSERT_EQ(h_ul.ul_crc_info(false), units::bytes{0});
  ASSERT_TRUE(h_dl.empty());
  ASSERT_TRUE(h_ul.empty());
}

TEST_F(single_harq_process_test,
       when_harq_retx_is_cancelled_while_harq_has_pending_retx_then_harq_is_emptied_right_away)
{
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));
  ASSERT_EQ(h_ul.ul_crc_info(false), units::bytes{0});
  ASSERT_TRUE(h_dl.has_pending_retx());
  ASSERT_TRUE(h_ul.has_pending_retx());

  h_dl.cancel_retxs();
  h_ul.cancel_retxs();
  ASSERT_TRUE(h_dl.empty());
  ASSERT_TRUE(h_ul.empty());
}

// HARQ entity tests

TEST_F(single_ue_harq_entity_test, when_harq_entity_is_created_all_harqs_are_empty)
{
  ASSERT_EQ(harq_ent.nof_dl_harqs(), this->nof_harqs);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), this->nof_harqs);
  ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_dl_retx(), std::nullopt);
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_ul_retx(), std::nullopt);
  ASSERT_TRUE(harq_ent.has_empty_dl_harqs());
  ASSERT_TRUE(harq_ent.has_empty_ul_harqs());
  for (unsigned i = 0; i != harq_ent.nof_dl_harqs(); ++i) {
    ASSERT_FALSE(harq_ent.dl_harq(to_harq_id(i)).has_value());
    ASSERT_FALSE(harq_ent.ul_harq(to_harq_id(i)).has_value());
  }
}

// Rel-16 PDSCH repetition tests for the UE's last known PDSCH slot (used by ue_cell::is_pdsch_enabled and the
// fallback scheduler to enforce that a UE is never scheduled a PDSCH that overlaps with another one).

TEST_F(single_ue_harq_entity_test, when_newtx_has_no_repetitions_then_last_pdsch_slot_matches_pdsch_slot)
{
  auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  ASSERT_TRUE(h_dl.has_value());
  dl_msg_alloc          ue_pdsch = make_dummy_ue_pdsch_info();
  dl_harq_alloc_context harq_ctxt{dci_dl_rnti_config_type::c_rnti_f1_0};
  h_dl->save_grant_params(harq_ctxt, ue_pdsch);

  ASSERT_EQ(h_dl->pdsch_slot(), current_slot);
  ASSERT_EQ(harq_ent.last_pdsch_slot(), current_slot);

  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::ack, std::nullopt));
  ASSERT_FALSE(harq_ent.last_pdsch_slot().valid());
}

TEST_F(single_ue_harq_entity_test, when_newtx_uses_pdsch_repetitions_then_last_pdsch_slot_covers_the_whole_bundle)
{
  constexpr uint8_t nof_reps = 4;

  auto h_dl =
      harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_dl.has_value());
  dl_msg_alloc          ue_pdsch = make_dummy_ue_pdsch_info();
  dl_harq_alloc_context harq_ctxt{dci_dl_rnti_config_type::c_rnti_f1_1};
  harq_ctxt.nof_repetitions = nof_reps;
  h_dl->save_grant_params(harq_ctxt, ue_pdsch);

  // pdsch_slot() always points at the first, PDCCH-scheduled occasion...
  ASSERT_EQ(h_dl->pdsch_slot(), current_slot);
  // ...but the UE's last known PDSCH slot must cover the whole repetition bundle, so that a different HARQ process
  // is not scheduled a PDSCH into the still-active repetition window.
  ASSERT_EQ(harq_ent.last_pdsch_slot(), current_slot + (nof_reps - 1));

  // Once the HARQ is ACKed, the reservation of the whole bundle must be released.
  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::ack, std::nullopt));
  ASSERT_FALSE(harq_ent.last_pdsch_slot().valid());
}

TEST_F(single_ue_harq_entity_test, when_retx_uses_pdsch_repetitions_then_last_pdsch_slot_follows_the_retx_bundle)
{
  constexpr uint8_t nof_reps = 4;

  auto h_dl =
      harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_dl.has_value());
  dl_msg_alloc          ue_pdsch = make_dummy_ue_pdsch_info();
  dl_harq_alloc_context harq_ctxt{dci_dl_rnti_config_type::c_rnti_f1_1};
  harq_ctxt.nof_repetitions = nof_reps;
  h_dl->save_grant_params(harq_ctxt, ue_pdsch);

  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));
  run_slot();
  ASSERT_TRUE(h_dl->new_retx(current_slot, k1, 0, nof_reps));
  h_dl->save_grant_params(harq_ctxt, ue_pdsch);

  // The retx's own bundle (starting at the new pdsch_slot()) must be reserved, not the original transmission's.
  ASSERT_EQ(h_dl->pdsch_slot(), current_slot);
  ASSERT_EQ(harq_ent.last_pdsch_slot(), current_slot + (nof_reps - 1));

  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::ack, std::nullopt));
  ASSERT_FALSE(harq_ent.last_pdsch_slot().valid());
}

TEST_F(single_ue_harq_entity_test,
       when_newtx_with_repetitions_is_reset_before_save_grant_params_then_reservation_is_released_immediately)
{
  constexpr uint8_t nof_reps = 4;

  // The repetition count is known and reserved from the moment the HARQ is allocated (not only once
  // save_grant_params runs).
  auto h_dl =
      harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_EQ(harq_ent.last_pdsch_slot(), current_slot + (nof_reps - 1));

  // The grant is aborted (e.g. RB allocation failure) before save_grant_params ever ran. Unlike
  // prev_tx_params.nof_repetitions (only correct once save_grant_params has run), last_occasion_slot is set together
  // with slot_tx regardless, so dealloc_harq can tell precisely that this HARQ held the whole reservation and
  // releases it immediately, rather than leaving the UE needlessly unschedulable for the rest of the nominal window.
  // NOTE: cancel_unsent_tx() immediately frees the HARQ-id itself for reuse (cell_harq_manager does not gate
  // alloc_dl_harq on last_pdsch_slot() at all); it is ue_cell::is_pdsch_enabled, one layer up and not exercised by this
  // test, that reads last_pdsch_slot() and would reject a newTx/reTx candidate for as long as a reservation is in
  // place.
  h_dl->cancel_unsent_tx();
  ASSERT_FALSE(harq_ent.last_pdsch_slot().valid());

  // The UE is immediately schedulable again, even for a slot still within the aborted bundle's nominal window.
  auto h_dl2 = harq_ent.alloc_dl_harq(current_slot, current_slot + 1, k1, max_retxs, 0);
  ASSERT_TRUE(h_dl2.has_value());
  ASSERT_EQ(harq_ent.last_pdsch_slot(), current_slot + 1);
}

// Rel-16/17 PUSCH repetition tests for CRC matching: a bundle draws one CRC report per occasion, in different
// slots, and the PHY accumulates soft bits across them, so any report may be the one that decodes the TB.
// find_ul_harq_waiting_ack must match every slot of the bundle and nothing outside it.

TEST_F(single_ue_harq_entity_test, when_newtx_has_no_pusch_repetitions_then_waiting_ack_matches_only_the_pusch_slot)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot, max_retxs);
  ASSERT_TRUE(h_ul.has_value());

  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(current_slot), h_ul);
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(current_slot + 1), std::nullopt);
}

TEST_F(single_ue_harq_entity_test, when_newtx_uses_pusch_repetitions_then_waiting_ack_matches_every_occasion_slot)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  constexpr uint8_t nof_reps = 4;

  auto h_ul = harq_ent.alloc_ul_harq(
      current_slot, current_slot, max_retxs, std::nullopt, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_ul.has_value());

  // Base occasion included: the PHY can already report a successful decode there.
  for (unsigned offset = 0; offset != nof_reps; ++offset) {
    ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(current_slot + offset), h_ul) << "at occasion offset " << offset;
  }
  // Just past the bundle there is nothing left to match.
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(current_slot + nof_reps), std::nullopt);
}

TEST_F(single_ue_harq_entity_test, when_one_occasion_of_a_pusch_bundle_decodes_then_the_combined_crc_stays_ok)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  constexpr uint8_t nof_reps = 4;

  auto h_ul = harq_ent.alloc_ul_harq(
      current_slot, current_slot, max_retxs, std::nullopt, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_ul.has_value());

  // The PHY reports per occasion while it accumulates soft bits, so the TB can decode part-way through the bundle.
  ASSERT_FALSE(h_ul->accumulate_crc(false));
  ASSERT_FALSE(h_ul->accumulate_crc(false));
  ASSERT_TRUE(h_ul->accumulate_crc(true));
  // A later occasion reporting a failure must not undo a decode that already happened.
  ASSERT_TRUE(h_ul->accumulate_crc(false));
}

TEST_F(single_ue_harq_entity_test, when_no_occasion_of_a_pusch_bundle_decodes_then_the_combined_crc_stays_ko)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  constexpr uint8_t nof_reps = 4;

  auto h_ul = harq_ent.alloc_ul_harq(
      current_slot, current_slot, max_retxs, std::nullopt, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_ul.has_value());

  for (unsigned i = 0; i != nof_reps; ++i) {
    ASSERT_FALSE(h_ul->accumulate_crc(false)) << "at occasion " << i;
  }
}

TEST_F(single_ue_harq_entity_test, when_a_harq_process_is_reused_then_the_combined_crc_is_cleared)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  constexpr uint8_t nof_reps = 4;

  // Take every UL HARQ, so that the process freed below is the only one left for the last allocation. A stale
  // combined CRC leaking into a reused process would turn a failed transmission into a false ACK.
  std::vector<ul_harq_process_handle> busy;
  for (unsigned i = 0, e = harq_ent.nof_ul_harqs(); i != e; ++i) {
    auto h = harq_ent.alloc_ul_harq(
        current_slot, current_slot, max_retxs, std::nullopt, harq_utils::harq_mode_t::normal, nof_reps);
    ASSERT_TRUE(h.has_value());
    busy.push_back(h.value());
  }
  const harq_id_t reused_id = busy.front().id();

  // An occasion of that bundle decoded the TB, concluding the transmission and freeing the process.
  ASSERT_TRUE(busy.front().accumulate_crc(true));
  ASSERT_TRUE(busy.front().ul_crc_info(true).has_value());

  auto h_new = harq_ent.alloc_ul_harq(
      current_slot, current_slot, max_retxs, std::nullopt, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_new.has_value());
  ASSERT_EQ(h_new->id(), reused_id);
  ASSERT_FALSE(h_new->accumulate_crc(false)) << "the reused HARQ process inherited the previous transmission's CRC";
}

TEST_F(single_ue_harq_entity_test, when_a_harq_process_is_retransmitted_then_the_combined_crc_is_cleared)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  constexpr uint8_t nof_reps = 4;

  auto h_ul = harq_ent.alloc_ul_harq(
      current_slot, current_slot, max_retxs, std::nullopt, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_ul.has_value());

  // A CRC accumulated by the previous transmission must not decide the outcome of the retx bundle.
  ASSERT_TRUE(h_ul->accumulate_crc(true));
  ASSERT_TRUE(h_ul->ul_crc_info(false).has_value());

  run_slot();
  ASSERT_TRUE(h_ul->new_retx(current_slot, nof_reps));
  ASSERT_FALSE(h_ul->accumulate_crc(false)) << "the retx inherited the previous transmission's CRC";
}

TEST_F(single_ue_harq_entity_test, when_retx_uses_pusch_repetitions_then_waiting_ack_follows_the_retx_bundle)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  constexpr uint8_t nof_reps = 4;

  auto h_ul = harq_ent.alloc_ul_harq(
      current_slot, current_slot, max_retxs, std::nullopt, harq_utils::harq_mode_t::normal, nof_reps);
  ASSERT_TRUE(h_ul.has_value());
  ASSERT_TRUE(h_ul->ul_crc_info(false).has_value());

  run_slot();
  ASSERT_TRUE(h_ul->new_retx(current_slot, nof_reps));

  // The window moves with the retx: the original base slot is now outside it, the retx bundle's occasions inside.
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(current_slot - 1), std::nullopt);
  for (unsigned offset = 0; offset != nof_reps; ++offset) {
    ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(current_slot + offset), h_ul) << "at occasion offset " << offset;
  }
}

TEST_F(single_ue_harq_entity_test, when_harq_is_allocated_then_harq_entity_finds_harq_in_waiting_ack_state)
{
  auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot, max_retxs);
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_TRUE(h_ul.has_value());
  ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(), h_dl);
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(), h_ul);
  ASSERT_EQ(harq_ent.find_pending_dl_retx(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_ul_retx(), std::nullopt);
}

TEST_F(single_ue_harq_entity_test, when_harq_is_nacked_then_harq_entity_finds_harq_with_pending_retx)
{
  auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot, max_retxs);
  ASSERT_TRUE(h_dl.value().dl_ack_info(mac_harq_ack_report_status::nack, 5));
  ASSERT_EQ(h_ul.value().ul_crc_info(false), units::bytes{0});
  ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_dl_retx(), h_dl);
  ASSERT_EQ(harq_ent.find_pending_ul_retx(), h_ul);
}

TEST_F(single_ue_harq_entity_test, when_all_harqs_are_allocated_harq_entity_cannot_find_empty_harq)
{
  ASSERT_EQ(harq_ent.nof_dl_harqs(), nof_harqs);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), nof_harqs);

  for (unsigned i = 0; i != nof_harqs; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_dl.value().is_waiting_ack());
    ASSERT_TRUE(h_ul.has_value());
    ASSERT_TRUE(h_ul.value().is_waiting_ack());
  }

  ASSERT_NE(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
  ASSERT_NE(harq_ent.find_ul_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_dl_retx(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_ul_retx(), std::nullopt);
  ASSERT_FALSE(harq_ent.has_empty_dl_harqs());
  ASSERT_FALSE(harq_ent.has_empty_ul_harqs());

  auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot, max_retxs);
  ASSERT_FALSE(h_dl.has_value());
  ASSERT_FALSE(h_ul.has_value());
}

TEST_F(single_ue_harq_entity_test, when_ue_harq_entity_is_deallocated_then_harq_resources_are_available_again)
{
  for (unsigned i = 0; i != nof_harqs; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_ul.has_value());
  }

  harq_ent.reset();
  harq_ent = cell_harqs.add_ue(ue_index, rnti, nof_harqs, nof_harqs);
  for (unsigned i = 0; i != nof_harqs; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_ul.has_value());
  }
}

TEST_F(single_ue_harq_entity_test, when_harq_entity_is_recycled_then_configured_grant_reservation_is_not_inherited)
{
  // Reserve HARQs of the UE for Configured Grant use.
  constexpr unsigned             nof_cg_reserved_harqs = 4;
  harq_dl_feedback_disabled_mask dl_feedback_disabled;
  harq_ul_mode_mask              ul_harq_mode_mask;
  harq_ent.reconfigure(nof_harqs, nof_harqs, dl_feedback_disabled, ul_harq_mode_mask, nof_cg_reserved_harqs);

  // The next UE takes the HARQ entity of the removed one, and must not inherit its reservation.
  harq_ent.reset();
  harq_ent = cell_harqs.add_ue(ue_index, rnti, nof_harqs, nof_harqs);

  for (unsigned i = 0; i != nof_harqs; ++i) {
    ASSERT_TRUE(harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).has_value())
        << "UL HARQ " << i << " is not available to the new UE";
  }
}

TEST_F(single_ue_harq_entity_test, when_max_retxs_reached_then_harq_entity_does_not_find_pending_retx)
{
  auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_TRUE(h_ul.has_value());
  for (unsigned i = 0; i != max_retxs; ++i) {
    ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(current_slot + k1, 0), h_dl);
    ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(current_slot + k2), h_ul);
    ASSERT_TRUE(h_dl.value().dl_ack_info(mac_harq_ack_report_status::nack, 5));
    ASSERT_EQ(h_ul.value().ul_crc_info(false), units::bytes{0});
    ASSERT_EQ(harq_ent.find_pending_dl_retx(), h_dl);
    ASSERT_EQ(harq_ent.find_pending_ul_retx(), h_ul);
    ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
    ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(), std::nullopt);

    run_slot();
    ASSERT_TRUE(h_dl.value().new_retx(current_slot, k1, 0));
    ASSERT_TRUE(h_ul.value().new_retx(current_slot + k2));
    ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(), h_dl);
    ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(), h_ul);
    ASSERT_EQ(harq_ent.find_pending_dl_retx(), std::nullopt);
    ASSERT_EQ(harq_ent.find_pending_ul_retx(), std::nullopt);
  }
  ASSERT_TRUE(h_dl.value().dl_ack_info(mac_harq_ack_report_status::nack, 5));
  ASSERT_EQ(h_ul.value().ul_crc_info(false), units::bytes{0});
  ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_dl_retx(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_ul_retx(), std::nullopt);
}

TEST_F(single_ue_harq_entity_test, after_max_ack_wait_timeout_dl_harqs_are_available_for_newtx)
{
  k2 = 4;
  for (unsigned i = 0; i != nof_harqs; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_ul.has_value());
  }
  for (unsigned i = 0; i != this->max_ack_wait_timeout + k1; ++i) {
    ASSERT_EQ(harq_ent.find_pending_dl_retx(), std::nullopt);
    ASSERT_EQ(harq_ent.find_pending_ul_retx(), std::nullopt);
    ASSERT_NE(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
    ASSERT_NE(harq_ent.find_ul_harq_waiting_ack(), std::nullopt);
    ASSERT_FALSE(harq_ent.has_empty_dl_harqs());
    ASSERT_FALSE(harq_ent.has_empty_ul_harqs());
    run_slot();
  }
  run_slot();
  ASSERT_EQ(harq_ent.find_pending_dl_retx(), std::nullopt);
  ASSERT_EQ(harq_ent.find_pending_ul_retx(), std::nullopt);
  ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(harq_ent.find_ul_harq_waiting_ack(), std::nullopt);
  ASSERT_TRUE(harq_ent.has_empty_dl_harqs());
  ASSERT_TRUE(harq_ent.has_empty_ul_harqs());
  for (unsigned i = 0; i != nof_harqs; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_ul.has_value());
  }
  ASSERT_EQ(timeout_handler.last_event, dummy_harq_timeout_handler::last_event_t::feedback_timeout);
}

TEST_F(single_ue_harq_entity_test, dl_harq_feedback_timeout_is_counted_from_the_last_pucch_repetition)
{
  // As per TS 38.213, Section 9.2.6, the HARQ-ACK of a PUCCH with repetitions is transmitted over several slots, the
  // last of which is the earliest point at which the feedback can be considered lost.
  const unsigned nof_rep_slots  = 4;
  const unsigned last_ack_delay = k1 + nof_rep_slots - 1;
  auto           h_dl           = harq_ent.alloc_dl_harq(
      current_slot, current_slot, k1, max_retxs, 0, harq_utils::harq_mode_t::normal, 1, last_ack_delay);
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_EQ(h_dl->uci_slot(), current_slot + k1);
  ASSERT_EQ(h_dl->last_uci_slot(), current_slot + last_ack_delay);

  // Test: The HARQ process is still waiting for the feedback after the timeout of a single-slot HARQ-ACK report.
  for (unsigned i = 0; i != max_ack_wait_timeout + k1 + 1; ++i) {
    run_slot();
  }
  ASSERT_NE(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(timeout_handler.last_event, dummy_harq_timeout_handler::last_event_t::none);

  // Test: The timeout is reached once the wait has elapsed since the last repetition.
  for (unsigned i = 0; i != nof_rep_slots - 1; ++i) {
    run_slot();
  }
  ASSERT_EQ(harq_ent.find_dl_harq_waiting_ack(), std::nullopt);
  ASSERT_EQ(timeout_handler.last_event, dummy_harq_timeout_handler::last_event_t::feedback_timeout);
  ASSERT_TRUE(timeout_handler.last_dir_is_dl);
  ASSERT_FALSE(timeout_handler.last_was_ack);
}

TEST_F(single_ue_harq_entity_test, when_pending_retx_harq_is_never_rescheduled_then_on_retx_timeout_is_notified)
{
  auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
  ASSERT_TRUE(h_ul.has_value());

  // NACK the HARQ so it enters pending_retx state, but never actually schedule a retransmission for it.
  ASSERT_EQ(h_ul.value().ul_crc_info(false), units::bytes{0});
  ASSERT_TRUE(h_ul.value().has_pending_retx());

  // Let time pass without ever granting a retx; the HARQ must eventually be force-discarded.
  unsigned i = 0;
  for (; i != this->max_harq_retx_timeout and harq_ent.find_pending_ul_retx().has_value(); ++i) {
    ASSERT_EQ(timeout_handler.last_event, dummy_harq_timeout_handler::last_event_t::none);
    run_slot();
  }
  ASSERT_LT(i, this->max_harq_retx_timeout) << "the pending-retx HARQ was never timed out";
  ASSERT_EQ(harq_ent.find_pending_ul_retx(), std::nullopt);

  // The retx-starved HARQ must be reported via on_retx_timeout (and not on_feedback_timeout), and the UE/direction
  // must be identifiable so that any parallel bookkeeping keyed on them (e.g. ra_scheduler's pending_msg3s) can be
  // released too.
  ASSERT_EQ(timeout_handler.last_event, dummy_harq_timeout_handler::last_event_t::retx_timeout);
  ASSERT_EQ(timeout_handler.last_ue_index, ue_index);
  ASSERT_FALSE(timeout_handler.last_dir_is_dl);

  // The HARQ resource must be available again for a new Tx.
  auto h_ul2 = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
  ASSERT_TRUE(h_ul2.has_value());
}

TEST_F(single_ue_harq_entity_harq_5bit_tester, when_5_harq_bits_are_acks_then_all_5_active_harqs_are_updated)
{
  constexpr unsigned active_harqs = 5;

  std::vector<harq_id_t> h_dls;
  for (unsigned i = 0; i != active_harqs; ++i) {
    h_dls.push_back(harq_ent.alloc_dl_harq(current_slot, current_slot, k1, this->max_retxs, i)->id());
  }
  slot_point pucch_slot = current_slot + k1;

  while (current_slot != pucch_slot) {
    run_slot();
  }

  // ACK received.
  for (unsigned i = 0; i != active_harqs; ++i) {
    auto h_dl = this->harq_ent.find_dl_harq_waiting_ack(pucch_slot, i);
    ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::ack, std::nullopt));
  }

  for (unsigned i = 0; i != h_dls.size(); ++i) {
    auto h_dl = this->harq_ent.dl_harq(h_dls[i]);
    ASSERT_FALSE(h_dl.has_value());
  }
}

TEST_F(single_ue_harq_entity_harq_5bit_tester, when_5_harq_bits_are_nacks_then_all_5_active_harqs_are_updated)
{
  constexpr unsigned active_harqs = 5;

  std::vector<harq_id_t> h_dls(active_harqs);
  for (unsigned i = 0; i != active_harqs; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, this->max_retxs, i);
    h_dls.push_back(h_dl->id());
  }
  slot_point pucch_slot = current_slot + k1;

  while (current_slot != pucch_slot) {
    run_slot();
  }

  // NACK received.
  for (unsigned i = 0; i != active_harqs; ++i) {
    auto h_dl_ack = this->harq_ent.find_dl_harq_waiting_ack(pucch_slot, i);
    ASSERT_TRUE(h_dl_ack->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));
  }

  for (unsigned i = 0; i != h_dls.size(); ++i) {
    ASSERT_TRUE(this->harq_ent.dl_harq(h_dls[i]).value().has_pending_retx());
  }
}

// Tests for HARQ manager with multiple UEs

TEST_F(multi_ue_harq_manager_test, when_ue_harq_entity_is_created_or_reset_then_cell_harq_manager_is_updated)
{
  for (unsigned i = 0; i != this->max_ues; ++i) {
    ASSERT_FALSE(cell_harqs.contains(to_du_ue_index(i)));
  }
  std::vector<unique_ue_harq_entity> harq_ents;
  for (unsigned i = 0; i != this->max_ues; ++i) {
    harq_ents.push_back(cell_harqs.add_ue(to_du_ue_index(i), to_rnti(0x4601 + i), nof_harqs, nof_harqs));
    ASSERT_TRUE(cell_harqs.contains(to_du_ue_index(i)));
  }
  for (unsigned i = 0; i != this->max_ues; ++i) {
    harq_ents[i].reset();
    ASSERT_FALSE(cell_harqs.contains(to_du_ue_index(i)));
  }
}

TEST_F(multi_ue_harq_manager_test, when_harq_entities_are_destroyed_then_pending_timeouts_are_not_triggered)
{
  unsigned              k1 = 4, max_retxs = 4;
  unique_ue_harq_entity harq_ent1 = cell_harqs.add_ue(to_du_ue_index(1), to_rnti(0x4601), nof_harqs, nof_harqs);
  unique_ue_harq_entity harq_ent2 = cell_harqs.add_ue(to_du_ue_index(2), to_rnti(0x4602), nof_harqs, nof_harqs);

  ASSERT_TRUE(harq_ent1.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).has_value());
  run_slot();
  ASSERT_TRUE(harq_ent2.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).has_value());

  // Delete UE1 before timeout.
  for (unsigned i = 0; i != this->max_ack_wait_timeout + k1 - 2; ++i) {
    run_slot();
    ASSERT_EQ(this->timeout_handler.last_ue_index, INVALID_DU_UE_INDEX);
  }

  harq_ent1.reset();
  run_slot(); // UE1 timeout should not trigger.
  ASSERT_EQ(this->timeout_handler.last_ue_index, INVALID_DU_UE_INDEX);

  run_slot();
  ASSERT_EQ(this->timeout_handler.last_ue_index, to_du_ue_index(2));
}

TEST_F(multi_ue_harq_manager_test, when_harq_entities_are_nacked_then_they_appear_in_list_of_harqs_with_pending_retxs)
{
  constexpr unsigned    k1 = 4, k2 = 6, max_retxs = 4;
  unique_ue_harq_entity harq_ent1 = cell_harqs.add_ue(to_du_ue_index(1), to_rnti(0x4601), nof_harqs, nof_harqs);
  unique_ue_harq_entity harq_ent2 = cell_harqs.add_ue(to_du_ue_index(2), to_rnti(0x4602), nof_harqs, nof_harqs);

  ASSERT_TRUE(cell_harqs.pending_dl_retxs().empty());
  ASSERT_TRUE(cell_harqs.pending_ul_retxs().empty());

  auto h_dl1 = harq_ent1.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  auto h_dl2 = harq_ent2.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  auto h_ul1 = harq_ent1.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
  auto h_ul2 = harq_ent2.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
  ASSERT_TRUE(h_dl1.has_value() and h_dl2.has_value() and h_ul1.has_value() and h_ul2.has_value());

  ASSERT_TRUE(cell_harqs.pending_dl_retxs().empty());
  ASSERT_TRUE(cell_harqs.pending_ul_retxs().empty());
  ASSERT_EQ(cell_harqs.pending_dl_retxs().begin(), cell_harqs.pending_dl_retxs().end());

  ASSERT_TRUE(harq_ent1.find_dl_harq_waiting_ack(current_slot + k1, 0)
                  ->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));
  ASSERT_EQ(harq_ent1.find_ul_harq_waiting_ack(current_slot + k2)->ul_crc_info(false), units::bytes{0});
  ASSERT_TRUE(harq_ent2.find_dl_harq_waiting_ack(current_slot + k1, 0)
                  ->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));
  ASSERT_EQ(harq_ent2.find_ul_harq_waiting_ack(current_slot + k2)->ul_crc_info(false), units::bytes{0});

  // HARQs are in the list of pending retxs.
  ASSERT_FALSE(cell_harqs.pending_dl_retxs().empty());
  ASSERT_FALSE(cell_harqs.pending_ul_retxs().empty());
  ASSERT_NE(cell_harqs.pending_dl_retxs().begin(), cell_harqs.pending_dl_retxs().end());
  unsigned count = 0;
  for (dl_harq_process_handle h : cell_harqs.pending_dl_retxs()) {
    ASSERT_TRUE(h.has_pending_retx());
    count++;
  }
  ASSERT_EQ(count, 2);
  count = 0;
  for (ul_harq_process_handle h : cell_harqs.pending_ul_retxs()) {
    ASSERT_TRUE(h.has_pending_retx());
    count++;
  }

  // If we cancel the HARQ, it should not show up in the list of pending reTx.
  h_dl2->cancel_retxs();
  h_ul1->cancel_retxs();
  count = 0;
  for (dl_harq_process_handle h : cell_harqs.pending_dl_retxs()) {
    ASSERT_EQ(h.rnti(), to_rnti(0x4601));
    count++;
  }
  ASSERT_EQ(count, 1);
  count = 0;
  for (ul_harq_process_handle h : cell_harqs.pending_ul_retxs()) {
    ASSERT_EQ(h.rnti(), to_rnti(0x4602));
    count++;
  }
  ASSERT_EQ(count, 1);
}

TEST_F(multi_ue_harq_manager_test, pending_harq_retxs_are_ordered_from_oldest_to_newest_ack)
{
  constexpr unsigned    k1 = 4, k2 = 6, max_retxs = 4;
  unique_ue_harq_entity harq_ent1 = cell_harqs.add_ue(to_du_ue_index(1), to_rnti(0x4601), nof_harqs, nof_harqs);
  unique_ue_harq_entity harq_ent2 = cell_harqs.add_ue(to_du_ue_index(2), to_rnti(0x4602), nof_harqs, nof_harqs);

  ASSERT_TRUE(harq_ent1.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).has_value());
  ASSERT_TRUE(harq_ent2.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).has_value());
  ASSERT_TRUE(harq_ent2.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).has_value());
  ASSERT_TRUE(harq_ent1.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).has_value());

  ASSERT_TRUE(harq_ent1.find_dl_harq_waiting_ack(current_slot + k1, 0)
                  ->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));
  ASSERT_TRUE(harq_ent2.find_dl_harq_waiting_ack(current_slot + k1, 0)
                  ->dl_ack_info(mac_harq_ack_report_status::nack, std::nullopt));
  ASSERT_EQ(harq_ent2.find_ul_harq_waiting_ack(current_slot + k2)->ul_crc_info(false), units::bytes{0});
  ASSERT_EQ(harq_ent1.find_ul_harq_waiting_ack(current_slot + k2)->ul_crc_info(false), units::bytes{0});

  unsigned count = 0;
  for (dl_harq_process_handle h : cell_harqs.pending_dl_retxs()) {
    ASSERT_EQ(h.rnti(), to_rnti(0x4601 + count));
    count++;
  }
  ASSERT_EQ(count, 2);
  count = 0;
  for (ul_harq_process_handle h : cell_harqs.pending_ul_retxs()) {
    ASSERT_EQ(h.rnti(), to_rnti(0x4602 - count));
    count++;
  }
  ASSERT_EQ(count, 2);
}

TEST_F(multi_ue_harq_manager_test, when_new_tx_occur_for_different_ues_then_ndi_is_still_valid)
{
  constexpr unsigned    k1 = 4, k2 = 6, max_retxs = 4;
  unique_ue_harq_entity harq_ent1 = cell_harqs.add_ue(to_du_ue_index(1), to_rnti(0x4601), nof_harqs, nof_harqs);
  unique_ue_harq_entity harq_ent2 = cell_harqs.add_ue(to_du_ue_index(2), to_rnti(0x4602), nof_harqs, nof_harqs);

  auto h_dl = harq_ent1.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  auto h_ul = harq_ent1.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_TRUE(h_ul.has_value());

  bool ndi_dl1 = h_dl->ndi();
  bool ndi_ul1 = h_ul->ndi();

  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::ack, std::nullopt));
  ASSERT_EQ(h_ul->ul_crc_info(true), units::bytes{0});

  h_dl = harq_ent2.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  h_ul = harq_ent2.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
  ASSERT_TRUE(h_dl.has_value());
  ASSERT_TRUE(h_ul.has_value());

  ASSERT_EQ(h_dl->ndi(), ndi_dl1);
  ASSERT_EQ(h_ul->ndi(), ndi_ul1);

  ASSERT_TRUE(h_dl->dl_ack_info(mac_harq_ack_report_status::ack, std::nullopt));
  ASSERT_EQ(h_ul->ul_crc_info(true), units::bytes{0});

  h_dl = harq_ent1.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
  h_ul = harq_ent1.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);

  ASSERT_NE(h_dl->ndi(), ndi_dl1);
  ASSERT_NE(h_ul->ndi(), ndi_ul1);
}

// HARQ extension tests for NTN 32 HARQ support.

class harq_extension_test : public base_harq_manager_test, public ::testing::Test
{
protected:
  harq_extension_test() : base_harq_manager_test(1, 20) {}

  const du_ue_index_t ue_index   = to_du_ue_index(0);
  const rnti_t        rnti       = to_rnti(0x4601);
  const unsigned      init_harqs = 16;
  const unsigned      k1         = 4;
  const unsigned      k2         = 6;
  const unsigned      max_retxs  = 4;
};

TEST_F(harq_extension_test, when_reconfigure_with_more_harqs_then_nof_harqs_increases)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-1");

  unique_ue_harq_entity harq_ent = cell_harqs.add_ue(ue_index, rnti, init_harqs, init_harqs);

  ASSERT_EQ(harq_ent.nof_dl_harqs(), init_harqs);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), init_harqs);

  // Extend to 32 HARQs via reconfigure.
  constexpr unsigned extended_harqs       = 32;
  constexpr unsigned nof_cg_reserved_harq = 0;
  harq_ent.reconfigure(extended_harqs, extended_harqs, {}, {}, nof_cg_reserved_harq);

  ASSERT_EQ(harq_ent.nof_dl_harqs(), extended_harqs);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), extended_harqs);
}

TEST_F(harq_extension_test, when_reconfigure_with_same_count_then_no_change)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-1");

  unique_ue_harq_entity harq_ent = cell_harqs.add_ue(ue_index, rnti, init_harqs, init_harqs);

  ASSERT_EQ(harq_ent.nof_dl_harqs(), init_harqs);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), init_harqs);

  constexpr unsigned nof_cg_reserved_harq = 0;
  // Reconfiguring with same count should not change anything.
  harq_ent.reconfigure(init_harqs, init_harqs, {}, {}, nof_cg_reserved_harq);

  ASSERT_EQ(harq_ent.nof_dl_harqs(), init_harqs);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), init_harqs);
}

TEST_F(harq_extension_test, when_reconfigure_extends_then_new_harqs_are_allocatable)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-1");

  unique_ue_harq_entity harq_ent = cell_harqs.add_ue(ue_index, rnti, init_harqs, init_harqs);

  // Allocate all initial HARQs.
  for (unsigned i = 0; i != init_harqs; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_ul.has_value());
  }

  // No more HARQs available.
  ASSERT_FALSE(harq_ent.has_empty_dl_harqs());
  ASSERT_FALSE(harq_ent.has_empty_ul_harqs());
  ASSERT_FALSE(harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).has_value());
  ASSERT_FALSE(harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).has_value());

  // Extend to 32 HARQs via reconfigure.
  constexpr unsigned extended_harqs       = 32;
  constexpr unsigned nof_cg_reserved_harq = 0;
  harq_ent.reconfigure(extended_harqs, extended_harqs, {}, {}, nof_cg_reserved_harq);

  // Now we should have more HARQs available.
  ASSERT_TRUE(harq_ent.has_empty_dl_harqs());
  ASSERT_TRUE(harq_ent.has_empty_ul_harqs());

  // Allocate the new HARQs (IDs 16 to 31).
  for (unsigned i = init_harqs; i != extended_harqs; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_ul.has_value());
    // New HARQs should have IDs >= init_harqs.
    ASSERT_GE(h_dl->id(), to_harq_id(init_harqs));
    ASSERT_GE(h_ul->id(), to_harq_id(init_harqs));
  }

  // Now no HARQs should be available.
  ASSERT_FALSE(harq_ent.has_empty_dl_harqs());
  ASSERT_FALSE(harq_ent.has_empty_ul_harqs());
}

TEST_F(harq_extension_test, when_reconfigure_extends_asymmetrically_then_dl_and_ul_differ)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-1");

  unique_ue_harq_entity harq_ent = cell_harqs.add_ue(ue_index, rnti, init_harqs, init_harqs);

  // Extend DL to 32, UL to 24 via reconfigure.
  constexpr unsigned nof_cg_reserved_harq = 0;
  harq_ent.reconfigure(32, 24, {}, {}, nof_cg_reserved_harq);

  ASSERT_EQ(harq_ent.nof_dl_harqs(), 32U);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), 24U);
}

// NTN tests.

TEST_F(single_ntn_ue_harq_normal_mode_process_test, when_ntn_normal_mode_wait_rtt_for_ack)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-TIM-1");

  float      pucch_snr   = 5;
  slot_point dl_ack_slot = current_slot + k1 + NTN_CELL_SPECIFIC_KOFFSET_MAX;
  slot_point ul_ack_slot = current_slot + k2 + NTN_CELL_SPECIFIC_KOFFSET_MAX;

  ASSERT_EQ(h_dl.pdsch_slot(), current_slot);
  ASSERT_EQ(h_dl.uci_slot(), dl_ack_slot);
  ASSERT_EQ(h_dl.max_nof_retxs(), max_retxs);
  ASSERT_EQ(h_dl.nof_retxs(), 0);
  ASSERT_EQ(h_dl.mode(), harq_utils::harq_mode_t::normal);
  ASSERT_EQ(h_ul.pusch_slot(), ul_ack_slot);
  ASSERT_EQ(h_ul.max_nof_retxs(), max_retxs);
  ASSERT_EQ(h_ul.nof_retxs(), 0);
  ASSERT_EQ(h_ul.mode(), harq_utils::harq_mode_t::normal);

  while (current_slot != std::max(dl_ack_slot, ul_ack_slot) + 1) {
    if (current_slot < dl_ack_slot) {
      ASSERT_TRUE(h_dl.is_waiting_ack());
      ASSERT_EQ(h_dl, harq_ent.dl_harq(to_harq_id(0)));
    } else if (current_slot == dl_ack_slot) {
      ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::ack, pucch_snr));
      ASSERT_FALSE(h_dl.is_waiting_ack());
      ASSERT_FALSE(h_dl.has_pending_retx());
    } else {
      ASSERT_TRUE(h_dl.empty());
      ASSERT_FALSE(harq_ent.dl_harq(to_harq_id(0)).has_value());
    }
    if (current_slot < ul_ack_slot) {
      ASSERT_TRUE(h_ul.is_waiting_ack());
      ASSERT_EQ(h_ul, harq_ent.ul_harq(to_harq_id(0)));
    } else if (current_slot == ul_ack_slot) {
      ASSERT_EQ(h_ul.ul_crc_info(true), pusch_info.tb_size_bytes);
      ASSERT_FALSE(h_ul.is_waiting_ack());
      ASSERT_FALSE(h_ul.has_pending_retx());
    } else {
      ASSERT_TRUE(h_ul.empty());
      ASSERT_FALSE(harq_ent.ul_harq(to_harq_id(0)).has_value());
    }
    run_slot();
  }
  ASSERT_EQ(harq_ent.total_ul_bytes_waiting_ack(), units::bytes{0});
}
TEST_F(single_ntn_ue_ul_harq_mode_b_process_test, when_ul_harq_allocated_then_it_flushes_soon_after)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-2");

  ASSERT_EQ(h_dl.mode(), harq_utils::harq_mode_t::normal);
  ASSERT_EQ(h_ul.mode(), harq_utils::harq_mode_t::feedback_disabled_or_mode_b);

  slot_point slot_ul_timeout = current_slot + ntn_ack_wait_timeout;

  while (current_slot != slot_ul_timeout + 1) {
    if (current_slot < slot_ul_timeout) {
      ASSERT_TRUE(h_ul.is_waiting_ack());
      ASSERT_EQ(h_ul, harq_ent.ul_harq(to_harq_id(nof_normal_mode_harqs)));
    } else {
      ASSERT_TRUE(h_ul.empty());
      ASSERT_FALSE(harq_ent.ul_harq(to_harq_id(nof_normal_mode_harqs)).has_value());
    }
    run_slot();
  }
}

/// A grant that asks for normal mode is given a process operating in that mode.
TEST_F(single_ntn_ue_ul_harq_mode_b_process_test, when_normal_mode_is_requested_then_a_normal_mode_process_is_allocated)
{
  auto h = harq_ent.alloc_ul_harq(
      current_slot, current_slot + k2 + ntn_cs_koffset, max_retxs, std::nullopt, harq_utils::harq_mode_t::normal);
  ASSERT_TRUE(h.has_value());
  ASSERT_EQ(h->mode(), harq_utils::harq_mode_t::normal);
}

/// The allocation fails instead of falling back to a process operating in the other mode.
TEST_F(single_ntn_ue_ul_harq_mode_b_process_test, when_every_process_of_the_requested_mode_is_busy_then_alloc_fails)
{
  for (unsigned i = 0; i != nof_normal_mode_harqs; ++i) {
    ASSERT_TRUE(harq_ent
                    .alloc_ul_harq(current_slot,
                                   current_slot + k2 + ntn_cs_koffset,
                                   max_retxs,
                                   std::nullopt,
                                   harq_utils::harq_mode_t::normal)
                    .has_value());
  }
  ASSERT_TRUE(harq_ent.has_empty_ul_harqs()) << "the mode B processes are still free";
  ASSERT_FALSE(harq_ent
                   .alloc_ul_harq(current_slot,
                                  current_slot + k2 + ntn_cs_koffset,
                                  max_retxs,
                                  std::nullopt,
                                  harq_utils::harq_mode_t::normal)
                   .has_value());
}

TEST_F(single_ntn_ue_ul_harq_mode_b_process_test, ul_harq_history_is_reachable_after_harq_release)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-2");

  ASSERT_EQ(h_dl.mode(), harq_utils::harq_mode_t::normal);
  ASSERT_EQ(h_ul.mode(), harq_utils::harq_mode_t::feedback_disabled_or_mode_b);

  slot_point slot_ul_timeout = current_slot + ntn_ack_wait_timeout;
  slot_point pusch_slot      = current_slot + ntn_cs_koffset + k2;

  ASSERT_TRUE(h_dl.is_waiting_ack());
  ASSERT_EQ(h_dl, harq_ent.dl_harq(to_harq_id(0)));
  ASSERT_TRUE(h_ul.is_waiting_ack());
  ASSERT_EQ(h_ul, harq_ent.ul_harq(to_harq_id(nof_normal_mode_harqs)));

  // After timeout, HARQ not available, but NTN HARQ allocation history is available.
  while (current_slot != slot_ul_timeout) {
    run_slot();
  }
  ASSERT_TRUE(h_ul.empty());
  ASSERT_FALSE(harq_ent.ul_harq(to_harq_id(nof_normal_mode_harqs)).has_value());
  // Get UL HARQ handlers from the NTN HARQ history.
  h_ul = harq_ent.find_ul_harq_waiting_ack(pusch_slot).value();
  ASSERT_EQ(h_ul.get_grant_params().tbs, units::bytes{pusch_info.tb_size_bytes});
  ASSERT_EQ(h_ul.get_grant_params().tbs, harq_ent.total_ul_bytes_waiting_ack());

  while (current_slot != pusch_slot + 1) {
    if (current_slot < pusch_slot) {
      h_ul = harq_ent.find_ul_harq_waiting_ack(pusch_slot).value();
      ASSERT_FALSE(h_ul.empty());
      ASSERT_EQ(h_ul.get_grant_params().tbs, pusch_info.tb_size_bytes);
      ASSERT_EQ(h_ul.get_grant_params().tbs, harq_ent.total_ul_bytes_waiting_ack());
    } else if (current_slot == pusch_slot) {
      // At pusch_slot, UL HARQ history still waiting for ACK.
      h_ul = harq_ent.find_ul_harq_waiting_ack(pusch_slot).value();
      ASSERT_FALSE(h_ul.empty());
      ASSERT_TRUE(h_ul.is_waiting_ack());
      ASSERT_EQ(h_ul.get_grant_params().tbs, pusch_info.tb_size_bytes);
      ASSERT_EQ(h_ul.get_grant_params().tbs, harq_ent.total_ul_bytes_waiting_ack());
      ASSERT_TRUE(h_ul.ul_crc_info(true));
      ASSERT_TRUE(h_ul.empty());
    } else {
      // After pusch_slot and ACK, UL HARQ history is not available.
      ASSERT_FALSE(harq_ent.find_ul_harq_waiting_ack(pusch_slot).has_value());
    }
    run_slot();
  }
}

TEST_F(single_ntn_ue_ul_harq_mode_b_process_test, when_ul_harq_gets_acked_then_it_reports_the_correct_tbs)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-2");

  ASSERT_EQ(h_dl.mode(), harq_utils::harq_mode_t::normal);
  ASSERT_EQ(h_ul.mode(), harq_utils::harq_mode_t::feedback_disabled_or_mode_b);

  slot_point uci_slot   = current_slot + ntn_cs_koffset + k1;
  slot_point pusch_slot = current_slot + ntn_cs_koffset + k2;

  ASSERT_TRUE(h_dl.is_waiting_ack());
  ASSERT_EQ(h_dl, harq_ent.dl_harq(to_harq_id(0)));
  ASSERT_TRUE(h_ul.is_waiting_ack());
  ASSERT_EQ(h_ul, harq_ent.ul_harq(to_harq_id(nof_normal_mode_harqs)));

  while (current_slot != uci_slot) {
    run_slot();
  }
  h_dl = harq_ent.find_dl_harq_waiting_ack(uci_slot, 0).value();
  ASSERT_TRUE(h_dl.dl_ack_info(mac_harq_ack_report_status::ack, std::nullopt));
  ASSERT_EQ(h_dl.get_grant_params().tbs, ue_pdsch.pdsch_cfg.codewords[0].tb_size_bytes);
  while (current_slot != pusch_slot) {
    run_slot();
  }
  h_ul = harq_ent.find_ul_harq_waiting_ack(pusch_slot).value();
  ASSERT_EQ(harq_ent.total_ul_bytes_waiting_ack(), pusch_info.tb_size_bytes);
  ASSERT_EQ(h_ul.ul_crc_info(true), units::bytes{pusch_info.tb_size_bytes});
  ASSERT_EQ(harq_ent.total_ul_bytes_waiting_ack(), units::bytes{0});
}

TEST_F(single_ntn_ue_harq_dl_feedback_disabled_process_test, release_dl_harq_after_timeout)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-2");

  ASSERT_EQ(h_dl.mode(), harq_utils::harq_mode_t::feedback_disabled_or_mode_b);
  ASSERT_EQ(h_ul.mode(), harq_utils::harq_mode_t::normal);

  slot_point slot_dl_timeout = current_slot + ntn_ack_wait_timeout;

  ASSERT_TRUE(h_dl.is_waiting_ack());
  ASSERT_EQ(h_dl, harq_ent.dl_harq(to_harq_id(nof_normal_mode_harqs)));
  ASSERT_TRUE(h_ul.is_waiting_ack());
  ASSERT_EQ(h_ul, harq_ent.ul_harq(to_harq_id(0)));

  while (current_slot != slot_dl_timeout + 1) {
    if (current_slot < slot_dl_timeout) {
      ASSERT_TRUE(h_dl.is_waiting_ack());
      ASSERT_EQ(h_dl, harq_ent.dl_harq(to_harq_id(nof_normal_mode_harqs)));
    } else {
      ASSERT_TRUE(h_dl.empty());
      ASSERT_FALSE(harq_ent.dl_harq(to_harq_id(nof_normal_mode_harqs)).has_value());
      ASSERT_EQ(timeout_handler.last_tbs, ue_pdsch.pdsch_cfg.codewords[0].tb_size_bytes);
    }
    run_slot();
  }
}

TEST_F(single_ntn_ue_harq_dl_feedback_disabled_process_test, release_follows_the_transmission_not_the_koffset)
{
  // No HARQ-ACK is reported for this process, so it is released shortly after its PDSCH, whatever Koffset placed the
  // HARQ-ACK slot it never uses.
  h_dl.cancel_unsent_tx();
  h_dl = harq_ent
             .alloc_dl_harq(current_slot,
                            current_slot,
                            k1 + ntn_cs_koffset,
                            max_retxs,
                            0,
                            harq_utils::harq_mode_t::feedback_disabled_or_mode_b)
             .value();
  dl_harq_alloc_context harq_ctxt{dci_dl_rnti_config_type::c_rnti_f1_0};
  ue_pdsch.pdsch_cfg.harq_id = h_dl.id();
  h_dl.save_grant_params(harq_ctxt, ue_pdsch);

  // A single PDSCH occasion in the current slot, so neither k1 nor the Koffset delays the release.
  const slot_point slot_dl_timeout = current_slot + ntn_ack_wait_timeout;

  while (current_slot != slot_dl_timeout) {
    ASSERT_TRUE(h_dl.is_waiting_ack()) << "HARQ released before the delay its transmission implies";
    run_slot();
  }
  run_slot();
  ASSERT_TRUE(h_dl.empty()) << "HARQ outlived the delay its transmission implies";
}

// Tests for HARQ count based on NTN mode.

class non_ntn_harq_count_test : public base_harq_manager_test, public ::testing::Test
{
protected:
  non_ntn_harq_count_test() : base_harq_manager_test(1) {}

  const du_ue_index_t ue_index  = to_du_ue_index(0);
  const rnti_t        rnti      = to_rnti(0x4601);
  const unsigned      max_retxs = 4;
  const unsigned      k1        = 4;
  const unsigned      k2        = 6;
};

class ntn_harq_count_test : public base_harq_manager_test, public ::testing::Test
{
protected:
  ntn_harq_count_test() : base_harq_manager_test(1, 20) {}

  const du_ue_index_t ue_index  = to_du_ue_index(0);
  const rnti_t        rnti      = to_rnti(0x4601);
  const unsigned      max_retxs = 4;
  const unsigned      k1        = 4;
  const unsigned      k2        = 6;
};

TEST_F(non_ntn_harq_count_test, when_non_ntn_cell_then_max_harqs_is_16)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-1");

  // Verify the max_harqs_per_ue is 16 for non-NTN cells.
  ASSERT_EQ(max_harqs_per_ue, MAX_NOF_HARQS_NON_NTN);
  ASSERT_EQ(max_harqs_per_ue, 16U);

  // Create UE with 16 HARQs (the maximum for non-NTN).
  unique_ue_harq_entity harq_ent = cell_harqs.add_ue(ue_index, rnti, MAX_NOF_HARQS_NON_NTN, MAX_NOF_HARQS_NON_NTN);
  ASSERT_EQ(harq_ent.nof_dl_harqs(), MAX_NOF_HARQS_NON_NTN);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), MAX_NOF_HARQS_NON_NTN);

  // Allocate all 16 HARQs.
  for (unsigned i = 0; i != MAX_NOF_HARQS_NON_NTN; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_ul.has_value());
  }

  // No more HARQs available after allocating 16.
  ASSERT_FALSE(harq_ent.has_empty_dl_harqs());
  ASSERT_FALSE(harq_ent.has_empty_ul_harqs());
  ASSERT_FALSE(harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).has_value());
  ASSERT_FALSE(harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).has_value());
}

TEST_F(ntn_harq_count_test, when_ntn_cell_then_max_harqs_is_32)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-HARQ-1");

  // Verify the max_harqs_per_ue is 32 for NTN cells.
  ASSERT_EQ(max_harqs_per_ue, MAX_NOF_HARQS);
  ASSERT_EQ(max_harqs_per_ue, 32U);

  // Create UE with 32 HARQs (the maximum for NTN).
  unique_ue_harq_entity harq_ent = cell_harqs.add_ue(ue_index, rnti, MAX_NOF_HARQS, MAX_NOF_HARQS);
  ASSERT_EQ(harq_ent.nof_dl_harqs(), MAX_NOF_HARQS);
  ASSERT_EQ(harq_ent.nof_ul_harqs(), MAX_NOF_HARQS);

  // Allocate all 32 HARQs.
  for (unsigned i = 0; i != MAX_NOF_HARQS; ++i) {
    auto h_dl = harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0);
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs);
    ASSERT_TRUE(h_dl.has_value());
    ASSERT_TRUE(h_ul.has_value());
  }

  // No more HARQs available after allocating 32.
  ASSERT_FALSE(harq_ent.has_empty_dl_harqs());
  ASSERT_FALSE(harq_ent.has_empty_ul_harqs());
  ASSERT_FALSE(harq_ent.alloc_dl_harq(current_slot, current_slot, k1, max_retxs, 0).has_value());
  ASSERT_FALSE(harq_ent.alloc_ul_harq(current_slot, current_slot + k2, max_retxs).has_value());
}

// Tests for CG (Configured Grant) HARQ-ID reservation.
// After reconfigure(nof_harqs, nof_harqs, {}, {}, nof_cg_reserved):
//   - HARQ IDs [0, nof_cg_reserved)  are reserved for CG use (requested by harq_id argument).
//   - HARQ IDs [nof_cg_reserved, nof_harqs) are available for regular UL scheduling (harq_id = nullopt).

class cg_harq_reservation_test : public base_harq_manager_test, public ::testing::Test
{
protected:
  cg_harq_reservation_test() : base_harq_manager_test(1)
  {
    harq_ent.reconfigure(nof_harqs, nof_harqs, {}, {}, nof_cg_reserved);
    // NOTE: in CG, there is no concept of "k2"; in the following, k2 is used to reuse the base class test code.
    pusch_slot = current_slot + k2;
  }

  const du_ue_index_t ue_index  = to_du_ue_index(0);
  const rnti_t        rnti      = to_rnti(0x4601);
  const unsigned      nof_harqs = 8;
  // IDs 0-3 reserved for CG; IDs 4-7 for regular scheduling.
  const unsigned        nof_cg_reserved = 4;
  const unsigned        k2              = 6;
  const unsigned        max_retxs       = 4;
  const unsigned        cg_harq_timeout = 160;
  slot_point            pusch_slot;
  unique_ue_harq_entity harq_ent = cell_harqs.add_ue(ue_index, rnti, nof_harqs, nof_harqs);
};

TEST_F(cg_harq_reservation_test, when_cg_harq_id_requested_then_returns_process_with_exact_id)
{
  // Request a specific HARQ-ID from the CG-reserved range [0, nof_cg_reserved).
  constexpr harq_id_t requested_id = to_harq_id(2);
  auto                h_ul =
      harq_ent.alloc_ul_harq(current_slot, pusch_slot, max_retxs, cg_harq_alloc_params{requested_id, cg_harq_timeout});

  ASSERT_TRUE(h_ul.has_value());
  ASSERT_EQ(h_ul->id(), requested_id);
}

TEST_F(cg_harq_reservation_test, when_cg_harq_id_outside_reserved_range_then_alloc_fails)
{
  // Request a HARQ-ID that falls in the non-reserved range (>= nof_cg_reserved). This should be rejected.
  const harq_id_t non_reserved_id = to_harq_id(nof_cg_reserved);
  const auto      h_ul            = harq_ent.alloc_ul_harq(
      current_slot, pusch_slot, max_retxs, cg_harq_alloc_params{non_reserved_id, cg_harq_timeout});

  ASSERT_FALSE(h_ul.has_value());
}

TEST_F(cg_harq_reservation_test, when_no_harq_id_specified_then_returns_non_reserved_id)
{
  // Without a specific HARQ-ID, allocation must pick from the non-reserved range.
  auto h_ul = harq_ent.alloc_ul_harq(current_slot, pusch_slot, max_retxs);

  ASSERT_TRUE(h_ul.has_value());
  ASSERT_GE(h_ul->id(), to_harq_id(nof_cg_reserved));
}

/// \brief Allocates the CG HARQ \c id and reports a CRC for it.
///
/// \return Whether the HARQ was left free, i.e. available for the next occasion that reuses the same ID.
static bool cg_harq_is_free_after_crc(unique_ue_harq_entity& harq_ent,
                                      harq_id_t              id,
                                      slot_point             pdcch_slot,
                                      slot_point             pusch_slot,
                                      unsigned               max_retxs,
                                      unsigned               cg_harq_timeout,
                                      bool                   crc_ack)
{
  auto h_ul = harq_ent.alloc_ul_harq(pdcch_slot, pusch_slot, max_retxs, cg_harq_alloc_params{id, cg_harq_timeout});
  EXPECT_TRUE(h_ul.has_value());
  EXPECT_TRUE(h_ul->ul_crc_info(crc_ack).has_value());
  return not h_ul->is_waiting_ack() and not h_ul->has_pending_retx();
}

TEST_F(cg_harq_reservation_test, crc_ack_frees_the_cg_harq_for_the_next_occasion)
{
  // A CG HARQ ID recurs every nof_harq_processes CG periods, far sooner than the CG HARQ timeout when the two are
  // misconfigured, so it is the CRC that has to release it in time.
  constexpr harq_id_t cg_id = to_harq_id(0);
  // NOTE: the check is on the HARQ state, not on whether the ID can be allocated again: forced reuse hands the ID out
  // whether or not the CRC released it, so a successful re-allocation would prove nothing.
  ASSERT_TRUE(cg_harq_is_free_after_crc(
      harq_ent, cg_id, current_slot, pusch_slot, max_retxs, cg_harq_timeout, /*crc_ack=*/true));
}

TEST_F(cg_harq_reservation_test, crc_ko_frees_the_cg_harq_when_no_retransmission_is_allowed)
{
  // CG PUSCHs are scheduled with nof_harq_retx = 0, so a CRC KO must free the process rather than leave it pending a
  // retransmission the CG scheduler would never issue.
  constexpr harq_id_t cg_id   = to_harq_id(0);
  constexpr unsigned  no_retx = 0;
  ASSERT_TRUE(cg_harq_is_free_after_crc(
      harq_ent, cg_id, current_slot, pusch_slot, no_retx, cg_harq_timeout, /*crc_ack=*/false));
}

TEST_F(cg_harq_reservation_test, cg_harq_still_waiting_ack_is_forcibly_reused)
{
  // When no CRC ever arrives and the timeout has not elapsed either, the process is still waiting for an ACK when its
  // next occasion comes round. Rather than stall the CG, the HARQ manager reclaims it, which is what lets the CG
  // scheduler keep issuing one grant per period. Note that this is a safety net: a configuration with
  // nof_harq_processes >= configured_grant_timer never reaches it, which validate_cg_cfg() enforces.
  constexpr harq_id_t cg_id = to_harq_id(0);

  auto h_first =
      harq_ent.alloc_ul_harq(current_slot, pusch_slot, max_retxs, cg_harq_alloc_params{cg_id, cg_harq_timeout});
  ASSERT_TRUE(h_first.has_value());
  ASSERT_TRUE(h_first->is_waiting_ack()) << "the HARQ should still be awaiting its CRC";

  // Same ID requested again while busy: it is handed out anyway, the previous transmission being discarded.
  auto h_second =
      harq_ent.alloc_ul_harq(current_slot + 1, pusch_slot + 1, max_retxs, cg_harq_alloc_params{cg_id, cg_harq_timeout});
  ASSERT_TRUE(h_second.has_value()) << "a busy CG HARQ must be forcibly reused rather than stall the CG occasions";
  ASSERT_EQ(h_second->id(), cg_id);
}

TEST_F(cg_harq_reservation_test, repeated_forced_reuse_of_a_cg_harq_keeps_the_free_list_sound)
{
  // Every occasion reuses the same ID without ever reporting a CRC. If forced reuse corrupted the free list, a later
  // allocation would start failing, or hand out an ID other than the one requested.
  constexpr harq_id_t cg_id      = to_harq_id(0);
  constexpr unsigned  nof_cycles = 16;

  for (unsigned i = 0; i != nof_cycles; ++i) {
    auto h_ul = harq_ent.alloc_ul_harq(
        current_slot + i, pusch_slot + i, max_retxs, cg_harq_alloc_params{cg_id, cg_harq_timeout});
    ASSERT_TRUE(h_ul.has_value()) << "CG HARQ allocation failed on cycle " << i;
    ASSERT_EQ(h_ul->id(), cg_id) << "a different HARQ ID was handed out on cycle " << i;
  }

  // The other reserved IDs must be untouched by all that reuse.
  for (unsigned id = 1; id != nof_cg_reserved; ++id) {
    auto h_ul = harq_ent.alloc_ul_harq(
        current_slot, pusch_slot, max_retxs, cg_harq_alloc_params{to_harq_id(id), cg_harq_timeout});
    ASSERT_TRUE(h_ul.has_value()) << "reserved CG HARQ id=" << id << " was lost";
    ASSERT_EQ(h_ul->id(), to_harq_id(id));
  }
}

TEST_F(cg_harq_reservation_test, when_no_harq_id_specified_then_reserved_ids_are_never_allocated)
{
  // Drain all non-reserved HARQ slots via regular scheduling.
  const unsigned nof_non_reserved = nof_harqs - nof_cg_reserved;
  for (unsigned i = 0; i != nof_non_reserved; ++i) {
    auto h_ul = harq_ent.alloc_ul_harq(current_slot, pusch_slot, max_retxs);
    ASSERT_TRUE(h_ul.has_value());
    ASSERT_GE(h_ul->id(), to_harq_id(nof_cg_reserved));
  }

  // Non-reserved pool is exhausted; further allocations without a specific ID must fail.
  ASSERT_FALSE(harq_ent.alloc_ul_harq(current_slot, pusch_slot, max_retxs).has_value());

  // Reserved IDs are still free and can be allocated individually.
  for (unsigned id = 0; id != nof_cg_reserved; ++id) {
    auto h_ul = harq_ent.alloc_ul_harq(
        current_slot, pusch_slot, max_retxs, cg_harq_alloc_params{to_harq_id(id), cg_harq_timeout});
    ASSERT_TRUE(h_ul.has_value());
    ASSERT_EQ(h_ul->id(), to_harq_id(id));
  }
}

/// \brief A mode B transmission is counted for the slice it was sized for alone.
///
/// A process in mode B is freed as soon as it is used, so its bytes are held in the allocation history rather than
/// in the process, and the history has to tell the slices apart just as the processes do.
TEST_F(single_ntn_ue_ul_harq_mode_b_process_test,
       mode_b_bytes_waiting_ack_are_counted_for_the_slice_they_were_sized_for)
{
  const ran_slice_id_t slice{1};
  const ran_slice_id_t other_slice{2};

  // The transmission the fixture allocated is bound to no slice.
  const units::bytes unbound_bytes = pusch_info.tb_size_bytes;
  ASSERT_EQ(harq_ent.total_ul_bytes_waiting_ack(), unbound_bytes);

  // A second one, in a later slot so that it takes a history entry of its own, is bound to a slice.
  ul_harq_process_handle h_ul_slice = harq_ent
                                          .alloc_ul_harq(current_slot,
                                                         current_slot + k2 + ntn_cs_koffset + 1,
                                                         max_retxs,
                                                         std::nullopt,
                                                         harq_utils::harq_mode_t::feedback_disabled_or_mode_b)
                                          .value();
  ul_harq_alloc_context ctxt{dci_ul_rnti_config_type::c_rnti_f0_0};
  ctxt.slice_id      = slice;
  pusch_info.harq_id = h_ul_slice.id();
  h_ul_slice.save_grant_params(ctxt, pusch_info);

  ASSERT_EQ(harq_ent.total_ul_bytes_waiting_ack(), unbound_bytes + pusch_info.tb_size_bytes);
  // The one naming no slice counts for both of them, the one naming a slice only for that slice.
  EXPECT_EQ(harq_ent.total_ul_bytes_waiting_ack(slice), unbound_bytes + pusch_info.tb_size_bytes);
  EXPECT_EQ(harq_ent.total_ul_bytes_waiting_ack(other_slice), unbound_bytes);
}
