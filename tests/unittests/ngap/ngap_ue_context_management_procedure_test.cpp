// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ngap_test_helpers.h"
#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/ngap/ngap_pdu_contents.h"
#include "ocudu/ran/rnti.h"
#include "ocudu/support/async/async_test_utils.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;

class ngap_ue_context_management_procedure_test : public ngap_test
{
protected:
  cu_cp_ue_index_t start_procedure(rnti_t rnti = rnti_t::MIN_CRNTI)
  {
    cu_cp_ue_index_t ue_index = create_ue(rnti);

    // Inject DL NAS transport message from AMF.
    run_dl_nas_transport(ue_index);

    // Inject UL NAS transport message from RRC.
    run_ul_nas_transport(ue_index);

    return ue_index;
  }

  bool was_initial_context_setup_response_sent() const
  {
    return n2_gw.last_ngap_msgs.back().pdu.successful_outcome().value.type() ==
           asn1::ngap::ngap_elem_procs_o::successful_outcome_c::types_opts::init_context_setup_resp;
  }

  bool was_initial_context_setup_failure_sent() const
  {
    return n2_gw.last_ngap_msgs.back().pdu.unsuccessful_outcome().value.type() ==
           asn1::ngap::ngap_elem_procs_o::unsuccessful_outcome_c::types_opts::init_context_setup_fail;
  }

  bool was_pdu_session_resource_setup_successful() const
  {
    bool setup_present = n2_gw.last_ngap_msgs.back()
                             .pdu.successful_outcome()
                             .value.init_context_setup_resp()
                             ->pdu_session_res_setup_list_cxt_res_present;

    bool fail_present = !n2_gw.last_ngap_msgs.back()
                             .pdu.successful_outcome()
                             .value.init_context_setup_resp()
                             ->pdu_session_res_failed_to_setup_list_cxt_res_present;

    return setup_present && fail_present;
  }

  bool was_pdu_session_resource_setup_unsuccessful() const
  {
    return n2_gw.last_ngap_msgs.back()
        .pdu.unsuccessful_outcome()
        .value.init_context_setup_fail()
        ->pdu_session_res_failed_to_setup_list_cxt_fail_present;
  }

  bool was_ue_context_release_request_sent() const
  {
    if (n2_gw.last_ngap_msgs.back().pdu.type() == asn1::ngap::ngap_pdu_c::types_opts::nulltype) {
      return false;
    }
    return n2_gw.last_ngap_msgs.back().pdu.init_msg().value.type() ==
           asn1::ngap::ngap_elem_procs_o::init_msg_c::types_opts::ue_context_release_request;
  }

  bool was_ue_context_release_complete_sent() const
  {
    return n2_gw.last_ngap_msgs.back().pdu.successful_outcome().value.type() ==
           asn1::ngap::ngap_elem_procs_o::successful_outcome_c::types_opts::ue_context_release_complete;
  }

  bool was_ue_added() const { return ngap->get_nof_ues() == 1; }

  bool was_ue_removed() const { return ngap->get_nof_ues() == 0; }

  void clear_last_received_msg() { n2_gw.last_ngap_msgs.back() = {}; }

  bool was_ue_release_requested(const test_ue& ue) const { return cu_cp_notifier.last_command.ue_index == ue.ue_index; }

  bool was_error_indication_sent() const
  {
    return n2_gw.last_ngap_msgs.back().pdu.init_msg().value.type() ==
           asn1::ngap::ngap_elem_procs_o::init_msg_c::types_opts::error_ind;
  }

  bool was_rrc_inactive_transition_report_sent() const
  {
    return n2_gw.last_ngap_msgs.back().pdu.init_msg().value.type() ==
           asn1::ngap::ngap_elem_procs_o::init_msg_c::types_opts::rrc_inactive_transition_report;
  }

  bool was_ue_context_modification_response_sent() const
  {
    return n2_gw.last_ngap_msgs.back().pdu.successful_outcome().value.type() ==
           asn1::ngap::ngap_elem_procs_o::successful_outcome_c::types_opts::ue_context_mod_resp;
  }

  bool was_ue_context_modification_failure_sent() const
  {
    return n2_gw.last_ngap_msgs.back().pdu.unsuccessful_outcome().value.type() ==
           asn1::ngap::ngap_elem_procs_o::unsuccessful_outcome_c::types_opts::ue_context_mod_fail;
  }
};

