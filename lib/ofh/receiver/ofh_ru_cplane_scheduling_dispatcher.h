// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_ru_rx_cplane_data_flow.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ofh_constants.h"

namespace ocudu {
namespace ofh {

/// \brief Handles a downlink scheduling Control-Plane command at the O-RU.
///
/// A downlink command tells the O-RU that the O-DU is about to send downlink User-Plane data; the handler prepares the
/// O-RU to receive it (e.g. allocates the resource grid the data is written into).
class ru_downlink_scheduling_handler
{
public:
  virtual ~ru_downlink_scheduling_handler() = default;

  virtual void handle_downlink_scheduling(unsigned eaxc, const cplane_message_decoder_results& results) = 0;
};

/// \brief Handles uplink and PRACH scheduling Control-Plane commands at the O-RU.
///
/// These commands tell the O-RU to capture uplink (or PRACH) and send it back to the O-DU as User-Plane data.
class ru_uplink_scheduling_handler
{
public:
  virtual ~ru_uplink_scheduling_handler() = default;

  virtual void handle_uplink_scheduling(unsigned eaxc, const cplane_message_decoder_results& results) = 0;
  virtual void handle_prach_scheduling(unsigned eaxc, const cplane_message_decoder_results& results)  = 0;
};

/// Configuration of the O-RU Control-Plane scheduling dispatcher.
struct ru_cplane_scheduling_dispatcher_config {
  /// Radio sector identifier.
  unsigned sector;
  /// Configured downlink eAxCs (the O-RU receives downlink for these).
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  /// Configured uplink eAxCs (the O-RU transmits uplink User-Plane for these).
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  /// Configured PRACH eAxCs (the O-RU transmits PRACH User-Plane for these).
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc;
};

/// \brief Routes a decoded received Control-Plane command to the matching O-RU scheduling handler.
///
/// PRACH/mixed-numerology commands (section type 3) go to the uplink handler's PRACH path; otherwise the command is
/// routed by its data direction (uplink commands to the uplink handler, downlink commands to the downlink handler). A
/// command whose eAxC is not configured for the resolved path is dropped, so the O-RU only acts on its own eAxCs.
class ru_cplane_scheduling_dispatcher : public ru_rx_cplane_notifier
{
public:
  ru_cplane_scheduling_dispatcher(const ru_cplane_scheduling_dispatcher_config& config,
                                  ocudulog::basic_logger&                       logger_,
                                  ru_downlink_scheduling_handler&               dl_handler_,
                                  ru_uplink_scheduling_handler&                 ul_handler_) :
    logger(logger_),
    dl_handler(dl_handler_),
    ul_handler(ul_handler_),
    sector_id(config.sector),
    dl_eaxc(config.dl_eaxc),
    ul_eaxc(config.ul_eaxc),
    prach_eaxc(config.prach_eaxc)
  {
  }

  // See interface for documentation.
  void on_cplane_message_received(unsigned eaxc, const cplane_message_decoder_results& results) override;

private:
  ocudulog::basic_logger&                         logger;
  ru_downlink_scheduling_handler&                 dl_handler;
  ru_uplink_scheduling_handler&                   ul_handler;
  const unsigned                                  sector_id;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc;
};

} // namespace ofh
} // namespace ocudu
