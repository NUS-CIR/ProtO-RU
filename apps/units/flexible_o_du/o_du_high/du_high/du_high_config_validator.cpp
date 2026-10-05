// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "du_high_config_validator.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/antenna_topology.h"
#include "ocudu/ran/duplex_mode.h"
#include "ocudu/ran/frame_types.h"
#include "ocudu/ran/nr_cell_identity.h"
#include "ocudu/ran/pdcch/pdcch_type0_css_coreset_config.h"
#include "ocudu/ran/pdsch/pdsch_constants.h"
#include "ocudu/ran/prach/prach_configuration.h"
#include "ocudu/ran/prach/prach_helper.h"
#include "ocudu/ran/pucch/pucch_constants.h"
#include "ocudu/ran/pucch/pucch_info.h"
#include "ocudu/ran/pucch/pucch_mapping.h"
#include "ocudu/ran/pusch/pusch_constants.h"
#include "ocudu/ran/rb_id.h"
#include "ocudu/ran/ssb/ssb_mapping.h"
#include "ocudu/ran/transform_precoding/transform_precoding_helpers.h"
#include "ocudu/rlc/rlc_config.h"
#include "ocudu/scheduler/rrm/ue_capability_summary.h"
#include "ocudu/support/math/math_utils.h"
#include <algorithm>
#include <cmath>
#include <set>
#include <unordered_map>

using namespace ocudu;

static bool validate_pcap_configs(const du_high_unit_config& config)
{
  if (config.pcaps.mac.enabled) {
    for (const auto& cell : config.cells_cfg) {
      // FIXME don't use just the nof_antennas, but compute MAX PDU size from bandwidth, MCS table, etc.
      if (cell.cell.nof_antennas_dl >= 2 && config.pcaps.mac.type != "dlt") {
        fmt::print("Using more than one DL antenna might make the MAC PDU larger than what is supported in the UDP "
                   "wrapper. Use PCAP DLT type to avoid pcap corruption.\n");
        return false;
      }
      if (cell.cell.nof_antennas_ul >= 2 && config.pcaps.mac.type != "dlt") {
        fmt::print("Using more than one UL antenna might make the MAC PDU larger than what is supported in the UDP "
                   "wrapper. Use PCAP DLT type to avoid pcap corruption.\n");
        return false;
      }
    }
  }
  return true;
}

template <typename id_type>
static bool validate_rlc_am_unit_config(id_type id, const du_high_unit_rlc_am_config& config)
{
  // Validate TX.
  rlc_am_sn_size tmp_sn_size;
  if (!from_number(tmp_sn_size, config.tx.sn_field_length)) {
    fmt::print("RLC AM TX SN length is neither 12 or 18 bits. {} sn_size={}\n", id, config.tx.sn_field_length);
    return false;
  }

  rlc_t_poll_retransmit tmp_t_poll_retransmit;
  if (!rlc_t_poll_retransmit_from_int(tmp_t_poll_retransmit, config.tx.t_poll_retx)) {
    fmt::print("Invalid RLC AM TX t-PollRetransmission. {} t_poll_retx={}\n", id, config.tx.t_poll_retx);
    fmt::print(" Valid values are: ms5, ms10, ms15, ms20, ms25, ms30, ms35,"
               " ms40, ms45, ms50, ms55, ms60, ms65, ms70, ms75, ms80, ms85,"
               " ms90, ms95, ms100, ms105, ms110, ms115, ms120, ms125, ms130,"
               " ms135, ms140, ms145, ms150, ms155, ms160, ms165, ms170, ms175,"
               " ms180, ms185, ms190, ms195, ms200, ms205, ms210, ms215, ms220,"
               " ms225, ms230, ms235, ms240, ms245, ms250, ms300, ms350, ms400,"
               " ms450, ms500, ms800, ms1000, ms2000, ms4000\n");
    return false;
  }

  rlc_max_retx_threshold tmp_max_retx_threshold;
  if (!rlc_max_retx_threshold_from_int(tmp_max_retx_threshold, config.tx.max_retx_thresh)) {
    fmt::print("Invalid RLC AM TX max retx threshold. {} max_retx_threshold={}\n", id, config.tx.max_retx_thresh);
    fmt::print(" Valid values are: t1, t2, t3, t4, t6, t8, t16, t32\n");
    return false;
  }

  rlc_poll_pdu tmp_poll_pdu;
  if (!rlc_poll_pdu_from_int(tmp_poll_pdu, config.tx.poll_pdu)) {
    fmt::print("Invalid RLC AM TX PollPDU. {} poll_pdu={}\n", id, config.tx.poll_pdu);
    fmt::print(" Valid values are:"
               "p4, p8, p16, p32, p64, p128, p256, p512, p1024, p2048,"
               " p4096, p6144, p8192, p12288, p16384,p20480,"
               " p24576, p28672, p32768, p40960, p49152, p57344, p65536\n");
    return false;
  }

  rlc_poll_kilo_bytes tmp_poll_bytes;
  if (!rlc_poll_kilo_bytes_from_int(tmp_poll_bytes, config.tx.poll_byte)) {
    fmt::print("Invalid RLC AM TX PollBytes. {} poll_bytes={}\n", id, config.tx.poll_byte);
    fmt::print(" Valid values are (in KBytes):"
               " kB1, kB2, kB5, kB8, kB10, kB15, kB25, kB50, kB75,"
               " kB100, kB125, kB250, kB375, kB500, kB750, kB1000,"
               " kB1250, kB1500, kB2000, kB3000, kB4000, kB4500,"
               " kB5000, kB5500, kB6000, kB6500, kB7000, kB7500,"
               " mB8, mB9, mB10, mB11, mB12, mB13, mB14, mB15,"
               " mB16, mB17, mB18, mB20, mB25, mB30, mB40, infinity\n");
    return false;
  }

  if (config.tx.queue_size == 0) {
    fmt::print("RLC AM TX queue size cannot be 0. {}\n", id);
    return false;
  }

  // Validate RX.

  if (!from_number(tmp_sn_size, config.rx.sn_field_length)) {
    fmt::print("RLC AM RX SN length is neither 12 or 18 bits. {} sn_size={}\n", id, config.rx.sn_field_length);
    return false;
  }

  rlc_t_reassembly tmp_t_reassembly;
  if (!rlc_t_reassembly_from_int(tmp_t_reassembly, config.rx.t_reassembly)) {
    fmt::print("RLC AM RX t-Reassembly is invalid. {} t_reassembly={}\n", id, config.rx.t_reassembly);
    fmt::print("Valid values are:"
               " ms40, ms45, ms50, ms55, ms60, ms65, ms70,"
               " ms75, ms80, ms85, ms90, ms95, ms100, ms110,"
               " ms120, ms130, ms140, ms150, ms160, ms170,"
               " ms180, ms190, ms200\n");
    return false;
  }

  rlc_t_status_prohibit tmp_t_status_prohibit;
  if (!rlc_t_status_prohibit_from_int(tmp_t_status_prohibit, config.rx.t_status_prohibit)) {
    fmt::print("RLC AM RX t-statusProhibit is invalid. {} t_status_prohibit={}\n", id, config.rx.t_status_prohibit);
    fmt::print("Valid values are:"
               "ms0, ms5, ms10, ms15, ms20, ms25, ms30, ms35,"
               "ms40, ms45, ms50, ms55, ms60, ms65, ms70,"
               "ms75, ms80, ms85, ms90, ms95, ms100, ms105,"
               "ms110, ms115, ms120, ms125, ms130, ms135,"
               "ms140, ms145, ms150, ms155, ms160, ms165,"
               "ms170, ms175, ms180, ms185, ms190, ms195,"
               "ms200, ms205, ms210, ms215, ms220, ms225,"
               "ms230, ms235, ms240, ms245, ms250, ms300,"
               "ms350, ms400, ms450, ms500, ms800, ms1000,"
               "ms1200, ms1600, ms2000, ms2400\n");
    return false;
  }

  if (config.rx.max_sn_per_status >= window_size(config.rx.sn_field_length)) {
    fmt::print("RLC AM RX max_sn_per_status={} exceeds window_size={}. sn_size={}\n",
               config.rx.max_sn_per_status,
               window_size(config.rx.sn_field_length),
               config.rx.sn_field_length);
    return false;
  }

  return true;
}

/// Validates the given SRB configuration. Returns true on success, otherwise false.
static bool validate_srb_unit_config(const std::vector<du_high_unit_srb_config>& config)
{
  for (const auto& srb : config) {
    const srb_id_t srb_id = static_cast<srb_id_t>(srb.srb_id);
    if (srb_id != srb_id_t::srb1 && srb_id != srb_id_t::srb2 && srb_id != srb_id_t::srb3) {
      fmt::print("Cannot configure {}. Only SRB1, SRB2 and SRB3 can be configured", srb_id);
      return false;
    }
    if (!validate_rlc_am_unit_config(srb_id, srb.rlc)) {
      return false;
    }
    if (srb_id != srb_id_t::srb1 && srb.mac.triggered_ul_grant.has_value()) {
      fmt::print("{} can't be configured with triggered UL grant. It's supported only for SBR1\n", srb_id);
      return false;
    }
    // Delay is in ms and capped to bound the pending-grant vector size in the scheduler.
    if (srb.mac.triggered_ul_grant.has_value() && srb.mac.triggered_ul_grant->delay > SCHEDULER_MAX_TRIG_UL_DELAY) {
      fmt::print("{} triggered_ul_grant delay={} exceeds maximum of {} ms\n",
                 srb_id,
                 srb.mac.triggered_ul_grant->delay,
                 SCHEDULER_MAX_TRIG_UL_DELAY);
      return false;
    }
  }
  return true;
}

static bool validate_pdcch_unit_config(const du_high_unit_base_cell_config& base_cell)
{
  const auto     band = base_cell.band.value_or(band_helper::get_band_from_dl_arfcn(base_cell.dl_f_ref_arfcn));
  const unsigned nof_crbs =
      band_helper::get_n_rbs_from_bw(base_cell.channel_bw_mhz, base_cell.common_scs, band_helper::get_freq_range(band));
  if (base_cell.pdcch_cfg.common.coreset0_index.has_value()) {
    const uint8_t  ss0_idx               = base_cell.pdcch_cfg.common.ss0_index;
    const unsigned cs0_idx               = base_cell.pdcch_cfg.common.coreset0_index.value();
    const auto     ssb_coreset0_freq_loc = band_helper::get_ssb_coreset0_freq_location_for_cset0_idx(
        base_cell.dl_f_ref_arfcn, band, nof_crbs, base_cell.common_scs, base_cell.common_scs, ss0_idx, cs0_idx);
    if (not ssb_coreset0_freq_loc.has_value()) {
      fmt::print("Unable to derive a valid SSB pointA and k_SSB for CORESET#0 index={}, SearchSpace#0 index={} and "
                 "cell bandwidth={}Mhz\n",
                 cs0_idx,
                 ss0_idx,
                 fmt::underlying(base_cell.channel_bw_mhz));
      return false;
    }
    // NOTE: A CORESET#0 duration of 3 symbols is allowed. make_default_ran_cell_config() then sets dmrs-TypeA-Position
    // to pos3, as per TS 38.211, Section 7.3.2.2.
    const pdcch_type0_css_coreset_description desc = ocudu::pdcch_type0_css_coreset_get(
        band, base_cell.common_scs, base_cell.common_scs, cs0_idx, ssb_coreset0_freq_loc->k_ssb.value());
    if (base_cell.pdcch_cfg.common.max_coreset0_duration.has_value() and
        desc.pattern != PDCCH_TYPE0_CSS_CORESET_RESERVED.pattern and
        desc.nof_symb_coreset > base_cell.pdcch_cfg.common.max_coreset0_duration.value()) {
      fmt::print("Configured CORESET#0 index={} results in duration={} > maximum CORESET#0 duration configured={}. "
                 "Try increasing maximum CORESET#0 duration or pick another CORESET#0 index\n",
                 cs0_idx,
                 desc.nof_symb_coreset,
                 base_cell.pdcch_cfg.common.max_coreset0_duration.value());
      return false;
    }
  }
  if (base_cell.pdcch_cfg.dedicated.coreset1_rb_start.has_value() and
      base_cell.pdcch_cfg.dedicated.coreset1_rb_start.value() > nof_crbs) {
    fmt::print("Invalid CORESET#1 RBs start={}\n", base_cell.pdcch_cfg.dedicated.coreset1_rb_start.value());
    return false;
  }
  if (base_cell.pdcch_cfg.dedicated.coreset1_l_crb.has_value() and
      base_cell.pdcch_cfg.dedicated.coreset1_l_crb.value() > nof_crbs) {
    fmt::print("Invalid CORESET#1 length in RBs={}\n", base_cell.pdcch_cfg.dedicated.coreset1_l_crb.value());
    return false;
  }
  if (base_cell.pdcch_cfg.dedicated.coreset1_rb_start.has_value() and
      base_cell.pdcch_cfg.dedicated.coreset1_l_crb.has_value() and
      base_cell.pdcch_cfg.dedicated.coreset1_rb_start.value() + base_cell.pdcch_cfg.dedicated.coreset1_l_crb.value() >
          nof_crbs) {
    fmt::print("CORESET#1 range={{},{}} outside of cell bandwidth={}CRBs\n",
               base_cell.pdcch_cfg.dedicated.coreset1_rb_start.value(),
               base_cell.pdcch_cfg.dedicated.coreset1_rb_start.value() +
                   base_cell.pdcch_cfg.dedicated.coreset1_l_crb.value(),
               nof_crbs);
    return false;
  }
  if (base_cell.pdcch_cfg.dedicated.ss2_type == search_space_configuration::type_t::common and
      base_cell.pdcch_cfg.dedicated.dci_format_0_1_and_1_1) {
    fmt::print("Non-fallback DCI format not allowed in Common SearchSpace\n");
    return false;
  }

  // Validate that the paging DCI aggregation level has at least one candidate in the common search space.
  const unsigned al  = base_cell.scheduler_cfg.paging_dci_aggr_lev;
  const unsigned idx = log2_ceil(al);
  if (base_cell.paging_cfg.paging_search_space_id == 0) {
    // Type0-PDCCH CSS (SS#0) has fixed candidate counts per TS 38.213, Table 10.1-1.
    // AL1 and AL2 always have 0 candidates and are therefore invalid for paging.
    if (al < 4) {
      fmt::print("Paging aggregation level {} has no PDCCH candidates in Type0-PDCCH CSS (SearchSpace#0). "
                 "Only AL {{4, 8, 16}} are valid. Adjust --paging_dci_aggr_lev.\n",
                 al);
      return false;
    }
  } else if (base_cell.paging_cfg.paging_search_space_id == 1) {
    // SS#1 candidate counts are user-configurable via ss1_n_candidates.
    if (base_cell.pdcch_cfg.common.ss1_n_candidates[idx] == 0) {
      fmt::print("Paging aggregation level {} has no PDCCH candidates in SearchSpace#1. "
                 "Adjust --ss1_n_candidates or --paging_dci_aggr_lev.\n",
                 al);
      return false;
    }
  }

  return true;
}

static bool validate_rv_sequence(span<const unsigned> rv_sequence)
{
  if (rv_sequence.empty()) {
    fmt::print("Invalid RV sequence. The RV sequence cannot be empty.\n");
    return false;
  }
  if (rv_sequence[0] != 0) {
    fmt::print("Invalid RV sequence. The first RV must be 0.\n");
    return false;
  }
  return true;
}

