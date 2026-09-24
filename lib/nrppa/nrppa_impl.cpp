// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "nrppa_impl.h"
#include "nrppa_asn1_helpers.h"
#include "nrppa_helper.h"
#include "procedures/e_cid_measurement_initiation_procedure.h"
#include "procedures/e_cid_measurement_termination_procedure.h"
#include "procedures/measurement_procedure.h"
#include "procedures/positioning_activation_procedure.h"
#include "procedures/positioning_information_exchange_procedure.h"
#include "procedures/trp_information_exchange_procedure.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/nrppa/common.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/support/async/coroutine.h"
#include "fmt/format.h"
#include <variant>

using namespace ocudu;
using namespace asn1::nrppa;
using namespace ocucp;

static inline void create_plmn_tac_map(const std::vector<supported_tracking_area>& supported_tas,
                                       std::map<plmn_identity, tac_t>&             plmn_tac_map)
{
  for (const auto& supported_ta : supported_tas) {
    for (const auto& plmn : supported_ta.plmn_list) {
      plmn_tac_map.emplace(plmn.plmn_id, supported_ta.tac);
    }
  }
}

nrppa_impl::nrppa_impl(const std::vector<supported_tracking_area>& supported_tas_,
                       nrppa_cu_cp_notifier&                       cu_cp_notifier_,
                       async_task_scheduler&                       common_task_sched_,
                       timer_manager&                              timers_,
                       task_executor&                              task_exec_) :
  logger(ocudulog::fetch_basic_logger("NRPPA")),
  ue_ctxt_list(logger),
  du_ctxt_list(logger),
  meas_ctxt_list(logger),
  cu_cp_notifier(cu_cp_notifier_),
  common_task_sched(common_task_sched_),
  timers(timers_),
  task_exec(task_exec_)
{
  create_plmn_tac_map(supported_tas_, plmn_to_tac);
}

// Note: For fwd declaration of member types, dtor cannot be trivial.
nrppa_impl::~nrppa_impl() {}

void nrppa_impl::remove_ue_context(cu_cp_ue_index_t ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.debug("ue={}: UE context not found", ue_index);
    return;
  }

  // Stop the measurement the gNB-DU runs, so that it does not keep reporting for a UE that is gone.
  terminate_du_e_cid_measurement(ue_index);

  ue_ctxt_list.remove_ue_context(ue_index);
}

void nrppa_impl::update_ue_index(cu_cp_ue_index_t         new_ue_index,
                                 cu_cp_ue_index_t         old_ue_index,
                                 nrppa_cu_cp_ue_notifier& new_ue_notifier)
{
  if (!ue_ctxt_list.contains(old_ue_index)) {
    // No NRPPA context to transfer.
    return;
  }

  ue_ctxt_list.update_ue_index(new_ue_index, old_ue_index, new_ue_notifier, timers, task_exec);
}

void nrppa_impl::handle_du_addition(cu_cp_du_index_t du_index, nrppa_f1ap_notifier& f1ap_notifier)
{
  if (du_ctxt_list.contains(du_index)) {
    logger.warning("du={}: NRPPA DU context already exists", du_index);
    return;
  }

  du_ctxt_list.add_du(du_index, f1ap_notifier);
}

void nrppa_impl::handle_du_removal(cu_cp_du_index_t du_index)
{
  if (!du_ctxt_list.contains(du_index)) {
    return;
  }

  trp_registry.remove_du(du_index);
  du_ctxt_list.remove_du_context(du_index);
}

void nrppa_impl::handle_e_cid_measurement_report(const e_cid_measurement_report_t& report)
{
  if (!ue_ctxt_list.contains(report.ue_index)) {
    logger.warning("ue={}: Dropping E-CID measurement report. UE context doesn't exist", report.ue_index);
    return;
  }

  nrppa_ue_context& ue_ctxt = ue_ctxt_list[report.ue_index];

  if (report.ran_ue_meas_id != ue_ctxt.ue_ids.ran_ue_meas_id) {
    ue_ctxt.logger.log_warning("Dropping E-CID measurement report for unknown ran_ue_meas_id={}",
                               fmt::underlying(report.ran_ue_meas_id));
    return;
  }

  if (!ue_ctxt.du_e_cid_meas_active) {
    ue_ctxt.logger.log_warning("Dropping E-CID measurement report. No measurement is running at the gNB-DU");
    return;
  }

  nrppa_cu_cp_ue_notifier* ue = ue_ctxt.get_cu_cp_ue();
  if (ue == nullptr) {
    ue_ctxt.logger.log_warning("Dropping E-CID measurement report. UE doesn't exist");
    return;
  }

  std::optional<nr_cell_global_id_t> serving_cell_id = ue->get_serving_cell_id();
  if (!serving_cell_id.has_value() or plmn_to_tac.find(serving_cell_id.value().plmn_id) == plmn_to_tac.end()) {
    ue_ctxt.logger.log_warning("Dropping E-CID measurement report. Serving cell of the UE is unknown");
    return;
  }

  nrppa_e_cid_meas_result meas_result;
  meas_result.serving_cell_id  = serving_cell_id.value();
  meas_result.serving_cell_tac = plmn_to_tac.at(serving_cell_id.value().plmn_id);
  meas_result.measured_results = fill_du_measured_results(report.e_cid_meas_result);

  handle_e_cid_meas_result(report.ue_index, meas_result);
}

