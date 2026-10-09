// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../config/logical_channel_list_config.h"
#include "../slicing/ran_slice_id.h"
#include "ocudu/adt/soa_table.h"
#include "ocudu/adt/stable_id_map.h"
#include "ocudu/adt/static_flat_map.h"
#include "ocudu/mac/mac_pdu_format.h"
#include "ocudu/ran/logical_channel/lcid_dl_sch.h"
#include "ocudu/scheduler/result/pdsch_info.h"
#include "ocudu/support/math/exponential_averager.h"
#include "ocudu/support/units.h"

namespace ocudu {

class ue_logical_channel_repository;
struct ul_bsr_indication_message;

namespace logical_channel_system_utils {

/// Helper class to map (UE, LCID) pairs to logical channel fields.
class logical_channel_mapper
{
  static constexpr uint16_t       invalid_row_id   = std::numeric_limits<uint16_t>::max();
  static constexpr ran_slice_id_t invalid_slice_id = ran_slice_id_t{std::numeric_limits<uint8_t>::max()};

public:
  logical_channel_mapper();

  /// Update logical channel states on each slot indication.
  void slot_indication();

  /// Create new or modify existing logical channel (LC) and logical channel group (LCG) entry.
  /// \return Pair of (LC row ID, LCG row ID).
  std::pair<soa::row_id, soa::row_id>
  addmod_lc_and_lcg(du_ue_index_t ue_index, const logical_channel_config& lc_cfg, unsigned slots_per_msec);

  /// Remove logical channel (LC) entry.
  void rem_lc(du_ue_index_t ue_index, lcid_t lcid);

  /// Remove logical channel group (LCG) entry.
  void rem_lcg(du_ue_index_t ue_index, lcg_id_t lcgid);

  /// Associate RAN Slice ID with the given LC.
  /// \param[in] lc_rid Row ID of the logical channel.
  /// \param[in] slice_id RAN Slice ID to associate.
  void register_lc_slice(soa::row_id lc_rid, ran_slice_id_t slice_id);

  /// Associate RAN Slice ID with the given LCG.
  /// \param[in] lcg_rid Row ID of the logical channel group.
  /// \param[in] slice_id RAN Slice ID to associate.
  void register_lcg_slice(soa::row_id lcg_rid, ran_slice_id_t slice_id);

  /// Deregister RAN Slice association from the given LC.
  void deregister_lc_slice(soa::row_id lc_rid);

  /// Deregister RAN Slice association from the given LCG.
  void deregister_lcg_slice(soa::row_id lcg_rid);

  /// Whether the given (UE, LCID) pair is registered.
  bool contains(du_ue_index_t ue_index, lcid_t lcid) const
  {
    return get_row_id_entry(ue_index, lcid) != invalid_row_id;
  }

  /// Get row ID for the given (UE, LCID) pair. Asserts if not found.
  soa::row_id get_row_id(du_ue_index_t ue_index, lcid_t lcid) const
  {
    const auto ridx = get_row_id_entry(ue_index, lcid);
    ocudu_sanity_check(ridx != invalid_row_id, "Logical Channel not found");
    return soa::row_id{ridx};
  }

  /// Get row ID for the given (UE, LCG-ID) pair. Asserts if not found.
  soa::row_id get_row_id(du_ue_index_t ue_index, lcg_id_t lcgid) const
  {
    const auto ridx = get_row_id_entry(ue_index, lcgid);
    ocudu_sanity_check(ridx != invalid_row_id, "Logical Channel Group not found");
    return soa::row_id{ridx};
  }

  /// Find row ID for the given (UE, LCID) pair. Returns std::nullopt if not found.
  std::optional<soa::row_id> find_row_id(du_ue_index_t ue_index, lcid_t lcid) const
  {
    if (const auto ridx = get_row_id_entry(ue_index, lcid); ridx != invalid_row_id) {
      return soa::row_id{ridx};
    }
    return std::nullopt;
  }

  /// Find row ID for the given (UE, LCG-ID) pair. Returns std::nullopt if not found.
  std::optional<soa::row_id> find_row_id(du_ue_index_t ue_index, lcg_id_t lcgid) const
  {
    if (const auto ridx = get_row_id_entry(ue_index, lcgid); ridx != invalid_row_id) {
      return soa::row_id{ridx};
    }
    return std::nullopt;
  }

  std::optional<stable_id_t>& dl_qos_row(soa::row_id lc_rid) { return dl_fields.at<dl_field_type::qos_row>(lc_rid); }
  const std::optional<stable_id_t>& dl_qos_row(soa::row_id lc_rid) const
  {
    return dl_fields.at<dl_field_type::qos_row>(lc_rid);
  }
  std::optional<stable_id_t>& ul_qos_row(soa::row_id lcg_rid) { return ul_fields.at<ul_field_type::qos_row>(lcg_rid); }
  const std::optional<stable_id_t>& ul_qos_row(soa::row_id lcg_rid) const
  {
    return ul_fields.at<ul_field_type::qos_row>(lcg_rid);
  }
  unsigned*           last_dl_sched_bytes(soa::row_id lc_rid);
  unsigned*           last_ul_sched_bytes(soa::row_id lc_rid);
  double              avg_dl_bits_per_slot(soa::row_id lc_rid) const;
  double              avg_ul_bits_per_slot(soa::row_id lcg_rid) const;
  unsigned*           ul_sched_bytes_accum(soa::row_id lcg_rid);
  slot_point&         hol_toa(soa::row_id lc_rid) { return dl_fields.at<dl_field_type::hol_toa>(lc_rid); }
  units::bytes&       dl_buf_st(soa::row_id lc_rid) { return dl_fields.at<dl_field_type::dl_buf_st>(lc_rid); }
  const units::bytes& dl_buf_st(soa::row_id lc_rid) const { return dl_fields.at<dl_field_type::dl_buf_st>(lc_rid); }
  units::bytes&       ul_buf_st(soa::row_id lcg_rid) { return ul_fields.at<ul_field_type::buf_st>(lcg_rid); }
  const units::bytes& ul_buf_st(soa::row_id lcg_rid) const { return ul_fields.at<ul_field_type::buf_st>(lcg_rid); }

