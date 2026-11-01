// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "procedures/rrc_reconfiguration_procedure.h"
#include "procedures/rrc_reestablishment_procedure.h"
#include "procedures/rrc_resume_procedure.h"
#include "procedures/rrc_setup_procedure.h"
#include "procedures/rrc_ue_capability_transfer_procedure.h"
#include "rrc_asn1_helpers.h"
#include "rrc_ue_helpers.h"
#include "rrc_ue_impl.h"
#include "rrc_ue_security_helpers.h"
#include "ue/rrc_asn1_converters.h"
#include "ue/rrc_measurement_types_asn1_converters.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/asn1_utils.h"
#include "ocudu/asn1/rrc_nr/dl_ccch_msg.h"
#include "ocudu/asn1/rrc_nr/dl_dcch_msg_ies.h"
#include "ocudu/asn1/rrc_nr/ul_ccch_msg.h"
#include "ocudu/ran/rb_id.h"
#include "ocudu/support/ocudu_assert.h"
#include <algorithm>
#include <chrono>

using namespace ocudu;
using namespace ocucp;
using namespace asn1::rrc_nr;

void rrc_ue_impl::handle_ul_ccch_pdu(byte_buffer pdu, rnti_t c_rnti)
{
  // Parse UL-CCCH.
  ul_ccch_msg_s ul_ccch_msg;
  {
    asn1::cbit_ref bref(pdu);
    if (ul_ccch_msg.unpack(bref) != asn1::OCUDUASN_SUCCESS or
        ul_ccch_msg.msg.type().value != ul_ccch_msg_type_c::types_opts::c1) {
      logger.log_error(pdu.begin(), pdu.end(), "Failed to unpack CCCH UL PDU");
      on_ue_release_required(ngap_cause_radio_network_t::unspecified);
      return;
    }
  }

  // Log Rx message.
  log_rrc_message(logger, Rx, pdu, ul_ccch_msg, srb_id_t::srb0, "CCCH UL");

  // Handle message.
  switch (ul_ccch_msg.msg.c1().type().value) {
    case ul_ccch_msg_type_c::c1_c_::types_opts::rrc_setup_request:
      handle_rrc_setup_request(ul_ccch_msg.msg.c1().rrc_setup_request());
      break;
    case ul_ccch_msg_type_c::c1_c_::types_opts::rrc_reest_request:
      handle_rrc_reest_request(ul_ccch_msg.msg.c1().rrc_reest_request());
      break;
    case ul_ccch_msg_type_c::c1_c_::types_opts::rrc_resume_request:
      handle_rrc_resume_request(ul_ccch_msg.msg.c1().rrc_resume_request(), c_rnti);
      break;
    default:
      logger.log_error("Unsupported CCCH UL message type");
      on_ue_release_required(ngap_cause_radio_network_t::unspecified);
  }
}

void rrc_ue_impl::handle_rrc_setup_request(const asn1::rrc_nr::rrc_setup_request_s& request_msg)
{
  const establishment_cause_t establishment_cause =
      asn1_to_establishment_cause(request_msg.rrc_setup_request.establishment_cause.value);

  // Perform various checks to make sure we can serve the RRC Setup Request.
  if (not cu_cp_notifier.on_ue_setup_request(establishment_cause)) {
    logger.log_warning("Sending Connection Reject. Cause: RRC connections not allowed");
    metrics_notifier.on_failed_rrc_connection_establishment(establishment_fail_cause_t::network_reject);
    on_ue_release_required(ngap_cause_radio_network_t::unspecified);
    return;
  }

  if (du_to_cu_container.empty()) {
    // If the DU to CU container is missing, assume the DU can't serve the UE, so the CU-CP should reject the UE, see
    // TS 38.473 section 8.4.1.2.
    logger.log_warning("Sending rrcReject. Cause: DU is not able to serve the UE");
    metrics_notifier.on_failed_rrc_connection_establishment(establishment_fail_cause_t::network_reject);
    on_ue_release_required(ngap_cause_radio_network_t::unspecified);
    return;
  }

  // Extract the setup ID and cause.
  const rrc_setup_request_ies_s& request_ies = request_msg.rrc_setup_request;
  switch (request_ies.ue_id.type().value) {
    case init_ue_id_c::types_opts::ng_5_g_s_tmsi_part1: {
      context.setup_ue_id = request_ies.ue_id.ng_5_g_s_tmsi_part1();
      break;
    }
    case asn1::rrc_nr::init_ue_id_c::types_opts::random_value:
      context.setup_ue_id = request_ies.ue_id.random_value().to_number();
      // TODO: communicate with NGAP
      break;
    default:
      logger.log_error("Unsupported RRCSetupRequest");
      metrics_notifier.on_failed_rrc_connection_establishment(establishment_fail_cause_t::network_reject);
      on_ue_release_required(ngap_cause_radio_network_t::unspecified);
      return;
  }
  context.connection_cause = asn1_to_establishment_cause(request_ies.establishment_cause.value);

  // Launch RRC setup procedure.
  cu_cp_ue_notifier.schedule_async_task(launch_async<rrc_setup_procedure>(context,
                                                                          du_to_cu_container,
                                                                          *this,
                                                                          get_rrc_ue_control_message_handler(),
                                                                          cu_cp_notifier,
                                                                          metrics_notifier,
                                                                          ngap_notifier,
                                                                          *event_mng,
                                                                          logger));
}

void rrc_ue_impl::handle_rrc_reest_request(const asn1::rrc_nr::rrc_reest_request_s& msg)
{
  // If the DU to CU container is missing, assume the DU can't serve the UE, so the CU-CP should reject the UE, see
  // TS 38.473 section 8.4.1.2.
  if (du_to_cu_container.empty()) {
    // Notify the CU-CP about the reestablishment. This will return the old RRC UE context if it exists and will also
    // cancel an possibly ongoing handover transaction for the old UE.
    rrc_ue_reestablishment_context_response old_ue_reest_context = cu_cp_notifier.on_rrc_reestablishment_request(
        msg.rrc_reest_request.ue_id.pci, to_rnti(msg.rrc_reest_request.ue_id.c_rnti));

    // Release the old UE.
    logger.log_debug("Requesting UE context release for old_ue={}", old_ue_reest_context.ue_index);
    cu_cp_notifier.on_rrc_reestablishment_failure(
        {.ue_index = old_ue_reest_context.ue_index, .cause = ngap_cause_radio_network_t::unspecified});

    // Reject and release the new UE.
    logger.log_warning("Sending rrcReject. Cause: DU is not able to serve the UE");
    on_ue_release_required(ngap_cause_radio_network_t::unspecified);
    return;
  }

  // Launch RRC re-establishment procedure.
  cu_cp_ue_notifier.schedule_async_task(
      launch_async<rrc_reestablishment_procedure>(msg,
                                                  context,
                                                  du_to_cu_container,
                                                  *this,
                                                  *this,
                                                  get_rrc_ue_control_message_handler(),
                                                  cu_cp_notifier,
                                                  cu_cp_ue_notifier,
                                                  metrics_notifier,
                                                  ngap_notifier,
                                                  *event_mng,
                                                  logger));
}

