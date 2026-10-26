// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "uci_allocator_impl.h"
#include "../support/csi_report_helpers.h"
#include "../support/sr_helper.h"
#include "../ue_context/ue_cell.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/csi_report/csi_report_on_pucch_helpers.h"
#include "ocudu/ran/csi_report/csi_report_on_pusch_helpers.h"
#include "ocudu/ran/csi_report/csi_report_size.h"
#include "ocudu/ran/pusch/pusch_uci_beta_offset.h"
#include "ocudu/scheduler/resource_grid_util.h"

using namespace ocudu;

////////////    C-tors and d-tors    ////////////

uci_allocator_impl::uci_allocator_impl(const cell_configuration& cell_cfg_, pucch_allocator& pucch_alloc_) :
  pucch_alloc{pucch_alloc_},
  logger(ocudulog::fetch_basic_logger("SCHED")),
  max_pucch_payload(cell_cfg_.params.init_bwp.pucch.resources.max_payload_234())
{
  // The ring must be at least 1 slot larger than the max. UL allocation delay, to take into account the current slot.
  const unsigned ring_size =
      get_pow2_allocator_ring_size_gt_min(get_max_slot_ul_alloc_delay(cell_cfg_.ntn_ul_alloc_delay_slots()) + 1);
  uci_alloc_grid.resize(ring_size);
}

uci_allocator_impl::~uci_allocator_impl() = default;

////////////    Static functions    ////////////

static void update_uci_on_pusch_harq_offsets(uci_info::harq_info& uci_harq, const uci_on_pusch& uci_cfg)
{
  // We assume the configuration contains the values for beta_offsets.
  const auto& beta_offsets = std::get<uci_on_pusch::beta_offsets_semi_static>(uci_cfg.beta_offsets_cfg.value());

  // The values of \c beta_offsets are set according to Section 9.3, TS38.213.
  const uint16_t harq_ack_nof_bits = uci_harq.harq_ack_nof_bits;
  if (harq_ack_nof_bits <= 2) {
    uci_harq.beta_offset_harq_ack = beta_offsets.beta_offset_ack_idx_1.value();
  } else if (harq_ack_nof_bits > 2 and harq_ack_nof_bits <= 11) {
    uci_harq.beta_offset_harq_ack = beta_offsets.beta_offset_ack_idx_2.value();
  } else {
    uci_harq.beta_offset_harq_ack = beta_offsets.beta_offset_ack_idx_3.value();
  }
}

