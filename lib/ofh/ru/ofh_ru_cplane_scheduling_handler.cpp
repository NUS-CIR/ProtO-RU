// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_cplane_scheduling_handler.h"
#include "ofh_ru_prach_frequency_mapping.h"
#include "ocudu/phy/support/resource_grid_context.h"

using namespace ocudu;
using namespace ofh;

ru_cplane_scheduling_handler::ru_cplane_scheduling_handler(
    const ru_cplane_scheduling_handler_config&       config,
    const ru_cplane_scheduling_handler_dependencies& dependencies) :
  logger(*dependencies.logger),
  upper_phy(*dependencies.upper_phy),
  grid_repo(dependencies.grid_repo),
  uplink_data_flow(*dependencies.uplink_data_flow),
  prach_data_flow(*dependencies.prach_data_flow),
  sector_id(config.sector)
{
  ocudu_assert(dependencies.upper_phy, "Invalid upper PHY");
  ocudu_assert(grid_repo, "Invalid received-grid repository");
  ocudu_assert(dependencies.uplink_data_flow, "Invalid uplink data flow");
  ocudu_assert(dependencies.prach_data_flow, "Invalid PRACH data flow");
}

void ru_cplane_scheduling_handler::handle_downlink_scheduling(unsigned                              eaxc,
                                                              const cplane_message_decoder_results& results)
{
  slot_point        slot = results.radio_hdr.slot;
  ofdm_symbol_range symbol_range(results.radio_hdr.start_symbol,
                                 results.radio_hdr.start_symbol + results.section.nof_symbols);

  // A resource grid represents the whole multi-port slot, while the O-DU sends one scheduling command per eAxC. Reuse
  // the grid prepared by an earlier command for this slot so that every eAxC is assembled into a different port of the
  // same grid.
  std::lock_guard<std::mutex> lock(downlink_mutex);
  grid_repo->process_pending_contexts();
  shared_resource_grid grid = grid_repo->find_grid(slot);
  if (!grid.is_valid()) {
    grid = upper_phy.get_downlink_rx_grid(slot);
  }
  if (!grid.is_valid()) {
    logger.warning(
        "Sector#{}: no downlink resource grid available to receive slot '{}' and eAxC '{}'", sector_id, slot, eaxc);
    return;
  }

  resource_grid_context context;
  context.slot   = slot;
  context.sector = sector_id;
  grid_repo->add(context, grid, symbol_range, logger);
}

void ru_cplane_scheduling_handler::handle_uplink_scheduling(unsigned                              eaxc,
                                                            const cplane_message_decoder_results& results)
{
  slot_point slot = results.radio_hdr.slot;

  // Get the uplink IQ to transmit.
  shared_resource_grid grid = upper_phy.get_uplink_tx_grid(slot);
  if (!grid.is_valid()) {
    logger.warning(
        "Sector#{}: no uplink resource grid available to transmit slot '{}' and eAxC '{}'", sector_id, slot, eaxc);
    return;
  }

  data_flow_uplane_resource_grid_context context;
  context.slot   = slot;
  context.sector = sector_id;
  context.port   = 0;
  context.eaxc   = eaxc;
  context.symbol_range =
      ofdm_symbol_range(results.radio_hdr.start_symbol, results.radio_hdr.start_symbol + results.section.nof_symbols);
  context.section_id = results.section.section_id;

  uplink_data_flow.enqueue_section_type_1_message(context, grid);
}

void ru_cplane_scheduling_handler::handle_prach_scheduling(unsigned eaxc, const cplane_message_decoder_results& results)
{
  prach_subcarrier_spacing prach_scs = to_prach_subcarrier_spacing(results.frame_structure_scs);
  if (!is_prach_filter_compatible(results.radio_hdr.filter_index, prach_scs)) {
    logger.info("Sector#{}: dropped PRACH Control-Plane request for slot '{}' and eAxC '{}': incompatible filter '{}' "
                "and frame-structure SCS '{}'",
                sector_id,
                results.radio_hdr.slot,
                eaxc,
                to_value(results.radio_hdr.filter_index),
                to_value(results.frame_structure_scs));
    return;
  }

  slot_point          slot  = results.radio_hdr.slot;
  const prach_buffer& prach = upper_phy.get_prach_tx_buffer(slot);

  data_flow_uplane_prach_context context;
  context.slot         = slot;
  context.sector       = sector_id;
  context.port         = 0;
  context.eaxc         = eaxc;
  context.prb_start    = results.section.prb_start;
  context.nof_prb      = results.section.nof_prb;
  context.start_symbol = results.radio_hdr.start_symbol;
  context.nof_symbols  = results.section.nof_symbols;
  context.filter_index = results.radio_hdr.filter_index;
  context.prach_scs    = prach_scs;
  context.section_id   = results.section.section_id;

  prach_data_flow.enqueue_prach_message(context, prach);
}