void rrc_ue_impl::handle_rrc_resume_request(const asn1::rrc_nr::rrc_resume_request_s& msg, rnti_t c_rnti)
{
  // If the DU to CU container is missing, assume the DU can't serve the UE, so the CU-CP should reject the UE, see
  // TS 38.473 section 8.4.1.2.
  if (du_to_cu_container.empty()) {
    // Reject and release the UE.
    logger.log_warning("Sending rrcReject. Cause: DU is not able to serve the UE");
    // Notify metrics about RRC connection resume followed by network release.
    metrics_notifier.on_rrc_connection_resume_followed_by_network_release(
        asn1_to_resume_cause(msg.rrc_resume_request.resume_cause));

    on_ue_release_required(ngap_cause_radio_network_t::unspecified);
    return;
  }

  // An I-RNTI that matches no local UE was allocated by a peer, which is asked for the context over Xn
  // (TS 38.300 section 9.2.2.4.1).
  //
  // An RNA update is excluded: it does not move the UE to RRC_CONNECTED, so retrieving the context would have to be
  // followed by a path switch and an RRCRelease with suspend indication to keep the UE in RRC_INACTIVE at this node
  // (TS 38.300 section 9.2.2.5). Until that anchor relocation exists, an RNA update from a peer's I-RNTI falls back to
  // RRC Setup, which TS 38.300 section 9.2.2.5 leaves open to the node.
  // TODO: Relocate the anchor on an RNA update instead of falling back, and signal the RRC Resume Cause IE
  // (TS 38.423 section 9.2.3.61) so the peer can decide whether it wants to hand the context over.
  const bool context_retrieval_required =
      context.state == rrc_state::idle and not context.cfg.force_resume_fallback and
      context.remote_resume_context.has_value() and context.remote_resume_context->rrc_resume_id.has_value() and
      asn1_to_resume_cause(msg.rrc_resume_request.resume_cause) != resume_cause_t::rna_upd;

  // If UE context wasn't found (UE is in IDLE) or forced by configuration, fallback to RRC Setup.
  if ((context.state == rrc_state::idle and not context_retrieval_required) or context.cfg.force_resume_fallback) {
    logger.log_info("Received RRC Resume Request, but falling back to RRC Setup. Cause: {}",
                    context.cfg.force_resume_fallback ? "RRC Resumes are disabled" : "UE context wasn't found");
    // Fallback to RRC Setup
    context.connection_cause = asn1_resume_cause_to_establishment_cause(msg.rrc_resume_request.resume_cause);

    // Notify metrics about the attempted RRC connection resume followed by RRC Setup.
    metrics_notifier.on_attempted_rrc_connection_resume_followed_by_rrc_setup(
        asn1_to_resume_cause(msg.rrc_resume_request.resume_cause));

    cu_cp_ue_notifier.schedule_async_task(launch_async<rrc_setup_procedure>(context,
                                                                            du_to_cu_container,
                                                                            *this,
                                                                            get_rrc_ue_control_message_handler(),
                                                                            cu_cp_notifier,
                                                                            metrics_notifier,
                                                                            ngap_notifier,
                                                                            *event_mng,
                                                                            logger,
                                                                            false,
                                                                            true));
    return;
  }

  // Launch RRC resume procedure.
  cu_cp_ue_notifier.schedule_async_task(launch_async<rrc_resume_procedure>(msg,
                                                                           context,
                                                                           c_rnti,
                                                                           du_to_cu_container,
                                                                           *this,
                                                                           *this,
                                                                           get_rrc_ue_control_message_handler(),
                                                                           cu_cp_notifier,
                                                                           cu_cp_ue_notifier,
                                                                           metrics_notifier,
                                                                           ngap_notifier,
                                                                           *event_mng,
                                                                           logger,
                                                                           context_retrieval_required));
}

void rrc_ue_impl::stop()
{
  if (event_mng != nullptr) {
    event_mng->transactions.stop();
  }
}

void rrc_ue_impl::handle_pdu(const srb_id_t srb_id, byte_buffer rrc_pdu, bool integrity_verified)
{
  // Parse UL-DCCH.
  ul_dcch_msg_s ul_dcch_msg;
  {
    asn1::cbit_ref bref(rrc_pdu);
    if (ul_dcch_msg.unpack(bref) != asn1::OCUDUASN_SUCCESS or
        (ul_dcch_msg.msg.type().value != ul_dcch_msg_type_c::types_opts::c1 and
         ul_dcch_msg.msg.type().value != ul_dcch_msg_type_c::types_opts::msg_class_ext)) {
      logger.log_error(rrc_pdu.begin(), rrc_pdu.end(), "Failed to unpack DCCH UL PDU");
      return;
    }
  }

  // Log Rx message.
  log_rrc_message(logger, Rx, rrc_pdu, ul_dcch_msg, srb_id, "DCCH UL");

  // According to TS 38.331 Annex B1 several messages are allowed to be sent unprotected.
  // P=+: Message can be sent unprotected prior to security activation (e.g. before or during SMC transition).
  // P=-: Message should never be sent unprotected.
  //
  // AI=+: Message can be sent without integrity protection after security activation.
  // AI=-: Message should never be sent without integrity protection after security activation.
  // AI=NA: Message can never bet sent after security activation.
  //
  // AC=+: Message can be sent unciphered after security activation.
  // AC=-: Message should never be sent unciphered after security activation.
  // AC=NA: Message can never bet sent after security activation.

  if (ul_dcch_msg.msg.type().value == ul_dcch_msg_type_c::types_opts::msg_class_ext) {
    handle_ul_dcch_msg_class_ext(ul_dcch_msg, integrity_verified);
    return;
  }

  switch (ul_dcch_msg.msg.c1().type().value) {
    case ul_dcch_msg_type_c::c1_c_::types_opts::options::ul_info_transfer:
      // P=+ AI=- CI=-
      handle_ul_info_transfer(ul_dcch_msg.msg.c1().ul_info_transfer().crit_exts.ul_info_transfer());
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::rrc_setup_complete:
      // P=+ AI=NA CI=NA
      handle_rrc_transaction_complete(ul_dcch_msg, ul_dcch_msg.msg.c1().rrc_setup_complete().rrc_transaction_id);
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::security_mode_complete:
      // P=- AI=NA CI=NA (Info: Integrity is applied, but no ciphering. Ciphering is activated after this procedure.)
      if (!integrity_verified) {
        cancel_rrc_transaction(ul_dcch_msg.msg.c1().security_mode_complete().rrc_transaction_id);
        handle_illegal_pdu_integrity(ul_dcch_msg.msg.c1().type().to_string(), integrity_verified);
        return;
      }
      handle_rrc_transaction_complete(ul_dcch_msg, ul_dcch_msg.msg.c1().security_mode_complete().rrc_transaction_id);
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::security_mode_fail:
      // P=+ AI=NA CI=NA (Info: Neither integrity nor ciphering applied.)
      if (integrity_verified) { // Never integrity protected.
        handle_illegal_pdu_integrity(ul_dcch_msg.msg.c1().type().to_string(), integrity_verified);
        return;
      }
      handle_rrc_transaction_complete(ul_dcch_msg, ul_dcch_msg.msg.c1().security_mode_fail().rrc_transaction_id);
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::ue_cap_info:
      // P=+ AI=- CI=-
      handle_rrc_transaction_complete(ul_dcch_msg, ul_dcch_msg.msg.c1().ue_cap_info().rrc_transaction_id);
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::rrc_recfg_complete:
      // P=+ AI=- CI=- (Info: Unprotected in response to RRCConnectionReconfiguration prior to security activation.)
      handle_rrc_transaction_complete(ul_dcch_msg, ul_dcch_msg.msg.c1().rrc_recfg_complete().rrc_transaction_id);
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::rrc_reest_complete:
      // P=- AI=- CI=-
      if (!integrity_verified) {
        handle_illegal_pdu_integrity(ul_dcch_msg.msg.c1().type().to_string(), integrity_verified);
        return;
      }
      handle_rrc_transaction_complete(ul_dcch_msg, ul_dcch_msg.msg.c1().rrc_reest_complete().rrc_transaction_id);
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::rrc_resume_complete:
      // P=- AI=- CI=-
      if (!integrity_verified) {
        handle_illegal_pdu_integrity(ul_dcch_msg.msg.c1().type().to_string(), integrity_verified);
        return;
      }
      handle_rrc_transaction_complete(ul_dcch_msg, ul_dcch_msg.msg.c1().rrc_resume_complete().rrc_transaction_id);
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::meas_report:
      // P=- AI=- CI=- (Info: Never sent unprotected to protect privacy of the UE.)
      if (!integrity_verified) {
        handle_illegal_pdu_integrity(ul_dcch_msg.msg.c1().type().to_string(), integrity_verified);
        return;
      }
      handle_measurement_report(ul_dcch_msg.msg.c1().meas_report());
      break;
    case ul_dcch_msg_type_c::c1_c_::types_opts::location_meas_ind:
      // P=- AI=- CI=- (Info: The UE sends it only after AS security activation, TS 38.331 section 5.5.6.1.)
      if (!integrity_verified) {
        handle_illegal_pdu_integrity(ul_dcch_msg.msg.c1().type().to_string(), integrity_verified);
        return;
      }
      handle_location_measurement_indication(ul_dcch_msg.msg.c1().location_meas_ind());
      break;
    default:
      logger.log_error("Unsupported DCCH UL message type");
      break;
  }
}

