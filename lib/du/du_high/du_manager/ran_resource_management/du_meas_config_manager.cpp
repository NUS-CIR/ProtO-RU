// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "du_meas_config_manager.h"
#include "du_ue_resource_config.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/dl_dcch_msg_ies.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg_ies.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/sr_configuration.h"
#include "ocudu/ran/ssb/ssb_properties.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/rrm/ue_capability_summary.h"
#include "ocudu/support/enum_utils.h"
#include <algorithm>
#include <array>
#include <limits>
#include <numeric>
#include <optional>
#include <vector>

using namespace ocudu;
using namespace odu;
using namespace asn1::rrc_nr;

static bool unpack_meas_cfg(meas_cfg_s& meas_cfg, const byte_buffer& container)
{
  asn1::cbit_ref bref{container};
  return meas_cfg.unpack(bref) == asn1::OCUDUASN_SUCCESS;
}

namespace {

// Default Measurement Gap Length for a given PCell SCS and SMTC duration.
meas_gap_length get_default_mgl(subcarrier_spacing scs, ssb_mtc_s::dur_opts::options dur)
{
  switch (dur) {
    case ssb_mtc_s::dur_opts::sf1:
      return scs != subcarrier_spacing::kHz15 ? meas_gap_length::ms1dot5 : meas_gap_length::ms3;
    case ssb_mtc_s::dur_opts::sf2:
      return meas_gap_length::ms3;
    case ssb_mtc_s::dur_opts::sf3:
      return meas_gap_length::ms4;
    case ssb_mtc_s::dur_opts::sf4:
    case ssb_mtc_s::dur_opts::sf5:
      return meas_gap_length::ms6;
    default:
      report_fatal_error("Invalid SSB MTC duration");
  }
}

std::pair<ssb_periodicity, unsigned> extract_smtc_period_offset(const ssb_mtc_s& smtc1)
{
  using opts = ssb_mtc_s::periodicity_and_offset_c_::types_opts;
  switch (smtc1.periodicity_and_offset.type().value) {
    case opts::sf5:
      return {ssb_periodicity::ms5, smtc1.periodicity_and_offset.sf5()};
    case opts::sf10:
      return {ssb_periodicity::ms10, smtc1.periodicity_and_offset.sf10()};
    case opts::sf20:
      return {ssb_periodicity::ms20, smtc1.periodicity_and_offset.sf20()};
    case opts::sf40:
      return {ssb_periodicity::ms40, smtc1.periodicity_and_offset.sf40()};
    case opts::sf80:
      return {ssb_periodicity::ms80, smtc1.periodicity_and_offset.sf80()};
    case opts::sf160:
      return {ssb_periodicity::ms160, smtc1.periodicity_and_offset.sf160()};
    default:
      report_fatal_error("Invalid SSB MTC periodicity_and_offset");
  }
}

enum class collision_check { strict, loose };

bool meas_gap_collides(const meas_gap_config&                   gap,
                       subcarrier_spacing                       scs,
                       span<const periodic_uci_config>          ul_occasions,
                       std::optional<std::chrono::microseconds> ul_ta,
                       collision_check                          mode)
{
  const unsigned mgrp_slots = static_cast<unsigned>(gap.mgrp) * get_nof_slots_per_subframe(scs);
  for (const auto& occ : ul_occasions) {
    // The pattern of MGRP-relative occasion positions repeats every LCM(MGRP, occ.period) slots, one SFN at most.
    // Checking it covers all phase shifts that occur when MGRP and the occasion period are not multiples of each other.
    const unsigned check_span_slots = std::lcm(mgrp_slots, occ.period_slots);
    if (mode == collision_check::strict) {
      // `strict` check collides when any periodic UL occasion repetition overlaps the measurement gap.
      for (unsigned slot = occ.offset_slots; slot < check_span_slots; slot += occ.period_slots) {
        if (is_inside_ul_meas_gap(gap, slot_point(scs, slot), ul_ta)) {
          return true;
        }
      }
    } else {
      // `loose` check collides when all periodic UL resource repetitions overlap the measurement gap.
      bool all_inside_meas_gap = true;
      for (unsigned slot = occ.offset_slots; slot < check_span_slots; slot += occ.period_slots) {
        if (!is_inside_ul_meas_gap(gap, slot_point(scs, slot), ul_ta)) {
          all_inside_meas_gap = false;
          break;
        }
      }
      if (all_inside_meas_gap) {
        return true;
      }
    }
  }
  return false;
}

} // namespace

