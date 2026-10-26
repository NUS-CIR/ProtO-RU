// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "uci_test_utils.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/pdcch/coreset.h"
#include "ocudu/scheduler/config/sched_cell_config_helpers.h"
#include "ocudu/scheduler/config/scheduler_expert_config.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include "ocudu/scheduler/result/pdcch_info.h"
#include <type_traits>

using namespace ocudu;

pucch_info
test_helpers::make_pucch_info(const cell_configuration& cell_cfg, const pucch_resource& res, pucch_uci_bits uci_bits)
{
  pucch_info info{.crnti    = to_rnti(0x4601),
                  .bwp_cfg  = &cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params,
                  .res      = &res,
                  .uci_bits = uci_bits};
  switch (res.format()) {
    case pucch_format::FORMAT_0: {
      info.format_params.emplace<pucch_info::f0_config>(pucch_info::f0_config{
          .group_hopping = pucch_group_hopping::NEITHER,
          .n_id_hopping  = cell_cfg.params.pci,
      });
    } break;
    case pucch_format::FORMAT_1: {
      info.format_params.emplace<pucch_info::f1_config>(pucch_info::f1_config{
          .group_hopping = pucch_group_hopping::NEITHER,
          .n_id_hopping  = cell_cfg.params.pci,
      });
    } break;
    case pucch_format::FORMAT_2: {
      info.format_params.emplace<pucch_info::f2_config>(pucch_info::f2_config{
          .n_id_scrambling   = cell_cfg.params.pci,
          .n_id_0_scrambling = cell_cfg.params.pci,
          .nof_prbs          = std::get<pucch_resource::f2_config>(res.format_params).nof_prbs,
      });
    } break;
    case pucch_format::FORMAT_3: {
      info.format_params.emplace<pucch_info::f3_config>(pucch_info::f3_config{
          .group_hopping     = pucch_group_hopping::NEITHER,
          .n_id_hopping      = cell_cfg.params.pci,
          .n_id_scrambling   = cell_cfg.params.pci,
          .n_id_0_scrambling = cell_cfg.params.pci,
          .nof_prbs          = std::get<pucch_resource::f3_config>(res.format_params).nof_prbs,
      });
    } break;
    case pucch_format::FORMAT_4: {
      info.format_params.emplace<pucch_info::f4_config>(pucch_info::f4_config{
          .group_hopping     = pucch_group_hopping::NEITHER,
          .n_id_hopping      = cell_cfg.params.pci,
          .n_id_scrambling   = cell_cfg.params.pci,
          .n_id_0_scrambling = cell_cfg.params.pci,
      });
    } break;
    default:
      ocudu_assertion_failure("Invalid PUCCH format");
      break;
  }

  return info;
}

std::vector<const pucch_info*> test_helpers::find_ue_pucchs(span<const pucch_info> pucch_pdus, rnti_t rnti)
{
  std::vector<const pucch_info*> ue_pucchs;
  for (const pucch_info& pucch : pucch_pdus) {
    if (pucch.crnti == rnti) {
      ue_pucchs.push_back(&pucch);
    }
  }
  return ue_pucchs;
}

