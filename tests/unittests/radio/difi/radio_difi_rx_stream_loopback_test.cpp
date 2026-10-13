// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief Loopback integration test for radio_difi_rx_stream: a tx stream sends DIFI packets to a
/// loopback address and the rx stream unpacks them, exercising the whole receive path.

#include "radio_difi_context_packet.h"
#include "radio_difi_data_packet.h"
#include "radio_difi_rx_stream.h"
#include "radio_difi_tx_stream.h"
#include "radio_difi_udp_socket.h"
#include "ocudu/gateways/baseband/buffer/baseband_gateway_buffer_reader.h"
#include "ocudu/gateways/baseband/buffer/baseband_gateway_buffer_writer.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/radio/radio_event_notifier.h"
#include <array>
#include <chrono>
#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <thread>
#include <utility>
#include <vector>

using namespace ocudu;

/// Logger for sockets built directly by these tests.
static ocudulog::basic_logger& test_socket_logger()
{
  return ocudulog::fetch_basic_logger("difi:test", false);
}

// ---- Stubs ------------------------------------------------------------------

class null_notifier_rx : public radio_event_notifier
{
public:
  void on_radio_rt_event(const event_description&) override {}
};

/// Simple buffer reader backed by a fixed sample vector.
class simple_buffer_reader_rx : public baseband_gateway_buffer_reader
{
public:
  explicit simple_buffer_reader_rx(std::vector<ci16_t> s) : samples(std::move(s)) {}
  unsigned           get_nof_channels() const override { return 1; }
  unsigned           get_nof_samples() const override { return static_cast<unsigned>(samples.size()); }
  span<const ci16_t> get_channel_buffer(unsigned) const override { return samples; }

private:
  std::vector<ci16_t> samples;
};

/// Simple buffer writer backed by a fixed-size vector.
class simple_buffer_writer : public baseband_gateway_buffer_writer
{
public:
  explicit simple_buffer_writer(unsigned nof_samples) : samples(nof_samples, ci16_t(0, 0)) {}
  unsigned     get_nof_channels() const override { return 1; }
  unsigned     get_nof_samples() const override { return static_cast<unsigned>(samples.size()); }
  span<ci16_t> get_channel_buffer(unsigned) override { return samples; }

  const std::vector<ci16_t>& data() const { return samples; }

private:
  std::vector<ci16_t> samples;
};

// ---- Test fixture -----------------------------------------------------------

// ctest runs the DIFI test binaries in parallel, so no two of them may share a UDP port. This binary uses 15001 to
// 15005, 15010 to 15017 and 15030. The transmit stream test uses 14991, 15006 to 15009 and 15040 to 15042. The socket
// test uses 19981 upwards, and the validator test 15050 upwards.

/// Pairs a tx stream (sending to 127.0.0.1:15001) with an rx stream (bound on
/// 127.0.0.1:15001).  Port 15001 is chosen to avoid conflicts.
class RxStreamLoopback : public ::testing::Test
{
protected:
  static constexpr uint16_t TEST_PORT   = 15001;
  static constexpr uint32_t STREAM_ID   = 0x00000002U;
  static constexpr double   SAMPLE_RATE = 1920000.0;

  void SetUp() override
  {
    radio_difi_rx_stream::stream_description rx_desc;
    rx_desc.ip             = "127.0.0.1";
    rx_desc.port           = TEST_PORT;
    rx_desc.stream_id      = STREAM_ID;
    rx_desc.bit_depth      = 16;
    rx_desc.sample_rate_Hz = SAMPLE_RATE;
    rx_desc.stream_id_str  = "test:rx:0";
    rx                     = std::make_unique<radio_difi_rx_stream>(rx_desc, notifier);
    ASSERT_TRUE(rx->is_successful());

    radio_difi_tx_stream::stream_description tx_desc;
    tx_desc.ip             = "127.0.0.1";
    tx_desc.port           = TEST_PORT;
    tx_desc.stream_id      = STREAM_ID;
    tx_desc.bit_depth      = 16;
    tx_desc.sample_rate_Hz = SAMPLE_RATE;
    tx_desc.center_freq_Hz = 0.0;
    tx_desc.stream_id_str  = "test:tx:0";
    tx                     = std::make_unique<radio_difi_tx_stream>(tx_desc, notifier);
    ASSERT_TRUE(tx->is_successful());

    // Start the transmitter first, so the context packet it sends goes out before the receive
    // socket is bound and is dropped by the kernel. Draining it through receive() instead would
    // advance the receiver timeline by a buffer and offset every expectation below.
    tx->start(0);
    rx->start(0);
  }

  void TearDown() override
  {
    if (tx)
      tx->stop();
    if (rx)
      rx->stop();
  }

  void transmit(const std::vector<ci16_t>& samples, baseband_gateway_timestamp ts = 0)
  {
    simple_buffer_reader_rx               buf(samples);
    baseband_gateway_transmitter_metadata meta{};
    meta.is_empty = false;
    meta.ts       = ts;
    tx->transmit(buf, meta);
  }

  null_notifier_rx                      notifier;
  std::unique_ptr<radio_difi_rx_stream> rx;
  std::unique_ptr<radio_difi_tx_stream> tx;
};

// ---- Tests ------------------------------------------------------------------

TEST_F(RxStreamLoopback, ReceiveTimeout)
{
  // No packet sent — receive() should time out and return current sample_count.
  simple_buffer_writer buf(4);
  const auto           meta = rx->receive(buf);
  // Timestamp should be the initial value (0) since no packet arrived.
  EXPECT_EQ(meta.ts, 0U);
}