void nrppa_impl::handle_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind)
{
  if (!ue_ctxt_list.contains(ind.ue_index)) {
    logger.warning("ue={}: Dropping E-CID measurement failure indication. UE context doesn't exist", ind.ue_index);
    return;
  }

  nrppa_ue_context& ue_ctxt = ue_ctxt_list[ind.ue_index];

  if (ind.ran_ue_meas_id != ue_ctxt.ue_ids.ran_ue_meas_id) {
    ue_ctxt.logger.log_warning("Dropping E-CID measurement failure indication for unknown ran_ue_meas_id={}",
                               fmt::underlying(ind.ran_ue_meas_id));
    return;
  }

  // The gNB-DU has terminated the E-CID measurements for the UE, as per TS 38.473 section 8.13.13.2.
  ue_ctxt.du_e_cid_meas_active = false;

  send_e_cid_meas_failure_indication(ue_ctxt, nrppa_cause_radio_network_t::requested_item_temporarily_not_available);
}

void nrppa_impl::send_e_cid_meas_failure_indication(const nrppa_ue_context& ue_ctxt, nrppa_cause_t cause)
{
  asn1::nrppa::nr_ppa_pdu_c nrppa_msg;
  nrppa_msg.set_init_msg();
  nrppa_msg.init_msg().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_FAIL_IND);

  auto& asn1_ind           = nrppa_msg.init_msg().value.e_c_id_meas_fail_ind();
  asn1_ind->lmf_ue_meas_id = to_underlying(ue_ctxt.ue_ids.lmf_ue_meas_id);
  asn1_ind->ran_ue_meas_id = to_underlying(ue_ctxt.ue_ids.ran_ue_meas_id);
  asn1_ind->cause          = cause_to_asn1(cause);

  byte_buffer nrppa_pdu = pack_into_pdu(nrppa_msg, "E-CID measurement failure indication");

  log_nrppa_message(logger, Tx, nrppa_pdu, nrppa_msg);
  cu_cp_notifier.on_ul_nrppa_pdu(nrppa_pdu, ue_ctxt.ue_ids.ue_index);
}

void nrppa_impl::terminate_du_e_cid_measurement(cu_cp_ue_index_t ue_index)
{
  nrppa_ue_context& ue_ctxt = ue_ctxt_list[ue_index];
  if (!ue_ctxt.du_e_cid_meas_active) {
    return;
  }

  nrppa_cu_cp_ue_notifier* ue = ue_ctxt.get_cu_cp_ue();
  if (ue == nullptr) {
    ue_ctxt.du_e_cid_meas_active = false;
    return;
  }

  cu_cp_du_index_t du_index = ue->get_du_index();
  if (du_ctxt_list.contains(du_index)) {
    du_ctxt_list[du_index].f1ap->on_e_cid_measurement_termination_command(e_cid_measurement_termination_command_t{
        ue_index, ue_ctxt.ue_ids.lmf_ue_meas_id, ue_ctxt.ue_ids.ran_ue_meas_id});
  }

  ue_ctxt.du_e_cid_meas_active = false;
}

void nrppa_impl::initialize_meas_report_timer(cu_cp_ue_index_t ue_index, std::chrono::milliseconds meas_periodicity_ms)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: UE context not found", ue_index);
    return;
  }

  nrppa_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  if (ue_ctxt.meas_report_timer.is_running()) {
    ue_ctxt.logger.log_warning("Measurement report timer already running");
    return;
  }

  // Start timer.
  ue_ctxt.meas_report_timer.set(meas_periodicity_ms, [this, ue_index]() { on_meas_report_timer_expired(ue_index); });
  ue_ctxt.meas_report_timer.run();
}