  const std::optional<mac_lc_config::triggered_ul_grant_cfg>& dl_triggered_ul_grant(soa::row_id lcg_rid) const
  {
    return ul_fields.at<ul_field_type::triggered_ul_grant>(lcg_rid);
  }

  /// Returns the RAN DL Slice ID associated with the given LC, or std::nullopt if none is associated.
  std::optional<ran_slice_id_t> dl_slice_id(soa::row_id lc_rid) const
  {
    const ran_slice_id_t slice_id = dl_fields.at<dl_field_type::slice_id>(lc_rid);
    if (slice_id == invalid_slice_id) {
      return std::nullopt;
    }
    return slice_id;
  }
  std::optional<ran_slice_id_t> dl_slice_id(du_ue_index_t ue_index, lcid_t lcid) const
  {
    return dl_slice_id(get_row_id(ue_index, lcid));
  }

  std::optional<ran_slice_id_t> ul_slice_id(soa::row_id lcg_rid) const
  {
    const ran_slice_id_t slice_id = ul_fields.at<ul_field_type::slice_id>(lcg_rid);
    if (slice_id == invalid_slice_id) {
      return std::nullopt;
    }
    return slice_id;
  }
  std::optional<ran_slice_id_t> ul_slice_id(du_ue_index_t ue_index, lcg_id_t lcgid) const
  {
    return ul_slice_id(get_row_id(ue_index, lcgid));
  }

private:
  /// QoS context relative to a single logical channel of a UE, which has QoS enabled.
  struct lc_qos_context {
    /// Sum of scheduled DL bytes for this LC and for the current slot.
    unsigned dl_last_sched_bytes = 0;
    /// Over-the-air DL Bytes-per-slot average for this logical channel.
    exp_average_fast_start<float> dl_avg_bytes_per_slot;
  };
  /// QoS context relative to a single logical channel group (LCG) of a UE, which has QoS enabled.
  struct lcg_qos_context {
    /// Sum of scheduled UL bytes for this LCG and for the current slot.
    unsigned ul_last_sched_bytes = 0;
    /// Scheduled UL bytes for this LCG since the last received BSR.
    unsigned sched_bytes_accum = 0;
    /// Over-the-air UL Bytes-per-slot average for this logical channel.
    exp_average_fast_start<float> ul_avg_bytes_per_slot;
  };

  /// Provides the mapping between (UE, LCID) and row IDs.
  static size_t get_index(du_ue_index_t ue_index, lcid_t lcid)
  {
    return static_cast<size_t>(ue_index) * MAX_NOF_RB_LCIDS + static_cast<size_t>(lcid);
  }

  /// Provides the mapping between (UE, LCG-ID) and row IDs.
  static size_t get_index(du_ue_index_t ue_index, lcg_id_t lcgid)
  {
    return static_cast<size_t>(ue_index) * MAX_NOF_LCGS + static_cast<size_t>(lcgid);
  }

  uint16_t& get_row_id_entry(du_ue_index_t ue_index, lcid_t lcid)
  {
    return ue_lcid_to_dl_row_id[get_index(ue_index, lcid)];
  }
  const uint16_t& get_row_id_entry(du_ue_index_t ue_index, lcid_t lcid) const
  {
    return ue_lcid_to_dl_row_id[get_index(ue_index, lcid)];
  }
  uint16_t& get_row_id_entry(du_ue_index_t ue_index, lcg_id_t lcgid)
  {
    return ue_lcgid_to_ul_row_id[get_index(ue_index, lcgid)];
  }
  const uint16_t& get_row_id_entry(du_ue_index_t ue_index, lcg_id_t lcgid) const
  {
    return ue_lcgid_to_ul_row_id[get_index(ue_index, lcgid)];
  }

  /// Mapping from (UE, LCID) to DL row IDs.
  std::vector<uint16_t> ue_lcid_to_dl_row_id;

  /// Mapping from (UE, LCG-ID) to UL row IDs.
  std::vector<uint16_t> ue_lcgid_to_ul_row_id;

  /// Table holding QoS contexts for logical channels with QoS tracking enabled.
  stable_id_map<lc_qos_context>  qos_channels;
  stable_id_map<lcg_qos_context> qos_lcgs;

  /// Table mapping from row ID to DL Logical Channel (LC).
  enum class dl_field_type {
    /// DL Buffer status of this LC.
    dl_buf_st,
    /// Slice associated with this LC.
    slice_id,
    /// In case QoS statistics are being tracked, holds the row in the \c qos_channels table.
    qos_row,
    /// Head-of-line (HOL) time-of-arrival
    hol_toa
  };
  soa::table<dl_field_type, units::bytes, ran_slice_id_t, std::optional<stable_id_t>, slot_point> dl_fields;

