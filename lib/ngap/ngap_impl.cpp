// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ngap_impl.h"
#include "log_helpers.h"
#include "ngap_asn1_helpers.h"
#include "ngap_asn1_utils.h"
#include "ngap_asn1_validators.h"
#include "ngap_error_indication_helper.h"
#include "procedures/ng_reset_procedure.h"
#include "procedures/ng_setup_procedure.h"
#include "procedures/ngap_dl_nas_message_transfer_procedure.h"
#include "procedures/ngap_dl_ran_status_transfer_procedure.h"
#include "procedures/ngap_handover_preparation_procedure.h"
#include "procedures/ngap_handover_resource_allocation_procedure.h"
#include "procedures/ngap_initial_context_setup_procedure.h"
#include "procedures/ngap_path_switch_procedure.h"
#include "procedures/ngap_pdu_session_resource_modify_procedure.h"
#include "procedures/ngap_pdu_session_resource_release_procedure.h"
#include "procedures/ngap_pdu_session_resource_setup_procedure.h"
#include "procedures/ngap_ue_context_modification_procedure.h"
#include "procedures/ngap_ue_context_release_procedure.h"
#include "ocudu/asn1/ngap/common.h"
#include "ocudu/asn1/ngap/ngap.h"
#include "ocudu/ngap/ngap_setup.h"
#include "ocudu/ngap/ngap_types.h"
#include "ocudu/ran/cause/ngap_cause.h"
#include "ocudu/ran/cu_cp_location_reporting_types.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/support/async/async_no_op_task.h"
#include "fmt/std.h"

using namespace ocudu;
using namespace asn1::ngap;
using namespace ocucp;

ngap_impl::ngap_impl(const ngap_configuration& ngap_cfg_,
                     ngap_cu_cp_notifier&      cu_cp_notifier_,
                     n2_connection_client&     n2_gateway,
                     ngap_ran_ue_id_allocator& ran_ue_ids,
                     timer_manager&            timers_,
                     task_executor&            ctrl_exec_) :
  logger(ocudulog::fetch_basic_logger("NGAP")),
  ue_ctxt_list(timer_factory{timers_, ctrl_exec_}, ran_ue_ids, logger),
  cu_cp_notifier(cu_cp_notifier_),
  timers(timers_),
  ctrl_exec(ctrl_exec_),
  ev_mng(timer_factory{timers, ctrl_exec}),
  conn_handler(ngap_cfg_.amf_index, n2_gateway, *this, cu_cp_notifier, ctrl_exec),
  tx_pdu_notifier(*this)
{
  context.gnb_id                      = ngap_cfg_.gnb_id;
  context.ran_node_name               = ngap_cfg_.ran_node_name;
  context.amf_index                   = ngap_cfg_.amf_index;
  context.supported_tas               = ngap_cfg_.supported_tas;
  context.amf_addr                    = ngap_cfg_.amf_addr;
  context.procedure_timeout           = ngap_cfg_.procedure_timeout;
  context.request_pdu_session_timeout = ngap_cfg_.request_pdu_session_timeout;
}

// Note: For fwd declaration of member types, dtor cannot be trivial.
ngap_impl::~ngap_impl() {}

bool ngap_impl::update_ue_index(cu_cp_ue_index_t        new_ue_index,
                                cu_cp_ue_index_t        old_ue_index,
                                ngap_cu_cp_ue_notifier& new_ue_notifier)
{
  if (!ue_ctxt_list.contains(old_ue_index)) {
    logger.warning("Failed to transfer NGAP UE context from ue={} to ue={}. Old UE context does not exist",
                   old_ue_index,
                   new_ue_index);
    return false;
  }

  ue_ctxt_list[old_ue_index].logger.log_debug("Transferring NGAP UE context to ue={}", new_ue_index);

  // Notify CU-CP about creation of NGAP UE.
  ngap_cu_cp_ue_notifier* ue = cu_cp_notifier.on_new_ngap_ue(new_ue_index);
  if (ue == nullptr) {
    logger.error("ue={}: Failed to transfer UE context", new_ue_index);
    return false;
  }

  ue_ctxt_list.update_ue_index(new_ue_index, old_ue_index, new_ue_notifier);

  return true;
}

std::optional<cu_cp_core_network_assist_info_for_inactive>
ngap_impl::get_cn_assist_info_for_inactive(cu_cp_ue_index_t ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: Cannot get core network assist info for inactive. UE context does not exist", ue_index);
    return std::nullopt;
  }
  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];
  return ue_ctxt.core_network_assist_info_for_inactive;
}

cu_cp_ue_index_t ngap_impl::remove_ue_association(ran_ue_id_t ran_ue_id)
{
  ngap_ue_context* ue_ctxt = ue_ctxt_list.find(ran_ue_id);
  if (ue_ctxt == nullptr) {
    return cu_cp_ue_index_t::invalid;
  }

  if (not ue_ctxt->take_over_allowed or ue_ctxt->release_requested or ue_ctxt->release_scheduled) {
    ue_ctxt->logger.log_warning("Keeping UE association. Cause: A new AMF cannot take over the UE in this state");
    return cu_cp_ue_index_t::invalid;
  }

  auto detached = schedule_ue_context_destruction(*ue_ctxt);
  if (detached == nullptr) {
    ue_ctxt->logger.log_warning("Keeping UE association. Cause: UE task queue is full");
    return cu_cp_ue_index_t::invalid;
  }

  cu_cp_ue_index_t ue_index = ue_ctxt->ue_ids.ue_index;
  ue_ctxt->logger.log_info("Removing UE association. Cause: A new AMF serves the UE");
  *detached = ue_ctxt_list.detach_ue_context(ran_ue_id);
  return ue_index;
}

std::shared_ptr<ngap_ue_context_list::detached_ue_context>
ngap_impl::schedule_ue_context_destruction(ngap_ue_context& ue_ctxt)
{
  // Procedures of the UE can still refer to the UE context. They run on the UE task loop, so the UE context is
  // destroyed there after them.
  auto                    detached = std::make_shared<ngap_ue_context_list::detached_ue_context>();
  ngap_cu_cp_ue_notifier* ue       = ue_ctxt.get_cu_cp_ue();
  if (ue != nullptr and
      not ue->schedule_async_task(launch_async([detached](coro_context<async_task<void>>& ctx) mutable {
        CORO_BEGIN(ctx);
        detached.reset();
        CORO_RETURN();
      }))) {
    return nullptr;
  }
  return detached;
}

bool ngap_impl::take_over_ue_association(ran_ue_id_t ran_ue_id)
{
  cu_cp_ue_index_t ue_index = cu_cp_notifier.on_ue_association_transfer_required(ran_ue_id);
  if (ue_index == cu_cp_ue_index_t::invalid) {
    return false;
  }

  ngap_cu_cp_ue_notifier* ue_notifier = cu_cp_notifier.on_new_ngap_ue(ue_index);
  if (ue_notifier == nullptr) {
    return false;
  }
  ue_ctxt_list.add_ue(ue_index, ran_ue_id, *ue_notifier);
  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];
  ue_ctxt.logger.log_info("Took over the UE association from the old AMF");
  start_request_pdu_session_timer(ue_ctxt);
  return true;
}

std::optional<guami_t> ngap_impl::get_serving_guami(cu_cp_ue_index_t ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    return std::nullopt;
  }
  return ue_ctxt_list[ue_index].serving_guami;
}

std::vector<s_nssai_t> ngap_impl::get_allowed_nssai(cu_cp_ue_index_t ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    return {};
  }
  return ue_ctxt_list[ue_index].allowed_nssai;
}

bool ngap_impl::handle_amf_tnl_connection_request()
{
  // This could be a reconnection, so make sure the tx_pdu_notifier is released before creating a new one.
  if (tx_pdu_notifier.is_connected()) {
    tx_pdu_notifier.disconnect();
  }

  std::unique_ptr<ngap_message_notifier> pdu_notifier = conn_handler.connect_to_amf();
  if (pdu_notifier == nullptr) {
    return false;
  }
  tx_pdu_notifier.connect(std::move(pdu_notifier));
  return true;
}

async_task<void> ngap_impl::handle_amf_disconnection_request()
{
  return conn_handler.handle_tnl_association_removal();
}

async_task<ngap_ng_setup_result> ngap_impl::handle_ng_setup_request(unsigned max_setup_retries)
{
  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_NG_SETUP);

  auto& ng_setup_request = ngap_msg.pdu.init_msg().value.ng_setup_request();
  fill_asn1_ng_setup_request(ng_setup_request, context);

  return launch_async<ng_setup_procedure>(
      context, ngap_msg, max_setup_retries, tx_pdu_notifier, ev_mng, timer_factory{timers, ctrl_exec}, logger);
}

async_task<void> ngap_impl::handle_ng_reset_message(const cu_cp_reset& msg)
{
  if (!std::holds_alternative<ngap_cause_t>(msg.cause)) {
    logger.error("Invalid cause type for NG Reset");
    return launch_async([](coro_context<async_task<void>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN();
    });
  }

  // Schedule NG Reset procedure.
  return launch_async<ng_reset_procedure>(context, msg, tx_pdu_notifier, ev_mng, ue_ctxt_list, logger);
}

void ngap_impl::handle_initial_ue_message(const cu_cp_initial_ue_message& msg)
{
  if (ue_ctxt_list.contains(msg.ue_index)) {
    logger.warning("ue={}: Dropping InitialUEMessage. UE context already exists", msg.ue_index);
    return;
  }

  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_INIT_UE_MSG);
  fill_asn1_initial_ue_message(ngap_msg.pdu.init_msg().value.init_ue_msg(), msg, context);

  send_initial_ue_message(msg.ue_index, ngap_msg);
}

void ngap_impl::start_request_pdu_session_timer(ngap_ue_context& ue_ctxt)
{
  const cu_cp_ue_index_t ue_index = ue_ctxt.ue_ids.ue_index;
  ue_ctxt.request_pdu_session_timer.set(context.request_pdu_session_timeout,
                                        [this, ue_index]() { on_request_pdu_session_timer_expired(ue_index); });
  ue_ctxt.request_pdu_session_timer.run();
  ue_ctxt.logger.log_debug("Starting PDU session creation timer (timeout={}ms)...",
                           ue_ctxt.request_pdu_session_timer.duration().count());
}

bool ngap_impl::send_initial_ue_message(cu_cp_ue_index_t ue_index, ngap_message& ngap_msg)
{
  // Create NGAP UE.
  // Allocate RAN-UE-ID.
  ran_ue_id_t ran_ue_id = ue_ctxt_list.allocate_ran_ue_id();
  if (ran_ue_id == ran_ue_id_t::invalid) {
    logger.error("ue={}: No RAN-UE-ID available", ue_index);
    return false;
  }

  // Create UE context and store it.
  ngap_cu_cp_ue_notifier* ue_notifier = cu_cp_notifier.on_new_ngap_ue(ue_index);
  ue_ctxt_list.add_ue(ue_index, ran_ue_id, *ue_notifier);

  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  ngap_msg.pdu.init_msg().value.init_ue_msg()->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);
  ue_ctxt.take_over_allowed                                   = true;

  start_request_pdu_session_timer(ue_ctxt);

  // Forward message to AMF.
  if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
    ue_ctxt.logger.log_warning("Cannot send InitialUEMessage");
    return false;
  }

  // InitialUEMessage reached AMF path, so count a connection establishment attempt.
  metrics_handler.aggregate_ue_associated_logical_ng_connection_establishment_attempt();
  return true;
}

