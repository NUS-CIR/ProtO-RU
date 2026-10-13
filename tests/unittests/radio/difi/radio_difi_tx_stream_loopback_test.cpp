// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// \file
/// \brief Loopback integration test for radio_difi_tx_stream: binds a UDP socket, drives start() and
/// transmit(), and validates the received bytes, serving the role of a capture without external tools.

#include "radio_difi_context_packet.h"
#include "radio_difi_data_packet.h"
#include "radio_difi_tx_stream.h"
#include "radio_difi_udp_socket.h"
#include "ocudu/gateways/baseband/buffer/baseband_gateway_buffer_reader.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/radio/radio_event_notifier.h"
#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <gtest/gtest.h>
#include <type_traits>
#include <vector>

using namespace ocudu;

/// Logger for sockets built directly by these tests.
static ocudulog::basic_logger& test_socket_logger()
{
  return ocudulog::fetch_basic_logger("difi:test", false);
}

/// How long a test waits for a datagram it expects to arrive.
static constexpr std::chrono::milliseconds RECV_TIMEOUT{100};

/// Waits up to \ref RECV_TIMEOUT for a datagram on \p sock and receives it into \p buf.
static expected<span<uint8_t>, difi_recv_error> recv_within(radio_difi_udp_socket& sock, span<uint8_t> buf)
{
  if (!sock.wait_readable(RECV_TIMEOUT)) {
    return make_unexpected(difi_recv_error::timeout);
  }
  return sock.try_recv(buf);
}

// ---- Stubs ------------------------------------------------------------------

/// No-op event notifier.
class null_notifier : public radio_event_notifier
{
public:
  void on_radio_rt_event(const event_description&) override {}
};

/// Simple in-memory buffer reader backed by a fixed vector of ci16_t samples.
class simple_buffer_reader : public baseband_gateway_buffer_reader
{
public:
  explicit simple_buffer_reader(std::vector<ci16_t> samples_) : samples(std::move(samples_)) {}

  unsigned           get_nof_channels() const override { return 1; }
  unsigned           get_nof_samples() const override { return static_cast<unsigned>(samples.size()); }
  span<const ci16_t> get_channel_buffer(unsigned /*channel_idx*/) const override { return samples; }

private:
  std::vector<ci16_t> samples;
};

// ---- Helpers ----------------------------------------------------------------

static uint32_t read_u32_be(const uint8_t* p)
{
  return (static_cast<uint32_t>(p[0]) << 24) | (static_cast<uint32_t>(p[1]) << 16) |
         (static_cast<uint32_t>(p[2]) << 8) | static_cast<uint32_t>(p[3]);
}

static uint64_t read_u64_be(const uint8_t* p)
{
  return (static_cast<uint64_t>(read_u32_be(p)) << 32) | read_u32_be(p + 4);
}

/// Reads one IQ component from the payload. Like every metadata field, the 16-bit IQ payload is big
/// endian, as VITA-49.2 puts the whole packet in network byte order.
static int16_t read_i16_iq(const uint8_t* p)
{
  return static_cast<int16_t>((static_cast<uint16_t>(p[0]) << 8) | static_cast<uint16_t>(p[1]));
}

// ---- Test fixture -----------------------------------------------------------

// ctest runs the DIFI test binaries in parallel, so no two of them may share a UDP port. This binary uses 14991,
// 15006 to 15009 and 15040 to 15042. The receive stream test uses 15001 to 15005, 15010 to 15017 and 15030. The socket
// test uses 19981 upwards, and the validator test 15050 upwards.

/// Creates a tx stream sending to 127.0.0.1:14991 and a matching Rx socket.
/// Port 14991 is chosen to avoid conflicts with any live DIFI traffic.
class TxStreamLoopback : public ::testing::Test
{
protected:
  static constexpr uint16_t TEST_PORT   = 14991;
  static constexpr uint32_t STREAM_ID   = 0x00000001U;
  static constexpr double   SAMPLE_RATE = 1920000.0;

  void SetUp() override
  {
    ASSERT_TRUE(rx_sock.open_rx("127.0.0.1", TEST_PORT)) << "Failed to open Rx socket";
    ASSERT_NO_FATAL_FAILURE(create_tx(difi_iq_byte_order::big));
  }

  /// Creates the transmit stream with the given IQ byte order, replacing any previous one.
  void create_tx(difi_iq_byte_order order)
  {
    radio_difi_tx_stream::stream_description desc;
    desc.ip             = "127.0.0.1";
    desc.port           = TEST_PORT;
    desc.stream_id      = STREAM_ID;
    desc.bit_depth      = 16;
    desc.iq_byte_order  = order;
    desc.sample_rate_Hz = SAMPLE_RATE;
    desc.center_freq_Hz = 3.5e9;
    desc.stream_id_str  = "test:tx:0";

    tx = std::make_unique<radio_difi_tx_stream>(desc, notifier);
    ASSERT_TRUE(tx->is_successful());
  }

  void TearDown() override
  {
    if (tx) {
      tx->stop();
    }
    rx_sock.close();
  }

