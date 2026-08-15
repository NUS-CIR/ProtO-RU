// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../receiver/ofh_ru_cplane_scheduling_dispatcher.h"
#include "../receiver/ofh_ru_rx_cplane_data_flow.h"
#include "../receiver/ofh_ru_rx_uplane_data_flow.h"
#include "../transmitter/ofh_data_flow_uplane_data.h"
#include "ofh_ru_cplane_scheduling_handler.h"
#include "ofh_ru_downlink_rx_window_handler.h"
#include "ofh_ru_message_receiver.h"
#include "ofh_ru_message_transmitter.h"
#include "ofh_ru_ota_symbol_task_dispatcher.h"
#include "ofh_ru_prach_window_responder.h"
#include "ofh_ru_uplink_request_repository.h"
#include "ofh_ru_uplink_scheduling_recorder.h"
#include "ofh_ru_uplink_symbol_responder.h"
#include "ocudu/ofh/ru_sector.h"
#include <array>
#include <memory>

namespace ocudu {
namespace ofh {

/// \brief Store-and-respond uplink components, populated only when the sector runs in
/// \ref ru_uplink_response_mode::store_and_respond (empty otherwise).
struct ru_uplink_response_components {
  /// Uplink request repository, written by the recorder and consumed by the uplink responder.
  std::shared_ptr<ru_uplink_request_repository> ul_request_repo;
  /// PRACH request repository, written by the recorder and consumed by the PRACH responder.
  std::shared_ptr<ru_uplink_request_repository> prach_request_repo;
  /// Records received uplink/PRACH Control-Plane requests (the dispatcher's uplink handler in this mode).
  std::unique_ptr<ru_uplink_scheduling_recorder> recorder;
  /// Builds uplink User-Plane replies from captured uplink IQ.
  std::unique_ptr<ru_uplink_symbol_responder> ul_responder;
  /// Builds PRACH User-Plane replies from captured PRACH IQ.
  std::unique_ptr<ru_prach_window_responder> prach_responder;
};

/// O-RU sector implementation. See \ref ru_sector for the public interface.
class ru_sector_impl : public ru_sector, public ru_uplink_iq_sink
{
public:
  ru_sector_impl(std::shared_ptr<rx_grid_context_repository>      grid_repo_,
                 std::shared_ptr<ether::eth_frame_pool>           uplink_uplane_pool_,
                 std::shared_ptr<ether::eth_frame_pool>           prach_pool_,
                 std::unique_ptr<data_flow_uplane_data>           uplink_data_flow_,
                 std::unique_ptr<data_flow_uplane_data>           prach_data_flow_,
                 std::unique_ptr<ru_cplane_scheduling_handler>    scheduling_handler_,
                 std::unique_ptr<ru_cplane_scheduling_dispatcher> cplane_dispatcher_,
                 std::unique_ptr<ru_rx_uplane_data_flow>          rx_uplane_data_flow_,
                 std::unique_ptr<ru_rx_cplane_data_flow>          rx_cplane_data_flow_,
                 std::unique_ptr<sequence_id_checker>             uplane_seq_id_checker_,
                 std::unique_ptr<sequence_id_checker>             cplane_dl_seq_id_checker_,
                 std::unique_ptr<sequence_id_checker>             cplane_ul_seq_id_checker_,
                 std::unique_ptr<rx_window_checker>               window_checker_,
                 std::unique_ptr<ru_downlink_rx_window_handler>   dl_rx_window_handler_,
                 std::unique_ptr<ru_message_receiver>             msg_receiver_,
                 std::unique_ptr<ru_message_transmitter>          msg_transmitter_,
                 std::unique_ptr<ru_ota_symbol_task_dispatcher>   msg_tx_dispatcher_,
                 ru_uplink_response_components                    uplink_components_);

  // See interface for documentation.
  void on_new_frame(span<const uint8_t> payload) override { msg_receiver->process_frame(payload); }

  // See interface for documentation.
  span<ota_symbol_boundary_notifier* const> get_ota_symbol_boundary_notifiers() override { return ota_notifiers; }

  // See interface for documentation.
  uint64_t get_nof_rx_on_time_messages() const override { return window_checker->nof_on_time_messages(); }

  // See interface for documentation.
  uint64_t get_nof_rx_early_messages() const override { return window_checker->nof_early_messages(); }

  // See interface for documentation.
  uint64_t get_nof_rx_late_messages() const override { return window_checker->nof_late_messages(); }

  // See interface for documentation.
  ru_uplink_iq_sink* get_uplink_iq_sink() override { return uplink_components.ul_responder ? this : nullptr; }

  // See interface for documentation.
  std::optional<ru_prach_occasion> get_recorded_prach_occasion(slot_point slot) const override
  {
    return uplink_components.prach_responder ? uplink_components.prach_responder->peek_prach_occasion(slot)
                                             : std::nullopt;
  }

  // See ru_uplink_iq_sink for documentation.
  void handle_uplink_symbol(slot_point slot, unsigned symbol, const shared_resource_grid& grid) override
  {
    if (uplink_components.ul_responder) {
      uplink_components.ul_responder->handle_uplink_symbol(slot, symbol, grid);
    }
  }

  // See ru_uplink_iq_sink for documentation.
  void handle_prach_window(slot_point slot, const prach_buffer& buffer) override
  {
    if (uplink_components.prach_responder) {
      uplink_components.prach_responder->handle_prach_window(slot, buffer);
    }
  }

private:
  // Declared so that referencing components are destroyed before the components they reference.
  std::shared_ptr<rx_grid_context_repository>      grid_repo;
  std::shared_ptr<ether::eth_frame_pool>           uplink_uplane_pool;
  std::shared_ptr<ether::eth_frame_pool>           prach_pool;
  std::unique_ptr<data_flow_uplane_data>           uplink_data_flow;
  std::unique_ptr<data_flow_uplane_data>           prach_data_flow;
  std::unique_ptr<ru_cplane_scheduling_handler>    scheduling_handler;
  std::unique_ptr<ru_cplane_scheduling_dispatcher> cplane_dispatcher;
  std::unique_ptr<ru_rx_uplane_data_flow>          rx_uplane_data_flow;
  std::unique_ptr<ru_rx_cplane_data_flow>          rx_cplane_data_flow;
  std::unique_ptr<sequence_id_checker>             uplane_seq_id_checker;
  std::unique_ptr<sequence_id_checker>             cplane_dl_seq_id_checker;
  std::unique_ptr<sequence_id_checker>             cplane_ul_seq_id_checker;
  std::unique_ptr<rx_window_checker>               window_checker;
  std::unique_ptr<ru_downlink_rx_window_handler>   dl_rx_window_handler;
  std::unique_ptr<ru_message_receiver>             msg_receiver;
  std::unique_ptr<ru_message_transmitter>          msg_transmitter;
  /// Optional decorator that defers the message transmitter's per-symbol Ethernet send onto the OFH transmit executor
  /// (null when running the send inline on the timing thread).
  std::unique_ptr<ru_ota_symbol_task_dispatcher> msg_tx_dispatcher;
  /// Store-and-respond uplink components (empty in immediate mode).
  ru_uplink_response_components uplink_components;
  /// OTA symbol boundary notifiers exposed for the application to subscribe to its timing source.
  std::array<ota_symbol_boundary_notifier*, 3> ota_notifiers;
};

} // namespace ofh
} // namespace ocudu
