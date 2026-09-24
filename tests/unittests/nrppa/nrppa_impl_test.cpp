// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/nrppa/nrppa_impl.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/nrppa/nrppa_test_message_validators.h"
#include "tests/test_doubles/nrppa/nrppa_test_messages.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/nrppa/nrppa_ies.h"
#include "ocudu/asn1/nrppa/nrppa_pdu_contents.h"
#include "ocudu/support/async/async_no_op_task.h"
#include "ocudu/support/async/async_test_utils.h"
#include "ocudu/support/async/fifo_async_task_scheduler.h"
#include "ocudu/support/executors/manual_task_worker.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;

/// \brief Fake CU-CP notifier whose on_new_nrppa_ue() can be told to return nullptr, simulating a CU-CP UE that no
/// longer exists when a DL NRPPA message arrives for it.
class test_nrppa_cu_cp_notifier : public nrppa_cu_cp_notifier
{
public:
  nrppa_cu_cp_ue_notifier* on_new_nrppa_ue(cu_cp_ue_index_t ue_index) override { return ue_notifier; }

  void on_ul_nrppa_pdu(const byte_buffer&                                nrppa_pdu,
                       std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index) override
  {
    ul_pdu_sent       = true;
    last_ul_nrppa_pdu = nrppa_pdu.copy();
  }

  async_task<trp_information_cu_cp_response_t>
  on_trp_information_request(const trp_information_request_t& request) override
  {
    return launch_no_op_task(trp_information_cu_cp_response_t{});
  }

  nrppa_cu_cp_ue_notifier* ue_notifier = nullptr;
  bool                     ul_pdu_sent = false;
  byte_buffer              last_ul_nrppa_pdu;
};

/// Fake F1AP notifier that records which positioning request reached the DU.
class test_nrppa_f1ap_notifier : public nrppa_f1ap_notifier
{
public:
  async_task<expected<positioning_information_response_t, positioning_information_failure_t>>
  on_positioning_information_request(const positioning_information_request_t& request) override
  {
    positioning_information_requested = true;
    return launch_no_op_task(expected<positioning_information_response_t, positioning_information_failure_t>{
        positioning_information_response_t{}});
  }

  async_task<expected<positioning_activation_response_t, positioning_activation_failure_t>>
  on_positioning_activation_request(const positioning_activation_request_t& request) override
  {
    positioning_activation_requested = true;
    return launch_no_op_task(expected<positioning_activation_response_t, positioning_activation_failure_t>{
        positioning_activation_response_t{}});
  }

  async_task<expected<measurement_response_t, measurement_failure_t>>
  on_measurement_information_request(const measurement_request_t& request) override
  {
    return launch_no_op_task(expected<measurement_response_t, measurement_failure_t>{measurement_response_t{}});
  }

  async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>
  on_e_cid_measurement_request(const e_cid_measurement_request_t& request) override
  {
    e_cid_measurement_requested = true;
    last_e_cid_request          = request;
    if (!e_cid_response.has_value()) {
      return launch_no_op_task(expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>{
          make_unexpected(e_cid_measurement_failure_t{})});
    }
    return launch_no_op_task(
        expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>{e_cid_response.value()});
  }

  void on_e_cid_measurement_termination_command(const e_cid_measurement_termination_command_t& command) override
  {
    e_cid_measurement_terminated = true;
    last_termination_command     = command;
  }

  bool                                                   e_cid_measurement_terminated = false;
  std::optional<e_cid_measurement_termination_command_t> last_termination_command;

  bool                                        positioning_information_requested = false;
  bool                                        positioning_activation_requested  = false;
  bool                                        e_cid_measurement_requested       = false;
  std::optional<e_cid_measurement_request_t>  last_e_cid_request;
  std::optional<e_cid_measurement_response_t> e_cid_response;
};

/// Fake CU-CP UE notifier that runs the scheduled task in line, so that the UE-associated procedures execute within
/// the test.
class test_nrppa_cu_cp_ue_notifier : public nrppa_cu_cp_ue_notifier
{
public:
  cu_cp_ue_index_t get_ue_index() const override { return ue_index; }
  cu_cp_du_index_t get_du_index() const override { return du_index; }

  std::optional<nr_cell_global_id_t> get_serving_cell_id() const override { return serving_cell_id; }

  std::optional<cell_measurement_positioning_info>& on_measurement_results_required() override { return meas_results; }

