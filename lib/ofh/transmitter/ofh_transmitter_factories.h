// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../support/prach_context_repository.h"
#include "../support/rx_grid_context_repository.h"
#include "../support/uplink_cplane_context_repository.h"
#include "../support/uplink_notified_grid_symbol_repository.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ethernet/ethernet_transmitter.h"
#include "ocudu/ofh/transmitter/ofh_transmitter.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_configuration.h"

namespace ocudu {

class task_executor;

namespace ether {
class eth_frame_pool;
} // namespace ether

namespace ofh {

/// Creates a transmitter with the given configuration and dependencies.
std::unique_ptr<transmitter>
create_transmitter(const transmitter_config&                               transmitter_cfg,
                   ocudulog::basic_logger&                                 logger,
                   task_executor&                                          tx_executor,
                   task_executor&                                          downlink_executor,
                   error_notifier&                                         err_notifier,
                   std::unique_ptr<ether::transmitter>                     eth_transmitter,
                   std::shared_ptr<prach_context_repository>               prach_context_repo,
                   std::shared_ptr<rx_grid_context_repository>             ul_slot_context_repo,
                   std::shared_ptr<uplink_cplane_context_repository>       ul_cp_context_repo,
                   std::shared_ptr<uplink_cplane_context_repository>       prach_cp_context_repo,
                   std::shared_ptr<uplink_notified_grid_symbol_repository> notifier_symbol_repo);

} // namespace ofh
} // namespace ocudu