meas_gap_config odu::create_meas_gap(subcarrier_spacing                       scs,
                                     const ssb_mtc_s&                         smtc1,
                                     span<const periodic_uci_config>          ul_occasions,
                                     std::optional<std::chrono::microseconds> ul_ta,
                                     const supported_meas_gap_patterns&       supported_patterns)
{
  // Shortest MGL that still encloses the SMTC window. Longer MGLs are only used if no supported gap pattern is found
  // with this one.
  const meas_gap_length default_mgl      = get_default_mgl(scs, smtc1.dur);
  const auto            smtc_po          = extract_smtc_period_offset(smtc1);
  const unsigned        smtc_period_ms   = static_cast<unsigned>(smtc_po.first);
  const unsigned        smtc_offset_ms   = smtc_po.second;
  const unsigned        smtc_duration_ms = smtc1.dur.to_number();
  const unsigned        slots_per_ms     = get_nof_slots_per_subframe(scs);

  // Measurement Gap Repetition Period must be at least as long as the SMTC period and all UL occasion periods
  // to ensure that one MGRP contains at least one SSB, SR and periodic CSI occasion we have to align it with.
  unsigned min_mgrp_ms = smtc_period_ms;
  for (const auto& occ : ul_occasions) {
    ocudu_assert(occ.period_slots > 0, "Periodic UL occasion must have a non-zero period");
    ocudu_assert(occ.offset_slots < occ.period_slots,
                 "Periodic UL occasion offset ({}) must be less than its period ({})",
                 occ.offset_slots,
                 occ.period_slots);
    const unsigned occ_period_ms = (occ.period_slots + slots_per_ms - 1) / slots_per_ms;
    min_mgrp_ms                  = std::max(min_mgrp_ms, occ_period_ms);
  }

  static constexpr std::array<meas_gap_repetition_period, 4> mgrp_candidates = {meas_gap_repetition_period::ms20,
                                                                                meas_gap_repetition_period::ms40,
                                                                                meas_gap_repetition_period::ms80,
                                                                                meas_gap_repetition_period::ms160};

  // Returns true if a (MGL, MGRP) gap pattern can be considered: it must be long enough to fit the SMTC period and all
  // UL occasions, and it must be one of the gap patterns supported by the UE (TS 38.133 Table 9.1.2-1).
  auto is_eligible = [&](meas_gap_length mgl, meas_gap_repetition_period mgrp) {
    return static_cast<unsigned>(mgrp) >= min_mgrp_ms and supported_patterns.is_supported(mgl, mgrp);
  };

  // Searches for a non-colliding gap config restricted to gap patterns using the given MGL.
  auto find_for_mgl = [&](meas_gap_length mgl) -> std::optional<meas_gap_config> {
    // Maximum integer ms we can slide the measGap left of an SMTC start while still fully enclosing the SMTC window.
    const int offset_slack_ms = static_cast<int>(meas_gap_length_to_msec(mgl)) - static_cast<int>(smtc_duration_ms);
    ocudu_assert(offset_slack_ms >= 0,
                 "MGL must be >= SMTC duration (got MGL={}ms, SMTC duration={}ms)",
                 meas_gap_length_to_msec(mgl),
                 smtc_duration_ms);

    // Searches for a gap offset within MGRP that fully encloses some SMTC window and passes the
    // collision check. Each SMTC repetition offers `offset_slack_ms + 1` candidate offsets — sliding
    // the gap left by 0..slack ms keeps the SMTC fully inside the gap.
    auto find_non_colliding_offset = [&](meas_gap_repetition_period mgrp,
                                         collision_check            mode) -> std::optional<meas_gap_config> {
      const unsigned mgrp_ms = static_cast<unsigned>(mgrp);
      // All SSB repetitions in a given MGRP.
      for (unsigned smtc_start_ms = smtc_offset_ms; smtc_start_ms < mgrp_ms; smtc_start_ms += smtc_period_ms) {
        // All measGap offsets that still allow to fully catch the SSB.
        for (int shift_ms = 0; shift_ms <= offset_slack_ms; ++shift_ms) {
          const unsigned        gap_offset_ms = (smtc_start_ms + mgrp_ms - shift_ms) % mgrp_ms;
          const meas_gap_config candidate{gap_offset_ms, mgl, mgrp};
          if (!meas_gap_collides(candidate, scs, ul_occasions, ul_ta, mode)) {
            return candidate;
          }
        }
      }
      return std::nullopt;
    };

    // Search for strictly non-colliding measGap offset at the shortest eligible MGRP.
    for (auto mgrp : mgrp_candidates) {
      if (not is_eligible(mgl, mgrp)) {
        continue;
      }
      if (auto candidate = find_non_colliding_offset(mgrp, collision_check::strict)) {
        return candidate;
      }
      break;
    }

    // Search for non-colliding measGap offset that allows all periodic UL occasions to have at least one non-colliding
    // instance in a given MGRP.
    for (auto mgrp : mgrp_candidates) {
      if (not is_eligible(mgl, mgrp)) {
        continue;
      }
      if (auto candidate = find_non_colliding_offset(mgrp, collision_check::loose)) {
        return candidate;
      }
    }

    return std::nullopt;
  };

  // Prefer the shortest MGL that encloses the SMTC, escalating to longer (still SMTC-enclosing) MGLs only if no
  // supported gap pattern can be found with the shorter one.
  for (unsigned mgl_idx = static_cast<unsigned>(default_mgl); mgl_idx <= static_cast<unsigned>(meas_gap_length::ms6);
       ++mgl_idx) {
    if (auto candidate = find_for_mgl(static_cast<meas_gap_length>(mgl_idx))) {
      return *candidate;
    }
  }

  // Best-effort fallback: no supported gap pattern could be placed without collisions while honoring the SMTC period.
  // Pick the supported, SMTC-enclosing gap pattern with the largest MGRP (fewest gap occurrences, hence fewest
  // collisions), aligned with the SMTC offset. Some UL occasions may still collide.
  std::optional<meas_gap_pattern> fallback;
  for (unsigned mgl_idx = static_cast<unsigned>(default_mgl); mgl_idx <= static_cast<unsigned>(meas_gap_length::ms6);
       ++mgl_idx) {
    const auto mgl = static_cast<meas_gap_length>(mgl_idx);
    for (auto mgrp : mgrp_candidates) {
      if (not supported_patterns.is_supported(mgl, mgrp)) {
        continue;
      }
      if (not fallback.has_value() or static_cast<unsigned>(mgrp) > static_cast<unsigned>(fallback->mgrp)) {
        fallback = meas_gap_pattern{mgl, mgrp};
      }
    }
  }
  if (fallback.has_value()) {
    const unsigned mgrp_ms = static_cast<unsigned>(fallback->mgrp);
    return meas_gap_config{smtc_offset_ms % mgrp_ms, fallback->mgl, fallback->mgrp};
  }

  // No supported enclosing pattern was found (should not happen, since gap patterns 0 and 1 are always supported).
  // Fall back to gap pattern 1 (MGL=6ms, MGRP=80ms), which is always supported and encloses any SMTC window.
  static constexpr unsigned mandatory_mgrp_ms = static_cast<unsigned>(meas_gap_repetition_period::ms80);
  return meas_gap_config{smtc_offset_ms % mandatory_mgrp_ms, meas_gap_length::ms6, meas_gap_repetition_period::ms80};
}