void rrc_ue_impl::handle_ul_dcch_msg_class_ext(const asn1::rrc_nr::ul_dcch_msg_s& ul_dcch_msg, bool integrity_verified)
{
  if (ul_dcch_msg.msg.msg_class_ext().type().value != ul_dcch_msg_type_c::msg_class_ext_c_::types_opts::c2) {
    logger.log_error("Unsupported DCCH UL message class extension");
    return;
  }

  const auto& c2 = ul_dcch_msg.msg.msg_class_ext().c2();
  switch (c2.type().value) {
    case ul_dcch_msg_type_c::msg_class_ext_c_::c2_c_::types_opts::ue_info_resp_r16:
      // P=- AI=- CI=- (Info: may carry the coarse UE location, so never accepted unprotected.)
      if (!integrity_verified) {
        handle_illegal_pdu_integrity(c2.type().to_string(), integrity_verified);
        return;
      }
      handle_rrc_transaction_complete(ul_dcch_msg, c2.ue_info_resp_r16().rrc_transaction_id);
      break;
    default:
      logger.log_error("Unsupported DCCH UL message type");
      break;
  }
}

void rrc_ue_impl::handle_illegal_pdu_integrity(const char* msg, bool integrity_verified)
{
  logger.log_warning("Requesting UE release. Cause: Illegal PDU integrity={} for msg={}", integrity_verified, msg);
  on_ue_release_required(cause_protocol_t::unspecified);
}

void rrc_ue_impl::handle_ul_dcch_pdu(srb_id_t srb_id, byte_buffer rrc_pdu, bool integrity_verified)
{
  logger.log_debug(rrc_pdu.begin(), rrc_pdu.end(), "Rx {} RRC PDU", srb_id);

  if (!context.pdcp_manager.has_srb(srb_id)) {
    logger.log_error(rrc_pdu.begin(), rrc_pdu.end(), "Dropping UL-DCCH PDU. Rx {} is not set up", srb_id);
    return;
  }

  handle_pdu(srb_id, std::move(rrc_pdu), integrity_verified);
}

void rrc_ue_impl::handle_security_mode_complete(const asn1::rrc_nr::security_mode_complete_s& msg)
{
  ocudu_sanity_check(context.pdcp_manager.has_srb(srb_id_t::srb1),
                     "Attempted to configure security, but there is no interface to PDCP");

  security::sec_128_as_config sec_cfg = cu_cp_ue_notifier.get_rrc_128_as_config();
  rrc_ue_pdcp_notifier&       srb1    = *context.get_pdcp_notifier(srb_id_t::srb1);
  srb1.enable_rx_security(security::integrity_enabled::on, security::ciphering_enabled::on, sec_cfg);
  srb1.enable_tx_security(security::integrity_enabled::on, security::ciphering_enabled::on, sec_cfg);
}

void rrc_ue_impl::handle_ul_info_transfer(const ul_info_transfer_ies_s& ul_info_transfer)
{
  on_ul_nas_pdu(ul_info_transfer.ded_nas_msg.copy());
}

void rrc_ue_impl::on_ul_nas_pdu(byte_buffer nas_pdu)
{
  cu_cp_ul_nas_transport ul_nas_msg      = {};
  ul_nas_msg.ue_index                    = context.ue_index;
  ul_nas_msg.nas_pdu                     = std::move(nas_pdu);
  ul_nas_msg.user_location_info.nr_cgi   = {context.plmn_id, context.cell.cgi.nci};
  ul_nas_msg.user_location_info.tai      = {context.plmn_id, context.cell.tac};
  ul_nas_msg.user_location_info.tac_list = context.cell.tac_list;
  fill_ue_derived_location(ul_nas_msg.user_location_info);

  if (!ngap_notifier.on_ul_nas_transport_message(ul_nas_msg)) {
    logger.log_info(
        "Requesting UE release. Cause: Received unexpected UL NAS Transport message and failed to forward it to NGAP");
    cancel_all_transactions(protocol_transaction_failure::abnormal);
    on_ue_release_required(cause_protocol_t::msg_not_compatible_with_receiver_state);
    return;
  }
}

void rrc_ue_impl::handle_measurement_report(const asn1::rrc_nr::meas_report_s& msg)
{
  store_coarse_ue_location(context.coarse_location,
                           msg.crit_exts.meas_report().meas_results.coarse_location_info_r17,
                           cu_cp_notifier,
                           logger);

  // Convert asn1 to common type.
  rrc_meas_results meas_results =
      asn1_to_measurement_results(msg.crit_exts.meas_report().meas_results, ocudulog::fetch_basic_logger("RRC"));
  // Send measurement results to cell measurement manager.
  measurement_notifier.on_measurement_report(meas_results);
}

void rrc_ue_impl::handle_location_measurement_indication(const asn1::rrc_nr::location_meas_ind_s& msg)
{
  if (msg.crit_exts.type().value != location_meas_ind_s::crit_exts_c_::types_opts::location_meas_ind) {
    logger.log_warning("Ignoring LocationMeasurementIndication. Cause: Unsupported critical extension");
    return;
  }

  const auto& meas_ind = msg.crit_exts.location_meas_ind().meas_ind;
  if (meas_ind.type().value == asn1::setup_release_opts::release) {
    if (!context.location_meas_info.has_value()) {
      logger.log_info("Ignoring LocationMeasurementIndication. Cause: The UE has no ongoing location measurements");
      return;
    }
    context.location_meas_info.reset();
  } else {
    context.location_meas_info = pack_into_pdu(meas_ind.setup(), "LocationMeasurementInfo");
  }

  // Notify the CU-CP, which requests the measurement gaps for the ongoing location measurements from the DU.
  cu_cp_notifier.on_location_measurement_indication();
}

std::optional<byte_buffer> rrc_ue_impl::get_packed_location_meas_info() const
{
  if (!context.location_meas_info.has_value()) {
    return std::nullopt;
  }
  return context.location_meas_info->copy();
}

void rrc_ue_impl::handle_dl_nas_transport_message(byte_buffer nas_pdu)
{
  if (context.state == rrc_state::inactive) {
    // Store NAS PDU in the UE context to be sent to the UE after a successful resume.
    context.pending_dl_nas_transport_messages.push_back(std::move(nas_pdu));
    // Notify the CU-CP about the reception of the DL NAS Transport message for a UE in RRC Inactive state.
    logger.log_debug("Received DL NAS Transport while UE is in RRC Inactive. Requesting RAN paging for the UE");
    cu_cp_notifier.on_ran_paging_required();
    return;
  }

  dl_dcch_msg_s           dl_dcch_msg;
  dl_info_transfer_ies_s& dl_info_transfer =
      dl_dcch_msg.msg.set_c1().set_dl_info_transfer().crit_exts.set_dl_info_transfer();
  dl_info_transfer.ded_nas_msg = nas_pdu.copy();

  if (context.pdcp_manager.has_srb(srb_id_t::srb2)) {
    send_dl_dcch(srb_id_t::srb2, dl_dcch_msg);
  } else {
    send_dl_dcch(srb_id_t::srb1, dl_dcch_msg);
  }
}

