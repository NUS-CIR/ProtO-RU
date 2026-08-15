// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../support/rx_grid_context_repository.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ru_upper_phy.h"
#include "ocudu/ofh/timing/ofh_ota_symbol_boundary_notifier.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// O-RU downlink reception window handler configuration.
struct ru_downlink_rx_window_handler_config {
  /// Radio sector identifier.
  unsigned sector;
  /// \brief Offset, in symbols behind the current OTA symbol, at which a downlink symbol's reception is finalized.
  ///
  /// Positive values finalize after the symbol's air time (e.g. one slot behind, adequate when the downlink is only
  /// consumed for statistics, like in loopback mode). Negative values finalize ahead of air time, required when a
  /// radio must transmit the grid over the air: the grid has to be handed to the radio before its processing pipeline
  /// (max_proc_delay) picks the slot up.
  int finalize_offset_symbols;
};

/// \brief O-RU downlink reception window handler.
///
/// Subscribed to the OTA symbol timing, it drains the received-grid repository each symbol so its pending-context queue
/// never overflows, and finalises the downlink grid whose reception window has closed: the grid is popped from the
/// repository (releasing it back to the upper PHY's pool) and the upper PHY is notified that the downlink reception for
/// that slot/symbol has completed.
///
/// This is the O-RU-side analog of the O-DU's closed reception window handler.
class ru_downlink_rx_window_handler : public ota_symbol_boundary_notifier
{
public:
  ru_downlink_rx_window_handler(const ru_downlink_rx_window_handler_config& config,
                                ru_upper_phy&                               upper_phy_,
                                std::shared_ptr<rx_grid_context_repository> grid_repo_,
                                ocudulog::basic_logger&                     logger_);

  // See interface for documentation.
  void on_new_symbol(const slot_symbol_point_context& symbol_point_context) override;

private:
  ocudulog::basic_logger&                     logger;
  ru_upper_phy&                               upper_phy;
  std::shared_ptr<rx_grid_context_repository> grid_repo;
  const unsigned                              sector_id;
  /// Symbols behind (positive) or ahead of (negative) the current OTA symbol at which a downlink symbol's reception
  /// is finalized.
  const int finalize_offset_symbols;
};

} // namespace ofh
} // namespace ocudu
