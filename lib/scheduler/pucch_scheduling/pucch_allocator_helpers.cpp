// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "pucch_allocator_helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/pucch/pucch_configuration.h"
#include "ocudu/ran/pucch/pucch_info.h"
#include "ocudu/ran/pucch/pucch_mapping.h"
#include "ocudu/scheduler/config/pucch_default_resource.h"
#include "ocudu/scheduler/config/pucch_resource_builder_params.h"
#include "ocudu/support/ocudu_assert.h"

using namespace ocudu;

template <unsigned ResourceSetId>
const pucch_resource& pucch_helper::get_harq_resource(const ue_cell_configuration& ue_cfg, unsigned pri)
{
  const auto& res_params = ue_cfg.cell_cfg_common.params.init_bwp.pucch.resources;
  const auto& cell_res   = ue_cfg.cell_cfg_common.bwp_res[to_bwp_id(0)].ul().pucch;
  const auto& ue_bwp     = *ue_cfg.init_bwp().ul.ue_cfg();
  if (pri < res_params.res_set_size.value()) {
    return cell_res.get_ded(res_params.harq_res_id<ResourceSetId>(ue_bwp.pucch.res_set_cfg_id, pri));
  }
  ocudu_assert(res_params.format_01() == pucch_format::FORMAT_0 and res_params.format_234() == pucch_format::FORMAT_2,
               "Extra PUCCH HARQ-ACK resources are only present when using F0+F2");
  if (ResourceSetId == 0) {
    if (pri == res_params.res_set_size.value()) {
      return cell_res.get_ded(res_params.sr_res_id(ue_bwp.pucch.sr_res_id));
    }
    ocudu_assert(pri == res_params.res_set_size.value() + 1, "Invalid PRI={} for Resource Set 0", pri);
    return cell_res.get_ded(res_params.csi_f0_res_id(ue_bwp.csi.periodic_report->pucch_res_id));
  }

  if (pri == res_params.res_set_size.value()) {
    return cell_res.get_ded(res_params.sr_f2_res_id(ue_bwp.pucch.sr_res_id));
  }
  ocudu_assert(pri == res_params.res_set_size.value() + 1, "Invalid PRI={} for Resource Set 1", pri);
  return cell_res.get_ded(res_params.csi_res_id(ue_bwp.csi.periodic_report->pucch_res_id));
}

template const pucch_resource& pucch_helper::get_harq_resource<0>(const ue_cell_configuration&, unsigned);
template const pucch_resource& pucch_helper::get_harq_resource<1>(const ue_cell_configuration&, unsigned);

const pucch_resource& pucch_helper::get_sr_resource(const ue_cell_configuration& ue_cfg)
{
  const auto& res_params = ue_cfg.cell_cfg_common.params.init_bwp.pucch.resources;
  const auto& cell_res   = ue_cfg.cell_cfg_common.bwp_res[to_bwp_id(0)].ul().pucch;
  const auto& ue_bwp     = *ue_cfg.init_bwp().ul.ue_cfg();
  return cell_res.get_ded(res_params.sr_res_id(ue_bwp.pucch.sr_res_id));
}

const pucch_resource& pucch_helper::get_csi_resource(const ue_cell_configuration& ue_cfg)
{
  const auto& ue_bwp = *ue_cfg.init_bwp().ul.ue_cfg();
  ocudu_assert(ue_bwp.csi.periodic_report.has_value(),
               "CSI resource requested, but periodic CSI reporting is not configured for the UE");
  const auto& res_params = ue_cfg.cell_cfg_common.params.init_bwp.pucch.resources;
  const auto& cell_res   = ue_cfg.cell_cfg_common.bwp_res[to_bwp_id(0)].ul().pucch;
  return cell_res.get_ded(res_params.csi_res_id(ue_bwp.csi.periodic_report->pucch_res_id));
}

