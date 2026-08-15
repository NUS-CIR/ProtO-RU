// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/serdes/ofh_cplane_message_builder_dynamic_compression_impl.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/serdes/ofh_serdes_factories.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

static constexpr subcarrier_spacing test_scs       = subcarrier_spacing::kHz30;
static constexpr unsigned           test_sector_id = 0;

static slot_point make_slot()
{
  // SFN 5, subframe 3, slot 1 (valid for 30 kHz, which has 2 slots per subframe).
  return slot_point(to_numerology_value(test_scs), 5, 3, 1);
}

static void check_radio_header_round_trip(const cplane_radio_application_header& in,
                                          const cplane_message_decoder_results&  out)
{
  ASSERT_EQ(in.direction, out.radio_hdr.direction);
  ASSERT_EQ(in.filter_index, out.radio_hdr.filter_index);
  ASSERT_EQ(in.start_symbol, out.radio_hdr.start_symbol);
  ASSERT_EQ(in.slot, out.radio_hdr.slot);
}

static void check_common_section_round_trip(const cplane_common_section_0_1_3_5_fields& in,
                                            const cplane_common_section_0_1_3_5_fields& out)
{
  ASSERT_EQ(in.section_id, out.section_id);
  ASSERT_EQ(in.prb_start, out.prb_start);
  ASSERT_EQ(in.nof_prb, out.nof_prb);
  ASSERT_EQ(in.re_mask, out.re_mask);
  ASSERT_EQ(in.nof_symbols, out.nof_symbols);
}

TEST(ofh_cplane_packet_decoder_impl_test, section_type1_uplink_round_trip)
{
  cplane_section_type1_parameters params;
  params.radio_hdr.direction          = data_direction::uplink;
  params.radio_hdr.filter_index       = filter_index_type::standard_channel_filter;
  params.radio_hdr.slot               = make_slot();
  params.radio_hdr.start_symbol       = 4;
  params.section_fields.common_fields = {/*section_id=*/0x123,
                                         /*prb_start=*/0x101,
                                         /*nof_prb=*/52,
                                         /*re_mask=*/0xfff,
                                         /*nof_symbols=*/14};
  params.compr_params                 = {compression_type::BFP, 9};

  std::vector<uint8_t>                            buffer(64, 0);
  cplane_message_builder_dynamic_compression_impl builder;
  unsigned                                        nof_bytes = builder.build_dl_ul_radio_channel_message(buffer, params);

  auto decoder =
      create_ofh_control_plane_message_decoder(ocudulog::fetch_basic_logger("TEST"), test_scs, test_sector_id);
  cplane_message_decoder_results results;
  ASSERT_TRUE(decoder->decode(results, span<const uint8_t>(buffer).first(nof_bytes)));

  ASSERT_EQ(1, results.section_type);
  check_radio_header_round_trip(params.radio_hdr, results);
  check_common_section_round_trip(params.section_fields.common_fields, results.section);
  // Dynamic uplink carries the real udCompHdr.
  ASSERT_EQ(params.compr_params.type, results.compr_params.type);
  ASSERT_EQ(params.compr_params.data_width, results.compr_params.data_width);
}

TEST(ofh_cplane_packet_decoder_impl_test, section_type1_downlink_round_trip)
{
  cplane_section_type1_parameters params;
  params.radio_hdr.direction          = data_direction::downlink;
  params.radio_hdr.filter_index       = filter_index_type::standard_channel_filter;
  params.radio_hdr.slot               = make_slot();
  params.radio_hdr.start_symbol       = 0;
  params.section_fields.common_fields = {/*section_id=*/7,
                                         /*prb_start=*/0,
                                         /*nof_prb=*/0,
                                         /*re_mask=*/0xfff,
                                         /*nof_symbols=*/14};
  params.compr_params                 = {compression_type::BFP, 9};

  std::vector<uint8_t>                            buffer(64, 0);
  cplane_message_builder_dynamic_compression_impl builder;
  unsigned                                        nof_bytes = builder.build_dl_ul_radio_channel_message(buffer, params);

  auto decoder =
      create_ofh_control_plane_message_decoder(ocudulog::fetch_basic_logger("TEST"), test_scs, test_sector_id);
  cplane_message_decoder_results results;
  ASSERT_TRUE(decoder->decode(results, span<const uint8_t>(buffer).first(nof_bytes)));

  ASSERT_EQ(1, results.section_type);
  check_radio_header_round_trip(params.radio_hdr, results);
  check_common_section_round_trip(params.section_fields.common_fields, results.section);
}

