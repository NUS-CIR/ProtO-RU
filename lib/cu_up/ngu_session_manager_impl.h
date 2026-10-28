// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ngu_session_manager.h"
#include "ocudu/adt/span.h"
#include <map>
#include <optional>
#include <vector>

namespace ocudu::ocuup {

class ngu_session_manager_impl : public ngu_session_manager
{
public:
  ~ngu_session_manager_impl() override = default;

  /// \brief Creates a manager over the given NG-U gateways.
  ///
  /// \param ngu_gws_ NG-U gateways.
  /// \param gw_snssais S-NSSAI each gateway is dedicated to, in the order of \c ngu_gws_. A gateway without an entry,
  /// or with an empty one, is a default gateway.
  explicit ngu_session_manager_impl(const std::vector<std::unique_ptr<gtpu_tnl_pdu_session>>& ngu_gws_,
                                    span<const std::optional<s_nssai_t>>                      gw_snssais = {});

  gtpu_tnl_pdu_session* get_next_ngu_gateway(const s_nssai_t& snssai) override;

private:
  /// Gateways that take turns serving the PDU sessions of one S-NSSAI.
  struct gateway_group {
    /// Indexes into \c ngu_gws.
    std::vector<unsigned> gw_indexes;
    /// Position in \c gw_indexes of the next gateway to hand out.
    unsigned next = 0;
  };

  /// Returns the next gateway of the given group, in round-robin order.
  gtpu_tnl_pdu_session& next_in_group(gateway_group& group);

  const std::vector<std::unique_ptr<gtpu_tnl_pdu_session>>& ngu_gws;
  /// Groups of the gateways dedicated to an S-NSSAI.
  std::map<s_nssai_t, gateway_group> dedicated_groups;
  /// Group of the default gateways.
  gateway_group default_group;
};

} // namespace ocudu::ocuup
