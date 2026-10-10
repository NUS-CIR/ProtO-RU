// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/scheduler/config/logical_channel_config.h"
#include "ocudu/scheduler/result/pdsch_info.h"
#include "ocudu/scheduler/result/pusch_info.h"
#include "ocudu/scheduler/result/sched_result_grid.h"
#include "ocudu/support/units.h"

namespace ocudu {

class slice_ue;
class ue_cell;

/// Type used to represent a UE priority.
using ue_sched_priority = double;

/// Priority value used to represent that the UE must not be scheduled.
const ue_sched_priority forbid_sched_priority = std::numeric_limits<ue_sched_priority>::lowest();

/// Lowest priority possible
const ue_sched_priority lowest_sched_priority = forbid_sched_priority + std::numeric_limits<ue_sched_priority>::min();

/// Maximum priority possible
constexpr ue_sched_priority max_sched_priority = std::numeric_limits<ue_sched_priority>::max();

/// UE candidate for DL or UL scheduling.
struct ue_newtx_candidate {
  const slice_ue*   ue;
  ue_sched_priority priority;
  units::bytes      pending_bytes;
  /// Restrictions the transport block carries, as selected for the pending bytes. Open for a UL candidate.
  dl_transport_block_restrictions dl_restrictions;
  /// Restrictions the grant satisfies, as selected for the pending bytes. Open for a DL candidate.
  ul_grant_restrictions ul_restrictions;
};

/// Interface of data scheduler that is used to allocate UE DL and UL grants in a given slot
/// The data_scheduler object will be common to all cells and slots.
class scheduler_policy
{
public:
  virtual ~scheduler_policy() = default;

  virtual void add_ue(du_ue_index_t ue_index) = 0;

  virtual void rem_ue(du_ue_index_t ue_index) = 0;

  /// \brief Compute UE candidate priorities for DL scheduling.
  ///
  /// This function is only called in DL slots and only when there are UE candidates with new HARQ transmissions to be
  /// scheduled.
  /// \param[in] pdcch_slot PDCCH slot.
  /// \param[in] pdsch_slot PDSCH slot.
  /// \param[in] ue_candidates UE candidates with new HARQ transmissions to be scheduled.
  virtual void
  compute_ue_dl_priorities(slot_point pdcch_slot, slot_point pdsch_slot, span<ue_newtx_candidate> ue_candidates) = 0;

  /// \brief Compute UE candidate priorities for UL scheduling.
  ///
  /// This function is only called in UL slots and only when there are UE candidates with new HARQ transmissions to be
  /// scheduled.
  /// \param[in] pdcch_slot PDCCH slot.
  /// \param[in] pusch_slot PUSCH slot.
  /// \param[in] ue_candidates UE candidates with new HARQ transmissions to be scheduled.
  virtual void
  compute_ue_ul_priorities(slot_point pdcch_slot, slot_point pusch_slot, span<ue_newtx_candidate> ue_candidates) = 0;

  /// Save UE DL newtx grants.
  virtual void save_dl_newtx_grants(pdu_span<dl_msg_alloc> dl_grants) = 0;

  /// Save UE UL newtx grants.
  virtual void save_ul_newtx_grants(pdu_span<ul_sched_info> ul_grants) = 0;
};

} // namespace ocudu
