// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_uplink_scheduling_recorder.h"
#include "ofh_ru_prach_frequency_mapping.h"

using namespace ocudu;
using namespace ofh;

/// Builds an uplink request from the decoded Control-Plane scheduling results.
static ru_uplink_request to_ru_uplink_request(const cplane_message_decoder_results& results)
{
  ru_uplink_request request;
  request.slot         = results.radio_hdr.slot;
  request.filter_index = results.radio_hdr.filter_index;
  request.start_symbol = results.radio_hdr.start_symbol;
  request.prb_start    = results.section.prb_start;
  request.nof_prb      = results.section.nof_prb;
  request.nof_symbols  = results.section.nof_symbols;
  request.section_id   = results.section.section_id;

  return request;
}

ru_uplink_scheduling_recorder::ru_uplink_scheduling_recorder(
    const ru_uplink_scheduling_recorder_config&   config,
    ocudulog::basic_logger&                       logger_,
    std::shared_ptr<ru_uplink_request_repository> ul_request_repo_,
    std::shared_ptr<ru_uplink_request_repository> prach_request_repo_) :
  logger(logger_),
  ul_request_repo(std::move(ul_request_repo_)),
  prach_request_repo(std::move(prach_request_repo_)),
  sector_id(config.sector),
  scs(config.scs),
  ru_nof_prbs(config.ru_nof_prbs)
{
  ocudu_assert(ul_request_repo, "Invalid uplink request repository");
  ocudu_assert(prach_request_repo, "Invalid PRACH request repository");
}

void ru_uplink_scheduling_recorder::handle_uplink_scheduling(unsigned                              eaxc,
                                                             const cplane_message_decoder_results& results)
{
  logger.debug("Sector#{}: recording uplink Control-Plane request for slot '{}' and eAxC '{}'",
               sector_id,
               results.radio_hdr.slot,
               eaxc);
  ul_request_repo->add(eaxc, to_ru_uplink_request(results));
}

void ru_uplink_scheduling_recorder::handle_prach_scheduling(unsigned                              eaxc,
                                                            const cplane_message_decoder_results& results)
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

  std::optional<unsigned> rb_offset = calculate_prach_rb_offset(results.frequency_offset, prach_scs, scs, ru_nof_prbs);
  if (!rb_offset) {
    logger.info("Sector#{}: dropped PRACH Control-Plane request for slot '{}' and eAxC '{}': frequency offset '{}' "
                "does not map to a valid PRACH allocation in a '{}'-PRB grid",
                sector_id,
                results.radio_hdr.slot,
                eaxc,
                results.frequency_offset,
                ru_nof_prbs);
    return;
  }

  logger.debug("Sector#{}: recording PRACH Control-Plane request for slot '{}' and eAxC '{}'",
               sector_id,
               results.radio_hdr.slot,
               eaxc);
  ru_uplink_request request = to_ru_uplink_request(results);
  request.prach_scs         = prach_scs;
  request.prach_start_rb    = *rb_offset;
  prach_request_repo->add(eaxc, request);
}
