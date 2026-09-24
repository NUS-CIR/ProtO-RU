// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_cu_test_helpers.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/f1ap/f1ap_test_message_validators.h"
#include "tests/test_doubles/f1ap/f1ap_test_messages.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/support/async/async_test_utils.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;
using namespace asn1::f1ap;

namespace {

const std::chrono::milliseconds e_cid_procedure_timeout{100};

constexpr uint16_t lmf_ue_meas_id = 1;
constexpr uint16_t ran_ue_meas_id = 2;

/// Azimuth Angle of Arrival in units of 0.1 degrees, as per TS 38.473, Section 9.3.1.157.
constexpr uint16_t azimuth_aoa = 1800;
constexpr uint16_t zenith_aoa  = 567;

class f1ap_cu_e_cid_measurement_initiation_test : public f1ap_cu_test
{
protected:
  f1ap_cu_e_cid_measurement_initiation_test() :
    f1ap_cu_test(f1ap_configuration{.proc_timeout = e_cid_procedure_timeout})
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");
  }

  e_cid_measurement_request_t make_request(cu_cp_ue_index_t         ue_index,
                                           report_characteristics_t report = report_characteristics_t::on_demand)
  {
    e_cid_measurement_request_t req;
    req.ue_index               = ue_index;
    req.lmf_ue_meas_id         = uint_to_lmf_ue_meas_id(lmf_ue_meas_id);
    req.ran_ue_meas_id         = uint_to_ran_ue_meas_id(ran_ue_meas_id);
    req.report_characteristics = report;
    req.meas_quantities        = {e_cid_meas_quantities_item_t::nr_angle_of_arrival};
    return req;
  }

  void start_procedure(const e_cid_measurement_request_t& req)
  {
    t = f1ap->handle_e_cid_measurement_request(req);
    t_launcher.emplace(t);
  }

  bool was_request_sent() const
  {
    if (f1ap_pdu_notifier.last_f1ap_msg.pdu.type().value != f1ap_pdu_c::types::init_msg) {
      return false;
    }
    return f1ap_pdu_notifier.last_f1ap_msg.pdu.init_msg().value.type().value ==
           f1ap_elem_procs_o::init_msg_c::types_opts::e_c_id_meas_initiation_request;
  }

  const e_c_id_meas_initiation_request_s& sent_request() const
  {
    return f1ap_pdu_notifier.last_f1ap_msg.pdu.init_msg().value.e_c_id_meas_initiation_request();
  }

  /// Returns the gNB-CU UE F1AP ID the F1AP allocated, taken from the request it just sent.
  gnb_cu_ue_f1ap_id_t sent_cu_ue_id() const { return int_to_gnb_cu_ue_f1ap_id(sent_request()->gnb_cu_ue_f1ap_id); }

