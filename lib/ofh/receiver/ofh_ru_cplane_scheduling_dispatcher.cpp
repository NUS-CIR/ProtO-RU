// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_cplane_scheduling_dispatcher.h"
#include "ocudu/adt/span.h"
#include <algorithm>

using namespace ocudu;
using namespace ofh;

/// O-RAN Control-Plane section type carrying PRACH and mixed-numerology scheduling.
static constexpr uint8_t CPLANE_SECTION_TYPE_PRACH = 3;

/// Returns true if the given eAxC is present in the configured list.
static bool is_eaxc_configured(span<const unsigned> eaxcs, unsigned eaxc)
{
  return std::find(eaxcs.begin(), eaxcs.end(), eaxc) != eaxcs.end();
}

void ru_cplane_scheduling_dispatcher::on_cplane_message_received(unsigned                              eaxc,
                                                                 const cplane_message_decoder_results& results)
{
  if (results.section_type == CPLANE_SECTION_TYPE_PRACH) {
    if (!is_eaxc_configured(prach_eaxc, eaxc)) {
      logger.info(
          "Sector#{}: dropped received PRACH Control-Plane message for unconfigured eAxC '{}'", sector_id, eaxc);
      return;
    }
    ul_handler.handle_prach_scheduling(eaxc, results);
    return;
  }

  if (results.radio_hdr.direction == data_direction::uplink) {
    if (!is_eaxc_configured(ul_eaxc, eaxc)) {
      logger.info(
          "Sector#{}: dropped received uplink Control-Plane message for unconfigured eAxC '{}'", sector_id, eaxc);
      return;
    }
    ul_handler.handle_uplink_scheduling(eaxc, results);
    return;
  }

  if (!is_eaxc_configured(dl_eaxc, eaxc)) {
    logger.info(
        "Sector#{}: dropped received downlink Control-Plane message for unconfigured eAxC '{}'", sector_id, eaxc);
    return;
  }
  dl_handler.handle_downlink_scheduling(eaxc, results);
}
