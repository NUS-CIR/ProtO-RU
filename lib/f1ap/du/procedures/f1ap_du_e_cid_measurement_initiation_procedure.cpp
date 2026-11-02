// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_du_e_cid_measurement_initiation_procedure.h"
#include "../../asn1_helpers.h"
#include "../ue_context/f1ap_du_ue.h"
#include "../ue_context/f1ap_du_ue_manager.h"
#include "f1ap_du_e_cid_measurement_report_procedure.h"
#include "proc_logger.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/f1ap/f1ap_message.h"
#include <algorithm>

using namespace ocudu;
using namespace odu;

f1ap_du_e_cid_measurement_initiation_procedure::f1ap_du_e_cid_measurement_initiation_procedure(
    const asn1::f1ap::e_c_id_meas_initiation_request_s& msg_,
    f1ap_du_positioning_handler&                        du_mng_,
    f1ap_du_ue_manager&                                 ues_,
    f1ap_du_positioning_procedure_launcher&             launcher_) :
  msg(msg_),
  du_mng(du_mng_),
  ues(ues_),
  launcher(launcher_),
  du_ue_id(int_to_gnb_du_ue_f1ap_id(msg_->gnb_du_ue_f1ap_id)),
  ue_ctxt(ues_.find(int_to_gnb_du_ue_f1ap_id(msg_->gnb_du_ue_f1ap_id))->context),
  logger(ocudulog::fetch_basic_logger("DU-F1"))
{
}

void f1ap_du_e_cid_measurement_initiation_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  logger.info("{}: Procedure started...", f1ap_log_prefix{ue_ctxt, name()});

  if (not read_request()) {
    send_failure();
    CORO_EARLY_RETURN();
  }

  if (is_periodic()) {
    add_periodic_measurement();
  }

  // Measure once, so that the DU only accepts a request that it can serve, as per TS 38.473 section 8.13.12.3.
  CORO_AWAIT_VALUE(du_result, request_e_cid_measurement());

  if (ues.find(du_ue_id) == nullptr) {
    logger.info("{}: Stopping procedure. Cause: UE was removed while the measurement was running",
                f1ap_log_prefix{ue_ctxt, name()});
    CORO_EARLY_RETURN();
  }

  if (is_periodic() and not holds_periodic_measurement()) {
    logger.info("{}: Stopping procedure. Cause: The measurement was terminated while it was running",
                f1ap_log_prefix{ue_ctxt, name()});
    send_failure();
    CORO_EARLY_RETURN();
  }

  if (not du_result.success) {
    if (is_periodic()) {
      ues.find(du_ue_id)->e_cid_meas.reset();
    }
    send_failure();
    CORO_EARLY_RETURN();
  }

  if (is_periodic()) {
    start_periodic_reporting();
  }

  send_response(not is_periodic());

  CORO_RETURN();
}

bool f1ap_du_e_cid_measurement_initiation_procedure::read_request()
{
  using namespace asn1::f1ap;

  if (msg->e_c_id_meas_quantities.size() == 0) {
    logger.warning("{}: E-CID measurement quantities list is empty", f1ap_log_prefix{ue_ctxt, name()});
    return false;
  }

  // The gNB-DU must initiate all requested quantities, or none of them, as per TS 38.473 section 8.13.12.3.
  for (const auto& item : msg->e_c_id_meas_quantities) {
    const auto& quantity = item->e_c_id_meas_quantities_item().e_c_id_meas_quantities_value;
    switch (quantity.value) {
      case e_c_id_meas_quantities_value_opts::default_value:
        quantities.push_back(e_cid_meas_quantity::default_quantity);
        break;
      case e_c_id_meas_quantities_value_opts::angle_of_arrival_nr:
        quantities.push_back(e_cid_meas_quantity::nr_angle_of_arrival);
        break;
      default:
        logger.warning("{}: E-CID measurement quantity \"{}\" not supported",
                       f1ap_log_prefix{ue_ctxt, name()},
                       quantity.to_string());
        return false;
    }
  }

  if (is_periodic() and ues.find(du_ue_id)->e_cid_meas.has_value()) {
    // The gNB-DU measures one periodic E-CID measurement per UE. The running one keeps its reports.
    logger.warning("{}: A periodic E-CID measurement already runs for this UE", f1ap_log_prefix{ue_ctxt, name()});
    return false;
  }

  if (is_periodic() and not get_report_period().has_value()) {
    logger.warning("{}: Periodic request carries no applicable measurement periodicity",
                   f1ap_log_prefix{ue_ctxt, name()});
    return false;
  }

  return true;
}

bool f1ap_du_e_cid_measurement_initiation_procedure::is_aoa_requested() const
{
  return std::find(quantities.begin(), quantities.end(), e_cid_meas_quantity::nr_angle_of_arrival) != quantities.end();
}