  async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>                        t;
  std::optional<lazy_task_launcher<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>> t_launcher;
};

} // namespace

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_request_is_sent_then_procedure_is_pending)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));

  start_procedure(make_request(ue.ue_index));

  ASSERT_TRUE(was_request_sent());
  ASSERT_TRUE(test_helpers::is_valid_e_cid_measurement_initiation_request(f1ap_pdu_notifier.last_f1ap_msg));
  ASSERT_EQ(sent_request()->lmf_ue_meas_id, lmf_ue_meas_id);
  ASSERT_EQ(sent_request()->ran_ue_meas_id, ran_ue_meas_id);
  ASSERT_EQ(sent_request()->e_c_id_report_characteristics.value, e_c_id_report_characteristics_opts::on_demand);
  ASSERT_FALSE(t.ready());
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_aoa_is_requested_then_request_carries_the_nr_aoa_quantity)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));

  start_procedure(make_request(ue.ue_index));

  ASSERT_EQ(sent_request()->e_c_id_meas_quantities.size(), 1);
  ASSERT_EQ(sent_request()->e_c_id_meas_quantities[0]->e_c_id_meas_quantities_item().e_c_id_meas_quantities_value.value,
            e_c_id_meas_quantities_value_opts::angle_of_arrival_nr);
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_ue_does_not_exist_then_request_is_rejected)
{
  start_procedure(make_request(uint_to_ue_index(0)));

  ASSERT_FALSE(was_request_sent());
  ASSERT_TRUE(t.ready());
  ASSERT_FALSE(t.get().has_value());
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_response_is_received_then_procedure_returns_the_aoa)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));

  f1ap->handle_message(test_helpers::generate_e_cid_measurement_initiation_response(
      *ue.du_ue_id, sent_cu_ue_id(), lmf_ue_meas_id, ran_ue_meas_id, azimuth_aoa));

  ASSERT_TRUE(t.ready());
  const auto& res = t.get();
  ASSERT_TRUE(res.has_value());
  ASSERT_EQ(res.value().lmf_ue_meas_id, uint_to_lmf_ue_meas_id(lmf_ue_meas_id));
  ASSERT_EQ(res.value().ran_ue_meas_id, uint_to_ran_ue_meas_id(ran_ue_meas_id));
  ASSERT_TRUE(res.value().e_cid_meas_result.has_value());
  ASSERT_EQ(res.value().e_cid_meas_result->measured_results.size(), 1);

  const ul_angle_of_arrival_t& aoa = res.value().e_cid_meas_result->measured_results[0];
  ASSERT_EQ(aoa.azimuth_aoa, azimuth_aoa);
  ASSERT_FALSE(aoa.zenith_aoa.has_value());
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_response_carries_zenith_aoa_then_it_is_returned)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));

  f1ap->handle_message(test_helpers::generate_e_cid_measurement_initiation_response(
      *ue.du_ue_id, sent_cu_ue_id(), lmf_ue_meas_id, ran_ue_meas_id, azimuth_aoa, zenith_aoa));

  ASSERT_TRUE(t.ready());
  const auto& res = t.get();
  ASSERT_TRUE(res.has_value());

  const ul_angle_of_arrival_t& aoa = res.value().e_cid_meas_result->measured_results[0];
  ASSERT_EQ(aoa.zenith_aoa, zenith_aoa);
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_response_carries_no_result_then_procedure_still_succeeds)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));

  f1ap->handle_message(test_helpers::generate_e_cid_measurement_initiation_response(
      *ue.du_ue_id, sent_cu_ue_id(), lmf_ue_meas_id, ran_ue_meas_id, std::nullopt));

  ASSERT_TRUE(t.ready());
  ASSERT_TRUE(t.get().has_value());
  ASSERT_FALSE(t.get().value().e_cid_meas_result.has_value());
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_failure_is_received_then_procedure_fails)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));

  f1ap->handle_message(test_helpers::generate_e_cid_measurement_initiation_failure(
      *ue.du_ue_id, sent_cu_ue_id(), lmf_ue_meas_id, ran_ue_meas_id));

  ASSERT_TRUE(t.ready());
  ASSERT_FALSE(t.get().has_value());
  ASSERT_EQ(t.get().error().lmf_ue_meas_id, uint_to_lmf_ue_meas_id(lmf_ue_meas_id));
  ASSERT_EQ(t.get().error().ran_ue_meas_id, uint_to_ran_ue_meas_id(ran_ue_meas_id));
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_du_does_not_respond_then_procedure_times_out)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));

  for (unsigned i = 0; i != e_cid_procedure_timeout.count(); ++i) {
    ASSERT_FALSE(t.ready());
    this->tick();
  }

  ASSERT_TRUE(t.ready());
  ASSERT_FALSE(t.get().has_value());
}

/// The transaction must be cancelled when the F1AP stops, e.g. on DU removal, so that no coroutine outlives the UE
/// context it holds.
TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_f1ap_is_stopped_then_pending_procedure_is_cancelled)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));
  ASSERT_FALSE(t.ready());

  async_task<void>         stop_task = f1ap->stop();
  lazy_task_launcher<void> stop_launcher(stop_task);

  ASSERT_TRUE(t.ready());
  ASSERT_FALSE(t.get().has_value());
}