///////////////////////////////////////////////////////////////////////////////
//                             Initial Context Setup
///////////////////////////////////////////////////////////////////////////////

/// Test Initial Context Setup Request.
TEST_F(ngap_ue_context_management_procedure_test, when_valid_initial_context_setup_request_received_then_response_send)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  // Check that AMF notifier was called with right type.
  ASSERT_TRUE(was_initial_context_setup_response_sent());

  ASSERT_TRUE(was_ue_added());
}

TEST_F(ngap_ue_context_management_procedure_test,
       when_initial_context_setup_request_is_received_then_allowed_nssai_is_stored)
{
  cu_cp_ue_index_t ue_index = this->start_procedure();
  auto&            ue       = test_ues.at(ue_index);

  ngap->handle_message(
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value()));

  std::vector<s_nssai_t> allowed_nssai = ngap->get_allowed_nssai(ue_index);
  ASSERT_EQ(allowed_nssai.size(), 1);
  ASSERT_EQ(allowed_nssai[0].sst.value(), 1);
  ASSERT_EQ(allowed_nssai[0].sd.value(), 0xdb2700);
}

/// Test Initial Context Setup Request with PDUSessionResourceSetupListCxtReq.
TEST_F(ngap_ue_context_management_procedure_test,
       when_initial_context_setup_request_with_pdu_session_received_then_response_send)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message_with_pdu_session(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  // Check that AMF notifier was called with right type.
  ASSERT_TRUE(was_initial_context_setup_response_sent());

  ASSERT_TRUE(was_ue_added());

  ASSERT_TRUE(was_pdu_session_resource_setup_successful());
}

/// Test Initial Context Setup Request with "updated" AMF UE ID.
TEST_F(ngap_ue_context_management_procedure_test,
       when_new_amf_ue_id_is_sent_in_initial_context_setup_request_received_then_id_is_updated)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Get "first" AMF UE ID received.
  amf_ue_id_t old_id = ue.amf_ue_id.value();

  // Randomly generate new ID assigned by core.
  amf_ue_id_t new_id = old_id;
  while (new_id == old_id) {
    new_id = uint_to_amf_ue_id(
        test_rng::uniform_int<uint64_t>(to_underlying(amf_ue_id_t::min), to_underlying(amf_ue_id_t::max)));
  }
  ASSERT_NE(old_id, new_id);

  // Inject Initial Context Setup Request with new ID
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(new_id, ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  // Check that AMF notifier was called with right type.
  ASSERT_TRUE(was_initial_context_setup_response_sent());

  ASSERT_TRUE(was_ue_added());
}

/// Test invalid Initial Context Setup Request.
TEST_F(ngap_ue_context_management_procedure_test, when_invalid_initial_context_setup_request_received_then_failure_sent)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_invalid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  // Check that AMF notifier was called with right type.
  ASSERT_TRUE(was_initial_context_setup_failure_sent());
}

/// Test Initial Context Setup Request with an invalid PLMN in the TAI list for RRC inactive.
TEST_F(ngap_ue_context_management_procedure_test,
       when_initial_context_setup_request_with_invalid_plmn_in_tai_list_for_inactive_received_then_failure_sent)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request = generate_valid_initial_context_setup_request_message(
      ue.amf_ue_id.value(),
      ue.ran_ue_id.value(),
      cu_cp_core_network_assist_info_for_inactive{.ue_id_idx_value       = 0x64c0,
                                                  .tai_list_for_inactive = {tai_t{plmn_identity::test_value(), 7}}});
  init_context_setup_request.pdu.init_msg()
      .value.init_context_setup_request()
      ->core_network_assist_info_for_inactive.tai_list_for_inactive[0]
      .tai.plmn_id.from_string("ffffff");
  ngap->handle_message(init_context_setup_request);

  // Check that an Initial Context Setup Failure with cause "semantic error" was sent (TS 38.413 section 10.4).
  ASSERT_TRUE(was_initial_context_setup_failure_sent());
  const auto& cause = n2_gw.last_ngap_msgs.back().pdu.unsuccessful_outcome().value.init_context_setup_fail()->cause;
  ASSERT_EQ(cause.type(), asn1::ngap::cause_c::types_opts::protocol);
  ASSERT_EQ(cause.protocol(), asn1::ngap::cause_protocol_opts::semantic_error);
}

