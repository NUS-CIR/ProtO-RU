// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_downlink_rx_window_handler.h"

using namespace ocudu;
using namespace ofh;

ru_downlink_rx_window_handler::ru_downlink_rx_window_handler(const ru_downlink_rx_window_handler_config& config,
                                                             ru_upper_phy&                               upper_phy_,
                                                             std::shared_ptr<rx_grid_context_repository> grid_repo_,
                                                             ocudulog::basic_logger&                     logger_) :
  logger(logger_),
  upper_phy(upper_phy_),
  grid_repo(std::move(grid_repo_)),
  sector_id(config.sector),
  finalize_offset_symbols(config.finalize_offset_symbols)
{
  ocudu_assert(grid_repo, "Invalid received-grid repository");
}

void ru_downlink_rx_window_handler::on_new_symbol(const slot_symbol_point_context& symbol_point_context)
{
  // Apply the pending grid registrations enqueued by the Control-Plane scheduling handler.
  grid_repo->process_pending_contexts();

  // Finalise the downlink symbol at the configured offset from the current OTA symbol (behind it when the downlink is
  // only consumed for statistics, ahead of it when a radio must transmit the grid over the air). Popping releases the
  // grid back to the upper PHY's pool; whatever downlink was received for it is handed back for the application to
  // consume, transmit or discard.
  slot_symbol_point closing = symbol_point_context.symbol_point - finalize_offset_symbols;

  auto info = grid_repo->pop_resource_grid_symbol(closing.get_slot(), closing.get_symbol_index());
  if (info.has_value()) {
    upper_phy.on_downlink_rx_grid_completed(info.value().context.slot, info.value().grid);
  }
}