void nrppa_impl::handle_e_cid_meas_result(cu_cp_ue_index_t ue_index, const nrppa_e_cid_meas_result& result)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: Dropping E-CID measurement result. UE context doesn't exists", ue_index);
    return;
  }
  auto& ue_ctxt = ue_ctxt_list[ue_index];

  // Fill measurement report.
  asn1::nrppa::nr_ppa_pdu_c nrppa_msg;
  nrppa_msg.set_init_msg();
  nrppa_msg.init_msg().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_REPORT);

  auto& asn1_report               = nrppa_msg.init_msg().value.e_c_id_meas_report();
  asn1_report->lmf_ue_meas_id     = to_underlying(ue_ctxt.ue_ids.lmf_ue_meas_id);
  asn1_report->ran_ue_meas_id     = to_underlying(ue_ctxt.ue_ids.ran_ue_meas_id);
  asn1_report->e_c_id_meas_result = e_cid_meas_result_to_asn1(result);

  byte_buffer nrppa_pdu = pack_into_pdu(nrppa_msg, "E-CID measurement report");

  // Log Tx message.
  log_nrppa_message(logger, Tx, nrppa_pdu, nrppa_msg);
  cu_cp_notifier.on_ul_nrppa_pdu(nrppa_pdu, ue_index);
}

void nrppa_impl::on_meas_report_timer_expired(cu_cp_ue_index_t ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: UE context not found", ue_index);
    return;
  }

  nrppa_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} lmf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_meas_id),
               fmt::underlying(ue_ctxt.ue_ids.lmf_ue_meas_id));

  // Restart timer.
  ue_ctxt.meas_report_timer.set(ue_ctxt.meas_periodicity_ms.value(),
                                [this, ue_index]() { on_meas_report_timer_expired(ue_index); });
  ue_ctxt.meas_report_timer.run();

  std::optional<cell_measurement_positioning_info>& ue_measurement_results = ue->on_measurement_results_required();
  if (!ue_measurement_results.has_value()) {
    logger.warning("ue={}: No measurement results available", ue_index);
    return;
  }

  if (ue_measurement_results.value().cell_measurements.empty()) {
    logger.warning("ue={}: No measurement results available", ue_index);
    return;
  }

  plmn_identity plmn = ue_measurement_results.value().serving_cell_id.plmn_id;
  if (plmn_to_tac.find(plmn) == plmn_to_tac.end()) {
    logger.warning("ue={}: TAC for PLMN={} not found", ue_index, plmn);
    return;
  }

  // Fill measurement result.
  nrppa_e_cid_meas_result meas_result;
  meas_result.serving_cell_id  = ue_measurement_results.value().serving_cell_id;
  meas_result.serving_cell_tac = plmn_to_tac.at(plmn);
  meas_result.measured_results = fill_rrc_measured_results(ue_measurement_results.value(), ue_ctxt.meas_quantities);
  if (meas_result.measured_results.empty()) {
    logger.warning("ue={}: No supported measurement quantity requested", ue_index);
    return;
  }

  handle_e_cid_meas_result(ue_index, meas_result);
}

void nrppa_impl::handle_new_nrppa_pdu(const byte_buffer&                                nrppa_pdu,
                                      std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index)
{
  // Parse NRPPa-PDU.
  asn1::nrppa::nr_ppa_pdu_c nrppa_msg;
  asn1::cbit_ref            bref(nrppa_pdu);
  if (nrppa_msg.unpack(bref) != asn1::OCUDUASN_SUCCESS) {
    logger.error(nrppa_pdu.begin(), nrppa_pdu.end(), "Failed to unpack NRPPa-PDU");
    return;
  }

  // Log Rx message.
  log_nrppa_message(logger, Rx, nrppa_pdu, nrppa_msg);

  switch (nrppa_msg.type().value) {
    case nr_ppa_pdu_c::types_opts::init_msg:
      handle_initiating_message(nrppa_msg.init_msg(), ue_or_amf_index);
      break;
    case nr_ppa_pdu_c::types_opts::successful_outcome:
      handle_successful_outcome(nrppa_msg.successful_outcome());
      break;
    case nr_ppa_pdu_c::types_opts::unsuccessful_outcome:
      handle_unsuccessful_outcome(nrppa_msg.unsuccessful_outcome());
      break;
    default:
      logger.error("Invalid PDU type");
      break;
  }
}

