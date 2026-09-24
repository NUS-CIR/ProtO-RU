// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_cu_impl.h"
#include "asn1_helpers.h"
#include "f1ap_asn1_helpers.h"
#include "f1ap_asn1_utils.h"
#include "log_helpers.h"
#include "procedures/du_config_update_procedure.h"
#include "procedures/f1_removal_procedure.h"
#include "procedures/f1_setup_procedure.h"
#include "procedures/f1ap_e_cid_measurement_initiation_procedure.h"
#include "procedures/f1ap_positioning_activation_procedure.h"
#include "procedures/f1ap_positioning_asn1_converters.h"
#include "procedures/f1ap_positioning_information_exchange_procedure.h"
#include "procedures/f1ap_positioning_measurement_procedure.h"
#include "procedures/f1ap_stop_procedure.h"
#include "procedures/f1ap_trp_information_exchange_procedure.h"
#include "procedures/gnb_cu_configuration_update_procedure.h"
#include "procedures/ue_context_modification_procedure.h"
#include "procedures/ue_context_release_procedure.h"
#include "procedures/ue_context_setup_procedure.h"
#include "procedures/write_replace_warning_procedure.h"
#include "ocudu/adt/expected.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/ran/subcarrier_spacing.h"

using namespace ocudu;
using namespace ocucp;

f1ap_cu_impl::f1ap_cu_impl(const f1ap_configuration&    f1ap_cfg_,
                           f1ap_message_notifier&       tx_pdu_notifier_,
                           f1ap_du_processor_notifier&  f1ap_du_processor_notifier_,
                           gnb_cu_ue_f1ap_id_allocator& cu_ue_id_alloc_,
                           timer_manager&               timers_,
                           task_executor&               ctrl_exec_) :
  cfg(f1ap_cfg_),
  logger(ocudulog::fetch_basic_logger("CU-CP-F1")),
  ue_ctxt_list(cu_ue_id_alloc_, timer_factory{timers_, ctrl_exec_}, logger),
  du_processor_notifier(f1ap_du_processor_notifier_),
  ctrl_exec(ctrl_exec_),
  tx_pdu_notifier(*this, tx_pdu_notifier_),
  ev_mng(timer_factory{timers_, ctrl_exec_})
{
}

// Note: For fwd declaration of member types, dtor cannot be trivial.
f1ap_cu_impl::~f1ap_cu_impl() {}

async_task<void> f1ap_cu_impl::stop()
{
  return launch_async<f1ap_stop_procedure>(du_processor_notifier, ue_ctxt_list, ev_mng);
}

const f1ap_du_context& f1ap_cu_impl::get_context() const
{
  return du_ctxt;
}

void f1ap_cu_impl::handle_dl_rrc_message_transfer(const f1ap_dl_rrc_message& msg)
{
  f1ap_ue_context* ue_ctxt = ue_ctxt_list.find(msg.ue_index);
  if (ue_ctxt == nullptr) {
    logger.warning("ue={}: Dropping \"DLRRCMessageTransfer\". UE context does not exist", msg.ue_index);
    return;
  }

  ue_ctxt->handle_dl_rrc_message(msg, tx_pdu_notifier);
}

async_task<f1ap_ue_context_setup_response>
f1ap_cu_impl::handle_ue_context_setup_request(const f1ap_ue_context_setup_request&          request,
                                              const std::optional<rrc_ue_transfer_context>& rrc_context)
{
  return launch_async<ue_context_setup_procedure>(
      cfg, request, ue_ctxt_list, du_processor_notifier, tx_pdu_notifier, logger, rrc_context);
}

async_task<cu_cp_ue_index_t> f1ap_cu_impl::handle_ue_context_release_command(const f1ap_ue_context_release_command& msg)
{
  if (!ue_ctxt_list.contains(msg.ue_index)) {
    logger.warning("ue={}: Dropping \"UEContextReleaseCommand\". Cause: UE context does not exist", msg.ue_index);

    return launch_async([](coro_context<async_task<cu_cp_ue_index_t>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN(cu_cp_ue_index_t::invalid);
    });
  }

  return launch_async<ue_context_release_procedure>(cfg, msg, ue_ctxt_list[msg.ue_index], tx_pdu_notifier);
}

async_task<f1ap_ue_context_modification_response>
f1ap_cu_impl::handle_ue_context_modification_request(const f1ap_ue_context_modification_request& request)
{
  if (!ue_ctxt_list.contains(request.ue_index)) {
    logger.warning("ue={}: Dropping \"UEContextModificationRequest\". UE context does not exist", request.ue_index);

    return launch_async([](coro_context<async_task<f1ap_ue_context_modification_response>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN(f1ap_ue_context_modification_response{});
    });
  }

  return launch_async<ue_context_modification_procedure>(cfg, request, ue_ctxt_list[request.ue_index], tx_pdu_notifier);
}

