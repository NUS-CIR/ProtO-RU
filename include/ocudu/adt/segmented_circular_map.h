// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/expected.h"
#include "ocudu/adt/mpmc_queue.h"
#include "ocudu/adt/span.h"
#include "ocudu/support/ocudu_assert.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

namespace ocudu {

namespace detail {

/// \brief Hand-rolled pair-like type used as the stored value in segmented_circular_map.
///
/// Intentionally not std::pair which is not trivially copyable even for trivially copyable members,
/// as it supports reference members via an explicit assignment operator. References are not needed here.
///
/// \tparam K Key type.
/// \tparam V Mapped value type.
template <typename K, typename V>
struct kv_obj {
  template <typename A, typename B>
  kv_obj(A&& first_, B&& second_) : first(std::forward<A>(first_)), second(std::forward<B>(second_))
  {
  }

  template <typename A, typename... ArgsB>
  kv_obj(A&& first_, ArgsB&&... args_) : first(std::forward<A>(first_)), second(std::forward<ArgsB>(args_)...)
  {
  }

  K first;
  V second;
};

/// \brief Entry in the primary table of segmented_circular_map.
///
/// Holds a span over a segment's optional slots (non-owning view into pool backing storage)
/// and tracks the number of occupied slots. An empty (default-constructed) span
/// indicates that no segment has been acquired from the pool yet.
template <typename K, typename V>
struct segment_entry {
  /// Non-owning view of the segment; empty iff not yet acquired.
  span<std::optional<kv_obj<K, V>>> segment;
  size_t                            count = 0;
};

} // namespace detail

/// \brief Abstract interface for a shared pool of segment storage.
///
/// get_segment() returns a span over an initialised optional slot array, or an empty span
/// when the pool is exhausted. return_segment() must only be called
/// with a span previously returned by get_segment() on the same pool.
/// Implementations may be thread-safe; see shared_map_segment_pool for details.
template <typename K, typename V>
class map_segment_pool_interface
{
public:
  virtual ~map_segment_pool_interface() = default;

  /// Acquire a segment from the pool. Returns an empty span if the pool is exhausted.
  virtual span<std::optional<detail::kv_obj<K, V>>> get_segment() = 0;

  /// Return a previously acquired segment to the pool.
  virtual void return_segment(span<std::optional<detail::kv_obj<K, V>>> seg) = 0;

  /// Returns the number of slots per segment.
  virtual size_t segment_size() const = 0;
};

/// \brief Shared memory pool for segment storage across multiple value types.
///
/// Maintains one contiguous backing region:
///   - elem_slots: raw storage for N * seg_size optional elements, each slot sized and
///     aligned to accommodate the largest std::optional<kv_obj<K, V>> among all V in Vs.
///
/// Any registered value type may draw from the same underlying allocation, keeping total
/// memory usage bounded. The segment size is provided as a runtime argument to the constructor.
/// When ForcePower2SegSize is true, the constructor asserts that segment_size is a power of 2,
/// enabling segmented_circular_map instances that also set ForcePower2SegSize=true to use
/// bit-wise AND and shifts instead of division and modulo for intra-segment indexing.
///
/// Use get_pool_of_type<V>() to obtain a map_segment_pool_interface<K, V> suitable for
/// constructing a segmented_circular_map<K, V>. The call fails to compile if V was not listed
/// in Vs, guaranteeing at compile time that every user type fits in a pool slot.
///
/// The pool is non-copyable and non-movable; construct it in a stable location (heap or
/// class member) before handing out interface references.
///
/// Thread-safety: get_segment() and return_segment() are safe to call concurrently from
/// multiple threads. The number of concurrent producers (callers of return_segment()) must
/// not exceed the nof_prereserved_producers value passed to the constructor.
///
/// \tparam K                  Key type.
/// \tparam ForcePower2SegSize If true, the constructor asserts that segment_size is a power of 2.
/// \tparam Vs                 Value types sharing this pool (non-empty, pairwise distinct).
template <typename K, bool ForcePower2SegSize = true, typename... Vs>
class shared_map_segment_pool
{
  static_assert(sizeof...(Vs) > 0, "shared_map_segment_pool requires at least one value type");