  /// Table mapping from row ID to UL Logical Channel Group (LCG).
  enum class ul_field_type {
    /// UL Buffer status of this LCG.
    buf_st,
    /// Slice associated with this LCG.
    slice_id,
    /// In case QoS statistics are being tracked, holds the row in the \c qos_channels table.
    qos_row,
    /// Optional config for proactive UL grant triggered by DL allocation.
    triggered_ul_grant,
  };
  soa::table<ul_field_type,
             units::bytes,
             ran_slice_id_t,
             std::optional<stable_id_t>,
             std::optional<mac_lc_config::triggered_ul_grant_cfg>>
      ul_fields;
};

} // namespace logical_channel_system_utils

/// System responsible for handling the logical channel configuration and buffer occupancy status of UEs.
class logical_channel_system
{
public:
  /// Holds the information relative to a MAC CE that needs to be scheduled.
  struct mac_ce_info {
    /// LCID of the MAC CE.
    lcid_dl_sch_t ce_lcid;
    /// Holds payload of CE except UE Contention Resolution Identity.
    dl_mac_ce_payload ce_payload = dummy_ce_payload{0};
  };

  logical_channel_system();

  /// Signal the start of a new slot.
  void slot_indication();

  /// Creates a new UE logical channel repository.
  /// \param[in] ue_index DU UE index.
  /// \param[in] slot_ind_scs Subcarrier spacing associated with slot indications.
  /// \param[in] starts_in_fallback Whether the UE starts in fallback mode (no DRB tx).
  /// \param[in] log_channels_configs List of logical channel configurations for this UE.
  /// \return Handle to the created UE logical channel repository.
  ue_logical_channel_repository create_ue(du_ue_index_t                   ue_index,
                                          subcarrier_spacing              slot_ind_scs,
                                          bool                            starts_in_fallback,
                                          logical_channel_config_list_ptr log_channels_configs);

  /// Number of UEs managed by the system.
  size_t nof_ues() const { return ues.size(); }

  /// Number of logical channels managed by the system.
  size_t nof_logical_channels() const;

  /// \brief Fills list with UEs that may have DL pending newTx data for the given RAN slice.
  /// \param[in] slice_id RAN slice identifier.
  /// \return Bitset of UEs with pending data for the provided RAN slice.
  bounded_bitset<MAX_NOF_DU_UES> get_ues_with_dl_pending_data(ran_slice_id_t slice_id) const;

  /// \brief Fills list with UEs that have UL pending newTx data for the given RAN slice (SR not considered).
  ///
  /// Note: This is an estimation based solely on the logical channel buffer status reported by the UE. It may happen
  /// that due to the number bytes stored in the allocated UL HARQs, a UE with pending data has no actual newTx pending
  /// data to transmit.
  /// \param[in] slice_id RAN slice identifier.
  /// \return Bitset of UEs with pending data for the provided RAN slice.
  bounded_bitset<MAX_NOF_DU_UES> get_ues_with_ul_pending_data(ran_slice_id_t slice_id) const;

private:
  friend class ue_logical_channel_repository;
  friend class ue_logical_channel_repository_view;
  static constexpr size_t MAX_RAN_SLICES_PER_UE = 8;

  /// Information relative to a slice of the scheduler.
  struct ran_slice_context {
    /// Bitset of UE indexes for UEs with pending newTx DL data for this RAN slice.
    bounded_bitset<MAX_NOF_DU_UES> pending_dl_ues;
    /// Bitset of UE indexes for UEs with pending newTx UL data for this RAN slice.
    bounded_bitset<MAX_NOF_DU_UES> pending_ul_ues;
  };

  /// Context of a single pending CE.
  struct mac_ce_context {
    /// Information relative to the MAC CE to be scheduled.
    mac_ce_info info;
    /// Next CE node in the linked list.
    stable_id_t next_ue_ce;
  };
  /// UE context relative to its configuration.
  struct ue_config_context {
    /// DU UE index of this UE.
    du_ue_index_t ue_index{INVALID_DU_UE_INDEX};
    /// Number of slots per millisecond for this UE, based on the subcarrier spacing.
    unsigned slots_per_msec{0};
    /// List of UE-dedicated logical channel configurations.
    logical_channel_config_list_ptr channel_configs;
  };
  /// UE context relative to its DL channel management.
  struct ue_dl_channel_context {
    /// Currently enqueued CEs for this UE.
    stable_id_intrusive_list<&mac_ce_context::next_ue_ce> pending_ces;
    /// List of active logical channel IDs sorted in decreasing order of priority. i.e. first element has the highest
    /// priority.
    static_vector<lcid_t, MAX_NOF_RB_LCIDS> sorted_channels;
    /// Context of channels currently configured.
    static_flat_map<lcid_t, soa::row_id, MAX_NOF_RB_LCIDS> channels;
  };
  /// UE context relative to its UL channel management.
  struct ue_ul_channel_context {
    /// Context of logical channel groups (LCGs) currently configured.
    static_flat_map<lcg_id_t, soa::row_id, MAX_NOF_LCGS> lcgs;
    /// Slot at which the oldest, unserved SR indication was received for this UE.
    slot_point oldest_sr_sl_rx;
  };
  /// UE context relative to its DL state.
  struct ue_context {
    /// Whether the UE is in fallback (no DRB tx).
    bool fallback_state : 1;
    /// Whether a CON RES CE needs to be sent.
    bool pending_con_res_id : 1;
    /// Cached sum of pending CE bytes for this UE, excluding CON RES CE.
    uint16_t pending_ce_bytes : 14;

