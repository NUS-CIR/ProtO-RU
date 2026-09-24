// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "e_cid_measurement_initiation_procedure.h"
#include "../nrppa_asn1_converters.h"
#include "../nrppa_asn1_helpers.h"
#include "../nrppa_helper.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/nrppa/common.h"
#include "ocudu/asn1/nrppa/nrppa.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/support/async/coroutine.h"
#include <algorithm>

using namespace ocudu;
using namespace ocucp;

/// \brief Convert the measurement periodicity of the request to milliseconds.
/// \param[in] request The E-CID measurement initiation request.
/// \returns The measurement periodicity in milliseconds.
static std::chrono::milliseconds get_meas_period_ms(const nrppa_e_cid_meas_initiation_request& request);

e_cid_measurement_initiation_procedure::e_cid_measurement_initiation_procedure(
    cu_cp_ue_index_t                           ue_index_,
    const nrppa_e_cid_meas_initiation_request& request_,
    uint16_t                                   transaction_id_,
    nrppa_ue_context_list&                     ue_ctxt_list_,
    nrppa_du_context_list&                     du_ctxt_list_,
    nrppa_cu_cp_notifier&                      cu_cp_notifier_,
    const std::map<plmn_identity, unsigned>&   plmn_to_tac_,
    nrppa_impl&                                parent_,
    ocudulog::basic_logger&                    logger_) :
  ue_index(ue_index_),
  e_cid_meas_init_request(request_),
  transaction_id(transaction_id_),
  ue_ctxt_list(ue_ctxt_list_),
  du_ctxt_list(du_ctxt_list_),
  cu_cp_notifier(cu_cp_notifier_),
  plmn_to_tac(plmn_to_tac_),
  parent(parent_),
  logger(logger_)
{
}