static void
add_csi_to_uci_on_pusch(uci_info::csi_info& uci_csi, const ue_cell_configuration& ue_cell_cfg, bool configured_grant)
{
  uci_csi.csi_rep_cfg = ue_cell_cfg.csi_report();

  // NOTE: The CSI size depends on whether the CSI is configured on PUSCH or PUCCH, as per Section 5.2.3, TS 38.214:
  // "For both Type I and Type II reports configured for PUCCH but transmitted on PUSCH, the determination of the
  // payload for CSI part 1 and CSI part 2 follows that of PUCCH as described in clause 5.2.4."
  if (not ue_cell_cfg.init_bwp().ul.ue_cfg()->csi.periodic_report.has_value()) {
    csi_report_size csi_size   = get_csi_report_pusch_size(uci_csi.csi_rep_cfg);
    uci_csi.csi_part1_nof_bits = csi_size.part1_size.value();

    ocudu_assert(not configured_grant, "Aperiodic CSI not supported with Configured Grant");
    const auto& uci_cfg = ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg.value().uci_cfg.value();

    // We assume the configuration contains the values for beta_offsets.
    const auto& beta_offsets = std::get<uci_on_pusch::beta_offsets_semi_static>(uci_cfg.beta_offsets_cfg.value());

    // The values of \c beta_offsets are set according to Section 9.3, TS 38.213.
    if (uci_csi.csi_part1_nof_bits <= MAX_NOF_CSI_BITS_BETA_OFFSET_IDX_1) {
      uci_csi.beta_offset_csi_1 = beta_offsets.beta_offset_csi_p1_idx_1.value();
    } else {
      uci_csi.beta_offset_csi_1 = beta_offsets.beta_offset_csi_p1_idx_2.value();
    }

    if (csi_size.part2_min_size.value() > 0U) {
      // As per Section 9.3, TS 38.213, the beta offset index depends on whether the CSI Part 2 payload exceeds
      // \c MAX_NOF_CSI_BITS_BETA_OFFSET_IDX_1 bits. The payload size is only known once CSI Part 1 is decoded,
      // hence the CSI report configuration must not allow payload sizes on both sides of the threshold.
      ocudu_assert((csi_size.part2_max_size.value() <= MAX_NOF_CSI_BITS_BETA_OFFSET_IDX_1) or
                       (csi_size.part2_min_size.value() > MAX_NOF_CSI_BITS_BETA_OFFSET_IDX_1),
                   "The CSI Part 2 size range (i.e., [{}, {}] bits) spans the beta offset threshold (i.e., {} bits)",
                   csi_size.part2_min_size.value(),
                   csi_size.part2_max_size.value(),
                   MAX_NOF_CSI_BITS_BETA_OFFSET_IDX_1);

      uci_csi.beta_offset_csi_2.emplace(csi_size.part2_max_size.value() <= MAX_NOF_CSI_BITS_BETA_OFFSET_IDX_1
                                            ? beta_offsets.beta_offset_csi_p2_idx_1.value()
                                            : beta_offsets.beta_offset_csi_p2_idx_2.value());
    }

  } else {
    uci_csi.csi_part1_nof_bits = get_csi_report_pucch_size(uci_csi.csi_rep_cfg).part1_size.value();
    // NOTE: with PUCCH-configured CSI report, CSI patrt 2 is not supported.

    ocudu_assert(not configured_grant or ue_cell_cfg.init_bwp().ul.ded()->cg_cfg.has_value(),
                 "Missing configuration for Configured Grant");
    const auto& uci_cfg = configured_grant ? ue_cell_cfg.init_bwp().ul.ded()->cg_cfg.value().uci_on_pusch_cfg
                                           : ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg.value().uci_cfg.value();

    // We assume the configuration contains the values for beta_offsets.
    const auto& beta_offsets = std::get<uci_on_pusch::beta_offsets_semi_static>(uci_cfg.beta_offsets_cfg.value());

    // The values of \c beta_offsets are set according to Section 9.3, TS 38.213.
    if (uci_csi.csi_part1_nof_bits <= MAX_NOF_CSI_BITS_BETA_OFFSET_IDX_1) {
      uci_csi.beta_offset_csi_1 = beta_offsets.beta_offset_csi_p1_idx_1.value();
    } else {
      uci_csi.beta_offset_csi_1 = beta_offsets.beta_offset_csi_p1_idx_2.value();
    }
  }
}

////////////    Private functions    ////////////

uci_allocator_impl::slot_alloc_list::ue_uci* uci_allocator_impl::get_uci_alloc(slot_point uci_slot, rnti_t rnti)
{
  auto& ucis = uci_alloc_grid[uci_slot.count()].ucis;
  auto* it   = std::find_if(ucis.begin(), ucis.end(), [rnti](const auto& uci) { return uci.rnti == rnti; });
  return it != ucis.end() ? it : nullptr;
}

unsigned uci_allocator_impl::get_min_pdsch_to_ack_slot_distance(const cell_configuration& cell_cfg,
                                                                slot_point                pdsch_slot,
                                                                rnti_t                    rnti,
                                                                unsigned                  min_k1,
                                                                unsigned                  max_k1,
                                                                unsigned                  ntn_koffset)
{
  ocudu_assert(min_k1 <= max_k1, "Minimum k1 value must be greater than maximum k1 value");
  for (int sl_inc = max_k1; sl_inc >= static_cast<int>(min_k1); --sl_inc) {
    const slot_point uci_slot = pdsch_slot + cell_cfg.ul_delay(sl_inc + ntn_koffset);
    if (get_uci_alloc(uci_slot, rnti) != nullptr) {
      return sl_inc;
    }
  }
  return min_k1;
}

////////////    Public functions    ////////////

void uci_allocator_impl::slot_indication(slot_point sl_tx)
{
  // Clear previous slot UCI allocations.
  uci_alloc_grid[(sl_tx - 1).count()].ucis.clear();
}

void uci_allocator_impl::stop()
{
  for (auto& sl_grid : uci_alloc_grid) {
    sl_grid.ucis.clear();
  }
}

