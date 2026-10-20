// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/csi_rs/csi_meas_config.h"
#include "ocudu/ran/precoding/precoding_codebook_type2_helpers.h"
#include "ocudu/ran/slot_point.h"
#include "ocudu/scheduler/config/pucch_resource_builder_params.h"
#include "ocudu/support/enum_utils.h"
#include "ocudu/support/math/math_utils.h"

using namespace ocudu;
using namespace csi_helper;

/// Number of NZP-CSI-Resources for TRS.
static const unsigned nof_trs_nzp_csi_resources = 4;

/// Get CRBs across which a CSI resource spans as per TS 38.331, "CSI-FrequencyOccupation".
///
/// \param nof_crbs Number of CRBs used for the CSI-RS.
/// \param bwp_nof_rbs Width of the BWP where the CSI-RS is used, in RBs.
/// \return A valid number of RBs for the CSI-RS.
static crb_interval get_csi_freq_occupation_rbs(unsigned nof_crbs, unsigned bwp_nof_rbs)
{
  // - Only multiples of 4 are allowed.
  // - The smallest configurable number is the MIN(24, BWP_width).
  // Note: If the configured value is larger than the width of the corresponding BWP, the UE shall assume that the
  // actual CSI-RS bandwidth is equal to the width of the BWP.
  const unsigned nof_rbs_in_multiples_of_4 = 4 * divide_ceil(nof_crbs, 4);

  return {0, std::max(nof_rbs_in_multiples_of_4, std::min(24U, bwp_nof_rbs))};
}

/// \brief Compute default CSI-RS signalling period to use, while constrained by TS 38.214, 5.1.6.1.1.
csi_resource_periodicity ocudu::csi_helper::get_max_csi_rs_period(subcarrier_spacing pdsch_scs)
{
  const csi_resource_periodicity max_csi_period =
      static_cast<csi_resource_periodicity>(std::min(80U * get_nof_slots_per_subframe(pdsch_scs), 640U));
  return max_csi_period;
}

bool ocudu::csi_helper::is_csi_rs_period_valid(csi_resource_periodicity       csi_rs_period,
                                               const tdd_ul_dl_config_common& tdd_cfg)
{
  // The CSI-RS period must be multiple of the TDD period.
  const unsigned tdd_period = nof_slots_per_tdd_period(tdd_cfg);
  if (static_cast<unsigned>(csi_rs_period) % tdd_period != 0) {
    return false;
  }

  // According to TS 38.214, Section 5.1.6.1.1, a UE expects periods of 10, 20, 40, or 80 milliseconds in CSI-RS for
  // tracking.
  unsigned                      nof_slots_per_subframe = get_nof_slots_per_subframe(tdd_cfg.ref_scs);
  const std::array<unsigned, 4> csi_opt_msec           = {10 * nof_slots_per_subframe,
                                                          20 * nof_slots_per_subframe,
                                                          40 * nof_slots_per_subframe,
                                                          80 * nof_slots_per_subframe};

  return std::find(csi_opt_msec.begin(), csi_opt_msec.end(), to_underlying(csi_rs_period)) != csi_opt_msec.end();
}

[[nodiscard]] bool ocudu::csi_helper::are_sr_and_csi_pucchs_scheduled_together(unsigned sr_period,
                                                                               unsigned sr_offset,
                                                                               unsigned csi_period,
                                                                               unsigned csi_offset)
{
  return crt_solvable(sr_offset, sr_period, csi_offset, csi_period);
}

std::optional<csi_resource_periodicity>
ocudu::csi_helper::find_valid_csi_rs_period(const tdd_ul_dl_config_common& tdd_cfg)
{
  const unsigned tdd_period = nof_slots_per_tdd_period(tdd_cfg);

  csi_resource_periodicity csi_rs_period = get_max_csi_rs_period(tdd_cfg.ref_scs);
  if (static_cast<unsigned>(csi_rs_period) % tdd_period == 0) {
    return csi_rs_period;
  }

  span<const csi_resource_periodicity> csi_options = csi_resource_periodicity_options();
  auto                                 rit         = std::find(csi_options.rbegin(), csi_options.rend(), csi_rs_period);
  auto found_rit = std::find_if(++rit, csi_options.rend(), [tdd_period](csi_resource_periodicity period) {
    return static_cast<unsigned>(period) % tdd_period == 0;
  });
  if (found_rit == csi_options.rend()) {
    return std::nullopt;
  }
  return *rit;
}

// Verifies whether a CSI-RS slot offset falls in a DL slot and does not collide with SIB1 or SSB.
static bool is_csi_slot_offset_valid(unsigned                       slot_offset,
                                     const tdd_ul_dl_config_common& tdd_cfg,
                                     unsigned                       max_csi_symbol_index,
                                     ssb_periodicity                ssb_period,
                                     span<const unsigned>           ssb_slot_offsets,
                                     unsigned                       sib1_period_slots,
                                     span<const unsigned>           sib1_slot_offsets)
{
  const unsigned ssb_period_slots = to_underlying(ssb_period) * get_nof_slots_per_subframe(tdd_cfg.ref_scs);

  const unsigned slot_in_ssb_period  = slot_offset % ssb_period_slots;
  const unsigned slot_in_sib1_period = slot_offset % sib1_period_slots;
  if (std::any_of(ssb_slot_offsets.begin(),
                  ssb_slot_offsets.end(),
                  [slot_in_ssb_period](unsigned s) { return slot_in_ssb_period == s; }) or
      std::any_of(sib1_slot_offsets.begin(), sib1_slot_offsets.end(), [slot_in_sib1_period](unsigned s) {
        return slot_in_sib1_period == s;
      })) {
    return false;
  }

  const unsigned slot_index = slot_offset % (get_nof_slots_per_subframe(tdd_cfg.ref_scs) * NOF_SUBFRAMES_PER_FRAME);
  return get_active_tdd_dl_symbols(tdd_cfg, slot_index, cyclic_prefix::NORMAL).length() > max_csi_symbol_index;
}

