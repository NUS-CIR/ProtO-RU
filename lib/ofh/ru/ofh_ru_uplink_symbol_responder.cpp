// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_uplink_symbol_responder.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include <algorithm>

using namespace ocudu;
using namespace ofh;

ru_uplink_symbol_responder::ru_uplink_symbol_responder(const ru_uplink_symbol_responder_config&      config,
                                                       ocudulog::basic_logger&                       logger_,
                                                       std::shared_ptr<ru_uplink_request_repository> ul_request_repo_,
                                                       data_flow_uplane_data&                        ul_data_flow_) :
  logger(logger_),
  ul_request_repo(std::move(ul_request_repo_)),
  ul_data_flow(ul_data_flow_),
  ul_eaxc(config.ul_eaxc),
  sector_id(config.sector)
{
  ocudu_assert(ul_request_repo, "Invalid uplink request repository");
}

void ru_uplink_symbol_responder::handle_uplink_symbol(slot_point                  slot,
                                                      unsigned                    symbol,
                                                      const shared_resource_grid& grid)
{
  const resource_grid_reader& reader    = grid.get_reader();
  unsigned                    nof_ports = std::min<unsigned>(reader.get_nof_ports(), ul_eaxc.size());

  for (unsigned port = 0; port != nof_ports; ++port) {
    unsigned                         eaxc    = ul_eaxc[port];
    std::optional<ru_uplink_request> request = ul_request_repo->get(slot, eaxc);

    // No request recorded for this slot/eAxC, or this symbol falls outside the requested reception window.
    if (!request || (symbol < request->start_symbol) ||
        (symbol >= unsigned(request->start_symbol) + request->nof_symbols)) {
      continue;
    }

    data_flow_uplane_resource_grid_context df_context;
    df_context.slot         = slot;
    df_context.sector       = sector_id;
    df_context.port         = port;
    df_context.eaxc         = eaxc;
    df_context.symbol_range = ofdm_symbol_range(symbol, symbol + 1);
    df_context.section_id   = request->section_id;

    logger.debug("Sector#{}: building uplink User-Plane reply for slot '{}', eAxC '{}', symbol '{}'",
                 sector_id,
                 slot,
                 eaxc,
                 symbol);
    ul_data_flow.enqueue_section_type_1_message(df_context, grid);

    // The request has been fully answered: clear it so it does not produce replies again (e.g. when the slot numbering
    // wraps around after the O-DU stopped sending Control-Plane).
    if (symbol == unsigned(request->start_symbol) + request->nof_symbols - 1) {
      ul_request_repo->clear(slot, eaxc);
    }
  }
}
