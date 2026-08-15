// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_prach_window_responder.h"
#include "ocudu/phy/support/prach_buffer.h"
#include <algorithm>

using namespace ocudu;
using namespace ofh;

ru_prach_window_responder::ru_prach_window_responder(const ru_prach_window_responder_config&       config,
                                                     ocudulog::basic_logger&                       logger_,
                                                     std::shared_ptr<ru_uplink_request_repository> prach_request_repo_,
                                                     data_flow_uplane_data&                        prach_data_flow_) :
  logger(logger_),
  prach_request_repo(std::move(prach_request_repo_)),
  prach_data_flow(prach_data_flow_),
  prach_eaxc(config.prach_eaxc),
  sector_id(config.sector)
{
  ocudu_assert(prach_request_repo, "Invalid PRACH request repository");
}

void ru_prach_window_responder::handle_prach_window(slot_point slot, const prach_buffer& buffer)
{
  unsigned nof_ports = std::min<unsigned>(buffer.get_max_nof_ports(), prach_eaxc.size());

  for (unsigned port = 0; port != nof_ports; ++port) {
    unsigned                         eaxc    = prach_eaxc[port];
    std::optional<ru_uplink_request> request = prach_request_repo->get(slot, eaxc);

    // No PRACH request recorded for this slot/eAxC.
    if (!request) {
      continue;
    }

    data_flow_uplane_prach_context df_context;
    df_context.slot         = slot;
    df_context.sector       = sector_id;
    df_context.port         = port;
    df_context.eaxc         = eaxc;
    df_context.prb_start    = request->prb_start;
    df_context.nof_prb      = request->nof_prb;
    df_context.start_symbol = request->start_symbol;
    df_context.nof_symbols  = request->nof_symbols;
    df_context.filter_index = request->filter_index;
    df_context.prach_scs    = request->prach_scs;
    df_context.section_id   = request->section_id;

    logger.debug("Sector#{}: building PRACH User-Plane reply for slot '{}', eAxC '{}', '{}' symbols",
                 sector_id,
                 slot,
                 eaxc,
                 request->nof_symbols);
    prach_data_flow.enqueue_prach_message(df_context, buffer);

    // The request has been answered: clear it so it does not produce replies again (e.g. when the slot numbering wraps
    // around after the O-DU stopped sending Control-Plane).
    prach_request_repo->clear(slot, eaxc);
  }
}

std::optional<ru_prach_occasion> ru_prach_window_responder::peek_prach_occasion(slot_point slot) const
{
  for (unsigned eaxc : prach_eaxc) {
    if (std::optional<ru_uplink_request> request = prach_request_repo->get(slot, eaxc)) {
      return ru_prach_occasion{slot, eaxc, request->start_symbol, request->prach_start_rb, request->prach_scs};
    }
  }
  return std::nullopt;
}