bool csi_helper::derive_valid_csi_rs_slot_offsets(du_csi_params&                 csi_params,
                                                  const std::optional<unsigned>& meas_csi_slot_offset,
                                                  const std::optional<unsigned>& tracking_csi_slot_offset,
                                                  span<const unsigned>           zp_csi_slot_offsets,
                                                  const tdd_ul_dl_config_common& tdd_cfg,
                                                  unsigned                       max_csi_symbol_index,
                                                  ssb_periodicity                ssb_period,
                                                  span<const unsigned>           ssb_slot_offsets,
                                                  unsigned                       sib1_period_slots,
                                                  span<const unsigned>           sib1_slot_offsets)
{
  ocudu_assert(is_csi_rs_period_valid(csi_params.csi_rs_period, tdd_cfg),
               "Invalid CSI-RS period {} for provided TDD pattern",
               fmt::underlying(csi_params.csi_rs_period));

  ocudu_assert(csi_params.nof_beams >= 1, "nof_beams must be at least 1");

  // Validate and pre-load ZP overrides.
  csi_params.zp_csi_slot_offsets.clear();
  if (not zp_csi_slot_offsets.empty()) {
    ocudu_assert(zp_csi_slot_offsets.size() == csi_params.nof_beams,
                 "zp_csi_slot_offsets size {} != nof_beams {}",
                 zp_csi_slot_offsets.size(),
                 csi_params.nof_beams);
    for (unsigned off : zp_csi_slot_offsets) {
      if (not is_csi_slot_offset_valid(
              off, tdd_cfg, max_csi_symbol_index, ssb_period, ssb_slot_offsets, sib1_period_slots, sib1_slot_offsets)) {
        return false;
      }
    }
    csi_params.zp_csi_slot_offsets.assign(zp_csi_slot_offsets.begin(), zp_csi_slot_offsets.end());
  }

  // Returns true if candidate collides with any already-assigned meas, pre-specified ZP, or tracking slot.
  auto is_occupied = [&](unsigned candidate) {
    for (unsigned s : csi_params.meas_csi_slot_offsets) {
      if (candidate == s) {
        return true;
      }
    }
    for (unsigned s : csi_params.zp_csi_slot_offsets) {
      if (candidate == s) {
        return true;
      }
    }
    return candidate == csi_params.tracking_csi_slot_offset or candidate == csi_params.tracking_csi_slot_offset + 1;
  };

  // Fill the pre-specified parameters and verify if valid.
  csi_params.meas_csi_slot_offsets.clear();
  if (meas_csi_slot_offset.has_value()) {
    if (not is_csi_slot_offset_valid(*meas_csi_slot_offset,
                                     tdd_cfg,
                                     max_csi_symbol_index,
                                     ssb_period,
                                     ssb_slot_offsets,
                                     sib1_period_slots,
                                     sib1_slot_offsets)) {
      return false;
    }
    csi_params.meas_csi_slot_offsets.push_back(*meas_csi_slot_offset);
  }
  if (tracking_csi_slot_offset.has_value()) {
    // Tracking CSI-RS uses two consecutive slots.
    if (not is_csi_slot_offset_valid(*tracking_csi_slot_offset,
                                     tdd_cfg,
                                     max_csi_symbol_index,
                                     ssb_period,
                                     ssb_slot_offsets,
                                     sib1_period_slots,
                                     sib1_slot_offsets) or
        not is_csi_slot_offset_valid(*tracking_csi_slot_offset + 1,
                                     tdd_cfg,
                                     max_csi_symbol_index,
                                     ssb_period,
                                     ssb_slot_offsets,
                                     sib1_period_slots,
                                     sib1_slot_offsets)) {
      return false;
    }
    csi_params.tracking_csi_slot_offset = *tracking_csi_slot_offset;
  }

  bool tracking_found = tracking_csi_slot_offset.has_value();
  for (unsigned i = 0; i < static_cast<unsigned>(csi_params.csi_rs_period) and
                       (not tracking_found or csi_params.meas_csi_slot_offsets.size() < csi_params.nof_beams);
       ++i) {
    if (not is_csi_slot_offset_valid(
            i, tdd_cfg, max_csi_symbol_index, ssb_period, ssb_slot_offsets, sib1_period_slots, sib1_slot_offsets)) {
      continue;
    }
    if (is_occupied(i)) {
      continue;
    }
    // Note: Tracking CSI-RS occupies two consecutive slots.
    if (not tracking_found and
        is_csi_slot_offset_valid(i + 1,
                                 tdd_cfg,
                                 max_csi_symbol_index,
                                 ssb_period,
                                 ssb_slot_offsets,
                                 sib1_period_slots,
                                 sib1_slot_offsets) and
        not is_occupied(i + 1)) {
      tracking_found                      = true;
      csi_params.tracking_csi_slot_offset = i;
      ++i;
      continue;
    }

    if (csi_params.meas_csi_slot_offsets.size() < csi_params.nof_beams) {
      csi_params.meas_csi_slot_offsets.push_back(i);
    }
  }

  if (not(tracking_found and csi_params.meas_csi_slot_offsets.size() == csi_params.nof_beams)) {
    return false;
  }

  // Auto-fill ZP slot offsets from meas when not explicitly specified.
  if (csi_params.zp_csi_slot_offsets.empty()) {
    csi_params.zp_csi_slot_offsets = csi_params.meas_csi_slot_offsets;
  }

  return true;
}