du_meas_config_manager::du_meas_config_manager(span<const du_cell_config> cell_cfg_list_) :
  cell_cfg_list(cell_cfg_list_), logger(ocudulog::fetch_basic_logger("DU-MNG"))
{
}

// Collects SR and periodic-CSI occasions from the UE's PCell serving cell config.
static std::vector<periodic_uci_config> collect_ul_occasions(const ue_cell_config& pcell_ue_cfg,
                                                             const du_cell_config& pcell_common)
{
  std::vector<periodic_uci_config> out;

  if (pcell_ue_cfg.serv_cell_cfg.ul_config.has_value() &&
      pcell_ue_cfg.serv_cell_cfg.ul_config->init_ul_bwp.pucch_cfg.has_value()) {
    for (const auto& sr : pcell_ue_cfg.serv_cell_cfg.ul_config->init_ul_bwp.pucch_cfg->sr_res_list) {
      out.push_back({sr_periodicity_to_slot(sr.period), sr.offset});
    }
  }

  const auto& periodic_csi = pcell_ue_cfg.bwps.front().ul.csi.periodic_report;
  if (periodic_csi.has_value() and pcell_common.ran.init_bwp.csi.has_value()) {
    const csi_report_periodicity period =
        csi_helper::convert_csi_resource_period_to_report_period(pcell_common.ran.init_bwp.csi->csi_rs_period);
    out.push_back({to_underlying(period), periodic_csi->offset});
  }

  return out;
}

