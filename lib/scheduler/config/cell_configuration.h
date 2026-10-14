// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "bwp_config_res_pool.h"
#include "ocudu/adt/slotted_vector.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include "ocudu/scheduler/config/ran_cell_config.h"
#include "ocudu/scheduler/config/scheduler_expert_config.h"
#include "ocudu/scheduler/scheduler_configurator.h"
#include <variant>

namespace ocudu {

/// Holds the configuration of a cell.
/// Additionally, this class pre-caches the computation of some const values related to the cell configuration
/// and provide parameter getter helpers.
class cell_configuration
{
public:
  explicit cell_configuration(const scheduler_expert_config&                  expert_cfg,
                              const sched_cell_configuration_request_message& msg,
                              const sched_bwp_config&                         init_bwp);
  cell_configuration(const cell_configuration&) = delete;
  cell_configuration(cell_configuration&&)      = delete;

  /// Expert parameters used to generate the cell configuration.
  const scheduler_expert_config& expert_cfg;

  /// Meta parameters used to generate the cell configuration.
  const ran_cell_config params;

  const du_cell_index_t       cell_index;
  const du_cell_group_index_t cell_group_index;
  /// List of RRM Policy members configured for this cell.
  std::vector<slice_rrm_policy_config> rrm_policy_members;
  /// Number of UE contexts that the cell is expected to hold, including the UEs to be RRC Rejected.
  const unsigned max_nof_ue_contexts;

  /// @name Derived parameters.
  /// Parameters derived from \ref ran_cell_config.
  ///@{

  const unsigned nof_dl_prbs;
  const unsigned nof_ul_prbs;
  const unsigned nof_slots_per_frame;
  /// List of zp-CSI-RS resources.
  const std::vector<zp_csi_rs_resource> zp_csi_rs_list;
  /// List of nzp-CSI-RS resources.
  const std::vector<nzp_csi_rs_resource> nzp_csi_rs_list;
  /// Size of the CSI request field in DCI Format 0_1. Zero if the cell configures no aperiodic CSI reports.
  const unsigned csi_report_trigger_size;
  /// Initial BWP configuration for this cell.
  const sched_bwp_config& init_bwp;
  /// List of BWP config resources handled by this cell.
  slotted_id_vector<bwp_id_t, bwp_config_res_pool> bwp_res;

  ///@}

  /// Cell-specific K-offset in slots at the cell SCS (K_cell,offset, TS 38.213 Section 4.2), broadcast in SIB19 and
  /// applied by the UE itself. Never add it to a delay without k_mac; use the conversions below.
  unsigned ntn_cs_koffset;

  /// k_mac in slots at the cell SCS, non-zero when the Common TA does not cover the whole feeder link.
  /// A clock skew, not a scheduling offset: the UE never adds it to a transmission time.
  unsigned ntn_k_mac_slots;

  /// \brief Uplink timing advance T_TA of a UE at the cell reference location, see
  /// \c ntn_cell_params::ref_location_ul_ta. Absent in a terrestrial cell, and in an NTN cell that configures no
  /// reference location or whose first value has not been computed yet.
  ///
  /// Used to place the uplink measurement gap window, see \ref is_inside_ul_meas_gap. Refreshed as the satellite moves.
  std::optional<std::chrono::microseconds> ntn_ref_location_ul_ta;

  subcarrier_spacing scs_common() const { return params.dl_cfg_common.init_dl_bwp.generic_params.scs; }

  /// \name NTN clock conversions
  ///
  /// DL and UL frame timing are misaligned at the gNB by k_mac (TS 38.300 Section 16.14.2.1): a UE uplink
  /// transmission at its UL slot p reaches the gNB antenna at DL clock slot p + k_mac. The scheduler runs on the gNB
  /// DL clock, so anything expressed in the UE uplink frame numbering crosses through one of these. All of them are
  /// the identity in a terrestrial cell.
  ///
  /// Which one to use follows from who derives the time:
  /// - the UE derives it on its own - PRACH occasions, the periodic SR, CSI and SRS occasions, the Type 1 configured
  ///   grant wheel, the RA-RNTI t_id: \c dl_to_ue_ul_slot, \c ue_ul_to_dl_slot and \c ue_ul_to_dl_slot_offset;
  /// - the gNB schedules it, so TS 38.213 states the relation as k1 or k2 plus \c ntn_cs_koffset: \c ul_delay,
  ///   which the caller indexes the grid with. k_mac is not part of the relation, only of the conversion;
  /// - it is neither, but a depth that has to outlive an outstanding uplink allocation - a ring size, a staleness
  ///   window: \c ntn_ul_alloc_delay_slots.
  ///
  /// MAC timers defined against the UE-gNB RTT need none of these. One anchored on an uplink reception does: see
  /// \c conres_win_start for the ra-ContentionResolutionTimer.
  /// @{

