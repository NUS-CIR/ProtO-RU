// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "phy_dummy_fapi_sector_adaptor.h"
#include "ocudu/fapi/common/error_indication.h"
#include "ocudu/fapi/common/error_indication_notifier.h"
#include "ocudu/fapi/p5/p5_messages.h"
#include "ocudu/fapi/p5/p5_responses_notifier.h"
#include "ocudu/fapi/p7/messages/crc_indication.h"
#include "ocudu/fapi/p7/messages/dl_tti_request.h"
#include "ocudu/fapi/p7/messages/rach_indication.h"
#include "ocudu/fapi/p7/messages/rx_data_indication.h"
#include "ocudu/fapi/p7/messages/slot_indication.h"
#include "ocudu/fapi/p7/messages/srs_indication.h"
#include "ocudu/fapi/p7/messages/tx_data_request.h"
#include "ocudu/fapi/p7/messages/uci_indication.h"
#include "ocudu/fapi/p7/messages/ul_dci_request.h"
#include "ocudu/fapi/p7/messages/ul_tti_request.h"
#include "ocudu/fapi/p7/p7_indications_notifier.h"
#include "ocudu/fapi/p7/p7_slot_indication_notifier.h"
#include "ocudu/fapi_adaptor/phy_dummy/phy_dummy_fapi_adaptor_factory.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/executors/manual_task_worker.h"
#include "ocudu/support/executors/task_worker.h"
#include <atomic>
#include <gtest/gtest.h>
#include <thread>

using namespace ocudu;
using namespace fapi_adaptor;

namespace {

class p5_responses_notifier_spy : public fapi::p5_responses_notifier
{
public:
  void on_param_response(const fapi::param_response& msg) override { param_responses.push_back(msg); }
  void on_config_response(const fapi::config_response& msg) override { config_responses.push_back(msg); }
  void on_stop_indication(const fapi::stop_indication& msg) override { ++nof_stop_indications; }

  std::vector<fapi::param_response>  param_responses;
  std::vector<fapi::config_response> config_responses;
  std::atomic<unsigned>              nof_stop_indications{0};
};

class error_indication_notifier_spy : public fapi::error_indication_notifier
{
public:
  void on_error_indication(const fapi::error_indication& msg) override { errors.push_back(msg); }

  std::vector<fapi::error_indication> errors;
};

class slot_indication_notifier_spy : public fapi::p7_slot_indication_notifier
{
public:
  void on_slot_indication(const fapi::slot_indication& msg) override { slots.push_back(msg.slot.without_hyper_sfn()); }

  std::vector<slot_point> slots;
};

class p7_indications_notifier_spy : public fapi::p7_indications_notifier
{
public:
  void on_rx_data_indication(const fapi::rx_data_indication& msg) override
  {
    rx_data.push_back(msg);
    rx_data_payloads.emplace_back(msg.pdu.transport_block.begin(), msg.pdu.transport_block.end());
  }
  void on_crc_indication(const fapi::crc_indication& msg) override { crcs.push_back(msg); }
  void on_uci_indication(const fapi::uci_indication& msg) override { ucis.push_back(msg); }
  void on_srs_indication(const fapi::srs_indication& msg) override { srss.push_back(msg); }
  void on_rach_indication(const fapi::rach_indication& msg) override { ++nof_rach; }

  bool empty() const { return rx_data.empty() && crcs.empty() && ucis.empty() && srss.empty() && nof_rach == 0; }

