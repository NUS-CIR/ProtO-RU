// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_du_test_helpers.h"
#include "tests/test_doubles/f1ap/f1ap_test_message_validators.h"
#include "tests/test_doubles/f1ap/f1ap_test_messages.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/du/du_cell_config_helpers.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace odu;

namespace {

constexpr uint16_t test_azimuth_aoa    = 1234;
constexpr uint16_t test_zenith_aoa     = 567;
constexpr uint16_t test_lmf_ue_meas_id = 3;
constexpr uint16_t test_ran_ue_meas_id = 4;

/// Identifiers of a second measurement, used when one measurement replaces another.
/// Orientation of the local coordinate system that the DU reports.
constexpr uint16_t test_alpha = 100;
constexpr uint16_t test_beta  = 200;
constexpr uint16_t test_gamma = 300;

constexpr uint16_t other_lmf_ue_meas_id = 5;
constexpr uint16_t other_ran_ue_meas_id = 6;

using e_cid_quantity = asn1::f1ap::e_c_id_meas_quantities_value_opts::options;

/// Shortest Measurement Periodicity NR-AoA, as per TS 38.473 section 9.2.12.20.
constexpr auto     test_nr_aoa_period    = asn1::f1ap::pos_meas_periodicity_nr_ao_a_opts::options::ms160;
constexpr unsigned test_nr_aoa_period_ms = 160;

} // namespace

class f1ap_du_e_cid_measurement_procedure_test : public f1ap_du_test
{
protected:
  f1ap_du_e_cid_measurement_procedure_test()
  {
    // Test Preamble.
    run_f1_setup_procedure();
    run_f1ap_ue_create(test_ue_index);
    f1ap_message msg = test_helpers::generate_ue_context_setup_request(
        cu_ue_id, du_ue_id, 1, {}, config_helpers::make_default_du_cell_config().nr_cgi);
    run_ue_context_setup_procedure(test_ue_index, msg);

    this->f1c_gw.clear_tx_pdus();
  }

  /// Programs the DU to report a successful E-CID measurement with a UL angle of arrival.
  void du_reports_aoa(std::optional<uint16_t> zenith_aoa = test_zenith_aoa)
  {
    du_e_cid_meas_response& resp = this->f1ap_du_cfg_handler.next_e_cid_meas_response;
    resp.success                 = true;
    resp.ul_aoa_results.push_back(pos_meas_result_ul_aoa{.azimuth_aoa = test_azimuth_aoa, .zenith_aoa = zenith_aoa});
  }

  f1ap_message make_request(const std::vector<e_cid_quantity>&                      quantities,
                            asn1::f1ap::e_c_id_report_characteristics_opts::options report_characteristics =
                                asn1::f1ap::e_c_id_report_characteristics_opts::options::on_demand)
  {
    return test_helpers::generate_e_cid_measurement_initiation_request(
        cu_ue_id, du_ue_id, quantities, report_characteristics, test_lmf_ue_meas_id, test_ran_ue_meas_id);
  }

  /// Builds a periodic request paced by the Measurement Periodicity NR-AoA IE.
  f1ap_message make_periodic_request(
      std::optional<asn1::f1ap::pos_meas_periodicity_nr_ao_a_opts::options> nr_aoa_period = test_nr_aoa_period,
      std::optional<asn1::f1ap::meas_periodicity_opts::options>             e_cid_period  = std::nullopt)
  {
    return test_helpers::generate_e_cid_measurement_initiation_request(
        cu_ue_id,
        du_ue_id,
        {e_cid_quantity::angle_of_arrival_nr},
        asn1::f1ap::e_c_id_report_characteristics_opts::options::periodic,
        test_lmf_ue_meas_id,
        test_ran_ue_meas_id,
        nr_aoa_period,
        e_cid_period);
  }

  /// Advances time by \c nof_ms milliseconds.
  void tick_ms(unsigned nof_ms)
  {
    for (unsigned i = 0; i != nof_ms; ++i) {
      this->tick();
    }
  }

  /// Counts the E-CID MEASUREMENT REPORT messages that the DU sent.
  unsigned pop_nof_reports()
  {
    unsigned count = 0;
    while (auto msg = this->f1c_gw.pop_tx_pdu()) {
      if (msg->pdu.type().value == asn1::f1ap::f1ap_pdu_c::types_opts::init_msg and
          msg->pdu.init_msg().value.type().value ==
              asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::e_c_id_meas_report) {
        ++count;
      }
    }
    return count;
  }

