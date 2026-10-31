// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_trp_information_exchange_procedure.h"
#include "asn1_helpers.h"
#include "f1ap_asn1_utils.h"
#include "f1ap_positioning_asn1_converters.h"
#include "ocudu/adt/expected.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/ran/cause/f1ap_cause.h"
#include "ocudu/ran/cause/f1ap_cause_converters.h"
#include "ocudu/ran/positioning/common.h"
#include "ocudu/ran/positioning/trp_information_exchange.h"
#include "ocudu/support/async/coroutine.h"

using namespace ocudu;
using namespace ocucp;

/// \brief Convert the TRP information request from common type to ASN.1.
/// \param[out] asn1_request The ASN.1 struct to store the result.
/// \param[in] request The common type TRP information request.
static void fill_asn1_trp_information_request(asn1::f1ap::trp_info_request_s&  asn1_request,
                                              const trp_information_request_t& request);

/// \brief Creates common type TRP info failure.
/// \param[in] cause The cause of the failure.
/// \return The TRP info failure PDU.
static trp_information_failure_t create_trp_info_failure(const f1ap_cause_t& cause);

/// \brief Creates common type TRP info response.
/// \param[in] asn1_resp The ASN.1 TRP info response.
/// \return The TRP info response PDU.
static trp_information_response_t create_trp_info_response(const asn1::f1ap::trp_info_resp_s& asn1_resp);

/// \brief Convert the DL PRS muting pattern from ASN.1 to common type.
static dl_prs_muting_pattern_t
asn1_to_dl_prs_muting_pattern(const asn1::f1ap::dl_prs_muting_pattern_c& asn1_dl_prs_muting_pattern);

// ---- TRP Information Procedure ----

f1ap_trp_information_exchange_procedure::f1ap_trp_information_exchange_procedure(
    const f1ap_configuration&        f1ap_cfg_,
    const trp_information_request_t& request_,
    f1ap_event_manager&              ev_mng_,
    f1ap_message_notifier&           f1ap_notif_,
    ocudulog::basic_logger&          logger_) :
  f1ap_cfg(f1ap_cfg_), request(request_), ev_mng(ev_mng_), f1ap_notifier(f1ap_notif_), logger(logger_)
{
}

void f1ap_trp_information_exchange_procedure::operator()(
    coro_context<async_task<expected<trp_information_response_t, trp_information_failure_t>>>& ctx)
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
  send_trp_information_request();

  // Await DU response.
  CORO_AWAIT(transaction);

  // Handle transaction response.
  CORO_RETURN(handle_procedure_outcome());
}

void f1ap_trp_information_exchange_procedure::send_trp_information_request()
{
  // Pack message into PDU.
  f1ap_message f1ap_trp_info_request_msg;
  f1ap_trp_info_request_msg.pdu.set_init_msg();
  f1ap_trp_info_request_msg.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_TRP_INFO_EXCHANGE);
  asn1::f1ap::trp_info_request_s& req = f1ap_trp_info_request_msg.pdu.init_msg().value.trp_info_request();

  req->transaction_id = transaction.id();

  // Convert common type to ASN.1.
  fill_asn1_trp_information_request(req, request);

  // Send TRP info request.
  f1ap_notifier.on_new_message(f1ap_trp_info_request_msg);
}

expected<trp_information_response_t, trp_information_failure_t>
f1ap_trp_information_exchange_procedure::handle_procedure_outcome()
{
  if (transaction.aborted()) {
    // Timeout or cancellation case.
    logger.warning("\"{}\" failed. Cause: Timeout or cancellation", name());
    return make_unexpected(create_trp_info_failure(f1ap_cause_radio_network_t::unspecified));
  }

  const f1ap_transaction_response& du_pdu_response = transaction.response();

  if (not du_pdu_response.has_value()) {
    procedure_outcome =
        make_unexpected(create_trp_info_failure(asn1_to_cause(du_pdu_response.error().value.trp_info_fail()->cause)));
    logger.warning(
        "\"{}\" failed. Cause: {}", name(), get_cause_str(du_pdu_response.error().value.trp_info_fail()->cause));
  } else {
    procedure_outcome = create_trp_info_response(du_pdu_response.value().value.trp_info_resp());
    logger.info("\"{}\" finished successfully", name());
  }

  // Send response to CU-CP.
  return procedure_outcome;
}