  /// Converts a slot on the gNB DL clock into the UE UL slot whose transmission arrives there.
  slot_point dl_to_ue_ul_slot(slot_point dl_sl) const { return dl_sl - ntn_k_mac_slots; }

  /// Converts a UE UL slot into the slot on the gNB DL clock where that transmission arrives.
  slot_point ue_ul_to_dl_slot(slot_point ue_ul_sl) const { return ue_ul_sl + ntn_k_mac_slots; }

  /// As \c ue_ul_to_dl_slot, for a delay rather than a slot.
  unsigned ul_delay(unsigned ue_ul_delay) const { return ue_ul_delay + ntn_k_mac_slots; }

  /// As \c ue_ul_to_dl_slot, for the periodic slot wheels keyed by offset within \c period.
  unsigned ue_ul_to_dl_slot_offset(unsigned ue_ul_offset, unsigned period) const
  {
    return ul_delay(ue_ul_offset) % period;
  }

  /// The whole UE-gNB round trip in slots: the K-offset the UE applies itself, converted to the gNB DL clock.
  unsigned ntn_ul_alloc_delay_slots() const { return ntn_cs_koffset + ntn_k_mac_slots; }
  /// @}

  /// Checks if the cell is configured in TDD mode.
  bool is_tdd() const { return params.tdd_cfg.has_value(); }

  /// Checks if DL is active for all symbols in the given slot.
  bool is_fully_dl_enabled(slot_point sl) const
  {
    if (dl_symbols_per_slot_lst.empty()) {
      // Note: dl_enabled_slot_lst is empty in the FDD case.
      return true;
    }
    if (sl.numerology() != to_numerology_value(params.tdd_cfg->ref_scs)) {
      // Convert slot into equivalent reference SCS.
      sl = set_slot_numerology(sl, to_numerology_value(params.tdd_cfg->ref_scs));
    }
    return dl_symbols_per_slot_lst[sl.count() % dl_symbols_per_slot_lst.size()] ==
           get_nsymb_per_slot(params.dl_cfg_common.init_dl_bwp.generic_params.cp);
  }

  /// Checks if UL is active for all symbols in the given slot.
  bool is_fully_ul_enabled(slot_point sl) const
  {
    if (ul_symbols_per_slot_lst.empty()) {
      // Note: ul_enabled_slot_lst is empty in the FDD case.
      return true;
    }
    if (sl.numerology() != to_numerology_value(params.tdd_cfg->ref_scs)) {
      // Convert slot into equivalent reference SCS.
      sl = set_slot_numerology(sl, to_numerology_value(params.tdd_cfg->ref_scs));
    }
    return ul_symbols_per_slot_lst[sl.count() % ul_symbols_per_slot_lst.size()] ==
           get_nsymb_per_slot(params.ul_cfg_common.init_ul_bwp.generic_params.cp);
  }

  /// Checks if DL is active for at least one symbol in the given slot.
  bool is_dl_enabled(slot_point sl) const
  {
    if (dl_symbols_per_slot_lst.empty()) {
      // Note: dl_enabled_slot_lst is empty in the FDD case.
      return true;
    }
    if (sl.numerology() != to_numerology_value(params.tdd_cfg->ref_scs)) {
      // Convert slot into equivalent reference SCS.
      sl = set_slot_numerology(sl, to_numerology_value(params.tdd_cfg->ref_scs));
    }
    return dl_symbols_per_slot_lst[sl.count() % dl_symbols_per_slot_lst.size()] > 0;
  }

  /// Checks if UL is active for at least one symbol in the given slot.
  bool is_ul_enabled(slot_point sl) const
  {
    if (ul_symbols_per_slot_lst.empty()) {
      // Note: ul_enabled_slot_lst is empty in the FDD case.
      return true;
    }
    if (sl.numerology() != to_numerology_value(params.tdd_cfg->ref_scs)) {
      // Convert slot into equivalent reference SCS.
      sl = set_slot_numerology(sl, to_numerology_value(params.tdd_cfg->ref_scs));
    }
    return ul_symbols_per_slot_lst[sl.count() % ul_symbols_per_slot_lst.size()] > 0;
  }

