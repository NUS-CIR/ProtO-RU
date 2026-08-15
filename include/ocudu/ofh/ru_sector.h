// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/span.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/compression/compression_params.h"
#include "ocudu/ofh/ethernet/ethernet_transmitter.h"
#include "ocudu/ofh/ethernet/vlan_ethernet_frame_params.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/receiver/ofh_receiver_timing_parameters.h"
#include "ocudu/ofh/ru_upper_phy.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/prach/prach_subcarrier_spacing.h"
#include "ocudu/ran/slot_point.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include "ocudu/support/units.h"
#include <cstdint>
#include <memory>
#include <optional>

namespace ocudu {

class prach_buffer;
class task_executor;

namespace ofh {

class ota_symbol_boundary_notifier;

/// \brief Strategy the O-RU sector uses to answer an uplink/PRACH Control-Plane request.
enum class ru_uplink_response_mode {
  /// Reply immediately with the upper PHY's IQ when the Control-Plane request is received (the test-IQ emulator).
  immediate,
  /// Record the request and reply later, when the corresponding IQ is pushed in through the uplink IQ sink (a real
  /// radio, or a fake one). This is the real-O-RU model used by ProtO-RU.
  store_and_respond
};

/// \brief Sink through which captured uplink/PRACH IQ is pushed into the O-RU sector.
///
/// Driven by the application's IQ source (a real lower PHY / radio, or a fake one) when the sector runs in
/// \ref ru_uplink_response_mode::store_and_respond. Each captured symbol/window is matched against the recorded
/// Control-Plane request and turned into a User-Plane reply.
class ru_uplink_iq_sink
{
public:
  virtual ~ru_uplink_iq_sink() = default;

  /// Handles a captured uplink resource grid symbol.
  virtual void handle_uplink_symbol(slot_point slot, unsigned symbol, const shared_resource_grid& grid) = 0;

  /// Handles a captured PRACH window.
  virtual void handle_prach_window(slot_point slot, const prach_buffer& buffer) = 0;
};

/// \brief A PRACH Control-Plane occasion recorded by the O-RU sector (store-and-respond mode).
///
/// Returned by \ref ru_sector::get_recorded_prach_occasion so the application's radio driver knows the O-DU scheduled
/// a PRACH for the slot and can issue the matching capture request. The PRACH detector configuration (format, root
/// sequence, ...) is not carried by the O-RAN Control-Plane, so the driver supplies it out of band; this carries the
/// timing and frequency placement the Control-Plane provides.
struct ru_prach_occasion {
  /// Slot the PRACH occasion is scheduled in.
  slot_point slot;
  /// PRACH eAxC the request was received on.
  unsigned eaxc;
  /// OFDM symbol within the slot where the PRACH acquisition window starts.
  unsigned start_symbol;
  /// PRACH allocation offset in PUSCH-grid RBs, derived from the section type 3 frequencyOffset.
  unsigned rb_offset = 0;
  /// PRACH subcarrier spacing decoded from the section type 3 frameStructure field.
  prach_subcarrier_spacing prach_scs = prach_subcarrier_spacing::invalid;
};

/// O-RU sector configuration.
struct ru_sector_config {
  /// Radio sector identifier.
  unsigned sector;
  /// Subcarrier spacing.
  subcarrier_spacing scs;
  /// Cyclic prefix.
  cyclic_prefix cp;
  /// RU bandwidth in PRBs.
  unsigned ru_nof_prbs;
  /// Uplink User-Plane compression (the O-RU compresses the captured/test uplink IQ it transmits).
  ru_compression_params ul_compr_params;
  /// Downlink User-Plane compression (the O-RU decompresses the downlink IQ it receives).
  ru_compression_params dl_compr_params;
  /// PRACH User-Plane compression (the O-RU compresses the captured/test PRACH IQ it transmits).
  ru_compression_params prach_compr_params;
  /// IQ scaling applied before compression.
  float iq_scaling = 1.0F;
  /// Static (out-of-band) compression header for the uplink/PRACH User-Plane the O-RU transmits.
  bool is_ul_static_compr_hdr = false;
  /// Static (out-of-band) compression header expected on the downlink User-Plane the O-RU receives.
  bool is_dl_static_compr_hdr = false;
  /// \brief Ethernet (VLAN) frame parameters.
  ///
  /// An empty \c vlan_config transmits untagged frames, for interfaces that insert the VLAN tag themselves.
  ether::vlan_frame_params vlan_params;
  /// Downlink eAxCs (the eAxCs the O-RU receives downlink User-Plane on).
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  /// Uplink eAxCs (regular uplink User-Plane).
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  /// PRACH eAxCs.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc;
  /// Maximum Ethernet frame (MTU) size in bytes.
  units::bytes mtu_size;
  /// Number of frames per symbol in each transmit frame pool.
  unsigned nof_frames_per_symbol;
  /// Transmit window start, in symbols ahead of the current OTA symbol.
  unsigned tx_window_start_symbols;
  /// Transmit window end, in symbols ahead of the current OTA symbol.
  unsigned tx_window_end_symbols;
  /// Reception window timing parameters (for the receive-window checker).
  rx_window_timing_parameters rx_window;
  /// Strategy used to answer uplink/PRACH Control-Plane requests.
  ru_uplink_response_mode uplink_response_mode = ru_uplink_response_mode::immediate;
  /// \brief Offset, in symbols behind the current OTA symbol, at which a received downlink symbol is finalized
  /// (handed back to the upper PHY and released to its pool).
  ///
  /// Positive values finalize after the symbol's air time: the default of one slot behind suits applications that
  /// only consume the downlink for statistics (the test-IQ emulator). Negative values finalize ahead of air time,
  /// required when a radio transmits the grid over the air: the grid must reach the radio before its processing
  /// pipeline (max_proc_delay slots ahead of air) picks the slot up. Downlink User-Plane arriving after the finalize
  /// point is dropped, so the advertised T2a_max_up window must deliver the data early enough to cover this offset.
  int dl_grid_finalize_offset_symbols = 14;
};

/// O-RU sector dependencies provided by the application.
struct ru_sector_dependencies {
  /// Logger.
  ocudulog::basic_logger* logger = nullptr;
  /// Upper PHY boundary (the application's, possibly fake, PHY).
  ru_upper_phy* upper_phy = nullptr;
  /// Ethernet transmitter used to send frames.
  std::unique_ptr<ether::transmitter> eth_transmitter;
  /// \brief Optional OFH encode executor. When set, the uplink User-Plane build + compression is deferred to it
  /// (decoupled from the calling baseband / immediate-response thread, like the O-DU's downlink); null = run inline.
  task_executor* uplink_encode_executor = nullptr;
};

/// \brief O-RU sector interface.
///
/// Owns and wires the O-RU receive and transmit data path. The application drives it by feeding received Ethernet
/// frames to \ref on_new_frame and by subscribing the sector's OTA symbol boundary notifiers (returned by
/// \ref get_ota_symbol_boundary_notifiers) to its timing source.
class ru_sector
{
public:
  virtual ~ru_sector() = default;

