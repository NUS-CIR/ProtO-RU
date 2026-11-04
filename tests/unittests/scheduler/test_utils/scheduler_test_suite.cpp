// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "tests/unittests/scheduler/test_utils/scheduler_test_suite.h"
#include "lib/scheduler/cell/resource_grid.h"
#include "lib/scheduler/common_scheduling/ra_scheduler.h"
#include "lib/scheduler/support/config_helpers.h"
#include "lib/scheduler/support/pdsch/pdsch_default_time_allocation.h"
#include "lib/scheduler/support/pucch/pucch_collision.h"
#include "lib/scheduler/support/sched_result_helpers.h"
#include "scheduler_output_test_helpers.h"
#include "tests/test_doubles/scheduler/scheduler_test_message_validators.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/pdcch/dci_packing.h"
#include "ocudu/ran/prach/prach_configuration.h"
#include "ocudu/ran/prach/prach_cyclic_shifts.h"
#include "ocudu/ran/prach/prach_preamble_information.h"
#include "ocudu/ran/prach/ra_helper.h"
#include "ocudu/ran/pucch/pucch_constants.h"
#include "ocudu/ran/resource_allocation/ofdm_symbol_range.h"
#include "ocudu/ran/resource_allocation/resource_allocation_frequency.h"
#include "ocudu/scheduler/scheduler_feedback_handler.h"
#include "ocudu/support/error_handling.h"
#include <gtest/gtest.h>

using namespace ocudu;

void ocudu::assert_tdd_pattern_consistency(const cell_configuration& cell_cfg,
                                           slot_point                sl_tx,
                                           sched_slot_result_reader  result)
{
  if (not cell_cfg.is_tdd()) {
    return;
  }
  const auto        dl         = result.dl();
  const auto        ul         = result.ul();
  ofdm_symbol_range dl_symbols = get_active_tdd_dl_symbols(
      *cell_cfg.params.tdd_cfg, sl_tx.slot_index(), cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.cp);
  ASSERT_EQ(dl_symbols.length(), dl.nof_dl_symbols());

  if (dl_symbols.empty()) {
    ASSERT_TRUE(dl.dl_pdcchs().empty());
    ASSERT_TRUE(dl.ul_pdcchs().empty());
    ASSERT_TRUE(dl.ssb_info().empty());
    ASSERT_TRUE(dl.sibs().empty());
    ASSERT_TRUE(dl.rar_grants().empty());
    ASSERT_TRUE(dl.paging_grants().empty());
    ASSERT_TRUE(dl.ue_grants().empty());
    ASSERT_TRUE(dl.csi_rs().empty());
  } else if (dl_symbols.length() != get_nsymb_per_slot(cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.cp)) {
    // Partial slot case.
    for (const auto& ssb : dl.ssb_info()) {
      ASSERT_TRUE(dl_symbols.contains(ssb.symbols));
    }
    for (const auto& sib : dl.sibs()) {
      ASSERT_TRUE(dl_symbols.contains(sib.pdsch_cfg.symbols));
    }
    for (const auto& rar : dl.rar_grants()) {
      ASSERT_TRUE(dl_symbols.contains(rar.pdsch_cfg.symbols));
    }
    for (const auto& paging_grant : dl.paging_grants()) {
      ASSERT_TRUE(dl_symbols.contains(paging_grant.pdsch_cfg.symbols));
    }
    for (const auto& ue_grant : dl.ue_grants()) {
      ASSERT_TRUE(dl_symbols.contains(ue_grant.pdsch_cfg.symbols));
    }
    for (const auto& csi_rs : dl.csi_rs()) {
      ASSERT_TRUE(dl_symbols.contains(csi_rs.symbol0));
    }
  }

  ofdm_symbol_range ul_symbols = get_active_tdd_ul_symbols(
      *cell_cfg.params.tdd_cfg, sl_tx.count(), cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.cp);
  ASSERT_EQ(ul_symbols.length(), ul.nof_ul_symbols());

  if (ul_symbols.empty()) {
    ASSERT_TRUE(ul.puschs().empty());
    ASSERT_TRUE(ul.prachs().empty());
    ASSERT_TRUE(ul.pucchs().empty());
    ASSERT_TRUE(ul.srss().empty());
  } else if (dl_symbols.length() != get_nsymb_per_slot(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.cp)) {
    for (const auto& ue_grant : ul.puschs()) {
      ASSERT_TRUE(ul_symbols.contains(ue_grant.pusch_cfg.symbols));
    }
    for (const auto& prach : ul.prachs()) {
      ofdm_symbol_range prach_symbols = {prach.start_symbol, prach.start_symbol + get_preamble_duration(prach.format)};
      ASSERT_TRUE(ul_symbols.contains(prach_symbols));
    }
    for (const auto& pucch : ul.pucchs()) {
      ASSERT_TRUE(ul_symbols.contains(pucch.res->syms));
    }
    for (const auto& srs : ul.srss()) {
      ASSERT_TRUE(ul_symbols.contains(srs.symbols));
    }
  }
}

void ocudu::assert_pdcch_pdsch_common_consistency(const cell_configuration&   cell_cfg,
                                                  const pdcch_dl_information& pdcch,
                                                  const pdsch_information&    pdsch)
{
  ASSERT_EQ(pdcch.ctx.rnti, pdsch.rnti);
  ASSERT_TRUE(*pdcch.ctx.bwp_cfg == *pdsch.bwp_cfg);
  ASSERT_TRUE(*pdcch.ctx.coreset_cfg == *pdsch.coreset_cfg);
  bwp_configuration bwp_cfg = cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params;
  // See TS 38.214, 5.1.2.2.2, Downlink resource allocation type 1.
  if (cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0.has_value()) {
    bwp_cfg.crbs = get_coreset0_crbs(cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common);
  }
  const crb_interval cs_zero_crbs = get_coreset0_crbs(cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common);

  unsigned                                          time_assignment = 0;
  unsigned                                          freq_assignment = 0;
  unsigned                                          N_rb_dl_bwp     = 0;
  span<const pdsch_time_domain_resource_allocation> td_list;
  switch (pdcch.dci.type()) {
    case dci_dl_rnti_config_type::si_f1_0: {
      ASSERT_EQ(pdcch.ctx.rnti, rnti_t::SI_RNTI);
      time_assignment = pdcch.dci.as_si_rnti_f1_0().time_resource;
      freq_assignment = pdcch.dci.as_si_rnti_f1_0().frequency_resource;
      N_rb_dl_bwp     = pdcch.dci.as_si_rnti_f1_0().N_rb_dl_bwp;
      if (pdcch.dci.as_si_rnti_f1_0().system_information_indicator == 1) {
        td_list = get_si_rnti_type0A_common_pdsch_time_domain_list(
            cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common,
            cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.cp,
            cell_cfg.params.dmrs_typeA_pos);
      } else {
        td_list = get_si_rnti_pdsch_time_domain_list(cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.cp,
                                                     cell_cfg.params.dmrs_typeA_pos);
      }
      ASSERT_EQ(N_rb_dl_bwp, cs_zero_crbs.length());
      break;
    }
    case dci_dl_rnti_config_type::ra_f1_0: {
      time_assignment = pdcch.dci.as_ra_rnti_f1_0().time_resource;
      td_list         = get_ra_rnti_pdsch_time_domain_list(cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common,
                                                   cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.cp,
                                                   cell_cfg.params.dmrs_typeA_pos);
      freq_assignment = pdcch.dci.as_ra_rnti_f1_0().frequency_resource;
      N_rb_dl_bwp     = pdcch.dci.as_ra_rnti_f1_0().N_rb_dl_bwp;
      ASSERT_EQ(N_rb_dl_bwp, bwp_cfg.crbs.length());
      ASSERT_EQ(pdcch.ctx.context.ss_id, cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.ra_search_space_id);
    } break;
    case dci_dl_rnti_config_type::tc_rnti_f1_0: {
      time_assignment = pdcch.dci.as_tc_rnti_f1_0().time_resource;
      freq_assignment = pdcch.dci.as_tc_rnti_f1_0().frequency_resource;
      N_rb_dl_bwp     = pdcch.dci.as_tc_rnti_f1_0().N_rb_dl_bwp;
      td_list         = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list;
      ASSERT_EQ(N_rb_dl_bwp, cs_zero_crbs.length());
    } break;
    case dci_dl_rnti_config_type::c_rnti_f1_0: {
      time_assignment = pdcch.dci.as_c_rnti_f1_0().time_resource;
      freq_assignment = pdcch.dci.as_c_rnti_f1_0().frequency_resource;
      N_rb_dl_bwp     = cs_zero_crbs.length();
      td_list         = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list;
    } break;
    case dci_dl_rnti_config_type::p_rnti_f1_0: {
      time_assignment = pdcch.dci.as_p_rnti_f1_0().time_resource;
      freq_assignment = pdcch.dci.as_p_rnti_f1_0().frequency_resource;
      N_rb_dl_bwp     = pdcch.dci.as_p_rnti_f1_0().N_rb_dl_bwp;
      td_list         = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list;
      ASSERT_EQ(N_rb_dl_bwp, cs_zero_crbs.length());
    } break;
    default:
      ocudu_terminate("DCI type not supported");
  }
  ASSERT_LT(time_assignment, td_list.size());
  ofdm_symbol_range symbols = td_list[time_assignment].symbols;
  ASSERT_EQ(symbols, pdsch.symbols) << "Mismatch of time-domain resource assignment and PDSCH symbols";

  unsigned pdsch_freq_resource = ra_frequency_type1_get_riv(
      ra_frequency_type1_configuration{N_rb_dl_bwp, pdsch.rbs.type1().start(), pdsch.rbs.type1().length()});
  ASSERT_EQ(pdsch_freq_resource, freq_assignment) << "DCI frequency resource does not match PDSCH PRBs";
}

