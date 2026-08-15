// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_rx_cplane_data_flow.h"

using namespace ocudu;
using namespace ofh;

ru_rx_cplane_data_flow::ru_rx_cplane_data_flow(ocudulog::basic_logger&                 logger_,
                                               std::unique_ptr<cplane_message_decoder> decoder_,
                                               ru_rx_cplane_notifier&                  notifier_) :
  logger(logger_), decoder(std::move(decoder_)), notifier(notifier_)
{
  ocudu_assert(decoder, "Invalid Control-Plane decoder");
}

void ru_rx_cplane_data_flow::decode_message(unsigned eaxc, span<const uint8_t> message)
{
  cplane_message_decoder_results results;
  if (!decoder->decode(results, message)) {
    return;
  }

  notifier.on_cplane_message_received(eaxc, results);
}