/// Test that the PDU sessions of a rejected Initial Context Setup Request are counted as failed.
TEST_F(ngap_ue_context_management_procedure_test,
       when_initial_context_setup_request_with_pdu_session_is_rejected_then_pdu_session_counted_as_failed)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request with an invalid PLMN in the GUAMI.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message_with_pdu_session(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  init_context_setup_request.pdu.init_msg().value.init_context_setup_request()->guami.plmn_id.from_string("ffffff");
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_initial_context_setup_failure_sent());

  // Check that the requested PDU session is counted as failed with cause "semantic error".
  const ngap_info&             metrics_report = ngap->get_metrics_handler().handle_ngap_metrics_report_request();
  const pdu_session_metrics_t& pdu_session_metrics =
      metrics_report.metrics.pdu_session_metrics.at(s_nssai_t{slice_service_type{1}});
  ASSERT_EQ(pdu_session_metrics.nof_pdu_sessions_requested_to_setup, 1);
  ASSERT_EQ(pdu_session_metrics.nof_pdu_sessions_failed_to_setup.get_count(cause_protocol_t::semantic_error), 1);
}

/// Test invalid Initial Context Setup Request with PDUSessionResourceSetupListCxtReq.
TEST_F(ngap_ue_context_management_procedure_test,
       when_invalid_initial_context_setup_request_with_pdu_session_received_then_failure_sent)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request = generate_invalid_initial_context_setup_request_message_with_pdu_session(
      ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  // Check that AMF notifier was called with right type.
  ASSERT_TRUE(was_initial_context_setup_failure_sent());

  ASSERT_TRUE(was_pdu_session_resource_setup_unsuccessful());
}

/// Test when Initial Context Setup Request with inconsistent NGAP ID pair is received,
/// an error indication is sent.
TEST_F(ngap_ue_context_management_procedure_test,
       when_init_context_setup_has_inconsistent_id_pair_then_error_indication_is_sent)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index1 = this->start_procedure();
  cu_cp_ue_index_t ue_index2 = this->start_procedure(to_rnti(0x2));

  auto& ue1 = test_ues.at(ue_index1);
  auto& ue2 = test_ues.at(ue_index2);

  // Inject Initial Context Setup Request with inconsistent NGAP ID pair.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue1.amf_ue_id.value(), ue2.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  // Check that both UEs are released: per TS 38.413 section 10.6, when the received IDs exist in
  // context but belong to different UEs, both connections must be locally released.
  ASSERT_TRUE(was_ue_removed());

  // Check that error indication has been sent to AMF.
  ASSERT_TRUE(was_error_indication_sent());
  ASSERT_EQ(n2_gw.last_ngap_msgs.back().pdu.init_msg().value.error_ind()->cause.radio_network(),
            asn1::ngap::cause_radio_network_e::options::inconsistent_remote_ue_ngap_id);
}

/// Test RRC Inactive Transition Report.
TEST_F(ngap_ue_context_management_procedure_test,
       when_rrc_inactive_transition_report_transmission_is_not_requested_then_report_is_not_sent)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MOB-10");

  // Test preamble.
  cu_cp_ue_index_t ue_index = this->start_procedure();

  // Trigger RRC Inactive Transition Report transmission.
  ngap_rrc_inactive_transition_report report;
  report.ue_index                          = ue_index;
  report.rrc_state                         = ngap_rrc_inactive_transition_report::ngap_rrc_state::inactive;
  report.user_location_info.nr_cgi.plmn_id = plmn_identity::test_value();
  report.user_location_info.nr_cgi.nci     = nr_cell_identity::create(gnb_id_t{411, 22}, 0).value();
  report.user_location_info.tai.plmn_id    = plmn_identity::test_value();
  report.user_location_info.tai.tac        = 7;

  async_task<bool>         t = ngap->handle_rrc_inactive_transition_report_required(report);
  lazy_task_launcher<bool> t_launcher(t);

  // Status: should have succeeded already.
  ASSERT_TRUE(t.ready());

  // Procedure should not have succeeded.
  ASSERT_TRUE(t.get());
  ASSERT_FALSE(was_rrc_inactive_transition_report_sent());
}