  std::vector<fapi::rx_data_indication> rx_data;
  std::vector<std::vector<uint8_t>>     rx_data_payloads;
  std::vector<fapi::crc_indication>     crcs;
  std::vector<fapi::uci_indication>     ucis;
  std::vector<fapi::srs_indication>     srss;
  unsigned                              nof_rach = 0;
};

constexpr subcarrier_spacing test_scs          = subcarrier_spacing::kHz30;
constexpr unsigned           test_ul_delay     = 3;
constexpr unsigned           test_proc_delay   = 2;
constexpr rnti_t             test_rnti         = to_rnti(0x4601);
constexpr unsigned           test_nof_rx_ports = 2;

fapi::ul_pusch_pdu build_pusch_pdu(std::optional<units::bytes> tb_size, units::bits harq_bits, units::bits csi1_bits)
{
  fapi::ul_pusch_pdu pdu = {};
  pdu.rnti               = test_rnti;
  pdu.harq_id            = to_harq_id(5);
  if (tb_size.has_value()) {
    pdu.pusch_data.emplace();
    pdu.pusch_data->tb_size = *tb_size;
  }
  if (harq_bits.value() > 0 || csi1_bits.value() > 0) {
    pdu.pusch_uci.emplace();
    pdu.pusch_uci->harq_ack_bit  = harq_bits;
    pdu.pusch_uci->csi_part1_bit = csi1_bits;
  }
  return pdu;
}

class phy_dummy_fapi_sector_adaptor_test : public ::testing::Test
{
protected:
  phy_dummy_fapi_sector_adaptor_test() :
    sector({.sector_id                  = 0,
            .scs                        = test_scs,
            .ul_indication_delay_slots  = test_ul_delay,
            .max_processing_delay_slots = test_proc_delay,
            .late_request_detection     = true},
           ocudulog::fetch_basic_logger("FAPI"),
           worker)
  {
    p5_adaptor().set_p5_responses_notifier(p5_spy);
    p5_adaptor().set_error_indication_notifier(error_spy);
    p7_adaptor().set_error_indication_notifier(error_spy);
    p7_adaptor().set_p7_slot_indication_notifier(slot_spy);
    p7_adaptor().set_p7_indications_notifier(p7_spy);
  }

  // Pending tasks hold the sector, which waits for them on destruction.
  ~phy_dummy_fapi_sector_adaptor_test() override { run_tasks(); }

  phy_fapi_p5_sector_adaptor& p5_adaptor() { return sector.get_p5_sector_adaptor(); }

  /// Runs the tasks deferred to the sector executor.
  void run_tasks() { worker.run_pending_tasks(); }

  /// Notifies the last request of the given slot.
  void complete_slot(slot_point slot)
  {
    p7_adaptor().get_p7_last_request_notifier().on_last_message(slot);
    run_tasks();
  }
  phy_fapi_p7_sector_adaptor& p7_adaptor() { return sector.get_p7_sector_adaptor(); }

  /// Configures and starts the sector, and notifies the first slot boundary.
  void start()
  {
    fapi::config_request config            = {};
    config.cell_cfg.carrier_cfg.num_rx_ant = test_nof_rx_ports;
    p5_adaptor().get_p5_requests_gateway().send_config_request(config);
    p5_adaptor().get_p5_requests_gateway().send_start_request({});
    run_tasks();
    run_slot();
  }

  /// \brief Notifies the next slot boundary and returns the notified slot.
  ///
  /// The last request of the previously notified slot is notified first, unless automatic completion is disabled.
  slot_point run_slot()
  {
    if (auto_complete && notified_any) {
      complete_slot(last_notified_slot());
    }
    slot_point_extended notified = current + test_proc_delay;
    sector.handle_slot_boundary(notified, std::chrono::system_clock::now());
    run_tasks();
    ++current;
    notified_any = true;
    return notified.without_hyper_sfn();
  }

  /// Returns the slot of the last notified slot boundary.
  slot_point last_notified_slot() const { return (current - 1 + test_proc_delay).without_hyper_sfn(); }

  void send_ul_tti(slot_point slot, std::initializer_list<fapi::ul_tti_request_pdu> pdus)
  {
    fapi::ul_tti_request msg;
    msg.slot = slot;
    for (const auto& pdu : pdus) {
      msg.pdus.push_back(pdu);
    }
    p7_adaptor().get_p7_requests_gateway().send_ul_tti_request(msg);
  }

  /// Runs slot boundaries until the indications of the given uplink slot are due, and checks none comes earlier.
  void run_until_ul_indications(slot_point ul_slot)
  {
    while (last_notified_slot() + 1 != ul_slot + test_proc_delay + test_ul_delay) {
      run_slot();
      ASSERT_TRUE(p7_spy.empty()) << fmt::format("Indications notified before slot {}",
                                                 ul_slot + test_proc_delay + test_ul_delay);
    }
    run_slot();
  }

  manual_task_worker            worker{256, false};
  p5_responses_notifier_spy     p5_spy;
  error_indication_notifier_spy error_spy;
  slot_indication_notifier_spy  slot_spy;
  p7_indications_notifier_spy   p7_spy;
  phy_dummy_fapi_sector_adaptor sector;
  slot_point_extended           current{test_scs, 1000};
  bool                          notified_any  = false;
  bool                          auto_complete = true;
};

} // namespace

