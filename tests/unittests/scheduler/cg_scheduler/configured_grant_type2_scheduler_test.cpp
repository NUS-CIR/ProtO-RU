// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief Unit tests for the configured_grant_scheduler. Verifies that the scheduler correctly places periodic
/// CG PUSCH grants in the right slots, and that the scheduler output (symbols, RBs, RNTI) matches the CG
/// configuration.

#include "tests/test_doubles/scheduler/cell_config_builder_profiles.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "tests/test_doubles/scheduler/scheduler_result_finder.h"
#include "tests/test_doubles/scheduler/scheduler_test_message_validators.h"
#include "tests/test_doubles/utils/test_rng_seed.h"
#include "tests/unittests/scheduler/test_utils/result_test_helpers.h"
#include "tests/unittests/scheduler/test_utils/scheduler_test_simulator.h"
#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/logical_channel/lcid.h"
#include "ocudu/ran/resource_allocation/resource_allocation_frequency.h"
#include "ocudu/scheduler/config/cg_builder_params.h"
#include "ocudu/scheduler/config/pucch_resource_builder_params.h"
#include "ocudu/scheduler/resource_grid_util.h"
#include <gtest/gtest.h>
#include <limits>

using namespace ocudu;

namespace {
/// Helper struct that holds parameters for a single CG test scenario.
struct cg_test_params {
  cg_configuration::periodicity_t periodicity    = cg_configuration::periodicity_t::sl40;
  unsigned                        period_slots   = 40;
  unsigned                        mcs            = 5;
  unsigned                        nof_harq_procs = 4;
  /// When false, the cell is built without CG configured at cell level, so the CG scheduler is not instantiated.
  bool cell_cg_enabled = true;
  /// Select Type 2 CG, which requires DCI activation before any CG PUSCH is scheduled.
  bool type2 = false;
  /// \brief Number of RBs available for CG at cell level.
  ///
  /// Each CG resource takes the number of PRBs one UE needs (10, at the default grant size and MCS), so the default
  /// leaves a single resource per slot offset and thus lets only one UE be activated on any given offset.
  unsigned max_nof_cell_cg_rbs = 10;
  /// When true, periodic SRS is enabled in the cell, which reserves the last symbols of every UL slot.
  bool srs_enabled = false;
  /// \brief When true, the UE-dedicated SearchSpace is set to monitor DCI formats 0_0/1_0 instead of 0_1/1_1.
  ///
  /// The CG Type 2 activation is only built as a DCI format 0_1, so a UE with such a SearchSpace cannot be activated.
  bool ue_ss_dci_f0_0 = false;
  /// \brief When true, a second entry is appended to the cell PUSCH time domain resource list.
  ///
  /// The default cell carries a single entry, which makes the time domain allocation index the Type 2 activation DCI
  /// announces carry no information: any index would resolve to the same k2 and symbols. A second, distinct entry
  /// gives that index a meaning, so that a divergence between the index put in the DCI and the one the CG occasions
  /// are built from becomes observable.
  bool extra_pusch_td_alloc = false;
};

/// \brief Second PUSCH time domain resource allocation, added when \c cg_test_params::extra_pusch_td_alloc is set.
///
/// Its symbols differ from those of the default first entry, which is what makes the time domain allocation index
/// observable. The cell configuration validator requires the list to be sorted by k2 (and, within the same k2, by
/// decreasing number of symbols) and to be a subset of the entries auto-derived from the duplex configuration, so the
/// entry can only differ in its symbol allocation.
constexpr pusch_time_domain_resource_allocation extra_pusch_td_res{
    .k2       = 4,
    .map_type = sch_mapping_type::typeA,
    .symbols  = {0, 12},
};

struct cg_duplex_test_params {
  std::string                            name;
  std::optional<tdd_ul_dl_config_common> tdd_pattern;
};

/// Formatter for duplex test params, used by gtest. Avoids the fallback raw-byte printer, which reads
/// uninitialized padding bytes.
void PrintTo(const cg_duplex_test_params& value, ::std::ostream* os)
{
  *os << value.name;
}

/// Builds cell_config_builder_params from duplex test params.
static cell_config_builder_params make_cell_builder_params(const cg_duplex_test_params& p)
{
  auto params = cell_config_builder_profiles::create(p.tdd_pattern.has_value() ? duplex_mode::TDD : duplex_mode::FDD);
  if (p.tdd_pattern.has_value()) {
    params.tdd_ul_dl_cfg_common = p.tdd_pattern.value();
  }
  return params;
}

/// \brief Grant parameters carried by a CG Type 2 activation DCI, decoded into the representation the CG PUSCH uses.
///
/// With CG Type 2 none of these is configured by RRC: the scheduler picks them when building the activation DCI and
/// stores them in the UE CG state manager, from which every subsequent CG PUSCH occasion is built.
struct cg_activation_grant_params {
  /// VRBs the UE is told to transmit the CG PUSCH on.
  vrb_interval vrbs;
  /// Index in the cell PUSCH time domain resource allocation list.
  unsigned time_resource;
  uint8_t  mcs;
};

/// Base class for all configured_grant_scheduler tests. Provides cell + UE setup with CG enabled and
/// helper methods to find CG grants in the scheduler output.
/// \note Kept with internal linkage: configured_grant_scheduler_test.cpp defines a class with the same name and a
/// different body, and both translation units are linked into the same test binary.
class configured_grant_scheduler_test : public scheduler_test_simulator, public ::testing::Test
{
protected:
  static constexpr rnti_t ue_crnti = to_rnti(0x4601);
  /// CS-RNTI assigned to the test UE. Matches the temporary value used in configured_grant_type1_rrm.cpp.
  static constexpr rnti_t cs_rnti = to_rnti(0xe0ef);

  cg_test_params                           cg_params;
  sched_cell_configuration_request_message cell_req;
  unsigned                                 default_cg_offset_;

