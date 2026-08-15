// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ofh/compression/compression_params.h"
#include "ocudu/ofh/serdes/ofh_message_properties.h"
#include "ocudu/ran/slot_point.h"

namespace ocudu {
namespace ofh {

/// Open Fronthaul User-Plane message parameters.
struct uplane_message_params {
  /// Data direction.
  data_direction direction;
  /// Frame, subframe and slot information.
  slot_point slot;
  /// Filter index.
  filter_index_type filter_index;
  /// Initial PRB index used in the given /c symbol_id.
  unsigned start_prb;
  /// Number of PRBs in this symbol.
  unsigned nof_prb;
  /// Symbol identifier.
  unsigned symbol_id;
  /// Section type of the message to be built.
  section_type sect_type;
  /// IQ data compression parameters.
  ru_compression_params compression_params;
  /// Section identifier. The O-DU transmitter always uses 0; an O-RU transmitter echoes the identifier of the
  /// Control-Plane request it is replying to.
  uint16_t section_id = 0;
};

} // namespace ofh
} // namespace ocudu
