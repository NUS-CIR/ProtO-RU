// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "logical_channel_system.h"
#include "ocudu/scheduler/scheduler_feedback_handler.h"

using namespace ocudu;

/// (Implementation-defined) Estimation of how much space the MAC should leave for RLC segmentation header overhead.
static constexpr unsigned RLC_SEGMENTATION_OVERHEAD = 3;

/// Estimation of the space required for MAC SDU (including RLC overhead) + MAC subheader
static constexpr unsigned get_mac_sdu_with_subhdr_and_rlc_hdr_estim(lcid_t lcid, unsigned payload)
{
  const unsigned rlc_ovh = (lcid != LCID_SRB0 && payload > 0) ? RLC_SEGMENTATION_OVERHEAD : 0;
  return get_mac_sdu_required_bytes(payload + rlc_ovh);
}

/// Determine the MAC SDU size from the total size of MAC SDU + MAC subheader.
static unsigned get_mac_sdu_size(unsigned sdu_and_subheader_bytes)
{
  if (sdu_and_subheader_bytes == 0) {
    return 0;
  }
  const unsigned sdu_size = sdu_and_subheader_bytes - MIN_MAC_SDU_SUBHEADER_SIZE;
  return sdu_size < MAC_SDU_SUBHEADER_LENGTH_THRES ? sdu_size : sdu_size - 1;
}

logical_channel_system_utils::logical_channel_mapper::logical_channel_mapper() :
  ue_lcid_to_dl_row_id(MAX_NOF_RB_LCIDS * MAX_NOF_DU_UES, invalid_row_id),
  ue_lcgid_to_ul_row_id(MAX_NOF_LCGS * MAX_NOF_DU_UES, invalid_row_id)
{
  /// (Implementation-defined) Pre-reserved number of DRBs per UE for QoS tracking.
  static constexpr unsigned PRERESERVED_NOF_DRBS_PER_UE = 2;
  qos_channels.reserve(PRERESERVED_NOF_DRBS_PER_UE * MAX_NOF_DU_UES);
  qos_lcgs.reserve(PRERESERVED_NOF_DRBS_PER_UE * MAX_NOF_DU_UES);
  dl_fields.reserve(MAX_NOF_RB_LCIDS * MAX_NOF_DU_UES);
  ul_fields.reserve(MAX_NOF_LCGS * MAX_NOF_DU_UES);
}

void logical_channel_system_utils::logical_channel_mapper::slot_indication()
{
  // Update the bit rates of the UE logical channels with tracked bit rates.
  for (lc_qos_context& lc : qos_channels) {
    lc.dl_avg_bytes_per_slot.push(lc.dl_last_sched_bytes);
    lc.dl_last_sched_bytes = 0;
  }
  for (lcg_qos_context& lcg : qos_lcgs) {
    lcg.ul_avg_bytes_per_slot.push(lcg.ul_last_sched_bytes);
    lcg.ul_last_sched_bytes = 0;
  }
}

std::pair<soa::row_id, soa::row_id>
logical_channel_system_utils::logical_channel_mapper::addmod_lc_and_lcg(du_ue_index_t                 ue_index,
                                                                        const logical_channel_config& lc_cfg,
                                                                        unsigned                      slots_per_msec)
{
  // Create LC entry if not already present.
  soa::row_id lc_rid;
  {
    uint16_t& lc_row = get_row_id_entry(ue_index, lc_cfg.lcid);
    if (lc_row == invalid_row_id) {
      // New LCID. Create a new LC.
      lc_rid = dl_fields.insert(units::bytes{0}, invalid_slice_id, std::nullopt, slot_point{});
      lc_row = static_cast<uint16_t>(lc_rid.value());
    } else {
      // Existing LC entry.
      lc_rid = soa::row_id{lc_row};
    }
  }

  // Create LCG entry if not already present.
  soa::row_id lcg_rid;
  {
    auto& ul_row = get_row_id_entry(ue_index, lc_cfg.lc_group);
    if (ul_row == invalid_row_id) {
      // New LCG entry.
      lcg_rid = ul_fields.insert(units::bytes{0}, invalid_slice_id, std::nullopt, lc_cfg.triggered_ul_grant);
      ul_row  = static_cast<uint16_t>(lcg_rid.value());
    } else {
      lcg_rid                                                  = soa::row_id{ul_row};
      ul_fields.at<ul_field_type::triggered_ul_grant>(lcg_rid) = lc_cfg.triggered_ul_grant;
    }
  }

  // Update QoS tracking config.
  auto& dl_qrow = dl_fields.at<dl_field_type::qos_row>(lc_rid);
  auto& ul_qrow = ul_fields.at<ul_field_type::qos_row>(lcg_rid);
  if (lc_cfg.qos.has_value() and lc_cfg.qos->gbr_qos_info.has_value()) {
    // QoS tracking is enabled.
    const unsigned N     = lc_cfg.qos->qos.average_window_ms.value() * slots_per_msec;
    const float    alpha = 1.0 / N;

    if (not dl_qrow.has_value()) {
      // Initiate QoS DL tracking.
      dl_qrow = qos_channels.insert(lc_qos_context{0, exp_average_fast_start<float>{alpha}});
    } else {
      // Update QoS DL tracking window decay factor.
      qos_channels[*dl_qrow].dl_avg_bytes_per_slot.set_alpha(alpha);
    }
    if (not ul_qrow.has_value()) {
      // Initiate QoS UL tracking.
      ul_qrow = qos_lcgs.insert(lcg_qos_context{0, 0, exp_average_fast_start<float>{alpha}});
    } else {
      // Update QoS UL tracking window decay factor.
      qos_lcgs[*ul_qrow].ul_avg_bytes_per_slot.set_alpha(alpha);
    }
  } else {
    // Disable DL QoS tracking, if previously enabled.
    if (dl_qrow.has_value()) {
      qos_channels.erase(*dl_qrow);
      dl_qrow = std::nullopt;
    }

    // Disable UL QoS tracking, if previously enabled.
    if (ul_qrow.has_value()) {
      qos_lcgs.erase(*ul_qrow);
      ul_qrow = std::nullopt;
    }
  }

  return std::make_pair(lc_rid, lcg_rid);
}

void logical_channel_system_utils::logical_channel_mapper::rem_lc(du_ue_index_t ue_index, lcid_t lcid)
{
  uint16_t& lc_row = get_row_id_entry(ue_index, lcid);
  ocudu_assert(lc_row != invalid_row_id, "Invalid UE LCID deregistration");
  const soa::row_id dl_rid = soa::row_id{lc_row};

  // Detach LC from QoS tracking, if active.
  if (auto qrow = dl_qos_row(dl_rid); qrow.has_value()) {
    qos_channels.erase(*qrow);
  }

  // Remove LC entry.
  dl_fields.erase(dl_rid);
  lc_row = invalid_row_id;
}

void logical_channel_system_utils::logical_channel_mapper::rem_lcg(du_ue_index_t ue_index, lcg_id_t lcgid)
{
  uint16_t& lcg_row = get_row_id_entry(ue_index, lcgid);
  ocudu_assert(lcg_row != invalid_row_id, "Invalid UE LCG-ID deregistration");
  const soa::row_id ul_rid = soa::row_id{lcg_row};

  // Detach LCG from QoS tracking, if active.
  if (auto qrow = ul_qos_row(ul_rid); qrow.has_value()) {
    qos_lcgs.erase(*qrow);
  }

  // Remove LCG entry.
  ul_fields.erase(ul_rid);
  lcg_row = invalid_row_id;
}

void logical_channel_system_utils::logical_channel_mapper::register_lc_slice(soa::row_id    lc_rid,
                                                                             ran_slice_id_t dl_slice_id)
{
  dl_fields.at<dl_field_type::slice_id>(lc_rid) = dl_slice_id;
}