bool ocudu::pucch_info_match(const pucch_info& expected, const pucch_info& test)
{
  bool is_equal = expected.crnti == test.crnti && *expected.bwp_cfg == *test.bwp_cfg && *expected.res == *test.res &&
                  expected.format() == test.format() && expected.repetition == test.repetition;
  if (not is_equal) {
    return false;
  }

  switch (expected.format()) {
    case pucch_format::FORMAT_0: {
      const auto& expected_f = std::get<pucch_info::f0_config>(expected.format_params);
      const auto& test_f     = std::get<pucch_info::f0_config>(test.format_params);
      is_equal               = is_equal && expected_f.group_hopping == test_f.group_hopping &&
                 expected_f.n_id_hopping == test_f.n_id_hopping && expected.uci_bits.sr_bits == test.uci_bits.sr_bits &&
                 expected.uci_bits.harq_ack_nof_bits == test.uci_bits.harq_ack_nof_bits;
    } break;
    case pucch_format::FORMAT_1: {
      const auto& expected_f = std::get<pucch_info::f1_config>(expected.format_params);
      const auto& test_f     = std::get<pucch_info::f1_config>(test.format_params);
      is_equal               = is_equal && expected_f.group_hopping == test_f.group_hopping &&
                 expected_f.n_id_hopping == test_f.n_id_hopping && expected.uci_bits.sr_bits == test.uci_bits.sr_bits &&
                 expected.uci_bits.harq_ack_nof_bits == test.uci_bits.harq_ack_nof_bits;
    } break;
    case pucch_format::FORMAT_2: {
      const auto& expected_f = std::get<pucch_info::f2_config>(expected.format_params);
      const auto& test_f     = std::get<pucch_info::f2_config>(test.format_params);
      is_equal               = is_equal && expected_f.n_id_scrambling == test_f.n_id_scrambling &&
                 expected_f.n_id_0_scrambling == test_f.n_id_0_scrambling && expected_f.nof_prbs == test_f.nof_prbs &&
                 expected.uci_bits.sr_bits == test.uci_bits.sr_bits &&
                 expected.uci_bits.harq_ack_nof_bits == test.uci_bits.harq_ack_nof_bits &&
                 expected.uci_bits.csi_part1_nof_bits == test.uci_bits.csi_part1_nof_bits;
    } break;
    case pucch_format::FORMAT_3: {
      const auto& expected_f = std::get<pucch_info::f3_config>(expected.format_params);
      const auto& test_f     = std::get<pucch_info::f3_config>(test.format_params);
      is_equal               = is_equal && expected_f.group_hopping == test_f.group_hopping &&
                 expected_f.n_id_hopping == test_f.n_id_hopping && expected_f.nof_prbs == test_f.nof_prbs &&
                 expected.uci_bits.sr_bits == test.uci_bits.sr_bits &&
                 expected.uci_bits.harq_ack_nof_bits == test.uci_bits.harq_ack_nof_bits &&
                 expected.uci_bits.csi_part1_nof_bits == test.uci_bits.csi_part1_nof_bits &&
                 expected_f.n_id_scrambling == test_f.n_id_scrambling &&
                 expected_f.n_id_0_scrambling == test_f.n_id_0_scrambling;
    } break;
    case pucch_format::FORMAT_4: {
      const auto& expected_f = std::get<pucch_info::f4_config>(expected.format_params);
      const auto& test_f     = std::get<pucch_info::f4_config>(test.format_params);
      is_equal               = is_equal && expected_f.group_hopping == test_f.group_hopping &&
                 expected_f.n_id_hopping == test_f.n_id_hopping && expected.uci_bits.sr_bits == test.uci_bits.sr_bits &&
                 expected.uci_bits.harq_ack_nof_bits == test.uci_bits.harq_ack_nof_bits &&
                 expected.uci_bits.csi_part1_nof_bits == test.uci_bits.csi_part1_nof_bits &&
                 expected_f.n_id_scrambling == test_f.n_id_scrambling &&
                 expected_f.n_id_0_scrambling == test_f.n_id_0_scrambling;
    } break;
    default: {
      return false;
    };
  }

  return is_equal;
}

static cell_config_builder_params make_custom_cell_config_builder_params(const test_bench_params& params)
{
  cell_config_builder_params cfg_params{};
  cfg_params.scs_common             = params.tdd ? subcarrier_spacing::kHz30 : subcarrier_spacing::kHz15;
  cfg_params.dl_carrier.carrier_bw  = bs_channel_bandwidth::MHz10;
  cfg_params.dl_carrier.arfcn_f_ref = params.tdd ? 520000U : 365000U;
  cfg_params.dl_carrier.nof_ant     = params.nof_ul_dl_ports;

  if (params.tdd) {
    cfg_params.tdd_ul_dl_cfg_common = tdd_ul_dl_config_common{.ref_scs  = subcarrier_spacing::kHz30,
                                                              .pattern1 = {.dl_ul_tx_period_nof_slots = 10,
                                                                           .nof_dl_slots              = 6,
                                                                           .nof_dl_symbols            = 8,
                                                                           .nof_ul_slots              = 3,
                                                                           .nof_ul_symbols = params.tdd_nof_ul_symbols},
                                                              .pattern2 = std::nullopt};
  }

  return cfg_params;
}
static sched_cell_configuration_request_message
make_custom_sched_cell_configuration_request(const test_bench_params& params)
{
  sched_cell_configuration_request_message req = sched_config_helper::make_default_sched_cell_configuration_request(
      make_custom_cell_config_builder_params(params));

  if (params.pucch_res_common.has_value()) {
    req.ran.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common = *params.pucch_res_common;
  } else {
    if (std::holds_alternative<pucch_f0_params>(params.pucch_ded_params.f0_or_f1_params)) {
      req.ran.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common = 0U;
    } else {
      req.ran.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common = 11U;
    }
  }
  req.ran.init_bwp.pucch.resources = params.pucch_ded_params;
  req.ran.init_bwp.pucch.sr_period = params.sr_period;
  if (params.ntn_cs_koffset.count() != 0 or params.ntn_k_mac.count() != 0) {
    req.ran.ntn_params.emplace();
    req.ran.ntn_params->ntn_cfg.cell_specific_koffset = params.ntn_cs_koffset;
    if (params.ntn_k_mac.count() != 0) {
      req.ran.ntn_params->ntn_cfg.k_mac = params.ntn_k_mac;
    }
  }
  if (params.csi_period.has_value()) {
    req.ran.init_bwp.csi.value().csi_rs_period          = params.csi_period.value();
    req.ran.init_bwp.csi.value().csi_report_slot_offset = params.csi_offset;
  } else {
    req.ran.init_bwp.csi = std::nullopt;
  }

  return req;
}