std::optional<std::chrono::milliseconds> f1ap_du_e_cid_measurement_initiation_procedure::get_report_period() const
{
  // NR Angle of Arrival has its own periodicity. The E-CID Measurement Periodicity IE does not apply to it, as per
  // TS 38.473 section 9.2.12.20.
  if (is_aoa_requested()) {
    if (not msg->pos_meas_periodicity_nr_ao_a_present) {
      return std::nullopt;
    }
    return std::chrono::milliseconds{msg->pos_meas_periodicity_nr_ao_a.to_number()};
  }

  if (not msg->e_c_id_meas_periodicity_present) {
    return std::nullopt;
  }

  // The codepoints of this IE count minutes above ms10240, as per TS 38.473 section 9.3.1.192.
  using asn1::f1ap::meas_periodicity_opts;
  switch (msg->e_c_id_meas_periodicity.value) {
    case meas_periodicity_opts::min1:
    case meas_periodicity_opts::min6:
    case meas_periodicity_opts::min12:
    case meas_periodicity_opts::min30:
      return std::chrono::minutes{msg->e_c_id_meas_periodicity.to_number()};
    case meas_periodicity_opts::extended:
      // The value comes in the Measurement Periodicity Extended IE, which the E-CID MEASUREMENT INITIATION REQUEST
      // does not carry. The gNB-DU has no period to use.
      return std::nullopt;
    default:
      return std::chrono::milliseconds{msg->e_c_id_meas_periodicity.to_number()};
  }
}

bool f1ap_du_e_cid_measurement_initiation_procedure::is_periodic() const
{
  return msg->e_c_id_report_characteristics.value == asn1::f1ap::e_c_id_report_characteristics_opts::periodic;
}

void f1ap_du_e_cid_measurement_initiation_procedure::add_periodic_measurement()
{
  f1ap_du_ue& ue = *ues.find(du_ue_id);

  ue.e_cid_meas.emplace();
  ue.e_cid_meas->lmf_ue_meas_id = msg->lmf_ue_meas_id;
  ue.e_cid_meas->ran_ue_meas_id = msg->ran_ue_meas_id;
  ue.e_cid_meas->quantities     = quantities;
  ue.e_cid_meas->period         = get_report_period().value();
  ue.e_cid_meas->report_timer   = ue.du_handler.get_ue_handler(ue.context.ue_index).create_timer();
}

bool f1ap_du_e_cid_measurement_initiation_procedure::holds_periodic_measurement() const
{
  const f1ap_du_ue* ue = ues.find(du_ue_id);
  return ue->e_cid_meas.has_value() and ue->e_cid_meas->matches(msg->lmf_ue_meas_id, msg->ran_ue_meas_id);
}

void f1ap_du_e_cid_measurement_initiation_procedure::start_periodic_reporting()
{
  start_e_cid_measurement_reporting(*ues.find(du_ue_id), du_mng, ues, launcher);
}

async_task<du_e_cid_meas_response> f1ap_du_e_cid_measurement_initiation_procedure::request_e_cid_measurement()
{
  du_e_cid_meas_request du_req;
  du_req.ue_index   = ue_ctxt.ue_index;
  du_req.quantities = quantities;

  return du_mng.request_e_cid_measurement(du_req);
}

void f1ap_du_e_cid_measurement_initiation_procedure::send_response(bool with_result) const
{
  using namespace asn1::f1ap;
  f1ap_message f1ap_msg;

  f1ap_msg.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_INITIATION);
  e_c_id_meas_initiation_resp_s& resp = f1ap_msg.pdu.successful_outcome().value.e_c_id_meas_initiation_resp();

  resp->gnb_cu_ue_f1ap_id = msg->gnb_cu_ue_f1ap_id;
  resp->gnb_du_ue_f1ap_id = msg->gnb_du_ue_f1ap_id;
  resp->lmf_ue_meas_id    = msg->lmf_ue_meas_id;
  resp->ran_ue_meas_id    = msg->ran_ue_meas_id;

  if (not with_result) {
    ues.find(du_ue_id)->f1ap_msg_notifier.on_new_message(f1ap_msg);
    logger.info("{}: Procedure finished successfully.", f1ap_log_prefix{ue_ctxt, name()});
    return;
  }

  resp->e_c_id_meas_result_present = true;
  if (du_result.geo_coords.has_value()) {
    resp->e_c_id_meas_result.geographical_coordinates_present = true;
    resp->e_c_id_meas_result.geographical_coordinates = geographical_coordinates_to_asn1(du_result.geo_coords.value());
  }
  resp->e_c_id_meas_result.measured_results_list.resize(du_result.ul_aoa_results.size());
  for (unsigned i = 0, e = du_result.ul_aoa_results.size(); i != e; ++i) {
    e_c_id_measured_results_item_s& item = resp->e_c_id_meas_result.measured_results_list[i];
    item.e_c_id_measured_results_value.set_value_angleof_arrival_nr() = ul_aoa_to_asn1(du_result.ul_aoa_results[i]);
  }

  ues.find(du_ue_id)->f1ap_msg_notifier.on_new_message(f1ap_msg);

  logger.info("{}: Procedure finished successfully.", f1ap_log_prefix{ue_ctxt, name()});
}

void f1ap_du_e_cid_measurement_initiation_procedure::send_failure() const
{
  using namespace asn1::f1ap;
  f1ap_message f1ap_msg;

  f1ap_msg.pdu.set_unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_INITIATION);
  e_c_id_meas_initiation_fail_s& fail = f1ap_msg.pdu.unsuccessful_outcome().value.e_c_id_meas_initiation_fail();

  fail->gnb_cu_ue_f1ap_id      = msg->gnb_cu_ue_f1ap_id;
  fail->gnb_du_ue_f1ap_id      = msg->gnb_du_ue_f1ap_id;
  fail->lmf_ue_meas_id         = msg->lmf_ue_meas_id;
  fail->ran_ue_meas_id         = msg->ran_ue_meas_id;
  fail->cause.set_misc().value = cause_misc_opts::unspecified;

  ues.find(du_ue_id)->f1ap_msg_notifier.on_new_message(f1ap_msg);

  logger.info("{}: Procedure failed.", f1ap_log_prefix{ue_ctxt, name()});
}