  explicit configured_grant_scheduler_test(const cg_test_params&             params_        = {},
                                           const cell_config_builder_params& builder_params = {},
                                           unsigned                          default_offset = 0) :
    scheduler_test_simulator(/*tx_rx_delay=*/4, builder_params.scs_common),
    cg_params(params_),
    default_cg_offset_(default_offset)
  {
    cell_req = sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    if (cg_params.srs_enabled) {
      // Set before the CG configuration below: the number of PRBs a CG grant needs is derived from the PUSCH symbols
      // left free by SRS.
      cell_req.ran.init_bwp.srs_cfg.srs_type_enabled = srs_type::periodic;
      // SRS resources sit at the end of the slot, so the PUCCH resources have to be capped to the symbols SRS leaves
      // free, or the two collide in the UL grid. The DU does this through
      // config_helpers::compute_max_nof_pucch_symbols(), which computes the very same value but is not linked into
      // this test; its DU-level config validator rejects a cell where the cap is missing.
      pucch_helper::cap_pucch_symbols(cell_req.ran.init_bwp.pucch.resources,
                                      NOF_OFDM_SYM_PER_SLOT_NORMAL_CP -
                                          cell_req.ran.init_bwp.srs_cfg.max_nof_symbols.value());
    }
    if (cg_params.cell_cg_enabled) {
      cell_req.ran.init_bwp.cg_cfg = cg_builder_params{
          .periodicity        = cg_params.periodicity,
          .type               = cg_params.type2 ? cg_builder_params::cg_type::type2 : cg_builder_params::cg_type::type1,
          .mcs                = cg_params.mcs,
          .nof_harq_processes = cg_params.nof_harq_procs,
          .max_nof_cell_cg_rbs = cg_params.max_nof_cell_cg_rbs,
      };
    }
    if (cg_params.ue_ss_dci_f0_0) {
      cell_req.ran.init_bwp.pdcch_cfg->search_spaces[0].set_non_ss0_monitored_dci_formats(
          search_space_configuration::ue_specific_dci_format::f0_0_and_f1_0);
    }
    if (cg_params.extra_pusch_td_alloc) {
      cell_req.ran.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list.push_back(extra_pusch_td_res);
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
      constexpr unsigned ul_delay = get_max_slot_ul_alloc_delay(/*ntn_cs_koffset=*/0);
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

  /// Sends a UE reconfiguration with modified CG parameters.
  void reconf_cg(std::optional<cg_configuration::periodicity_t> new_periodicity = std::nullopt,
                 std::optional<unsigned>                        new_offset      = std::nullopt,
                 std::optional<unsigned>                        new_mcs         = std::nullopt)
  {
    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = to_du_ue_index(0);
    ue_req.crnti    = ue_crnti;
    auto& cg_cfg    = ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.value();

    if (new_periodicity.has_value()) {
      cg_cfg.periodicity = new_periodicity.value();
    }
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

  /// \brief Creates the UE without any CG configuration, and waits for the creation to be processed.
  ///
  /// CG is installed separately by install_type2_cg(), since add_reconf_ue() is the sole entry point for CG scheduler
  /// setup and a reconfiguration requires the UE to already exist. Keeping the two steps apart also lets a test
  /// install the CG on several UEs within the same slot.
  void create_ue_without_cg(du_ue_index_t ue_idx = to_du_ue_index(0), rnti_t crnti = ue_crnti)
  {
    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = ue_idx;
    ue_req.crnti    = crnti;
    ASSERT_TRUE(ue_req.cfg.cells.has_value() and not ue_req.cfg.cells->empty());
    ASSERT_TRUE(ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config.has_value());
    ASSERT_TRUE(ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value());
    ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();
    add_ue(ue_req, /*wait_notification=*/true);
  }

  /// \brief Installs the Type 2 CG configuration on an existing UE, which triggers add_reconf_ue() in the scheduler.
  ///
  /// Unlike the Type 1 flow, no RRC-ConfiguredUplinkGrant is touched: with Type 2 the grant parameters are only known
  /// once the activation DCI is sent.
  ///
  /// \param notify_config_applied When true, the configuration-applied indication is sent, which is what makes the
  /// scheduler consider the UE ready for Type 2 activation (start_ue_cg_activation()).
  void install_type2_cg(bool          notify_config_applied,
                        du_ue_index_t ue_idx      = to_du_ue_index(0),
                        rnti_t        crnti       = ue_crnti,
                        rnti_t        cs_rnti_val = cs_rnti)
  {
    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = ue_idx;
    ue_req.crnti    = crnti;
    ASSERT_TRUE(ue_req.cfg.cells.has_value() and not ue_req.cfg.cells->empty());
    ASSERT_TRUE(ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config.has_value());
    ASSERT_TRUE(ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value());

    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = ue_idx;
    reconf.crnti    = crnti;
    reconf.cfg      = ue_req.cfg;
    reconf.cs_rnti  = cs_rnti_val;
    sched->handle_ue_reconfiguration_request(reconf);

    if (notify_config_applied) {
      sched->handle_ue_config_applied(ue_idx);
    }
  }

  /// Adds a UE with a Type 2 CG configuration: creation followed by the CG-installing reconfiguration.
  void add_type2_cg_ue(bool          notify_config_applied,
                       du_ue_index_t ue_idx      = to_du_ue_index(0),
                       rnti_t        crnti       = ue_crnti,
                       rnti_t        cs_rnti_val = cs_rnti)
  {
    ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(ue_idx, crnti));
    ASSERT_NO_FATAL_FAILURE(install_type2_cg(notify_config_applied, ue_idx, crnti, cs_rnti_val));
  }

  /// \brief Advances the scheduler until an UL PDCCH for \c search_rnti shows up in the output.
  ///
  /// \return The slot the PDCCH was found in, or an invalid slot_point if \c max_slots was exhausted.
  /// \note The CG Type 2 activation DCI is addressed to the CS-RNTI, as per TS 38.213, Section 10.2, so searching
  /// for that RNTI already isolates it from any dynamic grant addressed to the C-RNTI.
  slot_point run_until_next_ul_pdcch(unsigned max_slots, rnti_t search_rnti = cs_rnti)
  {
    for (unsigned i = 0; i != max_slots; ++i) {
      run_slot();
      if (find_ue_ul_pdcch(search_rnti) != nullptr) {
        return last_result_slot();
      }
    }
    return {};
  }

  /// \brief Reconfigures the test UE with a modified Type 2 CG configuration, and confirms the new configuration.
  ///
  /// No RRC-configured grant is touched, as a Type 2 CG has none; the number of CG HARQ processes is enough to make
  /// the new CG configuration compare different from the installed one.
  void reconf_type2_cg(unsigned new_nof_harq_procs)
  {
    auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index = to_du_ue_index(0);
    ue_req.crnti    = ue_crnti;
    ASSERT_TRUE(ue_req.cfg.cells.has_value() and not ue_req.cfg.cells->empty());
    ASSERT_TRUE(ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config.has_value());
    ASSERT_TRUE(ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value());
    ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg->nof_harq_processes = new_nof_harq_procs;

    sched_ue_reconfiguration_message reconf;
    reconf.ue_index = to_du_ue_index(0);
    reconf.crnti    = ue_crnti;
    reconf.cfg      = ue_req.cfg;
    reconf.cs_rnti  = cs_rnti;
    sched->handle_ue_reconfiguration_request(reconf);

    // Confirm the UE applied the new config, so it exits the pending_reconf state and is offered for activation again.
    sched->handle_ue_config_applied(to_du_ue_index(0));
  }

  /// Sends the CG activation confirmation, which takes the UE out of the scheduler's pending-activation list and
  /// stops the activation from being retried.
  void confirm_cg_activation(du_ue_index_t ue_idx = to_du_ue_index(0), rnti_t crnti = ue_crnti) const
  {
    cg_confirmation_indication_message cg_conf{};
    cg_conf.cell_index = to_du_cell_index(0);
    cg_conf.ue_index   = ue_idx;
    cg_conf.rnti       = crnti;
    cg_conf.slot_rx    = last_result_slot();
    sched->handle_cg_confirmation_message(cg_conf);
  }

  /// \brief Reports UL pending bytes for the test UE.
  ///
  /// The scheduler only records a CRC expectation for a CG occasion when the UE actually has UL data to send, so
  /// missed-activation detection makes no progress without this. A single report is enough: CG PUSCH allocations
  /// deliberately leave the logical channel buffers untouched (see allocate_cg_opportunity()), so the pending bytes
  /// persist across occasions.
  void push_ul_data(rnti_t crnti = ue_crnti, du_ue_index_t ue_idx = to_du_ue_index(0))
  {
    push_bsr(ul_bsr_indication_message{
        to_du_cell_index(0), ue_idx, crnti, bsr_format::SHORT_BSR, {{uint_to_lcg_id(0), 200}}});
  }

  /// Removes the UE and runs slots until the scheduler confirms the removal, so that the UE index can be reused.
  void rem_ue_and_wait(du_ue_index_t ue_idx = to_du_ue_index(0))
  {
    schedule_task(launch_rem_ue_task(ue_idx));
    run_until_all_pending_tasks_completion();
  }

  /// Checks that the DCI carried by \c pdcch is a CG Type 2 activation, i.e. that it is a DCI format 0_1 scrambled
  /// with the CS-RNTI whose NDI, HARQ process number and RV are all zero, as per TS 38.213, Section 10.2,
  /// Table 10.2-1.
  static void assert_is_cg_activation_dci(const pdcch_ul_information& pdcch)
  {
    ASSERT_EQ(pdcch.dci.type(), dci_ul_rnti_config_type::cs_rnti_f0_1);
    const dci_0_1_cs_rnti_configuration& dci = pdcch.dci.as_cs_rnti_f0_1();
    ASSERT_FALSE(dci.new_data_indicator);
    ASSERT_EQ(dci.harq_process_number, 0);
    ASSERT_EQ(dci.redundancy_version, 0);
  }

  /// \brief Decodes the grant parameters carried by a CG Type 2 activation DCI.
  ///
  /// The frequency allocation is encoded as a type-1 RIV over the UE's active UL BWP, which this test setup leaves
  /// equal to the cell's initial UL BWP.
  cg_activation_grant_params decode_cg_activation_dci(const pdcch_ul_information& pdcch) const
  {
    const dci_0_1_cs_rnti_configuration& dci = pdcch.dci.as_cs_rnti_f0_1();

    cg_activation_grant_params params{};
    params.time_resource = dci.time_resource;
    params.mcs           = dci.modulation_coding_scheme;

    const unsigned bwp_nof_rbs = cell_cfg().params.ul_cfg_common.init_ul_bwp.generic_params.crbs.length();
    const ra_frequency_type1_configuration freq = ra_frequency_type1_from_riv(bwp_nof_rbs, dci.frequency_resource);
    params.vrbs                                 = vrb_interval{freq.start_vrb, freq.start_vrb + freq.length_vrb};
    return params;
  }

  /// \brief Runs the scheduler until the CG Type 2 activation DCI is allocated.
  /// \return The grant parameters the DCI carried, or nullopt if no activation DCI showed up within \c max_slots.
  std::optional<cg_activation_grant_params> run_until_cg_activation(unsigned max_slots)
  {
    if (not run_until_next_ul_pdcch(max_slots).valid()) {
      return std::nullopt;
    }
    const pdcch_ul_information* pdcch = find_ue_ul_pdcch(cs_rnti);
    if (pdcch == nullptr) {
      return std::nullopt;
    }
    return decode_cg_activation_dci(*pdcch);
  }
};

class configured_grant_type2_scheduler_test : public configured_grant_scheduler_test
{
protected:
  configured_grant_type2_scheduler_test() : configured_grant_scheduler_test(cg_test_params{.type2 = true}) {}
};

/// Safety margin, in slots, added on top of the CG period when waiting for a CG event to show up.
constexpr unsigned safety_margin = 20U;

/// Mirrors ue_pending_activation::max_nof_missed_crc_events: the number of CG PUSCH occasions whose expected CRC must
/// go missing before the scheduler gives up on an activation attempt and retries it.
constexpr unsigned nof_missed_crcs_for_release = 5U;
/// Mirrors configured_grant_type2_sched_impl::max_expected_crc_delay: slots the scheduler waits for the CRC of a CG
/// occasion before counting that occasion as missed.
constexpr unsigned max_expected_crc_delay = 50U;

/// \brief Upper bound, in slots, on how long the scheduler takes to detect a missed activation and emit the retry DCI.
///
/// The UE gets one CG occasion per period and each occasion is only counted as missed \c max_expected_crc_delay slots
/// after it elapsed, so the first miss lands one expiry delay in and the rest follow one per period. Rounded up
/// generously: this is only how long a test waits before giving up, so a loose bound costs nothing on the happy path.
static unsigned missed_activation_window(unsigned period_slots)
{
  return max_expected_crc_delay + (nof_missed_crcs_for_release + 2) * period_slots + 2 * safety_margin;
}

/// \brief Lower bound, in slots, on the same detection: the scheduler cannot possibly have seen
/// \c nof_missed_crcs_for_release distinct occasions go by in less than this.
///
/// Used to assert that a retry is driven by accumulated misses and not emitted immediately.
static unsigned min_slots_before_retry(unsigned period_slots)
{
  return (nof_missed_crcs_for_release - 1) * period_slots;
}

TEST_F(configured_grant_type2_scheduler_test, add_reconf_ue_does_not_schedule_cg_before_ue_is_ready)
{
  // Do not send the configuration-applied indication: Type 2 must not allocate a DCI or PUSCH until
  // start_ue_cg_activation() is invoked by that indication.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/false));

  for (unsigned i = 0; i != cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    EXPECT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "CG PUSCH was scheduled before Type 2 activation";
  }
}

TEST_F(configured_grant_type2_scheduler_test, cg_activation_is_retried_when_ue_does_not_confirm_activation)
{
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  // Missed-activation detection counts CG occasions that the UE had data for but produced no CRC, so the UE must have
  // UL data pending for the retry to ever trigger.
  push_ul_data();

  // First activation attempt: the scheduler allocates the DCI that activates the Type 2 CG.
  const slot_point first_pdcch_slot = run_until_next_ul_pdcch(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(first_pdcch_slot.valid()) << "No CG Type 2 activation DCI was allocated";
  ASSERT_NO_FATAL_FAILURE(assert_is_cg_activation_dci(*find_ue_ul_pdcch(cs_rnti)));

  // Neither a confirmation message nor any CRC is sent, so from the scheduler's perspective the UE never gave a sign
  // of having received the activation DCI. Once nof_missed_crcs_for_release occasions have gone by unanswered, the
  // scheduler must release the CG resources it reserved and retry the activation with a new DCI.
  const slot_point second_pdcch_slot = run_until_next_ul_pdcch(missed_activation_window(cg_params.period_slots));
  ASSERT_TRUE(second_pdcch_slot.valid())
      << "CG Type 2 activation was not retried after the UE failed to confirm the first activation DCI";
  ASSERT_NO_FATAL_FAILURE(assert_is_cg_activation_dci(*find_ue_ul_pdcch(cs_rnti)));

  // The retry must be driven by accumulated missed CRCs, not emitted right after the first attempt.
  EXPECT_GT(second_pdcch_slot, first_pdcch_slot + static_cast<int>(min_slots_before_retry(cg_params.period_slots)))
      << "CG Type 2 activation was retried before enough CG occasions went unanswered";
}

TEST_F(configured_grant_type2_scheduler_test, cg_activation_retry_is_not_released_again_immediately)
{
  // Regression test: release_ue_activation() used to reset the missed-CRC bookkeeping but not the miss counter, which
  // stayed at its threshold. The next attempt was then torn down on the very next slot, before the UE had any chance
  // to transmit, leaving the UE in an activate/release loop that churned CG resources and the periodic wheel every
  // slot. Each retry must get a full detection window of its own.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  push_ul_data();

  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "No CG Type 2 activation DCI was allocated";

  const slot_point second_pdcch_slot = run_until_next_ul_pdcch(missed_activation_window(cg_params.period_slots));
  ASSERT_TRUE(second_pdcch_slot.valid()) << "CG Type 2 activation was not retried a first time";

  const slot_point third_pdcch_slot = run_until_next_ul_pdcch(missed_activation_window(cg_params.period_slots));
  ASSERT_TRUE(third_pdcch_slot.valid()) << "CG Type 2 activation was not retried a second time";
  ASSERT_NO_FATAL_FAILURE(assert_is_cg_activation_dci(*find_ue_ul_pdcch(cs_rnti)));

  EXPECT_GT(third_pdcch_slot, second_pdcch_slot + static_cast<int>(min_slots_before_retry(cg_params.period_slots)))
      << "the retried activation was released again immediately: the missed-CRC counter is not reset per attempt";
}

TEST_F(configured_grant_type2_scheduler_test, ue_reconfiguration_without_cg_is_handled)
{
  // CG is DRB-gated, so a UE is created without it and goes through at least one reconfiguration that still carries no
  // CG config (e.g. the SRB2/DRB setup preceding CG). The CG scheduler must tolerate that: it used to assume every
  // reconfiguration it saw carried a CG config, which brought the gNB down on the first UE that attached.
  auto ue_req     = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
  ue_req.ue_index = to_du_ue_index(0);
  ue_req.crnti    = ue_crnti;
  ASSERT_TRUE(ue_req.cfg.cells.has_value() and not ue_req.cfg.cells->empty());
  ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();
  add_ue(ue_req, /*wait_notification=*/true);

  sched_ue_reconfiguration_message reconf;
  reconf.ue_index = to_du_ue_index(0);
  reconf.crnti    = ue_crnti;
  // Same config as at creation: still no CG.
  reconf.cfg = ue_req.cfg;
  sched->handle_ue_reconfiguration_request(reconf);
  sched->handle_ue_config_applied(to_du_ue_index(0));

  // No CG PUSCH may be scheduled either, since this UE has no CG configured.
  for (unsigned i = 0; i != cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr) << "CG PUSCH scheduled for a UE without CG";
  }
}

TEST_F(configured_grant_type2_scheduler_test, cg_resources_are_returned_to_the_pool_when_ue_is_removed)
{
  // Each activation takes one resource out of the cell-wide Type 2 resource pool, which holds one resource per usable
  // slot offset of the CG period (36 for this cell configuration). rem_ue() must give that resource back: if it does
  // not, every add/activate/remove cycle leaks one and activation silently stops working once the pool runs dry.
  // Looping more times than the pool is deep is what turns such a leak into an observable failure, so nof_cycles must
  // stay comfortably above the pool size.
  constexpr unsigned nof_cycles = 60;

  for (unsigned i = 0; i != nof_cycles; ++i) {
    ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true)) << "cycle=" << i;

    ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
        << "CG Type 2 activation stopped working at cycle=" << i
        << ": the resource pool was likely drained by resources that were not returned on UE removal";

    // Complete the lifecycle the way a real UE would, so the resource is held by an activated UE (rather than one
    // still pending activation) by the time it is removed.
    confirm_cg_activation();
    run_slot();

    rem_ue_and_wait();
  }
}

TEST_F(configured_grant_type2_scheduler_test, config_applied_after_confirmation_does_not_activate_twice)
{
  // A configuration-applied indication can arrive with no CG reconfiguration in between, e.g. for an RRC
  // reconfiguration that leaves the CG untouched. A UE is dropped from the pending-activation list once it confirms its
  // activation, so the CG scheduler must not mistake this for a UE that still needs one: a second activation would
  // leave the UE in the periodic wheel at two offsets, transmitting at twice its configured periodicity, and the
  // resource taken by the first activation would never return to the pool.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "No CG Type 2 activation DCI was allocated";
  confirm_cg_activation();
  run_slot();

  // Measure the steady-state CG PUSCH rate over a few CG periods.
  const unsigned window_slots         = 3 * cg_params.period_slots;
  unsigned       nof_cg_puschs_before = 0;
  for (unsigned i = 0; i != window_slots; ++i) {
    run_slot();
    if (find_ue_pusch(cs_rnti, *last_sched_result()) != nullptr) {
      ++nof_cg_puschs_before;
    }
  }
  ASSERT_GT(nof_cg_puschs_before, 0U) << "the activated UE was given no CG PUSCH occasion to begin with";

  sched->handle_ue_config_applied(to_du_ue_index(0));

  unsigned nof_cg_puschs_after = 0;
  unsigned nof_extra_dcis      = 0;
  for (unsigned i = 0; i != window_slots; ++i) {
    run_slot();
    if (find_ue_pusch(cs_rnti, *last_sched_result()) != nullptr) {
      ++nof_cg_puschs_after;
    }
    if (find_ue_ul_pdcch(cs_rnti) != nullptr) {
      ++nof_extra_dcis;
    }
  }

  EXPECT_EQ(nof_extra_dcis, 0U) << "a second activation DCI was sent for an already-activated UE";
  EXPECT_EQ(nof_cg_puschs_after, nof_cg_puschs_before)
      << "the CG PUSCH rate changed after a repeated configuration-applied indication: the UE was activated twice";
}

/// A CRC outcome for a CG PUSCH occasion, together with whether it should count as the UE having transmitted.
struct cg_crc_confirmation_params {
  std::string name;
  bool        crc_ok;
  /// The gNB reports a CG PUSCH as DTX when a CRC KO comes with a SINR below scheduler_ue_expert_config::
  /// cg_pusch_sinr_threshold_dB (-8 dB by default), or with a non-finite one; anything else, a CRC KO carrying no
  /// SINR included, means the UE did transmit.
  std::optional<float> sinr_dB;
};

/// Formatter for CRC confirmation params, used by gtest. Avoids the fallback raw-byte printer, which reads
/// uninitialized padding bytes.
void PrintTo(const cg_crc_confirmation_params& value, ::std::ostream* os)
{
  *os << value.name;
}

class configured_grant_type2_crc_confirmation_test : public configured_grant_scheduler_test,
                                                     public ::testing::WithParamInterface<cg_crc_confirmation_params>
{
protected:
  configured_grant_type2_crc_confirmation_test() : configured_grant_scheduler_test(cg_test_params{.type2 = true}) {}
};

TEST_P(configured_grant_type2_crc_confirmation_test, non_dtx_crc_confirms_activation_and_stops_retries)
{
  // A CRC indication that is not a DTX proves the UE transmitted on its CG occasion, which can only happen if it
  // received the activation DCI. That confirms the activation just as a confirmation message would, so the scheduler
  // must stop retrying. Note that a CRC KO confirms as much as a CRC OK: both mean the UE did transmit.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  // Without pending UL data the retry would never trigger and this test would pass vacuously.
  push_ul_data();

  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "No CG Type 2 activation DCI was allocated";

  const ul_sched_info* grant = run_until_next_cg_pusch();
  ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion followed the activation DCI";
  send_cg_crc(*grant, last_result_slot(), GetParam().crc_ok, GetParam().sinr_dB);

  // The UE is now confirmed, so no further activation DCI may be sent, even well past the point where a missed
  // activation would have been detected.
  const unsigned window = missed_activation_window(cg_params.period_slots);
  for (unsigned i = 0; i != window; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
        << "CG Type 2 activation was retried even though a non-DTX CRC confirmed it";
  }
}

INSTANTIATE_TEST_SUITE_P(cg_crc_outcome,
                         configured_grant_type2_crc_confirmation_test,
                         ::testing::Values(cg_crc_confirmation_params{"crc_ok", true, 10.0F},
                                           cg_crc_confirmation_params{"crc_ko_above_sinr_threshold", false, 10.0F},
                                           cg_crc_confirmation_params{"crc_ko_without_sinr", false, std::nullopt}),
                         [](const auto& p) { return p.param.name; });

TEST_F(configured_grant_type2_scheduler_test, dtx_crc_does_not_confirm_activation)
{
  // A CRC KO whose SINR is below cg_pusch_sinr_threshold_dB means the gNB detected nothing on the CG occasion, i.e.
  // the UE did not transmit. That says nothing about whether it received the activation DCI, so it must not confirm
  // the activation and the retry must still go ahead.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  push_ul_data();

  const slot_point first_pdcch_slot = run_until_next_ul_pdcch(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(first_pdcch_slot.valid()) << "No CG Type 2 activation DCI was allocated";

  const ul_sched_info* grant = run_until_next_cg_pusch();
  ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion followed the activation DCI";
  // Well below the -8 dB default threshold, so the PUSCH is reported as DTX.
  send_cg_crc(*grant, last_result_slot(), /*crc_ok=*/false, /*sinr_dB=*/-20.0F);

  const slot_point retry_pdcch_slot = run_until_next_ul_pdcch(missed_activation_window(cg_params.period_slots));
  EXPECT_TRUE(retry_pdcch_slot.valid())
      << "CG Type 2 activation was treated as confirmed by a DTX, so it was never retried";
}

TEST_F(configured_grant_type2_scheduler_test, ko_crc_with_infinite_sinr_does_not_confirm_activation)
{
  // An occasion the UE did not transmit on can yield an infinite SINR from a degenerate noise estimate, which sails
  // past cg_pusch_sinr_threshold_dB and would otherwise read as a transmission. A CRC KO reporting a perfect SINR is
  // self-contradictory, so it must not confirm the activation: doing so drops the UE from the pending-activation list
  // and permanently disables the missed-activation detection, leaving the CG never re-activated.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  push_ul_data();

  const slot_point first_pdcch_slot = run_until_next_ul_pdcch(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(first_pdcch_slot.valid()) << "No CG Type 2 activation DCI was allocated";

  const ul_sched_info* grant = run_until_next_cg_pusch();
  ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion followed the activation DCI";
  send_cg_crc(*grant,
              last_result_slot(),
              /*crc_ok=*/false,
              /*sinr_dB=*/std::numeric_limits<float>::infinity());

  const slot_point retry_pdcch_slot = run_until_next_ul_pdcch(missed_activation_window(cg_params.period_slots));
  EXPECT_TRUE(retry_pdcch_slot.valid())
      << "CG Type 2 activation was treated as confirmed by a CRC KO reporting an infinite SINR, so it was never "
         "retried";
}

TEST_F(configured_grant_type2_scheduler_test, discarded_cg_pusch_does_not_count_as_a_missed_activation)
{
  // A CG PUSCH that the lower layers discarded never reached the PHY, so no CRC will ever be reported for it. That
  // silence says nothing about whether the UE received the activation DCI, so the occasion must not count towards
  // missed-activation detection. If every CG occasion is discarded, the activation must never be retried.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  push_ul_data();

  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "No CG Type 2 activation DCI was allocated";

  unsigned       nof_discarded = 0;
  const unsigned window        = missed_activation_window(cg_params.period_slots);
  for (unsigned i = 0; i != window; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
        << "CG Type 2 activation was retried although every CG PUSCH was discarded by the lower layers";
    if (find_ue_pusch(cs_rnti, *last_sched_result()) == nullptr) {
      continue;
    }
    // Report the CG PUSCH of this slot as discarded. Only the PUSCH is dropped: a discarded activation PDCCH is a
    // separate condition that invalidates the attempt outright.
    scheduler_slot_handler::error_outcome err{};
    err.pusch_and_pucch_discarded = true;
    sched->handle_error_indication(last_result_slot(), to_du_cell_index(0), err);
    ++nof_discarded;
  }

  // Guard against a vacuous pass: the detector must have had enough occasions to release the UE, had they counted.
  EXPECT_GE(nof_discarded, nof_missed_crcs_for_release)
      << "too few CG occasions were discarded to exercise missed-activation detection";
}

/// Type 2 CG whose period is shorter than max_expected_crc_delay, so CG occasions elapse faster than the scheduler
/// retires them and the per-UE list of slots expecting a CRC is driven to its capacity.
class configured_grant_type2_short_period_test : public configured_grant_scheduler_test
{
protected:
  configured_grant_type2_short_period_test() :
    configured_grant_scheduler_test(
        cg_test_params{.periodicity = cg_configuration::periodicity_t::sl10, .period_slots = 10, .type2 = true})
  {
  }
};

TEST_F(configured_grant_type2_short_period_test, missed_activation_is_detected_when_crc_expectations_saturate)
{
  // More occasions are outstanding than the fixed-capacity list of slots expecting a CRC can hold, so the scheduler
  // stops recording new ones until an entry is retired. That must neither trip the container's bounds nor stall
  // detection: the activation still has to be retried.
  // The list saturates once its capacity-th occasion arrives before the first one is retired.
  static_assert((nof_missed_crcs_for_release - 1) * 10U <= max_expected_crc_delay,
                "CG period must be short enough for the CRC expectation list to reach capacity");

  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  push_ul_data();

  const slot_point first_pdcch_slot = run_until_next_ul_pdcch(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(first_pdcch_slot.valid()) << "No CG Type 2 activation DCI was allocated";

  const slot_point retry_pdcch_slot = run_until_next_ul_pdcch(missed_activation_window(cg_params.period_slots));
  ASSERT_TRUE(retry_pdcch_slot.valid())
      << "CG Type 2 activation was never retried: saturated CRC expectations stalled missed-activation detection";
  ASSERT_NO_FATAL_FAILURE(assert_is_cg_activation_dci(*find_ue_ul_pdcch(cs_rnti)));
}

TEST_F(configured_grant_type2_scheduler_test, activation_dci_f0_1_is_a_valid_cg_activation)
{
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "No CG Type 2 activation DCI was allocated";

  const pdcch_ul_information* pdcch = find_ue_ul_pdcch(cs_rnti);
  ASSERT_NE(pdcch, nullptr);
  ASSERT_EQ(pdcch->dci.type(), dci_ul_rnti_config_type::cs_rnti_f0_1);
  const dci_0_1_cs_rnti_configuration& dci = pdcch->dci.as_cs_rnti_f0_1();

  // Activation signature, as per TS 38.213, Section 10.2, Table 10.2-1.
  EXPECT_FALSE(dci.new_data_indicator);
  EXPECT_EQ(dci.harq_process_number, 0);
  EXPECT_EQ(dci.redundancy_version, 0);

  // The CG PUSCH carries UL-SCH. A value of 0 tells the UE not to transmit UL-SCH on the PUSCH and, together with an
  // all-zero CSI request and SRS request, forms a combination a UE "is not expected to receive" and may discard
  // outright, as per TS 38.212, Section 7.3.1.1.2.
  EXPECT_EQ(dci.ul_sch_indicator, 1U) << "UL-SCH indicator must be set, otherwise the UE transmits nothing";

  // The activation must actually result in periodic CG PUSCHs.
  ASSERT_NE(run_until_next_cg_pusch(), nullptr) << "No CG PUSCH followed the DCI 0_1 activation";
}

/// \brief Fixture covering the contract between the CG Type 2 activation DCI and the CG PUSCH occasions that follow.
///
/// The UE only ever sees the activation DCI, whereas the scheduler builds every CG occasion from the copy of the same
/// parameters it stashed in the UE CG state manager. Any divergence between the two means the UE transmits on
/// resources the gNB does not decode, without either side noticing.
class configured_grant_type2_dci_contract_test : public configured_grant_scheduler_test
{
protected:
  configured_grant_type2_dci_contract_test() :
    configured_grant_scheduler_test(cg_test_params{.type2 = true, .extra_pusch_td_alloc = true})
  {
  }

  /// \brief Adds the test UE, activates its CG and advances to the first CG PUSCH occasion that follows.
  /// \param[out] dci_params Grant parameters carried by the activation DCI.
  /// \param[out] grant First CG PUSCH occasion the scheduler built from them.
  void activate_ue_and_get_first_cg_pusch(cg_activation_grant_params& dci_params, const ul_sched_info*& grant)
  {
    ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

    const std::optional<cg_activation_grant_params> params =
        run_until_cg_activation(cg_params.period_slots + safety_margin);
    ASSERT_TRUE(params.has_value()) << "No CG Type 2 activation DCI was allocated";
    dci_params = params.value();

    grant = run_until_next_cg_pusch();
    ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion followed the activation DCI";
  }
};

TEST_F(configured_grant_type2_dci_contract_test, cg_pusch_rbs_match_activation_dci)
{
  cg_activation_grant_params dci_params{};
  const ul_sched_info*       grant = nullptr;
  ASSERT_NO_FATAL_FAILURE(activate_ue_and_get_first_cg_pusch(dci_params, grant));

  ASSERT_TRUE(grant->pusch_cfg.rbs.is_type1()) << "Expected a type-1 (contiguous) VRB allocation";
  EXPECT_EQ(grant->pusch_cfg.rbs.type1(), dci_params.vrbs)
      << "the CG PUSCH sits on different VRBs than the activation DCI told the UE to transmit on";
}

TEST_F(configured_grant_type2_dci_contract_test, cg_pusch_symbols_match_activation_dci)
{
  cg_activation_grant_params dci_params{};
  const ul_sched_info*       grant = nullptr;
  ASSERT_NO_FATAL_FAILURE(activate_ue_and_get_first_cg_pusch(dci_params, grant));

  // The DCI carries the TD allocation as an index into the cell PUSCH time domain resource list; the symbols the
  // scheduler grants must be the ones that entry describes.
  const auto& pusch_td_list = cell_cfg().params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
  ASSERT_LT(dci_params.time_resource, pusch_td_list.size())
      << "the activation DCI points outside the cell PUSCH time domain resource list";
  EXPECT_EQ(grant->pusch_cfg.symbols, pusch_td_list[dci_params.time_resource].symbols)
      << "the CG PUSCH uses different symbols than the TD allocation announced in the activation DCI";
}

TEST_F(configured_grant_type2_dci_contract_test, cg_pusch_mcs_and_tbs_match_activation_dci)
{
  cg_activation_grant_params dci_params{};
  const ul_sched_info*       grant = nullptr;
  ASSERT_NO_FATAL_FAILURE(activate_ue_and_get_first_cg_pusch(dci_params, grant));

  EXPECT_EQ(grant->pusch_cfg.mcs_index.value(), dci_params.mcs)
      << "the CG PUSCH uses a different MCS than the one announced in the activation DCI";
  // Unlike CG Type 1, a Type 2 grant takes its MCS from the cell-level CG configuration, not from the UE-dedicated one.
  EXPECT_EQ(dci_params.mcs, cg_params.mcs) << "the activation DCI does not carry the cell-configured CG MCS";

  // The TBS is computed once, at activation, from the VRBs and TD allocation the DCI announced, and then cached and
  // reused by every occasion; is_valid_ul_sched_info() recomputes it from the grant's own fields and checks the
  // resulting code rate, so a stale or wrongly computed TBS fails here.
  EXPECT_GT(grant->pusch_cfg.tb_size_bytes.value(), 0U);
  EXPECT_TRUE(test_helper::is_valid_ul_sched_info(*grant))
      << "CG PUSCH TBS/code-rate inconsistent with the grant parameters announced in the activation DCI";
}

TEST_F(configured_grant_type2_dci_contract_test, first_cg_pusch_lands_k2_slots_after_the_activation_dci)
{
  // The phase of the CG occasions is derived at activation as (activation slot + k2) % CG period, with k2 taken from
  // the TD allocation the DCI announced. As k2 is shorter than the CG period, the first occasion must land exactly k2
  // slots after the activation DCI: any other phase means the UE and the gNB disagree on when the grant recurs, and
  // the UE would transmit into slots the gNB does not even reserve.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  const slot_point pdcch_slot = run_until_next_ul_pdcch(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(pdcch_slot.valid()) << "No CG Type 2 activation DCI was allocated";
  const pdcch_ul_information* pdcch = find_ue_ul_pdcch(cs_rnti);
  ASSERT_NE(pdcch, nullptr);
  const cg_activation_grant_params dci_params = decode_cg_activation_dci(*pdcch);

  const auto& pusch_td_list = cell_cfg().params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
  ASSERT_LT(dci_params.time_resource, pusch_td_list.size());
  const unsigned k2 = pusch_td_list[dci_params.time_resource].k2;
  ASSERT_LT(k2, cg_params.period_slots) << "this test assumes a k2 shorter than the CG period";

  ASSERT_NE(run_until_next_cg_pusch(), nullptr) << "No CG PUSCH occasion followed the activation DCI";
  EXPECT_EQ(last_result_slot() - pdcch_slot, static_cast<int>(k2))
      << "the first CG PUSCH occasion does not sit k2 slots after the activation DCI";
}

/// \brief Fixture whose CG period is shorter than the k2 the activation DCI announces.
///
/// The occasion phase is (activation slot + k2) % period, so with a period at or below k2 that phase recurs at least
/// once between the activation PDCCH and the UE's first transmission. Those earlier occasions belong to the grant the
/// UE is only about to receive, so it does not transmit on them.
class configured_grant_type2_short_period_vs_k2_test : public configured_grant_scheduler_test
{
protected:
  configured_grant_type2_short_period_vs_k2_test() :
    configured_grant_scheduler_test(
        cg_test_params{.periodicity = cg_configuration::periodicity_t::sl4, .period_slots = 4, .type2 = true})
  {
  }
};

TEST_F(configured_grant_type2_short_period_vs_k2_test, no_cg_pusch_is_scheduled_before_the_first_transmission)
{
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  const slot_point pdcch_slot = run_until_next_ul_pdcch(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(pdcch_slot.valid()) << "No CG Type 2 activation DCI was allocated";
  const pdcch_ul_information* pdcch = find_ue_ul_pdcch(cs_rnti);
  ASSERT_NE(pdcch, nullptr);
  const cg_activation_grant_params dci_params = decode_cg_activation_dci(*pdcch);

  const auto& pusch_td_list = cell_cfg().params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
  ASSERT_LT(dci_params.time_resource, pusch_td_list.size());
  const unsigned k2 = pusch_td_list[dci_params.time_resource].k2;

  // Guard against a vacuous pass: with a k2 below the period there is no occasion between the activation and the first
  // transmission, and the gate under test would never be consulted.
  ASSERT_GE(k2, cg_params.period_slots)
      << "this test needs a k2 of at least the CG period, otherwise no occasion precedes the first transmission";

  // Every slot from the activation up to the first transmission must be free of CG PUSCHs. The activation slot itself
  // is checked first: with k2 equal to the period the occasion phase lands on it, and run_slot() has already produced
  // its result, so advancing before looking would step straight over the case under test.
  EXPECT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
      << "a CG PUSCH was scheduled in the very slot that carried the activation DCI";
  for (unsigned i = 1; i != k2; ++i) {
    run_slot();
    EXPECT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "a CG PUSCH was scheduled " << i << " slots after the activation DCI, before the UE's first transmission at "
        << "k2=" << k2;
  }

  // The first transmission itself must be there.
  run_slot();
  EXPECT_NE(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
      << "no CG PUSCH at the first transmission slot, k2=" << k2 << " slots after the activation DCI";
  EXPECT_EQ(last_result_slot() - pdcch_slot, static_cast<int>(k2));
}

TEST_F(configured_grant_type2_scheduler_test, cg_puschs_recur_at_the_configured_cg_period)
{
  // Once activated, the UE must get one CG PUSCH occasion per CG period and no more: a wheel filled at the wrong
  // stride would give the UE a different UL rate than the one its CG configuration grants it, which no existing test
  // would notice, as they only ever check that some CG PUSCH shows up.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "No CG Type 2 activation DCI was allocated";
  // Confirm the activation, so that the missed-activation detector cannot release the grant midway and shift the
  // occasion phase under the test.
  confirm_cg_activation();

  ASSERT_NE(run_until_next_cg_pusch(), nullptr) << "No CG PUSCH occasion followed the activation DCI";
  slot_point prev_slot = last_result_slot();

  constexpr unsigned nof_periods_checked = 4;
  for (unsigned i = 0; i != nof_periods_checked; ++i) {
    ASSERT_NE(run_until_next_cg_pusch(cg_params.period_slots + safety_margin), nullptr)
        << "CG PUSCH occasion " << i + 1 << " never arrived";
    EXPECT_EQ(last_result_slot() - prev_slot, static_cast<int>(cg_params.period_slots))
        << "CG PUSCH occasion " << i + 1 << " is not spaced from the previous one by the configured CG period";
    prev_slot = last_result_slot();
  }
}

/// Type 2 CG whose only UE-specific SearchSpace is monitored with DCI formats 0_0/1_0.
class configured_grant_type2_unsupported_dci_format_test : public configured_grant_scheduler_test
{
protected:
  configured_grant_type2_unsupported_dci_format_test() :
    configured_grant_scheduler_test(cg_test_params{.type2 = true, .ue_ss_dci_f0_0 = true})
  {
  }
};

TEST_F(configured_grant_type2_unsupported_dci_format_test, ue_with_no_dci_f0_1_search_space_is_never_activated)
{
  // TS 38.213, Table 10.2-1 allows the activation to be carried by either UL DCI format, but only format 0_1 is
  // built. A UE that monitors none of its UE-specific SearchSpaces with that format therefore cannot be reached, and
  // the scheduler must simply leave it unactivated: sending the activation in the format the UE does monitor would
  // make it decode a DCI of the wrong size, and granting the CG PUSCH occasions anyway would have it stay silent
  // while the gNB reserved and decoded resources for it every period.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  for (unsigned i = 0; i != cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
        << "a CG Type 2 activation DCI was sent to a UE that monitors no SearchSpace with DCI format 0_1";
    ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "CG PUSCH scheduled for a UE whose CG was never activated";
  }
}

/// \brief Fixture for CG Type 2 tests with more than one UE.
///
/// The CG band is widened so that several CG resources fit side by side at each slot offset, which is what lets two
/// UEs be activated on the same offset.
class configured_grant_type2_multi_ue_test : public configured_grant_scheduler_test
{
protected:
  static constexpr rnti_t ue2_crnti = to_rnti(0x4602);
  static constexpr rnti_t cs_rnti_2 = to_rnti(0xe0f0);

  /// Wide enough for four CG resources of one UE each, at the default grant size and MCS.
  static constexpr unsigned wide_cg_band_rbs = 40;

  configured_grant_type2_multi_ue_test() :
    configured_grant_scheduler_test(cg_test_params{.type2 = true, .max_nof_cell_cg_rbs = wide_cg_band_rbs})
  {
  }

  /// \brief Adds both test UEs so that they become ready for activation in the same slot.
  ///
  /// The UEs are created first, and only then is the CG installed on both, with no slot run in between. Adding them
  /// one at a time would let the first one be activated while the second is still being created, which would place
  /// them on two different slot offsets and defeat the purpose of these tests.
  void add_both_ues()
  {
    ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(to_du_ue_index(0), ue_crnti));
    ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(to_du_ue_index(1), ue2_crnti));
    ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, to_du_ue_index(0), ue_crnti, cs_rnti));
    ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, to_du_ue_index(1), ue2_crnti, cs_rnti_2));
  }

  /// \brief Advances until a slot in which both UEs have a CG PUSCH, and reports their VRBs.
  /// \return False if no such slot came up within \c max_slots.
  bool run_until_both_ues_have_cg_pusch(unsigned max_slots, vrb_interval& vrbs_ue1, vrb_interval& vrbs_ue2)
  {
    for (unsigned i = 0; i != max_slots; ++i) {
      run_slot();
      const ul_sched_info* pusch_ue1 = find_ue_pusch(cs_rnti, *last_sched_result());
      const ul_sched_info* pusch_ue2 = find_ue_pusch(cs_rnti_2, *last_sched_result());
      if (pusch_ue1 == nullptr or pusch_ue2 == nullptr) {
        continue;
      }
      if (not pusch_ue1->pusch_cfg.rbs.is_type1() or not pusch_ue2->pusch_cfg.rbs.is_type1()) {
        return false;
      }
      vrbs_ue1 = pusch_ue1->pusch_cfg.rbs.type1();
      vrbs_ue2 = pusch_ue2->pusch_cfg.rbs.type1();
      return true;
    }
    return false;
  }
};

