// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_positioning_information_exchange_procedure.h"
#include "../../f1ap_asn1_utils.h"
#include "asn1_helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/asn1_utils.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/ran/cause/f1ap_cause_converters.h"
#include "ocudu/ran/positioning/positioning_information_exchange.h"
#include "ocudu/ran/srs/srs_configuration.h"
#include "ocudu/support/enum_utils.h"
#include <variant>

using namespace ocudu;
using namespace ocudu::ocucp;
using namespace asn1::f1ap;

/// \brief Convert the positioning information request from common type to ASN.1.
/// \param[out] asn1_request The ASN.1 struct to store the result.
/// \param[in] request The common type positioning information request.
static void fill_asn1_positioning_information_request(asn1::f1ap::positioning_info_request_s&  asn1_request,
                                                      const positioning_information_request_t& request);

/// \brief Convert the positioning information response from ASN.1 to common type.
/// \param[in] asn1_response The ASN.1 type positioning information response.
/// \returns the common type struct.
static positioning_information_response_t
fill_positioning_information_response(const asn1::f1ap::positioning_info_resp_s& asn1_response);

/// \brief Convert the positioning information failure from ASN.1 to common type.
/// \param[in] asn1_fail The ASN.1 type positioning information failure.
/// \returns the common type struct.
static positioning_information_failure_t
fill_positioning_information_failure(const asn1::f1ap::positioning_info_fail_s& asn1_fail);

f1ap_positioning_information_exchange_procedure::f1ap_positioning_information_exchange_procedure(
    const f1ap_configuration&                f1ap_cfg_,
    const positioning_information_request_t& request_,
    f1ap_ue_context&                         ue_ctxt_,
    f1ap_message_notifier&                   f1ap_notif_,
    ocudulog::basic_logger&                  logger_) :
  f1ap_cfg(f1ap_cfg_), request(request_), ue_ctxt(ue_ctxt_), f1ap_notifier(f1ap_notif_), logger(logger_)
{
}

void f1ap_positioning_information_exchange_procedure::operator()(
    coro_context<async_task<expected<positioning_information_response_t, positioning_information_failure_t>>>& ctx)
{
  CORO_BEGIN(ctx);

  logger.debug("{}: Procedure started...", f1ap_ue_log_prefix{ue_ctxt.ue_ids, name()});

  // Subscribe to respective publisher to receive POSITIONING INFORMATION RESPONSE/FAILURE message.
  transaction_sink.subscribe_to(ue_ctxt.ev_mng.positioning_information_outcome, f1ap_cfg.proc_timeout);

  // Send command to DU.
  send_positioning_information_request();

  // Await CU response.
  CORO_AWAIT(transaction_sink);

  // Handle response from DU and return UE index
  CORO_RETURN(create_positioning_information_result());
}

void f1ap_positioning_information_exchange_procedure::send_positioning_information_request()
{
  ocudu_sanity_check(ue_ctxt.ue_ids.du_ue_f1ap_id && ue_ctxt.ue_ids.du_ue_f1ap_id != gnb_du_ue_f1ap_id_t::invalid,
                     "Invalid gNB-DU-UE-F1AP-Id");

  // Pack message into PDU
  f1ap_message f1ap_pos_info_request_msg;
  f1ap_pos_info_request_msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_POSITIONING_INFO_EXCHANGE);
  positioning_info_request_s& pos_info_req = f1ap_pos_info_request_msg.pdu.init_msg().value.positioning_info_request();
  pos_info_req->gnb_du_ue_f1ap_id          = to_underlying(*ue_ctxt.ue_ids.du_ue_f1ap_id);
  pos_info_req->gnb_cu_ue_f1ap_id          = to_underlying(ue_ctxt.ue_ids.cu_ue_f1ap_id);

  fill_asn1_positioning_information_request(pos_info_req, request);

  // send positioning information request message
  f1ap_notifier.on_new_message(f1ap_pos_info_request_msg);
}

