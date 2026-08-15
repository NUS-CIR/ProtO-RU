// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../support/rx_grid_context_repository.h"
#include "ocudu/ocudulog/logger.h"

namespace ocudu {
namespace ofh {

struct uplane_message_decoder_results;

/// \brief Open Fronthaul User-Plane uplink received symbol data flow writer.
///
/// Writes IQ data received in an Open Fronthaul message to the corresponding resource grid.
class uplane_rx_symbol_data_flow_writer
{
public:
  uplane_rx_symbol_data_flow_writer(span<const unsigned>                        ul_eaxc_,
                                    unsigned                                    sector_id_,
                                    ocudulog::basic_logger&                     logger_,
                                    std::shared_ptr<rx_grid_context_repository> ul_context_repo_) :
    ul_eaxc(ul_eaxc_.begin(), ul_eaxc_.end()),
    sector_id(sector_id_),
    logger(logger_),
    ul_context_repo(std::move(ul_context_repo_))
  {
    ocudu_assert(!ul_eaxc.empty(), "Invalid number of uplink eAxCs");
    ocudu_assert(ul_context_repo, "Invalid uplink context repository");
  }

  /// Writes the given decoder results in the corresponding resource grid using the given eAxC. Returns true on success,
  /// otherwise false.
  bool write_to_resource_grid(unsigned eaxc, const uplane_message_decoder_results& results);

private:
  const static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  const unsigned                                        sector_id;
  ocudulog::basic_logger&                               logger;
  std::shared_ptr<rx_grid_context_repository>           ul_context_repo;
};

} // namespace ofh
} // namespace ocudu