bool du_meas_config_manager::update(du_ue_resource_config&       ue_cfg,
                                    const byte_buffer&           meas_cfg,
                                    const byte_buffer&           location_meas_info,
                                    const ue_capability_summary* ue_caps)
{
  const supported_meas_gap_patterns supported_patterns =
      ue_caps != nullptr ? ue_caps->supported_meas_gaps : supported_meas_gap_patterns{};

  if (not meas_cfg.empty()) {
    update_ssb_meas_gap(ue_cfg, meas_cfg, supported_patterns);
    if (location_meas_info.empty()) {
      // The CU-CP sends the LocationMeasurementInfo with each measConfig until the UE stops the location measurements.
      ue_cfg.prs_meas_gaps.clear();
      ue_cfg.meas_gap = ue_cfg.ssb_meas_gap;
      return true;
    }
  }
  if (location_meas_info.empty()) {
    return true;
  }
  return update_location_meas(ue_cfg, location_meas_info, supported_patterns);
}

void du_meas_config_manager::update_ssb_meas_gap(du_ue_resource_config&             ue_cfg,
                                                 const byte_buffer&                 packed_meas_cfg,
                                                 const supported_meas_gap_patterns& supported_patterns)
{
  meas_cfg_s meas_cfg;
  if (not unpack_meas_cfg(meas_cfg, packed_meas_cfg)) {
    logger.error("Failed to unpack meas config. Discarding it...");
    return;
  }

  const ue_cell_config& pcell_ue_cfg = ue_cfg.cell_group.cells.at(SERVING_PCELL_IDX);
  const du_cell_config& pcell_common = cell_cfg_list[pcell_ue_cfg.serv_cell_cfg.cell_index];

  const auto ul_occasions = collect_ul_occasions(pcell_ue_cfg, pcell_common);

  // The periodic UL occasions are expressed in the UE uplink frame numbering (TS 38.213, Section 9.2.4 and
  // TS 38.214, Section 5.2.1.4), while the gap is anchored to the UE downlink timing, so the UEs' uplink timing
  // advance is needed to compare the two.
  const std::optional<std::chrono::microseconds> ul_ta = get_ref_location_ul_ta(pcell_common.ran.ntn_params);
  if (not ul_ta.has_value() and pcell_common.ran.ntn_params.has_value() and pcell_common.ran.ntn_params->is_enabled()) {
    // With no T_TA the offset is placed as if it were zero, i.e. against the UE downlink timeline rather than its
    // uplink one. Correctness holds - is_ul_enabled() re-tests the window per slot - but in GEO, where T_TA barely
    // moves, a gap landing on an SR or CSI occasion shadows it for the whole connection.
    //
    // TODO: place the offset against the range of T_TA the cell will see, sourced from the NTN configuration manager.
    // Where no offset clears the occasions, the SR or CSI periodicity has to be picked alongside the gap. Reached on
    // every NTN cell today: ref_location_ul_ta only updates the du_cell_manager copy, not the one spanned here.
    logger.debug("cell={}: Choosing a measurement gap offset with no uplink timing advance estimate",
                 pcell_ue_cfg.serv_cell_cfg.cell_index);
  }

  for (const auto& asn1measobj : meas_cfg.meas_obj_to_add_mod_list) {
    if (asn1measobj.meas_obj.type().value != meas_obj_to_add_mod_s::meas_obj_c_::types_opts::meas_obj_nr) {
      logger.warning("Ignoring measObject of type {}. Cause: Unsupported", asn1measobj.meas_obj.type().to_string());
      continue;
    }
    const auto& asn1nr = asn1measobj.meas_obj.meas_obj_nr();

    if (not asn1nr.ssb_freq_present or not asn1nr.smtc1_present) {
      logger.info("Ignoring measObject of type {}. Cause: Lack of a SSB frequency or SMTC1 config",
                  asn1measobj.meas_obj.type().to_string());
      continue;
    }

    if (asn1nr.ssb_freq == pcell_common.ran.dl_cfg_common.freq_info_dl.absolute_frequency_ssb) {
      // Same frequency. No need for measGap.
      continue;
    }

    ue_cfg.ssb_meas_gap = create_meas_gap(pcell_common.ran.dl_cfg_common.init_dl_bwp.generic_params.scs,
                                          asn1nr.smtc1,
                                          ul_occasions,
                                          ul_ta,
                                          supported_patterns);
  }
}

