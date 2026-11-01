// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "rrc_setup_procedure.h"
#include "../rrc_asn1_helpers.h"
#include "ue/rrc_asn1_converters.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/dl_ccch_msg.h"
#include "ocudu/ran/cause/common.h"
#include <variant>

using namespace ocudu;
using namespace ocudu::ocucp;
using namespace asn1::rrc_nr;

rrc_setup_procedure::rrc_setup_procedure(rrc_ue_context_t&               context_,
                                         const byte_buffer&              du_to_cu_container_,
                                         rrc_ue_setup_proc_notifier&     rrc_ue_notifier_,
                                         rrc_ue_control_message_handler& srb_notifier_,
                                         rrc_ue_context_update_notifier& cu_cp_notifier_,
                                         rrc_ue_event_notifier&          metrics_notifier_,
                                         rrc_ue_ngap_notifier&           ngap_notifier_,
                                         rrc_ue_event_manager&           event_mng_,
                                         rrc_ue_logger&                  logger_,
                                         bool                            is_reestablishment_fallback_,
                                         bool                            is_resume_fallback_) :
  context(context_),
  du_to_cu_container(du_to_cu_container_),
  rrc_ue(rrc_ue_notifier_),
  srb_notifier(srb_notifier_),
  cu_cp_notifier(cu_cp_notifier_),
  metrics_notifier(metrics_notifier_),
  ngap_notifier(ngap_notifier_),
  event_mng(event_mng_),
  is_reestablishment_fallback(is_reestablishment_fallback_),
  is_resume_fallback(is_resume_fallback_),
  logger(logger_)
{
  procedure_timeout = context.cell.timers.t300 + context.cfg.rrc_procedure_guard_time_ms;
}

void rrc_setup_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  logger.log_info("\"{}\" started...", name());

  // create SRB1
  create_srb1();

  // create new transaction for RRCSetup
  transaction = event_mng.transactions.create_transaction(procedure_timeout);

  // send RRC setup to UE
  send_rrc_setup();

  // Await UE response
  CORO_AWAIT(transaction);

  if (!transaction.has_response()) {
    switch (transaction.failure_cause()) {
      case protocol_transaction_failure::timeout:
        logger.log_warning("\"{}\" timed out after {}ms", name(), procedure_timeout.count());
        metrics_notifier.on_failed_rrc_connection_establishment(establishment_fail_cause_t::no_reply);
        rrc_ue.on_ue_release_required(cause_protocol_t::unspecified);
        break;
      case protocol_transaction_failure::abnormal:
        logger.log_info("\"{}\" cancelled due to a UE-specific failure", name());
        metrics_notifier.on_failed_rrc_connection_establishment(establishment_fail_cause_t::no_reply);
        break;
      case protocol_transaction_failure::cancel:
      default:
        logger.log_info("\"{}\" cancelled", name());
        // Do nothing. We are likely shutting down the DU processor.
        break;
    }
    CORO_EARLY_RETURN();
  }
  const auto& resp = transaction.response();
  if (resp.msg.type().value != ul_dcch_msg_type_c::types_opts::c1 or
      resp.msg.c1().type().value != ul_dcch_msg_type_c::c1_c_::types_opts::rrc_setup_complete) {
    logger.log_warning("Unexpected UL-DCCH message type {} in RRC Setup Procedure",
                       static_cast<int>(resp.msg.type().value));
    metrics_notifier.on_failed_rrc_connection_establishment(establishment_fail_cause_t::no_reply);
    rrc_ue.on_ue_release_required(cause_protocol_t::unspecified);
    CORO_EARLY_RETURN();
  }

  rrc_setup_complete_msg = transaction.response().msg.c1().rrc_setup_complete();

  // Store selected PLMN in the RRC UE context and in the CU-CP UE.
  // Note: The selected PLMN starts at 1.
  sel_plmn_id = rrc_setup_complete_msg.crit_exts.rrc_setup_complete().sel_plmn_id;
  if (sel_plmn_id == 0 || sel_plmn_id > context.cell.plmn_identity_list.size()) {
    logger.log_warning("Invalid selected PLMN id {} in RRC Setup Complete", sel_plmn_id);
    metrics_notifier.on_failed_rrc_connection_establishment(establishment_fail_cause_t::other);
    rrc_ue.on_ue_release_required(ngap_cause_radio_network_t::unspecified);
    CORO_EARLY_RETURN();
  }

  selected_plmn = context.cell.plmn_identity_list[sel_plmn_id - 1];

  // Notify the CU-CP about the selected PLMN and the identities that select the AMF.
  store_five_g_s_tmsi();
  if (!cu_cp_notifier.on_ue_setup_complete_received(get_amf_selection_request())) {
    logger.log_warning("No AMF for PLMN {} accepts the UE, rejecting UE", selected_plmn);
    metrics_notifier.on_failed_rrc_connection_establishment(establishment_fail_cause_t::other);
    rrc_ue.on_ue_release_required(ngap_cause_radio_network_t::unspecified);
    CORO_EARLY_RETURN();
  }
  // Store the selected PLMN in the RRC UE context.
  context.plmn_id = selected_plmn;

  context.state = rrc_state::connected;

  if (is_reestablishment_fallback) {
    // Notify metrics about successful RRC connection reestablishment fallback.
    metrics_notifier.on_successful_rrc_connection_reestablishment_fallback();
  } else if (is_resume_fallback) {
    // Notify metrics about successful RRC connection resume with fallback.
    metrics_notifier.on_successful_rrc_connection_resume_with_fallback(
        establishment_cause_to_resume_cause(context.connection_cause));
  } else {
    // Notify metrics about successful RRC connection establishment.
    metrics_notifier.on_successful_rrc_connection_establishment(
        context.connection_cause,
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time));
  }

  // Notify metrics about new RRC connection.
  metrics_notifier.on_new_rrc_connection();

  send_initial_ue_msg();

  logger.log_info("\"{}\" finished successfully", name());

  CORO_RETURN();
}

