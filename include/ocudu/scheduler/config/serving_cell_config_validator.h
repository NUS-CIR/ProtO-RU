// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/tdd/tdd_ul_dl_config.h"
#include "ocudu/scheduler/config/bwp_configuration.h"
#include "ocudu/scheduler/config/pucch_resource_builder_params.h"
#include "ocudu/scheduler/config/serving_cell_config.h"
#include "ocudu/support/config/validator_result.h"

namespace ocudu {
namespace config_validators {

/// \brief Validates PDCCH Config in \c sched_ue_creation_request_message used to create a UE.
/// \param[in] ue_cell_cfg UE serving cell configuration to be validated.
/// \param[in] dl_cfg_common Cell DL common configuration.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
validator_result validate_pdcch_cfg(const serving_cell_config& ue_cell_cfg, const dl_config_common& dl_cfg_common);

/// \brief Validates PDSCH Config in \c sched_ue_creation_request_message used to create a UE.
/// \param[in] ue_cell_cfg UE serving cell configuration to be validated.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
validator_result validate_pdsch_cfg(const serving_cell_config& ue_cell_cfg);

/// \brief Validates PUCCH Config in \c sched_ue_creation_request_message used to create a UE.
/// \param[in] ue_cell_cfg UE serving cell configuration to be validated.
/// \param[in] res_params Parameters used to build the PUCCH resources for the UE.
/// \param[in] cell_pucch_res_list Cell PUCCH resource list.
/// \param[in] pucch_cfg_common PUCCH commmon configuration.
/// \param[in] nof_dl_antennas Number of antennas used for DL tx.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
validator_result validate_pucch_cfg(const serving_cell_config&           ue_cell_cfg,
                                    const pucch_resource_builder_params& res_params,
                                    const std::vector<pucch_resource>&   cell_pucch_res_list,
                                    const pucch_config_common&           pucch_cfg_common,
                                    unsigned                             nof_dl_antennas);

/// \brief Validates PUSCH Config in \c sched_ue_creation_request_message used to create a UE.
/// \param[in] ul_config Uplink configuration to be validated.
/// \param[in] has_csi determines whether the cell configures CSI.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
validator_result validate_pusch_cfg(const uplink_config& ul_config, bool has_csi);

/// \brief Validates SRS Config in \c sched_ue_creation_request_message used to create a UE.
/// \param[in] ue_cell_cfg UE serving cell configuration to be validated.
/// \param[in] ul_bwp_crbs UL BWP CRB interval.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
validator_result validate_srs_cfg(const serving_cell_config& ue_cell_cfg, crb_interval ul_bwp_crbs);

/// \brief Validates the NZP-CSI-RS Resource list in \c serving_cell_config passed to a UE.
validator_result validate_nzp_csi_rs_list(span<const nzp_csi_rs_resource>               nzp_csi_rs_list,
                                          const std::optional<tdd_ul_dl_config_common>& tdd_cfg_common);

/// \brief Validates CSI-MeasConfig and the ZP-CSI-RS resources in \c serving_cell_config passed to a UE.
///
/// The DU builds these from the cell configuration. Thus, the DU calls this function once per cell, with the default
/// UE configuration of the cell.
/// \param[in] ue_cell_cfg UE serving cell configuration to be validated.
/// \param[in] tdd_cfg_common TDD configuration, if any.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
validator_result validate_csi_meas_cfg(const serving_cell_config&                    ue_cell_cfg,
                                       const std::optional<tdd_ul_dl_config_common>& tdd_cfg_common,
                                       const ul_config_common&                       ul_cfg_common);

} // namespace config_validators
} // namespace ocudu