TEST_F(RxStreamLoopback, Receive16BitSamples)
{
  const std::vector<ci16_t> sent = {ci16_t(0x1000, 0x2000), ci16_t(0x3000, static_cast<int16_t>(0x4000))};
  transmit(sent);

  simple_buffer_writer buf(sent.size());
  const auto           meta = rx->receive(buf);

  ASSERT_EQ(meta.ts, 0U); // ts=0, sample_rate doesn't matter for zero timestamp
  EXPECT_EQ(buf.data()[0], sent[0]);
  EXPECT_EQ(buf.data()[1], sent[1]);
}

TEST_F(RxStreamLoopback, ReceiveSampleCountAdvances)
{
  const std::vector<ci16_t> sent(8, ci16_t(1, 2));
  transmit(sent);

  simple_buffer_writer buf(sent.size());
  rx->receive(buf);

  EXPECT_EQ(rx->get_sample_count(), static_cast<uint64_t>(sent.size()));
}

TEST_F(RxStreamLoopback, FirstPacketLatchesTransmitterEpoch)
{
  // The transmitter stamps its first packet a full second in, while the receiver timeline starts
  // at zero. The epoch latch must absorb that difference rather than read it as a huge gap.
  const auto                ts_ticks = static_cast<baseband_gateway_timestamp>(SAMPLE_RATE);
  const std::vector<ci16_t> sent     = {ci16_t(10, 20)};
  transmit(sent, ts_ticks);

  simple_buffer_writer buf(sent.size());
  const auto           meta = rx->receive(buf);

  // The reported timestamp is the local position, not the transmitter's.
  EXPECT_EQ(meta.ts, 0U);
  EXPECT_EQ(buf.data()[0], sent[0]) << "First packet must land at the start of the buffer";
}

TEST_F(RxStreamLoopback, ReceiveWrongStreamIdDiscarded)
{
  // Send a well-formed data packet carrying a foreign stream ID through a raw socket, so no
  // context packet is involved and the stream-ID filter is tested in isolation.
  const std::vector<ci16_t> payload = {ci16_t(99, 88)};

  difi_data_packet_params p{};
  p.stream_id = STREAM_ID + 1;
  p.full_secs = 0;
  p.frac_ps   = 0;
  p.bit_depth = 16;
  p.pkt_n     = 0;

  std::vector<uint8_t> raw(difi_data_packet_size(16, static_cast<unsigned>(payload.size())).value());
  build_difi_data_packet(raw, p, payload);

  radio_difi_udp_socket tx_sock(test_socket_logger());
  ASSERT_TRUE(tx_sock.open_tx("127.0.0.1", TEST_PORT));
  ASSERT_TRUE(tx_sock.send(raw));
  tx_sock.close();

  // The packet is filtered out, so the buffer is zero-filled instead of carrying its payload.
  simple_buffer_writer out(1);
  rx->receive(out);
  EXPECT_EQ(out.data()[0], ci16_t(0, 0)) << "Payload of a foreign stream must never be written";
}

TEST_F(RxStreamLoopback, ContextPacketIsSkipped)
{
  // Build and send a raw context packet directly via a UDP socket so we can
  // test the type-filter explicitly without relying on SetUp() side effects.
  difi_context_packet_params ctx_params{};
  ctx_params.stream_id      = STREAM_ID;
  ctx_params.full_secs      = 0;
  ctx_params.frac_ps        = 0;
  ctx_params.sample_rate_Hz = SAMPLE_RATE;
  ctx_params.center_freq_Hz = 0.0;
  ctx_params.bit_depth      = 16;
  ctx_params.ctx_pkt_n      = 1;

  std::array<uint8_t, DIFI_CONTEXT_PACKET_SIZE.value()> ctx_buf;
  build_difi_context_packet(ctx_buf, ctx_params);

  radio_difi_udp_socket tx_sock(test_socket_logger());
  ASSERT_TRUE(tx_sock.open_tx("127.0.0.1", TEST_PORT));
  ASSERT_TRUE(tx_sock.send(ctx_buf));
  tx_sock.close();

  // receive() sees the context packet, discards it on the type check, then finds no data and
  // zero-fills. The reported timestamp is still the buffer start, and the timeline advances by
  // the full buffer as it does for any other call.
  simple_buffer_writer out(4);
  const auto           meta = rx->receive(out);

  EXPECT_EQ(meta.ts, 0U);
  EXPECT_EQ(out.data()[0], ci16_t(0, 0)) << "A context packet must contribute no samples";
  EXPECT_EQ(rx->get_sample_count(), 4U);
}

TEST_F(RxStreamLoopback, UnderrunIsZeroFilled)
{
  // The packet carries 2 samples of the 8 requested. The remainder must be zero-filled and the timeline
  // must still advance by the whole buffer, since the caller derives the next timestamp from its size.
  const std::vector<ci16_t> sent = {ci16_t(0x0a00, 0x0b00), ci16_t(0x0c00, 0x0d00)};
  transmit(sent);

  simple_buffer_writer buf(8);
  const auto           meta = rx->receive(buf);

  EXPECT_EQ(meta.ts, 0U);
  EXPECT_EQ(buf.data()[0], sent[0]);
  EXPECT_EQ(buf.data()[1], sent[1]);
  for (unsigned i = 2; i != 8; ++i) {
    EXPECT_EQ(buf.data()[i], ci16_t(0, 0)) << "Sample " << i << " should be zero-filled";
  }
  EXPECT_EQ(rx->get_sample_count(), 8U) << "Timeline must advance by the full buffer";
}

