// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_e_cid_measurement_initiation_procedure.h"
#include "../../asn1_helpers.h"
#include "../../f1ap_asn1_utils.h"
#include "f1ap_positioning_asn1_converters.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/ran/cause/f1ap_cause_converters.h"
#include "ocudu/support/enum_utils.h"

using namespace ocudu;
using namespace ocudu::ocucp;
using namespace asn1::f1ap;

/// \brief Convert the E-CID measurement quantity from common type to ASN.1.
/// \param[in] quantity The common type E-CID measurement quantity.
/// \returns The ASN.1 type E-CID measurement quantity.
static e_c_id_meas_quantities_value_e e_cid_meas_quantity_to_asn1(const e_cid_meas_quantities_item_t& quantity);

/// \brief Convert the measurement periodicity from common type to ASN.1.
/// \param[in] periodicity The common type measurement periodicity.
/// \returns The ASN.1 type measurement periodicity, or nullopt when F1AP has no codepoint for it.
static std::optional<meas_periodicity_e> meas_periodicity_to_asn1(meas_periodicity_t periodicity);

/// \brief Convert the NR-AoA measurement periodicity from common type to ASN.1.
/// \param[in] periodicity The common type NR-AoA measurement periodicity.
/// \returns The ASN.1 type NR-AoA measurement periodicity.
static pos_meas_periodicity_nr_ao_a_e meas_periodicity_nr_aoa_to_asn1(meas_periodicity_nr_aoa_t periodicity);

f1ap_e_cid_measurement_initiation_procedure::f1ap_e_cid_measurement_initiation_procedure(
    const f1ap_configuration&          f1ap_cfg_,
    const e_cid_measurement_request_t& request_,
    f1ap_ue_context&                   ue_ctxt_,
    f1ap_message_notifier&             f1ap_notif_,
    ocudulog::basic_logger&            logger_) :
  f1ap_cfg(f1ap_cfg_), request(request_), ue_ctxt(ue_ctxt_), f1ap_notifier(f1ap_notif_), logger(logger_)
{
}

void f1ap_e_cid_measurement_initiation_procedure::operator()(
    coro_context<async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>>& ctx)
{
  CORO_BEGIN(ctx);

  logger.debug("{}: Procedure started...", f1ap_ue_log_prefix{ue_ctxt.ue_ids, name()});

  // Subscribe to respective publisher to receive E-CID MEASUREMENT INITIATION RESPONSE/FAILURE message.
  transaction_sink.subscribe_to(ue_ctxt.ev_mng.e_cid_measurement_outcome, f1ap_cfg.proc_timeout);

  // Send command to DU.
  if (!send_e_cid_measurement_initiation_request()) {
    CORO_EARLY_RETURN(make_unexpected(e_cid_measurement_failure_t{
        request.lmf_ue_meas_id, request.ran_ue_meas_id, f1ap_cause_t{cause_protocol_t::semantic_error}}));
  }

  // Await DU response.
  CORO_AWAIT(transaction_sink);

  CORO_RETURN(create_e_cid_measurement_result());
}