/// Validates the given PDSCH cell application configuration. Returns true on success, otherwise false.
static bool validate_pdsch_cell_unit_config(const du_high_unit_pdsch_config& config,
                                            unsigned                         cell_bw_crbs,
                                            unsigned                         nof_antennas_dl,
                                            bool                             is_ntn_band)
{
  if (config.min_ue_mcs > config.max_ue_mcs) {
    fmt::print("Invalid UE MCS range (i.e., [{}, {}]). The min UE MCS must be less than or equal to the max UE MCS.\n",
               config.min_ue_mcs,
               config.max_ue_mcs);
    return false;
  }

  if (config.res_alloc != pdsch_config::resource_allocation::resource_allocation_type_1 and
      config.interleaving_bundle_size != vrb_to_prb::mapping_type::non_interleaved) {
    fmt::print("PDSCH resource allocation type 0 is not supported with interleaving. Set interleaving_bundle_size to 0 "
               "or resource_allocation to type1.\n");
    return false;
  }

  unsigned max_ue_mcs = 28;
  if (config.mcs_table == pdsch_mcs_table::qam256) {
    max_ue_mcs = 27;
  }

  if (config.min_ue_mcs > max_ue_mcs) {
    fmt::print("Invalid PDSCH min_ue_mcs (i.e., {}) for the selected MCS table (i.e., {}).\n",
               config.min_ue_mcs,
               (config.mcs_table == pdsch_mcs_table::qam256) ? "qam256" : "qam64");
    return false;
  }

  if (not validate_rv_sequence(config.rv_sequence)) {
    return false;
  }

  if (config.end_rb <= config.start_rb) {
    fmt::print("Invalid RB allocation range [{}, {}) for UE PDSCHs. The start_rb must be less than the end_rb.\n",
               config.start_rb,
               config.end_rb);
    return false;
  }

  if (config.max_rb_size < config.min_rb_size) {
    fmt::print("Invalid UE PDSCH RB range [{}, {}). The min_rb_size must be less or equal to the max_rb_size.\n",
               config.min_rb_size,
               config.max_rb_size);
    return false;
  }

  if (config.start_rb >= cell_bw_crbs) {
    fmt::print("Invalid start RB {} for UE PDSCHs. The start_rb must be less than the cell BW", config.start_rb);
    return false;
  }

  if (config.max_rank.has_value()) {
    if (*config.max_rank > nof_antennas_dl) {
      fmt::print("Invalid maximum rank value (i.e., {}). It cannot be greater than number of available downlink "
                 "antennas (i.e., {}).\n",
                 *config.max_rank,
                 nof_antennas_dl);
      return false;
    }
    if (*config.max_rank > pdsch_constants::MAX_NOF_LAYERS_PER_CODEWORD) {
      fmt::print("Invalid maximum rank value (i.e., {}). It cannot be greater than maximum number of layers with "
                 "single codeword (i.e., {}).\n",
                 *config.max_rank,
                 pdsch_constants::MAX_NOF_LAYERS_PER_CODEWORD);
      return false;
    }

    if (*config.max_rank == 0) {
      fmt::print("Invalid maximum rank value (i.e., {}). It must be a positive non-zero value.\n", *config.max_rank);
      return false;
    }
  }

  if (config.nof_harqs == 32 and not is_ntn_band) {
    fmt::print("Number of UE DL HARQ processes can be equal to 32 only in NTN cells.\n");
    return false;
  }

  if (config.harq_feedback_disabled.any() and not is_ntn_band) {
    fmt::print("DL HARQ Feedback can be disabled only in NTN cells.\n");
    return false;
  }

  return true;
}

/// Validates the given CSI cell application configuration. Returns true on success, otherwise false.
static bool validate_csi_cell_unit_config(const du_high_unit_csi_config&                      config,
                                          subcarrier_spacing                                  scs_common,
                                          unsigned                                            cell_bw_crbs,
                                          unsigned                                            nof_antennas_dl,
                                          const std::optional<du_high_unit_tdd_ul_dl_config>& tdd_cfg)
{
  // CSI RS period limitation due to TS 38.214 Section 5.1.6.1.1:
  // "- the UE is not expected to be configured with the periodicity of 2 μ × 10 slots if the bandwidth of CSI-RS
  //    resource is larger than 52 resource blocks."
  if ((config.csi_rs_period_msec == 10) && (cell_bw_crbs > 52)) {
    fmt::print("Invalid CSI-RS period. UEs are not expected to be configured with CSI-RS period of 10ms when the "
               "bandwidth exceeds 52 resource blocks.\n");
    return false;
  }

  const auto csi_period_slots = get_nof_slots_per_subframe(scs_common) * config.csi_rs_period_msec;
  if (tdd_cfg.has_value()) {
    const unsigned tdd_period_slots =
        tdd_cfg.value().pattern1.dl_ul_period_slots +
        (tdd_cfg.value().pattern2.has_value() ? tdd_cfg.value().pattern2.value().dl_ul_period_slots : 0U);
    if (csi_period_slots < tdd_period_slots) {
      fmt::print(
          "In TDD mode, the CSI-RS period in slots (i.e., {}ms equivalent to {} slots) must be larger than or equal "
          "to the TDD period measured in slots\n",
          config.csi_rs_period_msec,
          csi_period_slots);
      return false;
    }
  }

  if (config.type2_codebook_enabled) {
    if (not config.csi_rs_enabled) {
      fmt::print("Type-II CSI reporting requires CSI-RS to be enabled.\n");
      return false;
    }

    // As per TS 38.214 Section 5.2.2.2.3, the Type-II codebook is only defined for 4 or more CSI-RS ports.
    if (nof_antennas_dl < 4) {
      fmt::print("Type-II CSI reporting requires at least 4 DL antennas, given {}.\n", nof_antennas_dl);
      return false;
    }

    // The Type-II PMI is reported in CSI Part 2, which is only multiplexed in PUSCH.
    if (config.report_type != csi_report_type::aperiodic) {
      fmt::print("Type-II CSI reporting requires aperiodic CSI reporting, as the PMI is carried in CSI Part 2.\n");
      return false;
    }
  }

  return true;
}

/// Validates the given PUSCH cell application configuration. Returns true on success, otherwise false.
static bool validate_pusch_cell_unit_config(const du_high_unit_pusch_config& config,
                                            unsigned                         cell_crbs,
                                            unsigned                         min_k1,
                                            bool                             is_ntn_band)
{
  if (config.min_ue_mcs > config.max_ue_mcs) {
    fmt::print("Invalid UE MCS range (i.e., [{}, {}]). The min UE MCS must be less than or equal to the max UE MCS.\n",
               config.min_ue_mcs,
               config.max_ue_mcs);
    return false;
  }

  // Modulation and Code Scheme MCS ranges are given in TS38.214 Section 6.1.4.2.
  unsigned max_ue_mcs = 28;
  if ((config.mcs_table == pusch_mcs_table::qam256) || config.enable_transform_precoding) {
    max_ue_mcs = 27;
  }

  if (config.min_ue_mcs > max_ue_mcs) {
    fmt::print(
        "Invalid PUSCH min_ue_mcs (i.e., {}) for the selected MCS table (i.e., {}) with transform precoding {}.\n",
        config.min_ue_mcs,
        (config.mcs_table == pusch_mcs_table::qam256) ? "qam256" : "qam64",
        config.enable_transform_precoding ? "enabled" : "disabled");
    return false;
  }

  if (not validate_rv_sequence(config.rv_sequence)) {
    return false;
  }

  // The PHY bounds how much it soft-combines, and a repetition counts the same as a retransmission.
  const unsigned nof_soft_combined_txs = (1U + config.max_nof_harq_retxs) * config.max_nof_rep;
  if (nof_soft_combined_txs > pusch_constants::MAX_NOF_SOFT_COMBINED_PUSCH_TXS) {
    fmt::print("Invalid PUSCH repetition configuration. A transport block would be transmitted up to {} times "
               "((1 + max_nof_harq_retxs={}) * max_nof_rep={}), above the {} receptions the receiver can combine. "
               "Lower max_nof_rep or max_nof_harq_retxs.\n",
               nof_soft_combined_txs,
               config.max_nof_harq_retxs,
               config.max_nof_rep,
               pusch_constants::MAX_NOF_SOFT_COMBINED_PUSCH_TXS);
    return false;
  }

  if (config.max_rb_size < config.min_rb_size) {
    fmt::print("Invalid UE PUSCH RB range [{}, {}). The min_rb_size must be less or equal to the max_rb_size.\n",
               config.min_rb_size,
               config.max_rb_size);
    return false;
  }

  if (config.enable_transform_precoding && !transform_precoding::is_nof_prbs_valid(config.min_rb_size)) {
    fmt::print("Invalid minimum UE PUSCH RB (i.e., {}) with transform precoding. The nearest lower number of PRB is {} "
               "and the higher is {}.\n",
               config.min_rb_size,
               transform_precoding::get_nof_prbs_lower_bound(config.min_rb_size),
               transform_precoding::get_nof_prbs_upper_bound(config.min_rb_size));
    return false;
  }

  if (config.end_rb <= config.start_rb) {
    fmt::print("Invalid RB allocation range [{}, {}) for UE PUSCHs. The start_rb must be less than the end_rb.\n",
               config.start_rb,
               config.end_rb);
    return false;
  }

  if (config.start_rb >= cell_crbs) {
    fmt::print("Invalid start RB {} for UE PUSCHs. The start_rb must be less than the cell BW.\n", config.start_rb);
    return false;
  }

  if (min_k1 < config.min_k2) {
    fmt::print("The value min_k2 {} set for PUSCH cannot be greater than the min_k1 {} set for PUCCH config.\n",
               config.min_k2,
               min_k1);
    return false;
  }

  if (config.nof_harqs == 32 and not is_ntn_band) {
    fmt::print("Number of UE UL HARQ processes can be equal to 32 only in NTN cells.\n");
    return false;
  }

  if (config.harq_mode_b.any() and not is_ntn_band) {
    fmt::print("UL HARQ Mode B can be used only in NTN cells.\n");
    return false;
  }

  return true;
}

static bool validate_global_ntn_config(const std::vector<ntn_satellite_config>& ntn_satellites)
{
  bool               valid = true;
  std::set<unsigned> seen_indices;

  for (const auto& sat : ntn_satellites) {
    if (!sat.satellite_idx) {
      fmt::print("ntn.satellites: satellite_idx has to be provided.\n");
      valid = false;
      continue;
    }
    if (!seen_indices.insert(*sat.satellite_idx).second) {
      fmt::print("ntn.satellites: duplicate satellite_idx {}.\n", *sat.satellite_idx);
      valid = false;
    }
    if (sat.gateway_location && sat.ta_info) {
      fmt::print("ntn.satellites[{}]: gateway_location and ta_info are mutually exclusive.\n", *sat.satellite_idx);
      valid = false;
    }
    if (!sat.ephemeris_info) {
      fmt::print("ntn.satellites[{}]: Ephemeris info has to be provided (in ECEF State Vector or ECI Orbital "
                 "Elements format).\n",
                 *sat.satellite_idx);
      valid = false;
    }
  }
  return valid;
}

static bool validate_ntn_satellite_refs(const std::vector<du_high_unit_cell_config>& cells,
                                        const std::vector<ntn_satellite_config>&     ntn_satellites)
{
  std::unordered_map<unsigned, const ntn_satellite_config*> available;
  for (const auto& sat : ntn_satellites) {
    available.emplace(*sat.satellite_idx, &sat);
  }

  bool valid = true;
  for (unsigned i = 0; i < cells.size(); ++i) {
    const auto& cell_cfg = cells[i].cell;
    if (!cell_cfg.ntn_cfg) {
      continue;
    }
    const auto& ntn = *cell_cfg.ntn_cfg;
    if (ntn.serving) {
      const auto& serving = *ntn.serving;
      if (serving.sat_ref.satellite_idx) {
        const auto it = available.find(*serving.sat_ref.satellite_idx);
        if (it == available.end()) {
          fmt::print(
              "cells[{}].ntn.satellite_idx={} not found in ntn.satellites.\n", i, *serving.sat_ref.satellite_idx);
          valid = false;
        } else if (serving.feeder_link_info) {
          if (not it->second->gateway_location and not it->second->ta_info) {
            fmt::print("cells[{}].ntn: feeder_link_info requires the referenced satellite (satellite_idx={}) to have "
                       "gateway_location or ta_info set.\n",
                       i,
                       *serving.sat_ref.satellite_idx);
            valid = false;
          }
        }
      }
      if (serving.sat_switch_with_resync && serving.sat_switch_with_resync->sat_ref.satellite_idx) {
        unsigned   sw_idx = *serving.sat_switch_with_resync->sat_ref.satellite_idx;
        const auto it     = available.find(sw_idx);
        if (it == available.end()) {
          fmt::print("cells[{}].ntn.sat_switch_with_resync.satellite_idx={} not found in ntn.satellites.\n", i, sw_idx);
          valid = false;
        } else if (not it->second->gateway_location and not it->second->ta_info) {
          fmt::print("cells[{}].ntn.sat_switch_with_resync: referenced satellite (satellite_idx={}) must have "
                     "gateway_location or ta_info set.\n",
                     i,
                     sw_idx);
          valid = false;
        }
      }
    }
    for (unsigned j = 0, e = ntn.ncells.size(); j != e; ++j) {
      const auto& ncell = ntn.ncells[j];
      if (ncell.sat_ref.satellite_idx) {
        const auto it = available.find(*ncell.sat_ref.satellite_idx);
        if (it == available.end()) {
          fmt::print("cells[{}].ntn.ncells[{}].satellite_idx={} not found in ntn.satellites.\n",
                     i,
                     j,
                     *ncell.sat_ref.satellite_idx);
          valid = false;
        } else if (ncell.has_feeder_link and not it->second->gateway_location and not it->second->ta_info) {
          // A neighbour can only broadcast ta-Info (the feeder-link common delay) if its satellite can compute one.
          fmt::print("cells[{}].ntn.ncells[{}]: has_feeder_link requires the referenced satellite (satellite_idx={}) "
                     "to have gateway_location or ta_info set.\n",
                     i,
                     j,
                     *ncell.sat_ref.satellite_idx);
          valid = false;
        }
      } else if (ncell.has_feeder_link and not ncell.sat_ref.gateway_location and not ncell.sat_ref.ta_info) {
        fmt::print("cells[{}].ntn.ncells[{}]: has_feeder_link requires gateway_location or ta_info to be set.\n", i, j);
        valid = false;
      }
    }
  }
  return valid;
}

/// Validates the ncells list of an NTN cell config. Valid for both NTN-band and TN-band serving cells, since
/// neighbor cells may be configured regardless of the serving cell's own band. Returns true on success, otherwise
/// false.
static bool validate_ntn_neighbor_cells(const du_high_unit_cell_ntn_config& ntn_cfg)
{
  bool valid = true;
  for (unsigned j = 0, e = ntn_cfg.ncells.size(); j != e; ++j) {
    const auto& ncell = ntn_cfg.ncells[j];
    if (ncell.sat_ref.satellite_idx) {
      if (ncell.sat_ref.epoch_timestamp || ncell.sat_ref.ephemeris_info || ncell.sat_ref.gateway_location ||
          ncell.sat_ref.ta_info) {
        fmt::print("ntn.ncells[{}]: satellite_idx is mutually exclusive with inline ephemeris fields.\n", j);
        valid = false;
      }
    } else {
      if (!ncell.sat_ref.epoch_timestamp || !ncell.sat_ref.ephemeris_info) {
        fmt::print("ntn.ncells[{}]: either satellite_idx or inline ephemeris definition (epoch_timestamp and "
                   "ephemeris_info) must be provided.\n",
                   j);
        valid = false;
      }
    }
  }
  return valid;
}

