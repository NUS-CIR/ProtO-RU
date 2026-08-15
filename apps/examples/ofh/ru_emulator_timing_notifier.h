// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/timing/ofh_ota_symbol_boundary_notifier_manager.h"
#include "ocudu/support/executors/task_executor.h"
#include <atomic>

namespace ocudu {

/// Realtime worker that generates OTA symbol notifications.
class ru_emulator_timing_notifier : public ofh::ota_symbol_boundary_notifier_manager
{
  enum class worker_status { running, stop_requested, stopped };

  ocudulog::basic_logger&                         logger;
  std::vector<ofh::ota_symbol_boundary_notifier*> ota_notifiers;
  task_executor&                                  executor;
  subcarrier_spacing                              scs;
  const unsigned                                  nof_symbols_per_slot;
  const unsigned                                  nof_symbols_per_sec;
  const unsigned                                  nof_slots_per_hyper_system_frame;
  const std::chrono::duration<double, std::nano>  symbol_duration;
  const std::chrono::nanoseconds                  sleep_time;
  unsigned                                        previous_symb_index = 0;
  std::atomic<worker_status>                      status{worker_status::running};

public:
  ru_emulator_timing_notifier(ocudulog::basic_logger& logger_, task_executor& executor_, subcarrier_spacing scs_);

  /// Starts operation of the timing notifier.
  void start();

  /// Stops operation of the timing notifier.
  void stop();

  /// See interface for documentation.
  void subscribe(span<ofh::ota_symbol_boundary_notifier*> notifiers) override;

private:
  /// Main timing loop.
  void timing_loop();

  /// Polls the system time checking for the start of a new OTA symbol.
  void poll();

  /// Notifies the given slot symbol point through the registered notifiers.
  void notify_slot_symbol_point(ofh::slot_symbol_point slot);
};

} // namespace ocudu