void logical_channel_system_utils::logical_channel_mapper::register_lcg_slice(soa::row_id rid, ran_slice_id_t slice_id)
{
  ul_fields.at<ul_field_type::slice_id>(rid) = slice_id;
}

void logical_channel_system_utils::logical_channel_mapper::deregister_lc_slice(soa::row_id rid)
{
  dl_fields.at<dl_field_type::slice_id>(rid) = invalid_slice_id;
}

void logical_channel_system_utils::logical_channel_mapper::deregister_lcg_slice(soa::row_id lcg_rid)
{
  ul_fields.at<ul_field_type::slice_id>(lcg_rid) = invalid_slice_id;
}

unsigned* logical_channel_system_utils::logical_channel_mapper::last_dl_sched_bytes(soa::row_id lc_rid)
{
  if (auto& qos_row_opt = dl_qos_row(lc_rid); qos_row_opt.has_value()) {
    return &qos_channels[*qos_row_opt].dl_last_sched_bytes;
  }
  return nullptr;
}

unsigned* logical_channel_system_utils::logical_channel_mapper::last_ul_sched_bytes(soa::row_id lc_rid)
{
  if (auto& qos_row_opt = ul_qos_row(lc_rid); qos_row_opt.has_value()) {
    return &qos_lcgs[*qos_row_opt].ul_last_sched_bytes;
  }
  return nullptr;
}

unsigned* logical_channel_system_utils::logical_channel_mapper::ul_sched_bytes_accum(soa::row_id lcg_rid)
{
  if (auto& qos_row_opt = ul_qos_row(lcg_rid); qos_row_opt.has_value()) {
    return &qos_lcgs[*qos_row_opt].sched_bytes_accum;
  }
  return nullptr;
}

double logical_channel_system_utils::logical_channel_mapper::avg_dl_bits_per_slot(soa::row_id lc_rid) const
{
  if (auto& qos_row_opt = dl_qos_row(lc_rid); qos_row_opt.has_value()) {
    return qos_channels[*qos_row_opt].dl_avg_bytes_per_slot.average() * 8U;
  }
  return 0.0;
}

double logical_channel_system_utils::logical_channel_mapper::avg_ul_bits_per_slot(soa::row_id lcg_rid) const
{
  if (auto& qos_row_opt = ul_qos_row(lcg_rid); qos_row_opt.has_value()) {
    return qos_lcgs[*qos_row_opt].ul_avg_bytes_per_slot.average() * 8U;
  }
  return 0.0;
}

logical_channel_system::logical_channel_system()
{
  static constexpr unsigned PRERESERVED_NOF_CES = MAX_NOF_DU_UES;
  static constexpr unsigned PRERESERVED_SLICES  = 4;

  // Pre-reserve space to avoid allocations in latency-critical path.
  pending_ces.reserve(PRERESERVED_NOF_CES);
  slices.reserve(PRERESERVED_SLICES);
  ues.reserve(MAX_NOF_DU_UES);
}

void logical_channel_system::slot_indication()
{
  lc_mapper.slot_indication();
}

static const logical_channel_config_list     empty_lc_cfg_list;
static const logical_channel_config_list_ptr empty_lc_cfg_list_ptr{empty_lc_cfg_list};

ue_logical_channel_repository logical_channel_system::create_ue(du_ue_index_t                   ue_index,
                                                                subcarrier_spacing              scs_common,
                                                                bool                            starts_in_fallback,
                                                                logical_channel_config_list_ptr log_channels_configs)
{
  ocudu_assert(configured_ues.size() <= ue_index or not configured_ues.test(ue_index), "duplicate UE index");
  // Creates a UE entry in the table.
  const soa::row_id ue_rid =
      ues.insert(ue_config_context{ue_index, get_nof_slots_per_subframe(scs_common), empty_lc_cfg_list_ptr},
                 ue_context{starts_in_fallback},
                 ue_dl_slice_context{},
                 ue_ul_slice_context{},
                 ue_dl_channel_context{},
                 ue_ul_channel_context{});
  if (configured_ues.size() <= ue_index) {
    // Need to resize UE bitsets.
    configured_ues.resize(ue_index + 1);
    ues_with_pending_ces.resize(ue_index + 1);
    ues_with_pending_sr.resize(ue_index + 1);
    for (auto it = slices.begin(); it != slices.end(); ++it) {
      it->second.pending_dl_ues.resize(ue_index + 1);
      it->second.pending_ul_ues.resize(ue_index + 1);
    }
  }
  configured_ues.set(ue_index);

  // Apply LC configuration.
  configure(ue_rid, log_channels_configs.has_value() ? log_channels_configs : empty_lc_cfg_list_ptr);

  // Return handle to UE logical channel repository.
  return ue_logical_channel_repository{*this, ue_index, ue_rid};
}

size_t logical_channel_system::nof_logical_channels() const
{
  size_t count = 0;
  for (const_ue_row u : ues) {
    count += u.at<ue_dl_channel_context>().channels.size();
  }
  return count;
}

bounded_bitset<MAX_NOF_DU_UES> logical_channel_system::get_ues_with_dl_pending_data(ran_slice_id_t slice_id) const
{
  auto slice_it = slices.find(slice_id);
  if (slice_it == slices.end()) {
    // This slice is not configured.
    return bounded_bitset<MAX_NOF_DU_UES>{};
  }
  if (slice_id != SRB_RAN_SLICE_ID) {
    return slice_it->second.pending_dl_ues;
  }
  return slice_it->second.pending_dl_ues | ues_with_pending_ces;
}

bounded_bitset<MAX_NOF_DU_UES> logical_channel_system::get_ues_with_ul_pending_data(ran_slice_id_t slice_id) const
{
  auto slice_it = slices.find(slice_id);
  if (slice_it == slices.end()) {
    // This slice is not configured.
    return bounded_bitset<MAX_NOF_DU_UES>{};
  }
  if (slice_id != SRB_RAN_SLICE_ID) {
    return slice_it->second.pending_ul_ues;
  }
  return slice_it->second.pending_ul_ues | ues_with_pending_sr;
}

void ue_logical_channel_repository::set_fallback_state(bool enter_fallback)
{
  auto        u      = get_ue_row();
  ue_context& ue_ctx = u.at<ue_context>();
  if (ue_ctx.fallback_state == enter_fallback) {
    // no-op.
    return;
  }
  // Fallback state toggled.
  ue_ctx.fallback_state = enter_fallback;

  // Update pending newTx data for all UE slices.
  auto  ueidx     = u.at<ue_config_context>().ue_index;
  auto& ue_dl_ctx = u.at<ue_dl_slice_context>();
  auto& ue_ul_ctx = u.at<ue_ul_slice_context>();
  if (ue_ctx.fallback_state) {
    // Entering fallback mode, clear all pending data in slices.
    for (auto it = ue_dl_ctx.pending_bytes_per_slice.begin(), it_end = ue_dl_ctx.pending_bytes_per_slice.end();
         it != it_end;
         ++it) {
      if (it->second > 0) {
        it->second = 0;
        parent->slices.at(it->first).pending_dl_ues.reset(ueidx);
      }
    }
    parent->ues_with_pending_ces.reset(ueidx);
    for (auto it = ue_ul_ctx.pending_bytes_per_slice.begin(), it_end = ue_ul_ctx.pending_bytes_per_slice.end();
         it != it_end;
         ++it) {
      if (it->second > 0) {
        it->second = 0;
        parent->slices.at(it->first).pending_ul_ues.reset(ueidx);
      }
    }
  } else {
    // Exiting fallback mode, recompute pending data in slices.
    const auto& ue_ch = u.at<ue_dl_channel_context>();
    for (const auto& [lcid, lc_rid] : ue_ch.channels) {
      parent->on_single_channel_buf_st_update(
          u, parent->lc_mapper.dl_slice_id(lc_rid), parent->lc_mapper.dl_buf_st(lc_rid).value(), 0);
    }
    if (not ue_ch.pending_ces.empty()) {
      parent->ues_with_pending_ces.set(ueidx);
    }
    auto& ue_ul_ch = u.at<ue_ul_channel_context>();
    for (const auto& [lcgid, lcg_rid] : ue_ul_ch.lcgs) {
      parent->on_single_lcg_buf_st_update(
          u, lcgid, parent->lc_mapper.ul_slice_id(lcg_rid), parent->lc_mapper.ul_buf_st(lcg_rid).value(), 0);
    }
  }
}

