// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/fapi_adaptor/phy_dummy/phy_dummy_fapi_adaptor_config.h"

namespace ocudu {

/// FAPI dummy application unit configuration.
struct fapi_dummy_unit_config {
  /// Slot clock mode.
  fapi_adaptor::phy_dummy_fapi_adaptor_clock_mode clock_mode =
      fapi_adaptor::phy_dummy_fapi_adaptor_clock_mode::realtime;
  /// Number of slots between an uplink slot and the notification of its indications.
  unsigned ul_indication_delay_slots = 4;
  /// Number of slots a SLOT.indication is notified in advance of the transmission time.
  unsigned max_processing_delay_slots = 2;
};

} // namespace ocudu
