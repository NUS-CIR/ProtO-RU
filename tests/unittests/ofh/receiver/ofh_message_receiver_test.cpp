// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/receiver/ofh_closed_rx_window_handler.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "../../../../lib/ofh/receiver/ofh_data_flow_uplane_decoding_metrics_collector.h"
#include "../../../../lib/ofh/receiver/ofh_message_receiver_impl.h"
#include "../../../../lib/ofh/receiver/ofh_rx_window_checker.h"
#include "../../../../lib/ofh/receiver/ofh_sequence_id_checker_dummy_impl.h"
#include "../../support/task_executor_test_doubles.h"
#include "../compression/ofh_iq_decompressor_test_doubles.h"
#include "ocudu/ofh/ethernet/ethernet_controller.h"
#include "ocudu/ofh/ethernet/ethernet_receiver_metrics_collector.h"
#include "ocudu/ofh/ethernet/ethernet_unique_buffer.h"
#include "ocudu/ofh/ofh_factories.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;
using namespace ocudu::ofh::testing;

namespace {

/// Dummy User-Plane received symbol notifier.
class dummy_uplane_rx_symbol_notifier : public uplane_rx_symbol_notifier
{
public:
  void on_new_uplink_symbol(const uplane_rx_symbol_context& context, shared_resource_grid grid, bool is_valid) override
  {
  }
  void on_new_prach_window_data(const prach_buffer_context& context, shared_prach_buffer buffer) override {}
};

/// Dummy Ethernet receive buffer.
class dummy_eth_rx_buffer : public ether::rx_buffer
{
public:
  explicit dummy_eth_rx_buffer(std::vector<uint8_t>&& init_values) { buffer = init_values; }

  span<const uint8_t> data() const override { return buffer; }

private:
  std::vector<uint8_t> buffer;
};

/// Data flow User-Plane uplink PRACH spy.
class data_flow_uplane_uplink_prach_spy : public data_flow_uplane_uplink_prach
{
  bool                                         decode_function_called = false;
  data_flow_message_decoding_metrics_collector metrics_collector{false};

public:
  // See interface for documentation.
  void decode_type1_message(unsigned eaxc, span<const uint8_t> msg, bool is_seq_id_correct) override
  {
    decode_function_called = true;
  }

  // See interface for documentation.
  data_flow_message_decoding_metrics_collector& get_metrics_collector() override { return metrics_collector; }

  /// Returns true if the decode_type1_message function has been called, otherwise false.
  bool has_decode_function_been_called() const { return decode_function_called; }
};

/// Data flow User-Plane uplink PRACH spy.
class data_flow_uplane_uplink_data_spy : public data_flow_uplane_uplink_data
{
  bool                                         decode_function_called = false;
  data_flow_message_decoding_metrics_collector metrics_collector{false};

public:
  // See interface for documentation.
  void decode_type1_message(unsigned eaxc, span<const uint8_t> msg, bool is_seq_id_correct) override
  {
    decode_function_called = true;
  }

  // See interface for documentation.
  data_flow_message_decoding_metrics_collector& get_metrics_collector() override { return metrics_collector; }

  /// Returns true if the decode_type1_message function has been called, otherwise false.
  bool has_decode_function_been_called() const { return decode_function_called; }

  /// Clears the record of the calls to the decode_type1_message function.
  void clear() { decode_function_called = false; }
};

/// VLAN frame decoder spy.
class vlan_frame_decoder_spy : public ether::vlan_frame_decoder
{
  bool                     decode_function_called = false;
  ether::vlan_frame_params eth_params;

public:
  explicit vlan_frame_decoder_spy(const ether::vlan_frame_params& eth_params_) : eth_params(eth_params_) {}

  span<const uint8_t> decode(span<const uint8_t> frame, ether::vlan_frame_params& eth_params_) override
  {
    decode_function_called = true;
    eth_params_            = eth_params;
    return frame;
  }

  /// Sets the VLAN frame parameters to the given one.
  void set_vlan_params(const ether::vlan_frame_params& eth_params_) { eth_params = eth_params_; }

  /// Returns true if the decode_type1_message function has been called, otherwise false.
  bool has_decode_function_been_called() const { return decode_function_called; }
};

/// eCPRI packet decoder spy.
class ecpri_packet_decoder_spy : public ecpri::packet_decoder
{
  bool                     decode_function_called = false;
  ecpri::packet_parameters ecpri_params;

public:
  explicit ecpri_packet_decoder_spy(const ecpri::packet_parameters& params) : ecpri_params(params) {}