void rrc_setup_procedure::create_srb1()
{
  // create SRB1
  srb_creation_message srb1_msg{};
  srb1_msg.ue_index = context.ue_index;
  srb1_msg.srb_id   = srb_id_t::srb1;
  srb1_msg.pdcp_cfg = {};
  srb_notifier.create_srb(srb1_msg);
}

void rrc_setup_procedure::send_rrc_setup()
{
  dl_ccch_msg_s dl_ccch_msg;
  dl_ccch_msg.msg.set_c1().set_rrc_setup();
  rrc_setup_s& rrc_setup = dl_ccch_msg.msg.c1().rrc_setup();
  if (!fill_asn1_rrc_setup_msg(rrc_setup, du_to_cu_container, transaction.id())) {
    logger.log_warning("ASN1 RRC setup message fill failed");
    return;
  }
  rrc_ue.on_new_dl_ccch(dl_ccch_msg);
}

void rrc_setup_procedure::send_initial_ue_msg()
{
  cu_cp_initial_ue_message init_ue_msg = {};

  const auto& rrc_setup_complete = rrc_setup_complete_msg.crit_exts.rrc_setup_complete();

  init_ue_msg.ue_index                    = context.ue_index;
  init_ue_msg.nas_pdu                     = rrc_setup_complete.ded_nas_msg.copy();
  init_ue_msg.establishment_cause         = context.connection_cause;
  init_ue_msg.user_location_info.nr_cgi   = {context.plmn_id, context.cell.cgi.nci};
  init_ue_msg.user_location_info.tai      = {context.plmn_id, context.cell.tac};
  init_ue_msg.user_location_info.tac_list = context.cell.tac_list;

  if (context.five_g_s_tmsi.has_value()) {
    init_ue_msg.five_g_s_tmsi = context.five_g_s_tmsi.value();
  }

  ngap_notifier.on_initial_ue_message(init_ue_msg);
}