void rrc_ue_impl::handle_rrc_transaction_complete(const ul_dcch_msg_s& msg, uint8_t transaction_id_)
{
  expected<uint8_t> transaction_id = transaction_id_;

  // Set transaction result and resume suspended procedure.
  if (not event_mng->transactions.set_response(transaction_id.value(), msg)) {
    logger.log_warning("Unexpected RRC transaction id={}", transaction_id.value());
  }
}

void rrc_ue_impl::cancel_rrc_transaction(uint8_t transaction_id_)
{
  expected<uint8_t> transaction_id = transaction_id_;

  // Set transaction result and resume suspended procedure.
  if (not event_mng->transactions.cancel_transaction(transaction_id.value())) {
    logger.log_warning("Unable to cancel RRC transaction id={}", transaction_id.value());
  }
}

rrc_ue_security_mode_command_context rrc_ue_impl::get_security_mode_command_context()
{
  // Activate SRB1 PDCP security.
  on_new_as_security_context(/* security_mode_active */ false);

  rrc_ue_security_mode_command_context smc_ctxt;

  if (!context.pdcp_manager.has_srb(srb_id_t::srb1)) {
    logger.log_error("Can't get security mode command. {} is not set up", srb_id_t::srb1);
    return smc_ctxt;
  }

  // Create transaction to get transaction ID.
  rrc_transaction transaction = event_mng->transactions.create_transaction();
  smc_ctxt.transaction_id     = transaction.id();

  // Get selected security algorithms.
  security::sec_selected_algos security_algos = cu_cp_ue_notifier.get_security_algos();

  // Pack SecurityModeCommand.
  dl_dcch_msg_s dl_dcch_msg;
  dl_dcch_msg.msg.set_c1().set_security_mode_cmd().crit_exts.set_security_mode_cmd();
  fill_asn1_rrc_smc_msg(dl_dcch_msg.msg.c1().security_mode_cmd(),
                        security_algos.integ_algo,
                        security_algos.cipher_algo,
                        smc_ctxt.transaction_id);

  // Pack DL DCCH msg.
  pdcp_tx_result pdcp_packing_result =
      context.get_pdcp_notifier(srb_id_t::srb1)->on_new_pdu(pack_into_pdu(dl_dcch_msg, "SecurityModeCommand"));
  if (!pdcp_packing_result.is_successful()) {
    logger.log_info("Requesting UE release. Cause: PDCP packing failed with {}",
                    pdcp_packing_result.get_failure_cause());
    on_ue_release_required(pdcp_packing_result.get_failure_cause());
    return smc_ctxt;
  }

  smc_ctxt.rrc_ue_security_mode_command_pdu = pdcp_packing_result.pop_pdu();
  smc_ctxt.sp_cell_id                       = context.cell.cgi;

  // Log Tx message.
  log_rrc_message(logger, Tx, smc_ctxt.rrc_ue_security_mode_command_pdu, dl_dcch_msg, srb_id_t::srb1, "DCCH DL");

  return smc_ctxt;
}

async_task<bool> rrc_ue_impl::handle_security_mode_complete_expected(uint8_t transaction_id)
{
  return launch_async([this,
                       timeout_ms = context.cfg.rrc_procedure_guard_time_ms,
                       transaction_id,
                       transaction = rrc_transaction{}](coro_context<async_task<bool>>& ctx) mutable {
    CORO_BEGIN(ctx);

    logger.log_debug("Awaiting RRC Security Mode Complete (timeout={}ms)", timeout_ms.count());
    // Create new transaction for RRC Security Mode Command procedure.
    transaction = event_mng->transactions.create_transaction(transaction_id, timeout_ms);

    CORO_AWAIT(transaction);

    if (!transaction.has_response()) {
      logger.log_debug("Did not receive RRC Security Mode Complete. Cause: {}",
                       transaction.failure_cause() == protocol_transaction_failure::timeout ? "timeout" : "canceled");
      CORO_EARLY_RETURN(false);
    }

    // A UE may answer with any message carrying this transaction id, including one from the message class extension.
    if (transaction.response().msg.type().value != ul_dcch_msg_type_c::types_opts::c1) {
      logger.log_warning("Received an unexpected message in place of RRC Security Mode Complete");
      CORO_EARLY_RETURN(false);
    }

    if (transaction.response().msg.c1().type() == ul_dcch_msg_type_c::c1_c_::types_opts::security_mode_fail) {
      logger.log_warning("Received RRC Security Mode Failure");
      CORO_EARLY_RETURN(false);
    }

    if (transaction.response().msg.c1().type() == ul_dcch_msg_type_c::c1_c_::types_opts::security_mode_complete) {
      logger.log_debug("Received RRC Security Mode Complete");
      handle_security_mode_complete(transaction.response().msg.c1().security_mode_complete());
    }

    CORO_RETURN(true);
  });
}

bool rrc_ue_impl::verify_reestablishment_short_mac_i(const security::sec_short_mac_i& short_mac_i,
                                                     pci_t                            source_pci,
                                                     rnti_t                           source_c_rnti,
                                                     nr_cell_identity                 target_nci)
{
  return ocucp::verify_short_mac_i(
      short_mac_i, source_pci, source_c_rnti, target_nci, cu_cp_ue_notifier.get_security_context(), logger);
}

bool rrc_ue_impl::verify_resume_mac_i(const security::sec_short_mac_i& resume_mac_i, nr_cell_identity target_nci)
{
  return ocucp::verify_resume_mac_i(
      resume_mac_i, context.cell.pci, context.c_rnti, target_nci, cu_cp_ue_notifier.get_security_context(), logger);
}

byte_buffer rrc_ue_impl::get_packed_ue_capability_rat_container_list() const
{
  byte_buffer pdu{};

  if (context.capabilities_list.has_value()) {
    asn1::bit_ref bref{pdu};

    if (pack_dyn_seq_of(bref, context.capabilities_list.value(), 0, 8) != asn1::OCUDUASN_SUCCESS) {
      logger.log_error("Error packing UECapabilityRATContainer List");
      return byte_buffer{};
    }
  } else {
    logger.log_debug("No UE capabilites available");
  }

  return pdu.copy();
}

byte_buffer rrc_ue_impl::get_packed_ue_radio_access_cap_info() const
{
  asn1::rrc_nr::ue_radio_access_cap_info_s      ue_radio_access_cap_info;
  asn1::rrc_nr::ue_radio_access_cap_info_ies_s& ue_radio_access_cap_info_ies =
      ue_radio_access_cap_info.crit_exts.set_c1().set_ue_radio_access_cap_info();
  ue_radio_access_cap_info_ies.ue_radio_access_cap_info = get_packed_ue_capability_rat_container_list();

  return pack_into_pdu(ue_radio_access_cap_info, "UE Radio Access Cap Info");
}