/// \brief Validates that the SR retransmissions of a UE in an NTN cell outlast the round trip of the uplink grant.
/// \param sr_cfg SR configuration of the cell.
/// \param sr_period_msec Period of the SR opportunities.
/// \param koffset \c cell_specific_koffset, i.e. the round trip.
static bool
validate_ntn_sr_config(const mac_sr_unit_config& sr_cfg, float sr_period_msec, std::chrono::milliseconds koffset)
{
  // The UE retransmits at the first SR opportunity after sr-ProhibitTimer expires (TS 38.321, Section 5.4.4). An absent
  // sr-ProhibitTimer means it retransmits at every SR opportunity.
  const float sr_prohibit_msec    = static_cast<float>(sr_cfg.sr_prohibit_timer.value_or(0));
  const float sr_retx_period_msec = sr_period_msec * std::max(1.0F, std::ceil(sr_prohibit_msec / sr_period_msec));
  const float sr_budget_msec      = sr_retx_period_msec * static_cast<float>(sr_cfg.sr_trans_max);
  const float koffset_msec        = static_cast<float>(koffset.count());
  // SR transmissions spent while the grant is in flight, the first one included.
  const unsigned sr_tx_in_flight = static_cast<unsigned>(std::floor(koffset_msec / sr_retx_period_msec)) + 1U;

  if (sr_budget_msec <= koffset_msec) {
    fmt::print("Warning: mac_cell_group.sr_cfg.sr_trans_max={} SR transmissions, one every {}ms "
               "(pucch.sr_period_ms={}, mac_cell_group.sr_cfg.sr_prohibit_timer={}), last {}ms, less than "
               "ntn.cell_specific_koffset={}ms, so the UE may fall back to random access before the grant arrives. "
               "Increase pucch.sr_period_ms, mac_cell_group.sr_cfg.sr_prohibit_timer or "
               "mac_cell_group.sr_cfg.sr_trans_max.\n",
               sr_cfg.sr_trans_max,
               sr_retx_period_msec,
               sr_period_msec,
               sr_prohibit_msec,
               sr_budget_msec,
               koffset.count());
  } else if (2 * sr_tx_in_flight > sr_cfg.sr_trans_max) {
    fmt::print("An SR uses {} of the mac_cell_group.sr_cfg.sr_trans_max={} transmissions, one every {}ms "
               "(pucch.sr_period_ms={}, mac_cell_group.sr_cfg.sr_prohibit_timer={}), within "
               "ntn.cell_specific_koffset={}ms, more than half. Increase pucch.sr_period_ms, "
               "mac_cell_group.sr_cfg.sr_prohibit_timer or mac_cell_group.sr_cfg.sr_trans_max.\n",
               sr_tx_in_flight,
               sr_cfg.sr_trans_max,
               sr_retx_period_msec,
               sr_period_msec,
               sr_prohibit_msec,
               koffset.count());
    return false;
  }

  return true;
}

/// Validates the NTN config of a cell in an NTN band. Returns true on success, otherwise false.
static bool validate_ntn_config(const du_high_unit_base_cell_config& cell_cfg, nr_band band)
{
  const du_high_unit_cell_ntn_config& ntn_cfg = *cell_cfg.ntn_cfg;

  if (!ntn_cfg.serving) {
    fmt::print("ntn: NTN parameters must be set for a cell in NTN band {}.\n", fmt::underlying(band));
    return false;
  }

  bool valid = true;

  if (ntn_cfg.serving->cell_specific_koffset.count() == 0) {
    fmt::print("ntn.cell_specific_koffset is required for NTN band {}.\n", fmt::underlying(band));
    valid = false;
  }
  const auto& serving = *ntn_cfg.serving;

  if (!serving.sat_ref.satellite_idx) {
    // No global satellite reference: require inline ephemeris.
    if (!serving.sat_ref.epoch_timestamp || !serving.sat_ref.ephemeris_info) {
      fmt::print("ntn: either satellite_idx or inline ephemeris definition (epoch_timestamp and ephemeris_info) must "
                 "be provided for NTN band {}.\n",
                 fmt::underlying(band));
      valid = false;
    }
  } else {
    // satellite_idx set: forbid inline satellite data.
    if (serving.sat_ref.epoch_timestamp) {
      fmt::print("ntn: satellite_idx and epoch_timestamp are mutually exclusive.\n");
      valid = false;
    }
    if (serving.sat_ref.gateway_location) {
      fmt::print("ntn: satellite_idx and gateway_location are mutually exclusive.\n");
      valid = false;
    }
    if (serving.sat_ref.ta_info) {
      fmt::print("ntn: satellite_idx and ta_info are mutually exclusive.\n");
      valid = false;
    }
  }

  if (serving.sat_ref.gateway_location and serving.sat_ref.ta_info) {
    fmt::print("ntn_gateway_location and ta_info are mutually exclusive. When ntn_gateway_location is set, "
               "TA-Common/Drift/DriftVariation are computed automatically from the satellite ephemeris and gateway "
               "position. ta_info is only required when ntn_gateway_location is absent, to force fixed "
               "TA-Common/Drift/DriftVariation values.\n");
    valid = false;
  }

  if (not serving.sat_ref.satellite_idx and serving.feeder_link_info and not serving.sat_ref.gateway_location and
      not serving.sat_ref.ta_info) {
    fmt::print("feeder_link requires gateway_location or ta_info to be set.\n");
    valid = false;
  }

  if (serving.ta_report_sr_enabled and not serving.ta_report_offset_threshold.has_value()) {
    fmt::print("ntn.ta_report_sr_enabled requires ntn.ta_report_offset_threshold to be set.\n");
    valid = false;
  }

  if (not validate_ntn_sr_config(
          cell_cfg.mcg_cfg.sr_cfg, cell_cfg.pucch_cfg.sr_period_msec, serving.cell_specific_koffset)) {
    valid = false;
  }

  if (serving.ue_specific_koffset_enabled) {
    if (not serving.ta_report_offset_threshold.has_value()) {
      fmt::print("ntn.ue_specific_koffset_enabled requires ntn.ta_report_offset_threshold to be set.\n");
      valid = false;
    }
    // The HARQ-ACK of its transport block is what tells the scheduler a UE applies a Differential Koffset, and a
    // HARQ process with feedback disabled never sends one. The UE would apply a value the scheduler drops.
    if (cell_cfg.pdsch_cfg.harq_feedback_disabled.any()) {
      fmt::print("ntn.ue_specific_koffset_enabled cannot be combined with pdsch.harq_feedback_disabled.\n");
      valid = false;
    }
    // Mode B reuses an uplink HARQ process before the round trip of its previous transmission has elapsed, and a
    // Differential Koffset moves the uplink of a UE earlier still.
    if (cell_cfg.pusch_cfg.harq_mode_b.any()) {
      fmt::print("ntn.ue_specific_koffset_enabled cannot be combined with pusch.harq_mode_b.\n");
      valid = false;
    }
    if (serving.ue_specific_koffset_margin >= serving.cell_specific_koffset) {
      fmt::print("ntn.ue_specific_koffset_margin must be smaller than ntn.cell_specific_koffset.\n");
      valid = false;
    }
    // T_TA grows by up to the reporting threshold before the UE reports again, and the margin has to cover it.
    if (serving.ta_report_offset_threshold.has_value() and
        static_cast<float>(serving.ue_specific_koffset_margin.count()) < serving.ta_report_offset_threshold.value()) {
      fmt::print("ntn.ue_specific_koffset_margin must be at least ntn.ta_report_offset_threshold.\n");
      valid = false;
    }
  }

  if (serving.sat_switch_with_resync) {
    const auto& sw = *serving.sat_switch_with_resync;
    if (!sw.t_service_start.has_value()) {
      // Per TS 38.331, t-ServiceStart has no documented behavior for absence, and it's the only field in
      // SatSwitchWithReSync that tells the UE *when* to use the target satellite's parameters.
      fmt::print("sat_switch_with_resync: t_service_start is required.\n");
      valid = false;
    }
    if (!serving.t_service.has_value()) {
      // Per TS 38.331 clause 5.7.19, the UE only initiates satellite switch with resynchronization when both
      // SatSwitchWithReSync and t-Service are included in SIB19; t-Service is also the instant the switch executes.
      fmt::print("sat_switch_with_resync: t_service is required in the serving cell NTN config.\n");
      valid = false;
    }
    if ((sw.cell_specific_koffset && *sw.cell_specific_koffset != serving.cell_specific_koffset) ||
        (sw.k_mac && sw.k_mac != serving.k_mac)) {
      // The gNB cannot reconfigure the scheduling offsets at runtime, so they cannot change across a satellite
      // switch.
      fmt::print("sat_switch_with_resync: cell_specific_koffset and k_mac must match the serving cell values; "
                 "changing them across a satellite switch is not supported.\n");
      valid = false;
    }
    if (sw.promote_neighbors && !sw.promote_to_serving) {
      fmt::print("sat_switch_with_resync: promote_neighbors has no effect unless promote_to_serving is enabled.\n");
      valid = false;
    }
    if (sw.sat_ref.satellite_idx) {
      if (sw.sat_ref.epoch_timestamp) {
        fmt::print("sat_switch_with_resync: satellite_idx and epoch_timestamp are mutually exclusive.\n");
        valid = false;
      }
      if (sw.sat_ref.gateway_location) {
        fmt::print("sat_switch_with_resync: satellite_idx and gateway_location are mutually exclusive.\n");
        valid = false;
      }
      if (sw.sat_ref.ta_info) {
        fmt::print("sat_switch_with_resync: satellite_idx and ta_info are mutually exclusive.\n");
        valid = false;
      }
      if (sw.sat_ref.ephemeris_info) {
        fmt::print("sat_switch_with_resync: satellite_idx and ephemeris_info are mutually exclusive.\n");
        valid = false;
      }
    } else {
      if (!sw.sat_ref.epoch_timestamp) {
        fmt::print("sat_switch_with_resync: epoch_timestamp is required when satellite_idx is not set.\n");
        valid = false;
      }
      if (!sw.sat_ref.ephemeris_info) {
        fmt::print("sat_switch_with_resync: ephemeris_info is required when satellite_idx is not set.\n");
        valid = false;
      }
    }
  }

  if (!validate_ntn_neighbor_cells(ntn_cfg)) {
    valid = false;
  }

  return valid;
}