expected<positioning_information_response_t, positioning_information_failure_t>
f1ap_positioning_information_exchange_procedure::create_positioning_information_result()
{
  expected<positioning_information_response_t, positioning_information_failure_t> res;

  auto logger_prefix = f1ap_ue_log_prefix{ue_ctxt.ue_ids, name()};

  if (transaction_sink.successful()) {
    const asn1::f1ap::positioning_info_resp_s& resp = transaction_sink.response();

    logger.info("{}: Procedure finished successfully", logger_prefix);

    res = fill_positioning_information_response(resp);

  } else if (transaction_sink.failed()) {
    const asn1::f1ap::positioning_info_fail_s& fail = transaction_sink.failure();

    logger.warning("{}: Procedure failed. Cause: {}", logger_prefix, get_cause_str(fail->cause));

    res = make_unexpected(fill_positioning_information_failure(fail));
  } else {
    logger.warning("{}: Procedure failed. Cause: Timeout reached for PositioningInformationResponse reception",
                   logger_prefix);

    res = make_unexpected(positioning_information_failure_t{f1ap_to_nrppa_cause(cause_misc_t::unspecified)});
  }

  return res;
}

static inline asn1::f1ap::ref_sig_c ref_sig_to_asn1(const spatial_relation_info_t::reference_signal& ref_sig)
{
  asn1::f1ap::ref_sig_c asn1_ref_sig;

  if (std::holds_alternative<nzp_csi_rs_res_id_t>(ref_sig)) {
    asn1_ref_sig.set_nzp_csi_rs() = std::get<nzp_csi_rs_res_id_t>(ref_sig);
  } else if (std::holds_alternative<srs_config::srs_res_id>(ref_sig)) {
    ssb_t              ssb      = std::get<ssb_t>(ref_sig);
    asn1::f1ap::ssb_s& asn1_ssb = asn1_ref_sig.set_ssb();
    asn1_ssb.pci_nr             = ssb.pci_nr;
    if (ssb.ssb_idx.has_value()) {
      asn1_ssb.ssb_idx_present = true;
      asn1_ssb.ssb_idx         = ssb.ssb_idx.value();
    }
  } else if (std::holds_alternative<srs_config::srs_res_id>(ref_sig)) {
    asn1_ref_sig.set_srs() = std::get<srs_config::srs_res_id>(ref_sig);
  } else if (std::holds_alternative<srs_config::srs_pos_res_id>(ref_sig)) {
    asn1_ref_sig.set_positioning_srs() = std::get<srs_config::srs_pos_res_id>(ref_sig);
  } else {
    dl_prs_t              dl_prs      = std::get<dl_prs_t>(ref_sig);
    asn1::f1ap::dl_prs_s& asn1_dl_prs = asn1_ref_sig.set_dl_prs();
    asn1_dl_prs.prsid                 = dl_prs.prs_id;
    asn1_dl_prs.dl_prs_res_set_id     = dl_prs.dl_prs_res_set_id;
    if (dl_prs.dl_prs_res_id.has_value()) {
      asn1_dl_prs.dl_prs_res_id_present = true;
      asn1_dl_prs.dl_prs_res_id         = dl_prs.dl_prs_res_id.value();
    }
  }

  return asn1_ref_sig;
}