    explicit ue_context(bool fallback = false) :
      fallback_state(fallback), pending_con_res_id(false), pending_ce_bytes(0)
    {
    }
  };
  /// Context relative to a UE DL slice state.
  struct ue_dl_slice_context {
    /// Mapping of RAN slice ID to the pending bytes of that slice, excluding any CE.
    static_flat_map<ran_slice_id_t, unsigned, MAX_RAN_SLICES_PER_UE> pending_bytes_per_slice;
  };
  /// Context relative to a UE UL slice state.
  struct ue_ul_slice_context {
    /// Mapping of RAN slice ID to the pending bytes of that slice, excluding SRs.
    static_flat_map<ran_slice_id_t, unsigned, MAX_RAN_SLICES_PER_UE> pending_bytes_per_slice;
  };
  enum class ue_field { config, context, dl_slice, ul_slice, dl_channel, ul_channel };
  using ue_table     = soa::table<ue_field,
                                  ue_config_context,
                                  ue_context,
                                  ue_dl_slice_context,
                                  ue_ul_slice_context,
                                  ue_dl_channel_context,
                                  ue_ul_channel_context>;
  using ue_row       = soa::row_view<ue_table>;
  using const_ue_row = soa::row_view<const ue_table>;

  const_ue_row get_ue(soa::row_id ue_rid) const { return ues.row(ue_rid); }
  ue_row       get_ue(soa::row_id ue_rid) { return ues.row(ue_rid); }

  void remove_ue(soa::row_id ue_rid);
  void configure(soa::row_id ue_rid, logical_channel_config_list_ptr log_channels_configs);
  void deactivate(soa::row_id ue_rid);

  void set_lcid_ran_slice(soa::row_id ue_rid, lcid_t lcid, ran_slice_id_t slice_id);
  void deregister_lc_ran_slice(soa::row_id ue_rid, lcid_t lcid);
  void set_lcg_ran_slice(soa::row_id, lcg_id_t lcgid, ran_slice_id_t slice_id);
  void deregister_lcg_ran_slice(soa::row_id ue_rid, lcg_id_t lcgid);

  void
  handle_dl_buffer_status_indication(soa::row_id ue_row_id, lcid_t lcid, unsigned buffer_status, slot_point hol_toa);
  void handle_mac_ce_indication(soa::row_id ue_row_id, const mac_ce_info& ce);
  void handle_bsr_indication(soa::row_id ue_row_id, const ul_bsr_indication_message& bsr);

  void       handle_sr_indication(soa::row_id ue_row_id, slot_point uci_slot);
  void       reset_sr_indication(soa::row_id ue_row_id);
  slot_point pending_sr_slot_rx(soa::row_id ue_row_id) const;

  unsigned allocate_mac_sdu(soa::row_id ue_rid, dl_msg_lc_info& subpdu, lcid_t lcid, unsigned rem_bytes);
  unsigned allocate_mac_sdu(soa::row_id ue_rid, dl_msg_lc_info& lch_info, unsigned rem_bytes, lcid_t lcid);
  unsigned allocate_mac_ce(soa::row_id ue_rid, dl_msg_lc_info& lch_info, unsigned rem_bytes);
  unsigned allocate_ue_con_res_id_mac_ce(soa::row_id ue_rid, dl_msg_lc_info& lch_info, unsigned rem_bytes);

  /// Adds an estimate of the upper layer required header bytes.
  static unsigned add_upper_layer_header_bytes(lcg_id_t lcgid, unsigned payload_bytes)
  {
    // Estimate of the number of bytes required for the upper layer header.
    static constexpr unsigned RLC_HEADER_SIZE_ESTIMATE = 3U;
    if (payload_bytes == 0 or lcgid == 0) {
      // In case of no payload or LCG-ID == 0, there is no need to account for upper layer header.
      // TODO: Is this a fair assumption for LCG-ID == 0?
      return payload_bytes;
    }
    return payload_bytes + RLC_HEADER_SIZE_ESTIMATE;
  }

  // Helper inlined methods
  unsigned dl_pending_bytes(soa::row_id lc_rid) const
  {
    // Note: DL buffer occupancy report already accounts for at least one RLC header.
    return get_mac_sdu_required_bytes(lc_mapper.dl_buf_st(lc_rid).value());
  }
  unsigned ul_pending_bytes(lcg_id_t lcgid, soa::row_id lcg_rid) const
  {
    // Note: TS38.321, 6.1.3.1 - The size of the RLC and MAC headers are not considered in the buffer size computation.
    return get_mac_sdu_required_bytes(add_upper_layer_header_bytes(lcgid, lc_mapper.ul_buf_st(lcg_rid).value()));
  }

  /// Helper method to update the pending bytes for a given RAN slice when a lc associated with a slice is updated.
  void on_single_channel_buf_st_update(ue_row&                              u,
                                       const std::optional<ran_slice_id_t>& slice_id,
                                       unsigned                             new_buf_st,
                                       unsigned                             prev_buf_st);
  void on_single_lcg_buf_st_update(ue_row&                              u,
                                   lcg_id_t                             lcgid,
                                   const std::optional<ran_slice_id_t>& slice_id,
                                   unsigned                             new_buf_st,
                                   unsigned                             prev_buf_st);

  /// \brief Returns the next highest priority LCID. The prioritization policy is implementation-defined.
  lcid_t get_max_prio_lcid(const const_ue_row& ue_row) const;

  /// Bitset of configured UEs.
  bounded_bitset<MAX_NOF_DU_UES> configured_ues;

  /// Bitset of UEs with pending CEs.
  bounded_bitset<MAX_NOF_DU_UES> ues_with_pending_ces;

  /// Bitset of UEs with pending SRs.
  bounded_bitset<MAX_NOF_DU_UES> ues_with_pending_sr;

  /// Table of Logical Channel fields.
  logical_channel_system_utils::logical_channel_mapper lc_mapper;

  /// List of bitsets of UEs with pending newTx DL data per RAN slice.
  flat_map<ran_slice_id_t, ran_slice_context> slices;

  /// List of MAC CEs pending transmission.
  stable_id_map<mac_ce_context> pending_ces;

