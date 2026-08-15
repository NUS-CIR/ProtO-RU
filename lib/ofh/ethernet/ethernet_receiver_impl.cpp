// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ethernet_receiver_impl.h"
#include "ethernet_constants.h"
#include "ethernet_receiver_utils.h"
#include "ethernet_rx_buffer_impl.h"
#include "ocudu/instrumentation/traces/ofh_traces.h"
#include "ocudu/ofh/ethernet/ethernet_frame_notifier.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include "ocudu/support/error_handling.h"
#include "ocudu/support/executors/task_executor.h"
#include "ocudu/support/synchronization/sync_event.h"
#include <arpa/inet.h>
#include <array>
#include <cstring>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

using namespace ocudu;
using namespace ether;

namespace {

class dummy_frame_notifier : public frame_notifier
{
  // See interface for documentation.
  void on_new_frame(ether::unique_rx_buffer buffer) override {}
};

} // namespace

/// This dummy object is passed to the constructor of the receiver implementation as a placeholder for the
/// actual frame notifier, which will be later set up through the \ref start() method.
static dummy_frame_notifier dummy_notifier;

receiver_impl::receiver_impl(const receiver_config& config, task_executor& executor_, ocudulog::basic_logger& logger_) :
  receiver_impl(-1, executor_, logger_, config.are_metrics_enabled)
{
  // ETH_P_ALL is required to receive an intact 802.1Q header on interfaces such as veth. A socket filter installed
  // immediately below ensures that unrelated traffic does not enter the userspace real-time path.
  socket_fd = ::socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (socket_fd < 0) {
    report_error("Unable to open raw socket for Ethernet receiver: {}", ::strerror(errno));
  }

  if (!attach_ecpri_rx_filter(socket_fd)) {
    report_error("Unable to attach eCPRI filter to Ethernet receiver socket: {}", ::strerror(errno));
  }

#ifdef PACKET_IGNORE_OUTGOING
  int ignore_outgoing = 1;
  if (::setsockopt(socket_fd, SOL_PACKET, PACKET_IGNORE_OUTGOING, &ignore_outgoing, sizeof(ignore_outgoing)) < 0) {
    logger.warning("Unable to suppress outgoing packets on Ethernet receiver socket: {}", ::strerror(errno));
  }
#endif

  int auxdata_enabled = 1;
  if (::setsockopt(socket_fd, SOL_PACKET, PACKET_AUXDATA, &auxdata_enabled, sizeof(auxdata_enabled)) < 0) {
    report_error("Unable to enable VLAN metadata on Ethernet receiver socket: {}", ::strerror(errno));
  }

  if (config.interface.size() > (IFNAMSIZ - 1)) {
    report_error("The Ethernet receiver interface name '{}' exceeds the maximum allowed length", config.interface);
  }

  if (config.is_promiscuous_mode_enabled) {
    // Set interface to promiscuous mode.
    ::ifreq if_opts;
    ::strncpy(if_opts.ifr_name, config.interface.c_str(), IFNAMSIZ - 1);
    if (::ioctl(socket_fd, SIOCGIFFLAGS, &if_opts) < 0) {
      report_error("Unable to get flags for NIC interface '{}' in the Ethernet receiver", config.interface);
    }
    if_opts.ifr_flags |= IFF_PROMISC;
    if (::ioctl(socket_fd, SIOCSIFFLAGS, &if_opts) < 0) {
      report_error("Unable to set flags for NIC interface '{}' in the Ethernet receiver", config.interface);
    }
  }

  const unsigned interface_index = ::if_nametoindex(config.interface.c_str());
  if (interface_index == 0) {
    report_error(
        "Unable to find the NIC interface '{}' for Ethernet receiver: {}", config.interface, ::strerror(errno));
  }

  ::sockaddr_ll socket_address = {};
  socket_address.sll_family    = AF_PACKET;
  socket_address.sll_protocol  = htons(ETH_P_ALL);
  socket_address.sll_ifindex   = interface_index;
  if (::bind(socket_fd, reinterpret_cast<const ::sockaddr*>(&socket_address), sizeof(socket_address)) < 0) {
    report_error("Unable to bind socket to the NIC interface '{}' in Ethernet receiver: {}",
                 config.interface,
                 ::strerror(errno));
  }

  logger.info("Opened successfully the NIC interface '{}' (fd = '{}') used by the Ethernet receiver",
              config.interface,
              socket_fd);
}

receiver_impl::receiver_impl(int                     socket_fd_,
                             task_executor&          executor_,
                             ocudulog::basic_logger& logger_,
                             bool                    are_metrics_enabled) :
  logger(logger_),
  executor(executor_),
  notifier(&dummy_notifier),
  socket_fd(socket_fd_),
  buffer_pool(BUFFER_SIZE + ETH_VLAN_TAG_SIZE.value()),
  metrics_collector(are_metrics_enabled)
{
}