static void fill_asn1_positioning_information_request(asn1::f1ap::positioning_info_request_s&  asn1_request,
                                                      const positioning_information_request_t& request)
{
  // Fill requested SRS TX characteristics.
  if (request.requested_srs_tx_characteristics.has_value()) {
    asn1_request->requested_srs_tx_characteristics_present = true;
    // Fill nof txs.
    if (request.requested_srs_tx_characteristics->nof_txs.has_value()) {
      asn1_request->requested_srs_tx_characteristics.nof_txs_present = true;
      asn1_request->requested_srs_tx_characteristics.nof_txs =
          request.requested_srs_tx_characteristics->nof_txs.value();
    }
    // Fill res type.
    if (request.requested_srs_tx_characteristics->res_type ==
        requested_srs_tx_characteristics_t::res_type_t::periodic) {
      asn1_request->requested_srs_tx_characteristics.res_type =
          asn1::f1ap::requested_srs_tx_characteristics_s::res_type_opts::options::periodic;
    } else if (request.requested_srs_tx_characteristics->res_type ==
               requested_srs_tx_characteristics_t::res_type_t::semi_persistent) {
      asn1_request->requested_srs_tx_characteristics.res_type =
          asn1::f1ap::requested_srs_tx_characteristics_s::res_type_opts::options::semi_persistent;
    } else {
      asn1_request->requested_srs_tx_characteristics.res_type =
          asn1::f1ap::requested_srs_tx_characteristics_s::res_type_opts::options::aperiodic;
    }
    // Fill BW SRS.
    if (std::holds_alternative<uint8_t>(request.requested_srs_tx_characteristics->bw)) {
      asn1::number_to_enum(asn1_request->requested_srs_tx_characteristics.bw_srs.set_fr1(),
                           std::get<uint8_t>(request.requested_srs_tx_characteristics->bw));
    } else {
      asn1::number_to_enum(asn1_request->requested_srs_tx_characteristics.bw_srs.set_fr2(),
                           std::get<uint16_t>(request.requested_srs_tx_characteristics->bw));
    }
    // Fill list of SRS res set.
    for (const auto& srs_res_set_item : request.requested_srs_tx_characteristics->list_of_srs_res_set) {
      asn1::f1ap::srs_res_set_item_s asn1_srs_res_set_item;
      // Fill nof SRS res per set.
      if (srs_res_set_item.nof_srs_res_per_set.has_value()) {
        asn1_srs_res_set_item.num_srs_resperset_present = true;
        asn1_srs_res_set_item.num_srs_resperset         = srs_res_set_item.nof_srs_res_per_set.value();
      }
      // Fill periodicity list.
      for (const auto& period : srs_res_set_item.periodicity_list) {
        asn1::f1ap::periodicity_list_item_s asn1_period;
        asn1_period.periodicity_srs = static_cast<asn1::f1ap::periodicity_srs_opts::options>((unsigned)period.value);
        asn1_srs_res_set_item.periodicity_list.push_back(asn1_period);
      }
      // Fill spatial relation info.
      if (srs_res_set_item.spatial_relation_info.has_value()) {
        asn1_srs_res_set_item.spatial_relation_info_present = true;
        for (const auto& ref_sig : srs_res_set_item.spatial_relation_info->reference_signals) {
          asn1::f1ap::spatial_relationfor_res_id_item_s asn1_spatial_relation_for_res_id_item;
          asn1_spatial_relation_for_res_id_item.ref_sig = ref_sig_to_asn1(ref_sig);
          asn1_srs_res_set_item.spatial_relation_info.spatial_relationfor_res_id.push_back(
              asn1_spatial_relation_for_res_id_item);
        }
      }
      // Fill pathloss ref info.
      if (srs_res_set_item.pathloss_ref_info.has_value()) {
        asn1_srs_res_set_item.pathloss_ref_info_present = true;
        if (std::holds_alternative<ssb_t>(srs_res_set_item.pathloss_ref_info->pathloss_ref_sig)) {
          ssb_t              ssb      = std::get<ssb_t>(srs_res_set_item.pathloss_ref_info->pathloss_ref_sig);
          asn1::f1ap::ssb_s& asn1_ssb = asn1_srs_res_set_item.pathloss_ref_info.pathloss_ref_sig.set_ssb();
          asn1_ssb.pci_nr             = ssb.pci_nr;
          if (ssb.ssb_idx.has_value()) {
            asn1_ssb.ssb_idx_present = true;
            asn1_ssb.ssb_idx         = ssb.ssb_idx.value();
          }
        } else {
          dl_prs_t              dl_prs      = std::get<dl_prs_t>(srs_res_set_item.pathloss_ref_info->pathloss_ref_sig);
          asn1::f1ap::dl_prs_s& asn1_dl_prs = asn1_srs_res_set_item.pathloss_ref_info.pathloss_ref_sig.set_dl_prs();
          asn1_dl_prs.prsid                 = dl_prs.prs_id;
          asn1_dl_prs.dl_prs_res_set_id     = dl_prs.dl_prs_res_set_id;
          if (dl_prs.dl_prs_res_id.has_value()) {
            asn1_dl_prs.dl_prs_res_id_present = true;
            asn1_dl_prs.dl_prs_res_id         = dl_prs.dl_prs_res_id.value();
          }
        }
      }
      // Fill spatial relation per SRS res list.
      for (const auto& spatial_relation_per_srs_res : srs_res_set_item.spatial_relation_per_srs_res_list) {
        asn1_srs_res_set_item.ie_exts_present                                  = true;
        asn1_srs_res_set_item.ie_exts.srs_spatial_relation_per_srs_res_present = true;
        for (const auto& ref_sig : spatial_relation_per_srs_res.reference_signals) {
          asn1::f1ap::spatial_relation_per_srs_res_item_s asn1_spatial_relation_per_srs_res_item;
          asn1_spatial_relation_per_srs_res_item.ref_sig = ref_sig_to_asn1(ref_sig);
          asn1_srs_res_set_item.ie_exts.srs_spatial_relation_per_srs_res.spatial_relation_per_srs_res_list.push_back(
              asn1_spatial_relation_per_srs_res_item);
        }
      }

      asn1_request->requested_srs_tx_characteristics.srs_res_set_list.push_back(asn1_srs_res_set_item);
    }
    // Fill SSB info.
    if (request.requested_srs_tx_characteristics->ssb_info.has_value()) {
      asn1_request->requested_srs_tx_characteristics.ssb_info_present = true;
      for (const auto& ssb_info_item : request.requested_srs_tx_characteristics->ssb_info->list_of_ssb_info) {
        asn1::f1ap::ssb_info_item_s asn1_ssb_info_item;
        // Fill SSB config.
        asn1_ssb_info_item.ssb_cfg.ssb_freq = ssb_info_item.ssb_cfg.ssb_freq;
        asn1::number_to_enum(asn1_ssb_info_item.ssb_cfg.ssb_subcarrier_spacing,
                             scs_to_khz(ssb_info_item.ssb_cfg.ssb_subcarrier_spacing));
        asn1_ssb_info_item.ssb_cfg.ssb_tx_pwr = ssb_info_item.ssb_cfg.ssb_tx_pwr;
        asn1::number_to_enum(asn1_ssb_info_item.ssb_cfg.ssb_periodicity,
                             to_underlying(ssb_info_item.ssb_cfg.ssb_period));
        asn1_ssb_info_item.ssb_cfg.ssb_half_frame_offset = ssb_info_item.ssb_cfg.ssb_half_frame_offset;
        asn1_ssb_info_item.ssb_cfg.ssb_sfn_offset        = ssb_info_item.ssb_cfg.ssb_sfn_offset;
        if (ssb_info_item.ssb_cfg.ssb_burst_position.has_value()) {
          asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst_present = true;
          asn1::f1ap::ssb_positions_in_burst_c asn1_ssb_positions_in_burst;
          if (ssb_info_item.ssb_cfg.ssb_burst_position->type == ssb_burst_position_t::bitmap_type_t::short_bitmap) {
            asn1_ssb_positions_in_burst.set_short_bitmap().from_number(
                ssb_info_item.ssb_cfg.ssb_burst_position->bitmap);
          } else if (ssb_info_item.ssb_cfg.ssb_burst_position->type ==
                     ssb_burst_position_t::bitmap_type_t::medium_bitmap) {
            asn1_ssb_positions_in_burst.set_medium_bitmap().from_number(
                ssb_info_item.ssb_cfg.ssb_burst_position->bitmap);
          } else {
            asn1_ssb_positions_in_burst.set_long_bitmap().from_number(ssb_info_item.ssb_cfg.ssb_burst_position->bitmap);
          }
          asn1_ssb_info_item.ssb_cfg.ssb_position_in_burst = asn1_ssb_positions_in_burst;
        }
        if (ssb_info_item.ssb_cfg.sfn_initialization_time.has_value()) {
          asn1_ssb_info_item.ssb_cfg.sfn_initisation_time_present = true;
          asn1_ssb_info_item.ssb_cfg.sfn_initisation_time.from_number(
              ssb_info_item.ssb_cfg.sfn_initialization_time.value());
        }

        // Fill PCI NR.
        asn1_ssb_info_item.pci_nr = ssb_info_item.pci_nr;

        asn1_request->requested_srs_tx_characteristics.ssb_info.ssb_info_list.push_back(asn1_ssb_info_item);
      }
    }
    // Fill SRS freqs.
    for (const auto& srs_freq : request.requested_srs_tx_characteristics->srs_freqs) {
      asn1_request->requested_srs_tx_characteristics.ie_exts_present          = true;
      asn1_request->requested_srs_tx_characteristics.ie_exts.srs_freq_present = true;
      asn1_request->requested_srs_tx_characteristics.ie_exts.srs_freq         = srs_freq;
    }
  }

  // Fill UE report info.
  if (request.ue_report_info.has_value()) {
    asn1_request->ue_report_info_present = true;
    asn1::number_to_enum(asn1_request->ue_report_info.report_amount, request.ue_report_info->report_amount);
    if (request.ue_report_info->report_interv == 0) {
      asn1_request->ue_report_info.report_interv = asn1::f1ap::ue_report_info_s::report_interv_opts::options::none;
    } else if (request.ue_report_info->report_interv == 1) {
      asn1_request->ue_report_info.report_interv = asn1::f1ap::ue_report_info_s::report_interv_opts::options::one;
    } else if (request.ue_report_info->report_interv == 2) {
      asn1_request->ue_report_info.report_interv = asn1::f1ap::ue_report_info_s::report_interv_opts::options::two;
    } else if (request.ue_report_info->report_interv == 4) {
      asn1_request->ue_report_info.report_interv = asn1::f1ap::ue_report_info_s::report_interv_opts::options::four;
    } else if (request.ue_report_info->report_interv == 8) {
      asn1_request->ue_report_info.report_interv = asn1::f1ap::ue_report_info_s::report_interv_opts::options::eight;
    } else if (request.ue_report_info->report_interv == 10) {
      asn1_request->ue_report_info.report_interv = asn1::f1ap::ue_report_info_s::report_interv_opts::options::ten;
    } else if (request.ue_report_info->report_interv == 16) {
      asn1_request->ue_report_info.report_interv = asn1::f1ap::ue_report_info_s::report_interv_opts::options::sixteen;
    } else if (request.ue_report_info->report_interv == 32) {
      asn1_request->ue_report_info.report_interv =
          asn1::f1ap::ue_report_info_s::report_interv_opts::options::thirty_two;
    } else {
      asn1_request->ue_report_info.report_interv =
          asn1::f1ap::ue_report_info_s::report_interv_opts::options::sixty_four;
    }
  }
}