TEST(RxStream8Bit, Receive8BitSamples)
{
  // Use a dedicated port to avoid conflicts with the 16-bit fixture.
  static constexpr uint16_t PORT_8BIT = 15002;
  static constexpr uint32_t STREAM_8  = 0x00000003U;
  static constexpr double   SRATE     = 1920000.0;

  null_notifier_rx notifier;

  radio_difi_rx_stream::stream_description rx_desc;
  rx_desc.ip             = "127.0.0.1";
  rx_desc.port           = PORT_8BIT;
  rx_desc.stream_id      = STREAM_8;
  rx_desc.bit_depth      = 8;
  rx_desc.sample_rate_Hz = SRATE;
  rx_desc.stream_id_str  = "test:rx:8bit";
  radio_difi_rx_stream rx_stream(rx_desc, notifier);
  rx_stream.start(0);

  radio_difi_tx_stream::stream_description tx_desc;
  tx_desc.ip             = "127.0.0.1";
  tx_desc.port           = PORT_8BIT;
  tx_desc.stream_id      = STREAM_8;
  tx_desc.bit_depth      = 8;
  tx_desc.sample_rate_Hz = SRATE;
  tx_desc.center_freq_Hz = 0.0;
  tx_desc.stream_id_str  = "test:tx:8bit";
  radio_difi_tx_stream tx_stream(tx_desc, notifier);
  tx_stream.start(0);

  // Drain context packet.
  simple_buffer_writer drain(1);
  rx_stream.receive(drain);

  // 8-bit samples are stored as int16 with the 8-bit value in the upper byte.
  // e.g. int8 value 0x12 → int16 0x1200.
  const std::vector<ci16_t> sent = {
      ci16_t(0x1200, static_cast<int16_t>(0xab00)),
      ci16_t(0x3400, static_cast<int16_t>(0xcd00)),
  };

  simple_buffer_reader_rx               buf(sent);
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = false;
  // Stamped at the receiver's current position, which the drain above advanced: timestamps are absolute,
  // so a packet stamped earlier is late and its leading samples discarded. This test covers unpacking.
  meta.ts = rx_stream.get_sample_count();
  tx_stream.transmit(buf, meta);

  simple_buffer_writer out(sent.size());
  rx_stream.receive(out);

  EXPECT_EQ(out.data()[0], sent[0]);
  EXPECT_EQ(out.data()[1], sent[1]);

  tx_stream.stop();
  rx_stream.stop();
}

// ---- Event notification tests -----------------------------------------------

/// Notifier that records every event it receives.
class capturing_notifier_rx : public radio_event_notifier
{
public:
  void on_radio_rt_event(const event_description& ev) override { events.push_back(ev); }

  std::vector<event_description> events;
};

/// Overflow: when a received packet's timestamp is ahead of sample_count, an
/// OVERFLOW event must be fired before the samples are written.
TEST(RxStreamEvents, OverflowFiredOnTimestampGap)
{
  static constexpr uint16_t PORT      = 15005;
  static constexpr uint32_t STREAM    = 0x00000007U;
  static constexpr double   SRATE     = 1920000.0;
  static constexpr unsigned N_SAMPLES = 4;

  capturing_notifier_rx notifier;

  radio_difi_rx_stream::stream_description rx_desc;
  rx_desc.ip             = "127.0.0.1";
  rx_desc.port           = PORT;
  rx_desc.stream_id      = STREAM;
  rx_desc.bit_depth      = 16;
  rx_desc.sample_rate_Hz = SRATE;
  rx_desc.stream_id_str  = "test:events:rx";
  radio_difi_rx_stream rx_stream(rx_desc, notifier);
  rx_stream.start(0);

  null_notifier_rx                         tx_notifier;
  radio_difi_tx_stream::stream_description tx_desc;
  tx_desc.ip             = "127.0.0.1";
  tx_desc.port           = PORT;
  tx_desc.stream_id      = STREAM;
  tx_desc.bit_depth      = 16;
  tx_desc.sample_rate_Hz = SRATE;
  tx_desc.center_freq_Hz = 0.0;
  tx_desc.stream_id_str  = "test:events:tx";
  radio_difi_tx_stream tx_stream(tx_desc, tx_notifier);
  tx_stream.start(0);

  // Drain the context packet (type filter discards it — no event expected).
  simple_buffer_writer drain(1);
  rx_stream.receive(drain);
  ASSERT_TRUE(notifier.events.empty());

  // First packet: ts=0, N_SAMPLES samples → sample_count becomes N_SAMPLES.
  // ts (0) == sample_count (0) → no overflow.
  const std::vector<ci16_t>             s1(N_SAMPLES, ci16_t(1, 2));
  simple_buffer_reader_rx               buf1(s1);
  baseband_gateway_transmitter_metadata meta1{};
  meta1.is_empty = false;
  meta1.ts       = 0;
  tx_stream.transmit(buf1, meta1);

  simple_buffer_writer out1(N_SAMPLES);
  rx_stream.receive(out1);
  EXPECT_TRUE(notifier.events.empty()) << "No overflow expected for contiguous first packet";

  // Second packet: ts = N_SAMPLES + 10 (gap of 10) → overflow must fire.
  const std::vector<ci16_t>             s2(N_SAMPLES, ci16_t(3, 4));
  simple_buffer_reader_rx               buf2(s2);
  baseband_gateway_transmitter_metadata meta2{};
  meta2.is_empty = false;
  meta2.ts       = static_cast<baseband_gateway_timestamp>(N_SAMPLES + 10);
  tx_stream.transmit(buf2, meta2);

  simple_buffer_writer out2(N_SAMPLES);
  rx_stream.receive(out2);

  ASSERT_EQ(notifier.events.size(), 1U) << "Expected exactly one OVERFLOW event";
  EXPECT_EQ(notifier.events[0].type, radio_event_type::OVERFLOW);
  EXPECT_EQ(notifier.events[0].source, radio_event_source::RECEIVE);
  EXPECT_EQ(notifier.events[0].stream_id, static_cast<unsigned>(STREAM));

  tx_stream.stop();
  rx_stream.stop();
}