  // Compile-time geometry for optional element storage (varies by V).
  static constexpr size_t elem_size  = std::max({sizeof(std::optional<detail::kv_obj<K, Vs>>)...});
  static constexpr size_t elem_align = std::max({alignof(std::optional<detail::kv_obj<K, Vs>>)...});

  struct alignas(elem_align) elem_slot {
    std::byte data[elem_size];
    elem_slot() noexcept
    {
      // user-provided (not =default) suppresses zero-initialization of data on value-init.
    }
  };

  template <typename V>
  class typed_adapter : public map_segment_pool_interface<K, V>
  {
    using opt_t = std::optional<detail::kv_obj<K, V>>;

  public:
    explicit typed_adapter(shared_map_segment_pool& p) noexcept : parent(p) {}

    size_t segment_size() const override { return parent.seg_size; }

    span<opt_t> get_segment() override
    {
      void* raw = nullptr;
      if (!parent.free_list.try_pop(raw)) {
        return {};
      }

      auto* elem_ptr = static_cast<opt_t*>(raw);
      for (size_t i = 0; i != parent.seg_size; ++i) {
        ::new (&elem_ptr[i]) opt_t();
      }
      return {elem_ptr, parent.seg_size};
    }

    void return_segment(span<opt_t> seg) override
    {
      for (size_t i = 0, e = seg.size(); i != e; ++i) {
        std::destroy_at(&seg[i]);
      }
      [[maybe_unused]] bool ok = parent.free_list.try_push(seg.data());
      ocudu_assert(ok, "shared_map_segment_pool: failed to return segment to free list");
    }

  private:
    shared_map_segment_pool& parent;
  };

  std::vector<elem_slot> elem_slots;
  concurrent_queue<void*, concurrent_queue_policy::lockfree_mpmc, concurrent_queue_wait_policy::non_blocking> free_list;
  size_t                                                                                                      seg_size;
  std::tuple<typed_adapter<Vs>...>                                                                            adapters;

public:
  explicit shared_map_segment_pool(size_t num_slots, size_t segment_size) :
    elem_slots(num_slots * segment_size),
    free_list(num_slots),
    seg_size(segment_size),
    adapters(typed_adapter<Vs>(*this)...)
  {
    if constexpr (ForcePower2SegSize) {
      report_fatal_error_if_not((segment_size & (segment_size - 1)) == 0,
                                "segment_size should be a power of 2, but a value of '{}' was used",
                                segment_size);
    }
    for (size_t i = 0; i != num_slots; ++i) {
      [[maybe_unused]] bool ok = free_list.try_push(&elem_slots[i * segment_size]);
      ocudu_assert(ok, "shared_map_segment_pool: failed to populate free list at construction");
    }
  }

  ~shared_map_segment_pool() = default;

  shared_map_segment_pool(const shared_map_segment_pool&)            = delete;
  shared_map_segment_pool& operator=(const shared_map_segment_pool&) = delete;
  shared_map_segment_pool(shared_map_segment_pool&&)                 = delete;
  shared_map_segment_pool& operator=(shared_map_segment_pool&&)      = delete;

