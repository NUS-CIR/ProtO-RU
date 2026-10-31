// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "measurement_procedure.h"
#include "../nrppa_asn1_converters.h"
#include "../nrppa_helper.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/nrppa/common.h"
#include "ocudu/asn1/nrppa/nrppa_pdu_contents.h"
#include "ocudu/ran/positioning/measurement_information.h"
#include "ocudu/support/async/coroutine.h"
#include "fmt/format.h"
#include <variant>

using namespace ocudu;
using namespace ocucp;

measurement_procedure::measurement_procedure(cu_cp_amf_index_t            amf_index_,
                                             const measurement_request_t& request_,
                                             uint16_t                     transaction_id_,
                                             const nrppa_trp_registry&    trp_registry_,
                                             nrppa_meas_context_list&     meas_ctxt_list_,
                                             nrppa_du_context_list&       du_ctxt_list_,
                                             nrppa_cu_cp_notifier&        cu_cp_notifier_,
                                             ocudulog::basic_logger&      logger_) :
  amf_index(amf_index_),
  meas_request(request_),
  transaction_id(transaction_id_),
  trp_registry(trp_registry_),
  meas_ctxt_list(meas_ctxt_list_),
  du_ctxt_list(du_ctxt_list_),
  cu_cp_notifier(cu_cp_notifier_),
  logger(logger_)
{
}

void measurement_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  logger.info("\"{}\" started...", name());

  // NOTE: create_measurement_context() must run first, as it allocates the RAN Meas ID that
  // prepare_du_measurement_information_requests() stamps onto every per-DU request.
  if (!create_measurement_context() or !prepare_du_measurement_information_requests()) {
    logger.warning("\"{}\" failed", name());
    // Send failure to CU-CP.
    send_ul_nrppa_pdu(logger,
                      cu_cp_notifier,
                      create_measurement_failure(nrppa_cause_protocol_t::msg_not_compatible_with_receiver_state),
                      "MeasResponse",
                      "MeasFailure",
                      amf_index);
    CORO_EARLY_RETURN();
  }

  // TODO: use when_all.
  for (du_request_it = du_meas_requests.begin(); du_request_it != du_meas_requests.end(); du_request_it++) {
    sub_procedure_du_index     = du_request_it->first;
    sub_procedure_meas_request = du_request_it->second;
    // Send measurement request to DU.
    CORO_AWAIT_VALUE(
        du_meas_outcome,
        du_ctxt_list[sub_procedure_du_index].f1ap->on_measurement_information_request(sub_procedure_meas_request));

    handle_du_measurement_information_outcome(sub_procedure_du_index);
  }

  handle_procedure_outcome();

  logger.info("\"{}\" finished successfully", name());

  CORO_RETURN();
}

bool measurement_procedure::create_measurement_context()
{
  // Allocate RAN MEAS ID.
  expected<ran_meas_id_t, std::string> ret = meas_ctxt_list.allocate_ran_meas_id();
  if (!ret.has_value()) {
    logger.warning("lmf_meas_id={}: Couldn't allocate RAN meas ID. Cause: {}",
                   fmt::underlying(meas_request.lmf_meas_id),
                   ret.error());
    return false;
  }

  ran_meas_id = ret.value();

  std::vector<trp_id_t> trp_list;
  trp_list.reserve(meas_request.trp_meas_request_list.size());
  for (const auto& trp_meas_req_item : meas_request.trp_meas_request_list) {
    trp_list.push_back(trp_meas_req_item.trp_id);
  }

  // Create measurement context and store it.
  meas_ctxt_list.add_measurement(amf_index, ran_meas_id, meas_request.lmf_meas_id, trp_list);

  return true;
}

