// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_du_e_cid_measurement_report_procedure.h"
#include "../../asn1_helpers.h"
#include "../ue_context/f1ap_du_ue.h"
#include "../ue_context/f1ap_du_ue_manager.h"
#include "proc_logger.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/f1ap_message.h"

using namespace ocudu;
using namespace odu;

f1ap_du_e_cid_measurement_report_procedure::f1ap_du_e_cid_measurement_report_procedure(
    f1ap_du_positioning_handler& du_mng_,
    f1ap_du_ue_manager&          ues_,
    gnb_du_ue_f1ap_id_t          du_ue_id_,
    uint16_t                     lmf_ue_meas_id_,
    uint16_t                     ran_ue_meas_id_) :
  du_mng(du_mng_),
  ues(ues_),
  du_ue_id(du_ue_id_),
  lmf_ue_meas_id(lmf_ue_meas_id_),
  ran_ue_meas_id(ran_ue_meas_id_),
  logger(ocudulog::fetch_basic_logger("DU-F1"))
{
}

void f1ap_du_e_cid_measurement_report_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  // The procedure runs outside the task loop of the UE, so it looks the UE up again on every step instead of holding
  // a reference to it.
  if (find_ue() == nullptr) {
    CORO_EARLY_RETURN();
  }

  CORO_AWAIT_VALUE(du_result, request_e_cid_measurement(*find_ue()));

  if (find_ue() == nullptr) {
    CORO_EARLY_RETURN();
  }

  find_ue()->e_cid_meas->report_pending = false;

  if (not du_result.success) {
    send_failure_indication(*find_ue());
    // The measurement is terminated, as per TS 38.473 section 8.13.13.2.
    find_ue()->e_cid_meas.reset();
    CORO_EARLY_RETURN();
  }

  send_report(*find_ue(), du_result);

  CORO_RETURN();
}

// The UE can be removed, and the measurement can be replaced or terminated, while the procedure waits. The
// identifiers tell the measurement of this procedure from a later one.
f1ap_du_ue* f1ap_du_e_cid_measurement_report_procedure::find_ue() const
{
  f1ap_du_ue* ue = ues.find(du_ue_id);
  if (ue == nullptr or not ue->e_cid_meas.has_value()) {
    return nullptr;
  }
  if (not ue->e_cid_meas->matches(lmf_ue_meas_id, ran_ue_meas_id)) {
    return nullptr;
  }
  return ue;
}

async_task<du_e_cid_meas_response>
f1ap_du_e_cid_measurement_report_procedure::request_e_cid_measurement(const f1ap_du_ue& ue) const
{
  du_e_cid_meas_request du_req;
  du_req.ue_index   = ue.context.ue_index;
  du_req.quantities = ue.e_cid_meas->quantities;

  return du_mng.request_e_cid_measurement(du_req);
}

void f1ap_du_e_cid_measurement_report_procedure::send_report(const f1ap_du_ue&             ue,
                                                             const du_e_cid_meas_response& du_result_) const
{
  using namespace asn1::f1ap;
  f1ap_message f1ap_msg;

  f1ap_msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_REPORT);
  e_c_id_meas_report_s& report = f1ap_msg.pdu.init_msg().value.e_c_id_meas_report();

  report->gnb_cu_ue_f1ap_id = to_underlying(ue.context.gnb_cu_ue_f1ap_id);
  report->gnb_du_ue_f1ap_id = to_underlying(ue.context.gnb_du_ue_f1ap_id);
  report->lmf_ue_meas_id    = lmf_ue_meas_id;
  report->ran_ue_meas_id    = ran_ue_meas_id;

  if (du_result_.geo_coords.has_value()) {
    report->e_c_id_meas_result.geographical_coordinates_present = true;
    report->e_c_id_meas_result.geographical_coordinates =
        geographical_coordinates_to_asn1(du_result_.geo_coords.value());
  }

  report->e_c_id_meas_result.measured_results_list.resize(du_result_.ul_aoa_results.size());
  for (unsigned i = 0, e = du_result_.ul_aoa_results.size(); i != e; ++i) {
    e_c_id_measured_results_item_s& item = report->e_c_id_meas_result.measured_results_list[i];

    item.e_c_id_measured_results_value.set_value_angleof_arrival_nr() = ul_aoa_to_asn1(du_result_.ul_aoa_results[i]);
  }

  ue.f1ap_msg_notifier.on_new_message(f1ap_msg);
}