  /// Returns the pool interface for value type V. Fails to compile if V is not in Vs.
  template <typename V>
  map_segment_pool_interface<K, V>& get_pool_of_type()
  {
    static_assert((std::is_same_v<V, Vs> || ...), "Type V was not registered in this shared pool");
    return std::get<typed_adapter<V>>(adapters);
  }
};

/// \brief Contiguous circular map with lazy per-segment allocation from a shared pool.
///
/// Functionally equivalent to circular_map, but storage is split into fixed-size segments
/// that are obtained on-demand from a shared pool and returned when they become empty.
/// This lets many map instances share a common memory pool without per-map pre-allocation.
///
/// Each segment is an span<std::optional<kv_obj<K,V>>> over pool-owned memory.
/// An empty (default-constructed) span means the segment slot has not been acquired.
///
/// Key mapping: flat = K % map_size, primary_idx = flat / seg_size, slot_idx = flat % seg_size.
/// When ForcePower2MapSize is true, the map_size modulo is replaced by a bit-wise AND.
/// When ForcePower2SegSize is true, seg_size division and modulo are replaced by a right shift
/// and bit-wise AND respectively.
/// Collision semantics are identical to circular_map: no resolution, insertion fails on collision.
/// There is no pointer or iterator invalidation.
///
/// \tparam K                  Key type (must be an unsigned integer).
/// \tparam V                  Mapped value type.
/// \tparam ForcePower2MapSize If true, map_size must be a power of 2; the key-to-flat-index
///                            conversion uses bit-wise AND instead of modulo.
/// \tparam ForcePower2SegSize If true, seg_size must be a power of 2; flat-to-segment and
///                            flat-to-slot conversions use bit shift and bit-wise AND instead of
///                            division and modulo.
template <typename K, typename V, bool ForcePower2MapSize = true, bool ForcePower2SegSize = true>
class segmented_circular_map
{
  static_assert(std::is_unsigned_v<K>, "Container key must be an unsigned integer");

public:
  using key_type        = K;
  using mapped_type     = V;
  using value_type      = std::pair<K, V>;
  using difference_type = std::ptrdiff_t;
  using obj_t           = detail::kv_obj<K, V>;

  /// Forward iterator that automatically skips absent slots, accelerating over null segments.
  class iterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = std::pair<K, V>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;

    constexpr iterator() = default;

    constexpr iterator(segmented_circular_map* map, size_t flat_idx_) : ptr(map), flat_idx(flat_idx_)
    {
      if (flat_idx < ptr->total_capacity() and not ptr->is_occupied(flat_idx)) {
        ++(*this);
      }
    }

    constexpr iterator& operator++()
    {
      ++flat_idx;
      while (flat_idx < ptr->total_capacity()) {
        size_t p = ptr->get_primary_idx(flat_idx);
        if (ptr->entries[p].segment.empty()) {
          flat_idx = ptr->get_segment_start(p + 1);
        } else if (not ptr->entries[p].segment[ptr->get_slot_idx(flat_idx)]) {
          ++flat_idx;
        } else {
          break;
        }
      }
      return *this;
    }

    constexpr obj_t& operator*()
    {
      ocudu_assert(
          flat_idx < ptr->total_capacity(), "Iterator out-of-bounds ({} >= {})", flat_idx, ptr->total_capacity());
      return ptr->get_obj(ptr->get_primary_idx(flat_idx), ptr->get_slot_idx(flat_idx));
    }

    constexpr const obj_t& operator*() const
    {
      ocudu_assert(
          flat_idx < ptr->total_capacity(), "Iterator out-of-bounds ({} >= {})", flat_idx, ptr->total_capacity());
      return ptr->get_obj(ptr->get_primary_idx(flat_idx), ptr->get_slot_idx(flat_idx));
    }

    constexpr obj_t* operator->()
    {
      ocudu_assert(
          flat_idx < ptr->total_capacity(), "Iterator out-of-bounds ({} >= {})", flat_idx, ptr->total_capacity());
      return &ptr->get_obj(ptr->get_primary_idx(flat_idx), ptr->get_slot_idx(flat_idx));
    }

    constexpr const obj_t* operator->() const
    {
      ocudu_assert(
          flat_idx < ptr->total_capacity(), "Iterator out-of-bounds ({} >= {})", flat_idx, ptr->total_capacity());
      return &ptr->get_obj(ptr->get_primary_idx(flat_idx), ptr->get_slot_idx(flat_idx));
    }

    constexpr bool operator==(const iterator& other) const { return ptr == other.ptr and flat_idx == other.flat_idx; }

    constexpr bool operator!=(const iterator& other) const { return not(*this == other); }

  private:
    friend class segmented_circular_map;
    segmented_circular_map* ptr      = nullptr;
    size_t                  flat_idx = 0;
  };

  /// Const forward iterator that automatically skips absent slots.
  class const_iterator
  {
  public:
    constexpr const_iterator() = default;

    constexpr const_iterator(const segmented_circular_map* map, size_t flat_idx_) : ptr(map), flat_idx(flat_idx_)
    {
      if (flat_idx < ptr->total_capacity() and not ptr->is_occupied(flat_idx)) {
        ++(*this);
      }
    }