TEST_F(configured_grant_type2_multi_ue_test, ues_activated_on_the_same_slot_offset_get_disjoint_vrbs)
{
  // Both UEs become ready in the same slot, so the activation procedure runs for both against the same slot offset and
  // has to hand each its own CG resource. Should they ever be given the same one, their CG PUSCHes would land on the
  // same RBs in the same slot, and neither would be decodable. cg_type2_resource_manager guarantees the resources of
  // one offset are disjoint, but nothing else checks that the scheduler asks it for one resource per UE.
  ASSERT_NO_FATAL_FAILURE(add_both_ues());

  vrb_interval vrbs_ue1;
  vrb_interval vrbs_ue2;
  ASSERT_TRUE(run_until_both_ues_have_cg_pusch(2 * (cg_params.period_slots + safety_margin), vrbs_ue1, vrbs_ue2))
      << "No slot was found in which both activated UEs have a CG PUSCH";

  EXPECT_FALSE(vrbs_ue1.overlaps(vrbs_ue2))
      << "the two UEs were given overlapping CG VRBs in the same slot: ue1=[" << vrbs_ue1.start() << ".."
      << vrbs_ue1.stop() << ") ue2=[" << vrbs_ue2.start() << ".." << vrbs_ue2.stop() << ")";
}

TEST_F(configured_grant_type2_multi_ue_test, removing_one_activated_ue_preserves_the_cg_grants_of_the_other)
{
  // rem_ue() unwinds the removed UE's wheel entries using the offset and period recorded when it was activated. A
  // sweep that is too wide would take the other UE's entries down with it and silently stop its CG.
  ASSERT_NO_FATAL_FAILURE(add_both_ues());

  vrb_interval vrbs_ue1;
  vrb_interval vrbs_ue2;
  ASSERT_TRUE(run_until_both_ues_have_cg_pusch(2 * (cg_params.period_slots + safety_margin), vrbs_ue1, vrbs_ue2))
      << "Pre-condition: both UEs must have CG PUSCH occasions before one of them is removed";

  rem_ue_and_wait(to_du_ue_index(0));

  unsigned nof_ue2_grants = 0;
  for (unsigned i = 0; i != 2 * cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "a CG PUSCH was still scheduled for the removed UE";
    if (find_ue_pusch(cs_rnti_2, *last_sched_result()) != nullptr) {
      ++nof_ue2_grants;
    }
  }
  EXPECT_GE(nof_ue2_grants, 2U) << "removing one UE took the other UE's CG occasions out of the slot wheel too";
}

