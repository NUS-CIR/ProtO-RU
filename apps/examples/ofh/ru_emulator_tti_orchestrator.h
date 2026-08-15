// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ru_emulator_sdr_upper_phy.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ru_sector.h"
#include "ocudu/phy/support/prach_buffer_context.h"
#include "ocudu/phy/support/resource_grid_pool.h"
#include "ocudu/phy/support/shared_prach_buffer.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include "ocudu/ru/ru_timing_notifier.h"
#include "ocudu/ru/ru_uplink_plane.h"
#include <atomic>
#include <memory>

namespace ocudu {

/// RU emulator SDR TTI orchestrator configuration.
struct ru_emulator_tti_orchestrator_config {
  /// Radio sector identifier.
  unsigned sector;
  /// RU operating bandwidth in PRBs.
  unsigned nof_prb;
  /// Number of uplink eAxCs, i.e. the number of ports of the uplink capture grid.
  unsigned nof_ul_ports;
  /// Subcarrier spacing (used as the PRACH context's PUSCH SCS).
  subcarrier_spacing scs;
  /// \brief PRACH peek lookback, in slots.
  ///
  /// The TTI for a slot fires max_proc_delay slots plus the lower PHY's one-millisecond baseband lead before its air
  /// time - earlier than the O-DU's PRACH Control-Plane can arrive within practical T2a windows. Peeking the occasion
  /// of the slot this many TTIs in the past (set it to max_proc_delay) issues the capture request one millisecond
  /// before the occasion's air time, after the request has been recorded but still ahead of the radio's reception.
  unsigned prach_peek_lookback_slots = 0;
};

/// \brief RU emulator SDR TTI orchestrator.
///
/// Driven by the SDR \ref radio_unit on the radio's timing (\ref ru_timing_notifier). On each TTI boundary it:
/// - requests the Radio Unit to capture the uplink for the slot (the captured IQ comes back through
///   \ref ru_emulator_rx_symbol_adapter into the O-RU sector's uplink IQ sink), and
/// - requests a PRACH capture when a PRACH Control-Plane occasion was recorded (peeked with a lookback, see
///   \ref ru_emulator_tti_orchestrator_config::prach_peek_lookback_slots).
///
/// The received downlink is not handled here: \ref ru_emulator_sdr_upper_phy pushes the completed grids straight to
/// the Radio Unit when the reception window finalizes them. It is the SDR-mode counterpart of ProtO-RU's
/// \c upper_phy_fake TTI-boundary handler, but the uplink matching and User-Plane encoding now live in the O-RU
/// library.
class ru_emulator_tti_orchestrator : public ru_timing_notifier
{
public:
  ru_emulator_tti_orchestrator(const ru_emulator_tti_orchestrator_config& config,
                               ocudulog::basic_logger&                    logger_,
                               ru_emulator_sdr_upper_phy&                 sdr_upper_phy_,
                               ofh::ru_sector&                            sector_,
                               std::unique_ptr<resource_grid_pool>        ul_capture_grid_pool_,
                               std::unique_ptr<prach_buffer_pool>         prach_capture_buffer_pool_);

  /// Out-of-line so the PRACH buffer pool (holding the otherwise-incomplete type prach_buffer) is destroyed where it
  /// is complete.
  ~ru_emulator_tti_orchestrator() override;

  /// Connects the Radio Unit's uplink plane handler (after the Radio Unit has been created with this orchestrator as
  /// its timing notifier).
  void connect(ru_uplink_plane_handler& ul_handler_) { ul_handler = &ul_handler_; }

  /// \brief Sets the offset, in slots, from the radio slot numbering to the GPS/OFH slot numbering.
  ///
  /// The O-RU sector and the SDR upper PHY key everything by the O-DU's GPS-derived wire slots, while the radio
  /// numbers its TTIs from its SFN0 anchor; this offset translates between the two (see
  /// \ref gps_slot_alignment::calculate_gps_slot_offset). Must be set before the radio starts ticking TTIs.
  void set_gps_slot_offset(unsigned offset) { gps_slot_offset.store(offset, std::memory_order_relaxed); }

  // See interface for documentation.
  void on_tti_boundary(const tti_boundary_context& slot_context) override;

  // See interface for documentation.
  void on_ul_half_slot_boundary(slot_point /* slot */) override {}

  // See interface for documentation.
  void on_ul_full_slot_boundary(slot_point /* slot */) override {}

private:
  ocudulog::basic_logger&             logger;
  ru_emulator_sdr_upper_phy&          sdr_upper_phy;
  ofh::ru_sector&                     sector;
  std::unique_ptr<resource_grid_pool> ul_capture_grid_pool;
  std::unique_ptr<prach_buffer_pool>  prach_capture_buffer_pool;
  const unsigned                      sector_id;
  /// Hardcoded PRACH capture context (short format B4); slot and start symbol are filled per occasion. The PRACH
  /// detector configuration is not carried by the O-RAN Control-Plane, so the O-RU emulator assumes it here.
  prach_buffer_context prach_context_template;
  /// PRACH peek lookback, in slots (see the configuration).
  const unsigned prach_peek_lookback_slots;
  /// Offset from the radio slot numbering to the GPS/OFH slot numbering, in slots.
  std::atomic<unsigned>    gps_slot_offset{0};
  ru_uplink_plane_handler* ul_handler = nullptr;
  // Diagnostics: TTIs seen, uplink captures requested and PRACH captures requested.
  uint64_t nof_tti           = 0;
  uint64_t nof_ul_capture    = 0;
  uint64_t nof_prach_capture = 0;
};

/// Creates an RU emulator SDR TTI orchestrator.
std::unique_ptr<ru_emulator_tti_orchestrator>
create_ru_emulator_tti_orchestrator(const ru_emulator_tti_orchestrator_config& config,
                                    ocudulog::basic_logger&                    logger,
                                    ru_emulator_sdr_upper_phy&                 sdr_upper_phy,
                                    ofh::ru_sector&                            sector);

} // namespace ocudu
