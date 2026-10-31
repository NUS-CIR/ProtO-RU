// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_positioning_measurement_procedure.h"
#include "asn1_helpers.h"
#include "f1ap_asn1_utils.h"
#include "ocudu/adt/expected.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/asn1_utils.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/ran/cause/f1ap_cause.h"
#include "ocudu/ran/cause/f1ap_cause_converters.h"
#include "ocudu/ran/positioning/common.h"
#include "ocudu/ran/positioning/positioning_ids.h"
#include "ocudu/support/async/coroutine.h"
#include <variant>

using namespace ocudu;
using namespace ocucp;

/// \brief Convert the positioning measurement request from common type to ASN.1.
/// \param[out] asn1_request The ASN.1 struct to store the result.
/// \param[in] request The common type positioning measurement request.
static void fill_asn1_positioning_measurement_request(asn1::f1ap::positioning_meas_request_s& asn1_request,
                                                      const measurement_request_t&            request);

/// \brief Creates common type measurement failure.
/// \param[in] cause The cause of the failure.
/// \return The measurement failure PDU.
static measurement_failure_t create_positioning_measurement_failure(const f1ap_cause_t& cause);

/// \brief Creates common type measurement response.
/// \param[in] asn1_resp The ASN.1 measurement response.
/// \return The measurement response PDU.
static measurement_response_t
create_positioning_measurement_response(const asn1::f1ap::positioning_meas_resp_s& asn1_resp,
                                        ocudulog::basic_logger&                    logger);

// ---- Positioning Measurement Procedure ----

f1ap_positioning_measurement_procedure::f1ap_positioning_measurement_procedure(const f1ap_configuration&    f1ap_cfg_,
                                                                               const measurement_request_t& request_,
                                                                               f1ap_event_manager&          ev_mng_,
                                                                               f1ap_message_notifier&       f1ap_notif_,
                                                                               ocudulog::basic_logger&      logger_) :
  f1ap_cfg(f1ap_cfg_), request(request_), ev_mng(ev_mng_), f1ap_notifier(f1ap_notif_), logger(logger_)
{
}

void f1ap_positioning_measurement_procedure::operator()(
    coro_context<async_task<expected<measurement_response_t, measurement_failure_t>>>& ctx)
{
  CORO_BEGIN(ctx);

  logger.info("\"{}\" started...", name());

  transaction = ev_mng.transactions.create_transaction(f1ap_cfg.proc_timeout);

  // The transaction may already be aborted at this point, e.g. if the F1AP is being stopped. Skip sending the
  // request in that case, since nothing will ever observe its response.
  if (transaction.aborted()) {
    CORO_EARLY_RETURN(handle_procedure_outcome());
  }

  // Send command to DU.
  send_positioning_measurement_request();

  // Await DU response.
  CORO_AWAIT(transaction);

  // Handle transaction response.
  CORO_RETURN(handle_procedure_outcome());
}

void f1ap_positioning_measurement_procedure::send_positioning_measurement_request()
{
  // Pack message into PDU.
  f1ap_message f1ap_positioning_meas_request;
  f1ap_positioning_meas_request.pdu.set_init_msg();
  f1ap_positioning_meas_request.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_POSITIONING_MEAS_EXCHANGE);
  asn1::f1ap::positioning_meas_request_s& req =
      f1ap_positioning_meas_request.pdu.init_msg().value.positioning_meas_request();

  req->transaction_id = transaction.id();

  // Convert common type to ASN.1.
  fill_asn1_positioning_measurement_request(req, request);

  // Send positioning measurement request message.
  f1ap_notifier.on_new_message(f1ap_positioning_meas_request);
}

expected<measurement_response_t, measurement_failure_t>
f1ap_positioning_measurement_procedure::handle_procedure_outcome()
{
  if (transaction.aborted()) {
    // Timeout or cancellation case.
    logger.warning("\"{}\" failed. Cause: Timeout or cancellation", name());
    return make_unexpected(create_positioning_measurement_failure(f1ap_cause_radio_network_t::unspecified));
  }

  const f1ap_transaction_response& du_pdu_response = transaction.response();

  if (not du_pdu_response.has_value()) {
    procedure_outcome = make_unexpected(create_positioning_measurement_failure(
        asn1_to_cause(du_pdu_response.error().value.positioning_meas_fail()->cause)));
    logger.warning("\"{}\" failed. Cause: {}",
                   name(),
                   get_cause_str(du_pdu_response.error().value.positioning_meas_fail()->cause));
  } else {
    procedure_outcome =
        create_positioning_measurement_response(du_pdu_response.value().value.positioning_meas_resp(), logger);
    logger.info("\"{}\" finished successfully", name());
  }

  // Send response to CU-CP.
  return procedure_outcome;
}

/// \brief Creates common type measurement failure.
/// \param[in] cause The cause of the failure.
/// \return The measurement failure PDU.
static measurement_failure_t create_positioning_measurement_failure(const f1ap_cause_t& cause)
{
  measurement_failure_t fail;
  fail.cause = f1ap_to_nrppa_cause(cause);

  return fail;
}

static inline relative_path_delay_t
asn1_to_relative_path_delay(const asn1::f1ap::relative_path_delay_c& asn1_relative_path_delay)
{
  relative_path_delay_t relative_path_delay;

  if (asn1_relative_path_delay.type() == asn1::f1ap::relative_path_delay_c::types_opts::options::k0) {
    relative_path_delay.type  = relative_path_delay_t::type_t::k0;
    relative_path_delay.value = asn1_relative_path_delay.k0();
  } else if (asn1_relative_path_delay.type() == asn1::f1ap::relative_path_delay_c::types_opts::options::k1) {
    relative_path_delay.type  = relative_path_delay_t::type_t::k1;
    relative_path_delay.value = asn1_relative_path_delay.k1();
  } else if (asn1_relative_path_delay.type() == asn1::f1ap::relative_path_delay_c::types_opts::options::k2) {
    relative_path_delay.type  = relative_path_delay_t::type_t::k2;
    relative_path_delay.value = asn1_relative_path_delay.k2();
  } else if (asn1_relative_path_delay.type() == asn1::f1ap::relative_path_delay_c::types_opts::options::k3) {
    relative_path_delay.type  = relative_path_delay_t::type_t::k3;
    relative_path_delay.value = asn1_relative_path_delay.k3();
  } else if (asn1_relative_path_delay.type() == asn1::f1ap::relative_path_delay_c::types_opts::options::k4) {
    relative_path_delay.type  = relative_path_delay_t::type_t::k4;
    relative_path_delay.value = asn1_relative_path_delay.k4();
  } else {
    relative_path_delay.type  = relative_path_delay_t::type_t::k5;
    relative_path_delay.value = asn1_relative_path_delay.k5();
  }

  return relative_path_delay;
}

static inline trp_meas_quality_t asn1_to_trp_meas_quality(const asn1::f1ap::trp_meas_quality_s& asn1_trp_meas_quality)
{
  trp_meas_quality_t trp_meas_quality;

  if (asn1_trp_meas_quality.tr_pmeas_quality_item.type() ==
      asn1::f1ap::trp_meas_quality_item_c::types_opts::options::timing_meas_quality) {
    const asn1::f1ap::timing_meas_quality_s& asn1_timing_meas_quality =
        asn1_trp_meas_quality.tr_pmeas_quality_item.timing_meas_quality();
    trp_meas_timing_quality_t timing_meas_quality;

    timing_meas_quality.meas_quality = asn1_timing_meas_quality.meas_quality;
    timing_meas_quality.resolution.value =
        trp_meas_timing_quality_t::resolution_t::value_t((uint8_t)asn1_timing_meas_quality.resolution.value);

    trp_meas_quality = timing_meas_quality;
  } else {
    const asn1::f1ap::angle_meas_quality_s& asn1_angle_meas_quality =
        asn1_trp_meas_quality.tr_pmeas_quality_item.angle_meas_quality();
    trp_meas_angle_quality_t angle_meas_quality;

    angle_meas_quality.azimuth_quality = asn1_angle_meas_quality.azimuth_quality;
    if (asn1_angle_meas_quality.zenith_quality_present) {
      angle_meas_quality.zenith_quality = asn1_angle_meas_quality.zenith_quality;
    }
    angle_meas_quality.resolution.value =
        trp_meas_angle_quality_t::resolution_t::value_t((uint8_t)asn1_angle_meas_quality.resolution.value);

    trp_meas_quality = angle_meas_quality;
  }

  return trp_meas_quality;
}

