// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ru_emulator_tti_orchestrator.h"
#include "ru_emulator_gps_slot_alignment.h"
#include "ocudu/phy/support/prach_buffer.h"
#include "ocudu/phy/support/resource_grid.h"
#include "ocudu/phy/support/resource_grid_context.h"
#include "ocudu/phy/support/support_factories.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/prach/prach_format_type.h"
#include "ocudu/ran/prach/prach_subcarrier_spacing.h"
#include "ocudu/ran/prach/restricted_set_config.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/support/error_handling.h"
#include <algorithm>
#include <vector>

using namespace ocudu;

namespace {

/// Number of grids held by the uplink capture pool. Covers the slots in flight between the capture request and the
/// reception of the captured symbols.
constexpr unsigned NOF_UL_CAPTURE_GRIDS = 40;

/// Number of PRACH buffers held by the PRACH capture pool. PRACH is sparse, so a small pool covers the few captures
/// in flight between request and reception.
constexpr unsigned NOF_PRACH_CAPTURE_BUFFERS = 8;

/// Converts the radio TTI's extended slot point into the slot point numbering used by the O-RU sector.
slot_point to_slot_point(slot_point_extended slot)
{
  return slot_point(slot.scs(), slot.count() % slot.nof_slots_per_hyper_system_frame());
}

/// \brief Builds the hardcoded PRACH capture context shared by every occasion (slot and start symbol are filled in per
/// occasion). The O-RAN Control-Plane does not carry the PRACH detector configuration, so the O-RU emulator assumes a
/// short format B4 with a single time/frequency-domain occasion. PRACH is captured on every uplink antenna (one capture
/// port per uplink eAxC), so a MIMO O-DU receives a PRACH User-Plane reply on each branch.
prach_buffer_context
make_prach_context_template(unsigned sector, unsigned nof_prb, unsigned nof_ul_ports, subcarrier_spacing scs)
{
  prach_buffer_context tmpl = {};
  for (unsigned port = 0; port != nof_ul_ports; ++port) {
    tmpl.ports.push_back(port);
  }
  tmpl.nof_prb_ul_grid  = nof_prb;
  tmpl.rb_offset        = 0;
  tmpl.sector           = sector;
  tmpl.format           = prach_format_type::B4;
  tmpl.nof_td_occasions = 1;
  tmpl.nof_fd_occasions = 1;
  tmpl.pusch_scs        = scs;
  // The remaining fields configure the PRACH detector (run by the O-DU, not the O-RU's capture path), so they are left
  // at neutral defaults: the lower PHY's PRACH window capture only uses slot, start_symbol, format, occasions, ports,
  // rb_offset, nof_prb_ul_grid and pusch_scs.
  tmpl.root_sequence_index   = 0;
  tmpl.restricted_set        = restricted_set_config::UNRESTRICTED;
  tmpl.zero_correlation_zone = 0;
  tmpl.start_preamble_index  = 0;
  tmpl.nof_preamble_indices  = 64;

  return tmpl;
}

} // namespace

ru_emulator_tti_orchestrator::ru_emulator_tti_orchestrator(
    const ru_emulator_tti_orchestrator_config& config,
    ocudulog::basic_logger&                    logger_,
    ru_emulator_sdr_upper_phy&                 sdr_upper_phy_,
    ofh::ru_sector&                            sector_,
    std::unique_ptr<resource_grid_pool>        ul_capture_grid_pool_,
    std::unique_ptr<prach_buffer_pool>         prach_capture_buffer_pool_) :
  logger(logger_),
  sdr_upper_phy(sdr_upper_phy_),
  sector(sector_),
  ul_capture_grid_pool(std::move(ul_capture_grid_pool_)),
  prach_capture_buffer_pool(std::move(prach_capture_buffer_pool_)),
  sector_id(config.sector),
  prach_context_template(
      make_prach_context_template(config.sector, config.nof_prb, std::max(1U, config.nof_ul_ports), config.scs)),
  prach_peek_lookback_slots(config.prach_peek_lookback_slots)
{
  ocudu_assert(ul_capture_grid_pool, "Invalid uplink capture grid pool");
  ocudu_assert(prach_capture_buffer_pool, "Invalid PRACH capture buffer pool");
}

ru_emulator_tti_orchestrator::~ru_emulator_tti_orchestrator() = default;