// ---- Reassembly tests -------------------------------------------------------
//
// A buffer larger than one packet is fragmented, so several datagrams reassemble into one: six at 11.52 MHz.

class RxReassembly : public ::testing::Test
{
protected:
  static constexpr double   SRATE     = 11520000.0;
  static constexpr unsigned SLOT_LEN  = 11520;
  static constexpr uint32_t STREAM_ID = 0x0000000bU;

  /// \brief Longest a paced receive() call may take.
  ///
  /// Far above any buffer duration in these tests, so slowdown under valgrind on a loaded machine stays inside it,
  /// while a receiver that stalls does not.
  static constexpr std::chrono::microseconds MAX_RECEIVE_DURATION{500000};

  void open(uint16_t port, double srate_Hz = SRATE)
  {
    test_port      = port;
    sample_rate_Hz = srate_Hz;

    radio_difi_rx_stream::stream_description rx_desc;
    rx_desc.ip             = "127.0.0.1";
    rx_desc.port           = port;
    rx_desc.stream_id      = STREAM_ID;
    rx_desc.bit_depth      = 16;
    rx_desc.sample_rate_Hz = srate_Hz;
    rx_desc.stream_id_str  = "test:reasm:rx";
    rx                     = std::make_unique<radio_difi_rx_stream>(rx_desc, notifier);
    ASSERT_TRUE(rx->is_successful());

    radio_difi_tx_stream::stream_description tx_desc;
    tx_desc.ip             = "127.0.0.1";
    tx_desc.port           = port;
    tx_desc.stream_id      = STREAM_ID;
    tx_desc.bit_depth      = 16;
    tx_desc.sample_rate_Hz = srate_Hz;
    tx_desc.center_freq_Hz = 1.8425e9;
    tx_desc.stream_id_str  = "test:reasm:tx";
    tx                     = std::make_unique<radio_difi_tx_stream>(tx_desc, notifier);
    ASSERT_TRUE(tx->is_successful());

    // Transmitter first, so its context packet is dropped before the receiver binds.
    tx->start(0);
    rx->start(0);
  }

  void TearDown() override
  {
    if (tx) {
      tx->stop();
    }
    if (rx) {
      rx->stop();
    }
  }

  /// Transmits a ramp of \p nof_samples where sample k is (k, -k), starting at tick \p ts.
  void transmit_ramp(unsigned nof_samples, uint64_t ts)
  {
    std::vector<ci16_t> samples(nof_samples);
    for (unsigned k = 0; k != nof_samples; ++k) {
      samples[k] = ramp_sample(k);
    }
    simple_buffer_reader_rx               buf(samples);
    baseband_gateway_transmitter_metadata meta{};
    meta.is_empty = false;
    meta.ts       = ts;
    tx->transmit(buf, meta);
  }

  /// Sends a raw DIFI data packet of \p nof_samples copies of \p value stamped at tick \p ts,
  /// bypassing the transmitter so fragments can be dropped or duplicated deliberately.
  void send_raw_fragment(unsigned nof_samples, ci16_t value, uint64_t ts)
  {
    difi_data_packet_params p{};
    p.stream_id = STREAM_ID;
    p.bit_depth = 16;
    p.pkt_n     = 0;
    difi_ticks_to_time(ts, sample_rate_Hz, p.full_secs, p.frac_ps);

    const std::vector<ci16_t> samples(nof_samples, value);
    std::vector<uint8_t>      raw(difi_data_packet_size(16, nof_samples).value());
    build_difi_data_packet(raw, p, samples);

    radio_difi_udp_socket sock(test_socket_logger());
    ASSERT_TRUE(sock.open_tx("127.0.0.1", test_port));
    ASSERT_TRUE(sock.send(raw));
    sock.close();
  }

  static ci16_t ramp_sample(unsigned k)
  {
    return ci16_t(static_cast<int16_t>(k), static_cast<int16_t>(-static_cast<int>(k)));
  }

  capturing_notifier_rx                 notifier;
  uint16_t                              test_port      = 0;
  double                                sample_rate_Hz = SRATE;
  std::unique_ptr<radio_difi_rx_stream> rx;
  std::unique_ptr<radio_difi_tx_stream> tx;
};

TEST_F(RxReassembly, SlotIsReassembledFromSixPackets)
{
  ASSERT_NO_FATAL_FAILURE(open(15010));

  transmit_ramp(SLOT_LEN, 0);

  simple_buffer_writer buf(SLOT_LEN);
  const auto           meta = rx->receive(buf);

  EXPECT_EQ(meta.ts, 0U);
  EXPECT_EQ(rx->get_sample_count(), SLOT_LEN);

  // The whole ramp must reappear in order, proving the six fragments were joined without
  // duplication, omission or reordering.
  for (unsigned k = 0; k != SLOT_LEN; ++k) {
    ASSERT_EQ(buf.data()[k], ramp_sample(k)) << "Mismatch at sample " << k;
  }

  EXPECT_TRUE(notifier.events.empty()) << "Contiguous fragments must raise no event";
}