/// Validates the given PUCCH cell application configuration. Returns true on success, otherwise false.
static bool validate_pucch_cell_unit_config(const du_high_unit_base_cell_config&                config,
                                            subcarrier_spacing                                  scs_common,
                                            unsigned                                            nof_crbs,
                                            const std::optional<du_high_unit_tdd_ul_dl_config>& tdd_cfg)
{
  const du_high_unit_pucch_config& pucch_cfg = config.pucch_cfg;
  const du_high_unit_csi_config&   csi_cfg   = config.csi_cfg;
  if (csi_cfg.csi_rs_enabled) {
    if (csi_cfg.report_type == csi_report_type::periodic and pucch_cfg.nof_cell_csi_resources == 0) {
      fmt::print("Number of PUCCH Format 2/3/4 cell resources for CSI must be greater than 0 when CSI is enabled and "
                 "CSI report type is periodic.\n");
      return false;
    }
    if (csi_cfg.report_type == csi_report_type::aperiodic and pucch_cfg.nof_cell_csi_resources > 0) {
      fmt::print("Number of PUCCH Format 2/3/4 cell resources for CSI must be zero when CSI is enabled and CSI "
                 "report type is aperiodic.\n");
      return false;
    }
  } else {
    if (pucch_cfg.nof_cell_csi_resources > 0) {
      fmt::print("Number of PUCCH Format 2/3/4 cell resources for CSI must be zero when CSI is disabled.\n");
      return false;
    }
  }

  // See \c periodicityAndOffset in \c SchedulingRequestResourceConfig of TS 38.331.
  static const std::map<unsigned, std::vector<unsigned>> mu_to_valid_sr_period_slots_lookup{
      {0, {1, 2, 4, 5, 8, 10, 16, 20, 40, 80}},
      {1, {1, 2, 4, 8, 10, 16, 20, 40, 80, 160}},
      {2, {1, 2, 4, 8, 16, 20, 40, 80, 160, 320}},
      {3, {1, 2, 4, 8, 16, 40, 80, 160, 320, 640}},
  };
  const auto sr_period_slots = static_cast<unsigned>(get_nof_slots_per_subframe(scs_common) * pucch_cfg.sr_period_msec);

  // Check that the SR period in milliseconds leads to an integer number of slots.
  if (get_nof_slots_per_subframe(scs_common) * pucch_cfg.sr_period_msec != static_cast<float>(sr_period_slots)) {
    fmt::print("SR period (i.e., {}ms) times the number of slots per subframe (i.e., {}) must be an integer number of "
               "slots.\n",
               pucch_cfg.sr_period_msec,
               get_nof_slots_per_subframe(scs_common));
    return false;
  }
  if (tdd_cfg.has_value()) {
    const unsigned tdd_period_slots =
        tdd_cfg.value().pattern1.dl_ul_period_slots +
        (tdd_cfg.value().pattern2.has_value() ? tdd_cfg.value().pattern2.value().dl_ul_period_slots : 0U);
    if (sr_period_slots < tdd_period_slots) {
      fmt::print("In TDD mode, the SR period in slots (i.e., {}ms equivalent to {} slots) must be larger than or equal "
                 "to the TDD period measured in slots\n",
                 pucch_cfg.sr_period_msec,
                 sr_period_slots);
      return false;
    }
  }
  span<const unsigned> valid_sr_period_slots = mu_to_valid_sr_period_slots_lookup.at(to_numerology_value(scs_common));
  if (std::find(valid_sr_period_slots.begin(), valid_sr_period_slots.end(), sr_period_slots) ==
      valid_sr_period_slots.end()) {
    fmt::print("SR period of {}ms (i.e. {} slots) is not valid for {}kHz SCS.\n",
               pucch_cfg.sr_period_msec,
               sr_period_slots,
               scs_to_khz(scs_common));
    return false;
  }

  if (!pucch_cfg.repetition_sinr_thresholds.empty()) {
    if (pucch_cfg.repetition_sinr_thresholds.size() > 3) {
      fmt::print("Too many PUCCH repetition SINR thresholds defined (maximum is 3 to allow repetition factor 8).\n");
      return false;
    }
    if (pucch_cfg.repetition_factors.size() != pucch_cfg.res_set_size) {
      fmt::print("With PUCCH repetition enabled, the provided number of repetition factors ({}) must equal the "
                 "resource set size ({}).\n",
                 pucch_cfg.repetition_factors.size(),
                 pucch_cfg.res_set_size);
      return false;
    }
    const unsigned max_factor =
        *std::max_element(pucch_cfg.repetition_factors.begin(), pucch_cfg.repetition_factors.end());
    // Enabling repetition factor 2 requires 1 threshold, 4 requires 2, and 8 requires 3.
    const unsigned required_thresholds = log2_ceil(max_factor);
    if (pucch_cfg.repetition_sinr_thresholds.size() < required_thresholds) {
      fmt::print(
          "Enabling PUCCH repetition factor {} requires {} PUCCH repetition SINR thresholds (only {} provided).\n",
          max_factor,
          required_thresholds,
          pucch_cfg.repetition_sinr_thresholds.size());
      return false;
    }
  }

  // We need to count pucch_cfg.nof_ue_pucch_res_harq_per_set twice, as we have 2 sets of PUCCH resources for HARQ-ACK
  // (PUCCH Resource Set Id 0 with Format 0/1 and PUCCH Resource Set Id 1 with Format 2/3/4).
  if (pucch_cfg.res_set_size * 2U * pucch_cfg.nof_cell_res_set_configs + pucch_cfg.nof_cell_sr_resources +
          pucch_cfg.nof_cell_csi_resources >
      pucch_constants::MAX_NOF_CELL_DED_RESOURCES) {
    fmt::print("With the given PUCCH parameters, the number of PUCCH resources per cell exceeds the limit={}.\n",
               pucch_constants::MAX_NOF_CELL_DED_RESOURCES);
    return false;
  }

  // [Implementation defined] The scheduler expects the resources from the common resource set and Resource Set 0 to
  // use the same format. The formats from the common resource sets are expressed in TS 38.213 Table 9.2.1-1.
  if (pucch_cfg.pucch_resource_common.has_value()) {
    if (pucch_f0f1_format(pucch_cfg.formats) == pucch_format::FORMAT_0 and
        pucch_cfg.pucch_resource_common.value() > 2) {
      fmt::print("When using PUCCH Format 0, the valid values for pucch_resource_common are {{0, 1, 2}}.\n");
      return false;
    }
    if (pucch_f0f1_format(pucch_cfg.formats) == pucch_format::FORMAT_1 and
        pucch_cfg.pucch_resource_common.value() <= 2) {
      fmt::print("When using PUCCH Format 1, the valid values for pucch_resource_common are {{3, ..., 15}}.\n");
      return false;
    }
  }

  // The number of symbols reserved for PUCCH depends on whether the GNB uses (periodic) Sounding Reference Signals
  // (SRS).
  const unsigned max_nof_srs_symbols =
      config.srs_cfg.srs_type_enabled == "periodic" or config.srs_cfg.srs_type_enabled == "aperiodic"
          ? config.srs_cfg.max_nof_symbols_per_slot
          : 0U;
  const unsigned max_nof_pucch_symbols = NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - max_nof_srs_symbols;
  unsigned       nof_f0_f1_rbs         = 0U;
  if (pucch_f0f1_format(pucch_cfg.formats) == pucch_format::FORMAT_0) {
    // The number of symbols per PUCCH resource F0 is not exposed to the DU user interface and set by default to 2.
    constexpr unsigned pucch_f0_nof_symbols = 2U;
    // We define a block as a set of Resources (either F0/F1 or F2) aligned over the same starting PRB.
    const unsigned nof_f0_per_block = max_nof_pucch_symbols / pucch_f0_nof_symbols;
    // Each PUCCH resource F0/F1 occupies 1 RB (per block).
    nof_f0_f1_rbs =
        static_cast<unsigned>(std::ceil(static_cast<float>(pucch_cfg.res_set_size * pucch_cfg.nof_cell_res_set_configs +
                                                           pucch_cfg.nof_cell_sr_resources) /
                                        static_cast<float>(nof_f0_per_block)));
    // With intraslot_freq_hopping, the nof of RBs is an even number.
    if (pucch_cfg.f0_intraslot_freq_hopping) {
      nof_f0_f1_rbs = static_cast<unsigned>(std::ceil(static_cast<float>(nof_f0_f1_rbs) / 2.0F)) * 2;
    }
  } else {
    // The number of symbols per PUCCH resource is not exposed to the DU user interface; for PUCCH F1, we use all
    // symbols available for PUCCH within a slot.
    const unsigned pucch_f1_nof_symbols = max_nof_pucch_symbols;
    const unsigned nof_occ_codes =
        pucch_cfg.f1_enable_occ ? format1_nof_td_occs(pucch_f1_nof_symbols, pucch_cfg.f1_intraslot_freq_hopping) : 1U;

    // We define a block as a set of Resources (either F0/F1 or F2) aligned over the same starting PRB.
    const unsigned nof_f1_per_block = nof_occ_codes * pucch_cfg.f1_nof_cyclic_shifts;
    // Each PUCCH resource F0/F1 occupies 1 RB (per block).
    nof_f0_f1_rbs =
        static_cast<unsigned>(std::ceil(static_cast<float>(pucch_cfg.res_set_size * pucch_cfg.nof_cell_res_set_configs +
                                                           pucch_cfg.nof_cell_sr_resources) /
                                        static_cast<float>(nof_f1_per_block)));
    // With intraslot_freq_hopping, the nof of RBs is an even number.
    if (pucch_cfg.f1_intraslot_freq_hopping) {
      nof_f0_f1_rbs = static_cast<unsigned>(std::ceil(static_cast<float>(nof_f0_f1_rbs) / 2.0F)) * 2;
    }
  }

  unsigned       nof_f2_f3_f4_rbs;
  const unsigned nof_res_f2_f3_f4 =
      pucch_cfg.res_set_size * pucch_cfg.nof_cell_res_set_configs + pucch_cfg.nof_cell_csi_resources;
  unsigned f2_f3_f4_max_payload = 0U;
  switch (pucch_f2f3f4_format(pucch_cfg.formats)) {
    case pucch_format::FORMAT_2: {
      // The number of symbols per PUCCH resource F2 is not exposed to the DU user interface and set by default to 2.
      constexpr unsigned pucch_f2_nof_symbols = 2U;
      const unsigned     f2_max_rbs           = pucch_cfg.f2_max_payload_bits.has_value()
                                                    ? get_pucch_format2_max_nof_prbs(pucch_cfg.f2_max_payload_bits.value(),
                                                                       pucch_f2_nof_symbols,
                                                                       to_float(pucch_cfg.f2_max_code_rate))
                                                    : pucch_cfg.f2_max_nof_rbs;

      const unsigned nof_f2_blocks = max_nof_pucch_symbols / pucch_f2_nof_symbols;
      nof_f2_f3_f4_rbs =
          static_cast<unsigned>(std::ceil(static_cast<float>(nof_res_f2_f3_f4) / static_cast<float>(nof_f2_blocks))) *
          f2_max_rbs;
      // With intraslot_freq_hopping, the nof of RBs is an even number of the PUCCH resource size in RB.
      if (pucch_cfg.f2_intraslot_freq_hopping) {
        nof_f2_f3_f4_rbs = static_cast<unsigned>(std::ceil(static_cast<float>(nof_f2_f3_f4_rbs) / 2.0F)) * 2;
      }
      f2_f3_f4_max_payload =
          get_pucch_format2_max_payload(f2_max_rbs, pucch_f2_nof_symbols, to_float(pucch_cfg.f2_max_code_rate));
    } break;
    case pucch_format::FORMAT_3: {
      // The number of symbols per PUCCH resource is not exposed to the DU user interface; for PUCCH F3, we use all
      // symbols available for PUCCH within a slot.
      const unsigned pucch_f3_nof_symbols = max_nof_pucch_symbols;
      const unsigned f3_max_rbs =
          pucch_cfg.f3_max_payload_bits.has_value()
              ? get_pucch_format3_max_nof_prbs(pucch_cfg.f3_max_payload_bits.value(),
                                               pucch_f3_nof_symbols,
                                               to_float(pucch_cfg.f3_max_code_rate),
                                               // Since we are forcing 14 symbols intraslot_freq_hopping doesn't matter.
                                               false,
                                               pucch_cfg.f3_additional_dmrs,
                                               pucch_cfg.f3_pi2_bpsk)
              : pucch_cfg.f3_max_nof_rbs;

      const unsigned nof_f3_blocks = max_nof_pucch_symbols / pucch_f3_nof_symbols;
      nof_f2_f3_f4_rbs =
          static_cast<unsigned>(std::ceil(static_cast<float>(nof_res_f2_f3_f4) / static_cast<float>(nof_f3_blocks))) *
          f3_max_rbs;
      // With intraslot_freq_hopping, the nof of RBs is an even number of the PUCCH resource size in RB.
      if (pucch_cfg.f3_intraslot_freq_hopping) {
        nof_f2_f3_f4_rbs = static_cast<unsigned>(std::ceil(static_cast<float>(nof_f2_f3_f4_rbs) / 2.0F)) * 2;
      }
      f2_f3_f4_max_payload = get_pucch_format3_max_payload(f3_max_rbs,
                                                           pucch_f3_nof_symbols,
                                                           to_float(pucch_cfg.f3_max_code_rate),
                                                           false,
                                                           pucch_cfg.f3_additional_dmrs,
                                                           pucch_cfg.f3_pi2_bpsk);
    } break;
    case pucch_format::FORMAT_4: {
      // The number of symbols per PUCCH resource is not exposed to the DU user interface; for PUCCH F4, we use all
      // symbols available for PUCCH within a slot.
      const unsigned pucch_f4_nof_symbols = max_nof_pucch_symbols;
      const unsigned nof_f4_blocks        = max_nof_pucch_symbols / pucch_f4_nof_symbols;
      nof_f2_f3_f4_rbs =
          static_cast<unsigned>(std::ceil(static_cast<float>(nof_res_f2_f3_f4) / static_cast<float>(nof_f4_blocks)));
      // With intraslot_freq_hopping, the nof of RBs is an even number of the PUCCH resource size in RB.
      if (pucch_cfg.f4_intraslot_freq_hopping) {
        nof_f2_f3_f4_rbs = static_cast<unsigned>(std::ceil(static_cast<float>(nof_f2_f3_f4_rbs) / 2.0F)) * 2;
      }
      f2_f3_f4_max_payload = get_pucch_format4_max_payload(pucch_f4_nof_symbols,
                                                           to_float(pucch_cfg.f4_max_code_rate),
                                                           false,
                                                           pucch_cfg.f4_additional_dmrs,
                                                           pucch_cfg.f4_pi2_bpsk,
                                                           static_cast<pucch_f4_occ_len>(pucch_cfg.f4_occ_length));
    } break;
    default:
      fmt::print("Invalid PUCCH format for Set Id 1.\n");
      return false;
  }

  // Make sure the PUCCH has the minimum required payload for the given number of antennas.
  if (config.nof_antennas_dl == 1 and f2_f3_f4_max_payload < 4U) {
    fmt::print("With the given parameters and 1 DL antenna, PUCCH F2/3/4 max payload must be at least 4 bits.\n");
    return false;
  }
  if (config.nof_antennas_dl == 2 and f2_f3_f4_max_payload < 7U) {
    fmt::print("With the given parameters and 2 DL antennas, PUCCH F2/3/4 max payload must be at least 7 bits.\n");
    return false;
  }
  if (config.nof_antennas_dl == 4 and f2_f3_f4_max_payload < 11U) {
    fmt::print("With the given parameters and 4 DL antennas, PUCCH F2/3/4 max payload must be at least 11 bits.\n");
    return false;
  }
  if (config.nof_antennas_dl == 8 and f2_f3_f4_max_payload < 13U) {
    fmt::print("With the given parameters and 8 DL antennas, PUCCH F2/3/4 max payload must be at least 13 bits.\n");
    return false;
  }

  // Verify the number of RBs for the PUCCH resources does not exceed the BWP size.
  // [Implementation-defined] We do not allow the PUCCH resources to occupy more than 50% of the BWP. This is an
  // extreme case, and ideally the PUCCH configuration should result in a much lower PRBs usage. NOTE: for 5MHz BW or
  // for TDD and 10MHz BW, the default PUCCH config will be overwritten to force it to pass this check; skip this
  // check here.
  const bool def_cfg_for_narrow_bw =
      (config.channel_bw_mhz < bs_channel_bandwidth::MHz10 or
       (config.tdd_ul_dl_cfg.has_value() and config.channel_bw_mhz <= bs_channel_bandwidth::MHz10)) and
      pucch_cfg == du_high_unit_pucch_config{};
  if (not def_cfg_for_narrow_bw) {
    constexpr float max_allowed_prbs_usage = 0.5F;
    if (static_cast<float>(nof_f0_f1_rbs + nof_f2_f3_f4_rbs) / static_cast<float>(nof_crbs) >= max_allowed_prbs_usage) {
      fmt::print("With the given parameters, the number of PRBs for PUCCH exceeds the 50% of the BWP PRBs.\n");
      return false;
    }
  }

  return true;
}

static bool validate_srs_cell_unit_config(const du_high_unit_srs_config&                      config,
                                          subcarrier_spacing                                  scs_common,
                                          unsigned                                            nof_crbs,
                                          unsigned                                            nof_ul_ports,
                                          const std::optional<du_high_unit_tdd_ul_dl_config>& tdd_cfg)
{
  if (config.srs_type_enabled == "periodic" or config.srs_type_enabled == "aperiodic") {
    const auto srs_period_slots =
        static_cast<unsigned>(get_nof_slots_per_subframe(scs_common) * config.srs_period_prohibit_time_ms);

    // Check that the SR period in milliseconds leads to an integer number of slots.
    if (static_cast<float>(get_nof_slots_per_subframe(scs_common) * config.srs_period_prohibit_time_ms) !=
        static_cast<float>(srs_period_slots)) {
      fmt::print(
          "SRS period (i.e., {}ms) times the number of slots per subframe (i.e., {}) must be an integer number of "
          "slots\n",
          config.srs_period_prohibit_time_ms,
          get_nof_slots_per_subframe(scs_common));
      return false;
    }

    if (tdd_cfg.has_value()) {
      const unsigned tdd_period_slots =
          tdd_cfg.value().pattern1.dl_ul_period_slots +
          (tdd_cfg.value().pattern2.has_value() ? tdd_cfg.value().pattern2.value().dl_ul_period_slots : 0U);
      if (srs_period_slots < tdd_period_slots) {
        fmt::print("In TDD mode, the SRS period/prohibit time in slots (i.e., {}ms equivalent to {} slots) must be "
                   "larger than or equal to the TDD period measured in slots\n",
                   config.srs_period_prohibit_time_ms,
                   srs_period_slots);
        return false;
      }
    }

    if (scs_common == ocudu::subcarrier_spacing::kHz30 and config.srs_period_prohibit_time_ms > 1280) {
      fmt::print("With 30kHz SCS the maximum SRS period is 1280ms\n");
      return false;
    }

    if (config.srs_type_enabled == "aperiodic" and config.srs_period_prohibit_time_ms < 20.0) {
      fmt::print("For aperiodic SRS, the minimum period is set to 20ms\n");
      return false;
    }
  }

  if (config.nof_symbols > config.max_nof_symbols_per_slot) {
    fmt::print("The number of symbols per SRS resource ({}) should be less than or equal to the maximum number of SRS "
               "symbols per slot ({})\n",
               config.nof_symbols,
               config.max_nof_symbols_per_slot);
    return false;
  }

  // TODO: revise this when GNB fully supports BWP starting from CRB > 0.
  if (config.freq_domain_shift >= nof_crbs) {
    fmt::print("SRS frequency shift ({}) is larger than the BPW BW in RBs ({})\n", config.freq_domain_shift, nof_crbs);
    return false;
  }

  if (config.tx_comb == 2U) {
    if (nof_ul_ports == 2 and (config.cyclic_shift_reuse_factor == 3 or config.cyclic_shift_reuse_factor == 6)) {
      fmt::print("With TX comb n2 and 2 UL antenna ports, valid cyclic-reuse-factor values are 1, 2, 4\n",
                 config.nof_symbols,
                 config.max_nof_symbols_per_slot);
      return false;
    }
    if (nof_ul_ports == 4 and (config.cyclic_shift_reuse_factor == 3 or config.cyclic_shift_reuse_factor == 4 or
                               config.cyclic_shift_reuse_factor == 6)) {
      fmt::print("With TX comb n2 and 4 UL antenna ports, valid cyclic-reuse-factor values are 1, 2\n",
                 config.nof_symbols,
                 config.max_nof_symbols_per_slot);
      return false;
    }
  } else {
    if (nof_ul_ports == 4 and (config.cyclic_shift_reuse_factor == 2 or config.cyclic_shift_reuse_factor == 4 or
                               config.cyclic_shift_reuse_factor == 6)) {
      fmt::print("With TX comb n4 and 4 UL antenna ports, valid cyclic-reuse-factor values are 1, 3\n",
                 config.nof_symbols,
                 config.max_nof_symbols_per_slot);
      return false;
    }
  }

  return true;
}

/// Validates the given PUCCH cell application configuration. Returns true on success, otherwise false.
static bool validate_ul_common_unit_config(const du_high_unit_ul_common_config& config, unsigned nof_crbs)
{
  if (config.max_ul_grants_per_slot <= config.max_pucchs_per_slot) {
    fmt::print("The max number of UL grants per slot should be greater than the maximum number of PUCCH grants.\n");
    return false;
  }

  if (config.min_pucch_pusch_prb_distance >= nof_crbs / 2) {
    fmt::print("The minimum distance between PUCCH and PUSCH PRBs ({}) should be less than half of the BWP size "
               "({}/2 = {}).\n",
               config.min_pucch_pusch_prb_distance,
               nof_crbs,
               nof_crbs / 2);
    return false;
  }

  return true;
}

/// Validates that the given ports are not duplicated. Returns true on success, otherwise false.
template <typename T>
[[gnu::noinline]] static bool validate_duplicated_ports(span<const T> ports)
{
  std::vector<T> temp_ports(ports.begin(), ports.end());
  std::sort(temp_ports.begin(), temp_ports.end());

  return std::unique(temp_ports.begin(), temp_ports.end()) == temp_ports.end();
}