bool ngap_impl::handle_rerouted_initial_ue_message(const ngap_reroute_nas_request& request)
{
  if (ue_ctxt_list.contains(request.ue_index)) {
    logger.warning("ue={}: Dropping rerouted InitialUEMessage. UE context already exists", request.ue_index);
    return false;
  }

  ngap_message reroute_msg;
  {
    asn1::cbit_ref bref(request.packed_reroute_nas_request);
    if (reroute_msg.pdu.unpack(bref) != asn1::OCUDUASN_SUCCESS or
        reroute_msg.pdu.type().value != ngap_pdu_c::types_opts::init_msg or
        reroute_msg.pdu.init_msg().value.type().value !=
            ngap_elem_procs_o::init_msg_c::types_opts::reroute_nas_request) {
      logger.warning("ue={}: Dropping rerouted InitialUEMessage. Invalid RerouteNASRequest", request.ue_index);
      return false;
    }
  }
  const auto& reroute = reroute_msg.pdu.init_msg().value.reroute_nas_request();

  // The NGAP Message IE contains the INITIAL UE MESSAGE (TS 38.413 section 9.2.5.5).
  ngap_message ngap_msg;
  {
    asn1::cbit_ref bref(reroute->ngap_msg);
    if (ngap_msg.pdu.unpack(bref) != asn1::OCUDUASN_SUCCESS or
        ngap_msg.pdu.type().value != ngap_pdu_c::types_opts::init_msg or
        ngap_msg.pdu.init_msg().value.type().value != ngap_elem_procs_o::init_msg_c::types_opts::init_ue_msg) {
      logger.warning("ue={}: Dropping rerouted InitialUEMessage. The NGAP Message IE holds no InitialUEMessage",
                     request.ue_index);
      return false;
    }
  }

  // The AMF Set ID IE marks the message as rerouted. The NG-RAN node propagates the NSSAI information and the Source
  // to Target AMF Information Reroute (TS 38.413 sections 8.6.1.2 and 8.6.5.2).
  auto& init_ue_msg               = ngap_msg.pdu.init_msg().value.init_ue_msg();
  init_ue_msg->amf_set_id_present = true;
  init_ue_msg->amf_set_id         = reroute->amf_set_id;
  if (reroute->allowed_nssai_present) {
    init_ue_msg->allowed_nssai_present = true;
    init_ue_msg->allowed_nssai         = reroute->allowed_nssai;
  }
  if (reroute->partially_allowed_nssai_present) {
    init_ue_msg->partially_allowed_nssai_present = true;
    init_ue_msg->partially_allowed_nssai         = reroute->partially_allowed_nssai;
  }
  if (reroute->source_to_target_amf_info_reroute_present) {
    init_ue_msg->source_to_target_amf_info_reroute_present = true;
    init_ue_msg->source_to_target_amf_info_reroute         = reroute->source_to_target_amf_info_reroute;
  }

  if (not send_initial_ue_message(request.ue_index, ngap_msg)) {
    if (ue_ctxt_list.contains(request.ue_index)) {
      ue_ctxt_list.remove_ue_context(request.ue_index);
    }
    return false;
  }
  return true;
}

void ngap_impl::release_ue_after_failed_reroute(ngap_ue_context& ue_ctxt)
{
  // The AMF gave up the UE, so the CU-CP releases the UE without NGAP signalling.
  const cu_cp_ue_index_t ue_index = ue_ctxt.ue_ids.ue_index;
  ue_ctxt.release_scheduled       = true;
  ue_ctxt.get_cu_cp_ue()->schedule_async_task(launch_async([this, ue_index](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);
    CORO_AWAIT(cu_cp_notifier.on_new_ue_context_release_command({ue_index, ngap_cause_radio_network_t::unspecified}));
    CORO_RETURN();
  }));
}

void ngap_impl::handle_amf_configuration_update(const asn1::ngap::amf_cfg_upd_s& msg)
{
  error_type<std::string> result = {};
  if (msg->served_guami_list_present) {
    result = validate_served_guami_list(msg->served_guami_list);
  }
  if (result.has_value() and msg->plmn_support_list_present) {
    result = validate_plmn_support_list(msg->plmn_support_list);
  }
  if (not result.has_value()) {
    logger.warning("Rejecting AMFConfigurationUpdate. Cause: {}", result.error());
    ngap_message ngap_msg;
    ngap_msg.pdu.set_unsuccessful_outcome().load_info_obj(ASN1_NGAP_ID_AMF_CFG_UPD);
    ngap_msg.pdu.unsuccessful_outcome().value.amf_cfg_upd_fail()->cause =
        cause_to_asn1(cause_protocol_t::semantic_error);
    if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
      logger.warning("Cannot send AMFConfigurationUpdateFailure");
    }
    return;
  }

  // The received information replaces the stored information. The NG-RAN node uses it for further AMF selection (TS
  // 38.413 section 8.7.3.2).
  if (msg->amf_name_present) {
    context.amf_name = msg->amf_name.to_string();
  }
  if (msg->served_guami_list_present) {
    context.served_guami_list = asn1_to_served_guami_list(msg->served_guami_list);
    context.unavailable_guami_list.clear();
  }
  if (msg->relative_amf_capacity_present) {
    context.relative_amf_capacity = msg->relative_amf_capacity;
  }
  if (msg->plmn_support_list_present) {
    context.plmn_support_list = asn1_to_plmn_support_list(msg->plmn_support_list);
  }

  ngap_message ngap_msg;
  ngap_msg.pdu.set_successful_outcome().load_info_obj(ASN1_NGAP_ID_AMF_CFG_UPD);
  auto& ack = ngap_msg.pdu.successful_outcome().value.amf_cfg_upd_ack();

  // The CU-CP uses one TNL association per AMF, so it sets up none of the TNL associations that the AMF adds.
  if (msg->amf_tnl_assoc_to_add_list_present) {
    logger.warning("Rejecting {} AMF TNL associations to add. Cause: The CU-CP uses one TNL association per AMF",
                   msg->amf_tnl_assoc_to_add_list.size());
    ack->amf_tnl_assoc_failed_to_setup_list_present = true;
    for (const auto& item : msg->amf_tnl_assoc_to_add_list) {
      asn1::ngap::tnl_assoc_item_s failed_item;
      failed_item.tnl_assoc_address = item.amf_tnl_assoc_address;
      failed_item.cause             = cause_to_asn1(ngap_cause_transport_t::unspecified);
      ack->amf_tnl_assoc_failed_to_setup_list.push_back(failed_item);
    }
  }

  if (msg->amf_tnl_assoc_to_rem_list_present or msg->amf_tnl_assoc_to_upd_list_present) {
    logger.warning(
        "Ignoring the AMF TNL associations to remove or update. Cause: The CU-CP uses one TNL association per AMF");
  }

  if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
    logger.warning("Cannot send AMFConfigurationUpdateAcknowledge");
  }
}

void ngap_impl::handle_amf_status_indication(const asn1::ngap::amf_status_ind_s& msg)
{
  // The NG-RAN node considers the indicated GUAMIs unavailable for further AMF selection (TS 38.413 section 8.7.6.2).
  for (const auto& asn1_item : msg->unavailable_guami_list) {
    expected<guami_t, std::string> guami = asn1_to_guami(asn1_item.guami);
    if (not guami.has_value()) {
      logger.warning("Ignoring unavailable GUAMI. Cause: {}", guami.error());
      continue;
    }

    ngap_unavailable_guami_item item;
    item.guami = guami.value();
    if (asn1_item.backup_amf_name_present) {
      item.backup_amf_name = asn1_item.backup_amf_name.to_string();
    }
    logger.info("GUAMI with AMF Set ID {} and AMF Pointer {} is unavailable. Backup AMF: {}",
                item.guami.amf_set_id,
                item.guami.amf_pointer,
                item.backup_amf_name.value_or("none"));

    // A new indication for a GUAMI replaces the stored one.
    auto it = std::find_if(context.unavailable_guami_list.begin(),
                           context.unavailable_guami_list.end(),
                           [&item](const ngap_unavailable_guami_item& stored) { return stored.guami == item.guami; });
    if (it != context.unavailable_guami_list.end()) {
      *it = std::move(item);
    } else {
      context.unavailable_guami_list.push_back(std::move(item));
    }
  }
}

void ngap_impl::handle_overload_start(const asn1::ngap::overload_start_s& msg)
{
  // A new Overload Start replaces the stored overload state (TS 38.413 section 8.7.7.2).
  ngap_amf_overload overload;
  if (msg->amf_overload_resp_present and
      msg->amf_overload_resp.type().value == asn1::ngap::overload_resp_c::types_opts::overload_action) {
    overload.action = asn1_to_overload_action(msg->amf_overload_resp.overload_action());
  }
  if (msg->amf_traffic_load_reduction_ind_present) {
    overload.traffic_load_reduction = msg->amf_traffic_load_reduction_ind;
  }
  for (const auto& asn1_item : msg->overload_start_nssai_list) {
    ngap_overload_start_nssai_item item;
    for (const auto& asn1_slice : asn1_item.slice_overload_list) {
      item.slice_overload_list.push_back(ngap_asn1_to_s_nssai(asn1_slice.s_nssai));
    }
    if (asn1_item.slice_overload_resp_present and
        asn1_item.slice_overload_resp.type().value == asn1::ngap::overload_resp_c::types_opts::overload_action) {
      item.slice_overload_action = asn1_to_overload_action(asn1_item.slice_overload_resp.overload_action());
    }
    if (asn1_item.slice_traffic_load_reduction_ind_present) {
      item.slice_traffic_load_reduction = asn1_item.slice_traffic_load_reduction_ind;
    }
    overload.overload_start_nssai_list.push_back(std::move(item));
  }
  context.overload = overload;

  logger.debug("AMF is overloaded. Overload action: {}. Traffic load reduction: {}. Slice overload items: {}",
               overload.action.has_value() ? msg->amf_overload_resp.overload_action().to_string() : "none",
               overload.traffic_load_reduction.has_value() ? fmt::format("{}%", *overload.traffic_load_reduction)
                                                           : std::string("none"),
               overload.overload_start_nssai_list.size());
}

void ngap_impl::handle_overload_stop()
{
  context.overload.reset();
  logger.debug("AMF is no longer overloaded");
}

