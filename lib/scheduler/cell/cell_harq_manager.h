// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../slicing/ran_slice_id.h"
#include "ocudu/adt/fixed_bitset.h"
#include "ocudu/adt/intrusive_list.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/csi_report/csi_report_data.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/ran/harq_id.h"
#include "ocudu/ran/logical_channel/lcid_dl_sch.h"
#include "ocudu/ran/pdsch/pdsch_mcs.h"
#include "ocudu/ran/pusch/pusch_mcs.h"
#include "ocudu/ran/slot_pdu_capacity_constants.h"
#include "ocudu/ran/slot_point.h"
#include "ocudu/scheduler/result/dci_info.h"
#include "ocudu/scheduler/result/vrb_alloc.h"
#include "ocudu/support/memory_pool/free_list_recycling_object_pool.h"

namespace ocudu {

struct dl_msg_alloc;
struct pusch_information;

class unique_ue_harq_entity;

/// \brief Notifier of HARQ process timeouts.
class harq_timeout_notifier
{
public:
  virtual ~harq_timeout_notifier() = default;

  /// \brief Notifies that a HARQ-ACK/CRC feedback indication was never received from the lower layers.
  virtual void on_feedback_timeout(du_ue_index_t ue_idx, bool is_dl, bool ack) = 0;

  /// \brief Notifies that a HARQ process with a pending retransmission was never rescheduled in time.
  virtual void on_retx_timeout(du_ue_index_t ue_idx, bool is_dl) = 0;

  /// \brief Notifies a timeout for Feedback Disabled HARQ.
  virtual void on_feedback_disabled_harq_timeout(du_ue_index_t ue_idx, bool is_dl, units::bytes tbs) = 0;
};

/// Parameters for the allocation of a UL HARQ process reserved for Configured Grants.
struct cg_harq_alloc_params {
  /// HARQ process ID reserved for the CG occasion.
  harq_id_t harq_id;
  /// Timeout, in slots, after which the CG HARQ process is released (configured_grant_timer x periodicity).
  unsigned cg_harq_timeout;
};

namespace harq_utils {

/// Possible states of a HARQ process.
enum class harq_state_t : uint8_t { empty, pending_retx, waiting_ack };

/// HARQ operation modes as per TS 38.300, Section 16.14.2.
enum class harq_mode_t : uint8_t {
  normal,                     // Standard Stop-and-Wait Operation (NTN: DL Feedback Enabled, UL Mode A)
  feedback_disabled_or_mode_b // Re-use allowed before RTT elapses (NTN: DL Feedback Disabled, UL Mode B)
};

/// Returns the DL HARQ feedback a process in the given mode carries.
inline dl_harq_feedback to_dl_harq_feedback(harq_mode_t mode)
{
  return mode == harq_mode_t::normal ? dl_harq_feedback::enabled : dl_harq_feedback::disabled;
}

/// \brief Returns the mode a process has to operate in to carry the given DL HARQ feedback.
///
/// An unset feedback leaves the transport block unrestricted, which the feedback disabled mode serves.
inline harq_mode_t to_harq_mode(const std::optional<dl_harq_feedback>& feedback)
{
  return feedback == dl_harq_feedback::enabled ? harq_mode_t::normal : harq_mode_t::feedback_disabled_or_mode_b;
}

/// \brief Returns the mode a process has to operate in to serve a grant using the given UL HARQ mode.
///
/// An unset mode leaves the grant unrestricted, which mode B serves.
inline harq_mode_t to_harq_mode(const std::optional<ul_harq_mode>& mode)
{
  return mode == ul_harq_mode::mode_a ? harq_mode_t::normal : harq_mode_t::feedback_disabled_or_mode_b;
}

struct pending_retx_list_tag;

/// Parameters that are common to DL and UL HARQ processes.
struct base_harq_process : public intrusive_double_linked_list_element<>,
                           public intrusive_double_linked_list_element<pending_retx_list_tag> {
  du_ue_index_t ue_idx;
  rnti_t        rnti;
  harq_id_t     h_id;
  harq_state_t  status = harq_state_t::empty;
  harq_mode_t   mode   = harq_mode_t::normal;
  /// Slot at which the PxSCH was transmitted.
  slot_point slot_tx;
  /// \brief Slot of the last occasion of the transmission starting at \c slot_tx. Equal to \c slot_tx, unless Rel-16
  /// PDSCH repetitions were used, in which case it is \c slot_tx + the number of repetitions - 1.
  slot_point last_occasion_slot;
  /// Slot at which the respective ACK/CRC is expected to be received in the PHY.
  slot_point slot_ack;
  /// \brief Slot of the last transmission that can carry the respective ACK/CRC.
  ///
  /// It only differs from \c slot_ack when the HARQ-ACK is reported over a multi-slot PUCCH repetition burst (see
  /// TS 38.213, Section 9.2.6), in which case it is the slot of the burst's last repetition. The feedback timeout is
  /// counted from this slot.
  slot_point slot_ack_end;
  /// \brief Slot at which the currently set timeout expires. In case of status == waiting_ack, the timeout expires
  /// if no ACK/CRC arrives. In case of status == pending_retx, the timeout expires if no new reTx is scheduled.
  slot_point slot_timeout;
  /// New Data Indicator. Its value should flip for every new Tx.
  bool ndi = false;
  /// Whether retransmissions for this HARQ process have been cancelled.
  bool retxs_cancelled = false;
  /// Number of retransmissions that took place for the current Transport Block.
  uint8_t nof_retxs = 0;
  /// Maximum number of retransmission before Transport Block is reset.
  uint8_t max_nof_harq_retxs = 0;
};

/// Parameters of a DL HARQ process.
struct dl_harq_process_impl : public base_harq_process {
  /// \brief Parameters relative to the last used PDSCH PDU that get stored in the HARQ process for future reuse.
  struct alloc_params {
    struct lc_alloc_info {
      lcid_dl_sch_t lcid;
      units::bytes  sched_bytes;
    };