  /// List of UE contexts.
  ue_table ues;
};

/// Non-owning view to UE logical channel repository.
class ue_logical_channel_repository_view
{
  using ue_row       = logical_channel_system::ue_row;
  using const_ue_row = logical_channel_system::const_ue_row;

public:
  using mac_ce_info = logical_channel_system::mac_ce_info;

  ue_logical_channel_repository_view() = default;

  bool valid() const { return parent != nullptr; }

  /// \brief Checks whether a SR indication handling is pending.
  [[nodiscard]] bool has_pending_sr() const { return parent->ues_with_pending_sr.test(ue_index); }

  /// UCI slot since which the SR is pending.
  [[nodiscard]] slot_point pending_sr_slot_rx() const;

  /// \brief Enqueue new MAC CE to be scheduled.
  void handle_mac_ce_indication(const mac_ce_info& ce);

private:
  friend class ue_logical_channel_repository;

  ue_logical_channel_repository_view(logical_channel_system& parent_, du_ue_index_t ue_index_, soa::row_id ue_row_) :
    parent(&parent_), ue_index(ue_index_), ue_row_id(ue_row_)
  {
  }

  const_ue_row get_ue_row() const { return parent->get_ue(ue_row_id); }
  ue_row       get_ue_row() { return parent->ues.row(ue_row_id); }

  logical_channel_system* parent   = nullptr;
  du_ue_index_t           ue_index = INVALID_DU_UE_INDEX;
  soa::row_id             ue_row_id{std::numeric_limits<uint32_t>::max()};
};

/// Handle of UE logical channel repository.
class ue_logical_channel_repository : private ue_logical_channel_repository_view
{
  using mac_ce_info           = logical_channel_system::mac_ce_info;
  using ue_config_context     = logical_channel_system::ue_config_context;
  using ue_context            = logical_channel_system::ue_context;
  using ue_dl_slice_context   = logical_channel_system::ue_dl_slice_context;
  using ue_ul_slice_context   = logical_channel_system::ue_ul_slice_context;
  using ue_dl_channel_context = logical_channel_system::ue_dl_channel_context;
  using ue_ul_channel_context = logical_channel_system::ue_ul_channel_context;
  using ue_row                = logical_channel_system::ue_row;
  using const_ue_row          = logical_channel_system::const_ue_row;

  ue_logical_channel_repository(logical_channel_system& parent_, du_ue_index_t ue_index_, soa::row_id ue_row_) :
    ue_logical_channel_repository_view(parent_, ue_index_, ue_row_)
  {
  }

public:
  // inherited methods
  using ue_logical_channel_repository_view::handle_mac_ce_indication;
  using ue_logical_channel_repository_view::has_pending_sr;
  using ue_logical_channel_repository_view::pending_sr_slot_rx;
  using ue_logical_channel_repository_view::valid;

  ue_logical_channel_repository()                                     = default;
  ue_logical_channel_repository(const ue_logical_channel_repository&) = delete;
  ue_logical_channel_repository(ue_logical_channel_repository&& other) noexcept { *this = std::move(other); }
  ue_logical_channel_repository& operator=(const ue_logical_channel_repository&) = delete;
  ue_logical_channel_repository& operator=(ue_logical_channel_repository&& other) noexcept
  {
    reset();
    parent    = std::exchange(other.parent, nullptr);
    ue_index  = std::exchange(other.ue_index, INVALID_DU_UE_INDEX);
    ue_row_id = other.ue_row_id;
    return *this;
  }
  ~ue_logical_channel_repository() { reset(); }

  void reset()
  {
    if (parent != nullptr) {
      parent->remove_ue(ue_row_id);
      parent = nullptr;
    }
  }

  /// Current list of of logical channel configurations for the given UE.
  const logical_channel_config_list& cfg() const { return *get_ue_row().at<ue_config_context>().channel_configs; }

  /// \brief Update the configurations of the provided lists of bearers.
  void configure(logical_channel_config_list_ptr log_channels_configs);

  /// \brief Deactivate all bearers and drop all pending CEs.
  void deactivate();

  /// Set UE fallback state.
  void set_fallback_state(bool fallback_active);

  /// Assign a RAN slice to a logical channel.
  void set_lcid_ran_slice(lcid_t lcid, ran_slice_id_t slice_id);

  /// Assign a RAN slice to a logical channel group.
  void set_lcg_ran_slice(lcg_id_t lcgid, ran_slice_id_t slice_id);

  /// Detach logical channel from previously set RAN slice.
  void reset_lcid_ran_slice(lcid_t lcid);

  /// Detach logical channel group from previously set RAN slice.
  void reset_lcg_ran_slice(lcg_id_t lcgid);

  /// Determines whether a RAN slice has at least one bearer associated with it.
  [[nodiscard]] bool has_slice(ran_slice_id_t slice_id) const
  {
    return get_ue_row().at<ue_dl_slice_context>().pending_bytes_per_slice.contains(slice_id);
  }

  /// Get the RAN slice ID associated with a logical channel.
  std::optional<ran_slice_id_t> get_slice_id(lcid_t lcid) const
  {
    auto rid = parent->lc_mapper.find_row_id(ue_index, lcid);
    if (rid.has_value()) {
      return parent->lc_mapper.dl_slice_id(*rid);
    }
    return std::nullopt;
  }

  /// Get the RAN slice ID associated with a logical channel group.
  std::optional<ran_slice_id_t> get_slice_id(lcg_id_t lcgid) const
  {
    auto rid = parent->lc_mapper.find_row_id(ue_index, lcgid);
    if (rid.has_value()) {
      return parent->lc_mapper.ul_slice_id(*rid);
    }
    return std::nullopt;
  }

  /// \brief Verifies if logical channel is activated for DL.
  [[nodiscard]] bool is_configured(lcid_t lcid) const
  {
    return parent->lc_mapper.find_row_id(ue_index, lcid).has_value();
  }

