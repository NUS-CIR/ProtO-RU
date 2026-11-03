// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/mpmc_queue.h"
#include "ocudu/adt/span.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ocuduvec/zero.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/serdes/ofh_message_properties.h"
#include "ocudu/ofh/timing/slot_symbol_point.h"
#include "ocudu/ran/frame_types.h"
#include "ocudu/ran/slot_point.h"
#include "ocudu/support/units.h"
#include <atomic>
#include <mutex>

namespace ocudu {
namespace ether {

class frame_buffer_pool;

/// The class manages storage dedicated for an Ethernet frame.
class frame_buffer
{
  friend class frame_buffer_pool;

  /// State of the buffer inside a buffer pool.
  enum class buffer_state : uint8_t {
    /// \brief The buffer is available for being reserved.
    free,
    /// \brief The buffer is reserved by an Open Fronthaul message encoder. It cannot be re-reserved.
    ///
    /// It can transition to one of the following states:
    /// -\c free: in case the buffer remains empty when it gets out of the scope where it was reserved.
    /// -\c pending: the buffer was written by an encoder and it is not empty when it goes out of reservation
    /// scope.
    reserved,
    /// \brief The buffer is pending to be transmitter by the message transmitter.
    ///
    /// It can transition to one of the following states:
    /// -\c queued: when the message transmitter adds it to its transmission queue.
    /// -\c free: when the parent buffer pool decides that the buffer transmission time has expired.
    /// -\c reserved: when the pool pops a buffer from the pending queue and decides that it is not expired. This is a
    /// workaround, as the state will be changed back to pending when the buffer goes out of scope, as described above.
    pending,
    /// \brief The buffer is owned by the Open Fronthaul message transmitter and is queued to be transmitted over
    /// Ethernet.
    ///
    /// From this state it can transition to \c free state when the message transmitter releases the ownership over it.
    queued
  };

  std::atomic<buffer_state> state = buffer_state::free;
  /// Actual storage.
  std::vector<uint8_t> buffer;
  /// Size of written data.
  size_t sz = 0;

  const ofh::slot_symbol_point invalid_timestamp{slot_point(), 0, 14};
  /// Timestamp indicates in which slot this buffer was reserved.
  /// When the buffer is in \c pending state this field is used to check if it has expired.
  ofh::slot_symbol_point reservation_symbol_point = invalid_timestamp;

public:
  /// Constructors.
  frame_buffer() = default;
  explicit frame_buffer(unsigned size) : buffer(size, 0) {}

  /// The buffer is considered empty if a the reserving part didn't call \c set_size method.
  constexpr bool empty() const noexcept { return sz == 0; }

  /// Returns the size of written data.
  constexpr size_t size() const noexcept { return sz; }

  /// Sets the size of written data.
  constexpr void set_size(size_t new_size) noexcept
  {
    ocudu_assert(new_size <= MAX_ETH_FRAME_LENGTH, "Can't set_size: new size if out-of-bounds");
    unsigned written_size = (new_size < MIN_ETH_FRAME_LENGTH) ? MIN_ETH_FRAME_LENGTH : new_size;
    if (OCUDU_UNLIKELY(new_size < MIN_ETH_FRAME_LENGTH)) {
      ocuduvec::zero(span<uint8_t>(&buffer[new_size], &buffer[MIN_ETH_FRAME_LENGTH]));
    }
    sz = written_size;
  }

  /// Resets the size of written data to zero.
  constexpr void clear() noexcept { sz = 0; }

  /// Returns timestamp of this buffer that indicates the slot and symbol in which the buffer was reserved and written.
  ofh::slot_symbol_point get_slot_symbol() { return reservation_symbol_point; }

  /// Gets the buffer.
  span<uint8_t> get_buffer() noexcept { return buffer; }