static zp_csi_rs_resource make_default_zp_csi_rs_resource(const csi_meas_config_builder_params& params)
{
  report_error_if_not(
      params.nof_ports <= MAX_NOF_ANTENNA_PORTS, "Unsupported number of antenna ports={}", params.nof_ports);

  if (params.nof_ports == 8) {
    // [Implementation-defined] For 8 ports TS 38.211 Table 7.4.1.5.3-1 allows Rows 6-8; Row 6 is chosen here (fd-CDM2,
    // frequency density 1, 4 CDM groups), as is the placement (frequency positions, first OFDM symbol, period and
    // offset).
    zp_csi_rs_resource res{};
    res.id                    = static_cast<zp_csi_rs_res_id_t>(0);
    res.res_mapping.nof_ports = 8;
    res.res_mapping.fd_alloc.resize(6);
    res.res_mapping.fd_alloc.fill(0, 4);
    res.res_mapping.cdm                     = csi_rs_cdm_type::fd_CDM2;
    res.res_mapping.first_ofdm_symbol_in_td = params.csi_params.zp_csi_ofdm_symbol_index;
    res.res_mapping.freq_density            = csi_rs_freq_density_type::one;
    res.res_mapping.freq_band_rbs           = get_csi_freq_occupation_rbs(params.nof_rbs, params.nof_rbs);
    res.period                              = params.csi_params.csi_rs_period;
    res.offset                              = 2;

    return res;
  }

  zp_csi_rs_resource res{};
  res.id = static_cast<zp_csi_rs_res_id_t>(0);
  // [Implementation-defined] The reason for using row 4 of Table 7.4.1.5.3-1 in TS 38.211 even in case of nof. ports <
  // 4 is due to some RUs not supporting more than 1 ZP CSI-RS resource per symbol. Also, the specification does not
  // restrict from using row 4 even in case of nof. ports < 4.
  // Freq Alloc -> Row4.
  res.res_mapping.nof_ports = 4;
  res.res_mapping.fd_alloc.resize(3);
  res.res_mapping.fd_alloc.set(params.pci % res.res_mapping.fd_alloc.size(), true);
  res.res_mapping.cdm                     = csi_rs_cdm_type::fd_CDM2;
  res.res_mapping.first_ofdm_symbol_in_td = params.csi_params.zp_csi_ofdm_symbol_index;
  res.res_mapping.freq_density            = csi_rs_freq_density_type::one;
  res.res_mapping.freq_band_rbs           = get_csi_freq_occupation_rbs(params.nof_rbs, params.nof_rbs);
  res.period                              = params.csi_params.csi_rs_period;
  res.offset                              = 2;

  return res;
}

std::vector<zp_csi_rs_resource>
ocudu::csi_helper::make_periodic_zp_csi_rs_resource_list(const csi_meas_config_builder_params& params)
{
  if (params.nof_ports > 8) {
    report_error("Unsupported number of antenna ports {}", params.nof_ports);
  }

  const unsigned nof_beams  = params.csi_params.nof_beams;
  const auto&    zp_offsets = params.csi_params.zp_csi_slot_offsets.empty() ? params.csi_params.meas_csi_slot_offsets
                                                                            : params.csi_params.zp_csi_slot_offsets;
  ocudu_assert(
      zp_offsets.size() == nof_beams, "zp_csi_slot_offsets size {} != nof_beams {}", zp_offsets.size(), nof_beams);

  const zp_csi_rs_resource        tmpl = make_default_zp_csi_rs_resource(params);
  std::vector<zp_csi_rs_resource> list(nof_beams, tmpl);
  for (unsigned i = 0; i < nof_beams; ++i) {
    list[i].id     = static_cast<zp_csi_rs_res_id_t>(i);
    list[i].offset = zp_offsets[i];
    list[i].period = params.csi_params.csi_rs_period;
  }

  return list;
}

zp_csi_rs_resource_set
ocudu::csi_helper::make_periodic_zp_csi_rs_resource_set(const csi_meas_config_builder_params& params)
{
  if (params.nof_ports > 8) {
    report_error("Unsupported number of antenna ports {}", params.nof_ports);
  }

  zp_csi_rs_resource_set zp_set{};
  zp_set.id = static_cast<zp_csi_rs_res_set_id_t>(0);
  for (unsigned i = 0; i < params.csi_params.nof_beams; ++i) {
    zp_set.zp_csi_rs_res_list.push_back(static_cast<zp_csi_rs_res_set_id_t>(i));
  }

  return zp_set;
}