static positioning_information_response_t
fill_positioning_information_response(const asn1::f1ap::positioning_info_resp_s& asn1_resp)
{
  positioning_information_response_t resp;

  // Fill SRS configuration.
  if (asn1_resp->srs_cfg_present) {
    srs_configuration_t srs_cfg;
    // Fill SRS carrier list.
    for (const auto& asn1_srs_carrier_list_item : asn1_resp->srs_cfg.srs_carrier_list) {
      srs_carrier_list_item_t srs_carrier_list_item;

      // Fill point A.
      srs_carrier_list_item.point_a = asn1_srs_carrier_list_item.point_a;

      // Fill UL CH BW per SCS list.
      for (const auto& asn1_carrier : asn1_srs_carrier_list_item.ul_ch_bw_per_scs_list) {
        scs_specific_carrier carrier;
        carrier.offset_to_carrier = asn1_carrier.offset_to_carrier;
        carrier.scs               = khz_to_scs(asn1_carrier.subcarrier_spacing.to_number());
        carrier.carrier_bandwidth = asn1_carrier.carrier_bw;
        srs_carrier_list_item.ul_ch_bw_per_scs_list.push_back(carrier);
      }

      // Fill active UL BWP.
      srs_carrier_list_item.active_ul_bwp.location_and_bw = asn1_srs_carrier_list_item.active_ul_bwp.location_and_bw;
      srs_carrier_list_item.active_ul_bwp.scs =
          khz_to_scs(asn1_srs_carrier_list_item.active_ul_bwp.subcarrier_spacing.to_number());
      if (asn1_srs_carrier_list_item.active_ul_bwp.cp.value == asn1::f1ap::active_ul_bwp_s::cp_opts::options::normal) {
        srs_carrier_list_item.active_ul_bwp.cp = cyclic_prefix::options::NORMAL;
      } else {
        srs_carrier_list_item.active_ul_bwp.cp = cyclic_prefix::options::EXTENDED;
      }
      srs_carrier_list_item.active_ul_bwp.tx_direct_current_location =
          asn1_srs_carrier_list_item.active_ul_bwp.tx_direct_current_location;
      if (asn1_srs_carrier_list_item.active_ul_bwp.shift7dot5k_hz_present) {
        srs_carrier_list_item.active_ul_bwp.shift7dot5k_hz = asn1_srs_carrier_list_item.active_ul_bwp.shift7dot5k_hz;
      }
      // > Fill SRS configuration.
      // >> Fill SRS res list.
      for (const auto& asn1_srs_res : asn1_srs_carrier_list_item.active_ul_bwp.srs_cfg.srs_res_list) {
        ocudu::srs_config::srs_resource srs_res;
        srs_res.id.ue_res_id = (ocudu::srs_config::srs_res_id)asn1_srs_res.srs_res_id;
        srs_res.nof_ports    = (ocudu::srs_config::srs_resource::nof_srs_ports)asn1_srs_res.nrof_srs_ports.to_number();

        if (asn1_srs_res.tx_comb.type() == asn1::f1ap::tx_comb_c::types_opts::options::n2) {
          srs_res.tx_comb.size                 = tx_comb_size::n2;
          srs_res.tx_comb.tx_comb_offset       = asn1_srs_res.tx_comb.n2().comb_offset_n2;
          srs_res.tx_comb.tx_comb_cyclic_shift = asn1_srs_res.tx_comb.n2().cyclic_shift_n2;
        } else {
          srs_res.tx_comb.size                 = tx_comb_size::n4;
          srs_res.tx_comb.tx_comb_offset       = asn1_srs_res.tx_comb.n4().comb_offset_n4;
          srs_res.tx_comb.tx_comb_cyclic_shift = asn1_srs_res.tx_comb.n4().cyclic_shift_n4;
        }
        srs_res.res_mapping.start_pos   = asn1_srs_res.start_position;
        srs_res.res_mapping.nof_symb    = srs_nof_symbols(asn1_srs_res.nrof_symbols.to_number());
        srs_res.res_mapping.rept_factor = srs_nof_symbols(asn1_srs_res.repeat_factor.to_number());
        srs_res.freq_domain_pos         = asn1_srs_res.freq_domain_position;
        srs_res.freq_domain_shift       = asn1_srs_res.freq_domain_shift;
        srs_res.freq_hop.c_srs          = asn1_srs_res.c_srs;
        srs_res.freq_hop.b_srs          = asn1_srs_res.b_srs;
        srs_res.freq_hop.b_hop          = asn1_srs_res.b_hop;
        if (asn1_srs_res.group_or_seq_hop == asn1::f1ap::srs_res_s::group_or_seq_hop_opts::options::neither) {
          srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::neither;
        } else if (asn1_srs_res.group_or_seq_hop == asn1::f1ap::srs_res_s::group_or_seq_hop_opts::options::group_hop) {
          srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::group_hopping;
        } else {
          srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::sequence_hopping;
        }
        if (asn1_srs_res.res_type.type() == asn1::f1ap::res_type_c::types_opts::options::periodic) {
          srs_res.res_type = srs_resource_type::periodic;
          srs_res.periodicity_and_offset.emplace();
          srs_res.periodicity_and_offset->period =
              srs_periodicity(asn1_srs_res.res_type.periodic().periodicity.to_number());
          srs_res.periodicity_and_offset->offset = asn1_srs_res.res_type.periodic().offset;
        } else if (asn1_srs_res.res_type.type() == asn1::f1ap::res_type_c::types_opts::options::semi_persistent) {
          srs_res.res_type = srs_resource_type::semi_persistent;
          srs_res.periodicity_and_offset.emplace();
          srs_res.periodicity_and_offset->period =
              srs_periodicity(asn1_srs_res.res_type.semi_persistent().periodicity.to_number());
          srs_res.periodicity_and_offset->offset = asn1_srs_res.res_type.semi_persistent().offset;
        } else {
          srs_res.res_type = srs_resource_type::aperiodic;
        }
        srs_res.sequence_id = asn1_srs_res.seq_id;

        srs_carrier_list_item.active_ul_bwp.srs_cfg.srs_res_list.push_back(srs_res);
      }
      // >> Fill pos SRS res list.
      for (const auto& asn1_pos_srs_res : asn1_srs_carrier_list_item.active_ul_bwp.srs_cfg.pos_srs_res_list) {
        srs_config::srs_pos_resource pos_srs_res;
        pos_srs_res.id.ue_res_id = srs_config::srs_res_id(asn1_pos_srs_res.srs_pos_res_id);
        if (asn1_pos_srs_res.tx_comb_pos.type() == asn1::f1ap::tx_comb_pos_c::types_opts::options::n2) {
          pos_srs_res.tx_comb.size                     = tx_comb_pos_size::n2;
          pos_srs_res.tx_comb.tx_comb_pos_offset       = asn1_pos_srs_res.tx_comb_pos.n2().comb_offset_n2;
          pos_srs_res.tx_comb.tx_comb_pos_cyclic_shift = asn1_pos_srs_res.tx_comb_pos.n2().cyclic_shift_n2;
        } else if (asn1_pos_srs_res.tx_comb_pos.type() == asn1::f1ap::tx_comb_pos_c::types_opts::options::n4) {
          pos_srs_res.tx_comb.size                     = tx_comb_pos_size::n4;
          pos_srs_res.tx_comb.tx_comb_pos_offset       = asn1_pos_srs_res.tx_comb_pos.n4().comb_offset_n4;
          pos_srs_res.tx_comb.tx_comb_pos_cyclic_shift = asn1_pos_srs_res.tx_comb_pos.n4().cyclic_shift_n4;
        } else {
          pos_srs_res.tx_comb.size                     = tx_comb_pos_size::n8;
          pos_srs_res.tx_comb.tx_comb_pos_offset       = asn1_pos_srs_res.tx_comb_pos.n8().comb_offset_n8;
          pos_srs_res.tx_comb.tx_comb_pos_cyclic_shift = asn1_pos_srs_res.tx_comb_pos.n8().cyclic_shift_n8;
        }
        pos_srs_res.res_mapping.start_pos = asn1_pos_srs_res.start_position;
        pos_srs_res.res_mapping.nof_symb  = srs_nof_symbols(asn1_pos_srs_res.nrof_symbols.to_number());
        pos_srs_res.freq_domain_shift     = asn1_pos_srs_res.freq_domain_shift;
        pos_srs_res.c_srs                 = asn1_pos_srs_res.c_srs;
        if (asn1_pos_srs_res.group_or_seq_hop ==
            asn1::f1ap::pos_srs_res_item_s::group_or_seq_hop_opts::options::neither) {
          pos_srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::neither;
        } else if (asn1_pos_srs_res.group_or_seq_hop ==
                   asn1::f1ap::pos_srs_res_item_s::group_or_seq_hop_opts::options::group_hop) {
          pos_srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::group_hopping;
        } else {
          pos_srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::sequence_hopping;
        }
        if (asn1_pos_srs_res.res_type_pos.type() == asn1::f1ap::res_type_pos_c::types_opts::options::periodic) {
          pos_srs_res.res_type = srs_resource_type::periodic;
          pos_srs_res.periodicity_and_offset.emplace();
          pos_srs_res.periodicity_and_offset->period =
              srs_periodicity(asn1_pos_srs_res.res_type_pos.periodic().periodicity.to_number());
          pos_srs_res.periodicity_and_offset->offset = asn1_pos_srs_res.res_type_pos.periodic().offset;
        } else if (asn1_pos_srs_res.res_type_pos.type() ==
                   asn1::f1ap::res_type_pos_c::types_opts::options::semi_persistent) {
          pos_srs_res.res_type = srs_resource_type::semi_persistent;
          pos_srs_res.periodicity_and_offset.emplace();
          pos_srs_res.periodicity_and_offset->period =
              srs_periodicity(asn1_pos_srs_res.res_type_pos.semi_persistent().periodicity.to_number());
          pos_srs_res.periodicity_and_offset->offset = asn1_pos_srs_res.res_type_pos.semi_persistent().offset;
        } else {
          pos_srs_res.res_type    = srs_resource_type::aperiodic;
          pos_srs_res.slot_offset = asn1_pos_srs_res.res_type_pos.aperiodic().slot_offset;
        }
        pos_srs_res.sequence_id = asn1_pos_srs_res.seq_id;

        if (asn1_pos_srs_res.spatial_relation_pos_present) {
          if (asn1_pos_srs_res.spatial_relation_pos.type() ==
              asn1::f1ap::spatial_relation_pos_c::types_opts::options::ssb_pos) {
            srs_config::srs_pos_resource::srs_spatial_relation_pos::ssb ssb;
            ssb.pci_nr = asn1_pos_srs_res.spatial_relation_pos.ssb_pos().pci_nr;
            if (asn1_pos_srs_res.spatial_relation_pos.ssb_pos().ssb_idx_present) {
              ssb.ssb_idx = asn1_pos_srs_res.spatial_relation_pos.ssb_pos().ssb_idx;
            }
            pos_srs_res.spatial_relation_info->reference_signal = ssb;
          } else {
            srs_config::srs_pos_resource::srs_spatial_relation_pos::prs prs;
            prs.id             = asn1_pos_srs_res.spatial_relation_pos.pr_si_nformation_pos().prs_id_pos;
            prs.prs_res_set_id = asn1_pos_srs_res.spatial_relation_pos.pr_si_nformation_pos().prs_res_set_id_pos;
            if (asn1_pos_srs_res.spatial_relation_pos.pr_si_nformation_pos().prs_res_id_pos_present) {
              prs.prs_res_id = asn1_pos_srs_res.spatial_relation_pos.pr_si_nformation_pos().prs_res_id_pos;
            }
            pos_srs_res.spatial_relation_info->reference_signal = prs;
          }
        }

        srs_carrier_list_item.active_ul_bwp.srs_cfg.pos_srs_res_list.push_back(pos_srs_res);
      }

      // >> Fill SRS res set list.
      for (const auto& asn1_srs_res_set : asn1_srs_carrier_list_item.active_ul_bwp.srs_cfg.srs_res_set_list) {
        srs_config::srs_resource_set srs_res_set;
        srs_res_set.id = srs_config::srs_res_set_id(asn1_srs_res_set.srs_res_set_id);
        for (const auto& asn1_res_id : asn1_srs_res_set.srs_res_id_list) {
          srs_res_set.srs_res_id_list.push_back(srs_config::srs_res_id(asn1_res_id));
        }
        if (asn1_srs_res_set.res_set_type.type() == asn1::f1ap::res_set_type_c::types_opts::options::periodic) {
          srs_res_set.res_type = srs_config::srs_resource_set::periodic_resource_type{};
        } else if (asn1_srs_res_set.res_set_type.type() ==
                   asn1::f1ap::res_set_type_c::types_opts::options::semi_persistent) {
          srs_res_set.res_type = srs_config::srs_resource_set::semi_persistent_resource_type{};
        } else {
          srs_config::srs_resource_set::aperiodic_resource_type aperiodic;
          aperiodic.aperiodic_srs_res_trigger = asn1_srs_res_set.res_set_type.aperiodic().srs_res_trigger_list;
          aperiodic.slot_offset               = asn1_srs_res_set.res_set_type.aperiodic().slotoffset;
          srs_res_set.res_type                = aperiodic;
        }
        srs_carrier_list_item.active_ul_bwp.srs_cfg.srs_res_set_list.push_back(srs_res_set);
      }

      // >> Fill pos SRS res set list.
      for (const auto& asn1_pos_srs_res_set : asn1_srs_carrier_list_item.active_ul_bwp.srs_cfg.pos_srs_res_set_list) {
        srs_config::srs_resource_set pos_srs_res_set;
        pos_srs_res_set.id = srs_config::srs_res_set_id(asn1_pos_srs_res_set.possrs_res_set_id);
        for (const auto& asn1_pos_srs_res_id : asn1_pos_srs_res_set.poss_rs_res_id_list) {
          pos_srs_res_set.srs_res_id_list.push_back(srs_config::srs_res_id(asn1_pos_srs_res_id));
        }
        if (asn1_pos_srs_res_set.posres_set_type.type() ==
            asn1::f1ap::pos_res_set_type_c::types_opts::options::periodic) {
          pos_srs_res_set.res_type = srs_config::srs_resource_set::periodic_resource_type{};
        } else if (asn1_pos_srs_res_set.posres_set_type.type() ==
                   asn1::f1ap::pos_res_set_type_c::types_opts::options::semi_persistent) {
          pos_srs_res_set.res_type = srs_config::srs_resource_set::semi_persistent_resource_type{};
        } else {
          srs_config::srs_resource_set::aperiodic_resource_type aperiodic;
          aperiodic.aperiodic_srs_res_trigger = asn1_pos_srs_res_set.posres_set_type.aperiodic().srs_res_trigger_list;
          pos_srs_res_set.res_type            = aperiodic;
        }

        srs_carrier_list_item.active_ul_bwp.srs_cfg.pos_srs_res_set_list.push_back(pos_srs_res_set);
      }

      // >> Fill PCI.
      if (asn1_srs_carrier_list_item.pci_present) {
        srs_carrier_list_item.pci_nr = asn1_srs_carrier_list_item.pci;
      }

      srs_cfg.srs_carrier_list.push_back(srs_carrier_list_item);
    }

    resp.srs_cfg = srs_cfg;
  }

  // Fill SFN initialisation time.
  if (asn1_resp->sfn_initisation_time_present) {
    resp.sfn_initialization_time = asn1_resp->sfn_initisation_time.to_number();
  }

  // TODO: Fill criticality diagnostics.

  // TODO: Add missing optional values.

  return resp;
}

static positioning_information_failure_t
fill_positioning_information_failure(const asn1::f1ap::positioning_info_fail_s& asn1_fail)
{
  positioning_information_failure_t fail;
  fail.cause = f1ap_to_nrppa_cause(asn1_to_cause(asn1_fail->cause));
  if (asn1_fail->crit_diagnostics_present) {
    // TODO: Add crit diagnostics
  }

  return fail;
}