const pucch_resource& pucch_helper::get_common_resource(const cell_configuration&      cell_cfg,
                                                        const dci_context_information& dci_info,
                                                        unsigned                       d_pri)
{
  // Get N_CCE (nof_coreset_cces) and n_{CCE,0} (start_cce_idx), as per TS 38.213, Section 9.2.1.
  const unsigned nof_coreset_cces = dci_info.coreset_cfg->get_nof_cces();
  const unsigned start_cce_idx    = dci_info.cces.ncce;

  // r_PUCCH, as per Section 9.2.1, TS 38.213.
  const unsigned r_pucch = get_pucch_default_resource_index(start_cce_idx, nof_coreset_cces, d_pri);
  ocudu_assert(r_pucch < 16, "r_PUCCH must be less than 16");
  return cell_cfg.bwp_res[to_bwp_id(0)].ul().pucch.get_cmn(r_pucch);
}

void pucch_helper::fill_common_pdu(pucch_info&               pucch_pdu,
                                   const cell_configuration& cell_cfg,
                                   const pucch_resource&     common_res,
                                   rnti_t                    rnti)
{
  pucch_pdu.crnti   = rnti;
  pucch_pdu.bwp_cfg = &cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params;
  pucch_pdu.res     = &common_res;
  // [Implementation-defined] We do not configure slot repetition for common PUCCH resources.
  pucch_pdu.repetition.reset();

  const pucch_config_common& pucch_cmn = *cell_cfg.params.ul_cfg_common.init_ul_bwp.pucch_cfg_common;
  const unsigned             n_id_hop =
      pucch_cmn.hopping_id.has_value() ? *pucch_cmn.hopping_id : static_cast<unsigned>(cell_cfg.params.pci);

  switch (common_res.format()) {
    case pucch_format::FORMAT_0: {
      auto& f0         = pucch_pdu.format_params.emplace<pucch_info::f0_config>();
      f0.group_hopping = pucch_cmn.group_hopping;
      f0.n_id_hopping  = n_id_hop;
      // SR cannot be reported using common PUCCH resources.
      pucch_pdu.uci_bits.sr_bits = sr_nof_bits::no_sr;
      // [Implementation-defined] For the default PUCCH resources, we assume only 1 HARQ-ACK process needs to be
      // reported.
      pucch_pdu.uci_bits.harq_ack_nof_bits = 1;
      break;
    }
    case pucch_format::FORMAT_1: {
      auto& format_1         = pucch_pdu.format_params.emplace<pucch_info::f1_config>();
      format_1.group_hopping = pucch_cmn.group_hopping;
      format_1.n_id_hopping  = n_id_hop;
      // SR cannot be reported using common PUCCH resources.
      pucch_pdu.uci_bits.sr_bits = sr_nof_bits::no_sr;
      // [Implementation-defined] For the default PUCCH resources, we assume only 1 HARQ-ACK process needs to be
      // reported.
      pucch_pdu.uci_bits.harq_ack_nof_bits = 1;
      break;
    }
    default:
      ocudu_assertion_failure("Only PUCCH Format 0 and 1 can be used for PUCCH common resources");
  }
}