csi_report_periodicity
ocudu::csi_helper::convert_csi_resource_period_to_report_period(csi_resource_periodicity res_period)
{
  switch (res_period) {
    case csi_resource_periodicity::slots4:
      return csi_report_periodicity::slots4;
    case csi_resource_periodicity::slots5:
      return csi_report_periodicity::slots5;
    case csi_resource_periodicity::slots8:
      return csi_report_periodicity::slots8;
    case csi_resource_periodicity::slots10:
      return csi_report_periodicity::slots10;
    case csi_resource_periodicity::slots16:
      return csi_report_periodicity::slots16;
    case csi_resource_periodicity::slots20:
      return csi_report_periodicity::slots20;
    case csi_resource_periodicity::slots32:
      // fallthrough
    case csi_resource_periodicity::slots40:
      return csi_report_periodicity::slots40;
    case csi_resource_periodicity::slots64:
      // fallthrough
    case csi_resource_periodicity::slots80:
      return csi_report_periodicity::slots80;
    case csi_resource_periodicity::slots160:
      return csi_report_periodicity::slots160;
    case csi_resource_periodicity::slots320:
      // fallthrough
    case csi_resource_periodicity::slots640:
      return csi_report_periodicity::slots320;
  }
  return ocudu::csi_report_periodicity::slots320;
}

// Fills the values that are common to all CSI-RS resources.
static nzp_csi_rs_resource make_common_nzp_csi_rs_resource(const csi_meas_config_builder_params& params)
{
  nzp_csi_rs_resource res{};

  // Fill csi_rs_resource_mapping.
  res.res_mapping.freq_density  = csi_rs_freq_density_type::three;
  res.res_mapping.freq_band_rbs = get_csi_freq_occupation_rbs(params.nof_rbs, params.nof_rbs);

  res.pwr_ctrl_offset       = params.csi_params.pwr_ctrl_offset;
  res.pwr_ctrl_offset_ss_db = 0;
  res.scrambling_id         = params.pci;

  res.csi_res_period = params.csi_params.csi_rs_period;

  res.qcl_info_periodic_csi_rs = static_cast<tci_state_id_t>(0);

  return res;
}

static nzp_csi_rs_resource make_channel_measurement_nzp_csi_rs_resource(const csi_meas_config_builder_params& params,
                                                                        unsigned slot_offset)
{
  ocudu_assert(slot_offset < to_underlying(params.csi_params.csi_rs_period),
               "Invalid CSI slot offset {} >= {}",
               slot_offset,
               to_underlying(params.csi_params.csi_rs_period));
  nzp_csi_rs_resource res = make_common_nzp_csi_rs_resource(params);

  res.res_id                              = static_cast<nzp_csi_rs_res_id_t>(0);
  res.csi_res_offset                      = slot_offset;
  res.res_mapping.first_ofdm_symbol_in_td = params.csi_params.cm_csi_ofdm_symbol_index;
  res.res_mapping.nof_ports               = params.nof_ports;
  res.res_mapping.freq_density            = csi_rs_freq_density_type::one;

  // Number of unique k-hat values based on TS 38.211 Table 7.4.1.5.3-1 column 5, default is 1 (k_0 only).
  unsigned nof_k_hat = 1;

  // Select the amount of frequency-domain resources and the CDM configuration depending on the number of transmit
  // ports.
  if (params.nof_ports == 1) {
    // Code multiplexing is not necessary when only one port is used. This makes it possible to have twelve possible
    // frequency domain allocations.
    res.res_mapping.fd_alloc.resize(12);
    res.res_mapping.cdm = csi_rs_cdm_type::no_CDM;
  } else if (params.nof_ports == 2) {
    // Code multiplexing of two resource elements is used when two transmitted ports are used. This allows six possible
    // frequency domain locations.
    res.res_mapping.fd_alloc.resize(6);
    res.res_mapping.cdm = csi_rs_cdm_type::fd_CDM2;
  } else if (params.nof_ports == 4) {
    // Code multiplexing of four resource elements is used when four transmitted ports are used. This allows three
    // possible frequency domain locations.
    res.res_mapping.fd_alloc.resize(3);
    res.res_mapping.cdm = csi_rs_cdm_type::fd_CDM2;
  } else if (params.nof_ports == 8) {
    // Code multiplexing of four resource elements is used when eight transmitted ports are used. This allows six
    // possible frequency domain locations.
    res.res_mapping.fd_alloc.resize(6);
    res.res_mapping.cdm = csi_rs_cdm_type::fd_CDM2;
    // Same as for ZP-CSI-RS, with 8 ports we use row 6 of TS 38.211 Table 7.4.1.5.3-1, which defines 4 k_hat values
    // (k_0, k_1, k_2, k_3).
    nof_k_hat = 4;
  } else {
    // Another number of ports is not currently supported.
    report_error("Number of ports {} not supported", params.nof_ports);
  }

  // [Implementation-defined] Select the frequency domain allocation in function of the PCI to avoid that NZP-CSI-RS
  // from neighbor cells overlap.
  for (unsigned i_k = 0; i_k != nof_k_hat; ++i_k) {
    res.res_mapping.fd_alloc.set((params.pci + i_k) % res.res_mapping.fd_alloc.size());
  }

  return res;
}