TEST_F(phy_dummy_fapi_sector_adaptor_test, param_and_config_requests_succeed)
{
  p5_adaptor().get_p5_requests_gateway().send_param_request({});
  p5_adaptor().get_p5_requests_gateway().send_config_request({});
  run_tasks();

  ASSERT_EQ(p5_spy.param_responses.size(), 1);
  ASSERT_EQ(p5_spy.config_responses.size(), 1);
  ASSERT_EQ(p5_spy.param_responses[0].error_code, fapi::error_code_id::msg_ok);
  ASSERT_EQ(p5_spy.config_responses[0].error_code, fapi::error_code_id::msg_ok);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, slot_indications_are_notified_only_while_running)
{
  run_slot();
  ASSERT_TRUE(slot_spy.slots.empty());

  start();
  slot_point second = run_slot();
  ASSERT_EQ(slot_spy.slots.size(), 2);
  ASSERT_EQ(slot_spy.slots.back(), second);
  ASSERT_EQ(slot_spy.slots.front() + 1, second);

  // The sector stops at the next slot boundary.
  p5_adaptor().get_p5_requests_gateway().send_stop_request({});
  run_tasks();
  ASSERT_EQ(p5_spy.nof_stop_indications, 0);
  run_slot();
  ASSERT_EQ(p5_spy.nof_stop_indications, 1);
  ASSERT_EQ(slot_spy.slots.size(), 2);

  run_slot();
  ASSERT_EQ(slot_spy.slots.size(), 2);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, stop_before_the_last_request_of_a_slot_lets_the_slot_complete)
{
  start();
  auto_complete   = false;
  slot_point slot = run_slot();

  p5_adaptor().get_p5_requests_gateway().send_stop_request({});
  run_tasks();
  complete_slot(slot);

  phy_dummy_fapi_sector_adaptor::slot_boundary_progress progress = sector.get_progress();
  ASSERT_EQ(progress.slot, slot);
  ASSERT_TRUE(progress.running);

  slot_point stop_slot = run_slot();
  progress             = sector.get_progress();
  ASSERT_EQ(progress.slot, stop_slot);
  ASSERT_FALSE(progress.running);
  ASSERT_EQ(p5_spy.nof_stop_indications, 1);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, idle_sector_finishes_slot_boundaries_right_away)
{
  slot_point slot = run_slot();

  phy_dummy_fapi_sector_adaptor::slot_boundary_progress progress = sector.get_progress();
  ASSERT_EQ(progress.slot, slot);
  ASSERT_FALSE(progress.running);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test,
       running_sector_finishes_slot_boundaries_right_away_until_the_mac_completes_a_slot)
{
  start();
  auto_complete = false;

  // The MAC discards slot indications until the cell is active, so the first slots do not wait for the MAC.
  slot_point slot = last_notified_slot();
  ASSERT_EQ(sector.get_progress().slot, slot);
  ASSERT_FALSE(sector.get_progress().running);

  complete_slot(slot);
  phy_dummy_fapi_sector_adaptor::slot_boundary_progress progress = sector.get_progress();
  ASSERT_EQ(progress.slot, slot);
  ASSERT_TRUE(progress.running);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, running_sector_finishes_slot_boundary_with_last_message)
{
  start();
  run_slot();

  // From the first completed slot on, a slot boundary finishes with its last request.
  slot_point slot = last_notified_slot();
  ASSERT_NE(sector.get_progress().slot, slot);

  complete_slot(slot);
  phy_dummy_fapi_sector_adaptor::slot_boundary_progress progress = sector.get_progress();
  ASSERT_EQ(progress.slot, slot);
  ASSERT_TRUE(progress.running);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, pusch_generates_uci_crc_and_rx_data)
{
  start();
  slot_point ul_slot = last_notified_slot();
  send_ul_tti(ul_slot, {{build_pusch_pdu(units::bytes(100), units::bits(2), units::bits(4))}});

  run_until_ul_indications(ul_slot);

  ASSERT_EQ(p7_spy.ucis.size(), 1);
  ASSERT_EQ(p7_spy.ucis[0].slot, ul_slot);
  const auto* uci = std::get_if<fapi::uci_pusch_pdu>(&p7_spy.ucis[0].pdu);
  ASSERT_NE(uci, nullptr);
  ASSERT_EQ(uci->rnti, test_rnti);
  ASSERT_TRUE(uci->harq.has_value());
  ASSERT_EQ(uci->harq->detection_status, uci_pusch_or_pucch_f2_3_4_detection_status::no_dtx);
  ASSERT_EQ(uci->harq->expected_bit_length, units::bits(2));
  ASSERT_TRUE(uci->harq->payload.all());
  ASSERT_TRUE(uci->csi_part1.has_value());
  ASSERT_EQ(uci->csi_part1->payload.size(), 4);
  ASSERT_FALSE(uci->csi_part2.has_value());

  ASSERT_EQ(p7_spy.crcs.size(), 1);
  ASSERT_EQ(p7_spy.crcs[0].slot, ul_slot);
  ASSERT_EQ(p7_spy.crcs[0].pdu.rnti, test_rnti);
  ASSERT_EQ(p7_spy.crcs[0].pdu.harq_id, to_harq_id(5));
  ASSERT_TRUE(p7_spy.crcs[0].pdu.tb_crc_status_ok);

  ASSERT_EQ(p7_spy.rx_data.size(), 1);
  ASSERT_EQ(p7_spy.rx_data[0].slot, ul_slot);
  ASSERT_EQ(p7_spy.rx_data[0].pdu.rnti, test_rnti);
  ASSERT_EQ(p7_spy.rx_data[0].pdu.harq_id, to_harq_id(5));
  ASSERT_EQ(p7_spy.rx_data_payloads[0].size(), 100);
  // The first byte is a padding subheader, so the MAC discards the rest of the PDU.
  ASSERT_EQ(p7_spy.rx_data_payloads[0][0], 0x3f);

  // Indications are notified once.
  run_slot();
  ASSERT_EQ(p7_spy.crcs.size(), 1);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, pusch_without_uci_generates_no_uci)
{
  start();
  slot_point ul_slot = last_notified_slot();
  send_ul_tti(ul_slot, {{build_pusch_pdu(units::bytes(10), units::bits(0), units::bits(0))}});

  run_until_ul_indications(ul_slot);

  ASSERT_TRUE(p7_spy.ucis.empty());
  ASSERT_EQ(p7_spy.crcs.size(), 1);
  ASSERT_EQ(p7_spy.rx_data.size(), 1);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, pucch_formats_generate_uci)
{
  start();
  slot_point ul_slot = last_notified_slot();

  fapi::ul_pucch_pdu f1 = {};
  f1.rnti               = test_rnti;
  f1.format             = fapi::ul_pucch_pdu_format_1{.sr_present = true, .bit_len_harq = units::bits(2)};

  fapi::ul_pucch_pdu f2 = {};
  f2.rnti               = test_rnti;
  f2.format             = fapi::ul_pucch_pdu_format_2{
                  .sr_bit_len = sr_nof_bits::one, .csi_part1_bit_length = units::bits(4), .bit_len_harq = units::bits(3)};

  fapi::ul_pucch_pdu f3 = {};
  f3.rnti               = test_rnti;
  f3.format             = fapi::ul_pucch_pdu_format_3{
                  .sr_bit_len = sr_nof_bits::no_sr, .csi_part1_bit_length = units::bits(11), .bit_len_harq = units::bits(4)};

  send_ul_tti(ul_slot, {{f1}, {f2}, {f3}});
  run_until_ul_indications(ul_slot);

  ASSERT_EQ(p7_spy.ucis.size(), 3);

  const auto* uci_f1 = std::get_if<fapi::uci_pucch_pdu_format_0_1>(&p7_spy.ucis[0].pdu);
  ASSERT_NE(uci_f1, nullptr);
  ASSERT_EQ(uci_f1->pucch_format, fapi::uci_pucch_pdu_format_0_1::format_type::format_1);
  ASSERT_TRUE(uci_f1->sr.has_value());
  ASSERT_FALSE(uci_f1->sr->sr_detected);
  ASSERT_TRUE(uci_f1->harq.has_value());
  ASSERT_EQ(uci_f1->harq->harq_values.size(), 2);
  ASSERT_EQ(uci_f1->harq->harq_values[0], uci_pucch_f0_or_f1_harq_values::ack);
  ASSERT_EQ(uci_f1->harq->harq_values[1], uci_pucch_f0_or_f1_harq_values::ack);

  const auto* uci_f2 = std::get_if<fapi::uci_pucch_pdu_format_2_3_4>(&p7_spy.ucis[1].pdu);
  ASSERT_NE(uci_f2, nullptr);
  ASSERT_EQ(uci_f2->pucch_format, fapi::uci_pucch_pdu_format_2_3_4::format_type::format_2);
  ASSERT_TRUE(uci_f2->sr.has_value());
  ASSERT_EQ(uci_f2->sr->sr_payload.size(), 1);
  ASSERT_TRUE(uci_f2->sr->sr_payload.none());
  ASSERT_EQ(uci_f2->harq->detection_status, uci_pusch_or_pucch_f2_3_4_detection_status::no_dtx);
  ASSERT_EQ(uci_f2->harq->payload.size(), 3);
  ASSERT_TRUE(uci_f2->harq->payload.all());
  ASSERT_EQ(uci_f2->csi_part1->payload.size(), 4);

  const auto* uci_f3 = std::get_if<fapi::uci_pucch_pdu_format_2_3_4>(&p7_spy.ucis[2].pdu);
  ASSERT_NE(uci_f3, nullptr);
  ASSERT_EQ(uci_f3->pucch_format, fapi::uci_pucch_pdu_format_2_3_4::format_type::format_3);
  ASSERT_FALSE(uci_f3->sr.has_value());
  ASSERT_EQ(uci_f3->harq->detection_status, uci_pusch_or_pucch_f2_3_4_detection_status::crc_pass);
  ASSERT_EQ(uci_f3->csi_part1->detection_status, uci_pusch_or_pucch_f2_3_4_detection_status::crc_pass);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, srs_generates_requested_reports)
{
  start();
  slot_point ul_slot = last_notified_slot();

  fapi::ul_srs_pdu srs_iq                     = {};
  srs_iq.rnti                                 = test_rnti;
  srs_iq.num_ant_ports                        = srs_resource_configuration::one_two_four_enum::two;
  srs_iq.enable_normalized_iq_matrix_report   = true;
  fapi::ul_srs_pdu srs_pos                    = srs_iq;
  srs_pos.enable_normalized_iq_matrix_report  = false;
  srs_pos.enable_positioning_report           = true;
  fapi::ul_srs_pdu srs_none                   = srs_iq;
  srs_none.enable_normalized_iq_matrix_report = false;

  send_ul_tti(ul_slot, {{srs_iq}, {srs_pos}, {srs_none}});
  run_until_ul_indications(ul_slot);

  ASSERT_EQ(p7_spy.srss.size(), 2);
  ASSERT_TRUE(p7_spy.srss[0].pdu.matrix.has_value());
  ASSERT_EQ(p7_spy.srss[0].pdu.matrix->get_nof_rx_ports(), test_nof_rx_ports);
  ASSERT_EQ(p7_spy.srss[0].pdu.matrix->get_nof_tx_ports(), 2);
  ASSERT_FALSE(p7_spy.srss[0].pdu.positioning.has_value());
  ASSERT_FALSE(p7_spy.srss[1].pdu.matrix.has_value());
  ASSERT_TRUE(p7_spy.srss[1].pdu.positioning.has_value());
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, prach_generates_no_indication)
{
  start();
  slot_point ul_slot = last_notified_slot();

  send_ul_tti(ul_slot, {{fapi::ul_prach_pdu{}}});
  run_until_ul_indications(ul_slot);

  ASSERT_TRUE(p7_spy.empty());
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, every_late_request_of_a_slot_is_reported_and_dropped)
{
  start();
  auto_complete = false;

  // The requests arrive once the current time, which lags the last notified slot by the processing delay, has passed
  // the slot.
  slot_point late_slot = last_notified_slot();
  for (unsigned i = 0; i != test_proc_delay + 1; ++i) {
    run_slot();
  }
  fapi::dl_tti_request dl_msg;
  dl_msg.slot = late_slot;
  p7_adaptor().get_p7_requests_gateway().send_dl_tti_request(dl_msg);
  send_ul_tti(late_slot, {{build_pusch_pdu(units::bytes(10), units::bits(0), units::bits(0))}});
  fapi::ul_dci_request ul_dci_msg;
  ul_dci_msg.slot = late_slot;
  p7_adaptor().get_p7_requests_gateway().send_ul_dci_request(ul_dci_msg);
  fapi::tx_data_request tx_data_msg;
  tx_data_msg.slot = late_slot;
  p7_adaptor().get_p7_requests_gateway().send_tx_data_request(tx_data_msg);
  complete_slot(late_slot);

  // As in the PHY, each late request is reported, so that the MAC discards every affected channel.
  ASSERT_EQ(error_spy.errors.size(), 4);
  ASSERT_EQ(error_spy.errors[0].message_id, fapi::message_type_id::dl_tti_request);
  ASSERT_EQ(error_spy.errors[0].error_code, fapi::error_code_id::out_of_sync);
  ASSERT_EQ(error_spy.errors[1].message_id, fapi::message_type_id::ul_tti_request);
  ASSERT_EQ(error_spy.errors[1].error_code, fapi::error_code_id::out_of_sync);
  ASSERT_EQ(error_spy.errors[2].message_id, fapi::message_type_id::ul_dci_request);
  ASSERT_EQ(error_spy.errors[2].error_code, fapi::error_code_id::msg_invalid_sfn);
  ASSERT_EQ(error_spy.errors[3].message_id, fapi::message_type_id::tx_data_request);
  ASSERT_EQ(error_spy.errors[3].error_code, fapi::error_code_id::msg_invalid_sfn);
  for (const auto& error : error_spy.errors) {
    ASSERT_EQ(error.slot, late_slot);
  }

  // The uplink requests of a slot with late requests generate no indication.
  run_until_ul_indications(late_slot);
  ASSERT_TRUE(p7_spy.empty());
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, request_arriving_at_its_slot_is_not_late)
{
  start();
  auto_complete = false;

  // The current time is the slot of the request.
  slot_point slot = last_notified_slot();
  for (unsigned i = 0; i != test_proc_delay; ++i) {
    run_slot();
  }
  send_ul_tti(slot, {{build_pusch_pdu(units::bytes(10), units::bits(0), units::bits(0))}});
  complete_slot(slot);

  ASSERT_TRUE(error_spy.errors.empty());
  ASSERT_EQ(sector.get_progress().slot, slot);
}