bool f1ap_cu_impl::handle_ue_id_update(cu_cp_ue_index_t ue_index, cu_cp_ue_index_t old_ue_index)
{
  if (!ue_ctxt_list.contains(ue_index) or !ue_ctxt_list.contains(old_ue_index)) {
    return false;
  }

  // Mark that an old gNB-DU UE F1AP ID needs to be sent to the DU in the next DL RRC Message Transfer.
  ocudu_assert(ue_ctxt_list[old_ue_index].ue_ids.du_ue_f1ap_id &&
                   ue_ctxt_list[old_ue_index].ue_ids.du_ue_f1ap_id != gnb_du_ue_f1ap_id_t::invalid,
               "GNB-DU-UE-F1AP-ID should be valid");
  ue_ctxt_list[ue_index].pending_old_ue_id = *ue_ctxt_list[old_ue_index].ue_ids.du_ue_f1ap_id;
  return true;
}

void f1ap_cu_impl::handle_paging(const cu_cp_paging_message& msg)
{
  asn1::f1ap::paging_s paging = {};

  // Pack message into PDU.
  f1ap_message paging_msg;
  paging_msg.pdu.set_init_msg();
  paging_msg.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_PAGING);

  fill_asn1_paging_message(paging_msg.pdu.init_msg().value.paging(), msg);

  // Send message to DU.
  tx_pdu_notifier.on_new_message(paging_msg);
}

void f1ap_cu_impl::handle_message(const f1ap_message& msg)
{
  // Run F1AP protocols in Control executor.
  if (not ctrl_exec.execute([this, msg]() {
        // Log received message.
        log_pdu(true, msg);

        switch (msg.pdu.type().value) {
          case asn1::f1ap::f1ap_pdu_c::types_opts::init_msg:
            handle_initiating_message(msg.pdu.init_msg());
            break;
          case asn1::f1ap::f1ap_pdu_c::types_opts::successful_outcome:
            handle_successful_outcome(msg.pdu.successful_outcome());
            break;
          case asn1::f1ap::f1ap_pdu_c::types_opts::unsuccessful_outcome:
            handle_unsuccessful_outcome(msg.pdu.unsuccessful_outcome());
            break;
          default:
            logger.warning("Invalid PDU type");
            break;
        }
      })) {
    logger.warning("Discarding F1AP PDU. Cause: CU-CP task queue is full");
  }
}

void f1ap_cu_impl::remove_ue_context(cu_cp_ue_index_t ue_index)
{
  if (!ue_ctxt_list.contains(ue_index)) {
    logger.debug("ue={}: UE context not found", ue_index);
    return;
  }

  ue_ctxt_list.remove_ue(ue_index);
}

async_task<expected<trp_information_response_t, trp_information_failure_t>>
f1ap_cu_impl::handle_trp_information_request(const trp_information_request_t& request)
{
  logger.info("Handling TRP information request");
  return launch_async<f1ap_trp_information_exchange_procedure>(cfg, request, ev_mng, tx_pdu_notifier, logger);
}

async_task<expected<positioning_information_response_t, positioning_information_failure_t>>
f1ap_cu_impl::handle_positioning_information_request(const positioning_information_request_t& request)
{
  logger.info("Handling positioning information request");

  if (!ue_ctxt_list.contains(request.ue_index)) {
    logger.warning("ue={}: Dropping \"UEPositioningInformationRequest\". UE context does not exist", request.ue_index);

    return launch_async(
        [](coro_context<async_task<expected<positioning_information_response_t, positioning_information_failure_t>>>&
               ctx) mutable {
          CORO_BEGIN(ctx);
          CORO_RETURN(make_unexpected(positioning_information_failure_t{}));
        });
  }

  return launch_async<f1ap_positioning_information_exchange_procedure>(
      cfg, request, ue_ctxt_list[request.ue_index], tx_pdu_notifier, logger);
}

async_task<expected<positioning_activation_response_t, positioning_activation_failure_t>>
f1ap_cu_impl::handle_positioning_activation_request(const positioning_activation_request_t& request)
{
  if (!ue_ctxt_list.contains(request.ue_index)) {
    logger.warning("ue={}: Dropping \"UEPositioningActivationRequest\". UE context does not exist", request.ue_index);

    return launch_async(
        [](coro_context<async_task<expected<positioning_activation_response_t, positioning_activation_failure_t>>>&
               ctx) mutable {
          CORO_BEGIN(ctx);
          CORO_RETURN(make_unexpected(positioning_activation_failure_t{}));
        });
  }

  f1ap_ue_context& ue_ctxt = ue_ctxt_list[request.ue_index];
  ue_ctxt.logger.log_info("Handling positioning activation request");

  return launch_async<f1ap_positioning_activation_procedure>(cfg, request, ue_ctxt, tx_pdu_notifier, logger);
}

async_task<expected<measurement_response_t, measurement_failure_t>>
f1ap_cu_impl::handle_positioning_measurement_request(const measurement_request_t& request)
{
  logger.info("Handling positioning measurement request");
  return launch_async<f1ap_positioning_measurement_procedure>(cfg, request, ev_mng, tx_pdu_notifier, logger);
}

