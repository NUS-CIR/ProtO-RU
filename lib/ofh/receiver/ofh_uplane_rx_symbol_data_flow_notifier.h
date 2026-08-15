// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../support/rx_grid_context_repository.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ofh_uplane_rx_symbol_notifier.h"

namespace ocudu {
namespace ofh {

/// Open Fronthaul User-Plane uplink received symbol data flow notifier.
class uplane_rx_symbol_data_flow_notifier
{
public:
  uplane_rx_symbol_data_flow_notifier(ocudulog::basic_logger&                     logger_,
                                      std::shared_ptr<rx_grid_context_repository> ul_context_repo_,
                                      std::shared_ptr<uplane_rx_symbol_notifier>  notifier_) :
    logger(logger_), ul_context_repo(std::move(ul_context_repo_)), notifier(std::move(notifier_))
  {
    ocudu_assert(ul_context_repo, "Invalid uplink context repository");
    ocudu_assert(notifier, "Invalid notifier pointer");
  }

  /// Notifies the received symbol for the given slot and symbol when all the PRBs for all the ports of the grid
  /// have been written.
  void notify_received_symbol(slot_point slot, unsigned symbol);

private:
  ocudulog::basic_logger&                     logger;
  std::shared_ptr<rx_grid_context_repository> ul_context_repo;
  std::shared_ptr<uplane_rx_symbol_notifier>  notifier;
};

} // namespace ofh
} // namespace ocudu