/// \brief Generate Tracking Reference Signal (TRS) resource set.
///
/// The TRS resource set contains four NZP-CSI-RS resources. The resources are mapped on two consecutive slots.
///
/// The NZP-CSI-RS resources selected for TRS are constrained by TS 38.214 Section 5.1.6.1.1 which specifies the number
/// of ports, multiplexing, OFDM symbols to use within the slot, periodicity, and density.
static void fill_tracking_nzp_csi_rs_resource(span<nzp_csi_rs_resource>             tracking_csi_rs,
                                              const csi_meas_config_builder_params& params,
                                              nzp_csi_rs_res_id_t                   first_csi_res_id)
{
  ocudu_assert(tracking_csi_rs.size() == nof_trs_nzp_csi_resources, "Invalid tracking CSI-RS resource list size");
  ocudu_assert(params.csi_params.tracking_csi_slot_offset + 1 < to_underlying(params.csi_params.csi_rs_period),
               "Invalid CSI slot offset");
  nzp_csi_rs_resource res = make_common_nzp_csi_rs_resource(params);

  res.res_mapping.nof_ports    = 1;
  res.res_mapping.cdm          = csi_rs_cdm_type::no_CDM;
  res.res_mapping.freq_density = csi_rs_freq_density_type::three;
  res.res_mapping.fd_alloc.resize(4);

  // [Implementation-defined] Select the frequency domain allocation in function of the PCI to avoid that NZP-CSI-RS
  // from neighbor cells overlap.
  res.res_mapping.fd_alloc.set(params.pci % res.res_mapping.fd_alloc.size());

  static constexpr unsigned rel_slot_offset[] = {0, 0, 1, 1};
  for (unsigned i = 0; i != nof_trs_nzp_csi_resources; ++i) {
    res.res_id                              = static_cast<nzp_csi_rs_res_id_t>(first_csi_res_id + i);
    res.res_mapping.first_ofdm_symbol_in_td = params.csi_params.tracking_csi_ofdm_symbol_indices[i];
    res.csi_res_offset                      = params.csi_params.tracking_csi_slot_offset + rel_slot_offset[i];
    tracking_csi_rs[i]                      = res;
  }
}

std::vector<nzp_csi_rs_resource>
ocudu::csi_helper::make_nzp_csi_rs_resource_list(const csi_meas_config_builder_params& params)
{
  ocudu_assert(not params.csi_params.meas_csi_slot_offsets.empty(), "meas_csi_slot_offsets must be non-empty");
  ocudu_assert(params.csi_params.meas_csi_slot_offsets.size() == params.csi_params.nof_beams,
               "meas_csi_slot_offsets size {} != nof_beams {}",
               params.csi_params.meas_csi_slot_offsets.size(),
               params.csi_params.nof_beams);

  // Beam resources at IDs 0..N-1; TRS at IDs N..N+3.
  const unsigned                   nof_beams = params.csi_params.nof_beams;
  std::vector<nzp_csi_rs_resource> list(nof_beams + nof_trs_nzp_csi_resources);

  // Measurement resources.
  for (unsigned i = 0; i < nof_beams; ++i) {
    list[i]        = make_channel_measurement_nzp_csi_rs_resource(params, params.csi_params.meas_csi_slot_offsets[i]);
    list[i].res_id = static_cast<nzp_csi_rs_res_id_t>(i);
  }

  // Tracking - Resources N..N+3.
  fill_tracking_nzp_csi_rs_resource(span<nzp_csi_rs_resource>(list).subspan(nof_beams, nof_trs_nzp_csi_resources),
                                    params,
                                    static_cast<nzp_csi_rs_res_id_t>(nof_beams));

  return list;
}

static std::vector<nzp_csi_rs_resource_set> make_nzp_csi_rs_resource_sets(const csi_meas_config_builder_params& params)
{
  std::vector<nzp_csi_rs_resource_set> sets(2);

  // Resource Set 0 - Measurement (single beam) or beam sweep (multi-beam).
  sets[0].res_set_id          = static_cast<nzp_csi_rs_res_set_id_t>(0);
  sets[0].is_trs_info_present = false;
  const unsigned nof_beams    = params.csi_params.nof_beams;
  for (unsigned i = 0; i < nof_beams; ++i) {
    sets[0].nzp_csi_rs_res.push_back(static_cast<nzp_csi_rs_res_id_t>(i));
  }
  // Single beam: repetition on (omnidirectional). Multi-beam: repetition off (distinct spatial directions).
  sets[0].is_repetition_on = (nof_beams == 1);

  // Resource Set 1 - Tracking: IDs N..N+3.
  sets[1].res_set_id = static_cast<nzp_csi_rs_res_set_id_t>(1);
  for (unsigned i = 0; i < nof_trs_nzp_csi_resources; ++i) {
    sets[1].nzp_csi_rs_res.push_back(static_cast<nzp_csi_rs_res_id_t>(nof_beams + i));
  }
  sets[1].is_trs_info_present = true;

  return sets;
}

/// \brief Returns the subcarrier location given the bit location set in \c frequencyDomainAllocation of CSI-RS-Resource
/// and size of the \c frequencyDomainAllocation bitmap.
static unsigned get_subcarrier_location_from_fd_alloc_bit_location(int     fd_alloc_bit_location,
                                                                   uint8_t fd_alloc_bitmap_size)
{
  // See TS 38.211, clause 7.4.1.5.3.
  switch (fd_alloc_bitmap_size) {
    case 4: {
      ocudu_assert(fd_alloc_bit_location < 4,
                   "Invalid bit location={} in frequency domain allocation of CSI-RS for row 1",
                   fd_alloc_bit_location);
      return fd_alloc_bit_location;
    }
    case 12: {
      ocudu_assert(fd_alloc_bit_location < 12,
                   "Invalid bit location={} in frequency domain allocation of CSI-RS for row 2",
                   fd_alloc_bit_location);
      return fd_alloc_bit_location;
    }
    case 3: {
      ocudu_assert(fd_alloc_bit_location < 3,
                   "Invalid bit location={} in frequency domain allocation of CSI-RS for row 4",
                   fd_alloc_bit_location);
      return 4 * fd_alloc_bit_location;
    }
    case 6: {
      ocudu_assert(fd_alloc_bit_location < 6,
                   "Invalid bit location={} in frequency domain allocation of CSI-RS for row other",
                   fd_alloc_bit_location);
      return 2 * fd_alloc_bit_location;
    }
    default:
      report_fatal_error("Invalid CSI-RS row");
  }
}