async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>
f1ap_cu_impl::handle_e_cid_measurement_request(const e_cid_measurement_request_t& request)
{
  if (!ue_ctxt_list.contains(request.ue_index)) {
    logger.warning("ue={}: Dropping \"ECIDMeasurementInitiationRequest\". UE context does not exist", request.ue_index);

    return launch_async(
        [request](coro_context<async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>>&
                      ctx) mutable {
          CORO_BEGIN(ctx);
          CORO_RETURN(make_unexpected(e_cid_measurement_failure_t{
              request.lmf_ue_meas_id, request.ran_ue_meas_id, f1ap_cause_t{cause_misc_t::unspecified}}));
        });
  }

  f1ap_ue_context& ue_ctxt = ue_ctxt_list[request.ue_index];
  ue_ctxt.logger.log_info("Handling E-CID measurement initiation request");

  return launch_async<f1ap_e_cid_measurement_initiation_procedure>(cfg, request, ue_ctxt, tx_pdu_notifier, logger);
}

async_task<f1ap_gnb_cu_configuration_update_response>
f1ap_cu_impl::handle_gnb_cu_configuration_update(const f1ap_gnb_cu_configuration_update& request)
{
  return launch_async<gnb_cu_configuration_update_procedure>(cfg, request, tx_pdu_notifier, ev_mng, logger);
}

async_task<f1ap_write_replace_warning_response>
f1ap_cu_impl::handle_write_replace_warning_request(const f1ap_write_replace_warning_request& request)
{
  return launch_async<write_replace_warning_procedure>(cfg, request, tx_pdu_notifier, ev_mng, logger);
}

void f1ap_cu_impl::handle_initiating_message(const asn1::f1ap::init_msg_s& msg)
{
  switch (msg.value.type().value) {
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::options::f1_setup_request:
      handle_f1_setup_request(msg.value.f1_setup_request());
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::init_ul_rrc_msg_transfer:
      handle_initial_ul_rrc_message(msg.value.init_ul_rrc_msg_transfer());
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::ul_rrc_msg_transfer:
      handle_ul_rrc_message(msg.value.ul_rrc_msg_transfer());
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::f1_removal_request:
      handle_f1_removal_request(msg);
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::options::ue_context_release_request:
      handle_ue_context_release_request(msg.value.ue_context_release_request());
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::options::access_success:
      handle_access_success(msg.value.access_success());
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::options::gnb_du_cfg_upd:
      handle_du_cfg_update(msg.value.gnb_du_cfg_upd());
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::options::ref_time_info_report:
      handle_ref_time_info_report(msg.value.ref_time_info_report());
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::options::e_c_id_meas_report:
      handle_e_cid_measurement_report(msg.value.e_c_id_meas_report());
      break;
    case asn1::f1ap::f1ap_elem_procs_o::init_msg_c::types_opts::options::e_c_id_meas_fail_ind:
      handle_e_cid_measurement_failure_indication(msg.value.e_c_id_meas_fail_ind());
      break;
    default:
      logger.warning("Initiating message of type {} is not supported", msg.value.type().to_string());
      break;
  }
}

void f1ap_cu_impl::handle_access_success(const asn1::f1ap::access_success_s& msg)
{
  if (!ue_ctxt_list.contains(int_to_gnb_cu_ue_f1ap_id(msg->gnb_cu_ue_f1ap_id))) {
    logger.warning("cu_ue={} du_ue={}: Dropping \"AccessSuccess\". UE context does not exist",
                   msg->gnb_cu_ue_f1ap_id,
                   msg->gnb_du_ue_f1ap_id);
    return;
  }

  f1ap_ue_context& ue_ctxt = ue_ctxt_list[int_to_gnb_cu_ue_f1ap_id(msg->gnb_cu_ue_f1ap_id)];
  ue_ctxt.logger.log_debug("Received \"AccessSuccess\"");

  auto cgi = cgi_from_asn1(msg->nr_cgi);
  if (!cgi.has_value()) {
    logger.warning("cu_ue={}: Dropping \"AccessSuccess\". Cause: Failed to parse NR-CGI", msg->gnb_cu_ue_f1ap_id);
    return;
  }

  f1ap_access_success ind;
  ind.ue_index = ue_ctxt.ue_ids.ue_index;
  ind.cgi      = cgi.value();

  du_processor_notifier.on_access_success(ind);
}

void f1ap_cu_impl::handle_du_cfg_update(const asn1::f1ap::gnb_du_cfg_upd_s& request)
{
  handle_du_config_update_procedure(request, du_ctxt, tx_pdu_notifier, du_processor_notifier, logger);
}

void f1ap_cu_impl::handle_f1_setup_request(const asn1::f1ap::f1_setup_request_s& request)
{
  bool accepted = handle_f1_setup_procedure(request, du_ctxt, tx_pdu_notifier, du_processor_notifier, logger);
  if (accepted && cfg.ref_time_report_ctrl) {
    handle_ref_time_info_report_ctrl(*cfg.ref_time_report_ctrl);
  }
}

