// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/byte_buffer.h"
#include "ocudu/asn1/nrppa/nrppa_ies.h"
#include "ocudu/nrppa/nrppa_e_cid.h"
#include "ocudu/ran/positioning/measurement_information.h"
#include "ocudu/ran/positioning/positioning_ids.h"

namespace ocudu::ocucp {

/// \brief Generate a valid E-CID measurement initiation request.
byte_buffer generate_valid_nrppa_e_cid_measurement_initiation_request(
    lmf_ue_meas_id_t                               lmf_ue_meas_id,
    const std::vector<nrppa_meas_quantities_item>& meas_quantities = {
        {nrppa_meas_quantities_item{nrppa_meas_quantities_value::ss_rsrp}}});

/// \brief Generate a valid E-CID measurement initiation request with periodic reports.
byte_buffer
generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_reports(lmf_ue_meas_id_t lmf_ue_meas_id,
                                                                                bool             request_aoa = false);

/// \brief Generate a valid E-CID measurement initiation request asking for periodic NR Angle of Arrival reports.
byte_buffer generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_aoa(
    lmf_ue_meas_id_t lmf_ue_meas_id,
    /// Quantities the LMF requests beside the NR Angle of Arrival.
    const std::vector<asn1::nrppa::meas_quantities_value_opts::options>& other_quantities = {});

/// \brief Generate a valid E-CID measurement termination command.
byte_buffer generate_valid_nrppa_e_cid_measurement_termination_command(lmf_ue_meas_id_t lmf_ue_meas_id,
                                                                       ran_ue_meas_id_t ran_ue_meas_id);

/// \brief Generate a valid TRP information request.
byte_buffer generate_valid_trp_information_request();

/// \brief Generate a valid positioning information request.
byte_buffer generate_valid_positioning_information_request();

/// \brief Generate a valid positioning activation request.
byte_buffer generate_valid_positioning_activation_request();

/// \brief Generate a valid NRPPa measurement request.
byte_buffer generate_valid_nrppa_measurement_request(
    lmf_meas_id_t                               lmf_meas_id,
    const std::vector<trp_meas_request_item_t>& trp_meas_request_list = {{trp_meas_request_item_t{trp_id_t::min}}},
    const std::vector<trp_meas_quantities_list_item_t>& trp_meas_quantities = {{trp_meas_quantities_item_t::ul_rtoa}});

} // namespace ocudu::ocucp