// Returns the measurement gap required by the PRS of one frequency layer, whose fields share the values of a GapConfig,
// as per TS 38.331.
static std::optional<meas_gap_config> make_prs_meas_gap(const nr_prs_meas_info_r16_s& prs_info)
{
  const auto mgl = static_cast<meas_gap_length>(prs_info.nr_meas_prs_len_r16.value);

  using repeat_opts             = nr_prs_meas_info_r16_s::nr_meas_prs_repeat_and_offset_r16_c_::types_opts;
  const auto& repeat_and_offset = prs_info.nr_meas_prs_repeat_and_offset_r16;
  switch (repeat_and_offset.type().value) {
    case repeat_opts::ms20_r16:
      return meas_gap_config{repeat_and_offset.ms20_r16(), mgl, meas_gap_repetition_period::ms20};
    case repeat_opts::ms40_r16:
      return meas_gap_config{repeat_and_offset.ms40_r16(), mgl, meas_gap_repetition_period::ms40};
    case repeat_opts::ms80_r16:
      return meas_gap_config{repeat_and_offset.ms80_r16(), mgl, meas_gap_repetition_period::ms80};
    case repeat_opts::ms160_r16:
      return meas_gap_config{repeat_and_offset.ms160_r16(), mgl, meas_gap_repetition_period::ms160};
    default:
      return std::nullopt;
  }
}

// Returns the shortest gap, of at most \c max_mgl and with a gap pattern supported by the UE, that encloses one
// occurrence of each gap.
static std::optional<meas_gap_config> make_enclosing_gap(span<const meas_gap_config>        gaps,
                                                         meas_gap_length                    max_mgl,
                                                         const supported_meas_gap_patterns& supported_patterns)
{
  // The periods are 20*2^i ms, so the gap with the smallest period can cover all other gaps.
  const unsigned min_period_ms = static_cast<unsigned>(
      std::min_element(gaps.begin(), gaps.end(), [](const meas_gap_config& lhs, const meas_gap_config& rhs) {
        return static_cast<unsigned>(lhs.mgrp) < static_cast<unsigned>(rhs.mgrp);
      })->mgrp);

  // A shorter gap period also divides all the periods, so it is tried when the UE does not support a gap pattern with
  // the smallest one, at the cost of more frequent gaps.
  static constexpr unsigned min_mgrp_ms = static_cast<unsigned>(meas_gap_repetition_period::ms20);
  for (unsigned mgrp_ms = min_period_ms; mgrp_ms >= min_mgrp_ms; mgrp_ms /= 2) {
    // The gaps wrap around the gap period, so each gap start is tried as the gap offset, keeping the one that encloses
    // all the gaps in the shortest span.
    unsigned gap_offset_ms = 0;
    float    gap_span_ms   = std::numeric_limits<float>::max();
    for (const meas_gap_config& anchor : gaps) {
      const unsigned candidate_offset_ms = anchor.offset % mgrp_ms;
      float          span_ms             = 0;
      for (const meas_gap_config& g : gaps) {
        const unsigned offset_ms = g.offset % mgrp_ms;
        // A gap starting before the candidate is enclosed by its occurrence in the next gap period.
        const unsigned start_ms = offset_ms >= candidate_offset_ms ? offset_ms - candidate_offset_ms
                                                                   : offset_ms + mgrp_ms - candidate_offset_ms;
        span_ms                 = std::max(span_ms, start_ms + meas_gap_length_to_msec(g.mgl));
      }
      if (span_ms < gap_span_ms) {
        gap_offset_ms = candidate_offset_ms;
        gap_span_ms   = span_ms;
      }
    }

    const auto mgrp = static_cast<meas_gap_repetition_period>(mgrp_ms);
    for (unsigned mgl_idx = 0; mgl_idx <= static_cast<unsigned>(max_mgl); ++mgl_idx) {
      const auto mgl = static_cast<meas_gap_length>(mgl_idx);
      if (meas_gap_length_to_msec(mgl) >= gap_span_ms and supported_patterns.is_supported(mgl, mgrp)) {
        return meas_gap_config{gap_offset_ms, mgl, mgrp};
      }
    }
  }
  return std::nullopt;
}

