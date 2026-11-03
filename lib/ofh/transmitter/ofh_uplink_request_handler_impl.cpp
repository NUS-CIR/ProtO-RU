// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_uplink_request_handler_impl.h"
#include "../support/logger_utils.h"
#include "helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/ofh/ofh_error_notifier.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include "ocudu/ran/prach/prach_configuration.h"
#include "ocudu/ran/prach/prach_frequency_mapping.h"
#include "ocudu/ran/prach/prach_preamble_information.h"
#include "ocudu/ran/resource_block.h"

using namespace ocudu;
using namespace ofh;

/// Determines and returns Open Fronthaul filter index type given the PRACH preamble info and associated context.
static filter_index_type get_prach_cplane_filter_index(const prach_buffer_context&       context,
                                                       const prach_preamble_information& preamble_info)
{
  if (context.format == prach_format_type::three) {
    return filter_index_type::ul_prach_preamble_5kHz;
  }

  if (is_long_preamble(context.format)) {
    return filter_index_type::ul_prach_preamble_1p25khz;
  }

  if (context.format == prach_format_type::C0 || context.format == prach_format_type::C2) {
    return (preamble_info.scs == prach_subcarrier_spacing::kHz15) ? filter_index_type::ul_prach_preamble_short_15kHz
                                                                  : filter_index_type::ul_prach_preamble_short_30kHz;
  }
  return filter_index_type::ul_prach_preamble_short;
}

/// Returns timing parameters with only UL parameters copied from the input, others set to zero.
static tx_window_timing_parameters extract_ul_cp_timing(const tx_window_timing_parameters tx_timing_params)
{
  return tx_window_timing_parameters{.sym_cp_dl_start = 0,
                                     .sym_cp_dl_end   = 0,
                                     .sym_cp_ul_start = tx_timing_params.sym_cp_ul_start,
                                     .sym_cp_ul_end   = tx_timing_params.sym_cp_ul_end,
                                     .sym_up_dl_start = 0,
                                     .sym_up_dl_end   = 0};
}

uplink_request_handler_impl::uplink_request_handler_impl(const uplink_request_handler_impl_config&  config,
                                                         uplink_request_handler_impl_dependencies&& dependencies) :
  logger(dependencies.logger),
  is_prach_cp_enabled(config.is_prach_cp_enabled),
  cp(config.cp),
  tdd_config(config.tdd_config),
  prach_eaxc(config.prach_eaxc),
  ul_eaxc(config.ul_data_eaxc),
  window_checker(dependencies.logger,
                 config.sector,
                 "uplink",
                 get_biggest_min_tx_parameter(extract_ul_cp_timing(config.tx_timing_params)),
                 get_nsymb_per_slot(config.cp)),
  ul_slot_repo(std::move(dependencies.ul_slot_repo)),
  ul_prach_repo(std::move(dependencies.ul_prach_repo)),
  notifier_symbol_repo(std::move(dependencies.notifier_symbol_repo)),
  data_flow(std::move(dependencies.data_flow)),
  frame_pool(std::move(dependencies.frame_pool)),
  err_notifier(dependencies.err_notifier),
  metrics_collector(data_flow->get_metrics_collector(), window_checker),
  enable_log_warnings_for_lates(config.enable_log_warnings_for_lates)
{
  ocudu_assert(ul_slot_repo, "Invalid uplink repository");
  ocudu_assert(ul_prach_repo, "Invalid PRACH repository");
  ocudu_assert(notifier_symbol_repo, "Invalid notified uplink grid symbol repository");
  ocudu_assert(data_flow, "Invalid data flow");
  ocudu_assert(frame_pool, "Invalid frame pool");
}

/// \brief Determine PRACH start symbol index.
///
/// Determine startSymbolId as the last symbol that starts right at or before the PRACH preamble (after the cyclic
/// prefix). According to O-RAN.WG4.CUS.0.R003 section 4.4.3 if the SCS value provided by "frameStructure" is less than
/// 15 kHz (e.g. for long preamble PRACH formats), then the symbol timing used to determine startSymbolId is based on
/// the numerology of 15 kHz SCS.
///
/// \param context PRACH context storing PRACH time and frequency mapping parameters.
/// \return OFDM symbol index that marks the start of PRACH preamble after the cyclic prefix.
static unsigned get_prach_start_symbol(const prach_buffer_context& context)
{
  // Get preamble information (assuming only a single occasion is supported).
  prach_preamble_information preamble_info =
      is_long_preamble(context.format)
          ? get_prach_preamble_long_info(context.format)
          : get_prach_preamble_short_info(context.format, to_ra_subcarrier_spacing(context.pusch_scs), true);

  subcarrier_spacing scs = is_short_preamble(context.format) ? static_cast<subcarrier_spacing>(preamble_info.scs)
                                                             : subcarrier_spacing::kHz15;

  double symbol_duration_sec =
      phy_time_unit::from_units_of_kappa((144U + 2048U) >> to_numerology_value(scs)).to_seconds();
  double cp_length_sec = preamble_info.cp_length.to_seconds();

  return context.start_symbol + unsigned(cp_length_sec / symbol_duration_sec);
}