    dci_dl_rnti_config_type dci_cfg_type;
    vrb_alloc               rbs;
    uint8_t                 nof_symbols;
    uint8_t                 nof_layers{1};
    /// Number of Rel-16 slot-based PDSCH repetitions of the grant. Value 1 means a single transmission.
    uint8_t                                     nof_repetitions{1};
    bool                                        is_fallback{false};
    cqi_value                                   cqi;
    pdsch_mcs_table                             mcs_table;
    sch_mcs_index                               mcs;
    units::bytes                                tbs;
    static_vector<lc_alloc_info, MAX_LC_PER_TB> lc_sched_info;
    /// RAN slice identifier.
    std::optional<ran_slice_id_t> slice_id;
    /// \brief MCS originally suggested by the OLLA. It might differ from the actual MCS used.
    std::optional<sch_mcs_index> olla_mcs;
  };

  /// Parameters used for the last Tx of this HARQ process.
  alloc_params prev_tx_params;
  /// HARQ-bit index corresponding to this HARQ process in the UCI PDU indication.
  uint8_t harq_bit_idx = 0;
  /// Stores the highest recorded PUCCH SNR for this HARQ process.
  std::optional<float> last_pucch_snr;
};

/// Parameters of a UL HARQ process.
struct ul_harq_process_impl : public base_harq_process {
  /// \brief Parameters relative to the last allocated PUSCH PDU for this HARQ process.
  struct alloc_params {
    dci_ul_rnti_config_type dci_cfg_type;
    vrb_alloc               rbs;
    pusch_mcs_table         mcs_table;
    sch_mcs_index           mcs;
    units::bytes            tbs;
    uint8_t                 nof_symbols;
    uint8_t                 nof_layers;
    /// Number of Rel-16 PUSCH repetitions of the original transmission. Value 1 means a single transmission.
    uint8_t                       nof_repetitions{1};
    std::optional<ran_slice_id_t> slice_id;
    std::optional<sch_mcs_index>  olla_mcs;
    /// Whether the HARQ process was allocated for a Configured Grant PUSCH. Fixed across HARQ retxs.
    bool is_cg = false;
  };

  /// Parameters used for the last Tx of this HARQ process.
  alloc_params prev_tx_params;
  /// \brief Decode result accumulated over the occasions of the current transmission, as a logical OR.
  ///
  /// A bundle draws one CRC report per occasion and the PHY keeps accumulating soft bits, so an occasion may decode
  /// the TB before the last one while a single failure says nothing. OR-ing keeps the successful report, whichever
  /// occasion it came from. Reset at the start of every transmission.
  bool combined_crc = false;
};

class ntn_dl_harq_alloc_history;
class ntn_ul_harq_alloc_history;

template <bool IsDl>
struct cell_harq_repository {
  using harq_type          = std::conditional_t<IsDl, dl_harq_process_impl, ul_harq_process_impl>;
  using harq_alloc_history = std::conditional_t<IsDl, ntn_dl_harq_alloc_history, ntn_ul_harq_alloc_history>;

  struct ue_harq_entity_impl {
    std::vector<harq_type> harqs;
    /// Processes that are free.
    fixed_size_bitset<MAX_NOF_HARQS> free_harqs;
    /// Mode of each process, a set bit standing for \c harq_mode_t::normal.
    fixed_size_bitset<MAX_NOF_HARQS> harq_mode_mask;
    slot_point                       last_slot_tx;
    slot_point                       last_slot_ack;
    /// NTN helper variable indicating the presence of HARQ process with feedback disabled or mode B.
    bool feedback_disabled_or_mode_b_harq_present = false;
    /// First HARQ-id that is not reserved for Configured Grant use.
    harq_id_t first_non_reserved_harq_id = to_harq_id(0);

    // ue_harq_entity_impl objects are recycled to avoid losing the allocated vector memory.
    void clear()
    {
      harqs.clear();
      free_harqs.reset();
      harq_mode_mask.reset();
      last_slot_tx                             = slot_point{};
      last_slot_ack                            = slot_point{};
      feedback_disabled_or_mode_b_harq_present = false;
      first_non_reserved_harq_id               = to_harq_id(0);
    }
  };

  using ue_entity_pool_type = free_list_recycling_object_pool<ue_harq_entity_impl>;
  using ue_entity_pool_ptr  = typename ue_entity_pool_type::ptr;

  cell_harq_repository(unsigned                max_nof_ue_indexes,
                       unsigned                max_nof_ue_contexts,
                       unsigned                max_ack_wait_in_slots,
                       unsigned                harq_retx_timeout,
                       unsigned                max_harqs_per_ue,
                       unsigned                ntn_ul_alloc_delay_slots,
                       bool                    harq_mode_b,
                       harq_timeout_notifier&  timeout_notifier_,
                       ocudulog::basic_logger& logger_);
  ~cell_harq_repository();