  /// Receive one packet into \p buf. Returns number of bytes received, or 0 if none arrived.
  ssize_t recv_packet(std::vector<uint8_t>& buf)
  {
    buf.resize(65536);
    const auto received = recv_within(rx_sock, buf);
    if (!received.has_value()) {
      return 0;
    }
    buf.resize(received.value().size());
    return static_cast<ssize_t>(received.value().size());
  }

  null_notifier                         notifier;
  radio_difi_udp_socket                 rx_sock{test_socket_logger()};
  std::unique_ptr<radio_difi_tx_stream> tx;
};

// ---- Tests ------------------------------------------------------------------

TEST_F(TxStreamLoopback, StartSendsContextPacket)
{
  tx->start(0);

  std::vector<uint8_t> pkt;
  const ssize_t        n = recv_packet(pkt);

  ASSERT_GT(n, 0) << "No packet received — context packet was not sent";
  ASSERT_EQ(pkt.size(), DIFI_CONTEXT_PACKET_SIZE.value()) << "Wrong context packet size";

  // Header: type=0x4 static bits, pkt_n=0, size=27 words.
  const uint32_t header   = read_u32_be(pkt.data());
  const uint32_t expected = 0x49600000U | (0U << 16) | 27U;
  EXPECT_EQ(header, expected) << "Context packet header word mismatch";

  // Stream ID.
  EXPECT_EQ(read_u32_be(pkt.data() + 4), STREAM_ID);

  // State-and-event field at offset 96.
  EXPECT_EQ(read_u32_be(pkt.data() + 96), 0x9ff00000U);

  // Payload format for 16-bit at offset 100.
  EXPECT_EQ(read_u64_be(pkt.data() + 100), 0xa00007cf00000000ULL);
}

TEST_F(TxStreamLoopback, StartContextPacketEncodesRfFreqAndSampleRate)
{
  tx->start(0);

  std::vector<uint8_t> pkt;
  ASSERT_GT(recv_packet(pkt), 0);
  ASSERT_EQ(pkt.size(), DIFI_CONTEXT_PACKET_SIZE.value());

  // RF reference frequency at offset 52 (3.5 GHz × 2^20).
  const int64_t rf_expected = static_cast<int64_t>(3.5e9 * (1 << 20));
  const int64_t rf_actual   = static_cast<int64_t>(read_u64_be(pkt.data() + 52));
  EXPECT_EQ(rf_actual, rf_expected);

  // Sample rate at offset 76 (1920000 × 2^20).
  const uint64_t sr_expected = static_cast<uint64_t>(SAMPLE_RATE * (1 << 20));
  EXPECT_EQ(read_u64_be(pkt.data() + 76), sr_expected);
}

// DIFI timestamps are absolute UTC, so the session hands both streams an offset that shifts the
// baseband timeline - which starts near zero - onto the wall clock. Transmit must apply it to every
// timestamp it puts on the wire, or a peer reads our stream as originating in 1970.
TEST_F(TxStreamLoopback, EpochOffsetShiftsWireTimestamps)
{
  // One hour of ticks: large enough to be unambiguous, small enough to stay exact.
  const int64_t offset_ticks = static_cast<int64_t>(SAMPLE_RATE) * 3600;

  tx->start(0, offset_ticks);

  std::vector<uint8_t> ctx;
  ASSERT_GT(recv_packet(ctx), 0);
  EXPECT_EQ(read_u32_be(ctx.data() + 16), 3600U) << "Context packet must carry the shifted epoch";

  const std::vector<ci16_t>             samples = {ci16_t(1, 2)};
  simple_buffer_reader                  buf(samples);
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = false;
  meta.ts       = 0;
  tx->transmit(buf, meta);

  std::vector<uint8_t> pkt;
  ASSERT_GT(recv_packet(pkt), 0);
  EXPECT_EQ(read_u32_be(pkt.data() + 16), 3600U) << "Data packet must carry the shifted epoch";
}

TEST_F(TxStreamLoopback, TransmitSendsDataPacket)
{
  tx->start(0);
  // Drain the context packet.
  std::vector<uint8_t> ctx;
  ASSERT_GT(recv_packet(ctx), 0);

  // Transmit two known samples.
  const std::vector<ci16_t> samples = {ci16_t(0x1000, 0x2000), ci16_t(0x3000, static_cast<int16_t>(0x4000))};
  simple_buffer_reader      buf(samples);
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = false;
  meta.ts       = 0;

  tx->transmit(buf, meta);

  std::vector<uint8_t> pkt;
  const ssize_t        n = recv_packet(pkt);
  ASSERT_GT(n, 0) << "No data packet received";

  // Expected total: 28-byte header + 2 samples × 4 bytes = 36 bytes = 9 words.
  ASSERT_EQ(pkt.size(), 36U);

  // Header: DATA_STATIC_BITS | pkt_n=0 | 9 words.
  const uint32_t header = read_u32_be(pkt.data());
  EXPECT_EQ(header, 0x18600000U | (0U << 16) | 9U);

  // Stream ID.
  EXPECT_EQ(read_u32_be(pkt.data() + 4), STREAM_ID);

  // Class ID: DIFI OUI in upper word, device class 0 in lower word.
  EXPECT_EQ(read_u32_be(pkt.data() + 8), 0x006a621eU);
  EXPECT_EQ(read_u32_be(pkt.data() + 12), 0U);

  // Timestamp (ts=0 → full_secs=0, frac_ps=0).
  EXPECT_EQ(read_u32_be(pkt.data() + 16), 0U);
  EXPECT_EQ(read_u64_be(pkt.data() + 20), 0ULL);

  // IQ payload: int16 pairs in the default big-endian byte order.
  EXPECT_EQ(read_i16_iq(pkt.data() + 28), 0x1000);
  EXPECT_EQ(read_i16_iq(pkt.data() + 30), 0x2000);
  EXPECT_EQ(read_i16_iq(pkt.data() + 32), 0x3000);
  EXPECT_EQ(read_i16_iq(pkt.data() + 34), static_cast<int16_t>(0x4000));
}

