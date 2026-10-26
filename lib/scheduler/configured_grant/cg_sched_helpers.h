// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../support/sch_pdu_builder.h"
#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/ran/harq_id.h"
#include "ocudu/ran/slot_point.h"

namespace ocudu {

class cell_configuration;
class ue_cell_configuration;

/// \brief Derives the HARQ process ID of a CG PUSCH occasion, as per TS 38.321, Section 5.4.1.
/// \param[in] pusch_slot Slot of the CG PUSCH occasion.
/// \param[in] symbol First OFDM symbol of the CG PUSCH occasion.
/// \param[in] periodicity CG periodicity.
/// \param[in] nof_harq_processes Number of HARQ processes reserved for CG.
harq_id_t get_cg_harq_id(slot_point                      pusch_slot,
                         unsigned                        symbol,
                         cg_configuration::periodicity_t periodicity,
                         uint8_t                         nof_harq_processes);

/// \brief Returns the PUSCH TDRA list a CG resolves its time domain allocation index against.
///
/// \param[in] ue_cell_cfg UE dedicated configuration; it must have a CG configured.
span<const pusch_time_domain_resource_allocation> get_cg_pusch_td_list(const ue_cell_configuration& ue_cell_cfg);

/// \brief Builds the PUSCH parameters of a CG occasion, used for TBS computation and PDU building.
///
/// The returned parameters are independent of the CG type: CG Type 1 takes \c time_domain_allocation from the
/// RRC-configured grant, whereas CG Type 2 picks it when building the activation DCI.
/// \param[in] ue_cell_cfg UE dedicated configuration; it must have a CG configured.
/// \param[in] time_domain_allocation Index in the list returned by \ref get_cg_pusch_td_list().
/// \remark \c nof_harq_ack_bits is left unset, as it can only be derived at scheduling time.
pusch_config_params build_cg_pusch_cfg_params(const ue_cell_configuration& ue_cell_cfg, uint8_t time_domain_allocation);

} // namespace ocudu