  /// Gets the read-only buffer.
  span<const uint8_t> data() const noexcept { return {buffer.data(), sz}; }

private:
  /// Sets the timestamp of the buffer upon reservation.
  void set_slot_symbol(ofh::slot_symbol_point symbol_point_) { reservation_symbol_point = symbol_point_; }
};

/// \brief Pool of Ethernet frame buffers with lock-free access.
///
/// The pool stores an array of unique pointers to \c frame_buffer.
class frame_buffer_pool
{
  using state = frame_buffer::buffer_state;

  /// Queue type used for storing buffers identifiers.
  using queue_type =
      concurrent_queue<unsigned, concurrent_queue_policy::lockfree_mpmc, concurrent_queue_wait_policy::non_blocking>;

  using unique_frame_buffer = std::unique_ptr<frame_buffer>;

  /// \brief Custom deleter of the \c std::unique_ptr<unique_frame_buffer> object.
  ///
  /// It doesn't free the memory but only updates the state of the buffer when a unique_ptr owning it goes out of scope.
  struct buffer_deleter {
    frame_buffer_pool* pool;

    void operator()(unique_frame_buffer* ptr)
    {
      if (ptr != nullptr) {
        pool->update_state(*ptr);
      }
    }
  };

public:
  /// A wrapper over std::unique_ptr<unique_frame_buffer> that simplifies access to the pointed \c frame_buffer object.
  class scoped_frame_buffer
  {
    using managed_frame_buffer = std::unique_ptr<unique_frame_buffer, buffer_deleter>;
    managed_frame_buffer buffer;

  public:
    scoped_frame_buffer() : buffer(nullptr, buffer_deleter{nullptr}) {}
    scoped_frame_buffer(unique_frame_buffer* ptr, buffer_deleter deleter) : buffer(ptr, deleter) {}

    /// These two methods simplify access to the frame buffer directly.
    frame_buffer* operator->() const noexcept { return (*buffer).get(); }
    frame_buffer& operator*() const noexcept { return **buffer; }

    /// Returns pointer to the managed object mimicking unique_ptr interface.
    unique_frame_buffer* get() noexcept { return buffer.get(); }

    /// Mimics the unique_ptr interface and allows to use the class object in boolean expressions.
    explicit operator bool() const noexcept { return static_cast<bool>(buffer); }
  };

  /// Constructor receives minimum required number of buffers with a given size required for encoding an OFDM symbol. It
  /// allocates twice the requested amount given that both data and PRACH can be scheduled in the same uplink symbol.
  /// The pool assigns an identifier to each of buffers and puts them into a list of free buffers.
  frame_buffer_pool(ocudulog::basic_logger& logger_, unsigned min_nof_buffers, unsigned buffer_size) :
    number_of_buffers(min_nof_buffers * 2),
    logger(logger_),
    free_list(number_of_buffers),
    pending_list(number_of_buffers)
  {
    for (unsigned i = 0; i != number_of_buffers; ++i) {
      entries.emplace_back(std::make_unique<frame_buffer>(buffer_size));
      while (!free_list.try_push(i)) {
      }
    }
  }

  /// The class is non-copyable.
  frame_buffer_pool(const frame_buffer_pool&)            = delete;
  frame_buffer_pool& operator=(const frame_buffer_pool&) = delete;

  /// \brief Tries to get a free buffer from the pre-allocated storage.
  ///
  /// \param[in] symbol_point - slot and symbol where the reservation of buffer is requested.
  /// \return returns non-empty \c scoped_frame_buffer on success, otherwise a \c scoped_frame_buffer initialized with
  /// nullptr.
  scoped_frame_buffer reserve(ofh::slot_symbol_point symbol_point)
  {
    unsigned idx;
    if (!free_list.try_pop(idx)) {
      logger.warning("Ethernet frame pool: failed to reserve a buffer");
      return {};
    }
    // Peek the free buffer.
    frame_buffer& buffer = *entries[idx];

    // Change state to 'reserved'.
    auto current_state = buffer.state.load(std::memory_order_acquire);
    ocudu_sanity_check(current_state == state::free,
                       "Ethernet frame pool: invalid state of the buffer popped from the free list");
    while (!buffer.state.compare_exchange_weak(current_state, state::reserved)) {
    }

    // Clear it before returning and save the reservation timestamp.
    buffer.clear();
    buffer.set_slot_symbol(symbol_point);

    return scoped_frame_buffer{&entries[idx], buffer_deleter{this}};
  }