    constexpr const_iterator& operator++()
    {
      ++flat_idx;
      while (flat_idx < ptr->total_capacity()) {
        size_t p = ptr->get_primary_idx(flat_idx);
        if (ptr->entries[p].segment.empty()) {
          flat_idx = ptr->get_segment_start(p + 1);
        } else if (not ptr->entries[p].segment[ptr->get_slot_idx(flat_idx)]) {
          ++flat_idx;
        } else {
          break;
        }
      }
      return *this;
    }

    constexpr const obj_t& operator*() const
    {
      return ptr->get_obj(ptr->get_primary_idx(flat_idx), ptr->get_slot_idx(flat_idx));
    }

    constexpr const obj_t* operator->() const
    {
      return &ptr->get_obj(ptr->get_primary_idx(flat_idx), ptr->get_slot_idx(flat_idx));
    }

    constexpr bool operator==(const const_iterator& other) const
    {
      return ptr == other.ptr and flat_idx == other.flat_idx;
    }

    constexpr bool operator!=(const const_iterator& other) const { return not(*this == other); }

  private:
    friend class segmented_circular_map;
    const segmented_circular_map* ptr      = nullptr;
    size_t                        flat_idx = 0;
  };

  segmented_circular_map(size_t size, map_segment_pool_interface<K, V>& pool_) :
    seg_size(pool_.segment_size()),
    seg_shift(compute_seg_shift(pool_.segment_size())),
    map_size(size),
    entries((size + seg_size - 1) / seg_size),
    pool(pool_)
  {
    ocudu_assert(size > 0, "segmented_circular_map requires at least one segment slot");
    ocudu_assert(seg_size > 0, "pool segment_size() must be greater than zero");
    if constexpr (ForcePower2MapSize) {
      report_fatal_error_if_not(
          (size & (size - 1)) == 0, "map_size should be a power of 2, but a value of '{}' was used", size);
    }
    if constexpr (ForcePower2SegSize) {
      report_fatal_error_if_not(
          (seg_size & (seg_size - 1)) == 0, "seg_size should be a power of 2, but a value of '{}' was used", seg_size);
    }
  }

  ~segmented_circular_map() { clear(); }

  segmented_circular_map(const segmented_circular_map&)            = delete;
  segmented_circular_map& operator=(const segmented_circular_map&) = delete;

  segmented_circular_map(segmented_circular_map&& other) noexcept :
    seg_size(other.seg_size),
    seg_shift(other.seg_shift),
    map_size(other.map_size),
    entries(std::move(other.entries)),
    pool(other.pool),
    elem_count(std::exchange(other.elem_count, 0U))
  {
  }

  /// Checks if there is an element with the given key in the container.
  constexpr bool contains(K key) const noexcept
  {
    size_t      flat = get_flat_idx(key);
    size_t      p    = get_primary_idx(flat);
    size_t      s    = get_slot_idx(flat);
    const auto& seg  = entries[p].segment;
    return not seg.empty() and seg[s] and seg[s]->first == key;
  }

  /// Inserts a new element constructed in-place with the given key if no collision is detected.
  /// Returns false on collision or pool exhaustion.
  template <typename... Args>
  constexpr bool emplace(K key, Args&&... args)
  {
    static_assert(std::is_constructible_v<V, Args...>, "Invalid argument types");
    size_t flat = get_flat_idx(key);
    size_t p    = get_primary_idx(flat);
    size_t s    = get_slot_idx(flat);
    if (entries[p].segment.empty()) {
      entries[p].segment = pool.get_segment();
      if (entries[p].segment.empty()) {
        return false;
      }
    } else if (entries[p].segment[s]) {
      return false;
    }
    entries[p].segment[s].emplace(key, std::forward<Args>(args)...);
    ++entries[p].count;
    ++elem_count;
    return true;
  }

  /// Inserts a new element with the given key if no collision is detected (lvalue version).
  /// Returns false on collision or pool exhaustion.
  constexpr bool insert(K key, const V& obj)
  {
    size_t flat = get_flat_idx(key);
    size_t p    = get_primary_idx(flat);
    size_t s    = get_slot_idx(flat);
    if (entries[p].segment.empty()) {
      entries[p].segment = pool.get_segment();
      if (entries[p].segment.empty()) {
        return false;
      }
    } else if (entries[p].segment[s]) {
      return false;
    }
    entries[p].segment[s].emplace(key, obj);
    ++entries[p].count;
    ++elem_count;
    return true;
  }