trp_information_failure_t create_trp_info_failure(const f1ap_cause_t& cause)
{
  trp_information_failure_t fail;
  fail.cause = f1ap_to_nrppa_cause(cause);

  return fail;
}

trp_information_response_t create_trp_info_response(const asn1::f1ap::trp_info_resp_s& asn1_resp)
{
  trp_information_response_t resp;

  // Fill TRP information list TRP response.
  for (const auto& asn1_trp_info_list_resp_item : asn1_resp->trp_info_list_trp_resp) {
    trp_information_list_trp_response_item_t trp_info_list_resp_item;

    // Fill TRP information.
    trp_info_list_resp_item.trp_info.trp_id =
        uint_to_trp_id(asn1_trp_info_list_resp_item->trp_info_item().trp_info.trp_id);
    for (const auto& asn1_trp_info_type_resp_item :
         asn1_trp_info_list_resp_item->trp_info_item().trp_info.trp_info_type_resp_list) {
      trp_information_type_response_item_t trp_info_type_resp_item;

      if (asn1_trp_info_type_resp_item.type().value ==
          asn1::f1ap::trp_info_type_resp_item_c::types_opts::options::pci_nr) {
        trp_info_type_resp_item = asn1_trp_info_type_resp_item.pci_nr();
      } else if (asn1_trp_info_type_resp_item.type().value ==
                 asn1::f1ap::trp_info_type_resp_item_c::types_opts::options::ng_ran_cgi) {
        nr_cell_global_id_t cgi = {};
        cgi.nci = nr_cell_identity::create(asn1_trp_info_type_resp_item.ng_ran_cgi().nr_cell_id.to_number()).value();
        cgi.plmn_id = plmn_identity::from_bytes(asn1_trp_info_type_resp_item.ng_ran_cgi().plmn_id.to_bytes()).value();
        trp_info_type_resp_item = cgi;
      } else if (asn1_trp_info_type_resp_item.type().value ==
                 asn1::f1ap::trp_info_type_resp_item_c::types_opts::options::nr_arfcn) {
        trp_info_type_resp_item = asn1_trp_info_type_resp_item.nr_arfcn();
      } else if (asn1_trp_info_type_resp_item.type().value ==
                 asn1::f1ap::trp_info_type_resp_item_c::types_opts::options::prs_cfg) {
        prs_cfg_t                    prs_cfg;
        const asn1::f1ap::prs_cfg_s& asn1_prs_cfg = asn1_trp_info_type_resp_item.prs_cfg();
        for (const auto& asn1_prs_res_set_item : asn1_prs_cfg.prs_res_set_list) {
          prs_resource_set_item_t prs_res_set_item;
          prs_res_set_item.prs_res_set_id = asn1_prs_res_set_item.prs_res_set_id;
          prs_res_set_item.scs            = khz_to_scs(asn1_prs_res_set_item.subcarrier_spacing.to_number());
          prs_res_set_item.prs_bw         = asn1_prs_res_set_item.pr_sbw;
          prs_res_set_item.start_prb      = asn1_prs_res_set_item.start_prb;
          prs_res_set_item.point_a        = asn1_prs_res_set_item.point_a;
          prs_res_set_item.comb_size      = asn1_prs_res_set_item.comb_size.to_number();
          if (asn1_prs_res_set_item.cp_type.value == asn1::f1ap::prs_res_set_item_s::cp_type_opts::options::normal) {
            prs_res_set_item.cp_type = cyclic_prefix::options::NORMAL;
          } else {
            prs_res_set_item.cp_type = cyclic_prefix::options::EXTENDED;
          }
          prs_res_set_item.res_set_periodicity = asn1_prs_res_set_item.res_set_periodicity.to_number();
          prs_res_set_item.res_set_slot_offset = asn1_prs_res_set_item.res_set_slot_offset;
          prs_res_set_item.res_repeat_factor   = asn1_prs_res_set_item.res_repeat_factor.to_number();
          prs_res_set_item.res_time_gap        = asn1_prs_res_set_item.res_time_gap.to_number();
          prs_res_set_item.res_numof_symbols   = asn1_prs_res_set_item.res_numof_symbols.to_number();

          if (asn1_prs_res_set_item.prs_muting_present) {
            prs_muting_t prs_muting;
            if (asn1_prs_res_set_item.prs_muting.prs_muting_option1_present) {
              prs_muting_option1_t prs_muting_option1;
              prs_muting_option1.muting_pattern =
                  asn1_to_dl_prs_muting_pattern(asn1_prs_res_set_item.prs_muting.prs_muting_option1.muting_pattern);
              prs_muting_option1.muting_bit_repeat_factor =
                  asn1_prs_res_set_item.prs_muting.prs_muting_option1.muting_bit_repeat_factor.to_number();
              prs_muting.prs_muting_option1 = prs_muting_option1;
            }

            if (asn1_prs_res_set_item.prs_muting.prs_muting_option2_present) {
              prs_muting_option2_t prs_muting_option2;
              prs_muting_option2.muting_pattern =
                  asn1_to_dl_prs_muting_pattern(asn1_prs_res_set_item.prs_muting.prs_muting_option2.muting_pattern);
              prs_muting.prs_muting_option2 = prs_muting_option2;
            }

            prs_res_set_item.prs_muting = prs_muting;
          }
          prs_res_set_item.prs_res_tx_pwr = asn1_prs_res_set_item.prs_res_tx_pwr;

          for (const auto& asn1_prs_res_item : asn1_prs_res_set_item.prs_res_list) {
            prs_res_item_t prs_res_item;
            prs_res_item.prs_res_id        = asn1_prs_res_item.prs_res_id;
            prs_res_item.seq_id            = asn1_prs_res_item.seq_id;
            prs_res_item.re_offset         = asn1_prs_res_item.re_offset;
            prs_res_item.res_slot_offset   = asn1_prs_res_item.res_slot_offset;
            prs_res_item.res_symbol_offset = asn1_prs_res_item.res_symbol_offset;
            if (asn1_prs_res_item.qcl_info_present) {
              prs_resource_qcl_info_t qcl_info;
              if (asn1_prs_res_item.qcl_info.type() ==
                  asn1::f1ap::prs_res_qcl_info_c::types_opts::options::qcl_source_ssb) {
                ssb_t qcl_info_ssb;
                qcl_info_ssb.pci_nr = asn1_prs_res_item.qcl_info.qcl_source_ssb().pci_nr;
                if (asn1_prs_res_item.qcl_info.qcl_source_ssb().ssb_idx_present) {
                  qcl_info_ssb.ssb_idx = asn1_prs_res_item.qcl_info.qcl_source_ssb().ssb_idx;
                }
                qcl_info = qcl_info_ssb;
              } else {
                prs_resource_qcl_source_prs_t qcl_info_prs;
                qcl_info_prs.qcl_source_prs_res_set_id =
                    asn1_prs_res_item.qcl_info.qcl_source_prs().qcl_source_prs_res_set_id;
                if (asn1_prs_res_item.qcl_info.qcl_source_prs().qcl_source_prs_res_id_present) {
                  qcl_info_prs.qcl_source_prs_res_id =
                      asn1_prs_res_item.qcl_info.qcl_source_prs().qcl_source_prs_res_id;
                }
                qcl_info = qcl_info_prs;
              }

              prs_res_item.qcl_info = qcl_info;
            }

            prs_res_set_item.prs_res_list.push_back(prs_res_item);
          }

          prs_cfg.prs_res_set_list.push_back(prs_res_set_item);
        }

        trp_info_type_resp_item = prs_cfg;
      } else if (asn1_trp_info_type_resp_item.type().value ==
                 asn1::f1ap::trp_info_type_resp_item_c::types_opts::options::ss_binfo) {
        const asn1::f1ap::ssb_info_s& asn1_ssb_info = asn1_trp_info_type_resp_item.ss_binfo();
        ssb_info_t                    ssb_info;

        for (const auto& asn1_ssb_info_item : asn1_ssb_info.ssb_info_list) {
          ssb_info_item_t ssb_info_item;
          // Fill SSB configuration.
          ssb_info_item.ssb_cfg.ssb_freq = asn1_ssb_info_item.ssb_cfg.ssb_freq;
          ssb_info_item.ssb_cfg.ssb_subcarrier_spacing =
              khz_to_scs(asn1_ssb_info_item.ssb_cfg.ssb_subcarrier_spacing.to_number());
          ssb_info_item.ssb_cfg.ssb_tx_pwr = asn1_ssb_info_item.ssb_cfg.ssb_tx_pwr;
          ssb_info_item.ssb_cfg.ssb_period = ssb_periodicity(asn1_ssb_info_item.ssb_cfg.ssb_periodicity.to_number());
          ssb_info_item.ssb_cfg.ssb_sfn_offset = asn1_ssb_info_item.ssb_cfg.ssb_sfn_offset;
          if (asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst_present) {
            ssb_burst_position_t burst_position;
            // short bitmap
            if (asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst.type() ==
                asn1::f1ap::ssb_positions_in_burst_c::types_opts::options::short_bitmap) {
              burst_position.type   = ssb_burst_position_t::bitmap_type_t::short_bitmap;
              burst_position.bitmap = asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst.short_bitmap().to_number();
            }
            // medium bitmap
            if (asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst.type() ==
                asn1::f1ap::ssb_positions_in_burst_c::types_opts::options::medium_bitmap) {
              burst_position.type   = ssb_burst_position_t::bitmap_type_t::medium_bitmap;
              burst_position.bitmap = asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst.medium_bitmap().to_number();
            }
            // long bitmap
            if (asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst.type() ==
                asn1::f1ap::ssb_positions_in_burst_c::types_opts::options::long_bitmap) {
              burst_position.type   = ssb_burst_position_t::bitmap_type_t::long_bitmap;
              burst_position.bitmap = asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst.long_bitmap().to_number();
            }
            ssb_info_item.ssb_cfg.ssb_burst_position = burst_position;
          }
          if (asn1_ssb_info_item.ssb_cfg.sfn_initisation_time_present) {
            ssb_info_item.ssb_cfg.sfn_initialization_time = asn1_ssb_info_item.ssb_cfg.sfn_initisation_time.to_number();
          }

          // Fill PCI.
          ssb_info_item.pci_nr = asn1_ssb_info_item.pci_nr;
          ssb_info.list_of_ssb_info.push_back(ssb_info_item);
        }

        trp_info_type_resp_item = ssb_info;
      } else if (asn1_trp_info_type_resp_item.type().value ==
                 asn1::f1ap::trp_info_type_resp_item_c::types_opts::options::sfn_initisation_time) {
        trp_info_type_resp_item = asn1_trp_info_type_resp_item.sfn_initisation_time().to_number();
      } else if (asn1_trp_info_type_resp_item.type().value ==
                 asn1::f1ap::trp_info_type_resp_item_c::types_opts::options::spatial_direction_info) {
        spatial_direction_info_t                    spatial_direction_info;
        const asn1::f1ap::spatial_direction_info_s& asn1_spatial_direction_info =
            asn1_trp_info_type_resp_item.spatial_direction_info();

        for (const auto& asn1_nr_prs_beam_info_item :
             asn1_spatial_direction_info.nr_prs_beam_info.nr_prs_beam_info_list) {
          nr_prs_beam_info_item_t nr_prs_beam_info_item;
          nr_prs_beam_info_item.pr_sres_set_id = asn1_nr_prs_beam_info_item.prs_res_set_id;
          for (const auto& asn1_prs_angle : asn1_nr_prs_beam_info_item.prs_angle_list) {
            prs_angle_item_t prs_angle_item;
            prs_angle_item.nr_prs_azimuth        = asn1_prs_angle.nr_prs_azimuth;
            prs_angle_item.nr_prs_azimuth_fine   = asn1_prs_angle.nr_prs_azimuth_fine;
            prs_angle_item.nr_prs_elevation      = asn1_prs_angle.nr_prs_elevation;
            prs_angle_item.nr_prs_elevation_fine = asn1_prs_angle.nr_prs_elevation_fine;
            if (asn1_prs_angle.ie_exts_present and asn1_prs_angle.ie_exts.prs_res_id_present) {
              prs_angle_item.prs_res_id = asn1_prs_angle.ie_exts.prs_res_id;
            }
            nr_prs_beam_info_item.prs_angle.push_back(prs_angle_item);
          }

          spatial_direction_info.nr_prs_beam_info.nr_prs_beam_info_list.push_back(nr_prs_beam_info_item);
        }

        for (const auto& asn1_lcs_to_gcs_translation_item :
             asn1_spatial_direction_info.nr_prs_beam_info.lc_sto_gcs_translation_list) {
          lcs_to_gcs_translation_item_t lcs_to_gcs_translation_item;
          lcs_to_gcs_translation_item.alpha = asn1_lcs_to_gcs_translation_item.alpha;
          if (asn1_lcs_to_gcs_translation_item.alpha_fine_present) {
            lcs_to_gcs_translation_item.alpha_fine = asn1_lcs_to_gcs_translation_item.alpha_fine;
          }
          lcs_to_gcs_translation_item.beta = asn1_lcs_to_gcs_translation_item.beta;
          if (asn1_lcs_to_gcs_translation_item.beta_fine_present) {
            lcs_to_gcs_translation_item.beta_fine = asn1_lcs_to_gcs_translation_item.beta_fine;
          }
          lcs_to_gcs_translation_item.gamma = asn1_lcs_to_gcs_translation_item.gamma;
          if (asn1_lcs_to_gcs_translation_item.gamma_fine_present) {
            lcs_to_gcs_translation_item.gamma_fine = asn1_lcs_to_gcs_translation_item.gamma_fine;
          }
          spatial_direction_info.nr_prs_beam_info.lcs_to_gcs_translation_list.push_back(lcs_to_gcs_translation_item);
        }

        trp_info_type_resp_item = spatial_direction_info;
      } else if (asn1_trp_info_type_resp_item.type().value ==
                 asn1::f1ap::trp_info_type_resp_item_c::types_opts::options::geographical_coordinates) {
        geographical_coordinates_t geo_coords =
            asn1_to_geographical_coordinates(asn1_trp_info_type_resp_item.geographical_coordinates());
        trp_info_type_resp_item = geo_coords;
      } else {
        const asn1::protocol_ie_single_container_s<asn1::f1ap::trp_info_type_resp_item_ext_ies_o>& asn1_ie_ext =
            asn1_trp_info_type_resp_item.choice_ext();

        if (asn1_ie_ext->type() ==
            asn1::f1ap::trp_info_type_resp_item_ext_ies_o::value_c::types_opts::options::trp_type) {
          trp_type_t                    trp_type;
          const asn1::f1ap::trp_type_e& asn1_trp_type = asn1_ie_ext->trp_type();
          if (asn1_trp_type == asn1::f1ap::trp_type_opts::options::prs_only_tp) {
            trp_type = trp_type_t::prs_only_tp;
          } else if (asn1_trp_type == asn1::f1ap::trp_type_opts::options::srs_only_rp) {
            trp_type = trp_type_t::srs_only_rp;
          } else if (asn1_trp_type == asn1::f1ap::trp_type_opts::options::tp) {
            trp_type = trp_type_t::tp;
          } else if (asn1_trp_type == asn1::f1ap::trp_type_opts::options::rp) {
            trp_type = trp_type_t::rp;
          } else {
            trp_type = trp_type_t::trp;
          }

          trp_info_type_resp_item = trp_type;
        } else if (asn1_ie_ext->type() ==
                   asn1::f1ap::trp_info_type_resp_item_ext_ies_o::value_c::types_opts::options::on_demand_prs) {
          on_demand_prs_info_t                    on_demand_prs_info;
          const asn1::f1ap::on_demand_prs_info_s& asn1_on_demand_prs_info = asn1_ie_ext->on_demand_prs();
          on_demand_prs_info.on_demand_prs_request_allowed =
              asn1_on_demand_prs_info.on_demand_prs_request_allowed.to_number();

          if (asn1_on_demand_prs_info.allowed_res_set_periodicity_values_present) {
            on_demand_prs_info.allowed_res_set_periodicity_values =
                asn1_on_demand_prs_info.allowed_res_set_periodicity_values.to_number();
          }
          if (asn1_on_demand_prs_info.allowed_prs_bw_values_present) {
            on_demand_prs_info.allowed_prs_bw_values = asn1_on_demand_prs_info.allowed_prs_bw_values.to_number();
          }
          if (asn1_on_demand_prs_info.allowed_res_repeat_factor_values_present) {
            on_demand_prs_info.allowed_res_repeat_factor_values =
                asn1_on_demand_prs_info.allowed_res_repeat_factor_values.to_number();
          }
          if (asn1_on_demand_prs_info.allowed_res_nof_symbols_values_present) {
            on_demand_prs_info.allowed_res_nof_symbols_values =
                asn1_on_demand_prs_info.allowed_res_nof_symbols_values.to_number();
          }
          if (asn1_on_demand_prs_info.allowed_comb_size_values_present) {
            on_demand_prs_info.allowed_comb_size_values = asn1_on_demand_prs_info.allowed_comb_size_values.to_number();
          }

          trp_info_type_resp_item = on_demand_prs_info;
        } else if (asn1_ie_ext->type() ==
                   asn1::f1ap::trp_info_type_resp_item_ext_ies_o::value_c::types_opts::options::trp_tx_teg_assoc) {
          std::vector<trpteg_item_t>            trp_tx_teg_assoc;
          const asn1::f1ap::trp_tx_teg_assoc_l& asn1_trp_tx_teg_assoc = asn1_ie_ext->trp_tx_teg_assoc();

          for (const auto& asn1_trpteg_item : asn1_trp_tx_teg_assoc) {
            trpteg_item_t trpteg_item;

            // Fill TRP TX TEG info.
            trpteg_item.trp_tx_teg_info.trp_tx_teg_id = asn1_trpteg_item.trp_tx_teg_info.trp_tx_teg_id;
            trpteg_item.trp_tx_teg_info.trp_tx_timing_error_margin =
                asn1_trpteg_item.trp_tx_teg_info.trp_tx_timing_error_margin;

            // Fill DL PRS res set ID.
            trpteg_item.dl_prs_res_set_id = asn1_trpteg_item.dl_prs_res_set_id;

            // Fill DL PRS res set ID list.
            for (const auto& asn1_dl_prs_res_id_item : asn1_trpteg_item.dl_prs_res_id_list) {
              dl_prs_res_id_item_t dl_prs_res_id_item;
              dl_prs_res_id_item.dl_prs_res_id = asn1_dl_prs_res_id_item.dl_prs_res_id;

              trpteg_item.dl_prs_res_id_list.push_back(dl_prs_res_id_item);
            }

            trp_tx_teg_assoc.push_back(trpteg_item);
          }

          trp_info_type_resp_item = trp_tx_teg_assoc;
        } else {
          trp_beam_ant_info_t                    trp_beam_ant_info;
          const asn1::f1ap::trp_beam_ant_info_s& asn1_trp_beam_ant_info = asn1_ie_ext->trp_beam_ant_info();

          if (asn1_trp_beam_ant_info.choice_trp_beam_ant_info_item.type() ==
              asn1::f1ap::choice_trp_beam_ant_info_item_c::types_opts::options::ref) {
            trp_beam_ant_info.choice_trp_beam_ant_info_item =
                asn1_trp_beam_ant_info.choice_trp_beam_ant_info_item.ref();
          } else {
            trp_beam_ant_explicit_info_t                    trp_beam_ant_explicit_info;
            const asn1::f1ap::trp_beam_ant_explicit_info_s& asn1_trp_beam_ant_explicit_info =
                asn1_trp_beam_ant_info.choice_trp_beam_ant_info_item.explicit_value();

            // Fill TRP beam ant angles.
            for (const auto& asn1_trp_beam_ant_angles_list_item : asn1_trp_beam_ant_explicit_info.trp_beam_ant_angles) {
              trp_beam_ant_angles_list_item_t trp_beam_ant_angles_list_item;

              trp_beam_ant_angles_list_item.trp_azimuth_angle = asn1_trp_beam_ant_angles_list_item.trp_azimuth_angle;
              if (asn1_trp_beam_ant_angles_list_item.trp_azimuth_angle_fine_present) {
                trp_beam_ant_angles_list_item.trp_azimuth_angle_fine =
                    asn1_trp_beam_ant_angles_list_item.trp_azimuth_angle_fine;
              }
              for (const auto& asn1_trp_elevation_angle_list_item :
                   asn1_trp_beam_ant_angles_list_item.trp_elevation_angle_list) {
                trp_elevation_angle_list_item_t trp_elevation_angle_list_item;
                trp_elevation_angle_list_item.trp_elevation_angle =
                    asn1_trp_elevation_angle_list_item.trp_elevation_angle;
                if (asn1_trp_elevation_angle_list_item.trp_elevation_angle_fine_present) {
                  trp_elevation_angle_list_item.trp_elevation_angle_fine =
                      asn1_trp_elevation_angle_list_item.trp_elevation_angle_fine;
                }
                for (const auto& asn1_trp_beam_pwr_item : asn1_trp_elevation_angle_list_item.trp_beam_pwr_list) {
                  trp_beam_pwr_item_t trp_beam_pwr_item;

                  if (asn1_trp_beam_pwr_item.prs_res_set_id_present) {
                    trp_beam_pwr_item.prs_res_set_id = asn1_trp_beam_pwr_item.prs_res_set_id;
                  }
                  trp_beam_pwr_item.prs_res_id   = asn1_trp_beam_pwr_item.prs_res_id;
                  trp_beam_pwr_item.relative_pwr = asn1_trp_beam_pwr_item.relative_pwr;
                  if (asn1_trp_beam_pwr_item.relative_pwr_fine_present) {
                    trp_beam_pwr_item.relative_pwr_fine = asn1_trp_beam_pwr_item.relative_pwr_fine;
                  }

                  trp_elevation_angle_list_item.trp_beam_pwr_list.push_back(trp_beam_pwr_item);
                }

                trp_beam_ant_angles_list_item.trp_elevation_angle_list.push_back(trp_elevation_angle_list_item);
              }

              trp_beam_ant_explicit_info.trp_beam_ant_angles.push_back(trp_beam_ant_angles_list_item);
            }

            // Fill LCS to GCS translation.
            if (asn1_trp_beam_ant_explicit_info.lcs_to_gcs_translation_present) {
              lcs_to_gcs_translation_t lcs_to_gcs_translation;
              lcs_to_gcs_translation.alpha = asn1_trp_beam_ant_explicit_info.lcs_to_gcs_translation.alpha;
              lcs_to_gcs_translation.beta  = asn1_trp_beam_ant_explicit_info.lcs_to_gcs_translation.beta;
              lcs_to_gcs_translation.gamma = asn1_trp_beam_ant_explicit_info.lcs_to_gcs_translation.gamma;

              trp_beam_ant_explicit_info.lcs_to_gcs_translation = lcs_to_gcs_translation;
            }

            trp_beam_ant_info.choice_trp_beam_ant_info_item = trp_beam_ant_explicit_info;
          }

          trp_info_type_resp_item = trp_beam_ant_info;
        }
      }

      trp_info_list_resp_item.trp_info.trp_info_type_resp_list.push_back(trp_info_type_resp_item);
    }

    resp.trp_info_list_trp_resp.push_back(trp_info_list_resp_item);
  }

  return resp;
}

