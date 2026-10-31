// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "nrppa_asn1_converters.h"
#include "ocudu/asn1/nrppa/nrppa.h"
#include "ocudu/asn1/nrppa/nrppa_pdu_contents.h"
#include "ocudu/nrppa/nrppa.h"
#include "ocudu/nrppa/nrppa_otdoa.h"
#include "ocudu/ran/positioning/measurement_information.h"

namespace ocudu::ocucp {

/// \brief Fills the common type \c nrppa_e_cid_meas_initiation_request struct.
/// \param[out] request The common type \c nrppa_e_cid_meas_initiation_request struct to fill.
/// \param[in] asn1_request The ASN.1 type E-CIDMeasInitiationRequest.
inline void fill_nrppa_e_cid_meas_initiation_request(nrppa_e_cid_meas_initiation_request&                 request,
                                                     const asn1::nrppa::e_c_id_meas_initiation_request_s& asn1_request)
{
  // Fill LMF UE meas ID.
  request.lmf_ue_meas_id = uint_to_lmf_ue_meas_id(asn1_request->lmf_ue_meas_id);

  // Fill report characteristics.
  if (asn1_request->report_characteristics == asn1::nrppa::report_characteristics_opts::on_demand) {
    request.report_characteristics = report_characteristics_t::on_demand;
  } else {
    request.report_characteristics = report_characteristics_t::periodic;
  }

  // Fill meas periodicity.
  if (asn1_request->meas_periodicity_present) {
    request.meas_periodicity = asn1_to_meas_periodicity(asn1_request->meas_periodicity);
  }

  // Fill meas quantities.
  for (const auto& asn1_meas_quantities_item : asn1_request->meas_quantities) {
    request.meas_quantities.push_back(asn1_to_meas_quantities_item(asn1_meas_quantities_item->meas_quantities_item()));
  }
}

/// \brief Fills the common type \c nrppa_otdoa_information_request struct.
/// \param[out] request The common type \c nrppa_otdoa_information_request struct to fill.
/// \param[in] asn1_request The ASN.1 type OTDOAInfoRequest.
inline void fill_nrppa_otdoa_information_request(nrppa_otdoa_information_request&         request,
                                                 const asn1::nrppa::otdoa_info_request_s& asn1_request)
{
  for (const auto& asn1_info_type_item : asn1_request->otdoa_info_type_group) {
    if (asn1_info_type_item.value().otdoa_info_type_item().otdoa_info_item.value !=
        asn1::nrppa::otdoa_info_item_opts::nulltype) {
      request.otdoa_info_type.push_back({static_cast<nrppa_otdoa_info_item>(
          asn1_info_type_item.value().otdoa_info_type_item().otdoa_info_item.value)});
    }
  }
}

static nrppa_result_ss_rsrp_item
fill_meas_result_ss_rsrp_item(const cell_measurement_positioning_info::cell_measurement_item_t& cell_meas_item)
{
  nrppa_result_ss_rsrp_item meas_result_rsrp_item;
  meas_result_rsrp_item.nr_pci             = cell_meas_item.meas_result.pci.value();
  meas_result_rsrp_item.nr_arfcn           = cell_meas_item.nr_arfcn;
  meas_result_rsrp_item.cgi_nr             = cell_meas_item.nr_cgi;
  meas_result_rsrp_item.value_ss_rsrp_cell = cell_meas_item.meas_result.cell_results.results_ssb_cell.value().rsrp;

  if (cell_meas_item.meas_result.rs_idx_results.has_value()) {
    for (const auto& ssb_idx_result : cell_meas_item.meas_result.rs_idx_results.value().results_ssb_idxes) {
      nrppa_result_ss_rsrp_per_ssb_item result_per_ssb_item;
      result_per_ssb_item.ssb_idx = ssb_idx_result.ssb_idx;
      if (ssb_idx_result.ssb_results.has_value() && ssb_idx_result.ssb_results.value().rsrp.has_value()) {
        result_per_ssb_item.value_ss_rsrp = ssb_idx_result.ssb_results.value().rsrp.value();
      }

      meas_result_rsrp_item.ss_rsrp_per_ssb.push_back(result_per_ssb_item);
    }
  }

  return meas_result_rsrp_item;
}

static nrppa_result_csi_rsrp_item
fill_meas_result_csi_rsrp_item(const cell_measurement_positioning_info::cell_measurement_item_t& cell_meas_item)
{
  nrppa_result_csi_rsrp_item meas_result_rsrp_item;
  meas_result_rsrp_item.nr_pci              = cell_meas_item.meas_result.pci.value();
  meas_result_rsrp_item.nr_arfcn            = cell_meas_item.nr_arfcn;
  meas_result_rsrp_item.cgi_nr              = cell_meas_item.nr_cgi;
  meas_result_rsrp_item.value_csi_rsrp_cell = cell_meas_item.meas_result.cell_results.results_csi_rs_cell.value().rsrp;

  if (cell_meas_item.meas_result.rs_idx_results.has_value()) {
    for (const auto& csi_rs_result : cell_meas_item.meas_result.rs_idx_results.value().results_csi_rs_idxes) {
      nrppa_result_csi_rsrp_per_csi_rs_item result_per_csi_rs;
      result_per_csi_rs.csi_rs_idx = csi_rs_result.csi_rs_idx;
      if (csi_rs_result.csi_rs_results.has_value() && csi_rs_result.csi_rs_results.value().rsrp.has_value()) {
        result_per_csi_rs.value_csi_rsrp = csi_rs_result.csi_rs_results.value().rsrp.value();
      }

      meas_result_rsrp_item.csi_rsrp_per_csi_rs.push_back(result_per_csi_rs);
    }
  }

  return meas_result_rsrp_item;
}

static nrppa_result_ss_rsrq_item
fill_meas_result_ss_rsrq_item(const cell_measurement_positioning_info::cell_measurement_item_t& cell_meas_item)
{
  nrppa_result_ss_rsrq_item meas_result_rsrq_item;
  meas_result_rsrq_item.nr_pci             = cell_meas_item.meas_result.pci.value();
  meas_result_rsrq_item.nr_arfcn           = cell_meas_item.nr_arfcn;
  meas_result_rsrq_item.cgi_nr             = cell_meas_item.nr_cgi;
  meas_result_rsrq_item.value_ss_rsrq_cell = cell_meas_item.meas_result.cell_results.results_ssb_cell.value().rsrq;

  if (cell_meas_item.meas_result.rs_idx_results.has_value()) {
    for (const auto& ssb_idx_result : cell_meas_item.meas_result.rs_idx_results.value().results_ssb_idxes) {
      nrppa_result_ss_rsrq_per_ssb_item result_per_ssb_item;
      result_per_ssb_item.ssb_idx = ssb_idx_result.ssb_idx;
      if (ssb_idx_result.ssb_results.has_value() && ssb_idx_result.ssb_results.value().rsrq.has_value()) {
        result_per_ssb_item.value_ss_rsrq = ssb_idx_result.ssb_results.value().rsrq.value();
      }

      meas_result_rsrq_item.ss_rsrq_per_ssb.push_back(result_per_ssb_item);
    }
  }

  return meas_result_rsrq_item;
}

static nrppa_result_csi_rsrq_item
fill_meas_result_csi_rsrq_item(const cell_measurement_positioning_info::cell_measurement_item_t& cell_meas_item)
{
  nrppa_result_csi_rsrq_item meas_result_rsrq_item;
  meas_result_rsrq_item.nr_pci              = cell_meas_item.meas_result.pci.value();
  meas_result_rsrq_item.nr_arfcn            = cell_meas_item.nr_arfcn;
  meas_result_rsrq_item.cgi_nr              = cell_meas_item.nr_cgi;
  meas_result_rsrq_item.value_csi_rsrq_cell = cell_meas_item.meas_result.cell_results.results_csi_rs_cell.value().rsrq;

  if (cell_meas_item.meas_result.rs_idx_results.has_value()) {
    for (const auto& csi_rs_result : cell_meas_item.meas_result.rs_idx_results.value().results_csi_rs_idxes) {
      nrppa_result_csi_rsrq_per_csi_rs_item result_per_csi_rs;
      result_per_csi_rs.csi_rs_idx = csi_rs_result.csi_rs_idx;
      if (csi_rs_result.csi_rs_results.has_value() && csi_rs_result.csi_rs_results.value().rsrq.has_value()) {
        result_per_csi_rs.value_csi_rsrq = csi_rs_result.csi_rs_results.value().rsrq.value();
      }

      meas_result_rsrq_item.csi_rsrq_per_csi_rs.push_back(result_per_csi_rs);
    }
  }
  return meas_result_rsrq_item;
}

static nrppa_measured_results_value
fill_meas_results(const nrppa_meas_quantities_item&                                 meas_quantity,
                  const cell_measurement_positioning_info::cell_measurement_item_t& cell_meas_item)
{
  nrppa_measured_results_value meas_results_value;

  // Fill SS RSRP.
  if (meas_quantity.meas_quantities_value == nrppa_meas_quantities_value::ss_rsrp) {
    if (cell_meas_item.meas_result.cell_results.results_ssb_cell.has_value()) {
      nrppa_result_ss_rsrp_item              meas_result_rsrp_item = fill_meas_result_ss_rsrp_item(cell_meas_item);
      std::vector<nrppa_result_ss_rsrp_item> meas_results_rsrp;
      meas_results_rsrp.push_back(meas_result_rsrp_item);
      return meas_results_rsrp;
    }
  }

  // Fill CSI RSRP.
  if (meas_quantity.meas_quantities_value == nrppa_meas_quantities_value::csi_rsrp) {
    if (cell_meas_item.meas_result.cell_results.results_csi_rs_cell.has_value()) {
      nrppa_result_csi_rsrp_item              meas_result_rsrp_item = fill_meas_result_csi_rsrp_item(cell_meas_item);
      std::vector<nrppa_result_csi_rsrp_item> meas_results_rsrp;
      meas_results_rsrp.push_back(meas_result_rsrp_item);
      return meas_results_rsrp;
    }
  }

  // Fill SSRSRQ.
  if (meas_quantity.meas_quantities_value == nrppa_meas_quantities_value::ss_rsrq) {
    if (cell_meas_item.meas_result.cell_results.results_ssb_cell.has_value()) {
      nrppa_result_ss_rsrq_item              meas_result_rsrq_item = fill_meas_result_ss_rsrq_item(cell_meas_item);
      std::vector<nrppa_result_ss_rsrq_item> meas_results_rsrq;
      meas_results_rsrq.push_back(meas_result_rsrq_item);
      return meas_results_rsrq;
    }
  }

  // Fill CSI RSRQ.
  if (meas_quantity.meas_quantities_value == nrppa_meas_quantities_value::csi_rsrq) {
    if (cell_meas_item.meas_result.cell_results.results_csi_rs_cell.has_value()) {
      nrppa_result_csi_rsrq_item              meas_result_rsrq_item = fill_meas_result_csi_rsrq_item(cell_meas_item);
      std::vector<nrppa_result_csi_rsrq_item> meas_results_rsrq;
      meas_results_rsrq.push_back(meas_result_rsrq_item);
      return meas_results_rsrq;
    }
  }

  return meas_results_value;
}

/// \brief Returns true when the given measurement quantity is sourced from the RRC measurement reports.
inline bool is_rrc_sourced_meas_quantity(nrppa_meas_quantities_value quantity)
{
  return quantity == nrppa_meas_quantities_value::ss_rsrp or quantity == nrppa_meas_quantities_value::ss_rsrq or
         quantity == nrppa_meas_quantities_value::csi_rsrp or quantity == nrppa_meas_quantities_value::csi_rsrq;
}

/// \brief Returns true when the given measurement quantity is measured by the gNB-DU.
inline bool is_du_sourced_meas_quantity(nrppa_meas_quantities_value quantity)
{
  return quantity == nrppa_meas_quantities_value::angle_of_arrival_nr;
}

/// \brief Collects the measured results the RRC measurement reports provide.
/// \param[in] meas_results The measurement results of the UE.
/// \param[in] meas_quantities The measurement quantities the LMF requested.
/// \returns The measured results for the requested quantities.
inline std::vector<nrppa_measured_results_value>
fill_rrc_measured_results(const cell_measurement_positioning_info&       meas_results,
                          const std::vector<nrppa_meas_quantities_item>& meas_quantities)
{
  std::vector<nrppa_measured_results_value> measured_results;

  for (const auto& meas_quantity : meas_quantities) {
    // The Cell-ID quantity is answered by the Serving Cell ID IE alone, so it adds no measured result.
    if (meas_quantity.meas_quantities_value == nrppa_meas_quantities_value::cell_id or
        is_du_sourced_meas_quantity(meas_quantity.meas_quantities_value)) {
      continue;
    }

    if (!is_rrc_sourced_meas_quantity(meas_quantity.meas_quantities_value)) {
      if (meas_quantity.meas_quantities_value == nrppa_meas_quantities_value::rsrp ||
          meas_quantity.meas_quantities_value == nrppa_meas_quantities_value::rsrq) {
        ocudulog::fetch_basic_logger("NRPPA").info("Unsupported measurement quantity requested ({}). RSRP/RSRQ "
                                                   "without SS/CSI resource type is not supported",
                                                   meas_quantity.meas_quantities_value);
      } else {
        ocudulog::fetch_basic_logger("NRPPA").info("Unsupported measurement quantity requested ({})",
                                                   meas_quantity.meas_quantities_value);
      }
      continue;
    }

    for (const auto& cell_meas_result_item : meas_results.cell_measurements) {
      nrppa_measured_results_value meas_results_value = fill_meas_results(meas_quantity, cell_meas_result_item.second);

      // Don't append empty measurement results (e.g. when RSRP/RSRQ is requested but not available for the cell).
      if (std::holds_alternative<std::vector<nrppa_result_ss_rsrp_item>>(meas_results_value) &&
          std::get<std::vector<nrppa_result_ss_rsrp_item>>(meas_results_value).empty()) {
        continue;
      }
      if (std::holds_alternative<std::vector<nrppa_result_ss_rsrq_item>>(meas_results_value) &&
          std::get<std::vector<nrppa_result_ss_rsrq_item>>(meas_results_value).empty()) {
        continue;
      }
      if (std::holds_alternative<std::vector<nrppa_result_csi_rsrp_item>>(meas_results_value) &&
          std::get<std::vector<nrppa_result_csi_rsrp_item>>(meas_results_value).empty()) {
        continue;
      }
      if (std::holds_alternative<std::vector<nrppa_result_csi_rsrq_item>>(meas_results_value) &&
          std::get<std::vector<nrppa_result_csi_rsrq_item>>(meas_results_value).empty()) {
        continue;
      }

      measured_results.push_back(meas_results_value);
    }
  }

  return measured_results;
}

/// \brief Converts the measured results the gNB-DU reported over F1AP.
/// \param[in] du_meas_result The E-CID measurement result of the gNB-DU.
/// \returns The measured results in NRPPa common type.
inline std::vector<nrppa_measured_results_value>
fill_du_measured_results(const e_cid_measurement_result_t& du_meas_result)
{
  std::vector<nrppa_measured_results_value> measured_results;

  for (const ul_angle_of_arrival_t& aoa : du_meas_result.measured_results) {
    measured_results.push_back(nrppa_ul_aoa{aoa.azimuth_aoa, aoa.zenith_aoa, aoa.lcs_to_gcs_translation});
  }

  return measured_results;
}

/// \brief Fills the common type \c trp_information_request_t struct.
/// \param[out] request The common type \c trp_information_request_t struct to fill.
/// \param[in] asn1_request The ASN.1 type TRPInformationRequest.
inline void fill_trp_information_request(trp_information_request_t&             request,
                                         const asn1::nrppa::trp_info_request_s& asn1_request)
{
  // Fill TRP list.
  for (const auto& asn1_trp_item : asn1_request->trp_list) {
    request.trp_list.push_back(uint_to_trp_id(asn1_trp_item.trp_id));
  }

  // Fill TRP information type list TRP request.
  for (const auto& asn1_trp_info_type_item_container : asn1_request->trp_info_type_list_trp_req) {
    std::optional<trp_information_type_item_t> trp_info_type_item =
        asn1_to_trp_info_type_item(asn1_trp_info_type_item_container->trp_info_type_item());
    if (trp_info_type_item.has_value()) {
      request.trp_info_type_list_trp_req.push_back(trp_info_type_item.value());
    }
  }
}

/// \brief Fills the common type \c positioning_information_request_t struct.
/// \param[out] request The common type \c positioning_information_request_t struct to fill.
/// \param[in] asn1_request The ASN.1 type PositioningInformationRequest.
inline void fill_positioning_information_request(positioning_information_request_t&             request,
                                                 const asn1::nrppa::positioning_info_request_s& asn1_request)
{
  // Fill requested SRS TX characteristics.
  if (asn1_request->requested_srs_tx_characteristics_present) {
    request.requested_srs_tx_characteristics =
        asn1_to_requested_srs_tx_characteristics(asn1_request->requested_srs_tx_characteristics);
  }

  // Fill UE report info.
  if (asn1_request->ue_report_info_present) {
    ue_report_info_t report_info;
    report_info.report_amount = asn1_request->ue_report_info.report_amount.to_number();

    if (asn1_request->ue_report_info.report_interv ==
        asn1::nrppa::ue_report_info_s::report_interv_opts::options::none) {
      report_info.report_interv = 0;
    } else if (asn1_request->ue_report_info.report_interv ==
               asn1::nrppa::ue_report_info_s::report_interv_opts::options::one) {
      report_info.report_interv = 1;
    } else if (asn1_request->ue_report_info.report_interv ==
               asn1::nrppa::ue_report_info_s::report_interv_opts::options::two) {
      report_info.report_interv = 2;
    } else if (asn1_request->ue_report_info.report_interv ==
               asn1::nrppa::ue_report_info_s::report_interv_opts::options::four) {
      report_info.report_interv = 4;
    } else if (asn1_request->ue_report_info.report_interv ==
               asn1::nrppa::ue_report_info_s::report_interv_opts::options::eight) {
      report_info.report_interv = 8;
    } else if (asn1_request->ue_report_info.report_interv ==
               asn1::nrppa::ue_report_info_s::report_interv_opts::options::ten) {
      report_info.report_interv = 10;
    } else if (asn1_request->ue_report_info.report_interv ==
               asn1::nrppa::ue_report_info_s::report_interv_opts::options::sixteen) {
      report_info.report_interv = 16;
    } else if (asn1_request->ue_report_info.report_interv ==
               asn1::nrppa::ue_report_info_s::report_interv_opts::options::twenty) {
      report_info.report_interv = 20;
    } else if (asn1_request->ue_report_info.report_interv ==
               asn1::nrppa::ue_report_info_s::report_interv_opts::options::thirty_two) {
      report_info.report_interv = 32;
    } else {
      report_info.report_interv = 64;
    }

    request.ue_report_info = report_info;
  }

  // Fill UE TEG info request.
  if (asn1_request->ue_teg_info_request_present) {
    ue_teg_info_request_t info_request;
    if (asn1_request->ue_teg_info_request == asn1::nrppa::ue_teg_info_request_opts::options::on_demand) {
      info_request = ue_teg_info_request_t::on_demand;
    } else if (asn1_request->ue_teg_info_request == asn1::nrppa::ue_teg_info_request_opts::options::periodic) {
      info_request = ue_teg_info_request_t::periodic;
    } else {
      info_request = ue_teg_info_request_t::stop;
    }
    request.ue_teg_info_request = info_request;
  }

  // Fill UE TEG report periodicity.
  if (asn1_request->ue_teg_report_periodicity_present) {
    request.ue_teg_report_periodicity = asn1_request->ue_teg_report_periodicity.to_number();
  }
}

/// \brief Fills the common type \c positioning_activation_request_t struct.
/// \param[out] request The common type \c positioning_activation_request_t struct to fill.
/// \param[in] asn1_request The ASN.1 type PositioningActivationRequest.
inline void fill_positioning_activation_request(positioning_activation_request_t&                    request,
                                                const asn1::nrppa::positioning_activation_request_s& asn1_request)
{
  // Fill SRS type.
  if (asn1_request->srs_type.type() == asn1::nrppa::srs_type_c::types_opts::options::semipersistent_srs) {
    const asn1::nrppa::semipersistent_srs_s asn1_srs_type = asn1_request->srs_type.semipersistent_srs();
    semipersistent_srs_t                    srs_type;
    srs_type.srs_res_set_id = asn1_srs_type.srs_res_set_id;
    if (asn1_srs_type.ie_exts_present) {
      if (asn1_srs_type.ie_exts.srs_spatial_relation_present) {
        spatial_relation_info_t spatial_relation_info;
        for (const auto& asn1_spatial_relation_for_res_id_item :
             asn1_srs_type.ie_exts.srs_spatial_relation.spatial_relationfor_res_id) {
          spatial_relation_info_t::reference_signal ref_sig =
              asn1_to_ref_sig(asn1_spatial_relation_for_res_id_item.ref_sig);
          spatial_relation_info.reference_signals.push_back(ref_sig);
        }
        srs_type.srs_spatial_relation = spatial_relation_info;
      }
      if (asn1_srs_type.ie_exts.srs_spatial_relation_per_srs_res_present) {
        spatial_relation_info_t spatial_relation_info;
        for (const auto& asn1_spatial_relation_per_srs_res_item :
             asn1_srs_type.ie_exts.srs_spatial_relation_per_srs_res.spatial_relation_per_srs_res_list) {
          spatial_relation_info_t::reference_signal ref_sig =
              asn1_to_ref_sig(asn1_spatial_relation_per_srs_res_item.ref_sig);
          spatial_relation_info.reference_signals.push_back(ref_sig);
        }

        srs_type.srs_spatial_relation_per_srs_res = spatial_relation_info;
      }
    }
    request.srs_type = srs_type;
  } else {
    const asn1::nrppa::aperiodic_srs_s asn1_srs_type = asn1_request->srs_type.aperiodic_srs();
    aperiodic_srs_t                    srs_type;
    srs_type.aperiodic = asn1_srs_type.aperiodic == asn1::nrppa::aperiodic_srs_s::aperiodic_opts::options::true_value;
    if (asn1_srs_type.srs_res_trigger_present) {
      srs_res_trigger_t srs_res_trigger;
      for (const auto& asn1_aperiodic_srs_res_trigger : asn1_srs_type.srs_res_trigger.aperiodic_srs_res_trigger_list) {
        srs_res_trigger.aperiodic_srs_res_trigger_list.push_back(asn1_aperiodic_srs_res_trigger);
      }
      srs_type.srs_res_trigger = srs_res_trigger;
    }
    request.srs_type = srs_type;
  }

  // Fill activation time.
  if (asn1_request->activation_time_present) {
    request.activation_time = asn1_request->activation_time.to_number();
  }
}

/// \brief Fills the common type \c measurement_request_t struct.
/// \param[out] request The common type \c measurement_request_t struct to fill.
/// \param[in] asn1_request The ASN.1 type MeasurementRequest.
inline void fill_measurement_request(measurement_request_t& request, const asn1::nrppa::meas_request_s& asn1_request)
{
  // Fill LMF meas ID.
  request.lmf_meas_id = uint_to_lmf_meas_id(asn1_request->lmf_meas_id);

  // Fill TRP meas request list.
  for (const auto& asn1_trp_meas_request_item : asn1_request->trp_meas_request_list) {
    request.trp_meas_request_list.push_back(asn1_to_trp_meas_request_item(asn1_trp_meas_request_item));
  }

  // Fill report characteristics.
  if (asn1_request->report_characteristics == asn1::nrppa::report_characteristics_opts::on_demand) {
    request.report_characteristics = report_characteristics_t::on_demand;
  } else {
    request.report_characteristics = report_characteristics_t::periodic;
  }

  // Fill meas periodicity.
  if (asn1_request->meas_periodicity_present) {
    request.meas_periodicity = asn1_to_meas_periodicity(asn1_request->meas_periodicity);
  }

  // Fill TRP meas quantities.
  for (const auto& asn1_trp_meas_quantities_item : asn1_request->trp_meas_quantities) {
    std::optional<trp_meas_quantities_list_item_t> trp_meas_quantities_list_item =
        asn1_to_trp_meas_quantities_list_item(asn1_trp_meas_quantities_item);
    if (trp_meas_quantities_list_item.has_value()) {
      request.trp_meas_quantities.push_back(trp_meas_quantities_list_item.value());
    }
  }

  // Fill SFN Initialization time.
  if (asn1_request->sfn_initisation_time_present) {
    request.sfn_initialization_time = asn1_request->sfn_initisation_time.to_number();
  }

  // Fill SRS configuration.
  if (asn1_request->srs_cfg_present) {
    request.srs_config = asn1_to_srs_configuration(asn1_request->srs_cfg);
  }

  // Fill measurement beam info request.
  if (asn1_request->meas_beam_info_request_present) {
    request.meas_beam_info_request =
        asn1_request->meas_beam_info_request == asn1::nrppa::meas_beam_info_request_opts::options::true_value;
  }

  // Fill system frame number.
  if (asn1_request->sys_frame_num_present) {
    request.sys_frame_num = asn1_request->sys_frame_num;
  }

  // Fill slot number.
  if (asn1_request->slot_num_present) {
    request.slot_num = asn1_request->slot_num;
  }

  // Fill measurement periodicity extended.
  if (asn1_request->meas_periodicity_extended_present) {
    request.meas_periodicity_extended = asn1_request->meas_periodicity_extended.to_number();
  }

  // Fill response time.
  if (asn1_request->resp_time_present) {
    response_time_t response_time;
    response_time.time      = asn1_request->resp_time.time;
    response_time.time_unit = response_time_t::time_unit_t((uint8_t)asn1_request->resp_time.time_unit.value);
    request.resp_time       = response_time;
  }

  // Fill measurement characteristics request indication.
  if (asn1_request->meas_characteristics_request_ind_present) {
    request.meas_characteristics_request_ind = asn1_request->meas_characteristics_request_ind.to_number();
  }

  // Fill measurement time occasion.
  if (asn1_request->meas_time_occasion_present) {
    request.meas_time_occasion = asn1_request->meas_time_occasion.to_number();
  }

  // Fill measurement amount.
  if (asn1_request->meas_amount_present) {
    request.meas_amount = asn1_request->meas_amount.to_number();
  }
}

} // namespace ocudu::ocucp