void f1ap_cu_impl::handle_f1_removal_request(const asn1::f1ap::init_msg_s& msg)
{
  // The IE container of the F1 Removal Request can be empty. Discard the message if the Transaction ID IE is missing,
  // as per TS 38.473 section 10.
  std::optional<uint8_t> transaction_id = get_transaction_id(msg);
  if (not transaction_id.has_value()) {
    logger.warning("Discarding received \"F1RemovalRequest\". Cause: Missing mandatory Transaction ID field");
    return;
  }

  du_processor_notifier.schedule_async_task(launch_async<f1_removal_procedure>(
      transaction_id.value(), tx_pdu_notifier, du_processor_notifier, ue_ctxt_list, logger));
}

void f1ap_cu_impl::handle_initial_ul_rrc_message(const asn1::f1ap::init_ul_rrc_msg_transfer_s& msg)
{
  const gnb_du_ue_f1ap_id_t du_ue_id = int_to_gnb_du_ue_f1ap_id(msg->gnb_du_ue_f1ap_id);

  expected<nr_cell_global_id_t> cgi = cgi_from_asn1(msg->nr_cgi);
  if (not cgi.has_value()) {
    logger.warning("du_ue={}: Dropping \"InitialULRRCMessageTransfer\". Invalid CGI", fmt::underlying(du_ue_id));
    return;
  }

  rnti_t crnti = to_rnti(msg->c_rnti);
  if (crnti == rnti_t::INVALID_RNTI) {
    logger.warning("du_ue={}: Dropping \"InitialULRRCMessageTransfer\". Cause: Invalid C-RNTI",
                   fmt::underlying(du_ue_id));
    return;
  }

  if (msg->sul_access_ind_present) {
    logger.debug("du_ue={}: Ignoring SUL access indicator", fmt::underlying(du_ue_id));
  }

  if (msg->rrc_container_rrc_setup_complete_present) {
    logger.warning("du_ue={}: Ignoring RRC Container RRCSetupComplete. Cause: Network Sharing with multiple cell-ID "
                   "broadcast is not supported",
                   fmt::underlying(du_ue_id));
  }

  const gnb_cu_ue_f1ap_id_t cu_ue_f1ap_id = ue_ctxt_list.allocate_gnb_cu_ue_f1ap_id();
  if (cu_ue_f1ap_id == gnb_cu_ue_f1ap_id_t::invalid) {
    logger.warning("du_ue={}: Dropping \"InitialULRRCMessageTransfer\". Cause: Failed to allocate CU-UE-F1AP-ID",
                   fmt::underlying(du_ue_id));
    return;
  }

  // Request UE index allocation from DU processor.
  cu_cp_ue_index_t ue_index = du_processor_notifier.request_new_ue_creation();
  if (ue_index == cu_cp_ue_index_t::invalid) {
    logger.warning("du_ue={}: Dropping \"InitialULRRCMessageTransfer\". Cause: Failed to create UE context",
                   fmt::underlying(du_ue_id));
    ue_ctxt_list.release_gnb_cu_ue_f1ap_id(cu_ue_f1ap_id);
    return;
  }

  // Add UE context.
  f1ap_ue_context& ue_ctxt = ue_ctxt_list.add_ue(ue_index, cu_ue_f1ap_id);
  ue_ctxt_list.add_du_ue_f1ap_id(cu_ue_f1ap_id, du_ue_id);

  // Request RRC UE entity creation.
  ue_rrc_context_creation_request req;
  req.ue_index  = ue_index;
  req.c_rnti    = crnti;
  req.cgi       = cgi.value();
  req.is_redcap = msg->nr_red_cap_ue_ind_present;
  if (msg->du_to_cu_rrc_container_present) {
    req.du_to_cu_rrc_container = byte_buffer(msg->du_to_cu_rrc_container);
  } else {
    // Assume the DU can't serve the UE, so the CU-CP should reject the UE, see TS 38.473 section 8.4.1.2.
    // We will forward an empty container to the RRC UE entity, that will trigger an RRC Reject.
    logger.debug("du_ue={}: Forwarding \"InitialULRRCMessageTransfer\" to RRC to reject the UE. Cause: Missing DU "
                 "to CU container",
                 fmt::underlying(du_ue_id));
  }
  req.rrc_container                    = msg->rrc_container.copy();
  ue_rrc_context_creation_outcome resp = du_processor_notifier.on_ue_rrc_context_creation_request(req);

  // Return if the UE creation was not successful.
  // Note: The higher layers will release the UE context, so we don't need to do anything here and must return directly.
  if (not resp.has_value()) {
    return;
  }

  // If the UE context creation was successful, create/update the UE context in the CU-CP and forward the RRC container
  // to RRC. Otherwise the UE will be rejected and the context will be released by higher layers.
  if (resp->ue_index != ue_index && ue_ctxt_list.contains(resp->ue_index)) {
    // This is a resumed UE. Update the existing context with the new DU UE F1AP ID.
    f1ap_ue_context& resume_ue_ctxt = ue_ctxt_list[resp->ue_index];
    ue_ctxt_list.add_du_ue_f1ap_id(resume_ue_ctxt.ue_ids.cu_ue_f1ap_id, du_ue_id);

    // Reset release state so the context can be released again if needed (e.g. RRC Resume with invalid MAC).
    resume_ue_ctxt.reset_release_state(); // TODO: This is a temporary solution, F1AP context should not be reused

    resume_ue_ctxt.logger.log_info("Updated resumed UE context with new DU UE F1AP ID");

    // Note: We the newly created UE will be released and removed by higher layers.
  } else {
    // Add SRB notifiers to the UE context.
    ue_ctxt_list.add_srb0_rrc_notifier(resp->ue_index, resp->f1ap_srb0_notifier);
    ue_ctxt_list.add_srb1_rrc_notifier(resp->ue_index, resp->f1ap_srb1_notifier);
    ue_ctxt_list.add_srb2_rrc_notifier(resp->ue_index, resp->f1ap_srb2_notifier);

    ue_ctxt.logger.log_info("Added UE context");
  }

  // Forward RRC container.
  std::variant<f1c_initial_ul_bearer_handler*, f1c_ul_bearer_handler*> bearer_var =
      ue_ctxt_list[resp->ue_index].get_ul_bearer_manager().get_srb(srb_id_t::srb0);

  ocudu_assert(std::holds_alternative<f1c_initial_ul_bearer_handler*>(bearer_var), "Incorrect SRB type for SRB0");
  auto* bearer = std::get<f1c_initial_ul_bearer_handler*>(bearer_var);
  ocudu_assert(bearer, "SRB0 should be always active");
  bearer->handle_initial_ul_rrc_message(msg->rrc_container.copy(), crnti);
}

