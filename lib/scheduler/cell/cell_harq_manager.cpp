// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cell_harq_manager.h"
#include "ocudu/scheduler/resource_grid_util.h"
#include "ocudu/scheduler/result/pdsch_info.h"
#include "ocudu/scheduler/result/pusch_info.h"
#include "ocudu/support/format/fmt_to_c_str.h"
#include <algorithm>

using namespace ocudu;
using namespace harq_utils;

namespace {

class noop_harq_timeout_notifier : public harq_timeout_notifier
{
public:
  void on_feedback_timeout(du_ue_index_t ue_idx, bool is_dl, bool ack) override
  { // Do nothing.
  }
  void on_retx_timeout(du_ue_index_t ue_idx, bool is_dl) override
  { // Do nothing.
  }
  void on_feedback_disabled_harq_timeout(du_ue_index_t ue_idx, bool is_dl, units::bytes tbs) override
  { // Do nothing.
  }
};

template <bool IsDl>
class base_ntn_harq_history
{
  using harq_impl_type = std::conditional_t<IsDl, dl_harq_process_impl, ul_harq_process_impl>;

public:
  base_ntn_harq_history(cell_harq_repository<IsDl>& parent_, unsigned ntn_ul_alloc_delay_slots_) :
    harq_pool(parent_), ntn_ul_alloc_delay_slots(ntn_ul_alloc_delay_slots_)
  {
    ocudu_assert(ntn_ul_alloc_delay_slots > 0, "NTN HARQ history requires a non-zero UL allocation delay");
    unsigned ring_size = get_allocator_ring_size_gt_min(ntn_ul_alloc_delay_slots * 2 + SCHEDULER_MAX_K1);
    history.resize(ring_size);
  }

  void slot_indication(slot_point sl_tx)
  {
    last_sl_ind = sl_tx;
    // If there are no allocations for a long time, we may need to reset last_sl_ack.
    if (last_sl_ack.valid() and last_sl_ack < sl_tx - ntn_ul_alloc_delay_slots - SCHEDULER_MAX_K1) {
      last_sl_ack.clear();
    }

    // Clear old entries.
    unsigned idx = get_current_index(sl_tx - ntn_ul_alloc_delay_slots - SCHEDULER_MAX_K1);
    history[idx].clear();
  }

  void save_harq_newtx_info(const harq_impl_type& h)
  {
    unsigned idx = get_current_index(h.slot_ack);
    history[idx].emplace_back(h);

    if (not last_sl_ack.valid() or h.slot_ack > last_sl_ack) {
      last_sl_ack = h.slot_ack;
    }
  }

  void handle_ack(harq_impl_type& h)
  {
    auto& hist_entry = history[get_current_index(h.slot_ack)];
    if (&h < hist_entry.data() or &h >= hist_entry.data() + hist_entry.size()) {
      // Check if the HARQ Process being ACKed is the same as the one managed by the NTN history.
      ocudu_assertion_failure("ACK Info handled for a non-NTN HARQ process");
      ocudulog::fetch_basic_logger("SCHED").error("ACK Info handled for a non-NTN HARQ process");
      return;
    }
    h.status = harq_state_t::empty;
  }

protected:
  unsigned get_current_index(slot_point sl) const { return mod(sl.count()); }
  unsigned mod(unsigned idx) const { return idx % history.size(); }

  cell_harq_repository<IsDl>&              harq_pool;
  unsigned                                 ntn_ul_alloc_delay_slots;
  std::vector<std::vector<harq_impl_type>> history;

  slot_point last_sl_ind;
  slot_point last_sl_ack;
};

} // namespace

namespace ocudu::harq_utils {

/// Handles the DL HARQ information in the case NTN mode.
class ntn_dl_harq_alloc_history : public base_ntn_harq_history<true>
{
public:
  using base_ntn_harq_history<true>::base_ntn_harq_history;

  std::optional<dl_harq_process_handle> find_dl_harq(du_ue_index_t ue_idx, slot_point uci_slot, unsigned harq_bit_idx)
  {
    unsigned idx = get_current_index(uci_slot);
    for (dl_harq_process_impl& h_impl : history[idx]) {
      if (h_impl.ue_idx == ue_idx and h_impl.status == harq_state_t::waiting_ack and h_impl.slot_ack == uci_slot and
          h_impl.harq_bit_idx == harq_bit_idx) {
        return dl_harq_process_handle{harq_pool, h_impl};
      }
    }
    return std::nullopt;
  }
};

/// Handles the UL HARQ information in the case NTN mode.
class ntn_ul_harq_alloc_history : public base_ntn_harq_history<false>
{
public:
  using base_ntn_harq_history<false>::base_ntn_harq_history;

  std::optional<ul_harq_process_handle> find_ul_harq(du_ue_index_t ue_idx, slot_point pusch_slot)
  {
    unsigned idx = get_current_index(pusch_slot);
    for (ul_harq_process_impl& h_impl : history[idx]) {
      if (h_impl.ue_idx == ue_idx and h_impl.status == harq_state_t::waiting_ack and h_impl.slot_ack == pusch_slot) {
        return ul_harq_process_handle{harq_pool, h_impl};
      }
    }
    return std::nullopt;
  }

  units::bytes sum_pending_ul_tbs(du_ue_index_t ue_idx, std::optional<ran_slice_id_t> slice_id) const
  {
    if (not last_sl_ack.valid() or last_sl_ack < last_sl_ind) {
      return units::bytes{0};
    }
    units::bytes sum{0};
    unsigned     end_idx = get_current_index(last_sl_ack + 1);
    for (unsigned idx = get_current_index(last_sl_ind); idx != end_idx; idx = mod(idx + 1)) {
      for (const ul_harq_process_impl& h : history[idx]) {
        if (h.ue_idx != ue_idx or h.status == harq_state_t::empty) {
          continue;
        }
        if (not slice_id.has_value() or not h.prev_tx_params.slice_id.has_value() or
            h.prev_tx_params.slice_id == slice_id) {
          sum += h.prev_tx_params.tbs;
        }
        // A UE is granted at most one PUSCH per slot.
        break;
      }
    }
    return sum;
  }
};

} // namespace ocudu::harq_utils

// [Implementation-defined] Slots a HARQ process owing no report waits before it is released.
static constexpr unsigned NTN_ACK_WAIT_TIMEOUT = 4;

/// Upper bound for the number of slots a HARQ can spend in pending reTx state, before it is considered zombie/stale.
static constexpr unsigned MAX_RETX_TIMEOUT = NOF_SFNS / 2;

template <bool IsDl>
cell_harq_repository<IsDl>::cell_harq_repository(unsigned                max_nof_ue_indexes,
                                                 unsigned                max_nof_ue_contexts,
                                                 unsigned                max_ack_wait_timeout,
                                                 unsigned                harq_retx_timeout_,
                                                 unsigned                max_harqs_per_ue_,
                                                 unsigned                ntn_ul_alloc_delay_slots_,
                                                 bool                    harq_mode_b_,
                                                 harq_timeout_notifier&  timeout_notifier_,
                                                 ocudulog::basic_logger& logger_) :
  max_ack_wait_in_slots(max_ack_wait_timeout),
  harq_retx_timeout(std::min(harq_retx_timeout_, MAX_RETX_TIMEOUT)),
  max_harqs_per_ue(max_harqs_per_ue_),
  timeout_notifier(timeout_notifier_),
  logger(logger_),
  // Reserve space in advance for the HARQs of each entity, so that no allocation is needed to add a UE.
  ue_entity_pool(max_nof_ue_contexts,
                 [max_harqs_per_ue_](ue_harq_entity_impl& u) { u.harqs.reserve(max_harqs_per_ue_); }),
  ntn_ul_alloc_delay_slots(ntn_ul_alloc_delay_slots_),
  alloc_hist(ntn_ul_alloc_delay_slots_ > 0 and harq_mode_b_
                 ? std::make_unique<harq_alloc_history>(*this, ntn_ul_alloc_delay_slots_)
                 : nullptr)
{
  ues.resize(max_nof_ue_indexes);

  harq_timeout_wheel.resize(
      get_allocator_ring_size_gt_min(max_ack_wait_timeout + get_max_slot_ul_alloc_delay(ntn_ul_alloc_delay_slots_)));
}

