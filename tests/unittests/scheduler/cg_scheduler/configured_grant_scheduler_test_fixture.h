// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief Fixture shared by the CG Type 1 and Type 2 scheduler tests.

#pragma once

#include "tests/test_doubles/scheduler/cell_config_builder_profiles.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "tests/test_doubles/scheduler/scheduler_result_finder.h"
#include "tests/unittests/scheduler/test_utils/scheduler_test_simulator.h"
#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/ran/resource_allocation/resource_allocation_frequency.h"
#include "ocudu/scheduler/config/cg_builder_params.h"
#include "ocudu/scheduler/config/pucch_resource_builder_params.h"
#include "ocudu/scheduler/resource_grid_util.h"
#include <chrono>
#include <gtest/gtest.h>
#include <optional>
#include <string>

namespace ocudu {

/// Parameters of a CG test scenario.
struct cg_test_params {
  cg_configuration::periodicity_t periodicity    = cg_configuration::periodicity_t::sl40;
  unsigned                        period_slots   = 40;
  unsigned                        mcs            = 5;
  unsigned                        nof_harq_procs = 4;
  /// When false, the cell is built without CG configured at cell level, so the CG scheduler is not instantiated.
  bool cell_cg_enabled = true;
  /// CG type. Type 2 requires an activation DCI before any CG PUSCH is scheduled.
  cg_builder_params::cg_type type = cg_builder_params::cg_type::type1;
  /// \brief Number of RBs available for CG at cell level.
  ///
  /// Each CG resource takes the number of PRBs one UE needs (10, at the default grant size and MCS), so the default
  /// leaves a single resource per slot offset.
  unsigned max_nof_cell_cg_rbs = 10;
  /// When true, the cell's HARQ-ACK PUCCH resources are configured with repetitions, which the UE always uses.
  bool pucch_rep_enabled = false;
  /// When true, periodic SRS is enabled in the cell, which reserves the last symbols of every UL slot.
  bool srs_enabled = false;
  /// \brief When true, a second entry is appended to the cell PUSCH time domain resource list.
  ///
  /// With a single entry, the time domain allocation index of a Type 2 activation DCI carries no information. A second,
  /// distinct entry makes a mismatch between the index in the DCI and the one the CG occasions use observable.
  bool extra_pusch_td_alloc = false;
};

/// \brief Second PUSCH time domain resource allocation, added when \c cg_test_params::extra_pusch_td_alloc is set.
///
/// The cell configuration validator requires the list to be sorted by k2 and to be a subset of the entries derived
/// from the duplex configuration, so the entry differs from the first one only in its symbols.
inline constexpr pusch_time_domain_resource_allocation extra_pusch_td_res{
    .k2       = 4,
    .map_type = sch_mapping_type::typeA,
    .symbols  = {0, 12},
};

/// Duplex configuration of a CG test scenario.
struct cg_duplex_test_params {
  std::string                            name;
  duplex_mode                            mode;
  std::optional<tdd_ul_dl_config_common> tdd_pattern;
  unsigned                               default_cg_offset;
};

/// Formatter for duplex test params, used by gtest. Avoids the fallback raw-byte printer, which reads
/// uninitialized padding bytes.
inline void PrintTo(const cg_duplex_test_params& value, ::std::ostream* os)
{
  *os << fmt::format("{}, default_cg_offset={}", value.name, value.default_cg_offset);
}

/// Builds cell_config_builder_params from duplex test params.
inline cell_config_builder_params make_cell_builder_params(const cg_duplex_test_params& p)
{
  auto params = cell_config_builder_profiles::create(p.mode);
  if (p.tdd_pattern.has_value()) {
    params.tdd_ul_dl_cfg_common = p.tdd_pattern.value();
  }
  return params;
}

/// CG period of a parameterised test.
struct cg_period_test_params {
  cg_configuration::periodicity_t periodicity;
  unsigned                        period_slots;
};

/// Formatter for period test params, used by gtest. Avoids the fallback raw-byte printer.
inline void PrintTo(const cg_period_test_params& value, ::std::ostream* os)
{
  *os << fmt::format("period={} slots", value.period_slots);
}

/// Base class for all configured_grant_scheduler tests. Provides cell + UE setup with CG enabled and
/// helper methods to find CG grants in the scheduler output.
class configured_grant_scheduler_test : public scheduler_test_simulator, public ::testing::Test
{
protected:
  static constexpr rnti_t ue_crnti = to_rnti(0x4601);
  /// CS-RNTI assigned to the test UE.
  static constexpr rnti_t cs_rnti = to_rnti(0xe0ef);

