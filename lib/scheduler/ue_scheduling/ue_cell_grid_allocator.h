// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../cell/resource_grid.h"
#include "../pdcch_scheduling/pdcch_resource_allocator.h"
#include "../srs/srs_allocator.h"
#include "../uci_scheduling/uci_allocator.h"
#include "../ue_context/ue_repository.h"
#include "grant_params_selector.h"
#include "ocudu/adt/noop_functor.h"

namespace ocudu {

struct scheduler_ue_expert_config;

/// Request to reserve space for control channels of a DL grant.
struct ue_newtx_dl_grant_request {
  /// UE to allocate.
  const slice_ue& user;
  /// Slot at which PDSCH will take place.
  slot_point pdsch_slot;
  /// Pending newTx bytes to allocate.
  units::bytes pending_bytes;
  /// Whether interleaving VRB-to-PRB mapping is enabled.
  bool interleaving_enabled;
  /// Restrictions the transport block carries, as selected for the pending bytes.
  dl_transport_block_restrictions restrictions;
};

/// Request for a reTx DL grant allocation.
struct ue_retx_dl_grant_request {
  /// UE to allocate.
  const slice_ue& user;
  /// Slot at which PDSCH PDU shall be allocated.
  slot_point pdsch_slot;
  /// DL HARQ process to reTx.
  dl_harq_process_handle h_dl;
  /// Current DL VRB occupation.
  vrb_bitmap& used_dl_vrbs;
  /// Whether interleaving VRB-to-PRB mapping is enabled.
  bool interleaving_enabled;
  /// Maximum number of RBs the slice allows for this grant (caps adaptive retx search).
  unsigned max_rbs = MAX_NOF_PRBS;
};

/// Request to reserve space for control channels of a UL grant.
struct ue_newtx_ul_grant_request {
  /// UE to allocate.
  const slice_ue& user;
  /// Slot at which PUSCH will take place.
  slot_point pusch_slot;
  /// Pending newTx bytes to allocate.
  units::bytes pending_bytes;
  /// Symbols that can be used for PUSCH allocation.
  ofdm_symbol_range allowed_symbols;
};

/// Request for a reTx UL grant allocation.
struct ue_retx_ul_grant_request {
  /// UE to allocate.
  const slice_ue& user;
  /// Slot at which PUSCH PDU shall be allocated.
  slot_point pusch_slot;
  /// UL HARQ process to reTx.
  ul_harq_process_handle h_ul;
  /// Current UL VRB occupation.
  vrb_bitmap& used_ul_vrbs;
  /// Symbols that can be used for PUSCH allocation.
  ofdm_symbol_range allowed_symbols;
  /// Maximum number of RBs the slice allows for this grant (caps adaptive retx search).
  unsigned max_rbs = MAX_NOF_PRBS;
};

/// \brief Status of a UE grant allocation, and action for the scheduler policy to follow afterwards.
///
/// The current status are:
/// - success - the allocation was successful with the provided parameters.
/// - skip_slot - failure to allocate and the scheduler policy should terminate the current slot processing.
/// - skip_ue - failure to allocate and the scheduler policy should move on to the next candidate UE.
/// - invalid_params - failure to allocate and the scheduler policy should try a different set of grant parameters.
enum class alloc_status { success, skip_slot, skip_ue, invalid_params };

/// \brief Status of a UE DL grant allocation, and action for the scheduler policy to follow afterwards.
///
/// The current status are:
/// - other - failure to allocate grant for reasons other than failure to allocate PDCCH or UCI.
/// - skip_slot - failure to allocate and the scheduler policy should terminate the current slot processing.
/// - pdcch_alloc_failed - failure to allocate a PDCCH for the given UE.
/// - uci_alloc_failed - failure to allocate a UCI for the given UE.
enum class dl_alloc_failure_cause { other, skip_slot, pdcch_alloc_failed, uci_alloc_failed };

/// \brief This class implements the methods to allocate PDCCH, UCI, PDSCH and PUSCH PDUs in the cell resource grid for
/// UE grants.
class ue_cell_grid_allocator
{
  // Parameters of a DL grant using Rel-16 PDSCH repetitions. Occasion 0 is the PDSCH scheduled at the PDSCH slot; the
  // remaining occasions are PDCCH-less copies of the same TB transmitted at PDSCH slot + offset, for each offset in
  // tx_offsets. Occasions falling in slots that cannot carry the PDSCH symbols are dropped, losing their RV.
  struct dl_repetition_info {
    // Nominal number of repetitions (window of consecutive slots).
    uint8_t nof_occasions;
    // Slot offsets (> 0) of the repetition occasions that are actually transmitted.
    static_vector<uint8_t, 15> tx_offsets;
  };

