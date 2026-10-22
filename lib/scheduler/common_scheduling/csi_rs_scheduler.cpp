// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "csi_rs_scheduler.h"
#include "ocudu/ran/csi_rs/csi_rs_config_helpers.h"

using namespace ocudu;

static void fill_csi_rs_info_res_map(csi_rs_info& csi_rs, const csi_rs_resource_mapping& res_map)
{
  csi_rs.crbs        = res_map.freq_band_rbs;
  csi_rs.freq_domain = res_map.fd_alloc;
  csi_rs.row         = csi_rs::get_csi_rs_resource_mapping_row_number(
      res_map.nof_ports, res_map.freq_density, res_map.cdm, res_map.fd_alloc);
  ocudu_assert(csi_rs.row > 0, "The CSI-RS configuration resulted in an invalid row of Table 7.4.1.5.3-1, TS 38.211");

  csi_rs.symbol0      = res_map.first_ofdm_symbol_in_td;
  csi_rs.symbol1      = res_map.first_ofdm_symbol_in_td2.has_value() ? *res_map.first_ofdm_symbol_in_td2 : 2;
  csi_rs.cdm_type     = res_map.cdm;
  csi_rs.freq_density = res_map.freq_density;
}

static csi_rs_info build_csi_rs_info(const bwp_configuration& bwp_cfg, const zp_csi_rs_resource& zp_csi_rs_res)
{
  csi_rs_info csi_rs;

  csi_rs.bwp_cfg = &bwp_cfg;
  csi_rs.type    = csi_rs_type::CSI_RS_ZP;
  fill_csi_rs_info_res_map(csi_rs, zp_csi_rs_res.res_mapping);

  csi_rs.scrambling_id        = 0;
  csi_rs.power_ctrl_offset    = 0;
  csi_rs.power_ctrl_offset_ss = 0;
  // Nothing is transmitted in a ZP CSI-RS, so the precoding and beamforming is left at its default.
  csi_rs.precoding_and_beamforming = make_default_precoding();

  return csi_rs;
}

static csi_rs_info build_csi_rs_info(const bwp_configuration& bwp_cfg, const nzp_csi_rs_resource& nzp_csi_rs_res)
{
  csi_rs_info csi_rs;

  csi_rs.bwp_cfg = &bwp_cfg;
  csi_rs.type    = csi_rs_type::CSI_RS_NZP;

  fill_csi_rs_info_res_map(csi_rs, nzp_csi_rs_res.res_mapping);

  csi_rs.scrambling_id        = nzp_csi_rs_res.scrambling_id;
  csi_rs.power_ctrl_offset    = nzp_csi_rs_res.pwr_ctrl_offset;
  csi_rs.power_ctrl_offset_ss = 0;
  // [Implementation-defined] The CSI-RS is neither precoded nor beamformed.
  csi_rs.precoding_and_beamforming = make_default_precoding();

  return csi_rs;
}

csi_rs_scheduler::csi_rs_scheduler(const cell_configuration& cell_cfg_) : cell_cfg(cell_cfg_)
{
  const auto& csi_cfg = cell_cfg.bwp_res[to_bwp_id(0)].dl().csi;
  if (not csi_cfg.has_value()) {
    return;
  }

  for (const auto& res : csi_cfg->zp_csi_rs_list) {
    cached_csi_rs.push_back(build_csi_rs_info(cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params, res));
  }

  for (const auto& nzp_csi : csi_cfg->nzp_csi_rs_list) {
    cached_csi_rs.push_back(build_csi_rs_info(cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params, nzp_csi));
  }
}

void csi_rs_scheduler::run_slot(cell_slot_resource_allocator& res_grid)
{
  if (cached_csi_rs.empty()) {
    return;
  }
  if (not cell_cfg.is_dl_enabled(res_grid.slot)) {
    return;
  }
  // The cache is not empty only if the cell configures CSI.
  const cell_bwp_csi_config& csi_cfg = *cell_cfg.bwp_res[to_bwp_id(0)].dl().csi;

  auto csi_rs_pdus = res_grid.result_builder().dl().csi_rs();

  // Schedule zp-CSI-RS PDUs.
  for (unsigned i = 0; i != csi_cfg.zp_csi_rs_list.size(); ++i) {
    const zp_csi_rs_resource& zp_csi = csi_cfg.zp_csi_rs_list[i];
    if ((res_grid.slot - zp_csi.offset).count() % (unsigned)zp_csi.period == 0) {
      [[maybe_unused]] const csi_rs_info* pdu = csi_rs_pdus.emplace_back(cached_csi_rs[i]);
      ocudu_assert(pdu != nullptr, "Failed to allocate ZP-CSI-RS PDU");
    }
  }

  // Schedule nzp-CSI-RS PDUs.
  for (unsigned i = 0; i != csi_cfg.nzp_csi_rs_list.size(); ++i) {
    const nzp_csi_rs_resource& nzp_csi    = csi_cfg.nzp_csi_rs_list[i];
    const auto&                csi_rs_pdu = cached_csi_rs[csi_cfg.zp_csi_rs_list.size() + i];
    if ((res_grid.slot - nzp_csi.csi_res_offset).count() % (unsigned)nzp_csi.csi_res_period == 0) {
      [[maybe_unused]] const csi_rs_info* pdu = csi_rs_pdus.emplace_back(csi_rs_pdu);
      ocudu_assert(pdu != nullptr, "Failed to allocate NZP-CSI-RS PDU");
    }
  }
}