/// Helper to retrieve logical channel priority.
static uint16_t get_lc_prio(const logical_channel_config& cfg)
{
  uint16_t prio = 0;
  if (is_srb(cfg.lcid)) {
    prio = cfg.lcid <= LCID_SRB1 ? 0 : 1;
  } else {
    prio = cfg.qos.has_value() ? cfg.qos->qos.priority.value() * cfg.qos->arp_priority.value()
                               : qos_prio_level_t::max() * arp_prio_level_t::max();
  }
  return prio;
}

void logical_channel_system::remove_ue(soa::row_id ue_rid)
{
  ocudu_assert(ues.has_row_id(ue_rid), "trying to remove non-existing UE");
  const auto ue_index = ues.row(ue_rid).at<ue_config_context>().ue_index;
  ocudu_assert(configured_ues.size() > ue_index and configured_ues.test(ue_index), "duplicate UE index");

  // Disable any slicing, QoS and CE tracking.
  deactivate(ue_rid);

  // Remove all logical channels.
  auto& ue_ch = ues.row(ue_rid).at<ue_dl_channel_context>();
  for (const auto& [lcid, lc_rid] : ue_ch.channels) {
    lc_mapper.rem_lc(ue_index, lcid);
  }
  auto& ue_ul_ch = ues.row(ue_rid).at<ue_ul_channel_context>();
  for (const auto& [lcgid, lcg_rid] : ue_ul_ch.lcgs) {
    lc_mapper.rem_lcg(ue_index, lcgid);
  }

  // Destroy UE context.
  ues.erase(ue_rid);

  // Update UE config bitset (strip rightmost zeros).
  // Note: if return of highest is -1 (not found), the new size will be 0.
  configured_ues.reset(ue_index);
  const auto new_size = static_cast<size_t>(configured_ues.find_highest(true) + 1);
  if (configured_ues.size() != new_size) {
    configured_ues.resize(new_size);
    ues_with_pending_ces.resize(new_size);
    ues_with_pending_sr.resize(new_size);
    for (auto it = slices.begin(), it_end = slices.end(); it != it_end; ++it) {
      it->second.pending_dl_ues.resize(new_size);
      it->second.pending_ul_ues.resize(new_size);
    }
  }
}

void logical_channel_system::configure(soa::row_id ue_rid, logical_channel_config_list_ptr log_channels_configs)
{
  auto  u        = ues.row(ue_rid);
  auto& ue_cfg   = u.at<ue_config_context>();
  auto& ue_ch    = u.at<ue_dl_channel_context>();
  auto& ue_ul_ch = u.at<ue_ul_channel_context>();

  // Exchange new config.
  const auto old_cfgs    = ue_cfg.channel_configs;
  ue_cfg.channel_configs = log_channels_configs;

  // Remove LCGs that are not in the new config anymore.
  for (const auto& old_lc : *old_cfgs) {
    if (not ue_cfg.channel_configs->contains(old_lc->lcid)) {
      // LCID got removed.

      // Detach from RAN slice, if association exists.
      deregister_lc_ran_slice(ue_rid, old_lc->lcid);

      // Delete the LC context.
      ue_ch.channels.erase(old_lc->lcid);
      lc_mapper.rem_lc(ue_cfg.ue_index, old_lc->lcid);
    }
    if (not ue_cfg.channel_configs->contains(old_lc->lc_group) and ue_ul_ch.lcgs.contains(old_lc->lc_group)) {
      // LCG-ID got removed.

      // Detach LCG from RAN slice, if association exists.
      deregister_lcg_ran_slice(ue_rid, old_lc->lc_group);

      // Delete the LCG context.
      ue_ul_ch.lcgs.erase(old_lc->lc_group);
      lc_mapper.rem_lcg(get_ue(ue_rid).at<ue_config_context>().ue_index, old_lc->lc_group);
    }
  }

  // Add new or modify existing LCs and LCGs.
  // Note: LCID will be removed from sorted_channels later.
  for (const auto& new_cfg : *ue_cfg.channel_configs) {
    // Add or modify LC and LCG.
    auto [lc_rid, lcg_rid] = lc_mapper.addmod_lc_and_lcg(ue_cfg.ue_index, *new_cfg, ue_cfg.slots_per_msec);

    if (not ue_ch.channels.contains(new_cfg->lcid)) {
      // New LCID, create context.
      ue_ch.channels.emplace(new_cfg->lcid, lc_rid);
    }

    if (not ue_ul_ch.lcgs.contains(new_cfg->lc_group)) {
      // New LCG, create context.
      ue_ul_ch.lcgs.emplace(new_cfg->lc_group, lcg_rid);
    }
  }

  // Refresh sorted channels list.
  ue_ch.sorted_channels.clear();
  dl_transport_block_restrictions dl_restrictions;
  ul_grant_restrictions           ul_restrictions;
  for (const auto& lc_cfg : *ue_cfg.channel_configs) {
    ue_ch.sorted_channels.push_back(lc_cfg->lcid);
    dl_restrictions.set(*lc_cfg);
    ul_restrictions.set(*lc_cfg);
  }
  ue_ch.restricted_channel_present    = dl_restrictions.is_restricted();
  ue_ul_ch.restricted_channel_present = ul_restrictions.is_restricted();
  std::sort(ue_ch.sorted_channels.begin(), ue_ch.sorted_channels.end(), [&ue_cfg](lcid_t lhs, lcid_t rhs) {
    return get_lc_prio(*ue_cfg.channel_configs.value()[lhs]) < get_lc_prio(*ue_cfg.channel_configs.value()[rhs]);
  });
}

void logical_channel_system::deactivate(soa::row_id ue_rid)
{
  auto  u      = ues.row(ue_rid);
  auto  ue_idx = u.at<ue_config_context>().ue_index;
  auto& ue_ch  = u.at<ue_dl_channel_context>();
  auto& ue_ctx = u.at<ue_context>();

  // Clear UE pending CEs.
  ue_ctx.pending_con_res_id = false;
  auto ce_list              = ue_ch.pending_ces.get_list(pending_ces);
  while (not ce_list.empty()) {
    pending_ces.erase(ce_list.pop_front());
  }
  ue_ctx.pending_ce_bytes = 0;
  ues_with_pending_ces.reset(ue_idx);

  // Reset any pending SR.
  ues_with_pending_sr.reset(ue_idx);

  // Set empty logical channel configuration.
  configure(ue_rid, empty_lc_cfg_list_ptr);
}