  /// Time interval, in slots, before the HARQ process assumes that the ACK/CRC went missing.
  const unsigned max_ack_wait_in_slots;
  /// Maximum number of slots before a HARQ with pending retransmission is discarded.
  const unsigned harq_retx_timeout;
  /// Maximum number of HARQs allowed per UE.
  const unsigned          max_harqs_per_ue;
  harq_timeout_notifier&  timeout_notifier;
  ocudulog::basic_logger& logger;

  slot_point last_sl_ind;

  // Note: Declared before the list of UEs, as the entities handed out to the UEs belong to it.
  ue_entity_pool_type                                            ue_entity_pool;
  std::vector<ue_entity_pool_ptr>                                ues;
  intrusive_double_linked_list<harq_type, pending_retx_list_tag> harq_pending_retx_list;
  std::vector<intrusive_double_linked_list<harq_type>>           harq_timeout_wheel;
  unsigned                                                       ntn_ul_alloc_delay_slots;
  std::unique_ptr<harq_alloc_history>                            alloc_hist;

  void slot_indication(slot_point sl_tx);
  void stop();
  void handle_harq_ack_timeout(harq_type& h, slot_point sl_tx);
  /// \brief Allocates a new HARQ process for a UE's new Tx.
  /// \param[in] ue_idx Index of the UE requesting the allocation.
  /// \param[in] sl_tx Slot of the PxSCH transmission.
  /// \param[in] sl_ack Slot at which the respective ACK/CRC is expected to be received in the PHY (see \c
  /// base_harq_process::slot_ack).
  /// \param[in] sl_ack_end Slot of the last transmission that can carry the respective ACK/CRC; equal to \c sl_ack
  /// unless the HARQ-ACK is reported over a multi-slot PUCCH repetition burst (see \c base_harq_process::slot_ack_end).
  /// \param[in] max_nof_harq_retxs Maximum number of retransmissions allowed for this HARQ process before it is reset.
  /// \param[in] cg_params If set, forces the allocation of this specific HARQ-id (e.g. for Configured Grants); the
  /// process must be free and, unless \c mode's constraint below applies, not reserved.
  /// \param[in] mode Mode the picked HARQ process must be currently operating in. Only relevant for NTN cells.
  /// \param[in] nof_repetitions Number of consecutive slot-equivalents spanned by this transmission, starting at
  /// \c sl_tx. Used solely to extend the UE entity's last known Tx slot to the end of the transmission. For DL this
  /// is the literal repetition count; for UL, whose Rel-17 available-slot-counting window may skip DL/special slots,
  /// the caller passes 1 + the last occasion's slot offset, which need not equal the repetition count.
  /// \return Pointer to the allocated HARQ process, or \c nullptr if the UE has no free (and, when applicable, no
  /// matching reserved or mode-matching) HARQ process available.
  harq_type* alloc_harq(du_ue_index_t                       ue_idx,
                        slot_point                          sl_pdcch,
                        slot_point                          sl_tx,
                        slot_point                          sl_ack,
                        slot_point                          sl_ack_end,
                        unsigned                            max_nof_harq_retxs,
                        std::optional<cg_harq_alloc_params> cg_params       = std::nullopt,
                        harq_mode_t                         mode            = harq_mode_t::normal,
                        uint8_t                             nof_repetitions = 1);
  void       dealloc_harq(harq_type& h);
  void       handle_ack(harq_type& h, bool ack);
  void       set_pending_retx(harq_type& h);
  /// \brief Reuses a HARQ process with a pending retransmission for a UE's new Tx.
  /// \param[in] h HARQ process to retransmit, which must currently have a pending retransmission (see \c
  /// set_pending_retx).
  /// \param[in] sl_tx, sl_ack, sl_ack_end, nof_repetitions See \c alloc_harq.
  /// \return Whether the retransmission was accepted. Fails if \c h has no pending retransmission.
  [[nodiscard]] bool handle_new_retx(harq_type& h,
                                     slot_point sl_tx,
                                     slot_point sl_ack,
                                     slot_point sl_ack_end,
                                     uint8_t    nof_repetitions = 1);
  void               reserve_ue_harqs(du_ue_index_t ue_idx, rnti_t rnti, unsigned nof_harqs);
  void               extend_ue_harqs(du_ue_index_t ue_idx, rnti_t rnti, unsigned new_nof_harqs);
  void               destroy_ue(du_ue_index_t ue_idx);
  void               cancel_retxs(harq_type& h);
  void               cancel_unsent_tx(harq_type& h);
  harq_type*         find_ue_harq_in_state(du_ue_index_t ue_idx, harq_utils::harq_state_t state);
  const harq_type*   find_ue_harq_in_state(du_ue_index_t ue_idx, harq_utils::harq_state_t state) const;
  bool               is_ntn_harq_mode_b_enabled() const;
};

template <bool IsDl>
class base_harq_process_handle
{
protected:
  using harq_pool      = cell_harq_repository<IsDl>;
  using harq_impl_type = std::conditional_t<IsDl, dl_harq_process_impl, ul_harq_process_impl>;

public:
  base_harq_process_handle(harq_pool& pool_, harq_impl_type& h_) : harq_repo(&pool_), impl(&h_) {}