void nrppa_impl::handle_initiating_message(const init_msg_s&                                 msg,
                                           std::variant<cu_cp_ue_index_t, cu_cp_amf_index_t> ue_or_amf_index)
{
  // The NRPPa PDU is opaque to NGAP, so the AMF may carry a UE-associated procedure in a non-UE-associated NRPPa
  // transport, or vice versa. Each procedure takes its index through one of these helpers, which drop the PDU when the
  // index is missing.
  auto with_ue_index = [&](auto&& handler) {
    if (const auto* ue_index = std::get_if<cu_cp_ue_index_t>(&ue_or_amf_index)) {
      handler(*ue_index);
      return;
    }
    logger.warning("Discarding {}. Cause: Received in a non-UE-associated NRPPa transport",
                   msg.value.type().to_string());
  };
  auto with_amf_index = [&](auto&& handler) {
    if (const auto* amf_index = std::get_if<cu_cp_amf_index_t>(&ue_or_amf_index)) {
      handler(*amf_index);
      return;
    }
    logger.warning("Discarding {}. Cause: Received in a UE-associated NRPPa transport", msg.value.type().to_string());
  };

  switch (msg.value.type().value) {
    case nr_ppa_elem_procs_o::init_msg_c::types_opts::e_c_id_meas_initiation_request:
      with_ue_index([&](cu_cp_ue_index_t ue_index) {
        handle_e_cid_meas_initiation_request(
            msg.value.e_c_id_meas_initiation_request(), ue_index, msg.nrppatransaction_id);
      });
      break;
    case nr_ppa_elem_procs_o::init_msg_c::types_opts::e_c_id_meas_termination_cmd:
      with_ue_index([&](cu_cp_ue_index_t ue_index) {
        handle_e_cid_meas_termination_command(msg.value.e_c_id_meas_termination_cmd(), ue_index);
      });
      break;
    case nr_ppa_elem_procs_o::init_msg_c::types_opts::trp_info_request:
      with_amf_index([&](cu_cp_amf_index_t amf_index) {
        handle_trp_information_request(msg.value.trp_info_request(), amf_index, msg.nrppatransaction_id);
      });
      break;
    case nr_ppa_elem_procs_o::init_msg_c::types_opts::positioning_info_request:
      with_ue_index([&](cu_cp_ue_index_t ue_index) {
        handle_positioning_information_request(msg.value.positioning_info_request(), ue_index, msg.nrppatransaction_id);
      });
      break;
    case nr_ppa_elem_procs_o::init_msg_c::types_opts::positioning_activation_request:
      with_ue_index([&](cu_cp_ue_index_t ue_index) {
        handle_positioning_activation_request(
            msg.value.positioning_activation_request(), ue_index, msg.nrppatransaction_id);
      });
      break;
    case nr_ppa_elem_procs_o::init_msg_c::types_opts::meas_request:
      with_amf_index([&](cu_cp_amf_index_t amf_index) {
        handle_measurement_request(msg.value.meas_request(), amf_index, msg.nrppatransaction_id);
      });
      break;
    case nr_ppa_elem_procs_o::init_msg_c::types_opts::otdoa_info_request:
      // OTDOA is an E-UTRA positioning method (TS 38.455 section 8.2.5).
      logger.info("Ignoring {}. Cause: OTDOA is not supported", msg.value.type().to_string());
      break;
    default:
      logger.error("Initiating message of type {} is not supported", msg.value.type().to_string());
      break;
  }
}