void ocudu::assert_pdcch_pdsch_common_consistency(const cell_configuration&      cell_cfg,
                                                  const cell_resource_allocator& cell_res_grid)
{
  pdu_span<pdcch_dl_information> pdcchs = cell_res_grid[0].result_reader().dl().dl_pdcchs();
  for (const pdcch_dl_information& pdcch : pdcchs) {
    const pdsch_information* linked_pdsch = nullptr;
    switch (pdcch.dci.type()) {
      case dci_dl_rnti_config_type::si_f1_0: {
        const auto      sibs = cell_res_grid[0].result_reader().dl().sibs();
        sib_information sib;
        const auto&     it = std::find_if(sibs.begin(), sibs.end(), [&pdcch](const auto& sib_) {
          unsigned pdsch_freq_resource =
              ra_frequency_type1_get_riv(ra_frequency_type1_configuration{pdcch.dci.as_si_rnti_f1_0().N_rb_dl_bwp,
                                                                          sib_.pdsch_cfg.rbs.type1().start(),
                                                                          sib_.pdsch_cfg.rbs.type1().length()});
          return (sib_.pdsch_cfg.rnti == pdcch.ctx.rnti) &&
                 (pdsch_freq_resource == pdcch.dci.as_si_rnti_f1_0().frequency_resource);
        });
        ASSERT_NE(it, sibs.end());
        linked_pdsch = &it->pdsch_cfg;
      } break;
      case dci_dl_rnti_config_type::ra_f1_0: {
        uint8_t k0 = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common
                         .pdsch_td_alloc_list[pdcch.dci.as_ra_rnti_f1_0().time_resource]
                         .k0;
        const auto rars = cell_res_grid[k0].result_reader().dl().rar_grants();
        auto       it   = std::find_if(
            rars.begin(), rars.end(), [&pdcch](const auto& rar) { return rar.pdsch_cfg.rnti == pdcch.ctx.rnti; });
        ASSERT_NE(it, rars.end());
        linked_pdsch = &it->pdsch_cfg;
      } break;
      case dci_dl_rnti_config_type::c_rnti_f1_0: {
        uint8_t k0 = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common
                         .pdsch_td_alloc_list[pdcch.dci.as_c_rnti_f1_0().time_resource]
                         .k0;
        const auto ue_grants = cell_res_grid[k0].result_reader().dl().ue_grants();
        const auto it        = std::find_if(ue_grants.begin(), ue_grants.end(), [&pdcch](const auto& grant) {
          return grant.pdsch_cfg.rnti == pdcch.ctx.rnti;
        });
        ASSERT_NE(it, ue_grants.end());
        linked_pdsch = &it->pdsch_cfg;
      } break;
      case dci_dl_rnti_config_type::tc_rnti_f1_0: {
        uint8_t k0 = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common
                         .pdsch_td_alloc_list[pdcch.dci.as_tc_rnti_f1_0().time_resource]
                         .k0;
        const auto ue_grants = cell_res_grid[k0].result_reader().dl().ue_grants();
        const auto it        = std::find_if(ue_grants.begin(), ue_grants.end(), [&pdcch](const auto& grant) {
          return grant.pdsch_cfg.rnti == pdcch.ctx.rnti;
        });
        ASSERT_NE(it, ue_grants.end());
        linked_pdsch = &it->pdsch_cfg;
      } break;
      case dci_dl_rnti_config_type::p_rnti_f1_0: {
        // No corresponding PDSCH.
        if (pdcch.dci.as_p_rnti_f1_0().short_messages_indicator ==
            dci_1_0_p_rnti_configuration::payload_info::short_messages) {
          break;
        }
        uint8_t k0 = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common
                         .pdsch_td_alloc_list[pdcch.dci.as_p_rnti_f1_0().time_resource]
                         .k0;
        const auto pg_grants = cell_res_grid[k0].result_reader().dl().paging_grants();
        const auto it        = std::find_if(pg_grants.begin(), pg_grants.end(), [&pdcch](const auto& grant) {
          return grant.pdsch_cfg.rnti == pdcch.ctx.rnti;
        });
        ASSERT_NE(it, pg_grants.end());
        linked_pdsch = &it->pdsch_cfg;
      } break;
      default:
        ocudu_terminate("DCI type not supported");
    }
    if (linked_pdsch) {
      assert_pdcch_pdsch_common_consistency(cell_cfg, pdcch, *linked_pdsch);
    }
  }
}

void ocudu::test_pdsch_sib_consistency(const cell_configuration& cell_cfg, span<const sib_information> sibs)
{
  bool has_coreset0 = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0.has_value();
  if (not has_coreset0) {
    ASSERT_TRUE(sibs.empty()) << fmt::format("SIB1 cannot be scheduled without CORESET#0");
    return;
  }

  bwp_configuration effective_init_bwp_cfg = cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params;
  effective_init_bwp_cfg.crbs              = get_coreset0_crbs(cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common);

  for (const sib_information& sib : sibs) {
    ASSERT_EQ(sib.pdsch_cfg.rnti, rnti_t::SI_RNTI);
    ASSERT_EQ(sib.pdsch_cfg.dci_fmt, dci_dl_format::f1_0);
    ASSERT_TRUE(sib.pdsch_cfg.rbs.is_type1());
    ASSERT_EQ(sib.pdsch_cfg.coreset_cfg->get_id(), to_coreset_id(0));
    if (sib.si_indicator == sib_information::sib1) {
      ASSERT_EQ(sib.pdsch_cfg.ss_set_type, search_space_set_type::type0);
      ASSERT_FALSE(sib.si_msg_index.has_value());
    } else {
      ASSERT_TRUE(sib.pdsch_cfg.ss_set_type == search_space_set_type::type0 or
                  sib.pdsch_cfg.ss_set_type == search_space_set_type::type0A);
      ASSERT_TRUE(sib.si_msg_index.has_value());
    }
    ASSERT_EQ(sib.pdsch_cfg.codewords.size(), 1);
    ASSERT_EQ(sib.pdsch_cfg.mcs_table, pdsch_mcs_table::qam64);
    vrb_interval vrbs = sib.pdsch_cfg.rbs.type1();
    ASSERT_LE(vrbs.stop(), effective_init_bwp_cfg.crbs.length())
        << fmt::format("PRB grant falls outside CORESET#0 RB boundaries");
  }
}