  du_ue_index_t ue_index() const { return impl->ue_idx; }
  rnti_t        rnti() const { return impl->rnti; }
  harq_id_t     id() const { return impl->h_id; }
  bool          is_waiting_ack() const { return impl->status == harq_utils::harq_state_t::waiting_ack; }
  bool          has_pending_retx() const { return impl->status == harq_utils::harq_state_t::pending_retx; }
  bool          empty() const { return impl->status == harq_utils::harq_state_t::empty; }
  unsigned      max_nof_retxs() const { return impl->max_nof_harq_retxs; }
  unsigned      nof_retxs() const { return impl->nof_retxs; }
  bool          ndi() const { return impl->ndi; }
  harq_mode_t   mode() const { return impl->mode; }

  /// \brief Cancels any retransmissions for this HARQ process.
  /// If the HARQ process has a pending retransmission, it is reset. If the ACK/CRC info has not been received yet, the
  /// HARQ process waits for it to arrive before being reset.
  void cancel_retxs();

  /// \brief Releases a HARQ process whose (re)transmission was allocated but whose DCI is not going to be sent.
  /// For a newTx, the NDI toggle done at allocation is undone, so that the next newTx in this HARQ process toggles the
  /// NDI with respect to the last DCI the UE actually received (TS 38.321, clauses 5.3.2.2 and 5.4.2.1). A reTx did
  /// not toggle the NDI. Ignored if the HARQ process is not waiting for an ACK.
  void cancel_unsent_tx();

  /// \brief Releases a HARQ process whose DCI was sent, dropping its transmission (e.g. discarded by the lower layers).
  /// The NDI is kept as the UE received it, so that the next newTx in this HARQ process toggles it and the UE takes it
  /// as new data (TS 38.321, clauses 5.3.2.2 and 5.4.2.1).
  void discard_sent_tx();

  bool operator==(const base_harq_process_handle& other) const
  {
    return harq_repo == other.harq_repo and impl == other.impl;
  }
  bool operator!=(const base_harq_process_handle& other) const { return !(*this == other); }

protected:
  harq_pool*      harq_repo = nullptr;
  harq_impl_type* impl      = nullptr;
};

} // namespace harq_utils

/// \brief Context of the scheduler during the current PDSCH allocation.
struct dl_harq_alloc_context {
  /// DCI format used to signal the PDSCH allocation.
  dci_dl_rnti_config_type dci_cfg_type;
  /// MCS suggested by the OLLA.
  std::optional<sch_mcs_index> olla_mcs;
  /// RAN slice identifier of the slice to which PDSCH belongs to.
  std::optional<ran_slice_id_t> slice_id;
  /// CQI value at the moment of newTx.
  std::optional<cqi_value> cqi;
  /// Whether the HARQ allocation was done in fallback mode.
  bool is_fallback = false;
  /// Number of Rel-16 slot-based PDSCH repetitions of the grant (1 = single transmission).
  uint8_t nof_repetitions = 1;
};

/// \brief Context of the scheduler during the current PUSCH allocation.
struct ul_harq_alloc_context {
  /// DCI format used to signal the PUSCH allocation.
  dci_ul_rnti_config_type dci_cfg_type;
  /// MCS suggested by the OLLA.
  std::optional<sch_mcs_index> olla_mcs;
  /// RAN slice identifier of the slice to which PUSCH belongs to.
  std::optional<ran_slice_id_t> slice_id;
  /// Number of Rel-16 PUSCH repetitions of the grant (1 = single transmission).
  uint8_t nof_repetitions = 1;
};

/// \brief Interface used to fetch and update the status of a DL HARQ process.
/// \remark This handle acts like a view to an internal HARQ process. It is not a "unique" type that controls the
/// lifetime of a HARQ. Avoid storing and using the same handle across different slots.
class dl_harq_process_handle : public harq_utils::base_harq_process_handle<true>
{
  using base_type = harq_utils::base_harq_process_handle<true>;

public:
  using grant_params = harq_utils::dl_harq_process_impl::alloc_params;

  using base_type::base_type;

  using base_type::empty;
  using base_type::has_pending_retx;
  using base_type::id;
  using base_type::is_waiting_ack;

  using base_type::max_nof_retxs;
  using base_type::ndi;
  using base_type::nof_retxs;

  using base_type::cancel_retxs;

  /// \brief Prepares the DL HARQ process for a new retransmission.
  /// \param[in] pdsch_slot Slot of the PDSCH retransmission.
  /// \param[in] ack_delay Delay, in slots, of the HARQ-ACK report with respect to the PDSCH.
  /// \param[in] harq_bit_idx Bit index of the HARQ-ACK in the UCI indication.
  /// \param[in] nof_repetitions Number of consecutive slots spanned by this transmission, starting at \c pdsch_slot
  /// (Rel-16 PDSCH repetitions; 1 for a single transmission). See \c cell_harq_repository::handle_new_retx.
  /// \param[in] last_ack_delay Delay, in slots, of the last transmission carrying the HARQ-ACK report with respect to
  /// the PDSCH. It only differs from \c ack_delay if the report is repeated over multiple slots. Defaults to
  /// \c ack_delay.
  [[nodiscard]] bool new_retx(slot_point              pdsch_slot,
                              unsigned                ack_delay,
                              uint8_t                 harq_bit_idx,
                              uint8_t                 nof_repetitions = 1,
                              std::optional<unsigned> last_ack_delay  = std::nullopt);

  /// \brief Update the state of the DL HARQ process waiting for an HARQ-ACK.
  /// \param[in] ack HARQ-ACK status received.
  /// \param[in] pucch_snr SNR of the PUCCH that carried the HARQ-ACK.
  /// \return True if successfully updated and false if the HARQ is not in a valid state to be ACKed/NACKed.
  [[nodiscard]] bool dl_ack_info(mac_harq_ack_report_status ack, const std::optional<float>& pucch_snr);