  // See interface for documentation.
  span<const uint8_t> decode(span<const uint8_t> packet, ecpri::packet_parameters& params) override
  {
    decode_function_called = true;
    params                 = ecpri_params;
    return packet;
  }

  /// Sets the VLAN frame parameters to the given one.
  void set_ecpri_params(const ecpri::packet_parameters& params) { ecpri_params = params; }

  /// Returns true if the decode_type1_message function has been called, otherwise false.
  bool has_decode_function_been_called() const { return decode_function_called; }
};

/// Dummy Ethernet receiver class.
class dummy_eth_receiver : public ether::receiver, public ether::receiver_operation_controller
{
  // See interface for documentation.
  void start(ether::frame_notifier& notifier) override {}

  // See interface for documentation.
  void stop() override {}

  // See interface for documentation.
  ether::receiver_operation_controller& get_operation_controller() override { return *this; }

  // See interface for documentation.
  ether::receiver_metrics_collector* get_metrics_collector() override { return nullptr; }
};

} // namespace

class ofh_message_receiver_fixture : public ::testing::Test
{
protected:
  ether::vlan_frame_params                        vlan_params   = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
                                                                   {0x00, 0x00, 0x00, 0x00, 0x00, 0x02},
                                                                   ether::vlan_parameters{.tci_vid = 4},
                                                                   0xaefe};
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_prach_eaxc = {0, 1};
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc       = {4, 5};
  manual_task_worker_always_enqueue_tasks         executor;
  closed_rx_window_handler                        closed_window_handler;
  rx_window_checker                               window_checker;
  ecpri::packet_parameters                        ecpri_params;
  data_flow_uplane_uplink_data_spy*               df_uplink;
  data_flow_uplane_uplink_prach_spy*              df_prach;
  ecpri_packet_decoder_spy*                       ecpri_decoder;
  vlan_frame_decoder_spy*                         vlan_decoder;
  bool                                            check_seq_id;
  message_receiver_impl                           ul_handler;

public:
  explicit ofh_message_receiver_fixture(bool check_seq_id_ = false) :
    executor(20),
    closed_window_handler({},
                          {&ocudulog::fetch_basic_logger("TEST"),
                           &executor,
                           std::make_shared<prach_context_repository>(20),
                           std::make_shared<rx_grid_context_repository>(20),
                           std::make_shared<dummy_uplane_rx_symbol_notifier>()}),
    window_checker(false, {}),
    check_seq_id(check_seq_id_),
    ul_handler(generate_config(), generate_dependencies())
  {
    window_checker.on_new_symbol({{{1, 0}, 0, 14}, {}});
  }

  message_receiver_config generate_config()
  {
    return message_receiver_config{.sector                 = 0,
                                   .nof_symbols            = 14,
                                   .scs                    = subcarrier_spacing::kHz30,
                                   .vlan_params            = vlan_params,
                                   .prach_eaxc             = ul_prach_eaxc,
                                   .ul_eaxc                = ul_eaxc,
                                   .warn_unreceived_frames = warn_unreceived_ru_frames::after_traffic_detection,
                                   .are_metrics_enabled    = check_seq_id};
  }

  message_receiver_dependencies generate_dependencies()
  {
    message_receiver_dependencies dependencies;
    dependencies.logger         = &ocudulog::fetch_basic_logger("TEST");
    dependencies.window_checker = &window_checker;
    dependencies.window_handler = &closed_window_handler;

    {
      auto temp                    = std::make_unique<data_flow_uplane_uplink_prach_spy>();
      df_prach                     = temp.get();
      dependencies.data_flow_prach = std::move(temp);
    }
    {
      auto temp                     = std::make_unique<data_flow_uplane_uplink_data_spy>();
      df_uplink                     = temp.get();
      dependencies.data_flow_uplink = std::move(temp);
    }
    {
      auto temp                      = std::make_unique<vlan_frame_decoder_spy>(vlan_params);
      vlan_decoder                   = temp.get();
      dependencies.eth_frame_decoder = std::move(temp);
    }
    {
      ecpri_params = {.header      = {.revision       = 1U,
                                      .is_last_packet = true,
                                      .msg_type       = ecpri::message_type::iq_data,
                                      .payload_size   = units::bytes(1)},
                      .type_params = ecpri::iq_data_parameters{.pc_id = 1, .seq_id = 2}};

      auto temp                  = std::make_unique<ecpri_packet_decoder_spy>(ecpri_params);
      ecpri_decoder              = temp.get();
      dependencies.ecpri_decoder = std::move(temp);
    }
    dependencies.eth_receiver = std::make_unique<dummy_eth_receiver>();
    dependencies.seq_id_checker =
        check_seq_id ? std::unique_ptr<sequence_id_checker>(std::make_unique<sequence_id_checker_impl>())
                     : std::unique_ptr<sequence_id_checker>(std::make_unique<sequence_id_checker_dummy_impl>());

    return dependencies;
  }
};