void ru_emulator_tti_orchestrator::on_tti_boundary(const tti_boundary_context& slot_context)
{
  // The Radio Unit's plane handler is connected after it is created; ignore boundaries until then.
  if (ul_handler == nullptr) {
    return;
  }

  // The radio numbers its TTIs from its SFN0 anchor; the O-RU sector keys everything by the O-DU's GPS-derived wire
  // slots. The radio handlers take the radio slot, the OFH-side lookups the translated one.
  slot_point radio_slot = to_slot_point(slot_context.slot);
  slot_point ofh_slot   = gps_slot_alignment::to_ofh_slot(radio_slot, gps_slot_offset.load(std::memory_order_relaxed));

  // Keep the SDR upper PHY's view of the radio time fresh: it disambiguates the wire-to-radio translation of the
  // downlink grids it pushes (the wire numbering wraps every 256 frames, the radio's every 1024).
  sdr_upper_phy.set_current_radio_slot(radio_slot);

  resource_grid_context context;
  context.slot   = radio_slot;
  context.sector = sector_id;

  // Request the Radio Unit to capture the uplink for this slot. The captured IQ is notified back through the received
  // symbol path into the O-RU sector's uplink IQ sink.
  shared_resource_grid ul_grid = ul_capture_grid_pool->allocate_resource_grid(radio_slot);
  if (ul_grid.is_valid()) {
    ul_handler->handle_new_uplink_slot(context, ul_grid);
    ++nof_ul_capture;
  } else {
    logger.warning("Sector#{}: no uplink capture grid available for slot '{}'", sector_id, radio_slot);
  }

  // If the O-DU scheduled a PRACH (recorded from its PRACH Control-Plane), request the Radio Unit to capture the PRACH
  // window. The occasion is peeked with a lookback: this TTI fires too early for its own slot's PRACH Control-Plane to
  // have arrived, so peek the slot whose air time is one millisecond away instead (its request has been recorded by
  // now, and the radio receives its samples only after air time). The captured window comes back through the
  // received-symbol path into the sector's uplink IQ sink, where it is matched against the recorded PRACH
  // Control-Plane and turned into a PRACH User-Plane reply.
  slot_point prach_radio_slot = radio_slot - prach_peek_lookback_slots;
  slot_point prach_ofh_slot   = ofh_slot - prach_peek_lookback_slots;
  if (std::optional<ofh::ru_prach_occasion> occasion = sector.get_recorded_prach_occasion(prach_ofh_slot)) {
    // SDR capture is currently configured for short B4, whose PRACH SCS equals the PUSCH/cell SCS. Do not capture a
    // differently-spaced request using the wrong lower-PHY context.
    prach_subcarrier_spacing expected_prach_scs = to_ra_subcarrier_spacing(prach_context_template.pusch_scs);
    if (occasion->prach_scs != expected_prach_scs) {
      logger.warning("Sector#{}: cannot capture PRACH in slot '{}': Control-Plane PRACH SCS '{}' is incompatible with "
                     "the SDR short-B4 capture SCS '{}'",
                     sector_id,
                     prach_ofh_slot,
                     to_string(occasion->prach_scs),
                     to_string(expected_prach_scs));
    } else {
      shared_prach_buffer prach_buffer = prach_capture_buffer_pool->get();
      if (prach_buffer) {
        prach_buffer_context prach_ctx = prach_context_template;
        prach_ctx.slot                 = prach_radio_slot;
        prach_ctx.start_symbol         = occasion->start_symbol;
        prach_ctx.rb_offset            = occasion->rb_offset;
        ul_handler->handle_prach_occasion(prach_ctx, std::move(prach_buffer));
        ++nof_prach_capture;
      } else {
        logger.warning("Sector#{}: no PRACH capture buffer available for slot '{}'", sector_id, prach_radio_slot);
      }
    }
  }

  // Periodic diagnostics: report roughly once per second of TTIs (2000 slots at 30 kHz SCS). The downlink count comes
  // from the SDR upper PHY, which pushes completed grids to the radio on the reception timing.
  if ((++nof_tti % 2000) == 0) {
    logger.info(
        "Sector#{}: TTI orchestrator alive: tti={} ul_capture_req={} dl_tx={} prach_capture_req={} (slot={} ofh={})",
        sector_id,
        nof_tti,
        nof_ul_capture,
        sdr_upper_phy.get_nof_transmitted_dl_grids(),
        nof_prach_capture,
        radio_slot,
        ofh_slot);
  }
}

std::unique_ptr<ru_emulator_tti_orchestrator>
ocudu::create_ru_emulator_tti_orchestrator(const ru_emulator_tti_orchestrator_config& config,
                                           ocudulog::basic_logger&                    logger,
                                           ru_emulator_sdr_upper_phy&                 sdr_upper_phy,
                                           ofh::ru_sector&                            sector)
{
  std::shared_ptr<resource_grid_factory> rg_factory = create_resource_grid_factory();
  report_error_if_not(rg_factory, "Failed to create resource grid factory for the RU emulator TTI orchestrator");

  const unsigned nof_subc     = config.nof_prb * NOF_SUBCARRIERS_PER_RB;
  const unsigned nof_symbols  = get_nsymb_per_slot(cyclic_prefix::NORMAL);
  const unsigned nof_ul_ports = std::max(1U, config.nof_ul_ports);

  std::vector<std::unique_ptr<resource_grid>> ul_grids;
  for (unsigned i = 0; i != NOF_UL_CAPTURE_GRIDS; ++i) {
    ul_grids.push_back(rg_factory->create(nof_ul_ports, nof_symbols, nof_subc));
  }
  auto ul_capture_grid_pool = create_generic_resource_grid_pool(std::move(ul_grids));

  // PRACH capture pool: short format B4, one capture port per uplink antenna and a single time / frequency-domain
  // occasion (see the context template). A real radio writes the captured PRACH window into one of these buffers.
  std::vector<std::unique_ptr<prach_buffer>> prach_buffers;
  for (unsigned i = 0; i != NOF_PRACH_CAPTURE_BUFFERS; ++i) {
    std::unique_ptr<prach_buffer> buffer = create_prach_buffer_short(nof_ul_ports, 1, 1);
    report_error_if_not(buffer, "Failed to create PRACH capture buffer for the RU emulator TTI orchestrator");
    prach_buffers.push_back(std::move(buffer));
  }
  auto prach_capture_buffer_pool = std::make_unique<prach_buffer_pool>(prach_buffers);

  return std::make_unique<ru_emulator_tti_orchestrator>(
      config, logger, sdr_upper_phy, sector, std::move(ul_capture_grid_pool), std::move(prach_capture_buffer_pool));
}