  /// \brief Stores grant parameters that are associated with the HARQ process (e.g. DCI format, PRBs, MCS) so that
  /// they can be later fetched and optionally reused.
  void save_grant_params(const dl_harq_alloc_context& ctx, const dl_msg_alloc& ue_pdsch);

  slot_point pdsch_slot() const { return impl->slot_tx; }
  slot_point uci_slot() const { return impl->slot_ack; }
  /// Slot of the last transmission that can carry the HARQ-ACK report (see \c base_harq_process::slot_ack_end).
  slot_point last_uci_slot() const { return impl->slot_ack_end; }

  const grant_params& get_grant_params() const { return impl->prev_tx_params; }
};

/// Interface used to fetch and update the status of a UL HARQ process.
/// \remark This handle acts like a view to an internal HARQ process. It is not a "unique" type that controls the
/// lifetime of a HARQ. Avoid storing and using the same handle across different slots.
class ul_harq_process_handle : public harq_utils::base_harq_process_handle<false>
{
  using base_type = harq_utils::base_harq_process_handle<false>;

public:
  using grant_params = harq_utils::ul_harq_process_impl::alloc_params;

  using base_type::base_type;

  using base_type::empty;
  using base_type::has_pending_retx;
  using base_type::id;
  using base_type::is_waiting_ack;

  using base_type::max_nof_retxs;
  using base_type::ndi;
  using base_type::nof_retxs;

  using base_type::cancel_retxs;

  /// \param[in] nof_repetitions Number of consecutive slots spanned by this transmission, starting at \c pusch_slot
  /// (Rel-16 PUSCH repetitions; 1 for a single transmission).
  [[nodiscard]] bool new_retx(slot_point pusch_slot, uint8_t nof_repetitions = 1);

  /// Update UL HARQ state given the received CRC indication.
  /// \return Transport Block size of the HARQ whose state was updated.
  expected<units::bytes> ul_crc_info(bool ack);

  /// \brief Stores grant parameters that are associated with the HARQ process (e.g. DCI format, PRBs, MCS) so that
  /// they can be later fetched and optionally reused.
  void save_grant_params(const ul_harq_alloc_context& ctx, const pusch_information& pusch);

  slot_point pusch_slot() const { return impl->slot_tx; }
  /// Slot of the last occasion of this transmission. Equals \c pusch_slot() outside a PUSCH repetition bundle.
  slot_point last_occasion_slot() const { return impl->last_occasion_slot; }
  /// \brief Folds one occasion's decode result into this transmission's accumulated result and returns it.
  ///
  /// Call once per CRC report; the value returned at the last occasion is the one to pass to \c ul_crc_info.
  bool accumulate_crc(bool crc_success)
  {
    impl->combined_crc = impl->combined_crc or crc_success;
    return impl->combined_crc;
  }

  const grant_params& get_grant_params() const { return impl->prev_tx_params; }

  bool is_cg() const { return impl->prev_tx_params.is_cg; }
};

namespace harq_utils {

template <bool IsDl>
class harq_pending_retx_list_impl
{
  using harq_pool      = cell_harq_repository<IsDl>;
  using harq_impl_type = std::conditional_t<IsDl, dl_harq_process_impl, ul_harq_process_impl>;
  using handle_type    = std::conditional_t<IsDl, dl_harq_process_handle, ul_harq_process_handle>;
  using harq_impl_it_t = typename intrusive_double_linked_list<harq_impl_type, pending_retx_list_tag>::iterator;

public:
  struct iterator {
    using value_type        = handle_type;
    using reference         = handle_type;
    using pointer           = std::optional<value_type>;
    using iterator_category = std::forward_iterator_tag;

    iterator(harq_pool& pool_, harq_impl_it_t it_) : pool(&pool_), it(it_) {}

    iterator& operator++()
    {
      ++it;
      return *this;
    }
    iterator operator++(int)
    {
      iterator ret{*this};
      ++it;
      return ret;
    }
    reference operator*()
    {
      ocudu_assert(it != pool->harq_pending_retx_list.end(), "Dereferencing list end()");
      return handle_type{*pool, *it};
    }
    pointer operator->()
    {
      if (it != pool->harq_pending_retx_list.end()) {
        return handle_type{*pool, *it};
      }
      return std::nullopt;
    }

    bool operator==(const iterator& other) const { return pool == other.pool and it == other.it; }
    bool operator!=(const iterator& other) const { return !(*this == other); }

  private:
    harq_pool*     pool;
    harq_impl_it_t it;
  };

  using value_type      = handle_type;
  using difference_type = std::ptrdiff_t;

  explicit harq_pending_retx_list_impl(harq_pool& pool_) : pool(&pool_) {}

  bool empty() const { return pool->harq_pending_retx_list.empty(); }

  iterator begin() { return iterator{*pool, pool->harq_pending_retx_list.begin()}; }
  iterator end() { return iterator{*pool, pool->harq_pending_retx_list.end()}; }

private:
  harq_pool* pool;
};

} // namespace harq_utils

/// List of HARQ processes with pending retransmissions
using dl_harq_pending_retx_list = harq_utils::harq_pending_retx_list_impl<true>;
using ul_harq_pending_retx_list = harq_utils::harq_pending_retx_list_impl<false>;

/// Manager of all HARQ processes in a given cell.
class cell_harq_manager
{
public:
  /// \brief Default timeout in slots after which the HARQ process assumes that the CRC/ACK went missing
  /// (implementation-defined).
  static constexpr unsigned DEFAULT_ACK_TIMEOUT_SLOTS = 256U;