bool f1ap_e_cid_measurement_initiation_procedure::send_e_cid_measurement_initiation_request()
{
  ocudu_sanity_check(ue_ctxt.ue_ids.du_ue_f1ap_id && ue_ctxt.ue_ids.du_ue_f1ap_id != gnb_du_ue_f1ap_id_t::invalid,
                     "Invalid gNB-DU-UE-F1AP-Id");

  f1ap_message msg;
  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_INITIATION);
  e_c_id_meas_initiation_request_s& req = msg.pdu.init_msg().value.e_c_id_meas_initiation_request();

  req->gnb_du_ue_f1ap_id = to_underlying(*ue_ctxt.ue_ids.du_ue_f1ap_id);
  req->gnb_cu_ue_f1ap_id = to_underlying(ue_ctxt.ue_ids.cu_ue_f1ap_id);
  req->lmf_ue_meas_id    = to_underlying(request.lmf_ue_meas_id);
  req->ran_ue_meas_id    = to_underlying(request.ran_ue_meas_id);

  const bool periodic = request.report_characteristics == report_characteristics_t::periodic;
  req->e_c_id_report_characteristics =
      periodic ? e_c_id_report_characteristics_opts::periodic : e_c_id_report_characteristics_opts::on_demand;

  for (const auto& quantity : request.meas_quantities) {
    asn1::protocol_ie_single_container_s<e_c_id_meas_quantities_item_ies_o> asn1_item;
    asn1_item.load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_QUANTITIES_ITEM);
    asn1_item->e_c_id_meas_quantities_item().e_c_id_meas_quantities_value = e_cid_meas_quantity_to_asn1(quantity);
    req->e_c_id_meas_quantities.push_back(asn1_item);
  }

  // The E-CID Measurement Periodicity IE is mandatory for a periodic report, as per TS 38.473, Section 9.2.12.20. It
  // does not apply to NR Angle of Arrival, which carries its own periodicity.
  if (periodic && request.meas_periodicity.has_value()) {
    std::optional<meas_periodicity_e> asn1_periodicity = meas_periodicity_to_asn1(request.meas_periodicity.value());
    if (!asn1_periodicity.has_value()) {
      logger.warning("{}: Procedure failed. Cause: The gNB-DU does not support the requested measurement periodicity",
                     f1ap_ue_log_prefix{ue_ctxt.ue_ids, name()});
      return false;
    }
    req->e_c_id_meas_periodicity_present = true;
    req->e_c_id_meas_periodicity         = asn1_periodicity.value();
  }
  if (periodic && request.meas_periodicity_nr_aoa.has_value()) {
    req->pos_meas_periodicity_nr_ao_a_present = true;
    req->pos_meas_periodicity_nr_ao_a = meas_periodicity_nr_aoa_to_asn1(request.meas_periodicity_nr_aoa.value());
  }

  f1ap_notifier.on_new_message(msg);

  return true;
}

expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>
f1ap_e_cid_measurement_initiation_procedure::create_e_cid_measurement_result()
{
  expected<e_cid_measurement_response_t, e_cid_measurement_failure_t> res;

  auto logger_prefix = f1ap_ue_log_prefix{ue_ctxt.ue_ids, name()};

  if (transaction_sink.successful()) {
    const e_c_id_meas_initiation_resp_s& resp = transaction_sink.response();

    logger.info("{}: Procedure finished successfully", logger_prefix);

    e_cid_measurement_response_t result;
    result.lmf_ue_meas_id = uint_to_lmf_ue_meas_id(resp->lmf_ue_meas_id);
    result.ran_ue_meas_id = uint_to_ran_ue_meas_id(resp->ran_ue_meas_id);
    if (resp->e_c_id_meas_result_present) {
      result.e_cid_meas_result = fill_e_cid_measurement_result(resp->e_c_id_meas_result);
    }
    if (resp->cell_portion_id_present) {
      result.cell_portion_id = resp->cell_portion_id;
    }

    res = result;

  } else if (transaction_sink.failed()) {
    const e_c_id_meas_initiation_fail_s& fail = transaction_sink.failure();

    // A declined measurement is a normal outcome. The caller decides how to report it.
    logger.info("{}: Procedure failed. Cause: {}", logger_prefix, get_cause_str(fail->cause));

    res = make_unexpected(e_cid_measurement_failure_t{uint_to_lmf_ue_meas_id(fail->lmf_ue_meas_id),
                                                      uint_to_ran_ue_meas_id(fail->ran_ue_meas_id),
                                                      asn1_to_cause(fail->cause)});

  } else {
    logger.warning("{}: Procedure failed. Cause: Timeout reached for ECIDMeasurementInitiationResponse reception",
                   logger_prefix);

    res = make_unexpected(e_cid_measurement_failure_t{
        request.lmf_ue_meas_id, request.ran_ue_meas_id, f1ap_cause_t{cause_misc_t::unspecified}});
  }

  return res;
}

