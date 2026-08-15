// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../transmitter/ofh_data_flow_uplane_data.h"
#include "ofh_ru_uplink_request_repository.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/ru_sector.h"
#include <memory>
#include <optional>

namespace ocudu {

class prach_buffer;

namespace ofh {

/// O-RU PRACH window responder configuration.
struct ru_prach_window_responder_config {
  /// Radio sector identifier.
  unsigned sector;
  /// PRACH eAxCs, indexed by PRACH buffer port.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc;
};

/// \brief Builds PRACH User-Plane replies from captured PRACH IQ.
///
/// Driven by the PRACH IQ source (a real lower PHY / radio, or the emulator's fake radio): for a captured PRACH window
/// it looks up the recorded PRACH Control-Plane request (see \ref ru_uplink_scheduling_recorder) for each buffer port's
/// eAxC and, if a request was recorded, enqueues the PRACH User-Plane message carrying the captured IQ and clears the
/// request (so it cannot generate further replies). It is the O-RU mirror of the O-DU's PRACH reception handler.
class ru_prach_window_responder
{
public:
  ru_prach_window_responder(const ru_prach_window_responder_config&       config,
                            ocudulog::basic_logger&                       logger_,
                            std::shared_ptr<ru_uplink_request_repository> prach_request_repo_,
                            data_flow_uplane_data&                        prach_data_flow_);

  /// Handles a captured PRACH window, enqueueing a PRACH User-Plane reply for each eAxC with a recorded request.
  void handle_prach_window(slot_point slot, const prach_buffer& buffer);

  /// \brief Returns the recorded PRACH Control-Plane occasion for the slot, if any (the first configured PRACH eAxC
  /// with a request recorded). Used by the application's radio driver to issue the matching capture request.
  std::optional<ru_prach_occasion> peek_prach_occasion(slot_point slot) const;

private:
  ocudulog::basic_logger&                         logger;
  std::shared_ptr<ru_uplink_request_repository>   prach_request_repo;
  data_flow_uplane_data&                          prach_data_flow;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc;
  const unsigned                                  sector_id;
};

} // namespace ofh
} // namespace ocudu
