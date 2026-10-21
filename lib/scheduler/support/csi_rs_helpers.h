// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../config/cell_configuration.h"
#include "ocudu/ran/resource_allocation/ofdm_symbol_range.h"
#include "ocudu/ran/resource_allocation/rb_bitmap.h"
#include "ocudu/scheduler/result/csi_rs_info.h"
#include "ocudu/scheduler/result/sched_result_grid.h"

namespace ocudu {
namespace csi_helper {

/// \brief Helpers that checks if the slot is a candidate for CSI RS signalling.
bool is_csi_rs_slot(const cell_configuration& cell_cfg, slot_point sl_tx);

/// \brief Returns the number of PDSCH resource elements occupied by the CSI-RS of its slot.
///
/// A resource element shared by several CSI-RS counts once.
///
/// \param[in] csi_rs_list CSI-RS transmitted in the slot of the PDSCH.
/// \param[in] symbols     OFDM symbols of the PDSCH within the slot.
/// \param[in] crbs        Common resource blocks of the PDSCH, after the VRB-to-PRB mapping.
/// \return The number of resource elements of the PDSCH occupied by CSI-RS.
unsigned get_nof_csi_rs_re(pdu_span<csi_rs_info> csi_rs_list, ofdm_symbol_range symbols, const crb_bitmap& crbs);

} // namespace csi_helper
} // namespace ocudu