void logical_channel_system::set_lcid_ran_slice(soa::row_id ue_rid, lcid_t lcid, ran_slice_id_t slice_id)
{
  auto  u     = ues.row(ue_rid);
  auto& ue_ch = u.at<ue_dl_channel_context>();
  ocudu_assert(ue_ch.channels.contains(lcid), "LCID not configured");

  // Fetch current slice ID for the LCID.
  const soa::row_id lc_rid    = ue_ch.channels.at(lcid);
  auto              cur_slice = lc_mapper.dl_slice_id(lc_rid);
  if (cur_slice == slice_id) {
    // No-op.
    return;
  }

  // Remove LCID from previous slice.
  deregister_lc_ran_slice(ue_rid, lcid);

  // If slice does not exist yet for the UE, create it.
  ue_dl_slice_context& ue_dl_ctx = u.at<ue_dl_slice_context>();
  auto                 slice_it  = ue_dl_ctx.pending_bytes_per_slice.find(slice_id);
  if (slice_it == ue_dl_ctx.pending_bytes_per_slice.end()) {
    // New slice, add to the list.
    auto data_slice_it = slices.find(slice_id);
    if (data_slice_it == slices.end()) {
      // Create slice if not existing yet.
      slices.emplace(slice_id,
                     ran_slice_context{bounded_bitset<MAX_NOF_DU_UES>(configured_ues.size()),
                                       bounded_bitset<MAX_NOF_DU_UES>(configured_ues.size())});
    }
    ue_dl_ctx.pending_bytes_per_slice.emplace(slice_id, 0);
  }

  // Add LCID to new slice.
  lc_mapper.register_lc_slice(lc_rid, slice_id);

  // Update pending bytes for the slice.
  on_single_channel_buf_st_update(u, slice_id, lc_mapper.dl_buf_st(lc_rid).value(), 0);
}

void logical_channel_system::set_lcg_ran_slice(soa::row_id ue_rid, lcg_id_t lcgid, ran_slice_id_t slice_id)
{
  auto  u      = ues.row(ue_rid);
  auto& ue_ch  = u.at<ue_ul_channel_context>();
  auto& ue_ctx = u.at<ue_ul_slice_context>();
  ocudu_assert(ue_ch.lcgs.contains(lcgid), "LCG-ID not configured");

  soa::row_id lcg_rid   = ue_ch.lcgs.at(lcgid);
  auto        cur_slice = lc_mapper.ul_slice_id(lcg_rid);
  if (cur_slice == slice_id) {
    // No-op.
    return;
  }

  // Remove LCG-ID from previous slice.
  deregister_lcg_ran_slice(ue_rid, lcgid);

  // If slice does not exist yet for the UE, create it.
  auto slice_it = ue_ctx.pending_bytes_per_slice.find(slice_id);
  if (slice_it == ue_ctx.pending_bytes_per_slice.end()) {
    // New slice, add to the list.
    auto data_slice_it = slices.find(slice_id);
    if (data_slice_it == slices.end()) {
      // Create slice if not existing yet.
      slices.emplace(slice_id,
                     ran_slice_context{bounded_bitset<MAX_NOF_DU_UES>(configured_ues.size()),
                                       bounded_bitset<MAX_NOF_DU_UES>(configured_ues.size())});
    }
    ue_ctx.pending_bytes_per_slice.emplace(slice_id, 0);
  }

  // Add LCG-ID to new slice.
  lc_mapper.register_lcg_slice(lcg_rid, slice_id);

  // Update pending bytes for the slice.
  on_single_lcg_buf_st_update(u, lcgid, slice_id, lc_mapper.ul_buf_st(lcg_rid).value(), 0);
}

void logical_channel_system::deregister_lc_ran_slice(soa::row_id ue_rid, lcid_t lcid)
{
  auto  u     = get_ue(ue_rid);
  auto& ue_ch = u.at<ue_dl_channel_context>();

  if (ue_ch.channels.contains(lcid)) {
    soa::row_id lc_rid        = ue_ch.channels.at(lcid);
    auto        prev_slice_id = lc_mapper.dl_slice_id(lc_rid);

    // Update pending bytes for the slice.
    on_single_channel_buf_st_update(u, prev_slice_id, 0, lc_mapper.dl_buf_st(lc_rid).value());

    if (prev_slice_id.has_value()) {
      // Remove slice from LC mapper.
      lc_mapper.deregister_lc_slice(lc_rid);

      // If it is the last LC attached to the slice, remove slice.
      if (std::none_of(ue_ch.channels.begin(), ue_ch.channels.end(), [this, slid = *prev_slice_id](const auto& p) {
            return lc_mapper.dl_slice_id(p.second) == slid;
          })) {
        // No other LC is still attached to the slice for this UE. Remove slice from UE.
        auto& ue_dl_ctxt = u.at<ue_dl_slice_context>();
        ue_dl_ctxt.pending_bytes_per_slice.erase(*prev_slice_id);
      }
    }
  }
}

void logical_channel_system::deregister_lcg_ran_slice(soa::row_id ue_rid, lcg_id_t lcgid)
{
  auto  u     = get_ue(ue_rid);
  auto& ue_ch = u.at<ue_ul_channel_context>();
  auto  lcgit = ue_ch.lcgs.find(lcgid);
  if (lcgit == ue_ch.lcgs.end()) {
    return;
  }
  soa::row_id lcg_rid = lcgit->second;

  // Update pending bytes for the slice.
  auto prev_slice_id = lc_mapper.ul_slice_id(lcg_rid);
  on_single_lcg_buf_st_update(u, lcgid, prev_slice_id, 0, lc_mapper.ul_buf_st(lcg_rid).value());

  if (prev_slice_id.has_value()) {
    // Remove slice from LC mapper.
    lc_mapper.deregister_lcg_slice(lcg_rid);

    // If it is the last LCG attached to the slice, remove slice.
    if (std::none_of(ue_ch.lcgs.begin(), ue_ch.lcgs.end(), [this, slid = *prev_slice_id](const auto& ch) {
          return lc_mapper.ul_slice_id(ch.second) == slid;
        })) {
      // No other LC is still attached to the slice for this UE. Remove slice from UE.
      auto& ue_ctxt = u.at<ue_ul_slice_context>();
      ue_ctxt.pending_bytes_per_slice.erase(*prev_slice_id);
    }
  }
}

void logical_channel_system::handle_dl_buffer_status_indication(soa::row_id ue_row_id,
                                                                lcid_t      lcid,
                                                                unsigned    buffer_status,
                                                                slot_point  hol_toa)
{
  // We apply this limit to avoid potential overflows.
  static constexpr unsigned max_buffer_status = 1U << 24U;
  ocudu_sanity_check(lcid < MAX_NOF_RB_LCIDS, "Max LCID value 32 exceeded");
  auto  u     = get_ue(ue_row_id);
  auto& ue_ch = u.at<ue_dl_channel_context>();
  if (not ue_ch.channels.contains(lcid)) {
    // LCID was probably removed meanwhile. Ignore buffer status indication.
    return;
  }

  soa::row_id lc_rid      = ue_ch.channels.at(lcid);
  auto&       buf_st      = lc_mapper.dl_buf_st(lc_rid);
  auto        prev_buf_st = buf_st.value();

  // Update LC buffer status.
  buf_st                    = units::bytes{std::min(buffer_status, max_buffer_status)};
  lc_mapper.hol_toa(lc_rid) = hol_toa;

  // Update slice pending bytes.
  on_single_channel_buf_st_update(u, lc_mapper.dl_slice_id(lc_rid), buf_st.value(), prev_buf_st);
}