static pdcch_dl_information make_default_dci(unsigned n_cces, const coreset_configuration* coreset_cfg)
{
  pdcch_dl_information dci{};
  dci.ctx.cces.ncce   = n_cces;
  dci.ctx.coreset_cfg = coreset_cfg;
  return dci;
}

test_bench::test_bench(const test_bench_params& params_) :
  params(params_),
  expert_cfg([]() {
    // Create default scheduler expert configuration and save it for the lifetime of the test bench.
    auto default_expert_cfg                            = config_helpers::make_default_scheduler_expert_config();
    default_expert_cfg.ue.min_pucch_pusch_prb_distance = 0U;
    return default_expert_cfg;
  }()),
  cfg_mng(make_custom_cell_config_builder_params(params), expert_cfg),
  cell_cfg(*cfg_mng.add_cell(make_custom_sched_cell_configuration_request(params))),
  cell_ues(cell_cfg, nullptr),
  ues(expert_cfg.ue),
  pucch_builder(cell_cfg.expert_cfg.ue.max_pucchs_per_slot),
  res_grid(cell_cfg),
  k0(cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list[0].k0),
  dci_info{make_default_dci(params.n_cces, &cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0.value())},
  pucch_alloc{cell_cfg, params.max_pucchs_per_slot, params.max_ul_grants_per_slot},
  uci_alloc(cell_cfg, pucch_alloc),
  uci_sched{cell_cfg, uci_alloc, cell_ues},
  sl_tx{to_numerology_value(cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.scs), 0}
{
  ues.register_cell(cell_ues);
  pucch_builder.add_cell(to_du_cell_index(0), cell_cfg.params);

  // Add main UE.
  add_ue();

  slot_indication(sl_tx);
}

const ue& test_bench::get_main_ue() const
{
  return get_ue(main_ue_idx);
}

const ue& test_bench::get_ue(du_ue_index_t ue_idx) const
{
  ocudu_assert(ues.contains(ue_idx), "User not found");
  return ues[ue_idx];
}

void test_bench::add_ue()
{
  sched_ue_creation_request_message ue_req =
      sched_config_helper::create_default_sched_ue_creation_request(cell_cfg.params);

  if (ue_ded_cfgs.empty()) {
    ue_req.ue_index = main_ue_idx;
    ue_req.crnti    = to_rnti(0x4601);
  } else {
    ue_req.ue_index =
        to_du_ue_index(static_cast<std::underlying_type_t<du_ue_index_t>>(ue_ded_cfgs.back()->ue_index) + 1);
    ue_req.crnti = to_rnti(static_cast<std::underlying_type_t<rnti_t>>(ue_ded_cfgs.back()->crnti) + 1);
  }

  ue_cell_config& ue_cell = *ue_req.cfg.cells->back().ded;
  const bool      success = pucch_builder.alloc_resources(ue_cell);
  ocudu_assert(success, "UE PUCCH configuration couldn't be built");
  if (params.ue_caps.has_value()) {
    pucch_builder.update_resources(ue_cell, *params.ue_caps);
  }

  // TODO: rewrite this test, we should never modify the UE cell config like this in unittests.
  ue_cell.init_bwp().ul.pucch.sr_offset                                         = params.sr_offset;
  ue_cell.serv_cell_cfg.ul_config->init_ul_bwp.pucch_cfg->sr_res_list[0].offset = params.sr_offset;
  if (params.csi_period.has_value()) {
    ue_cell.init_bwp().ul.csi.periodic_report.value().offset = params.csi_offset;
    auto& csi_report = std::get<csi_report_config::periodic_or_semi_persistent_report_on_pucch>(
        ue_cell.serv_cell_cfg.csi_meas_cfg.value().csi_report_cfg_list[0].report_cfg_type);
    csi_report.report_slot_offset = params.csi_offset;
  }

  ue_req.cfg.meas_gap_cfg = params.meas_gap;

  const ue_configuration* ue_cfg = cfg_mng.add_ue(ue_req);
  ocudu_assert(ue_cfg != nullptr, "Failed to create UE configuration");
  ue_ded_cfgs.push_back(ue_cfg);
  ues.add_ue(*ue_ded_cfgs.back(), {sched_config_helper::to_ue_creation_mode(ue_req), ue_req.ul_ccch_slot_rx});
  uci_sched.add_ue(ues[ue_req.ue_index].get_pcell().cfg());

  last_added_ue_idx  = ue_req.ue_index;
  last_added_ue_rnti = ue_req.crnti;
}

void test_bench::slot_indication(slot_point slot_tx)
{
  pucch_alloc.slot_indication(slot_tx);
  mac_logger.set_context(slot_tx.sfn(), slot_tx.slot_index());
  test_logger.set_context(slot_tx.sfn(), slot_tx.slot_index());
  res_grid.slot_indication(slot_tx);
}

void test_bench::fill_all_grid(slot_point slot_tx)
{
  cell_slot_resource_allocator& pucch_slot_alloc = res_grid[slot_tx];
  pucch_slot_alloc.ul_res_grid.fill(grant_info{cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs,
                                               ofdm_symbol_range{0, 14},
                                               cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs});
}