void e_cid_measurement_initiation_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  if (!ue_ctxt_list.contains(ue_index)) {
    logger.info("ue={}: Stopping \"{}\". UE was already removed", ue_index, name());
    CORO_EARLY_RETURN();
  }

  // Copy the UE identities. The context may be removed while the F1AP request is pending.
  ue_ids   = ue_ctxt_list[ue_index].ue_ids;
  du_index = ue_ctxt_list[ue_index].get_cu_cp_ue() != nullptr ? ue_ctxt_list[ue_index].get_cu_cp_ue()->get_du_index()
                                                              : cu_cp_du_index_t::invalid;

  logger.info("ue={}: \"{}\" started...", ue_index, name());

  aoa_requested = std::any_of(
      e_cid_meas_init_request.meas_quantities.begin(),
      e_cid_meas_init_request.meas_quantities.end(),
      [](const nrppa_meas_quantities_item& item) { return is_du_sourced_meas_quantity(item.meas_quantities_value); });

  if (e_cid_meas_init_request.report_characteristics == report_characteristics_t::periodic) {
    is_on_demand_measurement = false;

    // Check if periodic measurements are already configured.
    if (ue_ctxt_list[ue_index].meas_report_timer.is_running()) {
      logger.warning("ue={}: Stopping \"{}\". Periodic measurements are already configured", ue_index, name());
      send_failure(nrppa_cause_protocol_t::msg_not_compatible_with_receiver_state);
      CORO_EARLY_RETURN();
    }

    // Reject the request if periodic reporting was requested without the mandatory Measurement Periodicity IE.
    if (!e_cid_meas_init_request.meas_periodicity.has_value()) {
      logger.warning(
          "ue={}: Stopping \"{}\". Periodic reporting requested without Measurement Periodicity", ue_index, name());
      send_failure(nrppa_cause_protocol_t::semantic_error);
      CORO_EARLY_RETURN();
    }

    // Reject the request if NR Angle of Arrival was requested without its own mandatory periodicity, as per
    // TS 38.455 section 9.1.1.1.
    if (aoa_requested && !e_cid_meas_init_request.meas_periodicity_nr_aoa.has_value()) {
      logger.warning("ue={}: Stopping \"{}\". Periodic NR Angle of Arrival requested without Measurement Periodicity "
                     "NR-AoA",
                     ue_index,
                     name());
      send_failure(nrppa_cause_protocol_t::semantic_error);
      CORO_EARLY_RETURN();
    }

    if (aoa_requested) {
      // The gNB-DU paces the NR Angle of Arrival reporting on its own periodicity, as per TS 38.473
      // section 9.2.12.20, and reports through the E-CID Measurement Report procedure.
      if (!du_ctxt_list.contains(du_index)) {
        logger.warning("ue={}: Stopping \"{}\". DU serving the UE is not connected", ue_index, name());
        send_failure(nrppa_cause_radio_network_t::requested_item_temporarily_not_available);
        CORO_EARLY_RETURN();
      }

      CORO_AWAIT_VALUE(du_meas_outcome,
                       du_ctxt_list[du_index].f1ap->on_e_cid_measurement_request(create_du_measurement_request()));

      // The UE may have been removed while the F1AP request was pending.
      if (!ue_ctxt_list.contains(ue_index)) {
        logger.info("ue={}: Stopping \"{}\". UE was removed while the F1AP request was pending", ue_index, name());
        CORO_EARLY_RETURN();
      }

      if (du_meas_outcome.has_value()) {
        ue_ctxt_list[ue_index].du_e_cid_meas_active = true;
      } else {
        // The procedure continues with the quantities that the RRC measurement reports provide.
        logger.info("ue={}: The gNB-DU did not initiate the E-CID measurement", ue_index);
      }
    }

    // Fail only when the gNB-CU cannot report any of the requested quantities.
    const bool rrc_quantity_requested = std::any_of(e_cid_meas_init_request.meas_quantities.begin(),
                                                    e_cid_meas_init_request.meas_quantities.end(),
                                                    [](const nrppa_meas_quantities_item& item) {
                                                      return is_rrc_sourced_meas_quantity(item.meas_quantities_value);
                                                    });
    if (!rrc_quantity_requested && !ue_ctxt_list[ue_index].du_e_cid_meas_active) {
      logger.warning(
          "ue={}: Stopping \"{}\". No requested measurement quantity can be reported periodically", ue_index, name());
      send_failure(nrppa_cause_radio_network_t::requested_item_not_supported);
      CORO_EARLY_RETURN();
    }

    // Start the local report timer only once the measurement is established, so that a failure does not leave the
    // CU-CP reporting to an LMF that was told the procedure failed.
    setup_periodic_measurement();

  } else {
    is_on_demand_measurement = true;

    // Get the measurement results the RRC measurement reports provide.
    get_measurement_result();

    if (aoa_requested) {
      // The gNB-DU measures the UL Angle of Arrival, so request it over F1AP.
      if (!du_ctxt_list.contains(du_index)) {
        logger.warning("ue={}: Stopping \"{}\". DU serving the UE is not connected", ue_index, name());
        send_failure(nrppa_cause_radio_network_t::requested_item_temporarily_not_available);
        CORO_EARLY_RETURN();
      }

      CORO_AWAIT_VALUE(du_meas_outcome,
                       du_ctxt_list[du_index].f1ap->on_e_cid_measurement_request(create_du_measurement_request()));

      // The UE may have been removed while the F1AP request was pending.
      if (!ue_ctxt_list.contains(ue_index)) {
        logger.info("ue={}: Stopping \"{}\". UE was removed while the F1AP request was pending", ue_index, name());
        CORO_EARLY_RETURN();
      }

      handle_du_measurement_outcome();
    }
  }

  // Pack E-CID Measurement Initiation Response and forward to CU-CP.
  handle_procedure_outcome(is_on_demand_measurement);

  CORO_RETURN();
}

void e_cid_measurement_initiation_procedure::setup_periodic_measurement()
{
  nrppa_ue_context& ue_ctxt = ue_ctxt_list[ue_index];

  ue_ctxt.meas_quantities     = e_cid_meas_init_request.meas_quantities;
  ue_ctxt.meas_periodicity_ms = get_meas_period_ms(e_cid_meas_init_request);

  ue_ctxt.logger.log_debug("Setting measurement report timer to {}ms", ue_ctxt.meas_periodicity_ms.value().count());

  // Start timer for periodic reporting.
  parent.initialize_meas_report_timer(ue_index, ue_ctxt.meas_periodicity_ms.value());
}