/// CG Type 2 with the default CG band, which holds a single CG resource per slot offset.
class configured_grant_type2_single_resource_test : public configured_grant_scheduler_test
{
protected:
  static constexpr rnti_t ue2_crnti = to_rnti(0x4602);
  static constexpr rnti_t cs_rnti_2 = to_rnti(0xe0f0);

  configured_grant_type2_single_resource_test() : configured_grant_scheduler_test(cg_test_params{.type2 = true}) {}

  /// Adds both test UEs so that they become ready for activation in the same slot, and therefore contend for the
  /// single CG resource of the same slot offset.
  void add_both_ues()
  {
    ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(to_du_ue_index(0), ue_crnti));
    ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(to_du_ue_index(1), ue2_crnti));
    ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, to_du_ue_index(0), ue_crnti, cs_rnti));
    ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, to_du_ue_index(1), ue2_crnti, cs_rnti_2));
  }
};

TEST_F(configured_grant_type2_single_resource_test, second_ue_is_activated_on_a_different_slot_offset)
{
  // Only one CG resource exists per slot offset here, so the second UE cannot be activated in the same slot as the
  // first: activate_ue() finds no free resource at that offset and gives up for the slot. It must then succeed on a
  // later slot, which puts it on a different offset. Were the exhausted offset not honoured, both UEs would be handed
  // the same RBs in the same slot; were the retry not attempted, the second UE would never transmit at all.
  ASSERT_NO_FATAL_FAILURE(add_both_ues());

  std::optional<slot_point> ue1_pusch_slot;
  std::optional<slot_point> ue2_pusch_slot;
  for (unsigned i = 0; i != 2 * (cg_params.period_slots + safety_margin); ++i) {
    run_slot();
    if (not ue1_pusch_slot.has_value() and find_ue_pusch(cs_rnti, *last_sched_result()) != nullptr) {
      ue1_pusch_slot = last_result_slot();
    }
    if (not ue2_pusch_slot.has_value() and find_ue_pusch(cs_rnti_2, *last_sched_result()) != nullptr) {
      ue2_pusch_slot = last_result_slot();
    }
    // A slot carrying a CG PUSCH for both UEs would mean they share the single resource of that offset.
    ASSERT_FALSE(find_ue_pusch(cs_rnti, *last_sched_result()) != nullptr and
                 find_ue_pusch(cs_rnti_2, *last_sched_result()) != nullptr)
        << "both UEs were given a CG PUSCH in the same slot, although the offset holds a single CG resource";
  }

  ASSERT_TRUE(ue1_pusch_slot.has_value()) << "the first UE was never activated";
  ASSERT_TRUE(ue2_pusch_slot.has_value())
      << "the second UE was never activated: its activation was not retried after the first attempt found the slot "
         "offset exhausted";
}