  /// \brief Verifies if logical channel group is activated for UL.
  [[nodiscard]] bool is_configured(lcg_id_t lcgid) const
  {
    return parent->lc_mapper.find_row_id(ue_index, lcgid).has_value();
  }

  /// Check whether the UE is in fallback state.
  [[nodiscard]] bool is_in_fallback_state() const { return get_ue_row().at<ue_context>().fallback_state; }

  /// \brief Check whether the UE has pending DL data, given its current state.
  [[nodiscard]] bool has_dl_pending_bytes() const
  {
    auto        u      = get_ue_row();
    const auto& ue_ctx = u.at<ue_context>();
    if (ue_ctx.fallback_state) {
      return ue_ctx.pending_con_res_id or has_pending_bytes(LCID_SRB0) or has_pending_bytes(LCID_SRB1);
    }
    if (has_pending_ces()) {
      return true;
    }
    const auto& chs_ctx = u.at<ue_dl_channel_context>();
    return std::any_of(chs_ctx.channels.begin(), chs_ctx.channels.end(), [this](const auto& p) {
      if (p.first == LCID_SRB0) {
        return false;
      }
      return parent->lc_mapper.dl_buf_st(p.second).value() > 0;
    });
  }

  /// \brief Check whether the UE has pending UL data (excluding SR), given its current state.
  [[nodiscard]] bool has_ul_pending_bytes() const
  {
    auto        u      = get_ue_row();
    const auto& ue_ctx = u.at<ue_context>();
    if (ue_ctx.fallback_state) {
      return has_pending_bytes(uint_to_lcg_id(0U));
    }
    const auto& chs_ctx = u.at<ue_ul_channel_context>();
    return std::any_of(chs_ctx.lcgs.begin(), chs_ctx.lcgs.end(), [this](const auto& p) {
      return parent->lc_mapper.ul_buf_st(p.second).value() > 0;
    });
  }

  /// \brief Check whether the UE has pending data in the provided RAN slice.
  /// \return Returns true if the UE is active, in non-fallback mode, it has pending bytes for the provided RAN slice
  /// ID and the slice is configured. Returns false, otherwise.
  [[nodiscard]] bool has_pending_dl_bytes(ran_slice_id_t slice_id) const
  {
    auto slice_bitset_it = parent->slices.find(slice_id);
    if (slice_bitset_it != parent->slices.end() and slice_bitset_it->second.pending_dl_ues.test(ue_index)) {
      return true;
    }

    // In case SRB slice was selected (but with no data) and there are pending CE bytes (excluding ConRes, which is
    // only scheduled in fallback mode).
    if (slice_id == SRB_RAN_SLICE_ID and parent->ues_with_pending_ces.test(ue_index)) {
      // Check if any other slices have pending data. If they do, CE is not considered.
      // Note: This extra check is to avoid multiple slices report pending data when CEs are pending.
      return std::all_of(parent->slices.begin(), parent->slices.end(), [this, slice_id](const auto& p) {
        return p.first == slice_id or not p.second.pending_dl_ues.test(ue_index);
      });
    }
    return false;
  }

  /// \brief Check whether the UE has pending UL data in the provided RAN slice.
  /// \return Returns true if the UE is active, in non-fallback mode, it has pending bytes for the provided RAN slice
  /// ID and the slice is configured. Returns false, otherwise.
  [[nodiscard]] bool has_pending_ul_bytes(ran_slice_id_t slice_id) const
  {
    auto slice_bitset_it = parent->slices.find(slice_id);
    return slice_bitset_it != parent->slices.end() and slice_bitset_it->second.pending_ul_ues.test(ue_index);
  }

  /// \brief Checks whether a logical channel has pending data.
  [[nodiscard]] bool has_pending_bytes(lcid_t lcid) const
  {
    const auto lc_rid = parent->lc_mapper.find_row_id(ue_index, lcid);
    return lc_rid.has_value() and parent->lc_mapper.dl_buf_st(*lc_rid).value() > 0;
  }

  /// \brief Checks whether a logical channel has pending data.
  [[nodiscard]] bool has_pending_bytes(lcg_id_t lcgid) const
  {
    const auto lcg_rid = parent->lc_mapper.find_row_id(ue_index, lcgid);
    return lcg_rid.has_value() and parent->lc_mapper.ul_buf_st(*lcg_rid).value() > 0;
  }

  /// Transport block a slice can build next.
  struct dl_transport_block_selection {
    /// Restrictions the block carries, as the channels it serves set them.
    dl_transport_block_restrictions restrictions;
    /// Pending bytes the block can carry, i.e. the slice total less the channels it leaves out.
    unsigned pending_bytes;
  };

  /// \brief Selects the transport block a new grant for the given slice is built on.
  ///
  /// The channels holding data join in decreasing order of priority, the order the block is filled in, and the bytes
  /// of those left out are dropped from the size. With nothing buffered the highest priority restricted channel
  /// decides.
  [[nodiscard]] dl_transport_block_selection select_dl_transport_block(ran_slice_id_t slice_id) const;

