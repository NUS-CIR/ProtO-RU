// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/gtpu/gtpu_gateway.h"
#include "ocudu/ran/s_nssai.h"

namespace ocudu::ocuup {

class ngu_session_manager
{
public:
  virtual ~ngu_session_manager() = default;

  /// \brief Returns the NG-U gateway of a new PDU session of the given S-NSSAI.
  ///
  /// Gateways dedicated to the S-NSSAI take precedence over the default ones. Returns nullptr when no gateway serves
  /// the S-NSSAI.
  virtual gtpu_tnl_pdu_session* get_next_ngu_gateway(const s_nssai_t& snssai) = 0;
};

} // namespace ocudu::ocuup