static zoa_t asn1_to_zoa(const asn1::f1ap::zo_a_info_s& asn1_zoa)
{
  zoa_t zoa;

  zoa.zenith_aoa = asn1_zoa.zenith_ao_a;
  if (asn1_zoa.lcs_to_gcs_translation_present) {
    zoa.lcs_to_gcs_translation = {asn1_zoa.lcs_to_gcs_translation.alpha,
                                  asn1_zoa.lcs_to_gcs_translation.beta,
                                  asn1_zoa.lcs_to_gcs_translation.gamma};
  }

  return zoa;
}

static std::vector<multiple_ul_aoa_item_t>
asn1_to_multiple_ul_aoa(const asn1::f1ap::multiple_ul_ao_a_s& asn1_multiple_aoa)
{
  std::vector<multiple_ul_aoa_item_t> multiple_ul_aoa;

  for (const auto& asn1_multiple_ul_aoa_item : asn1_multiple_aoa.multiple_ul_ao_a) {
    if (asn1_multiple_ul_aoa_item.type() == asn1::f1ap::multiple_ul_ao_a_item_c::types_opts::options::ul_ao_a) {
      const asn1::f1ap::ul_ao_a_s& asn1_aoa = asn1_multiple_ul_aoa_item.ul_ao_a();
      ul_angle_of_arrival_t        aoa;

      aoa.azimuth_aoa = asn1_aoa.azimuth_ao_a;
      if (asn1_aoa.zenith_ao_a_present) {
        aoa.zenith_aoa = asn1_aoa.zenith_ao_a;
      }
      if (asn1_aoa.lcs_to_gcs_translation_present) {
        aoa.lcs_to_gcs_translation = {asn1_aoa.lcs_to_gcs_translation.alpha,
                                      asn1_aoa.lcs_to_gcs_translation.beta,
                                      asn1_aoa.lcs_to_gcs_translation.gamma};
      }

      multiple_ul_aoa.push_back(aoa);
    } else {
      multiple_ul_aoa.push_back(asn1_to_zoa(asn1_multiple_ul_aoa_item.ul_zo_a()));
    }
  }

  return multiple_ul_aoa;
}