void ngap_impl::handle_reroute_nas_request(const asn1::ngap::reroute_nas_request_s& msg)
{
  const ran_ue_id_t                ran_ue_id = uint_to_ran_ue_id(msg->ran_ue_ngap_id);
  const std::optional<amf_ue_id_t> amf_ue_id =
      msg->amf_ue_ngap_id_present ? std::optional<amf_ue_id_t>{uint_to_amf_ue_id(msg->amf_ue_ngap_id)} : std::nullopt;
  ngap_ue_context* ue_ctxt = ue_ctxt_list.find(ran_ue_id);
  if (ue_ctxt == nullptr) {
    logger.warning("ran_ue={}: Dropping RerouteNASRequest. UE context does not exist", msg->ran_ue_ngap_id);
    send_error_indication(
        tx_pdu_notifier, logger, ran_ue_id, amf_ue_id, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }
  if (amf_ue_id.has_value() and not validate_consistent_ue_id_pair(ran_ue_id, amf_ue_id.value())) {
    handle_inconsistent_ue_id_pair(ran_ue_id, amf_ue_id.value());
    return;
  }

  // The Allowed NSSAI and the Partially Allowed NSSAI hold at most 8 S-NSSAIs together, and no S-NSSAI is in both
  // (TS 38.413 section 8.6.5.3).
  if (msg->partially_allowed_nssai_present) {
    bool overlap = std::any_of(
        msg->partially_allowed_nssai.begin(), msg->partially_allowed_nssai.end(), [&msg](const auto& partial) {
          return std::any_of(msg->allowed_nssai.begin(), msg->allowed_nssai.end(), [&partial](const auto& allowed) {
            return ngap_asn1_to_s_nssai(allowed.s_nssai) == ngap_asn1_to_s_nssai(partial.s_nssai);
          });
        });
    if (msg->allowed_nssai.size() + msg->partially_allowed_nssai.size() > 8 or overlap) {
      ue_ctxt->logger.log_warning("Releasing UE. Cause: The RerouteNASRequest has an invalid NSSAI");
      release_ue_after_failed_reroute(*ue_ctxt);
      return;
    }
  }

  ngap_reroute_nas_request request;
  request.ue_index   = ue_ctxt->ue_ids.ue_index;
  request.amf_set_id = msg->amf_set_id.to_number();
  {
    ngap_message reroute_msg;
    reroute_msg.pdu.set_init_msg().load_info_obj(ASN1_NGAP_ID_REROUTE_NAS_REQUEST);
    reroute_msg.pdu.init_msg().value.reroute_nas_request() = msg;
    request.packed_reroute_nas_request                     = byte_buffer{byte_buffer::fallback_allocation_tag{}};
    asn1::bit_ref bref(request.packed_reroute_nas_request);
    if (reroute_msg.pdu.pack(bref) != asn1::OCUDUASN_SUCCESS) {
      ue_ctxt->logger.log_warning("Dropping RerouteNASRequest. Cannot pack it");
      return;
    }
  }

  // The UE association of the target AMF replaces the UE association of the source AMF.
  auto detached = schedule_ue_context_destruction(*ue_ctxt);
  if (detached == nullptr) {
    ue_ctxt->logger.log_warning("Dropping RerouteNASRequest. Cause: UE task queue is full");
    return;
  }

  ue_ctxt->logger.log_info("Rerouting InitialUEMessage to AMF Set {}", request.amf_set_id);
  if (not cu_cp_notifier.on_reroute_nas_request(request)) {
    ue_ctxt->logger.log_warning("Releasing UE. Cause: Cannot reroute InitialUEMessage to AMF Set {}",
                                request.amf_set_id);
    release_ue_after_failed_reroute(*ue_ctxt);
    return;
  }
  *detached = ue_ctxt_list.detach_ue_context(ran_ue_id);
}

void ngap_impl::handle_ul_nas_transport_message(const cu_cp_ul_nas_transport& msg)
{
  if (!ue_ctxt_list.contains(msg.ue_index)) {
    logger.warning("ue={}: Dropping UlNasTransportMessage. UE context does not exist", msg.ue_index);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[msg.ue_index];

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping UlNasTransportMessage. UE is already scheduled for release");
    return;
  }

  // The UE can send a NAS message before the AMF sends the AMF UE NGAP ID. Store the NAS message until the
  // UE-associated logical NG-connection exists (TS 38.413 sections 8.6.2.2 and 8.6.3.1).
  if (ue_ctxt.ue_ids.amf_ue_id == amf_ue_id_t::invalid) {
    if (ue_ctxt.pending_ul_nas_msgs.size() >= ngap_ue_context::max_nof_pending_ul_nas_msgs) {
      ue_ctxt.logger.log_warning("Dropping ULNASTransport. Cause: {} NAS messages already wait for the AMF UE NGAP ID",
                                 ue_ctxt.pending_ul_nas_msgs.size());
      return;
    }
    ue_ctxt.logger.log_info("Storing ULNASTransport until the AMF sends the AMF UE NGAP ID");
    ue_ctxt.pending_ul_nas_msgs.push_back(msg);
    return;
  }

  send_ul_nas_transport(ue_ctxt, msg);
}

void ngap_impl::send_ul_nas_transport(ngap_ue_context& ue_ctxt, const cu_cp_ul_nas_transport& msg)
{
  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UL_NAS_TRANSPORT);

  auto& ul_nas_transport_msg = ngap_msg.pdu.init_msg().value.ul_nas_transport();

  ul_nas_transport_msg->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);
  ul_nas_transport_msg->amf_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.amf_ue_id);

  fill_asn1_ul_nas_transport(ul_nas_transport_msg, msg);

  // Schedule transmission of UL NAS transport message to AMF.
  ue_ctxt.get_cu_cp_ue()->schedule_async_task(
      launch_async([this, ue_index = ue_ctxt.ue_ids.ue_index, ngap_msg](coro_context<async_task<void>>& ctx) {
        CORO_BEGIN(ctx);
        if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
          logger.error("ue={} ran_ue={} amf_ue={}: Cannot send ULNASTransportMessage",
                       ue_index,
                       ngap_msg.pdu.init_msg().value.ul_nas_transport()->ran_ue_ngap_id,
                       ngap_msg.pdu.init_msg().value.ul_nas_transport()->amf_ue_ngap_id);
          CORO_EARLY_RETURN();
        }
        CORO_RETURN();
      }));
}

void ngap_impl::send_pending_ul_nas_transports(ngap_ue_context& ue_ctxt)
{
  for (const cu_cp_ul_nas_transport& msg : ue_ctxt.pending_ul_nas_msgs) {
    send_ul_nas_transport(ue_ctxt, msg);
  }
  ue_ctxt.pending_ul_nas_msgs.clear();
}

void ngap_impl::handle_location_report_transmission(const location_report& msg)
{
  if (!ue_ctxt_list.contains(msg.ue_index)) {
    logger.warning("ue={}: Dropping Location Report message. UE context does not exist", msg.ue_index);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[msg.ue_index];

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  if (ue_ctxt.release_scheduled) {
    // TODO: check if we should still send it in that case or not, as it may be the last location report for this UE
    ue_ctxt.logger.log_info("Dropping Location Report message. UE is already scheduled for release");
    return;
  }

  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_LOCATION_REPORT);

  auto& location_report_msg = ngap_msg.pdu.init_msg().value.location_report();

  location_report_msg->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);

  amf_ue_id_t amf_ue_id = ue_ctxt.ue_ids.amf_ue_id;
  if (amf_ue_id == amf_ue_id_t::invalid) {
    logger.warning("ue={}: Dropping Location Report message. UE AMF ID not found", msg.ue_index);
    return;
  }
  location_report_msg->amf_ue_ngap_id = to_underlying(amf_ue_id);

  fill_asn1_location_report(*location_report_msg, msg);

  // Schedule transmission of Location Report message to AMF.
  ue->schedule_async_task(launch_async([this, msg, ngap_msg](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);
    if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
      logger.error("ue={}: Cannot send LocationReport", msg.ue_index);
      CORO_EARLY_RETURN();
    }
    CORO_RETURN();
  }));
}

void ngap_impl::handle_location_reporting_failure_indication_transmission(const location_report_failure_indication& msg)
{
  if (!ue_ctxt_list.contains(msg.ue_index)) {
    logger.warning("ue={}: Dropping Location Reporting Failure Indication. UE context does not exist", msg.ue_index);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[msg.ue_index];

  amf_ue_id_t amf_ue_id = ue_ctxt.ue_ids.amf_ue_id;
  if (amf_ue_id == amf_ue_id_t::invalid) {
    ue_ctxt.logger.log_warning("Dropping Location Reporting Failure Indication. UE AMF ID not found");
    return;
  }

  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_LOCATION_REPORT_FAIL_IND);

  auto& fail_ind_msg = ngap_msg.pdu.init_msg().value.location_report_fail_ind();

  fail_ind_msg->amf_ue_ngap_id = to_underlying(amf_ue_id);
  fail_ind_msg->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);
  fail_ind_msg->cause          = cause_to_asn1(msg.cause);

  // Forward message to AMF.
  if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
    ue_ctxt.logger.log_warning("Cannot send LocationReportingFailureIndication");
    return;
  }
}

void ngap_impl::handle_tx_ue_radio_capability_info_indication_required(
    const ngap_ue_radio_capability_info_indication& msg)
{
  if (!ue_ctxt_list.contains(msg.ue_index)) {
    logger.warning("ue={}: Dropping UE Radio Capability Info Indication. UE context does not exist", msg.ue_index);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[msg.ue_index];

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UE_RADIO_CAP_INFO_IND);
  auto& ue_radio_cap_info_ind_msg = ngap_msg.pdu.init_msg().value.ue_radio_cap_info_ind();

  ue_radio_cap_info_ind_msg->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);

  amf_ue_id_t amf_ue_id = ue_ctxt.ue_ids.amf_ue_id;
  if (amf_ue_id == amf_ue_id_t::invalid) {
    logger.warning("ue={}: Dropping UE Radio Capability Info message. UE does not have an AMF UE ID", msg.ue_index);
    return;
  }
  ue_radio_cap_info_ind_msg->amf_ue_ngap_id = to_underlying(amf_ue_id);
  ue_radio_cap_info_ind_msg->ue_radio_cap   = msg.ue_cap_rat_container_list.copy();

  ue_ctxt.logger.log_debug("Scheduling UE Radio Capability Info Indication");

  // Schedule transmission of UE Radio Capability Info Indication to AMF.
  ue->schedule_async_task(launch_async([this, msg, ngap_msg](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);
    if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
      logger.error("ue={} ran_ue={} amf_ue={}: Cannot send UERadioCapabilityInfoIndication",
                   msg.ue_index,
                   ngap_msg.pdu.init_msg().value.ul_nas_transport()->ran_ue_ngap_id,
                   ngap_msg.pdu.init_msg().value.ul_nas_transport()->amf_ue_ngap_id);
      CORO_EARLY_RETURN();
    }
    CORO_RETURN();
  }));
}

void ngap_impl::handle_message(const ngap_message& msg)
{
  // Run NGAP protocols in Control executor.
  if (not ctrl_exec.execute([this, msg]() {
        log_rx_pdu(msg);

        switch (msg.pdu.type().value) {
          case ngap_pdu_c::types_opts::init_msg:
            handle_initiating_message(msg.pdu.init_msg());
            break;
          case ngap_pdu_c::types_opts::successful_outcome:
            handle_successful_outcome(msg.pdu.successful_outcome());
            break;
          case ngap_pdu_c::types_opts::unsuccessful_outcome:
            handle_unsuccessful_outcome(msg.pdu.unsuccessful_outcome());
            break;
          default:
            logger.error("Invalid PDU type");
            break;
        }
      })) {
    logger.error("Discarding Rx NGAP PDU. Cause: task queue is full");
  }
}

void ngap_impl::handle_initiating_message(const init_msg_s& msg)
{
  switch (msg.value.type().value) {
    case ngap_elem_procs_o::init_msg_c::types_opts::dl_nas_transport:
      handle_dl_nas_transport_message(msg.value.dl_nas_transport());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::init_context_setup_request:
      handle_initial_context_setup_request(msg.value.init_context_setup_request());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::reroute_nas_request:
      handle_reroute_nas_request(msg.value.reroute_nas_request());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::amf_cfg_upd:
      handle_amf_configuration_update(msg.value.amf_cfg_upd());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::amf_status_ind:
      handle_amf_status_indication(msg.value.amf_status_ind());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::overload_start:
      handle_overload_start(msg.value.overload_start());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::overload_stop:
      handle_overload_stop();
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::ue_context_mod_request:
      handle_ue_context_modification_request(msg.value.ue_context_mod_request());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::pdu_session_res_setup_request:
      handle_pdu_session_resource_setup_request(msg.value.pdu_session_res_setup_request());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::pdu_session_res_modify_request:
      handle_pdu_session_resource_modify_request(msg.value.pdu_session_res_modify_request());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::pdu_session_res_release_cmd:
      handle_pdu_session_resource_release_command(msg.value.pdu_session_res_release_cmd());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::ue_context_release_cmd:
      handle_ue_context_release_command(msg.value.ue_context_release_cmd());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::paging:
      handle_paging(msg.value.paging());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::write_replace_warning_request:
      handle_write_replace_warning(msg.value.write_replace_warning_request());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::ho_request:
      handle_handover_request(msg.value.ho_request());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::dl_ran_status_transfer:
      handle_dl_ran_status_transfer(msg.value.dl_ran_status_transfer());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::dl_ue_associated_nrppa_transport:
      handle_dl_ue_associated_nrppa_transport(msg.value.dl_ue_associated_nrppa_transport());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::dl_non_ue_associated_nrppa_transport:
      handle_dl_non_ue_associated_nrppa_transport(msg.value.dl_non_ue_associated_nrppa_transport());
      break;
    case asn1::ngap::ngap_elem_procs_o::init_msg_c::types_opts::location_report_ctrl:
      handle_location_reporting_control_message(msg.value.location_report_ctrl());
      break;
    case ngap_elem_procs_o::init_msg_c::types_opts::error_ind:
      handle_error_indication(msg.value.error_ind());
      break;
    default:
      logger.error("Initiating message of type {} is not supported", msg.value.type().to_string());
  }
}