/// \brief Calculates the start time in phy time units of an OFDM symbol relative to the start of the subframe.
///
/// It accounts for the 16 kappa longer cyclic prefix of the symbols at 0 and 0.5 ms, as per TS38.211 Section 5.3.1.
///
/// \param slot         Slot containing the OFDM symbol.
/// \param symbol_index OFDM symbol index within the slot.
/// \param cp           Cyclic prefix.
/// \return The offset to the OFDM symbol start.
static phy_time_unit get_symbol_start_in_subframe(slot_point slot, unsigned symbol_index, cyclic_prefix cp)
{
  unsigned      symbol_index_sf = slot.subframe_slot_index() * get_nsymb_per_slot(cp) + symbol_index;
  phy_time_unit symbol_start    = phy_time_unit::from_units_of_kappa(2048U >> slot.numerology()) * symbol_index_sf;

  subcarrier_spacing scs = to_subcarrier_spacing(slot.numerology());
  for (unsigned i_symbol = 0; i_symbol != symbol_index_sf; ++i_symbol) {
    symbol_start += cp.get_length(i_symbol, scs);
  }
  return symbol_start;
}

/// \brief Calculates number of times the 16*kappa must be added to a short preamble cyclic prefix.
///
/// As per TS38.211 Section 5.3.2, it is the number of times the interval [preamble_start, preamble_end) overlaps with
/// either time instance 0 or 0.5 ms in the subframe.
///
/// \param preamble_start Preamble start time relative to the start of the subframe.
/// \param preamble_end   Preamble end time relative to the start of the subframe, without cyclic prefix extensions.
/// \return The number of cyclic prefix extensions.
static unsigned get_nof_prach_cp_extensions(phy_time_unit preamble_start, phy_time_unit preamble_end)
{
  static constexpr phy_time_unit half_subframe  = phy_time_unit::from_seconds(0.5e-3);
  static constexpr phy_time_unit start_subframe = phy_time_unit{};

  unsigned nof_extensions = 0;
  if (preamble_start == start_subframe) {
    ++nof_extensions;
  }
  if (preamble_start <= half_subframe && preamble_end > half_subframe) {
    ++nof_extensions;
  }
  return nof_extensions;
}

/// \brief Determines the PRACH timeOffset.
///
/// PRACH C-Plane messages set cpLength to zero, thus timeOffset points from the start of the slot to the start of the
/// first PRACH symbol after its cyclic prefix, as per O-RAN.WG4.CUS.0-R004 sections 4.4.3 and 7.5.2.12. The preamble
/// timing follows TS38.211 Section 5.3.2.
static phy_time_unit get_prach_time_offset(const prach_buffer_context&       context,
                                           const prach_preamble_information& preamble_info,
                                           cyclic_prefix                     cp)
{
  // Long preambles use the 15 kHz reference symbol timing (TS 38.211, clause 5.3.2).
  if (is_long_preamble(context.format)) {
    return get_symbol_start_in_subframe(
               set_slot_numerology(context.slot, to_numerology_value(subcarrier_spacing::kHz15)),
               context.start_symbol,
               cyclic_prefix::NORMAL) +
           preamble_info.cp_length;
  }

  phy_time_unit slot_start  = get_symbol_start_in_subframe(context.slot, 0, cp);
  phy_time_unit prach_start = get_symbol_start_in_subframe(context.slot, context.start_symbol, cp);
  phy_time_unit cp_length   = preamble_info.cp_length;
  phy_time_unit prach_end   = prach_start + cp_length + preamble_info.symbol_length();
  cp_length += phy_time_unit::from_units_of_kappa(16) * get_nof_prach_cp_extensions(prach_start, prach_end);

  return prach_start - slot_start + cp_length;
}

