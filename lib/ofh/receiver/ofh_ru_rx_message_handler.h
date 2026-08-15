// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/span.h"
#include <cstdint>

namespace ocudu {
namespace ofh {

/// \brief Handles a decoded Open Fronthaul message (User-Plane or Control-Plane) for an O-RU.
///
/// Implemented by the O-RU receive data flows so the message receiver can dispatch a received message to the matching
/// data flow by eCPRI message type.
class ru_rx_message_handler
{
public:
  virtual ~ru_rx_message_handler() = default;

  /// Decodes the given Open Fronthaul message for the given eAxC.
  virtual void decode_message(unsigned eaxc, span<const uint8_t> message) = 0;
};

} // namespace ofh
} // namespace ocudu
