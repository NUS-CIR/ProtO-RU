// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/expected.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg.h"
#include "ocudu/support/async/event_signal.h"
#include "ocudu/support/async/protocol_transaction_manager.h"

namespace ocudu {
namespace ocucp {

using rrc_outcome     = asn1::rrc_nr::ul_dcch_msg_s;
using rrc_transaction = protocol_transaction<rrc_outcome>;

class rrc_ue_event_manager
{
public:
  /// Transaction Response Container, which gets indexed by transaction_id.
  static constexpr size_t                   MAX_NOF_TRANSACTIONS = 4; // Two bit RRC transaction id
  protocol_transaction_manager<rrc_outcome> transactions;

  void cancel_all(protocol_transaction_failure cause = protocol_transaction_failure::cancel)
  {
    for (unsigned tid = 0; tid != rrc_ue_event_manager::MAX_NOF_TRANSACTIONS; ++tid) {
      transactions.cancel_transaction(tid, cause);
    }
  }

  explicit rrc_ue_event_manager(timer_factory timers) : transactions(MAX_NOF_TRANSACTIONS, timers) {}
  ~rrc_ue_event_manager() { cancel_all(); }
};

} // namespace ocucp
} // namespace ocudu