template <bool IsDl>
cell_harq_repository<IsDl>::~cell_harq_repository()
{
}

template <bool IsDl>
void cell_harq_repository<IsDl>::slot_indication(slot_point sl_tx)
{
  last_sl_ind = sl_tx;

  if (is_ntn_harq_mode_b_enabled()) {
    alloc_hist->slot_indication(sl_tx);
  }

  // Handle HARQs that timed out.
  auto& harqs_timing_out = harq_timeout_wheel[sl_tx.count() % harq_timeout_wheel.size()];
  while (not harqs_timing_out.empty()) {
    handle_harq_ack_timeout(harqs_timing_out.front(), sl_tx);
  }
}

template <bool IsDl>
void cell_harq_repository<IsDl>::stop()
{
  for (auto& u : ues) {
    if (u == nullptr) {
      continue;
    }
    for (auto& h : u->harqs) {
      if (h.status != harq_state_t::empty) {
        dealloc_harq(h);
      }
    }
  }
  last_sl_ind = {};
}

template <bool IsDl>
void cell_harq_repository<IsDl>::handle_harq_ack_timeout(harq_type& h, slot_point sl_tx)
{
  ocudu_sanity_check(h.status == harq_state_t::waiting_ack or h.status == harq_state_t::pending_retx,
                     "HARQ process in wrong state");

  if (h.mode == harq_mode_t::feedback_disabled_or_mode_b) {
    if (IsDl) {
      // No feedback from UE is expected, we need to inject ACK so the metrics are displayed correctly.
      timeout_notifier.on_feedback_disabled_harq_timeout(h.ue_idx, IsDl, h.prev_tx_params.tbs);
    }
    // Deallocate HARQ so it can be reused without waiting for feedback.
    dealloc_harq(h);
    return;
  }

  if (h.status == harq_state_t::pending_retx) {
    // This HARQ is being starved by the scheduler.
    logger.info("rnti={} h_id={}: Discarding {} HARQ. Cause: The scheduler took too long to reschedule this HARQ "
                "process ({} slots elapsed since last NOK).",
                h.rnti,
                h.h_id,
                IsDl ? std::string_view{"DL"} : std::string_view{"UL"},
                h.slot_timeout - h.slot_ack);

    // Report the retx timeout.
    timeout_notifier.on_retx_timeout(h.ue_idx, IsDl);
  } else {
    // The UCI outcome was never propagated to the HARQ.
    logger.warning("rnti={} h_id={}: Discarding {} HARQ. Cause: Timeout was reached ({} slots) to receive the "
                   "respective HARQ-ACK indication from lower layers (HARQ-ACK slot={})",
                   h.rnti,
                   h.h_id,
                   IsDl ? std::string_view{"DL"} : std::string_view{"UL"},
                   h.slot_timeout - h.slot_ack_end,
                   h.slot_ack);

    // Report timeout with NACK.
    timeout_notifier.on_feedback_timeout(h.ue_idx, IsDl, false);
  }

  // Deallocate HARQ.
  dealloc_harq(h);
}

/// Returns the processes of the entity that are free and operate in the given mode.
template <typename UeHarqEntity>
static fixed_size_bitset<MAX_NOF_HARQS> free_harq_mask(const UeHarqEntity& ue, harq_mode_t mode)
{
  return mode == harq_mode_t::normal ? ue.free_harqs & ue.harq_mode_mask : ue.free_harqs & ~ue.harq_mode_mask;
}

template <bool IsDl>
typename cell_harq_repository<IsDl>::harq_type*
cell_harq_repository<IsDl>::alloc_harq(du_ue_index_t                       ue_idx,
                                       slot_point                          sl_pdcch,
                                       slot_point                          sl_tx,
                                       slot_point                          sl_ack,
                                       slot_point                          sl_ack_end,
                                       unsigned                            max_nof_harq_retxs,
                                       std::optional<cg_harq_alloc_params> cg_params,
                                       harq_mode_t                         mode,
                                       uint8_t                             nof_repetitions)
{
  ocudu_sanity_check(ue_idx < ues.size() and ues[ue_idx] != nullptr, "Invalid UE index");
  ue_harq_entity_impl& ue_harq_entity = *ues[ue_idx];
  if (ue_harq_entity.free_harqs.none()) {
    return nullptr;
  }

  if constexpr (IsDl) {
    ocudu_assert(not cg_params.has_value(), "HARQ-ID reservation is only supported for UL");
  }
  ocudu_assert(not((cg_params.has_value() or ue_harq_entity.first_non_reserved_harq_id != 0) and
                   ue_harq_entity.feedback_disabled_or_mode_b_harq_present),
               "Reserved CG HARQ processes not supported with mode B or disabled feedback");
  // This is for Configured Grant.
  ocudu_assert(not cg_params.has_value() or cg_params->cg_harq_timeout != 0,
               "If a CG HARQ is requested, the HARQ timeout needs to be non-zero");

  // Allocation of free HARQ-id for the UE. The lowest free process is taken, and the cases below differ only in how
  // they narrow the set it is taken from.
  harq_id_t h_id = INVALID_HARQ_ID;
  if (cg_params.has_value()) {
    // A Configured Grant rides the process its configuration names, which the UE holds to, so the request for it
    // comes before any narrowing by mode.
    // TODO: review this once feedback disabled / mode B HARQ will be compatible with reserved HARQ for CG.
    const harq_id_t cg_h_id = cg_params->harq_id;
    if (cg_h_id >= ue_harq_entity.first_non_reserved_harq_id) {
      logger.warning("Requested HARQ-ID from non reserved set");
      return nullptr;
    }
    if (not ue_harq_entity.free_harqs.test(cg_h_id)) {
      // With a CG-specific HARQ timeout (configured_grant_timer × periodicity), the HARQ should have been
      // freed by the timeout wheel before the next CG occasion. If this fires, the timeout is misconfigured.
      logger.warning("rnti={} h_id={}: CG HARQ forced reuse — timeout did not release the process in time",
                     ue_harq_entity.harqs[cg_h_id].rnti,
                     cg_h_id);
      dealloc_harq(ue_harq_entity.harqs[cg_h_id]);
    }
    h_id = cg_h_id;
  } else if (ntn_ul_alloc_delay_slots > 0 && ue_harq_entity.feedback_disabled_or_mode_b_harq_present) {
    // The set is narrowed by the mode and, as in the branch below, by the processes held for Configured Grant use.
    const int lowest = free_harq_mask(ue_harq_entity, mode)
                           .find_lowest(ue_harq_entity.first_non_reserved_harq_id, ue_harq_entity.free_harqs.size());
    if (lowest < 0) {
      return nullptr;
    }
    h_id = to_harq_id(lowest);
  } else {
    // The processes below the first non-reserved one are held for Configured Grant use.
    const int lowest = ue_harq_entity.free_harqs.find_lowest(ue_harq_entity.first_non_reserved_harq_id,
                                                             ue_harq_entity.free_harqs.size());
    if (lowest < 0) {
      return nullptr;
    }
    h_id = to_harq_id(lowest);
  }
  ue_harq_entity.free_harqs.reset(h_id);
  harq_type& h = ue_harq_entity.harqs[h_id];
  ocudu_sanity_check(h.ue_idx == ue_idx and h.h_id == h_id, "Invalid state of HARQ");

  // Set allocated HARQ common params.
  h.status             = harq_state_t::waiting_ack;
  h.slot_tx            = sl_tx;
  h.last_occasion_slot = sl_tx + (nof_repetitions - 1U);
  h.slot_ack           = sl_ack;
  h.slot_ack_end       = sl_ack_end;
  h.nof_retxs          = 0;
  h.ndi                = !h.ndi;
  h.max_nof_harq_retxs = max_nof_harq_retxs;
  h.retxs_cancelled    = false;

  // Set UE HARQ entity common params. last_slot_tx covers the whole transmission (all repetition occasions, not just
  // sl_tx), so that ordering checks relying on it (ue_cell::is_pdsch_enabled, the fallback scheduler) know that the
  // remaining occasions still occupy the following slots.
  ue_harq_entity.last_slot_tx = ue_harq_entity.last_slot_tx.valid()
                                    ? std::max(ue_harq_entity.last_slot_tx, h.last_occasion_slot)
                                    : h.last_occasion_slot;
  ue_harq_entity.last_slot_ack =
      ue_harq_entity.last_slot_ack.valid() ? std::max(ue_harq_entity.last_slot_ack, sl_ack) : sl_ack;

  // Add HARQ to the timeout list. The wait starts at the last transmission that can carry the feedback.
  h.slot_timeout = sl_ack_end + max_ack_wait_in_slots;

  // CG-specific timeout: configured_grant_timer × periodicity slots. This ensures the HARQ is freed by the timeout
  // wheel before the next CG occasion for the same HARQ ID.
  if (cg_params.has_value()) {
    h.slot_timeout = sl_ack + cg_params->cg_harq_timeout;
  }

  // If HARQ mode B or DL HARQ Feedback disabled, set short timeout to release and reuse process quickly.
  if (h.mode == harq_mode_t::feedback_disabled_or_mode_b) {
    if constexpr (IsDl) {
      h.slot_timeout = h.last_occasion_slot + NTN_ACK_WAIT_TIMEOUT;
    } else {
      h.slot_timeout = sl_pdcch + NTN_ACK_WAIT_TIMEOUT;
    }
  }

  harq_timeout_wheel[h.slot_timeout.count() % harq_timeout_wheel.size()].push_front(&h);

  return &h;
}