/// Validates the given PRACH cell application configuration. Returns true on success, otherwise false.
static bool
validate_prach_cell_unit_config(const du_high_unit_rach_config& config, nr_band band, unsigned nof_rx_antennas)
{
  ocudu_assert(config.prach_config_index.has_value(), "The PRACH configuration index must be set.");

  frequency_range freq_range = band_helper::get_freq_range(band);
  duplex_mode     dplx_mode  = band_helper::get_duplex_mode(band);

  auto code = prach_helper::prach_config_index_is_valid(config.prach_config_index.value(), freq_range, dplx_mode);
  if (not code.has_value()) {
    fmt::print("{}", code.error());
    return false;
  }

  const prach_configuration prach_config =
      prach_configuration_get(freq_range, dplx_mode, config.prach_config_index.value());
  code = prach_helper::prach_root_sequence_index_is_valid(config.prach_root_sequence_index, prach_config.format);
  if (not code.has_value()) {
    fmt::print("{}", code.error());
    return false;
  }

  code = prach_helper::zero_correlation_zone_is_valid(
      config.zero_correlation_zone, config.prach_config_index.value(), freq_range, dplx_mode);
  if (not code.has_value()) {
    fmt::print("{}", code.error());
    return false;
  }

  code = prach_helper::nof_ssb_per_ro_and_nof_cb_preambles_per_ssb_is_valid(config.nof_ssb_per_ro,
                                                                            config.nof_cb_preambles_per_ssb);
  if (not code.has_value()) {
    fmt::print("{}", code.error());
    return false;
  }

  if (config.cfra_enabled and config.total_nof_ra_preambles == config.nof_cb_preambles_per_ssb) {
    fmt::print("Total nof. RA preambles per occasion ({}) used for Contention-based Random Access should be lower than "
               "{}, if Contention-free Random Access is enabled.\n",
               config.nof_cb_preambles_per_ssb,
               config.total_nof_ra_preambles);
    return false;
  }

  // See TS 38.331, ssb-perRACH-OccasionAndCB-PreamblesPerSSB and totalNumberOfRA-Preambles.
  // totalNumberOfRA-Preambles should be a multiple of the number of SSBs per RACH occasion.
  if (config.nof_ssb_per_ro >= ssb_per_rach_occasions::one) {
    if (config.total_nof_ra_preambles % static_cast<uint8_t>(ssb_per_rach_occ_to_float(config.nof_ssb_per_ro)) != 0) {
      fmt::print("Total nof. RA preambles ({}) should be a multiple of the number of SSBs per RACH occasion ({}).\n",
                 config.total_nof_ra_preambles,
                 ssb_per_rach_occ_to_float(config.nof_ssb_per_ro));
      return false;
    }
    // Ensure \c config.total_nof_ra_preambles can accommodate contention based RA preambles.
    // NOTE: \c config.total_nof_ra_preambles nof. RA preambles are shared among \c config.nof_ssb_per_ro nof. SSB
    // beams.
    if (config.nof_cb_preambles_per_ssb * static_cast<unsigned>(ssb_per_rach_occ_to_float(config.nof_ssb_per_ro)) >
        config.total_nof_ra_preambles) {
      fmt::print("Total nof. of contention based RA preambles ({} preambles/SSB x {} SSBs/RO = {}) exceeds total nof. "
                 "RA preambles ({}).\n",
                 config.nof_cb_preambles_per_ssb,
                 ssb_per_rach_occ_to_float(config.nof_ssb_per_ro),
                 config.nof_cb_preambles_per_ssb *
                     static_cast<unsigned>(ssb_per_rach_occ_to_float(config.nof_ssb_per_ro)),
                 config.total_nof_ra_preambles);
      return false;
    }
  } else {
    // Number of SSBs per RACH occasion is 1/8 or 1/4 or 1/2.
    const auto product =
        static_cast<float>(config.total_nof_ra_preambles) * ssb_per_rach_occ_to_float(config.nof_ssb_per_ro);
    if ((product - static_cast<uint8_t>(product)) > 0) {
      fmt::print("Total nof. RA preambles ({}) should be a multiple of the number of SSBs per RACH occasion ({}).\n",
                 config.total_nof_ra_preambles,
                 ssb_per_rach_occ_to_float(config.nof_ssb_per_ro));
      return false;
    }
    // Ensure \c config.total_nof_ra_preambles can accommodate contention based RA preambles.
    // NOTE: Each SSB beam has multiple RACH occasions and each occasion has \c config.total_nof_ra_preambles RA
    // preambles.
    if (config.nof_cb_preambles_per_ssb > config.total_nof_ra_preambles) {
      fmt::print("Total nof. of contention based RA preambles per SSB ({}) exceeds total nof. RA preambles ({}).\n",
                 config.nof_cb_preambles_per_ssb,
                 config.total_nof_ra_preambles);
      return false;
    }
  }

  if (config.ports.size() > nof_rx_antennas) {
    fmt::print("PRACH number of ports ({}) is bigger than the number of reception antennas ({}).\n",
               config.ports.size(),
               nof_rx_antennas);

    return false;
  }

  if (!validate_duplicated_ports<uint8_t>(config.ports)) {
    fmt::print("Detected duplicated PRACH port cell identifiers\n");
    return false;
  }

  for (auto port_id : config.ports) {
    if (port_id >= nof_rx_antennas) {
      fmt::print("PRACH port id '{}' out of range. Valid range {}-{}.\n", port_id, 0, nof_rx_antennas - 1);
      return false;
    }
  }

  if (config.two_step.has_value()) {
    const auto& ts = *config.two_step;
    if (ts.cb_preambles_per_ssb_per_shared_ro >= config.nof_cb_preambles_per_ssb) {
      fmt::print(
          "Two-step RACH: cb_preambles_per_ssb_per_shared_ro ({}) must be less than nof_cb_preambles_per_ssb ({}).\n",
          ts.cb_preambles_per_ssb_per_shared_ro,
          config.nof_cb_preambles_per_ssb);
      return false;
    }
  }

  if (config.backoff_indicator_max_preambles == 0) {
    fmt::print("backoff_indicator_max_preambles must be greater than 0.\n");
    return false;
  }

  return true;
}

static bool validate_tdd_ul_dl_pattern_unit_config(const tdd_ul_dl_pattern_unit_config& config,
                                                   subcarrier_spacing                   common_scs,
                                                   std::optional<unsigned>              dmrs_add_pos = std::nullopt)
{
  // NOTE: TDD pattern is assumed to use common SCS as reference SCS.
  if (common_scs > subcarrier_spacing::kHz120) {
    fmt::print("Invalid TDD UL DL reference SCS={}kHz. Must be 15, 30 or 60 kHz for FR1 and 120 kHz for FR2.\n",
               scs_to_khz(common_scs));
    return false;
  }

  if (config.nof_ul_slots != 0) {
    // NOTE: 1 of the slots in the TDD pattern is the special slot.
    if (config.nof_dl_slots + config.nof_ul_slots > config.dl_ul_period_slots - 1) {
      fmt::print("Invalid TDD pattern: the sum of DL and UL slots is not compatible with TDD period.\n");
      return false;
    }
  } else {
    // NOTE: If there are only DL slots in the TDD pattern, then the special slot is optional.
    if (config.nof_dl_slots < config.dl_ul_period_slots - 1 or config.nof_dl_slots > config.dl_ul_period_slots) {
      fmt::print("Invalid TDD pattern: the number of DL slots is not compatible with the TDD configuration.\n");
      return false;
    }
  }

  // Extended CP not currently supported: assume 14 symbols per slot; 2 symbols for DL-to-UL switching.
  // As per "5G-NR in Bullets", Table 352, we consider a number of guard symbols that accommodates both SCS 15kHz and
  // 30kHz.
  if (config.nof_dl_symbols + config.nof_ul_symbols > NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - 2U) {
    fmt::print("Invalid TDD pattern: the sum of DL and UL symbols in the special slot should not exceed 12.\n");
    return false;
  }

  const float period_msec =
      static_cast<float>(config.dl_ul_period_slots) / static_cast<float>(get_nof_slots_per_subframe(common_scs));

  static constexpr std::array<float, 10> valid_periods = {0.5F, 0.625, 1.0, 1.25, 2.0, 2.5, 3.0, 4.0, 5.0, 10.0};
  if (std::none_of(valid_periods.begin(), valid_periods.end(), [period_msec](float v) {
        return std::abs(period_msec - v) < 1e-3;
      })) {
    fmt::print(
        "Invalid TDD pattern periodicity={}ms. It must be in {{{}}}.\n", period_msec, fmt::join(valid_periods, ", "));
    return false;
  }

  // This input is provided when the CSI is enabled. If so, we need to make sure that the CSI-RS resources (whose
  // symbols depends on the DMRS position) fit within the given TDD pattern.
  if (dmrs_add_pos.has_value()) {
    // We ensure that DSUUU pattern has enough DL symbols in the Special slot (S) to accommodate the CSI-RS resources.
    // By default, in our configuration, the CSI-RS resources are in symbols {4,8}, or in symbols {6,10} id the
    // dmrs-AdditionalPosition is 3.
    const unsigned max_csi_rs_symbol = dmrs_add_pos.value() != 3U ? 8U : 10U;
    if (config.nof_dl_slots < 2U and config.nof_dl_symbols <= max_csi_rs_symbol) {
      fmt::print("Invalid TDD pattern. Cause: CSI-RS resources do not fit in the grid. When CSI-RS is enabled, the TDD "
                 "pattern must have at least 2 initial DL slots, or, 1 initial DL slot and {} DL symbols in the "
                 "special slot with dmrs-AdditionalPosition={}\n",
                 max_csi_rs_symbol + 1,
                 dmrs_add_pos.value());
      return false;
    }
  }

  return true;
}

/// Validates the given TDD UL DL pattern application configuration. Returns true on success, otherwise false.
static bool validate_tdd_ul_dl_unit_config(const du_high_unit_tdd_ul_dl_config& config,
                                           subcarrier_spacing                   common_scs,
                                           std::optional<unsigned>              dmrs_add_pos = std::nullopt)
{
  if (not validate_tdd_ul_dl_pattern_unit_config(config.pattern1, common_scs, dmrs_add_pos)) {
    return false;
  }
  if (config.pattern2.has_value() and not validate_tdd_ul_dl_pattern_unit_config(config.pattern2.value(), common_scs)) {
    return false;
  }

  const unsigned total_period_slots =
      config.pattern1.dl_ul_period_slots + (config.pattern2.has_value() ? config.pattern2->dl_ul_period_slots : 0);
  const unsigned slots_per_20ms = 20U * get_nof_slots_per_subframe(common_scs);
  // As per TS 38.213, clause 11.1, "A UE expects that P + P2 divides 20 msec".
  if (slots_per_20ms % total_period_slots != 0) {
    const float period_msec =
        static_cast<float>(total_period_slots) / static_cast<float>(get_nof_slots_per_subframe(common_scs));
    fmt::print("Invalid TDD pattern total periodicity={}ms. It must divide 20 msec.\n", period_msec);
    return false;
  }

  return true;
}

/// Validates that the cell has a downlink antenna topology and that its number of ports matches the number of downlink
/// antennas.
static bool validate_tx_antenna_topology(const std::optional<antenna_topology>& topology, unsigned nof_antennas_dl)
{
  if (!topology.has_value()) {
    fmt::print("Number of DL antennas {} does not define a default antenna topology. Valid values are 1, 2, 4 and 8, "
               "otherwise set tx_ant_topology.\n",
               nof_antennas_dl);
    return false;
  }

  unsigned nof_ports = get_total_nof_ports(*topology);
  if (nof_ports != nof_antennas_dl) {
    fmt::print("Antenna topology {} has {} ports, but the number of DL antennas is {}.\n",
               to_string(*topology),
               nof_ports,
               nof_antennas_dl);
    return false;
  }

  return true;
}

/// Validates that the beams assigned to the transmitted SSB candidates fit the antenna topology and are distinct.
static bool validate_ref_beams(const std::vector<du_high_unit_ref_beam_config>& beams, antenna_topology topology)
{
  const unsigned nof_panels   = get_nof_antenna_panels(topology);
  const unsigned nof_pol      = get_nof_antenna_polarizations(topology);
  const unsigned nof_beams_d1 = get_nof_beams_dim1(topology);
  const unsigned nof_beams_d2 = get_nof_beams_dim2(topology);

  for (const auto& beam : beams) {
    if (beam.i_panel >= nof_panels) {
      fmt::print("Panel index {} of beam {} is out of range. Antenna topology {} defines {} panels.\n",
                 beam.i_panel,
                 beam.ref_beam_id,
                 to_string(topology),
                 nof_panels);
      return false;
    }
    if (beam.i_pol >= nof_pol) {
      fmt::print("Polarization index {} of beam {} is out of range. Antenna topology {} defines {} polarizations.\n",
                 beam.i_pol,
                 beam.ref_beam_id,
                 to_string(topology),
                 nof_pol);
      return false;
    }
    if (beam.i_beam_dim1 >= nof_beams_d1) {
      fmt::print("First dimension beam index {} of beam {} is out of range. Antenna topology {} defines {} beams in "
                 "the first dimension.\n",
                 beam.i_beam_dim1,
                 beam.ref_beam_id,
                 to_string(topology),
                 nof_beams_d1);
      return false;
    }
    if (beam.i_beam_dim2 >= nof_beams_d2) {
      fmt::print("Second dimension beam index {} of beam {} is out of range. Antenna topology {} defines {} beams in "
                 "the second dimension.\n",
                 beam.i_beam_dim2,
                 beam.ref_beam_id,
                 to_string(topology),
                 nof_beams_d2);
      return false;
    }
  }

  for (unsigned i = 1, e = beams.size(); i != e; ++i) {
    for (unsigned j = 0; j != i; ++j) {
      if (beams[i].ref_beam_id == beams[j].ref_beam_id) {
        fmt::print("Beam identifier {} is configured more than once.\n", beams[i].ref_beam_id);
        return false;
      }
      if (beams[i].i_panel == beams[j].i_panel and beams[i].i_pol == beams[j].i_pol and
          beams[i].i_beam_dim1 == beams[j].i_beam_dim1 and beams[i].i_beam_dim2 == beams[j].i_beam_dim2) {
        fmt::print("Beams {} and {} select the same beam (panel {}, polarization {}, first dimension {}, second "
                   "dimension {}).\n",
                   beams[j].ref_beam_id,
                   beams[i].ref_beam_id,
                   beams[i].i_panel,
                   beams[i].i_pol,
                   beams[i].i_beam_dim1,
                   beams[i].i_beam_dim2);
        return false;
      }
    }
  }

  return true;
}

/// Validates that every transmitted SSB candidate selects a distinct beam of the cell.
static bool validate_ssb_beams(const du_high_unit_ssb_config&                   config,
                               const std::vector<du_high_unit_ref_beam_config>& ref_beams)
{
  for (const auto& ssb_beam : config.beams) {
    const unsigned ref_beam_id = ssb_beam.ref_beam_id.value();
    const bool     is_defined  = std::any_of(ref_beams.begin(), ref_beams.end(), [ref_beam_id](const auto& beam) {
      return beam.ref_beam_id == ref_beam_id;
    });
    if (!is_defined) {
      fmt::print(
          "SSB index {} selects the beam {}, which the cell does not define.\n", ssb_beam.ssb_index, ref_beam_id);
      return false;
    }
  }

  for (unsigned i = 1, e = config.beams.size(); i != e; ++i) {
    for (unsigned j = 0; j != i; ++j) {
      if (config.beams[i].ref_beam_id == config.beams[j].ref_beam_id) {
        fmt::print("SSB indexes {} and {} are assigned the same beam {}.\n",
                   config.beams[j].ssb_index,
                   config.beams[i].ssb_index,
                   config.beams[i].ref_beam_id.value());
        return false;
      }
    }
  }

  return true;
}

static bool validate_ssb_cell_unit_config(const du_high_unit_ssb_config&                   config,
                                          nr_band                                          band,
                                          arfcn_t                                          dl_arfcn,
                                          subcarrier_spacing                               ssb_scs,
                                          const std::vector<du_high_unit_ref_beam_config>& ref_beams)
{
  if (config.beams.empty()) {
    fmt::print("At least one SSB candidate must be transmitted.\n");
    return false;
  }

  const uint8_t l_max = ssb_get_L_max(ssb_scs, dl_arfcn, band);

  ssb_bitmap_t transmitted_ssbs;
  transmitted_ssbs.set_L_max(l_max);
  transmitted_ssbs.reset();

  for (const auto& ssb_beam : config.beams) {
    if (ssb_beam.ssb_index >= l_max) {
      fmt::print("SSB index {} is out of range. With band n{} and SSB SCS {}kHz, L_max is {}.\n",
                 ssb_beam.ssb_index,
                 fmt::underlying(band),
                 scs_to_khz(ssb_scs),
                 l_max);
      return false;
    }
    if (transmitted_ssbs.test(ssb_beam.ssb_index)) {
      fmt::print("SSB index {} is configured more than once.\n", ssb_beam.ssb_index);
      return false;
    }
    transmitted_ssbs.set(ssb_beam.ssb_index);
  }

  if (!validate_ssb_beams(config, ref_beams)) {
    return false;
  }

  // As per inOneGroup, ssb-PositionsInBurst, ServingCellConfigCommonSIB, TS 38.331, the non-zero groups of 8 bits must
  // all be equal.
  if (l_max == 64) {
    static constexpr uint8_t nof_groups_and_bits_per_gr = 8;
    std::optional<uint8_t>   first_non_zero_group;
    for (uint8_t group_idx = 0; group_idx != nof_groups_and_bits_per_gr; ++group_idx) {
      const auto group_8_bits =
          transmitted_ssbs.extract<uint8_t>(nof_groups_and_bits_per_gr * group_idx, nof_groups_and_bits_per_gr);
      if (group_8_bits == 0) {
        continue;
      }
      if (!first_non_zero_group.has_value()) {
        first_non_zero_group.emplace(group_8_bits);
      } else if (group_8_bits != *first_non_zero_group) {
        fmt::print("Invalid set of SSB indexes. With L_max 64, the transmitted SSB indexes must repeat the same "
                   "pattern in every group of 8 candidates.\n");
        return false;
      }
    }
  }

  return true;
}