void logical_channel_system::on_single_channel_buf_st_update(ue_row&                              u,
                                                             const std::optional<ran_slice_id_t>& slice_id,
                                                             unsigned                             new_buf_st,
                                                             unsigned                             prev_buf_st)
{
  if (not slice_id.has_value()) {
    // No slice associated with this logical channel or it is inactive.
    return;
  }
  auto& ue_ctx = u.at<ue_context>();
  if (ue_ctx.fallback_state) {
    // In fallback mode, slices are disabled.
    return;
  }

  // In case this logical channel has a RAN slice associated, (differentially) update the pending bytes for the slice.
  auto&      ue_dl_ctx               = u.at<ue_dl_slice_context>();
  auto&      slice_pending_bytes     = ue_dl_ctx.pending_bytes_per_slice.find(*slice_id)->second;
  const auto prev_buf_st_incl_subhdr = get_mac_sdu_required_bytes(prev_buf_st);
  const auto new_buf_st_incl_subhdr  = get_mac_sdu_required_bytes(new_buf_st);
  ocudu_sanity_check(slice_pending_bytes + new_buf_st_incl_subhdr >= prev_buf_st_incl_subhdr,
                     "Invalid slice pending bytes");
  const auto prev_slice_pending_bytes = slice_pending_bytes;
  slice_pending_bytes += new_buf_st_incl_subhdr - prev_buf_st_incl_subhdr;

  if ((prev_slice_pending_bytes > 0) != (slice_pending_bytes > 0)) {
    // zero crossing detected. Update the bitmap of UEs with pending data for a given slice.
    auto& ue_cfg = u.at<ue_config_context>();
    slices.at(*slice_id).pending_dl_ues.set(ue_cfg.ue_index, slice_pending_bytes > 0);
  }
}

void logical_channel_system::on_single_lcg_buf_st_update(ue_row&                              u,
                                                         lcg_id_t                             lcgid,
                                                         const std::optional<ran_slice_id_t>& slice_id,
                                                         unsigned                             new_buf_st,
                                                         unsigned                             prev_buf_st)
{
  if (not slice_id.has_value() or u.at<ue_context>().fallback_state) {
    // No slice associated with this logical channel group, the logical channel group is inactive or the UE is in
    // fallback.
    return;
  }
  auto& ue_ctx = u.at<ue_ul_slice_context>();

  // In case this logical channel has a RAN slice associated, (differentially) update the pending bytes for the slice.
  auto&      slice_pending_bytes     = ue_ctx.pending_bytes_per_slice.find(*slice_id)->second;
  const auto prev_buf_st_incl_subhdr = get_mac_sdu_required_bytes(add_upper_layer_header_bytes(lcgid, prev_buf_st));
  const auto new_buf_st_incl_subhdr  = get_mac_sdu_required_bytes(add_upper_layer_header_bytes(lcgid, new_buf_st));
  ocudu_assert(slice_pending_bytes + new_buf_st_incl_subhdr >= prev_buf_st_incl_subhdr, "Invalid slice pending bytes");
  const auto prev_slice_pending_bytes = slice_pending_bytes;
  slice_pending_bytes += new_buf_st_incl_subhdr - prev_buf_st_incl_subhdr;

  if ((prev_slice_pending_bytes > 0) != (slice_pending_bytes > 0)) {
    // zero crossing detected. Update the slice UE with pending data bitmap.
    auto& ue_cfg = u.at<ue_config_context>();
    slices.at(*slice_id).pending_ul_ues.set(ue_cfg.ue_index, slice_pending_bytes > 0);
  }
}

void logical_channel_system::handle_mac_ce_indication(soa::row_id ue_row_id, const mac_ce_info& ce)
{
  auto  u      = get_ue(ue_row_id);
  auto& ue_ctx = u.at<ue_context>();
  if (ce.ce_lcid == lcid_dl_sch_t::UE_CON_RES_ID) {
    // CON RES is a special case, as it needs to be always scheduled first.
    ue_ctx.pending_con_res_id = true;
    return;
  }
  auto&      ue_ch_ctx          = u.at<ue_dl_channel_context>();
  auto       ce_list            = ue_ch_ctx.pending_ces.get_list(pending_ces);
  auto       tail               = ce_list.before_begin();
  const bool supersedes_pending = ce.ce_lcid == lcid_dl_sch_t::TA_CMD or
                                  ce.ce_lcid == lcid_dl_sch_t::from_elcid(elcid_dl_sch_t::DIFFERENTIAL_KOFFSET);
  for (auto it = ce_list.begin(); it != ce_list.end(); ++it) {
    if (supersedes_pending and ce.ce_lcid == it->info.ce_lcid) {
      // A newer command of the same kind replaces the one still queued.
      // Note: The LCIDs match and both are fixed size, so the pending CE bytes do not change.
      pending_ces[it.id()].info.ce_payload = ce.ce_payload;
      return;
    }
    tail = it;
  }
  // New CE, append to the list.
  auto ce_rid = pending_ces.insert(mac_ce_context{ce, {}});
  ce_list.insert_after(tail, ce_rid);
  // Update sum of pending CE bytes.
  unsigned new_ce_bytes = ue_ctx.pending_ce_bytes +
                          (ce.ce_lcid.is_var_len_ce() ? get_mac_sdu_required_bytes(ce.ce_lcid.sizeof_ce())
                                                      : get_mac_ce_subheader_size(ce.ce_lcid) + ce.ce_lcid.sizeof_ce());
  static constexpr unsigned MAX_PENDING_CE_BYTES = 1U << 14U;
  ocudu_assert(new_ce_bytes <= MAX_PENDING_CE_BYTES, "Exceeded maximum pending CE bytes per UE");
  ue_ctx.pending_ce_bytes = new_ce_bytes;
  ues_with_pending_ces.set(u.at<ue_config_context>().ue_index, true);
}

void logical_channel_system::handle_sr_indication(soa::row_id ue_row_id, slot_point uci_slot)
{
  auto  u     = get_ue(ue_row_id);
  auto& ue_ch = u.at<ue_ul_channel_context>();
  if (not ue_ch.oldest_sr_sl_rx.valid()) {
    // This is a transition from SR not pending to SR pending.
    ue_ch.oldest_sr_sl_rx = uci_slot;
  }
}

void logical_channel_system::reset_sr_indication(soa::row_id ue_row_id)
{
  auto  u               = get_ue(ue_row_id);
  auto& ue_ch           = u.at<ue_ul_channel_context>();
  ue_ch.oldest_sr_sl_rx = {};
}

slot_point logical_channel_system::pending_sr_slot_rx(soa::row_id ue_row_id) const
{
  auto        u     = get_ue(ue_row_id);
  const auto& ue_ch = u.at<ue_ul_channel_context>();
  return ue_ch.oldest_sr_sl_rx;
}

void logical_channel_system::handle_bsr_indication(soa::row_id ue_row_id, const ul_bsr_indication_message& msg)
{
  // This is the maximum bounded value that can be reported by the BSR, as per TS 38.321, Table 6.1.3.1-1.
  static constexpr unsigned max_short_bsr = 150000;
  // We apply this limit to avoid potential overflows.
  static constexpr unsigned max_buffer_status = 1U << 24U;

  auto  u     = get_ue(ue_row_id);
  auto& ue_ch = u.at<ue_ul_channel_context>();

  for (const auto& lcg_report : msg.reported_lcgs) {
    // In the scheduler, we use 300000 to indicate the max (unbounded) S-BSR value; therefore, if the short or short
    // truncated BSRs report a value that is > 150000 and the current status of the lcg_id is > 150000, we can't
    // update buf_st, as the actual UE BSR (buf_st) could be much bigger than the max (unbounded) S-BSR reported
    // value.
    auto it = ue_ch.lcgs.find(lcg_report.lcg_id);
    if (it == ue_ch.lcgs.end()) {
      // LCG-ID not configured, ignore. Should we log this?
      continue;
    }
    soa::row_id   lcg_rid = it->second;
    units::bytes& buf_st  = lc_mapper.ul_buf_st(lcg_rid);

    if (msg.type == bsr_format::LONG_BSR or msg.type == bsr_format::LONG_TRUNC_BSR or
        lcg_report.nof_bytes <= max_short_bsr or buf_st.value() <= max_short_bsr) {
      auto prev_buf_st = buf_st.value();
      buf_st           = units::bytes{std::min(lcg_report.nof_bytes, max_buffer_status)};
      if (unsigned* sched_bytes_accum = lc_mapper.ul_sched_bytes_accum(lcg_rid); sched_bytes_accum != nullptr) {
        *sched_bytes_accum = 0;
      }
      on_single_lcg_buf_st_update(u, it->first, lc_mapper.ul_slice_id(lcg_rid), buf_st.value(), prev_buf_st);
    }
  }
}