  /// \brief Selects the restrictions a new grant for the given slice satisfies.
  ///
  /// A channel bound to mode A wins as soon as it has data: mode A holds each process a full round trip, so it cannot
  /// crowd mode B out. Otherwise the configuration decides, mode A first, as the UE cannot be served on a grant its
  /// channels forbid, whatever it buffers after its last report.
  [[nodiscard]] ul_grant_restrictions select_ul_grant_restrictions(ran_slice_id_t slice_id) const
  {
    std::optional<ul_harq_mode> with_data;
    std::optional<ul_harq_mode> configured;
    for (const logical_channel_config_ptr& lc_cfg : cfg()) {
      if (not lc_cfg->allowed_harq_mode.has_value()) {
        continue;
      }
      const std::optional<soa::row_id> lcg_rid = parent->lc_mapper.find_row_id(ue_index, lc_cfg->lc_group);
      if (not lcg_rid.has_value() or parent->lc_mapper.ul_slice_id(*lcg_rid) != slice_id) {
        continue;
      }
      // Mode A outranks mode B, so it wins whichever channel is seen first.
      const ul_harq_mode mode = *lc_cfg->allowed_harq_mode;
      if (not configured.has_value() or mode == ul_harq_mode::mode_a) {
        configured = mode;
      }
      if (parent->lc_mapper.ul_buf_st(*lcg_rid).value() > 0 and
          (not with_data.has_value() or mode == ul_harq_mode::mode_a)) {
        with_data = mode;
        if (mode == ul_harq_mode::mode_a) {
          // No channel left can outrank mode A with data.
          return {with_data};
        }
      }
    }
    if (with_data.has_value()) {
      return {with_data};
    }
    return {configured};
  }

  /// \brief Checks whether a ConRes CE is pending for transmission.
  bool is_con_res_id_pending() const { return get_ue_row().at<ue_context>().pending_con_res_id; }

  /// \brief Checks whether UE has pending CEs to be scheduled (ConRes included).
  bool has_pending_ces() const
  {
    return parent->ues_with_pending_ces.test(ue_index) or get_ue_row().at<ue_context>().pending_con_res_id;
  }

  /// \brief Calculates total number of DL bytes, including MAC header overhead, and without taking into account
  /// the UE state (same result if fallback or not fallback).
  unsigned total_dl_pending_bytes() const;

  /// \brief Calculates number of DL pending bytes, including MAC header overhead, and taking UE state into account.
  unsigned dl_pending_bytes() const;

  /// \brief Calculates number of UL pending bytes, including MAC header overhead, and taking UE state into account.
  unsigned ul_pending_bytes() const;

  /// Calculates the number of DL pending bytes, including MAC header overhead, for a RAN slice.
  unsigned dl_pending_bytes(ran_slice_id_t slice_id) const
  {
    const auto  u      = get_ue_row();
    const auto& ue_ctx = u.at<ue_context>();
    if (ue_ctx.fallback_state) {
      return 0;
    }
    const auto& ue_dl_ctx = u.at<ue_dl_slice_context>();
    auto        slice_it  = ue_dl_ctx.pending_bytes_per_slice.find(slice_id);
    if (slice_it == ue_dl_ctx.pending_bytes_per_slice.end()) {
      return 0;
    }

    unsigned       total_bytes = slice_it->second;
    const unsigned ce_bytes    = ue_ctx.pending_ce_bytes;
    if (ce_bytes > 0) {
      // There are also pending CE bytes.
      if (total_bytes > 0) {
        // In case the UE has pending bearer bytes, we also include the CE bytes.
        total_bytes += ce_bytes;
      } else if (slice_id == SRB_RAN_SLICE_ID) {
        // In case SRB1 was selected, and there are no pending bytes in the selected bearers, we return the pending CE
        // bytes iff the UE has no pending data on the remaining, non-selected bearers.
        // This is to avoid the situation where a UE, for instance, has DRB data to transmit, but the CE is allocated in
        // the SRB slice instead.
        if (std::all_of(ue_dl_ctx.pending_bytes_per_slice.begin(),
                        ue_dl_ctx.pending_bytes_per_slice.end(),
                        [](const auto& elem) { return elem.second == 0; })) {
          return ce_bytes;
        }
        return 0;
      }
    }

    return total_bytes;
  }

  /// Calculates the number of UL pending bytes for a RAN slice.
  unsigned ul_pending_bytes(ran_slice_id_t slice_id) const
  {
    if (get_ue_row().at<ue_context>().fallback_state) {
      return 0;
    }
    const auto& ue_ul_ctx = get_ue_row().at<ue_ul_slice_context>();
    auto        slice_it  = ue_ul_ctx.pending_bytes_per_slice.find(slice_id);
    if (slice_it == ue_ul_ctx.pending_bytes_per_slice.end()) {
      return 0;
    }
    return slice_it->second;
  }

  /// \brief Returns the UE pending CEs' bytes to be scheduled, if any.
  unsigned pending_ce_bytes() const
  {
    auto u = get_ue_row();
    return pending_con_res_ce_bytes() + u.at<ue_context>().pending_ce_bytes;
  }

  /// \brief Checks whether UE has pending UE Contention Resolution Identity CE to be scheduled.
  unsigned pending_con_res_ce_bytes() const
  {
    static constexpr auto ce_size = lcid_dl_sch_t{lcid_dl_sch_t::UE_CON_RES_ID}.sizeof_ce();
    auto                  u       = get_ue_row();
    return u.at<ue_context>().pending_con_res_id ? FIXED_SIZED_MAC_CE_SUBHEADER_SIZE + ce_size : 0;
  }

  /// \brief Last DL buffer status for given LCID (MAC subheader included).
  unsigned pending_bytes(lcid_t lcid) const
  {
    auto rid = parent->lc_mapper.find_row_id(ue_index, lcid);
    return rid.has_value() ? parent->dl_pending_bytes(*rid) : 0;
  }

  /// \brief Last UL buffer status for given LCG-ID (MAC subheader included).
  unsigned pending_bytes(lcg_id_t lcgid) const
  {
    auto rid = parent->lc_mapper.find_row_id(ue_index, lcgid);
    return rid.has_value() ? parent->ul_pending_bytes(lcgid, *rid) : 0;
  }

