// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/ru/ofh_ru_message_transmitter.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/cyclic_prefix.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

namespace {

/// Ethernet transmitter spy counting the frames sent.
class eth_transmitter_spy : public ether::transmitter
{
public:
  unsigned nof_frames = 0;

  void                                  send(span<span<const uint8_t>> frames) override { nof_frames += frames.size(); }
  ether::transmitter_metrics_collector* get_metrics_collector() override { return nullptr; }
};

/// Builds a transmitter with a Ta3 window of [3, 8] symbols after air time, returning the Ethernet spy.
ru_message_transmitter make_transmitter(eth_transmitter_spy*&                   eth_out,
                                        std::shared_ptr<ether::eth_frame_pool>& ul_pool_out,
                                        std::shared_ptr<ether::eth_frame_pool>& prach_pool_out)
{
  auto& logger = ocudulog::fetch_basic_logger("TEST");

  ul_pool_out = std::make_shared<ether::eth_frame_pool>(
      logger, units::bytes(9000), 2, message_type::user_plane, data_direction::uplink);
  prach_pool_out = std::make_shared<ether::eth_frame_pool>(
      logger, units::bytes(9000), 2, message_type::uplane_prach, data_direction::uplink);

  auto eth = std::make_unique<eth_transmitter_spy>();
  eth_out  = eth.get();

  ru_message_transmitter_config config;
  config.sector                  = 0;
  config.tx_window_start_symbols = 8;
  config.tx_window_end_symbols   = 3;

  ru_message_transmitter_dependencies deps;
  deps.logger             = &logger;
  deps.eth_transmitter    = std::move(eth);
  deps.uplink_uplane_pool = ul_pool_out;
  deps.prach_pool         = prach_pool_out;

  return {config, std::move(deps)};
}

/// Enqueues one pending frame into the pool for the given symbol point.
void enqueue_frame(ether::eth_frame_pool& pool, slot_symbol_point symbol_point)
{
  auto buffer = pool.reserve(symbol_point);
  ASSERT_TRUE(buffer);
  buffer->set_size(64);
}

slot_symbol_point make_symbol_point(unsigned slot, unsigned symbol)
{
  return {slot_point(to_numerology_value(subcarrier_spacing::kHz30), slot),
          symbol,
          get_nsymb_per_slot(cyclic_prefix::NORMAL)};
}

} // namespace

TEST(ru_message_transmitter_test, frame_is_sent_when_its_window_after_air_time_opens)
{
  eth_transmitter_spy*                   eth = nullptr;
  std::shared_ptr<ether::eth_frame_pool> ul_pool;
  std::shared_ptr<ether::eth_frame_pool> prach_pool;
  ru_message_transmitter                 transmitter = make_transmitter(eth, ul_pool, prach_pool);

  // An uplink User-Plane reply captured for slot 5, symbol 0.
  slot_symbol_point air_symbol = make_symbol_point(5, 0);
  enqueue_frame(*ul_pool, air_symbol);

  // Before the Ta3 window opens (less than 3 symbols past air time): nothing is sent. In particular, the O-DU's
  // downlink convention (transmitting AHEAD of air time) must not apply: ticking before air sends nothing.
  transmitter.on_new_symbol({make_symbol_point(4, 6), {}});
  ASSERT_EQ(eth->nof_frames, 0);
  transmitter.on_new_symbol({air_symbol, {}});
  ASSERT_EQ(eth->nof_frames, 0);

  // Within the window [air + 3, air + 8] symbols the frame goes out.
  transmitter.on_new_symbol({make_symbol_point(5, 4), {}});
  ASSERT_EQ(eth->nof_frames, 1);
}

TEST(ru_message_transmitter_test, prach_frames_are_drained_from_their_own_pool)
{
  eth_transmitter_spy*                   eth = nullptr;
  std::shared_ptr<ether::eth_frame_pool> ul_pool;
  std::shared_ptr<ether::eth_frame_pool> prach_pool;
  ru_message_transmitter                 transmitter = make_transmitter(eth, ul_pool, prach_pool);

  slot_symbol_point air_symbol = make_symbol_point(7, 2);
  enqueue_frame(*prach_pool, air_symbol);

  transmitter.on_new_symbol({make_symbol_point(7, 7), {}});
  ASSERT_EQ(eth->nof_frames, 1);
}