  // Information relative to a pending DL grant for this slot.
  struct dl_grant_info {
    const slice_ue*                   user;
    sched_helper::dl_sched_context    cfg;
    dl_harq_process_handle            h_dl;
    pdcch_dl_information*             pdcch;
    dl_msg_alloc*                     pdsch;
    uci_allocation                    uci_alloc;
    std::optional<dl_repetition_info> reps;
  };

  // Parameters of a UL grant using Rel-16 PUSCH repetitions. Occasion 0 is the PUSCH at the PUSCH slot, the rest are
  // PDCCH-less PDUs of the same grant.
  struct ul_repetition_info {
    // Number of repetitions in the bundle.
    uint8_t nof_occasions;
    // Slot offsets (> 0) of the repetition occasions, relative to the base occasion's slot.
    static_vector<uint8_t, 15> tx_offsets;
    // \brief Number of HARQ-ACK bits the UE reports in each slot of the bundle that carries any, or 0 if no slot
    // does. Every such slot has the same count, which is what makes one UL DAI describe them all.
    unsigned harq_ack_bits_per_slot;
  };

  // Information relative to a pending UL grant for this slot.
  struct ul_grant_info {
    const slice_ue*                   user;
    sched_helper::ul_sched_context    cfg;
    ul_harq_process_handle            h_ul;
    pdcch_ul_information*             pdcch;
    ul_sched_info*                    pusch;
    std::optional<ul_repetition_info> reps;
  };

public:
  /// \brief List of slots where a PDSCH repetition occasion was actually committed to the resource grid. This is a
  /// subset of the nominal repetition window, as an occasion can be dropped if it collides with another grant
  /// already placed in that (future) slot. Empty when the associated grant does not use repetitions.
  /// \remark Exposed so the caller can register the RBs of these occasions against the RAN slice's per-slot budget
  /// for the corresponding (future) slots, which the direct grant's own slice bookkeeping does not cover.
  using dl_repetition_occasion_list = static_vector<slot_point, 15>;

  /// Result of a successful reTx DL grant allocation.
  struct dl_retx_grant_result {
    /// Allocated VRBs for the (direct) reTx grant.
    vrb_alloc vrbs;
    /// Slots of any PDSCH repetition occasions actually committed to the resource grid (see \ref
    /// dl_repetition_occasion_list).
    dl_repetition_occasion_list repetition_slots;
  };

  /// \brief Slots where a PUSCH repetition occasion was committed to the resource grid. Unlike \ref
  /// dl_repetition_occasion_list, this lists every occasion of the bundle. Empty without repetitions.
  using ul_repetition_occasion_list = static_vector<slot_point, 15>;

  /// Result of a successful reTx UL grant allocation.
  struct ul_retx_grant_result {
    /// Allocated VRBs for the (direct) reTx grant.
    vrb_interval vrbs;
    /// Slots of any PUSCH repetition occasions committed to the resource grid (see \ref ul_repetition_occasion_list).
    ul_repetition_occasion_list repetition_slots;
  };

  /// \brief Interface for a DL grant, which allows deferred setting of PDSCH parameters.
  class dl_newtx_grant_builder
  {
  public:
    dl_newtx_grant_builder(dl_newtx_grant_builder&&) noexcept            = default;
    dl_newtx_grant_builder& operator=(dl_newtx_grant_builder&&) noexcept = default;
    ~dl_newtx_grant_builder() { ocudu_assert(parent == nullptr, "PDSCH parameters were not set"); }

    /// \brief Sets the final VRBs for the PDSCH allocation. Returns the slots of any PDSCH repetition occasions that
    /// were actually committed to the resource grid (see \ref dl_repetition_occasion_list).
    dl_repetition_occasion_list set_pdsch_params(const vrb_alloc& alloc_vrbs, bool enable_interleaving);

    /// \brief Returns the recommended VRBs of the PDSCH grant, of resource allocation type 0 or 1.
    ///
    /// \param[in] max_nof_rbs Maximum number of VRBs of the grant.
    /// \param[in] nof_rbs_remaining Hard limit on the VRBs of a type 0 grant, which may exceed \c max_nof_rbs to fit
    /// whole RBGs.
    vrb_alloc recommended_vrbs(const vrb_bitmap& used_vrbs,
                               unsigned          max_nof_rbs,
                               unsigned          nof_rbs_remaining,
                               bool              interleaving_enabled) const
    {
      const dl_grant_info& grant = grant_info();
      return compute_newtx_dl_vrbs(grant.cfg,
                                   grant.user->get_cc().cfg().search_space(grant.cfg.ss_id),
                                   used_vrbs,
                                   interleaving_enabled,
                                   max_nof_rbs,
                                   nof_rbs_remaining);
    }