e_cid_measurement_request_t e_cid_measurement_initiation_procedure::create_du_measurement_request() const
{
  e_cid_measurement_request_t request;

  request.ue_index                = ue_index;
  request.lmf_ue_meas_id          = ue_ids.lmf_ue_meas_id;
  request.ran_ue_meas_id          = ue_ids.ran_ue_meas_id;
  request.report_characteristics  = e_cid_meas_init_request.report_characteristics;
  request.meas_periodicity        = e_cid_meas_init_request.meas_periodicity;
  request.meas_periodicity_nr_aoa = e_cid_meas_init_request.meas_periodicity_nr_aoa;

  for (const auto& meas_quantity : e_cid_meas_init_request.meas_quantities) {
    if (meas_quantity.meas_quantities_value == nrppa_meas_quantities_value::angle_of_arrival_nr) {
      request.meas_quantities.push_back(e_cid_meas_quantities_item_t::nr_angle_of_arrival);
    }
  }

  return request;
}

void e_cid_measurement_initiation_procedure::get_measurement_result()
{
  nrppa_cu_cp_ue_notifier* ue = ue_ctxt_list[ue_index].get_cu_cp_ue();
  ocudu_assert(ue != nullptr,
               "ue={} ran_ue={} lmf_ue={}: UE for UE context doesn't exist",
               ue_ids.ue_index,
               fmt::underlying(ue_ids.ran_ue_meas_id),
               fmt::underlying(ue_ids.lmf_ue_meas_id));

  std::optional<cell_measurement_positioning_info>& ue_measurement_results = ue->on_measurement_results_required();

  // The Serving Cell ID and Serving Cell TAC IEs are mandatory in the E-CID Measurement Result IE, as per TS 38.455
  // section 9.2.5. Prefer the serving cell the RRC measurement reports name, and fall back to the cell the UE is
  // camping on when the UE has not reported any measurement yet.
  std::optional<nr_cell_global_id_t> serving_cell_id;
  if (ue_measurement_results.has_value()) {
    serving_cell_id = ue_measurement_results.value().serving_cell_id;
  } else {
    serving_cell_id = ue->get_serving_cell_id();
  }

  if (!serving_cell_id.has_value()) {
    logger.warning("ue={}: Serving cell of the UE is unknown", ue_index);
    e_cid_meas_results =
        make_unexpected(nrppa_cause_t{nrppa_cause_radio_network_t::requested_item_temporarily_not_available});
    return;
  }

  if (plmn_to_tac.find(serving_cell_id.value().plmn_id) == plmn_to_tac.end()) {
    logger.warning("ue={}: TAC for PLMN={} not found", ue_index, serving_cell_id.value().plmn_id);
    e_cid_meas_results =
        make_unexpected(nrppa_cause_t{nrppa_cause_radio_network_t::requested_item_temporarily_not_available});
    return;
  }

  nrppa_e_cid_meas_result meas_result;
  meas_result.serving_cell_id  = serving_cell_id.value();
  meas_result.serving_cell_tac = plmn_to_tac.at(serving_cell_id.value().plmn_id);

  if (ue_measurement_results.has_value()) {
    meas_result.measured_results =
        fill_rrc_measured_results(ue_measurement_results.value(), e_cid_meas_init_request.meas_quantities);
  }

  e_cid_meas_results = meas_result;
}

void e_cid_measurement_initiation_procedure::handle_du_measurement_outcome()
{
  if (!du_meas_outcome.has_value()) {
    logger.warning("ue={}: The gNB-DU could not initiate the E-CID measurement", ue_index);
    return;
  }

  if (!du_meas_outcome.value().e_cid_meas_result.has_value()) {
    return;
  }

  if (!e_cid_meas_results.has_value()) {
    return;
  }

  std::vector<nrppa_measured_results_value> du_results =
      fill_du_measured_results(du_meas_outcome.value().e_cid_meas_result.value());
  e_cid_meas_results.value().measured_results.insert(
      e_cid_meas_results.value().measured_results.end(), du_results.begin(), du_results.end());
}

void e_cid_measurement_initiation_procedure::send_failure(nrppa_cause_t cause)
{
  send_ul_nrppa_pdu(logger,
                    cu_cp_notifier,
                    create_e_cid_measurement_initiation_failure(cause),
                    "ECIDMeasInitiationResponse",
                    "ECIDMeasInitiationFailure",
                    ue_ids.ue_index);
}