bool measurement_procedure::prepare_du_measurement_information_requests()
{
  // A TRP is addressed by its ID, which only the TRP Information Exchange procedure maps to a DU.
  if (!trp_registry.has_trp_information()) {
    logger.warning("lmf_meas_id={}: No TRP information available", fmt::underlying(meas_request.lmf_meas_id));
    return false;
  }

  for (const auto& trp_meas_req_item : meas_request.trp_meas_request_list) {
    trp_id_t trp_id = trp_meas_req_item.trp_id;
    if (std::optional<cu_cp_du_index_t> du_idx = trp_registry.find_du(trp_id); du_idx.has_value()) {
      cu_cp_du_index_t du_index = du_idx.value();
      // If the DU index is not found in the du_meas_requests map, create it.
      if (du_meas_requests.find(du_index) == du_meas_requests.end()) {
        measurement_request_t du_meas_request;
        du_meas_request.lmf_meas_id                      = meas_request.lmf_meas_id;
        du_meas_request.ran_meas_id                      = ran_meas_id;
        du_meas_request.report_characteristics           = meas_request.report_characteristics;
        du_meas_request.meas_periodicity                 = meas_request.meas_periodicity;
        du_meas_request.trp_meas_quantities              = meas_request.trp_meas_quantities;
        du_meas_request.sfn_initialization_time          = meas_request.sfn_initialization_time;
        du_meas_request.srs_config                       = meas_request.srs_config;
        du_meas_request.meas_beam_info_request           = meas_request.meas_beam_info_request;
        du_meas_request.sys_frame_num                    = meas_request.sys_frame_num;
        du_meas_request.slot_num                         = meas_request.slot_num;
        du_meas_request.meas_periodicity_extended        = meas_request.meas_periodicity_extended;
        du_meas_request.resp_time                        = meas_request.resp_time;
        du_meas_request.meas_characteristics_request_ind = meas_request.meas_characteristics_request_ind;
        du_meas_request.meas_time_occasion               = meas_request.meas_time_occasion;
        du_meas_request.meas_amount                      = meas_request.meas_amount;

        du_meas_requests.emplace(du_index, du_meas_request);
      }

      du_meas_requests.at(du_index).trp_meas_request_list.push_back(trp_meas_req_item);
    }
  }

  if (du_meas_requests.empty()) {
    logger.warning("lmf_meas_id={}: No DU for TRP IDs found", fmt::underlying(meas_request.lmf_meas_id));
    return false;
  }

  return true;
}

void measurement_procedure::handle_du_measurement_information_outcome(cu_cp_du_index_t du_index)
{
  if (du_meas_outcome.has_value()) {
    if (procedure_outcome.has_value()) {
      // Append result to the response message.
      for (const auto& trp_meas_resp_item : du_meas_outcome.value().trp_meas_resp_list) {
        procedure_outcome->trp_meas_resp_list.push_back(trp_meas_resp_item);
      }
    } else {
      procedure_outcome = du_meas_outcome.value();
    }
  } else {
    logger.warning("Measurement Information Transfer Procedure failed for DU index={}", du_index);
  }
}

void measurement_procedure::handle_procedure_outcome()
{
  if (!procedure_outcome.has_value()) {
    meas_outcome = create_measurement_failure(nrppa_cause_radio_network_t::requested_item_not_supported);
    logger.warning("\"{}\" failed", name());
  } else {
    meas_outcome = create_measurement_response();
    logger.info("\"{}\" finished successfully", name());
  }

  // The measurement context only exists to correlate the DU response(s) with this one-shot query; release it now
  // that the procedure has completed, freeing up its RAN Meas ID for reuse.
  meas_ctxt_list.remove_measurement_context(meas_request.lmf_meas_id);

  // Send response to CU-CP.
  send_ul_nrppa_pdu(logger, cu_cp_notifier, meas_outcome, "MeasResponse", "MeasFailure", amf_index);
}

asn1::nrppa::nr_ppa_pdu_c measurement_procedure::create_measurement_failure(nrppa_cause_t cause) const
{
  asn1::nrppa::nr_ppa_pdu_c asn1_fail;

  asn1_fail.set_unsuccessful_outcome().load_info_obj(ASN1_NRPPA_ID_MEAS);
  asn1_fail.unsuccessful_outcome().nrppatransaction_id = transaction_id;
  asn1::nrppa::meas_fail_s& meas_init_fail             = asn1_fail.unsuccessful_outcome().value.meas_fail();

  meas_init_fail->lmf_meas_id = to_underlying(meas_request.lmf_meas_id);
  meas_init_fail->cause       = cause_to_asn1(cause);

  return asn1_fail;
}