  /// \brief Enqueues buffers in \c state::pending into the given vector.
  ///
  /// The pending buffers reserved for a slot other than the given one are expired: they were written after their slot
  /// was cleared, so they are freed instead of being enqueued.
  ///
  /// \param[out] burst_of_frames    - a vector in which the pending buffers must be enqueued.
  /// \param[out] nof_expired_frames - incremented by the number of expired buffers freed.
  /// \param[in]  slot               - slot being transmitted.
  /// \return true if all pending buffers were enqueued, false if the vector is full and buffers remain pending.
  bool enqueue_pending(static_vector<scoped_frame_buffer, MAX_TX_BURST_SIZE>& burst_of_frames,
                       unsigned&                                              nof_expired_frames,
                       slot_point                                             slot)
  {
    unsigned idx;
    while (!burst_of_frames.full() && pending_list.try_pop(idx)) {
      ocudu_sanity_check(idx < entries.size(), "Ethernet frame pool: invalid buffer popped from pending list");

      auto& buffer = entries[idx];
      if (OCUDU_UNLIKELY(buffer->get_slot_symbol().get_slot() != slot)) {
        // Updating the state of a pending buffer frees it.
        update_state(buffer);
        ++nof_expired_frames;
        continue;
      }
      // Change the state to 'queued'.
      auto current_state = buffer->state.load(std::memory_order_acquire);
      ocudu_sanity_check(current_state == state::pending, "Invalid state of the buffer popped from pending list");
      while (!buffer->state.compare_exchange_weak(current_state, state::queued)) {
      }
      // Add popped buffer into the output vector.
      burst_of_frames.emplace_back(scoped_frame_buffer{&entries[idx], buffer_deleter{this}});
    }

    return !burst_of_frames.full() || pending_list.empty();
  }

  /// \brief Enqueues buffers in \c state::pending into the given vector.
  ///
  /// This method is used when pending buffers need to be checked for expiry.
  ///
  /// \param[in] burst_of_frames - a vector in which the pending buffers must be enqueued.
  void pop_pending(static_vector<scoped_frame_buffer, MAX_TX_BURST_SIZE>& burst_of_frames)
  {
    unsigned idx;
    while (!burst_of_frames.full() && pending_list.try_pop(idx)) {
      ocudu_sanity_check(idx < entries.size(), "Invalid buffer popped from pending list");
      burst_of_frames.emplace_back(scoped_frame_buffer{&entries[idx], buffer_deleter{this}});
    }
  }

  /// \brief Puts the given buffer back into the pending queue.
  ///
  /// The method is used after verifying that the buffer has not been expired.
  void return_to_pending(unique_frame_buffer& buffer)
  {
    // Change the state to 'reserved' and it will be put to pending queue when it goes out of scope.
    auto current_state = buffer->state.load(std::memory_order_acquire);
    ocudu_sanity_check(current_state == state::pending, "Invalid state of the pending buffer");

    while (!buffer->state.compare_exchange_weak(current_state, state::reserved)) {
    }
  }

private:
  /// Transitions the given frame buffer into the next state based on its current state.
  void update_state(unique_frame_buffer& buffer)
  {
    auto current_state = buffer->state.load();
    bool success       = false;

    while (!success) {
      switch (current_state) {
        case state::reserved:
          if (buffer->empty()) {
            success = buffer->state.compare_exchange_strong(current_state, state::free);
            if (OCUDU_LIKELY(success)) {
              free(buffer);
            }
          } else {
            success = buffer->state.compare_exchange_strong(current_state, state::pending);
            if (OCUDU_LIKELY(success)) {
              put_to_pending_queue(buffer);
            }
          }
          break;
        case state::pending:
          /// When a 'pending' buffer is popped from the queue without changing its state to 'enqueued', it gets freed.
          success = buffer->state.compare_exchange_strong(current_state, state::free);
          if (OCUDU_LIKELY(success)) {
            free(buffer);
          }
          break;
        case state::queued:
          success = buffer->state.compare_exchange_strong(current_state, state::free);
          if (OCUDU_LIKELY(success)) {
            free(buffer);
          }
          break;
        default:
          success = true;
          break;
      }
    }
  }

