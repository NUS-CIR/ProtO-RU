// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "slice_ue_repository.h"
#include <algorithm>

using namespace ocudu;

slice_ue::slice_ue(ue& u_, ue_cell& ue_cc_, ran_slice_id_t slice_id_) : u(u_), ue_cc(ue_cc_), slice_id(slice_id_) {}

// class slice_ue_repository

slice_ue_repository::slice_ue_repository(ran_slice_id_t slice_id_, du_cell_index_t cell_index_, ue_repository& ues_) :
  slice_id(slice_id_), cell_index(cell_index_), ues(ues_)
{
}

bool slice_ue_repository::add_ue(ue& u)
{
  if (not ue_map.contains(u.ue_index)) {
    ue_cell* ue_cc = u.find_cell(cell_index);
    ocudu_sanity_check(ue_cc != nullptr, "Invalid UE added to RAN slice");
    ue_map.emplace(u.ue_index, u, *ue_cc, slice_id);
    return true;
  }
  return false;
}

void slice_ue_repository::add_logical_channel(ue& u, lcid_t lcid, lcg_id_t lcg_id)
{
  if (lcid == LCID_SRB0) {
    // SRB0 is never handled by slice scheduler.
    return;
  }

  // Create UE entry if not created yet.
  add_ue(u);

  // Add LCID to DL slice.
  u.logical_channels().set_lcid_ran_slice(lcid, slice_id);

  // Add LCG-ID to UL slice.
  u.logical_channels().set_lcg_ran_slice(lcg_id, slice_id);
}

void slice_ue_repository::rem_logical_channel(du_ue_index_t ue_idx, lcid_t lcid)
{
  if (not ue_map.contains(ue_idx)) {
    return;
  }
  auto& slice_u = ue_map[ue_idx];

  ue_logical_channel_repository& lc_mng = slice_u.u.logical_channels();
  ocudu_sanity_check(lc_mng.has_slice(slice_id), "slice_ue should not be created without slice");
  if (lc_mng.get_slice_id(lcid) != slice_id) {
    // LCID is not associated with this slice.
    return;
  }

  // Disconnect RAN slice from LCID.
  lc_mng.reset_lcid_ran_slice(lcid);

  // If there are still other bearers in the slice, do not remove the LCG ID.
  logical_channel_config_list_ptr lc_chs = slice_u.u.ue_cfg_dedicated()->logical_channels();
  lcg_id_t lcg_id_to_rem = lc_chs.value().contains(lcid) ? lc_chs.value()[lcid]->lc_group : MAX_NOF_LCGS;
  bool     to_destroy    = true;
  for (logical_channel_config_ptr lc : *lc_chs) {
    if (lc->lcid != lcid and lc->lc_group == lcg_id_to_rem and lc_mng.get_slice_id(lc->lcid) == slice_id) {
      to_destroy = false;
      break;
    }
  }
  if (to_destroy) {
    slice_u.u.logical_channels().reset_lcg_ran_slice(lcg_id_to_rem);
  }

  if (not slice_u.u.logical_channels().has_slice(slice_id)) {
    // If no more bearers active for this UE, remove it from the slice.
    ue_map.erase(ue_idx);
  }
}

void slice_ue_repository::rem_ue(du_ue_index_t ue_index)
{
  if (not ue_map.contains(ue_index)) {
    return;
  }
  ue_map.erase(ue_index);
}

static units::bytes sum_allocated_ul_harq_bytes(const ue& u, std::optional<ran_slice_id_t> slice_id)
{
  units::bytes bytes_in_harqs{0};
  for (unsigned cell_idx = 0, e = u.nof_cells(); cell_idx != e; ++cell_idx) {
    const ue_cell& cc = u.get_cell(to_serv_cell_index(cell_idx));
    bytes_in_harqs += cc.harqs.total_ul_bytes_waiting_ack(slice_id);
  }
  return bytes_in_harqs;
}

units::bytes slice_ue::pending_ul_newtx_bytes() const
{
  return get_newtx_bytes(units::bytes{u.logical_channels().ul_pending_bytes(slice_id)});
}

ue_logical_channel_repository::ul_grant_selection slice_ue::select_ul_grant() const
{
  ue_logical_channel_repository::ul_grant_selection selection = u.logical_channels().select_ul_grant(slice_id);
  const units::bytes                                served_bytes{selection.pending_bytes};
  const units::bytes                                slice_bytes{u.logical_channels().ul_pending_bytes(slice_id)};

  // The bytes in flight are counted per slice, not per group, so they come off the slice total and the served groups
  // cap the result. A group whose data is in flight then costs the grant padding, and only undersizes it once the
  // bytes in flight outgrow what the groups left out hold. A grant serving every group takes the slice total as it
  // stands, so that the minimum an SR earns outlives the bytes already in flight.
  units::bytes newtx_bytes = get_newtx_bytes(slice_bytes);
  if (slice_bytes.value() > 0 and selection.restrictions.is_restricted()) {
    newtx_bytes = std::min(newtx_bytes, served_bytes);
  }
  selection.pending_bytes = newtx_bytes.value();
  return selection;
}

units::bytes slice_ue::get_newtx_bytes(units::bytes slice_bytes) const
{
  static constexpr units::bytes SR_GRANT_BYTES{512};

  if (slice_bytes.value() > 0) {
    const units::bytes in_flight = sum_allocated_ul_harq_bytes(u, slice_id);
    if (slice_bytes > in_flight) {
      return slice_bytes - in_flight;
    }
  }

  // In case a SR is pending and this is the SRB slice, we return a minimum SR grant size if no other bearers have
  // pending data.
  if (slice_id == SRB_RAN_SLICE_ID and has_pending_sr()) {
    // The SR carries neither its volume nor the bearer it comes from, so every bearer of the UE decides.
    const units::bytes ue_bytes{u.logical_channels().ul_pending_bytes()};
    return ue_bytes > sum_allocated_ul_harq_bytes(u, std::nullopt) ? units::bytes{0} : SR_GRANT_BYTES;
  }

  return units::bytes{0};
}