template <bool IsDl>
void cell_harq_repository<IsDl>::dealloc_harq(harq_type& h)
{
  if (h.status == harq_state_t::empty) {
    // No-op
    return;
  }

  // Update HARQ process state.
  if (h.status == harq_state_t::pending_retx) {
    // Remove the HARQ from the pending Retx list.
    harq_pending_retx_list.pop(&h);
  }
  // Remove HARQ from the timeout list.
  harq_timeout_wheel[h.slot_timeout.count() % harq_timeout_wheel.size()].pop(&h);
  h.slot_timeout = {};
  h.status       = harq_state_t::empty;

  // Check if common HARQ entity params need to be updated.
  ue_harq_entity_impl& ue_harq_entity = *ues[h.ue_idx];
  if (ue_harq_entity.last_slot_tx.valid() and h.last_occasion_slot >= ue_harq_entity.last_slot_tx) {
    // If the HARQ being reset corresponds to the last recorded Tx, we also reset "last_slot_tx". This avoids
    // encountering ambiguities with the slot wrap-around, when the UE stays for very long without being scheduled.
    ue_harq_entity.last_slot_tx = {};
  }
  if (ue_harq_entity.last_slot_ack.valid() and h.slot_ack >= ue_harq_entity.last_slot_ack) {
    // We do the same with last_slot_ack as we did with last_slot_tx.
    ue_harq_entity.last_slot_ack = {};
  }

  // Mark HARQ as available again.
  ue_harq_entity.free_harqs.set(h.h_id);
}

template <bool IsDl>
void cell_harq_repository<IsDl>::handle_ack(harq_type& h, bool ack)
{
  if (not ack and h.nof_retxs >= h.max_nof_harq_retxs) {
    if (h.retxs_cancelled) {
      logger.debug("rnti={} h_id={}: Discarding {} HARQ process TB with tbs={}. Cause: Retxs for this HARQ process "
                   "were cancelled",
                   h.rnti,
                   h.h_id,
                   IsDl ? std::string_view{"DL"} : std::string_view{"UL"},
                   h.prev_tx_params.tbs);
    } else if (h.max_nof_harq_retxs != 0) {
      logger.info(
          "rnti={} h_id={}: Discarding {} HARQ process TB with tbs={}. Cause: Maximum number of reTxs {} exceeded",
          h.rnti,
          h.h_id,
          IsDl ? std::string_view{"DL"} : std::string_view{"UL"},
          h.prev_tx_params.tbs,
          h.max_nof_harq_retxs);
    }
  }

  if (h.mode == harq_mode_t::feedback_disabled_or_mode_b) {
    // In NTN mode B, ACK info does not affect the timeout/retx containers.
    alloc_hist->handle_ack(h);
    return;
  }

  if (ack or h.nof_retxs >= h.max_nof_harq_retxs) {
    // If the HARQ process is ACKed or the maximum number of retransmissions has been reached, we can deallocate the
    // HARQ process.
    dealloc_harq(h);
  } else {
    set_pending_retx(h);
  }
}

template <bool IsDl>
void cell_harq_repository<IsDl>::set_pending_retx(harq_type& h)
{
  ocudu_sanity_check(h.status != harq_state_t::empty, "HARQ process in wrong state");
  if (h.status == harq_state_t::pending_retx) {
    // No-op
    return;
  }

  // Remove the HARQ from the ACK timeout list and re-add it with a new timeout.
  harq_timeout_wheel[h.slot_timeout.count() % harq_timeout_wheel.size()].pop(&h);
  h.slot_timeout = last_sl_ind + harq_retx_timeout;
  harq_timeout_wheel[h.slot_timeout.count() % harq_timeout_wheel.size()].push_front(&h);

  // Add HARQ to pending Retx list.
  harq_pending_retx_list.push_back(&h);

  // Update HARQ process state.
  h.status = harq_state_t::pending_retx;
}

template <bool IsDl>
bool cell_harq_repository<IsDl>::handle_new_retx(harq_type& h,
                                                 slot_point sl_tx,
                                                 slot_point sl_ack,
                                                 slot_point sl_ack_end,
                                                 uint8_t    nof_repetitions)
{
  if (h.status != harq_state_t::pending_retx) {
    logger.warning("rnti={} h_id={}: Attempt of retx in a HARQ process that has no pending retx", h.rnti, h.h_id);
    return false;
  }

  // Remove HARQ from pending Retx list.
  harq_pending_retx_list.pop(&h);
  harq_timeout_wheel[h.slot_timeout.count() % harq_timeout_wheel.size()].pop(&h);
  h.slot_timeout = {};

  // Update HARQ common parameters.
  h.status             = harq_state_t::waiting_ack;
  h.slot_tx            = sl_tx;
  h.last_occasion_slot = sl_tx + (nof_repetitions - 1U);
  h.slot_ack           = sl_ack;
  h.slot_ack_end       = sl_ack_end;
  ++h.nof_retxs;

  // Set UE HARQ entity common params.
  ue_harq_entity_impl& ue_harq_entity = *ues[h.ue_idx];
  ue_harq_entity.last_slot_tx         = ue_harq_entity.last_slot_tx.valid()
                                            ? std::max(ue_harq_entity.last_slot_tx, h.last_occasion_slot)
                                            : h.last_occasion_slot;
  ue_harq_entity.last_slot_ack =
      ue_harq_entity.last_slot_ack.valid() ? std::max(ue_harq_entity.last_slot_ack, sl_ack) : sl_ack;

  // Add HARQ to the timeout list. The wait starts at the last transmission that can carry the feedback.
  h.slot_timeout = sl_ack_end + max_ack_wait_in_slots;
  harq_timeout_wheel[h.slot_timeout.count() % harq_timeout_wheel.size()].push_front(&h);

  return true;
}