  /// \brief Default timeout in slots for HARQ to be scheduled for retransmission after a negative CRC/ACK.
  static constexpr unsigned DEFAULT_HARQ_RETX_TIMEOUT_SLOTS = 200U;

  cell_harq_manager(unsigned                               max_nof_ue_indexes,
                    unsigned                               max_nof_ue_contexts,
                    unsigned                               max_harqs_per_ue,
                    std::unique_ptr<harq_timeout_notifier> dl_notifier              = nullptr,
                    std::unique_ptr<harq_timeout_notifier> ul_notifier              = nullptr,
                    unsigned                               dl_harq_retx_timeout     = DEFAULT_HARQ_RETX_TIMEOUT_SLOTS,
                    unsigned                               ul_harq_retx_timeout     = DEFAULT_HARQ_RETX_TIMEOUT_SLOTS,
                    unsigned                               max_ack_wait_timeout     = DEFAULT_ACK_TIMEOUT_SLOTS,
                    unsigned                               ntn_ul_alloc_delay_slots = 0,
                    bool                                   ul_harq_mode_b           = false);

  /// Update slot, and checks if there are HARQ processes that have reached maxReTx with no ACK
  void slot_indication(slot_point sl_tx);

  /// Called on cell deactivation.
  void stop();

  /// Create new UE HARQ entity.
  /// \param ue_idx Index of the UE.
  /// \param crnti C-RNTI of the UE.
  /// \param nof_dl_harq_procs Number of DL HARQ processes that the UE can support. This value is derived based on
  /// the UE capabilities, and passed to the UE via RRC signalling. See TS38.331, "nrofHARQ-ProcessesForPDSCH".
  /// Values: {2, 4, 6, 10, 12, 16, 32}.
  /// \param nof_ul_harq_procs Number of UL HARQ processes that gNB can support. This value is implementation-defined
  /// and can up to 32 (there are up to 4 bits for HARQ-Id signalling, up to 5 bits in an NTN cell).
  unique_ue_harq_entity add_ue(du_ue_index_t ue_idx,
                               rnti_t        crnti,
                               unsigned      nof_dl_harq_procs = MAX_NOF_HARQS,
                               unsigned      nof_ul_harq_procs = MAX_NOF_HARQS);

  /// Checks whether a UE with the provided ue index exists.
  bool contains(du_ue_index_t ue_idx) const;

  /// Retrieve list of HARQ processes with pending retxs.
  dl_harq_pending_retx_list pending_dl_retxs();
  ul_harq_pending_retx_list pending_ul_retxs();

private:
  friend class unique_ue_harq_entity;

  void destroy_ue(du_ue_index_t ue_idx);

  /// \brief Called on every DL new Tx to allocate an DL HARQ process.
  harq_utils::dl_harq_process_impl* new_dl_tx(du_ue_index_t           ue_idx,
                                              rnti_t                  rnti,
                                              slot_point              pdcch_slot,
                                              slot_point              pdsch_slot,
                                              unsigned                ack_delay,
                                              unsigned                last_ack_delay,
                                              unsigned                max_harq_nof_retxs,
                                              uint8_t                 harq_bit_idx,
                                              harq_utils::harq_mode_t mode            = harq_utils::harq_mode_t::normal,
                                              uint8_t                 nof_repetitions = 1);

  /// \brief Called on every UL new Tx to allocate an UL HARQ process.
  harq_utils::ul_harq_process_impl* new_ul_tx(du_ue_index_t                       ue_idx,
                                              rnti_t                              rnti,
                                              slot_point                          pdcch_slot,
                                              slot_point                          pusch_slot,
                                              unsigned                            max_harq_nof_retxs,
                                              std::optional<cg_harq_alloc_params> cg_params = std::nullopt,
                                              harq_utils::harq_mode_t mode            = harq_utils::harq_mode_t::normal,
                                              uint8_t                 nof_repetitions = 1);

  const uint8_t                          max_harqs_per_ue;
  std::unique_ptr<harq_timeout_notifier> dl_timeout_notifier;
  std::unique_ptr<harq_timeout_notifier> ul_timeout_notifier;
  ocudulog::basic_logger&                logger;

  harq_utils::cell_harq_repository<true>  dl;
  harq_utils::cell_harq_repository<false> ul;
};

/// HARQ entity that manages a set of HARQ processes of a single UE.
class unique_ue_harq_entity
{
  using dl_harq_ent_impl = harq_utils::cell_harq_repository<true>::ue_harq_entity_impl;
  using ul_harq_ent_impl = harq_utils::cell_harq_repository<false>::ue_harq_entity_impl;

public:
  unique_ue_harq_entity() = default;
  unique_ue_harq_entity(cell_harq_manager* mgr, du_ue_index_t ue_idx, rnti_t crnti_);
  ~unique_ue_harq_entity();
  unique_ue_harq_entity(const unique_ue_harq_entity&) = delete;
  unique_ue_harq_entity(unique_ue_harq_entity&& other) noexcept;
  unique_ue_harq_entity& operator=(const unique_ue_harq_entity&) = delete;
  unique_ue_harq_entity& operator=(unique_ue_harq_entity&& other) noexcept;