std::optional<uci_allocation> uci_allocator_impl::alloc_harq_ack(cell_resource_allocator& res_alloc,
                                                                 const ue_cell&           ue_cc,
                                                                 unsigned                 k0,
                                                                 span<const uint8_t>      k1_list,
                                                                 pucch_repetition_factor  max_rep_factor)
{
  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();

  // A slot with no k1 candidate carries no HARQ-ACK. The search below takes the min and max of the list, so it has
  // to stop here rather than on an empty range.
  if (k1_list.empty()) {
    return std::nullopt;
  }

  // [Implementation-defined] We restrict the number of HARQ bits per PUCCH that are expected to carry CSI reporting to
  // 2 , until the PUCCH allocator supports more than this.
  // TODO: remove this, as with the new refactor we are not constrained by this anymore.
  static constexpr uint8_t max_harq_bits_per_uci = 2U;

  const slot_point          pdsch_slot = res_alloc[k0].slot;
  const cell_configuration& cell_cfg   = ue_cell_cfg.cell_cfg_common;
  const unsigned            min_pdsch_to_ack_slot_distance =
      get_min_pdsch_to_ack_slot_distance(cell_cfg,
                                         pdsch_slot,
                                         ue_cell_cfg.crnti,
                                         *std::min_element(k1_list.begin(), k1_list.end()),
                                         *std::max_element(k1_list.begin(), k1_list.end()),
                                         ue_cc.ntn_koffset());

  for (const uint8_t k1_candidate : k1_list) {
    // Step 1: Check for validity of the UCI slot and other restrictions.

    // Check whether the UCI slot to be scheduled is >= last UCI HARQ ACK allocated slot for the UE.
    // See TS 38.214, clause 5.1, "In a given scheduled cell, the UE is not expected to receive a first PDSCH in slot i,
    // with the corresponding HARQ-ACK assigned to be transmitted in slot j, and a second PDSCH starting later than the
    // first PDSCH with its corresponding HARQ-ACK assigned to be transmitted in a slot before slot j".
    if (k1_candidate < min_pdsch_to_ack_slot_distance) {
      continue;
    }

    // Retrieve the scheduling results for slot = k0 + k1;
    cell_slot_resource_allocator& slot_alloc = res_alloc[cell_cfg.ul_delay(k0 + k1_candidate + ue_cc.ntn_koffset())];
    const slot_point              uci_slot   = slot_alloc.slot;

    // Check whether UCI slot is UL enabled — O(1), runs before the PUSCH scan below.
    if (not cell_cfg.is_fully_ul_enabled(uci_slot)) {
      continue;
    }
    if (not ue_cc.is_ul_enabled(uci_slot)) {
      continue;
    }

    if (uci_alloc_grid[slot_alloc.slot.count()].ucis.full()) {
      logger.info(
          "rnti={}: UCI allocation for slot={} skipped due to UCI cache full. Attempting this allocation for the next "
          "k1 candidate, if any k1 available",
          ue_cell_cfg.crnti,
          slot_alloc.slot);
      continue;
    }

    // As per Section 9, TS 38.213:
    // > A UE does not expect to detect a DCI format scheduling a PDSCH reception or a SPS PDSCH release and indicating
    // a resource for a PUCCH transmission with corresponding HARQ-ACK information in a slot if the UE previously
    // detects a DCI format scheduling a PUSCH transmission in the slot and if the UE multiplexes HARQ-ACK information
    // in the PUSCH transmission.
    // Note: the constraint applies only to the same UE — a PUSCH from a different UE must not block PUCCH placement.
    const bool ue_has_pusch_in_slot =
        std::any_of(slot_alloc.result_reader().ul().puschs().begin(),
                    slot_alloc.result_reader().ul().puschs().end(),
                    [rnti = ue_cell_cfg.crnti](const ul_sched_info& p) { return p.pusch_cfg.rnti == rnti; });
    if (ue_has_pusch_in_slot) {
      continue;
    }

    // SR and CSI period/offset are in the UE's UL slot numbering, uci_slot is on the gNB DL clock.
    const slot_point ue_ul_slot        = cell_cfg.dl_to_ue_ul_slot(uci_slot);
    const bool       is_sr_opportunity = sr_helper::is_sr_opportunity_slot(
        *ue_cell_cfg.init_bwp().ul.ue_cfg(), cell_cfg.params.init_bwp.pucch.sr_period, ue_ul_slot);
    const unsigned scheduled_harq_bits = get_scheduled_pdsch_counter_in_ue_uci(slot_alloc.slot, ue_cell_cfg.crnti);

    unsigned nof_available_harq_bits = 0U;

    if (ue_cell_cfg.init_bwp().ul.ue_cfg()->csi.periodic_report.has_value() and
        csi_helper::is_csi_reporting_slot(*ue_cell_cfg.init_bwp().ul.ue_cfg()->csi.periodic_report,
                                          cell_cfg.params.init_bwp.csi->csi_rs_period,
                                          ue_ul_slot)) {
      // TODO: Remove this when the PUCCH allocator handle properly more than 2 HARQ-ACK bits + CSI.
      const unsigned csi_report_size = get_csi_report_pucch_size(ue_cell_cfg.csi_report()).part1_size.value();
      // NOTE: This is only to avoid allocating more than 2 HARQ bits in PUCCH that are expected to carry CSI reporting.
      nof_available_harq_bits = std::min(max_pucch_payload - csi_report_size - static_cast<unsigned>(is_sr_opportunity),
                                         static_cast<unsigned>(max_harq_bits_per_uci));
    } else {
      nof_available_harq_bits = max_pucch_payload - static_cast<unsigned>(is_sr_opportunity);
    }

    if (scheduled_harq_bits >= nof_available_harq_bits) {
      continue;
    }

    // Step 2: Try to allocate UCI HARQ ACK for UE.
    auto d_pri =
        pucch_alloc.alloc_ded_harq_ack(res_alloc, ue_cell_cfg, k0, k1_candidate, ue_cc.ntn_koffset(), max_rep_factor);

    // Register new UCI allocation in the respective grid slot entry and derive DAI.
    if (d_pri.has_value()) {
      auto* uci = get_uci_alloc(slot_alloc.slot, ue_cell_cfg.crnti);
      if (uci == nullptr) {
        uci                             = &uci_alloc_grid[slot_alloc.slot.count()].ucis.emplace_back();
        uci->rnti                       = ue_cell_cfg.crnti;
        uci->scheduled_dl_pdcch_counter = 0;
      }
      // If the HARQ-ACK ended up in a PUCCH repetition burst, the report is only complete after its last repetition.
      const span<const slot_point> burst_slots = pucch_alloc.get_pucch_repetition_slots(ue_cell_cfg.crnti, uci_slot);
      const unsigned k1_last_rep = burst_slots.empty() ? k1_candidate : k1_candidate + (burst_slots.back() - uci_slot);

      uci_allocation res{.k1                  = k1_candidate,
                         .k1_last_rep         = k1_last_rep,
                         .harq_bit_idx        = static_cast<uint8_t>(uci->scheduled_dl_pdcch_counter),
                         .pucch_res_indicator = d_pri};
      ++uci->scheduled_dl_pdcch_counter;
      return res;
    }
  }
  return {};
}

