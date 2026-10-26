// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/ran/resource_allocation/rb_interval.h"
#include "ocudu/ran/slot_point.h"
#include "ocudu/support/units.h"
#include <optional>

namespace ocudu {

/// \brief Parameters of an active CG allocation.
///
/// The parameters are only meaningful as a set: either the UE has an active CG and all of them are valid, or it has
/// none and none of them are. \ref ue_conf_grant_state_manager hands them out wrapped in a \c std::optional, so that
/// the activation check cannot be skipped.
struct cg_grant_params {
  unsigned     periodicity_slots;
  unsigned     time_offset;
  units::bytes tbs;
  vrb_interval vrbs;
  uint8_t      mcs;
  uint8_t      time_domain_allocation;
  /// Resource id is only applicable to CG type 2.
  unsigned resource_id;
};

/// /brief Class that keeps track of CG state variables.
///
/// Motivated by CG type 2, in which TBS, MCS, VRBs and slot offset and TD allocation can potentially change during the
/// scheduling life. The class provides an API that can be used for both CG type 1 and type 2.
class ue_conf_grant_state_manager
{
public:
  explicit ue_conf_grant_state_manager() = default;

  /// Test whether this is a slot with CG opportunity.
  bool is_cg_slot(slot_point slot) const
  {
    if (not state.has_value()) {
      return false;
    }
    if (is_before_first_tx(slot)) {
      return false;
    }

    return slot.count() % state->periodicity_slots == state->time_offset;
  }

  /// \brief Test whether \c slot precedes the first occasion the UE will transmit on.
  ///
  /// With CG Type 2 the occasions recur every period from a phase derived from the activation DCI, so the ones that
  /// fall between the activation and the first transmission at \c slot_pdcch + k2 belong to that phase but predate the
  /// grant. The UE does not transmit on them, and the slot wheel - a ring indexed by slot modulo its length - cannot
  /// express the distinction, as anything omitted from it is omitted on every period thereafter.
  /// \remark The gate is only used between the activation and the first occasion, i.e. for at most k2 slots. Keeping
  /// it enabled indefinitely would be unsound: \c slot_point comparisons are only meaningful within half a hyperframe,
  /// so a stale first-transmission slot would eventually read as being in the future again and suppress real
  /// occasions. \ref clear_first_tx_slot() disables it as soon as the first occasion is reached.
  bool is_before_first_tx(slot_point slot) const { return first_tx_slot.has_value() and slot < first_tx_slot.value(); }

  /// \brief Replaces the TDRA row of the active CG allocation, leaving the rest of its state untouched.
  /// \remark The new row must have the same symbols as the current one, as the grid reservations follow them.
  void set_time_domain_allocation(uint8_t td_alloc)
  {
    if (state.has_value()) {
      state->time_domain_allocation = td_alloc;
    }
  }

  /// Disarms the first-transmission gate, once an occasion at or after it has been reached.
  void clear_first_tx_slot() { first_tx_slot.reset(); }

  /// Update CG state variables.
  /// \param first_tx The first slot the UE will transmit on, which gates the occasions preceding it. Left unset for
  /// CG Type 1, whose occasions are all valid from the moment the configuration is applied.
  void update_state(unsigned                  period_slots,
                    units::bytes              tbs,
                    vrb_interval              vrbs,
                    uint8_t                   mcs,
                    uint8_t                   td_alloc,
                    unsigned                  t_offset,
                    unsigned                  res_id,
                    std::optional<slot_point> first_tx = std::nullopt)
  {
    state         = cg_grant_params{period_slots, t_offset, tbs, vrbs, mcs, td_alloc, res_id};
    first_tx_slot = first_tx;
  }

  void reset_state()
  {
    state.reset();
    first_tx_slot.reset();
  }

  /// Parameters of the active CG allocation, or \c std::nullopt if the UE has no active CG.
  const std::optional<cg_grant_params>& get_grant_params() const { return state; }

private:
  // Values that are subject to change during scheduling life.
  std::optional<cg_grant_params> state;
  // First slot the UE transmits on. Set only while the gate is armed; see is_before_first_tx().
  std::optional<slot_point> first_tx_slot;
};

} // namespace ocudu