  cg_test_params                           cg_params;
  sched_cell_configuration_request_message cell_req;
  unsigned                                 default_cg_offset_;

  explicit configured_grant_scheduler_test(const cg_test_params&             params_         = {},
                                           const cell_config_builder_params& builder_params  = {},
                                           unsigned                          default_offset  = 0,
                                           std::chrono::milliseconds         ntn_cs_koffset_ = {},
                                           std::chrono::milliseconds         ntn_k_mac_      = {}) :
    scheduler_test_simulator(/*tx_rx_delay=*/4, builder_params.scs_common),
    cg_params(params_),
    default_cg_offset_(default_offset)
  {
    cell_req = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    if (cg_params.srs_enabled) {
      // Set before the CG configuration below: the number of PRBs a CG grant needs is derived from the PUSCH symbols
      // left free by SRS.
      cell_req.ran.init_bwp.srs_cfg.srs_type_enabled = srs_type::periodic;
      // SRS sits at the end of the slot, so the PUCCH resources are capped to the symbols SRS leaves free, as the DU
      // does through config_helpers::compute_max_nof_pucch_symbols().
      pucch_helper::cap_pucch_symbols(cell_req.ran.init_bwp.pucch.resources,
                                      NOF_OFDM_SYM_PER_SLOT_NORMAL_CP -
                                          cell_req.ran.init_bwp.srs_cfg.max_nof_symbols.value());
    }
    if (cg_params.cell_cg_enabled) {
      cell_req.ran.init_bwp.cg_cfg = cg_builder_params{
          .periodicity         = cg_params.periodicity,
          .type                = cg_params.type,
          .mcs                 = cg_params.mcs,
          .nof_harq_processes  = cg_params.nof_harq_procs,
          .max_nof_cell_cg_rbs = cg_params.max_nof_cell_cg_rbs,
      };
    }
    if (cg_params.pucch_rep_enabled) {
      // SINR thresholds that no SINR estimate reaches, so that n4 repetitions are always recommended.
      auto& pucch_res = cell_req.ran.init_bwp.pucch.resources;
      pucch_res.harq_ack_rep =
          pucch_harq_ack_rep_params{.sinr_thresholds = {1000.0F, 1000.0F},
                                    .factors_per_res = std::vector<pucch_repetition_factor>(
                                        pucch_res.res_set_size.value(), pucch_repetition_factor::n4)};
    }
    if (cg_params.extra_pusch_td_alloc) {
      cell_req.ran.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list.push_back(extra_pusch_td_res);
    }
    if (ntn_cs_koffset_.count() != 0 or ntn_k_mac_.count() != 0) {
      cell_req.ran.ntn_params.emplace();
      cell_req.ran.ntn_params->ntn_cfg.cell_specific_koffset = ntn_cs_koffset_;
      if (ntn_k_mac_.count() != 0) {
        cell_req.ran.ntn_params->ntn_cfg.k_mac = ntn_k_mac_;
      }
    }
    add_cell(cell_req);
  }

  /// Adds the test UE to the scheduler with CG configuration.
  /// CG is applied via a reconfiguration (not at creation) to match the DRB-gated CG production flow,
  /// where add_reconf_ue() is the sole entry point for CG scheduler setup.
  void add_cg_ue(du_ue_index_t           ue_idx         = to_du_ue_index(0),
                 std::optional<unsigned> cg_slot_offset = std::nullopt,
                 rnti_t                  crnti          = ue_crnti,
                 rnti_t                  cs_rnti_val    = cs_rnti,
                 std::optional<unsigned> cg_start_vrb   = std::nullopt)
  {
    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = ue_idx;
    ue_req.crnti    = crnti;
    ASSERT_TRUE(ue_req.cfg.cells.has_value() and not ue_req.cfg.cells->empty());
    auto& ue_cell = *ue_req.cfg.cells->front().ded;
    ASSERT_TRUE(ue_cell.serv_cell_cfg.ul_config.has_value());
    ASSERT_TRUE(ue_cell.serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value());
    if (cg_params.pucch_rep_enabled) {
      // Same as the UE reporting full support for dynamic PUCCH repetition in its capabilities.
      ue_cell.init_bwp().ul.pucch.rep_f1_3_4_enabled = true;
      ue_cell.init_bwp().ul.pucch.rep_f0_2_enabled   = true;
    }
    ue_cell.serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg->rrc_configured_ul_grant_cfg->time_domain_offset =
        cg_slot_offset.value_or(default_cg_offset_);
    if (cg_start_vrb.has_value()) {
      auto& freq = std::get<ra_frequency_type1_configuration>(
          ue_cell.serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg->rrc_configured_ul_grant_cfg->freq_domain_res);
      freq.start_vrb = cg_start_vrb.value();
    }

    // Step 1: Create the UE without CG — add_reconf_ue() is the sole CG entry point, so CG setup
    // is deferred to the reconfiguration below. Wait for the creation event to be processed before
    // issuing the reconfiguration, since handle_ue_reconfiguration_request requires the UE to exist.
    auto ue_req_no_cg = ue_req;
    ue_req_no_cg.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();
    add_ue(ue_req_no_cg, /*wait_notification=*/true);

    // Step 2: Reconfigure to install the CG config, which triggers add_reconf_ue() in the scheduler.
    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = ue_idx;
    reconf.crnti    = crnti;
    reconf.cfg      = ue_req.cfg;
    reconf.cs_rnti  = cs_rnti_val;
    sched->handle_ue_reconfiguration_request(reconf);

    // Step 3: Confirm the UE applied the new config, so it exits the pending_reconf state. Otherwise, the UE stays in
    // fallback, where HARQ-ACKs are never allocated on CG slots (the UE might not have applied the CG config yet).
    sched->handle_ue_config_applied(ue_idx);
  }

