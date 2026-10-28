// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ngu_session_manager_impl.h"
#include "ocudu/support/ocudu_assert.h"

using namespace ocudu;
using namespace ocuup;

ngu_session_manager_impl::ngu_session_manager_impl(const std::vector<std::unique_ptr<gtpu_tnl_pdu_session>>& ngu_gws_,
                                                   span<const std::optional<s_nssai_t>> gw_snssais) :
  ngu_gws(ngu_gws_)
{
  ocudu_assert(not ngu_gws.empty(), "NG-U gateways cannot be empty");
  ocudu_assert(gw_snssais.size() <= ngu_gws.size(),
               "More NG-U gateway S-NSSAIs ({}) than NG-U gateways ({})",
               gw_snssais.size(),
               ngu_gws.size());

  for (unsigned i = 0, e = ngu_gws.size(); i != e; ++i) {
    if (i < gw_snssais.size() and gw_snssais[i].has_value()) {
      dedicated_groups[*gw_snssais[i]].gw_indexes.push_back(i);
    } else {
      default_group.gw_indexes.push_back(i);
    }
  }
}

gtpu_tnl_pdu_session* ngu_session_manager_impl::get_next_ngu_gateway(const s_nssai_t& snssai)
{
  if (auto it = dedicated_groups.find(snssai); it != dedicated_groups.end()) {
    return &next_in_group(it->second);
  }
  // A slice without dedicated gateways never shares the gateways of another slice, so that the traffic of a dedicated
  // slice stays on its own sockets.
  if (default_group.gw_indexes.empty()) {
    return nullptr;
  }
  return &next_in_group(default_group);
}

gtpu_tnl_pdu_session& ngu_session_manager_impl::next_in_group(gateway_group& group)
{
  unsigned index = group.gw_indexes[group.next % group.gw_indexes.size()];
  ++group.next;
  return *ngu_gws[index];
}