TEST_F(RxReassembly, LostDatagramIsZeroFilledAndReportedOnce)
{
  ASSERT_NO_FATAL_FAILURE(open(15011));

  constexpr unsigned FRAG = 64;

  // Fragments at ticks 0 and 2*FRAG arrive; the one at FRAG never does.
  send_raw_fragment(FRAG, ci16_t(0x0111, 0x0222), 0);
  send_raw_fragment(FRAG, ci16_t(0x0333, 0x0444), 2 * FRAG);

  simple_buffer_writer buf(3 * FRAG);
  const auto           meta = rx->receive(buf);

  EXPECT_EQ(meta.ts, 0U);

  EXPECT_EQ(buf.data()[0], ci16_t(0x0111, 0x0222));
  EXPECT_EQ(buf.data()[FRAG - 1], ci16_t(0x0111, 0x0222));
  EXPECT_EQ(buf.data()[FRAG], ci16_t(0, 0)) << "Missing fragment must be zero-filled";
  EXPECT_EQ(buf.data()[2 * FRAG - 1], ci16_t(0, 0));
  EXPECT_EQ(buf.data()[2 * FRAG], ci16_t(0x0333, 0x0444)) << "Samples after a gap must keep their correct position";

  ASSERT_EQ(notifier.events.size(), 1U) << "A buffer must report at most one OVERFLOW";
  EXPECT_EQ(notifier.events[0].type, radio_event_type::OVERFLOW);
  EXPECT_EQ(notifier.events[0].source, radio_event_source::RECEIVE);
  EXPECT_EQ(notifier.events[0].timestamp, static_cast<uint64_t>(FRAG)) << "Event marks where the gap began";
}

TEST_F(RxReassembly, StaleDatagramIsDropped)
{
  ASSERT_NO_FATAL_FAILURE(open(15012));

  constexpr unsigned FRAG = 32;

  // The first packet latches the epoch and fills the first half.
  send_raw_fragment(FRAG, ci16_t(0x0555, 0x0666), 0);
  // A duplicate of that same span must neither overwrite nor displace the fill point.
  send_raw_fragment(FRAG, ci16_t(0x0777, 0x0888), 0);
  // The genuine continuation.
  send_raw_fragment(FRAG, ci16_t(0x0999, 0x0aaa), FRAG);

  simple_buffer_writer buf(2 * FRAG);
  rx->receive(buf);

  EXPECT_EQ(buf.data()[0], ci16_t(0x0555, 0x0666)) << "A duplicate must not overwrite";
  EXPECT_EQ(buf.data()[FRAG], ci16_t(0x0999, 0x0aaa)) << "Continuation must follow the first packet";
  EXPECT_TRUE(notifier.events.empty()) << "A duplicate is not a discontinuity";
}

// A transmitter's packet size need not divide the receive buffer, in which case a packet straddles
// the buffer boundary. Its tail must be carried into the next call: dropping it would make the
// following packet look late, so every boundary would lose samples and raise a spurious overflow.
TEST_F(RxReassembly, StraddlingPacketTailIsCarriedOver)
{
  ASSERT_NO_FATAL_FAILURE(open(15015));

  constexpr unsigned BUF  = 100;
  constexpr unsigned FRAG = 64;
  // 64 does not divide 100, so packet B spans the boundary: 36 of its samples land in the first
  // buffer and the remaining 28 must reappear at the start of the second.
  constexpr unsigned B_IN_FIRST = BUF - FRAG;        // 36
  constexpr unsigned B_CARRIED  = FRAG - B_IN_FIRST; // 28

  const ci16_t A(0x0011, 0x0011);
  const ci16_t B(0x0022, 0x0022);
  const ci16_t C(0x0033, 0x0033);

  send_raw_fragment(FRAG, A, 0);
  send_raw_fragment(FRAG, B, FRAG);
  send_raw_fragment(FRAG, C, 2 * FRAG);

  // First buffer: all of A, then the leading part of B.
  simple_buffer_writer first(BUF);
  EXPECT_EQ(rx->receive(first).ts, 0U);
  EXPECT_EQ(first.data()[0], A);
  EXPECT_EQ(first.data()[FRAG - 1], A);
  EXPECT_EQ(first.data()[FRAG], B) << "B must begin right after A";
  EXPECT_EQ(first.data()[BUF - 1], B);

  // Second buffer: the carried tail of B, then C, then zero-fill once the peer runs dry.
  simple_buffer_writer second(BUF);
  EXPECT_EQ(rx->receive(second).ts, BUF);

  for (unsigned i = 0; i != B_CARRIED; ++i) {
    ASSERT_EQ(second.data()[i], B) << "Carried tail of B missing at index " << i;
  }
  for (unsigned i = B_CARRIED; i != B_CARRIED + FRAG; ++i) {
    ASSERT_EQ(second.data()[i], C) << "C must follow the carried tail, index " << i;
  }
  for (unsigned i = B_CARRIED + FRAG; i != BUF; ++i) {
    EXPECT_EQ(second.data()[i], ci16_t(0, 0)) << "Tail beyond the stream must be zero, index " << i;
  }

  EXPECT_TRUE(notifier.events.empty())
      << "A straddling packet is contiguous data, not a gap — no overflow should be reported";
}