// With little-endian byte order the payload carries each component least significant byte first, while the header
// stays big endian.
TEST_F(TxStreamLoopback, TransmitLittleEndianPayload)
{
  ASSERT_NO_FATAL_FAILURE(create_tx(difi_iq_byte_order::little));
  tx->start(0);
  std::vector<uint8_t> ctx;
  ASSERT_GT(recv_packet(ctx), 0);

  const std::vector<ci16_t>             samples = {ci16_t(0x1234, static_cast<int16_t>(0xabcd))};
  simple_buffer_reader                  buf(samples);
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = false;
  meta.ts       = 0;
  tx->transmit(buf, meta);

  std::vector<uint8_t> pkt;
  ASSERT_GT(recv_packet(pkt), 0) << "No data packet received";
  ASSERT_EQ(pkt.size(), 32U);
  EXPECT_EQ(read_u32_be(pkt.data() + 4), STREAM_ID);

  const std::vector<uint8_t> payload(pkt.begin() + 28, pkt.end());
  EXPECT_EQ(payload, (std::vector<uint8_t>{0x34, 0x12, 0xcd, 0xab}));
}

// An empty buffer must still go out, as silence: DIFI is a continuous stream, so omitting one leaves a
// hole, and a receiver that concatenates rather than placing by timestamp then runs slow.
TEST_F(TxStreamLoopback, TransmitEmptyIsSentAsSilence)
{
  tx->start(0);
  // Drain the context packet.
  std::vector<uint8_t> ctx;
  ASSERT_GT(recv_packet(ctx), 0);

  // The lower PHY zeroes the whole buffer before flagging it empty, so that is what is modelled here.
  simple_buffer_reader                  buf({ci16_t(0, 0), ci16_t(0, 0)});
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = true;
  meta.ts       = 0;

  tx->transmit(buf, meta);

  std::vector<uint8_t> pkt;
  ASSERT_GT(recv_packet(pkt), 0) << "An empty buffer must still occupy its span of the stream";

  // Both samples present and zero: an empty buffer is not skipped, it still occupies its span of the stream.
  ASSERT_EQ(pkt.size(), 28U + 2U * 4U);
  for (unsigned i = 0; i != 4; ++i) {
    EXPECT_EQ(read_i16_iq(pkt.data() + 28 + i * 2), 0) << "Empty buffer must be sent as zeros, index " << i;
  }
}

// The active range says which samples carry signal, not which are transmitted: the whole buffer goes
// out with the rest zeroed. Sending only the active span would shorten the stream and, since a fragment
// timestamp derives from the buffer start, stamp it one tx_start too early.
TEST_F(TxStreamLoopback, WholeBufferIsTransmittedRegardlessOfTxWindow)
{
  tx->start(0);
  std::vector<uint8_t> ctx;
  ASSERT_GT(recv_packet(ctx), 0);

  // Buffer has 4 samples and the metadata marks only [1, 3) as carrying signal. The stream transmits the buffer as
  // it stands, because the lower PHY has already zeroed whatever lies outside that window; the entries here are
  // deliberately non-zero to prove no sample is dropped or overwritten on the way out.
  const std::vector<ci16_t> samples = {
      ci16_t(0x0100, 0x0200), ci16_t(0x0a00, 0x0b00), ci16_t(0x0c00, 0x0d00), ci16_t(0x0300, 0x0400)};
  simple_buffer_reader                  buf(samples);
  baseband_gateway_transmitter_metadata meta{};
  meta.is_empty = false;
  meta.ts       = 0;
  meta.tx_start = 1;
  meta.tx_end   = 3;

  tx->transmit(buf, meta);

  std::vector<uint8_t> pkt;
  ASSERT_GT(recv_packet(pkt), 0);

  // All 4 samples -> 44 bytes total.
  ASSERT_EQ(pkt.size(), 28U + 4U * 4U);
  EXPECT_EQ(read_i16_iq(pkt.data() + 28), 0x0100) << "Sample before tx_start must be transmitted unchanged";
  EXPECT_EQ(read_i16_iq(pkt.data() + 30), 0x0200) << "Sample before tx_start must be transmitted unchanged";
  EXPECT_EQ(read_i16_iq(pkt.data() + 32), 0x0a00);
  EXPECT_EQ(read_i16_iq(pkt.data() + 34), 0x0b00);
  EXPECT_EQ(read_i16_iq(pkt.data() + 36), 0x0c00);
  EXPECT_EQ(read_i16_iq(pkt.data() + 38), 0x0d00);
  EXPECT_EQ(read_i16_iq(pkt.data() + 40), 0x0300) << "Sample at or after tx_end must be transmitted unchanged";
  EXPECT_EQ(read_i16_iq(pkt.data() + 42), 0x0400) << "Sample at or after tx_end must be transmitted unchanged";
}

