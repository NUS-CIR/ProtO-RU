// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/cause/f1ap_cause.h"
#include "ocudu/ran/crit_diagnostics.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/ran/positioning/common.h"
#include "ocudu/ran/positioning/measurement_information.h"
#include "ocudu/ran/positioning/positioning_ids.h"
#include "ocudu/ran/positioning/trp_information_exchange.h"
#include <optional>
#include <vector>

namespace ocudu::ocucp {

/// E-CID Measurement Quantities Item, as per TS 38.473, Section 9.2.12.20.
enum class e_cid_meas_quantities_item_t { default_quantity, nr_angle_of_arrival };

/// Measurement Periodicity NR-AoA, as per TS 38.473, Section 9.2.12.20.
///
/// \remark NR Angle of Arrival has its own reporting periodicity. The E-CID Measurement Periodicity IE does not apply
/// to it.
enum class meas_periodicity_nr_aoa_t : uint32_t {
  ms160     = 160,
  ms320     = 320,
  ms640     = 640,
  ms1280    = 1280,
  ms2560    = 2560,
  ms5120    = 5120,
  ms10240   = 10240,
  ms20480   = 20480,
  ms40960   = 40960,
  ms61440   = 61440,
  ms81920   = 81920,
  ms368640  = 368640,
  ms737280  = 737280,
  ms1843200 = 1843200,
};

/// E-CID MEASUREMENT INITIATION REQUEST, as per TS 38.473, Section 9.2.12.20.
struct e_cid_measurement_request_t {
  cu_cp_ue_index_t                          ue_index;
  lmf_ue_meas_id_t                          lmf_ue_meas_id;
  ran_ue_meas_id_t                          ran_ue_meas_id;
  report_characteristics_t                  report_characteristics;
  std::vector<e_cid_meas_quantities_item_t> meas_quantities;
  std::optional<meas_periodicity_t>         meas_periodicity;
  std::optional<meas_periodicity_nr_aoa_t>  meas_periodicity_nr_aoa;
};

/// E-CID Measurement Result, as per TS 38.473, Section 9.3.1.199.
struct e_cid_measurement_result_t {
  /// Configured estimated geographical position of the antenna of the cell.
  std::optional<geographical_coordinates_t> geo_coords;
  std::vector<ul_angle_of_arrival_t>        measured_results;
};

/// E-CID MEASUREMENT INITIATION RESPONSE, as per TS 38.473, Section 9.2.12.21.
struct e_cid_measurement_response_t {
  lmf_ue_meas_id_t lmf_ue_meas_id;
  ran_ue_meas_id_t ran_ue_meas_id;
  /// Absent when the E-CID Report Characteristics IE is set to "Periodic", as per TS 38.473, Section 8.13.12.2.
  std::optional<e_cid_measurement_result_t> e_cid_meas_result;
  std::optional<uint16_t>                   cell_portion_id;
};

/// E-CID MEASUREMENT REPORT, as per TS 38.473, Section 9.2.12.24.
struct e_cid_measurement_report_t {
  cu_cp_ue_index_t           ue_index;
  lmf_ue_meas_id_t           lmf_ue_meas_id;
  ran_ue_meas_id_t           ran_ue_meas_id;
  e_cid_measurement_result_t e_cid_meas_result;
  std::optional<uint16_t>    cell_portion_id;
};

/// E-CID MEASUREMENT FAILURE INDICATION, as per TS 38.473, Section 9.2.12.23.
struct e_cid_measurement_failure_indication_t {
  cu_cp_ue_index_t ue_index;
  lmf_ue_meas_id_t lmf_ue_meas_id;
  ran_ue_meas_id_t ran_ue_meas_id;
  f1ap_cause_t     cause;
};

/// E-CID MEASUREMENT TERMINATION COMMAND, as per TS 38.473, Section 9.2.12.25.
struct e_cid_measurement_termination_command_t {
  cu_cp_ue_index_t ue_index;
  lmf_ue_meas_id_t lmf_ue_meas_id;
  ran_ue_meas_id_t ran_ue_meas_id;
};

/// E-CID MEASUREMENT INITIATION FAILURE, as per TS 38.473, Section 9.2.12.22.
struct e_cid_measurement_failure_t {
  lmf_ue_meas_id_t                  lmf_ue_meas_id;
  ran_ue_meas_id_t                  ran_ue_meas_id;
  f1ap_cause_t                      cause;
  std::optional<crit_diagnostics_t> crit_diagnostics;
};

} // namespace ocudu::ocucp
