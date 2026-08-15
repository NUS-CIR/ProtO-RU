// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/timing/ofh_ota_symbol_boundary_notifier.h"
#include "ocudu/support/executors/task_executor.h"

namespace ocudu {
namespace ofh {

/// \brief Defers a wrapped OTA symbol boundary notifier onto an OFH transmit executor.
///
/// Mirrors the O-DU's \ref transmitter_ota_symbol_task_dispatcher: the timing thread keeps the OTA-sensitive work
/// (reception-window checker, downlink reception window handler) on itself and hands the message transmitter's
/// per-symbol Ethernet drain + send off to a dedicated executor, so the timing thread only ticks. The O-RU wraps just
/// the message transmitter (a single notifier); the window checker and downlink reception window handler are subscribed
/// directly.
///
/// Lifetime: the deferred task references the wrapped notifier, so the executor must be stopped before this dispatcher
/// (and the notifier it wraps) is destroyed. The application stops its workers before tearing down the sector, so no
/// stop token is needed.
class ru_ota_symbol_task_dispatcher : public ota_symbol_boundary_notifier
{
public:
  ru_ota_symbol_task_dispatcher(unsigned                      sector_id_,
                                ocudulog::basic_logger&       logger_,
                                task_executor&                executor_,
                                ota_symbol_boundary_notifier& notifier_) :
    sector_id(sector_id_), logger(logger_), executor(executor_), notifier(notifier_)
  {
  }

  // See interface for documentation.
  void on_new_symbol(const slot_symbol_point_context& symbol_point_context) override
  {
    if (!executor.defer([this, symbol_point_context]() noexcept { notifier.on_new_symbol(symbol_point_context); })) {
      logger.warning("Sector#{}: failed to dispatch the message transmitter task for slot '{}' and symbol '{}'",
                     sector_id,
                     symbol_point_context.symbol_point.get_slot(),
                     symbol_point_context.symbol_point.get_symbol_index());
    }
  }

private:
  const unsigned                sector_id;
  ocudulog::basic_logger&       logger;
  task_executor&                executor;
  ota_symbol_boundary_notifier& notifier;
};

} // namespace ofh
} // namespace ocudu
