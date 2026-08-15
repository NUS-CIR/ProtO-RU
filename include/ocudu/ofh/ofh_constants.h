// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ran/antenna_topology.h"
#include <cstddef>

namespace ocudu {
namespace ofh {

/// \brief Open Fronthaul message type.
///
/// \c uplane_prach denotes uplink PRACH User-Plane messages. An O-RU keeps these in a frame-pool space separate from
/// regular User-Plane messages so that, within a slot, PRACH and other uplink U-Plane data do not overwrite each other.
/// It behaves like \c user_plane everywhere the distinction is not relevant.
enum class message_type { control_plane, user_plane, uplane_prach, num_ofh_types };

/// Maximum number of supported eAxC. Implementation defined.
constexpr unsigned MAX_NOF_SUPPORTED_EAXC = 16;

/// Maximum allowed value for eAxC ID.
constexpr size_t MAX_SUPPORTED_EAXC_ID_VALUE = 64;

/// \brief Maximum number of beamforming weights of a beam, one per O-RU TRX.
///
/// The beamforming weights are derived from the configured antenna topology, hence the number of O-RU TRXs is bounded
/// by the largest supported topology.
constexpr unsigned MAX_NOF_BEAMFORMING_WEIGHTS = get_max_nof_ports();

/// Maximum allowed value for the beam identifier, see O-RAN.WG4.CUS, 7.5.3.9.
constexpr unsigned MAX_BEAM_ID_VALUE = 32767;

} // namespace ofh
} // namespace ocudu
