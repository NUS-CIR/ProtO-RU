// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/ru/ofh_ru_message_transmitter.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/cyclic_prefix.h"
#include <gtest/gtest.h>
#include <tuple>
#include <utility>
#include <vector>

using namespace ocudu;
using namespace ofh;

namespace {

/// Ethernet transmitter spy counting the frames sent.
class eth_transmitter_spy : public ether::transmitter
{
public:
  unsigned              nof_frames = 0;
  std::vector<unsigned> burst_sizes;

  void send(span<span<const uint8_t>> frames) override
  {
    nof_frames += frames.size();
    burst_sizes.push_back(frames.size());
  }
  ether::transmitter_metrics_collector* get_metrics_collector() override { return nullptr; }
};

/// Builds a transmitter with the given Ta3 window and frame-pool capacity, returning the Ethernet spy.
ru_message_transmitter make_transmitter(eth_transmitter_spy*&                   eth_out,
                                        std::shared_ptr<ether::eth_frame_pool>& ul_pool_out,
                                        std::shared_ptr<ether::eth_frame_pool>& prach_pool_out,
                                        ru_message_transmitter_config           config = {0, 8, 3},
                                        unsigned nof_slots = ether::eth_frame_pool::default_pool_size_in_slots())
{
  auto& logger = ocudulog::fetch_basic_logger("TEST");

  ul_pool_out = std::make_shared<ether::eth_frame_pool>(
      logger, units::bytes(9000), 2, message_type::user_plane, data_direction::uplink, nof_slots);
  prach_pool_out = std::make_shared<ether::eth_frame_pool>(
      logger, units::bytes(9000), 2, message_type::uplane_prach, data_direction::uplink, nof_slots);

  auto eth = std::make_unique<eth_transmitter_spy>();
  eth_out  = eth.get();

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

class ru_message_transmitter_burst_test : public ::testing::TestWithParam<std::pair<unsigned, unsigned>>
{};

class ru_message_transmitter_timing_test :
  public ::testing::TestWithParam<std::tuple<subcarrier_spacing, unsigned, bool>>
{};

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

TEST_P(ru_message_transmitter_burst_test, all_due_frames_are_sent_before_the_window_closes)
{
  eth_transmitter_spy*                   eth = nullptr;
  std::shared_ptr<ether::eth_frame_pool> ul_pool;
  std::shared_ptr<ether::eth_frame_pool> prach_pool;
  ru_message_transmitter                 transmitter = make_transmitter(eth, ul_pool, prach_pool);
  const auto                             air_symbol  = make_symbol_point(5, 0);
  const auto [nof_ul_frames, nof_prach_frames]       = GetParam();

  // The pool reserves two frames per eAxC and doubles that capacity for data and PRACH.
  constexpr unsigned frames_per_symbol = 4 * MAX_NOF_SUPPORTED_EAXC;
  for (auto [pool, nof_frames] :
       {std::pair{ul_pool.get(), nof_ul_frames}, std::pair{prach_pool.get(), nof_prach_frames}}) {
    for (unsigned i = 0; i != nof_frames; ++i) {
      enqueue_frame(*pool, air_symbol + i / frames_per_symbol);
    }
  }

  // This tick is the final send opportunity for the oldest frames in both pools.
  transmitter.on_new_symbol({air_symbol + 8, {}});
  ASSERT_EQ(eth->nof_frames, nof_ul_frames + nof_prach_frames);
  for (unsigned burst_size : eth->burst_sizes) {
    EXPECT_GT(burst_size, 0U);
    EXPECT_LE(burst_size, ether::MAX_TX_BURST_SIZE);
  }

  // Later ticks must not resend frames that have already been drained.
  for (unsigned i = 9; i != 20; ++i) {
    transmitter.on_new_symbol({air_symbol + i, {}});
  }
  EXPECT_EQ(eth->nof_frames, nof_ul_frames + nof_prach_frames);
}

INSTANTIATE_TEST_SUITE_P(multiple_bursts,
                         ru_message_transmitter_burst_test,
                         ::testing::Values(std::pair{ether::MAX_TX_BURST_SIZE, 1U},
                                           std::pair{ether::MAX_TX_BURST_SIZE + 1, 1U},
                                           std::pair{1U, ether::MAX_TX_BURST_SIZE + 1},
                                           std::pair{ether::MAX_TX_BURST_SIZE, ether::MAX_TX_BURST_SIZE},
                                           std::pair{ether::MAX_TX_BURST_SIZE, 0U}));

TEST_P(ru_message_transmitter_timing_test, fresh_frames_survive_until_the_five_ms_window_opens)
{
  const auto [scs, boundary, zero_width_window] = GetParam();
  const unsigned          nof_symbols           = get_nsymb_per_slot(cyclic_prefix::NORMAL);
  const unsigned          nof_slots_per_frame   = NOF_SUBFRAMES_PER_FRAME * get_nof_slots_per_subframe(scs);
  const unsigned          symbols_per_ms        = nof_symbols * get_nof_slots_per_subframe(scs);
  const unsigned          window_start          = 5 * symbols_per_ms;
  const unsigned          window_end            = (zero_width_window ? 5 : 4) * symbols_per_ms;
  const unsigned          air_slot              = boundary == 0 ? 2 * nof_slots_per_frame
                                                                : (boundary == 1 ? nof_slots_per_frame : NOF_SFNS * nof_slots_per_frame) - 1;
  const slot_symbol_point air{slot_point(scs, air_slot), nof_symbols - 1, nof_symbols};

  eth_transmitter_spy*                   eth = nullptr;
  std::shared_ptr<ether::eth_frame_pool> ul_pool;
  std::shared_ptr<ether::eth_frame_pool> prach_pool;
  auto transmitter = make_transmitter(eth, ul_pool, prach_pool, {0, window_start, window_end}, nof_slots_per_frame);

  for (auto* pool : {ul_pool.get(), prach_pool.get()}) {
    enqueue_frame(*pool, air);
    enqueue_frame(*pool, air + 1);
  }

  // Immediate replies can enter the pool before air time; neither pool may discard them during earlier scans.
  for (int elapsed = -static_cast<int>(nof_symbols); elapsed < static_cast<int>(window_end); ++elapsed) {
    transmitter.on_new_symbol({air + elapsed, {}});
    ASSERT_EQ(eth->nof_frames, 0U);
  }

  transmitter.on_new_symbol({air + window_end, {}});
  EXPECT_EQ(eth->nof_frames, 2U);
  transmitter.on_new_symbol({air + window_end + 1, {}});
  EXPECT_EQ(eth->nof_frames, 4U);

  for (unsigned elapsed = window_end + 2; elapsed <= window_start + 1; ++elapsed) {
    transmitter.on_new_symbol({air + elapsed, {}});
  }
  EXPECT_EQ(eth->nof_frames, 4U);
}

INSTANTIATE_TEST_SUITE_P(five_ms_window,
                         ru_message_transmitter_timing_test,
                         ::testing::Combine(::testing::Values(subcarrier_spacing::kHz15, subcarrier_spacing::kHz30),
                                            ::testing::Values(0U, 1U, 2U),
                                            ::testing::Bool()));