static bool validate_dl_ul_arfcn_and_band(const du_high_unit_base_cell_config& config)
{
  const nr_band band = config.band.value_or(band_helper::get_band_from_dl_arfcn(config.dl_f_ref_arfcn));

  if (!band_helper::is_band_known(band)) {
    fmt::print("Band n{} is not a known NR band and has not been registered as a custom band.\n",
               fmt::underlying(band));
    return false;
  }

  // Check if the band is supported with given SCS or band.
  // NOTE: Band n46 would be compatible with the 10MHz BW, but there is no sync raster that falls within the band
  // limits. Also, the Coreset#0 width in RBs given in Table 13-4A, TS 38.213, is larger than the band itself, which
  // is odd. Therefore, we limit the band to minimum 20MHz BW.
  if (band == ocudu::nr_band::n46 and config.channel_bw_mhz < bs_channel_bandwidth::MHz20) {
    fmt::print("Minimum supported bandwidth for n46 is 20MHz.\n");
    return false;
  }

  // Check if the subcarier spacing is valid for the frequency range.
  frequency_range fr = band_helper::get_freq_range(band);
  if ((fr == frequency_range::FR1) && (config.common_scs > subcarrier_spacing::kHz60)) {
    fmt::print("Frequency range 1 does not support the subcarrier spacing of {}.\n", to_string(config.common_scs));
    return false;
  }

  if ((fr == frequency_range::FR2) && (config.common_scs < subcarrier_spacing::kHz60)) {
    fmt::print("Frequency range 2 does not support the subcarrier spacing of {}.\n", to_string(config.common_scs));
    return false;
  }

  // Obtain the minimum bandwidth for the subcarrier and band combination.
  min_channel_bandwidth min_chan_bw = band_helper::get_min_channel_bw(band, config.common_scs);
  if (min_chan_bw == min_channel_bandwidth::invalid) {
    fmt::print("Invalid combination for band n{} and subcarrier spacing {}.\n",
               fmt::underlying(band),
               to_string(config.common_scs));
    return false;
  }

  // Check that the configured bandwidth is greater than or equal to the minimum bandwidth
  if (bs_channel_bandwidth_to_MHz(config.channel_bw_mhz) < min_channel_bandwidth_to_MHz(min_chan_bw)) {
    fmt::print("Minimum supported bandwidth for n{} with SCS {} is {}MHz.\n",
               fmt::underlying(band),
               to_string(config.common_scs),
               min_channel_bandwidth_to_MHz(min_chan_bw));
    return false;
  }

  // Check whether the DL-ARFCN and the UL-ARFCN are within the band and follow the Raster step.
  if (config.band.has_value()) {
    error_type<std::string> ret = band_helper::is_dl_arfcn_valid_given_band(
        *config.band, config.dl_f_ref_arfcn, config.common_scs, config.channel_bw_mhz);
    if (not ret.has_value()) {
      fmt::print(
          "Invalid DL ARFCN={} for band {}. Cause: {}.\n", config.dl_f_ref_arfcn, fmt::underlying(band), ret.error());
      return false;
    }
    // Check if also the corresponding UL ARFCN is valid.
    const arfcn_t ul_arfcn = band_helper::get_ul_arfcn_from_dl_arfcn(config.dl_f_ref_arfcn, config.band.value());
    ret = band_helper::is_ul_arfcn_valid_given_band(*config.band, ul_arfcn, config.channel_bw_mhz, config.common_scs);
    if (not ret.has_value()) {
      // NOTE: The message must say that it's the DL ARFCN that is invalid, as that is the parameters set by the user.
      fmt::print(
          "Invalid DL ARFCN={} for band {}. Cause: {}.\n", config.dl_f_ref_arfcn, fmt::underlying(band), ret.error());
      return false;
    }
  } else {
    if (band == nr_band::invalid) {
      fmt::print("Invalid DL ARFCN={}. Cause: Could not find a valid band.\n", config.dl_f_ref_arfcn);
      return false;
    }
  }

  return true;
}

static bool validate_cell_sib_config(const du_high_unit_base_cell_config& cell_cfg)
{
  // See TS 38.331, V17.0.0, \c type1-r17 in \c SIB-TypeInfo-v1700.
  static constexpr unsigned r17_min_sib_type = 15;

  const du_high_unit_sib_config& sib_cfg = cell_cfg.sib_cfg;

  // A warning SIB has parameters of its own, in the etws and cmas configuration blocks.
  for (const auto& si_msg : sib_cfg.si_sched_info) {
    for (uint8_t sib_it : si_msg.sib_mapping_info) {
      if (sib_it == 6 || sib_it == 7 || sib_it == 8) {
        fmt::print("SIB{} cannot be mapped to an SI message of the si_sched_info list. Configure the {} block "
                   "instead.\n",
                   sib_it,
                   sib_it == 8 ? "cmas" : "etws");
        return false;
      }
    }
  }

  // A cell only broadcasts a warning while it is on air, but its SI window must be reserved from the start, so that
  // the SI messages of the normal operation keep their own as warnings come and go. ETWS takes one SI message for its
  // primary notification and another for its secondary one, CMAS takes one.
  const unsigned nof_pws_si_msgs = (sib_cfg.etws_cfg.has_value() ? 2 : 0) + (sib_cfg.cmas_cfg.has_value() ? 1 : 0);

  // Compute how many slots it takes to transmit all SI messages in sequence.
  const unsigned all_si_msg_slots = (sib_cfg.si_sched_info.size() + nof_pws_si_msgs) * sib_cfg.si_window_len_slots;

  // If the SI period of any SI message is shorter than the number of slots required to transmit the SI messages, the
  // configuration is invalid.
  auto si_period_fits_every_window = [&](unsigned si_period_rf) {
    const unsigned si_period_slots =
        si_period_rf * get_nof_slots_per_subframe(cell_cfg.common_scs) * NOF_SUBFRAMES_PER_FRAME;
    if (all_si_msg_slots > si_period_slots) {
      fmt::print("The SI message period (i.e., {} frames) is too small given the SI window length (i.e., {} slots). "
                 "Increase the SI period or decrease the SI window length.\n",
                 si_period_rf,
                 sib_cfg.si_window_len_slots);
      return false;
    }
    return true;
  };

  for (const auto& si_msg : sib_cfg.si_sched_info) {
    if (not si_period_fits_every_window(si_msg.si_period_rf)) {
      return false;
    }

    // Check if SIB19 is included together with any other SIB, which is not allowed.
    const auto sib19_included = std::find(si_msg.sib_mapping_info.begin(),
                                          si_msg.sib_mapping_info.end(),
                                          static_cast<std::underlying_type_t<sib_type>>(sib_type::sib19));
    if (sib19_included != si_msg.sib_mapping_info.end() && si_msg.sib_mapping_info.size() > 1) {
      fmt::print("SIB19 cannot be included in the SI messages together with other SIBs.\n");
      return false;
    }
  }
  if (sib_cfg.etws_cfg.has_value() and not si_period_fits_every_window(sib_cfg.etws_cfg->si_period_rf)) {
    return false;
  }
  if (sib_cfg.cmas_cfg.has_value() and not si_period_fits_every_window(sib_cfg.cmas_cfg->si_period_rf)) {
    return false;
  }

  std::vector<uint8_t>  sibs_included;
  std::vector<unsigned> si_window_positions;
  unsigned              n_sched_info_list_messages = 0;
  for (const auto& si_msg : sib_cfg.si_sched_info) {
    for (const uint8_t sib_it : si_msg.sib_mapping_info) {
      // si-WindowPosition-r17 is part of release 17 specification only. See TS 38.331, V17.0.0, \c
      // SchedulingInfo2-r17.
      if (sib_it < r17_min_sib_type and si_msg.si_window_position.has_value()) {
        fmt::print("The SIB{} cannot be configured with SI-window position.\n", sib_it);
        return false;
      }
      if (sib_it >= r17_min_sib_type) {
        if (!si_msg.si_window_position.has_value()) {
          fmt::print("The SIB{} must be configured with SI-window position.\n", sib_it);
          return false;
        }
        if (n_sched_info_list_messages == 0) {
          fmt::print("The SIB{} (ID >= 15) requires at least one SIB with ID < 15 to be present; otherwise "
                     "si-WindowLength will not be included.\n",
                     sib_it);
          return false;
        }
      }
      sibs_included.push_back(sib_it);
    }
    if (si_msg.si_window_position.has_value()) {
      si_window_positions.push_back(si_msg.si_window_position.value());
    } else {
      if (not si_window_positions.empty()) {
        fmt::print(
            "Invalid SIB ordering in si_sched_info: SIBs using schedulingInfoList (SIB IDs < 15) must precede SIBs "
            "using schedulingInfoList2 (SIB IDs >= 15).\n");
        return false;
      }
      n_sched_info_list_messages++;
    }
  }

  std::sort(sibs_included.begin(), sibs_included.end());
  // Check if there are repeated SIBs in the SI messages.
  const auto duplicate_it = std::adjacent_find(sibs_included.begin(), sibs_included.end());
  if (duplicate_it != sibs_included.end()) {
    fmt::print("The SIB{} cannot be included more than once in the broadcast SI messages.\n", *duplicate_it);
    return false;
  }

  // If NTN-config present, check if SIB19 sched info is provided.
  const auto sib19_included = std::find(
      sibs_included.begin(), sibs_included.end(), static_cast<std::underlying_type_t<sib_type>>(sib_type::sib19));
  if (cell_cfg.ntn_cfg.has_value() and sib19_included == sibs_included.end()) {
    fmt::print("NTN-Config is present, but SIB19 scheduling information was not provided.\n");
    return false;
  }

  // Check whether SI window position when provided in SI scheduling information is in ascending order. See TS 38.331,
  // \c si-WindowPosition.
  for (unsigned i = 0; i + 1 < si_window_positions.size(); ++i) {
    if (si_window_positions[i] > si_window_positions[i + 1]) {
      fmt::print("The SI window position in the subsequent entry in SI scheduling information must have higher value "
                 "than in the previous entry ({}>{})",
                 si_window_positions[i],
                 si_window_positions[i + 1]);
      return false;
    }
  }

  // SI messages scheduled by schedulingInfoList and/or posSchedulingInfoList do not overlap
  // with SI messages scheduled by schedulingInfoList2. See TS38.331 \c si-WindowPosition.
  if (!si_window_positions.empty() && (si_window_positions[0] <= n_sched_info_list_messages)) {
    fmt::print("Any SI element in schedulingInfoList2 must be scheduled after any SI "
               "in schedulingInfoList ({}<={})\n",
               si_window_positions[0],
               n_sched_info_list_messages);
    return false;
  }

  auto si_messages_collide_pred = [](unsigned pos_a, unsigned per_a, unsigned pos_b, unsigned per_b) -> bool {
    if (pos_a == pos_b) {
      return true;
    }
    unsigned g    = std::gcd(per_a, per_b);
    unsigned diff = (pos_a > pos_b) ? (pos_a - pos_b) : (pos_b - pos_a);
    return (diff % g) == 0;
  };

  if (sib_cfg.si_sched_info.size() > 1) {
    for (unsigned i = 0, e = sib_cfg.si_sched_info.size(); i + 1 != e; ++i) {
      const auto& si_i = sib_cfg.si_sched_info[i];

      // siSchedulingInfoList2 SI's \c si-WindowPosition is 1-indexed, so should be SIs from siSchedulingInfoList
      const unsigned pos_i = si_i.si_window_position.value_or(i + 1);
      const unsigned per_i = si_i.si_period_rf;

      for (unsigned j = i + 1; j != e; ++j) {
        const auto&    si_j  = sib_cfg.si_sched_info[j];
        const unsigned pos_j = si_j.si_window_position.value_or(j + 1);
        const unsigned per_j = si_j.si_period_rf;

        if (si_messages_collide_pred(pos_i, per_i, pos_j, per_j)) {
          fmt::print("SI element at window {} with si-periodicity {} "
                     "will collide with SI element at window {} with si-periodicity {}\n",
                     pos_i,
                     per_i,
                     pos_j,
                     per_j);
          return false;
        }
      }
    }
  }

  return true;
}
static bool validate_cell_slicing_config(const du_high_unit_cell_slice_config& slice_cfg)
{
  const auto& slice_sched_cfg = slice_cfg.sched_cfg;

  if (slice_sched_cfg.min_prb_policy_ratio > slice_sched_cfg.max_prb_policy_ratio) {
    fmt::print("Invalid parameters for slice sst={} sd={}: expected min_prb_policy_ratio <= max_prb_policy_ratio, but "
               "this was found instead ({}>{})\n",
               slice_cfg.sst,
               slice_cfg.sd,
               slice_sched_cfg.min_prb_policy_ratio,
               slice_sched_cfg.max_prb_policy_ratio);
    return false;
  }

  if (slice_sched_cfg.ded_prb_policy_ratio > slice_sched_cfg.min_prb_policy_ratio) {
    fmt::print("Invalid parameters for slice sst={} sd={}: expected ded_prb_policy_ratio <= min_prb_policy_ratio, but "
               "this was found instead ({}>{})\n",
               slice_cfg.sst,
               slice_cfg.sd,
               slice_sched_cfg.ded_prb_policy_ratio,
               slice_sched_cfg.min_prb_policy_ratio);
    return false;
  }

  return true;
}

/// Validates NTN config for TN bands: only ncells may be configured. Returns true on success, otherwise false.
static bool validate_ntn_tn_cell_config(const du_high_unit_cell_ntn_config& ntn, nr_band band)
{
  // TN band: only ncells may be configured; all serving-cell NTN fields must be absent.
  if (ntn.serving.has_value()) {
    fmt::print("ntn: serving-cell NTN fields (e.g. cell_specific_koffset) must not be set for TN band {}. Only "
               "ncells may be configured.\n",
               fmt::underlying(band));
    return false;
  }
  return validate_ntn_neighbor_cells(ntn);
}

static bool validate_cell_cg_config(const du_high_configured_grants& cg_cfg, unsigned nof_crbs, unsigned max_nof_harqs)
{
  if (cg_cfg.max_nof_cell_cg_rbs > nof_crbs) {
    fmt::print("Max number of RBs {} for configured grant exceeds the number of BWP RBs {}.\n",
               cg_cfg.max_nof_cell_cg_rbs,
               nof_crbs);
    return false;
  }

  if (cg_cfg.nof_harq_processes >= max_nof_harqs) {
    fmt::print("Too many UE HARQ processes {} reserved for configured grant (CG). Its value shouldn't exceed the total"
               " maximum number of PUSCH HARQ processes {} - 1.\n",
               cg_cfg.nof_harq_processes,
               max_nof_harqs);
    return false;
  }

  return true;
}

/// Validates the additional TACs broadcast by a cell in trackingAreaList, TS 38.331.
static bool validate_additional_tacs(const du_high_unit_base_cell_config& config, bool is_ntn_band)
{
  if (config.additional_tacs.empty()) {
    return true;
  }

  // TS 38.331: trackingAreaList is only present in an NTN cell.
  if (!is_ntn_band) {
    fmt::print("Additional TACs can only be broadcast by NTN cells.\n");
    return false;
  }

  // tac takes the first of the TACs allowed per PLMN.
  if (config.additional_tacs.size() + 1 > MAX_NOF_TACS_NTN) {
    fmt::print("A cell can broadcast at most {} TACs, but {} are configured.\n",
               MAX_NOF_TACS_NTN,
               config.additional_tacs.size() + 1);
    return false;
  }

  for (unsigned i = 0, e = config.additional_tacs.size(); i != e; ++i) {
    const tac_t tac = config.additional_tacs[i];
    if (!is_valid(tac)) {
      fmt::print("Invalid TAC '{}' in the additional TACs of the cell.\n", tac);
      return false;
    }
    if (tac == config.tac) {
      fmt::print("TAC '{}' is repeated in the additional TACs of the cell.\n", tac);
      return false;
    }
    for (unsigned j = i + 1; j != e; ++j) {
      if (config.additional_tacs[j] == tac) {
        fmt::print("TAC '{}' is repeated in the additional TACs of the cell.\n", tac);
        return false;
      }
    }
  }

  return true;
}