void f1ap_cu_impl::handle_ul_rrc_message(const asn1::f1ap::ul_rrc_msg_transfer_s& msg)
{
  f1ap_ue_context* ue_ctxt = ue_ctxt_list.find(int_to_gnb_cu_ue_f1ap_id(msg->gnb_cu_ue_f1ap_id));
  if (ue_ctxt == nullptr) {
    logger.warning("cu_ue={} du_ue={}: Dropping \"ULRRCMessageTransfer\". UE context does not exist",
                   msg->gnb_cu_ue_f1ap_id,
                   msg->gnb_du_ue_f1ap_id);
    return;
  }

  if (int_to_srb_id(msg->srb_id) == srb_id_t::srb0) {
    logger.warning("cu_ue={} du_ue={}: Dropping \"ULRRCMessageTransfer\". Using SRB0 for non-initial UL RRC message",
                   msg->gnb_cu_ue_f1ap_id,
                   msg->gnb_du_ue_f1ap_id);
    return;
  }

  std::variant<f1c_initial_ul_bearer_handler*, f1c_ul_bearer_handler*> bearer_var =
      ue_ctxt->get_ul_bearer_manager().get_srb(int_to_srb_id(msg->srb_id));

  ocudu_assert(std::holds_alternative<f1c_ul_bearer_handler*>(bearer_var),
               "Incorrect SRB type for {}",
               int_to_srb_id(msg->srb_id));

  auto* bearer = std::get<f1c_ul_bearer_handler*>(bearer_var);
  if (bearer == nullptr) {
    logger.warning("cu_ue={} du_ue={}: Dropping \"ULRRCMessageTransfer\". Cause: SRB{} has not been activated",
                   msg->gnb_cu_ue_f1ap_id,
                   msg->gnb_du_ue_f1ap_id,
                   int_to_srb_id(msg->srb_id));
    return;
  }
  bearer->handle_ul_rrc_message(msg->rrc_container.copy());
}

void f1ap_cu_impl::handle_ue_context_release_request(const asn1::f1ap::ue_context_release_request_s& msg)
{
  if (!ue_ctxt_list.contains(int_to_gnb_cu_ue_f1ap_id(msg->gnb_cu_ue_f1ap_id))) {
    logger.warning("cu_ue={} du_ue={}: Dropping \"UeContextReleaseRequest\". UE context does not exist",
                   msg->gnb_cu_ue_f1ap_id,
                   msg->gnb_du_ue_f1ap_id);
    return;
  }

  f1ap_ue_context& ue_ctxt = ue_ctxt_list[int_to_gnb_cu_ue_f1ap_id(msg->gnb_cu_ue_f1ap_id)];

  if (ue_ctxt.marked_for_release) {
    // UE context is already being released. Ignore the request.
    ue_ctxt.logger.log_debug("\"UeContextReleaseRequest\" ignored. UE context release procedure has already started");
    return;
  }

  ue_ctxt.logger.log_debug("Received \"UeContextReleaseRequest\"");

  f1ap_ue_context_release_request req;
  req.ue_index = ue_ctxt.ue_ids.ue_index;
  req.cause    = asn1_to_cause(msg->cause);

  du_processor_notifier.on_du_initiated_ue_context_release_request(req);
}

