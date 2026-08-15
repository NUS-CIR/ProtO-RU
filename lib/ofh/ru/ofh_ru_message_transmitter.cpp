// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_message_transmitter.h"
#include "ocudu/adt/static_vector.h"

using namespace ocudu;
using namespace ofh;

ru_message_transmitter::ru_message_transmitter(const ru_message_transmitter_config&  config,
                                               ru_message_transmitter_dependencies&& dependencies) :
  logger(*dependencies.logger),
  eth_transmitter(std::move(dependencies.eth_transmitter)),
  uplink_uplane_pool(std::move(dependencies.uplink_uplane_pool)),
  prach_pool(std::move(dependencies.prach_pool)),
  tx_window_start(static_cast<int>(config.tx_window_start_symbols)),
  tx_window_end(static_cast<int>(config.tx_window_end_symbols)),
  sector_id(config.sector)
{
  ocudu_assert(eth_transmitter, "Invalid Ethernet transmitter");
  ocudu_assert(uplink_uplane_pool, "Invalid uplink User-Plane frame pool");
  ocudu_assert(prach_pool, "Invalid PRACH frame pool");
}

void ru_message_transmitter::on_new_symbol(const slot_symbol_point_context& symbol_point_context)
{
  static_vector<ether::scoped_frame_buffer, ether::MAX_TX_BURST_SIZE> read_frames;

  // Collect the uplink User-Plane and PRACH frames whose transmit window is open: the Ta3 window opens AFTER the
  // symbol's air time (unlike the O-DU's downlink, transmitted ahead of air), so at OTA symbol T the due symbols are
  // the past ones in [T - Ta3_max, T - Ta3_min].
  ether::frame_pool_interval interval{symbol_point_context.symbol_point - tx_window_start,
                                      symbol_point_context.symbol_point - tx_window_end};
  uplink_uplane_pool->enqueue_pending_into_burst(interval, read_frames);
  prach_pool->enqueue_pending_into_burst(interval, read_frames);

  if (read_frames.empty()) {
    return;
  }

  static_vector<span<const uint8_t>, ether::MAX_TX_BURST_SIZE> frame_burst;
  for (const auto& frame : read_frames) {
    frame_burst.emplace_back(frame->data());
  }

  eth_transmitter->send(frame_burst);
}