bool uci_allocator_impl::alloc_sr_opportunity(cell_slot_resource_allocator& slot_alloc,
                                              const ue_cell_configuration&  ue_cell_cfg)
{
  // Retrieve the scheduling results for slot = k0 + k1;
  auto puschs = slot_alloc.result_builder().ul().puschs();
  auto existing_pusch =
      std::find_if(puschs.begin(), puschs.end(), [rnti = ue_cell_cfg.crnti](const ul_sched_info& pusch) {
        return pusch.pusch_cfg.rnti == rnti;
      });

  const bool has_pusch_grants = not puschs.empty() and existing_pusch != puschs.end();

  // If there is a PUSCH allocated for this UE, do not allocate any PUCCH SR grants.
  if (has_pusch_grants) {
    logger.debug("rnti={}: SR allocation skipped due to PUSCH grant allocated.", ue_cell_cfg.crnti);
    return false;
  }

  return pucch_alloc.alloc_sr_opportunity(slot_alloc, ue_cell_cfg);
}

bool uci_allocator_impl::alloc_csi_opportunity(cell_slot_resource_allocator& slot_alloc,
                                               const ue_cell_configuration&  ue_cell_cfg)
{
  auto puschs = slot_alloc.result_builder().ul().puschs();
  auto existing_pusch =
      std::find_if(puschs.begin(), puschs.end(), [rnti = ue_cell_cfg.crnti](const ul_sched_info& pusch) {
        return pusch.pusch_cfg.rnti == rnti;
      });

  const bool has_pusch_grants = not puschs.empty() and existing_pusch != puschs.end();

  // TODO: check if this case is possible. The first PUSCH that is allocated outside the RAR grant is will be allocated
  //       after the UE objected gets created with the UE dedicated configuration; this means the CSI scheduler should
  //       start allocating CSI grants before any PUSCH grants (except the PUSCH scheduled in the RAR).
  if (has_pusch_grants) {
    // If there is UCI grant allocated, allocate it.
    ocudu_assert(not existing_pusch->uci.has_value(), "UCI on PUSCH grant for CSI cannot be already allocated");
    existing_pusch->uci.emplace();
    existing_pusch->uci.value().alpha = ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg.value().uci_cfg.value().scaling;

    // The UCI scheduler should always start before the Configured Grant scheduler; therefore, when this function is
    // called, there should never be a Configured Grant PUSCH present.
    constexpr bool configured_grant = false;
    add_csi_to_uci_on_pusch(
        existing_pusch->uci.value().csi.emplace(uci_info::csi_info()), ue_cell_cfg, configured_grant);
    return false;
  }

  // Else, allocate the CSI on the PUCCH.
  return pucch_alloc.alloc_csi_opportunity(slot_alloc, ue_cell_cfg);
}

