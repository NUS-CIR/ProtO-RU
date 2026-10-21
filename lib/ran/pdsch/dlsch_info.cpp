// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/ran/pdsch/dlsch_info.h"
#include "ocudu/ran/dmrs/dmrs.h"
#include "ocudu/ran/resource_block.h"

using namespace ocudu;

dlsch_information ocudu::get_dlsch_information(const dlsch_configuration& config)
{
  using namespace units::literals;

  dlsch_information result = {};

  // Get shared channel parameters.
  result.sch = get_sch_segmentation_info(config.tbs, config.mcs_descr.get_normalised_target_code_rate());

  // Check whether the number of CDM groups without data is valid.
  ocudu_assert(config.nof_cdm_groups_without_data >= 1 &&
                   config.nof_cdm_groups_without_data <= get_max_nof_cdm_groups_without_data(config.dmrs_type),
               "The number of CDM groups without data (i.e., {}) is invalid (min. 1, max. {}).",
               config.nof_cdm_groups_without_data,
               get_max_nof_cdm_groups_without_data(config.dmrs_type));

  // Make sure the OFDM symbol that carry DM-RS are within the time allocation.
  ocudu_assert(config.dmrs_symbol_mask.count() > 0,
               "The number of OFDM symbols carrying DM-RS RE must be greater than zero.");
  ocudu_assert(
      static_cast<unsigned>(config.dmrs_symbol_mask.find_lowest(true)) >= config.start_symbol_index,
      "The index of the first OFDM symbol carrying DM-RS (i.e., {}) must be equal to or greater than the first symbol "
      "allocated to transmission (i.e., {}).",
      config.dmrs_symbol_mask.find_lowest(true),
      config.start_symbol_index);
  ocudu_assert(static_cast<unsigned>(config.dmrs_symbol_mask.find_highest(true)) <
                   (config.start_symbol_index + config.nof_symbols),
               "The index of the last OFDM symbol carrying DM-RS (i.e., {}) must be less than or equal to the last "
               "symbol allocated "
               "to transmission (i.e., {}).",
               config.dmrs_symbol_mask.find_highest(true),
               config.start_symbol_index + config.nof_symbols - 1);
  ocudu_assert(config.dmrs_symbol_mask.size() >= (config.start_symbol_index + config.nof_symbols),
               "The DM-RS symbol mask size (i.e., {}) must be the same as the number of symbols allocated to the "
               "transmission within the slot (i.e., {}).",
               config.dmrs_symbol_mask.size(),
               config.start_symbol_index + config.nof_symbols);

  // Count number of OFDM symbols that contain DM-RS.
  unsigned nof_symbols_dmrs = config.dmrs_symbol_mask.count();

  // Count number of RE used for DM-RS.
  unsigned nof_re_dmrs_per_rb =
      nof_symbols_dmrs * config.nof_cdm_groups_without_data * get_nof_re_per_prb(config.dmrs_type);

  // Count number of RE used for PT-RS.
  unsigned nof_pt_rs_per_rb = 0;

  // Count total number of resource elements available for DL-SCH, excluding the ones occupied by CSI-RS.
  unsigned nof_re_dl_sch =
      config.nof_rb * (config.nof_symbols * NOF_SUBCARRIERS_PER_RB - nof_re_dmrs_per_rb - nof_pt_rs_per_rb);
  ocudu_assert(config.nof_csi_rs_re < nof_re_dl_sch,
               "The number of RE occupied by CSI-RS (i.e., {}) must be less than the number of RE available for the "
               "transmission (i.e., {}).",
               config.nof_csi_rs_re,
               nof_re_dl_sch);
  nof_re_dl_sch -= config.nof_csi_rs_re;

  // Retrieve the modulation order.
  unsigned modulation_order = get_bits_per_symbol(config.mcs_descr.modulation);

  // Number of bits used for shared channel.
  result.nof_dl_sch_bits = units::bits(nof_re_dl_sch * config.nof_layers * modulation_order);

  // Count the number of rate matched bits that overlap with the DC position.
  if (config.contains_dc) {
    result.nof_dc_overlap_bits = units::bits(config.nof_symbols * modulation_order);
  } else {
    result.nof_dc_overlap_bits = 0_bits;
  }

  return result;
}