lcid_t logical_channel_system::get_max_prio_lcid(const const_ue_row& u) const
{
  for (const auto lcid : u.at<ue_dl_channel_context>().sorted_channels) {
    auto lc_rid = lc_mapper.get_row_id(u.at<ue_config_context>().ue_index, lcid);
    if (lc_mapper.dl_buf_st(lc_rid).value() > 0) {
      return lcid;
    }
  }
  return INVALID_LCID;
}

unsigned
logical_channel_system::allocate_mac_sdu(soa::row_id ue_rid, dl_msg_lc_info& subpdu, lcid_t lcid, unsigned rem_bytes)
{
  ocudu_sanity_check(lcid < MAX_NOF_RB_LCIDS, "Max LCID value 32 exceeded");
  const unsigned min_bytes_needed = get_mac_sdu_with_subhdr_and_rlc_hdr_estim(lcid, 1);
  if (rem_bytes < min_bytes_needed) {
    // There is no space even for a minimal MAC SDU.
    return 0;
  }
  auto        u         = get_ue(ue_rid);
  auto&       ue_ch_ctx = u.at<ue_dl_channel_context>();
  soa::row_id lc_rid    = ue_ch_ctx.channels.at(lcid);

  unsigned lch_bytes_and_subhr = dl_pending_bytes(lc_rid);
  if (lch_bytes_and_subhr == 0) {
    return 0;
  }
  // This ensures that regardless of the pending bytes, enough space is allocated for the RLC overhead.
  lch_bytes_and_subhr = std::max(lch_bytes_and_subhr, min_bytes_needed);

  // Account for available space and MAC subheader to decide the number of bytes to allocate.
  unsigned alloc_bytes = std::min(rem_bytes, lch_bytes_and_subhr);

  // Allocate all leftover bytes in following cases:
  // - [Implementation-defined] If \c leftover_bytes is less than 5 bytes, as it is unlikely they will be used for
  // another SDU.
  const unsigned            leftover_bytes     = rem_bytes - alloc_bytes;
  static constexpr unsigned MIN_LEFTOVER_BYTES = 5U;
  if (leftover_bytes > 0 and (leftover_bytes <= MIN_LEFTOVER_BYTES)) {
    alloc_bytes += leftover_bytes;
  }
  if (alloc_bytes == MAC_SDU_SUBHEADER_LENGTH_THRES + MIN_MAC_SDU_SUBHEADER_SIZE) {
    // Avoid invalid combination of MAC subPDU and subheader size.
    alloc_bytes--;
  }

  // Compute MAC SDU size without MAC subheader.
  const unsigned sdu_size = get_mac_sdu_size(alloc_bytes);

  // Update DL Buffer Status to avoid reallocating the same LCID bytes.
  auto&          buf_st           = lc_mapper.dl_buf_st(lc_rid);
  const unsigned last_sched_bytes = std::min(sdu_size, buf_st.value());
  auto           new_buf_st       = buf_st.value() - last_sched_bytes;
  if (lcid != LCID_SRB0 and new_buf_st > 0) {
    // Allocation was not enough to empty the logical channel. In this specific case, we add some bytes to account
    // for the RLC segmentation overhead.
    // Note: This update is only relevant for PDSCH allocations for slots > slot_tx. For the case of PDSCH
    // slot==slot_tx, there will be an RLC Buffer Occupancy update right away, which will set a new buffer value.
    new_buf_st += RLC_SEGMENTATION_OVERHEAD;
  }
  auto prev_buf_st = buf_st.value();
  buf_st           = units::bytes{new_buf_st};
  on_single_channel_buf_st_update(u, lc_mapper.dl_slice_id(lc_rid), new_buf_st, prev_buf_st);
  if (unsigned* last_sched_ptr = lc_mapper.last_dl_sched_bytes(lc_rid); last_sched_ptr != nullptr) {
    // In case of QoS tracking is activated, set the last scheduled bytes.
    *last_sched_ptr = last_sched_bytes;
  }

  // Set subPDU info.
  subpdu.lcid        = lcid;
  subpdu.sched_bytes = sdu_size;

  return alloc_bytes;
}

unsigned
logical_channel_system::allocate_mac_sdu(soa::row_id ue_rid, dl_msg_lc_info& subpdu, unsigned rem_bytes, lcid_t lcid)
{
  subpdu.lcid        = lcid_dl_sch_t::INVALID;
  subpdu.sched_bytes = 0;

  lcid_t lcid_with_prio = lcid == INVALID_LCID ? get_max_prio_lcid(get_ue(ue_rid)) : lcid;
  if (lcid_with_prio == INVALID_LCID) {
    return 0;
  }

  // Update Buffer Status of allocated LCID.
  return allocate_mac_sdu(ue_rid, subpdu, lcid_with_prio, rem_bytes);
}

unsigned logical_channel_system::allocate_mac_ce(soa::row_id ue_rid, dl_msg_lc_info& subpdu, unsigned rem_bytes)
{
  subpdu.lcid        = lcid_dl_sch_t::INVALID;
  subpdu.sched_bytes = 0;

  // Allocate ConRes CE ID if pending.
  unsigned alloc_bytes = allocate_ue_con_res_id_mac_ce(ue_rid, subpdu, rem_bytes);
  if (alloc_bytes > 0) {
    return alloc_bytes;
  }

  auto  u         = get_ue(ue_rid);
  auto& ue_ch_ctx = u.at<ue_dl_channel_context>();
  auto  ce_list   = ue_ch_ctx.pending_ces.get_list(pending_ces);
  if (ce_list.empty()) {
    return 0;
  }
  auto                it   = ce_list.begin();
  const lcid_dl_sch_t lcid = it->info.ce_lcid;

  // Derive space needed for CE subheader + payload.
  const unsigned ce_size = lcid.sizeof_ce();
  if (lcid.is_var_len_ce()) {
    alloc_bytes = get_mac_sdu_required_bytes(ce_size);
  } else {
    alloc_bytes = ce_size + get_mac_ce_subheader_size(lcid);
  }

  // Verify there is space for both MAC CE and subheader.
  if (rem_bytes < alloc_bytes) {
    return 0;
  }

  subpdu.lcid        = lcid;
  subpdu.sched_bytes = ce_size;
  subpdu.ce_payload  = it->info.ce_payload;

  // Pop MAC CE.
  stable_id_t ce_row_id = ce_list.pop_front();
  // Update sum of pending CE bytes.
  u.at<ue_context>().pending_ce_bytes -= alloc_bytes;
  pending_ces.erase(ce_row_id);
  if (ce_list.empty()) {
    ues_with_pending_ces.set(u.at<ue_config_context>().ue_index, false);
  }

  return alloc_bytes;
}