  /// Processes a received Ethernet frame.
  virtual void on_new_frame(span<const uint8_t> payload) = 0;

  /// \brief Returns the OTA symbol boundary notifiers the application must subscribe to its timing source.
  ///
  /// They are the message transmitter (drains the transmit pools each symbol), the reception-window checker (advances
  /// its notion of the current OTA symbol) and the downlink reception window handler (finalises received downlink
  /// grids).
  virtual span<ota_symbol_boundary_notifier* const> get_ota_symbol_boundary_notifiers() = 0;

  /// Returns the number of received messages that arrived on time within the reception window.
  virtual uint64_t get_nof_rx_on_time_messages() const = 0;

  /// Returns the number of received messages that arrived before the reception window opened.
  virtual uint64_t get_nof_rx_early_messages() const = 0;

  /// Returns the number of received messages that arrived after the reception window closed.
  virtual uint64_t get_nof_rx_late_messages() const = 0;

  /// \brief Returns the uplink IQ sink to push captured uplink/PRACH IQ into, or nullptr.
  ///
  /// Non-null only when the sector runs in \ref ru_uplink_response_mode::store_and_respond; in immediate mode the
  /// sector replies from the upper PHY directly and there is no sink to drive.
  virtual ru_uplink_iq_sink* get_uplink_iq_sink() = 0;

  /// \brief Returns the recorded PRACH Control-Plane occasion for the given slot, if any.
  ///
  /// In \ref ru_uplink_response_mode::store_and_respond the application's radio driver calls this each slot to learn
  /// whether the O-DU scheduled a PRACH, so it can issue the matching capture request into the uplink IQ sink. Returns
  /// nullopt in immediate mode or when no PRACH was recorded for the slot.
  virtual std::optional<ru_prach_occasion> get_recorded_prach_occasion(slot_point slot) const = 0;
};

/// Creates an O-RU sector from the given configuration and dependencies.
std::unique_ptr<ru_sector> create_ru_sector(const ru_sector_config& config, ru_sector_dependencies&& dependencies);

} // namespace ofh
} // namespace ocudu