    /// Getters for grant immutable parameters.
    const slice_ue&                       ue() const { return *grant_info().user; }
    const sched_helper::dl_sched_context& context() const { return grant_info().cfg; }
    units::bytes                          pending_bytes() const { return grant_info().cfg.pending_bytes; }

  private:
    friend class ue_cell_grid_allocator;

    dl_newtx_grant_builder(ue_cell_grid_allocator& parent_, unsigned grant_index_) :
      parent(&parent_), grant_index(grant_index_)
    {
    }

    const dl_grant_info& grant_info() const { return parent->dl_grants[grant_index]; }

    std::unique_ptr<ue_cell_grid_allocator, noop_operation> parent;
    unsigned                                                grant_index;
  };

  /// \brief Interface for a UL grant, which allows deferred setting of PUSCH parameters.
  class ul_newtx_grant_builder
  {
  public:
    ul_newtx_grant_builder(ul_newtx_grant_builder&&) noexcept            = default;
    ul_newtx_grant_builder& operator=(ul_newtx_grant_builder&&) noexcept = default;
    ~ul_newtx_grant_builder() { ocudu_assert(parent == nullptr, "PUSCH parameters were not set"); }

    /// \brief Sets the final VRBs for the PUSCH allocation. Returns the slots of any PUSCH repetition occasions that
    /// were committed to the resource grid (see \ref ul_repetition_occasion_list).
    ul_repetition_occasion_list set_pusch_params(const vrb_interval& alloc_vrbs);

    /// For a given max number of RBs and a bitmap of used VRBs, returns the recommended parameters for the PUSCH grant.
    vrb_interval recommended_vrbs(const vrb_bitmap& used_vrbs, unsigned max_nof_rbs = MAX_NOF_PRBS) const
    {
      const ul_grant_info& grant = grant_info();
      if (grant.reps.has_value()) {
        // The picked VRBs are repeated on every occasion of the bundle, so they must be free in all of its slots.
        const vrb_bitmap bundle_used = parent->bundle_used_vrbs(used_vrbs, *grant.user, grant.cfg, *grant.reps);
        return compute_newtx_ul_vrbs(grant.cfg, bundle_used, max_nof_rbs);
      }
      return compute_newtx_ul_vrbs(grant.cfg, used_vrbs, max_nof_rbs);
    }

    /// Getters for grant immutable parameters.
    const slice_ue&                       ue() const { return *grant_info().user; }
    const sched_helper::ul_sched_context& context() const { return grant_info().cfg; }
    units::bytes                          pending_bytes() const { return grant_info().cfg.pending_bytes; }

  private:
    friend class ue_cell_grid_allocator;

    ul_newtx_grant_builder(ue_cell_grid_allocator& parent_, unsigned grant_index_) :
      parent(&parent_), grant_index(grant_index_)
    {
    }

    const ul_grant_info& grant_info() const { return parent->ul_grants[grant_index]; }

    std::unique_ptr<ue_cell_grid_allocator, noop_operation> parent;
    unsigned                                                grant_index;
  };

  ue_cell_grid_allocator(const scheduler_ue_expert_config& expert_cfg_,
                         ue_repository&                    ues_,
                         pdcch_resource_allocator&         pdcch_sched_,
                         uci_allocator&                    uci_alloc_,
                         srs_allocator&                    srs_alloc_,
                         cell_resource_allocator&          cell_alloc_,
                         ocudulog::basic_logger&           logger_);

  /// Allocate PDCCH, UCI and PDSCH PDUs for a UE DL grant and return a builder to set the PDSCH parameters.
  expected<dl_newtx_grant_builder, dl_alloc_failure_cause> allocate_dl_grant(const ue_newtx_dl_grant_request& request);

  /// Allocates DL grant for a UE HARQ reTx.
  expected<dl_retx_grant_result, dl_alloc_failure_cause>
  allocate_dl_grant(const ue_retx_dl_grant_request& request) const;

  /// Allocate PDCCH, UCI and PUSCH PDUs for a UE UL grant and return a builder to set the PUSCH parameters.
  expected<ul_newtx_grant_builder, alloc_status> allocate_ul_grant(const ue_newtx_ul_grant_request& request);

  /// Allocates UL grant for a UE HARQ reTx.
  expected<ul_retx_grant_result, alloc_status> allocate_ul_grant(const ue_retx_ul_grant_request& request) const;