void ngap_impl::handle_dl_nas_transport_message(const asn1::ngap::dl_nas_transport_s& msg)
{
  // The AMF can use a RAN UE NGAP ID that the CU-CP allocated over another NG interface instance (TS 38.413 section
  // 8.6.2.2).
  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(msg->ran_ue_ngap_id)) and
      !take_over_ue_association(uint_to_ran_ue_id(msg->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping DlNasTransportMessage. UE context does not exist",
                   msg->ran_ue_ngap_id,
                   msg->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier,
                          logger,
                          uint_to_ran_ue_id(msg->ran_ue_ngap_id),
                          uint_to_amf_ue_id(msg->amf_ue_ngap_id),
                          ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  // Check whether another context doesn't exist already for the same AMF UE ID with mismatched RAN UE ID.
  if (not validate_consistent_ue_id_pair(uint_to_ran_ue_id(msg->ran_ue_ngap_id),
                                         uint_to_amf_ue_id(msg->amf_ue_ngap_id))) {
    // Release old UE context and send error indication with the received UE IDs to the AMF.
    handle_inconsistent_ue_id_pair(uint_to_ran_ue_id(msg->ran_ue_ngap_id), uint_to_amf_ue_id(msg->amf_ue_ngap_id));
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[uint_to_ran_ue_id(msg->ran_ue_ngap_id)];

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping DlNasTransportMessage. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                uint_to_amf_ue_id(msg->amf_ue_ngap_id)});
    return;
  }

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  // Store UE Aggregate Maximum Bitrate.
  if (msg->ue_aggr_max_bit_rate_present) {
    ue->set_ue_ambr(aggregate_maximum_bit_rate_t{msg->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_dl,
                                                 msg->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_ul});
  }

  // Store the Allowed NSSAI, for example after a UE Configuration Update (TS 23.502 section 4.2.4.2).
  if (msg->allowed_nssai_present) {
    ue_ctxt.allowed_nssai.clear();
    for (const auto& asn1_item : msg->allowed_nssai) {
      ue_ctxt.allowed_nssai.push_back(ngap_asn1_to_s_nssai(asn1_item.s_nssai));
    }
  }

  // Add AMF UE ID to ue ngap context if it is not set (this is the first DL NAS Transport message).
  if (ue_ctxt.ue_ids.amf_ue_id == amf_ue_id_t::invalid) {
    // Set AMF UE ID in the UE context and also in the lookup.
    ue_ctxt_list.update_amf_ue_id(ue_ctxt.ue_ids.ran_ue_id, uint_to_amf_ue_id(msg->amf_ue_ngap_id));
    // AMF UE ID is now associated, so count one establishment success.
    metrics_handler.aggregate_ue_associated_logical_ng_connection_establishment_success();
    send_pending_ul_nas_transports(ue_ctxt);
  }

  ngap_dl_nas_transport_message dl_nas_msg;
  fill_ngap_dl_nas_transport_message(dl_nas_msg, ue->get_ue_index(), msg);

  // Start routine.
  ue->schedule_async_task(launch_async<ngap_dl_nas_message_transfer_procedure>(
      dl_nas_msg, ue->get_ngap_rrc_ue_notifier(), get_ngap_ue_radio_cap_management_handler(), ue_ctxt.logger));
}

void ngap_impl::handle_initial_context_setup_request(const asn1::ngap::init_context_setup_request_s& request)
{
  // Notify metrics handler about requested PDU sessions.
  if (request->pdu_session_res_setup_list_cxt_req_present) {
    for (const auto& pdu_session : request->pdu_session_res_setup_list_cxt_req) {
      metrics_handler.aggregate_requested_pdu_session(ngap_asn1_to_s_nssai(pdu_session.s_nssai));
    }
  }

  // The AMF can use a RAN UE NGAP ID that the CU-CP allocated over another NG interface instance (TS 38.413 section
  // 8.3.1.2).
  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(request->ran_ue_ngap_id)) and
      !take_over_ue_association(uint_to_ran_ue_id(request->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping InitialContextSetupRequest. UE context does not exist",
                   request->ran_ue_ngap_id,
                   request->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier,
                          logger,
                          uint_to_ran_ue_id(request->ran_ue_ngap_id),
                          uint_to_amf_ue_id(request->amf_ue_ngap_id),
                          ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  // Check whether another context doesn't exist already for the same AMF UE ID with mismatched RAN UE ID.
  if (not validate_consistent_ue_id_pair(uint_to_ran_ue_id(request->ran_ue_ngap_id),
                                         uint_to_amf_ue_id(request->amf_ue_ngap_id))) {
    // Release old UE context and send error indication with the received UE IDs to the AMF.
    handle_inconsistent_ue_id_pair(uint_to_ran_ue_id(request->ran_ue_ngap_id),
                                   uint_to_amf_ue_id(request->amf_ue_ngap_id));
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[uint_to_ran_ue_id(request->ran_ue_ngap_id)];

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping InitialContextSetup. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                uint_to_amf_ue_id(request->amf_ue_ngap_id)});
    return;
  }

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  // If InitialContextSetupRequest contains PDU Session Setup list, stop pdu session setup timer.
  if (request->pdu_session_res_setup_list_cxt_req_present) {
    ue_ctxt.request_pdu_session_timer.stop();
  }

  // Update AMF ID and use the one from this Context Setup as per 3GPP TS 38.413 section 8.3.1.2.
  if (ue_ctxt.ue_ids.amf_ue_id == amf_ue_id_t::invalid) {
    // AMF UE ID update via InitialContextSetupRequest counts as establishment success if was empty.
    metrics_handler.aggregate_ue_associated_logical_ng_connection_establishment_success();
  }
  if (ue_ctxt.ue_ids.amf_ue_id != uint_to_amf_ue_id(request->amf_ue_ngap_id)) {
    ue_ctxt_list.update_amf_ue_id(ue_ctxt.ue_ids.ran_ue_id, uint_to_amf_ue_id(request->amf_ue_ngap_id));
  }
  send_pending_ul_nas_transports(ue_ctxt);

  // Convert to common type.
  ngap_init_context_setup_request init_ctxt_setup_req;
  init_ctxt_setup_req.ue_index = ue_ctxt.ue_ids.ue_index;
  if (!fill_ngap_initial_context_setup_request(init_ctxt_setup_req, request)) {
    // A logical error in a class 1 procedure is answered with its failure message (TS 38.413 section 10.4).
    ue_ctxt.logger.log_warning("Rejecting invalid InitialContextSetupRequest");
    ngap_message ngap_msg;
    ngap_msg.pdu.set_unsuccessful_outcome().load_info_obj(ASN1_NGAP_ID_INIT_CONTEXT_SETUP);
    auto& init_ctxt_setup_fail           = ngap_msg.pdu.unsuccessful_outcome().value.init_context_setup_fail();
    init_ctxt_setup_fail->amf_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.amf_ue_id);
    init_ctxt_setup_fail->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);
    init_ctxt_setup_fail->cause          = cause_to_asn1(cause_protocol_t::semantic_error);

    // Notify metrics handler about failed PDU sessions.
    if (request->pdu_session_res_setup_list_cxt_req_present) {
      for (const auto& pdu_session : request->pdu_session_res_setup_list_cxt_req) {
        metrics_handler.aggregate_failed_pdu_session_setup(ngap_asn1_to_s_nssai(pdu_session.s_nssai),
                                                           cause_protocol_t::semantic_error);
      }
    }

    if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
      ue_ctxt.logger.log_warning("Cannot send InitialContextSetupFailure");
    }
    return;
  }

  // Store Core Network Assist Info for Inactive if present.
  if (init_ctxt_setup_req.core_network_assist_info_for_inactive.has_value()) {
    ue_ctxt.core_network_assist_info_for_inactive = init_ctxt_setup_req.core_network_assist_info_for_inactive.value();
  }

  // Store the GUAMI of the AMF that serves the UE, and the Allowed NSSAI of the UE. A new AMF cannot take over the UE
  // any more.
  ue_ctxt.serving_guami     = init_ctxt_setup_req.guami;
  ue_ctxt.allowed_nssai     = init_ctxt_setup_req.allowed_nssai;
  ue_ctxt.take_over_allowed = false;

  // Store RRC inactive transition report request if present.
  if (init_ctxt_setup_req.rrc_inactive_transition_report_request.has_value()) {
    ue_ctxt.rrc_inactive_transition_report_request = init_ctxt_setup_req.rrc_inactive_transition_report_request.value();
  }

  // Store UE Aggregate Maximum Bitrate if it is set.
  if (init_ctxt_setup_req.ue_aggr_max_bit_rate.has_value()) {
    ue->set_ue_ambr(init_ctxt_setup_req.ue_aggr_max_bit_rate.value());
  } else {
    // Add stored UE Aggregate Maximum Bitrate to request.
    aggregate_maximum_bit_rate_t ue_ambr = ue->get_ue_ambr();
    if (ue_ambr.dl > 0 && ue_ambr.ul > 0) {
      init_ctxt_setup_req.ue_aggr_max_bit_rate = ue_ambr;
    }
  }

  // Log security context.
  ue_ctxt.logger.log_debug(request->security_key.data(), 32, "K_gnb");
  ue_ctxt.logger.log_debug("Supported integrity algorithms: {}",
                           init_ctxt_setup_req.security_context.supported_int_algos);
  ue_ctxt.logger.log_debug("Supported ciphering algorithms: {}",
                           init_ctxt_setup_req.security_context.supported_enc_algos);

  // Start routine.
  ue->schedule_async_task(launch_async<ngap_initial_context_setup_procedure>(
      init_ctxt_setup_req, ue_ctxt.ue_ids, cu_cp_notifier, metrics_handler, tx_pdu_notifier, ue_ctxt.logger));
}

void ngap_impl::handle_ue_context_modification_request(const asn1::ngap::ue_context_mod_request_s& request)
{
  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(request->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping UEContextModificationRequest. UE context doesn't exist",
                   request->ran_ue_ngap_id,
                   request->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier, logger, {}, {}, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  // Check whether another context doesn't exist already for the same AMF UE ID with mismatched RAN UE ID.
  if (not validate_consistent_ue_id_pair(uint_to_ran_ue_id(request->ran_ue_ngap_id),
                                         uint_to_amf_ue_id(request->amf_ue_ngap_id))) {
    // Release old UE context and send error indication with the received UE IDs to the AMF.
    handle_inconsistent_ue_id_pair(uint_to_ran_ue_id(request->ran_ue_ngap_id),
                                   uint_to_amf_ue_id(request->amf_ue_ngap_id));
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[uint_to_ran_ue_id(request->ran_ue_ngap_id)];

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping UEContextModificationRequest. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                uint_to_amf_ue_id(request->amf_ue_ngap_id)});
    return;
  }

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context modification doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  // Convert to common type.
  ngap_ue_context_modification_request ue_context_mod_request;
  ue_context_mod_request.ue_index = ue_ctxt.ue_ids.ue_index;
  if (!ue_context_modification_request_from_asn1(ue_context_mod_request, request)) {
    ue_ctxt.logger.log_warning("Conversion of UEContextModificationRequest failed");
    send_error_indication(tx_pdu_notifier, logger, ue_ctxt.ue_ids.ran_ue_id, ue_ctxt.ue_ids.amf_ue_id);
    return;
  }

  // Store Core Network Assist Info for Inactive if present.
  if (ue_context_mod_request.core_network_assist_info_for_inactive.has_value()) {
    ue_ctxt.core_network_assist_info_for_inactive =
        ue_context_mod_request.core_network_assist_info_for_inactive.value();
  }

  // Store serving PLMN if it is set.
  if (ue_context_mod_request.new_guami.has_value()) {
    ue_ctxt.serving_guami = ue_context_mod_request.new_guami.value();
  }

  // Store UE Aggregate Maximum Bitrate if it is set.
  if (ue_context_mod_request.ue_aggr_max_bit_rate.has_value()) {
    ue->set_ue_ambr(ue_context_mod_request.ue_aggr_max_bit_rate.value());
  }

  // Start routine.
  ue->schedule_async_task(launch_async<ngap_ue_context_modification_procedure>(
      ue_context_mod_request, ue_ctxt.ue_ids, cu_cp_notifier, tx_pdu_notifier, ue_ctxt.logger));
}

