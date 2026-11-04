// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../cell/cell_harq_manager.h"
#include "ocudu/adt/slotted_vector.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/ssb/ssb_configuration.h"
#include "ocudu/scheduler/scheduler_rach_handler.h"

namespace ocudu {

class cell_configuration;

/// \brief State of a UE's Random Access attempt, tracked from PRACH detection (TC-RNTI allocation) until
/// contention resolution, shared between the RA scheduler and the UE fallback scheduler.
struct ra_ue_context {
  /// Detected PRACH Preamble which will be associated to the Msg3/MsgA PUSCH to be scheduled.
  rach_indication_message::preamble preamble{};
  /// Slot at which the PRACH preamble was received.
  slot_point prach_slot_rx;
  /// Index of the SS/PBCH block associated with the detected preamble, as per TS 38.213, Section 8.1.
  ssb_id_t ssb_index;
  /// HARQ entity used to allocate the UL HARQ process for Msg3 (native 4-step or 2-step fallback).
  /// \note [TS 38.321, 5.4.2.1] "For UL transmission with UL grant in RA Response, HARQ process identifier 0 is
  /// used".
  unique_ue_harq_entity harq_ent;
  /// Set when \c erase was called on this entry while its Msg3 HARQ was still awaiting ACK/CRC feedback. The entry
  /// is only removed from the table once that HARQ concludes, so its ring slot cannot be reused while the HARQ
  /// manager still tracks it.
  bool pending_removal = false;
  /// Slot of the 2-step RACH successRAR MsgB PDSCH. Nullopt until \c set_msgb_scheduled commits it, only ever set
  /// for a successRAR completion.
  std::optional<slot_point> msgb_slot_tx;
  /// Slot of the successRAR's own HARQ-ACK PUCCH feedback. Set together with \c msgb_slot_tx.
  std::optional<slot_point> msgb_ack_slot_tx;

  /// TC-RNTI associated with this UE in RA.
  rnti_t tc_rnti() const { return preamble.tc_rnti; }

  /// True if this entry was created following a 2-step RACH successRAR completion (no Msg3 HARQ involved). Always
  /// false for a Msg3-tracking entry (native 4-step or 2-step fallback).
  bool is_msgb_success_rar() const { return harq_ent.empty(); }

  /// True if this is a successRAR entry whose MsgB HARQ-ACK PUCCH slot is not yet committed or still in the future.
  bool is_msgb_success_rar_pending(slot_point slot_tx) const
  {
    return is_msgb_success_rar() and (not msgb_ack_slot_tx.has_value() or *msgb_ack_slot_tx >= slot_tx);
  }
};

/// \brief Repository of in-flight Random Access attempts, indexed by TC-RNTI in a circular hashing fashion (the
/// UE index is not yet assigned at this stage of the RA procedure). Also owns the pool of HARQ processes backing
/// each attempt's \c harq_ent, so that the attempt and its backing HARQ process share a single lifetime.
///
/// Shared between the RA scheduler, which owns the RA procedure and is the sole writer, and the UE-dedicated
/// scheduler (fallback scheduler, UE event manager).
class ra_ue_repository
{
  /// Container for RA UE contexts, indexed by TC-RNTI.
  using storage_type = slotted_vector<ra_ue_context>;

  /// (Implementation-defined) limit for maximum number of concurrent Msg3s or MsgBs.
  static constexpr size_t MAX_CONCURRENT_MSG3_OR_MSGB = 512;

public:
  using iterator       = storage_type::iterator;
  using const_iterator = storage_type::const_iterator;

  explicit ra_ue_repository(const cell_configuration& cell_cfg,
                            ocudulog::basic_logger&   logger,
                            size_t                    capacity = MAX_CONCURRENT_MSG3_OR_MSGB);

  /// HARQ process pool backing the \c harq_ent field of the contexts held in this repository.
  cell_harq_manager& harqs() { return ra_harqs; }

  /// \brief Updates the HARQ process pool for the new slot, and erases any RA UE entry whose
  /// ra-ContentionResolutionTimer has expired.
  void slot_indication(slot_point sl_tx);

  /// \brief Clears all entries in the repository.
  void clear() { table.clear(); }

  /// \brief Returns the number of entries in the repository.
  size_t size() const { return table.size(); }

  /// \brief Checks if the repository is empty.
  bool empty() const { return table.empty(); }

  iterator       begin() { return table.begin(); }
  iterator       end() { return table.end(); }
  const_iterator begin() const { return table.begin(); }
  const_iterator end() const { return table.end(); }

  /// \brief Adds a new RA UE entry for the detected preamble's TC-RNTI, together with the UL HARQ entity used for
  /// its Msg3 retransmissions (native 4-step or 2-step fallback).
  /// \return Pointer to the newly created entry; \c nullptr if a ring-key collision was detected (an unrelated,
  /// still-live entry already occupies this TC-RNTI's ring slot).
  ra_ue_context* add(const rach_indication_message::preamble& preamble, slot_point prach_slot_rx, ssb_id_t ssb_index)
  {
    ra_ue_context* ctx = add_entry(preamble, prach_slot_rx, ssb_index);
    if (ctx == nullptr) {
      return nullptr;
    }
    ctx->harq_ent = ra_harqs.add_ue(get_temp_ue_index(preamble.tc_rnti), preamble.tc_rnti, 1, 1);
    return ctx;
  }

