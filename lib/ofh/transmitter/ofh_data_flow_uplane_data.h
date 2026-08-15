// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_data_flow_cuplane_encoding_metrics_collector.h"
#include "ocudu/ofh/serdes/ofh_message_properties.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_data_flow_metrics.h"
#include "ocudu/ran/prach/prach_subcarrier_spacing.h"
#include "ocudu/ran/resource_allocation/ofdm_symbol_range.h"
#include "ocudu/ran/slot_point.h"

namespace ocudu {
struct resource_grid_context;
class shared_resource_grid;
class prach_buffer;

namespace ofh {

class operation_controller;

/// Open Fronthaul User-Plane data flow resource grid context.
struct data_flow_uplane_resource_grid_context {
  /// Provides the slot context within the system frame.
  slot_point slot;
  /// Provides the sector identifier.
  uint8_t sector;
  /// Provides the port identifier.
  uint8_t port;
  /// eAxC.
  uint8_t eaxc;
  /// Symbol range.
  ofdm_symbol_range symbol_range;
  /// Section identifier. The O-DU transmitter always uses 0; an O-RU transmitter echoes the identifier of the uplink
  /// Control-Plane request it is replying to.
  uint16_t section_id = 0;
};

/// Open Fronthaul User-Plane PRACH data flow context.
struct data_flow_uplane_prach_context {
  /// Slot context within the system frame.
  slot_point slot;
  /// Sector identifier.
  uint8_t sector;
  /// PRACH buffer port to read the preamble from.
  uint8_t port;
  /// eAxC.
  uint8_t eaxc;
  /// Starting PRB of the PRACH data section.
  uint16_t prb_start;
  /// Number of contiguous PRBs of the PRACH data section.
  unsigned nof_prb;
  /// First OFDM symbol of the PRACH occasion within the slot.
  unsigned start_symbol;
  /// Number of PRACH OFDM symbols.
  unsigned nof_symbols;
  /// Filter index identifying the PRACH preamble format.
  filter_index_type filter_index;
  /// PRACH subcarrier spacing decoded from the section type 3 frameStructure field.
  prach_subcarrier_spacing prach_scs = prach_subcarrier_spacing::invalid;
  /// Section identifier, echoed from the PRACH Control-Plane request being replied to.
  uint16_t section_id = 0;
};

/// Open Fronthaul User-Plane data flow.
class data_flow_uplane_data
{
public:
  /// Default destructor.
  virtual ~data_flow_uplane_data() = default;

  /// Returns the controller of this Open Fronthaul User-Plane data flow.
  virtual operation_controller& get_operation_controller() = 0;

  /// Enqueues the User-Plane data messages with the given context and resource grid.
  virtual void enqueue_section_type_1_message(const data_flow_uplane_resource_grid_context& context,
                                              const shared_resource_grid&                   grid) = 0;

  /// \brief Enqueues uplink PRACH User-Plane messages built from the given PRACH buffer and context.
  ///
  /// Default implementation does nothing; the encoding data flow overrides it. Intended for an O-RU transmitter, where
  /// PRACH and regular uplink U-Plane are kept in separate frame-pool space.
  virtual void enqueue_prach_message(const data_flow_uplane_prach_context& context, const prach_buffer& buffer) {}

  /// Returns the performance metrics collector of this data flow.
  virtual data_flow_message_encoding_metrics_collector* get_metrics_collector() = 0;
};

} // namespace ofh
} // namespace ocudu