/// The 60min periodicity applies only to an ng-eNB, as per TS 38.455, Section 9.1.1.1, so F1AP cannot encode it. The
/// request must be rejected rather than sent without the mandatory E-CID Measurement Periodicity IE.
TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_periodicity_is_not_encodable_then_request_is_not_sent)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));

  e_cid_measurement_request_t req = make_request(ue.ue_index, report_characteristics_t::periodic);
  req.meas_periodicity            = meas_periodicity_t::min60;
  start_procedure(req);

  ASSERT_FALSE(was_request_sent());
  ASSERT_TRUE(t.ready());
  ASSERT_FALSE(t.get().has_value());
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_periodic_request_is_sent_then_both_periodicity_ies_are_encoded)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));

  e_cid_measurement_request_t req = make_request(ue.ue_index, report_characteristics_t::periodic);
  req.meas_periodicity            = meas_periodicity_t::ms1024;
  req.meas_periodicity_nr_aoa     = meas_periodicity_nr_aoa_t::ms1280;
  start_procedure(req);

  ASSERT_TRUE(was_request_sent());
  ASSERT_EQ(sent_request()->e_c_id_report_characteristics.value, e_c_id_report_characteristics_opts::periodic);
  ASSERT_TRUE(sent_request()->e_c_id_meas_periodicity_present);
  ASSERT_EQ(sent_request()->e_c_id_meas_periodicity.value, meas_periodicity_opts::ms1024);
  ASSERT_TRUE(sent_request()->pos_meas_periodicity_nr_ao_a_present);
  ASSERT_EQ(sent_request()->pos_meas_periodicity_nr_ao_a.value, pos_meas_periodicity_nr_ao_a_opts::ms1280);
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test, when_e_cid_measurement_report_is_received_then_it_is_forwarded)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));

  f1ap->handle_message(test_helpers::generate_e_cid_measurement_report(
      *ue.du_ue_id, sent_cu_ue_id(), lmf_ue_meas_id, ran_ue_meas_id, azimuth_aoa));

  ASSERT_TRUE(du_processor_notifier.last_e_cid_measurement_report.has_value());
  const auto& report = du_processor_notifier.last_e_cid_measurement_report.value();
  ASSERT_EQ(report.ue_index, ue.ue_index);
  ASSERT_EQ(report.ran_ue_meas_id, uint_to_ran_ue_meas_id(ran_ue_meas_id));
  ASSERT_EQ(report.e_cid_meas_result.measured_results.size(), 1);

  ASSERT_EQ(report.e_cid_meas_result.measured_results[0].azimuth_aoa, azimuth_aoa);
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test,
       when_e_cid_measurement_report_for_unknown_ue_is_received_then_it_is_dropped)
{
  f1ap->handle_message(test_helpers::generate_e_cid_measurement_report(
      int_to_gnb_du_ue_f1ap_id(0), int_to_gnb_cu_ue_f1ap_id(0), lmf_ue_meas_id, ran_ue_meas_id, azimuth_aoa));

  ASSERT_FALSE(du_processor_notifier.last_e_cid_measurement_report.has_value());
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test,
       when_e_cid_measurement_failure_indication_is_received_then_it_is_forwarded)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));

  f1ap->handle_message(test_helpers::generate_e_cid_measurement_failure_indication(
      *ue.du_ue_id, sent_cu_ue_id(), lmf_ue_meas_id, ran_ue_meas_id));

  ASSERT_TRUE(du_processor_notifier.last_e_cid_measurement_failure_indication.has_value());
  ASSERT_EQ(du_processor_notifier.last_e_cid_measurement_failure_indication->ue_index, ue.ue_index);
}

TEST_F(f1ap_cu_e_cid_measurement_initiation_test,
       when_termination_command_is_sent_then_du_receives_both_measurement_ids)
{
  test_ue& ue = create_ue(int_to_gnb_du_ue_f1ap_id(0));
  start_procedure(make_request(ue.ue_index));

  f1ap->get_f1ap_nrppa_message_handler().handle_e_cid_measurement_termination_command(
      e_cid_measurement_termination_command_t{
          ue.ue_index, uint_to_lmf_ue_meas_id(lmf_ue_meas_id), uint_to_ran_ue_meas_id(ran_ue_meas_id)});

  ASSERT_EQ(f1ap_pdu_notifier.last_f1ap_msg.pdu.init_msg().value.type().value,
            f1ap_elem_procs_o::init_msg_c::types_opts::e_c_id_meas_termination_cmd);

  const auto& cmd = f1ap_pdu_notifier.last_f1ap_msg.pdu.init_msg().value.e_c_id_meas_termination_cmd();
  ASSERT_EQ(cmd->lmf_ue_meas_id, lmf_ue_meas_id);
  ASSERT_EQ(cmd->ran_ue_meas_id, ran_ue_meas_id);
}