static inline asn1::nrppa::relative_path_delay_c
relative_path_delay_to_asn1(const relative_path_delay_t& relative_path_delay)
{
  asn1::nrppa::relative_path_delay_c asn1_relative_path_delay;

  if (relative_path_delay.type == relative_path_delay_t::type_t::k0) {
    asn1_relative_path_delay.set_k0() = relative_path_delay.value;
  } else if (relative_path_delay.type == relative_path_delay_t::type_t::k1) {
    asn1_relative_path_delay.set_k1() = relative_path_delay.value;
  } else if (relative_path_delay.type == relative_path_delay_t::type_t::k2) {
    asn1_relative_path_delay.set_k2() = relative_path_delay.value;
  } else if (relative_path_delay.type == relative_path_delay_t::type_t::k3) {
    asn1_relative_path_delay.set_k3() = relative_path_delay.value;
  } else if (relative_path_delay.type == relative_path_delay_t::type_t::k4) {
    asn1_relative_path_delay.set_k4() = relative_path_delay.value;
  } else {
    asn1_relative_path_delay.set_k5() = relative_path_delay.value;
  }

  return asn1_relative_path_delay;
}

static inline asn1::nrppa::trp_meas_quality_c trp_meas_qualitiy_to_asn1(const trp_meas_quality_t& trp_meas_quality)
{
  asn1::nrppa::trp_meas_quality_c asn1_trp_meas_quality;

  if (std::holds_alternative<trp_meas_timing_quality_t>(trp_meas_quality)) {
    const trp_meas_timing_quality_t& trp_meas_timing_quality = std::get<trp_meas_timing_quality_t>(trp_meas_quality);
    asn1::nrppa::trp_meas_timing_quality_s& asn1_trp_meas_timing_quality =
        asn1_trp_meas_quality.set_timing_meas_quality();

    asn1_trp_meas_timing_quality.meas_quality = trp_meas_timing_quality.meas_quality;
    asn1_trp_meas_timing_quality.resolution   = asn1::nrppa::trp_meas_timing_quality_s::resolution_opts::options(
        (uint8_t)trp_meas_timing_quality.resolution.value);
  } else {
    const trp_meas_angle_quality_t& trp_meas_angle_quality = std::get<trp_meas_angle_quality_t>(trp_meas_quality);
    asn1::nrppa::trp_meas_angle_quality_s& asn1_trp_meas_angle_quality = asn1_trp_meas_quality.set_angle_meas_quality();

    asn1_trp_meas_angle_quality.azimuth_quality = trp_meas_angle_quality.azimuth_quality;
    if (trp_meas_angle_quality.zenith_quality.has_value()) {
      asn1_trp_meas_angle_quality.zenith_quality_present = true;
      asn1_trp_meas_angle_quality.zenith_quality         = trp_meas_angle_quality.zenith_quality.value();
    }
    asn1_trp_meas_angle_quality.resolution = asn1::nrppa::trp_meas_angle_quality_s::resolution_opts::options(
        (uint8_t)trp_meas_angle_quality.resolution.value);
  }

  return asn1_trp_meas_quality;
}

static asn1::nrppa::lcs_to_gcs_translation_s
lcs_to_gcs_translation_to_asn1(const lcs_to_gcs_translation_t& lcs_to_gcs_translation)
{
  asn1::nrppa::lcs_to_gcs_translation_s asn1_lcs_to_gcs_translation;
  asn1_lcs_to_gcs_translation.alpha = lcs_to_gcs_translation.alpha;
  asn1_lcs_to_gcs_translation.beta  = lcs_to_gcs_translation.beta;
  asn1_lcs_to_gcs_translation.gamma = lcs_to_gcs_translation.gamma;
  return asn1_lcs_to_gcs_translation;
}