void f1ap_cu_impl::handle_successful_outcome(const asn1::f1ap::successful_outcome_s& outcome)
{
  auto get_ue_ctxt_in_ue_assoc_msg = [this](const asn1::f1ap::successful_outcome_s& outcome_) -> f1ap_ue_context* {
    std::optional<gnb_cu_ue_f1ap_id_t> cu_ue_id = get_gnb_cu_ue_f1ap_id(outcome_);
    // The GNB-CU-UE-F1AP-ID field is mandatory in all UE associated successful messages.
    if (!cu_ue_id.has_value()) {
      logger.warning("Discarding received \"{}\". Cause: Missing mandatory GNB-CU-UE-F1AP-ID field",
                     outcome_.value.type().to_string());
      return nullptr;
    }

    f1ap_ue_context* ue_ctxt = ue_ctxt_list.find(*cu_ue_id);
    if (ue_ctxt == nullptr) {
      logger.warning("cu_ue={}: Discarding received \"{}\". Cause: UE was not found",
                     fmt::underlying(*cu_ue_id),
                     outcome_.value.type().to_string());
      return nullptr;
    }
    return ue_ctxt;
  };

  std::optional<uint8_t> transaction_id = std::nullopt;

  switch (outcome.value.type().value) {
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::ue_context_release_complete:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.context_release_complete.set(outcome.value.ue_context_release_complete());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::ue_context_setup_resp:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.context_setup_outcome.set(outcome.value.ue_context_setup_resp());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::ue_context_mod_resp:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.context_modification_outcome.set(outcome.value.ue_context_mod_resp());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::trp_info_resp:
      transaction_id = get_transaction_id(outcome);
      if (not transaction_id.has_value()) {
        logger.error("Successful outcome of type {} is not supported", outcome.value.type().to_string());
        break;
      }
      if (not ev_mng.transactions.set_response(transaction_id.value(), outcome)) {
        logger.warning("Unexpected transaction id={}", transaction_id.value());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::positioning_info_resp:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.positioning_information_outcome.set(outcome.value.positioning_info_resp());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::e_c_id_meas_initiation_resp:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.e_cid_measurement_outcome.set(outcome.value.e_c_id_meas_initiation_resp());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::positioning_activation_resp:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.positioning_activation_outcome.set(outcome.value.positioning_activation_resp());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::positioning_meas_resp:
      transaction_id = get_transaction_id(outcome);
      if (not transaction_id.has_value()) {
        logger.error("Successful outcome of type {} is not supported", outcome.value.type().to_string());
        break;
      }
      if (not ev_mng.transactions.set_response(transaction_id.value(), outcome)) {
        logger.warning("Unexpected transaction id={}", transaction_id.value());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::gnb_cu_cfg_upd_ack:
      transaction_id = get_transaction_id(outcome);
      if (not transaction_id.has_value()) {
        logger.error("Successful outcome of type {} is not supported", outcome.value.type().to_string());
        break;
      }
      if (not ev_mng.transactions.set_response(transaction_id.value(), outcome)) {
        logger.warning("Unexpected transaction id={}", transaction_id.value());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::successful_outcome_c::types_opts::write_replace_warning_resp:
      transaction_id = get_transaction_id(outcome);
      if (not transaction_id.has_value()) {
        logger.error("Successful outcome of type {} is not supported", outcome.value.type().to_string());
        break;
      }
      if (not ev_mng.transactions.set_response(transaction_id.value(), outcome)) {
        logger.warning("Unexpected transaction id={}", transaction_id.value());
      }
      break;
    default:
      logger.warning("Successful outcome of type {} is not supported", outcome.value.type().to_string());
      break;
  }
}

void f1ap_cu_impl::handle_unsuccessful_outcome(const asn1::f1ap::unsuccessful_outcome_s& outcome)
{
  auto get_ue_ctxt_in_ue_assoc_msg = [this](const asn1::f1ap::unsuccessful_outcome_s& outcome_) -> f1ap_ue_context* {
    std::optional<gnb_cu_ue_f1ap_id_t> cu_ue_id = get_gnb_cu_ue_f1ap_id(outcome_);
    // The GNB-CU-UE-F1AP-ID field is mandatory in all UE associated failure messages.
    ocudu_assert(cu_ue_id,
                 "Discarding received \"{}\". Cause: GNB-CU-UE-F1AP-ID field is mandatory",
                 outcome_.value.type().to_string());

    f1ap_ue_context* ue_ctxt = ue_ctxt_list.find(*cu_ue_id);
    if (ue_ctxt == nullptr) {
      logger.warning("cu_ue={}: Discarding received \"{}\". Cause: UE was not found.",
                     fmt::underlying(*cu_ue_id),
                     outcome_.value.type().to_string());
      return nullptr;
    }
    return ue_ctxt;
  };

  std::optional<uint8_t> transaction_id = std::nullopt;

  switch (outcome.value.type().value) {
    case asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::ue_context_setup_fail:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.context_setup_outcome.set(outcome.value.ue_context_setup_fail());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::ue_context_mod_fail:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.context_modification_outcome.set(outcome.value.ue_context_mod_fail());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::trp_info_fail:
      transaction_id = get_transaction_id(outcome);
      if (not transaction_id.has_value()) {
        logger.error("Unsuccessful outcome of type {} is not supported", outcome.value.type().to_string());
        break;
      }
      if (not ev_mng.transactions.set_response(transaction_id.value(), make_unexpected(outcome))) {
        logger.warning("Unexpected transaction id={}", transaction_id.value());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::positioning_info_fail:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.positioning_information_outcome.set(outcome.value.positioning_info_fail());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::e_c_id_meas_initiation_fail:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.e_cid_measurement_outcome.set(outcome.value.e_c_id_meas_initiation_fail());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::positioning_activation_fail:
      if (auto* ue_ctxt = get_ue_ctxt_in_ue_assoc_msg(outcome)) {
        ue_ctxt->ev_mng.positioning_activation_outcome.set(outcome.value.positioning_activation_fail());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::positioning_meas_fail:
      transaction_id = get_transaction_id(outcome);
      if (not transaction_id.has_value()) {
        logger.error("Unsuccessful outcome of type {} is not supported", outcome.value.type().to_string());
        break;
      }
      if (not ev_mng.transactions.set_response(transaction_id.value(), make_unexpected(outcome))) {
        logger.warning("Unexpected transaction id={}", transaction_id.value());
      }
      break;
    case asn1::f1ap::f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::gnb_cu_cfg_upd_fail:
      transaction_id = get_transaction_id(outcome);
      if (not transaction_id.has_value()) {
        logger.error("Unsuccessful outcome of type {} is not supported", outcome.value.type().to_string());
        break;
      }
      if (not ev_mng.transactions.set_response(transaction_id.value(), make_unexpected(outcome))) {
        logger.warning("Unexpected transaction id={}", transaction_id.value());
      }
      break;
    default:
      logger.warning("Unsuccessful outcome of type {} is not supported", outcome.value.type().to_string());
      break;
  }
}

void f1ap_cu_impl::handle_ref_time_info_report(const asn1::f1ap::ref_time_info_report_s& msg)
{
  logger.debug("Received \"ReferenceTimeInformationReport\"");

  f1ap_time_ref_info info;
  info.ref_time_r16 = msg->time_ref_info.ref_time.copy();
  info.ref_slot     = slot_point{subcarrier_spacing::kHz15, msg->time_ref_info.ref_sfn, 0};
  if (msg->time_ref_info.uncertainty_present) {
    info.uncertainty = msg->time_ref_info.uncertainty;
  }
  info.is_local_clock = msg->time_ref_info.time_info_type_present;

  du_processor_notifier.on_ref_time_info_report(info);
}

void f1ap_cu_impl::handle_e_cid_measurement_report(const asn1::f1ap::e_c_id_meas_report_s& msg)
{
  f1ap_ue_context* ue_ctxt = ue_ctxt_list.find(int_to_gnb_cu_ue_f1ap_id(msg->gnb_cu_ue_f1ap_id));
  if (ue_ctxt == nullptr) {
    logger.warning("cu_ue={} du_ue={}: Dropping \"ECIDMeasurementReport\". UE context does not exist",
                   msg->gnb_cu_ue_f1ap_id,
                   msg->gnb_du_ue_f1ap_id);
    return;
  }

  e_cid_measurement_report_t report;
  report.ue_index          = ue_ctxt->ue_ids.ue_index;
  report.lmf_ue_meas_id    = uint_to_lmf_ue_meas_id(msg->lmf_ue_meas_id);
  report.ran_ue_meas_id    = uint_to_ran_ue_meas_id(msg->ran_ue_meas_id);
  report.e_cid_meas_result = fill_e_cid_measurement_result(msg->e_c_id_meas_result);
  if (msg->cell_portion_id_present) {
    report.cell_portion_id = msg->cell_portion_id;
  }

  du_processor_notifier.on_e_cid_measurement_report(report);
}

void f1ap_cu_impl::handle_e_cid_measurement_failure_indication(const asn1::f1ap::e_c_id_meas_fail_ind_s& msg)
{
  f1ap_ue_context* ue_ctxt = ue_ctxt_list.find(int_to_gnb_cu_ue_f1ap_id(msg->gnb_cu_ue_f1ap_id));
  if (ue_ctxt == nullptr) {
    logger.warning("cu_ue={} du_ue={}: Dropping \"ECIDMeasurementFailureIndication\". UE context does not exist",
                   msg->gnb_cu_ue_f1ap_id,
                   msg->gnb_du_ue_f1ap_id);
    return;
  }

  e_cid_measurement_failure_indication_t ind;
  ind.ue_index       = ue_ctxt->ue_ids.ue_index;
  ind.lmf_ue_meas_id = uint_to_lmf_ue_meas_id(msg->lmf_ue_meas_id);
  ind.ran_ue_meas_id = uint_to_ran_ue_meas_id(msg->ran_ue_meas_id);
  ind.cause          = asn1_to_cause(msg->cause);

  du_processor_notifier.on_e_cid_measurement_failure_indication(ind);
}

void f1ap_cu_impl::handle_e_cid_measurement_termination_command(const e_cid_measurement_termination_command_t& command)
{
  if (!ue_ctxt_list.contains(command.ue_index)) {
    logger.warning("ue={}: Dropping \"ECIDMeasurementTerminationCommand\". UE context does not exist",
                   command.ue_index);
    return;
  }

  f1ap_ue_context& ue_ctxt = ue_ctxt_list[command.ue_index];
  ocudu_sanity_check(ue_ctxt.ue_ids.du_ue_f1ap_id && ue_ctxt.ue_ids.du_ue_f1ap_id != gnb_du_ue_f1ap_id_t::invalid,
                     "Invalid gNB-DU-UE-F1AP-Id");

  f1ap_message f1ap_msg;
  f1ap_msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_TERMINATION);
  auto& cmd              = f1ap_msg.pdu.init_msg().value.e_c_id_meas_termination_cmd();
  cmd->gnb_cu_ue_f1ap_id = to_underlying(ue_ctxt.ue_ids.cu_ue_f1ap_id);
  cmd->gnb_du_ue_f1ap_id = to_underlying(*ue_ctxt.ue_ids.du_ue_f1ap_id);
  cmd->lmf_ue_meas_id    = to_underlying(command.lmf_ue_meas_id);
  cmd->ran_ue_meas_id    = to_underlying(command.ran_ue_meas_id);

  tx_pdu_notifier.on_new_message(f1ap_msg);
}

void f1ap_cu_impl::handle_ref_time_info_report_ctrl(const f1ap_ref_time_report_ctrl_request& request)
{
  using event_type_opts = asn1::f1ap::event_type_opts;

  f1ap_message msg{};
  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_REF_TIME_INFO_REPORT_CTRL);
  auto& ctrl           = msg.pdu.init_msg().value.ref_time_info_report_ctrl();
  ctrl->transaction_id = next_ref_time_transaction_id++;

  auto& rrt = ctrl->report_request_type;
  switch (request.event_type) {
    case f1ap_ref_time_event_type::on_demand:
      rrt.event_type.value = event_type_opts::on_demand;
      break;
    case f1ap_ref_time_event_type::periodic:
      rrt.event_type.value                 = event_type_opts::periodic;
      rrt.report_periodicity_value_present = request.report_periodicity_rf.has_value();
      if (request.report_periodicity_rf.has_value()) {
        rrt.report_periodicity_value = *request.report_periodicity_rf;
      }
      break;
    case f1ap_ref_time_event_type::stop:
      rrt.event_type.value = event_type_opts::stop;
      break;
  }

  tx_pdu_notifier.on_new_message(msg);
}

void f1ap_cu_impl::log_pdu(bool is_rx, const f1ap_message& pdu)
{
  using namespace asn1::f1ap;

  if (not logger.info.enabled()) {
    return;
  }

  // In case of F1 Setup, the gNB-DU-Id might not be set yet.
  gnb_du_id_t du_id = du_ctxt.gnb_du_id;
  if (du_id == gnb_du_id_t::invalid) {
    if (pdu.pdu.type().value == f1ap_pdu_c::types_opts::init_msg and
        pdu.pdu.init_msg().value.type().value == f1ap_elem_procs_o::init_msg_c::types_opts::f1_setup_request) {
      du_id = int_to_gnb_du_id(pdu.pdu.init_msg().value.f1_setup_request()->gnb_du_id);
    }
  }

  // Fetch UE index.
  auto                            cu_ue_id = get_gnb_cu_ue_f1ap_id(pdu.pdu);
  std::optional<cu_cp_ue_index_t> ue_idx;
  if (cu_ue_id.has_value()) {
    const auto* ue_ptr = ue_ctxt_list.find(cu_ue_id.value());
    if (ue_ptr != nullptr and ue_ptr->ue_ids.ue_index != cu_cp_ue_index_t::invalid) {
      ue_idx = ue_ptr->ue_ids.ue_index;
    }
  }

  // Log PDU.
  log_f1ap_pdu(logger, is_rx, du_id, ue_idx, pdu, cfg.json_log_enabled);
}

void f1ap_cu_impl::tx_pdu_notifier_with_logging::on_new_message(const f1ap_message& msg)
{
  // Log message.
  parent.log_pdu(false, msg);

  // Forward message to DU.
  decorated.on_new_message(msg);
}
