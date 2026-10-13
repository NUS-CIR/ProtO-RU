// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_difi_stream_config.h"
#include "radio_factory_difi_impl.h"
#include "ocudu/radio/capabilities/radio_tx_gain_control.h"
#include "ocudu/radio/radio_constants.h"
#include "ocudu/radio/radio_factory.h"
#include "ocudu/support/executors/task_worker.h"
#include "fmt/ostream.h"
#include "gtest/gtest.h"

using namespace ocudu;

namespace fmt {

template <>
struct formatter<ocudu::radio_configuration::radio> {
  template <typename ParseContext>
  auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ocudu::radio_configuration::radio& config, FormatContext& ctx) const
  {
    return fmt::format_to(
        ctx.out(), "sampling_rate={} otw={}", config.sampling_rate_Hz, static_cast<int>(config.otw_format));
  }
};

} // namespace fmt

namespace {

/// Base channel — DIFI does not use channel.args for connection params.
const radio_configuration::lo_frequency base_lo_frequency = {3.5e9, 0.0};
const radio_configuration::channel      base_channel      = {base_lo_frequency, 20.0, ""};

/// Base streams — connection params in stream.args as "ip=<addr>,port=<port>".
const radio_configuration::stream base_tx_stream = {{base_channel}, "ip=127.0.0.1,port=4991"};
const radio_configuration::stream base_rx_stream = {{base_channel}, "ip=0.0.0.0,port=4992"};

const radio_configuration::radio radio_base_config = {
    {radio_configuration::clock_sources::source::DEFAULT, radio_configuration::clock_sources::source::DEFAULT},
    {base_tx_stream},
    {base_rx_stream},
    1.92e6,
    radio_configuration::over_the_wire_format::DEFAULT,
    radio_configuration::transmission_mode::continuous,
    0.0F,
    "",
    ocudulog::basic_levels::none};

struct test_case_t {
  std::function<radio_configuration::radio()> get_config;
  std::string                                 message;
};

std::ostream& operator<<(std::ostream& os, const test_case_t& test_case)
{
  fmt::print(os, "{}", test_case.get_config());
  return os;
}

const std::vector<test_case_t> radio_difi_validator_test_data = {
    // Valid base configuration — must pass.
    {[] {
       radio_configuration::radio config = radio_base_config;
       return config;
     },
     ""},
    // Valid: explicit SC16 OTW format.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.otw_format                 = radio_configuration::over_the_wire_format::SC16;
       return config;
     },
     ""},
    // Valid: SC8 OTW format.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.otw_format                 = radio_configuration::over_the_wire_format::SC8;
       return config;
     },
     ""},
    // Valid: empty stream args fall back to defaults.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().args    = "";
       config.rx_streams.front().args    = "";
       return config;
     },
     ""},
    // Valid: non-default port values.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().args    = "ip=192.168.1.10,port=50001";
       config.rx_streams.front().args    = "ip=0.0.0.0,port=50002";
       return config;
     },
     ""},
    // Tx stream count != Rx stream count.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.clear();
       return config;
     },
     "Transmit and receive number of streams must be equal.\n"},
    // Both stream lists empty.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.clear();
       config.rx_streams.clear();
       return config;
     },
     "At least one transmit and one receive stream must be configured.\n"},
    // Zero sampling rate.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.sampling_rate_Hz           = 0.0;
       return config;
     },
     "The sampling rate must be non-zero, NAN nor infinite.\n"},
    // Negative sampling rate.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.sampling_rate_Hz           = -1.0;
       return config;
     },
     "The sampling rate must be greater than zero.\n"},
    // Unsupported OTW format (SC12).
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.otw_format                 = radio_configuration::over_the_wire_format::SC12;
       return config;
     },
     "Only DEFAULT, SC8 and SC16 OTW formats are supported by the DIFI radio.\n"},
    // Malformed Tx stream args (no '=' separator).
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().args    = "malformed";
       return config;
     },
     "Stream argument 'malformed' is malformed, expected key=value.\n"},
    // Unknown key in Tx stream args.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().args    = "bad_key=value";
       return config;
     },
     "Unknown stream argument key 'bad_key'.\n"},
    // Invalid IP address in Tx stream args.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().args    = "ip=not.valid.ip";
       return config;
     },
     "Invalid IP address 'not.valid.ip' in stream arguments.\n"},
    // Port zero in Tx stream args.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().args    = "port=0";
       return config;
     },
     "Stream port 0 is out of valid range [1, 65535].\n"},
    // Port out of range (> 65535) in Tx stream args.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().args    = "port=70000";
       return config;
     },
     "Stream port 70000 is out of valid range [1, 65535].\n"},
    // Non-integer port in Tx stream args.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().args    = "port=abc";
       return config;
     },
     "Stream port 'abc' is not a valid integer.\n"},
    // Malformed Rx stream args.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.rx_streams.front().args    = "malformed";
       return config;
     },
     "Stream argument 'malformed' is malformed, expected key=value.\n"},
    // Invalid IP address in Rx stream args.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.rx_streams.front().args    = "ip=999.999.999.999";
       return config;
     },
     "Invalid IP address '999.999.999.999' in stream arguments.\n"},
    // Port out of range in Rx stream args.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.rx_streams.front().args    = "port=0";
       return config;
     },
     "Stream port 0 is out of valid range [1, 65535].\n"},
    // Little-endian IQ payload.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.args                       = "iq_byte_order=little";
       return config;
     },
     ""},
    // Big-endian IQ payload among the other device arguments, with the whitespace of a folded configuration line.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.args                       = "tx_addr=127.0.0.1,tx_port=4991, iq_byte_order = big ,rx_port=4992";
       return config;
     },
     ""},
    // Two transmit channels: the first takes the stream port, the second names its own.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().channels.push_back({base_lo_frequency, 20.0, "port=4993"});
       return config;
     },
     ""},
    // Every channel may name its own port, the first one included.
    {[] {
       radio_configuration::radio config               = radio_base_config;
       config.tx_streams.front().channels.front().args = "port=5001";
       config.tx_streams.front().channels.push_back({base_lo_frequency, 20.0, "port=5003,"});
       return config;
     },
     ""},
    // A transmit stream without channels.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().channels.clear();
       return config;
     },
     "Every DIFI transmit stream needs at least one channel.\n"},
    // A second transmit channel without a port.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().channels.push_back(base_channel);
       return config;
     },
     "Channel 1 of DIFI stream 0 has no port, expected port=<number> in its arguments.\n"},
    // Two transmit channels on one port.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().channels.push_back({base_lo_frequency, 20.0, "port=4991"});
       return config;
     },
     "DIFI transmit port 4991 is used by more than one channel.\n"},
    // Two streams on one port.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.push_back(base_tx_stream);
       config.rx_streams.push_back({{base_channel}, "ip=0.0.0.0,port=4994"});
       return config;
     },
     "DIFI transmit port 4991 is used by more than one channel.\n"},
    // A channel takes a port only; the address is shared by the whole radio.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().channels.push_back({base_lo_frequency, 20.0, "ip=127.0.0.2,port=4993"});
       return config;
     },
     "Unknown channel argument key 'ip'.\n"},
    // A malformed channel argument.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().channels.push_back({base_lo_frequency, 20.0, "4993"});
       return config;
     },
     "Channel argument '4993' is malformed, expected key=value.\n"},
    // A larger transmit packet size.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.args                       = "tx_port=4991,pkt_samples=7680";
       return config;
     },
     ""},
    // The largest transmit packet that fits a UDP datagram.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.args                       = "pkt_samples=16369";
       return config;
     },
     ""},
    // Two receive channels, each on its own port.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.rx_streams.front().channels.push_back({base_lo_frequency, 20.0, "port=4994"});
       return config;
     },
     ""},
    // A receive stream without channels.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.rx_streams.front().channels.clear();
       return config;
     },
     "Every DIFI receive stream needs at least one channel.\n"},
    // A second receive channel without a port.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.rx_streams.front().channels.push_back(base_channel);
       return config;
     },
     "Channel 1 of DIFI stream 0 has no port, expected port=<number> in its arguments.\n"},
    // Two receive channels on one port.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.rx_streams.front().channels.push_back({base_lo_frequency, 20.0, "port=4992"});
       return config;
     },
     "DIFI receive port 4992 is used by more than one channel.\n"},
    // A channel port out of range.
    {[] {
       radio_configuration::radio config = radio_base_config;
       config.tx_streams.front().channels.push_back({base_lo_frequency, 20.0, "port=0"});
       return config;
     },
     "Stream port 0 is out of valid range [1, 65535].\n"},
};