template <bool IsDl>
void cell_harq_repository<IsDl>::reserve_ue_harqs(du_ue_index_t ue_idx, rnti_t rnti, unsigned nof_harqs)
{
  ues[ue_idx] = ue_entity_pool.get();
  report_fatal_error_if_not(ues[ue_idx] != nullptr,
                            "No HARQ entities left to add ue={}. The cell was dimensioned for {} UEs",
                            ue_idx,
                            ue_entity_pool.nof_objects());

  ue_harq_entity_impl& ue_harqs = *ues[ue_idx];
  ue_harqs.harqs.resize(nof_harqs);
  // Every process starts free and in the normal mode, which reconfigure() narrows afterwards.
  ue_harqs.free_harqs.reset();
  ue_harqs.free_harqs.fill(0, nof_harqs);
  ue_harqs.harq_mode_mask.reset();
  ue_harqs.harq_mode_mask.fill(0, nof_harqs);
  for (unsigned count = 0; count != nof_harqs; ++count) {
    harq_id_t h_id              = to_harq_id(count);
    ue_harqs.harqs[h_id].ue_idx = ue_idx;
    ue_harqs.harqs[h_id].rnti   = rnti;
    ue_harqs.harqs[h_id].h_id   = h_id;
    ue_harqs.harqs[h_id].ndi    = false;
    ue_harqs.harqs[h_id].mode   = harq_mode_t::normal;
  }
}

template <bool IsDl>
void cell_harq_repository<IsDl>::extend_ue_harqs(du_ue_index_t ue_idx, rnti_t rnti, unsigned new_nof_harqs)
{
  ocudu_sanity_check(ue_idx < ues.size() and ues[ue_idx] != nullptr, "Invalid UE index");
  auto&          ue_harqs          = *ues[ue_idx];
  const unsigned current_nof_harqs = ue_harqs.harqs.size();

  ocudu_assert(new_nof_harqs >= current_nof_harqs, "Cannot shrink nof HARQs");
  ocudu_assert(new_nof_harqs <= max_harqs_per_ue, "Cannot extend beyond max_harqs_per_ue={}", max_harqs_per_ue);

  // Extend HARQ vector.
  ue_harqs.harqs.resize(new_nof_harqs);

  // Initialize new HARQ processes and add to free list.
  for (unsigned count = current_nof_harqs; count != new_nof_harqs; ++count) {
    harq_id_t h_id              = to_harq_id(count);
    ue_harqs.harqs[h_id].ue_idx = ue_idx;
    ue_harqs.harqs[h_id].rnti   = rnti;
    ue_harqs.harqs[h_id].h_id   = h_id;
    ue_harqs.harqs[h_id].ndi    = false;
    ue_harqs.harqs[h_id].mode   = harq_mode_t::normal;
  }

  // The processes added above are free and in the normal mode, as reserve_ue_harqs() left the original ones.
  ue_harqs.free_harqs.fill(current_nof_harqs, new_nof_harqs);
  ue_harqs.harq_mode_mask.fill(current_nof_harqs, new_nof_harqs);

  logger.debug("ue={} rnti={}: Extended {} HARQs from {} to {}",
               ue_idx,
               rnti,
               IsDl ? "DL" : "UL",
               current_nof_harqs,
               new_nof_harqs);
}

template <bool IsDl>
void cell_harq_repository<IsDl>::destroy_ue(du_ue_index_t ue_idx)
{
  ocudu_assert(ues[ue_idx] != nullptr, "UE has no HARQ entity allocated");

  // Remove HARQ from list of pending retxs or timeout wheel.
  for (harq_type& h : ues[ue_idx]->harqs) {
    dealloc_harq(h);
  }
  // Note: The lists are cleared, and not destroyed (recycling pool), so that the entity keeps its memory for the next
  // UE.
  ues[ue_idx]->clear();
  ues[ue_idx].reset();
}

template <bool IsDl>
void cell_harq_repository<IsDl>::cancel_retxs(harq_type& h)
{
  if (h.status == harq_state_t::empty) {
    return;
  }
  if (h.status == harq_state_t::pending_retx) {
    // If a retx is pending, do not allow it to take place.
    dealloc_harq(h);
    return;
  }
  // If the HARQ is still waiting for an ACK to arrive, just mark it as max retxs have been exceeded. The ack_info
  // function will automatically update the HARQ state.
  h.max_nof_harq_retxs = h.nof_retxs;
  h.retxs_cancelled    = true;
}

template <bool IsDl>
void cell_harq_repository<IsDl>::cancel_unsent_tx(harq_type& h)
{
  if (h.status != harq_state_t::waiting_ack) {
    // Only a HARQ process that was just allocated for a (re)transmission can be cancelled. In particular, the NDI of
    // a released process must not be touched.
    logger.warning("rnti={} h_id={}: Attempt to cancel a HARQ transmission that was not allocated", h.rnti, h.h_id);
    return;
  }
  if (h.nof_retxs == 0) {
    // Undo the NDI toggle of alloc_harq(): the UE never saw it.
    h.ndi = !h.ndi;
  }
  // TODO: A cancelled reTx is dropped together with its TB. Undo handle_new_retx() instead (restore nof_retxs and
  // the pending_retx state), so that the reTx is retried in a later slot.
  dealloc_harq(h);
}

template <bool IsDl>
const typename cell_harq_repository<IsDl>::harq_type*
cell_harq_repository<IsDl>::find_ue_harq_in_state(du_ue_index_t ue_idx, harq_utils::harq_state_t state) const
{
  for (const harq_type& h : ues[ue_idx]->harqs) {
    if (h.status == state) {
      return &h;
    }
  }
  return nullptr;
}

template <bool IsDl>
bool cell_harq_repository<IsDl>::is_ntn_harq_mode_b_enabled() const
{
  return alloc_hist != nullptr;
}

template <bool IsDl>
typename cell_harq_repository<IsDl>::harq_type*
cell_harq_repository<IsDl>::find_ue_harq_in_state(du_ue_index_t ue_idx, harq_utils::harq_state_t state)
{
  for (harq_type& h : ues[ue_idx]->harqs) {
    if (h.status == state) {
      return &h;
    }
  }
  return nullptr;
}

template struct harq_utils::cell_harq_repository<true>;
template struct harq_utils::cell_harq_repository<false>;

template <bool IsDl>
void harq_utils::base_harq_process_handle<IsDl>::cancel_retxs()
{
  harq_repo->cancel_retxs(*impl);
}

template <bool IsDl>
void harq_utils::base_harq_process_handle<IsDl>::cancel_unsent_tx()
{
  harq_repo->cancel_unsent_tx(*impl);
}

template <bool IsDl>
void harq_utils::base_harq_process_handle<IsDl>::discard_sent_tx()
{
  harq_repo->dealloc_harq(*impl);
}