// The UE supports a single gap, as per TS 38.331 gapUE, so the gap of the PRS must also enclose the SSB gap to keep the
// SSB measurements running.
static std::optional<meas_gap_config> make_prs_gap(const std::optional<meas_gap_config>& ssb_gap,
                                                   span<const meas_gap_config>           prs_meas_gaps,
                                                   const supported_meas_gap_patterns&    supported_patterns)
{
  static_vector<meas_gap_config, MAX_NOF_PRS_FREQ_LAYERS + 1> gaps(prs_meas_gaps.begin(), prs_meas_gaps.end());
  if (ssb_gap.has_value()) {
    gaps.push_back(*ssb_gap);
  }

  // Gaps of 10ms and 20ms can only be configured if a PRS needs them, as per TS 38.133, Table 9.1.2-3, NOTE 8.
  // Thus, do not use them only to enclose several PRS and SSB gaps together.
  meas_gap_length max_mgl = meas_gap_length::ms6;
  for (const meas_gap_config& prs_meas_gap : prs_meas_gaps) {
    max_mgl = std::max(max_mgl, prs_meas_gap.mgl);
  }
  return make_enclosing_gap(gaps, max_mgl, supported_patterns);
}

bool du_meas_config_manager::update_location_meas(du_ue_resource_config&             ue_cfg,
                                                  const byte_buffer&                 packed_location_meas_info,
                                                  const supported_meas_gap_patterns& supported_patterns)
{
  location_meas_info_c location_meas_info;
  asn1::cbit_ref       bref{packed_location_meas_info};
  if (location_meas_info.unpack(bref) != asn1::OCUDUASN_SUCCESS) {
    logger.error("Failed to unpack LocationMeasurementInfo");
    return false;
  }

  if (location_meas_info.type().value != location_meas_info_c::types_opts::nr_prs_meas_r16) {
    logger.warning("Rejecting LocationMeasurementInfo. Cause: Unsupported type {}",
                   location_meas_info.type().to_string());
    return false;
  }
  static_vector<meas_gap_config, MAX_NOF_PRS_FREQ_LAYERS> prs_meas_gaps;
  for (const nr_prs_meas_info_r16_s& prs_info : location_meas_info.nr_prs_meas_r16()) {
    const std::optional<meas_gap_config> prs_meas_gap = make_prs_meas_gap(prs_info);
    if (not prs_meas_gap.has_value()) {
      logger.warning("Rejecting LocationMeasurementInfo. Cause: Unsupported PRS repetition period {}",
                     prs_info.nr_meas_prs_repeat_and_offset_r16.type().to_string());
      return false;
    }
    prs_meas_gaps.push_back(*prs_meas_gap);
  }

  const std::optional<meas_gap_config> gap = make_prs_gap(ue_cfg.ssb_meas_gap, prs_meas_gaps, supported_patterns);
  if (not gap.has_value()) {
    logger.warning("Rejecting LocationMeasurementInfo. Cause: PRS do not fit in a single gap supported by the UE with "
                   "the SSB measurement gap");
    return false;
  }
  ue_cfg.prs_meas_gaps = std::move(prs_meas_gaps);
  ue_cfg.meas_gap      = gap;
  return true;
}
