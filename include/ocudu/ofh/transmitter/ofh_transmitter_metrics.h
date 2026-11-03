// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ofh/ethernet/ethernet_transmitter_metrics.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_data_flow_metrics.h"

namespace ocudu {
namespace ofh {

/// Arrival time of the DL/UL requests that reached the OFH transmitter, measured with respect to an OTA slot.
struct tx_request_arrival_metrics {
  /// Earliest request, i.e., the biggest advance in number of symbols before the slot start.
  int earliest_request_in_symbols = 0;
  /// Latest request, i.e., the smallest advance in number of symbols before the slot start.
  int latest_request_in_symbols = 0;
  /// Average number of symbols before the slot start.
  float average_request_in_symbols = 0.0F;
  /// Number of late requests, i.e., requests that arrived after the transmission window closed.
  unsigned nof_late_requests = 0;
};

/// Open Fronthaul transmitter downlink processing metrics.
struct transmitter_dl_metrics {
  /// OFH transmitter downlink C-Plane processing metrics.
  tx_data_flow_perf_metrics dl_cp_metrics;
  /// OFH transmitter U-Plane processing metrics.
  tx_data_flow_perf_metrics dl_up_metrics;
  /// Arrival time and number of late downlink resource grids received from the PHY.
  tx_request_arrival_metrics dl_grid_arrival;
  /// Number of late Control-Plane downlink messages, ie, messages that were not transmitted.
  unsigned nof_late_cp_dl;
  /// Number of late User-Plane downlink messages, ie, messages that were not transmitted.
  unsigned nof_late_up_dl;
};

/// Open Fronthaul transmitter uplink requests processing metrics.
struct transmitter_ul_metrics {
  /// Open Fronthaul transmitter uplink C-Plane processing metrics.
  tx_data_flow_perf_metrics ul_cp_metrics;
  /// Arrival time and number of late uplink requests.
  tx_request_arrival_metrics ul_request_arrival;
  /// Number of late Control-Plane uplink messages, ie, messages that were not transmitted.
  unsigned nof_late_cp_ul;
};

/// Open Fronthaul message transmitter metrics.
struct message_transmitter_metrics {
  /// Message enqueueing and transmission statistics.
  float message_tx_min_latency_us;
  float message_tx_max_latency_us;
  float message_tx_avg_latency_us;

  /// CPU usage in microseconds of the message transmitter processing.
  float cpu_usage_us;
};

/// Open Fronthaul transmitter metrics.
struct transmitter_metrics {
  transmitter_dl_metrics      dl_metrics;
  transmitter_ul_metrics      ul_metrics;
  message_transmitter_metrics message_tx_metrics;
  ether::transmitter_metrics  eth_transmitter_metrics;
};

} // namespace ofh
} // namespace ocudu