static bool validate_redcap_config(const du_high_unit_base_cell_config& config, nr_band band)
{
  if (not config.redcap_cfg.enabled) {
    return true;
  }
  if (band_helper::get_freq_range(band) != frequency_range::FR1 or
      config.channel_bw_mhz > bs_channel_bandwidth::MHz20) {
    fmt::print("RedCap requires FR1 cells with at most 20 MHz bandwidth.\n");
    return false;
  }
  if (band_helper::is_ntn_band(band) or config.ntn_cfg.has_value() or config.cg_cfg.periodicity_slots.has_value()) {
    fmt::print("RedCap does not support NTN or configured grants.\n");
    return false;
  }
  return true;
}

/// Validates the given cell application configuration. Returns true on success, otherwise false.
static bool validate_base_cell_unit_config(const du_high_unit_base_cell_config& config)
{
  if (config.pci >= INVALID_PCI) {
    fmt::print("Invalid PCI (i.e. {}). PCI ranges from 0 to {}.\n", config.pci, MAX_PCI);
    return false;
  }
  if (config.nof_antennas_dl == 0) {
    fmt::print("The number of DL antennas cannot be zero.\n");
    return false;
  }
  if (config.nof_antennas_ul == 0) {
    fmt::print("The number of UL antennas cannot be zero.\n");
    return false;
  }

  if (config.tac == 0U || config.tac == 0xfffffeU || config.tac > 0xffffffU) {
    fmt::print("Invalid TAC value {}. Valid TAC values are in [1, 16777215] excluding the reserved value 16777214 "
               "(0xfffffe).\n",
               config.tac);
    return false;
  }

  if (config.common_scs == ocudu::subcarrier_spacing::kHz15 and
      config.channel_bw_mhz > ocudu::bs_channel_bandwidth::MHz50) {
    fmt::print("Maximum Channel BW with SCS common 15kHz is 50MHz.\n");
    return false;
  }
  if (config.common_scs == ocudu::subcarrier_spacing::kHz30 and
      config.channel_bw_mhz < ocudu::bs_channel_bandwidth::MHz10) {
    fmt::print("Minimum supported Channel BW with SCS common 30kHz is 10MHz.\n");
    return false;
  }
  if (config.common_scs == ocudu::subcarrier_spacing::kHz30 and
      config.channel_bw_mhz > ocudu::bs_channel_bandwidth::MHz100) {
    fmt::print("Maximum Channel BW with SCS common 30kHz is 100MHz.\n");
    return false;
  }
  if (config.common_scs == ocudu::subcarrier_spacing::kHz60 and
      config.channel_bw_mhz > ocudu::bs_channel_bandwidth::MHz200) {
    fmt::print("Maximum Channel BW with SCS common 60kHz is 200MHz.\n");
    return false;
  }
  if (config.common_scs == ocudu::subcarrier_spacing::kHz120 and
      config.channel_bw_mhz > ocudu::bs_channel_bandwidth::MHz400) {
    fmt::print("Maximum Channel BW with SCS common 120kHz is 400MHz.\n");
    return false;
  }
  if (!validate_dl_ul_arfcn_and_band(config)) {
    return false;
  }

  const nr_band band    = config.band.value_or(band_helper::get_band_from_dl_arfcn(config.dl_f_ref_arfcn));
  const auto    ssb_scs = band_helper::get_most_suitable_ssb_scs(band, config.common_scs);
  if (ssb_scs != config.common_scs) {
    fmt::print("Common SCS {}kHz is not equal to SSB SCS {}kHz. Different SCS for common and SSB is not supported.\n",
               scs_to_khz(config.common_scs),
               scs_to_khz(ssb_scs));
    return false;
  }

  if (!validate_tx_antenna_topology(config.tx_ant_topology, config.nof_antennas_dl)) {
    return false;
  }

  if (!validate_ref_beams(config.ref_beams, *config.tx_ant_topology)) {
    return false;
  }

  if (!validate_ssb_cell_unit_config(config.ssb_cfg, band, config.dl_f_ref_arfcn, ssb_scs, config.ref_beams)) {
    return false;
  }

  const unsigned nof_crbs =
      band_helper::get_n_rbs_from_bw(config.channel_bw_mhz, config.common_scs, band_helper::get_freq_range(band));

  const bool is_ntn_band = band_helper::is_ntn_band(band);
  if (!validate_additional_tacs(config, is_ntn_band)) {
    return false;
  }
  if (config.ntn_cfg) {
    if (is_ntn_band) {
      if (!validate_ntn_config(config, band)) {
        return false;
      }
    } else {
      if (!validate_ntn_tn_cell_config(*config.ntn_cfg, band)) {
        return false;
      }
    }
  }
  // Values beyond 128ms are only signalled via sr-ProhibitTimer-v1700, which is meant for NTN.
  if (config.mcg_cfg.sr_cfg.sr_prohibit_timer.value_or(0) > 128 and not is_ntn_band) {
    fmt::print("mac_cell_group.sr_cfg.sr_prohibit_timer={}ms is only supported in NTN cells. Maximum for TN cells is "
               "128ms.\n",
               *config.mcg_cfg.sr_cfg.sr_prohibit_timer);
    return false;
  }
  if (!validate_pdsch_cell_unit_config(config.pdsch_cfg, nof_crbs, config.nof_antennas_dl, is_ntn_band)) {
    return false;
  }

  if (!validate_csi_cell_unit_config(
          config.csi_cfg, config.common_scs, nof_crbs, config.nof_antennas_dl, config.tdd_ul_dl_cfg)) {
    return false;
  }

  if (!validate_pdcch_unit_config(config)) {
    return false;
  }

  if (!validate_pucch_cell_unit_config(config, config.common_scs, nof_crbs, config.tdd_ul_dl_cfg)) {
    return false;
  }

  if (!validate_srs_cell_unit_config(
          config.srs_cfg, config.common_scs, nof_crbs, config.nof_antennas_ul, config.tdd_ul_dl_cfg)) {
    return false;
  }

  if (!validate_ul_common_unit_config(config.ul_common_cfg, nof_crbs)) {
    return false;
  }

  if (!validate_pusch_cell_unit_config(config.pusch_cfg, nof_crbs, config.pucch_cfg.min_k1, is_ntn_band)) {
    return false;
  }

  if (!validate_prach_cell_unit_config(config.prach_cfg, band, config.nof_antennas_ul)) {
    return false;
  }

  if (band_helper::get_duplex_mode(band) == duplex_mode::TDD and config.tdd_ul_dl_cfg.has_value() and
      !validate_tdd_ul_dl_unit_config(
          config.tdd_ul_dl_cfg.value(),
          config.common_scs,
          config.csi_cfg.csi_rs_enabled ? std::optional<unsigned>{config.pdsch_cfg.dmrs_add_pos} : std::nullopt)) {
    return false;
  }

  if (!validate_cell_sib_config(config)) {
    return false;
  }

  if ((config.rlm_cfg.resource_type == rlm_resource_type::csi_rs or
       config.rlm_cfg.resource_type == rlm_resource_type::ssb_and_csi_rs) and
      not config.csi_cfg.csi_rs_enabled) {
    fmt::print("CSI-RS based Radio Link Monitoring requires CSI-RS to be enabled.\n");
    return false;
  }

  for (const auto& slice_cfg : config.slice_cfg) {
    if (not validate_cell_slicing_config(slice_cfg)) {
      return false;
    }
  }

  if (config.cg_cfg.periodicity_slots.has_value() and
      !validate_cell_cg_config(config.cg_cfg, nof_crbs, config.pusch_cfg.nof_harqs)) {
    return false;
  }

  return validate_redcap_config(config, band);
}

static bool validate_cell_unit_config(const du_high_unit_cell_config& config)
{
  return validate_base_cell_unit_config(config.cell);
}

/// Validates the given list of cell application configuration. Returns true on success, otherwise false.
static bool validate_cells_unit_config(span<const du_high_unit_cell_config> config, const gnb_id_t& gnb_id)
{
  tac_t tac = config[0].cell.tac;
  for (const auto& cell : config) {
    if (!validate_cell_unit_config(cell)) {
      return false;
    }
    if (cell.cell.tac != tac) {
      fmt::print("The TAC must be the same for all cells\n");
      return false;
    }

    if (cell.cell.sector_id.has_value() and
        not nr_cell_identity::create(gnb_id, cell.cell.sector_id.value()).has_value()) {
      fmt::print("Invalid Sector ID {}, for a gNB Id of {} bits\n", cell.cell.sector_id.value(), gnb_id.bit_length);
      return false;
    }
    const auto band          = cell.cell.band.value_or(band_helper::get_band_from_dl_arfcn(cell.cell.dl_f_ref_arfcn));
    bool       is_unlicensed = band_helper::is_unlicensed_band(band);
    // Check if the RA Response Window (in ms) is within the limits for licensed and unlicensed bands.
    unsigned max_ra_resp_window = is_unlicensed ? 40 : 10;
    unsigned ra_resp_window_ms =
        cell.cell.prach_cfg.ra_resp_window.value() >> to_numerology_value(cell.cell.common_scs);
    if (ra_resp_window_ms > max_ra_resp_window) {
      fmt::print("RA Response Window ({}sl -> {}ms) must be smaller than {}ms in {} bands.\n",
                 cell.cell.prach_cfg.ra_resp_window,
                 ra_resp_window_ms,
                 max_ra_resp_window,
                 is_unlicensed ? "unlicensed" : "licensed");
      return false;
    }
  }

  // Checks parameter collisions between cells that can lead to problems.
  for (const auto* it = config.begin(); it != config.end(); ++it) {
    for (const auto* it2 = it + 1; it2 != config.end(); ++it2) {
      const auto& cell1 = *it;
      const auto& cell2 = *it2;

      // Check if both cells are on the same frequency.
      if (cell1.cell.dl_f_ref_arfcn == cell2.cell.dl_f_ref_arfcn) {
        // Two cells on the same frequency should not have the same physical cell identifier.
        if (cell1.cell.pci == cell2.cell.pci) {
          fmt::print("Warning: two cells with the same DL ARFCN (i.e., {}) have the same PCI (i.e., {}).\n",
                     cell1.cell.dl_f_ref_arfcn,
                     cell1.cell.pci);
        }
        if (cell1.cell.sector_id.has_value() and cell1.cell.sector_id == cell2.cell.sector_id and
            cell1.cell.plmn == cell2.cell.plmn) {
          fmt::print("Error: two cells with the same PLMN (i.e., {}) and cell ID (i.e., {})\n",
                     cell1.cell.plmn,
                     cell1.cell.sector_id);
          return false;
        }

        // Two cells on the same frequency should not share the same PRACH root sequence index.
        if ((cell1.cell.prach_cfg.prach_frequency_start == cell2.cell.prach_cfg.prach_frequency_start) &&
            (cell1.cell.prach_cfg.prach_root_sequence_index == cell2.cell.prach_cfg.prach_root_sequence_index)) {
          fmt::print("Warning: cells with PCI {} and {} with the same DL ARFCN (i.e., {}) have the same PRACH root "
                     "sequence index (i.e., {}).\n",
                     cell1.cell.pci,
                     cell2.cell.pci,
                     cell1.cell.dl_f_ref_arfcn,
                     cell1.cell.prach_cfg.prach_root_sequence_index);
        }
      }
    }
  }

  return true;
}

static bool validate_test_mode_unit_config(const du_high_unit_config& config)
{
  if ((config.test_mode_cfg.test_ue.ri > 1) and
      not config.cells_cfg.front().cell.pdcch_cfg.dedicated.dci_format_0_1_and_1_1) {
    fmt::print("For test mode, RI shall not be set if UE is configured to use DCI format 1_0\n");
    return false;
  }
  if (config.test_mode_cfg.test_ue.ri > config.cells_cfg.front().cell.nof_antennas_dl) {
    fmt::print("For test mode, RI cannot be higher than the number of DL antenna ports ({} > {})\n",
               config.test_mode_cfg.test_ue.ri,
               config.cells_cfg.front().cell.nof_antennas_dl);
    return false;
  }

  if (config.test_mode_cfg.test_ue.ri != 2 and config.test_mode_cfg.test_ue.i_1_3 != 0) {
    fmt::print("For test mode with 4 antennas, i_1_3 can only be set for RI=2\n");
  }

  if (config.test_mode_cfg.test_ue.ri != 1 and config.test_mode_cfg.test_ue.i_2 > 1) {
    fmt::print("For test mode with 4 antennas, i_2 can only be higher than 1 for RI=1\n");
  }

  return true;
}

template <typename id_type>
static bool validate_rlc_am_appconfig(id_type id, const du_high_unit_rlc_am_config& config)
{
  // Validate TX.

  rlc_am_sn_size tmp_sn_size;
  if (!from_number(tmp_sn_size, config.tx.sn_field_length)) {
    fmt::print("RLC AM TX SN length is neither 12 or 18 bits. {} sn_size={}\n", id, config.tx.sn_field_length);
    return false;
  }

  rlc_t_poll_retransmit tmp_t_poll_retransmit;
  if (!rlc_t_poll_retransmit_from_int(tmp_t_poll_retransmit, config.tx.t_poll_retx)) {
    fmt::print("Invalid RLC AM TX t-PollRetransmission. {} t_poll_retx={}\n", id, config.tx.t_poll_retx);
    fmt::print(" Valid values are: ms5, ms10, ms15, ms20, ms25, ms30, ms35,"
               " ms40, ms45, ms50, ms55, ms60, ms65, ms70, ms75, ms80, ms85,"
               " ms90, ms95, ms100, ms105, ms110, ms115, ms120, ms125, ms130,"
               " ms135, ms140, ms145, ms150, ms155, ms160, ms165, ms170, ms175,"
               " ms180, ms185, ms190, ms195, ms200, ms205, ms210, ms215, ms220,"
               " ms225, ms230, ms235, ms240, ms245, ms250, ms300, ms350, ms400,"
               " ms450, ms500, ms800, ms1000, ms2000, ms4000\n");
    return false;
  }

  rlc_max_retx_threshold tmp_max_retx_threshold;
  if (!rlc_max_retx_threshold_from_int(tmp_max_retx_threshold, config.tx.max_retx_thresh)) {
    fmt::print("Invalid RLC AM TX max retx threshold. {} max_retx_threshold={}\n", id, config.tx.max_retx_thresh);
    fmt::print(" Valid values are: t1, t2, t3, t4, t6, t8, t16, t32\n");
    return false;
  }

  rlc_poll_pdu tmp_poll_pdu;
  if (!rlc_poll_pdu_from_int(tmp_poll_pdu, config.tx.poll_pdu)) {
    fmt::print("Invalid RLC AM TX PollPDU. {} poll_pdu={}\n", id, config.tx.poll_pdu);
    fmt::print(" Valid values are:"
               "p4, p8, p16, p32, p64, p128, p256, p512, p1024, p2048,"
               " p4096, p6144, p8192, p12288, p16384,p20480,"
               " p24576, p28672, p32768, p40960, p49152, p57344, p65536\n");
    return false;
  }

  rlc_poll_kilo_bytes tmp_poll_bytes;
  if (!rlc_poll_kilo_bytes_from_int(tmp_poll_bytes, config.tx.poll_byte)) {
    fmt::print("Invalid RLC AM TX PollBytes. {} poll_bytes={}\n", id, config.tx.poll_byte);
    fmt::print(" Valid values are (in KBytes):"
               " kB1, kB2, kB5, kB8, kB10, kB15, kB25, kB50, kB75,"
               " kB100, kB125, kB250, kB375, kB500, kB750, kB1000,"
               " kB1250, kB1500, kB2000, kB3000, kB4000, kB4500,"
               " kB5000, kB5500, kB6000, kB6500, kB7000, kB7500,"
               " mB8, mB9, mB10, mB11, mB12, mB13, mB14, mB15,"
               " mB16, mB17, mB18, mB20, mB25, mB30, mB40, infinity\n");
    return false;
  }

  if (config.tx.queue_size == 0) {
    fmt::print("RLC AM TX queue size cannot be 0. {}\n", id);
    return false;
  }
  if (config.tx.queue_size_bytes == 0) {
    fmt::print("RLC AM TX queue size bytes cannot be 0. {}\n", id);
    return false;
  }

  // Validate RX.

  if (!from_number(tmp_sn_size, config.rx.sn_field_length)) {
    fmt::print("RLC AM RX SN length is neither 12 or 18 bits. {} sn_size={}\n", id, config.rx.sn_field_length);
    return false;
  }

  rlc_t_reassembly tmp_t_reassembly;
  if (!rlc_t_reassembly_from_int(tmp_t_reassembly, config.rx.t_reassembly)) {
    fmt::print("RLC AM RX t-Reassembly is invalid. {} t_reassembly={}\n", id, config.rx.t_reassembly);
    fmt::print("Valid values are:"
               " ms40, ms45, ms50, ms55, ms60, ms65, ms70,"
               " ms75, ms80, ms85, ms90, ms95, ms100, ms110,"
               " ms120, ms130, ms140, ms150, ms160, ms170,"
               " ms180, ms190, ms200\n");
    return false;
  }

  rlc_t_status_prohibit tmp_t_status_prohibit;
  if (!rlc_t_status_prohibit_from_int(tmp_t_status_prohibit, config.rx.t_status_prohibit)) {
    fmt::print("RLC AM RX t-statusProhibit is invalid. {} t_status_prohibit={}\n", id, config.rx.t_status_prohibit);
    fmt::print("Valid values are:"
               "ms0, ms5, ms10, ms15, ms20, ms25, ms30, ms35,"
               "ms40, ms45, ms50, ms55, ms60, ms65, ms70,"
               "ms75, ms80, ms85, ms90, ms95, ms100, ms105,"
               "ms110, ms115, ms120, ms125, ms130, ms135,"
               "ms140, ms145, ms150, ms155, ms160, ms165,"
               "ms170, ms175, ms180, ms185, ms190, ms195,"
               "ms200, ms205, ms210, ms215, ms220, ms225,"
               "ms230, ms235, ms240, ms245, ms250, ms300,"
               "ms350, ms400, ms450, ms500, ms800, ms1000,"
               "ms1200, ms1600, ms2000, ms2400\n");
    return false;
  }

  if (config.rx.max_sn_per_status >= window_size(config.rx.sn_field_length)) {
    fmt::print("RLC AM RX max_sn_per_status={} exceeds window_size={}. sn_size={}\n",
               config.rx.max_sn_per_status,
               window_size(config.rx.sn_field_length),
               config.rx.sn_field_length);
    return false;
  }

  return true;
}