static e_c_id_meas_quantities_value_e e_cid_meas_quantity_to_asn1(const e_cid_meas_quantities_item_t& quantity)
{
  e_c_id_meas_quantities_value_e asn1_quantity;

  switch (quantity) {
    case e_cid_meas_quantities_item_t::nr_angle_of_arrival:
      asn1_quantity = e_c_id_meas_quantities_value_opts::angle_of_arrival_nr;
      break;
    default:
      asn1_quantity = e_c_id_meas_quantities_value_opts::default_value;
      break;
  }

  return asn1_quantity;
}

static std::optional<meas_periodicity_e> meas_periodicity_to_asn1(meas_periodicity_t periodicity)
{
  meas_periodicity_e asn1_periodicity;

  switch (periodicity) {
    case meas_periodicity_t::ms120:
      asn1_periodicity = meas_periodicity_opts::ms120;
      break;
    case meas_periodicity_t::ms240:
      asn1_periodicity = meas_periodicity_opts::ms240;
      break;
    case meas_periodicity_t::ms480:
      asn1_periodicity = meas_periodicity_opts::ms480;
      break;
    case meas_periodicity_t::ms640:
      asn1_periodicity = meas_periodicity_opts::ms640;
      break;
    case meas_periodicity_t::ms1024:
      asn1_periodicity = meas_periodicity_opts::ms1024;
      break;
    case meas_periodicity_t::ms2048:
      asn1_periodicity = meas_periodicity_opts::ms2048;
      break;
    case meas_periodicity_t::ms5120:
      asn1_periodicity = meas_periodicity_opts::ms5120;
      break;
    case meas_periodicity_t::ms10240:
      asn1_periodicity = meas_periodicity_opts::ms10240;
      break;
    case meas_periodicity_t::ms20480:
      asn1_periodicity = meas_periodicity_opts::ms20480;
      break;
    case meas_periodicity_t::ms40960:
      asn1_periodicity = meas_periodicity_opts::ms40960;
      break;
    case meas_periodicity_t::min1:
      asn1_periodicity = meas_periodicity_opts::min1;
      break;
    case meas_periodicity_t::min6:
      asn1_periodicity = meas_periodicity_opts::min6;
      break;
    case meas_periodicity_t::min12:
      asn1_periodicity = meas_periodicity_opts::min12;
      break;
    case meas_periodicity_t::min30:
      asn1_periodicity = meas_periodicity_opts::min30;
      break;
    default:
      // The 60min codepoint applies only to an ng-eNB, as per TS 38.455, Section 9.1.1.1, and F1AP has no counterpart.
      return std::nullopt;
  }

  return asn1_periodicity;
}

static pos_meas_periodicity_nr_ao_a_e meas_periodicity_nr_aoa_to_asn1(meas_periodicity_nr_aoa_t periodicity)
{
  pos_meas_periodicity_nr_ao_a_e asn1_periodicity;

  switch (periodicity) {
    case meas_periodicity_nr_aoa_t::ms160:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms160;
      break;
    case meas_periodicity_nr_aoa_t::ms320:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms320;
      break;
    case meas_periodicity_nr_aoa_t::ms640:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms640;
      break;
    case meas_periodicity_nr_aoa_t::ms1280:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms1280;
      break;
    case meas_periodicity_nr_aoa_t::ms2560:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms2560;
      break;
    case meas_periodicity_nr_aoa_t::ms5120:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms5120;
      break;
    case meas_periodicity_nr_aoa_t::ms10240:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms10240;
      break;
    case meas_periodicity_nr_aoa_t::ms20480:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms20480;
      break;
    case meas_periodicity_nr_aoa_t::ms40960:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms40960;
      break;
    case meas_periodicity_nr_aoa_t::ms61440:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms61440;
      break;
    case meas_periodicity_nr_aoa_t::ms81920:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms81920;
      break;
    case meas_periodicity_nr_aoa_t::ms368640:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms368640;
      break;
    case meas_periodicity_nr_aoa_t::ms737280:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms737280;
      break;
    default:
      asn1_periodicity = pos_meas_periodicity_nr_ao_a_opts::ms1843200;
      break;
  }

  return asn1_periodicity;
}