/// Test Initial Context Setup Request with RRC inactive transition report request.
TEST_F(ngap_ue_context_management_procedure_test,
       when_rrc_inactive_transition_report_transmission_is_requested_then_report_is_sent)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MOB-10");

  // Test preamble.
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request with RRC Inactive Transition Report Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  init_context_setup_request.pdu.init_msg()
      .value.init_context_setup_request()
      ->rrc_inactive_transition_report_request_present = true;
  init_context_setup_request.pdu.init_msg().value.init_context_setup_request()->rrc_inactive_transition_report_request =
      asn1::ngap::rrc_inactive_transition_report_request_e::options::subsequent_state_transition_report;
  ngap->handle_message(init_context_setup_request);

  // Check that AMF notifier was called with right type.
  ASSERT_TRUE(was_initial_context_setup_response_sent());

  ASSERT_TRUE(was_ue_added());

  // Trigger RRC Inactive Transition Report transmission.
  ngap_rrc_inactive_transition_report report;
  report.ue_index                          = ue_index;
  report.rrc_state                         = ngap_rrc_inactive_transition_report::ngap_rrc_state::inactive;
  report.user_location_info.nr_cgi.plmn_id = plmn_identity::test_value();
  report.user_location_info.nr_cgi.nci     = nr_cell_identity::create(gnb_id_t{411, 22}, 0).value();
  report.user_location_info.tai.plmn_id    = plmn_identity::test_value();
  report.user_location_info.tai.tac        = 7;

  async_task<bool>         t = ngap->handle_rrc_inactive_transition_report_required(report);
  lazy_task_launcher<bool> t_launcher(t);

  // Status: should have succeeded already.
  ASSERT_TRUE(t.ready());

  // Procedure should have succeeded.
  ASSERT_TRUE(t.get());
  ASSERT_TRUE(was_rrc_inactive_transition_report_sent());
}

///////////////////////////////////////////////////////////////////////////////
//                             UE Context Release
///////////////////////////////////////////////////////////////////////////////

/// Test successful UE context release.
TEST_F(
    ngap_ue_context_management_procedure_test,
    when_ue_context_release_command_as_first_message_from_core_received_then_ue_is_released_and_release_complete_is_sent)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index = create_ue();
  auto&            ue       = test_ues.at(ue_index);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Release Command.
  ngap_message ue_context_release_cmd =
      generate_valid_ue_context_release_command_with_ue_ngap_id_pair(amf_ue_id_t(1), ue.ran_ue_id.value());
  ngap->handle_message(ue_context_release_cmd);

  ASSERT_TRUE(was_ue_context_release_complete_sent());
  ASSERT_TRUE(was_ue_removed());
}

/// Test UE context release with a Cause that holds a choice extension.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_release_command_with_cause_choice_extension_received_then_ue_is_released)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index = create_ue();
  auto&            ue       = test_ues.at(ue_index);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Release Command with a Cause choice extension.
  ngap_message ue_context_release_cmd =
      generate_valid_ue_context_release_command_with_ue_ngap_id_pair(amf_ue_id_t(1), ue.ran_ue_id.value());
  ue_context_release_cmd.pdu.init_msg().value.ue_context_release_cmd()->cause.set_choice_exts();
  ngap->handle_message(ue_context_release_cmd);

  ASSERT_TRUE(was_ue_context_release_complete_sent());
  ASSERT_TRUE(was_ue_removed());
}

/// Test successful UE context release
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_release_command_with_amf_ue_ngap_id_received_then_ue_is_released_and_release_complete_is_sent)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Release Command.
  ngap_message ue_context_release_cmd =
      generate_valid_ue_context_release_command_with_amf_ue_ngap_id(ue.amf_ue_id.value());
  ngap->handle_message(ue_context_release_cmd);

  ASSERT_TRUE(was_ue_context_release_complete_sent());
  ASSERT_TRUE(was_ue_removed());
}

/// Test Release Command after Initial UE message is received.
TEST_F(ngap_ue_context_management_procedure_test,
       when_release_command_after_initial_ue_message_is_received_then_ue_is_released)
{
  ASSERT_EQ(ngap->get_nof_ues(), 0);

  // Test preamble.
  cu_cp_ue_index_t ue_index = create_ue();

  auto& ue = test_ues.at(ue_index);

  // Inject DL NAS transport message from AMF.
  run_dl_nas_transport(ue_index);

  // Inject UE Context Release Command.
  ngap_message ue_context_release_cmd =
      generate_valid_ue_context_release_command_with_amf_ue_ngap_id(ue.amf_ue_id.value());
  ngap->handle_message(ue_context_release_cmd);

  ASSERT_TRUE(was_ue_context_release_complete_sent());
  ASSERT_TRUE(was_ue_removed());
}