void ocudu::test_pdsch_rar_consistency(const cell_configuration& cell_cfg, pdu_span<rar_information> rars)
{
  std::set<rnti_t>                  ra_rntis;
  std::set<rnti_t>                  tc_rntis;
  const search_space_configuration& ss_cfg =
      cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common
          .search_spaces[cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.ra_search_space_id];
  crb_interval      coreset0_lims = get_coreset0_crbs(cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common);
  bwp_configuration init_bwp_cfg  = cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params;

  for (const rar_information& rar : rars) {
    rnti_t ra_rnti = rar.pdsch_cfg.rnti;
    ASSERT_TRUE(not rar.grants.empty() or rar.backoff_indicator.has_value())
        << fmt::format("RAR with RA-rnti={} has no corresponding MSG3 grants and no Backoff Indicator", ra_rnti);
    ASSERT_EQ(rar.pdsch_cfg.dci_fmt, dci_dl_format::f1_0);
    ASSERT_TRUE(rar.pdsch_cfg.rbs.is_type1()) << "Invalid allocation type for RAR";
    ASSERT_EQ(rar.pdsch_cfg.coreset_cfg->get_id(), ss_cfg.get_coreset_id());
    ASSERT_EQ(rar.pdsch_cfg.ss_set_type, search_space_set_type::type1);
    ASSERT_EQ(rar.pdsch_cfg.codewords.size(), 1);
    ASSERT_EQ(rar.pdsch_cfg.mcs_table, pdsch_mcs_table::qam64);

    const prb_interval rar_vrbs = {
        rar.pdsch_cfg.rbs.type1().start() + rar.pdsch_cfg.coreset_cfg->get_coreset_start_crb(),
        rar.pdsch_cfg.rbs.type1().stop() + rar.pdsch_cfg.coreset_cfg->get_coreset_start_crb()};
    crb_interval rar_crbs = prb_to_crb(init_bwp_cfg, rar_vrbs);
    ASSERT_TRUE(coreset0_lims.contains(rar_crbs)) << "RAR outside of initial active DL BWP RB limits";

    ASSERT_FALSE(ra_rntis.count(ra_rnti)) << fmt::format("Repeated RA-rnti={} detected", ra_rnti);
    ra_rntis.emplace(ra_rnti);

    for (const auto& ul_grant : rar.grants) {
      ASSERT_FALSE(tc_rntis.count(ul_grant.temp_crnti))
          << fmt::format("Repeated TC-RNTI={} detected", ul_grant.temp_crnti);
      tc_rntis.emplace(ul_grant.temp_crnti);
    }
  }
}

void ocudu::test_pdsch_ue_consistency(const cell_configuration& cell_cfg, pdu_span<dl_msg_alloc> grants)
{
  ASSERT_TRUE(
      test_helper::is_valid_dl_msg_alloc_list(grants, cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0));
}

static void
test_pdsch_cross_consistency(const cell_configuration& cell_cfg, slot_point sl_tx, sched_slot_dl_result_reader result)
{
  if (not result.csi_rs().empty()) {
    // If CSI-RS is scheduled, it should not collide with common grants.
    ASSERT_TRUE(result.sibs().empty());
    ASSERT_TRUE(result.rar_grants().empty());
    ASSERT_TRUE(result.paging_grants().empty());
  }
}

void ocudu::test_pusch_ue_consistency(const cell_configuration& cell_cfg, pdu_span<ul_sched_info> grants)
{
  ASSERT_LE(grants.size(), cell_cfg.expert_cfg.ue.max_puschs_per_slot);

  for (const ul_sched_info& grant : grants) {
    ASSERT_TRUE(test_helper::is_valid_ul_sched_info(grant));
  }
}

void ocudu::test_pucch_consistency(const cell_configuration& cell_cfg, span<const pucch_info> pucchs)
{
  ASSERT_LE(pucchs.size(), cell_cfg.expert_cfg.ue.max_pucchs_per_slot);

  // Check for collisions between PUCCHs.
  for (const auto* it1 = pucchs.begin(); it1 != pucchs.end(); ++it1) {
    for (const auto* it2 = std::next(it1); it2 != pucchs.end(); ++it2) {
      if (it1->crnti == it2->crnti) {
        // PUCCHs for the same UE can collide, as the UE will only transmit one of them.
        continue;
      }

      if (pucch_resources_collide(*it1->res, *it2->res)) {
        ASSERT_TRUE(false);
      }
    }
  }
}

/// \brief Tests the validity of the parameters chosen for the PDCCHs using common search spaces. Checks include:
/// - PDSCH time resource chosen (k0 and symbols) fall in DL symbols
/// - UCI delay chosen falls in an UL slot.
static void test_pdcch_common_consistency(const cell_configuration&      cell_cfg,
                                          slot_point                     pdcch_slot,
                                          pdu_span<pdcch_dl_information> dl_pdcchs)
{
  if (not cell_cfg.is_tdd()) {
    return;
  }
  const auto& init_dl_bwp = cell_cfg.params.dl_cfg_common.init_dl_bwp;
  for (const pdcch_dl_information& pdcch : dl_pdcchs) {
    span<const pdsch_time_domain_resource_allocation> pdsch_td_list;
    std::optional<unsigned>                           time_res;
    std::optional<unsigned>                           k1;
    switch (pdcch.dci.type()) {
      case dci_dl_rnti_config_type::si_f1_0:
        pdsch_td_list =
            get_si_rnti_pdsch_time_domain_list(init_dl_bwp.generic_params.cp, cell_cfg.params.dmrs_typeA_pos);
        time_res = pdcch.dci.as_si_rnti_f1_0().time_resource;
        break;
      case dci_dl_rnti_config_type::ra_f1_0:
        pdsch_td_list = get_ra_rnti_pdsch_time_domain_list(
            init_dl_bwp.pdsch_common, init_dl_bwp.generic_params.cp, cell_cfg.params.dmrs_typeA_pos);
        time_res = pdcch.dci.as_ra_rnti_f1_0().time_resource;
        break;
      case dci_dl_rnti_config_type::tc_rnti_f1_0:
        pdsch_td_list = init_dl_bwp.pdsch_common.pdsch_td_alloc_list;
        time_res      = pdcch.dci.as_tc_rnti_f1_0().time_resource;
        k1            = pdcch.dci.as_tc_rnti_f1_0().pdsch_harq_fb_timing_indicator + 1;
        break;
      default:
        break;
    }
    if (not time_res.has_value()) {
      // DCI likely using dedicated config.
      continue;
    }

    // Test PDSCH time resource chosen.
    ASSERT_LT(*time_res, pdsch_td_list.size());
    const pdsch_time_domain_resource_allocation& res        = pdsch_td_list[*time_res];
    const slot_point                             pdsch_slot = pdcch_slot + res.k0;
    const ofdm_symbol_range                      active_dl_symbols =
        get_active_tdd_dl_symbols(*cell_cfg.params.tdd_cfg, pdsch_slot.slot_index(), init_dl_bwp.generic_params.cp);
    ASSERT_TRUE(active_dl_symbols.contains(res.symbols)) << "PDSCH must fall in DL symbols";

    // Test HARQ delay chosen.
    if (k1.has_value()) {
      const slot_point pucch_slot = pdsch_slot + *k1;
      ASSERT_TRUE(has_active_tdd_ul_symbols(*cell_cfg.params.tdd_cfg, pucch_slot.slot_index()))
          << "PUCCH must fall in an UL slot";
    }
  }
}

static void test_ul_pdcch_consistency(const cell_configuration&      cell_cfg,
                                      slot_point                     sl_tx,
                                      pdu_span<pdcch_ul_information> ul_pdcchs)
{
  const auto& pusch_td_list = cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
  for (const pdcch_ul_information& pdcch : ul_pdcchs) {
    if (pdcch.dci.type() == dci_ul_rnti_config_type::tc_rnti_f0_0) {
      const auto& dci_tc = pdcch.dci.as_tc_rnti_f0_0();
      // Msg3 reTx.
      ASSERT_LT(dci_tc.time_resource, pusch_td_list.size());
    }
  }
}

static void assert_rar_grant_msg3_pusch_consistency(const cell_configuration& cell_cfg,
                                                    const rar_ul_grant&       rar_grant,
                                                    const pusch_information&  msg3_pusch)
{
  ASSERT_EQ(rar_grant.temp_crnti, msg3_pusch.rnti);
  ASSERT_TRUE(msg3_pusch.rbs.is_type1());
  ASSERT_TRUE(msg3_pusch.rbs.any()) << fmt::format("Msg3 with temp-c-rnti={} has no RBs", msg3_pusch.rnti);

  unsigned     N_rb_ul_bwp = cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  vrb_interval vrbs        = msg3_pusch.rbs.type1();
  uint8_t      pusch_freq_resource =
      ra_frequency_type1_get_riv(ra_frequency_type1_configuration{N_rb_ul_bwp, vrbs.start(), vrbs.length()});
  ASSERT_EQ(rar_grant.freq_resource_assignment, pusch_freq_resource)
      << fmt::format("Mismatch between RAR grant frequency assignment and corresponding Msg3 PUSCH PRBs");
}