// ---- Fragmentation ----------------------------------------------------------
//
// A slot exceeds the maximum UDP payload, so transmit() splits it into MAX_PKT_SAMPLES packets: six.

class TxFragmentation : public ::testing::Test
{
protected:
  static constexpr double   SRATE     = 11520000.0;
  static constexpr unsigned SLOT_LEN  = 11520;
  static constexpr uint32_t STREAM_ID = 0x0000000aU;

  /// Opens an Rx socket on \p port, creates a matching tx stream and drains the context packet.
  void open(uint16_t port)
  {
    ASSERT_TRUE(rx_sock.open_rx("127.0.0.1", port)) << "Failed to open Rx socket";

    radio_difi_tx_stream::stream_description desc;
    desc.ip             = "127.0.0.1";
    desc.port           = port;
    desc.stream_id      = STREAM_ID;
    desc.bit_depth      = 16;
    desc.sample_rate_Hz = SRATE;
    desc.center_freq_Hz = 1.8425e9;
    desc.stream_id_str  = "test:frag:tx";

    tx = std::make_unique<radio_difi_tx_stream>(desc, notifier);
    ASSERT_TRUE(tx->is_successful());

    tx->start(0);
    std::vector<uint8_t> ctx;
    ASSERT_GT(recv_packet(ctx), 0) << "Context packet from start() not received";
  }

  void TearDown() override
  {
    if (tx) {
      tx->stop();
    }
    rx_sock.close();
  }

  ssize_t recv_packet(std::vector<uint8_t>& buf)
  {
    buf.resize(65536);
    const auto received = recv_within(rx_sock, buf);
    if (!received.has_value()) {
      return 0;
    }
    buf.resize(received.value().size());
    return static_cast<ssize_t>(received.value().size());
  }

  /// Transmits a ramp of \p nof_samples where sample k is (k, -k), starting at tick \p ts.
  /// The ramp makes payload continuity across fragment boundaries directly checkable.
  void transmit_ramp(unsigned nof_samples, uint64_t ts)
  {
    std::vector<ci16_t> samples(nof_samples);
    for (unsigned k = 0; k != nof_samples; ++k) {
      samples[k] = ci16_t(static_cast<int16_t>(k), static_cast<int16_t>(-static_cast<int>(k)));
    }
    simple_buffer_reader                  buf(samples);
    baseband_gateway_transmitter_metadata meta{};
    meta.is_empty = false;
    meta.ts       = ts;
    tx->transmit(buf, meta);
  }

  /// Total datagram size for a fragment of \p nof_samples at 16-bit depth.
  static units::bytes pkt_size(unsigned nof_samples) { return DIFI_DATA_HEADER_SIZE + units::bytes(nof_samples * 4U); }

  null_notifier                         notifier;
  radio_difi_udp_socket                 rx_sock{test_socket_logger()};
  std::unique_ptr<radio_difi_tx_stream> tx;
};

TEST_F(TxFragmentation, SlotSplitsIntoWholePackets)
{
  ASSERT_NO_FATAL_FAILURE(open(15007));

  const unsigned nof_frags = SLOT_LEN / radio_difi_tx_stream::MAX_PKT_SAMPLES;
  ASSERT_EQ(nof_frags, 6U) << "A 11520-sample slot must split into exactly 6 fragments of 1920";

  transmit_ramp(SLOT_LEN, 0);

  // Exact fragment timestamps: floor(i * 1920 * 1e12 / 11520000) picoseconds. Fragment 3 falls
  // on 0.5 ms exactly; the rest are recurring fractions truncated towards zero.
  const uint64_t expected_frac_ps[6] = {0, 166666666, 333333333, 500000000, 666666666, 833333333};

  for (unsigned i = 0; i != nof_frags; ++i) {
    std::vector<uint8_t> pkt;
    ASSERT_GT(recv_packet(pkt), 0) << "Fragment " << i << " was not sent";
    ASSERT_EQ(pkt.size(), pkt_size(radio_difi_tx_stream::MAX_PKT_SAMPLES).value()) << "Size of fragment " << i;

    // Header carries the mod-16 packet counter and the total length in 32-bit words.
    const auto     words  = static_cast<uint32_t>(pkt.size() / 4U);
    const uint32_t header = read_u32_be(pkt.data());
    EXPECT_EQ(header, 0x18600000U | (i << 16) | words) << "Header word of fragment " << i;

    EXPECT_EQ(read_u32_be(pkt.data() + 4), STREAM_ID) << "Stream ID of fragment " << i;

    // Every fragment is stamped with the tick of its own first sample.
    EXPECT_EQ(read_u32_be(pkt.data() + 16), 0U) << "full_secs of fragment " << i;
    EXPECT_EQ(read_u64_be(pkt.data() + 20), expected_frac_ps[i]) << "frac_ps of fragment " << i;

    // Payload is contiguous: fragment i must begin at ramp sample i * 1920.
    const auto first = static_cast<int16_t>(i * radio_difi_tx_stream::MAX_PKT_SAMPLES);
    EXPECT_EQ(read_i16_iq(pkt.data() + 28), first) << "First I sample of fragment " << i;
    EXPECT_EQ(read_i16_iq(pkt.data() + 30), static_cast<int16_t>(-first)) << "First Q of fragment " << i;
  }

  std::vector<uint8_t> extra;
  EXPECT_EQ(recv_packet(extra), 0) << "Unexpected packet beyond the six fragments";
}