// A silent uplink must still take real time, because the transmit and receive paths share a
// thread: returning instantly would spin the downlink, and stalling would starve it. Bounds are
// deliberately wide so the test does not become load-sensitive.
TEST_F(RxReassembly, SilentUplinkIsPacedAtRealTime)
{
  ASSERT_NO_FATAL_FAILURE(open(15013));

  // 10 slots at 11.52 MHz — a 10 ms budget, long enough to measure reliably.
  constexpr unsigned NOF_SAMPLES = 10 * SLOT_LEN;

  simple_buffer_writer buf(NOF_SAMPLES);

  const auto t0      = std::chrono::steady_clock::now();
  const auto meta    = rx->receive(buf);
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0);

  EXPECT_GT(elapsed.count(), 5000) << "Returned too quickly — the downlink would run faster than real time";
  EXPECT_LT(elapsed.count(), MAX_RECEIVE_DURATION.count())
      << "Blocked far beyond the buffer duration — the downlink would stall";

  EXPECT_EQ(meta.ts, 0U);
  EXPECT_EQ(rx->get_sample_count(), NOF_SAMPLES) << "Timeline must advance even with no uplink";
  EXPECT_EQ(buf.data()[0], ci16_t(0, 0));
  EXPECT_EQ(buf.data()[NOF_SAMPLES - 1], ci16_t(0, 0));
}

// Conversely, a receiver that has fallen behind the wall clock must consume what is already queued without
// waiting, so it catches up instead of pacing itself into a backlog.
TEST_F(RxReassembly, QueuedSamplesAreConsumedWithoutWaitingWhenBehind)
{
  // A slow sample rate gives the span a long pacing budget, so draining it stays far inside that budget even under
  // a sanitizer or valgrind. The span carries the same bytes at any rate.
  constexpr double SLOW_SRATE = 115200.0;
  ASSERT_NO_FATAL_FAILURE(open(15014, SLOW_SRATE));

  // One datagram anchors the timeline.
  constexpr unsigned NOF_ANCHOR_SAMPLES = radio_difi_tx_stream::MAX_PKT_SAMPLES;
  // Three slots follow. Everything is queued before the first receive() call, and the kernel caps SO_RCVBUF at
  // net.core.rmem_max, which defaults to 208 kB. Queueing past that drops datagrams silently, and the stream then
  // waits out its budget for samples that never arrive.
  constexpr unsigned NOF_SAMPLES = 3 * SLOT_LEN;
  const auto         span_budget =
      std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<double>(NOF_SAMPLES / SLOW_SRATE));

  // Target lag behind schedule. Kept small, so wake-up delays under load stay short of the 50 ms re-anchoring
  // threshold.
  constexpr std::chrono::milliseconds LAG{10};

  // Queue the anchor and the span, 19 datagrams in all, up front. Transmission is slow under valgrind, and doing it
  // here keeps it out of the lag measured below.
  transmit_ramp(NOF_ANCHOR_SAMPLES, 0);
  transmit_ramp(NOF_SAMPLES, NOF_ANCHOR_SAMPLES);

  // Both buffers exist before the timed section. Filling a large one is slow under valgrind and would add to the lag.
  simple_buffer_writer first(NOF_ANCHOR_SAMPLES);
  simple_buffer_writer buf(NOF_SAMPLES);

  // Anchor the timeline. The receiver anchors when it reads the first datagram, right after the call starts, so the
  // start time stays a reliable reference even when the call itself wakes late, as it does under valgrind.
  const auto anchor_duration = std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::duration<double>(NOF_ANCHOR_SAMPLES / SLOW_SRATE));
  const auto anchor_start = std::chrono::steady_clock::now();
  rx->receive(first);

  // Sleep until the span is overdue by LAG.
  std::this_thread::sleep_until(anchor_start + anchor_duration + span_budget + LAG);

  const auto t0 = std::chrono::steady_clock::now();
  rx->receive(buf);
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0);

  // Pacing would take the whole span budget, so anything approaching it means receive() waited rather than drained.
  EXPECT_LT(elapsed.count(), span_budget.count() / 2)
      << "Overdue queued samples must not be paced, this blocks catch-up";

  for (unsigned k = 0; k != NOF_SAMPLES; ++k) {
    ASSERT_EQ(buf.data()[k], ramp_sample(k)) << "Mismatch at sample " << k;
  }
  EXPECT_TRUE(notifier.events.empty()) << "A fully queued span has no discontinuity";
}

// Samples queued ahead of the wall clock must not be handed over early: a peer sample-locked to this receiver
// would otherwise let the pair run faster than real time, shrinking every processing deadline measured in slots.
TEST_F(RxReassembly, QueuedSamplesAheadOfTheWallClockArePaced)
{
  ASSERT_NO_FATAL_FAILURE(open(15030));

  // Four slots at 11.52 MHz, a 4 ms span, kept under the default net.core.rmem_max.
  constexpr unsigned NOF_SAMPLES = 4 * SLOT_LEN;

  // Queue the whole span up front, so all of it is available immediately.
  transmit_ramp(NOF_SAMPLES, 0);

  simple_buffer_writer buf(NOF_SAMPLES);

  const auto t0 = std::chrono::steady_clock::now();
  rx->receive(buf);
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0);

  EXPECT_GT(elapsed.count(), 3000) << "Returned before the span elapsed, the pair would run faster than real time";
  EXPECT_LT(elapsed.count(), MAX_RECEIVE_DURATION.count())
      << "Blocked far beyond the buffer duration, the downlink would stall";

  for (unsigned k = 0; k != NOF_SAMPLES; ++k) {
    ASSERT_EQ(buf.data()[k], ramp_sample(k)) << "Mismatch at sample " << k;
  }
  EXPECT_TRUE(notifier.events.empty()) << "A fully queued span has no discontinuity";
}

// ---- Fidelity round-trip tests ----------------------------------------------
//
// Pseudo-random samples through the full transmit, UDP and receive path, recovered bit-for-bit.