  bool cfra_enabled() const { return params.init_bwp.rach.cfra_enabled; }

  bool paired_spectrum() const { return band_helper::is_paired_spectrum(params.dl_carrier.band); }

  nr_band band() const { return params.dl_carrier.band; }

  /// Returns the number of active DL symbols in the given slot.
  unsigned get_nof_dl_symbol_per_slot(slot_point sl) const
  {
    if (dl_symbols_per_slot_lst.empty()) {
      // Note: dl_enabled_slot_lst is empty in the FDD case.
      return get_nsymb_per_slot(params.dl_cfg_common.init_dl_bwp.generic_params.cp);
    }
    if (sl.numerology() != to_numerology_value(params.tdd_cfg->ref_scs)) {
      // Convert slot into equivalent reference SCS.
      sl = set_slot_numerology(sl, to_numerology_value(params.tdd_cfg->ref_scs));
    }
    return dl_symbols_per_slot_lst[sl.count() % dl_symbols_per_slot_lst.size()];
  }

  /// Returns the number of active UL symbols in the given slot.
  unsigned get_nof_ul_symbol_per_slot(slot_point sl) const
  {
    if (ul_symbols_per_slot_lst.empty()) {
      // Note: ul_enabled_slot_lst is empty in the FDD case.
      return get_nsymb_per_slot(params.ul_cfg_common.init_ul_bwp.generic_params.cp);
    }
    if (sl.numerology() != to_numerology_value(params.tdd_cfg->ref_scs)) {
      // Convert slot into equivalent reference SCS.
      sl = set_slot_numerology(sl, to_numerology_value(params.tdd_cfg->ref_scs));
    }
    return ul_symbols_per_slot_lst[sl.count() % ul_symbols_per_slot_lst.size()];
  }

  /// Returns the coreset configuration for the given coreset ID.
  const coreset_configuration& get_common_coreset(coreset_id cs_id) const
  {
    // The existence of the Coreset (either CommonCoreset or Coreset0) has been verified by the validator.
    return params.dl_cfg_common.init_dl_bwp.pdcch_common.common_coreset.has_value() and
                   params.dl_cfg_common.init_dl_bwp.pdcch_common.common_coreset.value().get_id() == cs_id
               ? params.dl_cfg_common.init_dl_bwp.pdcch_common.common_coreset.value()
               : params.dl_cfg_common.init_dl_bwp.pdcch_common.coreset0.value();
  }

  /// Checks if the cell is configured with PUCCH Format 0 and Format 2 resources.
  bool is_pucch_f0_and_f2() const
  {
    return std::holds_alternative<pucch_f0_params>(params.init_bwp.pucch.resources.f0_or_f1_params) and
           std::holds_alternative<pucch_f2_params>(params.init_bwp.pucch.resources.f2_or_f3_or_f4_params);
  }

  /// \brief Determines the use of transform precoding according to the parameter \e msg3-transformPrecoder.
  ///
  /// The UE determines the use of the transform precoder in TS 38.214 Section 6.1.3. It uses this parameter when a
  /// PUSCH is scheduled by:
  /// - a RAR UL grant;
  /// - DCI Format 0_0; or
  /// - DCI Format 0_1 and the parameter \e transformPrecoder in \e pusch-Config is not present.
  bool use_msg3_transform_precoder() const
  {
    if (not params.ul_cfg_common.init_ul_bwp.rach_cfg_common) {
      return false;
    }
    return params.ul_cfg_common.init_ul_bwp.rach_cfg_common->msg3_transform_precoder;
  }

private:
  /// Vector circularly indexed by slot with the list of nof active DL/UL symbols per slot.
  std::vector<unsigned> dl_symbols_per_slot_lst;
  std::vector<unsigned> ul_symbols_per_slot_lst;
};

/// \brief List of common configurations for the cells currently added in the scheduler.
///
/// Note: Pointers to cell configurations should remain valid on cell removal/addition.
using cell_common_configuration_list = slotted_id_vector<du_cell_index_t, cell_configuration*>;

} // namespace ocudu