TEST_F(TxFragmentation, ShortFinalFragment)
{
  ASSERT_NO_FATAL_FAILURE(open(15008));

  // One full fragment plus a remainder that must go out as a shorter packet.
  constexpr unsigned TAIL = 500;
  transmit_ramp(radio_difi_tx_stream::MAX_PKT_SAMPLES + TAIL, 0);

  std::vector<uint8_t> first;
  ASSERT_GT(recv_packet(first), 0) << "First fragment was not sent";
  EXPECT_EQ(first.size(), pkt_size(radio_difi_tx_stream::MAX_PKT_SAMPLES).value());

  std::vector<uint8_t> tail;
  ASSERT_GT(recv_packet(tail), 0) << "Short final fragment was not sent";
  EXPECT_EQ(tail.size(), pkt_size(TAIL).value()) << "Final fragment must carry only the remaining samples";

  // Stamped one full fragment later, and the next packet number in sequence.
  EXPECT_EQ(read_u64_be(tail.data() + 20), 166666666ULL);
  EXPECT_EQ((read_u32_be(tail.data()) >> 16) & 0xfU, 1U);

  // Its payload resumes exactly where the first fragment ended.
  EXPECT_EQ(read_i16_iq(tail.data() + 28), static_cast<int16_t>(radio_difi_tx_stream::MAX_PKT_SAMPLES));

  std::vector<uint8_t> extra;
  EXPECT_EQ(recv_packet(extra), 0) << "Unexpected third packet";
}

TEST_F(TxFragmentation, ExactlyOnePacketIsNotFragmented)
{
  ASSERT_NO_FATAL_FAILURE(open(15009));

  transmit_ramp(radio_difi_tx_stream::MAX_PKT_SAMPLES, 0);

  std::vector<uint8_t> pkt;
  ASSERT_GT(recv_packet(pkt), 0);
  EXPECT_EQ(pkt.size(), pkt_size(radio_difi_tx_stream::MAX_PKT_SAMPLES).value());

  std::vector<uint8_t> extra;
  EXPECT_EQ(recv_packet(extra), 0) << "A buffer of exactly MAX_PKT_SAMPLES must produce one packet";
}

// ---- PCAP writer ------------------------------------------------------------
//
// Minimal libpcap writer. IPv4 and UDP headers let Wireshark dissect the packets; LINKTYPE_RAW (101).

class pcap_writer
{
public:
  pcap_writer() = default;
  ~pcap_writer() { close(); }
  pcap_writer(const pcap_writer&)            = delete;
  pcap_writer& operator=(const pcap_writer&) = delete;

  /// Open \p path for writing and emit the global pcap header.
  /// Returns false if the file cannot be opened.
  bool open(const std::string& path)
  {
    file = std::fopen(path.c_str(), "wb");
    if (!file) {
      return false;
    }
    // Global header — all fields written in host byte order; the magic number
    // tells Wireshark which endianness to use when reading.
    write_u32(0xa1b2c3d4); // magic
    write_u16(2);          // version major
    write_u16(4);          // version minor
    write_u32(0);          // UTC offset
    write_u32(0);          // timestamp accuracy
    write_u32(65535);      // snap length
    write_u32(101);        // LINKTYPE_RAW (IPv4)
    return true;
  }