template class harq_utils::base_harq_process_handle<true>;
template class harq_utils::base_harq_process_handle<false>;

// Cell HARQ manager.

cell_harq_manager::cell_harq_manager(unsigned                               max_nof_ue_indexes,
                                     unsigned                               max_nof_ue_contexts,
                                     unsigned                               max_harqs_per_ue_,
                                     std::unique_ptr<harq_timeout_notifier> dl_notifier,
                                     std::unique_ptr<harq_timeout_notifier> ul_notifier,
                                     unsigned                               dl_harq_retx_timeout,
                                     unsigned                               ul_harq_retx_timeout,
                                     unsigned                               max_ack_wait_timeout,
                                     unsigned                               ntn_ul_alloc_delay_slots,
                                     bool                                   ul_harq_mode_b) :
  max_harqs_per_ue(max_harqs_per_ue_),
  dl_timeout_notifier(dl_notifier != nullptr ? std::move(dl_notifier) : std::make_unique<noop_harq_timeout_notifier>()),
  ul_timeout_notifier(not ul_harq_mode_b ? std::move(ul_notifier) : std::make_unique<noop_harq_timeout_notifier>()),
  logger(ocudulog::fetch_basic_logger("SCHED")),
  dl(max_nof_ue_indexes,
     max_nof_ue_contexts,
     max_ack_wait_timeout,
     dl_harq_retx_timeout,
     max_harqs_per_ue,
     ntn_ul_alloc_delay_slots,
     false,
     *dl_timeout_notifier,
     logger),
  ul(max_nof_ue_indexes,
     max_nof_ue_contexts,
     max_ack_wait_timeout,
     ul_harq_retx_timeout,
     max_harqs_per_ue,
     ntn_ul_alloc_delay_slots,
     ul_harq_mode_b,
     ul_timeout_notifier != nullptr ? *ul_timeout_notifier : *dl_timeout_notifier,
     logger)
{
}

void cell_harq_manager::slot_indication(slot_point sl_tx)
{
  dl.slot_indication(sl_tx);
  ul.slot_indication(sl_tx);
}

void cell_harq_manager::stop()
{
  dl.stop();
  ul.stop();
}

bool cell_harq_manager::contains(du_ue_index_t ue_idx) const
{
  return ue_idx < dl.ues.size() and dl.ues[ue_idx] != nullptr;
}

dl_harq_pending_retx_list cell_harq_manager::pending_dl_retxs()
{
  return dl_harq_pending_retx_list{dl};
}

ul_harq_pending_retx_list cell_harq_manager::pending_ul_retxs()
{
  return ul_harq_pending_retx_list{ul};
}

unique_ue_harq_entity
cell_harq_manager::add_ue(du_ue_index_t ue_idx, rnti_t crnti, unsigned nof_dl_harq_procs, unsigned nof_ul_harq_procs)
{
  ocudu_assert(nof_dl_harq_procs <= max_harqs_per_ue and nof_dl_harq_procs > 0, "Invalid number of DL HARQs");
  ocudu_assert(nof_ul_harq_procs <= max_harqs_per_ue and nof_ul_harq_procs > 0, "Invalid number of UL HARQs");
  ocudu_assert(ue_idx < dl.ues.size(), "Invalid ue_index");
  ocudu_assert(not contains(ue_idx), "Creating UE with duplicate ue_index");
  dl.reserve_ue_harqs(ue_idx, crnti, nof_dl_harq_procs);
  ul.reserve_ue_harqs(ue_idx, crnti, nof_ul_harq_procs);
  return {this, ue_idx, crnti};
}

void cell_harq_manager::destroy_ue(du_ue_index_t ue_idx)
{
  dl.destroy_ue(ue_idx);
  ul.destroy_ue(ue_idx);
}

harq_utils::dl_harq_process_impl* cell_harq_manager::new_dl_tx(du_ue_index_t ue_idx,
                                                               rnti_t        rnti,
                                                               slot_point    pdcch_slot,
                                                               slot_point    pdsch_slot,
                                                               unsigned      ack_delay,
                                                               unsigned      last_ack_delay,
                                                               unsigned      max_harq_nof_retxs,
                                                               uint8_t       harq_bit_idx,
                                                               harq_mode_t   mode,
                                                               uint8_t       nof_repetitions)
{
  dl_harq_process_impl* h = dl.alloc_harq(ue_idx,
                                          pdcch_slot,
                                          pdsch_slot,
                                          pdsch_slot + ack_delay,
                                          pdsch_slot + last_ack_delay,
                                          max_harq_nof_retxs,
                                          std::nullopt,
                                          mode,
                                          nof_repetitions);
  if (h == nullptr) {
    return nullptr;
  }

  // Save DL-specific parameters.
  h->prev_tx_params = {};
  h->harq_bit_idx   = harq_bit_idx;
  h->last_pucch_snr = std::nullopt;

  return h;
}

harq_utils::ul_harq_process_impl* cell_harq_manager::new_ul_tx(du_ue_index_t                       ue_idx,
                                                               rnti_t                              rnti,
                                                               slot_point                          pdcch_slot,
                                                               slot_point                          pusch_slot,
                                                               unsigned                            max_harq_nof_retxs,
                                                               std::optional<cg_harq_alloc_params> cg_params,
                                                               harq_mode_t                         mode,
                                                               uint8_t                             nof_repetitions)
{
  ul_harq_process_impl* h = ul.alloc_harq(
      ue_idx, pdcch_slot, pusch_slot, pusch_slot, pusch_slot, max_harq_nof_retxs, cg_params, mode, nof_repetitions);
  if (h == nullptr) {
    return nullptr;
  }

  // Save UL-specific parameters. Every occasion of this transmission contributes to combined_crc; start from
  // "not decoded".
  h->prev_tx_params       = {};
  h->prev_tx_params.is_cg = cg_params.has_value();
  h->combined_crc         = false;

  return h;
}

bool dl_harq_process_handle::new_retx(slot_point              pdsch_slot,
                                      unsigned                ack_delay,
                                      uint8_t                 harq_bit_idx,
                                      uint8_t                 nof_repetitions,
                                      std::optional<unsigned> last_ack_delay)
{
  if (not harq_repo->handle_new_retx(*impl,
                                     pdsch_slot,
                                     pdsch_slot + ack_delay,
                                     pdsch_slot + last_ack_delay.value_or(ack_delay),
                                     nof_repetitions)) {
    return false;
  }
  // Reset DL-only HARQ parameters.
  impl->harq_bit_idx   = harq_bit_idx;
  impl->last_pucch_snr = std::nullopt;
  return true;
}

bool dl_harq_process_handle::dl_ack_info(mac_harq_ack_report_status ack, const std::optional<float>& pucch_snr)
{
  if (impl->status != harq_state_t::waiting_ack) {
    // If the HARQ process is not expecting an HARQ-ACK, it means that it has already been ACKed/NACKed.
    harq_repo->logger.warning("rnti={} h_id={}: ACK arrived for inactive DL HARQ", impl->rnti, impl->h_id);
    return false;
  }

  if (impl->mode == harq_mode_t::feedback_disabled_or_mode_b) {
    harq_repo->logger.warning(
        "rnti={} h_id={}: ACK arrived for DL HARQ with feedback disabled", impl->rnti, impl->h_id);
    return false;
  }

  // Update HARQ state
  impl->last_pucch_snr = pucch_snr;
  const bool final_ack = ack == mac_harq_ack_report_status::ack;
  harq_repo->handle_ack(*impl, final_ack);

  return true;
}