static void assert_rar_grant_msg3_pusch_consistency(const cell_configuration&      cell_cfg,
                                                    const cell_resource_allocator& res_grid)
{
  std::set<rnti_t> tc_rntis;
  const auto&      pusch_td_list = cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;

  pdu_span<pdcch_dl_information> pdcchs = res_grid[0].result_reader().dl().dl_pdcchs();
  for (const pdcch_dl_information& pdcch : pdcchs) {
    if (pdcch.dci.type() != dci_dl_rnti_config_type::ra_f1_0) {
      continue;
    }

    // For a given PDCCH for a RAR, search for the respective RAR PDSCH.
    uint8_t k0 = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common
                     .pdsch_td_alloc_list[pdcch.dci.as_ra_rnti_f1_0().time_resource]
                     .k0;
    const auto rars   = res_grid[k0].result_reader().dl().rar_grants();
    auto       rar_it = std::find_if(
        rars.begin(), rars.end(), [&pdcch](const auto& rar) { return rar.pdsch_cfg.rnti == pdcch.ctx.rnti; });
    ASSERT_NE(rar_it, rars.end());
    const rar_information& rar = *rar_it;

    ASSERT_EQ(rar.pdsch_cfg.codewords.size(), 1);
    const units::bytes rar_pdu_size{8}; // MAC RAR PDU subheader + length (See TS38.321, 6.1.5 and 6.2.3).
    ASSERT_GE(rar.pdsch_cfg.codewords[0].tb_size_bytes.value(), rar_pdu_size.value() * rar.grants.size());

    // For all RAR grants within the same RAR, check that they are consistent with the respective Msg3 PUSCHs.
    for (const rar_ul_grant& rar_grant : rar.grants) {
      ASSERT_EQ(tc_rntis.count(rar_grant.temp_crnti), 0) << fmt::format("Repeated TC-RNTI detected");
      tc_rntis.emplace(rar_grant.temp_crnti);

      // 2-step RACH SuccessRAR: UE's MsgA PUSCH was decoded; no Msg3 is required.
      if (std::holds_alternative<rar_ul_grant::two_step_success_info>(rar_grant.type)) {
        continue;
      }

      // 4-step RAR and 2-step FallbackRAR: a Msg3 PUSCH must be scheduled.
      // For the initial Msg3 of an NTN cell the UE transmits at pdcch_slot + k2 + Koffset, counted in its own
      // uplink frame, and that transmission reaches the gNB k_mac slots later, so the grid holds it at
      // pdcch_slot + k2 + ntn_ul_alloc_delay_slots().
      ASSERT_LT(rar_grant.time_resource_assignment, pusch_td_list.size());
      const unsigned k2_total = ra_helper::get_msg3_delay(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs,
                                                          pusch_td_list[rar_grant.time_resource_assignment].k2) +
                                cell_cfg.ntn_ul_alloc_delay_slots();

      pdu_span<ul_sched_info> ul_grants = res_grid[k2_total].result_reader().ul().puschs();
      const auto              it = std::find_if(ul_grants.begin(), ul_grants.end(), [&rar_grant](const auto& ulgrant) {
        return ulgrant.pusch_cfg.rnti == rar_grant.temp_crnti;
      });
      ASSERT_NE(it, ul_grants.end()) << fmt::format("Msg3 was not found for the scheduled RAR grant with tc-rnti={}",
                                                    rar_grant.temp_crnti);
      assert_rar_grant_msg3_pusch_consistency(cell_cfg, rar_grant, it->pusch_cfg);
    }
  }
}

void ocudu::test_dl_resource_grid_collisions(const cell_configuration& cell_cfg, sched_slot_dl_result_reader result)
{
  cell_slot_resource_grid grid(cell_cfg.params.dl_cfg_common.freq_info_dl.scs_carrier_list);

  std::vector<test_grant_info> dl_grants = get_dl_grants(cell_cfg, result);
  for (const test_grant_info& test_grant : dl_grants) {
    ASSERT_FALSE(test_grant.grant.crbs.empty()) << "Resource is empty";
    // The DL-PRS resources of a resource set are multiplexed in the comb, so they share the RBs of the set and cannot
    // be checked against each other. They are checked against the rest of the DL grants below.
    if (test_grant.type == test_grant_info::PRS) {
      continue;
    }
    ASSERT_FALSE(grid.collides(test_grant.grant))
        << fmt::format("Resource collision for grant with rnti={}", test_grant.rnti);
    grid.fill(test_grant.grant);
  }

  for (const test_grant_info& test_grant : dl_grants) {
    if (test_grant.type == test_grant_info::PRS) {
      ASSERT_FALSE(grid.collides(test_grant.grant)) << "Resource collision for a DL-PRS resource";
    }
  }
}

void ocudu::test_prach_opportunity_validity(const cell_configuration&       cell_cfg,
                                            slot_point                      sl_tx,
                                            span<const prach_occasion_info> prachs)
{
  if (prachs.empty()) {
    return;
  }

  const rach_config_common& rach_cfg_common = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  const prach_configuration prach_cfg       = prach_configuration_get(band_helper::get_freq_range(cell_cfg.band()),
                                                                cell_cfg.is_tdd() ? duplex_mode::TDD : duplex_mode::FDD,
                                                                rach_cfg_common.rach_cfg_generic.prach_config_index);

  // PRACH can only occur on SFNs where n_SFN mod x == y. On all other SFNs no occasion may be allocated.
  // The rule constrains the UL occasion at which the UE transmits. With NTN k_mac != 0 the PRACH PDU is written at
  // the DL-time slot (UL occasion + k_mac, see prach_scheduler), so k_mac must be subtracted back out before
  // checking the SFN. When k_mac == 0 this is a no-op.
  const slot_point ul_occasion_slot = cell_cfg.dl_to_ue_ul_slot(sl_tx);
  const bool       valid_sfn        = std::any_of(
      prach_cfg.y.begin(), prach_cfg.y.end(), [&](uint8_t y) { return ul_occasion_slot.sfn() % prach_cfg.x == y; });
  ASSERT_TRUE(valid_sfn) << fmt::format(
      "PRACH occasions allocated on SFN={} which is not a valid PRACH SFN (x={}, y={})",
      ul_occasion_slot.sfn(),
      prach_cfg.x,
      fmt::join(prach_cfg.y, ","));

  const uint16_t expected_nof_cs =
      prach_cyclic_shifts_get(to_ra_subcarrier_spacing(rach_cfg_common.msg1_scs),
                              rach_cfg_common.restricted_set,
                              rach_cfg_common.rach_cfg_generic.zero_correlation_zone_config);

  for (const prach_occasion_info& prach : prachs) {
    // Check if the PRACH matches cell configuration.
    ASSERT_EQ(cell_cfg.params.pci, prach.pci);
    ASSERT_EQ(prach_cfg.format, prach.format);
    ASSERT_EQ(prach_cfg.starting_symbol, prach.start_symbol);
    ASSERT_EQ(prach_cfg.nof_occasions_within_slot, prach.nof_prach_occasions);
    ASSERT_EQ(rach_cfg_common.rach_cfg_generic.msg1_fdm, prach.nof_fd_ra);
    ASSERT_LT(prach.index_fd_ra, prach.nof_fd_ra);
    ASSERT_EQ(expected_nof_cs, prach.nof_cs);
    ASSERT_EQ(rach_cfg_common.total_nof_ra_preambles, prach.nof_preamble_indexes);
    if (prach.start_preamble_index != 255) {
      ASSERT_EQ(0, prach.start_preamble_index);
    }
  }
}