unsigned
logical_channel_system::allocate_ue_con_res_id_mac_ce(soa::row_id ue_rid, dl_msg_lc_info& subpdu, unsigned rem_bytes)
{
  auto& ue_ctx = get_ue(ue_rid).at<ue_context>();
  if (not ue_ctx.pending_con_res_id) {
    return 0;
  }

  subpdu.lcid        = lcid_dl_sch_t::INVALID;
  subpdu.sched_bytes = 0;

  static constexpr auto ce_size     = lcid_dl_sch_t{lcid_dl_sch_t::UE_CON_RES_ID}.sizeof_ce();
  unsigned              alloc_bytes = ce_size + FIXED_SIZED_MAC_CE_SUBHEADER_SIZE;

  // Verify there is space for both MAC CE and subheader.
  if (rem_bytes < alloc_bytes) {
    alloc_bytes = 0;
  }

  if (alloc_bytes > 0) {
    ue_ctx.pending_con_res_id = false;

    subpdu.lcid        = lcid_dl_sch_t{lcid_dl_sch_t::UE_CON_RES_ID};
    subpdu.sched_bytes = ce_size;
  }

  return alloc_bytes;
}

// class logical_channel_system::ue_logical_channel_repository

void ue_logical_channel_repository::configure(logical_channel_config_list_ptr log_channels_configs)
{
  parent->configure(ue_row_id, log_channels_configs.has_value() ? log_channels_configs : empty_lc_cfg_list_ptr);
}

void ue_logical_channel_repository::deactivate()
{
  parent->deactivate(ue_row_id);
}

void ue_logical_channel_repository::set_lcid_ran_slice(lcid_t lcid, ran_slice_id_t slice_id)
{
  parent->set_lcid_ran_slice(ue_row_id, lcid, slice_id);
}

void ue_logical_channel_repository::set_lcg_ran_slice(lcg_id_t lcgid, ran_slice_id_t slice_id)
{
  parent->set_lcg_ran_slice(ue_row_id, lcgid, slice_id);
}

void ue_logical_channel_repository::reset_lcid_ran_slice(lcid_t lcid)
{
  parent->deregister_lc_ran_slice(ue_row_id, lcid);
}

void ue_logical_channel_repository::reset_lcg_ran_slice(lcg_id_t lcgid)
{
  parent->deregister_lcg_ran_slice(ue_row_id, lcgid);
}

unsigned ue_logical_channel_repository::total_dl_pending_bytes() const
{
  auto     u     = get_ue_row();
  unsigned bytes = pending_ce_bytes();
  for (const auto& [lcid, lc_rid] : u.at<logical_channel_system::ue_dl_channel_context>().channels) {
    bytes += parent->dl_pending_bytes(lc_rid);
  }
  return bytes;
}

unsigned ue_logical_channel_repository::dl_pending_bytes() const
{
  auto u = get_ue_row();
  if (u.at<ue_context>().fallback_state) {
    return pending_con_res_ce_bytes() + pending_bytes(LCID_SRB0) + pending_bytes(LCID_SRB1);
  }
  unsigned bytes = pending_ce_bytes();
  auto&    ue_ch = u.at<ue_dl_channel_context>();
  for (auto [lcid, lc_rid] : ue_ch.channels) {
    if (lcid != LCID_SRB0) {
      bytes += parent->dl_pending_bytes(lc_rid);
    }
  }
  return bytes;
}

unsigned ue_logical_channel_repository::ul_pending_bytes() const
{
  auto u = get_ue_row();
  if (u.at<ue_context>().fallback_state) {
    return pending_bytes(uint_to_lcg_id(0U));
  }
  unsigned bytes = 0;
  for (const auto& [lcgid, ul_ch_ctx] : u.at<ue_ul_channel_context>().lcgs) {
    bytes += pending_bytes(lcgid);
  }
  return bytes;
}

ue_logical_channel_repository::dl_transport_block_selection
ue_logical_channel_repository::select_dl_transport_block(ran_slice_id_t slice_id) const
{
  // Nothing restricts the block, so it serves the whole slice.
  if (not get_ue_row().at<ue_dl_channel_context>().restricted_channel_present) {
    return {{}, dl_pending_bytes(slice_id)};
  }

  const logical_channel_config_list& lc_cfgs = cfg();
  // Resolves the row of a channel of the slice, so that its slice and its buffer are read with one lookup.
  const auto slice_row = [&](lcid_t lcid) -> std::optional<soa::row_id> {
    if (not lc_cfgs.contains(lcid)) {
      return std::nullopt;
    }
    const std::optional<soa::row_id> lc_rid = parent->lc_mapper.find_row_id(ue_index, lcid);
    if (not lc_rid.has_value() or parent->lc_mapper.dl_slice_id(*lc_rid) != slice_id) {
      return std::nullopt;
    }
    return lc_rid;
  };

  dl_transport_block_selection selection{{}, dl_pending_bytes(slice_id)};
  bool                         serves_data = false;
  for (const lcid_t lcid : get_prioritized_logical_channels()) {
    const std::optional<soa::row_id> lc_rid = slice_row(lcid);
    // An idle channel neither fills the block nor closes a restriction of it.
    if (not lc_rid.has_value() or parent->lc_mapper.dl_buf_st(*lc_rid).value() == 0) {
      continue;
    }
    if (selection.restrictions.admits(*lc_cfgs[lcid])) {
      selection.restrictions.set(*lc_cfgs[lcid]);
      serves_data = true;
      continue;
    }
    const unsigned dropped = parent->dl_pending_bytes(*lc_rid);
    selection.pending_bytes -= std::min(selection.pending_bytes, dropped);
  }
  if (serves_data) {
    return selection;
  }

  // Nothing is buffered, so the block carries what the channel it would serve first asks for.
  for (const lcid_t lcid : get_prioritized_logical_channels()) {
    if (not slice_row(lcid).has_value()) {
      continue;
    }
    selection.restrictions.set(*lc_cfgs[lcid]);
    if (selection.restrictions.is_restricted()) {
      break;
    }
  }
  return selection;
}

double ue_logical_channel_repository::average_dl_bit_rate(lcid_t lcid) const
{
  if (auto rid = parent->lc_mapper.find_row_id(ue_index, lcid); rid.has_value()) {
    return parent->lc_mapper.avg_dl_bits_per_slot(*rid) * get_ue_row().at<ue_config_context>().slots_per_msec * 1000;
  }
  return 0.0;
}

double ue_logical_channel_repository::average_ul_bit_rate(lcg_id_t lcgid) const
{
  if (auto rid = parent->lc_mapper.find_row_id(ue_index, lcgid); rid.has_value()) {
    return parent->lc_mapper.avg_ul_bits_per_slot(*rid) * get_ue_row().at<ue_config_context>().slots_per_msec * 1000;
  }
  return 0.0;
}

void ue_logical_channel_repository::handle_dl_buffer_status_indication(lcid_t     lcid,
                                                                       unsigned   buffer_status,
                                                                       slot_point hol_toa)
{
  parent->handle_dl_buffer_status_indication(ue_row_id, lcid, buffer_status, hol_toa);
}

slot_point ue_logical_channel_repository_view::pending_sr_slot_rx() const
{
  return parent->pending_sr_slot_rx(ue_row_id);
}

void ue_logical_channel_repository_view::handle_mac_ce_indication(const mac_ce_info& ce)
{
  parent->handle_mac_ce_indication(ue_row_id, ce);
}

void ue_logical_channel_repository::handle_bsr_indication(const ul_bsr_indication_message& msg)
{
  parent->handle_bsr_indication(ue_row_id, msg);
}

void ue_logical_channel_repository::handle_sr_indication(slot_point uci_slot)
{
  if (not parent->ues_with_pending_sr.test(ue_index)) {
    // This is a transition from SR not pending to SR pending.
    parent->handle_sr_indication(ue_row_id, uci_slot);
    parent->ues_with_pending_sr.set(ue_index, true);
  }
}

unsigned ue_logical_channel_repository::allocate_mac_sdu(dl_msg_lc_info& lch_info, unsigned rem_bytes, lcid_t lcid)
{
  return parent->allocate_mac_sdu(ue_row_id, lch_info, rem_bytes, lcid);
}