void pucch_helper::fill_ded_pdu(pucch_info&                     pucch_pdu,
                                const cell_configuration&       cell_cfg,
                                const pucch_resource&           pucch_res,
                                const pucch_uci_bits&           uci_bits,
                                const csi_report_configuration* csi_cfg,
                                rnti_t                          rnti,
                                pucch_repetition_tx_slot        rep_state,
                                slot_point                      rep_anchor_slot)
{
  pucch_pdu.crnti   = rnti;
  pucch_pdu.bwp_cfg = &cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params;
  pucch_pdu.res     = &pucch_res;

  if (pucch_res.format() == pucch_format::FORMAT_0 or pucch_res.format() == pucch_format::FORMAT_1) {
    ocudu_assert(pucch_pdu.uci_bits.harq_ack_nof_bits <= 2, "PUCCH F0/1 can carry 2 HARQ-ACK bits at most");
    ocudu_assert(pucch_pdu.uci_bits.sr_bits == sr_nof_bits::no_sr or pucch_pdu.uci_bits.sr_bits == sr_nof_bits::one,
                 "PUCCH F0/1 can carry 1 SR bit at most");
    ocudu_assert(pucch_pdu.uci_bits.csi_part1_nof_bits == 0, "PUCCH F0/1 can't carry CSI bits");
  }
  pucch_pdu.uci_bits = uci_bits;
  // Generate CSI report configuration if there are CSI bits in UCI.
  if (pucch_pdu.uci_bits.csi_part1_nof_bits != 0U) {
    pucch_pdu.csi_rep_cfg = *csi_cfg;
  }
  pucch_pdu.repetition = rep_state != pucch_repetition_tx_slot::no_multi_slot
                             ? std::optional<pucch_info::repetition_info>({rep_anchor_slot, rep_state})
                             : std::nullopt;

  // [Implementation-defined] We do not configure group or sequence hopping.
  constexpr auto group_hopping = pucch_group_hopping::NEITHER;
  // [Implementation-defined] We don't set the parameter hoppingId.
  const auto n_id_hopping = cell_cfg.params.pci;
  // [Implementation-defined] We don't set the parameter dataScramblingIdentityPUSCH.
  const auto n_id_scrambling = cell_cfg.params.pci;
  // [Implementation-defined] We don't set the parameter scramblingID0.
  const auto n_id_0_scrambling = cell_cfg.params.pci;

  switch (pucch_res.format()) {
    case pucch_format::FORMAT_0: {
      auto& format_0 = pucch_pdu.format_params.emplace<pucch_info::f0_config>();

      // \c pucch-GroupHopping and \c hoppingId are set as per TS 38.211, Section 6.3.2.2.1.
      format_0.group_hopping = group_hopping;
      format_0.n_id_hopping  = n_id_hopping;
    } break;
    case pucch_format::FORMAT_1: {
      auto& format_1 = pucch_pdu.format_params.emplace<pucch_info::f1_config>();

      // \c pucch-GroupHopping and \c hoppingId are set as per TS 38.211, Section 6.3.2.2.1.
      format_1.group_hopping = group_hopping;
      format_1.n_id_hopping  = n_id_hopping;
    } break;
    case pucch_format::FORMAT_2: {
      auto& f2 = pucch_pdu.format_params.emplace<pucch_info::f2_config>();

      f2.n_id_scrambling    = n_id_scrambling;
      f2.n_id_0_scrambling  = n_id_0_scrambling;
      const auto max_c_rate = to_float(cell_cfg.params.init_bwp.pucch.resources.max_code_rate_234());
      f2.nof_prbs           = uci_bits.harq_ack_nof_bits != 0U
                                  ? get_pucch_format2_nof_prbs(
                              uci_bits.get_total_bits(), pucch_res.prbs().length(), pucch_res.syms.length(), max_c_rate)
                                  : pucch_res.prbs().length();
    } break;
    case pucch_format::FORMAT_3: {
      const auto& res_f3 = std::get<pucch_resource::f3_config>(pucch_res.format_params);
      auto&       f3     = pucch_pdu.format_params.emplace<pucch_info::f3_config>();

      f3.group_hopping      = group_hopping;
      f3.n_id_hopping       = n_id_hopping;
      f3.n_id_scrambling    = n_id_scrambling;
      f3.n_id_0_scrambling  = n_id_0_scrambling;
      const auto max_c_rate = to_float(cell_cfg.params.init_bwp.pucch.resources.max_code_rate_234());
      f3.nof_prbs           = uci_bits.harq_ack_nof_bits != 0U ? get_pucch_format3_nof_prbs(uci_bits.get_total_bits(),
                                                                                  pucch_res.prbs().length(),
                                                                                  pucch_res.syms.length(),
                                                                                  max_c_rate,
                                                                                  pucch_res.second_hop_prb.has_value(),
                                                                                  res_f3.additional_dmrs,
                                                                                  res_f3.pi_2_bpsk)
                                                               : pucch_res.prbs().length();
    } break;
    case pucch_format::FORMAT_4: {
      auto& f4 = pucch_pdu.format_params.emplace<pucch_info::f4_config>();

      f4.group_hopping     = group_hopping;
      f4.n_id_hopping      = n_id_hopping;
      f4.n_id_scrambling   = n_id_scrambling;
      f4.n_id_0_scrambling = n_id_0_scrambling;
    } break;
    default:
      ocudu_assertion_failure("Invalid PUCCH format");
  }
}