void ocudu::test_ul_resource_grid_collisions(const cell_configuration& cell_cfg, sched_slot_ul_result_reader result)
{
  cell_slot_resource_grid      grid(cell_cfg.params.ul_cfg_common.freq_info_ul.scs_carrier_list);
  std::vector<test_grant_info> ul_grants = get_ul_grants(cell_cfg, result);
  std::set<rnti_t>             rntis;

  for (const test_grant_info& test_grant : ul_grants) {
    if (test_grant.type == test_grant_info::PUCCH) {
      // Collisions between PUCCH grants are tested in \c test_pucch_consistency.
      grid.fill(test_grant.grant);
      // RNTIs can be repeated in PUCCH grants.
      rntis.emplace(test_grant.rnti);
    }
  }

  for (const test_grant_info& test_grant : ul_grants) {
    if (test_grant.type == test_grant_info::PUCCH or test_grant.type == test_grant_info::SRS) {
      continue;
    }
    if (test_grant.type == test_grant_info::MSGA_PUSCH) {
      // Multiple preambles sharing the same MsgA PUSCH occasion produce duplicate ul_sched_info entries
      // with identical resources. Fill the grid only if not already occupied (idempotent, no assert).
      if (not grid.collides(test_grant.grant)) {
        grid.fill(test_grant.grant);
      }
      continue;
    }
    ASSERT_FALSE(grid.collides(test_grant.grant))
        << fmt::format("Resource collision for grant with rnti={}", test_grant.rnti);
    grid.fill(test_grant.grant);

    if (test_grant.type == test_grant_info::UE_UL) {
      ASSERT_TRUE(rntis.count(test_grant.rnti) == 0) << fmt::format("Duplicate RNTI detected: {}", test_grant.rnti);
      // TODO: Handle no multiplexing of UCI into PUSCH.
      rntis.emplace(test_grant.rnti);
    }
  }

  // SRSs of different UEs are multiplexed in the code domain (comb offset, cyclic shift and sequence ID) over the same
  // symbols and RBs, so they are only checked against the non-SRS grants, which are the ones already in the grid.
  for (const test_grant_info& test_grant : ul_grants) {
    if (test_grant.type != test_grant_info::SRS) {
      continue;
    }
    ASSERT_FALSE(grid.collides(test_grant.grant))
        << fmt::format("Resource collision for grant with rnti={}", test_grant.rnti);
  }
}

void ocudu::test_ul_consistency(const cell_configuration&   cell_cfg,
                                slot_point                  sl_tx,
                                sched_slot_ul_result_reader result)
{
  // Check that UL grant limits are respected.
  ASSERT_LE(result.pucchs().size() + result.puschs().size(), cell_cfg.expert_cfg.ue.max_ul_grants_per_slot);

  test_prach_opportunity_validity(cell_cfg, sl_tx, result.prachs());
  test_pusch_ue_consistency(cell_cfg, result.puschs());
  test_pucch_consistency(cell_cfg, result.pucchs().unsorted());
  test_ul_resource_grid_collisions(cell_cfg, result);
}

void ocudu::test_dl_consistency(const cell_configuration&   cell_cfg,
                                slot_point                  sl_tx,
                                sched_slot_dl_result_reader result)
{
  test_pdsch_sib_consistency(cell_cfg, result.sibs());
  test_pdsch_rar_consistency(cell_cfg, result.rar_grants());
  test_pdsch_ue_consistency(cell_cfg, result.ue_grants());
  test_pdsch_cross_consistency(cell_cfg, sl_tx, result);
  test_pdcch_common_consistency(cell_cfg, sl_tx, result.dl_pdcchs());
  test_ul_pdcch_consistency(cell_cfg, sl_tx, result.ul_pdcchs());
  test_dl_resource_grid_collisions(cell_cfg, result);
}

void ocudu::test_scheduler_result_consistency(const cell_configuration& cell_cfg,
                                              slot_point                sl_tx,
                                              sched_slot_result_reader  result)
{
  ASSERT_TRUE(result.success());
  assert_tdd_pattern_consistency(cell_cfg, sl_tx, result);
  test_dl_consistency(cell_cfg, sl_tx, result.dl());
  test_ul_consistency(cell_cfg, sl_tx, result.ul());
}

/// \brief Verifies that the cell resource grid PRBs and symbols was filled with the allocated PDSCHs.
void ocudu::assert_dl_resource_grid_filled(const cell_configuration&      cell_cfg,
                                           const cell_resource_allocator& cell_res_grid)
{
  std::vector<test_grant_info> dl_grants = get_dl_grants(cell_cfg, cell_res_grid[0].result_reader().dl());
  for (const test_grant_info& test_grant : dl_grants) {
    if (test_grant.type != ocudu::test_grant_info::DL_PDCCH and test_grant.type != ocudu::test_grant_info::UL_PDCCH) {
      ASSERT_TRUE(cell_res_grid[0].dl_res_grid.all_set(test_grant.grant))
          << fmt::format("The allocation with rnti={}, type={}, crbs={} was not registered in the cell resource grid",
                         test_grant.rnti,
                         fmt::underlying(test_grant.type),
                         test_grant.grant.crbs);
    }
  }
}

void ocudu::test_scheduler_result_consistency(const cell_configuration&      cell_cfg,
                                              const cell_resource_allocator& cell_res_grid)
{
  test_scheduler_result_consistency(cell_cfg, cell_res_grid[0].slot, cell_res_grid[0].result_reader());
  assert_pdcch_pdsch_common_consistency(cell_cfg, cell_res_grid);
  assert_dl_resource_grid_filled(cell_cfg, cell_res_grid);
  assert_rar_grant_msg3_pusch_consistency(cell_cfg, cell_res_grid);
}

static slot_interval get_rar_window(const cell_configuration& cell_cfg, slot_point rach_slot_rx)
{
  slot_point rar_win_start;
  for (unsigned i = 1; i != rach_slot_rx.nof_slots_per_frame(); ++i) {
    if (cell_cfg.is_dl_enabled(rach_slot_rx + i)) {
      rar_win_start = rach_slot_rx + i;
      break;
    }
  }
  return {rar_win_start,
          rar_win_start + cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.ra_resp_window};
}

/// Helper to check if Msg3 grant is consistent with the RACH preamble.
static bool is_rar_ul_grant_consistent_with_rach_preamble(const cell_configuration&                cell_cfg,
                                                          const rar_ul_grant&                      rar_grant,
                                                          const rach_indication_message::preamble& preamb)
{
  return rar_grant.temp_crnti == preamb.tc_rnti and rar_grant.rapid == preamb.preamble_id and
         rar_grant.ta == preamb.time_advance.to_Ta(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs);
}

/// Helper to check if Msg3 newTx PUSCH grant is consistent with the RAR UL grant.
static bool is_msg3_newtx_consistent_with_rar_ul_grant(const cell_configuration& cell_cfg,
                                                       const ul_sched_info&      ulgrant,
                                                       const rar_ul_grant&       rargrant)
{
  if (ulgrant.context.ue_index != INVALID_DU_UE_INDEX or ulgrant.context.nof_retxs != 0) {
    // Not Msg3 newTx.
    return false;
  }
  if (ulgrant.pusch_cfg.rnti != rargrant.temp_crnti) {
    return false;
  }

  const auto& td_list =
      get_pusch_time_domain_resource_table(*cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common);
  if (rargrant.time_resource_assignment >= td_list.size()) {
    return false;
  }
  const auto&    td_res = td_list[rargrant.time_resource_assignment];
  const unsigned expected_delay =
      ra_helper::get_msg3_delay(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs, td_res.k2) +
      cell_cfg.ntn_ul_alloc_delay_slots();
  if (*ulgrant.context.msg3_delay != expected_delay or ulgrant.pusch_cfg.symbols != td_res.symbols) {
    return false;
  }

  if (ulgrant.pusch_cfg.mcs_index.value() != rargrant.mcs or ulgrant.pusch_cfg.rv_index != 0) {
    return false;
  }

  if (not ulgrant.pusch_cfg.rbs.is_type1() or not ulgrant.pusch_cfg.rbs.any()) {
    return false;
  }
  const unsigned N_rb_ul_bwp = cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  const auto     vrbs        = ulgrant.pusch_cfg.rbs.type1();
  const uint8_t  freq_res =
      ra_frequency_type1_get_riv(ra_frequency_type1_configuration{N_rb_ul_bwp, vrbs.start(), vrbs.length()});

  return rargrant.freq_resource_assignment == freq_res;
}