  /// Puts the reserved buffer into the 'pending' queue.
  void put_to_pending_queue(unique_frame_buffer& buffer)
  {
    unsigned idx = &buffer - entries.data();
    ocudu_sanity_check(idx < entries.size(), "Ethernet frame pool: unknown buffer can't be saved in 'pending' queue");
    if (OCUDU_UNLIKELY(!pending_list.try_push(idx))) {
      logger.warning("Ethernet frame pool: failed to add buffer to the 'pending' queue");
      free(buffer);
    }
  }

  /// Puts the buffer into a queue of free buffers.
  void free(unique_frame_buffer& buffer)
  {
    unsigned idx = &buffer - entries.data();
    ocudu_sanity_check(idx < entries.size(), "Ethernet frame pool: cannot free a buffer not pertaining to this pool");

    // Push identifier back in the pool.
    while (OCUDU_UNLIKELY(!free_list.try_push(idx))) {
      logger.warning("Ethernet frame pool: failed to put buffer into a 'free' queue");
    }
  }

  const unsigned                   number_of_buffers;
  ocudulog::basic_logger&          logger;
  std::vector<unique_frame_buffer> entries;
  queue_type                       free_list;
  queue_type                       pending_list;
};

using scoped_frame_buffer = frame_buffer_pool::scoped_frame_buffer;

/// Specifies an interval of symbols for which \c eth_frame_pool object must be accessed. Interval includes start and
/// end symbols.
struct frame_pool_interval {
  ofh::slot_symbol_point start;
  ofh::slot_symbol_point end;
};

/// Pool of Ethernet frame buffers pre-allocated for symbols in slots.
class eth_frame_pool
{
  /// Number of slots the pool can accommodate.
  static constexpr size_t NUM_SLOTS = 10;

  /// Maximum number of entries contained by the pool (one entry per OFDM symbol).
  static constexpr size_t NUM_ENTRIES = NOF_OFDM_SYM_PER_SLOT_NORMAL_CP * NUM_SLOTS;

  /// Returns pool entry for the given slot and symbol.
  const frame_buffer_pool& get_pool_entry(slot_point slot_point, unsigned symbol) const
  {
    unsigned slot_index = slot_point.slot_index();
    unsigned sym_index  = (slot_index * NOF_OFDM_SYM_PER_SLOT_NORMAL_CP + symbol) % NUM_ENTRIES;
    return *pool[sym_index];
  }

  /// Returns pool entry for the given slot and symbol.
  frame_buffer_pool& get_pool_entry(slot_point slot_point, unsigned symbol)
  {
    return const_cast<frame_buffer_pool&>(const_cast<const eth_frame_pool*>(this)->get_pool_entry(slot_point, symbol));
  }

public:
  /// Constructor
  ///
  /// \param logger_        - Logger object.
  /// \param mtu            - MTU size of the Ethernet port. It defines the size of the allocated buffers.
  /// \param num_of_frames  - Number of frames with given MTU size that can be send in one symbol.
  /// \param type           - Type of Open Fronthaul messages this pool is dedicated for.
  /// \param direction      - Direction of the Open Fronthaul messages this pool is dedicated for.
  eth_frame_pool(ocudulog::basic_logger& logger_,
                 units::bytes            mtu,
                 unsigned                num_of_frames,
                 ofh::message_type       type,
                 ofh::data_direction     direction) :
    logger(logger_), pool_frames_type(type), pool_frames_data_direction(direction)
  {
    for (unsigned i = 0; i != NUM_ENTRIES; ++i) {
      /// Multiply by 2 because for uplink C-Plane we may need to allocate space for data and PRACH.
      pool.emplace_back(
          std::make_unique<frame_buffer_pool>(logger, num_of_frames * ofh::MAX_NOF_SUPPORTED_EAXC * 2, mtu.value()));
    }
  }

