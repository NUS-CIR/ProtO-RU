// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "csi_rs_helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ran/csi_rs/csi_rs_pattern.h"
#include "ocudu/ran/csi_rs/frequency_allocation_type.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/ran/slot_pdu_capacity_constants.h"

using namespace ocudu;

/// Returns the resource element pattern of a CSI-RS.
static csi_rs_pattern make_csi_rs_pattern(const csi_rs_info& csi_rs)
{
  csi_rs_pattern_configuration config{.start_rb                 = csi_rs.crbs.start(),
                                      .nof_rb                   = csi_rs.crbs.length(),
                                      .csi_rs_mapping_table_row = csi_rs.row,
                                      .freq_allocation_ref_idx  = {},
                                      .symbol_l0                = csi_rs.symbol0,
                                      .symbol_l1                = csi_rs.symbol1,
                                      .cdm                      = csi_rs.cdm_type,
                                      .freq_density             = csi_rs.freq_density};
  csi_rs::convert_freq_domain(config.freq_allocation_ref_idx, csi_rs.freq_domain, csi_rs.row);
  return get_csi_rs_pattern(config);
}

bool ocudu::csi_helper::is_csi_rs_slot(const cell_configuration& cell_cfg, slot_point sl_tx)
{
  if (not cell_cfg.is_dl_enabled(sl_tx)) {
    return false;
  }

  const auto& csi_cfg = cell_cfg.bwp_res[to_bwp_id(0)].dl().csi;
  if (not csi_cfg.has_value()) {
    return false;
  }

  if (not csi_cfg->zp_csi_rs_list.empty() and csi_cfg->nzp_csi_rs_list.empty()) {
    return false;
  }

  for (const auto& zp_csi : csi_cfg->zp_csi_rs_list) {
    if ((sl_tx - zp_csi.offset).count() % (unsigned)zp_csi.period == 0) {
      return true;
    }
  }

  for (const auto& nzp_csi : csi_cfg->nzp_csi_rs_list) {
    if ((sl_tx - nzp_csi.csi_res_offset).count() % (unsigned)nzp_csi.csi_res_period == 0) {
      return true;
    }
  }

  return false;
}

unsigned ocudu::csi_helper::get_nof_csi_rs_re(pdu_span<csi_rs_info> csi_rs_list,
                                              ofdm_symbol_range     symbols,
                                              const crb_bitmap&     crbs)
{
  if (csi_rs_list.empty() or crbs.none()) {
    return 0;
  }

  static_vector<csi_rs_pattern, MAX_CSI_RS_PDUS_PER_SLOT> patterns;
  for (const csi_rs_info& csi_rs : csi_rs_list) {
    patterns.push_back(make_csi_rs_pattern(csi_rs));
  }

  unsigned nof_re = 0;
  for (unsigned symbol = symbols.start(), symbol_end = symbols.stop(); symbol != symbol_end; ++symbol) {
    // Resource elements within a resource block that each CSI-RS occupies in this OFDM symbol, all ports combined.
    static_vector<std::pair<const csi_rs_pattern*, bounded_bitset<NOF_SUBCARRIERS_PER_RB>>, MAX_CSI_RS_PDUS_PER_SLOT>
        symbol_patterns;
    for (const csi_rs_pattern& pattern : patterns) {
      bounded_bitset<NOF_SUBCARRIERS_PER_RB> re_mask(NOF_SUBCARRIERS_PER_RB);
      for (const csi_rs_pattern_port& port : pattern.prb_patterns) {
        if (port.symbol_mask.test(symbol)) {
          re_mask |= port.re_mask;
        }
      }
      if (re_mask.any()) {
        symbol_patterns.emplace_back(&pattern, re_mask);
      }
    }
    if (symbol_patterns.empty()) {
      continue;
    }

    crbs.for_each(0, crbs.size(), [&symbol_patterns, &nof_re](size_t crb) {
      // Resource elements of the resource block occupied by any of the CSI-RS.
      bounded_bitset<NOF_SUBCARRIERS_PER_RB> re_mask(NOF_SUBCARRIERS_PER_RB);
      for (const auto& [pattern, pattern_re_mask] : symbol_patterns) {
        if (crb >= pattern->rb_begin and crb < pattern->rb_end and
            (crb - pattern->rb_begin) % pattern->rb_stride == 0) {
          re_mask |= pattern_re_mask;
        }
      }
      nof_re += re_mask.count();
    });
  }

  return nof_re;
}