/// Test successful UE context release.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_release_command_with_ue_ngap_id_pair_received_then_ue_is_released_and_release_complete_is_sent)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Release Command.
  ngap_message ue_context_release_cmd =
      generate_valid_ue_context_release_command_with_ue_ngap_id_pair(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(ue_context_release_cmd);

  ASSERT_TRUE(was_ue_context_release_complete_sent());
  ASSERT_TRUE(was_ue_removed());
}

/// Test UE context release for unknown UE.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_release_command_for_unknown_ue_received_then_ue_is_not_released_and_release_complete_is_not_sent)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Release Command for unknown UE.
  amf_ue_id_t unknown_ue_id = uint_to_amf_ue_id(to_underlying(ue.amf_ue_id.value()) + 1);

  ngap_message ue_context_release_cmd = generate_valid_ue_context_release_command_with_amf_ue_ngap_id(unknown_ue_id);
  ngap->handle_message(ue_context_release_cmd);

  ASSERT_FALSE(was_ue_context_release_complete_sent());
  ASSERT_FALSE(was_ue_removed());
}

/// Regression test: the UE-NGAP-IDs choice can legally decode into its extension alternative
/// (choice_exts), which carries neither an AMF UE NGAP ID nor a UE NGAP ID pair. The handler must
/// reject the message instead of looking up a UE context with an invalid/default-constructed AMF UE
/// ID.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_release_command_has_choice_exts_ue_ngap_ids_then_message_is_dropped)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Release Command with an unsupported UE-NGAP-IDs choice.
  ngap_message ue_context_release_cmd = generate_ue_context_release_command_with_choice_exts_ue_ngap_ids();
  ngap->handle_message(ue_context_release_cmd);

  ASSERT_FALSE(was_ue_context_release_complete_sent());
  ASSERT_FALSE(was_ue_removed());
}

/// Test UE context release request for UE that hasn't received an AMF UE ID yet.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_release_request_is_received_but_no_amf_ue_ngap_id_is_set_then_request_is_ignored)
{
  // Test preamble - Only create UE but do not have DL traffic from the AMF.
  cu_cp_ue_index_t ue_index = create_ue();

  // Trigger UE context release request.
  cu_cp_ue_context_release_request release_request;
  release_request.ue_index = ue_index;

  async_task<bool>         t = ngap->handle_ue_context_release_request(release_request);
  lazy_task_launcher<bool> t_launcher(t);

  // Status: should have failed already, as there is no UE.
  ASSERT_TRUE(t.ready());

  // Procedure should have failed.
  ASSERT_FALSE(t.get());
  ASSERT_FALSE(was_ue_context_release_request_sent());
}

/// Test UE context release request is not sent multiple times for same UE.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_release_request_is_received_multiple_times_ngap_message_is_not_sent_more_than_once)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_ue_added());

  // Trigger UE context release request.
  cu_cp_ue_context_release_request release_request;
  release_request.ue_index = ue_index;

  async_task<bool>         t = ngap->handle_ue_context_release_request(release_request);
  lazy_task_launcher<bool> t_launcher(t);

  // Status: Should have succeeded already.
  ASSERT_TRUE(t.ready());

  // Procedure should have succeeded.
  ASSERT_TRUE(t.get());
  ASSERT_TRUE(was_ue_context_release_request_sent());

  // Trigger 2nd UE context release request.
  clear_last_received_msg();
  async_task<bool>         t2 = ngap->handle_ue_context_release_request(release_request);
  lazy_task_launcher<bool> t_launcher2(t2);

  // Status: should have succeeded already, as a release request is already pending.
  ASSERT_TRUE(t2.ready());

  // Procedure should have succeeded.
  ASSERT_TRUE(t2.get());
  ASSERT_FALSE(was_ue_context_release_request_sent());
}