/// Helper to check if RAR UL grant reTx is consistent with respective UL-PDCCH.
static bool is_msg3_retx_consistent_with_ul_pdcch(const cell_configuration&   cell_cfg,
                                                  const ul_sched_info&        ulgrant,
                                                  const pdcch_ul_information& ul_pdcch)
{
  if (ul_pdcch.dci.type() != dci_ul_rnti_config_type::tc_rnti_f0_0) {
    // Not Msg3 PDCCH.
    return false;
  }
  if (ulgrant.context.ue_index != INVALID_DU_UE_INDEX or ulgrant.context.nof_retxs == 0) {
    // Not Msg3 reTx.
    return false;
  }
  if (ulgrant.pusch_cfg.rnti != ul_pdcch.ctx.rnti or ulgrant.context.ss_id != ul_pdcch.ctx.context.ss_id) {
    return false;
  }

  const auto& dci = ul_pdcch.dci.as_tc_rnti_f0_0();
  const auto& td_list =
      get_pusch_time_domain_resource_table(*cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common);
  if (dci.time_resource >= td_list.size()) {
    return false;
  }
  const auto& td_res = td_list[dci.time_resource];
  if (ulgrant.context.k2 != td_res.k2 or ulgrant.pusch_cfg.symbols != td_res.symbols) {
    return false;
  }

  const unsigned N_rb_ul_bwp = cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
  const auto     vrbs        = ulgrant.pusch_cfg.rbs.type1();
  const uint8_t  freq_res =
      ra_frequency_type1_get_riv(ra_frequency_type1_configuration{N_rb_ul_bwp, vrbs.start(), vrbs.length()});

  return dci.frequency_resource == freq_res and dci.modulation_coding_scheme == ulgrant.pusch_cfg.mcs_index.value() and
         dci.redundancy_version == ulgrant.pusch_cfg.rv_index;
}

test_helper::ra_scheduler_tracker::ra_scheduler_tracker(const cell_configuration& cell_cfg_) : cell_cfg(cell_cfg_)
{
  const rach_config_common& rach_cfg  = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  const prach_configuration prach_cfg = prach_configuration_get(band_helper::get_freq_range(cell_cfg.band()),
                                                                band_helper::get_duplex_mode(cell_cfg.band()),
                                                                rach_cfg.rach_cfg_generic.prach_config_index);
  prach_duration_slots                = get_prach_duration_info(prach_cfg, cell_cfg.scs_common()).prach_length_slots;
}

void test_helper::ra_scheduler_tracker::on_new_rach_ind(const rach_indication_message& ind)
{
  const rach_config_common& rach_cfg = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  for (const auto& occ : ind.occasions) {
    const slot_interval rar_window = get_rar_window(cell_cfg, ind.slot_rx);
    const unsigned      slot_idx   = occ.slot_index;
    for (const auto& preamb : occ.preambles) {
      if (is_msga_preamble(preamb.preamble_id)) {
        const rach_config_common_two_step& two_step_cfg = *rach_cfg.two_step_rach_cfg;
        msga_preamble_context              ctxt;
        ctxt.msgb_rnti        = ra_helper::get_msgb_rnti(slot_idx, occ.start_symbol, occ.frequency_index);
        ctxt.ra_rnti          = ra_helper::get_ra_rnti(slot_idx, occ.start_symbol, occ.frequency_index);
        ctxt.preamble_id      = preamb.preamble_id;
        ctxt.tc_rnti          = preamb.tc_rnti;
        ctxt.pusch_slot       = ind.slot_rx + two_step_cfg.pusch.td_offset;
        ctxt.msgb_window_stop = ind.slot_rx + prach_duration_slots + two_step_cfg.msgB_response_window_slots;
        pending_msga_preambles.push_back(ctxt);
      } else {
        preamble_context ctxt;
        ctxt.ra_rnti  = ra_helper::get_ra_rnti(slot_idx, occ.start_symbol, occ.frequency_index);
        ctxt.preamble = preamb;
        ctxt.rar_win  = rar_window;
        if (ra_helper::is_msg1_cf_preamble(rach_cfg, preamb.preamble_id)) {
          // A retransmitted contention-free preamble carries the C-RNTI of the attempt that the UE abandoned.
          pending_preambles.erase(std::remove_if(pending_preambles.begin(),
                                                 pending_preambles.end(),
                                                 [&preamb](const preamble_context& prev) {
                                                   return prev.preamble.tc_rnti == preamb.tc_rnti;
                                                 }),
                                  pending_preambles.end());
        }
        pending_preambles.push_back(ctxt);
      }
    }
  }
}

void test_helper::ra_scheduler_tracker::on_crc_indication(const ul_crc_indication& crc)
{
  for (const auto& pdu : crc.crcs) {
    auto it = std::find_if(pending_preambles.begin(), pending_preambles.end(), [&pdu](const preamble_context& preamb) {
      return preamb.preamble.tc_rnti == pdu.rnti;
    });
    if (it != pending_preambles.end()) {
      // Msg3 is being ACKed/NACKed.
      ASSERT_EQ(it->last_msg3_slot, crc.sl_rx)
          << fmt::format("CRC before Msg3 slot? ({} != {})", it->last_msg3_slot, crc.sl_rx);
      if (not it->acked and pdu.tb_crc_success) {
        msg3_ack_counter++;
        it->acked = true;
      }
    }
  }
}