void ngap_impl::handle_pdu_session_resource_setup_request(const asn1::ngap::pdu_session_res_setup_request_s& request)
{
  // Notify metrics handler about requested PDU sessions.
  for (const auto& pdu_session : request->pdu_session_res_setup_list_su_req) {
    metrics_handler.aggregate_requested_pdu_session(ngap_asn1_to_s_nssai(pdu_session.s_nssai));
  }

  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(request->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping PDUSessionResourceSetupRequest. UE context does not exist",
                   request->ran_ue_ngap_id,
                   request->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier, logger, {}, {}, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[uint_to_ran_ue_id(request->ran_ue_ngap_id)];

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping PDUSessionResourceSetupRequest. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                uint_to_amf_ue_id(request->amf_ue_ngap_id)});
    return;
  }

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  if (!ue->is_security_enabled()) {
    ue_ctxt.logger.log_warning("Dropping PDUSessionResourceSetupRequest. Security context is not activated");
    send_error_indication(tx_pdu_notifier, logger, ue_ctxt.ue_ids.ran_ue_id, ue_ctxt.ue_ids.amf_ue_id, {});
    return;
  }

  // Stop PDU session setup timer.
  ue_ctxt.request_pdu_session_timer.stop();

  // Store information in UE context.
  if (request->ue_aggr_max_bit_rate_present) {
    ue->set_ue_ambr(aggregate_maximum_bit_rate_t{request->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_dl,
                                                 request->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_ul});
  }

  // Convert to common type.
  ngap_pdu_session_resource_setup_request msg;
  msg.ue_index = ue_ctxt.ue_ids.ue_index;
  if (ue_ctxt.serving_guami.has_value()) {
    msg.serving_plmn = ue_ctxt.serving_guami->plmn;
  }
  if (!fill_ngap_pdu_session_resource_setup_request(msg, request->pdu_session_res_setup_list_su_req)) {
    ue_ctxt.logger.log_warning("Conversion of PDUSessionResourceSetupRequest failed");
    send_error_indication(tx_pdu_notifier, logger, ue_ctxt.ue_ids.ran_ue_id, ue_ctxt.ue_ids.amf_ue_id, {});
    return;
  }
  msg.ue_ambr = ue->get_ue_ambr();

  // Start routine.
  ue->schedule_async_task(launch_async<ngap_pdu_session_resource_setup_procedure>(
      msg, request, ue_ctxt.ue_ids, cu_cp_notifier, metrics_handler, tx_pdu_notifier, ue_ctxt.logger));
}

void ngap_impl::handle_pdu_session_resource_modify_request(const asn1::ngap::pdu_session_res_modify_request_s& request)
{
  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(request->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping PDUSessionResourceModifyRequest. UE context does not exist",
                   request->ran_ue_ngap_id,
                   request->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier, logger, {}, {}, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[uint_to_ran_ue_id(request->ran_ue_ngap_id)];
  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping PDUSessionResourceModifyRequest. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                uint_to_amf_ue_id(request->amf_ue_ngap_id)});
    return;
  }

  // Check for duplicate messages.
  byte_buffer asn1_request_pdu = pack_into_pdu(request);
  if (asn1_request_pdu == ue_ctxt.last_pdu_session_resource_modify_request) {
    ue_ctxt.logger.log_warning("Received duplicate PDUSessionResourceModifyRequest");
    schedule_error_indication(ue_ctxt.ue_ids.ue_index, ngap_cause_radio_network_t::unspecified);
    return;
  }

  // Store last PDU session resource modify request.
  ue_ctxt.last_pdu_session_resource_modify_request = asn1_request_pdu.copy();

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  if (request->ran_paging_prio_present) {
    ue_ctxt.logger.log_debug("Not handling RAN paging prio");
  }

  // Convert to common type.
  ngap_pdu_session_resource_modify_request msg;
  msg.ue_index = ue_ctxt.ue_ids.ue_index;
  if (!fill_ngap_pdu_session_resource_modify_request(msg, request->pdu_session_res_modify_list_mod_req)) {
    ue_ctxt.logger.log_warning("Unable to fill ASN1 contents for PDUSessionResourceModifyRequest");
    schedule_error_indication(ue_ctxt.ue_ids.ue_index, ngap_cause_radio_network_t::unspecified);
    return;
  }

  // Start routine.
  ue->schedule_async_task(launch_async<ngap_pdu_session_resource_modify_procedure>(msg,
                                                                                   request,
                                                                                   ue_ctxt.ue_ids,
                                                                                   cu_cp_notifier,
                                                                                   tx_pdu_notifier,
                                                                                   get_ngap_control_message_handler(),
                                                                                   ue_ctxt.logger));
}

void ngap_impl::handle_pdu_session_resource_release_command(const asn1::ngap::pdu_session_res_release_cmd_s& command)
{
  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(command->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping PDUSessionResourceReleaseCommand. UE context does not exist",
                   command->ran_ue_ngap_id,
                   command->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier, logger, {}, {}, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[uint_to_ran_ue_id(command->ran_ue_ngap_id)];

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping PDUSessionResourceReleaseCommand. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                uint_to_amf_ue_id(command->amf_ue_ngap_id)});
    return;
  }

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  // Convert to common type.
  ngap_pdu_session_resource_release_command msg;
  msg.ue_index = ue_ctxt.ue_ids.ue_index;
  fill_ngap_pdu_session_resource_release_command(msg, command);

  // Start routine.
  ue->schedule_async_task(launch_async<ngap_pdu_session_resource_release_procedure>(
      msg, ue_ctxt.ue_ids, cu_cp_notifier, tx_pdu_notifier, ue_ctxt.logger));
}

void ngap_impl::handle_ue_context_release_command(const asn1::ngap::ue_context_release_cmd_s& cmd)
{
  amf_ue_id_t amf_ue_id = amf_ue_id_t::invalid;
  ran_ue_id_t ran_ue_id = ran_ue_id_t::invalid;
  if (cmd->ue_ngap_ids.type() == asn1::ngap::ue_ngap_ids_c::types_opts::amf_ue_ngap_id) {
    amf_ue_id = uint_to_amf_ue_id(cmd->ue_ngap_ids.amf_ue_ngap_id());

    if (!ue_ctxt_list.contains(amf_ue_id)) {
      // TS 38.413 section 8.3.3 doesn't specify abnormal conditions, so we just drop the message and send an error
      // indication.
      logger.warning("{}amf_ue={}: Dropping UeContextReleaseCommand. UE does not exist",
                     ran_ue_id == ran_ue_id_t::invalid ? "" : fmt::format("ran_ue={} ", fmt::underlying(ran_ue_id)),
                     amf_ue_id);
      send_error_indication(
          tx_pdu_notifier, logger, {}, amf_ue_id, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
      return;
    }
  } else if (cmd->ue_ngap_ids.type() == asn1::ngap::ue_ngap_ids_c::types_opts::ue_ngap_id_pair) {
    amf_ue_id = uint_to_amf_ue_id(cmd->ue_ngap_ids.ue_ngap_id_pair().amf_ue_ngap_id);
    ran_ue_id = uint_to_ran_ue_id(cmd->ue_ngap_ids.ue_ngap_id_pair().ran_ue_ngap_id);

    // Check whether the AMF UE ID is already associated with a different RAN UE ID in another context.
    if (not validate_consistent_ue_id_pair(ran_ue_id, amf_ue_id)) {
      // Release old UE contexts and send error indication with the received UE IDs to the AMF.
      handle_inconsistent_ue_id_pair(ran_ue_id, amf_ue_id);
      return;
    }

    if (!ue_ctxt_list.contains(ran_ue_id)) {
      // TS 38.413 section 8.3.3 doesn't specify abnormal conditions, so we just drop the message and send an error
      // indication.
      logger.warning("ran_ue={} amf_ue={}: Dropping UeContextReleaseCommand. UE does not exist",
                     fmt::underlying(ran_ue_id),
                     amf_ue_id);
      send_error_indication(
          tx_pdu_notifier, logger, {}, amf_ue_id, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
      return;
    }

    // Check whether the stored AMF UE ID for this RAN UE ID mismatches the received one.
    // Unlike other procedures, the release command must use the same AMF UE ID that was assigned to
    // this UE — a different value is an inconsistent remote UE NGAP ID per TS 38.413 section 10.6.
    const amf_ue_id_t stored_amf_ue_id = ue_ctxt_list[ran_ue_id].ue_ids.amf_ue_id;
    if (stored_amf_ue_id != amf_ue_id_t::invalid && stored_amf_ue_id != amf_ue_id) {
      handle_inconsistent_ue_id_pair(ran_ue_id, amf_ue_id);
      return;
    }

    // Update AMF UE ID if not yet known for this RAN UE context.
    if (stored_amf_ue_id == amf_ue_id_t::invalid) {
      ue_ctxt_list.update_amf_ue_id(ran_ue_id, amf_ue_id);
    }
  } else {
    // Neither an AMF UE ID nor a UE ID pair was provided.
    logger.warning("Dropping UeContextReleaseCommand. Unsupported UE-NGAP-IDs choice={}",
                   cmd->ue_ngap_ids.type().to_string());
    send_error_indication(tx_pdu_notifier, logger, {}, {}, cause_protocol_t::abstract_syntax_error_reject);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[amf_ue_id];

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping UeContextReleaseCommand. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                amf_ue_id});
    return;
  }

  ue_ctxt.release_scheduled = true;

  if (ran_ue_id == ran_ue_id_t::invalid) {
    ran_ue_id = ue_ctxt.ue_ids.ran_ue_id;
  }

  // Add AMF UE ID to UE, if its not set.
  if (ue_ctxt.ue_ids.amf_ue_id == amf_ue_id_t::invalid) {
    ue_ctxt_list.update_amf_ue_id(ran_ue_id, amf_ue_id);
  }

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  // Convert to common type.
  cu_cp_ue_context_release_command msg;
  msg.ue_index = ue_ctxt.ue_ids.ue_index;
  fill_cu_cp_ue_context_release_command(msg, cmd);

  // Start routine.
  ue->schedule_async_task(launch_async<ngap_ue_context_release_procedure>(
      msg, ue_ctxt.ue_ids, stored_error_indications, cu_cp_notifier, tx_pdu_notifier, ue_ctxt.logger));
}

void ngap_impl::handle_paging(const asn1::ngap::paging_s& msg)
{
  // Notify metrics handler about received paging request.
  metrics_handler.aggregate_cn_initiated_paging_request();

  // Validate Paging message.
  auto msgerr = validate_paging(msg);
  if (not msgerr.has_value()) {
    logger.warning("Dropping invalid Paging message. Cause: {}", msgerr.error());
    metrics_handler.aggregate_discarded_cn_initiated_paging_request();
    send_error_indication(tx_pdu_notifier, logger);
    return;
  }

  // Convert to common type.
  cu_cp_paging_message cu_cp_paging_msg;
  if (!fill_cu_cp_paging_message(cu_cp_paging_msg, msg)) {
    logger.warning("Dropping invalid Paging message");
    metrics_handler.aggregate_discarded_cn_initiated_paging_request();
    send_error_indication(tx_pdu_notifier, logger);
    return;
  }

  if (not cu_cp_notifier.on_paging_message(cu_cp_paging_msg)) {
    // Notify metrics handler about the paging request that no DU received.
    metrics_handler.aggregate_discarded_cn_initiated_paging_request();
  }
}

void ngap_impl::handle_write_replace_warning(const asn1::ngap::write_replace_warning_request_s& msg)
{
  ngap_write_replace_warning_request req = {};
  if (!fill_ngap_write_replace_warning_request(req, msg)) {
    logger.warning("Dropping invalid Write-Replace Warning Request");
    send_error_indication(tx_pdu_notifier, logger);
    return;
  }

  if (!cu_cp_notifier.schedule_async_task(launch_async(
          [this, req, resp = ngap_write_replace_warning_response{}](coro_context<async_task<void>>& ctx) mutable {
            CORO_BEGIN(ctx);
            CORO_AWAIT_VALUE(resp, cu_cp_notifier.on_write_replace_warning_request(req));
            send_write_replace_warning_response(resp);
            CORO_RETURN();
          }))) {
    logger.warning("Failed to schedule Write-Replace Warning procedure");
  }
}

void ngap_impl::send_write_replace_warning_response(const ngap_write_replace_warning_response& resp)
{
  ngap_message ngap_msg;
  ngap_msg.pdu.set_successful_outcome();
  ngap_msg.pdu.successful_outcome().load_info_obj(ASN1_NGAP_ID_WRITE_REPLACE_WARNING);
  fill_asn1_write_replace_warning_response(ngap_msg.pdu.successful_outcome().value.write_replace_warning_resp(), resp);

  if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
    logger.warning("Failed to send Write-Replace Warning Response");
  }
}