  du_ue_index_t       test_ue_index = to_du_ue_index(0);
  gnb_cu_ue_f1ap_id_t cu_ue_id      = gnb_cu_ue_f1ap_id_t{0};
  gnb_du_ue_f1ap_id_t du_ue_id      = gnb_du_ue_f1ap_id_t{0};
};

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_valid_on_demand_request_is_received_then_du_is_notified)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr}));

  ASSERT_TRUE(this->f1ap_du_cfg_handler.last_e_cid_meas_request.has_value());
  const auto& du_req = this->f1ap_du_cfg_handler.last_e_cid_meas_request.value();
  ASSERT_EQ(du_req.ue_index, test_ue_index);
  ASSERT_EQ(du_req.quantities.size(), 1);
  ASSERT_EQ(du_req.quantities[0], e_cid_meas_quantity::nr_angle_of_arrival);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_nr_aoa_is_requested_then_response_contains_nr_aoa)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr}));

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_response(tx_msg.value()));

  const auto& resp = tx_msg.value().pdu.successful_outcome().value.e_c_id_meas_initiation_resp();
  ASSERT_EQ(resp->gnb_cu_ue_f1ap_id, to_underlying(cu_ue_id));
  ASSERT_EQ(resp->gnb_du_ue_f1ap_id, to_underlying(du_ue_id));
  ASSERT_EQ(resp->lmf_ue_meas_id, test_lmf_ue_meas_id);
  ASSERT_EQ(resp->ran_ue_meas_id, test_ran_ue_meas_id);

  ASSERT_TRUE(resp->e_c_id_meas_result_present);
  ASSERT_EQ(resp->e_c_id_meas_result.measured_results_list.size(), 1);
  const auto& result = resp->e_c_id_meas_result.measured_results_list[0].e_c_id_measured_results_value;
  ASSERT_EQ(result.type().value, asn1::f1ap::e_c_id_measured_results_value_c::types_opts::value_angleof_arrival_nr);
  ASSERT_EQ(result.value_angleof_arrival_nr().azimuth_ao_a, test_azimuth_aoa);
  ASSERT_TRUE(result.value_angleof_arrival_nr().zenith_ao_a_present);
  ASSERT_EQ(result.value_angleof_arrival_nr().zenith_ao_a, test_zenith_aoa);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_du_reports_an_lcs_to_gcs_translation_then_response_carries_it)
{
  constexpr lcs_to_gcs_translation_t lcs_to_gcs{.alpha = 2700, .beta = 60, .gamma = 3590};
  du_reports_aoa();
  this->f1ap_du_cfg_handler.next_e_cid_meas_response.ul_aoa_results[0].lcs_to_gcs_translation = lcs_to_gcs;

  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr}));

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_response(tx_msg.value()));
  const auto& resp = tx_msg.value().pdu.successful_outcome().value.e_c_id_meas_initiation_resp();
  const auto& aoa =
      resp->e_c_id_meas_result.measured_results_list[0].e_c_id_measured_results_value.value_angleof_arrival_nr();
  ASSERT_TRUE(aoa.lcs_to_gcs_translation_present);
  ASSERT_EQ(aoa.lcs_to_gcs_translation.alpha, lcs_to_gcs.alpha);
  ASSERT_EQ(aoa.lcs_to_gcs_translation.beta, lcs_to_gcs.beta);
  ASSERT_EQ(aoa.lcs_to_gcs_translation.gamma, lcs_to_gcs.gamma);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_du_reports_no_zenith_angle_then_response_omits_it)
{
  du_reports_aoa(std::nullopt);

  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr}));

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  const auto& resp   = tx_msg.value().pdu.successful_outcome().value.e_c_id_meas_initiation_resp();
  const auto& result = resp->e_c_id_meas_result.measured_results_list[0].e_c_id_measured_results_value;
  ASSERT_FALSE(result.value_angleof_arrival_nr().zenith_ao_a_present);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_timing_advance_is_requested_then_failure_is_sent)
{
  du_reports_aoa();

  // The gNB-DU must initiate all requested quantities, or none, as per TS 38.473 section 8.13.12.3.
  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr, e_cid_quantity::timing_advance_nr}));

  ASSERT_FALSE(this->f1ap_du_cfg_handler.last_e_cid_meas_request.has_value());

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_measurement_quantities_list_is_empty_then_failure_is_sent)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_request({}));

  ASSERT_FALSE(this->f1ap_du_cfg_handler.last_e_cid_meas_request.has_value());

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_du_cannot_measure_then_failure_is_sent)
{
  // The DU reports a failed measurement.
  this->f1ap_du_cfg_handler.next_e_cid_meas_response.success = false;

  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr}));

  ASSERT_TRUE(this->f1ap_du_cfg_handler.last_e_cid_meas_request.has_value());

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));

  const auto& fail = tx_msg.value().pdu.unsuccessful_outcome().value.e_c_id_meas_initiation_fail();
  ASSERT_EQ(fail->lmf_ue_meas_id, test_lmf_ue_meas_id);
  ASSERT_EQ(fail->ran_ue_meas_id, test_ran_ue_meas_id);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_ue_does_not_exist_then_failure_is_sent)
{
  du_reports_aoa();

  // Use a gNB-DU UE F1AP ID that no UE context uses.
  f1ap_message req = test_helpers::generate_e_cid_measurement_initiation_request(
      cu_ue_id,
      gnb_du_ue_f1ap_id_t{99},
      {e_cid_quantity::angle_of_arrival_nr},
      asn1::f1ap::e_c_id_report_characteristics_opts::options::on_demand,
      test_lmf_ue_meas_id,
      test_ran_ue_meas_id);
  this->f1ap->handle_message(req);

  ASSERT_FALSE(this->f1ap_du_cfg_handler.last_e_cid_meas_request.has_value());

  // The CU-CP gets an answer instead of waiting for its own procedure timeout.
  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_only_the_default_quantity_is_requested_then_response_is_sent)
{
  this->f1ap_du_cfg_handler.next_e_cid_meas_response.success = true;

  this->f1ap->handle_message(make_request({e_cid_quantity::default_value}));

  ASSERT_TRUE(this->f1ap_du_cfg_handler.last_e_cid_meas_request.has_value());
  ASSERT_EQ(this->f1ap_du_cfg_handler.last_e_cid_meas_request->quantities[0], e_cid_meas_quantity::default_quantity);

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_response(tx_msg.value()));
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_the_measurement_waits_for_the_srs_then_the_ue_task_loop_goes_on)
{
  du_reports_aoa();
  this->f1ap_du_cfg_handler.hold_measurements = true;

  // The first part of the procedure runs on the task loop of the UE and reaches the DU.
  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr}));
  ASSERT_EQ(this->f1ap_du_cfg_handler.nof_e_cid_meas_requests, 1);
  ASSERT_FALSE(this->f1c_gw.pop_tx_pdu().has_value());

  // The task loop of the UE is free while the measurement waits.
  bool task_ran = false;
  this->f1ap_du_cfg_handler.get_ue_handler(test_ue_index)
      .schedule_async_task(launch_async([&task_ran](coro_context<async_task<void>>& ctx) {
        CORO_BEGIN(ctx);
        task_ran = true;
        CORO_RETURN();
      }));
  ASSERT_TRUE(task_ran) << "A waiting E-CID measurement blocks the task loop of the UE";

  // The response follows once the SRS is measured.
  this->f1ap_du_cfg_handler.release_measurements();
  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_response(tx_msg.value()));
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_the_ue_is_removed_during_the_wait_then_nothing_is_sent)
{
  du_reports_aoa();
  this->f1ap_du_cfg_handler.hold_measurements = true;
  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr}));
  ASSERT_EQ(this->f1ap_du_cfg_handler.nof_e_cid_meas_requests, 1);

  this->f1ap->handle_ue_deletion_request(test_ue_index);

  this->f1ap_du_cfg_handler.release_measurements();
  ASSERT_FALSE(this->f1c_gw.pop_tx_pdu().has_value()) << "The procedure sent a message for a removed UE";
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test,
       when_the_du_disconnects_while_the_request_is_queued_then_the_procedure_does_not_start)
{
  du_reports_aoa();

  // A long task of the UE holds its task loop, so the E-CID request waits in the queue.
  manual_event_flag ue_task_done;
  this->f1ap_du_cfg_handler.get_ue_handler(test_ue_index)
      .schedule_async_task(launch_async([&ue_task_done](coro_context<async_task<void>>& ctx) {
        CORO_BEGIN(ctx);
        CORO_AWAIT(ue_task_done);
        CORO_RETURN();
      }));
  this->f1ap->handle_message(make_request({e_cid_quantity::angle_of_arrival_nr}));

  // The association is removed while the request waits. The returned procedure is kept, but never started.
  async_task<void> disconnection = this->f1ap->disconnect_from_cu_cp();

  // The queued request belongs to the removed association, so it does not reach the DU.
  ue_task_done.set();
  ASSERT_EQ(this->f1ap_du_cfg_handler.nof_e_cid_meas_requests, 0);
  ASSERT_FALSE(this->f1c_gw.pop_tx_pdu().has_value());
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_periodic_is_requested_then_response_carries_no_result)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());

  // The DU measures once, so that it only accepts a request it can serve, as per TS 38.473 section 8.13.12.3.
  ASSERT_TRUE(this->f1ap_du_cfg_handler.last_e_cid_meas_request.has_value());

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_response(tx_msg.value()));

  const auto& resp = tx_msg.value().pdu.successful_outcome().value.e_c_id_meas_initiation_resp();
  ASSERT_FALSE(resp->e_c_id_meas_result_present);
  ASSERT_FALSE(resp->cell_portion_id_present);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test,
       when_periodic_aoa_is_requested_without_nr_aoa_periodicity_then_failure_is_sent)
{
  du_reports_aoa();

  // The E-CID Measurement Periodicity IE does not pace NR AoA, as per TS 38.473 section 9.2.12.20.
  this->f1ap->handle_message(make_periodic_request(std::nullopt, asn1::f1ap::meas_periodicity_opts::options::ms1024));

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_periodic_is_armed_then_reports_are_sent_on_the_nr_aoa_cadence)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();

  // No report before the first period elapses.
  tick_ms(test_nr_aoa_period_ms - 1);
  ASSERT_EQ(pop_nof_reports(), 0);

  // One report per period.
  tick_ms(1);
  ASSERT_EQ(pop_nof_reports(), 1);

  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);

  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_periodic_is_armed_then_the_report_carries_the_nr_aoa)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();

  tick_ms(test_nr_aoa_period_ms);

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  const auto& report = tx_msg.value().pdu.init_msg().value.e_c_id_meas_report();
  ASSERT_EQ(report->lmf_ue_meas_id, test_lmf_ue_meas_id);
  ASSERT_EQ(report->ran_ue_meas_id, test_ran_ue_meas_id);
  ASSERT_EQ(report->e_c_id_meas_result.measured_results_list.size(), 1);
  const auto& result = report->e_c_id_meas_result.measured_results_list[0].e_c_id_measured_results_value;
  ASSERT_EQ(result.value_angleof_arrival_nr().azimuth_ao_a, test_azimuth_aoa);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_e_cid_meas_periodicity_is_also_present_then_it_is_ignored_for_aoa)
{
  du_reports_aoa();

  // A much longer E-CID Measurement Periodicity must not slow down the NR AoA reports.
  this->f1ap->handle_message(
      make_periodic_request(test_nr_aoa_period, asn1::f1ap::meas_periodicity_opts::options::ms5120));
  this->f1c_gw.clear_tx_pdus();

  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_termination_command_is_received_then_reports_stop)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();
  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);

  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_termination_command(
      cu_ue_id, du_ue_id, test_lmf_ue_meas_id, test_ran_ue_meas_id));

  // The command needs no answer, and the reports stop.
  tick_ms(test_nr_aoa_period_ms * 3);
  ASSERT_EQ(pop_nof_reports(), 0);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_termination_command_has_an_unknown_meas_id_then_reports_continue)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();

  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_termination_command(
      cu_ue_id, du_ue_id, test_lmf_ue_meas_id, test_ran_ue_meas_id + 1));

  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test,
       when_a_periodic_measurement_fails_then_failure_indication_is_sent_and_reports_stop)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();

  // The DU can no longer measure the UE.
  this->f1ap_du_cfg_handler.next_e_cid_meas_response = du_e_cid_meas_response{};

  tick_ms(test_nr_aoa_period_ms);

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_EQ(tx_msg->pdu.init_msg().value.type().value,
            asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::e_c_id_meas_fail_ind);
  const auto& ind = tx_msg->pdu.init_msg().value.e_c_id_meas_fail_ind();
  ASSERT_EQ(ind->lmf_ue_meas_id, test_lmf_ue_meas_id);
  ASSERT_EQ(ind->ran_ue_meas_id, test_ran_ue_meas_id);

  // The measurement is terminated, as per TS 38.473 section 8.13.13.2.
  tick_ms(test_nr_aoa_period_ms * 3);
  ASSERT_EQ(pop_nof_reports(), 0);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_the_ue_is_released_then_reports_stop)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();
  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);

  this->f1ap->handle_ue_deletion_request(test_ue_index);

  tick_ms(test_nr_aoa_period_ms * 3);
  ASSERT_EQ(pop_nof_reports(), 0);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test,
       when_periodic_default_only_is_requested_then_e_cid_periodicity_paces_it)
{
  this->f1ap_du_cfg_handler.next_e_cid_meas_response.success = true;

  // Without NR AoA the E-CID Measurement Periodicity IE gives the period, as per TS 38.473 section 9.2.12.20.
  f1ap_message req = test_helpers::generate_e_cid_measurement_initiation_request(
      cu_ue_id,
      du_ue_id,
      {e_cid_quantity::default_value},
      asn1::f1ap::e_c_id_report_characteristics_opts::options::periodic,
      test_lmf_ue_meas_id,
      test_ran_ue_meas_id,
      std::nullopt,
      asn1::f1ap::meas_periodicity_opts::options::ms120);
  this->f1ap->handle_message(req);
  this->f1c_gw.clear_tx_pdus();

  // No flood before the period elapses.
  tick_ms(119);
  ASSERT_EQ(pop_nof_reports(), 0);

  tick_ms(1);
  ASSERT_EQ(pop_nof_reports(), 1);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_periodic_default_only_has_no_periodicity_then_failure_is_sent)
{
  this->f1ap_du_cfg_handler.next_e_cid_meas_response.success = true;

  f1ap_message req = test_helpers::generate_e_cid_measurement_initiation_request(
      cu_ue_id,
      du_ue_id,
      {e_cid_quantity::default_value},
      asn1::f1ap::e_c_id_report_characteristics_opts::options::periodic,
      test_lmf_ue_meas_id,
      test_ran_ue_meas_id);
  this->f1ap->handle_message(req);

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_the_du_cannot_measure_then_a_periodic_request_is_rejected)
{
  // The DU cannot serve the UE, so it must not accept the periodic measurement.
  this->f1ap_du_cfg_handler.next_e_cid_meas_response.success = false;

  this->f1ap->handle_message(make_periodic_request());

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));

  // No reporting was armed.
  tick_ms(test_nr_aoa_period_ms * 3);
  ASSERT_EQ(pop_nof_reports(), 0);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_a_periodic_measurement_fails_then_a_new_request_is_accepted)
{
  du_reports_aoa();
  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();

  // The measurement fails, so the DU terminates it and drops the reporter.
  this->f1ap_du_cfg_handler.next_e_cid_meas_response = du_e_cid_meas_response{};
  tick_ms(test_nr_aoa_period_ms);
  this->f1c_gw.clear_tx_pdus();

  // A termination command for the finished measurement finds nothing to stop.
  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_termination_command(
      cu_ue_id, du_ue_id, test_lmf_ue_meas_id, test_ran_ue_meas_id));

  // A new periodic measurement can be started.
  du_reports_aoa();
  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();
  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test,
       when_the_periodicity_codepoint_counts_minutes_then_it_is_not_read_as_ms)
{
  this->f1ap_du_cfg_handler.next_e_cid_meas_response.success = true;

  // The codepoints above ms10240 count minutes, as per TS 38.473 section 9.3.1.192.
  f1ap_message req = test_helpers::generate_e_cid_measurement_initiation_request(
      cu_ue_id,
      du_ue_id,
      {e_cid_quantity::default_value},
      asn1::f1ap::e_c_id_report_characteristics_opts::options::periodic,
      test_lmf_ue_meas_id,
      test_ran_ue_meas_id,
      std::nullopt,
      asn1::f1ap::meas_periodicity_opts::options::min1);
  this->f1ap->handle_message(req);
  this->f1c_gw.clear_tx_pdus();

  // One minute is far away. Reading the codepoint as 1 ms would report on nearly every tick.
  tick_ms(1000);
  ASSERT_EQ(pop_nof_reports(), 0);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_the_measurement_periodicity_is_extended_then_failure_is_sent)
{
  du_reports_aoa();

  // The extended codepoint puts the value in an IE that this message does not carry, so the DU has no period.
  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_initiation_request(
      cu_ue_id,
      du_ue_id,
      {e_cid_quantity::default_value},
      asn1::f1ap::e_c_id_report_characteristics_opts::options::periodic,
      test_lmf_ue_meas_id,
      test_ran_ue_meas_id,
      std::nullopt,
      asn1::f1ap::meas_periodicity_opts::options::extended));

  const std::optional<f1ap_message> pdu = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(pdu.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(pdu.value()));
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test,
       when_a_measurement_is_terminated_while_its_report_waits_then_the_report_is_not_sent)
{
  du_reports_aoa();

  // Start the first periodic measurement and hold the report of its first period.
  this->f1ap->handle_message(make_periodic_request());
  this->f1ap_du_cfg_handler.hold_measurements = true;
  tick_ms(test_nr_aoa_period_ms);
  this->f1c_gw.clear_tx_pdus();

  // The gNB-CU terminates the measurement and starts another one with its own identifiers.
  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_termination_command(
      cu_ue_id, du_ue_id, test_lmf_ue_meas_id, test_ran_ue_meas_id));
  this->f1ap_du_cfg_handler.hold_measurements = false;
  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_initiation_request(
      cu_ue_id,
      du_ue_id,
      {e_cid_quantity::angle_of_arrival_nr},
      asn1::f1ap::e_c_id_report_characteristics_opts::options::periodic,
      other_lmf_ue_meas_id,
      other_ran_ue_meas_id,
      test_nr_aoa_period));
  this->f1c_gw.clear_tx_pdus();

  // The held report belongs to the terminated measurement, so the gNB-DU drops it.
  this->f1ap_du_cfg_handler.release_measurements();
  ASSERT_EQ(pop_nof_reports(), 0) << "The report of the terminated measurement was sent";
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_a_periodic_measurement_already_runs_then_a_second_is_rejected)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();

  // The running measurement keeps its reports, so the gNB-DU rejects the second request.
  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_initiation_request(
      cu_ue_id,
      du_ue_id,
      {e_cid_quantity::angle_of_arrival_nr},
      asn1::f1ap::e_c_id_report_characteristics_opts::options::periodic,
      other_lmf_ue_meas_id,
      other_ran_ue_meas_id,
      test_nr_aoa_period));

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));

  // The first measurement still reports.
  this->f1c_gw.clear_tx_pdus();
  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_the_report_carries_an_lcs_orientation_then_it_reaches_the_cu)
{
  du_reports_aoa();
  this->f1ap_du_cfg_handler.next_e_cid_meas_response.ul_aoa_results[0].lcs_to_gcs_translation =
      lcs_to_gcs_translation_t{test_alpha, test_beta, test_gamma};

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();

  tick_ms(test_nr_aoa_period_ms);

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  const auto& result = tx_msg.value()
                           .pdu.init_msg()
                           .value.e_c_id_meas_report()
                           ->e_c_id_meas_result.measured_results_list[0]
                           .e_c_id_measured_results_value;
  const auto& aoa = result.value_angleof_arrival_nr();
  ASSERT_TRUE(aoa.lcs_to_gcs_translation_present) << "The LMF would read the LCS angles as GCS angles";
  ASSERT_EQ(aoa.lcs_to_gcs_translation.alpha, test_alpha);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_a_report_waits_for_the_srs_then_the_period_does_not_grow)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();

  // The report of the first period waits for the SRS.
  this->f1ap_du_cfg_handler.hold_measurements = true;
  tick_ms(test_nr_aoa_period_ms);

  // The SRS arrives in the middle of the second period.
  constexpr unsigned srs_wait_ms = 40;
  tick_ms(srs_wait_ms);
  this->f1ap_du_cfg_handler.hold_measurements = false;
  this->f1ap_du_cfg_handler.release_measurements();
  ASSERT_EQ(pop_nof_reports(), 1);

  // The second period keeps the cadence of the first one, so it ends before a full period after the late report.
  tick_ms(test_nr_aoa_period_ms - srs_wait_ms);
  ASSERT_EQ(pop_nof_reports(), 1) << "The wait for the SRS was added to the period";
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_a_report_is_still_pending_then_the_period_is_skipped)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  const unsigned nof_reqs_after_initiation = this->f1ap_du_cfg_handler.nof_e_cid_meas_requests;
  this->f1c_gw.clear_tx_pdus();

  // The report of the first period waits for the SRS over the whole second period.
  this->f1ap_du_cfg_handler.hold_measurements = true;
  tick_ms(test_nr_aoa_period_ms * 2);

  // The gNB-DU measures one report at a time, so the second period measures nothing.
  ASSERT_EQ(this->f1ap_du_cfg_handler.nof_e_cid_meas_requests, nof_reqs_after_initiation + 1)
      << "A period measured again while the last report was still pending";

  this->f1ap_du_cfg_handler.hold_measurements = false;
  this->f1ap_du_cfg_handler.release_measurements();
  ASSERT_EQ(pop_nof_reports(), 1);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_a_report_waits_for_the_srs_then_the_ue_task_loop_goes_on)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1ap_du_cfg_handler.hold_measurements = true;
  tick_ms(test_nr_aoa_period_ms);

  // The task loop of the UE is free while the report waits for the SRS.
  bool task_ran = false;
  this->f1ap_du_cfg_handler.get_ue_handler(test_ue_index)
      .schedule_async_task(launch_async([&task_ran](coro_context<async_task<void>>& ctx) {
        CORO_BEGIN(ctx);
        task_ran = true;
        CORO_RETURN();
      }));
  ASSERT_TRUE(task_ran) << "A waiting E-CID report blocks the task loop of the UE";
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_the_du_disconnects_then_the_periodic_reports_stop)
{
  du_reports_aoa();

  this->f1ap->handle_message(make_periodic_request());
  this->f1c_gw.clear_tx_pdus();
  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);

  // The measurement belongs to the CU-CP association that asked for it.
  async_task<void> disconnection = this->f1ap->disconnect_from_cu_cp();

  tick_ms(test_nr_aoa_period_ms * 3);
  ASSERT_EQ(pop_nof_reports(), 0) << "The gNB-DU reported to a removed CU-CP association";
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_two_periodic_requests_overlap_then_the_second_is_rejected)
{
  du_reports_aoa();

  // The first request still waits for its measurement when the second one arrives.
  this->f1ap_du_cfg_handler.hold_measurements = true;
  this->f1ap->handle_message(make_periodic_request());
  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_initiation_request(
      cu_ue_id,
      du_ue_id,
      {e_cid_quantity::angle_of_arrival_nr},
      asn1::f1ap::e_c_id_report_characteristics_opts::options::periodic,
      other_lmf_ue_meas_id,
      other_ran_ue_meas_id,
      test_nr_aoa_period));

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));
  ASSERT_EQ(tx_msg.value().pdu.unsuccessful_outcome().value.e_c_id_meas_initiation_fail()->lmf_ue_meas_id,
            other_lmf_ue_meas_id);

  // The first request still starts its reporting.
  this->f1ap_du_cfg_handler.hold_measurements = false;
  this->f1ap_du_cfg_handler.release_measurements();
  this->f1c_gw.clear_tx_pdus();
  tick_ms(test_nr_aoa_period_ms);
  ASSERT_EQ(pop_nof_reports(), 1);
}

TEST_F(f1ap_du_e_cid_measurement_procedure_test, when_the_measurement_is_terminated_before_the_response_then_it_stops)
{
  du_reports_aoa();

  // The request still waits for its measurement when the termination command arrives.
  this->f1ap_du_cfg_handler.hold_measurements = true;
  this->f1ap->handle_message(make_periodic_request());
  this->f1ap->handle_message(test_helpers::generate_e_cid_measurement_termination_command(
      cu_ue_id, du_ue_id, test_lmf_ue_meas_id, test_ran_ue_meas_id));

  this->f1ap_du_cfg_handler.hold_measurements = false;
  this->f1ap_du_cfg_handler.release_measurements();

  auto tx_msg = this->f1c_gw.pop_tx_pdu();
  ASSERT_TRUE(tx_msg.has_value());
  ASSERT_TRUE(test_helpers::is_valid_f1ap_e_cid_measurement_initiation_failure(tx_msg.value()));

  // The terminated measurement sends no reports.
  this->f1c_gw.clear_tx_pdus();
  tick_ms(test_nr_aoa_period_ms * 3);
  ASSERT_EQ(pop_nof_reports(), 0) << "The terminated measurement started reporting";
}