/// CG Type 2 in a cell with periodic SRS, which reserves the last symbols of every UL slot. The extra PUSCH time
/// domain resource is what gives the activation procedure an entry that fits in the remaining symbols.
class configured_grant_type2_srs_test : public configured_grant_scheduler_test
{
protected:
  configured_grant_type2_srs_test() :
    // With SRS enabled the CG PUSCH gets 12 symbols instead of 14, so the same grant size needs 12 RBs rather than
    // the 10 the default CG band holds.
    configured_grant_scheduler_test(
        cg_test_params{.type2 = true, .max_nof_cell_cg_rbs = 12, .srs_enabled = true, .extra_pusch_td_alloc = true})
  {
  }
};

TEST_F(configured_grant_type2_srs_test, activation_picks_a_td_allocation_that_avoids_the_srs_symbols)
{
  // A CG Type 2 grant has no PDCCH of its own, so its symbols are fixed once at activation and used every period. If
  // the activation picked a TD allocation reaching into the SRS region, the CG PUSCH would collide with the SRS of
  // every UE in the cell on every single occasion.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  const ul_sched_info* grant = run_until_next_cg_pusch();
  ASSERT_NE(grant, nullptr) << "No CG PUSCH occasion was scheduled with SRS enabled";

  // SRS resources always sit at the end of the slot, as per TS 38.211, Section 6.4.1.4.1.
  const unsigned first_srs_symbol =
      NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - cell_cfg().params.init_bwp.srs_cfg.max_nof_symbols.value();
  EXPECT_LE(grant->pusch_cfg.symbols.stop(), first_srs_symbol)
      << "the CG PUSCH reaches into the symbols reserved for SRS";

  // Guard against a vacuous pass: the cell must carry a TD allocation that does collide with SRS, so that the
  // activation had to actually skip one.
  const auto& pusch_td_list = cell_cfg().params.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
  EXPECT_TRUE(std::any_of(pusch_td_list.begin(),
                          pusch_td_list.end(),
                          [first_srs_symbol](const pusch_time_domain_resource_allocation& td) {
                            return td.symbols.stop() > first_srs_symbol;
                          }))
      << "every TD allocation of this cell already avoids SRS, so nothing was skipped";
}