void uplink_request_handler_impl::handle_prach_occasion(const prach_buffer_context& context, shared_prach_buffer buffer)
{
  if (OCUDU_UNLIKELY(logger.debug.enabled())) {
    logger.debug("Registering PRACH context entry for slot '{}' and sector#{}", context.slot, context.sector);
  }

  metrics_collector.update_cp_ul_lates(frame_pool->clear_slot(context.slot, context.sector));

  if (OCUDU_UNLIKELY(window_checker.is_late(context.slot))) {
    log_conditional_warning(
        logger,
        enable_log_warnings_for_lates,
        "Sector#{}: dropped late PRACH request in slot '{}'. No OFH data will be requested from an RU for this slot",
        context.sector,
        context.slot);

    err_notifier.on_late_prach_message({context.slot, context.sector});
    return;
  }

  // Sampling rate defining the \f$T_s = 1/(\Delta f_{ref} \times N_{f,ref})\f$ parameter, see 3GPP TS38.211,
  // clause 4.1.
  // Open Fronthaul parameters timeOffset and cpLength are expressed in multiple of \f$T_s\f$ units.
  static constexpr double ref_srate_Hz = 30.72e6;

  // Store the context in the repository.
  if (is_short_preamble(context.format)) {
    ul_prach_repo->add(context, std::move(buffer), logger, std::nullopt);
    if (OCUDU_UNLIKELY(context.nof_td_occasions > 1)) {
      logger.info("Sector#{}: PRACH with multiple time-domain occasions is configured, however only the first occasion "
                  "will be used in slot '{}'",
                  context.sector,
                  context.slot);
    }
  } else {
    // Determine PRACH start symbol.
    unsigned start_symbol = get_prach_start_symbol(context);

    ul_prach_repo->add(context, std::move(buffer), logger, start_symbol);
  }

  if (!is_prach_cp_enabled) {
    return;
  }
  // Get preamble information (assuming only single occasion is supported).
  prach_preamble_information preamble_info =
      is_long_preamble(context.format)
          ? get_prach_preamble_long_info(context.format)
          : get_prach_preamble_short_info(context.format, to_ra_subcarrier_spacing(context.pusch_scs), true);

  // Get frequency mapping information.
  prach_frequency_mapping_information freq_mapping_info =
      prach_frequency_mapping_get(preamble_info.scs, context.pusch_scs);

  unsigned K = (1000 * scs_to_khz(context.pusch_scs)) / ra_scs_to_Hz(preamble_info.scs);

  data_flow_cplane_scheduling_prach_context cp_prach_context;
  cp_prach_context.slot            = context.slot;
  cp_prach_context.nof_repetitions = preamble_info.nof_symbols;
  cp_prach_context.start_symbol    = get_prach_start_symbol(context);
  cp_prach_context.prach_scs       = preamble_info.scs;
  cp_prach_context.scs             = context.pusch_scs;
  cp_prach_context.prach_nof_rb    = freq_mapping_info.nof_rb_ra * K;
  cp_prach_context.time_offset     = get_prach_time_offset(context, preamble_info, cp).to_samples(ref_srate_Hz);
  cp_prach_context.prach_start_re  = context.rb_offset * K * NOF_SUBCARRIERS_PER_RB;

  // Determine Open Fronthaul filter index.
  cp_prach_context.filter_type = get_prach_cplane_filter_index(context, preamble_info);

  for (auto eaxc : prach_eaxc) {
    cp_prach_context.eaxc = eaxc;
    data_flow->enqueue_section_type_3_prach_message(cp_prach_context);
  }
}

void uplink_request_handler_impl::handle_new_uplink_slot(const resource_grid_context& context,
                                                         const shared_resource_grid&  grid)
{
  if (OCUDU_UNLIKELY(logger.debug.enabled())) {
    logger.debug("Registering UL context entry for slot '{}' and sector#{}", context.slot, context.sector);
  }

  metrics_collector.update_cp_ul_lates(frame_pool->clear_slot(context.slot, context.sector));

  if (OCUDU_UNLIKELY(window_checker.is_late(context.slot))) {
    log_conditional_warning(
        logger,
        enable_log_warnings_for_lates,
        "Sector#{}: dropped late uplink request in slot '{}'. No OFH data will be requested from an RU for this slot",
        context.sector,
        context.slot);

    err_notifier.on_late_uplink_message({context.slot, context.sector});
    return;
  }

  data_flow_cplane_type_1_context df_context;
  df_context.slot         = context.slot;
  df_context.filter_type  = filter_index_type::standard_channel_filter;
  df_context.direction    = data_direction::uplink;
  df_context.symbol_range = tdd_config ? get_active_tdd_ul_symbols(*tdd_config, context.slot.slot_index(), cp)
                                       : ofdm_symbol_range(0, get_nsymb_per_slot(cp));

  // Store the context in the repository.
  ul_slot_repo->add(context, grid, df_context.symbol_range, logger);

  // Add entry to the notified symbol repository.
  notifier_symbol_repo->add(context.slot, df_context.symbol_range.start(), cp);

  for (auto eaxc : ul_eaxc) {
    df_context.eaxc = eaxc;
    data_flow->enqueue_section_type_1_message(df_context);
  }
}
