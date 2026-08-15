// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/receiver/ofh_ru_rx_cplane_data_flow.h"
#include "ocudu/ocudulog/ocudulog.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

namespace {

/// Control-Plane decoder spy returning preset results.
class cplane_message_decoder_spy : public cplane_message_decoder
{
  cplane_message_decoder_results spy_results;
  bool                           succeed = true;

public:
  bool decode(cplane_message_decoder_results& results, span<const uint8_t> /*message*/) override
  {
    results = spy_results;
    return succeed;
  }

  void set_results(const cplane_message_decoder_results& results) { spy_results = results; }
  void set_should_fail() { succeed = false; }
};

/// Notifier spy recording the forwarded Control-Plane result.
class ru_rx_cplane_notifier_spy : public ru_rx_cplane_notifier
{
  bool                           called    = false;
  unsigned                       last_eaxc = 0;
  cplane_message_decoder_results last_results;

public:
  void on_cplane_message_received(unsigned eaxc, const cplane_message_decoder_results& results) override
  {
    called       = true;
    last_eaxc    = eaxc;
    last_results = results;
  }

  bool                                  has_been_called() const { return called; }
  unsigned                              get_eaxc() const { return last_eaxc; }
  const cplane_message_decoder_results& get_results() const { return last_results; }
};

} // namespace

TEST(ru_rx_cplane_data_flow_test, decoded_message_is_forwarded_to_notifier)
{
  auto  decoder     = std::make_unique<cplane_message_decoder_spy>();
  auto* decoder_ptr = decoder.get();

  cplane_message_decoder_results results;
  results.section_type        = 1;
  results.radio_hdr.direction = data_direction::downlink;
  results.radio_hdr.slot      = slot_point(0, 0, 1);
  results.section.section_id  = 7;
  results.section.prb_start   = 0;
  results.section.nof_prb     = 51;
  decoder_ptr->set_results(results);

  ru_rx_cplane_notifier_spy notifier;
  ru_rx_cplane_data_flow    data_flow(ocudulog::fetch_basic_logger("TEST"), std::move(decoder), notifier);

  data_flow.decode_message(3, {});

  ASSERT_TRUE(notifier.has_been_called());
  ASSERT_EQ(3, notifier.get_eaxc());
  ASSERT_EQ(1, notifier.get_results().section_type);
  ASSERT_EQ(data_direction::downlink, notifier.get_results().radio_hdr.direction);
  ASSERT_EQ(7, notifier.get_results().section.section_id);
}

TEST(ru_rx_cplane_data_flow_test, failed_decode_does_not_notify)
{
  auto decoder = std::make_unique<cplane_message_decoder_spy>();
  decoder->set_should_fail();

  ru_rx_cplane_notifier_spy notifier;
  ru_rx_cplane_data_flow    data_flow(ocudulog::fetch_basic_logger("TEST"), std::move(decoder), notifier);

  data_flow.decode_message(3, {});

  ASSERT_FALSE(notifier.has_been_called());
}