TEST_F(configured_grant_type2_scheduler_test, discarded_activation_pdcch_is_retried_without_waiting_for_missed_crcs)
{
  // When the lower layers discard the PDCCHs of a slot, the activation DCI never reaches the UE, so the UE will never
  // transmit and no CRC will ever arrive to reveal it. The missed-CRC detector cannot help either: it only arms while
  // the UE has UL data pending. The scheduler must therefore invalidate the attempt as soon as the error indication
  // arrives and retry it, rather than leaving the UE with a grant it knows nothing about.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  const slot_point first_pdcch_slot = run_until_next_ul_pdcch(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(first_pdcch_slot.valid()) << "No CG Type 2 activation DCI was allocated";

  // Report the PDCCHs of the activation slot as discarded. Only the PDCCH is dropped here: a discarded CG PUSCH is a
  // separate condition, which merely drops the CRC expectation of that occasion.
  scheduler_slot_handler::error_outcome err{};
  err.pdcch_discarded = true;
  sched->handle_error_indication(first_pdcch_slot, to_du_cell_index(0), err);

  const slot_point retry_pdcch_slot = run_until_next_ul_pdcch(cg_params.period_slots + safety_margin);
  ASSERT_TRUE(retry_pdcch_slot.valid()) << "the CG Type 2 activation was never retried after its PDCCH was discarded";
  ASSERT_NO_FATAL_FAILURE(assert_is_cg_activation_dci(*find_ue_ul_pdcch(cs_rnti)));

  // Note that no UL data was ever reported for this UE, so the missed-CRC detector never armed: the retry can only
  // have come from the error indication.
  EXPECT_LT(retry_pdcch_slot - first_pdcch_slot, static_cast<int>(min_slots_before_retry(cg_params.period_slots)))
      << "the retry did not follow the error indication promptly";
}