class RadioDifiValidatorFixture : public ::testing::TestWithParam<test_case_t>
{
protected:
  static std::unique_ptr<radio_factory> factory;

  static void SetUpTestSuite()
  {
    if (factory) {
      return;
    }
    factory = std::make_unique<radio_factory_difi_impl>();
    ASSERT_NE(factory, nullptr);
  }
};

class radio_notifier_spy : public radio_event_notifier
{
public:
  void on_radio_rt_event(const event_description& description) override {}
};

std::unique_ptr<radio_factory> RadioDifiValidatorFixture::factory = nullptr;

TEST_P(RadioDifiValidatorFixture, RadioDifiValidatorTest)
{
  ASSERT_NE(factory, nullptr);

  const test_case_t& param = GetParam();

  radio_configuration::radio config = param.get_config();

  // Redirect stdout to buffer.
  ::testing::internal::CaptureStdout();

  bool        is_valid = factory->get_configuration_validator().is_configuration_valid(config);
  std::string output   = ::testing::internal::GetCapturedStdout();

  // An empty expected message means the config is valid.
  ASSERT_EQ(param.message.empty(), is_valid);

  // The printed output must match the expected error message exactly.
  ASSERT_EQ(output, param.message);

  // For valid configs, also verify that session creation succeeds.
  if (param.message.empty()) {
    task_worker                    async_task_worker("async_thread", 2 * RADIO_MAX_NOF_PORTS);
    std::unique_ptr<task_executor> async_task_executor = make_task_executor_ptr(async_task_worker);
    radio_notifier_spy             notifier;

    std::unique_ptr<radio_session> radio = factory->create(config, *async_task_executor, notifier);
    ASSERT_NE(radio, nullptr);
  }
}