static inline asn1::nrppa::multiple_ul_ao_a_item_c
multiple_ul_aoa_item_to_asn1(const multiple_ul_aoa_item_t& multiple_ul_aoa_item)
{
  asn1::nrppa::multiple_ul_ao_a_item_c asn1_multiple_ul_aoa_item;
  if (std::holds_alternative<ul_angle_of_arrival_t>(multiple_ul_aoa_item)) {
    const ul_angle_of_arrival_t& aoa      = std::get<ul_angle_of_arrival_t>(multiple_ul_aoa_item);
    asn1::nrppa::ul_ao_a_s&      asn1_aoa = asn1_multiple_ul_aoa_item.set_ul_ao_a();
    asn1_aoa.azimuth_ao_a                 = aoa.azimuth_aoa;
    if (aoa.zenith_aoa.has_value()) {
      asn1_aoa.zenith_ao_a_present = true;
      asn1_aoa.zenith_ao_a         = aoa.zenith_aoa.value();
    }
    if (aoa.lcs_to_gcs_translation.has_value()) {
      asn1_aoa.lcs_to_gcs_translation_present = true;
      asn1_aoa.lcs_to_gcs_translation.alpha   = aoa.lcs_to_gcs_translation->alpha;
      asn1_aoa.lcs_to_gcs_translation.beta    = aoa.lcs_to_gcs_translation->beta;
      asn1_aoa.lcs_to_gcs_translation.gamma   = aoa.lcs_to_gcs_translation->gamma;
    }
  } else {
    const zoa_t&         zoa      = std::get<zoa_t>(multiple_ul_aoa_item);
    asn1::nrppa::zo_a_s& asn1_zoa = asn1_multiple_ul_aoa_item.set_ul_zo_a();
    asn1_zoa.zenith_ao_a          = zoa.zenith_aoa;
    if (zoa.lcs_to_gcs_translation.has_value()) {
      asn1_zoa.lcs_to_gcs_translation_present = true;
      asn1_zoa.lcs_to_gcs_translation.alpha   = zoa.lcs_to_gcs_translation->alpha;
      asn1_zoa.lcs_to_gcs_translation.beta    = zoa.lcs_to_gcs_translation->beta;
      asn1_zoa.lcs_to_gcs_translation.gamma   = zoa.lcs_to_gcs_translation->gamma;
    }
  }

  return asn1_multiple_ul_aoa_item;
}

static inline asn1::nrppa::add_path_list_item_s
add_path_list_item_to_asn1(const add_path_list_item_t& add_path_list_item)
{
  asn1::nrppa::add_path_list_item_s asn1_add_path_list_item;
  // Fill relative time of path.
  asn1_add_path_list_item.relative_time_of_path = relative_path_delay_to_asn1(add_path_list_item.relative_time_of_path);

  // Fill path quality.
  if (add_path_list_item.path_quality.has_value()) {
    asn1_add_path_list_item.path_quality_present = true;
    asn1_add_path_list_item.path_quality         = trp_meas_qualitiy_to_asn1(add_path_list_item.path_quality.value());
  }

  // Fill IE exts.
  if (!add_path_list_item.multiple_ul_aoa.empty()) {
    asn1_add_path_list_item.ie_exts_present                  = true;
    asn1_add_path_list_item.ie_exts.multiple_ul_ao_a_present = true;
    for (const auto multiple_ul_aoa_item : add_path_list_item.multiple_ul_aoa) {
      asn1_add_path_list_item.ie_exts.multiple_ul_ao_a.multiple_ul_ao_a.push_back(
          multiple_ul_aoa_item_to_asn1(multiple_ul_aoa_item));
    }
  }

  if (add_path_list_item.path_pwr.has_value()) {
    asn1_add_path_list_item.ie_exts_present                    = true;
    asn1_add_path_list_item.ie_exts.path_pwr_present           = true;
    asn1_add_path_list_item.ie_exts.path_pwr.first_path_rsrp_p = add_path_list_item.path_pwr->first_path_rsrp_pwr;
  }

  return asn1_add_path_list_item;
}

static inline asn1::nrppa::extended_add_path_list_item_s
extended_add_path_list_item_to_asn1(const add_path_list_item_t& extended_add_path_list_item)
{
  asn1::nrppa::extended_add_path_list_item_s asn1_extended_add_path_list_item;
  // Fill relative time of path.
  asn1_extended_add_path_list_item.relative_time_of_path =
      relative_path_delay_to_asn1(extended_add_path_list_item.relative_time_of_path);

  // Fill path quality.
  if (extended_add_path_list_item.path_quality.has_value()) {
    asn1_extended_add_path_list_item.path_quality_present = true;
    asn1_extended_add_path_list_item.path_quality =
        trp_meas_qualitiy_to_asn1(extended_add_path_list_item.path_quality.value());
  }

  // Fill Multiple UL AOA.
  if (!extended_add_path_list_item.multiple_ul_aoa.empty()) {
    asn1_extended_add_path_list_item.multiple_ul_ao_a_present = true;
    for (const auto multiple_ul_aoa_item : extended_add_path_list_item.multiple_ul_aoa) {
      asn1_extended_add_path_list_item.multiple_ul_ao_a.multiple_ul_ao_a.push_back(
          multiple_ul_aoa_item_to_asn1(multiple_ul_aoa_item));
    }
  }

  if (extended_add_path_list_item.path_pwr.has_value()) {
    asn1_extended_add_path_list_item.path_pwr_present = true;
    asn1_extended_add_path_list_item.path_pwr.first_path_rsrp_p =
        extended_add_path_list_item.path_pwr->first_path_rsrp_pwr;
  }

  return asn1_extended_add_path_list_item;
}

