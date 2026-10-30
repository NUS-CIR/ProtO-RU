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
/// \remark The parameters are only meaningful as a set: either the UE has an active CG and all of them are valid, or it
/// has none and none of them are.
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

/// Class that keeps track of CG state variables.
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

  /// Test whether \c slot precedes the first occasion the UE will transmit on.
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
  /// CG Type 1.
  void update_state(unsigned                  period_slots,
                    units::bytes              tbs,
                    vrb_interval              vrbs,
                    uint8_t                   mcs,
                    uint8_t                   td_alloc,
                    unsigned                  t_offset,
                    unsigned                  res_id,
                    std::optional<slot_point> first_tx = std::nullopt)
  {
    state = cg_grant_params{period_slots, t_offset, tbs, vrbs, mcs, td_alloc, res_id};
    // Only used for CG type 2, as in type 1 all occasions are valid from the moment the configuration is applied.
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