/// \brief Sends pseudo-random 16-bit samples through the full transmit, UDP and receive path.
///
/// Returns the sent and the received samples, with each end using its own IQ byte order.
static std::pair<std::vector<ci16_t>, std::vector<ci16_t>>
run_round_trip_16bit(difi_iq_byte_order tx_order, difi_iq_byte_order rx_order, uint16_t port)
{
  static constexpr uint32_t STREAM    = 0x00000005U;
  static constexpr double   SRATE     = 1920000.0;
  static constexpr unsigned N_SAMPLES = 64;

  null_notifier_rx notifier;

  radio_difi_rx_stream::stream_description rx_desc;
  rx_desc.ip             = "127.0.0.1";
  rx_desc.port           = port;
  rx_desc.stream_id      = STREAM;
  rx_desc.bit_depth      = 16;
  rx_desc.iq_byte_order  = rx_order;
  rx_desc.sample_rate_Hz = SRATE;
  rx_desc.stream_id_str  = "test:fidelity:rx:16";
  radio_difi_rx_stream rx_stream(rx_desc, notifier);
  rx_stream.start(0);

  radio_difi_tx_stream::stream_description tx_desc;
  tx_desc.ip             = "127.0.0.1";
  tx_desc.port           = port;
  tx_desc.stream_id      = STREAM;
  tx_desc.bit_depth      = 16;
  tx_desc.iq_byte_order  = tx_order;
  tx_desc.sample_rate_Hz = SRATE;
  tx_desc.center_freq_Hz = 0.0;
  tx_desc.stream_id_str  = "test:fidelity:tx:16";
  radio_difi_tx_stream tx_stream(tx_desc, notifier);
  tx_stream.start(0);

  // Drain the context packet emitted by start().
  simple_buffer_writer drain(1);
  rx_stream.receive(drain);

  // Generate reproducible pseudo-random int16 samples.
  std::mt19937                       rng(0xdeadbeefU);
  std::uniform_int_distribution<int> dist(std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max());
  std::vector<ci16_t>                sent(N_SAMPLES);
  for (auto& s : sent) {
    s = ci16_t(static_cast<int16_t>(dist(rng)), static_cast<int16_t>(dist(rng)));
  }

  simple_buffer_reader_rx               buf(sent);
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = false;
  // Stamped at the receiver's current position, which the drain above advanced: timestamps are
  // absolute positions on a shared clock, so a packet stamped before where the receiver has
  // reached is genuinely late. This test is about sample fidelity, not epoch handling.
  meta.ts = rx_stream.get_sample_count();
  tx_stream.transmit(buf, meta);

  simple_buffer_writer out(N_SAMPLES);
  rx_stream.receive(out);

  tx_stream.stop();
  rx_stream.stop();
  return {sent, out.data()};
}

/// Returns \p value with its two bytes swapped.
static int16_t swap_int16_bytes(int16_t value)
{
  const auto word = static_cast<uint16_t>(value);
  return static_cast<int16_t>(static_cast<uint16_t>((word << 8) | (word >> 8)));
}

/// 16-bit fidelity in big-endian byte order: arbitrary int16 I/Q values survive the round trip unchanged.
TEST(FidelityRoundTrip, RoundTrip16BitBigEndian)
{
  const auto [sent, received] = run_round_trip_16bit(difi_iq_byte_order::big, difi_iq_byte_order::big, 15003);
  EXPECT_EQ(received, sent);
}

/// 16-bit fidelity in little-endian byte order: arbitrary int16 I/Q values survive the round trip unchanged.
TEST(FidelityRoundTrip, RoundTrip16BitLittleEndian)
{
  const auto [sent, received] = run_round_trip_16bit(difi_iq_byte_order::little, difi_iq_byte_order::little, 15031);
  EXPECT_EQ(received, sent);
}

/// Ends that disagree on the byte order receive every component byte-swapped: nothing corrects a mismatch silently.
TEST(FidelityRoundTrip, MismatchedByteOrderSwapsEveryComponent)
{
  const auto [sent, received] = run_round_trip_16bit(difi_iq_byte_order::big, difi_iq_byte_order::little, 15032);
  ASSERT_EQ(received.size(), sent.size());
  for (unsigned i = 0, e = sent.size(); i != e; ++i) {
    EXPECT_EQ(received[i], ci16_t(swap_int16_bytes(sent[i].real()), swap_int16_bytes(sent[i].imag())))
        << "Sample " << i;
  }
}