// Free function to generate a handover failure message.
static ngap_message generate_handover_failure(uint64_t amf_ue_id)
{
  ngap_message ngap_msg;
  ngap_msg.pdu.set_unsuccessful_outcome();
  ngap_msg.pdu.unsuccessful_outcome().load_info_obj(ASN1_NGAP_ID_HO_RES_ALLOC);
  auto& ho_fail           = ngap_msg.pdu.unsuccessful_outcome().value.ho_fail();
  ho_fail->amf_ue_ngap_id = amf_ue_id;
  ho_fail->cause.set_protocol();

  return ngap_msg;
}

void ngap_impl::send_handover_failure(uint64_t amf_ue_id)
{
  if (!tx_pdu_notifier.on_new_message(generate_handover_failure(amf_ue_id))) {
    logger.warning("Cannot send HandoverFailure");
    return;
  }
  logger.warning("Sending HandoverFailure");
}

void ngap_impl::handle_handover_request(const asn1::ngap::ho_request_s& msg)
{
  // Convert Handover Request to common type.
  ngap_handover_request ho_request;
  if (!fill_ngap_handover_request(ho_request, msg)) {
    logger.info("Received invalid HandoverRequest");
    send_handover_failure(msg->amf_ue_ngap_id);
    return;
  }

  logger.info("HandoverRequest - extracted target cell. plmn={}, target cell_id={}",
              ho_request.source_to_target_transparent_container.target_cell_id.plmn_id,
              ho_request.source_to_target_transparent_container.target_cell_id.nci);

  // Create UE in target cell.
  ho_request.ue_index = cu_cp_notifier.request_new_ue_index_allocation(
      ho_request.source_to_target_transparent_container.target_cell_id, ho_request.guami.plmn);
  if (ho_request.ue_index == cu_cp_ue_index_t::invalid) {
    logger.debug("Couldn't allocate UE index for handover target cell");
    send_handover_failure(msg->amf_ue_ngap_id);
    return;
  }

  // Inititialize security context of target UE.
  if (!cu_cp_notifier.on_handover_request_received(ho_request.ue_index, ho_request.security_context)) {
    send_handover_failure(msg->amf_ue_ngap_id);
    return;
  }

  if (!cu_cp_notifier.schedule_async_task(
          ho_request.ue_index,
          launch_async<ngap_handover_resource_allocation_procedure>(ho_request,
                                                                    uint_to_amf_ue_id(msg->amf_ue_ngap_id),
                                                                    ue_ctxt_list,
                                                                    cu_cp_notifier,
                                                                    tx_pdu_notifier,
                                                                    logger))) {
    logger.debug("Couldn't schedule handover resource allocation procedure");
    send_handover_failure(msg->amf_ue_ngap_id);
    return;
  }
}

void ngap_impl::handle_dl_ran_status_transfer(const asn1::ngap::dl_ran_status_transfer_s& msg)
{
  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(msg->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping DLRANStatusTransfer. UE context does not exist",
                   msg->ran_ue_ngap_id,
                   msg->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier, logger, {}, {}, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[uint_to_ran_ue_id(msg->ran_ue_ngap_id)];

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping DLRANStatusTransfer. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                uint_to_amf_ue_id(msg->amf_ue_ngap_id)});
    return;
  }

  ue_ctxt.ev_mng.dl_ran_status_transfer_outcome.set(msg);
}

void ngap_impl::handle_ul_ran_status_transfer(const cu_cp_status_transfer& ul_ran_status_transfer)
{
  const cu_cp_ue_index_t ue_index = ul_ran_status_transfer.ue_index;
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: Dropping ULRANStatusTransfer. UE context does not exist", ue_index);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UL_RAN_STATUS_TRANSFER);

  ul_ran_status_transfer_s& asn1_ul_status = ngap_msg.pdu.init_msg().value.ul_ran_status_transfer();
  asn1_ul_status->ran_ue_ngap_id           = to_underlying(ue_ctxt.ue_ids.ran_ue_id);
  asn1_ul_status->amf_ue_ngap_id           = to_underlying(ue_ctxt.ue_ids.amf_ue_id);

  fill_asn1_ul_ran_status_transfer(asn1_ul_status, ul_ran_status_transfer.drbs_subject_to_status_transfer_list);

  // Forward message to AMF.
  if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
    ue_ctxt.logger.log_warning("Cannot send ULRANStatusTransfer");
    return;
  }
}

async_task<expected<cu_cp_status_transfer>> ngap_impl::handle_dl_ran_status_transfer_required(cu_cp_ue_index_t ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: Cannot await DLRANStatusTransfer. UE context does not exist", ue_index);

    auto err_function = [](coro_context<async_task<expected<cu_cp_status_transfer>>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN(make_unexpected(default_error_t{}));
    };
    return launch_async(std::move(err_function));
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];
  return launch_async<ngap_dl_ran_status_transfer_procedure>(ue_ctxt.ev_mng, ue_ctxt.logger);
}

void ngap_impl::handle_dl_ue_associated_nrppa_transport(const asn1::ngap::dl_ue_associated_nrppa_transport_s& msg)
{
  // Store routing id.
  context.lmf_routing_id = msg->routing_id.copy();

  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(msg->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping DlUeAssociatedNrppaTransport. UE context does not exist",
                   msg->ran_ue_ngap_id,
                   msg->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier, logger, {}, {}, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[uint_to_ran_ue_id(msg->ran_ue_ngap_id)];

  if (ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_info("Dropping DlUeAssociatedNrppaTransport. UE is already scheduled for release");
    stored_error_indications.emplace(ue_ctxt.ue_ids.ue_index,
                                     error_indication_request_t{ngap_cause_radio_network_t::interaction_with_other_proc,
                                                                ue_ctxt.ue_ids.ran_ue_id,
                                                                uint_to_amf_ue_id(msg->amf_ue_ngap_id)});
    return;
  }

  // Forward to CU-CP.
  cu_cp_notifier.on_dl_ue_associated_nrppa_transport_pdu(ue_ctxt.ue_ids.ue_index, msg->nrppa_pdu);
}

void ngap_impl::handle_dl_non_ue_associated_nrppa_transport(
    const asn1::ngap::dl_non_ue_associated_nrppa_transport_s& msg)
{
  // Store routing id.
  context.lmf_routing_id = msg->routing_id.copy();

  // Forward to CU-CP.
  cu_cp_notifier.on_dl_non_ue_associated_nrppa_transport_pdu(context.amf_index, msg->nrppa_pdu);
}

void ngap_impl::handle_location_reporting_control_message(const asn1::ngap::location_report_ctrl_s& msg)
{
  if (!ue_ctxt_list.contains(uint_to_ran_ue_id(msg->ran_ue_ngap_id))) {
    logger.warning("ran_ue={} amf_ue={}: Dropping Location Reporting Control message. UE context does not exist",
                   msg->ran_ue_ngap_id,
                   msg->amf_ue_ngap_id);
    send_error_indication(tx_pdu_notifier,
                          logger,
                          {},
                          uint_to_amf_ue_id(msg->amf_ue_ngap_id),
                          ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
    return;
  }

  // Check whether another context doesn't exist already for the same AMF UE ID with mismatched RAN UE ID.
  if (not validate_consistent_ue_id_pair(uint_to_ran_ue_id(msg->ran_ue_ngap_id),
                                         uint_to_amf_ue_id(msg->amf_ue_ngap_id))) {
    // Release old UE context and send error indication with the received UE IDs to the AMF.
    handle_inconsistent_ue_id_pair(uint_to_ran_ue_id(msg->ran_ue_ngap_id), uint_to_amf_ue_id(msg->amf_ue_ngap_id));
    return;
  }
  cu_cp_ue_index_t        ue_index = ue_ctxt_list[uint_to_ran_ue_id(msg->ran_ue_ngap_id)].ue_ids.ue_index;
  location_report_request location_reporting_ctrl;

  if (!fill_ngap_location_report_request(location_reporting_ctrl, msg)) {
    logger.warning(
        "ran_ue={} amf_ue={}: Dropping invalid LocationReportingControl", msg->ran_ue_ngap_id, msg->amf_ue_ngap_id);
    send_error_indication(
        tx_pdu_notifier, logger, uint_to_ran_ue_id(msg->ran_ue_ngap_id), uint_to_amf_ue_id(msg->amf_ue_ngap_id));
    return;
  }
  cu_cp_notifier.on_location_reporting_control_message(ue_index, location_reporting_ctrl);
}

void ngap_impl::handle_error_indication(const asn1::ngap::error_ind_s& msg)
{
  amf_ue_id_t      amf_ue_id = amf_ue_id_t::invalid;
  ran_ue_id_t      ran_ue_id = ran_ue_id_t::invalid;
  cu_cp_ue_index_t ue_index  = cu_cp_ue_index_t::invalid;
  std::string      msg_cause;

  if (msg->cause_present) {
    msg_cause = asn1_cause_to_string(msg->cause);
  }

  if (msg->amf_ue_ngap_id_present) {
    amf_ue_id = uint_to_amf_ue_id(msg->amf_ue_ngap_id);
    if (!ue_ctxt_list.contains(uint_to_amf_ue_id(msg->amf_ue_ngap_id))) {
      logger.warning("amf_ue={}: Dropping ErrorIndication. UE context does not exist", msg->amf_ue_ngap_id);
      send_error_indication(
          tx_pdu_notifier, logger, {}, {}, ngap_cause_radio_network_t::inconsistent_remote_ue_ngap_id);
      return;
    }
    ue_index = ue_ctxt_list[amf_ue_id].ue_ids.ue_index;
  } else if (msg->ran_ue_ngap_id_present) {
    ran_ue_id = uint_to_ran_ue_id(msg->ran_ue_ngap_id);
    if (!ue_ctxt_list.contains(uint_to_ran_ue_id(msg->ran_ue_ngap_id))) {
      logger.warning("ran_ue={}: Dropping ErrorIndication. UE context does not exist", msg->ran_ue_ngap_id);
      send_error_indication(tx_pdu_notifier, logger, {}, {}, ngap_cause_radio_network_t::unknown_local_ue_ngap_id);
      return;
    }
    ue_index = ue_ctxt_list[ran_ue_id].ue_ids.ue_index;
  } else {
    logger.info("Received ErrorIndication{}", msg_cause.empty() ? "" : ". Cause: " + msg_cause);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  ue_ctxt.logger.log_info("Received ErrorIndication{}", msg_cause.empty() ? "" : ". Cause: " + msg_cause);

  // Release the UE locally if:
  // - an Error Indication was received while waiting for a UE Context Release Command: We consider the request failed.
  // - an Error Indication was received with cause "unknown-local-UE-NGAP-UE-ID": The UE was removed at the AMF.
  if (ue_ctxt.release_requested ||
      (msg->cause_present && msg->cause.type() == asn1::ngap::cause_c::types_opts::options::radio_network &&
       msg->cause.radio_network() == asn1::ngap::cause_radio_network_opts::options::unknown_local_ue_ngap_id)) {
    ue_ctxt.release_requested = false;
    ue_ctxt.release_scheduled = true;

    ue->schedule_async_task(
        launch_async([this, ue_index = ue_ctxt.ue_ids.ue_index](coro_context<async_task<void>>& ctx) {
          CORO_BEGIN(ctx);
          CORO_AWAIT(cu_cp_notifier.on_new_ue_context_release_command(
              {ue_index, ngap_cause_radio_network_t::release_due_to_5gc_generated_reason, true}));
          CORO_RETURN();
        }));
  }

  // Request UE release.
  ue->schedule_async_task(launch_async([this, ue_index](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);
    CORO_AWAIT(handle_ue_context_release_request(
        cu_cp_ue_context_release_request{ue_index, {}, ngap_cause_radio_network_t::unspecified}));
    CORO_RETURN();
  }));
}

void ngap_impl::handle_successful_outcome(const successful_outcome_s& outcome)
{
  auto get_ue_ctxt_in_ue_assoc_msg = [this](const asn1::ngap::successful_outcome_s& outcome_) -> ngap_ue_context* {
    std::optional<amf_ue_id_t> amf_ue_id = asn1_utils::get_amf_ue_id(outcome_);
    // The AMF_UE_NGAP_ID field is mandatory in all UE associated successful messages.
    if (!amf_ue_id.has_value()) {
      logger.warning("Discarding received \"{}\". Cause: AMF_UE_NGAP_ID field is mandatory",
                     outcome_.value.type().to_string());
      return nullptr;
    }

    ngap_ue_context* ue_ctxt = ue_ctxt_list.find(*amf_ue_id);
    if (ue_ctxt == nullptr) {
      logger.warning("amf_ue={}: Discarding received \"{}\". Cause: UE was not found.",
                     *amf_ue_id,
                     outcome_.value.type().to_string());
      return nullptr;
    }
    return ue_ctxt;
  };

  switch (outcome.value.type().value) {
    case ngap_elem_procs_o::successful_outcome_c::types_opts::ng_setup_resp: {
      ev_mng.ng_setup_outcome.set(outcome.value.ng_setup_resp());
    } break;
    case ngap_elem_procs_o::successful_outcome_c::types_opts::ng_reset_ack: {
      ev_mng.ng_reset_outcome.set(outcome.value.ng_reset_ack());
    } break;
    case ngap_elem_procs_o::successful_outcome_c::types_opts::ho_cmd: {
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.handover_preparation_outcome.set(outcome.value.ho_cmd());
      }
    } break;
    case ngap_elem_procs_o::successful_outcome_c::types_opts::ho_cancel_ack: {
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.handover_cancel_outcome.set(outcome.value.ho_cancel_ack());
      }
    } break;
    case ngap_elem_procs_o::successful_outcome_c::types_opts::path_switch_request_ack: {
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.path_switch_outcome.set(outcome.value.path_switch_request_ack());
      }
    } break;
    default:
      logger.error("Successful outcome of type {} is not supported", outcome.value.type().to_string());
  }
}