  bool empty() const { return cell_harq_mgr == nullptr; }

  /// Gets the maximum number of HARQ processes a UE can use, which depends on its configuration.
  unsigned nof_dl_harqs() const { return get_dl_ue().harqs.size(); }
  unsigned nof_ul_harqs() const { return get_ul_ue().harqs.size(); }

  /// \brief Number of UL HARQ processes reserved for Configured Grants, as set by the last reconfigure().
  ///
  /// These are the HARQ processes with the lowest ids; only a Configured Grant allocation may use them.
  unsigned nof_cg_reserved_ul_harqs() const { return static_cast<unsigned>(first_non_reserved_harq_id); }

  /// \brief Reconfigure HARQ entity with new settings.
  /// \param new_nof_dl_harqs Number of DL HARQ processes.
  /// \param new_nof_ul_harqs Number of UL HARQ processes.
  /// \param dl_harq_feedback_disabled_mask Bitmask for DL HARQ feedback disabled mode.
  /// \param ul_harq_mode_mask Bitmask for UL HARQ mode (mode A vs mode B).
  /// \param nof_cg_reserved_harqs Number of UL HARQ processes reserved for Configured Grants.
  void reconfigure(unsigned                              new_nof_dl_harqs,
                   unsigned                              new_nof_ul_harqs,
                   const harq_dl_feedback_disabled_mask& dl_harq_feedback_disabled_mask,
                   const harq_ul_mode_mask&              ul_harq_mode_mask,
                   unsigned                              nof_cg_reserved_harqs);

  /// Checks whether \c alloc_dl_harq() picks a free process by its mode, rather than taking any of them.
  bool is_dl_harq_mode_selective() const
  {
    return cell_harq_mgr->dl.ntn_ul_alloc_delay_slots > 0 and get_dl_ue().feedback_disabled_or_mode_b_harq_present;
  }

  /// Checks whether \c alloc_ul_harq() picks a free process by its mode, rather than taking any of them.
  bool is_ul_harq_mode_selective() const
  {
    return cell_harq_mgr->ul.ntn_ul_alloc_delay_slots > 0 and get_ul_ue().feedback_disabled_or_mode_b_harq_present;
  }

  /// \brief Checks whether there are free HARQ processes.
  /// \param[in] mode When set, only the free processes a grant in this mode can be built on are considered.
  bool   has_empty_dl_harqs(std::optional<harq_utils::harq_mode_t> mode = std::nullopt) const;
  bool   has_empty_ul_harqs(std::optional<harq_utils::harq_mode_t> mode = std::nullopt) const;
  size_t nof_empty_dl_harqs(std::optional<harq_utils::harq_mode_t> mode = std::nullopt) const;
  size_t nof_empty_ul_harqs(std::optional<harq_utils::harq_mode_t> mode = std::nullopt) const;

  /// Check the last HARQ allocations for the given UE.
  slot_point last_pdsch_slot() const { return get_dl_ue().last_slot_tx; }
  slot_point last_ack_slot() const { return get_dl_ue().last_slot_ack; }
  slot_point last_pusch_slot() const { return get_ul_ue().last_slot_tx; }
  /// Returns the last slot granted to the UE for uplink transmission (PUSCH or HARQ-ACK). Invalid if none.
  slot_point last_ul_slot() const
  {
    const slot_point ack   = last_ack_slot();
    const slot_point pusch = last_pusch_slot();
    if (not ack.valid()) {
      return pusch;
    }
    return pusch.valid() and pusch > ack ? pusch : ack;
  }

  /// Deallocate UE HARQ entity.
  void reset();

  /// Block new retxs of pending HARQs.
  void cancel_retxs();

  std::optional<dl_harq_process_handle> dl_harq(harq_id_t h_id)
  {
    if (h_id < get_dl_ue().harqs.size() and get_dl_ue().harqs[h_id].status != harq_utils::harq_state_t::empty) {
      return dl_harq_process_handle{cell_harq_mgr->dl, get_dl_ue().harqs[h_id]};
    }
    return std::nullopt;
  }
  std::optional<const dl_harq_process_handle> dl_harq(harq_id_t h_id) const
  {
    if (h_id < get_dl_ue().harqs.size() and get_dl_ue().harqs[h_id].status != harq_utils::harq_state_t::empty) {
      return dl_harq_process_handle{cell_harq_mgr->dl, cell_harq_mgr->dl.ues[ue_index]->harqs[h_id]};
    }
    return std::nullopt;
  }
  std::optional<ul_harq_process_handle>       ul_harq(harq_id_t h_id, slot_point slot);
  std::optional<const ul_harq_process_handle> ul_harq(harq_id_t h_id, slot_point slot) const;
  std::optional<ul_harq_process_handle>       ul_harq(harq_id_t h_id)
  {
    if (h_id < get_ul_ue().harqs.size() and get_ul_ue().harqs[h_id].status != harq_utils::harq_state_t::empty) {
      return ul_harq_process_handle{cell_harq_mgr->ul, get_ul_ue().harqs[h_id]};
    }
    return std::nullopt;
  }
  std::optional<const ul_harq_process_handle> ul_harq(harq_id_t h_id) const
  {
    if (h_id < get_ul_ue().harqs.size() and get_ul_ue().harqs[h_id].status != harq_utils::harq_state_t::empty) {
      return ul_harq_process_handle{cell_harq_mgr->ul, cell_harq_mgr->ul.ues[ue_index]->harqs[h_id]};
    }
    return std::nullopt;
  }