  /// Tries to reserves a buffer from the pool allocated for the given slot and symbol.
  scoped_frame_buffer reserve(ofh::slot_symbol_point symbol_point)
  {
    auto& p_entry = get_pool_entry(symbol_point.get_slot(), symbol_point.get_symbol_index());
    return p_entry.reserve(symbol_point);
  }

  /// \brief Enqueues buffers pending in the pool allocated for the given slot and symbol.
  ///
  /// \return true if all pending buffers were enqueued, false if the burst is full and buffers may remain pending.
  bool enqueue_pending_into_burst(ofh::slot_symbol_point                                 symbol_point,
                                  static_vector<scoped_frame_buffer, MAX_TX_BURST_SIZE>& burst)
  {
    auto& p_entry = get_pool_entry(symbol_point.get_slot(), symbol_point.get_symbol_index());

    unsigned nof_expired_frames = 0;
    bool     all_enqueued       = p_entry.enqueue_pending(burst, nof_expired_frames, symbol_point.get_slot());
    account_expired_frames(nof_expired_frames);
    return all_enqueued;
  }

  /// \brief Enqueues buffers pending in the pools allocated for the given interval of symbols.
  ///
  /// \return true if all pending buffers were enqueued, false otherwise. If the given vector fills up, the caller must
  /// empty the burst and call this method again to enqueue the remaining buffers.
  bool enqueue_pending_into_burst(const frame_pool_interval&                             interval,
                                  static_vector<scoped_frame_buffer, MAX_TX_BURST_SIZE>& burst)
  {
    if (interval.start > interval.end) {
      return true;
    }

    // Extra 1 is added to include the end symbol of the interval.
    unsigned distance           = (interval.end - interval.start) + 1;
    unsigned nof_expired_frames = 0;

    bool all_enqueued = true;
    for (unsigned i = 0; i != distance; ++i) {
      ofh::slot_symbol_point tmp_symbol = interval.start + i;

      auto& p_entry = get_pool_entry(tmp_symbol.get_slot(), tmp_symbol.get_symbol_index());
      if (OCUDU_UNLIKELY(!p_entry.enqueue_pending(burst, nof_expired_frames, tmp_symbol.get_slot()))) {
        all_enqueued = false;
        break;
      }
    }
    account_expired_frames(nof_expired_frames);

    return all_enqueued;
  }