void dl_harq_process_handle::save_grant_params(const dl_harq_alloc_context& ctx, const dl_msg_alloc& ue_pdsch)
{
  static constexpr size_t CW_INDEX = 0;

  const pdsch_information& pdsch = ue_pdsch.pdsch_cfg;
  ocudu_assert(pdsch.codewords.size() == 1, "Only one codeword supported");
  ocudu_sanity_check(pdsch.rnti == impl->rnti, "RNTI mismatch");
  ocudu_sanity_check(pdsch.harq_id == impl->h_id, "HARQ-id mismatch");
  ocudu_assert(impl->status == harq_utils::harq_state_t::waiting_ack,
               "Setting allocation parameters for DL HARQ process id={} in invalid state",
               id());

  const pdsch_codeword&               cw          = pdsch.codewords[CW_INDEX];
  dl_harq_process_impl::alloc_params& prev_params = impl->prev_tx_params;

  if (impl->nof_retxs == 0) {
    prev_params.tbs             = cw.tb_size_bytes;
    prev_params.dci_cfg_type    = ctx.dci_cfg_type;
    prev_params.nof_layers      = pdsch.nof_layers;
    prev_params.nof_repetitions = ctx.nof_repetitions;
    prev_params.olla_mcs        = ctx.olla_mcs;
    prev_params.slice_id        = ctx.slice_id;
    prev_params.cqi             = ctx.cqi.has_value() ? ctx.cqi.value() : cqi_value{1};
    prev_params.is_fallback     = ctx.is_fallback;
    prev_params.lc_sched_info.clear();
    for (const dl_msg_lc_info& lc : ue_pdsch.tb_list[CW_INDEX].lc_chs_to_sched) {
      prev_params.lc_sched_info.push_back({lc.lcid, units::bytes{lc.sched_bytes}});
    }
  } else {
    ocudu_assert(ctx.dci_cfg_type == prev_params.dci_cfg_type,
                 "DCI format and RNTI type cannot change during DL HARQ retxs");
    ocudu_assert(prev_params.tbs == cw.tb_size_bytes,
                 "TBS cannot change during DL HARQ retxs ({}!={}). Previous MCS={}, RBs={}. New MCS={}, RBs={}",
                 prev_params.tbs,
                 cw.tb_size_bytes,
                 prev_params.mcs,
                 prev_params.rbs,
                 cw.mcs_index,
                 pdsch.rbs);
    ocudu_assert(prev_params.nof_layers == pdsch.nof_layers, "Number of layers cannot change during HARQ retxs");
    ocudu_assert(prev_params.nof_repetitions == ctx.nof_repetitions,
                 "Number of PDSCH repetitions cannot change during HARQ retxs");
    ocudu_assert(prev_params.is_fallback == ctx.is_fallback, "Fallback state cannot change across DL HARQ retxs");
  }
  prev_params.mcs_table   = pdsch.mcs_table;
  prev_params.mcs         = cw.mcs_index;
  prev_params.rbs         = pdsch.rbs;
  prev_params.nof_symbols = pdsch.symbols.length();
}

bool ul_harq_process_handle::new_retx(slot_point pusch_slot, uint8_t nof_repetitions)
{
  if (not harq_repo->handle_new_retx(*impl, pusch_slot, pusch_slot, pusch_slot, nof_repetitions)) {
    return false;
  }
  // Reset UL-only HARQ parameters. Every occasion of this transmission contributes to combined_crc; start from
  // "not decoded".
  impl->combined_crc = false;
  return true;
}

expected<units::bytes> ul_harq_process_handle::ul_crc_info(bool ack)
{
  if (impl->status != harq_state_t::waiting_ack) {
    // HARQ is not expecting CRC info.
    harq_repo->logger.warning(
        "rnti={} h_id={}: Discarding CRC. Cause: UL HARQ process is not expecting any CRC", impl->rnti, impl->h_id);
    return make_unexpected(default_error_t{});
  }

  harq_repo->handle_ack(*impl, ack);

  return ack ? impl->prev_tx_params.tbs : units::bytes{0};
}

void ul_harq_process_handle::save_grant_params(const ul_harq_alloc_context& ctx, const pusch_information& pusch)
{
  ocudu_sanity_check(pusch.rnti == impl->rnti, "RNTI mismatch");
  ocudu_sanity_check(pusch.harq_id == impl->h_id, "HARQ-id mismatch");
  ocudu_assert(impl->status == harq_utils::harq_state_t::waiting_ack,
               "Setting allocation parameters for UL HARQ process id={} in invalid state",
               id());

  ul_harq_process_impl::alloc_params& prev_tx_params = impl->prev_tx_params;

  if (impl->nof_retxs == 0) {
    prev_tx_params.dci_cfg_type    = ctx.dci_cfg_type;
    prev_tx_params.nof_repetitions = ctx.nof_repetitions;
    prev_tx_params.olla_mcs        = ctx.olla_mcs;
    prev_tx_params.tbs             = pusch.tb_size_bytes;
    prev_tx_params.slice_id        = ctx.slice_id;
  } else {
    ocudu_assert(ctx.dci_cfg_type == prev_tx_params.dci_cfg_type,
                 "DCI format and RNTI type cannot change during HARQ retxs");
    ocudu_assert(prev_tx_params.tbs == pusch.tb_size_bytes, "TBS cannot change during HARQ retxs");
    ocudu_assert(ctx.nof_repetitions == prev_tx_params.nof_repetitions or ctx.nof_repetitions == 1,
                 "A reTx repeats the PUSCH as many times as the original transmission did, or not at all");
  }
  prev_tx_params.mcs_table   = pusch.mcs_table;
  prev_tx_params.mcs         = pusch.mcs_index;
  prev_tx_params.rbs         = pusch.rbs;
  prev_tx_params.nof_symbols = pusch.symbols.length();
  prev_tx_params.nof_layers  = pusch.nof_layers;

  if (impl->mode == harq_mode_t::feedback_disabled_or_mode_b) {
    // In NTN mode, save the HARQ info in history.
    harq_repo->alloc_hist->save_harq_newtx_info(*impl);
  }
}

// UE HARQ entity.

unique_ue_harq_entity::unique_ue_harq_entity(cell_harq_manager* mgr, du_ue_index_t ue_idx, rnti_t crnti_) :
  cell_harq_mgr(mgr), ue_index(ue_idx), crnti(crnti_)
{
}

unique_ue_harq_entity::unique_ue_harq_entity(unique_ue_harq_entity&& other) noexcept :
  cell_harq_mgr(other.cell_harq_mgr), ue_index(other.ue_index), crnti(other.crnti)
{
  other.cell_harq_mgr = nullptr;
  other.ue_index      = INVALID_DU_UE_INDEX;
  other.crnti         = rnti_t::INVALID_RNTI;
}

unique_ue_harq_entity& unique_ue_harq_entity::operator=(unique_ue_harq_entity&& other) noexcept
{
  if (cell_harq_mgr != nullptr) {
    cell_harq_mgr->destroy_ue(ue_index);
  }
  cell_harq_mgr       = other.cell_harq_mgr;
  ue_index            = other.ue_index;
  crnti               = other.crnti;
  other.cell_harq_mgr = nullptr;
  other.ue_index      = INVALID_DU_UE_INDEX;
  other.crnti         = rnti_t::INVALID_RNTI;
  return *this;
}

unique_ue_harq_entity::~unique_ue_harq_entity()
{
  if (cell_harq_mgr != nullptr) {
    cell_harq_mgr->destroy_ue(ue_index);
  }
}