  /// \brief Allocates a DL HARQ process for a new transmission.
  /// \param[in] sl_pdcch Slot of the PDCCH that schedules the PDSCH.
  /// \param[in] sl_pdsch Slot of the PDSCH.
  /// \param[in] ack_delay Delay, in slots, of the HARQ-ACK report with respect to the PDSCH.
  /// \param[in] max_harq_nof_retxs Maximum number of retransmissions for this HARQ process.
  /// \param[in] harq_bit_idx Bit index of the HARQ-ACK in the UCI indication.
  /// \param[in] mode Mode the selected HARQ process must be operating in.
  /// \param[in] nof_repetitions Number of consecutive slots spanned by this transmission, starting at \c sl_pdsch
  /// (Rel-16 PDSCH repetitions; 1 for a single transmission). See \c cell_harq_repository::alloc_harq.
  /// \param[in] last_ack_delay Delay, in slots, of the last transmission carrying the HARQ-ACK report with respect to
  /// the PDSCH. It only differs from \c ack_delay if the report is repeated over multiple slots. Defaults to
  /// \c ack_delay.
  std::optional<dl_harq_process_handle> alloc_dl_harq(slot_point              sl_pdcch,
                                                      slot_point              sl_pdsch,
                                                      unsigned                ack_delay,
                                                      unsigned                max_harq_nof_retxs,
                                                      unsigned                harq_bit_idx,
                                                      harq_utils::harq_mode_t mode = harq_utils::harq_mode_t::normal,
                                                      uint8_t                 nof_repetitions = 1,
                                                      std::optional<unsigned> last_ack_delay  = std::nullopt);

  /// \brief Allocates a UL HARQ process for a new transmission.
  /// \param[in] sl_pdcch Slot of the PDCCH that schedules the PUSCH.
  /// \param[in] sl_tx Slot of the PUSCH.
  /// \param[in] max_harq_nof_retxs Maximum number of retransmissions for this HARQ process.
  /// \param[in] cg_params If set, forces the allocation of this specific HARQ-id, as a Configured Grant requires.
  /// \param[in] mode Mode the selected HARQ process must be operating in.
  /// \param[in] nof_repetitions Slot span of the PUSCH repetition bundle starting at \c sl_tx (1 for a single
  /// transmission).
  std::optional<ul_harq_process_handle> alloc_ul_harq(slot_point                          sl_pdcch,
                                                      slot_point                          sl_tx,
                                                      unsigned                            max_harq_nof_retxs,
                                                      std::optional<cg_harq_alloc_params> cg_params = std::nullopt,
                                                      harq_utils::harq_mode_t mode = harq_utils::harq_mode_t::normal,
                                                      uint8_t                 nof_repetitions = 1);

  std::optional<dl_harq_process_handle>       find_pending_dl_retx();
  std::optional<const dl_harq_process_handle> find_pending_dl_retx() const;
  std::optional<ul_harq_process_handle>       find_pending_ul_retx();
  std::optional<const ul_harq_process_handle> find_pending_ul_retx() const;

  std::optional<dl_harq_process_handle>       find_dl_harq_waiting_ack();
  std::optional<const dl_harq_process_handle> find_dl_harq_waiting_ack() const;
  std::optional<ul_harq_process_handle>       find_ul_harq_waiting_ack();

  /// Fetch a DL HARQ process expecting ACK info based on HARQ-ACK UCI slot and HARQ bit index.
  /// \param[in] uci_slot Slot when the UCI is to be received.
  /// \param[in] harq_bit_idx Bit index of the HARQ-ACK in the UCI indication.
  /// \return Active DL HARQ process with matching UCI slot and HARQ bit index, if found.
  std::optional<dl_harq_process_handle> find_dl_harq_waiting_ack(slot_point uci_slot, uint8_t harq_bit_idx);

  /// Fetch active UL HARQ process based on slot when its PUSCH was transmitted.
  /// \param[in] pusch_slot Slot when the PUSCH was transmitted.
  /// \return Active UL HARQ process with matching PUSCH slot, if found.
  std::optional<ul_harq_process_handle> find_ul_harq_waiting_ack(slot_point pusch_slot);

  /// Determines the sum of the number of bytes that are in active UL HARQ processes.
  /// \param[in] slice_id When set, the processes sized for another RAN slice are left out. The ones naming no
  /// slice are counted, as the UE may fill them from any slice.
  units::bytes total_ul_bytes_waiting_ack(std::optional<ran_slice_id_t> slice_id = std::nullopt) const;

private:
  dl_harq_ent_impl&       get_dl_ue() { return *cell_harq_mgr->dl.ues[ue_index]; }
  const dl_harq_ent_impl& get_dl_ue() const { return *cell_harq_mgr->dl.ues[ue_index]; }
  ul_harq_ent_impl&       get_ul_ue() { return *cell_harq_mgr->ul.ues[ue_index]; }
  const ul_harq_ent_impl& get_ul_ue() const { return *cell_harq_mgr->ul.ues[ue_index]; }

  harq_id_t          first_non_reserved_harq_id = to_harq_id(0);
  cell_harq_manager* cell_harq_mgr              = nullptr;
  du_ue_index_t      ue_index                   = INVALID_DU_UE_INDEX;
  rnti_t             crnti                      = rnti_t::INVALID_RNTI;
};

} // namespace ocudu
