// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_ru_rx_message_handler.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/serdes/ofh_cplane_message_decoder.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// \brief Notifier invoked with each decoded received Control-Plane message.
///
/// Implemented by the O-RU orchestration to act on the scheduling command: prepare a downlink resource grid (downlink
/// section), or trigger uplink / PRACH User-Plane transmission (uplink and PRACH sections).
class ru_rx_cplane_notifier
{
public:
  /// Default destructor.
  virtual ~ru_rx_cplane_notifier() = default;

  /// Called when a Control-Plane message is received and decoded for the given eAxC.
  virtual void on_cplane_message_received(unsigned eaxc, const cplane_message_decoder_results& results) = 0;
};

/// \brief O-RU receive Control-Plane data flow.
///
/// Decodes a Control-Plane message received from the O-DU and forwards the decoded scheduling information to a
/// notifier. The O-DU uses the Control-Plane to tell the O-RU what to expect (downlink) and what to capture and
/// transmit (uplink and PRACH); this data flow recovers it and leaves the resulting action to the notifier.
class ru_rx_cplane_data_flow : public ru_rx_message_handler
{
public:
  ru_rx_cplane_data_flow(ocudulog::basic_logger&                 logger_,
                         std::unique_ptr<cplane_message_decoder> decoder_,
                         ru_rx_cplane_notifier&                  notifier_);

  /// Decodes the given Control-Plane message for the given eAxC and forwards the result to the notifier.
  void decode_message(unsigned eaxc, span<const uint8_t> message) override;

private:
  ocudulog::basic_logger&                 logger;
  std::unique_ptr<cplane_message_decoder> decoder;
  ru_rx_cplane_notifier&                  notifier;
};

} // namespace ofh
} // namespace ocudu
