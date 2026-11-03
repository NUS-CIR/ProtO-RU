// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_downlink_handler_impl.h"
#include "../support/logger_utils.h"
#include "helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/instrumentation/traces/ofh_traces.h"
#include "ocudu/ofh/ofh_error_notifier.h"
#include "ocudu/phy/support/resource_grid_context.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include "ocudu/ran/beamforming/beam_identifier.h"

using namespace ocudu;
using namespace ofh;

/// \brief Returns the number of eAxCs that the transmission of the given resource grid requires.
///
/// The resource grid is sized to the total number of beams that the antenna topology defines.
static unsigned get_nof_required_eaxc(const resource_grid_reader& reader, bool is_beamforming_enabled)
{
  // Category B maps the k-th non-empty beam-port onto the k-th eAxC.
  if (is_beamforming_enabled) {
    unsigned nof_active_ports = 0;
    for (unsigned port = 0, e = reader.get_nof_ports(); port != e; ++port) {
      nof_active_ports += reader.is_empty(port) ? 0 : 1;
    }

    return nof_active_ports;
  }

  // Category A maps each beam-port onto the eAxC with the same index. Its topology defines no beam grid, so the grid
  // has exactly one beam-port per antenna port and eAxC.
  return reader.get_nof_ports();
}

downlink_handler_impl::downlink_handler_impl(const downlink_handler_impl_config&  config,
                                             downlink_handler_impl_dependencies&& dependencies) :
  sector_id(config.sector),
  logger(dependencies.logger),
  cp(config.cp),
  is_cat_b_enabled(config.is_cat_b_enabled),
  tdd_config(config.tdd_config),
  dl_eaxc(config.dl_eaxc),
  window_checker(dependencies.logger,
                 config.sector,
                 "downlink",
                 get_biggest_min_tx_parameter(config.tx_timing_params),
                 get_nsymb_per_slot(config.cp)),
  data_flow_cplane(std::move(dependencies.data_flow_cplane)),
  data_flow_uplane(std::move(dependencies.data_flow_uplane)),
  frame_pool_dl_cp(std::move(dependencies.frame_pool_dl_cp)),
  frame_pool_dl_up(std::move(dependencies.frame_pool_dl_up)),
  err_notifier(dependencies.err_notifier),
  metrics_collector(*data_flow_cplane, *data_flow_uplane, window_checker),
  enable_log_warnings_for_lates(config.enable_log_warnings_for_lates)
{
  ocudu_assert(data_flow_cplane, "Invalid Control-Plane data flow");
  ocudu_assert(data_flow_uplane, "Invalid User-Plane data flow");
  ocudu_assert(frame_pool_dl_cp, "Invalid downlink Control-Plane frame pool");
  ocudu_assert(frame_pool_dl_up, "Invalid downlink User-Plane frame pool");
}

void downlink_handler_impl::start()
{
  stop_control.reset();

  // Start the data flows.
  data_flow_cplane->get_operation_controller().start();
  data_flow_uplane->get_operation_controller().start();
}

void downlink_handler_impl::stop()
{
  // Stop accepting grids.
  stop_control.stop();

  // Stop the data flows.
  data_flow_cplane->get_operation_controller().stop();
  data_flow_uplane->get_operation_controller().stop();
}

void downlink_handler_impl::handle_dl_data(const resource_grid_context& context, const shared_resource_grid& grid)
{
  auto token = stop_control.get_token();
  if (OCUDU_UNLIKELY(token.is_stop_requested())) {
    return;
  }

  // Detect resource grids delivered by the PHY out of slot order.
  // If the slot N+1 is delivered before slot N, the eCPRI sequence identifiers of the transmitted messages will show a
  // jump forward and then backward.
  if (slot_point prev_slot = last_processed_slot.exchange(context.slot, std::memory_order_relaxed); prev_slot.valid()) {
    if (OCUDU_UNLIKELY(context.slot < prev_slot)) {
      logger.info("Sector#{}: received out-of-order downlink resource grid for slot '{}' after slot '{}' - "
                  "eCPRI sequence identifiers may be non-monotonic",
                  sector_id,
                  context.slot,
                  prev_slot);
    }
  }

  const resource_grid_reader& reader = grid.get_reader();

  ocudu_assert(dl_eaxc.size() <= reader.get_nof_ports(),
               "Resource grid has '{}' beam-ports, but at least '{}' are required, one per configured downlink eAxC",
               reader.get_nof_ports(),
               dl_eaxc.size());

  unsigned nof_required_eaxc = get_nof_required_eaxc(reader, is_cat_b_enabled);
  report_error_if_not(nof_required_eaxc <= dl_eaxc.size(),
                      "Resource grid needs '{}' downlink eAxCs and only '{}' are configured",
                      nof_required_eaxc,
                      dl_eaxc.size());

  trace_point tp = ofh_tracer.now();

  // Clear any stale buffers associated with the context slot.
  metrics_collector.update_cp_dl_lates(frame_pool_dl_cp->clear_slot(context.slot, context.sector));
  metrics_collector.update_up_dl_lates(frame_pool_dl_up->clear_slot(context.slot, context.sector));

  // Nothing to do on empty resource grids.
  if (reader.is_empty()) {
    return;
  }

  if (OCUDU_UNLIKELY(logger.debug.enabled())) {
    logger.debug("Sector#{}: received downlink resource grid for slot {}", sector_id, context.slot);
  }

  if (window_checker.is_late(context.slot)) {
    log_conditional_warning(
        logger,
        enable_log_warnings_for_lates,
        "Sector#{}: dropped late downlink resource grid in slot '{}'. No OFH data will be transmitted for this slot",
        sector_id,
        context.slot);
    ofh_tracer << trace_event("ofh_handle_dl_late", tp);

    err_notifier.on_late_downlink_message({context.slot, sector_id});
    return;
  }

  data_flow_cplane_type_1_context cplane_context;
  cplane_context.slot         = context.slot;
  cplane_context.filter_type  = filter_index_type::standard_channel_filter;
  cplane_context.direction    = data_direction::downlink;
  cplane_context.symbol_range = tdd_config ? get_active_tdd_dl_symbols(*tdd_config, context.slot.slot_index(), cp)
                                           : ofdm_symbol_range(0, reader.get_nof_symbols());

  data_flow_uplane_resource_grid_context uplane_context;
  uplane_context.slot         = context.slot;
  uplane_context.sector       = context.sector;
  uplane_context.symbol_range = cplane_context.symbol_range;

  // For backward compatibility, Category A transmits every configured eAxC, regardless of whether its beam-port is
  // empty, whilst Category B scans every beam-port and transmits the non-empty ones.
  unsigned nof_scanned_beams = is_cat_b_enabled ? reader.get_nof_ports() : dl_eaxc.size();

  for (unsigned eaxc_index = 0, i_beam = 0; i_beam != nof_scanned_beams; ++i_beam) {
    if (is_cat_b_enabled && reader.is_empty(i_beam)) {
      continue;
    }

    unsigned eaxc = dl_eaxc[eaxc_index++];

    // Control-Plane data flow.
    cplane_context.eaxc    = eaxc;
    cplane_context.beam_id = to_beam_id(i_beam);
    data_flow_cplane->enqueue_section_type_1_message(cplane_context);

    // User-Plane data flow. Note that the port of the resource grid is a beam-port, which in Category A, where no
    // beamforming is applied, coincides with the antenna port.
    uplane_context.port = i_beam;
    uplane_context.eaxc = eaxc;
    data_flow_uplane->enqueue_section_type_1_message(uplane_context, grid);
  }

  ofh_tracer << trace_event("ofh_handle_downlink", tp);
}
