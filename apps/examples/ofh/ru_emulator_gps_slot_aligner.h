// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ru_emulator_gps_slot_alignment.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ru/ru_timing_notifier.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <functional>

namespace ocudu {

/// \brief Timing-notifier decorator that measures and tracks the GPS slot offset at runtime.
///
/// Sits between the SDR Radio Unit and the TTI orchestrator. Every TTI notification carries the radio slot and the
/// host time the lower PHY stamped on it; pairing the two yields the offset between the radio slot numbering and the
/// GPS wire numbering (see \ref gps_slot_alignment) - the same measurement the original ProtO-RU took inside its
/// patched lower PHY at radio start. Because the pairing uses the host wall clock, it works with any radio clock
/// source (gpsdo, external or internal), as long as the host clock is GPS/NTP-accurate.
///
/// The first second of notifications is skipped (the downlink pipeline fill and the radio's deferred stream start make
/// them fire early); the offset is then established as the median of a measurement window and applied through the
/// callback. Measurement then continues for the lifetime of the run: with a disciplined radio clock (gpsdo, or a
/// shared external reference) the radio and host timelines do not drift, so the median never changes and the offset is
/// reported exactly once. With the radio on its free-running internal clock, the radio sample clock drifts against the
/// host/GPS timeline the offset is keyed to; the tracker re-applies the offset whenever the windowed median moves by a
/// slot (gated by a two-window hysteresis to reject host-clock jitter), keeping the wire-slot labelling correct as the
/// drift accumulates. Each re-alignment is a one-slot step, so it is a far smaller disturbance than the unbounded
/// labelling error a frozen offset would accrue - though a disciplined clock remains the only way to remove the drift
/// (and the carrier CFO it implies) entirely.
class ru_emulator_gps_slot_aligner : public ru_timing_notifier
{
public:
  ru_emulator_gps_slot_aligner(ru_timing_notifier&           next_,
                               ocudulog::basic_logger&       logger_,
                               std::function<void(unsigned)> on_offset_measured_) :
    next(next_), logger(logger_), on_offset_measured(std::move(on_offset_measured_))
  {
  }

  // See interface for documentation.
  void on_tti_boundary(const tti_boundary_context& context) override
  {
    measure(context);
    next.on_tti_boundary(context);
  }

  // See interface for documentation.
  void on_ul_half_slot_boundary(slot_point slot) override { next.on_ul_half_slot_boundary(slot); }

  // See interface for documentation.
  void on_ul_full_slot_boundary(slot_point slot) override { next.on_ul_full_slot_boundary(slot); }

private:
  /// \brief Lead between a TTI notification's time point and the notified slot's air time.
  ///
  /// The lower PHY throttles downlink baseband processing to stay one millisecond ahead of reception (see
  /// rx_to_tx_max_delay in the lower PHY factory), and stamps the notification with now + TTI advance while the slot
  /// airs at now + TTI advance + this lead. It is the same quantity the original ProtO-RU compensated with its
  /// rx_to_tx_max_delay calibration term.
  static constexpr std::chrono::milliseconds DL_PROCESSING_TO_AIR_LEAD{1};

  /// Number of TTI measurements each windowed median is taken over.
  static constexpr unsigned NOF_MEASUREMENTS = 64;

  void measure(const tti_boundary_context& context)
  {
    const subcarrier_spacing scs = context.slot.scs();

    // Skip the first second of notifications.
    const uint64_t settle_ttis = uint64_t(1000U) * get_nof_slots_per_subframe(scs);
    if (nof_ttis++ < settle_ttis) {
      return;
    }

    const slot_point radio_slot(to_numerology_value(scs),
                                context.slot.count() % gps_slot_alignment::nof_slots_per_ofh_period(scs));
    samples[nof_samples++] =
        gps_slot_alignment::measure_gps_slot_offset(context.time_point + DL_PROCESSING_TO_AIR_LEAD, radio_slot);
    if (nof_samples != samples.size()) {
      return;
    }
    nof_samples = 0;

    // Windowed median rejects the host-clock jitter on individual measurements.
    std::array<unsigned, NOF_MEASUREMENTS> sorted = samples;
    std::sort(sorted.begin(), sorted.end());
    const unsigned median = sorted[sorted.size() / 2];

    // Establish the offset on the first full window.
    if (!applied) {
      apply(median, /*initial=*/true);
      return;
    }

    // Already aligned and the median has not moved: nothing to do.
    if (median == applied_offset) {
      pending_valid = false;
      return;
    }

    // The median moved. Adopt it only once a second consecutive window agrees, so a one-off jitter excursion across a
    // slot boundary does not trigger a spurious re-alignment.
    if (pending_valid && median == pending_offset) {
      apply(median, /*initial=*/false);
      pending_valid = false;
      return;
    }
    pending_offset = median;
    pending_valid  = true;
  }

  void apply(unsigned offset, bool initial)
  {
    applied        = true;
    applied_offset = offset;
    on_offset_measured(offset);
    if (initial) {
      logger.info("Aligned the radio slot numbering to the GPS wire numbering: slot offset '{}'. Tracking the radio "
                  "clock drift against the host clock; keep the host clock GPS/NTP-synchronized.",
                  offset);
    } else {
      logger.info("Re-aligned the radio slot numbering to the GPS wire numbering: slot offset now '{}' (radio clock "
                  "drift). Use a gpsdo or shared external reference to remove the drift.",
                  offset);
    }
  }

  ru_timing_notifier&                    next;
  ocudulog::basic_logger&                logger;
  std::function<void(unsigned)>          on_offset_measured;
  std::array<unsigned, NOF_MEASUREMENTS> samples{};
  uint64_t                               nof_ttis    = 0;
  unsigned                               nof_samples = 0;
  /// Currently applied offset and whether one has been applied yet.
  bool     applied        = false;
  unsigned applied_offset = 0;
  /// Candidate offset awaiting a second confirming window (drift-tracking hysteresis).
  bool     pending_valid  = false;
  unsigned pending_offset = 0;
};

} // namespace ocudu