static dl_prs_muting_pattern_t
asn1_to_dl_prs_muting_pattern(const asn1::f1ap::dl_prs_muting_pattern_c& asn1_dl_prs_muting_pattern)
{
  dl_prs_muting_pattern_t dl_prs_muting_pattern;
  if (asn1_dl_prs_muting_pattern.type() == asn1::f1ap::dl_prs_muting_pattern_c::types_opts::options::two) {
    dl_prs_muting_pattern.length = 2;
    dl_prs_muting_pattern.value  = asn1_dl_prs_muting_pattern.two().to_number();
  } else if (asn1_dl_prs_muting_pattern.type() == asn1::f1ap::dl_prs_muting_pattern_c::types_opts::options::four) {
    dl_prs_muting_pattern.length = 4;
    dl_prs_muting_pattern.value  = asn1_dl_prs_muting_pattern.four().to_number();
  } else if (asn1_dl_prs_muting_pattern.type() == asn1::f1ap::dl_prs_muting_pattern_c::types_opts::options::six) {
    dl_prs_muting_pattern.length = 6;
    dl_prs_muting_pattern.value  = asn1_dl_prs_muting_pattern.six().to_number();
  } else if (asn1_dl_prs_muting_pattern.type() == asn1::f1ap::dl_prs_muting_pattern_c::types_opts::options::eight) {
    dl_prs_muting_pattern.length = 8;
    dl_prs_muting_pattern.value  = asn1_dl_prs_muting_pattern.eight().to_number();
  } else if (asn1_dl_prs_muting_pattern.type() == asn1::f1ap::dl_prs_muting_pattern_c::types_opts::options::sixteen) {
    dl_prs_muting_pattern.length = 16;
    dl_prs_muting_pattern.value  = asn1_dl_prs_muting_pattern.sixteen().to_number();
  } else {
    dl_prs_muting_pattern.length = 32;
    dl_prs_muting_pattern.value  = asn1_dl_prs_muting_pattern.thirty_two().to_number();
  }

  return dl_prs_muting_pattern;
}