  bool schedule_async_task(async_task<void> task) override
  {
    pending_task = std::move(task);
    launcher.emplace(pending_task);
    return true;
  }

  cu_cp_ue_index_t                                 ue_index = uint_to_ue_index(0);
  cu_cp_du_index_t                                 du_index = uint_to_cu_cp_du_index(0);
  std::optional<nr_cell_global_id_t>               serving_cell_id;
  std::optional<cell_measurement_positioning_info> meas_results;
  async_task<void>                                 pending_task;
  std::optional<lazy_task_launcher<void>>          launcher;
};

static asn1::nrppa::nr_ppa_pdu_c unpack_nrppa_pdu(const byte_buffer& pdu)
{
  asn1::nrppa::nr_ppa_pdu_c nrppa_pdu;
  asn1::cbit_ref            bref(pdu);
  report_fatal_error_if_not(nrppa_pdu.unpack(bref) == asn1::OCUDUASN_SUCCESS, "Failed to unpack NRPPa-PDU");
  return nrppa_pdu;
}

/// Fixture class for the NRPPA implementation.
class nrppa_impl_test : public ::testing::Test
{
protected:
  nrppa_impl_test()
  {
    logger.set_level(ocudulog::basic_levels::debug);
    ocudulog::init();
  }

  ~nrppa_impl_test()
  {
    // Flush logger after each test.
    ocudulog::flush();
  }

  ocudulog::basic_logger&              logger = ocudulog::fetch_basic_logger("NRPPA");
  timer_manager                        timer_mng;
  manual_task_worker                   ctrl_worker{128};
  fifo_async_task_scheduler            task_sched{32};
  test_nrppa_cu_cp_notifier            cu_cp_notifier;
  std::vector<supported_tracking_area> supported_tas{
      supported_tracking_area{tac_t{7}, {plmn_item{plmn_identity::test_value(), {}}}}};
  nrppa_impl nrppa{supported_tas, cu_cp_notifier, task_sched, timer_mng, ctrl_worker};
  /// Builds a gNB-DU E-CID measurement response carrying one UL Angle of Arrival.
  static e_cid_measurement_response_t make_du_response(uint16_t azimuth)
  {
    e_cid_measurement_response_t resp;
    resp.lmf_ue_meas_id = uint_to_lmf_ue_meas_id(1);
    resp.ran_ue_meas_id = uint_to_ran_ue_meas_id(1);
    e_cid_measurement_result_t result;
    result.measured_results.push_back(ul_angle_of_arrival_t{azimuth, std::nullopt, std::nullopt});
    resp.e_cid_meas_result = result;
    return resp;
  }

  /// Builds a gNB-DU E-CID measurement report carrying one UL Angle of Arrival.
  e_cid_measurement_report_t make_du_report(uint16_t azimuth) const
  {
    e_cid_measurement_report_t report;
    report.ue_index       = ue_index;
    report.lmf_ue_meas_id = uint_to_lmf_ue_meas_id(1);
    report.ran_ue_meas_id = uint_to_ran_ue_meas_id(1);
    report.e_cid_meas_result.measured_results.push_back(ul_angle_of_arrival_t{azimuth, std::nullopt, std::nullopt});
    return report;
  }

  /// Brings the UE into a state where the gNB-DU runs a periodic NR Angle of Arrival measurement.
  void start_periodic_aoa_measurement()
  {
    cu_cp_notifier.ue_notifier   = &ue_notifier;
    ue_notifier.serving_cell_id  = serving_cell_id;
    f1ap_notifier.e_cid_response = e_cid_measurement_response_t{};
    nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);

    nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
        generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_aoa(uint_to_lmf_ue_meas_id(1)),
        std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});
  }

  /// Azimuth Angle of Arrival in units of 0.1 degrees, as per TS 38.473, Section 9.3.1.157.
  static constexpr uint16_t azimuth_aoa = 1800;

  cu_cp_ue_index_t             ue_index = uint_to_ue_index(0);
  const nr_cell_global_id_t    serving_cell_id{plmn_identity::test_value(), nr_cell_identity::create(0x19b0).value()};
  test_nrppa_cu_cp_ue_notifier ue_notifier;
  test_nrppa_f1ap_notifier     f1ap_notifier;
};