  /// Advances the scheduler until a CG PUSCH for \c search_rnti is seen in the output, or \c max_slots is exhausted.
  const ul_sched_info* run_until_next_cg_pusch(unsigned max_slots = 0, rnti_t search_rnti = cs_rnti)
  {
    if (max_slots == 0) {
      // The CG PUSCH is booked max_ul_slot_alloc_delay slots ahead, so the first grant appears in last_sched_result()
      // max_ul_slot_alloc_delay + (period - 1) steps after UE creation. Add extra margin for safety.
      constexpr unsigned ul_delay = get_max_slot_ul_alloc_delay(/*ntn_ul_alloc_delay_slots=*/0);
      max_slots                   = ul_delay + cg_params.period_slots + 20;
    }
    for (unsigned i = 0; i != max_slots; ++i) {
      run_slot();
      const ul_sched_info* grant = find_ue_pusch(search_rnti, *last_sched_result());
      if (grant != nullptr) {
        return grant;
      }
    }
    return nullptr;
  }

  /// \brief Sends a UE reconfiguration with modified CG parameters.
  ///
  /// The CG period is deliberately not reconfigurable here: every UE of a cell uses the cell's CG period.
  void reconf_cg(std::optional<unsigned> new_offset = std::nullopt, std::optional<unsigned> new_mcs = std::nullopt)
  {
    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = to_du_ue_index(0);
    ue_req.crnti    = ue_crnti;
    auto& cg_cfg    = ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.value();

    if (new_mcs.has_value()) {
      cg_cfg.rrc_configured_ul_grant_cfg->mcs = new_mcs.value();
    }
    cg_cfg.rrc_configured_ul_grant_cfg->time_domain_offset = new_offset.value_or(default_cg_offset_);

    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = to_du_ue_index(0);
    reconf.crnti    = ue_crnti;
    reconf.cfg      = ue_req.cfg;
    reconf.cs_rnti  = cs_rnti;
    sched->handle_ue_reconfiguration_request(reconf);

    // Confirm the UE applied the new config, so it exits the pending_reconf state (see add_cg_ue()).
    sched->handle_ue_config_applied(to_du_ue_index(0));
  }

  /// Sends a UE reconfiguration that removes the CG configuration (cg_cfg reset to nullopt).
  void remove_cg_via_reconfig() const
  {
    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = to_du_ue_index(0);
    ue_req.crnti    = ue_crnti;
    ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();

    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = to_du_ue_index(0);
    reconf.crnti    = ue_crnti;
    reconf.cfg      = ue_req.cfg;
    reconf.cs_rnti  = cs_rnti;
    sched->handle_ue_reconfiguration_request(reconf);
  }

  /// Sends a CRC indication for a CG PUSCH grant with the given CRC result and SINR.
  void send_cg_crc(const ul_sched_info& grant, slot_point pusch_slot, bool crc_ok, std::optional<float> sinr_dB) const
  {
    ul_crc_indication crc_ind;
    crc_ind.cell_index = to_du_cell_index(0);
    crc_ind.sl_rx      = pusch_slot;
    ul_crc_pdu_indication pdu{};
    pdu.rnti           = grant.pusch_cfg.rnti;
    pdu.ue_index       = grant.context.ue_index;
    pdu.harq_id        = grant.pusch_cfg.harq_id;
    pdu.tb_crc_success = crc_ok;
    pdu.ul_sinr_dB     = sinr_dB;
    crc_ind.crcs.push_back(pdu);
    sched->handle_crc_indication(crc_ind);
  }
};

} // namespace ocudu
