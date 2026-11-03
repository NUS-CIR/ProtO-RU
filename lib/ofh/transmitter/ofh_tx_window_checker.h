// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../support/metrics_helpers.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/timing/ofh_ota_symbol_boundary_notifier.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_metrics.h"
#include <atomic>
#include <limits>
#include <string_view>

namespace ocudu {
namespace ofh {

/// Open Fronthaul transmission window checker.
class tx_window_checker : public ota_symbol_boundary_notifier
{
public:
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
    slot_symbol_point slot_start_point(slot, 0, nof_symbols);
    update_arrival_stats(slot_start_point - ota_symbol_point);

    slot_symbol_point rg_point = slot_start_point - advance_time_in_symbols;

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

  /// Collects the request arrival statistics and the number of late requests since the last call and resets them.
  void collect_arrival_metrics(tx_request_arrival_metrics& metrics)
  {
    const int32_t  latest   = latest_arrival.exchange(ARRIVAL_LATEST, std::memory_order_relaxed);
    const int32_t  earliest = earliest_arrival.exchange(ARRIVAL_EARLIEST, std::memory_order_relaxed);
    const int64_t  sum      = arrival_sum.exchange(0, std::memory_order_relaxed);
    const uint32_t count    = arrival_count.exchange(0, std::memory_order_relaxed);

    metrics                   = {};
    metrics.nof_late_requests = late_counter.exchange(0, std::memory_order_relaxed);
    if (count == 0) {
      return;
    }
    metrics.earliest_request_in_symbols = earliest;
    metrics.latest_request_in_symbols   = latest;
    metrics.average_request_in_symbols  = static_cast<float>(sum) / static_cast<float>(count);
  }

private:
  static constexpr int32_t ARRIVAL_EARLIEST = std::numeric_limits<int32_t>::min();
  static constexpr int32_t ARRIVAL_LATEST   = std::numeric_limits<int32_t>::max();

  /// Updates the arrival statistics with the number of symbols between a request arrival and its slot start.
  void update_arrival_stats(int32_t nof_symbols_before_ota)
  {
    update_minmax(nof_symbols_before_ota, earliest_arrival, latest_arrival);
    arrival_sum.fetch_add(nof_symbols_before_ota, std::memory_order_relaxed);
    arrival_count.fetch_add(1, std::memory_order_relaxed);
  }

  ocudulog::basic_logger& logger;
  const unsigned          sector_id;
  const std::string_view  direction;
  const uint32_t          advance_time_in_symbols;
  const uint32_t          nof_symbols;
  std::atomic<uint32_t>   count_val{0};
  std::atomic<uint32_t>   late_counter{0};
  std::atomic<int32_t>    earliest_arrival{ARRIVAL_EARLIEST};
  std::atomic<int32_t>    latest_arrival{ARRIVAL_LATEST};
  std::atomic<int64_t>    arrival_sum{0};
  std::atomic<uint32_t>   arrival_count{0};
};

} // namespace ofh
} // namespace ocudu