  /// Wrap \p payload in an IPv4/UDP frame and append it to the pcap.
  /// \p pkt_index is used to space timestamps 1 ms apart for readability.
  void write_udp_packet(uint16_t       src_port,
                        uint16_t       dst_port,
                        const uint8_t* payload,
                        size_t         payload_len,
                        unsigned       pkt_index = 0,
                        uint32_t       src_ip    = 0x7f000001U,
                        uint32_t       dst_ip    = 0x7f000001U)
  {
    // An oversized payload does not fit an IPv4 datagram, and would overrun frame below.
    if (file == nullptr || payload_len > MAX_UDP_PAYLOAD_BYTES) {
      return;
    }

    const auto udp_len      = static_cast<uint16_t>(UDP_HEADER_BYTES + payload_len);
    const auto ip_total_len = static_cast<uint16_t>(IPV4_HEADER_BYTES + udp_len);

    uint8_t frame[IPV4_HEADER_BYTES + UDP_HEADER_BYTES + MAX_UDP_PAYLOAD_BYTES];

    // ---- IPv4 header (20 bytes) ----
    frame[0]  = 0x45; // version=4, IHL=5
    frame[1]  = 0;    // DSCP/ECN
    frame[2]  = static_cast<uint8_t>(ip_total_len >> 8);
    frame[3]  = static_cast<uint8_t>(ip_total_len);
    frame[4]  = static_cast<uint8_t>(++ip_id_ >> 8);
    frame[5]  = static_cast<uint8_t>(ip_id_);
    frame[6]  = 0x40; // don't fragment
    frame[7]  = 0;    // fragment offset
    frame[8]  = 64;   // TTL
    frame[9]  = 17;   // protocol: UDP
    frame[10] = 0;    // checksum (computed below)
    frame[11] = 0;
    frame[12] = static_cast<uint8_t>(src_ip >> 24);
    frame[13] = static_cast<uint8_t>(src_ip >> 16);
    frame[14] = static_cast<uint8_t>(src_ip >> 8);
    frame[15] = static_cast<uint8_t>(src_ip);
    frame[16] = static_cast<uint8_t>(dst_ip >> 24);
    frame[17] = static_cast<uint8_t>(dst_ip >> 16);
    frame[18] = static_cast<uint8_t>(dst_ip >> 8);
    frame[19] = static_cast<uint8_t>(dst_ip);

    // Compute IPv4 header checksum (one's complement of 16-bit word sum).
    uint32_t sum = 0;
    for (int i = 0; i < 20; i += 2) {
      sum += (static_cast<uint32_t>(frame[i]) << 8) | frame[i + 1];
    }
    while (sum >> 16) {
      sum = (sum & 0xffffU) + (sum >> 16);
    }
    const auto cksum = static_cast<uint16_t>(~sum);
    frame[10]        = static_cast<uint8_t>(cksum >> 8);
    frame[11]        = static_cast<uint8_t>(cksum);

    // ---- UDP header (8 bytes) ----
    frame[20] = static_cast<uint8_t>(src_port >> 8);
    frame[21] = static_cast<uint8_t>(src_port);
    frame[22] = static_cast<uint8_t>(dst_port >> 8);
    frame[23] = static_cast<uint8_t>(dst_port);
    frame[24] = static_cast<uint8_t>(udp_len >> 8);
    frame[25] = static_cast<uint8_t>(udp_len);
    frame[26] = 0; // checksum — optional for IPv4, set to 0
    frame[27] = 0;

    // ---- Payload ----
    std::memcpy(frame + 28, payload, payload_len);

    const size_t total = 28 + payload_len;

    // ---- pcap per-packet header ----
    const auto ts_sec  = static_cast<uint32_t>(std::time(nullptr));
    const auto ts_usec = static_cast<uint32_t>(pkt_index * 1000U); // 1 ms apart
    write_u32(ts_sec);
    write_u32(ts_usec);
    write_u32(static_cast<uint32_t>(total));
    write_u32(static_cast<uint32_t>(total));

    std::fwrite(frame, 1, total, file);
  }

  void close()
  {
    if (file) {
      std::fflush(file);
      std::fclose(file);
      file = nullptr;
    }
  }

private:
  /// IPv4 header length in bytes, without options.
  static constexpr size_t IPV4_HEADER_BYTES = 20;
  /// UDP header length in bytes.
  static constexpr size_t UDP_HEADER_BYTES = 8;
  /// Largest UDP payload that fits an IPv4 datagram: 65535 bytes in total, less both headers.
  static constexpr size_t MAX_UDP_PAYLOAD_BYTES = 65535 - IPV4_HEADER_BYTES - UDP_HEADER_BYTES;

  void write_u16(uint16_t v) { std::fwrite(&v, 2, 1, file); }
  void write_u32(uint32_t v) { std::fwrite(&v, 4, 1, file); }

  FILE*    file   = nullptr;
  uint16_t ip_id_ = 0;
};

static_assert(!std::is_copy_constructible_v<pcap_writer>, "pcap_writer owns a FILE* and must not be copied.");
static_assert(!std::is_copy_assignable_v<pcap_writer>, "pcap_writer owns a FILE* and must not be copied.");

// ---- Pcap capture test ------------------------------------------------------

TEST_F(TxStreamLoopback, SetFreqSendsUpdatedContextPacket)
{
  tx->start(0);
  // Drain the startup context packet.
  std::vector<uint8_t> ctx;
  ASSERT_GT(recv_packet(ctx), 0);

  // Change frequency and expect a new context packet.
  const double new_freq_Hz = 2.6e9;
  tx->set_freq(new_freq_Hz);

  std::vector<uint8_t> pkt;
  ASSERT_GT(recv_packet(pkt), 0);
  ASSERT_EQ(pkt.size(), DIFI_CONTEXT_PACKET_SIZE.value());

  // rf_ref_freq at offset 52 should reflect the new frequency.
  const int64_t expected = static_cast<int64_t>(new_freq_Hz * (1 << 20));
  EXPECT_EQ(static_cast<int64_t>(read_u64_be(pkt.data() + 52)), expected);
}