  /// Inserts a new element with the given key if no collision is detected (rvalue version).
  /// Returns an iterator to the inserted element or the object back on failure.
  constexpr expected<iterator, V> insert(K key, V&& obj)
  {
    size_t flat = get_flat_idx(key);
    size_t p    = get_primary_idx(flat);
    size_t s    = get_slot_idx(flat);
    if (entries[p].segment.empty()) {
      entries[p].segment = pool.get_segment();
      if (entries[p].segment.empty()) {
        return make_unexpected(std::move(obj));
      }
    } else if (entries[p].segment[s]) {
      return make_unexpected(std::move(obj));
    }
    entries[p].segment[s].emplace(key, std::move(obj));
    ++entries[p].count;
    ++elem_count;
    return iterator(this, flat);
  }

  /// Inserts a new element with the given key, overwriting any existing occupant at that slot (lvalue version).
  /// Returns false on pool exhaustion.
  constexpr bool overwrite(K key, const V& obj)
  {
    size_t flat = get_flat_idx(key);
    size_t p    = get_primary_idx(flat);
    size_t s    = get_slot_idx(flat);
    if (not entries[p].segment.empty() and entries[p].segment[s]) {
      erase(get_obj(p, s).first);
    }
    return insert(key, obj);
  }

  /// Inserts a new element with the given key, overwriting any existing occupant at that slot (rvalue version).
  /// Returns an iterator to the inserted element or the object back on pool exhaustion.
  constexpr expected<iterator, V> overwrite(K key, V&& obj)
  {
    size_t flat = get_flat_idx(key);
    size_t p    = get_primary_idx(flat);
    size_t s    = get_slot_idx(flat);
    if (not entries[p].segment.empty() and entries[p].segment[s]) {
      erase(get_obj(p, s).first);
    }
    return insert(key, std::move(obj));
  }

  /// Removes the element with the given key. Returns false if not found.
  constexpr bool erase(K key) noexcept
  {
    if (not contains(key)) {
      return false;
    }
    size_t flat = get_flat_idx(key);
    size_t p    = get_primary_idx(flat);
    size_t s    = get_slot_idx(flat);
    entries[p].segment[s].reset();
    --entries[p].count;
    --elem_count;
    maybe_return_segment(p);
    return true;
  }

  /// Removes the element pointed to by the iterator. Returns the next iterator.
  constexpr iterator erase(iterator it) noexcept
  {
    ocudu_assert(it.flat_idx < total_capacity() and it.ptr == this,
                 "Iterator out-of-bounds ({} >= {})",
                 it.flat_idx,
                 total_capacity());
    iterator next = it;
    ++next;
    size_t p = get_primary_idx(it.flat_idx);
    size_t s = get_slot_idx(it.flat_idx);
    entries[p].segment[s].reset();
    --entries[p].count;
    --elem_count;
    maybe_return_segment(p);
    return next;
  }

  /// Erases all elements and returns all segments to the pool.
  constexpr void clear() noexcept
  {
    for (auto& entry : entries) {
      if (not entry.segment.empty()) {
        pool.return_segment(entry.segment);
        entry.segment = {};
        entry.count   = 0;
      }
    }
    elem_count = 0;
  }

  /// Returns a reference to the value mapped to the given key. Asserts if not present.
  constexpr V& operator[](K key) noexcept
  {
    ocudu_assert(contains(key), "Accessing non-existent ID={}", (size_t)key);
    size_t flat = get_flat_idx(key);
    return get_obj(get_primary_idx(flat), get_slot_idx(flat)).second;
  }

  /// Returns a const reference to the value mapped to the given key. Asserts if not present.
  constexpr const V& operator[](K key) const noexcept
  {
    ocudu_assert(contains(key), "Accessing non-existent ID={}", (size_t)key);
    size_t flat = get_flat_idx(key);
    return get_obj(get_primary_idx(flat), get_slot_idx(flat)).second;
  }