TEST_F(phy_dummy_fapi_sector_adaptor_test, requests_are_ignored_when_not_running)
{
  slot_point ul_slot = (current + test_proc_delay).without_hyper_sfn();
  send_ul_tti(ul_slot, {{build_pusch_pdu(units::bytes(10), units::bits(0), units::bits(0))}});

  start();
  while (last_notified_slot() != ul_slot + test_proc_delay + test_ul_delay + 1) {
    run_slot();
  }

  ASSERT_TRUE(p7_spy.empty());
  ASSERT_TRUE(error_spy.errors.empty());
}

namespace {

/// Slot indication notifier that completes every notified slot.
class completing_slot_indication_notifier : public fapi::p7_slot_indication_notifier
{
public:
  void on_slot_indication(const fapi::slot_indication& msg) override
  {
    slot_point slot = msg.slot.without_hyper_sfn();
    if (last_slot.valid() && slot != last_slot + 1) {
      nof_discontinuities.fetch_add(1, std::memory_order_relaxed);
    }
    last_slot = slot;
    nof_slots.fetch_add(1, std::memory_order_relaxed);
    if (last_request_notifier != nullptr) {
      last_request_notifier->on_last_message(slot);
    }
  }

  fapi::p7_last_request_notifier* last_request_notifier = nullptr;
  slot_point                      last_slot;
  std::atomic<unsigned>           nof_slots{0};
  std::atomic<unsigned>           nof_discontinuities{0};
};

/// Waits until the condition is true or the timeout expires.
template <typename Func>
bool wait_for(const Func& condition, std::chrono::milliseconds timeout = std::chrono::seconds(5))
{
  auto deadline = std::chrono::steady_clock::now() + timeout;
  while (!condition()) {
    if (std::chrono::steady_clock::now() > deadline) {
      return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  return true;
}

class phy_dummy_fapi_adaptor_clock_test : public ::testing::TestWithParam<phy_dummy_fapi_adaptor_clock_mode>
{
protected:
  phy_dummy_fapi_adaptor_clock_test() :
    clock_worker("fapi_clock", 16),
    clock_executor(make_task_executor(clock_worker)),
    sector_workers{std::make_unique<task_worker>("fapi_sector0", 64),
                   std::make_unique<task_worker>("fapi_sector1", 64)},
    sector_executors{make_task_executor(*sector_workers[0]), make_task_executor(*sector_workers[1])},
    dummy(create_phy_dummy_fapi_adaptor({.scs                        = test_scs,
                                         .nof_sectors                = nof_sectors,
                                         .clock_mode                 = GetParam(),
                                         .ul_indication_delay_slots  = test_ul_delay,
                                         .max_processing_delay_slots = test_proc_delay},
                                        {.logger           = ocudulog::fetch_basic_logger("FAPI"),
                                         .clock_executor   = clock_executor,
                                         .sector_executors = {&sector_executors[0], &sector_executors[1]}}))
  {
    for (unsigned i_sector = 0; i_sector != nof_sectors; ++i_sector) {
      phy_fapi_sector_adaptor& sector = dummy->get_sector_adaptor(i_sector);
      sector.get_p5_sector_adaptor().set_p5_responses_notifier(p5_spies[i_sector]);
      sector.get_p7_sector_adaptor().set_p7_slot_indication_notifier(slot_spies[i_sector]);
      slot_spies[i_sector].last_request_notifier = &sector.get_p7_sector_adaptor().get_p7_last_request_notifier();
    }
  }

  static constexpr unsigned nof_sectors = 2;

  task_worker                                                  clock_worker;
  task_worker_executor                                         clock_executor;
  std::array<std::unique_ptr<task_worker>, nof_sectors>        sector_workers;
  std::array<task_worker_executor, nof_sectors>                sector_executors;
  std::array<p5_responses_notifier_spy, nof_sectors>           p5_spies;
  std::array<completing_slot_indication_notifier, nof_sectors> slot_spies;
  std::unique_ptr<phy_fapi_adaptor>                            dummy;
};

} // namespace

TEST_P(phy_dummy_fapi_adaptor_clock_test, sectors_receive_consecutive_slots_until_stopped)
{
  for (unsigned i_sector = 0; i_sector != nof_sectors; ++i_sector) {
    dummy->get_sector_adaptor(i_sector).get_p5_sector_adaptor().get_p5_requests_gateway().send_start_request({});
  }

  static constexpr unsigned min_nof_slots = 50;
  for (unsigned i_sector = 0; i_sector != nof_sectors; ++i_sector) {
    ASSERT_TRUE(wait_for([&]() { return slot_spies[i_sector].nof_slots >= min_nof_slots; }));
  }

  for (unsigned i_sector = 0; i_sector != nof_sectors; ++i_sector) {
    dummy->get_sector_adaptor(i_sector).get_p5_sector_adaptor().get_p5_requests_gateway().send_stop_request({});
  }
  for (unsigned i_sector = 0; i_sector != nof_sectors; ++i_sector) {
    ASSERT_TRUE(wait_for([&]() { return p5_spies[i_sector].nof_stop_indications == 1; }));
  }

  // No slot is notified after the stop indication.
  std::array<unsigned, nof_sectors> nof_slots_at_stop;
  for (unsigned i_sector = 0; i_sector != nof_sectors; ++i_sector) {
    nof_slots_at_stop[i_sector] = slot_spies[i_sector].nof_slots;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  for (unsigned i_sector = 0; i_sector != nof_sectors; ++i_sector) {
    ASSERT_EQ(slot_spies[i_sector].nof_slots, nof_slots_at_stop[i_sector]);
    // A realtime clock skips the slot boundaries of a sector that falls behind it.
    if (GetParam() == phy_dummy_fapi_adaptor_clock_mode::free_running) {
      ASSERT_EQ(slot_spies[i_sector].nof_discontinuities, 0);
    }
  }
}

INSTANTIATE_TEST_SUITE_P(phy_dummy_fapi_adaptor_clock,
                         phy_dummy_fapi_adaptor_clock_test,
                         ::testing::Values(phy_dummy_fapi_adaptor_clock_mode::realtime,
                                           phy_dummy_fapi_adaptor_clock_mode::free_running));

int main(int argc, char** argv)
{
  ocudulog::fetch_basic_logger("FAPI").set_level(ocudulog::basic_levels::warning);
  ocudulog::init();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
