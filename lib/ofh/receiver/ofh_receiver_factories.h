// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../support/prach_context_repository.h"
#include "../support/rx_grid_context_repository.h"
#include "../support/uplink_cplane_context_repository.h"
#include "../support/uplink_notified_grid_symbol_repository.h"
#include "ofh_receiver_impl.h"
#include "ocudu/ofh/receiver/ofh_receiver.h"
#include "ocudu/ofh/receiver/ofh_receiver_configuration.h"
#include <memory>

namespace ocudu {

class task_executor;

namespace ofh {

/// Creates a receiver with the given configuration and dependencies.
std::unique_ptr<receiver> create_receiver(const receiver_config&                                  receiver_cfg,
                                          ocudulog::basic_logger&                                 logger,
                                          task_executor&                                          uplink_executor,
                                          std::unique_ptr<ether::receiver>                        eth_receiver,
                                          uplane_rx_symbol_notifier*                              notifier,
                                          std::shared_ptr<prach_context_repository>               prach_context_repo,
                                          std::shared_ptr<rx_grid_context_repository>             ul_slot_context_repo,
                                          std::shared_ptr<uplink_cplane_context_repository>       ul_cp_context_repo,
                                          std::shared_ptr<uplink_cplane_context_repository>       prach_cp_context_repo,
                                          std::shared_ptr<uplink_notified_grid_symbol_repository> notifier_symbol_repo);

} // namespace ofh
} // namespace ocudu
