// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/carrier_configuration.h"
#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/ran/pdcch/aggregation_level.h"
#include "ocudu/ran/pdcch/coreset.h"
#include "ocudu/scheduler/config/cell_config_builder_params.h"
#include "ocudu/scheduler/config/ran_cell_config.h"

namespace ocudu::config_helpers {

/// Config struct that extends cell_config_builder_params with parameters that can be derived from the former.
struct cell_config_builder_params_extended : public cell_config_builder_params {
  cell_config_builder_params_extended(const cell_config_builder_params& source = {});

  /// \brief Absolute frequency of the SSB as ARFCN. This is the ARFCN of the \c SS_ref (or SSB central frequency).
  /// \c SS_ref is defined is per TS 38.104, Section 5.4.3.1 and 5.4.3.2.
  std::optional<arfcn_t> ssb_arfcn;
  unsigned               cell_nof_crbs;
  /// \brief Maximum number of DL layers. Derived from \c max_rank if configured, otherwise from the number of DL
  /// antenna ports, and in all cases bounded by the single-codeword layer limit.
  unsigned max_nof_layers;
};

/// Generates a default DL carrier configuration based on the input parameters.
carrier_configuration make_default_dl_carrier_configuration(const cell_config_builder_params_extended& params);

/// Generates a default UL carrier configuration based on the input parameters.
carrier_configuration make_default_ul_carrier_configuration(const cell_config_builder_params_extended& params);

/// Generates a default SSB configuration based on the input parameters.
ssb_configuration make_default_ssb_config(const cell_config_builder_params_extended& params);

/// Generates default RAN cell configuration used by gNB DU. The default configuration should be valid.
ran_cell_config make_default_ran_cell_config(const cell_config_builder_params_extended& params = {});

/// Builds CSI meas config builder parameters from the RAN cell configuration.
csi_helper::csi_meas_config_builder_params make_csi_meas_config_builder_params(const ran_cell_config& cell_cfg);

/// Builds a CORESET configuration from the input parameters and a CORESET ID.
coreset_configuration make_default_coreset_config(const config_helpers::cell_config_builder_params_extended& params,
                                                  coreset_id cs_id = to_coreset_id(1));

/// Builds a Search Space configuration from the input parameters.
search_space_configuration
make_default_common_search_space_config(const config_helpers::cell_config_builder_params_extended& params = {});

/// Compute the maximum number of candidates that can be accommodated in a CORESET for a given aggregation level.
uint8_t compute_max_nof_candidates(aggregation_level aggr_lvl, const coreset_configuration& cs_cfg);

/// \brief Returns the first symbol of the slot that the cell's SRS resources can never occupy.
///
/// The SRS resources are whole blocks of \c srs_cfg.nof_symbols anchored at the last symbol of the slot and laid down
/// while they still fit within \c srs_cfg.max_nof_symbols (see \c generate_cell_srs_list()), so the region actually
/// occupied is the largest multiple of the block length that fits in that budget. Whatever is left of the budget below
/// it is too short to hold another resource and stays free, hence treating the whole budget as occupied would give up
/// usable symbols; with a block length that does not divide the budget at all, no SRS resource is generated and the
/// entire slot is free.
/// \return The number of symbols in the slot, if SRS is disabled or no SRS resource fits.
unsigned compute_last_non_srs_symbol(const ran_cell_config& cell_cfg);

/// \brief Finds the index, into the cell's common PUSCH time-domain resource list, of the resource a Configured Grant
/// PUSCH should use, i.e. the first one whose symbols end before the SRS region at the end of the slot.
///
/// The list is sorted by increasing k2 first, then by decreasing \c symbols.stop(), so the first qualifying entry is
/// the one with both the lowest k2 and the most symbols among those that avoid the SRS.
/// \return The index of that resource, or \c std::nullopt if every resource in the list overlaps the SRS region, in
/// which case no CG PUSCH can be placed in the cell at all.
/// \remark This function must only be called if the cell has CG enabled (i.e. \c cell_cfg.init_bwp.cg_cfg is set).
std::optional<unsigned> find_cg_pusch_td_res_idx(const ran_cell_config& cell_cfg);

/// \brief Builds the cell-default Configured Grant configuration from the cell-level CG parameters.
/// \remark This function must only be called if the cell has CG enabled (i.e. \c cell_cfg.init_bwp.cg_cfg is set).
cg_configuration make_default_cell_cg_config(const ran_cell_config& cell_cfg);

/// \brief Computes the number of PRBs required per UE by a Configured Grant, derived from the CG grant size or
/// bitrate and the configured MCS.
/// \remark This function must only be called if the cell has CG enabled (i.e. \c cell_cfg.init_bwp.cg_cfg is set).
unsigned compute_nof_cg_prbs_per_ue(const ran_cell_config& cell_cfg, const cg_configuration& cg_cfg);

/// \brief Computes the CRB blocks usable by the Type-2 Configured Grant resources of a cell.
///
/// The band available for CG is the part of the UL BWP left free by the PUCCH guardbands, capped by \c
/// max_nof_cell_cg_rbs; it is split into contiguous blocks of the number of PRBs a UE requires, one block per CG
/// resource. Two UEs activated on the same slot offset are handed two different resources, so the blocks returned by
/// this function never overlap.
/// \return The CRB block of each CG resource, indexed by resource id. Empty if no CG resource fits in the cell.
/// \remark This function must only be called if the cell has CG enabled (i.e. \c cell_cfg.init_bwp.cg_cfg is set).
std::vector<crb_interval> compute_cg_type2_freq_resources(const ran_cell_config& cell_cfg);

/// \brief Computes the slot offsets, within the CG period, that the Configured Grant resources of a cell can be
/// placed at. Applies to both CG types.
///
/// A CG offset recurs every CG period, and each of its occurrences falls at a different point of the TDD and PRACH
/// patterns; the offset is usable only if every one of them is a full-UL slot (in TDD) carrying no PRACH occasion. The
/// occurrences within the LCM of the CG, PRACH and TDD periods are all the distinct slots the offset can land on.
/// \return The usable slot offsets, in increasing order. Empty if the cell has no slot available for CG.
/// \remark This function must only be called if the cell has CG enabled (i.e. \c cell_cfg.init_bwp.cg_cfg is set).
std::vector<unsigned> compute_cg_usable_slot_offsets(const ran_cell_config& cell_cfg);

} // namespace ocudu::config_helpers