std::vector<csi_im_resource> ocudu::csi_helper::make_csi_im_resource_list(const csi_meas_config_builder_params& params)
{
  if (params.nof_ports > 8) {
    report_error("Unsupported number of antenna ports={}", params.nof_ports);
  }

  const std::vector<zp_csi_rs_resource> zp_list = make_periodic_zp_csi_rs_resource_list(params);

  std::vector<csi_im_resource> res(zp_list.size());
  for (unsigned i = 0; i < zp_list.size(); ++i) {
    const zp_csi_rs_resource& zp = zp_list[i];
    res[i].res_id                = static_cast<csi_im_res_id_t>(i);
    res[i].csi_im_res_element_pattern.emplace();
    res[i].csi_im_res_element_pattern->pattern_type = csi_im_resource::csi_im_resource_element_pattern_type::pattern1;
    res[i].csi_im_res_element_pattern->subcarrier_location = get_subcarrier_location_from_fd_alloc_bit_location(
        zp.res_mapping.fd_alloc.find_lowest(), zp.res_mapping.fd_alloc.size());
    res[i].csi_im_res_element_pattern->symbol_location = zp.res_mapping.first_ofdm_symbol_in_td;
    res[i].freq_band_rbs                               = zp.res_mapping.freq_band_rbs;
    res[i].csi_res_period                              = *zp.period;
    res[i].csi_res_offset                              = *zp.offset;
  }
  return res;
}

static std::vector<csi_im_resource_set> make_csi_im_resource_sets(const csi_meas_config_builder_params& params)
{
  std::vector<csi_im_resource_set> res_set(1);

  res_set[0].res_set_id = static_cast<csi_im_res_set_id_t>(0);
  for (unsigned i = 0; i < params.csi_params.nof_beams; ++i) {
    res_set[0].csi_ims_resources.push_back(static_cast<csi_im_res_id_t>(i));
  }

  return res_set;
}

static std::vector<csi_resource_config> make_csi_resource_configs()
{
  std::vector<csi_resource_config> res_cfgs(3);

  // Resource 0 - Measurement (single beam) or beam sweep (multi-beam); always NZP set 0.
  res_cfgs[0].res_cfg_id = static_cast<csi_res_config_id_t>(0);
  res_cfgs[0].csi_rs_res_set_list =
      csi_resource_config::nzp_csi_rs_ssb{.nzp_csi_rs_res_set_list = {static_cast<nzp_csi_rs_res_set_id_t>(0)}};
  res_cfgs[0].bwp_id   = to_bwp_id(0);
  res_cfgs[0].res_type = csi_resource_config::resource_type::periodic;

  // Resource 1 - Interference; always CSI-IM set 0.
  res_cfgs[1]                     = res_cfgs[0];
  res_cfgs[1].res_cfg_id          = static_cast<csi_res_config_id_t>(1);
  res_cfgs[1].csi_rs_res_set_list = csi_resource_config::csi_im_resource_set_list{static_cast<csi_im_res_set_id_t>(0)};

  // Resource 2 - Tracking; always NZP set 1.
  res_cfgs[2]            = res_cfgs[0];
  res_cfgs[2].res_cfg_id = static_cast<csi_res_config_id_t>(2);
  res_cfgs[2].csi_rs_res_set_list =
      csi_resource_config::nzp_csi_rs_ssb{.nzp_csi_rs_res_set_list = {static_cast<nzp_csi_rs_res_set_id_t>(1)}};

  return res_cfgs;
}

codebook_config::type2 csi_helper::make_type2_codebook_config(const csi_meas_config_builder_params& params)
{
  using typeii = codebook_config::type2::typeii;

  const du_type2_codebook_params& type2_params = params.csi_params.type2_codebook.value();

  // As per TS38.214 Section 5.2.2.2.3, the Type-II codebook is only defined for four or more CSI-RS ports.
  ocudu_assert(params.nof_ports >= 4, "The Type-II codebook requires at least four ports, given {}", params.nof_ports);

  // The Type-II PMI is reported in CSI Part 2, which is only multiplexed in PUSCH.
  ocudu_assert(params.csi_params.enable_aperiodic_report,
               "The Type-II codebook requires aperiodic CSI reporting, as the PMI is carried in CSI Part 2");

  typeii sub_type{};
  if (params.nof_ports == 4) {
    // As per TS38.214 Section 5.2.2.2.3, four CSI-RS ports only support two beams.
    ocudu_assert(type2_params.nof_beams == 2,
                 "Four antenna ports only support two Type-II beams, given {}",
                 type2_params.nof_beams);

    sub_type.n1_n2_codebook_subset_restriction_type = typeii::n1_n2_codebook_subset_restriction_type_t::two_one;
    sub_type.n1_n2_codebook_subset_restriction_value.resize(16);
  } else if (params.nof_ports == 8) {
    sub_type.n1_n2_codebook_subset_restriction_type = typeii::n1_n2_codebook_subset_restriction_type_t::four_one;
    sub_type.n1_n2_codebook_subset_restriction_value.resize(32);
  } else {
    report_error("Unsupported number of antenna ports {} for the Type-II codebook", params.nof_ports);
  }

  // Enable all beam combinations.
  sub_type.n1_n2_codebook_subset_restriction_value.fill(0, sub_type.n1_n2_codebook_subset_restriction_value.size());

  // Limit the number of DL layers that can be requested by the UE via the Rank Indicator (RI). As per TS38.214
  // Section 5.2.2.2.3, the Type-II codebook supports at most two layers.
  sub_type.typeii_ri_restriction.resize(max_nof_typeII_layers);
  sub_type.typeii_ri_restriction.fill(0, std::min<unsigned>(params.max_nof_layers, max_nof_typeII_layers));

  codebook_config::type2 type2{};
  type2.sub_type            = sub_type;
  type2.nof_beams           = type2_params.nof_beams;
  type2.phase_alphabet_size = type2_params.phase_alphabet_size;
  // Subband amplitude reporting is not supported.
  type2.subband_amplitude = false;

  return type2;
}