static measurement_response_t
create_positioning_measurement_response(const asn1::f1ap::positioning_meas_resp_s& asn1_resp,
                                        ocudulog::basic_logger&                    logger)
{
  measurement_response_t resp;
  resp.lmf_meas_id = uint_to_lmf_meas_id(asn1_resp->lmf_meas_id);
  resp.ran_meas_id = uint_to_ran_meas_id(asn1_resp->ran_meas_id);

  // Fill TRP measurement response list.
  if (asn1_resp->pos_meas_result_list_present) {
    for (const auto& asn1_trp_meas_resp_item : asn1_resp->pos_meas_result_list) {
      trp_measurement_response_item_t trp_meas_resp_item;

      // Fill pos meas result.
      for (const auto& asn1_pos_meas_result_item : asn1_trp_meas_resp_item.pos_meas_result) {
        trp_meas_result_item_t pos_meas_result_item;

        // Fill measured results value.
        if (asn1_pos_meas_result_item.measured_results_value.type() ==
            asn1::f1ap::measured_results_value_c::types_opts::options::ul_angle_of_arrival) {
          const asn1::f1ap::ul_ao_a_s& asn1_aoa =
              asn1_pos_meas_result_item.measured_results_value.ul_angle_of_arrival();
          ul_angle_of_arrival_t aoa;

          aoa.azimuth_aoa = asn1_aoa.azimuth_ao_a;
          if (asn1_aoa.zenith_ao_a_present) {
            aoa.zenith_aoa = asn1_aoa.zenith_ao_a;
          }
          if (asn1_aoa.lcs_to_gcs_translation_present) {
            aoa.lcs_to_gcs_translation = {asn1_aoa.lcs_to_gcs_translation.alpha,
                                          asn1_aoa.lcs_to_gcs_translation.beta,
                                          asn1_aoa.lcs_to_gcs_translation.gamma};
          }

          pos_meas_result_item.measured_results_value = aoa;
        } else if (asn1_pos_meas_result_item.measured_results_value.type() ==
                   asn1::f1ap::measured_results_value_c::types_opts::options::ul_srs_rsrp) {
          pos_meas_result_item.measured_results_value = asn1_pos_meas_result_item.measured_results_value.ul_srs_rsrp();
        } else if (asn1_pos_meas_result_item.measured_results_value.type() ==
                   asn1::f1ap::measured_results_value_c::types_opts::options::ul_rtoa) {
          const asn1::f1ap::ul_rtoa_meas_s& asn1_ul_rtoa = asn1_pos_meas_result_item.measured_results_value.ul_rtoa();
          ul_rtoa_t                         ul_rtoa;

          // Fill UL RTOA meas.
          if (asn1_ul_rtoa.ul_rtoa_meas_item.type() == asn1::f1ap::ul_rtoa_meas_item_c::types_opts::options::k0) {
            ul_rtoa.ul_rtoa_meas.type  = ul_rtoa_meas_t::type_t::k0;
            ul_rtoa.ul_rtoa_meas.value = asn1_ul_rtoa.ul_rtoa_meas_item.k0();
          } else if (asn1_ul_rtoa.ul_rtoa_meas_item.type() ==
                     asn1::f1ap::ul_rtoa_meas_item_c::types_opts::options::k1) {
            ul_rtoa.ul_rtoa_meas.type  = ul_rtoa_meas_t::type_t::k1;
            ul_rtoa.ul_rtoa_meas.value = asn1_ul_rtoa.ul_rtoa_meas_item.k1();
          } else if (asn1_ul_rtoa.ul_rtoa_meas_item.type() ==
                     asn1::f1ap::ul_rtoa_meas_item_c::types_opts::options::k2) {
            ul_rtoa.ul_rtoa_meas.type  = ul_rtoa_meas_t::type_t::k2;
            ul_rtoa.ul_rtoa_meas.value = asn1_ul_rtoa.ul_rtoa_meas_item.k2();
          } else if (asn1_ul_rtoa.ul_rtoa_meas_item.type() ==
                     asn1::f1ap::ul_rtoa_meas_item_c::types_opts::options::k3) {
            ul_rtoa.ul_rtoa_meas.type  = ul_rtoa_meas_t::type_t::k3;
            ul_rtoa.ul_rtoa_meas.value = asn1_ul_rtoa.ul_rtoa_meas_item.k3();
          } else if (asn1_ul_rtoa.ul_rtoa_meas_item.type() ==
                     asn1::f1ap::ul_rtoa_meas_item_c::types_opts::options::k4) {
            ul_rtoa.ul_rtoa_meas.type  = ul_rtoa_meas_t::type_t::k4;
            ul_rtoa.ul_rtoa_meas.value = asn1_ul_rtoa.ul_rtoa_meas_item.k4();
          } else {
            ul_rtoa.ul_rtoa_meas.type  = ul_rtoa_meas_t::type_t::k5;
            ul_rtoa.ul_rtoa_meas.value = asn1_ul_rtoa.ul_rtoa_meas_item.k5();
          }

          // Fill add path list.
          for (const auto& asn1_add_path_list_item : asn1_ul_rtoa.add_path_list) {
            add_path_list_item_t add_path_list_item;

            add_path_list_item.relative_time_of_path =
                asn1_to_relative_path_delay(asn1_add_path_list_item.relative_path_delay);

            if (asn1_add_path_list_item.path_quality_present) {
              add_path_list_item.path_quality = asn1_to_trp_meas_quality(asn1_add_path_list_item.path_quality);
            }

            if (asn1_add_path_list_item.ie_exts_present) {
              if (asn1_add_path_list_item.ie_exts.multiple_ul_ao_a_present) {
                add_path_list_item.multiple_ul_aoa =
                    asn1_to_multiple_ul_aoa(asn1_add_path_list_item.ie_exts.multiple_ul_ao_a);
              }

              if (asn1_add_path_list_item.ie_exts.path_pwr_present) {
                add_path_list_item.path_pwr =
                    ul_srs_rsrp_pwr_t{asn1_add_path_list_item.ie_exts.path_pwr.first_path_rsrp_p};
              }
            }
          }

          if (asn1_ul_rtoa.ie_exts_present) {
            // Fill extended add path list.
            if (asn1_ul_rtoa.ie_exts.extended_add_path_list_present) {
              for (const auto& asn1_extended_add_path_list_item : asn1_ul_rtoa.ie_exts.extended_add_path_list) {
                add_path_list_item_t extended_add_path_list_item;

                extended_add_path_list_item.relative_time_of_path =
                    asn1_to_relative_path_delay(asn1_extended_add_path_list_item.relative_time_of_path);

                if (asn1_extended_add_path_list_item.path_quality_present) {
                  extended_add_path_list_item.path_quality =
                      asn1_to_trp_meas_quality(asn1_extended_add_path_list_item.path_quality);
                }

                if (asn1_extended_add_path_list_item.multiple_ul_ao_a_present) {
                  extended_add_path_list_item.multiple_ul_aoa =
                      asn1_to_multiple_ul_aoa(asn1_extended_add_path_list_item.multiple_ul_ao_a);
                }

                if (asn1_extended_add_path_list_item.path_pwr_present) {
                  extended_add_path_list_item.path_pwr =
                      ul_srs_rsrp_pwr_t{asn1_extended_add_path_list_item.path_pwr.first_path_rsrp_p};
                }
              }
            }

            // Fill TRP RX TEG info.
            if (asn1_ul_rtoa.ie_exts.trp_rx_teg_info_present) {
              ul_rtoa.trp_rx_teg_info =
                  trp_rx_teg_info_t{asn1_ul_rtoa.ie_exts.trp_rx_teg_info.trp_rx_teg_id,
                                    asn1_ul_rtoa.ie_exts.trp_rx_teg_info.trp_rx_timing_error_margin.to_number()};
            }
          }

          pos_meas_result_item.measured_results_value = ul_rtoa;

        } else if (asn1_pos_meas_result_item.measured_results_value.type() ==
                   asn1::f1ap::measured_results_value_c::types_opts::options::choice_ext) {
          const auto& asn1_ext = asn1_pos_meas_result_item.measured_results_value.choice_ext();
          if (asn1_ext->type() != asn1::f1ap::measured_results_value_ext_ies_o::value_c::types_opts::zo_a_info) {
            logger.warning("Positioning Measurement Response: ignoring unsupported measured results value {}",
                           asn1_ext->type().to_string());
            continue;
          }
          pos_meas_result_item.measured_results_value = asn1_to_zoa(asn1_ext->zo_a_info());
        } else {
          const asn1::f1ap::gnb_rx_tx_time_diff_s& asn1_rx_tx_time_diff =
              asn1_pos_meas_result_item.measured_results_value.gnb_rx_tx_time_diff();
          gnb_rx_tx_time_diff_t rx_tx_time_diff;

          // Fill RX TX time diff.
          if (asn1_rx_tx_time_diff.rx_tx_time_diff.type() ==
              asn1::f1ap::gnb_rx_tx_time_diff_meas_c::types_opts::options::k0) {
            rx_tx_time_diff.rx_tx_time_diff.type  = gnb_rx_tx_time_diff_meas_t::type_t::k0;
            rx_tx_time_diff.rx_tx_time_diff.value = asn1_rx_tx_time_diff.rx_tx_time_diff.k0();
          } else if (asn1_rx_tx_time_diff.rx_tx_time_diff.type() ==
                     asn1::f1ap::gnb_rx_tx_time_diff_meas_c::types_opts::options::k1) {
            rx_tx_time_diff.rx_tx_time_diff.type  = gnb_rx_tx_time_diff_meas_t::type_t::k1;
            rx_tx_time_diff.rx_tx_time_diff.value = asn1_rx_tx_time_diff.rx_tx_time_diff.k1();
          } else if (asn1_rx_tx_time_diff.rx_tx_time_diff.type() ==
                     asn1::f1ap::gnb_rx_tx_time_diff_meas_c::types_opts::options::k2) {
            rx_tx_time_diff.rx_tx_time_diff.type  = gnb_rx_tx_time_diff_meas_t::type_t::k2;
            rx_tx_time_diff.rx_tx_time_diff.value = asn1_rx_tx_time_diff.rx_tx_time_diff.k2();
          } else if (asn1_rx_tx_time_diff.rx_tx_time_diff.type() ==
                     asn1::f1ap::gnb_rx_tx_time_diff_meas_c::types_opts::options::k3) {
            rx_tx_time_diff.rx_tx_time_diff.type  = gnb_rx_tx_time_diff_meas_t::type_t::k3;
            rx_tx_time_diff.rx_tx_time_diff.value = asn1_rx_tx_time_diff.rx_tx_time_diff.k3();
          } else if (asn1_rx_tx_time_diff.rx_tx_time_diff.type() ==
                     asn1::f1ap::gnb_rx_tx_time_diff_meas_c::types_opts::options::k4) {
            rx_tx_time_diff.rx_tx_time_diff.type  = gnb_rx_tx_time_diff_meas_t::type_t::k4;
            rx_tx_time_diff.rx_tx_time_diff.value = asn1_rx_tx_time_diff.rx_tx_time_diff.k4();
          } else {
            rx_tx_time_diff.rx_tx_time_diff.type  = gnb_rx_tx_time_diff_meas_t::type_t::k5;
            rx_tx_time_diff.rx_tx_time_diff.value = asn1_rx_tx_time_diff.rx_tx_time_diff.k5();
          }

          // Fill add path list.
          for (const auto& asn1_add_path_list_item : asn1_rx_tx_time_diff.add_path_list) {
            add_path_list_item_t add_path_list_item;

            add_path_list_item.relative_time_of_path =
                asn1_to_relative_path_delay(asn1_add_path_list_item.relative_path_delay);

            if (asn1_add_path_list_item.path_quality_present) {
              add_path_list_item.path_quality = asn1_to_trp_meas_quality(asn1_add_path_list_item.path_quality);
            }

            if (asn1_add_path_list_item.ie_exts_present) {
              if (asn1_add_path_list_item.ie_exts.multiple_ul_ao_a_present) {
                add_path_list_item.multiple_ul_aoa =
                    asn1_to_multiple_ul_aoa(asn1_add_path_list_item.ie_exts.multiple_ul_ao_a);
              }

              if (asn1_add_path_list_item.ie_exts.path_pwr_present) {
                add_path_list_item.path_pwr =
                    ul_srs_rsrp_pwr_t{asn1_add_path_list_item.ie_exts.path_pwr.first_path_rsrp_p};
              }
            }
          }

          if (asn1_rx_tx_time_diff.ie_exts_present) {
            // Fill extended add path list.
            if (asn1_rx_tx_time_diff.ie_exts.extended_add_path_list_present) {
              for (const auto& asn1_extended_add_path_list_item : asn1_rx_tx_time_diff.ie_exts.extended_add_path_list) {
                add_path_list_item_t extended_add_path_list_item;

                extended_add_path_list_item.relative_time_of_path =
                    asn1_to_relative_path_delay(asn1_extended_add_path_list_item.relative_time_of_path);

                if (asn1_extended_add_path_list_item.path_quality_present) {
                  extended_add_path_list_item.path_quality =
                      asn1_to_trp_meas_quality(asn1_extended_add_path_list_item.path_quality);
                }

                if (asn1_extended_add_path_list_item.multiple_ul_ao_a_present) {
                  extended_add_path_list_item.multiple_ul_aoa =
                      asn1_to_multiple_ul_aoa(asn1_extended_add_path_list_item.multiple_ul_ao_a);
                }

                if (asn1_extended_add_path_list_item.path_pwr_present) {
                  extended_add_path_list_item.path_pwr =
                      ul_srs_rsrp_pwr_t{asn1_extended_add_path_list_item.path_pwr.first_path_rsrp_p};
                }
              }
            }

            // Fill TRP TEG info.
            if (asn1_rx_tx_time_diff.ie_exts.trpteg_info_present) {
              trp_teg_info_t trp_teg_info;
              if (asn1_rx_tx_time_diff.ie_exts.trpteg_info.type() ==
                  asn1::f1ap::trpteg_info_c::types_opts::options::rx_tx_teg) {
                asn1::f1ap::rx_tx_teg_s asn1_rx_tx_teg = asn1_rx_tx_time_diff.ie_exts.trpteg_info.rx_tx_teg();
                rx_tx_teg_t             rx_tx_teg;
                rx_tx_teg.trp_rx_tx_teg_info.trp_rx_tx_teg_id = asn1_rx_tx_teg.trp_rx_tx_teg_info.trp_rx_tx_teg_id;
                rx_tx_teg.trp_rx_tx_teg_info.trp_rx_tx_timing_error_margin.value = rx_tx_timing_error_margin_t::value_t(
                    (uint8_t)asn1_rx_tx_teg.trp_rx_tx_teg_info.trp_rx_tx_timing_error_margin.value);
                if (asn1_rx_tx_teg.trp_tx_teg_info_present) {
                  trp_tx_teg_info_t trp_tx_teg_info;
                  trp_tx_teg_info.trp_tx_teg_id = asn1_rx_tx_teg.trp_tx_teg_info.trp_tx_teg_id;
                  trp_tx_teg_info.trp_tx_timing_error_margin =
                      asn1_rx_tx_teg.trp_tx_teg_info.trp_tx_timing_error_margin.to_number();
                  rx_tx_teg.trp_tx_teg_info = trp_tx_teg_info;
                }

                trp_teg_info = rx_tx_teg;
              } else {
                asn1::f1ap::rx_teg_s asn1_rx_teg = asn1_rx_tx_time_diff.ie_exts.trpteg_info.rx_teg();
                rx_teg_t             rx_teg;

                rx_teg.trp_rx_teg_info.trp_rx_teg_id = asn1_rx_teg.trp_rx_teg_info.trp_rx_teg_id;
                rx_teg.trp_rx_teg_info.trp_rx_timing_error_margin =
                    asn1_rx_teg.trp_rx_teg_info.trp_rx_timing_error_margin.to_number();

                rx_teg.trp_tx_teg_info.trp_tx_teg_id = asn1_rx_teg.trp_tx_teg_info.trp_tx_teg_id;
                rx_teg.trp_tx_teg_info.trp_tx_timing_error_margin =
                    asn1_rx_teg.trp_tx_teg_info.trp_tx_timing_error_margin.to_number();

                trp_teg_info = rx_teg;
              }

              rx_tx_time_diff.trp_teg_info = trp_teg_info;
            }
          }

          pos_meas_result_item.measured_results_value = rx_tx_time_diff;
        }

        // Fill time stamp.
        pos_meas_result_item.time_stamp.sys_frame_num = asn1_pos_meas_result_item.time_stamp.sys_frame_num;
        if (asn1_pos_meas_result_item.time_stamp.slot_idx.type() ==
            asn1::f1ap::time_stamp_slot_idx_c::types_opts::options::scs_15) {
          pos_meas_result_item.time_stamp.slot_idx.type  = time_stamp_slot_idx_t::idx_type::scs15;
          pos_meas_result_item.time_stamp.slot_idx.value = asn1_pos_meas_result_item.time_stamp.slot_idx.scs_15();
        } else if (asn1_pos_meas_result_item.time_stamp.slot_idx.type() ==
                   asn1::f1ap::time_stamp_slot_idx_c::types_opts::options::scs_30) {
          pos_meas_result_item.time_stamp.slot_idx.type  = time_stamp_slot_idx_t::idx_type::scs30;
          pos_meas_result_item.time_stamp.slot_idx.value = asn1_pos_meas_result_item.time_stamp.slot_idx.scs_30();
        } else if (asn1_pos_meas_result_item.time_stamp.slot_idx.type() ==
                   asn1::f1ap::time_stamp_slot_idx_c::types_opts::options::scs_60) {
          pos_meas_result_item.time_stamp.slot_idx.type  = time_stamp_slot_idx_t::idx_type::scs60;
          pos_meas_result_item.time_stamp.slot_idx.value = asn1_pos_meas_result_item.time_stamp.slot_idx.scs_60();
        } else {
          pos_meas_result_item.time_stamp.slot_idx.type  = time_stamp_slot_idx_t::idx_type::scs120;
          pos_meas_result_item.time_stamp.slot_idx.value = asn1_pos_meas_result_item.time_stamp.slot_idx.scs_120();
        }
        if (asn1_pos_meas_result_item.time_stamp.meas_time_present) {
          pos_meas_result_item.time_stamp.meas_time = asn1_pos_meas_result_item.time_stamp.meas_time.to_number();
        }

        // Fill meas quality.
        if (asn1_pos_meas_result_item.meas_quality_present) {
          pos_meas_result_item.meas_quality = asn1_to_trp_meas_quality(asn1_pos_meas_result_item.meas_quality);
        }

        // Fill meas beam info.
        if (asn1_pos_meas_result_item.meas_beam_info_present) {
          meas_beam_info_t meas_beam_info;
          if (asn1_pos_meas_result_item.meas_beam_info.prs_res_id_present) {
            meas_beam_info.prs_res_id = asn1_pos_meas_result_item.meas_beam_info.prs_res_id;
          }
          if (asn1_pos_meas_result_item.meas_beam_info.prs_res_set_id_present) {
            meas_beam_info.prs_res_set_id = asn1_pos_meas_result_item.meas_beam_info.prs_res_set_id;
          }
          if (asn1_pos_meas_result_item.meas_beam_info.ssb_idx_present) {
            meas_beam_info.ssb_idx = asn1_pos_meas_result_item.meas_beam_info.ssb_idx;
          }

          pos_meas_result_item.meas_beam_info = meas_beam_info;
        }

        trp_meas_resp_item.meas_result.push_back(pos_meas_result_item);
      }

      // The TRP Measurement Result IE needs at least one item, as per TS 38.455 section 9.2.37.
      if (trp_meas_resp_item.meas_result.empty()) {
        continue;
      }

      // Fill TRP ID.
      trp_meas_resp_item.trp_id = uint_to_trp_id(asn1_trp_meas_resp_item.trp_id);

      // Fill CGI NR.
      if (asn1_trp_meas_resp_item.ie_exts_present and asn1_trp_meas_resp_item.ie_exts.nr_cgi_present) {
        trp_meas_resp_item.cgi_nr = cgi_from_asn1(asn1_trp_meas_resp_item.ie_exts.nr_cgi).value();
      }

      resp.trp_meas_resp_list.push_back(trp_meas_resp_item);
    }
  }

  if (asn1_resp->crit_diagnostics_present) {
    // TODO: Fill critical diagnostics.
  }

  return resp;
}