receiver_impl::~receiver_impl()
{
  ::close(socket_fd);
}

void receiver_impl::start(frame_notifier& notifier_)
{
  logger.info("Starting the ethernet frame receiver");

  stop_manager.reset();

  notifier = &notifier_;

  sync_event wait_event;
  if (!executor.defer([this, token = wait_event.get_token()] { receive_loop(); })) {
    report_error("Unable to start the ethernet frame receiver, fd = '{}'", socket_fd);
  }

  // Block waiting for receiver executor to start.
  wait_event.wait();

  logger.info("Started the ethernet frame receiver with fd = '{}'", socket_fd);
}

void receiver_impl::stop()
{
  logger.info("Requesting stop of the ethernet frame receiver with fd = '{}'", socket_fd);
  stop_manager.stop();
  logger.info("Stopped the ethernet frame receiver with fd = '{}'", socket_fd);
}

void receiver_impl::receive_loop()
{
  auto token = stop_manager.get_token();
  if (OCUDU_UNLIKELY(token.is_stop_requested())) {
    return;
  }

  receive();

  // Retry the task deferring when it fails.
  while (!executor.defer([this, tk = std::move(token)]() { receive_loop(); })) {
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}

/// Blocking function that waits for incoming data over the socket or until the specified timeout expires.
static bool wait_for_data(int socket, std::chrono::microseconds timeout)
{
  fd_set read_fs;
  FD_ZERO(&read_fs);
  FD_SET(socket, &read_fs);
  timeval tv = {0, static_cast<__suseconds_t>(timeout.count())};

  return (::select(socket + 1, &read_fs, nullptr, nullptr, &tv) > 0);
}

void receiver_impl::receive()
{
  if (!wait_for_data(socket_fd, std::chrono::microseconds(5))) {
    return;
  }

  auto        meas = metrics_collector.create_time_execution_measurer();
  trace_point tp   = ofh_tracer.now();

  auto exp_buffer = buffer_pool.reserve();
  if (!exp_buffer) {
    logger.warning("No buffer is available for receiving an Ethernet packet on the port bound to fd = '{}'", socket_fd);
    return;
  }
  ethernet_rx_buffer_impl buffer    = std::move(*exp_buffer);
  span<uint8_t>           data_span = buffer.storage();

  ::iovec io_vector = {.iov_base = data_span.data(), .iov_len = BUFFER_SIZE};
  alignas(::cmsghdr) std::array<uint8_t, CMSG_SPACE(sizeof(::tpacket_auxdata))> control_buffer{};
  ::sockaddr_ll                                                                 packet_address = {};
  ::msghdr                                                                      message        = {};
  message.msg_name                                                                             = &packet_address;
  message.msg_namelen                                                                          = sizeof(packet_address);
  message.msg_iov                                                                              = &io_vector;
  message.msg_iovlen                                                                           = 1;
  message.msg_control                                                                          = control_buffer.data();
  message.msg_controllen                                                                       = control_buffer.size();

  auto nof_bytes = ::recvmsg(socket_fd, &message, 0);

  if (nof_bytes < 0) {
    logger.warning("Ethernet receiver call to recvmsg failed, fd = '{}'", socket_fd);
    metrics_collector.update_stats(meas.stop());
    return;
  }

  if (packet_address.sll_pkttype == PACKET_OUTGOING) {
    metrics_collector.update_stats(meas.stop());
    return;
  }

  if ((message.msg_flags & (MSG_TRUNC | MSG_CTRUNC)) != 0) {
    logger.warning("Dropped truncated Ethernet frame received on fd = '{}'", socket_fd);
    metrics_collector.update_stats(meas.stop());
    return;
  }

  std::optional<rx_vlan_metadata> vlan_metadata = extract_rx_vlan_metadata(message);

  std::size_t frame_size = nof_bytes;
  if (normalize_rx_frame(data_span, frame_size, vlan_metadata) != rx_frame_normalization_result::success) {
    logger.warning("Dropped malformed Ethernet frame received on fd = '{}'", socket_fd);
    metrics_collector.update_stats(meas.stop());
    return;
  }

  buffer.resize(frame_size);

  metrics_collector.update_stats(meas.stop(), frame_size);

  notifier->on_new_frame(unique_rx_buffer(std::move(buffer)));
  ofh_tracer << trace_event("ofh_receiver", tp);
}

receiver_metrics_collector* receiver_impl::get_metrics_collector()
{
  return metrics_collector.disabled() ? nullptr : &metrics_collector;
}