void test_helper::ra_scheduler_tracker::on_new_result(slot_point sl_tx, sched_slot_result_reader result)
{
  for (auto& dl_pdcch : result.dl().dl_pdcchs()) {
    if (dl_pdcch.dci.type() != dci_dl_rnti_config_type::ra_f1_0) {
      // Skip PDCCHs that are not for RAR or MsgB.
      continue;
    }
    // Determine whether this is a MsgB PDCCH (2-step) or a regular RAR PDCCH (4-step).
    const bool is_msgb =
        std::any_of(pending_msga_preambles.begin(),
                    pending_msga_preambles.end(),
                    [&dl_pdcch](const msga_preamble_context& p) { return p.msgb_rnti == dl_pdcch.ctx.rnti; });
    if (not is_msgb) {
      auto it =
          std::find_if(pending_preambles.begin(), pending_preambles.end(), [&dl_pdcch](const preamble_context& preamb) {
            return preamb.ra_rnti == dl_pdcch.ctx.rnti;
          });
      ASSERT_NE(it, pending_preambles.end()) << "RA-RNTI scheduled with no associated RACH preamble";
    }
    ++ra_dl_pdcch_ack_counter;

    const auto& td_res = get_ra_rnti_pdsch_time_domain_list(
        cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common,
        cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.cp,
        cell_cfg.params.dmrs_typeA_pos)[dl_pdcch.dci.as_ra_rnti_f1_0().time_resource];
    auto& expected_rar      = pending_rars.emplace_back();
    expected_rar.pdcch      = dl_pdcch;
    expected_rar.pdcch_slot = sl_tx;
    expected_rar.rar_slot   = sl_tx + td_res.k0;
  }

  for (auto& rar : result.dl().rar_grants()) {
    // Check if the DL PDCCH matches the RAR/MsgB content.
    auto expected_rar_it =
        std::find_if(pending_rars.begin(), pending_rars.end(), [&sl_tx, &rar](const rar_context& pending) {
          return pending.rar_slot == sl_tx and pending.pdcch.ctx.rnti == rar.pdsch_cfg.rnti;
        });
    ASSERT_NE(expected_rar_it, pending_rars.end()) << "RAR scheduled with no associated PDCCH";
    ASSERT_NO_FATAL_FAILURE(assert_pdcch_pdsch_common_consistency(cell_cfg, expected_rar_it->pdcch, rar.pdsch_cfg));
    expected_rar_it->scheduled = true;
    ++rar_counter;

    const auto pusch_td_list =
        get_pusch_time_domain_resource_table(*cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common);
    for (const auto& ul_grant : rar.grants) {
      const bool is_two_step_success  = std::holds_alternative<rar_ul_grant::two_step_success_info>(ul_grant.type);
      const bool is_two_step_fallback = std::holds_alternative<rar_ul_grant::two_step_fallback_info>(ul_grant.type);
      if (is_two_step_success or is_two_step_fallback) {
        // 2-step RACH MsgB grant (SuccessRAR or FallbackRAR).
        auto msga_it =
            std::find_if(pending_msga_preambles.begin(),
                         pending_msga_preambles.end(),
                         [&ul_grant](const msga_preamble_context& p) { return p.tc_rnti == ul_grant.temp_crnti; });
        ASSERT_NE(msga_it, pending_msga_preambles.end()) << "MsgB grant has no associated MsgA preamble";
        ASSERT_TRUE(msga_it->pusch_sched) << "MsgB cannot be scheduled before MsgA PUSCH";
        ASSERT_FALSE(msga_it->msgb_sched) << "Duplicate MsgB grant for the same MsgA preamble";
        ASSERT_LE(sl_tx, msga_it->msgb_window_stop) << "MsgB scheduled outside of MsgB window";
        msga_it->msgb_sched = true;

        if (is_two_step_success) {
          ++success_rar_counter;
        } else {
          // FallbackRAR: set up Msg3 tracking in pending_preambles so the rest of the Msg3 logic applies.
          ++fallback_rar_counter;
          preamble_context& p = pending_preambles.emplace_back();
          p.preamble.tc_rnti  = ul_grant.temp_crnti;
          p.rar_slot          = sl_tx;
          p.first_msg3_slot =
              sl_tx +
              ra_helper::get_msg3_delay(cell_cfg.scs_common(), pusch_td_list[ul_grant.time_resource_assignment].k2) +
              cell_cfg.ntn_ul_alloc_delay_slots();
          p.first_grant = ul_grant;
        }
      } else {
        // 4-step RAR UL grant: find matching RACH preamble.
        auto it = std::find_if(
            pending_preambles.begin(), pending_preambles.end(), [&rar, &ul_grant](const preamble_context& preamb) {
              return preamb.ra_rnti == rar.pdsch_cfg.rnti and preamb.preamble.tc_rnti == ul_grant.temp_crnti;
            });
        ASSERT_NE(it, pending_preambles.end()) << "RAR scheduled with an RAR UL grant with no associated RACH preamble";

        auto& ctxt = *it;
        ASSERT_FALSE(ctxt.rar_slot.valid()) << "Duplicate RAR per RACH preamble";
        ctxt.rar_slot = sl_tx;
        ASSERT_TRUE(ctxt.rar_win.contains(sl_tx)) << "RAR outside of RAR window";
        ASSERT_TRUE(is_rar_ul_grant_consistent_with_rach_preamble(cell_cfg, ul_grant, ctxt.preamble));
        ctxt.first_msg3_slot =
            ctxt.rar_slot +
            ra_helper::get_msg3_delay(cell_cfg.scs_common(), pusch_td_list[ul_grant.time_resource_assignment].k2) +
            cell_cfg.ntn_ul_alloc_delay_slots();
        ctxt.first_grant = ul_grant;
      }
    }
  }

  // Handle Msg3 reTxs.
  const auto& pusch_td_list =
      get_pusch_time_domain_resource_table(*cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common);
  for (auto& ul_pdcch : result.dl().ul_pdcchs()) {
    if (ul_pdcch.dci.type() != dci_ul_rnti_config_type::tc_rnti_f0_0) {
      continue;
    }

    auto preamble_it =
        std::find_if(pending_preambles.begin(), pending_preambles.end(), [&ul_pdcch](const preamble_context& preamb) {
          return preamb.preamble.tc_rnti == ul_pdcch.ctx.rnti;
        });
    ASSERT_NE(preamble_it, pending_preambles.end()) << "Msg3 reTx UL PDCCH has no associated RACH preamble";
    ASSERT_GT(sl_tx, preamble_it->first_msg3_slot) << "Msg3 reTx UL PDCCH cannot be before Msg3 newTx";

    const auto& dci    = ul_pdcch.dci.as_tc_rnti_f0_0();
    const auto& td_res = pusch_td_list[dci.time_resource];

    const slot_point pusch_slot      = sl_tx + td_res.k2 + cell_cfg.ntn_ul_alloc_delay_slots();
    auto             pending_msg3_it = std::find_if(pending_msg3_retxs.begin(),
                                        pending_msg3_retxs.end(),
                                        [&ul_pdcch, pusch_slot](const msg3_retx_context& pending_msg3) {
                                          return pending_msg3.pdcch.ctx.rnti == ul_pdcch.ctx.rnti and
                                                 pending_msg3.pusch_slot == pusch_slot;
                                        });
    ASSERT_EQ(pending_msg3_it, pending_msg3_retxs.end()) << "Duplicate Msg3 reTx UL PDCCH";

    auto& pending_msg3      = pending_msg3_retxs.emplace_back();
    pending_msg3.pdcch      = ul_pdcch;
    pending_msg3.pdcch_slot = sl_tx;
    pending_msg3.pusch_slot = pusch_slot;
  }

  for (auto& pusch : result.ul().puschs()) {
    if (pusch.context.ue_index != INVALID_DU_UE_INDEX) {
      continue;
    }

    if (not pusch.context.msg3_delay.has_value() and pusch.context.nof_retxs == 0) {
      // MsgA PUSCH (2-step RACH): find in pending_msga_preambles. As per TS 38.211, 6.3.1.1, MsgA PUSCH is scheduled
      // and decoded using the RA-RNTI, not the preamble's TC-RNTI. Multiple preambles in the same PRACH occasion
      // share the same RA-RNTI, so the RAPID is also needed to disambiguate.
      ASSERT_TRUE(pusch.context.rapid.has_value()) << "MsgA PUSCH context is missing its RAPID";
      auto it = std::find_if(
          pending_msga_preambles.begin(), pending_msga_preambles.end(), [&pusch](const msga_preamble_context& p) {
            return p.ra_rnti == pusch.pusch_cfg.rnti and p.preamble_id == *pusch.context.rapid;
          });
      ASSERT_NE(it, pending_msga_preambles.end()) << "MsgA PUSCH has no associated MsgA preamble";
      ASSERT_EQ(it->pusch_slot, sl_tx) << "MsgA PUSCH scheduled in slot not matching the TD offset";
      it->pusch_sched = true;
      ++msga_pusch_counter;
    } else {
      // Msg3 PUSCH (4-step RACH or FallbackRAR): find in pending_preambles.
      auto it =
          std::find_if(pending_preambles.begin(), pending_preambles.end(), [&pusch](const preamble_context& preamb) {
            return pusch.pusch_cfg.rnti == preamb.preamble.tc_rnti;
          });
      ASSERT_NE(it, pending_preambles.end()) << "Msg3 PUSCH has no associated RACH preamble";
      auto& ctxt          = *it;
      ctxt.last_msg3_slot = sl_tx;

      if (pusch.context.nof_retxs == 0) {
        // First Msg3 tx.
        ASSERT_TRUE(ctxt.rar_slot.valid()) << "Msg3 PUSCH before RAR";
        ++msg3_newtx_counter;
        ASSERT_EQ(ctxt.first_msg3_slot, sl_tx);
        ASSERT_EQ(ctxt.first_msg3_slot - ctxt.rar_slot, *pusch.context.msg3_delay);
        ASSERT_TRUE(is_msg3_newtx_consistent_with_rar_ul_grant(cell_cfg, pusch, ctxt.first_grant));
      } else {
        ++msg3_retx_counter;
        ASSERT_GT(sl_tx, ctxt.first_msg3_slot);

        // PDCCH and Msg3 PUSCH match in content.
        auto expected_msg3_it = std::find_if(pending_msg3_retxs.begin(),
                                             pending_msg3_retxs.end(),
                                             [&pusch, sl_tx](const msg3_retx_context& pending_msg3) {
                                               return pending_msg3.pdcch.ctx.rnti == pusch.pusch_cfg.rnti and
                                                      pending_msg3.pusch_slot == sl_tx;
                                             });
        ASSERT_NE(expected_msg3_it, pending_msg3_retxs.end()) << "Msg3 reTx PUSCH has no associated UL PDCCH";
        ASSERT_TRUE(is_msg3_retx_consistent_with_ul_pdcch(cell_cfg, pusch, expected_msg3_it->pdcch));
        pending_msg3_retxs.erase(expected_msg3_it);
      }
    }
  }

  // Pop expired expected RARs.
  while (not pending_rars.empty() and pending_rars.front().rar_slot >= sl_tx) {
    ASSERT_TRUE(pending_rars.front().scheduled) << "DL PDCCH was scheduled but RAR was not";
    pending_rars.pop_front();
  }

  // Look for pending Msg3 reTxs, whose PUSCH was not scheduled.
  for (auto it = pending_msg3_retxs.begin(); it != pending_msg3_retxs.end(); ++it) {
    ASSERT_GT(it->pusch_slot, sl_tx) << fmt::format(
        "UL PDCCH in slot {} did not lead to a Msg3 reTx PUSCH in slot {} for tc-rnti={}",
        it->pdcch_slot,
        it->pusch_slot,
        it->pdcch.ctx.rnti);
  }

  // Pop expired RACH preambles.
  while (not pending_preambles.empty() and is_expired(pending_preambles.front(), sl_tx)) {
    pending_preambles.pop_front();
  }

  // Pop expired MsgA preambles (MsgB window passed without scheduling or MsgB was scheduled).
  auto new_end =
      std::remove_if(pending_msga_preambles.begin(),
                     pending_msga_preambles.end(),
                     [sl_tx](const msga_preamble_context& p) { return p.msgb_sched or p.msgb_window_stop < sl_tx; });
  pending_msga_preambles.erase(new_end, pending_msga_preambles.end());
}

