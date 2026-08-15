// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_rx_uplane_data_flow.h"
#include "ocudu/ofh/serdes/ofh_uplane_message_decoder_properties.h"

using namespace ocudu;
using namespace ofh;

ru_rx_uplane_data_flow::ru_rx_uplane_data_flow(const ru_rx_uplane_data_flow_config&  config,
                                               ru_rx_uplane_data_flow_dependencies&& dependencies) :
  logger(*dependencies.logger),
  uplane_decoder(std::move(dependencies.uplane_decoder)),
  rx_symbol_writer(config.eaxc, config.sector, *dependencies.logger, dependencies.grid_repo),
  sector_id(config.sector)
{
  ocudu_assert(uplane_decoder, "Invalid User-Plane decoder");
}

void ru_rx_uplane_data_flow::decode_message(unsigned eaxc, span<const uint8_t> message)
{
  uplane_message_decoder_results results;
  if (!uplane_decoder->decode(results, message)) {
    return;
  }

  // The symbol writer drops the message if no grid was prepared for this slot/symbol (i.e. no Control-Plane message was
  // received for it), so an O-RU does not need the O-DU's Control-Plane filter-index validation here.
  rx_symbol_writer.write_to_resource_grid(eaxc, results);
}