bool rrc_ue_impl::handle_rrc_handover_preparation_info(byte_buffer pdu)
{
  // Unpack Handover Preparation Info.
  asn1::rrc_nr::ho_prep_info_s ho_prep_info;
  asn1::cbit_ref               bref({pdu.begin(), pdu.end()});

  if (ho_prep_info.unpack(bref) != asn1::OCUDUASN_SUCCESS) {
    logger.log_error("Couldn't unpack HandoverPreparationInfo RRC container");
    return false;
  }

  // Store UE capabilities.
  auto& ue_capabilities_list = ho_prep_info.crit_exts.c1().ho_prep_info().ue_cap_rat_list;

  context.capabilities_list.emplace(ue_capabilities_list);

  // Parse UE capabilities, so capability checks work on the target.
  if (std::optional<rrc_ue_capabilities_t> caps = get_capabilities(ue_capabilities_list, logger); caps.has_value()) {
    context.capabilities = caps.value();
  }

  if (logger.get_basic_logger().debug.enabled()) {
    logger.log_debug("UE Capabilities:");
    asn1::json_writer json_writer;
    for (const auto& rat_container : ue_capabilities_list) {
      rat_container.to_json(json_writer);
      logger.log_debug("{}", json_writer.to_string().c_str());
    }
  }

  // TODO: handle optional fields.

  return true;
}

async_task<bool> rrc_ue_impl::handle_rrc_reconfiguration_request(const rrc_reconfiguration_procedure_request& msg)
{
  return launch_async<rrc_reconfiguration_procedure>(
      context, msg, *this, *event_mng, get_rrc_ue_control_message_handler(), logger);
}

rrc_ue_handover_reconfiguration_context
rrc_ue_impl::get_rrc_ue_handover_reconfiguration_context(const rrc_reconfiguration_procedure_request& request)
{
  rrc_ue_handover_reconfiguration_context ho_reconf_ctxt;

  if (!context.pdcp_manager.has_srb(srb_id_t::srb1)) {
    logger.log_error("Can't get handover reconfiguration context. {} is not set up", srb_id_t::srb1);
    return ho_reconf_ctxt;
  }

  // Create transaction to get transaction ID.
  rrc_transaction transaction   = event_mng->transactions.create_transaction();
  ho_reconf_ctxt.transaction_id = transaction.id();

  if (request.is_cho_preparation) {
    // CHO Preparation: Plain ASN.1, no DL-DCCH wrapper, no PDCP. Embedded in condRRCReconfiguration-r16.
    // Extract T304 from MasterCellGroup so the target execution routine can use it for the reconfiguration timeout.
    if (request.non_crit_ext.has_value() && !request.non_crit_ext->master_cell_group.empty()) {
      asn1::rrc_nr::cell_group_cfg_s cell_group_cfg;
      asn1::cbit_ref                 bref(request.non_crit_ext->master_cell_group);
      if (cell_group_cfg.unpack(bref) == asn1::OCUDUASN_SUCCESS && cell_group_cfg.sp_cell_cfg_present &&
          cell_group_cfg.sp_cell_cfg.recfg_with_sync_present) {
        context.cell.timers.t304 =
            std::chrono::milliseconds{cell_group_cfg.sp_cell_cfg.recfg_with_sync.t304.to_number()};
      }
    }
    rrc_recfg_s rrc_recfg;
    rrc_recfg.crit_exts.set_rrc_recfg();
    fill_asn1_rrc_reconfiguration_msg(rrc_recfg, ho_reconf_ctxt.transaction_id, request);
    ho_reconf_ctxt.rrc_ue_handover_reconfiguration_pdu = pack_into_pdu(rrc_recfg, "CHO Candidate RRCReconfiguration");
    logger.log_debug("ue={} CHO candidate: plain ASN.1 RRCReconfiguration (tid={}, size={})",
                     context.ue_index,
                     ho_reconf_ctxt.transaction_id,
                     ho_reconf_ctxt.rrc_ue_handover_reconfiguration_pdu.length());
  } else {
    // Regular handover: DL-DCCH wrapped + PDCP protected.
    dl_dcch_msg_s dl_dcch_msg;
    dl_dcch_msg.msg.set_c1().set_rrc_recfg().crit_exts.set_rrc_recfg();
    fill_asn1_rrc_reconfiguration_msg(dl_dcch_msg.msg.c1().rrc_recfg(), ho_reconf_ctxt.transaction_id, request);
    pdcp_tx_result result =
        context.get_pdcp_notifier(srb_id_t::srb1)->on_new_pdu(pack_into_pdu(dl_dcch_msg, "RRCReconfiguration"));
    if (!result.is_successful()) {
      logger.log_info("Requesting UE release. Cause: PDCP packing failed with {}", result.get_failure_cause());
      on_ue_release_required(result.get_failure_cause());
      return ho_reconf_ctxt;
    }
    ho_reconf_ctxt.rrc_ue_handover_reconfiguration_pdu = result.pop_pdu();
    log_rrc_message(
        logger, Tx, ho_reconf_ctxt.rrc_ue_handover_reconfiguration_pdu, dl_dcch_msg, srb_id_t::srb1, "DCCH DL");
  }
  return ho_reconf_ctxt;
}