void nrppa_impl::handle_e_cid_meas_initiation_request(const asn1::nrppa::e_c_id_meas_initiation_request_s& msg,
                                                      cu_cp_ue_index_t                                     ue_index,
                                                      uint16_t transaction_id)
{
  // If it doesn't exist, create NRPPA UE.
  if (!ue_ctxt_list.contains(ue_index)) {
    // Allocate RAN UE MEAS ID.
    auto ret = ue_ctxt_list.allocate_ran_ue_meas_id();
    if (!ret.has_value()) {
      logger.error("ue={}: Couldn't allocate ran ue meas id. Cause: {}", ue_index, ret.error());

      // Create failure response.
      asn1::nrppa::nr_ppa_pdu_c asn1_fail;
      asn1_fail.set_unsuccessful_outcome().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_INITIATION);
      asn1_fail.unsuccessful_outcome().nrppatransaction_id = transaction_id;
      asn1::nrppa::e_c_id_meas_initiation_fail_s& meas_init_fail =
          asn1_fail.unsuccessful_outcome().value.e_c_id_meas_initiation_fail();
      meas_init_fail->lmf_ue_meas_id = msg->lmf_ue_meas_id;
      meas_init_fail->cause          = cause_to_asn1(nrppa_cause_protocol_t::msg_not_compatible_with_receiver_state);
      byte_buffer ul_nrppa_pdu       = pack_into_pdu(asn1_fail, "ECIDMeasInitiationFailure");

      // Log Tx message.
      log_nrppa_message(logger, Tx, ul_nrppa_pdu, asn1_fail);

      // Send response to CU-CP.
      cu_cp_notifier.on_ul_nrppa_pdu(ul_nrppa_pdu, ue_index);

      return;
    }

    // Create UE context and store it.
    nrppa_cu_cp_ue_notifier* ue_notifier = cu_cp_notifier.on_new_nrppa_ue(ue_index);
    if (ue_notifier == nullptr) {
      logger.warning("ue={}: Dropping E-CID measurement initiation request. UE doesn't exist", ue_index);

      // Create failure response.
      asn1::nrppa::nr_ppa_pdu_c asn1_fail;
      asn1_fail.set_unsuccessful_outcome().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_INITIATION);
      asn1_fail.unsuccessful_outcome().nrppatransaction_id = transaction_id;
      asn1::nrppa::e_c_id_meas_initiation_fail_s& meas_init_fail =
          asn1_fail.unsuccessful_outcome().value.e_c_id_meas_initiation_fail();
      meas_init_fail->lmf_ue_meas_id = msg->lmf_ue_meas_id;
      meas_init_fail->cause          = cause_to_asn1(nrppa_cause_protocol_t::msg_not_compatible_with_receiver_state);
      byte_buffer ul_nrppa_pdu       = pack_into_pdu(asn1_fail, "ECIDMeasInitiationFailure");

      // Log Tx message.
      log_nrppa_message(logger, Tx, ul_nrppa_pdu, asn1_fail);

      // Send response to CU-CP.
      cu_cp_notifier.on_ul_nrppa_pdu(ul_nrppa_pdu, ue_index);

      return;
    }
    ue_ctxt_list.add_ue(
        ue_index, ret.value(), uint_to_lmf_ue_meas_id(msg->lmf_ue_meas_id), *ue_notifier, timers, task_exec);
  }

  nrppa_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} lmf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_meas_id),
               fmt::underlying(ue_ctxt.ue_ids.lmf_ue_meas_id));

  logger.debug("Handling E-CID measurement initiation request");

  nrppa_e_cid_meas_initiation_request request;
  fill_nrppa_e_cid_meas_initiation_request(request, msg);

  ue->schedule_async_task(launch_async<e_cid_measurement_initiation_procedure>(
      ue_index, request, transaction_id, ue_ctxt_list, du_ctxt_list, cu_cp_notifier, plmn_to_tac, *this, logger));
}

void nrppa_impl::handle_e_cid_meas_termination_command(const asn1::nrppa::e_c_id_meas_termination_cmd_s& msg,
                                                       cu_cp_ue_index_t                                  ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: Dropping E-CID Measurement Termination Command. UE context doesn't exists", ue_index);
    return;
  }

  nrppa_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  ue_ctxt.logger.log_debug("Handling E-CID measurement termination command");

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} lmf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_meas_id),
               fmt::underlying(ue_ctxt.ue_ids.lmf_ue_meas_id));

  ue->schedule_async_task(launch_async<e_cid_measurement_termination_procedure>(ue_index, ue_ctxt_list, *this, logger));
}

void nrppa_impl::handle_trp_information_request(const asn1::nrppa::trp_info_request_s& msg,
                                                cu_cp_amf_index_t                      amf_index,
                                                uint16_t                               transaction_id)
{
  logger.debug("Handling TRP Information Request");

  trp_information_request_t request;
  fill_trp_information_request(request, msg);

  common_task_sched.schedule(launch_async<trp_information_exchange_procedure>(
      amf_index, request, transaction_id, cu_cp_notifier, trp_registry, logger));
}

