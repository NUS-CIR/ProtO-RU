// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/config/periodic_resource_sched_validator.h"
#include "ocudu/adt/bounded_bitset.h"
#include "ocudu/adt/expected.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/csi_rs/csi_meas_config.h"
#include "ocudu/ran/csi_rs/csi_rs_config_helpers.h"
#include "ocudu/ran/csi_rs/csi_rs_pattern.h"
#include "ocudu/ran/csi_rs/frequency_allocation_type.h"
#include "ocudu/ran/prach/prach_configuration.h"
#include "ocudu/ran/prach/prach_frequency_mapping.h"
#include "ocudu/ran/prach/prach_preamble_information.h"
#include "ocudu/ran/prach/prach_time_mapping.h"
#include "ocudu/ran/prach/rach_config_common.h"
#include "ocudu/ran/prs/prs.h"
#include "ocudu/ran/resource_allocation/rb_interval.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/ran/srs/srs_configuration.h"
#include "ocudu/ran/srs/srs_configuration_helpers.h"
#include "ocudu/ran/srs/srs_information.h"
#include "ocudu/ran/srs/srs_resource_configuration.h"
#include "ocudu/ran/ssb/ssb_mapping.h"
#include "ocudu/ran/tdd/tdd_ul_dl_config.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include "ocudu/scheduler/sched_consts.h"
#include "ocudu/support/config/validator_helpers.h"
#include "ocudu/support/enum_utils.h"
#include "ocudu/support/math/math_utils.h"
#include <bitset>
#include <numeric>
#include <optional>

using namespace ocudu;

namespace {

/// \brief Identifies the origin of a \c periodic_occasion.
struct occasion_origin {
  enum class signal_type : uint8_t { SSB, NZP_CSI_RS, CSI_IM, PRS, SRS, PRACH, TDD_NON_DL, TDD_NON_UL } type;
  uint16_t primary_id;
  uint8_t  secondary_id;
};

/// \brief A resource occupancy that recurs periodically in time.
struct periodic_occasion {
  /// Identifies the static resource that this occasion originates from.
  occasion_origin origin;
  /// Periodicity, in slots, at which this occasion recurs.
  unsigned slot_period;
  /// Slot, within \c slot_period, at which this occasion occurs.
  unsigned slot_offset;
  /// \brief CRBs occupied by this occasion.
  bounded_bitset<MAX_NOF_PRBS> crbs;
  /// \brief RE mask per OFDM symbol.
  ///
  /// \remark Assumes the RE mask is the same for every CRB set in \c crbs.
  std::array<std::bitset<NOF_SUBCARRIERS_PER_RB>, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP> re_masks{};

  /// Checks whether this and \c other occasion can ever fall on the same slot.
  bool collides_in_slots(const periodic_occasion& other) const
  {
    return crt_solvable(slot_offset, slot_period, other.slot_offset, other.slot_period);
  }