TEST_F(nrppa_impl_test, when_malformed_pdu_is_received_then_it_is_dropped)
{
  byte_buffer garbage_pdu = make_byte_buffer("ffffffffff").value();

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(garbage_pdu,
                                                         std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_FALSE(cu_cp_notifier.ul_pdu_sent);
}

TEST_F(nrppa_impl_test, when_positioning_information_request_for_unknown_ue_is_received_then_failure_is_sent)
{
  cu_cp_notifier.ue_notifier = nullptr;

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(generate_valid_positioning_information_request(),
                                                         std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(cu_cp_notifier.ul_pdu_sent);
  ASSERT_TRUE(
      test_helpers::is_valid_nrppa_positioning_information_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

TEST_F(nrppa_impl_test, when_positioning_activation_request_for_unknown_ue_is_received_then_failure_is_sent)
{
  cu_cp_notifier.ue_notifier = nullptr;

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(generate_valid_positioning_activation_request(),
                                                         std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(cu_cp_notifier.ul_pdu_sent);
  ASSERT_TRUE(
      test_helpers::is_valid_nrppa_positioning_activation_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

TEST_F(nrppa_impl_test, when_the_du_serving_the_ue_has_no_context_then_positioning_information_request_is_rejected)
{
  cu_cp_notifier.ue_notifier = &ue_notifier;

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(generate_valid_positioning_information_request(),
                                                         std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(cu_cp_notifier.ul_pdu_sent);
  ASSERT_TRUE(
      test_helpers::is_valid_nrppa_positioning_information_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

TEST_F(nrppa_impl_test, when_the_du_serving_the_ue_has_no_context_then_positioning_activation_request_is_rejected)
{
  cu_cp_notifier.ue_notifier = &ue_notifier;

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(generate_valid_positioning_activation_request(),
                                                         std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(cu_cp_notifier.ul_pdu_sent);
  ASSERT_TRUE(
      test_helpers::is_valid_nrppa_positioning_activation_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

TEST_F(nrppa_impl_test, when_the_du_is_registered_then_positioning_information_request_reaches_the_du)
{
  cu_cp_notifier.ue_notifier = &ue_notifier;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(generate_valid_positioning_information_request(),
                                                         std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(f1ap_notifier.positioning_information_requested);
}

TEST_F(nrppa_impl_test, when_the_du_is_removed_then_positioning_information_request_is_rejected_again)
{
  cu_cp_notifier.ue_notifier = &ue_notifier;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);
  nrppa.get_nrppa_du_context_handler().handle_du_removal(ue_notifier.get_du_index());

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(generate_valid_positioning_information_request(),
                                                         std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_FALSE(f1ap_notifier.positioning_information_requested);
  ASSERT_TRUE(
      test_helpers::is_valid_nrppa_positioning_information_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

TEST_F(nrppa_impl_test, when_e_cid_meas_initiation_request_for_unknown_ue_is_received_then_failure_is_sent)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier = nullptr;

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request(uint_to_lmf_ue_meas_id(1)),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(cu_cp_notifier.ul_pdu_sent);
  ASSERT_TRUE(test_helpers::is_valid_e_cid_meas_initiation_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

TEST_F(nrppa_impl_test, when_aoa_is_requested_on_demand_then_f1ap_e_cid_request_is_sent)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);
  f1ap_notifier.e_cid_response = make_du_response(azimuth_aoa);

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request(
          uint_to_lmf_ue_meas_id(1), {{nrppa_meas_quantities_item{nrppa_meas_quantities_value::angle_of_arrival_nr}}}),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(f1ap_notifier.e_cid_measurement_requested);
  ASSERT_EQ(f1ap_notifier.last_e_cid_request->meas_quantities.size(), 1);
  ASSERT_EQ(f1ap_notifier.last_e_cid_request->meas_quantities[0], e_cid_meas_quantities_item_t::nr_angle_of_arrival);
}

TEST_F(nrppa_impl_test, when_f1ap_returns_aoa_then_e_cid_initiation_response_carries_it)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);
  f1ap_notifier.e_cid_response = make_du_response(azimuth_aoa);

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request(
          uint_to_lmf_ue_meas_id(1), {{nrppa_meas_quantities_item{nrppa_meas_quantities_value::angle_of_arrival_nr}}}),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(cu_cp_notifier.ul_pdu_sent);
  asn1::nrppa::nr_ppa_pdu_c pdu = unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu);
  ASSERT_EQ(pdu.type().value, asn1::nrppa::nr_ppa_pdu_c::types_opts::successful_outcome);

  const auto& resp = pdu.successful_outcome().value.e_c_id_meas_initiation_resp();
  ASSERT_TRUE(resp->e_c_id_meas_result_present);
  ASSERT_EQ(resp->e_c_id_meas_result.measured_results.size(), 1);

  // NR Angle of Arrival is carried in the extension of the Measured Results Value IE.
  const auto& result_value = resp->e_c_id_meas_result.measured_results[0];
  ASSERT_EQ(result_value.type().value, asn1::nrppa::measured_results_value_c::types_opts::choice_ext);
  ASSERT_EQ(result_value.choice_ext()->type().value,
            asn1::nrppa::measured_results_value_ext_ie_o::value_c::types_opts::angle_of_arrival_nr);
  ASSERT_EQ(result_value.choice_ext()->angle_of_arrival_nr().azimuth_ao_a, azimuth_aoa);
}

TEST_F(nrppa_impl_test, when_f1ap_e_cid_request_fails_then_e_cid_initiation_failure_is_sent)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);
  // Leave f1ap_notifier.e_cid_response unset, so the DU rejects the request.

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request(
          uint_to_lmf_ue_meas_id(1), {{nrppa_meas_quantities_item{nrppa_meas_quantities_value::angle_of_arrival_nr}}}),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(f1ap_notifier.e_cid_measurement_requested);
  ASSERT_TRUE(test_helpers::is_valid_e_cid_meas_initiation_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

TEST_F(nrppa_impl_test, when_aoa_is_requested_but_the_du_has_no_context_then_request_is_rejected)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request(
          uint_to_lmf_ue_meas_id(1), {{nrppa_meas_quantities_item{nrppa_meas_quantities_value::angle_of_arrival_nr}}}),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_FALSE(f1ap_notifier.e_cid_measurement_requested);
  ASSERT_TRUE(test_helpers::is_valid_e_cid_meas_initiation_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

/// The UE has reported no RRC measurements, so the serving cell is taken from the cell the UE camps on.
TEST_F(nrppa_impl_test, when_ue_has_no_rrc_measurements_then_aoa_result_carries_the_serving_cell_id)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  ASSERT_FALSE(ue_notifier.meas_results.has_value());
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);
  f1ap_notifier.e_cid_response = make_du_response(azimuth_aoa);

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request(
          uint_to_lmf_ue_meas_id(1), {{nrppa_meas_quantities_item{nrppa_meas_quantities_value::angle_of_arrival_nr}}}),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  asn1::nrppa::nr_ppa_pdu_c pdu = unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu);
  ASSERT_EQ(pdu.type().value, asn1::nrppa::nr_ppa_pdu_c::types_opts::successful_outcome);

  const auto& resp = pdu.successful_outcome().value.e_c_id_meas_initiation_resp();
  ASSERT_EQ(resp->e_c_id_meas_result.serving_cell_id.ng_ran_cell.nr_cell_id().to_number(), serving_cell_id.nci.value());
}

/// Periodic NR Angle of Arrival is paced by the gNB-DU, which reports through the E-CID Measurement Report procedure.
TEST_F(nrppa_impl_test, when_periodic_aoa_is_requested_then_f1ap_request_carries_periodic_report_characteristics)
{
  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);
  f1ap_notifier.e_cid_response = e_cid_measurement_response_t{};

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_aoa(uint_to_lmf_ue_meas_id(1)),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(f1ap_notifier.e_cid_measurement_requested);
  ASSERT_EQ(f1ap_notifier.last_e_cid_request->report_characteristics, report_characteristics_t::periodic);
}

TEST_F(nrppa_impl_test, when_du_reports_aoa_then_nrppa_e_cid_measurement_report_is_sent)
{
  start_periodic_aoa_measurement();

  nrppa.get_nrppa_e_cid_report_handler().handle_e_cid_measurement_report(make_du_report(azimuth_aoa));

  asn1::nrppa::nr_ppa_pdu_c pdu = unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu);
  ASSERT_EQ(pdu.type().value, asn1::nrppa::nr_ppa_pdu_c::types_opts::init_msg);
  ASSERT_EQ(pdu.init_msg().value.type().value,
            asn1::nrppa::nr_ppa_elem_procs_o::init_msg_c::types_opts::e_c_id_meas_report);

  const auto& report       = pdu.init_msg().value.e_c_id_meas_report();
  const auto& result_value = report->e_c_id_meas_result.measured_results[0];
  ASSERT_EQ(result_value.choice_ext()->angle_of_arrival_nr().azimuth_ao_a, azimuth_aoa);
}

TEST_F(nrppa_impl_test, when_du_report_has_unknown_ran_ue_meas_id_then_it_is_dropped)
{
  start_periodic_aoa_measurement();
  cu_cp_notifier.ul_pdu_sent = false;

  e_cid_measurement_report_t report = make_du_report(azimuth_aoa);
  report.ran_ue_meas_id             = uint_to_ran_ue_meas_id(99);
  nrppa.get_nrppa_e_cid_report_handler().handle_e_cid_measurement_report(report);

  ASSERT_FALSE(cu_cp_notifier.ul_pdu_sent);
}

TEST_F(nrppa_impl_test, when_failure_indication_is_received_then_nrppa_failure_indication_is_sent)
{
  start_periodic_aoa_measurement();

  nrppa.get_nrppa_e_cid_report_handler().handle_e_cid_measurement_failure_indication(
      e_cid_measurement_failure_indication_t{
          ue_index, uint_to_lmf_ue_meas_id(1), uint_to_ran_ue_meas_id(1), f1ap_cause_t{cause_misc_t::unspecified}});

  asn1::nrppa::nr_ppa_pdu_c pdu = unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu);
  ASSERT_EQ(pdu.init_msg().value.type().value,
            asn1::nrppa::nr_ppa_elem_procs_o::init_msg_c::types_opts::e_c_id_meas_fail_ind);
}

/// The gNB-DU has terminated the measurement itself, so the CU-CP must not send it a termination command.
TEST_F(nrppa_impl_test, when_failure_indication_is_received_then_no_termination_command_is_sent)
{
  start_periodic_aoa_measurement();

  nrppa.get_nrppa_e_cid_report_handler().handle_e_cid_measurement_failure_indication(
      e_cid_measurement_failure_indication_t{
          ue_index, uint_to_lmf_ue_meas_id(1), uint_to_ran_ue_meas_id(1), f1ap_cause_t{cause_misc_t::unspecified}});
  nrppa.get_nrppa_ue_context_removal_handler().remove_ue_context(ue_index);

  ASSERT_FALSE(f1ap_notifier.e_cid_measurement_terminated);
}

TEST_F(nrppa_impl_test, when_ue_is_removed_while_aoa_measurement_is_active_then_f1ap_termination_command_is_sent)
{
  start_periodic_aoa_measurement();

  nrppa.get_nrppa_ue_context_removal_handler().remove_ue_context(ue_index);

  ASSERT_TRUE(f1ap_notifier.e_cid_measurement_terminated);
  ASSERT_EQ(f1ap_notifier.last_termination_command->ue_index, ue_index);
}

/// Only the RRC-sourced quantities were requested, so the gNB-DU runs no measurement to terminate.
TEST_F(nrppa_impl_test, when_only_rrc_quantities_are_requested_then_no_f1ap_termination_command_is_sent)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_reports(uint_to_lmf_ue_meas_id(1)),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  nrppa.get_nrppa_ue_context_removal_handler().remove_ue_context(ue_index);

  ASSERT_FALSE(f1ap_notifier.e_cid_measurement_terminated);
}

/// A UE that has not reported any measurement yet may report one later, so the LMF is told to retry rather than that
/// the quantity is unsupported.
TEST_F(nrppa_impl_test, when_no_measurements_are_available_then_failure_cause_invites_a_retry)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request(uint_to_lmf_ue_meas_id(1)),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  asn1::nrppa::nr_ppa_pdu_c pdu = unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu);
  ASSERT_EQ(pdu.type().value, asn1::nrppa::nr_ppa_pdu_c::types_opts::unsuccessful_outcome);

  const auto& fail = pdu.unsuccessful_outcome().value.e_c_id_meas_initiation_fail();
  ASSERT_EQ(fail->cause.type().value, asn1::nrppa::cause_c::types_opts::radio_network);
  ASSERT_EQ(fail->cause.radio_network().value,
            asn1::nrppa::cause_radio_network_opts::requested_item_temporarily_not_available);
}

/// A periodic request the gNB-DU rejects must leave no reporting behind, or the LMF keeps receiving reports for a
/// measurement it was told had failed.
TEST_F(nrppa_impl_test, when_du_rejects_periodic_aoa_then_no_measurement_is_left_running)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-POS-16-1");

  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);
  // Leave f1ap_notifier.e_cid_response unset, so the gNB-DU rejects the request.

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_aoa(uint_to_lmf_ue_meas_id(1)),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_TRUE(test_helpers::is_valid_e_cid_meas_initiation_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));

  // No periodic measurement is configured, so a later request is accepted instead of being refused as a duplicate.
  f1ap_notifier.e_cid_response = e_cid_measurement_response_t{};
  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_aoa(uint_to_lmf_ue_meas_id(1)),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  asn1::nrppa::nr_ppa_pdu_c pdu = unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu);
  ASSERT_EQ(pdu.type().value, asn1::nrppa::nr_ppa_pdu_c::types_opts::successful_outcome);
}

