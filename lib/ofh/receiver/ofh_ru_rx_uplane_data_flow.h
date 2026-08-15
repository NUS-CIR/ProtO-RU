// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_ru_rx_message_handler.h"
#include "ofh_uplane_rx_symbol_data_flow_writer.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/serdes/ofh_uplane_message_decoder.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// O-RU receive User-Plane data flow configuration.
struct ru_rx_uplane_data_flow_config {
  /// Radio sector identifier.
  unsigned sector;
  /// eAxCs whose downlink User-Plane this data flow receives.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> eaxc;
};

/// O-RU receive User-Plane data flow dependencies.
struct ru_rx_uplane_data_flow_dependencies {
  /// Logger.
  ocudulog::basic_logger* logger = nullptr;
  /// Repository where the received downlink resource grids are assembled.
  std::shared_ptr<rx_grid_context_repository> grid_repo;
  /// Downlink-configured User-Plane message decoder.
  std::unique_ptr<uplane_message_decoder> uplane_decoder;
};

/// \brief O-RU receive User-Plane data flow.
///
/// Decodes a downlink Open Fronthaul User-Plane message received from the O-DU and writes its IQ samples into the
/// received-grid repository. It is the O-RU counterpart of the O-DU uplink receive data flow, reusing the same
/// direction-agnostic symbol writer with a downlink-configured decoder. The grid must already exist in the repository
/// (created from the associated Control-Plane message), which gates whether the samples are written.
class ru_rx_uplane_data_flow : public ru_rx_message_handler
{
public:
  ru_rx_uplane_data_flow(const ru_rx_uplane_data_flow_config&  config,
                         ru_rx_uplane_data_flow_dependencies&& dependencies);

  /// Decodes the given downlink User-Plane message for the given eAxC and writes it into the received-grid repository.
  void decode_message(unsigned eaxc, span<const uint8_t> message) override;

private:
  ocudulog::basic_logger&                 logger;
  std::unique_ptr<uplane_message_decoder> uplane_decoder;
  uplane_rx_symbol_data_flow_writer       rx_symbol_writer;
  const unsigned                          sector_id;
};

} // namespace ofh
} // namespace ocudu