/// 8-bit fidelity: only the upper byte of each int16 is transmitted on the
/// wire, so input samples must have their lower byte zeroed.  Values are
/// generated as random int8 values shifted into the upper byte.
TEST(FidelityRoundTrip, RoundTrip8Bit)
{
  static constexpr uint16_t PORT      = 15004;
  static constexpr uint32_t STREAM    = 0x00000006U;
  static constexpr double   SRATE     = 1920000.0;
  static constexpr unsigned N_SAMPLES = 64;

  null_notifier_rx notifier;

  radio_difi_rx_stream::stream_description rx_desc;
  rx_desc.ip             = "127.0.0.1";
  rx_desc.port           = PORT;
  rx_desc.stream_id      = STREAM;
  rx_desc.bit_depth      = 8;
  rx_desc.sample_rate_Hz = SRATE;
  rx_desc.stream_id_str  = "test:fidelity:rx:8";
  radio_difi_rx_stream rx_stream(rx_desc, notifier);
  rx_stream.start(0);

  radio_difi_tx_stream::stream_description tx_desc;
  tx_desc.ip             = "127.0.0.1";
  tx_desc.port           = PORT;
  tx_desc.stream_id      = STREAM;
  tx_desc.bit_depth      = 8;
  tx_desc.sample_rate_Hz = SRATE;
  tx_desc.center_freq_Hz = 0.0;
  tx_desc.stream_id_str  = "test:fidelity:tx:8";
  radio_difi_tx_stream tx_stream(tx_desc, notifier);
  tx_stream.start(0);

  // Drain the context packet.
  simple_buffer_writer drain(1);
  rx_stream.receive(drain);

  // Generate random int8 values and store them in the upper byte of int16.
  // The lower byte is 0, so the round-trip is exact (pack: >>8, unpack: <<8).
  std::mt19937                       rng(0xcafebabeU);
  std::uniform_int_distribution<int> dist(-128, 127);
  std::vector<ci16_t>                sent(N_SAMPLES);
  for (auto& s : sent) {
    s = ci16_t(static_cast<int16_t>(dist(rng) * 256), static_cast<int16_t>(dist(rng) * 256));
  }

  simple_buffer_reader_rx               buf(sent);
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = false;
  // Stamped at the receiver's current position, which the drain above advanced: timestamps are
  // absolute positions on a shared clock, so a packet stamped before where the receiver has
  // reached is genuinely late. This test is about sample fidelity, not epoch handling.
  meta.ts = rx_stream.get_sample_count();
  tx_stream.transmit(buf, meta);

  simple_buffer_writer out(N_SAMPLES);
  rx_stream.receive(out);

  for (unsigned i = 0; i < N_SAMPLES; ++i) {
    EXPECT_EQ(out.data()[i], sent[i]) << "Sample mismatch at index " << i;
  }

  tx_stream.stop();
  rx_stream.stop();
}

// ---- Configuration tests ----------------------------------------------------

/// Every scalar field carries a default, so a partially populated description never leaves the constructor reading an
/// indeterminate value into the logger level.
TEST(RxStreamConfig, DescriptionScalarDefaultsAreDeterministic)
{
  const radio_difi_rx_stream::stream_description desc;

  EXPECT_EQ(desc.port, 0U);
  EXPECT_EQ(desc.stream_id, 0U);
  EXPECT_EQ(desc.bit_depth, 16U);
  EXPECT_EQ(desc.sample_rate_Hz, 0.0);
  EXPECT_EQ(desc.log_level, ocudulog::basic_levels::info);
  EXPECT_FALSE(desc.latch_peer_epoch);
}

/// A port that is already in use must fail construction, instead of leaving the stream silently zero-filling.
TEST(RxStreamConfig, PortInUseFailsConstruction)
{
  static constexpr uint16_t PORT = 15016;

  radio_difi_udp_socket holder(test_socket_logger());
  ASSERT_TRUE(holder.open_rx("127.0.0.1", PORT));

  radio_difi_rx_stream::stream_description desc;
  desc.ip             = "127.0.0.1";
  desc.port           = PORT;
  desc.sample_rate_Hz = 1920000.0;
  desc.stream_id_str  = "test:inuse:rx";

  null_notifier_rx     notifier;
  radio_difi_rx_stream rx_stream(desc, notifier);
  EXPECT_FALSE(rx_stream.is_successful()) << "A port already in use must fail construction";
}

// ---- Start tests ------------------------------------------------------------

/// The socket is bound from construction, so a peer can queue datagrams before start(). The stream begins at its
/// initial time, so those datagrams must be discarded rather than received as current samples.
TEST(RxStreamStart, DatagramsQueuedBeforeStartAreDiscarded)
{
  static constexpr uint16_t PORT        = 15017;
  static constexpr uint32_t STREAM_ID   = 0x0000000cU;
  static constexpr double   SRATE       = 1920000.0;
  static constexpr unsigned NOF_SAMPLES = 1920;

  null_notifier_rx notifier;

  radio_difi_rx_stream::stream_description rx_desc;
  rx_desc.ip             = "127.0.0.1";
  rx_desc.port           = PORT;
  rx_desc.stream_id      = STREAM_ID;
  rx_desc.sample_rate_Hz = SRATE;
  rx_desc.stream_id_str  = "test:prestart:rx";
  radio_difi_rx_stream rx_stream(rx_desc, notifier);
  ASSERT_TRUE(rx_stream.is_successful());

  radio_difi_tx_stream::stream_description tx_desc;
  tx_desc.ip             = "127.0.0.1";
  tx_desc.port           = PORT;
  tx_desc.stream_id      = STREAM_ID;
  tx_desc.sample_rate_Hz = SRATE;
  tx_desc.stream_id_str  = "test:prestart:tx";
  radio_difi_tx_stream tx_stream(tx_desc, notifier);
  ASSERT_TRUE(tx_stream.is_successful());

  // Send non-zero samples stamped at the receiver's initial time, before the receiver starts.
  tx_stream.start(0);
  const std::vector<ci16_t>             samples(NOF_SAMPLES, ci16_t(1, 2));
  simple_buffer_reader_rx               reader(samples);
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = false;
  meta.ts       = 0;
  tx_stream.transmit(reader, meta);

  // Loopback delivery is near-immediate. The sleep only makes sure the datagram is queued before start().
  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  rx_stream.start(0);
  simple_buffer_writer buf(NOF_SAMPLES);
  rx_stream.receive(buf);

  for (unsigned k = 0; k != NOF_SAMPLES; ++k) {
    ASSERT_EQ(buf.data()[k], ci16_t(0, 0)) << "A datagram queued before start() was received, at sample " << k;
  }

  tx_stream.stop();
  rx_stream.stop();
}