static void fill_asn1_trp_information_request(asn1::f1ap::trp_info_request_s&  asn1_request,
                                              const trp_information_request_t& request)
{
  // Fill TRP list.
  for (const auto& trp_id : request.trp_list) {
    asn1::f1ap::trp_list_item_s asn1_trp_list_item;
    asn1_trp_list_item.trp_id = to_underlying(trp_id);

    asn1_request->trp_list.push_back(asn1_trp_list_item);
  }

  // Fill TRP info type list TRP request.
  for (const auto& trp_info_type : request.trp_info_type_list_trp_req) {
    asn1::protocol_ie_single_container_s<asn1::f1ap::trp_info_type_item_trp_req_o> asn1_trp_info_type_container;
    asn1::f1ap::trp_info_type_item_e& asn1_trp_info_type = asn1_trp_info_type_container.value().trp_info_type_item();

    if (trp_info_type == trp_information_type_item_t::nr_pci) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::nr_pci;
    }

    if (trp_info_type == trp_information_type_item_t::ng_ran_cgi) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::ng_ran_cgi;
    }

    if (trp_info_type == trp_information_type_item_t::arfcn) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::arfcn;
    }

    if (trp_info_type == trp_information_type_item_t::prs_cfg) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::prs_cfg;
    }

    if (trp_info_type == trp_information_type_item_t::ssb_info) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::ssb_cfg;
    }

    if (trp_info_type == trp_information_type_item_t::sfn_init_time) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::sfn_init_time;
    }

    if (trp_info_type == trp_information_type_item_t::spatial_direct_info) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::spatial_direct_info;
    }

    if (trp_info_type == trp_information_type_item_t::geo_coord) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::geo_coord;
    }

    if (trp_info_type == trp_information_type_item_t::trp_type) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::trp_type;
    }

    if (trp_info_type == trp_information_type_item_t::ondemand_prs_info) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::ondemand_prs;
    }

    if (trp_info_type == trp_information_type_item_t::trp_tx_teg) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::trp_tx_teg;
    }

    if (trp_info_type == trp_information_type_item_t::beam_ant_info) {
      asn1_trp_info_type = asn1::f1ap::trp_info_type_item_opts::options::beam_ant_info;
    }

    asn1_request->trp_info_type_list_trp_req.push_back(asn1_trp_info_type_container);
  }
}
