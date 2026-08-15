// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ethernet_receiver_impl.h"
#include "tests/unittests/support/task_executor_test_doubles.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/ethernet/ethernet_frame_notifier.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include "ocudu/ofh/ethernet/ethernet_receiver_metrics.h"
#include "ocudu/ofh/ethernet/ethernet_receiver_metrics_collector.h"
#include <array>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

using namespace ocudu;
using namespace ether;

namespace ocudu {
namespace ether {

class receiver_impl_test_accessor
{
public:
  static std::unique_ptr<receiver_impl>
  create(int socket_fd, task_executor& executor, ocudulog::basic_logger& logger, bool metrics_enabled)
  {
    return std::unique_ptr<receiver_impl>(new receiver_impl(socket_fd, executor, logger, metrics_enabled));
  }

  static void set_notifier(receiver_impl& receiver, frame_notifier& notifier) { receiver.notifier = &notifier; }

  static void receive(receiver_impl& receiver) { receiver.receive(); }
};

} // namespace ether
} // namespace ocudu

namespace {

class frame_notifier_spy : public frame_notifier
{
public:
  void on_new_frame(unique_rx_buffer buffer) override
  {
    span<const uint8_t> data = buffer.data();
    frame.assign(data.begin(), data.end());
    ++nof_notifications;
  }

  std::vector<uint8_t> frame;
  unsigned             nof_notifications = 0;
};

std::vector<uint8_t> make_ecpri_frame()
{
  std::vector<uint8_t> frame(64, 0);
  frame[12] = ECPRI_ETH_TYPE >> 8U;
  frame[13] = ECPRI_ETH_TYPE & 0xffU;
  return frame;
}

} // namespace

TEST(ethernet_receiver_impl_test, receives_frame_and_updates_metrics)
{
  std::array<int, 2> sockets{};
  ASSERT_EQ(::socketpair(AF_UNIX, SOCK_DGRAM, 0, sockets.data()), 0);

  manual_task_worker_always_enqueue_tasks executor(1);
  auto receiver = receiver_impl_test_accessor::create(sockets[1], executor, ocudulog::fetch_basic_logger("TEST"), true);
  frame_notifier_spy notifier;
  receiver_impl_test_accessor::set_notifier(*receiver, notifier);

  const std::vector<uint8_t> frame = make_ecpri_frame();
  ASSERT_EQ(::send(sockets[0], frame.data(), frame.size(), 0), static_cast<ssize_t>(frame.size()));

  receiver_impl_test_accessor::receive(*receiver);

  ASSERT_EQ(notifier.nof_notifications, 1U);
  EXPECT_EQ(notifier.frame, frame);

  receiver_metrics metrics{};
  ASSERT_NE(receiver->get_metrics_collector(), nullptr);
  receiver->get_metrics_collector()->collect_metrics(metrics);
  EXPECT_EQ(metrics.total_nof_bytes, frame.size());

  receiver.reset();
  ::close(sockets[0]);
}