  /// \brief Called at the end of a slot to process the allocations that took place and make some final adjustments.
  ///
  /// In particular, this function can redimension the existing grants to fill the remaining RBs if it deems necessary.
  void post_process_results();

private:
  // Setup DL grant builder.
  expected<dl_grant_info, dl_alloc_failure_cause>
  setup_dl_grant_builder(const slice_ue&                       user,
                         const sched_helper::dl_sched_context& params,
                         std::optional<dl_harq_process_handle> h_dl,
                         std::optional<dl_repetition_info>     reps = std::nullopt) const;

  // Builds the PDSCH repetition bundle for a grant whose selected TDRA row is a repetition row: computes the
  // transmitted occasions within the window and decides whether the bundle can start in this slot. Returns nullopt
  // when it cannot, deferring the allocation to a later slot.
  std::optional<dl_repetition_info>
  select_pdsch_repetitions(const ue_cell& ue_cc, const search_space_info& ss_info, uint8_t pdsch_td_res_index) const;

  // Builds the PUSCH repetition bundle for a grant whose TDRA row is a repetition row: computes the occasion slot
  // offsets and whether the bundle can start in this slot. Returns nullopt when it cannot, in which case the caller
  // downgrades the grant to a single transmission.
  std::optional<ul_repetition_info>
  select_pusch_repetitions(const ue_cell& ue_cc, const search_space_info& ss_info, uint8_t pusch_td_res_index) const;

  /// \brief Number of HARQ-ACK bits that a PUSCH repetition bundle will carry.
  ///
  /// The UE multiplexes the UCI onto a single occasion, so every HARQ-ACK booked in any of the bundle's slots rides
  /// on this grant and must be reflected in its size (RBs, MCS/TBS) and UL DAI.
  /// \remark Final once the bundle's slots are known: a PUCCH is refused in a slot already holding an occasion.
  unsigned bundle_uci_harq_bits(rnti_t crnti, slot_point base_slot, const ul_repetition_info& reps) const;

  /// \brief Returns the VRBs used across a whole repetition bundle. The goal is to find unused interval across all
  /// repetitions.
  /// \param[in] base_used_vrbs VRBs already taken in the grant's PUSCH slot.
  /// \param[in] user UE being scheduled.
  /// \param[in] ctxt UL scheduling context of the grant.
  /// \param[in] reps Bundle whose occasion slots are folded in.
  /// \return Bitmap of VRBs used across the bundle.
  vrb_bitmap bundle_used_vrbs(const vrb_bitmap&                     base_used_vrbs,
                              const slice_ue&                       user,
                              const sched_helper::ul_sched_context& ctxt,
                              const ul_repetition_info&             reps) const;

  // Setup UL grant builder.
  expected<ul_grant_info, alloc_status>
  setup_ul_grant_builder(const slice_ue&                       user,
                         const sched_helper::ul_sched_context& params,
                         std::optional<ul_harq_process_handle> h_ul,
                         std::optional<ul_repetition_info>     reps = std::nullopt) const;

  // Set final PDSCH parameters and allocate remaining DL grant resources. Returns the slots of any PDSCH repetition
  // occasions actually committed to the resource grid.
  dl_repetition_occasion_list
  set_pdsch_params(dl_grant_info& grant, const vrb_alloc& vrbs, bool enable_interleaving) const;

  // Set final PUSCH parameters and allocate remaining UL grant resources. Returns the committed occasion slots.
  ul_repetition_occasion_list set_pusch_params(ul_grant_info& grant, const vrb_interval& vrbs) const;

  std::optional<sch_mcs_tbs> calculate_dl_mcs_tbs(const cell_slot_resource_allocator& pdsch_alloc,
                                                  const search_space_info&            ss_info,
                                                  uint8_t                             pdsch_td_res_index,
                                                  const prb_bitmap&                   prbs,
                                                  sch_mcs_index                       mcs,
                                                  unsigned                            nof_layers) const;

  expected<pdcch_dl_information*, alloc_status> alloc_dl_pdcch(const ue_cell&           ue_cc,
                                                               const search_space_info& ss_info) const;

  std::optional<uci_allocation> alloc_uci(const ue_cell&           ue_cc,
                                          const search_space_info& ss_info,
                                          uint8_t                  pdsch_td_res_index,
                                          unsigned                 last_occasion_offset = 0) const;

  const scheduler_ue_expert_config& expert_cfg;
  ue_repository&                    ues;
  pdcch_resource_allocator&         pdcch_sched;
  uci_allocator&                    uci_alloc;
  srs_allocator&                    srs_alloc;
  cell_resource_allocator&          cell_alloc;
  ocudulog::basic_logger&           logger;

  std::vector<dl_grant_info> dl_grants;
  std::vector<ul_grant_info> ul_grants;
};

} // namespace ocudu