  /// Returns the number of elements in the container.
  constexpr size_t size() const noexcept { return elem_count; }

  /// Checks if the container has no elements.
  constexpr bool empty() const noexcept { return elem_count == 0; }

  /// Checks if the container has reached its maximum capacity.
  constexpr bool full() const noexcept { return elem_count == capacity(); }

  /// Returns the maximum capacity of the container.
  constexpr size_t capacity() const noexcept { return total_capacity(); }

  /// Checks if the slot for the given key is free (either segment absent or slot unoccupied).
  constexpr bool has_space(K key) const noexcept
  {
    size_t flat = get_flat_idx(key);
    size_t p    = get_primary_idx(flat);
    size_t s    = get_slot_idx(flat);
    return entries[p].segment.empty() or not entries[p].segment[s];
  }

  constexpr iterator       begin() { return iterator(this, 0); }
  constexpr iterator       end() { return iterator(this, total_capacity()); }
  constexpr const_iterator begin() const { return const_iterator(this, 0); }
  constexpr const_iterator end() const { return const_iterator(this, total_capacity()); }

  /// Finds an element with the given key.
  constexpr iterator find(K key)
  {
    if (contains(key)) {
      return iterator(this, get_flat_idx(key));
    }
    return end();
  }

  /// Finds an element with the given key.
  constexpr const_iterator find(K key) const
  {
    if (contains(key)) {
      return const_iterator(this, get_flat_idx(key));
    }
    return end();
  }

private:
  size_t total_capacity() const noexcept { return map_size; }

  /// Converts key to flat index: key % map_size, or key & (map_size-1) when ForcePower2MapSize.
  size_t get_flat_idx(K key) const noexcept
  {
    if constexpr (ForcePower2MapSize) {
      return static_cast<size_t>(key) & (map_size - 1);
    } else {
      return static_cast<size_t>(key) % map_size;
    }
  }

  /// Returns the segment (primary) index from a flat index: flat / seg_size, or flat >> seg_shift.
  size_t get_primary_idx(size_t flat) const noexcept
  {
    if constexpr (ForcePower2SegSize) {
      return flat >> seg_shift;
    } else {
      return flat / seg_size;
    }
  }

  /// Returns the slot index within a segment from a flat index: flat % seg_size, or flat & (seg_size-1).
  size_t get_slot_idx(size_t flat) const noexcept
  {
    if constexpr (ForcePower2SegSize) {
      return flat & (seg_size - 1);
    } else {
      return flat % seg_size;
    }
  }

  /// Returns the flat index of the first slot of a segment: primary * seg_size, or primary << seg_shift.
  size_t get_segment_start(size_t primary) const noexcept
  {
    if constexpr (ForcePower2SegSize) {
      return primary << seg_shift;
    } else {
      return primary * seg_size;
    }
  }

  obj_t&       get_obj(size_t primary, size_t slot) { return *entries[primary].segment[slot]; }
  const obj_t& get_obj(size_t primary, size_t slot) const { return *entries[primary].segment[slot]; }

  bool is_occupied(size_t flat) const noexcept
  {
    size_t p = get_primary_idx(flat);
    size_t s = get_slot_idx(flat);
    return not entries[p].segment.empty() and entries[p].segment[s].has_value();
  }

  void maybe_return_segment(size_t primary) noexcept
  {
    if (entries[primary].count == 0) {
      pool.return_segment(entries[primary].segment);
      entries[primary].segment = {};
    }
  }

  /// Returns __builtin_ctzll(seg_sz) when ForcePower2SegSize is true, 0 otherwise.
  static size_t compute_seg_shift(size_t seg_sz) noexcept
  {
    if constexpr (ForcePower2SegSize) {
      return static_cast<size_t>(__builtin_ctzll(static_cast<unsigned long long>(seg_sz)));
    }
    return 0;
  }

  size_t                                   seg_size;
  size_t                                   seg_shift;
  size_t                                   map_size;
  std::vector<detail::segment_entry<K, V>> entries;
  map_segment_pool_interface<K, V>&        pool;
  size_t                                   elem_count = 0;
};

} // namespace ocudu