bool test_helper::ra_scheduler_tracker::is_msga_preamble(uint8_t preamble_id) const
{
  const rach_config_common& rach_cfg = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  return ra_helper::is_msga_cb_preamble(rach_cfg, preamble_id);
}

bool test_helper::ra_scheduler_tracker::is_expired(const preamble_context& ctxt, slot_point sl_tx) const
{
  if (ctxt.acked) {
    return true;
  }
  if (not ctxt.rar_slot.valid() and ctxt.rar_win.stop() < sl_tx) {
    // RAR window expired.
    return true;
  }
  static constexpr unsigned max_slots_for_msg3_ack = 256;
  if (ctxt.last_msg3_slot.valid() and ctxt.last_msg3_slot + max_slots_for_msg3_ack < sl_tx) {
    // Msg3 was not ACKed.
    return true;
  }
  return false;
}

/// Removes the entries of \c rnti from a map keyed by HARQ.
template <typename HarqMap>
static void erase_ue_harqs(HarqMap& harqs, rnti_t rnti)
{
  for (auto it = harqs.begin(); it != harqs.end();) {
    it = it->first.rnti == rnti ? harqs.erase(it) : std::next(it);
  }
}

void test_helper::harq_tracker::on_ue_added(rnti_t rnti)
{
  erase_ue_harqs(dl_harqs, rnti);
  erase_ue_harqs(ul_harqs, rnti);
  erase_ue_harqs(pending_ul_dcis, rnti);
}

void test_helper::harq_tracker::on_new_result(slot_point /*sl_tx*/, sched_slot_result_reader result)
{
  // Build DL NDI map from DL PDCCHs (UE-specific formats with NDI).
  std::unordered_map<harq_key, bool, harq_key_hash> dl_ndi_map;
  for (const pdcch_dl_information& pdcch : result.dl().dl_pdcchs()) {
    bool     ndi     = false;
    unsigned harq_id = 0;
    switch (pdcch.dci.type()) {
      case dci_dl_rnti_config_type::c_rnti_f1_0:
        ndi     = pdcch.dci.as_c_rnti_f1_0().new_data_indicator != 0;
        harq_id = pdcch.dci.as_c_rnti_f1_0().harq_process_number;
        break;
      case dci_dl_rnti_config_type::tc_rnti_f1_0:
        ndi     = pdcch.dci.as_tc_rnti_f1_0().new_data_indicator != 0;
        harq_id = pdcch.dci.as_tc_rnti_f1_0().harq_process_number;
        break;
      case dci_dl_rnti_config_type::c_rnti_f1_1:
        ndi     = pdcch.dci.as_c_rnti_f1_1().tb1_new_data_indicator != 0;
        harq_id = pdcch.dci.as_c_rnti_f1_1().harq_process_number;
        break;
      default:
        continue;
    }
    dl_ndi_map[{pdcch.ctx.rnti, static_cast<harq_id_t>(harq_id)}] = ndi;
  }

  for (const dl_msg_alloc& grant : result.dl().ue_grants()) {
    const pdsch_information& pdsch  = grant.pdsch_cfg;
    const pdsch_codeword&    cw     = pdsch.codewords[0];
    const harq_key           key    = {pdsch.rnti, pdsch.harq_id};
    const auto               ndi_it = dl_ndi_map.find(key);
    if (ndi_it == dl_ndi_map.end()) {
      // PDCCH-less PDSCH: only allowed for Rel-16 PDSCH repetition occasions, which re-transmit the TB of a PDSCH
      // scheduled in a preceding slot.
      ASSERT_FALSE(cw.new_data) << fmt::format(
          "DL UE grant has no matching PDCCH for rnti={} h_id={}", pdsch.rnti, pdsch.harq_id);
      const auto rep_it = dl_harqs.find(key);
      ASSERT_NE(rep_it, dl_harqs.end()) << "PDSCH repetition occasion but no previous PDSCH for the same HARQ";
      ASSERT_EQ(rep_it->second.tbs, cw.tb_size_bytes) << "TBS mismatch for PDSCH repetition occasion";
      continue;
    }
    const bool ndi = ndi_it->second;
    const auto it  = dl_harqs.find(key);
    if (cw.new_data) {
      // newTx. TS 38.321, clause 5.3.2.2: the UE only takes the TB as new data if the NDI is toggled with respect to
      // the previous transmission it received for this HARQ process.
      if (it != dl_harqs.end()) {
        ASSERT_NE(it->second.ndi, ndi) << fmt::format(
            "NDI must toggle in a DL HARQ newTx (rnti={} h_id={})", pdsch.rnti, fmt::underlying(pdsch.harq_id));
      }
      dl_harqs[key] = {ndi, cw.tb_size_bytes};
    } else {
      // reTx.
      ASSERT_NE(it, dl_harqs.end()) << "PDSCH reTx but no previous PDSCH for the same HARQ";
      ASSERT_EQ(it->second.ndi, ndi) << "NDI cannot flip in a HARQ reTx";
      ASSERT_EQ(it->second.tbs, cw.tb_size_bytes) << "TBS mismatch for DL HARQ reTx";
    }
  }

  // Save the NDI of the UL PDCCHs (C-RNTI formats with NDI; tc_rnti_f0_0 has none) until their PUSCH, k2 slots later.
  for (const pdcch_ul_information& pdcch : result.dl().ul_pdcchs()) {
    bool     ndi     = false;
    unsigned harq_id = 0;
    switch (pdcch.dci.type()) {
      case dci_ul_rnti_config_type::c_rnti_f0_0:
        ndi     = pdcch.dci.as_c_rnti_f0_0().new_data_indicator != 0;
        harq_id = pdcch.dci.as_c_rnti_f0_0().harq_process_number;
        break;
      case dci_ul_rnti_config_type::c_rnti_f0_1:
        ndi     = pdcch.dci.as_c_rnti_f0_1().new_data_indicator != 0;
        harq_id = pdcch.dci.as_c_rnti_f0_1().harq_process_number;
        break;
      default:
        continue;
    }
    pending_ul_dcis[{pdcch.ctx.rnti, static_cast<harq_id_t>(harq_id)}] = ndi;
  }

  for (const ul_sched_info& sched_info : result.ul().puschs()) {
    const pusch_information& pusch  = sched_info.pusch_cfg;
    const harq_key           key    = {pusch.rnti, pusch.harq_id};
    const auto               ndi_it = pending_ul_dcis.find(key);
    if (ndi_it == pending_ul_dcis.end()) {
      // No matching PDCCH (e.g. Msg3 newTx, MsgA-PUSCH, CGs, PUSCH repetition occasions) — skip NDI tracking.
      continue;
    }
    const bool ndi = ndi_it->second;
    pending_ul_dcis.erase(ndi_it);
    const auto it = ul_harqs.find(key);
    if (pusch.new_data) {
      // newTx. TS 38.321, clause 5.4.2.1: the UE only builds a new MAC PDU if the NDI is toggled with respect to the
      // previous transmission of this HARQ process.
      if (it != ul_harqs.end()) {
        ASSERT_NE(it->second.ndi, ndi) << fmt::format(
            "NDI must toggle in a UL HARQ newTx (rnti={} h_id={})", pusch.rnti, fmt::underlying(pusch.harq_id));
      }
      ul_harqs[key] = {ndi, pusch.tb_size_bytes};
    } else {
      // reTx.
      ASSERT_NE(it, ul_harqs.end()) << "PUSCH reTx but no previous PUSCH for the same HARQ";
      ASSERT_EQ(it->second.ndi, ndi) << "NDI cannot flip in a UL HARQ reTx";
      ASSERT_EQ(it->second.tbs, pusch.tb_size_bytes) << "TBS mismatch for UL HARQ reTx";
    }
  }
}