TEST_F(TxStreamLoopback, SetGainSendsUpdatedContextPacket)
{
  tx->start(0);
  std::vector<uint8_t> ctx;
  ASSERT_GT(recv_packet(ctx), 0);

  // Set RF gain to 6 dB, IF gain to 3 dB.
  tx->set_gain(6.0, 3.0);

  std::vector<uint8_t> pkt;
  ASSERT_GT(recv_packet(pkt), 0);
  ASSERT_EQ(pkt.size(), DIFI_CONTEXT_PACKET_SIZE.value());

  // Gains field at offset 72: upper 16 = if_gain*128, lower 16 = rf_gain*128.
  const uint32_t gains     = read_u32_be(pkt.data() + 72);
  const int16_t  rf_actual = static_cast<int16_t>(gains & 0xffffU);
  const int16_t  if_actual = static_cast<int16_t>(gains >> 16);
  EXPECT_EQ(rf_actual, static_cast<int16_t>(6.0 * 128));
  EXPECT_EQ(if_actual, static_cast<int16_t>(3.0 * 128));
}

// Disabled by default: this writes a capture for inspection in Wireshark rather than asserting on it.
// Run with --gtest_also_run_disabled_tests to produce the file.
TEST_F(TxStreamLoopback, DISABLED_WritePcapFile)
{
  static constexpr const char* PCAP_PATH = "/tmp/difi_loopback.pcap";
  static constexpr uint16_t    SRC_PORT  = 12345;
  static constexpr uint16_t    DIFI_PORT = 14991;

  pcap_writer writer;
  ASSERT_TRUE(writer.open(PCAP_PATH)) << "Could not open " << PCAP_PATH << " for writing";

  unsigned pkt_index = 0;

  // ---- Context packet (sent by start()) ----
  tx->start(0);

  std::vector<uint8_t> ctx_pkt;
  ASSERT_GT(recv_packet(ctx_pkt), 0) << "Context packet not received";
  writer.write_udp_packet(SRC_PORT, DIFI_PORT, ctx_pkt.data(), ctx_pkt.size(), pkt_index++);

  // ---- Data packets with varied IQ samples ----
  // 8 samples covering positive, negative, and boundary values.
  const std::vector<ci16_t> samples = {
      ci16_t(0x0100, 0x0200),
      ci16_t(0x0300, 0x0400),
      ci16_t(0x0500, 0x0600),
      ci16_t(0x0700, static_cast<int16_t>(0x0800)),
      ci16_t(static_cast<int16_t>(-0x0100), static_cast<int16_t>(-0x0200)),
      ci16_t(static_cast<int16_t>(-0x0300), static_cast<int16_t>(-0x0400)),
      ci16_t(0x7f00, static_cast<int16_t>(0x8000)),
      ci16_t(0, 0),
  };

  for (int i = 0; i < 3; ++i) {
    simple_buffer_reader                  buf(samples);
    baseband_gateway_transmitter_metadata meta{};
    meta.is_empty = false;
    meta.ts = static_cast<baseband_gateway_timestamp>(i) * static_cast<baseband_gateway_timestamp>(samples.size());

    tx->transmit(buf, meta);

    std::vector<uint8_t> data_pkt;
    ASSERT_GT(recv_packet(data_pkt), 0) << "Data packet " << i << " not received";
    writer.write_udp_packet(SRC_PORT, DIFI_PORT, data_pkt.data(), data_pkt.size(), pkt_index++);
  }

  writer.close();
  std::printf("\n[WritePcapFile] PCAP written to: %s\n"
              "               Open with: wireshark %s\n",
              PCAP_PATH,
              PCAP_PATH);
}

// ---- Event notification tests -----------------------------------------------

/// Notifier that records every event it receives.
class capturing_notifier : public radio_event_notifier
{
public:
  void on_radio_rt_event(const event_description& ev) override { events.push_back(ev); }

  std::vector<event_description> events;
};