TEST_F(ofh_message_receiver_fixture, discard_frames_with_unexpected_ethernet_type)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1}));

  ether::vlan_frame_params params = vlan_params;
  params.eth_type                 = 7777;
  vlan_decoder->set_vlan_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_FALSE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, discard_frames_with_unexpected_src_mac)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1}));

  ether::vlan_frame_params params = vlan_params;
  params.mac_src_address          = {0xbe, 0xbe, 0xca, 0xfe, 0xba, 0xca};
  vlan_decoder->set_vlan_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_FALSE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, discard_frames_with_unexpected_dst_mac)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1}));

  ether::vlan_frame_params params = vlan_params;
  params.mac_dst_address          = {0xbe, 0xbe, 0xca, 0xfe, 0xba, 0xca};
  vlan_decoder->set_vlan_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_FALSE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, discard_frames_with_unexpected_vlan)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1}));

  ether::vlan_frame_params params = vlan_params;
  params.vlan_config              = ether::vlan_parameters{.tci_vid = 5};
  vlan_decoder->set_vlan_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_FALSE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, accept_frames_with_different_vlan_priority)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1, 0, 0, 0}));

  ether::vlan_frame_params params = vlan_params;
  params.vlan_config              = ether::vlan_parameters{.tci_vid = 4, .tci_pcp = 1};
  vlan_decoder->set_vlan_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_TRUE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, accept_untagged_frame_when_vlan_is_configured)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1, 0, 0, 0}));

  ether::vlan_frame_params params = vlan_params;
  params.vlan_config.reset();
  vlan_decoder->set_vlan_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_TRUE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, discard_ecpri_control_frames)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1}));

  ecpri::packet_parameters params;
  params.header.msg_type = ocudu::ecpri::message_type::rt_control_data;
  params.type_params.emplace<ecpri::realtime_control_parameters>(ecpri::realtime_control_parameters{1, 2});
  ecpri_decoder->set_ecpri_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, discard_frames_with_unexpected_uplink_eacx)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1}));

  ecpri::packet_parameters params                               = ecpri_params;
  std::get<ecpri::iq_data_parameters>(params.type_params).pc_id = 2;
  ecpri_decoder->set_ecpri_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, discard_frames_with_unexpected_prach_eacx)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1}));

  ecpri::packet_parameters params                               = ecpri_params;
  std::get<ecpri::iq_data_parameters>(params.type_params).pc_id = 6;
  ecpri_decoder->set_ecpri_params(params);

  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, invalid_slot_point_peek_does_not_call_data_flows)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1, 0, 0}));
  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, valid_uplink_message_gets_processed_by_data_flow)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{0, 0, 0, 0}));
  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_TRUE(df_uplink->has_decode_function_been_called());
  ASSERT_FALSE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, valid_long_prach_message_gets_processed_by_data_flow)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{1, 0, 0, 0}));
  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_TRUE(df_prach->has_decode_function_been_called());
}

TEST_F(ofh_message_receiver_fixture, valid_short_prach_message_gets_processed_by_data_flow)
{
  ether::unique_rx_buffer buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{3, 0, 0, 0}));
  ul_handler.on_new_frame(std::move(buffer));

  ASSERT_TRUE(vlan_decoder->has_decode_function_been_called());
  ASSERT_TRUE(ecpri_decoder->has_decode_function_been_called());
  ASSERT_FALSE(df_uplink->has_decode_function_been_called());
  ASSERT_TRUE(df_prach->has_decode_function_been_called());
}

/// Message receiver fixture that checks the eCPRI sequence identifier of the received messages.
class ofh_message_receiver_seq_id_fixture : public ofh_message_receiver_fixture
{
protected:
  ofh_message_receiver_seq_id_fixture() : ofh_message_receiver_fixture(true) {}

  /// Sends an uplink User-Plane message carrying the given eCPRI sequence identifier.
  void send_uplink_message(uint8_t seq_id)
  {
    ecpri::packet_parameters params                                = ecpri_params;
    std::get<ecpri::iq_data_parameters>(params.type_params).seq_id = static_cast<uint16_t>(seq_id) << 8;
    ecpri_decoder->set_ecpri_params(params);

    df_uplink->clear();
    ul_handler.on_new_frame(ether::unique_rx_buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{0, 0, 0, 0})));
  }

  /// Collects and resets the message receiver metrics.
  message_decoding_performance_metrics collect_metrics()
  {
    message_decoding_performance_metrics metrics   = {};
    message_receiver_metrics_collector*  collector = ul_handler.get_metrics_collector();
    EXPECT_NE(collector, nullptr) << "Metrics must be enabled in the message receiver";
    if (collector != nullptr) {
      collector->collect_metrics(metrics);
    }

    return metrics;
  }
};