static inline asn1::f1ap::srs_configuration_s srs_configuration_to_asn1(const srs_configuration_t& srs_cfg)
{
  asn1::f1ap::srs_configuration_s asn1_srs_cfg;

  for (const auto& srs_carrier_item : srs_cfg.srs_carrier_list) {
    asn1::f1ap::srs_carrier_list_item_s asn1_srs_carrier_item;
    // Fill point A.
    asn1_srs_carrier_item.point_a = srs_carrier_item.point_a;
    // Fill UL CH BW per SCS list.
    for (const auto& scs_specific_carrier : srs_carrier_item.ul_ch_bw_per_scs_list) {
      asn1::f1ap::scs_specific_carrier_s asn1_scs_specific_carrier;
      asn1_scs_specific_carrier.offset_to_carrier = scs_specific_carrier.offset_to_carrier;
      if (scs_specific_carrier.scs == ocudu::subcarrier_spacing::kHz15) {
        asn1_scs_specific_carrier.subcarrier_spacing =
            asn1::f1ap::scs_specific_carrier_s::subcarrier_spacing_opts::options::khz15;
      } else if (scs_specific_carrier.scs == ocudu::subcarrier_spacing::kHz30) {
        asn1_scs_specific_carrier.subcarrier_spacing =
            asn1::f1ap::scs_specific_carrier_s::subcarrier_spacing_opts::options::khz30;
      } else if (scs_specific_carrier.scs == ocudu::subcarrier_spacing::kHz60) {
        asn1_scs_specific_carrier.subcarrier_spacing =
            asn1::f1ap::scs_specific_carrier_s::subcarrier_spacing_opts::options::khz60;
      } else {
        asn1_scs_specific_carrier.subcarrier_spacing =
            asn1::f1ap::scs_specific_carrier_s::subcarrier_spacing_opts::options::khz120;
      }
      asn1_scs_specific_carrier.carrier_bw = scs_specific_carrier.carrier_bandwidth;
      asn1_srs_carrier_item.ul_ch_bw_per_scs_list.push_back(asn1_scs_specific_carrier);
    }
    // Fill active UL BWP.
    asn1_srs_carrier_item.active_ul_bwp.location_and_bw = srs_carrier_item.active_ul_bwp.location_and_bw;
    if (srs_carrier_item.active_ul_bwp.scs == ocudu::subcarrier_spacing::kHz15) {
      asn1_srs_carrier_item.active_ul_bwp.subcarrier_spacing =
          asn1::f1ap::active_ul_bwp_s::subcarrier_spacing_opts::options::khz15;
    } else if (srs_carrier_item.active_ul_bwp.scs == ocudu::subcarrier_spacing::kHz30) {
      asn1_srs_carrier_item.active_ul_bwp.subcarrier_spacing =
          asn1::f1ap::active_ul_bwp_s::subcarrier_spacing_opts::options::khz30;
    } else if (srs_carrier_item.active_ul_bwp.scs == ocudu::subcarrier_spacing::kHz60) {
      asn1_srs_carrier_item.active_ul_bwp.subcarrier_spacing =
          asn1::f1ap::active_ul_bwp_s::subcarrier_spacing_opts::options::khz60;
    } else {
      asn1_srs_carrier_item.active_ul_bwp.subcarrier_spacing =
          asn1::f1ap::active_ul_bwp_s::subcarrier_spacing_opts::options::khz120;
    }
    asn1::string_to_enum(asn1_srs_carrier_item.active_ul_bwp.cp, srs_carrier_item.active_ul_bwp.cp.to_string());
    asn1_srs_carrier_item.active_ul_bwp.tx_direct_current_location =
        srs_carrier_item.active_ul_bwp.tx_direct_current_location;
    if (srs_carrier_item.active_ul_bwp.shift7dot5k_hz.has_value()) {
      asn1_srs_carrier_item.active_ul_bwp.shift7dot5k_hz_present = true;
      if (srs_carrier_item.active_ul_bwp.shift7dot5k_hz.value()) {
        asn1_srs_carrier_item.active_ul_bwp.shift7dot5k_hz =
            asn1::f1ap::active_ul_bwp_s::shift7dot5k_hz_opts::options::true_value;
      }
    }
    // > Fill SRS configuration.
    // >> Fill SRS res list.
    for (const auto& srs_res : srs_carrier_item.active_ul_bwp.srs_cfg.srs_res_list) {
      asn1::f1ap::srs_res_s asn1_srs_res;
      asn1_srs_res.srs_res_id = srs_res.id.ue_res_id;
      asn1::number_to_enum(asn1_srs_res.nrof_srs_ports, (uint8_t)srs_res.nof_ports);
      // Fill TX comb.
      if (srs_res.tx_comb.size == tx_comb_size::n2) {
        asn1::f1ap::tx_comb_c::n2_s_ asn1_n2;
        asn1_n2.comb_offset_n2        = srs_res.tx_comb.tx_comb_offset;
        asn1_n2.cyclic_shift_n2       = srs_res.tx_comb.tx_comb_cyclic_shift;
        asn1_srs_res.tx_comb.set_n2() = asn1_n2;
      } else {
        asn1::f1ap::tx_comb_c::n4_s_ asn1_n4;
        asn1_n4.comb_offset_n4        = srs_res.tx_comb.tx_comb_offset;
        asn1_n4.cyclic_shift_n4       = srs_res.tx_comb.tx_comb_cyclic_shift;
        asn1_srs_res.tx_comb.set_n4() = asn1_n4;
      }
      asn1_srs_res.start_position = srs_res.res_mapping.start_pos;
      asn1::number_to_enum(asn1_srs_res.nrof_symbols, srs_res.res_mapping.nof_symb);
      asn1::number_to_enum(asn1_srs_res.repeat_factor, srs_res.res_mapping.rept_factor);
      asn1_srs_res.freq_domain_position = srs_res.freq_domain_pos;
      asn1_srs_res.freq_domain_shift    = srs_res.freq_domain_shift;
      asn1_srs_res.c_srs                = srs_res.freq_hop.c_srs;
      asn1_srs_res.b_srs                = srs_res.freq_hop.b_srs;
      asn1_srs_res.b_hop                = srs_res.freq_hop.b_hop;
      if (srs_res.grp_or_seq_hop == srs_group_or_sequence_hopping::neither) {
        asn1_srs_res.group_or_seq_hop = asn1::f1ap::srs_res_s::group_or_seq_hop_opts::options::neither;
      } else if (srs_res.grp_or_seq_hop == srs_group_or_sequence_hopping::group_hopping) {
        asn1_srs_res.group_or_seq_hop = asn1::f1ap::srs_res_s::group_or_seq_hop_opts::options::group_hop;
      } else {
        asn1_srs_res.group_or_seq_hop = asn1::f1ap::srs_res_s::group_or_seq_hop_opts::options::seq_hop;
      }
      // >> Fill res type.
      if (srs_res.res_type == srs_resource_type::periodic) {
        asn1::f1ap::res_type_periodic_s asn1_res_type_periodic;
        asn1::number_to_enum(asn1_res_type_periodic.periodicity, (uint16_t)srs_res.periodicity_and_offset->period);
        asn1_res_type_periodic.offset        = srs_res.periodicity_and_offset->offset;
        asn1_srs_res.res_type.set_periodic() = asn1_res_type_periodic;
      } else if (srs_res.res_type == srs_resource_type::semi_persistent) {
        asn1::f1ap::res_type_semi_persistent_s asn1_res_type_semi_persistent;
        asn1::number_to_enum(asn1_res_type_semi_persistent.periodicity,
                             (uint16_t)srs_res.periodicity_and_offset->period);
        asn1_res_type_semi_persistent.offset        = srs_res.periodicity_and_offset->offset;
        asn1_srs_res.res_type.set_semi_persistent() = asn1_res_type_semi_persistent;
      } else {
        asn1::f1ap::res_type_aperiodic_s asn1_res_type_aperiodic;
        asn1_res_type_aperiodic.aperiodic_res_type =
            asn1::f1ap::res_type_aperiodic_s::aperiodic_res_type_opts::options::true_value;
        asn1_srs_res.res_type.set_aperiodic() = asn1_res_type_aperiodic;
      }
      asn1_srs_res.seq_id = srs_res.sequence_id;

      asn1_srs_carrier_item.active_ul_bwp.srs_cfg.srs_res_list.push_back(asn1_srs_res);
    }
    // >> Fill pos SRS res list.
    for (const auto& pos_srs_res_item : srs_carrier_item.active_ul_bwp.srs_cfg.pos_srs_res_list) {
      asn1::f1ap::pos_srs_res_item_s asn1_pos_srs_res_item;
      // Fill SRS pos res ID.
      asn1_pos_srs_res_item.srs_pos_res_id = pos_srs_res_item.id.ue_res_id;
      // Fill TX comb pos.
      if (pos_srs_res_item.tx_comb.size == tx_comb_pos_size::n2) {
        asn1::f1ap::tx_comb_pos_c::n2_s_ asn1_n2;
        asn1_n2.comb_offset_n2                     = pos_srs_res_item.tx_comb.tx_comb_pos_offset;
        asn1_n2.cyclic_shift_n2                    = pos_srs_res_item.tx_comb.tx_comb_pos_cyclic_shift;
        asn1_pos_srs_res_item.tx_comb_pos.set_n2() = asn1_n2;
      } else if (pos_srs_res_item.tx_comb.size == tx_comb_pos_size::n4) {
        asn1::f1ap::tx_comb_pos_c::n4_s_ asn1_n4;
        asn1_n4.comb_offset_n4                     = pos_srs_res_item.tx_comb.tx_comb_pos_offset;
        asn1_n4.cyclic_shift_n4                    = pos_srs_res_item.tx_comb.tx_comb_pos_cyclic_shift;
        asn1_pos_srs_res_item.tx_comb_pos.set_n4() = asn1_n4;
      } else {
        asn1::f1ap::tx_comb_pos_c::n8_s_ asn1_n8;
        asn1_n8.comb_offset_n8                     = pos_srs_res_item.tx_comb.tx_comb_pos_offset;
        asn1_n8.cyclic_shift_n8                    = pos_srs_res_item.tx_comb.tx_comb_pos_cyclic_shift;
        asn1_pos_srs_res_item.tx_comb_pos.set_n8() = asn1_n8;
      }
      // Fill start position.
      asn1_pos_srs_res_item.start_position = pos_srs_res_item.res_mapping.start_pos;
      // Fill nrof symbols.
      asn1::number_to_enum(asn1_pos_srs_res_item.nrof_symbols, pos_srs_res_item.res_mapping.nof_symb);
      // Fill freq domain shift.
      asn1_pos_srs_res_item.freq_domain_shift = pos_srs_res_item.freq_domain_shift;
      // Fill c SRS.
      asn1_pos_srs_res_item.c_srs = pos_srs_res_item.c_srs;
      // Fill group or seq hop.
      if (pos_srs_res_item.grp_or_seq_hop == srs_group_or_sequence_hopping::neither) {
        asn1_pos_srs_res_item.group_or_seq_hop =
            asn1::f1ap::pos_srs_res_item_s::group_or_seq_hop_opts::options::neither;
      } else if (pos_srs_res_item.grp_or_seq_hop == srs_group_or_sequence_hopping::group_hopping) {
        asn1_pos_srs_res_item.group_or_seq_hop =
            asn1::f1ap::pos_srs_res_item_s::group_or_seq_hop_opts::options::group_hop;
      } else {
        asn1_pos_srs_res_item.group_or_seq_hop =
            asn1::f1ap::pos_srs_res_item_s::group_or_seq_hop_opts::options::seq_hop;
      }
      // Fill res type pos.
      if (pos_srs_res_item.res_type == srs_resource_type::periodic) {
        asn1::f1ap::res_type_periodic_pos_s asn1_res_type_pos_periodic;
        asn1::number_to_enum(asn1_res_type_pos_periodic.periodicity,
                             (uint16_t)pos_srs_res_item.periodicity_and_offset->period);
        asn1_res_type_pos_periodic.offset                 = pos_srs_res_item.periodicity_and_offset->offset;
        asn1_pos_srs_res_item.res_type_pos.set_periodic() = asn1_res_type_pos_periodic;
      } else if (pos_srs_res_item.res_type == srs_resource_type::semi_persistent) {
        asn1::f1ap::res_type_semi_persistent_pos_s asn1_res_type_semi_persisten_pos;
        asn1::number_to_enum(asn1_res_type_semi_persisten_pos.periodicity,
                             (uint16_t)pos_srs_res_item.periodicity_and_offset->period);
        asn1_res_type_semi_persisten_pos.offset                  = pos_srs_res_item.periodicity_and_offset->offset;
        asn1_pos_srs_res_item.res_type_pos.set_semi_persistent() = asn1_res_type_semi_persisten_pos;
      } else {
        asn1::f1ap::res_type_aperiodic_pos_s asn1_res_type_aperiodic_pos;
        asn1_res_type_aperiodic_pos.slot_offset            = pos_srs_res_item.slot_offset.value();
        asn1_pos_srs_res_item.res_type_pos.set_aperiodic() = asn1_res_type_aperiodic_pos;
      }
      // Fill seq ID.
      asn1_pos_srs_res_item.seq_id = pos_srs_res_item.sequence_id;

      // Fill spatial relation pos.
      if (pos_srs_res_item.spatial_relation_info.has_value()) {
        asn1_pos_srs_res_item.spatial_relation_pos_present = true;
        if (std::holds_alternative<srs_config::srs_pos_resource::srs_spatial_relation_pos::ssb>(
                pos_srs_res_item.spatial_relation_info->reference_signal)) {
          asn1::f1ap::ssb_s                                           asn1_ssb;
          srs_config::srs_pos_resource::srs_spatial_relation_pos::ssb ssb =
              std::get<srs_config::srs_pos_resource::srs_spatial_relation_pos::ssb>(
                  pos_srs_res_item.spatial_relation_info->reference_signal);
          asn1_ssb.pci_nr = ssb.pci_nr;
          if (ssb.ssb_idx.has_value()) {
            asn1_ssb.ssb_idx_present = true;
            asn1_ssb.ssb_idx         = ssb.ssb_idx->value();
          }
          asn1_pos_srs_res_item.spatial_relation_pos.set_ssb_pos() = asn1_ssb;
        } else {
          asn1::f1ap::pr_si_nformation_pos_s                          asn1_prs_info;
          srs_config::srs_pos_resource::srs_spatial_relation_pos::prs prs =
              std::get<srs_config::srs_pos_resource::srs_spatial_relation_pos::prs>(
                  pos_srs_res_item.spatial_relation_info->reference_signal);

          asn1_prs_info.prs_id_pos         = prs.id;
          asn1_prs_info.prs_res_set_id_pos = prs.prs_res_set_id;
          if (prs.prs_res_id.has_value()) {
            asn1_prs_info.prs_res_id_pos_present = true;
            asn1_prs_info.prs_res_id_pos         = prs.prs_res_id.value();
          }
          asn1_pos_srs_res_item.spatial_relation_pos.set_pr_si_nformation_pos() = asn1_prs_info;
        }
      }

      asn1_srs_carrier_item.active_ul_bwp.srs_cfg.pos_srs_res_list.push_back(asn1_pos_srs_res_item);
    }

    // >> Fill SRS res set list.
    for (const auto& srs_res_set : srs_carrier_item.active_ul_bwp.srs_cfg.srs_res_set_list) {
      asn1::f1ap::srs_res_set_s asn1_srs_res_set;
      asn1_srs_res_set.srs_res_set_id = srs_res_set.id;
      for (const auto& srs_res_id : srs_res_set.srs_res_id_list) {
        asn1_srs_res_set.srs_res_id_list.push_back(srs_res_id);
      }
      // Fill res set type.
      if (std::holds_alternative<srs_config::srs_resource_set::periodic_resource_type>(srs_res_set.res_type)) {
        asn1::f1ap::res_set_type_periodic_s asn1_res_set_type_periodic;
        asn1_res_set_type_periodic.periodic_set =
            asn1::f1ap::res_set_type_periodic_s::periodic_set_opts::options::true_value;
        asn1_srs_res_set.res_set_type.set_periodic() = asn1_res_set_type_periodic;
      } else if (std::holds_alternative<srs_config::srs_resource_set::semi_persistent_resource_type>(
                     srs_res_set.res_type)) {
        asn1::f1ap::res_set_type_semi_persistent_s asn1_res_set_type_semi_persistent;
        asn1_res_set_type_semi_persistent.semi_persistent_set =
            asn1::f1ap::res_set_type_semi_persistent_s::semi_persistent_set_opts::options::true_value;
        asn1_srs_res_set.res_set_type.set_semi_persistent() = asn1_res_set_type_semi_persistent;
      } else {
        asn1::f1ap::res_set_type_aperiodic_s                  asn1_res_set_type_aperiodic;
        srs_config::srs_resource_set::aperiodic_resource_type aperiodic =
            std::get<srs_config::srs_resource_set::aperiodic_resource_type>(srs_res_set.res_type);
        asn1_res_set_type_aperiodic.srs_res_trigger_list = aperiodic.aperiodic_srs_res_trigger;
        asn1_res_set_type_aperiodic.slotoffset           = aperiodic.slot_offset.value();
        asn1_srs_res_set.res_set_type.set_aperiodic()    = asn1_res_set_type_aperiodic;
      }

      asn1_srs_carrier_item.active_ul_bwp.srs_cfg.srs_res_set_list.push_back(asn1_srs_res_set);
    }

    // >> Fill pos SRS res set list.
    for (const auto& pos_srs_res_set_item : srs_carrier_item.active_ul_bwp.srs_cfg.pos_srs_res_set_list) {
      asn1::f1ap::pos_srs_res_set_item_s asn1_pos_srs_res_set_item;

      asn1_pos_srs_res_set_item.possrs_res_set_id = pos_srs_res_set_item.id;
      for (const auto& pos_srs_res_id : pos_srs_res_set_item.srs_res_id_list) {
        asn1_pos_srs_res_set_item.poss_rs_res_id_list.push_back(pos_srs_res_id);
      }
      // Fill res set type.
      if (std::holds_alternative<srs_config::srs_resource_set::periodic_resource_type>(pos_srs_res_set_item.res_type)) {
        asn1::f1ap::pos_res_set_type_pr_s asn1_pos_res_set_type_periodic;
        asn1_pos_res_set_type_periodic.posperiodic_set =
            asn1::f1ap::pos_res_set_type_pr_s::posperiodic_set_opts::options::true_value;
        asn1_pos_srs_res_set_item.posres_set_type.set_periodic() = asn1_pos_res_set_type_periodic;
      } else if (std::holds_alternative<srs_config::srs_resource_set::semi_persistent_resource_type>(
                     pos_srs_res_set_item.res_type)) {
        asn1::f1ap::pos_res_set_type_sp_s asn1_res_set_type_semi_persistent;
        asn1_res_set_type_semi_persistent.possemi_persistent_set =
            asn1::f1ap::pos_res_set_type_sp_s::possemi_persistent_set_opts::options::true_value;
        asn1_pos_srs_res_set_item.posres_set_type.set_semi_persistent() = asn1_res_set_type_semi_persistent;
      } else {
        asn1::f1ap::pos_res_set_type_ap_s asn1_res_set_type_aperiodic;
        asn1_res_set_type_aperiodic.srs_res_trigger_list =
            std::get<srs_config::srs_resource_set::aperiodic_resource_type>(pos_srs_res_set_item.res_type)
                .aperiodic_srs_res_trigger;
        asn1_pos_srs_res_set_item.posres_set_type.set_aperiodic() = asn1_res_set_type_aperiodic;
      }

      asn1_srs_carrier_item.active_ul_bwp.srs_cfg.pos_srs_res_set_list.push_back(asn1_pos_srs_res_set_item);
    }

    // Fill PCI.
    if (srs_carrier_item.pci_nr.has_value()) {
      asn1_srs_carrier_item.pci_present = true;
      asn1_srs_carrier_item.pci         = srs_carrier_item.pci_nr.value();
    }

    asn1_srs_cfg.srs_carrier_list.push_back(asn1_srs_carrier_item);
  }

  return asn1_srs_cfg;
}