TEST(ofh_cplane_packet_decoder_impl_test, reject_multiple_sections)
{
  cplane_section_type1_parameters params;
  params.radio_hdr.direction          = data_direction::downlink;
  params.radio_hdr.filter_index       = filter_index_type::standard_channel_filter;
  params.radio_hdr.slot               = make_slot();
  params.section_fields.common_fields = {/*section_id=*/0,
                                         /*prb_start=*/0,
                                         /*nof_prb=*/1,
                                         /*re_mask=*/0xfff,
                                         /*nof_symbols=*/1};

  std::vector<uint8_t>                            buffer(64, 0);
  cplane_message_builder_dynamic_compression_impl builder;
  unsigned                                        nof_bytes = builder.build_dl_ul_radio_channel_message(buffer, params);
  buffer[4]                                                 = 2;

  auto decoder =
      create_ofh_control_plane_message_decoder(ocudulog::fetch_basic_logger("TEST"), test_scs, test_sector_id);
  cplane_message_decoder_results results;
  ASSERT_FALSE(decoder->decode(results, span<const uint8_t>(buffer).first(nof_bytes)));
}

TEST(ofh_cplane_packet_decoder_impl_test, section_type3_prach_round_trip)
{
  cplane_section_type3_parameters params;
  params.radio_hdr.direction             = data_direction::uplink;
  params.radio_hdr.filter_index          = filter_index_type::ul_prach_preamble_1p25khz;
  params.radio_hdr.slot                  = make_slot();
  params.radio_hdr.start_symbol          = 2;
  params.section_fields.common_fields    = {/*section_id=*/9,
                                         /*prb_start=*/12,
                                         /*nof_prb=*/24,
                                         /*re_mask=*/0xfff,
                                         /*nof_symbols=*/1};
  params.section_fields.frequency_offset = -300;
  params.compr_params                    = {compression_type::BFP, 9};
  params.scs                             = cplane_scs::kHz30;
  params.fft_size                        = cplane_fft_size::fft_1024;
  params.cpLength                        = 0;
  params.time_offset                     = 25;

  std::vector<uint8_t>                            buffer(64, 0);
  cplane_message_builder_dynamic_compression_impl builder;
  unsigned nof_bytes = builder.build_prach_mixed_numerology_message(buffer, params);

  auto decoder =
      create_ofh_control_plane_message_decoder(ocudulog::fetch_basic_logger("TEST"), test_scs, test_sector_id);
  cplane_message_decoder_results results;
  ASSERT_TRUE(decoder->decode(results, span<const uint8_t>(buffer).first(nof_bytes)));

  ASSERT_EQ(3, results.section_type);
  check_radio_header_round_trip(params.radio_hdr, results);
  check_common_section_round_trip(params.section_fields.common_fields, results.section);
  ASSERT_EQ(params.time_offset, results.time_offset);
  ASSERT_EQ(params.scs, results.frame_structure_scs);
  ASSERT_EQ(params.fft_size, results.fft_size);
  ASSERT_EQ(params.section_fields.frequency_offset, results.frequency_offset);
  ASSERT_EQ(params.compr_params.type, results.compr_params.type);
  ASSERT_EQ(params.compr_params.data_width, results.compr_params.data_width);
}

TEST(ofh_cplane_packet_decoder_impl_test, section_type0_idle_guard_round_trip)
{
  cplane_section_type0_parameters params;
  params.radio_hdr.direction          = data_direction::downlink;
  params.radio_hdr.filter_index       = filter_index_type::standard_channel_filter;
  params.radio_hdr.slot               = make_slot();
  params.radio_hdr.start_symbol       = 0;
  params.section_fields.common_fields = {/*section_id=*/0,
                                         /*prb_start=*/0,
                                         /*nof_prb=*/0,
                                         /*re_mask=*/0xfff,
                                         /*nof_symbols=*/14};
  params.scs                          = subcarrier_spacing::kHz30;
  params.cp                           = cyclic_prefix::NORMAL;
  params.fft_size                     = cplane_fft_size::fft_noop;
  params.time_offset                  = 10;

  std::vector<uint8_t>                            buffer(64, 0);
  cplane_message_builder_dynamic_compression_impl builder;
  unsigned                                        nof_bytes = builder.build_idle_guard_period_message(buffer, params);

  auto decoder =
      create_ofh_control_plane_message_decoder(ocudulog::fetch_basic_logger("TEST"), test_scs, test_sector_id);
  cplane_message_decoder_results results;
  ASSERT_TRUE(decoder->decode(results, span<const uint8_t>(buffer).first(nof_bytes)));

  ASSERT_EQ(0, results.section_type);
  check_radio_header_round_trip(params.radio_hdr, results);
  check_common_section_round_trip(params.section_fields.common_fields, results.section);
  ASSERT_EQ(params.time_offset, results.time_offset);
  // The builder encodes the configured subcarrier spacing into the frame structure field.
  ASSERT_EQ(cplane_scs::kHz30, results.frame_structure_scs);
}