static bool validate_rlc_um_appconfig(five_qi_t five_qi, const du_high_unit_rlc_um_config& config)
{
  // Validate TX.

  rlc_um_sn_size tmp_sn_size;
  if (!from_number(tmp_sn_size, config.tx.sn_field_length)) {
    fmt::print("RLC UM TX SN length is neither 6 or 12 bits. {} sn_size={}\n", five_qi, config.tx.sn_field_length);
    return false;
  }

  if (config.tx.queue_size == 0) {
    fmt::print("RLC TX queue size cannot be 0. {}\n", five_qi);
    return false;
  }

  // Validate RX.

  if (!from_number(tmp_sn_size, config.rx.sn_field_length)) {
    fmt::print("RLC TX queue size cannot be 0. {}\n", five_qi);
    return false;
  }

  rlc_t_reassembly tmp_t_reassembly;
  if (!rlc_t_reassembly_from_int(tmp_t_reassembly, config.rx.t_reassembly)) {
    fmt::print("RLC UM RX t-Reassembly is invalid. {} t_reassembly={}\n", five_qi, config.rx.t_reassembly);
    fmt::print("Valid values are:"
               " ms40, ms45, ms50, ms55, ms60, ms65, ms70,"
               " ms75, ms80, ms85, ms90, ms95, ms100, ms110,"
               " ms120, ms130, ms140, ms150, ms160, ms170,"
               " ms180, ms190, ms200\n");
    return false;
  }
  return true;
}

static bool validate_rlc_unit_config(five_qi_t five_qi, const du_high_unit_rlc_bearer_config& config)
{
  // Check mode.
  if (config.mode != "am" && config.mode != "um-bidir") {
    fmt::print("RLC mode is neither \"am\" or \"um-bidir\". {} mode={}\n", five_qi, config.mode);
    return false;
  }

  // Check AM.
  if (config.mode == "am" && !validate_rlc_am_appconfig(five_qi, config.am)) {
    fmt::print("RLC AM config is invalid. {}\n", five_qi);
    return false;
  }

  // Check UM.
  if (config.mode == "um-bidir" && !validate_rlc_um_appconfig(five_qi, config.um)) {
    fmt::print("RLC UM config is invalid. {}\n", five_qi);
    return false;
  }
  return true;
}

static bool validate_custom_freq_bands(const std::vector<du_high_unit_custom_band_config>& bands)
{
  for (const auto& cb : bands) {
    if (cb.band_nr < 1 || cb.band_nr > 1024) {
      fmt::print("Custom band number {} is out of range [1, 1024].\n", cb.band_nr);
      return false;
    }
    if (cb.dl_freq_min >= cb.dl_freq_max) {
      fmt::print("Custom band {}: dl_freq_min ({}) >= dl_freq_max ({}).\n", cb.band_nr, cb.dl_freq_min, cb.dl_freq_max);
      return false;
    }
    if (cb.dl_freq_max - cb.dl_freq_min < 5.0) {
      fmt::print("Custom band {}: DL frequency range ({} - {} MHz) is less than the minimum 5 MHz bandwidth.\n",
                 cb.band_nr,
                 cb.dl_freq_min,
                 cb.dl_freq_max);
      return false;
    }
    if (cb.ul_freq_min.has_value() && *cb.ul_freq_min >= *cb.ul_freq_max) {
      fmt::print("Custom band {}: ul_freq_min ({}) >= ul_freq_max ({}) for FDD band.\n",
                 cb.band_nr,
                 *cb.ul_freq_min,
                 *cb.ul_freq_max);
      return false;
    }
    if (cb.ul_freq_min.has_value() && *cb.ul_freq_max - *cb.ul_freq_min < 5.0) {
      fmt::print("Custom band {}: UL frequency range ({} - {} MHz) is less than the minimum 5 MHz bandwidth.\n",
                 cb.band_nr,
                 *cb.ul_freq_min,
                 *cb.ul_freq_max);
      return false;
    }
    if (cb.f_raster == band_helper::delta_freq_raster::DEFAULT) {
      fmt::print("Custom band {}: f_raster is invalid (must be 15, 30, 60, 100, or 120 kHz).\n", cb.band_nr);
      return false;
    }
    if (cb.ssb_scs == subcarrier_spacing::invalid) {
      fmt::print("Custom band {}: ssb_scs is invalid (must be 15, 30, 120, or 240 kHz).\n", cb.band_nr);
      return false;
    }
    if (cb.delta_gscn != 1 && cb.delta_gscn != 3 && cb.delta_gscn != 7 && cb.delta_gscn != 16) {
      fmt::print("Custom band {}: delta_gscn={} is invalid (must be 1, 3, 7, or 16).\n", cb.band_nr, cb.delta_gscn);
      return false;
    }
  }
  return true;
}

/// Validates the given RLC configuration. Returns true on success, otherwise false.
static bool validate_rlc_config(const du_high_unit_rlc_config& config)
{
  if (config.drb_rx_window_seg_size == 0) {
    fmt::print("Invalid RLC configuration: drb_rx_window_seg_size must be greater than 0\n");
    return false;
  }
  if (config.drb_rx_window_seg_pool_size == 0) {
    fmt::print("Invalid RLC configuration: drb_rx_window_seg_pool_size must be greater than 0\n");
    return false;
  }
  if (config.drb_tx_window_seg_size == 0) {
    fmt::print("Invalid RLC configuration: drb_tx_window_seg_size must be greater than 0\n");
    return false;
  }
  if (config.drb_tx_window_seg_pool_size == 0) {
    fmt::print("Invalid RLC configuration: drb_tx_window_seg_pool_size must be greater than 0\n");
    return false;
  }
  if (config.srb_rx_window_seg_size == 0) {
    fmt::print("Invalid RLC configuration: srb_rx_window_seg_size must be greater than 0\n");
    return false;
  }
  if (config.srb_rx_window_seg_pool_size == 0) {
    fmt::print("Invalid RLC configuration: srb_rx_window_seg_pool_size must be greater than 0\n");
    return false;
  }
  if (config.srb_tx_window_seg_size == 0) {
    fmt::print("Invalid RLC configuration: srb_tx_window_seg_size must be greater than 0\n");
    return false;
  }
  if (config.srb_tx_window_seg_pool_size == 0) {
    fmt::print("Invalid RLC configuration: srb_tx_window_seg_pool_size must be greater than 0\n");
    return false;
  }
  if (config.pdu_recycler_size == 0) {
    fmt::print("Invalid RLC configuration: pdu_recycler_size must be greater than 0\n");
    return false;
  }
  return true;
}

static bool validate_qos_config(span<const du_high_unit_qos_config> config)
{
  for (const auto& qos : config) {
    if (!validate_rlc_unit_config(qos.five_qi, qos.rlc)) {
      return false;
    }
    // delay is in ms and capped to bound the pending-grant vector size in the scheduler.
    if (qos.mac.triggered_ul_grant.has_value() && qos.mac.triggered_ul_grant->delay > SCHEDULER_MAX_TRIG_UL_DELAY) {
      fmt::print("5QI {} triggered_ul_grant delay={} exceeds maximum of {} ms\n",
                 qos.five_qi,
                 qos.mac.triggered_ul_grant->delay,
                 SCHEDULER_MAX_TRIG_UL_DELAY);
      return false;
    }
  }
  return true;
}

/// Collects the DL HARQ feedback the configuration asks for, over the 5QIs.
static void collect_dl_harq_feedback(const du_high_unit_config& config, std::vector<dl_harq_feedback>& feedbacks)
{
  for (const auto& qos : config.qos_cfg) {
    if (not qos.mac.dl_harq_feedback.has_value()) {
      continue;
    }
    feedbacks.push_back(*qos.mac.dl_harq_feedback == "enabled" ? dl_harq_feedback::enabled
                                                               : dl_harq_feedback::disabled);
  }
}

/// Checks that every cell has a DL HARQ process carrying each feedback the configuration needs: the ones the 5QIs ask
/// for, plus enabled feedback for the SRBs. A logical channel restricted to a feedback no process carries would never
/// be multiplexed into a transport block.
static bool validate_dl_harq_feedback_config(const du_high_unit_config& config)
{
  std::vector<dl_harq_feedback> feedbacks;
  collect_dl_harq_feedback(config, feedbacks);

  for (const auto& cell : config.cells_cfg) {
    const auto& pdsch = cell.cell.pdsch_cfg;
    // A UE is given the processes of the cell capped to what it supports.
    const unsigned nof_harqs = std::min(pdsch.nof_harqs, ue_capability_summary::default_max_harq_process_num);

    // [Implementation-defined] The scheduler always serves the SRBs on a process with feedback, so at least one has to
    // keep it, even when no logical channel asks for a feedback.
    if (pdsch.harq_feedback_disabled.any() and
        not is_dl_harq_feedback_available(pdsch.harq_feedback_disabled, nof_harqs, dl_harq_feedback::enabled)) {
      fmt::print("None of the first {} DL HARQ processes of cell pci={} carries HARQ feedback. A UE is given more "
                 "only when it reports support for them, and the SRBs are served on a process with feedback, so "
                 "harq_feedback_disabled must leave at least one of them out\n",
                 nof_harqs,
                 cell.cell.pci);
      return false;
    }

    for (dl_harq_feedback feedback : feedbacks) {
      if (not is_dl_harq_feedback_available(pdsch.harq_feedback_disabled, nof_harqs, feedback)) {
        fmt::print("dl_harq_feedback={} cannot be met: none of the first {} DL HARQ processes of cell pci={} carries "
                   "it. Fix harq_feedback_disabled so one of them does\n",
                   feedback,
                   nof_harqs,
                   cell.cell.pci);
        return false;
      }
    }
  }
  return true;
}

/// Collects the UL HARQ modes the configuration asks for, over the 5QIs.
static void collect_allowed_harq_modes(const du_high_unit_config& config, std::vector<ul_harq_mode>& modes)
{
  for (const auto& qos : config.qos_cfg) {
    if (not qos.mac.allowed_harq_mode.has_value()) {
      continue;
    }
    modes.push_back(*qos.mac.allowed_harq_mode == "mode_a" ? ul_harq_mode::mode_a : ul_harq_mode::mode_b);
  }
}

/// Checks that every cell has a UL HARQ process in each mode the configuration needs: the ones the 5QIs ask for, plus
/// mode A for the SRBs. A logical channel restricted to a mode no process carries would never be multiplexed into a
/// grant.
static bool validate_allowed_harq_mode_config(const du_high_unit_config& config)
{
  std::vector<ul_harq_mode> modes;
  collect_allowed_harq_modes(config, modes);

  for (const auto& cell : config.cells_cfg) {
    const auto& pusch = cell.cell.pusch_cfg;
    // A bit set in harq_mode_b identifies a process in mode B, the inverse of the uplinkHARQ-mode mask of TS 38.331.
    const harq_ul_mode_mask mode_mask = ~pusch.harq_mode_b;
    // A UE is given the processes of the cell capped to what it supports.
    const unsigned nof_harqs = std::min(pusch.nof_harqs, ue_capability_summary::default_max_harq_process_num);

    // [Implementation-defined] The scheduler always serves the SRBs on a mode A process, so at least one has to stay
    // in mode A, even when no logical channel asks for a mode.
    if (pusch.harq_mode_b.any() and not is_ul_harq_mode_available(mode_mask, nof_harqs, ul_harq_mode::mode_a)) {
      fmt::print("None of the first {} UL HARQ processes of cell pci={} operates in UL HARQ mode A. A UE is given "
                 "more only when it reports support for them, and the SRBs are served in mode A, so harq_mode_b must "
                 "leave at least one of them out\n",
                 nof_harqs,
                 cell.cell.pci);
      return false;
    }

    if (modes.empty()) {
      continue;
    }

    // [Implementation-defined] A configured grant takes the HARQ process its periodicity lands on, so the scheduler
    // cannot steer it to the mode a restricted logical channel allows.
    if (cell.cell.cg_cfg.periodicity_slots.has_value()) {
      fmt::print("allowed_harq_mode is not supported together with configured grants, configured for cell pci={}\n",
                 cell.cell.pci);
      return false;
    }

    for (ul_harq_mode mode : modes) {
      if (not is_ul_harq_mode_available(mode_mask, nof_harqs, mode)) {
        fmt::print("allowed_harq_mode={} cannot be met: none of the first {} UL HARQ processes of cell pci={} is "
                   "configured in that mode. Fix harq_mode_b so one of them is\n",
                   mode,
                   nof_harqs,
                   cell.cell.pci);
        return false;
      }
    }
  }
  return true;
}

bool ocudu::validate_du_high_config(const du_high_unit_config& config)
{
  if (!validate_rlc_config(config.rlc_cfg)) {
    return false;
  }

  if (!validate_custom_freq_bands(config.custom_freq_bands)) {
    return false;
  }

  if (!validate_cells_unit_config(config.cells_cfg, config.gnb_id)) {
    return false;
  }

  if (!validate_srb_unit_config(config.srb_cfg)) {
    return false;
  }

  if (!validate_test_mode_unit_config(config)) {
    return false;
  }

  if (!validate_qos_config(config.qos_cfg)) {
    return false;
  }

  if (!validate_dl_harq_feedback_config(config)) {
    return false;
  }

  if (!validate_allowed_harq_mode_config(config)) {
    return false;
  }

  if (!validate_pcap_configs(config)) {
    return false;
  }

  if (!validate_global_ntn_config(config.ntn_satellites)) {
    return false;
  }

  if (!validate_ntn_satellite_refs(config.cells_cfg, config.ntn_satellites)) {
    return false;
  }

  return true;
}