codebook_config::type1 csi_helper::make_type1_codebook_config(const csi_meas_config_builder_params& params)
{
  codebook_config::type1               type1{};
  codebook_config::type1::single_panel single_panel{};
  if (params.nof_ports == 2) {
    codebook_config::type1::single_panel::two_antenna_ports_two_tx_codebook_subset_restriction bitmap(6);
    bitmap.fill(0, 6, true);
    single_panel.nof_antenna_ports = bitmap;
  } else if (params.nof_ports == 4) {
    codebook_config::type1::single_panel::more_than_two_antenna_ports port_cfg{};
    port_cfg.n1_n2_restriction_type =
        codebook_config::type1::single_panel::more_than_two_antenna_ports::n1_n2_restriction_type_t::two_one;
    // Enable all beam combinations.
    port_cfg.n1_n2_restriction_value.resize(8);
    port_cfg.n1_n2_restriction_value.fill(0, 8, true);
    // Enable all i2 options.
    port_cfg.typei_single_panel_codebook_subset_restriction_i2.resize(16);
    port_cfg.typei_single_panel_codebook_subset_restriction_i2.fill(0, 16, true);
    single_panel.nof_antenna_ports = port_cfg;
  } else if (params.nof_ports == 8) {
    codebook_config::type1::single_panel::more_than_two_antenna_ports port_cfg{};
    port_cfg.n1_n2_restriction_type =
        codebook_config::type1::single_panel::more_than_two_antenna_ports::n1_n2_restriction_type_t::four_one;
    // Enable all beam combinations.
    port_cfg.n1_n2_restriction_value.resize(16);
    port_cfg.n1_n2_restriction_value.fill(0, 16, true);
    // Enable all i2 options.
    port_cfg.typei_single_panel_codebook_subset_restriction_i2.resize(16);
    port_cfg.typei_single_panel_codebook_subset_restriction_i2.fill(0, 16, true);
    single_panel.nof_antenna_ports = port_cfg;
  } else {
    report_error("Unsupported number of antenna ports {}", params.nof_ports);
  }

  // Maximum number of layers must be in line with available number of ports.
  ocudu_assert(params.max_nof_layers <= params.nof_ports,
               "Maximum number of layers cannot be greater than number of ports");

  // Limit the number of DL layers that can be requested by the UE via the Rank Indicator (RI).
  // As per TS 38.214, section 5.2.2.2.1, this can be done by setting the RI restriction bitmap to 0b11...11,
  // where the number of 1s is set to be equal to the number of desired layers.
  single_panel.typei_single_panel_ri_restriction.resize(8);
  single_panel.typei_single_panel_ri_restriction.from_uint64((1U << params.max_nof_layers) - 1U);
  type1.sub_type      = single_panel;
  type1.codebook_mode = pmi_codebook_typeI_mode::one;
  return type1;
}