  /// Checks whether this and \c other occasion collide in the resource grid.
  bool collides_in_res(const periodic_occasion& other) const
  {
    if (not(crbs & other.crbs).any()) {
      return false;
    }

    for (unsigned sym = 0; sym != NOF_OFDM_SYM_PER_SLOT_NORMAL_CP; ++sym) {
      if ((re_masks[sym] & other.re_masks[sym]).any()) {
        return true;
      }
    }
    return false;
  }
};

/// \brief Returns the CRBs actually occupied by a CSI-RS or CSI-IM resource whose frequency band is \c freq_band_rbs.
///
/// \c csi-FrequencyOccupation only allows a number of RBs that is a multiple of 4, so the configured band can extend
/// past the end of the BWP. As per TS 38.331, \c csi-FrequencyOccupation, the actual bandwidth of the resource is then
/// the width of the BWP.
crb_interval get_csi_occupied_crbs(const crb_interval& freq_band_rbs, const crb_interval& bwp_crbs)
{
  return freq_band_rbs & bwp_crbs;
}

/// \brief Returns a bitset marking every CRB in \c crbs as occupied.
bounded_bitset<MAX_NOF_PRBS> to_crb_bitset(const crb_interval& crbs)
{
  bounded_bitset<MAX_NOF_PRBS> bitset(MAX_NOF_PRBS);
  bitset.fill(crbs.start(), crbs.stop());
  return bitset;
}

/// \brief Description of a collision between two periodic occasions.
struct occasion_collision {
  /// Origins of the two colliding occasions.
  occasion_origin origin_a, origin_b;
  /// First absolute slot, modulo \c period, at which both occasions occur.
  unsigned slot;
  /// Period at which the collision occurs, in slots.
  unsigned period;
};

/// Returns the details of a collision between \c a and \c b, if one exists.
std::optional<occasion_collision> find_collision(const periodic_occasion& a, const periodic_occasion& b)
{
  if (not(a.collides_in_slots(b) and a.collides_in_res(b))) {
    return std::nullopt;
  }

  return occasion_collision{
      .origin_a = a.origin,
      .origin_b = b.origin,
      .slot     = crt(a.slot_offset, a.slot_period, b.slot_offset, b.slot_period),
      .period   = std::lcm(a.slot_period, b.slot_period),
  };
}

/// \brief Returns a human-readable identification of the static resource an occasion originates from.
std::string to_string(const occasion_origin& origin)
{
  switch (origin.type) {
    case occasion_origin::signal_type::SSB:
      return fmt::format("SSB occasion (ssb-Index={})", origin.primary_id);
    case occasion_origin::signal_type::NZP_CSI_RS:
      return fmt::format("NZP-CSI-RS resource (nzp-CSI-RS-ResourceId={})", origin.primary_id);
    case occasion_origin::signal_type::CSI_IM:
      return fmt::format("CSI-IM resource (csi-IM-ResourceId={})", origin.primary_id);
    case occasion_origin::signal_type::PRS:
      return fmt::format(
          "DL-PRS resource (PRS Resource Set Id={}, PRS Resource Id={})", origin.primary_id, origin.secondary_id);
    case occasion_origin::signal_type::SRS:
      return fmt::format("SRS resource (SRS-ResourceId={})", origin.primary_id);
    case occasion_origin::signal_type::PRACH:
      return fmt::format("PRACH occasion (Frequency Domain Index={})", origin.primary_id);
    case occasion_origin::signal_type::TDD_NON_DL:
      return origin.secondary_id == 0
                 ? "TDD UL-DL pattern (no symbols are available for DL in that slot)"
                 : fmt::format("TDD UL-DL pattern (only symbols 0-{} are available for DL in that slot)",
                               origin.secondary_id - 1);
    case occasion_origin::signal_type::TDD_NON_UL:
      return origin.secondary_id == 0
                 ? "TDD UL-DL pattern (no symbols are available for UL in that slot)"
                 : fmt::format("TDD UL-DL pattern (only symbols {}-{} are available for UL in that slot)",
                               NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - origin.secondary_id,
                               NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - 1);
  }
  ocudu_assert(false, "Invalid occasion origin signal type");
  return {};
}

/// \brief Builds the periodic occasions of the SSB burst of a cell.
///
/// One occasion is created per transmitted SSB candidate. All occasions share the same slot period
/// (the SSB burst set periodicity) and CRBs, but each has its own slot offset and OFDM symbols within that slot, as
/// per TS 38.213, Section 4.1.
std::vector<periodic_occasion> get_ssb_occasions(const ran_cell_config& ran)
{
  std::vector<periodic_occasion> occasions;

  const ssb_configuration& ssb_cfg    = ran.ssb_cfg;
  const subcarrier_spacing scs_common = ran.dl_cfg_common.init_dl_bwp.generic_params.scs;
  const ssb_pattern_case   ssb_case   = band_helper::get_ssb_pattern(ran.dl_carrier.band, ssb_cfg.scs);
  const crb_interval       ssb_crbs   = get_ssb_crbs(ssb_cfg.scs, scs_common, ssb_cfg.offset_to_point_A, ssb_cfg.k_ssb);
  const unsigned ssb_period_slots     = to_underlying(ssb_cfg.ssb_period) * get_nof_slots_per_subframe(scs_common);

  for (uint8_t ssb_idx : ssb_cfg.ssb_beams.transmitted_indexes()) {
    // Absolute OFDM symbol index of the SSB occasion within the SSB burst set period.
    const unsigned burst_symbol = ssb_get_l_first(ssb_case, ssb_idx);

    periodic_occasion occ;
    occ.origin      = {occasion_origin::signal_type::SSB, ssb_idx, 0};
    occ.slot_period = ssb_period_slots;
    occ.slot_offset = burst_symbol / NOF_OFDM_SYM_PER_SLOT_NORMAL_CP;
    occ.crbs        = to_crb_bitset(ssb_crbs);

    // The 4 OFDM symbols of an SSB occasion never cross a slot boundary. The SSB fully occupies every RE of its
    // CRBs in those symbols.
    const unsigned symbol_start = burst_symbol % NOF_OFDM_SYM_PER_SLOT_NORMAL_CP;
    for (unsigned sym = symbol_start, sym_end = symbol_start + NOF_SSB_OFDM_SYMBOLS; sym != sym_end; ++sym) {
      occ.re_masks[sym].set();
    }

    occasions.push_back(occ);
  }

  return occasions;
}

/// \brief Builds the periodic occasions of the cell's periodic NZP-CSI-RS resources.
///
/// \remark ZP-CSI-RS resources are deliberately excluded: they do not represent a transmission by the cell, but an
/// instruction for the UE not to expect PDSCH there, so they cannot collide with anything.
std::vector<periodic_occasion> get_nzp_csi_rs_occasions(span<const nzp_csi_rs_resource> nzp_csi_rs_res_list,
                                                        const crb_interval&             dl_bwp_crbs)
{
  std::vector<periodic_occasion> occasions;

  for (const nzp_csi_rs_resource& res : nzp_csi_rs_res_list) {
    if (not res.csi_res_offset.has_value() or not res.csi_res_period.has_value()) {
      // Aperiodic NZP-CSI-RS resources do not recur, so they cannot be modelled as periodic occasions.
      continue;
    }

    const csi_rs_resource_mapping& res_mapping = res.res_mapping;
    const unsigned                 row         = csi_rs::get_csi_rs_resource_mapping_row_number(
        res_mapping.nof_ports, res_mapping.freq_density, res_mapping.cdm, res_mapping.fd_alloc);
    const crb_interval occupied_crbs = get_csi_occupied_crbs(res_mapping.freq_band_rbs, dl_bwp_crbs);

    csi_rs_pattern_configuration pattern_cfg{
        .start_rb                 = occupied_crbs.start(),
        .nof_rb                   = occupied_crbs.length(),
        .csi_rs_mapping_table_row = row,
        .symbol_l0                = res_mapping.first_ofdm_symbol_in_td,
        .symbol_l1                = res_mapping.first_ofdm_symbol_in_td2.value_or(0),
        .cdm                      = res_mapping.cdm,
        .freq_density             = res_mapping.freq_density,
    };
    csi_rs::convert_freq_domain(pattern_cfg.freq_allocation_ref_idx, res_mapping.fd_alloc, row);

    const csi_rs_pattern      pattern  = get_csi_rs_pattern(pattern_cfg);
    const csi_rs_pattern_port reserved = pattern.get_reserved_pattern();

    periodic_occasion occ;
    occ.origin      = {occasion_origin::signal_type::NZP_CSI_RS, static_cast<uint8_t>(res.res_id), 0};
    occ.slot_period = to_underlying(*res.csi_res_period);
    occ.slot_offset = *res.csi_res_offset;
    occ.crbs        = bounded_bitset<MAX_NOF_PRBS>(MAX_NOF_PRBS);
    // With a frequency density lower than one, not every CRB in the pattern's range actually carries the CSI-RS.
    for (unsigned rb = pattern.rb_begin; rb < pattern.rb_end; rb += pattern.rb_stride) {
      occ.crbs.set(rb);
    }
    for (unsigned sym = 0; sym != NOF_OFDM_SYM_PER_SLOT_NORMAL_CP; ++sym) {
      if (not reserved.symbol_mask.test(sym)) {
        continue;
      }
      for (unsigned re = 0; re != NOF_SUBCARRIERS_PER_RB; ++re) {
        occ.re_masks[sym].set(re, reserved.re_mask.test(re));
      }
    }

    occasions.push_back(occ);
  }

  return occasions;
}

/// \brief Builds the periodic occasions of the cell's periodic CSI-IM resources, as per TS 38.214, Section 5.2.2.4.
///
/// \remark A CSI-IM resource does not represent a transmission by the cell either, but it is still checked against
/// other transmissions: if something lands on a CSI-IM resource's REs, the cell's own signal leaks into what is meant
/// to be an interference-only measurement.
std::vector<periodic_occasion> get_csi_im_occasions(span<const csi_im_resource> csi_im_res_list,
                                                    const crb_interval&         dl_bwp_crbs)
{
  std::vector<periodic_occasion> occasions;

  for (const csi_im_resource& res : csi_im_res_list) {
    if (not res.csi_res_offset.has_value() or not res.csi_res_period.has_value() or
        not res.csi_im_res_element_pattern.has_value()) {
      // Aperiodic CSI-IM resources do not recur, so they cannot be modelled as periodic occasions.
      continue;
    }

    const auto&    pattern         = *res.csi_im_res_element_pattern;
    const unsigned nof_subcarriers = get_csi_im_pattern_nof_subcarriers(pattern.pattern_type);
    const unsigned nof_symbols     = get_csi_im_pattern_nof_symbols(pattern.pattern_type);

    periodic_occasion occ;
    occ.origin      = {occasion_origin::signal_type::CSI_IM, static_cast<uint8_t>(res.res_id), 0};
    occ.slot_period = to_underlying(*res.csi_res_period);
    occ.slot_offset = *res.csi_res_offset;
    occ.crbs        = to_crb_bitset(get_csi_occupied_crbs(res.freq_band_rbs, dl_bwp_crbs));
    for (unsigned sym = pattern.symbol_location, sym_end = sym + nof_symbols; sym != sym_end; ++sym) {
      for (unsigned sc = pattern.subcarrier_location, sc_end = sc + nof_subcarriers; sc != sc_end; ++sc) {
        occ.re_masks[sym].set(sc);
      }
    }

    occasions.push_back(occ);
  }

  return occasions;
}

/// \brief Builds the periodic occasions of the cell's DL-PRS resources, as per TS 38.211, Section 7.4.1.7.
std::vector<periodic_occasion> get_prs_occasions(const prs_config& prs_cfg)
{
  std::vector<periodic_occasion> occasions;

  for (unsigned set_id = 0, nof_sets = prs_cfg.resource_sets.size(); set_id != nof_sets; ++set_id) {
    const prs_resource_set& res_set           = prs_cfg.resource_sets[set_id];
    const crb_interval      crbs              = {res_set.start_prb, res_set.start_prb + res_set.bandwidth_prbs};
    const unsigned          comb_size         = static_cast<unsigned>(res_set.comb_size);
    const unsigned          nof_symbols       = static_cast<unsigned>(res_set.nof_symbols);
    const unsigned          repetition_factor = static_cast<unsigned>(res_set.repetition_factor);
    const unsigned          time_gap          = static_cast<unsigned>(res_set.time_gap);

    for (unsigned res_id = 0, nof_res = res_set.resources.size(); res_id != nof_res; ++res_id) {
      const prs_resource& res = res_set.resources[res_id];

      // The RE mask is the same for every repetition of the resource, as the frequency offset only depends on the
      // symbol index within the resource, not on the slot or repetition index.
      std::array<std::bitset<NOF_SUBCARRIERS_PER_RB>, NOF_OFDM_SYM_PER_SLOT_NORMAL_CP> re_masks{};
      for (unsigned l = 0; l != nof_symbols; ++l) {
        const unsigned k = (res.re_offset + get_prs_freq_offset(res_set.comb_size, l)) % comb_size;
        for (unsigned re = k; re < NOF_SUBCARRIERS_PER_RB; re += comb_size) {
          re_masks[res.symbol_offset + l].set(re);
        }
      }

      for (unsigned rep = 0; rep != repetition_factor; ++rep) {
        periodic_occasion occ;
        occ.origin = {occasion_origin::signal_type::PRS, static_cast<uint8_t>(set_id), static_cast<uint8_t>(res_id)};
        occ.slot_period = res_set.periodicity_slots;
        occ.slot_offset = res_set.slot_offset + res.slot_offset + rep * time_gap;
        occ.crbs        = to_crb_bitset(crbs);
        occ.re_masks    = re_masks;
        occasions.push_back(occ);
      }
    }
  }

  return occasions;
}

/// \brief Builds the periodic occasions of the cell's periodic SRS resources, as per TS 38.211, Section 6.4.1.4.
///
/// \remark Aperiodic and semi-persistent SRS resources are deliberately excluded: unlike periodic SRS, they are
/// triggered or activated dynamically by the network.
std::vector<periodic_occasion> get_srs_occasions(const srs_config& srs_cfg, const crb_interval& ul_bwp_crbs)
{
  std::vector<periodic_occasion> occasions;

  for (const srs_config::srs_resource& res : srs_cfg.srs_res_list) {
    if (res.res_type != srs_resource_type::periodic or not res.periodicity_and_offset.has_value()) {
      // Aperiodic and semi-persistent SRS resources do not recur, so they cannot be modelled as periodic occasions.
      continue;
    }

    const srs_resource_configuration res_cfg   = to_srs_resource_configuration(res, cyclic_prefix::NORMAL);
    const unsigned                   nof_ports = static_cast<unsigned>(res.nof_ports);
    const ofdm_symbol_range          symbol_range =
        ofdm_symbol_range::start_and_len(res_cfg.start_symbol.value(), res.res_mapping.nof_symb);

    periodic_occasion occ;
    occ.origin      = {occasion_origin::signal_type::SRS, static_cast<uint8_t>(res.id.cell_res_id), 0};
    occ.slot_period = to_underlying(res.periodicity_and_offset->period);
    occ.slot_offset = res.periodicity_and_offset->offset;

    for (unsigned port = 0; port != nof_ports; ++port) {
      const srs_information info = get_srs_information(res_cfg, port);

      if (port == 0) {
        // The occupied bandwidth and CRBs do not depend on the antenna port, as per TS 38.211, Section 6.4.1.4.3.
        const unsigned bandwidth_rbs = info.sequence_length * info.comb_size / NOF_SUBCARRIERS_PER_RB;
        const unsigned start_crb     = ul_bwp_crbs.start() + info.mapping_initial_subcarrier / NOF_SUBCARRIERS_PER_RB;
        occ.crbs                     = to_crb_bitset({start_crb, start_crb + bandwidth_rbs});
      }

      const unsigned k_tc = info.mapping_initial_subcarrier % NOF_SUBCARRIERS_PER_RB;
      for (unsigned sym = symbol_range.start(), sym_end = symbol_range.stop(); sym != sym_end; ++sym) {
        for (unsigned re = k_tc; re < NOF_SUBCARRIERS_PER_RB; re += info.comb_size) {
          occ.re_masks[sym].set(re);
        }
      }
    }

    occasions.push_back(occ);
  }

  return occasions;
}

/// \brief Builds the periodic occasions of the cell's PRACH occasions, as per TS 38.211, Section 6.3.3.2.
///
/// \remark The occasions are derived purely from the static PRACH configuration parameters, mirroring \c
/// prach_scheduler. The occasion validity rules of TS 38.213, Section 8.1 (e.g. TDD or SS/PBCH block collisions) are
/// not applied here, as invalid occasions are simply skipped at run time rather than transmitted, so they cannot
/// collide with anything; only PRACH's static time/frequency position is checked against other UL signals.
std::vector<periodic_occasion> get_prach_occasions(const ran_cell_config& ran)
{
  std::vector<periodic_occasion> occasions;

  if (not ran.ul_cfg_common.init_ul_bwp.rach_cfg_common.has_value()) {
    return occasions;
  }

  const rach_config_common& rach_cfg = *ran.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  const subcarrier_spacing  ul_scs   = ran.ul_cfg_common.init_ul_bwp.generic_params.scs;

  const prach_configuration prach_cfg = prach_configuration_get(band_helper::get_freq_range(ran.dl_carrier.band),
                                                                band_helper::get_duplex_mode(ran.dl_carrier.band),
                                                                rach_cfg.rach_cfg_generic.prach_config_index);

  const prach_helper::preamble_slot_mapping td_mapping{
      ran.dl_carrier.band, ul_scs, rach_cfg.rach_cfg_generic.prach_config_index};

  // The information we need are not related to whether it is the last PRACH occasion.
  constexpr bool                   is_last_prach_occasion = false;
  const prach_preamble_information info =
      is_long_preamble(prach_cfg.format)
          ? get_prach_preamble_long_info(prach_cfg.format)
          : get_prach_preamble_short_info(prach_cfg.format, to_ra_subcarrier_spacing(ul_scs), is_last_prach_occasion);

  const prach_symbols_slots_duration duration_info  = get_prach_duration_info(prach_cfg, ul_scs);
  const unsigned                     prach_nof_prbs = prach_frequency_mapping_get(info.scs, ul_scs).nof_rb_ra;

  const unsigned nof_slots_per_frame = get_nof_slots_per_subframe(ul_scs) * NOF_SUBFRAMES_PER_FRAME;
  // Only long preambles flag just the starting slot of the burst; short ones already flag every slot with occasions.
  const unsigned nof_burst_slots = td_mapping.has_long_preamble() ? td_mapping.prach_burst_length_slots() : 1;
  const unsigned slot_period     = prach_cfg.x * nof_slots_per_frame;

  for (unsigned id_fd_ra = 0; id_fd_ra != rach_cfg.rach_cfg_generic.msg1_fdm; ++id_fd_ra) {
    const unsigned     prb_start = rach_cfg.rach_cfg_generic.msg1_frequency_start + id_fd_ra * prach_nof_prbs;
    const crb_interval crbs      = {ran.ul_cfg_common.init_ul_bwp.generic_params.crbs.start() + prb_start,
                                    ran.ul_cfg_common.init_ul_bwp.generic_params.crbs.start() + prb_start + prach_nof_prbs};

    for (unsigned y : prach_cfg.y) {
      for (unsigned slot_idx = 0; slot_idx != nof_slots_per_frame; ++slot_idx) {
        if (not td_mapping.has_slot_index_prach_occasion(slot_idx)) {
          continue;
        }

        for (unsigned burst_slot = 0; burst_slot != nof_burst_slots; ++burst_slot) {
          const ofdm_symbol_range symbols =
              get_prach_burst_slot_symbols(duration_info, td_mapping.has_long_preamble(), burst_slot);

          periodic_occasion occ;
          occ.origin      = {occasion_origin::signal_type::PRACH, static_cast<uint8_t>(id_fd_ra), 0};
          occ.slot_period = slot_period;
          occ.slot_offset = (y * nof_slots_per_frame + slot_idx + burst_slot) % slot_period;
          occ.crbs        = to_crb_bitset(crbs);
          for (unsigned sym = symbols.start(); sym != symbols.stop(); ++sym) {
            occ.re_masks[sym].set();
          }
          occasions.push_back(occ);
        }
      }
    }
  }

  return occasions;
}

/// \brief Builds the periodic occasions that mark, for a given link direction, the OFDM symbols of each slot in the
/// cell's TDD DL-UL pattern that are *not* available for that direction, as per TS 38.213, Section 11.1.
///
/// DL (UL) occasions are checked against these to ensure they never occupy a symbol that the TDD pattern reserves
/// for the opposite direction or for the guard period.
std::vector<periodic_occasion> get_tdd_restricted_occasions(const tdd_ul_dl_config_common& tdd_cfg, bool is_dl)
{
  std::vector<periodic_occasion> occasions;

  const unsigned period_slots = nof_slots_per_tdd_period(tdd_cfg);
  for (unsigned slot_idx = 0; slot_idx != period_slots; ++slot_idx) {
    const ofdm_symbol_range active_symbols = is_dl
                                                 ? get_active_tdd_dl_symbols(tdd_cfg, slot_idx, cyclic_prefix::NORMAL)
                                                 : get_active_tdd_ul_symbols(tdd_cfg, slot_idx, cyclic_prefix::NORMAL);
    if (active_symbols.length() == NOF_OFDM_SYM_PER_SLOT_NORMAL_CP) {
      // The whole slot is available for this direction; nothing to restrict.
      continue;
    }

    // DL (UL) active symbols always start at the beginning (end) of the slot, so the restricted region is the
    // complementary, contiguous range of symbols.
    const ofdm_symbol_range restricted_symbols =
        is_dl ? ofdm_symbol_range{active_symbols.stop(), NOF_OFDM_SYM_PER_SLOT_NORMAL_CP}
              : ofdm_symbol_range{0, active_symbols.start()};

    periodic_occasion occ;
    occ.origin = {is_dl ? occasion_origin::signal_type::TDD_NON_DL : occasion_origin::signal_type::TDD_NON_UL,
                  static_cast<uint16_t>(slot_idx),
                  static_cast<uint8_t>(active_symbols.length())};
    occ.slot_period = period_slots;
    occ.slot_offset = slot_idx;
    // The restriction applies to the whole cell bandwidth.
    occ.crbs = to_crb_bitset({0, MAX_NOF_PRBS});
    for (unsigned sym = restricted_symbols.start(); sym != restricted_symbols.stop(); ++sym) {
      occ.re_masks[sym].set();
    }

    occasions.push_back(occ);
  }

  return occasions;
}

/// Appends \c src to \c dst.
void append(std::vector<periodic_occasion>& dst, std::vector<periodic_occasion> src)
{
  dst.insert(dst.end(), std::make_move_iterator(src.begin()), std::make_move_iterator(src.end()));
}

/// Checks every pair of occasions in \c occasions for a collision, and returns an error describing the first one found.
error_type<std::string> check_occasion_collisions(const std::vector<periodic_occasion>& occasions)
{
  for (auto it = occasions.begin(); it != occasions.end(); ++it) {
    auto it2 = it;
    for (++it2; it2 != occasions.end(); ++it2) {
      if (const std::optional<occasion_collision> coll = find_collision(*it, *it2)) {
        return make_unexpected(fmt::format("Detected a collision between the {} and the {} in slot {} (period {} "
                                           "slots)",
                                           to_string(coll->origin_a),
                                           to_string(coll->origin_b),
                                           coll->slot,
                                           coll->period));
      }
    }
  }
  return default_success_t();
}

} // namespace