rrc_ue_cond_reconfiguration_context
rrc_ue_impl::get_rrc_ue_cond_reconfiguration_context(const rrc_reconfiguration_procedure_request& request)
{
  rrc_ue_cond_reconfiguration_context cond_reconf_ctxt;

  if (!context.pdcp_manager.has_srb(srb_id_t::srb1)) {
    logger.log_error("Can't get CHO reconfiguration context. {} is not set up", srb_id_t::srb1);
    return cond_reconf_ctxt;
  }

  // Create transaction to get transaction ID.
  rrc_transaction transaction     = event_mng->transactions.create_transaction();
  cond_reconf_ctxt.transaction_id = transaction.id();

  // Build RRCReconfiguration with measConfig and conditionalReconfiguration-r16.
  dl_dcch_msg_s dl_dcch_msg;
  auto&         rrc_recfg        = dl_dcch_msg.msg.set_c1().set_rrc_recfg();
  rrc_recfg.rrc_transaction_id   = cond_reconf_ctxt.transaction_id;
  auto&                recfg_ies = rrc_recfg.crit_exts.set_rrc_recfg();
  std::vector<uint8_t> cho_meas_ids;

  // CHO measurement config (already filtered and includes conditional triggers).
  // The measurement config from generate_meas_config() is already complete:
  // - Measurement objects filtered to candidate target cells
  // - Conditional trigger report configs included in report_cfg_to_add_mod_list
  // - Measurement IDs correctly linking filtered MOs to conditional triggers
  if (request.meas_cfg.has_value()) {
    recfg_ies.meas_cfg_present = true;
    recfg_ies.meas_cfg         = meas_config_to_rrc_asn1(request.meas_cfg.value());

    // Include measurement gap config so the UE can perform inter-frequency measurements
    // during CHO condition evaluation (before any handover executes).
    if (!request.meas_gap_cfg.empty()) {
      recfg_ies.meas_cfg.meas_gap_cfg_present = true;
      asn1::cbit_ref bref(request.meas_gap_cfg);
      if (recfg_ies.meas_cfg.meas_gap_cfg.unpack(bref) != asn1::OCUDUASN_SUCCESS) {
        logger.log_warning("ue={}: Failed to decode measGapConfig for outer CHO RRCReconfiguration", context.ue_index);
        recfg_ies.meas_cfg.meas_gap_cfg_present = false;
      }
    }

    logger.log_debug("ue={}: Using CHO-specific measConfig with condTriggerConfig-r16 - {} meas_obj, {} "
                     "report_cfg, {} meas_id",
                     context.ue_index,
                     recfg_ies.meas_cfg.meas_obj_to_add_mod_list.size(),
                     recfg_ies.meas_cfg.report_cfg_to_add_mod_list.size(),
                     recfg_ies.meas_cfg.meas_id_to_add_mod_list.size());

    if (recfg_ies.meas_cfg.meas_obj_to_add_mod_list.size() == 0) {
      logger.log_warning("ue={}: CHO measConfig has no measurement objects; conditional execution will not trigger",
                         context.ue_index);
    }
  } else {
    logger.log_warning("ue={}: CHO RRCReconfiguration has no measConfig - UE may not have measurement conditions",
                       context.ue_index);
  }

  // Set up non-critical extensions chain to reach v1610 for conditionalReconfiguration.
  recfg_ies.non_crit_ext_present = true;
  auto& v1530                    = recfg_ies.non_crit_ext;
  v1530.non_crit_ext_present     = true;
  auto& v1540                    = v1530.non_crit_ext;
  v1540.non_crit_ext_present     = true;
  auto& v1560                    = v1540.non_crit_ext;
  v1560.non_crit_ext_present     = true;
  auto& v1610                    = v1560.non_crit_ext;

  // Add conditionalReconfiguration-r16 if CHO candidates are provided.
  if (request.cho_candidates.has_value() && !request.cho_candidates->empty()) {
    v1610.conditional_recfg_r16_present       = true;
    auto& cond_recfg                          = v1610.conditional_recfg_r16;
    cond_recfg.attempt_cond_recfg_r16_present = true;

    // Add each candidate with its specific measIds based on target NCI.
    for (const auto& candidate : request.cho_candidates.value()) {
      cond_recfg_to_add_mod_r16_s entry;
      entry.cond_recfg_id_r16 = candidate.cond_recfg_id.value();

      // Set execution condition (measurement ID reference) based on target NCI.
      if (request.cho_nci_to_meas_ids.has_value()) {
        auto it = request.cho_nci_to_meas_ids->find(candidate.target_cgi.nci);
        if (it != request.cho_nci_to_meas_ids->end()) {
          const auto& meas_ids = it->second;

          // Validate ASN.1 constraint: max 2 measIds per 3GPP TS 38.331.
          if (meas_ids.size() > 2) {
            logger.log_error("ue={}: CHO candidate cond_recfg_id={} target_nci={:#x} has {} measIds (max 2 allowed). "
                             "Using first 2 only.",
                             context.ue_index,
                             candidate.cond_recfg_id,
                             candidate.target_cgi.nci.value(),
                             meas_ids.size());
          }

          // Add up to 2 measIds.
          for (size_t i = 0; i < std::min(meas_ids.size(), size_t{2}); ++i) {
            entry.cond_execution_cond_r16.push_back(to_underlying(meas_ids[i]));
          }

          logger.log_debug("ue={}: CHO candidate cond_recfg_id={} target_nci={:#x} assigned {} measId(s): {}",
                           context.ue_index,
                           candidate.cond_recfg_id,
                           candidate.target_cgi.nci.value(),
                           entry.cond_execution_cond_r16.size(),
                           fmt::format("{}", fmt::join(entry.cond_execution_cond_r16, ", ")));
        } else {
          logger.log_warning("ue={}: CHO candidate cond_recfg_id={} target_nci={:#x} not found in measId mapping. "
                             "Using fallback measId=1.",
                             context.ue_index,
                             candidate.cond_recfg_id,
                             candidate.target_cgi.nci.value());
          entry.cond_execution_cond_r16.push_back(1);
        }
      } else {
        // Fallback if no mapping provided.
        logger.log_warning("ue={}: No CHO measId mapping provided. Using fallback measId=1 for cond_recfg_id={}",
                           context.ue_index,
                           candidate.cond_recfg_id);
        entry.cond_execution_cond_r16.push_back(1);
      }

      // Set the prepared RRC reconfiguration for this candidate.
      if (!candidate.prepared_rrc_recfg.empty()) {
        entry.cond_rrc_recfg_r16 = candidate.prepared_rrc_recfg.copy();
      }

      cond_recfg.cond_recfg_to_add_mod_list_r16.push_back(entry);
    }
  }

  // Pack DL DCCH message.
  pdcp_tx_result pdcp_packing_result =
      context.get_pdcp_notifier(srb_id_t::srb1)->on_new_pdu(pack_into_pdu(dl_dcch_msg, "CHO RRCReconfiguration"));

  if (!pdcp_packing_result.is_successful()) {
    logger.log_info("Requesting UE release. Cause: PDCP packing failed with {}",
                    pdcp_packing_result.get_failure_cause());
    on_ue_release_required(pdcp_packing_result.get_failure_cause());
    return cond_reconf_ctxt;
  }

  cond_reconf_ctxt.rrc_ue_cond_reconfiguration_pdu = pdcp_packing_result.pop_pdu();

  // Log Tx message.
  log_rrc_message(logger, Tx, cond_reconf_ctxt.rrc_ue_cond_reconfiguration_pdu, dl_dcch_msg, srb_id_t::srb1, "DCCH DL");

  return cond_reconf_ctxt;
}

async_task<bool> rrc_ue_impl::handle_handover_reconfiguration_complete_expected(uint8_t transaction_id,
                                                                                std::chrono::milliseconds timeout_ms,
                                                                                bool release_on_failure)
{
  return launch_async([this, timeout_ms, transaction_id, release_on_failure, transaction = rrc_transaction{}](
                          coro_context<async_task<bool>>& ctx) mutable {
    CORO_BEGIN(ctx);

    logger.log_debug("Awaiting RRC Reconfiguration Complete (timeout={}ms)", timeout_ms.count());
    // Create new transaction for RRC Reconfiguration procedure.
    transaction = event_mng->transactions.create_transaction(transaction_id, timeout_ms);

    CORO_AWAIT(transaction);

    bool procedure_result = false;
    if (transaction.has_response()) {
      logger.log_debug("Received RRC Reconfiguration Complete after HO");
      procedure_result = true;

      // Only a handover target UE enters connected state here. A CHO source UE that acknowledges its conditional
      // reconfiguration is already in connected state.
      if (context.state != rrc_state::connected) {
        // The UE in the target cell is in connected state on RRCReconfigurationComplete reception.
        context.state = rrc_state::connected;

        // A handover target serves the UE from a new RRC UE, with no coarse location and no Security Mode Command to
        // ask for one. Security is already active, which TS 38.300 sec. 16.14.8 gates the request on.
        request_coarse_ue_location();

        // Notify metrics.
        metrics_notifier.on_new_rrc_connection();
      }

    } else {
      // A cancellation is an expected outcome of a higher layer aborting the procedure, so it is not logged as a
      // warning.
      const bool        timed_out = transaction.failure_cause() == protocol_transaction_failure::timeout;
      const auto        level     = timed_out ? ocudulog::basic_levels::warning : ocudulog::basic_levels::info;
      const std::string cause_str = timed_out ? fmt::format("timeout ({}ms)", timeout_ms.count()) : "canceled";
      if (release_on_failure) {
        logger.log(level,
                   "Did not receive RRC Reconfiguration Complete after HO. Cause: {}. Requesting target UE release",
                   cause_str);
        on_ue_release_required(ngap_cause_radio_network_t::ho_fail_in_target_5_gc_ngran_node_or_target_sys);
      } else {
        logger.log(level,
                   "Did not receive RRC Reconfiguration Complete after HO. Cause: {}. UE release handled externally",
                   cause_str);
      }
    }

    CORO_RETURN(procedure_result);
  });
}