unsigned ue_logical_channel_repository::allocate_mac_ce(dl_msg_lc_info& lch_info, unsigned rem_bytes)
{
  return parent->allocate_mac_ce(ue_row_id, lch_info, rem_bytes);
}

unsigned ue_logical_channel_repository::allocate_ue_con_res_id_mac_ce(dl_msg_lc_info& lch_info, unsigned rem_bytes)
{
  return parent->allocate_ue_con_res_id_mac_ce(ue_row_id, lch_info, rem_bytes);
}

void ue_logical_channel_repository::handle_ul_grant(units::bytes grant_size)
{
  // Reset any pending SR indication.
  reset_sr_indication();

  // Update estimates of logical channel bit rates.
  auto& lcgs = get_ue_row().at<ue_ul_channel_context>().lcgs;
  for (auto it = lcgs.begin(); it != lcgs.end() and grant_size.value() > 0; ++it) {
    const soa::row_id lcg_rid = it->second;
    if (auto qos_rid = parent->lc_mapper.ul_qos_row(lcg_rid); not qos_rid.has_value()) {
      // This LCG is not being tracked for QoS.
      continue;
    }
    units::bytes& buf_st = parent->lc_mapper.ul_buf_st(lcg_rid);
    if (buf_st.value() == 0) {
      // This LCG has no data to send.
      continue;
    }
    unsigned* accum_bytes = parent->lc_mapper.ul_sched_bytes_accum(lcg_rid);
    ocudu_sanity_check(accum_bytes != nullptr, "QoS tracking not properly initialized for LCG");
    if (buf_st.value() <= *accum_bytes) {
      // There is no new data to send in this LCG.
      continue;
    }

    // Update scheduled bytes for this LCG.
    const unsigned bytes_sched = std::min(buf_st.value() - *accum_bytes, grant_size.value());
    *parent->lc_mapper.last_ul_sched_bytes(lcg_rid) += bytes_sched;
    *accum_bytes += bytes_sched;
    grant_size -= units::bytes{bytes_sched};
  }
}

void ue_logical_channel_repository::reset_sr_indication()
{
  if (parent->ues_with_pending_sr.test(ue_index)) {
    // This is a transition from SR pending to not pending.
    parent->reset_sr_indication(ue_row_id);
    parent->ues_with_pending_sr.set(ue_index, false);
  }
}

unsigned ocudu::allocate_mac_sdus(dl_msg_tb_info&                tb_info,
                                  ue_logical_channel_repository& lch_mng,
                                  units::bytes                   total_tbs,
                                  lcid_t                         lcid)
{
  // Minimum bytes needed for one MAC SDU: MAC subheader + 1 byte payload + RLC header (0 for SRB0 TM).
  const unsigned min_mac_sdu_space = get_mac_sdu_with_subhdr_and_rlc_hdr_estim(lcid, 1);
  unsigned       rem_tbs           = total_tbs.value();

  // If we do not have enough bytes to fit MAC subheader, skip MAC SDU allocation.
  // Note: We assume upper layer accounts for its own subheaders when updating the buffer state.
  while (rem_tbs >= min_mac_sdu_space and not tb_info.lc_chs_to_sched.full()) {
    dl_msg_lc_info subpdu;
    unsigned       alloc_bytes = lch_mng.allocate_mac_sdu(subpdu, rem_tbs, lcid);
    if (alloc_bytes == 0) {
      break;
    }

    // Add new subPDU.
    tb_info.lc_chs_to_sched.push_back(subpdu);

    // Update remaining space taking into account the MAC SDU subheader.
    rem_tbs -= alloc_bytes;
  }

  return total_tbs.value() - rem_tbs;
}

unsigned
ocudu::allocate_mac_ces(dl_msg_tb_info& tb_info, ue_logical_channel_repository& lch_mng, units::bytes total_tbs)
{
  unsigned rem_tbs = total_tbs.value();

  while (lch_mng.has_pending_ces() and not tb_info.lc_chs_to_sched.full()) {
    dl_msg_lc_info subpdu;
    unsigned       alloc_bytes = lch_mng.allocate_mac_ce(subpdu, rem_tbs);
    if (alloc_bytes == 0) {
      break;
    }

    // Add new subPDU.
    tb_info.lc_chs_to_sched.push_back(subpdu);

    // Update remaining space taking into account the MAC CE subheader.
    rem_tbs -= alloc_bytes;
  }
  return total_tbs.value() - rem_tbs;
}

unsigned ocudu::allocate_ue_con_res_id_mac_ce(dl_msg_tb_info&                tb_info,
                                              ue_logical_channel_repository& lch_mng,
                                              units::bytes                   total_tbs)
{
  unsigned rem_tbs = total_tbs.value();

  if (not tb_info.lc_chs_to_sched.full()) {
    dl_msg_lc_info subpdu;
    unsigned       alloc_bytes = lch_mng.allocate_ue_con_res_id_mac_ce(subpdu, rem_tbs);
    if (alloc_bytes != 0) {
      // Add new subPDU.
      tb_info.lc_chs_to_sched.push_back(subpdu);

      // Update remaining space taking into account the MAC CE subheader.
      rem_tbs -= alloc_bytes;
    }
  }
  return total_tbs.value() - rem_tbs;
}

unsigned ocudu::build_dl_fallback_transport_block_info(dl_msg_tb_info&                tb_info,
                                                       ue_logical_channel_repository& lch_mng,
                                                       units::bytes                   tb_size_bytes)
{
  unsigned total_subpdu_bytes = 0;
  total_subpdu_bytes += allocate_ue_con_res_id_mac_ce(tb_info, lch_mng, tb_size_bytes);
  // Since SRB0 PDU cannot be segmented, skip SRB0 if remaining TB size is not enough to fit entire PDU.
  if (lch_mng.has_pending_bytes(LCID_SRB0) and
      ((tb_size_bytes.value() - total_subpdu_bytes) >= lch_mng.pending_bytes(LCID_SRB0))) {
    total_subpdu_bytes +=
        allocate_mac_sdus(tb_info, lch_mng, units::bytes{tb_size_bytes.value() - total_subpdu_bytes}, LCID_SRB0);
    return total_subpdu_bytes;
  }
  total_subpdu_bytes +=
      allocate_mac_sdus(tb_info, lch_mng, units::bytes{tb_size_bytes.value() - total_subpdu_bytes}, LCID_SRB1);
  return total_subpdu_bytes;
}

unsigned ocudu::build_dl_transport_block_info(dl_msg_tb_info&                 tb_info,
                                              ue_logical_channel_repository&  lch_mng,
                                              units::bytes                    tb_size_bytes,
                                              ran_slice_id_t                  slice_id,
                                              dl_transport_block_restrictions restrictions)
{
  // A channel the block does not accept is left for another block.
  const logical_channel_config_list& lc_cfgs = lch_mng.cfg();

  unsigned total_subpdu_bytes = 0;
  total_subpdu_bytes += allocate_mac_ces(tb_info, lch_mng, tb_size_bytes);
  for (const auto lcid : lch_mng.get_prioritized_logical_channels()) {
    // An idle channel neither fills the block nor closes a restriction of it.
    if (lch_mng.get_slice_id(lcid) != slice_id or not lch_mng.has_pending_bytes(lcid)) {
      continue;
    }
    if (lc_cfgs.contains(lcid)) {
      if (not restrictions.admits(*lc_cfgs[lcid])) {
        continue;
      }
      restrictions.set(*lc_cfgs[lcid]);
    }
    total_subpdu_bytes +=
        allocate_mac_sdus(tb_info, lch_mng, units::bytes{tb_size_bytes.value() - total_subpdu_bytes}, lcid);
  }
  return total_subpdu_bytes;
}