void f1ap_du_e_cid_measurement_report_procedure::send_failure_indication(const f1ap_du_ue& ue) const
{
  using namespace asn1::f1ap;
  f1ap_message f1ap_msg;

  f1ap_msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_FAIL_IND);
  e_c_id_meas_fail_ind_s& ind = f1ap_msg.pdu.init_msg().value.e_c_id_meas_fail_ind();

  ind->gnb_cu_ue_f1ap_id      = to_underlying(ue.context.gnb_cu_ue_f1ap_id);
  ind->gnb_du_ue_f1ap_id      = to_underlying(ue.context.gnb_du_ue_f1ap_id);
  ind->lmf_ue_meas_id         = lmf_ue_meas_id;
  ind->ran_ue_meas_id         = ran_ue_meas_id;
  ind->cause.set_misc().value = cause_misc_opts::unspecified;

  ue.f1ap_msg_notifier.on_new_message(f1ap_msg);

  logger.info("{}: Reporting stopped. Cause: Measurement failed.", f1ap_log_prefix{ue.context, name()});
}

void ocudu::odu::start_e_cid_measurement_reporting(f1ap_du_ue&                             ue,
                                                   f1ap_du_positioning_handler&            du_mng,
                                                   f1ap_du_ue_manager&                     ues,
                                                   f1ap_du_positioning_procedure_launcher& launcher)
{
  f1ap_du_e_cid_measurement& meas   = ue.e_cid_meas.value();
  ocudulog::basic_logger&    logger = ocudulog::fetch_basic_logger("DU-F1");

  logger.info(
      "{}: Reporting every {}ms.", f1ap_log_prefix{ue.context, "E-CID Measurement Reporting"}, meas.period.count());

  // The timer outlives this call, so it holds only the identifiers of the measurement.
  const gnb_du_ue_f1ap_id_t du_ue_id   = ue.context.gnb_du_ue_f1ap_id;
  const du_ue_index_t       ue_index   = ue.context.ue_index;
  const uint16_t            lmf_id     = meas.lmf_ue_meas_id;
  const uint16_t            ran_id     = meas.ran_ue_meas_id;
  f1ap_du_configurator&     du_handler = ue.du_handler;

  meas.report_timer.set(
      meas.period, [&du_mng, &ues, &du_handler, &launcher, &logger, du_ue_id, ue_index, lmf_id, ran_id]() {
        f1ap_du_ue* timer_ue = ues.find(du_ue_id);
        if (timer_ue == nullptr or not timer_ue->e_cid_meas.has_value() or
            not timer_ue->e_cid_meas->matches(lmf_id, ran_id)) {
          return;
        }
        f1ap_du_e_cid_measurement& timer_meas = timer_ue->e_cid_meas.value();

        // The next period starts now, so that the wait for the SRS does not add to the period.
        timer_meas.report_timer.run();

        if (timer_meas.report_pending) {
          // The gNB-DU measures one report at a time, so a period without a result gives no report.
          logger.info("{}: Skipping a period. Cause: The last report still waits for a measurement",
                      f1ap_log_prefix{timer_ue->context, "E-CID Measurement Reporting"});
          return;
        }
        timer_meas.report_pending = true;

        // The first part of the procedure runs on the task loop of the UE. The wait for the SRS continues outside the
        // loop, so that the other procedures of the UE do not wait for it.
        async_task<void> proc =
            launch_async<f1ap_du_e_cid_measurement_report_procedure>(du_mng, ues, du_ue_id, lmf_id, ran_id);
        du_handler.get_ue_handler(ue_index).schedule_async_task(
            launcher.start_positioning_measurement_procedure_task(std::move(proc)));
      });
  meas.report_timer.run();
}
