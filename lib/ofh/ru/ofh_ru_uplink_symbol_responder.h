// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../transmitter/ofh_data_flow_uplane_data.h"
#include "ofh_ru_uplink_request_repository.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// O-RU uplink symbol responder configuration.
struct ru_uplink_symbol_responder_config {
  /// Radio sector identifier.
  unsigned sector;
  /// Uplink eAxCs, indexed by resource grid port (port \c p carries the IQ for \c ul_eaxc[p]).
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
};

/// \brief Builds uplink User-Plane replies from captured uplink IQ.
///
/// Driven by the uplink IQ source (a real lower PHY / radio, or the emulator's fake radio): for each captured uplink
/// resource grid symbol it looks up the recorded uplink Control-Plane request (see \ref ru_uplink_scheduling_recorder)
/// and, if the symbol falls within the requested window, enqueues the corresponding uplink User-Plane message carrying
/// the captured IQ. Once the last requested symbol has been answered, the request is cleared from the repository so it
/// cannot generate further replies. It is the O-RU mirror of the O-DU's uplink reception symbol handler.
class ru_uplink_symbol_responder
{
public:
  ru_uplink_symbol_responder(const ru_uplink_symbol_responder_config&      config,
                             ocudulog::basic_logger&                       logger_,
                             std::shared_ptr<ru_uplink_request_repository> ul_request_repo_,
                             data_flow_uplane_data&                        ul_data_flow_);

  /// Handles a captured uplink resource grid symbol, enqueueing a User-Plane reply for each eAxC with a matching
  /// recorded uplink Control-Plane request.
  void handle_uplink_symbol(slot_point slot, unsigned symbol, const shared_resource_grid& grid);

private:
  ocudulog::basic_logger&                         logger;
  std::shared_ptr<ru_uplink_request_repository>   ul_request_repo;
  data_flow_uplane_data&                          ul_data_flow;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  const unsigned                                  sector_id;
};

} // namespace ofh
} // namespace ocudu