asn1::nrppa::nr_ppa_pdu_c measurement_procedure::create_measurement_response()
{
  asn1::nrppa::nr_ppa_pdu_c asn1_resp;

  asn1_resp.set_successful_outcome().load_info_obj(ASN1_NRPPA_ID_MEAS);
  asn1_resp.successful_outcome().nrppatransaction_id = transaction_id;

  asn1::nrppa::meas_resp_s& asn1_meas_resp = asn1_resp.successful_outcome().value.meas_resp();

  asn1_meas_resp->lmf_meas_id = to_underlying(procedure_outcome->lmf_meas_id);
  asn1_meas_resp->ran_meas_id = to_underlying(ran_meas_id);

  if (!procedure_outcome->trp_meas_resp_list.empty()) {
    asn1_meas_resp->trp_meas_resp_list_present = true;

    for (const auto& trp_measurement_response_item : procedure_outcome->trp_meas_resp_list) {
      asn1::nrppa::trp_meas_resp_item_s asn1_trp_meas_resp_item;
      asn1_trp_meas_resp_item.trp_id = to_underlying(trp_measurement_response_item.trp_id);
      for (const auto& trp_meas_result_item : trp_measurement_response_item.meas_result) {
        asn1::nrppa::trp_meas_result_item_s asn1_trp_meas_result_item;

        // Fill measured results value.
        if (std::holds_alternative<ul_angle_of_arrival_t>(trp_meas_result_item.measured_results_value)) {
          ul_angle_of_arrival_t   ul_aoa = std::get<ul_angle_of_arrival_t>(trp_meas_result_item.measured_results_value);
          asn1::nrppa::ul_ao_a_s& asn1_ul_aoa =
              asn1_trp_meas_result_item.measured_results_value.set_ul_angle_of_arrival();
          asn1_ul_aoa.azimuth_ao_a = ul_aoa.azimuth_aoa;
          if (ul_aoa.zenith_aoa.has_value()) {
            asn1_ul_aoa.zenith_ao_a_present = true;
            asn1_ul_aoa.zenith_ao_a         = ul_aoa.zenith_aoa.value();
          }
          if (ul_aoa.lcs_to_gcs_translation.has_value()) {
            asn1_ul_aoa.lcs_to_gcs_translation_present = true;
            asn1_ul_aoa.lcs_to_gcs_translation = lcs_to_gcs_translation_to_asn1(ul_aoa.lcs_to_gcs_translation.value());
          }
        } else if (const auto* zoa = std::get_if<zoa_t>(&trp_meas_result_item.measured_results_value)) {
          auto& asn1_ext = asn1_trp_meas_result_item.measured_results_value.set_choice_ext();
          asn1_ext->set(asn1::nrppa::trp_measured_results_value_ext_ies_o::value_c::types_opts::zo_a);
          asn1::nrppa::zo_a_s& asn1_zoa = asn1_ext->zo_a();
          asn1_zoa.zenith_ao_a          = zoa->zenith_aoa;
          if (zoa->lcs_to_gcs_translation.has_value()) {
            asn1_zoa.lcs_to_gcs_translation_present = true;
            asn1_zoa.lcs_to_gcs_translation = lcs_to_gcs_translation_to_asn1(zoa->lcs_to_gcs_translation.value());
          }
        } else if (std::holds_alternative<uint8_t>(trp_meas_result_item.measured_results_value)) {
          asn1_trp_meas_result_item.measured_results_value.set_ul_srs_rsrp() =
              std::get<uint8_t>(trp_meas_result_item.measured_results_value);
        } else if (std::holds_alternative<ul_rtoa_t>(trp_meas_result_item.measured_results_value)) {
          ul_rtoa_t ul_rtoa = std::get<ul_rtoa_t>(trp_meas_result_item.measured_results_value);
          asn1::nrppa::ul_rtoa_measurement_s& asn1_ul_rtoa =
              asn1_trp_meas_result_item.measured_results_value.set_ul_rtoa();

          // Fill UL RTOA meas.
          if (ul_rtoa.ul_rtoa_meas.type == ul_rtoa_meas_t::type_t::k0) {
            asn1_ul_rtoa.ul_rto_ameas.set_k0() = ul_rtoa.ul_rtoa_meas.value;
          } else if (ul_rtoa.ul_rtoa_meas.type == ul_rtoa_meas_t::type_t::k1) {
            asn1_ul_rtoa.ul_rto_ameas.set_k1() = ul_rtoa.ul_rtoa_meas.value;
          } else if (ul_rtoa.ul_rtoa_meas.type == ul_rtoa_meas_t::type_t::k2) {
            asn1_ul_rtoa.ul_rto_ameas.set_k2() = ul_rtoa.ul_rtoa_meas.value;
          } else if (ul_rtoa.ul_rtoa_meas.type == ul_rtoa_meas_t::type_t::k3) {
            asn1_ul_rtoa.ul_rto_ameas.set_k3() = ul_rtoa.ul_rtoa_meas.value;
          } else if (ul_rtoa.ul_rtoa_meas.type == ul_rtoa_meas_t::type_t::k4) {
            asn1_ul_rtoa.ul_rto_ameas.set_k4() = ul_rtoa.ul_rtoa_meas.value;
          } else {
            asn1_ul_rtoa.ul_rto_ameas.set_k5() = (uint16_t)ul_rtoa.ul_rtoa_meas.value;
          }
          // Fill add path list.
          for (const auto& add_path_list_item : ul_rtoa.add_path_list) {
            asn1_ul_rtoa.add_path_list.push_back(add_path_list_item_to_asn1(add_path_list_item));
          }
          // Fill extended add path list.
          if (!ul_rtoa.extended_add_path_list.empty()) {
            asn1_ul_rtoa.ie_exts_present                        = true;
            asn1_ul_rtoa.ie_exts.extended_add_path_list_present = true;
            for (const auto& extended_add_path_list_item : ul_rtoa.extended_add_path_list) {
              asn1_ul_rtoa.ie_exts.extended_add_path_list.push_back(
                  extended_add_path_list_item_to_asn1(extended_add_path_list_item));
            }
          }
          // Fill TRP RX reg info.
          if (ul_rtoa.trp_rx_teg_info.has_value()) {
            asn1_ul_rtoa.ie_exts_present                       = true;
            asn1_ul_rtoa.ie_exts.trp_rx_teg_info_present       = true;
            asn1_ul_rtoa.ie_exts.trp_rx_teg_info.trp_rx_teg_id = ul_rtoa.trp_rx_teg_info->trp_rx_teg_id;
            asn1::number_to_enum(asn1_ul_rtoa.ie_exts.trp_rx_teg_info.trp_rx_timing_error_margin,
                                 ul_rtoa.trp_rx_teg_info->trp_rx_timing_error_margin);
          }
        } else {
          gnb_rx_tx_time_diff_t rx_tx_time_diff =
              std::get<gnb_rx_tx_time_diff_t>(trp_meas_result_item.measured_results_value);
          asn1::nrppa::gnb_rx_tx_time_diff_s& asn1_rx_tx_time_diff =
              asn1_trp_meas_result_item.measured_results_value.set_gnb_rx_tx_time_diff();

          // Fill RX TX time diff.
          if (rx_tx_time_diff.rx_tx_time_diff.type == gnb_rx_tx_time_diff_meas_t::type_t::k0) {
            asn1_rx_tx_time_diff.rx_tx_time_diff.set_k0() = rx_tx_time_diff.rx_tx_time_diff.value;
          } else if (rx_tx_time_diff.rx_tx_time_diff.type == gnb_rx_tx_time_diff_meas_t::type_t::k1) {
            asn1_rx_tx_time_diff.rx_tx_time_diff.set_k1() = rx_tx_time_diff.rx_tx_time_diff.value;
          } else if (rx_tx_time_diff.rx_tx_time_diff.type == gnb_rx_tx_time_diff_meas_t::type_t::k2) {
            asn1_rx_tx_time_diff.rx_tx_time_diff.set_k2() = rx_tx_time_diff.rx_tx_time_diff.value;
          } else if (rx_tx_time_diff.rx_tx_time_diff.type == gnb_rx_tx_time_diff_meas_t::type_t::k3) {
            asn1_rx_tx_time_diff.rx_tx_time_diff.set_k3() = rx_tx_time_diff.rx_tx_time_diff.value;
          } else if (rx_tx_time_diff.rx_tx_time_diff.type == gnb_rx_tx_time_diff_meas_t::type_t::k4) {
            asn1_rx_tx_time_diff.rx_tx_time_diff.set_k4() = rx_tx_time_diff.rx_tx_time_diff.value;
          } else {
            asn1_rx_tx_time_diff.rx_tx_time_diff.set_k5() = (uint16_t)rx_tx_time_diff.rx_tx_time_diff.value;
          }
          // Fill add path list.
          for (const auto& add_path_list_item : rx_tx_time_diff.add_path_list) {
            asn1_rx_tx_time_diff.add_path_list.push_back(add_path_list_item_to_asn1(add_path_list_item));
          }
          // Fill extended add path list.
          if (!rx_tx_time_diff.extended_add_path_list.empty()) {
            asn1_rx_tx_time_diff.ie_exts_present                        = true;
            asn1_rx_tx_time_diff.ie_exts.extended_add_path_list_present = true;
            for (const auto& extended_add_path_list_item : rx_tx_time_diff.extended_add_path_list) {
              asn1_rx_tx_time_diff.ie_exts.extended_add_path_list.push_back(
                  extended_add_path_list_item_to_asn1(extended_add_path_list_item));
            }
          }
          // Fill TRP TEG info.
          if (rx_tx_time_diff.trp_teg_info.has_value()) {
            asn1_rx_tx_time_diff.ie_exts_present             = true;
            asn1_rx_tx_time_diff.ie_exts.trpteg_info_present = true;
            if (std::holds_alternative<rx_tx_teg_t>(rx_tx_time_diff.trp_teg_info.value())) {
              const rx_tx_teg_t&        rx_tx_teg      = std::get<rx_tx_teg_t>(rx_tx_time_diff.trp_teg_info.value());
              asn1::nrppa::rx_tx_teg_s& asn1_rx_tx_teg = asn1_rx_tx_time_diff.ie_exts.trpteg_info.set_rx_tx_teg();
              asn1_rx_tx_teg.trp_rx_tx_teg_info.trp_rx_tx_teg_id = rx_tx_teg.trp_rx_tx_teg_info.trp_rx_tx_teg_id;
              asn1_rx_tx_teg.trp_rx_tx_teg_info.trp_rx_tx_timing_error_margin =
                  asn1::nrppa::rx_tx_timing_error_margin_opts::options(
                      (uint8_t)rx_tx_teg.trp_rx_tx_teg_info.trp_rx_tx_timing_error_margin.value);
              if (rx_tx_teg.trp_tx_teg_info.has_value()) {
                asn1_rx_tx_teg.trp_tx_teg_info_present       = true;
                asn1_rx_tx_teg.trp_tx_teg_info.trp_tx_teg_id = rx_tx_teg.trp_tx_teg_info->trp_tx_teg_id;
                asn1::number_to_enum(asn1_rx_tx_teg.trp_tx_teg_info.trp_tx_timing_error_margin,
                                     rx_tx_teg.trp_tx_teg_info->trp_tx_timing_error_margin);
              }
            } else {
              const rx_teg_t&        rx_teg             = std::get<rx_teg_t>(rx_tx_time_diff.trp_teg_info.value());
              asn1::nrppa::rx_teg_s& asn1_rx_teg        = asn1_rx_tx_time_diff.ie_exts.trpteg_info.set_rx_teg();
              asn1_rx_teg.trp_rx_teg_info.trp_rx_teg_id = rx_teg.trp_rx_teg_info.trp_rx_teg_id;
              asn1::number_to_enum(asn1_rx_teg.trp_rx_teg_info.trp_rx_timing_error_margin,
                                   rx_teg.trp_rx_teg_info.trp_rx_timing_error_margin);
              asn1_rx_teg.trp_tx_teg_info.trp_tx_teg_id = rx_teg.trp_tx_teg_info.trp_tx_teg_id;
              asn1::number_to_enum(asn1_rx_teg.trp_tx_teg_info.trp_tx_timing_error_margin,
                                   rx_teg.trp_tx_teg_info.trp_tx_timing_error_margin);
            }
          }
        }

        // Fill time stamp.
        asn1_trp_meas_result_item.time_stamp.sys_frame_num = trp_meas_result_item.time_stamp.sys_frame_num;
        if (trp_meas_result_item.time_stamp.slot_idx.type == time_stamp_slot_idx_t::idx_type::scs15) {
          asn1_trp_meas_result_item.time_stamp.slot_idx.set_scs_15() = trp_meas_result_item.time_stamp.slot_idx.value;
        } else if (trp_meas_result_item.time_stamp.slot_idx.type == time_stamp_slot_idx_t::idx_type::scs30) {
          asn1_trp_meas_result_item.time_stamp.slot_idx.set_scs_30() = trp_meas_result_item.time_stamp.slot_idx.value;
        } else if (trp_meas_result_item.time_stamp.slot_idx.type == time_stamp_slot_idx_t::idx_type::scs60) {
          asn1_trp_meas_result_item.time_stamp.slot_idx.set_scs_60() = trp_meas_result_item.time_stamp.slot_idx.value;
        } else {
          asn1_trp_meas_result_item.time_stamp.slot_idx.set_scs_120() = trp_meas_result_item.time_stamp.slot_idx.value;
        }
        if (trp_meas_result_item.time_stamp.meas_time.has_value()) {
          asn1_trp_meas_result_item.time_stamp.meas_time_present = true;
          asn1_trp_meas_result_item.time_stamp.meas_time.from_number(trp_meas_result_item.time_stamp.meas_time.value());
        }

        // Fill meas quality.
        if (trp_meas_result_item.meas_quality.has_value()) {
          asn1_trp_meas_result_item.meas_quality_present = true;
          asn1_trp_meas_result_item.meas_quality = trp_meas_qualitiy_to_asn1(trp_meas_result_item.meas_quality.value());
        }

        // Fill meas beam info.
        if (trp_meas_result_item.meas_beam_info.has_value()) {
          asn1_trp_meas_result_item.meas_beam_info_present = true;
          if (trp_meas_result_item.meas_beam_info->prs_res_id.has_value()) {
            asn1_trp_meas_result_item.meas_beam_info.prs_res_id_present = true;
            asn1_trp_meas_result_item.meas_beam_info.prs_res_id =
                trp_meas_result_item.meas_beam_info->prs_res_id.value();
          }
          if (trp_meas_result_item.meas_beam_info->prs_res_set_id.has_value()) {
            asn1_trp_meas_result_item.meas_beam_info.prs_res_set_id_present = true;
            asn1_trp_meas_result_item.meas_beam_info.prs_res_set_id =
                trp_meas_result_item.meas_beam_info->prs_res_set_id.value();
          }
          if (trp_meas_result_item.meas_beam_info->ssb_idx.has_value()) {
            asn1_trp_meas_result_item.meas_beam_info.ssb_idx_present = true;
            asn1_trp_meas_result_item.meas_beam_info.ssb_idx = trp_meas_result_item.meas_beam_info->ssb_idx.value();
          }
        }

        asn1_trp_meas_resp_item.meas_result.push_back(asn1_trp_meas_result_item);
      }

      if (trp_measurement_response_item.cgi_nr.has_value()) {
        asn1_trp_meas_resp_item.ie_exts_present         = true;
        asn1_trp_meas_resp_item.ie_exts.cell_id_present = true;
        asn1_trp_meas_resp_item.ie_exts.cell_id.nr_cell_id.from_number(
            trp_measurement_response_item.cgi_nr->nci.value());
        asn1_trp_meas_resp_item.ie_exts.cell_id.plmn_id = trp_measurement_response_item.cgi_nr->plmn_id.to_bytes();
      }

      asn1_meas_resp->trp_meas_resp_list.push_back(asn1_trp_meas_resp_item);
    }
  }

  return asn1_resp;
}