/// Test when UE Context Release Command with inconsistent NGAP ID pair is received,
/// an error indication is sent.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_release_command_has_inconsistent_id_pair_then_error_indication_is_sent)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index1 = this->start_procedure();
  cu_cp_ue_index_t ue_index2 = this->start_procedure(to_rnti(0x2));

  auto& ue1 = test_ues.at(ue_index1);
  auto& ue2 = test_ues.at(ue_index2);

  // Inject UE Context Release Command with inconsistent NGAP ID pair.
  ngap_message ue_context_release_cmd =
      generate_valid_ue_context_release_command_with_ue_ngap_id_pair(ue1.amf_ue_id.value(), ue2.ran_ue_id.value());
  ngap->handle_message(ue_context_release_cmd);

  // Check that both UEs are released: per TS 38.413 section 10.6, when the received IDs exist in
  // context but belong to different UEs, both connections must be locally released.
  ASSERT_TRUE(was_ue_removed());

  // Check that error indication has been sent to AMF.
  ASSERT_TRUE(was_error_indication_sent());
  ASSERT_EQ(n2_gw.last_ngap_msgs.back().pdu.init_msg().value.error_ind()->cause.radio_network(),
            asn1::ngap::cause_radio_network_e::options::inconsistent_remote_ue_ngap_id);
}

/// Test when a UE Context Release Command contains a pair where the AMF UE NGAP ID does not
/// match the stored AMF UE NGAP ID for the given RAN UE NGAP ID, the UE is locally released
/// and an error indication is sent (TS 38.413 section 10.6).
TEST_F(
    ngap_ue_context_management_procedure_test,
    when_ue_context_release_command_has_mismatched_amf_ue_ngap_id_for_known_ran_ue_ngap_id_then_ue_is_released_and_error_indication_is_sent)
{
  // Test preamble: set up a UE with known AMF UE ID and RAN UE ID.
  cu_cp_ue_index_t ue_index = this->start_procedure();
  auto&            ue       = test_ues.at(ue_index);

  // Inject UE Context Release Command with the correct RAN UE NGAP ID but a different
  // AMF UE NGAP ID than the one stored for this UE.
  amf_ue_id_t  wrong_amf_ue_id = uint_to_amf_ue_id(to_underlying(ue.amf_ue_id.value()) + 1);
  ngap_message ue_context_release_cmd =
      generate_valid_ue_context_release_command_with_ue_ngap_id_pair(wrong_amf_ue_id, ue.ran_ue_id.value());
  ngap->handle_message(ue_context_release_cmd);

  // Check that local release of the UE has been requested.
  ASSERT_TRUE(was_ue_release_requested(ue));

  // Check that an error indication with cause inconsistent_remote_ue_ngap_id has been sent.
  ASSERT_TRUE(was_error_indication_sent());
  ASSERT_EQ(n2_gw.last_ngap_msgs.back().pdu.init_msg().value.error_ind()->cause.radio_network(),
            asn1::ngap::cause_radio_network_e::options::inconsistent_remote_ue_ngap_id);
}

/// Test when a UE Context Release Command carries an AMF UE NGAP ID that is known but maps to a
/// different RAN UE NGAP ID than the one in the message, and the received RAN UE NGAP ID is
/// unknown, the UE with the known AMF UE ID is locally released and an error indication is sent
/// (TS 38.413 section 10.6).
TEST_F(
    ngap_ue_context_management_procedure_test,
    when_ue_context_release_command_has_known_amf_ue_ngap_id_paired_with_unknown_ran_ue_ngap_id_then_ue_is_released_and_error_indication_is_sent)
{
  cu_cp_ue_index_t ue_index = this->start_procedure();
  auto&            ue       = test_ues.at(ue_index);

  // Inject a UE Context Release Command with ue's AMF UE NGAP ID paired with a RAN UE NGAP ID that
  // does not exist in any context.
  ran_ue_id_t  unknown_ran_ue_id = uint_to_ran_ue_id(to_underlying(ue.ran_ue_id.value()) + 1);
  ngap_message ue_context_release_cmd =
      generate_valid_ue_context_release_command_with_ue_ngap_id_pair(ue.amf_ue_id.value(), unknown_ran_ue_id);
  ngap->handle_message(ue_context_release_cmd);

  // The UE owning the known AMF UE ID must be released.
  ASSERT_TRUE(was_ue_release_requested(ue));

  // Check that an error indication with cause inconsistent_remote_ue_ngap_id has been sent.
  ASSERT_TRUE(was_error_indication_sent());
  ASSERT_EQ(n2_gw.last_ngap_msgs.back().pdu.init_msg().value.error_ind()->cause.radio_network(),
            asn1::ngap::cause_radio_network_e::options::inconsistent_remote_ue_ngap_id);
}