TEST_F(configured_grant_type2_scheduler_test, removing_a_ue_still_pending_activation_is_clean)
{
  // A UE can be released in between the configuration-applied indication, which queues it for activation, and the
  // slot its activation DCI would have been sent in. rem_ue() has to drop it from the pending-activation list: a
  // stale entry left there would have run_ues_activation() call activate_ue() with a UE index the repository no
  // longer holds, which dereferences a null UE on the very next slot. Such a UE holds no CG resource and sits at no
  // offset of the slot wheel either, so nothing may be returned to the pool on its behalf.
  constexpr unsigned nof_cycles = 10;

  for (unsigned i = 0; i != nof_cycles; ++i) {
    // No slot is run between the configuration-applied indication and the removal, so the reconfiguration, the
    // indication and the removal are all processed in the same slot and the UE never reaches activation.
    ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true)) << "cycle=" << i;
    rem_ue_and_wait();

    for (unsigned n = 0; n != safety_margin; ++n) {
      run_slot();
      ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
          << "an activation DCI was sent for a UE that was removed while pending activation, at cycle=" << i;
      ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
          << "a CG PUSCH was scheduled for a UE that was removed while pending activation, at cycle=" << i;
    }
  }

  // The CG resource pool and the slot wheel must have been left untouched by those removals: a UE added now still
  // gets activated and served.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "a UE could no longer be activated after other UEs had been removed while pending activation";
  confirm_cg_activation();
  EXPECT_NE(run_until_next_cg_pusch(), nullptr) << "the newly activated UE was given no CG PUSCH occasion";
}

TEST_F(configured_grant_type2_scheduler_test, reconfiguration_of_an_activated_ue_requires_a_new_activation)
{
  // A reconfiguration tears the CG down (add_reconf_ue() -> rem_ue()), since the UE has to be told the parameters of
  // its new grant with a fresh activation DCI. Two things must hold afterwards: the UE is activated again, and the
  // wheel entries the first activation inserted are gone. Were they left behind, the UE would sit in the wheel at two
  // offsets at once and transmit at twice the rate its CG configuration grants it, on resources that the resource
  // manager has meanwhile handed to somebody else.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "No CG Type 2 activation DCI was allocated";
  confirm_cg_activation();
  ASSERT_NE(run_until_next_cg_pusch(), nullptr) << "Pre-condition: the UE must have CG occasions before the reconfig";

  constexpr unsigned reconfigured_nof_harq_procs = 8;
  ASSERT_NO_FATAL_FAILURE(reconf_type2_cg(reconfigured_nof_harq_procs));

  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "the reconfigured UE was never activated again, so its CG stayed down";
  ASSERT_NO_FATAL_FAILURE(assert_is_cg_activation_dci(*find_ue_ul_pdcch(cs_rnti)));
  confirm_cg_activation();

  ASSERT_NE(run_until_next_cg_pusch(), nullptr) << "CG PUSCH occasions did not resume after the re-activation";
  slot_point prev_slot = last_result_slot();

  constexpr unsigned nof_periods_checked = 4;
  for (unsigned i = 0; i != nof_periods_checked; ++i) {
    ASSERT_NE(run_until_next_cg_pusch(cg_params.period_slots + safety_margin), nullptr)
        << "CG PUSCH occasion " << i + 1 << " never arrived after the re-activation";
    EXPECT_EQ(last_result_slot() - prev_slot, static_cast<int>(cg_params.period_slots))
        << "the reconfigured UE is getting CG occasions more often than its CG period: the entries of the previous "
           "activation were left in the slot wheel";
    prev_slot = last_result_slot();
  }
}

TEST_F(configured_grant_type2_scheduler_test, reconfiguration_that_leaves_the_cg_unchanged_does_not_disturb_it)
{
  // Most reconfigurations a UE goes through do not touch its CG: a DRB being added, a measurement configuration
  // changing, and so on. None of those is a reason to take the grant away. Tearing it down would cost the UE a full
  // re-activation round trip and a gap in its CG occasions every time, and would move it to a new slot offset and a
  // new set of RBs for no reason at all.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));
  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "No CG Type 2 activation DCI was allocated";
  confirm_cg_activation();

  const ul_sched_info* grant_before = run_until_next_cg_pusch();
  ASSERT_NE(grant_before, nullptr) << "Pre-condition: the UE must have CG occasions before the reconfiguration";
  ASSERT_TRUE(grant_before->pusch_cfg.rbs.is_type1());
  const vrb_interval vrbs_before = grant_before->pusch_cfg.rbs.type1();
  // The phase of the CG occasions within the CG period. It is fixed at activation, so it changes if and only if the
  // UE has been re-activated.
  const unsigned phase_before = last_result_slot().count() % cg_params.period_slots;

  // Reconfigure the UE with exactly the configuration it already has, CG included.
  ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true));

  unsigned nof_cg_puschs = 0;
  for (unsigned i = 0; i != 3 * cg_params.period_slots; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
        << "the UE was re-activated by a reconfiguration that left its CG configuration untouched";
    const ul_sched_info* grant = find_ue_pusch(cs_rnti, *last_sched_result());
    if (grant == nullptr) {
      continue;
    }
    ++nof_cg_puschs;
    ASSERT_TRUE(grant->pusch_cfg.rbs.is_type1());
    EXPECT_EQ(grant->pusch_cfg.rbs.type1(), vrbs_before) << "the CG grant moved to a different set of VRBs";
    EXPECT_EQ(last_result_slot().count() % cg_params.period_slots, phase_before)
        << "the CG occasions moved to a different slot offset";
  }
  EXPECT_GE(nof_cg_puschs, 3U) << "the CG occasions stopped after a reconfiguration that did not change the CG";
}

TEST_F(configured_grant_type2_scheduler_test, cell_deactivation_returns_cg_resources_to_the_pool)
{
  // On cell deactivation the UEs are dropped straight from the UE repository, without going through rem_ue(), so the
  // CG scheduler has to hand their resources back itself. If it does not, every deactivation cycle leaks the resource
  // of each activated UE and activation silently stops working once the pool runs dry. Cycling more times than the
  // pool is deep is what turns such a leak into an observable failure, so nof_cycles must stay comfortably above the
  // pool size (36 resources for this cell configuration).
  GTEST_SKIP() << "blocked by a division by zero in intra_slice_scheduler::fill_ue_candidate_group() on cell "
                  "re-activation";

  constexpr unsigned nof_cycles = 60;

  for (unsigned i = 0; i != nof_cycles; ++i) {
    ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true)) << "cycle=" << i;

    ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
        << "CG Type 2 activation stopped working at cycle=" << i
        << ": the resource pool was likely drained by resources that were not returned on cell deactivation";

    // Hold the resource as an activated UE rather than one still pending activation, which is the case the cell
    // deactivation path has to clean up.
    confirm_cg_activation();
    run_slot();

    // Bring the cell down and straight back up. No slot is run in between: a deactivated cell scheduler expects no
    // slot indication until it is re-enabled.
    sched->handle_cell_deactivation_request(to_du_cell_index(0));
    sched->handle_cell_activation_request(to_du_cell_index(0));
    run_slot();
  }
}

/// \brief CG Type 2 on an F1AP-created UE that is still awaiting its C-RNTI MAC CE.
///
/// While contention resolution is pending, the CG scheduler is not told about the UE at all: the reconfiguration that
/// installs the CG is skipped, and so is the configuration-applied indication that would queue it for activation. The
/// deferred setup is caught up when the C-RNTI CE arrives, which is the only path that reaches add_reconf_ue() with
/// no previous configuration.
class configured_grant_type2_fallback_ue_test : public configured_grant_scheduler_test
{
protected:
  static constexpr du_ue_index_t ue_idx = to_du_ue_index(0);

  configured_grant_type2_fallback_ue_test() : configured_grant_scheduler_test(cg_test_params{.type2 = true}) {}

  /// \brief Creates the UE in fallback via the F1AP path (no UL-CCCH slot, no CFRA), without CG configuration.
  ///
  /// \note scheduler_test_simulator::add_ue() is bypassed, as it auto-completes contention resolution by sending the
  /// C-RNTI CE right after the creation request, whereas this fixture must keep the UE awaiting that CE.
  void add_fallback_crnti_ce_ue()
  {
    auto ue_req               = sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    ue_req.ue_index           = ue_idx;
    ue_req.crnti              = ue_crnti;
    ue_req.starts_in_fallback = true;
    ASSERT_EQ(sched_config_helper::to_ue_creation_mode(ue_req), ue_creation_mode::high_layers);
    ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();

    sched->handle_ue_creation_request(ue_req);
    notif.last_ue_index_cfg.reset();
    for (unsigned i = 0; i != 100 and notif.last_ue_index_cfg != ue_idx; ++i) {
      run_slot();
    }
    ASSERT_EQ(notif.last_ue_index_cfg, ue_idx) << "UE creation was not completed";
  }
};

