// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ofh/serdes/ofh_cplane_message_properties.h"
#include "ocudu/ran/prach/prach_frequency_mapping.h"
#include "ocudu/ran/prach/prach_subcarrier_spacing.h"
#include "ocudu/ran/resource_block.h"
#include <cstdint>
#include <optional>

namespace ocudu {
namespace ofh {

/// Converts the section type 3 frame-structure SCS into a PRACH subcarrier spacing.
inline prach_subcarrier_spacing to_prach_subcarrier_spacing(cplane_scs scs)
{
  switch (scs) {
    case cplane_scs::kHz15:
      return prach_subcarrier_spacing::kHz15;
    case cplane_scs::kHz30:
      return prach_subcarrier_spacing::kHz30;
    case cplane_scs::kHz60:
      return prach_subcarrier_spacing::kHz60;
    case cplane_scs::kHz120:
      return prach_subcarrier_spacing::kHz120;
    case cplane_scs::kHz1_25:
      return prach_subcarrier_spacing::kHz1_25;
    case cplane_scs::kHz5:
      return prach_subcarrier_spacing::kHz5;
    case cplane_scs::reserved:
    default:
      return prach_subcarrier_spacing::invalid;
  }
}

/// Returns true when the PRACH filter and frame-structure SCS describe the same supported preamble family.
inline bool is_prach_filter_compatible(filter_index_type filter, prach_subcarrier_spacing prach_scs)
{
  switch (filter) {
    case filter_index_type::ul_prach_preamble_1p25khz:
      return prach_scs == prach_subcarrier_spacing::kHz1_25;
    case filter_index_type::ul_prach_preamble_5kHz:
      return prach_scs == prach_subcarrier_spacing::kHz5;
    case filter_index_type::ul_prach_preamble_short:
      return is_short_preamble(prach_scs);
    case filter_index_type::ul_prach_preamble_short_15kHz:
      return prach_scs == prach_subcarrier_spacing::kHz15;
    case filter_index_type::ul_prach_preamble_short_30kHz:
      return prach_scs == prach_subcarrier_spacing::kHz30;
    default:
      return false;
  }
}

/// \brief Converts the signed section type 3 frequency offset into a PUSCH-grid RB offset.
///
/// The wire value is expressed in half-PRACH-subcarrier units relative to the carrier centre. Given
///   frequency_offset * prach_scs / 2 = rb_offset * 12 * pusch_scs - carrier_bandwidth / 2,
/// this function solves for rb_offset and verifies that the PRACH allocation fits in the RU grid.
inline std::optional<unsigned> calculate_prach_rb_offset(int                      frequency_offset,
                                                         prach_subcarrier_spacing prach_scs,
                                                         subcarrier_spacing       pusch_scs,
                                                         unsigned                 ru_nof_prbs)
{
  prach_frequency_mapping_information mapping = prach_frequency_mapping_get(prach_scs, pusch_scs);
  if (mapping.nof_rb_ra == 0) {
    return std::nullopt;
  }

  const int64_t prach_scs_hz = ra_scs_to_Hz(prach_scs);
  const int64_t pusch_scs_hz = static_cast<int64_t>(scs_to_khz(pusch_scs)) * 1000;
  const int64_t rb_bw_hz     = pusch_scs_hz * NOF_SUBCARRIERS_PER_RB;
  const int64_t total_bw_hz  = rb_bw_hz * ru_nof_prbs;

  // Multiply the equation by two to retain exact integer arithmetic for the half-subcarrier wire unit.
  const int64_t twice_offset_from_lower_edge_hz = total_bw_hz + static_cast<int64_t>(frequency_offset) * prach_scs_hz;
  const int64_t twice_rb_bw_hz                  = 2 * rb_bw_hz;
  if ((twice_offset_from_lower_edge_hz < 0) || (twice_offset_from_lower_edge_hz % twice_rb_bw_hz != 0)) {
    return std::nullopt;
  }

  const uint64_t rb_offset = static_cast<uint64_t>(twice_offset_from_lower_edge_hz / twice_rb_bw_hz);
  if (rb_offset + mapping.nof_rb_ra > ru_nof_prbs) {
    return std::nullopt;
  }

  return static_cast<unsigned>(rb_offset);
}

} // namespace ofh
} // namespace ocudu
