// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "support/prach_context_repository.h"
#include "support/rx_grid_context_repository.h"
#include "ocudu/ofh/ofh_controller.h"
#include "ocudu/ofh/receiver/ofh_receiver.h"
#include "ocudu/ofh/transmitter/ofh_transmitter.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// Open Fronthaul controller for the sector.
class sector_controller : public operation_controller
{
public:
  sector_controller(transmitter&                                ofh_tx_,
                    receiver&                                   ofh_rx_,
                    std::shared_ptr<rx_grid_context_repository> slot_repo_,
                    std::shared_ptr<prach_context_repository>   prach_repo_) :
    ofh_tx(ofh_tx_), ofh_rx(ofh_rx_), slot_repo(std::move(slot_repo_)), prach_repo(std::move(prach_repo_))
  {
  }

  // See interface for documentation.
  void start() override;

  // See interface for documentation.
  void stop() override;

private:
  transmitter&                                ofh_tx;
  receiver&                                   ofh_rx;
  std::shared_ptr<rx_grid_context_repository> slot_repo;
  std::shared_ptr<prach_context_repository>   prach_repo;
};

} // namespace ofh
} // namespace ocudu