resume_cause_t
rrc_setup_procedure::establishment_cause_to_resume_cause(const establishment_cause_t& establishment_cause)
{
  switch (establishment_cause) {
    case establishment_cause_t::emergency:
      return resume_cause_t::emergency;
    case establishment_cause_t::high_prio_access:
      return resume_cause_t::high_prio_access;
    case establishment_cause_t::mt_access:
      return resume_cause_t::mt_access;
    case establishment_cause_t::mo_sig:
      return resume_cause_t::mo_sig;
    case establishment_cause_t::mo_data:
      return resume_cause_t::mo_data;
    case establishment_cause_t::mo_voice_call:
      return resume_cause_t::mo_voice_call;
    case establishment_cause_t::mo_video_call:
      return resume_cause_t::mo_video_call;
    case establishment_cause_t::mo_sms:
      return resume_cause_t::mo_sms;
    case establishment_cause_t::mps_prio_access:
      return resume_cause_t::mps_prio_access;
    case establishment_cause_t::mcs_prio_access:
      return resume_cause_t::mcs_prio_access;
    default:
      return resume_cause_t::unknown;
  }
}

void rrc_setup_procedure::store_five_g_s_tmsi()
{
  const auto& rrc_setup_complete = rrc_setup_complete_msg.crit_exts.rrc_setup_complete();
  if (rrc_setup_complete.ng_5_g_s_tmsi_value_present) {
    if (rrc_setup_complete.ng_5_g_s_tmsi_value.type() ==
        asn1::rrc_nr::rrc_setup_complete_ies_s::ng_5_g_s_tmsi_value_c_::types_opts::options::ng_5_g_s_tmsi) {
      context.five_g_s_tmsi = asn1_to_five_g_s_tmsi(rrc_setup_complete.ng_5_g_s_tmsi_value.ng_5_g_s_tmsi());
    } else {
      if (!std::holds_alternative<asn1::fixed_bitstring<39>>(context.setup_ue_id)) {
        logger.log_warning("5G-S-TMSI part 1 is missing");
      } else {
        context.five_g_s_tmsi = asn1_to_five_g_s_tmsi(std::get<asn1::fixed_bitstring<39>>(context.setup_ue_id),
                                                      rrc_setup_complete.ng_5_g_s_tmsi_value.ng_5_g_s_tmsi_part2());
      }
    }
  }
}

amf_selection_request rrc_setup_procedure::get_amf_selection_request() const
{
  const auto& rrc_setup_complete = rrc_setup_complete_msg.crit_exts.rrc_setup_complete();

  amf_selection_request request;
  request.plmn                = selected_plmn;
  request.establishment_cause = context.connection_cause;
  for (const auto& asn1_s_nssai : rrc_setup_complete.s_nssai_list) {
    request.requested_nssai.push_back(asn1_to_s_nssai(asn1_s_nssai));
  }

  // The UE sends either the 5G-S-TMSI or the registered GUAMI (TS 24.501 section 5.3.1.1).
  request.five_g_s_tmsi = context.five_g_s_tmsi;

  if (rrc_setup_complete.registered_amf_present) {
    // The PLMN identity of the registered AMF is absent if it is the selected PLMN (TS 38.331 section 5.3.3.4).
    plmn_identity registered_plmn = selected_plmn;
    if (rrc_setup_complete.registered_amf.plmn_id_present) {
      expected<plmn_identity> plmn = asn1_to_plmn_identity(rrc_setup_complete.registered_amf.plmn_id, selected_plmn);
      if (not plmn.has_value()) {
        logger.log_warning("Ignoring the registered AMF. Cause: Invalid PLMN identity");
        return request;
      }
      registered_plmn = plmn.value();
    }
    amf_identifier_t amf_id  = asn1_to_amf_identifier(rrc_setup_complete.registered_amf.amf_id);
    request.registered_guami = guami_t{registered_plmn, amf_id.amf_set_id, amf_id.amf_pointer, amf_id.amf_region_id};
    if (rrc_setup_complete.guami_type_present and
        rrc_setup_complete.guami_type.value == asn1::rrc_nr::rrc_setup_complete_ies_s::guami_type_opts::mapped) {
      request.registered_guami_type = guami_type_t::mapped;
    }
  }

  return request;
}
