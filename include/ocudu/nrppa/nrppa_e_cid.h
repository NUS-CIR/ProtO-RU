// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/arfcn.h"
#include "ocudu/ran/cause/nrppa_cause.h"
#include "ocudu/ran/crit_diagnostics.h"
#include "ocudu/ran/nr_cgi.h"
#include "ocudu/ran/pci.h"
#include "ocudu/ran/positioning/common.h"
#include "ocudu/ran/positioning/e_cid_measurement.h"
#include "ocudu/ran/positioning/positioning_ids.h"
#include "ocudu/ran/tac.h"
#include <optional>
#include <variant>
#include <vector>

namespace ocudu::ocucp {

enum class nrppa_meas_quantities_value {
  cell_id,
  angle_of_arrival,
  timing_advance_type1,
  timing_advance_type2,
  rsrp,
  rsrq,
  ss_rsrp,
  ss_rsrq,
  csi_rsrp,
  csi_rsrq,
  angle_of_arrival_nr,
  timing_advance_nr,
  ue_rx_tx_time_diff
};

struct nrppa_meas_quantities_item {
  nrppa_meas_quantities_value meas_quantities_value;
};

struct nrppa_e_cid_meas_initiation_request {
  lmf_ue_meas_id_t                  lmf_ue_meas_id;
  report_characteristics_t          report_characteristics;
  std::optional<meas_periodicity_t> meas_periodicity;
  /// NR Angle of Arrival has its own reporting periodicity, as per TS 38.455 section 9.1.1.1.
  std::optional<meas_periodicity_nr_aoa_t> meas_periodicity_nr_aoa;
  std::vector<nrppa_meas_quantities_item>  meas_quantities;
};

struct nrppa_result_ss_rsrp_per_ssb_item {
  uint8_t ssb_idx;
  uint8_t value_ss_rsrp;
};

struct nrppa_result_ss_rsrp_item {
  pci_t                                          nr_pci;
  arfcn_t                                        nr_arfcn;
  std::optional<nr_cell_global_id_t>             cgi_nr;
  std::optional<uint8_t>                         value_ss_rsrp_cell;
  std::vector<nrppa_result_ss_rsrp_per_ssb_item> ss_rsrp_per_ssb;
};

struct nrppa_result_ss_rsrq_per_ssb_item {
  uint8_t ssb_idx;
  uint8_t value_ss_rsrq;
};

struct nrppa_result_ss_rsrq_item {
  pci_t                                          nr_pci;
  arfcn_t                                        nr_arfcn;
  std::optional<nr_cell_global_id_t>             cgi_nr;
  std::optional<uint8_t>                         value_ss_rsrq_cell;
  std::vector<nrppa_result_ss_rsrq_per_ssb_item> ss_rsrq_per_ssb;
};

struct nrppa_result_csi_rsrp_per_csi_rs_item {
  uint8_t csi_rs_idx;
  uint8_t value_csi_rsrp;
};

struct nrppa_result_csi_rsrp_item {
  pci_t                                              nr_pci;
  arfcn_t                                            nr_arfcn;
  std::optional<nr_cell_global_id_t>                 cgi_nr;
  std::optional<uint8_t>                             value_csi_rsrp_cell;
  std::vector<nrppa_result_csi_rsrp_per_csi_rs_item> csi_rsrp_per_csi_rs;
};

struct nrppa_result_csi_rsrq_per_csi_rs_item {
  uint8_t csi_rs_idx;
  uint8_t value_csi_rsrq;
};

struct nrppa_result_csi_rsrq_item {
  pci_t                                              nr_pci;
  arfcn_t                                            nr_arfcn;
  std::optional<nr_cell_global_id_t>                 cgi_nr;
  std::optional<uint8_t>                             value_csi_rsrq_cell;
  std::vector<nrppa_result_csi_rsrq_per_csi_rs_item> csi_rsrq_per_csi_rs;
};

struct nrppa_ul_aoa {
  uint16_t                                azimuth_aoa;
  std::optional<uint16_t>                 zenith_aoa;
  std::optional<lcs_to_gcs_translation_t> lcs_to_gcs_translation;
};

enum class nrppa_nr_tadv : uint16_t { min = 0, max = 7690 };

using nrppa_measured_results_value = std::variant<std::vector<nrppa_result_ss_rsrp_item>,
                                                  std::vector<nrppa_result_ss_rsrq_item>,
                                                  std::vector<nrppa_result_csi_rsrp_item>,
                                                  std::vector<nrppa_result_csi_rsrq_item>,
                                                  nrppa_ul_aoa,
                                                  nrppa_nr_tadv>;

struct nrppa_e_cid_meas_result {
  nr_cell_global_id_t                           serving_cell_id;
  tac_t                                         serving_cell_tac;
  std::optional<ng_ran_access_point_position_t> ng_ran_access_point_position;
  std::vector<nrppa_measured_results_value>     measured_results;
};

struct nrppa_e_cid_meas_initiation_response {
  lmf_ue_meas_id_t lmf_ue_meas_id;
  ran_ue_meas_id_t ran_ue_meas_id;
  // The Measured Results IE shall be included in the E-CID Measurement Result IE of the E-CID MEASUREMENT INITIATION
  // RESPONSE message when the Report Characteristics IE is set to "OnDemand" and measurement results other than the
  // "Cell-ID" have been requested, see TS 38.455.
  std::optional<nrppa_e_cid_meas_result> e_cid_meas_result;
  std::optional<crit_diagnostics_t>      crit_diagnostics;
};

struct nrppa_e_cid_meas_initiation_failure {
  lmf_ue_meas_id_t                  lmf_ue_meas_id;
  nrppa_cause_t                     cause;
  std::optional<crit_diagnostics_t> crit_diagnostics;
};

struct nrppa_e_cid_meas_failure_indication {
  lmf_ue_meas_id_t lmf_ue_meas_id;
  ran_ue_meas_id_t ran_ue_meas_id;
  nrppa_cause_t    cause;
};

struct nrppa_e_cid_meas_report {
  lmf_ue_meas_id_t        lmf_ue_meas_id;
  ran_ue_meas_id_t        ran_ue_meas_id;
  nrppa_e_cid_meas_result e_cid_meas_result;
};

struct nrppa_e_cid_meas_termination_command {
  lmf_ue_meas_id_t lmf_ue_meas_id;
  ran_ue_meas_id_t ran_ue_meas_id;
};

} // namespace ocudu::ocucp