TEST_F(configured_grant_type2_fallback_ue_test, cg_activation_is_deferred_until_the_crnti_ce_is_received)
{
  add_fallback_crnti_ce_ue();

  // Install the CG configuration while contention resolution is still pending. Both the CG scheduler registration and
  // the configuration-applied indication are skipped for a UE in fallback, so nothing may happen yet.
  ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, ue_idx));

  for (unsigned i = 0; i != cg_params.period_slots + safety_margin; ++i) {
    run_slot();
    ASSERT_EQ(find_ue_ul_pdcch(cs_rnti), nullptr)
        << "a CG Type 2 activation DCI was sent while contention resolution was still pending";
    ASSERT_EQ(find_ue_pusch(cs_rnti, *last_sched_result()), nullptr)
        << "a CG PUSCH was scheduled while contention resolution was still pending";
  }

  // C-RNTI CE received: contention resolution completes and the deferred CG setup is caught up.
  sched->handle_crnti_ce_received(ue_idx);

  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "the UE was never activated after its C-RNTI CE was received, so its CG stayed down for good";
  ASSERT_NO_FATAL_FAILURE(assert_is_cg_activation_dci(*find_ue_ul_pdcch(cs_rnti)));
  confirm_cg_activation(ue_idx);

  EXPECT_NE(run_until_next_cg_pusch(), nullptr) << "No CG PUSCH followed the activation";

  // The UE must still be removable cleanly afterwards.
  rem_ue_and_wait(ue_idx);
}

TEST_F(configured_grant_type2_fallback_ue_test, cg_and_dynamic_grants_use_disjoint_harq_processes)
{
  // The first HARQ processes of a UE with CG are reserved for its CG occasions; a dynamic UL grant handed one of them
  // would overwrite a CG transmission still awaiting its CRC. This checks the split holds for a UE whose CG setup was
  // completed by the C-RNTI CE, which is the path that reaches add_reconf_ue() with no previous configuration.
  //
  // The reservation itself is established by ue_cell::handle_reconfiguration_request(), which runs for every
  // reconfiguration. CG can only ever be installed by one, since the CS-RNTI its PUSCH is built with is carried by
  // the reconfiguration message alone, so that is always the path taken.
  add_fallback_crnti_ce_ue();
  ASSERT_NO_FATAL_FAILURE(install_type2_cg(/*notify_config_applied=*/true, ue_idx));
  sched->handle_crnti_ce_received(ue_idx);

  ASSERT_TRUE(run_until_next_ul_pdcch(cg_params.period_slots + safety_margin).valid())
      << "the UE was never activated after its C-RNTI CE was received";
  confirm_cg_activation(ue_idx);

  // Give the UE UL data, so that it is also served by dynamic grants competing for the same HARQ entity.
  push_ul_data(ue_crnti, ue_idx);

  unsigned nof_cg_grants      = 0;
  unsigned nof_dynamic_grants = 0;
  for (unsigned i = 0; i != 3 * cg_params.period_slots; ++i) {
    run_slot();
    if (const ul_sched_info* cg_grant = find_ue_pusch(cs_rnti, *last_sched_result()); cg_grant != nullptr) {
      ++nof_cg_grants;
      EXPECT_LT(cg_grant->pusch_cfg.harq_id, to_harq_id(cg_params.nof_harq_procs))
          << "a CG PUSCH used a HARQ process outside the range reserved for CG";
    }
    if (const ul_sched_info* dyn_grant = find_ue_pusch(ue_crnti, *last_sched_result()); dyn_grant != nullptr) {
      ++nof_dynamic_grants;
      EXPECT_GE(dyn_grant->pusch_cfg.harq_id, to_harq_id(cg_params.nof_harq_procs))
          << "a dynamic PUSCH was given a HARQ process reserved for CG, so the CG HARQ processes were never reserved";
    }
  }

  // Guard against a vacuous pass: both kinds of grant must have been seen.
  EXPECT_GT(nof_cg_grants, 0U) << "the UE was given no CG PUSCH occasion";
  EXPECT_GT(nof_dynamic_grants, 0U) << "the UE was given no dynamic UL grant, so no HARQ contention was exercised";
}

class configured_grant_type2_scheduler_duplex_test : public configured_grant_scheduler_test,
                                                     public ::testing::WithParamInterface<cg_duplex_test_params>
{
protected:
  configured_grant_type2_scheduler_duplex_test() :
    configured_grant_scheduler_test(cg_test_params{.type2 = true}, make_cell_builder_params(GetParam()))
  {
  }
};

TEST_P(configured_grant_type2_scheduler_duplex_test, cg_grants_are_allocated_after_ue_is_ready_for_activation)
{
  // The configuration-applied indication causes start_ue_cg_activation() to enqueue the UE for Type 2
  // activation.
  ASSERT_NO_FATAL_FAILURE(add_type2_cg_ue(/*notify_config_applied=*/true));

  ASSERT_NE(run_until_next_cg_pusch(), nullptr)
      << "No CG PUSCH was scheduled after the UE became ready for Type 2 activation";
}

INSTANTIATE_TEST_SUITE_P(
    cg_duplex,
    configured_grant_type2_scheduler_duplex_test,
    ::testing::Values(
        cg_duplex_test_params{"FDD", std::nullopt},
        cg_duplex_test_params{"TDD_6D1S3U", tdd_ul_dl_config_common{subcarrier_spacing::kHz30, {10, 6, 10, 3, 0}}},
        cg_duplex_test_params{"TDD_3D1S6U", tdd_ul_dl_config_common{subcarrier_spacing::kHz30, {10, 3, 10, 6, 0}}}),
    [](const auto& p) { return p.param.name; });

/// \brief Fixture in which more UEs become ready for activation in one slot than the per-slot cap allows.
///
/// run_ues_activation() rations the activation DCIs to MAX_CG_ACTIVATION_PDCCHS_PER_SLOT, so that a burst of UEs does
/// not take the PDCCH space the dynamic grants need. CG_CONFIRMATIONS_POOL_SIZE is sized on that cap holding.
class configured_grant_type2_activation_cap_test : public configured_grant_scheduler_test
{
protected:
  /// Enough UEs that the cap has to defer at least one of them.
  static constexpr unsigned nof_ues = MAX_CG_ACTIVATION_PDCCHS_PER_SLOT + 2;
  /// Wide enough for one CG resource per UE, at the default grant size and MCS, so that no UE is held back for want
  /// of a resource rather than by the cap.
  static constexpr unsigned wide_cg_band_rbs = 10 * nof_ues;

  configured_grant_type2_activation_cap_test() :
    configured_grant_scheduler_test(cg_test_params{.type2 = true, .max_nof_cell_cg_rbs = wide_cg_band_rbs})
  {
  }

  static rnti_t crnti_of(unsigned i) { return to_rnti(static_cast<uint16_t>(fmt::underlying(ue_crnti) + i)); }
  static rnti_t cs_rnti_of(unsigned i) { return to_rnti(static_cast<uint16_t>(fmt::underlying(cs_rnti) + i)); }

  /// \brief Adds every UE so that they all become ready for activation in the same slot.
  ///
  /// All the UEs are created first and the CG installed on them only afterwards, with no slot run in between: adding
  /// them one at a time would let the earlier ones be activated while the later ones are still being created, and no
  /// slot would ever see more activations pending than the cap allows.
  void add_all_ues()
  {
    for (unsigned i = 0; i != nof_ues; ++i) {
      ASSERT_NO_FATAL_FAILURE(create_ue_without_cg(to_du_ue_index(i), crnti_of(i)));
    }
    for (unsigned i = 0; i != nof_ues; ++i) {
      ASSERT_NO_FATAL_FAILURE(
          install_type2_cg(/*notify_config_applied=*/true, to_du_ue_index(i), crnti_of(i), cs_rnti_of(i)));
    }
  }

  /// Counts the CG Type 2 activation DCIs in the last scheduler result, which are the only format 0_1 DCIs addressed
  /// to a CS-RNTI.
  unsigned count_activation_dcis() const
  {
    const auto& ul_pdcchs = last_sched_result()->dl.ul_pdcchs;
    return std::count_if(ul_pdcchs.begin(), ul_pdcchs.end(), [](const pdcch_ul_information& pdcch) {
      return pdcch.dci.type() == dci_ul_rnti_config_type::cs_rnti_f0_1;
    });
  }
};

TEST_F(configured_grant_type2_activation_cap_test, no_slot_carries_more_activation_dcis_than_the_cap)
{
  ASSERT_NO_FATAL_FAILURE(add_all_ues());

  unsigned         max_activations_in_a_slot = 0;
  std::set<rnti_t> activated_ues;
  for (unsigned i = 0; i != 2 * cg_params.period_slots; ++i) {
    run_slot();

    const unsigned nof_activations = count_activation_dcis();
    ASSERT_LE(nof_activations, MAX_CG_ACTIVATION_PDCCHS_PER_SLOT)
        << "more CG activation DCIs were sent in one slot than the cap the CG confirmation pool is sized on";
    max_activations_in_a_slot = std::max(max_activations_in_a_slot, nof_activations);

    for (unsigned u = 0; u != nof_ues; ++u) {
      if (find_ue_ul_pdcch(cs_rnti_of(u)) != nullptr) {
        activated_ues.emplace(cs_rnti_of(u));
      }
    }
  }

  // Guard against a vacuous pass: unless some slot actually reached the cap, the assertion above never bit.
  ASSERT_EQ(max_activations_in_a_slot, MAX_CG_ACTIVATION_PDCCHS_PER_SLOT)
      << "no slot ever reached the activation cap, so the limit was never the binding constraint";

  // The cap must defer activations, not starve them.
  ASSERT_EQ(activated_ues.size(), nof_ues) << "some UEs were never activated after the cap deferred them";
}

} // namespace