void unique_ue_harq_entity::reconfigure(unsigned                              new_nof_dl_harqs,
                                        unsigned                              new_nof_ul_harqs,
                                        const harq_dl_feedback_disabled_mask& dl_harq_feedback_disabled_mask,
                                        const harq_ul_mode_mask&              ul_harq_mode_mask,
                                        unsigned                              nof_cg_reserved_harqs)
{
  ocudu_assert(cell_harq_mgr, "Cell HARQ manager not available");
  ocudu_assert(new_nof_dl_harqs >= nof_dl_harqs() and new_nof_dl_harqs > 0, "Cannot shrink nof DL HARQs");
  ocudu_assert(new_nof_ul_harqs >= nof_ul_harqs() and new_nof_ul_harqs > 0, "Cannot shrink nof UL HARQs");
  ocudu_assert(nof_cg_reserved_harqs <= cell_harq_mgr->ul.max_harqs_per_ue,
               "nof_cg_reserved_harqs={} exceeds max_harqs_per_ue={}",
               nof_cg_reserved_harqs,
               cell_harq_mgr->ul.max_harqs_per_ue);

  first_non_reserved_harq_id             = to_harq_id(nof_cg_reserved_harqs);
  get_ul_ue().first_non_reserved_harq_id = first_non_reserved_harq_id;

  if (cell_harq_mgr->ul.ntn_ul_alloc_delay_slots == 0) {
    // Not NTN cell, do not set DL HARQ Feedback Disabled and UL HARQ mode B.
    return;
  }

  // Extend HARQ processes if the new configuration requests more than currently available.
  if (new_nof_dl_harqs != nof_dl_harqs()) {
    cell_harq_mgr->dl.extend_ue_harqs(ue_index, crnti, new_nof_dl_harqs);
  }
  if (new_nof_ul_harqs != nof_ul_harqs()) {
    cell_harq_mgr->ul.extend_ue_harqs(ue_index, crnti, new_nof_ul_harqs);
  }

  if (not dl_harq_feedback_disabled_mask.empty()) {
    get_dl_ue().feedback_disabled_or_mode_b_harq_present = dl_harq_feedback_disabled_mask.any();
    for (auto& h : get_dl_ue().harqs) {
      // A bit set to one indicates HARQ processes with disabled DL HARQ feedback and the bit set to zero with enabled.
      if (dl_harq_feedback_disabled_mask.test(h.h_id)) {
        h.mode = harq_mode_t::feedback_disabled_or_mode_b;
      } else {
        h.mode = harq_mode_t::normal;
      }
      // Mirrored so that the free processes of a mode are found without walking them.
      get_dl_ue().harq_mode_mask.set(h.h_id, h.mode == harq_mode_t::normal);
    }
  }

  if (not ul_harq_mode_mask.empty()) {
    if (not ul_harq_mode_mask.all() and not cell_harq_mgr->ul.is_ntn_harq_mode_b_enabled()) {
      report_error("Cannot set UL HARQ process to mode B as the feature is not enabled in PUSCH config.");
    }
    get_ul_ue().feedback_disabled_or_mode_b_harq_present = not ul_harq_mode_mask.all();
    for (auto& h : get_ul_ue().harqs) {
      // A bit set to one identifies a HARQ process in mode A and a bit set to zero identifies a HARQ process in mode B.
      if (ul_harq_mode_mask.test(h.h_id)) {
        h.mode = harq_mode_t::normal;
      } else {
        h.mode = harq_mode_t::feedback_disabled_or_mode_b;
      }
      // Mirrored, as in the DL.
      get_ul_ue().harq_mode_mask.set(h.h_id, h.mode == harq_mode_t::normal);
    }
  }
}

bool unique_ue_harq_entity::has_empty_dl_harqs(std::optional<harq_mode_t> mode) const
{
  const auto& ue = get_dl_ue();
  // An entity that is not mode-selective takes any free process, so the list answers without consulting the modes.
  if (not mode.has_value() or not is_dl_harq_mode_selective()) {
    return ue.free_harqs.any();
  }
  return free_harq_mask(ue, *mode).any();
}

bool unique_ue_harq_entity::has_empty_ul_harqs(std::optional<harq_mode_t> mode) const
{
  const auto& ue = get_ul_ue();
  // The processes below the first non-reserved one are held for configured grants and are not up for allocation.
  if (not mode.has_value() or not is_ul_harq_mode_selective()) {
    return ue.free_harqs.any(first_non_reserved_harq_id, ue.free_harqs.size());
  }
  return free_harq_mask(ue, *mode).any(first_non_reserved_harq_id, ue.free_harqs.size());
}

size_t unique_ue_harq_entity::nof_empty_dl_harqs(std::optional<harq_mode_t> mode) const
{
  const auto& ue = get_dl_ue();
  if (not mode.has_value() or not is_dl_harq_mode_selective()) {
    return ue.free_harqs.count();
  }
  return free_harq_mask(ue, *mode).count();
}

size_t unique_ue_harq_entity::nof_empty_ul_harqs(std::optional<harq_mode_t> mode) const
{
  const auto& ue = get_ul_ue();
  if (not mode.has_value() or not is_ul_harq_mode_selective()) {
    return ue.free_harqs.count();
  }
  // The processes held for configured grants are dropped before the count, which takes no range of its own.
  fixed_size_bitset<MAX_NOF_HARQS> candidates = free_harq_mask(ue, *mode);
  candidates.fill(0, first_non_reserved_harq_id, false);
  return candidates.count();
}

void unique_ue_harq_entity::reset()
{
  if (cell_harq_mgr != nullptr) {
    cell_harq_mgr->destroy_ue(ue_index);
    cell_harq_mgr = nullptr;
  }
}

void unique_ue_harq_entity::cancel_retxs()
{
  for (auto& h_dl : get_dl_ue().harqs) {
    if (h_dl.status != harq_state_t::empty) {
      dl_harq_process_handle{cell_harq_mgr->dl, h_dl}.cancel_retxs();
    }
  }

  for (auto& h_ul : get_ul_ue().harqs) {
    if (h_ul.status != harq_state_t::empty) {
      ul_harq_process_handle{cell_harq_mgr->ul, h_ul}.cancel_retxs();
    }
  }
}

std::optional<ul_harq_process_handle> unique_ue_harq_entity::ul_harq(harq_id_t h_id, slot_point slot)
{
  if (get_ul_ue().harqs[h_id].mode == harq_mode_t::feedback_disabled_or_mode_b and
      cell_harq_mgr->ul.is_ntn_harq_mode_b_enabled()) {
    // First try to find in the allocation history.
    auto harq_mode_b = cell_harq_mgr->ul.alloc_hist->find_ul_harq(ue_index, slot);
    if (harq_mode_b.has_value()) {
      return harq_mode_b;
    }
  }
  return ul_harq(h_id);
}

std::optional<const ul_harq_process_handle> unique_ue_harq_entity::ul_harq(harq_id_t h_id, slot_point slot) const
{
  if (get_ul_ue().harqs[h_id].mode == harq_mode_t::feedback_disabled_or_mode_b and
      cell_harq_mgr->ul.is_ntn_harq_mode_b_enabled()) {
    // First try to find in the allocation history.
    auto harq_mode_b = cell_harq_mgr->ul.alloc_hist->find_ul_harq(ue_index, slot);
    if (harq_mode_b.has_value()) {
      return harq_mode_b;
    }
  }
  return ul_harq(h_id);
}