/// A gNB-DU that declines the NR Angle of Arrival must not stop the quantities that the gNB-CU reports itself.
TEST_F(nrppa_impl_test, when_du_rejects_periodic_aoa_with_rrc_quantities_then_the_other_quantities_are_reported)
{
  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);
  // Leave f1ap_notifier.e_cid_response unset, so the gNB-DU rejects the request.

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_aoa(
          uint_to_lmf_ue_meas_id(1), {asn1::nrppa::meas_quantities_value_opts::ss_rsrp}),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  asn1::nrppa::nr_ppa_pdu_c pdu = unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu);
  ASSERT_EQ(pdu.type().value, asn1::nrppa::nr_ppa_pdu_c::types_opts::successful_outcome);
}

/// The NRPPa PDU is opaque to NGAP, so the AMF can carry a non-UE-associated procedure in a UE-associated NRPPa
/// transport, or vice versa. Such a PDU must be dropped rather than dispatched to a handler expecting the other index.
TEST_F(nrppa_impl_test, when_trp_information_request_is_received_ue_associated_then_it_is_dropped)
{
  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(generate_valid_trp_information_request(),
                                                         std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_FALSE(cu_cp_notifier.ul_pdu_sent);
}

TEST_F(nrppa_impl_test, when_measurement_request_is_received_ue_associated_then_it_is_dropped)
{
  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_measurement_request(uint_to_lmf_meas_id(1)),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_FALSE(cu_cp_notifier.ul_pdu_sent);
}

TEST_F(nrppa_impl_test, when_positioning_information_request_is_received_non_ue_associated_then_it_is_dropped)
{
  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_positioning_information_request(),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{cu_cp_amf_index_t::min});

  ASSERT_FALSE(cu_cp_notifier.ul_pdu_sent);
}

