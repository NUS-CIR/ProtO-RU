// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/ru/ofh_ru_uplink_scheduling_recorder.h"
#include "ocudu/ocudulog/ocudulog.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

namespace {

cplane_message_decoder_results make_results(slot_point        slot,
                                            uint8_t           start_symbol,
                                            uint8_t           nof_symbols,
                                            uint16_t          prb_start,
                                            uint16_t          nof_prb,
                                            filter_index_type filter_index,
                                            uint16_t          section_id       = 0,
                                            cplane_scs        prach_scs        = cplane_scs::kHz30,
                                            int               frequency_offset = 0)
{
  cplane_message_decoder_results results;
  results.radio_hdr.slot         = slot;
  results.radio_hdr.start_symbol = start_symbol;
  results.radio_hdr.filter_index = filter_index;
  results.section.prb_start      = prb_start;
  results.section.nof_prb        = nof_prb;
  results.section.nof_symbols    = nof_symbols;
  results.section.section_id     = section_id;
  results.frame_structure_scs    = prach_scs;
  results.frequency_offset       = frequency_offset;
  return results;
}

ru_uplink_scheduling_recorder_config make_config()
{
  return {/*sector=*/0, subcarrier_spacing::kHz30, /*ru_nof_prbs=*/51};
}

} // namespace

TEST(ru_uplink_scheduling_recorder_test, uplink_request_is_recorded_in_uplink_repository)
{
  auto                          ul_repo    = std::make_shared<ru_uplink_request_repository>(40);
  auto                          prach_repo = std::make_shared<ru_uplink_request_repository>(40);
  ru_uplink_scheduling_recorder recorder(make_config(), ocudulog::fetch_basic_logger("TEST"), ul_repo, prach_repo);

  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 5);
  recorder.handle_uplink_scheduling(
      /*eaxc=*/2,
      make_results(slot,
                   /*start=*/4,
                   /*nof_sym=*/10,
                   /*prb_start=*/1,
                   /*nof_prb=*/51,
                   filter_index_type::standard_channel_filter,
                   /*section_id=*/3));

  std::optional<ru_uplink_request> request = ul_repo->get(slot, 2);
  ASSERT_TRUE(request.has_value());
  ASSERT_EQ(request->slot, slot);
  ASSERT_EQ(request->start_symbol, 4);
  ASSERT_EQ(request->nof_symbols, 10);
  ASSERT_EQ(request->prb_start, 1);
  ASSERT_EQ(request->nof_prb, 51);
  ASSERT_EQ(request->filter_index, filter_index_type::standard_channel_filter);
  ASSERT_EQ(request->section_id, 3);

  // Nothing recorded in the PRACH repository.
  ASSERT_FALSE(prach_repo->get(slot, 2).has_value());
}

TEST(ru_uplink_scheduling_recorder_test, prach_request_is_recorded_in_prach_repository)
{
  auto                          ul_repo    = std::make_shared<ru_uplink_request_repository>(40);
  auto                          prach_repo = std::make_shared<ru_uplink_request_repository>(40);
  ru_uplink_scheduling_recorder recorder(make_config(), ocudulog::fetch_basic_logger("TEST"), ul_repo, prach_repo);

  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 7);
  recorder.handle_prach_scheduling(
      /*eaxc=*/4,
      make_results(slot,
                   /*start=*/0,
                   /*nof_sym=*/12,
                   /*prb_start=*/0,
                   /*nof_prb=*/12,
                   filter_index_type::ul_prach_preamble_short,
                   /*section_id=*/1,
                   cplane_scs::kHz30,
                   /*frequency_offset for rb_offset 6=*/-468));

  std::optional<ru_uplink_request> request = prach_repo->get(slot, 4);
  ASSERT_TRUE(request.has_value());
  ASSERT_EQ(request->start_symbol, 0);
  ASSERT_EQ(request->nof_symbols, 12);
  ASSERT_EQ(request->filter_index, filter_index_type::ul_prach_preamble_short);
  ASSERT_EQ(request->section_id, 1);
  ASSERT_EQ(request->prach_scs, prach_subcarrier_spacing::kHz30);
  ASSERT_EQ(request->prach_start_rb, 6);

  // Nothing recorded in the uplink repository.
  ASSERT_FALSE(ul_repo->get(slot, 4).has_value());
}

TEST(ru_uplink_scheduling_recorder_test, prach_request_with_non_rb_aligned_frequency_offset_is_rejected)
{
  auto                          ul_repo    = std::make_shared<ru_uplink_request_repository>(40);
  auto                          prach_repo = std::make_shared<ru_uplink_request_repository>(40);
  ru_uplink_scheduling_recorder recorder(make_config(), ocudulog::fetch_basic_logger("TEST"), ul_repo, prach_repo);

  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 7);
  recorder.handle_prach_scheduling(
      /*eaxc=*/4,
      make_results(slot,
                   /*start=*/0,
                   /*nof_sym=*/12,
                   /*prb_start=*/0,
                   /*nof_prb=*/12,
                   filter_index_type::ul_prach_preamble_short,
                   /*section_id=*/1,
                   cplane_scs::kHz30,
                   /*frequency_offset=*/0));

  // A zero frequencyOffset would place the PRACH at RB 25.5 in a 51-RB carrier.
  ASSERT_FALSE(prach_repo->get(slot, 4).has_value());
}