void e_cid_measurement_initiation_procedure::handle_procedure_outcome(bool on_demand)
{
  // If we are not in on-demand mode, we must have a measurement result.
  if (on_demand && (!e_cid_meas_results.has_value() || e_cid_meas_results.value().measured_results.empty())) {
    // An empty result means the requested quantities are known but not measured yet, so the LMF may retry. A
    // quantity the CU-CP cannot serve at all is reported as not supported.
    nrppa_cause_t cause = e_cid_meas_results.has_value()
                              ? nrppa_cause_t{nrppa_cause_radio_network_t::requested_item_temporarily_not_available}
                              : e_cid_meas_results.error();
    e_cid_meas_outcome  = create_e_cid_measurement_initiation_failure(cause);
    logger.warning("ue={}: \"{}\" failed", ue_index, name());
  } else {
    e_cid_meas_outcome = create_e_cid_measurement_initiation_response(on_demand);
    logger.info("ue={}: \"{}\" finished successfully", ue_index, name());
  }

  // Send response to CU-CP.
  send_ul_nrppa_pdu(logger,
                    cu_cp_notifier,
                    e_cid_meas_outcome,
                    "ECIDMeasInitiationResponse",
                    "ECIDMeasInitiationFailure",
                    ue_ids.ue_index);
}

asn1::nrppa::nr_ppa_pdu_c
e_cid_measurement_initiation_procedure::create_e_cid_measurement_initiation_failure(nrppa_cause_t cause) const
{
  asn1::nrppa::nr_ppa_pdu_c asn1_fail;

  asn1_fail.set_unsuccessful_outcome().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_INITIATION);
  asn1_fail.unsuccessful_outcome().nrppatransaction_id = transaction_id;
  asn1::nrppa::e_c_id_meas_initiation_fail_s& meas_init_fail =
      asn1_fail.unsuccessful_outcome().value.e_c_id_meas_initiation_fail();

  meas_init_fail->lmf_ue_meas_id = to_underlying(ue_ids.lmf_ue_meas_id);
  meas_init_fail->cause          = cause_to_asn1(cause);

  return asn1_fail;
}

asn1::nrppa::nr_ppa_pdu_c
e_cid_measurement_initiation_procedure::create_e_cid_measurement_initiation_response(bool on_demand)
{
  asn1::nrppa::nr_ppa_pdu_c asn1_resp;

  asn1_resp.set_successful_outcome().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_INITIATION);
  asn1_resp.successful_outcome().nrppatransaction_id = transaction_id;

  asn1::nrppa::e_c_id_meas_initiation_resp_s& meas_init_resp =
      asn1_resp.successful_outcome().value.e_c_id_meas_initiation_resp();

  meas_init_resp->lmf_ue_meas_id = to_underlying(ue_ids.lmf_ue_meas_id);
  meas_init_resp->ran_ue_meas_id = to_underlying(ue_ids.ran_ue_meas_id);

  if (on_demand) {
    meas_init_resp->e_c_id_meas_result_present = true;
    meas_init_resp->e_c_id_meas_result         = e_cid_meas_result_to_asn1(e_cid_meas_results.value());
  }

  return asn1_resp;
}

static std::chrono::milliseconds get_meas_period_ms(const nrppa_e_cid_meas_initiation_request& request)
{
  std::chrono::milliseconds meas_period_ms;

  // Convert measurement periodicity to milliseconds.
  if (request.meas_periodicity.value() == meas_periodicity_t::min1 ||
      request.meas_periodicity.value() == meas_periodicity_t::min6 ||
      request.meas_periodicity.value() == meas_periodicity_t::min12 ||
      request.meas_periodicity.value() == meas_periodicity_t::min30 ||
      request.meas_periodicity.value() == meas_periodicity_t::min60) {
    meas_period_ms = std::chrono::milliseconds{static_cast<uint32_t>(request.meas_periodicity.value()) * 60000U};

  } else {
    meas_period_ms = std::chrono::milliseconds{static_cast<uint32_t>(request.meas_periodicity.value())};
  }

  return meas_period_ms;
}