  /// \brief Average DL bit rate, in bps, for a given LCID.
  double average_dl_bit_rate(lcid_t lcid) const;

  /// \brief Average UL bit rate, in bps, for a given LCG-ID.
  double average_ul_bit_rate(lcg_id_t lcgid) const;

  /// \brief Returns the HOL time-of-arrival for a given LCID.
  slot_point hol_toa(lcid_t lcid) const
  {
    return parent->lc_mapper.hol_toa(parent->lc_mapper.get_row_id(ue_index, lcid));
  }

  /// \brief Update DL buffer status for a given LCID.
  void handle_dl_buffer_status_indication(lcid_t lcid, unsigned buffer_status, slot_point hol_toa = {});

  /// \brief Handle a new Buffer Status Report.
  void handle_bsr_indication(const ul_bsr_indication_message& msg);

  /// \brief Indicate that the UE requested an UL grant.
  /// \param[in] uci_slot Slot at which SR was received.
  void handle_sr_indication(slot_point uci_slot);

  /// \brief Allocates highest priority MAC SDU within space of \c rem_bytes bytes. Updates \c lch_info with allocated
  /// bytes for the MAC SDU (no MAC subheader).
  /// \return Allocated bytes for MAC SDU (with subheader).
  unsigned allocate_mac_sdu(dl_msg_lc_info& lch_info, unsigned rem_bytes, lcid_t lcid = INVALID_LCID);

  /// \brief Allocates next MAC CE within space of \c rem_bytes bytes. Updates \c lch_info with allocated bytes for the
  /// MAC CE.
  /// \return Allocated bytes for MAC CE (with subheader).
  /// \remark Excludes UE Contention Resolution Identity CE.
  unsigned allocate_mac_ce(dl_msg_lc_info& lch_info, unsigned rem_bytes);

  /// \brief Allocates UE Contention Resolution Identity MAC CE within space of \c rem_bytes bytes. Updates \c lch_info
  /// with allocated bytes for the MAC CE.
  /// \return Allocated bytes for UE Contention Resolution Identity MAC CE (with subheader).
  unsigned allocate_ue_con_res_id_mac_ce(dl_msg_lc_info& lch_info, unsigned rem_bytes);

  /// \brief Register the scheduling of an UL grant for this UE.
  ///
  /// This event will be used to update estimated bit rates.
  void handle_ul_grant(units::bytes grant_size);

  /// Clear SR indication pending flag.
  void reset_sr_indication();

  /// \brief Returns a list of LCIDs sorted based on decreasing order of priority.
  span<const lcid_t> get_prioritized_logical_channels() const
  {
    return get_ue_row().at<ue_dl_channel_context>().sorted_channels;
  }

  ue_logical_channel_repository_view view() const
  {
    return ue_logical_channel_repository_view{*parent, ue_index, ue_row_id};
  }

private:
  friend class logical_channel_system;
};

/// \brief Allocate MAC SDUs and corresponding MAC subPDU subheaders.
/// \param[in] tb_info TB on which MAC subPDUs will be stored.
/// \param[in] lch_mng UE DL logical channel manager.
/// \param[in] total_tbs available space in bytes for subPDUs.
/// \param[in] lcid if provided, LCID of the logical channel to be allocated. Otherwise, the LCID with higher priority
/// is chosen.
/// \return Total number of bytes allocated (including MAC subheaders).
unsigned allocate_mac_sdus(dl_msg_tb_info&                tb_info,
                           ue_logical_channel_repository& lch_mng,
                           units::bytes                   total_tbs,
                           lcid_t                         lcid = INVALID_LCID);

/// \brief Allocate MAC subPDUs for pending MAC CEs.
/// \param[in] tb_info TB on which MAC subPDUs will be stored.
/// \param[in] lch_mng UE DL logical channel manager.
/// \param[in] total_tbs available space in bytes for subPDUs.
/// \return Total number of bytes allocated (including MAC subheaders).
/// \remark Excludes UE Contention Resolution Identity CE.
unsigned allocate_mac_ces(dl_msg_tb_info& tb_info, ue_logical_channel_repository& lch_mng, units::bytes total_tbs);

/// \brief Allocate MAC subPDUs for pending UE Contention Resolution Identity MAC CE.
/// \param[in] tb_info TB on which MAC subPDUs will be stored.
/// \param[in] lch_mng UE DL logical channel manager.
/// \param[in] total_tbs available space in bytes for subPDUs.
/// \return Total number of bytes allocated (including MAC subheaders).
unsigned
allocate_ue_con_res_id_mac_ce(dl_msg_tb_info& tb_info, ue_logical_channel_repository& lch_mng, units::bytes total_tbs);

/// \brief Defines the list of subPDUs, including LCID and payload size, that will compose the transport block for
/// SRB0 or for SRB1 in fallback mode.
/// It includes the UE Contention Resolution Identity CE if it is pending.
/// \return Returns the number of bytes reserved in the TB for subPDUs (other than padding).
unsigned build_dl_fallback_transport_block_info(dl_msg_tb_info&                tb_info,
                                                ue_logical_channel_repository& lch_mng,
                                                units::bytes                   tb_size_bytes);

/// \brief Defines the list of subPDUs, including LCID and payload size, that will compose the transport block for a
/// given RAN slice.
/// \return Returns the number of bytes reserved in the TB for subPDUs (other than padding).
/// \remark Excludes SRB0, as this operation is specific to a given RAN slice.
unsigned build_dl_transport_block_info(dl_msg_tb_info&                 tb_info,
                                       ue_logical_channel_repository&  lch_mng,
                                       units::bytes                    tb_size_bytes,
                                       ran_slice_id_t                  slice_id,
                                       dl_transport_block_restrictions restrictions);

} // namespace ocudu
