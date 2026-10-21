// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/bounded_bitset.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/dmrs/dmrs.h"
#include "ocudu/ran/sch/sch_mcs.h"
#include "ocudu/ran/sch/sch_segmentation.h"
#include "ocudu/support/units.h"

namespace ocudu {

/// Collects the necessary parameters to calculate the Downlink Shared Channel (DL-SCH) information.
struct dlsch_configuration {
  /// Transport block size.
  units::bits tbs;
  /// Modulation and coding scheme.
  sch_mcs_description mcs_descr;
  /// Transmission bandwidth in resource blocks.
  unsigned nof_rb;
  /// First OFDM index for the transmission within the slot.
  unsigned start_symbol_index;
  /// Number of OFDM symbols used for the transmission.
  unsigned nof_symbols;
  /// DM-RS Type.
  dmrs_config_type dmrs_type;
  /// Boolean mask indicating which OFDM symbols in the slot contain DM-RS.
  bounded_bitset<MAX_NSYMB_PER_SLOT> dmrs_symbol_mask;
  /// Number of CDM groups without data.
  unsigned nof_cdm_groups_without_data;
  /// Number of transmission layers.
  unsigned nof_layers;
  /// Set to true if the transmission overlaps with the Direct Current (DC).
  bool contains_dc;
  /// \brief Number of resource elements of the transmission occupied by CSI-RS, as per TS 38.214, Section 5.1.4.2.
  ///
  /// The count is the total over all RBs and OFDM symbols. It does not depend on the number of layers.
  unsigned nof_csi_rs_re = 0;
};

/// Collects Downlink Shared Channel (DL-SCH) derived parameters.
struct dlsch_information {
  /// Shared channel (SCH) parameters.
  sch_information sch;
  /// Number of encoded and rate-matched DL-SCH data bits.
  units::bits nof_dl_sch_bits;
  /// Number of bits that are affected by overlapping with the direct current.
  units::bits nof_dc_overlap_bits;

  /// \brief Calculates the effective code rate normalized between 0 and 1.
  ///
  /// The effective code rate is determined as the quotient of the number of information bits plus CRCs and the total
  /// number of channel bits.
  float get_effective_code_rate() const
  {
    ocudu_assert(nof_dl_sch_bits.value() != 0, "DL-SCH number of bits must not be zero.");
    ocudu_assert(sch.nof_bits_per_cb.value() > sch.nof_filler_bits_per_cb.value(),
                 "The number of bits per CB must be greater than the number of filler bits.");
    return static_cast<float>((sch.nof_bits_per_cb.value() - sch.nof_filler_bits_per_cb.value()) * sch.nof_cb) /
           static_cast<float>(nof_dl_sch_bits.value() - nof_dc_overlap_bits.value());
  }
};

/// Gets the DL-SCH information for a given PDSCH configuration.
dlsch_information get_dlsch_information(const dlsch_configuration& config);

} // namespace ocudu