  /// Pops 'pending' buffers from the pool corresponding to the given slot and symbol and checks whether they are
  /// expired. Clears expired buffers and logs the respective message. Returns the number of messages that could not be
  /// transmitted or 0 if there is no pending messages to transmit.
  unsigned clear_slot(slot_point slot, unsigned sector)
  {
    unsigned nof_lates = 0;
    {
      auto& entry = get_pool_entry(slot, 0);

      // The entry may store more pending buffers than a single 'frame_burst' can accommodate, hence 'pop_pending' is
      // called repeatedly until the entry is fully cleared.
      for (bool entry_cleared = false; !entry_cleared;) {
        static_vector<scoped_frame_buffer, MAX_TX_BURST_SIZE> frame_burst;
        entry.pop_pending(frame_burst);
        entry_cleared = !frame_burst.full();

        slot_point late_slot = {};

        for (auto& scoped_buffer : frame_burst) {
          if (scoped_buffer->get_slot_symbol().get_slot() != slot) {
            ++nof_lates;
            if (!late_slot.valid()) {
              late_slot = scoped_buffer->get_slot_symbol().get_slot();
            }
          } else {
            entry.return_to_pending(*scoped_buffer.get());
          }
        }
        if (nof_lates) {
          logger.info("Sector #{}: dropped {} {} {} message(s) for slot '{}' that were still pending in the "
                      "transmitter queue after the transmission window closed. The delay may originate upstream (e.g. "
                      "MAC/PHY delivering resource grid close to the deadline); check the '{}' metric",
                      sector,
                      nof_lates,
                      pool_frames_data_direction == ofh::data_direction::downlink ? "downlink" : "uplink",
                      pool_frames_type == ofh::message_type::control_plane ? "C-Plane" : "U-Plane",
                      late_slot,
                      pool_frames_data_direction == ofh::data_direction::downlink
                          ? "dl_rg_arrival_symbols_before_ota"
                          : "ul_req_arrival_symbols_before_ota");
        }
      }
    }

    // DL C-Plane is only written in the first symbol of a slot.
    if (pool_frames_type == ofh::message_type::control_plane &&
        pool_frames_data_direction == ofh::data_direction::downlink) {
      return nof_lates + take_expired_frames();
    }

    slot_point late_slot = {};
    for (unsigned symbol = 1; symbol != NOF_OFDM_SYM_PER_SLOT_NORMAL_CP; ++symbol) {
      auto& cup_entry = get_pool_entry(slot, symbol);

      // The entry may store more pending buffers than a single 'frame_burst' can accommodate, hence 'pop_pending' is
      // called repeatedly until the entry is fully cleared.
      for (bool entry_cleared = false; !entry_cleared;) {
        static_vector<scoped_frame_buffer, MAX_TX_BURST_SIZE> frame_burst;
        cup_entry.pop_pending(frame_burst);
        entry_cleared = !frame_burst.full();

        for (auto& scoped_buffer : frame_burst) {
          if (scoped_buffer->get_slot_symbol().get_slot() != slot) {
            ++nof_lates;
            if (!late_slot.valid()) {
              late_slot = scoped_buffer->get_slot_symbol().get_slot();
            }
          } else {
            cup_entry.return_to_pending(*scoped_buffer.get());
          }
        }
        if (nof_lates) {
          logger.info("Sector #{}: dropped {} {} {} message(s) for slot '{}' that were still pending in the "
                      "transmitter queue after the transmission window closed. The delay may originate upstream (e.g. "
                      "MAC/PHY delivering resource grid close to the deadline); check the '{}' metric",
                      sector,
                      nof_lates,
                      pool_frames_data_direction == ofh::data_direction::downlink ? "downlink" : "uplink",
                      pool_frames_type == ofh::message_type::control_plane ? "C-Plane" : "U-Plane",
                      late_slot,
                      pool_frames_data_direction == ofh::data_direction::downlink
                          ? "dl_rg_arrival_symbols_before_ota"
                          : "ul_req_arrival_symbols_before_ota");
        }
      }
    }
    return nof_lates + take_expired_frames();
  }

  /// Returns number of slots the pool can accommodate.
  static size_t pool_size_in_slots() { return NUM_SLOTS; }

private:
  /// Returns the number of frames expired in the transmission path since the last call, which are also late.
  unsigned take_expired_frames() { return nof_pending_expired_frames.exchange(0, std::memory_order_relaxed); }

  /// Accumulates the given number of expired frames, reported by the next call to \ref clear_slot.
  void account_expired_frames(unsigned nof_frames)
  {
    if (OCUDU_UNLIKELY(nof_frames != 0)) {
      nof_pending_expired_frames.fetch_add(nof_frames, std::memory_order_relaxed);
    }
  }

  ocudulog::basic_logger&                         logger;
  std::vector<std::unique_ptr<frame_buffer_pool>> pool;
  ofh::message_type                               pool_frames_type;
  ofh::data_direction                             pool_frames_data_direction;
  /// Number of frames expired in the transmission path, not yet reported by \ref clear_slot.
  std::atomic<unsigned> nof_pending_expired_frames{0};
};

} // namespace ether
} // namespace ocudu
