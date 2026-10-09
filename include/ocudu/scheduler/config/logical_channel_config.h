// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/mac/mac_lc_config.h"
#include "ocudu/ran/logical_channel/lcid.h"
#include "ocudu/ran/qos/arp_prio_level.h"
#include "ocudu/ran/qos/five_qi_qos_mapping.h"
#include "ocudu/ran/qos/qos_parameters.h"
#include "ocudu/ran/rrm.h"
#include "ocudu/ran/sr_configuration.h"
#include "ocudu/scheduler/config/logical_channel_group.h"

namespace ocudu {

/// \c LogicalChannelConfig, TS 38.331.
/// Information relative to the scheduling of a logical channel in the MAC scheduler.
struct logical_channel_config {
  /// QoS specific features associated with a logical channel (only used by DRBs).
  struct qos_info {
    /// QoS characteristics associated with the logical channel.
    standardized_qos_characteristics qos;
    /// The ARP Priority Level indicates a priority in scheduling resources among QoS Flows. The lowest Priority Level
    /// value corresponds to the highest priority. See TS 23.501, clause 5.7.2.2.
    arp_prio_level_t arp_priority;
    /// QoS information present only for GBR QoS flows.
    std::optional<gbr_qos_flow_information> gbr_qos_info;

    bool operator==(const qos_info& rhs) const
    {
      return qos == rhs.qos && arp_priority == rhs.arp_priority && gbr_qos_info == rhs.gbr_qos_info;
    }
  };

  lcid_t   lcid;
  lcg_id_t lc_group;
  /// Slice associated with this Bearer.
  rrm_policy_member rrm_policy;
  /// QoS information associated with this logical channel.
  std::optional<qos_info> qos;
  // TODO: add remaining fields;
  std::optional<scheduling_request_id> sr_id;
  bool                                 lc_sr_mask;
  bool                                 lc_sr_delay_timer_applied;
  // [Implementation defined] Configuration of triggeded UL grant feature.
  std::optional<mac_lc_config::triggered_ul_grant_cfg> triggered_ul_grant;
  /// UL HARQ mode a grant must use for this logical channel to be multiplexed into it, as per \c allowedHARQ-mode,
  /// TS 38.331. Unrestricted when absent.
  std::optional<ul_harq_mode> allowed_harq_mode;
  /// [Implementation-defined] DL HARQ feedback a process must carry for this logical channel to be multiplexed into
  /// its transport block. Unrestricted when absent.
  std::optional<dl_harq_feedback> allowed_dl_harq_feedback;

  bool operator==(const logical_channel_config& rhs) const
  {
    return lcid == rhs.lcid and lc_group == rhs.lc_group and rrm_policy == rhs.rrm_policy and qos == rhs.qos and
           sr_id == rhs.sr_id and lc_sr_mask == rhs.lc_sr_mask and
           lc_sr_delay_timer_applied == rhs.lc_sr_delay_timer_applied and
           triggered_ul_grant == rhs.triggered_ul_grant and allowed_harq_mode == rhs.allowed_harq_mode and
           allowed_dl_harq_feedback == rhs.allowed_dl_harq_feedback;
  }
};

/// \brief Restrictions a DL transport block carries.
///
/// A logical channel joins the block where the two agree, and closes the restrictions it sets for the channels after
/// it. A block carrying none serves every channel.
///
/// [Implementation-defined] TS 38.331 binds no logical channel to a DL HARQ feedback. The feedback a process carries
/// follows \c downlinkHARQ-FeedbackDisabled, which the cell sets per HARQ process.
struct dl_transport_block_restrictions {
  /// DL HARQ feedback the block carries, once a channel of the block restricts it.
  std::optional<dl_harq_feedback> harq_feedback;

  /// Checks whether the block carries any restriction, so that it serves only part of the logical channels.
  [[nodiscard]] bool is_restricted() const { return harq_feedback.has_value(); }

  /// Checks whether this block admits the given logical channel, which then joins the channels it carries.
  [[nodiscard]] bool admits(const logical_channel_config& lc_cfg) const
  {
    // A restriction either side leaves open agrees with the other.
    return not harq_feedback.has_value() or not lc_cfg.allowed_dl_harq_feedback.has_value() or
           harq_feedback == lc_cfg.allowed_dl_harq_feedback;
  }

  /// Takes on the restrictions the given logical channel sets, leaving the others open.
  void set(const logical_channel_config& lc_cfg)
  {
    if (lc_cfg.allowed_dl_harq_feedback.has_value()) {
      harq_feedback = lc_cfg.allowed_dl_harq_feedback;
    }
  }
};

/// \brief Restrictions a UL grant satisfies, as per the LCP mapping restrictions of TS 38.321, Section 5.4.3.1.2.
///
/// The UE serves a logical channel on the grant only where the two agree. A channel leaving a restriction unset rides
/// any grant.
struct ul_grant_restrictions {
  /// UL HARQ mode the grant uses, once a channel served by it restricts the mode.
  std::optional<ul_harq_mode> harq_mode;

  /// Checks whether the grant carries any restriction, so that it serves only part of the logical channels.
  [[nodiscard]] bool is_restricted() const { return harq_mode.has_value(); }

  /// Checks whether this grant admits the given logical channel, which the UE then serves on it.
  [[nodiscard]] bool admits(const logical_channel_config& lc_cfg) const
  {
    // A restriction either side leaves open agrees with the other.
    return not harq_mode.has_value() or not lc_cfg.allowed_harq_mode.has_value() or
           harq_mode == lc_cfg.allowed_harq_mode;
  }

  /// Takes on the restrictions the given logical channel sets, leaving the others open.
  void set(const logical_channel_config& lc_cfg)
  {
    if (lc_cfg.allowed_harq_mode.has_value()) {
      harq_mode = lc_cfg.allowed_harq_mode;
    }
  }
};

} // namespace ocudu