std::optional<dl_harq_process_handle> unique_ue_harq_entity::alloc_dl_harq(slot_point              sl_pdcch,
                                                                           slot_point              sl_pdsch,
                                                                           unsigned                ack_delay,
                                                                           unsigned                max_harq_nof_retxs,
                                                                           unsigned                harq_bit_idx,
                                                                           harq_mode_t             mode,
                                                                           uint8_t                 nof_repetitions,
                                                                           std::optional<unsigned> last_ack_delay)
{
  dl_harq_process_impl* h = cell_harq_mgr->new_dl_tx(ue_index,
                                                     crnti,
                                                     sl_pdcch,
                                                     sl_pdsch,
                                                     ack_delay,
                                                     last_ack_delay.value_or(ack_delay),
                                                     max_harq_nof_retxs,
                                                     harq_bit_idx,
                                                     mode,
                                                     nof_repetitions);
  if (h == nullptr) {
    return std::nullopt;
  }
  return dl_harq_process_handle(cell_harq_mgr->dl, *h);
}

std::optional<ul_harq_process_handle>
unique_ue_harq_entity::alloc_ul_harq(slot_point                          sl_pdcch,
                                     slot_point                          sl_tx,
                                     unsigned                            max_harq_nof_retxs,
                                     std::optional<cg_harq_alloc_params> cg_params,
                                     harq_mode_t                         mode,
                                     uint8_t                             nof_repetitions)
{
  ul_harq_process_impl* h =
      cell_harq_mgr->new_ul_tx(ue_index, crnti, sl_pdcch, sl_tx, max_harq_nof_retxs, cg_params, mode, nof_repetitions);
  if (h == nullptr) {
    return std::nullopt;
  }
  return ul_harq_process_handle(cell_harq_mgr->ul, *h);
}

std::optional<dl_harq_process_handle> unique_ue_harq_entity::find_pending_dl_retx()
{
  dl_harq_process_impl* h = cell_harq_mgr->dl.find_ue_harq_in_state(ue_index, harq_state_t::pending_retx);
  if (h == nullptr) {
    return std::nullopt;
  }
  return dl_harq_process_handle(cell_harq_mgr->dl, *h);
}

std::optional<const dl_harq_process_handle> unique_ue_harq_entity::find_pending_dl_retx() const
{
  dl_harq_process_impl* h = cell_harq_mgr->dl.find_ue_harq_in_state(ue_index, harq_state_t::pending_retx);
  if (h == nullptr) {
    return std::nullopt;
  }
  return dl_harq_process_handle(cell_harq_mgr->dl, *h);
}

std::optional<ul_harq_process_handle> unique_ue_harq_entity::find_pending_ul_retx()
{
  ul_harq_process_impl* h = cell_harq_mgr->ul.find_ue_harq_in_state(ue_index, harq_state_t::pending_retx);
  if (h == nullptr) {
    return std::nullopt;
  }
  return ul_harq_process_handle(cell_harq_mgr->ul, *h);
}

std::optional<const ul_harq_process_handle> unique_ue_harq_entity::find_pending_ul_retx() const
{
  ul_harq_process_impl* h = cell_harq_mgr->ul.find_ue_harq_in_state(ue_index, harq_state_t::pending_retx);
  if (h == nullptr) {
    return std::nullopt;
  }
  return ul_harq_process_handle(cell_harq_mgr->ul, *h);
}

std::optional<dl_harq_process_handle> unique_ue_harq_entity::find_dl_harq_waiting_ack()
{
  dl_harq_process_impl* h = cell_harq_mgr->dl.find_ue_harq_in_state(ue_index, harq_state_t::waiting_ack);
  if (h == nullptr) {
    return std::nullopt;
  }
  return dl_harq_process_handle(cell_harq_mgr->dl, *h);
}

std::optional<const dl_harq_process_handle> unique_ue_harq_entity::find_dl_harq_waiting_ack() const
{
  dl_harq_process_impl* h = cell_harq_mgr->dl.find_ue_harq_in_state(ue_index, harq_state_t::waiting_ack);
  if (h == nullptr) {
    return std::nullopt;
  }
  return dl_harq_process_handle(cell_harq_mgr->dl, *h);
}

std::optional<ul_harq_process_handle> unique_ue_harq_entity::find_ul_harq_waiting_ack()
{
  ul_harq_process_impl* h = cell_harq_mgr->ul.find_ue_harq_in_state(ue_index, harq_state_t::waiting_ack);
  if (h == nullptr) {
    return std::nullopt;
  }
  return ul_harq_process_handle(cell_harq_mgr->ul, *h);
}

std::optional<dl_harq_process_handle> unique_ue_harq_entity::find_dl_harq_waiting_ack(slot_point uci_slot,
                                                                                      uint8_t    harq_bit_idx)
{
  std::vector<dl_harq_process_impl>& dl_harqs = cell_harq_mgr->dl.ues[ue_index]->harqs;
  for (dl_harq_process_impl& h : dl_harqs) {
    if (h.mode == harq_utils::harq_mode_t::normal and h.status == harq_utils::harq_state_t::waiting_ack and
        h.slot_ack == uci_slot and h.harq_bit_idx == harq_bit_idx) {
      return dl_harq_process_handle(cell_harq_mgr->dl, h);
    }
  }
  return std::nullopt;
}

std::optional<ul_harq_process_handle> unique_ue_harq_entity::find_ul_harq_waiting_ack(slot_point pusch_slot)
{
  if (cell_harq_mgr->ul.is_ntn_harq_mode_b_enabled()) {
    // First try to find in the allocation history.
    auto harq_mode_b = cell_harq_mgr->ul.alloc_hist->find_ul_harq(ue_index, pusch_slot);
    if (harq_mode_b.has_value()) {
      return harq_mode_b;
    }
  }

  // Note: a bundle draws one CRC report per occasion, in different slots, so match any slot from the base occasion
  // to the last one and let the caller decide what a report means. Outside a bundle last_occasion_slot equals
  // slot_tx, so this reduces to the plain equality it replaces.
  std::vector<ul_harq_process_impl>& ul_harqs = cell_harq_mgr->ul.ues[ue_index]->harqs;
  for (ul_harq_process_impl& h : ul_harqs) {
    if (h.mode == harq_utils::harq_mode_t::normal and h.status == harq_utils::harq_state_t::waiting_ack and
        h.slot_tx <= pusch_slot and pusch_slot <= h.last_occasion_slot) {
      return ul_harq_process_handle(cell_harq_mgr->ul, h);
    }
  }
  return std::nullopt;
}

units::bytes unique_ue_harq_entity::total_ul_bytes_waiting_ack(std::optional<ran_slice_id_t> slice_id) const
{
  units::bytes ntn_mode_b_ul_bytes{0};
  if (cell_harq_mgr->ul.is_ntn_harq_mode_b_enabled()) {
    // Note: HARQs in alloc_hist are still pending ACK.
    ntn_mode_b_ul_bytes = cell_harq_mgr->ul.alloc_hist->sum_pending_ul_tbs(ue_index, slice_id);
  }

  units::bytes harq_bytes{0};
  for (unsigned i = 0, e = nof_ul_harqs(); i != e; ++i) {
    const ul_harq_process_impl& h = get_ul_ue().harqs[i];
    if (h.status == harq_utils::harq_state_t::empty or h.mode != harq_mode_t::normal) {
      continue;
    }
    // A transmission no slice claims drains whichever of them the UE fills it from, so it counts against each.
    if (not slice_id.has_value() or not h.prev_tx_params.slice_id.has_value() or
        h.prev_tx_params.slice_id == slice_id) {
      harq_bytes += h.prev_tx_params.tbs;
    }
  }

  return harq_bytes + ntn_mode_b_ul_bytes;
}