// Transmit gain takes the flat port index of radio_tx_gain_control, numbered by stream and then by channel.
TEST(RadioDifiSessionTest, TxGainUsesFlatPortIndex)
{
  // ctest runs the DIFI test binaries in parallel, so this test keeps to its own ports, 15050 upwards.
  static constexpr unsigned BASE_PORT    = 15050;
  static constexpr unsigned NOF_STREAMS  = 2;
  static constexpr unsigned NOF_CHANNELS = 2;

  radio_configuration::radio config = radio_base_config;
  config.tx_streams.clear();
  config.rx_streams.clear();
  unsigned port = BASE_PORT;
  for (unsigned stream_id = 0; stream_id != NOF_STREAMS; ++stream_id) {
    radio_configuration::stream& tx_stream = config.tx_streams.emplace_back();
    tx_stream.args                         = "ip=127.0.0.1";
    for (unsigned channel_id = 0; channel_id != NOF_CHANNELS; ++channel_id) {
      tx_stream.channels.push_back({base_lo_frequency, 20.0, fmt::format("port={}", port++)});
    }
    config.rx_streams.push_back({{base_channel}, fmt::format("ip=127.0.0.1,port={}", port++)});
  }

  task_worker                    async_task_worker("async_thread", 2 * RADIO_MAX_NOF_PORTS);
  std::unique_ptr<task_executor> async_task_executor = make_task_executor_ptr(async_task_worker);
  radio_notifier_spy             notifier;
  radio_factory_difi_impl        factory;

  ASSERT_TRUE(factory.get_configuration_validator().is_configuration_valid(config));
  std::unique_ptr<radio_session> radio = factory.create(config, *async_task_executor, notifier);
  ASSERT_NE(radio, nullptr);
  radio_tx_gain_control* tx_gain = radio->get_capabilities_registry().get<radio_tx_gain_control>();
  ASSERT_NE(tx_gain, nullptr);

  // Port 2 is the first channel of stream 1, which an index by stream would reject.
  for (unsigned port_id = 0; port_id != NOF_STREAMS * NOF_CHANNELS; ++port_id) {
    EXPECT_TRUE(tx_gain->set_tx_gain(port_id, 10.0)) << "Port " << port_id;
  }
  // Past the last port.
  EXPECT_FALSE(tx_gain->set_tx_gain(NOF_STREAMS * NOF_CHANNELS, 10.0));
}

// The packet size takes any integer from one sample to the largest packet that fits a UDP datagram, and defaults to
// the size that divides every slot.
TEST(RadioDifiDeviceArgsTest, PacketSize)
{
  EXPECT_EQ(parse_difi_pkt_samples(""), DIFI_DEFAULT_PKT_SAMPLES);
  EXPECT_EQ(parse_difi_pkt_samples("tx_port=4991, pkt_samples = 7680 "), 7680U);
  EXPECT_EQ(parse_difi_pkt_samples("pkt_samples=1"), 1U);
  EXPECT_EQ(parse_difi_pkt_samples(fmt::format("pkt_samples={}", DIFI_MAX_PKT_SAMPLES)), DIFI_MAX_PKT_SAMPLES);
  EXPECT_FALSE(parse_difi_pkt_samples(fmt::format("pkt_samples={}", DIFI_MAX_PKT_SAMPLES + 1)).has_value());
  EXPECT_FALSE(parse_difi_pkt_samples("pkt_samples=0").has_value());
  EXPECT_FALSE(parse_difi_pkt_samples("pkt_samples=lots").has_value());
  EXPECT_FALSE(parse_difi_pkt_samples("pkt_samples=-1").has_value());
}

INSTANTIATE_TEST_SUITE_P(RadioDifiValidatorTest,
                         RadioDifiValidatorFixture,
                         ::testing::ValuesIn(radio_difi_validator_test_data));

} // namespace