namespace fmt {

// nrppa_meas_quantities_value formatter.
template <>
struct formatter<ocudu::ocucp::nrppa_meas_quantities_value> {
  template <typename ParseContext>
  auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ocudu::ocucp::nrppa_meas_quantities_value& value, FormatContext& ctx) const
  {
    switch (value) {
      case ocudu::ocucp::nrppa_meas_quantities_value::cell_id:
        return format_to(ctx.out(), "cell_id");
      case ocudu::ocucp::nrppa_meas_quantities_value::angle_of_arrival:
        return format_to(ctx.out(), "angle_of_arrival");
      case ocudu::ocucp::nrppa_meas_quantities_value::timing_advance_type1:
        return format_to(ctx.out(), "timing_advance_type1");
      case ocudu::ocucp::nrppa_meas_quantities_value::timing_advance_type2:
        return format_to(ctx.out(), "timing_advance_type2");
      case ocudu::ocucp::nrppa_meas_quantities_value::rsrp:
        return format_to(ctx.out(), "rsrp");
      case ocudu::ocucp::nrppa_meas_quantities_value::rsrq:
        return format_to(ctx.out(), "rsrq");
      case ocudu::ocucp::nrppa_meas_quantities_value::ss_rsrp:
        return format_to(ctx.out(), "ss_rsrp");
      case ocudu::ocucp::nrppa_meas_quantities_value::ss_rsrq:
        return format_to(ctx.out(), "ss_rsrq");
      case ocudu::ocucp::nrppa_meas_quantities_value::csi_rsrp:
        return format_to(ctx.out(), "csi_rsrp");
      case ocudu::ocucp::nrppa_meas_quantities_value::csi_rsrq:
        return format_to(ctx.out(), "csi_rsrq");
      case ocudu::ocucp::nrppa_meas_quantities_value::angle_of_arrival_nr:
        return format_to(ctx.out(), "angle_of_arrival_nr");
      case ocudu::ocucp::nrppa_meas_quantities_value::timing_advance_nr:
        return format_to(ctx.out(), "timing_advance_nr");
      case ocudu::ocucp::nrppa_meas_quantities_value::ue_rx_tx_time_diff:
        return format_to(ctx.out(), "ue_rx_tx_time_diff");
    }
    return format_to(ctx.out(), "unknown");
  }
};

} // namespace fmt