bool rrc_ue_impl::store_ue_capabilities(byte_buffer ue_capabilities)
{
  // Unpack UE capabilities.
  asn1::rrc_nr::ue_radio_access_cap_info_s ue_radio_access_cap_info;
  asn1::cbit_ref                           bref({ue_capabilities.begin(), ue_capabilities.end()});

  if (ue_radio_access_cap_info.unpack(bref) != asn1::OCUDUASN_SUCCESS) {
    logger.log_error("Couldn't unpack UERadioAccessCapabilityInfo RRC container");
    return false;
  }

  asn1::rrc_nr::ue_cap_rat_container_list_l ue_cap_rat_container_list;
  asn1::cbit_ref                            bref2(
      {ue_radio_access_cap_info.crit_exts.c1().ue_radio_access_cap_info().ue_radio_access_cap_info.begin(),
                                  ue_radio_access_cap_info.crit_exts.c1().ue_radio_access_cap_info().ue_radio_access_cap_info.end()});

  if (asn1::unpack_dyn_seq_of(ue_cap_rat_container_list, bref2, 0, 8) != asn1::OCUDUASN_SUCCESS) {
    logger.log_error("Couldn't unpack UE Capability RAT Container List RRC container");
    return false;
  }

  context.capabilities_list.emplace(ue_cap_rat_container_list);

  // Parse UE capabilities for the capability checks.
  if (std::optional<rrc_ue_capabilities_t> caps = get_capabilities(ue_cap_rat_container_list, logger);
      caps.has_value()) {
    context.capabilities = caps.value();
  }

  if (logger.get_basic_logger().debug.enabled()) {
    logger.log_debug("UE Capabilities:\n");
    asn1::json_writer json_writer;
    for (const auto& rat_container : ue_cap_rat_container_list) {
      rat_container.to_json(json_writer);
      logger.log_debug("{}\n", json_writer.to_string().c_str());
    }
  }

  return true;
}

async_task<bool> rrc_ue_impl::handle_rrc_ue_capability_transfer_request(const rrc_ue_capability_transfer_request& msg)
{
  // Launch RRC UE capability transfer procedure.
  return launch_async<rrc_ue_capability_transfer_procedure>(context, *this, *event_mng, logger);
}

rrc_ue_release_context
rrc_ue_impl::get_rrc_ue_release_context(bool                                          requires_rrc_message,
                                        std::optional<std::chrono::seconds>           release_wait_time,
                                        std::optional<rrc_inactivity_context>         inactivity_context,
                                        std::optional<cu_cp_release_redirect_nr_info> redirect_nr_info)
{
  // Prepare location info to return.
  rrc_ue_release_context release_context;
  release_context.user_location_info.nr_cgi   = {context.plmn_id, context.cell.cgi.nci};
  release_context.user_location_info.tai      = {context.plmn_id, context.cell.tac};
  release_context.user_location_info.tac_list = context.cell.tac_list;
  fill_ue_derived_location(release_context.user_location_info);

  if (requires_rrc_message) {
    if (context.pdcp_manager.get_srb_ids().empty()) {
      // SRB1 was not created, so we need to reject the UE.
      // Create and RRCReject container, see section 5.3.15 in TS 38.331.
      dl_ccch_msg_s dl_ccch_msg;
      // SRB1 was not created, so we create a RRC Container with RRCReject.
      rrc_reject_ies_s& reject = dl_ccch_msg.msg.set_c1().set_rrc_reject().crit_exts.set_rrc_reject();
      // The RRC Reject always includes the waitTime field (TS 38.331 section 6.2.2, RRCReject-IEs field descriptions).
      reject.wait_time_present = true;
      reject.wait_time         = context.cfg.rrc_reject_wait_time.count();

      // Pack DL CCCH msg.
      release_context.rrc_pdu = pack_into_pdu(dl_ccch_msg, "RRCReject");
      release_context.srb_id  = srb_id_t::srb0;

      // Log Tx message.
      log_rrc_message(logger, Tx, release_context.rrc_pdu, dl_ccch_msg, srb_id_t::srb0, "CCCH DL");
    } else {
      // Prepare SRB1 RRC Release PDU to return.
      if (!context.pdcp_manager.has_srb(srb_id_t::srb1)) {
        logger.log_error("Can't create RRCRelease PDU. Rx {} is not set up", srb_id_t::srb1);
        return release_context;
      }

      dl_dcch_msg_s      dl_dcch_msg;
      rrc_release_ies_s& release = dl_dcch_msg.msg.set_c1().set_rrc_release().crit_exts.set_rrc_release();
      if (release_wait_time.has_value()) {
        release.non_crit_ext_present = true;
        // If wait time is provided, set it.
        release.non_crit_ext.wait_time_present = true;
        release.non_crit_ext.wait_time         = release_wait_time.value().count();
      }
      if (inactivity_context.has_value()) {
        release.suspend_cfg_present = true;
        // Set Full-I-RNTI.
        release.suspend_cfg.full_i_rnti.from_number(inactivity_context->i_rntis.full_i_rnti.value());

        // Set Short-I-RNTI.
        release.suspend_cfg.short_i_rnti.from_number(inactivity_context->i_rntis.short_i_rnti.value());

        // Set RAN paging cycle.
        release.suspend_cfg.ran_paging_cycle = ran_paging_cycle_to_asn1(inactivity_context->ran_paging_cycle);

        // Set next hop chaining count.
        release.suspend_cfg.next_hop_chaining_count = inactivity_context->next_hop_chaining_count;

        // Set RAN nofication area info.
        release.suspend_cfg.ran_notif_area_info_present = true;
        release.suspend_cfg.ran_notif_area_info =
            ran_notification_area_info_to_asn1(inactivity_context->ran_notification_area_info);

        // Set t380 timer value.
        release.suspend_cfg.t380_present = true;
        asn1::number_to_enum(release.suspend_cfg.t380, inactivity_context->t380.count());
      }

      if (redirect_nr_info.has_value()) {
        release.redirected_carrier_info_present = true;
        auto& nr_info                           = release.redirected_carrier_info.set_nr();
        nr_info.carrier_freq                    = redirect_nr_info->arfcn;
        nr_info.ssb_subcarrier_spacing          = subcarrier_spacing_to_rrc_asn1(redirect_nr_info->ssb_scs);
      }

      // Pack DL DCCH msg.
      pdcp_tx_result pdcp_packing_result =
          context.get_pdcp_notifier(srb_id_t::srb1)->on_new_pdu(pack_into_pdu(dl_dcch_msg, "RRCRelease"));
      if (!pdcp_packing_result.is_successful()) {
        logger.log_info("Requesting UE release. Cause: PDCP packing failed with {}",
                        pdcp_packing_result.get_failure_cause());
        on_ue_release_required(pdcp_packing_result.get_failure_cause());
        return release_context;
      }

      release_context.rrc_pdu = pdcp_packing_result.pop_pdu();
      release_context.srb_id  = srb_id_t::srb1;

      // Log Tx message.
      log_rrc_message(logger, Tx, release_context.rrc_pdu, dl_dcch_msg, srb_id_t::srb1, "DCCH DL");
    }

    // Log Tx message.
    logger.log_debug(
        release_context.rrc_pdu.begin(), release_context.rrc_pdu.end(), "Tx {} PDU", release_context.srb_id);
  }

  return release_context;
}

std::optional<rrc_meas_cfg> rrc_ue_impl::generate_meas_config(const std::optional<rrc_meas_cfg>& current_meas_config,
                                                              bool                               cond_meas,
                                                              span<const pci_t>                  candidate_pcis)
{
  auto result = measurement_notifier.on_measurement_config_request(
      context.cell.cgi.nci, current_meas_config, cond_meas, candidate_pcis);

  if (!cond_meas) {
    // A config carrying only removals leaves the UE without measurement configuration once applied.
    const bool removal_only = result.has_value() && result.value().meas_obj_to_add_mod_list.empty() &&
                              result.value().meas_id_to_add_mod_list.empty() &&
                              result.value().report_cfg_to_add_mod_list.empty();
    // Store regular meas config and derive serving cell MO.
    context.meas_cfg = removal_only ? std::nullopt : result;
    if (removal_only) {
      context.serving_cell_mo = std::nullopt;
    }
    if (context.meas_cfg.has_value()) {
      for (const auto& meas_obj : context.meas_cfg.value().meas_obj_to_add_mod_list) {
        if (meas_obj.meas_obj_nr.has_value() && meas_obj.meas_obj_nr.value().ssb_freq == context.cell.ssb_arfcn) {
          context.serving_cell_mo = to_underlying(meas_obj.meas_obj_id);
          break;
        }
      }
    }
  }

  return result;
}