void uci_allocator_impl::multiplex_uci_on_pusch(ul_sched_info&                pusch_grant,
                                                cell_slot_resource_allocator& slot_alloc,
                                                const ue_cell_configuration&  ue_cell_cfg,
                                                bool                          include_aperiodic_csi,
                                                bool                          configured_grant)

{
  // Move the bits that are carried by the PUCCH into the PUSCH.
  const pucch_uci_bits pucch_uci = pucch_alloc.remove_ue_uci_from_pucch(slot_alloc, ue_cell_cfg);

  // If there are no UCI bits from PUCCH and no aperiodic CSI, then there is no UCI to be multiplexed on the PUSCH.
  if (not include_aperiodic_csi and pucch_uci.harq_ack_nof_bits + pucch_uci.csi_part1_nof_bits == 0) {
    return;
  }

  // We assume that at this point, there are no existing UCI grants in the PUSCH; allocate a new one.
  uci_info& uci = pusch_grant.uci.emplace();
  // NOTE: As per TS 38.214, Section 6.1 (Second paragraph in Rel. 15.14.0), with Configured Grant, "scaling of
  // UCI-OnPUSCH" is provided by PUSCH-Config.
  uci.alpha = ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg.value().uci_cfg.value().scaling;

  if (pucch_uci.csi_part1_nof_bits != 0 or include_aperiodic_csi) {
    // The number of bits is computed based on the CSI report configuration.
    add_csi_to_uci_on_pusch(uci.csi.emplace(uci_info::csi_info()), ue_cell_cfg, configured_grant);
  }

  if (pucch_uci.harq_ack_nof_bits != 0) {
    uci.harq.emplace(uci_info::harq_info());
    uci.harq->harq_ack_nof_bits = pucch_uci.harq_ack_nof_bits;
    update_uci_on_pusch_harq_offsets(*uci.harq, ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg.value().uci_cfg.value());
  }
}

uint8_t uci_allocator_impl::get_scheduled_pdsch_counter_in_ue_uci(slot_point uci_slot, rnti_t crnti)
{
  auto* uci = get_uci_alloc(uci_slot, crnti);
  if (uci == nullptr) {
    return 0;
  }
  return uci->scheduled_dl_pdcch_counter;
}

bool uci_allocator_impl::has_harq_ack_on_common_pucch_res(rnti_t rnti, slot_point sl_tx)
{
  return pucch_alloc.has_common_pucch_grant(rnti, sl_tx);
}

bool uci_allocator_impl::has_pucch_repetition(rnti_t rnti, slot_point sl_tx)
{
  return not pucch_alloc.get_pucch_repetition_slots(rnti, sl_tx).empty();
}