TEST_F(nrppa_impl_test, when_e_cid_measurement_initiation_request_is_received_non_ue_associated_then_it_is_dropped)
{
  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request(uint_to_lmf_ue_meas_id(1)),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{cu_cp_amf_index_t::min});

  ASSERT_FALSE(cu_cp_notifier.ul_pdu_sent);
}

/// The Measurement Periodicity NR-AoA IE is conditional-mandatory for a periodic NR Angle of Arrival request, as per
/// TS 38.455 section 9.1.1.1.
TEST_F(nrppa_impl_test, when_periodic_aoa_is_requested_without_its_periodicity_then_request_is_rejected)
{
  cu_cp_notifier.ue_notifier  = &ue_notifier;
  ue_notifier.serving_cell_id = serving_cell_id;
  nrppa.get_nrppa_du_context_handler().handle_du_addition(ue_notifier.get_du_index(), f1ap_notifier);

  nrppa.get_nrppa_message_handler().handle_new_nrppa_pdu(
      generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_reports(uint_to_lmf_ue_meas_id(1), true),
      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t>{ue_index});

  ASSERT_FALSE(f1ap_notifier.e_cid_measurement_requested);
  ASSERT_TRUE(test_helpers::is_valid_e_cid_meas_initiation_failure(unpack_nrppa_pdu(cu_cp_notifier.last_ul_nrppa_pdu)));
}

/// A report racing a termination must not reach the LMF, which has already been told the measurement ended.
TEST_F(nrppa_impl_test, when_no_measurement_is_running_then_du_report_is_dropped)
{
  start_periodic_aoa_measurement();

  nrppa.get_nrppa_e_cid_report_handler().handle_e_cid_measurement_failure_indication(
      e_cid_measurement_failure_indication_t{
          ue_index, uint_to_lmf_ue_meas_id(1), uint_to_ran_ue_meas_id(1), f1ap_cause_t{cause_misc_t::unspecified}});

  cu_cp_notifier.ul_pdu_sent = false;
  nrppa.get_nrppa_e_cid_report_handler().handle_e_cid_measurement_report(make_du_report(azimuth_aoa));

  ASSERT_FALSE(cu_cp_notifier.ul_pdu_sent);
}
