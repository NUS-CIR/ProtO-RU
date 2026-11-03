// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_data_flow_cplane_scheduling_commands.h"
#include "ofh_tx_window_checker.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_metrics.h"

namespace ocudu {
namespace ofh {

/// Open Fronthaul uplink request handler metrics collector.
class uplink_request_handler_metrics_collector
{
public:
  uplink_request_handler_metrics_collector(data_flow_message_encoding_metrics_collector* df_metrics_collector_,
                                           tx_window_checker&                            window_checker_) :
    df_metrics_collector(df_metrics_collector_), window_checker(window_checker_)
  {
  }

  /// Updates the number of late messages (messages that were not transmitted) of the Control-Plane uplink by the
  /// given value.
  void update_cp_ul_lates(unsigned nof_lates) { nof_late_cp_ul.fetch_add(nof_lates, std::memory_order_relaxed); }

  /// Increments the number of uplink requests that could not be dispatched to the processing executor.
  void increment_dispatch_failures() { nof_dispatch_failures.fetch_add(1, std::memory_order_relaxed); }

  /// Collects uplink request handler metrics.
  void collect_metrics(transmitter_ul_metrics& metrics)
  {
    if (df_metrics_collector) {
      df_metrics_collector->collect_metrics(metrics.ul_cp_metrics);
    }
    metrics.ul_cp_metrics.nof_dispatch_failures = nof_dispatch_failures.exchange(0, std::memory_order_relaxed);
    metrics.nof_late_cp_ul                      = nof_late_cp_ul.exchange(0, std::memory_order_relaxed);
    window_checker.collect_arrival_metrics(metrics.ul_request_arrival);
  }

private:
  data_flow_message_encoding_metrics_collector* df_metrics_collector;
  tx_window_checker&                            window_checker;
  std::atomic<uint32_t>                         nof_late_cp_ul        = {0};
  std::atomic<uint32_t>                         nof_dispatch_failures = {0};
};

} // namespace ofh
} // namespace ocudu