void ngap_impl::handle_unsuccessful_outcome(const unsuccessful_outcome_s& outcome)
{
  auto get_ue_ctxt_in_ue_assoc_msg = [this](const asn1::ngap::unsuccessful_outcome_s& outcome_) -> ngap_ue_context* {
    std::optional<amf_ue_id_t> amf_ue_id = asn1_utils::get_amf_ue_id(outcome_);
    // The AMF_UE_NGAP_ID field is mandatory in all UE associated failure messages.
    if (!amf_ue_id.has_value()) {
      logger.warning("Discarding received \"{}\". Cause: AMF_UE_NGAP_ID field is mandatory",
                     outcome_.value.type().to_string());
      return nullptr;
    }

    ngap_ue_context* ue_ctxt = ue_ctxt_list.find(*amf_ue_id);
    if (ue_ctxt == nullptr) {
      logger.warning("amf_ue={}: Discarding received \"{}\". Cause: UE was not found.",
                     *amf_ue_id,
                     outcome_.value.type().to_string());
      return nullptr;
    }
    return ue_ctxt;
  };

  switch (outcome.value.type().value) {
    case ngap_elem_procs_o::unsuccessful_outcome_c::types_opts::ng_setup_fail: {
      ev_mng.ng_setup_outcome.set(outcome.value.ng_setup_fail());
    } break;
    case ngap_elem_procs_o::unsuccessful_outcome_c::types_opts::ho_prep_fail: {
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.handover_preparation_outcome.set(outcome.value.ho_prep_fail());
      }
    } break;
    case ngap_elem_procs_o::unsuccessful_outcome_c::types_opts::path_switch_request_fail: {
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.path_switch_outcome.set(outcome.value.path_switch_request_fail());
      }
    } break;
    default:
      logger.error("Unsuccessful outcome of type {} is not supported", outcome.value.type().to_string());
  }
}

async_task<bool> ngap_impl::handle_ue_context_release_request(const cu_cp_ue_context_release_request& msg)
{
  if (!ue_ctxt_list.contains(msg.ue_index)) {
    logger.info("ue={}: Ignoring UEContextReleaseRequest. Cause: UE has no NGAP context", msg.ue_index);
    return launch_async([](coro_context<async_task<bool>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN(false);
    });
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[msg.ue_index];

  // Stop PDU session setup timer.
  ue_ctxt.request_pdu_session_timer.stop();

  if (ue_ctxt.ue_ids.amf_ue_id == amf_ue_id_t::invalid) {
    ue_ctxt.logger.log_debug("Ignoring UeContextReleaseRequest. UE does not have an AMF UE ID");
    return launch_async([](coro_context<async_task<bool>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN(false);
    });
  }

  if (ue_ctxt.release_requested or ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_debug("Ignoring UeContextReleaseRequest. Cause: Release {} already pending",
                             ue_ctxt.release_scheduled ? "command" : "request");
    return launch_async([](coro_context<async_task<bool>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN(true);
    });
  }

  ngap_message ngap_msg;
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UE_CONTEXT_RELEASE_REQUEST);

  auto& ue_context_release_request = ngap_msg.pdu.init_msg().value.ue_context_release_request();

  ue_context_release_request->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);
  ue_context_release_request->amf_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.amf_ue_id);

  fill_asn1_ue_context_release_request(ue_context_release_request, msg);

  // Forward message to AMF.
  // Mark UE so retx of request are avoided.
  ue_ctxt.release_requested = true;

  // Schedule transmission of UE Context Release Request.
  return launch_async([this, msg, ngap_msg](coro_context<async_task<bool>>& ctx) {
    CORO_BEGIN(ctx);

    if (!ue_ctxt_list.contains(msg.ue_index)) {
      logger.warning("ue={} ran_ue_id={} amf_ue_id={}: Dropping scheduled UeContextReleaseRequest. UE context does not "
                     "exist anymore",
                     msg.ue_index,
                     ngap_msg.pdu.init_msg().value.ue_context_release_request()->ran_ue_ngap_id,
                     ngap_msg.pdu.init_msg().value.ue_context_release_request()->amf_ue_ngap_id);
    } else {
      if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
        logger.error("ue={} ran_ue_id={} amf_ue_id={}: Cannot send UEContextReleaseRequest",
                     msg.ue_index,
                     ngap_msg.pdu.init_msg().value.ue_context_release_request()->ran_ue_ngap_id,
                     ngap_msg.pdu.init_msg().value.ue_context_release_request()->amf_ue_ngap_id);
        CORO_EARLY_RETURN(false);
      }
    }
    CORO_RETURN(true);
  });
}

async_task<ngap_handover_preparation_response>
ngap_impl::handle_handover_preparation_request(const ngap_handover_preparation_request& msg)
{
  auto err_function = [](coro_context<async_task<ngap_handover_preparation_response>>& ctx) {
    CORO_BEGIN(ctx);
    CORO_RETURN(ngap_handover_preparation_response{false});
  };

  if (!ue_ctxt_list.contains(msg.ue_index)) {
    logger.warning("ue={}: Dropping HandoverPreparationRequest. UE context does not exist", msg.ue_index);
    return launch_async(std::move(err_function));
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[msg.ue_index];

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  ue_ctxt.logger.log_info("Starting HO preparation");

  return launch_async<ngap_handover_preparation_procedure>(msg,
                                                           ue_ctxt.ue_ids,
                                                           tx_pdu_notifier,
                                                           ue->get_ngap_rrc_ue_notifier(),
                                                           cu_cp_notifier,
                                                           ue_ctxt.ev_mng,
                                                           timer_factory{timers, ctrl_exec},
                                                           ue_ctxt.logger);
}

void ngap_impl::handle_inter_cu_ho_rrc_recfg_complete(const cu_cp_ue_index_t             ue_index,
                                                      const cu_cp_user_location_info_nr& user_location_info)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: Dropping RrcReconfigurationComplete. UE context does not exist", ue_index);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_HO_NOTIF);

  auto& ho_notify           = ngap_msg.pdu.init_msg().value.ho_notify();
  ho_notify->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);
  ho_notify->amf_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.amf_ue_id);

  fill_asn1_handover_notify(ho_notify, user_location_info);

  // Forward message to AMF.
  if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
    ue_ctxt.logger.log_warning("Cannot send HandoverNotify");
    return;
  }
}

void ngap_impl::handle_ul_ue_associated_nrppa_transport(cu_cp_ue_index_t ue_index, const byte_buffer& nrppa_pdu)
{
  // Forward message to AMF.
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: Dropping ULUEAssociatedNRPPATransport. UE context does not exist", ue_index);
    return;
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UL_UE_ASSOCIATED_NRPPA_TRANSPORT);
  ngap_msg.pdu.init_msg().value.ul_ue_associated_nrppa_transport()->routing_id = context.lmf_routing_id.copy();
  ngap_msg.pdu.init_msg().value.ul_ue_associated_nrppa_transport()->nrppa_pdu  = nrppa_pdu.copy();
  ngap_msg.pdu.init_msg().value.ul_ue_associated_nrppa_transport()->amf_ue_ngap_id =
      to_underlying(ue_ctxt.ue_ids.amf_ue_id);
  ngap_msg.pdu.init_msg().value.ul_ue_associated_nrppa_transport()->ran_ue_ngap_id =
      to_underlying(ue_ctxt.ue_ids.ran_ue_id);

  auto* ue = ue_ctxt.get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
               ue_ctxt.ue_ids.ue_index,
               fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
               ue_ctxt.ue_ids.amf_ue_id);

  // Schedule transmission of UE associated NRPPA transport.
  ue->schedule_async_task(launch_async([this, ue_index, ngap_msg](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);

    if (!ue_ctxt_list.contains(ue_index)) {
      logger.warning("ue={} ran_ue={} amf_ue={}: Dropping scheduled ULUEAssociatedNRPPATransport. UE context does not "
                     "exist anymore",
                     ue_index,
                     ngap_msg.pdu.init_msg().value.ul_ue_associated_nrppa_transport()->ran_ue_ngap_id,
                     ngap_msg.pdu.init_msg().value.ul_ue_associated_nrppa_transport()->amf_ue_ngap_id);
    } else {
      if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
        logger.error("ue={} ran_ue={} amf_ue={}: Cannot send ULUEAssociatedNRPPATransport",
                     ue_index,
                     ngap_msg.pdu.init_msg().value.ul_ue_associated_nrppa_transport()->ran_ue_ngap_id,
                     ngap_msg.pdu.init_msg().value.ul_ue_associated_nrppa_transport()->amf_ue_ngap_id);
        CORO_EARLY_RETURN();
      }
    }
    CORO_RETURN();
  }));
}

async_task<void> ngap_impl::handle_ul_non_ue_associated_nrppa_transport(const byte_buffer& nrppa_pdu)
{
  // Forward message to AMF.
  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UL_NON_UE_ASSOCIATED_NRPPA_TRANSPORT);
  ngap_msg.pdu.init_msg().value.ul_non_ue_associated_nrppa_transport()->routing_id = context.lmf_routing_id.copy();
  ngap_msg.pdu.init_msg().value.ul_non_ue_associated_nrppa_transport()->nrppa_pdu  = nrppa_pdu.copy();

  return launch_async([this, ngap_msg](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);

    // Transmit non UE associated NRPPA transport.
    if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
      logger.warning("Cannot send ULnonUEAssociatedNRPPATransport");
      CORO_EARLY_RETURN();
    }
    CORO_RETURN();
  });
}

async_task<bool>
ngap_impl::handle_rrc_inactive_transition_report_required(const ngap_rrc_inactive_transition_report& report)
{
  const cu_cp_ue_index_t ue_index = report.ue_index;
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.warning("ue={}: Dropping RRCInactiveTransitionReport. UE context does not exist", ue_index);
    return launch_async([](coro_context<async_task<bool>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN(false);
    });
  }

  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  if (ue_ctxt.rrc_inactive_transition_report_request !=
      cu_cp_rrc_inactive_transition_report_request::subsequent_state_transition_report) {
    logger.debug("ue={}: Ignoring RRCInactiveTransitionReport. Cause: Report was not requested", ue_index);
    return launch_async([](coro_context<async_task<bool>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN(true);
    });
  }

  if (ue_ctxt.release_requested or ue_ctxt.release_scheduled) {
    ue_ctxt.logger.log_debug("Ignoring RRCInactiveTransitionReport. Cause: Release {} already pending",
                             ue_ctxt.release_scheduled ? "command" : "request");
    return launch_async([](coro_context<async_task<bool>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN(false);
    });
  }

  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_RRC_INACTIVE_TRANSITION_REPORT);

  rrc_inactive_transition_report_s& asn1_rrc_transition_report =
      ngap_msg.pdu.init_msg().value.rrc_inactive_transition_report();
  asn1_rrc_transition_report->ran_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.ran_ue_id);
  asn1_rrc_transition_report->amf_ue_ngap_id = to_underlying(ue_ctxt.ue_ids.amf_ue_id);

  fill_asn1_rrc_inactive_transition_report(asn1_rrc_transition_report, report);

  // Schedule transmission of RRC Inactive Transition Report.
  return launch_async([this, report, ngap_msg](coro_context<async_task<bool>>& ctx) {
    CORO_BEGIN(ctx);

    if (!ue_ctxt_list.contains(report.ue_index)) {
      logger.warning(
          "ue={} ran_ue_id={} amf_ue_id={}: Dropping scheduled RRCInactiveTransitionReport. UE context does not "
          "exist anymore",
          report.ue_index,
          ngap_msg.pdu.init_msg().value.rrc_inactive_transition_report()->ran_ue_ngap_id,
          ngap_msg.pdu.init_msg().value.rrc_inactive_transition_report()->amf_ue_ngap_id);
      CORO_EARLY_RETURN(false);
    }

    if (!tx_pdu_notifier.on_new_message(ngap_msg)) {
      logger.error("ue={} ran_ue_id={} amf_ue_id={}: Cannot send RRCInactiveTransitionReport",
                   report.ue_index,
                   ngap_msg.pdu.init_msg().value.rrc_inactive_transition_report()->ran_ue_ngap_id,
                   ngap_msg.pdu.init_msg().value.rrc_inactive_transition_report()->amf_ue_ngap_id);
      CORO_EARLY_RETURN(false);
    }

    CORO_RETURN(true);
  });
}

