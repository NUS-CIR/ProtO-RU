// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ethernet_rx_buffer_pool.h"
#include "ethernet_rx_metrics_collector_impl.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ethernet/ethernet_controller.h"
#include "ocudu/ofh/ethernet/ethernet_receiver.h"
#include "ocudu/ofh/ethernet/ethernet_receiver_config.h"
#include "ocudu/support/synchronization/stop_event.h"

namespace ocudu {

class task_executor;

namespace ether {

/// Implementation for the Ethernet receiver.
class receiver_impl : public receiver, private receiver_operation_controller
{
  static constexpr unsigned BUFFER_SIZE = 9600;

  friend class receiver_impl_test_accessor;

public:
  receiver_impl(const receiver_config& config, task_executor& executor_, ocudulog::basic_logger& logger_);

  ~receiver_impl() override;

  // See interface for documentation.
  receiver_operation_controller& get_operation_controller() override { return *this; }

  // See interface for documentation.
  receiver_metrics_collector* get_metrics_collector() override;

private:
  /// Constructs a receiver around an existing socket for unit testing.
  receiver_impl(int socket_fd_, task_executor& executor_, ocudulog::basic_logger& logger_, bool are_metrics_enabled);

  // See interface for documentation.
  void start(frame_notifier& notifier_) override;

  // See interface for documentation.
  void stop() override;

  /// Main receiving loop.
  void receive_loop();

  /// Receives new Ethernet frames from the socket.
  ///
  /// \note This function will block until new frames become available.
  void receive();

private:
  ocudulog::basic_logger&         logger;
  task_executor&                  executor;
  frame_notifier*                 notifier;
  int                             socket_fd = -1;
  rt_stop_event_source            stop_manager;
  ethernet_rx_buffer_pool         buffer_pool;
  receiver_metrics_collector_impl metrics_collector;
};

} // namespace ether
} // namespace ocudu