/// Underflow: when transmit() is called with a timestamp gap, an UNDERFLOW event
/// must be fired before the packet is sent.
TEST(TxStreamEvents, UnderflowFiredOnTimestampGap)
{
  static constexpr uint16_t PORT      = 15006;
  static constexpr uint32_t STREAM    = 0x00000009U;
  static constexpr double   SRATE     = 1920000.0;
  static constexpr unsigned N_SAMPLES = 4;

  // Rx socket to drain outgoing packets so the socket send() doesn't block.
  radio_difi_udp_socket drain_sock(test_socket_logger());
  ASSERT_TRUE(drain_sock.open_rx("127.0.0.1", PORT));

  capturing_notifier notifier;

  radio_difi_tx_stream::stream_description tx_desc;
  tx_desc.ip             = "127.0.0.1";
  tx_desc.port           = PORT;
  tx_desc.stream_id      = STREAM;
  tx_desc.bit_depth      = 16;
  tx_desc.sample_rate_Hz = SRATE;
  tx_desc.center_freq_Hz = 0.0;
  tx_desc.stream_id_str  = "test:underflow:tx";
  radio_difi_tx_stream tx_stream(tx_desc, notifier);
  tx_stream.start(0); // last_tx_end_ts = 0

  // Drain context packet sent by start().
  std::vector<uint8_t> ctx(65536);
  recv_within(drain_sock, ctx);

  // First transmit: ts=0, N_SAMPLES samples → last_tx_end_ts = N_SAMPLES.
  // ts (0) == last_tx_end_ts (0) → no underflow.
  const std::vector<ci16_t>             s1(N_SAMPLES, ci16_t(1, 2));
  simple_buffer_reader                  buf1(s1);
  baseband_gateway_transmitter_metadata meta1{};
  meta1.is_empty = false;
  meta1.ts       = 0;
  tx_stream.transmit(buf1, meta1);
  recv_within(drain_sock, ctx);
  EXPECT_TRUE(notifier.events.empty()) << "No underflow expected for first contiguous packet";

  // Second transmit: ts = N_SAMPLES + 10 (gap of 10) → underflow must fire.
  const std::vector<ci16_t>             s2(N_SAMPLES, ci16_t(3, 4));
  simple_buffer_reader                  buf2(s2);
  baseband_gateway_transmitter_metadata meta2{};
  meta2.is_empty = false;
  meta2.ts       = static_cast<baseband_gateway_timestamp>(N_SAMPLES + 10);
  tx_stream.transmit(buf2, meta2);

  ASSERT_EQ(notifier.events.size(), 1U) << "Expected exactly one UNDERFLOW event";
  EXPECT_EQ(notifier.events[0].type, radio_event_type::UNDERFLOW);
  EXPECT_EQ(notifier.events[0].source, radio_event_source::TRANSMIT);
  EXPECT_EQ(notifier.events[0].stream_id, static_cast<unsigned>(STREAM));

  tx_stream.stop();
  drain_sock.close();
}

// ---- Configuration tests ----------------------------------------------------

/// Every scalar field carries a default, so a partially populated description never leaves the
/// constructor reading an indeterminate value into the logger level or the context packet gains.
TEST(TxStreamConfig, DescriptionScalarDefaultsAreDeterministic)
{
  const radio_difi_tx_stream::stream_description desc;

  EXPECT_EQ(desc.port, 0U);
  EXPECT_EQ(desc.stream_id, 0U);
  EXPECT_EQ(desc.bit_depth, 16U);
  EXPECT_EQ(desc.sample_rate_Hz, 0.0);
  EXPECT_EQ(desc.center_freq_Hz, 0.0);
  EXPECT_EQ(desc.rf_gain_dB, 0.0);
  EXPECT_EQ(desc.if_gain_dB, 0.0);
  EXPECT_EQ(desc.log_level, ocudulog::basic_levels::info);
}

/// An unsupported IQ bit depth comes from user configuration, so it must fail construction rather
/// than trip an assert: the assert is compiled out unless ASSERTS_ENABLED is set, and the packet
/// builder would then size and fill the payload inconsistently.
TEST(TxStreamConfig, UnsupportedBitDepthIsRejected)
{
  static constexpr uint16_t PORT = 15040;

  // Bound first, so any packet a rejected stream wrongly sent would be waiting to be read.
  radio_difi_udp_socket drain_sock(test_socket_logger());
  ASSERT_TRUE(drain_sock.open_rx("127.0.0.1", PORT));

  radio_difi_tx_stream::stream_description desc;
  desc.ip            = "127.0.0.1";
  desc.port          = PORT;
  desc.bit_depth     = 12;
  desc.stream_id_str = "test:baddepth:tx";

  null_notifier        notifier;
  radio_difi_tx_stream tx_stream(desc, notifier);
  EXPECT_FALSE(tx_stream.is_successful()) << "An unsupported bit depth must fail construction";

  // A rejected stream stays silent even if the caller ignores is_successful().
  tx_stream.start(0);

  std::vector<uint8_t> pkt(65536);
  EXPECT_FALSE(recv_within(drain_sock, pkt).has_value()) << "A rejected stream must not put a packet on the wire";

  tx_stream.stop();
  drain_sock.close();
}

/// Both documented depths are accepted.
/// An unusable destination address comes from user configuration, so it must fail construction instead of leaving
/// the stream silent.
TEST(TxStreamConfig, InvalidAddressFailsConstruction)
{
  radio_difi_tx_stream::stream_description desc;
  desc.ip            = "not.an.address";
  desc.port          = 15042;
  desc.stream_id_str = "test:badaddr:tx";

  null_notifier        notifier;
  radio_difi_tx_stream tx_stream(desc, notifier);
  EXPECT_FALSE(tx_stream.is_successful()) << "An invalid address must fail construction";
}

TEST(TxStreamConfig, SupportedBitDepthsAreAccepted)
{
  null_notifier notifier;

  for (unsigned bit_depth : {8U, 16U}) {
    radio_difi_tx_stream::stream_description desc;
    desc.ip            = "127.0.0.1";
    desc.port          = 15041;
    desc.bit_depth     = bit_depth;
    desc.stream_id_str = "test:depth:tx";

    radio_difi_tx_stream tx_stream(desc, notifier);
    EXPECT_TRUE(tx_stream.is_successful()) << "Bit depth " << bit_depth << " must be accepted";
  }
}