void nrppa_impl::handle_positioning_information_request(const asn1::nrppa::positioning_info_request_s& msg,
                                                        cu_cp_ue_index_t                               ue_index,
                                                        uint16_t                                       transaction_id)
{
  logger.debug("Handling positioning information request");

  nrppa_cu_cp_ue_notifier* ue_notifier = cu_cp_notifier.on_new_nrppa_ue(ue_index);
  if (ue_notifier == nullptr) {
    logger.warning("ue={}: Dropping positioning information request. UE doesn't exist", ue_index);

    // Create failure response.
    asn1::nrppa::nr_ppa_pdu_c asn1_fail;
    asn1_fail.set_unsuccessful_outcome().load_info_obj(ASN1_NRPPA_ID_POSITIONING_INFO_EXCHANGE);
    asn1_fail.unsuccessful_outcome().nrppatransaction_id = transaction_id;
    asn1::nrppa::positioning_info_fail_s& pos_info_fail =
        asn1_fail.unsuccessful_outcome().value.positioning_info_fail();
    pos_info_fail->cause     = cause_to_asn1(nrppa_cause_protocol_t::msg_not_compatible_with_receiver_state);
    byte_buffer ul_nrppa_pdu = pack_into_pdu(asn1_fail, "PositioningInformationFailure");

    // Log Tx message.
    log_nrppa_message(logger, Tx, ul_nrppa_pdu, asn1_fail);

    // Send response to CU-CP.
    cu_cp_notifier.on_ul_nrppa_pdu(ul_nrppa_pdu, ue_index);

    return;
  }
  cu_cp_du_index_t du_index = ue_notifier->get_du_index();

  positioning_information_request_t request;
  request.ue_index = ue_index;
  fill_positioning_information_request(request, msg);

  ue_notifier->schedule_async_task(launch_async<positioning_information_exchange_procedure>(
      request, transaction_id, du_index, du_ctxt_list, cu_cp_notifier, logger));
}

void nrppa_impl::handle_positioning_activation_request(const asn1::nrppa::positioning_activation_request_s& msg,
                                                       cu_cp_ue_index_t                                     ue_index,
                                                       uint16_t transaction_id)
{
  logger.debug("Handling positioning activation request");

  nrppa_cu_cp_ue_notifier* ue_notifier = cu_cp_notifier.on_new_nrppa_ue(ue_index);
  if (ue_notifier == nullptr) {
    logger.warning("ue={}: Dropping positioning activation request. UE doesn't exist", ue_index);

    // Create failure response.
    asn1::nrppa::nr_ppa_pdu_c asn1_fail;
    asn1_fail.set_unsuccessful_outcome().load_info_obj(ASN1_NRPPA_ID_POSITIONING_ACTIVATION);
    asn1_fail.unsuccessful_outcome().nrppatransaction_id = transaction_id;
    asn1::nrppa::positioning_activation_fail_s& pos_activation_fail =
        asn1_fail.unsuccessful_outcome().value.positioning_activation_fail();
    pos_activation_fail->cause = cause_to_asn1(nrppa_cause_protocol_t::msg_not_compatible_with_receiver_state);
    byte_buffer ul_nrppa_pdu   = pack_into_pdu(asn1_fail, "PositioningActivationFailure");

    // Log Tx message.
    log_nrppa_message(logger, Tx, ul_nrppa_pdu, asn1_fail);

    // Send response to CU-CP.
    cu_cp_notifier.on_ul_nrppa_pdu(ul_nrppa_pdu, ue_index);

    return;
  }
  cu_cp_du_index_t du_index = ue_notifier->get_du_index();

  positioning_activation_request_t request;
  request.ue_index = ue_index;
  fill_positioning_activation_request(request, msg);

  ue_notifier->schedule_async_task(launch_async<positioning_activation_procedure>(
      request, transaction_id, du_index, du_ctxt_list, cu_cp_notifier, logger));
}

void nrppa_impl::handle_measurement_request(const asn1::nrppa::meas_request_s& msg,
                                            cu_cp_amf_index_t                  amf_index,
                                            uint16_t                           transaction_id)
{
  logger.debug("Handling Measurement Request");

  measurement_request_t request;
  fill_measurement_request(request, msg);

  common_task_sched.schedule(launch_async<measurement_procedure>(
      amf_index, request, transaction_id, trp_registry, meas_ctxt_list, du_ctxt_list, cu_cp_notifier, logger));
}

void nrppa_impl::handle_successful_outcome(const successful_outcome_s& outcome)
{
  switch (outcome.value.type().value) {
    default:
      logger.error("Successful outcome of type {} is not supported", outcome.value.type().to_string());
  }
}

void nrppa_impl::handle_unsuccessful_outcome(const unsuccessful_outcome_s& outcome)
{
  switch (outcome.value.type().value) {
    default:
      logger.error("Unsuccessful outcome of type {} is not supported", outcome.value.type().to_string());
  }
}
