// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_uplane_rx_symbol_data_flow_notifier.h"
#include "ocudu/ofh/ofh_uplane_rx_symbol_notifier.h"

using namespace ocudu;
using namespace ofh;

void uplane_rx_symbol_data_flow_notifier::notify_received_symbol(slot_point slot, unsigned symbol)
{
  expected<rx_grid_context::rx_grid_context_resource_grid_info> context =
      ul_context_repo->try_popping_complete_resource_grid_symbol(slot, symbol);

  if (!context) {
    return;
  }

  rx_grid_context::rx_grid_context_resource_grid_info& ctx_value = *context;
  uplane_rx_symbol_context notification_context = {ctx_value.context.slot, symbol, ctx_value.context.sector};
  notifier->on_new_uplink_symbol(notification_context, std::move(ctx_value.grid), true);

  if (OCUDU_UNLIKELY(logger.debug.enabled())) {
    logger.debug("Notifying UL symbol in slot '{}', symbol '{}' for sector#{}",
                 notification_context.slot,
                 notification_context.symbol,
                 notification_context.sector);
  }
}
