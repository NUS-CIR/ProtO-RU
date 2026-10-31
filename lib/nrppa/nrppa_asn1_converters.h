// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/asn1/asn1_utils.h"
#include "ocudu/asn1/nrppa/nrppa_ies.h"
#include "ocudu/nrppa/nrppa_e_cid.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/nr_cell_identity.h"
#include "ocudu/ran/nr_cgi.h"
#include "ocudu/ran/positioning/common.h"
#include "ocudu/ran/positioning/measurement_information.h"
#include "ocudu/ran/positioning/positioning_information_exchange.h"
#include "ocudu/ran/positioning/trp_information_exchange.h"
#include "ocudu/ran/srs/srs_configuration.h"
#include "ocudu/support/enum_utils.h"
#include <variant>

namespace ocudu::ocucp {

// Helper to create PDU from NRPPA message
template <class T>
byte_buffer pack_into_pdu(const T& msg, const char* context_name = nullptr)
{
  context_name = context_name == nullptr ? __FUNCTION__ : context_name;
  byte_buffer   pdu{};
  asn1::bit_ref bref{pdu};
  if (msg.pack(bref) == asn1::OCUDUASN_ERROR_ENCODE_FAIL) {
    ocudulog::fetch_basic_logger("NRPPA").error("Failed to pack message in {} - discarding it", context_name);
    pdu.clear();
  }
  return pdu;
}

inline meas_periodicity_t asn1_to_meas_periodicity(asn1::nrppa::meas_periodicity_e asn1_meas_periodicity)
{
  if (asn1_meas_periodicity == asn1::nrppa::meas_periodicity_opts::nulltype) {
    report_fatal_error("Unsupported NRPPa meas periodicity. NRPPa meas periodicity={}",
                       fmt::underlying(asn1_meas_periodicity.value));
  }

  return static_cast<meas_periodicity_t>(asn1_meas_periodicity.to_number());
}

inline nrppa_meas_quantities_item
asn1_to_meas_quantities_item(const asn1::nrppa::meas_quantities_item_s& asn1_meas_quantities_item)
{
  nrppa_meas_quantities_item meas_quantities_item;

  switch (asn1_meas_quantities_item.meas_quantities_value) {
    case asn1::nrppa::meas_quantities_value_e::cell_id:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::cell_id;
      break;
    case asn1::nrppa::meas_quantities_value_e::angle_of_arrival:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::angle_of_arrival;
      break;
    case asn1::nrppa::meas_quantities_value_e::timing_advance_type1:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::timing_advance_type1;
      break;
    case asn1::nrppa::meas_quantities_value_e::timing_advance_type2:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::timing_advance_type2;
      break;
    case asn1::nrppa::meas_quantities_value_e::rsrp:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::rsrp;
      break;
    case asn1::nrppa::meas_quantities_value_e::rsrq:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::rsrq;
      break;
    case asn1::nrppa::meas_quantities_value_e::ss_rsrp:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::ss_rsrp;
      break;
    case asn1::nrppa::meas_quantities_value_e::ss_rsrq:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::ss_rsrq;
      break;
    case asn1::nrppa::meas_quantities_value_e::csi_rsrp:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::csi_rsrp;
      break;
    case asn1::nrppa::meas_quantities_value_e::csi_rsrq:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::csi_rsrq;
      break;
    case asn1::nrppa::meas_quantities_value_e::angle_of_arrival_nr:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::angle_of_arrival_nr;
      break;
    case asn1::nrppa::meas_quantities_value_e::timing_advance_nr:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::timing_advance_nr;
      break;
    case asn1::nrppa::meas_quantities_value_e::ue_rx_tx_time_diff:
      meas_quantities_item.meas_quantities_value = nrppa_meas_quantities_value::ue_rx_tx_time_diff;
      break;
    default:
      report_fatal_error("Unsupported NRPPa meas quantities value. NRPPa meas quantities value={}",
                         asn1_meas_quantities_item.meas_quantities_value.to_number());
  }

  return meas_quantities_item;
}

inline asn1::nrppa::meas_quantities_item_s
meas_quantities_item_to_asn1(const nrppa_meas_quantities_item& meas_quantities_item)
{
  asn1::nrppa::meas_quantities_item_s asn1_meas_quantities_item;

  switch (meas_quantities_item.meas_quantities_value) {
    case nrppa_meas_quantities_value::cell_id:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::cell_id;
      break;
    case nrppa_meas_quantities_value::angle_of_arrival:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::angle_of_arrival;
      break;
    case nrppa_meas_quantities_value::timing_advance_type1:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::timing_advance_type1;
      break;
    case nrppa_meas_quantities_value::timing_advance_type2:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::timing_advance_type2;
      break;
    case nrppa_meas_quantities_value::rsrp:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::rsrp;
      break;
    case nrppa_meas_quantities_value::rsrq:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::rsrq;
      break;
    case nrppa_meas_quantities_value::ss_rsrp:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::ss_rsrp;
      break;
    case nrppa_meas_quantities_value::ss_rsrq:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::ss_rsrq;
      break;
    case nrppa_meas_quantities_value::csi_rsrp:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::csi_rsrp;
      break;
    case nrppa_meas_quantities_value::csi_rsrq:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::csi_rsrq;
      break;
    case nrppa_meas_quantities_value::angle_of_arrival_nr:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::angle_of_arrival_nr;
      break;
    case nrppa_meas_quantities_value::timing_advance_nr:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::timing_advance_nr;
      break;
    case nrppa_meas_quantities_value::ue_rx_tx_time_diff:
      asn1_meas_quantities_item.meas_quantities_value = asn1::nrppa::meas_quantities_value_opts::ue_rx_tx_time_diff;
      break;
    default:
      report_fatal_error("Unsupported NRPPa meas quantities value");
  }

  return asn1_meas_quantities_item;
}

inline asn1::nrppa::ng_ran_access_point_position_s
ng_ran_access_point_position_to_asn1(const ng_ran_access_point_position_t& ng_ran_access_point_position)
{
  asn1::nrppa::ng_ran_access_point_position_s asn1_ng_ran_access_point_position;

  // Fill latitude sign.
  asn1_ng_ran_access_point_position.latitude_sign.value =
      static_cast<asn1::nrppa::ng_ran_access_point_position_s::latitude_sign_opts::options>(
          ng_ran_access_point_position.latitude_sign);

  // Fill latutide.
  asn1_ng_ran_access_point_position.latitude = ng_ran_access_point_position.latitude;

  // Fill longitude.
  asn1_ng_ran_access_point_position.longitude = ng_ran_access_point_position.longitude;

  // Direction of altitude.
  asn1_ng_ran_access_point_position.direction_of_altitude.value =
      static_cast<asn1::nrppa::ng_ran_access_point_position_s::direction_of_altitude_opts::options>(
          ng_ran_access_point_position.direction_of_altitude);

  // Fill altitude.
  asn1_ng_ran_access_point_position.altitude = ng_ran_access_point_position.altitude;

  // Fill uncertainty semi major.
  asn1_ng_ran_access_point_position.uncertainty_semi_major = ng_ran_access_point_position.uncertainty_semi_major;

  // Fill uncertainty semi minor.
  asn1_ng_ran_access_point_position.uncertainty_semi_minor = ng_ran_access_point_position.uncertainty_semi_minor;

  // Fill orientation of major axis.
  asn1_ng_ran_access_point_position.orientation_of_major_axis = ng_ran_access_point_position.orientation_of_major_axis;

  // Fill uncertainty altitude.
  asn1_ng_ran_access_point_position.uncertainty_altitude = ng_ran_access_point_position.uncertainty_altitude;

  // Fill confidence.
  asn1_ng_ran_access_point_position.confidence = ng_ran_access_point_position.confidence;

  return asn1_ng_ran_access_point_position;
}

inline asn1::nrppa::e_c_id_meas_result_s e_cid_meas_result_to_asn1(const nrppa_e_cid_meas_result& meas_result)
{
  asn1::nrppa::e_c_id_meas_result_s asn1_meas_result;

  // Serving cell id.
  asn1_meas_result.serving_cell_id.ng_ran_cell.set_nr_cell_id().from_number(meas_result.serving_cell_id.nci.value());
  asn1_meas_result.serving_cell_id.plmn_id = meas_result.serving_cell_id.plmn_id.to_bytes();

  // Serving cell tac.
  asn1_meas_result.serving_cell_tac.from_number(meas_result.serving_cell_tac);

  // NG-RAN access point position.
  if (meas_result.ng_ran_access_point_position.has_value()) {
    asn1_meas_result.ng_ran_access_point_position_present = true;
    asn1_meas_result.ng_ran_access_point_position =
        ng_ran_access_point_position_to_asn1(meas_result.ng_ran_access_point_position.value());
  }

  // Measured results.
  // Note that only NR results are supported.
  for (const auto& measured_results_value : meas_result.measured_results) {
    asn1::nrppa::measured_results_value_c asn1_measured_results_value;
    auto&                                 result_nr = asn1_measured_results_value.set_choice_ext();

    // SS RSRP.
    if (const auto* result = std::get_if<std::vector<nrppa_result_ss_rsrp_item>>(&measured_results_value)) {
      asn1::nrppa::result_ss_rsrp_l rsrp_list;

      for (const auto& result_item : *result) {
        asn1::nrppa::result_ss_rsrp_item_s rsrp_item;
        rsrp_item.nr_pci   = result_item.nr_pci;
        rsrp_item.nr_arfcn = result_item.nr_arfcn.value();

        if (result_item.cgi_nr.has_value()) {
          rsrp_item.cgi_nr_present = true;
          rsrp_item.cgi_nr.nr_cell_id.from_number(result_item.cgi_nr->nci.value());
          rsrp_item.cgi_nr.plmn_id = result_item.cgi_nr->plmn_id.to_bytes();
        }

        if (result_item.value_ss_rsrp_cell.has_value()) {
          rsrp_item.value_ss_rsrp_cell_present = true;
          rsrp_item.value_ss_rsrp_cell         = result_item.value_ss_rsrp_cell.value();
        }

        for (const auto& rsrp_per_ssb_item : result_item.ss_rsrp_per_ssb) {
          asn1::nrppa::result_ss_rsrp_per_ssb_item_s asn1_rsrp_per_ssb_item;
          asn1_rsrp_per_ssb_item.ssb_idx       = rsrp_per_ssb_item.ssb_idx;
          asn1_rsrp_per_ssb_item.value_ss_rsrp = rsrp_per_ssb_item.value_ss_rsrp;

          rsrp_item.ss_rsrp_per_ssb.push_back(asn1_rsrp_per_ssb_item);
        }

        rsrp_list.push_back(rsrp_item);
      }

      result_nr->set(asn1::nrppa::measured_results_value_ext_ie_o::value_c::types_opts::result_ss_rsrp);
      result_nr->result_ss_rsrp() = rsrp_list;
    }

    // SS RSRQ.
    if (const auto* result = std::get_if<std::vector<nrppa_result_ss_rsrq_item>>(&measured_results_value)) {
      asn1::nrppa::result_ss_rsrq_l rsrq_list;

      for (const auto& result_item : *result) {
        asn1::nrppa::result_ss_rsrq_item_s rsrq_item;
        rsrq_item.nr_pci   = result_item.nr_pci;
        rsrq_item.nr_arfcn = result_item.nr_arfcn.value();

        if (result_item.cgi_nr.has_value()) {
          rsrq_item.cgi_nr_present = true;
          rsrq_item.cgi_nr.nr_cell_id.from_number(result_item.cgi_nr->nci.value());
          rsrq_item.cgi_nr.plmn_id = result_item.cgi_nr->plmn_id.to_bytes();
        }

        if (result_item.value_ss_rsrq_cell.has_value()) {
          rsrq_item.value_ss_rsrq_cell_present = true;
          rsrq_item.value_ss_rsrq_cell         = result_item.value_ss_rsrq_cell.value();
        }

        for (const auto& rsrq_per_ssb_item : result_item.ss_rsrq_per_ssb) {
          asn1::nrppa::result_ss_rsrq_per_ssb_item_s asn1_rsrq_per_ssb_item;
          asn1_rsrq_per_ssb_item.ssb_idx       = rsrq_per_ssb_item.ssb_idx;
          asn1_rsrq_per_ssb_item.value_ss_rsrq = rsrq_per_ssb_item.value_ss_rsrq;

          rsrq_item.ss_rsrq_per_ssb.push_back(asn1_rsrq_per_ssb_item);
        }

        rsrq_list.push_back(rsrq_item);
      }

      result_nr->set(asn1::nrppa::measured_results_value_ext_ie_o::value_c::types_opts::result_ss_rsrq);
      result_nr->result_ss_rsrq() = rsrq_list;
    }

    // CSI RSRP.
    if (const auto* result = std::get_if<std::vector<nrppa_result_csi_rsrp_item>>(&measured_results_value)) {
      asn1::nrppa::result_csi_rsrp_l rsrp_list;

      for (const auto& result_item : *result) {
        asn1::nrppa::result_csi_rsrp_item_s rsrp_item;
        rsrp_item.nr_pci   = result_item.nr_pci;
        rsrp_item.nr_arfcn = result_item.nr_arfcn.value();

        if (result_item.cgi_nr.has_value()) {
          rsrp_item.cgi_nr_present = true;
          rsrp_item.cgi_nr.nr_cell_id.from_number(result_item.cgi_nr->nci.value());
          rsrp_item.cgi_nr.plmn_id = result_item.cgi_nr->plmn_id.to_bytes();
        }

        if (result_item.value_csi_rsrp_cell.has_value()) {
          rsrp_item.value_csi_rsrp_cell_present = true;
          rsrp_item.value_csi_rsrp_cell         = result_item.value_csi_rsrp_cell.value();
        }

        for (const auto& csi_rsrp_per_csi_rs_item : result_item.csi_rsrp_per_csi_rs) {
          asn1::nrppa::result_csi_rsrp_per_csi_rs_item_s asn1_csi_rsrp_per_csi_rs_item;
          asn1_csi_rsrp_per_csi_rs_item.csi_rs_idx     = csi_rsrp_per_csi_rs_item.csi_rs_idx;
          asn1_csi_rsrp_per_csi_rs_item.value_csi_rsrp = csi_rsrp_per_csi_rs_item.value_csi_rsrp;

          rsrp_item.csi_rsrp_per_csi_rs.push_back(asn1_csi_rsrp_per_csi_rs_item);
        }

        rsrp_list.push_back(rsrp_item);
      }

      result_nr->set(asn1::nrppa::measured_results_value_ext_ie_o::value_c::types_opts::result_csi_rsrp);
      result_nr->result_csi_rsrp() = rsrp_list;
    }

    // CSI RSRQ.
    if (const auto* result = std::get_if<std::vector<nrppa_result_csi_rsrq_item>>(&measured_results_value)) {
      asn1::nrppa::result_csi_rsrq_l rsrq_list;

      for (const auto& result_item : *result) {
        asn1::nrppa::result_csi_rsrq_item_s rsrq_item;
        rsrq_item.nr_pci   = result_item.nr_pci;
        rsrq_item.nr_arfcn = result_item.nr_arfcn.value();

        if (result_item.cgi_nr.has_value()) {
          rsrq_item.cgi_nr_present = true;
          rsrq_item.cgi_nr.nr_cell_id.from_number(result_item.cgi_nr->nci.value());
          rsrq_item.cgi_nr.plmn_id = result_item.cgi_nr->plmn_id.to_bytes();
        }

        if (result_item.value_csi_rsrq_cell.has_value()) {
          rsrq_item.value_csi_rsrq_cell_present = true;
          rsrq_item.value_csi_rsrq_cell         = result_item.value_csi_rsrq_cell.value();
        }

        for (const auto& csi_rsrq_per_csi_rs_item : result_item.csi_rsrq_per_csi_rs) {
          asn1::nrppa::result_csi_rsrq_per_csi_rs_item_s asn1_csi_rsrq_per_csi_rs_item;
          asn1_csi_rsrq_per_csi_rs_item.csi_rs_idx     = csi_rsrq_per_csi_rs_item.csi_rs_idx;
          asn1_csi_rsrq_per_csi_rs_item.value_csi_rsrq = csi_rsrq_per_csi_rs_item.value_csi_rsrq;

          rsrq_item.csi_rsrq_per_csi_rs.push_back(asn1_csi_rsrq_per_csi_rs_item);
        }

        rsrq_list.push_back(rsrq_item);
      }

      result_nr->set(asn1::nrppa::measured_results_value_ext_ie_o::value_c::types_opts::result_csi_rsrq);
      result_nr->result_csi_rsrq() = rsrq_list;
    }

    // Angle of arrival.
    if (const auto* result = std::get_if<nrppa_ul_aoa>(&measured_results_value)) {
      asn1::nrppa::ul_ao_a_s asn1_aoa;
      asn1_aoa.azimuth_ao_a = result->azimuth_aoa;

      if (result->zenith_aoa.has_value()) {
        asn1_aoa.zenith_ao_a_present = true;
        asn1_aoa.zenith_ao_a         = result->zenith_aoa.value();
      }

      if (result->lcs_to_gcs_translation.has_value()) {
        asn1_aoa.lcs_to_gcs_translation_present = true;
        asn1_aoa.lcs_to_gcs_translation.alpha   = result->lcs_to_gcs_translation->alpha;
        asn1_aoa.lcs_to_gcs_translation.beta    = result->lcs_to_gcs_translation->beta;
        asn1_aoa.lcs_to_gcs_translation.gamma   = result->lcs_to_gcs_translation->gamma;
      }

      result_nr->set(asn1::nrppa::measured_results_value_ext_ie_o::value_c::types_opts::angle_of_arrival_nr);
      result_nr->angle_of_arrival_nr() = asn1_aoa;
    }

    // NR Timing advance.
    if (const auto* result = std::get_if<nrppa_nr_tadv>(&measured_results_value)) {
      result_nr->set(asn1::nrppa::measured_results_value_ext_ie_o::value_c::types_opts::nr_tadv);
      result_nr->nr_tadv() = static_cast<uint16_t>(*result);
    }

    asn1_meas_result.measured_results.push_back(asn1_measured_results_value);
  }

  return asn1_meas_result;
}

/// \brief Convert \c nrppa_cause_t type to NRPPA ASN.1 cause.
/// \param cause The nrppa_cause_t type.
/// \return The NRPPA ASN.1 cause.
inline asn1::nrppa::cause_c cause_to_asn1(nrppa_cause_t cause)
{
  asn1::nrppa::cause_c asn1_cause;

  if (const auto* result = std::get_if<nrppa_cause_radio_network_t>(&cause)) {
    asn1_cause.set_radio_network() = static_cast<asn1::nrppa::cause_radio_network_opts::options>(*result);
    return asn1_cause;
  }
  if (const auto* result = std::get_if<nrppa_cause_protocol_t>(&cause)) {
    asn1_cause.set_protocol() = static_cast<asn1::nrppa::cause_protocol_opts::options>(*result);
    return asn1_cause;
  }
  if (const auto* result = std::get_if<nrppa_cause_misc_t>(&cause)) {
    asn1_cause.set_misc() = static_cast<asn1::nrppa::cause_misc_opts::options>(*result);
    return asn1_cause;
  }

  report_fatal_error("Cannot convert cause to NRPPA type");
  return asn1_cause;
}

inline std::optional<trp_information_type_item_t>
asn1_to_trp_info_type_item(const asn1::nrppa::trp_info_type_item_e& asn1_trp_info_type_item)
{
  trp_information_type_item_t trp_info_type_item;
  switch (asn1_trp_info_type_item) {
    case asn1::nrppa::trp_info_type_item_opts::options::nr_pci:
      trp_info_type_item = trp_information_type_item_t::nr_pci;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::ng_ran_cgi:
      trp_info_type_item = trp_information_type_item_t::ng_ran_cgi;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::arfcn:
      trp_info_type_item = trp_information_type_item_t::arfcn;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::prs_cfg:
      trp_info_type_item = trp_information_type_item_t::prs_cfg;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::ssb_info:
      trp_info_type_item = trp_information_type_item_t::ssb_info;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::sfn_init_time:
      trp_info_type_item = trp_information_type_item_t::sfn_init_time;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::spatial_direct_info:
      trp_info_type_item = trp_information_type_item_t::spatial_direct_info;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::geo_coord:
      trp_info_type_item = trp_information_type_item_t::geo_coord;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::trp_type:
      trp_info_type_item = trp_information_type_item_t::trp_type;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::ondemand_pr_si_nfo:
      trp_info_type_item = trp_information_type_item_t::ondemand_prs_info;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::trp_tx_teg:
      trp_info_type_item = trp_information_type_item_t::trp_tx_teg;
      break;
    case asn1::nrppa::trp_info_type_item_opts::options::beam_ant_info:
      trp_info_type_item = trp_information_type_item_t::beam_ant_info;
      break;
    default:
      ocudulog::fetch_basic_logger("NRPPA").info("Unsupported NRPPa TRP info type item ({}). Ignoring it",
                                                 asn1_trp_info_type_item.to_string());
      return std::nullopt;
  }

  return trp_info_type_item;
}

inline asn1::nrppa::dl_prs_muting_pattern_c
dl_prs_muting_pattern_to_asn1(const dl_prs_muting_pattern_t& dl_prs_muting_pattern)
{
  asn1::nrppa::dl_prs_muting_pattern_c asn1_dl_prs_muting_pattern;

  if (dl_prs_muting_pattern.length == 2) {
    asn1_dl_prs_muting_pattern.set_two().from_number(dl_prs_muting_pattern.value);
  }

  if (dl_prs_muting_pattern.length == 4) {
    asn1_dl_prs_muting_pattern.set_four().from_number(dl_prs_muting_pattern.value);
  }

  if (dl_prs_muting_pattern.length == 6) {
    asn1_dl_prs_muting_pattern.set_six().from_number(dl_prs_muting_pattern.value);
  }

  if (dl_prs_muting_pattern.length == 8) {
    asn1_dl_prs_muting_pattern.set_eight().from_number(dl_prs_muting_pattern.value);
  }

  if (dl_prs_muting_pattern.length == 16) {
    asn1_dl_prs_muting_pattern.set_sixteen().from_number(dl_prs_muting_pattern.value);
  }

  if (dl_prs_muting_pattern.length == 32) {
    asn1_dl_prs_muting_pattern.set_thirty_two().from_number(dl_prs_muting_pattern.value);
  }

  return asn1_dl_prs_muting_pattern;
}

inline asn1::nrppa::prs_muting_s prs_muting_to_asn1(const prs_muting_t& prs_muting)
{
  asn1::nrppa::prs_muting_s asn1_prs_muting;

  if (prs_muting.prs_muting_option1.has_value()) {
    asn1_prs_muting.prs_muting_option1_present = true;
    asn1_prs_muting.prs_muting_option1.muting_pattern =
        dl_prs_muting_pattern_to_asn1(prs_muting.prs_muting_option1->muting_pattern);
    asn1::number_to_enum(asn1_prs_muting.prs_muting_option1.muting_bit_repeat_factor,
                         prs_muting.prs_muting_option1->muting_bit_repeat_factor);
  }

  if (prs_muting.prs_muting_option2.has_value()) {
    asn1_prs_muting.prs_muting_option2_present = true;
    asn1_prs_muting.prs_muting_option2.muting_pattern =
        dl_prs_muting_pattern_to_asn1(prs_muting.prs_muting_option2->muting_pattern);
  }

  return asn1_prs_muting;
}

inline asn1::nrppa::prs_res_set_item_s prs_resource_set_item_to_asn1(const prs_resource_set_item_t& prs_res_set_item)
{
  asn1::nrppa::prs_res_set_item_s asn1_prs_res_set_item;

  asn1_prs_res_set_item.prs_res_set_id = prs_res_set_item.prs_res_set_id;
  asn1::number_to_enum(asn1_prs_res_set_item.subcarrier_spacing, scs_to_khz(prs_res_set_item.scs));
  asn1_prs_res_set_item.pr_sbw    = prs_res_set_item.prs_bw;
  asn1_prs_res_set_item.start_prb = prs_res_set_item.start_prb;
  asn1_prs_res_set_item.point_a   = prs_res_set_item.point_a;
  asn1::number_to_enum(asn1_prs_res_set_item.comb_size, prs_res_set_item.comb_size);
  if (prs_res_set_item.cp_type == cyclic_prefix::options::NORMAL) {
    asn1_prs_res_set_item.cp_type = asn1::nrppa::prs_res_set_item_s::cp_type_opts::options::normal;
  } else {
    asn1_prs_res_set_item.cp_type = asn1::nrppa::prs_res_set_item_s::cp_type_opts::options::extended;
  }
  asn1::number_to_enum(asn1_prs_res_set_item.res_set_periodicity, prs_res_set_item.res_set_periodicity);
  asn1_prs_res_set_item.res_set_slot_offset = prs_res_set_item.res_set_slot_offset;
  asn1::number_to_enum(asn1_prs_res_set_item.res_repeat_factor, prs_res_set_item.res_repeat_factor);
  asn1::number_to_enum(asn1_prs_res_set_item.res_time_gap, prs_res_set_item.res_time_gap);
  asn1::number_to_enum(asn1_prs_res_set_item.res_numof_symbols, prs_res_set_item.res_numof_symbols);
  if (prs_res_set_item.prs_muting.has_value()) {
    asn1_prs_res_set_item.prs_muting_present = true;
    asn1_prs_res_set_item.prs_muting         = prs_muting_to_asn1(prs_res_set_item.prs_muting.value());
  }
  asn1_prs_res_set_item.prs_res_tx_pwr = prs_res_set_item.prs_res_tx_pwr;
  for (const auto& prs_res_item : prs_res_set_item.prs_res_list) {
    asn1::nrppa::prs_res_item_s asn1_prs_res_item;
    asn1_prs_res_item.prs_res_id        = prs_res_item.prs_res_id;
    asn1_prs_res_item.seq_id            = prs_res_item.seq_id;
    asn1_prs_res_item.re_offset         = prs_res_item.re_offset;
    asn1_prs_res_item.res_slot_offset   = prs_res_item.res_slot_offset;
    asn1_prs_res_item.res_symbol_offset = prs_res_item.res_symbol_offset;
    if (prs_res_item.qcl_info.has_value()) {
      asn1_prs_res_item.qcl_info_present = true;

      if (std::holds_alternative<ssb_t>(prs_res_item.qcl_info.value())) {
        asn1::nrppa::prs_res_qcl_source_ssb_s& asn1_pres_res_qcl_source_ssb =
            asn1_prs_res_item.qcl_info.set_qcl_source_ssb();

        const ssb_t& prs_res_qcl_source_ssb = std::get<ssb_t>(prs_res_item.qcl_info.value());

        asn1_pres_res_qcl_source_ssb.pci_nr = prs_res_qcl_source_ssb.pci_nr;

        if (prs_res_qcl_source_ssb.ssb_idx.has_value()) {
          asn1_pres_res_qcl_source_ssb.ssb_idx_present = true;
          asn1_pres_res_qcl_source_ssb.ssb_idx         = prs_res_qcl_source_ssb.ssb_idx.value();
        }
      } else {
        asn1::nrppa::prs_res_qcl_source_prs_s& asn1_prs_res_qcl_source_prs =
            asn1_prs_res_item.qcl_info.set_qcl_source_prs();

        const prs_resource_qcl_source_prs_t& prs_res_qcl_source_prs =
            std::get<prs_resource_qcl_source_prs_t>(prs_res_item.qcl_info.value());

        asn1_prs_res_qcl_source_prs.qcl_source_prs_res_set_id = prs_res_qcl_source_prs.qcl_source_prs_res_set_id;
        if (prs_res_qcl_source_prs.qcl_source_prs_res_id.has_value()) {
          asn1_prs_res_qcl_source_prs.qcl_source_prs_res_id_present = true;
          asn1_prs_res_qcl_source_prs.qcl_source_prs_res_id = prs_res_qcl_source_prs.qcl_source_prs_res_id.value();
        }
      }
    }

    asn1_prs_res_set_item.prs_res_list.push_back(asn1_prs_res_item);
  }

  return asn1_prs_res_set_item;
}

inline asn1::nrppa::ssb_info_item_s ssb_info_item_to_asn1(const ssb_info_item_t& ssb_info_item)
{
  asn1::nrppa::ssb_info_item_s asn1_ssb_info_item;

  // Fill SSB config.
  asn1_ssb_info_item.ssb_cfg.ssb_freq = ssb_info_item.ssb_cfg.ssb_freq;
  asn1::number_to_enum(asn1_ssb_info_item.ssb_cfg.ssb_subcarrier_spacing,
                       scs_to_khz(ssb_info_item.ssb_cfg.ssb_subcarrier_spacing));
  asn1_ssb_info_item.ssb_cfg.ssb_tx_pwr = ssb_info_item.ssb_cfg.ssb_tx_pwr;
  asn1::number_to_enum(asn1_ssb_info_item.ssb_cfg.ssb_periodicity, to_underlying(ssb_info_item.ssb_cfg.ssb_period));
  asn1_ssb_info_item.ssb_cfg.ssb_half_frame_offset = ssb_info_item.ssb_cfg.ssb_half_frame_offset;
  asn1_ssb_info_item.ssb_cfg.ssb_sfn_offset        = ssb_info_item.ssb_cfg.ssb_sfn_offset;
  if (ssb_info_item.ssb_cfg.ssb_burst_position.has_value()) {
    asn1_ssb_info_item.ssb_cfg.ssb_burst_position_present = true;

    if (ssb_info_item.ssb_cfg.ssb_burst_position.value().type == ssb_burst_position_t::bitmap_type_t::short_bitmap) {
      asn1_ssb_info_item.ssb_cfg.ssb_burst_position.set_short_bitmap();
      asn1_ssb_info_item.ssb_cfg.ssb_burst_position.short_bitmap().from_number(
          static_cast<uint8_t>(ssb_info_item.ssb_cfg.ssb_burst_position.value().bitmap));
    }
    if (ssb_info_item.ssb_cfg.ssb_burst_position.value().type == ssb_burst_position_t::bitmap_type_t::medium_bitmap) {
      asn1_ssb_info_item.ssb_cfg.ssb_burst_position.set_medium_bitmap();
      asn1_ssb_info_item.ssb_cfg.ssb_burst_position.medium_bitmap().from_number(
          static_cast<uint8_t>(ssb_info_item.ssb_cfg.ssb_burst_position.value().bitmap));
    }
    if (ssb_info_item.ssb_cfg.ssb_burst_position.value().type == ssb_burst_position_t::bitmap_type_t::long_bitmap) {
      asn1_ssb_info_item.ssb_cfg.ssb_burst_position.set_long_bitmap();
      asn1_ssb_info_item.ssb_cfg.ssb_burst_position.long_bitmap().from_number(
          ssb_info_item.ssb_cfg.ssb_burst_position.value().bitmap);
    }
  }
  if (ssb_info_item.ssb_cfg.sfn_initialization_time.has_value()) {
    asn1_ssb_info_item.ssb_cfg.sfn_initisation_time_present = true;
    asn1_ssb_info_item.ssb_cfg.sfn_initisation_time.from_number(ssb_info_item.ssb_cfg.sfn_initialization_time.value());
  }

  // Fill PCI NR.
  asn1_ssb_info_item.pci_nr = ssb_info_item.pci_nr;

  return asn1_ssb_info_item;
}

inline asn1::nrppa::spatial_direction_info_s
spatial_direction_info_to_asn1(const spatial_direction_info_t& spatial_direction_info)
{
  asn1::nrppa::spatial_direction_info_s asn1_spatial_direction_info;

  // Fill NR PRS beam info list.
  for (const auto& nr_prs_beam_info_item : spatial_direction_info.nr_prs_beam_info.nr_prs_beam_info_list) {
    asn1::nrppa::nr_prs_beam_info_item_s asn1_nr_prs_beam_info_item;

    // Fill PR SRES set ID.
    asn1_nr_prs_beam_info_item.pr_sres_set_id = nr_prs_beam_info_item.pr_sres_set_id;

    // Fill PRS angle.
    for (const auto& prs_angle : nr_prs_beam_info_item.prs_angle) {
      asn1::nrppa::prs_angle_item_s asn1_prs_angle_item;

      asn1_prs_angle_item.nr_prs_azimuth = prs_angle.nr_prs_azimuth;
      if (prs_angle.nr_prs_azimuth_fine.has_value()) {
        asn1_prs_angle_item.nr_prs_azimuth_fine_present = true;
        asn1_prs_angle_item.nr_prs_azimuth_fine         = prs_angle.nr_prs_azimuth_fine.value();
      }
      if (prs_angle.nr_prs_elevation.has_value()) {
        asn1_prs_angle_item.nr_prs_elevation_present = true;
        asn1_prs_angle_item.nr_prs_elevation         = prs_angle.nr_prs_elevation.value();
      }
      if (prs_angle.nr_prs_elevation_fine.has_value()) {
        asn1_prs_angle_item.nr_prs_elevation_fine_present = true;
        asn1_prs_angle_item.nr_prs_elevation_fine         = prs_angle.nr_prs_elevation_fine.value();
      }

      asn1_prs_angle_item.ie_exts_present            = true;
      asn1_prs_angle_item.ie_exts.prs_res_id_present = true;
      asn1_prs_angle_item.ie_exts.prs_res_id         = prs_angle.prs_res_id;

      asn1_nr_prs_beam_info_item.prs_angle.push_back(asn1_prs_angle_item);
    }

    asn1_spatial_direction_info.nr_prs_beam_info.nr_prs_beam_info_list.push_back(asn1_nr_prs_beam_info_item);
  }

  // Fill LCS to GCS translation list.
  for (const auto& lcs_to_gcs_translation_item : spatial_direction_info.nr_prs_beam_info.lcs_to_gcs_translation_list) {
    asn1::nrppa::lcs_to_gcs_translation_item_s asn1_lcs_to_gcs_translation_item;
    asn1_lcs_to_gcs_translation_item.alpha = lcs_to_gcs_translation_item.alpha;
    if (lcs_to_gcs_translation_item.alpha_fine.has_value()) {
      asn1_lcs_to_gcs_translation_item.alpha_fine_present = true;
      asn1_lcs_to_gcs_translation_item.alpha_fine         = lcs_to_gcs_translation_item.alpha_fine.value();
    }
    asn1_lcs_to_gcs_translation_item.beta = lcs_to_gcs_translation_item.beta;
    if (lcs_to_gcs_translation_item.beta_fine.has_value()) {
      asn1_lcs_to_gcs_translation_item.beta_fine_present = true;
      asn1_lcs_to_gcs_translation_item.beta_fine         = lcs_to_gcs_translation_item.beta_fine.value();
    }
    asn1_lcs_to_gcs_translation_item.gamma = lcs_to_gcs_translation_item.gamma;
    if (lcs_to_gcs_translation_item.gamma_fine.has_value()) {
      asn1_lcs_to_gcs_translation_item.gamma_fine_present = true;
      asn1_lcs_to_gcs_translation_item.gamma_fine         = lcs_to_gcs_translation_item.gamma_fine.value();
    }

    asn1_spatial_direction_info.nr_prs_beam_info.lcs_to_gcs_translation_list.push_back(
        asn1_lcs_to_gcs_translation_item);
  }

  return asn1_spatial_direction_info;
}

inline asn1::nrppa::ngran_high_accuracy_access_point_position_s ng_ran_high_accuracy_access_point_position_to_asn1(
    const ng_ran_high_accuracy_access_point_position_t& access_point_position)
{
  asn1::nrppa::ngran_high_accuracy_access_point_position_s asn1_access_point_position;

  asn1_access_point_position.latitude                  = access_point_position.latitude;
  asn1_access_point_position.longitude                 = access_point_position.longitude;
  asn1_access_point_position.altitude                  = access_point_position.altitude;
  asn1_access_point_position.uncertainty_semi_major    = access_point_position.uncertainty_semi_major;
  asn1_access_point_position.uncertainty_semi_minor    = access_point_position.uncertainty_semi_minor;
  asn1_access_point_position.orientation_of_major_axis = access_point_position.orientation_of_major_axis;
  asn1_access_point_position.horizontal_confidence     = access_point_position.horizontal_confidence;
  asn1_access_point_position.uncertainty_altitude      = access_point_position.uncertainty_altitude;
  asn1_access_point_position.vertical_confidence       = access_point_position.vertical_confidence;

  return asn1_access_point_position;
}

inline asn1::nrppa::relative_geodetic_location_s
relative_geodetic_location_to_asn1(const relative_geodetic_location_t& location)
{
  asn1::nrppa::relative_geodetic_location_s asn1_location;

  if (location.milli_arc_second_units == milli_arc_second_units_t::zerodot03) {
    asn1_location.milli_arc_second_units =
        asn1::nrppa::relative_geodetic_location_s::milli_arc_second_units_opts::zerodot03;
  } else if (location.milli_arc_second_units == milli_arc_second_units_t::zerodot3) {
    asn1_location.milli_arc_second_units =
        asn1::nrppa::relative_geodetic_location_s::milli_arc_second_units_opts::zerodot3;
  } else {
    asn1_location.milli_arc_second_units =
        asn1::nrppa::relative_geodetic_location_s::milli_arc_second_units_opts::three;
  }

  if (location.height_units == height_units_t::mm) {
    asn1_location.height_units = asn1::nrppa::relative_geodetic_location_s::height_units_opts::mm;
  } else if (location.height_units == height_units_t::cm) {
    asn1_location.height_units = asn1::nrppa::relative_geodetic_location_s::height_units_opts::cm;
  } else {
    asn1_location.height_units = asn1::nrppa::relative_geodetic_location_s::height_units_opts::m;
  }

  asn1_location.delta_latitude  = location.delta_latitude;
  asn1_location.delta_longitude = location.delta_longitude;
  asn1_location.delta_height    = location.delta_height;

  asn1_location.location_uncertainty.horizontal_uncertainty = location.location_uncertainty.horizontal_uncertainty;
  asn1_location.location_uncertainty.horizontal_confidence  = location.location_uncertainty.horizontal_confidence;
  asn1_location.location_uncertainty.vertical_uncertainty   = location.location_uncertainty.vertical_uncertainty;
  asn1_location.location_uncertainty.vertical_confidence    = location.location_uncertainty.vertical_confidence;

  return asn1_location;
}

inline asn1::nrppa::relative_cartesian_location_s
relative_cartesian_location_to_asn1(const relative_cartesian_location_t& location)
{
  asn1::nrppa::relative_cartesian_location_s asn1_location;

  if (location.xyz_unit == xyz_unit_t::mm) {
    asn1_location.xy_zunit = asn1::nrppa::relative_cartesian_location_s::xy_zunit_opts::options::mm;
  } else if (location.xyz_unit == xyz_unit_t::cm) {
    asn1_location.xy_zunit = asn1::nrppa::relative_cartesian_location_s::xy_zunit_opts::cm;
  } else {
    asn1_location.xy_zunit = asn1::nrppa::relative_cartesian_location_s::xy_zunit_opts::dm;
  }

  asn1_location.xvalue = location.xvalue;
  asn1_location.yvalue = location.yvalue;
  asn1_location.zvalue = location.zvalue;

  asn1_location.location_uncertainty.horizontal_uncertainty = location.location_uncertainty.horizontal_uncertainty;
  asn1_location.location_uncertainty.horizontal_confidence  = location.location_uncertainty.horizontal_confidence;
  asn1_location.location_uncertainty.vertical_uncertainty   = location.location_uncertainty.vertical_uncertainty;
  asn1_location.location_uncertainty.vertical_confidence    = location.location_uncertainty.vertical_confidence;

  return asn1_location;
}

inline asn1::nrppa::geographical_coordinates_s
geographical_coordinates_to_asn1(const geographical_coordinates_t& geographical_coordinates)
{
  asn1::nrppa::geographical_coordinates_s asn1_geographical_coordinates;

  // Fill TRP position definition type.
  if (std::holds_alternative<trp_position_direct_t>(geographical_coordinates.trp_position_definition_type)) {
    asn1::nrppa::trp_position_direct_s& asn1_trp_position_direct =
        asn1_geographical_coordinates.trp_position_definition_type.set_direct();

    const trp_position_direct_t& trp_position_direct =
        std::get<trp_position_direct_t>(geographical_coordinates.trp_position_definition_type);

    // Fill accuracy.
    if (std::holds_alternative<ng_ran_access_point_position_t>(trp_position_direct.accuracy)) {
      asn1_trp_position_direct.accuracy.set_trp_position() =
          ng_ran_access_point_position_to_asn1(std::get<ng_ran_access_point_position_t>(trp_position_direct.accuracy));
    } else {
      asn1_trp_position_direct.accuracy.set_trph_aposition() = ng_ran_high_accuracy_access_point_position_to_asn1(
          std::get<ng_ran_high_accuracy_access_point_position_t>(trp_position_direct.accuracy));
    }
  }

  if (std::holds_alternative<trp_position_refd_t>(geographical_coordinates.trp_position_definition_type)) {
    asn1::nrppa::trp_position_refd_s& asn1_trp_position_refd =
        asn1_geographical_coordinates.trp_position_definition_type.set_refd();

    const trp_position_refd_t& trp_position_refd =
        std::get<trp_position_refd_t>(geographical_coordinates.trp_position_definition_type);

    // Fill ref point.
    if (std::holds_alternative<uint16_t>(trp_position_refd.ref_point)) {
      asn1_trp_position_refd.ref_point.set_relative_coordinate_id() = std::get<uint16_t>(trp_position_refd.ref_point);
    } else if (std::holds_alternative<ng_ran_access_point_position_t>(trp_position_refd.ref_point)) {
      asn1_trp_position_refd.ref_point.set_ref_point_coordinate() =
          ng_ran_access_point_position_to_asn1(std::get<ng_ran_access_point_position_t>(trp_position_refd.ref_point));
    } else {
      asn1_trp_position_refd.ref_point.set_ref_point_coordinate_ha() =
          ng_ran_high_accuracy_access_point_position_to_asn1(
              std::get<ng_ran_high_accuracy_access_point_position_t>(trp_position_refd.ref_point));
    }

    // Fill ref point type.
    if (std::holds_alternative<relative_geodetic_location_t>(trp_position_refd.ref_point_type)) {
      asn1_trp_position_refd.ref_point_type.set_trp_position_relative_geodetic() =
          relative_geodetic_location_to_asn1(std::get<relative_geodetic_location_t>(trp_position_refd.ref_point_type));
    } else {
      asn1_trp_position_refd.ref_point_type.set_trp_position_relative_cartesian() = relative_cartesian_location_to_asn1(
          std::get<relative_cartesian_location_t>(trp_position_refd.ref_point_type));
    }
  }

  return asn1_geographical_coordinates;
}

inline asn1::nrppa::trp_type_e trp_type_to_asn1(const trp_type_t& trp_type)
{
  asn1::nrppa::trp_type_e asn1_trp_type;

  if (trp_type == trp_type_t::prs_only_tp) {
    asn1_trp_type = asn1::nrppa::trp_type_opts::prs_only_tp;
  } else if (trp_type == trp_type_t::srs_only_rp) {
    asn1_trp_type = asn1::nrppa::trp_type_opts::srs_only_rp;
  } else if (trp_type == trp_type_t::tp) {
    asn1_trp_type = asn1::nrppa::trp_type_opts::tp;
  } else if (trp_type == trp_type_t::rp) {
    asn1_trp_type = asn1::nrppa::trp_type_opts::rp;
  } else {
    asn1_trp_type = asn1::nrppa::trp_type_opts::trp;
  }

  return asn1_trp_type;
}

inline asn1::nrppa::on_demand_prs_info_s on_demand_prs_info_to_asn1(const on_demand_prs_info_t& on_demand_prs_info)
{
  asn1::nrppa::on_demand_prs_info_s asn1_on_demand_prs_info;

  asn1_on_demand_prs_info.on_demand_prs_request_allowed.from_number(on_demand_prs_info.on_demand_prs_request_allowed);
  if (on_demand_prs_info.allowed_res_set_periodicity_values.has_value()) {
    asn1_on_demand_prs_info.allowed_res_set_periodicity_values_present = true;
    asn1_on_demand_prs_info.allowed_res_set_periodicity_values.from_number(
        on_demand_prs_info.allowed_res_set_periodicity_values.value());
  }
  if (on_demand_prs_info.allowed_prs_bw_values.has_value()) {
    asn1_on_demand_prs_info.allowed_prs_bw_values_present = true;
    asn1_on_demand_prs_info.allowed_prs_bw_values.from_number(on_demand_prs_info.allowed_prs_bw_values.value());
  }
  if (on_demand_prs_info.allowed_res_repeat_factor_values.has_value()) {
    asn1_on_demand_prs_info.allowed_res_repeat_factor_values_present = true;
    asn1_on_demand_prs_info.allowed_res_repeat_factor_values.from_number(
        on_demand_prs_info.allowed_res_repeat_factor_values.value());
  }
  if (on_demand_prs_info.allowed_res_nof_symbols_values.has_value()) {
    asn1_on_demand_prs_info.allowed_res_nof_symbols_values_present = true;
    asn1_on_demand_prs_info.allowed_res_nof_symbols_values.from_number(
        on_demand_prs_info.allowed_res_nof_symbols_values.value());
  }
  if (on_demand_prs_info.allowed_comb_size_values.has_value()) {
    asn1_on_demand_prs_info.allowed_comb_size_values_present = true;
    asn1_on_demand_prs_info.allowed_comb_size_values.from_number(on_demand_prs_info.allowed_comb_size_values.value());
  }

  return asn1_on_demand_prs_info;
}

inline asn1::nrppa::trp_tx_teg_assoc_l trp_tx_teg_assoc_list_to_asn1(const std::vector<trpteg_item_t>& trp_tx_teg_assoc)
{
  asn1::nrppa::trp_tx_teg_assoc_l asn1_trp_tx_teg_assoc;

  for (const auto& trpteg_item : trp_tx_teg_assoc) {
    asn1::nrppa::trpteg_item_s asn1_trpteg_item;

    asn1_trpteg_item.trp_tx_teg_info.trp_tx_teg_id = trpteg_item.trp_tx_teg_info.trp_tx_teg_id;
    asn1::number_to_enum(asn1_trpteg_item.trp_tx_teg_info.trp_tx_timing_error_margin,
                         trpteg_item.trp_tx_teg_info.trp_tx_timing_error_margin);

    asn1_trpteg_item.dl_prs_res_set_id = trpteg_item.dl_prs_res_set_id;

    for (const auto& dl_prs_res_id_item : trpteg_item.dl_prs_res_id_list) {
      asn1::nrppa::dl_prs_res_id_item_s asn1_dl_prs_res_id_item;
      asn1_dl_prs_res_id_item.dl_prs_res_id = dl_prs_res_id_item.dl_prs_res_id;
      asn1_trpteg_item.dl_prs_res_id_list.push_back(asn1_dl_prs_res_id_item);
    }

    asn1_trp_tx_teg_assoc.push_back(asn1_trpteg_item);
  }

  return asn1_trp_tx_teg_assoc;
}

inline asn1::nrppa::trp_beam_ant_info_s trp_beam_ant_info_to_asn1(const trp_beam_ant_info_t& trp_beam_ant_info)
{
  asn1::nrppa::trp_beam_ant_info_s asn1_trp_beam_ant_info;

  if (std::holds_alternative<uint32_t>(trp_beam_ant_info.choice_trp_beam_ant_info_item)) {
    asn1_trp_beam_ant_info.choice_trp_beam_ant_info_item.set_ref() =
        std::get<uint32_t>(trp_beam_ant_info.choice_trp_beam_ant_info_item);
  } else {
    asn1::nrppa::trp_beam_ant_explicit_info_s& asn1_trp_beam_ant_explicit_info =
        asn1_trp_beam_ant_info.choice_trp_beam_ant_info_item.set_explicit_value();
    const trp_beam_ant_explicit_info_t& trp_beam_ant_explicit_info =
        std::get<trp_beam_ant_explicit_info_t>(trp_beam_ant_info.choice_trp_beam_ant_info_item);

    // Fill TRP beam ant angles.
    for (const auto& trp_beam_ant_angle : trp_beam_ant_explicit_info.trp_beam_ant_angles) {
      asn1::nrppa::trp_beam_ant_angles_list_item_s asn1_trp_beam_ant_angle;
      asn1_trp_beam_ant_angle.trp_azimuth_angle = trp_beam_ant_angle.trp_azimuth_angle;
      if (trp_beam_ant_angle.trp_azimuth_angle_fine.has_value()) {
        asn1_trp_beam_ant_angle.trp_azimuth_angle_fine_present = true;
        asn1_trp_beam_ant_angle.trp_azimuth_angle_fine         = trp_beam_ant_angle.trp_azimuth_angle_fine.value();
      }
      for (const auto& trp_elevation_angle : trp_beam_ant_angle.trp_elevation_angle_list) {
        asn1::nrppa::trp_elevation_angle_list_item_s asn1_trp_elevation_angle;
        asn1_trp_elevation_angle.trp_elevation_angle = trp_elevation_angle.trp_elevation_angle;
        if (trp_elevation_angle.trp_elevation_angle_fine.has_value()) {
          asn1_trp_elevation_angle.trp_elevation_angle_fine_present = true;
          asn1_trp_elevation_angle.trp_elevation_angle_fine = trp_elevation_angle.trp_elevation_angle_fine.value();
        }

        for (const auto& trp_beam_pwr_item : trp_elevation_angle.trp_beam_pwr_list) {
          asn1::nrppa::trp_beam_pwr_item_s asn1_trp_beam_pwr_item;
          if (trp_beam_pwr_item.prs_res_set_id.has_value()) {
            asn1_trp_beam_pwr_item.prs_res_set_id_present = true;
            asn1_trp_beam_pwr_item.prs_res_set_id         = trp_beam_pwr_item.prs_res_set_id.value();
          }
          asn1_trp_beam_pwr_item.prs_res_id   = trp_beam_pwr_item.prs_res_id;
          asn1_trp_beam_pwr_item.relative_pwr = trp_beam_pwr_item.relative_pwr;
          if (trp_beam_pwr_item.relative_pwr_fine.has_value()) {
            asn1_trp_beam_pwr_item.relative_pwr_fine_present = true;
            asn1_trp_beam_pwr_item.relative_pwr_fine         = trp_beam_pwr_item.relative_pwr_fine.value();
          }

          asn1_trp_elevation_angle.trp_beam_pwr_list.push_back(asn1_trp_beam_pwr_item);
        }

        asn1_trp_beam_ant_angle.trp_elevation_angle_list.push_back(asn1_trp_elevation_angle);
      }

      asn1_trp_beam_ant_explicit_info.trp_beam_ant_angles.push_back(asn1_trp_beam_ant_angle);
    }

    // Fill LCS to GCS translation.
    if (trp_beam_ant_explicit_info.lcs_to_gcs_translation.has_value()) {
      asn1_trp_beam_ant_explicit_info.lcs_to_gcs_translation_present = true;
      asn1_trp_beam_ant_explicit_info.lcs_to_gcs_translation.alpha =
          trp_beam_ant_explicit_info.lcs_to_gcs_translation->alpha;
      asn1_trp_beam_ant_explicit_info.lcs_to_gcs_translation.beta =
          trp_beam_ant_explicit_info.lcs_to_gcs_translation->beta;
      asn1_trp_beam_ant_explicit_info.lcs_to_gcs_translation.gamma =
          trp_beam_ant_explicit_info.lcs_to_gcs_translation->gamma;
    }
  }

  return asn1_trp_beam_ant_info;
}

inline asn1::nrppa::trp_info_list_trp_resp_item_s_ trp_information_list_trp_response_item_to_asn1(
    const trp_information_list_trp_response_item_t& trp_info_list_trp_response_item)
{
  asn1::nrppa::trp_info_list_trp_resp_item_s_ asn1_trp_info_list_trp_response_item;

  // Fill TRP ID.
  asn1_trp_info_list_trp_response_item.trp_info.trp_id = to_underlying(trp_info_list_trp_response_item.trp_info.trp_id);

  // Fill TRP info type response list.
  for (const auto& trp_info_type_resp_item : trp_info_list_trp_response_item.trp_info.trp_info_type_resp_list) {
    asn1::nrppa::trp_info_type_resp_item_c asn1_trp_info_type_resp_item;

    if (std::holds_alternative<pci_t>(trp_info_type_resp_item)) {
      asn1_trp_info_type_resp_item.set_pci_nr() = std::get<pci_t>(trp_info_type_resp_item);
    }

    if (std::holds_alternative<nr_cell_global_id_t>(trp_info_type_resp_item)) {
      asn1::nrppa::cgi_nr_s& cgi_nr = asn1_trp_info_type_resp_item.set_cgi_nr();
      cgi_nr.nr_cell_id.from_number(std::get<nr_cell_global_id_t>(trp_info_type_resp_item).nci.value());
      cgi_nr.plmn_id = std::get<nr_cell_global_id_t>(trp_info_type_resp_item).plmn_id.to_bytes();
    }

    if (std::holds_alternative<uint32_t>(trp_info_type_resp_item)) {
      asn1_trp_info_type_resp_item.set_arfcn() = std::get<uint32_t>(trp_info_type_resp_item);
    }

    if (std::holds_alternative<prs_cfg_t>(trp_info_type_resp_item)) {
      asn1::nrppa::prs_cfg_s& asn1_prs_cfg = asn1_trp_info_type_resp_item.set_prs_cfg();

      for (const auto& prs_resource_set_item : std::get<prs_cfg_t>(trp_info_type_resp_item).prs_res_set_list) {
        asn1_prs_cfg.prs_res_set_list.push_back(prs_resource_set_item_to_asn1(prs_resource_set_item));
      }
    }

    if (std::holds_alternative<ssb_info_t>(trp_info_type_resp_item)) {
      asn1::nrppa::ssb_info_s& asn1_ssb_info = asn1_trp_info_type_resp_item.set_ss_binfo();
      for (const auto& ssb_info_item : std::get<ssb_info_t>(trp_info_type_resp_item).list_of_ssb_info) {
        asn1_ssb_info.list_of_ssb_info.push_back(ssb_info_item_to_asn1(ssb_info_item));
      }
    }

    if (std::holds_alternative<uint64_t>(trp_info_type_resp_item)) {
      asn1_trp_info_type_resp_item.set_sfn_initisation_time().from_number(std::get<uint64_t>(trp_info_type_resp_item));
    }

    if (std::holds_alternative<spatial_direction_info_t>(trp_info_type_resp_item)) {
      asn1_trp_info_type_resp_item.set_spatial_direction_info() =
          spatial_direction_info_to_asn1(std::get<spatial_direction_info_t>(trp_info_type_resp_item));
    }

    if (std::holds_alternative<geographical_coordinates_t>(trp_info_type_resp_item)) {
      asn1_trp_info_type_resp_item.set_geographical_coordinates() =
          geographical_coordinates_to_asn1(std::get<geographical_coordinates_t>(trp_info_type_resp_item));
    }

    if (std::holds_alternative<trp_type_t>(trp_info_type_resp_item)) {
      asn1::protocol_ie_single_container_s<asn1::nrppa::trp_info_type_resp_item_ext_ies_o>& asn1_ie_exts_container =
          asn1_trp_info_type_resp_item.set_choice_ext();
      asn1_ie_exts_container.value().trp_type() = trp_type_to_asn1(std::get<trp_type_t>(trp_info_type_resp_item));
    }

    if (std::holds_alternative<on_demand_prs_info_t>(trp_info_type_resp_item)) {
      asn1::protocol_ie_single_container_s<asn1::nrppa::trp_info_type_resp_item_ext_ies_o>& asn1_ie_exts_container =
          asn1_trp_info_type_resp_item.set_choice_ext();
      asn1_ie_exts_container.value().on_demand_prs() =
          on_demand_prs_info_to_asn1(std::get<on_demand_prs_info_t>(trp_info_type_resp_item));
    }

    if (std::holds_alternative<std::vector<trpteg_item_t>>(trp_info_type_resp_item)) {
      asn1::protocol_ie_single_container_s<asn1::nrppa::trp_info_type_resp_item_ext_ies_o>& asn1_ie_exts_container =
          asn1_trp_info_type_resp_item.set_choice_ext();
      asn1_ie_exts_container.value().trp_tx_teg_assoc() =
          trp_tx_teg_assoc_list_to_asn1(std::get<std::vector<trpteg_item_t>>(trp_info_type_resp_item));
    }

    if (std::holds_alternative<trp_beam_ant_info_t>(trp_info_type_resp_item)) {
      asn1::protocol_ie_single_container_s<asn1::nrppa::trp_info_type_resp_item_ext_ies_o>& asn1_ie_exts_container =
          asn1_trp_info_type_resp_item.set_choice_ext();
      asn1_ie_exts_container.value().trp_beam_ant_info() =
          trp_beam_ant_info_to_asn1(std::get<trp_beam_ant_info_t>(trp_info_type_resp_item));
    }

    asn1_trp_info_list_trp_response_item.trp_info.trp_info_type_resp_list.push_back(asn1_trp_info_type_resp_item);
  }

  return asn1_trp_info_list_trp_response_item;
}

inline ssb_info_t asn1_to_ssb_info(const asn1::nrppa::ssb_info_s& asn1_ssb_info)
{
  ssb_info_t ssb_info;

  for (const auto& asn1_ssb_info_item : asn1_ssb_info.list_of_ssb_info) {
    ssb_info_item_t ssb_info_item;
    // Fill SSB configuration.
    ssb_info_item.ssb_cfg.ssb_freq = asn1_ssb_info_item.ssb_cfg.ssb_freq;
    ssb_info_item.ssb_cfg.ssb_subcarrier_spacing =
        khz_to_scs(asn1_ssb_info_item.ssb_cfg.ssb_subcarrier_spacing.to_number());
    ssb_info_item.ssb_cfg.ssb_tx_pwr = asn1_ssb_info_item.ssb_cfg.ssb_tx_pwr;
    ssb_info_item.ssb_cfg.ssb_period = ssb_periodicity(asn1_ssb_info_item.ssb_cfg.ssb_periodicity.to_number());
    ssb_info_item.ssb_cfg.ssb_half_frame_offset = asn1_ssb_info_item.ssb_cfg.ssb_half_frame_offset;
    ssb_info_item.ssb_cfg.ssb_sfn_offset        = asn1_ssb_info_item.ssb_cfg.ssb_sfn_offset;
    if (asn1_ssb_info_item.ssb_cfg.ssb_burst_position_present) {
      ssb_burst_position_t burst_position;
      // short bitmap
      if (asn1_ssb_info_item.ssb_cfg.ssb_burst_position.type() ==
          asn1::nrppa::ssb_burst_position_c::types_opts::options::short_bitmap) {
        burst_position.type   = ssb_burst_position_t::bitmap_type_t::short_bitmap;
        burst_position.bitmap = asn1_ssb_info_item.ssb_cfg.ssb_burst_position.short_bitmap().to_number();
      }
      // medium bitmap
      if (asn1_ssb_info_item.ssb_cfg.ssb_burst_position.type() ==
          asn1::nrppa::ssb_burst_position_c::types_opts::options::medium_bitmap) {
        burst_position.type   = ssb_burst_position_t::bitmap_type_t::medium_bitmap;
        burst_position.bitmap = asn1_ssb_info_item.ssb_cfg.ssb_burst_position.medium_bitmap().to_number();
      }
      // long bitmap
      if (asn1_ssb_info_item.ssb_cfg.ssb_burst_position.type() ==
          asn1::nrppa::ssb_burst_position_c::types_opts::options::long_bitmap) {
        burst_position.type   = ssb_burst_position_t::bitmap_type_t::long_bitmap;
        burst_position.bitmap = asn1_ssb_info_item.ssb_cfg.ssb_burst_position.long_bitmap().to_number();
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

  return ssb_info;
}

inline spatial_relation_info_t::reference_signal asn1_to_ref_sig(const asn1::nrppa::ref_sig_c& asn1_ref_sig)
{
  spatial_relation_info_t::reference_signal ref_sig;

  if (asn1_ref_sig.type() == asn1::nrppa::ref_sig_c::types_opts::options::nzp_csi_rs) {
    ref_sig = nzp_csi_rs_res_id_t{asn1_ref_sig.nzp_csi_rs()};
  } else if (asn1_ref_sig.type() == asn1::nrppa::ref_sig_c::types_opts::options::ssb) {
    const asn1::nrppa::ssb_s& asn1_ssb = asn1_ref_sig.ssb();
    ssb_t                     ssb;
    ssb.pci_nr = asn1_ssb.pci_nr;
    if (asn1_ssb.ssb_idx_present) {
      ssb.ssb_idx = asn1_ssb.ssb_idx;
    }
    ref_sig = ssb;
  } else if (asn1_ref_sig.type() == asn1::nrppa::ref_sig_c::types_opts::options::srs) {
    ref_sig = srs_config::srs_res_id(asn1_ref_sig.srs());
  } else if (asn1_ref_sig.type() == asn1::nrppa::ref_sig_c::types_opts::options::positioning_srs) {
    ref_sig = srs_config::srs_pos_res_id(asn1_ref_sig.positioning_srs());
  } else {
    const asn1::nrppa::dl_prs_s& asn1_dl_prs = asn1_ref_sig.dl_prs();
    dl_prs_t                     dl_prs;
    dl_prs.prs_id            = asn1_dl_prs.prsid;
    dl_prs.dl_prs_res_set_id = asn1_dl_prs.dl_prs_res_set_id;
    if (asn1_dl_prs.dl_prs_res_id_present) {
      dl_prs.dl_prs_res_id = asn1_dl_prs.dl_prs_res_id;
    }

    ref_sig = dl_prs;
  }

  return ref_sig;
}

inline std::optional<requested_srs_tx_characteristics_t>
asn1_to_requested_srs_tx_characteristics(const asn1::nrppa::requested_srs_tx_characteristics_s& asn1_req)
{
  requested_srs_tx_characteristics_t req;

  if (asn1_req.nof_txs_present) {
    req.nof_txs = asn1_req.nof_txs;
  }

  if (asn1_req.res_type == asn1::nrppa::requested_srs_tx_characteristics_s::res_type_opts::options::periodic) {
    req.res_type = requested_srs_tx_characteristics_t::res_type_t::periodic;
  } else if (asn1_req.res_type ==
             asn1::nrppa::requested_srs_tx_characteristics_s::res_type_opts::options::semi_persistent) {
    req.res_type = requested_srs_tx_characteristics_t::res_type_t::semi_persistent;
  } else {
    req.res_type = requested_srs_tx_characteristics_t::res_type_t::aperiodic;
  }

  if (asn1_req.bw.type() == asn1::nrppa::bw_srs_c::types_opts::options::fr1) {
    req.bw = asn1_req.bw.fr1().to_number();
  } else if (asn1_req.bw.type() == asn1::nrppa::bw_srs_c::types_opts::options::fr2) {
    req.bw = asn1_req.bw.fr2().to_number();
  } else {
    // BandwidthSRS is mandatory, so an unrecognized choice leaves the whole RequestedSRSTransmissionCharacteristics
    // without a well-defined bandwidth, and it is dropped.
    ocudulog::fetch_basic_logger("NRPPA").warning("Unsupported BandwidthSRS choice ({}). Ignoring the request",
                                                  asn1_req.bw.type().to_string());
    return std::nullopt;
  }

  for (const auto& asn1_srs_res_set_item : asn1_req.list_of_srs_res_set) {
    srs_res_set_item_t srs_res_set_item;

    if (asn1_srs_res_set_item.nof_srs_res_per_set_present) {
      srs_res_set_item.nof_srs_res_per_set = asn1_srs_res_set_item.nof_srs_res_per_set;
    }
    for (const auto& asn1_period : asn1_srs_res_set_item.periodicity_list) {
      periodicity_item_t period_item;
      period_item.value = static_cast<periodicity_item_t::value_t>((unsigned)asn1_period.value);
      srs_res_set_item.periodicity_list.push_back(period_item);
    }
    if (asn1_srs_res_set_item.spatial_relation_info_present) {
      spatial_relation_info_t spatial_relation_info;
      for (const auto& asn1_spatial_relation_for_res_id_item :
           asn1_srs_res_set_item.spatial_relation_info.spatial_relationfor_res_id) {
        spatial_relation_info_t::reference_signal ref_sig =
            asn1_to_ref_sig(asn1_spatial_relation_for_res_id_item.ref_sig);
        spatial_relation_info.reference_signals.push_back(ref_sig);
      }
      srs_res_set_item.spatial_relation_info = spatial_relation_info;
    }

    req.list_of_srs_res_set.push_back(srs_res_set_item);
  }

  if (asn1_req.ssb_info_present) {
    req.ssb_info = asn1_to_ssb_info(asn1_req.ssb_info);
  }

  if (asn1_req.ie_exts_present) {
    if (asn1_req.ie_exts.srs_freq_present) {
      req.srs_freqs.push_back(asn1_req.ie_exts.srs_freq);
    }
  }

  return req;
}

inline srs_configuration_t asn1_to_srs_configuration(const asn1::nrppa::srs_configuration_s& asn1_srs_cfg)
{
  srs_configuration_t srs_cfg;
  // Fill SRS carrier list.
  for (const auto& asn1_srs_carrier_list_item : asn1_srs_cfg.srs_carrier_list) {
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
    if (asn1_srs_carrier_list_item.active_ul_bwp.cp.value == asn1::nrppa::active_ul_bwp_s::cp_opts::options::normal) {
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
      srs_config::srs_resource srs_res;
      srs_res.id.ue_res_id = (srs_config::srs_res_id)asn1_srs_res.srs_res_id;
      srs_res.nof_ports    = (srs_config::srs_resource::nof_srs_ports)asn1_srs_res.nrof_srs_ports.to_number();
      if (asn1_srs_res.tx_comb.type() == asn1::nrppa::tx_comb_c::types_opts::options::n2) {
        srs_res.tx_comb.size                 = tx_comb_size::n2;
        srs_res.tx_comb.tx_comb_offset       = asn1_srs_res.tx_comb.n2().comb_offset_n2;
        srs_res.tx_comb.tx_comb_cyclic_shift = asn1_srs_res.tx_comb.n2().cyclic_shift_n2;
      } else if (asn1_srs_res.tx_comb.type() == asn1::nrppa::tx_comb_c::types_opts::options::n4) {
        srs_res.tx_comb.size                 = tx_comb_size::n4;
        srs_res.tx_comb.tx_comb_offset       = asn1_srs_res.tx_comb.n4().comb_offset_n4;
        srs_res.tx_comb.tx_comb_cyclic_shift = asn1_srs_res.tx_comb.n4().cyclic_shift_n4;
      } else {
        // TxComb is mandatory for the SRS resource, so an unrecognized choice leaves its comb configuration
        // without a well-defined value, and the resource is dropped.
        ocudulog::fetch_basic_logger("NRPPA").warning("Unsupported TxComb choice ({}). Dropping the SRS resource",
                                                      asn1_srs_res.tx_comb.type().to_string());
        continue;
      }
      srs_res.res_mapping.start_pos   = asn1_srs_res.start_position;
      srs_res.res_mapping.nof_symb    = (srs_nof_symbols)asn1_srs_res.nrof_symbols.to_number();
      srs_res.res_mapping.rept_factor = (srs_nof_symbols)asn1_srs_res.repeat_factor.to_number();
      srs_res.freq_domain_pos         = asn1_srs_res.freq_domain_position;
      srs_res.freq_domain_shift       = asn1_srs_res.freq_domain_shift;
      srs_res.freq_hop.c_srs          = asn1_srs_res.c_srs;
      srs_res.freq_hop.b_srs          = asn1_srs_res.b_srs;
      srs_res.freq_hop.b_hop          = asn1_srs_res.b_hop;
      if (asn1_srs_res.group_or_seq_hop == asn1::nrppa::srs_res_s::group_or_seq_hop_opts::options::neither) {
        srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::neither;
      } else if (asn1_srs_res.group_or_seq_hop == asn1::nrppa::srs_res_s::group_or_seq_hop_opts::options::group_hop) {
        srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::group_hopping;
      } else {
        srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::sequence_hopping;
      }
      if (asn1_srs_res.res_type.type() == asn1::nrppa::res_type_c::types_opts::options::periodic) {
        srs_res.res_type = srs_resource_type::periodic;
        srs_res.periodicity_and_offset.emplace();
        srs_res.periodicity_and_offset->period =
            (srs_periodicity)asn1_srs_res.res_type.periodic().periodicity.to_number();
        srs_res.periodicity_and_offset->offset = asn1_srs_res.res_type.periodic().offset;
      } else if (asn1_srs_res.res_type.type() == asn1::nrppa::res_type_c::types_opts::options::semi_persistent) {
        srs_res.res_type = srs_resource_type::semi_persistent;
        srs_res.periodicity_and_offset.emplace();
        srs_res.periodicity_and_offset->period =
            (srs_periodicity)asn1_srs_res.res_type.semi_persistent().periodicity.to_number();
        srs_res.periodicity_and_offset->offset = asn1_srs_res.res_type.semi_persistent().offset;
      } else if (asn1_srs_res.res_type.type() == asn1::nrppa::res_type_c::types_opts::options::aperiodic) {
        srs_res.res_type = srs_resource_type::aperiodic;
      } else {
        // ResourceType is mandatory for the SRS resource, so an unrecognized choice leaves its resource type
        // without a well-defined value, and the resource is dropped.
        ocudulog::fetch_basic_logger("NRPPA").warning("Unsupported ResourceType choice ({}). Dropping the SRS resource",
                                                      asn1_srs_res.res_type.type().to_string());
        continue;
      }
      srs_res.sequence_id = asn1_srs_res.seq_id;

      srs_carrier_list_item.active_ul_bwp.srs_cfg.srs_res_list.push_back(srs_res);
    }
    // >> Fill pos SRS res list.
    for (const auto& asn1_pos_srs_res : asn1_srs_carrier_list_item.active_ul_bwp.srs_cfg.pos_srs_res_list) {
      srs_config::srs_pos_resource pos_srs_res;
      pos_srs_res.id.ue_res_id = (srs_config::srs_res_id)asn1_pos_srs_res.srs_pos_res_id;
      if (asn1_pos_srs_res.tx_comb_pos.type() == asn1::nrppa::tx_comb_pos_c::types_opts::options::n2) {
        pos_srs_res.tx_comb.tx_comb_pos_offset       = asn1_pos_srs_res.tx_comb_pos.n2().comb_offset_n2;
        pos_srs_res.tx_comb.tx_comb_pos_cyclic_shift = asn1_pos_srs_res.tx_comb_pos.n2().cyclic_shift_n2;
      } else if (asn1_pos_srs_res.tx_comb_pos.type() == asn1::nrppa::tx_comb_pos_c::types_opts::options::n4) {
        pos_srs_res.tx_comb.tx_comb_pos_offset       = asn1_pos_srs_res.tx_comb_pos.n4().comb_offset_n4;
        pos_srs_res.tx_comb.tx_comb_pos_cyclic_shift = asn1_pos_srs_res.tx_comb_pos.n4().cyclic_shift_n4;
      } else if (asn1_pos_srs_res.tx_comb_pos.type() == asn1::nrppa::tx_comb_pos_c::types_opts::options::n8) {
        pos_srs_res.tx_comb.tx_comb_pos_offset       = asn1_pos_srs_res.tx_comb_pos.n8().comb_offset_n8;
        pos_srs_res.tx_comb.tx_comb_pos_cyclic_shift = asn1_pos_srs_res.tx_comb_pos.n8().cyclic_shift_n8;
      } else {
        // TxCombPos is mandatory for the positioning SRS resource, so an unrecognized choice leaves its comb
        // configuration without a well-defined value, and the resource is dropped.
        ocudulog::fetch_basic_logger("NRPPA").warning(
            "Unsupported TxCombPos choice ({}). Dropping the positioning SRS resource",
            asn1_pos_srs_res.tx_comb_pos.type().to_string());
        continue;
      }
      pos_srs_res.res_mapping.start_pos = asn1_pos_srs_res.start_position;
      pos_srs_res.res_mapping.nof_symb  = (srs_nof_symbols)asn1_pos_srs_res.nrof_symbols.to_number();
      pos_srs_res.freq_domain_shift     = asn1_pos_srs_res.freq_domain_shift;
      pos_srs_res.c_srs                 = asn1_pos_srs_res.c_srs;
      if (asn1_pos_srs_res.group_or_seq_hop ==
          asn1::nrppa::pos_srs_res_item_s::group_or_seq_hop_opts::options::neither) {
        pos_srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::neither;
      } else if (asn1_pos_srs_res.group_or_seq_hop ==
                 asn1::nrppa::pos_srs_res_item_s::group_or_seq_hop_opts::options::group_hop) {
        pos_srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::group_hopping;
      } else {
        pos_srs_res.grp_or_seq_hop = srs_group_or_sequence_hopping::sequence_hopping;
      }
      if (asn1_pos_srs_res.res_type_pos.type() == asn1::nrppa::res_type_pos_c::types_opts::options::periodic) {
        pos_srs_res.res_type = srs_resource_type::periodic;
        pos_srs_res.periodicity_and_offset.emplace();
        pos_srs_res.periodicity_and_offset->period =
            (srs_periodicity)asn1_pos_srs_res.res_type_pos.periodic().srs_periodicity.to_number();
        pos_srs_res.periodicity_and_offset->offset = asn1_pos_srs_res.res_type_pos.periodic().offset;
      } else if (asn1_pos_srs_res.res_type_pos.type() ==
                 asn1::nrppa::res_type_pos_c::types_opts::options::semi_persistent) {
        pos_srs_res.res_type = srs_resource_type::semi_persistent;
        pos_srs_res.periodicity_and_offset.emplace();
        pos_srs_res.periodicity_and_offset->period =
            (srs_periodicity)asn1_pos_srs_res.res_type_pos.semi_persistent().srs_periodicity.to_number();
        pos_srs_res.periodicity_and_offset->offset = asn1_pos_srs_res.res_type_pos.semi_persistent().offset;
      } else if (asn1_pos_srs_res.res_type_pos.type() == asn1::nrppa::res_type_pos_c::types_opts::options::aperiodic) {
        pos_srs_res.res_type    = srs_resource_type::aperiodic;
        pos_srs_res.slot_offset = asn1_pos_srs_res.res_type_pos.aperiodic().slot_offset;
      } else {
        // ResourceTypePos is mandatory for the positioning SRS resource, so an unrecognized choice leaves its
        // resource type without a well-defined value, and the resource is dropped.
        ocudulog::fetch_basic_logger("NRPPA").warning(
            "Unsupported ResourceTypePos choice ({}). Dropping the positioning SRS resource",
            asn1_pos_srs_res.res_type_pos.type().to_string());
        continue;
      }
      pos_srs_res.sequence_id = asn1_pos_srs_res.seq_id;

      if (asn1_pos_srs_res.spatial_relation_pos_present) {
        if (asn1_pos_srs_res.spatial_relation_pos.type() ==
            asn1::nrppa::spatial_relation_pos_c::types_opts::options::ssb_pos) {
          srs_config::srs_pos_resource::srs_spatial_relation_pos::ssb ssb;
          ssb.pci_nr = asn1_pos_srs_res.spatial_relation_pos.ssb_pos().pci_nr;
          if (asn1_pos_srs_res.spatial_relation_pos.ssb_pos().ssb_idx_present) {
            ssb.ssb_idx = asn1_pos_srs_res.spatial_relation_pos.ssb_pos().ssb_idx;
          }
          pos_srs_res.spatial_relation_info.emplace();
          pos_srs_res.spatial_relation_info->reference_signal = ssb;
        } else if (asn1_pos_srs_res.spatial_relation_pos.type() ==
                   asn1::nrppa::spatial_relation_pos_c::types_opts::options::pr_si_nformation_pos) {
          srs_config::srs_pos_resource::srs_spatial_relation_pos::prs prs;
          prs.id             = asn1_pos_srs_res.spatial_relation_pos.pr_si_nformation_pos().prs_id_pos;
          prs.prs_res_set_id = asn1_pos_srs_res.spatial_relation_pos.pr_si_nformation_pos().prs_res_set_id_pos;
          if (asn1_pos_srs_res.spatial_relation_pos.pr_si_nformation_pos().prs_res_id_pos_present) {
            prs.prs_res_id = asn1_pos_srs_res.spatial_relation_pos.pr_si_nformation_pos().prs_res_id_pos;
          }
          pos_srs_res.spatial_relation_info.emplace();
          pos_srs_res.spatial_relation_info->reference_signal = prs;
        } else {
          ocudulog::fetch_basic_logger("NRPPA").warning("Unsupported SpatialRelationPos choice ({}). Ignoring it",
                                                        asn1_pos_srs_res.spatial_relation_pos.type().to_string());
        }
      }

      srs_carrier_list_item.active_ul_bwp.srs_cfg.pos_srs_res_list.push_back(pos_srs_res);
    }

    // >> Fill SRS res set list.
    for (const auto& asn1_srs_res_set : asn1_srs_carrier_list_item.active_ul_bwp.srs_cfg.srs_res_set_list) {
      srs_config::srs_resource_set srs_res_set;
      srs_res_set.id = (srs_config::srs_res_set_id)asn1_srs_res_set.srs_res_set_id1;
      for (const auto& asn1_res_id : asn1_srs_res_set.srs_res_id_list) {
        srs_res_set.srs_res_id_list.push_back((srs_config::srs_res_id)asn1_res_id);
      }
      if (asn1_srs_res_set.res_set_type.type() == asn1::nrppa::res_set_type_c::types_opts::options::periodic) {
        srs_res_set.res_type = srs_config::srs_resource_set::periodic_resource_type{};
      } else if (asn1_srs_res_set.res_set_type.type() ==
                 asn1::nrppa::res_set_type_c::types_opts::options::semi_persistent) {
        srs_res_set.res_type = srs_config::srs_resource_set::semi_persistent_resource_type{};
      } else if (asn1_srs_res_set.res_set_type.type() == asn1::nrppa::res_set_type_c::types_opts::options::aperiodic) {
        srs_config::srs_resource_set::aperiodic_resource_type aperiodic;
        aperiodic.aperiodic_srs_res_trigger = asn1_srs_res_set.res_set_type.aperiodic().srs_res_trigger;
        aperiodic.slot_offset               = asn1_srs_res_set.res_set_type.aperiodic().slotoffset;
        srs_res_set.res_type                = aperiodic;
      } else {
        // ResourceSetType is mandatory for the SRS resource set, so an unrecognized choice leaves its resource
        // type without a well-defined value, and the resource set is dropped.
        ocudulog::fetch_basic_logger("NRPPA").warning(
            "Unsupported ResourceSetType choice ({}). Dropping the SRS resource set",
            asn1_srs_res_set.res_set_type.type().to_string());
        continue;
      }

      srs_carrier_list_item.active_ul_bwp.srs_cfg.srs_res_set_list.push_back(srs_res_set);
    }

    // >> Fill pos SRS res set list.
    for (const auto& asn1_pos_srs_res_set : asn1_srs_carrier_list_item.active_ul_bwp.srs_cfg.pos_srs_res_set_list) {
      srs_config::srs_resource_set pos_srs_res_set;
      pos_srs_res_set.id = (srs_config::srs_res_set_id)asn1_pos_srs_res_set.possrs_res_set_id;
      for (const auto& asn1_pos_srs_res_id : asn1_pos_srs_res_set.poss_rs_res_id_per_set_list) {
        pos_srs_res_set.srs_res_id_list.push_back((srs_config::srs_res_id)asn1_pos_srs_res_id);
      }
      if (asn1_pos_srs_res_set.posres_set_type.type() ==
          asn1::nrppa::pos_res_set_type_c::types_opts::options::periodic) {
        pos_srs_res_set.res_type = srs_config::srs_resource_set::periodic_resource_type{};
      } else if (asn1_pos_srs_res_set.posres_set_type.type() ==
                 asn1::nrppa::pos_res_set_type_c::types_opts::options::semi_persistent) {
        pos_srs_res_set.res_type = srs_config::srs_resource_set::semi_persistent_resource_type{};
      } else if (asn1_pos_srs_res_set.posres_set_type.type() ==
                 asn1::nrppa::pos_res_set_type_c::types_opts::options::aperiodic) {
        srs_config::srs_resource_set::aperiodic_resource_type aperiodic;
        aperiodic.aperiodic_srs_res_trigger = asn1_pos_srs_res_set.posres_set_type.aperiodic().srs_res_trigger;
        pos_srs_res_set.res_type            = aperiodic;
      } else {
        // PosResourceSetType is mandatory for the positioning SRS resource set, so an unrecognized choice leaves
        // its resource type without a well-defined value, and the resource set is dropped.
        ocudulog::fetch_basic_logger("NRPPA").warning(
            "Unsupported PosResourceSetType choice ({}). Dropping the positioning SRS resource set",
            asn1_pos_srs_res_set.posres_set_type.type().to_string());
        continue;
      }

      srs_carrier_list_item.active_ul_bwp.srs_cfg.pos_srs_res_set_list.push_back(pos_srs_res_set);
    }

    // Fill PCI.
    if (asn1_srs_carrier_list_item.pci_nr_present) {
      srs_carrier_list_item.pci_nr = asn1_srs_carrier_list_item.pci_nr;
    }

    srs_cfg.srs_carrier_list.push_back(srs_carrier_list_item);
  }

  return srs_cfg;
}

inline expected<nr_cell_global_id_t> cgi_from_asn1(const asn1::nrppa::cgi_nr_s& asn1_cgi)
{
  auto plmn = plmn_identity::from_bytes(asn1_cgi.plmn_id.to_bytes());
  if (not plmn.has_value()) {
    return make_unexpected(plmn.error());
  }
  auto nci = nr_cell_identity::create(asn1_cgi.nr_cell_id.to_number());
  if (not nci.has_value()) {
    return make_unexpected(nci.error());
  }
  return nr_cell_global_id_t{plmn.value(), nci.value()};
}

inline trp_meas_request_item_t
asn1_to_trp_meas_request_item(const asn1::nrppa::trp_meas_request_item_s& asn1_trp_meas_request_item)
{
  trp_meas_request_item_t trp_meas_request_item;

  // Fill TRP ID.
  trp_meas_request_item.trp_id = uint_to_trp_id(asn1_trp_meas_request_item.trp_id);

  // Fill search window info.
  if (asn1_trp_meas_request_item.search_win_info_present) {
    trp_meas_request_item.search_win_info = {
        asn1_trp_meas_request_item.search_win_info.expected_propagation_delay,
        static_cast<uint8_t>(asn1_trp_meas_request_item.search_win_info.delay_uncertainty)};
  }

  // Fill IE exts.
  if (asn1_trp_meas_request_item.ie_exts_present) {
    // Fill CGI NR.
    if (asn1_trp_meas_request_item.ie_exts.cell_id_present) {
      expected<nr_cell_global_id_t> cgi = cgi_from_asn1(asn1_trp_meas_request_item.ie_exts.cell_id);
      ocudu_assert(cgi.has_value(), "Received invalid CGI NR");

      trp_meas_request_item.cgi_nr = cgi.value();
    }

    // Fill AoA search window.
    if (asn1_trp_meas_request_item.ie_exts.ao_a_search_win_present) {
      aoa_assist_info_t aoa_search_win;

      if (asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.type() ==
          asn1::nrppa::angle_meas_type_c::types_opts::options::expected_ul_ao_a) {
        expected_ul_aoa_t expected_ul_aoa;
        expected_ul_aoa.expected_azimuth_aoa.expected_azimuth_aoa_value =
            asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
                .expected_azimuth_ao_a.expected_azimuth_ao_a_value;
        expected_ul_aoa.expected_azimuth_aoa.expected_azimuth_aoa_uncertainty =
            asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
                .expected_azimuth_ao_a.expected_azimuth_ao_a_uncertainty;

        if (asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
                .expected_zenith_ao_a_present) {
          expected_zenith_aoa_t expected_zenith_aoa;

          expected_zenith_aoa.expected_zenith_aoa_value =
              asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
                  .expected_zenith_ao_a.expected_zenith_ao_a_value;
          expected_zenith_aoa.expected_zenith_aoa_uncertainty =
              asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
                  .expected_zenith_ao_a.expected_zenith_ao_a_uncertainty;

          expected_ul_aoa.expected_zenith_aoa = expected_zenith_aoa;
        }

        aoa_search_win.angle_meas = expected_ul_aoa;
      } else {
        expected_zoa_only_t expected_zoa_only;
        expected_zoa_only.expected_zoa_only.expected_zenith_aoa_value =
            asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_zo_a()
                .expected_zo_a_only.expected_zenith_ao_a_value;
        expected_zoa_only.expected_zoa_only.expected_zenith_aoa_uncertainty =
            asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_zo_a()
                .expected_zo_a_only.expected_zenith_ao_a_uncertainty;

        aoa_search_win.angle_meas = expected_zoa_only;
      }

      trp_meas_request_item.aoa_search_win = aoa_search_win;
    }

    // Fill number of TRP RX TEG.
    if (asn1_trp_meas_request_item.ie_exts.nof_trp_rx_teg_present) {
      trp_meas_request_item.nof_trp_rx_teg = asn1_trp_meas_request_item.ie_exts.nof_trp_rx_teg.to_number();
    }

    // Fill number of TRP RX TX TEG.
    if (asn1_trp_meas_request_item.ie_exts.nof_trp_rx_tx_teg_present) {
      trp_meas_request_item.nof_trp_rx_tx_teg = asn1_trp_meas_request_item.ie_exts.nof_trp_rx_tx_teg.to_number();
    }
  }

  return trp_meas_request_item;
}

inline asn1::nrppa::trp_meas_request_item_s
trp_meas_request_item_to_asn1(const trp_meas_request_item_t& trp_meas_request_item)
{
  asn1::nrppa::trp_meas_request_item_s asn1_trp_meas_request_item;

  // Fill TRP ID.
  asn1_trp_meas_request_item.trp_id = to_underlying(trp_meas_request_item.trp_id);

  // Fill search window info.
  if (trp_meas_request_item.search_win_info.has_value()) {
    asn1_trp_meas_request_item.search_win_info_present = true;
    asn1_trp_meas_request_item.search_win_info.expected_propagation_delay =
        trp_meas_request_item.search_win_info->expected_propagation_delay;
    asn1_trp_meas_request_item.search_win_info.delay_uncertainty =
        trp_meas_request_item.search_win_info->delay_uncertainty;
  }

  // Fill IE exts.
  // > Fill CGI NR.
  if (trp_meas_request_item.cgi_nr.has_value()) {
    asn1_trp_meas_request_item.ie_exts_present         = true;
    asn1_trp_meas_request_item.ie_exts.cell_id_present = true;
    asn1_trp_meas_request_item.ie_exts.cell_id.nr_cell_id.from_number(trp_meas_request_item.cgi_nr->nci.value());
    asn1_trp_meas_request_item.ie_exts.cell_id.plmn_id = trp_meas_request_item.cgi_nr->plmn_id.to_bytes();
  }

  // > Fill AoA search window.
  if (trp_meas_request_item.aoa_search_win.has_value()) {
    asn1_trp_meas_request_item.ie_exts_present                 = true;
    asn1_trp_meas_request_item.ie_exts.ao_a_search_win_present = true;

    if (std::holds_alternative<expected_ul_aoa_t>(trp_meas_request_item.aoa_search_win->angle_meas)) {
      const auto& expected_ul_aoa = std::get<expected_ul_aoa_t>(trp_meas_request_item.aoa_search_win->angle_meas);
      asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.set_expected_ul_ao_a();
      asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
          .expected_azimuth_ao_a.expected_azimuth_ao_a_value =
          expected_ul_aoa.expected_azimuth_aoa.expected_azimuth_aoa_value;
      asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
          .expected_azimuth_ao_a.expected_azimuth_ao_a_uncertainty =
          expected_ul_aoa.expected_azimuth_aoa.expected_azimuth_aoa_uncertainty;

      if (expected_ul_aoa.expected_zenith_aoa.has_value()) {
        asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a().expected_zenith_ao_a_present =
            true;
        asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
            .expected_zenith_ao_a.expected_zenith_ao_a_value =
            expected_ul_aoa.expected_zenith_aoa->expected_zenith_aoa_value;
        asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_ul_ao_a()
            .expected_zenith_ao_a.expected_zenith_ao_a_uncertainty =
            expected_ul_aoa.expected_zenith_aoa->expected_zenith_aoa_uncertainty;
      }
    } else {
      const auto& expected_zoa_only = std::get<expected_zoa_only_t>(trp_meas_request_item.aoa_search_win->angle_meas);
      asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.set_expected_zo_a();
      asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_zo_a()
          .expected_zo_a_only.expected_zenith_ao_a_value =
          expected_zoa_only.expected_zoa_only.expected_zenith_aoa_value;
      asn1_trp_meas_request_item.ie_exts.ao_a_search_win.angle_meas.expected_zo_a()
          .expected_zo_a_only.expected_zenith_ao_a_uncertainty =
          expected_zoa_only.expected_zoa_only.expected_zenith_aoa_uncertainty;
    }
  }

  // > Fill number of TRP RX TEG.
  if (trp_meas_request_item.nof_trp_rx_teg.has_value()) {
    asn1_trp_meas_request_item.ie_exts_present                = true;
    asn1_trp_meas_request_item.ie_exts.nof_trp_rx_teg_present = true;
    asn1::number_to_enum(asn1_trp_meas_request_item.ie_exts.nof_trp_rx_teg,
                         trp_meas_request_item.nof_trp_rx_teg.value());
  }

  // > Fill number of TRP RX TX TEG.
  if (trp_meas_request_item.nof_trp_rx_tx_teg.has_value()) {
    asn1_trp_meas_request_item.ie_exts_present                   = true;
    asn1_trp_meas_request_item.ie_exts.nof_trp_rx_tx_teg_present = true;
    asn1::number_to_enum(asn1_trp_meas_request_item.ie_exts.nof_trp_rx_tx_teg,
                         trp_meas_request_item.nof_trp_rx_tx_teg.value());
  }

  return asn1_trp_meas_request_item;
}

inline std::optional<trp_meas_quantities_list_item_t> asn1_to_trp_meas_quantities_list_item(
    const asn1::nrppa::trp_meas_quantities_list_item_s& asn1_trp_meas_quantities_list_item)
{
  trp_meas_quantities_list_item_t trp_meas_quantities_list_item;

  switch (asn1_trp_meas_quantities_list_item.trp_meas_quantities_item) {
    case asn1::nrppa::trp_meas_type_e::gnb_rx_tx_time_diff:
      trp_meas_quantities_list_item.trp_meas_quantities_item = trp_meas_quantities_item_t::gnb_rx_tx_time_diff;
      break;
    case asn1::nrppa::trp_meas_type_e::ul_srs_rsrp:
      trp_meas_quantities_list_item.trp_meas_quantities_item = trp_meas_quantities_item_t::ul_srs_rsrp;
      break;
    case asn1::nrppa::trp_meas_type_e::ul_ao_a:
      trp_meas_quantities_list_item.trp_meas_quantities_item = trp_meas_quantities_item_t::ul_aoa;
      break;
    case asn1::nrppa::trp_meas_type_e::ul_rtoa:
      trp_meas_quantities_list_item.trp_meas_quantities_item = trp_meas_quantities_item_t::ul_rtoa;
      break;
    case asn1::nrppa::trp_meas_type_e::multiple_ul_ao_a:
      trp_meas_quantities_list_item.trp_meas_quantities_item = trp_meas_quantities_item_t::multiple_ul_aoa;
      break;
    case asn1::nrppa::trp_meas_type_e::ul_srs_rsrp_p:
      trp_meas_quantities_list_item.trp_meas_quantities_item = trp_meas_quantities_item_t::ul_srs_rsrp_p;
      break;
    default:
      ocudulog::fetch_basic_logger("NRPPA").debug(
          "Unsupported NRPPa TRP meas quantities item ({}). Ignoring it",
          asn1_trp_meas_quantities_list_item.trp_meas_quantities_item.to_string());
      return std::nullopt;
  }

  if (asn1_trp_meas_quantities_list_item.timing_report_granularity_factor_present) {
    trp_meas_quantities_list_item.timing_report_granularity_factor =
        asn1_trp_meas_quantities_list_item.timing_report_granularity_factor;
  }

  return trp_meas_quantities_list_item;
}

inline asn1::nrppa::trp_meas_quantities_list_item_s
trp_meas_quantities_list_item_to_asn1(const trp_meas_quantities_list_item_t& trp_meas_quantities_list_item)
{
  asn1::nrppa::trp_meas_quantities_list_item_s asn1_trp_meas_quantities_list_item;

  switch (trp_meas_quantities_list_item.trp_meas_quantities_item) {
    case trp_meas_quantities_item_t::gnb_rx_tx_time_diff:
      asn1_trp_meas_quantities_list_item.trp_meas_quantities_item = asn1::nrppa::trp_meas_type_e::gnb_rx_tx_time_diff;
      break;
    case trp_meas_quantities_item_t::ul_srs_rsrp:
      asn1_trp_meas_quantities_list_item.trp_meas_quantities_item = asn1::nrppa::trp_meas_type_e::ul_srs_rsrp;
      break;
    case trp_meas_quantities_item_t::ul_aoa:
      asn1_trp_meas_quantities_list_item.trp_meas_quantities_item = asn1::nrppa::trp_meas_type_e::ul_ao_a;
      break;
    case trp_meas_quantities_item_t::ul_rtoa:
      asn1_trp_meas_quantities_list_item.trp_meas_quantities_item = asn1::nrppa::trp_meas_type_e::ul_rtoa;
      break;
    case trp_meas_quantities_item_t::multiple_ul_aoa:
      asn1_trp_meas_quantities_list_item.trp_meas_quantities_item = asn1::nrppa::trp_meas_type_e::multiple_ul_ao_a;
      break;
    case trp_meas_quantities_item_t::ul_srs_rsrp_p:
      asn1_trp_meas_quantities_list_item.trp_meas_quantities_item = asn1::nrppa::trp_meas_type_e::ul_srs_rsrp_p;
      break;
    default:
      report_fatal_error("Cannot convert NRPPa TRP meas quantities item to ASN.1");
  }

  if (trp_meas_quantities_list_item.timing_report_granularity_factor.has_value()) {
    asn1_trp_meas_quantities_list_item.timing_report_granularity_factor_present = true;
    asn1_trp_meas_quantities_list_item.timing_report_granularity_factor =
        trp_meas_quantities_list_item.timing_report_granularity_factor.value();
  }

  return asn1_trp_meas_quantities_list_item;
}

} // namespace ocudu::ocucp