byte_buffer rrc_ue_impl::get_packed_meas_config(span<const pci_t> candidate_pcis)
{
  if (!candidate_pcis.empty()) {
    auto cfg = generate_meas_config(context.meas_cfg, true, candidate_pcis);
    if (!cfg.has_value()) {
      return {};
    }
    // Convert to ASN1, pack and return.
    return pack_into_pdu(meas_config_to_rrc_asn1(cfg.value()), "RRCMeasConfig");
  }

  // (Re-)generate regular measurement config and update stored context.
  generate_meas_config(context.meas_cfg);
  if (context.meas_cfg.has_value()) {
    // Convert to ASN1, pack and return.
    return pack_into_pdu(meas_config_to_rrc_asn1(context.meas_cfg.value()), "RRCMeasConfig");
  }

  return {};
}

void rrc_ue_impl::update_meas_config(const rrc_meas_cfg& cfg)
{
  context.meas_cfg = cfg;
  // Re-derive the serving cell measurement object ID from the new config.
  if (context.meas_cfg.has_value()) {
    for (const auto& meas_obj : context.meas_cfg.value().meas_obj_to_add_mod_list) {
      if (meas_obj.meas_obj_nr.has_value() && meas_obj.meas_obj_nr->ssb_freq == context.cell.ssb_arfcn) {
        context.serving_cell_mo = to_underlying(meas_obj.meas_obj_id);
        break;
      }
    }
  }
}

std::optional<uint8_t> rrc_ue_impl::get_serving_cell_mo()
{
  // If the measurement config was never generated, generate it now.
  if (!context.meas_cfg.has_value()) {
    context.meas_cfg = generate_meas_config(std::nullopt);
  }

  return context.serving_cell_mo;
}

rrc_ue_transfer_context rrc_ue_impl::get_transfer_context()
{
  rrc_ue_transfer_context transfer_context;
  transfer_context.sec_context               = cu_cp_ue_notifier.get_security_context();
  transfer_context.meas_cfg                  = context.meas_cfg;
  transfer_context.srbs                      = get_srbs();
  transfer_context.up_ctx                    = cu_cp_notifier.on_up_context_required();
  transfer_context.handover_preparation_info = get_packed_handover_preparation_message();
  transfer_context.ue_cap_rat_container_list = get_packed_ue_capability_rat_container_list();
  transfer_context.location_meas_info        = get_packed_location_meas_info();

  return transfer_context;
}

rrc_ue_reestablishment_context_response rrc_ue_impl::get_context()
{
  rrc_ue_reestablishment_context_response rrc_reest_context;
  rrc_reest_context.sec_context = cu_cp_ue_notifier.get_security_context();

  if (context.capabilities_list.has_value()) {
    rrc_reest_context.capabilities_list = asn1_to_ue_cap_rat_container_list(context.capabilities_list.value());
  }
  rrc_reest_context.up_ctx             = cu_cp_notifier.on_up_context_required();
  rrc_reest_context.location_meas_info = get_packed_location_meas_info();

  // TODO: Handle scenario with multiple reestablishments for the same UE.
  rrc_reest_context.reestablishment_ongoing = context.reestablishment_ongoing;

  // If no reestablishment is ongoing, set it to true.
  if (not context.reestablishment_ongoing) {
    context.reestablishment_ongoing = true;
  }

  return rrc_reest_context;
}

byte_buffer rrc_ue_impl::get_rrc_handover_command(const rrc_reconfiguration_procedure_request& request,
                                                  unsigned                                     transaction_id)
{
  // Unpack MasterCellGroup to extract T304.
  asn1::rrc_nr::cell_group_cfg_s cell_group_cfg;
  asn1::cbit_ref                 bref2(request.non_crit_ext->master_cell_group);
  if (cell_group_cfg.unpack(bref2) != asn1::OCUDUASN_SUCCESS) {
    report_fatal_error("Failed to unpack MasterCellGroupCfg");
  }
  context.cell.timers.t304 = std::chrono::milliseconds{cell_group_cfg.sp_cell_cfg.recfg_with_sync.t304.to_number()};

  // Pack RRCReconfiguration.
  rrc_recfg_s rrc_reconfig;
  fill_asn1_rrc_reconfiguration_msg(rrc_reconfig, transaction_id, request);
  byte_buffer reconfig_pdu = pack_into_pdu(rrc_reconfig, "RRCReconfiguration");

  ho_cmd_s ho_cmd;
  ho_cmd.crit_exts.set_c1().set_ho_cmd().ho_cmd_msg = reconfig_pdu.copy();

  // Pack HandoverCommand.
  byte_buffer ho_cmd_pdu = pack_into_pdu(ho_cmd, "RRCHandoverCommand");

  // Log message.
  logger.log_debug(ho_cmd_pdu.begin(), ho_cmd_pdu.end(), "RRCHandoverCommand ({} B)", ho_cmd_pdu.length());
  if (logger.get_basic_logger().debug.enabled()) {
    asn1::json_writer js;
    ho_cmd.to_json(js);
    logger.log_debug("Containerized RRCHandoverCommand: {}", js.to_string());
  }

  return ho_cmd_pdu;
}

byte_buffer rrc_ue_impl::handle_rrc_handover_command(byte_buffer cmd)
{
  byte_buffer ho_reconf_pdu = byte_buffer{};

  // Unpack HandoverCommand.
  asn1::rrc_nr::ho_cmd_s handover_command;
  asn1::cbit_ref         bref({cmd.begin(), cmd.end()});

  if (handover_command.unpack(bref) != asn1::OCUDUASN_SUCCESS) {
    logger.log_error("Couldn't unpack Handover Command RRC container");
    return ho_reconf_pdu;
  }

  // Unpack RRCReconfiguration to new DL DCCH message.
  asn1::cbit_ref bref2({handover_command.crit_exts.c1().ho_cmd().ho_cmd_msg.begin(),
                        handover_command.crit_exts.c1().ho_cmd().ho_cmd_msg.end()});

  dl_dcch_msg_s dl_dcch_msg;
  auto&         rrc_recfg = dl_dcch_msg.msg.set_c1().set_rrc_recfg();

  if (rrc_recfg.unpack(bref2) != asn1::OCUDUASN_SUCCESS) {
    logger.log_error("Couldn't unpack RRC Reconfiguration container");
    return ho_reconf_pdu;
  }

  // Pack DL DCCH msg.
  rrc_ue_pdcp_notifier* srb1_notifier = context.get_pdcp_notifier(srb_id_t::srb1);
  if (srb1_notifier == nullptr) {
    logger.log_error("Can't pack handover command. {} is not set up", srb_id_t::srb1);
    return ho_reconf_pdu;
  }
  pdcp_tx_result pdcp_packing_result = srb1_notifier->on_new_pdu(pack_into_pdu(dl_dcch_msg, "RRCReconfiguration"));
  if (!pdcp_packing_result.is_successful()) {
    logger.log_info("Requesting UE release. Cause: PDCP packing failed with {}",
                    pdcp_packing_result.get_failure_cause());
    on_ue_release_required(pdcp_packing_result.get_failure_cause());
    return ho_reconf_pdu;
  }

  ho_reconf_pdu = pdcp_packing_result.pop_pdu();

  // Log Tx message.
  log_rrc_message(logger, Tx, ho_reconf_pdu, dl_dcch_msg, srb_id_t::srb1, "DCCH DL");

  return ho_reconf_pdu;
}
