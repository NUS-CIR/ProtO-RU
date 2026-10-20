// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/span.h"
#include "ocudu/ran/csi_rs/csi_meas_config.h"
#include "ocudu/ran/csi_rs/csi_report_config.h"
#include "ocudu/ran/pci.h"
#include "ocudu/ran/pdsch/pdsch_mcs.h"
#include "ocudu/ran/precoding/precoding_codebook_properties.h"
#include "ocudu/ran/ssb/ssb_properties.h"
#include "ocudu/ran/tdd/tdd_ul_dl_config.h"
#include <array>
#include <optional>
#include <vector>

namespace ocudu {

/// \brief Configurable Type-II codebook parameters in a given DU cell.
///
/// The Type-II codebook is described in TS38.214 Section 5.2.2.2.3.
struct du_type2_codebook_params {
  /// Number of beams \f$L\f$ used for the linear combination, given by \e numberOfBeams. Values: {2, 3, 4}.
  unsigned nof_beams = 2;
  /// Phase alphabet size \f$N_{PSK}\f$, given by \e phaseAlphabetSize.
  pmi_codebook_typeII_phase_size phase_alphabet_size = pmi_codebook_typeII_phase_size::qpsk;
};

/// Configurable CSI parameters in a given DU cell.
struct du_csi_params {
  /// Symbol index within the slot assigned to CSI-RS for channel measurement.
  unsigned cm_csi_ofdm_symbol_index = 4;
  /// Symbol index within the slot assigned to ZP-CSI-RS.
  unsigned zp_csi_ofdm_symbol_index = 8;
  /// \brief Symbol indices within the slot assigned to CSI-RS for tracking resources.
  ///
  /// The possible sets of values are given in TS38.214 Section 5.1.6.1.1. It must be set to {4, 8, 4, 8}, {5, 9, 5, 9}
  /// or {6, 10, 6, 10}.
  std::array<unsigned, 4> tracking_csi_ofdm_symbol_indices = {4, 8, 4, 8};
  /// \brief Period of the CSI-RS resources.
  ///
  /// The UE expects a period of \f$2^\mu\{10, 20, 40, 80\}\f$ slots.
  csi_resource_periodicity csi_rs_period = csi_resource_periodicity::slots80;
  /// \brief Number of NZP-CSI-RS resources used for channel measurement.
  ///
  /// When set to 1 (default), a single omnidirectional resource is configured. When set to N > 1,
  /// N resources are configured in one resource set with repetition disabled, allowing the UE to
  /// measure and report the best resource via CRI.
  unsigned nof_beams = 1;
  /// \brief Slot offsets for NZP channel-measurement CSI-RS resources.
  ///
  /// Size equals \c nof_beams. If left empty, it should be auto-derived.
  /// All offsets are within [0, csi_rs_period) and avoid SSB, SIB1, ZP-CSI-RS, and tracking slots.
  std::vector<unsigned> meas_csi_slot_offsets;
  /// Slot offset of the first CSI-RS resource used for tracking. Note: Should avoid collisions with SSB and SIB1.
  unsigned tracking_csi_slot_offset = 12;
  /// \brief Slot offsets for ZP-CSI-RS resources (size == nof_beams after derivation).
  ///
  /// When left empty, the slot offsets are automatically set to \c meas_csi_slot_offsets.
  /// When non-empty, must have the same size as \c nof_beams and each offset must be valid.
  std::vector<unsigned> zp_csi_slot_offsets;
  /// Report slot offset for periodic CSI reports.
  std::optional<unsigned> csi_report_slot_offset = 9;
  /// Whether to configure aperiodic CSI reports.
  bool enable_aperiodic_report = false;
  /// Power offset of PDSCH RE to NZP CSI-RS RE. Value in dB {-8,...,15}.
  int8_t pwr_ctrl_offset = 0;
  /// \brief Type-II codebook parameters. If empty, the Type-I single-panel codebook is configured.
  ///
  /// The Type-II codebook requires four or more CSI-RS ports and aperiodic CSI reporting, as its PMI is carried in
  /// CSI Part 2.
  std::optional<du_type2_codebook_params> type2_codebook;
};

namespace csi_helper {

/// The current CSI report helper functions only support up to 2, 4 and 8 transmit antenna ports in the downlink.
constexpr unsigned MAX_NOF_ANTENNA_PORTS = 8;

/// Extension of \c du_csi_params with other cell parameters involved in the CSI Meas Config generation.
struct csi_meas_config_builder_params {
  /// PCI of the cell that will determine the scrambling.
  pci_t pci;
  /// \brief Number of RBs used for the CSI-RS. The csi config generators will find the closest number of RBs to
  /// the one provided that is a multiple of 4 as required by the TS 38.331, "CSI-FrequencyOccupation".
  unsigned nof_rbs;
  /// Number of ports set for the CSI-RS.
  unsigned nof_ports = 1;
  /// Maximum number of DL layers.
  unsigned max_nof_layers = 1;
  /// PDSCH MCS table used to select the CSI CQI table.
  pdsch_mcs_table mcs_table = pdsch_mcs_table::qam64;
  /// DU-level CSI parameters.
  du_csi_params csi_params;
};

/// \brief Creates the Type-I single-panel codebook configuration for the given cell parameters.
///
/// Every UE supports this codebook, hence it does not depend on the UE capabilities.
codebook_config::type1 make_type1_codebook_config(const csi_meas_config_builder_params& params);

/// \brief Creates the Type-II codebook configuration for the given cell parameters.
///
/// \remark The cell parameters must enable the Type-II codebook, i.e. \c du_csi_params::type2_codebook must be set.
codebook_config::type2 make_type2_codebook_config(const csi_meas_config_builder_params& params);

/// \brief Compute default CSI-RS signalling period to use, while constrained by TS38.214, 5.1.6.1.1.
csi_resource_periodicity get_max_csi_rs_period(subcarrier_spacing pdsch_scs);

/// \brief Checks whether a specified CSI-RS period is valid for a given TDD pattern.
///
/// The CSI-RS period is valid if:
/// - it is multiple of the TDD period; and
/// - it is one that the UE expects it for the CSI-RS for tracking.
[[nodiscard]] bool is_csi_rs_period_valid(csi_resource_periodicity       csi_rs_period,
                                          const tdd_ul_dl_config_common& tdd_cfg);

/// Checks whether the given SR and CSI periods+offsets will result in SR and CSI being scheduled together in some slot.
[[nodiscard]] bool are_sr_and_csi_pucchs_scheduled_together(unsigned sr_period,
                                                            unsigned sr_offset,
                                                            unsigned csi_period,
                                                            unsigned csi_offset);

/// \brief Searches for a valid CSI-RS periodicity, while constrained by TDD pattern periodicity.
std::optional<csi_resource_periodicity> find_valid_csi_rs_period(const tdd_ul_dl_config_common& tdd_cfg);

/// \brief Search for valid CSI-RS slot offsets for measurement, tracking and interference management.
///
/// \param[in/out] csi_params Parameters used to generate CSI Meas Config. This function assumes that the CSI-RS
/// period, is already set.
/// \param[in] meas_csi_slot_offset Slot offset override for the first (or only) measurement CSI-RS resource. If
/// passed as empty, all \c nof_beams offsets are derived automatically. If passed as non-empty, it is validated and
/// used as \c meas_csi_slot_offsets[0]; remaining beam slots (when \c nof_beams > 1) are derived.
/// \param[in] tracking_csi_slot_offset Slot offset for tracking CSI-RS resources. If passed as empty, a new value is
/// derived. If passed as non-empty, the function will check whether the value is valid.
/// \param[in] zp_csi_slot_offsets Slot offsets for ZP-CSI-RS resources. If empty, automatically set equal to the
/// derived \c meas_csi_slot_offsets. If non-empty, must have size equal to \c nof_beams; each offset is validated.
/// \param[in] tdd_cfg TDD pattern.
/// \param[in] max_csi_symbol_index Maximum CSI symbol among those used for CSI-RS.
/// \param[in] ssb_period SSB periodicity.
/// \param[in] ssb_slot_offsets Slot offsets (within one SSB period) that carry SSB transmissions, using SCS common
/// as reference (not SSB SCS). All listed slots are excluded from CSI-RS placement.
/// \param[in] sib1_period_slots Period of the SIB1 Type0-CSS monitoring window in \c tdd_cfg SCS slots, as returned
/// by \c sib_helper::type0_css_occasions::window_period_slots. Derived from the CORESET multiplexing pattern.
/// \param[in] sib1_slot_offsets Slot offsets (within one SIB1 monitoring window) carrying SIB1 PDSCHs, as returned
/// by \c sib_helper::type0_css_occasions::slot_offsets. All listed slots are excluded from CSI-RS placement.
[[nodiscard]] bool derive_valid_csi_rs_slot_offsets(du_csi_params&                 csi_params,
                                                    const std::optional<unsigned>& meas_csi_slot_offset,
                                                    const std::optional<unsigned>& tracking_csi_slot_offset,
                                                    span<const unsigned>           zp_csi_slot_offsets,
                                                    const tdd_ul_dl_config_common& tdd_cfg,
                                                    unsigned                       max_csi_symbol_index,
                                                    ssb_periodicity                ssb_period,
                                                    span<const unsigned>           ssb_slot_offsets,
                                                    unsigned                       sib1_period_slots,
                                                    span<const unsigned>           sib1_slot_offsets);

/// \brief Generate list of zp-CSI-RS Resources.
std::vector<zp_csi_rs_resource> make_periodic_zp_csi_rs_resource_list(const csi_meas_config_builder_params& params);

/// \brief Generate set of periodic zp-CSI-RS Resources.
zp_csi_rs_resource_set make_periodic_zp_csi_rs_resource_set(const csi_meas_config_builder_params& params);

/// \brief Generate wideband NZP-CSI-RS Resource List, composed by nzp-CSI-RS resources for tracking and channel
/// measurements.
std::vector<nzp_csi_rs_resource> make_nzp_csi_rs_resource_list(const csi_meas_config_builder_params& params);

/// \brief Converts a CSI resource periodicity to a suitable CSI report periodicity. Report peridiocity should be
/// larger than the resource periodicity.
csi_report_periodicity convert_csi_resource_period_to_report_period(csi_resource_periodicity res_period);

/// \brief Generate CSI-IM Resource List, one resource for each periodic ZP-CSI-RS resource.
std::vector<csi_im_resource> make_csi_im_resource_list(const csi_meas_config_builder_params& params);

/// \brief Gets \e reportTriggerSize, the size of the CSI request field in DCI Format 0_1.
/// \return Zero if the cell configures no aperiodic CSI reports.
unsigned get_csi_report_trigger_size(const du_csi_params& csi_params);

/// \brief Generate CSI-MeasConfig.
csi_meas_config make_csi_meas_config(const csi_meas_config_builder_params&                     params,
                                     const std::vector<pusch_time_domain_resource_allocation>& pusch_td_alloc_list);

} // namespace csi_helper
} // namespace ocudu