TEST_F(ofh_message_receiver_seq_id_fixture, out_of_order_message_gets_processed_by_data_flow)
{
  send_uplink_message(0);
  ASSERT_TRUE(df_uplink->has_decode_function_been_called());

  // The message with sequence identifier 1 is delivered after the one with sequence identifier 2.
  send_uplink_message(2);
  ASSERT_TRUE(df_uplink->has_decode_function_been_called());

  send_uplink_message(1);
  ASSERT_TRUE(df_uplink->has_decode_function_been_called())
      << "A message with a sequence identifier from the past must be processed";
}

TEST_F(ofh_message_receiver_seq_id_fixture, duplicated_message_gets_processed_by_data_flow)
{
  send_uplink_message(0);
  ASSERT_TRUE(df_uplink->has_decode_function_been_called());

  send_uplink_message(0);
  ASSERT_TRUE(df_uplink->has_decode_function_been_called()) << "A duplicated message must still be processed";
}

TEST_F(ofh_message_receiver_seq_id_fixture, out_of_order_messages_registered_correctly_in_metrics)
{
  send_uplink_message(0);
  send_uplink_message(2);
  send_uplink_message(1);

  message_decoding_performance_metrics metrics = collect_metrics();
  ASSERT_EQ(1, metrics.ecpri_metrics.nof_future_seq_id_messages);
  ASSERT_EQ(1, metrics.ecpri_metrics.nof_past_seq_id_messages);
}

TEST_F(ofh_message_receiver_seq_id_fixture, lost_messages_registered_correctly_in_metrics)
{
  send_uplink_message(0);
  send_uplink_message(2);
  send_uplink_message(4);

  message_decoding_performance_metrics metrics = collect_metrics();
  ASSERT_EQ(2, metrics.ecpri_metrics.nof_future_seq_id_messages);
  ASSERT_EQ(0, metrics.ecpri_metrics.nof_past_seq_id_messages);
}

/// Message receiver fixture that checks the eCPRI sequence identifier of the received messages.
class ofh_message_receiver_ecpri_corrupted_msg_fixture : public ofh_message_receiver_fixture
{
protected:
  ofh_message_receiver_ecpri_corrupted_msg_fixture() : ofh_message_receiver_fixture(true) {}

  /// Sends an uplink User-Plane message carrying the given eCPRI sequence identifier.
  void send_uplink_message(ecpri::packet_parameters params)
  {
    ecpri_decoder->set_ecpri_params(params);
    std::get<ecpri::iq_data_parameters>(params.type_params).pc_id = 8888;

    df_uplink->clear();
    ul_handler.on_new_frame(ether::unique_rx_buffer(dummy_eth_rx_buffer(std::vector<uint8_t>{0, 0, 0, 0})));
  }

  /// Collects and resets the message receiver metrics.
  message_decoding_performance_metrics collect_metrics()
  {
    message_decoding_performance_metrics metrics   = {};
    message_receiver_metrics_collector*  collector = ul_handler.get_metrics_collector();
    EXPECT_NE(collector, nullptr) << "Metrics must be enabled in the message receiver";
    if (collector != nullptr) {
      collector->collect_metrics(metrics);
    }

    return metrics;
  }
};

TEST_F(ofh_message_receiver_ecpri_corrupted_msg_fixture, incorrect_eaxc_increases_corrupted_messages)
{
  ecpri::packet_parameters params                               = ecpri_params;
  std::get<ecpri::iq_data_parameters>(params.type_params).pc_id = 8888;
  send_uplink_message(params);

  message_decoding_performance_metrics metrics = collect_metrics();
  ASSERT_EQ(1, metrics.ecpri_metrics.nof_corrupted_messages);
}

TEST_F(ofh_message_receiver_ecpri_corrupted_msg_fixture, incorrect_ecpri_type_increases_corrupted_messages)
{
  ecpri::packet_parameters params = ecpri_params;
  params.header.msg_type          = ecpri::message_type::rt_control_data;

  const unsigned nof_corrupted_packets = 3;
  for (unsigned i = 0; i != nof_corrupted_packets; ++i) {
    send_uplink_message(params);
  }

  message_decoding_performance_metrics metrics = collect_metrics();
  ASSERT_EQ(nof_corrupted_packets, metrics.ecpri_metrics.nof_corrupted_messages);
}