async_task<cu_cp_path_switch_response>
ngap_impl::handle_path_switch_request_required(const cu_cp_path_switch_request& request, const guami_t& serving_guami)
{
  cu_cp_path_switch_response        failure_response;
  cu_cp_path_switch_request_failure failure;
  failure.ue_index = request.ue_index;
  failure_response = failure;

  if (ue_ctxt_list.contains(request.ue_index)) {
    logger.warning("ue={}: Dropping Path Switch Request. UE context already exists", request.ue_index);
    return launch_no_op_task(failure_response);
  }

  // Create NGAP UE.
  // Allocate RAN-UE-ID.
  ran_ue_id_t ran_ue_id = ue_ctxt_list.allocate_ran_ue_id();
  if (ran_ue_id == ran_ue_id_t::invalid) {
    logger.error("ue={}: No RAN-UE-ID available, dropping Path Switch Request", request.ue_index);
    return launch_no_op_task(failure_response);
  }

  // Create UE context and store it.
  ngap_cu_cp_ue_notifier* ue_notifier = cu_cp_notifier.on_new_ngap_ue(request.ue_index);
  ue_ctxt_list.add_ue(request.ue_index, ran_ue_id, *ue_notifier);
  ue_ctxt_list.update_amf_ue_id(ran_ue_id, uint_to_amf_ue_id(request.source_amf_ue_ngap_id));

  // Store the GUAMI that the source NG-RAN node reported for the UE.
  ngap_ue_context& ue_ctxt = ue_ctxt_list[request.ue_index];
  ue_ctxt.serving_guami    = serving_guami;

  return launch_async<ngap_path_switch_procedure>(request, ue_ctxt, ue_ctxt_list, tx_pdu_notifier);
}

ngap_info ngap_impl::handle_ngap_metrics_report_request() const
{
  ngap_info ngap_info;
  ngap_info.amf_name        = context.amf_name;
  ngap_info.connected       = conn_handler.is_connected();
  ngap_info.supported_plmns = context.get_supported_plmns();
  ngap_info.metrics         = metrics_handler.request_metrics_report();
  return ngap_info;
}

void ngap_impl::remove_ue_context(cu_cp_ue_index_t ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.debug("ue={}: UE context not found", ue_index);
    return;
  }

  ue_ctxt_list.remove_ue_context(ue_index);
}

cu_cp_ue_index_t ngap_impl::get_ue_index(const amf_ue_id_t& amf_ue_ngap_id)
{
  if (ue_ctxt_list.contains(amf_ue_ngap_id)) {
    ngap_ue_context& ue_ctxt = ue_ctxt_list[amf_ue_ngap_id];
    return ue_ctxt.ue_ids.ue_index;
  }
  return cu_cp_ue_index_t::invalid;
}

amf_ue_id_t ngap_impl::get_amf_ue_id(const cu_cp_ue_index_t& ue_index)
{
  if (ue_ctxt_list.contains(ue_index)) {
    ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];
    return ue_ctxt.ue_ids.amf_ue_id;
  }
  return amf_ue_id_t::invalid;
}

void ngap_impl::schedule_error_indication(cu_cp_ue_index_t           ue_index,
                                          ngap_cause_t               cause,
                                          std::optional<amf_ue_id_t> amf_ue_id)
{
  ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];
  auto*            ue      = ue_ctxt.get_cu_cp_ue();

  ocudu_assert(ue != nullptr, "ue={} amf_ue={}: UE for UE context doesn't exist", ue_index, amf_ue_id);

  // The UE context can leave this NGAP before the task runs, so the task keeps the RAN UE NGAP ID.
  const ran_ue_id_t ran_ue_id = ue_ctxt.ue_ids.ran_ue_id;
  ue->schedule_async_task(launch_async([this, ran_ue_id, cause, amf_ue_id](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);
    send_error_indication(tx_pdu_notifier, logger, ran_ue_id, amf_ue_id, cause);
    CORO_RETURN();
  }));
}

void ngap_impl::on_request_pdu_session_timer_expired(cu_cp_ue_index_t ue_index)
{
  if (ue_ctxt_list.contains(ue_index)) {
    ngap_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

    auto* ue = ue_ctxt.get_cu_cp_ue();
    ocudu_assert(ue != nullptr,
                 "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
                 ue_ctxt.ue_ids.ue_index,
                 fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
                 ue_ctxt.ue_ids.amf_ue_id);

    if (ue_ctxt.ue_ids.amf_ue_id == amf_ue_id_t::invalid) {
      // AMF never responded to InitialUEMessage, so we only remove the UE from the DU.
      ue_ctxt.logger.log_info("UE did not request a PDU session after {}ms. Releasing UE from DU",
                              ue_ctxt.request_pdu_session_timer.duration().count());

      ue->schedule_async_task(launch_async([this, ue_index](coro_context<async_task<void>>& ctx) {
        CORO_BEGIN(ctx);
        CORO_AWAIT(
            cu_cp_notifier.on_new_ue_context_release_command({ue_index, ngap_cause_radio_network_t::unspecified}));
        CORO_RETURN();
      }));
    } else {
      ue_ctxt.logger.log_info("UE did not request a PDU session after {}ms. Requesting UE release",
                              ue_ctxt.request_pdu_session_timer.duration().count());

      // Request UE release.
      ue->schedule_async_task(launch_async([this, ue_index](coro_context<async_task<void>>& ctx) {
        CORO_BEGIN(ctx);
        CORO_AWAIT(handle_ue_context_release_request(
            cu_cp_ue_context_release_request{ue_index, {}, ngap_cause_radio_network_t::unspecified}));
        CORO_RETURN();
      }));
    }
  } else {
    logger.debug("ue={}: Ignoring expired PDU session setup timer. UE context not found", ue_index);
    return;
  }
}

bool ngap_impl::validate_consistent_ue_id_pair(ran_ue_id_t ran_ue_ngap_id, amf_ue_id_t amf_ue_ngap_id)
{
  // Check: does the AMF UE ID map to a different RAN UE ID?
  if (ue_ctxt_list.contains(amf_ue_ngap_id)) {
    ngap_ue_context& ue_ctxt = ue_ctxt_list[amf_ue_ngap_id];
    if (ue_ctxt.ue_ids.ran_ue_id != ran_ue_ngap_id) {
      logger.warning("Inconsistency detected in UE id pair. ue={} ran_ue={} amf_ue={} rx_ran_ue={} rx_amf_ue={} ",
                     ue_ctxt.ue_ids.ue_index,
                     fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
                     ue_ctxt.ue_ids.amf_ue_id,
                     fmt::underlying(ran_ue_ngap_id),
                     amf_ue_ngap_id);
      return false;
    }
  }
  return true;
}

void ngap_impl::handle_inconsistent_ue_id_pair(ran_ue_id_t ran_ue_ngap_id, amf_ue_id_t amf_ue_ngap_id)
{
  // Release context keyed by AMF UE ID if it maps to a different RAN UE ID.
  if (ue_ctxt_list.contains(amf_ue_ngap_id)) {
    ngap_ue_context& ue_ctxt = ue_ctxt_list[amf_ue_ngap_id];
    auto*            ue      = ue_ctxt.get_cu_cp_ue();
    ocudu_assert(ue != nullptr,
                 "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
                 ue_ctxt.ue_ids.ue_index,
                 fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
                 ue_ctxt.ue_ids.amf_ue_id);
    ue_ctxt.release_scheduled = true;
    ue->schedule_async_task(
        launch_async([this, ue_index = ue_ctxt.ue_ids.ue_index](coro_context<async_task<void>>& ctx) {
          CORO_BEGIN(ctx);
          CORO_AWAIT(cu_cp_notifier.on_new_ue_context_release_command(
              {ue_index, ngap_cause_radio_network_t::inconsistent_remote_ue_ngap_id, true}));
          CORO_RETURN();
        }));
  }

  // Also release context keyed by RAN UE ID if it has a mismatched (already known) AMF UE ID.
  // Per TS 38.413 section 10.6, when both received IDs exist in context but for different UEs
  // (e.g. an erroneous AMF mixing IDs from two UEs), both connections must be released.
  if (ue_ctxt_list.contains(ran_ue_ngap_id)) {
    ngap_ue_context& ue_ctxt = ue_ctxt_list[ran_ue_ngap_id];
    if (ue_ctxt.ue_ids.amf_ue_id != amf_ue_id_t::invalid && ue_ctxt.ue_ids.amf_ue_id != amf_ue_ngap_id) {
      auto* ue = ue_ctxt.get_cu_cp_ue();
      ocudu_assert(ue != nullptr,
                   "ue={} ran_ue={} amf_ue={}: UE for UE context doesn't exist",
                   ue_ctxt.ue_ids.ue_index,
                   fmt::underlying(ue_ctxt.ue_ids.ran_ue_id),
                   ue_ctxt.ue_ids.amf_ue_id);
      ue_ctxt.release_scheduled = true;
      ue->schedule_async_task(
          launch_async([this, ue_index = ue_ctxt.ue_ids.ue_index](coro_context<async_task<void>>& ctx) {
            CORO_BEGIN(ctx);
            CORO_AWAIT(cu_cp_notifier.on_new_ue_context_release_command(
                {ue_index, ngap_cause_radio_network_t::inconsistent_remote_ue_ngap_id, true}));
            CORO_RETURN();
          }));
    }
  }

  send_error_indication(tx_pdu_notifier,
                        logger,
                        ran_ue_ngap_id,
                        amf_ue_ngap_id,
                        ngap_cause_radio_network_t::inconsistent_remote_ue_ngap_id);
}

static auto log_pdu_helper(ocudulog::basic_logger&       logger,
                           bool                          json_log,
                           bool                          is_rx,
                           const ngap_ue_context_list&   ue_ctxt_list,
                           const asn1::ngap::ngap_pdu_c& pdu)
{
  if (not logger.info.enabled()) {
    return;
  }

  std::optional<ran_ue_id_t>      ran_ue_id = asn1_utils::get_ran_ue_id(pdu);
  std::optional<cu_cp_ue_index_t> ue_idx;
  if (ran_ue_id.has_value()) {
    const auto* ue = ue_ctxt_list.find(ran_ue_id.value());
    if (ue != nullptr) {
      ue_idx = ue->ue_ids.ue_index;
    }
  }

  log_ngap_pdu(logger, json_log, is_rx, ue_idx, pdu);
}

void ngap_impl::log_rx_pdu(const ngap_message& msg)
{
  log_pdu_helper(logger, logger.debug.enabled(), true, ue_ctxt_list, msg.pdu);
}

bool ngap_impl::tx_pdu_notifier_with_logging::on_new_message(const ngap_message& msg)
{
  log_pdu_helper(parent.logger, parent.logger.debug.enabled(), false, parent.ue_ctxt_list, msg.pdu);
  if (decorated == nullptr) {
    return false;
  }
  return decorated->on_new_message(msg);
}
