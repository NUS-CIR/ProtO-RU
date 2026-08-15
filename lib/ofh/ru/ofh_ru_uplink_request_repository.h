// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../support/context_repository_helpers.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/serdes/ofh_cplane_message_properties.h"
#include "ocudu/ran/prach/prach_subcarrier_spacing.h"
#include <array>
#include <mutex>
#include <optional>
#include <vector>

namespace ocudu {
namespace ofh {

/// Uplink Control-Plane request recorded by an O-RU, pending a User-Plane reply.
struct ru_uplink_request {
  /// Slot the request was scheduled for.
  slot_point slot;
  /// Filter index.
  filter_index_type filter_index;
  /// Start symbol identifier.
  uint8_t start_symbol;
  /// Starting PRB of the data section.
  uint16_t prb_start;
  /// Number of contiguous PRBs per data section.
  uint16_t nof_prb;
  /// Number of symbols.
  uint8_t nof_symbols;
  /// Section identifier, echoed back in the User-Plane reply.
  uint16_t section_id;
  /// PRACH subcarrier spacing decoded from the section type 3 frameStructure field (unused for regular uplink).
  prach_subcarrier_spacing prach_scs = prach_subcarrier_spacing::invalid;
  /// PRACH allocation offset in PUSCH-grid RBs, derived from the section type 3 frequencyOffset.
  unsigned prach_start_rb = 0;
};

/// \brief Repository of uplink Control-Plane requests recorded by an O-RU.
///
/// Written by the uplink scheduling recorder when an uplink/PRACH Control-Plane request is received and consumed by
/// the symbol-driven responders once the requested IQ has been captured. Unlike the O-DU's uplink Control-Plane
/// context repository, entries carry the slot they were recorded for and \ref get only returns exact matches, and
/// the responders \ref clear an entry once it has been answered: a request must produce exactly one reply, even when
/// the O-DU stops sending Control-Plane (a stale entry must not generate uplink traffic forever).
class ru_uplink_request_repository
{
  /// Entry per eAxC; \c nof_symbols == 0 marks an empty entry.
  using repo_entry = std::array<ru_uplink_request, MAX_SUPPORTED_EAXC_ID_VALUE>;

  std::vector<repo_entry> repo;
  mutable std::mutex      mutex;

  /// Returns the entry of the repository for the given slot and eAxC.
  ru_uplink_request& entry(slot_point slot, unsigned eaxc)
  {
    unsigned index = calculate_repository_index(slot, repo.size());
    return repo[index][eaxc];
  }

  /// Returns the entry of the repository for the given slot and eAxC.
  const ru_uplink_request& entry(slot_point slot, unsigned eaxc) const
  {
    unsigned index = calculate_repository_index(slot, repo.size());
    return repo[index][eaxc];
  }

public:
  explicit ru_uplink_request_repository(unsigned size_) : repo(size_) {}

  /// Records the given request at its slot and the given eAxC.
  void add(unsigned eaxc, const ru_uplink_request& request)
  {
    std::lock_guard<std::mutex> lock(mutex);
    entry(request.slot, eaxc) = request;
  }

  /// Returns the request recorded for exactly the given slot and eAxC, if any.
  std::optional<ru_uplink_request> get(slot_point slot, unsigned eaxc) const
  {
    std::lock_guard<std::mutex> lock(mutex);
    const ru_uplink_request&    request = entry(slot, eaxc);
    if ((request.nof_symbols != 0) && (request.slot == slot)) {
      return request;
    }
    return std::nullopt;
  }

  /// Clears the request recorded for the given slot and eAxC (a no-op if the entry holds a different slot).
  void clear(slot_point slot, unsigned eaxc)
  {
    std::lock_guard<std::mutex> lock(mutex);
    ru_uplink_request&          request = entry(slot, eaxc);
    if (request.slot == slot) {
      request.nof_symbols = 0;
    }
  }
};

} // namespace ofh
} // namespace ocudu
