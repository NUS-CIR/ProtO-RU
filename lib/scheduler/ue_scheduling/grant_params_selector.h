// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../cell/cell_harq_manager.h"
#include "../slicing/slice_ue_repository.h"
#include "../support/sch_pdu_builder.h"
#include "ocudu/ran/sch/sch_mcs.h"

namespace ocudu {

class ue_cell;
class slice_ue;
struct search_space_info;

namespace sched_helper {

/// PDCCH and PDSCH parameters recommended for a DL grant.
struct dl_sched_context {
  /// SearchSpace to use.
  search_space_id ss_id;
  /// PDSCH time-domain resource index. Indexes the PDSCH TDRA list in use for the SearchSpace (Rel-16 list if
  /// configured, else common/legacy) and is signalled directly in the DCI Time domain resource assignment field.
  uint8_t pdsch_td_res_index;
  /// Recommended MCS, considering channel state or, in case of reTx, last HARQ MCS.
  sch_mcs_index recommended_mcs;
  /// Recommended number of layers.
  unsigned recommended_ri;
  /// Limits for grant size in RBs.
  interval<unsigned> nof_rb_lims;
  /// Expected number of RBs to allocate.
  unsigned expected_nof_rbs;
  /// Pending bytes for newTx.
  units::bytes pending_bytes;
  /// Restrictions the transport block carries. Open for a reTx, which follows the process it already holds.
  dl_transport_block_restrictions restrictions;
  /// Number of Rel-16 slot-based PDSCH repetitions of the selected TDRA row, or nullopt for a single transmission.
  std::optional<uint8_t> nof_repetitions;
};

/// Retrieve recommended PDCCH and PDSCH parameters for a newTx DL grant.
/// The number of Rel-16 PDSCH repetitions to request is decided by link adaptation: when it asks for repetitions, the
/// selector picks the TDRA row carrying that repetitionNumber-r16 and returns nullopt if it does not fit the slot;
/// otherwise it picks a single-transmission row.
std::optional<dl_sched_context> get_newtx_dl_sched_context(const slice_ue&                 u,
                                                           slot_point                      pdcch_slot,
                                                           slot_point                      pdsch_slot,
                                                           bool                            interleaving_enabled,
                                                           units::bytes                    pending_bytes,
                                                           dl_transport_block_restrictions restrictions);

/// Retrieve recommended PDCCH and PDSCH parameters for a reTx DL grant.
/// The reTx keeps the repetition scheme of the original transmission, so only the TDRA row carrying its
/// repetitionNumber-r16 is eligible. Returns nullopt, deferring the grant, when that row does not fit the slot.
std::optional<dl_sched_context> get_retx_dl_sched_context(const slice_ue&               u,
                                                          slot_point                    pdcch_slot,
                                                          slot_point                    pdsch_slot,
                                                          bool                          interleaving_enabled,
                                                          const dl_harq_process_handle& h_dl,
                                                          unsigned                      max_rbs = MAX_NOF_PRBS);

/// Minimum number of VRBs of a DL newTx grant.
unsigned get_min_newtx_dl_nof_vrbs(const search_space_info& ss_info, bool interleaving_enabled);

/// \brief Select DL VRBs to allocate for a newTx, using resource allocation type 0 or 1.
///
/// \param[in] max_nof_rbs Maximum number of VRBs of the grant.
/// \param[in] nof_rbs_remaining Hard limit on the VRBs of a type 0 grant, which may exceed \c max_nof_rbs to fit whole
/// RBGs.
vrb_alloc compute_newtx_dl_vrbs(const dl_sched_context&  decision_ctxt,
                                const search_space_info& ss_info,
                                const vrb_bitmap&        used_vrbs,
                                bool                     interleaving_enabled,
                                unsigned                 max_nof_rbs,
                                unsigned                 nof_rbs_remaining);

/// Select DL VRBs to allocate for a reTx, keeping the number of VRBs of the previous transmission.
vrb_alloc compute_retx_dl_vrbs(const dl_sched_context&  decision_ctxt,
                               const search_space_info& ss_info,
                               const vrb_bitmap&        used_vrbs,
                               bool                     interleaving_enabled);

/// PDCCH and PUSCH parameters recommended for a UL grant.
struct ul_sched_context {
  /// SearchSpace to use.
  search_space_id ss_id;
  /// PUSCH time-domain resource index.
  uint8_t pusch_td_res_index;
  /// Limits on VRBs for UL grant allocation.
  vrb_interval vrb_lims;
  /// Limits for grant size in RBs.
  interval<unsigned> nof_rb_lims;
  /// Recommended MCS, considering channel state or, in case of reTx, last HARQ MCS.
  sch_mcs_index recommended_mcs;
  /// Expected number of RBs to allocate.
  unsigned expected_nof_rbs;
  /// Pending bytes for newTx.
  units::bytes pending_bytes;
  /// PUSCH config params.
  pusch_config_params pusch_cfg;
  /// Number of Rel-16 PUSCH repetitions of the selected TDRA row, or nullopt for a single transmission.
  std::optional<uint8_t> nof_repetitions;
  /// \brief TDRA index of the single-transmission row to fall back to, when the bundle implied by
  /// \c pusch_td_res_index turns out not to be schedulable in this slot.
  ///
  /// Picked by the very search that picked \c pusch_td_res_index, which walks the same candidates either way, so it
  /// costs nothing. Empty when the selected row is already a single transmission, or when no single-transmission row
  /// qualifies for this slot.
  std::optional<uint8_t> single_tx_pusch_td_res_index;
};

/// Retrieve recommended PDCCH and PUSCH parameters for a newTx UL grant.
/// The number of Rel-16 PUSCH repetitions to request is decided by link adaptation: when it asks for repetitions, the
/// selector picks the TDRA row carrying that numberOfRepetitions-r16 and returns nullopt if it does not fit the slot;
/// otherwise it picks a single-transmission row.
std::optional<ul_sched_context> get_newtx_ul_sched_context(const slice_ue&   u,
                                                           slot_point        pdcch_slot,
                                                           slot_point        pusch_slot,
                                                           unsigned          uci_nof_harq_bits,
                                                           units::bytes      pending_bytes,
                                                           ofdm_symbol_range allowed_symbols);

/// Retrieve recommended PDCCH and PUSCH parameters for a reTx UL grant.
/// The reTx tries to reuse the repetition scheme of the original transmission, if it's not possible, fallback to single
/// transmission.
std::optional<ul_sched_context> get_retx_ul_sched_context(const slice_ue&               u,
                                                          slot_point                    pdcch_slot,
                                                          slot_point                    pusch_slot,
                                                          unsigned                      uci_nof_harq_bits,
                                                          const ul_harq_process_handle& h_ul,
                                                          ofdm_symbol_range             allowed_symbols,
                                                          unsigned                      max_rbs = MAX_NOF_PRBS);

/// Re-size an already selected UL grant for a UCI payload that became known only after the \c ul_sched_context was
/// built, i.e. once a PUSCH repetition bundle's slots were resolved. Only the UCI-dependent parameters are
/// recomputed (PUSCH config params, MCS, RB count); the searchSpace, TDRA row and RB limits stay as selected.
/// \param[in] bundle_tx_offsets Slot offsets of the bundle's occasions beyond the base one, relative to
/// \c pusch_slot. The UE multiplexes the UCI onto a single occasion, so a CSI report due in any of them rides here.
/// \return false if no valid MCS/RB combination fits the new payload, in which case \c ctxt is left untouched and the
/// grant is to be deferred.
/// TODO: Refactor \c get_ul_sched_context to avoid this resizing in case of PUSCH repetitions.
bool resize_newtx_ul_grant_for_uci(ul_sched_context&   ctxt,
                                   const slice_ue&     u,
                                   slot_point          pusch_slot,
                                   unsigned            uci_nof_harq_bits,
                                   span<const uint8_t> bundle_tx_offsets);

/// \brief Downgrade an already selected newTx UL grant to the single-transmission TDRA row picked alongside the
/// repetition one, for when the bundle cannot be scheduled after all.
///
/// The row taking over spans a different number of symbols, so the grant is re-sized for it (PUSCH config params,
/// MCS, RB count); the searchSpace and RB limits stay as selected.
/// \param[in] uci_nof_harq_bits HARQ-ACK bits booked in \c pusch_slot, the only slot a single transmission occupies.
/// \return false if the selector found no single-transmission row, or if no valid MCS/RB combination fits it, in
/// which case \c ctxt is left untouched and the grant is to be skipped.
bool downgrade_newtx_ul_grant_to_single_tx(ul_sched_context& ctxt,
                                           const slice_ue&   u,
                                           slot_point        pusch_slot,
                                           unsigned          uci_nof_harq_bits);

/// Downgrade an already selected reTx UL grant to a single transmission. See
/// \ref downgrade_newtx_ul_grant_to_single_tx.
bool downgrade_retx_ul_grant_to_single_tx(ul_sched_context&             ctxt,
                                          const slice_ue&               u,
                                          slot_point                    pusch_slot,
                                          unsigned                      uci_nof_harq_bits,
                                          const ul_harq_process_handle& h_ul);

/// Re-size an already selected reTx UL grant for a UCI payload known only after the bundle was resolved. See
/// \ref resize_newtx_ul_grant_for_uci.
bool resize_retx_ul_grant_for_uci(ul_sched_context&             ctxt,
                                  const slice_ue&               u,
                                  slot_point                    pusch_slot,
                                  unsigned                      uci_nof_harq_bits,
                                  const ul_harq_process_handle& h_ul,
                                  span<const uint8_t>           bundle_tx_offsets);

/// Select UL VRBs to allocate for a newTx.
vrb_interval compute_newtx_ul_vrbs(const ul_sched_context& decision_ctxt,
                                   const vrb_bitmap&       used_vrbs,
                                   unsigned                max_nof_rbs = MAX_NOF_PRBS);

/// Select UL VRBs to allocate for a reTx.
vrb_interval compute_retx_ul_vrbs(const ul_sched_context& decision_ctxt, const vrb_bitmap& used_vrbs);

} // namespace sched_helper
} // namespace ocudu