static void fill_asn1_positioning_measurement_request(asn1::f1ap::positioning_meas_request_s& asn1_request,
                                                      const measurement_request_t&            request)
{
  asn1_request->lmf_meas_id = to_underlying(request.lmf_meas_id);
  asn1_request->ran_meas_id = to_underlying(request.ran_meas_id);

  // Fill TRP meas request list.
  for (const auto& trp_meas_request_item : request.trp_meas_request_list) {
    asn1::f1ap::trp_meas_request_item_s asn1_trp_meas_request_item;
    asn1_trp_meas_request_item.trp_id = to_underlying(trp_meas_request_item.trp_id);
    if (trp_meas_request_item.search_win_info.has_value()) {
      asn1_trp_meas_request_item.search_win_info_present = true;
      asn1_trp_meas_request_item.search_win_info.expected_propagation_delay =
          trp_meas_request_item.search_win_info.value().expected_propagation_delay;
      asn1_trp_meas_request_item.search_win_info.delay_uncertainty =
          trp_meas_request_item.search_win_info.value().delay_uncertainty;
    }

    if (trp_meas_request_item.cgi_nr.has_value()) {
      asn1_trp_meas_request_item.ie_exts_present        = true;
      asn1_trp_meas_request_item.ie_exts.nr_cgi_present = true;
      asn1_trp_meas_request_item.ie_exts.nr_cgi.nr_cell_id.from_number(trp_meas_request_item.cgi_nr->nci.value());
      asn1_trp_meas_request_item.ie_exts.nr_cgi.plmn_id = trp_meas_request_item.cgi_nr->plmn_id.to_bytes();
    }

    if (trp_meas_request_item.aoa_search_win.has_value()) {
      asn1_trp_meas_request_item.ie_exts_present                 = true;
      asn1_trp_meas_request_item.ie_exts.ao_a_search_win_present = true;

      if (std::holds_alternative<expected_ul_aoa_t>(trp_meas_request_item.aoa_search_win->angle_meas)) {
        const expected_ul_aoa_t& expected_ul_aoa =
            std::get<expected_ul_aoa_t>(trp_meas_request_item.aoa_search_win->angle_meas);
        asn1::f1ap::expected_ul_ao_a_s& asn1_expected_ul_aoa =
            asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.set_expected_ul_ao_a();
        asn1_expected_ul_aoa.expected_azimuth_ao_a.expected_azimuth_ao_a_value =
            expected_ul_aoa.expected_azimuth_aoa.expected_azimuth_aoa_value;
        asn1_expected_ul_aoa.expected_azimuth_ao_a.expected_azimuth_ao_a_uncertainty =
            expected_ul_aoa.expected_azimuth_aoa.expected_azimuth_aoa_uncertainty;

        if (expected_ul_aoa.expected_zenith_aoa.has_value()) {
          asn1_expected_ul_aoa.expected_zenith_ao_a_present = true;
          asn1_expected_ul_aoa.expected_zenith_ao_a.expected_zenith_ao_a_value =
              expected_ul_aoa.expected_zenith_aoa->expected_zenith_aoa_value;
          asn1_expected_ul_aoa.expected_zenith_ao_a.expected_zenith_ao_a_uncertainty =
              expected_ul_aoa.expected_zenith_aoa->expected_zenith_aoa_uncertainty;
        }
      } else {
        const expected_zoa_only_t& expected_zoa =
            std::get<expected_zoa_only_t>(trp_meas_request_item.aoa_search_win->angle_meas);
        asn1::f1ap::expected_zo_a_only_s& asn1_expected_zoa =
            asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.set_expected_zo_a();
        asn1_expected_zoa.expected_zo_a_only.expected_zenith_ao_a_value =
            expected_zoa.expected_zoa_only.expected_zenith_aoa_value;
        asn1_expected_zoa.expected_zo_a_only.expected_zenith_ao_a_uncertainty =
            expected_zoa.expected_zoa_only.expected_zenith_aoa_uncertainty;
      }

      if (trp_meas_request_item.aoa_search_win->lcs_to_gcs_translation.has_value()) {
        asn1_trp_meas_request_item.ie_exts.ao_a_search_win.lcs_to_gcs_translation_present = true;
        asn1_trp_meas_request_item.ie_exts.ao_a_search_win.lcs_to_gcs_translation.alpha =
            trp_meas_request_item.aoa_search_win->lcs_to_gcs_translation->alpha;
        asn1_trp_meas_request_item.ie_exts.ao_a_search_win.lcs_to_gcs_translation.beta =
            trp_meas_request_item.aoa_search_win->lcs_to_gcs_translation->beta;
        asn1_trp_meas_request_item.ie_exts.ao_a_search_win.lcs_to_gcs_translation.gamma =
            trp_meas_request_item.aoa_search_win->lcs_to_gcs_translation->gamma;
      }
    }

    if (trp_meas_request_item.nof_trp_rx_teg.has_value()) {
      asn1_trp_meas_request_item.ie_exts_present                = true;
      asn1_trp_meas_request_item.ie_exts.nof_trp_rx_teg_present = true;
      asn1::number_to_enum(asn1_trp_meas_request_item.ie_exts.nof_trp_rx_teg,
                           trp_meas_request_item.nof_trp_rx_teg.value());
    }

    if (trp_meas_request_item.nof_trp_rx_tx_teg.has_value()) {
      asn1_trp_meas_request_item.ie_exts_present                   = true;
      asn1_trp_meas_request_item.ie_exts.nof_trp_rx_tx_teg_present = true;
      asn1::number_to_enum(asn1_trp_meas_request_item.ie_exts.nof_trp_rx_tx_teg,
                           trp_meas_request_item.nof_trp_rx_tx_teg.value());
    }

    asn1_request->trp_meas_request_list.push_back(asn1_trp_meas_request_item);
  }

  // Fill report characteristics.
  if (request.report_characteristics == report_characteristics_t::on_demand) {
    asn1_request->pos_report_characteristics = asn1::f1ap::pos_report_characteristics_opts::options::ondemand;
  } else {
    asn1_request->pos_report_characteristics = asn1::f1ap::pos_report_characteristics_opts::options::periodic;
  }

  // Fill meas periodicity.
  if (request.meas_periodicity.has_value()) {
    asn1_request->pos_meas_periodicity_present = true;
    asn1::number_to_enum(asn1_request->pos_meas_periodicity, (uint32_t)request.meas_periodicity.value());
  }

  // Fill TRP meas quantities.
  for (const auto& trp_meas_quantities_list_item : request.trp_meas_quantities) {
    asn1::f1ap::pos_meas_quantities_item_s asn1_pos_meas_quantities_item;

    if (trp_meas_quantities_list_item.trp_meas_quantities_item == trp_meas_quantities_item_t::gnb_rx_tx_time_diff) {
      asn1_pos_meas_quantities_item.pos_meas_type = asn1::f1ap::pos_meas_type_opts::options::gnb_rx_tx;
    } else if (trp_meas_quantities_list_item.trp_meas_quantities_item == trp_meas_quantities_item_t::ul_srs_rsrp) {
      asn1_pos_meas_quantities_item.pos_meas_type = asn1::f1ap::pos_meas_type_opts::options::ul_srs_rsrp;
    } else if (trp_meas_quantities_list_item.trp_meas_quantities_item == trp_meas_quantities_item_t::ul_aoa) {
      asn1_pos_meas_quantities_item.pos_meas_type = asn1::f1ap::pos_meas_type_opts::options::ul_aoa;
    } else if (trp_meas_quantities_list_item.trp_meas_quantities_item == trp_meas_quantities_item_t::ul_rtoa) {
      asn1_pos_meas_quantities_item.pos_meas_type = asn1::f1ap::pos_meas_type_opts::options::ul_rtoa;
    } else if (trp_meas_quantities_list_item.trp_meas_quantities_item == trp_meas_quantities_item_t::multiple_ul_aoa) {
      asn1_pos_meas_quantities_item.pos_meas_type = asn1::f1ap::pos_meas_type_opts::options::multiple_ul_aoa;
    } else {
      asn1_pos_meas_quantities_item.pos_meas_type = asn1::f1ap::pos_meas_type_opts::options::ul_srs_rsrpp;
    }

    if (trp_meas_quantities_list_item.timing_report_granularity_factor.has_value()) {
      asn1_pos_meas_quantities_item.timing_report_granularity_factor_present = true;
      asn1_pos_meas_quantities_item.timing_report_granularity_factor =
          trp_meas_quantities_list_item.timing_report_granularity_factor.value();
    }

    asn1_request->pos_meas_quantities.push_back(asn1_pos_meas_quantities_item);
  }

  // Fill SFN initialization time.
  if (request.sfn_initialization_time.has_value()) {
    asn1_request->sfn_initisation_time_present = true;
    asn1_request->sfn_initisation_time.from_number(request.sfn_initialization_time.value());
  }

  // Fill SRS configuration.
  if (request.srs_config.has_value()) {
    asn1_request->srs_cfg_present = true;
    asn1_request->srs_cfg         = srs_configuration_to_asn1(request.srs_config.value());
  }

  // Fill measurement beam info request.
  if (request.meas_beam_info_request.has_value()) {
    asn1_request->meas_beam_info_request_present = true;
    asn1::bool_to_enum(asn1_request->meas_beam_info_request, request.meas_beam_info_request.value());
  }

  // Fill system frame number.
  if (request.sys_frame_num.has_value()) {
    asn1_request->sys_frame_num_present = true;
    asn1_request->sys_frame_num         = request.sys_frame_num.value();
  }

  // Fill slot number.
  if (request.slot_num.has_value()) {
    asn1_request->slot_num_present = true;
    asn1_request->slot_num         = request.slot_num.value();
  }

  // Fill measurement periodicity extended.
  if (request.meas_periodicity_extended.has_value()) {
    asn1_request->pos_meas_periodicity_extended_present = true;
    asn1::number_to_enum(asn1_request->pos_meas_periodicity_extended, request.meas_periodicity_extended.value());
  }

  // Fill response time.
  if (request.resp_time.has_value()) {
    asn1_request->resp_time_present = true;
    asn1_request->resp_time.time    = request.resp_time->time;
    asn1_request->resp_time.time_unit =
        asn1::f1ap::resp_time_s::time_unit_opts::options((uint8_t)request.resp_time->time_unit);
  }

  // Fill measurement characteristics request indication.
  if (request.meas_characteristics_request_ind.has_value()) {
    asn1_request->meas_characteristics_request_ind_present = true;
    asn1_request->meas_characteristics_request_ind.from_number(request.meas_characteristics_request_ind.value());
  }

  // Fill measurement time occasion.
  if (request.meas_time_occasion.has_value()) {
    asn1_request->meas_time_occasion_present = true;
    asn1::bool_to_enum(asn1_request->meas_time_occasion, request.meas_time_occasion.value());
  }

  // Fill measurement amount.
  if (request.meas_amount.has_value()) {
    asn1_request->pos_meas_amount_present = true;
    asn1::bool_to_enum(asn1_request->pos_meas_amount, request.meas_amount.value());
  }
}