///////////////////////////////////////////////////////////////////////////////
//                             UE Context Modification
///////////////////////////////////////////////////////////////////////////////

/// Test successful UE Context Modification Request.
TEST_F(ngap_ue_context_management_procedure_test,
       when_valid_ue_context_modification_request_received_then_response_send)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Modification Request
  ngap_message ue_context_mod_request = generate_valid_ue_context_modification_request_message(
      ue.amf_ue_id.value(),
      ue.ran_ue_id.value(),
      aggregate_maximum_bit_rate_t{.dl = 100000, .ul = 100000},
      cu_cp_core_network_assist_info_for_inactive{.ue_id_idx_value = 0xc0},
      guami_t{.plmn = plmn_identity::test_value(), .amf_set_id = 1, .amf_pointer = 1, .amf_region_id = 1});
  ngap->handle_message(ue_context_mod_request);

  // Check that AMF notifier was called with right type
  ASSERT_TRUE(was_ue_context_modification_response_sent());
}

/// Test invalid UE Context Modification Request.
TEST_F(ngap_ue_context_management_procedure_test,
       when_invalid_ue_context_modification_request_received_then_error_indication_is_sent)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Modification Request.
  ngap_message ue_context_mod_request =
      generate_invalid_ue_context_modification_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(ue_context_mod_request);

  // Check that error indication has been sent to AMF.
  ASSERT_TRUE(was_error_indication_sent());
}

/// Test UE context modification for unknown UE.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_modification_request_for_unknown_ue_received_then_failure_is_sent)
{
  // Test preamble
  cu_cp_ue_index_t ue_index = this->start_procedure();

  auto& ue = test_ues.at(ue_index);

  // Inject Initial Context Setup Request.
  ngap_message init_context_setup_request =
      generate_valid_initial_context_setup_request_message(ue.amf_ue_id.value(), ue.ran_ue_id.value());
  ngap->handle_message(init_context_setup_request);

  ASSERT_TRUE(was_ue_added());

  // Inject UE Context Modification Request for unknown UE.
  amf_ue_id_t unknown_amf_ue_id = uint_to_amf_ue_id(to_underlying(ue.amf_ue_id.value()) + 1);
  ran_ue_id_t unknown_ran_ue_id = uint_to_ran_ue_id(to_underlying(ue.ran_ue_id.value()) + 1);

  ngap_message ue_context_mod_request = generate_valid_ue_context_modification_request_message(
      unknown_amf_ue_id,
      unknown_ran_ue_id,
      aggregate_maximum_bit_rate_t{.dl = 100000, .ul = 100000},
      cu_cp_core_network_assist_info_for_inactive{.ue_id_idx_value = 0xc0},
      guami_t{.plmn = plmn_identity::test_value(), .amf_set_id = 1, .amf_pointer = 1, .amf_region_id = 1});
  ngap->handle_message(ue_context_mod_request);

  // Check that error indication has been sent to AMF.
  ASSERT_TRUE(was_error_indication_sent());
}

/// Test when UE Context Modification Request with inconsistent NGAP ID pair is received,
/// an error indication is sent.
TEST_F(ngap_ue_context_management_procedure_test,
       when_ue_context_modification_request_has_inconsistent_id_pair_then_error_indication_is_sent)
{
  // Test preamble.
  cu_cp_ue_index_t ue_index1 = this->start_procedure();
  cu_cp_ue_index_t ue_index2 = this->start_procedure(to_rnti(0x2));

  auto& ue1 = test_ues.at(ue_index1);
  auto& ue2 = test_ues.at(ue_index2);

  // Inject UE Context Modification Request with inconsistent NGAP ID pair.
  ngap_message ue_context_mod_request =
      generate_valid_ue_context_modification_request_message(ue1.amf_ue_id.value(), ue2.ran_ue_id.value());
  ngap->handle_message(ue_context_mod_request);

  // Check that error indication has been sent to AMF.
  ASSERT_TRUE(was_error_indication_sent());
  ASSERT_EQ(n2_gw.last_ngap_msgs.back().pdu.init_msg().value.error_ind()->cause.radio_network(),
            asn1::ngap::cause_radio_network_e::options::inconsistent_remote_ue_ngap_id);
}