static std::vector<csi_report_config>
make_csi_report_configs(const csi_meas_config_builder_params&                     params,
                        const std::vector<pusch_time_domain_resource_allocation>& pusch_td_alloc_list)
{
  ocudu_assert(params.csi_params.csi_report_slot_offset.has_value() or params.csi_params.enable_aperiodic_report,
               "At least one of periodic or aperiodic CSI reporting must be enabled");
  // TODO: support both periodic and aperiodic reporting simultaneously.
  ocudu_assert(not(params.csi_params.csi_report_slot_offset.has_value() and params.csi_params.enable_aperiodic_report),
               "Simultaneous periodic and aperiodic CSI reporting is not supported");
  std::vector<csi_report_config> reps(1);

  reps[0].report_cfg_id               = static_cast<csi_report_config_id_t>(0);
  reps[0].res_for_channel_meas        = static_cast<csi_res_config_id_t>(0);
  reps[0].csi_im_res_for_interference = static_cast<csi_res_config_id_t>(1);

  // Set Report Config.
  if (params.csi_params.csi_report_slot_offset.has_value()) {
    csi_report_config::periodic_or_semi_persistent_report_on_pucch report_cfg_type{};
    report_cfg_type.report_type =
        csi_report_config::periodic_or_semi_persistent_report_on_pucch::report_type_t::periodic;
    report_cfg_type.report_slot_period =
        csi_helper::convert_csi_resource_period_to_report_period(params.csi_params.csi_rs_period);
    report_cfg_type.report_slot_offset = *params.csi_params.csi_report_slot_offset;
    pucch_resource_builder_params pucch_builder_params{};
    const unsigned                cell_res_id =
        ((pucch_builder_params.res_set_size.value() + pucch_builder_params.res_set_size.value()) *
         pucch_builder_params.nof_cell_res_set_configs) +
        pucch_builder_params.nof_cell_sr_resources;

    static constexpr unsigned nof_ue_sr_resources = 1U;
    const unsigned            ue_res_id =
        pucch_builder_params.res_set_size.value() + nof_ue_sr_resources + pucch_builder_params.res_set_size.value();
    report_cfg_type.pucch_csi_res_list = {csi_report_config::pucch_csi_resource{
        .ul_bwp = to_bwp_id(0), .pucch_res_id = pucch_res_id_t::make_ded(cell_res_id, ue_res_id)}};
    reps[0].report_cfg_type            = report_cfg_type;
  }
  if (params.csi_params.enable_aperiodic_report) {
    csi_report_config::aperiodic_report report_cfg_type{};
    // K2 values for CSI PUSCHs. We set them to be the same as the PUSCH time domain allocations K2 values.
    report_cfg_type.report_slot_offset_list.resize(pusch_td_alloc_list.size());
    for (unsigned i = 0; i < pusch_td_alloc_list.size(); ++i) {
      report_cfg_type.report_slot_offset_list[i] = pusch_td_alloc_list[i].k2;
    }
    reps[0].report_cfg_type = report_cfg_type;
  }

  reps[0].report_qty_type = csi_report_config::report_quantity_type_t::cri_ri_pmi_cqi;

  reps[0].report_freq_cfg.emplace();
  reps[0].report_freq_cfg->cqi_format_ind = csi_report_config::cqi_format_indicator::wideband_cqi;
  reps[0].report_freq_cfg->pmi_format_ind = csi_report_config::pmi_format_indicator::wideband_pmi;

  if (params.nof_ports > 1) {
    reps[0].codebook_cfg.emplace();
    if (params.csi_params.type2_codebook.has_value()) {
      reps[0].codebook_cfg->codebook_type = make_type2_codebook_config(params);
    } else {
      reps[0].codebook_cfg->codebook_type = make_type1_codebook_config(params);
    }
  }

  reps[0].is_group_based_beam_reporting_enabled = false;
  reps[0].cqi_table                             = cqi_table_t::table1;
  reps[0].subband_size                          = csi_report_config::subband_size_t::value1;

  // Set CQI table of CSI-ReportConfig according to the MCS table used for PDSCH.
  cqi_table_t cqi_table = cqi_table_t::table1;
  switch (params.mcs_table) {
    case pdsch_mcs_table::qam64:
      cqi_table = cqi_table_t::table1;
      break;
    case pdsch_mcs_table::qam256:
      cqi_table = cqi_table_t::table2;
      break;
    case pdsch_mcs_table::qam64LowSe:
      cqi_table = cqi_table_t::table3;
      break;
    default:
      report_error("Invalid MCS table={}\n", static_cast<unsigned>(params.mcs_table));
  }
  for (auto& csi_report_cfg : reps) {
    csi_report_cfg.cqi_table = cqi_table;
  }

  return reps;
}

unsigned ocudu::csi_helper::get_csi_report_trigger_size(const du_csi_params& csi_params)
{
  // [Implementation-defined] There is only one aperiodic trigger state.
  return csi_params.enable_aperiodic_report ? 1 : 0;
}

csi_meas_config
ocudu::csi_helper::make_csi_meas_config(const csi_meas_config_builder_params&                     params,
                                        const std::vector<pusch_time_domain_resource_allocation>& pusch_td_alloc_list)
{
  csi_meas_config csi_meas{};

  // NZP-CSI-RS-Resources.
  csi_meas.nzp_csi_rs_res_list = make_nzp_csi_rs_resource_list(params);

  // NZP-CSI-RS-ResourceSets.
  csi_meas.nzp_csi_rs_res_set_list = make_nzp_csi_rs_resource_sets(params);

  // csi-IM-Resources.
  csi_meas.csi_im_res_list = make_csi_im_resource_list(params);

  // csi-IM-ResourceSets.
  csi_meas.csi_im_res_set_list = make_csi_im_resource_sets(params);

  // CSI-ResourceConfig.
  csi_meas.csi_res_cfg_list = make_csi_resource_configs();

  // CSI-ReportConfig.
  csi_meas.csi_report_cfg_list = make_csi_report_configs(params, pusch_td_alloc_list);

  // Aperiodic parameters.
  if (params.csi_params.enable_aperiodic_report) {
    csi_aperiodic_trigger_state       ts;
    csi_associated_report_config_info report_cfg_info;
    // [Implementation-defined] We have only one report config.
    report_cfg_info.report_cfg_id = static_cast<csi_report_config_id_t>(0);
    report_cfg_info.res_for_channel.emplace<csi_associated_report_config_info::nzp_csi_rs>(
        csi_associated_report_config_info::nzp_csi_rs{.resource_set = 1});
    report_cfg_info.csi_im_resources_for_interference = 1;
    ts.associated_report_cfg_info_list.push_back(report_cfg_info);
    csi_meas.aperiodic_trigger_state_list.push_back(ts);
    csi_meas.report_trigger_size = get_csi_report_trigger_size(params.csi_params);
  }

  return csi_meas;
}