  /// \brief Restarts the RA attempt of the Msg3-tracking entry of \c preamble's TC-RNTI with the new preamble.
  /// \return Pointer to the restarted entry; \c nullptr if no Msg3-tracking entry exists for this exact TC-RNTI, or if
  /// its Msg3 HARQ is still awaiting CRC.
  ra_ue_context*
  restart(const rach_indication_message::preamble& preamble, slot_point prach_slot_rx, ssb_id_t ssb_index)
  {
    iterator it = find(preamble.tc_rnti);
    if (it == end() or it->is_msgb_success_rar() or it->harq_ent.find_ul_harq_waiting_ack().has_value()) {
      return nullptr;
    }
    // The UE abandoned the previous attempt, so its pending Msg3 reTx is released, leaving the HARQ free for the new
    // attempt.
    it->harq_ent.cancel_retxs();
    it->preamble        = preamble;
    it->prach_slot_rx   = prach_slot_rx;
    it->ssb_index       = ssb_index;
    it->pending_removal = false;
    return &*it;
  }

  /// Adds a new RA UE entry as soon as MsgA CRC=OK is known, before the RA scheduler commits the successRAR grant
  /// (can lag by several slots). No Msg3 HARQ is allocated, since contention is already resolved.
  /// \return Pointer to the new entry; \c nullptr on a TC-RNTI ring-key collision.
  ra_ue_context*
  add_msgb_pending(const rach_indication_message::preamble& preamble, slot_point prach_slot_rx, ssb_id_t ssb_index)
  {
    return add_entry(preamble, prach_slot_rx, ssb_index);
  }

  /// Records the successRAR MsgB PDSCH slot and its HARQ-ACK feedback slot on an entry created by \c
  /// add_msgb_pending.
  /// \return \c false if no entry exists for \c tc_rnti.
  bool set_msgb_scheduled(rnti_t tc_rnti, slot_point msgb_slot_tx, slot_point msgb_ack_slot_tx)
  {
    iterator it = find(tc_rnti);
    if (it == end()) {
      return false;
    }
    it->msgb_slot_tx     = msgb_slot_tx;
    it->msgb_ack_slot_tx = msgb_ack_slot_tx;
    return true;
  }

  /// \brief Erase a RA UE entry from the repository.
  /// \note If the entry's Msg3 HARQ is still awaiting ACK/CRC feedback, the removal is deferred: retransmissions are
  /// cancelled and the entry is only actually erased, by \c slot_indication, once that HARQ concludes.
  iterator erase(rnti_t tc_rnti) { return erase(find(tc_rnti)); }
  iterator erase(iterator it)
  {
    if (it == end()) {
      return it;
    }
    if (not it->harq_ent.empty() and it->harq_ent.find_ul_harq_waiting_ack().has_value()) {
      it->harq_ent.cancel_retxs();
      it->pending_removal = true;
      iterator next_it    = it;
      ++next_it;
      return next_it;
    }
    return table.erase(it);
  }

  /// \brief Looks up the RA context for a TC-RNTI.
  /// \return The RA context, if an entry exists for this exact TC-RNTI. Returns \c nullptr otherwise.
  const_iterator find(rnti_t tc_rnti) const
  {
    auto it = table.find(ring_key(tc_rnti));
    return it != end() and it->tc_rnti() == tc_rnti ? it : end();
  }
  iterator find(rnti_t tc_rnti)
  {
    auto it = table.find(ring_key(tc_rnti));
    return it != end() and it->tc_rnti() == tc_rnti ? it : end();
  }

private:
  class msg3_harq_timeout_notifier;

  /// Maps a TC-RNTI to its ring index in this repository.
  uint16_t ring_key(rnti_t tc_rnti) const { return static_cast<uint16_t>(to_underlying(tc_rnti) % ring_capacity); }

  /// Derive temporary UE index.
  /// \note RA UEs don't have a UE index yet assigned, so we generate a temporary one. This index will be internal to
  /// the repository.
  du_ue_index_t get_temp_ue_index(rnti_t tc_rnti) const { return static_cast<du_ue_index_t>(ring_key(tc_rnti)); }

  /// \brief Inserts a new entry for the detected preamble's TC-RNTI, saving the preamble directly.
  /// \return Pointer to the newly created entry; \c nullptr if a ring-key collision was detected (an unrelated,
  /// still-live entry already occupies this TC-RNTI's ring slot).
  ra_ue_context*
  add_entry(const rach_indication_message::preamble& preamble, slot_point prach_slot_rx, ssb_id_t ssb_index)
  {
    const uint16_t key = ring_key(preamble.tc_rnti);
    if (table.contains(key)) {
      return nullptr;
    }
    auto& ctx         = table.emplace(key);
    ctx.preamble      = preamble;
    ctx.prach_slot_rx = prach_slot_rx;
    ctx.ssb_index     = ssb_index;
    return &ctx;
  }

  // ra-ContentionResolutionTimer duration, in slots.
  const unsigned ra_con_res_timer_slots;

  // Hard bound on the number of concurrent RA UE entries, and the modulus used by \c ring_key.
  const uint16_t ring_capacity;

  // Manager of UL HARQs for Msg3. Declared before \c table so it outlives every \c ra_ue_context::harq_ent it
  // backs: members are destroyed in reverse declaration order, so the contexts are torn down first.
  cell_harq_manager ra_harqs;

  /// Table of TC-RNTI -> RA UE contexts.
  storage_type table;
};

} // namespace ocudu
