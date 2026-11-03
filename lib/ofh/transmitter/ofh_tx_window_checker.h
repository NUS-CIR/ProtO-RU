// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/timing/ofh_ota_symbol_boundary_notifier.h"
#include <atomic>
#include <string_view>

namespace ocudu {
namespace ofh {

/// Open Fronthaul transmission window checker.
class tx_window_checker : public ota_symbol_boundary_notifier
{
public:
  /// \param[in] direction_ Name of the checked request type used in log messages, e.g. "downlink" or "uplink".
  tx_window_checker(ocudulog::basic_logger& logger_,
                    unsigned                sector_id_,
                    std::string_view        direction_,
                    uint32_t                advance_time_in_symbols_,
                    uint32_t                nof_symbols_) :
    logger(logger_),
    sector_id(sector_id_),
    direction(direction_),
    advance_time_in_symbols(advance_time_in_symbols_),
    nof_symbols(nof_symbols_)
  {
  }

  // See interface for documentation.
  void on_new_symbol(const slot_symbol_point_context& symbol_point_context) override
  {
    // This atomic is only written from a single thread.
    count_val.store(symbol_point_context.symbol_point.to_uint(), std::memory_order::memory_order_relaxed);
  }

  /// Returns true if the given slot is already late compared to the current OTA time, otherwise false.
  bool is_late(slot_point slot)
  {
    slot_symbol_point ota_symbol_point(
        slot.numerology(), count_val.load(std::memory_order::memory_order_relaxed), nof_symbols);

    // Use symbol 0 as the worst case for the resource grid slot.
    slot_symbol_point rg_point(slot, 0, nof_symbols);
    rg_point -= advance_time_in_symbols;

    if (ota_symbol_point < rg_point) {
      return false;
    }

    if (OCUDU_UNLIKELY(logger.debug.enabled())) {
      logger.debug("Sector#{}: {} request for slot '{}_{}' arrived after the transmission window closed, current "
                   "ota_slot='{}_{}', the request must arrive at least '{}' symbols before this slot",
                   sector_id,
                   direction,
                   slot,
                   0,
                   ota_symbol_point.get_slot(),
                   ota_symbol_point.get_symbol_index(),
                   advance_time_in_symbols);
    }

    late_counter.fetch_add(1, std::memory_order_relaxed);
    return true;
  }

  uint32_t get_nof_lates_and_reset() { return late_counter.exchange(0, std::memory_order_relaxed); }

private:
  ocudulog::basic_logger& logger;
  const unsigned          sector_id;
  const std::string_view  direction;
  const uint32_t          advance_time_in_symbols;
  const uint32_t          nof_symbols;
  std::atomic<uint32_t>   count_val{0};
  std::atomic<uint32_t>   late_counter{0};
};

} // namespace ofh
} // namespace ocudu