error_type<std::string> ocudu::check_periodic_resource_collisions(const ran_cell_config& ran)
{
  const serving_cell_config serv_cell_cfg = config_helpers::make_default_ue_cell_config(ran).serv_cell_cfg;

  const crb_interval& dl_bwp_crbs = ran.dl_cfg_common.init_dl_bwp.generic_params.crbs;

  std::vector<periodic_occasion> dl_occasions = get_ssb_occasions(ran);
  if (ran.init_bwp.csi.has_value()) {
    const auto csi_params = config_helpers::make_csi_meas_config_builder_params(ran);
    append(dl_occasions, get_nzp_csi_rs_occasions(csi_helper::make_nzp_csi_rs_resource_list(csi_params), dl_bwp_crbs));
    append(dl_occasions, get_csi_im_occasions(csi_helper::make_csi_im_resource_list(csi_params), dl_bwp_crbs));
  }
  append(dl_occasions, get_prs_occasions(ran.prs_cfg));

  std::vector<periodic_occasion> ul_occasions = get_prach_occasions(ran);
  if (serv_cell_cfg.ul_config.has_value() and serv_cell_cfg.ul_config->init_ul_bwp.srs_cfg.has_value()) {
    append(ul_occasions,
           get_srs_occasions(*serv_cell_cfg.ul_config->init_ul_bwp.srs_cfg,
                             ran.ul_cfg_common.init_ul_bwp.generic_params.crbs));
  }

  if (ran.tdd_cfg.has_value()) {
    // In TDD, DL and UL occasions must also respect the cell's TDD DL-UL pattern.
    append(dl_occasions, get_tdd_restricted_occasions(*ran.tdd_cfg, true));
    append(ul_occasions, get_tdd_restricted_occasions(*ran.tdd_cfg, false));
  }

  // DL and UL signals are checked independently.
  HANDLE_ERROR(check_occasion_collisions(dl_occasions));
  HANDLE_ERROR(check_occasion_collisions(ul_occasions));
  return default_success_t();
}
