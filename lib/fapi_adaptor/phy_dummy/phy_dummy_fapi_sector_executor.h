// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ocudulog/logger.h"
#include "ocudu/support/executors/task_executor.h"
#include "ocudu/support/synchronization/stop_event.h"
#include <utility>

namespace ocudu {
namespace fapi_adaptor {

/// Sequential executor shared by the P5 and P7 parts of a PHY dummy FAPI adaptor sector.
class phy_dummy_fapi_sector_executor
{
public:
  phy_dummy_fapi_sector_executor(unsigned sector_id_, ocudulog::basic_logger& logger_, task_executor& executor_) :
    sector_id(sector_id_), logger(logger_), executor(executor_)
  {
  }

  ~phy_dummy_fapi_sector_executor() { stop(); }

  /// Defers the given task. The task is not run once the executor is stopped.
  template <typename Func>
  void defer(const char* task_name, Func&& task)
  {
    if (!executor.defer([token = stop_control.get_token(), task = std::forward<Func>(task)]() mutable {
          if (!token.is_stop_requested()) {
            task();
          }
        })) {
      logger.warning(
          "Sector#{}: PHY dummy FAPI adaptor failed to dispatch {}. Cause: task queue is full", sector_id, task_name);
    }
  }

  /// Stops running the deferred tasks and waits for the pending ones to finish.
  void stop() { stop_control.stop(); }

private:
  const unsigned          sector_id;
  ocudulog::basic_logger& logger;
  /// Sequential executor of the sector logic.
  task_executor& executor;
  /// Stop control of the deferred tasks.
  stop_event_source stop_control;
};

} // namespace fapi_adaptor
} // namespace ocudu
